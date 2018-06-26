#pragma once

#include <cstdint>
#include <maths/vector2.hpp>

namespace neon
{
    struct window_handle
    {
        uint16_t idx;
    };

    inline bool is_valid(const window_handle handle)
    {
        return UINT16_MAX != handle.idx;
    }

    struct gamepad_handle
    {
        uint16_t idx;
    };

    inline bool is_valid(const gamepad_handle handle)
    {
        return UINT16_MAX != handle.idx;
    }

    enum class mouse_button
    {
        none,
        left,
        middle,
        right,
        count
    };

    enum class gamepad_axis
    {
        left_x,
        left_y,
        left_z,
        right_x,
        right_y,
        right_z,
        count
    };

    enum class keyboard
    {
        none,
        esc,
        enter,
        tab,
        space,
        backspace,
        up,
        down,
        left,
        right,
        insert,
        del,
        home,
        end,
        page_up,
        page_down,
        print,
        plus,
        minus,
        left_bracket,
        right_bracket,
        semicolon,
        quote,
        comma,
        period,
        slash,
        backslash,
        tilde,
        f1,
        f2,
        f3,
        f4,
        f5,
        f6,
        f7,
        f8,
        f9,
        f10,
        f11,
        f12,
        num_pad0,
        num_pad1,
        num_pad2,
        num_pad3,
        num_pad4,
        num_pad5,
        num_pad6,
        num_pad7,
        num_pad8,
        num_pad9,
        key_0,
        key_1,
        key_2,
        key_3,
        key_4,
        key_5,
        key_6,
        key_7,
        key_8,
        key_9,
        key_a,
        key_b,
        key_c,
        key_d,
        key_e,
        key_f,
        key_g,
        key_h,
        key_i,
        key_j,
        key_k,
        key_l,
        key_m,
        key_n,
        key_o,
        key_p,
        key_q,
        key_r,
        key_s,
        key_t,
        key_u,
        key_v,
        key_w,
        key_x,
        key_y,
        key_z,
        left_alt,
        right_alt,
        left_ctrl,
        right_ctrl,
        left_shift,
        right_shift,
        left_meta,
        right_meta
    };

    enum class gamepad_button
    {
        gamepad_a,
        gamepad_b,
        gamepad_x,
        gamepad_y,
        gamepad_thumb_left,
        gamepad_thumb_right,
        gamepad_shoulder_left,
        gamepad_shoulder_right,
        gamepad_up,
        gamepad_down,
        gamepad_left,
        gamepad_right,
        gamepad_back,
        gamepad_start,
        gamepad_guide,
        count
    };

    struct mouse_state
    {
        vector2_int positions;
        std::array<mouse_button, static_cast<size_t>(mouse_button::count)> keys;

        mouse_state()
            : positions(0)
        {}
    };

    struct GamepadState
    {
        GamepadState()
        {}
    };
}
