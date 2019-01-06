#pragma once

#include <vector>
#include <initializer_list>
#include "vertex_attribute.hpp"

namespace moka
{
	struct vertex_layout_builder;

	struct vertex_layout
	{
	private:
		std::vector<vertex_attribute> layout_;
	public:
		vertex_layout() = default;

		vertex_layout(std::initializer_list<vertex_attribute> layout)
			: layout_(layout)
		{}

		vertex_layout(std::vector<vertex_attribute>&& layout)
			: layout_(std::move(layout))
		{}

		using builder = vertex_layout_builder;

		size_t total_size() const noexcept;

		auto begin() noexcept
		{
			return layout_.begin();
		}

		auto end() noexcept
		{
			return layout_.end();
		}

		auto begin() const noexcept
		{
			return layout_.begin();
		}

		auto end() const noexcept
		{
			return layout_.end();
		}
	};
}