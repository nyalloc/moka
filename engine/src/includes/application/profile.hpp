#pragma once

#include <chrono>

namespace moka
{
	template<typename Duration = std::chrono::microseconds,
		typename F,
		typename ... Args>
		typename Duration::rep profile(F&& fun, Args&&... args)
	{
		const auto beg = std::chrono::high_resolution_clock::now();
		std::forward<F>(fun)(std::forward<Args>(args)...);
		const auto end = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<Duration>(end - beg).count();
	}
}
