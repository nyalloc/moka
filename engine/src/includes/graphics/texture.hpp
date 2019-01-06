#pragma once

#include <cstdint>

namespace moka
{
	enum class wrap_mode : uint8_t
	{
		clamp_to_edge,
		mirrored_repeat,
		repeat
	};

	struct texture_wrap_mode
	{
		wrap_mode x = wrap_mode::clamp_to_edge;
		wrap_mode y = wrap_mode::clamp_to_edge;
	};

	struct texture
	{
		uint16_t id;

		bool operator == (const texture& rhs)
		{
			return id == rhs.id;
		}

		bool operator != (const texture& rhs)
		{
			return id != rhs.id;
		}

		bool operator > (const texture& rhs)
		{
			return id > rhs.id;
		}

		bool operator < (const texture& rhs)
		{
			return id < rhs.id;
		}

		bool operator >= (const texture& rhs)
		{
			return id >= rhs.id;
		}

		bool operator <= (const texture& rhs)
		{
			return id <= rhs.id;
		}
	};
}