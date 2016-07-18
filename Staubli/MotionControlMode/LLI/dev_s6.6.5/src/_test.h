// *********************************************************************
// COPYRIGHT: This file is proprietary to Staubli SCA and can not be 
//            copied or used in this totality or partially without 
//            written authorization.
// ~NAME : _test.h
// ~AUTHOR : L. Joly
// ~DESCRIPTION :
//	include file necessary o compile all the sources
// **********************************************************************
#ifndef __TEST_H_
#define __TEST_H_

// ----------------------------------------------------------------
// Standard libraries
// ----------------------------------------------------------------
#ifdef VXWORKS // Need to configure COM1
#include <iolib.h>
#include <loglib.h>
#include <usrlib.h>
#include <tasklib.h>
#endif

#include <stdio.h>
#include <math.h>

// ----------------------------------------------------------------
// External modules
// ----------------------------------------------------------------
#include "lli_lowlevelinterf.h"
#include "test.h"


// ----------------------------------------------------------------
// Internal constants
// ----------------------------------------------------------------

// ----------------------------------------------------------------
// Internal headers
// ----------------------------------------------------------------
#include "systemfunctions.h"
#include "isr.h"
#include "synchro.h"

void displayJoint (LLI_RobotId x_robotId, const LLI_Joint* x_jntPos);
void testKinOnly (const char* x_pathSys,const char* x_pathUsr,const char* x_pathLog);
void testKin (LLI_RobotId x_rob, int x_verbose);

#endif //__TEST_H_
