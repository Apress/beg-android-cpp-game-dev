#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include <unordered_map>
#include "Component.h"

namespace Framework
{
	class GameObject
	{
		template <class T>
		friend T* component_cast(GameObject& object);

		template <class T>
		friend T* component_cast(GameObject* pObject);

	private:
		typedef std::tr1::unordered_map<unsigned int, Component*>	ComponentUnorderedMap;
		typedef ComponentUnorderedMap::iterator						ComponentUnorderedMapIterator;
		ComponentUnorderedMap	m_components;

		template <class T>
		T*	GetComponent() { return static_cast<T*>(GetComponent(T::GetId())); }

		Component*	GetComponent(unsigned int id);

	public:
		GameObject();
		~GameObject();

		template <class T>
		bool AddComponent();
	};

	template <class T>
	T* component_cast(GameObject& object)
	{
		return object.GetComponent<T>();
	}

	template <class T>
	T* component_cast(GameObject* pObject)
	{
		T* pComponent = NULL;
		if (pObject)
		{
			pComponent = pObject->GetComponent<T>();
		}

		return pComponent;
	}

	template <class T>
	bool GameObject::AddComponent()
	{
		bool added = false;

		ComponentUnorderedMapIterator result = m_components.find(T::GetId());
		if (result == m_components.end())
		{
			T* pNewComponent = new T(this);

			if (pNewComponent)
			{
				std::pair<unsigned int, Component*> newComponent(T::GetId(), pNewComponent);
				std::pair<ComponentUnorderedMapIterator, bool> addedIter = m_components.insert(newComponent);
				added = addedIter.second;
			}
		}

		return added;
	}
}

#endif // GAMEOBJECT_H_
