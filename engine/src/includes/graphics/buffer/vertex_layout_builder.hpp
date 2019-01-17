#pragma once

#include <graphics/buffer/vertex_attribute.hpp>
#include <vector>

namespace moka
{
    class vertex_layout;

    struct vertex_layout_builder final
    {
    private:
        std::vector<vertex_attribute> attr_;

    public:
        vertex_layout_builder& add_attribute(
            size_t index, attribute_type type, size_t size, bool normalized, size_t stride, size_t offset);

        vertex_layout build();
    };

} // namespace moka
