#ifndef MOVEMENTCOMPONENT_H_
#define MOVEMENTCOMPONENT_H_

#include "Framework/GameObjects/Component.h"
#include "Framework/EventManager/EventHandler.h"
#include "Framework/Math/Vector3.h"

namespace Framework
{
	class GameObject;
}

class MovementComponent
		:	public Framework::Component
		,	public Framework::EventHandler
{
private:
	static const unsigned int s_id = 9;

	Framework::Vector3		m_acceleration;
	Framework::Vector3		m_velocity;

public:
	static unsigned int GetId()	{ return s_id; }

	explicit MovementComponent(Framework::GameObject* pObject);
	virtual ~MovementComponent();

	virtual void Initialize();

	virtual void HandleEvent(Framework::Event* pEvent);
};

#endif // MOVEMENTCOMPONENT_H_
