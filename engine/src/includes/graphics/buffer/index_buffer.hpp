#pragma once

#include <cstdint>
#include <limits>

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

	struct index_buffer_handle final
	{
		uint16_t id = std::numeric_limits<uint16_t>::max();

		bool operator ==(const index_buffer_handle& rhs) const;

		bool operator !=(const index_buffer_handle& rhs) const;

		bool operator >(const index_buffer_handle& rhs) const;

		bool operator <(const index_buffer_handle& rhs) const;

		bool operator >=(const index_buffer_handle& rhs) const;

		bool operator <=(const index_buffer_handle& rhs) const;
	};
}
