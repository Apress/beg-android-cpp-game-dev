#include "Chapter9App.h"

Chapter9App::Chapter9App(android_app* pState)
	:	Framework::Application(pState)
	,	m_chapter9Task(Framework::Task::GAME_PRIORITY)
{
}

Chapter9App::~Chapter9App()
{

}

bool Chapter9App::Initialize()
{
	bool success = Framework::Application::Initialize();

	if (success)
	{
		m_kernel.AddTask(&m_chapter9Task);
	}

	return success;
}
