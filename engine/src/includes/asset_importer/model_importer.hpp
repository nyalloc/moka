#pragma once

#include <string>
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include <graphics/model.hpp>
#include <asset_importer/asset_importer.hpp>
#include <graphics/material/material.hpp>
#include <map>

namespace moka
{	
	template<>
	class asset_importer<model>
	{
		graphics_device& device_;
		std::filesystem::path root_directory_;
		std::map<std::string, program> shaders_;
	public:

		asset_importer(const std::filesystem::path& path, graphics_device& device);

		model load(const std::filesystem::path& path);
	};
}