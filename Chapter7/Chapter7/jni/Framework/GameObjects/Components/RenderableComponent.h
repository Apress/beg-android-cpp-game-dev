#ifndef RENDERABLECOMPONENT_H_
#define RENDERABLECOMPONENT_H_

#include "../Component.h"
#include "../../Renderer/Renderable.h"
#include "../../EventManager/EventHandler.h"
#include "../../EventManager/EventId.h"

namespace Framework
{
	class RenderableComponent
		:	public Component
		,	public EventHandler
	{
	private:
		static const unsigned int s_id = 1;

		Renderable		m_renderable;

	public:
		static unsigned int GetId()	{ return s_id; }

		explicit RenderableComponent(GameObject* pOwner);
		virtual ~RenderableComponent();

		virtual void Initialize();

		Renderable& GetRenderable()	{ return m_renderable; }

		virtual void HandleEvent(Event* pEvent);
	};
}

#endif // RENDERABLECOMPONENT_H_
