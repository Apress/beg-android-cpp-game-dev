#include "Chapter6App.h"

Chapter6App::Chapter6App(android_app* pState)
	:	Framework::Application(pState)
	,	m_chapter6Task(Framework::Task::GAME_PRIORITY)
{
}

Chapter6App::~Chapter6App()
{

}

bool Chapter6App::Initialize()
{
	bool success = Framework::Application::Initialize();

	if (success)
	{
		m_kernel.AddTask(&m_chapter6Task);
	}

	return success;
}
