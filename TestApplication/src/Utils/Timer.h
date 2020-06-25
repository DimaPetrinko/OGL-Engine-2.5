#pragma once

#include <iostream>
#include <chrono>

namespace Utils
{
	class Timer
	{
	private:
		std::chrono::time_point<std::chrono::_V2::high_resolution_clock> mStart, mEnd;
		std::chrono::duration<float> mDuration;
	public:
		Timer()
		{
			mStart = std::chrono::high_resolution_clock::now();
		}

		~Timer()
		{
			mEnd = std::chrono::high_resolution_clock::now();
			mDuration = mEnd - mStart;

			auto ms = mDuration.count() * 1000.0f;
			std::cout << ms << "ms" << std::endl;
		}
	};
}