#pragma once

#include "graphics_command.hpp"
#include "graphics_visitor.hpp"

namespace moka
{
	class clear_command : public graphics_command
	{
	public:
		glm::vec4 color;
		bool clear_depth = false;
		bool clear_color = false;

        virtual ~clear_command() = default;
        
		void accept(graphics_visitor& visitor) override
		{
			visitor.visit(*this);
		}

		clear_command& set_color(const float r, const float g, const float b, const float a)
		{
			color = { r, g, b, a };
			return *this;
		}

		clear_command& set_clear_color(const bool val)
		{
			clear_color = val;
			return *this;
		}

		clear_command& set_clear_depth(const bool val)
		{
			clear_depth = val;
			return *this;
		}
	};
}