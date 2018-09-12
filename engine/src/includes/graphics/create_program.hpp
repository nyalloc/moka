#pragma once
#include <messaging/message_base.hpp>
#include <graphics/graphics_api.hpp>
#include <messaging/basic_dispatcher.hpp>

namespace moka
{
	class create_program_cmd : public message_base
	{
		std::function<void(program_handle)> completion_handler_;
	public:
		shader_handle vertex_handle;
		shader_handle fragment_handle;

		explicit create_program_cmd(
			const shader_handle vertex_handle, 
			const shader_handle fragment_handle, 
			std::function<void(program_handle)>&& completion_handler)
			: completion_handler_(move(completion_handler))
			, vertex_handle(vertex_handle)
			, fragment_handle(fragment_handle)
		{}

		void operator()(const program_handle& handle) const
		{
			completion_handler_(handle);
		}

		using dispatcher = basic_dispatcher<create_program_cmd>;

		bool dispatch(base_dispatcher& visitor) override
		{
			return visitor.dispatch(*this);
		}
	};
}
