#pragma once

namespace moka
{
	class base_dispatcher;

	class message_base
	{
	public:
		virtual ~message_base() = default;
		virtual bool dispatch(base_dispatcher& visitor) = 0;
	};
}