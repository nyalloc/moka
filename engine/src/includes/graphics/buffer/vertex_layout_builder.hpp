#pragma once

#include <graphics/buffer/vertex_attribute.hpp>
#include <vector>

namespace moka
{
    class vertex_layout;

    /**
     * \brief A builder class to help you build a vertex.
     */
    struct vertex_layout_builder final
    {
    private:
        std::vector<vertex_attribute> attr_;

    public:
        /**
         * \brief Add a vertex attribute to this layout.
         * \param index The index of the new vertex_attribute object.
         * \param type The type of the new vertex_attribute object.
         * \param size The size of the new vertex_attribute object.
         * \param normalized Should the new vertex_attribute be normalized?
         * \param stride The stride of the new vertex_attribute object.
         * \param offset The offset of the new vertex_attribute object.
         * \return A reference to this vertex_layout_builder object to enable method chaining.
         */
        vertex_layout_builder& add_attribute(
            size_t index, size_t size, attribute_type type, bool normalized, size_t stride, size_t offset);

        /**
         * \brief Build the final vertex layout, upload it to the device, and return the handle.
         * \return The handle to the new vertex layout.
         */
        vertex_layout build();
    };

} // namespace moka
