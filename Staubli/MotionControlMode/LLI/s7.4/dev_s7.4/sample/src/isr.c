// *********************************************************************
// COPYRIGHT: This file is proprietary to Staubli SCA and can not be 
//            copied or used in this totality or partially without 
//            written authorization.
// ~NAME : isr.c
// ~AUTHOR : L. Joly
// ~DESCRIPTION : Implementation of the functions related to
// sercos ISR related 
// **********************************************************************
#include "systemfunctions.h"
#include "isr.h"

static SEM_ID _isrSemId;

// ----------------------------------------------------------------
// setIsrSem
// ----------------------------------------------------------------
void setIsrSem(SEM_ID x_semId)
{
	_isrSemId = x_semId;
}


// ----------------------------------------------------------------
// isr
// Remark: the LLI_PlugIn is the address of this function,i.e. &isr
// ----------------------------------------------------------------
void isr(void)
{	
	giveSem(_isrSemId);
}

