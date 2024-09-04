#pragma once

#include "DeviceCamera.h"

typedef void* tPvHandle;

namespace Ravid
{
	namespace Algorithms
	{
		class CRavidImage;
	}

	namespace Device
	{
		class AFX_EXT_CLASS CDeviceAlliedGigE : public CDeviceCamera
		{
		public:
			RavidUseDynamicCreation();

			RavidPreventCopySelf(CDeviceAlliedGigE);

			DECLARE_DYNAMIC(CDeviceAlliedGigE)

		public:
			CDeviceAlliedGigE();
			virtual ~CDeviceAlliedGigE();

			virtual EDeviceInitializeResult Initialize() override;
			virtual EDeviceTerminateResult Terminate() override;

			virtual bool LoadSettings() override;

			virtual EDeviceGrabResult Grab() override;
			virtual EDeviceLiveResult Live() override;
			virtual EDeviceStopResult Stop() override;

			virtual EDeviceTriggerResult Trigger() override;

			EAlliedGigEGetFunction GetDeviceFindWaittingTime(_Out_ int* pParam);
			EAlliedGigESetFunction SetDeviceFindWaittingTime(_In_ int nParam);

			EAlliedGigEGetFunction GetGrabWaittingTime(_Out_ int* pParam);
			EAlliedGigESetFunction SetGrabWaittingTime(_In_ int nParam);

			EAlliedGigEGetFunction GetInitializeType(_Out_ EDeviceAlliedGigEInitializeType* pParam);
			EAlliedGigESetFunction SetInitializeType(_In_ EDeviceAlliedGigEInitializeType eParam);

			EAlliedGigEGetFunction GetCanvasWidth(_Out_ int* pParam);
			EAlliedGigESetFunction SetCanvasWidth(_In_ int nParam);

			EAlliedGigEGetFunction GetCanvasHeight(_Out_ int* pParam);
			EAlliedGigESetFunction SetCanvasHeight(_In_ int nParam);

			EAlliedGigEGetFunction GetRegionX(_Out_ int* pParam);
			EAlliedGigESetFunction SetRegionX(_In_ int nParam);

			EAlliedGigEGetFunction GetRegionY(_Out_ int* pParam);
			EAlliedGigESetFunction SetRegionY(_In_ int nParam);

			EAlliedGigEGetFunction GetBinningX(_Out_ int* pParam);
			EAlliedGigESetFunction SetBinningX(_In_ int nParam);

			EAlliedGigEGetFunction GetBinningY(_Out_ int* pParam);
			EAlliedGigESetFunction SetBinningY(_In_ int nParam);

			EAlliedGigEGetFunction GetDecimationHorizontal(_Out_ int* pParam);
			EAlliedGigESetFunction SetDecimationHorizontal(_In_ int nParam);

			EAlliedGigEGetFunction GetDecimationVertical(_Out_ int* pParam);
			EAlliedGigESetFunction SetDecimationVertical(_In_ int nParam);

			EAlliedGigEGetFunction GetPixelFormat(_Out_ EDeviceAlliedGigEPixelFormat* pParam);
			EAlliedGigESetFunction SetPixelFormat(_In_ EDeviceAlliedGigEPixelFormat eParam);

			EAlliedGigEGetFunction GetPacketSize(_Out_ int* pParam);
			EAlliedGigESetFunction SetPacketSize(_In_ int nParam);

			EAlliedGigEGetFunction GetAcqEndTriggerEvent(_Out_ EDeviceAlliedGigETriggerEvent* pParam);
			EAlliedGigESetFunction SetAcqEndTriggerEvent(_In_ EDeviceAlliedGigETriggerEvent eParam);

			EAlliedGigEGetFunction GetAcqEndTriggerMode(_Out_ EDeviceAlliedGigETriggerMode* pParam);
			EAlliedGigESetFunction SetAcqEndTriggerMode(_In_ EDeviceAlliedGigETriggerMode eParam);

			EAlliedGigEGetFunction GetAcqRecTriggerEvent(_Out_ EDeviceAlliedGigETriggerEvent* pParam);
			EAlliedGigESetFunction SetAcqRecTriggerEvent(_In_ EDeviceAlliedGigETriggerEvent eParam);

			EAlliedGigEGetFunction GetAcqRecTriggerMode(_Out_ EDeviceAlliedGigETriggerMode* pParam);
			EAlliedGigESetFunction SetAcqRecTriggerMode(_In_ EDeviceAlliedGigETriggerMode eParam);

			EAlliedGigEGetFunction GetAcqStartTriggerEvent(_Out_ EDeviceAlliedGigETriggerEvent* pParam);
			EAlliedGigESetFunction SetAcqStartTriggerEvent(_In_ EDeviceAlliedGigETriggerEvent eParam);

			EAlliedGigEGetFunction GetAcqStartTriggerMode(_Out_ EDeviceAlliedGigETriggerMode* pParam);
			EAlliedGigESetFunction SetAcqStartTriggerMode(_In_ EDeviceAlliedGigETriggerMode eParam);

			EAlliedGigEGetFunction GetFrameRate(_Out_ float* pParam);
			EAlliedGigESetFunction SetFrameRate(_In_ float fParam);

			EAlliedGigEGetFunction GetFrameStartTriggerDelay(_Out_ int* pParam);
			EAlliedGigESetFunction SetFrameStartTriggerDelay(_In_ int nParam);

			EAlliedGigEGetFunction GetFrameStartTriggerEvent(_Out_ EDeviceAlliedGigETriggerEvent* pParam);
			EAlliedGigESetFunction SetFrameStartTriggerEvent(_In_ EDeviceAlliedGigETriggerEvent eParam);

			EAlliedGigEGetFunction GetFrameStartTriggerMode(_Out_ EDeviceAlliedGigEFrameMode* pParam);
			EAlliedGigESetFunction SetFrameStartTriggerMode(_In_ EDeviceAlliedGigEFrameMode eParam);

			EAlliedGigEGetFunction GetFrameStartTriggerOverlap(_Out_ EDeviceAlliedGigETriggerOverlap* pParam);
			EAlliedGigESetFunction SetFrameStartTriggerOverlap(_In_ EDeviceAlliedGigETriggerOverlap eParam);

			EAlliedGigEGetFunction GetAcquisitionFrameCount(_Out_ int* pParam);
			EAlliedGigESetFunction SetAcquisitionFrameCount(_In_ int nParam);

			EAlliedGigEGetFunction GetAcquisitionMode(_Out_ EDeviceAlliedGigEAcquisitionMode* pParam);
			EAlliedGigESetFunction SetAcquisitionMode(_In_ EDeviceAlliedGigEAcquisitionMode eParam);

			EAlliedGigEGetFunction GetRecorderPreEventCount(_Out_ int* pParam);
			EAlliedGigESetFunction SetRecorderPreEventCount(_In_ int nParam);

			EAlliedGigEGetFunction GetColorTransformationMode(_Out_ EDeviceAlliedGigEColorTransformationMode* pParam);
			EAlliedGigESetFunction SetColorTransformationMode(_In_ EDeviceAlliedGigEColorTransformationMode eParam);

			EAlliedGigEGetFunction GetColorTransformationValueBB(_Out_ float* pParam);
			EAlliedGigESetFunction SetColorTransformationValueBB(_In_ float fParam);

			EAlliedGigEGetFunction GetColorTransformationValueBG(_Out_ float* pParam);
			EAlliedGigESetFunction SetColorTransformationValueBG(_In_ float fParam);

			EAlliedGigEGetFunction GetColorTransformationValueBR(_Out_ float* pParam);
			EAlliedGigESetFunction SetColorTransformationValueBR(_In_ float fParam);

			EAlliedGigEGetFunction GetColorTransformationValueGB(_Out_ float* pParam);
			EAlliedGigESetFunction SetColorTransformationValueGB(_In_ float fParam);

			EAlliedGigEGetFunction GetColorTransformationValueGG(_Out_ float* pParam);
			EAlliedGigESetFunction SetColorTransformationValueGG(_In_ float fParam);

			EAlliedGigEGetFunction GetColorTransformationValueGR(_Out_ float* pParam);
			EAlliedGigESetFunction SetColorTransformationValueGR(_In_ float fParam);

			EAlliedGigEGetFunction GetColorTransformationValueRB(_Out_ float* pParam);
			EAlliedGigESetFunction SetColorTransformationValueRB(_In_ float fParam);

			EAlliedGigEGetFunction GetColorTransformationValueRG(_Out_ float* pParam);
			EAlliedGigESetFunction SetColorTransformationValueRG(_In_ float fParam);

			EAlliedGigEGetFunction GetColorTransformationValueRR(_Out_ float* pParam);
			EAlliedGigESetFunction SetColorTransformationValueRR(_In_ float fParam);

			EAlliedGigEGetFunction GetDSPSubregionBottom(_Out_ int* pParam);
			EAlliedGigESetFunction SetDSPSubregionBottom(_In_ int nParam);

			EAlliedGigEGetFunction GetDSPSubregionLeft(_Out_ int* pParam);
			EAlliedGigESetFunction SetDSPSubregionLeft(_In_ int nParam);

			EAlliedGigEGetFunction GetDSPSubregionRight(_Out_ int* pParam);
			EAlliedGigESetFunction SetDSPSubregionRight(_In_ int nParam);

			EAlliedGigEGetFunction GetDSPSubregionTop(_Out_ int* pParam);
			EAlliedGigESetFunction SetDSPSubregionTop(_In_ int nParam);

			EAlliedGigEGetFunction GetExposureMode(_Out_ EDeviceAlliedGigEExposureMode* pParam);
			EAlliedGigESetFunction SetExposureMode(_In_ EDeviceAlliedGigEExposureMode eParam);

			EAlliedGigEGetFunction GetExposureValue(_Out_ int* pParam);
			EAlliedGigESetFunction SetExposureValue(_In_ int nParam);

			EAlliedGigEGetFunction GetExposureAutoAdjustTol(_Out_ int* pParam);
			EAlliedGigESetFunction SetExposureAutoAdjustTol(_In_ int nParam);

			EAlliedGigEGetFunction GetExposureAutoAlg(_Out_ EDeviceAlliedGigEExposureAutoAlg* pParam);
			EAlliedGigESetFunction SetExposureAutoAlg(_In_ EDeviceAlliedGigEExposureAutoAlg eParam);

			EAlliedGigEGetFunction GetExposureAutoMax(_Out_ int* pParam);
			EAlliedGigESetFunction SetExposureAutoMax(_In_ int nParam);

			EAlliedGigEGetFunction GetExposureAutoMin(_Out_ int* pParam);
			EAlliedGigESetFunction SetExposureAutoMin(_In_ int nParam);

			EAlliedGigEGetFunction GetExposureAutoOutliers(_Out_ int* pParam);
			EAlliedGigESetFunction SetExposureAutoOutliers(_In_ int nParam);

			EAlliedGigEGetFunction GetExposureAutoRate(_Out_ int* pParam);
			EAlliedGigESetFunction SetExposureAutoRate(_In_ int nParam);

			EAlliedGigEGetFunction GetExposureAutoTarget(_Out_ int* pParam);
			EAlliedGigESetFunction SetExposureAutoTarget(_In_ int nParam);

			EAlliedGigEGetFunction GetGainMode(_Out_ EDeviceAlliedGigEGainMode* pParam);
			EAlliedGigESetFunction SetGainMode(_In_ EDeviceAlliedGigEGainMode eParam);

			EAlliedGigEGetFunction GetGainValue(_Out_ int* pParam);
			EAlliedGigESetFunction SetGainValue(_In_ int nParam);

			EAlliedGigEGetFunction GetGainAutoAdjustTol(_Out_ int* pParam);
			EAlliedGigESetFunction SetGainAutoAdjustTol(_In_ int nParam);

			EAlliedGigEGetFunction GetGainAutoMax(_Out_ int* pParam);
			EAlliedGigESetFunction SetGainAutoMax(_In_ int nParam);

			EAlliedGigEGetFunction GetGainAutoMin(_Out_ int* pParam);
			EAlliedGigESetFunction SetGainAutoMin(_In_ int nParam);

			EAlliedGigEGetFunction GetGainAutoOutliers(_Out_ int* pParam);
			EAlliedGigESetFunction SetGainAutoOutliers(_In_ int nParam);

			EAlliedGigEGetFunction GetGainAutoRate(_Out_ int* pParam);
			EAlliedGigESetFunction SetGainAutoRate(_In_ int nParam);

			EAlliedGigEGetFunction GetGainAutoTarget(_Out_ int* pParam);
			EAlliedGigESetFunction SetGainAutoTarget(_In_ int nParam);

			EAlliedGigEGetFunction GetIrisAutoTarget(_Out_ int* pParam);
			EAlliedGigESetFunction SetIrisAutoTarget(_In_ int nParam);

			EAlliedGigEGetFunction GetIrisMode(_Out_ EDeviceAlliedGigEIrisMode* pParam);
			EAlliedGigESetFunction SetIrisMode(_In_ EDeviceAlliedGigEIrisMode eParam);

			EAlliedGigEGetFunction GetIrisVideoLevelMax(_Out_ int* pParam);
			EAlliedGigESetFunction SetIrisVideoLevelMax(_In_ int nParam);

			EAlliedGigEGetFunction GetIrisVideoLevelMin(_Out_ int* pParam);
			EAlliedGigESetFunction SetIrisVideoLevelMin(_In_ int nParam);

			EAlliedGigEGetFunction GetLUTEnable(_Out_ bool* pParam);
			EAlliedGigESetFunction SetLUTEnable(_In_ bool bParam);

			EAlliedGigEGetFunction GetLUTIndex(_Out_ int* pParam);
			EAlliedGigESetFunction SetLUTIndex(_In_ int nParam);

			EAlliedGigEGetFunction GetLUTMode(_Out_ EDeviceAlliedGigELUTMode* pParam);
			EAlliedGigESetFunction SetLUTMode(_In_ EDeviceAlliedGigELUTMode eParam);

			EAlliedGigEGetFunction GetLUTSelector(_Out_ EDeviceAlliedGigELUTSelector* pParam);
			EAlliedGigESetFunction SetLUTSelector(_In_ EDeviceAlliedGigELUTSelector eParam);

			EAlliedGigEGetFunction GetLUTValue(_Out_ int* pParam);
			EAlliedGigESetFunction SetLUTValue(_In_ int nParam);

			EAlliedGigEGetFunction GetEdgeFilter(_Out_ EDeviceAlliedGigEEdgeFilter* pParam);
			EAlliedGigESetFunction SetEdgeFilter(_In_ EDeviceAlliedGigEEdgeFilter eParam);

			EAlliedGigEGetFunction GetGamma(_Out_ float* pParam);
			EAlliedGigESetFunction SetGamma(_In_ float fParam);

			EAlliedGigEGetFunction GetHue(_Out_ float* pParam);
			EAlliedGigESetFunction SetHue(_In_ float fParam);

			EAlliedGigEGetFunction GetOffsetValue(_Out_ int* pParam);
			EAlliedGigESetFunction SetOffsetValue(_In_ int nParam);

			EAlliedGigEGetFunction GetSaturation(_Out_ float* pParam);
			EAlliedGigESetFunction SetSaturation(_In_ float fParam);

			EAlliedGigEGetFunction GetWhitebalMode(_Out_ EDeviceAlliedGigEWhitebalMode* pParam);
			EAlliedGigESetFunction SetWhitebalMode(_In_ EDeviceAlliedGigEWhitebalMode eParam);

			EAlliedGigEGetFunction GetWhitebalValueBlue(_Out_ int* pParam);
			EAlliedGigESetFunction SetWhitebalValueBlue(_In_ int nParam);

			EAlliedGigEGetFunction GetWhitebalValueRed(_Out_ int* pParam);
			EAlliedGigESetFunction SetWhitebalValueRed(_In_ int nParam);

			EAlliedGigEGetFunction GetWhitebalAutoAdjustTol(_Out_ int* pParam);
			EAlliedGigESetFunction SetWhitebalAutoAdjustTol(_In_ int nParam);

			EAlliedGigEGetFunction GetWhitebalAutoRate(_Out_ int* pParam);
			EAlliedGigESetFunction SetWhitebalAutoRate(_In_ int nParam);

			EAlliedGigEGetFunction GetBandwidthCtrlMode(_Out_ EDeviceAlliedGigEBandwidthCtrlMode* pParam);
			EAlliedGigESetFunction SetBandwidthCtrlMode(_In_ EDeviceAlliedGigEBandwidthCtrlMode eParam);

			EAlliedGigEGetFunction GetHeartbeatInterval(_Out_ int* pParam);
			EAlliedGigESetFunction SetHeartbeatInterval(_In_ int nParam);

			EAlliedGigEGetFunction GetHeartbeatTimeout(_Out_ int* pParam);
			EAlliedGigESetFunction SetHeartbeatTimeout(_In_ int nParam);

			EAlliedGigEGetFunction GetGvcpRetries(_Out_ int* pParam);
			EAlliedGigESetFunction SetGvcpRetries(_In_ int nParam);

			EAlliedGigEGetFunction GetGvspLookbackWindow(_Out_ int* pParam);
			EAlliedGigESetFunction SetGvspLookbackWindow(_In_ int nParam);

			EAlliedGigEGetFunction GetGvspResendPercent(_Out_ float* pParam);
			EAlliedGigESetFunction SetGvspResendPercent(_In_ float fParam);

			EAlliedGigEGetFunction GetGvspRetries(_Out_ int* pParam);
			EAlliedGigESetFunction SetGvspRetries(_In_ int nParam);

			EAlliedGigEGetFunction GetGvspSocketBuffersCount(_Out_ EDeviceAlliedGigEGvspSocketBuffersCount* pParam);
			EAlliedGigESetFunction SetGvspSocketBuffersCount(_In_ EDeviceAlliedGigEGvspSocketBuffersCount eParam);

			EAlliedGigEGetFunction GetGvspTimeout(_Out_ int* pParam);
			EAlliedGigESetFunction SetGvspTimeout(_In_ int nParam);

			EAlliedGigEGetFunction GetStrobe1ControlledDuration(_Out_ EDeviceAlliedGigESwitch* pParam);
			EAlliedGigESetFunction SetStrobe1ControlledDuration(_In_ EDeviceAlliedGigESwitch eParam);

			EAlliedGigEGetFunction GetStrobe1Delay(_Out_ int* pParam);
			EAlliedGigESetFunction SetStrobe1Delay(_In_ int nParam);

			EAlliedGigEGetFunction GetStrobe1Duration(_Out_ int* pParam);
			EAlliedGigESetFunction SetStrobe1Duration(_In_ int nParam);

			EAlliedGigEGetFunction GetStrobe1Mode(_Out_ EDeviceAlliedGigEStrobeMode* pParam);
			EAlliedGigESetFunction SetStrobe1Mode(_In_ EDeviceAlliedGigEStrobeMode eParam);

			EAlliedGigEGetFunction GetSyncIn1GlitchFilter(_Out_ int* pParam);
			EAlliedGigESetFunction SetSyncIn1GlitchFilter(_In_ int nParam);

			EAlliedGigEGetFunction GetSyncIn2GlitchFilter(_Out_ int* pParam);
			EAlliedGigESetFunction SetSyncIn2GlitchFilter(_In_ int nParam);

			EAlliedGigEGetFunction GetSyncOut1Invert(_Out_ EDeviceAlliedGigESwitch* pParam);
			EAlliedGigESetFunction SetSyncOut1Invert(_In_ EDeviceAlliedGigESwitch eParam);

			EAlliedGigEGetFunction GetSyncOut1Mode(_Out_ EDeviceAlliedGigESyncOutMode* pParam);
			EAlliedGigESetFunction SetSyncOut1Mode(_In_ EDeviceAlliedGigESyncOutMode eParam);

			EAlliedGigEGetFunction GetSyncOut2Invert(_Out_ EDeviceAlliedGigESwitch* pParam);
			EAlliedGigESetFunction SetSyncOut2Invert(_In_ EDeviceAlliedGigESwitch eParam);

			EAlliedGigEGetFunction GetSyncOut2Mode(_Out_ EDeviceAlliedGigESyncOutMode* pParam);
			EAlliedGigESetFunction SetSyncOut2Mode(_In_ EDeviceAlliedGigESyncOutMode eParam);

			EAlliedGigEGetFunction GetSyncOutGpoLevels(_Out_ int* pParam);
			EAlliedGigESetFunction SetSyncOutGpoLevels(_In_ int nParam);
			
			EAlliedGigEGetFunction GetStreamBytesPerSecond(_Out_ int* pParam);
			EAlliedGigESetFunction SetStreamBytesPerSecond(_In_ int nParam);

			EAlliedGigEGetFunction GetStreamFrameRateConstrain(_Out_ bool* pParam);
			EAlliedGigESetFunction SetStreamFrameRateConstrain(_In_ bool bParam);

			EAlliedGigEGetFunction GetStreamHoldEnable(_Out_ EDeviceAlliedGigESwitch* pParam);
			EAlliedGigESetFunction SetStreamHoldEnable(_In_ EDeviceAlliedGigESwitch eParam);

			bool ConvertStringToVector(_In_ char* pString, _In_ int nStringSize, _Out_ std::vector<CString>* pVctString);

			virtual bool OnParameterChanged(_In_ _In_ int nParam, _In_ CString strValue) override;

		public:
			DECLARE_MESSAGE_MAP()

		protected:

			virtual bool DoesModuleExist();

			static UINT LiveThread(_In_ LPVOID pParam);

		protected:
			_In_ int m_nGrabCount = 0;

			CWinThread* m_pLiveThread = nullptr;

			tPvHandle m_hDevice;
			void* m_pFrames = nullptr;
		};
	}
}
