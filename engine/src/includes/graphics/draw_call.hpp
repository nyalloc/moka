#pragma once

#include <graphics/sort_key.hpp>
#include <graphics/graphics_api.hpp>
#include <asset_importer/model_importer.hpp>

#undef max
#undef min

namespace moka
{
	// instead of defining start / end regions for the static uniform buffer
	// have a vector of indexes that point towards relevant elements in a dynamic unfiform buffer
	// draw calls will be able to share uniforms in this buffer and they won't just copy data into it without care

	struct draw_call
	{
		vertex_buffer_handle vertex_buffer = { std::numeric_limits<handle_id>::max() };
		uint32_t vertex_start = 0;
		uint32_t vertex_count = 0;

		index_buffer_handle index_buffer = { std::numeric_limits<handle_id>::max() };
		uint32_t index_count = 0;

		const material* material = nullptr;

		uint64_t state = 0; 

		sort_key key;
	};
}
