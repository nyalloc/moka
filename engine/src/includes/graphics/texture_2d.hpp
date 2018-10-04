#pragma once

#include <graphics/graphics_device.hpp>

namespace moka
{
	enum class alpha_mode : uint8_t
	{
		opaque, // The rendered output is fully opaque and any alpha value is ignored.
		mask, // The rendered output is either fully opaque or fully transparent depending on the alpha value and the specified alpha cutoff value. This mode is used to simulate geometry such as tree leaves or wire fences.
		blend // The rendered output is combined with the background using the normal painting operation(i.e.the Porter and Duff over operator). This mode is used to simulate geometry such as guaze cloth or animal fur.
	};

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