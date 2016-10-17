#include "Application.h"
#include "../Utilities/File.h"

namespace Framework
{
	Application::Application(android_app* state)
		:	m_rendererTask(state, Task::RENDER_PRIORITY)
		,	m_androidTask(state, Task::PLATFORM_PRIORITY, &m_rendererTask)
		,	m_timerTask(Task::TIMER_PRIORITY)
	{
		File::SetAssetManager(state->activity->assetManager);
	}

	Application::~Application()
	{

	}

	bool Application::Initialize()
	{
		bool ret = true;

		m_kernel.AddTask(&m_androidTask);
		m_kernel.AddTask(&m_timerTask);
		m_kernel.AddTask(&m_rendererTask);

		return ret;
	}

	void Application::Run()
	{
		m_kernel.Execute();
	}
}
