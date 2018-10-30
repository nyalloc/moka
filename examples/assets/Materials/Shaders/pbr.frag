#version 330

// Moka GLSL standard uber-shader

#moka_compilation_flags
// #define DIFFUSE_MAP
// #define NORMAL_MAP
// #define EMISSIVE_MAP
// #define METALLIC_ROUGHNESS_MAP
// #define AO_MAP
// #define MASK_ALPHA

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

struct directional_light
{
    vec3 ambient;
    vec3 position;
    vec3 diffuse;
    vec3 specular;
};

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
uniform directional_light light;

vec4 get_diffuse()
{
    #ifdef DIFFUSE_MAP
        return texture(material.diffuse_map, in_texture_coord).rgba * material.diffuse_factor;
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
        vec3 material_normal = normalize(in_normal);
    #endif
    
    return material_normal;
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
        return texture(material.metallic_roughness_map, in_texture_coord).g * 1.0f;
    #else
        return material.roughness_factor;
    #endif
}

float get_metalness()
{
    #ifdef METALLIC_ROUGHNESS_MAP
        return texture(material.metallic_roughness_map, in_texture_coord).b * material.metalness_factor;
    #else 
        return material.metalness_factor;
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

void main()
{
    if(discard_fragment())
    {
        discard;
    }

    // ambient
    
    vec4 base_diffuse = get_diffuse();
    
    vec4 ambient = vec4(light.ambient * base_diffuse.rgb, base_diffuse.a);
  	
    // diffuse 
    vec3 norm = get_normal();

    vec3 lightDir = normalize(light.position);  
    float diff = max(dot(norm, lightDir), 0.0);
    
    vec4 diffuse = vec4(light.diffuse * diff * base_diffuse.rgb, base_diffuse.a);
        
    // specular
    vec3 viewDir = normalize(view_pos - in_frag_pos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 16.0f);
    vec4 specular = vec4(light.specular * spec * get_roughness(), base_diffuse.a);  
        
    frag_color = ambient + diffuse + specular;
}