#pragma once
#include <mutex>
#include <queue>

namespace moka
{
	template<typename T>
	class blocking_queue
	{
		std::mutex m_;
		std::condition_variable c_;
		std::queue<T> q_;
	public:
		size_t size() const
		{
			return q_.size();
		}

		bool empty() const
		{
			return q_.empty();
		}

		void push(T&& msg)
		{
			std::lock_guard<std::mutex> lk(m_);
			q_.emplace(std::forward<T>(msg));
			c_.notify_all();
		}

		T pop()
		{
			std::unique_lock<std::mutex> lk(m_);
			c_.wait(lk, [&] { return !q_.empty(); });
			auto res = std::move(q_.front());
			q_.pop();
			return res;
		}
	};
}