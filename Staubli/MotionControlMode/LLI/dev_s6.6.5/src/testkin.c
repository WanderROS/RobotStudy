// *********************************************************************
// COPYRIGHT: This file is proprietary to Staubli SCA and can not be 
//            copied or used in this totality or partially without 
//            written authorization.
// ~NAME : test.c
// ~AUTHOR : F. Pertin
// ~DESCRIPTION :
// **********************************************************************

#include <math.h>
#include <assert.h>
#include <string.h>
#include "lli_lowlevelinterf.h"
#include "_test.h"

#define _RAD_TO_DEG (180.0 / PI)
#define _DEG_TO_RAD (PI / 180.0)
#define _M_TO_MM (1000.0)
#define _MM_TO_M (0.001)
#define _MAX_NB_JOINT (6)

#define S_SMALL_FLOAT		(1.e-5)
#define S_VERY_SMALL_FLOAT	(1.e-10)
#define S_PI				PI
#define S_BIG_FLOAT			1.e10

// ----------------------------------------------------------------
// _getRxRyRzCoord
// ----------------------------------------------------------------
static void _getRxRyRzCoord (const LLI_Frame* x_fr, double* x_Rx, double* x_Ry, double* x_Rz)
{
	double l_sinRy;

	l_sinRy = x_fr->m_ax;
	// ATTENTION : it may be possible that sinRy > 1.0 or < -1.0 (numerical pbm)
	if(l_sinRy < (-1.0 + S_SMALL_FLOAT*S_SMALL_FLOAT/2.0))
	{
		*x_Rx = 0;
		*x_Ry = -S_PI/2.0;
		*x_Rz = atan2(x_fr->m_ny, x_fr->m_nz);
	} else if(l_sinRy > (1.0 - S_SMALL_FLOAT*S_SMALL_FLOAT/2.0))
	{
		*x_Rx = 0;
		*x_Ry = S_PI/2.0;
		*x_Rz = atan2(x_fr->m_ny, -x_fr->m_nz);
	} else
	{
		double l_sign = 1.0;
		*x_Ry = asin(l_sinRy);
		if((x_fr->m_az < 0.0) && (x_fr->m_nx < 0.0)
		   &&((fabs(l_sinRy)>S_VERY_SMALL_FLOAT)||(fabs(x_fr->m_ay)>S_VERY_SMALL_FLOAT)))
		{	// if cosRy > 0, and both cosRx and cosRz are < 0, choose Ry so that
			// cosRx and cosRz are positive
			if(*x_Ry >= 0.0)
				*x_Ry = S_PI - *x_Ry;
			else
				*x_Ry = -S_PI - *x_Ry;
			l_sign = -1.0;
		}
		*x_Rx = atan2(-l_sign * x_fr->m_ay, l_sign * x_fr->m_az);
		*x_Rz = atan2(-l_sign * x_fr->m_ox, l_sign * x_fr->m_nx);
	}
}

// ----------------------------------------------------------------
// _getSinCos
// ----------------------------------------------------------------
static void _getSinCos(double x_angle, double* x_sin, double* x_cos)
{
	double l_t, l_t2;

	// Too big angle involve numerical noise
	if(fabs(x_angle) > S_BIG_FLOAT)
	{
		x_angle = fmod(x_angle, 2.0*S_PI);
	}
	l_t = tan(x_angle/2.0);
	l_t2 = l_t*l_t;
	*x_sin = 2.0*l_t/(1.0+l_t2);
	*x_cos = (1.0-l_t2)/(1.0+l_t2);
}

// ----------------------------------------------------------------
// _setRxRyRzCoord
// ----------------------------------------------------------------
static void _setRxRyRzCoord
(
	const double x_Rx, 
	const double x_Ry, 
	const double x_Rz,
	LLI_Frame* x_fr
)
{
	double l_sinRx, l_sinRy, l_sinRz;
	double l_cosRx, l_cosRy, l_cosRz;

	_getSinCos(x_Rx, &l_sinRx, &l_cosRx);
	_getSinCos(x_Ry, &l_sinRy, &l_cosRy);
	_getSinCos(x_Rz, &l_sinRz, &l_cosRz);

	x_fr->m_nx =  l_cosRz * l_cosRy;
	x_fr->m_ny =  l_cosRz * l_sinRy * l_sinRx + l_sinRz * l_cosRx;
	x_fr->m_nz = -l_cosRz * l_sinRy * l_cosRx + l_sinRz * l_sinRx;

	x_fr->m_ox = -l_sinRz * l_cosRy;
	x_fr->m_oy =  l_cosRz * l_cosRx - l_sinRz * l_sinRy * l_sinRx;
	x_fr->m_oz =  l_cosRz * l_sinRx + l_sinRz * l_sinRy * l_cosRx;

	x_fr->m_ax =  l_sinRy;
	x_fr->m_ay = -l_cosRy * l_sinRx;
	x_fr->m_az =  l_cosRy * l_cosRx; 
}

// ----------------------------------------------------------------
// _displayFrame
// ----------------------------------------------------------------
static void _displayFrame (const LLI_Frame* x_fr)
{
	printf ("x=%f mm\n", x_fr->m_px*_M_TO_MM);
	printf ("y=%f mm\n", x_fr->m_py*_M_TO_MM);
	printf ("z=%f mm\n", x_fr->m_pz*_M_TO_MM);

	{
		double l_Rx, l_Ry, l_Rz;
		_getRxRyRzCoord (x_fr, &l_Rx, &l_Ry, &l_Rz);

		printf ("Rx=%f deg\n", l_Rx*_RAD_TO_DEG);
		printf ("Ry=%f deg\n", l_Ry*_RAD_TO_DEG);
		printf ("Rz=%f deg\n", l_Rz*_RAD_TO_DEG);
	}
}

// ----------------------------------------------------------------
// _checkJoint
// ----------------------------------------------------------------
static int _checkJoint(const LLI_Joint* x_jointOut, const LLI_Joint* x_jointWait, int x_size)
{
	int l_ret=1;
	int l_jnt;
	for (l_jnt=0;l_jnt<x_size;l_jnt++)
	{
		int l_err=((x_jointOut[l_jnt]-x_jointWait[l_jnt]) > S_SMALL_FLOAT);
		l_ret = l_ret && !l_err;
	}

	return l_ret;
}

// ----------------------------------------------------------------
// _checkTarget
// ----------------------------------------------------------------
static int _checkTarget(const LLI_Frame* x_frameOut, const LLI_Frame* x_frameWait)
{
	int l_ret=1;

	int l_err=((x_frameOut->m_ax-x_frameWait->m_ax) > S_SMALL_FLOAT);
	l_ret = l_ret && !l_err;
	l_err=((x_frameOut->m_ay-x_frameWait->m_ay) > S_SMALL_FLOAT);
	l_ret = l_ret && !l_err;
	l_err=((x_frameOut->m_az-x_frameWait->m_az) > S_SMALL_FLOAT);
	l_ret = l_ret && !l_err;

	l_err=((x_frameOut->m_ox-x_frameWait->m_ox) > S_SMALL_FLOAT);
	l_ret = l_ret && !l_err;
	l_err=((x_frameOut->m_oy-x_frameWait->m_oy) > S_SMALL_FLOAT);
	l_ret = l_ret && !l_err;
	l_err=((x_frameOut->m_oz-x_frameWait->m_oz) > S_SMALL_FLOAT);
	l_ret = l_ret && !l_err;

	l_err=((x_frameOut->m_nx-x_frameWait->m_nx) > S_SMALL_FLOAT);
	l_ret = l_ret && !l_err;
	l_err=((x_frameOut->m_ny-x_frameWait->m_ny) > S_SMALL_FLOAT);
	l_ret = l_ret && !l_err;
	l_err=((x_frameOut->m_nz-x_frameWait->m_nz) > S_SMALL_FLOAT);
	l_ret = l_ret && !l_err;

	l_err=((x_frameOut->m_px-x_frameWait->m_px) > S_SMALL_FLOAT);
	l_ret = l_ret && !l_err;
	l_err=((x_frameOut->m_py-x_frameWait->m_py) > S_SMALL_FLOAT);
	l_ret = l_ret && !l_err;
	l_err=((x_frameOut->m_pz-x_frameWait->m_pz) > S_SMALL_FLOAT);
	l_ret = l_ret && !l_err;

	return l_ret;
}

// ----------------------------------------------------------------
// testKin
// ----------------------------------------------------------------
void testKin (LLI_RobotId x_rob, int x_verbose)
{
	LLI_RobotType l_robotType;
	const char* l_robName="tx90";

	LLI_Status l_stat=LLI_ioctrl(x_rob, LLI_GET_ROBOT_TYPE, (void*)&l_robotType);
	assert (l_stat==LLI_OK);
	if ( strncmp(l_robotType, l_robName, strlen(l_robName))==0 )
	{
		int l_nbJnt;
		LLI_JointRange l_jointRange[_MAX_NB_JOINT];
		l_stat=LLI_ioctrl (x_rob, LLI_GET_JNT_NUMBER, &l_nbJnt);
		assert (l_stat==LLI_OK);
		assert (l_nbJnt<=_MAX_NB_JOINT);
		l_stat=LLI_ioctrl (x_rob, LLI_GET_JNT_RANGE, l_jointRange);
		assert (l_stat==LLI_OK);
		
		{
			LLI_Joint l_jointIn[_MAX_NB_JOINT];
			LLI_Frame l_target;
			LLI_Frame l_targetErr;
			LLI_Frame l_targetOut;
			LLI_Config l_config;
			LLI_Joint l_jointOut[_MAX_NB_JOINT];
			LLI_Joint l_joint[_MAX_NB_JOINT];
			LLI_ReversingResult l_result;
			int l_jnt;
			double l_tmp[2];

			l_target.m_px=0.50131784053065;
			l_target.m_py=0.41788169521755;
			l_target.m_pz=-0.07865660924855;
			l_target.m_nx=0.07945931129895;  
			l_target.m_ny=0.86237243569579;  
			l_target.m_nz=0.50000000000000;   
			l_target.m_ox=0.92361313213266; 
			l_target.m_oy=0.12500000000000;  
			l_target.m_oz=-0.36237243569579; 
			l_target.m_ax=-0.37500000000000;
			l_target.m_ay=0.49060043024044;
			l_target.m_az=-0.78656609248549;
			
			l_joint[0]=S_PI/6;
			l_joint[1]=S_PI/4;
			l_joint[2]=S_PI/2;
			l_joint[3]=S_PI/4;
			l_joint[4]=S_PI/3;
			l_joint[5]=S_PI/2;

			if (x_verbose==1)
			{
				printf ("target frame\n");
				_displayFrame (&l_target);
				printf ("target joint\n");
				displayJoint (x_rob, l_joint);
			}

			// inverse kin
			l_config.m_rxConfig.m_shoulder=LLI_SFREE;
			l_config.m_rxConfig.m_elbow=LLI_PNFREE;
			l_config.m_rxConfig.m_wrist=LLI_PNFREE;

			for (l_jnt=0;l_jnt<l_nbJnt;l_jnt++)
			{
				l_jointIn[l_jnt]=0;
			}
			l_stat=LLI_reverseKin 
			(
				x_rob,
				l_jointIn,
				&l_target,
				&l_config,
				l_jointRange,
				l_jointOut,
				&l_result
			);
			assert (l_stat==LLI_OK);
			if (x_verbose==1)
			{
				printf ("JointOut\n");
				displayJoint (x_rob, l_jointOut);
				printf ("JointWait\n");
				displayJoint (x_rob, l_joint);
			}
			assert (_checkJoint(l_jointOut, l_joint, l_nbJnt)!=0);

			// inverse kine out of range
			l_tmp[0]=l_jointRange[2].m_min;
			l_tmp[1]=l_jointRange[2].m_max;
			l_jointRange[2].m_min=-PI/3;
			l_jointRange[2].m_max=PI/3;
			l_stat=LLI_reverseKin 
			(
				x_rob,
				l_jointIn,
				&l_target,
				&l_config,
				l_jointRange,
				l_jointOut,
				&l_result
			);
			l_jointRange[2].m_min=l_tmp[0];
			l_jointRange[2].m_max=l_tmp[1];
			assert (l_stat==LLI_ERROR);
			assert (l_result==LLI_OUT_OF_JNT_RANGE);

			// inverse kine not constraint frame
			l_targetErr.m_px=0.4776;
			l_targetErr.m_py=0.3335;
			l_targetErr.m_pz=0.0707;
			l_targetErr.m_nx=0.0795;
			l_targetErr.m_ny=0.8624;
			l_targetErr.m_nz=0.5000;
			l_targetErr.m_ox=-0.7866;
			l_targetErr.m_oy=0.3624;
			l_targetErr.m_oz=-0.5000;
			l_targetErr.m_ax=-0.6124;
			l_targetErr.m_ay=-0.3536;
			l_targetErr.m_az=0.7071;
			l_stat=LLI_reverseKin 
			(
				x_rob,
				l_jointIn,
				&l_targetErr,
				&l_config,
				l_jointRange,
				l_jointOut,
				&l_result
			);
			assert (l_stat==LLI_ERROR);
			assert (l_result==LLI_UNCONSTRAINT_FRAME);

			// forward kine
			l_stat=LLI_forwardKin (x_rob, l_joint, &l_targetOut, &l_config);
			assert (l_stat==LLI_OK);
			assert (_checkTarget(&l_target, &l_targetOut)!=0);
			assert (l_config.m_rxConfig.m_shoulder==LLI_LEFTY);
			assert (l_config.m_rxConfig.m_elbow==LLI_POSITIVE);
			assert (l_config.m_rxConfig.m_wrist==LLI_POSITIVE);
		}
		printf ("\n\nKinematics test succeed %s\n\n", l_robName);
	}
}

// ----------------------------------------------------------------
// testKinOnly
// ----------------------------------------------------------------
void testKinOnly (const char* x_pathSys,const char* x_pathUsr,const char* x_pathLog)
{
	LLI_RobotId l_rob[2];
	LLI_Status l_stat;

	// check construction/destruction
	l_rob[0] = LLI_constructKinOnly(x_pathSys, x_pathUsr, x_pathLog);
	assert (l_rob[0]!=NULL);
	l_stat=LLI_destroy(&l_rob[0]);
	assert (l_stat==LLI_OK);

	// check construction/destruction 2 robots
	l_rob[0] = LLI_constructKinOnly(x_pathSys, x_pathUsr, x_pathLog);
	assert (l_rob[0]!=NULL);
	l_rob[1] = LLI_constructKinOnly(x_pathSys, x_pathUsr, x_pathLog);
	assert (l_rob[1]!=NULL);
	l_stat=LLI_destroy(&l_rob[1]);
	assert (l_stat==LLI_OK);
	l_stat=LLI_destroy(&l_rob[0]);
	assert (l_stat==LLI_OK);

	// Build a robot for test
	l_rob[0] = LLI_constructKinOnly(x_pathSys, x_pathUsr, x_pathLog);
	// run kinematic test
	testKin(l_rob[0], 1);
	l_stat=LLI_destroy(&l_rob[0]);
	assert (l_stat==LLI_OK);
}
