#ifndef COLLISIONLISTENER_H_
#define COLLISIONLISTENER_H_

namespace Framework
{
	struct CollisionEventData;

	class CollisionListener
	{
	public:
		virtual void HandleCollision(CollisionEventData* pData) = 0;
	};
}

#endif // COLLISIONLISTENER_H_
