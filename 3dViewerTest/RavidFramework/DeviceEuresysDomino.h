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
		class AFX_EXT_CLASS CDeviceEuresysDomino : public CDeviceEuresysMulticam
		{
		public:
			CDeviceEuresysDomino();
			virtual ~CDeviceEuresysDomino();

			virtual EDeviceInitializeResult Initialize() override;
			virtual EDeviceTerminateResult Terminate() override;

			virtual bool LoadSettings() override;
			
			virtual EEuresysGetFunction GetAcquisitionMode(_Out_ EDeviceDominoAcquisitionMode* pParam);
			virtual EEuresysSetFunction SetAcquisitionMode(_In_ EDeviceDominoAcquisitionMode eParam);

			virtual EEuresysGetFunction GetTrigMode(_Out_ EDeviceDominoTrigMode* pParam);
			virtual EEuresysSetFunction SetTrigMode(_In_ EDeviceDominoTrigMode eParam);

			virtual EEuresysGetFunction GetVideoFilter(_Out_ EDeviceDominoVideoFilter* pParam);
			virtual EEuresysSetFunction SetVideoFilter(_In_ EDeviceDominoVideoFilter eParam);

			virtual EEuresysGetFunction GetCamConfig(_Out_ EDeviceDominoCamConfig* pParam);
			virtual EEuresysSetFunction SetCamConfig(_In_ EDeviceDominoCamConfig eParam);

			virtual EEuresysGetFunction GetCamera(_Out_ EDeviceDominoCamera* pParam);
			virtual EEuresysSetFunction SetCamera(_In_ EDeviceDominoCamera eParam);

			virtual EEuresysGetFunction GetDataLink(_Out_ EDeviceDominoDataLink* pParam);
			virtual EEuresysSetFunction SetDataLink(_In_ EDeviceDominoDataLink eParam);

			virtual EEuresysGetFunction GetImaging(_Out_ EDeviceDominoImaging* pParam);
			virtual EEuresysSetFunction SetImaging(_In_ EDeviceDominoImaging eParam);

			virtual EEuresysGetFunction GetSpectrum(_Out_ EDeviceDominoSpectrum* pParam);
			virtual EEuresysSetFunction SetSpectrum(_In_ EDeviceDominoSpectrum eParam);

			virtual EEuresysGetFunction GetTapStructure(_Out_ EDeviceDominoTapStructure* pParam);
			virtual EEuresysSetFunction SetTapStructure(_In_ EDeviceDominoTapStructure eParam);

			virtual EEuresysGetFunction GetScanning(_Out_ EDeviceDominoScanning* pParam);
			virtual EEuresysSetFunction SetScanning(_In_ EDeviceDominoScanning eParam);

			virtual EEuresysGetFunction GetStandard(_Out_ EDeviceDominoStandard* pParam);
			virtual EEuresysSetFunction SetStandard(_In_ EDeviceDominoStandard eParam);

			virtual EEuresysGetFunction GetAuxResetCtl(_Out_ EDeviceDominoAuxResetCtl* pParam);
			virtual EEuresysSetFunction SetAuxResetCtl(_In_ EDeviceDominoAuxResetCtl eParam);

			virtual EEuresysGetFunction GetColorMethod(_Out_ EDeviceDominoColorMethod* pParam);
			virtual EEuresysSetFunction SetColorMethod(_In_ EDeviceDominoColorMethod eParam);

			virtual EEuresysGetFunction GetExposeOverlap(_Out_ EDeviceDominoExposeOverlap* pParam);
			virtual EEuresysSetFunction SetExposeOverlap(_In_ EDeviceDominoExposeOverlap eParam);

			virtual EEuresysGetFunction GetReadout(_Out_ EDeviceDominoReadout* pParam);
			virtual EEuresysSetFunction SetReadout(_In_ EDeviceDominoReadout eParam);

			virtual EEuresysGetFunction GetResetCtl(_Out_ EDeviceDominoResetCtl* pParam);
			virtual EEuresysSetFunction SetResetCtl(_In_ EDeviceDominoResetCtl eParam);

			virtual EEuresysGetFunction GetResetEdge(_Out_ EDeviceDominoResetEdge* pParam);
			virtual EEuresysSetFunction SetResetEdge(_In_ EDeviceDominoResetEdge eParam);

			virtual EEuresysGetFunction GetACReset(_Out_ EDeviceDominoACReset* pParam);
			virtual EEuresysSetFunction SetACReset(_In_ EDeviceDominoACReset eParam);

			virtual EEuresysGetFunction GetChannelTopology(_Out_ EDeviceDominoChannelTopology* pParam);
			virtual EEuresysSetFunction SetChannelTopology(_In_ EDeviceDominoChannelTopology eParam);

			virtual EEuresysGetFunction GetHReset(_Out_ EDeviceDominoHReset* pParam);
			virtual EEuresysSetFunction SetHReset(_In_ EDeviceDominoHReset eParam);

			virtual EEuresysGetFunction GetHsyncCtl(_Out_ EDeviceDominoHsyncCtl* pParam);
			virtual EEuresysSetFunction SetHsyncCtl(_In_ EDeviceDominoHsyncCtl eParam);

			virtual EEuresysGetFunction GetHsyncEdge(_Out_ EDeviceDominoHsyncEdge* pParam);
			virtual EEuresysSetFunction SetHsyncEdge(_In_ EDeviceDominoHsyncEdge eParam);

			virtual EEuresysGetFunction GetHVdriveCtl(_Out_ EDeviceDominoHVdriveCtl* pParam);
			virtual EEuresysSetFunction SetHVdriveCtl(_In_ EDeviceDominoHVdriveCtl eParam);

			virtual EEuresysGetFunction GetHVdriveEdge(_Out_ EDeviceDominoHVdriveEdge* pParam);
			virtual EEuresysSetFunction SetHVdriveEdge(_In_ EDeviceDominoHVdriveEdge eParam);

			virtual EEuresysGetFunction GetPixelClkCtl(_Out_ EDeviceDominoPixelClkCtl* pParam);
			virtual EEuresysSetFunction SetPixelClkCtl(_In_ EDeviceDominoPixelClkCtl eParam);

			virtual EEuresysGetFunction GetVCSync(_Out_ EDeviceDominoVCSync* pParam);
			virtual EEuresysSetFunction SetVCSync(_In_ EDeviceDominoVCSync eParam);

			virtual EEuresysGetFunction GetVHSync(_Out_ EDeviceDominoVHSync* pParam);
			virtual EEuresysSetFunction SetVHSync(_In_ EDeviceDominoVHSync eParam);

			virtual EEuresysGetFunction GetVsyncCtl(_Out_ EDeviceDominoVsyncCtl* pParam);
			virtual EEuresysSetFunction SetVsyncCtl(_In_ EDeviceDominoVsyncCtl eParam);

			virtual EEuresysGetFunction GetVsyncEdge(_Out_ EDeviceDominoVsyncEdge* pParam);
			virtual EEuresysSetFunction SetVsyncEdge(_In_ EDeviceDominoVsyncEdge eParam);
			
			virtual EEuresysGetFunction GetBreakEffect(_Out_ EDeviceDominoBreakEffect* pParam);
			virtual EEuresysSetFunction SetBreakEffect(_In_ EDeviceDominoBreakEffect eParam);

			virtual EEuresysGetFunction GetEndTrigMode(_Out_ EDeviceDominoEndTrigMode* pParam);
			virtual EEuresysSetFunction SetEndTrigMode(_In_ EDeviceDominoEndTrigMode eParam);

			virtual EEuresysGetFunction GetNextTrigMode(_Out_ EDeviceDominoNextTrigMode* pParam);
			virtual EEuresysSetFunction SetNextTrigMode(_In_ EDeviceDominoNextTrigMode eParam);

			virtual EEuresysGetFunction GetAuxResetLine(_Out_ EDeviceDominoAuxResetLine* pParam);
			virtual EEuresysSetFunction SetAuxResetLine(_In_ EDeviceDominoAuxResetLine eParam);

			virtual EEuresysGetFunction GetCable(_Out_ EDeviceDominoCable* pParam);
			virtual EEuresysSetFunction SetCable(_In_ EDeviceDominoCable eParam);

			virtual EEuresysGetFunction GetCsyncLine(_Out_ EDeviceDominoCsyncLine* pParam);
			virtual EEuresysSetFunction SetCsyncLine(_In_ EDeviceDominoCsyncLine eParam);

			virtual EEuresysGetFunction GetHdriveLine(_Out_ EDeviceDominoHdriveLine* pParam);
			virtual EEuresysSetFunction SetHdriveLine(_In_ EDeviceDominoHdriveLine eParam);

			virtual EEuresysGetFunction GetHsyncLine(_Out_ EDeviceDominoHsyncLine* pParam);
			virtual EEuresysSetFunction SetHsyncLine(_In_ EDeviceDominoHsyncLine eParam);

			virtual EEuresysGetFunction GetPixelClkLine(_Out_ EDeviceDominoPixelClkLine* pParam);
			virtual EEuresysSetFunction SetPixelClkLine(_In_ EDeviceDominoPixelClkLine eParam);

			virtual EEuresysGetFunction GetVdriveLine(_Out_ EDeviceDominoVdriveLine* pParam);
			virtual EEuresysSetFunction SetVdriveLine(_In_ EDeviceDominoVdriveLine eParam);

			virtual EEuresysGetFunction GetVsyncLine(_Out_ EDeviceDominoVsyncLine* pParam);
			virtual EEuresysSetFunction SetVsyncLine(_In_ EDeviceDominoVsyncLine eParam);

			virtual EEuresysGetFunction GetResetLine(_Out_ EDeviceDominoResetLine* pParam);
			virtual EEuresysSetFunction SetResetLine(_In_ EDeviceDominoResetLine eParam);

			virtual EEuresysGetFunction GetGrabField(_Out_ EDeviceDominoGrabField* pParam);
			virtual EEuresysSetFunction SetGrabField(_In_ EDeviceDominoGrabField eParam);

			virtual EEuresysGetFunction GetNextGrabField(_Out_ EDeviceDominoNextGrabField* pParam);
			virtual EEuresysSetFunction SetNextGrabField(_In_ EDeviceDominoNextGrabField eParam);

			virtual EEuresysGetFunction GetForceTrig(_Out_ EDeviceDominoForceTrig* pParam);
			virtual EEuresysSetFunction SetForceTrig(_In_ EDeviceDominoForceTrig eParam);

			virtual EEuresysGetFunction GetStrobeCtl(_Out_ EDeviceDominoStrobeCtl* pParam);
			virtual EEuresysSetFunction SetStrobeCtl(_In_ EDeviceDominoStrobeCtl eParam);

			virtual EEuresysGetFunction GetStrobeMode(_Out_ EDeviceDominoStrobeMode* pParam);
			virtual EEuresysSetFunction SetStrobeMode(_In_ EDeviceDominoStrobeMode eParam);			

			virtual EEuresysGetFunction GetStrobeLine(_Out_ EDeviceDominoStrobeLine* pParam);
			virtual EEuresysSetFunction SetStrobeLine(_In_ EDeviceDominoStrobeLine eParam);

			virtual EEuresysGetFunction GetStrobeLevel(_Out_ EDeviceDominoStrobeLevel* pParam);
			virtual EEuresysSetFunction SetStrobeLevel(_In_ EDeviceDominoStrobeLevel eParam);

			virtual EEuresysGetFunction GetADresolution(_Out_ EDeviceDominoADresolution* pParam);
			virtual EEuresysSetFunction SetADresolution(_In_ EDeviceDominoADresolution eParam);

			virtual EEuresysGetFunction GetVideoTerminator(_Out_ EDeviceDominoVideoTerminator* pParam);
			virtual EEuresysSetFunction SetVideoTerminator(_In_ EDeviceDominoVideoTerminator eParam);

			virtual EEuresysGetFunction GetGrabWindow(_Out_ EDeviceDominoGrabWindow* pParam);
			virtual EEuresysSetFunction SetGrabWindow(_In_ EDeviceDominoGrabWindow eParam);

			virtual EEuresysGetFunction GetGainCtl(_Out_ EDeviceDominoGainCtl* pParam);
			virtual EEuresysSetFunction SetGainCtl(_In_ EDeviceDominoGainCtl eParam);

			virtual EEuresysGetFunction GetCalPat(_Out_ EDeviceDominoCalPat* pParam);
			virtual EEuresysSetFunction SetCalPat(_In_ EDeviceDominoCalPat eParam);
			
			virtual EEuresysGetFunction GetBoardIdentifier(_Out_ CString* pParam);
			virtual EEuresysSetFunction SetBoardIdentifier(_In_ CString strParam);

			virtual EEuresysGetFunction GetBoardName(_Out_ CString* pParam);
			virtual EEuresysSetFunction SetBoardName(_In_ CString strParam);

			virtual EEuresysGetFunction GetColorFormat(_Out_ EDeviceDominoColorFormat* pParam);
			virtual EEuresysSetFunction SetColorFormat(_In_ EDeviceDominoColorFormat eParam);

			virtual EEuresysGetFunction GetImageFlipX(_Out_ EDeviceDominoImageFlipX* pParam);
			virtual EEuresysSetFunction SetImageFlipX(_In_ EDeviceDominoImageFlipX eParam);

			virtual EEuresysGetFunction GetImageFlipY(_Out_ EDeviceDominoImageFlipY* pParam);
			virtual EEuresysSetFunction SetImageFlipY(_In_ EDeviceDominoImageFlipY eParam);
			
			virtual EEuresysGetFunction GetSurfaceAllocation(_Out_ EDeviceDominoSurfaceAllocation* pParam);
			virtual EEuresysSetFunction SetSurfaceAllocation(_In_ EDeviceDominoSurfaceAllocation eParam);

			virtual EEuresysGetFunction GetChannelState(_Out_ EDeviceDominoChannelState* pParam);
			virtual EEuresysSetFunction SetChannelState(_In_ EDeviceDominoChannelState eParam);

			virtual EEuresysGetFunction GetAcquisitionCleanup(_Out_ EDeviceDominoAcquisitionCleanup* pParam);
			virtual EEuresysSetFunction SetAcquisitionCleanup(_In_ EDeviceDominoAcquisitionCleanup eParam);

			virtual EEuresysGetFunction GetParamsConsistencyCheck(_Out_ EDeviceDominoParamsConsistencyCheck* pParam);
			virtual EEuresysSetFunction SetParamsConsistencyCheck(_In_ EDeviceDominoParamsConsistencyCheck eParam);

			virtual EEuresysGetFunction GetExpose(_Out_ EDeviceDominoExpose* pParam);
			virtual EEuresysSetFunction SetExpose(_In_ EDeviceDominoExpose eParam);

			virtual EEuresysGetFunction GetCsyncCtl(_Out_ EDeviceDominoCsyncCtl* pParam);
			virtual EEuresysSetFunction SetCsyncCtl(_In_ EDeviceDominoCsyncCtl eParam);

			virtual EEuresysGetFunction GetCsyncEdge(_Out_ EDeviceDominoCsyncEdge* pParam);
			virtual EEuresysSetFunction SetCsyncEdge(_In_ EDeviceDominoCsyncEdge eParam);

			virtual EEuresysGetFunction GetCameraControlSignalSet(_Out_ EDeviceDominoCameraControlSignalSet* pParam);
			virtual EEuresysSetFunction SetCameraControlSignalSet(_In_ EDeviceDominoCameraControlSignalSet eParam);

			virtual EEuresysGetFunction GetExpose_us(_Out_ int* pParam);
			virtual EEuresysSetFunction SetExpose_us(_In_ int nParam);

			virtual EEuresysGetFunction GetSeqLength_Fr(_Out_ int* pParam);
			virtual EEuresysSetFunction SetSeqLength_Fr(_In_ int nParam);

			virtual EEuresysGetFunction GetPhaseLength_Fr(_Out_ int* pParam);
			virtual EEuresysSetFunction SetPhaseLength_Fr(_In_ int nParam);

			virtual EEuresysGetFunction GetGain(_Out_ int* pParam);
			virtual EEuresysSetFunction SetGain(_In_ int nParam);

			virtual EEuresysGetFunction GetOffset(_Out_ int* pParam);
			virtual EEuresysSetFunction SetOffset(_In_ int nParam);

			virtual EEuresysGetFunction GetVactive_Ln(_Out_ int* pParam);
			virtual EEuresysSetFunction SetVactive_Ln(_In_ int nParam);

			virtual EEuresysGetFunction GetVsyncAft_Ln(_Out_ int* pParam);
			virtual EEuresysSetFunction SetVsyncAft_Ln(_In_ int nParam);

			virtual EEuresysGetFunction GetLineRate_Hz(_Out_ int* pParam);
			virtual EEuresysSetFunction SetLineRate_Hz(_In_ int nParam);

			virtual EEuresysGetFunction GetACResetPostDelay_us(_Out_ int* pParam);
			virtual EEuresysSetFunction SetACResetPostDelay_us(_In_ int nParam);

			virtual EEuresysGetFunction GetACResetWidth_us(_Out_ int* pParam);
			virtual EEuresysSetFunction SetACResetWidth_us(_In_ int nParam);

			virtual EEuresysGetFunction GetHCSyncAft_ns(_Out_ int* pParam);
			virtual EEuresysSetFunction SetHCSyncAft_ns(_In_ int nParam);

			virtual EEuresysGetFunction GetHCsyncBfr_ns(_Out_ int* pParam);
			virtual EEuresysSetFunction SetHCsyncBfr_ns(_In_ int nParam);

			virtual EEuresysGetFunction GetHCsyncDur_ns(_Out_ int* pParam);
			virtual EEuresysSetFunction SetHCsyncDur_ns(_In_ int nParam);

			virtual EEuresysGetFunction GetHsyncDly_ns(_Out_ int* pParam);
			virtual EEuresysSetFunction SetHsyncDly_ns(_In_ int nParam);

			virtual EEuresysGetFunction GetHdriveDur_ns(_Out_ int* pParam);
			virtual EEuresysSetFunction SetHdriveDur_ns(_In_ int nParam);

			virtual EEuresysGetFunction GetHdriveDly_ns(_Out_ int* pParam);
			virtual EEuresysSetFunction SetHdriveDly_ns(_In_ int nParam);

			virtual EEuresysGetFunction GetHactive_ns(_Out_ int* pParam);
			virtual EEuresysSetFunction SetHactive_ns(_In_ int nParam);

			virtual EEuresysGetFunction GetPixelClk_Hz(_Out_ int* pParam);
			virtual EEuresysSetFunction SetPixelClk_Hz(_In_ int nParam);

			virtual EEuresysGetFunction GetVCsyncAft_Ln(_Out_ int* pParam);
			virtual EEuresysSetFunction SetVCsyncAft_Ln(_In_ int nParam);

			virtual EEuresysGetFunction GetVCgatePos_Ln(_Out_ int* pParam);
			virtual EEuresysSetFunction SetVCgatePos_Ln(_In_ int nParam);

			virtual EEuresysGetFunction GetVtotal_Ln(_Out_ int* pParam);
			virtual EEuresysSetFunction SetVtotal_Ln(_In_ int nParam);

			virtual EEuresysGetFunction GetVsyncBfrEndExp_us(_Out_ int* pParam);
			virtual EEuresysSetFunction SetVsyncBfrEndExp_us(_In_ int nParam);

			virtual EEuresysGetFunction GetVdriveDur_Ln(_Out_ int* pParam);
			virtual EEuresysSetFunction SetVdriveDur_Ln(_In_ int nParam);

			virtual EEuresysGetFunction GetVdriveDly_Ln(_Out_ int* pParam);
			virtual EEuresysSetFunction SetVdriveDly_Ln(_In_ int nParam);
						
			virtual EEuresysGetFunction GetExposeMin_us(_Out_ int* pParam);
			virtual EEuresysSetFunction SetExposeMin_us(_In_ int nParam);

			virtual EEuresysGetFunction GetExposeMax_us(_Out_ int* pParam);
			virtual EEuresysSetFunction SetExposeMax_us(_In_ int nParam);

			virtual EEuresysGetFunction GetCameraSignalSet(_Out_ int* pParam);
			virtual EEuresysSetFunction SetCameraSignalSet(_In_ int nParam);

			virtual EEuresysGetFunction GetResetDur(_Out_ int* pParam);
			virtual EEuresysSetFunction SetResetDur(_In_ int nParam);

			virtual EEuresysGetFunction GetResetPos(_Out_ int* pParam);
			virtual EEuresysSetFunction SetResetPos(_In_ int nParam);

			virtual EEuresysGetFunction GetCableTopology(_Out_ int* pParam);
			virtual EEuresysSetFunction SetCableTopology(_In_ int nParam);

			virtual EEuresysGetFunction GetActivityLength(_Out_ int* pParam);
			virtual EEuresysSetFunction SetActivityLength(_In_ int nParam);

			virtual EEuresysGetFunction GetTrigDelay_us(_Out_ int* pParam);
			virtual EEuresysSetFunction SetTrigDelay_us(_In_ int nParam);

			virtual EEuresysGetFunction GetTrueExp_us(_Out_ int* pParam);
			virtual EEuresysSetFunction SetTrueExp_us(_In_ int nParam);

			virtual EEuresysGetFunction GetExposeTrim(_Out_ int* pParam);
			virtual EEuresysSetFunction SetExposeTrim(_In_ int nParam);

			virtual EEuresysGetFunction GetStrobeDur(_Out_ int* pParam);
			virtual EEuresysSetFunction SetStrobeDur(_In_ int nParam);

			virtual EEuresysGetFunction GetStrobePos(_Out_ int* pParam);
			virtual EEuresysSetFunction SetStrobePos(_In_ int nParam);

			virtual EEuresysGetFunction GetPreStrobe_us(_Out_ int* pParam);
			virtual EEuresysSetFunction SetPreStrobe_us(_In_ int nParam);

			virtual EEuresysGetFunction GetOffsetX_Px(_Out_ int* pParam);
			virtual EEuresysSetFunction SetOffsetX_Px(_In_ int nParam);

			virtual EEuresysGetFunction GetOffsetY_Ln(_Out_ int* pParam);
			virtual EEuresysSetFunction SetOffsetY_Ln(_In_ int nParam);

			virtual EEuresysGetFunction GetSampleTrim(_Out_ int* pParam);
			virtual EEuresysSetFunction SetSampleTrim(_In_ int nParam);

			virtual EEuresysGetFunction GetSampleClk_Hz(_Out_ int* pParam);
			virtual EEuresysSetFunction SetSampleClk_Hz(_In_ int nParam);

			virtual EEuresysGetFunction GetInputLut(_Out_ int* pParam);
			virtual EEuresysSetFunction SetInputLut(_In_ int nParam);

			virtual EEuresysGetFunction GetLutIndex(_Out_ int* pParam);
			virtual EEuresysSetFunction SetLutIndex(_In_ int nParam);

			virtual EEuresysGetFunction GetDriverIndex(_Out_ int* pParam);
			virtual EEuresysSetFunction SetDriverIndex(_In_ int nParam);

			virtual EEuresysGetFunction GetPciPosition(_Out_ int* pParam);
			virtual EEuresysSetFunction SetPciPosition(_In_ int nParam);

			virtual EEuresysGetFunction GetImageSizeX(_Out_ int* pParam);
			virtual EEuresysSetFunction SetImageSizeX(_In_ int nParam);

			virtual EEuresysGetFunction GetAcqTimeout_ms(_Out_ int* pParam);
			virtual EEuresysSetFunction SetAcqTimeout_ms(_In_ int nParam);

			virtual EEuresysGetFunction GetSurfaceCount(_Out_ int* pParam);
			virtual EEuresysSetFunction SetSurfaceCount(_In_ int nParam);

			virtual bool OnParameterChanged(_In_ int nParam, _In_ CString strValue) override;

		protected:
			static void WINAPI MulticamCallback(PMCSIGNALINFO CbInfo);

			virtual void SetUpdateDevice();

			virtual void SetUpdateParameter();
		};
	}
}

