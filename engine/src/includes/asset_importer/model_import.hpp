#pragma once

#include <glm/glm.hpp>
#include <filesystem>
#include <graphics/graphics_api.hpp>
#include <graphics/transform.hpp>
#include <graphics/model.hpp>

namespace moka
{	
	class graphics_device;

	model load_model(const std::filesystem::path& path, graphics_device& device);

	void unload(model& data);
}