#pragma once

#include <cstdint>
#include <graphics/command/graphics_command.hpp>
#include <graphics/device/graphics_visitor.hpp>
#include <type_traits>
#include <vector>

namespace moka
{
    using sort_key = uint64_t;

    class command_buffer
    {
        std::vector<graphics_command_ptr> commands_;

        sort_key id_ = 0;

        template <typename T>
        T& emplace_back();

    public:
        explicit command_buffer(sort_key id);

        command_buffer() = default;

        ~command_buffer() = default;

        command_buffer(const command_buffer& command_buffer) = delete;

        command_buffer(command_buffer&& command_buffer) noexcept;

        command_buffer& operator=(command_buffer&& command_buffer) noexcept;

        command_buffer& operator=(const command_buffer& command_buffer) = delete;

        const sort_key& get_key() const;

        void accept(graphics_visitor& device);

        clear_command& clear();

        draw_command& draw();

        scissor_command& scissor();

        viewport_command& viewport();

        fill_index_buffer_command& fill_index_buffer();

        fill_vertex_buffer_command& fill_vertex_buffer();
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
