// *********************************************************************
// COPYRIGHT: This file is proprietary to Staubli SCA and can not be 
//            copied or used in this totality or partially without 
//            written authorization.
// ~NAME : synchro.c
// ~AUTHOR : L. Joly
// ~DESCRIPTION :
//	Implementation of functions for synchronous task
// **********************************************************************

#include "test.h"
#include "systemfunctions.h"
#include "lli_lowlevelinterf.h"
#include "algo1.h"
#include "algo2.h"
#include "synchro.h"

typedef struct __SynchroData
{
	LLI_RobotId		m_robotId;
	SEM_ID			m_semId;
	LLI_JointCmd	m_cmd[MAX_NB_JNT];
	LLI_JointFbk	m_fbk[MAX_NB_JNT];
	int				m_synchroTaskId;
	double			m_cycleTime;		// cycle time, in seconds
	int				m_algo;
	unsigned int    m_nbJnt;
} _SynchroData;
static _SynchroData	_synchData;

// ----------------------------------------------------------------
// _synchro
//	entry function of the synchronous task
// ----------------------------------------------------------------
void _synchro(void)
{
	LLI_State l_state;

	//~WHILE	endless loop 
	while (1)
	{
		// wait on synchronization semaphore
		takeSem(_synchData.m_semId);

		// get current joint feedback
		LLI_get(_synchData.m_robotId, _synchData.m_fbk);

		// get current status
		LLI_state(_synchData.m_robotId, &l_state);

		// compute new joint command
		//~SWITCH on current algorithm
		switch (_synchData.m_algo)
		{
		case 1:
			computeAlgo1(	_synchData.m_nbJnt,
							_synchData.m_cmd,
							_synchData.m_fbk,
							&l_state,
							_synchData.m_cycleTime);
			break;
		case 2: //~CASE 
			computeAlgo2(	_synchData.m_nbJnt,
							_synchData.m_cmd,
							_synchData.m_fbk,
							&l_state,
							_synchData.m_cycleTime);
			break;
		} // ~ENDSWITCH 

		// set new joint command
		LLI_set(_synchData.m_robotId, _synchData.m_cmd);

	} //~ENDWHILE 
}

// ----------------------------------------------------------------
// getSynchroJntCmd
// ----------------------------------------------------------------
void getSynchroJntCmd(LLI_JointCmd x_jntCmd[])
{
	unsigned int l_jnt;
	// ~FOR 
	for (l_jnt=0; l_jnt<_synchData.m_nbJnt;l_jnt++)
	{
		x_jntCmd[l_jnt] = _synchData.m_cmd[l_jnt];
	} // ~ENDFOR 
}

// ----------------------------------------------------------------
// getSynchroJntFbk
// ----------------------------------------------------------------
void getSynchroJntFbk(LLI_JointFbk x_jntFbk[])
{
	unsigned int l_jnt;
	// ~FOR 
	for (l_jnt=0; l_jnt<_synchData.m_nbJnt;l_jnt++)
	{
		x_jntFbk[l_jnt] = _synchData.m_fbk[l_jnt];
	} // ~ENDFOR 
}


// ----------------------------------------------------------------
// startSynchroTask
// ----------------------------------------------------------------
void startSynchroTask(	SEM_ID		x_semId,	// semaphore for synchronization
						LLI_RobotId	x_robotId,	// robot on which to operate
						int			x_algo)	// control algo to be used (1 or 2)
{
	int	l_priority;

	_synchData.m_semId = x_semId;
	_synchData.m_robotId = x_robotId;
	_synchData.m_algo = x_algo;
	// Get number of joints
	LLI_ioctrl(x_robotId, LLI_GET_JNT_NUMBER, &_synchData.m_nbJnt);

	//  ~IF cycle time can be found
	if (LLI_ioctrl(	x_robotId,
					LLI_GET_CYCLE_TIME,
					(void*) (&_synchData.m_cycleTime))
			== LLI_OK)
	{ //  ~THEN continue
		//  ~IF synchro task priority can be found
		if (LLI_ioctrl(x_robotId, LLI_GET_SYNCHRO_TASK_PRIORITY, (void*) (&l_priority)) == LLI_OK)
		{ //  ~THEN continue
			_synchData.m_synchroTaskId =  spawnTask("synchro",		// task name
													l_priority,		// priority
													25000,			// stack size
													(FUNCPTR)(_synchro));		// entry function
			//  ~IF construction failed
			if (_synchData.m_synchroTaskId == 0)
			{ //  ~THEN 
				printf("UNABLE TO START SYNCHRO TASK !!!\n");
			} else
			{ //  ~ELSE 
				printf("Synchro task started\n");
			} // ~ENDIF 
		} else
		{ //  ~ELSE 
			printf("Cannot get synchronous task priority -> synchro task not started!\n");
		} // ~ENDIF 
	} else
	{ //  ~ELSE 
		printf("Could not get cycle time -> synchro task not started!\n");
	} // ~ENDIF 
}

// ----------------------------------------------------------------
// stopSynchroTask
// ----------------------------------------------------------------
void stopSynchroTask(void)
{
	deleteTask(_synchData.m_synchroTaskId);
	_synchData.m_synchroTaskId = 0;
}

// ----------------------------------------------------------------
// setAlgo
// ----------------------------------------------------------------
void setAlgo(int x_algo)
{
	_synchData.m_algo = x_algo;
}

// ----------------------------------------------------------------
// getAlgo
// ----------------------------------------------------------------
int getAlgo(void)
{
	return 	_synchData.m_algo;
}

