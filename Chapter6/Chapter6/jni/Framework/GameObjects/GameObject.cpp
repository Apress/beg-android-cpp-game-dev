#include "GameObject.h"

namespace Framework
{
	GameObject::GameObject()
	{

	}

	GameObject::~GameObject()
	{
		for (ComponentUnorderedMapIterator iter = m_components.begin(); iter != m_components.end(); ++iter)
		{
			Component* pComponent = iter->second;
			if (pComponent)
			{
				delete pComponent;
				pComponent = NULL;
			}
		}
	}

	Component* GameObject::GetComponent(unsigned int id)
	{
		ComponentUnorderedMapIterator result = m_components.find(id);
		return result == m_components.end()
				?	NULL
				:	result->second;
	}
}
