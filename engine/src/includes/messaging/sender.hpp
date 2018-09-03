#pragma once

#include "message_queue.hpp"

namespace moka
{
	class sender
	{
		message_queue* _q;
	public:
		sender()
			: _q(nullptr)
		{}

		explicit sender(message_queue* q)
			: _q(q)
		{}

		template <typename Message>
		void send(const Message& msg)
		{
			if (_q)
			{
				_q->push(msg);
			}
		}
	};
}
