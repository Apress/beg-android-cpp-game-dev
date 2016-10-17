#ifndef CHAPTER10APP_H_
#define CHAPTER10APP_H_

#include "Framework/Application/Application.h"
#include "Chapter10Task.h"
#include <android_native_app_glue.h>

class Chapter10App
	:	public Framework::Application
{
private:
	Chapter10Task	m_chapter10Task;

public:
	Chapter10App(android_app* pState);
	virtual ~Chapter10App();

	virtual bool Initialize();
};

#endif // CHAPTER10APP_H_
