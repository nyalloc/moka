#pragma once

#include <cstdint>
#include <limits>

namespace moka
{
	struct sort_key_generaor
	{
		uint64_t operator()(float depth, uint32_t material_id, bool alpha_mode)
		{
			return
			{
				static_cast<uint64_t>(depth * std::numeric_limits<uint16_t>::max()) |
				static_cast<uint64_t>(alpha_mode) << 16 |
				static_cast<uint64_t>(material_id) << 24
			};
		}
	};
}