// *********************************************************************
// COPYRIGHT: This file is proprietary to Staubli SCA and can not be 
//            copied or used in this totality or partially without 
//            written authorization.
// ~NAME : vxfunctions.c
// ~AUTHOR : L. Joly
// ~DESCRIPTION : Interface to vxworks functions
// **********************************************************************
#include "_test.h"


// ----------------------------------------------------------------
// delay
// ----------------------------------------------------------------
//void delay(int x_time)
//{
//	taskDelay ( (sysClkRateGet() * x_time ) / 1000 );
//}

// ----------------------------------------------------------------
// createSemB
// ----------------------------------------------------------------
SEM_ID createSemB(void)
{
	return semBCreate(SEM_Q_PRIORITY, 0);
}

// ----------------------------------------------------------------
// giveSem
// ----------------------------------------------------------------
int giveSem(SEM_ID x_id)
{
	return (semGive (x_id)==OK) ? 1 : 0;
}

// ----------------------------------------------------------------
// takeSem
// ----------------------------------------------------------------
int takeSem(SEM_ID x_id)
{
	// take semaphore, 'wait forever' option
	return (semTake (x_id, -1)==OK) ? 1 : 0;
}

// ----------------------------------------------------------------
// spawnTask
// ----------------------------------------------------------------
int spawnTask(char* x_name, int x_priority,
			  int x_stackSize, FUNCPTR x_entry)
{
	return taskSpawn(x_name, x_priority, VX_FP_TASK,
					 x_stackSize, x_entry, 
					 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

// ----------------------------------------------------------------
// deleteTask
// ----------------------------------------------------------------
void deleteTask(int x_taskId)
{
	taskDelete (x_taskId);
}
