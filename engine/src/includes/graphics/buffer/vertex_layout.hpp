#pragma once

#include <vector>
#include <initializer_list>
#include <graphics/buffer/vertex_attribute.hpp>

namespace moka
{
	struct vertex_layout_builder;

	struct vertex_layout final
	{
		using builder = vertex_layout_builder;
		using iterator = std::vector<vertex_attribute>::iterator;
		using const_iterator = std::vector<vertex_attribute>::const_iterator;

		vertex_layout() = default;

		vertex_layout(std::initializer_list<vertex_attribute> layout);

		explicit vertex_layout(std::vector<vertex_attribute>&& layout);

		iterator begin() noexcept;

		iterator end() noexcept;

		const_iterator begin() const noexcept;

		const_iterator end() const noexcept;

		size_t total_size() const noexcept;

	private:
		std::vector<vertex_attribute> layout_;
	};
}