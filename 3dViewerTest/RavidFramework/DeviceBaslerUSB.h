#pragma once

#include "DeviceCamera.h"

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
		class AFX_EXT_CLASS CDeviceBaslerUSB : public CDeviceCamera
		{
		public:
			RavidUseDynamicCreation();

			RavidPreventCopySelf(CDeviceBaslerUSB);

			DECLARE_DYNAMIC(CDeviceBaslerUSB)

		public:
			CDeviceBaslerUSB();
			virtual ~CDeviceBaslerUSB();
			
			virtual EDeviceInitializeResult Initialize() override;

			virtual EDeviceTerminateResult Terminate() override;

			virtual bool LoadSettings() override;

			virtual EDeviceGrabResult Grab() override;
			virtual EDeviceLiveResult Live() override;
			virtual EDeviceStopResult Stop() override;

			virtual EDeviceTriggerResult Trigger();

		public:
			virtual EBaslerGetFunction GetGrabCount(_Out_ int* pParam);
			virtual EBaslerSetFunction SetGrabCount(_In_ int unParam);

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

			virtual EBaslerGetFunction GetReverseX(_Out_ bool* pParam);
			virtual EBaslerSetFunction SetReverseX(_In_ bool bParam);

			virtual EBaslerGetFunction GetReverseY(_Out_ bool* pParam);
			virtual EBaslerSetFunction SetReverseY(_In_ bool bParam);

			virtual EBaslerGetFunction GetGrabWaitingTime(_Out_ int* pParam);
			virtual EBaslerSetFunction SetGrabWaitingTime(_In_ int nParam);

 			virtual EBaslerGetFunction GetPixelFormat(_Out_ EDeviceBaslerUSBPixelFormat* pParam);
 			virtual EBaslerSetFunction SetPixelFormat(_In_ EDeviceBaslerUSBPixelFormat eParam);

			virtual EBaslerGetFunction GetGainAuto(_Out_ EDeviceBaslerUSBGainAuto* pParam);
			virtual EBaslerSetFunction SetGainAuto(_In_ EDeviceBaslerUSBGainAuto eParam);

			virtual EBaslerGetFunction GetGainSelector(_Out_ EDeviceBaslerUSBGainSelector* pParam);
			virtual EBaslerSetFunction SetGainSelector(_In_ EDeviceBaslerUSBGainSelector eParam);

			virtual EBaslerGetFunction GetAllGain(_Out_ double* pParam);
			virtual EBaslerSetFunction SetAllGain(_In_ double dblParam);

			virtual EBaslerGetFunction GetBlackLevelSelector(_Out_ EDeviceBaslerUSBBlackLevelSelector* pParam);
			virtual EBaslerSetFunction SetBlackLevelSelector(_In_ EDeviceBaslerUSBBlackLevelSelector eParam);

			virtual EBaslerGetFunction GetBlackLevel(_Out_ double* pParam);
			virtual EBaslerSetFunction SetBlackLevel(_In_ double dblParam);

			virtual EBaslerGetFunction GetGamma(_Out_ double* pParam);
			virtual EBaslerSetFunction SetGamma(_In_ double dblParam);

			virtual EBaslerGetFunction GetShutterMode(_Out_ EDeviceBaslerUSBShutterMode* pParam);
			virtual EBaslerSetFunction SetShutterMode(_In_ EDeviceBaslerUSBShutterMode eParam);

			virtual EBaslerGetFunction GetExposureAuto(_Out_ EDeviceBaslerUSBExposureAuto* pParam);
			virtual EBaslerSetFunction SetExposureAuto(_In_ EDeviceBaslerUSBExposureAuto eParam);

			virtual EBaslerGetFunction GetExposureMode(_Out_ EDeviceBaslerUSBExposureMode* pParam);
			virtual EBaslerSetFunction SetExposureMode(_In_ EDeviceBaslerUSBExposureMode eParam);

			virtual EBaslerGetFunction GetExposureTime(_Out_ double* pParam);
			virtual EBaslerSetFunction SetExposureTime(_In_ double dblParam);

			virtual EBaslerGetFunction GetTriggerSelector(_Out_ EDeviceBaslerUSBTriggerSelector* pParam);
			virtual EBaslerSetFunction SetTriggerSelector(_In_ EDeviceBaslerUSBTriggerSelector eParam);

			virtual EBaslerGetFunction GetFrameStartTriggerMode(_Out_ EDeviceBaslerUSBTriggerMode* pParam);
			virtual EBaslerSetFunction SetFrameStartTriggerMode(_In_ EDeviceBaslerUSBTriggerMode eParam);

			virtual EBaslerGetFunction GetFrameStartTriggerSource(_Out_ EDeviceBaslerUSBTriggerSource* pParam);
			virtual EBaslerSetFunction SetFrameStartTriggerSource(_In_ EDeviceBaslerUSBTriggerSource eParam);

			virtual EBaslerGetFunction GetFrameStartTriggerActivation(_Out_ EDeviceBaslerUSBTriggerActivation* pParam);
			virtual EBaslerSetFunction SetFrameStartTriggerActivation(_In_ EDeviceBaslerUSBTriggerActivation eParam);

			virtual EBaslerGetFunction GetFrameStartTriggerDelay(_Out_ double* pParam);
			virtual EBaslerSetFunction SetFrameStartTriggerDelay(_In_ double dblParam);

			virtual EBaslerGetFunction GetFrameBurstStartTriggerMode(_Out_ EDeviceBaslerUSBTriggerMode* pParam);
			virtual EBaslerSetFunction SetFrameBurstStartTriggerMode(_In_ EDeviceBaslerUSBTriggerMode eParam);

			virtual EBaslerGetFunction GetFrameBurstStartTriggerSource(_Out_ EDeviceBaslerUSBTriggerSource* pParam);
			virtual EBaslerSetFunction SetFrameBurstStartTriggerSource(_In_ EDeviceBaslerUSBTriggerSource eParam);

			virtual EBaslerGetFunction GetFrameBurstStartTriggerActivation(_Out_ EDeviceBaslerUSBTriggerActivation* pParam);
			virtual EBaslerSetFunction SetFrameBurstStartTriggerActivation(_In_ EDeviceBaslerUSBTriggerActivation eParam);

			virtual EBaslerGetFunction GetFrameBurstStartTriggerDelay(_Out_ double* pParam);
			virtual EBaslerSetFunction SetFrameBurstStartTriggerDelay(_In_ double dblParam);

			virtual EBaslerGetFunction GetEnableAcquisitionFrameRate(_Out_ bool* pParam);							 
			virtual EBaslerSetFunction SetEnableAcquisitionFrameRate(_In_ bool bParam);

			virtual EBaslerGetFunction GetAcquisitionFrameRate(_Out_ double* pParam);
			virtual EBaslerSetFunction SetAcquisitionFrameRate(_In_ double dblParam);

			virtual EBaslerGetFunction GetAcquisitionStatusSelector(_Out_ EDeviceBaslerUSBAcquisitionStatusSelector* pParam);
			virtual EBaslerSetFunction SetAcquisitionStatusSelector(_In_ EDeviceBaslerUSBAcquisitionStatusSelector eParam);

			virtual EBaslerGetFunction GetLineSelector(_Out_ EDeviceBaslerUSBLineSelector* pParam);
			virtual EBaslerSetFunction SetLineSelector(_In_ EDeviceBaslerUSBLineSelector eParam);

			virtual EBaslerGetFunction GetLine1Mode(_Out_ EDeviceBaslerUSBLineMode* pParam);
			virtual EBaslerSetFunction SetLine1Mode(_In_ EDeviceBaslerUSBLineMode eParam);

			virtual EBaslerGetFunction GetLine1Source(_Out_ EDeviceBaslerUSBLineSource* pParam);
			virtual EBaslerSetFunction SetLine1Source(_In_ EDeviceBaslerUSBLineSource eParam);

			virtual EBaslerGetFunction GetLine1Inverter(_Out_ bool* pParam);
			virtual EBaslerSetFunction SetLine1Inverter(_In_ bool bParam);

			virtual EBaslerGetFunction GetLine1DebouncerTime(_Out_ double* pParam);
			virtual EBaslerSetFunction SetLine1DebouncerTime(_In_ double dblParam);

			virtual EBaslerGetFunction GetLine1MinOutputPulseWidth(_Out_ double* pParam);
			virtual EBaslerSetFunction SetLine1MinOutputPulseWidth(_In_ double dblParam);

			virtual EBaslerGetFunction GetLine2Mode(_Out_ EDeviceBaslerUSBLineMode* pParam);
			virtual EBaslerSetFunction SetLine2Mode(_In_ EDeviceBaslerUSBLineMode eParam);

			virtual EBaslerGetFunction GetLine2Source(_Out_ EDeviceBaslerUSBLineSource* pParam);
			virtual EBaslerSetFunction SetLine2Source(_In_ EDeviceBaslerUSBLineSource eParam);

			virtual EBaslerGetFunction GetLine2Inverter(_Out_ bool* pParam);
			virtual EBaslerSetFunction SetLine2Inverter(_In_ bool bParam);

			virtual EBaslerGetFunction GetLine2DebouncerTime(_Out_ double* pParam);
			virtual EBaslerSetFunction SetLine2DebouncerTime(_In_ double dblParam);

			virtual EBaslerGetFunction GetLine2MinOutputPulseWidth(_Out_ double* pParam);
			virtual EBaslerSetFunction SetLine2MinOutputPulseWidth(_In_ double dblParam);

			virtual EBaslerGetFunction GetLine3Mode(_Out_ EDeviceBaslerUSBLineMode* pParam);
			virtual EBaslerSetFunction SetLine3Mode(_In_ EDeviceBaslerUSBLineMode eParam);

			virtual EBaslerGetFunction GetLine3Source(_Out_ EDeviceBaslerUSBLineSource* pParam);
			virtual EBaslerSetFunction SetLine3Source(_In_ EDeviceBaslerUSBLineSource eParam);

			virtual EBaslerGetFunction GetLine3Inverter(_Out_ bool* pParam);
			virtual EBaslerSetFunction SetLine3Inverter(_In_ bool bParam);

			virtual EBaslerGetFunction GetLine3DebouncerTime(_Out_ double* pParam);
			virtual EBaslerSetFunction SetLine3DebouncerTime(_In_ double dblParam);

			virtual EBaslerGetFunction GetLine3MinOutputPulseWidth(_Out_ double* pParam);
			virtual EBaslerSetFunction SetLine3MinOutputPulseWidth(_In_ double dblParam);

			virtual EBaslerGetFunction GetLine4Mode(_Out_ EDeviceBaslerUSBLineMode* pParam);
			virtual EBaslerSetFunction SetLine4Mode(_In_ EDeviceBaslerUSBLineMode eParam);

			virtual EBaslerGetFunction GetLine4Source(_Out_ EDeviceBaslerUSBLineSource* pParam);
			virtual EBaslerSetFunction SetLine4Source(_In_ EDeviceBaslerUSBLineSource eParam);

			virtual EBaslerGetFunction GetLine4Inverter(_Out_ bool* pParam);
			virtual EBaslerSetFunction SetLine4Inverter(_In_ bool bParam);

			virtual EBaslerGetFunction GetLine4DebouncerTime(_Out_ double* pParam);
			virtual EBaslerSetFunction SetLine4DebouncerTime(_In_ double dblParam);

			virtual EBaslerGetFunction GetLine4MinOutputPulseWidth(_Out_ double* pParam);
			virtual EBaslerSetFunction SetLine4MinOutputPulseWidth(_In_ double dblParam);

			virtual bool OnParameterChanged(_In_ int nParam, _In_ CString strValue) override;

		public:
			DECLARE_MESSAGE_MAP()

		protected:
			virtual bool DoesModuleExist();

			virtual bool GetCommandInt32(const char* pCommnadString, int32_t* pGetValue);
			virtual bool SetCommandInt32(const char* pCommnadString, int32_t nSetValue);

			virtual bool GetCommandInt64(const char* pCommnadString, int64_t* pGetValue);
			virtual bool SetCommandInt64(const char* pCommnadString, int64_t nSetValue);

		protected:
			static UINT LiveThread(_In_ LPVOID pParam);

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
