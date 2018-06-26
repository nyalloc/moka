
#include <application/application.hpp>
#include <graphics_api.hpp>

using namespace neon;

class triangle_application : public application
{
    // triangle vertex positions
    constexpr static std::array<float, 18> vertices =
    {
        // positions        // colors
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
       -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
    };

    // define vertex attributes
    constexpr static std::array<attribute, 2> attributes =
    {
        make_attribute<float>(0, 3, false, 6 * sizeof(float), 0),
        make_attribute<float>(1, 3, false, 6 * sizeof(float), 3)
    };

    // glsl vertex shader source
    constexpr static char* vertex_source = 
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aColor;\n"
        "out vec3 ourColor;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos, 1.0);\n"
        "   ourColor = aColor;\n"
        "}\0";

    // glsl fragment shader source
    constexpr static char* fragment_source = 
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec3 ourColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(ourColor, 1.0f);\n"
        "}\n\0";

    vertex_buffer_handle vertex_buffer_;
    program_handle program_;

    vertex_buffer_handle create_vertex_buffer() const
    {
        return graphics_.create_vertex_buffer(
            vertices.data(), 
            vertices.size(), 
            attributes.data(), 
            attributes.size());
    }

    program_handle create_program() const
    {
        return graphics_.create_program(
            graphics_.create_shader(shader_type::vertex, vertex_source), 
            graphics_.create_shader(shader_type::fragment, fragment_source));
    }
public:
    triangle_application(const int argc, char* argv[])
        : application{ argc, argv }
        , vertex_buffer_{ create_vertex_buffer() }
        , program_{ create_program() }
    {}

    ~triangle_application()
    {
        graphics_.destroy(vertex_buffer_);
        graphics_.destroy(program_);
    }

    void draw(const game_time delta_time) override
    {
        graphics_.clear_colour(colour::cornflower_blue());

        graphics_.submit(vertex_buffer_, program_);

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