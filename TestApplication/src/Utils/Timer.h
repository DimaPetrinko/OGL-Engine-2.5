#pragma once

#include <iostream>
#include <chrono>

namespace Utils
{
	class Timer
	{
	private:
		std::chrono::time_point<std::chrono::_V2::high_resolution_clock> start, end;
		std::chrono::duration<float> duration;
	public:
		Timer()
		{
			start = std::chrono::high_resolution_clock::now();
		}

		~Timer()
		{
			end = std::chrono::high_resolution_clock::now();
			duration = end - start;

			auto ms = duration.count() * 1000.0f;
			std::cout << ms << "ms" << std::endl;
		}
	};
}