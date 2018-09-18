#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <asset_importer/texture_import.hpp>

namespace moka
{
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
}