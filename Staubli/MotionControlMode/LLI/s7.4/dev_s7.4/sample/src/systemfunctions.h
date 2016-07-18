// *********************************************************************
// COPYRIGHT: This file is proprietary to Staubli SCA and can not be 
//            copied or used in this totality or partially without 
//            written authorization.
// ~NAME : systemfunctions.h
// ~AUTHOR : L. Joly
// ~DESCRIPTION : definition of interface to RTOS
// **********************************************************************
#ifndef _SYSTEMFUNCTIONS_H_
#define _SYSTEMFUNCTIONS_H_

#ifdef VXWORKS
	#include <taskLib.h>
	#include <syslib.h>
	#include <semlib.h>
#else
	#include <windows.h>
	typedef HANDLE SEM_ID;
	typedef LPTHREAD_START_ROUTINE FUNCPTR;
#endif

// void delay(int x_time);

SEM_ID createSemB(void);
int giveSem(SEM_ID x_id);
int takeSem(SEM_ID x_id);

int spawnTask(char* x_name, int x_priority, int x_stackSize, FUNCPTR x_entry);
void deleteTask(int x_taskId);
int taksNameToInt(char* x_name);

#endif //_SYSTEMFUNCTIONS_H_
