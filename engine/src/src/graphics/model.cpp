#include <graphics/model.hpp>
#include <graphics/graphics_device.hpp>

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

	const material& primitive::get_material() const
	{
		return material_;
	}

	mesh::mesh(std::vector<primitive>&& primitives, transform && transform)
		: primitives_(std::move(primitives)), transform_(transform)
	{}

	void primitive::draw(command_list& cmd) const
	{
		cmd.draw()
			.set_vertex_buffer(vertex_buffer_)
			.set_vertex_count(vertex_count_)
			.set_index_buffer(index_buffer_)
			.set_index_type(index_type_)
			.set_primitive_type(type_)
			.set_index_count(index_count_)
			.set_index_buffer_offset(index_buffer_offset_)
			.set_material(material_);
	}

	primitive::primitive(vertex_buffer vertex_buffer
		, uint32_t vertex_count
		, index_buffer index_buffer
		, index_type index_type
		, uint32_t index_count
		, uint32_t index_buffer_offset
		, material&& material)
		: vertex_buffer_(vertex_buffer)
		, vertex_count_(vertex_count)
		, index_buffer_(index_buffer)
		, index_type_(index_type)
		, index_count_(index_count)
		, index_buffer_offset_(index_buffer_offset)
		, material_(std::move(material))
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
		: meshes_(std::move(meshes)), transform_(transform)
	{}

	std::vector<mesh>& model::get_meshes()
	{
		return meshes_;
	}
}