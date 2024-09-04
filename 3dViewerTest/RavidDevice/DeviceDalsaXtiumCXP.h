#pragma once

#ifdef USING_DEVICE

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
		class CDeviceDalsaXtiumCXP : public CDeviceDalsaFrameGrabbers
		{
		public:
			RavidUseDynamicCreation();

			RavidPreventCopySelf(CDeviceDalsaXtiumCXP);

			DECLARE_DYNAMIC(CDeviceDalsaXtiumCXP)

		public:
			DECLARE_MESSAGE_MAP()

		public:
			CDeviceDalsaXtiumCXP();
			virtual ~CDeviceDalsaXtiumCXP();

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

		protected:
#ifndef _WIN64
			static void WINAPI CallbackFunction(SapXferCallbackInfo *pInfo);
#else
			static void WINAPI CallbackFunction(SapXferCallbackInfo* pInfo);
#endif
			virtual bool DoesModuleExist();

		protected:
			int m_nDriveIndex = -1;

			SapTransfer* m_pXfer = nullptr;
			SapBuffer* m_pBuffers = nullptr;
			SapAcquisition* m_pDevice = nullptr;
		};
	}
}

#endif