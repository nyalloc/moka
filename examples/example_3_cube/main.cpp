
#include <application/application.hpp>
#include <graphics/graphics_device.hpp>
#include <graphics/draw_call_builder.hpp>
#include <maths/vector2.hpp>
#include <maths/vector3.hpp>
#include <maths/vector4.hpp>
#include <asset_importer/texture_import.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <graphics/camera.hpp>

using namespace moka;

struct texture_uniform
{
	texture_handle texture;
	uniform_handle uniform;
};

struct material_uniforms
{
	texture_uniform albedo;
	texture_uniform specular;
	uniform_handle shininess;
};

struct light
{
	uniform_handle position;
	uniform_handle ambient;
	uniform_handle diffuse;
	uniform_handle specular;
};

class lighting_application : public application
{
	float vertices_[288] =
	{
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

	vertex_layout vertex_layout_ = vertex_layout::builder()
		.add_attribute(0, attribute_type::float32, 3, false, 8 * sizeof(float), 0)
		.add_attribute(1, attribute_type::float32, 3, false, 8 * sizeof(float), 3 * sizeof(float))
		.add_attribute(2, attribute_type::float32, 2, false, 8 * sizeof(float), 6 * sizeof(float))
		.build();

	const char* cube_vertex_ =
		R"(
		#version 330 core
		layout (location = 0) in vec3 aPos;
		layout (location = 1) in vec3 aNormal;
		layout (location = 2) in vec2 aTexCoords;

		out vec3 FragPos;
		out vec3 Normal;
		out vec2 TexCoords;

		uniform mat4 model;
		uniform mat4 view;
		uniform mat4 projection;

		void main()
		{
			FragPos = vec3(model * vec4(aPos, 1.0));
			Normal = mat3(transpose(inverse(model))) * aNormal;  
			TexCoords = aTexCoords;
    
			gl_Position = projection * view * vec4(FragPos, 1.0);
		}
	)";

	const char* cube_fragment_ =
		R"(
		#version 330 core
		out vec4 FragColor;

		struct Material 
		{
			sampler2D diffuse;
			sampler2D specular;    
			float shininess;
		}; 

		struct Light 
		{
			vec3 position;
			vec3 ambient;
			vec3 diffuse;
			vec3 specular;
		};

		in vec3 FragPos;  
		in vec3 Normal;  
		in vec2 TexCoords;
  
		uniform vec3 viewPos;
		uniform Material material;
		uniform Light light;

		void main()
		{
			// ambient
			vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
  	
			// diffuse 
			vec3 norm = normalize(Normal);
			vec3 lightDir = normalize(light.position - FragPos);
			float diff = max(dot(norm, lightDir), 0.0);
			vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
    
			// specular
			vec3 viewDir = normalize(viewPos - FragPos);
			vec3 reflectDir = reflect(-lightDir, norm);  
			float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
			vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;  
        
			vec3 result = ambient + diffuse + specular;
			FragColor = vec4(result, 1.0);
		}
	)";

	const char* lamp_vertex_ =
		R"(
		#version 330 core
		layout (location = 0) in vec3 aPos;

		uniform mat4 model;
		uniform mat4 view;
		uniform mat4 projection;

		void main()
		{
			gl_Position = projection * view * model * vec4(aPos, 1.0);
		}
	)";

	const char* lamp_fragment_ =
		R"(
		#version 330 core
		out vec4 FragColor;

		uniform vec3 colour;

		void main()
		{
			FragColor = vec4(colour, 1.0f); 
		}
	)";

	vertex_buffer_handle vertex_buffer_;

	program_handle cube_program_;
	program_handle lamp_program_;

	uniform_handle model_uniform_;
	uniform_handle colour_;

	uniform_handle view_uniform_;
	uniform_handle projection_uniform_;
	uniform_handle view_pos_;

	light light_;
	material_uniforms material_;

	camera camera_;
public:

	lighting_application(const int argc, char* argv[])
		: application(argc, argv)

		, camera_(camera::builder()
			.set_perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f)
			.set_position(glm::vec3(-3.0f, 0.75f, 2.5f))
			.set_view_target(glm::vec3{ 0 })
			.build())
	{
		model_uniform_ = graphics_.create_uniform("model", uniform_type::mat4);

		shader_handle vert = graphics_.create_shader(shader_type::vertex, cube_vertex_);
		shader_handle frag = graphics_.create_shader(shader_type::fragment, cube_fragment_);
		cube_program_ = graphics_.create_program(vert, frag);

		vert = graphics_.create_shader(shader_type::vertex, lamp_vertex_);
		frag = graphics_.create_shader(shader_type::fragment, lamp_fragment_);
		lamp_program_ = graphics_.create_program(vert, frag);

		vertex_buffer_ = graphics_.create_vertex_buffer(vertices_, sizeof vertices_, vertex_layout_);

		material_.albedo.uniform = graphics_.create_uniform("material.diffuse", uniform_type::texture);
		material_.specular.uniform = graphics_.create_uniform("material.specular", uniform_type::texture);
		material_.shininess = graphics_.create_uniform("material.shininess", uniform_type::float32);
		material_.albedo.texture = graphics_.create_texture(load(data_path() / "crate_diffuse.png"));
		material_.specular.texture = graphics_.create_texture(load(data_path() / "crate_specular.png"));

		light_.position = graphics_.create_uniform("light.position", uniform_type::vec3);
		light_.ambient = graphics_.create_uniform("light.ambient", uniform_type::vec3);
		light_.diffuse = graphics_.create_uniform("light.diffuse", uniform_type::vec3);
		light_.specular = graphics_.create_uniform("light.specular", uniform_type::vec3);

		colour_ = graphics_.create_uniform("colour", uniform_type::vec3);

		view_uniform_ = graphics_.create_uniform("view", uniform_type::mat4);
		projection_uniform_ = graphics_.create_uniform("projection", uniform_type::mat4);
		view_pos_ = graphics_.create_uniform("viewPos", uniform_type::vec3);
	}

	~lighting_application()
	{
		graphics_.destroy(vertex_buffer_);
		graphics_.destroy(cube_program_);
		timer_.stop();
	}

	void draw(const game_time delta_time) override
	{
		glm::mat4 model;

		auto light_pos = glm::vec3(0.0f, 0.5f, 1.0f);

		auto sin_val = sin(elapsed());

		light_pos.x = sin_val * 0.5f;

		glm::vec4 white = color::white();
		glm::vec4 ambient = color::night_rider();
		glm::vec4 red = color::red();
		glm::vec4 blue = color::blue();
		glm::vec4 color = glm::mix(red, blue, sin_val);

		graphics_.begin()
			.set_vertex_buffer(vertex_buffer_, 0, 36)
			.set_program(cube_program_)
			.set_texture(0, material_.albedo.uniform, material_.albedo.texture)
			.set_texture(1, material_.specular.uniform, material_.specular.texture)
			.set_uniform(material_.shininess, 64.0f)
			.set_uniform(model_uniform_, model)
			.set_uniform(light_.position, light_pos)
			.set_uniform(light_.ambient, ambient)
			.set_uniform(light_.diffuse, color)
			.set_uniform(light_.specular, white)
			.set_uniform(view_uniform_, camera_.get_view())
			.set_uniform(projection_uniform_, camera_.get_projection())
			.set_uniform(view_pos_, camera_.get_transform().get_position())
			.end();

		model = glm::mat4();
		model = glm::translate(model, light_pos);
		model = glm::scale(model, glm::vec3(0.2f));

		graphics_.begin()
			.set_vertex_buffer(vertex_buffer_, 0, 36)
			.set_program(lamp_program_)
			.set_uniform(model_uniform_, model)
			.set_uniform(colour_, color)
			.set_uniform(view_uniform_, camera_.get_view())
			.set_uniform(projection_uniform_, camera_.get_projection())
			.set_uniform(view_pos_, camera_.get_transform().get_position())
			.end();

		graphics_.frame();
	}

	void update(const game_time delta_time) override
	{
		camera_.update();

		auto normalize = [](const glm::vec3& vector)
		{
			if (vector != glm::vec3{ 0 })
			{
				return glm::normalize(vector);
			}
			return glm::vec3{ 0 };
		};

		auto& camera_transform = camera_.get_transform();

		const auto& state = keyboard_.get_state();

		glm::vec3 movement_direction;

		if (state.is_key_down(key::key_w))
		{
			movement_direction += camera_transform.front();
		}
		if (state.is_key_down(key::key_s))
		{
			movement_direction += camera_transform.back();
		}
		if (state.is_key_down(key::key_a))
		{
			movement_direction += camera_transform.left();
		}
		if (state.is_key_down(key::key_d))
		{
			movement_direction += camera_transform.right();
		}

		const auto& current_pos = camera_transform.get_position();
		auto target_pos = glm::mix(current_pos, current_pos + normalize(movement_direction), delta_time);

		camera_transform.set_position(target_pos);

		camera_transform.look_at(glm::vec3{ 0 });
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
	return lighting_application{ argc, argv }.run();
}