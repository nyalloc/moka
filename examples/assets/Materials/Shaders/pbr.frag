#version 330

#moka_compilation_flags
#define PI 3.1415926535897932384626433832795

struct pbr_material 
{
    vec4 diffuse_factor;    // default = vec3(1.0, 1.0, 1.0, 1.0)    
    vec4 emissive_factor;   // default = vec3(0.0, 0.0, 0.0, 1.0)  
    float roughness_factor; // default = 1.0f 
    float metalness_factor; // default = 1.0f 
    
    #ifdef DIFFUSE_MAP  
        sampler2D diffuse_map;
    #endif

    #ifdef NORMAL_MAP  
        sampler2D normal_map;
    #endif
    
    #ifdef EMISSIVE_MAP 
        sampler2D emissive_map;
    #endif

    #ifdef METALLIC_ROUGHNESS_MAP 
        sampler2D metallic_roughness_map;
    #endif

    #ifdef AO_MAP        
        sampler2D ao_map;
    #endif
    
    #ifdef MASK_ALPHA
        float alpha_cutoff;
    #endif
};

// IBL
uniform samplerCube irradiance_map;
uniform samplerCube prefilter_map;
uniform sampler2D brdf_lut;
uniform float gamma;
uniform float exposure;

in vec3 in_frag_pos;  
in vec3 in_normal;  
in vec2 in_texture_coord;
out vec4 frag_color;

#ifdef NORMAL_MAP 
    in mat3 tbn_matrix;
    in float green_channel_scalar;
#endif

uniform vec3 view_pos;
uniform pbr_material material;

vec4 get_albedo()
{
    #ifdef DIFFUSE_MAP
        return texture(material.diffuse_map, in_texture_coord).rgba;
    #else 
        return material.diffuse_factor;
    #endif
}

vec3 get_normal()
{
    #ifdef NORMAL_MAP
        vec3 material_normal = texture(material.normal_map, in_texture_coord).rgb;
        material_normal = normalize(material_normal * 2.0 - 1.0); 
        material_normal = normalize(tbn_matrix * material_normal);

        // green channel scalar is a dot product between the cross of the bitangent and tangents and the surface normal.
        // it will inform us if the texture has been flipped horizontally. If it has, we need to flip the green component.
        // otherwise the lighting calculations will be all wrong! They end up the opposite y direction.
    #else 
        vec3 material_normal = in_normal;
    #endif
    
    return normalize(material_normal);
}

vec4 get_emissive()
{
    #ifdef EMISSIVE_MAP
        return texture(material.emissive_map, in_texture_coord).rgba * material.emissive_factor;
    #else 
        return material.emissive_factor;
    #endif
}

float get_roughness()
{
    #ifdef METALLIC_ROUGHNESS_MAP
        return texture(material.metallic_roughness_map, in_texture_coord).g * material.roughness_factor;
    #else
        return material.roughness_factor;
    #endif
}

float get_metallic()
{
    #ifdef METALLIC_ROUGHNESS_MAP
        return texture(material.metallic_roughness_map, in_texture_coord).b * material.metalness_factor;
    #else 
        return material.metalness_factor;
    #endif
}

float get_ao()
{
    #ifdef AO_MAP
        return texture(material.ao_map, in_texture_coord).r;
    #else 
        return 1.0f;
    #endif
}

bool discard_fragment()
{
    #ifdef MASK_ALPHA
        return texture(material.diffuse_map, in_texture_coord).a < material.alpha_cutoff;
    #else
        return false;
    #endif  
}

float distribution_ggx(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float n_dot_h = max(dot(N, H), 0.0);
    float n_dot_h2 = n_dot_h * n_dot_h;

    float nom   = a2;
    float denom = (n_dot_h2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float geometry_schlick_ggx(float n_dot_v, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float nom   = n_dot_v;
    float denom = n_dot_v * (1.0 - k) + k;

    return nom / denom;
}

float geometry_smith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float n_dot_v = max(dot(N, V), 0.0);
    float n_dot_l = max(dot(N, L), 0.0);
    float ggx2 = geometry_schlick_ggx(n_dot_v, roughness);
    float ggx1 = geometry_schlick_ggx(n_dot_l, roughness);

    return ggx1 * ggx2;
}

vec3 fresnel_schlick(float cos_theta, vec3 f0)
{
    return f0 + (1.0 - f0) * pow(1.0 - cos_theta, 5.0);
}

vec3 fresnel_schlick_roughness(float cos_theta, vec3 f0, float roughness)
{
    return f0 + (max(vec3(1.0 - roughness), f0) - f0) * pow(1.0 - cos_theta, 5.0);
}   

void main()
{
    if(discard_fragment())
    {
        discard;
    }  
        
	vec4 albedo = get_albedo();
	vec4 emissive = get_emissive();
	float metallic = get_metallic();
	float roughness = get_roughness();
	float ao = get_ao();
	
	vec3 n = get_normal();
	vec3 v = normalize(view_pos - in_frag_pos);
	vec3 r = reflect(-v, n);
	
	vec3 f0 = vec3(0.04f);
	f0 = mix(f0, albedo.rgb, metallic);
	
    vec4 lo = vec4(0.0f);
	
    vec3 kS = fresnel_schlick_roughness(max(dot(n, v), 0.0f), f0, roughness);
    vec3 kD = 1.0f - kS;
    kD *= 1.0f - metallic;	  
	
    vec4 irradiance = texture(irradiance_map, n);
    vec4 diffuse = irradiance * albedo;
	
	const float MAX_REFLECTION_LOD = 8.0f;
	
	vec3 prefiltered_color = textureLod(prefilter_map, r, roughness * MAX_REFLECTION_LOD).rgb;   
	vec2 brdf  = texture(brdf_lut, vec2(max(dot(n, v), 0.0f), roughness)).rg;
    vec4 specular = vec4(prefiltered_color * (kS * brdf.x + brdf.y), 1.0f);

    vec4 ambient = (vec4(kD, 1.0f) * diffuse + specular) * ao;
    
    vec4 color = ambient + lo + emissive;
	
    vec4 mapped = vec4(1.0f) - exp(-color * exposure);
	
	vec3 corrected_color = vec3(1.0f / gamma);
	
    mapped = pow(mapped, vec4(corrected_color, 1.0f));
  
    frag_color = mapped;
}