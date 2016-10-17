#include "Application.h"
#include "../Utilities/File.h"
#include "../EventManager/EventManager.h"
#include "../Utilities/Timer.h"
#include "../Renderer/Renderer.h"

namespace Framework
{
	Application::Application(android_app* state)
		:	m_pAppState(state)
		,	m_androidTask(state, Task::PLATFORM_PRIORITY)
	{
		File::SetAssetManager(state->activity->assetManager);
	}

	Application::~Application()
	{

	}

	bool Application::Initialize()
	{
		bool ret = true;

		CreateSingletons();

		m_kernel.AddTask(&m_androidTask);
		m_kernel.AddTask(Timer::GetSingletonPtr());
		assert(Renderer::GetSingletonPtr());
		m_kernel.AddTask(Renderer::GetSingletonPtr());

		return ret;
	}

	void Application::Run()
	{
		m_kernel.Execute();

		DestroySingletons();
	}

	void Application::CreateSingletons()
	{
		new Timer(Task::TIMER_PRIORITY);
		new Renderer(m_pAppState, Task::RENDER_PRIORITY);
		new EventManager();
	}

	void Application::DestroySingletons()
	{
		assert(EventManager::GetSingletonPtr());
		delete EventManager::GetSingletonPtr();

		assert(Renderer::GetSingletonPtr());
		delete Renderer::GetSingletonPtr();

		assert(Timer::GetSingletonPtr());
		delete Timer::GetSingletonPtr();
	}
}
