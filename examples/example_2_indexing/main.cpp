
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

class indexing_application : public application
{
	vertex vertices_a_[4] =
	{
		vertex{ vector3{  0.5f,  0.5f,  0.0f  }, color::red()    },
		vertex{ vector3{  0.5f, -0.5f,  0.0f  }, color::blue()   },
		vertex{ vector3{ -0.5f, -0.5f,  0.0f  }, color::lime()   },
		vertex{ vector3{ -0.5f,  0.5f,  0.0f  }, color::yellow() }
	};

	uint32_t indices_a_[6] =
	{
		0, 1, 3,
		1, 2, 3
	};

	vertex_layout vertex_layout_ = vertex_layout::builder()
		.add_attribute(attribute::position, 3, attribute_type::float32)
		.add_attribute(attribute::color0,   4, attribute_type::float32)
		.build();

	vertex vertices_b_[3] =
	{
		vertex{ vector3{ -0.25f, -0.25f, 1.0f }, color::red()  },
		vertex{ vector3{  0.25f, -0.25f, 1.0f }, color::blue() },
		vertex{ vector3{  0.0f,   0.25f, 1.0f }, color::lime() },
	};

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

	vertex_buffer_handle vertex_buffer_a_;
	vertex_buffer_handle vertex_buffer_b_;
	index_buffer_handle index_buffer_a_;
	shader_handle vertex_shader_;
	shader_handle fragment_shader_;
	program_handle program_;

public:

	indexing_application(const int argc, char* argv[])
		: application(argc, argv)
		, vertex_buffer_a_(graphics_.create_vertex_buffer(vertices_a_, sizeof vertices_a_, vertex_layout_))
		, vertex_buffer_b_(graphics_.create_vertex_buffer(vertices_b_, sizeof vertices_b_, vertex_layout_))
		, index_buffer_a_(graphics_.create_index_buffer(indices_a_, sizeof indices_a_))
		, vertex_shader_(graphics_.create_shader(shader_type::vertex, vertex_source_))
		, fragment_shader_(graphics_.create_shader(shader_type::fragment, fragment_source_))
		, program_(graphics_.create_program(vertex_shader_, fragment_shader_))
	{}

	~indexing_application()
	{
		graphics_.destroy(index_buffer_a_);
		graphics_.destroy(vertex_buffer_a_);
		graphics_.destroy(vertex_buffer_b_);
		graphics_.destroy(program_);
	}

	void draw(const game_time delta_time) override
	{
		graphics_.begin()
			.set_vertex_buffer(vertex_buffer_a_, 0, 4)
			.set_index_buffer(index_buffer_a_, 6)
			.set_program(program_)
			.end();

		graphics_.begin()
			.set_vertex_buffer(vertex_buffer_b_, 0, 3)
			.set_program(program_)
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
	return indexing_application{ argc, argv }.run();
}