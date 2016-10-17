#include "RenderableComponent.h"
#include "../../Renderer/Renderer.h"
#include "CollisionComponent.h"
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

			CollisionComponent* pCollisionComponent = component_cast<CollisionComponent>(GetOwner());
			if (pCollisionComponent)
			{
				m_renderable.SetBoundMin(pCollisionComponent->GetMin());
				m_renderable.SetBoundMax(pCollisionComponent->GetMax());
				m_renderable.SetUseBounds(true);
			}
			else
			{
				m_renderable.SetUseBounds(false);
			}

			assert(Renderer::GetSingletonPtr());
			Renderer::GetSingleton().AddRenderable(&m_renderable);
		}
	}
}
