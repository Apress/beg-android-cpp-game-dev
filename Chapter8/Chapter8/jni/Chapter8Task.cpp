#include "Chapter8Task.h"
#include "Framework/Renderer/Renderer.h"
#include "Framework/EventManager/EventManager.h"
#include "Framework/Math/Transform.h"

using namespace Framework;

Chapter8Task::Chapter8Task(const unsigned int priority)
	:	Framework::Task(priority)
{
}

Chapter8Task::~Chapter8Task()
{
}

bool Chapter8Task::Start()
{
	RegisterEvent(UPDATE_EVENT);
	RegisterEvent(POSTUPDATE_EVENT);
	RegisterEvent(RENDER_EVENT);
	RegisterEvent(JUMP_EVENT);
	RegisterEvent(COLLISION_EVENT);

	m_pLevel = new DroidRunnerLevel();

	return true;
}

void Chapter8Task::OnSuspend()
{

}

void Chapter8Task::Update()
{
	if (Renderer::GetSingleton().IsInitialized())
	{
		assert(m_pLevel);
		if (m_pLevel && !m_pLevel->IsInitialized())
		{
			Framework::Vector3 levelOrigin(-21.0f, 7.75f, 35.0f);
			m_pLevel->Initialize(levelOrigin);
			Framework::AttachEvent(POSTUPDATE_EVENT, *m_pLevel);
		}

		Framework::SendEvent(Framework::UPDATE_EVENT);
		Framework::SendEvent(Framework::POSTUPDATE_EVENT);
		Framework::SendEvent(Framework::RENDER_EVENT);
	}
}

void Chapter8Task::OnResume()
{

}

void Chapter8Task::Stop()
{
	Framework::DetachEvent(POSTUPDATE_EVENT, *m_pLevel);

	delete m_pLevel;
	m_pLevel = NULL;
}
