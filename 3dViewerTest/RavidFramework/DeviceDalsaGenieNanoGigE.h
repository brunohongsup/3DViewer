#pragma once

#include "DeviceCamera.h"

class SapAcqDevice;
class SapAcquisition;
class SapBuffer;
class SapView;
class SapAcqToBuf;
class SapTransfer;
class SapFeature;
class SapXferCallbackInfo;
class SapColorConversion;
class SapProCallbackInfo;
class SapMyProcessing;
class SapProCallbackInfo;

namespace Ravid
{
	namespace Algorithms
	{
		class CRavidImage;
	}

	namespace Device
	{
		class AFX_EXT_CLASS CDeviceDalsaGenieNanoGigE : public CDeviceCamera
		{
		public:
			RavidUseDynamicCreation();

			RavidPreventCopySelf(CDeviceDalsaGenieNanoGigE);

			DECLARE_DYNAMIC(CDeviceDalsaGenieNanoGigE)

		public:
			DECLARE_MESSAGE_MAP()

		public:
			CDeviceDalsaGenieNanoGigE();
			virtual ~CDeviceDalsaGenieNanoGigE();

			virtual EDeviceInitializeResult Initialize() override;
			virtual EDeviceTerminateResult Terminate() override;

			virtual bool LoadSettings() override;

			virtual EDeviceGrabResult Grab() override;
			virtual EDeviceLiveResult Live() override;
			virtual EDeviceStopResult Stop() override;

			virtual EDeviceTriggerResult Trigger();

		public:
			virtual EDalsaGenieNanoGigEGetFunction GetGrabCount(_Out_ int* pParam);
			virtual EDalsaGenieNanoGigESetFunction SetGrabCount(_In_ int nParam);

			virtual EDalsaGenieNanoGigEGetFunction GetCanvasWidth(_Out_ int* pParam);
			virtual EDalsaGenieNanoGigESetFunction SetCanvasWidth(_In_ int nParam);

			virtual EDalsaGenieNanoGigEGetFunction GetCanvasHeight(_Out_ int* pParam);
			virtual EDalsaGenieNanoGigESetFunction SetCanvasHeight(_In_ int nParam);

			virtual EDalsaGenieNanoGigEGetFunction GetOffsetX(_Out_ int* pParam);
			virtual EDalsaGenieNanoGigESetFunction SetOffsetX(_In_ int nParam);
												   
			virtual EDalsaGenieNanoGigEGetFunction GetOffsetY(_Out_ int* pParam);
			virtual EDalsaGenieNanoGigESetFunction SetOffsetY(_In_ int nParam);

			virtual EDalsaGenieNanoGigEGetFunction GetFlipHorizontal(_Out_ bool* pParam);
			virtual EDalsaGenieNanoGigESetFunction SetFlipHorizontal(_In_ bool bParam);

			virtual EDalsaGenieNanoGigEGetFunction GetFlipVertical(_Out_ bool* pParam);
			virtual EDalsaGenieNanoGigESetFunction SetFlipVertical(_In_ bool bParam);

			virtual EDalsaGenieNanoGigEGetFunction GetPixelFormat(_Out_ EDalsaGenieNanoGigEPixelFormat* pParam);
			virtual EDalsaGenieNanoGigESetFunction SetPixelFormat(_In_ EDalsaGenieNanoGigEPixelFormat eParam);

			virtual EDalsaGenieNanoGigEGetFunction GetPacketSize(_Out_ int* pParam);
			virtual EDalsaGenieNanoGigESetFunction SetPacketSize(_In_ int nParam);

			virtual EDalsaGenieNanoGigEGetFunction GetDeviceBufferCount(_Out_ int* pParam);
			virtual EDalsaGenieNanoGigESetFunction SetDeviceBufferCount(_In_ int nParam);

			virtual EDalsaGenieNanoGigEGetFunction GetTriggerSelector(_Out_ EDalsaGenieNanoGigETriggerSelector* pParam);
			virtual EDalsaGenieNanoGigESetFunction SetTriggerSelector(_In_ EDalsaGenieNanoGigETriggerSelector eParam);

			virtual EDalsaGenieNanoGigEGetFunction GetFrameStartMode(_Out_ EDalsaGenieNanoGigESwitch* pParam);
			virtual EDalsaGenieNanoGigESetFunction SetFrameStartMode(_In_ EDalsaGenieNanoGigESwitch eParam);

			virtual EDalsaGenieNanoGigEGetFunction GetFrameStartTriggerSource(_Out_ EDalsaGenieNanoGigESource* pParam);
			virtual EDalsaGenieNanoGigESetFunction SetFrameStartTriggerSource(_In_ EDalsaGenieNanoGigESource eParam);

			virtual EDalsaGenieNanoGigEGetFunction GetFrameStartTriggerActivation(_Out_ EDalsaGenieNanoGigEActivation* pParam);
			virtual EDalsaGenieNanoGigESetFunction SetFrameStartTriggerActivation(_In_ EDalsaGenieNanoGigEActivation eParam);

			virtual EDalsaGenieNanoGigEGetFunction GetFrameBurstStartMode(_Out_ EDalsaGenieNanoGigESwitch* pParam);
			virtual EDalsaGenieNanoGigESetFunction SetFrameBurstStartMode(_In_ EDalsaGenieNanoGigESwitch eParam);

			virtual EDalsaGenieNanoGigEGetFunction GetFrameBurstStartTriggerSource(_Out_ EDalsaGenieNanoGigESource* pParam);
			virtual EDalsaGenieNanoGigESetFunction SetFrameBurstStartTriggerSource(_In_ EDalsaGenieNanoGigESource eParam);

			virtual EDalsaGenieNanoGigEGetFunction GetFrameBurstStartTriggerCount(_Out_ int* pParam);
			virtual EDalsaGenieNanoGigESetFunction SetFrameBurstStartTriggerCount(_In_ int nParam);

			virtual EDalsaGenieNanoGigEGetFunction GetAcquisitionStartMode(_Out_ EDalsaGenieNanoGigESwitch* pParam);
			virtual EDalsaGenieNanoGigESetFunction SetAcquisitionStartMode(_In_ EDalsaGenieNanoGigESwitch eParam);

			virtual EDalsaGenieNanoGigEGetFunction GetAcquisitionStartTriggerSource(_Out_ EDalsaGenieNanoGigESource* pParam);
			virtual EDalsaGenieNanoGigESetFunction SetAcquisitionStartTriggerSource(_In_ EDalsaGenieNanoGigESource eParam);
			
			virtual EDalsaGenieNanoGigEGetFunction GetFrameRateControl(_Out_ EDalsaGenieNanoGigEFrameRateControl* pParam);
			virtual EDalsaGenieNanoGigESetFunction SetFrameRateControl(_In_ EDalsaGenieNanoGigEFrameRateControl eParam);

			virtual EDalsaGenieNanoGigEGetFunction GetFrameRate(_Out_ double* pParam);
			virtual EDalsaGenieNanoGigESetFunction SetFrameRate(_In_ double dblParam);

			virtual EDalsaGenieNanoGigEGetFunction GetExposureTime(_Out_ double* pParam);
			virtual EDalsaGenieNanoGigESetFunction SetExposureTime(_In_ double dblParam);

			virtual EDalsaGenieNanoGigEGetFunction GetGainSelector(_Out_ EDalsaGenieNanoGigEGainSelector* pParam);
			virtual EDalsaGenieNanoGigESetFunction SetGainSelector(_In_ EDalsaGenieNanoGigEGainSelector eParam);

			virtual EDalsaGenieNanoGigEGetFunction GetSensorGain(_Out_ double* pParam);
			virtual EDalsaGenieNanoGigESetFunction SetSensorGain(_In_ double dblParam);

			virtual EDalsaGenieNanoGigEGetFunction GetDigitalGain(_Out_ double* pParam);
			virtual EDalsaGenieNanoGigESetFunction SetDigitalGain(_In_ double dblParam);

			virtual EDalsaGenieNanoGigEGetFunction GetBlackLevelSelector(_Out_ EDalsaGenieNanoGigEBlackLevelSelector* pParam);
			virtual EDalsaGenieNanoGigESetFunction SetBlackLevelSelector(_In_ EDalsaGenieNanoGigEBlackLevelSelector eParam);

			virtual EDalsaGenieNanoGigEGetFunction GetBlackLevel(_Out_ double* pParam);
			virtual EDalsaGenieNanoGigESetFunction SetBlackLevel(_In_ double dblParam);

			virtual EDalsaGenieNanoGigEGetFunction GetLineSelector(_Out_ EDalsaGenieNanoGigELineSelector* pParam);
			virtual EDalsaGenieNanoGigESetFunction SetLineSelector(_In_ EDalsaGenieNanoGigELineSelector eParam);

			virtual EDalsaGenieNanoGigEGetFunction GetLine1DebouncingPeriod(_Out_ int* pParam);
			virtual EDalsaGenieNanoGigESetFunction SetLine1DebouncingPeriod(_In_ int nParam);

			virtual EDalsaGenieNanoGigEGetFunction GetLine1Inverter(_Out_ EDalsaGenieNanoGigESwitch* pParam);
			virtual EDalsaGenieNanoGigESetFunction SetLine1Inverter(_In_ EDalsaGenieNanoGigESwitch eParam);

			virtual EDalsaGenieNanoGigEGetFunction GetLine2DebouncingPeriod(_Out_ int* pParam);
			virtual EDalsaGenieNanoGigESetFunction SetLine2DebouncingPeriod(_In_ int nParam);

			virtual EDalsaGenieNanoGigEGetFunction GetLine2Inverter(_Out_ EDalsaGenieNanoGigESwitch* pParam);
			virtual EDalsaGenieNanoGigESetFunction SetLine2Inverter(_In_ EDalsaGenieNanoGigESwitch eParam);

			virtual EDalsaGenieNanoGigEGetFunction GetLine3Source(_Out_ EDalsaGenieNanoGigELineSource* pParam);
			virtual EDalsaGenieNanoGigESetFunction SetLine3Source(_In_ EDalsaGenieNanoGigELineSource eParam);

			virtual EDalsaGenieNanoGigEGetFunction GetLine3Inverter(_Out_ EDalsaGenieNanoGigESwitch* pParam);
			virtual EDalsaGenieNanoGigESetFunction SetLine3Inverter(_In_ EDalsaGenieNanoGigESwitch eParam);

			virtual EDalsaGenieNanoGigEGetFunction GetLine3Activation(_Out_ EDalsaGenieNanoGigEActivation* pParam);
			virtual EDalsaGenieNanoGigESetFunction SetLine3Activation(_In_ EDalsaGenieNanoGigEActivation eParam);

			virtual EDalsaGenieNanoGigEGetFunction GetLine3Delay(_Out_ int* pParam);
			virtual EDalsaGenieNanoGigESetFunction SetLine3Delay(_In_ int nParam);

			virtual EDalsaGenieNanoGigEGetFunction GetLine3Duration(_Out_ int* pParam);
			virtual EDalsaGenieNanoGigESetFunction SetLine3Duration(_In_ int nParam);

			virtual EDalsaGenieNanoGigEGetFunction GetLine3Value(_Out_ EDalsaGenieNanoGigELineValue* pParam);
			virtual EDalsaGenieNanoGigESetFunction SetLine3Value(_In_ EDalsaGenieNanoGigELineValue eParam);

			virtual EDalsaGenieNanoGigEGetFunction GetLine4Source(_Out_ EDalsaGenieNanoGigELineSource* pParam);
			virtual EDalsaGenieNanoGigESetFunction SetLine4Source(_In_ EDalsaGenieNanoGigELineSource eParam);

			virtual EDalsaGenieNanoGigEGetFunction GetLine4Inverter(_Out_ EDalsaGenieNanoGigESwitch* pParam);
			virtual EDalsaGenieNanoGigESetFunction SetLine4Inverter(_In_ EDalsaGenieNanoGigESwitch eParam);

			virtual EDalsaGenieNanoGigEGetFunction GetLine4Activation(_Out_ EDalsaGenieNanoGigEActivation* pParam);
			virtual EDalsaGenieNanoGigESetFunction SetLine4Activation(_In_ EDalsaGenieNanoGigEActivation eParam);

			virtual EDalsaGenieNanoGigEGetFunction GetLine4Delay(_Out_ int* pParam);
			virtual EDalsaGenieNanoGigESetFunction SetLine4Delay(_In_ int nParam);

			virtual EDalsaGenieNanoGigEGetFunction GetLine4Duration(_Out_ int* pParam);
			virtual EDalsaGenieNanoGigESetFunction SetLine4Duration(_In_ int nParam);

			virtual EDalsaGenieNanoGigEGetFunction GetLine4Value(_Out_ EDalsaGenieNanoGigELineValue* pParam);
			virtual EDalsaGenieNanoGigESetFunction SetLine4Value(_In_ EDalsaGenieNanoGigELineValue eParam);

			virtual bool OnParameterChanged(_In_ int nParam, _In_ CString strValue) override;

		protected:
#ifndef _WIN64
			static void XferCallback(SapXferCallbackInfo *pInfo);
			static void ProCallback(SapProCallbackInfo *pInfo);
#else
			static void WINAPI XferCallback(SapXferCallbackInfo* pInfo);
			static void WINAPI ProCallback(SapProCallbackInfo* pInfo);
#endif
			virtual bool DoesModuleExist();

		protected:
			int m_nGrabCount = 0;

			bool m_bColorCamera = false;

			SapView* m_pView = nullptr;
			SapTransfer* m_pXfer = nullptr;
			SapBuffer* m_pBuffers = nullptr;
			SapMyProcessing* m_pProcessing = nullptr;
			SapAcqDevice* m_pDevice = nullptr;
			SapColorConversion* m_pColorConv = nullptr;
		};
	}
}
