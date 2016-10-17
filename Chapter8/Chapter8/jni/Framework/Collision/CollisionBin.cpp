#include "CollisionBin.h"

namespace Framework
{
	CollisionBin::CollisionBin()
	{

	}

	CollisionBin::~CollisionBin()
	{
		m_collisionObjects.clear();
	}

	void CollisionBin::AddObject(CollisionComponent* pCollisionComponent)
	{
		m_collisionObjects.push_back(pCollisionComponent);
	}

	CollisionComponent* CollisionBin::GetFirst()
	{
		m_currentObject = m_collisionObjects.begin();
		return m_currentObject != m_collisionObjects.end()
				?	*m_currentObject
				:	NULL;
	}

	CollisionComponent* CollisionBin::GetNext()
	{
		CollisionComponent* pRet = NULL;

		if (m_currentObject != m_collisionObjects.end())
		{
			++m_currentObject;
			pRet = m_currentObject != m_collisionObjects.end()
						?	*m_currentObject
						:	NULL;
		}

		return pRet;
	}

	void CollisionBin::Clear()
	{
		m_collisionObjects.clear();
		m_currentObject = m_collisionObjects.begin();
	}
}
