#pragma once

#include "generate_mipmaps_command.hpp"
#include <graphics/command/clear_command.hpp>
#include <graphics/command/command_buffer.hpp>
#include <graphics/command/command_list.hpp>
#include <graphics/command/draw_command.hpp>
#include <graphics/command/fill_index_buffer_command.hpp>
#include <graphics/command/fill_vertex_buffer_command.hpp>
#include <graphics/command/frame_buffer_command.hpp>
#include <graphics/command/frame_buffer_texture_command.hpp>
#include <graphics/command/scissor_command.hpp>
#include <graphics/command/set_material_properties_command.hpp>
#include <graphics/command/viewport_command.hpp>
#include <vector>

namespace moka
{
    /**
     * \brief A command_list is a list of command_buffer objects that can be sorted before being submitted to a graphics_device.
     */
    class command_list final
    {
        bool is_sorted_ = false;
        sort_key current_key_ = 0;
        std::vector<command_buffer> command_packets_;

        using iterator = std::vector<command_buffer>::iterator;
        using const_iterator = std::vector<command_buffer>::const_iterator;

        struct key_comparator final
        {
            bool operator()(const command_buffer& left, const command_buffer& right) const;
        };

    public:
        command_list();

        ~command_list();

        command_list(const command_list& command_list) = delete;

        command_list(command_list&& command_list) noexcept;

        command_list& operator=(command_list&& command_list) noexcept;

        command_list& operator=(const command_list& command_list) = delete;

        /**
         * \brief Clear the command_list.
         */
        void destroy();

        /**
         * \brief Accept a graphics_visitor object and pass every command to it.
         * \param device The graphics_visitor object.
         */
        void accept(graphics_visitor& device);

        /**
         * \brief Sort the command_list.
         */
        void sort();

        /**
         * \brief Is this command_list sorted?
         * \return True if the command_list is sorted. Otherwise, false.
         */
        bool is_sorted() const;

        /**
         * \brief Is the command_list empty?
         * \return True if the command_list is empty. Otherwise, false.
         */
        bool is_empty() const;

        iterator begin();

        const_iterator begin() const;

        iterator end();

        const_iterator end() const;

        /**
         * \brief Create and return a command_buffer object.
         * \return A reference to the new command_buffer object.
         */
        command_buffer& make_command_buffer();

        /**
         * \brief Create and return a command_buffer object.
         * \param key Use this sort_key to sort the command.
         * \return A reference to the new command_buffer object.
         */
        command_buffer& make_command_buffer(sort_key key);

        /**
         * \brief Create and return a set_material_parameters_command object.
         * \return A reference to the new set_material_parameters_command object.
         */
        set_material_parameters_command& set_material_parameters();

        /**
         * \brief Create and return a set_material_parameters_command object.
         * \param key Use this sort_key to sort the command.
         * \return A reference to the new set_material_parameters_command object.
         */
        set_material_parameters_command& set_material_parameters(sort_key key);

        /**
         * \brief Create and return a frame_buffer_command object.
         * \return A reference to the new frame_buffer_command object.
         */
        frame_buffer_command& frame_buffer();

        /**
         * \brief Create and return a frame_buffer_command object.
         * \param key Use this sort_key to sort the command.
         * \return A reference to the new frame_buffer_command object.
         */
        frame_buffer_command& frame_buffer(sort_key key);

        /**
         * \brief Create and return a frame_buffer_texture_command object.
         * \return A reference to the new frame_buffer_texture_command object.
         */
        frame_buffer_texture_command& frame_buffer_texture();

        /**
         * \brief Create and return a frame_buffer_texture_command object.
         * \param key Use this sort_key to sort the command.
         * \return A reference to the new frame_buffer_texture_command object.
         */
        frame_buffer_texture_command& frame_buffer_texture(sort_key key);

        /**
         * \brief Create and return a clear_command object.
         * \return A reference to the new clear_command object.
         */
        clear_command& clear();

        /**
         * \brief Create and return a clear_command object.
         * \param key Use this sort_key to sort the command.
         * \return A reference to the new clear_command object.
         */
        clear_command& clear(sort_key key);

        /**
         * \brief Create and return a draw_command object.
         * \return A reference to the new draw_command object.
         */
        draw_command& draw();

        /**
         * \brief Create and return a draw_command object.
         * \param key Use this sort_key to sort the command.
         * \return A reference to the new draw_command object.
         */
        draw_command& draw(sort_key key);

        /**
         * \brief Create and return a scissor_command object.
         * \return A reference to the new scissor_command object.
         */
        scissor_command& scissor();

        /**
         * \brief Create and return a scissor_command object.
         * \param key Use this sort_key to sort the command.
         * \return A reference to the new scissor_command object.
         */
        scissor_command& scissor(sort_key key);

        /**
         * \brief Create and return a viewport_command object.
         * \return A reference to the new viewport_command object.
         */
        viewport_command& viewport();

        /**
         * \brief Create and return a viewport_command object.
         * \param key Use this sort_key to sort the command.
         * \return A reference to the new viewport_command object.
         */
        viewport_command& viewport(sort_key key);

        /**
         * \brief Create and return a fill_index_buffer_command object.
         * \return A reference to the new fill_index_buffer_command object.
         */
        fill_index_buffer_command& fill_index_buffer();

        /**
         * \brief Create and return a fill_index_buffer_command object.
         * \param key Use this sort_key to sort the command.
         * \return A reference to the new fill_index_buffer_command object.
         */
        fill_index_buffer_command& fill_index_buffer(sort_key key);

        /**
         * \brief Create and return a fill_vertex_buffer_command object.
         * \return A reference to the new fill_vertex_buffer_command object.
         */
        fill_vertex_buffer_command& fill_vertex_buffer();

        /**
         * \brief Create and return a fill_vertex_buffer_command object.
         * \param key Use this sort_key to sort the command.
         * \return A reference to the new fill_vertex_buffer_command object.
         */
        fill_vertex_buffer_command& fill_vertex_buffer(sort_key key);

        /**
         * \brief Create and return a generate_mipmaps_command object.
         * \return A reference to the new generate_mipmaps_command object.
         */
        generate_mipmaps_command& generate_mipmaps();

        /**
         * \brief Create and return a generate_mipmaps_command object.
         * \param key Use this sort_key to sort the command.
         * \return A reference to the new generate_mipmaps_command object.
         */
        generate_mipmaps_command& generate_mipmaps(sort_key key);
    };
} // namespace moka
