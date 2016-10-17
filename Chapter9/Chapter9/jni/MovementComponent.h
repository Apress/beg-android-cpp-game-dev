#ifndef MOVEMENTCOMPONENT_H_
#define MOVEMENTCOMPONENT_H_

#include "Framework/GameObjects/Component.h"
#include "Framework/EventManager/EventHandler.h"
#include "Framework/Collision/CollisionListener.h"
#include "Framework/Math/Vector3.h"

namespace Framework
{
	class GameObject;
}

class MovementComponent
		:	public Framework::Component
		,	public Framework::EventHandler
		,	public Framework::CollisionListener
{
private:
	static const unsigned int s_id = 9;

	Framework::Vector3		m_acceleration;
	Framework::Vector3		m_velocity;

	float					m_floor;
	bool					m_isSupported;
	bool					m_isDead;

public:
	static unsigned int GetId()	{ return s_id; }

	explicit MovementComponent(Framework::GameObject* pObject);
	virtual ~MovementComponent();

	virtual void Initialize() {}

	virtual void HandleEvent(Framework::Event* pEvent);

	void SetIsSupported(bool isSupported, float floor = 0.0f)	{ m_isSupported = isSupported; m_floor = floor; }

	virtual void HandleCollision(Framework::CollisionEventData* pData);

	bool	IsDead() const { return m_isDead; }
};

#endif // MOVEMENTCOMPONENT_H_
