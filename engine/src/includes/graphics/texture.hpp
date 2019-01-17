#pragma once

#include <cstdint>

namespace moka
{
    /**
     * \brief
     */
    enum class wrap_mode : uint8_t
    {
        clamp_to_edge,
        mirrored_repeat,
        repeat
    };

    /**
     * \brief x and y texture wrap modes. Both default to clamp_to_edge.
     */
    struct texture_wrap_mode final
    {
        wrap_mode x = wrap_mode::clamp_to_edge;
        wrap_mode y = wrap_mode::clamp_to_edge;
    };

    /**
     * \brief The texture magnification function is used whenever the
     * level-of-detail function used when sampling from the texture determines
     * that the texture should be magnified. It sets the texture magnification
     * function to either nearest or linear. nearest is generally faster than
     * linear, but it can produce textured images with sharper edges because
     * the transition between texture elements is not as smooth.
     */
    enum class mag_filter_mode : uint8_t
    {
        /**
         * \brief Returns the value of the texture element that is nearest
         * (in Manhattan distance) to the specified texture coordinates.
         */
        nearest,
        /**
         * \brief Returns the weighted average of the four texture elements
         * that are closest to the specified texture coordinates. These can
         * include items wrapped or repeated from other parts of a texture,
         * depending on the values of texture_wrap_mode::x and
         * texture_wrap_mode::y, and on the exact mapping.
         */
        linear
    };

    /**
     * \brief The texture minifying function is used whenever the level-of-detail
     * function used when sampling from the texture determines that the texture
     * should be minified. There are six defined minifying functions. Two of them
     * use either the nearest texture elements or a weighted average of multiple
     * texture elements to compute the texture value. The other four use mipmaps.
     */
    enum class min_filter_mode : uint8_t
    {
        /**
         * \brief Returns the value of the texture element that is nearest
         * (in Manhattan distance) to the specified texture coordinates.
         */
        nearest,
        /**
         * \brief Returns the weighted average of the four texture elements
         * that are closest to the specified texture coordinates. These can
         * include items wrapped or repeated from other parts of a texture,
         * depending on the values of texture_wrap_mode::x and
         * texture_wrap_mode::y, and on the exact mapping.
         */
        linear,
        /**
         * \brief Chooses the mipmap that most closely matches the size of
         * the pixel being textured and uses the nearest criterion (the
         * texture element closest to the specified texture coordinates)
         * to produce a texture value.
         */
        nearest_mipmap_nearest,
        /**
         * \brief Chooses the mipmap that most closely matches the size of
         * the pixel being textured and uses the linear criterion (a
         * weighted average of the four texture elements that are closest
         * to the specified texture coordinates) to produce a texture value.
         */
        linear_mipmap_nearest,
        /**
         * \brief Chooses the two mipmaps that most closely match the size of
         * the pixel being textured and uses the nearest criterion (the
         * texture element closest to the specified texture coordinates)
         * to produce a texture value from each mipmap. The final texture
         * value is a weighted average of those two values.
         */
        nearest_mipmap_linear,
        /**
         * \brief Chooses the two mipmaps that most closely match the size of
         * the pixel being textured and uses the linear criterion (a weighted
         * average of the texture elements that are closest to the specified
         * texture coordinates) to produce a texture value from each mipmap.
         * The final texture value is a weighted average of those two values.
         */
        linear_mipmap_linear
    };

    /**
     * \brief Min and max texture filter modes. Both default to linear filtering.
     */
    struct texture_filter_mode
    {
        mag_filter_mode mag = mag_filter_mode::linear;
        min_filter_mode min = min_filter_mode::linear;
    };

    enum class texture_target : uint8_t
    {
        texture_2d,
        texture_2d_proxy,
        array_1d,
        array_1d_proxy,
        rectangle,
        rectangle_proxy,
        cubemap,
        cubemap_positive_x,
        cubemap_positive_y,
        cubemap_positive_z,
        cubemap_negative_x,
        cubemap_negative_y,
        cubemap_negative_z
    };

    enum class base_pixel_format : uint8_t
    {
        r,
        rg,
        rgb,
        bgr,
        rgba,
        bgra,
        auto_detect
    };

    // will add to this as it becomes necessary
    enum class internal_pixel_format : uint8_t
    {
        r,
        rg,
        rgb,
        rgb16f,
        bgr,
        rgba,
        bgra,
        auto_detect
    };

    enum class texture_type : uint8_t
    {
        int8,    // byte
        int16,   // short
        int32,   // int
        uint8,   // unsigned byte
        uint16,  // unsigned short
        uint32,  // unsigned int
        float16, // half float
        float32  // float
    };

    struct texture
    {
        uint16_t id;

        bool operator==(const texture& rhs) const
        {
            return id == rhs.id;
        }

        bool operator!=(const texture& rhs) const
        {
            return id != rhs.id;
        }

        bool operator>(const texture& rhs) const
        {
            return id > rhs.id;
        }

        bool operator<(const texture& rhs) const
        {
            return id < rhs.id;
        }

        bool operator>=(const texture& rhs) const
        {
            return id >= rhs.id;
        }

        bool operator<=(const texture& rhs) const
        {
            return id <= rhs.id;
        }
    };
} // namespace moka
