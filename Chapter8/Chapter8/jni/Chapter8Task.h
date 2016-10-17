#ifndef CHAPTER8TASK_H_
#define CHAPTER8TASK_H_

#include "Framework/Kernel/Task.h"
#include "DroidRunnerLevel.h"

class Chapter8Task
	:	public Framework::Task
{
private:
	DroidRunnerLevel*	m_pLevel;

public:
	Chapter8Task(const unsigned int priority);
	virtual ~Chapter8Task();

	// From Task
	virtual bool	Start();
	virtual void	OnSuspend();
	virtual void	Update();
	virtual void	OnResume();
	virtual void	Stop();
};

#endif // CHAPTER8TASK_H_
