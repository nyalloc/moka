#pragma once

#include <graphics/material_properties.hpp>
#include <graphics/graphics_device.hpp>
#include <graphics/material.hpp>
#include <graphics/texture_2d.hpp>
#include <filesystem>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

namespace moka
{
	class material_builder
	{
		graphics_device& graphics_device_;
		std::vector<material_property> texture_maps_;
		std::map<std::string, program_handle>& shaders_;
		parameter_collection parameters_;
		std::string fragment_shader_src_;
		std::string vertex_shader_src_;
		alpha_mode alpha_mode_ = alpha_mode::opaque;
	public:
		static std::string get_property_name(const material_property property)
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
				break;
			}
		}
			
		material_builder(graphics_device& device, std::map<std::string, program_handle>& shaders)
			: graphics_device_(device)
			, shaders_{ shaders }
			, parameters_
			{ effect_parameter("view_pos",                  glm::vec3(0.0f))
			, effect_parameter("material.diffuse_factor",   glm::vec3(1.0f))
			, effect_parameter("material.emissive_factor",  glm::vec3(0.0f))
			, effect_parameter("material.roughness_factor", 1.0f           )
			, effect_parameter("material.metalness_factor", 1.0f           ) 
			}
		{}

		material_builder& set_vertex_shader(const std::filesystem::path& vertex_shader)
		{
			std::ifstream vert(vertex_shader);
			std::stringstream src;
			src << vert.rdbuf();
			vertex_shader_src_ = src.str();
			return *this;
		}

		material_builder& set_fragment_shader(const std::filesystem::path& fragment_shader)
		{
			std::ifstream frag(fragment_shader);
			std::stringstream src;
			src << frag.rdbuf();
			fragment_shader_src_ = src.str();
			return *this;
		}

		material_builder& set_vertex_shader(const std::string& vertex_shader)
		{
			vertex_shader_src_ = vertex_shader;
			return *this;
		}

		material_builder& set_fragment_shader(const std::string& fragment_shader)
		{
			fragment_shader_src_ = fragment_shader;
			return *this;
		}

		material_builder& set_alpha_mode(const alpha_mode alpha_mode)
		{
			alpha_mode_ = alpha_mode;
			return *this;
		}

		material_builder& add_uniform(const std::string& name, const float data)
		{
			parameters_[name] = effect_parameter{ name, parameter_type::float32, data };
			return *this;
		}

		material_builder& add_uniform(const std::string& name, const texture_2d& data)
		{
			parameters_[name] = effect_parameter{ name, parameter_type::texture, data };
			return *this;
		}

		material_builder& add_uniform(const std::string& name, const glm::vec3& data)
		{
			parameters_[name] = effect_parameter{ name, parameter_type::vec3, data };
			return *this;
		}

		material_builder& add_uniform(const std::string& name, const glm::vec4& data)
		{
			parameters_[name] = effect_parameter{ name, parameter_type::vec4, data };
			return *this;
		}

		material_builder& add_uniform(const std::string& name, const glm::mat3& data)
		{
			parameters_[name] = effect_parameter{ name, parameter_type::mat3, data };
			return *this;
		}

		material_builder& add_uniform(const std::string& name, const glm::mat4& data)
		{
			parameters_[name] = effect_parameter{ name, parameter_type::mat4, data };
			return *this;
		}

		material_builder& add_texture(material_property property, const texture_2d texture)
		{
			texture_maps_.emplace_back(property);
			auto name = get_property_name(property);
			add_uniform(name, texture);
			return *this;
		}

		bool replace(std::string& source, const std::string& target, const std::string& replacement) 
		{
			size_t start_pos = source.find(target);
			if (start_pos == std::string::npos)
			{
				return false;
			}
			source.replace(start_pos, target.length(), replacement);
			return true;
		}

		material build()
		{
			std::string compiler_flags;

			switch (alpha_mode_)
			{
			case alpha_mode::blend:
				break;
			case alpha_mode::mask:
				compiler_flags.append("#define MASK_ALPHA\n");
				break;
			case alpha_mode::opaque:
				break;
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

			program_handle program;

			replace(vertex_shader_src_, "#moka_compilation_flags\n", compiler_flags);
			replace(fragment_shader_src_, "#moka_compilation_flags\n", compiler_flags);

			auto key = vertex_shader_src_ + fragment_shader_src_;

			auto it = shaders_.find(key);
			if (it != shaders_.end())
			{
				program = it->second;
			}
			else
			{
				auto vertex_shader = graphics_device_.create_shader(
					shader_type::vertex
					, vertex_shader_src_);

				auto fragment_shader = graphics_device_.create_shader(
					shader_type::fragment
					, fragment_shader_src_);

				 program = graphics_device_.create_program(vertex_shader, fragment_shader);

				 shaders_[key] = program;
			}

			return { program, std::move(parameters_), alpha_mode_ };
		}
	};
}