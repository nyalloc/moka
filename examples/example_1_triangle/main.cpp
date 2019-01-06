//
//#include <application/application.hpp>
//#include <graphics/graphics_device.hpp>
//#include <graphics/vertex_layout.hpp>
//#include <graphics/vertex_layout_builder.hpp>
//#include <graphics/command_list.hpp>
//#include <graphics/vertex_buffer.hpp>
//#include <graphics/index_buffer.hpp>
//#include <graphics/material.hpp>
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
//		"    void main()                                     \n"
//		"    {                                               \n"
//		"        FragColor = vec4(out_color0, 1.0f);         \n"
//		"    }                                               \0";
//
//	vertex_buffer vertex_buffer_;
//	index_buffer index_buffer_;
//	material material_;
//
//public:
//
//	texture_application(const texture_application& lhs) = delete;
//	texture_application(texture_application&& lhs) = delete;
//	texture_application& operator=(const texture_application& lhs) = delete;
//	texture_application& operator=(texture_application&& lhs) = delete;
//
//	texture_application(const app_settings& app_settings)
//		: application(app_settings)
//	{
//		vertex_layout layout = 
//		{
//			{ 0, attribute_type::float32, 2, false, 6 * sizeof(float), 0                 },
//			{ 1, attribute_type::float32, 3, false, 6 * sizeof(float), 3 * sizeof(float) }
//		};
//
//		vertex_buffer_ = graphics_.make_vertex_buffer(
//			vertices_, 
//			sizeof vertices_,
//			std::move(layout),
//			buffer_usage::static_draw);
//
//		//material_ = graphics_.make_program(
//		//	graphics_.make_shader(
//		//		shader_type::vertex,
//		//		vertex_source_), 
//		//	graphics_.make_shader(
//		//		shader_type::fragment, fragment_source_));
//	}
//
//	~texture_application()
//	{
//		// todo: destroy graphics resources 
//		timer_.stop();
//	}
//
//	void draw(const game_time delta_time) override
//	{
//		command_list list;
//
//		list.clear()
//			.set_color(1.0f, 0.0f, 0.0f, 1.0f)
//			.set_clear_color(true)
//			.set_clear_depth(true);
//
//		list.draw()
//			.set_vertex_buffer(vertex_buffer_)
//			.set_material(material_)
//			.set_vertex_count(3);
//
//		graphics_.submit_and_swap(std::move(list));
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
//		std::filesystem::path result{ MOKA_ASSET_PATH };
//		return result.lexically_normal();
//	}
//};
//
//int main(const int argc, char* argv[])
//{
//	const app_settings settings(argc, argv);
//	return texture_application{ settings }.run();
//}
int main(const int argc, char* argv[])
{
	return 0;
}