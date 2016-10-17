#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "../Kernel/Kernel.h"
#include "../Platform/Android.h"
#include <android_native_app_glue.h>

namespace Framework
{
	class Application
	{
	protected:
		android_app*	m_pAppState;
		Kernel			m_kernel;
		Android			m_androidTask;

		virtual void CreateSingletons();
		virtual void DestroySingletons();

	public:
		explicit Application(android_app* state);
		virtual ~Application();

		virtual bool	Initialize();
		virtual void	Run();
	};
}

#endif // APPLICATION_H_
