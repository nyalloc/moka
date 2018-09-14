#pragma once
#include <messaging/message_base.hpp>
#include <graphics/graphics_api.hpp>
#include <messaging/basic_dispatcher.hpp>

namespace moka
{
	class create_index_buffer_cmd : public message_base
	{
		std::function<void(index_buffer_handle)> completion_handler_;
	public:
		const void* indices;
		const size_t size;

		explicit create_index_buffer_cmd(
			const void* indices,
			const size_t size,
			std::function<void(index_buffer_handle)>&& completion_handler)
			: completion_handler_(move(completion_handler))
			, indices(indices)
			, size(size)
		{}

		void operator()(const index_buffer_handle& handle) const
		{
			completion_handler_(handle);
		}

		using dispatcher = basic_dispatcher<create_index_buffer_cmd>;

		bool dispatch(base_dispatcher& visitor) override
		{
			return visitor.dispatch(*this);
		}
	};
}
