#include "CollisionManager.h"
#include "../GameObjects/GameObject.h"
#include "../EventManager/EventManager.h"

namespace Framework
{
	CollisionManager::CollisionManager()
	{

	}

	CollisionManager::~CollisionManager()
	{

	}

	void CollisionManager::AddCollisionBin()
	{
		m_collisionBins.push_back();
	}

	void CollisionManager::AddObjectToBin(const unsigned int binIndex, CollisionComponent* pObject)
	{
		assert(binIndex < m_collisionBins.size());
		m_collisionBins[binIndex].AddObject(pObject);
	}

	void CollisionManager::TestAgainstBin(const unsigned int binIndex, CollisionComponent* pObject)
	{
		assert(binIndex < m_collisionBins.size());
		CollisionBin& bin = m_collisionBins[binIndex];
		CollisionComponent* pBinObject = bin.GetFirst();
		while (pBinObject)
		{
			if (pBinObject != pObject &&
				pBinObject->Intersects(*pObject))
			{
				CollisionEventData collisionData;
				collisionData.m_pCollider = pBinObject->GetOwner();
				SendEventToHandler(COLLISION_EVENT, *static_cast<EventHandler*>(pObject), &collisionData);
			}
			pBinObject = bin.GetNext();
		}
	}

	void CollisionManager::ClearBin(const unsigned int binIndex)
	{
		assert(binIndex < m_collisionBins.size());
		CollisionBin& bin = m_collisionBins[binIndex];
		bin.Clear();
	}
}
