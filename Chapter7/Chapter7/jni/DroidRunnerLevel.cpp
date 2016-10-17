#include "DroidRunnerLevel.h"
#include "Framework/Renderer/Renderer.h"
#include "Framework/GameObjects/Components/TransformComponent.h"
#include "MovementComponent.h"
#include "PatrolComponent.h"
#include "Framework/GameObjects/Components/RenderableComponent.h"
#include "Framework/GameObjects/Components/CollisionComponent.h"
#include "Framework/EventManager/EventManager.h"
#include "Framework/Collision/CollisionManager.h"

using namespace Framework;

namespace
{
	float sphereVerts[] =
	{
#include "AndroidVerts.h"
	};

	unsigned short sphereIndices[] =
	{
#include "AndroidIndices.h"
	};

	float cubeVerts[] =
	{
#include "CubeVerts.h"
	};

	unsigned short cubeIndices[] =
	{
#include "CubeIndices.h"
	};
}

DroidRunnerLevel::DroidRunnerLevel()
	:	m_pPlayerCollisionComponent(NULL)
{
	m_levelObjects.reserve(64);
}

DroidRunnerLevel::~DroidRunnerLevel()
{
	for (unsigned int i=0; i<m_levelObjects.size(); ++i)
	{
		delete m_levelObjects[i];
	}
	m_levelObjects.clear();

	Framework::Renderer* pRenderer = Framework::Renderer::GetSingletonPtr();
	if (pRenderer)
	{
		pRenderer->RemoveShader(&m_transformShader);
	}
}

void DroidRunnerLevel::Initialize(const Vector3& origin)
{
	m_sphereGeometry.SetVertexBuffer(sphereVerts);
	m_sphereGeometry.SetNumVertices(sizeof(sphereVerts) / sizeof(sphereVerts[0]));
	m_sphereGeometry.SetIndexBuffer(sphereIndices);
	m_sphereGeometry.SetNumIndices(sizeof(sphereIndices) / sizeof(sphereIndices[0]));
	m_sphereGeometry.SetName("android");

	m_sphereGeometry.SetNumVertexPositionElements(3);
	m_sphereGeometry.SetVertexStride(0);

	m_cubeGeometry.SetVertexBuffer(cubeVerts);
	m_cubeGeometry.SetNumVertices(sizeof(cubeVerts) / sizeof(cubeVerts[0]));
	m_cubeGeometry.SetIndexBuffer(cubeIndices);
	m_cubeGeometry.SetNumIndices(sizeof(cubeIndices) / sizeof(cubeIndices[0]));
	m_cubeGeometry.SetName("cube");

	m_cubeGeometry.SetNumVertexPositionElements(3);
	m_cubeGeometry.SetVertexStride(0);

	m_origin.Set(origin);

	CollisionManager::GetSingleton().AddCollisionBin();
	const Vector3 min(-3.0f, -3.0f, -3.0f);
	const Vector3 max(3.0f, 3.0f, 3.0f);

	const unsigned char tiles[] =
	{
			EMPTY,	EMPTY,	EMPTY,	EMPTY,	AI,		AI,		AI,		AI,
			EMPTY,	EMPTY,	EMPTY,	EMPTY,	BOX,	BOX,	BOX,	BOX,
			EMPTY,	PLAYER,	EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY,	EMPTY,
			BOX,	BOX,	BOX,	BOX,	BOX,	BOX,	BOX,	BOX
	};

	const unsigned int numTiles = sizeof(tiles) / sizeof(tiles[0]);

	const unsigned int numRows	= 4;
	const unsigned int rowWidth = numTiles / numRows;

	for (unsigned int i=0; i<numTiles; ++i)
	{
		if (tiles[i] == BOX)
		{
			const unsigned int row		= i / rowWidth;
			const unsigned int column	= i % rowWidth;

			GameObject* pNewObject = new GameObject();

			SetObjectPosition(pNewObject, row, column);
			AddCollisionComponent(pNewObject, min, max);

			Vector4 color(0.0f, 0.0f, 1.0f, 1.0f);
			AddRenderableComponent(pNewObject, m_cubeGeometry, m_transformShader, color);

			m_levelObjects.push_back(pNewObject);
		}
		else if (tiles[i] == PLAYER)
		{
			const unsigned int row		= i / rowWidth;
			const unsigned int column	= i % rowWidth;

			GameObject* pNewObject = new GameObject();

			SetObjectPosition(pNewObject, row, column);
			AddMovementComponent(pNewObject);

			AddCollisionComponent(pNewObject, min, max);

			MovementComponent* pMovementComponent = component_cast<MovementComponent>(pNewObject);
			m_pPlayerCollisionComponent = component_cast<CollisionComponent>(pNewObject);
			if (pMovementComponent && m_pPlayerCollisionComponent)
			{
				m_pPlayerCollisionComponent->AddEventListener(pMovementComponent);
			}

			Vector4 color(0.0f, 1.0f, 0.0f, 1.0f);
			AddRenderableComponent(pNewObject, m_sphereGeometry, m_transformShader, color);

			m_levelObjects.push_back(pNewObject);
		}
		else if (tiles[i] == AI)
		{
			const unsigned int row			= i / rowWidth;
			const unsigned int column		= i % rowWidth;

			unsigned int patrolEndRow		= 0;
			unsigned int patrolEndColumn	= 0;

			for (unsigned int j=i; j<numTiles; ++j)
			{
				if (tiles[j] != AI)
				{
					i = j;

					--j;
					patrolEndRow			= j / rowWidth;
					patrolEndColumn			= j % rowWidth;
					break;
				}
			}

			GameObject* pNewObject = new GameObject();

			SetObjectPosition(pNewObject, row, column);
			AddCollisionComponent(pNewObject, min, max);

			AddPatrolComponent(pNewObject, row, column, patrolEndRow, patrolEndColumn);

			Vector4 color(1.0f, 0.0f, 0.0f, 1.0f);
			AddRenderableComponent(pNewObject, m_sphereGeometry, m_transformShader, color);
		}
	}

	Renderer* pRenderer = Renderer::GetSingletonPtr();
	if (pRenderer)
	{
		pRenderer->AddShader(&m_transformShader);
	}
}

void DroidRunnerLevel::SetObjectPosition(Framework::GameObject* pObject, const unsigned int row, const unsigned int column)
{
	assert(pObject);
	pObject->AddComponent<TransformComponent>();
	TransformComponent* pTransformComponent = component_cast<TransformComponent>(pObject);
	if (pTransformComponent)
	{
		Vector3 translation(m_origin);
		translation.m_x += TILE_WIDTH * column;
		translation.m_y -= TILE_HEIGHT * row;
		pTransformComponent->GetTransform().SetTranslation(translation);
	}
}

void DroidRunnerLevel::AddMovementComponent(GameObject* pObject)
{
	assert(pObject);
	pObject->AddComponent<MovementComponent>();
	MovementComponent* pMovementComponent = component_cast<MovementComponent>(pObject);
	if (pMovementComponent)
	{
		AttachEvent(JUMP_EVENT, *pMovementComponent);
		AttachEvent(UPDATE_EVENT, *pMovementComponent);
	}
}

void DroidRunnerLevel::AddCollisionComponent(Framework::GameObject* pObject, const Framework::Vector3& min, const Framework::Vector3& max)
{
	assert(pObject);
	pObject->AddComponent<CollisionComponent>();
	CollisionComponent* pCollisionComponent = component_cast<CollisionComponent>(pObject);
	if (pCollisionComponent)
	{
		pCollisionComponent->SetMin(min);
		pCollisionComponent->SetMax(max);
		AttachEvent(COLLISION_EVENT, *pCollisionComponent);
		CollisionManager::GetSingleton().AddObjectToBin(0, pCollisionComponent);
	}
}

void DroidRunnerLevel::AddPatrolComponent(Framework::GameObject* pObject, const unsigned int startRow, const unsigned int startColumn, const unsigned int endRow, const unsigned int endColumn)
{
	assert(pObject);
	pObject->AddComponent<PatrolComponent>();
	PatrolComponent* pPatrolComponent = component_cast<PatrolComponent>(pObject);
	if (pPatrolComponent)
	{
		Vector3 startPoint(m_origin);
		startPoint.m_x += TILE_WIDTH * startColumn;
		startPoint.m_y -= TILE_HEIGHT * startRow;

		Vector3 endPoint(m_origin);
		endPoint.m_x += TILE_WIDTH * endColumn;
		endPoint.m_y -= TILE_HEIGHT * endRow;

		pPatrolComponent->SetStartPoint(startPoint);
		pPatrolComponent->SetEndPoint(endPoint);
		pPatrolComponent->SetSpeed(12.0f);

		AttachEvent(UPDATE_EVENT, *pPatrolComponent);
	}
}

void DroidRunnerLevel::AddRenderableComponent(GameObject* pObject, Geometry& geometry, Shader& shader, Vector4& color)
{
	assert(pObject);
	pObject->AddComponent<RenderableComponent>();
	RenderableComponent* pRenderableComponent = component_cast<RenderableComponent>(pObject);
	if (pRenderableComponent)
	{
		Renderable& renderable = pRenderableComponent->GetRenderable();
		renderable.SetGeometry(&geometry);
		renderable.SetShader(&shader);
		Vector4& renderableColor = renderable.GetColor();
		renderableColor.Set(color);
		Framework::AttachEvent(Framework::RENDER_EVENT, *pRenderableComponent);
	}
}

void DroidRunnerLevel::HandleEvent(Event* pEvent)
{
	if (pEvent->GetID() == POSTUPDATE_EVENT && m_pPlayerCollisionComponent)
	{
		CollisionManager::GetSingleton().TestAgainstBin(0, m_pPlayerCollisionComponent);
	}
}
