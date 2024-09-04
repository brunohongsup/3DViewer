#pragma once

#include "DeviceFrameGrabber.h"

typedef UINT32 MCHANDLE, *PMCHANDLE;

typedef struct _MC_CALLBACK_INFO MCSIGNALINFO;
typedef struct _MC_CALLBACK_INFO *PMCSIGNALINFO;
typedef struct _MC_CALLBACK_INFO *PMCCALLBACKINFO;
typedef struct _MC_CALLBACK_INFO MCCALLBACKINFO;

namespace Ravid
{
	namespace Algorithms
	{
		class CRavidImage;
	}

	namespace Device
	{
		class AFX_EXT_CLASS CDeviceEuresysMulticam : public CDeviceFrameGrabber
		{
		public:
			RavidUseDynamicCreation();

			RavidPreventCopySelf(CDeviceEuresysMulticam);

			DECLARE_DYNAMIC(CDeviceEuresysMulticam)

		public:
			CDeviceEuresysMulticam();
			virtual ~CDeviceEuresysMulticam();

			virtual EDeviceInitializeResult Initialize() override;
			virtual EDeviceTerminateResult Terminate() override;

			virtual bool LoadSettings() override;

			virtual EDeviceGrabResult Grab() override;
			virtual EDeviceLiveResult Live() override;
			virtual EDeviceStopResult Stop() override;

			virtual EDeviceTriggerResult Trigger();

			virtual EEuresysGetFunction GetInitializetype(_Out_ EDeviceMulticamInitializeType* pParam);
			virtual EEuresysSetFunction SetInitializetype(_In_ EDeviceMulticamInitializeType eParam);

			virtual EEuresysGetFunction GetCamfilePath(_Out_ CString* pParam);
			virtual EEuresysSetFunction SetCamfilePath(_In_ CString strParam);

			virtual EEuresysGetFunction GetCamfileMatch(_Out_ bool* pParam);
			virtual EEuresysSetFunction SetCamfileMatch(_In_ bool bParam);

			virtual bool OnParameterChanged(_In_ int nParam, _In_ CString strValue) override;

		protected:
			virtual bool DoesModuleExist();

		protected:
			bool m_bIsLine = false;				//line scan camera?
			bool m_bIsColor = false;			//color format?

			int m_nDriveIndex = -1;

			volatile int m_nCurrentCount = 1;	//grabcount
			volatile int m_nConnectBoard = 0;	//boardcount

			MCHANDLE m_hDevice = -1;
		};
	}
}

