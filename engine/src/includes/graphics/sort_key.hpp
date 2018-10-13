#pragma once

#include <cstdint>
#include <graphics/graphics_api.hpp>

namespace moka
{
	class sort_key
	{
		uint64_t key_ = 0;

	public:
		sort_key() = default;

		explicit sort_key(const program_handle program, const alpha_mode alpha_mode, const float depth)
			: key_
			(
				static_cast<uint64_t>(depth * std::numeric_limits<uint16_t>::max()) |
				static_cast<uint64_t>(alpha_mode) << 16 |
				static_cast<uint64_t>(program.id) << 24 |
				static_cast<uint64_t>(program.id) << 40 |
				static_cast<uint64_t>(program.id) << 48 |
				static_cast<uint64_t>(program.id) << 56
			)
		{}

		program_handle program_handle() const
		{
			return { static_cast<uint16_t>(key_) };
		}

		bool operator > (const sort_key& rhs) const
		{
			return key_ > rhs.key_;
		}

		bool operator < (const sort_key& rhs) const
		{
			return key_ < rhs.key_;
		}

		bool operator <= (const sort_key& rhs) const
		{
			return key_ <= rhs.key_;
		}

		bool operator >= (const sort_key& rhs) const
		{
			return key_ >= rhs.key_;
		}

		bool operator == (const sort_key& rhs) const
		{
			return key_ == rhs.key_;
		}
	};

}
