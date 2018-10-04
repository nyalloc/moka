#pragma once
#include <messaging/message_base.hpp>
#include <graphics/graphics_api.hpp>
#include <messaging/basic_dispatcher.hpp>
#include <asset_importer/texture_importer.hpp>

namespace moka
{
	class create_texture_cmd : public message_base
	{
		std::function<void(texture_handle)> completion_handler_;
	public:
		texture_data data;

		explicit create_texture_cmd(
			const texture_data data,
			std::function<void(texture_handle)>&& completion_handler)
			: data(data)
			, completion_handler_(move(completion_handler))
		{}

		void operator()(const texture_handle& handle) const
		{
			completion_handler_(handle);
		}

		using dispatcher = basic_dispatcher<create_texture_cmd>;

		bool dispatch(base_dispatcher& visitor) override
		{
			return visitor.dispatch(*this);
		}
	};
}
