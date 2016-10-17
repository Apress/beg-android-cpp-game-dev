#include "CameraComponent.h"
#include "../../EventManager/Event.h"
#include "TransformComponent.h"
#include "../GameObject.h"
#include "../../Renderer/Renderer.h"
#include <cassert>

namespace Framework
{
	CameraComponent::CameraComponent(GameObject* pOwner)
		: Component(pOwner)
	{
		memset(m_frustumParameters, 0, sizeof(float) * Renderer::NUM_PARAMS);
	}

	CameraComponent::~CameraComponent()
	{

	}

	void CameraComponent::SetFrustum(const float verticalFieldOfView, const float aspectRatio, const float near, const float far)
	{
		float tanHalfAngleRadians						= static_cast<float>(tan(0.5f * verticalFieldOfView * (3.1415926536f / 180.0f)));
		m_frustumParameters[Renderer::TOP]				= near * tanHalfAngleRadians;
		m_frustumParameters[Renderer::BOTTOM]			= -m_frustumParameters[Renderer::TOP];
		m_frustumParameters[Renderer::RIGHT]			= aspectRatio * m_frustumParameters[Renderer::TOP];
		m_frustumParameters[Renderer::LEFT]				= -m_frustumParameters[Renderer::RIGHT];
		m_frustumParameters[Renderer::NEAR]				= near;
		m_frustumParameters[Renderer::FAR]				= far;
	}

	void CameraComponent::HandleEvent(Event* pEvent)
	{
		if (pEvent->GetID() == POSTUPDATE_EVENT)
		{
			TransformComponent* pTransformComponent = component_cast<TransformComponent>(GetOwner());
			assert(pTransformComponent);

			Renderer& renderer = Renderer::GetSingleton();

			Transform& cameraTransform = pTransformComponent->GetTransform();
			renderer.SetCameraMatrix(cameraTransform.GetMatrix());
			Matrix4 inverseCamera;
			cameraTransform.GetInverseMatrix(inverseCamera);
			renderer.SetViewMatrix(inverseCamera);
			renderer.SetFrustum(m_frustumParameters);
		}
	}
}
