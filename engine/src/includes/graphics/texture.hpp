#pragma once

#include <cstdint>

namespace moka
{
    class graphics_device;

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
    struct wrap final
    {
        wrap_mode s = wrap_mode::clamp_to_edge;
        wrap_mode t = wrap_mode::clamp_to_edge;
        wrap_mode r = wrap_mode::clamp_to_edge;
    };

    /**
     * \brief The texture magnification function is used whenever the
     * level-of-detail function used when sampling from the texture determines
     * that the texture should be magnified. It sets the texture magnification
     * function to either nearest or linear. nearest is generally faster than
     * linear, but it can produce textured images with sharper edges because
     * the transition between texture elements is not as smooth.
     */
    enum class mag_filter : uint8_t
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
         * depending on the values of wrap::x and
         * wrap::y, and on the exact mapping.
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
    enum class min_filter : uint8_t
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
         * depending on the values of wrap::x and
         * wrap::y, and on the exact mapping.
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
    struct filter final
    {
        mag_filter mag = mag_filter::linear;
        min_filter min = min_filter::linear;
    };

    enum class texture_target : uint8_t
    {
        texture_1d,
        texture_2d,
        texture_3d,
        array_1d,
        array_2d,
        rectangle,
        cubemap,
        cubemap_array,
        texture_buffer,
        texture_2d_multisample,
        texture_2d_multisample_array
    };

    enum class image_target : uint8_t
    {
        texture_2d,
        texture_2d_proxy,
        array_1d,
        array_1d_proxy,
        rectangle,
        rectangle_proxy,
        cubemap_positive_x,
        cubemap_positive_y,
        cubemap_positive_z,
        cubemap_negative_x,
        cubemap_negative_y,
        cubemap_negative_z,
        cubemap_proxy
    };

    enum class host_format : uint8_t
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
    enum class device_format : uint8_t
    {
        r,
        rg,
        rgb,
        rgb16f,
        bgr,
        rgba,
        bgra,
    };

    enum class pixel_type : uint8_t
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

    struct texture final
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

    struct image_metadata final
    {
        image_target target = image_target::texture_2d;

        int mip_level = 0;

        pixel_type type = pixel_type::uint8;

        device_format internal_format = device_format::rgba;

        int width = 0;

        int height = 0;

        int border = 0;

        host_format base_format = host_format::auto_detect;
    };

    struct host_image_data final
    {
        image_metadata metadata; // description of pixel data
        void* data = nullptr;    // pointer to pixel data buffer
    };

    struct texture_metadata final
    {
        texture_target target = texture_target::texture_2d;

        std::vector<image_metadata> data;

        wrap wrap = {wrap_mode::clamp_to_edge, wrap_mode::clamp_to_edge, wrap_mode::clamp_to_edge};

        filter filter_mode = {mag_filter::linear, min_filter::linear};

        bool generate_mipmaps = false;
    };

    class texture_builder final
    {
    public:
        explicit texture_builder(graphics_device& device);

        texture_builder& set_target(texture_target target);

        texture_builder& set_wrap_s(wrap_mode mode);

        texture_builder& set_wrap_t(wrap_mode mode);

        texture_builder& set_wrap_r(wrap_mode mode);

        texture_builder& set_mag_filter(mag_filter mag);

        texture_builder& set_min_filter(min_filter min);

        texture_builder& free_host_data(bool free_data);

        texture_builder& set_mipmaps(bool generate_mipmaps);

        texture build();

        texture_builder& add_image_data(
            image_target target,
            int mip_level,
            device_format internal_format,
            int width,
            int height,
            int border,
            host_format base_format,
            pixel_type type,
            void* data);

    private:
        bool free_host_data_ = false;

        std::vector<void*> host_data_;

        texture_metadata metadata_;

        graphics_device& device_;
    };
} // namespace moka
