#ifndef CHAPTER5APP_H_
#define CHAPTER5APP_H_

#include "Framework/Application/Application.h"
#include "Chapter5Task.h"
#include <android_native_app_glue.h>

class Chapter5App
	:	public Framework::Application
{
private:
	Chapter5Task	m_chapter5Task;

public:
	Chapter5App(android_app* pState);
	virtual ~Chapter5App();

	virtual bool Initialize();
};

#endif // CHAPTER5APP_H_
