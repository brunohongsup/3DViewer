#pragma once

#include "DeviceEuresysMulticam.h"

typedef UINT32 MCHANDLE, *PMCHANDLE;


namespace Ravid
{
	namespace Algorithms
	{
		class CRavidImage;
	}

	namespace Device
	{
		class AFX_EXT_CLASS CDeviceEuresysGrablink : public CDeviceEuresysMulticam
		{
		public:
			CDeviceEuresysGrablink();
			virtual ~CDeviceEuresysGrablink();

			virtual EDeviceInitializeResult Initialize() override;
			virtual EDeviceTerminateResult Terminate() override;

			virtual bool LoadSettings() override;

			virtual EEuresysGetFunction GetAcquisitionMode(_Out_ EDeviceGrablinkAcquisitionMode* pParam);
			virtual EEuresysSetFunction SetAcquisitionMode(_In_ EDeviceGrablinkAcquisitionMode eParam);

			virtual EEuresysGetFunction GetTrigMode(_Out_ EDeviceGrablinkTrigMode* pParam);
			virtual EEuresysSetFunction SetTrigMode(_In_ EDeviceGrablinkTrigMode eParam);

			virtual EEuresysGetFunction GetExpose_us(_Out_ int* pParam);
			virtual EEuresysSetFunction SetExpose_us(_In_ int nParam);

			virtual EEuresysGetFunction GetSeqLength_Fr(_Out_ int* pParam);
			virtual EEuresysSetFunction SetSeqLength_Fr(_In_ int nParam);

			virtual EEuresysGetFunction GetPhaseLength_Fr(_Out_ int* pParam);
			virtual EEuresysSetFunction SetPhaseLength_Fr(_In_ int nParam);

			virtual EEuresysGetFunction GetSeqLength_Ln(_Out_ int* pParam);
			virtual EEuresysSetFunction SetSeqLength_Ln(_In_ int nParam);

			virtual EEuresysGetFunction GetSeqLength_Pg(_Out_ int* pParam);
			virtual EEuresysSetFunction SetSeqLength_Pg(_In_ int nParam);

			virtual EEuresysGetFunction GetPageLength_Ln(_Out_ int* pParam);
			virtual EEuresysSetFunction SetPageLength_Ln(_In_ int nParam);

			virtual EEuresysGetFunction GetLineCaptureMode(_Out_ EDeviceGrablinkLineCaptureMode* pParam);
			virtual EEuresysSetFunction SetLineCaptureMode(_In_ EDeviceGrablinkLineCaptureMode eParam);

			virtual EEuresysGetFunction GetLineRateMode(_Out_ EDeviceGrablinkLineRateMode* pParam);
			virtual EEuresysSetFunction SetLineRateMode(_In_ EDeviceGrablinkLineRateMode eParam);

			virtual EEuresysGetFunction GetCamConfig(_Out_ EDeviceGrablinkCamConfig* pParam);
			virtual EEuresysSetFunction SetCamConfig(_In_ EDeviceGrablinkCamConfig eParam);

			virtual EEuresysGetFunction GetCamera(_Out_ EDeviceGrablinkCamera* pParam);
			virtual EEuresysSetFunction SetCamera(_In_ EDeviceGrablinkCamera eParam);

			virtual EEuresysGetFunction GetDataLink(_Out_ EDeviceGrablinkDataLink* pParam);
			virtual EEuresysSetFunction SetDataLink(_In_ EDeviceGrablinkDataLink eParam);

			virtual EEuresysGetFunction GetImaging(_Out_ EDeviceGrablinkImaging* pParam);
			virtual EEuresysSetFunction SetImaging(_In_ EDeviceGrablinkImaging eParam);

			virtual EEuresysGetFunction GetSpectrum(_Out_ EDeviceGrablinkSpectrum* pParam);
			virtual EEuresysSetFunction SetSpectrum(_In_ EDeviceGrablinkSpectrum eParam);

			virtual EEuresysGetFunction GetExposeRecovery_us(_Out_ int* pParam);
			virtual EEuresysSetFunction SetExposeRecovery_us(_In_ int nParam);

			virtual EEuresysGetFunction GetFrameRate_mHz(_Out_ int* pParam);
			virtual EEuresysSetFunction SetFrameRate_mHz(_In_ int nParam);

			virtual EEuresysGetFunction GetHactive_Px(_Out_ int* pParam);
			virtual EEuresysSetFunction SetHactive_Px(_In_ int nParam);

			virtual EEuresysGetFunction GetHsyncAft_Tk(_Out_ int* pParam);
			virtual EEuresysSetFunction SetHsyncAft_Tk(_In_ int nParam);

			virtual EEuresysGetFunction GetPixelClkMode(_Out_ EDeviceGrablinkPixelClkMode* pParam);
			virtual EEuresysSetFunction SetPixelClkMode(_In_ EDeviceGrablinkPixelClkMode eParam);

			virtual EEuresysGetFunction GetReadoutRecovery_us(_Out_ int* pParam);
			virtual EEuresysSetFunction SetReadoutRecovery_us(_In_ int nParam);

			virtual EEuresysGetFunction GetVactive_Ln(_Out_ int* pParam);
			virtual EEuresysSetFunction SetVactive_Ln(_In_ int nParam);

			virtual EEuresysGetFunction GetVsyncAft_Ln(_Out_ int* pParam);
			virtual EEuresysSetFunction SetVsyncAft_Ln(_In_ int nParam);

			virtual EEuresysGetFunction GetLineRate_Hz(_Out_ int* pParam);
			virtual EEuresysSetFunction SetLineRate_Hz(_In_ int nParam);

			virtual EEuresysGetFunction GetTapConfiguration(_Out_ EDeviceGrablinkTapConfiguration* pParam);
			virtual EEuresysSetFunction SetTapConfiguration(_In_ EDeviceGrablinkTapConfiguration eParam);

			virtual EEuresysGetFunction GetAuxResetCtl(_Out_ EDeviceGrablinkAuxResetCtl* pParam);
			virtual EEuresysSetFunction SetAuxResetCtl(_In_ EDeviceGrablinkAuxResetCtl eParam);

			virtual EEuresysGetFunction GetCC1Usage(_Out_ EDeviceGrablinkCC1Usage* pParam);
			virtual EEuresysSetFunction SetCC1Usage(_In_ EDeviceGrablinkCC1Usage eParam);

			virtual EEuresysGetFunction GetCC2Usage(_Out_ EDeviceGrablinkCC2Usage* pParam);
			virtual EEuresysSetFunction SetCC2Usage(_In_ EDeviceGrablinkCC2Usage eParam);

			virtual EEuresysGetFunction GetCC3Usage(_Out_ EDeviceGrablinkCC3Usage* pParam);
			virtual EEuresysSetFunction SetCC3Usage(_In_ EDeviceGrablinkCC3Usage eParam);

			virtual EEuresysGetFunction GetCC4Usage(_Out_ EDeviceGrablinkCC4Usage* pParam);
			virtual EEuresysSetFunction SetCC4Usage(_In_ EDeviceGrablinkCC4Usage eParam);

			virtual EEuresysGetFunction GetColorMethod(_Out_ EDeviceGrablinkColorMethod* pParam);
			virtual EEuresysSetFunction SetColorMethod(_In_ EDeviceGrablinkColorMethod eParam);

			virtual EEuresysGetFunction GetColorRegistrationControl(_Out_ EDeviceGrablinkColorRegistrationControl* pParam);
			virtual EEuresysSetFunction SetColorRegistrationControl(_In_ EDeviceGrablinkColorRegistrationControl eParam);

			virtual EEuresysGetFunction GetDvalMode(_Out_ EDeviceGrablinkDvalMode* pParam);
			virtual EEuresysSetFunction SetDvalMode(_In_ EDeviceGrablinkDvalMode eParam);

			virtual EEuresysGetFunction GetExpose(_Out_ EDeviceGrablinkExpose* pParam);
			virtual EEuresysSetFunction SetExpose(_In_ EDeviceGrablinkExpose eParam);

			virtual EEuresysGetFunction GetExposeMin_us(_Out_ int* pParam);
			virtual EEuresysSetFunction SetExposeMin_us(_In_ int nParam);

			virtual EEuresysGetFunction GetExposeMax_us(_Out_ int* pParam);
			virtual EEuresysSetFunction SetExposeMax_us(_In_ int nParam);

			virtual EEuresysGetFunction GetExposeOverlap(_Out_ EDeviceGrablinkExposeOverlap* pParam);
			virtual EEuresysSetFunction SetExposeOverlap(_In_ EDeviceGrablinkExposeOverlap eParam);

			virtual EEuresysGetFunction GetFvalMode(_Out_ EDeviceGrablinkFvalMode* pParam);
			virtual EEuresysSetFunction SetFvalMode(_In_ EDeviceGrablinkFvalMode eParam);

			virtual EEuresysGetFunction GetLvalMode(_Out_ EDeviceGrablinkLvalMode* pParam);
			virtual EEuresysSetFunction SetLvalMode(_In_ EDeviceGrablinkLvalMode eParam);

			virtual EEuresysGetFunction GetReadout(_Out_ EDeviceGrablinkReadout* pParam);
			virtual EEuresysSetFunction SetReadout(_In_ EDeviceGrablinkReadout eParam);

			virtual EEuresysGetFunction GetResetCtl(_Out_ EDeviceGrablinkResetCtl* pParam);
			virtual EEuresysSetFunction SetResetCtl(_In_ EDeviceGrablinkResetCtl eParam);

			virtual EEuresysGetFunction GetResetEdge(_Out_ EDeviceGrablinkResetEdge* pParam);
			virtual EEuresysSetFunction SetResetEdge(_In_ EDeviceGrablinkResetEdge eParam);

			virtual EEuresysGetFunction GetTwoLineSynchronization(_Out_ EDeviceGrablinkTwoLineSynchronization* pParam);
			virtual EEuresysSetFunction SetTwoLineSynchronization(_In_ EDeviceGrablinkTwoLineSynchronization eParam);

			virtual EEuresysGetFunction GetTapGeometry(_Out_ EDeviceGrablinkTapGeometry* pParam);
			virtual EEuresysSetFunction SetTapGeometry(_In_ EDeviceGrablinkTapGeometry eParam);

			virtual EEuresysGetFunction GetAuxresetLine(_Out_ EDeviceGrablinkAuxresetLine* pParam);
			virtual EEuresysSetFunction SetAuxresetLine(_In_ EDeviceGrablinkAuxresetLine eParam);

			virtual EEuresysGetFunction GetResetLine(_Out_ EDeviceGrablinkResetLine* pParam);
			virtual EEuresysSetFunction SetResetLine(_In_ EDeviceGrablinkResetLine eParam);

			virtual EEuresysGetFunction GetActivityLength(_Out_ int* pParam);
			virtual EEuresysSetFunction SetActivityLength(_In_ int nParam);

			virtual EEuresysGetFunction GetBreakEffect(_Out_ EDeviceGrablinkBreakEffect* pParam);
			virtual EEuresysSetFunction SetBreakEffect(_In_ EDeviceGrablinkBreakEffect eParam);

			virtual EEuresysGetFunction GetEndTrigMode(_Out_ EDeviceGrablinkEndTrigMode* pParam);
			virtual EEuresysSetFunction SetEndTrigMode(_In_ EDeviceGrablinkEndTrigMode eParam);

			virtual EEuresysGetFunction GetNextTrigMode(_Out_ EDeviceGrablinkNextTrigMode* pParam);
			virtual EEuresysSetFunction SetNextTrigMode(_In_ EDeviceGrablinkNextTrigMode eParam);

			virtual EEuresysGetFunction GetSynchronizedAcquisition(_Out_ EDeviceGrablinkSynchronizedAcquisition* pParam);
			virtual EEuresysSetFunction SetSynchronizedAcquisition(_In_ EDeviceGrablinkSynchronizedAcquisition eParam);

			virtual EEuresysGetFunction GetForceTrig(_Out_ EDeviceGrablinkForceTrig* pParam);
			virtual EEuresysSetFunction SetForceTrig(_In_ EDeviceGrablinkForceTrig eParam);

			virtual EEuresysGetFunction GetNextTrigDelay_Pls(_Out_ int* pParam);
			virtual EEuresysSetFunction SetNextTrigDelay_Pls(_In_ int nParam);

			virtual EEuresysGetFunction GetTrigCtl(_Out_ EDeviceGrablinkTrigCtl* pParam);
			virtual EEuresysSetFunction SetTrigCtl(_In_ EDeviceGrablinkTrigCtl eParam);

			virtual EEuresysGetFunction GetTrigDelay_Pls(_Out_ int* pParam);
			virtual EEuresysSetFunction SetTrigDelay_Pls(_In_ int nParam);

			virtual EEuresysGetFunction GetTrigDelay_us(_Out_ int* pParam);
			virtual EEuresysSetFunction SetTrigDelay_us(_In_ int nParam);

			virtual EEuresysGetFunction GetTrigEdge(_Out_ EDeviceGrablinkTrigEdge* pParam);
			virtual EEuresysSetFunction SetTrigEdge(_In_ EDeviceGrablinkTrigEdge eParam);

			virtual EEuresysGetFunction GetTrigFilter(_Out_ EDeviceGrablinkTrigFilter* pParam);
			virtual EEuresysSetFunction SetTrigFilter(_In_ EDeviceGrablinkTrigFilter eParam);

			virtual EEuresysGetFunction GetTrigLine(_Out_ EDeviceGrablinkTrigLine* pParam);
			virtual EEuresysSetFunction SetTrigLine(_In_ EDeviceGrablinkTrigLine eParam);

			virtual EEuresysGetFunction GetPageDelay_Ln(_Out_ int* pParam);
			virtual EEuresysSetFunction SetPageDelay_Ln(_In_ int nParam);

			virtual EEuresysGetFunction GetExposeTrim(_Out_ int* pParam);
			virtual EEuresysSetFunction SetExposeTrim(_In_ int nParam);

			virtual EEuresysGetFunction GetStrobeCtl(_Out_ EDeviceGrablinkStrobeCtl* pParam);
			virtual EEuresysSetFunction SetStrobeCtl(_In_ EDeviceGrablinkStrobeCtl eParam);

			virtual EEuresysGetFunction GetStrobeDur(_Out_ int* pParam);
			virtual EEuresysSetFunction SetStrobeDur(_In_ int nParam);

			virtual EEuresysGetFunction GetStrobeMode(_Out_ EDeviceGrablinkStrobeMode* pParam);
			virtual EEuresysSetFunction SetStrobeMode(_In_ EDeviceGrablinkStrobeMode eParam);

			virtual EEuresysGetFunction GetStrobePos(_Out_ int* pParam);
			virtual EEuresysSetFunction SetStrobePos(_In_ int nParam);

			virtual EEuresysGetFunction GetPreStrobe_us(_Out_ int* pParam);
			virtual EEuresysSetFunction SetPreStrobe_us(_In_ int nParam);

			virtual EEuresysGetFunction GetLineTrigCtl(_Out_ EDeviceGrablinkLineTrigCtl* pParam);
			virtual EEuresysSetFunction SetLineTrigCtl(_In_ EDeviceGrablinkLineTrigCtl eParam);

			virtual EEuresysGetFunction GetLineTrigEdge(_Out_ EDeviceGrablinkLineTrigEdge* pParam);
			virtual EEuresysSetFunction SetLineTrigEdge(_In_ EDeviceGrablinkLineTrigEdge eParam);

			virtual EEuresysGetFunction GetLineTrigFilter(_Out_ EDeviceGrablinkLineTrigFilter* pParam);
			virtual EEuresysSetFunction SetLineTrigFilter(_In_ EDeviceGrablinkLineTrigFilter eParam);

			virtual EEuresysGetFunction GetRateDivisionFactor(_Out_ int* pParam);
			virtual EEuresysSetFunction SetRateDivisionFactor(_In_ int nParam);

			virtual EEuresysGetFunction GetLineTrigLine(_Out_ EDeviceGrablinkLineTrigLine* pParam);
			virtual EEuresysSetFunction SetLineTrigLine(_In_ EDeviceGrablinkLineTrigLine eParam);

			virtual EEuresysGetFunction GetRateDividerRestart(EDeviceGrablinkRateDividerRestart* pParam);
			virtual EEuresysSetFunction SetRateDividerRestart(EDeviceGrablinkRateDividerRestart eParam);

			virtual EEuresysGetFunction GetECCO_PLLResetControl(_Out_ EDeviceGrablinkECCO_PLLResetControl* pParam);
			virtual EEuresysSetFunction SetECCO_PLLResetControl(_In_ EDeviceGrablinkECCO_PLLResetControl eParam);

			virtual EEuresysGetFunction GetECCO_SkewCompensation(_Out_ EDeviceGrablinkECCO_SkewCompensation* pParam);
			virtual EEuresysSetFunction SetECCO_SkewCompensation(_In_ EDeviceGrablinkECCO_SkewCompensation eParam);

			virtual EEuresysGetFunction GetFvalMin_Tk(_Out_ EDeviceGrablinkFvalMin_Tk* pParam);
			virtual EEuresysSetFunction SetFvalMin_Tk(_In_ EDeviceGrablinkFvalMin_Tk eParam);

			virtual EEuresysGetFunction GetLvalMin_Tk(_Out_ EDeviceGrablinkLvalMin_Tk* pParam);
			virtual EEuresysSetFunction SetLvalMin_Tk(_In_ EDeviceGrablinkLvalMin_Tk eParam);

			virtual EEuresysGetFunction GetMetadataInsertion(_Out_ EDeviceGrablinkMetadataInsertion* pParam);
			virtual EEuresysSetFunction SetMetadataInsertion(_In_ EDeviceGrablinkMetadataInsertion eParam);

			virtual EEuresysGetFunction GetGrabWindow(_Out_ EDeviceGrablinkGrabWindow* pParam);
			virtual EEuresysSetFunction SetGrabWindow(_In_ EDeviceGrablinkGrabWindow eParam);

			virtual EEuresysGetFunction GetOffsetX_Px(_Out_ int* pParam);
			virtual EEuresysSetFunction SetOffsetX_Px(_In_ int nParam);

			virtual EEuresysGetFunction GetWindowX_Px(_Out_ int* pParam);
			virtual EEuresysSetFunction SetWindowX_Px(_In_ int nParam);

			virtual EEuresysGetFunction GetColorFormat(_Out_ EDeviceGrablinkColorFormat* pParam);
			virtual EEuresysSetFunction SetColorFormat(_In_ EDeviceGrablinkColorFormat eParam);

			virtual EEuresysGetFunction GetImageFlipX(_Out_ EDeviceGrablinkImageFlipX* pParam);
			virtual EEuresysSetFunction SetImageFlipX(_In_ EDeviceGrablinkImageFlipX eParam);

			virtual EEuresysGetFunction GetImageFlipY(_Out_ EDeviceGrablinkImageFlipY* pParam);
			virtual EEuresysSetFunction SetImageFlipY(_In_ EDeviceGrablinkImageFlipY eParam);

			virtual EEuresysGetFunction GetRedBlueSwap(_Out_ EDeviceGrablinkRedBlueSwap* pParam);
			virtual EEuresysSetFunction SetRedBlueSwap(_In_ EDeviceGrablinkRedBlueSwap eParam);

			virtual EEuresysGetFunction GetMaxFillingSurfaces(_Out_ EDeviceGrablinkMaxFillingSurfaces* pParam);
			virtual EEuresysSetFunction SetMaxFillingSurfaces(_In_ EDeviceGrablinkMaxFillingSurfaces eParam);

			virtual EEuresysGetFunction GetSurfaceCount(_Out_ int* pParam);
			virtual EEuresysSetFunction SetSurfaceCount(_In_ int nParam);

			virtual EEuresysGetFunction GetAcquisitionCleanup(_Out_ EDeviceGrablinkAcquisitionCleanup* pParam);
			virtual EEuresysSetFunction SetAcquisitionCleanup(_In_ EDeviceGrablinkAcquisitionCleanup eParam);

			virtual EEuresysGetFunction GetAcqTimeout_ms(_Out_ int* pParam);
			virtual EEuresysSetFunction SetAcqTimeout_ms(_In_ int nParam);

			virtual EEuresysGetFunction GetParamsConsistencyCheck(_Out_ EDeviceGrablinkParamsConsistencyCheck* pParam);
			virtual EEuresysSetFunction SetParamsConsistencyCheck(_In_ EDeviceGrablinkParamsConsistencyCheck eParam);

			virtual EEuresysGetFunction GetTriggerSkipHold(_Out_ EDeviceGrablinkTriggerSkipHold* pParam);
			virtual EEuresysSetFunction SetTriggerSkipHold(_In_ EDeviceGrablinkTriggerSkipHold eParam);

			virtual EEuresysGetFunction GetCallbackPriority(_Out_ EDeviceGrablinkCallbackPriority* pParam);
			virtual EEuresysSetFunction SetCallbackPriority(_In_ EDeviceGrablinkCallbackPriority eParam);

			virtual EEuresysGetFunction GetInterleavedAcquisition(_Out_ EDeviceGrablinkInterleavedAcquisition* pParam);
			virtual EEuresysSetFunction SetInterleavedAcquisition(_In_ EDeviceGrablinkInterleavedAcquisition eParam);

			virtual EEuresysGetFunction GetExposureTime_P1_us(_Out_ int* pParam);
			virtual EEuresysSetFunction SetExposureTime_P1_us(_In_ int nParam);

			virtual EEuresysGetFunction GetExposureTime_P2_us(_Out_ int* pParam);
			virtual EEuresysSetFunction SetExposureTime_P2_us(_In_ int nParam);

			virtual EEuresysGetFunction GetStrobeDuration_P1_us(_Out_ int* pParam);
			virtual EEuresysSetFunction SetStrobeDuration_P1_us(_In_ int nParam);

			virtual EEuresysGetFunction GetStrobeDuration_P2_us(_Out_ int* pParam);
			virtual EEuresysSetFunction SetStrobeDuration_P2_us(_In_ int nParam);

			virtual EEuresysGetFunction GetStrobeDelay_P1_us(_Out_ int* pParam);
			virtual EEuresysSetFunction SetStrobeDelay_P1_us(_In_ int nParam);

			virtual EEuresysGetFunction GetStrobeDelay_P2_us(_Out_ int* pParam);
			virtual EEuresysSetFunction SetStrobeDelay_P2_us(_In_ int nParam);

			virtual EEuresysGetFunction GetExposureDelay_MAN_P1_us(_Out_ int* pParam);
			virtual EEuresysSetFunction SetExposureDelay_MAN_P1_us(_In_ int nParam);

			virtual EEuresysGetFunction GetExposureDelay_MAN_P2_us(_Out_ int* pParam);
			virtual EEuresysSetFunction SetExposureDelay_MAN_P2_us(_In_ int nParam);

			virtual bool OnParameterChanged(_In_ int nParam, _In_ CString strValue) override;

		protected:
			static void WINAPI MulticamCallback(PMCSIGNALINFO CbInfo);

			virtual void SetUpdateDevice();

			virtual void SetUpdateParameter();
		};
	}
}

