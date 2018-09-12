#pragma once
#include <messaging/message_base.hpp>
#include <graphics/graphics_api.hpp>
#include <messaging/basic_dispatcher.hpp>

namespace moka
{
	class create_vertex_buffer_cmd : public message_base
	{
		std::function<void(vertex_buffer_handle)> completion_handler_;
	public:
		const memory vertices;
		const vertex_layout layout;

		explicit create_vertex_buffer_cmd(
			const memory& vertices,
			const vertex_layout& layout,
			std::function<void(vertex_buffer_handle)>&& completion_handler)
			: completion_handler_(move(completion_handler))
			, vertices(vertices)
			, layout(layout)
		{}

		void operator()(const vertex_buffer_handle& handle) const
		{
			completion_handler_(handle);
		}

		using dispatcher = basic_dispatcher<create_vertex_buffer_cmd>;

		bool dispatch(base_dispatcher& visitor) override
		{
			return visitor.dispatch(*this);
		}
	};
}
