#include <application/application.hpp>
#include <application/open_file.hpp>
#include <asset_importer/model_importer.hpp>
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <graphics/camera/camera.hpp>
#include <graphics/color.hpp>
#include <graphics/device/graphics_device.hpp>
#include <graphics/material/material_builder.hpp>
#include <graphics/model.hpp>
#include <imgui.h>
#include <imgui.hpp>

using namespace moka;

model make_hdr_environment_map(
    graphics_device& device,
    const std::filesystem::path& texture_path,
    texture& irradiance_,
    texture& brdf,
    texture& prefiltered)
{
    // we need to take a 2D texture, then turn it into an environment map.
    // to do this, we need to use a shader that will project the 2d texture onto the sides of a cube.
    // we will then render this cube from all 6 directions and capture the result in a frame buffer.
    // we can then use the 6 textures to stitch together our irradiance_.

    auto environment_size = 512;

    auto height = 0;
    auto width = 0;
    host_format format;

    const auto data = texture_load_hdr(texture_path, width, height, format);

    const auto hdr = device.build_texture()
                         .add_image_data(
                             image_target::texture_2d,
                             0,
                             device_format::rgb16f,
                             width,
                             height,
                             0,
                             host_format::rgb,
                             pixel_type::float32,
                             data)
                         .set_wrap_s(wrap_mode::clamp_to_edge)
                         .set_wrap_t(wrap_mode::clamp_to_edge)
                         .set_min_filter(min_filter::linear)
                         .set_mag_filter(mag_filter::linear)
                         .free_host_data(true)
                         .build();

    // clang-format off
    float cube_vertices[] = 
    {
        // back face-----------------------------------------------------------|
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, //< bottom-left  |
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, //< top-right    |
         1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, //< bottom-right |
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, //< top-right    |
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, //< bottom-left  |
        -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, //< top-left     |
        // front face----------------------------------------------------------|
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, //< bottom-left  |
         1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, //< bottom-right |
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, //< top-right    |
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, //< top-right    |
        -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, //< top-left     |
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, //< bottom-left  |
        // left face-----------------------------------------------------------|
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, //< top-right    |
        -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, //< top-left     |
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, //< bottom-left  |
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, //< bottom-left  |
        -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, //< bottom-right |
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, //< top-right    |
        // right face----------------------------------------------------------|
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, //< top-left     |
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, //< bottom-right |
         1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, //< top-right    |
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, //< bottom-right |
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, //< top-left     |
         1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, //< bottom-left  |
        // bottom face---------------------------------------------------------|
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, //< top-right    |
         1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, //< top-left     |
         1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, //< bottom-left  |
         1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, //< bottom-left  |
        -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, //< bottom-right |
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, //< top-right    |
        // top face------------------------------------------------------------|
        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, //< top-left     |
         1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, //< bottom-right |
         1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, //< top-right    |
         1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, //< bottom-right |
        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, //< top-left     |
        -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  //< bottom-left  |
        // --------------------------------------------------------------------|
    };
    // clang-format on

    const auto size = sizeof(float);

    auto cube_layout =
        vertex_layout::builder{}
            .add_attribute(0, 3, attribute_type::float32, false, 8 * size, 0)
            .add_attribute(1, 3, attribute_type::float32, false, 8 * size, 3 * size)
            .add_attribute(2, 2, attribute_type::float32, false, 8 * size, 6 * size)
            .build();

    const auto cube_buffer = device.make_vertex_buffer(
        cube_vertices, sizeof cube_vertices, std::move(cube_layout), buffer_usage::static_draw);

    std::map<std::string, program> shaders;

    const auto hdr_to_cube_vert = R"(

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

    const auto hdr_to_cube_frag = R"(

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

    const auto cube_to_irradiance_vert = R"(

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

    const auto cube_to_irradiance_frag = R"(

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

    const auto projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);

    glm::mat4 capture_views[] = {
        lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
        lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
        lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))};

    image_target image_targets[] = {
        image_target::cubemap_positive_x,
        image_target::cubemap_negative_x,
        image_target::cubemap_positive_y,
        image_target::cubemap_negative_y,
        image_target::cubemap_positive_z,
        image_target::cubemap_negative_z,
    };

    // set up irradiance_ from hdr environment map -----------------------

    auto hdr_material = material::builder{device, shaders}
                            .set_vertex_shader(hdr_to_cube_vert)
                            .set_fragment_shader(hdr_to_cube_frag)
                            .add_uniform("projection", projection)
                            .add_uniform("view", parameter_type::mat4)
                            .add_uniform("map", hdr)
                            .set_culling_enabled(false)
                            .build();

    command_list hdr_map_list;

    auto hdr_texture_builder = device.build_texture();

    for (auto& image_target : image_targets)
    {
        hdr_texture_builder.add_image_data(
            image_target,
            0,
            device_format::rgb16f,
            environment_size,
            environment_size,
            0,
            host_format::rgb,
            pixel_type::float32,
            nullptr);
    }

    auto hdr_cubemap = hdr_texture_builder.set_target(texture_target::cubemap)
                           .set_wrap_s(wrap_mode::clamp_to_edge)
                           .set_wrap_t(wrap_mode::clamp_to_edge)
                           .set_wrap_r(wrap_mode::clamp_to_edge)
                           .set_min_filter(min_filter::linear)
                           .set_mag_filter(mag_filter::linear)
                           .build();

    const auto hdr_frame_buffer =
        device.build_frame_buffer()
            .add_depth_attachment(frame_format::depth_component24, environment_size, environment_size)
            .build();

    hdr_map_list.viewport().set_rectangle(0, 0, environment_size, environment_size);

    hdr_map_list.frame_buffer().set_frame_buffer(hdr_frame_buffer);

    // each draw call renders to the side of a irradiance_
    for (auto i = 0; i < 6; i++)
    {
        hdr_material["view"] = capture_views[i];

        auto image_target = image_targets[i];

        hdr_map_list.frame_buffer_texture()
            .set_texture(hdr_cubemap)
            .set_attachment(frame_attachment::color)
            .set_target(image_target)
            .set_mip_level(0);

        hdr_map_list.clear().set_clear_color(true).set_clear_depth(true);

        hdr_map_list.draw()
            .set_vertex_buffer(cube_buffer)
            .set_vertex_count(36)
            .set_primitive_type(primitive_type::triangles)
            .set_material(hdr_material);
    }

    device.submit(std::move(hdr_map_list), false);

    // irradiance irradiance_ --------------------------------

    auto irradiance_material = material::builder{device, shaders}
                                   .set_vertex_shader(cube_to_irradiance_vert)
                                   .set_fragment_shader(cube_to_irradiance_frag)
                                   .add_uniform("projection", projection)
                                   .add_uniform("view", parameter_type::mat4)
                                   .add_uniform("environmentMap", hdr_cubemap)
                                   .set_culling_enabled(false)
                                   .build();

    command_list irradiance_list;

    auto irradiance_size = 32;

    auto irradiance_texture_builder = device.build_texture();

    for (auto& image_target : image_targets)
    {
        irradiance_texture_builder.add_image_data(
            image_target,
            0,
            device_format::rgb16f,
            irradiance_size,
            irradiance_size,
            0,
            host_format::rgb,
            pixel_type::float32,
            nullptr);
    }

    auto irradiance_cubemap =
        irradiance_texture_builder.set_target(texture_target::cubemap)
            .set_wrap_s(wrap_mode::clamp_to_edge)
            .set_wrap_t(wrap_mode::clamp_to_edge)
            .set_wrap_r(wrap_mode::clamp_to_edge)
            .set_min_filter(min_filter::linear)
            .set_mag_filter(mag_filter::linear)
            .build();

    const auto irradiance_frame_buffer =
        device.build_frame_buffer()
            .add_depth_attachment(frame_format::depth_component24, irradiance_size, irradiance_size)
            .build();

    irradiance_list.viewport().set_rectangle(0, 0, irradiance_size, irradiance_size);

    irradiance_list.frame_buffer().set_frame_buffer(irradiance_frame_buffer);

    // each draw call renders to the side of a irradiance_
    for (auto i = 0; i < 6; i++)
    {
        irradiance_material["view"] = capture_views[i];

        auto image_target = image_targets[i];

        irradiance_list.frame_buffer_texture()
            .set_texture(irradiance_cubemap)
            .set_attachment(frame_attachment::color)
            .set_target(image_target)
            .set_mip_level(0);

        irradiance_list.clear().set_clear_color(true).set_clear_depth(true);

        irradiance_list.draw()
            .set_vertex_buffer(cube_buffer)
            .set_vertex_count(36)
            .set_primitive_type(primitive_type::triangles)
            .set_material(irradiance_material);
    }

    device.submit(std::move(irradiance_list), false);

    // irradiance_ ------------------------------------------
    auto cubemap_vert = R"(

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

    auto cubemap_frag = R"(

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

    auto cubemap_material = material::builder{device, shaders}
                                .set_vertex_shader(cubemap_vert)
                                .set_fragment_shader(cubemap_frag)
                                .add_uniform("projection", parameter_type::mat4)
                                .add_uniform("view", parameter_type::mat4)
                                .add_uniform("environmentMap", hdr_cubemap)
                                .set_culling_enabled(false)
                                .build();

    irradiance_ = irradiance_cubemap;

    const auto brdf_vert = R"(
    
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

    const auto brdf_frag = R"(
    
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
            
            const uint SAMPLE_COUNT = 1024u;
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

    // brdf look up texture --------------------------------

    auto brdf_material = material::builder{device, shaders}
                             .set_vertex_shader(brdf_vert)
                             .set_fragment_shader(brdf_frag)
                             .build();

    command_list brdf_list;

    // clang-format off
    float quad_vertices[] = 
    {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };
    // clang-format on

    auto quad_layout =
        vertex_layout::builder{}
            .add_attribute(0, 3, attribute_type::float32, false, 5 * size, 0)
            .add_attribute(1, 2, attribute_type::float32, false, 5 * size, 3 * size)
            .build();

    const auto quad_buffer = device.make_vertex_buffer(
        quad_vertices, sizeof quad_vertices, std::move(quad_layout), buffer_usage::static_draw);

    auto brdf_size = 512;

    // allocate space for the lookup texture
    auto brdf_image = device.build_texture()
                          .add_image_data(
                              image_target::texture_2d,
                              0,
                              device_format::rg16f,
                              brdf_size,
                              brdf_size,
                              0,
                              host_format::rg,
                              pixel_type::float32,
                              nullptr)
                          .set_target(texture_target::texture_2d)
                          .set_wrap_s(wrap_mode::clamp_to_edge)
                          .set_wrap_t(wrap_mode::clamp_to_edge)
                          .set_min_filter(min_filter::linear)
                          .set_mag_filter(mag_filter::linear)
                          .build();

    // create a framebuffer to render to
    const auto brdf_frame_buffer =
        device.build_frame_buffer()
            .add_depth_attachment(frame_format::depth_component24, brdf_size, brdf_size)
            .build();

    brdf_list.frame_buffer().set_frame_buffer(brdf_frame_buffer);

    brdf_list.viewport().set_rectangle(0, 0, brdf_size, brdf_size);

    brdf_list.frame_buffer_texture()
        .set_texture(brdf_image)
        .set_attachment(frame_attachment::color)
        .set_target(image_target::texture_2d)
        .set_mip_level(0);

    brdf_list.clear().set_clear_color(true).set_clear_depth(true);

    brdf_list.draw()
        .set_vertex_buffer(quad_buffer)
        .set_vertex_count(4)
        .set_primitive_type(primitive_type::triangle_strip)
        .set_material(brdf_material);

    device.submit(std::move(brdf_list), false);

    brdf = brdf_image;

    const auto prefilter_vert = R"(
    
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

    const auto prefilter_frag = R"(
    
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

            const uint SAMPLE_COUNT = 1024u;
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
                    float D   = DistributionGGX(N, H, roughness);
                    float NdotH = max(dot(N, H), 0.0);
                    float HdotV = max(dot(H, V), 0.0);
                    float pdf = D * NdotH / (4.0 * HdotV) + 0.0001;

                    float resolution = 512.0; // resolution of source cubemap (per face)
                    float saTexel  = 4.0 * PI / (6.0 * resolution * resolution);
                    float saSample = 1.0 / (float(SAMPLE_COUNT) * pdf + 0.0001);

                    float mipLevel = roughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel);
    
                    prefilteredColor += textureLod(environmentMap, L, mipLevel).rgb * NdotL;
                    totalWeight      += NdotL;
                }
            }

            prefilteredColor = prefilteredColor / totalWeight;

            FragColor = vec4(prefilteredColor, 1.0);
        }

    )";

    command_list prefilter_list;

    auto prefilter_size = 128;

    auto prefilter_texture_builder = device.build_texture();

    for (auto& image_target : image_targets)
    {
        prefilter_texture_builder.add_image_data(
            image_target,
            0,
            device_format::rgb16f,
            prefilter_size,
            prefilter_size,
            0,
            host_format::rgb,
            pixel_type::float32,
            nullptr);
    }

    auto prefilter_cubemap =
        prefilter_texture_builder.set_target(texture_target::cubemap)
            .set_wrap_s(wrap_mode::clamp_to_edge)
            .set_wrap_t(wrap_mode::clamp_to_edge)
            .set_wrap_r(wrap_mode::clamp_to_edge)
            .set_min_filter(min_filter::linear_mipmap_linear)
            .set_mag_filter(mag_filter::linear)
            .set_mipmaps(true)
            .build();

    auto prefilter_material = material::builder{device, shaders}
                                  .set_vertex_shader(prefilter_vert)
                                  .set_fragment_shader(prefilter_frag)
                                  .add_uniform("roughness", parameter_type::float32)
                                  .add_uniform("environmentMap", hdr_cubemap)
                                  .add_uniform("projection", projection)
                                  .add_uniform("view", parameter_type::mat4)
                                  .set_culling_enabled(false)
                                  .build();

    const size_t max_mip_levels = 5;
    for (size_t mip = 0; mip < max_mip_levels; ++mip)
    {
        const size_t mip_width = prefilter_size * std::pow(0.5, mip);
        const size_t mip_height = prefilter_size * std::pow(0.5, mip);

        const auto prefilter_frame_buffer =
            device.build_frame_buffer()
                .add_depth_attachment(frame_format::depth_component24, mip_width, mip_height)
                .build();

        prefilter_list.frame_buffer().set_frame_buffer(prefilter_frame_buffer);

        prefilter_list.viewport().set_rectangle(0, 0, mip_width, mip_height);

        auto roughness = static_cast<float>(mip) / static_cast<float>(max_mip_levels - 1);
        prefilter_material["roughness"] = roughness;
        for (unsigned int i = 0; i < 6; ++i)
        {
            prefilter_material["view"] = capture_views[i];

            prefilter_list.frame_buffer_texture()
                .set_attachment(frame_attachment::color)
                .set_target(image_targets[i])
                .set_texture(prefilter_cubemap)
                .set_mip_level(mip);

            prefilter_list.clear().set_clear_color(true).set_clear_depth(true);

            prefilter_list.draw()
                .set_vertex_buffer(cube_buffer)
                .set_vertex_count(36)
                .set_primitive_type(primitive_type::triangles)
                .set_material(prefilter_material);
        }
    }

    device.submit(std::move(prefilter_list), false);

    prefiltered = prefilter_cubemap;

    return model(mesh(primitive(cube_buffer, 36, std::move(cubemap_material))));
}

class app final : public application
{
    camera camera_;

    asset_importer<model> model_importer_;

    rectangle viewport_ = {0, 0, 1280, 720};

    model model_;

    texture irradiance_{};

    texture brdf_{};

    texture prefiltered_{};

    model cube_;

    glm::vec4 color_{0.8f, 0.8f, 0.8f, 1.0f};

    imgui imgui_;

    float gamma_ = 2.2f;
    float exposure_ = 1.0f;

    bool environment_ = true;
    bool rotate_ = true;

public:
    explicit app(const app_settings& settings)
        : application(settings),
          camera_(camera::builder{}
                      .set_mouse_controller(mouse_)
                      .set_perspective(glm::radians(70.0f), window_.aspect())
                      .build()),
          model_importer_(app::data_path(), graphics_),
          model_(model_importer_.load(
              app::data_path() / "Models" / "FlightHelmet" / "FlightHelmet.gltf",
              app::data_path() / "Materials" / "pbr.material")),
          cube_(make_hdr_environment_map(
              graphics_, app::data_path() / "Textures" / "gym.hdr", irradiance_, brdf_, prefiltered_)),
          imgui_(window_, keyboard_, mouse_, graphics_)
    {
    }

    static uint32_t depth_to_bits(const float depth)
    {
        // stolen from http://aras-p.info/blog/2014/01/16/rough-sorting-by-depth/
        union {
            float f = 0.0f;
            uint32_t i;
        } data{};
        data.f = depth;
        return data.i >> 22; // take highest 10 bits
    }

    static sort_key generate_sort_key(const float depth, const uint16_t material_id, const alpha_mode alpha)
    {
        // http://realtimecollisiondetection.net/blog/?p=86
        // sort by alpha, then by depth, then by material
        return static_cast<sort_key>(material_id) |
               static_cast<sort_key>(depth_to_bits(depth)) << 16 |
               static_cast<sort_key>(alpha == alpha_mode::blend) << 48;
    }

    void draw_imgui(const game_time delta_time)
    {
        imgui_.new_frame(delta_time);

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open"))
                {
                    open_file();
                }

                if (ImGui::MenuItem("Exit"))
                {
                }

                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Options"))
                {
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("View"))
            {
                if (ImGui::MenuItem("Options"))
                {
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        ImGui::SliderFloat("Gamma", &gamma_, 0.0f, 10.0f, "ratio = %.3f");
        ImGui::SliderFloat("Exposure", &exposure_, 0.0f, 10.0f, "ratio = %.3f");
        ImGui::Checkbox("Auto Rotate Camera", &rotate_);
        ImGui::Checkbox("Draw Environment", &environment_);
        ImGui::ColorEdit4("Clear Color", reinterpret_cast<float*>(&color_));

        graphics_.submit_and_swap(imgui_.draw());
    }

    void draw(const game_time delta_time) override
    {
        command_list scene_draw;

        scene_draw.viewport().set_rectangle(viewport_);

        scene_draw.scissor().set_rectangle(viewport_);

        scene_draw.clear().set_color(color_).set_clear_color(true).set_clear_depth(true);

        for (auto& mesh : model_)
        {
            for (auto& primitive : mesh)
            {
                auto& material = primitive.get_material();

                const auto distance = glm::distance(
                    mesh.get_transform().get_position(), camera_.get_position());

                material["gamma"] = gamma_;
                material["exposure"] = exposure_;
                material["irradiance_map"] = irradiance_;
                material["prefilter_map"] = prefiltered_;
                material["brdf_lut"] = brdf_;
                material["model"] = mesh.get_transform().to_matrix();
                material["view"] = camera_.get_view();
                material["projection"] = camera_.get_projection();
                material["view_pos"] = camera_.get_position();

                const auto sort_key = generate_sort_key(
                    distance, material.get_program().id, material.get_alpha_mode());

                auto& buffer = scene_draw.make_command_buffer(sort_key);

                primitive.draw(buffer);
            }
        }

        if (environment_)
        {
            for (auto& mesh : cube_)
            {
                for (auto& primitive : mesh)
                {
                    auto& material = primitive.get_material();

                    material["gamma"] = gamma_;
                    material["exposure"] = exposure_;
                    material["view"] = camera_.get_view();
                    material["projection"] = camera_.get_projection();

                    auto& buffer = scene_draw.make_command_buffer();

                    primitive.draw(buffer);
                }
            }
        }

        graphics_.submit(std::move(scene_draw));

        draw_imgui(delta_time);
    }

    void update(const game_time delta_time) override
    {
        camera_.update(delta_time);
    }

    std::filesystem::path data_path() override
    {
        // cmake-defined macro points to example project asset folder relative
        // to source. A real application could point this wherever it wanted.
        std::filesystem::path result{MOKA_ASSET_PATH};
        return result.lexically_normal();
    }
};

int main(const int argc, char* argv[])
{
    const app_settings settings(argc, argv);
    return app{settings}.run();
}
