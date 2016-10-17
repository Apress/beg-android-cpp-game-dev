#include "PatrolComponent.h"
#include "Framework/EventManager/Event.h"
#include "Framework/GameObjects/Components/TransformComponent.h"
#include "Framework/GameObjects/GameObject.h"
#include "Framework/Utilities/Timer.h"
#include <cassert>

using namespace Framework;

PatrolComponent::PatrolComponent(GameObject* pObject)
	:	Component(pObject)
	,	m_speed(0.0f)
{

}

PatrolComponent::~PatrolComponent()
{

}

void PatrolComponent::Initialize()
{

}

void PatrolComponent::HandleEvent(Event* pEvent)
{
	if (pEvent->GetID() == Framework::UPDATE_EVENT && m_pTargetPoint)
	{
		assert(m_direction.LengthSquared() > FLT_EPSILON);
		assert(m_speed > 0.0f);

		TransformComponent* pTransformComponent = component_cast<TransformComponent>(GetOwner());
		assert(pTransformComponent);
		if (pTransformComponent)
		{
			Vector3 translation = m_direction;
			translation.Multiply(m_speed * Timer::GetSingleton().GetTimeSim());
			translation.Add(pTransformComponent->GetTransform().GetTranslation());
			pTransformComponent->GetTransform().SetTranslation(translation);

			Vector3 distance = *m_pTargetPoint;
			distance.Subtract(translation);
			if (distance.LengthSquared() < 2.0f)
			{
				Vector3* temp	= m_pTargetPoint;
				m_pTargetPoint	= m_pOriginPoint;
				m_pOriginPoint	= temp;

				m_direction = *m_pTargetPoint;
				m_direction.Subtract(*m_pOriginPoint);
				m_direction.Normalize();
			}
		}
	}
}

void PatrolComponent::SetStartPoint(Vector3& startPoint)
{
	m_startPoint.Set(startPoint);
	TransformComponent* pTransformComponent = component_cast<TransformComponent>(GetOwner());
	assert(pTransformComponent);
	if (pTransformComponent)
	{
		pTransformComponent->GetTransform().SetTranslation(m_startPoint);
	}
}

void PatrolComponent::SetEndPoint(Vector3& endPoint)
{
	assert(m_startPoint.LengthSquared() > FLT_EPSILON);

	m_endPoint.Set(endPoint);

	m_direction = m_endPoint;
	m_direction.Subtract(m_startPoint);
	m_direction.Normalize();

	m_pOriginPoint = &m_startPoint;
	m_pTargetPoint = &m_endPoint;
}
