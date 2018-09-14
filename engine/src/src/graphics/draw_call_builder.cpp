#include <graphics/draw_call_builder.hpp>
#include <graphics/graphics_device.hpp>

namespace moka
{
	draw_call_builder::draw_call_builder(
		graphics_device& device)
	: device_(device)
	{
		call_.uniform_start = 0;
		call_.uniform_end = 0;
	}

	draw_call_builder& draw_call_builder::set_texture_unit(
		const size_t texture_unit, 
		const uniform_handle sampler_handle,
		const texture_handle texture)
	{
		return *this;
	}

	draw_call_builder& draw_call_builder::set_face_culling(
		const face_culling culling)
	{
		call_.state = call_.state | static_cast<uint64_t>(culling);
		return *this;
	}

	draw_call_builder& draw_call_builder::set_vertex_buffer(
		const vertex_buffer_handle vertex_buffer,
		const uint32_t vertex_start,
		const uint32_t vertex_count)
	{
		call_.vertex_buffer = vertex_buffer;
		call_.vertex_count = vertex_count;
		call_.vertex_start = vertex_start;
		return *this;
	}

	draw_call_builder& draw_call_builder::set_index_buffer(
		const index_buffer_handle index_buffer,
		const uint32_t index_count)
	{
		call_.index_buffer = index_buffer;
		call_.index_count = index_count;
		return *this;
	}

	draw_call_builder& draw_call_builder::set_uniform_internal(
		const uniform_handle uniform, 
		const void* data)
	{
		// update uniform buffer
		const auto& uniform_data = device_.set_uniform(uniform, data);

		if (!is_first_uniform_set_)
		{
			call_.uniform_start = uniform_data.buffer_start;
			is_first_uniform_set_ = true;
		}

		call_.uniform_end = uniform_data.buffer_end;

		return *this;
	}

	draw_call_builder& draw_call_builder::set_program(
		const program_handle program)
	{
		call_.program = program;
		return *this;
	}

	draw_call draw_call_builder::build()
	{
		call_.key = sort_key(call_.program);
		return call_;
	}

	void draw_call_builder::submit()
	{
		device_.submit(build());
	}
}
