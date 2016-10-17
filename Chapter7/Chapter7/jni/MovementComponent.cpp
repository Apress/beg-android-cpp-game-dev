#include "MovementComponent.h"
#include "Framework/GameObjects/Components/CollisionComponent.h"
#include "Framework/GameObjects/Components/TransformComponent.h"
#include "Framework/EventManager/EventManager.h"
#include "Framework/GameObjects/GameObject.h"
#include "Framework/Utilities/Timer.h"
#include "PatrolComponent.h"

using namespace Framework;

MovementComponent::MovementComponent(GameObject* pObject)
	:	Component(pObject)
	,	m_floor(0.0f)
	,	m_isSupported(false)
{
}

MovementComponent::~MovementComponent()
{

}

void MovementComponent::HandleEvent(Event* pEvent)
{
	if (pEvent->GetID() == JUMP_EVENT)
	{
		TransformComponent* pTransformComponent = component_cast<TransformComponent>(GetOwner());
		assert(pTransformComponent);
		if (pTransformComponent &&
				m_isSupported)
		{
			static const float JUMP_ACCELERATION = 80.0f;
			m_acceleration.m_y = JUMP_ACCELERATION;
		}
	}
	else if (pEvent->GetID() == UPDATE_EVENT)
	{
		TransformComponent* pTransformComponent = component_cast<TransformComponent>(GetOwner());
		assert(pTransformComponent);
		CollisionComponent* pCollisionComponent = component_cast<CollisionComponent>(GetOwner());
		assert(pCollisionComponent);
		if (pTransformComponent && pCollisionComponent)
		{
			const Vector3& position = pTransformComponent->GetTransform().GetTranslation();

			bool falling = m_acceleration.m_y < 0.0f;

			Vector3 bvMin = pCollisionComponent->GetMin();

			Vector3 translation = m_velocity;
			translation.Multiply(Timer::GetSingleton().GetTimeSim());
			translation.Add(position);

			const float offsetFloor = m_floor - bvMin.m_y;
			if (m_isSupported && falling && (translation.m_y < offsetFloor))
			{
				translation.m_y = offsetFloor;
			}

			pTransformComponent->GetTransform().SetTranslation(translation);

			Timer& timer = Timer::GetSingleton();
			Vector3 accel = m_acceleration;
			accel.Multiply(timer.GetTimeSim());
			m_velocity.Add(accel);

			static const float GRAVITY_MULTIPLIER	= 15.0f;
			static const float GRAVITY_CONSTANT		=  -9.8f;
			m_acceleration.m_y += GRAVITY_MULTIPLIER * GRAVITY_CONSTANT * timer.GetTimeSim();
			if (falling && m_isSupported)
			{
				m_acceleration.m_y  = 0.0f;
				m_velocity.m_y		= 0.0f;
			}
		}

		// Always undo support after an update, we'll be re-supported if we are colliding with a block.
		SetIsSupported(false);
	}
}

void MovementComponent::HandleCollision(Framework::CollisionEventData* pData)
{
	PatrolComponent* pPatrolComponent = component_cast<PatrolComponent>(pData->m_pCollider);
	if (pPatrolComponent)
	{
		// We're colliding with an AI, we're dead!
	}
	else
	{
		// We're colliding with a block
		TransformComponent* pColliderTransformComponent = component_cast<TransformComponent>(pData->m_pCollider);
		CollisionComponent* pColliderCollisionComponent = component_cast<CollisionComponent>(pData->m_pCollider);
		assert(pColliderTransformComponent && pColliderCollisionComponent);

		const Vector3& translation = pColliderTransformComponent->GetTransform().GetTranslation();

		Vector3 minPosition(pColliderCollisionComponent->GetMin());
		minPosition.Add(translation);

		TransformComponent* pObjectTransformComponent = component_cast<TransformComponent>(GetOwner());
		if (pObjectTransformComponent->GetTransform().GetTranslation().m_x < minPosition.m_x)
		{
			// We're dead because we've hit the side of the block
		}
		else
		{
			SetIsSupported(true, pColliderCollisionComponent->GetMax().m_y + translation.m_y);
		}
	}
}
