#ifndef CHAPTER7TASK_H_
#define CHAPTER7TASK_H_

#include "Framework/Kernel/Task.h"
#include "DroidRunnerLevel.h"

class Chapter7Task
	:	public Framework::Task
{
private:
	DroidRunnerLevel	m_level;

public:
	Chapter7Task(const unsigned int priority);
	virtual ~Chapter7Task();

	// From Task
	virtual bool	Start();
	virtual void	OnSuspend();
	virtual void	Update();
	virtual void	OnResume();
	virtual void	Stop();
};

#endif // CHAPTER7TASK_H_
