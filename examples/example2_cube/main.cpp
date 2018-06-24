
#include <application/application.hpp>
#include <graphics_api.hpp>

using namespace neon;

class triangle_application : public application
{
	// triangle vertex positions
	constexpr static std::array<float, 9> vertices = 
	{
		-0.5f, -0.5f,  0.0f,
		 0.5f, -0.5f,  0.0f,
		 0.0f,  0.5f,  0.0f
	};

	// glsl vertex shader
	constexpr static char* vertex_shader_source = 
	R"(
		#version 330 core
		layout (location = 0) in vec3 aPos;
		void main()
		{
		   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
		}
	)";

	// glsl fragment shader
	constexpr static char* fragment_shader_source =
	R"(
		#version 330 core
		out vec4 FragColor;
		void main()
		{
		   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
		}
	)";

	vertex_buffer_handle vertex_buffer_handle_;
	shader_handle vertex_shader_handle_;
	shader_handle fragment_shader_handle_;
	program_handle program_handle_;
public:
	triangle_application(const int argc, char* argv[]) :
		application{ argc, argv }
		, vertex_buffer_handle_{ graphics_.create_vertex() }
		, vertex_shader_handle_{ graphics_.create_shader(vertex_shader_source) }
		, fragment_shader_handle_{ graphics_.create_shader(fragment_shader_source) }
		, program_handle_{ graphics_.create_program(vertex_shader_handle_, fragment_shader_handle_) }
	{}

	~triangle_application()
	{
		graphics_.destroy(vertex_buffer_handle_);
		graphics_.destroy(vertex_shader_handle_);
		graphics_.destroy(fragment_shader_handle_);
		graphics_.destroy(program_handle_);
	}

	void draw(const game_time delta_time) override
	{
		graphics_.clear_colour(colour{ 255, 0, 0, 255});
		application::draw(delta_time);
	}

	void update(const game_time delta_time) override
	{
		application::update(delta_time);
	}
};

int main(const int argc, char* argv[])
{
	return triangle_application{ argc, argv }.run();
}