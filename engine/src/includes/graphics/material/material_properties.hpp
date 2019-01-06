#pragma once

#include <cstdint>

namespace moka
{
	enum class material_property : uint8_t
	{
		diffuse_map              = 0x20
		, normal_map             = 0x10
		, emissive_map           = 0x8
		, metallic_roughness_map = 0x4
		, ao_map		         = 0x1
	};
}