#pragma once

#include <array>
#include <glm/glm.hpp>

namespace moka
{
    namespace detail
    {
        template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
        constexpr std::byte float_to_byte(const T val) noexcept
        {
            return static_cast<std::byte>(
                static_cast<int>(glm::clamp(val, static_cast<T>(0.0f), static_cast<T>(1.0f)) * 255));
        }

        template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
        constexpr std::byte int_to_byte(const T val) noexcept
        {
            return static_cast<std::byte>(glm::clamp(val, static_cast<T>(0), static_cast<T>(255)));
        }

        constexpr float byte_to_float(const std::byte val) noexcept
        {
            return static_cast<float>(val) / 255;
        }
    } // namespace detail

    /**
     * \brief A colour class, a wrapper around four bytes.
     */
    class colour final
    {
        std::array<std::byte, 4> rgba_bytes_{};

    public:
        operator glm::vec4() const noexcept
        {
            return glm::vec4{r(), g(), b(), a()};
        }

        operator glm::vec3() const noexcept
        {
            return glm::vec3{r(), g(), b()};
        }

        constexpr colour(const std::byte r, const std::byte g, const std::byte b, const std::byte a = static_cast<std::byte>(255)) noexcept
            : rgba_bytes_{r, g, b, a}
        {
        }

        constexpr colour(const int r, const int g, const int b, const int a = 255) noexcept
            : colour{detail::int_to_byte(r), detail::int_to_byte(g), detail::int_to_byte(b), detail::int_to_byte(a)}
        {
        }

        constexpr colour(const float r, const float g, const float b, const float a = 1.0f) noexcept
            : colour{detail::float_to_byte(r), detail::float_to_byte(g), detail::float_to_byte(b), detail::float_to_byte(a)}
        {
        }

        constexpr colour() noexcept
            : colour{detail::int_to_byte(255), detail::int_to_byte(255), detail::int_to_byte(255), detail::int_to_byte(255)}
        {
        }

        constexpr colour(const glm::vec3& colour) noexcept
            : colour{detail::float_to_byte(colour.x),
                     detail::float_to_byte(colour.y),
                     detail::float_to_byte(colour.z)}
        {
        }

        constexpr colour(const glm::vec4& colour) noexcept
            : colour{detail::float_to_byte(colour.x),
                     detail::float_to_byte(colour.y),
                     detail::float_to_byte(colour.z),
                     detail::float_to_byte(colour.w)}
        {
        }

        constexpr float operator[](const size_t pos) const noexcept
        {
            return detail::byte_to_float(rgba_bytes_[pos]);
        }

        constexpr void set_r(const int val) noexcept
        {
            rgba_bytes_[0] = detail::int_to_byte(val);
        }

        constexpr void set_r(const float val) noexcept
        {
            rgba_bytes_[0] = detail::float_to_byte(val);
        }

        constexpr void set_g(const int val) noexcept
        {
            rgba_bytes_[1] = detail::int_to_byte(val);
        }

        constexpr void set_g(const float val) noexcept
        {
            rgba_bytes_[1] = detail::float_to_byte(val);
        }

        constexpr void set_b(const int val) noexcept
        {
            rgba_bytes_[2] = detail::int_to_byte(val);
        }

        constexpr void set_b(const float val) noexcept
        {
            rgba_bytes_[2] = detail::float_to_byte(val);
        }

        constexpr void set_a(const int val) noexcept
        {
            rgba_bytes_[3] = detail::int_to_byte(val);
        }

        constexpr void set_a(const float val) noexcept
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

        static constexpr colour alice_blue() noexcept
        {
            return {240, 248, 255};
        }

        static constexpr colour antique_white() noexcept
        {
            return {250, 235, 215};
        }

        static constexpr colour aqua() noexcept
        {
            return {0, 255, 255};
        }

        static constexpr colour aquamarine() noexcept
        {
            return {127, 255, 212};
        }

        static constexpr colour azure() noexcept
        {
            return {240, 255, 255};
        }

        static constexpr colour beige() noexcept
        {
            return {245, 245, 220};
        }

        static constexpr colour bisque() noexcept
        {
            return {255, 228, 196};
        }

        static constexpr colour black() noexcept
        {
            return {0, 0, 0};
        }

        static constexpr colour blanched_almond() noexcept
        {
            return {255, 235, 205};
        }

        static constexpr colour blue() noexcept
        {
            return {0, 0, 255};
        }

        static constexpr colour blue_violet() noexcept
        {
            return {138, 43, 226};
        }

        static constexpr colour brown() noexcept
        {
            return {165, 42, 42};
        }

        static constexpr colour burly_wood() noexcept
        {
            return {222, 184, 135};
        }

        static constexpr colour burnt_sienna() noexcept
        {
            return {233, 116, 81};
        }

        static constexpr colour cadet_blue() noexcept
        {
            return {95, 158, 160};
        }

        static constexpr colour chartreuse() noexcept
        {
            return {127, 255, 0};
        }

        static constexpr colour chocolate() noexcept
        {
            return {210, 105, 30};
        }

        static constexpr colour coral() noexcept
        {
            return {255, 127, 80};
        }

        static constexpr colour cornflower_blue() noexcept
        {
            return {100, 149, 237};
        }

        static constexpr colour cornsilk() noexcept
        {
            return {255, 248, 220};
        }

        static constexpr colour crimson() noexcept
        {
            return {220, 20, 60};
        }

        static constexpr colour cyan() noexcept
        {
            return {0, 255, 255};
        }

        static constexpr colour dark_blue() noexcept
        {
            return {0, 0, 139};
        }

        static constexpr colour dark_cyan() noexcept
        {
            return {0, 139, 139};
        }

        static constexpr colour dark_goldenrod() noexcept
        {
            return {184, 134, 11};
        }

        static constexpr colour dark_gray() noexcept
        {
            return {169, 169, 169};
        }

        static constexpr colour nero() noexcept
        {
            return {25, 25, 25};
        }

        static constexpr colour night_rider() noexcept
        {
            return {50, 50, 50};
        }

        static constexpr colour charcoal() noexcept
        {
            return {75, 75, 75};
        }

        static constexpr colour studio() noexcept
        {
            return {134, 78, 160};
        }

        static constexpr colour dark_grey() noexcept
        {
            return dark_gray();
        }

        static constexpr colour dark_green() noexcept
        {
            return {0, 100, 0};
        }

        static constexpr colour dark_khaki() noexcept
        {
            return {189, 183, 107};
        }

        static constexpr colour dark_magenta() noexcept
        {
            return {139, 0, 139};
        }

        static constexpr colour dark_olive_green() noexcept
        {
            return {85, 107, 47};
        }

        static constexpr colour dark_orange() noexcept
        {
            return {255, 140, 0};
        }

        static constexpr colour dark_orchid() noexcept
        {
            return {153, 50, 204};
        }

        static constexpr colour dark_red() noexcept
        {
            return {139, 0, 0};
        }

        static constexpr colour dark_salmon() noexcept
        {
            return {233, 150, 122};
        }

        static constexpr colour dark_sea_green() noexcept
        {
            return {143, 188, 142};
        }

        static constexpr colour dark_slate_blue() noexcept
        {
            return {72, 61, 139};
        }

        static constexpr colour dark_slate_gray() noexcept
        {
            return {47, 79, 79};
        }

        static constexpr colour dark_turquoise() noexcept
        {
            return {0, 206, 209};
        }

        static constexpr colour dark_violet() noexcept
        {
            return {148, 0, 211};
        }

        static constexpr colour deep_pink() noexcept
        {
            return {255, 20, 147};
        }

        static constexpr colour deep_sky_blue() noexcept
        {
            return {0, 191, 255};
        }

        static constexpr colour dim_gray() noexcept
        {
            return {105, 105, 105};
        }

        static constexpr colour dim_grey() noexcept
        {
            return {105, 105, 105};
        }

        static constexpr colour dodger_blue() noexcept
        {
            return {30, 144, 255};
        }

        static constexpr colour firebrick() noexcept
        {
            return {178, 34, 34};
        }

        static constexpr colour floral_white() noexcept
        {
            return {255, 250, 240};
        }

        static constexpr colour forest_green() noexcept
        {
            return {34, 139, 34};
        }

        static constexpr colour fuchsia() noexcept
        {
            return {255, 0, 255};
        }

        static constexpr colour gainsboro() noexcept
        {
            return {220, 220, 220};
        }

        static constexpr colour ghost_white() noexcept
        {
            return {248, 248, 248};
        }

        static constexpr colour gold() noexcept
        {
            return {255, 215, 0};
        }

        static constexpr colour goldenrod() noexcept
        {
            return {218, 165, 32};
        }

        static constexpr colour gray() noexcept
        {
            return {128, 128, 128};
        }

        static constexpr colour green() noexcept
        {
            return {0, 128, 0};
        }

        static constexpr colour green_yellow() noexcept
        {
            return {173, 255, 47};
        }

        static constexpr colour grey() noexcept
        {
            return {128, 128, 128};
        }

        static constexpr colour honeydew() noexcept
        {
            return {240, 255, 240};
        }

        static constexpr colour hot_pink() noexcept
        {
            return {255, 105, 180};
        }

        static constexpr colour indian_red() noexcept
        {
            return {205, 92, 92};
        }

        static constexpr colour indigo() noexcept
        {
            return {75, 0, 130};
        }

        static constexpr colour ivory() noexcept
        {
            return {255, 255, 240};
        }

        static constexpr colour khaki() noexcept
        {
            return {240, 230, 140};
        }

        static constexpr colour lavender() noexcept
        {
            return {230, 230, 250};
        }

        static constexpr colour lavender_blush() noexcept
        {
            return {255, 240, 245};
        }

        static constexpr colour lawn_green() noexcept
        {
            return {124, 252, 0};
        }

        static constexpr colour lemon_chiffon() noexcept
        {
            return {255, 250, 205};
        }

        static constexpr colour light_blue() noexcept
        {
            return {173, 216, 230};
        }

        static constexpr colour light_coral() noexcept
        {
            return {240, 128, 128};
        }

        static constexpr colour light_cyan() noexcept
        {
            return {224, 255, 255};
        }

        static constexpr colour light_goldenrod_yellow() noexcept
        {
            return {250, 250, 210};
        }

        static constexpr colour light_gray() noexcept
        {
            return {211, 211, 211};
        }

        static constexpr colour light_green() noexcept
        {
            return {144, 238, 144};
        }

        static constexpr colour light_grey() noexcept
        {
            return {211, 211, 211};
        }

        static constexpr colour light_pink() noexcept
        {
            return {255, 182, 193};
        }

        static constexpr colour light_salmon() noexcept
        {
            return {255, 160, 122};
        }

        static constexpr colour light_sea_green() noexcept
        {
            return {32, 178, 170};
        }

        static constexpr colour light_sky_blue() noexcept
        {
            return {135, 206, 250};
        }

        static constexpr colour light_slate_gray() noexcept
        {
            return {119, 136, 153};
        }

        static constexpr colour light_slate_grey() noexcept
        {
            return {119, 136, 153};
        }

        static constexpr colour light_steel_blue() noexcept
        {
            return {176, 196, 222};
        }

        static constexpr colour light_yellow() noexcept
        {
            return {255, 255, 224};
        }

        static constexpr colour lime() noexcept
        {
            return {0, 255, 0};
        }

        static constexpr colour lime_green() noexcept
        {
            return {50, 205, 50};
        }

        static constexpr colour linen() noexcept
        {
            return {250, 240, 230};
        }

        static constexpr colour magenta() noexcept
        {
            return {255, 0, 255};
        }

        static constexpr colour maroon() noexcept
        {
            return {128, 0, 0};
        }

        static constexpr colour medium_aquamarine() noexcept
        {
            return {102, 205, 170};
        }

        static constexpr colour medium_blue() noexcept
        {
            return {0, 0, 205};
        }

        static constexpr colour medium_orchid() noexcept
        {
            return {186, 85, 211};
        }

        static constexpr colour medium_purple() noexcept
        {
            return {147, 112, 219};
        }

        static constexpr colour medium_sea_green() noexcept
        {
            return {60, 179, 113};
        }

        static constexpr colour medium_slate_blue() noexcept
        {
            return {123, 104, 238};
        }

        static constexpr colour medium_spring_green() noexcept
        {
            return {0, 250, 154};
        }

        static constexpr colour medium_turquoise() noexcept
        {
            return {72, 209, 204};
        }

        static constexpr colour medium_violet_red() noexcept
        {
            return {199, 21, 133};
        }

        static constexpr colour midnight_blue() noexcept
        {
            return {25, 25, 112};
        }

        static constexpr colour mint_cream() noexcept
        {
            return {245, 255, 250};
        }

        static constexpr colour misty_rose() noexcept
        {
            return {255, 228, 225};
        }

        static constexpr colour moccasin() noexcept
        {
            return {255, 228, 181};
        }

        static constexpr colour navajo_white() noexcept
        {
            return {255, 222, 173};
        }

        static constexpr colour navy() noexcept
        {
            return {0, 0, 128};
        }

        static constexpr colour old_lace() noexcept
        {
            return {253, 245, 230};
        }

        static constexpr colour olive() noexcept
        {
            return {128, 128, 0};
        }

        static constexpr colour olive_drab() noexcept
        {
            return {107, 142, 35};
        }

        static constexpr colour orange() noexcept
        {
            return {255, 69, 0};
        }

        static constexpr colour orange_red() noexcept
        {
            return {255, 69, 0};
        }

        static constexpr colour orchid() noexcept
        {
            return {218, 112, 214};
        }

        static constexpr colour pale_goldenrod() noexcept
        {
            return {238, 232, 170};
        }

        static constexpr colour pale_green() noexcept
        {
            return {152, 251, 152};
        }

        static constexpr colour pale_turquoise() noexcept
        {
            return {175, 238, 238};
        }

        static constexpr colour pale_violet_red() noexcept
        {
            return {219, 112, 147};
        }

        static constexpr colour papaya_whip() noexcept
        {
            return {255, 239, 213};
        }

        static constexpr colour peach_puff() noexcept
        {
            return {255, 218, 185};
        }

        static constexpr colour peru() noexcept
        {
            return {205, 133, 63};
        }

        static constexpr colour pink() noexcept
        {
            return {255, 192, 203};
        }

        static constexpr colour plum() noexcept
        {
            return {221, 160, 221};
        }

        static constexpr colour powder_blue() noexcept
        {
            return {176, 224, 230};
        }

        static constexpr colour purple() noexcept
        {
            return {128, 0, 128};
        }

        static constexpr colour red() noexcept
        {
            return {255, 0, 0};
        }

        static constexpr colour rosy_brown() noexcept
        {
            return {188, 143, 143};
        }

        static constexpr colour royal_blue() noexcept
        {
            return {65, 105, 225};
        }

        static constexpr colour saddle_brown() noexcept
        {
            return {139, 69, 19};
        }

        static constexpr colour salmon() noexcept
        {
            return {250, 128, 114};
        }

        static constexpr colour sandy_brown() noexcept
        {
            return {244, 164, 96};
        }

        static constexpr colour sea_green() noexcept
        {
            return {46, 139, 87};
        }

        static constexpr colour sea_shell() noexcept
        {
            return {255, 245, 238};
        }

        static constexpr colour sienna() noexcept
        {
            return {160, 82, 45};
        }

        static constexpr colour silver() noexcept
        {
            return {192, 192, 192};
        }

        static constexpr colour sky_blue() noexcept
        {
            return {135, 206, 235};
        }

        static constexpr colour slate_blue() noexcept
        {
            return {106, 90, 205};
        }

        static constexpr colour slate_gray() noexcept
        {
            return {112, 128, 144};
        }

        static constexpr colour slate_grey() noexcept
        {
            return {112, 128, 144};
        }

        static constexpr colour snow() noexcept
        {
            return {255, 250, 250};
        }

        static constexpr colour spring_green() noexcept
        {
            return {0, 255, 127};
        }

        static constexpr colour steel_blue() noexcept
        {
            return {70, 130, 180};
        }

        static constexpr colour tan() noexcept
        {
            return {210, 180, 140};
        }

        static constexpr colour teal() noexcept
        {
            return {0, 128, 128};
        }

        static constexpr colour thistle() noexcept
        {
            return {216, 191, 216};
        }

        static constexpr colour tomato() noexcept
        {
            return {255, 99, 71};
        }

        static constexpr colour transparent_black() noexcept
        {
            return {0, 0, 0};
        }

        static constexpr colour turquoise() noexcept
        {
            return {64, 244, 208};
        }

        static constexpr colour violet() noexcept
        {
            return {238, 130, 238};
        }

        static constexpr colour wheat() noexcept
        {
            return {245, 222, 179};
        }

        static constexpr colour white() noexcept
        {
            return {255, 255, 255};
        }

        static constexpr colour white_smoke() noexcept
        {
            return {245, 245, 245};
        }

        static constexpr colour yellow() noexcept
        {
            return {255, 255, 0};
        }

        static constexpr colour yellow_green() noexcept
        {
            return {154, 205, 50};
        }
    };
} // namespace moka
