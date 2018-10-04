//
//#include <application/application.hpp>
//#include <graphics/graphics_device.hpp>
//#include <graphics/draw_call_builder.hpp>
//
//using namespace moka;
//
//class texture_application : public application
//{
//	float vertices_[18] = 
//	{
//		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
//		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
//		0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
//	};
//
//	vertex_layout vertex_layout_ = vertex_layout::builder()
//		.add_attribute(0, attribute_type::float32, 3, false, 3 * sizeof(float), 0)
//		.add_attribute(1, attribute_type::float32, 3, false, 3 * sizeof(float), 3 * sizeof(float))
//		.build();
//
//	const char* vertex_source_ =
//		"    #version 330 core                               \n"
//		"    layout (location = 0) in vec3 position;         \n"
//		"    layout (location = 1) in vec4 color0;           \n"
//		"    out vec3 out_color0;                            \n"
//		"    void main()                                     \n"
//		"    {                                               \n"
//		"        gl_Position = vec4(position, 1.0);          \n"
//		"        out_color0 = color0.xyz;                    \n"
//		"    }                                               \0";
//
//	const char* fragment_source_ =
//		"    #version 330 core                               \n"
//		"    out vec4 FragColor;                             \n"
//		"    in vec3 out_color0;                             \n"
//		"    uniform vec4 color;                             \n"
//		"    void main()                                     \n"
//		"    {                                               \n"
//		"        FragColor = vec4(out_color0, 1.0f) * color; \n"
//		"    }                                               \0";
//
//	vertex_buffer_handle vertex_buffer_;
//	index_buffer_handle index_buffer_;
//	shader_handle vertex_shader_;
//	shader_handle fragment_shader_;
//	program_handle program_;
//
//	uniform_handle colour_uniform_;
//	glm::vec4 white_;
//public:
//
//	texture_application(const int argc, char* argv[])
//		: application(argc, argv)
//		, vertex_buffer_(graphics_.create_vertex_buffer(vertices_, sizeof vertices_, vertex_layout_))
//		, vertex_shader_(graphics_.create_shader(shader_type::vertex, vertex_source_))
//		, fragment_shader_(graphics_.create_shader(shader_type::fragment, fragment_source_))
//		, program_(graphics_.create_program(vertex_shader_, fragment_shader_))
//		, colour_uniform_(graphics_.create_uniform("color", uniform_type::vec4, 1))
//		, white_(color::white())
//	{}
//
//	~texture_application()
//	{
//		graphics_.destroy(vertex_buffer_);
//		graphics_.destroy(program_);
//		timer_.stop();
//	}
//
//	void draw(const game_time delta_time) override
//	{
//		float current_time = elapsed();
//
//		float colour_val = (sin(current_time) / 2.0f) + 0.5f;
//
//		graphics_.begin()
//			.set_vertex_buffer(vertex_buffer_, 0, 3)
//			.set_program(program_)
//			.set_uniform(colour_uniform_, white_ * colour_val)
//			.end();
//
//		graphics_.frame();
//	}
//
//	void update(const game_time delta_time) override
//	{
//	}
//
//	std::filesystem::path data_path() override
//	{
//		// cmake-defined macro points to example project asset folder relative to source.
//		// A real application could point this wherever it wanted.
//		std::filesystem::path result{ ASSET_PATH };
//		return result.lexically_normal();
//	}
//};
//
//int main(const int argc, char* argv[])
//{
//	return texture_application{ argc, argv }.run();
//}
int main(const int argc, char* argv[])
{
	return 0;
}