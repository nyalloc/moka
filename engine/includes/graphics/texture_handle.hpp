/*
===========================================================================
Moka Source Code
Copyright 2019 Stuart Adams. All rights reserved.
https://github.com/stuartdadams/moka
stuartdadams | linkedin.com/in/stuartdadams

This file is part of the Moka Real-Time Physically-Based Rendering Project.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

===========================================================================
*/
#pragma once

#include <cstdint>
#include <vector>

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
        rg16f,
        rgb,
        srgb8,
        rgb16f,
        bgr,
        rgba,
        srgb8_alpha8,
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

    /**
     * \brief A handle to a texture object on the device.
     */
    struct texture_handle final
    {
        uint16_t id;

        bool operator==(const texture_handle& rhs) const;

        bool operator!=(const texture_handle& rhs) const;

        bool operator>(const texture_handle& rhs) const;

        bool operator<(const texture_handle& rhs) const;

        bool operator>=(const texture_handle& rhs) const;

        bool operator<=(const texture_handle& rhs) const;
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

        wrap wrap_mode = {
            wrap_mode::clamp_to_edge, wrap_mode::clamp_to_edge, wrap_mode::clamp_to_edge};

        filter filter_mode = {mag_filter::linear, min_filter::linear};

        bool generate_mipmaps = false;
    };

    /**
     * \brief A builder class to help you build a texture.
     */
    class texture_builder final
    {
    public:
        /**
         * \brief Build a texture builder
         * \param device The graphics_device object that this texture will be uploaded to
         */
        explicit texture_builder(graphics_device& device);

        /**
         * \brief Set the texture's target.
         * \param target The texture's target.
         * \return A reference to this texture_builder object to enable method chaining.
         */
        texture_builder& set_target(texture_target target);

        /**
         * \brief Set the texture's S wrap mode
         * \param mode The wrap mode
         * \return A reference to this texture_builder object to enable method chaining.
         */
        texture_builder& set_wrap_s(wrap_mode mode);

        /**
         * \brief Set the texture's T wrap mode
         * \param mode The wrap mode
         * \return A reference to this texture_builder object to enable method chaining.
         */
        texture_builder& set_wrap_t(wrap_mode mode);

        /**
         * \brief Set the texture's R wrap mode
         * \param mode The wrap mode
         * \return A reference to this texture_builder object to enable method chaining.
         */
        texture_builder& set_wrap_r(wrap_mode mode);

        /**
         * \brief Set the texture's mag filter.
         * \param mag The mag filter.
         * \return A reference to this texture_builder object to enable method chaining.
         */
        texture_builder& set_mag_filter(mag_filter mag);

        /**
         * \brief Set the texture's min filter.
         * \param min The min filter.
         * \return A reference to this texture_builder object to enable method chaining.
         */
        texture_builder& set_min_filter(min_filter min);

        /**
         * \brief Free the host texture buffer after the texture is built.
         * \param free_data True if the host buffer should be freed. Otherwise, false.
         * \return A reference to this texture_builder object to enable method chaining.
         */
        texture_builder& free_host_data(bool free_data);

        /**
         * \brief Generate mipmaps with this texture.
         * \param generate_mipmaps True if this texture should be built with mipmaps.
         * \return A reference to this texture_builder object to enable method chaining.
         */
        texture_builder& set_mipmaps(bool generate_mipmaps);

        /**
         * \brief Build the final texture, upload it to the device, and return
         * the handle. \return The handle to the new texture.
         */
        texture_handle build();

        /**
         * \brief Specify a two-dimensional image. Describe the parameters of the texture image, such as height, width, width of the border, level-of-detail.
         * \param target Specify the target texture.
         * \param mip_level Specify the level of detail.
         * \param internal_format Specify the number of components in the texture.
         * \param width Specify the width of the texture image.
         * \param height Specify the height of the texture image.
         * \param border This value must be 0 (Why do we need it then?)
         * \param base_format Specifies the format of the pixel data.
         * \param type Specify the data type of the pixel data.
         * \param data A pointer to the image data in host memory.
         * \return A reference to this texture_builder object to enable method chaining.
         */
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
