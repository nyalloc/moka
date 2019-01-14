#include <graphics/command/clear_command.hpp>
#include <graphics/command/command_buffer.hpp>
#include <graphics/command/draw_command.hpp>
#include <graphics/command/fill_index_buffer_command.hpp>
#include <graphics/command/fill_vertex_buffer_command.hpp>
#include <graphics/command/scissor_command.hpp>
#include <graphics/command/viewport_command.hpp>

namespace moka
{
    command_buffer::command_buffer(sort_key id) : id_(id)
    {
    }

    command_buffer::command_buffer(command_buffer&& command_buffer) noexcept
        : commands_(std::move(command_buffer.commands_)), id_(command_buffer.id_)
    {
    }

    command_buffer& command_buffer::operator=(command_buffer&& command_buffer) noexcept
    {
        commands_ = std::move(command_buffer.commands_);
        id_ = command_buffer.id_;
        return *this;
    }

    const sort_key& command_buffer::get_key() const
    {
        return id_;
    }

    void command_buffer::accept(graphics_visitor& device)
    {
        for (auto& command : commands_)
        {
            command->accept(device);
        }
    }

    clear_command& command_buffer::clear()
    {
        return emplace_back<moka::clear_command>();
    }

    draw_command& command_buffer::draw()
    {
        return emplace_back<moka::draw_command>();
    }

    scissor_command& command_buffer::scissor()
    {
        return emplace_back<moka::scissor_command>();
    }

    viewport_command& command_buffer::viewport()
    {
        return emplace_back<moka::viewport_command>();
    }

    fill_index_buffer_command& command_buffer::fill_index_buffer()
    {
        return emplace_back<moka::fill_index_buffer_command>();
    }

    fill_vertex_buffer_command& command_buffer::fill_vertex_buffer()
    {
        return emplace_back<moka::fill_vertex_buffer_command>();
    }
} // namespace moka
