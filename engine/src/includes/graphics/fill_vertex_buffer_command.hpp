#pragma once

#include "graphics_command.hpp"
#include "graphics_visitor.hpp"
#include "vertex_buffer.hpp"
#include "vertex_layout.hpp"

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