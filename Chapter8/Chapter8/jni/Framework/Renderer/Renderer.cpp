#include "Renderer.h"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <cassert>
#include "Renderable.h"
#include "Geometry/Geometry.h"
#include "Textures/Texture.h"
#include "Shaders/Shader.h"
#include "../EventManager/EventManager.h"

namespace Framework
{
	Renderer::Renderer(android_app* pState, const unsigned int priority)
		:	Task(priority)
		,	m_display(EGL_NO_DISPLAY)
		,	m_context(EGL_NO_CONTEXT)
		,	m_surface(EGL_NO_SURFACE)
		,	m_width(0.0f)
		,	m_height(0.0f)
		,	m_initialized(false)
		,	m_paused(false)
	{
		m_pState = pState;
		m_renderables.reserve(16);

		m_viewMatrix.Identify();
		m_projectionMatrix.Identify();

		memset(m_frustumParameters, 0, sizeof(float) * NUM_PARAMS);
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
				EGL_DEPTH_SIZE, 24,
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
		AttachEvent(PAUSEAPP_EVENT, *this);
		AttachEvent(RESUMEAPP_EVENT, *this);

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

	void Renderer::BuildFrustumPlanes(Plane frustumPlanes[])
	{
		// Get the camera orientation vectors and position as Vector3
		Vector3 cameraRight(m_cameraMatrix.m_m[0], m_cameraMatrix.m_m[1], m_cameraMatrix.m_m[2]);
		Vector3 cameraUp(m_cameraMatrix.m_m[4], m_cameraMatrix.m_m[5], m_cameraMatrix.m_m[6]);
		Vector3 cameraForward(m_cameraMatrix.m_m[8], m_cameraMatrix.m_m[9], m_cameraMatrix.m_m[10]);
		Vector3 cameraPosition(m_cameraMatrix.m_m[12], m_cameraMatrix.m_m[13], m_cameraMatrix.m_m[14]);

		// Calculate the center of the near plane
		Vector3 nearCenter = cameraForward;
		nearCenter.Multiply(m_frustumParameters[NEAR]);
		nearCenter.Add(cameraPosition);

		// Calculate the center of the far plane
		Vector3 farCenter = cameraForward;
		farCenter.Multiply(m_frustumParameters[FAR]);
		farCenter.Add(cameraPosition);

		// Calculate the normal for the top plane
		Vector3 towardsTop = cameraUp;
		towardsTop.Multiply(m_frustumParameters[TOP]);
		towardsTop.Add(nearCenter);
		towardsTop.Subtract(cameraPosition);
		towardsTop.Normalize();
		towardsTop = cameraRight.Cross(towardsTop);
		frustumPlanes[TOP].BuildPlane(cameraPosition, towardsTop);

		// Calculate the normal for the bottom plane
		Vector3 towardsBottom = cameraUp;
		towardsBottom.Multiply(m_frustumParameters[BOTTOM]);
		towardsBottom.Add(nearCenter);
		towardsBottom.Subtract(cameraPosition);
		towardsBottom.Normalize();
		towardsBottom = towardsBottom.Cross(cameraRight);
		frustumPlanes[BOTTOM].BuildPlane(cameraPosition, towardsBottom);

		// Calculate the normal for the right plane
		Vector3 towardsRight = cameraRight;
		towardsRight.Multiply(m_frustumParameters[RIGHT]);
		towardsRight.Add(nearCenter);
		towardsRight.Subtract(cameraPosition);
		towardsRight.Normalize();
		towardsRight = towardsRight.Cross(cameraUp);
		frustumPlanes[RIGHT].BuildPlane(cameraPosition, towardsRight);

		// Calculate the normal for the left plane
		Vector3 towardsLeft = cameraRight;
		towardsLeft.Multiply(m_frustumParameters[LEFT]);
		towardsLeft.Add(nearCenter);
		towardsLeft.Subtract(cameraPosition);
		towardsLeft.Normalize();
		towardsLeft = cameraUp.Cross(towardsLeft);
		frustumPlanes[LEFT].BuildPlane(cameraPosition, towardsLeft);

		Vector3 towardsNear = cameraForward;
		frustumPlanes[NEAR].BuildPlane(nearCenter, towardsNear);

		Vector3 towardsFar = cameraForward;
		towardsFar.Negate();
		frustumPlanes[FAR].BuildPlane(farCenter, towardsFar);
	}

	bool Renderer::ShouldDraw(Renderable* pRenderable, Plane frustumPlanes[]) const
	{
		bool shouldDraw = true;

		if (pRenderable && pRenderable->GetUseBounds())
		{
			shouldDraw = false;

			Vector3 min = pRenderable->GetBoundMin();
			min.Add(pRenderable->GetTransform().GetTranslation());

			Vector3 max = pRenderable->GetBoundMax();
			max.Add(pRenderable->GetTransform().GetTranslation());

			static const unsigned int NUM_POINTS = 8;
			Vector3 points[NUM_POINTS];
			points[0] = min;
			points[1] = max;

			points[2].m_x = min.m_x;
			points[2].m_y = min.m_y;
			points[2].m_z = max.m_z;

			points[3].m_x = min.m_x;
			points[3].m_y = max.m_y;
			points[3].m_z = max.m_z;

			points[4].m_x = max.m_x;
			points[4].m_y = min.m_y;
			points[4].m_z = max.m_z;

			points[5].m_x = max.m_x;
			points[5].m_y = max.m_y;
			points[5].m_z = min.m_z;

			points[6].m_x = min.m_x;
			points[6].m_y = max.m_y;
			points[6].m_z = min.m_z;

			points[7].m_x = max.m_x;
			points[7].m_y = min.m_y;
			points[7].m_z = min.m_z;

			for (unsigned int j=0; j<NUM_POINTS; ++j)
			{
				unsigned int numPlanesInFront = 0;

				for (unsigned int i=0; i<6; ++i)
				{
					if (!frustumPlanes[i].IsInFront(points[j]))
					{
						break;
					}

					++numPlanesInFront;
				}

				if (numPlanesInFront == 6)
				{
					shouldDraw = true;
					break;
				}
			}
		}

		return shouldDraw;
	}

	void Renderer::Update()
	{
		if (m_initialized && !m_paused)
		{
			Plane	frustumPlanes[NUM_PARAMS];
			BuildFrustumPlanes(frustumPlanes);

			glEnable(GL_DEPTH_TEST);
			glClearColor(0.95f, 0.95f, 0.95f, 1);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			for (RenderableVectorIterator iter = m_renderables.begin(); iter != m_renderables.end(); ++iter)
			{
				Renderable* pRenderable = *iter;
				if (pRenderable)
				{
					bool bDraw = ShouldDraw(pRenderable, frustumPlanes);
					if (bDraw)
					{
						Draw(pRenderable);
					}
				}
			}

			eglSwapBuffers(m_display, m_surface);

			m_renderables.clear();
		}
	}

	void Renderer::OnResume()
	{

	}

	void Renderer::Stop()
	{
		DetachEvent(RESUMEAPP_EVENT, *this);
		DetachEvent(PAUSEAPP_EVENT, *this);
	}

	void Renderer::HandleEvent(Event* pEvent)
	{
		if (pEvent->GetID() == PAUSEAPP_EVENT)
		{
			m_paused = true;
		}
		else if (pEvent->GetID() == RESUMEAPP_EVENT)
		{
			m_paused = false;
		}
	}

	void Renderer::SetFrustum(const float frustumParameters[])
	{
		for (unsigned int i=0; i<NUM_PARAMS; ++i)
		{
			m_frustumParameters[i] = frustumParameters[i];
		}

		m_projectionMatrix.m_m[0] 	= (2.0f * m_frustumParameters[NEAR]) / (m_frustumParameters[RIGHT] - m_frustumParameters[LEFT]);
		m_projectionMatrix.m_m[1] 	= 0.0f;
		m_projectionMatrix.m_m[2] 	= 0.0f;
		m_projectionMatrix.m_m[3] 	= 0.0f;
		m_projectionMatrix.m_m[4] 	= 0.0f;
		m_projectionMatrix.m_m[5] 	= (2.0f * m_frustumParameters[NEAR]) / (m_frustumParameters[TOP] - m_frustumParameters[BOTTOM]);
		m_projectionMatrix.m_m[6] 	= 0.0f;
		m_projectionMatrix.m_m[7] 	= 0.0f;
		m_projectionMatrix.m_m[8] 	= -((m_frustumParameters[RIGHT] + m_frustumParameters[LEFT]) / (m_frustumParameters[RIGHT] - m_frustumParameters[LEFT]));
		m_projectionMatrix.m_m[9]	= -((m_frustumParameters[TOP] + m_frustumParameters[BOTTOM]) / (m_frustumParameters[TOP] - m_frustumParameters[BOTTOM]));
		m_projectionMatrix.m_m[10]	= (m_frustumParameters[FAR] + m_frustumParameters[NEAR]) / (m_frustumParameters[FAR] - m_frustumParameters[NEAR]);
		m_projectionMatrix.m_m[11]	= 1.0f;
		m_projectionMatrix.m_m[12]	= 0.0f;
		m_projectionMatrix.m_m[13]	= 0.0f;
		m_projectionMatrix.m_m[14]	= -(2.0f * m_frustumParameters[NEAR] * m_frustumParameters[FAR]) / (m_frustumParameters[FAR] - m_frustumParameters[NEAR]);
		m_projectionMatrix.m_m[15]	= 0.0f;
	}
}
