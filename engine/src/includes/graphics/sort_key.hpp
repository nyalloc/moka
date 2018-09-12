#pragma once

#include <cstdint>
#include <graphics/graphics_api.hpp>

namespace moka
{
	class sort_key
	{
		uint64_t _key = 0;

	public:
		sort_key() = default;

		explicit sort_key(const program_handle program)
			: _key
			(
				static_cast<uint64_t>(program.id)
			)
		{}

		program_handle program_handle() const
		{
			return { static_cast<uint16_t>(_key) };
		}

		bool operator > (const sort_key& rhs) const
		{
			return _key > rhs._key;
		}

		bool operator < (const sort_key& rhs) const
		{
			return _key < rhs._key;
		}

		bool operator <= (const sort_key& rhs) const
		{
			return _key <= rhs._key;
		}

		bool operator >= (const sort_key& rhs) const
		{
			return _key >= rhs._key;
		}

		bool operator == (const sort_key& rhs) const
		{
			return _key == rhs._key;
		}
	};

}
