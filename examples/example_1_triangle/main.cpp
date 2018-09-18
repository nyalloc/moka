
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

class texture_application : public application
{
	vertex vertices_[3] =
	{
		vertex{ vector3{ -0.5f, -0.5f,  0.0f  }, color::red()  },
		vertex{ vector3{  0.5f, -0.5f,  0.0f  }, color::blue() },
		vertex{ vector3{  0.0f,  0.5f,  0.0f  }, color::lime() },
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
		"    uniform vec4 color;                             \n"
		"    void main()                                     \n"
		"    {                                               \n"
		"        FragColor = vec4(out_color0, 1.0f) * color; \n"
		"    }                                               \0";

	vertex_buffer_handle vertex_buffer_;
	index_buffer_handle index_buffer_;
	shader_handle vertex_shader_;
	shader_handle fragment_shader_;
	program_handle program_;

	uniform_handle colour_uniform_;
	vector4 white_;
public:

	texture_application(const int argc, char* argv[])
		: application(argc, argv)
		, vertex_buffer_(graphics_.create_vertex_buffer(vertices_, sizeof vertices_, vertex_layout_))
		, vertex_shader_(graphics_.create_shader(shader_type::vertex, vertex_source_))
		, fragment_shader_(graphics_.create_shader(shader_type::fragment, fragment_source_))
		, program_(graphics_.create_program(vertex_shader_, fragment_shader_))
		, colour_uniform_(graphics_.create_uniform("color", uniform_type::vec4, 1))
		, white_(color::white().to_vector4())
	{}

	~texture_application()
	{
		graphics_.destroy(vertex_buffer_);
		graphics_.destroy(program_);
		timer_.stop();
	}

	void draw(const game_time delta_time) override
	{
		float current_time = timer_.elapsed() / 1000.0f;
		float colour_val = (sin(current_time) / 2.0f) + 0.5f;

		graphics_.begin()
			.set_vertex_buffer(vertex_buffer_, 0, 3)
			.set_program(program_)
			.set_uniform(colour_uniform_, white_ * colour_val)
			.end();

		graphics_.frame();
	}

	void update(const game_time delta_time) override
	{
	}

	std::filesystem::path data_path() override
	{
		// cmake-defined macro points to example project asset folder relative to source.
		// A real application could point this wherever it wanted.
		std::filesystem::path result{ ASSET_PATH };
		return result.lexically_normal();
	}
};

int main(const int argc, char* argv[])
{
	return texture_application{ argc, argv }.run();
}