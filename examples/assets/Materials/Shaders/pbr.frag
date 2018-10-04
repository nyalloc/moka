#version 330

// Moka GLSL standard uber-shader

#moka_compilation_flags
// #define DIFFUSE_MAP
// #define NORMAL_MAP
// #define EMISSIVE_MAP
// #define METALLIC_ROUGHNESS_MAP
// #define AO_MAP

struct pbr_material 
{
    vec3 diffuse_factor;    // default = vec3(1.0, 1.0, 1.0)    
    vec3 emissive_factor;   // default = vec3(0.0, 0.0, 0.0)  
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
#endif

uniform vec3 view_pos;
uniform pbr_material material;
uniform directional_light light;

vec3 get_diffuse()
{
    #ifdef DIFFUSE_MAP
        return texture(material.diffuse_map, in_texture_coord).rgb * material.diffuse_factor;
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
        return material_normal;
    #else 
        return in_normal;
    #endif
}

vec3 get_emissive()
{
    #ifdef EMISSIVE_MAP
        return texture(material.emissive_map, in_texture_coord).rgb * material.emissive_factor;
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

float get_metalness()
{
    #ifdef METALLIC_ROUGHNESS_MAP
        return texture(material.metallic_roughness_map, in_texture_coord).b * material.metalness_factor;
    #else 
        return material.metalness_factor;
    #endif
}

void main()
{
    vec3 material_diffuse = get_diffuse();
    vec3 material_normal = get_normal();
    vec3 material_emissive = get_emissive();
    
    float material_roughness = get_roughness();
    float material_metalness = get_metalness();
    
    #ifdef AO_MAP
        float material_occlusion = texture(material.ao_map, in_texture_coord).r;
    #endif
        
    vec3 result = material_diffuse + material_emissive;
    
    frag_color = vec4(result, 1.0);
}