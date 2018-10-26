#include <asset_importer/model_importer.hpp>
#include <asset_importer/texture_importer.hpp>
#include <graphics/graphics_device.hpp>
#include <graphics/material_builder.hpp>
#include <graphics/graphics_api.hpp>
#include <graphics/material_builder.hpp>
#include <graphics/vertex_layout_builder.hpp>
#include <graphics/vertex_layout.hpp>
#include "../deps/nlohmann/json.hpp"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <tiny_gltf.h>
#include <GL/glew.h>

#include <cstdio>
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

	constexpr texture_components stb_to_moka(int format)
	{
		switch (format)
		{
		case STBI_grey:
			return texture_components::grey;
		case STBI_grey_alpha:
			return texture_components::grey_alpha;
		case STBI_rgb:
			return texture_components::rgb;
		case STBI_rgb_alpha:
			return texture_components::rgb_alpha;
		case STBI_default:
			return texture_components::auto_detect;
		}
		throw std::runtime_error("Invalid texture_components value");
	}

	constexpr int moka_to_stb(texture_components format)
	{
		switch (format)
		{
		case texture_components::grey:
			return STBI_grey;
		case texture_components::grey_alpha:
			return STBI_grey_alpha;
		case texture_components::rgb:
			return STBI_rgb;
		case texture_components::rgb_alpha:
			return STBI_rgb_alpha;
		case texture_components::auto_detect:
			return STBI_default;
		}
		throw std::runtime_error("Invalid texture_components value");
	}

	void load_texture(
		const std::filesystem::path& path,
		void* data,
		glm::ivec2& resolution,
		texture_components& components,
		texture_components components_requested)
	{
		if (!std::filesystem::exists(path))
		{
			throw std::runtime_error("Texture specified at " + path.string() + " cannot be found.");
		}

		stbi_set_flip_vertically_on_load(true);

		int n;

		auto path_string = path.string();
		auto path_c_string = path_string.c_str();

		data = stbi_load(path_c_string, &resolution.x, &resolution.y, &n, moka_to_stb(components));

		components = stb_to_moka(n);
	}

	void free_texture(void* data)
	{
		stbi_image_free(data);
	}

	asset_importer<model>::asset_importer(const std::filesystem::path& path, graphics_device& device)
		: device_(device), root_directory_(path)
	{}

	mesh load_mesh(const tinygltf::Model& model, const tinygltf::Mesh& mesh, graphics_device& device, transform& trans, const std::filesystem::path& root_path, std::map<std::string, program>& shaders)
	{
		auto transform = trans;

		std::vector<moka::primitive> primitives;

		for (size_t i = 0; i < mesh.primitives.size(); i++)
		{
			const auto& primitive = mesh.primitives[i];

			std::vector<uint8_t> index_buffer;
			std::vector<uint8_t> vertex_buffer;

			moka::vertex_buffer vertex_handle;
			moka::index_buffer index_handle;

			const auto& indices_accessor = model.accessors[primitive.indices];
			const auto& indices_buffer_view = model.bufferViews[indices_accessor.bufferView];
			const auto& indices_buffer = model.buffers[indices_buffer_view.buffer].data;

			index_buffer.insert(
				index_buffer.end(),
				indices_buffer.begin() + indices_buffer_view.byteOffset,
				indices_buffer.begin() + indices_buffer_view.byteOffset + indices_buffer_view.byteLength);

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
			}

			vertex_layout::builder layout_builder;

			size_t vertices_count = 0;

			auto get_size = [](int type) 
			{
				switch (type)
				{
				case TINYGLTF_TYPE_SCALAR: return 1;
				case TINYGLTF_TYPE_VEC2: return 2;
				case TINYGLTF_TYPE_VEC3: return 3;
				case TINYGLTF_TYPE_VEC4: return 4;
				default:
					throw std::runtime_error("Invalid TinyGLTF type");
				}
			};

			auto tex_coord0 = primitive.attributes.find("TEXCOORD_0");
			if (tex_coord0 != primitive.attributes.end())
			{
				const auto& vertices_accessor = model.accessors[tex_coord0->second];
				auto vertices_buffer_view = model.bufferViews[vertices_accessor.bufferView];
				vertices_count = vertices_accessor.count;
				const auto& vertices_buffer = model.buffers[vertices_buffer_view.buffer].data;
				layout_builder.add_attribute(3, attribute_type::float32, get_size(vertices_accessor.type), vertices_accessor.normalized, 0, vertex_buffer.size());
				vertex_buffer.insert(
					vertex_buffer.end(),
					vertices_buffer.begin() + vertices_buffer_view.byteOffset,
					vertices_buffer.begin() + vertices_buffer_view.byteOffset + vertices_buffer_view.byteLength);
			}

			auto tangent = primitive.attributes.find("TANGENT");
			if (tangent != primitive.attributes.end())
			{
				const auto& vertices_accessor = model.accessors[tangent->second];
				auto vertices_buffer_view = model.bufferViews[vertices_accessor.bufferView];
				vertices_count = vertices_accessor.count;
				const auto& vertices_buffer = model.buffers[vertices_buffer_view.buffer].data;
				layout_builder.add_attribute(2, attribute_type::float32, get_size(vertices_accessor.type), vertices_accessor.normalized, 0, vertex_buffer.size());
				vertex_buffer.insert(
					vertex_buffer.end(),
					vertices_buffer.begin() + vertices_buffer_view.byteOffset,
					vertices_buffer.begin() + vertices_buffer_view.byteOffset + vertices_buffer_view.byteLength);
			}

			auto normal = primitive.attributes.find("NORMAL");
			if (normal != primitive.attributes.end())
			{
				const auto& vertices_accessor = model.accessors[normal->second];
				auto vertices_buffer_view = model.bufferViews[vertices_accessor.bufferView];
				vertices_count = vertices_accessor.count;
				const auto& vertices_buffer = model.buffers[vertices_buffer_view.buffer].data;
				layout_builder.add_attribute(1, attribute_type::float32, get_size(vertices_accessor.type), vertices_accessor.normalized, 0, vertex_buffer.size());
				vertex_buffer.insert(
					vertex_buffer.end(),
					vertices_buffer.begin() + vertices_buffer_view.byteOffset,
					vertices_buffer.begin() + vertices_buffer_view.byteOffset + vertices_buffer_view.byteLength);
			}

			auto position = primitive.attributes.find("POSITION");
			if (position != primitive.attributes.end())
			{
				const auto& vertices_accessor = model.accessors[position->second];
				auto vertices_buffer_view = model.bufferViews[vertices_accessor.bufferView];
				vertices_count = vertices_accessor.count;
				const auto& vertices_buffer = model.buffers[vertices_buffer_view.buffer].data;
				layout_builder.add_attribute(0, attribute_type::float32, get_size(vertices_accessor.type), vertices_accessor.normalized, 0, vertex_buffer.size());
				vertex_buffer.insert(
					vertex_buffer.end(),
					vertices_buffer.begin() + vertices_buffer_view.byteOffset,
					vertices_buffer.begin() + vertices_buffer_view.byteOffset + vertices_buffer_view.byteLength);
			}

			vertex_handle = device.make_vertex_buffer(vertex_buffer.data(), vertex_buffer.size(), layout_builder.build(), buffer_usage::static_draw);
			index_handle = device.make_index_buffer(index_buffer.data(), index_buffer.size(), type, buffer_usage::static_draw);

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

			material_builder mat_builder(device, shaders);

			mat_builder.add_uniform("view_pos", glm::vec3(0.0f));
			mat_builder.add_uniform("material.diffuse_factor", glm::vec3(1.0f));
			mat_builder.add_uniform("material.emissive_factor", glm::vec3(0.0f));
			mat_builder.add_uniform("material.roughness_factor", 1.0f);
			mat_builder.add_uniform("material.metalness_factor", 1.0f);

			mat_builder.set_vertex_shader(root_path / "Materials" / "Shaders" / "pbr.vert");
			mat_builder.set_fragment_shader(root_path / "Materials" / "Shaders" / "pbr.frag");

			if (primitive.material != -1)
			{
				auto material = model.materials[primitive.material];

				if (!material.values.empty())
				{
					if (auto base_color_factor_itr = material.values.find("baseColorFactor"); base_color_factor_itr != material.values.end())
					{
						auto data = base_color_factor_itr->second.number_array;
						glm::vec3 diffuse_factor(data[0], data[1], data[2]);
						mat_builder.add_uniform("material.diffuse_factor", diffuse_factor);
					}

					if (auto base_color_texture_itr = material.values.find("baseColorTexture"); base_color_texture_itr != material.values.end())
					{
						auto texture_name = base_color_texture_itr->first;
						auto texture_value = base_color_texture_itr->second;

						auto properties = texture_value.json_double_value;

						if (auto index_itr = properties.find("index"); index_itr != properties.end())
						{
							auto index = index_itr->second;
							auto texture = model.textures[size_t(index)];
							auto texture_source = texture.source;

							auto image_data = model.images[texture_source];

							auto diffuse_map = device.make_texture(
								image_data.image.data(),
								glm::ivec2{ image_data.width, image_data.height },
								stb_to_moka(image_data.component),
								{},
								true,
								false);

							mat_builder.add_texture(material_property::diffuse_map, diffuse_map);
						}
					}

					if (auto metallic_factor_itr = material.values.find("metallicFactor"); metallic_factor_itr != material.values.end())
					{
						auto data = metallic_factor_itr->second.number_value;
						float metallic_factor(static_cast<float>(data));
						mat_builder.add_uniform("material.metalness_factor", metallic_factor);
					}

					if (auto roughness_factor_itr = material.values.find("roughnessFactor"); roughness_factor_itr != material.values.end())
					{
						auto data = roughness_factor_itr->second.number_value;
						auto roughness_factor(static_cast<float>(data));
						mat_builder.add_uniform("material.roughness_factor", roughness_factor);
					}

					if (auto metallic_roughness_texture_itr = material.values.find("metallicRoughnessTexture"); metallic_roughness_texture_itr != material.values.end())
					{
						auto texture_name = metallic_roughness_texture_itr->first;
						auto texture_value = metallic_roughness_texture_itr->second;

						auto properties = texture_value.json_double_value;

						if (auto index_itr = properties.find("index"); index_itr != properties.end())
						{
							auto index = index_itr->second;
							auto texture = model.textures[size_t(index)];
							auto texture_source = texture.source;

							auto image_data = model.images[texture_source];

							auto metallic_roughness_map = device.make_texture(
								image_data.image.data(),
								glm::ivec2{ image_data.width, image_data.height },
								stb_to_moka(image_data.component),
								{},
								true,
								false);

							mat_builder.add_texture(material_property::metallic_roughness_map, metallic_roughness_map);
						}
					}
				}

				if (!material.additionalValues.empty())
				{
					if (auto name_itr = material.additionalValues.find("name"); name_itr != material.additionalValues.end())
					{

					}

					if (auto normal_texture_itr = material.additionalValues.find("normalTexture"); normal_texture_itr != material.additionalValues.end())
					{
						auto texture_name = normal_texture_itr->first;
						auto texture_value = normal_texture_itr->second;

						auto properties = texture_value.json_double_value;

						if (auto index_itr = properties.find("index"); index_itr != properties.end())
						{
							auto index = index_itr->second;
							auto texture = model.textures[size_t(index)];
							auto texture_source = texture.source;

							auto image_data = model.images[texture_source];

							auto normal_map = device.make_texture(
								image_data.image.data(),
								glm::ivec2{ image_data.width, image_data.height },
								stb_to_moka(image_data.component),
								{},
								true,
								false);

							mat_builder.add_texture(material_property::normal_map, normal_map);
						}
					}

					if (auto occlusion_texture_itr = material.additionalValues.find("occlusionTexture"); occlusion_texture_itr != material.additionalValues.end())
					{
						auto texture_name = occlusion_texture_itr->first;
						auto texture_value = occlusion_texture_itr->second;

						auto properties = texture_value.json_double_value;

						if (auto index_itr = properties.find("index"); index_itr != properties.end())
						{
							auto index = index_itr->second;
							auto texture = model.textures[size_t(index)];
							auto texture_source = texture.source;

							auto image_data = model.images[texture_source];

							auto occlusion_map = device.make_texture(
								image_data.image.data(), 
								glm::ivec2{ image_data.width, image_data.height },
								stb_to_moka(image_data.component), 
								{}, 
								true,
								false);

							mat_builder.add_texture(material_property::ao_map, occlusion_map);
						}
					}

					if (auto emissive_factor_itr = material.additionalValues.find("emissiveFactor"); emissive_factor_itr != material.additionalValues.end())
					{
						auto data = emissive_factor_itr->second.number_array;
						glm::vec3 emissive_factor(data[0], data[1], data[2]);
						mat_builder.add_uniform("material.emissive_factor", emissive_factor);
					}

					if (auto emissive_texture_itr = material.additionalValues.find("emissiveTexture"); emissive_texture_itr != material.additionalValues.end())
					{
						auto texture_name = emissive_texture_itr->first;
						auto texture_value = emissive_texture_itr->second;

						auto properties = texture_value.json_double_value;

						if (auto index_itr = properties.find("index"); index_itr != properties.end())
						{
							auto index = index_itr->second;
							auto texture = model.textures[size_t(index)];
							auto texture_source = texture.source;

							auto image_data = model.images[texture_source];

							auto emissive_map = device.make_texture(
								image_data.image.data(),
								glm::ivec2{ image_data.width, image_data.height },
								stb_to_moka(image_data.component),
								{},
								true,
								false);

							mat_builder.add_texture(material_property::emissive_map, emissive_map);
						}
					}

					//The alpha cutoff value of the material.	
					auto alpha_cutoff = 0.5f;

					if (auto alpha_cutoff_itr = material.additionalValues.find("alphaCutoff"); alpha_cutoff_itr != material.additionalValues.end())
					{
						auto alpha_cutoff_name = alpha_cutoff_itr->first;
						auto alpha_cutoff_value = alpha_cutoff_itr->second;

						alpha_cutoff = static_cast<float>(alpha_cutoff_value.number_value);
					}

					alpha_mode alpha = alpha_mode::opaque;

					if (auto alpha_mode_itr = material.additionalValues.find("alphaMode"); alpha_mode_itr != material.additionalValues.end())
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
						}
						else if (alpha_str == "BLEND")
						{
							alpha = alpha_mode::blend;
						}
					}

					mat_builder.set_alpha_mode(alpha);
				}
			}

			index_buffer.clear();
			vertex_buffer.clear();

			primitives.emplace_back(vertex_handle, vertices_count, index_handle, type, indices_count, 0, mat_builder.build());
		}

		return { std::move(primitives), std::move(trans) };
	}

	model load_model(const tinygltf::Model& model, graphics_device& device, const std::filesystem::path& root_path, std::map<std::string, program>& shaders)
	{
		std::vector<mesh> meshes;

		for (size_t i = 0; i < model.nodes.size(); i++)
		{
			auto mesh_id = model.nodes[i].mesh;

			if (mesh_id == -1) continue;

			auto model_translation = model.nodes[i].translation;
			auto model_rotation = model.nodes[i].rotation;
			auto model_scale = model.nodes[i].scale;

			transform trans;
			if (!model_translation.empty())
			{
				std::vector<float> translation(model_translation.begin(), model_translation.end());
				trans.set_position({ translation[0], translation[1], translation[2] });
			}
			if (!model_scale.empty())
			{
				std::vector<float> scale(model_scale.begin(), model_scale.end());
				trans.set_scale({ scale[0], scale[1], scale[2] });
			}
			if (!model_rotation.empty())
			{
				std::vector<float> rotation(model_rotation.begin(), model_rotation.end());
				trans.set_rotation({ rotation[3], rotation[0], rotation[1], rotation[2] });
			}

			meshes.emplace_back(load_mesh(model, model.meshes[mesh_id], device, trans, root_path, shaders));
		}

		return { std::move(meshes) };
	}

	model asset_importer<model>::load(const std::filesystem::path& path)
	{
		std::ifstream i(root_directory_ / path);
		nlohmann::json j;
		i >> j;

		auto model_path = root_directory_ / j["model"].get<std::string>();
		auto material_path = root_directory_ / j["material"].get<std::string>();

		tinygltf::Model model;
		tinygltf::TinyGLTF gltf_ctx;

		std::string err;
		std::string warn;

		auto ext = model_path.extension();

		bool ret = false;

		if (ext == ".glb")
		{
			std::cout << "Reading binary glTF" << std::endl;
			// assume binary glTF.
			ret = gltf_ctx.LoadBinaryFromFile(&model, &err, &warn, model_path.string());
		}
		else if (ext == ".gltf")
		{
			std::cout << "Reading ASCII glTF" << std::endl;
			// assume ascii glTF.
			ret = gltf_ctx.LoadASCIIFromFile(&model, &err, &warn, model_path.string());
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

		return load_model(model, device_, root_directory_, shaders_);
	}
}