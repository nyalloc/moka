#pragma once

#include <chrono>

namespace moka 
{
	class timer final
	{
		bool started_;
		bool paused_;
		std::chrono::steady_clock::time_point reference_;
		std::chrono::duration<long double> accumulated_;
	public:
		timer(const timer& other) = default;

		timer(timer&& other) = default;

		timer& operator=(const timer& other) = default;

		timer& operator=(timer&& other) = default;

		~timer();

		explicit timer(bool start);

		void start();

		void stop();

		void reset();

		template <class duration_t = std::chrono::milliseconds>
		auto elapsed() const;
	};

	template <class duration_t>
	auto timer::elapsed() const
	{
		if (started_)
		{
			if (paused_)
			{
				return std::chrono::duration_cast<duration_t>(accumulated_).count();
			}
			return std::chrono::duration_cast<duration_t>(
				accumulated_ + (std::chrono::steady_clock::now() - reference_)).count();
		}
		return duration_t(0).count();
	}
}
