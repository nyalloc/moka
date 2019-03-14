#pragma once

namespace moka
{
    namespace shaders
    {
        using shader_source = const char*;

        namespace shade_cubemap
        {
            constexpr static shader_source vert = R"(

                #version 330 core

                layout (location = 0) in vec3 aPos;

                uniform mat4 projection;
                uniform mat4 view;

                out vec3 localPos;

                void main()
                {
                    localPos = aPos;

                    mat4 rotView = mat4(mat3(view)); // remove translation from the view matrix
                    vec4 clipPos = projection * rotView * vec4(localPos, 1.0);

                    gl_Position = clipPos.xyww;
                }
            )";

            constexpr static shader_source frag = R"(

                #version 330 core

                out vec4 FragColor;

                in vec3 localPos;
              
                uniform samplerCube environmentMap;

                uniform float gamma;

                uniform float exposure;

                void main()
                {
                    vec3 envColor = texture(environmentMap, localPos).rgb;
                
                    vec3 mapped = vec3(1.0f) - exp(-envColor * exposure);
            
                    mapped = pow(mapped, vec3(1.0f / gamma));

                    FragColor = vec4(mapped, 1.0f);
                }
            )";
        } // namespace shade_cubemap

        namespace make_cubemap
        {
            constexpr static shader_source vert = R"(

                #version 330 core

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

                #version 330 core

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
        } // namespace make_cubemap

        namespace make_irradiance_map
        {
            constexpr static shader_source vert = R"(

                #version 330 core
                layout (location = 0) in vec3 aPos;

                out vec3 WorldPos;

                uniform mat4 projection;
                uniform mat4 view;

                void main()
                {
                    WorldPos = aPos;  
                    gl_Position =  projection * view * vec4(WorldPos, 1.0);
                }
            )";

            constexpr static shader_source frag = R"(

                #version 330 core
                out vec4 FragColor;
                in vec3 WorldPos;

                uniform samplerCube environmentMap;

                const float PI = 3.14159265359;

                void main()
                {		
	                // The world vector acts as the normal of a tangent surface
                    // from the origin, aligned to WorldPos. Given this normal, calculate all
                    // incoming radiance of the environment. The result of this radiance
                    // is the radiance of light coming from -Normal direction, which is what
                    // we use in the PBR shader to sample irradiance.
                    vec3 N = normalize(WorldPos);

                    vec3 irradiance = vec3(0.0);   
            
                    // tangent space calculation from origin point
                    vec3 up    = vec3(0.0, 1.0, 0.0);
                    vec3 right = cross(up, N);
                    up         = cross(N, right);
               
                    float sampleDelta = 0.025;
                    float nrSamples = 0.0;
                    for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
                    {
                        for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
                        {
                            // spherical to cartesian (in tangent space)
                            vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
                            // tangent space to world
                            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N; 

                            irradiance += texture(environmentMap, sampleVec).rgb * cos(theta) * sin(theta);
                            nrSamples++;
                        }
                    }
                    irradiance = PI * irradiance * (1.0 / float(nrSamples));
            
                    FragColor = vec4(irradiance, 1.0);
                }
            )";
        } // namespace make_irradiance_map

        namespace make_specular_map
        {
            constexpr static shader_source vert = R"(

                #version 330 core
                layout (location = 0) in vec3 aPos;
                out vec3 WorldPos;
                uniform mat4 projection;
                uniform mat4 view;
                void main()
                {
                    WorldPos = aPos;
                    gl_Position =  projection * view * vec4(WorldPos, 1.0);
                }
            )";

            constexpr static shader_source frag = R"(

                #version 330 core
                out vec4 FragColor;
                in vec3 WorldPos;
                uniform samplerCube environmentMap;
                uniform float roughness;
                const float PI = 3.14159265359;
                // ----------------------------------------------------------------------------
                float DistributionGGX(vec3 N, vec3 H, float roughness)
                {
                    float a = roughness*roughness;
                    float a2 = a*a;
                    float NdotH = max(dot(N, H), 0.0);
                    float NdotH2 = NdotH*NdotH;
                    float nom   = a2;
                    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
                    denom = PI * denom * denom;
                    return nom / denom;
                }
                // ----------------------------------------------------------------------------
                // http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html
                // efficient VanDerCorpus calculation.
                float RadicalInverse_VdC(uint bits)
                {
                     bits = (bits << 16u) | (bits >> 16u);
                     bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
                     bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
                     bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
                     bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
                     return float(bits) * 2.3283064365386963e-10; // / 0x100000000
                }
                // ----------------------------------------------------------------------------
                vec2 Hammersley(uint i, uint N)
                {
                    return vec2(float(i)/float(N), RadicalInverse_VdC(i));
                }
                // ----------------------------------------------------------------------------
                vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
                {
                    float a = roughness*roughness;
    
                    float phi = 2.0 * PI * Xi.x;
                    float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
                    float sinTheta = sqrt(1.0 - cosTheta*cosTheta);
    
                    // from spherical coordinates to cartesian coordinates - halfway vector
                    vec3 H;
                    H.x = cos(phi) * sinTheta;
                    H.y = sin(phi) * sinTheta;
                    H.z = cosTheta;
                    H = normalize(H);
    
                    // from tangent-space H vector to world-space sample vector
                    vec3 up          = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
                    vec3 tangent   = normalize(cross(up, N));
                    vec3 bitangent = cross(N, tangent);
    
                    vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
                    return normalize(sampleVec);
                }
                // ----------------------------------------------------------------------------
                void main()
                {
                    vec3 N = normalize(WorldPos);
    
                    // make the simplyfying assumption that V equals R equals the normal
                    vec3 R = N;
                    vec3 V = R;
                    const uint SAMPLE_COUNT = 2048u;
                    vec3 prefilteredColor = vec3(0.0);
                    float totalWeight = 0.0;
    
                    for(uint i = 0u; i < SAMPLE_COUNT; ++i)
                    {
                        // generates a sample vector that's biased towards the preferred alignment direction (importance sampling). 
                        vec2 Xi = Hammersley(i, SAMPLE_COUNT); 
                        vec3 H = ImportanceSampleGGX(Xi, N, roughness); 
                        vec3 L  = normalize(2.0 * dot(V, H) * H - V);
                        float NdotL = max(dot(N, L), 0.0);
                        if(NdotL > 0.0)
                        {
                            // sample from the environment's mip level based on roughness/pdf
                            float D   = DistributionGGX(N, H, roughness) / PI;
                            float NdotH = max(dot(N, H), 0.0);
                            float HdotV = max(dot(H, V), 0.0);
                            float pdf = D * NdotH / (4.0 * HdotV) + 0.0001;
                            float resolution = 1024.0; // resolution of source cubemap (per face)
                            float saTexel  = 4.0 * PI / (6.0 * resolution * resolution);
                            float saSample = 1.0 / (float(SAMPLE_COUNT) * pdf + 0.0001);
                            float mipLevel = roughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel);
    
                            prefilteredColor += textureLod(environmentMap, L, mipLevel).rgb * NdotL;
                            totalWeight      += NdotL;
                        }
                    }
                    if(totalWeight > 0.0f)
                    {
                        prefilteredColor = prefilteredColor / totalWeight;
                    }  
    
                    FragColor = vec4(prefilteredColor, 1.0);
                }
            )";
        } // namespace make_specular_map

        namespace make_brdf_map
        {
            constexpr static shader_source vert = R"(

                #version 330 core
                layout (location = 0) in vec3 aPos;
                layout (location = 1) in vec2 aTexCoords;

                out vec2 TexCoords;

                void main()
                {
                    TexCoords = aTexCoords;
                    gl_Position = vec4(aPos, 1.0);
                }
            )";

            constexpr static shader_source frag = R"(

                #version 330 core
                out vec2 FragColor;
                in vec2 TexCoords;

                const float PI = 3.14159265359;
                // ----------------------------------------------------------------------------
                // http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html
                // efficient VanDerCorpus calculation.
                float RadicalInverse_VdC(uint bits) 
                {
                     bits = (bits << 16u) | (bits >> 16u);
                     bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
                     bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
                     bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
                     bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
                     return float(bits) * 2.3283064365386963e-10; // / 0x100000000
                }
                // ----------------------------------------------------------------------------
                vec2 Hammersley(uint i, uint N)
                {
                    return vec2(float(i)/float(N), RadicalInverse_VdC(i));
                }
                // ----------------------------------------------------------------------------
                vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
                {
                    float a = roughness*roughness;
    
                    float phi = 2.0 * PI * Xi.x;
                    float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
                    float sinTheta = sqrt(1.0 - cosTheta*cosTheta);
    
                    // from spherical coordinates to cartesian coordinates - halfway vector
                    vec3 H;
                    H.x = cos(phi) * sinTheta;
                    H.y = sin(phi) * sinTheta;
                    H.z = cosTheta;

                    H = normalize(H);
    
                    // from tangent-space H vector to world-space sample vector
                    vec3 up          = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
                    vec3 tangent   = normalize(cross(up, N));
                    vec3 bitangent = cross(N, tangent);
    
                    vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
                    return normalize(sampleVec);
                }
                // ----------------------------------------------------------------------------
                float GeometrySchlickGGX(float NdotV, float roughness)
                {
                    // note that we use a different k for IBL
                    float a = roughness;
                    float k = (a * a) / 2.0;

                    float nom   = NdotV;
                    float denom = NdotV * (1.0 - k) + k;

                    return nom / denom;
                }
                // ----------------------------------------------------------------------------
                float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
                {
                    float NdotV = max(dot(N, V), 0.0);
                    float NdotL = max(dot(N, L), 0.0);
                    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
                    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

                    return ggx1 * ggx2;
                }
                // ----------------------------------------------------------------------------
                vec2 IntegrateBRDF(float NdotV, float roughness)
                {
                    vec3 V;
                    V.x = sqrt(1.0 - NdotV*NdotV);
                    V.y = 0.0;
                    V.z = NdotV;

                    float A = 0.0;
                    float B = 0.0; 

                    vec3 N = vec3(0.0, 0.0, 1.0);
            
                    const uint SAMPLE_COUNT = 2048u;
                    for(uint i = 0u; i < SAMPLE_COUNT; ++i)
                    {
                        // generates a sample vector that's biased towards the
                        // preferred alignment direction (importance sampling).
                        vec2 Xi = Hammersley(i, SAMPLE_COUNT);
                        vec3 H = ImportanceSampleGGX(Xi, N, roughness);
                        vec3 L = normalize(2.0 * dot(V, H) * H - V);

                        float NdotL = max(L.z, 0.0);
                        float NdotH = max(H.z, 0.0);
                        float VdotH = max(dot(V, H), 0.0);

                        if(NdotL > 0.0)
                        {
                            float G = GeometrySmith(N, V, L, roughness);
                            float G_Vis = (G * VdotH) / (NdotH * NdotV);
                            float Fc = pow(1.0 - VdotH, 5.0);

                            A += (1.0 - Fc) * G_Vis;
                            B += Fc * G_Vis;
                        }
                    }
                    A /= float(SAMPLE_COUNT);
                    B /= float(SAMPLE_COUNT);
                    return vec2(A, B);
                }
                // ----------------------------------------------------------------------------
                void main() 
                {
                    vec2 integratedBRDF = IntegrateBRDF(TexCoords.x, TexCoords.y);
                    FragColor = integratedBRDF;
                }
            )";
        } // namespace make_brdf_map

    } // namespace shaders
} // namespace moka
