#pragma once

#include <graphics/buffer/index_buffer_handle.hpp>
#include <graphics/command/graphics_command.hpp>

namespace moka
{
    class fill_index_buffer_command final : public graphics_command
    {
    public:
        index_buffer_handle handle;
        const void* data;
        size_t size;

        virtual ~fill_index_buffer_command();

        void accept(graphics_visitor& visitor) override;

        fill_index_buffer_command& set_buffer(index_buffer_handle handle, const void* data, size_t size);
    };
} // namespace moka
