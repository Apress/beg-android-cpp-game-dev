#include "CollisionComponent.h"
#include "TransformComponent.h"
#include "../../Math/Transform.h"
#include "../GameObject.h"
#include "../../EventManager/Event.h"

namespace Framework
{
	CollisionComponent::CollisionComponent(GameObject* pOwner)
		:	Component(pOwner)
	{

	}

	CollisionComponent::~CollisionComponent()
	{

	}

	void CollisionComponent::Initialize()
	{

	}

	bool CollisionComponent::Intersects(CollisionComponent& target)
	{
		bool intersecting = true;

		Vector3 thisMin = m_min;
		Vector3 thisMax = m_max;

		TransformComponent* pThisTransformComponent = component_cast<TransformComponent>(GetOwner());
		if (pThisTransformComponent)
		{
			Transform& transform = pThisTransformComponent->GetTransform();
			thisMin.Add(transform.GetTranslation());
			thisMax.Add(transform.GetTranslation());
		}

		Vector3 targetMin = target.m_min;
		Vector3 targetMax = target.m_max;

		TransformComponent* pTargetTransformComponent = component_cast<TransformComponent>(target.GetOwner());
		if (pTargetTransformComponent)
		{
			Transform& transform = pTargetTransformComponent->GetTransform();
			targetMin.Add(transform.GetTranslation());
			targetMax.Add(transform.GetTranslation());
		}

		if (thisMin.m_x > targetMax.m_x ||
			thisMax.m_x < targetMin.m_x ||
			thisMin.m_y > targetMax.m_y ||
			thisMax.m_y < targetMin.m_y ||
			thisMin.m_z > targetMax.m_z ||
			thisMax.m_z < targetMin.m_z)
		{
			intersecting = false;
		}

		return intersecting;
	}

	void CollisionComponent::HandleEvent(Event* pEvent)
	{
		if (pEvent->GetID() == COLLISION_EVENT)
		{
			CollisionEventData* pCollisionData = static_cast<CollisionEventData*>(pEvent->GetData());
			if (pCollisionData && pCollisionData->m_pCollider)
			{
				for (CollisionListenerVectorIterator iter = m_eventListeners.begin(); iter != m_eventListeners.end(); ++iter)
				{
					(*iter)->HandleCollision(pCollisionData);
				}
			}
		}
	}
}
