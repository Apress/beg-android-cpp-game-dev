#ifndef CHAPTER9APP_H_
#define CHAPTER9APP_H_

#include "Framework/Application/Application.h"
#include "Chapter9Task.h"
#include <android_native_app_glue.h>

class Chapter9App
	:	public Framework::Application
{
private:
	Chapter9Task	m_chapter9Task;

public:
	Chapter9App(android_app* pState);
	virtual ~Chapter9App();

	virtual bool Initialize();
};

#endif // CHAPTER9APP_H_
