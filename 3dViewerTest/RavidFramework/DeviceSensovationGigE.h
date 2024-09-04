#pragma once

#include "DeviceCamera.h"

typedef INT64 HSVCAM;
typedef INT64 HSVIMG;

namespace Ravid
{
	namespace Algorithms
	{
		class CRavidImage;
	}

	namespace Device
	{
		class AFX_EXT_CLASS CDeviceSensovationGigE : public CDeviceCamera
		{
		public:
			RavidUseDynamicCreation();

			RavidPreventCopySelf(CDeviceSensovationGigE);

			DECLARE_DYNAMIC(CDeviceSensovationGigE)

		public:
			CDeviceSensovationGigE();
			virtual ~CDeviceSensovationGigE();

			virtual EDeviceInitializeResult Initialize() override;
			
			virtual EDeviceTerminateResult Terminate() override;

			virtual bool LoadSettings() override;

			virtual EDeviceGrabResult Grab() override;
			virtual EDeviceLiveResult Live() override;
			virtual EDeviceStopResult Stop() override;

			virtual EDeviceTriggerResult Trigger();
			
		public:
			virtual ESensovationGetFunction GetGrabCount(_Out_ int* pParam);
			virtual ESensovationSetFunction SetGrabCount(_In_ int nParam);

			virtual ESensovationGetFunction GetOffsetX(_Out_ int* pParam);
			virtual ESensovationSetFunction SetOffsetX(_In_ int nParam);

			virtual ESensovationGetFunction GetOffsetY(_Out_ int* pParam);
			virtual ESensovationSetFunction SetOffsetY(_In_ int nParam);

			virtual ESensovationGetFunction GetCanvasWidth(_Out_ int* pParam);
			virtual ESensovationSetFunction SetCanvasWidth(_In_ int nParam);

			virtual ESensovationGetFunction GetCanvasHeight(_Out_ int* pParam);
			virtual ESensovationSetFunction SetCanvasHeight(_In_ int nParam);

			virtual ESensovationGetFunction GetBinning(_Out_ EDeviceSensovationGigEBinning* pParam);
			virtual ESensovationSetFunction SetBinning(_In_ EDeviceSensovationGigEBinning eParam);

			virtual ESensovationGetFunction GetRealWidth(_Out_ int* pParam);
			virtual ESensovationSetFunction SetRealWidth(_In_ int nParam);
											   
			virtual ESensovationGetFunction GetRealHeight(_Out_ int* pParam);
			virtual ESensovationSetFunction SetRealHeight(_In_ int nParam);

			virtual ESensovationGetFunction GetExposureTime(_Out_ double* pParam);
			virtual ESensovationSetFunction SetExposureTime(_In_ double dblParam);

			virtual ESensovationGetFunction GetGain(_Out_ double* pParam);
			virtual ESensovationSetFunction SetGain(_In_ double dblParam);

			virtual ESensovationGetFunction GetTriggerMode(_Out_ EDeviceSensovationGigETriggerMode* pParam);
			virtual ESensovationSetFunction SetTriggerMode(_In_ EDeviceSensovationGigETriggerMode eParam);

			virtual bool OnParameterChanged(_In_ int nParam, _In_ CString strValue) override;

		public:
			DECLARE_MESSAGE_MAP()

		protected:
			virtual bool DoesModuleExist();

			static UINT LiveThread(_In_ LPVOID pParam);

		protected:
			bool m_bLoadLib = false;

			static int m_nCameraCount;

			unsigned long m_nGrabCount = 0;

			volatile int32_t m_nWaitTime = 0;

			CWinThread* m_pLiveThread = nullptr;

			HSVIMG m_hImage = 0;
			HSVCAM m_hCamera = 0;
		};
	}
}