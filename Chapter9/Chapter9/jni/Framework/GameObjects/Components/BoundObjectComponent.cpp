#include "BoundObjectComponent.h"
#include "TransformComponent.h"
#include "../../EventManager/Event.h"
#include "../GameObject.h"
#include <cassert>

namespace Framework
{
	BoundObjectComponent::BoundObjectComponent(GameObject* pOwner)
		:	Component(pOwner)
		,	m_pBoundObject(NULL)
	{

	}

	BoundObjectComponent::~BoundObjectComponent()
	{

	}

	void BoundObjectComponent::HandleEvent(Event* pEvent)
	{
		if (pEvent->GetID() == UPDATE_EVENT && m_pBoundObject)
		{
			TransformComponent* pTransformComponent = component_cast<TransformComponent>(GetOwner());
			assert(pTransformComponent);
			Transform& ourTransform = pTransformComponent->GetTransform();
			const Transform& boundTransform = m_pBoundObject->GetTransform();
			Vector3 xTranslation;
			xTranslation.m_x = boundTransform.GetTranslation().m_x;

			Vector3 translation = m_offsetTransform.GetTranslation();
			translation.Add(xTranslation);
			ourTransform.SetTranslation(translation);

			ourTransform.UpdateMatrix();
		}
	}
}
