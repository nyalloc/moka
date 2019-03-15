#pragma once

#include <cstdint>

namespace moka
{
    enum class attribute_type
    {
        int8,
        int16,
        int32,
        int64,
        uint8,
        uint16,
        uint32,
        uint64,
        float16,
        float32,
        float64,
        boolean,
    };

    constexpr size_t size(const attribute_type attr_type)
    {
        switch (attr_type)
        {
        case attribute_type::int8:
            return sizeof(int8_t);
        case attribute_type::int16:
            return sizeof(int16_t);
        case attribute_type::int32:
            return sizeof(int32_t);
        case attribute_type::int64:
            return sizeof(int64_t);
        case attribute_type::uint8:
            return sizeof(uint8_t);
        case attribute_type::uint16:
            return sizeof(uint16_t);
        case attribute_type::uint32:
            return sizeof(uint32_t);
        case attribute_type::uint64:
            return sizeof(int64_t);
        case attribute_type::float32:
            return sizeof(float);
        case attribute_type::float64:
            return sizeof(double);
        default:
            return 0;
        }
    }

    struct vertex_attribute final
    {
        size_t index;
        attribute_type type;
        size_t size;
        bool normalized;
        size_t stride;
        size_t offset;

        constexpr vertex_attribute(
            size_t index, attribute_type type, size_t size, bool normalized, size_t stride, size_t offset) noexcept;
    };

    constexpr vertex_attribute::vertex_attribute(
        const size_t index,
        const attribute_type type,
        const size_t size,
        const bool normalized,
        const size_t stride,
        const size_t offset) noexcept
        : index(index), type(type), size(size), normalized(normalized), stride(stride), offset(offset)
    {
    }
} // namespace moka
