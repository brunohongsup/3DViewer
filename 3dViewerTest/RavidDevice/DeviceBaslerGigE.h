#pragma once

#ifdef USING_DEVICE

#include "../RavidFramework/DeviceCamera.h"
#include "../RavidFramework/DefinitionsDevice.h"

struct PYLON_DEVICE_HANDLE_;
struct PYLON_STREAMGRABBER_HANDLE_;
struct PYLON_CHUNKPARSER_HANDLE_;
struct PYLON_WAITOBJECT_HANDLE_;
struct PYLON_STREAMBUFFER_HANDLE_;

namespace Ravid
{
	namespace Algorithms
	{
		class CRavidImage;
	}

	namespace Device
	{
		class CDeviceBaslerGigE : public CDeviceCamera
		{
		public:
			RavidUseDynamicCreation();

			RavidPreventCopySelf(CDeviceBaslerGigE);

			DECLARE_DYNAMIC(CDeviceBaslerGigE)

		public:
			CDeviceBaslerGigE();
			virtual ~CDeviceBaslerGigE();

			virtual EDeviceInitializeResult Initialize() override;
			
			virtual EDeviceTerminateResult Terminate() override;

			virtual bool LoadSettings() override;

			virtual EDeviceGrabResult Grab() override;
			virtual EDeviceLiveResult Live() override;
			virtual EDeviceStopResult Stop() override;

			virtual EDeviceTriggerResult Trigger();
			
		public:
			virtual EBaslerGetFunction GetPixelFormat(_Out_ EDeviceBaslerGigEPixelFormat* pParam);
			virtual EBaslerSetFunction SetPixelFormat(_In_ EDeviceBaslerGigEPixelFormat eParam);

			virtual EBaslerGetFunction GetTriggerSelector(_Out_ EDeviceBaslerGigETriggerSelector* pParam);
			virtual EBaslerSetFunction SetTriggerSelector(_In_ EDeviceBaslerGigETriggerSelector eParam);

			virtual EBaslerGetFunction GetFrameStartMode(_Out_ EDeviceBaslerGigESwitch* pParam);
			virtual EBaslerSetFunction SetFrameStartMode(_In_ EDeviceBaslerGigESwitch eParam);

			virtual EBaslerGetFunction GetLineStartMode(_Out_ EDeviceBaslerGigESwitch* pParam);
			virtual EBaslerSetFunction SetLineStartMode(_In_ EDeviceBaslerGigESwitch eParam);

			virtual EBaslerGetFunction GetAcquisitionStartMode(_Out_ EDeviceBaslerGigESwitch* pParam);
			virtual EBaslerSetFunction SetAcquisitionStartMode(_In_ EDeviceBaslerGigESwitch eParam);

			virtual EBaslerGetFunction GetFrameStartTriggerSource(_Out_ EDeviceBaslerGigETriggerSource* pParam);
			virtual EBaslerSetFunction SetFrameStartTriggerSource(_In_ EDeviceBaslerGigETriggerSource eParam);

			virtual EBaslerGetFunction GetLineStartTriggerSource(_Out_ EDeviceBaslerGigETriggerSource* pParam);
			virtual EBaslerSetFunction SetLineStartTriggerSource(_In_ EDeviceBaslerGigETriggerSource eParam);

			virtual EBaslerGetFunction GetAcquisitionStartTriggerSource(_Out_ EDeviceBaslerGigETriggerSource* pParam);
			virtual EBaslerSetFunction SetAcquisitionStartTriggerSource(_In_ EDeviceBaslerGigETriggerSource eParam);

			virtual EBaslerGetFunction GetFrameStartTriggerActivation(_Out_ EDeviceBaslerGigETriggerActivation* pParam);
			virtual EBaslerSetFunction SetFrameStartTriggerActivation(_In_ EDeviceBaslerGigETriggerActivation eParam);

			virtual EBaslerGetFunction GetLineStartTriggerActivation(_Out_ EDeviceBaslerGigETriggerActivation* pParam);
			virtual EBaslerSetFunction SetLineStartTriggerActivation(_In_ EDeviceBaslerGigETriggerActivation eParam);

			virtual EBaslerGetFunction GetAcquisitionStartTriggerActivation(_Out_ EDeviceBaslerGigETriggerActivation* pParam);
			virtual EBaslerSetFunction SetAcquisitionStartTriggerActivation(_In_ EDeviceBaslerGigETriggerActivation eParam);

			virtual EBaslerGetFunction GetExposureAuto(_Out_ EDeviceBaslerGigEAuto* pParam);
			virtual EBaslerSetFunction SetExposureAuto(_In_ EDeviceBaslerGigEAuto eParam);

			virtual EBaslerGetFunction GetGainAuto(_Out_ EDeviceBaslerGigEAuto* pParam);
			virtual EBaslerSetFunction SetGainAuto(_In_ EDeviceBaslerGigEAuto eParam);

			virtual EBaslerGetFunction GetGainSelector(_Out_ EDeviceBaslerGigEGainSelector* pParam);
			virtual EBaslerSetFunction SetGainSelector(_In_ EDeviceBaslerGigEGainSelector eParam);
			
			virtual EBaslerGetFunction GetGrabCount(_Out_ int* pParam);
			virtual EBaslerSetFunction SetGrabCount(_In_ int nParam);

			virtual EBaslerGetFunction GetCanvasWidth(_Out_ int* pParam);
			virtual EBaslerSetFunction SetCanvasWidth(_In_ int nParam);

			virtual EBaslerGetFunction GetCanvasHeight(_Out_ int* pParam);
			virtual EBaslerSetFunction SetCanvasHeight(_In_ int nParam);

			virtual EBaslerGetFunction GetOffsetCenterX(_Out_ bool* pParam);
			virtual EBaslerSetFunction SetOffsetCenterX(_In_ bool bParam);

			virtual EBaslerGetFunction GetOffsetCenterY(_Out_ bool* pParam);
			virtual EBaslerSetFunction SetOffsetCenterY(_In_ bool bParam);

			virtual EBaslerGetFunction GetOffsetX(_Out_ int* pParam);
			virtual EBaslerSetFunction SetOffsetX(_In_ int nParam);

			virtual EBaslerGetFunction GetOffsetY(_Out_ int* pParam);
			virtual EBaslerSetFunction SetOffsetY(_In_ int nParam);

			virtual EBaslerGetFunction GetGrabWaitingTime(_Out_ int* pParam);
			virtual EBaslerSetFunction SetGrabWaitingTime(_In_ int nParam);

			virtual EBaslerGetFunction GetExposure(_Out_ int* pParam);
			virtual EBaslerSetFunction SetExposure(_In_ int nParam);

			virtual EBaslerGetFunction GetAllGain(_Out_ int* pParam);
			virtual EBaslerSetFunction SetAllGain(_In_ int nParam);

			virtual EBaslerGetFunction GetAnalogGain(_Out_ int* pParam);
			virtual EBaslerSetFunction SetAnalogGain(_In_ int nParam);

			virtual EBaslerGetFunction GetDigitalGain(_Out_ int* pParam);
			virtual EBaslerSetFunction SetDigitalGain(_In_ int nParam);

			virtual EBaslerGetFunction GetTap1Gain(_Out_ int* pParam);
			virtual EBaslerSetFunction SetTap1Gain(_In_ int nParam);

			virtual EBaslerGetFunction GetTap2Gain(_Out_ int* pParam);
			virtual EBaslerSetFunction SetTap2Gain(_In_ int nParam);

			virtual EBaslerGetFunction GetPacketSize(_Out_ int* pParam);
			virtual EBaslerSetFunction SetPacketSize(_In_ int nParam);

			virtual EBaslerGetFunction GetLineSelector(_Out_ EDeviceBaslerGigELineSelector* pParam);
			virtual EBaslerSetFunction SetLineSelector(_In_ EDeviceBaslerGigELineSelector eParam);

			virtual EBaslerGetFunction GetLine1Mode(_Out_ EDeviceBaslerGigELineMode* pParam);
			virtual EBaslerSetFunction SetLine1Mode(_In_ EDeviceBaslerGigELineMode eParam);

			virtual EBaslerGetFunction GetLine1Source(_Out_ EDeviceBaslerGigELineSource* pParam);
			virtual EBaslerSetFunction SetLine1Source(_In_ EDeviceBaslerGigELineSource eParam);

			virtual EBaslerGetFunction GetLine1Inverter(_Out_ bool* pParam);
			virtual EBaslerSetFunction SetLine1Inverter(_In_ bool bParam);

			virtual EBaslerGetFunction GetLine1DebouncerTime(_Out_ double* pParam);
			virtual EBaslerSetFunction SetLine1DebouncerTime(_In_ double dblParam);

			virtual EBaslerGetFunction GetLine1MinOutputPulseWidth(_Out_ double* pParam);
			virtual EBaslerSetFunction SetLine1MinOutputPulseWidth(_In_ double dblParam);

			virtual EBaslerGetFunction GetLine2Mode(_Out_ EDeviceBaslerGigELineMode* pParam);
			virtual EBaslerSetFunction SetLine2Mode(_In_ EDeviceBaslerGigELineMode eParam);

			virtual EBaslerGetFunction GetLine2Source(_Out_ EDeviceBaslerGigELineSource* pParam);
			virtual EBaslerSetFunction SetLine2Source(_In_ EDeviceBaslerGigELineSource eParam);

			virtual EBaslerGetFunction GetLine2Inverter(_Out_ bool* pParam);
			virtual EBaslerSetFunction SetLine2Inverter(_In_ bool bParam);

			virtual EBaslerGetFunction GetLine2DebouncerTime(_Out_ double* pParam);
			virtual EBaslerSetFunction SetLine2DebouncerTime(_In_ double dblParam);

			virtual EBaslerGetFunction GetLine2MinOutputPulseWidth(_Out_ double* pParam);
			virtual EBaslerSetFunction SetLine2MinOutputPulseWidth(_In_ double dblParam);

			virtual EBaslerGetFunction GetLine3Mode(_Out_ EDeviceBaslerGigELineMode* pParam);
			virtual EBaslerSetFunction SetLine3Mode(_In_ EDeviceBaslerGigELineMode eParam);

			virtual EBaslerGetFunction GetLine3Source(_Out_ EDeviceBaslerGigELineSource* pParam);
			virtual EBaslerSetFunction SetLine3Source(_In_ EDeviceBaslerGigELineSource eParam);

			virtual EBaslerGetFunction GetLine3Inverter(_Out_ bool* pParam);
			virtual EBaslerSetFunction SetLine3Inverter(_In_ bool bParam);

			virtual EBaslerGetFunction GetLine3DebouncerTime(_Out_ double* pParam);
			virtual EBaslerSetFunction SetLine3DebouncerTime(_In_ double dblParam);

			virtual EBaslerGetFunction GetLine3MinOutputPulseWidth(_Out_ double* pParam);
			virtual EBaslerSetFunction SetLine3MinOutputPulseWidth(_In_ double dblParam);

			virtual EBaslerGetFunction GetLine4Mode(_Out_ EDeviceBaslerGigELineMode* pParam);
			virtual EBaslerSetFunction SetLine4Mode(_In_ EDeviceBaslerGigELineMode eParam);

			virtual EBaslerGetFunction GetLine4Source(_Out_ EDeviceBaslerGigELineSource* pParam);
			virtual EBaslerSetFunction SetLine4Source(_In_ EDeviceBaslerGigELineSource eParam);

			virtual EBaslerGetFunction GetLine4Inverter(_Out_ bool* pParam);
			virtual EBaslerSetFunction SetLine4Inverter(_In_ bool bParam);

			virtual EBaslerGetFunction GetLine4DebouncerTime(_Out_ double* pParam);
			virtual EBaslerSetFunction SetLine4DebouncerTime(_In_ double dblParam);

			virtual EBaslerGetFunction GetLine4MinOutputPulseWidth(_Out_ double* pParam);
			virtual EBaslerSetFunction SetLine4MinOutputPulseWidth(_In_ double dblParam);

			virtual EBaslerGetFunction GetLightSourceSelector(_Out_ EDeviceBaslerGigELightSourceSelector* pParam);
			virtual EBaslerSetFunction SetLightSourceSelector(_In_ EDeviceBaslerGigELightSourceSelector eParam);

			virtual EBaslerGetFunction GetBalanceWhiteReset(_Out_ bool* pParam);
			virtual EBaslerSetFunction SetBalanceWhiteReset(_In_ bool bParam);

			virtual EBaslerGetFunction GetBalanceWhiteAuto(_Out_ EDeviceBaslerGigEAuto* pParam);
			virtual EBaslerSetFunction SetBalanceWhiteAuto(_In_ EDeviceBaslerGigEAuto eParam);

			virtual EBaslerGetFunction GetBalanceWhiteRawRed(_Out_ int* pParam);
			virtual EBaslerSetFunction SetBalanceWhiteRawRed(_In_ int nParam);

			virtual EBaslerGetFunction GetBalanceWhiteRawGreen(_Out_ int* pParam);
			virtual EBaslerSetFunction SetBalanceWhiteRawGreen(_In_ int nParam);

			virtual EBaslerGetFunction GetBalanceWhiteRawBlue(_Out_ int* pParam);
			virtual EBaslerSetFunction SetBalanceWhiteRawBlue(_In_ int nParam);

			virtual EBaslerGetFunction GetColorTransRawGain00(_Out_ int* pParam);
			virtual EBaslerSetFunction SetColorTransRawGain00(_In_ int nParam);

			virtual EBaslerGetFunction GetColorTransRawGain01(_Out_ int* pParam);
			virtual EBaslerSetFunction SetColorTransRawGain01(_In_ int nParam);

			virtual EBaslerGetFunction GetColorTransRawGain02(_Out_ int* pParam);
			virtual EBaslerSetFunction SetColorTransRawGain02(_In_ int nParam);

			virtual EBaslerGetFunction GetColorTransRawGain10(_Out_ int* pParam);
			virtual EBaslerSetFunction SetColorTransRawGain10(_In_ int nParam);

			virtual EBaslerGetFunction GetColorTransRawGain11(_Out_ int* pParam);
			virtual EBaslerSetFunction SetColorTransRawGain11(_In_ int nParam);

			virtual EBaslerGetFunction GetColorTransRawGain12(_Out_ int* pParam);
			virtual EBaslerSetFunction SetColorTransRawGain12(_In_ int nParam);

			virtual EBaslerGetFunction GetColorTransRawGain20(_Out_ int* pParam);
			virtual EBaslerSetFunction SetColorTransRawGain20(_In_ int nParam);

			virtual EBaslerGetFunction GetColorTransRawGain21(_Out_ int* pParam);
			virtual EBaslerSetFunction SetColorTransRawGain21(_In_ int nParam);

			virtual EBaslerGetFunction GetColorTransRawGain22(_Out_ int* pParam);
			virtual EBaslerSetFunction SetColorTransRawGain22(_In_ int nParam);

			virtual EBaslerGetFunction GetColorTransMatrixFactorRaw(_Out_ int* pParam);
			virtual EBaslerSetFunction SetColorTransMatrixFactorRaw(_In_ int nParam);

			virtual bool OnParameterChanged(_In_ int nParam, _In_ CString strValue) override;

			virtual bool GetCommandBool(const char* pCommnadString, bool* pGetValue);
			virtual bool SetCommandBool(const char* pCommnadString, bool bSetValue);

			virtual bool GetCommandInt32(const char* pCommnadString, int32_t* pGetValue);
			virtual bool SetCommandInt32(const char* pCommnadString, int32_t nSetValue);

			virtual bool GetCommandInt64(const char* pCommnadString, int64_t* pGetValue);
			virtual bool SetCommandInt64(const char* pCommnadString, int64_t nSetValue);

			virtual bool GetCommandFloat(const char* pCommnadString, double* pGetValue);
			virtual bool SetCommandFloat(const char* pCommnadString, double dblSetValue);

		public:
			DECLARE_MESSAGE_MAP()

		protected:
			virtual bool DoesModuleExist();

		protected:
			static UINT CallbackFunction(_In_ LPVOID pParam);

		protected:

			bool m_bColorCamera = false;
			
			unsigned long m_nGrabCount = 0;

			volatile int32_t m_nWaitTime = 0;

			CWinThread* m_pLiveThread = nullptr;

			PYLON_DEVICE_HANDLE_* m_hDevice = nullptr;
			PYLON_STREAMGRABBER_HANDLE_* m_hGrabber = nullptr;
			PYLON_CHUNKPARSER_HANDLE_* m_hChunkParser = nullptr;
			PYLON_WAITOBJECT_HANDLE_* m_hWait = nullptr;
			PYLON_STREAMBUFFER_HANDLE_* m_hGrabBuffer = nullptr;
		};
	}
}

#endif