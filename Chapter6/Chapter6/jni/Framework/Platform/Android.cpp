#include "Android.h"
#include <cassert>
#include "../Renderer/Renderer.h"
#include "../EventManager/EventManager.h"
#include "../EventManager/EventId.h"

namespace Framework
{
	bool Android::m_bClosing	= false;
	bool Android::m_bPaused		= false;

	static int android_handle_input(struct android_app* app, AInputEvent* event)
	{
		int handled = 0;
		if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)
		{
			int action = AMotionEvent_getAction(event);
			if (action == AMOTION_EVENT_ACTION_DOWN)
			{
				Framework::SendEvent(Framework::JUMP_EVENT);
				handled = 1;
			}
		}
		return handled;
	}

	static void android_handle_cmd(struct android_app* app, int32_t cmd)
	{
		switch (cmd)
		{
		case APP_CMD_INIT_WINDOW:
		{
			assert(Renderer::GetSingletonPtr());
			Renderer::GetSingleton().Init();
		}
		break;

		case APP_CMD_DESTROY:
		{
			assert(Renderer::GetSingletonPtr());
			Renderer::GetSingleton().Destroy();
		}
		break;

		case APP_CMD_TERM_WINDOW:
		{
			assert(Renderer::GetSingletonPtr());
			Renderer::GetSingleton().Destroy();
		}
		break;

		case APP_CMD_RESUME:
		{
			Android::SetPaused(false);
		}
		break;

		case APP_CMD_PAUSE:
			{
				Android::SetPaused(true);
			}
			break;
		}
	}

	Android::Android(android_app* pState, unsigned int priority)
		:	Task(priority)
	{
		m_pState				= pState;
		m_pState->onAppCmd		= android_handle_cmd;
		m_pState->onInputEvent	= android_handle_input;
	}

	Android::~Android()
	{
	}

	bool Android::Start()
	{
		return true;
	}

	void Android::OnSuspend()
	{
	}

	void Android::Update()
	{
		int events;
		struct android_poll_source* pSource;
		int ident = ALooper_pollAll(0, 0, &events, (void**)&pSource);
		if (ident >= 0)
		{
			if (pSource)
			{
				pSource->process(m_pState, pSource);
			}

			if (m_pState->destroyRequested)
			{
				m_bClosing = true;
			}
		}
	}

	void Android::OnResume()
	{
	}

	void Android::Stop()
	{
	}
}
