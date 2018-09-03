#pragma once
#include "message_queue.hpp"
#include "blocking_policy.hpp"
#include "close_queue.hpp"
#include "chained_dispatcher.hpp"

namespace moka
{
	template<typename BlockingPolicy>
	class dispatcher
	{
		message_queue* _q;
		bool _chained;

		template <typename = BlockingPolicy
			, typename Dispatcher
			, typename OtherMsg
			, typename OtherFunc>
			friend class chained_dispatcher;

		void wait_and_dispatch() const
		{
			for (;;)
			{
				const auto msg = _q->wait_and_pop();
				dispatch(msg);
			}
		}

		void try_and_dispatch() const
		{
			const auto msg = _q->try_and_pop();
			if (msg)
			{
				dispatch(msg);
			}
		}

		static bool dispatch(const std::shared_ptr<message_base>& msg)
		{
			close_queue::dispatcher v;
			return msg->dispatch(v);
		}
	public:
		dispatcher(const dispatcher&) = delete;
		dispatcher& operator=(const dispatcher&) = delete;

		dispatcher(dispatcher&& other) noexcept
			: _q(other._q)
			, _chained(other._chained)
		{
			other._chained = true;
		}

		explicit dispatcher(message_queue* q)
			: _q(q)
			, _chained(false)
		{}

		template <typename Message, typename Func>
		chained_dispatcher<BlockingPolicy, dispatcher, Message, Func> handle(Func&& f)
		{
			return chained_dispatcher<BlockingPolicy, dispatcher, Message, Func>(_q, this, std::forward<Func>(f));
		}

		~dispatcher() noexcept(false)
		{
			if constexpr(std::is_same<blocking, BlockingPolicy>::value)
			{
				if (!_chained)
				{
					wait_and_dispatch();
				}
			}
			else if constexpr(std::is_same<non_blocking, BlockingPolicy>::value)
			{
				if (!_chained)
				{
					try_and_dispatch();
				}
			}
		}
	};
}
