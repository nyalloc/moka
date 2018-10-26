#pragma once

#include "graphics_command.hpp"
#include "graphics_visitor.hpp"
#include "index_buffer.hpp"

namespace moka
{
	class fill_index_buffer_command : public graphics_command
	{
	public:
        index_buffer handle;
        const void* data;
        size_t size;
        
        virtual ~fill_index_buffer_command() = default;

		void accept(graphics_visitor& visitor) override
		{
			visitor.visit(*this);
		}

		fill_index_buffer_command& set_buffer(index_buffer handle, const void* data, size_t size)
		{
			this->handle = handle;
            this->data = data;
            this->size = size;
			return *this;
		}
	};
}