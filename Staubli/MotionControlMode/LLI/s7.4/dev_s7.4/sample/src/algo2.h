//*********************************************************************
//  COPYRIGHT: This file is proprietary to Staubli SCA and can not be 
//             copied or used in this totality or partially without 
//             written authorization.
// ~NAME : algo2.h
// ~AUTHOR : L. Joly
// ~DESCRIPTION : interface of Algo2
//**********************************************************************
#ifndef _ALGO2_H_
#define _ALGO2_H_

// ----------------------------------------------------------------
// initAlgo1
// ----------------------------------------------------------------
void initAlgo2(unsigned int	x_jntNb);

// ----------------------------------------------------------------
// setGain
// ----------------------------------------------------------------
void setGain(unsigned int x_jntNb, double x_gain[]);

// ----------------------------------------------------------------
// computeAlgo2
// Compute next setpoint according to current feedback and state
// ----------------------------------------------------------------
void computeAlgo2(	unsigned int	x_jntNb,		// number of axes
					LLI_JointCmd	x_jntCmd[],		// next setpoint (out)
					LLI_JointFbk	x_jntFbk[],		// current feedback (in)
					LLI_State*		x_pState,		// current state (in)
					double			x_cycleTime);	// cycleTime in seconds (in)


#endif //_ALGO2_H_
