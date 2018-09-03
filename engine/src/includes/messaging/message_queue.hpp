#pragma once
#include <mutex>
#include <queue>

namespace moka
{
	class message_base;

	class message_queue
	{
		std::mutex _m;
		std::condition_variable _c;
		std::queue<std::shared_ptr<message_base>> _q;
	public:
		template <typename T>
		void push(const T& msg)
		{
			std::lock_guard<std::mutex> lk(_m);
			_q.push(std::make_shared<T>(msg));
			_c.notify_all();
		}

		std::shared_ptr<message_base> wait_and_pop()
		{
			std::unique_lock<std::mutex> lk(_m);
			_c.wait(lk, [&] {return !_q.empty(); });
			auto res = _q.front();
			_q.pop();
			return res;
		}

		std::shared_ptr<message_base> try_and_pop()
		{
			std::unique_lock<std::mutex> lk(_m);
			if (_q.empty())
			{
				return nullptr;
			}
			auto res = _q.front();
			_q.pop();
			return res;
		}
	};
}
