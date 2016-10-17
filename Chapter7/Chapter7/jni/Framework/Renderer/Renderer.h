#ifndef RENDERER_H_
#define RENDERER_H_

#include  "../Kernel/Task.h"
#include "../Utilities/Singleton.h"
#include <android_native_app_glue.h>
#include <EGL/egl.h>
#include <vector>

namespace Framework
{
	class Shader;
	class Texture;
	class Renderable;

	class Renderer
		:	public Task
		,	public Singleton<Renderer>
	{
	private:
		android_app*	m_pState;
		EGLDisplay		m_display;
		EGLContext		m_context;
		EGLSurface		m_surface;
		int				m_width;
		int				m_height;
		bool			m_initialized;

		typedef std::vector<Shader*>		ShaderVector;
		typedef ShaderVector::iterator		ShaderVectorIterator;

		typedef std::vector<Texture*>		TextureVector;
		typedef TextureVector::iterator		TextureVectorIterator;

		typedef std::vector<Renderable*>	RenderableVector;
		typedef RenderableVector::iterator	RenderableVectorIterator;

		RenderableVector	m_renderables;
		TextureVector		m_textures;
		ShaderVector		m_shaders;

		void Draw(Renderable* pRenderable);

	public:
		explicit Renderer(android_app* pState, const unsigned int priority);
		virtual ~Renderer();

		void Init();
		void Destroy();

		void AddRenderable(Renderable* pRenderable);

		void AddShader(Shader* pShader);
		void RemoveShader(Shader* pShader);

		void AddTexture(Texture* pTexture);
		void RemoveTexture(Texture* pTexture);

		// From Task
		virtual bool	Start();
		virtual void	OnSuspend();
		virtual void	Update();
		virtual void	OnResume();
		virtual void	Stop();

		bool IsInitialized() { return m_initialized; }
	};
}

#endif // RENDERER_H_
