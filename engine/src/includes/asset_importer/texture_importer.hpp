#pragma once

#include <maths/vector2.hpp>
#include <filesystem>

namespace moka
{
	enum class texture_components : uint8_t
	{
		grey,
		grey_alpha,
		rgb,
		rgb_alpha,
		auto_detect
	};

	struct texture_data
	{
		void* data;
		vector2_uint resolution;
		texture_components components;

		texture_data(
			void* data, 
			const vector2_uint& resolution, 
			const texture_components& components);

		~texture_data();
	};

	texture_data load(
		const std::filesystem::path& path,
		texture_components components = texture_components::auto_detect);

	void unload(texture_data& data);
}