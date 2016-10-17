#ifndef TIMER_H_
#define TIMER_H_

#include "../Kernel/Task.h"

namespace Framework
{
	class Timer
		:	public Task
	{
	public:
		typedef long long	TimeUnits;

	private:
		TimeUnits nanoTime();

		TimeUnits	m_timeLastFrame;
		float		m_frameDt;
		float		m_simDt;
		float		m_simMultiplier;

	public:
		Timer(const unsigned int priority);
		~Timer();

		float			GetTimeFrame() const;
		float			GetTimeSim() const;
		void			SetSimMultiplier(const float simMultiplier);

		virtual bool	Start();
		virtual void 	OnSuspend();
		virtual void 	Update();
		virtual void 	OnResume();
		virtual void 	Stop();
	};

	inline float Timer::GetTimeFrame() const
	{
		return m_frameDt;
	}

	inline float Timer::GetTimeSim() const
	{
		return m_simDt;
	}

	inline void Timer::SetSimMultiplier(const float simMultiplier)
	{
		m_simMultiplier = simMultiplier;
	}
}

#endif // TIMER_H_
