//
//#include <application/application.hpp>
//#include <graphics/graphics_device.hpp>
//#include <graphics/draw_call_builder.hpp>
//#include <maths/vector2.hpp>
//#include <maths/vector3.hpp>
//#include <maths/vector4.hpp>
//#include <asset_importer/texture_import.hpp>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//
//using namespace moka;
//
//class texture_application : public application
//{
//	float vertices_[32] = 
//	{
//		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 
//		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
//		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 
//		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  
//	};
//
//	uint32_t indices_[6] =
//	{
//		0, 1, 3,
//		1, 2, 3
//	};
//
//	vertex_layout vertex_layout_ = vertex_layout::builder()
//		.add_attribute(0, attribute_type::float32, 3, false, 3 * sizeof(float), 0)
//		.add_attribute(1, attribute_type::float32, 3, false, 3 * sizeof(float), 3 * sizeof(float))
//		.add_attribute(2, attribute_type::float32, 2, false, 2 * sizeof(float), 6 * sizeof(float))
//		.build();
//
//	const char* vertex_source_ =
//		"    #version 330 core                                  \n"
//		"    layout (location = 0)  in vec3 position;           \n"
//		"    layout (location = 1)  in vec4 color0;             \n"
//		"    layout (location = 2)  in vec2 tex_coord0;         \n"
//		"    out vec2 tex_coord;                                \n" 
//		"    uniform mat4 transform;                            \n"
//		"    void main()                                        \n"
//		"    {                                                  \n"
//		"        gl_Position = transform * vec4(position, 1.0); \n"
//		"        tex_coord = tex_coord0;                        \n"
//		"    }                                                  \0";
//
//	const char* fragment_source_ =
//		"    #version 330 core                                      \n"
//		"    out vec4 FragColor;                                    \n"
//		"    in vec2 tex_coord;                                     \n"
//		"    uniform sampler2D tile_texture;                        \n"
//		"    uniform sampler2D test_texture;                        \n"
//		"    void main()                                            \n"
//		"    {                                                      \n"
//		"        vec4 tex1 = texture(tile_texture, tex_coord).rgba;	\n"
//		"        vec4 tex2 = texture(test_texture, tex_coord).rgba;	\n"
//		"                                                           \n"
//		"        FragColor = mix(tex1, tex2, tex2.a);               \n"
//		"    }                                                      \0";
//
//	vertex_buffer_handle vertex_buffer_;
//	index_buffer_handle index_buffer_;
//	
//	shader_handle vertex_shader_;
//	shader_handle fragment_shader_;
//	program_handle program_;
//
//	texture_handle tile_texture_;
//	uniform_handle tile_texture_uniform_;
//
//	texture_handle test_texture_;
//	uniform_handle test_texture_uniform_;
//	
//	uniform_handle transform_uniform_;
//	
//	vector4 white_;
//public:
//
//	texture_application(const int argc, char* argv[])
//		: application(argc, argv)
//		, vertex_buffer_(graphics_.create_vertex_buffer(vertices_, sizeof vertices_, vertex_layout_))
//		, index_buffer_(graphics_.create_index_buffer(indices_, sizeof indices_))
//		, vertex_shader_(graphics_.create_shader(shader_type::vertex, vertex_source_))
//		, fragment_shader_(graphics_.create_shader(shader_type::fragment, fragment_source_))
//		, program_(graphics_.create_program(vertex_shader_, fragment_shader_))
//		, tile_texture_uniform_(graphics_.create_uniform("tile_texture", uniform_type::texture))
//		, tile_texture_(graphics_.create_texture(load(data_path() / "tile.png")))
//		, test_texture_uniform_(graphics_.create_uniform("test_texture", uniform_type::texture))
//		, test_texture_(graphics_.create_texture(load(data_path() / "test_alpha.png")))
//		, transform_uniform_(graphics_.create_uniform("transform", uniform_type::mat4))
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
//		glm::mat4 trans;
//		trans = glm::rotate(trans, current_time, glm::vec3(0.0f, 0.0f, 1.0f));
//
//		graphics_.begin()
//			.set_vertex_buffer(vertex_buffer_, 0, 3)
//			.set_index_buffer(index_buffer_, 6)
//			.set_program(program_)
//			.set_texture(0, tile_texture_uniform_, tile_texture_)
//			.set_texture(1, test_texture_uniform_, test_texture_)
//			.set_uniform(transform_uniform_, trans)
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