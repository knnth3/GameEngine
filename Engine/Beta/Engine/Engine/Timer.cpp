#include "Timer.h"

using namespace Graphics;
using namespace std;
using  ms = chrono::milliseconds;
using  sec = chrono::seconds;
using get_time = chrono::steady_clock;

Graphics::Timer::Timer()
{
	m_numSamples = 10;
	m_polledFps = new double[m_numSamples]();
	m_start = get_time::now();
	m_current = m_start;
}

Graphics::Timer::~Timer()
{
}

void Graphics::Timer::update()
{
	static int time_index = 0;
	time_point newTime = get_time::now();
	auto diff = newTime - m_current;
	m_elapsed = (double)chrono::duration_cast<ms>(diff).count();
	m_polledFps[time_index++] = m_elapsed;
	time_index %= m_numSamples;
	m_current = get_time::now();
}

double Graphics::Timer::ms_per_frame()const
{
	double totalFPS = 0.0;
	for (int index = 0; index < m_numSamples; index++)
	{
		totalFPS += m_polledFps[index];
	}
	return totalFPS / (double)m_numSamples;
}

double Graphics::Timer::elapsed() const
{
	return m_elapsed;
}

double Graphics::Timer::total_seconds() const
{
	time_point newTime = get_time::now();
	auto diff = newTime - m_start;
	return (double)chrono::duration_cast<ms>(diff).count() / 1000.0;
}

void Graphics::Timer::set_sample_rate(int samples)
{
	m_numSamples = samples;
	delete[] m_polledFps;
	m_polledFps = new double[m_numSamples]();
}
