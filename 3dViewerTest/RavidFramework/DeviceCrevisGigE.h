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
		class AFX_EXT_CLASS CDeviceCrevisGigE : public CDeviceCamera
		{
		public:
			RavidUseDynamicCreation();

			RavidPreventCopySelf(CDeviceCrevisGigE);

			DECLARE_DYNAMIC(CDeviceCrevisGigE)

		public:
			CDeviceCrevisGigE();
			virtual ~CDeviceCrevisGigE();

			virtual EDeviceInitializeResult Initialize() override;
			
			virtual EDeviceTerminateResult Terminate() override;

			virtual bool LoadSettings() override;

			virtual EDeviceGrabResult Grab() override;
			virtual EDeviceLiveResult Live() override;
			virtual EDeviceStopResult Stop() override;

			virtual EDeviceTriggerResult Trigger();

		public:
			DECLARE_MESSAGE_MAP()
			
		public:			
			//General parameter
			ECrevisGigEGetFunction GetInitializeMode(_Out_ EDeviceCrevisGigEInitMode *pParam);
			ECrevisGigESetFunction SetInitializeMode(_In_ EDeviceCrevisGigEInitMode eParam);

			ECrevisGigEGetFunction GetAcqInvalidTime(_Out_ int *pParam);
			ECrevisGigESetFunction SetAcqInvalidTime(_In_ int nParam);

			ECrevisGigEGetFunction GetGrabTimeout(_Out_ int *pParam);
			ECrevisGigESetFunction SetGrabTimeout(_In_ int nParam);

			ECrevisGigEGetFunction GetDetailedLog(_Out_ bool *pParam);
			ECrevisGigESetFunction SetDetailedLog(_In_ bool bParam);

			ECrevisGigEGetFunction GetGevHeartbeatTimeout(_Out_ int *pParam);
			ECrevisGigESetFunction SetGevHeartbeatTimeout(_In_ int nParam);

			ECrevisGigEGetFunction GetGevGVCPHearteatDisable(_Out_ bool *pParam);
			ECrevisGigESetFunction SetGevGVCPHearteatDisable(_In_ bool bParam);

			ECrevisGigEGetFunction GetGevSCPSPacketSize(_Out_ int *pParam);
			ECrevisGigESetFunction SetGevSCPSPacketSize(_In_ int nParam);

			//Image format control
			ECrevisGigEGetFunction GetWidth(_Out_ int *pParam);
			ECrevisGigESetFunction SetWidth(_In_ int nParam);

			ECrevisGigEGetFunction GetHeight(_Out_ int *pParam);
			ECrevisGigESetFunction SetHeight(_In_ int nParam);

			ECrevisGigEGetFunction GetPixelFormat(_Out_ EDeviceCrevisGigEPixelFormat *pParam);
			ECrevisGigESetFunction SetPixelFormat(_In_ EDeviceCrevisGigEPixelFormat eParam);

			ECrevisGigEGetFunction GetOffsetX(_Out_ int *pParam);
			ECrevisGigESetFunction SetOffsetX(_In_ int nParam);

			ECrevisGigEGetFunction GetOffsetY(_Out_ int *pParam);
			ECrevisGigESetFunction SetOffsetY(_In_ int nParam);

			ECrevisGigEGetFunction GetTestPattern(_Out_ EDeviceCrevisGigETestPattern *pParam);
			ECrevisGigESetFunction SetTestPattern(_In_ EDeviceCrevisGigETestPattern eParam);

			//Acquisition control
			ECrevisGigEGetFunction GetAcquisitionMode(_Out_ EDeviceCrevisGigEAcquistionMode *pParam);
			ECrevisGigESetFunction SetAcquisitionMode(_In_ EDeviceCrevisGigEAcquistionMode eParam);

			ECrevisGigEGetFunction GetAcquisitionFrameCount(_Out_ int *pParam);
			ECrevisGigESetFunction SetAcquisitionFrameCount(_In_ int nParam);

			ECrevisGigEGetFunction GetTriggerSelector(_Out_ EDeviceCrevisGigETriggerSelector *pParam);
			ECrevisGigESetFunction SetTriggerSelector(_In_ EDeviceCrevisGigETriggerSelector eParam);

			ECrevisGigEGetFunction GetTriggerSource(_Out_ EDeviceCrevisGigETriggerSource *pParam);
			ECrevisGigESetFunction SetTriggerSource(_In_ EDeviceCrevisGigETriggerSource eParam);

			ECrevisGigEGetFunction GetLine1TriggerMode(_Out_ EDeviceCrevisGigETriggerMode *pParam);
			ECrevisGigESetFunction SetLine1TriggerMode(_In_ EDeviceCrevisGigETriggerMode eParam);

			ECrevisGigEGetFunction GetLine1TriggerActivation(_Out_ EDeviceCrevisGigETriggerActivation *pParam);
			ECrevisGigESetFunction SetLine1TriggerActivation(_In_ EDeviceCrevisGigETriggerActivation eParam);

			ECrevisGigEGetFunction GetLine1TriggerDelay(_Out_ float *pParam);
			ECrevisGigESetFunction SetLine1TriggerDelay(_In_ float fParam);

			ECrevisGigEGetFunction GetSoftwareTriggerMode(_Out_ EDeviceCrevisGigETriggerMode *pParam);
			ECrevisGigESetFunction SetSoftwareTriggerMode(_In_ EDeviceCrevisGigETriggerMode eParam);

			ECrevisGigEGetFunction GetSoftwareTriggerActivation(_Out_ EDeviceCrevisGigETriggerActivation *pParam);
			ECrevisGigESetFunction SetSoftwareTriggerActivation(_In_ EDeviceCrevisGigETriggerActivation eParam);

			ECrevisGigEGetFunction GetSoftwareTriggerDelay(_Out_ float *pParam);
			ECrevisGigESetFunction SetSoftwareTriggerDelay(_In_ float fParam);

			ECrevisGigEGetFunction GetExposureMode(_Out_ EDeviceCrevisGigEExposureMode *pParam);
			ECrevisGigESetFunction SetExposureMode(_In_ EDeviceCrevisGigEExposureMode eParam);

			ECrevisGigEGetFunction GetExposureAuto(_Out_ EDeviceCrevisGigEExposureAuto *pParam);
			ECrevisGigESetFunction SetExposureAuto(_In_ EDeviceCrevisGigEExposureAuto eParam);

			ECrevisGigEGetFunction GetExposureTime(_Out_ float *pParam);
			ECrevisGigESetFunction SetExposureTime(_In_ float fParam);

			ECrevisGigEGetFunction GetAcquisitionFrameRateEnable(_Out_ EDeviceCrevisGigEAcquistionFramerateEnable *pParam);
			ECrevisGigESetFunction SetAcquisitionFrameRateEnable(_In_ EDeviceCrevisGigEAcquistionFramerateEnable eParam);

			ECrevisGigEGetFunction GetAcquisitionFrameRate(_Out_ float *pParam);
			ECrevisGigESetFunction SetAcquisitionFrameRate(_In_ float fParam);

			ECrevisGigEGetFunction GetAutoExposureTarget(_Out_ int *pParam);
			ECrevisGigESetFunction SetAutoExposureTarget(_In_ int nParam);

			//Digital IO Control
			ECrevisGigEGetFunction GetLineSelector(_Out_ EDeviceCrevisGigELineSelector *pParam);
			ECrevisGigESetFunction SetLineSelector(_In_ EDeviceCrevisGigELineSelector eParam);

			ECrevisGigEGetFunction GetLineInverter(_Out_ bool *pParam);
			ECrevisGigESetFunction SetLineInverter(_In_ bool bParam);

			ECrevisGigEGetFunction GetLineSource(_Out_ EDeviceCrevisGigELineSource *pParam);
			ECrevisGigESetFunction SetLineSource(_In_ EDeviceCrevisGigELineSource eParam);

			ECrevisGigEGetFunction GetUserOutputSelector(_Out_ EDeviceCrevisGigEUserOutputSelector *pParam);
			ECrevisGigESetFunction SetUserOutputSelector(_In_ EDeviceCrevisGigEUserOutputSelector eParam);

			ECrevisGigEGetFunction GetTimerSelector(_Out_ EDeviceCrevisGigETimerSelector *pParam);
			ECrevisGigESetFunction SetTimerSelector(_In_ EDeviceCrevisGigETimerSelector eParam);

			ECrevisGigEGetFunction GetTimerDuration(_Out_ float *pParam);
			ECrevisGigESetFunction SetTimerDuration(_In_ float fParam);

			ECrevisGigEGetFunction GetTimerDelay(_Out_ float *pParam);
			ECrevisGigESetFunction SetTimerDelay(_In_ float fParam);

			//LUT Control
			ECrevisGigEGetFunction GetLUTEnable(_Out_ bool *pParam);
			ECrevisGigESetFunction SetLUTEnable(_In_ bool bParam);

			ECrevisGigEGetFunction GetLUTIndex(_Out_ int *pParam);
			ECrevisGigESetFunction SetLUTIndex(_In_ int nParam);

			ECrevisGigEGetFunction GetLUTValue(_Out_ int *pParam);
			ECrevisGigESetFunction SetLUTValue(_In_ int nParam);

			//Analog Control
			ECrevisGigEGetFunction GetGainselector(_Out_ EDeviceCrevisGigEGainSelector *pParam);
			ECrevisGigESetFunction SetGainselector(_In_ EDeviceCrevisGigEGainSelector eParam);

			ECrevisGigEGetFunction GetGainRaw(_Out_ int *pParam);
			ECrevisGigESetFunction SetGainRaw(_In_ int nParam);

			ECrevisGigEGetFunction GetGainAuto(_Out_ EDeviceCrevisGigEGainAuto *pParam);
			ECrevisGigESetFunction SetGainAuto(_In_ EDeviceCrevisGigEGainAuto eParam);

			ECrevisGigEGetFunction GetBlackLevelSelector(_Out_ EDeviceCrevisGigEBlackLevelSelector *pParam);
			ECrevisGigESetFunction SetBlackLevelSelector(_In_ EDeviceCrevisGigEBlackLevelSelector eParam);

			ECrevisGigEGetFunction GetBlackLevelRaw(_Out_ int *pParam);
			ECrevisGigESetFunction SetBlackLevelRaw(_In_ int nParam);

			//color only
			ECrevisGigEGetFunction GetBalanceRatioSelector(_Out_ EDeviceCrevisGigEBalanceRatioSelector *pParam);
			ECrevisGigESetFunction SetBalanceRatioSelector(_In_ EDeviceCrevisGigEBalanceRatioSelector eParam);

			ECrevisGigEGetFunction GetBalanceRatio(_Out_ float *pParam);
			ECrevisGigESetFunction SetBalanceRatio(_In_ float fParam);

			ECrevisGigEGetFunction GetBalanceWhiteAuto(_Out_ EDeviceCrevisGigEBalanceWhiteAuto *pParam);
			ECrevisGigESetFunction SetBalanceWhiteAuto(_In_ EDeviceCrevisGigEBalanceWhiteAuto eParam);

			//User control
			ECrevisGigEGetFunction GetUserSetSelector(_Out_ EDeviceCrevisGigEUserSetSelector *pParam);
			ECrevisGigESetFunction SetUserSetSelector(_In_ EDeviceCrevisGigEUserSetSelector eParam);

			//Device Option Control
			ECrevisGigEGetFunction GetDeviceFilterDriverMode(_Out_ EDeviceCrevisGigEDeviceFilterDriverMode *pParam);
			ECrevisGigESetFunction SetDeviceFilterDriverMode(_In_ EDeviceCrevisGigEDeviceFilterDriverMode eParam);

			ECrevisGigEGetFunction GetDeviceCommandTimeout(_Out_ int *pParam);
			ECrevisGigESetFunction SetDeviceCommandTimeout(_In_ int nParam);

			ECrevisGigEGetFunction GetDeviceCommandRetryNumber(_Out_ int *pParam);
			ECrevisGigESetFunction SetDeviceCommandRetryNumber(_In_ int nParam);

			ECrevisGigEGetFunction GetDeviceMissingPacketReceive(_Out_ EDeviceCrevisGigEDeviceMissingPackedReceive *pParam);
			ECrevisGigESetFunction SetDeviceMissingPacketReceive(_In_ EDeviceCrevisGigEDeviceMissingPackedReceive eParam);

			ECrevisGigEGetFunction GetDevicePacketResend(_Out_ bool *pParam);
			ECrevisGigESetFunction SetDevicePacketResend(_In_ bool bParam);

			ECrevisGigEGetFunction GetDeviceMaxPacketResendCount(_Out_ int *pParam);
			ECrevisGigESetFunction SetDeviceMaxPacketResendCount(_In_ int nParam);
			
			//Color Transformation Control
			ECrevisGigEGetFunction GetColorTransformationEnable(_Out_ bool *pParam);
			ECrevisGigESetFunction SetColorTransformationEnable(_In_ bool bParam);

			ECrevisGigEGetFunction GetColorTransformationValueSelector(_Out_ EDeviceCrevisGigEColorTransformationValueSeletor *pParam);
			ECrevisGigESetFunction SetColorTransformationValueSelector(_In_ EDeviceCrevisGigEColorTransformationValueSeletor eParam);

			ECrevisGigEGetFunction GetColorTransformationValue(_Out_ float *pParam);
			ECrevisGigESetFunction SetColorTransformationValue(_In_ float fParam);

			virtual bool OnParameterChanged(_In_ int nParam, _In_ CString strValue) override;
			
			static __int32 CallbackFunction(__int32 event, void* pImage, void* pUserDefine);

			virtual void UpdateDeviceToParameter();

		public: // lib api
			int __ST_SetIntReg(const char* NodeName, __int32 val);
			int __ST_GetIntReg(const char* NodeName, __int32 *pVal);
			int __ST_SetFloatReg(const char* NodeName, double fVal);
			int __ST_GetFloatReg(const char* NodeName, double *pFval);
			int __ST_SetBoolReg(const char* NodeName, bool bVal);
			int __ST_GetBoolReg(const char* NodeName, bool *pBval);
			int __ST_SetEnumReg(const char* NodeName, char* val);
			int __ST_GetEnumReg(const char* NodeName, char* pInfo, unsigned __int32* pSize);
			int __ST_SetStrReg(const char* NodeName, char* val);
			int __ST_GetStrReg(const char* NodeName, char* pInfo, unsigned __int32* pSize);
			int __ST_SetCmdReg(const char* NodeName);

		protected:
			virtual bool DoesModuleExist();

		protected:
			INT32 m_hDevice = -1;

			bool m_bColorCamera = false;
			
			unsigned long m_nGrabCount = 0;

			volatile int32_t m_nWaitTime = 0;
		};
	}
}
