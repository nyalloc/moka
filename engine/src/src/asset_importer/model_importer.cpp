
#include <asset_importer/model_importer.hpp>
#include <asset_importer/texture_importer.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <graphics/api/graphics_api.hpp>
#include <graphics/buffer/vertex_layout.hpp>
#include <graphics/buffer/vertex_layout_builder.hpp>
#include <graphics/device/graphics_device.hpp>
#include <graphics/material/material.hpp>
#include <json.hpp>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <tiny_gltf.h>

#include <fstream>
#include <iostream>
#include <vector>

namespace moka
{
    enum class buffer_target
    {
        vertex_buffer = 34962,
        index_buffer = 34963
    };

    constexpr wrap_mode gltf_wrap_to_moka(const int format)
    {
        switch (format)
        {
        case TINYGLTF_TEXTURE_WRAP_CLAMP_TO_EDGE:
            return wrap_mode::clamp_to_edge;
        case TINYGLTF_TEXTURE_WRAP_REPEAT:
            return wrap_mode::repeat;
        case TINYGLTF_TEXTURE_WRAP_MIRRORED_REPEAT:
            return wrap_mode::mirrored_repeat;
        default:;
        }
        throw std::runtime_error("Invalid pixel_format value");
    }

    constexpr min_filter gltf_min_filter_to_moka(const int format)
    {
        switch (format)
        {
        case TINYGLTF_TEXTURE_FILTER_NEAREST:
            return min_filter::nearest;
        case TINYGLTF_TEXTURE_FILTER_LINEAR:
            return min_filter::linear;
        case TINYGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_NEAREST:
            return min_filter::nearest_mipmap_nearest;
        case TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_NEAREST:
            return min_filter::linear_mipmap_nearest;
        case TINYGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_LINEAR:
            return min_filter::nearest_mipmap_linear;
        case TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_LINEAR:
            return min_filter::linear_mipmap_linear;
        default:;
        }
        throw std::runtime_error("Invalid min filter value");
    }

    constexpr mag_filter gltf_mag_filter_to_moka(const int format)
    {
        switch (format)
        {
        case TINYGLTF_TEXTURE_FILTER_NEAREST:
            return mag_filter::nearest;
        case TINYGLTF_TEXTURE_FILTER_LINEAR:
            return mag_filter::linear;
        default:;
        }
        throw std::runtime_error("Invalid mag filter value");
    }

    constexpr host_format stb_to_moka(const int format)
    {
        switch (format)
        {
        case STBI_grey:
            return host_format::r;
        case STBI_grey_alpha:
            return host_format::rg;
        case STBI_rgb:
            return host_format::rgb;
        case STBI_rgb_alpha:
            return host_format::rgba;
        case STBI_default:
            return host_format::auto_detect;
        default:;
        }
        throw std::runtime_error("Invalid pixel_format value");
    }

    constexpr int moka_to_stb(const host_format format)
    {
        switch (format)
        {
        case host_format::r:
            return STBI_grey;
        case host_format::rg:
            return STBI_grey_alpha;
        case host_format::rgb:
            return STBI_rgb;
        case host_format::rgba:
            return STBI_rgb_alpha;
        case host_format::auto_detect:
            return STBI_default;
        default:;
        }
        throw std::runtime_error("Invalid pixel_format value");
    }

    std::byte* texture_load(
        const std::filesystem::path& path, int& width, int& height, host_format& format, const host_format requested_format)
    {
        stbi_set_flip_vertically_on_load(true);

        const auto req_comp = moka_to_stb(requested_format);

        auto comp = 0;

        auto* result = stbi_load(path.string().c_str(), &width, &height, &comp, req_comp);

        format = stb_to_moka(comp);

        return reinterpret_cast<std::byte*>(result);
    }

    float* texture_load_hdr(
        const std::filesystem::path& path, int& width, int& height, host_format& format, const host_format requested_format)
    {
        stbi_set_flip_vertically_on_load(true);

        const auto req_comp = moka_to_stb(requested_format);

        auto comp = 0;

        auto* result = stbi_loadf(path.string().c_str(), &width, &height, &comp, req_comp);

        format = stb_to_moka(comp);

        return result;
    }

    void free_texture(void* data)
    {
        stbi_image_free(data);
    }

    asset_importer<model>::asset_importer(const std::filesystem::path& path, graphics_device& device)
        : device_(device), root_directory_(path)
    {
    }

    mesh load_mesh(
        const tinygltf::Model& model,
        const tinygltf::Mesh& mesh,
        graphics_device& device,
        const glm::mat4& trans,
        const std::filesystem::path& root_directory,
        const std::filesystem::path& material_path,
        const std::filesystem::path& parent_path)
    {
        auto& texture_cache = device.get_texture_cache();
        auto& shader_cache = device.get_shader_cache();
        auto& material_cache = device.get_material_cache();

        auto transform = trans;

        std::vector<primitive> primitives;

        for (const auto& primitive : mesh.primitives)
        {
            std::vector<uint8_t> index_buffer;
            std::vector<uint8_t> vertex_buffer;

            moka::vertex_buffer vertex_handle;
            moka::index_buffer index_handle;

            const auto& indices_accessor = model.accessors[primitive.indices];
            const auto& indices_buffer_view = model.bufferViews[indices_accessor.bufferView];
            const auto& indices_buffer =
                model.buffers[indices_buffer_view.buffer].data;

            std::move(
                indices_buffer.begin() + indices_buffer_view.byteOffset +
                    indices_accessor.byteOffset,
                indices_buffer.begin() + indices_buffer_view.byteOffset +
                    indices_buffer_view.byteLength,
                std::back_inserter(index_buffer));

            auto indices_count = indices_accessor.count;

            index_type type;

            switch (indices_accessor.componentType)
            {
            case 5120:
                type = index_type::int8;
                break;
            case 5121:
                type = index_type::uint8;
                break;
            case 5122:
                type = index_type::int16;
                break;
            case 5123:
                type = index_type::uint16;
                break;
            case 5125:
                type = index_type::uint32;
                break;
            case 5126:
                type = index_type::float32;
                break;
            default:;
            }

            vertex_layout::builder layout_builder;

            size_t vertices_count = 0;

            auto get_size = [](const int type) {
                switch (type)
                {
                case TINYGLTF_TYPE_SCALAR:
                    return 1;
                case TINYGLTF_TYPE_VEC2:
                    return 2;
                case TINYGLTF_TYPE_VEC3:
                    return 3;
                case TINYGLTF_TYPE_VEC4:
                    return 4;
                default:
                    throw std::runtime_error("Invalid TinyGLTF type");
                }
            };

            auto tex_coordinate_0 = primitive.attributes.find("TEXCOORD_0");
            if (tex_coordinate_0 != primitive.attributes.end())
            {
                const auto& vertices_accessor = model.accessors[tex_coordinate_0->second];
                const auto& vertices_buffer_view =
                    model.bufferViews[vertices_accessor.bufferView];
                vertices_count = vertices_accessor.count;
                const auto& vertices_buffer =
                    model.buffers[vertices_buffer_view.buffer].data;
                layout_builder.add_attribute(
                    3,
                    get_size(vertices_accessor.type),
                    attribute_type::float32,
                    vertices_accessor.normalized,
                    0,
                    vertex_buffer.size());

                std::move(
                    vertices_buffer.begin() + vertices_buffer_view.byteOffset +
                        vertices_accessor.byteOffset,
                    vertices_buffer.begin() + vertices_buffer_view.byteOffset +
                        vertices_buffer_view.byteLength,
                    std::back_inserter(vertex_buffer));
            }

            // Implementation note: When tangents are not specified, client implementations should calculate tangents using default MikkTSpace algorithms.
            // For best results, the mesh triangles should also be processed using default MikkTSpace algorithms.
            auto tangent = primitive.attributes.find("TANGENT");
            if (tangent != primitive.attributes.end())
            {
                const auto& vertices_accessor = model.accessors[tangent->second];
                const auto& vertices_buffer_view =
                    model.bufferViews[vertices_accessor.bufferView];
                vertices_count = vertices_accessor.count;
                const auto& vertices_buffer =
                    model.buffers[vertices_buffer_view.buffer].data;
                layout_builder.add_attribute(
                    2,
                    get_size(vertices_accessor.type),
                    attribute_type::float32,
                    vertices_accessor.normalized,
                    0,
                    vertex_buffer.size());

                std::move(
                    vertices_buffer.begin() + vertices_buffer_view.byteOffset +
                        vertices_accessor.byteOffset,
                    vertices_buffer.begin() + vertices_buffer_view.byteOffset +
                        vertices_buffer_view.byteLength,
                    std::back_inserter(vertex_buffer));
            }
            else
            {
                // struct mikktspace_in
                //{
                //    const tinygltf::Model& model;
                //    const tinygltf::Primitive& primitive;
                //};

                // SMikkTSpaceInterface i;
                // i.m_getNumFaces = [](const SMikkTSpaceContext* x) -> int {
                //    auto in = static_cast<mikktspace_in*>(x->m_pUserData);
                //    return 0;
                //};
                // i.m_getNumVerticesOfFace = [](const SMikkTSpaceContext* x,
                //                              const int iface) -> int {
                //    auto in = static_cast<mikktspace_in*>(x->m_pUserData);
                //    return 0;
                //};
                // i.m_getPosition = [](const SMikkTSpaceContext* x,
                //                     float pos[],
                //                     const int iface,
                //                     const int ivert) -> void {
                //    auto in = static_cast<mikktspace_in*>(x->m_pUserData);
                //};
                // i.m_getNormal = [](const SMikkTSpaceContext* x,
                //                   float pos[],
                //                   const int iface,
                //                   const int ivert) -> void {
                //    auto in = static_cast<mikktspace_in*>(x->m_pUserData);
                //};
                // i.m_getTexCoord = [](const SMikkTSpaceContext* x,
                //                     float pos[],
                //                     const int iface,
                //                     const int ivert) -> void {
                //    auto in = static_cast<mikktspace_in*>(x->m_pUserData);
                //};
                // i.m_setTSpaceBasic = [](const SMikkTSpaceContext* x,
                //                        const float tangent[],
                //                        const float sign,
                //                        const int iface,
                //                        const int ivert) -> void {
                //    auto in = static_cast<mikktspace_in*>(x->m_pUserData);
                //};
                // i.m_setTSpace = [](const SMikkTSpaceContext* x,
                //                   const float tangent[],
                //                   const float bitangent[],
                //                   const float mags,
                //                   const float magt,
                //                   const tbool orientation_preserving,
                //                   const int iface,
                //                   const int ivert) -> void {
                //    auto in = static_cast<mikktspace_in*>(x->m_pUserData);
                //};

                // SMikkTSpaceContext context;
                // context.m_pInterface = &i;

                // mikktspace_in in{model, primitive};

                // context.m_pUserData = (void*)&in;

                // genTangSpaceDefault(&context);
            }

            auto normal = primitive.attributes.find("NORMAL");
            if (normal != primitive.attributes.end())
            {
                const auto& vertices_accessor = model.accessors[normal->second];
                const auto& vertices_buffer_view =
                    model.bufferViews[vertices_accessor.bufferView];
                vertices_count = vertices_accessor.count;
                const auto& vertices_buffer =
                    model.buffers[vertices_buffer_view.buffer].data;
                layout_builder.add_attribute(
                    1,
                    get_size(vertices_accessor.type),
                    attribute_type::float32,
                    vertices_accessor.normalized,
                    0,
                    vertex_buffer.size());

                std::move(
                    vertices_buffer.begin() + vertices_buffer_view.byteOffset +
                        vertices_accessor.byteOffset,
                    vertices_buffer.begin() + vertices_buffer_view.byteOffset +
                        vertices_buffer_view.byteLength,
                    std::back_inserter(vertex_buffer));
            }

            auto position = primitive.attributes.find("POSITION");
            if (position != primitive.attributes.end())
            {
                const auto& vertices_accessor = model.accessors[position->second];
                const auto& vertices_buffer_view =
                    model.bufferViews[vertices_accessor.bufferView];
                vertices_count = vertices_accessor.count;
                const auto& vertices_buffer =
                    model.buffers[vertices_buffer_view.buffer].data;
                layout_builder.add_attribute(
                    0,
                    get_size(vertices_accessor.type),
                    attribute_type::float32,
                    vertices_accessor.normalized,
                    0,
                    vertex_buffer.size());

                std::move(
                    vertices_buffer.begin() + vertices_buffer_view.byteOffset +
                        vertices_accessor.byteOffset,
                    vertices_buffer.begin() + vertices_buffer_view.byteOffset +
                        vertices_buffer_view.byteLength,
                    std::back_inserter(vertex_buffer));
            }

            vertex_handle = device.make_vertex_buffer(
                vertex_buffer.data(), vertex_buffer.size(), layout_builder.build(), buffer_usage::static_draw);
            index_handle = device.make_index_buffer(
                index_buffer.data(), index_buffer.size(), type, buffer_usage::static_draw);

            /*
            Importing assets authored by third parties brings additional complexity - each asset may define a number of materials
            and each may be vastly different from the last. For Moka to be able to import and render assets in a uniform, generic
            way without requiring modifications to the asset, it must have a way to deal with shader permutations. When loading an
            asset, materials must be attached to a shader that is written to expect those exact inputs. A naive approach would be
            to write a simple .json file that creates a 1:1 relationship between a 3D asset and a shader, but this will not work for
            more complicated 3D assets that define a number of complex materials. Moka will feature a simple, automatic system for
            dealing with shader permutations. As materials are processed, a description of the material will be built, detailing the
            inputs and their uses. Moka will hash the material description and maintain a lookup table of currently loaded shaders,
            using the hashed value as a key. At the end of the importing process, Moka will perform a lookup to see if a shader
            capable of rendering a material of that description exists. If a shader exists, it is used. If a shader does not, it is
            created by using conditional compilation techniques, including all the code snippets necessary to deal with those material
            inputs.
            Todo: hash the shader code after adding the #defines, then use it as a key for a table of existing shaders.
            Where should this shader table live? graphics_device is a strong contender
            */

            material_builder mat_builder(device);

            mat_builder.add_uniform("view_pos", glm::vec3(0.0f));
            mat_builder.add_uniform(
                "material.diffuse_factor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
            mat_builder.add_uniform(
                "material.emissive_factor", glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
            mat_builder.add_uniform("material.roughness_factor", 1.0f);
            mat_builder.add_uniform("material.metalness_factor", 1.0f);

            std::ifstream i(material_path);
            json j;
            i >> j;

            auto vertex = j["vertex"]["file"].get<std::string>();
            auto fragment = j["fragment"]["file"].get<std::string>();

            mat_builder.set_vertex_shader(root_directory / vertex);
            mat_builder.set_fragment_shader(root_directory / fragment);

            if (primitive.material != -1)
            {
                auto& material = model.materials[primitive.material];

                if (!material.values.empty())
                {
                    if (auto base_color_factor_itr =
                            material.values.find("baseColorFactor");
                        base_color_factor_itr != material.values.end())
                    {
                        auto& data = base_color_factor_itr->second.number_array;
                        glm::vec4 diffuse_factor(data[0], data[1], data[2], data[3]);
                        mat_builder.add_uniform("material.diffuse_factor", diffuse_factor);
                    }

                    if (auto base_color_texture_itr =
                            material.values.find("baseColorTexture");
                        base_color_texture_itr != material.values.end())
                    {
                        auto& texture_name = base_color_texture_itr->first;
                        auto& texture_value = base_color_texture_itr->second;

                        auto properties = texture_value.json_double_value;

                        if (auto index_itr = properties.find("index");
                            index_itr != properties.end())
                        {
                            auto& index = index_itr->second;
                            auto& texture = model.textures[size_t(index)];
                            auto& texture_source = texture.source;
                            auto& image_data = model.images[texture_source];
                            auto uri = parent_path / image_data.uri;

                            if (!texture_cache.exists(uri.string().c_str()))
                            {
                                auto wrap_s = wrap_mode::clamp_to_edge;
                                auto wrap_t = wrap_mode::clamp_to_edge;
                                auto min = min_filter::linear_mipmap_linear;
                                auto mag = mag_filter::linear;
                                if (texture.sampler != -1)
                                {
                                    auto& sampler = model.samplers[texture.sampler];
                                    wrap_s = gltf_wrap_to_moka(sampler.wrapS);
                                    wrap_t = gltf_wrap_to_moka(sampler.wrapT);
                                    min = gltf_min_filter_to_moka(sampler.minFilter);
                                    mag = gltf_mag_filter_to_moka(sampler.magFilter);
                                }

                                auto diffuse_map =
                                    device.build_texture()
                                        .add_image_data(
                                            image_target::texture_2d,
                                            0,
                                            device_format::srgb8_alpha8,
                                            image_data.width,
                                            image_data.height,
                                            0,
                                            stb_to_moka(image_data.component),
                                            pixel_type::uint8,
                                            (void*)image_data.image.data())
                                        .set_mipmaps(true)
                                        .set_wrap_s(wrap_s)
                                        .set_wrap_t(wrap_t)
                                        .set_min_filter(min)
                                        .set_mag_filter(mag)
                                        .build();

                                mat_builder.add_texture(
                                    material_property::diffuse_map, diffuse_map);

                                texture_cache.add_texture(
                                    diffuse_map, uri.string().c_str());
                            }
                            else
                            {
                                auto diffuse_map =
                                    texture_cache.get_texture(uri.string().c_str());

                                mat_builder.add_texture(
                                    material_property::diffuse_map, diffuse_map);
                            }
                        }
                    }

                    if (auto metallic_factor_itr =
                            material.values.find("metallicFactor");
                        metallic_factor_itr != material.values.end())
                    {
                        auto& data = metallic_factor_itr->second.number_value;
                        auto metallic_factor(static_cast<float>(data));
                        mat_builder.add_uniform("material.metalness_factor", metallic_factor);
                    }

                    if (auto roughness_factor_itr =
                            material.values.find("roughnessFactor");
                        roughness_factor_itr != material.values.end())
                    {
                        auto& data = roughness_factor_itr->second.number_value;
                        auto roughness_factor(static_cast<float>(data));
                        mat_builder.add_uniform("material.roughness_factor", roughness_factor);
                    }

                    if (auto metallic_roughness_texture_itr =
                            material.values.find("metallicRoughnessTexture");
                        metallic_roughness_texture_itr != material.values.end())
                    {
                        auto& texture_name = metallic_roughness_texture_itr->first;
                        auto& texture_value = metallic_roughness_texture_itr->second;

                        auto& properties = texture_value.json_double_value;

                        if (auto index_itr = properties.find("index");
                            index_itr != properties.end())
                        {
                            auto& index = index_itr->second;
                            auto& texture = model.textures[size_t(index)];
                            auto& texture_source = texture.source;
                            auto& image_data = model.images[texture_source];
                            auto uri = parent_path / image_data.uri;

                            if (!texture_cache.exists(uri.string().c_str()))
                            {
                                auto wrap_s = wrap_mode::clamp_to_edge;
                                auto wrap_t = wrap_mode::clamp_to_edge;
                                auto min = min_filter::linear_mipmap_linear;
                                auto mag = mag_filter::linear;
                                if (texture.sampler != -1)
                                {
                                    auto& sampler = model.samplers[texture.sampler];
                                    wrap_s = gltf_wrap_to_moka(sampler.wrapS);
                                    wrap_t = gltf_wrap_to_moka(sampler.wrapT);
                                    min = gltf_min_filter_to_moka(sampler.minFilter);
                                    mag = gltf_mag_filter_to_moka(sampler.magFilter);
                                }

                                auto metallic_roughness_map =
                                    device.build_texture()
                                        .add_image_data(
                                            image_target::texture_2d,
                                            0,
                                            device_format::rgba,
                                            image_data.width,
                                            image_data.height,
                                            0,
                                            stb_to_moka(image_data.component),
                                            pixel_type::uint8,
                                            (void*)image_data.image.data())
                                        .set_mipmaps(true)
                                        .set_wrap_s(wrap_s)
                                        .set_wrap_t(wrap_t)
                                        .set_min_filter(min)
                                        .set_mag_filter(mag)
                                        .build();

                                mat_builder.add_texture(
                                    material_property::metallic_roughness_map,
                                    metallic_roughness_map);

                                texture_cache.add_texture(
                                    metallic_roughness_map, uri.string().c_str());
                            }
                            else
                            {
                                auto metallic_roughness_map =
                                    texture_cache.get_texture(uri.string().c_str());

                                mat_builder.add_texture(
                                    material_property::metallic_roughness_map,
                                    metallic_roughness_map);
                            }
                        }
                    }
                }

                if (!material.additionalValues.empty())
                {
                    if (auto name_itr = material.additionalValues.find("name");
                        name_itr != material.additionalValues.end())
                    {
                    }

                    if (auto normal_texture_itr = material.additionalValues.find("normalTexture");
                        normal_texture_itr != material.additionalValues.end())
                    {
                        auto& texture_name = normal_texture_itr->first;
                        auto& texture_value = normal_texture_itr->second;

                        auto& properties = texture_value.json_double_value;

                        if (auto index_itr = properties.find("index");
                            index_itr != properties.end())
                        {
                            auto& index = index_itr->second;
                            auto& texture = model.textures[size_t(index)];
                            auto& texture_source = texture.source;
                            auto& image_data = model.images[texture_source];
                            auto uri = parent_path / image_data.uri;

                            if (!texture_cache.exists(uri.string().c_str()))
                            {
                                auto wrap_s = wrap_mode::clamp_to_edge;
                                auto wrap_t = wrap_mode::clamp_to_edge;
                                auto min = min_filter::linear_mipmap_linear;
                                auto mag = mag_filter::linear;
                                if (texture.sampler != -1)
                                {
                                    auto& sampler = model.samplers[texture.sampler];
                                    wrap_s = gltf_wrap_to_moka(sampler.wrapS);
                                    wrap_t = gltf_wrap_to_moka(sampler.wrapT);
                                    min = gltf_min_filter_to_moka(sampler.minFilter);
                                    mag = gltf_mag_filter_to_moka(sampler.magFilter);
                                }

                                auto normal_map =
                                    device.build_texture()
                                        .add_image_data(
                                            image_target::texture_2d,
                                            0,
                                            device_format::rgba,
                                            image_data.width,
                                            image_data.height,
                                            0,
                                            stb_to_moka(image_data.component),
                                            pixel_type::uint8,
                                            (void*)image_data.image.data())
                                        .set_mipmaps(true)
                                        .set_wrap_s(wrap_s)
                                        .set_wrap_t(wrap_t)
                                        .set_min_filter(min)
                                        .set_mag_filter(mag)
                                        .build();

                                mat_builder.add_texture(
                                    material_property::normal_map, normal_map);

                                texture_cache.add_texture(
                                    normal_map, uri.string().c_str());
                            }
                            else
                            {
                                auto normal_map =
                                    texture_cache.get_texture(uri.string().c_str());

                                mat_builder.add_texture(
                                    material_property::normal_map, normal_map);
                            }
                        }
                    }

                    if (auto occlusion_texture_itr =
                            material.additionalValues.find("occlusionTexture");
                        occlusion_texture_itr != material.additionalValues.end())
                    {
                        auto& texture_name = occlusion_texture_itr->first;
                        auto& texture_value = occlusion_texture_itr->second;

                        auto& properties = texture_value.json_double_value;

                        if (auto index_itr = properties.find("index");
                            index_itr != properties.end())
                        {
                            auto& index = index_itr->second;
                            auto& texture = model.textures[size_t(index)];
                            auto& texture_source = texture.source;
                            auto& image_data = model.images[texture_source];
                            auto uri = parent_path / image_data.uri;

                            if (!texture_cache.exists(uri.string().c_str()))
                            {
                                auto wrap_s = wrap_mode::clamp_to_edge;
                                auto wrap_t = wrap_mode::clamp_to_edge;
                                auto min = min_filter::linear_mipmap_linear;
                                auto mag = mag_filter::linear;
                                if (texture.sampler != -1)
                                {
                                    auto& sampler = model.samplers[texture.sampler];
                                    wrap_s = gltf_wrap_to_moka(sampler.wrapS);
                                    wrap_t = gltf_wrap_to_moka(sampler.wrapT);
                                    min = gltf_min_filter_to_moka(sampler.minFilter);
                                    mag = gltf_mag_filter_to_moka(sampler.magFilter);
                                }

                                auto occlusion_map =
                                    device.build_texture()
                                        .add_image_data(
                                            image_target::texture_2d,
                                            0,
                                            device_format::rgba,
                                            image_data.width,
                                            image_data.height,
                                            0,
                                            stb_to_moka(image_data.component),
                                            pixel_type::uint8,
                                            (void*)image_data.image.data())
                                        .set_mipmaps(true)
                                        .set_wrap_s(wrap_s)
                                        .set_wrap_t(wrap_t)
                                        .set_min_filter(min)
                                        .set_mag_filter(mag)
                                        .build();

                                mat_builder.add_texture(material_property::ao_map, occlusion_map);

                                texture_cache.add_texture(
                                    occlusion_map, uri.string().c_str());
                            }
                            else
                            {
                                auto occlusion_map =
                                    texture_cache.get_texture(uri.string().c_str());

                                mat_builder.add_texture(material_property::ao_map, occlusion_map);
                            }
                        }
                    }

                    if (auto emissive_factor_itr =
                            material.additionalValues.find("emissiveFactor");
                        emissive_factor_itr != material.additionalValues.end())
                    {
                        auto data = emissive_factor_itr->second.number_array;
                        glm::vec4 emissive_factor(data[0], data[1], data[2], 1.0f);
                        mat_builder.add_uniform("material.emissive_factor", emissive_factor);
                    }

                    if (auto emissive_texture_itr =
                            material.additionalValues.find("emissiveTexture");
                        emissive_texture_itr != material.additionalValues.end())
                    {
                        auto& texture_name = emissive_texture_itr->first;
                        auto& texture_value = emissive_texture_itr->second;

                        auto& properties = texture_value.json_double_value;

                        if (auto index_itr = properties.find("index");
                            index_itr != properties.end())
                        {
                            auto& index = index_itr->second;
                            auto& texture = model.textures[size_t(index)];
                            auto& texture_source = texture.source;
                            auto& image_data = model.images[texture_source];
                            auto uri = parent_path / image_data.uri;

                            if (!texture_cache.exists(uri.string().c_str()))
                            {
                                auto wrap_s = wrap_mode::clamp_to_edge;
                                auto wrap_t = wrap_mode::clamp_to_edge;
                                auto min = min_filter::linear_mipmap_linear;
                                auto mag = mag_filter::linear;
                                if (texture.sampler != -1)
                                {
                                    auto& sampler = model.samplers[texture.sampler];
                                    wrap_s = gltf_wrap_to_moka(sampler.wrapS);
                                    wrap_t = gltf_wrap_to_moka(sampler.wrapT);
                                    min = gltf_min_filter_to_moka(sampler.minFilter);
                                    mag = gltf_mag_filter_to_moka(sampler.magFilter);
                                }

                                auto emissive_map =
                                    device.build_texture()
                                        .add_image_data(
                                            image_target::texture_2d,
                                            0,
                                            device_format::srgb8_alpha8,
                                            image_data.width,
                                            image_data.height,
                                            0,
                                            stb_to_moka(image_data.component),
                                            pixel_type::uint8,
                                            (void*)image_data.image.data())
                                        .set_mipmaps(true)
                                        .set_wrap_s(wrap_s)
                                        .set_wrap_t(wrap_t)
                                        .set_min_filter(min)
                                        .set_mag_filter(mag)
                                        .build();

                                mat_builder.add_texture(
                                    material_property::emissive_map, emissive_map);

                                texture_cache.add_texture(
                                    emissive_map, uri.string().c_str());
                            }
                            else
                            {
                                auto emissive_map =
                                    texture_cache.get_texture(uri.string().c_str());

                                mat_builder.add_texture(
                                    material_property::emissive_map, emissive_map);
                            }
                        }
                    }

                    // The alpha cutoff value of the material.
                    auto alpha_cutoff = 0.5f;

                    if (auto alpha_cutoff_itr = material.additionalValues.find("alphaCutoff");
                        alpha_cutoff_itr != material.additionalValues.end())
                    {
                        auto alpha_cutoff_name = alpha_cutoff_itr->first;
                        auto alpha_cutoff_value = alpha_cutoff_itr->second;

                        alpha_cutoff = static_cast<float>(alpha_cutoff_value.number_value);
                    }

                    auto alpha = alpha_mode::opaque;

                    if (auto alpha_mode_itr =
                            material.additionalValues.find("alphaMode");
                        alpha_mode_itr != material.additionalValues.end())
                    {
                        auto alpha_mode_name = alpha_mode_itr->first;
                        auto alpha_mode_value = alpha_mode_itr->second;

                        auto alpha_str = alpha_mode_value.string_value;

                        if (alpha_str == "OPAQUE")
                        {
                            alpha = alpha_mode::opaque;
                        }
                        else if (alpha_str == "MASK")
                        {
                            alpha = alpha_mode::mask;
                            mat_builder.add_uniform("material.alpha_cutoff", alpha_cutoff);
                            mat_builder.set_blend_enabled(true);
                        }
                        else if (alpha_str == "BLEND")
                        {
                            alpha = alpha_mode::blend;
                            mat_builder.set_blend_enabled(true);
                        }
                    }

                    mat_builder.set_alpha_mode(alpha);
                }
            }

            index_buffer.clear();
            vertex_buffer.clear();

            primitives.emplace_back(
                vertex_handle,
                vertices_count,
                index_handle,
                type,
                indices_count,
                0,
                mat_builder.build());
        }

        return moka::mesh{std::move(primitives), transform::from_matrix(transform)};
    }

    glm::mat4 get_transform(const tinygltf::Node& n)
    {
        auto model_translation = n.translation;
        auto model_rotation = n.rotation;
        auto model_scale = n.scale;
        auto model_matrix = n.matrix;

        if (!model_matrix.empty())
        {
            glm::mat4 matrix(
                model_matrix[0],
                model_matrix[1],
                model_matrix[2],
                model_matrix[3],
                model_matrix[4],
                model_matrix[5],
                model_matrix[6],
                model_matrix[7],
                model_matrix[8],
                model_matrix[9],
                model_matrix[10],
                model_matrix[11],
                model_matrix[12],
                model_matrix[13],
                model_matrix[14],
                model_matrix[15]);
            return matrix;
        }

        transform trans;
        if (!model_translation.empty())
        {
            std::vector<float> translation(
                model_translation.begin(), model_translation.end());
            trans.set_position({translation[0], translation[1], translation[2]});
        }
        if (!model_scale.empty())
        {
            std::vector<float> scale(model_scale.begin(), model_scale.end());
            trans.set_scale({scale[0], scale[1], scale[2]});
        }
        if (!model_rotation.empty())
        {
            std::vector<float> rotation(model_rotation.begin(), model_rotation.end());
            trans.set_rotation({rotation[3], rotation[0], rotation[1], rotation[2]});
        }

        return trans.to_matrix();
    }

    void add_node(
        const glm::mat4& parent_transform,
        std::vector<mesh>& meshes,
        const int node_id,
        const tinygltf::Model& model,
        graphics_device& device,
        const std::filesystem::path& root_directory,
        const std::filesystem::path& material_path,
        const std::filesystem::path& parent_path)
    {
        const auto mesh_id = model.nodes[node_id].mesh;

        if (mesh_id == -1)
            return;

        const auto trans = parent_transform * get_transform(model.nodes[node_id]);

        for (const auto i : model.nodes[node_id].children)
        {
            add_node(trans, meshes, i, model, device, root_directory, material_path, parent_path);
        }

        meshes.emplace_back(load_mesh(
            model, model.meshes[mesh_id], device, trans, root_directory, material_path, parent_path));
    }

    model load_model(
        const tinygltf::Model& model,
        graphics_device& device,
        const std::filesystem::path& root_directory,
        const std::filesystem::path& material_path,
        const std::filesystem::path& parent_path)
    {
        std::vector<mesh> meshes;

        for (const auto& scene : model.scenes)
        {
            for (const auto& node : scene.nodes)
            {
                auto transform = get_transform(model.nodes[node]);

                if (model.nodes[node].mesh != -1)
                {
                    const auto mesh_id = model.nodes[node].mesh;

                    if (mesh_id == -1)
                        continue;

                    const auto trans = get_transform(model.nodes[node]);

                    for (const auto i : model.nodes[node].children)
                    {
                        add_node(trans, meshes, i, model, device, root_directory, material_path, parent_path);
                    }

                    meshes.emplace_back(load_mesh(
                        model, model.meshes[mesh_id], device, trans, root_directory, material_path, parent_path));
                }

                for (const auto i : model.nodes[node].children)
                {
                    add_node(transform, meshes, i, model, device, root_directory, material_path, parent_path);
                }
            }
        }

        return moka::model{std::move(meshes)};
    }

    model asset_importer<model>::load(
        const std::filesystem::path& model_path, const std::filesystem::path& material_path)
    {
        tinygltf::Model model;
        tinygltf::TinyGLTF gltf_ctx;

        std::string err;
        std::string warn;

        auto absolute_model_path = root_directory_ / model_path;
        auto absolute_material_path = root_directory_ / material_path;

        const auto ext = absolute_model_path.extension();

        auto ret = false;

        if (ext == ".glb")
        {
            std::cout << "Reading binary glTF" << std::endl;
            // assume binary glTF.
            ret = gltf_ctx.LoadBinaryFromFile(
                &model, &err, &warn, absolute_model_path.string());
        }
        else if (ext == ".gltf")
        {
            std::cout << "Reading ASCII glTF" << std::endl;
            // assume ascii glTF.
            ret = gltf_ctx.LoadASCIIFromFile(
                &model, &err, &warn, absolute_model_path.string());
        }
        else
        {
            std::cout << "Attempting to load non-glTF asset." << std::endl;
        }

        if (!warn.empty())
        {
            std::cout << "Warn: " << warn.c_str() << std::endl;
        }

        if (!err.empty())
        {
            std::cout << "Err: " << err.c_str() << std::endl;
        }

        if (!ret)
        {
            std::cout << "Failed to parse glTF: " << std::endl;
        }

        return load_model(
            model,
            device_,
            root_directory_,
            absolute_material_path,
            absolute_model_path.parent_path());
    }
} // namespace moka
