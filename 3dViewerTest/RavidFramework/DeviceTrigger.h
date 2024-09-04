#pragma once

#include "DeviceBase.h"

namespace Ravid
{
	namespace Miscellaneous
	{
		class CPerformanceCounter;
	}

	namespace Device
	{
		/**
		* 트리거 디바이스 계열의 기저 클래스입니다.
		*/
		class AFX_EXT_CLASS CDeviceTrigger : public CDeviceBase
		{
		public:
			CDeviceTrigger();
			virtual ~CDeviceTrigger();

			virtual ptrdiff_t OnMessage(_In_ Ravid::Framework::CMessageBase* pMessage);

			virtual EDeviceInitializeResult Initialize() override = 0;
			virtual EDeviceTerminateResult Terminate() override = 0;

			virtual bool EnableTrigger() = 0;
			virtual bool DisableTrigger() = 0;

			virtual bool IsTriggerEnabled() = 0;

			virtual long GetEncoderCount() = 0;
			virtual long GetTriggerCount() = 0;
			virtual bool ResetCounter() = 0;

		protected:
			virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

			virtual bool DisplayCounter();

			virtual bool AddControls();

		public:
			virtual BOOL OnInitDialog();
			afx_msg void OnDestroy();
			afx_msg void OnTimer(UINT_PTR nIDEvent);

		protected:
			long m_nBeforeEncoderValue = 0;
			long m_nBeforeTriggerValue = 0;

			Ravid::Miscellaneous::CPerformanceCounter* m_pRpc = nullptr;
		};
	}
}

