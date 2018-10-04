#pragma once

#include <glm/vec2.hpp>

namespace moka
{
	class mouse_state
	{
		friend class app;

		glm::vec2 position_;
		glm::vec2 motion_;
	public:

		const glm::vec2& get_position() const
		{
			return position_;
		}

		const glm::vec2& get_motion() const
		{
			return motion_;
		}
	};

	class mouse
	{
		friend class app;

		inline static mouse_state state{};
	public:
		static const mouse_state& get_state()
		{
			return state;
		}
	};
}