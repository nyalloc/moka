#pragma once
#include <messaging/message_base.hpp>
#include <graphics/graphics_api.hpp>
#include <messaging/basic_dispatcher.hpp>

namespace moka
{
	class create_shader_cmd : public message_base
	{
		std::function<void(shader_handle)> completion_handler_;
	public:
		const shader_type type;
		const std::string source;

		explicit create_shader_cmd(
			const shader_type type,
			const std::string& source,
			std::function<void(shader_handle)>&& completion_handler)
			: completion_handler_(move(completion_handler))
			, type(type)
			, source(source)
		{}

		void operator()(const shader_handle& handle) const
		{
			completion_handler_(handle);
		}

		using dispatcher = basic_dispatcher<create_shader_cmd>;

		bool dispatch(base_dispatcher& visitor) override
		{
			return visitor.dispatch(*this);
		}
	};
}
