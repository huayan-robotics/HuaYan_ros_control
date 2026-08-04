/*******************************************************************
 *	Copyright(c) 2020-2025 
 *  All rights reserved.
 *	
 *	FileName:HR_Pro.h
 *	Descriptio:C++SDK
 *  version:1.0.16.0
 *	Modification Records:
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 *   SDK Version                    Date                                                     Add                                                                         Modify                                Delete
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 *   V1.0.0.0                       2022.05.12                                               Create a file
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 *   V1.0.0.1                       2023.02.17                                               HRIF_ReadOverride                                                                                                    
 *                                                                                           HRIF_ReadPointByName                                                                                                    
 *                                                                                           HRIF_SetFreeDriveMotionFreedom                                                                 
 *                                                                                           HRIF_SetPoseTrackingMaxMotionLimit                                                                     
 *                                                                                           HRIF_SetPoseTrackingPIDParams                                                                     
 *                                                                                           HRIF_SetPoseTrackingTargetPos                                                                      
 *                                                                                           HRIF_SetPoseTrackingState                                                                      
 *                                                                                           HRIF_SetUpdateTrackingPose                                                                     
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 *   V1.0.2.1                       2023.03.21                                               HRIF_GetErrorCodeStr                                                                             
 *                                                                                           HRIF_MoveCAngle                                                                              
 *                                                                                           HRIF_MoveLinearWeave                                                                             
 *                                                                                           HRIF_MoveCircularWeave                                                                           
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 *   V1.0.3.1                       2023.06.27                                               HRIF_EnterSafetyGuard                                                                         
 *                                                                                           HRIF_GetCircularViaPose                                                                        
 *                                                                                           HRIF_EndPushMovePath                                                                         
 *                                                                                           HRIF_ConfigTCP                                                                              
 *                                                                                           HRIF_ConfigUCS                                                                              
 *                                                                                           HRIF_MoveToSS                                                                              
 *                                                                                           HRIF_SetCollideLevel                                                                     
 *                                                                                           HRIF_ReadMaxPayload                                                                          
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 *   V1.0.4.0                       2024.03.04                                               HRIF_SetSimulation                                                                        
 *                                                                                           HRIF_SetMoveParamsAO                                                                     
 *                                                                                           HRIF_CalUcsPlane                                                                         
 *                                                                                           HRIF_CalUcsLine                                                                             
 *                                                                                           HRIF_CalTcp3P                                                                           
 *                                                                                           HRIF_CalTcp4P                                                                     
 *                                                                                           HRIF_SetFreeDriveMotionFreedom                                                              
 *                                                                                           HRIF_SetFTFreeFactor                                                                     
 *                                                                                           HRIF_SetTangentForceBounds                                                                    
 *                                                                                           HRIF_SetFreeDriveCompensateForce                                                             
 *                                                                                           HRIF_InitPathJ                                                                             
 *                                                                                           HRIF_InitPathL                                                                              
 *                                                                                           HRIF_PushPathPoints                                                                       
 *                                                                                           HRIF_EndPushPathPoints                                                                    
 *                                                                                           HRIF_DelPath                                                                            
 *                                                                                           HRIF_ReadPathList                                                                        
 *                                                                                           HRIF_ReadPathInfo                                                                        
 *                                                                                           HRIF_UpdatePathName                                                                     
 *                                                                                           HRIF_ReadPathState                                                                     
 *                                                                                           HRIF_ReadTrackProcess                                                                    
 *                                                                                           HRIF_SpeedJ                                                                              
 *                                                                                           HRIF_SetPoseTrackingStopTimeOut                                                           
 *                                                                                           HRIF_GetCollideLevel                                                                     
 *                                                                                           HRIF_SetECATAO                                                                          
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 *   V1.0.5.0                       2024.03.11                                               HRIF_InitPath                                                                        
 *                                                                                           HRIF_EndPushMovePath                                                                    
 *                                                                                                                                                                                                             HRIF_InitPathJ(Delete)  
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 *   V1.0.5.1                       2024.03.25                                               HRIF_OpenBrake                                                                         
 *                                                                                           HRIF_CloseBrake                                                                     
 *                                                                                           HRIF_ReadBrakeStatus                                                                     
 *                                                                                           HRIF_ReadPayload                                                                     
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 *   V1.0.6.0                       2024.04.09                                               HRIF_SetFTWrenchThresholds                                                              
 *                                                                                           HRIF_SetMaxFreeDriveVel                                                                    
 *                                                                                           HRIF_ReadFTMotionFreedom                                                                 
 *                                                                                           HRIF_SetMaxSearchDistance                                                              
 *                                                                                           HRIF_SetSteadyContactDeviationRange                                                       
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 *   V1.0.6.1                       2024.04.09                                               No changes                                                                     
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 *   V1.0.7.0                       2024.09.18                                               HRIF_XToStandby                                                                         
 *                                                                                           HRIF_EnableEndBTN                                                                    
 *                                                                                           HRIF_cdsSetIO                                                                     
 *                                                                                           HRIF_ReadPointList                                                                      
 *                                                                                           HRIF_ReadTCPList                                                                      
 *                                                                                           HRIF_ReadUCSList                                                                      
 *                                                                                           HRIF_SetFTFreeDriveSpeedMode                                                           
 *                                                                                           HRIF_ReadFTFreeDriveSpeedMode                                                                 
 *                                                                                           HRIF_MovePathJOL                                                                     
 *                                                                                           HRIF_GetMovePathJOLIndex                                                                  
 *                                                                                           HRIF_SpeedL                                                                     
 *                                                                                           HRIF_SpeedJ
 *                                                                                           HRIF_ReadMaxRange    
 *                                                                                           HRIF_MoveAlignToZ      
 *                                                                                           HRIF_AddSafePlane  
 *                                                                                           HRIF_DelSafePlane      
 *                                                                                           HRIF_ReadSafePlaneList                                       
 *                                                                                           HRIF_ReadSafePlane            
 *                                                                                           HRIF_UpdateSafePlane
 *                                                                                           HRIF_SetDepthThresholdForDampingArea
 *                                                                                           HRIF_SetTriStageSwitch
 *                                                                                           HRIF_ReadTriStageSwitch
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 *   V1.0.8.0                       2024.10.28                                               HRIF_SetJointMaxVel_nJ                     
 *                                                                                           HRIF_SetJointMaxAcc_nJ
 *                                                                                           HRIF_SetMaxAcsRange_nJ
 *                                                                                           HRIF_ReadCmdJointPos_nJ
 *                                                                                           HRIF_ReadCmdJointCur_nJ
 *                                                                                           HRIF_ReadActJointCur_nJ
 *                                                                                           HRIF_ReadActJointPos_nJ
 *                                                                                           HRIF_ReadCmdJointVel_nJ
 *                                                                                           HRIF_ReadActJointVel_nJ
 *                                                                                           HRIF_GetInverseKin_nJ
 *                                                                                           HRIF_GetForwardKin_nJ
 *                                                                                           HRIF_WayPointRel_nJ
 *                                                                                           HRIF_MoveJ_nJ
 *                                                                                           HRIF_MoveL_nJ
 *                                                                                           HRIF_MoveC_nJ
 *                                                                                           HRIF_ReadJointMaxVel_nJ
 *                                                                                           HRIF_ReadJointMaxJerk_nJ
 *                                                                                           HRIF_ReadJointMaxAcc_nJ
 *                                                                                           HRIF_ReadPointByName_nJ
 *                                                                                           HRIF_ReadActPos_nJ
 *                                                                                           HRIF_WayPointEx_nJ
 *                                                                                           HRIF_WayPoint2_nJ
 *                                                                                           HRIF_WayPoint_nJ
 *                                                                                           HRIF_ReadAxisErrorCode_nJ
 *                                                                                           HRIF_SwitchScript
 *                                                                                           HRIF_ReadDefaultScript
 *                                                                                           HRIF_EnterSafeGuard
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 *   V1.0.9.0                       2025.01.16                                               HRIF_ReadRobotType
 *                                                                                           HRIF_ClearCoControlExpandAxisPos            
 *                                                                                           HRIF_SetRotationVelocityControlMode
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 *   V1.0.10.0                       2025.03.24                                              HRIF_SetBaseInstallingAngle
 *                                                                                           HRIF_GetBaseInstallingAngle
 *                                                                                           HRIF_GetLoadIdentifyResult
 *                                                                                           HRIF_LoadIdentify
 *                                                                                           HRIF_GetLastCalibParams
 *                                                                                           HRIF_SetInitializeForceSensor
 *                                                                                           HRIF_SetFTCalibration
 *                                                                                           HRIF_ReadForceData
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 *   V1.0.11.0                       2025.03.25                                              HRIF_StartWeaveWeld
 *                                                                                           HRIF_SetWeaveParams
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 *   V1.0.12.0                       2025.03.25                                              HRIF_LongMoveEvent
 *                                                                                           HRIF_ShortJogJ
 *                                                                                           HRIF_ShortJogL
 *                                                                                           HRIF_LongJogJ
 *                                                                                           HRIF_LongJogL
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 *   V1.0.13.0                       2025.04.15                                              HRIF_CalTCPOrt
 *                                                                                           HRIF_SetDftTCP
 *                                                                                           HRIF_ReadActCoord
 *                                                                                           HRIF_ReadActCoord_nJ
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 *   V1.0.14.0                       2025.08.12                                              HRIF_GetInverseDynamic
 *                                                                                           HRIF_SetPathRefJoints
 *                                                                                           HRIF_ReadAllBoxCI
 *                                                                                           HRIF_ReadAllBoxCO
 *                                                                                           HRIF_ReadAllBoxDI
 *                                                                                           HRIF_ReadAllBoxDO
 *                                                                                           HRIF_SetFTUCS
 *                                                                                           HRIF_ReadFTUCS
 *                                                                                           HRIF_HRAppData
 *                                                                                           HRIF_SetEndModbusInfo
 *   V1.0.14.1                       2025.08.12                                                                                                                          MoveC
 *                                                                                                                                                                       MoveC_nJ
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 *   V1.0.15.0                       2025.11.19                                              HRIF_MoveC2
 *                                                                                           HRIF_SetReduceMode
 *                                                                                           HRIF_ReadReduceMode
 *                                                                                           HRIF_MoveLByLockAxis_nJ
 *                                                                                           HRIF_MoveCByLockAxis_nJ
 *                                                                                           HRIF_WayPoint2ByLockAxis_nJ
 *                                                                                                                                                                                                             HRIF_SetLogDbgCB
 *                                                                                                                                                                                                             HRIF_SetEventCB
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 *   V1.0.16.0                       2026.7.1                                                HRIF_ReadRobotModelRbt
 *                                                                                           HRIF_ReadSafetyPlaneList
 *                                                                                           HRIF_ReadSafetyPlane
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 ***************************************************************************************************************************************/

#ifndef _HR_PRO_H_
#define _HR_PRO_H_


#include <string>
#include <vector>
using namespace std;

#define MaxBox 5
#define MaxJointNums 15

#ifdef _MSC_VER    // MSVC compiler 
    #ifdef ROBOT_PRO_EXPORTS
        #define ROBOT_PRO_API __declspec(dllexport)  
    #else  
        #define ROBOT_PRO_API __declspec(dllimport)  
    #endif  
#elif (__MINGW32__ || __MINGW64__)    // MinGW compiler  
    #ifdef ROBOT_PRO_EXPORTS
        #define ROBOT_PRO_API __attribute__((dllexport))  
    #else  
        #define ROBOT_PRO_API __attribute__((dllimport))  
    #endif  
#else
    #define ROBOT_PRO_API  
#endif

#ifdef __cplusplus
extern "C"
{
#endif


//************************************************************************/
//**   Interfaces for initialization                                                            
//************************************************************************/
/**
 *	@brief: Connect to port 10003 of CPS 
 *	@param boxID : Control box ID, 0 as default 
 *	@param hostName : IP address of CPS 
 *	@param nPort : Port number, 10003 as default
 *	
 *	@return : nRet=0 : Function call succeeded
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_Connect(unsigned int boxID, const char* hostName, unsigned short nPort);

/**
 *	@brief: Disconnet from port 10003 of CPS 
 *	@param boxID : Control box ID, 0 as default 
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call
 */
ROBOT_PRO_API int HRIF_DisConnect(unsigned int boxID);

/**
 *	@brief: Check the connection status to CPS 
 *	@param boxID : Control box ID, 0 as default
 *	@return : false : Disconnected 
 *            true : Connected
 */
ROBOT_PRO_API bool HRIF_IsConnected(unsigned int boxID);

/**
 *	@brief: Power off the robot and shut down the system 
 *	@param boxID : Control box ID, 0 as default 
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ShutdownRobot(unsigned int boxID);

/**
 *	@brief: Connect to control box 
 *	@param boxID : Control box ID, 0 as default 
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_Connect2Box(unsigned int boxID);

/**
 *	@brief: Power on the robot 
 *	@param boxID : Control box ID, 0 as default 
  *	@return : nRet=0 : Function call succeeded
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_Electrify(unsigned int boxID);

/**
 *	@brief: Power off the robot 
 *	@param boxID : Control box ID, 0 as default 
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_Blackout(unsigned int boxID);

/**
 *	@brief: Connect to the controller, start master, initialize slave, configure and check paramters, finally switch to DISABLE state 
 *	@param boxID : Control box ID, 0 as default 
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_Connect2Controller(unsigned int boxID);

/**
 *	@brief: Check the simulation status 
 *	@param boxID : Control box ID, 0 as default
 *	@param nSimulateRobot :Simulation status
 *	                           0 : real mode 
 *                             1 : simulated mode 
 *
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_IsSimulateRobot(unsigned int boxID, int& nSimulateRobot);

/**
 *	@brief: Check the simulation status 
 *	@param boxID : Control box ID, 0 as default 
 *	@param nSimulation :Simulation status
 *	                           0 : real mode 
 *                             1 : simulated mode
 *
 *	@return : nRet=0 : Function call succeeded
 *            nRet>0 : Error code of function call
 */
ROBOT_PRO_API int HRIF_SetSimulation(unsigned int boxID, unsigned int rbtID, int nSimulation);

/**
 *	@brief: Check the controller's state 
 *	@param boxID : Control box ID, 0 as default
 *	@param nStarted :Start status
 *	                      0 : not started
 *                        1 : started 
 * 
 *	@return : nRet=0 : Function call succeeded
 *            nRet>0 : Error code of function call
 */
ROBOT_PRO_API int HRIF_IsControllerStarted(unsigned int boxID, int& nStarted);

/**
 *	@brief: Read the version of robot 
 *          Version consists of digits separated by '.'
 *  CPSVer.controlVer.BoxVerMajor.BoxVerMid.BoxVerMin.AlgorithmVer.ElfinFirmwareVer

 *	@param rbtID : Robot ID, 0 as default 
 *	@param strVer : Whole version 
 *	@param nCPSVersion : CPS version 
 *	@param nCodesysVersion : Controller version 
 *	@param nBoxVerMajor : Control box version, 0 for simulated, 1~4 for real 
 *	@param nBoxVerMid : Firmware version of control board 
 *	@param nBoxVerMin : Firmware version of control board 
 *	@param nAlgorithmVer : Algorithm version 
 *	@param nElfinFirmwareVer : Firmware version
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call
 */
ROBOT_PRO_API int HRIF_ReadVersion(unsigned int boxID, unsigned int rbtID, string& strVer, int& nCPSVersion, int& nCodesysVersion,
                     int &nBoxVerMajor, int &nBoxVerMid, int &nBoxVerMin,
                     int &nAlgorithmVer, int &nElfinFirmwareVer);

/**
 *	@brief: Get explanation of error code 
 *	@param boxID : Control box ID, 0 as default 
 *	@param nErrorCode : Error code 
 *  @param strErrorMsg : Error code explanation 
 *
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_GetErrorCodeStr(unsigned int boxID, int nErrorCode, string &strErrorMsg);

/**
 *	@brief: Read the robot model 
 *	@param boxID : Control box ID, 0 as default
 *	@param strModel : Robot model 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadRobotModel(unsigned int boxID, string& strModel);

/**
 *	@brief: Read the robot model 
 *	@param boxID : Control box ID, 0 as default
 *	@param rbtID : Robot ID, 0 as default 
 *	@param strModel : Robot model 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadRobotModelRbt(unsigned int boxID, unsigned int rbtID, string& strModel);

/**
 *	@brief: Read the robot model 
 *	@param boxID : Control box ID, 0 as default 
 *	@param RobotType : Robot Type 
 *	@param MovelSet: MovelSet 
 *	@param Axist6Type: The Sixth axis type
 *  @param Axist7Type: The Seventh  axis type
 *  @param Axist8Type: The Eighth  axis type
 *  @param Axist9Type: The Ninth axis type
 *  @param Axist10Type: The Tenth axis type
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadRobotType(unsigned int boxID, int& RobotType, string& MovelSet, int& Axist6Type, int& Axist7Type, int& Axist8Type, int& Axist9Type, int& Axist10Type);

/**
 *	@brief: Read the robot type by robot id
 *	@param boxID : Control box ID, 0 as default
 *	@param rbtID : Robot ID, 0 as default
 *	@param RobotType : Robot Type
 *	@param MovelSet: MovelSet
 *	@param Axist6Type: The Sixth axis type
 *  @param Axist7Type: The Seventh  axis type
 *  @param Axist8Type: The Eighth  axis type
 *  @param Axist9Type: The Ninth axis type
 *  @param Axist10Type: The Tenth axis type
 *	@return : nRet=0 : Function call succeeded
 *            nRet>0 : Error code of function call
 */
ROBOT_PRO_API int HRIF_ReadRobotTypeRbt(unsigned int boxID, unsigned int rbtID, int& RobotType, string& MovelSet, int& Axist6Type, int& Axist7Type, int& Axist8Type, int& Axist9Type, int& Axist10Type);


//************************************************************************/
//**   Axis group control command interface 
//************************************************************************/
/**
 *	@brief: Enable the robot 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_GrpEnable(unsigned int boxID, unsigned int rbtID);

/**
 *	@brief: Disable the robot 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_GrpDisable(unsigned int boxID, unsigned int rbtID);

/**
 *	@brief: Reset the robot 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_GrpReset(unsigned int boxID, unsigned int rbtID);

/**
 *	@brief: Stop robot moving 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_GrpStop(unsigned int boxID, unsigned int rbtID);

/**
 *	@brief: Pause robot moving 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_GrpInterrupt(unsigned int boxID, unsigned int rbtID);

/**
 *	@brief: Continue robot moving after pause 
 *	@param boxID : Control box number, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_GrpContinue(unsigned int boxID, unsigned int rbtID);

/**
 *	@brief: Open free driver 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_GrpOpenFreeDriver(unsigned int boxID, unsigned int rbtID);

/**
 *	@brief: Close free driver 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_GrpCloseFreeDriver(unsigned int boxID, unsigned int rbtID);

/**
 *	@brief: Open the brake 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nAxisID : Robot Axis ID,0-5 
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_OpenBrake(unsigned int boxID, unsigned int rbtID, int nAxisID);

/**
 *	@brief: Close the brake 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nAxisID : Robot Axis ID,0-5 
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_CloseBrake(unsigned int boxID, unsigned int rbtID, int nAxisID);

/**
 *	@brief: Read the brake status of each Axis 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param stateJ1~J6 : Robot Axis status 
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadBrakeStatus(unsigned int boxID,unsigned int rbtID,int& stateJ1,int& stateJ2,int& stateJ3,int& stateJ4,int& stateJ5,int& stateJ6);

/**
 *	@brief: Read whether the three-stage switch is turned on and the mode 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nThreeStageEnable : Three stage switch  
 *                                                              0:colse
 *                                                              1:open
 *  @param nThreeStageMode : Mode 
 *                                                              0:FreeDrive
 *                                                              1:Enable
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadTriStageSwitch(unsigned int boxID, unsigned int rbtID,int&  nThreeStageEnable, int& nThreeStageMode);

/**
 *	@brief: Set three-stage switch and mode 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nThreeStageEnable : Three stage switch  
 *                                                              0:colse
 *                                                              1:open
 *  @param nThreeStageMode : Mode 
 *                                                              0:FreeDrive
 *                                                              1:Enable
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetTriStageSwitch(unsigned int boxID, unsigned int rbtID, int  nThreeStageEnable, int nThreeStageMode);

/**
 *	@brief: Z-axis alignment
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param TcpName : Tcp Name  
 *  @param UcsName : Ucs Name 
 *  @param bIsReached : Is the Z-axis aligned
 *  @param dbJ1-dbJ6 : J1-J6 Joint Position 
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_MoveAlignToZ(unsigned int boxID, unsigned int rbtID, string TcpName, string UcsName,bool &bIsReached,double &dbJ1,double &dbJ2,double &dbJ3,double &dbJ4,double &dbJ5,double &dbJ6);

//************************************************************************/
//**    Interfaces for script 
//************************************************************************/
/**
 *	@brief: Run the specified function 
 *	@param boxID : Control box ID, 0 as default 
 *	@param strFuncName : The specified function name composed in the teaching pendant 
 *	@param param :  Parameters 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_RunFunc(unsigned int boxID, string strFuncName, vector<string> param);

/**
 *	@brief: Start a task to bring the robot to Standby state from other states, replacing other multiple commands.  After the task is launched, the system checks the current state and sends related commands automaticallly. If the system can't return to Standby state within 30s, the task will end. 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *  @return : nRet=0 : Task launched successfully 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_XToStandby(unsigned int boxID, unsigned int rbtID);

/**
 *	@brief: Run the main function to start the script execution composed and compiled in the teaching pendant 
 *	@param boxID : Control box ID, 0 as default 
 *
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_StartScript(unsigned int boxID);

/**
 *	@brief: Stop the script execution running in teaching pendant 
 *	@param boxID : Control box ID, 0 as default 
 *
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_StopScript(unsigned int boxID);

/**
 *	@brief: Pause the script execution running in teaching pendant 
 *	@param boxID : Control box ID, 0 as default 
 *
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_PauseScript(unsigned int boxID);

/**
 *	@brief:Continue the script execution paused int the teaching pendant 
 *	@param rbtID : Robot ID, 0 as default 
 *
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ContinueScript(unsigned int boxID);

/**
 *	@brief:Applies the specified script file 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *  @param strScriptName : ScriptName 
 *
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SwitchScript(unsigned int boxID, unsigned int rbtID, string strScriptName);

/**
 *	@brief:Read the current application script file 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *  @param strScriptName : ScriptName 
 *
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadDefaultScript(unsigned int boxID, unsigned int rbtID, string &strScriptName);

//************************************************************************/
//**    Interfaces for control box 
//************************************************************************/
/**
 *	@brief: Read box message
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : robot ID, 0 as default 
 *	@param nConnected : Control box connection status 
 *	@param n48V_ON : Voltage value with 48V 
 *	@param d48OUT_Voltag : 48V output voltage 
 *	@param d48OUT_Current : 48V output current 
 *	@param nRemoteBTN : Remote emergency stop status 
 *	@param nThreeStageBTN : Three stage status 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadBoxInfo(unsigned int boxID, unsigned int rbtID,
                    int &nConnected, 
                    int &n48V_ON, 
                    double &d48OUT_Voltag, 
                    double &d48OUT_Current, 
                    int &nRemoteBTN, 
                    int &nThreeStageBTN);

/**
 *	@brief: Read box control input 
 *	@param boxID : Control box ID, 0 as default 
 *	@param nBit : CI bit 
 *	@param nVal : CI value 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadBoxCI(unsigned int boxID, int nBit, int& nVal);

/**
 *	@brief: Read box digital input 
 *	@param boxID : Control box ID, 0 as default 
 *	@param nBit : DI bit 
 *	@param nVal : DI value 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadBoxDI(unsigned int boxID, int nBit, int& nVal);

/**
 *	@brief: Read box control output 
 *	@param boxID : Control box ID, 0 as default 
 *	@param nBit : CO bit 
 *	@param nVal : CO value 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadBoxCO(unsigned int boxID, int nBit, int& nVal);

/**
 *	@brief: Read box digital output 
 *	@param boxID : Control box ID, 0 as default 
 *	@param nBit : DO bit 
 *	@param nVal : DO value 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadBoxDO(unsigned int boxID, int nBit, int& nVal);

/**
 *	@brief: Read all control box config digital input statuses 
 *	@param boxID : Control box ID, 0 by default 
 *	@param Val : Vector storing all config input (CI) values 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Function call error code 
 */
ROBOT_PRO_API int HRIF_ReadAllBoxCI(unsigned int boxID, vector<int>& Val);

/**
 *	@brief: Read all control box general digital input statuses 
 *	@param boxID : Control box ID, 0 by default 
 *	@param Val : Vector storing all digital input (DI) values 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Function call error code 
 */
ROBOT_PRO_API int HRIF_ReadAllBoxDI(unsigned int boxID, vector<int>& Val);

/**
 *	@brief: Read all control box config digital output statuses 
 *	@param boxID : Control box ID, 0 by default 
 *	@param Val : Vector storing all config output (CO) values 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Function call error code 
 */
ROBOT_PRO_API int HRIF_ReadAllBoxCO(unsigned int boxID, vector<int>& Val);

/**
 *	@brief: Read all control box general digital output statuses 
 *	@param boxID : Control box ID, 0 by default 
 *	@param Val : Vector storing all digital output (DO) values 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Function call error code 
 */
ROBOT_PRO_API int HRIF_ReadAllBoxDO(unsigned int boxID, vector<int>& Val);

/**
 *	@brief: Read box analog input 
 *	@param boxID : Control box ID, 0 as default 
 *	@param nBit : AI bit 
 *	@param dbVal : Current (4~20mA) or voltage (0~10V) 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadBoxAI(unsigned int boxID, int nBit, double& dbVal);

/**
 *	@brief: Read box analog output 
 *	@param boxID : Control box ID, 0 as default 
 *	@param nBit : AO bit 
 *	@param nMode : Current or voltage mode 
 *	@param dbVal : Current value (4~20mA) or voltage value (0~10V) 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadBoxAO(unsigned int boxID, int nBit, int& nMode, double& dbVal);

/**
 *	@brief: Set Box control output 
 *	@param boxID : Control box ID, 0 as default 
 *	@param nBit : CO bit 
 *	@param nVal : CO value 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetBoxCO(unsigned int boxID, int nBit, int nVal);

/**
 *	@brief: Set Box digital output 
 *	@param boxID : Control box ID, 0 as default 
 *	@param nBit : DO bit 
 *	@param nVal : DO value 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetBoxDO(unsigned int boxID, int nBit, int nVal);

/**
 *	@brief: Set Box analog output mode 
 *	@param boxID : Control box ID, 0 as default 
 *	@param nBit : AO bit 
 *	@param nMode : AO mode 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetBoxAOMode(unsigned int boxID, int nBit, int nMode);

/**
 *	@brief: Set Box analog output 
 *	@param boxID : Control box ID, 0 as default 
 *	@param nBit : AO bit 
  *	@param dbVal : AO value 
 *	@param nMode : AO mode 

 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetBoxAOVal(unsigned int boxID, int nBit, double dbVal, int nMode);

/**
 *	@brief: Read End digital input 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nBit : End DI bit 
 *	@param nVal : End DI value 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadEndDI(unsigned int boxID, unsigned int rbtID, int& nBit, int& nVal);

/**
 *	@brief: Read End digital output 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nBit : End DO bit 
 *	@param nVal : End DO value 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadEndDO(unsigned int boxID, unsigned int rbtID, int& nBit, int& nVal);

/**
 *	@brief: Set End digital output 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nBit : End DO bit 
 *	@param nVal : End DO value 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetEndDO(unsigned int boxID, unsigned int rbtID, int nBit, int nVal);

/**
 *	@brief: Read End analog input 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nBit :  End AI bit 
 *	@param nVal : End AI value 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadEndAI(unsigned int boxID, unsigned int rbtID, int& nBit, double& dVal);

/**
 *	@brief: Read End Button 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nBit1 : End button 1 
 *	@param nBit2 : End button 2 
 *	@param nBit3 : End button 3 
 *	@param nBit4 : End button 4 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadEndBTN(unsigned int boxID, unsigned int rbtID, int& nBit1, int& nBit2, int& nBit3, int& nBit4);

/**
 *	@brief: Set End Button 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nStatus : End button status, 1 for enabled, 0 for enabled 
 *
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_EnableEndBTN(unsigned int boxID, unsigned int rbtID, int nStatus);

/**
 *	@brief: Set IO before the motion command reaches the target point 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nEndDOMask : The EndDO to be modified is identified by bit
 *  @param nEndDOVal :  The target state of each EndDO to be modified.
 *  @param nBoxDOMask : The BoxDO to be modified is identified by bit. 
 *  @param nBoxDOVal : The target state of each BoxDO to be modified. 
 *  @param nBoxCOMask : The BoxCO to be modified is identified by bit 
 *  @param nBoxCOVal : The target state of each BoxCO to be modified 
 *  @param nBoxAOCH0_Mask : Indicator of whether BoxAOCH0 needs to be modified.
 *  @param nBoxAOCH0_Mode :  Mode 
 *  @param nBoxAOCH1_Mask :  Indicator of whether BoxAOCH1 needs to be modified.
 *  @param nBoxAOCH1_Mode : Mode 
 *  @param dbBoxAOCH0_Val :  Corresponding analog value 
 * @param dbBoxAOCH1_Val :  Corresponding analog value 
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_cdsSetIO(unsigned int boxID, unsigned int rbtID, int nEndDOMask, int nEndDOVal,int nBoxDOMask,int nBoxDOVal,int nBoxCOMask,int nBoxCOVal,int nBoxAOCH0_Mask,int nBoxAOCH0_Mode,int nBoxAOCH1_Mask,int nBoxAOCH1_Mode,double dbBoxAOCH0_Val,double dbBoxAOCH1_Val);

//////////////////////////////////////////////////////////////////////////////////////
/**
 *	@brief: Set Override 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dbOverride : Override value (0.01~1.00) 
 *
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetOverride(unsigned int boxID, unsigned int rbtID, double dOverride);

/**
 *	@brief: Read Override 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dbOverride : Override value  
 *
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadOverride(unsigned int boxID, unsigned int rbtID, double &dOverride);

/**
 *	@brief: Set Tool motion 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nState : 0 for closed, 1 for open 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetToolMotion(unsigned int boxID, unsigned int rbtID, int nState);

/**
 *	@brief: Read the maximum payload 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dbMaxPayload : maximum payload at the end (kg) 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadMaxPayload(unsigned int boxID, unsigned int rbtID, double& dbMaxPayload);

/**
 *	@brief: Set payload 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param Mass:Mass 
 *	@param dX:Gravity-CX 
 *	@param dY:Gravity-CY 
 *	@param dZ:Gravity-CZ 
 *	@param bUpdateDB:Optional parameters,default to 1 if not written
 *                    1:Modify payload,modify mass,write to database
 *                    3:Modify payload,not modify mass,write to database
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetPayload(unsigned int boxID, unsigned int rbtID, double dMass, double dX, double dY, double dZ, unsigned char byOption=1);

/**
 *	@brief: Read payload 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param Mass:Mass 
 *	@param dX:Gravity-CX 
 *	@param dY:Gravity-CY 
 *	@param dZ:Gravity-CZ 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadPayload(unsigned int boxID, unsigned int rbtID, double& dMass, double& dX, double& dY, double& dZ);

// Motion speed: mm/s or °/s ,Motion acceleration: mm/(s^2) or °/(s^2)
/**
 *	@brief: Set joint max velocity 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dJ1~6 : Joint1~6 max velocity [°/ s]
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetJointMaxVel(unsigned int boxID, unsigned int rbtID, double dJ1, double dJ2, double dJ3, double dJ4, double dJ5, double dJ6);

/**
 *	@brief: Set joint max acceleration 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dJ1~6 : Joint1~6 max acceleration [°/ s^2]
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetJointMaxAcc(unsigned int boxID, unsigned int rbtID, double dJ1, double dJ2, double dJ3, double dJ4, double dJ5, double dJ6);

/**
 *	@brief: Set linear max velocity 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dMaxVel : Linear max velocity 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetLinearMaxVel(unsigned int boxID, unsigned int rbtID, double dMaxVel);

/**
 *	@brief: Set linear max acceleration 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dMaxAcc : Linear max acceleration 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetLinearMaxAcc(unsigned int boxID, unsigned int rbtID, double dMaxAcc);

/**
 *	@brief: Set max range of joint motion 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dMaxJ1-dMaxJ6 : Max joint angle 
 *	@param dMinJ1-dMinJ6 : Min joint angle 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	
ROBOT_PRO_API int HRIF_SetMaxAcsRange(unsigned int boxID, unsigned int rbtID,
                        double dMaxJ1, double dMaxJ2, double dMaxJ3, double dMaxJ4, double dMaxJ5, double dMaxJ6,
                        double dMinJ1, double dMinJ2, double dMinJ3, double dMinJ4, double dMinJ5, double dMinJ6);

/**
 *	@brief: Set max range of linear motion 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dMaxX-dMaxZ : Max range of XYZ 
 *	@param dMinX-dMinZ : Min range of XYZ 
 *	@param dUcs_X~Rz : UCS pose 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	
ROBOT_PRO_API int HRIF_SetMaxPcsRange(unsigned int boxID, unsigned int rbtID,
                        double dMaxX, double dMaxY, double dMaxZ, double dMinX, double dMinY, double dMinZ,
                        double dUcs_X, double dUcs_Y, double dUcs_Z, double dUcs_Rx, double dUcs_Ry, double dUcs_Rz);

/**
 *	@brief: Set max range of linear motion 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nMode : Whether to enable the speed limit constraint on the change rate of orientation angles for MoveC/L commands.
 *	                           0 : Restricted (default) mode 
 *                             1 : Unrestricted mode 
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	
ROBOT_PRO_API int HRIF_SetRotationVelocityControlMode(unsigned int boxID, unsigned int rbtID,int nMode);

/**
 *	@brief: Read joint range and linear range 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dMaxJ1-dMaxJ6 : Max joint angle 
 *	@param dMinJ1-dMinJ6 : Min joint angle 
 *	@param dMaxX-dMaxRZ : Max range of XYZRXRYRZ 
 *	@param dMinX-dMinRZ : Min range of XYZRXRYRZ 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	
ROBOT_PRO_API int HRIF_ReadMaxRange(unsigned int boxID, unsigned int rbtID,vector<double> &MaxJointRange,vector<double> &MinJointRange,vector<double> &MaxLinerRange,vector<double> &MinLinerRange);

/**
 *	@brief: Read joint range and linear range 
 *	@param boxID : Control box ID, 0 as default
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dMaxJ1-dMaxJ6 : Max joint angle 
 *	@param dMinJ1-dMinJ6 : Min joint angle 
 *	@param dMaxX-dMaxRZ : Max range of XYZRXRYRZ 
 *	@param dMinX-dMinRZ : Min range of XYZRXRYRZ 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	
ROBOT_PRO_API int HRIF_ReadMaxRange_nJ(unsigned int boxID, unsigned int rbtID,vector<double> &MaxJointRange,vector<double> &MinJointRange,vector<double> &MaxLinerRange,vector<double> &MinLinerRange);

/**
 *	@brief: Set welding analog output voltage 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nState : Channel switch status. 0:off 1:open
 *	@param nIndex : Voltage analog channel: AO0 and AO1
 *	@param dInitAO : Initial voltage 
 *	@param dWeldingAO : Welding voltage. Unit: V
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetMoveParamsAO(unsigned int boxID, unsigned int rbtID,
                        int nState, int nIndex, double dInitAO, double dWeldingAO);

/**
 *	@brief: Read joint max velocity 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dJ1~6 : Max velocity of J1~J6 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadJointMaxVel(unsigned int boxID, unsigned int rbtID, double& dJ1, double& dJ2, double& dJ3, double& dJ4, double& dJ5, double& dJ6);

/**
 *	@brief: Read joint max acceleration 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dJ1~6 : Max acceleration of J1~J6 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadJointMaxAcc(unsigned int boxID, unsigned int rbtID, double& dJ1, double& dJ2, double& dJ3, double& dJ4, double& dJ5, double& dJ6);

/**
 *	@brief: Read joint max jerk 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dJ1~6 : Max jerk of J1~J6
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadJointMaxJerk(unsigned int boxID, unsigned int rbtID, double& dJ1, double& dJ2, double& dJ3, double& dJ4, double& dJ5, double& dJ6);

/**
 *	@brief:Read linear motion parameters 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dMaxVel : Linear motion velocity 
 *	@param dMaxAcc : Linear motion acceleration 
 *	@param dMaxJerk : Linear motion jerk 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadLinearMaxSpeed(unsigned int boxID, unsigned int rbtID, double& dMaxVel, double& dMaxAcc, double& dMaxJerk);

//////////////////////////////////////////////////////////////////////////////////////
/**
 *	@brief: Read Emergency stop information 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nESTO_Error: Error occurs when the two emergency signals are different 
 *	@param nESTO: Cut off 48V output to the robot when an emergency stop occurs 
 *	@param nSafeGuard_Error: Error occurs when the two safeguard signals are different 
 *	@param nSafeGuard:Stop the motion but never cutt off power supply when safeguard occurs 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadEmergencyInfo(unsigned int boxID, unsigned int rbtID,
                            int &nESTO_Error, 
                            int &nESTO, 
                            int &nSafeGuard_Error, 
                            int &nSafeGuard);

/**
 *	@brief: Enter or exit safeguard status by force to realize soft EStop(emergency stop). Move will be stopped. System can be reset after EStop is canceled. 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nFlag:1 to enter, 0 to cancel 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_EnterSafetyGuard(unsigned int boxID, unsigned int rbtID, int nFlag);

/**
 *	@brief: Enter or exit safeguard status by force to realize soft EStop(emergency stop). Move will be stopped. System can be reset after EStop is canceled. 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nFlag:1 to enter, 0 to cancel 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_EnterSafeGuard(unsigned int boxID, unsigned int rbtID, int nFlag);

/**
 *	@brief: Read robot state 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nMovingState : Moving state 
 *	@param nEnableState : Enable state 
 *	@param nErrorState : Error state 
 *	@param nErrorCode : Error code 
 *	@param nErrorAxis : Error axis ID 
 *	@param nBreaking : Breaking status 
 *	@param nPause : Pause state 
 *	@param nEmergencyStop : Emergency stop state 
 *	@param nSafeGuard : Safty guard state 
 *	@param nElectrify : Electrify state 
 *	@param nIsConnectToBox : Connection of control box state 
 *	@param nBlendingDone : Moving blending done state 
 *	@param nInPos : In actual pose state 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadRobotState(unsigned int boxID, unsigned int rbtID,
						int &nMovingState, 
						int &nEnableState, 
						int &nErrorState, 
						int &nErrorCode, 
						int &nErrorAxis,
						int &nBreaking, 
						int &nPause,
						int &nEmergencyStop, 
						int &nSafeGuard, 
						int &nElectrify, 
						int &nIsConnectToBox,
						int &nBlendingDone, 
						int &nInpos);

/**
 *	@brief: Read robot state 
 *	@param boxID : control box number, 0 as default 
 *	@param rbtID : robot ID, 0 as default 
 *	@param nMovingState : Moving state 
 *	@param nEnableState : Enable state 
 *	@param nErrorState : Error state 
 *	@param nErrorCode : Error code 
 *	@param nErrorAxis : Error axis ID 
 *	@param nBreaking : Breaking status 
 *	@param nPause : Pause state 
 *	@param nBlendingDone : Moving blending done state 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadRobotFlags(unsigned int boxID, unsigned int rbtID,
						int &nMovingState, 
						int &nEnableState, 
						int &nErrorState, 
						int &nErrorCode, 
						int &nErrorAxis,
						int &nBreaking, 
						int &nPause,
						int &nBlendingDone);
/**
 *	@brief: Read current waypoint ID 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param strCurWaypointID : Current ID 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadCurWaypointID(unsigned int boxID, unsigned int rbtID, string& strCurWaypointID);

/**
 *	@brief: Read axis error code 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nErrorCode : Error code 
 *	@param Params : The error code of J1~J6, 0 if no error 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadAxisErrorCode(unsigned int boxID, unsigned int rbtID, int& nErrorCode, int& nJ1, int& nJ2, int& nJ3, int& nJ4, int& nJ5, int& nJ6);

/**
 *	@brief: Read current FSM 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nCurFSM : Current FSM 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadCurFSM(unsigned int boxID, unsigned int rbtID, int& nCurFSM, string& strCurFSM);

/**
 *	@brief: Read FSM from CPS 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nCurFSM : Current FSM
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadCurFSMFromCPS(unsigned int boxID, unsigned int rbtID, int& nCurFSM);

/**
 *	@brief: Read actual pose and joint positions 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dX~Rz : pose 
 *	@param dJ1~6 : joint positions 
 *	@param dTcp_X~Rz : TCP pose 
 *	@param dUcs_X~Rz : UCS pose 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadActPos(unsigned int boxID, unsigned int rbtID,
					 double &dX, double &dY, double &dZ, double &dRx, double &dRy, double &dRz,
					 double &dJ1, double &dJ2, double &dJ3, double &dJ4, double &dJ5, double &dJ6,
					 double &dTcp_X, double &dTcp_Y, double &dTcp_Z, double &dTcp_Rx, double &dTcp_Ry, double &dTcp_Rz,
                    double &dUcs_X, double &dUcs_Y, double &dUcs_Z, double &dUcs_Rx, double &dUcs_Ry, double &dUcs_Rz);

/**
 *	@brief: Read point pose, joint positions, UCS and TCP by point name 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dJ1~6 : joint positions 
 *	@param dX~Rz : pose 
 *	@param dTcp_X~Rz : TCP pose
 *	@param dUcs_X~Rz : UCS pose 
 *
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadPointByName(unsigned int boxID, unsigned int rbtID, string strPointName,
    double& dJ1, double& dJ2, double& dJ3, double& dJ4, double& dJ5, double& dJ6,
    double& dX, double& dY, double& dZ, double& dRx, double& dRy, double& dRz,
    double& dTcp_X, double& dTcp_Y, double& dTcp_Z, double& dTcp_Rx, double& dTcp_Ry, double& dTcp_Rz,
    double& dUcs_X, double& dUcs_Y, double& dUcs_Z, double& dUcs_Rx, double& dUcs_Ry, double& dUcs_Rz);

/**
 *	@brief: Read point list as name 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param PointList : point list 
 *
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadPointList(unsigned int boxID, unsigned int rbtID, vector<string> &PointList);

/**
 *	@brief: Read command joint positions 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dJ1~6 : Joint command positions 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadCmdJointPos(unsigned int boxID, unsigned int rbtID, double& dJ1, double& dJ2, double& dJ3, double& dJ4, double& dJ5, double& dJ6);

/**
 *	@brief: Read actual joint positions 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dJ1~6 : Joint actual positions 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadActJointPos(unsigned int boxID, unsigned int rbtID, double& dJ1, double& dJ2, double& dJ3, double& dJ4, double& dJ5, double& dJ6);

/**
 *	@brief: Read command pose of TCP 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dX~Rz : TCP command pose 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadCmdTcpPos(unsigned int boxID, unsigned int rbtID, double& dX, double& dY, double& dZ, double& dRx, double& dRy, double& dRz);

/**
 *	@brief: Read actual pose of TCP 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dX~Rz : TCP actual pose 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadActTcpPos(unsigned int boxID, unsigned int rbtID, double& dX, double& dY, double& dZ, double& dRx, double& dRy, double& dRz);

/**
 *	@brief: Read command velocity of joint 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dJ1~6 : Joint command velocity 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadCmdJointVel(unsigned int boxID, unsigned int rbtID, double& dJ1, double& dJ2, double& dJ3, double& dJ4, double& dJ5, double& dJ6);

/**
 *	@brief: Read actual velocity of joint 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dJ1~6 : Joint actual velocity 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadActJointVel(unsigned int boxID, unsigned int rbtID, double& dJ1, double& dJ2, double& dJ3, double& dJ4, double& dJ5, double& dJ6);

/**
 *	@brief: Read command velocity of TCP 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dX~Rz : TCP command velocity 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadCmdTcpVel(unsigned int boxID, unsigned int rbtID, double& dX, double& dY, double& dZ, double& dRx, double& dRy, double& dRz);

/**
 *	@brief: Read actual velocity of TCP 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dX~Rz : TCP actual velocity 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadActTcpVel(unsigned int boxID, unsigned int rbtID, double& dX, double& dY, double& dZ, double& dRx, double& dRy, double& dRz);

/**
 *	@brief: Read command current of joint 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dJ1~6 : Joint command current 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadCmdJointCur(unsigned int boxID, unsigned int rbtID, double& dJ1, double& dJ2, double& dJ3, double& dJ4, double& dJ5, double& dJ6);

/**
 *	@brief: Read actual current of joint 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dJ1~6 : Joint actual current 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadActJointCur(unsigned int boxID, unsigned int rbtID, double& dJ1, double& dJ2, double& dJ3, double& dJ4, double& dJ5, double& dJ6);

/**
 *	@brief: Read End velocity of TCP 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dCmdVel : TCP command velocity mm/s
 *	@param dActVel : TCP actual velocity mm/s
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadTcpVelocity(unsigned int boxID, unsigned int rbtID, double& dCmdVel, double& dActVel);

/**
 *	@brief: Set reduce mode
 *	@param boxID : Control box ID, 0 as default
 *	@param rbtID : Robot ID, 0 as default
 *	@param nReduceMode : Reduce mode, 0 to exit reduce mode, 1 to entry reduce mode
 *	
 *	@return : nRet=0 : Function call succeeded
 *            nRet>0 : Error code of function call
 */
ROBOT_PRO_API int HRIF_SetReduceMode(unsigned int boxID, unsigned int rbtID, int nReduceMode);

/**
 *	@brief: Read reduce mode
 *	@param boxID : Control box ID, 0 as default
 *	@param rbtID : Robot ID, 0 as default
 *	@param nReduceMode : Current reduce mode, 0 for not reduce, 1 for reduce mode
 *	
 *	@return : nRet=0 : Function call succeeded
 *            nRet>0 : Error code of function cal
 */
ROBOT_PRO_API int HRIF_ReadReduceMode(unsigned int boxID, unsigned int rbtID, int& nReduceMode);


//************************************************************************/
//**    Interfaces for calculation 
//************************************************************************/
/**
 *	@brief: Quaternion to Euler angle (°)
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dQuaW : Quaternion W 
 *	@param dQuaX : Quaternion X 
 *	@param dQuaY : Quaternion Y 
 *	@param dQuaZ : Quaternion Z 
 *	@param dRx: Euler angle RX 
 *	@param rRy: Euler angle RY 
 *	@param dRz: Euler angle RZ 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_Quaternion2RPY(unsigned int boxID, unsigned int rbtID, double dQuaW, double dQuaX, double dQuaY, double dQuaZ, double& dRx, double& dRy, double& dRz);

/**
 *	@brief: Euler angle to Quaternion (°)
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dRx: Euler angle RX 
 *	@param dRy: Euler angle RY 
 *	@param dRz: Euler angle RZ 
 *	@param dQuaW : Quaternion W 
 *	@param dQuaX : Quaternion X 
 *	@param dQuaY : Quaternion Y 
 *	@param dQuaZ : Quaternion Z 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_RPY2Quaternion(unsigned int boxID, unsigned int rbtID, double dRx, double dRy, double dRz, double& dQuaW, double& dQuaX, double& dQuaY, double& dQuaZ);
/**
 *	@brief: Inverse kinematics transformation from pose to joint positions 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dCoord_X~Rz : pose in specified UCS and TCP 
 *	@param dTcp_X~Rz : TCP pose 
 *	@param dUcs_X~Rz : UCS pose 
 *	@param dJ1~6 : Reference joint positions used for the selection in multiple solutions 
 *	@param dTargetJ1~6 : Inverse solution of J1~J6 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_GetInverseKin(unsigned int boxID, unsigned int rbtID, double dCoord_X, double dCoord_Y, double dCoord_Z, double dCoord_Rx, double dCoord_Ry, double dCoord_Rz,
                 double dTcp_X, double dTcp_Y, double dTcp_Z, double dTcp_Rx, double dTcp_Ry, double dTcp_Rz,
                 double dUcs_X, double dUcs_Y, double dUcs_Z, double dUcs_Rx, double dUcs_Ry, double dUcs_Rz,
                 double dJ1, double dJ2, double dJ3, double dJ4, double dJ5, double dJ6,
                 double &dTargetJ1, double &dTargetJ2, double &dTargetJ3, double &dTargetJ4, double &dTargetJ5, double &dTargetJ6);

/**
 *	@brief: Forward kinematics transformation from joint positions to pose 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dJ1~6 : joint positions 
 *	@param dTcp_X~Rz : TCP pose for the target 
 *	@param dUcs_X~Rz : UCS pose for the target 
 *	@param dTarget_X~Rz : pose in specified UCS and TCP 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */				 
ROBOT_PRO_API int HRIF_GetForwardKin(unsigned int boxID, unsigned int rbtID, double dJ1, double dJ2, double dJ3, double dJ4, double dJ5, double dJ6,
                 double dTcp_X, double dTcp_Y, double dTcp_Z, double dTcp_Rx, double dTcp_Ry, double dTcp_Rz,
                 double dUcs_X, double dUcs_Y, double dUcs_Z, double dUcs_Rx, double dUcs_Ry, double dUcs_Rz,
                 double &dTarget_X, double &dTarget_Y, double &dTarget_Z, double &dTarget_Rx, double &dTarget_Ry, double &dTarget_Rz);

/***
 *	@brief: Calculate inverse dynamics results 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dPose_J1~6 : Joint position (°)
 *	@param dVel_J1~6 : Joint velocities (°/s)
 *	@param dAcc_J1~6 : Joint accelerations (°/s²)
 *	@param dTorq_J1~6 : Joint torques calculated by inverse dynamics (Nm)
 *
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_GetInverseDynamics(unsigned int boxID, unsigned int rbtID, double dPose_J1, double dPose_J2, double dPose_J3, double dPose_J4, double dPose_J5, double dPose_J6,
                    double dVel_J1,double dVel_J2,double dVel_J3,double dVel_J4,double dVel_J5,double dVel_J6,
                    double dAcc_J1,double dAcc_J2,double dAcc_J3,double dAcc_J4,double dAcc_J5,double dAcc_J6,
                    double& dTorq_J1,double& dTorq_J2,double& dTorq_J3,double& dTorq_J4,double& dTorq_J5,double& dTorq_J6);


/**
 *	@brief: Calculate the robot's configuration from joint angles
 *	@param boxID : Control box ID, 0 as default
 *	@param rbtID : Robot ID, 0 as default
 *	@param dJ1~6 : joint positions,(unit: degrees)
 *	@param nCf1, nCf4, nCf6: Joint configurations for axes 1, 4, and 6, range: -4 ~ 3 
 *	@param nCfx: Joint configuration for axis 5, range: 0 ~ 7
 *	
 *	@return : nRet=0 : Function call succeeded
 *            nRet>0 : Error code of function call
 */
ROBOT_PRO_API int HRIF_ACS2PoseConf(unsigned int boxID, unsigned int rbtID, double dJ1, double dJ2, double dJ3, double dJ4, double dJ5, double dJ6,
                 int& nCf1, int& nCf4, int& nCf6, int& nCfx);

/**
 *	@brief: Inverse kinematics transformation from pose to joint positions
 *	@param boxID : Control box ID, 0 as default
 *	@param rbtID : Robot ID, 0 as default
 *	@param dCoord_X~Rz : pose in specified UCS and TCP
 *	@param dTcp_X~Rz : TCP pose
 *	@param dUcs_X~Rz : UCS pose
 *	@param nCf1, nCf4, nCf6: Joint configurations for axes 1, 4, and 6, range: -4 ~ 3
 *	@param nCfx: Joint configuration for axis 5, range: 0 ~ 7
 *	@param dTargetJ1~6 : Inverse solution of J1~J6
 *	
 *	@return : nRet=0 : Function call succeeded
 *            nRet>0 : Error code of function call
 */
ROBOT_PRO_API int HRIF_GetInverseKin2(unsigned int boxID, unsigned int rbtID, double dCoord_X, double dCoord_Y, double dCoord_Z, double dCoord_Rx, double dCoord_Ry, double dCoord_Rz,
                 double dTcp_X, double dTcp_Y, double dTcp_Z, double dTcp_Rx, double dTcp_Ry, double dTcp_Rz,
                 double dUcs_X, double dUcs_Y, double dUcs_Z, double dUcs_Rx, double dUcs_Ry, double dUcs_Rz,
                 int nCf1, int nCf4, int nCf6, int nCfx,
                 double &dTargetJ1, double &dTargetJ2, double &dTargetJ3, double &dTargetJ4, double &dTargetJ5, double &dTargetJ6);


/**
 *	@brief: Pose transformation from Base to UCS&TCP 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dCoord_X~Rz : pose in Base coordinate system 
 *	@param dTcp_X~Rz : TCP pose for the target 
 *	@param dUcs_X~Rz : UCS pose for the target 
 *	@param dTarget_X~Rz : pose in the specified UCS and TCP 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */				
ROBOT_PRO_API int HRIF_Base2UcsTcp(unsigned int boxID, unsigned int rbtID, double dCoord_X, double dCoord_Y, double dCoord_Z, double dCoord_Rx, double dCoord_Ry, double dCoord_Rz,
                     double dTcp_X, double dTcp_Y, double dTcp_Z, double dTcp_Rx, double dTcp_Ry, double dTcp_Rz,
                     double dUcs_X, double dUcs_Y, double dUcs_Z, double dUcs_Rx, double dUcs_Ry, double dUcs_Rz,
                     double &dTarget_X, double &dTarget_Y, double &dTarget_Z, double &dTarget_Rx, double &dTarget_Ry, double &dTarget_Rz);
					 
/**
 *	@brief: Pose transformation from UCS&TCP to Base 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dCoord_X~Rz : pose in speicified UCS and TCP 
 *	@param dTcp_X~Rz : TCP pose for dCoord 
 *	@param dUcs_X~Rz : UCS pose for dCoord 
 *	@param dTarget_X~Rz : pose in Base coordinate system 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */		
ROBOT_PRO_API int HRIF_UcsTcp2Base(unsigned int boxID, unsigned int rbtID, double dCoord_X, double dCoord_Y, double dCoord_Z, double dCoord_Rx, double dCoord_Ry, double dCoord_Rz,
                     double dTcp_X, double dTcp_Y, double dTcp_Z, double dTcp_Rx, double dTcp_Ry, double dTcp_Rz,
                     double dUcs_X, double dUcs_Y, double dUcs_Z, double dUcs_Rx, double dUcs_Ry, double dUcs_Rz,
                     double &dTarget_X, double &dTarget_Y, double &dTarget_Z, double &dTarget_Rx, double &dTarget_Ry, double &dTarget_Rz);

/**
 *	@brief: Pose addition calculation 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dPose1_X~Rz : Pose of point 1 
 *	@param dPose2_X~Rz : Pose of point 2 
 *	@param dPose3_X~Rz : Calculation result 
 *
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	
ROBOT_PRO_API int HRIF_PoseAdd(unsigned int boxID, unsigned int rbtID, double dPose1_X, double dPose1_Y, double dPose1_Z, double dPose1_Rx, double dPose1_Ry, double dPose1_Rz,
                    double dPose2_X,double dPose2_Y,double dPose2_Z,double dPose2_Rx,double dPose2_Ry,double dPose2_Rz,
                    double &dPose3_X,double &dPose3_Y,double &dPose3_Z,double &dPose3_Rx,double &dPose3_Ry,double &dPose3_Rz);

/**
 *	@brief: Pose subtraction calculation 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dPose1_X~Rz : Pose of point 1 
 *	@param dPose2_X~Rz : Pose of point 2 
 *	@param dPose3_X~Rz : Calculation result 
 *
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	                  
ROBOT_PRO_API int HRIF_PoseSub(unsigned int boxID, unsigned int rbtID, double dPose1_X, double dPose1_Y, double dPose1_Z, double dPose1_Rx, double dPose1_Ry, double dPose1_Rz,
                    double dPose2_X,double dPose2_Y,double dPose2_Z,double dPose2_Rx,double dPose2_Ry,double dPose2_Rz,
                    double &dPose3_X,double &dPose3_Y,double &dPose3_Z,double &dPose3_Rx,double &dPose3_Ry,double &dPose3_Rz);

/**
 *	@brief: Pose transformation through combined caculations, from P1 based on Base coordinate system to P3 based on UCS with P2 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dPose1_X~Rz : Pose of P1 
 *	@param dPose2_X~Rz : Pose of P2 
 *	@param dPose3_X~Rz : Calculation result, P3 
 *
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	                       
ROBOT_PRO_API int HRIF_PoseTrans(unsigned int boxID, unsigned int rbtID, double dPose1_X, double dPose1_Y, double dPose1_Z, double dPose1_Rx, double dPose1_Ry, double dPose1_Rz,
                    double dPose2_X,double dPose2_Y,double dPose2_Z,double dPose2_Rx,double dPose2_Ry,double dPose2_Rz,
                    double &dPose3_X,double &dPose3_Y,double &dPose3_Z,double &dPose3_Rx,double &dPose3_Ry,double &dPose3_Rz);

/**
 *	@brief: Inverse kinematics transformation for pose 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dPoseFrom_X~Rz : Pose of oringinal point 
 *	@param dPoseTo_X~Rz : Pose of calculation result 
 *
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	                       
ROBOT_PRO_API int HRIF_PoseInverse(unsigned int boxID, unsigned int rbtID, double dPoseFrom_X, double dPoseFrom_Y, double dPoseFrom_Z, double dPoseFrom_Rx, double dPoseFrom_Ry, double dPoseFrom_Rz,
                    double &dPoseTo_X,double &dPoseTo_Y,double &dPoseTo_Z,double &dPoseTo_Rx,double &dPoseTo_Ry,double &dPoseTo_Rz);

/**
 *	@brief:  Calculate points distance 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dPose1_X~Rz : Pose of point 1 
 *	@param dPose2_X~Rz : Pose of point 2 
 *	@param dDistance : Points distance (mm) 
 *	@param dAngle : Angle between poses (°)
 *
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	                       
ROBOT_PRO_API int HRIF_PoseDist(unsigned int boxID, unsigned int rbtID, double dPose1_X, double dPose1_Y, double dPose1_Z, double dPose1_Rx, double dPose1_Ry, double dPose1_Rz,
                    double dPose2_X,double dPose2_Y,double dPose2_Z,double dPose2_Rx,double dPose2_Ry,double dPose2_Rz,
                    double &dDistance,double &dAngle);


/**
 *	@brief: Linear interpolation calculation for pose 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dPose1_X~Rz : Pose of point 1 
 *	@param dPose2_X~Rz : Pose of point 2 
 *	@param dAlpha : Interpolation scale 
 *	@param dPose3_X~Rz : Calculation result, pose of point 3 
 *
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	                      
ROBOT_PRO_API int HRIF_PoseInterpolate(unsigned int boxID, unsigned int rbtID, double dPose1_X, double dPose1_Y, double dPose1_Z, double dPose1_Rx, double dPose1_Ry, double dPose1_Rz,
                    double dPose2_X,double dPose2_Y,double dPose2_Z,double dPose2_Rx,double dPose2_Ry,double dPose2_Rz,double dAlpha,
                    double &dPose3_X,double &dPose3_Y,double &dPose3_Z,double &dPose3_Rx,double &dPose3_Ry,double &dPose3_Rz);

/***
 *	@brief: Calculate the rotation center. P1,P2,P3 are the points before rotation and P4,P5,P6 are the points after rotation 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dPose1_X~Z : Pose of point 1 
 *	@param dPose2_X~Z : Pose of point 2 
 *	@param dPose3_X~Z : Pose of point 3 
 *	@param dPose4_X~Z : Pose of point 4 
 *	@param dPose5_X~Z : Pose of point 5 
 *	@param dPose6_X~Z : Pose of point 6 
 *	@param dUcs_X~Rz : Calculation result, UCS 
 *
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	  
ROBOT_PRO_API int HRIF_PoseDefdFrame(unsigned int boxID, unsigned int rbtID, double dPose1_X, double dPose1_Y, double dPose1_Z, double dPose2_X, double dPose2_Y, double dPose2_Z,
                    double dPose3_X,double dPose3_Y,double dPose3_Z,double dPose4_X,double dPose4_Y,double dPose4_Z,
                    double dPose5_X,double dPose5_Y,double dPose5_Z,double dPose6_X,double dPose6_Y,double dPose6_Z,
                    double &dUcs_X,double &dUcs_Y,double &dUcs_Z,double &dUcs_Rx,double &dUcs_Ry,double &dUcs_Rz);

/***
 *	@brief: Calculate the two passing through intermediate points in arc using three-point teaching 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dPose1_X~Rz : Pose of start point 
 *	@param dPose2_X~Rz : Pose of aux point 
 *	@param dPose3_X~Rz : Pose of end point 
 *  @param dRetPose1_X~Rz  : Pose of point between start point and aux point 
 *	@param dRetPose2_X~Rz  : Pose of point between start point and aux point 
 *
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	  
ROBOT_PRO_API int HRIF_GetCircularViaPose(unsigned int boxID, unsigned int rbtID, 
                    double dPose1_X, double dPose1_Y, double dPose1_Z, double dPose1_Rx, double dPose1_Ry, double dPose1_Rz,
                    double dPose2_X,double dPose2_Y,double dPose2_Z,double dPose2_Rx,double dPose2_Ry,double dPose2_Rz,
                    double dPose3_X,double dPose3_Y,double dPose3_Z,double dPose3_Rx,double dPose3_Ry,double dPose3_Rz,
                    double& dRetPose1_X,double& dRetPose1_Y,double& dRetPose1_Z,double& dRetPose1_Rx,double& dRetPose1_Ry,double& dRetPose1_Rz,
                    double& dRetPose2_X,double& dRetPose2_Y,double& dRetPose2_Z,double& dRetPose2_Rx,double& dRetPose2_Ry,double& dRetPose2_Rz);

/***
 *	@brief: Calculate UCS through 3-point plane 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dPose1_X~Z : Position of point 1 in Base and specified TCP 
 *	@param dPose2_X~Z : Position of point 2 in Base and specified TCP 
 *	@param dPose3_X~Z : Position of point 3 in Base and specified TCP 
 *  @param dRetPose_X~Rz  : Pose of UCS 
 *
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	  
ROBOT_PRO_API int HRIF_CalUcsPlane(unsigned int boxID, unsigned int rbtID, 
                    double dPose1_X, double dPose1_Y, double dPose1_Z,
                    double dPose2_X,double dPose2_Y,double dPose2_Z,
                    double dPose3_X,double dPose3_Y,double dPose3_Z,
                    double& dRetPose_X,double& dRetPose_Y,double& dRetPose_Z,double& dRetPose_Rx,double& dRetPose_Ry,double& dRetPose_Rz);

/***
 *	@brief: Calculate UCS through 2-point linee 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dPose1_X~Rz : Pose of point 1 in Base and specified TCP 
 *	@param dPose2_X~Rz : Pose of point 2 in Base and specified TCP 
 *  @param dRetPose_X~Rz  : Pose of UCS 
 *
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	  
ROBOT_PRO_API int HRIF_CalUcsLine(unsigned int boxID, unsigned int rbtID, 
                    double dPose1_X, double dPose1_Y, double dPose1_Z, double dPose1_Rx, double dPose1_Ry, double dPose1_Rz,
                    double dPose2_X,double dPose2_Y,double dPose2_Z,double dPose2_Rx,double dPose2_Ry,double dPose2_Rz,
                    double& dRetPose_X,double& dRetPose_Y,double& dRetPose_Z,double& dRetPose_Rx,double& dRetPose_Ry,double& dRetPose_Rz);

/***
 *	@brief: Calculate TCP through 3-point 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dPose1_X~Rx : Pose of point 1 in Base and system default TCP 
 *	@param dPose2_X~Rz : Pose of point 2 in Base and system default TCP 
 *	@param dPose3_X~Rz : Pose of point 3 in Base and system default TCP 
 *  @param dRetPose_X~Z  : Position of TCP 
 *  @param dRetPose_Rx~Rz  : Orientation of TCP, usually 0 
 *  @param quality : quality of the result, 0 for good, 1 for poor, 2 for abnormal 
 *
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	  
ROBOT_PRO_API int HRIF_CalTcp3P(unsigned int boxID, unsigned int rbtID, 
                    double dPose1_X, double dPose1_Y, double dPose1_Z, double dPose1_Rx, double dPose1_Ry, double dPose1_Rz,
                    double dPose2_X,double dPose2_Y,double dPose2_Z,double dPose2_Rx,double dPose2_Ry,double dPose2_Rz,
                    double dPose3_X,double dPose3_Y,double dPose3_Z,double dPose3_Rx,double dPose3_Ry,double dPose3_Rz,
                    double& dRetPose_X,double& dRetPose_Y,double& dRetPose_Z,double& dRetPose_Rx,double& dRetPose_Ry,double& dRetPose_Rz,
                    int& quality);

 /***
 *	@brief: Calculate TCP through 4-point 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dPose1_X~Rx : Pose of point 1 in Base and system default TCP 
 *	@param dPose2_X~Rz : Pose of point 2 in Base and system default TCP 
 *	@param dPose3_X~Rz : Pose of point 3 in Base and system default TCP 
 *	@param dPose4_X~Rz : Pose of point 4 in Base and system default TCP 
 *  @param dRetPose_X~Z  : Position of TCP 
 *  @param dRetPose_Rx~Rz  : Orientation of TCP, usually 0 
 *  @param quality : quality of the result, 0 for good, 1 for poor, 2 for abnormal 
 *  @param erroIndex_P1~4 : error index for the 4 source points, 0 for abnormal, 1 for normal 
 *
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	  
ROBOT_PRO_API int HRIF_CalTcp4P(unsigned int boxID, unsigned int rbtID, 
                    double dPose1_X, double dPose1_Y, double dPose1_Z, double dPose1_Rx, double dPose1_Ry, double dPose1_Rz,
                    double dPose2_X,double dPose2_Y,double dPose2_Z,double dPose2_Rx,double dPose2_Ry,double dPose2_Rz,
                    double dPose3_X,double dPose3_Y,double dPose3_Z,double dPose3_Rx,double dPose3_Ry,double dPose3_Rz,
                    double dPose4_X,double dPose4_Y,double dPose4_Z,double dPose4_Rx,double dPose4_Ry,double dPose4_Rz,
                    double& dRetPose_X,double& dRetPose_Y,double& dRetPose_Z,double& dRetPose_Rx,double& dRetPose_Ry,double& dRetPose_Rz,
                    int& quality, int& erroIndex_P1,int& erroIndex_P2,int& erroIndex_P3,int& erroIndex_P4);

//////////////////////////////////////////////////////////////////////////////////////
//************************************************************************/
//**    Interfaces for TCP and UCS
//************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////
/**
 *	@brief:Set current TCP 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dTcp_X~Rz : Pose of TCP 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	
ROBOT_PRO_API int HRIF_SetTCP(unsigned int boxID, unsigned int rbtID, double dTcp_X, double dTcp_Y, double dTcp_Z, double dTcp_Rx, double dTcp_Ry, double dTcp_Rz);

/**
 *	@brief: Set current UCS 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dUcs_X~Rz : Pose of UCS 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	
ROBOT_PRO_API int HRIF_SetUCS(unsigned int boxID, unsigned int rbtID, double dUcs_X, double dUcs_Y, double dUcs_Z, double dUcs_Rx, double dUcs_Ry, double dUcs_Rz);

/**
 *	@brief: Read current TCP 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dTcp_X~Rz : Pose of TCP 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadCurTCP(unsigned int boxID, unsigned int rbtID, double& dTcp_X, double& dTcp_Y, double& dTcp_Z, double& dTcp_Rx, double& dTcp_Ry, double& dTcp_Rz);

/**
 *	@brief: Read TCP list 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param TCPList : vector of TCP list 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadTCPList(unsigned int boxID, unsigned int rbtID, vector<string> &TCPList);

/**
 *	@brief: Read current UCS 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID :  Robot ID, 0 as default 
 *	@param dUcs_X~Rz : Pose of UCS 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadCurUCS(unsigned int boxID, unsigned int rbtID, double& dUcs_X, double& dUcs_Y, double& dUcs_Z, double& dUcs_Rx, double& dUcs_Ry, double& dUcs_Rz);

/**
 *	@brief: Read UCS list 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param TCPList : vector of UCS list 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadUCSList(unsigned int boxID, unsigned int rbtID, vector<string> &UCSList);

/**
 *	@brief: Set current TCP By Name 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param sTcpName : TCP name
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetTCPByName(unsigned int boxID, unsigned int rbtID, string sTcpName);

/**
 *	@brief: Set current UCS By Name 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param sUcsName : UCS name 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetUCSByName(unsigned int boxID, unsigned int rbtID, string sUcsName);

/**
 *	@brief: Read TCP By Name 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param sName : TCP name 
 *	@param dTcp_X~Rz : Pose of TCP 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	
ROBOT_PRO_API int HRIF_ReadTCPByName(unsigned int boxID, unsigned int rbtID,
                        string sName, 
                        double& dTcp_X, 
                        double& dTcp_Y, 
                        double& dTcp_Z, 
                        double& dTcp_Rx, 
                        double& dTcp_Ry, 
                        double& dTcp_Rz);

/**
 *	@brief: Read UCS By Name 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param sName : UCS name 
 *	@param dUcs_X~Rz: Pose of UCS 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	
ROBOT_PRO_API int HRIF_ReadUCSByName(unsigned int boxID, unsigned int rbtID,
                        string sName, 
                        double& dUcs_X, 
                        double& dUcs_Y, 
                        double& dUcs_Z, 
                        double& dUcs_Rx, 
                        double& dUcs_Ry, 
                        double& dUcs_Rz);

/**
 *	@brief: Configure TCP by Name 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param sTcpName : TCP name 
 *	@param dX~Rz : Pose of TCP 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
 //ROBOT_PRO_API int HRIF_ConfigTCP(unsigned int boxID, string sTcpName,double dX, double dY, double dZ, double dRx, double dRy, double dRz);


 
//////////////////////////////////////////////////////////////////////////////////////
//************************************************************************/
//**    Interfaces for force control 
//************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////
ROBOT_PRO_API int HRIF_GetForceParams(unsigned int boxID, unsigned int rbtID, double* dInertia, double* dDamping, double* dStiffness);

/**
 *	@brief: Set force parameters 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dInertia : Mass (size=6)
 *	@param dDamping : Damp (size=6)
 *	@param dStiffness : Stiffness (size=6)
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetForceParams(unsigned int boxID, unsigned int rbtID, double* dInertia, double* dDamping, double* dStiffness);

/**
 *	@brief: Set force control parameters 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dForceLimit : Force limit (size=6)
 *	@param dDistLimit : Distance limit (size=6)
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetForceControlParams(unsigned int boxID, unsigned int rbtID, double* dForceLimit, double* dDistLimit);

/**
 *	@brief: Set force control switch 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dEnbaleFlag : true for open, false for closed 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetForceMode(unsigned int boxID, unsigned int rbtID, bool dEnbaleFlag);

/**
 *	@brief: Set force control status 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nState : 0 for closed, 1 for open 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetForceControlState(unsigned int boxID, unsigned int rbtID, int nState);
/**
 *	@brief: Read force control status 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nState : 0 for closed, 1 for seeking, 2 for seeking completed, 3 for free drive 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadForceControlState(unsigned int boxID, unsigned int rbtID, int& nState);
/**
 *	@brief: Set force sensor direction align with TCP 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nState : 0 for not align with TCP, 1 for align with TCP 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetForceToolCoordinateMotion(unsigned int boxID, unsigned int rbtID, int nMode);
/**
 *	@brief: Disable force control 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ForceControlInterrupt(unsigned int boxID, unsigned int rbtID);

/**
 *	@brief: Enable force control 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ForceControlContinue(unsigned int boxID, unsigned int rbtID);
/**
 *	@brief: Reset force sensor to zero 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetForceZero(unsigned int boxID, unsigned int rbtID);
/**
 *	@brief: Set max velocity of force seeking 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dMaxLinearVelocity : Max linear velocity 
 *	@param dMaxAngularVelocity : Max angular velocity 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetMaxSearchVelocities(unsigned int boxID, unsigned int rbtID, double dMaxLinearVelocity, double dMaxAngularVelocity);

/**
 *	@brief: Set control freedom 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nX~Rz : 0 for closed / 1 for open 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetControlFreedom(unsigned int boxID, unsigned int rbtID, int nX, int nY, int nZ, int nRx, int nRy, int nRz);

/**
 *	@brief: Set force control strategy 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nState : 0 for Compliant force and 1 for Constant force 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetForceControlStrategy(unsigned int boxID, unsigned int rbtID, int nState);
/**
 *	@brief: Set force sensor pose 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dX~Rz : Pose 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetFreeDrivePositionAndOrientation(unsigned int boxID, unsigned int rbtID, double dX, double dY, double dZ, double dRx, double dRy, double dRz);
/**
 *	@brief: Set PID for force seeking 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dFp~Td : PID
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetPIDControlParams(unsigned int boxID, unsigned int rbtID, double dFp, double dFi, double dFd, double dTp, double dTi, double dTd);
/**
 *	@brief: Set mass parameters 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dX~Rz : Mass parameters 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetMassParams(unsigned int boxID, unsigned int rbtID, double dX, double dY, double dZ, double dRx, double dRy, double dRz);
/**
 *	@brief: Set damp parameters 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dX~Rz : Damp parameters 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetDampParams(unsigned int boxID, unsigned int rbtID, double dX, double dY, double dZ, double dRx, double dRy, double dRz);
/**
 *	@brief: Set stiffness parameters 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dX~Rz : Stiffness parameters 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetStiffParams(unsigned int boxID, unsigned int rbtID, double dX, double dY, double dZ, double dRx, double dRy, double dRz);
/**
 *	@brief: Set force control goal 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dX~Rz: Goal values 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetForceControlGoal(unsigned int boxID, unsigned int rbtID, double dX, double dY, double dZ, double dRX, double dRY, double dRZ);
/**
 *	@brief: Set control seeking goal 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dWrench_X~Rz : Seeking force 
 *	@param dDistance_X~Rz : Seeking distance 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	
ROBOT_PRO_API int HRIF_SetControlGoal(unsigned int boxID, unsigned int rbtID, double dWrench_X, double dWrench_Y, double dWrench_Z, double dWrench_Rx, double dWrench_Ry, double dWrench_Rz,
                        double dDistance_X,double dDistance_Y, double dDistance_Z,double dDistance_Rx,double dDistance_Ry, double dDistance_Rz);

/**
 *	@brief: Set force data limit 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dMax_X~Rz) : Max limit 
 *	@param dMin_X~Rz) : Min limit 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	
ROBOT_PRO_API int HRIF_SetForceDataLimit(unsigned int boxID, unsigned int rbtID, double dMax_X, double dMax_Y, double dMax_Z, double dMax_Rx, double dMax_Ry, double dMax_Rz,
                            double dMin_X,double dMin_Y, double dMin_Z,double dMin_Rx,double dMin_Ry, double dMin_Rz);
/**
 *	@brief: Set force distance limit 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dAllowDistance : Allowed seeking distance 
 *	@param dStrengthLevel : Power term for the deviation from boundary. 2 for square, 3 for cubic 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetForceDistanceLimit(unsigned int boxID, unsigned int rbtID, double dAllowDistance, double dStrengthLevel);

/**
 *	@brief: Set force free drive mode 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nEnableFlag : 0 for closed, 1 for open 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetForceFreeDriveMode(unsigned int boxID, unsigned int rbtID, int nEnableFlag);

/**
 *	@brief:Read force control calibration data 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dX~Rz : Calibration data 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadFTCabData(unsigned int boxID, unsigned int rbtID, double& dX, double& dY, double& dZ, double& dRX, double& dRY, double& dRZ);

/**
 *	@brief: Set the end freedom in free drive 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nX-nRz : The available freedom parameters in free drive 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetFreeDriveMotionFreedom(unsigned int boxID, unsigned int rbtID, int nX, int nY, int nZ, int nRx, int nRy, int nRz);

/**
 *	@brief: Set force free drive translation compliance and rotation compliance 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param Linear : translation compliance
 *	@param Angular : rotation compliance
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetFTFreeFactor(unsigned int boxID, unsigned int rbtID, double Linear, double Angular);

/**
 *	@brief: Set speed mode for force free drive 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nMode : speed mode, 0 as normal mode, 1 as slow mode, 2 as fast mode,3 as Welding mode 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetFTFreeDriveSpeedMode(unsigned int boxID, unsigned int rbtID, int nMode);

/**
 *	@brief:Read force control calibration data 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nMode : speed mode, 0 as normal mode, 1 as slow mode, 2 as fast mode,3 as Welding mode  
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadFTFreeDriveSpeedMode(unsigned int boxID, unsigned int rbtID, int &nMode);

/**
 *	@brief: Set the force control free drive filter mode 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nMode : filter mode, 0 as close filter, 1 as open input low-pass filter, 2 as open output mean filter 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetFTFreeDriveFilter(unsigned int boxID, unsigned int rbtID, int nMode);

/**
 *	@brief:Read the force control free drive filter mode 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nMode : filter mode, 0 as close filter, 1 as open input low-pass filter, 2 as open output mean filter 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadFTFreeDriveFilter(unsigned int boxID, unsigned int rbtID, int &nMode);

/**
 *	@brief: Set the maximum value, minimum value of the tangential force in the x/y direction and the maximum speed of lifting 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dMax : Max tangential force (N) 
 *	@param dMin : Min tangential force (N) 
 *  @param dVel : Max lifting Velocity (mm/s) 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetTangentForceBounds(unsigned int boxID, unsigned int rbtID, double dMax, double dMin, double dVel);

/**
 *	@brief: Set the orientation compensation force size and vector direction [x,y,z] in FreeDrive mode 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dForce : Compensation force size(N) 
 *	@param dX-dZ :The vactor direction of compensation force based Base 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetFreeDriveCompensateForce(unsigned int boxID, unsigned int rbtID, double dForce, double dX, double dY, double dZ);

/**
 *	@brief: Set the six-dimensional force activation threshold (force and torque) 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dForceThreshold : Force activation threshold(N) 
 *	@param dTorqueThreshold : Torque activation threshold (Nm) 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetFTWrenchThresholds(unsigned int boxID, unsigned int rbtID, double dForceThreshold, double dTorqueThreshold);

/**
 *	@brief: Set the maximum linear speed and attitude angular speed of force free drive 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dMaxLinearVelocity : Max linear speed  of force free drive(mm/s) 
 *	@param dMaxAngularVelocity : Max attitude angular speed  of force free drive (°/s) 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetMaxFreeDriveVel(unsigned int boxID, unsigned int rbtID, double dMaxLinearVelocity, double dMaxAngularVelocity);

/**
 *	@brief: Read the end degrees of freedom of the force free drive 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nX-nRz : The available freedom parameters in free drive 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadFTMotionFreedom(unsigned int boxID,unsigned int rbtID,int& nX,int& nY,int& nZ,int& nRx,int& nRy,int& nRz);

/**
 *	@brief: Set each degree of freedom (X/Y/Z/RX/RY/RZ) to force control the maximum distance of exploration 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param Dis_X-Dis_RZ : Max distance of exploration(mm) 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetMaxSearchDistance(unsigned int boxID,unsigned int rbtID,double Dis_X,double Dis_Y,double Dis_Z,double Dis_RX,double Dis_RY,double Dis_RZ);

/**
 *	@brief: Set constant force control stability phase boundary 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param Pos_X-Pos_RZ : Max distance of positive (mm)  
 *	@param Neg_X-Neg_RZ : Max distance of negative (mm)  
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetSteadyContactDeviationRange(unsigned int boxID,unsigned int rbtID,double Pos_X,double Pos_Y,double Pos_Z,double Pos_RX,double Pos_RY,double Pos_RZ,
                                                double Neg_X,double Neg_Y,double Neg_Z,double Neg_RX,double Neg_RY,double Neg_RZ);

/**
 *	@brief: Set current UCS 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dUcs_X~Rz : Pose of UCS 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	
ROBOT_PRO_API int HRIF_SetFTUCS(unsigned int boxID, unsigned int rbtID, double dUcs_X, double dUcs_Y, double dUcs_Z, double dUcs_Rx, double dUcs_Ry, double dUcs_Rz);

/**
 *	@brief: Read current UCS 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID :  Robot ID, 0 as default 
 *	@param dUcs_X~Rz : Pose of UCS 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadFTUCS(unsigned int boxID, unsigned int rbtID, double& dUcs_X, double& dUcs_Y, double& dUcs_Z, double& dUcs_Rx, double& dUcs_Ry, double& dUcs_Rz);


//************************************************************************/
//**    Interfaces for moving 
//************************************************************************/
/**
 *	@brief: Start joint short jog 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nAxisId : Axis ID, 0~5 
 *	@param nDirection : Moving direction, 0 for negative and 1 for positive 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ShortJogJ(unsigned int boxID, unsigned int rbtID, int nAxisId, int nDirection);

/**
 *	@brief: Cartesian short jog 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nAxisId : Axis ID, 0~5 
 *	@param nDirection :  Moving direction, 0 for negative and 1 for positive 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ShortJogL(unsigned int boxID, unsigned int rbtID, int nAxisId, int nDirection);

/**
 *	@brief: Start joint long jog 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nAxisId : Axis ID, 0~5 
 *	@param nDirection : Moving direction, 0 for negative and 1 for positive 
 *	@param nState : 0 for closed and 1 for open
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_LongJogJ(unsigned int boxID, unsigned int rbtID, int nAxisId, int nDirection, int nState);

/**
 *	@brief: Start cartesian long jog 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nAxisId : Axis ID, 0~5 
 *	@param nDirection : Moving direction, 0 for negative and 1 for positive 
 *	@param nState : 0 for closed and 1 for open
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_LongJogL(unsigned int boxID, unsigned int rbtID, int nAxisId, int nDirection, int nState);

/**
 *	@brief: Continue long jog. Successively call this function after HRIF_ShortJogL or HRIF_LongJogJ with less than 500 ms interval to keep continuous moving. 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_LongMoveEvent(unsigned int boxID, unsigned int rbtID);

//////////////////////////////////////////////////////////////////////////////////////

/**
 *	@brief: Check if the motion is done 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param bDone : true for done, false for not done 
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call, the value of bDone is meaningless
 */
ROBOT_PRO_API int HRIF_IsMotionDone(unsigned int boxID, unsigned int rbtID, bool& bDone);

/**
 *	@brief: Check if the waypoint blending is done 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param bDone : true for done, false for not done 
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call, the value of bDone is meaningless
 */
ROBOT_PRO_API int HRIF_IsBlendingDone(unsigned int boxID, unsigned int rbtID, bool& bDone);

/**
 *	@brief: Start waypoint move. 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nMoveType : Move type, 0 for MoveJ, 1 for MoveL 
 *	@param dX~Rz : Target pose, invalid when IsUseJoint equals 1. Target joint positions is obtained through inversely solving from this pose when IsUseJoint equals 0 
 *	@param dJ1~6 : Target joint positions when IsUseJoint equals 1; Reference joint positions for the inversely solving when IsUseJoint equals 0. 
 *	@param dTcp_X~Rz : Target TCP, invalid when IsUseJoint equals 1 
 *	@param dUcs_X~Rz : Target UCS, invalid when IsUseJoint equals 1 
 *	@param dVelocity : Motion speed, ( mm/s or °/s) 
 *	@param dAcc : Motion acceleration, ( mm/(s^2) or °/(s^2) ) 
 *	@param dRadius : Blending radius, (mm) 
 *	@param nIsUseJoint : Using joint or not, usable when MoveType equals 0, 0 for not using joint, 1 for using joint 
 *	@param nIsSeek,nIOBit,nIOState:Seeking or not, 1 for seeking; Motion stops when DO-nIOBit equals nIoState 
 *	@param strCmdID:Command ID, waypoint ID also, customized or set to 1, 2, 3 in order 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	
ROBOT_PRO_API int HRIF_WayPointEx(unsigned int boxID, unsigned int rbtID, int nMoveType,
                    double dX, double dY, double dZ, double dRx, double dRy, double dRz,                        
                    double dJ1, double dJ2, double dJ3, double dJ4, double dJ5, double dJ6,                      
                    double dTcp_X, double dTcp_Y, double dTcp_Z, double dTcp_Rx, double dTcp_Ry, double dTcp_Rz,
                    double dUcs_X, double dUcs_Y, double dUcs_Z, double dUcs_Rx, double dUcs_Ry, double dUcs_Rz,
                    double dVelocity, double dAcc, double dRadius, int nIsUseJoint, int nIsSeek, int nIOBit, int nIOState, string strCmdID);
/**
 *	@brief: Start waypoint move. 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nMoveType : Move type, 0 for MoveJ, 1 for MoveL 
 *	@param dX~Rz : Target pose, invalid when IsUseJoint equals 1. Target joint positions is obtained through inversely solving from this pose when IsUseJoint equals 0 
 *	@param dJ1~6 : Target joint positions when IsUseJoint equals 1; Reference joint positions for the inversely solving when IsUseJoint equals 0. 
 *	@param sTcpName : Target TCP name, invalid when IsUseJoint equals 1, "TCP" is valid as default 
 *	@param sUcsName : Target UCS name, invalid when IsUseJoint equals 1, "Base" is valid as default 
 *	@param dVelocity : Motion speed, mm/s or °/s 
 *	@param dAcc : Motion acceleration, mm/(s^2) or °/(s^2) 
 *	@param dRadius : Blending radius, mm 
 *	@param nIsUseJoint : Using joint or not, usable when MoveType equals 0, 0 for not using joint, 1 for using joint 
 *	@param nIsSeek,nIOBit,nIOState:Seeking or not, 1 for seeking; Motion stops when DO-nIOBit equals nIoState 
 *	@param strCmdID:Command ID, waypoint ID also, customized or set to 1, 2, 3 in order 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	
ROBOT_PRO_API int HRIF_WayPoint(unsigned int boxID, unsigned int rbtID, int nMoveType,
                double dX, double dY, double dZ, double dRx, double dRy, double dRz,                         
                double dJ1, double dJ2, double dJ3, double dJ4, double dJ5, double dJ6,
                string sTcpName, string sUcsName, double dVelocity, double dAcc, double dRadius, 
                int nIsUseJoint,int nIsSeek, int nIOBit, int nIOState, string strCmdID);

/**
 *	@brief: Start waypoint move. 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nMoveType : Move type, 0 for MoveJ, 1 for MoveL, 2 for MoveC 
 *	@param dEndPos_X~Rz : Target pose, invalid when IsUseJoint equals 1. Target pose is obtained through inverse kinematics from this pose when IsUseJoint equals 0 
*	@param dAuxPos_X~Rz : Target auxiliary pose invalid when IsUseJoint equals 1, used as the middle point when nMoveType equals 2. Target pose is obtained through inverse kinematics from this pose when IsUseJoint equals 0 
 *	@param dJ1~6 : Target joint positions when IsUseJoint equals 1; Reference joint positions for the inverse kinematics when IsUseJoint equals 0. 
 *	@param sTcpName : Target TCP name, invalid when IsUseJoint equals 1, "TCP" is valid as default 
 *	@param sUcsName : Target UCS name, invalid when IsUseJoint equals 1, "Base" is valid as default 
 *	@param dVelocity : Motion speed, mm/s or °/s 
 *	@param dAcc : Motion acceleration, mm/(s^2) or °/(s^2) 
 *	@param dRadius : Blending radius, mm 
 *	@param nIsUseJoint : Using joint or not, usable when MoveType equals 0, 0 for not using joint, 1 for using joint 
 *	@param nIsSeek,nIOBit,nIOState:Seeking or not, 1 for seeking; Motion stops when DO-nIOBit equals nIoState 
 *	@param strCmdID:Command ID, waypoint ID also, customized or set to 1, 2, 3 in order 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	
ROBOT_PRO_API int HRIF_WayPoint2(unsigned int boxID, unsigned int rbtID, int nMoveType,
                double dEndPos_X, double dEndPos_Y, double dEndPos_Z, double dEndPos_Rx, double dEndPos_Ry, double dEndPos_Rz,   
                double dAuxPos_X, double dAuxPos_Y, double dAuxPos_Z, double dAuxPos_Rx, double dAuxPos_Ry, double dAuxPos_Rz,                       
                double dJ1, double dJ2, double dJ3, double dJ4, double dJ5, double dJ6,
                string sTcpName, string sUcsName, double dVelocity, double dAcc, double dRadius, 
                int nIsUseJoint,int nIsSeek, int nIOBit, int nIOState, string strCmdID);


/**
 *	@brief: Start joint move. HRIF_WayPoint is more recommended yet 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dX~Rz : Target pose, invalid when IsUseJoint equals 1. Target joint positions is obtained through inversely solving from this pose when IsUseJoint equals 0 
 *	@param dJ1~6 : Target joint positions when IsUseJoint equals 1; Reference joint positions for the inversely solving when IsUseJoint equals 0. 
 *	@param sTcpName : Target TCP name, invalid when IsUseJoint equals 1, "TCP" is valid as default 
 *	@param sUcsName : Target UCS name, invalid when IsUseJoint equals 1, "Base" is valid as default 
 *	@param dVelocity : Motion speed, mm/s or °/s 
 *	@param dAcc : Motion acceleration, mm/(s^2) or °/(s^2) 
 *	@param dRadius : Blending radius, mm 
 *	@param nIsUseJoint : Using joint or not, usable when MoveType equals 0, 0 for not using joint, 1 for using joint 
 *	@param nIsSeek,nIOBit,nIOState:Seeking or not, 1 for seeking; Motion stops when DO-nIOBit equals nIoState 
 *	@param strCmdID:Command ID, waypoint ID also, customized or set to 1, 2, 3 in order 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	
ROBOT_PRO_API int HRIF_MoveJ(unsigned int boxID, unsigned int rbtID, double dX, double dY, double dZ, double dRx, double dRy, double dRz,
                double dJ1, double dJ2, double dJ3, double dJ4, double dJ5, double dJ6,
                string sTcpName, string sUcsName, double dVelocity, double dAcc, double dRadius, 
                int nIsUseJoint,int nIsSeek, int nIOBit, int nIOState, string strCmdID);

/**
 *	@brief: Start linear move. HRIF_WayPoint is more recommended yet 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dX~Rz : Target pose 
 *	@param dJ1~6 : Reference joint positions near to the target pose 
 *	@param sTcpName : TCP name 
 *	@param sUcsName : UCS name 
 *	@param dVelocity : Motion speed, mm/s or °/s 
 *	@param dAcc : Motion acceleration, mm/(s^2) or °/(s^2) 
 *	@param dRadius :Blending radius, mm 
 *	@param nIsSeek,nIOBit,nIOState:Seeking or not, 1 for seeking; Motion stops when DO-nIOBit equals nIoState 
 *	@param strCmdID:Command ID, waypoint ID also, customized or set to 1, 2, 3 in order 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	
ROBOT_PRO_API int HRIF_MoveL(unsigned int boxID, unsigned int rbtID, double dX, double dY, double dZ, double dRx, double dRy, double dRz,
                double dJ1, double dJ2, double dJ3, double dJ4, double dJ5, double dJ6,
                string sTcpName, string sUcsName, double dVelocity, double dAcc, double dRadius,
                int nIsSeek, int nIOBit, int nIOState, string strCmdID);

/**
 *	@brief: Start linear move. HRIF_WayPoint is more recommended yet / 机器人直线运动到指定的空间坐标位置, 建议使用HRIF_WayPoint
 *	@param boxID : Control box ID, 0 as default / 电箱ID号，默认值=0
 *	@param rbtID : Robot ID, 0 as default / 机器人ID号，默认值=0
 *	@param dX~Rz : Target pose
 *	@param nCf1, dnCf4, dnCf6: Joint configurations for axes 1, 4, and 6, range: -4 ~ 3
 *	@param nCfx: Joint configuration for axis 5, range: 0 ~ 7
 *	@param sTcpName : TCP name
 *	@param sUcsName : UCS name
 *	@param dVelocity : Motion speed, mm/s or °/s
 *	@param dAcc : Motion acceleration, mm/(s^2) or °/(s^2)
 *	@param dRadius :Blending radius, mm
 *	@param strCmdID：Command ID, waypoint ID also, customized or set to 1, 2, 3 in order
 *	
 *	@return : nRet=0 : Function call succeeded
 *            nRet>0 : Error code of function call
 */	
ROBOT_PRO_API int HRIF_MoveL2(unsigned int boxID, unsigned int rbtID, double dX, double dY, double dZ, double dRx, double dRy, double dRz,
                int nCf1, int dnCf4, int dnCf6, int dnCfx,
                string sTcpName, string sUcsName, double dVelocity, double dAcc, double dRadius, string strCmdID);

/**
 *	@brief: Start round move 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dStartPos_X~Rz : Start pose 
 *	@param dAuxPos_X~Rz : Via pose 
 *	@param dEndPos_X~Rz : End pose 
 *	@param nFixedPosure : 0 for unfixed orientation, 1 for fixed orientation, 2 for newly added gradual orientation arc motion 
 *
 *	@param nMoveCType : 1 for arc, 0 for circle 
 *	@param dRadLen : Invalid when nMoveCType equals 1; Circles when nMoveCType equals 0 
 *			         
 *	@param dVelocity : Motion speed, mm/s 
 *	@param dAcc : Motion acceleration, mm/(s^2) 
 *	@param dRadius : Blending radius, mm 
 *	@param sTcpName : TCP name 
 *	@param sUcsName : UCS name 
 *	@param sCmdID : Command ID 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	
ROBOT_PRO_API int HRIF_MoveC(unsigned int boxID, unsigned int rbtID,
                double dStartPos_X,double dStartPos_Y,double dStartPos_Z,double dStartPos_Rx,double dStartPos_Ry,double dStartPos_Rz,
                double dAuxPos_X,double dAuxPos_Y,double dAuxPos_Z,double dAuxPos_Rx,double dAuxPos_Ry,double dAuxPos_Rz,
                double dEndPos_X,double dEndPos_Y,double dEndPos_Z,double dEndPos_Rx,double dEndPos_Ry,double dEndPos_Rz,
                int nFixedPosure,int nMoveCType,double dRadLen,double dVelocity, double dAcc, 
                double dRadius, string sTcpName, string sUcsName, string sCmdID);

/**
 *	@brief: Start round move 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dStartPos_X~Rz : Start pose 
 *	@param dAux1Pos_X~Rz: Via pose 1 
 *	@param dAux2Pos_X~Rz : Via pose 2 
 *	@param dAngle : The angle at which the entire circle runs
 *	@param sTcpName : TCP name 
 *	@param sUcsName : UCS name 
 *	@param dVelocity : Motion speed, mm/s 
 *	@param dAcc : Motion acceleration, mm/(s^2) 
 *	@param dRadius : Blending radius, mm 
 *	@param nIsSeek,nIOBit,nIOState:Seeking or not, 1 for seeking; Motion stops when DO-nIOBit equals nIoState 
 *	@param sCmdID : Command ID 
 *	@param vecLastPoint : The end pose of the trajectory 
 *
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	
ROBOT_PRO_API int HRIF_MoveCAngle(unsigned int boxID, unsigned int rbtID,
                double dStartPos_X,double dStartPos_Y,double dStartPos_Z,double dStartPos_Rx,double dStartPos_Ry,double dStartPos_Rz,
                double dAux1Pos_X,double dAux1Pos_Y,double dAux1Pos_Z,double dAux1Pos_Rx,double dAux1Pos_Ry,double dAux1Pos_Rz,
                double dAux2Pos_X,double dAux2Pos_Y,double dAux2Pos_Z,double dAux2Pos_Rx,double dAux2Pos_Ry,double dAux2Pos_Rz,
                double dAngle, string sTcpName, string sUcsName, double dVelocity, double dAcc, 
                double dRadius, int nIsSeek, int nBit, int nDIType, string sCmdID, vector<string> &vecLastPoint);

/**
 *	@brief: Start round move 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dAuxPos_X~Rz : Via pose 
 *	@param dEndPos_X~Rz : End pose 
 *	@param nFixedPosure : 0 for Not fixed posture and posture angle change less than 180 °, 1 for fixed orientation, 2 for Unsteady posture and posture angle change greater than 180 °
 *                       
 *	@param dRadLen : When the arc length is 0, it means that the target pose is determined by endPose. If set to 360 °, it can run a full circle, and if set to 720 °, it can run two circles
 *                                            
 *	@param dVelocity : Motion speed, mm/s 
 *	@param dAcc : Motion acceleration, mm/(s^2) 
 *	@param dRadius : Blending radius, mm 
 *	@param sTcpName : TCP name 
 *	@param sUcsName : UCS name 
 *	@param sCmdID : Command ID 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	
ROBOT_PRO_API int HRIF_MoveC2(unsigned int boxID, unsigned int rbtID,
    double dAuxPos_X,double dAuxPos_Y,double dAuxPos_Z,double dAuxPos_Rx,double dAuxPos_Ry,double dAuxPos_Rz,
    double dEndPos_X,double dEndPos_Y,double dEndPos_Z,double dEndPos_Rx,double dEndPos_Ry,double dEndPos_Rz,
    int nFixedPosure,double dRadLen,double dVelocity, double dAcc, 
    double dRadius, string sTcpName, string sUcsName, string sCmdID);

/**
 *	@brief: Start zigzag move 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dStartPos_X~Rz : Start pose 
 *	@param dEndPos_X~Rz : End pose 
 *	@param dPlanePos_X~Rz : Plane pose 
 *	@param dVelocity : Motion speed, mm/s 
 *	@param dAcc : Motion acceleration, mm/(s^2) 
 *	@param dWIdth : Width 
 *	@param dDensity : Density, mm 
 *	@param nEnableDensity : 0 is disable density and 1 for enable density 
 *	@param nEnablePlane : Use plane pose or not; UCS is automatically used if plane pose is not used 
 *	@param nEnableWaiTime : 0 for disable waiting time, 1 for enable waiting time 
 *	@param nPosiTime : Waiting time (ms) for Positive turning point 
 *	@param nNegaTime : Waiting time (ms) for Negative turning point 
 *	@param dRadius : Blending radius, mm 
 *	@param sTcpName : TCP name 
 *	@param sUcsName : UCS name 
 *	@param sCmdID : Command ID 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	
ROBOT_PRO_API int HRIF_MoveZ(unsigned int boxID, unsigned int rbtID,
                double dStartPos_X,double dStartPos_Y,double dStartPos_Z,double dStartPos_Rx,double dStartPos_Ry,double dStartPos_Rz,
                double dEndPos_X,double dEndPos_Y,double dEndPos_Z,double dEndPos_Rx,double dEndPos_Ry,double dEndPos_Rz,
                double dPlanePos_X,double dPlanePos_Y,double dPlanePos_Z,double dPlanePos_Rx,double dPlanePos_Ry,double dPlanePos_Rz,
                double dVelocity, double dAcc, double dWIdth, double dDensity, int nEnableDensity, int nEnablePlane, int nEnableWaiTime, 
                int nPosiTime, int nNegaTime, double dRadius, string sTcpName, string sUcsName, string sCmdID);

/**
 *	@brief: Start linear weave move 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dStartPos_X~Rz : Start pose 
 *	@param dEndPos_X~Rz : End pose 
 *	@param dVelocity : Motion speed, mm/s 
 *	@param dAcc : Motion acceleration, mm/(s^2) 
 *	@param dRadius : Blending radius, mm 
 *	@param dAmplitude : Weaving amplitude, mm 
 *	@param dIntervalDistance : Referenced weaving interval, robot will rectify it slightly to ensure the StartPos and EndPos be passed through after some weaving cycles, mm 
 *	@param nWeaveFrameType : the way to determine the weaving plane and weaving direction
 *                           0: The +X of weaving plane is from StartPos to EndPos. The +Z(normal direction) of weaving plane is along TCP +Z. The +Y of weaving plane is determined by +X and +Z. The start direction of weaving move is toward the +Y side.
 *                           1: The +X of weaving plane is from StartPos to EndPos. The Z of weaving plane is parallel with TCP Z. The +Y of weaving plane is along TCP +Y. The start direction of weaving move is toward the +Y side.
 *                           
 *                           
 *                          
 *	@param dElevation : The elevation of the weave, degree
 *	@param dAzimuth : The azimuth of the weave, degree 
 *	@param dCentreRise : The bulge height of the welding torch at the weaving weld center, mm 
 *  @param nEnableWaiTime : 0 to disable waiting time, 1 to enable waiting time 
 *	@param nPosiTime : Waiting time (ms) at Positive turning point 
 *	@param nNegaTime : Waiting time (ms) at Negative turning point 
 *	@param sTcpName : TCP name 
 *	@param sUcsName : UCS name 
 *	@param sCmdID : Command ID 
 *  @param vecLastPoint : The end pose of the trajectory 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	
ROBOT_PRO_API int HRIF_MoveLinearWeave(unsigned int boxID, unsigned int rbtID,
                double dStartPos_X,double dStartPos_Y,double dStartPos_Z,double dStartPos_Rx,double dStartPos_Ry,double dStartPos_Rz,
                double dEndPos_X,double dEndPos_Y,double dEndPos_Z,double dEndPos_Rx,double dEndPos_Ry,double dEndPos_Rz,
                double dVelocity, double dAcc, double dRadius, double dAmplitude, double dIntervalDistance, int nWeaveFrameType, double dElevation, 
                double dAzimuth, double dCentreRise, int nEnableWaiTime, int nPosiTime, int nNegaTime, string sTcpName, string sUcsName, string sCmdID, vector<string> &vecLastPoint);

/**
 *	@brief: Start circular weave move 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dStartPos_X~Rz : Start pose 
 *	@param dAuxPos_X~Rz : via pose 
 *	@param dEndPos_X~Rz : End pose 
 *	@param dVelocity : Motion speed, mm/s 
 *	@param dAcc : Motion acceleration, mm/(s^2) 
 *	@param dRadius : Blending radius, mm 
 *	@param nOrientMode : 0 for unfixed orientation, 1 for fixed orientation
 *	@param nMoveWhole : 0 for circle, 1 for arc
 *	@param dMoveWholeLen : Invalid when nMoveCType equals 1; Circles when nMoveCType equals 0
 *                          
 *	@param dAmplitude : Amplitude, mm 
 *	@param dIntervalDistance : Referenced weaving interval, robot will rectify it slightly to ensure the StartPos and EndPos be passed through after some weaving cycles, mm 
 *	@param nWeaveFrameType : the way to determine the weaving plane and weaving direction
 *                           0: The +X of weaving plane is from StartPos to EndPos. The +Z(normal direction) of weaving plane is along TCP +Z. The +Y of weaving plane is determined by +X and +Z. The start direction of weaving move is toward the +Y side.
 *                           1: The +X of weaving plane is from StartPos to EndPos. The Z of weaving plane is parallel with TCP Z. The +Y of weaving plane is along TCP +Y. The start direction of weaving move is toward the +Y side.
 *                           
 *                           
 *                           
 *	@param dElevation : The elevation of the weave, degree 
 *	@param dAzimuth : The azimuth of the weave, degree 
 *	@param dCentreRise : The bulge height of the welding torch at the weaving weld center, mm 
 *  @param nEnableWaiTime : 0 to disable waiting time, 1 to enable waiting time 
 *	@param nPosiTime : Waiting time (ms) at Positive turning point 
 *	@param nNegaTime : Waiting time (ms) at Negative turning point 
 *	@param sTcpName : TCP name 
 *	@param sUcsName : UCS name 
 *	@param sCmdID : Command ID 
 *  @param vecLastPoint : The end pose of the trajectory 
 *
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	
ROBOT_PRO_API int HRIF_MoveCircularWeave(unsigned int boxID, unsigned int rbtID,
                double dStartPos_X,double dStartPos_Y,double dStartPos_Z,double dStartPos_Rx,double dStartPos_Ry,double dStartPos_Rz,
                double dAuxPos_X,double dAuxPos_Y,double dAuxPos_Z,double dAuxPos_RX,double dAuxPos_RY,double dAuxPos_RZ,
                double dEndPos_X,double dEndPos_Y,double dEndPos_Z,double dEndPos_Rx,double dEndPos_Ry,double dEndPos_Rz,
                double dVelocity, double dAcc, double dRadius, int nOrientMode, int nMoveWhole, double dMoveWholeLen, 
                double dAmplitude, double dIntervalDistance, int nWeaveFrameType, double dElevation, double dAzimuth, 
                double dCentreRise, int nEnableWaiTime, int nPosiTime, int nNegaTime, string sTcpName, string sUcsName, string sCmdID, vector<string> &vecLastPoint);

/**
 *	@brief: Start elliptical move 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dP1_X~Rz : Teaching p1 (p1~p5 should be listed as sequence in arc)
 *	@param dP2_X~Rz : Teaching p2 
 *	@param dP3_X~Rz : Teaching p3 
 *	@param dP4_X~Rz : Teaching p4 
 *	@param dP5_X~Rz : Teaching p5 
 *	@param nOrientMode : 0 is for arc, 1 is for circle 
 *	@param nMoveType : 0 for unfixed orientation , 1 for fixed orientation 
 *	@param dArcLength : Arc length (mm) 
 *	@param dVelocity : Motion speed, mm/s 
 *	@param dAcc : Motion acceleration, mm/(s^2) 
 *	@param dRadius : Blending radius, mm 
 *	@param sTcpName : TCP name 
 *	@param sUcsName : UCS name 
 *	@param sCmdID : Command ID 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	

ROBOT_PRO_API int HRIF_MoveE(unsigned int boxID, unsigned int rbtID,
                double dP1_X, double dP1_Y, double dP1_Z, double dP1_Rx, double dP1_Ry, double dP1_Rz, 
                double dP2_X, double dP2_Y, double dP2_Z, double dP2_Rx, double dP2_Ry, double dP2_Rz, 
                double dP3_X, double dP3_Y, double dP3_Z, double dP3_Rx, double dP3_Ry, double dP3_Rz, 
                double dP4_X, double dP4_Y, double dP4_Z, double dP4_Rx, double dP4_Ry, double dP4_Rz, 
                double dP5_X, double dP5_Y, double dP5_Z, double dP5_Rx, double dP5_Ry, double dP5_Rz, 
                int nOrientMode, int nMoveType, double dArcLength,
                double dVelocity, double dAcc, double dRadius, string sTcpName, string sUcsName, string sCmdID);

/**
 *	@brief: Start spiral move 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dSpiralIncrement : Spiral Increment radius 
 *	@param dSpiralRadius : Spiral end radius 
 *	@param dVelocity : Motion speed, mm/s 
 *	@param dAcc : Motion acceleration, mm/(s^2) 
 *	@param dRadius : Blending radius, mm 
 *	@param sUcsName : UCS name 
 *	@param sCmdID : Command ID 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	
ROBOT_PRO_API int HRIF_MoveS(unsigned int boxID, unsigned int rbtID, double dSpiralIncrement, double dSpiralRadius,
                double dVelocity, double dAcc, double dRadius, string sTCPName, string sUcsName, string sCmdID);

/**
 *	@brief: Start relative joint move 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nAxisId: Axis ID , 0~5 for jonit 1 ~6 
 *	@param nDirection: Move direction, 0 for negative,1 for positive 
 *	@param dDistance: Move distance (°) 
 *
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_MoveRelJ(unsigned int boxID, unsigned int rbtID, int nAxisId, int nDirection, double dDistance);

/**
 *	@brief: Start relative linear move 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nPoseId: Pose ID, 0~5 for X, Y, Z, Rx, Ry, Rz 
 *	@param nDirection: Move direction, 0 for negative, 1 for positive 
 *	@param dDistance: Move distance (mm or °) 
 *  @param nToolMotion : Tool motion, 0 for UCS based move, 1 for TCP based move 
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_MoveRelL(unsigned int boxID, unsigned int rbtID, int nPoseId, int nDirection, double dDistance, int nToolMotion);

/**
 *	@brief: Start relative wayPoint 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nType : Move type, 0 for joint, 1 for linear 
 *  @param nPointList : Using point saved in points list or not, 0 for not using,  1 for using 
 *  @param dPos_X~Rz : Target pose 
 *  @param dPos_J1~6 : Target joint positions 
 *  @param nrelMoveType : Relative move type, 0 for absolute value, 1 for superimposed value 
 *  @param nAxisMask_1~6 : Is Axis/Direction Moving, 0 for not moving, 1 for moving 
 *  @param dTarget_1~6 : Moving distance; invalid when nAxisMask equals 0; joint move an absolute or superimposed distance when nAxisMask equals 1 and nType equals 0; Pose move an absolute or superimposed distance when nAxisMask equals 1 and nType equals 1. 
 *	@param sTcpName : Target TCP name, invalid when IsUseJoint equals 1, "TCP" is valid as default 
 *	@param sUcsName : Target UCS name, invalid when IsUseJoint equals 1, "Base" is valid as default 
 *  @param dVelocity: Velocity (mm/s) or (°/s) 
 *  @param dAcc: Acceleration (mm/s^2) or (°/s^2) 
 *  @param dRadius: Blending radius, mm 
 *	@param nIsUseJoint : Using joint or not, usable when MoveType equals 0, 0 for not using joint, 1 for using joint 
 *	@param nIsSeek,nIOBit,nIOState:Seeking or not, 1 for seeking; Motion stops when DO-nIOBit equals nIoState 
 *	@param strCmdID:Command ID, waypoint ID also, customized or set to 1, 2, 3 in order 
 *
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_WayPointRel(unsigned int boxID, unsigned int rbtID,
    int nType,
    int nPointList,
    double dPos_X, double dPos_Y, double dPos_Z, double dPos_Rx, double dPos_Ry, double dPos_Rz,
    double dPos_J1, double dPos_J2, double dPos_J3, double dPos_J4, double dPos_J5, double dPos_J6,
    int nrelMoveType,
    int nAxisMask_1, int nAxisMask_2, int nAxisMask_3, int nAxisMask_4, int nAxisMask_5, int nAxisMask_6,
    double dTarget_1, double dTarget_2, double dTarget_3, double dTarget_4, double dTarget_5, double dTarget_6,
    string sTcpName, string sUcsName,
    double dVelocity, double dAcc, double dRadius,
    int nIsUseJoint, int nIsSeek, int nIOBit, int nIOState, string strCmdID);

//************************************************************************/
//**    Interfaces for trajectory 
//************************************************************************/
/**
 *	@brief: Initialize path 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nRawDataType : Raw points data type, 0 for ACS, 1 for PCS 
 *	@param sPathName : Path name 
 *	@param dSpeedRatio : Speed override for PathJ calculation 
 *	@param dRadius : Blending radius for PathJ calculation, mm 
 *	@param dVelocity : Velocity for PathL calculation 
 *	@param dAcc : Acceleration for PathL calculation 
 *	@param dJerk : Jerk for PathL calculation 
 *	@param sUcsName : UCS name of raw point for PathL calculation 
 *	@param sTcpName : TCP name of raw point for PathL calculation 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_InitPath(unsigned int boxID, unsigned int rbtID, int nRawDataType, string sPathName, double dSpeedRatio,
    double dRadius, double dVelocity, double dAcc, double dJerk, string sUcsName, string sTcpName);

/**
 *	@brief: Push raw points to path 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param sPathName : Path name 
 *	@param sPoints : Points data, separated by comma, including 6 data for each point. 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_PushPathPoints(unsigned int boxID, unsigned int rbtID, string sPathName, string sPoints);

/**
 *	@brief: End pushing points to the path and start calculating the pathJ/L 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param sPathName : Path name 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_EndPushPathPoints(unsigned int boxID,unsigned int rbtID, string sPathName);

/**
 *	@brief: Delete the Path with the specified name 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param sPathName : Path name 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_DelPath(unsigned int boxID, unsigned int rbtID, string sPathName);

/**
 *	@brief: Read all path lists 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param result : Path name list 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadPathList(unsigned int boxID, unsigned int rbtID, string &result);

/**
 *	@brief: Read the information of the specified name path 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param sPathName : Path name 
 *	@param result : Path name list 
 *           
 *           nRawDataType:Raw point type, 0 for ACS, 1 for PCS 
 *           stateJ:PathJ status: Not taught (0), Teaching in progress (1), Teaching completed (4), Calculating (2), Calculation error (5), 
 *                  Calculation completed (3), Imported (9), Post-import processing ( 10) 
 *                  
 *           ErrorCodeJ:PathJ error code 
 *           stateL:The state of PathL, the same as stateJ 
 *           ErrorCodeL:PathL error code 
 *           dOverride:Path speed ratio 
 *           dRadius:Blending radius, mm 
 *           dLinearVel:Velocity for Path 
 *           dLinearAcc: Acceleration for Path 
 *           dLinearJerk:Jerk for Path 
 *           dUcs_X-dUcs_Rz:User coordinates 
 *           dTcp_X-dTcp_Rz:Tool coordinates 
 *           nRawPointCount:Raw points count 
 *           HeadPoint[]:The first raw point coordinates (J1/J2/J3/J4/J5/J6 or X/Y/Z/RX/RY/RZ) 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadPathInfo(unsigned int boxID, unsigned int rbtID, string sPathName, string &result);

/**
 *	@brief: Update the name of the specified path 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param sPathName : Raw path name 
 *	@param sPathNewName : New path name 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_UpdatePathName(unsigned int boxID, unsigned int rbtID, string sPathName, string sPathNewName);

/**
 *	@brief: Read the status of the path 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param sPathName : Path name 
 *	@param nStateJ : nStateJ, refer to HRIF_ReadPathInfo
 *	@param nErrorCodeJ : nErrorCodeJ
 *	@param nStateL : nStateL, refer to HRIF_ReadPathInfo
 *	@param nErrorCodeL : nErrorCodeL
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadPathState(unsigned int boxID,unsigned int rbtID,string sPathName,int &nStateJ,int &nErrorCodeJ,int &nStateL,int &nErrorCodeL);

/**
 *	@brief: Run PathJ 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param sPathName : Path name 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_MovePathJ(unsigned int boxID, unsigned int rbtID, string sPathName);

/**
 *	@brief: Run PathL 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param sPathName : Path name 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_MovePathL(unsigned int boxID,unsigned int rbtID, string sPathName);

/**
 *	@brief: Start to push ACS points for MovePathJ 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param sPathName : MovePathJ name 
 *	@param dSpeedRatio : Speed override 
 *	@param dRadius : Blending radius, mm 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_StartPushMovePathJ(unsigned int boxID, unsigned int rbtID, string sPathName, double dSpeedRatio, double dRadius);

/**
 *	@brief: Push an ACS raw point to MovePathJ. 4 points at lease are needed. 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param sPathName : MovePathJ name 
 *	@param J1~6 : joint positions 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_PushMovePathJ(unsigned int boxID, unsigned int rbtID, string sPathName, double dJ1, double dJ2, double dJ3, double dJ4, double dJ5, double dJ6);

/**
 *	@brief: End pushing ACS raw point to MovePathJ and start calculating the path 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param sPathName : MovePathJ name 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_EndPushMovePathJ(unsigned int boxID, unsigned int rbtID, string sPathName);

/**
 *	@brief: End pushing PCS raw point to MovePathL and start calculating the path 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param sPathName : MovePathL name 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_EndPushMovePath(unsigned int boxID, unsigned int rbtID, string sPathName);

/**
 *	@brief: Read status of MovePathJ 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param sPathName : MovePathJ name 
 *	@param nResult : MovePathJ status 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadMovePathJState(unsigned int boxID, unsigned int rbtID, string sPathName, int& nResult);

/**
 *	@brief: Update MovePathJ name 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param sPathName : MovePathJ name 
 *	@param sPathNewName : New MovePathJ name 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_UpdateMovePathJName(unsigned int boxID, unsigned int rbtID, string PathName, string sPathNewName);

/**
 *	@brief: Delete a MovePathJ 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param sPathName : MovePathJ name 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_DelMovePathJ(unsigned int boxID, unsigned int rbtID, string sPathName);

/**
 *	@brief:  Initialize MovePathL 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param sPathName : MovePathL name 
 *	@param dVelocity : Velocity 
 *	@param dAcceleration : Acceleration 
 *	@param jerk : Jerk 
 *	@param sUcsName : UCS name 
 *	@param sTcpName : TCP name 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_InitMovePathL(unsigned int boxID, unsigned int rbtID, string sPathName, double dVelocity, double dAceleration, double djerk, string sUcsName, string sTcpName);

/**
 *	@brief: Push an PCS point to MovePathL 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param sPathName : MovePathL name 
 *	@param dX~Rz : Raw point pose 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_PushMovePathL(unsigned int boxID, unsigned int rbtID, string sPathName, double dX, double dY, double dZ, double dRX, double dRY, double dRZ);

/**
 *	@brief: Push an list of ACS or PCS points to MovePathJ or MovePathL 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param sPathName : MovePathJ or MovePathL name 
 *	@param nMoveType : Move type, 0 for MovePathJ, 1 for MovePathL 
 *	@param nPointsSize : Points number 
 *	@param sPoints : Points list, separated by comma  
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_PushMovePaths(unsigned int boxID, unsigned int rbtID, string sPathName, int nMoveType, int nPointsSize, string sPoints);

/**
 *	@brief: Set Override for MovePathL 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dOverride: Override 
 *
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetMovePathOverride(unsigned int boxID, unsigned int rbtID, double dOverride);

/**
 *	@brief: Read process and point index of a path move 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dProcess : Current path process, 0~1, more than 0.999999 for move done 
 *	@param nIndex : Point index finished 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadTrackProcess(unsigned int boxID, unsigned int rbtID, double& dProcess, int& nIndex);

/**
 * @brief   Structure of parameters required for MovePathJOL motion points,This structure is specifically for the HRIF_MovePathJOL interface
 */
typedef struct _MovePathJOLPointsData
{
    /**
     * @brief  Joint target position of axis 1 
     */
    double  dbJ1;
    /**
     * @brief  Joint target position of axis 2 
     */
    double  dbJ2;
    /**
     *@brief  Joint target position of axis 3 
     */
    double  dbJ3;
    /**
     * @brief  Joint target position of axis 4 
     */
    double  dbJ4;
    /**
     * @brief  Joint target position of axis 5 
     */
    double  dbJ5;
    /**
     * @brief  Joint target position of axis 6 
     */
    double  dbJ6;
    /**
     * @brief  Whether IO is set / set IO
     */
    int nIsSetIO;
    /**
     * @brief  The EndDO to be modified is identified by bit
     */
    int nEndDOMask;
    /**
     * @brief  The target state of each EndDO to be modified.
     */
    int nEndDOVal;
     /**
     * @brief   The BoxDO to be modified is identified by bit. 
     */
    int nBoxDOMask;
    /**
     * @brief   The target state of each BoxDO to be modified. 
     */
    int nBoxDOVal;
    /**
     * @brief   The BoxCO to be modified is identified by bit 
     */
    int nBoxCOMask;
    /**
     * @brief   The target state of each BoxCO to be modified 
     */
    int nBoxCOVal;
    /**
     * @brief   Indicator of whether BoxAOCH0 needs to be modified
     */
    int nBoxAOCH0_Mask;
    /**
     * @brief   Mode 
     */
    int nBoxAOCH0_Mode;
    /**
     * @brief   Indicator of whether BoxAOCH1 needs to be modified
     */
    int nBoxAOCH1_Mask;
     /**
     * @brief    Mode 
     */
    int nBoxAOCH1_Mode;
    /**
     * @brief    Corresponding analog value 
     */
    double dbBoxAOCH0_Val;
    /**
     * @brief    Corresponding analog value 
     */
    double dbBoxAOCH1_Val;
    _MovePathJOLPointsData():dbJ1(0),dbJ2(0),dbJ3(0),dbJ4(0),dbJ5(0),dbJ6(0),nIsSetIO(0),nEndDOMask(0),nEndDOVal(0),nBoxDOMask(0),nBoxDOVal(0),nBoxCOMask(0),nBoxCOVal(0),nBoxAOCH0_Mask(0),nBoxAOCH0_Mode(0),nBoxAOCH1_Mask(0),nBoxAOCH1_Mode(0),dbBoxAOCH0_Val(0),dbBoxAOCH1_Val(0)
   {
   }
}MovePathJOLPointsData;

/**
 *	@brief: Start MovePathJ for online implementation planning 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dVel : Joint velocitie
 *  @param dAcc : Joint acceleration
 *  @param dTol : Filter parameter
 * @param JOLPointsData :  MovePathJOL motion point data. MovePathJOLPointsData is a structure that contains various data required for setting the MovePathJOL motion points
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_MovePathJOL(unsigned int boxID, unsigned int rbtID, int dVel, int dAcc, int dTol,vector<MovePathJOLPointsData> JOLPointsData);

/**
 *	@brief: Obtain the index number of the current point position of MovePathJOL motion and the total number of points in the trajectory motion 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nCurIndex : Point index finished 
 *  @param nIndexNum : The total number of points in the current trajectory movement 
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_GetMovePathJOLIndex(unsigned int boxID, unsigned int rbtID, int& nCurIndex, int& nIndexNum);

/**
 *	@brief:  Set the reference joint coordinates for space point trajectory
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param strPathName : Path name 
 *  @param dJ1-dJ6 : Reference joint coordinates
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetPathRefJoints(unsigned int boxID, unsigned int rbtID, string& strPathName, double dJ1, double dJ2, double dJ3, double dJ4, double dJ5, double dJ6);

//************************************************************************/
//**    Interfaces for Servo 
//************************************************************************/
/**
 *	@brief: Start Servo J/P with the specified ServoTime of update and LookaheadTime. This function is a member of the suite including HRIF_StartServo, HRIF_PushServoJ and HRIF_PushServoP. 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dServoTime : interval of update (s) 
 *	@param dLookaheadTime : LookaheadTime (s) 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_StartServo(unsigned int boxID, unsigned int rbtID, double dServoTime, double dLookaheadTime);

/**
 *	@brief: Push point of joint positions to robot with the ServoTime and LookaheadTime specified in HRIF_StartServo function, and the robot will track the joint positions in real time. This function is a member of the suite including HRIF_StartServo, HRIF_PushServoJ and HRIF_PushServoP. 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param J1~6 : Joint positions 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_PushServoJ(unsigned int boxID, unsigned int rbtID, double dJ1, double dJ2, double dJ3, double dJ4, double dJ5, double dJ6);

/**
 *	@brief: Push pose point to robot with the ServoTime and LookaheadTime specified in HRIF_StartServo function, and the robot will track the pose in real time. This function is a member of the suite including HRIF_StartServo, HRIF_PushServoJ and HRIF_PushServoP. 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param vecCoord : pose list 
 *	@param vecUcs : UCS list 
 *	@param vecTcp : TCP list 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_PushServoP(unsigned int boxID, unsigned int rbtID, vector<double>& vecCoord, vector<double>& vecUcs, vector<double>& vecTcp);

/**
 *	@brief: In SpeedJ mode,Push joint command velocity 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dJ1CmdVel~dJ6CmdVel : Joint speed, °/s
 *  @param dAcc : Joint Acc, °/s^2
 *  @param dRuntime : Command execution exceeds this time, the movement will stop, s
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SpeedJ(unsigned int boxID, unsigned int rbtID, double dJ1CmdVel,double dJ2CmdVel,double dJ3CmdVel,double dJ4CmdVel,
                    double dJ5CmdVel,double dJ6CmdVel,double dAcc,double dRuntime);

/**
 *	@brief: In SpeedL mode,Push position command velocity 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dXCmdVel~dZCmdVel : position velocity, mm/s
 *	@param dRxCmdVel~dRzCmdVel : angular velocity, °/s
 *  @param dLinearAcc : linear Acc, mm/s^2
 *  @param dAngularAcc : angular Acc, °/s^2
 *  @param dRuntime : Command execution exceeds this time, the movement will stop, s
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SpeedL(unsigned int boxID, unsigned int rbtID, double dXCmdVel,double dYCmdVel,double dZCmdVel,double dRxCmdVel,
                    double dRyCmdVel,double dRzCmdVel,double dLinearAcc,double dAngularAcc,double dRuntime);
                    
/**
 *	@brief: Initialize servoEsJ, truncating points 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_InitServoEsJ(unsigned int boxID, unsigned int rbtID);

/**
 *	@brief: Start move for servoEsJ 
 *	@param boxID : control box number, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dServoTime : interval of update (s) 
 *	@param dLookaheadTime : LookaheadTime (s) 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_StartServoEsJ(unsigned int boxID, unsigned int rbtID, double dServoTime, double dLookheadTime);

/**
 *	@brief: Push points list for servoEsJ 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nPointsSize: Point size, maximum 500 
 *	@param sPoints : Points list, separated by ","  
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_PushServoEsJ(unsigned int boxID, unsigned int rbtID, int nPointsSize, string sPoints);

/**
 *	@brief: Read servoEsJ state with call interval more than 20 ms 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nState: Pushing point is allowed or not; 0 for allowed, 1 for not allowed
 *	@param nIndex: Current point index reached 
 *	@param nCount: Points number finished 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadServoEsJState(unsigned int boxID, unsigned int rbtID, int& nState, int& nIndex, int& nCount);

/**
 *	@brief: Set motion parameters of tracking motion 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nState: 0 to disable tracking, 1 to enable tracking 
 *	@param dDistance: Relative distance of tracking 
 *	@param dAwayVelocity: Away velocity of tracking 
 *	@param dGobackVelocity: Back velocity of tracking 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetMoveTraceParams(unsigned int boxID, unsigned int rbtID, int nState, double dDistance, double dAwayVelocity, double dGobackVelocity);

/**
 *	@brief: Set initial parameters of tracking motion 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dK,dB: parameters in equation as y = dK * x + dB 
 *	@param dMaxLimit: Max distance detected by laser sensor 
 *	@param dMinLinit: Min distance detected by laser sensor 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetMoveTraceInitParams(unsigned int boxID, unsigned int rbtID, double dK, double dB, double dMaxLimit, double dMinLinit);

/**
 *	@brief: Set end orientation in tracking motion 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dX~Z: invalid, 0 recommended 
 *	@param dRx~Rz: End orientation in specified UCS through function HRIF_SetUCSByName 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetMoveTraceUcs(unsigned int boxID, unsigned int rbtID, double dX, double dY, double dZ, double dRx, double dRy, double dRz);

/**
 *	@brief: Set tracking motion state 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nState: 0 to disable tracking, 1 to enable tracking
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetTrackingState(unsigned int boxID, unsigned int rbtID, int nState);

/**
 *	@brief: Excute HRApp command 
 *	@param boxID : Control box ID, 0 as default 
 *	@param AppName: AppName
 *	@param sCmdName: Command name 
 *	@param sParams: Parameters list 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_HRAppCmd(unsigned int boxID,string AppName, string sCmdName, string sParams);

/**
 *	@brief: Excute HRApp command 
 *	@param boxID : Control box ID, 0 as default 
 *	@param AppName: AppName
 *	@param sCmdName: Command name 
 *	@param sParams: Parameters list 
 *	@param sResult: Result list 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_HRAppData(unsigned int boxID,string AppName, string sCmdName, string sParams, vector<string>& sResults);

/**
 *	@brief: Write End holding registers of Modbus slave 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nSlaveID: Modbus slave ID 
 *	@param nFunction: Function code 
 *	@param nRegAddr: Register beginning address 
 *	@param nRegCount: Registers number, maximum 11 
 *	@param vecData: Registers value, with the registers size of nRegCount 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_WriteEndHoldingRegisters(unsigned int boxID, unsigned int rbtID, int nSlaveID, int nFunction, int nRegAddr, int nRegCount, vector<int> vecData);

/**
 *	@brief: Read End holding registers of Modbus slave 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nSlaveID: Modbus slave ID 
 *	@param nFunction: Function code 
 *	@param nRegAddr: Register beginning address 
 *	@param nRegCount: Registers number, maximum 12 
 *	@param vecData: obtained registers value, with the registers size of nRegCount 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	
ROBOT_PRO_API int HRIF_ReadEndHoldingRegisters(unsigned int boxID, unsigned int rbtID, int nSlaveID, int nFunction, int nRegAddr, int nRegCount, vector<int>& vecData);

/**
 *	@brief: Set Modbus configuration for end connection 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nBaudrate: Baud rate, 115200 as default 
 *	@param nDataBit: Data bits, 8 as default 
 *	@param nParityBit: Parity bit (78: No parity; 79: Odd parity; 69: Even parity) 
 *	@param nStopBit: Stop bits, 1 as default 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	
ROBOT_PRO_API int HRIF_SetEndModbusInfo(unsigned int boxID, unsigned int rbtID, int nBaudrate, int nDataBit, int nParityBit, int nStopBit);

/**
 *	@brief: End-effector RS485 raw transaction (hex frame)
 *	@param boxID : Control box ID, 0 as default
 *	@param rbtID : Robot ID, 0 as default
 *	@param hexString : Hex payload string (e.g. Modbus RTU frame without spaces)
 *	@param timeoutMs : Transaction timeout in milliseconds
 *	@param result : Raw response string returned from end-effector 485 transaction
 *
 *	@return : nRet=0 : Function call succeeded
 *            nRet>0 : Error code of function call
 */
ROBOT_PRO_API int HRIF_EndRaw485Transact(unsigned int boxID, unsigned int rbtID, const char* hexString, int timeoutMs, string& result);

/**
 *	@brief: Set max velocity of tracking position  
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dMaxLineVel: Max follow position velocity, mm/s
 *	@param dMaxOriVel:  Max follow angular velocity, °/s
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	
ROBOT_PRO_API int HRIF_SetPoseTrackingMaxMotionLimit(unsigned int boxID, unsigned int rbtID,
                        double dMaxLineVel,double dMaxOriVel);

/**
 *	@brief: Set location tracking timeout stop time 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dTime: time out 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	
ROBOT_PRO_API int HRIF_SetPoseTrackingStopTimeOut(unsigned int boxID, unsigned int rbtID, double dTime);

/**
 *	@brief: Set PID of tracking position 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dPosPID1-dPosPID3: Follow position PID 
 *	@param dOriPID1-dOriPID3: Follow angular PID 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	
ROBOT_PRO_API int HRIF_SetPoseTrackingPIDParams(unsigned int boxID, unsigned int rbtID, 
                        double dPosPID1,double dPosPID2,double dPosPID3,
                        double dOriPID1,double dOriPID2,double dOriPID3);

/**
 *	@brief: Set target of tracking position 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dX-dRz: Target Position 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	
ROBOT_PRO_API int HRIF_SetPoseTrackingTargetPos(unsigned int boxID, unsigned int rbtID, 
                        double dX,double dY,double dZ,double dRx,double dRy,double dRz);

/**
 *	@brief: Set state of tracking position 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nState: 0: Disable tracking position
 *                 1: Enable tracking position
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	
ROBOT_PRO_API int HRIF_SetPoseTrackingState(unsigned int boxID, unsigned int rbtID, int nState);

/**
 *	@brief:Set real time position update of tracking position
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dX-dRz: Checking distance 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	
ROBOT_PRO_API int HRIF_SetUpdateTrackingPose(unsigned int boxID, unsigned int rbtID, 
                        double dX,double dY,double dZ,double dRx,double dRy,double dRz);

/**
 *	@brief: Confige TCP 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dX-dRz: TCP Position
 *                 
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	
ROBOT_PRO_API int HRIF_ConfigTCP(unsigned int boxID, unsigned int rbtID,
                                     string sTcpName,double dX, double dY, double dZ, double dRx, double dRy, double dRz);

/**
 *	@brief: Confige UCS 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dX-dRz: UCS Position 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	
ROBOT_PRO_API int HRIF_ConfigUCS(unsigned int boxID, unsigned int rbtID,
                                     string strName,double x,double y,double z,double rx,double ry,double rz);

/**
 *	@brief:Move to target pose 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dX-dRz: target pose 
 *	@param strTCPName: TCP name
 *	@param strUCSName: UCS name
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	
//ROBOT_PRO_API int HRIF_MoveLTo(unsigned int boxID, unsigned int nRbtID, 
//                                    double dX, double dY, double dZ, double dRx, double dRy, double dRz, string strTCPName, string strUCSName);
                                    
/**
 *	@brief: Move to target joint positions 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param J1-J6: target positions 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	
//ROBOT_PRO_API int HRIF_MoveJTo(unsigned int boxID, unsigned int nRbtID, 
//                                    double J1, double J2, double J3, double J4, double J5, double J6);

/**
 *	@brief: Move to safe space 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_MoveToSS(unsigned int boxID, unsigned int rbtID);

/**
 *	@brief: set the collision sensitivity level 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nSafeLevel : sensitivity level, 0-5, 0 for the highest, 5 for the lowest 
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetCollideLevel(unsigned int boxID, unsigned int rbtID,int nSafeLevel);

/**
 *	@brief: read the collision sensitivity level 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nSafeLevel : sensitivity level, 0-5, 0 for the highest, 5 for the lowest 
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_GetCollideLevel(unsigned int boxID, unsigned int rbtID,int &nSafeLevel);

/**
 *	@brief:Set EtherCatAO
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nIndex: AO
 *  @param dValue: Analog value
 *
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetECATAO(unsigned int boxID, unsigned int rbtID, int nIndex, double dValue);


/**
 *	@brief:Add a new safe plane
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *  @param Name: plane name
 *  @param UcsName:UCS Name
 *  @param Mode: Mode
 *  @param Display: Display
 *  @param Switch: Enable
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_AddSafePlane(unsigned int boxID, unsigned int rbtID,  string Name, string UcsName , int Mode, int Display, int Switch);

/**
 *	@brief:Delete Safe Plane
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *  @param Name: plane name
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_DelSafePlane(unsigned int boxID, unsigned int rbtID,  string Name);


/**
 *	@brief:Get SafePlane List 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *  @param BordersName: plane name
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadSafePlaneList(unsigned int boxID, unsigned int rbtID,  vector<string>  &BordersName);

/**
 *	@brief:Get SafePlane List 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *  @param BordersName: plane name
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadSafetyPlaneList(unsigned int boxID, unsigned int rbtID,  vector<string>  &BordersName);

/**
 *	@brief:Read SafePlane Info
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *  @param Name: plane name
 *  @param UcsName:UCS Name
 *  @param Mode: Mode
 *  @param Display: Display
 *  @param Switch: Enable
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadSafePlane(unsigned int boxID, unsigned int rbtID,  string Name, string &UcsName , int &Mode, int &Display, int &Switch);

/**
 *	@brief:Read SafePlane Info
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *  @param Name: plane name
 *  @param Ucs_X - Ucs_Rz: UCS Data
 *  @param Mode: Mode
 *  @param Display: Display
 *  @param Switch: Enable
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadSafetyPlane(unsigned int boxID, unsigned int rbtID,  string Name, double &UCS_X, double &UCS_Y,double &UCS_Z,double &UCS_Rx, double &UCS_Ry, double &UCS_Rz, int &Mode, int &Display, int &Switch);

/**
 *	@brief:Update SafePlane Info
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *  @param Name: plane name
 *  @param UcsName:UCS Name
 *  @param Mode: Mode
 *  @param Display: Display
 *  @param Switch: Enable
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_UpdateSafePlane(unsigned int boxID, unsigned int rbtID,  string Name, string UcsName , int Mode, int Display, int Switch);

/**
 *	@brief:Set the distance threshold when the virtual wall begins to generate damping 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *  @param dDepth: Threshold size
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetDepthThresholdForDampingArea(unsigned int boxID, unsigned int rbtID, double dDepth);



//************************************************************************/
//**    Interfaces for Extended Axis 
//************************************************************************/

struct JointsData
{
    double dJoint[MaxJointNums]={0};
};

/**
 *	@brief: Set joint max velocity 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param joints : Joint1~n max velocity, °/s
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetJointMaxVel_nJ(unsigned int boxID, unsigned int rbtID, JointsData joints);


/**
 *	@brief: Set joint max acceleration 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param joints : Joint1~n max acceleration
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetJointMaxAcc_nJ(unsigned int boxID, unsigned int rbtID, JointsData joints);


/**
 *	@brief: Set max range of joint motion 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param jdMaxJoints : Max joint angle 
 *	@param jdMinJoints : Min joint angle 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	
ROBOT_PRO_API int HRIF_SetMaxAcsRange_nJ(unsigned int boxID, unsigned int rbtID, JointsData jdMaxJoints, JointsData jdMinJoints);


/**
 *	@brief: Read command joint positions 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param joints : Joint command positions 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadCmdJointPos_nJ(unsigned int boxID, unsigned int rbtID, JointsData& joints);

/**
 *	@brief: Read command current of joint 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param joints : Joint command current 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadCmdJointCur_nJ(unsigned int boxID, unsigned int rbtID, JointsData& joints);

/**
 *	@brief: Read actual current of joint 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param joints : Joint actual current 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadActJointCur_nJ(unsigned int boxID, unsigned int rbtID, JointsData& joints);

/**
 *	@brief: Read actual joint positions 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param joints : Joint actual positions 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadActJointPos_nJ(unsigned int boxID, unsigned int rbtID, JointsData& joints);

/**
 *	@brief: Read command velocity of joint 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param joints : Joint command velocity 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadCmdJointVel_nJ(unsigned int boxID, unsigned int rbtID, JointsData& joints);

/**
 *	@brief: Read actual velocity of joint 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param joints : Joint actual velocity 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadActJointVel_nJ(unsigned int boxID, unsigned int rbtID, JointsData& joints);

/**
 *	@brief: Inverse kinematics transformation from pose to joint positions 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dCoord_X~Rz : pose in specified UCS and TCP 
 *	@param dTcp_X~Rz : TCP pose
 *	@param dUcs_X~Rz : UCS pose
 *	@param joints : Reference joint positions used for the selection in multiple solutions 
 *	@param dTargetJoints : Inverse solution of J1~Jn 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_GetInverseKin_nJ(unsigned int boxID, unsigned int rbtID, double dCoord_X, double dCoord_Y, double dCoord_Z, double dCoord_Rx, double dCoord_Ry, double dCoord_Rz,
                 double dTcp_X, double dTcp_Y, double dTcp_Z, double dTcp_Rx, double dTcp_Ry, double dTcp_Rz,
                 double dUcs_X, double dUcs_Y, double dUcs_Z, double dUcs_Rx, double dUcs_Ry, double dUcs_Rz,
                 JointsData joints,
				 JointsData &dTargetJoints);

/**
 *	@brief: Forward kinematics transformation from joint positions to pose 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param joints : joint positions 
 *	@param dTcp_X~Rz : TCP pose for the target
 *	@param dUcs_X~Rz : UCS pose for the target 
 *	@param dTarget_X~Rz : pose in specified UCS and TCP 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */				 
ROBOT_PRO_API int HRIF_GetForwardKin_nJ(unsigned int boxID, unsigned int rbtID, JointsData joints,
                 double dTcp_X, double dTcp_Y, double dTcp_Z, double dTcp_Rx, double dTcp_Ry, double dTcp_Rz,
                 double dUcs_X, double dUcs_Y, double dUcs_Z, double dUcs_Rx, double dUcs_Ry, double dUcs_Rz,
                 double &dTarget_X, double &dTarget_Y, double &dTarget_Z, double &dTarget_Rx, double &dTarget_Ry, double &dTarget_Rz);

/**
 *	@brief: Start relative wayPoint 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nType : Move type, 0 for joint, 1 for linear 
 *  @param nPointList : Using point saved in points list or not, 0 for not using,  1 for using 
 *  @param dPos_X~Rz : Target pose 
 *  @param dJointsPos : Target joint positions 
 *  @param nrelMoveType : Relative move type, 0 for absolute value, 1 for superimposed value 
 *  @param nAxisMask : Is Axis/Direction Moving, 0 for not moving, 1 for moving 
 *  @param dTarget : Moving distance; invalid when nAxisMask equals 0; joint move an absolute or superimposed distance when nAxisMask equals 1 and nType equals 0; Pose move an absolute or superimposed distance when nAxisMask equals 1 and nType equals 1. 
 *	@param sTcpName : Target TCP name, invalid when IsUseJoint equals 1, "TCP" is valid as default 
 *	@param sUcsName : Target UCS name, invalid when IsUseJoint equals 1, "Base" is valid as default 
 *  @param dVelocity: Velocity (mm/s) or (°/s) 
 *  @param dAcc: Acceleration (mm/s^2) or (°/s^2) 
 *  @param dRadius: Blending radius, mm 
 *	@param nIsUseJoint : Using joint or not, usable when MoveType equals 0, 0 for not using joint, 1 for using joint 
 *	@param nIsSeek,nIOBit,nIOState:Seeking or not, 1 for seeking; Motion stops when DO-nIOBit equals nIoState 
 *	@param strCmdID:Command ID, waypoint ID also, customized or set to 1, 2, 3 in order 
 *
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_WayPointRel_nJ(unsigned int boxID, unsigned int rbtID,
    int nType,
    int nPointList,
    double dPos_X, double dPos_Y, double dPos_Z, double dPos_Rx, double dPos_Ry, double dPos_Rz,
    JointsData dJointsPos,
    int nrelMoveType,
    JointsData nAxisMask,
    JointsData dTarget,
    string sTcpName, string sUcsName,
    double dVelocity, double dAcc, double dRadius,
    int nIsUseJoint, int nIsSeek, int nIOBit, int nIOState, string strCmdID);

/**
 *	@brief: Start joint move. HRIF_WayPoint is more recommended yet 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dX~Rz : Target pose, invalid when IsUseJoint equals 1. Target joint positions is obtained through inversely solving from this pose when IsUseJoint equals 0 
 *	@param joints : Target joint positions when IsUseJoint equals 1; Reference joint positions for the inversely solving when IsUseJoint equals 0. 
 *	@param sTcpName : Target TCP name, invalid when IsUseJoint equals 1, "TCP" is valid as default 
 *	@param sUcsName : Target UCS name, invalid when IsUseJoint equals 1, "Base" is valid as default 
 *	@param dVelocity : Motion speed, mm/s or °/s 
 *	@param dAcc : Motion acceleration, mm/(s^2) or °/(s^2) 
 *	@param dRadius : Blending radius, mm 
 *	@param nIsUseJoint : Using joint or not, usable when MoveType equals 0, 0 for not using joint, 1 for using joint 
 *	@param nIsSeek,nIOBit,nIOState:Seeking or not, 1 for seeking; Motion stops when DO-nIOBit equals nIoState 
 *	@param strCmdID:Command ID, waypoint ID also, customized or set to 1, 2, 3 in order 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	
ROBOT_PRO_API int HRIF_MoveJ_nJ(unsigned int boxID, unsigned int rbtID, double dX, double dY, double dZ, double dRx, double dRy, double dRz,
                JointsData joints,
                string sTcpName, string sUcsName, double dVelocity, double dAcc, double dRadius, 
                int nIsUseJoint,int nIsSeek, int nIOBit, int nIOState, string strCmdID);

/**
 *	@brief: Start linear move. HRIF_WayPoint is more recommended yet 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dX~Rz : Target pose 
 *	@param joints : Reference joint positions near to the target pose 
 *	@param sTcpName : TCP name 
 *	@param sUcsName : UCS name 
 *	@param dVelocity : Motion speed, mm/s or °/s 
 *	@param dAcc : Motion acceleration, mm/(s^2) or °/(s^2) 
 *	@param dRadius :Blending radius, mm 
 *	@param nIsSeek,nIOBit,nIOState:Seeking or not, 1 for seeking; Motion stops when DO-nIOBit equals nIoState 
 *	@param strCmdID:Command ID, waypoint ID also, customized or set to 1, 2, 3 in order 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	
ROBOT_PRO_API int HRIF_MoveL_nJ(unsigned int boxID, unsigned int rbtID, double dX, double dY, double dZ, double dRx, double dRy, double dRz,
                JointsData joints,
                string sTcpName, string sUcsName, double dVelocity, double dAcc, double dRadius,
                int nIsSeek, int nIOBit, int nIOState, string strCmdID);

/**
 *	@brief: Start linear move. HRIF_WayPoint is more recommended yet
 *	@param boxID : Control box ID, 0 as default
 *	@param rbtID : Robot ID, 0 as default
 *	@param dX~Rz : Target pose
 *	@param joints : Reference joint positions near to the target pose
 *	@param sTcpName : TCP name
 *	@param sUcsName : UCS name
 *	@param dVelocity : Motion speed, mm/s or °/s
 *	@param dAcc : Motion acceleration, mm/(s^2) or °/(s^2)
 *	@param dRadius :Blending radius, mm
 *	@param nIsSeek,nIOBit,nIOState：Seeking or not, 1 for seeking; Motion stops when DO-nIOBit equals nIoState
 *	@param strCmdID：Command ID, waypoint ID also, customized or set to 1, 2, 3 in order
 *  @param lockedFlags : Joint locking flags (bitmask). Bits 6-9 represent external axes J7-J10. Set bit to 1 to lock the corresponding axis, 0 to unlock.
 *                          Example values: Lock J7: 0x40 (64 in decimal),Lock J7 and J8: 0xC0 (192 in decimal),Valid range: The joints that can be locked need to be determined by the specific controller version.
 *	@return : nRet=0 : Function call succeeded
 *            nRet>0 : Error code of function call
 */	
ROBOT_PRO_API int HRIF_MoveLByLockAxis_nJ(unsigned int boxID, unsigned int rbtID, double dX, double dY, double dZ, double dRx, double dRy, double dRz,
                JointsData joints,
                string sTcpName, string sUcsName, double dVelocity, double dAcc, double dRadius,
                int nIsSeek, int nIOBit, int nIOState, string strCmdID,int lockedFlags);

/**
 *	@brief: Start round move
 *	@param boxID : Control box ID, 0 as default
 *	@param rbtID : Robot ID, 0 as default
 *	@param dStartPos_X~Rz : Start pose
 *	@param dAuxPos_X~Rz : Via pose
 *	@param dEndPos_X~Rz : End pose
 *	@param nFixedPosure : 0 for unfixed orientation, 1 for fixed orientation, 2 for newly added gradual orientation arc motion.
 *	@param nMoveCType : 1 for arc, 0 for circle
 *	@param dRadLen : Invalid when nMoveCType equals 1; Circles when nMoveCType equals 0.
 *	@param dVelocity : Motion speed, mm/s
 *	@param dAcc : Motion acceleration, mm/(s^2)
 *	@param dRadius : Blending radius, mm
 *	@param sTcpName : TCP name
 *	@param sUcsName : UCS name
 *	@param sCmdID : Command ID
 *  @param joints : The reference joint position of the end point
 *  @param nIsUseCurRefACS : Whether to use the reference joint Angle
 *	
 *	@return : nRet=0 : Function call succeeded
 *            nRet>0 : Error code of function call
 */	
ROBOT_PRO_API int HRIF_MoveC_nJ(unsigned int boxID, unsigned int rbtID,
                double dStartPos_X,double dStartPos_Y,double dStartPos_Z,double dStartPos_Rx,double dStartPos_Ry,double dStartPos_Rz,
                double dAuxPos_X,double dAuxPos_Y,double dAuxPos_Z,double dAuxPos_Rx,double dAuxPos_Ry,double dAuxPos_Rz,
                double dEndPos_X,double dEndPos_Y,double dEndPos_Z,double dEndPos_Rx,double dEndPos_Ry,double dEndPos_Rz,
                int nFixedPosure,int nMoveCType,double dRadLen,double dVelocity, double dAcc, 
                double dRadius, string sTcpName, string sUcsName, string sCmdID, JointsData joints,int nIsUseCurRefACS = 0);


/**
 *	@brief: Start round move
 *	@param boxID : Control box ID, 0 as default
 *	@param rbtID : Robot ID, 0 as default
 *	@param dAuxPos_X~Rz : Via pose
 *	@param dEndPos_X~Rz : End pose
 *  @param joints : The reference joint position of the end point
 *	@param nFixedPosure : 0 for Not fixed posture and posture angle change less than 180 °, 1 for fixed orientation, 2 for Unsteady posture and posture angle change greater than 180 °
 *	@param dRadLen : When the arc length is 0, it means that the target pose is determined by endPose. If set to 360 °, it can run a full circle, and if set to 720 °, it can run two circles
 *	@param dVelocity : Motion speed, mm/s
 *	@param dAcc : Motion acceleration, mm/(s^2)
 *	@param dRadius : Blending radius, mm
 *	@param sTcpName : TCP name
 *	@param sUcsName : UCS name
 *	@param sCmdID : Command ID
 *	
 *	@return : nRet=0 : Function call succeeded
 *            nRet>0 : Error code of function call
 */	
ROBOT_PRO_API int HRIF_MoveC2_nJ(unsigned int boxID, unsigned int rbtID,
                double dAuxPos_X,double dAuxPos_Y,double dAuxPos_Z,double dAuxPos_Rx,double dAuxPos_Ry,double dAuxPos_Rz,
                double dEndPos_X,double dEndPos_Y,double dEndPos_Z,double dEndPos_Rx,double dEndPos_Ry,double dEndPos_Rz,
                JointsData joint,
                int nFixedPosure,double dRadLen,double dVelocity, double dAcc, 
                double dRadius, string sTcpName, string sUcsName, string sCmdID);


/**
 *	@brief: Start round move
 *	@param boxID : Control box ID, 0 as default
 *	@param rbtID : Robot ID, 0 as default
 *	@param dStartPos_X~Rz : Start pose
 *	@param dAuxPos_X~Rz : Via pose
 *	@param dEndPos_X~Rz : End pose
 *	@param nFixedPosure : 0 for unfixed orientation , 1 for fixed orientation
 *	@param nMoveCType : 1 for arc, 0 for circle
 *	@param dRadLen : Invalid when nMoveCType equals 1; Circles when nMoveCType equals 0. 
 *	@param dVelocity : Motion speed, mm/s
 *	@param dAcc : Motion acceleration, mm/(s^2)
 *	@param dRadius : Blending radius, mm
 *	@param sTcpName : TCP name
 *	@param sUcsName : UCS name
 *	@param sCmdID : Command ID
 *  @param joints : The reference joint position of the end point
 *  @param lockedFlags : Joint locking flags (bitmask). Bits 6-9 represent external axes J7-J10. Set bit to 1 to lock the corresponding axis, 0 to unlock.
 *                          Example values: Lock J7: 0x40 (64 in decimal),Lock J7 and J8: 0xC0 (192 in decimal),Valid range: The joints that can be locked need to be determined by the specific controller version.
 *  @param nIsUseCurRefACS : Whether to use the reference joint Angle
 *	
 *	@return : nRet=0 : Function call succeeded / 函数调用成功
 *            nRet>0 : Error code of function call / 函数调用失败的错误码
 */	
ROBOT_PRO_API int HRIF_MoveCByLockAxis_nJ(unsigned int boxID, unsigned int rbtID,
                double dStartPos_X,double dStartPos_Y,double dStartPos_Z,double dStartPos_Rx,double dStartPos_Ry,double dStartPos_Rz,
                double dAuxPos_X,double dAuxPos_Y,double dAuxPos_Z,double dAuxPos_Rx,double dAuxPos_Ry,double dAuxPos_Rz,
                double dEndPos_X,double dEndPos_Y,double dEndPos_Z,double dEndPos_Rx,double dEndPos_Ry,double dEndPos_Rz,
                int nFixedPosure,int nMoveCType,double dRadLen,double dVelocity, double dAcc, 
                double dRadius, string sTcpName, string sUcsName, string sCmdID, JointsData joints, int lockedFlags, int nIsUseCurRefACS = 0);

/**
 *	@brief: Read joint max velocity 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param joints : Max velocity of J1~Jn
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadJointMaxVel_nJ(unsigned int boxID, unsigned int rbtID, JointsData &joints);

/**
 *	@brief: Read joint max jerk 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param joints : Max jerk of J1~Jn
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadJointMaxJerk_nJ(unsigned int boxID, unsigned int rbtID, JointsData &joints);

/**
 *	@brief: Read joint max acceleration 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param joints : Max acceleration of J1~Jn
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadJointMaxAcc_nJ(unsigned int boxID, unsigned int rbtID, JointsData &joints);

/**
 *	@brief: Read point pose, joint positions, UCS and TCP by point name 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param joints : joint positions 
 *	@param dX~Rz : pose 
 *	@param dTcp_X~Rz : TCP pose 
 *	@param dUcs_X~Rz : UCS pose 
 *
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadPointByName_nJ(unsigned int boxID, unsigned int rbtID, string strPointName,
    JointsData &joints,
    double& dX, double& dY, double& dZ, double& dRx, double& dRy, double& dRz,
    double& dTcp_X, double& dTcp_Y, double& dTcp_Z, double& dTcp_Rx, double& dTcp_Ry, double& dTcp_Rz,
    double& dUcs_X, double& dUcs_Y, double& dUcs_Z, double& dUcs_Rx, double& dUcs_Ry, double& dUcs_Rz);

/**
 *	@brief: Read actual pose and joint positions 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param dX~Rz : pose 
 *	@param joints: joint positions 
 *	@param dTcp_X~Rz : TCP pose 
 *	@param dUcs_X~Rz : UCS pose 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadActPos_nJ(unsigned int boxID, unsigned int rbtID,
                    double &dX, double &dY, double &dZ, double &dRx, double &dRy, double &dRz,
                    JointsData &joints,
                    double &dTcp_X, double &dTcp_Y, double &dTcp_Z, double &dTcp_Rx, double &dTcp_Ry, double &dTcp_Rz,
                    double &dUcs_X, double &dUcs_Y, double &dUcs_Z, double &dUcs_Rx, double &dUcs_Ry, double &dUcs_Rz);

/**
 *	@brief: Start waypoint move. 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nMoveType : Move type, 0 for MoveJ, 1 for MoveL 
 *	@param dX~Rz : Target pose, invalid when IsUseJoint equals 1. Target joint positions is obtained through inversely solving from this pose when IsUseJoint equals 0 
 *	@param joints : Target joint positions when IsUseJoint equals 1; Reference joint positions for the inversely solving when IsUseJoint equals 0. 
 *	@param dTcp_X~Rz : Target TCP, invalid when IsUseJoint equals 1 
 *	@param dUcs_X~Rz : Target UCS, invalid when IsUseJoint equals 1 
 *	@param dVelocity : Motion speed, mm/s or °/s 
 *	@param dAcc : Motion acceleration, mm/(s^2) or °/(s^2) 
 *	@param dRadius : Blending radius, mm 
 *	@param nIsUseJoint : Using joint or not, usable when MoveType equals 0, 0 for not using joint, 1 for using joint 
 *	@param nIsSeek,nIOBit,nIOState:Seeking or not, 1 for seeking; Motion stops when DO-nIOBit equals nIoState 
 *	@param strCmdID:Command ID, waypoint ID also, customized or set to 1, 2, 3 in order 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	
ROBOT_PRO_API int HRIF_WayPointEx_nJ(unsigned int boxID, unsigned int rbtID, int nMoveType,
                    double dX, double dY, double dZ, double dRx, double dRy, double dRz,                        
                    JointsData joints,
                    double dTcp_X, double dTcp_Y, double dTcp_Z, double dTcp_Rx, double dTcp_Ry, double dTcp_Rz,
                    double dUcs_X, double dUcs_Y, double dUcs_Z, double dUcs_Rx, double dUcs_Ry, double dUcs_Rz,
                    double dVelocity, double dAcc, double dRadius, int nIsUseJoint, int nIsSeek, int nIOBit, int nIOState, string strCmdID);

/**
 *	@brief: Start waypoint move. 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nMoveType : Move type, 0 for MoveJ, 1 for MoveL, 2 for MoveC 
 *	@param dEndPos_X~Rz : Target pose, invalid when IsUseJoint equals 1. Target pose is obtained through inverse kinematics from this pose when IsUseJoint equals 0 
*	@param dAuxPos_X~Rz : Target auxiliary pose invalid when IsUseJoint equals 1, used as the middle point when nMoveType equals 2. Target pose is obtained through inverse kinematics from this pose when IsUseJoint equals 0 
 *	@param joints : Target joint positions when IsUseJoint equals 1; Reference joint positions for the inverse kinematics when IsUseJoint equals 0. 
 *	@param sTcpName : Target TCP name, invalid when IsUseJoint equals 1, "TCP" is valid as default 
 *	@param sUcsName : Target UCS name, invalid when IsUseJoint equals 1, "Base" is valid as default 
 *	@param dVelocity : Motion speed, mm/s or °/s 
 *	@param dAcc : Motion acceleration, mm/(s^2) or °/(s^2) 
 *	@param dRadius : Blending radius, mm 
 *	@param nIsUseJoint : Using joint or not, usable when MoveType equals 0, 0 for not using joint, 1 for using joint 
 *	@param nIsSeek,nIOBit,nIOState:Seeking or not, 1 for seeking; Motion stops when DO-nIOBit equals nIoState 
 *	@param strCmdID:Command ID, waypoint ID also, customized or set to 1, 2, 3 in order 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	
ROBOT_PRO_API int HRIF_WayPoint2_nJ(unsigned int boxID, unsigned int rbtID, int nMoveType,
                double dEndPos_X, double dEndPos_Y, double dEndPos_Z, double dEndPos_Rx, double dEndPos_Ry, double dEndPos_Rz,   
                double dAuxPos_X, double dAuxPos_Y, double dAuxPos_Z, double dAuxPos_Rx, double dAuxPos_Ry, double dAuxPos_Rz,                       
                JointsData joints,
                string sTcpName, string sUcsName, double dVelocity, double dAcc, double dRadius, 
                int nIsUseJoint,int nIsSeek, int nIOBit, int nIOState, string strCmdID);

/**
 *	@brief: Start waypoint move.
 *	@param boxID : Control box ID, 0 as default
 *	@param rbtID : Robot ID, 0 as default
 *	@param nMoveType : Move type, 0 for MoveJ, 1 for MoveL, 2 for MoveC
 *	@param dEndPos_X~Rz : Target pose, invalid when IsUseJoint equals 1. Target pose is obtained through inverse kinematics from this pose when IsUseJoint equals 0
*	@param dAuxPos_X~Rz : Target auxiliary pose invalid when IsUseJoint equals 1, used as the middle point when nMoveType equals 2. Target pose is obtained through inverse kinematics from this pose when IsUseJoint equals 0
 *	@param joints : Target joint positions when IsUseJoint equals 1; Reference joint positions for the inverse kinematics when IsUseJoint equals 0.
 *	@param sTcpName : Target TCP name, invalid when IsUseJoint equals 1, "TCP" is valid as default
 *	@param sUcsName : Target UCS name, invalid when IsUseJoint equals 1, "Base" is valid as default
 *	@param dVelocity : Motion speed, mm/s or °/s
 *	@param dAcc : Motion acceleration, mm/(s^2) or °/(s^2)
 *	@param dRadius : Blending radius, mm
 *	@param nIsUseJoint : Using joint or not, usable when MoveType equals 0, 0 for not using joint, 1 for using joint
 *	@param nIsSeek,nIOBit,nIOState：Seeking or not, 1 for seeking; Motion stops when DO-nIOBit equals nIoStat
 *	@param strCmdID：Command ID, waypoint ID also, customized or set to 1, 2, 3 in order
 *  @param lockedFlags : Joint locking flags (bitmask). Bits 6-9 represent external axes J7-J10. Set bit to 1 to lock the corresponding axis, 0 to unlock.
 *                          Example values: Lock J7: 0x40 (64 in decimal),Lock J7 and J8: 0xC0 (192 in decimal),Valid range: The joints that can be locked need to be determined by the specific controller version.
 *	@return : nRet=0 : Function call succeeded
 *            nRet>0 : Error code of function call
 */	
ROBOT_PRO_API int HRIF_WayPoint2ByLockAxis_nJ(unsigned int boxID, unsigned int rbtID, int nMoveType,
                double dEndPos_X, double dEndPos_Y, double dEndPos_Z, double dEndPos_Rx, double dEndPos_Ry, double dEndPos_Rz,   
                double dAuxPos_X, double dAuxPos_Y, double dAuxPos_Z, double dAuxPos_Rx, double dAuxPos_Ry, double dAuxPos_Rz,                       
                JointsData joints,
                string sTcpName, string sUcsName, double dVelocity, double dAcc, double dRadius, 
                int nIsUseJoint,int nIsSeek, int nIOBit, int nIOState, string strCmdID, int lockedFlags);

/**
 *	@brief: Start waypoint move. 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nMoveType : Move type, 0 for MoveJ, 1 for MoveL 
 *	@param dX~Rz : Target pose, invalid when IsUseJoint equals 1. Target joint positions is obtained through inversely solving from this pose when IsUseJoint equals 0 
 *	@param joints : Target joint positions when IsUseJoint equals 1; Reference joint positions for the inversely solving when IsUseJoint equals 0. 
 *	@param sTcpName : Target TCP name, invalid when IsUseJoint equals 1, "TCP" is valid as default 
 *	@param sUcsName : Target UCS name, invalid when IsUseJoint equals 1, "Base" is valid as default 
 *	@param dVelocity : Motion speed, mm/s or °/s 
 *	@param dAcc : Motion acceleration, mm/(s^2) or °/(s^2) 
 *	@param dRadius : Blending radius, mm 
 *	@param nIsUseJoint : Using joint or not, usable when MoveType equals 0, 0 for not using joint, 1 for using joint 
 *	@param nIsSeek,nIOBit,nIOState:Seeking or not, 1 for seeking; Motion stops when DO-nIOBit equals nIoState 
 *	@param strCmdID:Command ID, waypoint ID also, customized or set to 1, 2, 3 in order 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */	
ROBOT_PRO_API int HRIF_WayPoint_nJ(unsigned int boxID, unsigned int rbtID, int nMoveType,
                double dX, double dY, double dZ, double dRx, double dRy, double dRz,
                JointsData joints,
                string sTcpName, string sUcsName, double dVelocity, double dAcc, double dRadius, 
                int nIsUseJoint,int nIsSeek, int nIOBit, int nIOState, string strCmdID);

/**
 *	@brief: Read axis error code 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nErrorCode : Error code 
 *	@param Params : The error code of J1~Jn, 0 if no error 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadAxisErrorCode_nJ(unsigned int boxID, unsigned int rbtID, int& nErrorCode, JointsData &nAxisErrorCode);

/**
 *	@brief: Set default TCP (Tool Center Point) 
 *	@param boxID : Control box ID, 0 by default 
 *	@param rbtID : Robot ID, 0 by default 
 *	@param name : Name of the TCP to be set as default 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Function call error code 
 */
ROBOT_PRO_API int HRIF_SetDftTCP(unsigned int boxID, unsigned int rbtID,string name);

/**
 *	@brief: Calculate TCP orientation 
 *	@param boxID : Control box ID, 0 by default 
 *	@param rbtID : Robot ID, 0 by default 
 *	@param dUCS_X-dUCS_RZ : Coordinates and rotation angles in user coordinate system 
 *	@param dTCP_X-dTCP_RZ : Coordinates and rotation angles of TCP
 *	@param RX-RZ : Calculated rotation angles  
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Function call error code 
 */
ROBOT_PRO_API int HRIF_CalTCPOrt(unsigned int boxID, unsigned int rbtID, double dUCS_X,double dUCS_Y,double dUCS_Z,double dUCS_RX,double dUCS_RY,double dUCS_RZ,double dTCP_X,double dTCP_Y,double dTCP_Z,double dTCP_RX,double dTCP_RY,double dTCP_RZ,double &RX,double&RY,double &RZ);

/**
 *	@brief: Read current coordinates of the robot arm 
 *	@details: Get joint coordinates and user coordinates of the robot arm in current state by inputting TCP and UCS names 
 *	@param boxID : Control box ID, 0 by default 
 *	@param rbtID : Robot ID, 0 by default 
 *	@param UcsName : Name of the user coordinate system 
 *	@param TcpName : Name of the tool center point 
 *	@param dJ1-dJ6 : Current joint coordinates 
 *	@param dX-dRz : Current user coordinates 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Function call error code 
 */
ROBOT_PRO_API int HRIF_ReadActCoord(unsigned int boxID, unsigned int rbtID, string UcsName,string TcpName,double &dJ1,double &dJ2,double &dJ3,double &dJ4,double &dJ5,double &dJ6,double &dX,double &dY,double &dZ,double &dRx,double &dRy,double &dRz);

/**
 *	@brief: Read current coordinates of the robot arm (with joint data structure) 
 *	@details: Get joint coordinates and user coordinates of the robot arm in current state by inputting TCP and UCS names 
 *	@param boxID : Control box ID, 0 by default 
 *	@param rbtID : Robot ID, 0 by default 
 *	@param UcsName : Name of the user coordinate system 
 *	@param TcpName : Name of the tool center point 
 *	@param joints : Structure storing current joint coordinates 
 *	@param dX-dRz : Current user coordinates  
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Function call error code 
 */
ROBOT_PRO_API int HRIF_ReadActCoord_nJ(unsigned int boxID, unsigned int rbtID, string UcsName,string TcpName,JointsData& joints,double &dX,double &dY,double &dZ,double &dRx,double &dRy,double &dRz);



/**
 *	@brief: Clear the linkage extension axis position (set the current position of the linkage extension axis to zero point) 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ClearCoControlExpandAxisPos(unsigned int boxID, unsigned int rbtID);

/**
 *	@brief: Set the current base mounting angle. 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nRotation : Set base rotation angle
 *  @param nTilt: Set base tilt angle
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetBaseInstallingAngle(unsigned int boxID, unsigned int rbtID,int nRotation,int nTilt);

/**
 *	@brief: Get the current base mounting angle 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	@param nRotation : Get base rotation angle 
 *  @param nTilt: Get base tilt angle 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_GetBaseInstallingAngle(unsigned int boxID, unsigned int rbtID,int &nRotation,int &nTilt);

/**
 *	@brief: Start Load Identification. 
 *	@param boxID : Control box ID, 0 as default 
 *	@param rbtID : Robot ID, 0 as default 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_LoadIdentify(unsigned int boxID,unsigned int rbtID);

/**
 * @brief Get the load identification result.
 *
 * @param boxID : Control box ID, 0 as default 
 * @param rbtID : Robot ID, 0 as default 
 * @param status Load identification status:
 *               - 0: Success
 *               - 1: First trajectory error
 *               - 2: Second trajectory error
 *               - 3: Third trajectory error
 *               - 4: Load identification in progress
 *               - 5: Unexpected stop
 *               - 6: Unexpected exit
 *               - 7: Initialization, load identification not started
 * @param progress: Progress status (0~100)(Current load identification progress)
 *                 
 * @param mass : Identified load mass (unit: kg).
 * @param cx :Load center of mass X coordinate.
 * @param cy :Load center of mass Y coordinate.
 * @param cz : Load center of mass Z coordinate.
 */
ROBOT_PRO_API int HRIF_GetLoadIdentifyResult(unsigned int boxID, unsigned int rbtID,int &status,int &progress,double &mass,double &cx,double &cy,double &cz);

/**
 * @brief Get calibration parameters
 * @param boxID : Control box ID, 0 as default 
 * @param rbtID : Robot ID, 0 as default 
 * @param dCalib_Fx Get calibration force in x-direction 
 * @param dCalib_Fy Get calibration force in y-direction 
 * @param dCalib_Fz Get calibration force in z-direction 
 * @param dCalib_Mx Get calibration moment in x-direction 
 * @param dCalib_My Get calibration moment in y-direction 
 * @param dCalib_Mz Get calibration moment in z-direction 
 * @param dCalib_G Get gravity 
 * @param dCalib_X Get center of gravity offset in x-direction 
 * @param dCalib_Y Get center of gravity offset in y-direction
 * @param dCalib_Z Get center of gravity offset in z-direction
 * @param dRotateAngle Get current base mounting Installation angle 
 * @return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
*/
ROBOT_PRO_API int HRIF_GetLastCalibParams(unsigned int boxID, unsigned int rbtID, double &dCalib_Fx,
    double &dCalib_Fy,
    double &dCalib_Fz,
    double &dCalib_Mx,
    double &dCalib_My,
    double &dCalib_Mz,
    double &dCalib_G,
    double &dCalib_X,
    double &dCalib_Y,
    double &dCalib_Z,
    double &dRotateAngle);

/**
 * @brief Set calibration parameters 
 * @param boxID :Control box ID, 0 as default 
 * @param rbtID : Robot ID, 0 as default 
 * @param dCalib_Fx Set calibration force in x-direction 
 * @param dCalib_Fy Set calibration force in y-direction 
 * @param dCalib_Fz Set calibration force in z-direction 
 * @param dCalib_Mx Set calibration moment in x-direction 
 * @param dCalib_My Set calibration moment in y-direction 
 * @param dCalib_Mz Set calibration moment in z-direction 
 * @param dCalib_G Set gravity 
 * @param dCalib_X Set center of gravity offset in x-direction 
 * @param dCalib_Y Set center of gravity offset in y-direction
 * @param dCalib_Z Set center of gravity offset in z-direction
 * @param dRotateAngle Set current base mounting Installation angle 
 * @return : nRet=0 : Function call succeeded 
 *           nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_SetInitializeForceSensor(unsigned int boxID, unsigned int rbtID,double dCalib_Fx,
        double dCalib_Fy,
        double dCalib_Fz,
        double dCalib_Mx,
        double dCalib_My,
        double dCalib_Mz,
        double dCalib_G,
        double dCalib_X,
        double dCalib_Y,
        double dCalib_Z,
        double dRotateAngle);
    

/**
 * @brief  Set the calibration data for the force sensor.
 *
 * @param boxID :Control box ID, 0 as default 
 * @param rbtID : Robot ID, 0 as default 
 * @param pointnum:  Number of calibration points
 *                
 * @param param Point data for force control calibration. Total number of parameters must be a multiple of 12 and between 96 and 192.
 *              The parameters are stored in the order of X, Y, Z, Rx, Ry, Rz, Fx, Fy, Fz, Tx, Ty, Tz.
 *              Each group contains 12 parameters, supporting 8~16 groups (i.e., 96~192 parameters).
 *              The first six are position coordinates, and the last six are force control data (unit: mm, N).
 */
ROBOT_PRO_API int HRIF_SetFTCalibration(unsigned int boxID, unsigned int rbtID, int pointnum,vector<vector<double>> param);

/**
 * @brief Read  force data from the force sensor.
 * @param boxID :Control box ID, 0 as default 
 * @param rbtID : Robot ID, 0 as default 
 * @param dX :  Force data in the X direction
 * @param dY Force data in the Y direction
 * @param dZ :Force data in the Z direction
 * @param dRX :Force data in the RX direction
 * @param dRY: Force data in the RY direction
 * @param dRZ :Force data in the RZ direction
 * @return : nRet=0 : Function call succeeded 
 *           nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_ReadForceData(unsigned int boxID,  unsigned int rbtID,double &dX, double &dy, double &dZ, double &dRX, double &dRY, double &dRZ);

/**
 * @brief Read  force data from the force sensor.
 * @param boxID :Control box ID, 0 as default 
 * @param rbtID : Robot ID, 0 as default 
 * @param weaveType :  Weaving type ( 0 = sine weaving; other types may be added later).
 * @param frequency Oscillation frequency (double, unit: Hz).
 * @param amplitude :Oscillation amplitude (double, unit: mm
 * @param weaveFrameType :Weaving plane selection (int, 0: Plane formed by the tool coordinate system’s Z-axis and welding path direction; 1: Plane formed by the tool coordinate system’s Z-axis and Y-axis).
 * @param rightDwell:  Right dwell time (double, unit: seconds).
 * @param leftDwell :Left dwell time (double, unit: seconds).
 * @param centreDwell :Centre dwell time (double, unit: seconds).
 * @param LPatternAngle : L-pattern angle (double, unit: degrees; currently unsupported, set to 0 for now).
 * @param elevation :Elevation angle (double, unit: degrees).
 * @param azimuth :Azimuth angle (double, unit: degrees).
 * @param centreRise: Center rise (double, unit: mm; currently should be set to 0, support will be added later).
 * @param radius :Radius (double, unit: mm)..
 * @return : nRet=0 : Function call succeeded 
 *           nRet>0 : Error code of function call 
 */ 
ROBOT_PRO_API int HRIF_SetWeaveParams(unsigned int boxID, unsigned int rbtID, int weaveType, double frequency, double amplitude, int weaveFrameType, double rightDwell, double leftDwell, double centreDwell, double LPatternAngle,
    double elevation, double azimuth, double centreRise, double radius);

/**
 * @brief Start or Stop Weave Welding Operation.
 * @param boxID :Control box ID, 0 as default 
 * @param rbtID : Robot ID, 0 as default 
 * @param startWeave : Weaving Welding Enable Flag (1 = ON, 0 = OFF)
 * @return           nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int HRIF_StartWeaveWeld(unsigned int boxID,  unsigned int rbtID,int startWeave);


/**
 *	@brief: SDK Automated Test Print Interface Functions 
  *	@param boxID : Control box ID, 0 as default 
 *	@param bState : Set the status to false to disable printing, or to true to enable printing 
 *	
 *	@return : nRet=0 : Function call succeeded 
 *            nRet>0 : Error code of function call 
 */
ROBOT_PRO_API int SetAutoTestState(unsigned int boxID, bool bState);

#ifdef __cplusplus
}
#endif
#endif // !_HR_PRO_H_
