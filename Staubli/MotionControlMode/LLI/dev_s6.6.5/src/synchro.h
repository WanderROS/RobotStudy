// *********************************************************************
// COPYRIGHT: This file is proprietary to Staubli SCA and can not be 
//            copied or used in this totality or partially without 
//            written authorization.
// ~NAME : synchro.h
// ~AUTHOR : L. Joly
// ~DESCRIPTION : interface for the synchronous task functions.
//		(task schedule by sercos ISR)
// **********************************************************************
#ifndef _SYNCHRO_H_
#define _SYNCHRO_H_


// ----------------------------------------------------------------
// startSynchroTask
// ----------------------------------------------------------------
void startSynchroTask(	SEM_ID		x_semId,	// semaphore for synchronisation
						LLI_RobotId	x_robotId,	// robot on which to operate
						int			x_algo);	// control algo to be used (1 or 2)

// ----------------------------------------------------------------
// stopSynchroTask
// ----------------------------------------------------------------
void stopSynchroTask(void);

// ----------------------------------------------------------------
// getSynchroJntCmd
// get current joint command from synchronous task
// ----------------------------------------------------------------
void getSynchroJntCmd(LLI_JointCmd x_jntCmd[]);

// ----------------------------------------------------------------
// getSynchroJntFbk
// get current joint feedback from synchronous task
// ----------------------------------------------------------------
void getSynchroJntFbk(LLI_JointFbk x_jntFbk[]);

// ----------------------------------------------------------------
// setAlgo
//	Set the current algorithm to be used (1 or 2)
// ----------------------------------------------------------------
void setAlgo(int x_algo);

// ----------------------------------------------------------------
// getAlgo
//	Get the current algorithm used (1 or 2)
// ----------------------------------------------------------------
int getAlgo(void);

#endif //_SYNCHRO_H_
