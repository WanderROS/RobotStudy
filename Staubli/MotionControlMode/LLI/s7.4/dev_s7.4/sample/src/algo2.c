//*********************************************************************
//  COPYRIGHT: This file is proprietary to Staubli SCA and can not be 
//             copied or used in this totality or partially without 
//             written authorization.
// ~NAME : algo2.c
// ~AUTHOR : L. Joly
// ~DESCRIPTION : Implementation of Algo2
//**********************************************************************

#include "test.h"
#include "lli_lowlevelinterf.h"
#include "algo2.h"

typedef struct __Algo2Data
{
	double	m_gain[MAX_NB_JNT];		// proportional servo gain
	double	m_posCmd[MAX_NB_JNT];	// setPoint (setpoint for servo is different
								//				from the setpoint sent to the drives)
} _Algo2Data;

static _Algo2Data	_algo2Data;


// ----------------------------------------------------------------
// initAlgo2
// ----------------------------------------------------------------
void initAlgo2(unsigned int	x_jntNb)
{
	unsigned int l_jnt;

	// set all gains to zero
	for (l_jnt=0; l_jnt<x_jntNb; l_jnt++)
		_algo2Data.m_gain[l_jnt] = 0.0;
}

// ----------------------------------------------------------------
// _synchro2
// ----------------------------------------------------------------
void computeAlgo2(	unsigned int	x_jntNb,		// number of axes
				    LLI_JointCmd	x_jntCmd[],		// next setpoint (out)
					LLI_JointFbk	x_jntFbk[],		// current feedback (in)
					LLI_State*		x_pState,		// current state (in)
					double			x_cycleTime)	// cycleTime in seconds (in)
{
	unsigned int	l_jnt;

	// compute new joint torques
	// ~SWITCH 
	switch (x_pState->m_enableState)
	{
	case LLI_DISABLED :
		// update position setpoint, no torque command
		// ~FOR each joint
		for (l_jnt = 0; l_jnt<x_jntNb; l_jnt++)
		{
			_algo2Data.m_posCmd[l_jnt] = x_jntFbk[l_jnt].m_pos;
			x_jntCmd[l_jnt].m_torqueFfw = 0.0;
		} // ~ENDFOR 
		break;
	default :
		// position setpoint unchanged
		// run servo algo : torque = K.(posCmd-posMeasured)
		// ~FOR each joint
		for (l_jnt = 0; l_jnt<x_jntNb; l_jnt++)
		{
			x_jntCmd[l_jnt].m_torqueFfw = _algo2Data.m_gain[l_jnt]*
					(_algo2Data.m_posCmd[l_jnt] - x_jntFbk[l_jnt].m_pos);
		} // ~ENDFOR 
		break;
	} // ~ENDSWITCH 

	// copy pos and vel fbk into the commands
	// ~FOR each joint
	for (l_jnt = 0; l_jnt<x_jntNb; l_jnt++)
	{
		x_jntCmd[l_jnt].m_pos = x_jntFbk[l_jnt].m_pos;
		x_jntCmd[l_jnt].m_vel = x_jntFbk[l_jnt].m_vel;
	} // ~ENDFOR 
}

// ----------------------------------------------------------------
// setGain
// ----------------------------------------------------------------
void setGain(unsigned int l_jntNb, double x_gain[])
{
	unsigned int l_jnt;

	// set all gains to specified value
	for (l_jnt=0; l_jnt<l_jntNb; l_jnt++)
		_algo2Data.m_gain[l_jnt] = x_gain[l_jnt];
}





