#pragma once

#include <graphics/draw_call_builder.hpp>
#include <graphics/uniform_buffer.hpp>

namespace moka
{
	class camera;
	class graphics_device;

	class command_buffer
	{
		constexpr static size_t max_draw_calls = 2048;

		camera& cam_;
		graphics_device& device_;

		uniform_buffer buffer_;
		size_t draw_call_buffer_pos_ = 0;
		std::array<draw_call, max_draw_calls> draw_call_buffer_;

		uniform_handle view_uniform_;
		uniform_handle projection_uniform_;
		uniform_handle view_pos_;
	public:
		command_buffer(camera& cam, graphics_device& graphics);
		draw_call_builder begin();

		void submit();
	};
}