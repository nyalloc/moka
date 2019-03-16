#pragma once

#include <cstdint>

namespace moka
{
    /**
     * \brief The type of a vertex attribute.
     */
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

    /**
     * \brief Get the size of a vertex attribute in bytes.
     * \param attr_type The type of vertex attribute you want to get the size of.
     * \return The size of a vertex attribute in bytes.
     */
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

    /**
     * \brief A vertex attribute. Defines one vertex attribute as part of a vertex layout.
     */
    struct vertex_attribute final
    {
        size_t index; /**< Specifies the index of the generic vertex attribute to be modified. */
        attribute_type type; /**< Specifies the data type of each component in the array.*/
        size_t size; /**< Specifies the number of components per generic vertex attribute. Must be 1, 2, 3, 4.*/
        bool normalized = false; /**< Specifies whether fixed-point data values should be normalized (true) or converted directly as fixed-point values (false) when they are accessed. The initial value is false.*/
        size_t stride = 0; /**< Specifies the byte offset between consecutive generic vertex attributes. If stride is 0, the generic vertex attributes are understood to be tightly packed in the array. The initial value is 0.*/
        size_t offset = 0; /**< Specifies a offset of the first component of the first generic vertex attribute in the array. The initial value is 0.*/

        /**
         * \brief Create a new vertex_attribute object.
         * \param index The index of the new vertex_attribute object.
         * \param type The type of the new vertex_attribute object.
         * \param size The size of the new vertex_attribute object.
         * \param normalized Should the new vertex_attribute be normalized?
         * \param stride The stride of the new vertex_attribute object.
         * \param offset The offset of the new vertex_attribute object.
         */
        constexpr vertex_attribute(size_t index, attribute_type type, size_t size, bool normalized, size_t stride, size_t offset) noexcept;
    };

    constexpr vertex_attribute::vertex_attribute(
        const size_t index, const attribute_type type, const size_t size, const bool normalized, const size_t stride, const size_t offset) noexcept
        : index(index), type(type), size(size), normalized(normalized), stride(stride), offset(offset)
    {
    }
} // namespace moka
