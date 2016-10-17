#include <jni.h>
#include <android_native_app_glue.h>
#include "Chapter8App.h"
#include <unistd.h>

void android_main(struct android_app* state)
{
	sleep(5);

	app_dummy();

	Chapter8App app(state);

	if (app.Initialize())
	{
		app.Run();
	}
}
