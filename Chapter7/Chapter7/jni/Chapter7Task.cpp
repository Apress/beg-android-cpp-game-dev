#include "Chapter7Task.h"
#include "Framework/Renderer/Renderer.h"
#include "Framework/EventManager/EventManager.h"
#include "Framework/Math/Transform.h"

using namespace Framework;

Chapter7Task::Chapter7Task(const unsigned int priority)
	:	Framework::Task(priority)
{
}

Chapter7Task::~Chapter7Task()
{
}

bool Chapter7Task::Start()
{
	RegisterEvent(UPDATE_EVENT);
	RegisterEvent(POSTUPDATE_EVENT);
	RegisterEvent(RENDER_EVENT);
	RegisterEvent(JUMP_EVENT);
	RegisterEvent(COLLISION_EVENT);

	Framework::Vector3 levelOrigin(-21.0f, 7.75f, 35.0f);
	m_level.Initialize(levelOrigin);
	Framework::AttachEvent(POSTUPDATE_EVENT, m_level);

	return true;
}

void Chapter7Task::OnSuspend()
{

}

void Chapter7Task::Update()
{
	if (Renderer::GetSingleton().IsInitialized())
	{
		Framework::SendEvent(Framework::UPDATE_EVENT);
		Framework::SendEvent(Framework::POSTUPDATE_EVENT);
		Framework::SendEvent(Framework::RENDER_EVENT);
	}
}

void Chapter7Task::OnResume()
{

}

void Chapter7Task::Stop()
{

}
