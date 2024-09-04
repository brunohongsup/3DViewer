#pragma once

#include "DeviceCamera.h"

struct GENICAM_Camera;
struct GENICAM_AcquisitionControl;
struct GENICAM_StreamSource;

namespace Ravid
{
	namespace Algorithms
	{
		class CRavidImage;
	}

	namespace Device
	{
		class AFX_EXT_CLASS CDeviceDahuaGigE : public CDeviceCamera
		{
		public:
			RavidUseDynamicCreation();

			RavidPreventCopySelf(CDeviceDahuaGigE);

			DECLARE_DYNAMIC(CDeviceDahuaGigE)

		public:
			CDeviceDahuaGigE();
			virtual ~CDeviceDahuaGigE();

			virtual EDeviceInitializeResult Initialize() override;
			
			virtual EDeviceTerminateResult Terminate() override;

			virtual bool LoadSettings() override;

			virtual EDeviceGrabResult Grab() override;
			virtual EDeviceLiveResult Live() override;
			virtual EDeviceStopResult Stop() override;

			virtual EDeviceTriggerResult Trigger();
			
			virtual EDahuaGigEGetFunction GetGrabCount(_Out_ int* pParam);
			virtual EDahuaGigESetFunction SetGrabCount(_In_ int nParam);

			virtual EDahuaGigEGetFunction GetCanvasWidth(_Out_ int* pParam);
			virtual EDahuaGigESetFunction SetCanvasWidth(_In_ int nParam);

			virtual EDahuaGigEGetFunction GetCanvasHeight(_Out_ int* pParam);
			virtual EDahuaGigESetFunction SetCanvasHeight(_In_ int nParam);

			virtual EDahuaGigEGetFunction GetOffsetX(_Out_ int* pParam);
			virtual EDahuaGigESetFunction SetOffsetX(_In_ int nParam);

			virtual EDahuaGigEGetFunction GetOffsetY(_Out_ int* pParam);
			virtual EDahuaGigESetFunction SetOffsetY(_In_ int nParam);

			virtual EDahuaGigEGetFunction GetReverseX(_Out_ bool* pParam);
			virtual EDahuaGigESetFunction SetReverseX(_In_ bool bParam);

			virtual EDahuaGigEGetFunction GetReverseY(_Out_ bool* pParam);
			virtual EDahuaGigESetFunction SetReverseY(_In_ bool bParam);

			virtual EDahuaGigEGetFunction GetPixelFormat(_Out_ EDeviceDahuaGigEPixelFormat* pParam);
			virtual EDahuaGigESetFunction SetPixelFormat(_In_ EDeviceDahuaGigEPixelFormat eParam);

			virtual EDahuaGigEGetFunction GetAcquisitionMode(_Out_ EDeviceDahuaGigEAcquisitionMode* pParam);
			virtual EDahuaGigESetFunction SetAcquisitionMode(_In_ EDeviceDahuaGigEAcquisitionMode eParam);

			virtual EDahuaGigEGetFunction GetAcquisitionFrameCount(_Out_ int* pParam);
			virtual EDahuaGigESetFunction SetAcquisitionFrameCount(_In_ int nParam);

			virtual EDahuaGigEGetFunction GetAcquisitionFrameRate(_Out_ float* pParam);
			virtual EDahuaGigESetFunction SetAcquisitionFrameRate(_In_ float fParam);

			virtual EDahuaGigEGetFunction GetAcquisitionFrameRateEnable(_Out_ bool* pParam);
			virtual EDahuaGigESetFunction SetAcquisitionFrameRateEnable(_In_ bool bParam);

			virtual EDahuaGigEGetFunction GetAcquisitionStatusSelector(_Out_ EDeviceDahuaGigEAcquisitionSelector* pParam);
			virtual EDahuaGigESetFunction SetAcquisitionStatusSelector(_In_ EDeviceDahuaGigEAcquisitionSelector eParam);

			virtual EDahuaGigEGetFunction GetTriggerSelector(_Out_ EDeviceDahuaGigETriggerSelector* pParam);
			virtual EDahuaGigESetFunction SetTriggerSelector(_In_ EDeviceDahuaGigETriggerSelector eParam);

			virtual EDahuaGigEGetFunction GetFrameStartTriggerMode(_Out_ EDeviceDahuaGigETriggerMode* pParam);
			virtual EDahuaGigESetFunction SetFrameStartTriggerMode(_In_ EDeviceDahuaGigETriggerMode eParam);

			virtual EDahuaGigEGetFunction GetFrameStartTriggerSource(_Out_ EDeviceDahuaGigETriggerSource* pParam);
			virtual EDahuaGigESetFunction SetFrameStartTriggerSource(_In_ EDeviceDahuaGigETriggerSource eParam);

			virtual EDahuaGigEGetFunction GetFrameStartTriggerActivation(_Out_ EDeviceDahuaGigETriggerActivation* pParam);
			virtual EDahuaGigESetFunction SetFrameStartTriggerActivation(_In_ EDeviceDahuaGigETriggerActivation eParam);

			virtual EDahuaGigEGetFunction GetFrameStartTriggerDelay(_Out_ float* pParam);
			virtual EDahuaGigESetFunction SetFrameStartTriggerDelay(_In_ float fParam);

			virtual EDahuaGigEGetFunction GetAcquisitionStartTriggerMode(_Out_ EDeviceDahuaGigETriggerMode* pParam);
			virtual EDahuaGigESetFunction SetAcquisitionStartTriggerMode(_In_ EDeviceDahuaGigETriggerMode eParam);

			virtual EDahuaGigEGetFunction GetAcquisitionStartTriggerSource(_Out_ EDeviceDahuaGigETriggerSource* pParam);
			virtual EDahuaGigESetFunction SetAcquisitionStartTriggerSource(_In_ EDeviceDahuaGigETriggerSource eParam);

			virtual EDahuaGigEGetFunction GetAcquisitionStartTriggerActivation(_Out_ EDeviceDahuaGigETriggerActivation* pParam);
			virtual EDahuaGigESetFunction SetAcquisitionStartTriggerActivation(_In_ EDeviceDahuaGigETriggerActivation eParam);

			virtual EDahuaGigEGetFunction GetAcquisitionStartTriggerDelay(_Out_ float* pParam);
			virtual EDahuaGigESetFunction SetAcquisitionStartTriggerDelay(_In_ float fParam);

			virtual EDahuaGigEGetFunction GetLightTriggerDelay(_Out_ float* pParam);
			virtual EDahuaGigESetFunction SetLightTriggerDelay(_In_ float fParam);

			virtual EDahuaGigEGetFunction GetExposureMode(_Out_ EDeviceDahuaGigEExposureMode* pParam);
			virtual EDahuaGigESetFunction SetExposureMode(_In_ EDeviceDahuaGigEExposureMode eParam);

			virtual EDahuaGigEGetFunction GetExposureTime(_Out_ float* pParam);
			virtual EDahuaGigESetFunction SetExposureTime(_In_ float fParam);

			virtual EDahuaGigEGetFunction GetExposureAuto(_Out_ EDeviceDahuaGigEExposureAuto* pParam);
			virtual EDahuaGigESetFunction SetExposureAuto(_In_ EDeviceDahuaGigEExposureAuto eParam);

			virtual EDahuaGigEGetFunction GetGainSelector(_Out_ EDeviceDahuaGigEGainSelector* pParam);
			virtual EDahuaGigESetFunction SetGainSelector(_In_ EDeviceDahuaGigEGainSelector eParam);

			virtual EDahuaGigEGetFunction GetAllGainRaw(_Out_ float* pParam);
			virtual EDahuaGigESetFunction SetAllGainRaw(_In_ float fParam);

			virtual EDahuaGigEGetFunction GetBlackLevelSelector(_Out_ EDeviceDahuaGigEBlackLevelSelector* pParam);
			virtual EDahuaGigESetFunction SetBlackLevelSelector(_In_ EDeviceDahuaGigEBlackLevelSelector eParam);

			virtual EDahuaGigEGetFunction GetBlackLevel(_Out_ int* pParam);
			virtual EDahuaGigESetFunction SetBlackLevel(_In_ int nParam);

			virtual EDahuaGigEGetFunction GetBlackLevelAuto(_Out_ EDeviceDahuaGigEBlackLevelAuto* pParam);
			virtual EDahuaGigESetFunction SetBlackLevelAuto(_In_ EDeviceDahuaGigEBlackLevelAuto eParam);

			virtual EDahuaGigEGetFunction GetGamma(_Out_ float* pParam);
			virtual EDahuaGigESetFunction SetGamma(_In_ float fParam);

			virtual EDahuaGigEGetFunction GetLineSelector(_Out_ EDeviceDahuaGigELineSelector* pParam);
			virtual EDahuaGigESetFunction SetLineSelector(_In_ EDeviceDahuaGigELineSelector eParam);

			virtual EDahuaGigEGetFunction GetLine0Inverter(_Out_ EDeviceDahuaGigELineInverter* pParam);
			virtual EDahuaGigESetFunction SetLine0Inverter(_In_ EDeviceDahuaGigELineInverter eParam);

			virtual EDahuaGigEGetFunction GetLine0Source(_Out_ EDeviceDahuaGigELineSource* pParam);
			virtual EDahuaGigESetFunction SetLine0Source(_In_ EDeviceDahuaGigELineSource eParam);

			virtual EDahuaGigEGetFunction GetLine0Format(_Out_ EDeviceDahuaGigELineFormat* pParam);
			virtual EDahuaGigESetFunction SetLine0Format(_In_ EDeviceDahuaGigELineFormat eParam);

			virtual EDahuaGigEGetFunction GetLine1Inverter(_Out_ EDeviceDahuaGigELineInverter* pParam);
			virtual EDahuaGigESetFunction SetLine1Inverter(_In_ EDeviceDahuaGigELineInverter eParam);

			virtual EDahuaGigEGetFunction GetLine1Source(_Out_ EDeviceDahuaGigELineSource* pParam);
			virtual EDahuaGigESetFunction SetLine1Source(_In_ EDeviceDahuaGigELineSource eParam);

			virtual EDahuaGigEGetFunction GetLine1Format(_Out_ EDeviceDahuaGigELineFormat* pParam);
			virtual EDahuaGigESetFunction SetLine1Format(_In_ EDeviceDahuaGigELineFormat eParam);

			virtual bool OnParameterChanged(_In_ int nParam, _In_ CString strValue) override;

		public:
			DECLARE_MESSAGE_MAP()

		protected:
			virtual bool DoesModuleExist();

		protected:
			bool GetDeviceValueBool(_In_ CString strCommand, _Out_ bool* pData);
			bool SetDeviceValueBool(_In_ CString strCommand, _In_ bool bData);
						
			bool GetDeviceValueInt(_In_ CString strCommand, _Out_ int* pData);
			bool SetDeviceValueInt(_In_ CString strCommand, _In_ int nData);

			bool GetDeviceValueDouble(_In_ CString strCommand, _Out_ double* pData);
			bool SetDeviceValueDouble(_In_ CString strCommand, _In_ double dblData);

			bool GetDeviceValueEnum(_In_ CString strCommand, _Out_ CString* pData);
			bool SetDeviceValueEnum(_In_ CString strCommand, _In_ CString strData);

			bool GetDeviceValueString(_In_ CString strCommand, _Out_ CString* pData);
			bool SetDeviceValueString(_In_ CString strCommand, _In_ CString strData);

			static UINT LiveThread(_In_ LPVOID pParam);

		protected:

			bool m_bColorCamera = false;

			volatile int m_nGrabCount = 0;

			volatile int32_t m_nWaitTime = 0;

			CWinThread* m_pLiveThread = nullptr;

			GENICAM_Camera* m_pCamera = NULL;
			GENICAM_StreamSource* m_pStreamSource = NULL;
			GENICAM_AcquisitionControl* m_pAcquisitionCtrl = NULL;
		};
	}
}
