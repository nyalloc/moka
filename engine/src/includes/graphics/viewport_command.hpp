#pragma once

#include "graphics_command.hpp"
#include "graphics_visitor.hpp"

namespace moka
{
	class viewport_command : public graphics_command
	{
	public:
        int width = 0;
        int height = 0;
        int x = 0;
        int y = 0;

		const char* name = "viewport_command";

        virtual ~viewport_command() = default;
        
		void accept(graphics_visitor& visitor) override
		{
			visitor.visit(*this);
		}

		viewport_command& set_rectangle(int x, int y, int width, int height)
		{
			this->width = width;
            this->height = height;
            this->x = x;
            this->y = y;
			return *this;
		}
	};
}