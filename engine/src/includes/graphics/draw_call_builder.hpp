#pragma once
#include "sort_key.hpp"
#include "draw_call.hpp"

namespace moka
{
	class graphics_device;

	class draw_call_builder
	{
		draw_call call_;
		graphics_device& device_;
		bool is_first_uniform_set_;

		draw_call_builder& set_uniform_internal(
			const uniform_handle uniform,
			const void* data);

		draw_call build();
	public:
		draw_call_builder(
			graphics_device& device);

		draw_call_builder& set_texture_unit(
			const size_t texture_unit,
			const uniform_handle sampler_handle,
			const texture_handle texture);

		draw_call_builder& set_face_culling(
			const face_culling culling);

		draw_call_builder& set_vertex_buffer(
			const vertex_buffer_handle vertex_buffer,
			const uint32_t start_vertex = 0,
			const uint32_t vertex_count = std::numeric_limits<uint32_t>::max());

		draw_call_builder& set_index_buffer(
			const index_buffer_handle index_buffer,
			const uint32_t index_count = std::numeric_limits<uint32_t>::max());

		template<typename T>
		draw_call_builder& set_uniform(
			const uniform_handle uniform,
			const T& data);

		draw_call_builder& set_program(
			const program_handle program);

		void submit();
	};

	template<typename T>
	inline draw_call_builder& draw_call_builder::set_uniform(const uniform_handle uniform, const T & data)
	{
		return set_uniform_internal(uniform, &data);
	}

}
