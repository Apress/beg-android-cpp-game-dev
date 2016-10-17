#ifndef CHAPTER5TASK_H_
#define CHAPTER5TASK_H_

#include "Framework/Kernel/Task.h"
#include "Framework/Renderer/Geometry/Geometry.h"
#include "Framework/Renderer/Textures/Texture.h"
#include "Framework/Renderer/Shaders/TextureShader.h"
#include "Framework/Renderer/Renderable.h"
#include "Framework/Utilities/File.h"

namespace Framework
{
	class Renderer;
}

class Chapter5Task
	:	public Framework::Task
{
private:
	enum State
	{
		LOADING_FILE,
		CREATE_TEXTURE,
		RUNNING
	};

	State						m_state;

	Framework::File				m_file;
	Framework::Renderer*		m_pRenderer;
	Framework::Geometry			m_geometry;
	Framework::TextureShader	m_textureShader;
	Framework::Renderable		m_renderable;
	Framework::Texture			m_texture;

	void*					m_pTGABuffer;
	unsigned int			m_readBytes;
	unsigned int			m_fileLength;

public:
	Chapter5Task(Framework::Renderer* pRenderer, const unsigned int priority);
	virtual ~Chapter5Task();

	// From Task
	virtual bool	Start();
	virtual void	OnSuspend();
	virtual void	Update();
	virtual void	OnResume();
	virtual void	Stop();
};

#endif // CHAPTER5TASK_H_
