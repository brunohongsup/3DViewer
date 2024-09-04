#pragma once

#include "DeviceCamera.h"

typedef void* fc2Context;

namespace Ravid
{
	namespace Algorithms
	{
		class CRavidImage;
	}

	namespace Device
	{
		class AFX_EXT_CLASS CDevicePointGrey : public CDeviceCamera
		{
		public:
			RavidUseDynamicCreation();

			RavidPreventCopySelf(CDevicePointGrey);

			DECLARE_DYNAMIC(CDevicePointGrey)

		public:
			CDevicePointGrey();
			virtual ~CDevicePointGrey();

			virtual EDeviceInitializeResult Initialize() override;
			virtual EDeviceTerminateResult Terminate() override;

			virtual bool LoadSettings() override;

			virtual EDeviceGrabResult Grab() override;
			virtual EDeviceLiveResult Live() override;
			virtual EDeviceStopResult Stop() override;

			virtual EDeviceTriggerResult Trigger();

		public:

			virtual EPointGreyGetFunction GetDeviceID(_Out_ unsigned int* pDeviceID);
			virtual EPointGreySetFunction SetDeviceID(_In_ int nDeviceID);

			virtual EPointGreyGetFunction GetGrabCount(_Out_ unsigned int* pGrabCount);
			virtual EPointGreySetFunction SetGrabCount(_In_ int nGrabCount);

			virtual EPointGreyGetFunction GetPacketSize(_Out_ unsigned int* pPacketSize);
			virtual EPointGreySetFunction SetPacketSize(_In_ unsigned int nPacketSize);

			virtual EPointGreyGetFunction GetPacketDelay(_Out_ unsigned int* pPacketDelay);
			virtual EPointGreySetFunction SetPacketDelay(_In_ unsigned int nPacketDelay);

			virtual EPointGreyGetFunction GetGrabWaitingTime(_Out_ unsigned int* pGrabWaitingTime);
			virtual EPointGreySetFunction SetGrabWaitingTime(_In_ int nGrabWaitingTime);

			virtual EPointGreyGetFunction GetGrabMode(_Out_ EDevicePointGreyGrabMode* pParam);
			virtual EPointGreySetFunction SetGrabMode(_In_ EDevicePointGreyGrabMode eParam);

			virtual EPointGreyGetFunction GetGrabNumBuffers(_Out_ unsigned int* pGrabNumBuffers);
			virtual EPointGreySetFunction SetGrabNumBuffers(_In_ unsigned int nGrabNumBuffers);

			virtual EPointGreyGetFunction GetCanvasWidth(_Out_ unsigned int* pCanvasWidth);
			virtual EPointGreySetFunction SetCanvasWidth(_In_ unsigned int nCanvasWidth);

			virtual EPointGreyGetFunction GetCanvasHeight(_Out_ unsigned int* pCanvasHeight);
			virtual EPointGreySetFunction SetCanvasHeight(_In_ unsigned int nCanvasHeight);

			virtual EPointGreyGetFunction GetOffsetX(_Out_ unsigned int* pOffsetX);
			virtual EPointGreySetFunction SetOffsetX(_In_ unsigned int nOffsetX);

			virtual EPointGreyGetFunction GetOffsetY(_Out_ unsigned int* pOffsetY);
			virtual EPointGreySetFunction SetOffsetY(_In_ unsigned int nOffsetY);

			virtual EPointGreyGetFunction GetPixelFormat(_Out_ EDevicePointGreyPixelFormat* pParam);
			virtual EPointGreySetFunction SetPixelFormat(_In_ EDevicePointGreyPixelFormat eParam);

			virtual EPointGreyGetFunction GetFormatMode(_Out_ EDevicePointGreyImagemode* pParam);
			virtual EPointGreySetFunction SetFormatMode(_In_ EDevicePointGreyImagemode eParam);

			virtual EPointGreyGetFunction GetTriggerOnOff(_Out_ bool* pOn);
			virtual EPointGreySetFunction SetTriggerOnOff(_In_ bool bOn);

			virtual EPointGreyGetFunction GetTriggerMode(_Out_ unsigned int* pTriggerMode);
			virtual EPointGreySetFunction SetTriggerMode(_In_ int nTriggerMode);

			virtual EPointGreyGetFunction GetTriggerSource(_Out_ unsigned int* pTriggerSource);
			virtual EPointGreySetFunction SetTriggerSource(_In_ int nTriggerSource);  //trigger source gpio pin

			virtual EPointGreyGetFunction GetTriggerParameter(_Out_ unsigned int* pTriggerParameter);
			virtual EPointGreySetFunction SetTriggerParameter(_In_ int nTriggerParameter); //trigger mode multi-shot only

			virtual EPointGreyGetFunction GetTriggerPolarity(_Out_ unsigned int* pTriggerPolarity);
			virtual EPointGreySetFunction SetTriggerPolarity(_In_ int nTriggerPolarity);

			virtual EPointGreyGetFunction GetBrightnessValue(_Out_ unsigned int* pBrightness);
			virtual EPointGreySetFunction SetBrightnessValue(_In_ unsigned int nBrightness);

			virtual EPointGreyGetFunction GetBrightnessManual(_Out_ EDevicePointGreySwitch* pParam);
			virtual EPointGreySetFunction SetBrightnessManual(_In_ EDevicePointGreySwitch eParam);

			virtual EPointGreyGetFunction GetExposureValue(_Out_ unsigned int* pExposure);
			virtual EPointGreySetFunction SetExposureValue(_In_ unsigned int nExposure);

			virtual EPointGreyGetFunction GetExposureManual(_Out_ EDevicePointGreySwitch* pParam);
			virtual EPointGreySetFunction SetExposureManual(_In_ EDevicePointGreySwitch eParam);

			virtual EPointGreyGetFunction GetSharpnessValue(_Out_ unsigned int* pSharpness);
			virtual EPointGreySetFunction SetSharpnessValue(_In_ unsigned int nSharpness);

			virtual EPointGreyGetFunction GetSharpnessManual(_Out_ EDevicePointGreySwitch* pParam);
			virtual EPointGreySetFunction SetSharpnessManual(_In_ EDevicePointGreySwitch eParam);

			virtual EPointGreyGetFunction GetWhiteBalanceValue(_Out_ unsigned int* pWhiteBalance);
			virtual EPointGreySetFunction SetWhiteBalanceValue(_In_ unsigned int nWhiteBalance);

			virtual EPointGreyGetFunction GetWhiteBalanceManual(_Out_ EDevicePointGreySwitch* pParam);
			virtual EPointGreySetFunction SetWhiteBalanceManual(_In_ EDevicePointGreySwitch eParam);

			virtual EPointGreyGetFunction GetHueValue(_Out_ unsigned int* pHue);
			virtual EPointGreySetFunction SetHueValue(_In_ unsigned int nHue);

			virtual EPointGreyGetFunction GetHueManual(_Out_ EDevicePointGreySwitch* pParam);
			virtual EPointGreySetFunction SetHueManual(_In_ EDevicePointGreySwitch eParam);

			virtual EPointGreyGetFunction GetSaturationValue(_Out_ unsigned int* pSaturation);
			virtual EPointGreySetFunction SetSaturationValue(_In_ unsigned int nSaturation);

			virtual EPointGreyGetFunction GetSaturationManual(_Out_ EDevicePointGreySwitch* pParam);
			virtual EPointGreySetFunction SetSaturationManual(_In_ EDevicePointGreySwitch eParam);

			virtual EPointGreyGetFunction GetGammaValue(_Out_ unsigned int* pGamma);
			virtual EPointGreySetFunction SetGammaValue(_In_ unsigned int nGamma);

			virtual EPointGreyGetFunction GetGammaManual(_Out_ EDevicePointGreySwitch* pParam);
			virtual EPointGreySetFunction SetGammaManual(_In_ EDevicePointGreySwitch eParam);

			virtual EPointGreyGetFunction GetIrisValue(_Out_ unsigned int* pIris);
			virtual EPointGreySetFunction SetIrisValue(_In_ unsigned int nIris);

			virtual EPointGreyGetFunction GetIrisManual(_Out_ EDevicePointGreySwitch* pParam);
			virtual EPointGreySetFunction SetIrisManual(_In_ EDevicePointGreySwitch eParam);

			virtual EPointGreyGetFunction GetFocusValue(_Out_ unsigned int* pFocus);
			virtual EPointGreySetFunction SetFocusValue(_In_ unsigned int nFocus);

			virtual EPointGreyGetFunction GetFocusManual(_Out_ EDevicePointGreySwitch* pParam);
			virtual EPointGreySetFunction SetFocusManual(_In_ EDevicePointGreySwitch eParam);

			virtual EPointGreyGetFunction GetZoomValue(_Out_ unsigned int* pZoom);
			virtual EPointGreySetFunction SetZoomValue(_In_ unsigned int nZoom);

			virtual EPointGreyGetFunction GetZoomManual(_Out_ EDevicePointGreySwitch* pParam);
			virtual EPointGreySetFunction SetZoomManual(_In_ EDevicePointGreySwitch eParam);

			virtual EPointGreyGetFunction GetPanValue(_Out_ unsigned int* pPan);
			virtual EPointGreySetFunction SetPanValue(_In_ unsigned int nPan);

			virtual EPointGreyGetFunction GetPanManual(_Out_ EDevicePointGreySwitch* pParam);
			virtual EPointGreySetFunction SetPanManual(_In_ EDevicePointGreySwitch eParam);

			virtual EPointGreyGetFunction GetTiltValue(_Out_ unsigned int* pTilt);
			virtual EPointGreySetFunction SetTiltValue(_In_ unsigned int nTilt);

			virtual EPointGreyGetFunction GetTiltManual(_Out_ EDevicePointGreySwitch* pParam);
			virtual EPointGreySetFunction SetTiltManual(_In_ EDevicePointGreySwitch eParam);

			virtual EPointGreyGetFunction GetShutterValue(_Out_ unsigned int* pShutter);
			virtual EPointGreySetFunction SetShutterValue(_In_ unsigned int nShutter);

			virtual EPointGreyGetFunction GetShutterManual(_Out_ EDevicePointGreySwitch* pParam);
			virtual EPointGreySetFunction SetShutterManual(_In_ EDevicePointGreySwitch eParam);

			virtual EPointGreyGetFunction GetGainValue(_Out_ unsigned int* pGain);
			virtual EPointGreySetFunction SetGainValue(_In_ unsigned int nGain);

			virtual EPointGreyGetFunction GetGainManual(_Out_ EDevicePointGreySwitch* pParam);
			virtual EPointGreySetFunction SetGainManual(_In_ EDevicePointGreySwitch eParam);

			virtual EPointGreyGetFunction GetFrameRateValue(_Out_ unsigned int* pGain);
			virtual EPointGreySetFunction SetFrameRateValue(_In_ unsigned int nGain);

			virtual EPointGreyGetFunction GetFrameRateManual(_Out_ EDevicePointGreySwitch* pParam);
			virtual EPointGreySetFunction SetFrameRateManual(_In_ EDevicePointGreySwitch eParam);

			virtual EPointGreyGetFunction GetTriggerDelay(_Out_ unsigned int* pTriggerDelay);
			virtual EPointGreySetFunction SetTriggerDelay(_In_ int nTriggerDelay);

			virtual bool OnParameterChanged(_In_ int nParam, _In_ CString strValue) override;

		protected:
			virtual void SetUpdateParameter();

			bool PollForTriggerReady(fc2Context context);

		public:
			DECLARE_MESSAGE_MAP()

		protected:
			virtual bool DoesModuleExist();

			static UINT LiveThread(_In_ LPVOID pParam);

		protected:
			bool m_bColorCamera = false;

			unsigned int m_nGrabCount = 0;
			unsigned int m_nNumCameras = 0;

			bool m_bTriggerOnOff = false;
			bool m_bAbsoluteMode = false;

			CWinThread* m_pLiveThread = nullptr;

			fc2Context m_pContext = nullptr;
		};
	}
}
