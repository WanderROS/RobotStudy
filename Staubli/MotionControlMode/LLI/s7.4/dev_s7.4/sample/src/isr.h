// *********************************************************************
// COPYRIGHT: This file is proprietary to Staubli SCA and can not be 
//            copied or used in this totality or partially without 
//            written authorization.
// ~NAME : isr.h
// ~AUTHOR : L. Joly
// ~DESCRIPTION : interface for the SERCOS ISR plug-in function
// **********************************************************************
#ifndef _ISR_H_
#define _ISR_H_

// ----------------------------------------------------------------
// setIsrSem
// must be called before ISR is started
// ----------------------------------------------------------------
void setIsrSem(SEM_ID x_semId);

// ----------------------------------------------------------------
// isr
// plug-in function for the SERCOS ISR
// ----------------------------------------------------------------
void isr(void);

#endif //_ISR_H_
