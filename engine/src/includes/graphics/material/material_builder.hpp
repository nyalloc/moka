#pragma once

#include <filesystem>
#include <graphics/material/material.hpp>
#include <graphics/material/material_properties.hpp>
#include <graphics/texture.hpp>
#include <map>
#include <string>
#include <vector>

namespace moka
{
    class graphics_device;

    class material_builder
    {
        graphics_device& graphics_device_;

        std::vector<material_property> texture_maps_;

        parameter_collection parameters_;
        std::string fragment_shader_src_;
        std::string vertex_shader_src_;

        alpha_mode alpha_mode_ = alpha_mode::opaque;
        blend blend_;
        culling culling_;
        polygon_mode polygon_mode_;
        bool scissor_test_ = false;
        bool depth_test_ = true;

    public:
        static std::string get_property_name(material_property property);

        material_builder(graphics_device& device);

        material_builder& set_vertex_shader(const std::filesystem::path& vertex_shader);

        material_builder& set_vertex_shader(const char* vertex_shader);

        material_builder& set_vertex_shader(const std::string& vertex_shader);

        material_builder& set_fragment_shader(const std::filesystem::path& fragment_shader);

        material_builder& set_fragment_shader(const char* fragment_shader);

        material_builder& set_fragment_shader(const std::string& fragment_shader);

        material_builder& set_blend_equation(blend_equation equation);

        material_builder& set_blend_function(blend_function_factor source, blend_function_factor destination);

        material_builder& set_blend_enabled(bool enabled);

        material_builder& set_culling_enabled(bool enabled);

        material_builder& set_depth_test_enabled(bool enabled);

        material_builder& set_scissor_test_enabled(bool enabled);

        material_builder& set_culling_faces(face faces);

        material_builder& set_alpha_mode(alpha_mode alpha_mode);

        material_builder& set_polygon_mode(face faces, polygon_draw_mode mode);

        material_builder& add_uniform(const std::string& name, parameter_type type);

        material_builder& add_uniform(const std::string& name, float data);

        material_builder& add_uniform(const std::string& name, const texture& data);

        material_builder& add_uniform(const std::string& name, const glm::vec3& data);

        material_builder& add_uniform(const std::string& name, const glm::vec4& data);

        material_builder& add_uniform(const std::string& name, const glm::mat3& data);

        material_builder& add_uniform(const std::string& name, const glm::mat4& data);

        material_builder& add_texture(material_property property, texture texture);

        static bool replace(std::string& source, const std::string& target, const std::string& replacement);

        material_handle build();
    };
} // namespace moka
