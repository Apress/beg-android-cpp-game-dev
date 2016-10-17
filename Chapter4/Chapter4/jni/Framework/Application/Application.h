#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "../Kernel/Kernel.h"
#include "../Platform/Android.h"
#include "../Utilities/Timer.h"
#include <android_native_app_glue.h>

namespace Framework
{
	class Application
	{
	private:
		Kernel		m_kernel;
		Android		m_androidTask;
		Timer		m_timerTask;

	public:
		Application(android_app* state);
		virtual ~Application();

		bool	Initialize();
		void	Run();
	};
}

#endif // APPLICATION_H_
