#pragma once

#include "base_dispatcher.hpp"
#include "message_base.hpp"

namespace moka
{
	class close_queue : public message_base
	{
	public:
		class dispatcher : public base_dispatcher
		{
			bool dispatch(const close_queue& event) override
			{
				return true;
			}
		};

		bool dispatch(base_dispatcher& visitor) override
		{
			return visitor.dispatch(*this);
		}
	};
}
