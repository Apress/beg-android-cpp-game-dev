#include "Chapter7App.h"

Chapter7App::Chapter7App(android_app* pState)
	:	Framework::Application(pState)
	,	m_chapter7Task(Framework::Task::GAME_PRIORITY)
{
}

Chapter7App::~Chapter7App()
{

}

bool Chapter7App::Initialize()
{
	bool success = Framework::Application::Initialize();

	if (success)
	{
		m_kernel.AddTask(&m_chapter7Task);
	}

	return success;
}
