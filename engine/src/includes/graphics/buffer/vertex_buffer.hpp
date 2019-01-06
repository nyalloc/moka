#pragma once

#include "buffer_usage.hpp"
#include "vertex_layout.hpp"
#include <cstdint>

namespace moka
{
	struct vertex_buffer
	{
		uint16_t id = std::numeric_limits<uint16_t>::max();

		bool operator == (const vertex_buffer& rhs)
		{
			return id == rhs.id;
		}

		bool operator != (const vertex_buffer& rhs)
		{
			return id != rhs.id;
		}

		bool operator > (const vertex_buffer& rhs)
		{
			return id > rhs.id;
		}

		bool operator < (const vertex_buffer& rhs)
		{
			return id < rhs.id;
		}

		bool operator >= (const vertex_buffer& rhs)
		{
			return id >= rhs.id;
		}

		bool operator <= (const vertex_buffer& rhs)
		{
			return id <= rhs.id;
		}
	};
}