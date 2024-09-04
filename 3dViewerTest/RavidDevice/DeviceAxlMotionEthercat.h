#pragma once

#ifdef USING_DEVICE

#include "../RavidFramework/DeviceMotion.h"
#include "../RavidFramework/DefinitionsDevice.h"

typedef struct _MOTION_INFO MOTION_INFO, *PMOTION_INFO;

namespace Ravid
{
	namespace Device
	{
		class CDeviceAxlMotionEthercat : public CDeviceMotion
		{
		public:
			RavidUseDynamicCreation();

			RavidPreventCopySelf(CDeviceAxlMotionEthercat);

			DECLARE_DYNAMIC(CDeviceAxlMotionEthercat)

			CDeviceAxlMotionEthercat();
			virtual ~CDeviceAxlMotionEthercat();

			virtual EDeviceInitializeResult Initialize() override;
			virtual EDeviceTerminateResult Terminate() override;

			virtual bool LoadSettings() override;

			enum ESearchSignal
			{
				ESearchSignal_Pos = 0,
				ESearchSignal_Neg = 1,
				ESearchSignal_Home = 4,
				ESearchSignal_EncoderZphas = 5,
				ESearchSignal_UniInput02 = 6,
				ESearchSignal_UniInput03 = 7,
			};

			enum ESearchEdge
			{
				ESearchEdge_Down = 0,
				ESearchEdge_Up = 1,
			};

			enum ESearchMethod
			{
				ESearchMethod_Emergency = 0,
				ESearchMethod_Slow = 1,
			};

		public:
			virtual AxlMotionEthercatGetFunction GetUnitPulse(_Out_ double* pData);
			virtual AxlMotionEthercatSetFunction SetUnitPulse(_In_ double dblData);

			virtual AxlMotionEthercatGetFunction GetStartSpeed(_Out_ double* pData);
			virtual AxlMotionEthercatSetFunction SetStartSpeed(_In_ double dblData);

			virtual AxlMotionEthercatGetFunction GetMaxSpeed(_Out_ double* pData);
			virtual AxlMotionEthercatSetFunction SetMaxSpeed(_In_ double dblData);

			virtual AxlMotionEthercatGetFunction GetPulseOutputMethod(_Out_ EDeviceAxlMotionEthercatPulseOutputMethod* pEnum);
			virtual AxlMotionEthercatSetFunction SetPulseOutputMethod(_In_ EDeviceAxlMotionEthercatPulseOutputMethod eEnum);

			virtual AxlMotionEthercatGetFunction GetEncoderMethod(_Out_ EDeviceAxlMotionEthercatEncoderMethod* pEnum);
			virtual AxlMotionEthercatSetFunction SetEncoderMethod(_In_ EDeviceAxlMotionEthercatEncoderMethod eEnum);

			virtual AxlMotionEthercatGetFunction GetServoLevel(_Out_ EDeviceAxlMotionEthercatLevelMethod* pEnum);
			virtual AxlMotionEthercatSetFunction SetServoLevel(_In_ EDeviceAxlMotionEthercatLevelMethod eEnum);

			virtual AxlMotionEthercatGetFunction GetHomeLevel(_Out_ EDeviceAxlMotionEthercatLevelMethod* pEnum);
			virtual AxlMotionEthercatSetFunction SetHomeLevel(_In_ EDeviceAxlMotionEthercatLevelMethod eEnum);

			virtual AxlMotionEthercatGetFunction GetPLimitLevel(_Out_ EDeviceAxlMotionEthercatLevelMethod* pEnum);
			virtual AxlMotionEthercatSetFunction SetPLimitLevel(_In_ EDeviceAxlMotionEthercatLevelMethod eEnum);

			virtual AxlMotionEthercatGetFunction GetNLimitLevel(_Out_ EDeviceAxlMotionEthercatLevelMethod* pEnum);
			virtual AxlMotionEthercatSetFunction SetNLimitLevel(_In_ EDeviceAxlMotionEthercatLevelMethod eEnum);

			virtual AxlMotionEthercatGetFunction GetAlarmInputLevel(_Out_ EDeviceAxlMotionEthercatLevelMethod* pEnum);
			virtual AxlMotionEthercatSetFunction SetAlarmInputLevel(_In_ EDeviceAxlMotionEthercatLevelMethod eEnum);

			virtual AxlMotionEthercatGetFunction GetAlarmResetLevel(_Out_ EDeviceAxlMotionEthercatLevelMethod* pEnum);
			virtual AxlMotionEthercatSetFunction SetAlarmResetLevel(_In_ EDeviceAxlMotionEthercatLevelMethod eEnum);

			virtual AxlMotionEthercatGetFunction GetInpositionInputLevel(_Out_ EDeviceAxlMotionEthercatLevelMethod* pEnum);
			virtual AxlMotionEthercatSetFunction SetInpositionInputLevel(_In_ EDeviceAxlMotionEthercatLevelMethod eEnum);

			virtual AxlMotionEthercatGetFunction GetEnableSoftwareLimit(_Out_ bool* pParam);
			virtual AxlMotionEthercatSetFunction SetEnableSoftwareLimit(_In_ bool bParam);

			virtual AxlMotionEthercatGetFunction GetSoftwareStopMode(_Out_ EDeviceAxlMotionEthercatStopMode* pParam);
			virtual AxlMotionEthercatSetFunction SetSoftwareStopMode(_In_ EDeviceAxlMotionEthercatStopMode eParam);

			virtual AxlMotionEthercatGetFunction GetSoftwareCounterSelection(_Out_ EDeviceAxlMotionEthercatCounterSelection* pParam);
			virtual AxlMotionEthercatSetFunction SetSoftwareCounterSelection(_In_ EDeviceAxlMotionEthercatCounterSelection eParam);

			virtual AxlMotionEthercatGetFunction GetPositiveLimit(_Out_ double* ppParam);
			virtual AxlMotionEthercatSetFunction SetPositiveLimit(_In_ double dblParam);

			virtual AxlMotionEthercatGetFunction GetNegativeLimit(_Out_ double* ppParam);
			virtual AxlMotionEthercatSetFunction SetNegativeLimit(_In_ double dblParam);

			virtual AxlMotionEthercatGetFunction GetHomeSearchSensor(_Out_ EDeviceAxlMotionEthercatSearchSensor* pEnum);
			virtual AxlMotionEthercatSetFunction SetHomeSearchSensor(_In_ EDeviceAxlMotionEthercatSearchSensor eEnum);

			virtual AxlMotionEthercatGetFunction GetHomeDirection(_Out_ EDeviceAxlMotionEthercatDirection* pEnum);
			virtual AxlMotionEthercatSetFunction SetHomeDirection(_In_ EDeviceAxlMotionEthercatDirection eEnum);

			virtual AxlMotionEthercatGetFunction GetHomeZphase(_Out_ EDeviceAxlMotionEthercatZPhase* pEnum);
			virtual AxlMotionEthercatSetFunction SetHomeZphase(_In_ EDeviceAxlMotionEthercatZPhase eEnum);

			virtual AxlMotionEthercatGetFunction GetHomeClearTime(_Out_ double* pData);
			virtual AxlMotionEthercatSetFunction SetHomeClearTime(_In_ double dblData);

			virtual AxlMotionEthercatGetFunction GetHomeOffset(_Out_ double* pData);
			virtual AxlMotionEthercatSetFunction SetHomeOffset(_In_ double dblData);

			virtual AxlMotionEthercatGetFunction GetHomeFirstVelocity(_Out_ double* pData);
			virtual AxlMotionEthercatSetFunction SetHomeFirstVelocity(_In_ double dblData);

			virtual AxlMotionEthercatGetFunction GetHomeSecondVelocity(_Out_ double* pData);
			virtual AxlMotionEthercatSetFunction SetHomeSecondVelocity(_In_ double dblData);

			virtual AxlMotionEthercatGetFunction GetHomeThirdVelocity(_Out_ double* pData);
			virtual AxlMotionEthercatSetFunction SetHomeThirdVelocity(_In_ double dblData);

			virtual AxlMotionEthercatGetFunction GetHomeLastVelocity(_Out_ double* pData);
			virtual AxlMotionEthercatSetFunction SetHomeLastVelocity(_In_ double dblData);

			virtual AxlMotionEthercatGetFunction GetHomeStartAcc(_Out_ double* pData);
			virtual AxlMotionEthercatSetFunction SetHomeStartAcc(_In_ double dblData);

			virtual AxlMotionEthercatGetFunction GetHomeEndAcc(_Out_ double* pData);
			virtual AxlMotionEthercatSetFunction SetHomeEndAcc(_In_ double dblData);

			virtual bool MoveToSignal(_In_ double dblVel, _In_ double dblAcc, _In_ ESearchSignal eDetectSignal, _In_ ESearchEdge eSignalEdge, _In_ ESearchMethod eSignalMethod);

			virtual bool GetServoStatus() override;
			virtual bool SetServoOn(_In_ bool bOn) override;

			virtual bool GetCommandPosition(_Out_ double *pPos) override;
			virtual bool SetCommandPositionClear() override;

			virtual bool GetActualPosition(_Out_ double *pPos) override;
			virtual bool SetActualPositionClear() override;

			virtual bool GetAlarmStatus() override;
			virtual bool SetAlarmClear() override;

			virtual bool GetInposition() override;
			virtual bool GetLimitSensorN() override;
			virtual bool GetLimitSensorP() override;
			virtual bool GetHomeSensor() override;

			virtual bool MovePosition(_In_ double dPos, _In_ double dVel, _In_ double dAcc, _In_ bool bSCurve) override;
			virtual bool MoveDistance(_In_ double dDist, _In_ double dVel, _In_ double dAcc, _In_ bool bSCurve) override;
			virtual bool MoveVelocity(_In_ double dVel, _In_ double dAcc, _In_ bool bSCurve) override;

			virtual bool MoveJog(_In_ double dVel, _In_ double dAcl, _In_ BOOL bSCurve) override;
			virtual bool StopJog() override;

			virtual bool IsMotionDone() override;

			virtual bool MotorStop() override;
			virtual bool MotorEStop() override;

			virtual bool ChangeSpeed(_In_ double dSpeed) override;

			virtual bool WriteGenOutBit(_In_ int nBit, _In_ bool bOn) override;
			virtual bool ReadGenOutBit(_In_ int nBit) override;
			virtual bool ReadGenInputBit(_In_ int nBit) override;

			virtual bool MoveToHome() override;

			virtual EDeviceMotionHommingStatus GetHommingStatus() override;

			DWORD __AxmInfoGetAxis(long lAxisNo, long *lpBoardNo, long *lpModulePos, DWORD *upModuleID);
			DWORD __AxmInfoIsMotionModule(DWORD *upStatus);
			DWORD __AxmInfoIsInvalidAxisNo(long lAxisNo);
			DWORD __AxmInfoGetAxisStatus(long lAxisNo);
			DWORD __AxmInfoGetAxisCount(long *lpAxisCount);
			DWORD __AxmInfoGetFirstAxisNo(long lBoardNo, long lModulePos, long *lpAxisNo);
			DWORD __AxmInfoGetBoardFirstAxisNo(long lBoardNo, long lModulePos, long *lpAxisNo);

			DWORD __AxmVirtualSetAxisNoMap(long lRealAxisNo, long lVirtualAxisNo);
			DWORD __AxmVirtualGetAxisNoMap(long lRealAxisNo, long *lpVirtualAxisNo);
			DWORD __AxmVirtualSetMultiAxisNoMap(long lSize, long *lpRealAxesNo, long *lpVirtualAxesNo);
			DWORD __AxmVirtualGetMultiAxisNoMap(long lSize, long *lpRealAxesNo, long *lpVirtualAxesNo);
			DWORD __AxmVirtualResetAxisMap();

			DWORD __AxmInterruptSetAxisEnable(long lAxisNo, DWORD uUse);
			DWORD __AxmInterruptGetAxisEnable(long lAxisNo, DWORD *upUse);
			DWORD __AxmInterruptRead(long *lpAxisNo, DWORD *upFlag);
			DWORD __AxmInterruptReadAxisFlag(long lAxisNo, long lBank, DWORD *upFlag);
			DWORD __AxmInterruptSetUserEnable(long lAxisNo, long lBank, DWORD uInterruptNum);
			DWORD __AxmInterruptGetUserEnable(long lAxisNo, long lBank, DWORD *upInterruptNum);
			DWORD __AxmMotLoadParaAll(char *szFilePath);
			DWORD __AxmMotSaveParaAll(char *szFilePath);
			DWORD __AxmMotSetParaLoad(long lAxisNo, double dInitPos, double dInitVel, double dInitAccel, double dInitDecel);
			DWORD __AxmMotGetParaLoad(long lAxisNo, double *dpInitPos, double *dpInitVel, double *dpInitAccel, double *dpInitDecel);
			DWORD __AxmMotSetPulseOutMethod(long lAxisNo, DWORD uMethod);
			DWORD __AxmMotGetPulseOutMethod(long lAxisNo, DWORD *upMethod);
			DWORD __AxmMotSetEncInputMethod(long lAxisNo, DWORD uMethod);
			DWORD __AxmMotGetEncInputMethod(long lAxisNo, DWORD *upMethod);
			DWORD __AxmMotSetMoveUnitPerPulse(long lAxisNo, double dUnit, long lPulse);
			DWORD __AxmMotGetMoveUnitPerPulse(long lAxisNo, double *dpUnit, long *lpPulse);
			DWORD __AxmMotSetDecelMode(long lAxisNo, DWORD uMethod);
			DWORD __AxmMotGetDecelMode(long lAxisNo, DWORD *upMethod);
			DWORD __AxmMotSetRemainPulse(long lAxisNo, DWORD uData);
			DWORD __AxmMotGetRemainPulse(long lAxisNo, DWORD *upData);
			DWORD __AxmMotSetMaxVel(long lAxisNo, double dVel);
			DWORD __AxmMotGetMaxVel(long lAxisNo, double *dpVel);
			DWORD __AxmMotSetAbsRelMode(long lAxisNo, DWORD uAbsRelMode);
			DWORD __AxmMotGetAbsRelMode(long lAxisNo, DWORD *upAbsRelMode);
			DWORD __AxmMotSetProfileMode(long lAxisNo, DWORD uProfileMode);
			DWORD __AxmMotGetProfileMode(long lAxisNo, DWORD *upProfileMode);
			DWORD __AxmMotSetAccelUnit(long lAxisNo, DWORD uAccelUnit);
			DWORD __AxmMotGetAccelUnit(long lAxisNo, DWORD *upAccelUnit);
			DWORD __AxmMotSetMinVel(long lAxisNo, double dMinVel);
			DWORD __AxmMotGetMinVel(long lAxisNo, double *dpMinVel);
			DWORD __AxmMotSetAccelJerk(long lAxisNo, double dAccelJerk);
			DWORD __AxmMotGetAccelJerk(long lAxisNo, double *dpAccelJerk);
			DWORD __AxmMotSetDecelJerk(long lAxisNo, double dDecelJerk);
			DWORD __AxmMotGetDecelJerk(long lAxisNo, double *dpDecelJerk);
			DWORD __AxmMotSetProfilePriority(long lAxisNo, DWORD uPriority);
			DWORD __AxmMotGetProfilePriority(long lAxisNo, DWORD *upPriority);
			DWORD __AxmSignalSetZphaseLevel(long lAxisNo, DWORD uLevel);
			DWORD __AxmSignalGetZphaseLevel(long lAxisNo, DWORD *upLevel);
			DWORD __AxmSignalSetServoOnLevel(long lAxisNo, DWORD uLevel);
			DWORD __AxmSignalGetServoOnLevel(long lAxisNo, DWORD *upLevel);
			DWORD __AxmSignalSetServoAlarmResetLevel(long lAxisNo, DWORD uLevel);
			DWORD __AxmSignalGetServoAlarmResetLevel(long lAxisNo, DWORD *upLevel);
			DWORD __AxmSignalSetInpos(long lAxisNo, DWORD uUse);
			DWORD __AxmSignalGetInpos(long lAxisNo, DWORD *upUse);
			DWORD __AxmSignalReadInpos(long lAxisNo, DWORD *upStatus);
			DWORD __AxmSignalSetServoAlarm(long lAxisNo, DWORD uUse);
			DWORD __AxmSignalGetServoAlarm(long lAxisNo, DWORD *upUse);
			DWORD __AxmSignalReadServoAlarm(long lAxisNo, DWORD *upStatus);
			DWORD __AxmSignalSetLimit(long lAxisNo, DWORD uStopMode, DWORD uPositiveLevel, DWORD uNegativeLevel);
			DWORD __AxmSignalGetLimit(long lAxisNo, DWORD *upStopMode, DWORD *upPositiveLevel, DWORD *upNegativeLevel);
			DWORD __AxmSignalReadLimit(long lAxisNo, DWORD *upPositiveStatus, DWORD *upNegativeStatus);

			DWORD __AxmSignalSetSoftLimit(long lAxisNo, DWORD uUse, DWORD uStopMode, DWORD uSelection, double dPositivePos, double dNegativePos);
			DWORD __AxmSignalGetSoftLimit(long lAxisNo, DWORD *upUse, DWORD *upStopMode, DWORD *upSelection, double *dpPositivePos, double *dpNegativePos);

			DWORD __AxmSignalReadSoftLimit(long lAxisNo, DWORD *upPositiveStatus, DWORD *upNegativeStatus);
			DWORD __AxmSignalSetStop(long lAxisNo, DWORD uStopMode, DWORD uLevel);
			DWORD __AxmSignalGetStop(long lAxisNo, DWORD *upStopMode, DWORD *upLevel);
			DWORD __AxmSignalReadStop(long lAxisNo, DWORD *upStatus);
			DWORD __AxmSignalServoOn(long lAxisNo, DWORD uOnOff);
			DWORD __AxmSignalIsServoOn(long lAxisNo, DWORD *upOnOff);
			DWORD __AxmSignalServoAlarmReset(long lAxisNo, DWORD uOnOff);
			DWORD __AxmSignalWriteOutput(long lAxisNo, DWORD uValue);
			DWORD __AxmSignalReadOutput(long lAxisNo, DWORD *upValue);
			DWORD __AxmSignalReadBrakeOn(long lAxisNo, DWORD *upOnOff);
			DWORD __AxmSignalWriteOutputBit(long lAxisNo, long lBitNo, DWORD uOnOff);
			DWORD __AxmSignalReadOutputBit(long lAxisNo, long lBitNo, DWORD *upOnOff);
			DWORD __AxmSignalReadInput(long lAxisNo, DWORD *upValue);
			DWORD __AxmSignalReadInputBit(long lAxisNo, long lBitNo, DWORD *upOn);
			DWORD __AxmSignalSetFilterBandwidth(long lAxisNo, DWORD uSignal, double dBandwidthUsec);
			DWORD __AxmStatusReadInMotion(long lAxisNo, DWORD *upStatus);
			DWORD __AxmStatusReadDrivePulseCount(long lAxisNo, long *lpPulse);
			DWORD __AxmStatusReadMotion(long lAxisNo, DWORD *upStatus);
			DWORD __AxmStatusReadStop(long lAxisNo, DWORD *upStatus);
			DWORD __AxmStatusReadMechanical(long lAxisNo, DWORD *upStatus);
			DWORD __AxmStatusReadVel(long lAxisNo, double *dpVel);
			DWORD __AxmStatusReadPosError(long lAxisNo, double *dpError);
			DWORD __AxmStatusReadDriveDistance(long lAxisNo, double *dpUnit);
			DWORD __AxmStatusSetPosType(long lAxisNo, DWORD uPosType, double dPositivePos, double dNegativePos);
			DWORD __AxmStatusGetPosType(long lAxisNo, DWORD *upPosType, double *dpPositivePos, double *dpNegativePos);
			DWORD __AxmStatusSetAbsOrgOffset(long lAxisNo, double dOrgOffsetPos);
			DWORD __AxmStatusSetActPos(long lAxisNo, double dPos);
			DWORD __AxmStatusGetActPos(long lAxisNo, double *dpPos);
			DWORD __AxmStatusSetCmdPos(long lAxisNo, double dPos);
			DWORD __AxmStatusGetCmdPos(long lAxisNo, double *dpPos);
			DWORD __AxmStatusSetPosMatch(long lAxisNo, double dPos);
			DWORD __AxmStatusReadMotionInfo(long lAxisNo, PMOTION_INFO pMI);
			DWORD __AxmStatusRequestServoAlarm(long lAxisNo);
			DWORD __AxmStatusReadServoAlarm(long lAxisNo, DWORD uReturnMode, DWORD *upAlarmCode);
			DWORD __AxmStatusGetServoAlarmString(long lAxisNo, DWORD uAlarmCode, long lAlarmStringSize, char *szAlarmString);
			DWORD __AxmStatusRequestServoAlarmHistory(long lAxisNo);
			DWORD __AxmStatusReadServoAlarmHistory(long lAxisNo, DWORD uReturnMode, long *lpCount, DWORD *upAlarmCode);
			DWORD __AxmStatusClearServoAlarmHistory(long lAxisNo);
			DWORD __AxmHomeSetSignalLevel(long lAxisNo, DWORD uLevel);
			DWORD __AxmHomeGetSignalLevel(long lAxisNo, DWORD *upLevel);
			DWORD __AxmHomeReadSignal(long lAxisNo, DWORD *upStatus);

			DWORD __AxmHomeSetMethod(long lAxisNo, long lHmDir, DWORD uHomeSignal, DWORD uZphas, double dHomeClrTime, double dHomeOffset);
			DWORD __AxmHomeGetMethod(long lAxisNo, long *lpHmDir, DWORD *upHomeSignal, DWORD *upZphas, double *dpHomeClrTime, double *dpHomeOffset);
			DWORD __AxmHomeSetFineAdjust(long lAxisNo, double dHomeDogLength, long lLevelScanTime, DWORD uFineSearchUse, DWORD uHomeClrUse);
			DWORD __AxmHomeGetFineAdjust(long lAxisNo, double *dpHomeDogLength, long *lpLevelScanTime, DWORD *upFineSearchUse, DWORD *upHomeClrUse);
			DWORD __AxmHomeSetVel(long lAxisNo, double dVelFirst, double dVelSecond, double dVelThird, double dVelLast, double dAccFirst, double dAccSecond);
			DWORD __AxmHomeGetVel(long lAxisNo, double *dpVelFirst, double *dpVelSecond, double *dpVelThird, double *dpVelLast, double *dpAccFirst, double *dpAccSecond);

			DWORD __AxmHomeSetStart(long lAxisNo);
			DWORD __AxmHomeSetResult(long lAxisNo, DWORD uHomeResult);
			DWORD __AxmHomeGetResult(long lAxisNo, DWORD *upHomeResult);
			DWORD __AxmHomeGetRate(long lAxisNo, DWORD *upHomeMainStepNumber, DWORD *upHomeStepNumber);
			DWORD __AxmMoveStartPos(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel);
			DWORD __AxmMovePos(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel);
			DWORD __AxmMoveVel(long lAxisNo, double dVel, double dAccel, double dDecel);
			DWORD __AxmMoveStartMultiVel(long lArraySize, long *lpAxesNo, double *dpVel, double *dpAccel, double *dpDecel);

			DWORD __AxmMoveStartMultiVelEx(long lArraySize, long *lpAxesNo, double *dpVel, double *dpAccel, double *dpDecel, DWORD dwSyncMode);

			DWORD __AxmMoveStartLineVel(long lArraySize, long *lpAxesNo, double *dpDis, double dVel, double dAccel, double dDecel);

			DWORD __AxmMoveSignalSearch(long lAxisNo, double dVel, double dAccel, long lDetectSignal, long lSignalEdge, long lSignalMethod);
			DWORD __AxmMoveSignalSearchAtDis(long lAxisNo, double dVel, double dAccel, double dDecel, long lDetectSignal, double dDis);
			DWORD __AxmMoveSignalCapture(long lAxisNo, double dVel, double dAccel, long lDetectSignal, long lSignalEdge, long lTarget, long lSignalMethod);

			DWORD __AxmMoveGetCapturePos(long lAxisNo, double *dpCapPotition);
			DWORD __AxmMoveStartMultiPos(long lArraySize, long *lpAxisNo, double *dpPos, double *dpVel, double *dpAccel, double *dpDecel);
			DWORD __AxmMoveStartTorque(long lAxisNo, double dTorque, double dVel, DWORD dwAccFilterSel, DWORD dwGainSel, DWORD dwSpdLoopSel);

			DWORD __AxmMoveTorqueStop(long lAxisNo, DWORD dwMethod);

			DWORD __AxmMoveStartPosWithList(long lAxisNo, double dPosition, double *dpVel, double *dpAccel, double *dpDecel, long lListNum);
			DWORD __AxmMoveStartPosWithPosEvent(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel, long lEventAxisNo, double dComparePosition, DWORD uPositionSource);

			DWORD __AxmMoveStop(long lAxisNo, double dDecel);
			DWORD __AxmMoveStopEx(long lAxisNo, double dDecel);
			DWORD __AxmMoveEStop(long lAxisNo);
			DWORD __AxmMoveSStop(long lAxisNo);
			DWORD __AxmOverridePos(long lAxisNo, double dOverridePos);
			DWORD __AxmOverrideSetMaxVel(long lAxisNo, double dOverrideMaxVel);
			DWORD __AxmOverrideVel(long lAxisNo, double dOverrideVel);
			DWORD __AxmOverrideAccelVelDecel(long lAxisNo, double dOverrideVelocity, double dMaxAccel, double dMaxDecel);

			DWORD __AxmOverrideVelAtPos(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel, double dOverridePos, double dOverrideVel, long lTarget);
			DWORD __AxmOverrideVelAtMultiPos(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel, long lArraySize, double* dpOverridePos, double* dpOverrideVel, long lTarget, DWORD dwOverrideMode);
			DWORD __AxmOverrideVelAtMultiPos2(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel, long lArraySize, double* dpOverridePos, double* dpOverrideVel, double* dpOverrideAccelDecel, long lTarget, DWORD dwOverrideMode);

			DWORD __AxmLinkSetMode(long lMasterAxisNo, long lSlaveAxisNo, double dSlaveRatio);
			DWORD __AxmLinkGetMode(long lMasterAxisNo, long *lpSlaveAxisNo, double *dpGearRatio);
			DWORD __AxmLinkResetMode(long lMasterAxisNo);

			DWORD __AxmGantrySetEnable(long lMasterAxisNo, long lSlaveAxisNo, DWORD uSlHomeUse, double dSlOffset, double dSlOffsetRange);
			DWORD __AxmGantryGetEnable(long lMasterAxisNo, DWORD *upSlHomeUse, double *dpSlOffset, double *dpSlORange, DWORD *upGatryOn);

			DWORD __AxmGantrySetDisable(long lMasterAxisNo, long lSlaveAxisNo);
			DWORD __AxmGantrySetCompensationGain(long lMasterAxisNo, long lMasterGain, long lSlaveGain);
			DWORD __AxmGantryGetCompensationGain(long lMasterAxisNo, long *lpMasterGain, long *lpSlaveGain);
			DWORD __AxmGantrySetErrorRange(long lMasterAxisNo, double dErrorRange, DWORD uUse);
			DWORD __AxmGantryGetErrorRange(long lMasterAxisNo, double *dpErrorRange, DWORD *upUse);
			DWORD __AxmGantryReadErrorRangeStatus(long lMasterAxisNo, DWORD *dwpStatus);
			DWORD __AxmGantryReadErrorRangeComparePos(long lMasterAxisNo, double *dpComparePos);
			DWORD __AxmLineMove(long lCoord, double *dpEndPos, double dVel, double dAccel, double dDecel);
			DWORD __AxmLineMoveEx2(long lCoord, double *dpEndPos, double dVel, double dAccel, double dDecel);

			DWORD __AxmCircleCenterMove(long lCoord, long *lAxisNo, double *dCenterPos, double *dEndPos, double dVel, double dAccel, double dDecel, DWORD uCWDir);
			DWORD __AxmCirclePointMove(long lCoord, long *lAxisNo, double *dMidPos, double *dEndPos, double dVel, double dAccel, double dDecel, long lArcCircle);
			DWORD __AxmCircleRadiusMove(long lCoord, long *lAxisNo, double dRadius, double *dEndPos, double dVel, double dAccel, double dDecel, DWORD uCWDir, DWORD uShortDistance);
			DWORD __AxmCircleAngleMove(long lCoord, long *lAxisNo, double *dCenterPos, double dAngle, double dVel, double dAccel, double dDecel, DWORD uCWDir);

			DWORD __AxmContiSetAxisMap(long lCoord, long lSize, long *lpAxesNo);
			DWORD __AxmContiGetAxisMap(long lCoord, long *lpSize, long *lpAxesNo);
			DWORD __AxmContiSetAbsRelMode(long lCoord, DWORD uAbsRelMode);
			DWORD __AxmContiGetAbsRelMode(long lCoord, DWORD *upAbsRelMode);
			DWORD __AxmContiReadFree(long lCoord, DWORD *upQueueFree);
			DWORD __AxmContiReadIndex(long lCoord, long *lpQueueIndex);
			DWORD __AxmContiWriteClear(long lCoord);
			DWORD __AxmContiBeginNode(long lCoord);
			DWORD __AxmContiEndNode(long lCoord);
			DWORD __AxmContiStart(long lCoord, DWORD dwProfileset, long lAngle);
			DWORD __AxmContiIsMotion(long lCoord, DWORD *upInMotion);
			DWORD __AxmContiGetNodeNum(long lCoord, long *lpNodeNum);
			DWORD __AxmContiGetTotalNodeNum(long lCoord, long *lpNodeNum);
			DWORD __AxmTriggerSetTimeLevel(long lAxisNo, double dTrigTime, DWORD uTriggerLevel, DWORD uSelect, DWORD uInterrupt);

			DWORD __AxmTriggerGetTimeLevel(long lAxisNo, double *dpTrigTime, DWORD *upTriggerLevel, DWORD *upSelect, DWORD *upInterrupt);

			DWORD __AxmTriggerSetAbsPeriod(long lAxisNo, DWORD uMethod, double dPos);
			DWORD __AxmTriggerGetAbsPeriod(long lAxisNo, DWORD *upMethod, double *dpPos);
			DWORD __AxmTriggerSetBlock(long lAxisNo, double dStartPos, double dEndPos, double dPeriodPos);
			DWORD __AxmTriggerGetBlock(long lAxisNo, double *dpStartPos, double *dpEndPos, double *dpPeriodPos);
			DWORD __AxmTriggerOneShot(long lAxisNo);
			DWORD __AxmTriggerSetTimerOneshot(long lAxisNo, long lmSec);
			DWORD __AxmTriggerOnlyAbs(long lAxisNo, long lTrigNum, double* dpTrigPos);
			DWORD __AxmTriggerSetReset(long lAxisNo);
			DWORD __AxmCrcSetMaskLevel(long lAxisNo, DWORD uLevel, DWORD uMethod);
			DWORD __AxmCrcGetMaskLevel(long lAxisNo, DWORD *upLevel, DWORD *upMethod);
			DWORD __AxmCrcSetOutput(long lAxisNo, DWORD uOnOff);
			DWORD __AxmCrcGetOutput(long lAxisNo, DWORD *upOnOff);
			DWORD __AxmMPGSetEnable(long lAxisNo, long lInputMethod, long lDriveMode, double dMPGPos, double dVel, double dAccel);

			DWORD __AxmMPGGetEnable(long lAxisNo, long *lpInputMethod, long *lpDriveMode, double *dpMPGPos, double *dpVel, double *dAccel);

			DWORD __AxmMPGSetRatio(long lAxisNo, DWORD uMPGnumerator, DWORD uMPGdenominator);
			DWORD __AxmMPGGetRatio(long lAxisNo, DWORD *upMPGnumerator, DWORD *upMPGdenominator);
			DWORD __AxmMPGReset(long lAxisNo);

			DWORD __AxmHelixCenterMove(long lCoord, double dCenterXPos, double dCenterYPos, double dEndXPos, double dEndYPos, double dZPos, double dVel, double dAccel, double dDecel, DWORD uCWDir);
			DWORD __AxmHelixPointMove(long lCoord, double dMidXPos, double dMidYPos, double dEndXPos, double dEndYPos, double dZPos, double dVel, double dAccel, double dDecel);
			DWORD __AxmHelixRadiusMove(long lCoord, double dRadius, double dEndXPos, double dEndYPos, double dZPos, double dVel, double dAccel, double dDecel, DWORD uCWDir, DWORD uShortDistance);
			DWORD __AxmHelixAngleMove(long lCoord, double dCenterXPos, double dCenterYPos, double dAngle, double dZPos, double dVel, double dAccel, double dDecel, DWORD uCWDir);
			DWORD __AxmSplineWrite(long lCoord, long lPosSize, double *dpPosX, double *dpPosY, double dVel, double dAccel, double dDecel, double dPosZ, long lPointFactor);
			DWORD __AxmCompensationSet(long lAxisNo, long lNumEntry, double dStartPos, double *dpPosition, double *dpCorrection, DWORD dwRollOver);
			DWORD __AxmCompensationGet(long lAxisNo, long *lpNumEntry, double *dpStartPos, double *dpPosition, double *dpCorrection, DWORD *dwpRollOver);

			DWORD __AxmCompensationEnable(long lAxisNo, DWORD dwEnable);
			DWORD __AxmCompensationIsEnable(long lAxisNo, DWORD *dwpEnable);
			DWORD __AxmCompensationGetCorrection(long lAxisNo, double *dpCorrection);
			DWORD __AxmCompensationSetBacklash(long lAxisNo, long lBacklashDir, double dBacklash);
			DWORD __AxmCompensationGetBacklash(long lAxisNo, long *lpBacklashDir, double *dpBacklash);
			DWORD __AxmCompensationEnableBacklash(long lAxisNo, DWORD dwEnable);
			DWORD __AxmCompensationIsEnableBacklash(long lAxisNo, DWORD *dwpEnable);
			DWORD __AxmCompensationSetLocating(long lAxisNo, double dVel, double dAccel, double dDecel, double dWaitTime);

			DWORD __AxmEcamSet(long lAxisNo, long lMasterAxis, long lNumEntry, double dMasterStartPos, double *dpMasterPos, double *dpSlavePos);
			DWORD __AxmEcamSetWithSource(long lAxisNo, long lMasterAxis, long lNumEntry, double dMasterStartPos, double *dpMasterPos, double *dpSlavePos, DWORD dwSource);
			DWORD __AxmEcamGet(long lAxisNo, long *lpMasterAxis, long *lpNumEntry, double *dpMasterStartPos, double *dpMasterPos, double *dpSlavePos);
			DWORD __AxmEcamGetWithSource(long lAxisNo, long *lpMasterAxis, long *lpNumEntry, double *dpMasterStartPos, double *dpMasterPos, double *dpSlavePos, DWORD *dwpSource);

			DWORD __AxmEcamEnableBySlave(long lAxisNo, DWORD dwEnable);
			DWORD __AxmEcamEnableByMaster(long lAxisNo, DWORD dwEnable);
			DWORD __AxmEcamIsSlaveEnable(long lAxisNo, DWORD *dwpEnable);
			DWORD __AxmStatusSetServoMonitor(long lAxisNo, DWORD dwSelMon, double dActionValue, DWORD dwAction);
			DWORD __AxmStatusGetServoMonitor(long lAxisNo, DWORD dwSelMon, double *dpActionValue, DWORD *dwpAction);
			DWORD __AxmStatusSetServoMonitorEnable(long lAxisNo, DWORD dwEnable);
			DWORD __AxmStatusGetServoMonitorEnable(long lAxisNo, DWORD *dwpEnable);
			DWORD __AxmStatusReadServoMonitorFlag(long lAxisNo, DWORD dwSelMon, DWORD *dwpMonitorFlag, double *dpMonitorValue);
			DWORD __AxmStatusReadServoMonitorValue(long lAxisNo, DWORD dwSelMon, double *dpMonitorValue);
			DWORD __AxmStatusSetReadServoLoadRatio(long lAxisNo, DWORD dwSelMon);
			DWORD __AxmStatusReadServoLoadRatio(long lAxisNo, double *dpMonitorValue);
			DWORD __AxmMotSetScaleCoeff(long lAxisNo, long lScaleCoeff);
			DWORD __AxmMotGetScaleCoeff(long lAxisNo, long *lpScaleCoeff);

			DWORD __AxmMoveSignalSearchEx(long lAxisNo, double dVel, double dAccel, long lDetectSignal, long lSignalEdge, long lSignalMethod);

			DWORD __AxmMoveToAbsPos(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel);
			DWORD __AxmStatusReadVelEx(long lAxisNo, double *dpVel);
			DWORD __AxmMotSetElectricGearRatio(long lAxisNo, long lNumerator, long lDenominator);
			DWORD __AxmMotGetElectricGearRatio(long lAxisNo, long *lpNumerator, long *lpDenominator);
			DWORD __AxmMotSetTorqueLimit(long lAxisNo, double dbPlusDirTorqueLimit, double dbMinusDirTorqueLimit);
			DWORD __AxmMotGetTorqueLimit(long lAxisNo, double* dbpPlusDirTorqueLimit, double* dbpMinusDirTorqueLimit);
			DWORD __AxmMotSetTorqueLimitEx(long lAxisNo, double dbPlusDirTorqueLimit, double dbMinusDirTorqueLimit);
			DWORD __AxmMotGetTorqueLimitEx(long lAxisNo, double* dbpPlusDirTorqueLimit, double* dbpMinusDirTorqueLimit);

			DWORD __AxmMotSetTorqueLimitAtPos(long lAxisNo, double dbPlusDirTorqueLimit, double dbMinusDirTorqueLimit, double dPosition, long lTarget);
			DWORD __AxmMotGetTorqueLimitAtPos(long lAxisNo, double* dbpPlusDirTorqueLimit, double* dbpMinusDirTorqueLimit, double* dpPosition, long* lpTarget);

			DWORD __AxmMotSetTorqueLimitEnable(long lAxisNo, DWORD uUse);
			DWORD __AxmMotGetTorqueLimitEnable(long lAxisNo, DWORD* upUse);
			DWORD __AxmOverridePosSetFunction(long lAxisNo, DWORD dwUsage, long lDecelPosRatio, double dReserved);
			DWORD __AxmOverridePosGetFunction(long lAxisNo, DWORD *dwpUsage, long *lpDecelPosRatio, double *dpReserved);

			DWORD __AxmSignalSetWriteOutputBitAtPos(long lAxisNo, long lModuleNo, long lOffset, DWORD uValue, double dPosition, long lTarget);
			DWORD __AxmSignalGetWriteOutputBitAtPos(long lAxisNo, long* lpModuleNo, long* lOffset, DWORD* upValue, double* dpPosition, long* lpTarget);
			DWORD __AxmAdvVSTSetParameter(long lCoord, DWORD dwISTSize, double* dbpFrequency, double* dbpDampingRatio, DWORD* dwpImpulseCount);
			DWORD __AxmAdvVSTGetParameter(long lCoord, DWORD* dwpISTSize, double* dbpFrequency, double* dbpDampingRatio, DWORD* dwpImpulseCount);

			DWORD __AxmAdvVSTSetEnabele(long lCoord, DWORD dwISTEnable);
			DWORD __AxmAdvVSTGetEnabele(long lCoord, DWORD* dwpISTEnable);

			DWORD __AxmAdvLineMove(long lCoordinate, double *dPosition, double dMaxVelocity, double dStartVel, double dStopVel, double dMaxAccel, double dMaxDecel);
			DWORD __AxmAdvOvrLineMove(long lCoordinate, double *dPosition, double dMaxVelocity, double dStartVel, double dStopVel, double dMaxAccel, double dMaxDecel, long lOverrideMode);
			DWORD __AxmAdvCircleCenterMove(long lCoord, long *lAxisNo, double *dCenterPos, double *dEndPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir);
			DWORD __AxmAdvCirclePointMove(long lCoord, long *lAxisNo, double *dMidPos, double *dEndPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, long lArcCircle);
			DWORD __AxmAdvCircleAngleMove(long lCoord, long *lAxisNo, double *dCenterPos, double dAngle, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir);
			DWORD __AxmAdvCircleRadiusMove(long lCoord, long *lAxisNo, double dRadius, double *dEndPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir, DWORD uShortDistance);
			DWORD __AxmAdvOvrCircleRadiusMove(long lCoord, long *lAxisNo, double dRadius, double *dEndPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir, DWORD uShortDistance, long lOverrideMode);
			DWORD __AxmAdvHelixCenterMove(long lCoord, double dCenterXPos, double dCenterYPos, double dEndXPos, double dEndYPos, double dZPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir);
			DWORD __AxmAdvHelixPointMove(long lCoord, double dMidXPos, double dMidYPos, double dEndXPos, double dEndYPos, double dZPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel);
			DWORD __AxmAdvHelixAngleMove(long lCoord, double dCenterXPos, double dCenterYPos, double dAngle, double dZPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir);
			DWORD __AxmAdvHelixRadiusMove(long lCoord, double dRadius, double dEndXPos, double dEndYPos, double dZPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir, DWORD uShortDistance);
			DWORD __AxmAdvOvrHelixRadiusMove(long lCoord, double dRadius, double dEndXPos, double dEndYPos, double dZPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir, DWORD uShortDistance, long lOverrideMode);
			DWORD __AxmAdvScriptLineMove(long lCoordinate, double *dPosition, double dMaxVelocity, double dStartVel, double dStopVel, double dMaxAccel, double dMaxDecel, DWORD dwScript, long lScirptAxisNo, double dScriptPos);
			DWORD __AxmAdvScriptOvrLineMove(long lCoordinate, double *dPosition, double dMaxVelocity, double dStartVel, double dStopVel, double dMaxAccel, double dMaxDecel, long lOverrideMode, DWORD dwScript, long lScirptAxisNo, double dScriptPos);
			DWORD __AxmAdvScriptCircleCenterMove(long lCoord, long *lAxisNo, double *dCenterPos, double *dEndPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir, DWORD dwScript, long lScirptAxisNo, double dScriptPos);
			DWORD __AxmAdvScriptCirclePointMove(long lCoord, long *lAxisNo, double *dMidPos, double *dEndPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, long lArcCircle, DWORD dwScript, long lScirptAxisNo, double dScriptPos);
			DWORD __AxmAdvScriptCircleAngleMove(long lCoord, long *lAxisNo, double *dCenterPos, double dAngle, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir, DWORD dwScript, long lScirptAxisNo, double dScriptPos);
			DWORD __AxmAdvScriptCircleRadiusMove(long lCoord, long *lAxisNo, double dRadius, double *dEndPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir, DWORD uShortDistance, DWORD dwScript, long lScirptAxisNo, double dScriptPos);
			DWORD __AxmAdvScriptOvrCircleRadiusMove(long lCoord, long *lAxisNo, double dRadius, double *dEndPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir, DWORD uShortDistance, long lOverrideMode, DWORD dwScript, long lScirptAxisNo, double dScriptPos);
			DWORD __AxmAdvScriptHelixCenterMove(long lCoord, double dCenterXPos, double dCenterYPos, double dEndXPos, double dEndYPos, double dZPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir, DWORD dwScript, long lScirptAxisNo, double dScriptPos);
			DWORD __AxmAdvScriptHelixPointMove(long lCoord, double dMidXPos, double dMidYPos, double dEndXPos, double dEndYPos, double dZPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD dwScript, long lScirptAxisNo, double dScriptPos);
			DWORD __AxmAdvScriptHelixAngleMove(long lCoord, double dCenterXPos, double dCenterYPos, double dAngle, double dZPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir, DWORD dwScript, long lScirptAxisNo, double dScriptPos);
			DWORD __AxmAdvScriptHelixRadiusMove(long lCoord, double dRadius, double dEndXPos, double dEndYPos, double dZPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir, DWORD uShortDistance, DWORD dwScript, long lScirptAxisNo, double dScriptPos);
			DWORD __AxmAdvScriptOvrHelixRadiusMove(long lCoord, double dRadius, double dEndXPos, double dEndYPos, double dZPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir, DWORD uShortDistance, long lOverrideMode, DWORD dwScript, long lScirptAxisNo, double dScriptPos);

			DWORD __AxmAdvContiGetNodeNum(long lCoordinate, long *lpNodeNum);
			DWORD __AxmAdvContiGetTotalNodeNum(long lCoordinate, long *lpNodeNum);
			DWORD __AxmAdvContiReadIndex(long lCoordinate, long *lpQueueIndex);
			DWORD __AxmAdvContiReadFree(long lCoordinate, DWORD *upQueueFree);
			DWORD __AxmAdvContiWriteClear(long lCoordinate);
			DWORD __AxmAdvOvrContiWriteClear(long lCoordinate);
			DWORD __AxmAdvContiStart(long lCoord, DWORD dwProfileset, long lAngle);
			DWORD __AxmAdvContiStop(long lCoordinate, double dDecel);
			DWORD __AxmAdvContiSetAxisMap(long lCoord, long lSize, long *lpAxesNo);
			DWORD __AxmAdvContiGetAxisMap(long lCoord, long *lpSize, long *lpAxesNo);
			DWORD __AxmAdvContiSetAbsRelMode(long lCoord, DWORD uAbsRelMode);
			DWORD __AxmAdvContiGetAbsRelMode(long lCoord, DWORD *uAbsRelMode);
			DWORD __AxmAdvContiIsMotion(long lCoordinate, DWORD *upInMotion);
			DWORD __AxmAdvContiBeginNode(long lCoord);
			DWORD __AxmAdvContiEndNode(long lCoord);
			DWORD __AxmMoveMultiStop(long lArraySize, long *lpAxesNo, double *dMaxDecel);
			DWORD __AxmMoveMultiEStop(long lArraySize, long *lpAxesNo);
			DWORD __AxmMoveMultiSStop(long lArraySize, long *lpAxesNo);
			DWORD __AxmStatusReadActVel(long lAxisNo, double *dpVel);          //2010.10.11
			DWORD __AxmStatusReadServoCmdStat(long lAxisNo, DWORD *upStatus);
			DWORD __AxmStatusReadServoCmdCtrl(long lAxisNo, DWORD *upStatus);
			DWORD __AxmGantryGetMstToSlvOverDist(long lAxisNo, double *dpPosition);
			DWORD __AxmGantrySetMstToSlvOverDist(long lAxisNo, double dPosition);
			DWORD __AxmSignalReadServoAlarmCode(long lAxisNo, WORD *upCodeStatus);
			DWORD __AxmM3ServoCoordinatesSet(long lAxisNo, DWORD dwPosData, DWORD dwPos_sel, DWORD dwRefe);
			DWORD __AxmM3ServoBreakOn(long lAxisNo);
			DWORD __AxmM3ServoBreakOff(long lAxisNo);
			DWORD __AxmM3ServoConfig(long lAxisNo, DWORD dwCfMode);
			DWORD __AxmM3ServoSensOn(long lAxisNo);
			DWORD __AxmM3ServoSensOff(long lAxisNo);
			DWORD __AxmM3ServoSmon(long lAxisNo);
			DWORD __AxmM3ServoGetSmon(long lAxisNo, BYTE *pbParam);
			DWORD __AxmM3ServoSvOn(long lAxisNo);
			DWORD __AxmM3ServoSvOff(long lAxisNo);
			DWORD __AxmM3ServoInterpolate(long lAxisNo, DWORD dwTPOS, DWORD dwVFF, DWORD dwTFF, DWORD dwTLIM);
			DWORD __AxmM3ServoPosing(long lAxisNo, DWORD dwTPOS, DWORD dwSPD, DWORD dwACCR, DWORD dwDECR, DWORD dwTLIM);
			DWORD __AxmM3ServoFeed(long lAxisNo, long lSPD, DWORD dwACCR, DWORD dwDECR, DWORD dwTLIM);

			DWORD __AxmM3ServoExFeed(long lAxisNo, long lSPD, DWORD dwACCR, DWORD dwDECR, DWORD dwTLIM, DWORD dwExSig1, DWORD dwExSig2);
			DWORD __AxmM3ServoExPosing(long lAxisNo, DWORD dwTPOS, DWORD dwSPD, DWORD dwACCR, DWORD dwDECR, DWORD dwTLIM, DWORD dwExSig1, DWORD dwExSig2);
			DWORD __AxmM3ServoZret(long lAxisNo, DWORD dwSPD, DWORD dwACCR, DWORD dwDECR, DWORD dwTLIM, DWORD dwExSig1, DWORD dwExSig2, BYTE bHomeDir, BYTE bHomeType);

			DWORD __AxmM3ServoVelctrl(long lAxisNo, DWORD dwTFF, DWORD dwVREF, DWORD dwACCR, DWORD dwDECR, DWORD dwTLIM);
			DWORD __AxmM3ServoTrqctrl(long lAxisNo, DWORD dwVLIM, long lTQREF);
			DWORD __AxmM3ServoGetParameter(long lAxisNo, WORD wNo, BYTE bSize, BYTE bMode, BYTE *pbParam);
			DWORD __AxmM3ServoSetParameter(long lAxisNo, WORD wNo, BYTE bSize, BYTE bMode, BYTE *pbParam);
			DWORD __AxmServoCmdExecution(long lAxisNo, DWORD dwCommand, DWORD dwSize, DWORD *pdwExcData);
			DWORD __AxmM3ServoGetTorqLimit(long lAxisNo, DWORD *dwpTorqLimit);
			DWORD __AxmM3ServoSetTorqLimit(long lAxisNo, DWORD dwTorqLimit);
			DWORD __AxmM3ServoGetSendSvCmdIOOutput(long lAxisNo, DWORD *dwData);
			DWORD __AxmM3ServoSetSendSvCmdIOOutput(long lAxisNo, DWORD dwData);
			DWORD __AxmM3ServoGetSvCmdCtrl(long lAxisNo, DWORD *dwData);
			DWORD __AxmM3ServoSetSvCmdCtrl(long lAxisNo, DWORD dwData);
			DWORD __AxmM3AdjustmentOperation(long lAxisNo, DWORD dwReqCode);
			DWORD __AxmM3ServoSetMonSel(long lAxisNo, DWORD dwMon0, DWORD dwMon1, DWORD dwMon2);
			DWORD __AxmM3ServoGetMonSel(long lAxisNo, DWORD *upMon0, DWORD *upMon1, DWORD *upMon2);
			DWORD __AxmM3ServoReadMonData(long lAxisNo, DWORD dwMonSel, DWORD *dwpMonData);
			DWORD __AxmAdvTorqueContiSetAxisMap(long lCoord, long lSize, long *lpAxesNo, DWORD dwTLIM, DWORD dwConMode);
			DWORD __AxmM3ServoSetTorqProfile(long lCoord, long lAxisNo, long TorqueSign, DWORD dwVLIM, DWORD dwProfileMode, DWORD dwStdTorq, DWORD dwStopTorq);
			DWORD __AxmM3ServoGetTorqProfile(long lCoord, long lAxisNo, long *lpTorqueSign, DWORD *updwVLIM, DWORD *upProfileMode, DWORD *upStdTorq, DWORD *upStopTorq);

			DWORD __AxmSignalSetInposRange(long lAxisNo, double dInposRange);
			DWORD __AxmSignalGetInposRange(long lAxisNo, double *dpInposRange);
			DWORD __AxmMotSetOverridePosMode(long lAxisNo, DWORD dwAbsRelMode);
			DWORD __AxmMotGetOverridePosMode(long lAxisNo, DWORD *dwpAbsRelMode);
			DWORD __AxmMotSetOverrideLinePosMode(long lCoordNo, DWORD dwAbsRelMode);
			DWORD __AxmMotGetOverrideLinePosMode(long lCoordNo, DWORD *dwpAbsRelMode);
			DWORD __AxmMoveStartPosEx(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel, double dEndVel);
			DWORD __AxmMovePosEx(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel, double dEndVel);
			DWORD __AxmMoveCoordStop(long lCoordNo, double dDecel);
			DWORD __AxmMoveCoordEStop(long lCoordNo);
			DWORD __AxmMoveCoordSStop(long lCoordNo);
			DWORD __AxmOverrideLinePos(long lCoordNo, double *dpOverridePos);
			DWORD __AxmOverrideLineVel(long lCoordNo, double dOverrideVel, double *dpDistance);

			DWORD __AxmOverrideLineAccelVelDecel(long lCoordNo, double dOverrideVelocity, double dMaxAccel, double dMaxDecel, double *dpDistance);
			DWORD __AxmOverrideAccelVelDecelAtPos(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel, double dOverridePos, double dOverrideVel, double dOverrideAccel, double dOverrideDecel, long lTarget);

			DWORD __AxmEGearSet(long lMasterAxisNo, long lSize, long* lpSlaveAxisNo, double* dpGearRatio);
			DWORD __AxmEGearGet(long lMasterAxisNo, long* lpSize, long* lpSlaveAxisNo, double* dpGearRatio);
			DWORD __AxmEGearReset(long lMasterAxisNo);
			DWORD __AxmEGearEnable(long lMasterAxisNo, DWORD dwEnable);
			DWORD __AxmEGearIsEnable(long lMasterAxisNo, DWORD *dwpEnable);
			DWORD __AxmMotSetEndVel(long lAxisNo, double dEndVelocity);
			DWORD __AxmMotGetEndVel(long lAxisNo, double *dpEndVelocity);

			virtual bool OnParameterChanged(_In_ int nParam, _In_ CString strValue) override;
			
			/**
			* 디바이스 초기화 시 하드웨어 초기화를 켜고 끕니다.
			*/
			virtual EGetFunction GetOpenNoHardware(_Out_ bool * pParam);
			virtual ESetFunction SetOpenNoHardware(_In_ bool bParam);

		public:
			DECLARE_MESSAGE_MAP()

		protected:
			virtual bool DoesModuleExist();

		protected:
			double m_dblLastVelocity = 0.;
		};
	}
}

#endif