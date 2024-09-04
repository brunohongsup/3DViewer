#pragma once

#include "DeviceCamera.h"

class PvDevice;
class PvStream;
class PvPipeline;

namespace Ravid
{
	namespace Algorithms
	{
		class CRavidImage;
	}

	namespace Device
	{
		class AFX_EXT_CLASS CDeviceMicroview : public CDeviceCamera
		{
		public:
			RavidUseDynamicCreation();

			RavidPreventCopySelf(CDeviceMicroview);

			DECLARE_DYNAMIC(CDeviceMicroview)

		public:
			CDeviceMicroview();
			virtual ~CDeviceMicroview();

			virtual EDeviceInitializeResult Initialize() override;
			virtual EDeviceTerminateResult Terminate() override;

			virtual bool LoadSettings() override;

			virtual EDeviceGrabResult Grab() override;
			virtual EDeviceLiveResult Live() override;
			virtual EDeviceStopResult Stop() override;

			virtual EDeviceTriggerResult Trigger();

			bool GetIntegerValue(CString& strName, int64_t &nValue);
			bool SetIntegerValue(CString& strName, int64_t nValue);
			bool GetFloatValue(CString& strName, double &dblValue);
			bool SetFloatValue(CString& strName, double dblValue);
			bool GetEnumValue(CString& strName, CString &strValue);
			bool GetEnumValue(CString& strName, int64_t &nValue);
			bool SetEnumValue(CString& strName, CString &strValue);
			bool SetEnumValue(CString& strName, int64_t nValue);
			bool GetBooleanValue(CString& strName, bool &bValue);
			bool SetBooleanValue(CString& strName, bool bValue);
			bool GetStringValue(CString& strName, CString &strValue);
			bool SetStringValue(CString& strName, CString &strValue);
			bool ExecuteCommand(CString& strName);

		public:
			DECLARE_MESSAGE_MAP()

		protected:
			virtual bool DoesModuleExist();

			static UINT LiveThread(_In_ LPVOID pParam);

		protected:
			CWinThread* m_pLiveThread = nullptr;

			PvDevice* m_pCamera = nullptr;
			PvStream* m_pStream = nullptr;
			PvPipeline* m_pPipeline = nullptr;
		};
	}
}
