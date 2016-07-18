//*********************************************************************
//  COPYRIGHT: This file is proprietary to Staubli SCA and can not be 
//             copied or used in this totality or partially without 
//             written authorization.
// ~NAME : algo1.h
// ~AUTHOR : L. Joly
// ~DESCRIPTION : Implementation of Algo1
//**********************************************************************
#ifndef _ALGO1_H_
#define _ALGO1_H_

// ----------------------------------------------------------------
// initAlgo1
// ----------------------------------------------------------------
void initAlgo1(void);

// ----------------------------------------------------------------
// move
// examples:
//	move(2) moves joint 2 in positive direction
//	move(-4) moves joint 4 in negative direction
// ----------------------------------------------------------------
void move( LLI_RobotId	x_robotId, int x_joint);

// ----------------------------------------------------------------
// computeAlgo1
// Compute next setpoint according to current feedback and state
// ----------------------------------------------------------------
void computeAlgo1(	unsigned int	x_jntNb,		// number of axes
					LLI_JointCmd	x_jntCmd[],		// next setpoint (out)
					LLI_JointFbk	x_jntFbk[],		// current feedback (in)
					LLI_State*		x_pState,		// current state (in)
					double			x_cycleTime);	// cycleTime in seconds (in)


#endif //_ALGO1_H_
