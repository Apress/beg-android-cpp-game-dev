#include "Chapter10App.h"

Chapter10App::Chapter10App(android_app* pState)
	:	Framework::Application(pState)
	,	m_chapter10Task(Framework::Task::GAME_PRIORITY)
{
}

Chapter10App::~Chapter10App()
{

}

bool Chapter10App::Initialize()
{
	bool success = Framework::Application::Initialize();

	if (success)
	{
		m_kernel.AddTask(&m_chapter10Task);
	}

	return success;
}
