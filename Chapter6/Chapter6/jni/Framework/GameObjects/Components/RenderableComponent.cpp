#include "RenderableComponent.h"
#include "../../Renderer/Renderer.h"
#include "TransformComponent.h"
#include "../GameObject.h"
#include "../../EventManager/Event.h"
#include <cassert>

namespace Framework
{
	RenderableComponent::RenderableComponent(GameObject* pOwner)
		:	Component(pOwner)
	{

	}

	RenderableComponent::~RenderableComponent()
	{

	}

	void RenderableComponent::Initialize()
	{

	}

	void RenderableComponent::HandleEvent(Event* pEvent)
	{
		assert(pEvent);
		if (pEvent->GetID() == RENDER_EVENT)
		{
			TransformComponent* pTransformComponent = component_cast<TransformComponent>(GetOwner());
			if (pTransformComponent)
			{
				m_renderable.GetTransform().Clone(pTransformComponent->GetTransform());
			}

			assert(Renderer::GetSingletonPtr());
			Renderer::GetSingleton().AddRenderable(&m_renderable);
		}
	}
}
