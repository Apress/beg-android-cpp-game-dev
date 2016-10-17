#include <jni.h>
#include <android_native_app_glue.h>
#include "Chapter7App.h"
#include <unistd.h>

void android_main(struct android_app* state)
{
	sleep(5);

	app_dummy();

	Chapter7App app(state);

	if (app.Initialize())
	{
		app.Run();
	}
}
