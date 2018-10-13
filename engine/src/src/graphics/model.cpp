#include <graphics/model.hpp>
#include <graphics/graphics_device.hpp>
#include <graphics/draw_call_builder.hpp>

namespace moka
{
	transform& mesh::get_transform()
	{
		return transform_;
	}

	mesh::iterator mesh::begin()
	{
		return primitives_.begin();
	}

	mesh::const_iterator mesh::begin() const
	{
		return primitives_.begin();
	}

	mesh::iterator mesh::end()
	{
		return primitives_.end();
	}

	mesh::const_iterator mesh::end() const
	{
		return primitives_.end();
	}

	material& primitive::get_material()
	{
		return material_;
	}

	mesh::mesh(std::vector<primitive>&& primitives, transform && transform)
		: primitives_(std::move(primitives)), transform_(std::move(transform))
	{}

	void primitive::draw(graphics_device& device)
	{
		device.draw()
			.set_vertex_buffer(vertex_buffer_, 0, vertex_count_)
			.set_index_buffer(index_buffer_, index_count_)
			.set_material(material_);
	}

	primitive::primitive(vertex_buffer_handle vertex_buffer
		, uint32_t vertex_count
		, index_buffer_handle index_buffer
		, uint32_t index_count
		, const material& material)
		: vertex_buffer_(vertex_buffer)
		, vertex_count_(vertex_count)
		, index_buffer_(index_buffer)
		, index_count_(index_count)
		, material_(material)
	{}

	model::iterator model::begin()
	{
		return meshes_.begin();
	}

	model::const_iterator model::begin() const
	{
		return meshes_.begin();
	}

	model::iterator model::end()
	{
		return meshes_.end();
	}

	model::const_iterator model::end() const
	{
		return meshes_.end();
	}

	model::model(std::vector<mesh>&& meshes, transform&& transform)
		: meshes_(std::move(meshes)), transform_(std::move(transform))
	{}

	std::vector<mesh>& model::get_meshes()
	{
		return meshes_;
	}
}