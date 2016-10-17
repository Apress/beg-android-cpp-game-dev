#ifndef EVENTMANAGER_H_
#define EVENTMANAGER_H_

#include "../Utilities/Singleton.h"
#include <unordered_map>
#include "EventId.h"
#include "Event.h"

#include <cassert>

namespace Framework
{
	class EventManager
		:	public Singleton<EventManager>
	{
		friend void SendEvent(EventID eventId);
		friend void SendEventToHandler(EventID eventId, EventHandler& eventHandler);
		friend bool RegisterEvent(EventID eventId);
		friend void AttachEvent(EventID eventId, EventHandler& eventHandler);
		friend void DetachEvent(EventID eventId, EventHandler& eventHandler);

	private:
		typedef std::tr1::unordered_map<EventID, Event*>	EventMap;
		typedef EventMap::iterator							EventMapIterator;

		EventMap								m_eventMap;

		void SendEvent(EventID eventId);
		void SendEventToHandler(EventID eventId, EventHandler& eventHandler);
		bool RegisterEvent(EventID eventId);
		void AttachEvent(EventID eventId, EventHandler& eventHandler);
		void DetachEvent(EventID eventId, EventHandler& eventHandler);

	public:
		EventManager();
		~EventManager();
	};

	inline void SendEvent(EventID eventId)
	{
		EventManager* pEventManager = EventManager::GetSingletonPtr();
		assert(pEventManager);
		if (pEventManager)
		{
			pEventManager->SendEvent(eventId);
		}
	}

	inline void SendEventToHandler(EventID eventId, EventHandler& eventHandler)
	{
		EventManager* pEventManager = EventManager::GetSingletonPtr();
		assert(pEventManager);
		if (pEventManager)
		{
			pEventManager->SendEventToHandler(eventId, eventHandler);
		}
	}

	inline bool RegisterEvent(EventID eventId)
	{
		EventManager* pEventManager = EventManager::GetSingletonPtr();
		assert(pEventManager);
		if (pEventManager)
		{
			pEventManager->RegisterEvent(eventId);
		}
	}

	inline void AttachEvent(EventID eventId, EventHandler& eventHandler)
	{
		EventManager* pEventManager = EventManager::GetSingletonPtr();
		assert(pEventManager);
		if (pEventManager)
		{
			pEventManager->AttachEvent(eventId, eventHandler);
		}
	}

	inline void DetachEvent(EventID eventId, EventHandler& eventHandler)
	{
		EventManager* pEventManager = EventManager::GetSingletonPtr();
		assert(pEventManager);
		if (pEventManager)
		{
			pEventManager->DetachEvent(eventId, eventHandler);
		}
	}
}

#endif // EVENTMANAGER_H_
