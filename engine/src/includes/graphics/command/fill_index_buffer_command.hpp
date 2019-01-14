#pragma once

#include <graphics/buffer/index_buffer.hpp>
#include <graphics/device/graphics_device.hpp>
#include <graphics/device/graphics_visitor.hpp>

namespace moka
{
    class fill_index_buffer_command final : public graphics_command
    {
    public:
        index_buffer handle;
        const void* data;
        size_t size;

        virtual ~fill_index_buffer_command();

        void accept(graphics_visitor& visitor) override;

        fill_index_buffer_command& set_buffer(index_buffer handle, const void* data, size_t size);
    };
} // namespace moka
