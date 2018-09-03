#pragma once

#include "message_queue.hpp"
#include "blocking_policy.hpp"

namespace moka
{
	template <typename BlockingPolicy
		, typename PreviousDispatcher
		, typename Msg
		, typename Func>
		class chained_dispatcher
	{
		message_queue* _q;
		PreviousDispatcher* _prev;
		Func _f;
		bool _chained;

		template <typename = BlockingPolicy
			, typename Dispatcher
			, typename OtherMsg
			, typename OtherFunc>
			friend class chained_dispatcher;

		void wait_and_dispatch()
		{
			for (;;)
			{
				const auto msg = _q->wait_and_pop();
				if (dispatch(msg))
				{
					break;
				}
			}
		}

		void try_and_dispatch()
		{
			const auto msg = _q->try_and_pop();
			if (msg)
			{
				dispatch(msg);
			}
		}

		bool dispatch(const std::shared_ptr<message_base>& msg)
		{
			typename Msg::dispatcher v(_f);
			if (msg->dispatch(v))
			{
				return true;
			}
			return _prev->dispatch(msg);
		}
	public:
		chained_dispatcher(const chained_dispatcher&) = delete;
		chained_dispatcher& operator=(const chained_dispatcher&) = delete;

		chained_dispatcher(chained_dispatcher&& other) noexcept
			: _q(other._q)
			, _prev(other._prev)
			, _f(std::move(other._f))
			, _chained(other._chained)
		{
			other._chained = true;
		}

		chained_dispatcher(message_queue* q
			, PreviousDispatcher* prev
			, Func&& f)
			: _q(q)
			, _prev(prev)
			, _f(std::forward<Func>(f))
			, _chained(false)
		{
			prev->_chained = true;
		}

		template <typename OtherMsg, typename OtherFunc>
		chained_dispatcher<BlockingPolicy, chained_dispatcher, OtherMsg, OtherFunc>
			handle(OtherFunc&& of)
		{
			return chained_dispatcher<BlockingPolicy, chained_dispatcher, OtherMsg, OtherFunc>(
				_q, this, std::forward<OtherFunc>(of));
		}

		~chained_dispatcher() noexcept(false)
		{
			if constexpr(std::is_same<blocking, BlockingPolicy>::value)
			{
				if (!_chained)
				{
					wait_and_dispatch();
				}
			}
			if constexpr(std::is_same<non_blocking, BlockingPolicy>::value)
			{
				if (!_chained)
				{
					try_and_dispatch();
				}
			}
		}
	};
}
