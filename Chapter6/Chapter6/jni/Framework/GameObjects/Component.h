#ifndef COMPONENT_H_
#define COMPONENT_H_

#include "../Utilities/Hash.h"

namespace Framework
{
	class GameObject;

	class Component
	{
	private:
		GameObject*				m_pOwner;

	public:
		explicit Component(GameObject* pOwner)
			:	m_pOwner(pOwner)
		{
		}

		virtual ~Component()	{}

		virtual void Initialize()	= 0;

		GameObject* GetOwner() { return m_pOwner; }
	};
}

#endif /* COMPONENT_H_ */
