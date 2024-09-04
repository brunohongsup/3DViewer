#pragma once

#include "DeviceCamera.h"
#include "../RavidCore/RavidPoint.h"


#define GENTL_INVALID_HANDLE  NULL

namespace GenTLConsumerImplHelper
{
	class CToFCamera;
}

namespace Ravid
{
	namespace Algorithms
	{
		class CRavidImage;
	}

	namespace Device
	{
		class AFX_EXT_CLASS CDeviceBaslerTOFGigE : public CDeviceCamera
		{
		public:
			RavidUseDynamicCreation();

			RavidPreventCopySelf(CDeviceBaslerTOFGigE);

			DECLARE_DYNAMIC(CDeviceBaslerTOFGigE)

		public:
			CDeviceBaslerTOFGigE();
			virtual ~CDeviceBaslerTOFGigE();

			virtual ptrdiff_t OnMessage(_In_ Ravid::Framework::CMessageBase* pMessage);

			virtual EDeviceInitializeResult Initialize() override;
			
			virtual EDeviceTerminateResult Terminate() override;

			virtual bool LoadSettings() override;

			virtual EDeviceGrabResult Grab() override;
			virtual EDeviceLiveResult Live() override;
			virtual EDeviceStopResult Stop() override;

			virtual EDeviceTriggerResult Trigger();

			virtual EBaslerTOFGetFunction GetDrawPoint(_Out_ bool* pParam);
			virtual EBaslerTOFSetFunction SetDrawPoint(_In_ bool bParam);

			virtual EBaslerTOFGetFunction GetDrawImage(_Out_ EDeviceBaslerTOFGigESelectImage* pParam);
			virtual EBaslerTOFSetFunction SetDrawImage(_In_ EDeviceBaslerTOFGigESelectImage eParam);

			virtual EBaslerTOFGetFunction GetDeviceChannel(_Out_ EDeviceBaslerTOFGigEDeviceChannel* pParam);
			virtual EBaslerTOFSetFunction SetDeviceChannel(_In_ EDeviceBaslerTOFGigEDeviceChannel eParam);

			virtual EBaslerTOFGetFunction GetCalibrationRangeOffset(_Out_ int* pParam);
			virtual EBaslerTOFSetFunction SetCalibrationRangeOffset(_In_ int nParam);

			virtual EBaslerTOFGetFunction GetRectification(_Out_ EDeviceBaslerTOFGigESwitch* pParam);
			virtual EBaslerTOFSetFunction SetRectification(_In_ EDeviceBaslerTOFGigESwitch eParam);

			virtual EBaslerTOFGetFunction GetDeviceTemperatureSelector(_Out_ EDeviceBaslerTOFGigETemperatureSelector* pParam);
			virtual EBaslerTOFSetFunction SetDeviceTemperatureSelector(_In_ EDeviceBaslerTOFGigETemperatureSelector eParam);

			virtual EBaslerTOFGetFunction GetBinning(_Out_ bool* pParam);
			virtual EBaslerTOFSetFunction SetBinning(_In_ bool bParam);

			virtual EBaslerTOFGetFunction GetCanvasWidth(_Out_ int* pParam);
			virtual EBaslerTOFSetFunction SetCanvasWidth(_In_ int nParam);

			virtual EBaslerTOFGetFunction GetCanvasHeight(_Out_ int* pParam);
			virtual EBaslerTOFSetFunction SetCanvasHeight(_In_ int nParam);

			virtual EBaslerTOFGetFunction GetOffsetX(_Out_ int* pParam);
			virtual EBaslerTOFSetFunction SetOffsetX(_In_ int nParam);

			virtual EBaslerTOFGetFunction GetOffsetY(_Out_ int* pParam);
			virtual EBaslerTOFSetFunction SetOffsetY(_In_ int nParam);

			virtual EBaslerTOFGetFunction GetMinimumDepth_mm(_Out_ int* pParam);
			virtual EBaslerTOFSetFunction SetMinimumDepth_mm(_In_ int nParam);

			virtual EBaslerTOFGetFunction GetMaximumDepth_mm(_Out_ int* pParam);
			virtual EBaslerTOFSetFunction SetMaximumDepth_mm(_In_ int nParam);

			virtual EBaslerTOFGetFunction GetComponentSelector(_Out_ EDeviceBaslerTOFGigEComponentSelector* pParam);
			virtual EBaslerTOFSetFunction SetComponentSelector(_In_ EDeviceBaslerTOFGigEComponentSelector eParam);

			virtual EBaslerTOFGetFunction GetLEDDisable(_Out_ bool* pParam);
			virtual EBaslerTOFSetFunction SetLEDDisable(_In_ bool bParam);

			virtual EBaslerTOFGetFunction GetProcessingMode(_Out_ EDeviceBaslerTOFGigEProcessingMode* pParam);
			virtual EBaslerTOFSetFunction SetProcessingMode(_In_ EDeviceBaslerTOFGigEProcessingMode eParam);

			virtual EBaslerTOFGetFunction GetAcquisitionFrameRate(_Out_ float* pParam);
			virtual EBaslerTOFSetFunction SetAcquisitionFrameRate(_In_ float fParam);

			virtual EBaslerTOFGetFunction GetExposureAuto(_Out_ EDeviceBaslerTOFGigEExposureAuto* pParam);
			virtual EBaslerTOFSetFunction SetExposureAuto(_In_ EDeviceBaslerTOFGigEExposureAuto eParam);

			virtual EBaslerTOFGetFunction GetAgility(_Out_ float* pParam);
			virtual EBaslerTOFSetFunction SetAgility(_In_ float fParam);

			virtual EBaslerTOFGetFunction GetDelay(_Out_ float* pParam);
			virtual EBaslerTOFSetFunction SetDelay(_In_ float fParam);

			virtual EBaslerTOFGetFunction GetExposureTimeSelector(_Out_ int* pParam);
			virtual EBaslerTOFSetFunction SetExposureTimeSelector(_In_ int nParam);

			virtual EBaslerTOFGetFunction GetExposureTime_us(_Out_ int* pParam);
			virtual EBaslerTOFSetFunction SetExposureTime_us(_In_ int nParam);

			virtual EBaslerTOFGetFunction GetTriggerMode(_Out_ EDeviceBaslerTOFGigESwitch* pParam);
			virtual EBaslerTOFSetFunction SetTriggerMode(_In_ EDeviceBaslerTOFGigESwitch eParam);

			virtual EBaslerTOFGetFunction GetTriggerSource(_Out_ EDeviceBaslerTOFGigETriggerSource* pParam);
			virtual EBaslerTOFSetFunction SetTriggerSource(_In_ EDeviceBaslerTOFGigETriggerSource eParam);

			virtual EBaslerTOFGetFunction GetTriggerDelay_us(_Out_ float* pParam);
			virtual EBaslerTOFSetFunction SetTriggerDelay_us(_In_ float fParam);

			virtual EBaslerTOFGetFunction GetTimestampLatch(_Out_ bool* pParam);
			virtual EBaslerTOFSetFunction SetTimestampLatch(_In_ bool nParam);

			virtual EBaslerTOFGetFunction GetTimestampLow_ns(_Out_ int* pParam);
			virtual EBaslerTOFSetFunction SetTimestampLow_ns(_In_ int nParam);

			virtual EBaslerTOFGetFunction GetTimestampHigh_ns(_Out_ int* pParam);
			virtual EBaslerTOFSetFunction SetTimestampHigh_ns(_In_ int nParam);

			virtual EBaslerTOFGetFunction GetStartTimeLow_us(_Out_ int* pParam);
			virtual EBaslerTOFSetFunction SetStartTimeLow_us(_In_ int nParam);

			virtual EBaslerTOFGetFunction GetStartTimeHigh_us(_Out_ int* pParam);
			virtual EBaslerTOFSetFunction SetStartTimeHigh_us(_In_ int nParam);

			virtual EBaslerTOFGetFunction GetTriggerRat_Hz(_Out_ float* pParam);
			virtual EBaslerTOFSetFunction SetTriggerRat_Hz(_In_ float fParam);

			virtual EBaslerTOFGetFunction GetUpdate(_Out_ bool* pParam);
			virtual EBaslerTOFSetFunction SetUpdate(_In_ bool bParam);

			virtual EBaslerTOFGetFunction GetConfidenceThreshold(_Out_ int* pParam);
			virtual EBaslerTOFSetFunction SetConfidenceThreshold(_In_ int nParam);

			virtual EBaslerTOFGetFunction GetSpatialFilter(_Out_ bool* pParam);
			virtual EBaslerTOFSetFunction SetSpatialFilter(_In_ bool bParam);

			virtual EBaslerTOFGetFunction GetTemporalFilter(_Out_ bool* pParam);
			virtual EBaslerTOFSetFunction SetTemporalFilter(_In_ bool bParam);

			virtual EBaslerTOFGetFunction GetStrength(_Out_ int* pParam);
			virtual EBaslerTOFSetFunction SetStrength(_In_ int nParam);

			virtual EBaslerTOFGetFunction GetOutlierTolerance(_Out_ int* pParam);
			virtual EBaslerTOFSetFunction SetOutlierTolerance(_In_ int nParam);

			virtual EBaslerTOFGetFunction GetRangeFilter(_Out_ bool* pParam);
			virtual EBaslerTOFSetFunction SetRangeFilter(_In_ bool bParam);

			virtual EBaslerTOFGetFunction GetLineSelector(_Out_ EDeviceBaslerTOFGigELineSelector* pParam);
			virtual EBaslerTOFSetFunction SetLineSelector(_In_ EDeviceBaslerTOFGigELineSelector eParam);

			virtual EBaslerTOFGetFunction GetLine1Inverter(_Out_ bool* pParam);
			virtual EBaslerTOFSetFunction SetLine1Inverter(_In_ bool bParam);

			virtual EBaslerTOFGetFunction GetLine1DebouncerTime_us(_Out_ float* pParam);
			virtual EBaslerTOFSetFunction SetLine1DebouncerTime_us(_In_ float fParam);

			virtual EBaslerTOFGetFunction GetLine2Inverter(_Out_ bool* pParam);
			virtual EBaslerTOFSetFunction SetLine2Inverter(_In_ bool bParam);

			virtual EBaslerTOFGetFunction GetLine2DebouncerTime_us(_Out_ float* pParam);
			virtual EBaslerTOFSetFunction SetLine2DebouncerTime_us(_In_ float fParam);

			virtual EBaslerTOFGetFunction GetOutputLine1Source(_Out_ EDeviceBaslerTOFGigELineSource* pParam);
			virtual EBaslerTOFSetFunction SetOutputLine1Source(_In_ EDeviceBaslerTOFGigELineSource eParam);

			virtual EBaslerTOFGetFunction GetOutputLine1Inverter(_Out_ bool* pParam);
			virtual EBaslerTOFSetFunction SetOutputLine1Inverter(_In_ bool bParam);

			virtual EBaslerTOFGetFunction GetOutputLine1OutputPulseMinWidth(_Out_ float* pParam);
			virtual EBaslerTOFSetFunction SetOutputLine1OutputPulseMinWidth(_In_ float fParam);

			virtual EBaslerTOFGetFunction GetOutputLine2Source(_Out_ EDeviceBaslerTOFGigELineSource* pParam);
			virtual EBaslerTOFSetFunction SetOutputLine2Source(_In_ EDeviceBaslerTOFGigELineSource eParam);

			virtual EBaslerTOFGetFunction GetOutputLine2Inverter(_Out_ bool* pParam);
			virtual EBaslerTOFSetFunction SetOutputLine2Inverter(_In_ bool bParam);

			virtual EBaslerTOFGetFunction GetOutputLine2OutputPulseMinWidth(_Out_ float* pParam);
			virtual EBaslerTOFSetFunction SetOutputLine2OutputPulseMinWidth(_In_ float fParam);

			virtual EBaslerTOFGetFunction GetUserOutputSelector(_Out_ EDeviceBaslerTOFGigEUserOutputSelector* pParam);
			virtual EBaslerTOFSetFunction SetUserOutputSelector(_In_ EDeviceBaslerTOFGigEUserOutputSelector eParam);

			virtual EBaslerTOFGetFunction GetUserOutput1Value(_Out_ bool* pParam);
			virtual EBaslerTOFSetFunction SetUserOutput1Value(_In_ bool bParam);

			virtual EBaslerTOFGetFunction GetUserOutput2Value(_Out_ bool* pParam);
			virtual EBaslerTOFSetFunction SetUserOutput2Value(_In_ bool bParam);

			virtual EBaslerTOFGetFunction GetUserOutputValueAll(_Out_ float* pParam);
			virtual EBaslerTOFSetFunction SetUserOutputValueAll(_In_ float fParam);
			
		public:		
			virtual bool GetInformation(_In_ ptrdiff_t x, _In_ ptrdiff_t y, _Out_ float* pXCoordinate, _Out_ float* pYCoordinate, _Out_ float* pZCoordinate, _Out_ float* pDistance);
			virtual bool GetInformation(_In_ CRavidPoint<int> point, _Out_ float* pXCoordinate, _Out_ float* pYCoordinate, _Out_ float* pZCoordinate, _Out_ float* pDistance);

			virtual void DrawPixelInfo(_In_ int nDrawIndex);

			virtual Ravid::Algorithms::CRavidImage* Get3DImageInfo();
			virtual Ravid::Algorithms::CRavidImage* GetIntensityInfo();
			virtual Ravid::Algorithms::CRavidImage* GetConfidenceInfo();

			virtual bool Save3DImageData(_In_ CString strFilePath, _Out_ Ravid::Algorithms::CRavidImage* pSaveImage);
			virtual bool Load3DImageData(_In_ CString strFilePath, _Out_ Ravid::Algorithms::CRavidImage* pLoadImage);

			virtual bool OnParameterChanged(_In_ int nParam, _In_ CString strValue) override;

		public:
			DECLARE_MESSAGE_MAP()

		protected:
			virtual bool DoesModuleExist();

			static UINT LiveThread(_In_ LPVOID pParam);

			virtual bool SetBoolean(_In_ const char* pCommand, _In_ bool bData);
			virtual bool SetInteger(_In_ const char* pCommand, _In_ int nData);
			virtual bool SetFloating(_In_ const char* pCommand, _In_ float fData);
			virtual bool SetEnumeration(_In_ const char* pCommand, _In_ const char* pData);

		protected:
			Ravid::Algorithms::CRavidImage* m_p3DInfo = nullptr;
			Ravid::Algorithms::CRavidImage* m_pConfidenceMap = nullptr;

			bool m_bDrawPoint = false;
			CRavidPoint<int> m_DrawPoint;

			bool m_bColorCamera = false;
			
			unsigned long m_nGrabCount = 0;

			volatile int32_t m_nWaitTime = 0;
			
			ptrdiff_t m_nCanvasX = 0;
			ptrdiff_t m_nCanvasY = 0;

			CWinThread* m_pLiveThread = nullptr;

			GenTLConsumerImplHelper::CToFCamera* m_pCamera = nullptr;
		};
	}
}
