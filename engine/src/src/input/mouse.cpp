#include <input/mouse.hpp>

namespace moka
{
    const glm::ivec2& mouse_state::get_position() const
    {
        return position_;
    }

    const glm::ivec2& mouse_state::get_motion() const
    {
        return motion_;
    }

    bool mouse_state::is_button_down(const mouse_button button) const
    {
        const auto pos = static_cast<size_t>(button);
        return buttons_[pos];
    }

    bool mouse_state::is_button_up(const mouse_button button) const
    {
        return !is_button_down(button);
    }

    glm::ivec2 mouse_state::get_scroll() const
    {
        return scroll_;
    }

    const mouse_state& mouse::get_state() const
    {
        return state_;
    }
} // namespace moka
