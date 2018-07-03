#pragma once

#include <maths/vector4.hpp>
#include <maths/vector3.hpp>

namespace moka
{
    using byte = uint8_t;

    namespace detail
    {
        template<typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
        constexpr byte float_to_byte(const T val) noexcept
        {
            return static_cast<byte>(moka::clamp(val, static_cast<T>(0.0f), static_cast<T>(1.0f)) * std::numeric_limits<byte>::max());
        }

        template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
        constexpr byte int_to_byte(const T val) noexcept
        {
            return static_cast<byte>(moka::clamp(val, static_cast<T>(0), static_cast<T>(255)));
        }

        constexpr float byte_to_float(const byte val) noexcept
        {
            return static_cast<float>(val) / std::numeric_limits<byte>::max();
        }
    }

    class colour
    {
        detail::vector4<byte> rgba_bytes_;
    public:
        constexpr colour(const byte r, const byte g, const byte b, const byte a = 255) noexcept
            : rgba_bytes_{ r, g, b, a }
        {}

        constexpr colour() noexcept
            : colour{ detail::int_to_byte(255), detail::int_to_byte(255), detail::int_to_byte(255), detail::int_to_byte(255) }
        {}

        constexpr colour(const vector3& colour, const float alpha = 1.0f) noexcept
            : colour{ detail::float_to_byte(colour.x()), detail::float_to_byte(colour.y()), detail::float_to_byte(colour.z()), detail::float_to_byte(alpha) }
        {}

        constexpr colour(const vector4& colour) noexcept
            : colour{ detail::float_to_byte(colour.x()), detail::float_to_byte(colour.y()), detail::float_to_byte(colour.z()), detail::float_to_byte(colour.w()) }
        {}

        constexpr colour(const int r, const int g, const int b, const int a = 255) noexcept
            : colour{ detail::int_to_byte(r), detail::int_to_byte(g), detail::int_to_byte(b), detail::int_to_byte(a) }
        {}

        constexpr colour(const float r, const float g, const float b, const float a = 1.0f) noexcept
            : colour{ detail::float_to_byte(r), detail::float_to_byte(g), detail::float_to_byte(b), detail::float_to_byte(a) }
        {}

        constexpr void r(const int val) noexcept
        {
            rgba_bytes_[0] = detail::int_to_byte(val);
        }

        constexpr void r(const float val) noexcept
        {
            rgba_bytes_[0] = detail::float_to_byte(val);
        }

        constexpr void g(const int val) noexcept
        {
            rgba_bytes_[1] = detail::int_to_byte(val);
        }

        constexpr void g(const float val) noexcept
        {
            rgba_bytes_[1] = detail::float_to_byte(val);
        }

        constexpr void b(const int val) noexcept
        {
            rgba_bytes_[2] = detail::int_to_byte(val);
        }

        constexpr void b(const float val) noexcept
        {
            rgba_bytes_[2] = detail::float_to_byte(val);
        }

        constexpr void a(const int val) noexcept
        {
            rgba_bytes_[3] = detail::int_to_byte(val);
        }

        constexpr void a(const float val) noexcept
        {
            rgba_bytes_[3] = detail::float_to_byte(val);
        }

        constexpr float r() const noexcept
        {
            return detail::byte_to_float(rgba_bytes_[0]);
        }

        constexpr float g() const noexcept
        {
            return detail::byte_to_float(rgba_bytes_[1]);
        }

        constexpr float b() const noexcept
        {
            return detail::byte_to_float(rgba_bytes_[2]);
        }

        constexpr float a() const noexcept
        {
            return detail::byte_to_float(rgba_bytes_[3]);
        }

        constexpr vector4 to_vector4() const noexcept
        {
            return { r(), g(), b(), a() };
        }

        /**
         * \brief Create an Alice Blue colour.
         * \return R: 240, G: 248, B: 255.
         */
        constexpr static colour alice_blue() noexcept
        {
            return { 240, 248, 255 };
        }

        constexpr static colour antique_white() noexcept
        {
            return { 250, 235, 215 };
        }

        constexpr static colour aqua() noexcept
        {
            return { 0, 255, 255 };
        }

        constexpr static colour aquamarine() noexcept
        {
            return { 127, 255, 212 };
        }

        constexpr static colour azure() noexcept
        {
            return { 240, 255, 255 };
        }

        constexpr static colour beige() noexcept
        {
            return { 245, 245, 220 };
        }

        constexpr static colour bisque() noexcept
        {
            return { 255, 228, 196 };
        }

        constexpr static colour black() noexcept
        {
            return { 0, 0, 0 };
        }

        constexpr static colour blanched_almond() noexcept
        {
            return { 255, 235, 205 };
        }

        constexpr static colour blue() noexcept
        {
            return { 0, 0, 255 };
        }

        constexpr static colour blue_violet() noexcept
        {
            return { 138, 43, 226 };
        }

        constexpr static colour brown() noexcept
        {
            return { 165, 42, 42 };
        }
        
        constexpr static colour burly_wood() noexcept
        {
            return { 222, 184, 135 };
        }

        constexpr static colour cadet_blue() noexcept
        {
            return { 95, 158, 160 };
        }

        constexpr static colour chartreuse() noexcept
        {
            return { 127, 255, 0 };
        }

        constexpr static colour chocolate() noexcept
        {
            return { 210, 105, 30 };
        }

        constexpr static colour coral() noexcept
        {
            return { 255, 127, 80 };
        }

        constexpr static colour cornflower_blue() noexcept
        {
            return { 100, 149, 237 };
        }

        constexpr static colour cornsilk() noexcept
        {
            return { 255, 248, 220 };
        }

        constexpr static colour crimson() noexcept
        {
            return { 220, 20, 60 };
        }

        constexpr static colour cyan() noexcept
        {
            return { 0, 255, 255 };
        }

        constexpr static colour dark_blue() noexcept
        {
            return { 0, 0, 139 };
        }

        constexpr static colour dark_cyan() noexcept
        {
            return { 0, 139, 139 };
        }

        constexpr static colour dark_goldenrod() noexcept
        {
            return { 184, 134, 11 };
        }

        constexpr static colour dark_gray() noexcept
        {
            return { 169, 169, 169 };
        }

        constexpr static colour dark_grey() noexcept
        {
            return dark_gray();
        }

        constexpr static colour dark_green() noexcept
        {
            return { 0, 100, 0 };
        }

        constexpr static colour dark_khaki() noexcept
        {
            return { 189, 183, 107 };
        }

        constexpr static colour dark_magenta() noexcept
        {
            return { 139, 0, 139 };
        }

        constexpr static colour dark_olive_green() noexcept
        {
            return { 85, 107, 47 };
        }

        constexpr static colour dark_orange() noexcept
        {
            return { 255, 140, 0 };
        }

        constexpr static colour dark_orchid() noexcept
        {
            return { 153, 50, 204 };
        }

        constexpr static colour dark_red() noexcept
        {
            return { 139, 0, 0 };
        }

        //    dark_salmon 233 150 122
        //    dark_sea_green 143 188 142
        //    dark_slate_blue 72 61 139
        //    dark_slate_gray 47 79 79
        //    dark_slate_grey 47 79 79
        //    dark_turquoise 0 206 209
        //    dark_violet 148 0 211
        //    deep_pink 255 20 147
        //    deep_sky_blue 0 191 255
        //    dim_gray 105 105 105
        //    dim_grey 105 105 105
        //    dodger_blue 30 144 255
        //    firebrick 178 34 34
        //    floral_white 255 250 240
        //    forest_green 34 139 34
        //    fuchsia 255 0 255
        //    gainsboro 220 220 220
        //    ghost_white 248 248 248
        //    gold 255 215 0
        //    goldenrod 218 165 32
        //    gray 128 128 128
        //    green 0 128 0
        //    green_yellow 173 255 47
        //    grey 128 128 128
        //    honeydew 240 255 240
        //    hot_pink 255 105 180
        //    indian_red 205 92 92
        //    indigo 75 0 130
        //    ivory 255 255 240
        //    khaki 240 230 140
        //    lavender 230 230 250
        //    lavender_blush 255 240 245
        //    lawn_green 124 252 0
        //    lemon_chiffon 255 250 205
        //    light_blue 173 216 230
        //    light_coral 240 128 128
        //    light_cyan 224 255 255
        //    light_goldenrod_yellow 250 250 210
        //    light_gray 211 211 211
        //    light_green 144 238 144
        //    light_grey 211 211 211
        //    light_pink 255 182 193
        //    light_salmon 255 160 122
        //    light_sea_green 32 178 170
        //    light_sky_blue 135 206 250
        //    light_slate_gray 119 136 153
        //    light_slate_grey 119 136 153
        //    light_steel_blue 176 196 222
        //    light_yellow 255 255 224
        //    lime 0 255 0
        //    lime_green 50 205 50
        //    linen 250 240 230
        //    magenta 255 0 255
        //    maroon 128 0 0
        //    medium_aquamarine 102 205 170
        //    medium_blue 0 0 205
        //    medium_orchid 186 85 211
        //    medium_purple 147 112 219
        //    medium_sea_green 60 179 113
        //    medium_slate_blue 123 104 238
        //    medium_spring_green 0 250 154
        //    medium_turquoise 72 209 204
        //    medium_violet_red 199 21 133
        //    midnight_blue 25 25 112
        //    mint_cream 245 255 250
        //    misty_rose 255 228 225
        //    moccasin 255 228 181
        //    navajo_white 255 222 173
        //    navy 0 0 128
        //    old_lace 253 245 230
        //    olive 128 128 0
        //    olive_drab 107 142 35
        //    orange 255 69 0
        //    orange_red 255 69 0
        //    orchid 218 112 214
        //    pale_goldenrod 238 232 170
        //    pale_green 152 251 152
        //    pale_turquoise 175 238 238
        //    pale_violet_red 219 112 147
        //    papaya_whip 255 239 213
        //    peach_puff 255 218 185
        //    peru 205 133 63
        //    pink 255 192 203
        //    plum 221 160 221
        //    powder_blue 176 224 230
        //    purple 128 0 128
        //    red 255 0 0
        //    rosy_brown 188 143 143
        //    royal_blue 65 105 225
        //    saddle_brown 139 69 19
        //    salmon 250 128 114
        //    sandy_brown 244 164 96
        //    sea_green 46 139 87
        //    sea_shell 255 245 238
        //    sienna 160 82 45
        //    silver 192 192 192
        //    sky_blue 135 206 235
        //    slate_blue 106 90 205
        //    slate_gray 112 128 144
        //    slate_grey 112 128 144
        //    snow 255 250 250
        //    spring_green 0 255 127
        //    steel_blue 70 130 180
        //    tan 210 180 140
        //    teal 0 128 128
        //    thistle 216 191 216
        //    tomato 255 99 71
        //    transparent_black 0 0 0
        //        turquoise 64 244 208
        //        violet 238 130 238
        //        wheat 245 222 179
        //        white 255 255 255
        //        white_smoke 245 245 245
        //        yellow 255 255 0
        //        yellow_green 154 205 50
    };

    using color = colour;
}