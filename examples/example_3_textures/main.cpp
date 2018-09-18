
#include <application/application.hpp>
#include <graphics/graphics_device.hpp>
#include <maths/vector2.hpp>
#include <maths/vector3.hpp>
#include <maths/vector4.hpp>
#include <asset_importer/texture_import.hpp>

using namespace moka;

struct vertex
{
	vector3 position{ 0.0f };
	vector4 color{ 0.0f };
	vector2 texture_coord{ 0.0f };

	constexpr vertex(
		const vector3& position,
		const colour& color,
		const vector2& texture_coord) noexcept
		: position(position)
		, color(color.to_vector4())
		, texture_coord(texture_coord)
	{}
};

class texture_application : public application
{
	vertex vertices_[4] =
	{
		vertex{ vector3{  0.5f,  0.5f,  0.0f  }, color::red(),    vector2{ 1.0f, 1.0f } },
		vertex{ vector3{  0.5f, -0.5f,  0.0f  }, color::blue(),   vector2{ 1.0f, 0.0f } },
		vertex{ vector3{ -0.5f, -0.5f,  0.0f  }, color::lime(),   vector2{ 0.0f, 0.0f } },
		vertex{ vector3{ -0.5f,  0.5f,  0.0f  }, color::yellow(), vector2{ 0.0f, 1.0f } }
	};

	uint32_t indices_[6] =
	{
		0, 1, 3,
		1, 2, 3
	};

	vertex_layout vertex_layout_ = vertex_layout::builder()
		.add_attribute(attribute::position,   3, attribute_type::float32)
		.add_attribute(attribute::color0,     4, attribute_type::float32)
		.add_attribute(attribute::tex_coord0, 2, attribute_type::float32)
		.build();

	const char* vertex_source_ =
		"    #version 330 core                                  \n"
		"    layout (location = 0)  in vec3 position;           \n"
		"    layout (location = 4)  in vec4 color0;             \n"
		"    layout (location = 10) in vec2 tex_coord0;         \n"
		"    out vec2 tex_coord;"
		"    void main()                                        \n"
		"    {                                                  \n"
		"        gl_Position = vec4(position, 1.0);             \n"
		"        tex_coord = tex_coord0;                        \n"
		"    }                                                  \0";

	const char* fragment_source_ =
		"    #version 330 core                                      \n"
		"    out vec4 FragColor;                                    \n"
		"    in vec2 tex_coord;                                     \n"
		"    uniform vec4 color;                                    \n"
		"    uniform sampler2D tile_texture;                        \n"
		"    uniform sampler2D test_texture;                        \n"
		"    void main()                                            \n"
		"    {                                                      \n"
		"        vec4 tex1 = texture(tile_texture, tex_coord).rgba;	\n"
		"        vec4 tex2 = texture(test_texture, tex_coord).rgba;	\n"
		"        tex2 *= color;                                     \n"
		"                                                           \n"
		"        FragColor = mix(tex1, tex2, tex2.a);               \n"
		"    }                                                      \0";

	vertex_buffer_handle vertex_buffer_;
	index_buffer_handle index_buffer_;
	
	shader_handle vertex_shader_;
	shader_handle fragment_shader_;
	program_handle program_;

	texture_handle tile_texture_;
	uniform_handle tile_texture_uniform_;

	texture_handle test_texture_;
	uniform_handle test_texture_uniform_;
	
	uniform_handle colour_uniform_;
	
	vector4 white_;
public:

	texture_application(const int argc, char* argv[])
		: application(argc, argv)
		, vertex_buffer_(graphics_.create_vertex_buffer(vertices_, sizeof vertices_, vertex_layout_))
		, index_buffer_(graphics_.create_index_buffer(indices_, sizeof indices_))
		, vertex_shader_(graphics_.create_shader(shader_type::vertex, vertex_source_))
		, fragment_shader_(graphics_.create_shader(shader_type::fragment, fragment_source_))
		, program_(graphics_.create_program(vertex_shader_, fragment_shader_))
		, colour_uniform_(graphics_.create_uniform("color", uniform_type::vec4))
		, tile_texture_uniform_(graphics_.create_uniform("tile_texture", uniform_type::texture))
		, tile_texture_(graphics_.create_texture(load(data_path() / "tile.png")))
		, test_texture_uniform_(graphics_.create_uniform("test_texture", uniform_type::texture))
		, test_texture_(graphics_.create_texture(load(data_path() / "test_alpha.png")))
		, white_(color::white().to_vector4())
	{
	}

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
			.set_index_buffer(index_buffer_, 6)
			.set_program(program_)
			.set_texture(0, tile_texture_uniform_, tile_texture_)
			.set_texture(1, test_texture_uniform_, test_texture_)
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