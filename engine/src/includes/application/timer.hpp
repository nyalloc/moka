#pragma once

#include <chrono>

namespace moka 
{
	class timer 
	{
		bool started_;
		bool paused_;
		std::chrono::steady_clock::time_point reference_;
		std::chrono::duration<long double> accumulated_;
	public:
		timer(bool start = false);
		timer(const timer& other) = default;
		timer(timer&& other) = default;
		virtual ~timer() = default;
		timer& operator=(const timer& other) = default;
		timer& operator=(timer&& other) = default;
		void start();
		void stop();
		void reset();
		template <class duration_t = std::chrono::milliseconds>
		typename duration_t::rep elapsed() const;
	};
}

inline moka::timer::timer(bool start)
	: started_(false)
	, paused_(false)
	, reference_(std::chrono::steady_clock::now())
	, accumulated_(std::chrono::duration<long double>(0)) 
{
	if (start) 
	{
		this->start();
	}
}

inline void moka::timer::start() 
{
	if (!started_) 
	{
		started_ = true;
		paused_ = false;
		accumulated_ = std::chrono::duration<long double>(0);
		reference_ = std::chrono::steady_clock::now();
	}
	else if (paused_) 
	{
		reference_ = std::chrono::steady_clock::now();
		paused_ = false;
	}
}

inline void moka::timer::stop() 
{
	if (started_ && !paused_) 
	{
		std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
		accumulated_ = accumulated_ + std::chrono::duration_cast< std::chrono::duration<long double> >(now - reference_);
		paused_ = true;
	}
}

inline void moka::timer::reset() 
{
	if (started_)
	{
		started_ = false;
		paused_ = false;
		reference_ = std::chrono::steady_clock::now();
		accumulated_ = std::chrono::duration<long double>(0);
	}
}

template <class duration_t>
typename duration_t::rep moka::timer::elapsed() const 
{
	if (started_) 
	{
		if (paused_)
		{
			return std::chrono::duration_cast<duration_t>(accumulated_).count();
		}
		else 
		{
			return std::chrono::duration_cast<duration_t>(
				accumulated_ + (std::chrono::steady_clock::now() - reference_)).count();
		}
	}
	else 
	{
		return duration_t(0).count();
	}
}