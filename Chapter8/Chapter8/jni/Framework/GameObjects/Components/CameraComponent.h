#ifndef CAMERACOMPONENT_H_
#define CAMERACOMPONENT_H_

#include "../Component.h"
#include "../../EventManager/EventHandler.h"
#include "../../Renderer/Renderer.h"

namespace Framework
{
	class CameraComponent
		:	public Component
		,	public EventHandler
	{
	private:
		static const unsigned int s_id = 3;

		float		m_frustumParameters[Renderer::NUM_PARAMS];

	public:
		static unsigned int GetId()	{ return s_id; }

		explicit CameraComponent(GameObject* pOwner);
		virtual ~CameraComponent();

		virtual void Initialize() {}

		void SetFrustum(const float verticalFieldOfView, const float aspectRatio, const float near, const float far);

		virtual void HandleEvent(Event* pEvent);
	};
}

#endif // CAMERACOMPONENT_H_
