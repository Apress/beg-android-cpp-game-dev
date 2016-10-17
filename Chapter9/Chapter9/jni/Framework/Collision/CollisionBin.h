#ifndef COLLISIONBIN_H_
#define COLLISIONBIN_H_

#include <vector>
#include "../GameObjects/Components/CollisionComponent.h"

namespace Framework
{
	class CollisionBin
	{
	private:
		typedef std::vector<CollisionComponent*>	CollisionComponentVector;
		typedef CollisionComponentVector::iterator	CollisionComponentVectorIterator;

		CollisionComponentVector					m_collisionObjects;
		CollisionComponentVectorIterator			m_currentObject;

	public:
		CollisionBin();
		~CollisionBin();

		void 				AddObject(CollisionComponent* pCollisionComponent);
		CollisionComponent*	GetFirst();
		CollisionComponent*	GetNext();
		void				Clear();
	};
}

#endif // COLLISIONBIN_H_
