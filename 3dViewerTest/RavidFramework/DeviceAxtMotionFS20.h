#pragma once

#include "DeviceMotion.h"

namespace Ravid
{
	namespace Device
	{
		class AFX_EXT_CLASS CDeviceAxtMotionFS20 : public CDeviceMotion
		{
		public:
			RavidUseDynamicCreation();

			RavidPreventCopySelf(CDeviceAxtMotionFS20);

			DECLARE_DYNAMIC(CDeviceAxtMotionFS20)

			CDeviceAxtMotionFS20();
			virtual ~CDeviceAxtMotionFS20();

			virtual EDeviceInitializeResult Initialize() override;
			virtual EDeviceTerminateResult Terminate() override;

			virtual bool LoadSettings() override;

		public:
			virtual EFS20GetFunction GetUnitPulse(_Out_ double* pParam);
			virtual EFS20SetFunction SetUnitPulse(_In_ double dblParam);

			virtual EFS20GetFunction GetStartSpeed(_Out_ double* pParam);
			virtual EFS20SetFunction SetStartSpeed(_In_ double dblParam);

			virtual EFS20GetFunction GetMaxSpeed(_Out_ double* pParam);
			virtual EFS20SetFunction SetMaxSpeed(_In_ double dblParam);

			virtual EFS20GetFunction GetPulseOutputMethod(_Out_ EDeviceAxtMotionFS20PulseOutputMethod* pParam);
			virtual EFS20SetFunction SetPulseOutputMethod(_In_ EDeviceAxtMotionFS20PulseOutputMethod eParam);

			virtual EFS20GetFunction GetEncoderMethod(_Out_ EDeviceAxtMotionFS20EncoderMethod* pParam);
			virtual EFS20SetFunction SetEncoderMethod(_In_ EDeviceAxtMotionFS20EncoderMethod eParam);

			virtual EFS20GetFunction GetPLimitLevel(_Out_ EDeviceAxtMotionFS20LevelMethod* pParam);
			virtual EFS20SetFunction SetPLimitLevel(_In_ EDeviceAxtMotionFS20LevelMethod eParam);

			virtual EFS20GetFunction GetNLimitLevel(_Out_ EDeviceAxtMotionFS20LevelMethod* pParam);
			virtual EFS20SetFunction SetNLimitLevel(_In_ EDeviceAxtMotionFS20LevelMethod eParam);

			virtual EFS20GetFunction GetAlarmInputLevel(_Out_ EDeviceAxtMotionFS20LevelMethod* pParam);
			virtual EFS20SetFunction SetAlarmInputLevel(_In_ EDeviceAxtMotionFS20LevelMethod eParam);

			virtual EFS20GetFunction GetInpositionInputLevel(_Out_ EDeviceAxtMotionFS20LevelMethod* pParam);
			virtual EFS20SetFunction SetInpositionInputLevel(_In_ EDeviceAxtMotionFS20LevelMethod eParam);

			virtual EFS20GetFunction GetHomesteps(_Out_ EDeviceAxtMotionFS20HomeSteps* pParam);
			virtual EFS20SetFunction SetHomesteps(_In_ EDeviceAxtMotionFS20HomeSteps eParam);

			virtual EFS20GetFunction GetStep1Stop(_Out_ EDeviceAxtMotionFS20StopMethod* pParam);
			virtual EFS20SetFunction SetStep1Stop(_In_ EDeviceAxtMotionFS20StopMethod eParam);

			virtual EFS20GetFunction GetStep1Direction(_Out_ EDeviceAxtMotionFS20DirectionMethod* pParam);
			virtual EFS20SetFunction SetStep1Direction(_In_ EDeviceAxtMotionFS20DirectionMethod eParam);

			virtual EFS20GetFunction GetStep1Detect(_Out_ EDeviceAxtMotionFS20DetectSignal* pParam);
			virtual EFS20SetFunction SetStep1Detect(_In_ EDeviceAxtMotionFS20DetectSignal eParam);

			virtual EFS20GetFunction GetStep1Velocity(_Out_ double* pParam);
			virtual EFS20SetFunction SetStep1Velocity(_In_ double dblParam);

			virtual EFS20GetFunction GetStep1Accel(_Out_ double* pParam);
			virtual EFS20SetFunction SetStep1Accel(_In_ double dblParam);

			virtual EFS20GetFunction GetStep2Stop(_Out_ EDeviceAxtMotionFS20StopMethod* pParam);
			virtual EFS20SetFunction SetStep2Stop(_In_ EDeviceAxtMotionFS20StopMethod eParam);

			virtual EFS20GetFunction GetStep2Direction(_Out_ EDeviceAxtMotionFS20DirectionMethod* pParam);
			virtual EFS20SetFunction SetStep2Direction(_In_ EDeviceAxtMotionFS20DirectionMethod eParam);

			virtual EFS20GetFunction GetStep2Detect(_Out_ EDeviceAxtMotionFS20DetectSignal* pParam);
			virtual EFS20SetFunction SetStep2Detect(_In_ EDeviceAxtMotionFS20DetectSignal eParam);

			virtual EFS20GetFunction GetStep2Velocity(_Out_ double* pParam);
			virtual EFS20SetFunction SetStep2Velocity(_In_ double dblParam);

			virtual EFS20GetFunction GetStep2Accel(_Out_ double* pParam);
			virtual EFS20SetFunction SetStep2Accel(_In_ double dblParam);

			virtual EFS20GetFunction GetStep3Stop(_Out_ EDeviceAxtMotionFS20StopMethod* pParam);
			virtual EFS20SetFunction SetStep3Stop(_In_ EDeviceAxtMotionFS20StopMethod eParam);

			virtual EFS20GetFunction GetStep3Direction(_Out_ EDeviceAxtMotionFS20DirectionMethod* pParam);
			virtual EFS20SetFunction SetStep3Direction(_In_ EDeviceAxtMotionFS20DirectionMethod eParam);

			virtual EFS20GetFunction GetStep3Detect(_Out_ EDeviceAxtMotionFS20DetectSignal* pParam);
			virtual EFS20SetFunction SetStep3Detect(_In_ EDeviceAxtMotionFS20DetectSignal eParam);

			virtual EFS20GetFunction GetStep3Velocity(_Out_ double* pParam);
			virtual EFS20SetFunction SetStep3Velocity(_In_ double dblParam);

			virtual EFS20GetFunction GetStep3Accel(_Out_ double* pParam);
			virtual EFS20SetFunction SetStep3Accel(_In_ double dblParam);

			virtual EFS20GetFunction GetStep4Stop(_Out_ EDeviceAxtMotionFS20StopMethod* pParam);
			virtual EFS20SetFunction SetStep4Stop(_In_ EDeviceAxtMotionFS20StopMethod eParam);

			virtual EFS20GetFunction GetStep4Direction(_Out_ EDeviceAxtMotionFS20DirectionMethod* pParam);
			virtual EFS20SetFunction SetStep4Direction(_In_ EDeviceAxtMotionFS20DirectionMethod eParam);

			virtual EFS20GetFunction GetStep4Detect(_Out_ EDeviceAxtMotionFS20DetectSignal* pParam);
			virtual EFS20SetFunction SetStep4Detect(_In_ EDeviceAxtMotionFS20DetectSignal eParam);

			virtual EFS20GetFunction GetStep4Velocity(_Out_ double* pParam);
			virtual EFS20SetFunction SetStep4Velocity(_In_ double dblParam);

			virtual EFS20GetFunction GetStep4Accel(_Out_ double* pParam);
			virtual EFS20SetFunction SetStep4Accel(_In_ double dblParam);

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

