#pragma once

#include <messaging/dispatcher.hpp>
#include <messaging/sender.hpp>

namespace moka
{
	class receiver
	{
		message_queue _q;
	public:
		sender make_sender()
		{
			return sender(&_q);
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
