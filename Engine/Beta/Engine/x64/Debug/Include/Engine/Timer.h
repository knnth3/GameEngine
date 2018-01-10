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
		__engine_decl Timer();
		__engine_decl ~Timer();
		__engine_decl void update();
		__engine_decl double ms_per_frame()const;
		__engine_decl double elapsed()const;
		__engine_decl double total_seconds()const;
		__engine_decl void set_sample_rate(int samples);

	private:
		int m_numSamples;
		double* m_polledFps;
		double m_elapsed;
		time_point m_start;
		time_point m_current;
	};

}