#pragma once

#include <graphics/buffer/vertex_buffer.hpp>
#include <graphics/command/graphics_command.hpp>

namespace moka
{
    class fill_vertex_buffer_command final : public graphics_command
    {
    public:
        vertex_buffer handle;
        const void* data;
        size_t size;

        virtual ~fill_vertex_buffer_command();

        void accept(graphics_visitor& visitor) override;

        fill_vertex_buffer_command& set_buffer(vertex_buffer handle, const void* data, size_t size);
    };
} // namespace moka
