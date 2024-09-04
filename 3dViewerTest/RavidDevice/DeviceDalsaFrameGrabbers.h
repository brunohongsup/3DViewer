#pragma once

#ifdef USING_DEVICE

#include "../RavidFramework/DeviceFrameGrabber.h"
#include "../RavidFramework/DefinitionsDevice.h"

namespace Ravid
{
	namespace Algorithms
	{
		class CRavidImage;
	}

	namespace Device
	{
		class CDeviceDalsaFrameGrabbers : public CDeviceFrameGrabber
		{
		public:
			RavidUseDynamicCreation();

			RavidPreventCopySelf(CDeviceDalsaFrameGrabbers);

			DECLARE_DYNAMIC(CDeviceDalsaFrameGrabbers)

		public:
			DECLARE_MESSAGE_MAP()

		public:
			CDeviceDalsaFrameGrabbers();
			virtual ~CDeviceDalsaFrameGrabbers();

			virtual EDeviceInitializeResult Initialize() override;
			virtual EDeviceTerminateResult Terminate() override;

			virtual bool LoadSettings() override;

			virtual EDeviceGrabResult Grab() override;
			virtual EDeviceLiveResult Live() override;
			virtual EDeviceStopResult Stop() override;

			virtual EDeviceTriggerResult Trigger();

			virtual EDalsaFrameGrabbersGetFunction GetCamfileName(_Out_ CString* pParam);
			virtual EDalsaFrameGrabbersSetFunction SetCamfileName(_In_ CString strParam);

			virtual bool OnParameterChanged(_In_ int nParam, _In_ CString strValue) override;

		protected:
			virtual bool DoesModuleExist();

		protected:			
			int m_nDriveIndex = -1;
		};
	}
}

#endif