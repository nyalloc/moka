#pragma once

#include <array>
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
        const glm::ivec2& get_position() const;

        const glm::ivec2& get_motion() const;

        bool is_button_down(mouse_button button) const;

        bool is_button_up(mouse_button button) const;

        glm::ivec2 get_scroll() const;
    };

    class mouse
    {
        friend class application;

        mouse_state state_;

    public:
        mouse() = default;
        mouse(const mouse& rhs) = delete;
        mouse(mouse&& rhs) = delete;
        mouse& operator=(const mouse& rhs) = delete;
        mouse& operator=(mouse&& rhs) = delete;
        ~mouse() = default;

        const mouse_state& get_state() const;
    };
} // namespace moka
