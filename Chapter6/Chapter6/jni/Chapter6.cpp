#include <jni.h>
#include <android_native_app_glue.h>
#include "Chapter6App.h"
#include <unistd.h>

void android_main(struct android_app* state)
{
	sleep(5);

	app_dummy();

	Chapter6App app(state);

	if (app.Initialize())
	{
		app.Run();
	}
}
