#pragma once

#include "DeviceMotion.h"

namespace Ravid
{
	namespace Device
	{
		class AFX_EXT_CLASS CDeviceEziServo2PlusE : public CDeviceMotion
		{
		public:
			RavidUseDynamicCreation();

			RavidPreventCopySelf(CDeviceEziServo2PlusE);

			DECLARE_DYNAMIC(CDeviceEziServo2PlusE)

			CDeviceEziServo2PlusE();
			virtual ~CDeviceEziServo2PlusE();

			virtual ptrdiff_t OnMessage(_In_ Ravid::Framework::CMessageBase* pMessage);

			virtual EDeviceInitializeResult Initialize() override;
			virtual EDeviceTerminateResult Terminate() override;

			virtual bool LoadSettings() override;

			EEziServo2PlusEGetFunction GetIPAddress(_Out_ CString* pParam);
			EEziServo2PlusESetFunction SetIPAddress(_In_ CString strParam);

			EEziServo2PlusEGetFunction GetMotionType(_Out_ EEziServo2PlusEMotionType* pParam);
			EEziServo2PlusESetFunction SetMotionType(_In_ EEziServo2PlusEMotionType eParam);

			EEziServo2PlusEGetFunction GetServo2PulsePerRevolution(_Out_ int* pParam);
			EEziServo2PlusESetFunction SetServo2PulsePerRevolution(_In_ int nParam);

			EEziServo2PlusEGetFunction GetServo2AxisMaxSpeed(_Out_ int* pParam);
			EEziServo2PlusESetFunction SetServo2AxisMaxSpeed(_In_ int nParam);

			EEziServo2PlusEGetFunction GetServo2AxisStartSpeed(_Out_ int* pParam);
			EEziServo2PlusESetFunction SetServo2AxisStartSpeed(_In_ int nParam);

			EEziServo2PlusEGetFunction GetServo2AxisAccTime(_Out_ int* pParam);
			EEziServo2PlusESetFunction SetServo2AxisAccTime(_In_ int nParam);	

			EEziServo2PlusEGetFunction GetServo2AxisDecTime(_Out_ int* pParam);
			EEziServo2PlusESetFunction SetServo2AxisDecTime(_In_ int nParam);	

			EEziServo2PlusEGetFunction GetServo2SpeedOverride(_Out_ int* pParam);
			EEziServo2PlusESetFunction SetServo2SpeedOverride(_In_ int nParam);

			EEziServo2PlusEGetFunction GetServo2JogSpeed(_Out_ int* pParam);
			EEziServo2PlusESetFunction SetServo2JogSpeed(_In_ int nParam);

			EEziServo2PlusEGetFunction GetServo2JogStartSpeed(_Out_ int* pParam);
			EEziServo2PlusESetFunction SetServo2JogStartSpeed(_In_ int nParam);

			EEziServo2PlusEGetFunction GetServo2JogAccDecTime(_Out_ int* pParam);
			EEziServo2PlusESetFunction SetServo2JogAccDecTime(_In_ int nParam);

			EEziServo2PlusEGetFunction GetServo2SWLimitPlusValue(_Out_ int* pParam);
			EEziServo2PlusESetFunction SetServo2SWLimitPlusValue(_In_ int nParam);

			EEziServo2PlusEGetFunction GetServo2SWLimitMinusValue(_Out_ int* pParam);
			EEziServo2PlusESetFunction SetServo2SWLimitMinusValue(_In_ int nParam);

			EEziServo2PlusEGetFunction GetServo2SWLimitStopMethod(_Out_ EEziServo2PlusESWLimitStopMethod* pParam);
			EEziServo2PlusESetFunction SetServo2SWLimitStopMethod(_In_ EEziServo2PlusESWLimitStopMethod eParam);

			EEziServo2PlusEGetFunction GetServo2HWLimitStopMethod(_Out_ EEziServo2PlusEHWLimitStopMethod* pParam);
			EEziServo2PlusESetFunction SetServo2HWLimitStopMethod(_In_ EEziServo2PlusEHWLimitStopMethod eParam);

			EEziServo2PlusEGetFunction GetServo2LimitSensorLogic(_Out_ EEziServo2PlusELogic* pParam);
			EEziServo2PlusESetFunction SetServo2LimitSensorLogic(_In_ EEziServo2PlusELogic eParam);

			EEziServo2PlusEGetFunction GetServo2OrgSpeed(_Out_ int* pParam);
			EEziServo2PlusESetFunction SetServo2OrgSpeed(_In_ int nParam);

			EEziServo2PlusEGetFunction GetServo2OrgSearchSpeed(_Out_ int* pParam);
			EEziServo2PlusESetFunction SetServo2OrgSearchSpeed(_In_ int nParam);

			EEziServo2PlusEGetFunction GetServo2OrgAccDecTime(_Out_ int* pParam);
			EEziServo2PlusESetFunction SetServo2OrgAccDecTime(_In_ int nParam);

			EEziServo2PlusEGetFunction GetServo2OrgMethod(_Out_ EEziServo2PlusEOrgMethod* pParam);
			EEziServo2PlusESetFunction SetServo2OrgMethod(_In_ EEziServo2PlusEOrgMethod eParam);

			EEziServo2PlusEGetFunction GetServo2OrgDir(_Out_ EEziServo2PlusEDir* pParam);
			EEziServo2PlusESetFunction SetServo2OrgDir(_In_ EEziServo2PlusEDir eParam);

			EEziServo2PlusEGetFunction GetServo2OrgOffSet(_Out_ int* pParam);
			EEziServo2PlusESetFunction SetServo2OrgOffSet(_In_ int nParam);

			EEziServo2PlusEGetFunction GetServo2OrgPositionSet(_Out_ int* pParam);
			EEziServo2PlusESetFunction SetServo2OrgPositionSet(_In_ int nParam);

			EEziServo2PlusEGetFunction GetServo2OrgSensorLogic(_Out_ EEziServo2PlusELogic* pParam);
			EEziServo2PlusESetFunction SetServo2OrgSensorLogic(_In_ EEziServo2PlusELogic eParam);

			EEziServo2PlusEGetFunction GetServo2PositionLoopGain(_Out_ int* pParam);
			EEziServo2PlusESetFunction SetServo2PositionLoopGain(_In_ int nParam);

			EEziServo2PlusEGetFunction GetServo2InposValue(_Out_ int* pParam);
			EEziServo2PlusESetFunction SetServo2InposValue(_In_ int nParam);

			EEziServo2PlusEGetFunction GetServo2PosTrackingLimit(_Out_ int* pParam);
			EEziServo2PlusESetFunction SetServo2PosTrackingLimit(_In_ int nParam);

			EEziServo2PlusEGetFunction GetServo2MotionDir(_Out_ EEziServo2PlusEDir* pParam);
			EEziServo2PlusESetFunction SetServo2MotionDir(_In_ EEziServo2PlusEDir eParam);

			EEziServo2PlusEGetFunction GetServo2LimitSensorDir(_Out_ EEziServo2PlusEDir* pParam);
			EEziServo2PlusESetFunction SetServo2LimitSensorDir(_In_ EEziServo2PlusEDir eParam);

			EEziServo2PlusEGetFunction GetServo2OrgTorqueRatio(_Out_ int* pParam);
			EEziServo2PlusESetFunction SetServo2OrgTorqueRatio(_In_ int nParam);

			EEziServo2PlusEGetFunction GetServo2PosErrorOverflowLimit(_Out_ int* pParam);
			EEziServo2PlusESetFunction SetServo2PosErrorOverflowLimit(_In_ int nParam);

			EEziServo2PlusEGetFunction GetServo2BrakeDelayTime(_Out_ int* pParam);
			EEziServo2PlusESetFunction SetServo2BrakeDelayTime(_In_ int nParam);


			EEziServo2PlusEGetFunction GetMotionLink2EncoderMultiply(_Out_ EEziServo2PlusEEncoderMethod* pParam);
			EEziServo2PlusESetFunction SetMotionLink2EncoderMultiply(_In_ EEziServo2PlusEEncoderMethod eParam);

			EEziServo2PlusEGetFunction GetMotionLink2AxisMaxSpeed(_Out_ int* pParam);
			EEziServo2PlusESetFunction SetMotionLink2AxisMaxSpeed(_In_ int nParam);

			EEziServo2PlusEGetFunction GetMotionLink2AxisStartSpeed(_Out_ int* pParam);
			EEziServo2PlusESetFunction SetMotionLink2AxisStartSpeed(_In_ int nParam);

			EEziServo2PlusEGetFunction GetMotionLink2AxisAccTime(_Out_ int* pParam);
			EEziServo2PlusESetFunction SetMotionLink2AxisAccTime(_In_ int nParam);

			EEziServo2PlusEGetFunction GetMotionLink2AxisDecTime(_Out_ int* pParam);
			EEziServo2PlusESetFunction SetMotionLink2AxisDecTime(_In_ int nParam);

			EEziServo2PlusEGetFunction GetMotionLink2SpeedOverride(_Out_ int* pParam);
			EEziServo2PlusESetFunction SetMotionLink2SpeedOverride(_In_ int nParam);

			EEziServo2PlusEGetFunction GetMotionLink2JogSpeed(_Out_ int* pParam);
			EEziServo2PlusESetFunction SetMotionLink2JogSpeed(_In_ int nParam);

			EEziServo2PlusEGetFunction GetMotionLink2JogStartSpeed(_Out_ int* pParam);
			EEziServo2PlusESetFunction SetMotionLink2JogStartSpeed(_In_ int nParam);

			EEziServo2PlusEGetFunction GetMotionLink2JogAccDecTime(_Out_ int* pParam);
			EEziServo2PlusESetFunction SetMotionLink2JogAccDecTime(_In_ int nParam);

			EEziServo2PlusEGetFunction GetMotionLink2SWLimitPlusValue(_Out_ int* pParam);
			EEziServo2PlusESetFunction SetMotionLink2SWLimitPlusValue(_In_ int nParam);

			EEziServo2PlusEGetFunction GetMotionLink2SWLimitMinusValue(_Out_ int* pParam);
			EEziServo2PlusESetFunction SetMotionLink2SWLimitMinusValue(_In_ int nParam);

			EEziServo2PlusEGetFunction GetMotionLink2SWLimitStopMethod(_Out_ EEziServo2PlusESWLimitStopMethod* pParam);
			EEziServo2PlusESetFunction SetMotionLink2SWLimitStopMethod(_In_ EEziServo2PlusESWLimitStopMethod eParam);

			EEziServo2PlusEGetFunction GetMotionLink2HWLimitStopMethod(_Out_ EEziServo2PlusESWLimitStopMethod* pParam);
			EEziServo2PlusESetFunction SetMotionLink2HWLimitStopMethod(_In_ EEziServo2PlusESWLimitStopMethod eParam);

			EEziServo2PlusEGetFunction GetMotionLink2LimitSensorLogic(_Out_ EEziServo2PlusELogic* pParam);
			EEziServo2PlusESetFunction SetMotionLink2LimitSensorLogic(_In_ EEziServo2PlusELogic eParam);

			EEziServo2PlusEGetFunction GetMotionLink2OrgSpeed(_Out_ int* pParam);
			EEziServo2PlusESetFunction SetMotionLink2OrgSpeed(_In_ int nParam);

			EEziServo2PlusEGetFunction GetMotionLink2OrgSearchSpeed(_Out_ int* pParam);
			EEziServo2PlusESetFunction SetMotionLink2OrgSearchSpeed(_In_ int nParam);

			EEziServo2PlusEGetFunction GetMotionLink2OrgAccDecTime(_Out_ int* pParam);
			EEziServo2PlusESetFunction SetMotionLink2OrgAccDecTime(_In_ int nParam);

			EEziServo2PlusEGetFunction GetMotionLink2OrgMethod(_Out_ EEziServo2PlusEMotionLinek2OrgMethod* pParam);
			EEziServo2PlusESetFunction SetMotionLink2OrgMethod(_In_ EEziServo2PlusEMotionLinek2OrgMethod eParam);

			EEziServo2PlusEGetFunction GetMotionLink2OrgDir(_Out_ EEziServo2PlusEDir* pParam);
			EEziServo2PlusESetFunction SetMotionLink2OrgDir(_In_ EEziServo2PlusEDir eParam);

			EEziServo2PlusEGetFunction GetMotionLink2OrgOffSet(_Out_ int* pParam);
			EEziServo2PlusESetFunction SetMotionLink2OrgOffSet(_In_ int nParam);

			EEziServo2PlusEGetFunction GetMotionLink2OrgPositionSet(_Out_ int* pParam);
			EEziServo2PlusESetFunction SetMotionLink2OrgPositionSet(_In_ int nParam);

			EEziServo2PlusEGetFunction GetMotionLink2OrgSensorLogic(_Out_ EEziServo2PlusELogic* pParam);
			EEziServo2PlusESetFunction SetMotionLink2OrgSensorLogic(_In_ EEziServo2PlusELogic eParam);

			EEziServo2PlusEGetFunction GetMotionLink2LimitSensorDir(_Out_ EEziServo2PlusEDir* pParam);
			EEziServo2PlusESetFunction SetMotionLink2LimitSensorDir(_In_ EEziServo2PlusEDir eParam);

			EEziServo2PlusEGetFunction GetMotionLink2PulseType(_Out_ EEziServo2PlusEPulseType* pParam);
			EEziServo2PlusESetFunction SetMotionLink2PulseType(_In_ EEziServo2PlusEPulseType eParam);

			EEziServo2PlusEGetFunction GetMotionLink2EncoderDir(_Out_ EEziServo2PlusEDir* pParam);
			EEziServo2PlusESetFunction SetMotionLink2EncoderDir(_In_ EEziServo2PlusEDir eParam);

			EEziServo2PlusEGetFunction GetMotionLink2MotionDir(_Out_ EEziServo2PlusEDir* pParam);
			EEziServo2PlusESetFunction SetMotionLink2MotionDir(_In_ EEziServo2PlusEDir eParam);

			EEziServo2PlusEGetFunction GetMotionLink2ServoAlarmresetLogic(_Out_ EEziServo2PlusELogic* pParam);
			EEziServo2PlusESetFunction SetMotionLink2ServoAlarmresetLogic(_In_ EEziServo2PlusELogic eParam);

			EEziServo2PlusEGetFunction GetMotionLink2ServoOnOutputLogic(_Out_ EEziServo2PlusELogic* pParam);
			EEziServo2PlusESetFunction SetMotionLink2ServoOnOutputLogic(_In_ EEziServo2PlusELogic eParam);

			EEziServo2PlusEGetFunction GetMotionLink2ServoAlarmLogic(_Out_ EEziServo2PlusELogic* pParam);
			EEziServo2PlusESetFunction SetMotionLink2ServoAlarmLogic(_In_ EEziServo2PlusELogic eParam);

			EEziServo2PlusEGetFunction GetMotionLink2ServoInpositionLogic(_Out_ EEziServo2PlusELogic* pParam);
			EEziServo2PlusESetFunction SetMotionLink2ServoInpositionLogic(_In_ EEziServo2PlusELogic eParam);

		public:
			
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

			virtual long GetOutputPortCount();
			virtual long GetInputPortCount();

			virtual bool GetTriggerOutputStatus();

			virtual bool EnableTriggerOutput(_In_ long lStartPos, _In_ DWORD dwPeriod, _In_ DWORD dwPulseWidth);
			virtual bool DisableTriggerOutput();

			virtual bool OnParameterChanged(_In_ int nParam, _In_ CString strValue) override;

		public:
			DECLARE_MESSAGE_MAP()

			void OnBnClickedOutLed(_In_ UINT nID);

			virtual BOOL OnInitDialog();
			afx_msg void OnTimer(_In_ UINT_PTR nIDEvent);
			afx_msg void OnDestroy();

		protected:
			virtual void AllocateIO(_In_ long nInputSize, _In_ long nOutputSize);
			virtual void FreeIO();

			virtual bool AddControls();

			virtual bool CreateMotionControl(_In_ double dblDPIScale, _In_ CFont* pBaseFont, _In_ CRavidPoint<int> rpLeftTopPt, _In_ CRavidPoint<int> rpRightTopPt, _Out_ CRavidRect<int>& rrResultGroupBox);

			virtual bool CreateLedArray(_In_ double dblDPIScale, _In_ CFont* pBaseFont, _In_ CRavidPoint<int> rpLeftTopPt, _In_ CRavidPoint<int> rpRightTopPt, _Out_ CRavidRect<int>& rrResultGroupBox);

			virtual bool DoesModuleExist();

		protected:
			bool* m_pBUsingInputArray = nullptr;
			bool* m_pBUsingOutputArray = nullptr;

			CString* m_pStrInputNameArray = nullptr;
			CString* m_pStrOutputNameArray = nullptr;

			EEziServo2PlusEMotionType m_eCurrentMode = EEziServo2PlusEMotionType_Count;
		};
	}
}

