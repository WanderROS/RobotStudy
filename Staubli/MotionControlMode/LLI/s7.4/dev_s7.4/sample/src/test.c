// *********************************************************************
// COPYRIGHT: This file is proprietary to Staubli SCA and can not be 
//            copied or used in this totality or partially without 
//            written authorization.
// ~NAME : test.c
// ~AUTHOR : L. Joly
// ~DESCRIPTION :
// **********************************************************************

#include <math.h>
#include <assert.h>
#include <string.h>
#include "_test.h"
#include "systemfunctions.h"
#include "isr.h"
#include "lli_lowlevelinterf.h"
#include "synchro.h"
#include "algo1.h"
#include "algo2.h"
#ifdef VXWORKS // Needed to configure COM1 (function _initCom)
	#include <iolib.h>
	#include <loglib.h>
	#include <usrlib.h>
	#include <tasklib.h>
#endif

#define _RAD_TO_DEG (180.0 / PI)
#define _DEG_TO_RAD (PI / 180.0)
#define _M_TO_MM (1000.0)
#define _MM_TO_M (0.001)
#define _MAX_NB_JOINT (6)

#define S_SMALL_FLOAT		(1.e-5)
#define S_VERY_SMALL_FLOAT	(1.e-10)
#define S_PI				PI
#define S_BIG_FLOAT			1.e10

static LLI_EventFunc _pEventFunc=NULL;
static LLI_RobotId _robotId = NULL;

// ----------------------------------------------------------------
// _state
// ----------------------------------------------------------------
char _isScara(LLI_RobotId x_robotId)
{
	char l_ret = 0;
	LLI_RobotType l_robotType;
	LLI_ioctrl(x_robotId, LLI_GET_ROBOT_TYPE, (void*)&l_robotType);
	if((l_robotType[0]=='r') && (l_robotType[1]=='s'))
		l_ret = 1;		// Return TRUE is name starts with 'rs'
	return l_ret;
}

// ----------------------------------------------------------------
// _state
// ----------------------------------------------------------------
static void _state(LLI_RobotId x_robotId)
{
	char l_isScara = _isScara(x_robotId);
	unsigned int	l_jntNb;
	unsigned int	l_jnt;
	LLI_State		l_state;
	LLI_JointCmd	l_jntCmd[MAX_NB_JNT];
	LLI_JointFbk	l_jntFbk[MAX_NB_JNT];

	// get robot state
	LLI_state(x_robotId, &l_state);
	// Get number of joints
	LLI_ioctrl(x_robotId, LLI_GET_JNT_NUMBER, &l_jntNb);

	// display init state
	printf("Init state =           %s\n",
		( l_state.m_initState == LLI_INITIALIZED)?		"LLI_INITIALIZED":
		( l_state.m_initState == LLI_NOT_INITIALIZED)?	"LLI_NOT_INITIALIZED":
		( l_state.m_initState == LLI_NO_LICENCE)?	"LLI_NO_LICENCE":
														"????");
	// display enable state
	printf("Enable state =         %s\n",
		( l_state.m_enableState == LLI_ENABLED)?	"LLI_ENABLED":
		( l_state.m_enableState == LLI_ENABLING)?	"LLI_ENABLING":
		( l_state.m_enableState == LLI_DISABLED)?	"LLI_DISABLED":
		( l_state.m_enableState == LLI_DISABLING)?	"LLI_DISABLING":
													"????");
	// display calibrate state
	printf("Calibrate state =      %s\n",
		( l_state.m_calibrateState == LLI_CALIBRATED)?		"LLI_CALIBRATED":
		( l_state.m_calibrateState == LLI_NOT_CALIBRATED)?	"LLI_NOT_CALIBRATED":
															"????");

	// display settle state
	printf("Settle state =         %s\n",
		( l_state.m_settleState == LLI_SETTLED)?		"LLI_SETTLED":
		( l_state.m_settleState == LLI_NOT_SETTLED)?	"LLI_NOT_SETTLED":
														"????");
	// display emergency stop state
	printf("Emergency stop state = %s\n",
		( l_state.m_estopState == LLI_ESTOP)?		"LLI_ESTOP":
		( l_state.m_estopState == LLI_NO_ESTOP)?	"LLI_NO_ESTOP":
													"????");

	printf("\n");

	// display commands and feebacks
	getSynchroJntCmd(l_jntCmd);
	getSynchroJntFbk(l_jntFbk);
	printf("pos cmd : ");
	for (l_jnt=0; l_jnt<l_jntNb; l_jnt++)
		if((l_isScara)&&(l_jnt==SCARA_LINEAR_JNT))
			printf("  %7.3f", l_jntCmd[l_jnt].m_pos*_M_TO_MM);
		else
			printf("  %7.3f", l_jntCmd[l_jnt].m_pos*_RAD_TO_DEG);
	printf("\nvel cmd : ");
	for (l_jnt=0; l_jnt<l_jntNb; l_jnt++)
		if((l_isScara)&&(l_jnt==SCARA_LINEAR_JNT))
			printf("  %7.3f", l_jntCmd[l_jnt].m_vel*_M_TO_MM);
		else
			printf("  %7.3f", l_jntCmd[l_jnt].m_vel*_RAD_TO_DEG);
	printf("\npos fbk : ");
	for (l_jnt=0; l_jnt<l_jntNb; l_jnt++)
		if((l_isScara)&&(l_jnt==SCARA_LINEAR_JNT))
			printf("  %7.3f", l_jntFbk[l_jnt].m_pos*_M_TO_MM);
		else
			printf("  %7.3f", l_jntFbk[l_jnt].m_pos*_RAD_TO_DEG);
	printf("\nvel fbk : ");
	for (l_jnt=0; l_jnt<l_jntNb; l_jnt++)
		if((l_isScara)&&(l_jnt==SCARA_LINEAR_JNT))
			printf("  %7.3f", l_jntFbk[l_jnt].m_vel*_M_TO_MM);
		else
			printf("  %7.3f", l_jntFbk[l_jnt].m_vel*_RAD_TO_DEG);
	printf("\npos err : ");
	for (l_jnt=0; l_jnt<l_jntNb; l_jnt++)
		if((l_isScara)&&(l_jnt==SCARA_LINEAR_JNT))
			printf("  %7.3f", l_jntFbk[l_jnt].m_posErr*_M_TO_MM);
		else
			printf("  %7.3f", l_jntFbk[l_jnt].m_posErr*_RAD_TO_DEG);
	printf("\ntorque  : ");
	for (l_jnt=0; l_jnt<l_jntNb; l_jnt++)
		printf("  %7.3f", l_jntFbk[l_jnt].m_torque);
	printf("\n\n");
}

// ----------------------------------------------------------------
// _enable
// ----------------------------------------------------------------
static void _enable(LLI_RobotId x_robotId)
{
	printf("enabling robot\n");
	//  ~IF initialization fails
	if (LLI_enable(x_robotId) != LLI_OK)
	{ //  ~THEN
		printf("enable returned ERROR\n");
	} else
	{ //  ~ELSE 
		printf("enable returned OK\n");
	} // ~ENDIF 
}

// ----------------------------------------------------------------
// _disable
// ----------------------------------------------------------------
static void _disable(LLI_RobotId x_robotId)
{
	printf("disabling robot\n");
	//  ~IF initialization fails
	if (LLI_disable(x_robotId) != LLI_OK)
	{ //  ~THEN
		printf("disable returned ERROR\n");
	} else
	{ //  ~ELSE 
		printf("disable returned OK\n");
	} // ~ENDIF 
}

// ----------------------------------------------------------------
// _move
// ----------------------------------------------------------------
static void _move(LLI_RobotId x_robotId)
{
	int		l_jnt;

	printf("\nWhich joint (with sign) (e.g. -3 -> joint 3 moves in negative direction\n");
	scanf("%d", &l_jnt);
	getchar();
	move(x_robotId, l_jnt);
}

// ----------------------------------------------------------------
// _setGains
// ----------------------------------------------------------------
static void _setGains(LLI_RobotId x_robotId)
{
	unsigned int l_jntNb;
	unsigned int l_jnt;
	double	l_gain[MAX_NB_JNT];
	int		l_value;

	// Get number of joints
	LLI_ioctrl(x_robotId, LLI_GET_JNT_NUMBER, &l_jntNb);

	printf("WARNING: changing gains while power is enabled is dangerous\n");

	//~FOR each joint
	for (l_jnt=0; l_jnt<l_jntNb; l_jnt++)
	{
		printf("\nWhich gain on joint %d\n",(l_jnt+1));
		scanf("%d", &l_value);
		getchar();
		l_gain[l_jnt] = l_value;
	} //~ENDFOR 

	printf("Gains set to:\n");
	//~FOR each joint
	for (l_jnt=0; l_jnt<l_jntNb; l_jnt++)
		printf("\t%f\n",l_gain[l_jnt]);

	setGain(l_jntNb, l_gain);
}

// ----------------------------------------------------------------
// _recorderStart
// ----------------------------------------------------------------
static void _recorderStart(LLI_RobotId x_robotId)
{
	if(LLI_recInit(x_robotId) != LLI_OK)
	{
		printf("Error during recorder initialization\n");
		return;
	}

	if(LLI_recStart(x_robotId) != LLI_OK)
	{
		printf("Error during record\n");
		return;
	}
}

// ----------------------------------------------------------------
// _recorderStop
// ----------------------------------------------------------------
static void _recorderStop(LLI_RobotId x_robotId)
{
	if(LLI_recStop(x_robotId) != LLI_OK)
	{
		printf("Error during stop\n");
		return;
	}

	if(LLI_recStore(x_robotId) != LLI_OK)
	{
		printf("Error during storage\n");
		return;
	}

	printf("Record succefully done and stored !!\n");
}

// ----------------------------------------------------------------
// _getSwVersion
// ----------------------------------------------------------------
static void _getSwVersion(LLI_RobotId x_robotId)
{
	LLI_SoftwareVersion l_softVer;

	if (LLI_ioctrl(x_robotId, LLI_GET_SOFTWARE_VERSION, (void*) (l_softVer) ) == LLI_OK)
	{ //  ~THEN 
		printf("Software version is %s\n\n", l_softVer);
	} else
	{ //  ~ELSE 
		printf("LLI_ioctrl returned LLI_ERROR\n\n");
	} // ~ENDIF 
}

// ----------------------------------------------------------------
// _getRobotType
// ----------------------------------------------------------------
static void _getRobotType(LLI_RobotId x_robotId)
{
	LLI_RobotType l_robotType;

	if (LLI_ioctrl(x_robotId, LLI_GET_ROBOT_TYPE, (void*) (l_robotType) ) == LLI_OK)
	{ //  ~THEN 
		printf("Robot type: %s\n\n", l_robotType);	
	} else
	{ //  ~ELSE 
		printf("LLI_ioctrl returned LLI_ERROR\n\n");
	} // ~ENDIF 
}

// ----------------------------------------------------------------
// _getCycleTime
// ----------------------------------------------------------------
static void _getCycleTime(LLI_RobotId x_robotId)
{
	double l_cycleTime;

	if (LLI_ioctrl(x_robotId, LLI_GET_CYCLE_TIME, (void*) (&l_cycleTime)) == LLI_OK)
	{ //  ~THEN 
		printf("Cycle time is %f second\n\n", l_cycleTime);
	} else
	{ //  ~ELSE 
		printf("LLI_ioctrl returned LLI_ERROR\n\n");
	} // ~ENDIF 
}

// ----------------------------------------------------------------
// _getPriority
// ----------------------------------------------------------------
static void _getPriority(LLI_RobotId x_robotId)
{
	int l_priority;

	if (LLI_ioctrl(x_robotId, LLI_GET_SYNCHRO_TASK_PRIORITY, (void*) (&l_priority)) == LLI_OK)
	{ //  ~THEN 
		printf("Synchronous task priority is %d \n\n", l_priority);
	} else
	{ //  ~ELSE 
		printf("LLI_ioctrl returned LLI_ERROR\n\n");
	} // ~ENDIF 
}

// ----------------------------------------------------------------
// _setAbsJntPos
// ----------------------------------------------------------------
static void _setAbsJntPos(LLI_RobotId x_robotId)
{
	double l_absJntPos[MAX_NB_JNT] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

	printf("Setting absolute joint position to 0 on all axes\n");
	if (LLI_ioctrl(x_robotId, LLI_SET_ABS_JNT_POS, (void*) (l_absJntPos) ) == LLI_OK)
	{ //  ~THEN 
		printf("OK\n\n");	
	} else
	{ //  ~ELSE 
		printf("LLI_ioctrl returned LLI_ERROR\n\n");
	} // ~ENDIF 
}

// ----------------------------------------------------------------
// displayJoint
// ----------------------------------------------------------------
void displayJoint (LLI_RobotId x_robotId, const LLI_Joint* x_jntPos)
{
	unsigned int l_ind;
	unsigned int l_jntNb;
	char l_isScara = _isScara(x_robotId);

	// Get number of joints
	if (LLI_ioctrl(x_robotId, LLI_GET_JNT_NUMBER, &l_jntNb)==LLI_OK)
	{
		for (l_ind = 0; l_ind <l_jntNb; l_ind++)
		{
			if((l_isScara)&&(l_ind==SCARA_LINEAR_JNT))
				printf(" %f", x_jntPos[l_ind]*_M_TO_MM);
			else
				printf(" %f", x_jntPos[l_ind]*_RAD_TO_DEG);
		}
	}
}

// ----------------------------------------------------------------
// _recoverAbsJntPos
// ----------------------------------------------------------------
static void _recoverAbsJntPos(LLI_RobotId x_robotId)
{
	double l_absJntPos[MAX_NB_JNT];

	printf("Recover joint position to user marks\n");
	if (LLI_ioctrl(x_robotId, LLI_GET_USER_MARKS, (void*) (l_absJntPos) ) == LLI_OK)
	{ //  ~THEN 
		displayJoint (x_robotId, l_absJntPos);
		printf("\n");
		if (LLI_ioctrl(x_robotId, LLI_RECOVER_ABS_JNT_POS, (void*) (l_absJntPos) ) == LLI_OK)
			printf("OK\n\n");	
		else
			printf("LLI_ioctrl returned LLI_ERROR\n\n");
	} else
		printf("LLI_ioctrl returned LLI_ERROR\n\n");
}

// ----------------------------------------------------------------
// _getUserMarks
// ----------------------------------------------------------------
static void _getUserMarks(LLI_RobotId x_robotId)
{
	double l_userMarks[MAX_NB_JNT];
	unsigned int l_ind;

	if (LLI_ioctrl(x_robotId, LLI_GET_USER_MARKS, (void*) (l_userMarks) ) == LLI_OK)
	{ //  ~THEN 
		char l_isScara = _isScara(x_robotId);
		unsigned int l_jntNb;
		// Get number of joints
		LLI_ioctrl(x_robotId, LLI_GET_JNT_NUMBER, &l_jntNb);

		printf("User marks (degrees/mm):");
		//~FOR 
		for (l_ind = 0; l_ind <l_jntNb; l_ind++)
			if ((l_isScara)&&(l_ind==SCARA_LINEAR_JNT))
				printf(" %f", l_userMarks[l_ind]*_M_TO_MM);
			else
				printf(" %f", l_userMarks[l_ind]*_RAD_TO_DEG);
		printf("\n");
	} else
		printf("LLI_ioctrl returned LLI_ERROR\n\n");
}

// ----------------------------------------------------------------
// _setUserMarks
// ----------------------------------------------------------------
static void _setUserMarks(LLI_RobotId x_robotId)
{
	double l_userMarks[MAX_NB_JNT] = {90.0*_DEG_TO_RAD, 91.0*_DEG_TO_RAD, 92.0*_DEG_TO_RAD, 
		                           93.0*_DEG_TO_RAD, 94.0*_DEG_TO_RAD, 95.0*_DEG_TO_RAD};

	printf("Setting user marks to {90 91 92 ...} (degrees/mm)\n");
	if(_isScara(x_robotId))
		l_userMarks[SCARA_LINEAR_JNT] = 92.0*_MM_TO_M;
	if (LLI_ioctrl(x_robotId, LLI_SET_USER_MARKS, (void*) (l_userMarks) ) == LLI_OK)
		printf("OK\n\n");	
	else
		printf("LLI_ioctrl returned LLI_ERROR\n\n");
}

// ----------------------------------------------------------------
// _setTuning
// ----------------------------------------------------------------
static void _setTuning(LLI_RobotId x_robotId)
{
	char l_cmd[100];

	l_cmd[0]=0;
	printf("Enter command:");
	scanf("%[^\r\n]", l_cmd);
	getchar();
	if (LLI_ioctrl(x_robotId, LLI_SET_TUNING, (void*)l_cmd) == LLI_OK)
		printf("OK\n\n");	
	else
		printf("LLI_ioctrl returned LLI_ERROR, command status:%s\n\n", l_cmd);
}

static int _confirmation (LLI_State x_state)
{
	int l_ret=0;
	int l_rep='y';

	//~IF power is enabled
	if (x_state.m_enableState == LLI_ENABLED)
	{
		printf("Warning: Power is on, Do you want to change mode(y/n)\n");
		l_rep=getchar();
	}
	if ( (l_rep=='y') || (l_rep=='Y') )
	{
		l_ret = 1;
	}
	else
	{
		l_ret = 0;
		printf ("Change mode cancel\n");
	}

	return l_ret;
}

// ----------------------------------------------------------------
// _setTorqueMode
// ----------------------------------------------------------------
static void _setTorqueMode(LLI_RobotId x_robotId)
{
	// get robot state
	LLI_State		l_state;
	LLI_state(x_robotId, &l_state);

	printf("\nSetting torque mode in progress...\n");
	//~IF power is disabled
	if ( _confirmation(l_state)==1 )
	{	//~THEN 
		if (LLI_ioctrl(x_robotId, LLI_SET_TORQUE_MODE, (void*) (NULL) ) == LLI_OK)
		{ //  ~THEN 
			// switch to algo2
			setAlgo(2);

			printf("\nTorque mode set OK\n\n\n");	
			printf("*****************************************************\n\n");
			printf("WARNING !! Working in torque mode may be dangerous.\n");
			printf("Make sure to set correct gains before enabling power.\n");
			printf("Risk of unexpected arm movement, instability, robot\n");
			printf("damage, and injury !!!\n");
			printf("*****************************************************\n\n");
		} else
		{ //  ~ELSE 
			printf("LLI_ioctrl returned an error\n");
		} // ~ENDIF 
	} //~ENDIF 
}

// ----------------------------------------------------------------
// _setPosVelMode
// ----------------------------------------------------------------
static void _setPosVelMode(LLI_RobotId x_robotId)
{
	// get robot state
	LLI_State		l_state;
	LLI_state(x_robotId, &l_state);

	printf("\nSetting Pos Vel mode in progress...\n");
	if ( _confirmation(l_state)==1 )
	{
		if (LLI_ioctrl(x_robotId, LLI_SET_POS_VEL_MODE, (void*) (NULL) ) == LLI_OK)
		{ //  ~THEN 
			// switch to algo2
			setAlgo(1);

			printf("\nPos Vel mode set OK\n\n\n");	
		} else
		{ //  ~ELSE 
			printf("LLI_ioctrl returned an error\n\n");
		} // ~ENDIF 
	}
}

// ----------------------------------------------------------------
// _getJntRange
// ----------------------------------------------------------------
static void _getJntRange(LLI_RobotId x_robotId)
{
	LLI_JointRange l_jointRange[_MAX_NB_JOINT];

	if (LLI_ioctrl(x_robotId, LLI_GET_JNT_RANGE, l_jointRange ) == LLI_OK)
	{ //  ~THEN 
		LLI_Joint l_jntMin[_MAX_NB_JOINT];
		LLI_Joint l_jntMax[_MAX_NB_JOINT];
		int l_jnt;

		for (l_jnt=0;l_jnt<_MAX_NB_JOINT;l_jnt++)
		{
			l_jntMin[l_jnt]=l_jointRange[l_jnt].m_min;
			l_jntMax[l_jnt]=l_jointRange[l_jnt].m_max;
		}
		printf ("Min values:\n");
		displayJoint (x_robotId, l_jntMin);
		printf ("\n");
		printf ("Max values:\n");
		displayJoint (x_robotId, l_jntMax);
	} else
	{ //  ~ELSE 
		printf("LLI_ioctrl returned an error\n\n");
	} // ~ENDIF 
}

// ----------------------------------------------------------------
// _errorFunc
// ----------------------------------------------------------------
static void _eventFunc(char* x_errStr)
{
	printf("Event received: %s\n", x_errStr);
}

// ----------------------------------------------------------------
// _plugEventFunc
// ----------------------------------------------------------------
static void _plugEventFunc(LLI_RobotId x_robotId)
{
	//~IF 
	if (_pEventFunc==NULL)
	{	//~THEN 
		printf("Plug event function\n");
		_pEventFunc = _eventFunc;
	} else
	{	//~ELSE 
		printf("Unplug event function\n");
		_pEventFunc =NULL;
	} //~ENDIF 
	if (LLI_plugEventFunc(x_robotId, _pEventFunc) == LLI_OK)
	{ //  ~THEN 
		printf("OK\n\n");	
	} else
	{ //  ~ELSE 
		printf("LLI_plugEventFunct returned LLI_ERROR\n\n");
	} // ~ENDIF 
}

// ----------------------------------------------------------------
// _ioctrlMenu
// ----------------------------------------------------------------
static void _ioctrlMenu(LLI_RobotId x_robotId)
{
	char	l_choice;

	// ~DO 
	do {
		// Print menu
		printf("\t\n\n");
		printf("\t\t 0:\tExit ioctrl menu\n");
		printf("\t\t 1:\tLLI_GET_SOFTWARE_VERSION\n");
		printf("\t\t 2:\tLLI_GET_ROBOT_TYPE\n");
		printf("\t\t 3:\tLLI_GET_CYCLE_TIME\n");
		printf("\t\t 4:\tLLI_GET_SYNCHRO_TASK_PRIORITY\n");
		printf("\t\t 5:\tLLI_SET_ABS_JNT_POS\n");
		printf("\t\t 6:\tLLI_RECOVER_ABS_JNT_POS\n");
		printf("\t\t 7:\tLLI_GET_USER_MARKS\n");
		printf("\t\t 8:\tLLI_SET_USER_MARKS\n");
		if (getAlgo()==1)
			printf("\t\t 9:\tSET_TORQUE_MODE\n");
		if (getAlgo()==2)
			printf("\t\t a:\tSET_POS_VEL_MODE\n");
		printf("\t\t b:\tLLI_GET_JNT_RANGE\n");
		printf("\t\t c:\tLLI_SET_TUNING\n");

		printf ("\n\t Selection ? ");

		l_choice = getchar();
		fflush (stdin);
		printf ("\n\n");

		// ~SWITCH 
		switch (l_choice)
		{
			case '0':	/*nothing*/						break;
			case '1':	_getSwVersion(x_robotId);		break;
			case '2':	_getRobotType(x_robotId);		break;
			case '3':	_getCycleTime(x_robotId);		break;
			case '4':	_getPriority(x_robotId);		break;
			case '5':	_setAbsJntPos(x_robotId);		break;
			case '6':	_recoverAbsJntPos(x_robotId);	break;
			case '7':	_getUserMarks(x_robotId);		break;
			case '8':	_setUserMarks(x_robotId);		break;
			case '9':	_setTorqueMode(x_robotId);		break;
			case 'a':	_setPosVelMode(x_robotId);		break;
			case 'b':	_getJntRange(x_robotId);		break;
			case 'c':	_setTuning(x_robotId);			break;
		} // ~ENDSWITCH 
	} while (l_choice != '0');
}

// ----------------------------------------------------------------
// _getDoutId
// ----------------------------------------------------------------
static void _getDoutId(LLI_RobotId x_robotId, char* x_pDioStr, LLI_IoId* x_pDoutId)
{
	//~IF Dout not found 
	if (LLI_getDoutId(x_robotId, x_pDioStr, x_pDoutId) == LLI_ERROR)
	{	//~THEN
		printf("Error: Dout %s not found\n", x_pDioStr);
	} //~ENDIF 
}

// ----------------------------------------------------------------
// _getDinId
// ----------------------------------------------------------------
static void _getDinId(LLI_RobotId x_robotId, char* x_pDioStr, LLI_IoId* x_pDinId)
{
	//~IF Din not found 
	if (LLI_getDinId(x_robotId, x_pDioStr, x_pDinId) == LLI_ERROR)
	{	//~THEN
		printf("Error: Din %s not found\n", x_pDioStr);
	} //~ENDIF 
}

// ----------------------------------------------------------------
// _readDio
// ----------------------------------------------------------------
static void _readDio(LLI_IoId* x_pDio)
{
	LLI_DioStatus l_status;

	//~IF Dio found 
	if (x_pDio->m_pIo != NULL)
	{	//~THEN
		//~IF reading OK
		if (LLI_readDio(x_pDio, &l_status)==LLI_OK)
		{	//~THEN 
			printf("Dio Status: %c\n\n", 
				(l_status==DIO_ONE)?'1':(l_status==DIO_ZERO)?'0':'x');
		} else
		{	//~ELSE 
			printf("LLI_readDio returned LLI_ERROR\n\n");
		} //~ENDIF 
	} else
	{	//~ELSE 
		printf("Error: Dio not found\n\n");
	} //~ENDIF 
}

// ----------------------------------------------------------------
// _writeDout
// ----------------------------------------------------------------
static void _writeDout(LLI_IoId* x_pDout)
{
	LLI_DoutValue l_value;
	char		  l_val;

	//~IF Dio found 
	if (x_pDout->m_pIo != NULL)
	{	//~THEN
		// Get Dout value
		printf("\n\tEnter the value (0/1): ");
		l_val = getchar();
		fflush (stdin);
		printf ("\n");
		// Convert char to LLI_DoutValue
		l_value = (l_val=='1') ? DOUT_ONE : DOUT_ZERO;
		//~IF writing OK
		if (LLI_writeDout(x_pDout, l_value)==LLI_OK)
		{	//~THEN 
			printf("LLI_writeDout returned LLI_OK\n\n");
		} else
		{	//~ELSE 
			printf("LLI_writeDout returned LLI_ERROR\n\n");
		} //~ENDIF 
	} else
	{	//~ELSE 
		printf("Error: Dout not found\n\n");
	} //~ENDIF 
}

// ----------------------------------------------------------------
// _dioMenu
// ----------------------------------------------------------------
static void _dioMenu(LLI_RobotId x_robotId)
{
	char	 l_choice;
	LLI_IoId l_doutValve1;
	LLI_IoId l_bOut0;
	LLI_IoId l_bIn0;
	LLI_IoId l_mOut0;

	// Get Dout id
	_getDoutId(x_robotId, "valve1", &l_doutValve1);
	_getDoutId(x_robotId, "bOut0", &l_bOut0);
	_getDoutId(x_robotId, "mOut0", &l_mOut0);
	// Get Din id
	_getDinId(x_robotId, "bIn0", &l_bIn0);

	// ~DO 
	do {
		// Print menu
		printf("\t\n\n");
		printf("\t\t 0:\tExit dio menu\n");
		printf("\t\t 1:\tWrite dout 'valve1'\n");
		printf("\t\t 2:\tRead  dout 'valve1'\n");
		printf("\t\t 3:\tRead  din  'bIn0'\n");
		printf("\t\t 4:\tWrite dout 'bOut0'\n");
		printf("\t\t 5:\tRead  dout 'bOut0'\n");
		printf("\t\t 6:\tWrite dout 'mOut0'\n");
		printf("\t\t 7:\tRead  dout 'mOut0'\n");

		printf ("\n\t Selection ? ");

		l_choice = getchar();
		fflush (stdin);
		printf ("\n\n");

		// ~SWITCH 
		switch (l_choice)
		{
			case '0':	/*nothing*/					break;
			case '1':	_writeDout(&l_doutValve1);	break;
			case '2':	_readDio(&l_doutValve1);	break;
			case '3':	_readDio(&l_bIn0);			break;
			case '4':	_writeDout(&l_bOut0);		break;
			case '5':	_readDio(&l_bOut0);			break;
			case '6':	_writeDout(&l_mOut0);		break;
			case '7':	_readDio(&l_mOut0);			break;
		} // ~ENDSWITCH 
	} while (l_choice != '0');
}

// ----------------------------------------------------------------
// _initCom
// init COM 1
// ----------------------------------------------------------------
static void _initCom(void)
{
#ifdef VXWORKS
	int l_op;

	// open COM 1
	l_op = open("/tyCo/0", O_RDWR | O_CREAT, 0);
	//~IF open OK
	if (l_op != ERROR)
	{	//~THEN redirect output/input on COM1
		ioGlobalStdSet(STD_IN, l_op);
		ioGlobalStdSet(STD_OUT, l_op);
		ioGlobalStdSet(STD_ERR, l_op);
		// COM SETUP
		(void) ioctl (l_op, FIOSETOPTIONS,
			OPT_ECHO | OPT_CRMOD | OPT_TANDEM | OPT_7_BIT);

	} //~ENDIF 
	// Stop Shell task 
	td(taskNameToId("tShell"));
#endif
}

// ----------------------------------------------------------------
// _stop
// ----------------------------------------------------------------
static void _stop(LLI_RobotId* x_pRobotId)
{
	// stop synchronous task
	stopSynchroTask();
	// destroy LLI_Robot
	//  ~IF destruction fails
	if ( LLI_destroy(x_pRobotId) != LLI_OK)
		printf("destruction failed\n");
	else
		printf("destruction OK\n");
}

// ----------------------------------------------------------------
// menu
// ----------------------------------------------------------------
void menu(void)
{
	char l_choice = 0;
#ifdef VXWORKS
	int l_stdinOptions = ioctl(STD_IN, FIOGETOPTIONS, 0);
	if((l_stdinOptions & OPT_LINE)!=0)
		ioctl(STD_IN, FIOSETOPTIONS, (l_stdinOptions & (~OPT_LINE)));
#endif
	// ~DO 
	while((_robotId != NULL)&&(l_choice != 'q'))
	{
		// Print menu
		printf("\t\n\n");
#ifdef VXWORKS
		printf("\t\t q:\tQuit menu. Enter 'menu' to start it again\n");
#endif
		
		printf("\t\t 1:\tState and errors\n");
		printf("\t\t 2:\tEnable\n");
		printf("\t\t 3:\tDisable\n");
		if (getAlgo()==1)
			printf("\t\t 4:\tMove\n");
		printf("\t\t 5:\tPlug event function\n");
		printf("\t\t 6:\tIoctrl menu\n");
		printf("\t\t 7:\tDio menu\n");
		printf("\t\t 8:\tConfigure COM1\n");
		if (getAlgo()==2)
			printf("\t\t 9:\tSet gains\n");
		printf("\t\t a:\tRecorder (init and start)\n");
		printf("\t\t b:\tRecorder (stop and store)\n");
		printf("\n");
		printf("\t\t z:\tStop LLI\n");
		printf("\n");
		printf("\t Selection ? ");

		l_choice = getchar();
		fflush (stdin);
		printf ("\n\n");

		// ~SWITCH 
		switch (l_choice)
		{
			case 'z':	_stop(&_robotId);				break;
			case '1':	_state(_robotId);				break;
			case '2':	_enable(_robotId);				break;
			case '3':	_disable(_robotId);			break;
			case '4':	if(getAlgo()==1)
							_move(_robotId);			break;
			case '5':	_plugEventFunc(_robotId);		break;
			case '6':	_ioctrlMenu(_robotId);			break;
			case '7':	_dioMenu(_robotId);			break;
			case '8':	_initCom();						break;
			case '9':	if(getAlgo()==2)
							_setGains(_robotId);		break;
			case 'a':	_recorderStart(_robotId);			break;
			case 'b':	_recorderStop(_robotId);			break;

			case 'q': 
#ifndef VXWORKS
				l_choice = 0;
#endif
				break;
			default: break;
		} // ~ENDSWITCH 
	}
	printf("bye!\n");
#ifdef VXWORKS
	ioctl (STD_IN, FIOSETOPTIONS, l_stdinOptions);
#endif
	// ~ENDDO 
}

// ----------------------------------------------------------------
// testControl
// ----------------------------------------------------------------
void testControl (const char* x_pathSys,const char* x_pathUsr,const char* x_pathLog)
{
	SEM_ID		l_isrSemId;
	unsigned int l_jntNb;
	LLI_State		l_state;

	// create binary semaphore and pass it to sercos ISR
	l_isrSemId = createSemB();
	if (l_isrSemId == NULL)
		printf("Unable to create semaphore!!\n");
	setIsrSem(l_isrSemId);

	// construct LLI_Robot
	printf("Constructing robot\n");
	_robotId = LLI_construct(&isr, x_pathSys, x_pathUsr, x_pathLog);
	//  ~IF construction fails
	if (_robotId == NULL)
		printf("construction failed\n");
	else
		printf("construction OK\n");

	// Get number of axes
	LLI_ioctrl(_robotId, LLI_GET_JNT_NUMBER, &l_jntNb);

	// init algorithms
	initAlgo1();
	initAlgo2(l_jntNb);

	// spawn synchronous task (waiting on semaphore)
	startSynchroTask(l_isrSemId, _robotId,1);

	// initialize robot
	printf("initializing robot\n");
	//  ~IF initialization fails
	if (LLI_init(_robotId) != LLI_OK)
		printf("initialization failed\n");
	else
		printf("initialization OK\n");

	printf("\n\n\n\n");
	printf("\t\t***************************\n");
	printf("\t\t*   Tests of LLI module   *\n");
	printf("\t\t***************************\n");
	printf("\n\n");
	// run kin test
	testKin (_robotId, 0);
	// get robot state
	LLI_state(_robotId, &l_state);
	// display init state
	if((l_state.m_initState==LLI_NO_LICENCE))
	{
		printf("\t\tNo LLI licence found in /usr/configs/controller.cfx.\n");
		_stop(&_robotId);
	}
	else
	{
#ifdef WIN32
		menu();
#else
		printf("\t\tCtrl-C to get VxWorks prompt, then 'menu' to start LLI test menu.\n");
#endif

	}
}

//#define _TEST_KIN_ONLY
void test(const char* x_pathSys,const char* x_pathUsr,const char* x_pathLog)
{
#ifdef _TEST_KIN_ONLY
	testKinOnly(x_pathSys, x_pathUsr, x_pathLog);
#else
	testControl(x_pathSys, x_pathUsr, x_pathLog);
#endif
}

// ----------------------------------------------------------------
// main
// ----------------------------------------------------------------
#ifdef WIN32
void main(void)
{
	test(NULL, NULL, NULL);
}
#else
void CS8_start(const char* x_pathSys,const char* x_pathUsr,const char* x_pathLog)
{
	test(x_pathSys, x_pathUsr, x_pathLog); 
}
#endif	// WIN32

