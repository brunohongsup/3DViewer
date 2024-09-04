#pragma once

#include "DeviceFrameGrabber.h"

typedef                long         MIL_INT32;
typedef const          long         MIL_CINT32;
typedef       unsigned long         MIL_UINT32;
typedef const unsigned long         MIL_CUINT32;

#if defined(_MSC_VER) && (_MSC_VER < 1310)
	typedef                __int64      MIL_INT64;
	typedef const          __int64      MIL_CINT64;
	typedef       unsigned __int64      MIL_UINT64;
	typedef const unsigned __int64      MIL_CUINT64;
#else
	typedef                long long    MIL_INT64;
	typedef const          long long    MIL_CINT64;
	typedef       unsigned long long    MIL_UINT64;
	typedef const unsigned long long    MIL_CUINT64;
#endif

#ifdef _WIN64
	typedef MIL_INT64 MIL_INT;
	typedef MIL_INT64 MIL_ID;
#else
	typedef MIL_INT32 MIL_INT;
	typedef MIL_INT32 MIL_ID;
#endif

#define MFTYPE        _stdcall
#define MPTYPE                         // Pointer and Reference type

#if M_MIL_USE_LINUX && M_MIL_USE_64BIT
#define M_NULL                                           0L
#else
#define M_NULL                                           0
#endif


namespace Ravid
{
	namespace Algorithms
	{
		class CRavidImage;
	}

	namespace Device
	{
		class AFX_EXT_CLASS CDeviceMatroxIntellicam : public CDeviceFrameGrabber
		{
		public:
			RavidUseDynamicCreation();

			DECLARE_DYNAMIC(CDeviceMatroxIntellicam)

		public:
			CDeviceMatroxIntellicam();
			virtual ~CDeviceMatroxIntellicam();

			virtual EDeviceInitializeResult Initialize() override;
			virtual EDeviceTerminateResult Terminate() override;

			virtual bool LoadSettings() override;

			virtual EDeviceGrabResult Grab() override;
			virtual EDeviceLiveResult Live() override;
			virtual EDeviceStopResult Stop() override;

			virtual EDeviceTriggerResult Trigger();

			virtual EMatroxGetFunction GetCameraName(_Out_ CString* pParam);
			virtual EMatroxSetFunction SetCameraName(_In_ CString strParam);

			virtual EMatroxGetFunction GetInitilizeType(_Out_ EDeviceMatrox_InitilizeType* pParam);	
			virtual EMatroxSetFunction SetInitilizeType(_In_ EDeviceMatrox_InitilizeType eParam);

			virtual EMatroxGetFunction GetCamfilePath(_Out_ CString* pParam);
			virtual EMatroxSetFunction SetCamfilePath(_In_ CString strParam);

			virtual EMatroxGetFunction GetParameterMatch(_Out_ bool* pParam);
			virtual EMatroxSetFunction SetParameterMatch(_In_ bool bParam);

			virtual EMatroxGetFunction GetGrabCount(_Out_ int* pParam);
			virtual EMatroxSetFunction SetGrabCount(_In_ int nParam);

			virtual EMatroxGetFunction GetMILImageCount(_Out_ int* pParam);
			virtual EMatroxSetFunction SetMILImageCount(_In_ int nParam);

			virtual EMatroxGetFunction GetMILBufferCount(_Out_ int* pParam);
			virtual EMatroxSetFunction SetMILBufferCount(_In_ int nParam);

			virtual EMatroxGetFunction GetGrabDirectionX(_Out_ EDeviceMatrox_DirectionSwitch* pParam);
			virtual EMatroxSetFunction SetGrabDirectionX(_In_ EDeviceMatrox_DirectionSwitch eParam);

			virtual EMatroxGetFunction GetGrabDirectionY(_Out_ EDeviceMatrox_DirectionSwitch* pParam);
			virtual EMatroxSetFunction SetGrabDirectionY(_In_ EDeviceMatrox_DirectionSwitch eParam);

			virtual EMatroxGetFunction GetGrabScale(_Out_ double* pParam);
			virtual EMatroxSetFunction SetGrabScale(_In_ double dblParam);

			virtual EMatroxGetFunction GetGrabScaleX(_Out_ double* pParam);
			virtual EMatroxSetFunction SetGrabScaleX(_In_ double dblParam);

			virtual EMatroxGetFunction GetGrabScaleY(_Out_ double* pParam);
			virtual EMatroxSetFunction SetGrabScaleY(_In_ double dblParam);

			virtual EMatroxGetFunction GetSourceOffsetX(_Out_ int* pParam);
			virtual EMatroxSetFunction SetSourceOffsetX(_In_ int nParam);

			virtual EMatroxGetFunction GetSourceOffsetY(_Out_ int* pParam);
			virtual EMatroxSetFunction SetSourceOffsetY(_In_ int nParam);

			virtual EMatroxGetFunction GetSourceSizeX(_Out_ int* pParam);
			virtual EMatroxSetFunction SetSourceSizeX(_In_ int nParam);

			virtual EMatroxGetFunction GetSourceSizeY(_Out_ int* pParam);
			virtual EMatroxSetFunction SetSourceSizeY(_In_ int nParam);

			virtual EMatroxGetFunction GetbTriggerContinuouesEndTrigger(_Out_ EDeviceMatrox_ActiveSwitch* pParam);
			virtual EMatroxSetFunction SetbTriggerContinuouesEndTrigger(_In_ EDeviceMatrox_ActiveSwitch eParam);

			virtual EMatroxGetFunction GetGrabTriggerActivation(_Out_ EDeviceMatrox_ActivationSelect* pParam);
			virtual EMatroxSetFunction SetGrabTriggerActivation(_In_ EDeviceMatrox_ActivationSelect eParam);

			virtual EMatroxGetFunction GetGrabTriggerSoursce(_Out_ EDeviceMatrox_TriggerSourceSelect* pParam);
			virtual EMatroxSetFunction SetGrabTriggerSoursce(_In_ EDeviceMatrox_TriggerSourceSelect eParam);

			virtual EMatroxGetFunction GetGrabTriggerState(_Out_ EDeviceMatrox_ActiveSwitch* pParam);
			virtual EMatroxSetFunction SetGrabTriggerState(_In_ EDeviceMatrox_ActiveSwitch eParam);
// 
// 			virtual EMatroxGetFunction GetTimerSelector(_Out_ EDeviceMatrox_TimerSelect* pParam);
// 			virtual EMatroxSetFunction SetTimerSelector(_In_ EDeviceMatrox_TimerSelect eParam);

			//////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////

			virtual EMatroxGetFunction GetCameraType(_Out_ EDeviceMatrox_CameraType* pParam);
			virtual EMatroxSetFunction SetCameraType(_In_ EDeviceMatrox_CameraType eParam);

			virtual EMatroxGetFunction GetCameraNumberOfTaps(_Out_ EDeviceMatrox_NumberOfTaps* pParam);
			virtual EMatroxSetFunction SetCameraNumberOfTaps(_In_ EDeviceMatrox_NumberOfTaps eParam);

			virtual EMatroxGetFunction GetCameraBayerMode(_Out_ EDeviceMatrox_BayerMode* pParam);
			virtual EMatroxSetFunction SetCameraBayerMode(_In_ EDeviceMatrox_BayerMode eParam);

			virtual EMatroxGetFunction GetCameraLinkConfiguration(_Out_ EDeviceMatrox_ConfigurationType*pParam);
			virtual EMatroxSetFunction SetCameraLinkConfiguration(_In_ EDeviceMatrox_ConfigurationType eParam);

			virtual EMatroxGetFunction GetCameraMode(_Out_ EDeviceMatrox_CameraTapsMode* pParam);
			virtual EMatroxSetFunction SetCameraMode(_In_ EDeviceMatrox_CameraTapsMode eParam);

			virtual EMatroxGetFunction GetRegionsX(_Out_ EDeviceMatrox_TapsSelector*pParam);
			virtual EMatroxSetFunction SetRegionsX(_In_ EDeviceMatrox_TapsSelector eParam);

			virtual EMatroxGetFunction GetRegionsY(_Out_ EDeviceMatrox_TapsSelector* pParam);
			virtual EMatroxSetFunction SetRegionsY(_In_ EDeviceMatrox_TapsSelector eParam);

			virtual EMatroxGetFunction GetTaps1Direction(_Out_ EDeviceMatrox_TapsDirection* pParam);
			virtual EMatroxSetFunction SetTaps1Direction(_In_ EDeviceMatrox_TapsDirection eParam);

			virtual EMatroxGetFunction GetTaps2Direction(_Out_ EDeviceMatrox_TapsDirection* pParam);
			virtual EMatroxSetFunction SetTaps2Direction(_In_ EDeviceMatrox_TapsDirection eParam);

			virtual EMatroxGetFunction GetTaps3Direction(_Out_ EDeviceMatrox_TapsDirection* pParam);
			virtual EMatroxSetFunction SetTaps3Direction(_In_ EDeviceMatrox_TapsDirection eParam);

			virtual EMatroxGetFunction GetTaps4Direction(_Out_ EDeviceMatrox_TapsDirection* pParam);
			virtual EMatroxSetFunction SetTaps4Direction(_In_ EDeviceMatrox_TapsDirection eParam);

			virtual EMatroxGetFunction GetTaps5Direction(_Out_ EDeviceMatrox_TapsDirection* pParam);
			virtual EMatroxSetFunction SetTaps5Direction(_In_ EDeviceMatrox_TapsDirection eParam);

			virtual EMatroxGetFunction GetTaps6Direction(_Out_ EDeviceMatrox_TapsDirection* pParam);
			virtual EMatroxSetFunction SetTaps6Direction(_In_ EDeviceMatrox_TapsDirection eParam);

			virtual EMatroxGetFunction GetTaps7Direction(_Out_ EDeviceMatrox_TapsDirection* pParam);
			virtual EMatroxSetFunction SetTaps7Direction(_In_ EDeviceMatrox_TapsDirection eParam);

			virtual EMatroxGetFunction GetTaps8Direction(_Out_ EDeviceMatrox_TapsDirection* pParam);
			virtual EMatroxSetFunction SetTaps8Direction(_In_ EDeviceMatrox_TapsDirection eParam);
			
			virtual EMatroxGetFunction GetAdjacentPixelsX(_Out_ EDeviceMatrox_TapsSelector* pParam);
			virtual EMatroxSetFunction SetAdjacentPixelsX(_In_ EDeviceMatrox_TapsSelector eParam);

			virtual EMatroxGetFunction GetAdjacentPixelsY(_Out_ EDeviceMatrox_TapsSelector* pParam);
			virtual EMatroxSetFunction SetAdjacentPixelsY(_In_ EDeviceMatrox_TapsSelector eParam);

			virtual EMatroxGetFunction GetVideoSignalType(_Out_ EDeviceMatrox_VideoSignalType* pParam);
			virtual EMatroxSetFunction SetVideoSignalType(_In_ EDeviceMatrox_VideoSignalType eParam);

			virtual EMatroxGetFunction GetDataBusWidth(_Out_ EDeviceMatrox_DataBusWidth* pParam);
			virtual EMatroxSetFunction SetDataBusWidth(_In_ EDeviceMatrox_DataBusWidth eParam);

			virtual EMatroxGetFunction GetVideoSignalStandard(_Out_ EDeviceMatrox_Standard* pParam);
			virtual EMatroxSetFunction SetVideoSignalStandard(_In_ EDeviceMatrox_Standard eParam);

			virtual EMatroxGetFunction GetMILChannel(_Out_ EDeviceMatrox_MILChannel* pParam);
			virtual EMatroxSetFunction SetMILChannel(_In_ EDeviceMatrox_MILChannel eParam);

			virtual EMatroxGetFunction GetVideoSignalFormat(_Out_ EDeviceMatrox_InoutFormat* pParam);
			virtual EMatroxSetFunction SetVideoSignalFormat(_In_ EDeviceMatrox_InoutFormat eParam);

			virtual EMatroxGetFunction GetVideoSignalSource(_Out_ EDeviceMatrox_ValidSignalSource* pParam);
			virtual EMatroxSetFunction SetVideoSignalSource(_In_ EDeviceMatrox_ValidSignalSource eParam);

			virtual EMatroxGetFunction GetTimingsVertical(_Out_ EDeviceMatrox_InterlacedSelector* pParam);
			virtual EMatroxSetFunction SetTimingsVertical(_In_ EDeviceMatrox_InterlacedSelector eParam);

			virtual EMatroxGetFunction GetVideoTimingAutoAdjust(_Out_ bool* pParam);
			virtual EMatroxSetFunction SetVideoTimingAutoAdjust(_In_ bool bParam);

			virtual EMatroxGetFunction GetHorizontalSyncClk(_Out_ int* pParam);
			virtual EMatroxSetFunction SetHorizontalSyncClk(_In_ int nParam);

			virtual EMatroxGetFunction GetHorizontalBPorchClk(_Out_ int* pParam);
			virtual EMatroxSetFunction SetHorizontalBPorchClk(_In_ int nParam);

			virtual EMatroxGetFunction GetHorizontalActiveClk(_Out_ int* pParam);
			virtual EMatroxSetFunction SetHorizontalActiveClk(_In_ int nParam);

			virtual EMatroxGetFunction GetHorizontalFPorchClk(_Out_ int* pParam);
			virtual EMatroxSetFunction SetHorizontalFPorchClk(_In_ int nParam);

			virtual EMatroxGetFunction GetHorizontalFrequencyHz(_Out_ double* pParam);
			virtual EMatroxSetFunction SetHorizontalFrequencyHz(_In_ double dblParam);

			virtual EMatroxGetFunction GetHorizontalLockActiveAndTotal(_Out_ bool* pParam);
			virtual EMatroxSetFunction SetHorizontalLockActiveAndTotal(_In_ bool bParam);

			virtual EMatroxGetFunction GetVerticalSyncLines(_Out_ int* pParam);
			virtual EMatroxSetFunction SetVerticalSyncLines(_In_ int nParam);

			virtual EMatroxGetFunction GetVerticalBPorchLines(_Out_ int* pParam);
			virtual EMatroxSetFunction SetVerticalBPorchLines(_In_ int nParam);

			virtual EMatroxGetFunction GetVerticalActiveLines(_Out_ int* pParam);
			virtual EMatroxSetFunction SetVerticalActiveLines(_In_ int nParam);

			virtual EMatroxGetFunction GetVerticalFPorchLines(_Out_ int* pParam);
			virtual EMatroxSetFunction SetVerticalFPorchLines(_In_ int nParam);

			virtual EMatroxGetFunction GetVerticalFrequencyHz(_Out_ double* pParam);
			virtual EMatroxSetFunction SetVerticalFrequencyHz(_In_ double dblParam);

			virtual EMatroxGetFunction GetVerticalLockActiveAndTotal(_Out_ bool* pParam);
			virtual EMatroxSetFunction SetVerticalLockActiveAndTotal(_In_ bool bParam);

			virtual EMatroxGetFunction GetVideoTimingImagesizeX(_Out_ int* pParam);
			virtual EMatroxSetFunction SetVideoTimingImagesizeX(_In_ int nParam);

			virtual EMatroxGetFunction GetVideoTimingImagesizeY(_Out_ int* pParam);
			virtual EMatroxSetFunction SetVideoTimingImagesizeY(_In_ int nParam);

			virtual EMatroxGetFunction GetVideoTimingDelayX(_Out_ int* pParam);
			virtual EMatroxSetFunction SetVideoTimingDelayX(_In_ int nParam);

			virtual EMatroxGetFunction GetVideoTimingDelayY(_Out_ int* pParam);
			virtual EMatroxSetFunction SetVideoTimingDelayY(_In_ int nParam);

			virtual EMatroxGetFunction GetPixelClockFrequency(_Out_ double* pParam);
			virtual EMatroxSetFunction SetPixelClockFrequency(_In_ double dblParam);

			virtual EMatroxGetFunction GetAutoAdjustInTheVideoTimingsPage(_Out_ EDeviceMatrox_AdjustInTheVideoTimingsPage* pParam);
			virtual EMatroxSetFunction SetAutoAdjustInTheVideoTimingsPage(_In_ EDeviceMatrox_AdjustInTheVideoTimingsPage eParam);

			virtual EMatroxGetFunction GetSendToExternalCircuitOtherThanCamera(_Out_ bool* pParam);
			virtual EMatroxSetFunction SetSendToExternalCircuitOtherThanCamera(_In_ bool bParam);

			virtual EMatroxGetFunction GetExternalClockSignal(_Out_ EDeviceMatrox_ExternalClockSignal* pParam);
			virtual EMatroxSetFunction SetExternalClockSignal(_In_ EDeviceMatrox_ExternalClockSignal eParam);

			virtual EMatroxGetFunction GetPixelClockInputFrequency(_Out_ EDeviceMatrox_PixelClockFrequency* pParam);
			virtual EMatroxSetFunction SetPixelClockInputFrequency(_In_ EDeviceMatrox_PixelClockFrequency eParam);

			virtual EMatroxGetFunction GetPixelClockInputFormat(_Out_ EDeviceMatrox_InoutFormat* pParam);
			virtual EMatroxSetFunction SetPixelClockInputFormat(_In_ EDeviceMatrox_InoutFormat eParam);

			virtual EMatroxGetFunction GetPixelClockInputPolarity(_Out_ EDeviceMatrox_InoutPolarity* pParam);
			virtual EMatroxSetFunction SetPixelClockInputPolarity(_In_ EDeviceMatrox_InoutPolarity eParam);

			virtual EMatroxGetFunction GetPixelClockOutputFrequency(_Out_ EDeviceMatrox_PixelClockFrequency* pParam);
			virtual EMatroxSetFunction SetPixelClockOutputFrequency(_In_ EDeviceMatrox_PixelClockFrequency eParam);

			virtual EMatroxGetFunction GetPixelClockOutputFormat(_Out_ EDeviceMatrox_InoutFormat* pParam);
			virtual EMatroxSetFunction SetPixelClockOutputFormat(_In_ EDeviceMatrox_InoutFormat eParam);

			virtual EMatroxGetFunction GetPixelClockOutputPolarity(_Out_ EDeviceMatrox_InoutPolarity* pParam);
			virtual EMatroxSetFunction SetPixelClockOutputPolarity(_In_ EDeviceMatrox_InoutPolarity eParam);

			virtual EMatroxGetFunction GetPixelClockDelay(_Out_ int* pParam);
			virtual EMatroxSetFunction SetPixelClockDelay(_In_ int nParam);

			virtual EMatroxGetFunction GetTimer1Mode(_Out_ EDeviceMatrox_GenerationMode* pParam);
			virtual EMatroxSetFunction SetTimer1Mode(_In_ EDeviceMatrox_GenerationMode eParam);

			virtual EMatroxGetFunction GetTimer1Format(_Out_ EDeviceMatrox_InoutFormatDefault* pParam);
			virtual EMatroxSetFunction SetTimer1Format(_In_ EDeviceMatrox_InoutFormatDefault eParam);

			virtual EMatroxGetFunction GetTimer1Polarity(_Out_ EDeviceMatrox_InoutPolarity* pParam);
			virtual EMatroxSetFunction SetTimer1Polarity(_In_ EDeviceMatrox_InoutPolarity eParam);

			virtual EMatroxGetFunction GetTimer1TrgFormat(_Out_ EDeviceMatrox_InoutFormatDefault* pParam);
			virtual EMatroxSetFunction SetTimer1TrgFormat(_In_ EDeviceMatrox_InoutFormatDefault eParam);

			virtual EMatroxGetFunction GetTimer1TrgSignal(_Out_ EDeviceMatrox_ExposureTriggerSignal* pParam);
			virtual EMatroxSetFunction SetTimer1TrgSignal(_In_ EDeviceMatrox_ExposureTriggerSignal eParam);

			virtual EMatroxGetFunction GetTimer1TrgPolarity(_Out_ EDeviceMatrox_InoutPolarity* pParam);
			virtual EMatroxSetFunction SetTimer1TrgPolarity(_In_ EDeviceMatrox_InoutPolarity eParam);

			virtual EMatroxGetFunction GetTimer1Pulse1Clk(_Out_ int* pParam);
			virtual EMatroxSetFunction SetTimer1Pulse1Clk(_In_ int nParam);

			virtual EMatroxGetFunction GetTimer1Delay1Clk(_Out_ int* pParam);
			virtual EMatroxSetFunction SetTimer1Delay1Clk(_In_ int nParam);

			virtual EMatroxGetFunction GetTimer1Pulse2Clk(_Out_ int* pParam);
			virtual EMatroxSetFunction SetTimer1Pulse2Clk(_In_ int nParam);

			virtual EMatroxGetFunction GetTimer1Delay2Clk(_Out_ int* pParam);
			virtual EMatroxSetFunction SetTimer1Delay2Clk(_In_ int nParam);

			virtual EMatroxGetFunction GetTimer1ExposureClockType(_Out_ EDeviceMatrox_ExposureClockType* pParam);
			virtual EMatroxSetFunction SetTimer1ExposureClockType(_In_ EDeviceMatrox_ExposureClockType eParam);

			virtual EMatroxGetFunction GetTimer1ExposureClockDivisionFactor(_Out_ EDeviceMatrox_DivisionFactor* pParam);
			virtual EMatroxSetFunction SetTimer1ExposureClockDivisionFactor(_In_ EDeviceMatrox_DivisionFactor eParam);

			virtual EMatroxGetFunction GetTimer1ExposureClockBaseClockFrequency(_Out_ double* pParam);
			virtual EMatroxSetFunction SetTimer1ExposureClockBaseClockFrequency(_In_ double dblParam);
			
			virtual EMatroxGetFunction GetTimer2Mode(_Out_ EDeviceMatrox_GenerationMode* pParam);
			virtual EMatroxSetFunction SetTimer2Mode(_In_ EDeviceMatrox_GenerationMode eParam);

			virtual EMatroxGetFunction GetTimer2Format(_Out_ EDeviceMatrox_InoutFormatDefault* pParam);
			virtual EMatroxSetFunction SetTimer2Format(_In_ EDeviceMatrox_InoutFormatDefault eParam);

			virtual EMatroxGetFunction GetTimer2Polarity(_Out_ EDeviceMatrox_InoutPolarity* pParam);
			virtual EMatroxSetFunction SetTimer2Polarity(_In_ EDeviceMatrox_InoutPolarity eParam);

			virtual EMatroxGetFunction GetTimer2TrgFormat(_Out_ EDeviceMatrox_InoutFormatDefault* pParam);
			virtual EMatroxSetFunction SetTimer2TrgFormat(_In_ EDeviceMatrox_InoutFormatDefault eParam);

			virtual EMatroxGetFunction GetTimer2TrgSignal(_Out_ EDeviceMatrox_ExposureTriggerSignal* pParam);
			virtual EMatroxSetFunction SetTimer2TrgSignal(_In_ EDeviceMatrox_ExposureTriggerSignal eParam);

			virtual EMatroxGetFunction GetTimer2TrgPolarity(_Out_ EDeviceMatrox_InoutPolarity* pParam);
			virtual EMatroxSetFunction SetTimer2TrgPolarity(_In_ EDeviceMatrox_InoutPolarity eParam);

			virtual EMatroxGetFunction GetTimer2Pulse1Clk(_Out_ int* pParam);
			virtual EMatroxSetFunction SetTimer2Pulse1Clk(_In_ int nParam);

			virtual EMatroxGetFunction GetTimer2Delay1Clk(_Out_ int* pParam);
			virtual EMatroxSetFunction SetTimer2Delay1Clk(_In_ int nParam);

			virtual EMatroxGetFunction GetTimer2Pulse2Clk(_Out_ int* pParam);
			virtual EMatroxSetFunction SetTimer2Pulse2Clk(_In_ int nParam);

			virtual EMatroxGetFunction GetTimer2Delay2Clk(_Out_ int* pParam);
			virtual EMatroxSetFunction SetTimer2Delay2Clk(_In_ int nParam);

			virtual EMatroxGetFunction GetTimer2ExposureClockType(_Out_ EDeviceMatrox_ExposureClockType* pParam);
			virtual EMatroxSetFunction SetTimer2ExposureClockType(_In_ EDeviceMatrox_ExposureClockType eParam);

			virtual EMatroxGetFunction GetTimer2ExposureClockDivisionFactor(_Out_ EDeviceMatrox_DivisionFactor* pParam);
			virtual EMatroxSetFunction SetTimer2ExposureClockDivisionFactor(_In_ EDeviceMatrox_DivisionFactor eParam);

			virtual EMatroxGetFunction GetTimer2ExposureClockBaseClockFrequency(_Out_ double* pParam);
			virtual EMatroxSetFunction SetTimer2ExposureClockBaseClockFrequency(_In_ double dblParam);

			virtual EMatroxGetFunction GetTimer1AdvancedTimerOutputSignal(_Out_ EDeviceMatrox_TimerOutputSignal* pParam);
			virtual EMatroxSetFunction SetTimer1AdvancedTimerOutputSignal(_In_ EDeviceMatrox_TimerOutputSignal eParam);

			virtual EMatroxGetFunction GetTimer1AdvancedCaptureTrigger(_Out_ EDeviceMatrox_CaptureATriggerEveryTrigger* pParam);
			virtual EMatroxSetFunction SetTimer1AdvancedCaptureTrigger(_In_ EDeviceMatrox_CaptureATriggerEveryTrigger eParam);

			virtual EMatroxGetFunction GetTimer1AdvancedTriggerArmMode(_Out_ EDeviceMatrox_TriggerArmMode* pParam);
			virtual EMatroxSetFunction SetTimer1AdvancedTriggerArmMode(_In_ EDeviceMatrox_TriggerArmMode eParam);

			virtual EMatroxGetFunction GetTimer1AdvancedTriggerArmFormat(_Out_ EDeviceMatrox_InoutFormatDefault* pParam);
			virtual EMatroxSetFunction SetTimer1AdvancedTriggerArmFormat(_In_ EDeviceMatrox_InoutFormatDefault eParam);

			virtual EMatroxGetFunction GetTimer1AdvancedTriggerArmSource(_Out_ EDeviceMatrox_TriggerArmSource* pParam);
			virtual EMatroxSetFunction SetTimer1AdvancedTriggerArmSource(_In_ EDeviceMatrox_TriggerArmSource eParam);

			virtual EMatroxGetFunction GetTimer1AdvancedTriggerArmPolarity(_Out_ EDeviceMatrox_InoutPolarity* pParam);
			virtual EMatroxSetFunction SetTimer1AdvancedTriggerArmPolarity(_In_ EDeviceMatrox_InoutPolarity eParam);

			virtual EMatroxGetFunction GetTimer2AdvancedTimerOutputSignal(_Out_ EDeviceMatrox_TimerOutputSignal* pParam);
			virtual EMatroxSetFunction SetTimer2AdvancedTimerOutputSignal(_In_ EDeviceMatrox_TimerOutputSignal eParam);

			virtual EMatroxGetFunction GetTimer2AdvancedCaptureATrigger(_Out_ EDeviceMatrox_CaptureATriggerEveryTrigger* pParam);
			virtual EMatroxSetFunction SetTimer2AdvancedCaptureATrigger(_In_ EDeviceMatrox_CaptureATriggerEveryTrigger eParam);

			virtual EMatroxGetFunction GetTimer2AdvancedTriggerArmMode(_Out_ EDeviceMatrox_TriggerArmMode* pParam);
			virtual EMatroxSetFunction SetTimer2AdvancedTriggerArmMode(_In_ EDeviceMatrox_TriggerArmMode eParam);

			virtual EMatroxGetFunction GetTimer2AdvancedTriggerArmFormat(_Out_ EDeviceMatrox_InoutFormatDefault* pParam);
			virtual EMatroxSetFunction SetTimer2AdvancedTriggerArmFormat(_In_ EDeviceMatrox_InoutFormatDefault eParam);

			virtual EMatroxGetFunction GetTimer2AdvancedTriggerArmSource(_Out_ EDeviceMatrox_TriggerArmSource* pParam);
			virtual EMatroxSetFunction SetTimer2AdvancedTriggerArmSource(_In_ EDeviceMatrox_TriggerArmSource eParam);

			virtual EMatroxGetFunction GetTimer2AdvancedTriggerArmPolarity(_Out_ EDeviceMatrox_InoutPolarity* pParam);
			virtual EMatroxSetFunction SetTimer2AdvancedTriggerArmPolarity(_In_ EDeviceMatrox_InoutPolarity eParam);

			virtual EMatroxGetFunction GetGrabMode(_Out_ EDeviceMatrox_GrabMode* pParam);
			virtual EMatroxSetFunction SetGrabMode(_In_ EDeviceMatrox_GrabMode eParam);

			virtual EMatroxGetFunction GetGrabActivationMode(_Out_ EDeviceMatrox_GrabModeActivation* pParam);
			virtual EMatroxSetFunction SetGrabActivationMode(_In_ EDeviceMatrox_GrabModeActivation eParam);

			virtual EMatroxGetFunction GetGrabFormat(_Out_ EDeviceMatrox_InoutFormatDefault* pParam);
			virtual EMatroxSetFunction SetGrabFormat(_In_ EDeviceMatrox_InoutFormatDefault eParam);

			virtual EMatroxGetFunction GetGrabSignal(_Out_ EDeviceMatrox_GrabModeSignal* pParam);
			virtual EMatroxSetFunction SetGrabSignal(_In_ EDeviceMatrox_GrabModeSignal eParam);

			virtual EMatroxGetFunction GetGrabPolarity(_Out_ EDeviceMatrox_InoutPolarity* pParam);
			virtual EMatroxSetFunction SetGrabPolarity(_In_ EDeviceMatrox_InoutPolarity eParam);

			virtual EMatroxGetFunction GetGrabTriggerArmFormat(_Out_ EDeviceMatrox_InoutFormatDefault* pParam);
			virtual EMatroxSetFunction SetGrabTriggerArmFormat(_In_ EDeviceMatrox_InoutFormatDefault eParam);

			virtual EMatroxGetFunction GetGrabTriggerArmSignal(_Out_ EDeviceMatrox_TriggerArmSignal* pParam);
			virtual EMatroxSetFunction SetGrabTriggerArmSignal(_In_ EDeviceMatrox_TriggerArmSignal eParam);

			virtual EMatroxGetFunction GetGrabTriggerArmPolarity(_Out_ EDeviceMatrox_InoutPolarity* pParam);
			virtual EMatroxSetFunction SetGrabTriggerArmPolarity(_In_ EDeviceMatrox_InoutPolarity eParam);

			virtual EMatroxGetFunction GetSyncSignalSource(_Out_ EDeviceMatrox_SyncSignalSource* pParam);
			virtual EMatroxSetFunction SetSyncSignalSource(_In_ EDeviceMatrox_SyncSignalSource eParam);

			virtual EMatroxGetFunction GetSyncSignalFormat(_Out_ EDeviceMatrox_SyncSignalFormat* pParam);
			virtual EMatroxSetFunction SetSyncSignalFormat(_In_ EDeviceMatrox_SyncSignalFormat eParam);

			virtual EMatroxGetFunction GetSyncSignalLatency(_Out_ int* pParam);
			virtual EMatroxSetFunction SetSyncSignalLatency(_In_ int nParam);

			virtual EMatroxGetFunction GetSyncSignalSynchronizAtionSignalAvailable(_Out_ EDeviceMatrox_SynchronizAtionSignalAvailable* pParam);
			virtual EMatroxSetFunction SetSyncSignalSynchronizAtionSignalAvailable(_In_ EDeviceMatrox_SynchronizAtionSignalAvailable eParam);

			virtual EMatroxGetFunction GetSyncSignalBlockSynchronizationTypeEnable(_Out_ bool* pParam);
			virtual EMatroxSetFunction SetSyncSignalBlockSynchronizationTypeEnable(_In_ bool bParam);

			virtual EMatroxGetFunction GetSyncSignalExternalVSYNCSignalIsEnable(_Out_ bool* pParam);
			virtual EMatroxSetFunction SetSyncSignalExternalVSYNCSignalIsEnable(_In_ bool bParam);

			virtual EMatroxGetFunction GetHSyncInputActivce(_Out_ bool* pParam);
			virtual EMatroxSetFunction SetHSyncInputActivce(_In_ bool bParam);

			virtual EMatroxGetFunction GetHSyncInputFormat(_Out_ EDeviceMatrox_InoutFormat* pParam);
			virtual EMatroxSetFunction SetHSyncInputFormat(_In_ EDeviceMatrox_InoutFormat eParam);

			virtual EMatroxGetFunction GetHSyncInputPolarity(_Out_ EDeviceMatrox_InoutPolarity* pParam);
			virtual EMatroxSetFunction SetHSyncInputPolarity(_In_ EDeviceMatrox_InoutPolarity eParam);

			virtual EMatroxGetFunction GetHSyncOutputActivce(_Out_ bool* pParam);
			virtual EMatroxSetFunction SetHSyncOutputActivce(_In_ bool bParam);

			virtual EMatroxGetFunction GetHSyncOutputFormat(_Out_ EDeviceMatrox_InoutFormat* pParam);
			virtual EMatroxSetFunction SetHSyncOutputFormat(_In_ EDeviceMatrox_InoutFormat eParam);

			virtual EMatroxGetFunction GetHSyncOutputPolarity(_Out_ EDeviceMatrox_InoutPolarity* pParam);
			virtual EMatroxSetFunction SetHSyncOutputPolarity(_In_ EDeviceMatrox_InoutPolarity eParam);

			virtual EMatroxGetFunction GetVSyncInputActivce(_Out_ bool* pParam);
			virtual EMatroxSetFunction SetVSyncInputActivce(_In_ bool bParam);

			virtual EMatroxGetFunction GetVSyncInputFormat(_Out_ EDeviceMatrox_InoutFormat* pParam);
			virtual EMatroxSetFunction SetVSyncInputFormat(_In_ EDeviceMatrox_InoutFormat eParam);

			virtual EMatroxGetFunction GetVSyncInputPolarity(_Out_ EDeviceMatrox_InoutPolarity* pParam);
			virtual EMatroxSetFunction SetVSyncInputPolarity(_In_ EDeviceMatrox_InoutPolarity eParam);

			virtual EMatroxGetFunction GetVSyncOutputActivce(_Out_ bool* pParam);
			virtual EMatroxSetFunction SetVSyncOutputActivce(_In_ bool bParam);

			virtual EMatroxGetFunction GetVSyncOutputFormat(_Out_ EDeviceMatrox_InoutFormat* pParam);
			virtual EMatroxSetFunction SetVSyncOutputFormat(_In_ EDeviceMatrox_InoutFormat eParam);

			virtual EMatroxGetFunction GetVSyncOutputPolarity(_Out_ EDeviceMatrox_InoutPolarity* pParam);
			virtual EMatroxSetFunction SetVSyncOutputPolarity(_In_ EDeviceMatrox_InoutPolarity eParam);

			virtual EMatroxGetFunction GetCSyncInputActivce(_Out_ bool* pParam);
			virtual EMatroxSetFunction SetCSyncInputActivce(_In_ bool bParam);

			virtual EMatroxGetFunction GetCSyncInputFormat(_Out_ EDeviceMatrox_InoutFormat* pParam);
			virtual EMatroxSetFunction SetCSyncInputFormat(_In_ EDeviceMatrox_InoutFormat eParam);

			virtual EMatroxGetFunction GetCSyncInputPolarity(_Out_ EDeviceMatrox_InoutPolarity* pParam);
			virtual EMatroxSetFunction SetCSyncInputPolarity(_In_ EDeviceMatrox_InoutPolarity eParam);

			virtual EMatroxGetFunction GetCSyncOutputActivce(_Out_ bool* pParam);
			virtual EMatroxSetFunction SetCSyncOutputActivce(_In_ bool bParam);

			virtual EMatroxGetFunction GetCSyncOutputFormat(_Out_ EDeviceMatrox_InoutFormat* pParam);
			virtual EMatroxSetFunction SetCSyncOutputFormat(_In_ EDeviceMatrox_InoutFormat eParam);

			virtual EMatroxGetFunction GetCSyncOutputPolarity(_Out_ EDeviceMatrox_InoutPolarity* pParam);
			virtual EMatroxSetFunction SetCSyncOutputPolarity(_In_ EDeviceMatrox_InoutPolarity eParam);

			virtual EMatroxGetFunction GetCSyncSerrationPulse(_Out_ bool* pParam);
			virtual EMatroxSetFunction SetCSyncSerrationPulse(_In_ bool bParam);

			virtual EMatroxGetFunction GetCSyncEqualizationPulse(_Out_ bool* pParam);
			virtual EMatroxSetFunction SetCSyncEqualizationPulse(_In_ bool bParam);

			virtual EMatroxGetFunction GetCameraControlBitsSourcesControl1(_Out_ EDeviceMatrox_CameraBitsSourcesControl* pParam);
			virtual EMatroxSetFunction SetCameraControlBitsSourcesControl1(_In_ EDeviceMatrox_CameraBitsSourcesControl eParam);

			virtual EMatroxGetFunction GetCameraControlBitsSourcesControl2(_Out_ EDeviceMatrox_CameraBitsSourcesControl* pParam);
			virtual EMatroxSetFunction SetCameraControlBitsSourcesControl2(_In_ EDeviceMatrox_CameraBitsSourcesControl eParam);

			virtual EMatroxGetFunction GetCameraControlBitsSourcesControl3(_Out_ EDeviceMatrox_CameraBitsSourcesControl* pParam);
			virtual EMatroxSetFunction SetCameraControlBitsSourcesControl3(_In_ EDeviceMatrox_CameraBitsSourcesControl eParam);

			virtual EMatroxGetFunction GetCameraControlBitsSourcesControl4(_Out_ EDeviceMatrox_CameraBitsSourcesControl* pParam);
			virtual EMatroxSetFunction SetCameraControlBitsSourcesControl4(_In_ EDeviceMatrox_CameraBitsSourcesControl eParam);

			virtual EMatroxGetFunction GetEnableCCOutputsOnConnector1(_Out_ bool* pParam);
			virtual EMatroxSetFunction SetEnableCCOutputsOnConnector1(_In_ bool bParam);

			virtual EMatroxGetFunction GetEnableCCOutputsOnConnector2(_Out_ bool* pParam);
			virtual EMatroxSetFunction SetEnableCCOutputsOnConnector2(_In_ bool bParam);

			virtual bool OnParameterChanged(_In_ int nParam, _In_ CString strValue) override;

		protected:
			static MIL_INT MFTYPE ProcessingFunction(MIL_INT HookType, MIL_ID HookId, void* HookDataPtr);

			virtual bool IsError();

			virtual bool DoesModuleExist();

		protected:
			bool m_bIsLine = false;				//line scan camera?
			bool m_bIsColor = false;			//color format?

			int m_nDriveIndex = -1;

			volatile int m_nCurrentCount = 1;	//grabcount
			volatile int m_nConnectBoard = 0;	//boardcount

		public:
			static MIL_ID m_hMilApplication; 		// 1. Application	identifier.

		protected:  
			MIL_ID m_hMilSystem;					// 2. System		identifier.       
			MIL_ID m_hMilDigitizer;					// 3. Digitizer		identifier.
			MIL_ID m_hMilImage;						// 4. Image buffer	identifier.
			MIL_ID* m_pMilImageList = nullptr;		// 5. Image buffer list	identifier.

			Ravid::Algorithms::CRavidImage* m_pTempImage = nullptr;
		};
	}
}
