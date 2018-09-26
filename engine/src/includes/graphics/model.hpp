#pragma once

#include <graphics/graphics_device.hpp>
#include <graphics/effect.hpp>

namespace moka
{
	enum class alpha_mode : uint8_t
	{
		opaque, // The rendered output is fully opaque and any alpha value is ignored.
		mask, // The rendered output is either fully opaque or fully transparent depending on the alpha value and the specified alpha cutoff value. This mode is used to simulate geometry such as tree leaves or wire fences.
		blend // The rendered output is combined with the background using the normal painting operation(i.e.the Porter and Duff over operator). This mode is used to simulate geometry such as guaze cloth or animal fur.
	};

	struct texture_2d
	{
		texture_handle handle;
	};

	struct material
	{
		texture_2d albedo;
		texture_2d roughness;
		texture_2d occlusion;
		texture_2d normals;
	};

	struct model_mesh
	{
		transform trans;

		//effect effect;

		vertex_buffer_handle vertex_buffer = { std::numeric_limits<handle_id>::max() };
		uint32_t vertex_count = 0;

		index_buffer_handle index_buffer = { std::numeric_limits<handle_id>::max() };
		uint32_t index_count = 0;

		alpha_mode alpha;

		material mat;
	};

	struct model
	{
		std::vector<model_mesh> meshes;
	};
}