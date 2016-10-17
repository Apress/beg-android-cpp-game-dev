#include "Timer.h"
#include <time.h>

namespace Framework
{
	Timer::Timer(const unsigned int priority)
		: Task(priority)
		, m_timeLastFrame(0)
		, m_frameDt(0.0f)
		, m_simDt(0.0f)
		, m_simMultiplier(1.0f)
	{
	}

	Timer::~Timer()
	{
	}

	Timer::TimeUnits Timer::nanoTime()
	{
		timespec now;
		int err = clock_gettime(CLOCK_MONOTONIC, &now);
		return now.tv_sec*1000000000LL + now.tv_nsec;
	}

	bool Timer::Start()
	{
		m_timeLastFrame = nanoTime();
		return true;
	}

	void Timer::OnSuspend()
	{
	}

	void Timer::Update()
	{
		// Get the delta between the last frame and this
		TimeUnits currentTime = nanoTime();
		const float MULTIPLIER = 0.000000001f;
		m_frameDt = (currentTime-m_timeLastFrame) * MULTIPLIER;
		m_timeLastFrame = currentTime;

		if (m_frameDt > 0.2f)
		{
			m_frameDt = 0.2f;
		}

		m_simDt = m_frameDt * m_simMultiplier;
	}

	void Timer::OnResume()
	{
		m_timeLastFrame = nanoTime();
	}

	void Timer::Stop()
	{
	}
}
