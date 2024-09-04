#pragma once

#include "DeviceBase.h"

namespace Ravid
{
	namespace Device
	{
		/**
		* 모션 디바이스 계열의 기저 클래스입니다.
		*/
		class AFX_EXT_CLASS CDeviceMotion : public CDeviceBase
		{
		public:
			CDeviceMotion();
			virtual ~CDeviceMotion();

			virtual ptrdiff_t OnMessage(_In_ Ravid::Framework::CMessageBase* pMessage);

			virtual EDeviceInitializeResult Initialize() override = 0;
			virtual EDeviceTerminateResult Terminate() override = 0;

			virtual bool IsServoOn();

			virtual bool GetServoStatus() = 0;
			virtual bool SetServoOn(_In_ bool bOn) = 0;

			virtual bool GetCommandPosition(_Out_ double *pPos) = 0;
			virtual bool SetCommandPositionClear() = 0;

			virtual bool GetActualPosition(_Out_ double *pPos) = 0;
			virtual bool SetActualPositionClear() = 0;

			virtual bool GetAlarmStatus() = 0;
			virtual bool SetAlarmClear() = 0;

			virtual bool GetInposition() = 0;
			virtual bool GetLimitSensorN() = 0;
			virtual bool GetLimitSensorP() = 0;
			virtual bool GetHomeSensor() = 0;

			virtual bool MovePosition(_In_ double dPos, _In_ double dVel, _In_ double dAcc, _In_ bool bSCurve) = 0;
			virtual bool MoveDistance(_In_ double dDist, _In_ double dVel, _In_ double dAcc, _In_ bool bSCurve) = 0;
			virtual bool MoveVelocity(_In_ double dVel, _In_ double dAcc, _In_ bool bSCurve) = 0;

			virtual bool MoveJog(_In_ double dVel, _In_ double dAcl, _In_ BOOL bSCurve) = 0;
			virtual bool StopJog() = 0;

			virtual bool IsMotionDone() = 0;

			virtual bool MotorStop() = 0;
			virtual bool MotorEStop() = 0;

			virtual bool ChangeSpeed(_In_ double dSpeed) = 0;

			virtual bool WriteGenOutBit(_In_ int nBit, _In_ bool bOn) = 0;
			virtual bool ReadGenOutBit(_In_ int nBit) = 0;
			virtual bool ReadGenInputBit(_In_ int nBit) = 0;
						 
			virtual bool MoveToHome() = 0;

			virtual EDeviceMotionHommingStatus GetHommingStatus() = 0;

			virtual double GetVelocity();
			virtual void SetVelocity(_In_ double dblVel);

			virtual double GetAccel();
			virtual void SetAccel(_In_ double dblVel);

			virtual double GetDistance();
			virtual void SetDistance(_In_ double dblVel);

			virtual double GetPosition();
			virtual void SetPosition(_In_ double dblVel);

			virtual BOOL PreTranslateMessage(MSG* pMsg);

			afx_msg void OnBnClickedRadioJog();
			afx_msg void OnBnClickedRadioMove();
			afx_msg void OnBnClickedRadioPosition();
			afx_msg void OnBnClickedBtnMotionForward();
			afx_msg void OnBnClickedBtnMotionStop();
			afx_msg void OnBnClickedBtnMotionBackward();
			afx_msg void OnBnClickedBtnServoModeOn();
			afx_msg void OnBnClickedBtnServoModeOff();
			afx_msg void OnBnClickedBtnMoveToHome();
			afx_msg void OnBnClickedButtonMotionCommandPositionClear();
			afx_msg void OnBnClickedButtonMotionActualPositionClear();
			afx_msg void OnBnClickedButtonMotionAlarmClear();

		public:
			DECLARE_MESSAGE_MAP()

		protected:
			virtual bool AddControls();

			virtual bool DisplayCounter();

		public:
			virtual BOOL OnInitDialog();
			afx_msg void OnDestroy();
			afx_msg void OnTimer(UINT_PTR nIDEvent);

		protected:
			volatile bool m_bServo;

			volatile bool m_bIsRunningCheckingThread;
		};
	}
}