#pragma once

#include <graphics/graphics_device.hpp>
#include <graphics/transform.hpp>
#include <graphics/material.hpp>

namespace moka
{
	class primitive
	{
		vertex_buffer_handle vertex_buffer_;
		uint32_t vertex_count_ = 0;

		index_buffer_handle index_buffer_;
		uint32_t index_count_ = 0;

		primitive_type type_ = primitive_type::triangles;

		material material_;
	public:
		static constexpr char* id = "primitive";

		material& get_material();

		primitive(vertex_buffer_handle vertex_buffer
			, uint32_t vertex_count
			, index_buffer_handle index_buffer
			, uint32_t index_count
			, const material& material);

		void draw(graphics_device& device);
	};

	class mesh
	{
		std::vector<primitive> primitives_;

		transform transform_;
	public:
		static constexpr char* id = "mesh";

		using iterator = std::vector<primitive>::iterator;
		using const_iterator = std::vector<primitive>::const_iterator;

		transform& get_transform();

		iterator begin();

		const_iterator begin() const;

		iterator end();

		const_iterator end() const;

		mesh(std::vector<primitive>&& primitives, transform&& transform);
	};

	struct model
	{
		std::vector<mesh> meshes_;
		transform transform_;
	public:
		static constexpr char* id = "model";

		using iterator = std::vector<mesh>::iterator;
		using const_iterator = std::vector<mesh>::const_iterator;

		iterator begin();

		const_iterator begin() const;

		iterator end();

		const_iterator end() const;

		model(std::vector<mesh>&& meshes, transform&& transform = {});

		std::vector<mesh>& get_meshes();
	};
}