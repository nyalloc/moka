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
#include <graphics/device/graphics_device.hpp>
#include <graphics/texture_handle.hpp>

namespace moka
{
    bool texture_handle::operator==(const texture_handle& rhs) const
    {
        return id == rhs.id;
    }

    bool texture_handle::operator!=(const texture_handle& rhs) const
    {
        return id != rhs.id;
    }

    bool texture_handle::operator>(const texture_handle& rhs) const
    {
        return id > rhs.id;
    }

    bool texture_handle::operator<(const texture_handle& rhs) const
    {
        return id < rhs.id;
    }

    bool texture_handle::operator>=(const texture_handle& rhs) const
    {
        return id >= rhs.id;
    }

    bool texture_handle::operator<=(const texture_handle& rhs) const
    {
        return id <= rhs.id;
    }

    texture_builder::texture_builder(graphics_device& device) : device_(device)
    {
    }

    texture_builder& texture_builder::set_target(const texture_target target)
    {
        metadata_.target = target;
        return *this;
    }

    texture_builder& texture_builder::set_wrap_s(const wrap_mode mode)
    {
        metadata_.wrap_mode.s = mode;
        return *this;
    }

    texture_builder& texture_builder::set_wrap_t(const wrap_mode mode)
    {
        metadata_.wrap_mode.t = mode;
        return *this;
    }

    texture_builder& texture_builder::set_wrap_r(const wrap_mode mode)
    {
        metadata_.wrap_mode.r = mode;
        return *this;
    }

    texture_builder& texture_builder::set_mag_filter(const mag_filter mag)
    {
        metadata_.filter_mode.mag = mag;
        return *this;
    }

    texture_builder& texture_builder::set_min_filter(const min_filter min)
    {
        metadata_.filter_mode.min = min;
        return *this;
    }

    texture_builder& texture_builder::free_host_data(const bool free_data)
    {
        free_host_data_ = free_data;
        return *this;
    }

    texture_builder& texture_builder::set_mipmaps(const bool generate_mipmaps)
    {
        metadata_.generate_mipmaps = generate_mipmaps;
        return *this;
    }

    texture_handle texture_builder::build()
    {
        return device_.make_texture(host_data_.data(), std::move(metadata_), free_host_data_);
    }

    texture_builder& texture_builder::add_image_data(
        image_target target,
        int mip_level,
        device_format internal_format,
        int width,
        int height,
        int border,
        host_format base_format,
        pixel_type type,
        const void* data)
    {
        metadata_.data.emplace_back(image_metadata{
            target, mip_level, type, internal_format, width, height, border, base_format});

        host_data_.emplace_back(data);

        return *this;
    }
} // namespace moka
