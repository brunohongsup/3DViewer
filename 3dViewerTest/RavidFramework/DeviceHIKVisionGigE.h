#pragma once

#include "DeviceCamera.h"

namespace Ravid
{
	namespace Algorithms
	{
		class CRavidImage;
	}

	namespace Device
	{
		class AFX_EXT_CLASS CDeviceHIKVisionGigE : public CDeviceCamera
		{
		public:
			RavidUseDynamicCreation();

			RavidPreventCopySelf(CDeviceHIKVisionGigE);

			DECLARE_DYNAMIC(CDeviceHIKVisionGigE)

		public:
			CDeviceHIKVisionGigE();
			virtual ~CDeviceHIKVisionGigE();

			virtual EDeviceInitializeResult Initialize() override;

			virtual EDeviceTerminateResult Terminate() override;

			virtual bool LoadSettings() override;

			virtual EDeviceGrabResult Grab() override;
			virtual EDeviceLiveResult Live() override;
			virtual EDeviceStopResult Stop() override;

			virtual EDeviceTriggerResult Trigger();

		public:
			EDeviceHIKVisionGigEGetFunction GetInitializeMode(_Out_ EDeviceHIKVisionGigEInitMode *pParam);
			EDeviceHIKVisionGigEGetFunction SetInitializeMode(_In_ EDeviceHIKVisionGigEInitMode eParam);

			virtual EHIKVisionGetFunction GetCanvasWidth(_Out_ int* pParam);
			virtual EHIKVisionSetFunction SetCanvasWidth(_In_ int nParam);

			virtual EHIKVisionGetFunction GetCanvasHeight(_Out_ int* pParam);
			virtual EHIKVisionSetFunction SetCanvasHeight(_In_ int nParam);

			virtual EHIKVisionGetFunction GetAcquisitionBurstFrameCount(_Out_ int* nParam);
			virtual EHIKVisionSetFunction SetAcquisitionBurstFrameCount(_In_ int nParam);

			virtual EHIKVisionGetFunction GetAcquisitionFrameRate(_Out_ float* pParam);
			virtual EHIKVisionSetFunction SetAcquisitionFrameRate(_In_ float fParam);

			virtual EHIKVisionGetFunction GetAcquisitionFrameRateControlEnable(_Out_ bool *pParam);
			virtual EHIKVisionSetFunction SetAcquisitionFrameRateControlEnable(_In_ bool bParam);

			virtual EHIKVisionGetFunction GetAcquisitionTriggerSelector(_Out_ EDeviceHIKVisionGigEAcquisitionTriggerSelector *pParam);
			virtual EHIKVisionSetFunction SetAcquisitionTriggerSelector(_In_ EDeviceHIKVisionGigEAcquisitionTriggerSelector eParam);

			virtual EHIKVisionGetFunction GetAcquisitionMode(_Out_ EDeviceHIKVisionAcquisitionMode* pParam);
			virtual EHIKVisionSetFunction SetAcquisitionMode(_In_ EDeviceHIKVisionAcquisitionMode eParam);

			virtual EHIKVisionGetFunction GetImagePixelFormat(_Out_ EDeviceHIKVisionGigEPixelFormat* pParam);
			virtual EHIKVisionSetFunction SetImagePixelFormat(_In_ EDeviceHIKVisionGigEPixelFormat nParam);

			virtual EHIKVisionSetFunction SetLineSelector(_Out_ EDeviceHIKVisionGigELineSelect eParam);
			virtual EHIKVisionGetFunction GetLineSelector(_In_ EDeviceHIKVisionGigELineSelect *eParam);

			virtual EHIKVisionGetFunction GetLine0DebouncerTime(_Out_ int *pParam);
			virtual EHIKVisionSetFunction SetLine0DebouncerTime(_In_ int nParam);

			virtual EHIKVisionGetFunction GetLine2DebouncerTime(_Out_ int *pParam);
			virtual EHIKVisionSetFunction SetLine2DebouncerTime(_In_ int nParam);

			virtual EHIKVisionSetFunction SetLine1Inverter(_In_ bool bParam);
			virtual EHIKVisionGetFunction GetLine1Inverter(_Out_ bool *pParam);

			virtual EHIKVisionSetFunction SetLine2Inverter(_In_ bool bParam);
			virtual EHIKVisionGetFunction GetLine2Inverter(_Out_ bool *pParam);

			virtual EHIKVisionSetFunction SetLine1Source(_In_ EDeviceHIKVisionGigELineSource eParam);
			virtual EHIKVisionGetFunction GetLine1Source(_Out_ EDeviceHIKVisionGigELineSource *pParam);

			virtual EHIKVisionSetFunction SetLine2Source(_In_ EDeviceHIKVisionGigELineSource eParam);
			virtual EHIKVisionGetFunction GetLine2Source(_Out_ EDeviceHIKVisionGigELineSource *pParam);

			virtual EHIKVisionGetFunction GetOffsetX(_Out_ int* pParam);
			virtual EHIKVisionSetFunction SetOffsetX(_In_ int nParam);

			virtual EHIKVisionGetFunction GetOffsetY(_Out_ int* pParam);
			virtual EHIKVisionSetFunction SetOffsetY(_In_ int nParam);

			virtual EHIKVisionGetFunction GetReverseX(_Out_ bool* pParam);
			virtual EHIKVisionSetFunction SetReverseX(_In_ bool bParam);

			virtual EHIKVisionGetFunction GetReverseY(_Out_ bool* pParam);
			virtual EHIKVisionSetFunction SetReverseY(_In_ bool bParam);

			virtual EHIKVisionGetFunction GetDeviceStreamChannelPackSize(_Out_ int* pParam);
			virtual	EHIKVisionSetFunction SetDeviceStreamChannelPackSize(_In_ int nParam);

			virtual EHIKVisionGetFunction GetAnalogGaindB(_Out_ float* pParam);
			virtual	EHIKVisionSetFunction SetAnalogGaindB(_In_ float fParam);

			virtual EHIKVisionGetFunction GetAnalogGainAuto(_Out_ EDeviceHIKVisionGigEGainAuto* pParam);
			virtual	EHIKVisionSetFunction SetAnalogGainAuto(_In_ EDeviceHIKVisionGigEGainAuto eParam);

			virtual EHIKVisionSetFunction SetLine1StrobEnable(_In_ bool bParam);
			virtual EHIKVisionGetFunction GetLine1StrobEnable(_Out_ bool *pParam);

			virtual EHIKVisionSetFunction SetLine2StrobEnable(_In_ bool bParam);
			virtual EHIKVisionGetFunction GetLine2StrobEnable(_Out_ bool *pParam);

			virtual EHIKVisionGetFunction GetDigitalShift(_Out_ float* pParam);
			virtual	EHIKVisionSetFunction SetDigitalShift(_In_ float fParam);

			virtual EHIKVisionGetFunction GetDigitalShiftEnable(_Out_ bool* bParam);
			virtual	EHIKVisionSetFunction SetDigitalShiftEnable(_In_ bool bParam);

			virtual EHIKVisionGetFunction GetBlackLevel(_Out_ int* pParam);
			virtual	EHIKVisionSetFunction SetBlackLevel(_In_ int fParam);

			virtual EHIKVisionGetFunction GetBlackLevelEnable(_Out_ bool* pParam);
			virtual	EHIKVisionSetFunction SetBlackLevelEnable(_In_ bool bParam);

			virtual EHIKVisionGetFunction GetLine2StrobeLineDuration(_Out_ int* pParam);
			virtual	EHIKVisionSetFunction SetLine2StrobeLineDuration(_In_ int nParam);

			virtual	EHIKVisionGetFunction GetLine1StrobeLineDuration(_Out_ int* pParam);
			virtual	EHIKVisionSetFunction SetLine1StrobeLineDuration(_In_ int nParam);

			virtual	EHIKVisionGetFunction GetLine2StrobeLineDelay(_Out_ int* pParam);
			virtual	EHIKVisionSetFunction SetLine2StrobeLineDelay(_In_ int nParam);

			virtual	EHIKVisionGetFunction GetLine2StrobeLinePreDelay(_Out_ int* pParam);
			virtual	EHIKVisionSetFunction SetLine2StrobeLinePreDelay(_In_ int nParam);

			virtual	EHIKVisionGetFunction GetLine1StrobeLineDelay(_Out_ int* pParam);
			virtual	EHIKVisionSetFunction SetLine1StrobeLineDelay(_In_ int nParam);

			virtual	EHIKVisionGetFunction GetLine1StrobeLinePreDelay(_Out_ int* pParam);
			virtual	EHIKVisionSetFunction SetLine1StrobeLinePreDelay(_In_ int nParam);

			virtual EHIKVisionGetFunction GetLine1HardWareTriggerSource(_Out_ EDeviceHIKVisionGigEHardTriggerLineSelect *eParam);
			virtual EHIKVisionSetFunction SetLine1HardWareTriggerSource(_In_ EDeviceHIKVisionGigEHardTriggerLineSelect eParam);

			virtual EHIKVisionGetFunction GetLine2HardWareTriggerSource(_Out_ EDeviceHIKVisionGigEHardTriggerLineSelect *eParam);
			virtual EHIKVisionSetFunction SetLine2HardWareTriggerSource(_In_ EDeviceHIKVisionGigEHardTriggerLineSelect eParam);

			virtual EHIKVisionSetFunction SetLine1HardWareTriggerActivation(_In_ EDeviceHIKVisionGigETriggerActivation eParam);
			virtual	EHIKVisionGetFunction GetLine1HardWareTriggerActivation(_Out_ EDeviceHIKVisionGigETriggerActivation *eParam);

			virtual EHIKVisionSetFunction SetLine2HardWareTriggerActivation(_In_ EDeviceHIKVisionGigETriggerActivation eParam);
			virtual	EHIKVisionGetFunction GetLine2HardWareTriggerActivation(_Out_ EDeviceHIKVisionGigETriggerActivation *eParam);

			virtual EHIKVisionSetFunction SetLine2Mode(_In_ EDeviceHIKVisionGigELineMode eParam);
			virtual	EHIKVisionGetFunction GetLine2Mode(_Out_ EDeviceHIKVisionGigELineMode* pParam);

			virtual EHIKVisionGetFunction GetAcquisitionTriggerMode(_Out_ EDeviceHIKVisionGigESwitch* pParam);
			virtual EHIKVisionSetFunction SetAcquisitionTriggerMode(_In_ EDeviceHIKVisionGigESwitch eParam);

			virtual EHIKVisionGetFunction GetAcquisitionTriggerSource(_Out_ EDeviceHIKVisionGigEAcquisitionTriggerSource* pParam);
			virtual	EHIKVisionSetFunction SetAcquisitionTriggerSource(_In_ EDeviceHIKVisionGigEAcquisitionTriggerSource eParam);

			virtual EHIKVisionGetFunction GetAcquisitionTriggerActivation(_Out_ EDeviceHIKVisionGigEAcquisitionTriggerActivation* pParam);
			virtual	EHIKVisionSetFunction SetAcquisitionTriggerActivation(_In_ EDeviceHIKVisionGigEAcquisitionTriggerActivation eParam);

			virtual EHIKVisionGetFunction GetAcquisitionTriggerDelay(_Out_ float* pParam);
			virtual	EHIKVisionSetFunction SetAcquisitionTriggerDelay(_In_ float fParam);

			virtual EHIKVisionGetFunction GetAcquisitionExposureMode(_Out_ EDeviceHIKVisionGigEAcquisitionExposureMode* pParam);
			virtual	EHIKVisionSetFunction SetAcquisitionExposureMode(_In_ EDeviceHIKVisionGigEAcquisitionExposureMode eParam);

			virtual EHIKVisionGetFunction GetAcquisitionExposureTime(_Out_ float *pParam);
			virtual	EHIKVisionSetFunction SetAcquisitionExposureTime(_In_ float fParam);

			virtual EHIKVisionGetFunction GetAcquisitionExposureAuto(_Out_ EDeviceHIKVisionGigEAuto *pParam);
			virtual	EHIKVisionSetFunction SetAcquisitionExposureAuto(_In_ EDeviceHIKVisionGigEAuto eParam);
			
			virtual EHIKVisionGetFunction GetGrabCount(_Out_ int *pParam);
			virtual EHIKVisionSetFunction SetGrabCount(_In_ int nParam);

			EDeviceHIKVisionGigEGetFunction GetPixelFormat(_Out_ EDeviceHIKVisionGigEPixelFormat *pParam);
			EDeviceHIKVisionGigEGetFunction SetPixelFormat(_In_ EDeviceHIKVisionGigEPixelFormat eParam);

			EDeviceHIKVisionGigEGetFunction GetWidth(_Out_ int *pParam);
			EDeviceHIKVisionGigEGetFunction SetWidth(_In_ int nParam);

			EDeviceHIKVisionGigEGetFunction GetHeight(_Out_ int *pParam);
			EDeviceHIKVisionGigEGetFunction SetHeight(_In_ int nParam);

			EDeviceHIKVisionGigEGetFunction SetHeartbeatTimeout_ms(_In_ unsigned int unParam);
			
			void IncreaseGrabCount();
			int GetIncreaseGrabCount();
			void* GetDeviceHandler();

			virtual bool OnParameterChanged(_In_ int nParam, _In_ CString strValue) override;

			void UpdateDeviceToParameter();

			virtual bool GetCommandInt32(const char* pCommnadString, int32_t* pGetValue);
			virtual bool SetCommandInt32(const char* pCommnadString, int32_t nSetValue);

			virtual bool GetCommandInt64(const char* pCommnadString, int64_t* pGetValue);
			virtual bool SetCommandInt64(const char* pCommnadString, int64_t nSetValue);

			virtual bool GetCommandFloat(const char* pCommnadString, double* pGetValue);
			virtual bool SetCommandFloat(const char* pCommnadString, double dblSetValue);

			virtual bool GetCommandBool(const char* pCommnadString, bool* pGetValue);
			virtual bool SetCommandBool(const char* pCommnadString, bool bGetValue);

			virtual bool GetCommandString(const char* pCommnadString, const char* pGetValue);
			virtual bool SetCommandString(const char* pCommnadString, const char* pSetValue);

		public:
			DECLARE_MESSAGE_MAP()

		protected:
			virtual bool DoesModuleExist();

		protected:
			int m_nRequestGrabCount = 0;

			unsigned long m_nGrabCount = 0;

			void* m_pHikDevice = nullptr;
		};
	}
}
