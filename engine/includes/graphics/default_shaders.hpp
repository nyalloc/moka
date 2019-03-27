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
#pragma once

namespace moka
{
    namespace shaders
    {
        using shader_source = const char*;

        namespace shade_cubemap
        {
            constexpr static shader_source vert = R"(

                layout (location = 0) in vec3 a_pos;

                uniform mat4 projection;
                uniform mat4 view;

                out vec3 local_position;

                void main()
                {
                    local_position = a_pos;

                    mat4 rot_view = mat4(mat3(view)); 

                    vec4 clip_pos = projection * rot_view * vec4(local_position, 1.0);

                    gl_Position = clip_pos.xyww;
                }
            )";

            constexpr static shader_source frag = R"(

                out vec4 fragment_color;

                in vec3 local_position;
              
                uniform samplerCube environment_map;

                uniform float gamma;

                uniform float exposure;

                void main()
                {
                    vec3 environment_color = texture(environment_map, local_position).rgb;
                
                    vec3 mapped = vec3(1.0f) - exp(-environment_color * exposure);
            
                    mapped = pow(mapped, vec3(1.0f / gamma));

                    fragment_color = vec4(mapped, 1.0f);
                }
            )";
        } // namespace shade_cubemap

        namespace equirectangular_to_cube
        {
            constexpr static shader_source vert = R"(

                layout (location = 0) in vec3 a_pos;

                out vec3 local_pos;

                uniform mat4 projection;
                uniform mat4 view;

                void main()
                {
                    local_pos = a_pos;  
                    gl_Position =  projection * view * vec4(local_pos, 1.0f);
                }
            )";

            constexpr static shader_source frag = R"(

                out vec4 frag_color;
                in vec3 local_pos;

                uniform sampler2D map;

                const vec2 inv_atan = vec2(0.1591f, 0.3183f);
                vec2 sample_spherical_map(vec3 v)
                {
                    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
                    uv *= inv_atan;
                    uv += 0.5f;
                    return uv;
                }

                void main()
                {		
                    vec2 uv = sample_spherical_map(normalize(local_pos));
                    vec3 color = texture(map, uv).rgb;
            
                    frag_color = vec4(color, 1.0f);
                }
            )";
        } // namespace equirectangular_to_cube

        namespace make_irradiance_map
        {
            constexpr static shader_source vert = R"(

                layout (location = 0) in vec3 a_pos;

                out vec3 world_position;

                uniform mat4 projection;
                uniform mat4 view;

                void main()
                {
                    world_position = a_pos;  
                    gl_Position =  projection * view * vec4(world_position, 1.0);
                }
            )";

            constexpr static shader_source frag = R"(

                out vec4 fragment_color;
                in vec3 world_position;

                uniform samplerCube environment_map;

                const float PI = 3.14159265359;

                void main()
                {		
                    vec3 n = normalize(world_position);

                    vec3 irradiance = vec3(0.0);   
            
                    vec3 up    = vec3(0.0, 1.0, 0.0);
                    vec3 right = cross(up, n);
                    up         = cross(n, right);
               
                    float sample_delta = 0.025;
                    float number_of_samples = 0.0;

                    for(float phi = 0.0; phi < 2.0 * PI; phi += sample_delta)
                    {
                        for(float theta = 0.0; theta < 0.5 * PI; theta += sample_delta)
                        {
                            vec3 tangent_sample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));

                            vec3 sample_vec = tangent_sample.x * right + tangent_sample.y * up + tangent_sample.z * n; 

                            irradiance += texture(environment_map, sample_vec).rgb * cos(theta) * sin(theta);

                            number_of_samples++;
                        }
                    }
                    irradiance = PI * irradiance * (1.0 / number_of_samples);
            
                    fragment_color = vec4(irradiance, 1.0);
                }
            )";
        } // namespace make_irradiance_map

        namespace make_specular_map
        {
            constexpr static shader_source vert = R"(

                layout (location = 0) in vec3 a_pos;

                out vec3 world_position;
                uniform mat4 projection;
                uniform mat4 view;

                void main()
                {
                    world_position = a_pos;
                    gl_Position =  projection * view * vec4(world_position, 1.0);
                }
            )";

            constexpr static shader_source frag = R"(

                out vec4 fragment_color;
                in vec3 world_position;
                uniform samplerCube environment_map;
                uniform float roughness;
                const float PI = 3.14159265359;

                float distribution_ggx(vec3 n, vec3 h, float roughness)
                {
                    float a = roughness * roughness;
                    float a_squared = a * a;
                    float n_dot_h = max(dot(n, h), 0.0);
                    float n_dot_h_squared = n_dot_h*n_dot_h;
                    float numerator   = a_squared;
                    float denominator = (n_dot_h_squared * (a_squared - 1.0) + 1.0);
                    denominator = PI * denominator * denominator;
                    return numerator / denominator;
                }

                float radical_inverse_vdc(uint bits)
                {
                     bits = (bits << 16u) | (bits >> 16u);
                     bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
                     bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
                     bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
                     bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
                     return float(bits) * 2.3283064365386963e-10; // / 0x100000000
                }

                vec2 hammersley(uint i, uint n)
                {
                    return vec2(float(i)/float(n), radical_inverse_vdc(i));
                }

                vec3 importance_sample_ggx(vec2 xi, vec3 n, float roughness)
                {
                    float a = roughness * roughness;
    
                    float phi = 2.0 * PI * xi.x;
                    float cos_theta = sqrt((1.0 - xi.y) / (1.0 + (a*a - 1.0) * xi.y));
                    float sin_theta = sqrt(1.0 - cos_theta * cos_theta);
    
                    vec3 h;
                    h.x = cos(phi) * sin_theta;
                    h.y = sin(phi) * sin_theta;
                    h.z = cos_theta;
                    h = normalize(h);
    
                    vec3 up        = abs(n.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
                    vec3 tangent   = normalize(cross(up, n));
                    vec3 bitangent = cross(n, tangent);
    
                    vec3 sample_vec = tangent * h.x + bitangent * h.y + n * h.z;
                    return normalize(sample_vec);
                }

                void main()
                {
                    vec3 n = normalize(world_position);
    
                    vec3 r = n;
                    vec3 v = r;
                    const uint sample_count = 2048u;
                    vec3 prefiltered_color  = vec3(0.0);
                    float total_weight      = 0.0;
    
                    for(uint i = 0u; i < sample_count; ++i)
                    {
                        vec2 xi       = hammersley(i, sample_count); 
                        vec3 h        = importance_sample_ggx(xi, n, roughness); 
                        vec3 l        = normalize(2.0 * dot(v, h) * h - v);
                        float n_dot_l = max(dot(n, l), 0.0);

                        if(n_dot_l > 0.0)
                        {
                            float d          = distribution_ggx(n, h, roughness) / PI;
                            float n_dot_h    = max(dot(n, h), 0.0);
                            float h_dot_v    = max(dot(h, v), 0.0);
                            float pdf        = d * n_dot_h / (4.0 * h_dot_v) + 0.0001;
                            float resolution = 1024.0; 
                            float sa_texel   = 4.0 * PI / (6.0 * resolution * resolution);
                            float sa_sample  = 1.0 / (float(sample_count) * pdf + 0.0001);
                            float mip_level  = roughness == 0.0 ? 0.0 : 0.5 * log2(sa_sample / sa_texel);
    
                            prefiltered_color += textureLod(environment_map, l, mip_level).rgb * n_dot_l;
                            total_weight      += n_dot_l;
                        }
                    }
                    if(total_weight > 0.0f)
                    {
                        prefiltered_color = prefiltered_color / total_weight;
                    }  
    
                    fragment_color = vec4(prefiltered_color, 1.0);
                }
            )";
        } // namespace make_specular_map

        namespace make_brdf_map
        {
            constexpr static shader_source vert = R"(

                layout (location = 0) in vec3 a_pos;
                layout (location = 1) in vec2 aTexCoords;

                out vec2 texture_coords;

                void main()
                {
                    texture_coords = aTexCoords;
                    gl_Position = vec4(a_pos, 1.0);
                }
            )";

            constexpr static shader_source frag = R"(

                out vec2 fragment_color;
                in vec2 texture_coords;

                const float PI = 3.14159265359;

                float radical_inverse_vdc(uint bits) 
                {
                     bits = (bits << 16u) | (bits >> 16u);
                     bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
                     bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
                     bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
                     bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
                     return float(bits) * 2.3283064365386963e-10; // / 0x100000000
                }

                vec2 hammersley(uint i, uint n)
                {
                    return vec2(float(i) / float(n), radical_inverse_vdc(i));
                }

                vec3 importance_sample_ggx(vec2 xi, vec3 n, float roughness)
                {
                    float a = roughness * roughness;
    
                    float phi = 2.0 * PI * xi.x;
                    float cos_theta = sqrt((1.0 - xi.y) / (1.0 + (a*a - 1.0) * xi.y));
                    float sin_theta = sqrt(1.0 - cos_theta * cos_theta);
    
                    vec3 h;
                    h.x = cos(phi) * sin_theta;
                    h.y = sin(phi) * sin_theta;
                    h.z = cos_theta;

                    h = normalize(h);
    
                    vec3 up        = abs(n.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
                    vec3 tangent   = normalize(cross(up, n));
                    vec3 bitangent = cross(n, tangent);
    
                    vec3 sample_vec = tangent * h.x + bitangent * h.y + n * h.z;
                    return normalize(sample_vec);
                }

                float geometry_schlick_ggx(float n_dot_v, float roughness)
                {
                    float a = roughness;
                    float k = (a * a) / 2.0;

                    float numerator   = n_dot_v;
                    float denominator = n_dot_v * (1.0 - k) + k;

                    return numerator / denominator;
                }

                float geometry_smith(vec3 n, vec3 v, vec3 l, float roughness)
                {
                    float n_dot_v = max(dot(n, v), 0.0);
                    float n_dot_l = max(dot(n, l), 0.0);
                    float ggx2 = geometry_schlick_ggx(n_dot_v, roughness);
                    float ggx1 = geometry_schlick_ggx(n_dot_l, roughness);

                    return ggx1 * ggx2;
                }

                vec2 integrate_brdf(float n_dot_v, float roughness)
                {
                    vec3 v;

                    v.x = sqrt(1.0 - n_dot_v * n_dot_v);
                    v.y = 0.0;
                    v.z = n_dot_v;

                    float a = 0.0;
                    float b = 0.0; 

                    vec3 n = vec3(0.0, 0.0, 1.0);
            
                    const uint sample_count = 2048u;

                    for(uint i = 0u; i < sample_count; ++i)
                    {
                        vec2 xi = hammersley(i, sample_count);
                        vec3 h = importance_sample_ggx(xi, n, roughness);
                        vec3 l = normalize(2.0 * dot(v, h) * h - v);

                        float n_dot_l = max(l.z, 0.0);
                        float n_dot_h = max(h.z, 0.0);
                        float v_dot_h = max(dot(v, h), 0.0);

                        if(n_dot_l > 0.0)
                        {
                            float g = geometry_smith(n, v, l, roughness);
                            float g_vis = (g * v_dot_h) / (n_dot_h * n_dot_v);
                            float fc = pow(1.0 - v_dot_h, 5.0);

                            a += (1.0 - fc) * g_vis;
                            b += fc * g_vis;
                        }
                    }
                    a /= float(sample_count);
                    b /= float(sample_count);
                    return vec2(a, b);
                }

                void main() 
                {
                    vec2 integrated_brdf = integrate_brdf(texture_coords.x, texture_coords.y);
                    fragment_color = integrated_brdf;
                }
            )";
        } // namespace make_brdf_map
    }     // namespace shaders
} // namespace moka
