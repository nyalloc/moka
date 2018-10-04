#pragma once

#include "sort_key.hpp"
#include "draw_call.hpp"
#include <graphics/material.hpp>

namespace moka
{
	class graphics_device;

	class draw_call_builder
	{
		draw_call call_;

		graphics_device& device_;

		draw_call build();
	public:
		draw_call_builder(
			graphics_device& device);

		draw_call_builder& set_material(
			const material& material);

		draw_call_builder& set_face_culling(
			const face_culling culling);

		draw_call_builder& set_vertex_buffer(
			const vertex_buffer_handle vertex_buffer,
			const uint32_t vertex_start,
			const uint32_t vertex_count);

		draw_call_builder& set_index_buffer(
			const index_buffer_handle index_buffer,
			const uint32_t index_count);

		void end();
	};
}
