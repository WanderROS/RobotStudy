//*********************************************************************
//  COPYRIGHT: This file is proprietary to Staubli SCA and can not be 
//             copied or used in this totality or partially without 
//             written authorization.
// ~NAME : algo1.c
// ~AUTHOR : L. Joly
// ~DESCRIPTION : implementation of Algo1
//**********************************************************************


#include <math.h>
#include <stdlib.h>
#include "test.h"
#include "lli_lowlevelinterf.h"
#include "algo1.h"

extern char _isScara( LLI_RobotId	x_robotId);


#define _AMPLITUDE_RAD 0.1	// rad/s
#define _AMPLITUDE_M 0.02	// m/s
#define _PULSATION 1.0	// rad/s

// ----------------------------------------------------------------
// _Algo1Data structure
// ----------------------------------------------------------------
typedef struct __Algo1Data
{
	int				m_move;		// indicate which joint, and in which direction
	double			m_time;		// time of movement (seconds)
	double			m_amplitude;	// amplitude of movement (rad/s or m/s)
	double			m_pulsation;	// pulsation of movement (rad/s)
} _Algo1Data;

static _Algo1Data	_algo1Data;


// ----------------------------------------------------------------
// initAlgo1
// ----------------------------------------------------------------
void initAlgo1(void)
{
	_algo1Data.m_move = 0;
	_algo1Data.m_time = 0.0;
	_algo1Data.m_amplitude = 0.0;
	_algo1Data.m_pulsation = _PULSATION;
}


// ----------------------------------------------------------------
// computeAlgo1
// ----------------------------------------------------------------
void computeAlgo1(	unsigned int	x_jntNb,		// number of axes
					LLI_JointCmd	x_jntCmd[],		// next setpoint (out)
					LLI_JointFbk	x_jntFbk[],		// current feedback (in)
					LLI_State*		x_pState,		// current state (in)
					double			x_cycleTime)	// cycleTime in seconds (in)
{
	unsigned int l_jnt;
	double	l_speed;

	// position and velocity commands depend on the current state
	// ~SWITCH on the current state
	switch (x_pState->m_enableState)
	{
	case LLI_DISABLED : // following mode 
		// copy fbk into the command
		// ~FOR each joint
		for (l_jnt = 0; l_jnt<x_jntNb; l_jnt++)
		{
			x_jntCmd[l_jnt].m_pos = x_jntFbk[l_jnt].m_pos;
			x_jntCmd[l_jnt].m_vel = 0.0;
		} // ~ENDFOR 
		break;
	case LLI_ENABLED : // move if necessary 
		//  ~IF 
		if (_algo1Data.m_move != 0)
		{ //  ~THEN 
			l_jnt = abs(_algo1Data.m_move) - 1;
			_algo1Data.m_time +=x_cycleTime;
			l_speed = _algo1Data.m_amplitude * sin(_algo1Data.m_pulsation*_algo1Data.m_time);
			if (_algo1Data.m_move < 0) l_speed = -l_speed;
			x_jntCmd[l_jnt].m_vel = l_speed;
			x_jntCmd[l_jnt].m_pos += l_speed*x_cycleTime;
			//  ~IF movement is finished
			if (_PULSATION*_algo1Data.m_time > PI)
			{ //  ~THEN stop motion
				_algo1Data.m_time = 0.0;
				_algo1Data.m_move = 0;
				// keep position command unchanged, set velocity command to zero
				x_jntCmd[l_jnt].m_vel = 0;
			} // ~ENDIF 
		} // ~ENDIF 
		break;
	default : // ~DEFAULT (enabling/disabling: command is unchanged)
		for (l_jnt = 0; l_jnt<x_jntNb; l_jnt++)
		{
			// keep position command unchanged, set velocity command to zero
			x_jntCmd[l_jnt].m_vel = 0.0;
		} // ~ENDFOR 
		break;
	} // ~ENDSWITCH 

	// feedforwards are always zero (not used)
	// ~FOR each joint
	for (l_jnt = 0; l_jnt<x_jntNb; l_jnt++)
	{
		x_jntCmd[l_jnt].m_torqueFfw = 0.0;
	} // ~ENDFOR 

}



// ----------------------------------------------------------------
// move
// ----------------------------------------------------------------
void move( LLI_RobotId	x_robotId, int x_joint)
{
	int l_jntNb;
	// Get number of joints
	LLI_ioctrl(x_robotId, LLI_GET_JNT_NUMBER, &l_jntNb);

	//  ~IF joint number is invalid
	if ( (x_joint == 0) || (x_joint >l_jntNb) || (x_joint< -l_jntNb) )
	{ //  ~THEN 
		printf("invalid joint was selected : %d", x_joint);
	} else
	{ //  ~ELSE 
		//  ~IF no joint is currently moving
		if (_algo1Data.m_move == 0)
		{ //  ~THEN 
			_algo1Data.m_move = x_joint;
			if((_isScara(x_robotId))
				&&((x_joint==SCARA_LINEAR_JNT+1)||(x_joint==(-SCARA_LINEAR_JNT-1))))
				_algo1Data.m_amplitude = _AMPLITUDE_M;
			else
				_algo1Data.m_amplitude = _AMPLITUDE_RAD;
			printf("movement started on joint %d", _algo1Data.m_move);
		} else
		{ //  ~ELSE 
			printf("Already moving on joint %d !\n", _algo1Data.m_move);
		} // ~ENDIF 
	} // ~ENDIF 
}

