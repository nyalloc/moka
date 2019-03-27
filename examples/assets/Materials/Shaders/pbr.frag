#define PI 3.1415926535897932384626433832795

struct pbr_material {
    vec4 diffuse_factor;    // default = vec3(1.0, 1.0, 1.0, 1.0)    
    vec4 emissive_factor;   // default = vec3(0.0, 0.0, 0.0, 1.0)  
    float roughness_factor; // default = 1.0 
    float metalness_factor; // default = 1.0 
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

uniform samplerCube irradiance_map;
uniform samplerCube prefilter_map;
uniform sampler2D brdf_lut;
uniform float gamma;
uniform float exposure;
uniform vec3 view_pos;
uniform pbr_material material;
in vec3 in_frag_pos;  
in vec3 in_normal;  
in vec2 in_texture_coord;
out vec4 frag_color;

#ifdef NORMAL_MAP 
    in mat3 tbn_matrix;
    in float green_channel_scalar;
#endif

const float mip_count = 4.0;

vec4 get_albedo() {
    #ifdef DIFFUSE_MAP
        return texture(material.diffuse_map, in_texture_coord).rgba;
    #else 
        return material.diffuse_factor;
    #endif
}

vec3 get_normal() {
    #ifdef NORMAL_MAP
        vec3 material_normal = texture(material.normal_map, in_texture_coord).rgb;
        material_normal = normalize(material_normal * 2.0 - 1.0); 
        material_normal = normalize(tbn_matrix * material_normal);
    #else 
        vec3 material_normal = in_normal;
    #endif
    return normalize(material_normal);
}

vec4 get_emissive() {
    #ifdef EMISSIVE_MAP
        return texture(material.emissive_map, in_texture_coord).rgba * material.emissive_factor;
    #else 
        return material.emissive_factor;
    #endif
}

float get_roughness() {
    #ifdef METALLIC_ROUGHNESS_MAP
        return texture(material.metallic_roughness_map, in_texture_coord).g * material.roughness_factor;
    #else
        return material.roughness_factor;
    #endif
}

float get_metallic() {
    #ifdef METALLIC_ROUGHNESS_MAP
        return texture(material.metallic_roughness_map, in_texture_coord).b * material.metalness_factor;
    #else 
        return material.metalness_factor;
    #endif
}

float get_ao() {
    #ifdef AO_MAP
        return texture(material.ao_map, in_texture_coord).r;
    #else 
        return 1.0;
    #endif
}

bool discard_fragment() {
    #ifdef MASK_ALPHA
        return texture(material.diffuse_map, in_texture_coord).a < material.alpha_cutoff;
    #else
        return false;
    #endif  
}

float distribution_ggx(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float n_dot_h = max(dot(N, H), 0.0);
    float n_dot_h2 = n_dot_h * n_dot_h;
    float nom   = a2;
    float denom = (n_dot_h2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
    return nom / denom;
}

float geometry_schlick_ggx(float n_dot_v, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;
    float nom   = n_dot_v;
    float denom = n_dot_v * (1.0 - k) + k;
    return nom / denom;
}

float geometry_smith(vec3 N, vec3 V, vec3 L, float roughness) {
    float n_dot_v = max(dot(N, V), 0.0);
    float n_dot_l = max(dot(N, L), 0.0);
    float ggx2 = geometry_schlick_ggx(n_dot_v, roughness);
    float ggx1 = geometry_schlick_ggx(n_dot_l, roughness);
    return ggx1 * ggx2;
}

vec3 fresnel_schlick(float cos_theta, vec3 f0) {
    return f0 + (1.0 - f0) * pow(1.0 - cos_theta, 5.0);
}

vec3 fresnel_schlick_roughness(float cos_theta, vec3 f0, float roughness) {
    return f0 + (max(vec3(1.0 - roughness), f0) - f0) * pow(1.0 - cos_theta, 5.0);
}   

void main() {
    if(discard_fragment()) {
        discard;
    }
    // fetch the material properties
    vec4 albedo = get_albedo();
    vec4 emissive = get_emissive();
    float metallic = get_metallic();
    float roughness = get_roughness();
    float ao = get_ao();	
    // fetch input lighting data
    vec3 n = get_normal();
    vec3 v = normalize(view_pos - in_frag_pos);
    vec3 r = reflect(-v, n);	
	float n_dot_v = dot(n, v);
	
	
    // get the diffuse light contribution
    vec4 irradiance = texture(irradiance_map, n);
    vec4 diffuse = (irradiance * albedo);
    // calculate the reflectance at normal incidence
    vec3 f0 = vec3(0.04);
    f0 = mix(f0, albedo.rgb, metallic);			
    vec3 kS = fresnel_schlick_roughness(max(n_dot_v, 0.0), f0, roughness);
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;	  		
    // calculate final lighting 
    vec3 prefiltered_color = textureLod(prefilter_map, r, roughness * mip_count).rgb;   	
    vec2 brdf  = texture(brdf_lut, vec2(max(n_dot_v, 0.0), roughness)).rg;
    vec4 specular = vec4(prefiltered_color * (kS * brdf.x + brdf.y), 1.0);
    vec4 ambient = (vec4(kD, 1.0) * diffuse + specular) * ao;
    vec4 color = ambient + emissive;
    // gamma correction + HDR tone mapping
    vec4 mapped = vec4(1.0) - exp(-color * exposure);
    vec3 corrected_color = vec3(1.0 / gamma);
    frag_color = vec4(pow(mapped.xyz, corrected_color), albedo.a);
}