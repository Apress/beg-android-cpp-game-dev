#ifndef EVENTID_H_
#define EVENTID_H_

namespace Framework
{
	class GameObject;

	typedef unsigned int EventID;

	static const EventID UPDATE_EVENT		= 0;
	static const EventID POSTUPDATE_EVENT	= 1;
	static const EventID RENDER_EVENT		= 2;
	static const EventID JUMP_EVENT			= 3;
	static const EventID COLLISION_EVENT	= 4;
	static const EventID PAUSEAPP_EVENT		= 5;
	static const EventID RESUMEAPP_EVENT	= 6;
	static const EventID PLAYERJUMP_EVENT	= 7;

	struct CollisionEventData
	{
		GameObject*	m_pCollider;
	};
}

#endif // EVENTID_H_
