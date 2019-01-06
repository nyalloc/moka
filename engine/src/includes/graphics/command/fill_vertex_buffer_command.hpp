#pragma once

#include <graphics/command/graphics_command.hpp>
#include <graphics/device/graphics_visitor.hpp>
#include <graphics/buffer/vertex_buffer.hpp>
#include <graphics/buffer/vertex_layout.hpp>

namespace moka
{
	class fill_vertex_buffer_command : public graphics_command
	{
	public:
        vertex_buffer handle;
        const void* data;
        size_t size;

        virtual ~fill_vertex_buffer_command() = default;
        
		void accept(graphics_visitor& visitor) override
		{
			visitor.visit(*this);
		}

		fill_vertex_buffer_command& set_buffer(vertex_buffer handle, const void* data, size_t size)
		{
			this->handle = handle;
            this->data = data;
            this->size = size;
			return *this;
		}
	};
}