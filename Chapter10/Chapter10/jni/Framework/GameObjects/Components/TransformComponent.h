#ifndef TRANSFORMCOMPONENT_H_
#define TRANSFORMCOMPONENT_H_

#include "../Component.h"
#include "../../Math/Transform.h"

namespace Framework
{
	class GameObject;

	class TransformComponent
		:	public Component
	{
	private:
		static const unsigned int s_id = 0;

		Transform	m_transform;

	public:
		static unsigned int GetId()	{ return s_id; }

		explicit TransformComponent(GameObject* pOwner);
		virtual ~TransformComponent();

		virtual void Initialize();

		Transform& GetTransform()				{ return m_transform; }
		const Transform& GetTransform() const	{ return m_transform; }
	};
}

#endif // TRANSFORMCOMPONENT_H_
