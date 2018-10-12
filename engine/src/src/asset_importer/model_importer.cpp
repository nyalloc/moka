#pragma once

#include <asset_importer/model_importer.hpp>
#include <asset_importer/texture_importer.hpp>
#include <graphics/graphics_device.hpp>
#include <graphics/material_builder.hpp>
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
	}

	texture_data load(const std::filesystem::path& path, const texture_components components)
	{
		if (!std::filesystem::exists(path))
		{
			throw std::runtime_error("Texture specified at " + path.string() + " cannot be found.");
		}

		stbi_set_flip_vertically_on_load(true);

		int x, y, n;
		auto path_string = path.string();
		auto path_c_string = path_string.c_str();
		unsigned char *data = stbi_load(path_c_string, &x, &y, &n, moka_to_stb(components));
		return texture_data{ data, vector2_uint{ x, y }, stb_to_moka(n) };
	}

	void unload(texture_data& data)
	{
		stbi_image_free(data.data);
	}

	texture_data::texture_data(
		void* data
		, const vector2_uint& resolution
		, const texture_components& components)
		: data(data)
		, resolution(resolution)
		, components(components)
	{}

	texture_data::~texture_data() = default;

	asset_importer<model>::asset_importer(const std::filesystem::path& path, graphics_device& device)
		: root_directory_(path), device_(device)
	{}

	mesh load_mesh(const tinygltf::Model& model, const tinygltf::Mesh& mesh, graphics_device& device, transform& trans, const std::filesystem::path& root_path, std::map<std::string, program_handle>& shaders)
	{
		material_builder mat_builder(device, shaders);

		mat_builder.set_vertex_shader(root_path / "Materials" / "Shaders" / "pbr.vert");
		mat_builder.set_fragment_shader(root_path / "Materials" / "Shaders" / "pbr.frag");

		auto transform = trans;

		std::vector<moka::primitive> primitives;


		for (const auto& primitive : mesh.primitives)
		{
			std::vector<uint8_t> index_buffer;
			std::vector<uint8_t> vertex_buffer;

			vertex_buffer_handle vertex_handle;
			index_buffer_handle index_handle;

			const auto& indices_accessor = model.accessors[primitive.indices];
			const auto& indices_buffer_view = model.bufferViews[indices_accessor.bufferView];
			const auto& indices_buffer = model.buffers[indices_buffer_view.buffer].data;

			index_buffer.insert(
				index_buffer.end(),
				indices_buffer.begin() + indices_buffer_view.byteOffset,
				indices_buffer.begin() + indices_buffer_view.byteOffset + indices_buffer_view.byteLength);

			auto indices_count = indices_accessor.count;

			vertex_layout::builder layout_builder;

			size_t vertices_count = 0;

			auto get_size = [](int type) 
			{
				// based on the type, figure out the number of floats they consist of
				if (type == TINYGLTF_TYPE_SCALAR)
				{
					return 1;
				}
				else if (type == TINYGLTF_TYPE_VEC2)
				{
					return 2;
				}
				else if (type == TINYGLTF_TYPE_VEC3)
				{
					return 3;
				}
				else if (type == TINYGLTF_TYPE_VEC4)
				{
					return 4;
				}
				else
				{
					assert(0);
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

			vertex_handle = device.create_vertex_buffer(vertex_buffer.data(), vertex_buffer.size(), layout_builder.build());
			index_handle = device.create_index_buffer(index_buffer.data(), index_buffer.size());

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

							texture_data data
							{
								image_data.image.data(),
								moka::vector2_uint{ image_data.width, image_data.height },
								stb_to_moka(image_data.component)
							};

							auto albedo = device.create_texture(data, false);

							texture_2d diffuse_map;
							diffuse_map.handle = albedo;

							mat_builder.add_texture(material_property::diffuse_map, diffuse_map);
						}
					}

					if (auto metallic_factor_itr = material.values.find("metallicFactor"); metallic_factor_itr != material.values.end())
					{
						auto data = metallic_factor_itr->second.number_value;
						float metallic_factor(data);
						mat_builder.add_uniform("material.metallic_factor", metallic_factor);
					}

					if (auto roughness_factor_itr = material.values.find("roughnessFactor"); roughness_factor_itr != material.values.end())
					{
						auto data = roughness_factor_itr->second.number_value;
						float roughness_factor(data);
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

							texture_data data
							{
								image_data.image.data(),
								moka::vector2_uint{ image_data.width, image_data.height },
								stb_to_moka(image_data.component)
							};

							auto metallic_roughness = device.create_texture(data, false);

							texture_2d metallic_roughness_map;
							metallic_roughness_map.handle = metallic_roughness;

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

							texture_data data
							{
								image_data.image.data(),
								moka::vector2_uint{ image_data.width, image_data.height },
								stb_to_moka(image_data.component)
							};

							auto normals = device.create_texture(data, false);

							texture_2d normal_map;
							normal_map.handle = normals;

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

							texture_data data
							{
								image_data.image.data(),
								moka::vector2_uint{ image_data.width, image_data.height },
								stb_to_moka(image_data.component)
							};

							auto occlusion = device.create_texture(data, false);
							texture_2d occlusion_map;
							occlusion_map.handle = occlusion;

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

							texture_data data
							{
								image_data.image.data(),
								moka::vector2_uint{ image_data.width, image_data.height },
								stb_to_moka(image_data.component)
							};

							auto emissive = device.create_texture(data, false);
							texture_2d emissive_map;
							emissive_map.handle = emissive;

							mat_builder.add_texture(material_property::emissive_map, emissive_map);
						}
					}

					if (auto alpha_mode_itr = material.additionalValues.find("alphaMode"); alpha_mode_itr != material.additionalValues.end())
					{
						auto alpha_mode_name = alpha_mode_itr->first;
						auto alpha_mode_value = alpha_mode_itr->second;

						auto alpha_str = alpha_mode_value.string_value;

						alpha_mode alpha;

						if (alpha_str == "OPAQUE")
						{
							alpha = alpha_mode::opaque;
						}
						else if (alpha_str == "MASK")
						{
							alpha = alpha_mode::mask;
						}
						else if (alpha_str == "BLEND")
						{
							alpha = alpha_mode::blend;
						}
					}
				}
			}

			primitives.emplace_back(vertex_handle, vertices_count, index_handle, indices_count, mat_builder.build());
		}

		return { std::move(primitives), std::move(trans) };
	}

	model load_model(const tinygltf::Model& model, graphics_device& device, const std::filesystem::path& root_path, std::map<std::string, program_handle>& shaders)
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