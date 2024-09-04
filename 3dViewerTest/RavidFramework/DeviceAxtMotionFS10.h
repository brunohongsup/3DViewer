#pragma once

#include "DeviceMotion.h"

namespace Ravid
{
	namespace Device
	{
		class AFX_EXT_CLASS CDeviceAxtMotionFS10 : public CDeviceMotion
		{
		public:
			RavidUseDynamicCreation();

			RavidPreventCopySelf(CDeviceAxtMotionFS10);

			DECLARE_DYNAMIC(CDeviceAxtMotionFS10)

			CDeviceAxtMotionFS10();
			virtual ~CDeviceAxtMotionFS10();

			virtual EDeviceInitializeResult Initialize() override;
			virtual EDeviceTerminateResult Terminate() override;

			virtual bool LoadSettings() override;

		public:
			virtual EFS10GetFunction GetUnitPulse(_Out_ double* pData);
			virtual EFS10SetFunction SetUnitPulse(_In_ double dblData);

			virtual EFS10GetFunction GetStartSpeed(_Out_ double* pData);
			virtual EFS10SetFunction SetStartSpeed(_In_ double dblData);

			virtual EFS10GetFunction GetMaxSpeed(_Out_ double* pData);
			virtual EFS10SetFunction SetMaxSpeed(_In_ double dblData);

			virtual EFS10GetFunction GetPulseOutputMethod(_Out_ EDeviceAxtMotionFS10PulseOutputMethod* pEnum);
			virtual EFS10SetFunction SetPulseOutputMethod(_In_ EDeviceAxtMotionFS10PulseOutputMethod eEnum);

			virtual EFS10GetFunction GetEncoderMethod(_Out_ EDeviceAxtMotionFS10EncoderMethod* pEnum);
			virtual EFS10SetFunction SetEncoderMethod(_In_ EDeviceAxtMotionFS10EncoderMethod eEnum);

			virtual EFS10GetFunction GetPLimitLevel(_Out_ EDeviceAxtMotionFS10LevelMethod* pEnum);
			virtual EFS10SetFunction SetPLimitLevel(_In_ EDeviceAxtMotionFS10LevelMethod eEnum);

			virtual EFS10GetFunction GetNLimitLevel(_Out_ EDeviceAxtMotionFS10LevelMethod* pEnum);
			virtual EFS10SetFunction SetNLimitLevel(_In_ EDeviceAxtMotionFS10LevelMethod eEnum);

			virtual EFS10GetFunction GetAlarmInputLevel(_Out_ EDeviceAxtMotionFS10LevelMethod* pEnum);
			virtual EFS10SetFunction SetAlarmInputLevel(_In_ EDeviceAxtMotionFS10LevelMethod eEnum);

			virtual EFS10GetFunction GetInpositionInputLevel(_Out_ EDeviceAxtMotionFS10LevelMethod* pEnum);
			virtual EFS10SetFunction SetInpositionInputLevel(_In_ EDeviceAxtMotionFS10LevelMethod eEnum);
			
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

			virtual bool OnParameterChanged(_In_ int nParam, _In_ CString strValue) override;
			
		public:
			DECLARE_MESSAGE_MAP()

		protected:
			virtual bool DoesModuleExist();
		};
	}
}

