#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "../Kernel/Kernel.h"
#include "../Platform/Android.h"
#include "../Renderer/Renderer.h"
#include "../Utilities/Timer.h"
#include <android_native_app_glue.h>

namespace Framework
{
	class Application
	{
	protected:
		Kernel		m_kernel;
		Android		m_androidTask;
		Timer		m_timerTask;
		Renderer	m_rendererTask;

	public:
		explicit Application(android_app* state);
		virtual ~Application();

		virtual bool	Initialize();
		virtual void	Run();
	};
}

#endif // APPLICATION_H_
