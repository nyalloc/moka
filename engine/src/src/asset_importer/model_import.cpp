#pragma once

#include <asset_importer/model_import.hpp>
#include <asset_importer/texture_import.hpp>
#include <graphics/graphics_device.hpp>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <tiny_gltf.h>
#include <GL/glew.h>

#include <cstdio>
#include <fstream>
#include <iostream>

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

	static std::string print_mode(int mode)
	{
		if (mode == TINYGLTF_MODE_POINTS)
		{
			return "POINTS";
		}
		else if (mode == TINYGLTF_MODE_LINE)
		{
			return "LINE";
		}
		else if (mode == TINYGLTF_MODE_LINE_LOOP)
		{
			return "LINE_LOOP";
		}
		else if (mode == TINYGLTF_MODE_TRIANGLES)
		{
			return "TRIANGLES";
		}
		else if (mode == TINYGLTF_MODE_TRIANGLE_FAN)
		{
			return "TRIANGLE_FAN";
		}
		else if (mode == TINYGLTF_MODE_TRIANGLE_STRIP)
		{
			return "TRIANGLE_STRIP";
		}
		return "**UNKNOWN**";
	}

	static std::string print_target(int target)
	{
		if (target == 34962)
		{
			return "GL_ARRAY_BUFFER";
		}
		else if (target == 34963)
		{
			return "GL_ELEMENT_ARRAY_BUFFER";
		}
		else
		{
			return "**UNKNOWN**";
		}
	}

	static std::string print_type(int ty)
	{
		if (ty == TINYGLTF_TYPE_SCALAR)
		{
			return "SCALAR";
		}
		else if (ty == TINYGLTF_TYPE_VECTOR)
		{
			return "VECTOR";
		}
		else if (ty == TINYGLTF_TYPE_VEC2)
		{
			return "VEC2";
		}
		else if (ty == TINYGLTF_TYPE_VEC3)
		{
			return "VEC3";
		}
		else if (ty == TINYGLTF_TYPE_VEC4)
		{
			return "VEC4";
		}
		else if (ty == TINYGLTF_TYPE_MATRIX)
		{
			return "MATRIX";
		}
		else if (ty == TINYGLTF_TYPE_MAT2)
		{
			return "MAT2";
		}
		else if (ty == TINYGLTF_TYPE_MAT3)
		{
			return "MAT3";
		}
		else if (ty == TINYGLTF_TYPE_MAT4)
		{
			return "MAT4";
		}
		return "**UNKNOWN**";
	}

	static std::string print_component_type(int ty)
	{
		if (ty == TINYGLTF_COMPONENT_TYPE_BYTE)
		{
			return "BYTE";
		}
		else if (ty == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE)
		{
			return "UNSIGNED_BYTE";
		}
		else if (ty == TINYGLTF_COMPONENT_TYPE_SHORT)
		{
			return "SHORT";
		}
		else if (ty == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
		{
			return "UNSIGNED_SHORT";
		}
		else if (ty == TINYGLTF_COMPONENT_TYPE_INT)
		{
			return "INT";
		}
		else if (ty == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT)
		{
			return "UNSIGNED_INT";
		}
		else if (ty == TINYGLTF_COMPONENT_TYPE_FLOAT)
		{
			return "FLOAT";
		}
		else if (ty == TINYGLTF_COMPONENT_TYPE_DOUBLE)
		{
			return "DOUBLE";
		}

		return "**UNKNOWN**";
	}

#if 0
	static std::string PrintParameterType(int ty)
	{
		if (ty == TINYGLTF_PARAMETER_TYPE_BYTE)
		{
			return "BYTE";
		}
		else if (ty == TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE)
		{
			return "UNSIGNED_BYTE";
		}
		else if (ty == TINYGLTF_PARAMETER_TYPE_SHORT)
		{
			return "SHORT";
		}
		else if (ty == TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT)
		{
			return "UNSIGNED_SHORT";
		}
		else if (ty == TINYGLTF_PARAMETER_TYPE_INT)
		{
			return "INT";
		}
		else if (ty == TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT)
		{
			return "UNSIGNED_INT";
		}
		else if (ty == TINYGLTF_PARAMETER_TYPE_FLOAT)
		{
			return "FLOAT";
		}
		else if (ty == TINYGLTF_PARAMETER_TYPE_FLOAT_VEC2)
		{
			return "FLOAT_VEC2";
		}
		else if (ty == TINYGLTF_PARAMETER_TYPE_FLOAT_VEC3)
		{
			return "FLOAT_VEC3";
		}
		else if (ty == TINYGLTF_PARAMETER_TYPE_FLOAT_VEC4)
		{
			return "FLOAT_VEC4";
		}
		else if (ty == TINYGLTF_PARAMETER_TYPE_INT_VEC2)
		{
			return "INT_VEC2";
		}
		else if (ty == TINYGLTF_PARAMETER_TYPE_INT_VEC3)
		{
			return "INT_VEC3";
		}
		else if (ty == TINYGLTF_PARAMETER_TYPE_INT_VEC4)
		{
			return "INT_VEC4";
		}
		else if (ty == TINYGLTF_PARAMETER_TYPE_BOOL)
		{
			return "BOOL";
		}
		else if (ty == TINYGLTF_PARAMETER_TYPE_BOOL_VEC2)
		{
			return "BOOL_VEC2";
		}
		else if (ty == TINYGLTF_PARAMETER_TYPE_BOOL_VEC3)
		{
			return "BOOL_VEC3";
		}
		else if (ty == TINYGLTF_PARAMETER_TYPE_BOOL_VEC4)
		{
			return "BOOL_VEC4";
		}
		else if (ty == TINYGLTF_PARAMETER_TYPE_FLOAT_MAT2)
		{
			return "FLOAT_MAT2";
		}
		else if (ty == TINYGLTF_PARAMETER_TYPE_FLOAT_MAT3)
		{
			return "FLOAT_MAT3";
		}
		else if (ty == TINYGLTF_PARAMETER_TYPE_FLOAT_MAT4)
		{
			return "FLOAT_MAT4";
		}
		else if (ty == TINYGLTF_PARAMETER_TYPE_SAMPLER_2D)
		{
			return "SAMPLER_2D";
		}

		return "**UNKNOWN**";
	}
#endif

	static std::string print_wrap_mode(int mode)
	{
		if (mode == TINYGLTF_TEXTURE_WRAP_REPEAT)
		{
			return "REPEAT";
		}
		else if (mode == TINYGLTF_TEXTURE_WRAP_CLAMP_TO_EDGE)
		{
			return "CLAMP_TO_EDGE";
		}
		else if (mode == TINYGLTF_TEXTURE_WRAP_MIRRORED_REPEAT)
		{
			return "MIRRORED_REPEAT";
		}

		return "**UNKNOWN**";
	}

	static std::string print_filter_mode(int mode)
	{
		if (mode == TINYGLTF_TEXTURE_FILTER_NEAREST)
		{
			return "NEAREST";
		}
		else if (mode == TINYGLTF_TEXTURE_FILTER_LINEAR)
		{
			return "LINEAR";
		}
		else if (mode == TINYGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_NEAREST)
		{
			return "NEAREST_MIPMAP_NEAREST";
		}
		else if (mode == TINYGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_LINEAR)
		{
			return "NEAREST_MIPMAP_LINEAR";
		}
		else if (mode == TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_NEAREST)
		{
			return "LINEAR_MIPMAP_NEAREST";
		}
		else if (mode == TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_LINEAR)
		{
			return "LINEAR_MIPMAP_LINEAR";
		}
		return "**UNKNOWN**";
	}

	static std::string print_int_array(const std::vector<int> &arr)
	{
		if (arr.size() == 0)
		{
			return "";
		}

		std::stringstream ss;
		ss << "[ ";
		for (size_t i = 0; i < arr.size(); i++)
		{
			ss << arr[i] << ((i != arr.size() - 1) ? ", " : "");
		}
		ss << " ]";

		return ss.str();
	}

	static std::string print_float_array(const std::vector<double> &arr)
	{
		if (arr.size() == 0)
		{
			return "";
		}

		std::stringstream ss;
		ss << "[ ";
		for (size_t i = 0; i < arr.size(); i++)
		{
			ss << arr[i] << ((i != arr.size() - 1) ? ", " : "");
		}
		ss << " ]";

		return ss.str();
	}

	static std::string indent(const int indent)
	{
		std::string s;
		for (int i = 0; i < indent; i++)
		{
			s += "  ";
		}

		return s;
	}

	static std::string print_parameter_value(const tinygltf::Parameter &param)
	{
		if (!param.number_array.empty())
		{
			return print_float_array(param.number_array);
		}
		else
		{
			return param.string_value;
		}
	}

#if 0
	static std::string print_parameter_map(const tinygltf::ParameterMap &pmap)
	{
		std::stringstream ss;

		ss << pmap.size() << std::endl;
		for (auto &kv : pmap)
		{
			ss << kv.first << " : " << PrintParameterValue(kv.second) << std::endl;
		}

		return ss.str();
	}
#endif

	static std::string print_value(const std::string &name,
		const tinygltf::Value &value, const int indentation, const bool tag = true)
	{
		std::stringstream ss;

		if (value.IsObject())
		{
			const tinygltf::Value::Object &o = value.Get<tinygltf::Value::Object>();
			tinygltf::Value::Object::const_iterator it(o.begin());
			tinygltf::Value::Object::const_iterator itEnd(o.end());
			for (; it != itEnd; it++)
			{
				ss << print_value(it->first, it->second, indentation + 1) << std::endl;
			}
		}
		else if (value.IsString())
		{
			if (tag)
			{
				ss << indent(indentation) << name << " : " << value.Get<std::string>();
			}
			else
			{
				ss << " " << value.Get<std::string>() << " ";
			}
		}
		else if (value.IsBool())
		{
			if (tag)
			{
				ss << indent(indentation) << name << " : " << value.Get<bool>();
			}
			else
			{
				ss << " " << value.Get<bool>() << " ";
			}
		}
		else if (value.IsNumber())
		{
			if (tag)
			{
				ss << indent(indentation) << name << " : " << value.Get<double>();
			}
			else
			{
				ss << " " << value.Get<double>() << " ";
			}
		}
		else if (value.IsInt())
		{
			if (tag)
			{
				ss << indent(indentation) << name << " : " << value.Get<int>();
			}
			else
			{
				ss << " " << value.Get<int>() << " ";
			}
		}
		else if (value.IsArray())
		{
			ss << indent(indentation) << name << " [ ";
			for (size_t i = 0; i < value.Size(); i++)
			{
				ss << print_value("", value.Get(int(i)), indentation + 1, /* tag */false);
				if (i != (value.ArrayLen() - 1))
				{
					ss << ", ";
				}

			}
			ss << indent(indentation) << "] ";
		}

		// @todo { binary }

		return ss.str();
	}

	static void dump_node(const tinygltf::Node &node, int indentation)
	{
		std::cout << indent(indentation) << "name        : " << node.name << std::endl;
		std::cout << indent(indentation) << "camera      : " << node.camera << std::endl;
		std::cout << indent(indentation) << "mesh        : " << node.mesh << std::endl;

		if (!node.rotation.empty())
		{
			std::cout << indent(indentation) << "rotation    : " << print_float_array(node.rotation) << std::endl;
		}

		if (!node.scale.empty())
		{
			std::cout << indent(indentation) << "scale       : " << print_float_array(node.scale) << std::endl;
		}

		if (!node.translation.empty())
		{
			std::cout << indent(indentation) << "translation : " << print_float_array(node.translation) << std::endl;
		}

		if (!node.matrix.empty())
		{
			std::cout << indent(indentation) << "matrix      : " << print_float_array(node.matrix) << std::endl;
		}

		std::cout << indent(indentation) << "children    : " << print_int_array(node.children) << std::endl;
	}

	static void dump_string_int_map(const std::map<std::string, int> &m, int indentation)
	{
		std::map<std::string, int>::const_iterator it(m.begin());
		std::map<std::string, int>::const_iterator itEnd(m.end());
		for (; it != itEnd; it++)
		{
			std::cout << indent(indentation) << it->first << ": " << it->second << std::endl;
		}
	}

	static void dump_primitive(const tinygltf::Primitive &primitive, int indentation)
	{
		std::cout << indent(indentation) << "material : " << primitive.material << std::endl;
		std::cout << indent(indentation) << "indices : " << primitive.indices << std::endl;
		std::cout << indent(indentation) << "mode     : " << print_mode(primitive.mode) << "(" << primitive.mode << ")" << std::endl;
		std::cout << indent(indentation) << "attributes(items=" << primitive.attributes.size() << ")" << std::endl;
		dump_string_int_map(primitive.attributes, indentation + 1);

		std::cout << indent(indentation) << "extras :" << std::endl << print_value("extras", primitive.extras, indentation + 1) << std::endl;
	}

	static void dump_extensions(const tinygltf::ExtensionMap &extension, const int indentation)
	{
		// TODO(syoyo): pritty print Value
		for (auto &e : extension)
		{
			std::cout << indent(indentation) << e.first << std::endl;
			std::cout << print_value("extensions", e.second, indentation + 1) << std::endl;
		}
	}

	static void print_model_info(const tinygltf::Model &gltf_model)
	{
		std::cout << "=== Dump glTF ===" << std::endl;
		std::cout << "asset.copyright          : " << gltf_model.asset.copyright << std::endl;
		std::cout << "asset.generator          : " << gltf_model.asset.generator << std::endl;
		std::cout << "asset.version            : " << gltf_model.asset.version << std::endl;
		std::cout << "asset.minVersion         : " << gltf_model.asset.minVersion << std::endl;
		std::cout << std::endl;

		std::cout << "=== Dump scene ===" << std::endl;
		std::cout << "defaultScene: " << gltf_model.defaultScene << std::endl;

		{
			std::cout << "scenes(items=" << gltf_model.scenes.size() << ")" << std::endl;
			for (size_t i = 0; i < gltf_model.scenes.size(); i++)
			{
				std::cout << indent(1) << "scene[" << i << "] name  : " << gltf_model.scenes[i].name << std::endl;
				dump_extensions(gltf_model.scenes[i].extensions, 1);
			}
		}

		{
			std::cout << "meshes(item=" << gltf_model.meshes.size() << ")" << std::endl;
			for (size_t i = 0; i < gltf_model.meshes.size(); i++)
			{
				std::cout << indent(1) << "name     : " << gltf_model.meshes[i].name << std::endl;
				std::cout << indent(1) << "primitives(items=" << gltf_model.meshes[i].primitives.size() << "): " << std::endl;

				for (size_t k = 0; k < gltf_model.meshes[i].primitives.size(); k++)
				{
					dump_primitive(gltf_model.meshes[i].primitives[k], 2);
				}
			}
		}

		{
			for (size_t i = 0; i < gltf_model.accessors.size(); i++)
			{
				const tinygltf::Accessor &accessor = gltf_model.accessors[i];
				std::cout << indent(1) << "name         : " << accessor.name << std::endl;
				std::cout << indent(2) << "bufferView   : " << accessor.bufferView << std::endl;
				std::cout << indent(2) << "byteOffset   : " << accessor.byteOffset << std::endl;
				std::cout << indent(2) << "componentType: " << print_component_type(accessor.componentType) << "(" << accessor.componentType << ")" << std::endl;
				std::cout << indent(2) << "count        : " << accessor.count << std::endl;
				std::cout << indent(2) << "type         : " << print_type(accessor.type) << std::endl;
				if (!accessor.minValues.empty())
				{
					std::cout << indent(2) << "min          : [";
					for (size_t k = 0; k < accessor.minValues.size(); k++)
					{
						std::cout << accessor.minValues[k] << ((k != accessor.minValues.size() - 1) ? ", " : "");
					}
					std::cout << "]" << std::endl;
				}
				if (!accessor.maxValues.empty())
				{
					std::cout << indent(2) << "max          : [";
					for (size_t k = 0; k < accessor.maxValues.size(); k++)
					{
						std::cout << accessor.maxValues[k] << ((k != accessor.maxValues.size() - 1) ? ", " : "");
					}
					std::cout << "]" << std::endl;
				}
			}
		}

		{
			std::cout << "animations(items=" << gltf_model.animations.size() << ")" << std::endl;
			for (size_t i = 0; i < gltf_model.animations.size(); i++)
			{
				const tinygltf::Animation &animation = gltf_model.animations[i];
				std::cout << indent(1) << "name         : " << animation.name << std::endl;

				std::cout << indent(1) << "channels : [ " << std::endl;
				for (size_t j = 0; i < animation.channels.size(); i++)
				{
					std::cout << indent(2)
						<< "sampler     : " << animation.channels[j].sampler
						<< std::endl;
					std::cout << indent(2)
						<< "target.id   : " << animation.channels[j].target_node
						<< std::endl;
					std::cout << indent(2)
						<< "target.path : " << animation.channels[j].target_path
						<< std::endl;
					std::cout << ((i != (animation.channels.size() - 1)) ? "  , " : "");
				}
				std::cout << "  ]" << std::endl;

				std::cout << indent(1) << "samplers(items=" << animation.samplers.size()
					<< ")" << std::endl;

				for (size_t j = 0; j < animation.samplers.size(); j++)
				{
					const tinygltf::AnimationSampler &sampler = animation.samplers[j];
					std::cout << indent(2) << "input         : " << sampler.input
						<< std::endl;
					std::cout << indent(2) << "interpolation : " << sampler.interpolation
						<< std::endl;
					std::cout << indent(2) << "output        : " << sampler.output
						<< std::endl;
				}
			}
		}

		{
			std::cout << "bufferViews(items=" << gltf_model.bufferViews.size() << ")"
				<< std::endl;

			for (size_t i = 0; i < gltf_model.bufferViews.size(); i++)
			{
				const tinygltf::BufferView &bufferView = gltf_model.bufferViews[i];
				std::cout << indent(1) << "name         : " << bufferView.name
					<< std::endl;
				std::cout << indent(2) << "buffer       : " << bufferView.buffer
					<< std::endl;
				std::cout << indent(2) << "byteLength   : " << bufferView.byteLength
					<< std::endl;
				std::cout << indent(2) << "byteOffset   : " << bufferView.byteOffset
					<< std::endl;
				std::cout << indent(2) << "byteStride   : " << bufferView.byteStride
					<< std::endl;
				std::cout << indent(2)
					<< "target       : " << print_target(bufferView.target)
					<< std::endl;
			}
		}

		{
			std::cout << "buffers(items=" << gltf_model.buffers.size() << ")" << std::endl;
			for (size_t i = 0; i < gltf_model.buffers.size(); i++)
			{
				const tinygltf::Buffer &buffer = gltf_model.buffers[i];
				std::cout << indent(1) << "name         : " << buffer.name << std::endl;
				std::cout << indent(2) << "byteLength   : " << buffer.data.size()
					<< std::endl;
			}
		}

		{
			std::cout << "materials(items=" << gltf_model.materials.size() << ")"
				<< std::endl;
			for (size_t i = 0; i < gltf_model.materials.size(); i++)
			{
				const tinygltf::Material &material = gltf_model.materials[i];
				std::cout << indent(1) << "name         : " << material.name << std::endl;
				std::cout << indent(1) << "values(items=" << material.values.size() << ")"
					<< std::endl;

				tinygltf::ParameterMap::const_iterator p(material.values.begin());
				tinygltf::ParameterMap::const_iterator pEnd(material.values.end());
				for (; p != pEnd; p++)
				{
					std::cout << indent(2) << p->first << ": "
						<< print_parameter_value(p->second) << std::endl;
				}
			}
		}

		{
			std::cout << "nodes(items=" << gltf_model.nodes.size() << ")" << std::endl;
			for (size_t i = 0; i < gltf_model.nodes.size(); i++)
			{
				const tinygltf::Node &node = gltf_model.nodes[i];
				std::cout << indent(1) << "name         : " << node.name << std::endl;

				dump_node(node, 2);
			}
		}

		{
			std::cout << "images(items=" << gltf_model.images.size() << ")" << std::endl;
			for (size_t i = 0; i < gltf_model.images.size(); i++)
			{
				const tinygltf::Image &image = gltf_model.images[i];
				std::cout << indent(1) << "name         : " << image.name << std::endl;

				std::cout << indent(2) << "width     : " << image.width << std::endl;
				std::cout << indent(2) << "height    : " << image.height << std::endl;
				std::cout << indent(2) << "component : " << image.component << std::endl;
				dump_extensions(image.extensions, 1);
			}
		}

		{
			std::cout << "textures(items=" << gltf_model.textures.size() << ")" << std::endl;
			for (size_t i = 0; i < gltf_model.textures.size(); i++)
			{
				const tinygltf::Texture &texture = gltf_model.textures[i];
				std::cout << indent(1) << "sampler        : " << texture.sampler
					<< std::endl;
				std::cout << indent(1) << "source         : " << texture.source
					<< std::endl;
				dump_extensions(texture.extensions, 1);
			}
		}

		{
			std::cout << "samplers(items=" << gltf_model.samplers.size() << ")" << std::endl;

			for (size_t i = 0; i < gltf_model.samplers.size(); i++)
			{
				const tinygltf::Sampler &sampler = gltf_model.samplers[i];
				std::cout << indent(1) << "name (id)    : " << sampler.name << std::endl;
				std::cout << indent(2)
					<< "minFilter    : " << print_filter_mode(sampler.minFilter)
					<< std::endl;
				std::cout << indent(2)
					<< "magFilter    : " << print_filter_mode(sampler.magFilter)
					<< std::endl;
				std::cout << indent(2)
					<< "wrapS        : " << print_wrap_mode(sampler.wrapS)
					<< std::endl;
				std::cout << indent(2)
					<< "wrapT        : " << print_wrap_mode(sampler.wrapT)
					<< std::endl;
			}
		}

		{
			std::cout << "cameras(items=" << gltf_model.cameras.size() << ")" << std::endl;

			for (size_t i = 0; i < gltf_model.cameras.size(); i++)
			{
				const tinygltf::Camera &camera = gltf_model.cameras[i];
				std::cout << indent(1) << "name (id)    : " << camera.name << std::endl;
				std::cout << indent(1) << "type         : " << camera.type << std::endl;

				if (camera.type.compare("perspective") == 0) {
					std::cout << indent(2)
						<< "aspectRatio   : " << camera.perspective.aspectRatio
						<< std::endl;
					std::cout << indent(2) << "yfov          : " << camera.perspective.yfov
						<< std::endl;
					std::cout << indent(2) << "zfar          : " << camera.perspective.zfar
						<< std::endl;
					std::cout << indent(2) << "znear         : " << camera.perspective.znear
						<< std::endl;
				}
				else if (camera.type.compare("orthographic") == 0) {
					std::cout << indent(2) << "xmag          : " << camera.orthographic.xmag
						<< std::endl;
					std::cout << indent(2) << "ymag          : " << camera.orthographic.ymag
						<< std::endl;
					std::cout << indent(2) << "zfar          : " << camera.orthographic.zfar
						<< std::endl;
					std::cout << indent(2)
						<< "znear         : " << camera.orthographic.znear
						<< std::endl;
				}
			}
		}

		// toplevel extensions
		{
			std::cout << "extensions(items=" << gltf_model.extensions.size() << ")" << std::endl;
			dump_extensions(gltf_model.extensions, 1);
		}
	}

	struct vertex_data
	{
		vertex_layout layout;
		size_t start = 0;
		size_t length = 0;
		size_t count = 0;
	};

	model_mesh read_mesh(const tinygltf::Model& model, const tinygltf::Mesh& mesh, graphics_device& device, transform& trans)
	{
		auto buffer = &model.buffers[0].data.at(0);

 		model_mesh result;

		result.trans = trans;

		auto& primitive = mesh.primitives[0];
		
		// make index buffer (this one is easy)
		const auto& accessor = model.accessors[primitive.indices];
		const auto& view = model.bufferViews[accessor.bufferView];
		result.index_buffer = device.create_index_buffer(buffer + view.byteOffset, view.byteLength);
		result.index_count = accessor.count;

		// assumptions made with this approach:
		// attributes are always stored in a contiguous buffer
		// attributes are stored sequentially, not interleaved.
		// data is stored next to data of the same type, essentially (xyzxyzxyz... rgbrgb... ststst...)
		vertex_layout::builder layout_builder;

		size_t end_pos = 0;
		size_t start_pos = UINT32_MAX;

		std::vector<std::pair<std::string, int>> vertex_arrays;

		for (const auto&[name, pos] : primitive.attributes)
		{
			vertex_arrays.emplace_back(name, pos);
		}

		std::sort(vertex_arrays.begin(), vertex_arrays.end(), [](const std::pair<std::string, int>& lhs, const std::pair<std::string, int>& rhs)
		{
			return lhs.second < rhs.second;
		});

		// iterate over the vertex attributes and figure out what our vertex layout is
		for (size_t i = 0; i < vertex_arrays.size(); i++)
		{
			int pos = vertex_arrays[i].second;
			auto type = vertex_arrays[i].first;

			const auto& accessor = model.accessors[pos];

			int size = 1;
			if (accessor.type == TINYGLTF_TYPE_SCALAR)
			{
				size = 1;
			}
			else if (accessor.type == TINYGLTF_TYPE_VEC2)
			{
				size = 2;
			}
			else if (accessor.type == TINYGLTF_TYPE_VEC3)
			{
				size = 3;
			}
			else if (accessor.type == TINYGLTF_TYPE_VEC4)
			{
				size = 4;
			}
			else
			{
				assert(0);
			}

			auto buffer_view = model.bufferViews[accessor.bufferView];
			int byteStride = accessor.ByteStride(model.bufferViews[accessor.bufferView]);

			if (type == "POSITION")
			{
				layout_builder.add_attribute(0, attribute_type::float32, size, accessor.normalized, 0, end_pos);
			}
			else if (type == "NORMAL")
			{
				layout_builder.add_attribute(1, attribute_type::float32, size, accessor.normalized, 0, end_pos);
			}
			else if (type == "TEXCOORD_0")
			{
				layout_builder.add_attribute(2, attribute_type::float32, size, accessor.normalized, 0, end_pos);
			}
			else if (type == "TANGENT")
			{
				layout_builder.add_attribute(3, attribute_type::float32, size, accessor.normalized, 0, end_pos);
			}
			else
			{
				std::cout << "Unknown attribute type: " << type << std::endl;
			}

			end_pos += buffer_view.byteLength;

			if (start_pos > buffer_view.byteOffset)
			{
				start_pos = buffer_view.byteOffset;
			}
		}

		result.vertex_buffer = device.create_vertex_buffer(buffer + start_pos, end_pos, layout_builder.build());
		result.vertex_count = accessor.count;

		if (primitive.material != -1)
		{
			auto material = model.materials[primitive.material];

			if (!material.values.empty())
			{
				if (auto base_color_factor_itr = material.values.find("baseColorFactor"); base_color_factor_itr != material.values.end())
				{

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

						result.mat.albedo.handle = device.create_texture(data, false);
					}
				}

				if (auto metallic_factor_itr = material.values.find("metallicFactor"); metallic_factor_itr != material.values.end())
				{

				}

				if (auto roughness_factor_itr = material.values.find("roughnessFactor"); roughness_factor_itr != material.values.end())
				{

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

						result.mat.roughness.handle = device.create_texture(data, false);
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

						result.mat.normals.handle = device.create_texture(data, false);
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

						result.mat.occlusion.handle = device.create_texture(data, false);
					}
				}

				if (auto alpha_mode_itr = material.additionalValues.find("alphaMode"); alpha_mode_itr != material.additionalValues.end())
				{
					auto alpha_mode_name = alpha_mode_itr->first;
					auto alpha_mode_value = alpha_mode_itr->second;

					auto alpha_str = alpha_mode_value.string_value;

					if (alpha_str == "OPAQUE")
					{
						result.alpha = alpha_mode::opaque;
					}
					else if (alpha_str == "MASK")
					{
						result.alpha = alpha_mode::mask;
					}
					else if (alpha_str == "BLEND")
					{
						result.alpha = alpha_mode::blend;
					}
				}
			}
		}

		return result;
	}

	model read_model(const tinygltf::Model& model, graphics_device& device)
	{
		moka::model result;

		std::vector<model_mesh> meshes;

		for (size_t i = 0; i < model.nodes.size(); i++)
		{
			auto mesh_id = model.nodes[i].mesh;

			if (mesh_id == -1) continue;

			auto translation = model.nodes[i].translation;
			auto rotation = model.nodes[i].rotation;
			auto scale = model.nodes[i].scale;

			transform trans;
			if (!translation.empty())
			{
				trans.set_position({ (float)translation[0], (float)translation[1], (float)translation[2] });
			}
			if (!scale.empty())
			{
				trans.set_scale({ (float)scale[0], (float)scale[1], (float)scale[2] });
			}
			if (!rotation.empty())
			{
				trans.set_rotation({ (float)rotation[0], (float)rotation[1], (float)rotation[2], (float)rotation[3] });
			}

			meshes.emplace_back(read_mesh(model, model.meshes[mesh_id], device, trans));
		}

		result.meshes = meshes;

		return result;
	}

	model load_model(const std::filesystem::path& path, graphics_device& device)
	{
		tinygltf::Model model;
		tinygltf::TinyGLTF gltf_ctx;

		std::string err;
		std::string warn;

		auto ext = path.extension();

		bool ret = false;

		if (ext == ".glb")
		{
			std::cout << "Reading binary glTF" << std::endl;
			// assume binary glTF.
			ret = gltf_ctx.LoadBinaryFromFile(&model, &err, &warn, path.string());
		}
		else if (ext == ".gltf")
		{
			std::cout << "Reading ASCII glTF" << std::endl;
			// assume ascii glTF.
			ret = gltf_ctx.LoadASCIIFromFile(&model, &err, &warn, path.string());
		}
		else
		{
			printf("Attempting to load non-glTF asset.\n");
		}

		if (!warn.empty())
		{
			printf("Warn: %s\n", warn.c_str());
		}

		if (!err.empty())
		{
			printf("Err: %s\n", err.c_str());
		}

		if (!ret)
		{
			printf("Failed to parse glTF\n");
		}

		print_model_info(model);

		return read_model(model, device);
	}

	void unload(model& data)
	{

	}
}