#pragma once

#include <cstdint>

namespace moka
{
	enum class shader_type
	{
		vertex,
		fragment,
		compute
	};

	struct shader
	{
		uint16_t id;

		bool operator == (const shader& rhs)
		{
			return id == rhs.id;
		}

		bool operator != (const shader& rhs)
		{
			return id != rhs.id;
		}

		bool operator > (const shader& rhs)
		{
			return id > rhs.id;
		}

		bool operator < (const shader& rhs)
		{
			return id < rhs.id;
		}

		bool operator >= (const shader& rhs)
		{
			return id >= rhs.id;
		}

		bool operator <= (const shader& rhs)
		{
			return id <= rhs.id;
		}
	};
}