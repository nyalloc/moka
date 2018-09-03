#pragma once

#include "base_dispatcher.hpp"
#include <functional>

namespace moka
{
	// templated concrete visitor. Dispatches a specific message type, T.
	// returns false if a different type of message is specified when calling dispatch()
	template<typename T>
	class basic_dispatcher : public base_dispatcher
	{
		std::function<void(const T&)> _func;
	public:
		explicit basic_dispatcher(std::function<void(const T&)> func)
			: _func(std::move(func))
		{}

		bool dispatch(const T& event) override
		{
			_func(event);
			return true;
		}
	};
}
