//*********************************************************************
//  COPYRIGHT: This file is proprietary to Staubli SCA and can not be 
//             copied or used in this totality or partially without 
//             written authorization.
// ~NAME : synchroProtected.h
// ~AUTHOR : L. Joly
// ~DESCRIPTION : Functions and data shared by "Synchronous Task"
//					and derived classes
//**********************************************************************
#ifndef _SYNCHROPROTECTED_H_
#define _SYNCHROPROTECTED_H_

// ----------------------------------------------------------------
// SynchroData structure
// ----------------------------------------------------------------
typedef struct 
{
	LLI_RobotId		m_robotId;
	SEM_ID			m_semId;
	LLI_JointCmd	m_cmd[_NB_JNT];
	LLI_JointFbk	m_fbk[_NB_JNT];
	int				m_synchroTaskId;
	FUNCPTR			m_pFunc;			// pointer to the synchronous function
	double			m_cycleTime;		// cycle time, in seconds
} SynchroData;

// protected data, shared only by Synchronous task and derives classes
extern SynchroData	synchData;

// ----------------------------------------------------------------
// setSynchroTaskPtr
// ----------------------------------------------------------------
void setSynchroTaskPtr(FUNCPTR x_pFunc);


#endif //_SYNCHROPROTECTED_H_
