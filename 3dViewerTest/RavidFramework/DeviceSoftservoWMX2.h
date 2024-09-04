#pragma once

#include "DeviceMotion.h"

namespace wmxapi
{
	class WMXLib;
}

namespace Ravid
{
	namespace Device
	{
		class AFX_EXT_CLASS CDeviceSoftservoWMX2 : public CDeviceMotion
		{
		public:
			RavidUseDynamicCreation();

			RavidPreventCopySelf(CDeviceSoftservoWMX2);

			DECLARE_DYNAMIC(CDeviceSoftservoWMX2)

			CDeviceSoftservoWMX2();
			virtual ~CDeviceSoftservoWMX2();

			virtual EDeviceInitializeResult Initialize() override;
			virtual EDeviceTerminateResult Terminate() override;

			virtual bool LoadSettings() override;

		public:
			virtual ESoftservoGetFunction GetMotionInfoPath(_Out_ CString* pParam);
			virtual ESoftservoSetFunction SetMotionInfoPath(_In_ CString strParam);

			virtual ESoftservoGetFunction GetUnitPulse(_Out_ double* pParam);
			virtual ESoftservoSetFunction SetUnitPulse(_In_ double dblParam);

			virtual ESoftservoGetFunction GetStartSpeed(_Out_ double* pParam);
			virtual ESoftservoSetFunction SetStartSpeed(_In_ double dblParam);

			virtual ESoftservoGetFunction GetEndSpeed(_Out_ double* pParam);
			virtual ESoftservoSetFunction SetEndSpeed(_In_ double dblParam);

			virtual ESoftservoGetFunction GetHomeLevel(_Out_ EDeviceSoftservoWMX2LevelMethod* pParam);
			virtual ESoftservoSetFunction SetHomeLevel(_In_ EDeviceSoftservoWMX2LevelMethod eParam);

			virtual ESoftservoGetFunction GetPolarityLimitLevel(_Out_ EDeviceSoftservoWMX2LevelMethod* pParam);
			virtual ESoftservoSetFunction SetPolarityLimitLevel(_In_ EDeviceSoftservoWMX2LevelMethod eParam);

			virtual ESoftservoGetFunction GetPLimitLevel(_Out_ EDeviceSoftservoWMX2LevelMethod* pParam);
			virtual ESoftservoSetFunction SetPLimitLevel(_In_ EDeviceSoftservoWMX2LevelMethod eParam);

			virtual ESoftservoGetFunction GetNLimitLevel(_Out_ EDeviceSoftservoWMX2LevelMethod* pParam);
			virtual ESoftservoSetFunction SetNLimitLevel(_In_ EDeviceSoftservoWMX2LevelMethod eParam);

			virtual ESoftservoGetFunction GetGearRatioDenominator(_Out_ DWORD* pParam);
			virtual ESoftservoSetFunction SetGearRatioDenominator(_In_ DWORD dwParam);

			virtual ESoftservoGetFunction GetGearRatioNumerator(_Out_ DWORD* pParam);
			virtual ESoftservoSetFunction SetGearRatioNumerator(_In_ DWORD dwParam);

			virtual ESoftservoGetFunction GetInposWidth(_Out_ long* pParam);
			virtual ESoftservoSetFunction SetInposWidth(_In_ long nParam);

			virtual ESoftservoGetFunction GetPosCompletedWidth(_Out_ long* pParam);
			virtual ESoftservoSetFunction SetPosCompletedWidth(_In_ long nParam);

			virtual ESoftservoGetFunction GetHomeType(_Out_ EDeviceSoftservoWMX2HomeType* pParam);
			virtual ESoftservoSetFunction SetHomeType(_In_ EDeviceSoftservoWMX2HomeType eParam);

			virtual ESoftservoGetFunction GetHomeDirection(_Out_ EDeviceSoftservoWMX2HomeDirection* pParam);
			virtual ESoftservoSetFunction SetHomeDirection(_In_ EDeviceSoftservoWMX2HomeDirection eParam);

			virtual ESoftservoGetFunction GetHomeFastVelocity(_Out_ double* pParam);
			virtual ESoftservoSetFunction SetHomeFastVelocity(_In_ double dblParam);

			virtual ESoftservoGetFunction GetHomeFastAccel(_Out_ double* pParam);
			virtual ESoftservoSetFunction SetHomeFastAccel(_In_ double dblParam);

			virtual ESoftservoGetFunction GetHomeFastDeceleration(_Out_ double* pParam);
			virtual ESoftservoSetFunction SetHomeFastDeceleration(_In_ double dblParam);

			virtual ESoftservoGetFunction GetHomeSlowVelocity(_Out_ double* pParam);
			virtual ESoftservoSetFunction SetHomeSlowVelocity(_In_ double dblParam);

			virtual ESoftservoGetFunction GetHomeSlowAccel(_Out_ double* pParam);
			virtual ESoftservoSetFunction SetHomeSlowAccel(_In_ double dblParam);

			virtual ESoftservoGetFunction GetHomeSlowDeceleration(_Out_ double* pParam);
			virtual ESoftservoSetFunction SetHomeSlowDeceleration(_In_ double dblParam);

			virtual ESoftservoGetFunction GetHomeShiftVelocity(_Out_ double* pParam);
			virtual ESoftservoSetFunction SetHomeShiftVelocity(_In_ double dblParam);

			virtual ESoftservoGetFunction GetHomeShiftAccel(_Out_ double* pParam);
			virtual ESoftservoSetFunction SetHomeShiftAccel(_In_ double dblParam);

			virtual ESoftservoGetFunction GetHomeShiftDeceleration(_Out_ double* pParam);
			virtual ESoftservoSetFunction SetHomeShiftDeceleration(_In_ double dblParam);

			virtual ESoftservoGetFunction GetHomeShiftDistance(_Out_ double* pParam);
			virtual ESoftservoSetFunction SetHomeShiftDistance(_In_ double dblParam);

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
			
			wmxapi::WMXLib* GetDevice();

			virtual bool OnParameterChanged(_In_ int nParam, _In_ CString strValue) override;

		public:
			DECLARE_MESSAGE_MAP()

		protected:
			virtual bool DoesModuleExist();

		protected:
			double m_dblLastVelocity = 0.;

			wmxapi::WMXLib* m_pLibModule = nullptr;
		};
	}
}

