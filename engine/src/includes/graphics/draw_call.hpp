#pragma once

#include <graphics/sort_key.hpp>

#undef max
#undef min

namespace moka
{
	struct draw_call
	{
		program_handle program = { std::numeric_limits<handle_id>::max() };

		vertex_buffer_handle vertex_buffer = { std::numeric_limits<handle_id>::max() };
		uint32_t start_vertex = 0;
		uint32_t vertex_count = 0;

		index_buffer_handle index_buffer = { std::numeric_limits<handle_id>::max() };
		uint32_t index_count = 0;

		uint32_t uniform_start = std::numeric_limits<uint32_t>::max();
		uint32_t uniform_end   = std::numeric_limits<uint32_t>::max();   

		uint64_t state = 0; 
		sort_key key;
	};
}
