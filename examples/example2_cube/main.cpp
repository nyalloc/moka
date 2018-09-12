
#include <SDL.h>
#include <maths/vector2.hpp>

#undef main

#include <graphics/graphics_api.hpp>
#include <graphics/graphics_device.hpp>
#include <graphics/draw_call.hpp>
#include <application/window.hpp>

namespace moka
{
	bool running = true;

	void poll_events()
	{
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
			{
				break;
			}
			case SDL_MOUSEMOTION:
			{
				break;
			}
			case SDL_KEYDOWN:
			{
				break;
			}
			default:;
			}
		}
	}

	void run(graphics_device& graphics)
	{
		poll_events();

		const vertex_buffer_handle vertex_buffer{};
		const index_buffer_handle index_buffer{};
		const program_handle program{};
		const texture_handle texture1{};
		const uniform_handle texture_sampler1{};
		const texture_handle texture2{};
		const uniform_handle texture_sampler2{};

		const uniform_handle uniform_buffer{};

		// submit a draw call for rendering
		graphics.draw()
			.set_vertex_buffer(vertex_buffer)
			.set_index_buffer(index_buffer)
			.set_face_culling(face_culling::back)
			.set_program(program)
			.set_texture_unit(0, texture_sampler1, texture1)
			.set_texture_unit(1, texture_sampler2, texture2)
			.submit();
	}
}

// this is all gnarly stuff, raw opengl and SDL will all be hidden in the real deal
int main()
{
	// create main window
	moka::window window("Moka");

	// context for graphics thread
	const auto worker_context = window.make_context();

	// context for main thread (needed for some reason...)
	auto main_context = window.make_context();

	moka::graphics_device graphics(window);

	while (moka::running)
	{
		run(graphics);
	}
}