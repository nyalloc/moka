#pragma once

#include <graphics/command/graphics_command.hpp>
#include <graphics/device/graphics_visitor.hpp>

namespace moka
{
	class scissor_command : public graphics_command
	{
	public:
        int width = 0;
        int height = 0;
        int x = 0;
        int y = 0;

        virtual ~scissor_command() = default;
        
		void accept(graphics_visitor& visitor) override
		{
			visitor.visit(*this);
		}

		scissor_command& set_rectangle(int x, int y, int width, int height)
		{
			this->width = width;
            this->height = height;
            this->x = x;
            this->y = y;
			return *this;
		}

		scissor_command& set_rectangle(const rectangle& rectangle)
		{
			this->width = rectangle.width;
			this->height = rectangle.height;
			this->x = rectangle.x;
			this->y = rectangle.y;
			return *this;
		}
	};
}