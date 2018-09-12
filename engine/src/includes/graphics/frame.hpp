#pragma once
#include <messaging/message_base.hpp>
#include <messaging/basic_dispatcher.hpp>

namespace moka
{
	class frame_cmd : public message_base
	{
		std::function<void()> completion_handler_;
	public:
		frame_cmd(std::function<void()>&& completion_handler)
			: completion_handler_(std::move(completion_handler))
		{}

		void operator()() const
		{
			completion_handler_();
		}

		using dispatcher = basic_dispatcher<frame_cmd>;

		bool dispatch(base_dispatcher& visitor) override
		{
			return visitor.dispatch(*this);
		}
	};
}
