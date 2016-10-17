#ifndef PATROLCOMPONENT_H_
#define PATROLCOMPONENT_H_

#include "Framework/GameObjects/Component.h"
#include "Framework/EventManager/EventHandler.h"
#include "Framework/Math/Vector3.h"

namespace Framework
{
	class GameObject;
}

class PatrolComponent
	:	public Framework::Component
	,	public Framework::EventHandler
{
private:
	Framework::Vector3		m_direction;
	Framework::Vector3		m_startPoint;
	Framework::Vector3		m_endPoint;
	Framework::Vector3*		m_pOriginPoint;
	Framework::Vector3*		m_pTargetPoint;
	float					m_speed;

	static const unsigned int s_id = 10;

public:
	static unsigned int GetId()	{ return s_id; }

	explicit PatrolComponent(Framework::GameObject* pObject);
	virtual ~PatrolComponent();

	virtual void Initialize();

	virtual void HandleEvent(Framework::Event* pEvent);

	void SetStartPoint(Framework::Vector3& startPoint);
	void SetEndPoint(Framework::Vector3& endPoint);
	void SetSpeed(float speed)	{ m_speed = speed; }
};

#endif // PATROLCOMPONENT_H_
