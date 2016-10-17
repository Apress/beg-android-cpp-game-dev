#include "DroidRunnerLevel.h"
#include "Framework/Renderer/Renderer.h"
#include "Framework/GameObjects/Components/TransformComponent.h"
#include "MovementComponent.h"
#include "PatrolComponent.h"
#include "Framework/GameObjects/Components/RenderableComponent.h"
#include "Framework/GameObjects/Components/CollisionComponent.h"
#include "Framework/GameObjects/Components/CameraComponent.h"
#include "Framework/GameObjects/Components/BoundObjectComponent.h"
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
	:	m_pPlayerMovementComponent(NULL)
	,	m_pPlayerTransformComponent(NULL)
	,	m_pPlayerCollisionComponent(NULL)
	,	m_levelEnd(0.0f)
	,	m_initialized(false)
	,	m_levelBuilt(false)
{
	m_levelObjects.reserve(64);

	RegisterEvent(PLAYERJUMP_EVENT);
}

DroidRunnerLevel::~DroidRunnerLevel()
{
	CleanLevel();

	AudioManager* pAudioManager = AudioManager::GetSingletonPtr();
	if (pAudioManager)
	{
		pAudioManager->DestroySFX(m_explosionHandle);
		pAudioManager->DestroySFX(m_jumpHandle);
	}

	Framework::Renderer* pRenderer = Framework::Renderer::GetSingletonPtr();
	if (pRenderer)
	{
		pRenderer->RemoveShader(&m_shader);
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
	m_sphereGeometry.SetNumNormalPositionElements(3);
	m_sphereGeometry.SetVertexStride(sizeof(float) * 6);

	m_cubeGeometry.SetVertexBuffer(cubeVerts);
	m_cubeGeometry.SetNumVertices(sizeof(cubeVerts) / sizeof(cubeVerts[0]));
	m_cubeGeometry.SetIndexBuffer(cubeIndices);
	m_cubeGeometry.SetNumIndices(sizeof(cubeIndices) / sizeof(cubeIndices[0]));
	m_cubeGeometry.SetName("cube");

	m_cubeGeometry.SetNumVertexPositionElements(3);
	m_cubeGeometry.SetNumNormalPositionElements(3);
	m_cubeGeometry.SetVertexStride(sizeof(float) * 6);

	m_origin.Set(origin);

	m_playerMaterial.SetShader(&m_shader);
	Vector4 playerColor(0.0f, 1.0f, 0.0f, 1.0f);
	m_playerMaterial.SetAmbientColor(playerColor);
	m_playerMaterial.SetDiffuseColor(playerColor);

	float specExponent = 192.0f;

	Vector4 playerSpecColor(0.0f, 1.0f, 0.0f, 1.0f);
	m_playerMaterial.SetSpecularColor(playerSpecColor);
	m_playerMaterial.SetSpecularExponent(specExponent);

	m_aiMaterial.SetShader(&m_shader);
	Vector4 aiColor(1.0f, 0.0f, 0.0f, 1.0f);
	m_aiMaterial.SetAmbientColor(aiColor);
	m_aiMaterial.SetDiffuseColor(aiColor);
	Vector4 aiSpecColor(1.0f, 0.0f, 0.0f, 1.0f);
	m_aiMaterial.SetSpecularColor(aiSpecColor);
	m_aiMaterial.SetSpecularExponent(specExponent);

	m_boxMaterial.SetShader(&m_shader);
	Vector4 boxColor(0.0f, 0.0f, 1.0f, 1.0f);
	m_boxMaterial.SetAmbientColor(boxColor);
	m_boxMaterial.SetDiffuseColor(boxColor);
	Vector4 boxSpecColor(0.0f, 0.0f, 1.0f, 1.0f);
	m_boxMaterial.SetSpecularColor(boxSpecColor);
	m_boxMaterial.SetSpecularExponent(specExponent);

	CollisionManager::GetSingleton().AddCollisionBin();

	BuildLevelData();

	Renderer* pRenderer = Renderer::GetSingletonPtr();
	if (pRenderer)
	{
		pRenderer->AddShader(&m_shader);
	}

	AudioManager& audioManager = AudioManager::GetSingleton();
	std::string jumpEffectName("sounds/jump.ogg");
	m_jumpHandle = audioManager.CreateSFX(jumpEffectName);
	AttachEvent(PLAYERJUMP_EVENT, *this);

	std::string explosionEffectName("sounds/impact.ogg");
	m_explosionHandle = audioManager.CreateSFX(explosionEffectName);

	m_initialized = true;
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
		pPatrolComponent->SetSpeed(8.0f);

		AttachEvent(UPDATE_EVENT, *pPatrolComponent);
	}
}

void DroidRunnerLevel::AddRenderableComponent(GameObject* pObject, Geometry& geometry, Material& material)
{
	assert(pObject);
	pObject->AddComponent<RenderableComponent>();
	RenderableComponent* pRenderableComponent = component_cast<RenderableComponent>(pObject);
	if (pRenderableComponent)
	{
		Renderable& renderable = pRenderableComponent->GetRenderable();
		renderable.SetGeometry(&geometry);
		renderable.SetMaterial(&material);
		Framework::AttachEvent(Framework::RENDER_EVENT, *pRenderableComponent);
	}
}

void DroidRunnerLevel::HandleEvent(Event* pEvent)
{
	if (pEvent->GetID() == POSTUPDATE_EVENT)
	{
		bool endLevel = false;

		if (m_pPlayerTransformComponent)
		{
			if (m_pPlayerTransformComponent->GetTransform().GetTranslation().m_x > m_levelEnd)
			{
				endLevel = true;
			}
		}

		if (m_pPlayerMovementComponent && m_pPlayerMovementComponent->IsDead())
		{
			AudioManager* pAudioManager = AudioManager::GetSingletonPtr();
			pAudioManager->PlaySFX(m_explosionHandle);
			endLevel = true;
		}

		if (endLevel)
		{
			CleanLevel();
			BuildLevelData();
		}

		if (m_pPlayerCollisionComponent)
		{
			CollisionManager::GetSingleton().TestAgainstBin(0, m_pPlayerCollisionComponent);
		}
	}
	else if (pEvent->GetID() == PLAYERJUMP_EVENT)
	{
		AudioManager* pAudioManager = AudioManager::GetSingletonPtr();
		pAudioManager->PlaySFX(m_jumpHandle);
	}
}

void DroidRunnerLevel::CleanLevel()
{
	if (m_levelBuilt)
	{
		CollisionManager* pCollisionManager = CollisionManager::GetSingletonPtr();
		if (pCollisionManager)
		{
			CollisionManager::GetSingleton().ClearBin(0);
		}

		m_pPlayerTransformComponent = NULL;
		m_pPlayerMovementComponent	= NULL;
		m_pPlayerCollisionComponent = NULL;

		for (unsigned int i=0; i<m_levelObjects.size(); ++i)
		{
			GameObject* pObject = m_levelObjects[i];

			MovementComponent* pMovementComponent = component_cast<MovementComponent>(pObject);
			if (pMovementComponent)
			{
				DetachEvent(JUMP_EVENT, *pMovementComponent);
				DetachEvent(UPDATE_EVENT, *pMovementComponent);
			}

			CollisionComponent* pCollisionComponent = component_cast<CollisionComponent>(pObject);
			if (pCollisionComponent)
			{
				DetachEvent(COLLISION_EVENT, *pCollisionComponent);
			}

			PatrolComponent* pPatrolComponent = component_cast<PatrolComponent>(pObject);
			if (pPatrolComponent)
			{
				DetachEvent(UPDATE_EVENT, *pPatrolComponent);
			}

			RenderableComponent* pRenderableComponent = component_cast<RenderableComponent>(pObject);
			if (pRenderableComponent)
			{
				DetachEvent(RENDER_EVENT, *pRenderableComponent);
			}

			BoundObjectComponent* pBoundObjectComponent = component_cast<BoundObjectComponent>(pObject);
			if (pBoundObjectComponent)
			{
				DetachEvent(UPDATE_EVENT, *pBoundObjectComponent);
			}

			CameraComponent* pCameraComponent = component_cast<CameraComponent>(pObject);
			if (pCameraComponent)
			{
				DetachEvent(POSTUPDATE_EVENT, *pCameraComponent);
			}

			delete pObject;
		}
		m_levelObjects.clear();

		m_levelBuilt = false;
	}
}

void DroidRunnerLevel::BuildLevelData()
{
	assert(m_levelBuilt == false);

	const Vector3 min(-3.0f, -3.0f, -3.0f);
	const Vector3 max(3.0f, 3.0f, 3.0f);

	const unsigned char tiles[] =
	{
#include "Tiles.h"
	};

	const unsigned int numTiles = sizeof(tiles) / sizeof(tiles[0]);

	const unsigned int numRows	= 4;
	const unsigned int rowWidth = numTiles / numRows;

	m_levelEnd = (rowWidth * TILE_WIDTH) + m_origin.m_x;

	for (unsigned int i=0; i<numTiles; ++i)
	{
		if (tiles[i] == BOX)
		{
			const unsigned int row		= i / rowWidth;
			const unsigned int column	= i % rowWidth;

			GameObject* pNewObject = new GameObject();

			SetObjectPosition(pNewObject, row, column);
			AddCollisionComponent(pNewObject, min, max);

			AddRenderableComponent(pNewObject, m_cubeGeometry, m_boxMaterial);

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

			m_pPlayerMovementComponent	= component_cast<MovementComponent>(pNewObject);
			m_pPlayerCollisionComponent = component_cast<CollisionComponent>(pNewObject);
			if (m_pPlayerMovementComponent && m_pPlayerCollisionComponent)
			{
				m_pPlayerCollisionComponent->AddEventListener(m_pPlayerMovementComponent);
			}

			m_pPlayerTransformComponent = component_cast<TransformComponent>(pNewObject);

			AddRenderableComponent(pNewObject, m_sphereGeometry, m_playerMaterial);

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

			AddRenderableComponent(pNewObject, m_sphereGeometry, m_aiMaterial);

			m_levelObjects.push_back(pNewObject);
		}
	}

	// Create a camera object
	GameObject* pCameraObject = new GameObject();

	pCameraObject->AddComponent<TransformComponent>();

	pCameraObject->AddComponent<BoundObjectComponent>();
	BoundObjectComponent* pBoundObjectComponent = component_cast<BoundObjectComponent>(pCameraObject);
	assert(pBoundObjectComponent);
	pBoundObjectComponent->SetBoundObject(m_pPlayerTransformComponent);
	pBoundObjectComponent->GetTransform().SetTranslation(Vector3(6.0f, 4.25f, -45.0f));
	AttachEvent(UPDATE_EVENT, *pBoundObjectComponent);

	pCameraObject->AddComponent<CameraComponent>();
	CameraComponent* pCameraComponent = component_cast<CameraComponent>(pCameraObject);
	assert(pCameraComponent);
	Renderer& renderer = Renderer::GetSingleton();
	float width = static_cast<float>(renderer.GetWidth());
	float height = static_cast<float>(renderer.GetHeight());
	pCameraComponent->SetFrustum(35.0f, width / height, 1.0f, 100.0f);
	AttachEvent(POSTUPDATE_EVENT, *pCameraComponent);

	Vector4 ambientLightColor(0.0f, 0.2f, 0.2f, 1.0f);
	renderer.SetAmbientLightColor(ambientLightColor);

	Vector4 diffuseLightColor(0.5f, 0.5f, 0.5f, 1.0f);
	renderer.SetDiffuseLightColor(diffuseLightColor);

	Vector3 lightDirection(1.0f, 1.0f, -1.0f);
	lightDirection.Normalize();
	renderer.SetLightDirection(lightDirection);

	Vector4 specularLightColor(1.0f, 1.0f, 1.0f, 1.0f);
	renderer.SetSpecularLightColor(specularLightColor);

	m_levelObjects.push_back(pCameraObject);

	m_levelBuilt = true;
}
