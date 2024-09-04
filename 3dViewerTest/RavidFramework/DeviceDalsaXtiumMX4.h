#pragma once

#include "DeviceDalsaFrameGrabbers.h"

class SapAcquisition;
class SapBuffer;
class SapView;
class SapTransfer;
class SapXferCallbackInfo;

namespace Ravid
{
	namespace Algorithms
	{
		class CRavidImage;
	}

	namespace Device
	{
		class AFX_EXT_CLASS CDeviceDalsaXtiumMX4 : public CDeviceDalsaFrameGrabbers
		{
		public:
			RavidUseDynamicCreation();

			RavidPreventCopySelf(CDeviceDalsaXtiumMX4);

			DECLARE_DYNAMIC(CDeviceDalsaXtiumMX4)

		public:
			DECLARE_MESSAGE_MAP()

		public:
			CDeviceDalsaXtiumMX4();
			virtual ~CDeviceDalsaXtiumMX4();

			virtual EDeviceInitializeResult Initialize() override;
			virtual EDeviceTerminateResult Terminate() override;

			virtual bool LoadSettings() override;

			virtual EDeviceGrabResult Grab() override;
			virtual EDeviceLiveResult Live() override;
			virtual EDeviceStopResult Stop() override;

			virtual EDeviceTriggerResult Trigger();

			virtual bool OnParameterChanged(_In_ int nParam, _In_ CString strValue) override;

			virtual BOOL GetParameter(int param, void *pValue);
			virtual BOOL SetParameter(int param, int value, BOOL updateNow = TRUE);
			virtual BOOL SetParameter(int param, void *pValue, BOOL updateNow = TRUE);

			virtual void CreateImageBuffer(_In_ int nCount);

			virtual Algorithms::CRavidImage* GetImageInfo();

		protected:
#ifndef _WIN64
			static void WINAPI XferCallback(SapXferCallbackInfo *pInfo);
#else
			static void WINAPI XferCallback(SapXferCallbackInfo* pInfo);
#endif
			virtual bool DoesModuleExist();

		protected:
			int m_nDriveIndex = -1;

			SapTransfer* m_pXfer = nullptr;
			SapBuffer* m_pBuffers = nullptr;
			SapAcquisition* m_pDevice = nullptr;

			Ravid::Algorithms::CRavidImage** m_pImageBuffer = nullptr;
			int m_nCount = 0;
			int m_nImageCount = 2;
		};
	}
}

