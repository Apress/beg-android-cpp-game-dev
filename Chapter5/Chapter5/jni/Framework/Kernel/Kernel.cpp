#include "Kernel.h"
#include "../Platform/Android.h"

namespace Framework
{
	Kernel::Kernel()
	{
	}

	Kernel::~Kernel()
	{
	}

	void Kernel::PriorityAdd(Task* pTask)
	{
		TaskListIterator iter;
		for (iter = m_tasks.begin(); iter != m_tasks.end(); ++iter)
		{
			Task* pCurrentTask = (*iter);
			if (pCurrentTask->Priority() > pTask->Priority())
			{
				break;
			}
		}
		m_tasks.insert(iter, pTask);
	}

	void Kernel::Execute()
    {
    	while (m_tasks.size())
    	{
    		if (Android::IsClosing())
        	{
            	KillAllTasks();
        	}

    		TaskListIterator iter;
    		for (iter = m_tasks.begin(); iter != m_tasks.end(); ++iter)
    		{
    			Task* pTask = (*iter);
    			if (!pTask->CanKill())
    			{
    				pTask->Update();
    			}
    		}

    		for (iter = m_tasks.begin(); iter != m_tasks.end();)
    		{
    			Task* pTask = (*iter);
    			++iter;
    			if (pTask->CanKill())
    			{
    				pTask->Stop();
                	m_tasks.remove(pTask);
                	pTask = 0;
    			}
    		}
    	}

		Android::ClearClosing();
    }

    bool Kernel::AddTask(Task* pTask)
	{
		bool started = pTask->Start();

		if (started)
		{
			PriorityAdd(pTask);
		}
		return started;
	}

	void Kernel::SuspendTask(Task* pTask)
	{
		// Ensure that task is running
		if (std::find(m_tasks.begin(), m_tasks.end(), pTask) != m_tasks.end())
		{
			pTask->OnSuspend();
			m_tasks.remove(pTask);
			m_pausedTasks.push_back(pTask);
		}
	}

	void Kernel::ResumeTask(Task* pTask)
	{
		if (std::find(m_pausedTasks.begin(), m_pausedTasks.end(), pTask) != m_pausedTasks.end())
		{
			pTask->OnResume();
			m_pausedTasks.remove(pTask);

			PriorityAdd(pTask);
		}
	}

	void Kernel::RemoveTask(Task* pTask)
	{
		if (std::find(m_tasks.begin(), m_tasks.end(), pTask) != m_tasks.end())
		{
			pTask->SetCanKill(true);
		}
	}

	void Kernel::KillAllTasks()
	{
		for (TaskListIterator iter = m_tasks.begin(); iter != m_tasks.end(); ++iter)
		{
			(*iter)->SetCanKill(true);
		}
	}
}
