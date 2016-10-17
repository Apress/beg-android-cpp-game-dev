#ifndef CHAPTER6TASK_H_
#define CHAPTER6TASK_H_

#include "Framework/Kernel/Task.h"
#include "Framework/Renderer/Geometry/Geometry.h"
#include "Framework/Renderer/Textures/Texture.h"
#include "Framework/Renderer/Shaders/TransformShader.h"
#include "Framework/Renderer/Renderable.h"
#include "Framework/Utilities/File.h"
#include "Framework/GameObjects/GameObject.h"

class Chapter6Task
	:	public Framework::Task
{
private:
	Framework::Geometry			m_geometry;
	Framework::TransformShader	m_transformShader;

	Framework::GameObject		m_playerObject;
	Framework::GameObject		m_aiObject;

public:
	Chapter6Task(const unsigned int priority);
	virtual ~Chapter6Task();

	// From Task
	virtual bool	Start();
	virtual void	OnSuspend();
	virtual void	Update();
	virtual void	OnResume();
	virtual void	Stop();
};

#endif // CHAPTER6TASK_H_
