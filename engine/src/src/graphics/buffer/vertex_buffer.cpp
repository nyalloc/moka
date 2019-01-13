#include <graphics/buffer/vertex_buffer.hpp>

namespace moka
{
	bool vertex_buffer::operator==(const vertex_buffer& rhs) const
	{
		return id == rhs.id;
	}

	bool vertex_buffer::operator!=(const vertex_buffer& rhs) const
	{
		return id != rhs.id;
	}

	bool vertex_buffer::operator>(const vertex_buffer& rhs) const
	{
		return id > rhs.id;
	}

	bool vertex_buffer::operator<(const vertex_buffer& rhs) const
	{
		return id < rhs.id;
	}

	bool vertex_buffer::operator>=(const vertex_buffer& rhs) const
	{
		return id >= rhs.id;
	}

	bool vertex_buffer::operator<=(const vertex_buffer& rhs) const
	{
		return id <= rhs.id;
	}
}
