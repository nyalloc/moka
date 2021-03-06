/*
===========================================================================
Moka Source Code
Copyright 2019 Stuart Adams. All rights reserved.
https://github.com/stuartdadams/moka
stuartdadams | linkedin.com/in/stuartdadams

This file is part of the Moka Real-Time Physically-Based Rendering Project.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

===========================================================================
*/
#pragma once

#include <cstdint>
#include <graphics/command/graphics_command.hpp>
#include <graphics/device/graphics_visitor.hpp>
#include <type_traits>
#include <vector>

namespace moka
{
    using sort_key = uint64_t;

    /**
     * \brief A command_buffer is a collection of render commands married by a sort_key. Command buffers are owned by a command_list.
     */
    class command_buffer
    {
        std::vector<graphics_command_ptr> commands_;

        sort_key id_ = 0;

        template <typename T>
        T& emplace_back();

    public:
        /**
         * \brief Create a new command_buffer object.
         * \param id The sort key that we should use for this command_buffer.
         */
        explicit command_buffer(sort_key id);

        command_buffer() = default;

        ~command_buffer() = default;

        command_buffer(const command_buffer& command_buffer) = delete;

        command_buffer(command_buffer&& command_buffer) noexcept;

        command_buffer& operator=(command_buffer&& command_buffer) noexcept;

        command_buffer& operator=(const command_buffer& command_buffer) = delete;

        /**
         * \brief Get the sort key of this command_buffer.
         * \return The sort key of this command_buffer.
         */
        const sort_key& get_key() const;

        /**
         * \brief Accept a graphics_visitor object and pass every command to it.
         * \param device The graphics_visitor object.
         */
        void accept(graphics_visitor& device);

        /**
         * \brief Create and return a frame_buffer_command object.
         * \return A reference to the new frame_buffer_command object.
         */
        frame_buffer_command& frame_buffer();

        /**
         * \brief Create and return a frame_buffer_texture_command object.
         * \return A reference to the new frame_buffer_texture_command object.
         */
        frame_buffer_texture_command& frame_buffer_texture();

        /**
         * \brief Create and return a clear_command object.
         * \return A reference to the new clear_command object.
         */
        clear_command& clear();

        /**
         * \brief Create and return a draw_command object.
         * \return A reference to the new draw_command object.
         */
        draw_command& draw();

        /**
         * \brief Create and return a scissor_command object.
         * \return A reference to the new scissor_command object.
         */
        scissor_command& scissor();

        /**
         * \brief Create and return a viewport_command object.
         * \return A reference to the new viewport_command object.
         */
        viewport_command& viewport();

        /**
         * \brief Create and return a fill_index_buffer_command object.
         * \return A reference to the new fill_index_buffer_command object.
         */
        fill_index_buffer_command& fill_index_buffer();

        /**
         * \brief Create and return a fill_vertex_buffer_command object.
         * \return A reference to the new fill_vertex_buffer_command object.
         */
        fill_vertex_buffer_command& fill_vertex_buffer();

        /**
         * \brief Create and return a generate_mipmaps_command object.
         * \return A reference to the new generate_mipmaps_command object.
         */
        generate_mipmaps_command& generate_mipmaps();

        /**
         * \brief Create and return a set_material_parameters_command object.
         * \return A reference to the new set_material_parameters_command object.
         */
        set_material_parameters_command& set_material_parameters();
    };

    template <typename T>
    T& command_buffer::emplace_back()
    {
        static_assert(
            std::is_base_of_v<graphics_command, T>,
            "Please only emplace commands that derive from the "
            "graphics_command interface!");

        commands_.emplace_back(std::make_unique<T>());

        auto& ptr = commands_.back();
        auto cast = static_cast<T*>(ptr.get());
        return *cast;
    }
} // namespace moka
