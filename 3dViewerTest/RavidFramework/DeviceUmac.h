#pragma once

#include "DeviceMotion.h"

namespace Ravid
{
	namespace Algorithms
	{
		class CRavidImage;
	}

	namespace Device
	{
		class AFX_EXT_CLASS CDeviceUmac : public CDeviceMotion
		{
		public:
			RavidUseDynamicCreation();

			DECLARE_DYNAMIC(CDeviceUmac)

		public:
			CDeviceUmac();
			virtual ~CDeviceUmac();

			virtual EDeviceInitializeResult Initialize() override;
			virtual EDeviceTerminateResult Terminate() override;

			virtual bool LoadSettings() override;

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
			virtual bool UpdateControls() override;

			virtual bool DoesModuleExist();
		};
	}
}
