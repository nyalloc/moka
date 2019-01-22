#include <graphics/buffer/vertex_layout.hpp>
#include <graphics/buffer/vertex_layout_builder.hpp>

namespace moka
{
    vertex_layout_builder& vertex_layout_builder::add_attribute(
        size_t index, size_t size, attribute_type type, bool normalized, size_t stride, size_t offset)
    {
        this->attr_.emplace_back(index, type, size, normalized, stride, offset);
        return *this;
    }

    vertex_layout vertex_layout_builder::build()
    {
        return vertex_layout{std::move(attr_)};
    }
} // namespace moka
