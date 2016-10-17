#ifndef CHAPTER10TASK_H_
#define CHAPTER10TASK_H_

#include "Framework/Kernel/Task.h"
#include "DroidRunnerLevel.h"

class Chapter10Task
	:	public Framework::Task
{
private:
	DroidRunnerLevel*	m_pLevel;

public:
	Chapter10Task(const unsigned int priority);
	virtual ~Chapter10Task();

	// From Task
	virtual bool	Start();
	virtual void	OnSuspend();
	virtual void	Update();
	virtual void	OnResume();
	virtual void	Stop();
};

#endif // CHAPTER10TASK_H_
