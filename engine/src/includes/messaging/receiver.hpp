#pragma once

#include <messaging/dispatcher.hpp>

namespace moka
{
	class receiver
	{
		message_queue _q;
	public:
		template <typename Message>
		void send(const Message& msg)
		{
			_q.push(msg);
		}

		dispatcher<blocking> wait()
		{
			return dispatcher<blocking>(&_q);
		}

		dispatcher<non_blocking> poll()
		{
			return dispatcher<non_blocking>(&_q);
		}
	};
}
