#ifndef CHAPTER8APP_H_
#define CHAPTER8APP_H_

#include "Framework/Application/Application.h"
#include "Chapter8Task.h"
#include <android_native_app_glue.h>

class Chapter8App
	:	public Framework::Application
{
private:
	Chapter8Task	m_chapter8Task;

public:
	Chapter8App(android_app* pState);
	virtual ~Chapter8App();

	virtual bool Initialize();
};

#endif // CHAPTER8APP_H_
