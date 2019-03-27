#include <graphics/buffer/index_buffer_handle.hpp>

namespace moka
{
	bool index_buffer_handle::operator==(const index_buffer_handle& rhs) const
	{
		return id == rhs.id;
	}

	bool index_buffer_handle::operator!=(const index_buffer_handle& rhs) const
	{
		return id != rhs.id;
	}

	bool index_buffer_handle::operator>(const index_buffer_handle& rhs) const
	{
		return id > rhs.id;
	}

	bool index_buffer_handle::operator<(const index_buffer_handle& rhs) const
	{
		return id < rhs.id;
	}

	bool index_buffer_handle::operator>=(const index_buffer_handle& rhs) const
	{
		return id >= rhs.id;
	}

	bool index_buffer_handle::operator<=(const index_buffer_handle& rhs) const
	{
		return id <= rhs.id;
	}
}
