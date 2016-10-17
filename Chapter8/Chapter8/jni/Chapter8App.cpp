#include "Chapter8App.h"

Chapter8App::Chapter8App(android_app* pState)
	:	Framework::Application(pState)
	,	m_chapter8Task(Framework::Task::GAME_PRIORITY)
{
}

Chapter8App::~Chapter8App()
{

}

bool Chapter8App::Initialize()
{
	bool success = Framework::Application::Initialize();

	if (success)
	{
		m_kernel.AddTask(&m_chapter8Task);
	}

	return success;
}
