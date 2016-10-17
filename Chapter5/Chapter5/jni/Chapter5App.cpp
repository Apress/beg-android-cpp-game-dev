#include "Chapter5App.h"

Chapter5App::Chapter5App(android_app* pState)
	:	Framework::Application(pState)
	,	m_chapter5Task(&m_rendererTask, Framework::Task::GAME_PRIORITY)
{
}

Chapter5App::~Chapter5App()
{

}

bool Chapter5App::Initialize()
{
	bool success = Framework::Application::Initialize();

	if (success)
	{
		m_kernel.AddTask(&m_chapter5Task);
	}

	return success;
}
