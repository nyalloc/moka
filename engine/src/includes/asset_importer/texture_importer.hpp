#pragma once

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

	void load_texture(
		const std::filesystem::path& path,
		void* data,
		glm::ivec2& resolution,
		texture_components& components,
		texture_components components_requested = texture_components::auto_detect);

	void free_texture(void* data);
}