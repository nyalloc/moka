#pragma once

#include <graphics/command/clear_command.hpp>
#include <graphics/command/command_buffer.hpp>
#include <graphics/command/command_list.hpp>
#include <graphics/command/draw_command.hpp>
#include <graphics/command/fill_index_buffer_command.hpp>
#include <graphics/command/fill_vertex_buffer_command.hpp>
#include <graphics/command/scissor_command.hpp>
#include <graphics/command/viewport_command.hpp>
#include <vector>

namespace moka
{
    class command_list
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

        void destroy();

        void accept(graphics_visitor& device);

        void sort();

        bool is_sorted() const;

        bool is_empty() const;

        iterator begin();

        const_iterator begin() const;

        iterator end();

        const_iterator end() const;

        command_buffer& make_command_buffer(sort_key key);

        command_buffer& make_command_buffer();

        clear_command& clear();

        clear_command& clear(sort_key key);

        draw_command& draw();

        draw_command& draw(sort_key key);

        scissor_command& scissor();

        scissor_command& scissor(sort_key key);

        viewport_command& viewport();

        viewport_command& viewport(sort_key key);

        fill_index_buffer_command& fill_index_buffer();

        fill_index_buffer_command& fill_index_buffer(sort_key key);

        fill_vertex_buffer_command& fill_vertex_buffer();

        fill_vertex_buffer_command& fill_vertex_buffer(sort_key key);
    };
} // namespace moka
