#include <graphics/buffer/index_buffer.hpp>

namespace moka
{
	bool index_buffer::operator==(const index_buffer& rhs) const
	{
		return id == rhs.id;
	}

	bool index_buffer::operator!=(const index_buffer& rhs) const
	{
		return id != rhs.id;
	}

	bool index_buffer::operator>(const index_buffer& rhs) const
	{
		return id > rhs.id;
	}

	bool index_buffer::operator<(const index_buffer& rhs) const
	{
		return id < rhs.id;
	}

	bool index_buffer::operator>=(const index_buffer& rhs) const
	{
		return id >= rhs.id;
	}

	bool index_buffer::operator<=(const index_buffer& rhs) const
	{
		return id <= rhs.id;
	}
}
