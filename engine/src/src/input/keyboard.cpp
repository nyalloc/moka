#include <input/keyboard.hpp>

namespace moka
{
    void keyboard_state::set_key_down(const key key)
    {
        keys_[static_cast<int>(key)] = true;
    }

    void keyboard_state::set_key_up(const key key)
    {
        keys_[static_cast<int>(key)] = false;
    }

    bool keyboard_state::is_key_down(const key key) const
    {
        return keys_[static_cast<int>(key)];
    }

    bool keyboard_state::is_key_up(const key key) const
    {
        return !is_key_down(key);
    }

    const keyboard_state& keyboard::get_state() const
    {
        return state_;
    }
} // namespace moka
