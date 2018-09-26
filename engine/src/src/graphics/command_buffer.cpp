#include <graphics/command_buffer.hpp>
#include <graphics/camera.hpp>
#include <graphics/graphics_device.hpp>

namespace moka
{
	command_buffer::command_buffer(camera& cam, graphics_device& graphics)
		: cam_(cam)
		, device_(graphics)
		, view_uniform_(device_.create_uniform("view", uniform_type::mat4))
		, projection_uniform_(device_.create_uniform("projection", uniform_type::mat4))
		, view_pos_(device_.create_uniform("viewPos", uniform_type::vec3))
	{}

	draw_call_builder command_buffer::begin()
	{
		draw_call_builder builder{ device_ };
		builder.set_uniform(view_uniform_, cam_.get_view());
		builder.set_uniform(projection_uniform_, cam_.get_projection());
		builder.set_uniform(view_pos_, cam_.get_transform().get_position());
		return builder;
	}

	void command_buffer::submit()
	{

	}
}