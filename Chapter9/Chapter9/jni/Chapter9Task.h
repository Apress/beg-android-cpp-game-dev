#ifndef CHAPTER9TASK_H_
#define CHAPTER9TASK_H_

#include "Framework/Kernel/Task.h"
#include "DroidRunnerLevel.h"

class Chapter9Task
	:	public Framework::Task
{
private:
	DroidRunnerLevel*	m_pLevel;

public:
	Chapter9Task(const unsigned int priority);
	virtual ~Chapter9Task();

	// From Task
	virtual bool	Start();
	virtual void	OnSuspend();
	virtual void	Update();
	virtual void	OnResume();
	virtual void	Stop();
};

#endif // CHAPTER9TASK_H_
