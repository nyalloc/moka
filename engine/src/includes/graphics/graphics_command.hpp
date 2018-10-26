#pragma once

#include <memory>

namespace moka
{
	class graphics_visitor;

	class graphics_command
	{
	public:
		virtual void accept(graphics_visitor& visitor) = 0;

		virtual ~graphics_command() = default;
	};

	using graphics_command_ptr = std::unique_ptr<graphics_command>;
}