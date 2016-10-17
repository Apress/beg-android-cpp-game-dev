#ifndef CHAPTER6APP_H_
#define CHAPTER6APP_H_

#include "Framework/Application/Application.h"
#include "Chapter6Task.h"
#include <android_native_app_glue.h>

class Chapter6App
	:	public Framework::Application
{
private:
	Chapter6Task	m_chapter6Task;

public:
	Chapter6App(android_app* pState);
	virtual ~Chapter6App();

	virtual bool Initialize();
};

#endif // CHAPTER6APP_H_
