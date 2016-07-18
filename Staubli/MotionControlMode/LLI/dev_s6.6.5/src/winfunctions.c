// *********************************************************************
// COPYRIGHT: This file is proprietary to Staubli SCA and can not be 
//            copied or used in this totality or partially without 
//            written authorization.
// ~NAME : winfunctions.c
// ~AUTHOR : C. Exertier
// ~DESCRIPTION : Interface to Windows functions
// **********************************************************************
#include "_test.h"

static	HANDLE m_pThread=NULL;
static	DWORD m_threadId=-1;

// ----------------------------------------------------------------
// delay
// ----------------------------------------------------------------
//void delay(int x_time)
//{
//	Sleep ( x_time );
//}

// ----------------------------------------------------------------
// createSemB
// ----------------------------------------------------------------
SEM_ID createSemB(void)
{
	return CreateSemaphore(NULL, 0, 1, NULL);
}

// ----------------------------------------------------------------
// giveSem
// ----------------------------------------------------------------
int giveSem(SEM_ID x_id)
{
	return (ReleaseSemaphore( x_id, 1,0 )==TRUE) ? 1 : 0;
}

// ----------------------------------------------------------------
// takeSem
// ----------------------------------------------------------------
int takeSem(SEM_ID x_id)
{
	// take semaphore, 'wait forever' option
	return (WaitForSingleObject( x_id, INFINITE)==WAIT_FAILED) ? 0 : 1;
}

// ----------------------------------------------------------------
// spawnTask
// ----------------------------------------------------------------
int spawnTask(char* x_name, int x_priority,
			  int x_stackSize, FUNCPTR x_entry)
{
	int l_prio;

	// Create the thread
	m_pThread=CreateThread
	(
		// pointer to thread security attributes 
		NULL,
		// initial thread stack size, in bytes 
		0,
		// pointer to thread function 
		(LPTHREAD_START_ROUTINE) x_entry,
		// argument for new thread 
		0,
		// creation flags 
		0,
		// pointer to returned thread identifier 
		&m_threadId
	);
  
	// set the thread priority
	// determine thread priority for win32
	if (x_priority <= 2)
		l_prio = THREAD_PRIORITY_TIME_CRITICAL;
	else if (x_priority <= 5)
		l_prio = THREAD_PRIORITY_HIGHEST;
	else if (x_priority <= 10)
		l_prio = THREAD_PRIORITY_ABOVE_NORMAL;
	else if (x_priority <= 100)
		l_prio = THREAD_PRIORITY_NORMAL;
	else if (x_priority <= 200)
		l_prio = THREAD_PRIORITY_BELOW_NORMAL;
	else if (x_priority <= 300)
		l_prio = THREAD_PRIORITY_LOWEST;
	else
		l_prio = THREAD_PRIORITY_IDLE;

	SetThreadPriority( m_pThread, l_prio);

	return m_threadId;
}

// ----------------------------------------------------------------
// deleteTask
// ----------------------------------------------------------------
void deleteTask(int x_taskId)
{
	if (m_pThread != NULL)
	{
		TerminateThread( (void*)m_pThread, 0); 
	}
}