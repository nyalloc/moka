#pragma once

#include <graphics/graphics_device.hpp>
#include <graphics/transform.hpp>
#include <graphics/material.hpp>

namespace moka
{
	class primitive
	{
		vertex_buffer vertex_buffer_;
		uint32_t vertex_count_ = 0;

		index_buffer index_buffer_;
		index_type index_type_;
		uint32_t index_count_ = 0;
		uint32_t index_buffer_offset_ = 0;

		primitive_type type_ = primitive_type::triangles;

		material material_;
	public:
		material& get_material();

		const material& get_material() const;

		primitive(vertex_buffer vertex_buffer
			, uint32_t vertex_count
			, index_buffer index_buffer
			, index_type index_type
			, uint32_t index_count
			, uint32_t index_buffer_offset
			, material&& material);

		void draw(command_list& list) const;
	};

	class mesh
	{
		std::vector<primitive> primitives_;

		transform transform_;
	public:
		using iterator = std::vector<primitive>::iterator;
		using const_iterator = std::vector<primitive>::const_iterator;

		transform& get_transform();

		iterator begin();

		const_iterator begin() const;

		iterator end();

		const_iterator end() const;

		mesh(std::vector<primitive>&& primitives, transform&& transform);
	};

	class model
	{
		std::vector<mesh> meshes_;
		transform transform_;
	public:

		using iterator = std::vector<mesh>::iterator;
		using const_iterator = std::vector<mesh>::const_iterator;

		iterator begin();

		const_iterator begin() const;

		iterator end();

		const_iterator end() const;

		model() = default;

		model(std::vector<mesh>&& meshes, transform&& transform = {});

		std::vector<mesh>& get_meshes();
	};
}