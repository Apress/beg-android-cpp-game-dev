#ifndef DROIDRUNNERLEVEL_H_
#define DROIDRUNNERLEVEL_H_

#include "Framework/EventManager/EventHandler.h"
#include <vector>
#include "Framework/GameObjects/GameObject.h"
#include "Framework/Math/Vector3.h"

#include "Framework/Renderer/Geometry/Geometry.h"
#include "Framework/Renderer/Textures/Texture.h"
#include "Framework/Renderer/Shaders/TransformAmbientShader.h"
#include "Framework/Renderer/Shaders/TransformAmbientDiffuseShader.h"
#include "Framework/Renderer/Shaders/TransformAmbientDiffuseSpecularShader.h"
#include "Framework/Renderer/Renderable.h"
#include "Framework/GameObjects/GameObject.h"


class MovementComponent;
namespace Framework
{
	class TransformComponent;
	class CollisionComponent;
}

class DroidRunnerLevel
	:	public Framework::EventHandler
{
private:

	MovementComponent* 									m_pPlayerMovementComponent;
	Framework::TransformComponent* 						m_pPlayerTransformComponent;
	Framework::CollisionComponent*						m_pPlayerCollisionComponent;

	Framework::Geometry									m_sphereGeometry;
	Framework::Geometry									m_cubeGeometry;
	Framework::TransformAmbientDiffuseSpecularShader	m_shader;

	Framework::Material									m_playerMaterial;
	Framework::Material									m_aiMaterial;
	Framework::Material									m_boxMaterial;

	enum TileTypes
	{
		EMPTY = 0,
		BOX,
		AI,
		PLAYER
	};

	typedef std::vector<Framework::GameObject*>	GameObjectVector;
	typedef GameObjectVector::iterator			GameObjectVectorIterator;

	GameObjectVector							m_levelObjects;

	void SetObjectPosition(Framework::GameObject* pObject, const unsigned int row, const unsigned int column);
	void AddMovementComponent(Framework::GameObject* pObject);
	void AddCollisionComponent(Framework::GameObject* pObject, const Framework::Vector3& min, const Framework::Vector3& max);
	void AddPatrolComponent(Framework::GameObject* pObject, const unsigned int startRow, const unsigned int startColumn, const unsigned int endRow, const unsigned int endColumn);
	void AddRenderableComponent(Framework::GameObject* pObject, Framework::Geometry& geometry, Framework::Material& material);

	static const float							TILE_WIDTH	= 6.0f;
	static const float							TILE_HEIGHT = 6.0f;

	Framework::Vector3							m_origin;
	float										m_levelEnd;

	bool										m_initialized;
	bool										m_levelBuilt;

	void CleanLevel();
	void BuildLevelData();

public:
	DroidRunnerLevel();
	~DroidRunnerLevel();

	void Initialize(const Framework::Vector3& origin);

	virtual void HandleEvent(Framework::Event* pEvent);

	bool	IsInitialized() const	{ return m_initialized; }
};

#endif // DROIDRUNNERLEVEL_H_
