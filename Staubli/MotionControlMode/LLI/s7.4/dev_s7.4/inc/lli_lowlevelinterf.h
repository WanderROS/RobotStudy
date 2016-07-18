// *********************************************************************
// COPYRIGHT: This file is proprietary to Staubli SCA and can not be 
//            copied or used in this totality or partially without 
//            written authorization.
// ~NAME : lli_lowlevelinterf.h
// ~AUTHOR : L. Joly
// ~DESCRIPTION :
// **********************************************************************
#ifndef _LLI_LOWLEVELINTERF_H_
#define _LLI_LOWLEVELINTERF_H_

#ifndef S_EXPORT
#	ifdef WIN32
#		define S_EXPORT __declspec(dllimport)
#	else
#		define S_EXPORT extern
#	endif
#endif

// ----------------------------------------------------------------
// Types and structures
// ----------------------------------------------------------------
// LLI_RobotType type
#define	LLI_S_STR_ROBOT_TYPE	32	
typedef char LLI_RobotType[LLI_S_STR_ROBOT_TYPE+1];
// LLI_SoftwareVersion type
#define	LLI_S_STR_SOFT_VERSION	32	
typedef char LLI_SoftwareVersion[LLI_S_STR_SOFT_VERSION+1];

// Return code
typedef enum _LLI_Status
{
	LLI_OK,  
	LLI_ERROR
} LLI_Status;

// Init state
typedef enum _LLI_InitState
{
	LLI_NO_LICENCE,
	LLI_NOT_INITIALIZED,
	LLI_INITIALIZED
} LLI_InitState;

// Enable state
typedef enum _LLI_EnableState
{
	LLI_DISABLED,
	LLI_ENABLING,
	LLI_ENABLED,
	LLI_DISABLING,
} LLI_EnableState;

// Calibration state
typedef enum _LLI_CalibState
{
	LLI_CALIBRATED,
	LLI_NOT_CALIBRATED,
} LLI_CalibState;

// Settle state
typedef enum _LLI_SettleState
{
	LLI_NOT_SETTLED,
	LLI_SETTLED
} LLI_SettleState;

// Estop State
typedef enum _LLI_EstopState
{
	LLI_ESTOP,
	LLI_NO_ESTOP,
} LLI_EstopState;

// LLI State (function LLI_state)
typedef struct _LLI_State
{
	LLI_InitState		m_initState;
	LLI_EnableState 	m_enableState;
	LLI_CalibState		m_calibrateState;
	LLI_SettleState		m_settleState;
	LLI_EstopState		m_estopState;
} LLI_State;

// Ioctrl code
typedef enum _LLI_IoctrlCode
{
	LLI_GET_SOFTWARE_VERSION,
	LLI_GET_ROBOT_TYPE,
	LLI_GET_JNT_NUMBER,
	LLI_GET_CYCLE_TIME,
	LLI_GET_SYNCHRO_TASK_PRIORITY,
	LLI_SET_ABS_JNT_POS,
	LLI_RECOVER_ABS_JNT_POS,
	LLI_GET_USER_MARKS,
	LLI_SET_USER_MARKS,
	LLI_SET_TORQUE_MODE,
	LLI_SET_POS_VEL_MODE,
	LLI_GET_JNT_RANGE,
	LLI_SET_TUNING
} LLI_IoctrlCode;

// Io type
typedef enum _LLI_IoType
{
	IO_D_INPUT,
	IO_D_OUTPUT,
	IO_A_INPUT,
	IO_A_OUTPUT,
	IO_UNDEFINED
} LLI_IoType;

// Dio status (function LLI_readDio)
typedef enum _LLI_DioStatus
{
	DIO_ONE,
	DIO_ZERO,
	DIO_UNDEFINED
} LLI_DioStatus;

// Dout value (function LLI_writeDout)
typedef enum _LLI_DoutValue
{
	DOUT_ONE,
	DOUT_ZERO
} LLI_DoutValue;

// Io Id
typedef struct _LLI_IoId
{
	LLI_IoType		m_type;		// UNDEFINED if Io not found
	void*			m_pIo;		// NULL if Io not found
} LLI_IoId;

// LLI_PlugIn is a pointer on a function "void function(void)"
typedef void (*LLI_PlugIn)(void);

// LLI_EventFunc is a pointer on a function "void function(char *)"
typedef void (*LLI_EventFunc)(char *);

// Robot ID
struct _LLI_Robot;
typedef struct _LLI_Robot LLI_Robot;
typedef LLI_Robot* LLI_RobotId;

// LLI command (at the joint level, after motor reduction)
typedef struct _LLI_JointCmd
{
	double 	m_pos;			/* rad or m */
	double 	m_vel;			/* rad/s or m/s */
	double	m_torqueFfw;	/* N.m */
} LLI_JointCmd;

// LLI feedbacks (at the joint level, after motor reduction)
typedef struct _LLI_JointFbk
{
	double 	m_pos;			/* rad or m */
	double 	m_vel;			/* rad/s or m/s */
	double 	m_posErr;		/* rad or m */
	double 	m_torque;		/* N.m */
} LLI_JointFbk;

// Cartesian position 
// nx	ox	ax	px
// ny	oy	ay	py
// nz	oz	az	pz
//  0	 0	 0	 1
typedef struct _LLI_Frame
{
	/*rot*/
	double m_nx;
	double m_ny;
	double m_nz;
	double m_ox;
	double m_oy;
	double m_oz;
	double m_ax;
	double m_ay;
	double m_az;
	/*pos*/
	double m_px;
	double m_py;
	double m_pz;
} LLI_Frame;

// Joint position
typedef double LLI_Joint;

// Joint range
typedef struct _LLI_JointRange
{
	LLI_Joint m_min;
	LLI_Joint m_max;
} LLI_JointRange;

// Robot configuration
typedef enum _LLI_ShoulderConfig
{
	LLI_SSAME=0,
	LLI_LEFTY=1,
	LLI_RIGHTY=2,
	LLI_SFREE=3
} LLI_ShoulderConfig;

typedef enum _LLI_PositiveNegativeConfig
{
	LLI_PNSAME=0,
	LLI_POSITIVE=1,
	LLI_NEGATIVE=2,
	LLI_PNFREE=3
} LLI_PositiveNegativeConfig;

typedef struct _LLI_RxConfig
{
	LLI_ShoulderConfig			m_shoulder;
	LLI_PositiveNegativeConfig	m_elbow;
	LLI_PositiveNegativeConfig	m_wrist;
} LLI_RxConfig;

typedef struct _LLI_ScaraConfig
{
	LLI_ShoulderConfig	m_shoulder;
} LLI_ScaraConfig;

typedef union _LLI_Config
{
	LLI_RxConfig	m_rxConfig;
	LLI_ScaraConfig	m_scaraConfig;
} LLI_Config;

// Inverse kinematics result
typedef enum _LLI_ReversingResult
{
	LLI_REVERSE_OK = 0,	
	LLI_NO_CONVERGENCE,
	LLI_OUT_OF_JNT_RANGE,
	LLI_OUT_OF_WORKSPACE,
	LLI_INVALID_CONFIG,
	LLI_INVALID_ORIENTATION,
	LLI_UNSUPPORTED_KINEMATICS,
	LLI_UNCONSTRAINT_FRAME,
	LLI_INVALID_ERROR_CODE
} LLI_ReversingResult;

// ----------------------------------------------------------------
// Functions prototypes
// ----------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

S_EXPORT LLI_RobotId LLI_construct
(
	LLI_PlugIn x_pFunct, 
	const char* x_pathSys, 
	const char* x_pathUsr,  
	const char* x_pathLog
);
S_EXPORT LLI_RobotId LLI_constructKinOnly
(
	const char* x_pathSys, 
	const char* x_pathUsr,  
	const char* x_pathLog
);
S_EXPORT LLI_Status	LLI_destroy(LLI_RobotId* x_pRobotId);
S_EXPORT LLI_Status	LLI_init(LLI_RobotId x_robotId);
S_EXPORT LLI_Status	LLI_enable(LLI_RobotId x_robotId);
S_EXPORT LLI_Status	LLI_disable(LLI_RobotId x_robotId);
S_EXPORT LLI_Status	LLI_get(LLI_RobotId x_robotId, LLI_JointFbk x_jntFbk[]);
S_EXPORT LLI_Status	LLI_set(LLI_RobotId x_robotId, LLI_JointCmd x_jntCmd[]);
S_EXPORT LLI_Status	LLI_state(LLI_RobotId x_robotId, LLI_State* x_pState);
S_EXPORT LLI_Status	LLI_plugEventFunc(LLI_RobotId x_robotId, LLI_EventFunc x_pFunc);
S_EXPORT LLI_Status	LLI_ioctrl(LLI_RobotId x_robotId, int x_code, void* x_pIoctrlData);
S_EXPORT LLI_Status	LLI_getDinId(LLI_RobotId x_robotId, char* x_pDioStr, LLI_IoId* x_pDinId);
S_EXPORT LLI_Status	LLI_getDoutId(LLI_RobotId x_robotId, char* x_pDioStr, LLI_IoId* x_pDoutId);
S_EXPORT LLI_Status	LLI_readDio(LLI_IoId* x_pDioId, LLI_DioStatus* x_pDioStatus);
S_EXPORT LLI_Status	LLI_writeDout(LLI_IoId* x_pDoutId, LLI_DoutValue x_doutValue);
S_EXPORT LLI_Status LLI_reverseKin
(
	LLI_RobotId x_robotId, 
	const LLI_Joint x_jointIn[], 
	const LLI_Frame* x_cTarget, 
	const LLI_Config* x_config,
	const LLI_JointRange x_jointRange[],
	LLI_Joint x_jointOut[], 
	LLI_ReversingResult* x_reversingResult
);
S_EXPORT LLI_Status LLI_forwardKin
(
	LLI_RobotId x_robotId, 
	const LLI_Joint x_jointIn[], 
	LLI_Frame* x_position,
	LLI_Config* x_config
);

// ----------------------------------------------------------------
// Recorder-related functions
// ----------------------------------------------------------------

S_EXPORT LLI_Status LLI_recInit(LLI_RobotId x_robotId);
S_EXPORT LLI_Status LLI_recStart(LLI_RobotId x_robotId);
S_EXPORT LLI_Status LLI_recStop(LLI_RobotId x_robotId);
S_EXPORT LLI_Status LLI_recStore(LLI_RobotId x_robotId);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif //_LLI_LOWLEVELINTERF_H_
