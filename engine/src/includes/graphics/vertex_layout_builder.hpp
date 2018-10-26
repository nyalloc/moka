#pragma once

#include <graphics/vertex_layout.hpp>
#include <graphics/vertex_attribute.hpp>

namespace moka
{
	struct vertex_layout_builder
	{
	private:
		std::vector<vertex_attribute> attr;
	public:
		vertex_layout_builder& add_attribute(size_t index, attribute_type type, size_t size, bool normalized, size_t stride, size_t offset);
		vertex_layout build();

		operator vertex_layout()
		{
			return build();
		}
	};

	inline size_t vertex_layout::total_size() const noexcept
	{
		size_t result = 0;
		for (const auto& element : layout_)
		{
			result += element.size;
		}
		return result;
	}

	inline vertex_layout_builder& vertex_layout_builder::add_attribute(
		size_t index, attribute_type type, size_t size, bool normalized, size_t stride, size_t offset)
	{
		this->attr.emplace_back(index, type, size, normalized, stride, offset);
		return *this;
	}

	inline vertex_layout vertex_layout_builder::build()
	{
		return vertex_layout{ std::move(attr) };
	}
}