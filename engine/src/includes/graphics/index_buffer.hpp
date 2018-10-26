#pragma once

#include "buffer_usage.hpp"
#include <cstdint>

namespace moka
{
	enum class index_type
	{
		int8,
		int16,
		int32,
		int64,
		uint8,
		uint16,
		uint32,
		uint64,
		float32
	};

	struct index_buffer
	{
		uint16_t id = std::numeric_limits<uint16_t>::max();

		bool operator == (const index_buffer& rhs)
		{
			return id == rhs.id;
		}

		bool operator != (const index_buffer& rhs)
		{
			return id != rhs.id;
		}

		bool operator > (const index_buffer& rhs)
		{
			return id > rhs.id;
		}

		bool operator < (const index_buffer& rhs)
		{
			return id < rhs.id;
		}

		bool operator >= (const index_buffer& rhs)
		{
			return id >= rhs.id;
		}

		bool operator <= (const index_buffer& rhs)
		{
			return id <= rhs.id;
		}
	};
}