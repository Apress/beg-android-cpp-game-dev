#include "Renderer.h"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <cassert>
#include "Renderable.h"
#include "Geometry/Geometry.h"
#include "Textures/Texture.h"
#include "Shaders/Shader.h"

namespace Framework
{
	Renderer::Renderer(android_app* pState, const unsigned int priority)
		:	Task(priority)
		,	m_display(EGL_NO_DISPLAY)
		,	m_context(EGL_NO_CONTEXT)
		,	m_surface(EGL_NO_SURFACE)
		,	m_width(0.0f)
		,	m_height(0.0f)
	{
		m_pState = pState;
		m_renderables.reserve(16);
	}

	Renderer::~Renderer()
	{
	}

	void Renderer::Init()
	{
		// initialize OpenGL ES and EGL

		/*
		 * Here specify the attributes of the desired configuration.
		 * Below, we select an EGLConfig with at least 8 bits per color
		 * component compatible with on-screen windows
		 */
		const EGLint attribs[] =
		{
				EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
				EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
				EGL_BLUE_SIZE, 8,
				EGL_GREEN_SIZE, 8,
				EGL_RED_SIZE, 8,
				EGL_NONE
		};

		EGLint format;
		EGLint numConfigs;
		EGLConfig config;

		m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

		eglInitialize(m_display, NULL, NULL);

		/* Here, the application chooses the configuration it desires. In this
		 * sample, we have a very simplified selection process, where we pick
		 * the first EGLConfig that matches our criteria */
		eglChooseConfig(m_display, attribs, &config, 1, &numConfigs);

		/* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
		 * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
		 * As soon as we picked a EGLConfig, we can safely reconfigure the
		 * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
		eglGetConfigAttrib(m_display, config, EGL_NATIVE_VISUAL_ID, &format);

		ANativeWindow_setBuffersGeometry(m_pState->window, 0, 0, format);

		m_surface = eglCreateWindowSurface(m_display, config, m_pState->window, NULL);

		EGLint contextAttribs[] =
		{
				EGL_CONTEXT_CLIENT_VERSION, 2,
				EGL_NONE
		};
		m_context = eglCreateContext(m_display, config, NULL, contextAttribs);

		eglMakeCurrent(m_display, m_surface, m_surface, m_context);

		eglQuerySurface(m_display, m_surface, EGL_WIDTH, &m_width);
		eglQuerySurface(m_display, m_surface, EGL_HEIGHT, &m_height);

		for (TextureVectorIterator iter = m_textures.begin(); iter != m_textures.end(); ++iter)
		{
			Texture* pCurrent = *iter;
			pCurrent->Init();
		}

		for (ShaderVectorIterator iter = m_shaders.begin(); iter != m_shaders.end(); ++iter)
		{
			Shader* pCurrent = *iter;
			pCurrent->Link();
		}

		m_initialized = true;
	}

	void Renderer::Destroy()
	{
		m_initialized = false;

		if (m_display != EGL_NO_DISPLAY)
		{
			eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
			if (m_context != EGL_NO_CONTEXT)
			{
				eglDestroyContext(m_display, m_context);
			}
			if (m_surface != EGL_NO_SURFACE)
			{
				eglDestroySurface(m_display, m_surface);
			}
			eglTerminate(m_display);
		}
		m_display = EGL_NO_DISPLAY;
		m_context = EGL_NO_CONTEXT;
		m_surface = EGL_NO_SURFACE;
	}

	void Renderer::AddRenderable(Renderable* pRenderable)
	{
		m_renderables.push_back(pRenderable);
	}

	void Renderer::RemoveRenderable(Renderable* pRenderable)
	{
		for (RenderableVectorIterator iter = m_renderables.begin(); iter != m_renderables.end(); ++iter)
		{
			Renderable* pCurrent = *iter;
			if (pCurrent == pRenderable)
			{
				m_renderables.erase(iter);
				break;
			}
		}
	}

	void Renderer::AddShader(Shader* pShader)
	{
		assert(pShader);
		if (m_initialized)
		{
			pShader->Link();
		}
		m_shaders.push_back(pShader);
	}

	void Renderer::RemoveShader(Shader* pShader)
	{
		for (ShaderVectorIterator iter = m_shaders.begin(); iter != m_shaders.end(); ++iter)
		{
			Shader* pCurrent = *iter;
			if (pCurrent == pShader)
			{
				m_shaders.erase(iter);
				break;
			}
		}
	}

	void Renderer::AddTexture(Texture* pTexture)
	{
		assert(pTexture);
		if (m_initialized)
		{
			pTexture->Init();
		}
		m_textures.push_back(pTexture);
	}

	void Renderer::RemoveTexture(Texture* pTexture)
	{
		for (TextureVectorIterator iter = m_textures.begin(); iter != m_textures.end(); ++iter)
		{
			Texture* pCurrent = *iter;
			if (pCurrent == pTexture)
			{
				m_textures.erase(iter);
				break;
			}
		}
	}

	bool Renderer::Start()
	{
		return true;
	}

	void Renderer::OnSuspend()
	{

	}

	void Renderer::Draw(Renderable* pRenderable)
	{
		assert(pRenderable);
		if (pRenderable)
		{
			Geometry* pGeometry = pRenderable->GetGeometry();
			Shader* pShader = pRenderable->GetShader();
			assert(pShader && pGeometry);
			if (pShader && pGeometry)
			{
				pShader->Setup(*pRenderable);

				glDrawElements(GL_TRIANGLES, pGeometry->GetNumIndices(), GL_UNSIGNED_SHORT, pGeometry->GetIndexBuffer());
			}
		}
	}

	void Renderer::Update()
	{
		if (m_initialized)
		{
			glClearColor(0.95f, 0.95f, 0.95f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (RenderableVectorIterator iter = m_renderables.begin(); iter != m_renderables.end(); ++iter)
			{
				Renderable* pRenderable = *iter;
				if (pRenderable)
				{
					Draw(pRenderable);
				}
			}

			eglSwapBuffers(m_display, m_surface);
		}
	}

	void Renderer::OnResume()
	{

	}

	void Renderer::Stop()
	{

	}
}
