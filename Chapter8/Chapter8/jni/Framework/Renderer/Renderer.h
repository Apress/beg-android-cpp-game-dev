#ifndef RENDERER_H_
#define RENDERER_H_

#include  "../Kernel/Task.h"
#include "../Utilities/Singleton.h"
#include "../Math/Matrix4.h"
#include <android_native_app_glue.h>
#include <EGL/egl.h>
#include <vector>
#include "../Math/Plane.h"
#include "../EventManager/EventHandler.h"

namespace Framework
{
	class Shader;
	class Texture;
	class Renderable;

	class Renderer
		:	public Task
		,	public EventHandler
		,	public Singleton<Renderer>
	{
	public:
		enum FrustumParameters
		{
			TOP,
			BOTTOM,
			RIGHT,
			LEFT,
			NEAR,
			FAR,
			NUM_PARAMS
		};

	private:
		android_app*	m_pState;
		EGLDisplay		m_display;
		EGLContext		m_context;
		EGLSurface		m_surface;
		int				m_width;
		int				m_height;
		bool			m_initialized;
		bool			m_paused;

		typedef std::vector<Shader*>		ShaderVector;
		typedef ShaderVector::iterator		ShaderVectorIterator;

		typedef std::vector<Texture*>		TextureVector;
		typedef TextureVector::iterator		TextureVectorIterator;

		typedef std::vector<Renderable*>	RenderableVector;
		typedef RenderableVector::iterator	RenderableVectorIterator;

		RenderableVector	m_renderables;
		TextureVector		m_textures;
		ShaderVector		m_shaders;

		float				m_frustumParameters[NUM_PARAMS];

		Matrix4				m_cameraMatrix;
		Matrix4				m_viewMatrix;
		Matrix4				m_projectionMatrix;

		void Draw(Renderable* pRenderable);
		void BuildFrustumPlanes(Plane frustumPlanes[]);
		bool ShouldDraw(Renderable* pRenderable, Plane frustumPlanes[]) const;

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

		virtual void	HandleEvent(Event* event);

		bool IsInitialized() { return m_initialized; }

		void			SetCameraMatrix(const Matrix4& cameraMatrix)			{ m_cameraMatrix = cameraMatrix; }
		const Matrix4&	GetCameraMatrix() const									{ return m_cameraMatrix; }

		void			SetViewMatrix(const Matrix4& viewMatrix)				{ m_viewMatrix = viewMatrix; }
		const Matrix4&	GetViewMatrix() const									{ return m_viewMatrix; }

		void			SetFrustum(const float frustumParameters[]);
		const Matrix4&	GetProjectionMatrix() const								{ return m_projectionMatrix; }

		int				GetWidth() const										{ return m_width; }
		int				GetHeight() const										{ return m_height; }
	};
}

#endif // RENDERER_H_
