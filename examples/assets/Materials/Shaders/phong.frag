/*
===========================================================================
Moka Source Code
Copyright 2019 Stuart Adams. All rights reserved.
https://github.com/stuartdadams/moka
stuartdadams | linkedin.com/in/stuartdadams

This file is part of the Moka Real-Time Physically-Based Rendering Project.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

References:

    Karis, B. (2013). Real Shading in Unreal Engine 4. In: Physically Based Shading
    in Theory and Practice. [online] SIGGRAPH. Available at:
    http://gamedevs.org/uploads/real-shading-in-unreal-engine-4.pdf/ [Accessed 20 Nov. 2018].

    Lagarde, S. and Rousiers, C. (2014). Moving Frostbite to Physically Based Rendering 3.0.
    In: Physically Based Shading in Theory and Practice. [online] SIGGRAPH. Available at:
    https://seblagarde.wordpress.com/2015/07/14/siggraph-2014-moving-frostbite-to-physically-based-rendering/
    [Accessed 20 Nov. 2018].

    Spogreev, I. (2016). Physically Based Light Probe Generation on GPU. In: W. Engel, ed.,
    GPU Pro 6. Boca Raton, FL: CRC Press, pp.243-266.

===========================================================================
*/

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

struct directional_light {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
};

uniform samplerCube irradiance_map;
uniform samplerCube prefilter_map;
uniform sampler2D brdf_lut;
uniform float gamma;
uniform float exposure;
uniform vec3 view_pos;
uniform pbr_material material;
uniform directional_light light;
uniform bool use_directional_light;
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
	vec3 l = normalize(-light.direction);  
		
	float n_dot_v = dot(n, v);
	float n_dot_l = dot(n, l);
	
	vec4 diffuse = vec4(0.0);
    vec4 specular = vec4(0.0);
	vec4 ambient = vec4(light.ambient * albedo.rgb, 1.0);
  	
	if(use_directional_light)
	{
		float diff = max(n_dot_l, 0.0);
		diffuse = vec4(light.diffuse * diff * albedo.rgb, 1.0);  
    
		vec3 reflect_dir = reflect(-l, n);  
		float spec = pow(max(dot(v, reflect_dir), 0.0), 64.0f);
		specular = vec4(vec3(1.0) * spec * roughness, 1.0);  
	}
	
    vec4 color = ambient + diffuse + specular + emissive;
    // gamma correction + HDR tone mapping
    vec4 mapped = vec4(1.0) - exp(-color * exposure);
    vec3 corrected_color = vec3(1.0 / gamma);
    frag_color = vec4(pow(mapped.xyz, corrected_color), albedo.a);
}