#ifndef BOUNDOBJECTCOMPONENT_H_
#define BOUNDOBJECTCOMPONENT_H_

#include "../Component.h"
#include "../../EventManager/EventHandler.h"
#include "../../Math/Transform.h"

namespace Framework
{
	class TransformComponent;

	class BoundObjectComponent
		:	public Component
		,	public EventHandler
	{
	private:
		static const unsigned int s_id = 4;

		Transform						m_offsetTransform;

		const TransformComponent*		m_pBoundObject;

	public:
		static unsigned int GetId()	{ return s_id; }

		explicit BoundObjectComponent(GameObject* pOwner);
		virtual ~BoundObjectComponent();

		virtual void Initialize() {}

		Transform&					GetTransform()			{ return m_offsetTransform; }
		const Transform&			GetTransform() const	{ return m_offsetTransform; }

		void						SetBoundObject(const TransformComponent* pComponent)	{ m_pBoundObject = pComponent; }
		const TransformComponent*	GetBoundObject() const									{ return m_pBoundObject; }

		virtual void HandleEvent(Event* pEvent);
	};
}

#endif // BOUNDOBJECTCOMPONENT_H_
