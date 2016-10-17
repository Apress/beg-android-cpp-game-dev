#ifndef EVENT_H_
#define EVENT_H_

#include <vector>
#include "EventHandler.h"
#include "EventId.h"

namespace Framework
{
	class Event
	{
	private:
		typedef std::vector<EventHandler*>		EventHandlerList;
		typedef EventHandlerList::iterator		EventHandlerListIterator;

		EventHandlerList						m_listeners;
		EventID									m_id;
		void*									m_pData;

	public:
		explicit Event(EventID eventId);
		~Event();

		void Send(void* pData);
		void SendToHandler(EventHandler& eventHandler, void* pData);
		void AttachListener(EventHandler& eventHandler);
		void DetachListener(EventHandler& eventHandler);

		EventID		GetID()	const				{ return m_id; }
		void*		GetData()					{ return m_pData; }
	};
}

#endif // EVENT_H_
