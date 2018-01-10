#pragma once
#include "DllSettings.h"
#include <mutex>
#include <chrono>

namespace Graphics
{
	typedef std::chrono::time_point<std::chrono::steady_clock> time_point;

	class Timer
	{
	public:
		GRAPHICS_DLL_API Timer();
		GRAPHICS_DLL_API ~Timer();
		GRAPHICS_DLL_API void update();
		GRAPHICS_DLL_API double ms_per_frame()const;
		GRAPHICS_DLL_API double elapsed()const;
		GRAPHICS_DLL_API double total_seconds()const;
		GRAPHICS_DLL_API void set_sample_rate(int samples);

	private:
		int m_numSamples;
		double* m_polledFps;
		double m_elapsed;
		time_point m_start;
		time_point m_current;
	};

}