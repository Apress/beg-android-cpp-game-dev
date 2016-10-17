#ifndef COLLISIONCOMPONENT_H_
#define COLLISIONCOMPONENT_H_

#include "../Component.h"
#include "../../EventManager/EventHandler.h"
#include "../../Math/Vector3.h"
#include <vector>
#include "../../Collision/CollisionListener.h"

namespace Framework
{
	class CollisionComponent
		:	public Component
		,	public EventHandler
	{
	private:
		static const unsigned int s_id = 2;

		Vector3		m_min;
		Vector3		m_max;

		typedef std::vector<CollisionListener*>		CollisionListenerVector;
		typedef CollisionListenerVector::iterator	CollisionListenerVectorIterator;

		CollisionListenerVector						m_eventListeners;

	public:
		static unsigned int GetId()	{ return s_id; }

		explicit CollisionComponent(GameObject* pOwner);
		virtual ~CollisionComponent();

		virtual void Initialize();

		void SetMin(const Vector3& point)	{ m_min = point; }
		void SetMax(const Vector3& point)	{ m_max = point; }

		const Vector3& GetMin() const { return m_min; }
		const Vector3& GetMax() const { return m_max; }

		bool Intersects(CollisionComponent& target);

		virtual void HandleEvent(Event* pEvent);

		void AddEventListener(CollisionListener* pListener)	{ m_eventListeners.push_back(pListener); }
	};
}

#endif // COLLISIONCOMPONENT_H_
