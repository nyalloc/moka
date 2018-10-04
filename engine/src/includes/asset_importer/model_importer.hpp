#pragma once

#include <string>
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include <graphics/model.hpp>
#include <asset_importer/asset_importer.hpp>
#include <graphics/material.hpp>

namespace moka
{	
	template<>
	class asset_importer<material>
	{
		graphics_device& device_;
		std::filesystem::path root_directory_;
	public:

		asset_importer(const std::filesystem::path& path, graphics_device& device);

		material load(const std::filesystem::path& path);
	};

	template<>
	class asset_importer<model>
	{
		graphics_device& device_;
		std::filesystem::path root_directory_;

		asset_importer<material> material_loader_;
	public:

		asset_importer(const std::filesystem::path& path, graphics_device& device);

		model load(const std::filesystem::path& path);
	};
}