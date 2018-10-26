#pragma once

#include <cstdint>

namespace moka
{
	struct program
	{
		uint16_t id;

		bool operator == (const program& rhs)
		{
			return id == rhs.id;
		}

		bool operator != (const program& rhs)
		{
			return id != rhs.id;
		}

		bool operator > (const program& rhs)
		{
			return id > rhs.id;
		}

		bool operator < (const program& rhs)
		{
			return id < rhs.id;
		}

		bool operator >= (const program& rhs)
		{
			return id >= rhs.id;
		}

		bool operator <= (const program& rhs)
		{
			return id <= rhs.id;
		}
	};
}