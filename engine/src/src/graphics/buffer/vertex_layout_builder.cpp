#include <graphics/buffer/vertex_layout_builder.hpp>

namespace moka
{
	vertex_layout_builder& vertex_layout_builder::add_attribute(size_t index, attribute_type type, size_t size,
	                                                            bool normalized, size_t stride, size_t offset)
	{
		this->attr_.emplace_back(index, type, size, normalized, stride, offset);
		return *this;
	}

	vertex_layout vertex_layout_builder::build()
	{
		return vertex_layout{std::move(attr_)};
	}
}
