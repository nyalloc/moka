#pragma once

#include <cstdint>
#include <limits>

namespace moka
{
	struct vertex_buffer final
	{
		uint16_t id = std::numeric_limits<uint16_t>::max();

		bool operator ==(const vertex_buffer& rhs) const;

		bool operator !=(const vertex_buffer& rhs) const;

		bool operator >(const vertex_buffer& rhs) const;

		bool operator <(const vertex_buffer& rhs) const;

		bool operator >=(const vertex_buffer& rhs) const;

		bool operator <=(const vertex_buffer& rhs) const;
	};
}
