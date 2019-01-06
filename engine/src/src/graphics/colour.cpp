#pragma once

#include <glm/glm.hpp>
#include <array>

namespace moka
{
    using byte = uint8_t;

    namespace detail
    {
        template<typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
        constexpr byte float_to_byte(const T val) noexcept
        {
            return static_cast<byte>(glm::clamp(val, static_cast<T>(0.0f), static_cast<T>(1.0f)) * std::numeric_limits<byte>::max());
        }

        template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
        constexpr byte int_to_byte(const T val) noexcept
        {
            return static_cast<byte>(glm::clamp(val, static_cast<T>(0), static_cast<T>(255)));
        }

        constexpr float byte_to_float(const byte val) noexcept
        {
            return static_cast<float>(val) / std::numeric_limits<uint8_t>::max();
        }
    }

    class colour
    {
		std::array<byte, 4> rgba_bytes_{};
    public:

		operator glm::vec4() const noexcept
		{
			return glm::vec4{ r(), g(), b(), a() };
		}

		operator glm::vec3() const noexcept
		{
			return glm::vec3{ r(), g(), b() };
		}

        constexpr colour(
			  const byte r
			, const byte g
			, const byte b
			, const byte a = static_cast<byte>(255)) noexcept
            : rgba_bytes_{ r, g, b, a }
        {}

        constexpr colour() noexcept
            : colour{ detail::int_to_byte(255)
			, detail::int_to_byte(255)
			, detail::int_to_byte(255)
			, detail::int_to_byte(255) }
        {}

        constexpr colour(const glm::vec3& colour) noexcept
            : colour{ detail::float_to_byte(colour.x)
			, detail::float_to_byte(colour.y)
			, detail::float_to_byte(colour.z) }
        {}

        constexpr colour(const glm::vec4& colour) noexcept
            : colour{ detail::float_to_byte(colour.x)
			, detail::float_to_byte(colour.y)
			, detail::float_to_byte(colour.z)
			, detail::float_to_byte(colour.w) }
        {}

        constexpr colour(
			const int r, 
			const int g, 
			const int b,
			const int a = 255) noexcept
            : colour{ detail::int_to_byte(r)
			, detail::int_to_byte(g)
			, detail::int_to_byte(b)
			, detail::int_to_byte(a) }
        {}

        constexpr colour(
			const float r,
			const float g, 
			const float b, 
			const float a = 1.0f) noexcept
            : colour{ detail::float_to_byte(r)
			, detail::float_to_byte(g)
			, detail::float_to_byte(b)
			, detail::float_to_byte(a) }
        {}

		constexpr float operator [](const size_t pos) const noexcept
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

		constexpr static colour nero() noexcept
		{
			return { 25, 25, 25 };
		}

		constexpr static colour night_rider() noexcept
		{
			return { 50, 50, 50 };
		}

		constexpr static colour charcoal() noexcept
		{
			return { 75, 75, 75 };
		}

		constexpr static colour studio() noexcept
		{
			return { 134, 78, 160 };
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

		constexpr static colour dark_salmon() noexcept
		{
			return { 233, 150, 122 };
		}

		constexpr static colour dark_sea_green() noexcept
		{
			return { 143, 188, 142 };
		}

		constexpr static colour dark_slate_blue() noexcept
		{
			return { 72, 61, 139 };
		}

		constexpr static colour dark_slate_gray() noexcept
		{
			return { 47, 79, 79 };
		}

		constexpr static colour dark_turquoise() noexcept
		{
			return { 0, 206, 209 };
		}

		constexpr static colour dark_violet() noexcept
		{
			return { 148, 0, 211 };
		}

		constexpr static colour deep_pink() noexcept
		{
			return { 255, 20, 147 };
		}

		constexpr static colour deep_sky_blue() noexcept
		{
			return { 0, 191, 255 };
		}

		constexpr static colour dim_gray() noexcept
		{
			return { 105, 105, 105 };
		}

		constexpr static colour dim_grey() noexcept
		{
			return { 105, 105, 105 };
		}

		constexpr static colour dodger_blue() noexcept
		{
			return { 30, 144, 255 };
		}

		constexpr static colour firebrick() noexcept
		{
			return { 178, 34, 34 };
		}

		constexpr static colour floral_white() noexcept
		{
			return { 255, 250, 240 };
		}

		constexpr static colour forest_green() noexcept
		{
			return { 34, 139, 34 };
		}

		constexpr static colour fuchsia() noexcept
		{
			return { 255, 0, 255 };
		}

		constexpr static colour gainsboro() noexcept
		{
			return { 220, 220, 220 };
		}

		constexpr static colour ghost_white() noexcept
		{
			return { 248, 248, 248 };
		}

		constexpr static colour gold() noexcept
		{
			return { 255, 215, 0 };
		}

		constexpr static colour goldenrod() noexcept
		{
			return { 218, 165, 32 };
		}

		constexpr static colour gray() noexcept
		{
			return { 128, 128, 128 };
		}

		constexpr static colour green() noexcept
		{
			return { 0, 128, 0 };
		}

		constexpr static colour green_yellow() noexcept
		{
			return { 173, 255, 47 };
		}

		constexpr static colour grey() noexcept
		{
			return { 128, 128, 128 };
		}

		constexpr static colour honeydew() noexcept
		{
			return { 240, 255, 240 };
		}

		constexpr static colour hot_pink() noexcept
		{
			return { 255, 105, 180 };
		}

		constexpr static colour indian_red() noexcept
		{
			return { 205, 92, 92 };
		}

		constexpr static colour indigo() noexcept
		{
			return { 75, 0, 130 };
		}

		constexpr static colour ivory() noexcept
		{
			return { 255, 255, 240 };
		}

		constexpr static colour khaki() noexcept
		{
			return { 240, 230, 140 };
		}

		constexpr static colour lavender() noexcept
		{
			return { 230, 230, 250 };
		}

		constexpr static colour lavender_blush() noexcept
		{
			return { 255, 240, 245 };
		}

		constexpr static colour lawn_green() noexcept
		{
			return { 124, 252, 0 };
		}

		constexpr static colour lemon_chiffon() noexcept
		{
			return { 255, 250, 205 };
		}

		constexpr static colour light_blue() noexcept
		{
			return { 173, 216, 230 };
		}

		constexpr static colour light_coral() noexcept
		{
			return { 240, 128, 128 };
		}

		constexpr static colour light_cyan() noexcept
		{
			return { 224, 255, 255 };
		}

		constexpr static colour light_goldenrod_yellow() noexcept
		{
			return { 250, 250, 210 };
		}
		
		constexpr static colour light_gray() noexcept
		{
			return { 211, 211, 211 };
		}

		constexpr static colour light_green() noexcept
		{
			return { 144, 238, 144 };
		}

		constexpr static colour light_grey() noexcept
		{
			return { 211, 211, 211 };
		}

		constexpr static colour light_pink() noexcept
		{
			return { 255, 182, 193 };
		}

		constexpr static colour light_salmon() noexcept
		{
			return { 255, 160, 122 };
		}

		constexpr static colour light_sea_green() noexcept
		{
			return { 32, 178, 170 };
		}

		constexpr static colour light_sky_blue() noexcept
		{
			return { 135, 206, 250 };
		}

		constexpr static colour light_slate_gray() noexcept
		{
			return { 119, 136, 153 };
		}

		constexpr static colour light_slate_grey() noexcept
		{
			return { 119, 136, 153 };
		}
		
		constexpr static colour light_steel_blue() noexcept
		{
			return { 176, 196, 222 };
		}

		constexpr static colour light_yellow() noexcept
		{
			return { 255, 255, 224 };
		}

		constexpr static colour lime() noexcept
		{
			return { 0, 255, 0 };
		}        
		
		constexpr static colour lime_green() noexcept
		{
			return { 50, 205, 50 };
		}
		
		constexpr static colour linen() noexcept
		{
			return { 250, 240, 230 };
		}

		constexpr static colour magenta() noexcept
		{
			return { 255, 0, 255 };
		}
		
		constexpr static colour maroon() noexcept
		{
			return { 128, 0, 0 };
		}

		constexpr static colour medium_aquamarine() noexcept
		{
			return { 102, 205, 170 };
		}

		constexpr static colour medium_blue() noexcept
		{
			return { 0, 0, 205 };
		}

		constexpr static colour medium_orchid() noexcept
		{
			return { 186, 85, 211 };
		}

		constexpr static colour medium_purple() noexcept
		{
			return { 147, 112, 219 };
		}

		constexpr static colour medium_sea_green() noexcept
		{
			return { 60, 179, 113 };
		}
		
		constexpr static colour medium_slate_blue() noexcept
		{
			return { 123, 104, 238 };
		}
		
		constexpr static colour medium_spring_green() noexcept
		{
			return { 0, 250, 154 };
		}
		
		constexpr static colour medium_turquoise() noexcept
		{
			return { 72, 209, 204 };
		}
		
		constexpr static colour medium_violet_red() noexcept
		{
			return { 199, 21, 133 };
		}
		
		constexpr static colour midnight_blue() noexcept
		{
			return { 25, 25, 112 };
		}
		
		constexpr static colour mint_cream() noexcept
		{
			return { 245, 255, 250 };
		}

		constexpr static colour misty_rose() noexcept
		{
			return { 255, 228, 225 };
		}
		
		constexpr static colour moccasin() noexcept
		{
			return { 255, 228, 181 };
		}
		
		constexpr static colour navajo_white() noexcept
		{
			return { 255, 222, 173 };
		}
		
		constexpr static colour navy() noexcept
		{
			return { 0, 0, 128 };
		}
		
		constexpr static colour old_lace() noexcept
		{
			return { 253, 245, 230 };
		}
		
		constexpr static colour olive() noexcept
		{
			return { 128, 128, 0 };
		}
		
		constexpr static colour olive_drab() noexcept
		{
			return { 107, 142, 35 };
		}

		constexpr static colour orange() noexcept
		{
			return { 255, 69, 0 };
		}
		
		constexpr static colour orange_red() noexcept
		{
			return { 255, 69, 0 };
		}
		
		constexpr static colour orchid() noexcept
		{
			return { 218, 112, 214 };
		}

		constexpr static colour pale_goldenrod() noexcept
		{
			return { 238, 232, 170 };
		}       
		
		constexpr static colour pale_green() noexcept
		{
			return { 152, 251, 152 };
		}    
		
		constexpr static colour pale_turquoise() noexcept
		{
			return { 175, 238, 238 };
		}    
		
		constexpr static colour pale_violet_red() noexcept
		{
			return { 219, 112, 147 };
		}

		constexpr static colour papaya_whip() noexcept
		{
			return { 255, 239, 213 };
		}

		constexpr static colour peach_puff() noexcept
		{
			return { 255, 218, 185 };
		}
		
		constexpr static colour peru() noexcept
		{
			return { 205, 133, 63 };
		}
		
		constexpr static colour pink() noexcept
		{
			return { 255, 192, 203 };
		}
		
		constexpr static colour plum() noexcept
		{
			return { 221, 160, 221 };
		}
		
		constexpr static colour powder_blue() noexcept
		{
			return { 176, 224, 230 };
		}
		
		constexpr static colour purple() noexcept
		{
			return { 128, 0, 128 };
		}
		
		constexpr static colour red() noexcept
		{
			return { 255, 0, 0 };
		}

		constexpr static colour rosy_brown() noexcept
		{
			return { 188, 143, 143 };
		}
		
		constexpr static colour royal_blue() noexcept
		{
			return { 65, 105, 225 };
		}
		
		constexpr static colour saddle_brown() noexcept
		{
			return { 139, 69, 19 };
		}
		
		constexpr static colour salmon() noexcept
		{
			return { 250, 128, 114 };
		}
		
		constexpr static colour sandy_brown() noexcept
		{
			return { 244, 164, 96 };
		}
		
		constexpr static colour sea_green() noexcept
		{
			return { 46, 139, 87 };
		}
		
		constexpr static colour sea_shell() noexcept
		{
			return { 255, 245, 238 };
		}
		
		constexpr static colour sienna() noexcept
		{
			return { 160, 82, 45 };
		}
		
		constexpr static colour silver() noexcept
		{
			return { 192, 192, 192 };
		}
		
		constexpr static colour sky_blue() noexcept
		{
			return { 135, 206, 235 };
		}

		constexpr static colour slate_blue() noexcept
		{
			return { 106, 90, 205 };
		}
		
		constexpr static colour slate_gray() noexcept
		{
			return { 112, 128, 144 };
		}
		
		constexpr static colour slate_grey() noexcept
		{
			return { 112, 128, 144 };
		}
		
		constexpr static colour snow() noexcept
		{
			return { 255, 250, 250 };
		}

		constexpr static colour spring_green() noexcept
		{
			return { 0, 255, 127 };
		}
		
		constexpr static colour steel_blue() noexcept
		{
			return { 70, 130, 180 };
		}

		constexpr static colour tan() noexcept
		{
			return { 210, 180, 140 };
		}
		
		constexpr static colour teal() noexcept
		{
			return { 0, 128, 128 };
		}

		constexpr static colour thistle() noexcept
		{
			return { 216, 191, 216 };
		}
		
		constexpr static colour tomato() noexcept
		{
			return { 255, 99, 71 };
		}

		constexpr static colour transparent_black() noexcept
		{
			return { 0, 0, 0 };
		}        
		
		constexpr static colour turquoise() noexcept
		{
			return { 64, 244, 208 };
		} 

		constexpr static colour violet() noexcept
		{
			return { 238, 130, 238 };
		}
		
		constexpr static colour wheat() noexcept
		{
			return { 245, 222, 179 };
		}
		
		constexpr static colour white() noexcept
		{
			return { 255, 255, 255 };
		}
		
		constexpr static colour white_smoke() noexcept
		{
			return { 245, 245, 245 };
		}
		
		constexpr static colour yellow() noexcept
		{
			return { 255, 255, 0 };
		}

		constexpr static colour yellow_green() noexcept
		{
			return { 154, 205, 50 };
		}
    };

    using color = colour;
}