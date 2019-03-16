#include <graphics/buffer/vertex_layout.hpp>

namespace moka
{
    vertex_layout::vertex_layout(std::vector<vertex_attribute>&& layout)
        : layout_(std::move(layout))
    {
        for (const auto& element : layout_)
        {
            total_size_ += element.size;
        }
    }

    vertex_layout::vertex_layout(const std::initializer_list<vertex_attribute> layout)
        : vertex_layout(std::vector<vertex_attribute>{layout})
    {
    }

    vertex_layout::iterator vertex_layout::begin() noexcept
    {
        return layout_.begin();
    }

    vertex_layout::iterator vertex_layout::end() noexcept
    {
        return layout_.end();
    }

    vertex_layout::const_iterator vertex_layout::begin() const noexcept
    {
        return layout_.begin();
    }

    vertex_layout::const_iterator vertex_layout::end() const noexcept
    {
        return layout_.end();
    }

    size_t vertex_layout::total_size() const noexcept
    {
        return total_size_;
    }
} // namespace moka
