
#include <application/application.hpp>
#include <graphics/graphics_device.hpp>

using namespace moka;

struct vertex
{
	vector3 position{ 0.0f };
	vector4 color{ 0.0f };

	constexpr vertex(
		const vector3& position,
		const colour& color) noexcept
		: position(position),
		color(color.to_vector4())
	{}
};

class triangle_application : public application
{
	vertex vertices_[3] =
	{
		vertex{ vector3{ 0.5f, -0.5f, 0.0f }, color::red()  },
		vertex{ vector3{-0.5f, -0.5f, 0.0f }, color::blue() },
		vertex{ vector3{ 0.0f,  0.5f, 0.0f }, color::lime() },
	};

	vertex_layout vertex_layout_ = vertex_layout::builder()
		.add_attribute(attribute::position, 3, attribute_type::float32)
		.add_attribute(attribute::color0,   4, attribute_type::float32)
		.build();

	const char* vertex_source_ =
		"    #version 330 core                               \n"
		"    layout (location = 0) in vec3 position;         \n"
		"    layout (location = 4) in vec4 color0;           \n"
		"    out vec3 out_color0;                            \n"
		"    void main()                                     \n"
		"    {                                               \n"
		"        gl_Position = vec4(position, 1.0);          \n"
		"        out_color0 = color0.xyz;                    \n"
		"    }                                               \0";

	const char* fragment_source_ =
		"    #version 330 core                               \n"
		"    out vec4 FragColor;                             \n"
		"    in vec3 out_color0;                             \n"
		"    void main()                                     \n"
		"    {                                               \n"
		"        FragColor = vec4(out_color0, 1.0f);         \n"
		"    }                                               \0";

	vertex_buffer_handle vertex_buffer_;
	program_handle program_;
	uniform_handle color_;

	vertex_buffer_handle create_vertex_buffer()
	{
		return graphics_.create_vertex_buffer(
			make_buffer(vertices_, sizeof vertices_),
			vertex_layout_);
	}

	program_handle create_program()
	{
		return graphics_.create_program(
			graphics_.create_shader(shader_type::vertex, vertex_source_),
			graphics_.create_shader(shader_type::fragment, fragment_source_));
	}

public:
	triangle_application(const int argc, char* argv[])
		: application(argc, argv)
		, vertex_buffer_(create_vertex_buffer())
		, program_(create_program())
		, color_(graphics_.create_uniform("color", uniform_type::vec4))
	{}

	~triangle_application()
	{
		graphics_.destroy(vertex_buffer_);
		graphics_.destroy(program_);
	}

	void draw(const game_time delta_time) override
	{
		vector4 color{ 0.5f, 0.5f, 0.5f, 1.0f };

		graphics_.draw()
			.set_vertex_buffer(vertex_buffer_, 0, 3)
			.set_program(program_)
			.set_uniform(color_, color)
			.submit();

		graphics_.frame();
	}

	void update(const game_time delta_time) override
	{
	}
};

int main(const int argc, char* argv[])
{
	return triangle_application{ argc, argv }.run();
}