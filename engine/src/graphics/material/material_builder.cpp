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

===========================================================================
*/
#include <fstream>
#include <graphics/device/graphics_device.hpp>
#include <graphics/material/material.hpp>
#include <graphics/material/material_builder.hpp>
#include <sstream>

namespace moka
{
    std::string material_builder::get_property_name(const material_property property)
    {
        switch (property)
        {
        case material_property::diffuse_map:
            return "material.diffuse_map";
        case material_property::emissive_map:
            return "material.emissive_map";
        case material_property::normal_map:
            return "material.normal_map";
        case material_property::metallic_roughness_map:
            return "material.metallic_roughness_map";
        case material_property::ao_map:
            return "material.ao_map";
        default:
            return "error";
        }
    }

    material_builder::material_builder(graphics_device& device)
        : graphics_device_(device)
    {
    }

    material_builder& material_builder::add_vertex_shader(const std::filesystem::path& vertex_shader)
    {
        std::ifstream vert(vertex_shader);
        std::stringstream src;
        src << vert.rdbuf();
        vertex_shaders_src_.emplace_back(src.str());
        return *this;
    }

    material_builder& material_builder::add_fragment_shader(const std::filesystem::path& fragment_shader)
    {
        std::ifstream frag(fragment_shader);
        std::stringstream src;
        src << frag.rdbuf();
        fragment_shaders_src_.emplace_back(src.str());
        return *this;
    }

    material_builder& material_builder::add_fragment_shader(const char* fragment_shader)
    {
        fragment_shaders_src_.emplace_back(fragment_shader);
        return *this;
    }

    material_builder& material_builder::add_vertex_shader(const char* vertex_shader)
    {
        vertex_shaders_src_.emplace_back(vertex_shader);
        return *this;
    }

    material_builder& material_builder::add_vertex_shader(const std::string& vertex_shader)
    {
        vertex_shaders_src_.emplace_back(vertex_shader);
        return *this;
    }

    material_builder& material_builder::set_blend_equation(const blend_equation equation)
    {
        blend_.equation = equation;
        return *this;
    }

    material_builder& material_builder::set_blend_function(
        const blend_function_factor source, const blend_function_factor destination)
    {
        blend_.source = source;
        blend_.destination = destination;
        return *this;
    }

    material_builder& material_builder::set_blend_enabled(const bool enabled)
    {
        blend_.enabled = enabled;
        return *this;
    }

    material_builder& material_builder::set_culling_enabled(const bool enabled)
    {
        culling_.enabled = enabled;
        return *this;
    }

    material_builder& material_builder::set_depth_test_enabled(const bool enabled)
    {
        depth_test_ = enabled;
        return *this;
    }

    material_builder& material_builder::set_scissor_test_enabled(const bool enabled)
    {
        scissor_test_ = enabled;
        return *this;
    }

    material_builder& material_builder::set_culling_faces(const face faces)
    {
        culling_.faces = faces;
        return *this;
    }

    material_builder& material_builder::add_fragment_shader(const std::string& fragment_shader)
    {
        fragment_shaders_src_.emplace_back(fragment_shader);
        return *this;
    }

    material_builder& material_builder::set_alpha_mode(const alpha_mode alpha_mode)
    {
        alpha_mode_ = alpha_mode;
        return *this;
    }

    material_builder& material_builder::set_polygon_mode(const face faces, const polygon_draw_mode mode)
    {
        polygon_mode_.faces = faces;
        polygon_mode_.mode = mode;
        return *this;
    }

    material_builder& material_builder::add_material_parameter(const std::string& name, const parameter_type type)
    {
        parameters_[name] = material_parameter{name, type};
        return *this;
    }

    material_builder& material_builder::add_material_parameter(const std::string& name, const float data)
    {
        parameters_[name] = material_parameter{name, parameter_type::float32, data};
        return *this;
    }

    material_builder& material_builder::add_material_parameter(const std::string& name, const texture_handle& data)
    {
        parameters_[name] = material_parameter{name, parameter_type::texture, data};
        return *this;
    }

    material_builder& material_builder::add_material_parameter(
        const std::string& name, const glm::vec3& data)
    {
        parameters_[name] = material_parameter{name, parameter_type::vec3, data};
        return *this;
    }

    material_builder& material_builder::add_material_parameter(
        const std::string& name, const glm::vec4& data)
    {
        parameters_[name] = material_parameter{name, parameter_type::vec4, data};
        return *this;
    }

    material_builder& material_builder::add_material_parameter(
        const std::string& name, const glm::mat3& data)
    {
        parameters_[name] = material_parameter{name, parameter_type::mat3, data};
        return *this;
    }

    material_builder& material_builder::add_material_parameter(
        const std::string& name, const glm::mat4& data)
    {
        parameters_[name] = material_parameter{name, parameter_type::mat4, data};
        return *this;
    }

    material_builder& material_builder::add_texture(material_property property, const texture_handle texture)
    {
        texture_maps_.emplace_back(property);
        const auto name = get_property_name(property);
        add_material_parameter(name, texture);
        return *this;
    }

    bool material_builder::replace(std::string& source, const std::string& target, const std::string& replacement)
    {
        const auto start_pos = source.find(target);
        if (start_pos == std::string::npos)
        {
            return false;
        }
        source.replace(start_pos, target.length(), replacement);
        return true;
    }

    void material_builder::build_shader_source()
    {
        std::string compiler_flags;

        if (alpha_mode_ == alpha_mode::mask)
        {
            compiler_flags.append("#define MASK_ALPHA\n");
        }

        for (const auto& property : texture_maps_)
        {
            switch (property)
            {
            case material_property::diffuse_map:
                compiler_flags.append("#define DIFFUSE_MAP\n");
                break;
            case material_property::emissive_map:
                compiler_flags.append("#define EMISSIVE_MAP\n");
                break;
            case material_property::normal_map:
                compiler_flags.append("#define NORMAL_MAP\n");
                break;
            case material_property::metallic_roughness_map:
                compiler_flags.append("#define METALLIC_ROUGHNESS_MAP\n");
                break;
            case material_property::ao_map:
                compiler_flags.append("#define AO_MAP\n");
                break;
            }
        }

        for (auto& src : vertex_shaders_src_)
        {
            src.insert(0, compiler_flags);
            src.insert(0, "#version 330 core\n");
        }

        for (auto& src : fragment_shaders_src_)
        {
            src.insert(0, compiler_flags);
            src.insert(0, "#version 330 core\n");
        }
    }

    material_handle material_builder::build()
    {
        build_shader_source();

        std::hash<std::string> hash;

        std::vector<program_handle> programs;

        for (size_t i = 0; i < vertex_shaders_src_.size(); ++i)
        {
            const auto key = hash(vertex_shaders_src_[i] + fragment_shaders_src_[i]);

            auto& cache = graphics_device_.get_program_cache();

            if (cache.exists(key))
            {
                programs.emplace_back(cache.get_program(key));
            }
            else
            {
                const auto vertex_shader = graphics_device_.make_shader(
                    shader_type::vertex, vertex_shaders_src_[i]);

                const auto fragment_shader = graphics_device_.make_shader(
                    shader_type::fragment, fragment_shaders_src_[i]);

                auto& handle = programs.emplace_back(
                    graphics_device_.make_program(vertex_shader, fragment_shader));

                cache.add_program(handle, key);
            }
        }

        material mat = {std::move(programs),
                        std::move(parameters_),
                        alpha_mode_,
                        blend_,
                        culling_,
                        polygon_mode_,
                        depth_test_,
                        scissor_test_};

        return graphics_device_.get_material_cache().add_material(std::move(mat));
    }
} // namespace moka
