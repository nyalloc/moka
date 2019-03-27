#include <application/timer.hpp>

namespace moka
{
	timer::~timer()
	{
		stop();
	}

	timer::timer(bool start)
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

	void timer::start()
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

	void timer::stop()
	{
		if (started_ && !paused_)
		{
			const auto now = std::chrono::steady_clock::now();
			accumulated_ = accumulated_ + std::chrono::duration_cast<std::chrono::duration<long double>>(
				now - reference_);
			paused_ = true;
		}
	}

	void timer::reset()
	{
		if (started_)
		{
			started_ = false;
			paused_ = false;
			reference_ = std::chrono::steady_clock::now();
			accumulated_ = std::chrono::duration<long double>(0);
		}
	}
}