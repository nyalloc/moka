#pragma once

#include "sort_key.hpp"
#include "draw_call.hpp"
#include <graphics/material.hpp>

namespace moka
{
	class graphics_device;

	class draw_call_builder
	{
		float depth_ = 0.0f; // z distance away from the camera! Needed for sorting transparent objects.
							 // 0.0f - 1.0f range calculated by subtracting the position of the object
		                     // from the position of the camera, then dividing it by the Z of the frustum?
							 // just a guess at the moment, still need to test :)

		draw_call call_;	 // vertex buffers, index buffers, materials... all the usual boring stuff!

		graphics_device& device_; // device that this builder will submit the draw call to

		draw_call build();
	public:
		~draw_call_builder();

		draw_call_builder(
			graphics_device& device);

		draw_call_builder& set_material(
			const material& material);

		draw_call_builder& set_depth(
			const float depth);

		draw_call_builder& set_face_culling(
			const face_culling culling);

		draw_call_builder& set_vertex_buffer(
			const vertex_buffer_handle vertex_buffer,
			const uint32_t vertex_start,
			const uint32_t vertex_count);

		draw_call_builder& set_index_buffer(
			const index_buffer_handle index_buffer,
			const uint32_t index_count);
	};
}
