#pragma once

#include <graphics/graphics_device.hpp>

namespace moka
{
	enum class wrap_mode : uint8_t
	{
		clamp_to_edge,
		mirrored_repeat,
		repeat
	};

	struct texture_wrap_mode
	{
		wrap_mode x = wrap_mode::clamp_to_edge;
		wrap_mode y = wrap_mode::clamp_to_edge;
	};

	struct texture_2d
	{
		texture_handle handle;
		texture_wrap_mode wrap_mode;
		bool has_mipmaps;
		glm::ivec2 size;
	};

	struct texture_unit
	{
		size_t unit = 0;
		texture_2d texture;
	};
}