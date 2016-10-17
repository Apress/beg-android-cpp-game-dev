#ifndef ANDROID_H_
#define ANDROID_H_

#include <android_native_app_glue.h>
#include "../Kernel/Task.h"

namespace Framework
{
	class Renderer;

	class Android
		:	public Task
	{
	private:
		static bool		m_bClosing;
		static bool		m_bPaused;
		android_app*	m_pState;

	public:
		Android(android_app* pState, const unsigned int priority, Renderer* pRenderer);
		virtual ~Android();

		android_app*	GetAppState() { return m_pState; }

		virtual bool	Start();
		virtual void 	OnSuspend();
		virtual void 	Update();
		virtual void 	OnResume();
		virtual void 	Stop();

		static void ClearClosing()					{ m_bClosing = false; }
		static bool IsClosing()						{ return m_bClosing; }
		static void SetPaused(const bool paused)	{ m_bPaused = paused; }
		static bool IsPaused()		 				{ return m_bPaused; }
	};
}

#endif // ANDROID_H_
