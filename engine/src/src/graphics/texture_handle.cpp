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
        const image_target target,
        const int mip_level,
        const device_format internal_format,
        const int width,
        const int height,
        const int border,
        const host_format base_format,
        const pixel_type type,
        void* data)
    {
        metadata_.data.emplace_back(image_metadata{
            target, mip_level, type, internal_format, width, height, border, base_format});

        host_data_.emplace_back(data);

        return *this;
    }
} // namespace moka
