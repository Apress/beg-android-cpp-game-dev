#ifndef CHAPTER7APP_H_
#define CHAPTER7APP_H_

#include "Framework/Application/Application.h"
#include "Chapter7Task.h"
#include <android_native_app_glue.h>

class Chapter7App
	:	public Framework::Application
{
private:
	Chapter7Task	m_chapter7Task;

public:
	Chapter7App(android_app* pState);
	virtual ~Chapter7App();

	virtual bool Initialize();
};

#endif // CHAPTER7APP_H_
