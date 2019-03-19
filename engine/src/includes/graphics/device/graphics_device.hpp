#pragma once

#include <application/window.hpp>
#include <graphics/api/graphics_api.hpp>
#include <graphics/buffer/frame_buffer_handle.hpp>
#include <graphics/command/command_list.hpp>
#include <graphics/material/material_builder.hpp>
#include <memory>

namespace moka
{
    /**
     * \brief The type of graphics api to initialize.
     */
    enum class graphics_backend
    {
        direct3d_9,
        direct3d_11,
        direct3d_12,
        gnm,
        metal,
        opengl_es,
        opengl,
        vulkan,
        null
    };

    /* string containing unique identifier of the texture
     */
    using texture_id = std::string;

    /**
     * \brief A cache of loaded textures. Used to avoid loading the same file multiple times when it is referenced multiple times in a 3D model.
     */
    class texture_cache
    {
        graphics_device& device_;

        std::vector<texture_handle> textures_;
        std::unordered_map<texture_id, int> texture_lookup_;

    public:
        /**
         * \brief Create a new texture cache object.
         * \param device The graphics device object to use.
         * \param initial_capacity The initial capacity of the texture cache.
         */
        explicit texture_cache(graphics_device& device, size_t initial_capacity = 0);

        /**
         * \brief Add a texture to the texture cache.
         * \param handle The texture you want to add to the cache.
         * \param id The path's unique identifier.
         */
        void add_texture(texture_handle handle, const texture_id& id);

        /**
         * \brief Check if a texture already exists in the cache.
         * \param id The texture's unique identifier.
         * \return True if the texture is cached already, false if it is not present.
         */
        bool exists(const texture_id& id) const;

        /**
         * \brief Get a texture through its unique identifier.
         * \param id The texture's unique identifier.
         * \return The texture identified by the id.
         */
        texture_handle get_texture(const texture_id& id) const;
    };

    /* string containing unique identifier of shader source code + all preprocessor definitions
     * the preprocessor definitions are necessary to differentiate different configurations of the same shader source
     */
    using program_id = std::string;

    /**
     * \brief A cache of loaded programs. Used to avoid loading the same program multiple times.
     */
    class program_cache
    {
        graphics_device& device_;

        std::vector<program_handle> shaders_;
        std::unordered_map<program_id, int> shader_lookup_;

    public:
        /**
         * \brief Create a new program cache object.
         * \param device The graphics device object to use.
         * \param initial_capacity The initial capacity of the texture cache.
         */
        explicit program_cache(graphics_device& device, size_t initial_capacity = 0);

        /**
         * \brief Add a program to the program cache.
         * \param handle The program you want to add to the cache.
         * \param id The program's unique identifier.
         */
        void add_program(program_handle handle, const program_id& id);

        /**
         * \brief Check if a program already exists in the cache.
         * \param id The program's unique identifier.
         * \return True if the program is cached already, false if it is not present.
         */
        bool exists(const program_id& id) const;

        /**
         * \brief Get a program through its unique identifier.
         * \param id The program's unique identifier.
         * \return The program identified by the id.
         */
        program_handle get_program(const program_id& id) const;
    };

    /**
     * \brief A cache of materials.
     */
    class material_cache
    {
        graphics_device& device_;

        std::vector<material> materials_;

    public:
        /**
         * \brief Create a new material cache object.
         * \param device The graphics device object to use.
         * \param initial_capacity The initial capacity of the texture cache.
         */
        explicit material_cache(graphics_device& device, size_t initial_capacity = 0);

        /**
         * \brief Add a material to the material cache.
         * \param material The material you want to add to the cache.
         * \return The material id.
         */
        material_handle add_material(material&& material);

        /**
         * \brief Get the material identified by its id.
         * \param handle The material id.
         * \return The material identified by the id.
         */
        material* get_material(material_handle handle);

        /**
         * \brief Get the material identified by its id.
         * \param handle The material id.
         * \return The material identified by the id.
         */
        const material* get_material(material_handle handle) const;
    };

    class graphics_device
    {
        std::unique_ptr<graphics_api> graphics_api_;

        texture_cache textures_;

        program_cache shaders_;

        material_cache materials_;

    public:
        /**
         * \brief Get the texture cache.
         * \return The texture cache.
         */
        texture_cache& get_texture_cache();

        /**
         * \brief Get the texture cache.
         * \return The texture cache.
         */
        const texture_cache& get_texture_cache() const;

        /**
         * \brief Get the program cache.
         * \return The program cache.
         */
        program_cache& get_program_cache();

        /**
         * \brief Get the program cache.
         * \return The program cache.
         */
        const program_cache& get_program_cache() const;

        /**
         * \brief Get the material cache.
         * \return The material cache.
         */
        material_cache& get_material_cache();

        /**
         * \brief Get the material cache.
         * \return The material cache.
         */
        const material_cache& get_material_cache() const;

        /**
         * \brief Create a graphics device object
         * \param window The window to attach to this graphics device
         * \param graphics_backend The graphics API to use with this graphics device
         */
        explicit graphics_device(window& window, graphics_backend graphics_backend = graphics_backend::opengl);

        /**
         * \brief Create a new vertex buffer.
         * \param vertices The host memory buffer that will be used as vertex data.
         * \param size The size of the host vertex buffer.
         * \param layout The layout of the vertex data.
         * \param use A buffer usage hint.
         * \return A new vertex_buffer_handle representing an vertex buffer on the device.
         */
        vertex_buffer_handle make_vertex_buffer(
            const void* vertices, size_t size, vertex_layout&& layout, buffer_usage use) const;

        /**
         * \brief Create a new index buffer.
         * \param indices The host memory buffer that will be used as index data.
         * \param size The size of the host index buffer.
         * \param type The layout of the index data.
         * \param use A buffer usage hint.
         * \return A new index_buffer_handle representing an index buffer on the device.
         */
        index_buffer_handle make_index_buffer(
            const void* indices, size_t size, index_type type, buffer_usage use) const;

        /**
         * \brief Create a shader from source code.
         * \param type The type of shader you want to create.
         * \param source The source code of the shader you want to create.
         * \return A new shader_handle representing a shader on the device.
         */
        shader_handle make_shader(shader_type type, const std::string& source) const;

        /**
         * \brief Create a shader program from vertex & fragment shaders.
         * \param vertex_handle The vertex shader that you want to link to this program.
         * \param fragment_handle The fragment shader that you want to link to this program.
         * \return A new program_handle representing a program on the device.
         */
        program_handle make_program(shader_handle vertex_handle, shader_handle fragment_handle) const;

        /**
         * \brief Create a new texture.
         * \param data The host memory buffer that will be used as texture data.
         * \param metadata Metadata describing the texture data.
         * \param free_host_data If true, free the host memory after uploading to the device. Otherwise allow the calling code to free it.
         * \return A new texture_handle representing a texture on the device.
         */
        texture_handle make_texture(void** data, texture_metadata&& metadata, bool free_host_data) const;

        /**
         * \brief Create a texture builder object.
         * \return A new texture builder.
         */
        texture_builder build_texture();

        /**
         * \brief Create a new frame buffer.
         * \param render_textures An array of render_texture_data.
         * \param render_texture_count Size of the render_textures array.
         * \return A new frame_buffer_handle representing a frame buffer on the device.
         */
        frame_buffer_handle make_frame_buffer(render_texture_data* render_textures, size_t render_texture_count) const;

        /**
         * \brief Create a frame buffer builder object.
         * \return A new frame buffer builder.
         */
        frame_buffer_builder build_frame_buffer();

        /**
         * \brief Create a material builder object.
         * \return A new material builder.
         */
        material_builder build_material();

        /**
         * \brief Destroy a program.
         * \param handle The program you want to destroy.
         */
        void destroy(program_handle handle);

        /**
         * \brief Destroy a shader.
         * \param handle The shader you want to destroy.
         */
        void destroy(shader_handle handle);

        /**
         * \brief Destroy a framebuffer.
         * \param handle The framebuffer you want to destroy.
         */
        void destroy(frame_buffer_handle handle);

        /**
         * \brief Destroy a vertex buffer.
         * \param handle The vertex buffer you want to destroy.
         */
        void destroy(vertex_buffer_handle handle);

        /**
         * \brief Destroy an index buffer.
         * \param handle The index buffer you want to destroy.
         */
        void destroy(index_buffer_handle handle);

        /**
         * \brief Submit a command_list to execute on the device.
         * \param command_list The command_list you wish to run.
         * \param sort Sort the command list before submitting it to the graphics device.
         */
        void submit(command_list&& command_list, bool sort = true) const;

        /**
         * \brief Submit a command_list to execute on the device. The main framebuffer will be
         * swapped after executing, advancing a frame.
         *  \param command_list The command_list you wish to run.
         * \param sort Sort the command list before submitting it to the graphics device.
         */
        void submit_and_swap(command_list&& command_list, bool sort = true) const;
    };
} // namespace moka
