#include "Chapter6Task.h"
#include "Framework/Renderer/Renderer.h"
#include "Framework/Utilities/TGAFile.h"
#include "MovementComponent.h"
#include "PatrolComponent.h"
#include "Framework/GameObjects/Components/TransformComponent.h"
#include "Framework/GameObjects/Components/RenderableComponent.h"
#include "Framework/Renderer/Renderer.h"
#include "Framework/EventManager/EventManager.h"
#include "Framework/Math/Transform.h"

namespace
{
	float verts[] =
	{
#include "AndroidVerts.h"
	};

	unsigned short indices[] =
	{
#include "AndroidIndices.h"
	};

}

Chapter6Task::Chapter6Task(const unsigned int priority)
	:	Framework::Task(priority)
{
}

Chapter6Task::~Chapter6Task()
{
	Framework::Renderer* pRenderer = Framework::Renderer::GetSingletonPtr();
	if (pRenderer)
	{
		pRenderer->RemoveShader(&m_transformShader);
	}
}

bool Chapter6Task::Start()
{
	using namespace Framework;

	Renderer* pRenderer = Renderer::GetSingletonPtr();
	if (pRenderer)
	{
		pRenderer->AddShader(&m_transformShader);
	}

	m_geometry.SetVertexBuffer(verts);
	m_geometry.SetNumVertices(sizeof(verts) / sizeof(verts[0]));
	m_geometry.SetIndexBuffer(indices);
	m_geometry.SetNumIndices(sizeof(indices) / sizeof(indices[0]));
	m_geometry.SetName("android");

	m_geometry.SetNumVertexPositionElements(3);
	m_geometry.SetVertexStride(0);

	RegisterEvent(UPDATE_EVENT);
	RegisterEvent(RENDER_EVENT);
	RegisterEvent(JUMP_EVENT);

	m_playerObject.AddComponent<MovementComponent>();
	MovementComponent* pMovementComponent = component_cast<MovementComponent>(m_playerObject);
	if (pMovementComponent)
	{
		Framework::AttachEvent(Framework::UPDATE_EVENT, *pMovementComponent);
		Framework::AttachEvent(Framework::JUMP_EVENT, *pMovementComponent);
	}

	m_playerObject.AddComponent<TransformComponent>();
	TransformComponent* pTransformComponent = component_cast<TransformComponent>(m_playerObject);
	if (pTransformComponent)
	{
		Vector3 translation(-10.0f, 0.0f, 50.0f);
		Transform& transform = pTransformComponent->GetTransform();
		transform.SetTranslation(translation);
	}

	m_playerObject.AddComponent<RenderableComponent>();
	RenderableComponent* pRenderableComponent = component_cast<RenderableComponent>(m_playerObject);
	if (pRenderableComponent)
	{
		Renderable& renderable = pRenderableComponent->GetRenderable();
		renderable.SetGeometry(&m_geometry);
		renderable.SetShader(&m_transformShader);
		Vector4& color = renderable.GetColor();
		color.m_x = 0.0f;
		color.m_y = 1.0f;
		color.m_z = 0.0f;
		color.m_w = 1.0f;
		Framework::AttachEvent(Framework::RENDER_EVENT, *pRenderableComponent);
	}

	m_aiObject.AddComponent<TransformComponent>();
	pTransformComponent = component_cast<TransformComponent>(m_aiObject);

	m_aiObject.AddComponent<PatrolComponent>();
	PatrolComponent* pPatrolComponent = component_cast<PatrolComponent>(m_aiObject);
	if (pPatrolComponent)
	{
		Vector3 startPoint(10.0f, -10.0f, 75.0f);
		pPatrolComponent->SetStartPoint(startPoint);
		Vector3 endPoint(15.0f, 7.5f, 25.0f);
		pPatrolComponent->SetEndPoint(endPoint);
		pPatrolComponent->SetSpeed(25.0f);
		Framework::AttachEvent(UPDATE_EVENT, *pPatrolComponent);
	}

	m_aiObject.AddComponent<RenderableComponent>();
	pRenderableComponent = component_cast<RenderableComponent>(m_aiObject);
	if (pRenderableComponent)
	{
		Renderable& renderable = pRenderableComponent->GetRenderable();
		renderable.SetGeometry(&m_geometry);
		renderable.SetShader(&m_transformShader);
		Vector4& color = renderable.GetColor();
		color.m_x = 1.0f;
		color.m_y = 0.0f;
		color.m_z = 0.0f;
		color.m_w = 1.0f;
		Framework::AttachEvent(Framework::RENDER_EVENT, *pRenderableComponent);
	}

	return true;
}

void Chapter6Task::OnSuspend()
{

}

void Chapter6Task::Update()
{
	Framework::SendEvent(Framework::UPDATE_EVENT);
	Framework::SendEvent(Framework::RENDER_EVENT);
}

void Chapter6Task::OnResume()
{

}

void Chapter6Task::Stop()
{
	Framework::RenderableComponent* pRenderableComponent = Framework::component_cast<Framework::RenderableComponent>(m_playerObject);
	if (pRenderableComponent)
	{
		Framework::DetachEvent(Framework::RENDER_EVENT, *pRenderableComponent);
	}
}
