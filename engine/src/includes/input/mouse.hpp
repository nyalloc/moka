#pragma once

#include <glm/vec2.hpp>

namespace moka
{
	enum class mouse_button : uint8_t
	{
		left = 0,
		right,
		middle
	};

	class mouse_state
	{
		friend class application;

		glm::ivec2 position_;
		glm::ivec2 motion_;
		std::array<bool, 3> buttons_ = {};
		glm::ivec2 scroll_;
	public:

		const glm::ivec2& get_position() const
		{
			return position_;
		}

		const glm::ivec2& get_motion() const
		{
			return motion_;
		}

		bool is_button_down(const mouse_button button) const
		{
			const auto pos = static_cast<size_t>(button);
			return buttons_[pos];
		}

		bool is_button_up(const mouse_button button) const
		{
			return !is_button_down(button);
		}

		glm::ivec2 get_scroll() const
		{
			return scroll_;
		}
	};

	class mouse
	{
		friend class application;

		mouse_state state_;
	public:
		mouse() = default;
		mouse(const mouse& rhs) = delete;
		mouse(mouse&& rhs) = delete;
		mouse& operator = (const mouse& rhs) = delete;
		mouse& operator = (mouse&& rhs) = delete;
		~mouse() = default;

		const mouse_state& get_state() const
		{
			return state_;
		}
	};
}