#include <graphics/draw_call_builder.hpp>
#include <graphics/graphics_device.hpp>
#include <graphics/draw_call.hpp>

namespace moka
{
	draw_call_builder::draw_call_builder(
		graphics_device& device)
	: device_(device)
	{}

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

	draw_call_builder& draw_call_builder::set_material(
		const material& effect)
	{
		call_.material = &effect;
		return *this;
	}

	draw_call draw_call_builder::build()
	{
		call_.key = sort_key(call_.material->get_program());
		return call_;
	}

	void draw_call_builder::end()
	{
		device_.submit(build());
	}
}
