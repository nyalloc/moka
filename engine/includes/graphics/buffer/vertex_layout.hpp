#pragma once

#include <graphics/buffer/vertex_attribute.hpp>
#include <graphics/buffer/vertex_layout_builder.hpp>
#include <initializer_list>
#include <vector>

namespace moka
{
    /**
     * \brief Describes the layout of a single vertex in a vertex buffer.
     */
    class vertex_layout final
    {
    public:
        using builder = vertex_layout_builder;
        using iterator = std::vector<vertex_attribute>::iterator;
        using const_iterator = std::vector<vertex_attribute>::const_iterator;

        vertex_layout() = default;

        /**
         * \brief Create a vertex_layout object.
         * \param layout A initializer_list of vertex_attribute objects.
         */
        vertex_layout(std::initializer_list<vertex_attribute> layout);

        /**
         * \brief Create a vertex_layout object.
         * \param layout A vector of vertex_attribute objects.
         */
        explicit vertex_layout(std::vector<vertex_attribute>&& layout);

        iterator begin() noexcept;

        iterator end() noexcept;

        const_iterator begin() const noexcept;

        const_iterator end() const noexcept;

        /**
         * \brief Get the size of a single vertex in this vertex layout
         * \return The total size of a single vertex in this layout
         */
        size_t total_size() const noexcept;

    private:
        size_t total_size_ = 0;
        std::vector<vertex_attribute> layout_;
    };
} // namespace moka
