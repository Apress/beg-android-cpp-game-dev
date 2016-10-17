#include "MovementComponent.h"
#include "Framework/GameObjects/Components/TransformComponent.h"
#include "Framework/EventManager/EventManager.h"
#include "Framework/GameObjects/GameObject.h"
#include "Framework/Utilities/Timer.h"

using namespace Framework;

MovementComponent::MovementComponent(GameObject* pObject)
:	Component(pObject)
{
}

MovementComponent::~MovementComponent()
{

}

void MovementComponent::Initialize()
{

}

void MovementComponent::HandleEvent(Event* pEvent)
{
	if (pEvent->GetID() == JUMP_EVENT)
	{
		TransformComponent* pTransformComponent = component_cast<TransformComponent>(GetOwner());
		assert(pTransformComponent);
		if (pTransformComponent &&
				pTransformComponent->GetTransform().GetTranslation().m_y < FLT_EPSILON)
		{
			static const float JUMP_ACCELERATION = 80.0f;
			m_acceleration.m_y = JUMP_ACCELERATION;
		}
	}
	else if (pEvent->GetID() == UPDATE_EVENT)
	{
		TransformComponent* pTransformComponent = component_cast<TransformComponent>(GetOwner());
		assert(pTransformComponent);
		if (pTransformComponent)
		{
			const Vector3& position = pTransformComponent->GetTransform().GetTranslation();
			bool onFloor = false;
			if (position.m_y < FLT_EPSILON)
			{
				onFloor = true;
			}

			bool falling = m_acceleration.m_y < 0.0f;

			Vector3 translation = m_velocity;
			translation.Multiply(Timer::GetSingleton().GetTimeSim());
			translation.Add(position);
			if (falling && translation.m_y < 0.0f)
			{
				translation.m_y = 0.0f;
			}

			pTransformComponent->GetTransform().SetTranslation(translation);

			Timer& timer = Timer::GetSingleton();
			Vector3 accel = m_acceleration;
			accel.Multiply(timer.GetTimeSim());
			m_velocity.Add(accel);

			static const float GRAVITY_MULTIPLIER	= 15.0f;
			static const float GRAVITY_CONSTANT		=  -9.8f;
			m_acceleration.m_y += GRAVITY_MULTIPLIER * GRAVITY_CONSTANT * timer.GetTimeSim();
			if (falling && onFloor)
			{
				m_acceleration.m_y  = 0.0f;
				m_velocity.m_y		= 0.0f;
			}
		}
	}
}
