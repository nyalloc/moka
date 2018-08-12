
#include <application/application.hpp>
#include <graphics_api.hpp>

using namespace moka;

struct vertex
{
    vector3 position{ 0.0f };
    vector4 color1{ 0.0f };

    constexpr vertex(const vector3& position,
        const color& color1) noexcept
        : position(position),
        color1(color1.to_vector4())
    {}

    constexpr vertex(vertex&& rhs) noexcept
        : position(std::move(rhs.position)),
        color1(std::move(rhs.color1))
    {}
};

class triangle_application : public application
{
    // triangle vertex positions
    constexpr static std::array<vertex, 3> vertices =
    {
        vertex{ vector3{  0.5f, -0.5f, 0.0f }, color::dark_red() },
        vertex{ vector3{ -0.5f, -0.5f, 0.0f }, color::dark_green() },
        vertex{ vector3{  0.0f,  0.5f, 0.0f }, color::azure() },
    };

    // vertex decl describes the memory layout of the vertex buffer
    vertex_decl vertex_decl_ = vertex_decl::builder()
        .add_attribute(attribute::position, 3, attribute_type::float32)
        .add_attribute(attribute::color0,   4, attribute_type::float32)
        .build();

    // glsl vertex shader source
    constexpr static char* vertex_source = 
    "    #version 330 core                        \n"
    "    layout (location = 0) in vec3 position;  \n"
    "    layout (location = 4) in vec4 color0;    \n"
    "    out vec3 out_color0;                     \n"
    "    void main()                              \n"
    "    {                                        \n"
    "        gl_Position = vec4(position, 1.0);   \n"
    "        out_color0 = color0.xyz;             \n"
    "    }                                        \0";

    // glsl fragment shader source
    constexpr static char* fragment_source = 
    "    #version 330 core                        \n"
    "    out vec4 FragColor;                      \n"
    "    in vec3 out_color0;                      \n"
    "    void main()                              \n"
    "    {                                        \n"
    "        FragColor = vec4(out_color0, 1.0f);  \n"
    "    }                                        \0";

    vertex_buffer_handle vertex_buffer_;
    vertex_buffer_handle create_vertex_buffer() const
    {
        return graphics_.create_vertex_buffer(
            make_buffer(vertices.data(), sizeof vertices),
            vertex_decl_);
    }

    program_handle program_;
    program_handle create_program() const
    {
        return graphics_.create_program(
            graphics_.create_shader(shader_type::vertex, vertex_source), 
            graphics_.create_shader(shader_type::fragment, fragment_source));
    }
public:
    triangle_application(const int argc, char* argv[])
        : application(argc, argv)
        , vertex_buffer_(create_vertex_buffer())
        , program_(create_program())
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