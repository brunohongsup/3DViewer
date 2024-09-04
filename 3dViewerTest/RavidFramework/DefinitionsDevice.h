#pragma once

#include "DefinitionMVTechBoard.h"

enum class RavidInterfaceControlID
{
	DeviceSheetParameter = 4000,
	DeviceButtonInitialize = 4001,
	DeviceButtonTerminate = 4002,
	DeviceStaticStatus = 4003,
	DeviceComboList = 4004,
	DeviceStaticMessage = 4005,
};

// UI & Data format struct
namespace Ravid
{
	namespace Device
	{
		enum EDeviceAddControlID
		{
			EDeviceAddControlID_BtnGrab = 0x3250,
			EDeviceAddControlID_BtnLive,
			EDeviceAddControlID_BtnStop,
			EDeviceAddControlID_BtnSetSoftwareTrigger,
			EDeviceAddControlID_StaticGroupBox,
			EDeviceAddControlID_StaticSendingFormat,
			EDeviceAddControlID_StaticSendingStatus,
			EDeviceAddControlID_StaticCaption,
			EDeviceAddControlID_EncoderCountCaption,
			EDeviceAddControlID_EncoderCountValue,
			EDeviceAddControlID_TriggerCountCaption,
			EDeviceAddControlID_TriggerCountValue,
			EDeviceAddControlID_EncoderCaption,
			EDeviceAddControlID_TriggerCaption,
			EDeviceAddControlID_LedEncoder,
			EDeviceAddControlID_LedTrigger,
			EDeviceAddControlID_EnableTrigger,
			EDeviceAddControlID_DisableTrigger,
			EDeviceAddControlID_ResetCounter,
			EDeviceAddControlID_BtnMinus,
			EDeviceAddControlID_BtnPause,
			EDeviceAddControlID_BtnPlus,
			EDeviceAddControlID_BtnJog,
			EDeviceAddControlID_BtnMove,
			EDeviceAddControlID_BtnPosition,
			EDeviceAddControlID_EditVelocity,
			EDeviceAddControlID_EditAcceleration,
			EDeviceAddControlID_EditDistance,
			EDeviceAddControlID_EditPosition,
			EDeviceAddControlID_LedLimitPlus,
			EDeviceAddControlID_LedHome,
			EDeviceAddControlID_LedLimitMinus,
			EDeviceAddControlID_LedAlarm,
			EDeviceAddControlID_LedInposition,
			EDeviceAddControlID_CommandPosition,
			EDeviceAddControlID_ActualPosition,
			EDeviceAddControlID_BtnCommandReset,
			EDeviceAddControlID_BtnActualReset,
			EDeviceAddControlID_BtnAlarmReset,
			EDeviceAddControlID_BtnServoModeOn,
			EDeviceAddControlID_BtnServoModeOff,
			EDeviceAddControlID_BtnHomming,
			EDeviceAddControlID_ConnectedCount,
			EDeviceAddControlID_ReceivceData,
			EDeviceAddControlID_SendData,
			EDeviceAddControlID_LedTx,
			EDeviceAddControlID_LedRx,
			EDeviceAddControlID_BtnSerialSend,
			EDeviceAddControlID_EditSerialSend,
			EDeviceAddControlID_EditSerialReceived,
			EDeviceAddControlID_EditSerialStatus,
			EDeviceAddControlID_ComboAxisSelect,
		};

		enum EDeviceAddEnumeratedControlID
		{
			EDeviceAddEnumeratedControlID_InputLedStart = 0x4000,
			EDeviceAddEnumeratedControlID_InputLedEnd = EDeviceAddEnumeratedControlID_InputLedStart + 31,
			EDeviceAddEnumeratedControlID_OutputLedStart = EDeviceAddEnumeratedControlID_InputLedEnd + 1,
			EDeviceAddEnumeratedControlID_OutputLedEnd = EDeviceAddEnumeratedControlID_OutputLedStart + 31,
			EDeviceAddEnumeratedControlID_InputLedLabelStart = EDeviceAddEnumeratedControlID_OutputLedEnd + 1,
			EDeviceAddEnumeratedControlID_InputLedLabelEnd = EDeviceAddEnumeratedControlID_InputLedLabelStart + 31,
			EDeviceAddEnumeratedControlID_OutputLedLabelStart = EDeviceAddEnumeratedControlID_InputLedLabelEnd + 1,
			EDeviceAddEnumeratedControlID_OutputLedLabelEnd = EDeviceAddEnumeratedControlID_OutputLedLabelStart + 31,

			EDeviceAddEnumeratedControlID_ExtInputLedStart = EDeviceAddEnumeratedControlID_OutputLedLabelEnd + 1,
			EDeviceAddEnumeratedControlID_ExtInputLedEnd = EDeviceAddEnumeratedControlID_ExtInputLedStart + 255,
			EDeviceAddEnumeratedControlID_ExtOutputLedStart = EDeviceAddEnumeratedControlID_ExtInputLedEnd + 1,
			EDeviceAddEnumeratedControlID_ExtOutputLedEnd = EDeviceAddEnumeratedControlID_ExtOutputLedStart + 255,
			EDeviceAddEnumeratedControlID_ExtInputLedLabelStart = EDeviceAddEnumeratedControlID_ExtOutputLedEnd + 1,
			EDeviceAddEnumeratedControlID_ExtInputLedLabelEnd = EDeviceAddEnumeratedControlID_ExtInputLedLabelStart + 255,
			EDeviceAddEnumeratedControlID_ExtOutputLedLabelStart = EDeviceAddEnumeratedControlID_ExtInputLedLabelEnd + 1,
			EDeviceAddEnumeratedControlID_ExtOutputLedLabelEnd = EDeviceAddEnumeratedControlID_ExtOutputLedLabelStart + 255,


			EDeviceAddEnumeratedControlID_QueueLedStart = EDeviceAddEnumeratedControlID_ExtOutputLedLabelEnd + 1,
			EDeviceAddEnumeratedControlID_QueueLedEnd = EDeviceAddEnumeratedControlID_QueueLedStart + 288,
			EDeviceAddEnumeratedControlID_QueueLedLabelStart = EDeviceAddEnumeratedControlID_QueueLedEnd + 1,
			EDeviceAddEnumeratedControlID_QueueLedLabelEnd = EDeviceAddEnumeratedControlID_QueueLedLabelStart + 288,


			EDeviceAddEnumeratedControlID_EncCaptionStart = EDeviceAddEnumeratedControlID_QueueLedLabelEnd + 1,
			EDeviceAddEnumeratedControlID_EncCaptionEnd = EDeviceAddEnumeratedControlID_EncCaptionStart + 16,
			EDeviceAddEnumeratedControlID_EncLedStart = EDeviceAddEnumeratedControlID_EncCaptionEnd + 1,
			EDeviceAddEnumeratedControlID_EncLedEnd = EDeviceAddEnumeratedControlID_EncLedStart + 16,


			EDeviceAddEnumeratedControlID_TrgCaptionStart = EDeviceAddEnumeratedControlID_EncLedEnd + 1,
			EDeviceAddEnumeratedControlID_TrgCaptionEnd = EDeviceAddEnumeratedControlID_TrgCaptionStart + 16,
			EDeviceAddEnumeratedControlID_TrgLedStart = EDeviceAddEnumeratedControlID_TrgCaptionEnd + 1,
			EDeviceAddEnumeratedControlID_TrgLedEnd = EDeviceAddEnumeratedControlID_TrgLedStart + 16,

			EDeviceAddEnumeratedControlID_EncButtonStart = EDeviceAddEnumeratedControlID_TrgLedEnd + 1,
			EDeviceAddEnumeratedControlID_EncButtonEnd = EDeviceAddEnumeratedControlID_EncButtonStart + 16,
			EDeviceAddEnumeratedControlID_EnableButtonStart = EDeviceAddEnumeratedControlID_EncButtonEnd + 1,
			EDeviceAddEnumeratedControlID_EnableButtonEnd = EDeviceAddEnumeratedControlID_EnableButtonStart + 16,
			EDeviceAddEnumeratedControlID_DisableButtonStart = EDeviceAddEnumeratedControlID_EnableButtonEnd + 1,
			EDeviceAddEnumeratedControlID_DisableButtonEnd = EDeviceAddEnumeratedControlID_DisableButtonStart + 16,
			EDeviceAddEnumeratedControlID_ResetButtonStart = EDeviceAddEnumeratedControlID_DisableButtonEnd + 1,
			EDeviceAddEnumeratedControlID_ResetButtonEnd = EDeviceAddEnumeratedControlID_ResetButtonStart + 16,

			EDeviceAddEnumeratedControlID_TriggerIDStart = EDeviceAddEnumeratedControlID_ResetButtonEnd + 1,
			EDeviceAddEnumeratedControlID_TriggerIDEnd = EDeviceAddEnumeratedControlID_TriggerIDStart + 64,

			EDeviceAddEnumeratedControlID_StaticStart = EDeviceAddEnumeratedControlID_TriggerIDEnd + 1,
			EDeviceAddEnumeratedControlID_StaticEnd = EDeviceAddEnumeratedControlID_StaticStart + 256,

		};

		enum EParameterFieldType
		{
			EParameterFieldType_None = 0,
			EParameterFieldType_Edit,
			EParameterFieldType_Static,
			EParameterFieldType_Combo,
			EParameterFieldType_Radio,
			EParameterFieldType_Check,
			EParameterFieldType_Spinner,
			EParameterFieldType_IPAddress,
			EParameterFieldType_HideEdit,
			EParameterFieldType_Count,
		};

		typedef struct sDeviceParameterFieldConfigurations
		{
			bool bShow = true;
			int nDepth = 0;
			int nPrimaryKey = 0;

			EParameterFieldType eType = EParameterFieldType_None;

			CString strParameterName = _T("");
			CString strParameterValue = _T("");
			CString strOptComboList = _T("");
			CString strToolTip = _T("");
			CString strIntroduction = _T("");

			void* pTreeItem = nullptr;
		}
		SDeviceParameterFieldConfigurations, *LPDeviceParameterFieldConfigurations;

		typedef struct sRegisterDeviceInfo
		{
			int nSequence = 0;
			CString strDeviceClass = _T("");
			CString strDeviceOpt = _T("");
		}SRegisterDeviceInfo;
	}
}

namespace Ravid
{
	namespace Device
	{
		enum EDeviceParameter
		{
			EDeviceParameter_NotUsed = -1,
		};

		enum EDeviceParameterBaslerGigE
		{
			EDeviceParameterBaslerGigE_DeviceID = 0,
			EDeviceParameterBaslerGigE_CameraSetting,
			EDeviceParameterBaslerGigE_GrabCount,
			EDeviceParameterBaslerGigE_CanvasWidth,
			EDeviceParameterBaslerGigE_CanvasHeight,
			EDeviceParameterBaslerGigE_OffsetCenterX,
			EDeviceParameterBaslerGigE_OffsetCenterY,
			EDeviceParameterBaslerGigE_OffsetX,
			EDeviceParameterBaslerGigE_OffsetY,
			EDeviceParameterBaslerGigE_GrabWaitingTime,
			EDeviceParameterBaslerGigE_PixelFormat,
			EDeviceParameterBaslerGigE_PacketSize,
			EDeviceParameterBaslerGigE_TriggerSetting,
			EDeviceParameterBaslerGigE_TriggerSelector,
			EDeviceParameterBaslerGigE_FrameStart,
			EDeviceParameterBaslerGigE_FrameStartTriggerMode,
			EDeviceParameterBaslerGigE_FrameStartTriggerSource,
			EDeviceParameterBaslerGigE_FrameStartTriggerActivation,
			EDeviceParameterBaslerGigE_LineStart,
			EDeviceParameterBaslerGigE_LineStartTriggerMode,
			EDeviceParameterBaslerGigE_LineStartTriggerSource,
			EDeviceParameterBaslerGigE_LineStartTriggerActivation,
			EDeviceParameterBaslerGigE_AcquisitionStart,
			EDeviceParameterBaslerGigE_AcquisitionStartTriggerMode,
			EDeviceParameterBaslerGigE_AcquisitionStartTriggerSource,
			EDeviceParameterBaslerGigE_AcquisitionStartTriggerActivation,
			EDeviceParameterBaslerGigE_ExposureAndGainSetting,
			EDeviceParameterBaslerGigE_ExposureAuto,
			EDeviceParameterBaslerGigE_Exposure,
			EDeviceParameterBaslerGigE_GainAuto,
			EDeviceParameterBaslerGigE_GainSelector,
			EDeviceParameterBaslerGigE_Gain,
			EDeviceParameterBaslerGigE_AnalogGain,
			EDeviceParameterBaslerGigE_DigitalGain,
			EDeviceParameterBaslerGigE_Tap1Gain,
			EDeviceParameterBaslerGigE_Tap2Gain,

			EDeviceParameterBaslerGigE_ColorImprovementsControl,
			EDeviceParameterBaslerGigE_LightSourceSelector,
			EDeviceParameterBaslerGigE_BalanceWhiteReset,
			EDeviceParameterBaslerGigE_BalanceWhiteAuto,
			EDeviceParameterBaslerGigE_BalanceRationRawRed,
			EDeviceParameterBaslerGigE_BalanceRationRawGreen,
			EDeviceParameterBaslerGigE_BalanceRationRawBlue,


			// LightSource selector -> Custom 시 사용
			EDeviceParameterBaslerGigE_ColorTransformationGain,

			EDeviceParameterBaslerGigE_ColorTransValueRawGain00,
			EDeviceParameterBaslerGigE_ColorTransValueRawGain01,
			EDeviceParameterBaslerGigE_ColorTransValueRawGain02,

			EDeviceParameterBaslerGigE_ColorTransValueRawGain10,
			EDeviceParameterBaslerGigE_ColorTransValueRawGain11,
			EDeviceParameterBaslerGigE_ColorTransValueRawGain12,

			EDeviceParameterBaslerGigE_ColorTransValueRawGain20,
			EDeviceParameterBaslerGigE_ColorTransValueRawGain21,
			EDeviceParameterBaslerGigE_ColorTransValueRawGain22,

			// LightSource selector -> off, custom 제외한 나머지

			EDeviceParameterBaslerGigE_ColorTransformationMatrix,

			EDeviceParameterBaslerGigE_ColorTransMatrixFactorRaw,

			// 일반

			EDeviceParameterBaslerGigE_DigitalIOConrol,
			EDeviceParameterBaslerGigE_LineSelector,
			EDeviceParameterBaslerGigE_Line1,
			EDeviceParameterBaslerGigE_Line1Mode,
			EDeviceParameterBaslerGigE_Line1Source,
			EDeviceParameterBaslerGigE_Line1Inverter,
			EDeviceParameterBaslerGigE_Line1DebouncerTime_us,
			EDeviceParameterBaslerGigE_Line1MinimumOuputPulseWidth_us,
			EDeviceParameterBaslerGigE_Line2,
			EDeviceParameterBaslerGigE_Line2Mode,
			EDeviceParameterBaslerGigE_Line2Source,
			EDeviceParameterBaslerGigE_Line2Inverter,
			EDeviceParameterBaslerGigE_Line2DebouncerTime_us,
			EDeviceParameterBaslerGigE_Line2MinimumOuputPulseWidth_us,
			EDeviceParameterBaslerGigE_Line3,
			EDeviceParameterBaslerGigE_Line3Mode,
			EDeviceParameterBaslerGigE_Line3Source,
			EDeviceParameterBaslerGigE_Line3Inverter,
			EDeviceParameterBaslerGigE_Line3DebouncerTime_us,
			EDeviceParameterBaslerGigE_Line3MinimumOuputPulseWidth_us,
			EDeviceParameterBaslerGigE_Line4,
			EDeviceParameterBaslerGigE_Line4Mode,
			EDeviceParameterBaslerGigE_Line4Source,
			EDeviceParameterBaslerGigE_Line4Inverter,
			EDeviceParameterBaslerGigE_Line4DebouncerTime_us,
			EDeviceParameterBaslerGigE_Line4MinimumOuputPulseWidth_us,
			EDeviceParameterBaslerGigE_Count,
		};

		enum EDeviceParameterBaslerUSB
		{
			EDeviceParameterBaslerUSB_DeviceID = 0,
			EDeviceParameterBaslerUSB_CameraSetting,
			EDeviceParameterBaslerUSB_GrabCount,
			EDeviceParameterBaslerUSB_CanvasWidth,
			EDeviceParameterBaslerUSB_CanvasHeight,
			EDeviceParameterBaslerUSB_OffsetCenterX,
			EDeviceParameterBaslerUSB_OffsetCenterY,
			EDeviceParameterBaslerUSB_OffsetX,
			EDeviceParameterBaslerUSB_OffsetY,
			EDeviceParameterBaslerUSB_ReverseX,
			EDeviceParameterBaslerUSB_ReverseY,
			EDeviceParameterBaslerUSB_GrabWaitingTime,
			EDeviceParameterBaslerUSB_PixelFormat,
			EDeviceParameterBaslerUSB_AnalogControl,
			EDeviceParameterBaslerUSB_GainAuto,
			EDeviceParameterBaslerUSB_GainSelector,
			EDeviceParameterBaslerUSB_Gain,
			EDeviceParameterBaslerUSB_BlackLevelSelector,
			EDeviceParameterBaslerUSB_BlackLevel,
			EDeviceParameterBaslerUSB_Gamma,
			EDeviceParameterBaslerUSB_AcquisitionControl,
			EDeviceParameterBaslerUSB_ShutterMode,
			EDeviceParameterBaslerUSB_ExposureAuto,
			EDeviceParameterBaslerUSB_ExposureMode,
			EDeviceParameterBaslerUSB_ExposureTime,
			EDeviceParameterBaslerUSB_TriggerSelector,
			EDeviceParameterBaslerUSB_FrameStart,
			EDeviceParameterBaslerUSB_FrameStartTriggerMode,
			EDeviceParameterBaslerUSB_FrameStartTriggerSource,
			EDeviceParameterBaslerUSB_FrameStartTriggerActivation,
			EDeviceParameterBaslerUSB_FrameStartTriggerDelay,
			EDeviceParameterBaslerUSB_FrameBurstStart,
			EDeviceParameterBaslerUSB_FrameBurstStartTriggerMode,
			EDeviceParameterBaslerUSB_FrameBurstStartTriggerSource,
			EDeviceParameterBaslerUSB_FrameBurstStartTriggerActivation,
			EDeviceParameterBaslerUSB_FrameBurstStartTriggerDelay,
			EDeviceParameterBaslerUSB_EnableAcquisitionFrameRate,
			EDeviceParameterBaslerUSB_AcquisitionFrameRate,
			EDeviceParameterBaslerUSB_AcquisitionStatusSelector,
			EDeviceParameterBaslerUSB_DigitalIOConrol,
			EDeviceParameterBaslerUSB_LineSelector,
			EDeviceParameterBaslerUSB_Line1,
			EDeviceParameterBaslerUSB_Line1Mode,
			EDeviceParameterBaslerUSB_Line1Source,
			EDeviceParameterBaslerUSB_Line1Inverter,
			EDeviceParameterBaslerUSB_Line1DebouncerTime_us,
			EDeviceParameterBaslerUSB_Line1MinimumOuputPulseWidth_us,
			EDeviceParameterBaslerUSB_Line2,
			EDeviceParameterBaslerUSB_Line2Mode,
			EDeviceParameterBaslerUSB_Line2Source,
			EDeviceParameterBaslerUSB_Line2Inverter,
			EDeviceParameterBaslerUSB_Line2DebouncerTime_us,
			EDeviceParameterBaslerUSB_Line2MinimumOuputPulseWidth_us,
			EDeviceParameterBaslerUSB_Line3,
			EDeviceParameterBaslerUSB_Line3Mode,
			EDeviceParameterBaslerUSB_Line3Source,
			EDeviceParameterBaslerUSB_Line3Inverter,
			EDeviceParameterBaslerUSB_Line3DebouncerTime_us,
			EDeviceParameterBaslerUSB_Line3MinimumOuputPulseWidth_us,
			EDeviceParameterBaslerUSB_Line4,
			EDeviceParameterBaslerUSB_Line4Mode,
			EDeviceParameterBaslerUSB_Line4Source,
			EDeviceParameterBaslerUSB_Line4Inverter,
			EDeviceParameterBaslerUSB_Line4DebouncerTime_us,
			EDeviceParameterBaslerUSB_Line4MinimumOuputPulseWidth_us,
			EDeviceParameterBaslerUSB_Count,
		};

		enum EDeviceParameterBaumerGigE
		{
			EDeviceParameterBaumerGigE_DeviceID = 0,
			EDeviceParameterBaumerGigE_CameraSetting,
			EDeviceParameterBaumerGigE_GrabCount,
			EDeviceParameterBaumerGigE_CanvasWidth,
			EDeviceParameterBaumerGigE_CanvasHeight,
			EDeviceParameterBaumerGigE_OffsetX,
			EDeviceParameterBaumerGigE_OffsetY,
			EDeviceParameterBaumerGigE_GrabWaitingTime,
			EDeviceParameterBaumerGigE_GrabBufferCount,
			EDeviceParameterBaumerGigE_PixelFormat,
			EDeviceParameterBaumerGigE_FlipHorizontal,
			EDeviceParameterBaumerGigE_FlipVertical,
			EDeviceParameterBaumerGigE_TriggerSetting,
			EDeviceParameterBaumerGigE_TriggerSelector,
			EDeviceParameterBaumerGigE_FrameStart,
			EDeviceParameterBaumerGigE_FrameStartTriggerMode,
			EDeviceParameterBaumerGigE_FrameStartTriggerSource,
			EDeviceParameterBaumerGigE_FrameStartTriggerActivation,
			EDeviceParameterBaumerGigE_LineStart,
			EDeviceParameterBaumerGigE_LineStartTriggerMode,
			EDeviceParameterBaumerGigE_LineStartTriggerSource,
			EDeviceParameterBaumerGigE_LineStartTriggerActivation,
			EDeviceParameterBaumerGigE_AcquisitionStart,
			EDeviceParameterBaumerGigE_AcquisitionStartTriggerMode,
			EDeviceParameterBaumerGigE_AcquisitionStartTriggerSource,
			EDeviceParameterBaumerGigE_AcquisitionStartTriggerActivation,
			EDeviceParameterBaumerGigE_ExposureAndGainSetting,
			EDeviceParameterBaumerGigE_ExposureAuto,
			EDeviceParameterBaumerGigE_ExposureMode,
			EDeviceParameterBaumerGigE_Exposure,
			EDeviceParameterBaumerGigE_GainAuto,
			EDeviceParameterBaumerGigE_GainSelector,
			EDeviceParameterBaumerGigE_Gain,
			EDeviceParameterBaumerGigE_AnalogGain,
			EDeviceParameterBaumerGigE_DigitalGain,
			EDeviceParameterBaumerGigE_Tap1Gain,
			EDeviceParameterBaumerGigE_Tap2Gain,
			EDeviceParameterBaumerGigE_DigitalIOControl,
			EDeviceParameterBaumerGigE_LineSelector,
			EDeviceParameterBaumerGigE_Line0,
			EDeviceParameterBaumerGigE_Line0Inverter,
			EDeviceParameterBaumerGigE_Line0Mode,
			EDeviceParameterBaumerGigE_Line1,
			EDeviceParameterBaumerGigE_Line1Inverter,
			EDeviceParameterBaumerGigE_Line1Mode,
			EDeviceParameterBaumerGigE_Line1Source,
			EDeviceParameterBaumerGigE_Line2,
			EDeviceParameterBaumerGigE_Line2Inverter,
			EDeviceParameterBaumerGigE_Line2Mode,
			EDeviceParameterBaumerGigE_Line2Source,
			EDeviceParameterBaumerGigE_Line3,
			EDeviceParameterBaumerGigE_Line3Inverter,
			EDeviceParameterBaumerGigE_Line3Mode,
			EDeviceParameterBaumerGigE_Line3Source,
			EDeviceParameterBaumerGigE_UserOutputSelector,
			EDeviceParameterBaumerGigE_UserOutputValue,
			EDeviceParameterBaumerGigE_UserOutputValueAll,

			EDeviceParameterBaumerGigE_TimerControl,
			EDeviceParameterBaumerGigE_TimerSelector,
			EDeviceParameterBaumerGigE_TimerTriggerSource,
			EDeviceParameterBaumerGigE_TimerTriggerActivation,
			EDeviceParameterBaumerGigE_TimerDuration,
			EDeviceParameterBaumerGigE_TimerDelay,
			EDeviceParameterBaumerGigE_Count,
		};

		enum EDeviceParameterDahuaGigE
		{
			EDeviceParameterDahuaGigE_DeviceID = 0,
			EDeviceParameterDahuaGigE_GrabCount,
			EDeviceParameterDahuaGigE_ImageFormatControl,
			EDeviceParameterDahuaGigE_CanvasWidth,
			EDeviceParameterDahuaGigE_CanvasHeight,
			EDeviceParameterDahuaGigE_OffsetX,
			EDeviceParameterDahuaGigE_OffsetY,
			EDeviceParameterDahuaGigE_ReverseX,
			EDeviceParameterDahuaGigE_ReverseY,
			EDeviceParameterDahuaGigE_PixelFormat,
			EDeviceParameterDahuaGigE_AcquisitionControl,
			EDeviceParameterDahuaGigE_AcquisitionMode,
			EDeviceParameterDahuaGigE_AcquisitionFrameCount,
			EDeviceParameterDahuaGigE_AcquisitionFrameRate,
			EDeviceParameterDahuaGigE_AcquisitionFrameRateEnable,
			EDeviceParameterDahuaGigE_AcquisitionStatusSelector,
			EDeviceParameterDahuaGigE_TriggerSelector,
			EDeviceParameterDahuaGigE_FrameStart,
			EDeviceParameterDahuaGigE_FrameStartTriggerMode,
			EDeviceParameterDahuaGigE_FrameStartTriggerSource,
			EDeviceParameterDahuaGigE_FrameStartTriggerActivation,
			EDeviceParameterDahuaGigE_FrameStartTriggerDelay,
			EDeviceParameterDahuaGigE_AcquisitionStart,
			EDeviceParameterDahuaGigE_AcquisitionStartTriggerMode,
			EDeviceParameterDahuaGigE_AcquisitionStartTriggerSource,
			EDeviceParameterDahuaGigE_AcquisitionStartTriggerActivation,
			EDeviceParameterDahuaGigE_AcquisitionStartTriggerDelay,
			EDeviceParameterDahuaGigE_LightTriggerDelay,
			EDeviceParameterDahuaGigE_ExposureMode,
			EDeviceParameterDahuaGigE_ExposureTime,
			EDeviceParameterDahuaGigE_ExposureAuto,
			EDeviceParameterDahuaGigE_AnalogControl,
			EDeviceParameterDahuaGigE_GainSelector,
			EDeviceParameterDahuaGigE_AllGainRaw,
			EDeviceParameterDahuaGigE_BlackLevelSelector,
			EDeviceParameterDahuaGigE_BlackLevel,
			EDeviceParameterDahuaGigE_BlackLevelAuto,
			EDeviceParameterDahuaGigE_Gamma,
			EDeviceParameterDahuaGigE_DigitalIOControl,
			EDeviceParameterDahuaGigE_LineSelector,
			EDeviceParameterDahuaGigE_Line0,
			EDeviceParameterDahuaGigE_Line0Inverter,
			EDeviceParameterDahuaGigE_Line0Source,
			EDeviceParameterDahuaGigE_Line0Format,
			EDeviceParameterDahuaGigE_Line1,
			EDeviceParameterDahuaGigE_Line1Inverter,
			EDeviceParameterDahuaGigE_Line1Source,
			EDeviceParameterDahuaGigE_Line1Format,
			EDeviceParameterDahuaGigE_Count,
		};

		enum EDeviceParameterDalsaGenieNanoGigE
		{
			EDeviceParameterDalsaGenieNanoGigE_DeviceID = 0,
			EDeviceParameterDalsaGenieNanoGigE_CameraSetting,
			EDeviceParameterDalsaGenieNanoGigE_GrabCount,
			EDeviceParameterDalsaGenieNanoGigE_CanvasWidth,
			EDeviceParameterDalsaGenieNanoGigE_CanvasHeight,
			EDeviceParameterDalsaGenieNanoGigE_OffsetX,
			EDeviceParameterDalsaGenieNanoGigE_OffsetY,
			EDeviceParameterDalsaGenieNanoGigE_FlipHorizontal,
			EDeviceParameterDalsaGenieNanoGigE_FlipVertical,
			EDeviceParameterDalsaGenieNanoGigE_PixelFormat,
			EDeviceParameterDalsaGenieNanoGigE_PacketSize,
			EDeviceParameterDalsaGenieNanoGigE_DeviceBufferCount,
			EDeviceParameterDalsaGenieNanoGigE_TriggerSetting,
			EDeviceParameterDalsaGenieNanoGigE_TriggerSelector,
			EDeviceParameterDalsaGenieNanoGigE_FrameStart,
			EDeviceParameterDalsaGenieNanoGigE_FrameStartTriggerMode,
			EDeviceParameterDalsaGenieNanoGigE_FrameStartTriggerSource,
			EDeviceParameterDalsaGenieNanoGigE_FrameStartTriggerActivation,
			EDeviceParameterDalsaGenieNanoGigE_FrameBurstStart,
			EDeviceParameterDalsaGenieNanoGigE_FrameBurstStartTriggerMode,
			EDeviceParameterDalsaGenieNanoGigE_FrameBurstStartTriggerSource,
			EDeviceParameterDalsaGenieNanoGigE_FrameBurstStartTriggerCount,
			EDeviceParameterDalsaGenieNanoGigE_AcquisitionStart,
			EDeviceParameterDalsaGenieNanoGigE_AcquisitionStartTriggerMode,
			EDeviceParameterDalsaGenieNanoGigE_AcquisitionStartTriggerSource,
			EDeviceParameterDalsaGenieNanoGigE_ExposureAndGainSetting,
			EDeviceParameterDalsaGenieNanoGigE_FrameRateControl,
			EDeviceParameterDalsaGenieNanoGigE_FrameRate,
			EDeviceParameterDalsaGenieNanoGigE_ExposureTime,
			EDeviceParameterDalsaGenieNanoGigE_GainSelector,
			EDeviceParameterDalsaGenieNanoGigE_SensorGain,
			EDeviceParameterDalsaGenieNanoGigE_DigitalGain,
			EDeviceParameterDalsaGenieNanoGigE_BlackLevelSelector,
			EDeviceParameterDalsaGenieNanoGigE_BlackLevel,
			EDeviceParameterDalsaGenieNanoGigE_LineSelector,
			EDeviceParameterDalsaGenieNanoGigE_Line1,
			EDeviceParameterDalsaGenieNanoGigE_Line1DebouncingPeriod,
			EDeviceParameterDalsaGenieNanoGigE_Line1Inverter,
			EDeviceParameterDalsaGenieNanoGigE_Line2,
			EDeviceParameterDalsaGenieNanoGigE_Line2DebouncingPeriod,
			EDeviceParameterDalsaGenieNanoGigE_Line2Inverter,
			EDeviceParameterDalsaGenieNanoGigE_Line3,
			EDeviceParameterDalsaGenieNanoGigE_Line3Source,
			EDeviceParameterDalsaGenieNanoGigE_Line3Inverter,
			EDeviceParameterDalsaGenieNanoGigE_Line3Activation,
			EDeviceParameterDalsaGenieNanoGigE_Line3Delay,
			EDeviceParameterDalsaGenieNanoGigE_Line3Duration,
			EDeviceParameterDalsaGenieNanoGigE_Line3Value,
			EDeviceParameterDalsaGenieNanoGigE_Line4,
			EDeviceParameterDalsaGenieNanoGigE_Line4Source,
			EDeviceParameterDalsaGenieNanoGigE_Line4Inverter,
			EDeviceParameterDalsaGenieNanoGigE_Line4Activation,
			EDeviceParameterDalsaGenieNanoGigE_Line4Delay,
			EDeviceParameterDalsaGenieNanoGigE_Line4Duration,
			EDeviceParameterDalsaGenieNanoGigE_Line4Value,
			EDeviceParameterDalsaGenieNanoGigE_Count,
		};

		enum EDeviceParameterPointGrey
		{
			EDeviceParameterPointGrey_DeviceID = 0,
			EDeviceParameterPointGrey_VideoModes,
			EDeviceParameterPointGrey_GrabWaitingTime,
			EDeviceParameterPointGrey_GrabMode,
			EDeviceParameterPointGrey_GrabNumBuffers,
			EDeviceParameterPointGrey_CanvasWidth,
			EDeviceParameterPointGrey_CanvasHeight,
			EDeviceParameterPointGrey_OffsetX,
			EDeviceParameterPointGrey_OffsetY,
			EDeviceParameterPointGrey_PixelFormat,
			EDeviceParameterPointGrey_FormatMode,
			EDeviceParameterPointGrey_CameraSetting,
			EDeviceParameterPointGrey_GrabCount,
			EDeviceParameterPointGrey_PacketSize,
			EDeviceParameterPointGrey_PacketDelay,
			EDeviceParameterPointGrey_BrightnessSetting,
			EDeviceParameterPointGrey_BrightnessManual,
			EDeviceParameterPointGrey_BrightnessValue,
			EDeviceParameterPointGrey_ExposureSetting,
			EDeviceParameterPointGrey_ExposureManual,
			EDeviceParameterPointGrey_ExposureValue,
			EDeviceParameterPointGrey_SharpnessSetting,
			EDeviceParameterPointGrey_SharpnessManual,
			EDeviceParameterPointGrey_SharpnessValue,
			EDeviceParameterPointGrey_WhiteBalanceSetting,
			EDeviceParameterPointGrey_WhiteBalanceManual,
			EDeviceParameterPointGrey_WhiteBalanceValue,
			EDeviceParameterPointGrey_HueSetting,
			EDeviceParameterPointGrey_HueManual,
			EDeviceParameterPointGrey_HueValue,
			EDeviceParameterPointGrey_SaturationSetting,
			EDeviceParameterPointGrey_SaturationManual,
			EDeviceParameterPointGrey_SaturationValue,
			EDeviceParameterPointGrey_GammaSetting,
			EDeviceParameterPointGrey_GammaManual,
			EDeviceParameterPointGrey_GammaValue,
			EDeviceParameterPointGrey_IrisSetting,
			EDeviceParameterPointGrey_IrisManual,
			EDeviceParameterPointGrey_IrisValue,
			EDeviceParameterPointGrey_FocusSetting,
			EDeviceParameterPointGrey_FocusManual,
			EDeviceParameterPointGrey_FocusValue,
			EDeviceParameterPointGrey_ZoomSetting,
			EDeviceParameterPointGrey_ZoomManual,
			EDeviceParameterPointGrey_ZoomValue,
			EDeviceParameterPointGrey_PanSetting,
			EDeviceParameterPointGrey_PanManual,
			EDeviceParameterPointGrey_PanValue,
			EDeviceParameterPointGrey_TiltSetting,
			EDeviceParameterPointGrey_TiltManual,
			EDeviceParameterPointGrey_TiltValue,
			EDeviceParameterPointGrey_ShutterSetting,
			EDeviceParameterPointGrey_ShutterManual,
			EDeviceParameterPointGrey_ShutterValue,
			EDeviceParameterPointGrey_GainSetting,
			EDeviceParameterPointGrey_GainManual,
			EDeviceParameterPointGrey_GainValue,
			EDeviceParameterPointGrey_FrameRateSetting,
			EDeviceParameterPointGrey_FrameRateManual,
			EDeviceParameterPointGrey_FrameRateValue,
			EDeviceParameterPointGrey_TriggerSetting,
			EDeviceParameterPointGrey_TriggerOnOff,
			EDeviceParameterPointGrey_TriggerMode,
			EDeviceParameterPointGrey_TriggerSource,
			EDeviceParameterPointGrey_TriggerParameter,
			EDeviceParameterPointGrey_TriggerPolarity,
			EDeviceParameterPointGrey_Count,
		};

		enum EDeviceParameterMulticam
		{
			EDeviceParameterMulticam_DeviceID = 0,
			EDeviceParameterMulticam_InitializeType,
			EDeviceParameterMulticam_CamfilePath,
			EDeviceParameterMulticam_CamfileMatch,
			EDeviceParameterMulticam_Count,
		};

		enum EDeviceParameterEuresysDominoHarmony
		{
			EDeviceParameterEuresysDominoHarmony_Topology = EDeviceParameterMulticam_Count,
			EDeviceParameterEuresysDominoHarmony_Connector,
			EDeviceParameterEuresysDominoHarmony_Count,
		};

		enum EDeviceParameterEuresysDominoMelody
		{
			EDeviceParameterEuresysDominoMelody_Topology = EDeviceParameterMulticam_Count,
			EDeviceParameterEuresysDominoMelody_Connector,
			EDeviceParameterEuresysDominoMelody_Count,
		};

		enum EDeviceParameterEuresysDominoSymphony
		{
			EDeviceParameterEuresysDominoSymphony_Topology = EDeviceParameterMulticam_Count,
			EDeviceParameterEuresysDominoSymphony_Connector,
			EDeviceParameterEuresysDominoSymphony_Count,
		};

		enum EDeviceParameterEuresysDomino
		{
			EDeviceParameterEuresysDomino_SummaryControl = EDeviceParameterEuresysDominoSymphony_Count,
			EDeviceParameterEuresysDomino_AcquisitionMode,
			EDeviceParameterEuresysDomino_TrigMode,
			EDeviceParameterEuresysDomino_Expose_us,
			EDeviceParameterEuresysDomino_AreaCamera,
			EDeviceParameterEuresysDomino_SeqLength_Fr,
			EDeviceParameterEuresysDomino_PhaseLength_Fr,
			EDeviceParameterEuresysDomino_TapStructure,
			EDeviceParameterEuresysDomino_Gain,
			EDeviceParameterEuresysDomino_Offset,
			EDeviceParameterEuresysDomino_VideoFilter,
			EDeviceParameterEuresysDomino_CameraSpecification,
			EDeviceParameterEuresysDomino_CamConfig,
			EDeviceParameterEuresysDomino_Camera,
			EDeviceParameterEuresysDomino_DataLink,
			EDeviceParameterEuresysDomino_Imaging,
			EDeviceParameterEuresysDomino_Spectrum,
			EDeviceParameterEuresysDomino_CameraTiming,
			EDeviceParameterEuresysDomino_Vactive_Ln,
			EDeviceParameterEuresysDomino_VsyncAft_Ln,
			EDeviceParameterEuresysDomino_LineRate_Hz,
			EDeviceParameterEuresysDomino_ACResetPostDelay_us,
			EDeviceParameterEuresysDomino_ACResetWidth_us,
			EDeviceParameterEuresysDomino_HCSyncAft_ns,
			EDeviceParameterEuresysDomino_HCsyncBfr_ns,
			EDeviceParameterEuresysDomino_HCsyncDur_ns,
			EDeviceParameterEuresysDomino_HsyncDly_ns,
			EDeviceParameterEuresysDomino_HdriveDur_ns,
			EDeviceParameterEuresysDomino_HdriveDly_ns,
			EDeviceParameterEuresysDomino_Hactive_ns,
			EDeviceParameterEuresysDomino_PixelClk_Hz,
			EDeviceParameterEuresysDomino_Scanning,
			EDeviceParameterEuresysDomino_Standard,
			EDeviceParameterEuresysDomino_VCsyncAft_Ln,
			EDeviceParameterEuresysDomino_VCgatePos_Ln,
			EDeviceParameterEuresysDomino_Vtotal_Ln,
			EDeviceParameterEuresysDomino_VsyncBfrEndExp_us,
			EDeviceParameterEuresysDomino_VdriveDur_Ln,
			EDeviceParameterEuresysDomino_VdriveDly_Ln,
			EDeviceParameterEuresysDomino_CameraFeatures,
			EDeviceParameterEuresysDomino_AuxResetCtl,
			EDeviceParameterEuresysDomino_ColorMethod,
			EDeviceParameterEuresysDomino_Expose,
			EDeviceParameterEuresysDomino_ExposeMin_us,
			EDeviceParameterEuresysDomino_ExposeMax_us,
			EDeviceParameterEuresysDomino_ExposeOverlap,
			EDeviceParameterEuresysDomino_Readout,
			EDeviceParameterEuresysDomino_ResetCtl,
			EDeviceParameterEuresysDomino_ResetEdge,
			EDeviceParameterEuresysDomino_ACReset,
			EDeviceParameterEuresysDomino_ChannelTopology,
			EDeviceParameterEuresysDomino_CsyncCtl,
			EDeviceParameterEuresysDomino_CsyncEdge,
			EDeviceParameterEuresysDomino_CameraControlSignalSet,
			EDeviceParameterEuresysDomino_CameraSignalSet,
			EDeviceParameterEuresysDomino_HReset,
			EDeviceParameterEuresysDomino_HsyncCtl,
			EDeviceParameterEuresysDomino_HsyncEdge,
			EDeviceParameterEuresysDomino_HVdriveCtl,
			EDeviceParameterEuresysDomino_HVdriveEdge,
			EDeviceParameterEuresysDomino_PixelClkCtl,
			EDeviceParameterEuresysDomino_ResetDur,
			EDeviceParameterEuresysDomino_ResetPos,
			EDeviceParameterEuresysDomino_VCSync,
			EDeviceParameterEuresysDomino_VHSync,
			EDeviceParameterEuresysDomino_VsyncCtl,
			EDeviceParameterEuresysDomino_VsyncEdge,
			EDeviceParameterEuresysDomino_CableFeatures,
			EDeviceParameterEuresysDomino_AuxResetLine,
			EDeviceParameterEuresysDomino_ResetLine,
			EDeviceParameterEuresysDomino_Cable,
			EDeviceParameterEuresysDomino_CsyncLine,
			EDeviceParameterEuresysDomino_HdriveLine,
			EDeviceParameterEuresysDomino_HsyncLine,
			EDeviceParameterEuresysDomino_PixelClkLine,
			EDeviceParameterEuresysDomino_VdriveLine,
			EDeviceParameterEuresysDomino_VsyncLine,
			EDeviceParameterEuresysDomino_CableTopology,
			EDeviceParameterEuresysDomino_AcquisitionControl,
			EDeviceParameterEuresysDomino_ActivityLength,
			EDeviceParameterEuresysDomino_BreakEffect,
			EDeviceParameterEuresysDomino_EndTrigMode,
			EDeviceParameterEuresysDomino_NextTrigMode,
			EDeviceParameterEuresysDomino_GrabField,
			EDeviceParameterEuresysDomino_NextGrabField,
			EDeviceParameterEuresysDomino_TriggerControl,
			EDeviceParameterEuresysDomino_ForceTrig,
			EDeviceParameterEuresysDomino_TrigDelay_us,
			EDeviceParameterEuresysDomino_ExposureControl,
			EDeviceParameterEuresysDomino_ExposeTrim,
			EDeviceParameterEuresysDomino_TrueExp_us,
			EDeviceParameterEuresysDomino_StrobeControl,
			EDeviceParameterEuresysDomino_StrobeCtl,
			EDeviceParameterEuresysDomino_StrobeDur,
			EDeviceParameterEuresysDomino_StrobeMode,
			EDeviceParameterEuresysDomino_StrobePos,
			EDeviceParameterEuresysDomino_PreStrobe_us,
			EDeviceParameterEuresysDomino_StrobeLine,
			EDeviceParameterEuresysDomino_StrobeLevel,
			EDeviceParameterEuresysDomino_GrabberConfiguration,
			EDeviceParameterEuresysDomino_ADresolution,
			EDeviceParameterEuresysDomino_VideoTerminator,
			EDeviceParameterEuresysDomino_GrabberTiming,
			EDeviceParameterEuresysDomino_GrabWindow,
			EDeviceParameterEuresysDomino_OffsetX_Px,
			EDeviceParameterEuresysDomino_OffsetY_Ln,
			EDeviceParameterEuresysDomino_SampleTrim,
			EDeviceParameterEuresysDomino_SampleClk_Hz,
			EDeviceParameterEuresysDomino_GrabberConditioning,
			EDeviceParameterEuresysDomino_GainCtl,
			EDeviceParameterEuresysDomino_CalPat,
			EDeviceParameterEuresysDomino_InputLut,
			EDeviceParameterEuresysDomino_LutIndex,
			EDeviceParameterEuresysDomino_BoardLinkage,
			EDeviceParameterEuresysDomino_BoardIdentifier,
			EDeviceParameterEuresysDomino_BoardName,
			EDeviceParameterEuresysDomino_DriverIndex,
			EDeviceParameterEuresysDomino_PciPosition,
			EDeviceParameterEuresysDomino_Cluster,
			EDeviceParameterEuresysDomino_ColorFormat,
			EDeviceParameterEuresysDomino_ImageFlipX,
			EDeviceParameterEuresysDomino_ImageFlipY,
			EDeviceParameterEuresysDomino_ImageSizeX,
			EDeviceParameterEuresysDomino_SurfaceCount,
			EDeviceParameterEuresysDomino_SurfaceAllocation,
			EDeviceParameterEuresysDomino_ChannelManagement,
			EDeviceParameterEuresysDomino_ChannelState,
			EDeviceParameterEuresysDomino_ExceptionManagement,
			EDeviceParameterEuresysDomino_AcquisitionCleanup,
			EDeviceParameterEuresysDomino_AcqTimeout_ms,
			EDeviceParameterEuresysDomino_ParamsConsistencyCheck,
			EDeviceParameterEuresysDomino_Count,
		};

		enum EDeviceParameterEuresysGrablinkBase
		{
			EDeviceParameterEuresysGrablinkBase_Topology = EDeviceParameterMulticam_Count,
			EDeviceParameterEuresysGrablinkBase_Connector,
			EDeviceParameterEuresysGrablinkBase_Count,
		};

		enum EDeviceParameterEuresysGrablinkDualBase
		{
			EDeviceParameterEuresysGrablinkDualBase_Topology = EDeviceParameterMulticam_Count,
			EDeviceParameterEuresysGrablinkDualBase_Connector,
			EDeviceParameterEuresysGrablinkDualBase_Count,
		};

		enum EDeviceParameterEuresysGrablinkFull
		{
			EDeviceParameterEuresysGrablinkFull_Topology = EDeviceParameterMulticam_Count,
			EDeviceParameterEuresysGrablinkFull_Connector,
			EDeviceParameterEuresysGrablinkFull_Count,
		};

		enum EDeviceParameterEuresysGrablink
		{
			EDeviceParameterEuresysGrablink_SummaryControl = EDeviceParameterEuresysGrablinkFull_Count,
			EDeviceParameterEuresysGrablink_AcquisitionMode,
			EDeviceParameterEuresysGrablink_TrigMode,
			EDeviceParameterEuresysGrablink_Expose_us,
			EDeviceParameterEuresysGrablink_AreaCamera,
			EDeviceParameterEuresysGrablink_SeqLength_Fr,
			EDeviceParameterEuresysGrablink_PhaseLength_Fr,
			EDeviceParameterEuresysGrablink_LineCamera,
			EDeviceParameterEuresysGrablink_SeqLength_Ln,
			EDeviceParameterEuresysGrablink_SeqLength_Pg,
			EDeviceParameterEuresysGrablink_PageLength_Ln,
			EDeviceParameterEuresysGrablink_LineCaptureMode,
			EDeviceParameterEuresysGrablink_LineRateMode,
			EDeviceParameterEuresysGrablink_CameraSpecification,
			EDeviceParameterEuresysGrablink_CamConfig,
			EDeviceParameterEuresysGrablink_Camera,
			EDeviceParameterEuresysGrablink_DataLink,
			EDeviceParameterEuresysGrablink_Imaging,
			EDeviceParameterEuresysGrablink_Spectrum,
			EDeviceParameterEuresysGrablink_CameraTiming,
			EDeviceParameterEuresysGrablink_ExposeRecovery_us,
			EDeviceParameterEuresysGrablink_FrameRate_mHz,
			EDeviceParameterEuresysGrablink_Hactive_Px,
			EDeviceParameterEuresysGrablink_HsyncAft_Tk,
			EDeviceParameterEuresysGrablink_PixelClkMode,
			EDeviceParameterEuresysGrablink_ReadoutRecovery_us,
			EDeviceParameterEuresysGrablink_Vactive_Ln,
			EDeviceParameterEuresysGrablink_VsyncAft_Ln,
			EDeviceParameterEuresysGrablink_LineRate_Hz,
			EDeviceParameterEuresysGrablink_CameraFeatures,
			EDeviceParameterEuresysGrablink_TapConfiguration,
			EDeviceParameterEuresysGrablink_AuxResetCtl,
			EDeviceParameterEuresysGrablink_CC1USage,
			EDeviceParameterEuresysGrablink_CC2Usage,
			EDeviceParameterEuresysGrablink_CC3Usage,
			EDeviceParameterEuresysGrablink_CC4Usage,
			EDeviceParameterEuresysGrablink_ColorMethod,
			EDeviceParameterEuresysGrablink_ColorRegistrationControl,
			EDeviceParameterEuresysGrablink_DvalMode,
			EDeviceParameterEuresysGrablink_Expose,
			EDeviceParameterEuresysGrablink_ExposeMin_us,
			EDeviceParameterEuresysGrablink_ExposeMax_us,
			EDeviceParameterEuresysGrablink_ExposeOverlap,
			EDeviceParameterEuresysGrablink_FvalMode,
			EDeviceParameterEuresysGrablink_LvalMode,
			EDeviceParameterEuresysGrablink_Readout,
			EDeviceParameterEuresysGrablink_ResetCtl,
			EDeviceParameterEuresysGrablink_ResetEdge,
			EDeviceParameterEuresysGrablink_TwoLineSynchronization,
			EDeviceParameterEuresysGrablink_TapGeometry,
			EDeviceParameterEuresysGrablink_CableFeatures,
			EDeviceParameterEuresysGrablink_AuxresetLine,
			EDeviceParameterEuresysGrablink_ResetLine,
			EDeviceParameterEuresysGrablink_AcquisitionControl,
			EDeviceParameterEuresysGrablink_ActivityLength,
			EDeviceParameterEuresysGrablink_BreakEffect,
			EDeviceParameterEuresysGrablink_EndTrigMode,
			EDeviceParameterEuresysGrablink_NextTrigMode,
			EDeviceParameterEuresysGrablink_SynchronizedAcquisition,
			EDeviceParameterEuresysGrablink_TriggerControl,
			EDeviceParameterEuresysGrablink_ForceTrig,
			EDeviceParameterEuresysGrablink_NextTrigDelay_Pls,
			EDeviceParameterEuresysGrablink_TrigCtl,
			EDeviceParameterEuresysGrablink_TrigDelay_Pls,
			EDeviceParameterEuresysGrablink_TrigDelay_us,
			EDeviceParameterEuresysGrablink_TrigEdge,
			EDeviceParameterEuresysGrablink_TrigFilter,
			EDeviceParameterEuresysGrablink_TrigLine,
			EDeviceParameterEuresysGrablink_PageDelay_Ln,
			EDeviceParameterEuresysGrablink_ExposureControl,
			EDeviceParameterEuresysGrablink_ExposeTrim,
			EDeviceParameterEuresysGrablink_StrobeControl,
			EDeviceParameterEuresysGrablink_StrobeCtl,
			EDeviceParameterEuresysGrablink_StrobeDur,
			EDeviceParameterEuresysGrablink_StrobeMode,
			EDeviceParameterEuresysGrablink_StrobePos,
			EDeviceParameterEuresysGrablink_PreStrobe_us,
			EDeviceParameterEuresysGrablink_EncoderControl,
			EDeviceParameterEuresysGrablink_LineTrigCtl,
			EDeviceParameterEuresysGrablink_LineTrigEdge,
			EDeviceParameterEuresysGrablink_LineTrigFilter,
			EDeviceParameterEuresysGrablink_RateDivisionFactor,
			EDeviceParameterEuresysGrablink_LineTrigLine,
			EDeviceParameterEuresysGrablink_RateDividerRestart,
			EDeviceParameterEuresysGrablink_GrabberConfiguration,
			EDeviceParameterEuresysGrablink_ECCO_PLLResetControl,
			EDeviceParameterEuresysGrablink_ECCO_SkewCompensation,
			EDeviceParameterEuresysGrablink_FvalMin_Tk,
			EDeviceParameterEuresysGrablink_LvalMin_Tk,
			EDeviceParameterEuresysGrablink_MetadataInsertion,
			EDeviceParameterEuresysGrablink_GrabberTiming,
			EDeviceParameterEuresysGrablink_GrabWindow,
			EDeviceParameterEuresysGrablink_OffsetX_Px,
			EDeviceParameterEuresysGrablink_WindowX_Px,
			EDeviceParameterEuresysGrablink_Cluster,
			EDeviceParameterEuresysGrablink_ColorFormat,
			EDeviceParameterEuresysGrablink_ImageFlipX,
			EDeviceParameterEuresysGrablink_ImageFlipY,
			EDeviceParameterEuresysGrablink_RedBlueSwap,
			EDeviceParameterEuresysGrablink_MaxFillingSurfaces,
			EDeviceParameterEuresysGrablink_SurfaceCount,
			EDeviceParameterEuresysGrablink_ExceptionManagement,
			EDeviceParameterEuresysGrablink_AcquisitionCleanup,
			EDeviceParameterEuresysGrablink_AcqTimeout_ms,
			EDeviceParameterEuresysGrablink_ParamsConsistencyCheck,
			EDeviceParameterEuresysGrablink_TriggerSkipHold,
			EDeviceParameterEuresysGrablink_Signaling,
			EDeviceParameterEuresysGrablink_CallbackPriority,
			EDeviceParameterEuresysGrablink_InterleavedAcquisitionCategory,
			EDeviceParameterEuresysGrablink_InterleavedAcquisition,
			EDeviceParameterEuresysGrablink_ExposureTime_P1_us,
			EDeviceParameterEuresysGrablink_ExposureTime_P2_us,
			EDeviceParameterEuresysGrablink_StrobeDuration_P1_us,
			EDeviceParameterEuresysGrablink_StrobeDuration_P2_us,
			EDeviceParameterEuresysGrablink_StrobeDelay_P1_us,
			EDeviceParameterEuresysGrablink_StrobeDelay_P2_us,
			EDeviceParameterEuresysGrablink_ExposureDelay_MAN_P1_us,
			EDeviceParameterEuresysGrablink_ExposureDelay_MAN_P2_us,
			EDeviceParameterEuresysGrablink_Count,
		};



//////////////////////////////////////////////////////////////////////////
		//Solios

		enum EDeviceParameterMatrox
		{
			EDeviceParameterMatrox_DeviceID = 0,
			EDeviceParameterMatrox_SubUnitID,
			EDeviceParameterMatrox_CameraName,
			EDeviceParameterMatrox_InitializeType,
			EDeviceParameterMatrox_Camfilepath,
			EDeviceParameterMatrox_Parametermatch,
			EDeviceParameterMatrox_GrabCount,
			EDeviceParameterMatrox_MILImageCount,
			EDeviceParameterMatrox_MILBufferCount,

			EDeviceParameterMatrox_GrabSetting,
			EDeviceParameterMatrox_GrabDirectionX,
			EDeviceParameterMatrox_GrabDirectionY,
			EDeviceParameterMatrox_GrabScale,
			EDeviceParameterMatrox_GrabScaleX,
			EDeviceParameterMatrox_GrabScaleY,
			EDeviceParameterMatrox_SourceOffsetX,
			EDeviceParameterMatrox_SourceOffsetY,
			EDeviceParameterMatrox_SourceSizeX,
			EDeviceParameterMatrox_SourceSizeY,
			EDeviceParameterMatrox_GrabTriggerSetting,
			EDeviceParameterMatrox_GrabTriggerContinuouesEndTrigger,
			EDeviceParameterMatrox_GrabTriggerActivation,
			EDeviceParameterMatrox_GrabTriggerSoursce,
			EDeviceParameterMatrox_GrabTriggerState,
// 			EDeviceParameterMatrox_TimerSetting,
// 			EDeviceParameterMatrox_TimerSelector,
// 
// 			EDeviceParameterMatrox_Timer1,
// 			EDeviceParameterMatrox_Timer1ClockSource,
// 			EDeviceParameterMatrox_Timer1Delay,
// 			EDeviceParameterMatrox_Timer1Duration,
// 			EDeviceParameterMatrox_Timer1OutputInverter,
// 			EDeviceParameterMatrox_Timer1State,
// 			EDeviceParameterMatrox_Timer1TriggerActivation,
// 			EDeviceParameterMatrox_Timer1TriggerMissed,
// 			EDeviceParameterMatrox_Timer1TriggerRateDivider,
// 			EDeviceParameterMatrox_Timer1TriggerSource,
// 			EDeviceParameterMatrox_Timer2,
// 			EDeviceParameterMatrox_Timer2ClockSource,
// 			EDeviceParameterMatrox_Timer2Delay,
// 			EDeviceParameterMatrox_Timer2Duration,
// 			EDeviceParameterMatrox_Timer2OutputInverter,
// 			EDeviceParameterMatrox_Timer2State,
// 			EDeviceParameterMatrox_Timer2TriggerActivation,
// 			EDeviceParameterMatrox_Timer2TriggerMissed,
// 			EDeviceParameterMatrox_Timer2TriggerRateDivider,
// 			EDeviceParameterMatrox_Timer2TriggerSource,
// 			EDeviceParameterMatrox_Timer3,
// 			EDeviceParameterMatrox_Timer3ClockSource,
// 			EDeviceParameterMatrox_Timer3Delay,
// 			EDeviceParameterMatrox_Timer3Duration,
// 			EDeviceParameterMatrox_Timer3OutputInverter,
// 			EDeviceParameterMatrox_Timer3State,
// 			EDeviceParameterMatrox_Timer3TriggerActivation,
// 			EDeviceParameterMatrox_Timer3TriggerMissed,
// 			EDeviceParameterMatrox_Timer3TriggerRateDivider,
// 			EDeviceParameterMatrox_Timer3TriggerSource,
// 			EDeviceParameterMatrox_Timer4,
// 			EDeviceParameterMatrox_Timer4ClockSource,
// 			EDeviceParameterMatrox_Timer4Delay,
// 			EDeviceParameterMatrox_Timer4Duration,
// 			EDeviceParameterMatrox_Timer4OutputInverter,
// 			EDeviceParameterMatrox_Timer4State,
// 			EDeviceParameterMatrox_Timer4TriggerActivation,
// 			EDeviceParameterMatrox_Timer4TriggerMissed,
// 			EDeviceParameterMatrox_Timer4TriggerRateDivider,
// 			EDeviceParameterMatrox_Timer4TriggerSource,

EDeviceParameterMatrox_Camera,
EDeviceParameterMatrox_CameraSettings,
EDeviceParameterMatrox_CameraType,
EDeviceParameterMatrox_CameraNumberOfTaps,
EDeviceParameterMatrox_CameraBayerMode,
EDeviceParameterMatrox_CameraLinkConfiguration,
EDeviceParameterMatrox_ConfigurationType,
EDeviceParameterMatrox_CameraMode,
EDeviceParameterMatrox_TapConfiguration,
EDeviceParameterMatrox_Regions,
EDeviceParameterMatrox_RegionsX,
EDeviceParameterMatrox_RegionsY,
EDeviceParameterMatrox_AdjacentPixels,
EDeviceParameterMatrox_AdjacentX,
EDeviceParameterMatrox_AdjacentY,
EDeviceParameterMatrox_TapsDirection,
EDeviceParameterMatrox_Tap1,
EDeviceParameterMatrox_Tap2,
EDeviceParameterMatrox_Tap3,
EDeviceParameterMatrox_Tap4,
EDeviceParameterMatrox_Tap5,
EDeviceParameterMatrox_Tap6,
EDeviceParameterMatrox_Tap7,
EDeviceParameterMatrox_Tap8,
EDeviceParameterMatrox_VideoSignal,
EDeviceParameterMatrox_VideoSignalInformation,
EDeviceParameterMatrox_VideoSignalType,
EDeviceParameterMatrox_VideoSignalDatabusWidth,
EDeviceParameterMatrox_VideoSignalStandard,
EDeviceParameterMatrox_VideoSignalMILChannelInput,
EDeviceParameterMatrox_DigitalVideoSignalInformation,
EDeviceParameterMatrox_DigitalVideoSignalFormat,
EDeviceParameterMatrox_VideoTiming,
EDeviceParameterMatrox_ValidSignalSource,
EDeviceParameterMatrox_UseFrameGrabber,
EDeviceParameterMatrox_Timings,
EDeviceParameterMatrox_TimingsVertical,
EDeviceParameterMatrox_PixelClockFrqAutoAdjust,
EDeviceParameterMatrox_Horizontal,
EDeviceParameterMatrox_HorizontalSyncClk,
EDeviceParameterMatrox_HorizontalBPorchClk,
EDeviceParameterMatrox_HorizontalActiveClk,
EDeviceParameterMatrox_HorizontalFPorchClk,
EDeviceParameterMatrox_HorizontalFrequencyHz,
EDeviceParameterMatrox_HorizontalLockActiveAndTotal,
EDeviceParameterMatrox_Vertical,
EDeviceParameterMatrox_VerticalSyncLine,
EDeviceParameterMatrox_VerticalBPorchLine,
EDeviceParameterMatrox_VerticalActiveLine,
EDeviceParameterMatrox_VerticalFPorchLine,
EDeviceParameterMatrox_VerticalFrequencyHz,
EDeviceParameterMatrox_VerticalLockActiveAndTotal,
EDeviceParameterMatrox_UseCamera,
EDeviceParameterMatrox_CameralLinkSettings,
EDeviceParameterMatrox_ImagesizeX,
EDeviceParameterMatrox_ImagesizeY,
EDeviceParameterMatrox_DelayX,
EDeviceParameterMatrox_DelayY,
EDeviceParameterMatrox_PixelClock,
EDeviceParameterMatrox_PixelClockFrequency,
EDeviceParameterMatrox_PixelClockFrequencyMHz,
EDeviceParameterMatrox_AutoAdjustVideoTimingsPage,
EDeviceParameterMatrox_IOCharacteristics,
EDeviceParameterMatrox_SendToExternalCircuit,
EDeviceParameterMatrox_ExternalClockSignal,
EDeviceParameterMatrox_EXTInput,
EDeviceParameterMatrox_EXTInputFrequency,
EDeviceParameterMatrox_EXTInputFormat,
EDeviceParameterMatrox_EXTInputPolarity,
EDeviceParameterMatrox_EXTOutput,
EDeviceParameterMatrox_EXTOutputFrequency,
EDeviceParameterMatrox_EXTOutputFormat,
EDeviceParameterMatrox_EXTOutputPolarity,
EDeviceParameterMatrox_EXTIODelay,
EDeviceParameterMatrox_ExposureSignal,
EDeviceParameterMatrox_ExpTimer1,
EDeviceParameterMatrox_Timer1Generation,
EDeviceParameterMatrox_Timer1Mode,
EDeviceParameterMatrox_Timer1ExpSignal,
EDeviceParameterMatrox_Timer1ExpFormat,
EDeviceParameterMatrox_Timer1ExpPolarity,
EDeviceParameterMatrox_Timer1TriggerInfo,
EDeviceParameterMatrox_Timer1TrgFormat,
EDeviceParameterMatrox_Timer1TrgSignal,
EDeviceParameterMatrox_Timer1TrgPolarity,
EDeviceParameterMatrox_Timer1Pulse,
EDeviceParameterMatrox_Timer1Pulse1Clk,
EDeviceParameterMatrox_Timer1Delay1Clk,
EDeviceParameterMatrox_Timer1Pulse2Clk,
EDeviceParameterMatrox_Timer1Delay2Clk,
EDeviceParameterMatrox_Timer1ExposureClock,
EDeviceParameterMatrox_Timer1Type,
EDeviceParameterMatrox_Timer1DivisionFactor,
EDeviceParameterMatrox_Timer1BaseClockFrequency,
EDeviceParameterMatrox_ExpTimer2,
EDeviceParameterMatrox_Timer2Generation,
EDeviceParameterMatrox_Timer2Mode,
EDeviceParameterMatrox_Timer2ExpSignal,
EDeviceParameterMatrox_Timer2ExpFormat,
EDeviceParameterMatrox_Timer2ExpPolarity,
EDeviceParameterMatrox_Timer2TriggerInfo,
EDeviceParameterMatrox_Timer2TrgFormat,
EDeviceParameterMatrox_Timer2TrgSignal,
EDeviceParameterMatrox_Timer2TrgPolarity,
EDeviceParameterMatrox_Timer2Pulse,
EDeviceParameterMatrox_Timer2Pulse1Clk,
EDeviceParameterMatrox_Timer2Delay1Clk,
EDeviceParameterMatrox_Timer2Pulse2Clk,
EDeviceParameterMatrox_Timer2Delay2Clk,
EDeviceParameterMatrox_Timer2ExposureClock,
EDeviceParameterMatrox_Timer2Type,
EDeviceParameterMatrox_Timer2DivisionFactor,
EDeviceParameterMatrox_Timer2BaseClockFrequency,
EDeviceParameterMatrox_ExpTimer1Advanced,
EDeviceParameterMatrox_Timer1Options,
EDeviceParameterMatrox_Timer1TimerOutputSignal,
EDeviceParameterMatrox_Timer1CaptureTrigger,
EDeviceParameterMatrox_Timer1TriggerArm,
EDeviceParameterMatrox_Timer1TrgArmMode,
EDeviceParameterMatrox_Timer1TrgArmFormat,
EDeviceParameterMatrox_Timer1TrgArmSource,
EDeviceParameterMatrox_Timer1TrgArmPolarity,
EDeviceParameterMatrox_ExpTimer2Advanced,
EDeviceParameterMatrox_Timer2Options,
EDeviceParameterMatrox_Timer2TimerOutputSignal,
EDeviceParameterMatrox_Timer2CaptureTrigger,
EDeviceParameterMatrox_Timer2TriggerArm,
EDeviceParameterMatrox_Timer2TrgArmMode,
EDeviceParameterMatrox_Timer2TrgArmFormat,
EDeviceParameterMatrox_Timer2TrgArmSource,
EDeviceParameterMatrox_Timer2TrgArmPolarity,
EDeviceParameterMatrox_GrabMode,
EDeviceParameterMatrox_GrabCharacteristics,
EDeviceParameterMatrox_GrabCharacteristicsMode,
EDeviceParameterMatrox_GrabActivationMode,
EDeviceParameterMatrox_TriggerSignalCharacteristics,
EDeviceParameterMatrox_GrabTrgFormat,
EDeviceParameterMatrox_GrabTrgSignal,
EDeviceParameterMatrox_GrabTrgPolarity,
EDeviceParameterMatrox_TriggerArmCharacteristics,
EDeviceParameterMatrox_GrabTrgArmFormat,
EDeviceParameterMatrox_GrabTrgArmSignal,
EDeviceParameterMatrox_GrabTrgArmPolarity,
EDeviceParameterMatrox_SyncSignal,
EDeviceParameterMatrox_SourceSpecification,
EDeviceParameterMatrox_SpcSource,
EDeviceParameterMatrox_SpcFormat,
EDeviceParameterMatrox_SpcCameraOutInSignalLatency,
EDeviceParameterMatrox_SpcSyncSignalAvailable,
EDeviceParameterMatrox_SpcBlockSyncTypeEnable,
EDeviceParameterMatrox_SpcExternalVSYNCEnable,
EDeviceParameterMatrox_DigitalSynchro,
EDeviceParameterMatrox_HSync,
EDeviceParameterMatrox_HSyncInputActive,
EDeviceParameterMatrox_HSyncInputFormat,
EDeviceParameterMatrox_HSyncInputPolarity,
EDeviceParameterMatrox_HSyncOutputActive,
EDeviceParameterMatrox_HSyncOutputFormat,
EDeviceParameterMatrox_HSyncOutputPolarity,
EDeviceParameterMatrox_VSync,
EDeviceParameterMatrox_VSyncInputActive,
EDeviceParameterMatrox_VSyncInputFormat,
EDeviceParameterMatrox_VSyncInputPolarity,
EDeviceParameterMatrox_VSyncOutputActive,
EDeviceParameterMatrox_VSyncOutputFormat,
EDeviceParameterMatrox_VSyncOutputPolarity,
EDeviceParameterMatrox_CSync,
EDeviceParameterMatrox_CSyncInputActive,
EDeviceParameterMatrox_CSyncInputFormat,
EDeviceParameterMatrox_CSyncInputPolarity,
EDeviceParameterMatrox_CSyncOutputActive,
EDeviceParameterMatrox_CSyncOutputFormat,
EDeviceParameterMatrox_CSyncOutputPolarity,
EDeviceParameterMatrox_CSyncSerrationPulse,
EDeviceParameterMatrox_CSyncEqualizationPulse,
EDeviceParameterMatrox_Other,
EDeviceParameterMatrox_CameraLinkControlBits,
EDeviceParameterMatrox_CameraControlBitsSources,
EDeviceParameterMatrox_Control1,
EDeviceParameterMatrox_Control2,
EDeviceParameterMatrox_Control3,
EDeviceParameterMatrox_Control4,
EDeviceParameterMatrox_SignalsOutput,
EDeviceParameterMatrox_EnableCCoutputsConnector1,
EDeviceParameterMatrox_EnableCCoutputsConnector2,
EDeviceParameterMatrox_Count,
		};

//////////////////////////////////////////////////////////////////////////

		enum EDeviceParameterAdvantechDio
		{
			EDeviceParameterAdvantechDio_DeviceID = 0,
			EDeviceParameterAdvantechDio_InputStatus,
			EDeviceParameterAdvantechDio_UseInputNo0,
			EDeviceParameterAdvantechDio_UseInputNo1,
			EDeviceParameterAdvantechDio_UseInputNo2,
			EDeviceParameterAdvantechDio_UseInputNo3,
			EDeviceParameterAdvantechDio_UseInputNo4,
			EDeviceParameterAdvantechDio_UseInputNo5,
			EDeviceParameterAdvantechDio_UseInputNo6,
			EDeviceParameterAdvantechDio_UseInputNo7,
			EDeviceParameterAdvantechDio_UseInputNo8,
			EDeviceParameterAdvantechDio_UseInputNo9,
			EDeviceParameterAdvantechDio_UseInputNo10,
			EDeviceParameterAdvantechDio_UseInputNo11,
			EDeviceParameterAdvantechDio_UseInputNo12,
			EDeviceParameterAdvantechDio_UseInputNo13,
			EDeviceParameterAdvantechDio_UseInputNo14,
			EDeviceParameterAdvantechDio_UseInputNo15,
			EDeviceParameterAdvantechDio_OutputStatus,
			EDeviceParameterAdvantechDio_UseOutputNo0,
			EDeviceParameterAdvantechDio_UseOutputNo1,
			EDeviceParameterAdvantechDio_UseOutputNo2,
			EDeviceParameterAdvantechDio_UseOutputNo3,
			EDeviceParameterAdvantechDio_UseOutputNo4,
			EDeviceParameterAdvantechDio_UseOutputNo5,
			EDeviceParameterAdvantechDio_UseOutputNo6,
			EDeviceParameterAdvantechDio_UseOutputNo7,
			EDeviceParameterAdvantechDio_UseOutputNo8,
			EDeviceParameterAdvantechDio_UseOutputNo9,
			EDeviceParameterAdvantechDio_UseOutputNo10,
			EDeviceParameterAdvantechDio_UseOutputNo11,
			EDeviceParameterAdvantechDio_UseOutputNo12,
			EDeviceParameterAdvantechDio_UseOutputNo13,
			EDeviceParameterAdvantechDio_UseOutputNo14,
			EDeviceParameterAdvantechDio_UseOutputNo15,
			EDeviceParameterAdvantechDio_InputName,
			EDeviceParameterAdvantechDio_InputNo0Name,
			EDeviceParameterAdvantechDio_InputNo1Name,
			EDeviceParameterAdvantechDio_InputNo2Name,
			EDeviceParameterAdvantechDio_InputNo3Name,
			EDeviceParameterAdvantechDio_InputNo4Name,
			EDeviceParameterAdvantechDio_InputNo5Name,
			EDeviceParameterAdvantechDio_InputNo6Name,
			EDeviceParameterAdvantechDio_InputNo7Name,
			EDeviceParameterAdvantechDio_InputNo8Name,
			EDeviceParameterAdvantechDio_InputNo9Name,
			EDeviceParameterAdvantechDio_InputNo10Name,
			EDeviceParameterAdvantechDio_InputNo11Name,
			EDeviceParameterAdvantechDio_InputNo12Name,
			EDeviceParameterAdvantechDio_InputNo13Name,
			EDeviceParameterAdvantechDio_InputNo14Name,
			EDeviceParameterAdvantechDio_InputNo15Name,
			EDeviceParameterAdvantechDio_OutputName,
			EDeviceParameterAdvantechDio_OutputNo0Name,
			EDeviceParameterAdvantechDio_OutputNo1Name,
			EDeviceParameterAdvantechDio_OutputNo2Name,
			EDeviceParameterAdvantechDio_OutputNo3Name,
			EDeviceParameterAdvantechDio_OutputNo4Name,
			EDeviceParameterAdvantechDio_OutputNo5Name,
			EDeviceParameterAdvantechDio_OutputNo6Name,
			EDeviceParameterAdvantechDio_OutputNo7Name,
			EDeviceParameterAdvantechDio_OutputNo8Name,
			EDeviceParameterAdvantechDio_OutputNo9Name,
			EDeviceParameterAdvantechDio_OutputNo10Name,
			EDeviceParameterAdvantechDio_OutputNo11Name,
			EDeviceParameterAdvantechDio_OutputNo12Name,
			EDeviceParameterAdvantechDio_OutputNo13Name,
			EDeviceParameterAdvantechDio_OutputNo14Name,
			EDeviceParameterAdvantechDio_OutputNo15Name,
			EDeviceParameterAdvantechDio_Count,
		};

		enum EDeviceParameterAxlDio
		{
			EDeviceParameterAxlDio_DeviceID,
			EDeviceParameterAxlDio_InputStatus,
			EDeviceParameterAxlDio_UseInputNo0,
			EDeviceParameterAxlDio_UseInputNo1,
			EDeviceParameterAxlDio_UseInputNo2,
			EDeviceParameterAxlDio_UseInputNo3,
			EDeviceParameterAxlDio_UseInputNo4,
			EDeviceParameterAxlDio_UseInputNo5,
			EDeviceParameterAxlDio_UseInputNo6,
			EDeviceParameterAxlDio_UseInputNo7,
			EDeviceParameterAxlDio_UseInputNo8,
			EDeviceParameterAxlDio_UseInputNo9,
			EDeviceParameterAxlDio_UseInputNo10,
			EDeviceParameterAxlDio_UseInputNo11,
			EDeviceParameterAxlDio_UseInputNo12,
			EDeviceParameterAxlDio_UseInputNo13,
			EDeviceParameterAxlDio_UseInputNo14,
			EDeviceParameterAxlDio_UseInputNo15,
			EDeviceParameterAxlDio_UseInputNo16,
			EDeviceParameterAxlDio_UseInputNo17,
			EDeviceParameterAxlDio_UseInputNo18,
			EDeviceParameterAxlDio_UseInputNo19,
			EDeviceParameterAxlDio_UseInputNo20,
			EDeviceParameterAxlDio_UseInputNo21,
			EDeviceParameterAxlDio_UseInputNo22,
			EDeviceParameterAxlDio_UseInputNo23,
			EDeviceParameterAxlDio_UseInputNo24,
			EDeviceParameterAxlDio_UseInputNo25,
			EDeviceParameterAxlDio_UseInputNo26,
			EDeviceParameterAxlDio_UseInputNo27,
			EDeviceParameterAxlDio_UseInputNo28,
			EDeviceParameterAxlDio_UseInputNo29,
			EDeviceParameterAxlDio_UseInputNo30,
			EDeviceParameterAxlDio_UseInputNo31,
			EDeviceParameterAxlDio_OutputStatus,
			EDeviceParameterAxlDio_UseOutputNo0,
			EDeviceParameterAxlDio_UseOutputNo1,
			EDeviceParameterAxlDio_UseOutputNo2,
			EDeviceParameterAxlDio_UseOutputNo3,
			EDeviceParameterAxlDio_UseOutputNo4,
			EDeviceParameterAxlDio_UseOutputNo5,
			EDeviceParameterAxlDio_UseOutputNo6,
			EDeviceParameterAxlDio_UseOutputNo7,
			EDeviceParameterAxlDio_UseOutputNo8,
			EDeviceParameterAxlDio_UseOutputNo9,
			EDeviceParameterAxlDio_UseOutputNo10,
			EDeviceParameterAxlDio_UseOutputNo11,
			EDeviceParameterAxlDio_UseOutputNo12,
			EDeviceParameterAxlDio_UseOutputNo13,
			EDeviceParameterAxlDio_UseOutputNo14,
			EDeviceParameterAxlDio_UseOutputNo15,
			EDeviceParameterAxlDio_UseOutputNo16,
			EDeviceParameterAxlDio_UseOutputNo17,
			EDeviceParameterAxlDio_UseOutputNo18,
			EDeviceParameterAxlDio_UseOutputNo19,
			EDeviceParameterAxlDio_UseOutputNo20,
			EDeviceParameterAxlDio_UseOutputNo21,
			EDeviceParameterAxlDio_UseOutputNo22,
			EDeviceParameterAxlDio_UseOutputNo23,
			EDeviceParameterAxlDio_UseOutputNo24,
			EDeviceParameterAxlDio_UseOutputNo25,
			EDeviceParameterAxlDio_UseOutputNo26,
			EDeviceParameterAxlDio_UseOutputNo27,
			EDeviceParameterAxlDio_UseOutputNo28,
			EDeviceParameterAxlDio_UseOutputNo29,
			EDeviceParameterAxlDio_UseOutputNo30,
			EDeviceParameterAxlDio_UseOutputNo31,
			EDeviceParameterAxlDio_InputName,
			EDeviceParameterAxlDio_InputNo0Name,
			EDeviceParameterAxlDio_InputNo1Name,
			EDeviceParameterAxlDio_InputNo2Name,
			EDeviceParameterAxlDio_InputNo3Name,
			EDeviceParameterAxlDio_InputNo4Name,
			EDeviceParameterAxlDio_InputNo5Name,
			EDeviceParameterAxlDio_InputNo6Name,
			EDeviceParameterAxlDio_InputNo7Name,
			EDeviceParameterAxlDio_InputNo8Name,
			EDeviceParameterAxlDio_InputNo9Name,
			EDeviceParameterAxlDio_InputNo10Name,
			EDeviceParameterAxlDio_InputNo11Name,
			EDeviceParameterAxlDio_InputNo12Name,
			EDeviceParameterAxlDio_InputNo13Name,
			EDeviceParameterAxlDio_InputNo14Name,
			EDeviceParameterAxlDio_InputNo15Name,
			EDeviceParameterAxlDio_InputNo16Name,
			EDeviceParameterAxlDio_InputNo17Name,
			EDeviceParameterAxlDio_InputNo18Name,
			EDeviceParameterAxlDio_InputNo19Name,
			EDeviceParameterAxlDio_InputNo20Name,
			EDeviceParameterAxlDio_InputNo21Name,
			EDeviceParameterAxlDio_InputNo22Name,
			EDeviceParameterAxlDio_InputNo23Name,
			EDeviceParameterAxlDio_InputNo24Name,
			EDeviceParameterAxlDio_InputNo25Name,
			EDeviceParameterAxlDio_InputNo26Name,
			EDeviceParameterAxlDio_InputNo27Name,
			EDeviceParameterAxlDio_InputNo28Name,
			EDeviceParameterAxlDio_InputNo29Name,
			EDeviceParameterAxlDio_InputNo30Name,
			EDeviceParameterAxlDio_InputNo31Name,
			EDeviceParameterAxlDio_OutputName,
			EDeviceParameterAxlDio_OutputNo0Name,
			EDeviceParameterAxlDio_OutputNo1Name,
			EDeviceParameterAxlDio_OutputNo2Name,
			EDeviceParameterAxlDio_OutputNo3Name,
			EDeviceParameterAxlDio_OutputNo4Name,
			EDeviceParameterAxlDio_OutputNo5Name,
			EDeviceParameterAxlDio_OutputNo6Name,
			EDeviceParameterAxlDio_OutputNo7Name,
			EDeviceParameterAxlDio_OutputNo8Name,
			EDeviceParameterAxlDio_OutputNo9Name,
			EDeviceParameterAxlDio_OutputNo10Name,
			EDeviceParameterAxlDio_OutputNo11Name,
			EDeviceParameterAxlDio_OutputNo12Name,
			EDeviceParameterAxlDio_OutputNo13Name,
			EDeviceParameterAxlDio_OutputNo14Name,
			EDeviceParameterAxlDio_OutputNo15Name,
			EDeviceParameterAxlDio_OutputNo16Name,
			EDeviceParameterAxlDio_OutputNo17Name,
			EDeviceParameterAxlDio_OutputNo18Name,
			EDeviceParameterAxlDio_OutputNo19Name,
			EDeviceParameterAxlDio_OutputNo20Name,
			EDeviceParameterAxlDio_OutputNo21Name,
			EDeviceParameterAxlDio_OutputNo22Name,
			EDeviceParameterAxlDio_OutputNo23Name,
			EDeviceParameterAxlDio_OutputNo24Name,
			EDeviceParameterAxlDio_OutputNo25Name,
			EDeviceParameterAxlDio_OutputNo26Name,
			EDeviceParameterAxlDio_OutputNo27Name,
			EDeviceParameterAxlDio_OutputNo28Name,
			EDeviceParameterAxlDio_OutputNo29Name,
			EDeviceParameterAxlDio_OutputNo30Name,
			EDeviceParameterAxlDio_OutputNo31Name,
			EDeviceParameterAxlDio_UseClearInitialize,
			EDeviceParameterAxlDio_UseClearTerminate,
			EDeviceParameterAxlDio_OpenNoReset,
			EDeviceParameterAxlDio_Count,
		};

		enum EDeviceParameterAxtDio
		{
			EDeviceParameterAxtDio_DeviceID,
			EDeviceParameterAxtDio_SubUnitID,
			EDeviceParameterAxtDio_InputStatus,
			EDeviceParameterAxtDio_UseInputNo0,
			EDeviceParameterAxtDio_UseInputNo1,
			EDeviceParameterAxtDio_UseInputNo2,
			EDeviceParameterAxtDio_UseInputNo3,
			EDeviceParameterAxtDio_UseInputNo4,
			EDeviceParameterAxtDio_UseInputNo5,
			EDeviceParameterAxtDio_UseInputNo6,
			EDeviceParameterAxtDio_UseInputNo7,
			EDeviceParameterAxtDio_UseInputNo8,
			EDeviceParameterAxtDio_UseInputNo9,
			EDeviceParameterAxtDio_UseInputNo10,
			EDeviceParameterAxtDio_UseInputNo11,
			EDeviceParameterAxtDio_UseInputNo12,
			EDeviceParameterAxtDio_UseInputNo13,
			EDeviceParameterAxtDio_UseInputNo14,
			EDeviceParameterAxtDio_UseInputNo15,
			EDeviceParameterAxtDio_OutputStatus,
			EDeviceParameterAxtDio_UseOutputNo0,
			EDeviceParameterAxtDio_UseOutputNo1,
			EDeviceParameterAxtDio_UseOutputNo2,
			EDeviceParameterAxtDio_UseOutputNo3,
			EDeviceParameterAxtDio_UseOutputNo4,
			EDeviceParameterAxtDio_UseOutputNo5,
			EDeviceParameterAxtDio_UseOutputNo6,
			EDeviceParameterAxtDio_UseOutputNo7,
			EDeviceParameterAxtDio_UseOutputNo8,
			EDeviceParameterAxtDio_UseOutputNo9,
			EDeviceParameterAxtDio_UseOutputNo10,
			EDeviceParameterAxtDio_UseOutputNo11,
			EDeviceParameterAxtDio_UseOutputNo12,
			EDeviceParameterAxtDio_UseOutputNo13,
			EDeviceParameterAxtDio_UseOutputNo14,
			EDeviceParameterAxtDio_UseOutputNo15,
			EDeviceParameterAxtDio_InputName,
			EDeviceParameterAxtDio_InputNo0Name,
			EDeviceParameterAxtDio_InputNo1Name,
			EDeviceParameterAxtDio_InputNo2Name,
			EDeviceParameterAxtDio_InputNo3Name,
			EDeviceParameterAxtDio_InputNo4Name,
			EDeviceParameterAxtDio_InputNo5Name,
			EDeviceParameterAxtDio_InputNo6Name,
			EDeviceParameterAxtDio_InputNo7Name,
			EDeviceParameterAxtDio_InputNo8Name,
			EDeviceParameterAxtDio_InputNo9Name,
			EDeviceParameterAxtDio_InputNo10Name,
			EDeviceParameterAxtDio_InputNo11Name,
			EDeviceParameterAxtDio_InputNo12Name,
			EDeviceParameterAxtDio_InputNo13Name,
			EDeviceParameterAxtDio_InputNo14Name,
			EDeviceParameterAxtDio_InputNo15Name,
			EDeviceParameterAxtDio_OutputName,
			EDeviceParameterAxtDio_OutputNo0Name,
			EDeviceParameterAxtDio_OutputNo1Name,
			EDeviceParameterAxtDio_OutputNo2Name,
			EDeviceParameterAxtDio_OutputNo3Name,
			EDeviceParameterAxtDio_OutputNo4Name,
			EDeviceParameterAxtDio_OutputNo5Name,
			EDeviceParameterAxtDio_OutputNo6Name,
			EDeviceParameterAxtDio_OutputNo7Name,
			EDeviceParameterAxtDio_OutputNo8Name,
			EDeviceParameterAxtDio_OutputNo9Name,
			EDeviceParameterAxtDio_OutputNo10Name,
			EDeviceParameterAxtDio_OutputNo11Name,
			EDeviceParameterAxtDio_OutputNo12Name,
			EDeviceParameterAxtDio_OutputNo13Name,
			EDeviceParameterAxtDio_OutputNo14Name,
			EDeviceParameterAxtDio_OutputNo15Name,
			EDeviceParameterAxtDio_Count,
		};

		enum EDeviceParameterMVTechDio
		{
			EDeviceParameterMVTechDio_DeviceID = 0,
			EDeviceParameterMVTechDio_InputStatus,
			EDeviceParameterMVTechDio_UseInputNo0,
			EDeviceParameterMVTechDio_UseInputNo1,
			EDeviceParameterMVTechDio_UseInputNo2,
			EDeviceParameterMVTechDio_UseInputNo3,
			EDeviceParameterMVTechDio_UseInputNo4,
			EDeviceParameterMVTechDio_UseInputNo5,
			EDeviceParameterMVTechDio_UseInputNo6,
			EDeviceParameterMVTechDio_UseInputNo7,
			EDeviceParameterMVTechDio_UseInputNo8,
			EDeviceParameterMVTechDio_UseInputNo9,
			EDeviceParameterMVTechDio_UseInputNo10,
			EDeviceParameterMVTechDio_UseInputNo11,
			EDeviceParameterMVTechDio_UseInputNo12,
			EDeviceParameterMVTechDio_UseInputNo13,
			EDeviceParameterMVTechDio_UseInputNo14,
			EDeviceParameterMVTechDio_UseInputNo15,
			EDeviceParameterMVTechDio_UseInputNo16,
			EDeviceParameterMVTechDio_UseInputNo17,
			EDeviceParameterMVTechDio_UseInputNo18,
			EDeviceParameterMVTechDio_UseInputNo19,
			EDeviceParameterMVTechDio_UseInputNo20,
			EDeviceParameterMVTechDio_UseInputNo21,
			EDeviceParameterMVTechDio_UseInputNo22,
			EDeviceParameterMVTechDio_UseInputNo23,
			EDeviceParameterMVTechDio_UseInputNo24,
			EDeviceParameterMVTechDio_UseInputNo25,
			EDeviceParameterMVTechDio_UseInputNo26,
			EDeviceParameterMVTechDio_UseInputNo27,
			EDeviceParameterMVTechDio_UseInputNo28,
			EDeviceParameterMVTechDio_UseInputNo29,
			EDeviceParameterMVTechDio_UseInputNo30,
			EDeviceParameterMVTechDio_UseInputNo31,
			EDeviceParameterMVTechDio_OutputStatus,
			EDeviceParameterMVTechDio_UseOutputNo0,
			EDeviceParameterMVTechDio_UseOutputNo1,
			EDeviceParameterMVTechDio_UseOutputNo2,
			EDeviceParameterMVTechDio_UseOutputNo3,
			EDeviceParameterMVTechDio_UseOutputNo4,
			EDeviceParameterMVTechDio_UseOutputNo5,
			EDeviceParameterMVTechDio_UseOutputNo6,
			EDeviceParameterMVTechDio_UseOutputNo7,
			EDeviceParameterMVTechDio_UseOutputNo8,
			EDeviceParameterMVTechDio_UseOutputNo9,
			EDeviceParameterMVTechDio_UseOutputNo10,
			EDeviceParameterMVTechDio_UseOutputNo11,
			EDeviceParameterMVTechDio_UseOutputNo12,
			EDeviceParameterMVTechDio_UseOutputNo13,
			EDeviceParameterMVTechDio_UseOutputNo14,
			EDeviceParameterMVTechDio_UseOutputNo15,
			EDeviceParameterMVTechDio_UseOutputNo16,
			EDeviceParameterMVTechDio_UseOutputNo17,
			EDeviceParameterMVTechDio_UseOutputNo18,
			EDeviceParameterMVTechDio_UseOutputNo19,
			EDeviceParameterMVTechDio_UseOutputNo20,
			EDeviceParameterMVTechDio_UseOutputNo21,
			EDeviceParameterMVTechDio_UseOutputNo22,
			EDeviceParameterMVTechDio_UseOutputNo23,
			EDeviceParameterMVTechDio_UseOutputNo24,
			EDeviceParameterMVTechDio_UseOutputNo25,
			EDeviceParameterMVTechDio_UseOutputNo26,
			EDeviceParameterMVTechDio_UseOutputNo27,
			EDeviceParameterMVTechDio_UseOutputNo28,
			EDeviceParameterMVTechDio_UseOutputNo29,
			EDeviceParameterMVTechDio_UseOutputNo30,
			EDeviceParameterMVTechDio_UseOutputNo31,
			EDeviceParameterMVTechDio_InputName,
			EDeviceParameterMVTechDio_InputNo0Name,
			EDeviceParameterMVTechDio_InputNo1Name,
			EDeviceParameterMVTechDio_InputNo2Name,
			EDeviceParameterMVTechDio_InputNo3Name,
			EDeviceParameterMVTechDio_InputNo4Name,
			EDeviceParameterMVTechDio_InputNo5Name,
			EDeviceParameterMVTechDio_InputNo6Name,
			EDeviceParameterMVTechDio_InputNo7Name,
			EDeviceParameterMVTechDio_InputNo8Name,
			EDeviceParameterMVTechDio_InputNo9Name,
			EDeviceParameterMVTechDio_InputNo10Name,
			EDeviceParameterMVTechDio_InputNo11Name,
			EDeviceParameterMVTechDio_InputNo12Name,
			EDeviceParameterMVTechDio_InputNo13Name,
			EDeviceParameterMVTechDio_InputNo14Name,
			EDeviceParameterMVTechDio_InputNo15Name,
			EDeviceParameterMVTechDio_InputNo16Name,
			EDeviceParameterMVTechDio_InputNo17Name,
			EDeviceParameterMVTechDio_InputNo18Name,
			EDeviceParameterMVTechDio_InputNo19Name,
			EDeviceParameterMVTechDio_InputNo20Name,
			EDeviceParameterMVTechDio_InputNo21Name,
			EDeviceParameterMVTechDio_InputNo22Name,
			EDeviceParameterMVTechDio_InputNo23Name,
			EDeviceParameterMVTechDio_InputNo24Name,
			EDeviceParameterMVTechDio_InputNo25Name,
			EDeviceParameterMVTechDio_InputNo26Name,
			EDeviceParameterMVTechDio_InputNo27Name,
			EDeviceParameterMVTechDio_InputNo28Name,
			EDeviceParameterMVTechDio_InputNo29Name,
			EDeviceParameterMVTechDio_InputNo30Name,
			EDeviceParameterMVTechDio_InputNo31Name,
			EDeviceParameterMVTechDio_OutputName,
			EDeviceParameterMVTechDio_OutputNo0Name,
			EDeviceParameterMVTechDio_OutputNo1Name,
			EDeviceParameterMVTechDio_OutputNo2Name,
			EDeviceParameterMVTechDio_OutputNo3Name,
			EDeviceParameterMVTechDio_OutputNo4Name,
			EDeviceParameterMVTechDio_OutputNo5Name,
			EDeviceParameterMVTechDio_OutputNo6Name,
			EDeviceParameterMVTechDio_OutputNo7Name,
			EDeviceParameterMVTechDio_OutputNo8Name,
			EDeviceParameterMVTechDio_OutputNo9Name,
			EDeviceParameterMVTechDio_OutputNo10Name,
			EDeviceParameterMVTechDio_OutputNo11Name,
			EDeviceParameterMVTechDio_OutputNo12Name,
			EDeviceParameterMVTechDio_OutputNo13Name,
			EDeviceParameterMVTechDio_OutputNo14Name,
			EDeviceParameterMVTechDio_OutputNo15Name,
			EDeviceParameterMVTechDio_OutputNo16Name,
			EDeviceParameterMVTechDio_OutputNo17Name,
			EDeviceParameterMVTechDio_OutputNo18Name,
			EDeviceParameterMVTechDio_OutputNo19Name,
			EDeviceParameterMVTechDio_OutputNo20Name,
			EDeviceParameterMVTechDio_OutputNo21Name,
			EDeviceParameterMVTechDio_OutputNo22Name,
			EDeviceParameterMVTechDio_OutputNo23Name,
			EDeviceParameterMVTechDio_OutputNo24Name,
			EDeviceParameterMVTechDio_OutputNo25Name,
			EDeviceParameterMVTechDio_OutputNo26Name,
			EDeviceParameterMVTechDio_OutputNo27Name,
			EDeviceParameterMVTechDio_OutputNo28Name,
			EDeviceParameterMVTechDio_OutputNo29Name,
			EDeviceParameterMVTechDio_OutputNo30Name,
			EDeviceParameterMVTechDio_OutputNo31Name,
			EDeviceParameterMVTechDio_UseClearInitialize,
			EDeviceParameterMVTechDio_UseClearTerminate,
			EDeviceParameterMVTechDio_CheckPortFromBoard,
			EDeviceParameterMVTechDio_Count,
		};

		enum EDeviceParameterAxtMotionFS10
		{
			EDeviceParameterAxtMotionFS10_DeviceID = 0,
			EDeviceParameterAxtMotionFS10_SubUnitID,
			EDeviceParameterAxtMotionFS10_UnitPulse,
			EDeviceParameterAxtMotionFS10_StartSpeed,
			EDeviceParameterAxtMotionFS10_MaxSpeed,
			EDeviceParameterAxtMotionFS10_PulseOutputMethod,
			EDeviceParameterAxtMotionFS10_EncoderMethod,
			EDeviceParameterAxtMotionFS10_PLimitLevel,
			EDeviceParameterAxtMotionFS10_NLimitLevel,
			EDeviceParameterAxtMotionFS10_AlarmInputLevel,
			EDeviceParameterAxtMotionFS10_InpositionInputLevel,
			EDeviceParameterAxtMotionFS10_Count,
		};

		enum EDeviceParameterAxtMotionFS20
		{
			EDeviceParameterAxtMotionFS20_DeviceID = 0,
			EDeviceParameterAxtMotionFS20_SubUnitID,
			EDeviceParameterAxtMotionFS20_UnitPulse,
			EDeviceParameterAxtMotionFS20_StartSpeed,
			EDeviceParameterAxtMotionFS20_MaxSpeed,
			EDeviceParameterAxtMotionFS20_PulseOutputMethod,
			EDeviceParameterAxtMotionFS20_EncoderMethod,
			EDeviceParameterAxtMotionFS20_PLimitLevel,
			EDeviceParameterAxtMotionFS20_NLimitLevel,
			EDeviceParameterAxtMotionFS20_AlarmInputLevel,
			EDeviceParameterAxtMotionFS20_InpositionInputLevel,
			EDeviceParameterAxtMotionFS20_HomeAttribute,
			EDeviceParameterAxtMotionFS20_HomeSteps,
			EDeviceParameterAxtMotionFS20_Step1Method,
			EDeviceParameterAxtMotionFS20_Step1Stop,
			EDeviceParameterAxtMotionFS20_Step1Direction,
			EDeviceParameterAxtMotionFS20_Step1Detect,
			EDeviceParameterAxtMotionFS20_Step1Velocity,
			EDeviceParameterAxtMotionFS20_Step1Accel,
			EDeviceParameterAxtMotionFS20_Step2Method,
			EDeviceParameterAxtMotionFS20_Step2Stop,
			EDeviceParameterAxtMotionFS20_Step2Direction,
			EDeviceParameterAxtMotionFS20_Step2Detect,
			EDeviceParameterAxtMotionFS20_Step2Velocity,
			EDeviceParameterAxtMotionFS20_Step2Accel,
			EDeviceParameterAxtMotionFS20_Step3Method,
			EDeviceParameterAxtMotionFS20_Step3Stop,
			EDeviceParameterAxtMotionFS20_Step3Direction,
			EDeviceParameterAxtMotionFS20_Step3Detect,
			EDeviceParameterAxtMotionFS20_Step3Velocity,
			EDeviceParameterAxtMotionFS20_Step3Accel,
			EDeviceParameterAxtMotionFS20_Step4Method,
			EDeviceParameterAxtMotionFS20_Step4Stop,
			EDeviceParameterAxtMotionFS20_Step4Direction,
			EDeviceParameterAxtMotionFS20_Step4Detect,
			EDeviceParameterAxtMotionFS20_Step4Velocity,
			EDeviceParameterAxtMotionFS20_Step4Accel,
			EDeviceParameterAxtMotionFS20_Count,
		};

		enum EDeviceParameterAxlMotion
		{
			EDeviceParameterAxlMotion_DeviceID = 0,
			EDeviceParameterAxlMotion_SubUnitID,
			EDeviceParameterAxlMotion_UnitPulse,
			EDeviceParameterAxlMotion_StartSpeed,
			EDeviceParameterAxlMotion_MaxSpeed,
			EDeviceParameterAxlMotion_PulseOutputMethod,
			EDeviceParameterAxlMotion_EncoderMethod,
			EDeviceParameterAxlMotion_ServoLevel,
			EDeviceParameterAxlMotion_HomeLevel,
			EDeviceParameterAxlMotion_PLimitLevel,
			EDeviceParameterAxlMotion_NLimitLevel,
			EDeviceParameterAxlMotion_AlarmInputLevel,
			EDeviceParameterAxlMotion_AlarmResetLevel,
			EDeviceParameterAxlMotion_InpositionInputLevel,
			EDeviceParameterAxlMotion_LimitParameter,
			EDeviceParameterAxlMotion_UseSoftwareLimit,
			EDeviceParameterAxlMotion_StopMode,
			EDeviceParameterAxlMotion_CounterSelection,
			EDeviceParameterAxlMotion_PosLimitValue,
			EDeviceParameterAxlMotion_NegLimitValue,
			EDeviceParameterAxlMotion_HomeParameter,
			EDeviceParameterAxlMotion_HomeSearchSensor,
			EDeviceParameterAxlMotion_HomeDirection,
			EDeviceParameterAxlMotion_HomeZphase,
			EDeviceParameterAxlMotion_HomeHomeClearTime,
			EDeviceParameterAxlMotion_HomeOffset,
			EDeviceParameterAxlMotion_HomeFirstVel,
			EDeviceParameterAxlMotion_HomeSecondVel,
			EDeviceParameterAxlMotion_HomeThirdVel,
			EDeviceParameterAxlMotion_HomeLastVel,
			EDeviceParameterAxlMotion_HomeStartAcc,
			EDeviceParameterAxlMotion_HomeEndAcc,
			EDeviceParameterAxlMotion_OpenNoReset,
			EDeviceParameterAxlMotion_Count,
		};

		enum EDeviceParameterAxlMotionEthercat
		{
			EDeviceParameterAxlMotionEthercat_DeviceID = 0,
			EDeviceParameterAxlMotionEthercat_SubUnitID,
			EDeviceParameterAxlMotionEthercat_UnitPulse,
			EDeviceParameterAxlMotionEthercat_StartSpeed,
			EDeviceParameterAxlMotionEthercat_MaxSpeed,
			EDeviceParameterAxlMotionEthercat_PulseOutputMethod,
			EDeviceParameterAxlMotionEthercat_EncoderMethod,
			EDeviceParameterAxlMotionEthercat_ServoLevel,
			EDeviceParameterAxlMotionEthercat_HomeLevel,
			EDeviceParameterAxlMotionEthercat_PLimitLevel,
			EDeviceParameterAxlMotionEthercat_NLimitLevel,
			EDeviceParameterAxlMotionEthercat_AlarmInputLevel,
			EDeviceParameterAxlMotionEthercat_AlarmResetLevel,
			EDeviceParameterAxlMotionEthercat_InpositionInputLevel,
			EDeviceParameterAxlMotionEthercat_LimitParameter,
			EDeviceParameterAxlMotionEthercat_UseSoftwareLimit,
			EDeviceParameterAxlMotionEthercat_StopMode,
			EDeviceParameterAxlMotionEthercat_CounterSelection,
			EDeviceParameterAxlMotionEthercat_PosLimitValue,
			EDeviceParameterAxlMotionEthercat_NegLimitValue,
			EDeviceParameterAxlMotionEthercat_HomeParameter,
			EDeviceParameterAxlMotionEthercat_HomeSearchSensor,
			EDeviceParameterAxlMotionEthercat_HomeDirection,
			EDeviceParameterAxlMotionEthercat_HomeZphase,
			EDeviceParameterAxlMotionEthercat_HomeHomeClearTime,
			EDeviceParameterAxlMotionEthercat_HomeOffset,
			EDeviceParameterAxlMotionEthercat_HomeFirstVel,
			EDeviceParameterAxlMotionEthercat_HomeSecondVel,
			EDeviceParameterAxlMotionEthercat_HomeThirdVel,
			EDeviceParameterAxlMotionEthercat_HomeLastVel,
			EDeviceParameterAxlMotionEthercat_HomeStartAcc,
			EDeviceParameterAxlMotionEthercat_HomeEndAcc,
			EDeviceParameterAxlMotionEthercat_OpenNoReset,
			EDeviceParameterAxlMotionEthercat_Count,
		};

		enum EDeviceParameterAxtTrigger
		{
			EDeviceParameterAxtTrigger_DeviceID = 0,
			EDeviceParameterAxtTrigger_SubUnitID,
			EDeviceParameterAxtTrigger_Startposition,
			EDeviceParameterAxtTrigger_Endposition,
			EDeviceParameterAxtTrigger_Triggercycle,
			EDeviceParameterAxtTrigger_Triggerpulsedurationtime,
			EDeviceParameterAxtTrigger_Encoderphase,
			EDeviceParameterAxtTrigger_Encoderphasemode,
			EDeviceParameterAxtTrigger_Encoderincreasementdirection,
			EDeviceParameterAxtTrigger_Activelevel,
			EDeviceParameterAxtTrigger_Count,
		};

		enum EDeviceParameterAxlTriggerSIOCN2CH
		{
			EDeviceParameterAxlTriggerSIOCN2CH_DeviceID = 0,
			EDeviceParameterAxlTriggerSIOCN2CH_SubUnitID,
			EDeviceParameterAxlTriggerSIOCN2CH_UnitPerPulse,
			EDeviceParameterAxlTriggerSIOCN2CH_TriggerFunction,
			EDeviceParameterAxlTriggerSIOCN2CH_SignalSetting,
			EDeviceParameterAxlTriggerSIOCN2CH_EncoderInputMethod,
			EDeviceParameterAxlTriggerSIOCN2CH_EncoderSource,
			EDeviceParameterAxlTriggerSIOCN2CH_EncoderReverse,
			EDeviceParameterAxlTriggerSIOCN2CH_TriggerSetting,
			EDeviceParameterAxlTriggerSIOCN2CH_TriggerMode,
			EDeviceParameterAxlTriggerSIOCN2CH_TriggerActiveLevel,
			EDeviceParameterAxlTriggerSIOCN2CH_TriggerDirectionCheck,
			EDeviceParameterAxlTriggerSIOCN2CH_TriggerPulseWidth,
			EDeviceParameterAxlTriggerSIOCN2CH_Periodic,
			EDeviceParameterAxlTriggerSIOCN2CH_StartPosition,
			EDeviceParameterAxlTriggerSIOCN2CH_EndPosition,
			EDeviceParameterAxlTriggerSIOCN2CH_TriggerPeriod,
			EDeviceParameterAxlTriggerSIOCN2CH_OpenNoReset,
			EDeviceParameterAxlTriggerSIOCN2CH_Count,
		};

		enum EDeviceParameterMVTechTrigger
		{
			EDeviceParameterMVTechTrigger_DeviceID = 0,
			EDeviceParameterMVTechTrigger_Startposition,
			EDeviceParameterMVTechTrigger_Endposition,
			EDeviceParameterMVTechTrigger_Triggercycle,
			EDeviceParameterMVTechTrigger_Triggerpulsedurationtime,
			EDeviceParameterMVTechTrigger_Encoderincreasementdirection,
			EDeviceParameterMVTechTrigger_Triggercountingdirection,
			EDeviceParameterMVTechTrigger_Count,
		};

		enum EDeviceParameterMVTechMSPTrigger
		{
			EDeviceParameterMVTechMSPTrigger_DeviceID = 0,
			EDeviceParameterMVTechMSPTrigger_BoardType,
			EDeviceParameterMVTechMSPTrigger_UIInfo,
			EDeviceParameterMVTechMSPTrigger_ShowDIO,
			EDeviceParameterMVTechMSPTrigger_ShowTrgEnc,
			EDeviceParameterMVTechMSPTrigger_ShowQueueCH1,
			EDeviceParameterMVTechMSPTrigger_ShowQueueCH2,
			EDeviceParameterMVTechMSPTrigger_ShowID,
			EDeviceParameterMVTechMSPTrigger_TriggerInfo,
			EDeviceParameterMVTechMSPTrigger_Channel1,
			EDeviceParameterMVTechMSPTrigger_EncoderDirectionCH1,
			EDeviceParameterMVTechMSPTrigger_StartPositionCH1,
			EDeviceParameterMVTechMSPTrigger_EndPositionCH1,
			EDeviceParameterMVTechMSPTrigger_TriggerOnTimeCH1,
			EDeviceParameterMVTechMSPTrigger_TriggerCycleCH1,
			EDeviceParameterMVTechMSPTrigger_StrobeTimeCH1,
			EDeviceParameterMVTechMSPTrigger_Channel2,
			EDeviceParameterMVTechMSPTrigger_EncoderDirectionCH2,
			EDeviceParameterMVTechMSPTrigger_StartPositionCH2,
			EDeviceParameterMVTechMSPTrigger_EndPositionCH2,
			EDeviceParameterMVTechMSPTrigger_TriggerOnTimeCH2,
			EDeviceParameterMVTechMSPTrigger_TriggerCycleCH2,
			EDeviceParameterMVTechMSPTrigger_StrobeTimeCH2,
			EDeviceParameterMVTechMSPTrigger_UseInput,
			EDeviceParameterMVTechMSPTrigger_UseInputNo0,
			EDeviceParameterMVTechMSPTrigger_UseInputNo1,
			EDeviceParameterMVTechMSPTrigger_UseInputNo2,
			EDeviceParameterMVTechMSPTrigger_UseInputNo3,
			EDeviceParameterMVTechMSPTrigger_UseInputNo4,
			EDeviceParameterMVTechMSPTrigger_UseInputNo5,
			EDeviceParameterMVTechMSPTrigger_UseInputNo6,
			EDeviceParameterMVTechMSPTrigger_UseInputNo7,
			EDeviceParameterMVTechMSPTrigger_UseInputNo8,
			EDeviceParameterMVTechMSPTrigger_UseInputNo9,
			EDeviceParameterMVTechMSPTrigger_UseInputNo10,
			EDeviceParameterMVTechMSPTrigger_UseInputNo11,
			EDeviceParameterMVTechMSPTrigger_UseInputNo12,
			EDeviceParameterMVTechMSPTrigger_UseInputNo13,
			EDeviceParameterMVTechMSPTrigger_UseInputNo14,
			EDeviceParameterMVTechMSPTrigger_UseInputNo15,
			EDeviceParameterMVTechMSPTrigger_UseOutput,
			EDeviceParameterMVTechMSPTrigger_UseOutputNo0,
			EDeviceParameterMVTechMSPTrigger_UseOutputNo1,
			EDeviceParameterMVTechMSPTrigger_UseOutputNo2,
			EDeviceParameterMVTechMSPTrigger_UseOutputNo3,
			EDeviceParameterMVTechMSPTrigger_UseOutputNo4,
			EDeviceParameterMVTechMSPTrigger_UseOutputNo5,
			EDeviceParameterMVTechMSPTrigger_UseOutputNo6,
			EDeviceParameterMVTechMSPTrigger_UseOutputNo7,
			EDeviceParameterMVTechMSPTrigger_UseOutputNo8,
			EDeviceParameterMVTechMSPTrigger_UseOutputNo9,
			EDeviceParameterMVTechMSPTrigger_UseOutputNo10,
			EDeviceParameterMVTechMSPTrigger_UseOutputNo11,
			EDeviceParameterMVTechMSPTrigger_UseOutputNo12,
			EDeviceParameterMVTechMSPTrigger_UseOutputNo13,
			EDeviceParameterMVTechMSPTrigger_UseOutputNo14,
			EDeviceParameterMVTechMSPTrigger_UseOutputNo15,
			EDeviceParameterMVTechMSPTrigger_InputName,
			EDeviceParameterMVTechMSPTrigger_InputNo0Name,
			EDeviceParameterMVTechMSPTrigger_InputNo1Name,
			EDeviceParameterMVTechMSPTrigger_InputNo2Name,
			EDeviceParameterMVTechMSPTrigger_InputNo3Name,
			EDeviceParameterMVTechMSPTrigger_InputNo4Name,
			EDeviceParameterMVTechMSPTrigger_InputNo5Name,
			EDeviceParameterMVTechMSPTrigger_InputNo6Name,
			EDeviceParameterMVTechMSPTrigger_InputNo7Name,
			EDeviceParameterMVTechMSPTrigger_InputNo8Name,
			EDeviceParameterMVTechMSPTrigger_InputNo9Name,
			EDeviceParameterMVTechMSPTrigger_InputNo10Name,
			EDeviceParameterMVTechMSPTrigger_InputNo11Name,
			EDeviceParameterMVTechMSPTrigger_InputNo12Name,
			EDeviceParameterMVTechMSPTrigger_InputNo13Name,
			EDeviceParameterMVTechMSPTrigger_InputNo14Name,
			EDeviceParameterMVTechMSPTrigger_InputNo15Name,
			EDeviceParameterMVTechMSPTrigger_OutputName,
			EDeviceParameterMVTechMSPTrigger_OutputNo0Name,
			EDeviceParameterMVTechMSPTrigger_OutputNo1Name,
			EDeviceParameterMVTechMSPTrigger_OutputNo2Name,
			EDeviceParameterMVTechMSPTrigger_OutputNo3Name,
			EDeviceParameterMVTechMSPTrigger_OutputNo4Name,
			EDeviceParameterMVTechMSPTrigger_OutputNo5Name,
			EDeviceParameterMVTechMSPTrigger_OutputNo6Name,
			EDeviceParameterMVTechMSPTrigger_OutputNo7Name,
			EDeviceParameterMVTechMSPTrigger_OutputNo8Name,
			EDeviceParameterMVTechMSPTrigger_OutputNo9Name,
			EDeviceParameterMVTechMSPTrigger_OutputNo10Name,
			EDeviceParameterMVTechMSPTrigger_OutputNo11Name,
			EDeviceParameterMVTechMSPTrigger_OutputNo12Name,
			EDeviceParameterMVTechMSPTrigger_OutputNo13Name,
			EDeviceParameterMVTechMSPTrigger_OutputNo14Name,
			EDeviceParameterMVTechMSPTrigger_OutputNo15Name,
			EDeviceParameterMVTechMSPTrigger_Count,
		};

		enum EDeviceParameterTcpClient
		{
			EDeviceParameterTcpClient_DeviceID = 0,
			EDeviceParameterTcpClient_Connectionsettings,
			EDeviceParameterTcpClient_ConnectingServerIPAddress,
			EDeviceParameterTcpClient_ConnectingServerPort,
			EDeviceParameterTcpClient_ConnectingTimeout,
			EDeviceParameterTcpClient_Transmissionsettings,
			EDeviceParameterTcpClient_TxBufferSize,
			EDeviceParameterTcpClient_RxBufferSize,
			EDeviceParameterTcpClient_Connectionvalidity,
			EDeviceParameterTcpClient_Usecheckingalive,
			EDeviceParameterTcpClient_Checkaliveinterval,
			EDeviceParameterTcpClient_Checkalivetimeout,
			EDeviceParameterTcpClient_Count,
		};

		enum EDeviceParameterTcpServer
		{
			EDeviceParameterTcpServer_DeviceID = 0,
			EDeviceParameterTcpServer_Connectionsettings,
			EDeviceParameterTcpServer_BindingIPAddress,
			EDeviceParameterTcpServer_BindingPort,
			EDeviceParameterTcpServer_Transmissionsettings,
			EDeviceParameterTcpServer_TxBufferSize,
			EDeviceParameterTcpServer_RxBufferSize,
			EDeviceParameterTcpServer_Connectionvalidity,
			EDeviceParameterTcpServer_Usecheckingalive,
			EDeviceParameterTcpServer_Checkaliveinterval,
			EDeviceParameterTcpServer_Checkalivetimeout,
			EDeviceParameterTcpServer_Count,
		};

		enum EDeviceParameterRS232C
		{
			EDeviceParameterRS232C_DeviceID = 0,
			EDeviceParameterRS232C_TransmissionSettings,
			EDeviceParameterRS232C_BaudRate,
			EDeviceParameterRS232C_DataBits,
			EDeviceParameterRS232C_Parity,
			EDeviceParameterRS232C_StopBits,
			EDeviceParameterRS232C_TxBufferSize,
			EDeviceParameterRS232C_RxBufferSize,
			EDeviceParameterRS232C_Display,
			EDeviceParameterRS232C_SendingFormat,
			EDeviceParameterRS232C_ReceivingViewFormat,
			EDeviceParameterRS232C_Count,
		};


		enum EDeviceParameterGoogolGTS
		{
			EDeviceParameterGoogolGTS_DeviceID = 0,
			EDeviceParameterGoogolGTS_InitializeType,
			EDeviceParameterGoogolGTS_LoadfilePath,
			EDeviceParameterGoogolGTS_LoadExtFilePath,
			EDeviceParameterGoogolGTS_MotionSetting,
			EDeviceParameterGoogolGTS_Axis1Band,
			EDeviceParameterGoogolGTS_Axis1Time,
			EDeviceParameterGoogolGTS_Axis1AbruptStop,
			EDeviceParameterGoogolGTS_Axis1EStop,
			EDeviceParameterGoogolGTS_Axis2Band,
			EDeviceParameterGoogolGTS_Axis2Time,
			EDeviceParameterGoogolGTS_Axis2AbruptStop,
			EDeviceParameterGoogolGTS_Axis2EStop,
			EDeviceParameterGoogolGTS_Axis3Band,
			EDeviceParameterGoogolGTS_Axis3Time,
			EDeviceParameterGoogolGTS_Axis3AbruptStop,
			EDeviceParameterGoogolGTS_Axis3EStop,
			EDeviceParameterGoogolGTS_Axis4Band,
			EDeviceParameterGoogolGTS_Axis4Time,
			EDeviceParameterGoogolGTS_Axis4AbruptStop,
			EDeviceParameterGoogolGTS_Axis4EStop,
			EDeviceParameterGoogolGTS_Axis5Band,
			EDeviceParameterGoogolGTS_Axis5Time,
			EDeviceParameterGoogolGTS_Axis5AbruptStop,
			EDeviceParameterGoogolGTS_Axis5EStop,
			EDeviceParameterGoogolGTS_Axis6Band,
			EDeviceParameterGoogolGTS_Axis6Time,
			EDeviceParameterGoogolGTS_Axis6AbruptStop,
			EDeviceParameterGoogolGTS_Axis6EStop,
			EDeviceParameterGoogolGTS_Axis7Band,
			EDeviceParameterGoogolGTS_Axis7Time,
			EDeviceParameterGoogolGTS_Axis7AbruptStop,
			EDeviceParameterGoogolGTS_Axis7EStop,
			EDeviceParameterGoogolGTS_Axis8Band,
			EDeviceParameterGoogolGTS_Axis8Time,
			EDeviceParameterGoogolGTS_Axis8AbruptStop,
			EDeviceParameterGoogolGTS_Axis8EStop,
			EDeviceParameterGoogolGTS_Home,
			EDeviceParameterGoogolGTS_Axis1DirAndPos,
			EDeviceParameterGoogolGTS_Axis1Velocity,
			EDeviceParameterGoogolGTS_Axis1Accel,
			EDeviceParameterGoogolGTS_Axis1Offset,
			EDeviceParameterGoogolGTS_Axis2DirAndPos,
			EDeviceParameterGoogolGTS_Axis2Velocity,
			EDeviceParameterGoogolGTS_Axis2Accel,
			EDeviceParameterGoogolGTS_Axis2Offset,
			EDeviceParameterGoogolGTS_Axis3DirAndPos,
			EDeviceParameterGoogolGTS_Axis3Velocity,
			EDeviceParameterGoogolGTS_Axis3Accel,
			EDeviceParameterGoogolGTS_Axis3Offset,
			EDeviceParameterGoogolGTS_Axis4DirAndPos,
			EDeviceParameterGoogolGTS_Axis4Velocity,
			EDeviceParameterGoogolGTS_Axis4Accel,
			EDeviceParameterGoogolGTS_Axis4Offset,
			EDeviceParameterGoogolGTS_Axis5DirAndPos,
			EDeviceParameterGoogolGTS_Axis5Velocity,
			EDeviceParameterGoogolGTS_Axis5Accel,
			EDeviceParameterGoogolGTS_Axis5Offset,
			EDeviceParameterGoogolGTS_Axis6DirAndPos,
			EDeviceParameterGoogolGTS_Axis6Velocity,
			EDeviceParameterGoogolGTS_Axis6Accel,
			EDeviceParameterGoogolGTS_Axis6Offset,
			EDeviceParameterGoogolGTS_Axis7DirAndPos,
			EDeviceParameterGoogolGTS_Axis7Velocity,
			EDeviceParameterGoogolGTS_Axis7Accel,
			EDeviceParameterGoogolGTS_Axis7Offset,
			EDeviceParameterGoogolGTS_Axis8DirAndPos,
			EDeviceParameterGoogolGTS_Axis8Velocity,
			EDeviceParameterGoogolGTS_Axis8Accel,
			EDeviceParameterGoogolGTS_Axis8Offset,
			EDeviceParameterGoogolGTS_InputStatus,
			EDeviceParameterGoogolGTS_UseInputNo0,
			EDeviceParameterGoogolGTS_UseInputNo1,
			EDeviceParameterGoogolGTS_UseInputNo2,
			EDeviceParameterGoogolGTS_UseInputNo3,
			EDeviceParameterGoogolGTS_UseInputNo4,
			EDeviceParameterGoogolGTS_UseInputNo5,
			EDeviceParameterGoogolGTS_UseInputNo6,
			EDeviceParameterGoogolGTS_UseInputNo7,
			EDeviceParameterGoogolGTS_UseInputNo8,
			EDeviceParameterGoogolGTS_UseInputNo9,
			EDeviceParameterGoogolGTS_UseInputNo10,
			EDeviceParameterGoogolGTS_UseInputNo11,
			EDeviceParameterGoogolGTS_UseInputNo12,
			EDeviceParameterGoogolGTS_UseInputNo13,
			EDeviceParameterGoogolGTS_UseInputNo14,
			EDeviceParameterGoogolGTS_UseInputNo15,
			EDeviceParameterGoogolGTS_OutputStatus,
			EDeviceParameterGoogolGTS_UseOutputNo0,
			EDeviceParameterGoogolGTS_UseOutputNo1,
			EDeviceParameterGoogolGTS_UseOutputNo2,
			EDeviceParameterGoogolGTS_UseOutputNo3,
			EDeviceParameterGoogolGTS_UseOutputNo4,
			EDeviceParameterGoogolGTS_UseOutputNo5,
			EDeviceParameterGoogolGTS_UseOutputNo6,
			EDeviceParameterGoogolGTS_UseOutputNo7,
			EDeviceParameterGoogolGTS_UseOutputNo8,
			EDeviceParameterGoogolGTS_UseOutputNo9,
			EDeviceParameterGoogolGTS_UseOutputNo10,
			EDeviceParameterGoogolGTS_UseOutputNo11,
			EDeviceParameterGoogolGTS_UseOutputNo12,
			EDeviceParameterGoogolGTS_UseOutputNo13,
			EDeviceParameterGoogolGTS_UseOutputNo14,
			EDeviceParameterGoogolGTS_UseOutputNo15,
			EDeviceParameterGoogolGTS_InputName,
			EDeviceParameterGoogolGTS_InputNo0Name,
			EDeviceParameterGoogolGTS_InputNo1Name,
			EDeviceParameterGoogolGTS_InputNo2Name,
			EDeviceParameterGoogolGTS_InputNo3Name,
			EDeviceParameterGoogolGTS_InputNo4Name,
			EDeviceParameterGoogolGTS_InputNo5Name,
			EDeviceParameterGoogolGTS_InputNo6Name,
			EDeviceParameterGoogolGTS_InputNo7Name,
			EDeviceParameterGoogolGTS_InputNo8Name,
			EDeviceParameterGoogolGTS_InputNo9Name,
			EDeviceParameterGoogolGTS_InputNo10Name,
			EDeviceParameterGoogolGTS_InputNo11Name,
			EDeviceParameterGoogolGTS_InputNo12Name,
			EDeviceParameterGoogolGTS_InputNo13Name,
			EDeviceParameterGoogolGTS_InputNo14Name,
			EDeviceParameterGoogolGTS_InputNo15Name,
			EDeviceParameterGoogolGTS_OutputName,
			EDeviceParameterGoogolGTS_OutputNo0Name,
			EDeviceParameterGoogolGTS_OutputNo1Name,
			EDeviceParameterGoogolGTS_OutputNo2Name,
			EDeviceParameterGoogolGTS_OutputNo3Name,
			EDeviceParameterGoogolGTS_OutputNo4Name,
			EDeviceParameterGoogolGTS_OutputNo5Name,
			EDeviceParameterGoogolGTS_OutputNo6Name,
			EDeviceParameterGoogolGTS_OutputNo7Name,
			EDeviceParameterGoogolGTS_OutputNo8Name,
			EDeviceParameterGoogolGTS_OutputNo9Name,
			EDeviceParameterGoogolGTS_OutputNo10Name,
			EDeviceParameterGoogolGTS_OutputNo11Name,
			EDeviceParameterGoogolGTS_OutputNo12Name,
			EDeviceParameterGoogolGTS_OutputNo13Name,
			EDeviceParameterGoogolGTS_OutputNo14Name,
			EDeviceParameterGoogolGTS_OutputNo15Name,
			EDeviceParameterGoogolGTS_ExternalIOStatus,
			EDeviceParameterGoogolGTS_ShowExt1,
			EDeviceParameterGoogolGTS_ShowExt2,
			EDeviceParameterGoogolGTS_ShowExt3,
			EDeviceParameterGoogolGTS_ShowExt4,
			EDeviceParameterGoogolGTS_ShowExt5,
			EDeviceParameterGoogolGTS_ShowExt6,
			EDeviceParameterGoogolGTS_ShowExt7,
			EDeviceParameterGoogolGTS_ShowExt8,
			EDeviceParameterGoogolGTS_ShowExt9,
			EDeviceParameterGoogolGTS_ShowExt10,
			EDeviceParameterGoogolGTS_ShowExt11,
			EDeviceParameterGoogolGTS_ShowExt12,
			EDeviceParameterGoogolGTS_ShowExt13,
			EDeviceParameterGoogolGTS_ShowExt14,
			EDeviceParameterGoogolGTS_ShowExt15,
			EDeviceParameterGoogolGTS_ShowExt16,
			EDeviceParameterGoogolGTS_Ext1InputName,
			EDeviceParameterGoogolGTS_Ext1InputNo0Name,
			EDeviceParameterGoogolGTS_Ext1InputNo1Name,
			EDeviceParameterGoogolGTS_Ext1InputNo2Name,
			EDeviceParameterGoogolGTS_Ext1InputNo3Name,
			EDeviceParameterGoogolGTS_Ext1InputNo4Name,
			EDeviceParameterGoogolGTS_Ext1InputNo5Name,
			EDeviceParameterGoogolGTS_Ext1InputNo6Name,
			EDeviceParameterGoogolGTS_Ext1InputNo7Name,
			EDeviceParameterGoogolGTS_Ext1InputNo8Name,
			EDeviceParameterGoogolGTS_Ext1InputNo9Name,
			EDeviceParameterGoogolGTS_Ext1InputNo10Name,
			EDeviceParameterGoogolGTS_Ext1InputNo11Name,
			EDeviceParameterGoogolGTS_Ext1InputNo12Name,
			EDeviceParameterGoogolGTS_Ext1InputNo13Name,
			EDeviceParameterGoogolGTS_Ext1InputNo14Name,
			EDeviceParameterGoogolGTS_Ext1InputNo15Name,
			EDeviceParameterGoogolGTS_Ext2InputName,
			EDeviceParameterGoogolGTS_Ext2InputNo0Name,
			EDeviceParameterGoogolGTS_Ext2InputNo1Name,
			EDeviceParameterGoogolGTS_Ext2InputNo2Name,
			EDeviceParameterGoogolGTS_Ext2InputNo3Name,
			EDeviceParameterGoogolGTS_Ext2InputNo4Name,
			EDeviceParameterGoogolGTS_Ext2InputNo5Name,
			EDeviceParameterGoogolGTS_Ext2InputNo6Name,
			EDeviceParameterGoogolGTS_Ext2InputNo7Name,
			EDeviceParameterGoogolGTS_Ext2InputNo8Name,
			EDeviceParameterGoogolGTS_Ext2InputNo9Name,
			EDeviceParameterGoogolGTS_Ext2InputNo10Name,
			EDeviceParameterGoogolGTS_Ext2InputNo11Name,
			EDeviceParameterGoogolGTS_Ext2InputNo12Name,
			EDeviceParameterGoogolGTS_Ext2InputNo13Name,
			EDeviceParameterGoogolGTS_Ext2InputNo14Name,
			EDeviceParameterGoogolGTS_Ext2InputNo15Name,
			EDeviceParameterGoogolGTS_Ext3InputName,
			EDeviceParameterGoogolGTS_Ext3InputNo0Name,
			EDeviceParameterGoogolGTS_Ext3InputNo1Name,
			EDeviceParameterGoogolGTS_Ext3InputNo2Name,
			EDeviceParameterGoogolGTS_Ext3InputNo3Name,
			EDeviceParameterGoogolGTS_Ext3InputNo4Name,
			EDeviceParameterGoogolGTS_Ext3InputNo5Name,
			EDeviceParameterGoogolGTS_Ext3InputNo6Name,
			EDeviceParameterGoogolGTS_Ext3InputNo7Name,
			EDeviceParameterGoogolGTS_Ext3InputNo8Name,
			EDeviceParameterGoogolGTS_Ext3InputNo9Name,
			EDeviceParameterGoogolGTS_Ext3InputNo10Name,
			EDeviceParameterGoogolGTS_Ext3InputNo11Name,
			EDeviceParameterGoogolGTS_Ext3InputNo12Name,
			EDeviceParameterGoogolGTS_Ext3InputNo13Name,
			EDeviceParameterGoogolGTS_Ext3InputNo14Name,
			EDeviceParameterGoogolGTS_Ext3InputNo15Name,
			EDeviceParameterGoogolGTS_Ext4InputName,
			EDeviceParameterGoogolGTS_Ext4InputNo0Name,
			EDeviceParameterGoogolGTS_Ext4InputNo1Name,
			EDeviceParameterGoogolGTS_Ext4InputNo2Name,
			EDeviceParameterGoogolGTS_Ext4InputNo3Name,
			EDeviceParameterGoogolGTS_Ext4InputNo4Name,
			EDeviceParameterGoogolGTS_Ext4InputNo5Name,
			EDeviceParameterGoogolGTS_Ext4InputNo6Name,
			EDeviceParameterGoogolGTS_Ext4InputNo7Name,
			EDeviceParameterGoogolGTS_Ext4InputNo8Name,
			EDeviceParameterGoogolGTS_Ext4InputNo9Name,
			EDeviceParameterGoogolGTS_Ext4InputNo10Name,
			EDeviceParameterGoogolGTS_Ext4InputNo11Name,
			EDeviceParameterGoogolGTS_Ext4InputNo12Name,
			EDeviceParameterGoogolGTS_Ext4InputNo13Name,
			EDeviceParameterGoogolGTS_Ext4InputNo14Name,
			EDeviceParameterGoogolGTS_Ext4InputNo15Name,
			EDeviceParameterGoogolGTS_Ext5InputName,
			EDeviceParameterGoogolGTS_Ext5InputNo0Name,
			EDeviceParameterGoogolGTS_Ext5InputNo1Name,
			EDeviceParameterGoogolGTS_Ext5InputNo2Name,
			EDeviceParameterGoogolGTS_Ext5InputNo3Name,
			EDeviceParameterGoogolGTS_Ext5InputNo4Name,
			EDeviceParameterGoogolGTS_Ext5InputNo5Name,
			EDeviceParameterGoogolGTS_Ext5InputNo6Name,
			EDeviceParameterGoogolGTS_Ext5InputNo7Name,
			EDeviceParameterGoogolGTS_Ext5InputNo8Name,
			EDeviceParameterGoogolGTS_Ext5InputNo9Name,
			EDeviceParameterGoogolGTS_Ext5InputNo10Name,
			EDeviceParameterGoogolGTS_Ext5InputNo11Name,
			EDeviceParameterGoogolGTS_Ext5InputNo12Name,
			EDeviceParameterGoogolGTS_Ext5InputNo13Name,
			EDeviceParameterGoogolGTS_Ext5InputNo14Name,
			EDeviceParameterGoogolGTS_Ext5InputNo15Name,
			EDeviceParameterGoogolGTS_Ext6InputName,
			EDeviceParameterGoogolGTS_Ext6InputNo0Name,
			EDeviceParameterGoogolGTS_Ext6InputNo1Name,
			EDeviceParameterGoogolGTS_Ext6InputNo2Name,
			EDeviceParameterGoogolGTS_Ext6InputNo3Name,
			EDeviceParameterGoogolGTS_Ext6InputNo4Name,
			EDeviceParameterGoogolGTS_Ext6InputNo5Name,
			EDeviceParameterGoogolGTS_Ext6InputNo6Name,
			EDeviceParameterGoogolGTS_Ext6InputNo7Name,
			EDeviceParameterGoogolGTS_Ext6InputNo8Name,
			EDeviceParameterGoogolGTS_Ext6InputNo9Name,
			EDeviceParameterGoogolGTS_Ext6InputNo10Name,
			EDeviceParameterGoogolGTS_Ext6InputNo11Name,
			EDeviceParameterGoogolGTS_Ext6InputNo12Name,
			EDeviceParameterGoogolGTS_Ext6InputNo13Name,
			EDeviceParameterGoogolGTS_Ext6InputNo14Name,
			EDeviceParameterGoogolGTS_Ext6InputNo15Name,
			EDeviceParameterGoogolGTS_Ext7InputName,
			EDeviceParameterGoogolGTS_Ext7InputNo0Name,
			EDeviceParameterGoogolGTS_Ext7InputNo1Name,
			EDeviceParameterGoogolGTS_Ext7InputNo2Name,
			EDeviceParameterGoogolGTS_Ext7InputNo3Name,
			EDeviceParameterGoogolGTS_Ext7InputNo4Name,
			EDeviceParameterGoogolGTS_Ext7InputNo5Name,
			EDeviceParameterGoogolGTS_Ext7InputNo6Name,
			EDeviceParameterGoogolGTS_Ext7InputNo7Name,
			EDeviceParameterGoogolGTS_Ext7InputNo8Name,
			EDeviceParameterGoogolGTS_Ext7InputNo9Name,
			EDeviceParameterGoogolGTS_Ext7InputNo10Name,
			EDeviceParameterGoogolGTS_Ext7InputNo11Name,
			EDeviceParameterGoogolGTS_Ext7InputNo12Name,
			EDeviceParameterGoogolGTS_Ext7InputNo13Name,
			EDeviceParameterGoogolGTS_Ext7InputNo14Name,
			EDeviceParameterGoogolGTS_Ext7InputNo15Name,
			EDeviceParameterGoogolGTS_Ext8InputName,
			EDeviceParameterGoogolGTS_Ext8InputNo0Name,
			EDeviceParameterGoogolGTS_Ext8InputNo1Name,
			EDeviceParameterGoogolGTS_Ext8InputNo2Name,
			EDeviceParameterGoogolGTS_Ext8InputNo3Name,
			EDeviceParameterGoogolGTS_Ext8InputNo4Name,
			EDeviceParameterGoogolGTS_Ext8InputNo5Name,
			EDeviceParameterGoogolGTS_Ext8InputNo6Name,
			EDeviceParameterGoogolGTS_Ext8InputNo7Name,
			EDeviceParameterGoogolGTS_Ext8InputNo8Name,
			EDeviceParameterGoogolGTS_Ext8InputNo9Name,
			EDeviceParameterGoogolGTS_Ext8InputNo10Name,
			EDeviceParameterGoogolGTS_Ext8InputNo11Name,
			EDeviceParameterGoogolGTS_Ext8InputNo12Name,
			EDeviceParameterGoogolGTS_Ext8InputNo13Name,
			EDeviceParameterGoogolGTS_Ext8InputNo14Name,
			EDeviceParameterGoogolGTS_Ext8InputNo15Name,
			EDeviceParameterGoogolGTS_Ext9InputName,
			EDeviceParameterGoogolGTS_Ext9InputNo0Name,
			EDeviceParameterGoogolGTS_Ext9InputNo1Name,
			EDeviceParameterGoogolGTS_Ext9InputNo2Name,
			EDeviceParameterGoogolGTS_Ext9InputNo3Name,
			EDeviceParameterGoogolGTS_Ext9InputNo4Name,
			EDeviceParameterGoogolGTS_Ext9InputNo5Name,
			EDeviceParameterGoogolGTS_Ext9InputNo6Name,
			EDeviceParameterGoogolGTS_Ext9InputNo7Name,
			EDeviceParameterGoogolGTS_Ext9InputNo8Name,
			EDeviceParameterGoogolGTS_Ext9InputNo9Name,
			EDeviceParameterGoogolGTS_Ext9InputNo10Name,
			EDeviceParameterGoogolGTS_Ext9InputNo11Name,
			EDeviceParameterGoogolGTS_Ext9InputNo12Name,
			EDeviceParameterGoogolGTS_Ext9InputNo13Name,
			EDeviceParameterGoogolGTS_Ext9InputNo14Name,
			EDeviceParameterGoogolGTS_Ext9InputNo15Name,
			EDeviceParameterGoogolGTS_Ext10InputName,
			EDeviceParameterGoogolGTS_Ext10InputNo0Name,
			EDeviceParameterGoogolGTS_Ext10InputNo1Name,
			EDeviceParameterGoogolGTS_Ext10InputNo2Name,
			EDeviceParameterGoogolGTS_Ext10InputNo3Name,
			EDeviceParameterGoogolGTS_Ext10InputNo4Name,
			EDeviceParameterGoogolGTS_Ext10InputNo5Name,
			EDeviceParameterGoogolGTS_Ext10InputNo6Name,
			EDeviceParameterGoogolGTS_Ext10InputNo7Name,
			EDeviceParameterGoogolGTS_Ext10InputNo8Name,
			EDeviceParameterGoogolGTS_Ext10InputNo9Name,
			EDeviceParameterGoogolGTS_Ext10InputNo10Name,
			EDeviceParameterGoogolGTS_Ext10InputNo11Name,
			EDeviceParameterGoogolGTS_Ext10InputNo12Name,
			EDeviceParameterGoogolGTS_Ext10InputNo13Name,
			EDeviceParameterGoogolGTS_Ext10InputNo14Name,
			EDeviceParameterGoogolGTS_Ext10InputNo15Name,
			EDeviceParameterGoogolGTS_Ext11InputName,
			EDeviceParameterGoogolGTS_Ext11InputNo0Name,
			EDeviceParameterGoogolGTS_Ext11InputNo1Name,
			EDeviceParameterGoogolGTS_Ext11InputNo2Name,
			EDeviceParameterGoogolGTS_Ext11InputNo3Name,
			EDeviceParameterGoogolGTS_Ext11InputNo4Name,
			EDeviceParameterGoogolGTS_Ext11InputNo5Name,
			EDeviceParameterGoogolGTS_Ext11InputNo6Name,
			EDeviceParameterGoogolGTS_Ext11InputNo7Name,
			EDeviceParameterGoogolGTS_Ext11InputNo8Name,
			EDeviceParameterGoogolGTS_Ext11InputNo9Name,
			EDeviceParameterGoogolGTS_Ext11InputNo10Name,
			EDeviceParameterGoogolGTS_Ext11InputNo11Name,
			EDeviceParameterGoogolGTS_Ext11InputNo12Name,
			EDeviceParameterGoogolGTS_Ext11InputNo13Name,
			EDeviceParameterGoogolGTS_Ext11InputNo14Name,
			EDeviceParameterGoogolGTS_Ext11InputNo15Name,
			EDeviceParameterGoogolGTS_Ext12InputName,
			EDeviceParameterGoogolGTS_Ext12InputNo0Name,
			EDeviceParameterGoogolGTS_Ext12InputNo1Name,
			EDeviceParameterGoogolGTS_Ext12InputNo2Name,
			EDeviceParameterGoogolGTS_Ext12InputNo3Name,
			EDeviceParameterGoogolGTS_Ext12InputNo4Name,
			EDeviceParameterGoogolGTS_Ext12InputNo5Name,
			EDeviceParameterGoogolGTS_Ext12InputNo6Name,
			EDeviceParameterGoogolGTS_Ext12InputNo7Name,
			EDeviceParameterGoogolGTS_Ext12InputNo8Name,
			EDeviceParameterGoogolGTS_Ext12InputNo9Name,
			EDeviceParameterGoogolGTS_Ext12InputNo10Name,
			EDeviceParameterGoogolGTS_Ext12InputNo11Name,
			EDeviceParameterGoogolGTS_Ext12InputNo12Name,
			EDeviceParameterGoogolGTS_Ext12InputNo13Name,
			EDeviceParameterGoogolGTS_Ext12InputNo14Name,
			EDeviceParameterGoogolGTS_Ext12InputNo15Name,
			EDeviceParameterGoogolGTS_Ext13InputName,
			EDeviceParameterGoogolGTS_Ext13InputNo0Name,
			EDeviceParameterGoogolGTS_Ext13InputNo1Name,
			EDeviceParameterGoogolGTS_Ext13InputNo2Name,
			EDeviceParameterGoogolGTS_Ext13InputNo3Name,
			EDeviceParameterGoogolGTS_Ext13InputNo4Name,
			EDeviceParameterGoogolGTS_Ext13InputNo5Name,
			EDeviceParameterGoogolGTS_Ext13InputNo6Name,
			EDeviceParameterGoogolGTS_Ext13InputNo7Name,
			EDeviceParameterGoogolGTS_Ext13InputNo8Name,
			EDeviceParameterGoogolGTS_Ext13InputNo9Name,
			EDeviceParameterGoogolGTS_Ext13InputNo10Name,
			EDeviceParameterGoogolGTS_Ext13InputNo11Name,
			EDeviceParameterGoogolGTS_Ext13InputNo12Name,
			EDeviceParameterGoogolGTS_Ext13InputNo13Name,
			EDeviceParameterGoogolGTS_Ext13InputNo14Name,
			EDeviceParameterGoogolGTS_Ext13InputNo15Name,
			EDeviceParameterGoogolGTS_Ext14InputName,
			EDeviceParameterGoogolGTS_Ext14InputNo0Name,
			EDeviceParameterGoogolGTS_Ext14InputNo1Name,
			EDeviceParameterGoogolGTS_Ext14InputNo2Name,
			EDeviceParameterGoogolGTS_Ext14InputNo3Name,
			EDeviceParameterGoogolGTS_Ext14InputNo4Name,
			EDeviceParameterGoogolGTS_Ext14InputNo5Name,
			EDeviceParameterGoogolGTS_Ext14InputNo6Name,
			EDeviceParameterGoogolGTS_Ext14InputNo7Name,
			EDeviceParameterGoogolGTS_Ext14InputNo8Name,
			EDeviceParameterGoogolGTS_Ext14InputNo9Name,
			EDeviceParameterGoogolGTS_Ext14InputNo10Name,
			EDeviceParameterGoogolGTS_Ext14InputNo11Name,
			EDeviceParameterGoogolGTS_Ext14InputNo12Name,
			EDeviceParameterGoogolGTS_Ext14InputNo13Name,
			EDeviceParameterGoogolGTS_Ext14InputNo14Name,
			EDeviceParameterGoogolGTS_Ext14InputNo15Name,
			EDeviceParameterGoogolGTS_Ext15InputName,
			EDeviceParameterGoogolGTS_Ext15InputNo0Name,
			EDeviceParameterGoogolGTS_Ext15InputNo1Name,
			EDeviceParameterGoogolGTS_Ext15InputNo2Name,
			EDeviceParameterGoogolGTS_Ext15InputNo3Name,
			EDeviceParameterGoogolGTS_Ext15InputNo4Name,
			EDeviceParameterGoogolGTS_Ext15InputNo5Name,
			EDeviceParameterGoogolGTS_Ext15InputNo6Name,
			EDeviceParameterGoogolGTS_Ext15InputNo7Name,
			EDeviceParameterGoogolGTS_Ext15InputNo8Name,
			EDeviceParameterGoogolGTS_Ext15InputNo9Name,
			EDeviceParameterGoogolGTS_Ext15InputNo10Name,
			EDeviceParameterGoogolGTS_Ext15InputNo11Name,
			EDeviceParameterGoogolGTS_Ext15InputNo12Name,
			EDeviceParameterGoogolGTS_Ext15InputNo13Name,
			EDeviceParameterGoogolGTS_Ext15InputNo14Name,
			EDeviceParameterGoogolGTS_Ext15InputNo15Name,
			EDeviceParameterGoogolGTS_Ext16InputName,
			EDeviceParameterGoogolGTS_Ext16InputNo0Name,
			EDeviceParameterGoogolGTS_Ext16InputNo1Name,
			EDeviceParameterGoogolGTS_Ext16InputNo2Name,
			EDeviceParameterGoogolGTS_Ext16InputNo3Name,
			EDeviceParameterGoogolGTS_Ext16InputNo4Name,
			EDeviceParameterGoogolGTS_Ext16InputNo5Name,
			EDeviceParameterGoogolGTS_Ext16InputNo6Name,
			EDeviceParameterGoogolGTS_Ext16InputNo7Name,
			EDeviceParameterGoogolGTS_Ext16InputNo8Name,
			EDeviceParameterGoogolGTS_Ext16InputNo9Name,
			EDeviceParameterGoogolGTS_Ext16InputNo10Name,
			EDeviceParameterGoogolGTS_Ext16InputNo11Name,
			EDeviceParameterGoogolGTS_Ext16InputNo12Name,
			EDeviceParameterGoogolGTS_Ext16InputNo13Name,
			EDeviceParameterGoogolGTS_Ext16InputNo14Name,
			EDeviceParameterGoogolGTS_Ext16InputNo15Name,
			EDeviceParameterGoogolGTS_Ext1OutputName,
			EDeviceParameterGoogolGTS_Ext1OutputNo0Name,
			EDeviceParameterGoogolGTS_Ext1OutputNo1Name,
			EDeviceParameterGoogolGTS_Ext1OutputNo2Name,
			EDeviceParameterGoogolGTS_Ext1OutputNo3Name,
			EDeviceParameterGoogolGTS_Ext1OutputNo4Name,
			EDeviceParameterGoogolGTS_Ext1OutputNo5Name,
			EDeviceParameterGoogolGTS_Ext1OutputNo6Name,
			EDeviceParameterGoogolGTS_Ext1OutputNo7Name,
			EDeviceParameterGoogolGTS_Ext1OutputNo8Name,
			EDeviceParameterGoogolGTS_Ext1OutputNo9Name,
			EDeviceParameterGoogolGTS_Ext1OutputNo10Name,
			EDeviceParameterGoogolGTS_Ext1OutputNo11Name,
			EDeviceParameterGoogolGTS_Ext1OutputNo12Name,
			EDeviceParameterGoogolGTS_Ext1OutputNo13Name,
			EDeviceParameterGoogolGTS_Ext1OutputNo14Name,
			EDeviceParameterGoogolGTS_Ext1OutputNo15Name,
			EDeviceParameterGoogolGTS_Ext2OutputName,
			EDeviceParameterGoogolGTS_Ext2OutputNo0Name,
			EDeviceParameterGoogolGTS_Ext2OutputNo1Name,
			EDeviceParameterGoogolGTS_Ext2OutputNo2Name,
			EDeviceParameterGoogolGTS_Ext2OutputNo3Name,
			EDeviceParameterGoogolGTS_Ext2OutputNo4Name,
			EDeviceParameterGoogolGTS_Ext2OutputNo5Name,
			EDeviceParameterGoogolGTS_Ext2OutputNo6Name,
			EDeviceParameterGoogolGTS_Ext2OutputNo7Name,
			EDeviceParameterGoogolGTS_Ext2OutputNo8Name,
			EDeviceParameterGoogolGTS_Ext2OutputNo9Name,
			EDeviceParameterGoogolGTS_Ext2OutputNo10Name,
			EDeviceParameterGoogolGTS_Ext2OutputNo11Name,
			EDeviceParameterGoogolGTS_Ext2OutputNo12Name,
			EDeviceParameterGoogolGTS_Ext2OutputNo13Name,
			EDeviceParameterGoogolGTS_Ext2OutputNo14Name,
			EDeviceParameterGoogolGTS_Ext2OutputNo15Name,
			EDeviceParameterGoogolGTS_Ext3OutputName,
			EDeviceParameterGoogolGTS_Ext3OutputNo0Name,
			EDeviceParameterGoogolGTS_Ext3OutputNo1Name,
			EDeviceParameterGoogolGTS_Ext3OutputNo2Name,
			EDeviceParameterGoogolGTS_Ext3OutputNo3Name,
			EDeviceParameterGoogolGTS_Ext3OutputNo4Name,
			EDeviceParameterGoogolGTS_Ext3OutputNo5Name,
			EDeviceParameterGoogolGTS_Ext3OutputNo6Name,
			EDeviceParameterGoogolGTS_Ext3OutputNo7Name,
			EDeviceParameterGoogolGTS_Ext3OutputNo8Name,
			EDeviceParameterGoogolGTS_Ext3OutputNo9Name,
			EDeviceParameterGoogolGTS_Ext3OutputNo10Name,
			EDeviceParameterGoogolGTS_Ext3OutputNo11Name,
			EDeviceParameterGoogolGTS_Ext3OutputNo12Name,
			EDeviceParameterGoogolGTS_Ext3OutputNo13Name,
			EDeviceParameterGoogolGTS_Ext3OutputNo14Name,
			EDeviceParameterGoogolGTS_Ext3OutputNo15Name,
			EDeviceParameterGoogolGTS_Ext4OutputName,
			EDeviceParameterGoogolGTS_Ext4OutputNo0Name,
			EDeviceParameterGoogolGTS_Ext4OutputNo1Name,
			EDeviceParameterGoogolGTS_Ext4OutputNo2Name,
			EDeviceParameterGoogolGTS_Ext4OutputNo3Name,
			EDeviceParameterGoogolGTS_Ext4OutputNo4Name,
			EDeviceParameterGoogolGTS_Ext4OutputNo5Name,
			EDeviceParameterGoogolGTS_Ext4OutputNo6Name,
			EDeviceParameterGoogolGTS_Ext4OutputNo7Name,
			EDeviceParameterGoogolGTS_Ext4OutputNo8Name,
			EDeviceParameterGoogolGTS_Ext4OutputNo9Name,
			EDeviceParameterGoogolGTS_Ext4OutputNo10Name,
			EDeviceParameterGoogolGTS_Ext4OutputNo11Name,
			EDeviceParameterGoogolGTS_Ext4OutputNo12Name,
			EDeviceParameterGoogolGTS_Ext4OutputNo13Name,
			EDeviceParameterGoogolGTS_Ext4OutputNo14Name,
			EDeviceParameterGoogolGTS_Ext4OutputNo15Name,
			EDeviceParameterGoogolGTS_Ext5OutputName,
			EDeviceParameterGoogolGTS_Ext5OutputNo0Name,
			EDeviceParameterGoogolGTS_Ext5OutputNo1Name,
			EDeviceParameterGoogolGTS_Ext5OutputNo2Name,
			EDeviceParameterGoogolGTS_Ext5OutputNo3Name,
			EDeviceParameterGoogolGTS_Ext5OutputNo4Name,
			EDeviceParameterGoogolGTS_Ext5OutputNo5Name,
			EDeviceParameterGoogolGTS_Ext5OutputNo6Name,
			EDeviceParameterGoogolGTS_Ext5OutputNo7Name,
			EDeviceParameterGoogolGTS_Ext5OutputNo8Name,
			EDeviceParameterGoogolGTS_Ext5OutputNo9Name,
			EDeviceParameterGoogolGTS_Ext5OutputNo10Name,
			EDeviceParameterGoogolGTS_Ext5OutputNo11Name,
			EDeviceParameterGoogolGTS_Ext5OutputNo12Name,
			EDeviceParameterGoogolGTS_Ext5OutputNo13Name,
			EDeviceParameterGoogolGTS_Ext5OutputNo14Name,
			EDeviceParameterGoogolGTS_Ext5OutputNo15Name,
			EDeviceParameterGoogolGTS_Ext6OutputName,
			EDeviceParameterGoogolGTS_Ext6OutputNo0Name,
			EDeviceParameterGoogolGTS_Ext6OutputNo1Name,
			EDeviceParameterGoogolGTS_Ext6OutputNo2Name,
			EDeviceParameterGoogolGTS_Ext6OutputNo3Name,
			EDeviceParameterGoogolGTS_Ext6OutputNo4Name,
			EDeviceParameterGoogolGTS_Ext6OutputNo5Name,
			EDeviceParameterGoogolGTS_Ext6OutputNo6Name,
			EDeviceParameterGoogolGTS_Ext6OutputNo7Name,
			EDeviceParameterGoogolGTS_Ext6OutputNo8Name,
			EDeviceParameterGoogolGTS_Ext6OutputNo9Name,
			EDeviceParameterGoogolGTS_Ext6OutputNo10Name,
			EDeviceParameterGoogolGTS_Ext6OutputNo11Name,
			EDeviceParameterGoogolGTS_Ext6OutputNo12Name,
			EDeviceParameterGoogolGTS_Ext6OutputNo13Name,
			EDeviceParameterGoogolGTS_Ext6OutputNo14Name,
			EDeviceParameterGoogolGTS_Ext6OutputNo15Name,
			EDeviceParameterGoogolGTS_Ext7OutputName,
			EDeviceParameterGoogolGTS_Ext7OutputNo0Name,
			EDeviceParameterGoogolGTS_Ext7OutputNo1Name,
			EDeviceParameterGoogolGTS_Ext7OutputNo2Name,
			EDeviceParameterGoogolGTS_Ext7OutputNo3Name,
			EDeviceParameterGoogolGTS_Ext7OutputNo4Name,
			EDeviceParameterGoogolGTS_Ext7OutputNo5Name,
			EDeviceParameterGoogolGTS_Ext7OutputNo6Name,
			EDeviceParameterGoogolGTS_Ext7OutputNo7Name,
			EDeviceParameterGoogolGTS_Ext7OutputNo8Name,
			EDeviceParameterGoogolGTS_Ext7OutputNo9Name,
			EDeviceParameterGoogolGTS_Ext7OutputNo10Name,
			EDeviceParameterGoogolGTS_Ext7OutputNo11Name,
			EDeviceParameterGoogolGTS_Ext7OutputNo12Name,
			EDeviceParameterGoogolGTS_Ext7OutputNo13Name,
			EDeviceParameterGoogolGTS_Ext7OutputNo14Name,
			EDeviceParameterGoogolGTS_Ext7OutputNo15Name,
			EDeviceParameterGoogolGTS_Ext8OutputName,
			EDeviceParameterGoogolGTS_Ext8OutputNo0Name,
			EDeviceParameterGoogolGTS_Ext8OutputNo1Name,
			EDeviceParameterGoogolGTS_Ext8OutputNo2Name,
			EDeviceParameterGoogolGTS_Ext8OutputNo3Name,
			EDeviceParameterGoogolGTS_Ext8OutputNo4Name,
			EDeviceParameterGoogolGTS_Ext8OutputNo5Name,
			EDeviceParameterGoogolGTS_Ext8OutputNo6Name,
			EDeviceParameterGoogolGTS_Ext8OutputNo7Name,
			EDeviceParameterGoogolGTS_Ext8OutputNo8Name,
			EDeviceParameterGoogolGTS_Ext8OutputNo9Name,
			EDeviceParameterGoogolGTS_Ext8OutputNo10Name,
			EDeviceParameterGoogolGTS_Ext8OutputNo11Name,
			EDeviceParameterGoogolGTS_Ext8OutputNo12Name,
			EDeviceParameterGoogolGTS_Ext8OutputNo13Name,
			EDeviceParameterGoogolGTS_Ext8OutputNo14Name,
			EDeviceParameterGoogolGTS_Ext8OutputNo15Name,
			EDeviceParameterGoogolGTS_Ext9OutputName,
			EDeviceParameterGoogolGTS_Ext9OutputNo0Name,
			EDeviceParameterGoogolGTS_Ext9OutputNo1Name,
			EDeviceParameterGoogolGTS_Ext9OutputNo2Name,
			EDeviceParameterGoogolGTS_Ext9OutputNo3Name,
			EDeviceParameterGoogolGTS_Ext9OutputNo4Name,
			EDeviceParameterGoogolGTS_Ext9OutputNo5Name,
			EDeviceParameterGoogolGTS_Ext9OutputNo6Name,
			EDeviceParameterGoogolGTS_Ext9OutputNo7Name,
			EDeviceParameterGoogolGTS_Ext9OutputNo8Name,
			EDeviceParameterGoogolGTS_Ext9OutputNo9Name,
			EDeviceParameterGoogolGTS_Ext9OutputNo10Name,
			EDeviceParameterGoogolGTS_Ext9OutputNo11Name,
			EDeviceParameterGoogolGTS_Ext9OutputNo12Name,
			EDeviceParameterGoogolGTS_Ext9OutputNo13Name,
			EDeviceParameterGoogolGTS_Ext9OutputNo14Name,
			EDeviceParameterGoogolGTS_Ext9OutputNo15Name,
			EDeviceParameterGoogolGTS_Ext10OutputName,
			EDeviceParameterGoogolGTS_Ext10OutputNo0Name,
			EDeviceParameterGoogolGTS_Ext10OutputNo1Name,
			EDeviceParameterGoogolGTS_Ext10OutputNo2Name,
			EDeviceParameterGoogolGTS_Ext10OutputNo3Name,
			EDeviceParameterGoogolGTS_Ext10OutputNo4Name,
			EDeviceParameterGoogolGTS_Ext10OutputNo5Name,
			EDeviceParameterGoogolGTS_Ext10OutputNo6Name,
			EDeviceParameterGoogolGTS_Ext10OutputNo7Name,
			EDeviceParameterGoogolGTS_Ext10OutputNo8Name,
			EDeviceParameterGoogolGTS_Ext10OutputNo9Name,
			EDeviceParameterGoogolGTS_Ext10OutputNo10Name,
			EDeviceParameterGoogolGTS_Ext10OutputNo11Name,
			EDeviceParameterGoogolGTS_Ext10OutputNo12Name,
			EDeviceParameterGoogolGTS_Ext10OutputNo13Name,
			EDeviceParameterGoogolGTS_Ext10OutputNo14Name,
			EDeviceParameterGoogolGTS_Ext10OutputNo15Name,
			EDeviceParameterGoogolGTS_Ext11OutputName,
			EDeviceParameterGoogolGTS_Ext11OutputNo0Name,
			EDeviceParameterGoogolGTS_Ext11OutputNo1Name,
			EDeviceParameterGoogolGTS_Ext11OutputNo2Name,
			EDeviceParameterGoogolGTS_Ext11OutputNo3Name,
			EDeviceParameterGoogolGTS_Ext11OutputNo4Name,
			EDeviceParameterGoogolGTS_Ext11OutputNo5Name,
			EDeviceParameterGoogolGTS_Ext11OutputNo6Name,
			EDeviceParameterGoogolGTS_Ext11OutputNo7Name,
			EDeviceParameterGoogolGTS_Ext11OutputNo8Name,
			EDeviceParameterGoogolGTS_Ext11OutputNo9Name,
			EDeviceParameterGoogolGTS_Ext11OutputNo10Name,
			EDeviceParameterGoogolGTS_Ext11OutputNo11Name,
			EDeviceParameterGoogolGTS_Ext11OutputNo12Name,
			EDeviceParameterGoogolGTS_Ext11OutputNo13Name,
			EDeviceParameterGoogolGTS_Ext11OutputNo14Name,
			EDeviceParameterGoogolGTS_Ext11OutputNo15Name,
			EDeviceParameterGoogolGTS_Ext12OutputName,
			EDeviceParameterGoogolGTS_Ext12OutputNo0Name,
			EDeviceParameterGoogolGTS_Ext12OutputNo1Name,
			EDeviceParameterGoogolGTS_Ext12OutputNo2Name,
			EDeviceParameterGoogolGTS_Ext12OutputNo3Name,
			EDeviceParameterGoogolGTS_Ext12OutputNo4Name,
			EDeviceParameterGoogolGTS_Ext12OutputNo5Name,
			EDeviceParameterGoogolGTS_Ext12OutputNo6Name,
			EDeviceParameterGoogolGTS_Ext12OutputNo7Name,
			EDeviceParameterGoogolGTS_Ext12OutputNo8Name,
			EDeviceParameterGoogolGTS_Ext12OutputNo9Name,
			EDeviceParameterGoogolGTS_Ext12OutputNo10Name,
			EDeviceParameterGoogolGTS_Ext12OutputNo11Name,
			EDeviceParameterGoogolGTS_Ext12OutputNo12Name,
			EDeviceParameterGoogolGTS_Ext12OutputNo13Name,
			EDeviceParameterGoogolGTS_Ext12OutputNo14Name,
			EDeviceParameterGoogolGTS_Ext12OutputNo15Name,
			EDeviceParameterGoogolGTS_Ext13OutputName,
			EDeviceParameterGoogolGTS_Ext13OutputNo0Name,
			EDeviceParameterGoogolGTS_Ext13OutputNo1Name,
			EDeviceParameterGoogolGTS_Ext13OutputNo2Name,
			EDeviceParameterGoogolGTS_Ext13OutputNo3Name,
			EDeviceParameterGoogolGTS_Ext13OutputNo4Name,
			EDeviceParameterGoogolGTS_Ext13OutputNo5Name,
			EDeviceParameterGoogolGTS_Ext13OutputNo6Name,
			EDeviceParameterGoogolGTS_Ext13OutputNo7Name,
			EDeviceParameterGoogolGTS_Ext13OutputNo8Name,
			EDeviceParameterGoogolGTS_Ext13OutputNo9Name,
			EDeviceParameterGoogolGTS_Ext13OutputNo10Name,
			EDeviceParameterGoogolGTS_Ext13OutputNo11Name,
			EDeviceParameterGoogolGTS_Ext13OutputNo12Name,
			EDeviceParameterGoogolGTS_Ext13OutputNo13Name,
			EDeviceParameterGoogolGTS_Ext13OutputNo14Name,
			EDeviceParameterGoogolGTS_Ext13OutputNo15Name,
			EDeviceParameterGoogolGTS_Ext14OutputName,
			EDeviceParameterGoogolGTS_Ext14OutputNo0Name,
			EDeviceParameterGoogolGTS_Ext14OutputNo1Name,
			EDeviceParameterGoogolGTS_Ext14OutputNo2Name,
			EDeviceParameterGoogolGTS_Ext14OutputNo3Name,
			EDeviceParameterGoogolGTS_Ext14OutputNo4Name,
			EDeviceParameterGoogolGTS_Ext14OutputNo5Name,
			EDeviceParameterGoogolGTS_Ext14OutputNo6Name,
			EDeviceParameterGoogolGTS_Ext14OutputNo7Name,
			EDeviceParameterGoogolGTS_Ext14OutputNo8Name,
			EDeviceParameterGoogolGTS_Ext14OutputNo9Name,
			EDeviceParameterGoogolGTS_Ext14OutputNo10Name,
			EDeviceParameterGoogolGTS_Ext14OutputNo11Name,
			EDeviceParameterGoogolGTS_Ext14OutputNo12Name,
			EDeviceParameterGoogolGTS_Ext14OutputNo13Name,
			EDeviceParameterGoogolGTS_Ext14OutputNo14Name,
			EDeviceParameterGoogolGTS_Ext14OutputNo15Name,
			EDeviceParameterGoogolGTS_Ext15OutputName,
			EDeviceParameterGoogolGTS_Ext15OutputNo0Name,
			EDeviceParameterGoogolGTS_Ext15OutputNo1Name,
			EDeviceParameterGoogolGTS_Ext15OutputNo2Name,
			EDeviceParameterGoogolGTS_Ext15OutputNo3Name,
			EDeviceParameterGoogolGTS_Ext15OutputNo4Name,
			EDeviceParameterGoogolGTS_Ext15OutputNo5Name,
			EDeviceParameterGoogolGTS_Ext15OutputNo6Name,
			EDeviceParameterGoogolGTS_Ext15OutputNo7Name,
			EDeviceParameterGoogolGTS_Ext15OutputNo8Name,
			EDeviceParameterGoogolGTS_Ext15OutputNo9Name,
			EDeviceParameterGoogolGTS_Ext15OutputNo10Name,
			EDeviceParameterGoogolGTS_Ext15OutputNo11Name,
			EDeviceParameterGoogolGTS_Ext15OutputNo12Name,
			EDeviceParameterGoogolGTS_Ext15OutputNo13Name,
			EDeviceParameterGoogolGTS_Ext15OutputNo14Name,
			EDeviceParameterGoogolGTS_Ext15OutputNo15Name,
			EDeviceParameterGoogolGTS_Ext16OutputName,
			EDeviceParameterGoogolGTS_Ext16OutputNo0Name,
			EDeviceParameterGoogolGTS_Ext16OutputNo1Name,
			EDeviceParameterGoogolGTS_Ext16OutputNo2Name,
			EDeviceParameterGoogolGTS_Ext16OutputNo3Name,
			EDeviceParameterGoogolGTS_Ext16OutputNo4Name,
			EDeviceParameterGoogolGTS_Ext16OutputNo5Name,
			EDeviceParameterGoogolGTS_Ext16OutputNo6Name,
			EDeviceParameterGoogolGTS_Ext16OutputNo7Name,
			EDeviceParameterGoogolGTS_Ext16OutputNo8Name,
			EDeviceParameterGoogolGTS_Ext16OutputNo9Name,
			EDeviceParameterGoogolGTS_Ext16OutputNo10Name,
			EDeviceParameterGoogolGTS_Ext16OutputNo11Name,
			EDeviceParameterGoogolGTS_Ext16OutputNo12Name,
			EDeviceParameterGoogolGTS_Ext16OutputNo13Name,
			EDeviceParameterGoogolGTS_Ext16OutputNo14Name,
			EDeviceParameterGoogolGTS_Ext16OutputNo15Name,
			EDeviceParameterGoogolGTS_Count,
		};

		enum EDeviceParameterSoftservoWMX2
		{
			EDeviceParameterSoftservoWMX2_DeviceID = 0,
			EDeviceParameterSoftservoWMX2_MotionInfoPath,
			EDeviceParameterSoftservoWMX2_UnitPulse,
			EDeviceParameterSoftservoWMX2_StartSpeed,
			EDeviceParameterSoftservoWMX2_EndSpeed,
			EDeviceParameterSoftservoWMX2_HomeLevel,
			EDeviceParameterSoftservoWMX2_PolarityLimitLevel,
			EDeviceParameterSoftservoWMX2_PosLimitLevel,
			EDeviceParameterSoftservoWMX2_NegLimitLevel,
			EDeviceParameterSoftservoWMX2_GearRatioDenominator,
			EDeviceParameterSoftservoWMX2_GearRatioNumerator,
			EDeviceParameterSoftservoWMX2_InposWidth,
			EDeviceParameterSoftservoWMX2_PosCompletedWidth,
			EDeviceParameterSoftservoWMX2_Home,
			EDeviceParameterSoftservoWMX2_HomeType,
			EDeviceParameterSoftservoWMX2_HomeDirection,
			EDeviceParameterSoftservoWMX2_HomeFastVelocity,
			EDeviceParameterSoftservoWMX2_HomeFastAccel,
			EDeviceParameterSoftservoWMX2_HomeFastDeceleration,
			EDeviceParameterSoftservoWMX2_HomeSlowVelocity,
			EDeviceParameterSoftservoWMX2_HomeSlowAccel,
			EDeviceParameterSoftservoWMX2_HomeSlowDeceleration,
			EDeviceParameterSoftservoWMX2_HomeShiftVelocity,
			EDeviceParameterSoftservoWMX2_HomeShiftAccel,
			EDeviceParameterSoftservoWMX2_HomeShiftDeceleration,
			EDeviceParameterSoftservoWMX2_HomeShiftDistance,
			EDeviceParameterSoftservoWMX2_Count,
		};

		enum EDeviceParameterSoftservoWMX3
		{
			EDeviceParameterSoftservoWMX3_DeviceID = 0,
			EDeviceParameterSoftservoWMX3_InitializeType,
			EDeviceParameterSoftservoWMX3_MotionInfoPath,
			EDeviceParameterSoftservoWMX3_ParamPath,
			EDeviceParameterSoftservoWMX3_StartSpeed,
			EDeviceParameterSoftservoWMX3_EndSpeed,
			EDeviceParameterSoftservoWMX3_Level,
			EDeviceParameterSoftservoWMX3_HomeLevel,
			EDeviceParameterSoftservoWMX3_PosLimitLevel,
			EDeviceParameterSoftservoWMX3_NearPosLimitLevel,
			EDeviceParameterSoftservoWMX3_ExternPosLimitLevel,
			EDeviceParameterSoftservoWMX3_NegLimitLevel,
			EDeviceParameterSoftservoWMX3_NearNegLimitLevel,
			EDeviceParameterSoftservoWMX3_ExternNegLimitLevel,
			EDeviceParameterSoftservoWMX3_GearRatioDenominator,
			EDeviceParameterSoftservoWMX3_GearRatioNumerator,
			EDeviceParameterSoftservoWMX3_InposWidth,
			EDeviceParameterSoftservoWMX3_PosCompletedWidth,
			EDeviceParameterSoftservoWMX3_Home,
			EDeviceParameterSoftservoWMX3_HomeType,
			EDeviceParameterSoftservoWMX3_HomeDirection,
			EDeviceParameterSoftservoWMX3_HomeFastVelocity,
			EDeviceParameterSoftservoWMX3_HomeFastAccel,
			EDeviceParameterSoftservoWMX3_HomeFastDeceleration,
			EDeviceParameterSoftservoWMX3_HomeSlowVelocity,
			EDeviceParameterSoftservoWMX3_HomeSlowAccel,
			EDeviceParameterSoftservoWMX3_HomeSlowDeceleration,
			EDeviceParameterSoftservoWMX3_HomeShiftVelocity,
			EDeviceParameterSoftservoWMX3_HomeShiftAccel,
			EDeviceParameterSoftservoWMX3_HomeShiftDeceleration,
			EDeviceParameterSoftservoWMX3_HomeShiftDistance,
			EDeviceParameterSoftservoWMX3_Count,
		};

		enum EDeviceParameterMVTechMSPDio
		{
			EDeviceParameterMVTechMSPDio_DeviceID = 0,
			EDeviceParameterMVTechMSPDio_BoardType,
			EDeviceParameterMVTechMSPDio_InputStatus,
			EDeviceParameterMVTechMSPDio_UseInputNo0,
			EDeviceParameterMVTechMSPDio_UseInputNo1,
			EDeviceParameterMVTechMSPDio_UseInputNo2,
			EDeviceParameterMVTechMSPDio_UseInputNo3,
			EDeviceParameterMVTechMSPDio_UseInputNo4,
			EDeviceParameterMVTechMSPDio_UseInputNo5,
			EDeviceParameterMVTechMSPDio_UseInputNo6,
			EDeviceParameterMVTechMSPDio_UseInputNo7,
			EDeviceParameterMVTechMSPDio_UseInputNo8,
			EDeviceParameterMVTechMSPDio_UseInputNo9,
			EDeviceParameterMVTechMSPDio_UseInputNo10,
			EDeviceParameterMVTechMSPDio_UseInputNo11,
			EDeviceParameterMVTechMSPDio_UseInputNo12,
			EDeviceParameterMVTechMSPDio_UseInputNo13,
			EDeviceParameterMVTechMSPDio_UseInputNo14,
			EDeviceParameterMVTechMSPDio_UseInputNo15,
			EDeviceParameterMVTechMSPDio_UseInputNo16,
			EDeviceParameterMVTechMSPDio_UseInputNo17,
			EDeviceParameterMVTechMSPDio_UseInputNo18,
			EDeviceParameterMVTechMSPDio_UseInputNo19,
			EDeviceParameterMVTechMSPDio_UseInputNo20,
			EDeviceParameterMVTechMSPDio_UseInputNo21,
			EDeviceParameterMVTechMSPDio_UseInputNo22,
			EDeviceParameterMVTechMSPDio_UseInputNo23,
			EDeviceParameterMVTechMSPDio_UseInputNo24,
			EDeviceParameterMVTechMSPDio_UseInputNo25,
			EDeviceParameterMVTechMSPDio_UseInputNo26,
			EDeviceParameterMVTechMSPDio_UseInputNo27,
			EDeviceParameterMVTechMSPDio_UseInputNo28,
			EDeviceParameterMVTechMSPDio_UseInputNo29,
			EDeviceParameterMVTechMSPDio_UseInputNo30,
			EDeviceParameterMVTechMSPDio_UseInputNo31,
			EDeviceParameterMVTechMSPDio_OutputStatus,
			EDeviceParameterMVTechMSPDio_UseOutputNo0,
			EDeviceParameterMVTechMSPDio_UseOutputNo1,
			EDeviceParameterMVTechMSPDio_UseOutputNo2,
			EDeviceParameterMVTechMSPDio_UseOutputNo3,
			EDeviceParameterMVTechMSPDio_UseOutputNo4,
			EDeviceParameterMVTechMSPDio_UseOutputNo5,
			EDeviceParameterMVTechMSPDio_UseOutputNo6,
			EDeviceParameterMVTechMSPDio_UseOutputNo7,
			EDeviceParameterMVTechMSPDio_UseOutputNo8,
			EDeviceParameterMVTechMSPDio_UseOutputNo9,
			EDeviceParameterMVTechMSPDio_UseOutputNo10,
			EDeviceParameterMVTechMSPDio_UseOutputNo11,
			EDeviceParameterMVTechMSPDio_UseOutputNo12,
			EDeviceParameterMVTechMSPDio_UseOutputNo13,
			EDeviceParameterMVTechMSPDio_UseOutputNo14,
			EDeviceParameterMVTechMSPDio_UseOutputNo15,
			EDeviceParameterMVTechMSPDio_UseOutputNo16,
			EDeviceParameterMVTechMSPDio_UseOutputNo17,
			EDeviceParameterMVTechMSPDio_UseOutputNo18,
			EDeviceParameterMVTechMSPDio_UseOutputNo19,
			EDeviceParameterMVTechMSPDio_UseOutputNo20,
			EDeviceParameterMVTechMSPDio_UseOutputNo21,
			EDeviceParameterMVTechMSPDio_UseOutputNo22,
			EDeviceParameterMVTechMSPDio_UseOutputNo23,
			EDeviceParameterMVTechMSPDio_UseOutputNo24,
			EDeviceParameterMVTechMSPDio_UseOutputNo25,
			EDeviceParameterMVTechMSPDio_UseOutputNo26,
			EDeviceParameterMVTechMSPDio_UseOutputNo27,
			EDeviceParameterMVTechMSPDio_UseOutputNo28,
			EDeviceParameterMVTechMSPDio_UseOutputNo29,
			EDeviceParameterMVTechMSPDio_UseOutputNo30,
			EDeviceParameterMVTechMSPDio_UseOutputNo31,
			EDeviceParameterMVTechMSPDio_InputName,
			EDeviceParameterMVTechMSPDio_InputNo0Name,
			EDeviceParameterMVTechMSPDio_InputNo1Name,
			EDeviceParameterMVTechMSPDio_InputNo2Name,
			EDeviceParameterMVTechMSPDio_InputNo3Name,
			EDeviceParameterMVTechMSPDio_InputNo4Name,
			EDeviceParameterMVTechMSPDio_InputNo5Name,
			EDeviceParameterMVTechMSPDio_InputNo6Name,
			EDeviceParameterMVTechMSPDio_InputNo7Name,
			EDeviceParameterMVTechMSPDio_InputNo8Name,
			EDeviceParameterMVTechMSPDio_InputNo9Name,
			EDeviceParameterMVTechMSPDio_InputNo10Name,
			EDeviceParameterMVTechMSPDio_InputNo11Name,
			EDeviceParameterMVTechMSPDio_InputNo12Name,
			EDeviceParameterMVTechMSPDio_InputNo13Name,
			EDeviceParameterMVTechMSPDio_InputNo14Name,
			EDeviceParameterMVTechMSPDio_InputNo15Name,
			EDeviceParameterMVTechMSPDio_InputNo16Name,
			EDeviceParameterMVTechMSPDio_InputNo17Name,
			EDeviceParameterMVTechMSPDio_InputNo18Name,
			EDeviceParameterMVTechMSPDio_InputNo19Name,
			EDeviceParameterMVTechMSPDio_InputNo20Name,
			EDeviceParameterMVTechMSPDio_InputNo21Name,
			EDeviceParameterMVTechMSPDio_InputNo22Name,
			EDeviceParameterMVTechMSPDio_InputNo23Name,
			EDeviceParameterMVTechMSPDio_InputNo24Name,
			EDeviceParameterMVTechMSPDio_InputNo25Name,
			EDeviceParameterMVTechMSPDio_InputNo26Name,
			EDeviceParameterMVTechMSPDio_InputNo27Name,
			EDeviceParameterMVTechMSPDio_InputNo28Name,
			EDeviceParameterMVTechMSPDio_InputNo29Name,
			EDeviceParameterMVTechMSPDio_InputNo30Name,
			EDeviceParameterMVTechMSPDio_InputNo31Name,
			EDeviceParameterMVTechMSPDio_OutputName,
			EDeviceParameterMVTechMSPDio_OutputNo0Name,
			EDeviceParameterMVTechMSPDio_OutputNo1Name,
			EDeviceParameterMVTechMSPDio_OutputNo2Name,
			EDeviceParameterMVTechMSPDio_OutputNo3Name,
			EDeviceParameterMVTechMSPDio_OutputNo4Name,
			EDeviceParameterMVTechMSPDio_OutputNo5Name,
			EDeviceParameterMVTechMSPDio_OutputNo6Name,
			EDeviceParameterMVTechMSPDio_OutputNo7Name,
			EDeviceParameterMVTechMSPDio_OutputNo8Name,
			EDeviceParameterMVTechMSPDio_OutputNo9Name,
			EDeviceParameterMVTechMSPDio_OutputNo10Name,
			EDeviceParameterMVTechMSPDio_OutputNo11Name,
			EDeviceParameterMVTechMSPDio_OutputNo12Name,
			EDeviceParameterMVTechMSPDio_OutputNo13Name,
			EDeviceParameterMVTechMSPDio_OutputNo14Name,
			EDeviceParameterMVTechMSPDio_OutputNo15Name,
			EDeviceParameterMVTechMSPDio_OutputNo16Name,
			EDeviceParameterMVTechMSPDio_OutputNo17Name,
			EDeviceParameterMVTechMSPDio_OutputNo18Name,
			EDeviceParameterMVTechMSPDio_OutputNo19Name,
			EDeviceParameterMVTechMSPDio_OutputNo20Name,
			EDeviceParameterMVTechMSPDio_OutputNo21Name,
			EDeviceParameterMVTechMSPDio_OutputNo22Name,
			EDeviceParameterMVTechMSPDio_OutputNo23Name,
			EDeviceParameterMVTechMSPDio_OutputNo24Name,
			EDeviceParameterMVTechMSPDio_OutputNo25Name,
			EDeviceParameterMVTechMSPDio_OutputNo26Name,
			EDeviceParameterMVTechMSPDio_OutputNo27Name,
			EDeviceParameterMVTechMSPDio_OutputNo28Name,
			EDeviceParameterMVTechMSPDio_OutputNo29Name,
			EDeviceParameterMVTechMSPDio_OutputNo30Name,
			EDeviceParameterMVTechMSPDio_OutputNo31Name,
			EDeviceParameterMVTechMSPDio_Count,
		};

		enum EDeviceHIKVisionGigEGetFunction
		{
			EDeviceHIKVisionGigEGetFunction_OK = 0,
			EDeviceHIKVisionGigEGetFunction_UnknownError,
			EDeviceHIKVisionGigEGetFunction_EnumTypeError,
			EDeviceHIKVisionGigEGetFunction_NullptrError,
		};

		enum EDeviceHIKVisionGigELineSelect
		{
			EDeviceHIKVisionGigELineSelect_eLine0 = 0,
			EDeviceHIKVisionGigELineSelect_eLine1,
			EDeviceHIKVisionGigELineSelect_eLine2,
			EDeviceHIKVisionGigELineSelect_Count,
		};

		enum EDeviceHIKVisionGigEModel
		{
			EDeviceHIKVisionGigEModel_MV_CA050_10GM = 0,
			EDeviceHIKVisionGigEModel_MV_CA013_20GM,
			EDeviceHIKVisionGigEModel_Count,

		};

		enum EDeviceHIKVisionGigEHardTriggerLineSelect
		{
			EDeviceHIKVisionGigEHardTriggerLineSelect_Line0 = 0,
			EDeviceHIKVisionGigEHardTriggerLineSelect_Line2,
			EDeviceHIKVisionGigEHardTriggerLineSelect_Count,
		};

		enum EDeviceHIKVisionGigEInitMode
		{
			EDeviceHIKVisionGigEInitMode_DeviceOnly = 0,
			EDeviceHIKVisionGigEInitMode_Parameter,
			EDeviceHIKVisionGigEInitMode_Count,
		};

		enum EDeviceParameterHIKVisionGigE
		{
			//DeviceControl
			EDeviceParameterHIKVisionGigE_DeviceID = 0,
			EDeviceParameterHIKVisionGigE_InitializeMode,
			//Initialize Mode
			EDeviceParameterHIKVisionGigE_DeviceControl,
			EDeviceParameterHIKVisionGigE_DeviceStreamChannelPackSize,
			EDeviceParameterHIKVisionGigE_DeviceGrabCount,

			//ImageFormatControl
			EDeviceParameterHIKVisionGigE_ImageFormatControl,
			EDeviceParameterHIKVisionGigE_Width,
			EDeviceParameterHIKVisionGigE_Height,
			EDeviceParameterHIKVisionGigE_OffsetX,
			EDeviceParameterHIKVisionGigE_OffsetY,
			EDeviceParameterHIKVisionGigE_ReverseX,
			EDeviceParameterHIKVisionGigE_ReverseY,
			EDeviceParameterHIKVisionGigE_PixelFormat,

			//AcquisitionControl
			EDeviceParameterHIKVisionGigE_AcquisitionControl,
			EDeviceParameterHIKVisionGigE_AcquisitionMode,
			EDeviceParameterHIKVisionGigE_AcquisitionBurstFrameCount,
			EDeviceParameterHIKVisionGigE_AcquisitionFrameRate,
			EDeviceParameterHIKVisionGigE_AcquisitionFrameRateControlEnable,
			EDeviceParameterHIKVisionGigE_AcquisitionTriggerSelector,
			EDeviceParameterHIKVisionGigE_AcquisitionTriggerMode,
			EDeviceParameterHIKVisionGigE_AcquisitionTriggerSource,
			EDeviceParameterHIKVisionGigE_AcquisitionTriggerActivation,
			EDeviceParameterHIKVisionGigE_AcquisitionTriggerDelay,
			EDeviceParameterHIKVisionGigE_AcquisitionExposureMode,
			EDeviceParameterHIKVisionGigE_AcquisitionExposureTime,
			EDeviceParameterHIKVisionGigE_AcquisitionExposureAuto,

			//Analog Control
			EDeviceParameterHIKVisionGigE_AnalogControl,
			EDeviceParameterHIKVisionGigE_AnalogGaindB,
			EDeviceParameterHIKVisionGigE_AnalogGainAuto,
			EDeviceParameterHIKVisionGigE_AnalogDigitalShift,
			EDeviceParameterHIKVisionGigE_AnalogDigitalShiftEnable,
			EDeviceParameterHIKVisionGigE_AnalogBlackLevel,
			EDeviceParameterHIKVisionGigE_AnalogBlackLevelEnable,

			//Digital IO Control
			EDeviceParameterHIKVisionGigE_DigitalIOControl,
			EDeviceParameterHIKVisionGigE_DigitalLineSelector,

			EDeviceParameterHIKVisionGigE_DigitalLine0,
			EDeviceParameterHIKVisionGigE_DigitalLine0Mode,
			EDeviceParameterHIKVisionGigE_DigitalLine0DebouncerTime,

			EDeviceParameterHIKVisionGigE_DigitalLine1,
			EDeviceParameterHIKVisionGigE_DigitalLine1Mode,
			EDeviceParameterHIKVisionGigE_DigitalLine1Inverter,
			EDeviceParameterHIKVisionGigE_DigitalLine1Source,
			EDeviceParameterHIKVisionGigE_DigitalLine1StrobeEnable,
			EDeviceParameterHIKVisionGigE_DigitalLine1StrobeLineDuration,
			EDeviceParameterHIKVisionGigE_DigitalLine1StrobeLineDelay,
			EDeviceParameterHIKVisionGigE_DigitalLine1StrobeLinePreDelay,

			EDeviceParameterHIKVisionGigE_DigitalLine1StrobeHardTriggerActiveMode,
			EDeviceParameterHIKVisionGigE_DigitalLine1StrobeHardTriggerSource,
			EDeviceParameterHIKVisionGigE_DigitalLine1StrobeHardTriggerActivaion,




			EDeviceParameterHIKVisionGigE_DigitalLine2,
			EDeviceParameterHIKVisionGigE_DigitalLine2Mode,
			EDeviceParameterHIKVisionGigE_DigitalLine2Input,
			EDeviceParameterHIKVisionGigE_DigitalLine2DebouncerTime,
			EDeviceParameterHIKVisionGigE_DigitalLine2Strob,
			EDeviceParameterHIKVisionGigE_DigitalLine2Inverter,
			EDeviceParameterHIKVisionGigE_DigitalLine2Source,
			EDeviceParameterHIKVisionGigE_DigitalLine2StrobeEnable,
			EDeviceParameterHIKVisionGigE_DigitalLine2StrobeLineDuration,
			EDeviceParameterHIKVisionGigE_DigitalLine2StrobeLineDelay,
			EDeviceParameterHIKVisionGigE_DigitalLine2StrobeLinePreDelay,

			EDeviceParameterHIKVisionGigE_DigitalLine2StrobeHardTriggerActiveMode,
			EDeviceParameterHIKVisionGigE_DigitalLine2StrobeHardTriggerSource,
			EDeviceParameterHIKVisionGigE_DigitalLine2StrobeHardTriggerActivaion,

			EDeviceParameterHIKVisionGigE_Count,
		};

		enum EDeviceParameterAxlTrigger
		{
			EDeviceParameterAxlTrigger_DeviceID = 0,
			EDeviceParameterAxlTrigger_SubUnitID,
			EDeviceParameterAxlTrigger_UnitPerPulse,
			EDeviceParameterAxlTrigger_TriggerFunction,
			EDeviceParameterAxlTrigger_SignalSetting,
			EDeviceParameterAxlTrigger_EncoderInputMethod,
			EDeviceParameterAxlTrigger_EncoderSource,
			EDeviceParameterAxlTrigger_EncoderReverse,
			EDeviceParameterAxlTrigger_TriggerSetting,
			EDeviceParameterAxlTrigger_TriggerMode,
			EDeviceParameterAxlTrigger_TriggerActiveLevel,
			EDeviceParameterAxlTrigger_TriggerDirectionCheck,
			EDeviceParameterAxlTrigger_TriggerPulseWidth,
			EDeviceParameterAxlTrigger_Periodic,
			EDeviceParameterAxlTrigger_StartPosition,
			EDeviceParameterAxlTrigger_EndPosition,
			EDeviceParameterAxlTrigger_Count,
		};

	}
}

// HIKVision
namespace Ravid
{
	namespace Device
	{
		enum EHIKVisionGetFunction
		{
			EHIKVisionGetFunction_OK = 0,
			EHIKVisionGetFunction_UnknownError,
			EHIKVisionGetFunction_NullptrError,
			EHIKVisionGetFunction_EnumTypeError,
		};

		enum EImagePixelFormat
		{
			eMono8 = 0x01080001,
			eMono10 = 0x01100003,
			eMono10Packed = 0x010C0004,
			eMono12 = 0x01100005,
			eMono12Packed = 0x010C0006,
			eRGB8packed = 0x02180014,
			eYUV422_YUYV_Packed = 0x02100032,
			eYUV422Packed = 0x0210001F,
			eBayerRG8 = 0x01080009,
			eBayerRG10 = 0x0110000d,
			eBayerRG10Packed = 0x010C0027,
			eBayerRG12 = 0x01100011,
			eBayerRG12Packed = 0x010C002B,
		};

		enum EDeviceHIKVisionAcquisitionMode
		{
			EDeviceHIKVisionAcquisitionMode_eContinuous = 0,
			EDeviceHIKVisionAcquisitionMode_eSingleFrame,
			EDeviceHIKVisionAcquisitionMode_Count,
		};

		enum EHIKVisionSetFunction
		{
			EHIKVisionSetFunction_OK = 0,
			EHIKVisionSetFunction_UnknownError,
			EHIKVisionSetFunction_NotFoundApiError,
			EHIKVisionSetFunction_AlreadyInitializedError,
			EHIKVisionSetFunction_NotInitializedError,
			EHIKVisionSetFunction_NotFindCommandError,
			EHIKVisionSetFunction_DeviceNullptrError,
			EHIKVisionSetFunction_NotSupportError,
			EHIKVisionSetFunction_FunctionLockedError,
			EHIKVisionSetFunction_WriteToDeviceError,
			EHIKVisionSetFunction_WriteToDatabaseError,
			EHIKVisionSetFunction_ReadOnDatabaseError,
			EHIKVisionSetFunction_ReadOnDeviceError,
			EHIKVisionSetFunction_ActivatedDeviceError,
		};

		enum EDeviceHIKVisionGigEPixelFormat
		{
			EDeviceHIKVisionGigEPixelFormat_Mono8 = 0,
			EDeviceHIKVisionGigEPixelFormat_Mono10,
			EDeviceHIKVisionGigEPixelFormat_Mono10packed,
			EDeviceHIKVisionGigEPixelFormat_Mono12,
			EDeviceHIKVisionGigEPixelFormat_Mono12packed,
			EDeviceHIKVisionGigEPixelFormat_RGB8packed,
			EDeviceHIKVisionGigEPixelFormat_YUV422_YUYV_Packed,
			EDeviceHIKVisionGigEPixelFormat_YUV422Packed,
			EDeviceHIKVisionGigEPixelFormat_BayerRG8,
			EDeviceHIKVisionGigEPixelFormat_BayerRG10,
			EDeviceHIKVisionGigEPixelFormat_BayerRG10Packed,
			EDeviceHIKVisionGigEPixelFormat_BayerRG12,
			EDeviceHIKVisionGigEPixelFormat_BayerRG12Packed,
			EDeviceHIKVisionGigEPixelFormat_Count,
		};

		enum EDeviceHIKVisionGigETriggerSelector
		{
			EDeviceHIKVisionGigETriggerSelector_FrameStart = 0,
			EDeviceHIKVisionGigETriggerSelector_LineStart,
			EDeviceHIKVisionGigETriggerSelector_AcquisitionStart,
			EDeviceHIKVisionGigETriggerSelector_Count,
		};

		enum EDeviceHIKVisionGigESwitch
		{
			EDeviceHIKVisionGigESwitch_Off = 0,
			EDeviceHIKVisionGigESwitch_On,
			EDeviceHIKVisionGigESwitch_Count,
		};

		enum EDeviceHIKVisionGigELineSource
		{
			EDeviceHIKVisionGigELineSource_ExposureStartActive = 0,
			EDeviceHIKVisionGigELineSource_AcquisitionStartActive,
			EDeviceHIKVisionGigELineSource_AcquisitionStopActive,
			EDeviceHIKVisionGigELineSource_FrameBurstStartActive,
			EDeviceHIKVisionGigELineSource_FrameBurstEndActive,
			EDeviceHIKVisionGigELineSource_SoftTriggerActive,
			EDeviceHIKVisionGigELineSource_HardTriggerActive,
			EDeviceHIKVisionGigELineSource_CounterActive,
			EDeviceHIKVisionGigELineSource_TimerActive,
			EDeviceHIKVisionGigELineSource_Count,
		};

		enum EDeviceHIKVisionGigETriggerActivation
		{
			EDeviceHIKVisionGigETriggerActivation_RisingEdge = 0,
			EDeviceHIKVisionGigETriggerActivation_FallingEdge,
			EDeviceHIKVisionGigETriggerActivation_Count,
		};

		enum EDeviceHIKVisionGigEGainAuto
		{
			EDeviceHIKVisionGigEGainAuto_Off = 0,
			EDeviceHIKVisionGigEGainAuto_Once,
			EDeviceHIKVisionGigEGainAuto_Continuous,
			EDeviceHIKVisionGigEGainAuto_Count,
		};

		enum EDeviceHIKVisionGigEAuto
		{
			EDeviceHIKVisionGigEAuto_Off = 0,
			EDeviceHIKVisionGigEAuto_Once,
			EDeviceHIKVisionGigEAuto_Continuous,
			EDeviceHIKVisionGigEAuto_Count,
		};

		enum EDeviceHIKVisionGigEGainSelector
		{
			EDeviceHIKVisionGigEGainSelector_All = 0,
			EDeviceHIKVisionGigEGainSelector_AnalogAll,
			EDeviceHIKVisionGigEGainSelector_DigitalAll,
			EDeviceHIKVisionGigEGainSelector_Tap1,
			EDeviceHIKVisionGigEGainSelector_Tap2,
			EDeviceHIKVisionGigEGainSelector_Count,
		};

		enum EDeviceHIKVisionGigELineSelector
		{
			EDeviceHIKVisionGigELineSelector_Line1 = 0,
			EDeviceHIKVisionGigELineSelector_Line2,
			EDeviceHIKVisionGigELineSelector_Line3,
			EDeviceHIKVisionGigELineSelector_Line4,
			EDeviceHIKVisionGigELineSelector_Count,
		};

		enum EDeviceHIKVisionGigELineMode
		{
			EDeviceHIKVisionGigELineMode_Input = 0,
			EDeviceHIKVisionGigELineMode_Strob,
			EDeviceHIKVisionGigELineMode_Count,
		};

		enum EDeviceHIKVisionGigEAcquisitionTriggerSelector
		{
			EDeviceHIKVisionGigEAcquisitionTriggerSelector_EFrameBurstStart = 0,
			EDeviceHIKVisionGigEAcquisitionTriggerSelector_Count,
		};


		enum EDeviceHIKVisionGigEAcquisitionTriggerSource
		{
			EDeviceHIKVisionGigETriggerSource_SoftWare = 0,
			EDeviceHIKVisionGigETriggerSource_Line0,
			EDeviceHIKVisionGigETriggerSource_Line2,
			EDeviceHIKVisionGigETriggerSource_Counter0,
			EDeviceHIKVisionGigETriggerSource_Count,
		};

		enum EDeviceHIKVisionGigEAcquisitionTriggerActivation
		{
			EDeviceHIKVisionGigEAcquisitionTriggerActivation_RisingEdge = 0,
			EDeviceHIKVisionGigEAcquisitionTriggerActivation_FallingEdge,
			EDeviceHIKVisionGigEAcquisitionTriggerActivation_LevelHigh,
			EDeviceHIKVisionGigEAcquisitionTriggerActivation_LevelLow,
			EDeviceHIKVisionGigEAcquisitionTriggerActivation_Count,
		};

		enum EDeviceHIKVisionGigEAcquisitionExposureMode
		{
			EDeviceHIKVisionGigEAcquisitionExposureMode_Timed = 0,
			EDeviceHIKVisionGigEAcquisitionExposureMode_Count
		};

		enum EDeviceHIKVisionGigELightSourceSelector
		{
			EDeviceHIKVisionGigELightSourceSelector_Off = 0,
			EDeviceHIKVisionGigELightSourceSelector_Custom,
			EDeviceHIKVisionGigELightSourceSelector_Daylight,
			EDeviceHIKVisionGigELightSourceSelector_Tungsten,
			EDeviceHIKVisionGigELightSourceSelector_Daylight6500K,
			EDeviceHIKVisionGigELightSourceSelector_Count,
		};
	}
}


// Basler
namespace Ravid
{
	namespace Device
	{
		enum EBaslerGetFunction
		{
			EBaslerGetFunction_OK = 0,
			EBaslerGetFunction_UnknownError,
			EBaslerGetFunction_NullptrError,
			EBaslerGetFunction_EnumTypeError, //범위 벗어나는 경우
		};

		enum EBaslerSetFunction
		{
			EBaslerSetFunction_OK = 0,
			EBaslerSetFunction_UnknownError,
			EBaslerSetFunction_NotFoundApiError,
			EBaslerSetFunction_AlreadyInitializedError,
			EBaslerSetFunction_NotInitializedError,
			EBaslerSetFunction_NotFindCommandError,
			EBaslerSetFunction_DeviceNullptrError,
			EBaslerSetFunction_NotSupportError,
			EBaslerSetFunction_FunctionLockedError,
			EBaslerSetFunction_WriteToDeviceError,
			EBaslerSetFunction_WriteToDatabaseError,
			EBaslerSetFunction_ReadOnDatabaseError,
			EBaslerSetFunction_ReadOnDeviceError,
			EBaslerSetFunction_ActivatedDeviceError,
		};

		enum EDeviceBaslerGigEPixelFormat
		{
			EDeviceBaslerGigEPixelFormat_Mono8 = 0,
			EDeviceBaslerGigEPixelFormat_Mono10,
			EDeviceBaslerGigEPixelFormat_Mono12,
			EDeviceBaslerGigEPixelFormat_Mono12packed,
			EDeviceBaslerGigEPixelFormat_Mono16,
			EDeviceBaslerGigEPixelFormat_BayerGR8,
			EDeviceBaslerGigEPixelFormat_BayerRG8,
			EDeviceBaslerGigEPixelFormat_BayerGB8,
			EDeviceBaslerGigEPixelFormat_BayerBG8,
			EDeviceBaslerGigEPixelFormat_BayerGR10,
			EDeviceBaslerGigEPixelFormat_BayerRG10,
			EDeviceBaslerGigEPixelFormat_BayerGB10,
			EDeviceBaslerGigEPixelFormat_BayerBG10,
			EDeviceBaslerGigEPixelFormat_BayerGR12,
			EDeviceBaslerGigEPixelFormat_BayerRG12,
			EDeviceBaslerGigEPixelFormat_BayerGB12,
			EDeviceBaslerGigEPixelFormat_BayerBG12,
			EDeviceBaslerGigEPixelFormat_RGB8Packed,
			EDeviceBaslerGigEPixelFormat_BGR8Packed,
			EDeviceBaslerGigEPixelFormat_YUV422Packed,
			EDeviceBaslerGigEPixelFormat_YUV422_YUYV_Packed,
			EDeviceBaslerGigEPixelFormat_BayerGB12Packed,
			EDeviceBaslerGigEPixelFormat_BayerGR12Packed,
			EDeviceBaslerGigEPixelFormat_BayerRG12Packed,
			EDeviceBaslerGigEPixelFormat_BayerBG12Packed,
			EDeviceBaslerGigEPixelFormat_Count,
		};

		enum EDeviceBaslerGigETriggerSelector
		{
			EDeviceBaslerGigETriggerSelector_FrameStart = 0,
			EDeviceBaslerGigETriggerSelector_LineStart,
			EDeviceBaslerGigETriggerSelector_AcquisitionStart,
			EDeviceBaslerGigETriggerSelector_Count,
		};

		enum EDeviceBaslerGigESwitch
		{
			EDeviceBaslerGigESwitch_Off = 0,
			EDeviceBaslerGigESwitch_On,
			EDeviceBaslerGigESwitch_Count,
		};

		enum EDeviceBaslerGigETriggerSource
		{
			EDeviceBaslerGigETriggerSource_Software = 0,
			EDeviceBaslerGigETriggerSource_Line1,
			EDeviceBaslerGigETriggerSource_Line2,
			EDeviceBaslerGigETriggerSource_Line3,
			EDeviceBaslerGigETriggerSource_Line4,
			EDeviceBaslerGigETriggerSource_Line5,
			EDeviceBaslerGigETriggerSource_Line6,
			EDeviceBaslerGigETriggerSource_Line7,
			EDeviceBaslerGigETriggerSource_Line8,
			EDeviceBaslerGigETriggerSource_CC1,
			EDeviceBaslerGigETriggerSource_CC2,
			EDeviceBaslerGigETriggerSource_CC3,
			EDeviceBaslerGigETriggerSource_CC4,
			EDeviceBaslerGigETriggerSource_Action1,
			EDeviceBaslerGigETriggerSource_Action2,
			EDeviceBaslerGigETriggerSource_Action3,
			EDeviceBaslerGigETriggerSource_Action4,
			EDeviceBaslerGigETriggerSource_Count,
		};

		enum EDeviceBaslerGigETriggerActivation
		{
			EDeviceBaslerGigETriggerActivation_RisingEdge = 0,
			EDeviceBaslerGigETriggerActivation_FallingEdge,
			EDeviceBaslerGigETriggerActivation_AnyEdge,
			EDeviceBaslerGigETriggerActivation_LevelHigh,
			EDeviceBaslerGigETriggerActivation_LevelLow,
			EDeviceBaslerGigETriggerActivation_Count,
		};

		enum EDeviceBaslerGigEAuto
		{
			EDeviceBaslerGigEAuto_Off = 0,
			EDeviceBaslerGigEAuto_Once,
			EDeviceBaslerGigEAuto_Continuous,
			EDeviceBaslerGigEAuto_Count,
		};

		enum EDeviceBaslerGigEGainSelector
		{
			EDeviceBaslerGigEGainSelector_All = 0,
			EDeviceBaslerGigEGainSelector_AnalogAll,
			EDeviceBaslerGigEGainSelector_DigitalAll,
			EDeviceBaslerGigEGainSelector_Tap1,
			EDeviceBaslerGigEGainSelector_Tap2,
			EDeviceBaslerGigEGainSelector_Count,
		};

		enum EDeviceBaslerGigELineSelector
		{
			EDeviceBaslerGigELineSelector_Line1 = 0,
			EDeviceBaslerGigELineSelector_Line2,
			EDeviceBaslerGigELineSelector_Line3,
			EDeviceBaslerGigELineSelector_Line4,
			EDeviceBaslerGigELineSelector_Count,
		};

		enum EDeviceBaslerGigELineMode
		{
			EDeviceBaslerGigELineMode_Input = 0,
			EDeviceBaslerGigELineMode_Output,
			EDeviceBaslerGigELineMode_Count,
		};

		enum EDeviceBaslerGigELineSource
		{
			EDeviceBaslerGigELineSource_ExposureActive = 0,
			EDeviceBaslerGigELineSource_FrameTriggerWait,
			EDeviceBaslerGigELineSource_FrameBurstTriggerWait,
			EDeviceBaslerGigELineSource_Timer1Active,
			EDeviceBaslerGigELineSource_UserOutput0,
			EDeviceBaslerGigELineSource_UserOutput1,
			EDeviceBaslerGigELineSource_UserOutput2,
			EDeviceBaslerGigELineSource_UserOutput3,
			EDeviceBaslerGigELineSource_FlashWindow,
			EDeviceBaslerGigELineSource_Count,
		};

		enum EDeviceBaslerGigELightSourceSelector
		{
			EDeviceBaslerGigELightSourceSelector_Off = 0,
			EDeviceBaslerGigELightSourceSelector_Custom,
			EDeviceBaslerGigELightSourceSelector_Daylight,
			EDeviceBaslerGigELightSourceSelector_Tungsten,
			EDeviceBaslerGigELightSourceSelector_Daylight6500K,
			EDeviceBaslerGigELightSourceSelector_Count,
		};
	}
}

// Baumer
namespace Ravid
{
	namespace Device
	{
		enum EDeviceBaumerPixelFormat
		{
			EDeviceBaumerPixelFormat_Mono8 = 0,
			EDeviceBaumerPixelFormat_Mono10,
			EDeviceBaumerPixelFormat_Mono12,
			EDeviceBaumerPixelFormat_Mono14,
			EDeviceBaumerPixelFormat_Mono16,
			EDeviceBaumerPixelFormat_BayerBG8,
			EDeviceBaumerPixelFormat_BayerBG10,
			EDeviceBaumerPixelFormat_BayerBG12,
			EDeviceBaumerPixelFormat_BayerBG16,
			EDeviceBaumerPixelFormat_BayerGB8,
			EDeviceBaumerPixelFormat_BayerGB10,
			EDeviceBaumerPixelFormat_BayerGB12,
			EDeviceBaumerPixelFormat_BayerGB16,
			EDeviceBaumerPixelFormat_BayerGR8,
			EDeviceBaumerPixelFormat_BayerGR10,
			EDeviceBaumerPixelFormat_BayerGR12,
			EDeviceBaumerPixelFormat_BayerGR16,
			EDeviceBaumerPixelFormat_BayerRG8,
			EDeviceBaumerPixelFormat_BayerRG10,
			EDeviceBaumerPixelFormat_BayerRG12,
			EDeviceBaumerPixelFormat_BayerRG16,
			EDeviceBaumerPixelFormat_RGB8,
			EDeviceBaumerPixelFormat_RGB10,
			EDeviceBaumerPixelFormat_RGB12,
			EDeviceBaumerPixelFormat_RGB16,
			EDeviceBaumerPixelFormat_BGR8,
			EDeviceBaumerPixelFormat_BGR10,
			EDeviceBaumerPixelFormat_BGR12,
			EDeviceBaumerPixelFormat_BGR16,
			EDeviceBaumerPixelFormat_Count,
		};

		enum EDeviceBaumerTriggerSelector
		{
			EDeviceBaumerTriggerSelector_FrameStart = 0,
			EDeviceBaumerTriggerSelector_LineStart,
			EDeviceBaumerTriggerSelector_AcquisitionStart,
			EDeviceBaumerTriggerSelector_Count,
		};

		enum EDeviceBaumerSwitch
		{
			EDeviceBaumerSwitch_Off = 0,
			EDeviceBaumerSwitch_On,
			EDeviceBaumerSwitch_Count,
		};

		enum EDeviceBaumerTriggerSource
		{
			EDeviceBaumerTriggerSource_Software = 0,
			EDeviceBaumerTriggerSource_Line1,
			EDeviceBaumerTriggerSource_Line2,
			EDeviceBaumerTriggerSource_Line3,
			EDeviceBaumerTriggerSource_Line4,
			EDeviceBaumerTriggerSource_Line5,
			EDeviceBaumerTriggerSource_Line6,
			EDeviceBaumerTriggerSource_Line7,
			EDeviceBaumerTriggerSource_Line8,
			EDeviceBaumerTriggerSource_CC1,
			EDeviceBaumerTriggerSource_CC2,
			EDeviceBaumerTriggerSource_CC3,
			EDeviceBaumerTriggerSource_CC4,
			EDeviceBaumerTriggerSource_Action1,
			EDeviceBaumerTriggerSource_Action2,
			EDeviceBaumerTriggerSource_Action3,
			EDeviceBaumerTriggerSource_Action4,
			EDeviceBaumerTriggerSource_Count,
		};

		enum EDeviceBaumerTriggerActivation
		{
			EDeviceBaumerTriggerActivation_RisingEdge = 0,
			EDeviceBaumerTriggerActivation_FallingEdge,
			EDeviceBaumerTriggerActivation_AnyEdge,
			EDeviceBaumerTriggerActivation_LevelHigh,
			EDeviceBaumerTriggerActivation_LevelLow,
			EDeviceBaumerTriggerActivation_Count,
		};

		enum EDeviceBaumerAuto
		{
			EDeviceBaumerAuto_Off = 0,
			EDeviceBaumerAuto_Once,
			EDeviceBaumerAuto_Continuous,
			EDeviceBaumerAuto_Count,
		};

		enum EDeviceBaumerGainSelector
		{
			EDeviceBaumerGainSelector_All = 0,
			EDeviceBaumerGainSelector_AnalogAll,
			EDeviceBaumerGainSelector_DigitalAll,
			EDeviceBaumerGainSelector_Count,
		};

		enum EDeviceBaumerExposureMode
		{
			EDeviceBaumerExposureMode_Off = 0,
			EDeviceBaumerExposureMode_Timed,
			EDeviceBaumerExposureMode_TriggerControlled,
			EDeviceBaumerExposureMode_TriggerWidth,
			EDeviceBaumerExposureMode_Count,
		};

		enum EDeviceBaumerLineSelector
		{
			EDeviceBaumerLineSelector_Line0 = 0,
			EDeviceBaumerLineSelector_Line1,
			EDeviceBaumerLineSelector_Line2,
			EDeviceBaumerLineSelector_Line3,
			EDeviceBaumerLineSelector_Count,
		};

		enum EDeviceBaumerLineMode
		{
			EDeviceBaumerLineMode_Input = 0,
			EDeviceBaumerLineMode_Output,
			EDeviceBaumerLineMode_Count,
		};

		enum EDeviceBaumerLineSource
		{
			EDeviceBaumerLineSource_Off = 0,
			EDeviceBaumerLineSource_ExposureActive,
			EDeviceBaumerLineSource_ReadoutActive,
			EDeviceBaumerLineSource_Timer1Active,
			EDeviceBaumerLineSource_TriggerReady,
			EDeviceBaumerLineSource_UserOutput1,
			EDeviceBaumerLineSource_Count,
		};

		enum EDeviceBaumerUserOutputSelector
		{
			EDeviceBaumerUserOutputSelector_UserOutput0 = 0,
			EDeviceBaumerUserOutputSelector_UserOutput1,
			EDeviceBaumerUserOutputSelector_UserOutput2,
			EDeviceBaumerUserOutputSelector_UserOutput3,
			EDeviceBaumerUserOutputSelector_Count,
		};

		enum EDeviceBaumerTimerSelector
		{
			EDeviceBaumerTimerSelector_Timer1 = 0,
			EDeviceBaumerTimerSelector_Timer2,
			EDeviceBaumerTimerSelector_Timer3,
			EDeviceBaumerTimerSelector_Timer4,
			EDeviceBaumerTimerSelector_Timer5,
			EDeviceBaumerTimerSelector_Timer6,
			EDeviceBaumerTimerSelector_Timer7,
			EDeviceBaumerTimerSelector_Timer8,
			EDeviceBaumerTimerSelector_Count,
		};

		enum EDeviceBaumerTimerTriggerSource
		{
			EDeviceBaumerTimerTriggerSource_Off = 0,
			EDeviceBaumerTimerTriggerSource_Action1,
			EDeviceBaumerTimerTriggerSource_ExposureEnd,
			EDeviceBaumerTimerTriggerSource_ExposureStart,
			EDeviceBaumerTimerTriggerSource_FrameTransferSkipped,
			EDeviceBaumerTimerTriggerSource_Line0,
			EDeviceBaumerTimerTriggerSource_Software,
			EDeviceBaumerTimerTriggerSource_TriggerSkipped,
			EDeviceBaumerTimerTriggerSource_Count,
		};
	}
}

// Common
namespace Ravid
{
	namespace Device
	{
		enum EDeviceInitializeResult
		{
			EDeviceInitializeResult_OK = 0,						// 오류 없음
			EDeviceInitializeResult_UnknownError,				// 알려지지 않은 오류
			EDeviceInitializeResult_NotFoundApiError,			// API dll 유무
			EDeviceInitializeResult_NotFoundLibraries,
			EDeviceInitializeResult_NotInitLibraries,
			EDeviceInitializeResult_NotFoundSerial,
			EDeviceInitializeResult_RangeOfParameterError,
			EDeviceInitializeResult_RangeOfDeviceIDError,		// Device ID 범위	
			EDeviceInitializeResult_NotSupportedObjectID,
			EDeviceInitializeResult_AlreadyInitializedError,	// 이미 초기화 상태
			EDeviceInitializeResult_NotInitializeDevice,
			EDeviceInitializeResult_WriteToDeviceError,			// Device에 쓰기 실패
			EDeviceInitializeResult_WriteToDatabaseError,		// DB에 쓰기 실패
			EDeviceInitializeResult_EnumerateDeviceError,		// Device 항목 얻기 실패
			EDeviceInitializeResult_NotFoundDeviceError,		// Device 찾기 실패
			EDeviceInitializeResult_NotCreateDeviceError,		// Device 생성 실패
			EDeviceInitializeResult_NotSupportedDeviceError,	// Device에서 지원하지 않는 오류
			EDeviceInitializeResult_NotOpenedDeviceError,		// Device 열기 실패
			EDeviceInitializeResult_ReadOnDatabaseError,		// DB에서 읽기 실패
			EDeviceInitializeResult_ReadOnDeviceError,			// Device에서 읽기 실패
			EDeviceInitializeResult_AllocateMemoryError,
			EDeviceInitializeResult_ConvertFormatError,
			EDeviceInitializeResult_NotSupportOS,
			EDeviceInitializeResult_NotCreateImagebuffer,
			EDeviceInitializeResult_MatchedInfoError,			// Device 내용 다를 오류
			EDeviceInitializeResult_NotMappingFile,
			EDeviceInitializeResult_NotMakeFile,
			EDeviceInitializeResult_NotInitializeMotionError,

			// 180123 추가부분 - Basler
			EDeviceInitializeResult_NotFoundDeviceInfo,			// Device 정보얻기 실패
			EDeviceInitializeResult_NotCorrespondSerialNumber,	// SerialNumber 불일치
			EDeviceInitializeResult_NotFoundSerialNumber,		// SerialNumber 찾을 수 없음
			EDeviceInitializeResult_DeviceHandleFailedGenerate,			// Device Handle 생성 실패
			EDeviceInitializeResult_NotResetDevice,
			EDeviceInitializeResult_CanNotReadDBWaitingTime,
			EDeviceInitializeResult_CanNotWriteDBWaitingTime,
			EDeviceInitializeResult_CanNotReadDBPixelFormat,
			EDeviceInitializeResult_CanNotApplyPixelFormat,
			EDeviceInitializeResult_CanNotReadDBOffsetX,
			EDeviceInitializeResult_CanNotApplyOffsetX,
			EDeviceInitializeResult_CanNotApplyOffsetY,
			EDeviceInitializeResult_CanNotReadDBCanvasWidth,
			EDeviceInitializeResult_WidthMaxGetFailed,
			EDeviceInitializeResult_CanNotApplyWidth,
			EDeviceInitializeResult_CanNotReadDBCanvasHeight,
			EDeviceInitializeResult_HeightMaxGetFailed,
			EDeviceInitializeResult_CanNotApplyHeight,
			EDeviceInitializeResult_CanNotReadDBOffsetCenterX,
			EDeviceInitializeResult_CanNotApplyCenterX,
			//EDeviceInitializeResult_CanNotReadDBOffsetX,
			//EDeviceInitializeResult_CanNotApplyOffsetX,
			EDeviceInitializeResult_CanNotReadDBOffsetCenterY,
			EDeviceInitializeResult_CanNotApplyCenterY,
			EDeviceInitializeResult_CanNotReadDBOffsetY,
			//EDeviceInitializeResult_CanNotApplyOffsetY,
			EDeviceInitializeResult_CanNotReadDBExposureAuto,
			EDeviceInitializeResult_CanNotApplyExposureAuto,
			EDeviceInitializeResult_CanNotReadDBExposure,
			EDeviceInitializeResult_CanNotApplyExposureTimeRaw,
			EDeviceInitializeResult_CanNotReadDBGainAuto,
			EDeviceInitializeResult_CanNotApplyGainAuto,
			EDeviceInitializeResult_CanNotReadDBAllGain,
			EDeviceInitializeResult_CanNotApplyAllGainRaw,
			EDeviceInitializeResult_CanNotReadDBAnalogGain,
			EDeviceInitializeResult_CanNotApplyAnalogAllGainRaw,
			EDeviceInitializeResult_CanNotReadDBDigitalGain,
			EDeviceInitializeResult_CanNotApplyDigitalAllGainRaw,
			EDeviceInitializeResult_CanNotReadDBTap1Gain,
			EDeviceInitializeResult_CanNotApplyTap1GainRaw,
			EDeviceInitializeResult_CanNotReadDBTap2Gain,
			EDeviceInitializeResult_CanNotApplyTap2GainRaw,
			EDeviceInitializeResult_CanNotReadDBFrameStartMode,
			EDeviceInitializeResult_CanNotApplyFrameStartTriggerMode,
			EDeviceInitializeResult_CanNotReadDBFrameStartTriggerSource,
			EDeviceInitializeResult_CanNotApplyTriggerSource,
			EDeviceInitializeResult_CanNotReadDBLineStartMode,
			EDeviceInitializeResult_CanNotApplyLineStartTriggerMode,
			EDeviceInitializeResult_CanNotReadDBLineStartTriggerSource,
			EDeviceInitializeResult_CanNotReadDBAcquisitionStartMode,
			EDeviceInitializeResult_CanNotApplyAcquisitionStartTriggerMode,
			EDeviceInitializeResult_CanNotReadDBAcquisitionStartTriggerSource,
			EDeviceInitializeResult_CanNotReadDBTriggerSelector,
			EDeviceInitializeResult_CanNotApplyTriggerSelector,
			EDeviceInitializeResult_CanNotApplyAcquisitionModeContinuous,
			EDeviceInitializeResult_CanNotReadDBPacketSize,
			EDeviceInitializeResult_CanNotWriteDBPacketSize,
			EDeviceInitializeResult_CanNotReadDevicePacketSize,
			EDeviceInitializeResult_CanNotApplyGevSCPSPacketSize,

			//

			//AXT DIO
			EDeviceInitializeResult_NotInitializeDIOError,
			EDeviceInitializeResult_ClearOutputPortError,
			EDeviceInitializeResult_OtherDeviceInitError,
			EDeviceInitializeResult_ObjectIDError,
			// Advantech DIO
			EDeviceInitializeResult_CreateOutputError,			// output 생성 오류	
			EDeviceInitializeResult_CreateInputError,			// input 생성 오류
			EDeviceInitializeResult_SupportOutputError,			// output 지원 오류
			EDeviceInitializeResult_SupportInputError,			// input 지원 오류
			EDeviceInitializeResult_UseIOCountError,			// IO 지원가능한 포트가 없는 오류
			EDeviceInitializeResult_SelectOutputError,			// output 선택 오류	
			EDeviceInitializeResult_SelectInputError,			// input 선택 오류

			// RS232 전용
			EDeviceInitializeResult_InitializePortError,		// port 초기화 오류
			EDeviceInitializeResult_CreatePortError,			// port 생성 오류
			EDeviceInitializeResult_CreateIOPortError,			// port I/O 생성 오류	
			EDeviceInitializeResult_ClearPortError,				// port clear 오류
			EDeviceInitializeResult_SetPortError,				// port set 오류
			EDeviceInitializeResult_ChangeMaskPortError,		// port mask 수정 오류

			// socket 전용
			EDeviceInitializeResult_InitializeSocketError,		// socket 초기화 오류
			EDeviceInitializeResult_InvalidSocketError,			// socket 유효성 오류
			EDeviceInitializeResult_ConnectSocketError,			// socket 연결 오류
			EDeviceInitializeResult_BindSocketError,			// socket bind 오류
			EDeviceInitializeResult_ListenSocketError,			// socket listen 오류
			EDeviceInitializeResult_ChangeModeSocketError,		// socket 모드 수정 오류
			EDeviceInitializeResult_CreateThreadError,			// thread 생성 오류

			//HIKVision 전용
			EDeviceInitializeResult_CanNotReadDBRevereX,		// DB에서 Y축 대칭의 유무 얻어오기 오류
			EDeviceInitializeResult_CanNotApplyRevereX,			// 디바이스에 Y축 대칭 적용 오류
			EDeviceInitializeResult_CanNotReadDBRevereY,		// DB에서 X축 대칭의 유무 얻어오기 오류
			EDeviceInitializeResult_CanNotApplyRevereY,			// 디바이스에 X축 대칭 적용 오류
			EDeviceInitializeResult_CanNotApplyGain,			// 디바이스에 Gain값 적용 오류
			EDeviceInitializeResult_CanNotApplyAcquisitionMode, // 디바이스에 AcqusistionMode 적용 오류
			EDeviceInitializeResult_CanNotReadDBAcquisitionBurstFrameCount, // DB에서 AcqusistionBurstFrameCount 오류
			EDeviceInitializeResult_CanNotApplyAcquisitionBurstFrameCount,
			EDeviceInitializeResult_CanNotReadDBAcquisitionFramRateControlEnable,
			EDeviceInitializeResult_CanNotApplyAcquisitionFramRateControlEnable,
			EDeviceInitializeResult_CanNotReadDBAcquisitionBurstFrameRate,
			EDeviceInitializeResult_CanNotApplyAcquisitionBurstFrameRate,
			EDeviceInitializeResult_CanNotReadDBAcquisitionStartTriggerSelector,
			EDeviceInitializeResult_CanNotApplyAcquisitionStartTriggerSelector,
			EDeviceInitializeResult_CanNotReadDBAcquisitionStartTriggerMode,
			EDeviceInitializeResult_CanNotApplyAcquisitionTriggerMode,
			EDeviceInitializeResult_CanNotReadDBAcquisitionTriggerSource,
			EDeviceInitializeResult_CanNotApplyAcquisitionTriggerSource,
			EDeviceInitializeResult_CanNotReadDBAcquisitionTriggerActivation,
			EDeviceInitializeResult_CanNotApplyAcquisitionTriggerActivation,
			EDeviceInitializeResult_CanNotReadDBAcquisitionExposureAuto,
			EDeviceInitializeResult_CanNotApplyAcquisitionExposureAuto,
			EDeviceInitializeResult_CanNotReadDBAcquisitionExposureTime,
			EDeviceInitializeResult_CanNotApplyAcquisitionExposureTime,
			EDeviceInitializeResult_CanNotReadDBAcquisitionTriggerDelay,
			EDeviceInitializeResult_CanNotApplyAcquisitionTriggerDelay,
			EDeviceInitializeResult_CanNotReadDBBlackLevelEnable,
			EDeviceInitializeResult_CanNotApplyBlackLevelEnable,
			EDeviceInitializeResult_CanNotReadDBBlackLevel,
			EDeviceInitializeResult_CanNotApplyBlackLevel,
			EDeviceInitializeResult_CanNotReadDBDigitalShiftEnable,
			EDeviceInitializeResult_CanNotApplyDigitalShiftEnable,
			EDeviceInitializeResult_CanNotReadDBDigitalShift,
			EDeviceInitializeResult_CanNotApplyDigitalShift,
			EDeviceInitializeResult_CanNotReadDBLineSelector,
			EDeviceInitializeResult_CanNotApplyLineSelector,
			EDeviceInitializeResult_CanNotReadDBLineDebouncerTime,
			EDeviceInitializeResult_CanNotApplyLineDebouncerTime,
			EDeviceInitializeResult_CanNotReadDBLineInverterStatus,
			EDeviceInitializeResult_CanNotApplyLineInverterStatus,
			EDeviceInitializeResult_CanNotReadDBLineSource,
			EDeviceInitializeResult_CanNotApplyLineSource,
			EDeviceInitializeResult_CanNotReadDBLineStrobEnable,
			EDeviceInitializeResult_CanNotApplyLineStrobEnable,
			EDeviceInitializeResult_CanNotReadDBLineStrobeLineDuration,
			EDeviceInitializeResult_CanNotApplyLineStrobeLineDuration,
			EDeviceInitializeResult_CanNotReadDBLineStrobeLineDelay,
			EDeviceInitializeResult_CanNotApplyLineStrobeLineDelay,
			EDeviceInitializeResult_CanNotReadDBLineStrobeLinePreDelay,
			EDeviceInitializeResult_CanNotApplyLineStrobeLinePreDelay,
			EDeviceInitializeResult_CanNotReadDBHardWareTriggerActivaion,
			EDeviceInitializeResult_CanNotApplyHardWareTriggerActivaion,
			EDeviceInitializeResult_CanNotReadDBLineMode,
			EDeviceInitializeResult_CanNotApplyLineMode,
			EDeviceInitializeResult_CanNotReadDBAcquisitionExposureMode,
			EDeviceInitializeResult_CanNotApplyAcquisitionExposureMode,
			EDeviceInitializeResult_CanNotApplyImageCallBack,
		};

		enum EDeviceTerminateResult
		{
			EDeviceTerminateResult_OK = 0,
			EDeviceTerminateResult_UnknownError,
			EDeviceTerminateResult_NotFoundApiError,
			EDeviceTerminateResult_AlreadyTerminatedError,
			EDeviceTerminateResult_NotFoundDeviceError,
			EDeviceTerminateResult_NotCreateDeviceError,
			EDeviceTerminateResult_NotOpenDeviceError,
			EDeviceTerminateResult_WriteToDeviceError,
			EDeviceTerminateResult_FailedToStopError, // Basler IsLive, !GrabAvailable 안 Stop() 실패시
			EDeviceTerminateResult_NotToStopError,
			EDeviceTerminateResult_NotClosedDeviceError,
			EDeviceTerminateResult_NotDestroyedDeviceError,
			EDeviceTerminateResult_NotSupportOS,

			// axt
			EDeviceTerminateResult_ClearOutputPortError,
			// RS232 전용
			EDeviceTerminateResult_InitializePortError,			// port 초기화 오류
		};

		enum EDeviceGrabResult
		{
			EDeviceGrabResult_OK = 0,
			EDeviceGrabResult_UnknownError,
			EDeviceGrabResult_NotInitializedError,
			EDeviceGrabResult_AlreadyGrabError,
			EDeviceGrabResult_WriteToDeviceError,
			EDeviceGrabResult_ReadOnDeviceError,
			EDeviceGrabResult_ReadOnDatabaseError,
			EDeviceGrabResult_CreateThreadError,
		};

		enum EDeviceLiveResult
		{
			EDeviceLiveResult_OK = 0,
			EDeviceLiveResult_UnknownError,
			EDeviceLiveResult_NotInitializedError,
			EDeviceLiveResult_AlreadyGrabError,
			EDeviceLiveResult_WriteToDeviceError,
			EDeviceLiveResult_ReadOnDeviceError,
			EDeviceLiveResult_ReadOnDatabaseError,
			EDeviceLiveResult_CreateThreadError,
			EDeviceLiveResult_DoNotSupport,
		};

		enum EDeviceStopResult
		{
			EDeviceStopResult_OK = 0,
			EDeviceStopResult_UnknownError,
			EDeviceStopResult_NotInitializedError,
			EDeviceStopResult_AlreadyStopError,
			EDeviceStopResult_WriteToDeviceError,
			EDeviceStopResult_ReadOnDeviceError,
			EDeviceStopResult_ReadOnDatabaseError,
			EDeviceStopResult_CreateThreadError,
		};

		enum EDeviceTriggerResult
		{
			EDeviceTriggerResult_OK = 0,
			EDeviceTriggerResult_UnknownError,
			EDeviceTriggerResult_NotInitializedError,
			EDeviceTriggerResult_AlreadyStopError,
			EDeviceTriggerResult_WriteToDeviceError,
			EDeviceTriggerResult_ReadOnDeviceError,
			EDeviceTriggerResult_ReadOnDatabaseError,
			EDeviceTriggerResult_CreateThreadError,
		};

	}
}

namespace Ravid
{
	namespace Device
	{
		// 오류 없음
		// OK = 0,

		// 정상적 오류 영역 벗어남
		// UnknownError,

		// DB에서 쓰기 실패 에러
		// WriteToDatabaseError,

		// DB에서 읽기 실패 에러
		// ReadFromDatabaseError,

		// 명령어가 아닐 경우 에러
		// NotFoundCommandError,

		// 초기화상태 사용 불가 에러
		// InitalizedDeviceError,

		// 이미지 grab, live 중 사용 불가 에러
		// ActivatedDeviceError,

		// 디바이스에서 지원하지 않는 명령어 에러
		// NotSupportedError,

		// 디바이스에서 기능 잠김 에러
		// FunctionLockedError,

		// 디바이스에서 읽기 실패 에러
		// ReadOnDeviceError,

		// 디바이스에 쓰기 실패 에러
		// WriteToDeviceError,

		/*
		OK,
		UnknownError,
		WriteToDevice,
		WriteToDatabase
		ReadFromDevice,
		ReadFromDatabase,
		FunctionLocked,
		AlreadyInit,
		CommandNotFound,

		ActivatedDevice,
		디바이스에서 해당기능을 지원하지 않습니다.
		NotSupportedFeature

		*/




		enum EBaumerGetFunction
		{
			EBaumerGetFunction_OK = 0,
			EBaumerGetFunction_UnknownError,
			EBaumerGetFunction_EnumTypeError,
			EBaumerGetFunction_NullptrError,
		};

		enum EBaumerSetFunction
		{
			EBaumerSetFunction_OK = 0,
			EBaumerSetFunction_UnknownError,
			EBaumerSetFunction_NotFoundApiError,
			EBaumerSetFunction_AlreadyInitializedError,
			EBaumerSetFunction_NotInitializedError,
			EBaumerSetFunction_NotFindCommandError, // _CommandDoesNotExist
			EBaumerSetFunction_DeviceNullptrError,
			EBaumerSetFunction_NotSupportError,
			EBaumerSetFunction_FunctionLockedError,
			EBaumerSetFunction_WriteToDeviceError,
			EBaumerSetFunction_WriteToDatabaseError,
			EBaumerSetFunction_ReadOnDatabaseError,
			EBaumerSetFunction_ReadOnDeviceError,
			EBaumerSetFunction_ActivatedDeviceError,
		};
	}
}

// DeviceID, SubunitID
namespace Ravid
{
	namespace Device
	{
		enum EDeviceBaseIDResult
		{
			EDeviceBaseIDResult_OK = 0,
			EDeviceBaseIDResult_UnknownError,
			EDeviceBaseIDResult_AlreadyInitializedError,
			EDeviceBaseIDResult_NotFindCommandError,
			EDeviceBaseIDResult_NotWriteToDatabase,
		};
	}
}

// Tcp, RS232
namespace Ravid
{
	namespace Device
	{
		enum ECommSendResult
		{
			ECommSendResult_OK = 0,
			ECommSendResult_UnknownError,
			ECommSendResult_NullptrPacket,
			ECommSendResult_NullptrData,
			ECommSendResult_PacketSizeZero,
			ECommSendResult_NotInitialized,
			ECommSendResult_SendingError,
		};

		enum ETcpGetFunction
		{
			ETcpGetFunction_OK = 0,
			ETcpGetFunction_UnknownError,
			ETcpGetFunction_NullptrOutput,
		};

		enum ETcpSetFunction
		{
			ETcpSetFunction_OK = 0,
			ETcpSetFunction_UnknownError,
			ETcpSetFunction_InitializedError,
			ETcpSetFunction_WriteToDatabaseError,
			ETcpSetFunction_OutOfRangeError,
		};

		enum ERs232GetFunction
		{
			ERs232GetFunction_OK = 0,
			ERs232GetFunction_UnknownError,
			ERs232GetFunction_NullptrOutput,
			ERs232GetFunction_NotFindCommand,
		};

		enum ERs232SetFunction
		{
			ERs232SetFunction_OK = 0,
			ERs232SetFunction_UnknownError,
			ERs232SetFunction_InitializedError,
			ERs232SetFunction_NotFindCommand,
			ERs232SetFunction_WriteToDatabaseError,
		};

		enum EDeviceRS232CBaudRate
		{
			EDeviceRS232CBaudRate_110 = 0,
			EDeviceRS232CBaudRate_300,
			EDeviceRS232CBaudRate_1200,
			EDeviceRS232CBaudRate_2400,
			EDeviceRS232CBaudRate_4800,
			EDeviceRS232CBaudRate_9600,
			EDeviceRS232CBaudRate_19200,
			EDeviceRS232CBaudRate_38400,
			EDeviceRS232CBaudRate_57600,
			EDeviceRS232CBaudRate_115200,
			EDeviceRS232CBaudRate_230400,
			EDeviceRS232CBaudRate_460800,
			EDeviceRS232CBaudRate_921600,
			EDeviceRS232CBaudRate_Count,
		};

		enum EDeviceRS232CDataBit
		{
			EDeviceRS232CDataBit_5 = 0,
			EDeviceRS232CDataBit_6,
			EDeviceRS232CDataBit_7,
			EDeviceRS232CDataBit_8,
			EDeviceRS232CDataBit_Count,
		};

		enum EDeviceRS232CParity
		{
			EDeviceRS232CParity_None = 0,
			EDeviceRS232CParity_Odd,
			EDeviceRS232CParity_Even,
			EDeviceRS232CParity_Mark,
			EDeviceRS232CParity_Space,
			EDeviceRS232CParity_Count,
		};

		enum EDeviceRS232CStopBits
		{
			EDeviceRS232CStopBits_1 = 0,
			EDeviceRS232CStopBits_1Point5,
			EDeviceRS232CStopBits_2,
			EDeviceRS232CStopBits_Count,
		};

		enum EDeviceRS232CDataFormat
		{
			EDeviceRS232CDataFormat_Ascii = 0,
			EDeviceRS232CDataFormat_Binary,
			EDeviceRS232CDataFormat_Oct,
			EDeviceRS232CDataFormat_Hex,
			EDeviceRS232CDataFormat_Count,
		};

		enum EDeviceSocketDisplayUnit
		{
			EDeviceSocketDisplayUnit_Byte = 0,
			EDeviceSocketDisplayUnit_KByte,
			EDeviceSocketDisplayUnit_MByte,
			EDeviceSocketDisplayUnit_GByte,
			EDeviceSocketDisplayUnit_TByte,
			EDeviceSocketDisplayUnit_EByte,
			EDeviceSocketDisplayUnit_Count,
		};
	}
}

// DIO
namespace Ravid
{
	namespace Device
	{
		enum EDeviceDIOSwitch
		{
			EDeviceDIOSwitch_Off = 0,
			EDeviceDIOSwitch_On,
			EDeviceDIOSwitch_Count,
		};
	}
}

// DIO
namespace Ravid
{
	namespace Device
	{
		enum EDeviceMSPDIoBoardType
		{
			EDeviceMSPDIoBoardType_MSP_P24_T8_Q8_1 = 0,
			EDeviceMSPDIoBoardType_MSP_P24_T8_Q8_2,
			EDeviceMSPDIoBoardType_MSP_P24_T8_Q8_3,
			EDeviceMSPDIoBoardType_MSP_P24_T8_Q8_4,
			EDeviceMSPDIoBoardType_MSP_P24_T0_Q8_1,
			EDeviceMSPDIoBoardType_MSP_P24_T6_Q24_1,
			EDeviceMSPDIoBoardType_Count,
		};
	}
}

// AXTMotionFS10 
namespace Ravid
{
	namespace Device
	{
		enum EFS10GetFunction
		{
			EFS10GetFunction_OK = 0,
			EFS10GetFunction_UnknownError,
			EFS10GetFunction_NullptrError,
		};

		enum EFS10SetFunction
		{
			EFS10SetFunction_OK = 0,
			EFS10SetFunction_UnknownError,
			EFS10SetFunction_AlreadyInitializedError,
			EFS10SetFunction_ReadOnDatabaseError,
			EFS10SetFunction_WriteToDatabaseError,
			EFS10SetFunction_ReadOnDeviceError,
			EFS10SetFunction_NotFoundCommandError,
		};

		enum EDeviceAxtMotionFS10PulseOutputMethod
		{
			EDeviceAxtMotionFS10PulseOutputMethod_OneHighLowHigh = 0,
			EDeviceAxtMotionFS10PulseOutputMethod_OneHighHighLow,
			EDeviceAxtMotionFS10PulseOutputMethod_OneLowLowHigh,
			EDeviceAxtMotionFS10PulseOutputMethod_OneLowHighLow,
			EDeviceAxtMotionFS10PulseOutputMethod_TwoCcwCwHigh,
			EDeviceAxtMotionFS10PulseOutputMethod_TwoCcwCwLow,
			EDeviceAxtMotionFS10PulseOutputMethod_TwoCwCcwHigh,
			EDeviceAxtMotionFS10PulseOutputMethod_TwoCwCcwLow,
			EDeviceAxtMotionFS10PulseOutputMethod_Count,
		};

		enum EDeviceAxtMotionFS10EncoderMethod
		{
			EDeviceAxtMotionFS10EncoderMethod_UpDownMode = 0,
			EDeviceAxtMotionFS10EncoderMethod_Sqr1Mode,
			EDeviceAxtMotionFS10EncoderMethod_Sqr2Mode,
			EDeviceAxtMotionFS10EncoderMethod_Sqr4Mode,
			EDeviceAxtMotionFS10EncoderMethod_Count,
		};

		enum EDeviceAxtMotionFS10LevelMethod
		{
			EDeviceAxtMotionFS10LevelMethod_LOW = 0,
			EDeviceAxtMotionFS10LevelMethod_HIGH,
			EDeviceAxtMotionFS10LevelMethod_Count,
		};
	}
}

// AXTMotionFS20 

namespace Ravid
{
	namespace Device
	{
		enum EFS20GetFunction
		{
			EFS20GetFunction_OK = 0,
			EFS20GetFunction_UnknownError,
			EFS20GetFunction_NullptrError,
		};

		enum EFS20SetFunction
		{
			EFS20SetFunction_OK = 0,
			EFS20SetFunction_UnknownError,
			EFS20SetFunction_AlreadyInitializedError,
			EFS20SetFunction_ReadOnDatabaseError,
			EFS20SetFunction_WriteToDatabaseError,
			EFS20SetFunction_ReadOnDeviceError,
			EFS20SetFunction_NotFoundCommandError,
		};

		enum EDeviceAxtMotionFS20PulseOutputMethod
		{
			EDeviceAxtMotionFS20PulseOutputMethod_OneHighLowHigh = 0,
			EDeviceAxtMotionFS20PulseOutputMethod_OneHighHighLow,
			EDeviceAxtMotionFS20PulseOutputMethod_OneLowLowHigh,
			EDeviceAxtMotionFS20PulseOutputMethod_OneLowHighLow,
			EDeviceAxtMotionFS20PulseOutputMethod_TwoCcwCwHigh,
			EDeviceAxtMotionFS20PulseOutputMethod_TwoCcwCwLow,
			EDeviceAxtMotionFS20PulseOutputMethod_TwoCwCcwHigh,
			EDeviceAxtMotionFS20PulseOutputMethod_TwoCwCcwLow,
			EDeviceAxtMotionFS20PulseOutputMethod_Count,
		};

		enum EDeviceAxtMotionFS20EncoderMethod
		{
			EDeviceAxtMotionFS20EncoderMethod_UpDownMode = 0,
			EDeviceAxtMotionFS20EncoderMethod_Sqr1Mode,
			EDeviceAxtMotionFS20EncoderMethod_Sqr2Mode,
			EDeviceAxtMotionFS20EncoderMethod_Sqr4Mode,
			EDeviceAxtMotionFS20EncoderMethod_Count,
		};

		enum EDeviceAxtMotionFS20LevelMethod
		{
			EDeviceAxtMotionFS20LevelMethod_LOW = 0,
			EDeviceAxtMotionFS20LevelMethod_HIGH,
			EDeviceAxtMotionFS20LevelMethod_Count,
		};

		enum EDeviceAxtMotionFS20DetectSignal
		{
			EDeviceAxtMotionFS20DetectSignal_PositiveEndLimitFallingEdge = 0,
			EDeviceAxtMotionFS20DetectSignal_NegativeEndLimitFallingEdge,
			EDeviceAxtMotionFS20DetectSignal_PositiveSlowdownLimitFallingEdge,
			EDeviceAxtMotionFS20DetectSignal_NegativeSlowdownLimitFallingEdge,
			EDeviceAxtMotionFS20DetectSignal_In0_Origin_FallingEdge,
			EDeviceAxtMotionFS20DetectSignal_In1_Zaxis_FallingEdge,
			EDeviceAxtMotionFS20DetectSignal_In2_General_FallingEdge,
			EDeviceAxtMotionFS20DetectSignal_In3_General_FallingEdge,
			EDeviceAxtMotionFS20DetectSignal_PositiveEndLimitRisingEdge,
			EDeviceAxtMotionFS20DetectSignal_NegativeEndLimitRisingEdge,
			EDeviceAxtMotionFS20DetectSignal_PositiveSlowdownLimitRisingEdge,
			EDeviceAxtMotionFS20DetectSignal_NegativeSlowdownLimitRisingEdge,
			EDeviceAxtMotionFS20DetectSignal_In0_Origin_RisingEdge,
			EDeviceAxtMotionFS20DetectSignal_In1_Zaxis_RisingEdge,
			EDeviceAxtMotionFS20DetectSignal_In2_General_RisingEdge,
			EDeviceAxtMotionFS20DetectSignal_In3_General_RisingEdge,
			EDeviceAxtMotionFS20DetectSignal_Count,
		};

		enum EDeviceAxtMotionFS20HomeSteps
		{
			EDeviceAxtMotionFS20HomeSteps_1 = 0,
			EDeviceAxtMotionFS20HomeSteps_2,
			EDeviceAxtMotionFS20HomeSteps_3,
			EDeviceAxtMotionFS20HomeSteps_4,
			EDeviceAxtMotionFS20HomeSteps_Count,
		};

		enum EDeviceAxtMotionFS20StopMethod
		{
			EDeviceAxtMotionFS20StopMethod_ReductionStop = 0,
			EDeviceAxtMotionFS20StopMethod_EmergencyStop,
			EDeviceAxtMotionFS20StopMethod_Count,
		};

		enum EDeviceAxtMotionFS20DirectionMethod
		{
			EDeviceAxtMotionFS20DirectionMethod_CW = 0,
			EDeviceAxtMotionFS20DirectionMethod_CCW,
			EDeviceAxtMotionFS20DirectionMethod_Count,
		};
	}
}

// AxlMotion
namespace Ravid
{
	namespace Device
	{
		enum AxlMotionGetFunction
		{
			AxlMotionGetFunction_OK = 0,
			AxlMotionGetFunction_UnknownError,
			AxlMotionGetFunction_NullptrError,
		};

		enum AxlMotionSetFunction
		{
			AxlMotionSetFunction_OK = 0,
			AxlMotionSetFunction_UnknownError,
			AxlMotionSetFunction_AlreadyInitializedError,
			AxlMotionSetFunction_ReadOnDatabaseError,
			AxlMotionSetFunction_WriteToDatabaseError,
			AxlMotionSetFunction_ReadOnDeviceError,
			AxlMotionSetFunction_NotFoundCommandError,
		};

		enum EDeviceAxlMotionPulseOutputMethod
		{
			EDeviceAxlMotionPulseOutputMethod_OneHighLowHigh = 0,
			EDeviceAxlMotionPulseOutputMethod_OneHighHighLow,
			EDeviceAxlMotionPulseOutputMethod_OneLowLowHigh,
			EDeviceAxlMotionPulseOutputMethod_OneLowHighLow,
			EDeviceAxlMotionPulseOutputMethod_TwoCcwCwHigh,
			EDeviceAxlMotionPulseOutputMethod_TwoCcwCwLow,
			EDeviceAxlMotionPulseOutputMethod_TwoCwCcwHigh,
			EDeviceAxlMotionPulseOutputMethod_TwoCwCcwLow,
			EDeviceAxlMotionPulseOutputMethod_TwoPhase,
			EDeviceAxlMotionPulseOutputMethod_TwoPhaseReverse,
			EDeviceAxlMotionPulseOutputMethod_Count,
		};

		enum EDeviceAxlMotionEncoderMethod
		{
			EDeviceAxlMotionEncoderMethod_ObverseUpDownMode = 0,
			EDeviceAxlMotionEncoderMethod_ObverseSqr1Mode,
			EDeviceAxlMotionEncoderMethod_ObverseSqr2Mode,
			EDeviceAxlMotionEncoderMethod_ObverseSqr4Mode,
			EDeviceAxlMotionEncoderMethod_ReverseUpDownMode,
			EDeviceAxlMotionEncoderMethod_ReverseSqr1Mode,
			EDeviceAxlMotionEncoderMethod_ReverseSqr2Mode,
			EDeviceAxlMotionEncoderMethod_ReverseSqr4Mode,
			EDeviceAxlMotionEncoderMethod_Count,
		};

		enum EDeviceAxlMotionLevelMethod
		{
			EDeviceAxlMotionLevelMethod_LOW = 0,
			EDeviceAxlMotionLevelMethod_HIGH,
			EDeviceAxlMotionLevelMethod_Count,
		};

		enum EDeviceAxlMotionDirection
		{
			EDeviceAxlMotionDirection_CCW = 0,
			EDeviceAxlMotionDirection_CW,
			EDeviceAxlMotionDirection_Count,
		};

		enum EDeviceAxlMotionZPhase
		{
			EDeviceAxlMotionZPhase_NotUsed = 0,
			EDeviceAxlMotionZPhase_ReverseHomeDir,
			EDeviceAxlMotionZPhase_HomeDir,
			EDeviceAxlMotionZPhase_Count,
		};

		enum EDeviceAxlMotionSearchSensor
		{
			EDeviceAxlMotionSearchSensor_PosEndLimit = 0,
			EDeviceAxlMotionSearchSensor_NegEndLimit,
			EDeviceAxlMotionSearchSensor_HomeSensor,
			EDeviceAxlMotionSearchSensor_Count,
		};

		enum EDeviceAxlMotionEnableDisable
		{
			EDeviceAxlMotionEnableDisable_Disable = 0,
			EDeviceAxlMotionEnableDisable_Enable,
			EDeviceAxlMotionEnableDisable_Count,
		};

		enum EDeviceAxlMotionStopMode
		{
			EDeviceAxlMotionStopMode_EmergencyStop = 0,
			EDeviceAxlMotionStopMode_SlowdownStop,
			EDeviceAxlMotionStopMode_Count,
		};

		enum EDeviceAxlMotionCounterSelection
		{
			EDeviceAxlMotionCounterSelection_Command = 0,
			EDeviceAxlMotionCounterSelection_Actual,
			EDeviceAxlMotionCounterSelection_Count,
		};
	}
}

// AxlMotionEthercat
namespace Ravid
{
	namespace Device
	{
		enum AxlMotionEthercatGetFunction
		{
			AxlMotionEthercatGetFunction_OK = 0,
			AxlMotionEthercatGetFunction_UnknownError,
			AxlMotionEthercatGetFunction_NullptrError,
		};

		enum AxlMotionEthercatSetFunction
		{
			AxlMotionEthercatSetFunction_OK = 0,
			AxlMotionEthercatSetFunction_UnknownError,
			AxlMotionEthercatSetFunction_AlreadyInitializedError,
			AxlMotionEthercatSetFunction_ReadOnDatabaseError,
			AxlMotionEthercatSetFunction_WriteToDatabaseError,
			AxlMotionEthercatSetFunction_ReadOnDeviceError,
			AxlMotionEthercatSetFunction_NotFoundCommandError,
		};

		enum EDeviceAxlMotionEthercatPulseOutputMethod
		{
			EDeviceAxlMotionEthercatPulseOutputMethod_OneHighLowHigh = 0,
			EDeviceAxlMotionEthercatPulseOutputMethod_OneHighHighLow,
			EDeviceAxlMotionEthercatPulseOutputMethod_OneLowLowHigh,
			EDeviceAxlMotionEthercatPulseOutputMethod_OneLowHighLow,
			EDeviceAxlMotionEthercatPulseOutputMethod_TwoCcwCwHigh,
			EDeviceAxlMotionEthercatPulseOutputMethod_TwoCcwCwLow,
			EDeviceAxlMotionEthercatPulseOutputMethod_TwoCwCcwHigh,
			EDeviceAxlMotionEthercatPulseOutputMethod_TwoCwCcwLow,
			EDeviceAxlMotionEthercatPulseOutputMethod_TwoPhase,
			EDeviceAxlMotionEthercatPulseOutputMethod_TwoPhaseReverse,
			EDeviceAxlMotionEthercatPulseOutputMethod_Count,
		};

		enum EDeviceAxlMotionEthercatEncoderMethod
		{
			EDeviceAxlMotionEthercatEncoderMethod_ObverseUpDownMode = 0,
			EDeviceAxlMotionEthercatEncoderMethod_ObverseSqr1Mode,
			EDeviceAxlMotionEthercatEncoderMethod_ObverseSqr2Mode,
			EDeviceAxlMotionEthercatEncoderMethod_ObverseSqr4Mode,
			EDeviceAxlMotionEthercatEncoderMethod_ReverseUpDownMode,
			EDeviceAxlMotionEthercatEncoderMethod_ReverseSqr1Mode,
			EDeviceAxlMotionEthercatEncoderMethod_ReverseSqr2Mode,
			EDeviceAxlMotionEthercatEncoderMethod_ReverseSqr4Mode,
			EDeviceAxlMotionEthercatEncoderMethod_Count,
		};

		enum EDeviceAxlMotionEthercatLevelMethod
		{
			EDeviceAxlMotionEthercatLevelMethod_LOW = 0,
			EDeviceAxlMotionEthercatLevelMethod_HIGH,
			EDeviceAxlMotionEthercatLevelMethod_Count,
		};

		enum EDeviceAxlMotionEthercatDirection
		{
			EDeviceAxlMotionEthercatDirection_CCW = 0,
			EDeviceAxlMotionEthercatDirection_CW,
			EDeviceAxlMotionEthercatDirection_Count,
		};

		enum EDeviceAxlMotionEthercatZPhase
		{
			EDeviceAxlMotionEthercatZPhase_NotUsed = 0,
			EDeviceAxlMotionEthercatZPhase_ReverseHomeDir,
			EDeviceAxlMotionEthercatZPhase_HomeDir,
			EDeviceAxlMotionEthercatZPhase_Count,
		};

		enum EDeviceAxlMotionEthercatSearchSensor
		{
			EDeviceAxlMotionEthercatSearchSensor_PosEndLimit = 0,
			EDeviceAxlMotionEthercatSearchSensor_NegEndLimit,
			EDeviceAxlMotionEthercatSearchSensor_HomeSensor,
			EDeviceAxlMotionEthercatSearchSensor_EncoderZPhase,
			EDeviceAxlMotionEthercatSearchSensor_Count,
		};

		enum EDeviceAxlMotionEthercatEnableDisable
		{
			EDeviceAxlMotionEthercatEnableDisable_Disable = 0,
			EDeviceAxlMotionEthercatEnableDisable_Enable,
			EDeviceAxlMotionEthercatEnableDisable_Count,
		};

		enum EDeviceAxlMotionEthercatStopMode
		{
			EDeviceAxlMotionEthercatStopMode_EmergencyStop = 0,
			EDeviceAxlMotionEthercatStopMode_SlowdownStop,
			EDeviceAxlMotionEthercatStopMode_Count,
		};

		enum EDeviceAxlMotionEthercatCounterSelection
		{
			EDeviceAxlMotionEthercatCounterSelection_Command = 0,
			EDeviceAxlMotionEthercatCounterSelection_Actual,
			EDeviceAxlMotionEthercatCounterSelection_Count,
		};
	}
}

// Point Grey
namespace Ravid
{
	namespace Device
	{
		enum EPointGreyGetFunction
		{
			EPointGreyGetFunction_OK = 0,
			EPointGreyGetFunction_UnknownError,
			EPointGreyGetFunction_NullptrError,
			EPointGreyGetFunction_EnumTypeError, //범위 벗어나는 경우
		};

		enum EPointGreySetFunction
		{
			EPointGreySetFunction_OK = 0,
			EPointGreySetFunction_UnknownError,
			EPointGreySetFunction_NotFoundApiError,
			EPointGreySetFunction_AlreadyInitializedError,
			EPointGreySetFunction_NotInitializedError,
			EPointGreySetFunction_NotFindCommandError, // _CommandDoesNotExist
			EPointGreySetFunction_DeviceNullptrError,
			EPointGreySetFunction_NotSupportError,
			EPointGreySetFunction_FunctionLockedError,
			EPointGreySetFunction_WriteToDeviceError,
			EPointGreySetFunction_WriteToDatabaseError,
			EPointGreySetFunction_ReadOnDatabaseError,
			EPointGreySetFunction_ReadOnDeviceError,
			EPointGreySetFunction_ActivatedDeviceError,

		};

		enum EPointGreySetDeviceID
		{
			EPointGreySetDeviceID_OK = 0,
			EPointGreySetDeviceID_UnknownError,
			EPointGreySetDeviceID_WriteToDeviceError,
			EPointGreySetDeviceID_WriteToDatabaseError,
		};

		enum EPointGreySetGrabCount
		{
			EPointGreySetGrabCount_OK = 0,
			EPointGreySetGrabCount_UnknownError,
			EPointGreySetGrabCount_WriteToDeviceError,
			EPointGreySetGrabCount_WriteToDatabaseError,
		};

		enum EPointGreySetTriggerOnOff
		{
			EPointGreySetTriggerOnOff_OK = 0,
			EPointGreySetTriggerOnOff_UnknownError,
			EPointGreySetTriggerOnOff_WriteToDeviceError,
			EPointGreySetTriggerOnOff_WriteToDatabaseError,
		};

		enum EPointGreySetBrightness
		{
			EPointGreySetBrightness_OK = 0,
			EPointGreySetBrightness_UnknownError,
			EPointGreySetBrightness_WriteToDeviceError,
			EPointGreySetBrightness_WriteToDatabaseError,
		};

		enum EPointGreySetExposure
		{
			EPointGreySetExposure_OK = 0,
			EPointGreySetExposure_UnknownError,
			EPointGreySetExposure_WriteToDeviceError,
			EPointGreySetExposure_WriteToDatabaseError,
		};

		enum EPointGreySetSharpness
		{
			EPointGreySetSharpness_OK = 0,
			EPointGreySetSharpness_UnknownError,
			EPointGreySetSharpness_WriteToDeviceError,
			EPointGreySetSharpness_WriteToDatabaseError,
		};

		enum EPointGreySetWhiteBalance
		{
			EPointGreySetWhiteBalance_OK = 0,
			EPointGreySetWhiteBalance_UnknownError,
			EPointGreySetWhiteBalance_WriteToDeviceError,
			EPointGreySetWhiteBalance_WriteToDatabaseError,
		};

		enum EPointGreySetHue
		{
			EPointGreySetHue_OK = 0,
			EPointGreySetHue_UnknownError,
			EPointGreySetHue_WriteToDeviceError,
			EPointGreySetHue_WriteToDatabaseError,
		};

		enum EPointGreySetSaturation
		{
			EPointGreySetSaturation_OK = 0,
			EPointGreySetSaturation_UnknownError,
			EPointGreySetSaturation_WriteToDeviceError,
			EPointGreySetSaturation_WriteToDatabaseError,
		};

		enum EPointGreySetGamma
		{
			EPointGreySetGamma_OK = 0,
			EPointGreySetGamma_UnknownError,
			EPointGreySetGamma_WriteToDeviceError,
			EPointGreySetGamma_WriteToDatabaseError,
		};

		enum EPointGreySetIris
		{
			EPointGreySetIris_OK = 0,
			EPointGreySetIris_UnknownError,
			EPointGreySetIris_WriteToDeviceError,
			EPointGreySetIris_WriteToDatabaseError,
		};

		enum EPointGreySetFocus
		{
			EPointGreySetFocus_OK = 0,
			EPointGreySetFocus_UnknownError,
			EPointGreySetFocus_WriteToDeviceError,
			EPointGreySetFocus_WriteToDatabaseError,
		};

		enum EPointGreySetZoom
		{
			EPointGreySetZoom_OK = 0,
			EPointGreySetZoom_UnknownError,
			EPointGreySetZoom_WriteToDeviceError,
			EPointGreySetZoom_WriteToDatabaseError,
		};

		enum EPointGreySetPan
		{
			EPointGreySetPan_OK = 0,
			EPointGreySetPan_UnknownError,
			EPointGreySetPan_WriteToDeviceError,
			EPointGreySetPan_WriteToDatabaseError,
		};

		enum EPointGreySetTilt
		{
			EPointGreySetTilt_OK = 0,
			EPointGreySetTilt_UnknownError,
			EPointGreySetTilt_WriteToDeviceError,
			EPointGreySetTilt_WriteToDatabaseError,
		};

		enum EPointGreySetShutter
		{
			EPointGreySetShutter_OK = 0,
			EPointGreySetShutter_UnknownError,
			EPointGreySetShutter_WriteToDeviceError,
			EPointGreySetShutter_WriteToDatabaseError,
		};

		enum EPointGreySetGain
		{
			EPointGreySetGain_OK = 0,
			EPointGreySetGain_UnknownError,
			EPointGreySetGain_WriteToDeviceError,
			EPointGreySetGain_WriteToDatabaseError,
		};

		enum EPointGreySetTriggerMode
		{
			EPointGreySetTriggerMode_OK = 0,
			EPointGreySetTriggerMode_UnknownError,
			EPointGreySetTriggerMode_WriteToDeviceError,
			EPointGreySetTriggerMode_WriteToDatabaseError,
		};

		enum EPointGreySetTriggerSource
		{
			EPointGreySetTriggerSource_OK = 0,
			EPointGreySetTriggerSource_UnknownError,
			EPointGreySetTriggerSource_WriteToDeviceError,
			EPointGreySetTriggerSource_WriteToDatabaseError,
		};

		enum EPointGreySetTriggerParameter
		{
			EPointGreySetTriggerParameter_OK = 0,
			EPointGreySetTriggerParameter_UnknownError,
			EPointGreySetTriggerParameter_WriteToDeviceError,
			EPointGreySetTriggerParameter_WriteToDatabaseError,
		};

		enum EPointGreySetTriggerPolarity
		{
			EPointGreySetTriggerPolarity_OK = 0,
			EPointGreySetTriggerPolarity_UnknownError,
			EPointGreySetTriggerPolarity_WriteToDeviceError,
			EPointGreySetTriggerPolarity_WriteToDatabaseError,
		};

		enum EPointGreySetTriggerDelay
		{
			EPointGreySetTriggerDelay_OK = 0,
			EPointGreySetTriggerDelay_UnknownError,
			EPointGreySetTriggerDelay_WriteToDeviceError,
			EPointGreySetTriggerDelay_WriteToDatabaseError,
		};

		enum EPointGreySetTemperature
		{
			EPointGreySetTemperature_OK = 0,
			EPointGreySetTemperature_UnknownError,
			EPointGreySetTemperature_WriteToDeviceError,
			EPointGreySetTemperature_WriteToDatabaseError,
		};

		enum EPointGreySetCanvas
		{
			EPointGreySetCanvas_OK = 0,
			EPointGreySetCanvas_UnknownError,
			EPointGreySetCanvas_InitializedDeviceError,
			EPointGreySetCanvas_WriteToDatabaseError,
		};

		enum EPointGreySetOffsetCenter
		{
			EPointGreySetOffsetCenter_OK = 0,
			EPointGreySetOffsetCenter_UnknownError,
			EPointGreySetOffsetCenter_NotSupportedError,
			EPointGreySetOffsetCenter_WriteToDeviceError,
		};

		enum EPointGreySetOffset
		{
			EPointGreySetOffset_OK = 0,
			EPointGreySetOffset_UnknownError,
			EPointGreySetOffset_NotSupportedError,
			EPointGreySetOffset_WriteToDeviceError,
		};

		enum EPointGreySetGrabWaitingTime
		{
			EPointGreySetGrabWaitingTime_OK = 0,
			EPointGreySetGrabWaitingTime_UnknownError,
			EPointGreySetGrabWaitingTime_ActivatedDeviceError,
			EPointGreySetGrabWaitingTime_NotSupportedError,
			EPointGreySetGrabWaitingTime_WriteToDeviceError,
			EPointGreySetGrabWaitingTime_WriteToDatabaseError,
		};

		enum EPointGreySetCanvasWidth
		{
			EPointGreySetCanvasWidth_OK = 0,
			EPointGreySetCanvasWidth_UnknownError,
			EPointGreySetCanvasWidth_ActivatedDeviceError,
			EPointGreySetCanvasWidth_NotSupportedError,
			EPointGreySetCanvasWidth_WriteToDeviceError,
			EPointGreySetCanvasWidth_WriteToDatabaseError,
		};

		enum EPointGreySetCanvasHeight
		{
			EPointGreySetCanvasHeight_OK = 0,
			EPointGreySetCanvasHeight_UnknownError,
			EPointGreySetCanvasHeight_ActivatedDeviceError,
			EPointGreySetCanvasHeight_NotSupportedError,
			EPointGreySetCanvasHeight_WriteToDeviceError,
			EPointGreySetCanvasHeight_WriteToDatabaseError,
		};

		enum EPointGreySetOffsetX
		{
			EPointGreySetOffsetX_OK = 0,
			EPointGreySetOffsetX_UnknownError,
			EPointGreySetOffsetX_ActivatedDeviceError,
			EPointGreySetOffsetX_NotSupportedError,
			EPointGreySetOffsetX_WriteToDeviceError,
			EPointGreySetOffsetX_WriteToDatabaseError,
		};

		enum EPointGreySetOffsetY
		{
			EPointGreySetOffsetY_OK = 0,
			EPointGreySetOffsetY_UnknownError,
			EPointGreySetOffsetY_ActivatedDeviceError,
			EPointGreySetOffsetY_NotSupportedError,
			EPointGreySetOffsetY_WriteToDeviceError,
			EPointGreySetOffsetY_WriteToDatabaseError,
		};

		enum EPointGreySetCustomFrameRate
		{
			EPointGreySetCustomFrameRate_OK = 0,
			EPointGreySetCustomFrameRate_UnknownError,
			EPointGreySetCustomFrameRate_ActivatedDeviceError,
			EPointGreySetCustomFrameRate_NotSupportedError,
			EPointGreySetCustomFrameRate_WriteToDeviceError,
			EPointGreySetCustomFrameRate_WriteToDatabaseError,
		};

		//enum EPointGreySetExposure
		//{
		//	EPointGreySetExposure_OK = 0,
		//	EPointGreySetExposure_UnknownError,
		//	EPointGreySetExposure_NotSupportedError,
		//	EPointGreySetExposure_WriteToDeviceError,
		//	EPointGreySetExposure_WriteToDatabaseError,
		//};

		enum EPointGreySetPacketSize
		{
			EPointGreySetPacketSize_OK = 0,
			EPointGreySetPacketSize_UnknownError,
			EPointGreySetPacketSize_WriteToDeviceError,
		};

		enum EPointGreySetPixelFormat
		{
			EPointGreySetPixelFormat_OK = 0,
			EPointGreySetPixelFormat_UnknownError,
			EPointGreySetPixelFormat_InitalizedDeviceError,
			EPointGreySetPixelFormat_WriteToDatabaseError,
			EPointGreySetPixelFormat_EnumTypeError,
		};

		enum EPointGreySetTriggerSelector
		{
			EPointGreySetTriggerSelector_OK = 0,
			EPointGreySetTriggerSelector_UnknownError,
			EPointGreySetTriggerSelector_NotSupportedError,
			EPointGreySetTriggerSelector_WriteToDeviceError,
			EPointGreySetTriggerSelector_WriteToDatabaseError,
		};

		enum EPointGreySetStartMode // EPointGreyStartModeResult
		{
			EPointGreySetStartMode_OK = 0,
			EPointGreySetStartMode_UnknownError,
			EPointGreySetStartMode_ActivatedDeviceError,
			EPointGreySetStartMode_NotSupportedError,
			EPointGreySetStartMode_WriteToDeviceError,
			EPointGreySetStartMode_WriteToDatabaseError,
		};

		enum EPointGreySetStartTriggerSource
		{
			EPointGreySetStartTriggerSource_OK = 0,
			EPointGreySetStartTriggerSource_UnknownError,
			EPointGreySetStartTriggerSource_WriteToDeviceError,
			EPointGreySetStartTriggerSource_WriteToDatabaseError,
		};

		enum EPointGreySetStartTriggerActivation
		{
			EPointGreySetStartTriggerActivation_OK = 0,
			EPointGreySetStartTriggerActivation_UnknownError,
			EPointGreySetStartTriggerActivation_WriteToDeviceError,
			EPointGreySetStartTriggerActivation_NotSupportedError,
			EPointGreySetStartTriggerActivation_WriteToDatabaseError,
		};

		enum EPointGreySetExposureAuto
		{
			EPointGreySetExposureAuto_OK = 0,
			EPointGreySetExposureAuto_UnknownError,
			EPointGreySetExposureAuto_NotSupportedError,
			EPointGreySetExposureAuto_WriteToDeviceError,
			EPointGreySetExposureAuto_WriteToDatabaseError,
			EPointGreySetExposureAuto_ReadOnDatabaseError,

		};

		enum EPointGreySetGainAuto
		{
			EPointGreySetGainAuto_OK = 0,
			EPointGreySetGainAuto_UnknownError,
			EPointGreySetGainAuto_NotSupportedError,
			EPointGreySetGainAuto_WriteToDeviceError,
			EPointGreySetGainAuto_ReadOnDatabaseError,
			EPointGreySetGainAuto_WriteToDatabaseError,
		};

		enum EPointGreySetGainSelector
		{
			EPointGreySetGainSelector_OK = 0,
			EPointGreySetGainSelector_UnknownError,
			EPointGreySetGainSelector_NotSupportedError,
			EPointGreySetGainSelector_WriteToDeviceError,
			EPointGreySetGainSelector_ReadOnDatabaseError,
			EPointGreySetGainSelector_WriteToDatabaseError,
		};
		enum EDevicePointGreyFrameRate
		{
			EDevicePointGreyFrameRate_1_875 = 0,
			EDevicePointGreyFrameRate_3_75,
			EDevicePointGreyFrameRate_7_5,
			EDevicePointGreyFrameRate_15,
			EDevicePointGreyFrameRate_30,
			EDevicePointGreyFrameRate_60,
			EDevicePointGreyFrameRate_120,
			EDevicePointGreyFrameRate_240,
			EDevicePointGreyFrameRate_Custom,
			EDevicePointGreyFrameRate_Count,
		};

		enum EDevicePointGreyPixelFormat
		{
			EDevicePointGreyPixelFormat_MONO8 = 0,
			EDevicePointGreyPixelFormat_411YUV8,
			EDevicePointGreyPixelFormat_422YUV8,
			EDevicePointGreyPixelFormat_444YUV8,
			EDevicePointGreyPixelFormat_RGB8,
			EDevicePointGreyPixelFormat_MONO16,
			EDevicePointGreyPixelFormat_RGB16,
			EDevicePointGreyPixelFormat_S_MONO16,
			EDevicePointGreyPixelFormat_S_RGB16,
			EDevicePointGreyPixelFormat_RAW8,
			EDevicePointGreyPixelFormat_RAW16,
			EDevicePointGreyPixelFormat_MONO12,
			EDevicePointGreyPixelFormat_RAW12,
			EDevicePointGreyPixelFormat_BGR,
			EDevicePointGreyPixelFormat_BGRU,
			EDevicePointGreyPixelFormat_RGB,
			EDevicePointGreyPixelFormat_RGBU,
			EDevicePointGreyPixelFormat_BGR16,
			EDevicePointGreyPixelFormat_BGRU16,
			EDevicePointGreyPixelFormat_422YUV8_JPEG,
			EDevicePointGreyPixelFormat_Count,
		};

		enum EDevicePointGreyImagemode
		{
			EDevicePointGreyImagemode_0 = 0,
			EDevicePointGreyImagemode_1,
			EDevicePointGreyImagemode_2,
			EDevicePointGreyImagemode_3,
			EDevicePointGreyImagemode_4,
			EDevicePointGreyImagemode_5,
			EDevicePointGreyImagemode_6,
			EDevicePointGreyImagemode_7,
			EDevicePointGreyImagemode_8,
			EDevicePointGreyImagemode_9,
			EDevicePointGreyImagemode_10,
			EDevicePointGreyImagemode_11,
			EDevicePointGreyImagemode_12,
			EDevicePointGreyImagemode_13,
			EDevicePointGreyImagemode_14,
			EDevicePointGreyImagemode_15,
			EDevicePointGreyImagemode_16,
			EDevicePointGreyImagemode_17,
			EDevicePointGreyImagemode_18,
			EDevicePointGreyImagemode_19,
			EDevicePointGreyImagemode_20,
			EDevicePointGreyImagemode_21,
			EDevicePointGreyImagemode_22,
			EDevicePointGreyImagemode_23,
			EDevicePointGreyImagemode_24,
			EDevicePointGreyImagemode_25,
			EDevicePointGreyImagemode_26,
			EDevicePointGreyImagemode_27,
			EDevicePointGreyImagemode_28,
			EDevicePointGreyImagemode_29,
			EDevicePointGreyImagemode_30,
			EDevicePointGreyImagemode_31,
			EDevicePointGreyImagemode_Count,
		};

		enum EDevicePointGreyGrabMode
		{
			EDevicePointGreyGrabMode_DROP_FRAMES = 0,
			EDevicePointGreyGrabMode_BUFFER_FRAMES,
			EDevicePointGreyGrabMode_Count,
		};

		enum EDevicePointGreyVideoMode
		{
			EDevicePointGreyVideoMode_160x120YUV444 = 0,
			EDevicePointGreyVideoMode_320x240YUV422,
			EDevicePointGreyVideoMode_640x480YUV411,
			EDevicePointGreyVideoMode_640x480YUV422,
			EDevicePointGreyVideoMode_640x480RGB,
			EDevicePointGreyVideoMode_640x480Y8,
			EDevicePointGreyVideoMode_640x480Y16,
			EDevicePointGreyVideoMode_800x600YUV422,
			EDevicePointGreyVideoMode_800x600RGB,
			EDevicePointGreyVideoMode_800x600Y8,
			EDevicePointGreyVideoMode_800x600Y16,
			EDevicePointGreyVideoMode_1024x768YUV422,
			EDevicePointGreyVideoMode_1024x768RGB,
			EDevicePointGreyVideoMode_1024x768Y8,
			EDevicePointGreyVideoMode_1024x768Y16,
			EDevicePointGreyVideoMode_1280x960YUV422,
			EDevicePointGreyVideoMode_1280x960RGB,
			EDevicePointGreyVideoMode_1280x960Y8,
			EDevicePointGreyVideoMode_1280x960Y16,
			EDevicePointGreyVideoMode_1600x1200YUV42,
			EDevicePointGreyVideoMode_1600x1200RGB,
			EDevicePointGreyVideoMode_1600x1200Y8,
			EDevicePointGreyVideoMode_1600x1200Y16,
			EDevicePointGreyVideoMode_FORMAT7,
			EDevicePointGreyVideoMode_Count,
		};

		enum EDevicePointGreySwitch
		{
			EDevicePointGreySwitch_CustomValue = 0,
			EDevicePointGreySwitch_Auto,
			EDevicePointGreySwitch_OnePush,
			EDevicePointGreySwitch_Count,
		};

		enum EDevicePointGreyTriggerPolarity
		{
			EDevicePointGreyTriggerPolarity_Low = 0, //Trigger active low
			EDevicePointGreyTriggerPolarity_High, //Trigger active high
			EDevicePointGreyTriggerPolarity_Count,
		};

		enum EDevicePointGreyTriggerMode
		{
			EDevicePointGreyTriggerMode_StandardExternal = 0,
			EDevicePointGreyTriggerMode_BulbShutter,
			EDevicePointGreyTriggerMode_SkipFrames,
			EDevicePointGreyTriggerMode_MultipleExposurePreset,
			EDevicePointGreyTriggerMode_MultipleExposurePulseWidth,
			EDevicePointGreyTriggerMode_LowSmearTrigger,
			EDevicePointGreyTriggerMode_OverlappedExposureReadout,
			EDevicePointGreyTriggerMode_MultiShot,
			EDevicePointGreyTriggerMode_Count,
		};

	}
}

// Euresys
namespace Ravid
{
	namespace Device
	{

		enum EEuresysGetFunction
		{
			EEuresysGetFunction_OK = 0,
			EEuresysGetFunction_UnknownError,
			EEuresysGetFunction_EnumTypeError,
			EEuresysGetFunction_NullptrError,
		};

		enum EEuresysSetFunction
		{
			EEuresysSetFunction_OK = 0,
			EEuresysSetFunction_UnknownError,
			EEuresysSetFunction_AlreadyGrabError,
			EEuresysSetFunction_NotFoundApiError,
			EEuresysSetFunction_AlreadyInitializedError,
			EEuresysSetFunction_NotInitializedError,
			EEuresysSetFunction_NotFindCommandError, // _CommandDoesNotExist
			EEuresysSetFunction_DeviceNullptrError,
			EEuresysSetFunction_NotSupportError,
			EEuresysSetFunction_FunctionLockedError,
			EEuresysSetFunction_WriteToDeviceError,
			EEuresysSetFunction_WriteToDatabaseError,
			EEuresysSetFunction_ReadOnDatabaseError,
			EEuresysSetFunction_ReadOnDeviceError,
			EEuresysSetFunction_ActivatedDeviceError,
		};

		enum EDeviceMulticamInitializeType
		{
			EDeviceMulticamInitializeType_Camfile = 0,
			EDeviceMulticamInitializeType_Parameter,
			EDeviceMulticamInitializeType_Count,
		};

		enum EDeviceMulticamSwitch
		{
			EDeviceMulticamSwitch_Off = 0,
			EDeviceMulticamSwitch_On,
			EDeviceMulticamSwitch_Count,
		};
	}
}

// Euresys - Grablink
namespace Ravid
{
	namespace Device
	{
		enum EDeviceGrablinkTopology
		{
			EDeviceGrablinkTopology_0_2 = 0,
			EDeviceGrablinkTopology_0_3,
			EDeviceGrablinkTopology_0_3_1,
			EDeviceGrablinkTopology_1,
			EDeviceGrablinkTopology_1_,
			EDeviceGrablinkTopology_1_01_2,
			EDeviceGrablinkTopology_1_1,
			EDeviceGrablinkTopology_1_1_1,
			EDeviceGrablinkTopology_1_1_1_1,
			EDeviceGrablinkTopology_1_1_2,
			EDeviceGrablinkTopology_1_11,
			EDeviceGrablinkTopology_1_11_0,
			EDeviceGrablinkTopology_1_2,
			EDeviceGrablinkTopology_1_2_0,
			EDeviceGrablinkTopology_1_3,
			EDeviceGrablinkTopology_1_3_1,
			EDeviceGrablinkTopology_11_11,
			EDeviceGrablinkTopology_11_11_11,
			EDeviceGrablinkTopology_11_11_2,
			EDeviceGrablinkTopology_11_3_1,
			EDeviceGrablinkTopology_2_0,
			EDeviceGrablinkTopology_2_1,
			EDeviceGrablinkTopology_2_11,
			EDeviceGrablinkTopology_2_2,
			EDeviceGrablinkTopology_2_2_0,
			EDeviceGrablinkTopology_2_2_1,
			EDeviceGrablinkTopology_2_2_11,
			EDeviceGrablinkTopology_2_2_2,
			EDeviceGrablinkTopology_2_3_1,
			EDeviceGrablinkTopology_3_0,
			EDeviceGrablinkTopology_3_1,
			EDeviceGrablinkTopology_3_3_0,
			EDeviceGrablinkTopology_BASE_MEDIUM_FULL,
			EDeviceGrablinkTopology_DECA,
			EDeviceGrablinkTopology_DUO,
			EDeviceGrablinkTopology_DUO_EXT1,
			EDeviceGrablinkTopology_DUO_OPT1,
			EDeviceGrablinkTopology_DUO_SLOW,
			EDeviceGrablinkTopology_ECCO,
			EDeviceGrablinkTopology_ECCO_DECA,
			EDeviceGrablinkTopology_ECCO_OFF,
			EDeviceGrablinkTopology_MONO,
			EDeviceGrablinkTopology_MONO_DECA,
			EDeviceGrablinkTopology_MONO_DECA_AFE,
			EDeviceGrablinkTopology_MONO_DECA_AFE_SE1,
			EDeviceGrablinkTopology_MONO_DECA_DFE,
			EDeviceGrablinkTopology_MONO_DECA_EF1,
			EDeviceGrablinkTopology_MONO_DECA_EXT1,
			EDeviceGrablinkTopology_MONO_DECA_OPT1,
			EDeviceGrablinkTopology_MONO_EF1,
			EDeviceGrablinkTopology_MONO_EXT1,
			EDeviceGrablinkTopology_MONO_From20MHzTo35MHz,
			EDeviceGrablinkTopology_MONO_From30MHzTo70MHz,
			EDeviceGrablinkTopology_MONO_From60MHzTo85MHz,
			EDeviceGrablinkTopology_MONO_OPT1,
			EDeviceGrablinkTopology_MONO_SLOW,
			EDeviceGrablinkTopology_Count,
		};

		enum EDeviceGrablinkConnector
		{
			EDeviceGrablinkConnector_A = 0,
			EDeviceGrablinkConnector_B,
			EDeviceGrablinkConnector_C,
			EDeviceGrablinkConnector_CAM0,
			EDeviceGrablinkConnector_CAM1,
			EDeviceGrablinkConnector_CAM2,
			EDeviceGrablinkConnector_CAM3,
			EDeviceGrablinkConnector_D,
			EDeviceGrablinkConnector_M,
			EDeviceGrablinkConnector_STEREOXY,
			EDeviceGrablinkConnector_UNDEFINED,
			EDeviceGrablinkConnector_VID1,
			EDeviceGrablinkConnector_VID1_COMP,
			EDeviceGrablinkConnector_VID10,
			EDeviceGrablinkConnector_VID11,
			EDeviceGrablinkConnector_VID12,
			EDeviceGrablinkConnector_VID13,
			EDeviceGrablinkConnector_VID14,
			EDeviceGrablinkConnector_VID15,
			EDeviceGrablinkConnector_VID16,
			EDeviceGrablinkConnector_VID2,
			EDeviceGrablinkConnector_VID2_COMP,
			EDeviceGrablinkConnector_VID3,
			EDeviceGrablinkConnector_VID3_COMP,
			EDeviceGrablinkConnector_VID4,
			EDeviceGrablinkConnector_VID4_COMP,
			EDeviceGrablinkConnector_VID5,
			EDeviceGrablinkConnector_VID6,
			EDeviceGrablinkConnector_VID7,
			EDeviceGrablinkConnector_VID8,
			EDeviceGrablinkConnector_VID9,
			EDeviceGrablinkConnector_X,
			EDeviceGrablinkConnector_X_Y,
			EDeviceGrablinkConnector_X1,
			EDeviceGrablinkConnector_X2,
			EDeviceGrablinkConnector_XBIS,
			EDeviceGrablinkConnector_Y,
			EDeviceGrablinkConnector_Y1,
			EDeviceGrablinkConnector_Y2,
			EDeviceGrablinkConnector_YBIS,
			EDeviceGrablinkConnector_YC,
			EDeviceGrablinkConnector_YC1,
			EDeviceGrablinkConnector_YC2,
			EDeviceGrablinkConnector_YC3,
			EDeviceGrablinkConnector_YC4,
			EDeviceGrablinkConnector_Z,
			EDeviceGrablinkConnector_ZBIS,
			EDeviceGrablinkConnector_Count,
		};

		enum EDeviceGrablinkAcquisitionMode
		{
			EDeviceGrablinkAcquisitionMode_HFR = 0,
			EDeviceGrablinkAcquisitionMode_INVALID,
			EDeviceGrablinkAcquisitionMode_LONGPAGE,
			EDeviceGrablinkAcquisitionMode_PAGE,
			EDeviceGrablinkAcquisitionMode_SNAPSHOT,
			EDeviceGrablinkAcquisitionMode_VIDEO,
			EDeviceGrablinkAcquisitionMode_WEB,
			EDeviceGrablinkAcquisitionMode_Count,
		};

		enum EDeviceGrablinkTrigMode
		{
			EDeviceGrablinkTrigMode_AUTO = 0,
			EDeviceGrablinkTrigMode_COMBINED,
			EDeviceGrablinkTrigMode_ENC,
			EDeviceGrablinkTrigMode_EXT,
			EDeviceGrablinkTrigMode_EXTRC,
			EDeviceGrablinkTrigMode_HARD,
			EDeviceGrablinkTrigMode_IMMEDIATE,
			EDeviceGrablinkTrigMode_INT,
			EDeviceGrablinkTrigMode_MASTER_CHANNEL,
			EDeviceGrablinkTrigMode_PAUSE,
			EDeviceGrablinkTrigMode_SLAVE,
			EDeviceGrablinkTrigMode_SOFT,
			EDeviceGrablinkTrigMode_Count,
		};

		enum EDeviceGrablinkLineCaptureMode
		{
			EDeviceGrablinkLineCaptureMode_ADR = 0,
			EDeviceGrablinkLineCaptureMode_ALL,
			EDeviceGrablinkLineCaptureMode_PICK,
			EDeviceGrablinkLineCaptureMode_TAG,
			EDeviceGrablinkLineCaptureMode_Count,
		};

		enum EDeviceGrablinkLineRateMode
		{
			EDeviceGrablinkLineRateMode_CAMERA = 0,
			EDeviceGrablinkLineRateMode_CONVERT,
			EDeviceGrablinkLineRateMode_EXPOSE,
			EDeviceGrablinkLineRateMode_MASTER_CHANNEL,
			EDeviceGrablinkLineRateMode_PERIOD,
			EDeviceGrablinkLineRateMode_PULSE,
			EDeviceGrablinkLineRateMode_SLAVE,
			EDeviceGrablinkLineRateMode_Count,
		};

		enum EDeviceGrablinkCamConfig
		{
			EDeviceGrablinkCamConfig_FACTORY = 0,
			EDeviceGrablinkCamConfig_I10SC,
			EDeviceGrablinkCamConfig_I120RA,
			EDeviceGrablinkCamConfig_I120SA,
			EDeviceGrablinkCamConfig_I120SM,
			EDeviceGrablinkCamConfig_I15RA,
			EDeviceGrablinkCamConfig_I15SA,
			EDeviceGrablinkCamConfig_I25SA,
			EDeviceGrablinkCamConfig_I30SA,
			EDeviceGrablinkCamConfig_I50RA,
			EDeviceGrablinkCamConfig_I50SA,
			EDeviceGrablinkCamConfig_I50SM,
			EDeviceGrablinkCamConfig_I50SM_R,
			EDeviceGrablinkCamConfig_I60RA,
			EDeviceGrablinkCamConfig_I60SA,
			EDeviceGrablinkCamConfig_I60SM,
			EDeviceGrablinkCamConfig_I60SM_R,
			EDeviceGrablinkCamConfig_I7RA,
			EDeviceGrablinkCamConfig_I7SA,
			EDeviceGrablinkCamConfig_IxxRM,
			EDeviceGrablinkCamConfig_IxxRM_4,
			EDeviceGrablinkCamConfig_IxxSA,
			EDeviceGrablinkCamConfig_IxxSA_D,
			EDeviceGrablinkCamConfig_IxxSC,
			EDeviceGrablinkCamConfig_IxxSD,
			EDeviceGrablinkCamConfig_IxxSM,
			EDeviceGrablinkCamConfig_IxxSM_L,
			EDeviceGrablinkCamConfig_IxxSM_R,
			EDeviceGrablinkCamConfig_L1024RC,
			EDeviceGrablinkCamConfig_L1024RG,
			EDeviceGrablinkCamConfig_L1024RG_Base,
			EDeviceGrablinkCamConfig_L1024RG_Medium,
			EDeviceGrablinkCamConfig_L1024RGM,
			EDeviceGrablinkCamConfig_L1024RP,
			EDeviceGrablinkCamConfig_L1024SC,
			EDeviceGrablinkCamConfig_L1024SP,
			EDeviceGrablinkCamConfig_L12500RP,
			EDeviceGrablinkCamConfig_L1365RG,
			EDeviceGrablinkCamConfig_L2048RC,
			EDeviceGrablinkCamConfig_L2048RG,
			EDeviceGrablinkCamConfig_L2048RG_Base,
			EDeviceGrablinkCamConfig_L2048RG_Medium,
			EDeviceGrablinkCamConfig_L2048RGM,
			EDeviceGrablinkCamConfig_L2048RP,
			EDeviceGrablinkCamConfig_L2048SC,
			EDeviceGrablinkCamConfig_L2048SP,
			EDeviceGrablinkCamConfig_L2098RC,
			EDeviceGrablinkCamConfig_L2098RG,
			EDeviceGrablinkCamConfig_L2098RP,
			EDeviceGrablinkCamConfig_L2098SC,
			EDeviceGrablinkCamConfig_L2098SP,
			EDeviceGrablinkCamConfig_L4080RC,
			EDeviceGrablinkCamConfig_L4080RG,
			EDeviceGrablinkCamConfig_L4080RP,
			EDeviceGrablinkCamConfig_L4080SC,
			EDeviceGrablinkCamConfig_L4080SP,
			EDeviceGrablinkCamConfig_L4096RC,
			EDeviceGrablinkCamConfig_L4096RG,
			EDeviceGrablinkCamConfig_L4096RGS,
			EDeviceGrablinkCamConfig_L4096RP,
			EDeviceGrablinkCamConfig_L4096SC,
			EDeviceGrablinkCamConfig_L4096SP,
			EDeviceGrablinkCamConfig_L512RC,
			EDeviceGrablinkCamConfig_L512RG,
			EDeviceGrablinkCamConfig_L512RP,
			EDeviceGrablinkCamConfig_L512SC,
			EDeviceGrablinkCamConfig_L512SP,
			EDeviceGrablinkCamConfig_L5150RP,
			EDeviceGrablinkCamConfig_L5150SP,
			EDeviceGrablinkCamConfig_L6144RC,
			EDeviceGrablinkCamConfig_L6144RG,
			EDeviceGrablinkCamConfig_L6144RP,
			EDeviceGrablinkCamConfig_L6144SC,
			EDeviceGrablinkCamConfig_L6144SP,
			EDeviceGrablinkCamConfig_L7400RP,
			EDeviceGrablinkCamConfig_L7400SP,
			EDeviceGrablinkCamConfig_L7500RC,
			EDeviceGrablinkCamConfig_L7500RG,
			EDeviceGrablinkCamConfig_L7500RP,
			EDeviceGrablinkCamConfig_L7500SC,
			EDeviceGrablinkCamConfig_L7500SP,
			EDeviceGrablinkCamConfig_L8192RC,
			EDeviceGrablinkCamConfig_L8192RG,
			EDeviceGrablinkCamConfig_L8192RP,
			EDeviceGrablinkCamConfig_L8192SC,
			EDeviceGrablinkCamConfig_L8192SP,
			EDeviceGrablinkCamConfig_LxxxxRC,
			EDeviceGrablinkCamConfig_LxxxxRG,
			EDeviceGrablinkCamConfig_LxxxxRG2,
			EDeviceGrablinkCamConfig_LxxxxRP,
			EDeviceGrablinkCamConfig_LxxxxSC,
			EDeviceGrablinkCamConfig_LxxxxSP,
			EDeviceGrablinkCamConfig_P08RG,
			EDeviceGrablinkCamConfig_P08SC,
			EDeviceGrablinkCamConfig_P100RA,
			EDeviceGrablinkCamConfig_P100RA_G,
			EDeviceGrablinkCamConfig_P100RC,
			EDeviceGrablinkCamConfig_P100SA,
			EDeviceGrablinkCamConfig_P100SC,
			EDeviceGrablinkCamConfig_P106RM_D,
			EDeviceGrablinkCamConfig_P106SM,
			EDeviceGrablinkCamConfig_P10RA,
			EDeviceGrablinkCamConfig_P10RC,
			EDeviceGrablinkCamConfig_P10RD,
			EDeviceGrablinkCamConfig_P10RG,
			EDeviceGrablinkCamConfig_P10RM,
			EDeviceGrablinkCamConfig_P10SA,
			EDeviceGrablinkCamConfig_P10SC,
			EDeviceGrablinkCamConfig_P10SD,
			EDeviceGrablinkCamConfig_P10SM,
			EDeviceGrablinkCamConfig_P117RG,
			EDeviceGrablinkCamConfig_P117SC,
			EDeviceGrablinkCamConfig_P120RA,
			EDeviceGrablinkCamConfig_P120RG,
			EDeviceGrablinkCamConfig_P120RM,
			EDeviceGrablinkCamConfig_P120RM_D,
			EDeviceGrablinkCamConfig_P120RM_G,
			EDeviceGrablinkCamConfig_P120SA,
			EDeviceGrablinkCamConfig_P120SC,
			EDeviceGrablinkCamConfig_P120SM,
			EDeviceGrablinkCamConfig_P12RA,
			EDeviceGrablinkCamConfig_P12RC,
			EDeviceGrablinkCamConfig_P12RD,
			EDeviceGrablinkCamConfig_P12RG,
			EDeviceGrablinkCamConfig_P12RM,
			EDeviceGrablinkCamConfig_P12SA,
			EDeviceGrablinkCamConfig_P12SC,
			EDeviceGrablinkCamConfig_P12SD,
			EDeviceGrablinkCamConfig_P12SM,
			EDeviceGrablinkCamConfig_P13SC,
			EDeviceGrablinkCamConfig_P150RG,
			EDeviceGrablinkCamConfig_P150SC,
			EDeviceGrablinkCamConfig_P15RA,
			EDeviceGrablinkCamConfig_P15RC,
			EDeviceGrablinkCamConfig_P15RD,
			EDeviceGrablinkCamConfig_P15RG,
			EDeviceGrablinkCamConfig_P15RM,
			EDeviceGrablinkCamConfig_P15SA,
			EDeviceGrablinkCamConfig_P15SC,
			EDeviceGrablinkCamConfig_P15SD,
			EDeviceGrablinkCamConfig_P15SM,
			EDeviceGrablinkCamConfig_P16RA,
			EDeviceGrablinkCamConfig_P16RA_G,
			EDeviceGrablinkCamConfig_P16RC,
			EDeviceGrablinkCamConfig_P16RD,
			EDeviceGrablinkCamConfig_P16RD_G,
			EDeviceGrablinkCamConfig_P16RG,
			EDeviceGrablinkCamConfig_P16RM,
			EDeviceGrablinkCamConfig_P16RM_G,
			EDeviceGrablinkCamConfig_P16SA,
			EDeviceGrablinkCamConfig_P16SC,
			EDeviceGrablinkCamConfig_P16SD,
			EDeviceGrablinkCamConfig_P16SM,
			EDeviceGrablinkCamConfig_P17RA,
			EDeviceGrablinkCamConfig_P17RA_G,
			EDeviceGrablinkCamConfig_P17RD,
			EDeviceGrablinkCamConfig_P17RD_G,
			EDeviceGrablinkCamConfig_P17RG,
			EDeviceGrablinkCamConfig_P17SA,
			EDeviceGrablinkCamConfig_P17SC,
			EDeviceGrablinkCamConfig_P17SD,
			EDeviceGrablinkCamConfig_P200RC,
			EDeviceGrablinkCamConfig_P200RG,
			EDeviceGrablinkCamConfig_P200SC,
			EDeviceGrablinkCamConfig_P21SC,
			EDeviceGrablinkCamConfig_P22RC,
			EDeviceGrablinkCamConfig_P22RG,
			EDeviceGrablinkCamConfig_P22SC,
			EDeviceGrablinkCamConfig_P240RA,
			EDeviceGrablinkCamConfig_P24RG,
			EDeviceGrablinkCamConfig_P24SC,
			EDeviceGrablinkCamConfig_P25RA,
			EDeviceGrablinkCamConfig_P25RA_G,
			EDeviceGrablinkCamConfig_P25RD,
			EDeviceGrablinkCamConfig_P25RG,
			EDeviceGrablinkCamConfig_P25SA,
			EDeviceGrablinkCamConfig_P25SC,
			EDeviceGrablinkCamConfig_P25SC_S10,
			EDeviceGrablinkCamConfig_P25SC_S8,
			EDeviceGrablinkCamConfig_P25SM_L,
			EDeviceGrablinkCamConfig_P27RC,
			EDeviceGrablinkCamConfig_P27SC,
			EDeviceGrablinkCamConfig_P30RA,
			EDeviceGrablinkCamConfig_P30RA_D,
			EDeviceGrablinkCamConfig_P30RA_G,
			EDeviceGrablinkCamConfig_P30RC,
			EDeviceGrablinkCamConfig_P30RD,
			EDeviceGrablinkCamConfig_P30RG,
			EDeviceGrablinkCamConfig_P30RG_Base,
			EDeviceGrablinkCamConfig_P30RG_Medium,
			EDeviceGrablinkCamConfig_P30RG_O,
			EDeviceGrablinkCamConfig_P30RG_O_Base,
			EDeviceGrablinkCamConfig_P30RG_O_Medium,
			EDeviceGrablinkCamConfig_P30RM,
			EDeviceGrablinkCamConfig_P30RM_2,
			EDeviceGrablinkCamConfig_P30RM_G,
			EDeviceGrablinkCamConfig_P30SA,
			EDeviceGrablinkCamConfig_P30SC,
			EDeviceGrablinkCamConfig_P30SC_Base,
			EDeviceGrablinkCamConfig_P30SC_Medium,
			EDeviceGrablinkCamConfig_P30SD,
			EDeviceGrablinkCamConfig_P30SM,
			EDeviceGrablinkCamConfig_P30SM_L,
			EDeviceGrablinkCamConfig_P32RG,
			EDeviceGrablinkCamConfig_P32SC,
			EDeviceGrablinkCamConfig_P33RG,
			EDeviceGrablinkCamConfig_P33SC,
			EDeviceGrablinkCamConfig_P34RG,
			EDeviceGrablinkCamConfig_P34SC,
			EDeviceGrablinkCamConfig_P400SM,
			EDeviceGrablinkCamConfig_P42RG,
			EDeviceGrablinkCamConfig_P42SC,
			EDeviceGrablinkCamConfig_P48RG,
			EDeviceGrablinkCamConfig_P48SC,
			EDeviceGrablinkCamConfig_P50,
			EDeviceGrablinkCamConfig_P50RA,
			EDeviceGrablinkCamConfig_P50RA_G,
			EDeviceGrablinkCamConfig_P50RD,
			EDeviceGrablinkCamConfig_P50RG,
			EDeviceGrablinkCamConfig_P50RG_DX,
			EDeviceGrablinkCamConfig_P50RM,
			EDeviceGrablinkCamConfig_P50RM_G,
			EDeviceGrablinkCamConfig_P50SA,
			EDeviceGrablinkCamConfig_P50SC,
			EDeviceGrablinkCamConfig_P50SC_D10,
			EDeviceGrablinkCamConfig_P50SC_D8,
			EDeviceGrablinkCamConfig_P50SD,
			EDeviceGrablinkCamConfig_P50SM,
			EDeviceGrablinkCamConfig_P50SM_L,
			EDeviceGrablinkCamConfig_P60RA,
			EDeviceGrablinkCamConfig_P60RA_D,
			EDeviceGrablinkCamConfig_P60RA_G,
			EDeviceGrablinkCamConfig_P60RC,
			EDeviceGrablinkCamConfig_P60RD,
			EDeviceGrablinkCamConfig_P60RD_G,
			EDeviceGrablinkCamConfig_P60RG,
			EDeviceGrablinkCamConfig_P60RM,
			EDeviceGrablinkCamConfig_P60RM_A,
			EDeviceGrablinkCamConfig_P60RM_D,
			EDeviceGrablinkCamConfig_P60RM_G,
			EDeviceGrablinkCamConfig_P60RM_O,
			EDeviceGrablinkCamConfig_P60RM_R,
			EDeviceGrablinkCamConfig_P60SA,
			EDeviceGrablinkCamConfig_P60SC,
			EDeviceGrablinkCamConfig_P60SD,
			EDeviceGrablinkCamConfig_P60SM,
			EDeviceGrablinkCamConfig_P60SM_L,
			EDeviceGrablinkCamConfig_P64RG,
			EDeviceGrablinkCamConfig_P64SC,
			EDeviceGrablinkCamConfig_P70RA,
			EDeviceGrablinkCamConfig_P70RM,
			EDeviceGrablinkCamConfig_P70SA,
			EDeviceGrablinkCamConfig_P70SM,
			EDeviceGrablinkCamConfig_P74RG,
			EDeviceGrablinkCamConfig_P74SC,
			EDeviceGrablinkCamConfig_P75RG,
			EDeviceGrablinkCamConfig_P75SC,
			EDeviceGrablinkCamConfig_P80RG,
			EDeviceGrablinkCamConfig_P80SC,
			EDeviceGrablinkCamConfig_P8RC,
			EDeviceGrablinkCamConfig_P8RG,
			EDeviceGrablinkCamConfig_P8SC,
			EDeviceGrablinkCamConfig_P94RG,
			EDeviceGrablinkCamConfig_P94SC,
			EDeviceGrablinkCamConfig_PxxRA,
			EDeviceGrablinkCamConfig_PxxRA_D,
			EDeviceGrablinkCamConfig_PxxRA_M,
			EDeviceGrablinkCamConfig_PxxRC,
			EDeviceGrablinkCamConfig_PxxRD,
			EDeviceGrablinkCamConfig_PxxRG,
			EDeviceGrablinkCamConfig_PxxRG_PartialScan,
			EDeviceGrablinkCamConfig_PxxRM_1,
			EDeviceGrablinkCamConfig_PxxRM_2,
			EDeviceGrablinkCamConfig_PxxRM_A,
			EDeviceGrablinkCamConfig_PxxRM_D,
			EDeviceGrablinkCamConfig_PxxSA,
			EDeviceGrablinkCamConfig_PxxSA_D,
			EDeviceGrablinkCamConfig_PxxSC,
			EDeviceGrablinkCamConfig_PxxSD,
			EDeviceGrablinkCamConfig_PxxSM,
			EDeviceGrablinkCamConfig_PxxSM_L,
			EDeviceGrablinkCamConfig_STD,
			EDeviceGrablinkCamConfig_TEST,
			EDeviceGrablinkCamConfig_Count,
		};

		enum EDeviceGrablinkCamera
		{
			EDeviceGrablinkCamera_1000m = 0,
			EDeviceGrablinkCamera_1000m_D,
			EDeviceGrablinkCamera_1000m_S,
			EDeviceGrablinkCamera_1600c,
			EDeviceGrablinkCamera_1600c_D,
			EDeviceGrablinkCamera_1600c_D_RGB,
			EDeviceGrablinkCamera_1600c_S,
			EDeviceGrablinkCamera_1600c_S_RGB,
			EDeviceGrablinkCamera_1600m,
			EDeviceGrablinkCamera_1600m_D,
			EDeviceGrablinkCamera_1600m_S,
			EDeviceGrablinkCamera_1M28_SA,
			EDeviceGrablinkCamera_2000c_D,
			EDeviceGrablinkCamera_2000m,
			EDeviceGrablinkCamera_2000m_D,
			EDeviceGrablinkCamera_2000m_S,
			EDeviceGrablinkCamera_2M30_SA,
			EDeviceGrablinkCamera_4000m,
			EDeviceGrablinkCamera_4000m_D,
			EDeviceGrablinkCamera_4000m_S,
			EDeviceGrablinkCamera_A102k,
			EDeviceGrablinkCamera_A201b,
			EDeviceGrablinkCamera_A202k,
			EDeviceGrablinkCamera_A301bc,
			EDeviceGrablinkCamera_A302b,
			EDeviceGrablinkCamera_A402k,
			EDeviceGrablinkCamera_A403k,
			EDeviceGrablinkCamera_A501k,
			EDeviceGrablinkCamera_AVIIVA,
			EDeviceGrablinkCamera_AViiVA_C2CL4010,
			EDeviceGrablinkCamera_AViiVA_M4CL6007,
			EDeviceGrablinkCamera_AViiVA_M4CL8007,
			EDeviceGrablinkCamera_AVIIVA0514,
			EDeviceGrablinkCamera_AVIIVA1014,
			EDeviceGrablinkCamera_AVIIVA2014,
			EDeviceGrablinkCamera_AVIIVA4010,
			EDeviceGrablinkCamera_CAMERA_CCIR,
			EDeviceGrablinkCamera_CAMERA_EIA,
			EDeviceGrablinkCamera_CAMERA_NTSC,
			EDeviceGrablinkCamera_CAMERA_PAL,
			EDeviceGrablinkCamera_CAMERA_sCCIR,
			EDeviceGrablinkCamera_CAMERA_sEIA,
			EDeviceGrablinkCamera_CAMERA_sNTSC,
			EDeviceGrablinkCamera_CAMERA_sPAL,
			EDeviceGrablinkCamera_CameraExposeAR,
			EDeviceGrablinkCamera_CCIR,
			EDeviceGrablinkCamera_COLIBRI1024CL,
			EDeviceGrablinkCamera_COLIBRI2048CL,
			EDeviceGrablinkCamera_CS3920,
			EDeviceGrablinkCamera_CS6910CL,
			EDeviceGrablinkCamera_CS8530,
			EDeviceGrablinkCamera_CS8531,
			EDeviceGrablinkCamera_CS8531D,
			EDeviceGrablinkCamera_CS8541D,
			EDeviceGrablinkCamera_CS8550D,
			EDeviceGrablinkCamera_CS8550i,
			EDeviceGrablinkCamera_CS8550i_01,
			EDeviceGrablinkCamera_CSB1100CL,
			EDeviceGrablinkCamera_CSB4000CL,
			EDeviceGrablinkCamera_CUSTOM,
			EDeviceGrablinkCamera_CV_A1,
			EDeviceGrablinkCamera_CV_A11,
			EDeviceGrablinkCamera_CV_A33,
			EDeviceGrablinkCamera_CV_A50C,
			EDeviceGrablinkCamera_CV_A50E,
			EDeviceGrablinkCamera_CV_M1,
			EDeviceGrablinkCamera_CV_M10C,
			EDeviceGrablinkCamera_CV_M10E,
			EDeviceGrablinkCamera_CV_M30,
			EDeviceGrablinkCamera_CV_M40,
			EDeviceGrablinkCamera_CV_M4CL,
			EDeviceGrablinkCamera_CV_M4CLp,
			EDeviceGrablinkCamera_CV_M50,
			EDeviceGrablinkCamera_CV_M50C,
			EDeviceGrablinkCamera_CV_M50E,
			EDeviceGrablinkCamera_CV_M77,
			EDeviceGrablinkCamera_CV_M7CL,
			EDeviceGrablinkCamera_CV_M7CLp,
			EDeviceGrablinkCamera_CV_M8CL,
			EDeviceGrablinkCamera_CV_M9CL,
			EDeviceGrablinkCamera_DT4100,
			EDeviceGrablinkCamera_EIA,
			EDeviceGrablinkCamera_Generic,
			EDeviceGrablinkCamera_Generic_Dual,
			EDeviceGrablinkCamera_Generic_RGB,
			EDeviceGrablinkCamera_GrabberExposeAR,
			EDeviceGrablinkCamera_HV_C20AEK,
			EDeviceGrablinkCamera_HV_C20AP,
			EDeviceGrablinkCamera_ICD_FA41,
			EDeviceGrablinkCamera_ICD_FA41E,
			EDeviceGrablinkCamera_IK_TF1,
			EDeviceGrablinkCamera_IK_TF5,
			EDeviceGrablinkCamera_IK_TF7,
			EDeviceGrablinkCamera_iMVS157,
			EDeviceGrablinkCamera_InterlaceFR,
			EDeviceGrablinkCamera_InterlaceRR,
			EDeviceGrablinkCamera_KP_F100BCL,
			EDeviceGrablinkCamera_KP_F120CL,
			EDeviceGrablinkCamera_KP_F200CL,
			EDeviceGrablinkCamera_KP_F2B,
			EDeviceGrablinkCamera_KP_F3,
			EDeviceGrablinkCamera_KP_F30,
			EDeviceGrablinkCamera_KP_F30x,
			EDeviceGrablinkCamera_KP_F3W,
			EDeviceGrablinkCamera_KP_M32P,
			EDeviceGrablinkCamera_KP120CL,
			EDeviceGrablinkCamera_L101k,
			EDeviceGrablinkCamera_L103k,
			EDeviceGrablinkCamera_L104k,
			EDeviceGrablinkCamera_L301bc,
			EDeviceGrablinkCamera_L301kc,
			EDeviceGrablinkCamera_L401k,
			EDeviceGrablinkCamera_L402k,
			EDeviceGrablinkCamera_L403k,
			EDeviceGrablinkCamera_LD3521PGx,
			EDeviceGrablinkCamera_LD3522PGx,
			EDeviceGrablinkCamera_LD3523PGx,
			EDeviceGrablinkCamera_LD3541PGx,
			EDeviceGrablinkCamera_LD3542PGx,
			EDeviceGrablinkCamera_LD3543PGx,
			EDeviceGrablinkCamera_LongExposeFR,
			EDeviceGrablinkCamera_MV_D1024,
			EDeviceGrablinkCamera_MV_D1024_160_CL_8,
			EDeviceGrablinkCamera_MV_D1024_80_CL_8,
			EDeviceGrablinkCamera_MV_D640,
			EDeviceGrablinkCamera_MV_D640_33_CL_10,
			EDeviceGrablinkCamera_MV_D640_66_CL_10,
			EDeviceGrablinkCamera_MV_D640C_33_CL_10,
			EDeviceGrablinkCamera_MV_D640C_66_CL_10,
			EDeviceGrablinkCamera_MyCameraLink,
			EDeviceGrablinkCamera_MyColorScanCameraLink,
			EDeviceGrablinkCamera_NoCamera,
			EDeviceGrablinkCamera_NSUF7400CL,
			EDeviceGrablinkCamera_NTSC,
			EDeviceGrablinkCamera_NUCL4096D,
			EDeviceGrablinkCamera_NUCL7500D,
			EDeviceGrablinkCamera_P2_2x_01k40,
			EDeviceGrablinkCamera_P2_2x_02k40,
			EDeviceGrablinkCamera_P2_2x_04k40,
			EDeviceGrablinkCamera_P2_2x_06k40,
			EDeviceGrablinkCamera_P2_2x_08k40,
			EDeviceGrablinkCamera_P2_4x_04k40,
			EDeviceGrablinkCamera_P2_4x_06k40,
			EDeviceGrablinkCamera_P2_4x_08k40,
			EDeviceGrablinkCamera_PAL,
			EDeviceGrablinkCamera_PC_640CL,
			EDeviceGrablinkCamera_PRICOLOR1024C,
			EDeviceGrablinkCamera_PRICOLOR2048C,
			EDeviceGrablinkCamera_ProgressiveFR,
			EDeviceGrablinkCamera_SI_M100,
			EDeviceGrablinkCamera_SIMA,
			EDeviceGrablinkCamera_SIMUL,
			EDeviceGrablinkCamera_SKC_131,
			EDeviceGrablinkCamera_SKC_141,
			EDeviceGrablinkCamera_SKC_141TC,
			EDeviceGrablinkCamera_SKCm141TC,
			EDeviceGrablinkCamera_STC_1000,
			EDeviceGrablinkCamera_STC_1100b,
			EDeviceGrablinkCamera_Std_BW_CCIR,
			EDeviceGrablinkCamera_Std_BW_EIA,
			EDeviceGrablinkCamera_Std_RGB_CCIR,
			EDeviceGrablinkCamera_Std_RGB_EIA,
			EDeviceGrablinkCamera_SU51,
			EDeviceGrablinkCamera_SU74,
			EDeviceGrablinkCamera_SVS084MFCL,
			EDeviceGrablinkCamera_SVS084MSCL,
			EDeviceGrablinkCamera_SVS085MFCL,
			EDeviceGrablinkCamera_SVS085MSCL,
			EDeviceGrablinkCamera_SVS204MFCL,
			EDeviceGrablinkCamera_SVS204MSCL,
			EDeviceGrablinkCamera_SVS282CSCL,
			EDeviceGrablinkCamera_SVS285MFCL,
			EDeviceGrablinkCamera_SVS285MSCL,
			EDeviceGrablinkCamera_TI_324A,
			EDeviceGrablinkCamera_TM_1020_15CL,
			EDeviceGrablinkCamera_TM_1320_15CL,
			EDeviceGrablinkCamera_TM_2016_8CL,
			EDeviceGrablinkCamera_TM_4000CL,
			EDeviceGrablinkCamera_TM_6702,
			EDeviceGrablinkCamera_TM_6710CL,
			EDeviceGrablinkCamera_TM_9701,
			EDeviceGrablinkCamera_TMC_1000,
			EDeviceGrablinkCamera_TMC_4000CL,
			EDeviceGrablinkCamera_TMC_6700,
			EDeviceGrablinkCamera_VCC_810,
			EDeviceGrablinkCamera_VCC_8350,
			EDeviceGrablinkCamera_VCC_8350CL,
			EDeviceGrablinkCamera_VCC_850A,
			EDeviceGrablinkCamera_VCC_8550,
			EDeviceGrablinkCamera_VCC_8550CL,
			EDeviceGrablinkCamera_VCC_870,
			EDeviceGrablinkCamera_VCC_870A,
			EDeviceGrablinkCamera_VCC_8750CL,
			EDeviceGrablinkCamera_VCC_8850CL,
			EDeviceGrablinkCamera_VCC_G22V31CL_PoCL,
			EDeviceGrablinkCamera_XC_003,
			EDeviceGrablinkCamera_XC_55,
			EDeviceGrablinkCamera_XC_56,
			EDeviceGrablinkCamera_XC_7500,
			EDeviceGrablinkCamera_XC_8500CE,
			EDeviceGrablinkCamera_XC_ES30,
			EDeviceGrablinkCamera_XC_ES30CE,
			EDeviceGrablinkCamera_XC_HR300,
			EDeviceGrablinkCamera_XC_HR50,
			EDeviceGrablinkCamera_XC_HR57,
			EDeviceGrablinkCamera_XC_HR58,
			EDeviceGrablinkCamera_XC_HR70,
			EDeviceGrablinkCamera_XC_ST50,
			EDeviceGrablinkCamera_XC_ST50CE,
			EDeviceGrablinkCamera_XC_ST70CE,
			EDeviceGrablinkCamera_XCL_U1000,
			EDeviceGrablinkCamera_XCL_U1000C,
			EDeviceGrablinkCamera_XCL_V500,
			EDeviceGrablinkCamera_XCL_X700,
			EDeviceGrablinkCamera_XCLmU1000,
			EDeviceGrablinkCamera_XCLmU1000C,
			EDeviceGrablinkCamera_XCLmV500,
			EDeviceGrablinkCamera_XCLmX700,
			EDeviceGrablinkCamera_YD5040,
			EDeviceGrablinkCamera_YD5060,
			EDeviceGrablinkCamera_Count,
		};

		enum EDeviceGrablinkDataLink
		{
			EDeviceGrablinkDataLink_ANALOG = 0,
			EDeviceGrablinkDataLink_CAMERALINK,
			EDeviceGrablinkDataLink_COMPOSITE,
			EDeviceGrablinkDataLink_Count,
		};

		enum EDeviceGrablinkImaging
		{
			EDeviceGrablinkImaging_AREA = 0,
			EDeviceGrablinkImaging_LINE,
			EDeviceGrablinkImaging_TDI,
			EDeviceGrablinkImaging_Count,
		};

		enum EDeviceGrablinkSpectrum
		{
			EDeviceGrablinkSpectrum_BW = 0,
			EDeviceGrablinkSpectrum_COLOR,
			EDeviceGrablinkSpectrum_IR,
			EDeviceGrablinkSpectrum_Count,
		};

		enum EDeviceGrablinkPixelClkMode
		{
			EDeviceGrablinkPixelClkMode_INTERMITTENT = 0,
			EDeviceGrablinkPixelClkMode_PERMANENT,
			EDeviceGrablinkPixelClkMode_Count,
		};

		enum EDeviceGrablinkTapConfiguration
		{
			EDeviceGrablinkTapConfiguration_BASE_1T10 = 0,
			EDeviceGrablinkTapConfiguration_BASE_1T12,
			EDeviceGrablinkTapConfiguration_BASE_1T14,
			EDeviceGrablinkTapConfiguration_BASE_1T16,
			EDeviceGrablinkTapConfiguration_BASE_1T24,
			EDeviceGrablinkTapConfiguration_BASE_1T24B3,
			EDeviceGrablinkTapConfiguration_BASE_1T30B2,
			EDeviceGrablinkTapConfiguration_BASE_1T30B3,
			EDeviceGrablinkTapConfiguration_BASE_1T36B2,
			EDeviceGrablinkTapConfiguration_BASE_1T36B3,
			EDeviceGrablinkTapConfiguration_BASE_1T42B2,
			EDeviceGrablinkTapConfiguration_BASE_1T42B3,
			EDeviceGrablinkTapConfiguration_BASE_1T48B2,
			EDeviceGrablinkTapConfiguration_BASE_1T48B3,
			EDeviceGrablinkTapConfiguration_BASE_1T8,
			EDeviceGrablinkTapConfiguration_BASE_2T10,
			EDeviceGrablinkTapConfiguration_BASE_2T12,
			EDeviceGrablinkTapConfiguration_BASE_2T14B2,
			EDeviceGrablinkTapConfiguration_BASE_2T16B2,
			EDeviceGrablinkTapConfiguration_BASE_2T24B2,
			EDeviceGrablinkTapConfiguration_BASE_2T8,
			EDeviceGrablinkTapConfiguration_BASE_3T10B2,
			EDeviceGrablinkTapConfiguration_BASE_3T12B2,
			EDeviceGrablinkTapConfiguration_BASE_3T14B2,
			EDeviceGrablinkTapConfiguration_BASE_3T16B2,
			EDeviceGrablinkTapConfiguration_BASE_3T8,
			EDeviceGrablinkTapConfiguration_BASE_4T10B2,
			EDeviceGrablinkTapConfiguration_BASE_4T12B2,
			EDeviceGrablinkTapConfiguration_BASE_4T8B2,
			EDeviceGrablinkTapConfiguration_DECA_10T8,
			EDeviceGrablinkTapConfiguration_DECA_10T8C,
			EDeviceGrablinkTapConfiguration_DECA_3T24,
			EDeviceGrablinkTapConfiguration_DECA_9T8,
			EDeviceGrablinkTapConfiguration_FULL_8T8,
			EDeviceGrablinkTapConfiguration_FULL_8T8C,
			EDeviceGrablinkTapConfiguration_INVALID,
			EDeviceGrablinkTapConfiguration_LITE_1T10,
			EDeviceGrablinkTapConfiguration_LITE_1T8,
			EDeviceGrablinkTapConfiguration_MEDIUM_1T30,
			EDeviceGrablinkTapConfiguration_MEDIUM_1T36,
			EDeviceGrablinkTapConfiguration_MEDIUM_1T42,
			EDeviceGrablinkTapConfiguration_MEDIUM_1T48,
			EDeviceGrablinkTapConfiguration_MEDIUM_2T14,
			EDeviceGrablinkTapConfiguration_MEDIUM_2T16,
			EDeviceGrablinkTapConfiguration_MEDIUM_2T24,
			EDeviceGrablinkTapConfiguration_MEDIUM_3T10,
			EDeviceGrablinkTapConfiguration_MEDIUM_3T12,
			EDeviceGrablinkTapConfiguration_MEDIUM_3T14,
			EDeviceGrablinkTapConfiguration_MEDIUM_3T16,
			EDeviceGrablinkTapConfiguration_MEDIUM_4T10,
			EDeviceGrablinkTapConfiguration_MEDIUM_4T12,
			EDeviceGrablinkTapConfiguration_MEDIUM_4T8,
			EDeviceGrablinkTapConfiguration_MEDIUM_6T8,
			EDeviceGrablinkTapConfiguration_MEDIUM_8T8B2,
			EDeviceGrablinkTapConfiguration_Count,
		};

		enum EDeviceGrablinkAuxResetCtl
		{
			EDeviceGrablinkAuxResetCtl_DIFF = 0,
			EDeviceGrablinkAuxResetCtl_NONE,
			EDeviceGrablinkAuxResetCtl_TTL,
			EDeviceGrablinkAuxResetCtl_Count,
		};
		enum EDeviceGrablinkCC1Usage
		{
			EDeviceGrablinkCC1Usage_ACQPHASE = 0,
			EDeviceGrablinkCC1Usage_ACQSEQUENCE,
			EDeviceGrablinkCC1Usage_AUXRESET,
			EDeviceGrablinkCC1Usage_DIN1,
			EDeviceGrablinkCC1Usage_HIGH,
			EDeviceGrablinkCC1Usage_IIN1,
			EDeviceGrablinkCC1Usage_LOW,
			EDeviceGrablinkCC1Usage_RESET,
			EDeviceGrablinkCC1Usage_SOFT,
			EDeviceGrablinkCC1Usage_Count,
		};

		enum EDeviceGrablinkCC2Usage
		{
			EDeviceGrablinkCC2Usage_ACQPHASE = 0,
			EDeviceGrablinkCC2Usage_ACQSEQUENCE,
			EDeviceGrablinkCC2Usage_AUXRESET,
			EDeviceGrablinkCC2Usage_DIN2,
			EDeviceGrablinkCC2Usage_HIGH,
			EDeviceGrablinkCC2Usage_LOW,
			EDeviceGrablinkCC2Usage_RESET,
			EDeviceGrablinkCC2Usage_SOFT,
			EDeviceGrablinkCC2Usage_Count,
		};

		enum EDeviceGrablinkCC3Usage
		{
			EDeviceGrablinkCC3Usage_ACQPHASE = 0,
			EDeviceGrablinkCC3Usage_ACQSEQUENCE,
			EDeviceGrablinkCC3Usage_AUXRESET,
			EDeviceGrablinkCC3Usage_HIGH,
			EDeviceGrablinkCC3Usage_IIN1,
			EDeviceGrablinkCC3Usage_LOW,
			EDeviceGrablinkCC3Usage_READOUTALLOWED,
			EDeviceGrablinkCC3Usage_RESET,
			EDeviceGrablinkCC3Usage_SOFT,
			EDeviceGrablinkCC3Usage_Count,
		};

		enum EDeviceGrablinkCC4Usage
		{
			EDeviceGrablinkCC4Usage_ACQPHASE = 0,
			EDeviceGrablinkCC4Usage_ACQSEQUENCE,
			EDeviceGrablinkCC4Usage_AUXRESET,
			EDeviceGrablinkCC4Usage_HIGH,
			EDeviceGrablinkCC4Usage_LOW,
			EDeviceGrablinkCC4Usage_RESET,
			EDeviceGrablinkCC4Usage_SOFT,
			EDeviceGrablinkCC4Usage_TRIGGERALLOWED,
			EDeviceGrablinkCC4Usage_Count,
		};

		enum EDeviceGrablinkColorMethod
		{
			EDeviceGrablinkColorMethod_BAYER = 0,
			EDeviceGrablinkColorMethod_NONE,
			EDeviceGrablinkColorMethod_PRISM,
			EDeviceGrablinkColorMethod_RGB,
			EDeviceGrablinkColorMethod_TRILINEAR,
			EDeviceGrablinkColorMethod_Count,
		};

		enum EDeviceGrablinkColorRegistrationControl
		{
			EDeviceGrablinkColorRegistrationControl_FVAL = 0,
			EDeviceGrablinkColorRegistrationControl_NONE,
			EDeviceGrablinkColorRegistrationControl_Count,
		};

		enum EDeviceGrablinkDvalMode
		{
			EDeviceGrablinkDvalMode_DC = 0,
			EDeviceGrablinkDvalMode_DG,
			EDeviceGrablinkDvalMode_DID2P0,
			EDeviceGrablinkDvalMode_DN,
			EDeviceGrablinkDvalMode_Count,
		};

		enum EDeviceGrablinkExpose
		{
			EDeviceGrablinkExpose_EXTPRM = 0,
			EDeviceGrablinkExpose_Fld,
			EDeviceGrablinkExpose_INTCTL,
			EDeviceGrablinkExpose_INTPRM,
			EDeviceGrablinkExpose_P1_Effective_us,
			EDeviceGrablinkExpose_P2_Effective_us,
			EDeviceGrablinkExpose_PLSCTL,
			EDeviceGrablinkExpose_PLSTRG,
			EDeviceGrablinkExpose_Tk,
			EDeviceGrablinkExpose_us,
			EDeviceGrablinkExpose_WIDTH,
			EDeviceGrablinkExpose_WIDTH_DX,
			EDeviceGrablinkExpose_Count,
		};

		enum EDeviceGrablinkExposeOverlap
		{
			EDeviceGrablinkExposeOverlap_ALLOW = 0,
			EDeviceGrablinkExposeOverlap_FORBID,
			EDeviceGrablinkExposeOverlap_Count,
		};

		enum EDeviceGrablinkFvalMode
		{
			EDeviceGrablinkFvalMode_FA = 0,
			EDeviceGrablinkFvalMode_FC,
			EDeviceGrablinkFvalMode_FN,
			EDeviceGrablinkFvalMode_LA,
			EDeviceGrablinkFvalMode_PC,
			EDeviceGrablinkFvalMode_Count,
		};

		enum EDeviceGrablinkLvalMode
		{
			EDeviceGrablinkLvalMode_DG = 0,
			EDeviceGrablinkLvalMode_LA,
			EDeviceGrablinkLvalMode_LB,
			EDeviceGrablinkLvalMode_LC,
			EDeviceGrablinkLvalMode_LN,
			EDeviceGrablinkLvalMode_LP,
			EDeviceGrablinkLvalMode_Count,
		};

		enum EDeviceGrablinkReadout
		{
			EDeviceGrablinkReadout_100ns = 0,
			EDeviceGrablinkReadout_INTCTL,
			EDeviceGrablinkReadout_PLSTRG,
			EDeviceGrablinkReadout_us,
			EDeviceGrablinkReadout_WIDTH,
			EDeviceGrablinkReadout_Count,
		};

		enum EDeviceGrablinkResetCtl
		{
			EDeviceGrablinkResetCtl_DIFF = 0,
			EDeviceGrablinkResetCtl_NONE,
			EDeviceGrablinkResetCtl_TTL,
			EDeviceGrablinkResetCtl_Count,
		};

		enum EDeviceGrablinkResetEdge
		{
			EDeviceGrablinkResetEdge_GOHIGH = 0,
			EDeviceGrablinkResetEdge_GOLOW,
			EDeviceGrablinkResetEdge_Count,
		};

		enum EDeviceGrablinkTwoLineSynchronization
		{
			EDeviceGrablinkTwoLineSynchronization_DISABLE = 0,
			EDeviceGrablinkTwoLineSynchronization_ENABLE,
			EDeviceGrablinkTwoLineSynchronization_Count,
		};

		enum EDeviceGrablinkTapGeometry
		{
			EDeviceGrablinkTapGeometry_10X = 0,
			EDeviceGrablinkTapGeometry_10X_1Y,
			EDeviceGrablinkTapGeometry_1X,
			EDeviceGrablinkTapGeometry_1X_1Y,
			EDeviceGrablinkTapGeometry_1X_1Y2,
			EDeviceGrablinkTapGeometry_1X_2Y,
			EDeviceGrablinkTapGeometry_1X_2YE,
			EDeviceGrablinkTapGeometry_1X10,
			EDeviceGrablinkTapGeometry_1X10_1Y,
			EDeviceGrablinkTapGeometry_1X2,
			EDeviceGrablinkTapGeometry_1X2_1Y,
			EDeviceGrablinkTapGeometry_1X2_1Y2,
			EDeviceGrablinkTapGeometry_1X2_2YE,
			EDeviceGrablinkTapGeometry_1X3,
			EDeviceGrablinkTapGeometry_1X3_1Y,
			EDeviceGrablinkTapGeometry_1X3_1Y2,
			EDeviceGrablinkTapGeometry_1X3_2YE,
			EDeviceGrablinkTapGeometry_1X4,
			EDeviceGrablinkTapGeometry_1X4_1Y,
			EDeviceGrablinkTapGeometry_1X4_1Y2,
			EDeviceGrablinkTapGeometry_1X4_2YE,
			EDeviceGrablinkTapGeometry_1X8,
			EDeviceGrablinkTapGeometry_1X8_1Y,
			EDeviceGrablinkTapGeometry_2X,
			EDeviceGrablinkTapGeometry_2X_1Y,
			EDeviceGrablinkTapGeometry_2X_1Y2,
			EDeviceGrablinkTapGeometry_2X_2YE,
			EDeviceGrablinkTapGeometry_2X2,
			EDeviceGrablinkTapGeometry_2X2_1Y,
			EDeviceGrablinkTapGeometry_2X2_1Y2,
			EDeviceGrablinkTapGeometry_2X2_2YE,
			EDeviceGrablinkTapGeometry_2X2E,
			EDeviceGrablinkTapGeometry_2X2E_1Y,
			EDeviceGrablinkTapGeometry_2X2E_1Y2,
			EDeviceGrablinkTapGeometry_2X2E_2YE,
			EDeviceGrablinkTapGeometry_2X2M,
			EDeviceGrablinkTapGeometry_2X2M_1Y,
			EDeviceGrablinkTapGeometry_2X2M_1Y2,
			EDeviceGrablinkTapGeometry_2X2M_2YE,
			EDeviceGrablinkTapGeometry_2X4,
			EDeviceGrablinkTapGeometry_2X4_1Y,
			EDeviceGrablinkTapGeometry_2XE,
			EDeviceGrablinkTapGeometry_2XE_1Y,
			EDeviceGrablinkTapGeometry_2XE_1Y2,
			EDeviceGrablinkTapGeometry_2XE_2YE,
			EDeviceGrablinkTapGeometry_2XM,
			EDeviceGrablinkTapGeometry_2XM_1Y,
			EDeviceGrablinkTapGeometry_2XM_1Y2,
			EDeviceGrablinkTapGeometry_2XM_2YE,
			EDeviceGrablinkTapGeometry_2XR,
			EDeviceGrablinkTapGeometry_2XR_1Y,
			EDeviceGrablinkTapGeometry_2XR_1Y2,
			EDeviceGrablinkTapGeometry_2XR_2YE,
			EDeviceGrablinkTapGeometry_3X,
			EDeviceGrablinkTapGeometry_3X_1Y,
			EDeviceGrablinkTapGeometry_3X_1Y2,
			EDeviceGrablinkTapGeometry_3X_1Y3,
			EDeviceGrablinkTapGeometry_3X_2YE,
			EDeviceGrablinkTapGeometry_4X,
			EDeviceGrablinkTapGeometry_4X_1Y,
			EDeviceGrablinkTapGeometry_4X_1Y2,
			EDeviceGrablinkTapGeometry_4X_2YE,
			EDeviceGrablinkTapGeometry_4X2,
			EDeviceGrablinkTapGeometry_4X2_1Y,
			EDeviceGrablinkTapGeometry_4X2E,
			EDeviceGrablinkTapGeometry_4X2E_1Y,
			EDeviceGrablinkTapGeometry_4XE,
			EDeviceGrablinkTapGeometry_4XE_1Y,
			EDeviceGrablinkTapGeometry_4XE_1Y2,
			EDeviceGrablinkTapGeometry_4XE_2YE,
			EDeviceGrablinkTapGeometry_4XR,
			EDeviceGrablinkTapGeometry_4XR_1Y,
			EDeviceGrablinkTapGeometry_4XR_1Y2,
			EDeviceGrablinkTapGeometry_4XR_2YE,
			EDeviceGrablinkTapGeometry_8X,
			EDeviceGrablinkTapGeometry_8X_1Y,
			EDeviceGrablinkTapGeometry_8XR,
			EDeviceGrablinkTapGeometry_8XR_1Y,
			EDeviceGrablinkTapGeometry_Y,
			EDeviceGrablinkTapGeometry_Y_1Y,
			EDeviceGrablinkTapGeometry_Y_1Y2,
			EDeviceGrablinkTapGeometry_Y_1Y3,
			EDeviceGrablinkTapGeometry_Y_2YE,
			EDeviceGrablinkTapGeometry_Count,
		};

		enum EDeviceGrablinkAuxresetLine
		{
			EDeviceGrablinkAuxresetLine_CC1 = 0,
			EDeviceGrablinkAuxresetLine_CC2,
			EDeviceGrablinkAuxresetLine_CC3,
			EDeviceGrablinkAuxresetLine_CC4,
			EDeviceGrablinkAuxresetLine_CC5,
			EDeviceGrablinkAuxresetLine_DUAL,
			EDeviceGrablinkAuxresetLine_EXP,
			EDeviceGrablinkAuxresetLine_Feature,
			EDeviceGrablinkAuxresetLine_Feature_Available,
			EDeviceGrablinkAuxresetLine_Feature_NotAvailable,
			EDeviceGrablinkAuxresetLine_GATE,
			EDeviceGrablinkAuxresetLine_HIO,
			EDeviceGrablinkAuxresetLine_NC,
			EDeviceGrablinkAuxresetLine_RST,
			EDeviceGrablinkAuxresetLine_VIO,
			EDeviceGrablinkAuxresetLine_Count,
		};

		enum EDeviceGrablinkResetLine
		{
			EDeviceGrablinkResetLine_CC1 = 0,
			EDeviceGrablinkResetLine_CC2,
			EDeviceGrablinkResetLine_CC3,
			EDeviceGrablinkResetLine_CC4,
			EDeviceGrablinkResetLine_CC5,
			EDeviceGrablinkResetLine_DUAL,
			EDeviceGrablinkResetLine_EXP,
			EDeviceGrablinkResetLine_GATE,
			EDeviceGrablinkResetLine_HIO,
			EDeviceGrablinkResetLine_NC,
			EDeviceGrablinkResetLine_RST,
			EDeviceGrablinkResetLine_VIO,
			EDeviceGrablinkResetLine_Count,
		};

		enum EDeviceGrablinkBreakEffect
		{
			EDeviceGrablinkBreakEffect_ABORT = 0,
			EDeviceGrablinkBreakEffect_FINISH,
			EDeviceGrablinkBreakEffect_Count,
		};

		enum EDeviceGrablinkEndTrigMode
		{
			EDeviceGrablinkEndTrigMode_AUTO = 0,
			EDeviceGrablinkEndTrigMode_HARD,
			EDeviceGrablinkEndTrigMode_MASTER_CHANNEL,
			EDeviceGrablinkEndTrigMode_SLAVE,
			EDeviceGrablinkEndTrigMode_Count,
		};

		enum EDeviceGrablinkNextTrigMode
		{
			EDeviceGrablinkNextTrigMode_COMBINED = 0,
			EDeviceGrablinkNextTrigMode_HARD,
			EDeviceGrablinkNextTrigMode_IMMEDIATE,
			EDeviceGrablinkNextTrigMode_NextTrigMode,
			EDeviceGrablinkNextTrigMode_PAUSE,
			EDeviceGrablinkNextTrigMode_PERIODIC,
			EDeviceGrablinkNextTrigMode_REPEAT,
			EDeviceGrablinkNextTrigMode_SAME,
			EDeviceGrablinkNextTrigMode_SLAVE,
			EDeviceGrablinkNextTrigMode_SOFT,
			EDeviceGrablinkNextTrigMode_TIMER,
			EDeviceGrablinkNextTrigMode_Count,
		};

		enum EDeviceGrablinkSynchronizedAcquisition
		{
			EDeviceGrablinkSynchronizedAcquisition_INTERNAL_MAST = 0,
			EDeviceGrablinkSynchronizedAcquisition_INTERNAL_SLAV,
			EDeviceGrablinkSynchronizedAcquisition_LOCAL_MASTER,
			EDeviceGrablinkSynchronizedAcquisition_LOCAL_SLAVE,
			EDeviceGrablinkSynchronizedAcquisition_MASTER,
			EDeviceGrablinkSynchronizedAcquisition_OFF,
			EDeviceGrablinkSynchronizedAcquisition_SLAVE,
			EDeviceGrablinkSynchronizedAcquisition_Count,
		};

		enum EDeviceGrablinkForceTrig
		{
			EDeviceGrablinkForceTrig_ENDTRIG = 0,
			EDeviceGrablinkForceTrig_TRIG,
			EDeviceGrablinkForceTrig_Count,
		};

		enum EDeviceGrablinkTrigCtl
		{
			EDeviceGrablinkTrigCtl_12V = 0,
			EDeviceGrablinkTrigCtl_5V,
			EDeviceGrablinkTrigCtl_AUTO,
			EDeviceGrablinkTrigCtl_CAMERA,
			EDeviceGrablinkTrigCtl_DIFF,
			EDeviceGrablinkTrigCtl_DTTL,
			EDeviceGrablinkTrigCtl_I12V,
			EDeviceGrablinkTrigCtl_ISO,
			EDeviceGrablinkTrigCtl_ITTL,
			EDeviceGrablinkTrigCtl_LVDS,
			EDeviceGrablinkTrigCtl_RELAY,
			EDeviceGrablinkTrigCtl_RS485,
			EDeviceGrablinkTrigCtl_TGOHIGH,
			EDeviceGrablinkTrigCtl_TGOLOW,
			EDeviceGrablinkTrigCtl_TTL,
			EDeviceGrablinkTrigCtl_TTLHiz,
			EDeviceGrablinkTrigCtl_TTLPD,
			EDeviceGrablinkTrigCtl_VGOHIGH,
			EDeviceGrablinkTrigCtl_VGOLOW,
			EDeviceGrablinkTrigCtl_Count,
		};

		enum EDeviceGrablinkTrigEdge
		{
			EDeviceGrablinkTrigEdge_GOHIGH = 0,
			EDeviceGrablinkTrigEdge_GOLOW,
			EDeviceGrablinkTrigEdge_GOOPEN,
			EDeviceGrablinkTrigEdge_HIGH,
			EDeviceGrablinkTrigEdge_LOW,
			EDeviceGrablinkTrigEdge_OPEN,
			EDeviceGrablinkTrigEdge_Count,
		};

		enum EDeviceGrablinkTrigFilter
		{
			EDeviceGrablinkTrigFilter_FILTER_100ms = 0,
			EDeviceGrablinkTrigFilter_FILTER_10ms,
			EDeviceGrablinkTrigFilter_MEDIUM,
			EDeviceGrablinkTrigFilter_OFF,
			EDeviceGrablinkTrigFilter_ON,
			EDeviceGrablinkTrigFilter_STRONG,
			EDeviceGrablinkTrigFilter_Count,
		};

		enum EDeviceGrablinkTrigLine
		{
			EDeviceGrablinkTrigLine_ALT1 = 0,
			EDeviceGrablinkTrigLine_ALT2,
			EDeviceGrablinkTrigLine_ALT3,
			EDeviceGrablinkTrigLine_ALT4,
			EDeviceGrablinkTrigLine_CAM,
			EDeviceGrablinkTrigLine_DIN1,
			EDeviceGrablinkTrigLine_DIN2,
			EDeviceGrablinkTrigLine_Filte,
			EDeviceGrablinkTrigLine_FVAL,
			EDeviceGrablinkTrigLine_I1,
			EDeviceGrablinkTrigLine_I2,
			EDeviceGrablinkTrigLine_I3,
			EDeviceGrablinkTrigLine_I4,
			EDeviceGrablinkTrigLine_IIN1,
			EDeviceGrablinkTrigLine_IIN2,
			EDeviceGrablinkTrigLine_IIN3,
			EDeviceGrablinkTrigLine_IIN4,
			EDeviceGrablinkTrigLine_IO,
			EDeviceGrablinkTrigLine_IO1,
			EDeviceGrablinkTrigLine_IO2,
			EDeviceGrablinkTrigLine_IO3,
			EDeviceGrablinkTrigLine_IO4,
			EDeviceGrablinkTrigLine_ISOA1,
			EDeviceGrablinkTrigLine_ISOA2,
			EDeviceGrablinkTrigLine_LTACT,
			EDeviceGrablinkTrigLine_NOM,
			EDeviceGrablinkTrigLine_NONE,
			EDeviceGrablinkTrigLine_PG,
			EDeviceGrablinkTrigLine_TR,
			EDeviceGrablinkTrigLine_TRA,
			EDeviceGrablinkTrigLine_TRA1,
			EDeviceGrablinkTrigLine_TRA2,
			EDeviceGrablinkTrigLine_TRB,
			EDeviceGrablinkTrigLine_TRC,
			EDeviceGrablinkTrigLine_TRD,
			EDeviceGrablinkTrigLine_TRG,
			EDeviceGrablinkTrigLine_TRX,
			EDeviceGrablinkTrigLine_TRY,
			EDeviceGrablinkTrigLine_TRZ,
			EDeviceGrablinkTrigLine_Count,
		};

		enum EDeviceGrablinkStrobeCtl
		{
			EDeviceGrablinkStrobeCtl_DIFF = 0,
			EDeviceGrablinkStrobeCtl_IOC,
			EDeviceGrablinkStrobeCtl_IOE,
			EDeviceGrablinkStrobeCtl_ITTL,
			EDeviceGrablinkStrobeCtl_NONE,
			EDeviceGrablinkStrobeCtl_OPTO,
			EDeviceGrablinkStrobeCtl_SSRLY,
			EDeviceGrablinkStrobeCtl_TTL,
			EDeviceGrablinkStrobeCtl_Count,
		};

		enum EDeviceGrablinkStrobeMode
		{
			EDeviceGrablinkStrobeMode_AUTO = 0,
			EDeviceGrablinkStrobeMode_MAN,
			EDeviceGrablinkStrobeMode_NONE,
			EDeviceGrablinkStrobeMode_OFF,
			EDeviceGrablinkStrobeMode_Count,
		};

		enum EDeviceGrablinkLineTrigCtl
		{
			EDeviceGrablinkLineTrigCtl_DIFF = 0,
			EDeviceGrablinkLineTrigCtl_DIFF_PAIRED,
			EDeviceGrablinkLineTrigCtl_I12V,
			EDeviceGrablinkLineTrigCtl_ISO,
			EDeviceGrablinkLineTrigCtl_ISO_PAIRED,
			EDeviceGrablinkLineTrigCtl_ITTL,
			EDeviceGrablinkLineTrigCtl_LVDS,
			EDeviceGrablinkLineTrigCtl_TTL,
			EDeviceGrablinkLineTrigCtl_Count,
		};

		enum EDeviceGrablinkLineTrigEdge
		{
			EDeviceGrablinkLineTrigEdge_ALL_A = 0,
			EDeviceGrablinkLineTrigEdge_ALL_A_B,
			EDeviceGrablinkLineTrigEdge_FALLING_A,
			EDeviceGrablinkLineTrigEdge_GOHIGH,
			EDeviceGrablinkLineTrigEdge_GOLOW,
			EDeviceGrablinkLineTrigEdge_RISING_A,
			EDeviceGrablinkLineTrigEdge_Count,
		};

		enum EDeviceGrablinkLineTrigFilter
		{
			EDeviceGrablinkLineTrigFilter_Filter_100ns = 0,
			EDeviceGrablinkLineTrigFilter_Filter_10us,
			EDeviceGrablinkLineTrigFilter_Filter_1us,
			EDeviceGrablinkLineTrigFilter_Filter_200ns,
			EDeviceGrablinkLineTrigFilter_Filter_40ns,
			EDeviceGrablinkLineTrigFilter_Filter_500ns,
			EDeviceGrablinkLineTrigFilter_Filter_5us,
			EDeviceGrablinkLineTrigFilter_MEDIUM,
			EDeviceGrablinkLineTrigFilter_OFF,
			EDeviceGrablinkLineTrigFilter_STRONG,
			EDeviceGrablinkLineTrigFilter_Count,
		};

		enum EDeviceGrablinkLineTrigLine
		{
			EDeviceGrablinkLineTrigLine_CAM = 0,
			EDeviceGrablinkLineTrigLine_DIN1,
			EDeviceGrablinkLineTrigLine_DIN1_DIN2,
			EDeviceGrablinkLineTrigLine_DIN2,
			EDeviceGrablinkLineTrigLine_IIN1,
			EDeviceGrablinkLineTrigLine_IIN1_IIN2,
			EDeviceGrablinkLineTrigLine_IIN2,
			EDeviceGrablinkLineTrigLine_IIN3,
			EDeviceGrablinkLineTrigLine_IIN3_IIN4,
			EDeviceGrablinkLineTrigLine_IIN4,
			EDeviceGrablinkLineTrigLine_IO1,
			EDeviceGrablinkLineTrigLine_IO2,
			EDeviceGrablinkLineTrigLine_IO3,
			EDeviceGrablinkLineTrigLine_IO4,
			EDeviceGrablinkLineTrigLine_ISOA1,
			EDeviceGrablinkLineTrigLine_ISOA2,
			EDeviceGrablinkLineTrigLine_NOM,
			EDeviceGrablinkLineTrigLine_TRA1,
			EDeviceGrablinkLineTrigLine_TRA2,
			EDeviceGrablinkLineTrigLine_TRG,
			EDeviceGrablinkLineTrigLine_TRX,
			EDeviceGrablinkLineTrigLine_TRY,
			EDeviceGrablinkLineTrigLine_TRZ,
			EDeviceGrablinkLineTrigLine_Count,
		};

		enum EDeviceGrablinkRateDividerRestart
		{
			EDeviceGrablinkRateDividerRestart_NEVER = 0,
			EDeviceGrablinkRateDividerRestart_START_OF_SCAN,
			EDeviceGrablinkRateDividerRestart_Count,
		};

		enum EDeviceGrablinkConnectLoc
		{
			EDeviceGrablinkConnectLoc_BOTH = 0,
			EDeviceGrablinkConnectLoc_CENTER,
			EDeviceGrablinkConnectLoc_FOURTH,
			EDeviceGrablinkConnectLoc_LEFTMOST,
			EDeviceGrablinkConnectLoc_LOWER,
			EDeviceGrablinkConnectLoc_MIDDLELEFT,
			EDeviceGrablinkConnectLoc_MIDDLERIGHT,
			EDeviceGrablinkConnectLoc_RIGHTMOST,
			EDeviceGrablinkConnectLoc_SECOND,
			EDeviceGrablinkConnectLoc_THIRD,
			EDeviceGrablinkConnectLoc_UPPER,
			EDeviceGrablinkConnectLoc_Count,
		};

		enum EDeviceGrablinkECCO_PLLResetControl
		{
			EDeviceGrablinkECCO_PLLResetControl_AUTOMATIC = 0,
			EDeviceGrablinkECCO_PLLResetControl_CHANNEL_ACTIVATI,
			EDeviceGrablinkECCO_PLLResetControl_CHANNEL_ACTIVE,
			EDeviceGrablinkECCO_PLLResetControl_Count,
		};

		enum EDeviceGrablinkECCO_SkewCompensation
		{
			EDeviceGrablinkECCO_SkewCompensation_OFF = 0,
			EDeviceGrablinkECCO_SkewCompensation_ON,
			EDeviceGrablinkECCO_SkewCompensation_Count,
		};

		enum EDeviceGrablinkFvalMin_Tk
		{
			EDeviceGrablinkFvalMin_Tk_1 = 0,
			EDeviceGrablinkFvalMin_Tk_3,
			EDeviceGrablinkFvalMin_Tk_Count,
		};

		enum EDeviceGrablinkLvalMin_Tk
		{
			EDeviceGrablinkLvalMin_Tk_1 = 0,
			EDeviceGrablinkLvalMin_Tk_2,
			EDeviceGrablinkLvalMin_Tk_Count,
		};

		enum EDeviceGrablinkMetadataContent
		{
			EDeviceGrablinkMetadataContent_NONE = 0,
			EDeviceGrablinkMetadataContent_ONE_FIELD,
			EDeviceGrablinkMetadataContent_THREE_FIELD,
			EDeviceGrablinkMetadataContent_TWO_FIELD,
			EDeviceGrablinkMetadataContent_Count,
		};

		enum EDeviceGrablinkMetadataInsertion
		{
			EDeviceGrablinkMetadataInsertion_DISABLE = 0,
			EDeviceGrablinkMetadataInsertion_ENABLE,
			EDeviceGrablinkMetadataInsertion_Count,
		};

		enum EDeviceGrablinkMetadataLocation
		{
			EDeviceGrablinkMetadataLocation_LEFT = 0,
			EDeviceGrablinkMetadataLocation_LEFT_BOARDER,
			EDeviceGrablinkMetadataLocation_LVALRISE,
			EDeviceGrablinkMetadataLocation_SPARSE_1,
			EDeviceGrablinkMetadataLocation_TAP10,
			EDeviceGrablinkMetadataLocation_Count,
		};

		enum EDeviceGrablinkGrabWindow
		{
			EDeviceGrablinkGrabWindow_FIT = 0,
			EDeviceGrablinkGrabWindow_MAN,
			EDeviceGrablinkGrabWindow_NOBLACK,
			EDeviceGrablinkGrabWindow_NOLOSS,
			EDeviceGrablinkGrabWindow_STD,
			EDeviceGrablinkGrabWindow_WOI,
			EDeviceGrablinkGrabWindow_WOI_NO,
			EDeviceGrablinkGrabWindow_WOI_YES,
			EDeviceGrablinkGrabWindow_Count,
		};

		enum EDeviceGrablinkColorFormat
		{
			EDeviceGrablinkColorFormat_ARGB32 = 0,
			EDeviceGrablinkColorFormat_ARGB64,
			EDeviceGrablinkColorFormat_BAYER10,
			EDeviceGrablinkColorFormat_BAYER12,
			EDeviceGrablinkColorFormat_BAYER14,
			EDeviceGrablinkColorFormat_BAYER16,
			EDeviceGrablinkColorFormat_BAYER8,
			EDeviceGrablinkColorFormat_BGR24,
			EDeviceGrablinkColorFormat_BGR32,
			EDeviceGrablinkColorFormat_DX50,
			EDeviceGrablinkColorFormat_FLOAT32,
			EDeviceGrablinkColorFormat_GRABBER,
			EDeviceGrablinkColorFormat_I420,
			EDeviceGrablinkColorFormat_IYU2,
			EDeviceGrablinkColorFormat_IYUV,
			EDeviceGrablinkColorFormat_JPEG,
			EDeviceGrablinkColorFormat_MJPG,
			EDeviceGrablinkColorFormat_MP4S,
			EDeviceGrablinkColorFormat_RAW10,
			EDeviceGrablinkColorFormat_RAW12,
			EDeviceGrablinkColorFormat_RAW14,
			EDeviceGrablinkColorFormat_RAW16,
			EDeviceGrablinkColorFormat_RAW8,
			EDeviceGrablinkColorFormat_RGB10_10_10,
			EDeviceGrablinkColorFormat_RGB10_12_10,
			EDeviceGrablinkColorFormat_RGB15,
			EDeviceGrablinkColorFormat_RGB15Y8,
			EDeviceGrablinkColorFormat_RGB16,
			EDeviceGrablinkColorFormat_RGB16Y8,
			EDeviceGrablinkColorFormat_RGB24,
			EDeviceGrablinkColorFormat_RGB24PL,
			EDeviceGrablinkColorFormat_RGB24PLY10,
			EDeviceGrablinkColorFormat_RGB24PLY12,
			EDeviceGrablinkColorFormat_RGB24PLY16,
			EDeviceGrablinkColorFormat_RGB24PLY8,
			EDeviceGrablinkColorFormat_RGB24Y10,
			EDeviceGrablinkColorFormat_RGB24Y12,
			EDeviceGrablinkColorFormat_RGB24Y16,
			EDeviceGrablinkColorFormat_RGB24Y8,
			EDeviceGrablinkColorFormat_RGB30,
			EDeviceGrablinkColorFormat_RGB30PL,
			EDeviceGrablinkColorFormat_RGB30PLY10,
			EDeviceGrablinkColorFormat_RGB30PLY12,
			EDeviceGrablinkColorFormat_RGB30PLY16,
			EDeviceGrablinkColorFormat_RGB30PLY8,
			EDeviceGrablinkColorFormat_RGB32,
			EDeviceGrablinkColorFormat_RGB36,
			EDeviceGrablinkColorFormat_RGB36PL,
			EDeviceGrablinkColorFormat_RGB36PLY10,
			EDeviceGrablinkColorFormat_RGB36PLY12,
			EDeviceGrablinkColorFormat_RGB36PLY16,
			EDeviceGrablinkColorFormat_RGB36PLY8,
			EDeviceGrablinkColorFormat_RGB42,
			EDeviceGrablinkColorFormat_RGB42PL,
			EDeviceGrablinkColorFormat_RGB48,
			EDeviceGrablinkColorFormat_RGB48PL,
			EDeviceGrablinkColorFormat_RGB48PLY10,
			EDeviceGrablinkColorFormat_RGB48PLY12,
			EDeviceGrablinkColorFormat_RGB48PLY16,
			EDeviceGrablinkColorFormat_RGB48PLY8,
			EDeviceGrablinkColorFormat_RGB64,
			EDeviceGrablinkColorFormat_UINT32,
			EDeviceGrablinkColorFormat_Y10,
			EDeviceGrablinkColorFormat_Y12,
			EDeviceGrablinkColorFormat_Y14,
			EDeviceGrablinkColorFormat_Y16,
			EDeviceGrablinkColorFormat_Y41B,
			EDeviceGrablinkColorFormat_Y41P,
			EDeviceGrablinkColorFormat_Y42B,
			EDeviceGrablinkColorFormat_Y42P,
			EDeviceGrablinkColorFormat_Y8,
			EDeviceGrablinkColorFormat_YRGB32,
			EDeviceGrablinkColorFormat_YRGB64,
			EDeviceGrablinkColorFormat_YUV411,
			EDeviceGrablinkColorFormat_YUV411PL,
			EDeviceGrablinkColorFormat_YUV411PL_DEC,
			EDeviceGrablinkColorFormat_YUV422,
			EDeviceGrablinkColorFormat_YUV422PL,
			EDeviceGrablinkColorFormat_YUV422PL_DEC,
			EDeviceGrablinkColorFormat_YUV444,
			EDeviceGrablinkColorFormat_YUV444PL,
			EDeviceGrablinkColorFormat_YUV9,
			EDeviceGrablinkColorFormat_YV12,
			EDeviceGrablinkColorFormat_YVU9,
			EDeviceGrablinkColorFormat_Count,
		};

		enum EDeviceGrablinkImageFlipX
		{
			EDeviceGrablinkImageFlipX_OFF = 0,
			EDeviceGrablinkImageFlipX_ON,
			EDeviceGrablinkImageFlipX_Count,
		};

		enum EDeviceGrablinkImageFlipY
		{
			EDeviceGrablinkImageFlipY_OFF = 0,
			EDeviceGrablinkImageFlipY_ON,
			EDeviceGrablinkImageFlipY_Count,
		};

		enum EDeviceGrablinkMaxFillingSurfaces
		{
			EDeviceGrablinkMaxFillingSurfaces_MAXIMUM = 0,
			EDeviceGrablinkMaxFillingSurfaces_MINIMUM,
			EDeviceGrablinkMaxFillingSurfaces_Count,
		};

		enum EDeviceGrablinkRedBlueSwap
		{
			EDeviceGrablinkRedBlueSwap_DISABLE = 0,
			EDeviceGrablinkRedBlueSwap_ENABLE,
			EDeviceGrablinkRedBlueSwap_Count,
		};

		enum EDeviceGrablinkChannelState
		{
			EDeviceGrablinkChannelState_ACTIVE = 0,
			EDeviceGrablinkChannelState_FREE,
			EDeviceGrablinkChannelState_IDLE,
			EDeviceGrablinkChannelState_ORPHAN,
			EDeviceGrablinkChannelState_READY,
			EDeviceGrablinkChannelState_Count,
		};

		enum EDeviceGrablinkAcquisitionCleanup
		{
			EDeviceGrablinkAcquisitionCleanup_DISABLED = 0,
			EDeviceGrablinkAcquisitionCleanup_ENABLED,
			EDeviceGrablinkAcquisitionCleanup_Count,
		};

		enum EDeviceGrablinkParamsConsistencyCheck
		{
			EDeviceGrablinkParamsConsistencyCheck_AUTO = 0,
			EDeviceGrablinkParamsConsistencyCheck_MANUAL,
			EDeviceGrablinkParamsConsistencyCheck_Count,
		};

		enum EDeviceGrablinkTriggerSkipHold
		{
			EDeviceGrablinkTriggerSkipHold_HOLD = 0,
			EDeviceGrablinkTriggerSkipHold_SKIP,
			EDeviceGrablinkTriggerSkipHold_Count,
		};

		enum EDeviceGrablinkCallbackPriority
		{
			EDeviceGrablinkCallbackPriority_ABOVE_NORMAL = 0,
			EDeviceGrablinkCallbackPriority_BELOW_NORMAL,
			EDeviceGrablinkCallbackPriority_HIGHEST,
			EDeviceGrablinkCallbackPriority_LOWEST,
			EDeviceGrablinkCallbackPriority_NORMAL,
			EDeviceGrablinkCallbackPriority_TIME_CRITICAL,
			EDeviceGrablinkCallbackPriority_Count,
		};

		enum EDeviceGrablinkInterleavedAcquisition
		{
			EDeviceGrablinkInterleavedAcquisition_OFF = 0,
			EDeviceGrablinkInterleavedAcquisition_ON,
			EDeviceGrablinkInterleavedAcquisition_Count,
		};
	}
}

// Euresys - Grablink - Value
namespace Ravid
{
	namespace Device
	{
		enum EDeviceEuresysValueTopology
		{
			EDeviceEuresysValueTopology_None = 0,
			EDeviceEuresysValueTopology_Count,
		};

		enum EDeviceEuresysValueConnector
		{
			EDeviceEuresysValueConnector_M = 0,
			EDeviceEuresysValueConnector_Count,
		};
	}
}

// Euresys - Grablink - Base
namespace Ravid
{
	namespace Device
	{
		enum EDeviceEuresysBaseTopology
		{
			EDeviceEuresysBaseTopology_MONO = 0,
			EDeviceEuresysBaseTopology_MONO_EF1,
			EDeviceEuresysBaseTopology_MONO_EXT1,
			EDeviceEuresysBaseTopology_MONO_From20MHzTo35MHz,
			EDeviceEuresysBaseTopology_MONO_From30MHzTo70MHz,
			EDeviceEuresysBaseTopology_MONO_From60MHzTo85MHz,
			EDeviceEuresysBaseTopology_MONO_OPT1,
			EDeviceEuresysBaseTopology_MONO_SLOW,
			EDeviceEuresysBaseTopology_Count,
		};

		enum EDeviceEuresysBaseConnector
		{
			EDeviceEuresysBaseConnector_M = 0,
			EDeviceEuresysBaseConnector_Count,
		};
	}
}

// Euresys - Grablink - Dualbase
namespace Ravid
{
	namespace Device
	{
		enum EDeviceEuresysDualbaseTopology
		{
			EDeviceEuresysDualbaseTopology_DUO = 0,
			EDeviceEuresysDualbaseTopology_DUO_EXT1,
			EDeviceEuresysDualbaseTopology_DUO_OPT1,
			EDeviceEuresysDualbaseTopology_DUO_SLOW,
			EDeviceEuresysDualbaseTopology_Count,
		};

		enum EDeviceEuresysDualbaseConnector
		{
			EDeviceEuresysDualbaseConnector_A = 0,
			EDeviceEuresysDualbaseConnector_B,
			EDeviceEuresysDualbaseConnector_Count,
		};
	}
}

// Euresys - Grablink - Full
namespace Ravid
{
	namespace Device
	{
		enum EDeviceEuresysFullTopology
		{
			EDeviceEuresysFullTopology_DECA = 0,
			EDeviceEuresysFullTopology_MONO,
			EDeviceEuresysFullTopology_MONO_DECA,
			EDeviceEuresysFullTopology_MONO_DECA_AFE,
			EDeviceEuresysFullTopology_MONO_DECA_AFE_SE1,
			EDeviceEuresysFullTopology_MONO_DECA_DFE,
			EDeviceEuresysFullTopology_MONO_DECA_EF1,
			EDeviceEuresysFullTopology_MONO_DECA_EXT1,
			EDeviceEuresysFullTopology_MONO_DECA_OPT1,
			EDeviceEuresysFullTopology_MONO_EF1,
			EDeviceEuresysFullTopology_MONO_EXT1,
			EDeviceEuresysFullTopology_MONO_From20MHzTo35MHz,
			EDeviceEuresysFullTopology_MONO_From30MHzTo70MHz,
			EDeviceEuresysFullTopology_MONO_From60MHzTo85MHz,
			EDeviceEuresysFullTopology_MONO_OPT1,
			EDeviceEuresysFullTopology_MONO_SLOW,
			EDeviceEuresysFullTopology_Count,
		};

		enum EDeviceEuresysFullConnector
		{
			EDeviceEuresysFullConnector_M,
			EDeviceEuresysFullConnector_Count,
		};
	}
}

// Euresys - Domino
namespace Ravid
{
	namespace Device
	{
		enum EDeviceDominoVideoFilter
		{
			EDeviceDominoVideoFilter_NONE = 0,
			EDeviceDominoVideoFilter_COLORTRAP,
			EDeviceDominoVideoFilter_Count,
		};

		enum EDeviceDominoTapStructure
		{
			EDeviceDominoTapStructure_SINGLE_A = 0,
			EDeviceDominoTapStructure_TRIPLE_A,
			EDeviceDominoTapStructure_Count,
		};

		enum EDeviceDominoScanning
		{
			EDeviceDominoScanning_PROGRESSIVE = 0,
			EDeviceDominoScanning_INTERLACE,
			EDeviceDominoScanning_Count,
		};

		enum EDeviceDominoStandard
		{
			EDeviceDominoStandard_CCIR = 0,
			EDeviceDominoStandard_EIA,
			EDeviceDominoStandard_Count
		};

		enum EDeviceDominoACReset
		{
			EDeviceDominoACReset_NO = 0,
			EDeviceDominoACReset_YES,
			EDeviceDominoACReset_Count,
		};

		enum EDeviceDominoChannelTopology
		{
			EDeviceDominoChannelTopology_SINGLE = 0,
			EDeviceDominoChannelTopology_TRIPLE,
			EDeviceDominoChannelTopology_Count,
		};

		enum EDeviceDominoCsyncCtl
		{
			EDeviceDominoCsyncCtl_NONE = 0,
			EDeviceDominoCsyncCtl_TTL,
			EDeviceDominoCsyncCtl_DIFF,
			EDeviceDominoCsyncCtl_Count,
		};

		enum EDeviceDominoCsyncEdge
		{
			EDeviceDominoCsyncEdge_GOHIGH = 0,
			EDeviceDominoCsyncEdge_GOLOW,
			EDeviceDominoCsyncEdge_Count,
		};

		enum EDeviceDominoCameraControlSignalSet
		{
			EDeviceDominoCameraControlSignalSet_CSYNC = 0,
			EDeviceDominoCameraControlSignalSet_HDRIVE,
			EDeviceDominoCameraControlSignalSet_HDRIVE_RESET,
			EDeviceDominoCameraControlSignalSet_HDRIVE_RESET_VSYNC,
			EDeviceDominoCameraControlSignalSet_HDRIVE_VDRIVE,
			EDeviceDominoCameraControlSignalSet_HDRIVE_VSYNC,
			EDeviceDominoCameraControlSignalSet_HSYNC,
			EDeviceDominoCameraControlSignalSet_HSYNC_VSYNC,
			EDeviceDominoCameraControlSignalSet_INVALID,
			EDeviceDominoCameraControlSignalSet_NONE,
			EDeviceDominoCameraControlSignalSet_RESET,
			EDeviceDominoCameraControlSignalSet_RESET_HSYNC,
			EDeviceDominoCameraControlSignalSet_RESET_HSYNC_VSYNC,
			EDeviceDominoCameraControlSignalSet_RESET_VSYNC,
			EDeviceDominoCameraControlSignalSet_UNAVAILABLE_PxxRA_M,
			EDeviceDominoCameraControlSignalSet_UNAVAILABLE_PxxRM_1,
			EDeviceDominoCameraControlSignalSet_UNAVAILABLE_PxxRM_2,
			EDeviceDominoCameraControlSignalSet_Count,
		};

		enum EDeviceDominoHReset
		{
			EDeviceDominoHReset_NO = 0,
			EDeviceDominoHReset_YES,
			EDeviceDominoHReset_Count,
		};

		enum EDeviceDominoHsyncCtl
		{
			EDeviceDominoHsyncCtl_NONE = 0,
			EDeviceDominoHsyncCtl_TTL,
			EDeviceDominoHsyncCtl_DIFF,
			EDeviceDominoHsyncCtl_Count,
		};

		enum EDeviceDominoHsyncEdge
		{
			EDeviceDominoHsyncEdge_GOHIGH = 0,
			EDeviceDominoHsyncEdge_GOLOW,
			EDeviceDominoHsyncEdge_Count,
		};

		enum EDeviceDominoHVdriveCtl
		{
			EDeviceDominoHVdriveCtl_NONE = 0,
			EDeviceDominoHVdriveCtl_TTL,
			EDeviceDominoHVdriveCtl_DIFF,
			EDeviceDominoHVdriveCtl_Count,
		};

		enum EDeviceDominoHVdriveEdge
		{
			EDeviceDominoHVdriveEdge_GOHIGH = 0,
			EDeviceDominoHVdriveEdge_GOLOW,
			EDeviceDominoHVdriveEdge_Count,
		};

		enum EDeviceDominoPixelClkCtl
		{
			EDeviceDominoPixelClkCtl_NONE = 0,
			EDeviceDominoPixelClkCtl_Count,
		};

		enum EDeviceDominoVCSync
		{
			EDeviceDominoVCSync_YES = 0,
			EDeviceDominoVCSync_NO,
			EDeviceDominoVCSync_Count,
		};

		enum EDeviceDominoVHSync
		{
			EDeviceDominoVHSync_YES = 0,
			EDeviceDominoVHSync_NO,
			EDeviceDominoVHSync_Count,
		};

		enum EDeviceDominoVsyncCtl
		{
			EDeviceDominoVsyncCtl_NONE = 0,
			EDeviceDominoVsyncCtl_TTL,
			EDeviceDominoVsyncCtl_DIFF,
			EDeviceDominoVsyncCtl_Count,
		};

		enum EDeviceDominoVsyncEdge
		{
			EDeviceDominoVsyncEdge_GOHIGH = 0,
			EDeviceDominoVsyncEdge_GOLOW,
			EDeviceDominoVsyncEdge_Count,
		};

		enum EDeviceDominoCable
		{
			EDeviceDominoCable_A15_C00 = 0,
			EDeviceDominoCable_A15_C01,
			EDeviceDominoCable_A15_C02,
			EDeviceDominoCable_A15_C03,
			EDeviceDominoCable_A15_C05,
			EDeviceDominoCable_A15_C06,
			EDeviceDominoCable_A15_C07,
			EDeviceDominoCable_A15_C08,
			EDeviceDominoCable_A15_C09,
			EDeviceDominoCable_A15_C11,
			EDeviceDominoCable_A15_C12,
			EDeviceDominoCable_A15_C13,
			EDeviceDominoCable_A15_C14,
			EDeviceDominoCable_A15_C15,
			EDeviceDominoCable_A15_C16,
			EDeviceDominoCable_A15_C17,
			EDeviceDominoCable_A15_C18,
			EDeviceDominoCable_A15_C19,
			EDeviceDominoCable_A15_C20,
			EDeviceDominoCable_A15_C21,
			EDeviceDominoCable_Count,
		};

		enum EDeviceDominoCableName // 확인 필요
		{
			EDeviceDominoCableName_My_Cable = 0,
			EDeviceDominoCableName_GenericCable,
			EDeviceDominoCableName_Single_Channel_R,
			EDeviceDominoCableName_Single_Channel_R_B,
			EDeviceDominoCableName_Single_Channel_R_C,
			EDeviceDominoCableName_Single_Channel_RC,
			EDeviceDominoCableName_Single_Channel_RG,
			EDeviceDominoCableName_Single_Channel_RGC,
			EDeviceDominoCableName_Single_Channel_RGC_A,
			EDeviceDominoCableName_Single_Channel_RGC_wo_PxClk,
			EDeviceDominoCableName_Single_Channel_RGC_2,
			EDeviceDominoCableName_Dual_Channel,
			EDeviceDominoCableName_Dual_Channel_RG,
			EDeviceDominoCableName_Dual_Channel_RG_2,
			EDeviceDominoCableName_Dual_Channel_RG_2_A,
			EDeviceDominoCableName_Triple_Channel_R,
			EDeviceDominoCableName_Triple_Channel_RG_2,
			EDeviceDominoCableName_Triple_Channel_RG_3,
			EDeviceDominoCableName_Count,
		};

		enum EDeviceDominoAuxResetLine
		{
			EDeviceDominoAuxResetLine_NC = 0,
			EDeviceDominoAuxResetLine_CC5,
			EDeviceDominoAuxResetLine_CC3,
			EDeviceDominoAuxResetLine_Count,
		};

		enum EDeviceDominoCsyncLine
		{
			EDeviceDominoCsyncLine_CC1 = 0,
			EDeviceDominoCsyncLine_CC2,
			EDeviceDominoCsyncLine_CC3,
			EDeviceDominoCsyncLine_CC4,
			EDeviceDominoCsyncLine_CC5,
			EDeviceDominoCsyncLine_NC,
			EDeviceDominoCsyncLine_Count,
		};

		enum EDeviceDominoHdriveLine
		{
			EDeviceDominoHdriveLine_CC1 = 0,
			EDeviceDominoHdriveLine_CC2,
			EDeviceDominoHdriveLine_CC3,
			EDeviceDominoHdriveLine_CC4,
			EDeviceDominoHdriveLine_CC5,
			EDeviceDominoHdriveLine_NC,
			EDeviceDominoHdriveLine_Count,
		};

		enum EDeviceDominoHsyncLine
		{
			EDeviceDominoHsyncLine_CC1 = 0,
			EDeviceDominoHsyncLine_CC2,
			EDeviceDominoHsyncLine_CC3,
			EDeviceDominoHsyncLine_CC4,
			EDeviceDominoHsyncLine_CC5,
			EDeviceDominoHsyncLine_NC,
			EDeviceDominoHsyncLine_Count,
		};

		enum EDeviceDominoPixelClkLine
		{
			EDeviceDominoPixelClkLine_NC = 0,
			EDeviceDominoPixelClkLine_Count,
		};

		enum EDeviceDominoVdriveLine
		{
			EDeviceDominoVdriveLine_CC1 = 0,
			EDeviceDominoVdriveLine_CC2,
			EDeviceDominoVdriveLine_CC3,
			EDeviceDominoVdriveLine_CC4,
			EDeviceDominoVdriveLine_CC5,
			EDeviceDominoVdriveLine_NC,
			EDeviceDominoVdriveLine_Count,
		};

		enum EDeviceDominoVsyncLine
		{
			EDeviceDominoVsyncLine_CC1 = 0,
			EDeviceDominoVsyncLine_CC2,
			EDeviceDominoVsyncLine_CC3,
			EDeviceDominoVsyncLine_CC4,
			EDeviceDominoVsyncLine_CC5,
			EDeviceDominoVsyncLine_NC,
			EDeviceDominoVsyncLine_Count,
		};

		enum EDeviceDominoGrabField
		{
			EDeviceDominoGrabField_FLD = 0,
			EDeviceDominoGrabField_UP,
			EDeviceDominoGrabField_DOWN,
			EDeviceDominoGrabField_FRAME,
			EDeviceDominoGrabField_UPDW,
			EDeviceDominoGrabField_DWUP,
			EDeviceDominoGrabField_Count,
		};

		enum EDeviceDominoNextGrabField
		{
			EDeviceDominoNextGrabField_FLD = 0,
			EDeviceDominoNextGrabField_UP,
			EDeviceDominoNextGrabField_DOWN,
			EDeviceDominoNextGrabField_FRAME,
			EDeviceDominoNextGrabField_UPDW,
			EDeviceDominoNextGrabField_DWUP,
			EDeviceDominoNextGrabField_SAME,
			EDeviceDominoNextGrabField_KEEP,
			EDeviceDominoNextGrabField_CHANGE,
			EDeviceDominoNextGrabField_Count,
		};

		enum EDeviceDominoStrobeLine
		{
			EDeviceDominoStrobeLine_NOM = 0,
			EDeviceDominoStrobeLine_IO1,
			EDeviceDominoStrobeLine_IO2,
			EDeviceDominoStrobeLine_IO3,
			EDeviceDominoStrobeLine_IO4,
			EDeviceDominoStrobeLine_ST,
			EDeviceDominoStrobeLine_STX,
			EDeviceDominoStrobeLine_STY,
			EDeviceDominoStrobeLine_STA,
			EDeviceDominoStrobeLine_STB,
			EDeviceDominoStrobeLine_STC,
			EDeviceDominoStrobeLine_STD,
			EDeviceDominoStrobeLine_Count,
		};

		enum EDeviceDominoStrobeLevel
		{
			EDeviceDominoStrobeLevel_PLSHIGH = 0,
			EDeviceDominoStrobeLevel_PLSLOW,
			EDeviceDominoStrobeLevel_Count,
		};

		enum EDeviceDominoADresolution
		{
			EDeviceDominoADresolution_10BIT = 0,
			EDeviceDominoADresolution_Count,
		};

		enum EDeviceDominoVideoTerminator
		{
			EDeviceDominoVideoTerminator_ON = 0,
			EDeviceDominoVideoTerminator_OFF,
			EDeviceDominoVideoTerminator_Count,
		};

		enum EDeviceDominoHMode
		{
			EDeviceDominoHMode_ANALOG_LANE1 = 0,
			EDeviceDominoHMode_DIGITAL_PULSED,
			EDeviceDominoHMode_DIGITAL_MIXED,
			EDeviceDominoHMode_LOCAL,
			EDeviceDominoHMode_Count,
		};

		enum EDeviceDominoHReference
		{
			EDeviceDominoHReference_CAMERA = 0,
			EDeviceDominoHReference_LOCAL,
			EDeviceDominoHReference_Count,
		};

		enum EDeviceDominoVMode
		{
			EDeviceDominoVMode_ANALOG_LANE1 = 0,
			EDeviceDominoVMode_DIGITAL_PULSED,
			EDeviceDominoVMode_DIGITAL_MIXED,
			EDeviceDominoVMode_LOCAL,
			EDeviceDominoVMode_Count,
		};

		enum EDeviceDominoVReference
		{
			EDeviceDominoVReference_CAMERA = 0,
			EDeviceDominoVReference_LOCAL,
			EDeviceDominoVReference_Count,
		};

		enum EDeviceDominoGainCtl
		{
			EDeviceDominoGainCtl_LIN = 0,
			EDeviceDominoGainCtl_P3DB,
			EDeviceDominoGainCtl_P2DB,
			EDeviceDominoGainCtl_P1DB,
			EDeviceDominoGainCtl_0DB,
			EDeviceDominoGainCtl_M1dB,
			EDeviceDominoGainCtl_M2dB,
			EDeviceDominoGainCtl_M3dB,
			EDeviceDominoGainCtl_Count,
		};

		enum EDeviceDominoCalPat
		{
			EDeviceDominoCalPat_NONE = 0,
			EDeviceDominoCalPat_TPG,
			EDeviceDominoCalPat_Count,
		};

		enum EDeviceDominoSurfaceAllocation
		{
			EDeviceDominoSurfaceAllocation_ANYWHERE = 0,
			EDeviceDominoSurfaceAllocation_BELOW4G,
			EDeviceDominoSurfaceAllocation_Count,
		};

		enum EDeviceDominoAcquisitionMode
		{
			EDeviceDominoAcquisitionMode_HFR = 0,
			EDeviceDominoAcquisitionMode_INVALID,
			EDeviceDominoAcquisitionMode_LONGPAGE,
			EDeviceDominoAcquisitionMode_PAGE,
			EDeviceDominoAcquisitionMode_SNAPSHOT,
			EDeviceDominoAcquisitionMode_VIDEO,
			EDeviceDominoAcquisitionMode_WEB,
			EDeviceDominoAcquisitionMode_Count,
		};

		enum EDeviceDominoTrigMode
		{
			EDeviceDominoTrigMode_AUTO = 0,
			EDeviceDominoTrigMode_COMBINED,
			EDeviceDominoTrigMode_ENC,
			EDeviceDominoTrigMode_EXT,
			EDeviceDominoTrigMode_EXTRC,
			EDeviceDominoTrigMode_HARD,
			EDeviceDominoTrigMode_IMMEDIATE,
			EDeviceDominoTrigMode_INT,
			EDeviceDominoTrigMode_MASTER_CHANNEL,
			EDeviceDominoTrigMode_PAUSE,
			EDeviceDominoTrigMode_SLAVE,
			EDeviceDominoTrigMode_SOFT,
			EDeviceDominoTrigMode_Count,
		};

		enum EDeviceDominoCamConfig
		{
			EDeviceDominoCamConfig_FACTORY = 0,
			EDeviceDominoCamConfig_I10SC,
			EDeviceDominoCamConfig_I120RA,
			EDeviceDominoCamConfig_I120SA,
			EDeviceDominoCamConfig_I120SM,
			EDeviceDominoCamConfig_I15RA,
			EDeviceDominoCamConfig_I15SA,
			EDeviceDominoCamConfig_I25SA,
			EDeviceDominoCamConfig_I30SA,
			EDeviceDominoCamConfig_I50RA,
			EDeviceDominoCamConfig_I50SA,
			EDeviceDominoCamConfig_I50SM,
			EDeviceDominoCamConfig_I50SM_R,
			EDeviceDominoCamConfig_I60RA,
			EDeviceDominoCamConfig_I60SA,
			EDeviceDominoCamConfig_I60SM,
			EDeviceDominoCamConfig_I60SM_R,
			EDeviceDominoCamConfig_I7RA,
			EDeviceDominoCamConfig_I7SA,
			EDeviceDominoCamConfig_IxxRM,
			EDeviceDominoCamConfig_IxxRM_4,
			EDeviceDominoCamConfig_IxxSA,
			EDeviceDominoCamConfig_IxxSA_D,
			EDeviceDominoCamConfig_IxxSC,
			EDeviceDominoCamConfig_IxxSD,
			EDeviceDominoCamConfig_IxxSM,
			EDeviceDominoCamConfig_IxxSM_L,
			EDeviceDominoCamConfig_IxxSM_R,
			EDeviceDominoCamConfig_L1024RC,
			EDeviceDominoCamConfig_L1024RG,
			EDeviceDominoCamConfig_L1024RG_Base,
			EDeviceDominoCamConfig_L1024RG_Medium,
			EDeviceDominoCamConfig_L1024RGM,
			EDeviceDominoCamConfig_L1024RP,
			EDeviceDominoCamConfig_L1024SC,
			EDeviceDominoCamConfig_L1024SP,
			EDeviceDominoCamConfig_L12500RP,
			EDeviceDominoCamConfig_L1365RG,
			EDeviceDominoCamConfig_L2048RC,
			EDeviceDominoCamConfig_L2048RG,
			EDeviceDominoCamConfig_L2048RG_Base,
			EDeviceDominoCamConfig_L2048RG_Medium,
			EDeviceDominoCamConfig_L2048RGM,
			EDeviceDominoCamConfig_L2048RP,
			EDeviceDominoCamConfig_L2048SC,
			EDeviceDominoCamConfig_L2048SP,
			EDeviceDominoCamConfig_L2098RC,
			EDeviceDominoCamConfig_L2098RG,
			EDeviceDominoCamConfig_L2098RP,
			EDeviceDominoCamConfig_L2098SC,
			EDeviceDominoCamConfig_L2098SP,
			EDeviceDominoCamConfig_L4080RC,
			EDeviceDominoCamConfig_L4080RG,
			EDeviceDominoCamConfig_L4080RP,
			EDeviceDominoCamConfig_L4080SC,
			EDeviceDominoCamConfig_L4080SP,
			EDeviceDominoCamConfig_L4096RC,
			EDeviceDominoCamConfig_L4096RG,
			EDeviceDominoCamConfig_L4096RGS,
			EDeviceDominoCamConfig_L4096RP,
			EDeviceDominoCamConfig_L4096SC,
			EDeviceDominoCamConfig_L4096SP,
			EDeviceDominoCamConfig_L512RC,
			EDeviceDominoCamConfig_L512RG,
			EDeviceDominoCamConfig_L512RP,
			EDeviceDominoCamConfig_L512SC,
			EDeviceDominoCamConfig_L512SP,
			EDeviceDominoCamConfig_L5150RP,
			EDeviceDominoCamConfig_L5150SP,
			EDeviceDominoCamConfig_L6144RC,
			EDeviceDominoCamConfig_L6144RG,
			EDeviceDominoCamConfig_L6144RP,
			EDeviceDominoCamConfig_L6144SC,
			EDeviceDominoCamConfig_L6144SP,
			EDeviceDominoCamConfig_L7400RP,
			EDeviceDominoCamConfig_L7400SP,
			EDeviceDominoCamConfig_L7500RC,
			EDeviceDominoCamConfig_L7500RG,
			EDeviceDominoCamConfig_L7500RP,
			EDeviceDominoCamConfig_L7500SC,
			EDeviceDominoCamConfig_L7500SP,
			EDeviceDominoCamConfig_L8192RC,
			EDeviceDominoCamConfig_L8192RG,
			EDeviceDominoCamConfig_L8192RP,
			EDeviceDominoCamConfig_L8192SC,
			EDeviceDominoCamConfig_L8192SP,
			EDeviceDominoCamConfig_LxxxxRC,
			EDeviceDominoCamConfig_LxxxxRG,
			EDeviceDominoCamConfig_LxxxxRG2,
			EDeviceDominoCamConfig_LxxxxRP,
			EDeviceDominoCamConfig_LxxxxSC,
			EDeviceDominoCamConfig_LxxxxSP,
			EDeviceDominoCamConfig_P08RG,
			EDeviceDominoCamConfig_P08SC,
			EDeviceDominoCamConfig_P100RA,
			EDeviceDominoCamConfig_P100RA_G,
			EDeviceDominoCamConfig_P100RC,
			EDeviceDominoCamConfig_P100SA,
			EDeviceDominoCamConfig_P100SC,
			EDeviceDominoCamConfig_P106RM_D,
			EDeviceDominoCamConfig_P106SM,
			EDeviceDominoCamConfig_P10RA,
			EDeviceDominoCamConfig_P10RC,
			EDeviceDominoCamConfig_P10RD,
			EDeviceDominoCamConfig_P10RG,
			EDeviceDominoCamConfig_P10RM,
			EDeviceDominoCamConfig_P10SA,
			EDeviceDominoCamConfig_P10SC,
			EDeviceDominoCamConfig_P10SD,
			EDeviceDominoCamConfig_P10SM,
			EDeviceDominoCamConfig_P117RG,
			EDeviceDominoCamConfig_P117SC,
			EDeviceDominoCamConfig_P120RA,
			EDeviceDominoCamConfig_P120RG,
			EDeviceDominoCamConfig_P120RM,
			EDeviceDominoCamConfig_P120RM_D,
			EDeviceDominoCamConfig_P120RM_G,
			EDeviceDominoCamConfig_P120SA,
			EDeviceDominoCamConfig_P120SC,
			EDeviceDominoCamConfig_P120SM,
			EDeviceDominoCamConfig_P12RA,
			EDeviceDominoCamConfig_P12RC,
			EDeviceDominoCamConfig_P12RD,
			EDeviceDominoCamConfig_P12RG,
			EDeviceDominoCamConfig_P12RM,
			EDeviceDominoCamConfig_P12SA,
			EDeviceDominoCamConfig_P12SC,
			EDeviceDominoCamConfig_P12SD,
			EDeviceDominoCamConfig_P12SM,
			EDeviceDominoCamConfig_P13SC,
			EDeviceDominoCamConfig_P150RG,
			EDeviceDominoCamConfig_P150SC,
			EDeviceDominoCamConfig_P15RA,
			EDeviceDominoCamConfig_P15RC,
			EDeviceDominoCamConfig_P15RD,
			EDeviceDominoCamConfig_P15RG,
			EDeviceDominoCamConfig_P15RM,
			EDeviceDominoCamConfig_P15SA,
			EDeviceDominoCamConfig_P15SC,
			EDeviceDominoCamConfig_P15SD,
			EDeviceDominoCamConfig_P15SM,
			EDeviceDominoCamConfig_P16RA,
			EDeviceDominoCamConfig_P16RA_G,
			EDeviceDominoCamConfig_P16RC,
			EDeviceDominoCamConfig_P16RD,
			EDeviceDominoCamConfig_P16RD_G,
			EDeviceDominoCamConfig_P16RG,
			EDeviceDominoCamConfig_P16RM,
			EDeviceDominoCamConfig_P16RM_G,
			EDeviceDominoCamConfig_P16SA,
			EDeviceDominoCamConfig_P16SC,
			EDeviceDominoCamConfig_P16SD,
			EDeviceDominoCamConfig_P16SM,
			EDeviceDominoCamConfig_P17RA,
			EDeviceDominoCamConfig_P17RA_G,
			EDeviceDominoCamConfig_P17RD,
			EDeviceDominoCamConfig_P17RD_G,
			EDeviceDominoCamConfig_P17RG,
			EDeviceDominoCamConfig_P17SA,
			EDeviceDominoCamConfig_P17SC,
			EDeviceDominoCamConfig_P17SD,
			EDeviceDominoCamConfig_P200RC,
			EDeviceDominoCamConfig_P200RG,
			EDeviceDominoCamConfig_P200SC,
			EDeviceDominoCamConfig_P21SC,
			EDeviceDominoCamConfig_P22RC,
			EDeviceDominoCamConfig_P22RG,
			EDeviceDominoCamConfig_P22SC,
			EDeviceDominoCamConfig_P240RA,
			EDeviceDominoCamConfig_P24RG,
			EDeviceDominoCamConfig_P24SC,
			EDeviceDominoCamConfig_P25RA,
			EDeviceDominoCamConfig_P25RA_G,
			EDeviceDominoCamConfig_P25RD,
			EDeviceDominoCamConfig_P25RG,
			EDeviceDominoCamConfig_P25SA,
			EDeviceDominoCamConfig_P25SC,
			EDeviceDominoCamConfig_P25SC_S10,
			EDeviceDominoCamConfig_P25SC_S8,
			EDeviceDominoCamConfig_P25SM_L,
			EDeviceDominoCamConfig_P27RC,
			EDeviceDominoCamConfig_P27SC,
			EDeviceDominoCamConfig_P30RA,
			EDeviceDominoCamConfig_P30RA_D,
			EDeviceDominoCamConfig_P30RA_G,
			EDeviceDominoCamConfig_P30RC,
			EDeviceDominoCamConfig_P30RD,
			EDeviceDominoCamConfig_P30RG,
			EDeviceDominoCamConfig_P30RG_Base,
			EDeviceDominoCamConfig_P30RG_Medium,
			EDeviceDominoCamConfig_P30RG_O,
			EDeviceDominoCamConfig_P30RG_O_Base,
			EDeviceDominoCamConfig_P30RG_O_Medium,
			EDeviceDominoCamConfig_P30RM,
			EDeviceDominoCamConfig_P30RM_2,
			EDeviceDominoCamConfig_P30RM_G,
			EDeviceDominoCamConfig_P30SA,
			EDeviceDominoCamConfig_P30SC,
			EDeviceDominoCamConfig_P30SC_Base,
			EDeviceDominoCamConfig_P30SC_Medium,
			EDeviceDominoCamConfig_P30SD,
			EDeviceDominoCamConfig_P30SM,
			EDeviceDominoCamConfig_P30SM_L,
			EDeviceDominoCamConfig_P32RG,
			EDeviceDominoCamConfig_P32SC,
			EDeviceDominoCamConfig_P33RG,
			EDeviceDominoCamConfig_P33SC,
			EDeviceDominoCamConfig_P34RG,
			EDeviceDominoCamConfig_P34SC,
			EDeviceDominoCamConfig_P400SM,
			EDeviceDominoCamConfig_P42RG,
			EDeviceDominoCamConfig_P42SC,
			EDeviceDominoCamConfig_P48RG,
			EDeviceDominoCamConfig_P48SC,
			EDeviceDominoCamConfig_P50,
			EDeviceDominoCamConfig_P50RA,
			EDeviceDominoCamConfig_P50RA_G,
			EDeviceDominoCamConfig_P50RD,
			EDeviceDominoCamConfig_P50RG,
			EDeviceDominoCamConfig_P50RG_DX,
			EDeviceDominoCamConfig_P50RM,
			EDeviceDominoCamConfig_P50RM_G,
			EDeviceDominoCamConfig_P50SA,
			EDeviceDominoCamConfig_P50SC,
			EDeviceDominoCamConfig_P50SC_D10,
			EDeviceDominoCamConfig_P50SC_D8,
			EDeviceDominoCamConfig_P50SD,
			EDeviceDominoCamConfig_P50SM,
			EDeviceDominoCamConfig_P50SM_L,
			EDeviceDominoCamConfig_P60RA,
			EDeviceDominoCamConfig_P60RA_D,
			EDeviceDominoCamConfig_P60RA_G,
			EDeviceDominoCamConfig_P60RC,
			EDeviceDominoCamConfig_P60RD,
			EDeviceDominoCamConfig_P60RD_G,
			EDeviceDominoCamConfig_P60RG,
			EDeviceDominoCamConfig_P60RM,
			EDeviceDominoCamConfig_P60RM_A,
			EDeviceDominoCamConfig_P60RM_D,
			EDeviceDominoCamConfig_P60RM_G,
			EDeviceDominoCamConfig_P60RM_O,
			EDeviceDominoCamConfig_P60RM_R,
			EDeviceDominoCamConfig_P60SA,
			EDeviceDominoCamConfig_P60SC,
			EDeviceDominoCamConfig_P60SD,
			EDeviceDominoCamConfig_P60SM,
			EDeviceDominoCamConfig_P60SM_L,
			EDeviceDominoCamConfig_P64RG,
			EDeviceDominoCamConfig_P64SC,
			EDeviceDominoCamConfig_P70RA,
			EDeviceDominoCamConfig_P70RM,
			EDeviceDominoCamConfig_P70SA,
			EDeviceDominoCamConfig_P70SM,
			EDeviceDominoCamConfig_P74RG,
			EDeviceDominoCamConfig_P74SC,
			EDeviceDominoCamConfig_P75RG,
			EDeviceDominoCamConfig_P75SC,
			EDeviceDominoCamConfig_P80RG,
			EDeviceDominoCamConfig_P80SC,
			EDeviceDominoCamConfig_P8RC,
			EDeviceDominoCamConfig_P8RG,
			EDeviceDominoCamConfig_P8SC,
			EDeviceDominoCamConfig_P94RG,
			EDeviceDominoCamConfig_P94SC,
			EDeviceDominoCamConfig_PxxRA,
			EDeviceDominoCamConfig_PxxRA_D,
			EDeviceDominoCamConfig_PxxRA_M,
			EDeviceDominoCamConfig_PxxRC,
			EDeviceDominoCamConfig_PxxRD,
			EDeviceDominoCamConfig_PxxRG,
			EDeviceDominoCamConfig_PxxRG_PartialScan,
			EDeviceDominoCamConfig_PxxRM_1,
			EDeviceDominoCamConfig_PxxRM_2,
			EDeviceDominoCamConfig_PxxRM_A,
			EDeviceDominoCamConfig_PxxRM_D,
			EDeviceDominoCamConfig_PxxSA,
			EDeviceDominoCamConfig_PxxSA_D,
			EDeviceDominoCamConfig_PxxSC,
			EDeviceDominoCamConfig_PxxSD,
			EDeviceDominoCamConfig_PxxSM,
			EDeviceDominoCamConfig_PxxSM_L,
			EDeviceDominoCamConfig_STD,
			EDeviceDominoCamConfig_TEST,
			EDeviceDominoCamConfig_Count,
		};

		enum EDeviceDominoCamera
		{
			EDeviceDominoCamera_1000m = 0,
			EDeviceDominoCamera_1000m_D,
			EDeviceDominoCamera_1000m_S,
			EDeviceDominoCamera_1600c,
			EDeviceDominoCamera_1600c_D,
			EDeviceDominoCamera_1600c_D_RGB,
			EDeviceDominoCamera_1600c_S,
			EDeviceDominoCamera_1600c_S_RGB,
			EDeviceDominoCamera_1600m,
			EDeviceDominoCamera_1600m_D,
			EDeviceDominoCamera_1600m_S,
			EDeviceDominoCamera_1M28_SA,
			EDeviceDominoCamera_2000c_D,
			EDeviceDominoCamera_2000m,
			EDeviceDominoCamera_2000m_D,
			EDeviceDominoCamera_2000m_S,
			EDeviceDominoCamera_2M30_SA,
			EDeviceDominoCamera_4000m,
			EDeviceDominoCamera_4000m_D,
			EDeviceDominoCamera_4000m_S,
			EDeviceDominoCamera_A102k,
			EDeviceDominoCamera_A201b,
			EDeviceDominoCamera_A202k,
			EDeviceDominoCamera_A301bc,
			EDeviceDominoCamera_A302b,
			EDeviceDominoCamera_A402k,
			EDeviceDominoCamera_A403k,
			EDeviceDominoCamera_A501k,
			EDeviceDominoCamera_AVIIVA,
			EDeviceDominoCamera_AViiVA_C2CL4010,
			EDeviceDominoCamera_AViiVA_M4CL6007,
			EDeviceDominoCamera_AViiVA_M4CL8007,
			EDeviceDominoCamera_AVIIVA0514,
			EDeviceDominoCamera_AVIIVA1014,
			EDeviceDominoCamera_AVIIVA2014,
			EDeviceDominoCamera_AVIIVA4010,
			EDeviceDominoCamera_CAMERA_CCIR,
			EDeviceDominoCamera_CAMERA_EIA,
			EDeviceDominoCamera_CAMERA_NTSC,
			EDeviceDominoCamera_CAMERA_PAL,
			EDeviceDominoCamera_CAMERA_sCCIR,
			EDeviceDominoCamera_CAMERA_sEIA,
			EDeviceDominoCamera_CAMERA_sNTSC,
			EDeviceDominoCamera_CAMERA_sPAL,
			EDeviceDominoCamera_CameraExposeAR,
			EDeviceDominoCamera_CCIR,
			EDeviceDominoCamera_COLIBRI1024CL,
			EDeviceDominoCamera_COLIBRI2048CL,
			EDeviceDominoCamera_CS3920,
			EDeviceDominoCamera_CS6910CL,
			EDeviceDominoCamera_CS8530,
			EDeviceDominoCamera_CS8531,
			EDeviceDominoCamera_CS8531D,
			EDeviceDominoCamera_CS8541D,
			EDeviceDominoCamera_CS8550D,
			EDeviceDominoCamera_CS8550i,
			EDeviceDominoCamera_CS8550i_01,
			EDeviceDominoCamera_CSB1100CL,
			EDeviceDominoCamera_CSB4000CL,
			EDeviceDominoCamera_CUSTOM,
			EDeviceDominoCamera_CV_A1,
			EDeviceDominoCamera_CV_A11,
			EDeviceDominoCamera_CV_A33,
			EDeviceDominoCamera_CV_A50C,
			EDeviceDominoCamera_CV_A50E,
			EDeviceDominoCamera_CV_M1,
			EDeviceDominoCamera_CV_M10C,
			EDeviceDominoCamera_CV_M10E,
			EDeviceDominoCamera_CV_M30,
			EDeviceDominoCamera_CV_M40,
			EDeviceDominoCamera_CV_M4CL,
			EDeviceDominoCamera_CV_M4CLp,
			EDeviceDominoCamera_CV_M50,
			EDeviceDominoCamera_CV_M50C,
			EDeviceDominoCamera_CV_M50E,
			EDeviceDominoCamera_CV_M77,
			EDeviceDominoCamera_CV_M7CL,
			EDeviceDominoCamera_CV_M7CLp,
			EDeviceDominoCamera_CV_M8CL,
			EDeviceDominoCamera_CV_M9CL,
			EDeviceDominoCamera_DT4100,
			EDeviceDominoCamera_EIA,
			EDeviceDominoCamera_Generic,
			EDeviceDominoCamera_Generic_Dual,
			EDeviceDominoCamera_Generic_RGB,
			EDeviceDominoCamera_GrabberExposeAR,
			EDeviceDominoCamera_HV_C20AEK,
			EDeviceDominoCamera_HV_C20AP,
			EDeviceDominoCamera_ICD_FA41,
			EDeviceDominoCamera_ICD_FA41E,
			EDeviceDominoCamera_IK_TF1,
			EDeviceDominoCamera_IK_TF5,
			EDeviceDominoCamera_IK_TF7,
			EDeviceDominoCamera_iMVS157,
			EDeviceDominoCamera_InterlaceFR,
			EDeviceDominoCamera_InterlaceRR,
			EDeviceDominoCamera_KP_F100BCL,
			EDeviceDominoCamera_KP_F120CL,
			EDeviceDominoCamera_KP_F200CL,
			EDeviceDominoCamera_KP_F2B,
			EDeviceDominoCamera_KP_F3,
			EDeviceDominoCamera_KP_F30,
			EDeviceDominoCamera_KP_F30x,
			EDeviceDominoCamera_KP_F3W,
			EDeviceDominoCamera_KP_M32P,
			EDeviceDominoCamera_KP120CL,
			EDeviceDominoCamera_L101k,
			EDeviceDominoCamera_L103k,
			EDeviceDominoCamera_L104k,
			EDeviceDominoCamera_L301bc,
			EDeviceDominoCamera_L301kc,
			EDeviceDominoCamera_L401k,
			EDeviceDominoCamera_L402k,
			EDeviceDominoCamera_L403k,
			EDeviceDominoCamera_LD3521PGx,
			EDeviceDominoCamera_LD3522PGx,
			EDeviceDominoCamera_LD3523PGx,
			EDeviceDominoCamera_LD3541PGx,
			EDeviceDominoCamera_LD3542PGx,
			EDeviceDominoCamera_LD3543PGx,
			EDeviceDominoCamera_LongExposeFR,
			EDeviceDominoCamera_MV_D1024,
			EDeviceDominoCamera_MV_D1024_160_CL_8,
			EDeviceDominoCamera_MV_D1024_80_CL_8,
			EDeviceDominoCamera_MV_D640,
			EDeviceDominoCamera_MV_D640_33_CL_10,
			EDeviceDominoCamera_MV_D640_66_CL_10,
			EDeviceDominoCamera_MV_D640C_33_CL_10,
			EDeviceDominoCamera_MV_D640C_66_CL_10,
			EDeviceDominoCamera_MyCameraLink,
			EDeviceDominoCamera_MyColorScanCameraLink,
			EDeviceDominoCamera_NoCamera,
			EDeviceDominoCamera_NSUF7400CL,
			EDeviceDominoCamera_NTSC,
			EDeviceDominoCamera_NUCL4096D,
			EDeviceDominoCamera_NUCL7500D,
			EDeviceDominoCamera_P2_2x_01k40,
			EDeviceDominoCamera_P2_2x_02k40,
			EDeviceDominoCamera_P2_2x_04k40,
			EDeviceDominoCamera_P2_2x_06k40,
			EDeviceDominoCamera_P2_2x_08k40,
			EDeviceDominoCamera_P2_4x_04k40,
			EDeviceDominoCamera_P2_4x_06k40,
			EDeviceDominoCamera_P2_4x_08k40,
			EDeviceDominoCamera_PAL,
			EDeviceDominoCamera_PC_640CL,
			EDeviceDominoCamera_PRICOLOR1024C,
			EDeviceDominoCamera_PRICOLOR2048C,
			EDeviceDominoCamera_ProgressiveFR,
			EDeviceDominoCamera_SI_M100,
			EDeviceDominoCamera_SIMA,
			EDeviceDominoCamera_SIMUL,
			EDeviceDominoCamera_SKC_131,
			EDeviceDominoCamera_SKC_141,
			EDeviceDominoCamera_SKC_141TC,
			EDeviceDominoCamera_SKCm141TC,
			EDeviceDominoCamera_STC_1000,
			EDeviceDominoCamera_STC_1100b,
			EDeviceDominoCamera_Std_BW_CCIR,
			EDeviceDominoCamera_Std_BW_EIA,
			EDeviceDominoCamera_Std_RGB_CCIR,
			EDeviceDominoCamera_Std_RGB_EIA,
			EDeviceDominoCamera_SU51,
			EDeviceDominoCamera_SU74,
			EDeviceDominoCamera_SVS084MFCL,
			EDeviceDominoCamera_SVS084MSCL,
			EDeviceDominoCamera_SVS085MFCL,
			EDeviceDominoCamera_SVS085MSCL,
			EDeviceDominoCamera_SVS204MFCL,
			EDeviceDominoCamera_SVS204MSCL,
			EDeviceDominoCamera_SVS282CSCL,
			EDeviceDominoCamera_SVS285MFCL,
			EDeviceDominoCamera_SVS285MSCL,
			EDeviceDominoCamera_TI_324A,
			EDeviceDominoCamera_TM_1020_15CL,
			EDeviceDominoCamera_TM_1320_15CL,
			EDeviceDominoCamera_TM_2016_8CL,
			EDeviceDominoCamera_TM_4000CL,
			EDeviceDominoCamera_TM_6702,
			EDeviceDominoCamera_TM_6710CL,
			EDeviceDominoCamera_TM_9701,
			EDeviceDominoCamera_TMC_1000,
			EDeviceDominoCamera_TMC_4000CL,
			EDeviceDominoCamera_TMC_6700,
			EDeviceDominoCamera_VCC_810,
			EDeviceDominoCamera_VCC_8350,
			EDeviceDominoCamera_VCC_8350CL,
			EDeviceDominoCamera_VCC_850A,
			EDeviceDominoCamera_VCC_8550,
			EDeviceDominoCamera_VCC_8550CL,
			EDeviceDominoCamera_VCC_870,
			EDeviceDominoCamera_VCC_870A,
			EDeviceDominoCamera_VCC_8750CL,
			EDeviceDominoCamera_VCC_8850CL,
			EDeviceDominoCamera_VCC_G22V31CL_PoCL,
			EDeviceDominoCamera_XC_003,
			EDeviceDominoCamera_XC_55,
			EDeviceDominoCamera_XC_56,
			EDeviceDominoCamera_XC_7500,
			EDeviceDominoCamera_XC_8500CE,
			EDeviceDominoCamera_XC_ES30,
			EDeviceDominoCamera_XC_ES30CE,
			EDeviceDominoCamera_XC_HR300,
			EDeviceDominoCamera_XC_HR50,
			EDeviceDominoCamera_XC_HR57,
			EDeviceDominoCamera_XC_HR58,
			EDeviceDominoCamera_XC_HR70,
			EDeviceDominoCamera_XC_ST50,
			EDeviceDominoCamera_XC_ST50CE,
			EDeviceDominoCamera_XC_ST70CE,
			EDeviceDominoCamera_XCL_U1000,
			EDeviceDominoCamera_XCL_U1000C,
			EDeviceDominoCamera_XCL_V500,
			EDeviceDominoCamera_XCL_X700,
			EDeviceDominoCamera_XCLmU1000,
			EDeviceDominoCamera_XCLmU1000C,
			EDeviceDominoCamera_XCLmV500,
			EDeviceDominoCamera_XCLmX700,
			EDeviceDominoCamera_YD5040,
			EDeviceDominoCamera_YD5060,
			EDeviceDominoCamera_Count,
		};

		enum EDeviceDominoDataLink
		{
			EDeviceDominoDataLink_ANALOG = 0,
			EDeviceDominoDataLink_CAMERALINK,
			EDeviceDominoDataLink_COMPOSITE,
			EDeviceDominoDataLink_Count,
		};

		enum EDeviceDominoImaging
		{
			EDeviceDominoImaging_AREA = 0,
			EDeviceDominoImaging_LINE,
			EDeviceDominoImaging_TDI,
			EDeviceDominoImaging_Count,
		};

		enum EDeviceDominoSpectrum
		{
			EDeviceDominoSpectrum_BW = 0,
			EDeviceDominoSpectrum_COLOR,
			EDeviceDominoSpectrum_IR,
			EDeviceDominoSpectrum_Count,
		};

		enum EDeviceDominoAuxResetCtl
		{
			EDeviceDominoAuxResetCtl_DIFF = 0,
			EDeviceDominoAuxResetCtl_NONE,
			EDeviceDominoAuxResetCtl_TTL,
			EDeviceDominoAuxResetCtl_Count,
		};

		enum EDeviceDominoColorMethod
		{
			EDeviceDominoColorMethod_BAYER = 0,
			EDeviceDominoColorMethod_NONE,
			EDeviceDominoColorMethod_PRISM,
			EDeviceDominoColorMethod_RGB,
			EDeviceDominoColorMethod_TRILINEAR,
			EDeviceDominoColorMethod_Count,
		};

		enum EDeviceDominoExposeOverlap
		{
			EDeviceDominoExposeOverlap_ALLOW = 0,
			EDeviceDominoExposeOverlap_FORBID,
			EDeviceDominoExposeOverlap_Count,
		};

		enum EDeviceDominoReadout
		{
			EDeviceDominoReadout_100ns = 0,
			EDeviceDominoReadout_INTCTL,
			EDeviceDominoReadout_PLSTRG,
			EDeviceDominoReadout_us,
			EDeviceDominoReadout_WIDTH,
			EDeviceDominoReadout_Count,
		};

		enum EDeviceDominoResetCtl
		{
			EDeviceDominoResetCtl_DIFF = 0,
			EDeviceDominoResetCtl_NONE,
			EDeviceDominoResetCtl_TTL,
			EDeviceDominoResetCtl_Count,
		};

		enum EDeviceDominoResetEdge
		{
			EDeviceDominoResetEdge_GOHIGH = 0,
			EDeviceDominoResetEdge_GOLOW,
			EDeviceDominoResetEdge_Count,
		};

		enum EDeviceDominoAuxresetLine
		{
			EDeviceDominoAuxresetLine_CC1 = 0,
			EDeviceDominoAuxresetLine_CC2,
			EDeviceDominoAuxresetLine_CC3,
			EDeviceDominoAuxresetLine_CC4,
			EDeviceDominoAuxresetLine_CC5,
			EDeviceDominoAuxresetLine_DUAL,
			EDeviceDominoAuxresetLine_EXP,
			EDeviceDominoAuxresetLine_Feature,
			EDeviceDominoAuxresetLine_Feature_Available,
			EDeviceDominoAuxresetLine_Feature_NotAvailable,
			EDeviceDominoAuxresetLine_GATE,
			EDeviceDominoAuxresetLine_HIO,
			EDeviceDominoAuxresetLine_NC,
			EDeviceDominoAuxresetLine_RST,
			EDeviceDominoAuxresetLine_VIO,
			EDeviceDominoAuxresetLine_Count,
		};

		enum EDeviceDominoResetLine
		{
			EDeviceDominoResetLine_CC1 = 0,
			EDeviceDominoResetLine_CC2,
			EDeviceDominoResetLine_CC3,
			EDeviceDominoResetLine_CC4,
			EDeviceDominoResetLine_CC5,
			EDeviceDominoResetLine_DUAL,
			EDeviceDominoResetLine_EXP,
			EDeviceDominoResetLine_GATE,
			EDeviceDominoResetLine_HIO,
			EDeviceDominoResetLine_NC,
			EDeviceDominoResetLine_RST,
			EDeviceDominoResetLine_VIO,
			EDeviceDominoResetLine_Count,
		};

		enum EDeviceDominoBreakEffect
		{
			EDeviceDominoBreakEffect_ABORT = 0,
			EDeviceDominoBreakEffect_FINISH,
			EDeviceDominoBreakEffect_Count,
		};

		enum EDeviceDominoEndTrigMode
		{
			EDeviceDominoEndTrigMode_AUTO = 0,
			EDeviceDominoEndTrigMode_HARD,
			EDeviceDominoEndTrigMode_MASTER_CHANNEL,
			EDeviceDominoEndTrigMode_SLAVE,
			EDeviceDominoEndTrigMode_Count,
		};

		enum EDeviceDominoNextTrigMode
		{
			EDeviceDominoNextTrigMode_COMBINED = 0,
			EDeviceDominoNextTrigMode_HARD,
			EDeviceDominoNextTrigMode_IMMEDIATE,
			EDeviceDominoNextTrigMode_NextTrigMode,
			EDeviceDominoNextTrigMode_PAUSE,
			EDeviceDominoNextTrigMode_PERIODIC,
			EDeviceDominoNextTrigMode_REPEAT,
			EDeviceDominoNextTrigMode_SAME,
			EDeviceDominoNextTrigMode_SLAVE,
			EDeviceDominoNextTrigMode_SOFT,
			EDeviceDominoNextTrigMode_TIMER,
			EDeviceDominoNextTrigMode_Count,
		};

		enum EDeviceDominoForceTrig
		{
			EDeviceDominoForceTrig_ENDTRIG = 0,
			EDeviceDominoForceTrig_TRIG,
			EDeviceDominoForceTrig_Count,
		};

		enum EDeviceDominoStrobeCtl
		{
			EDeviceDominoStrobeCtl_DIFF = 0,
			EDeviceDominoStrobeCtl_IOC,
			EDeviceDominoStrobeCtl_IOE,
			EDeviceDominoStrobeCtl_ITTL,
			EDeviceDominoStrobeCtl_NONE,
			EDeviceDominoStrobeCtl_OPTO,
			EDeviceDominoStrobeCtl_SSRLY,
			EDeviceDominoStrobeCtl_TTL,
			EDeviceDominoStrobeCtl_Count,
		};

		enum EDeviceDominoStrobeMode
		{
			EDeviceDominoStrobeMode_AUTO = 0,
			EDeviceDominoStrobeMode_MAN,
			EDeviceDominoStrobeMode_NONE,
			EDeviceDominoStrobeMode_OFF,
			EDeviceDominoStrobeMode_Count,
		};

		enum EDeviceDominoGrabWindow
		{
			EDeviceDominoGrabWindow_FIT = 0,
			EDeviceDominoGrabWindow_MAN,
			EDeviceDominoGrabWindow_NOBLACK,
			EDeviceDominoGrabWindow_NOLOSS,
			EDeviceDominoGrabWindow_STD,
			EDeviceDominoGrabWindow_WOI,
			EDeviceDominoGrabWindow_WOI_NO,
			EDeviceDominoGrabWindow_WOI_YES,
			EDeviceDominoGrabWindow_Count,
		};

		enum EDeviceDominoColorFormat
		{
			EDeviceDominoColorFormat_ARGB32 = 0,
			EDeviceDominoColorFormat_ARGB64,
			EDeviceDominoColorFormat_BAYER10,
			EDeviceDominoColorFormat_BAYER12,
			EDeviceDominoColorFormat_BAYER14,
			EDeviceDominoColorFormat_BAYER16,
			EDeviceDominoColorFormat_BAYER8,
			EDeviceDominoColorFormat_BGR24,
			EDeviceDominoColorFormat_BGR32,
			EDeviceDominoColorFormat_DX50,
			EDeviceDominoColorFormat_FLOAT32,
			EDeviceDominoColorFormat_GRABBER,
			EDeviceDominoColorFormat_I420,
			EDeviceDominoColorFormat_IYU2,
			EDeviceDominoColorFormat_IYUV,
			EDeviceDominoColorFormat_JPEG,
			EDeviceDominoColorFormat_MJPG,
			EDeviceDominoColorFormat_MP4S,
			EDeviceDominoColorFormat_RAW10,
			EDeviceDominoColorFormat_RAW12,
			EDeviceDominoColorFormat_RAW14,
			EDeviceDominoColorFormat_RAW16,
			EDeviceDominoColorFormat_RAW8,
			EDeviceDominoColorFormat_RGB10_10_10,
			EDeviceDominoColorFormat_RGB10_12_10,
			EDeviceDominoColorFormat_RGB15,
			EDeviceDominoColorFormat_RGB15Y8,
			EDeviceDominoColorFormat_RGB16,
			EDeviceDominoColorFormat_RGB16Y8,
			EDeviceDominoColorFormat_RGB24,
			EDeviceDominoColorFormat_RGB24PL,
			EDeviceDominoColorFormat_RGB24PLY10,
			EDeviceDominoColorFormat_RGB24PLY12,
			EDeviceDominoColorFormat_RGB24PLY16,
			EDeviceDominoColorFormat_RGB24PLY8,
			EDeviceDominoColorFormat_RGB24Y10,
			EDeviceDominoColorFormat_RGB24Y12,
			EDeviceDominoColorFormat_RGB24Y16,
			EDeviceDominoColorFormat_RGB24Y8,
			EDeviceDominoColorFormat_RGB30,
			EDeviceDominoColorFormat_RGB30PL,
			EDeviceDominoColorFormat_RGB30PLY10,
			EDeviceDominoColorFormat_RGB30PLY12,
			EDeviceDominoColorFormat_RGB30PLY16,
			EDeviceDominoColorFormat_RGB30PLY8,
			EDeviceDominoColorFormat_RGB32,
			EDeviceDominoColorFormat_RGB36,
			EDeviceDominoColorFormat_RGB36PL,
			EDeviceDominoColorFormat_RGB36PLY10,
			EDeviceDominoColorFormat_RGB36PLY12,
			EDeviceDominoColorFormat_RGB36PLY16,
			EDeviceDominoColorFormat_RGB36PLY8,
			EDeviceDominoColorFormat_RGB42,
			EDeviceDominoColorFormat_RGB42PL,
			EDeviceDominoColorFormat_RGB48,
			EDeviceDominoColorFormat_RGB48PL,
			EDeviceDominoColorFormat_RGB48PLY10,
			EDeviceDominoColorFormat_RGB48PLY12,
			EDeviceDominoColorFormat_RGB48PLY16,
			EDeviceDominoColorFormat_RGB48PLY8,
			EDeviceDominoColorFormat_RGB64,
			EDeviceDominoColorFormat_UINT32,
			EDeviceDominoColorFormat_Y10,
			EDeviceDominoColorFormat_Y12,
			EDeviceDominoColorFormat_Y14,
			EDeviceDominoColorFormat_Y16,
			EDeviceDominoColorFormat_Y41B,
			EDeviceDominoColorFormat_Y41P,
			EDeviceDominoColorFormat_Y42B,
			EDeviceDominoColorFormat_Y42P,
			EDeviceDominoColorFormat_Y8,
			EDeviceDominoColorFormat_YRGB32,
			EDeviceDominoColorFormat_YRGB64,
			EDeviceDominoColorFormat_YUV411,
			EDeviceDominoColorFormat_YUV411PL,
			EDeviceDominoColorFormat_YUV411PL_DEC,
			EDeviceDominoColorFormat_YUV422,
			EDeviceDominoColorFormat_YUV422PL,
			EDeviceDominoColorFormat_YUV422PL_DEC,
			EDeviceDominoColorFormat_YUV444,
			EDeviceDominoColorFormat_YUV444PL,
			EDeviceDominoColorFormat_YUV9,
			EDeviceDominoColorFormat_YV12,
			EDeviceDominoColorFormat_YVU9,
			EDeviceDominoColorFormat_Count,
		};

		enum EDeviceDominoImageFlipX
		{
			EDeviceDominoImageFlipX_OFF = 0,
			EDeviceDominoImageFlipX_ON,
			EDeviceDominoImageFlipX_Count,
		};

		enum EDeviceDominoImageFlipY
		{
			EDeviceDominoImageFlipY_OFF = 0,
			EDeviceDominoImageFlipY_ON,
			EDeviceDominoImageFlipY_Count,
		};

		enum EDeviceDominoChannelState
		{
			EDeviceDominoChannelState_ACTIVE = 0,
			EDeviceDominoChannelState_FREE,
			EDeviceDominoChannelState_IDLE,
			EDeviceDominoChannelState_ORPHAN,
			EDeviceDominoChannelState_READY,
			EDeviceDominoChannelState_Count,
		};

		enum EDeviceDominoAcquisitionCleanup
		{
			EDeviceDominoAcquisitionCleanup_DISABLED = 0,
			EDeviceDominoAcquisitionCleanup_ENABLED,
			EDeviceDominoAcquisitionCleanup_Count,
		};

		enum EDeviceDominoParamsConsistencyCheck
		{
			EDeviceDominoParamsConsistencyCheck_AUTO = 0,
			EDeviceDominoParamsConsistencyCheck_MANUAL,
			EDeviceDominoParamsConsistencyCheck_Count,
		};


		enum EDeviceDominoExpose
		{
			EDeviceDominoExpose_EXTPRM = 0,
			EDeviceDominoExpose_Fld,
			EDeviceDominoExpose_INTCTL,
			EDeviceDominoExpose_INTPRM,
			EDeviceDominoExpose_P1_Effective_us,
			EDeviceDominoExpose_P2_Effective_us,
			EDeviceDominoExpose_PLSCTL,
			EDeviceDominoExpose_PLSTRG,
			EDeviceDominoExpose_Tk,
			EDeviceDominoExpose_us,
			EDeviceDominoExpose_WIDTH,
			EDeviceDominoExpose_WIDTH_DX,
			EDeviceDominoExpose_Count,
		};
	}
}

// Euresys - Domino - Hamony
namespace Ravid
{
	namespace Device
	{
		enum EDeviceDominoHarmonyTopology
		{
			EDeviceDominoHarmonyTopology_1_1 = 0,
			EDeviceDominoHarmonyTopology_3_0,
			EDeviceDominoHarmonyTopology_Count,
		};

		enum EDeviceDominoHarmonyConnector
		{
			EDeviceDominoHarmonyConnector_X = 0,
			EDeviceDominoHarmonyConnector_Y,
			EDeviceDominoHarmonyConnector_Count,
		};
	}
}

// Euresys - Domino - Melody
namespace Ravid
{
	namespace Device
	{
		enum EDeviceDominoMelodyTopology
		{
			EDeviceDominoMelodyTopology_1 = 0,
			EDeviceDominoMelodyTopology_Count,
		};

		enum EDeviceDominoMelodyConnector
		{
			EDeviceDominoMelodyConnector_X = 0,
			EDeviceDominoMelodyConnector_Count,
		};
	}
}

// Euresys - Domino - Symphony
namespace Ravid
{
	namespace Device
	{
		enum EDeviceDominoSymphonyTopology
		{
			EDeviceDominoSymphonyTopology_1_1_1_1 = 0,
			EDeviceDominoSymphonyTopology_Count,
		};

		enum EDeviceDominoSymphonyConnector
		{
			EDeviceDominoSymphonyConnector_A = 0,
			EDeviceDominoSymphonyConnector_B,
			EDeviceDominoSymphonyConnector_C,
			EDeviceDominoSymphonyConnector_D,
			EDeviceDominoSymphonyConnector_Count,
		};
	}
}

// MVTech Trigger
namespace Ravid
{
	namespace Device
	{
		enum EMVTTriggerGetFunction
		{
			EMVTTriggerGetFunction_OK = 0,
			EMVTTriggerGetFunction_UnknownError,
			EMVTTriggerGetFunction_EnumTypeError,
			EMVTTriggerGetFunction_NullptrError,
		};

		enum EMVTTriggerSetFunction
		{
			EMVTTriggerSetFunction_OK = 0,
			EMVTTriggerSetFunction_UnknownError,
			EMVTTriggerSetFunction_NotFoundApiError,
			EMVTTriggerSetFunction_AlreadyInitializedError,
			EMVTTriggerSetFunction_NotInitializedError,
			EMVTTriggerSetFunction_NotFindCommandError, // _CommandDoesNotExist
			EMVTTriggerSetFunction_DeviceNullptrError,
			EMVTTriggerSetFunction_NotSupportError,
			EMVTTriggerSetFunction_FunctionLockedError,
			EMVTTriggerSetFunction_WriteToDeviceError,
			EMVTTriggerSetFunction_WriteToDatabaseError,
			EMVTTriggerSetFunction_ReadOnDatabaseError,
			EMVTTriggerSetFunction_ActivatedDeviceError,
		};

		enum EDeviceMVTTriggerEncoderIncreasementDirection
		{
			EDeviceMVTTriggerEncoderIncreasementDirection_CW = 0,
			EDeviceMVTTriggerEncoderIncreasementDirection_CCW,
		};

		enum EDeviceMVTTriggerTriggerCountingDirection
		{
			EDeviceMVTTriggerTriggerCountingDirection_CW = 0,
			EDeviceMVTTriggerTriggerCountingDirection_CCW,
		};
	}
}

// Motion
namespace Ravid
{
	namespace Device
	{
		enum EDeviceMotionHommingStatus
		{
			EDeviceMotionHommingStatus_Success = 0,
			EDeviceMotionHommingStatus_Searching,
			EDeviceMotionHommingStatus_Error,
			EDeviceMotionHommingStatus_Count,
		};

		enum EDeviceMotionTriggerLevel
		{
			EDeviceMotionTriggerLevel_ActiveLow = 0,
			EDeviceMotionTriggerLevel_ActiveHigh,
		};

		enum EDeviceMotionTriggerMode
		{
			EDeviceMotionTriggerMode_Absolute = 0,
			EDeviceMotionTriggerMode_Period,
		};

		enum EDeviceMotionTriggerSource
		{
			EDeviceMotionTriggerSource_ActualPosition = 0,
			EDeviceMotionTriggerSource_CommandPosition,
		};
	}
}

// DalsaGenieNonoGigE
namespace Ravid
{
	namespace Device
	{
		enum EDalsaGenieNanoGigEGetFunction
		{
			EDalsaGenieNanoGigEGetFunction_OK = 0,
			EDalsaGenieNanoGigEGetFunction_UnknownError,
			EDalsaGenieNanoGigEGetFunction_NullptrError,
			EDalsaGenieNanoGigEGetFunction_EnumTypeError,
		};

		enum EDalsaGenieNanoGigESetFunction
		{
			EDalsaGenieNanoGigESetFunction_OK = 0,
			EDalsaGenieNanoGigESetFunction_UnknownError,
			EDalsaGenieNanoGigESetFunction_InitializedDeviceError,
			EDalsaGenieNanoGigESetFunction_ActiveDeviceError,
			EDalsaGenieNanoGigESetFunction_WriteToDatabaseError,
			EDalsaGenieNanoGigESetFunction_NotSupportedError,
			EDalsaGenieNanoGigESetFunction_WriteToDeviceError,
			EDalsaGenieNanoGigESetFunction_NotFindCommandError,
			EDalsaGenieNanoGigESetFunction_NotSelectFeatureError,
			EDalsaGenieNanoGigESetFunction_ReadOnDeviceError,
			EDalsaGenieNanoGigESetFunction_ReadOnDatabaseError,
		};

		enum EDalsaGenieNanoGigEPixelFormat
		{
			EDalsaGenieNanoGigEPixelFormat_Mono8 = 0,
			EDalsaGenieNanoGigEPixelFormat_Mono8Signed,
			EDalsaGenieNanoGigEPixelFormat_Mono10,
			EDalsaGenieNanoGigEPixelFormat_Mono12,
			EDalsaGenieNanoGigEPixelFormat_Mono14,
			EDalsaGenieNanoGigEPixelFormat_Mono16,
			EDalsaGenieNanoGigEPixelFormat_BayerRG8,
			EDalsaGenieNanoGigEPixelFormat_BayerRG10,
			EDalsaGenieNanoGigEPixelFormat_BayerRG12,
			EDalsaGenieNanoGigEPixelFormat_BayerRG16,
			EDalsaGenieNanoGigEPixelFormat_BayerGB8,
			EDalsaGenieNanoGigEPixelFormat_BayerGB10,
			EDalsaGenieNanoGigEPixelFormat_BayerGB12,
			EDalsaGenieNanoGigEPixelFormat_BayerGB16,
			EDalsaGenieNanoGigEPixelFormat_BayerGR8,
			EDalsaGenieNanoGigEPixelFormat_BayerGR10,
			EDalsaGenieNanoGigEPixelFormat_BayerGR12,
			EDalsaGenieNanoGigEPixelFormat_BayerGR16,
			EDalsaGenieNanoGigEPixelFormat_BayerBG8,
			EDalsaGenieNanoGigEPixelFormat_BayerBG10,
			EDalsaGenieNanoGigEPixelFormat_BayerBG12,
			EDalsaGenieNanoGigEPixelFormat_BayerBG16,
			EDalsaGenieNanoGigEPixelFormat_BGR8Packed,
			EDalsaGenieNanoGigEPixelFormat_BGRA8Packed,
			EDalsaGenieNanoGigEPixelFormat_BGR10Packed,
			EDalsaGenieNanoGigEPixelFormat_BGR12Packed,
			EDalsaGenieNanoGigEPixelFormat_YUV411Packed,
			EDalsaGenieNanoGigEPixelFormat_YUV422_YUYV_Packed,
			EDalsaGenieNanoGigEPixelFormat_YUV422Packed,
			EDalsaGenieNanoGigEPixelFormat_Count,
		};

		enum EDalsaGenieNanoGigETriggerSelector
		{
			EDalsaGenieNanoGigETriggerSelector_FrameStart = 0,
			EDalsaGenieNanoGigETriggerSelector_FrameBurstStart,
			EDalsaGenieNanoGigETriggerSelector_AcquisitionStart,
			EDalsaGenieNanoGigETriggerSelector_Count,
		};

		enum EDalsaGenieNanoGigESwitch
		{
			EDalsaGenieNanoGigESwitch_Off = 0,
			EDalsaGenieNanoGigESwitch_On,
			EDalsaGenieNanoGigESwitch_Count,
		};

		enum EDalsaGenieNanoGigESource
		{
			EDalsaGenieNanoGigESource_Software = 0,
			EDalsaGenieNanoGigESource_Line1,
			EDalsaGenieNanoGigESource_Line2,
			EDalsaGenieNanoGigESource_Action1,
			EDalsaGenieNanoGigESource_Action2,
			EDalsaGenieNanoGigESource_Count,
		};

		enum EDalsaGenieNanoGigEActivation
		{
			EDalsaGenieNanoGigEActivation_RisingEdge = 0,
			EDalsaGenieNanoGigEActivation_FallingEdge,
			EDalsaGenieNanoGigEActivation_AnyEdge,
			EDalsaGenieNanoGigEActivation_Count,
		};

		enum EDalsaGenieNanoGigEFrameRateControl
		{
			EDalsaGenieNanoGigEFrameRateControl_Programmable = 0,
			EDalsaGenieNanoGigEFrameRateControl_MaximumSpeed,
			EDalsaGenieNanoGigEFrameRateControl_Count,
		};


		enum EDalsaGenieNanoGigEGainSelector
		{
			EDalsaGenieNanoGigEGainSelector_SensorAll = 0,
			EDalsaGenieNanoGigEGainSelector_DigitalAll,
			EDalsaGenieNanoGigEGainSelector_Count,
		};

		enum EDalsaGenieNanoGigEBlackLevelSelector
		{
			EDalsaGenieNanoGigEBlackLevelSelector_Analog = 0,
			EDalsaGenieNanoGigEBlackLevelSelector_Count,
		};

		enum EDalsaGenieNanoGigELineSelector
		{
			EDalsaGenieNanoGigELineSelector_Line1 = 0,
			EDalsaGenieNanoGigELineSelector_Line2,
			EDalsaGenieNanoGigELineSelector_Line3,
			EDalsaGenieNanoGigELineSelector_Line4,
			EDalsaGenieNanoGigELineSelector_Count,
		};

		enum EDalsaGenieNanoGigELineSource
		{
			EDalsaGenieNanoGigELineSource_Off = 0,
			EDalsaGenieNanoGigELineSource_SoftwareControlled,
			EDalsaGenieNanoGigELineSource_StartOfFrame,
			EDalsaGenieNanoGigELineSource_StartOfExposure,
			EDalsaGenieNanoGigELineSource_EndOfExposure,
			EDalsaGenieNanoGigELineSource_StartOfReadout,
			EDalsaGenieNanoGigELineSource_EndOfReadout,
			EDalsaGenieNanoGigELineSource_ValidFrameTrigger,
			EDalsaGenieNanoGigELineSource_RejectedFrameTrigger,
			EDalsaGenieNanoGigELineSource_StartOfAcquisition,
			EDalsaGenieNanoGigELineSource_EndOfAcquisition,
			EDalsaGenieNanoGigELineSource_EndOfTimer1,
			EDalsaGenieNanoGigELineSource_EndOfCounter1,
			EDalsaGenieNanoGigELineSource_Input1Event,
			EDalsaGenieNanoGigELineSource_Input2Event,
			EDalsaGenieNanoGigELineSource_Action1,
			EDalsaGenieNanoGigELineSource_Action2,
			EDalsaGenieNanoGigELineSource_SoftwareCommand,
			EDalsaGenieNanoGigELineSource_ExposureActive,
			EDalsaGenieNanoGigELineSource_Count,
		};

		enum EDalsaGenieNanoGigELineValue
		{
			EDalsaGenieNanoGigELineValue_Inactive = 0,
			EDalsaGenieNanoGigELineValue_Active,
			EDalsaGenieNanoGigELineValue_Count,
		};
	}
}

namespace Ravid
{
	namespace Device
	{
		enum EDahuaGigEGetFunction
		{
			EDahuaGigEGetFunction_OK = 0,
			EDahuaGigEGetFunction_UnknownError,
			EDahuaGigEGetFunction_EnumTypeError,
			EDahuaGigEGetFunction_NullptrError,
		};

		enum EDahuaGigESetFunction
		{
			EDahuaGigESetFunction_OK = 0,
			EDahuaGigESetFunction_UnknownError,
			EDahuaGigESetFunction_AlreadyGrabError,
			EDahuaGigESetFunction_InitializedDeviceError,
			EDahuaGigESetFunction_WriteToDatabaseError,
			EDahuaGigESetFunction_NotSupportedError,
			EDahuaGigESetFunction_WriteToDeviceError,
			EDahuaGigESetFunction_NotFindCommandError,
			EDahuaGigESetFunction_NotSelectFeatureError,
			EDahuaGigESetFunction_ReadOnDeviceError,
			EDahuaGigESetFunction_ReadOnDatabaseError,
		};
	}
}

// DahuaGigE
namespace Ravid
{
	namespace Device
	{
		enum EDeviceDahuaGigEPixelFormat
		{
			EDeviceDahuaGigEPixelFormat_Mono8 = 0,
			EDeviceDahuaGigEPixelFormat_Mono10,
			EDeviceDahuaGigEPixelFormat_Mono10Packed,
			EDeviceDahuaGigEPixelFormat_Count,
		};

		enum EDeviceDahuaGigEAcquisitionMode
		{
			EDeviceDahuaGigEAcquisitionMode_Continuous = 0,
			EDeviceDahuaGigEAcquisitionMode_SingleFrame,
			EDeviceDahuaGigEAcquisitionMode_MultiFrame,
			EDeviceDahuaGigEAcquisitionMode_Count,
		};

		enum EDeviceDahuaGigEAcquisitionSelector
		{
			EDeviceDahuaGigEAcquisitionSelector_AcquisitionTriggerWait = 0,
			EDeviceDahuaGigEAcquisitionSelector_FrameTriggerWait,
			EDeviceDahuaGigEAcquisitionSelector_Count,
		};

		enum EDeviceDahuaGigETriggerSelector
		{
			EDeviceDahuaGigETriggerSelector_AcquisitionStart = 0,
			EDeviceDahuaGigETriggerSelector_FrameStart,
			EDeviceDahuaGigETriggerSelector_Count,
		};

		enum EDeviceDahuaGigETriggerMode
		{
			EDeviceDahuaGigETriggerMode_Off = 0,
			EDeviceDahuaGigETriggerMode_On,
			EDeviceDahuaGigETriggerMode_Count,
		};

		enum EDeviceDahuaGigETriggerSource
		{
			EDeviceDahuaGigETriggerSource_Software = 0,
			EDeviceDahuaGigETriggerSource_Line1,
			EDeviceDahuaGigETriggerSource_Count,
		};

		enum EDeviceDahuaGigETriggerActivation
		{
			EDeviceDahuaGigETriggerActivation_RisingEdge = 0,
			EDeviceDahuaGigETriggerActivation_FallingEdge,
			EDeviceDahuaGigETriggerActivation_Count,
		};

		enum EDeviceDahuaGigEExposureMode
		{
			EDeviceDahuaGigEExposureMode_Timed = 0,
			EDeviceDahuaGigEExposureMode_Count,
		};

		enum EDeviceDahuaGigEExposureAuto
		{
			EDeviceDahuaGigEExposureAuto_Off = 0,
			EDeviceDahuaGigEExposureAuto_Once,
			EDeviceDahuaGigEExposureAuto_Continuous,
			EDeviceDahuaGigEExposureAuto_Count,
		};

		enum EDeviceDahuaGigEGainSelector
		{
			EDeviceDahuaGigEGainSelector_All = 0,
			EDeviceDahuaGigEGainSelector_Count,
		};

		enum EDeviceDahuaGigEBlackLevelSelector
		{
			EDeviceDahuaGigEBlackLevelSelector_All = 0,
			EDeviceDahuaGigEBlackLevelSelector_Count,
		};

		enum EDeviceDahuaGigEBlackLevelAuto
		{
			EDeviceDahuaGigEBlackLevelAuto_Off = 0,
			EDeviceDahuaGigEBlackLevelAuto_Once,
			EDeviceDahuaGigEBlackLevelAuto_Continuous,
			EDeviceDahuaGigEBlackLevelAuto_Count,
		};

		enum EDeviceDahuaGigELineSelector
		{
			EDeviceDahuaGigELineSelector_Line0 = 0,
			EDeviceDahuaGigELineSelector_Line1,
			EDeviceDahuaGigELineSelector_Count,
		};

		enum EDeviceDahuaGigELineInverter
		{
			EDeviceDahuaGigELineInverter_False = 0,
			EDeviceDahuaGigELineInverter_True,
			EDeviceDahuaGigELineInverter_Count,
		};

		enum EDeviceDahuaGigELineSource
		{
			EDeviceDahuaGigELineSource_ExposureActive = 0,
			EDeviceDahuaGigELineSource_FrameTriggerWait,
			EDeviceDahuaGigELineSource_Timer0Active,
			EDeviceDahuaGigELineSource_UserOutput0,
			EDeviceDahuaGigELineSource_AcquisitionTriggerWait,
			EDeviceDahuaGigELineSource_LightTrigger,
			EDeviceDahuaGigELineSource_Count,
		};

		enum EDeviceDahuaGigELineFormat
		{
			EDeviceDahuaGigELineFormat_OptoCoupled = 0,
			EDeviceDahuaGigELineFormat_Count,
		};

		enum EDeviceDahuaGigESwitch
		{
			EDeviceDahuaGigESwitch_Off = 0,
			EDeviceDahuaGigESwitch_On,
			EDeviceDahuaGigESwitch_Count,
		};
	}
}

// AxlTriggerSIOCN2CH
namespace Ravid
{
	namespace Device
	{
		enum EAxlTriggerSIOCN2CHGetFunction
		{
			EAxlTriggerSIOCN2CHGetFunction_OK = 0,
			EAxlTriggerSIOCN2CHGetFunction_UnknownError,
			EAxlTriggerSIOCN2CHGetFunction_EnumTypeError,
			EAxlTriggerSIOCN2CHGetFunction_NullptrError,
		};

		enum EAxlTriggerSIOCN2CHSetFunction
		{
			EAxlTriggerSIOCN2CHSetFunction_OK = 0,
			EAxlTriggerSIOCN2CHSetFunction_UnknownError,
			EAxlTriggerSIOCN2CHSetFunction_NotFoundApiError,
			EAxlTriggerSIOCN2CHSetFunction_AlreadyInitializedError,
			EAxlTriggerSIOCN2CHSetFunction_NotInitializedError,
			EAxlTriggerSIOCN2CHSetFunction_NotFindCommandError, // _CommandDoesNotExist
			EAxlTriggerSIOCN2CHSetFunction_NotSelectModeError,
			EAxlTriggerSIOCN2CHSetFunction_DeviceNullptrError,
			EAxlTriggerSIOCN2CHSetFunction_NotSupportError,
			EAxlTriggerSIOCN2CHSetFunction_FunctionLockedError,
			EAxlTriggerSIOCN2CHSetFunction_WriteToDeviceError,
			EAxlTriggerSIOCN2CHSetFunction_WriteToDatabaseError,
			EAxlTriggerSIOCN2CHSetFunction_ReadOnDatabaseError,
			EAxlTriggerSIOCN2CHSetFunction_ReadOnDeviceError,
			EAxlTriggerSIOCN2CHSetFunction_ActivatedDeviceError,
		};

		enum EDeviceAxlTriggerSIOCN2CTriggerFunction
		{
			EDeviceAxlTriggerSIOCN2CTriggerFunction_PeriodAndAbsolute = 0,
			EDeviceAxlTriggerSIOCN2CTriggerFunction_Count,
		};

		enum EDeviceAxlTriggerSIOCN2CHEncoderInputMethod
		{
			EDeviceAxlTriggerSIOCN2CHEncoderInputMethod_UpDown = 0,
			EDeviceAxlTriggerSIOCN2CHEncoderInputMethod_Sqr1,
			EDeviceAxlTriggerSIOCN2CHEncoderInputMethod_Sqr2,
			EDeviceAxlTriggerSIOCN2CHEncoderInputMethod_Sqr4,
			EDeviceAxlTriggerSIOCN2CHEncoderInputMethod_Count,
		};

		enum EDeviceAxlTriggerSIOCN2CHEncoderSource
		{
			EDeviceAxlTriggerSIOCN2CHEncoderSource_AB = 0,
			EDeviceAxlTriggerSIOCN2CHEncoderSource_Z,
			EDeviceAxlTriggerSIOCN2CHEncoderSource_Count,
		};

		enum EDeviceAxlTriggerSIOCN2CHActiveLevel
		{
			EDeviceAxlTriggerSIOCN2CHActiveLevel_Low = 0,
			EDeviceAxlTriggerSIOCN2CHActiveLevel_High,
			EDeviceAxlTriggerSIOCN2CHActiveLevel_Count,
		};

		enum EDeviceAxlTriggerSIOCN2CHTriggerMode
		{
			EDeviceAxlTriggerSIOCN2CHTriggerMode_Absolute = 0,
			EDeviceAxlTriggerSIOCN2CHTriggerMode_Periodic,
			EDeviceAxlTriggerSIOCN2CHTriggerMode_Count,
		};

		enum EDeviceAxlTriggerSIOCN2CHTriggerDirectionCheck
		{
			EDeviceAxlTriggerSIOCN2CHTriggerDirectionCheck_Both = 0,
			EDeviceAxlTriggerSIOCN2CHTriggerDirectionCheck_CW,
			EDeviceAxlTriggerSIOCN2CHTriggerDirectionCheck_CCW,
			EDeviceAxlTriggerSIOCN2CHTriggerDirectionCheck_Count,
		};

		enum EDeviceAxlTriggerSIOCN2CHSwitch
		{
			EDeviceAxlTriggerSIOCN2CHSwitch_Off = 0,
			EDeviceAxlTriggerSIOCN2CHSwitch_On,
			EDeviceAxlTriggerSIOCN2CHSwitch_Count,
		};
	}
}

// matrox
namespace Ravid
{
	namespace Device
	{
		enum EMatroxGetFunction
		{
			EMatroxGetFunction_OK = 0,
			EMatroxGetFunction_UnknownError,
			EMatroxGetFunction_EnumTypeError,
			EMatroxGetFunction_NullptrError,
		};

		enum EMatroxSetFunction
		{
			EMatroxSetFunction_OK = 0,
			EMatroxSetFunction_UnknownError,
			EMatroxSetFunction_NotFoundApiError,
			EMatroxSetFunction_AlreadyInitializedError,
			EMatroxSetFunction_NotInitializedError,
			EMatroxSetFunction_NotFindCommandError, // _CommandDoesNotExist
			EMatroxSetFunction_DeviceNullptrError,
			EMatroxSetFunction_NotSupportError,
			EMatroxSetFunction_FunctionLockedError,
			EMatroxSetFunction_WriteToDeviceError,
			EMatroxSetFunction_WriteToDatabaseError,
			EMatroxSetFunction_ReadOnDatabaseError,
			EMatroxSetFunction_ActivatedDeviceError,
			EMatroxSetFunction_RangeOverError,
		};

		enum EDeviceMatrox_InitilizeType
		{
			EDeviceMatrox_InitilizeType_Camfile = 0,
			EDeviceMatrox_InitilizeType_Parameter,
			EDeviceMatrox_InitilizeType_Count,
		};

		enum EDeviceMatrox_CameraType
		{
			EDeviceMatrox_CameraType_FrameScan = 0,
			EDeviceMatrox_CameraType_LineScan,
			EDeviceMatrox_CameraType_Count,
		};

		enum EDeviceMatrox_NumberOfTaps
		{
			EDeviceMatrox_NumberOfTaps_1Tap = 0,
			EDeviceMatrox_NumberOfTaps_2Taps,
			EDeviceMatrox_NumberOfTaps_4Taps,
			EDeviceMatrox_NumberOfTaps_8Taps,
			EDeviceMatrox_NumberOfTaps_10Taps,
			EDeviceMatrox_NumberOfTaps_Count,
		};

		enum EDeviceMatrox_BayerMode
		{
			EDeviceMatrox_BayerMode_Disable = 0,
			EDeviceMatrox_BayerMode_Blue_Green,
			EDeviceMatrox_BayerMode_Green_Blue,
			EDeviceMatrox_BayerMode_Green_Red,
			EDeviceMatrox_BayerMode_Red_Green,
			EDeviceMatrox_BayerMode_Count,
		};

		enum EDeviceMatrox_ConfigurationType
		{
			EDeviceMatrox_ConfigurationType_Base = 0,
			EDeviceMatrox_ConfigurationType_Medium,
			EDeviceMatrox_ConfigurationType_Full,
			EDeviceMatrox_ConfigurationType_Count,
		};

		enum EDeviceMatrox_CameraTapsMode
		{
			EDeviceMatrox_CameraTapsMode_1Tap8_16Bits = 0,
			EDeviceMatrox_CameraTapsMode_2Taps8Bits,
			EDeviceMatrox_CameraTapsMode_2Taps10_12Bits,
			EDeviceMatrox_CameraTapsMode_2Taps14_16BitsTimeMultiplexed,
			EDeviceMatrox_CameraTapsMode_2Taps14_16Bits_ABCD,
			EDeviceMatrox_CameraTapsMode_2Taps14_16Bits_ABDE,
			EDeviceMatrox_CameraTapsMode_4Taps8BitsTimeMultiplexed,
			EDeviceMatrox_CameraTapsMode_4Taps10_12BitsTimeMultiplexed,
			EDeviceMatrox_CameraTapsMode_4Taps8Bits,
			EDeviceMatrox_CameraTapsMode_4Taps10_12Bits,
			EDeviceMatrox_CameraTapsMode_4Taps14_16Bits,
			EDeviceMatrox_CameraTapsMode_8Taps8BitsTimeMultiplexed,
			EDeviceMatrox_CameraTapsMode_8Taps8Bits,
			EDeviceMatrox_CameraTapsMode_10Taps8Bits,
			EDeviceMatrox_CameraTapsMode_3Taps8Bits_RGB,
			EDeviceMatrox_CameraTapsMode_3Taps10_12Bits_RGB,
			EDeviceMatrox_CameraTapsMode_3Taps14_16Bits_RGB,
			EDeviceMatrox_CameraTapsMode_6Taps8BitsTimeMultiplexed_2RGB,
			EDeviceMatrox_CameraTapsMode_6Taps8_2RGB_ABC_DEF_NonADJ,
			EDeviceMatrox_CameraTapsMode_6Taps8_2RGB_ABCDEFGH_ADJ,
			EDeviceMatrox_CameraTapsMode_Count,
		};

		enum EDeviceMatrox_TapsSelector
		{
			EDeviceMatrox_TapsSelector_1 = 0,
			EDeviceMatrox_TapsSelector_2,
			EDeviceMatrox_TapsSelector_4,
			EDeviceMatrox_TapsSelector_8,
			EDeviceMatrox_TapsSelector_10,
			EDeviceMatrox_TapsSelector_Count,
		};

		enum EDeviceMatrox_TapsDirection
		{
			EDeviceMatrox_TapsDirection_LeftTop = 0,
			EDeviceMatrox_TapsDirection_RightTop,
			EDeviceMatrox_TapsDirection_LeftBottom,
			EDeviceMatrox_TapsDirection_RightBottom,
			EDeviceMatrox_TapsDirection_Count,
		};

		enum EDeviceMatrox_VideoSignalType
		{
			EDeviceMatrox_VideoSignalType_Digital = 0,
			EDeviceMatrox_VideoSignalType_Analog,
			EDeviceMatrox_VideoSignalType_Count,
		};

		enum EDeviceMatrox_DataBusWidth
		{
			EDeviceMatrox_DataBusWidth_8bits = 0,
			EDeviceMatrox_DataBusWidth_10bits,
			EDeviceMatrox_DataBusWidth_12bits,
			EDeviceMatrox_DataBusWidth_14bits,
			EDeviceMatrox_DataBusWidth_16bits,
			EDeviceMatrox_DataBusWidth_24bits,
			EDeviceMatrox_DataBusWidth_32bits,
			EDeviceMatrox_DataBusWidth_64bits,
			EDeviceMatrox_DataBusWidth_Count,
		};

		enum EDeviceMatrox_Standard
		{
			EDeviceMatrox_Standard_Monochrome = 0,
			EDeviceMatrox_Standard_RGBColor,
			EDeviceMatrox_Standard_RGBPack,
			EDeviceMatrox_Standard_RGBAlpha,
			EDeviceMatrox_Standard_SVID,
			EDeviceMatrox_Standard_YUVVID,
			EDeviceMatrox_Standard_Count,
		};

		enum EDeviceMatrox_MILChannel
		{
			EDeviceMatrox_MILChannel_0 = 0,
			EDeviceMatrox_MILChannel_1,
			EDeviceMatrox_MILChannel_2,
			EDeviceMatrox_MILChannel_3,
			EDeviceMatrox_MILChannel_Count,
		};

		enum EDeviceMatrox_InoutFormat
		{
			EDeviceMatrox_InoutFormat_TTL = 0,
			EDeviceMatrox_InoutFormat_422,
			EDeviceMatrox_InoutFormat_OPTO,
			EDeviceMatrox_InoutFormat_LVDS,
			EDeviceMatrox_InoutFormat_Count,
		};

		enum EDeviceMatrox_InoutFormatDefault
		{
			EDeviceMatrox_InoutFormatDefault_TTL = 0,
			EDeviceMatrox_InoutFormatDefault_422,
			EDeviceMatrox_InoutFormatDefault_OPTO,
			EDeviceMatrox_InoutFormatDefault_LVDS,
			EDeviceMatrox_InoutFormatDefault_Default,
			EDeviceMatrox_InoutFormatDefault_Count,
		};

		enum EDeviceMatrox_InoutPolarity
		{
			EDeviceMatrox_InoutPolarity_PosEdgeTrig = 0,
			EDeviceMatrox_InoutPolarity_NegEdgeTrig,
			EDeviceMatrox_InoutPolarity_Count,
		};

		enum EDeviceMatrox_ValidSignalSource
		{
			EDeviceMatrox_ValidSignalSource_UseFrameGrabber = 0,
			EDeviceMatrox_ValidSignalSource_UseCamera,
			EDeviceMatrox_ValidSignalSource_Count,
		};

		enum EDeviceMatrox_InterlacedSelector
		{
			EDeviceMatrox_InterlacedSelector_Interlaced = 0,
			EDeviceMatrox_InterlacedSelector_Non_Interlaced,
			EDeviceMatrox_InterlacedSelector_Count,
		};

		enum EDeviceMatrox_AdjustInTheVideoTimingsPage
		{
			EDeviceMatrox_AdjustInTheVideoTimingsPage_uSec = 0,
			EDeviceMatrox_AdjustInTheVideoTimingsPage_Pclk,
			EDeviceMatrox_AdjustInTheVideoTimingsPage_Count,
		};

		enum EDeviceMatrox_ExternalClockSignal
		{
			EDeviceMatrox_ExternalClockSignal_NoClockExchange = 0,
			EDeviceMatrox_ExternalClockSignal_GeneratedByCamera,
			EDeviceMatrox_ExternalClockSignal_GeneratedByDigitizer,
			EDeviceMatrox_ExternalClockSignal_GeneratedByDigitizerAndReturnedByCamera,
			EDeviceMatrox_ExternalClockSignal_Count,
		};

		enum EDeviceMatrox_PixelClockFrequency
		{
			EDeviceMatrox_PixelClockFrequency_1PixelClock = 0,
			EDeviceMatrox_PixelClockFrequency_Count,
		};

		enum EDeviceMatrox_GenerationMode
		{
			EDeviceMatrox_GenerationMode_Disable = 0,
			EDeviceMatrox_GenerationMode_Periodic,
			EDeviceMatrox_GenerationMode_OnTriggerEvent,
			EDeviceMatrox_GenerationMode_Count,
		};

		enum EDeviceMatrox_ExposureClockType
		{
			EDeviceMatrox_ExposureClockType_SynchronousToPixelClock,
			EDeviceMatrox_ExposureClockType_ClockGenerator,
			EDeviceMatrox_ExposureClockType_HSyncClock,
			EDeviceMatrox_ExposureClockType_VSyncClock,
			EDeviceMatrox_ExposureClockType_Timer_Output,
			EDeviceMatrox_ExposureClockType_Aux1_Input_LVDS,
			EDeviceMatrox_ExposureClockType_Count,
		};

		enum EDeviceMatrox_ExposureTriggerSignal
		{
			EDeviceMatrox_ExposureTriggerSignal_PSG_HSync = 0,
			EDeviceMatrox_ExposureTriggerSignal_PSG_VSync,
			EDeviceMatrox_ExposureTriggerSignal_Software,
			EDeviceMatrox_ExposureTriggerSignal_Timer_Output,
			EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_0_Common,
			EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_1_Common,
			EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_2_Common,
			EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_3_Common,
			EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_4_Common,
			EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_5_Common,
			EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_6,
			EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_7,
			EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_8,
			EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_9,
			EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_10,
			EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_11,
			EDeviceMatrox_ExposureTriggerSignal_RotaryEncoderFowardTrigger,
			EDeviceMatrox_ExposureTriggerSignal_RotaryEncoderReversedTrigger,
			EDeviceMatrox_ExposureTriggerSignal_Count,
		};

		enum EDeviceMatrox_DivisionFactor
		{
			EDeviceMatrox_DivisionFactor_BaseClock_1 = 0,
			EDeviceMatrox_DivisionFactor_BaseClock_2,
			EDeviceMatrox_DivisionFactor_BaseClock_4,
			EDeviceMatrox_DivisionFactor_BaseClock_8,
			EDeviceMatrox_DivisionFactor_BaseClock_16,
			EDeviceMatrox_DivisionFactor_Count,
		};

		enum EDeviceMatrox_TimerOutputSignal
		{
			EDeviceMatrox_TimerOutputSignal_Timer1 = 0,
			EDeviceMatrox_TimerOutputSignal_Timer1_XOR_T_imer2,
			EDeviceMatrox_TimerOutputSignal_Timer2,
			EDeviceMatrox_TimerOutputSignal_Count,
		};

		enum EDeviceMatrox_CaptureATriggerEveryTrigger
		{
			EDeviceMatrox_CaptureATriggerEveryTrigger_1 = 0,
			EDeviceMatrox_CaptureATriggerEveryTrigger_2,
			EDeviceMatrox_CaptureATriggerEveryTrigger_4,
			EDeviceMatrox_CaptureATriggerEveryTrigger_8,
			EDeviceMatrox_CaptureATriggerEveryTrigger_16,
			EDeviceMatrox_CaptureATriggerEveryTrigger_Count,
		};

		enum EDeviceMatrox_TriggerArmMode
		{
			EDeviceMatrox_TriggerArmMode_Enable = 0,
			EDeviceMatrox_TriggerArmMode_Disable,
			EDeviceMatrox_TriggerArmMode_Count,
		};

		enum EDeviceMatrox_GrabModeSignal
		{
			EDeviceMatrox_GrabModeSignal_PSH_VSync = 0,
			EDeviceMatrox_GrabModeSignal_Timer_1_Output,
			EDeviceMatrox_GrabModeSignal_Timer_2_Output,
			EDeviceMatrox_GrabModeSignal_AuxiliaryIO_0_Common,
			EDeviceMatrox_GrabModeSignal_AuxiliaryIO_1_Common,
			EDeviceMatrox_GrabModeSignal_AuxiliaryIO_2_Common,
			EDeviceMatrox_GrabModeSignal_AuxiliaryIO_3_Common,
			EDeviceMatrox_GrabModeSignal_AuxiliaryIO_4_Common,
			EDeviceMatrox_GrabModeSignal_AuxiliaryIO_5_Common,
			EDeviceMatrox_GrabModeSignal_AuxiliaryIO_6,
			EDeviceMatrox_GrabModeSignal_AuxiliaryIO_7,
			EDeviceMatrox_GrabModeSignal_AuxiliaryIO_8,
			EDeviceMatrox_GrabModeSignal_AuxiliaryIO_9,
			EDeviceMatrox_GrabModeSignal_AuxiliaryIO_10,
			EDeviceMatrox_GrabModeSignal_AuxiliaryIO_11,
			EDeviceMatrox_GrabModeSignal_RotaryEncoderFowardTrigger,
			EDeviceMatrox_GrabModeSignal_RotaryEncoderReversedTrigger,
			EDeviceMatrox_GrabModeSignal_Count,
		};

		enum EDeviceMatrox_TriggerArmSource
		{
			EDeviceMatrox_TriggerArmSource_Timer1_0 = 0,
			EDeviceMatrox_TriggerArmSource_Software_Arm,
			EDeviceMatrox_TriggerArmSource_PSG_HSync,
			EDeviceMatrox_TriggerArmSource_PSG_VSync,
			EDeviceMatrox_TriggerArmSource_Timer_Output,
			EDeviceMatrox_TriggerArmSource_AuxiliaryIO_0_Common,
			EDeviceMatrox_TriggerArmSource_AuxiliaryIO_1_Common,
			EDeviceMatrox_TriggerArmSource_AuxiliaryIO_2_Common,
			EDeviceMatrox_TriggerArmSource_AuxiliaryIO_3_Common,
			EDeviceMatrox_TriggerArmSource_AuxiliaryIO_4_Common,
			EDeviceMatrox_TriggerArmSource_AuxiliaryIO_5_Common,
			EDeviceMatrox_TriggerArmSource_AuxiliaryIO_6,
			EDeviceMatrox_TriggerArmSource_AuxiliaryIO_7,
			EDeviceMatrox_TriggerArmSource_AuxiliaryIO_8,
			EDeviceMatrox_TriggerArmSource_AuxiliaryIO_9,
			EDeviceMatrox_TriggerArmSource_AuxiliaryIO_10,
			EDeviceMatrox_TriggerArmSource_AuxiliaryIO_11,
			EDeviceMatrox_TriggerArmSource_RotaryEncoderFowardTrigger,
			EDeviceMatrox_TriggerArmSource_RotaryEncoderReversedTrigger,
			EDeviceMatrox_TriggerArmSource_Count,
		};

		enum EDeviceMatrox_TriggerArmSignal
		{
			EDeviceMatrox_TriggerArmSignal_FrameRetrigger = 0,
			EDeviceMatrox_TriggerArmSignal_HardwareIgnoreFrameRetrigger,
			EDeviceMatrox_TriggerArmSignal_SoftwareIgnoreFrameRetrigger,
			EDeviceMatrox_TriggerArmSignal_FrameRetriggerLatched,
			EDeviceMatrox_TriggerArmSignal_Count,
		};

		enum EDeviceMatrox_GrabMode
		{
			EDeviceMatrox_GrabMode_Continuous = 0,
			EDeviceMatrox_GrabMode_SoftwareTrigger,
			EDeviceMatrox_GrabMode_HardwareTrigger,
			EDeviceMatrox_GrabMode_Count,
		};

		enum EDeviceMatrox_GrabModeActivation
		{
			EDeviceMatrox_GrabModeActivation_NextValidFrameOrField,
			EDeviceMatrox_GrabModeActivation_AsynchronousReset,
			EDeviceMatrox_GrabModeActivation_Asynchronous_ResetDelayed_1_Frame,
			EDeviceMatrox_GrabModeActivation_Count,
		};

		enum EDeviceMatrox_SyncSignalSource
		{
			EDeviceMatrox_SyncSignalSource_Digitizer = 0,
			EDeviceMatrox_SyncSignalSource_Camera,
			EDeviceMatrox_SyncSignalSource_Count,
		};

		enum EDeviceMatrox_SyncSignalFormat
		{
			EDeviceMatrox_SyncSignalFormat_Digital = 0,
			EDeviceMatrox_SyncSignalFormat_Analog,
			EDeviceMatrox_SyncSignalFormat_Count,
		};

		enum EDeviceMatrox_SynchronizAtionSignalAvailable
		{
			EDeviceMatrox_SynchronizAtionSignalAvailable_CSync = 0,
			EDeviceMatrox_SynchronizAtionSignalAvailable_HSync_Only,
			EDeviceMatrox_SynchronizAtionSignalAvailable_VSync_Only,
			EDeviceMatrox_SynchronizAtionSignalAvailable_HSync_And_VSync,
			EDeviceMatrox_SynchronizAtionSignalAvailable_Count,
		};

		enum EDeviceMatrox_CameraBitsSourcesControl
		{
			EDeviceMatrox_CameraBitsSourcesControl_Timer_1_Output = 0,
			EDeviceMatrox_CameraBitsSourcesControl_Timer_2_Output,
			EDeviceMatrox_CameraBitsSourcesControl_User_0_OutputHIGH,
			EDeviceMatrox_CameraBitsSourcesControl_User_0_OutputLOW,
			EDeviceMatrox_CameraBitsSourcesControl_User_1_OutputHIGH,
			EDeviceMatrox_CameraBitsSourcesControl_User_1_OutputLOW,
			EDeviceMatrox_CameraBitsSourcesControl_PSG_VSYNC,
			EDeviceMatrox_CameraBitsSourcesControl_PSG_HSYNC,
			EDeviceMatrox_CameraBitsSourcesControl_PixelClock,
			EDeviceMatrox_CameraBitsSourcesControl_Count,
		};

		enum EDeviceMatrox_TestPixelMode
		{
			EDeviceMatrox_TestPixelMode_Default = 0,
			EDeviceMatrox_TestPixelMode_Count,
		};

		enum EDeviceMatrox_SlowScan_PixelClockSelection
		{
			EDeviceMatrox_SlowScan_PixelClockSelection_Default = 0,
			EDeviceMatrox_SlowScan_PixelClockSelection_Count,
		};

		enum EDeviceMatrox_DirectionSwitch
		{
			EDeviceMatrox_DirectionSwitch_Default = 0,
			EDeviceMatrox_DirectionSwitch_Forward,
			EDeviceMatrox_DirectionSwitch_Reverse,
			EDeviceMatrox_DirectionSwitch_Count
		};

		enum EDeviceMatrox_ActiveSwitch
		{
			EDeviceMatrox_ActiveSwitch_Default = 0,
			EDeviceMatrox_ActiveSwitch_Disable,
			EDeviceMatrox_ActiveSwitch_Enable,
			EDeviceMatrox_ActiveSwitch_Count
		};

		enum EDeviceMatrox_ActivationSelect
		{
			EDeviceMatrox_ActivationSelect_Default = 0,
			EDeviceMatrox_ActivationSelect_EdgeFalling,
			EDeviceMatrox_ActivationSelect_EdgeRising,
			EDeviceMatrox_ActivationSelect_LevelHigh,
			EDeviceMatrox_ActivationSelect_LevelLow,
			EDeviceMatrox_ActivationSelect_Count
		};

		enum EDeviceMatrox_TriggerSourceSelect
		{
			EDeviceMatrox_TriggerSourceSelect_AUXIO0 = 0,
			EDeviceMatrox_TriggerSourceSelect_AUXIO1,
			EDeviceMatrox_TriggerSourceSelect_AUXIO2,
			EDeviceMatrox_TriggerSourceSelect_AUXIO3,
			EDeviceMatrox_TriggerSourceSelect_AUXIO4,
			EDeviceMatrox_TriggerSourceSelect_AUXIO5,
			EDeviceMatrox_TriggerSourceSelect_AUXIO6,
			EDeviceMatrox_TriggerSourceSelect_AUXIO7,
			EDeviceMatrox_TriggerSourceSelect_AUXIO8,
			EDeviceMatrox_TriggerSourceSelect_AUXIO9,
			EDeviceMatrox_TriggerSourceSelect_AUXIO10,
			EDeviceMatrox_TriggerSourceSelect_AUXIO11,
			EDeviceMatrox_TriggerSourceSelect_CCIO1,
			EDeviceMatrox_TriggerSourceSelect_CCIO2,
			EDeviceMatrox_TriggerSourceSelect_CCIO3,
			EDeviceMatrox_TriggerSourceSelect_CCIO4,
			EDeviceMatrox_TriggerSourceSelect_Default,
			EDeviceMatrox_TriggerSourceSelect_Hsync,
			EDeviceMatrox_TriggerSourceSelect_RotaryEncoder,
			EDeviceMatrox_TriggerSourceSelect_Software,
			EDeviceMatrox_TriggerSourceSelect_Timer0,
			EDeviceMatrox_TriggerSourceSelect_Timer1,
			EDeviceMatrox_TriggerSourceSelect_Timer2,
			EDeviceMatrox_TriggerSourceSelect_Timer3,
			EDeviceMatrox_TriggerSourceSelect_Timer4,
			EDeviceMatrox_TriggerSourceSelect_Timer5,
			EDeviceMatrox_TriggerSourceSelect_Timer6,
			EDeviceMatrox_TriggerSourceSelect_Timer7,
			EDeviceMatrox_TriggerSourceSelect_Timer8,
			EDeviceMatrox_TriggerSourceSelect_Timer9,
			EDeviceMatrox_TriggerSourceSelect_Timer10,
			EDeviceMatrox_TriggerSourceSelect_Timer11,
			EDeviceMatrox_TriggerSourceSelect_Timer12,
			EDeviceMatrox_TriggerSourceSelect_Timer13,
			EDeviceMatrox_TriggerSourceSelect_Timer14,
			EDeviceMatrox_TriggerSourceSelect_Timer15,
			EDeviceMatrox_TriggerSourceSelect_Timer16,
			EDeviceMatrox_TriggerSourceSelect_Vsync,
			EDeviceMatrox_TriggerSourceSelect_Count,
		};

		enum EDeviceMatrox_TimerSelect
		{
			EDeviceMatrox_TimerSelect_NULL = 0,
			EDeviceMatrox_TimerSelect_Timer1,
			EDeviceMatrox_TimerSelect_Timer2,
			EDeviceMatrox_TimerSelect_Timer3,
			EDeviceMatrox_TimerSelect_Timer4,
			EDeviceMatrox_TimerSelect_Count,
		};


		enum EDeviceMatrox_Switch
		{
			EDeviceMatrox_Switch_Off = 0,
			EDeviceMatrox_Switch_On,
			EDeviceMatrox_Switch_Count
		};
	}
}
//MSP
namespace Ravid
{
	namespace Device
	{
		enum EDeviceMSPBoardType
		{
			EDeviceMSPBoardType_MSP_P16_E2T6_Q288 = 0,
			EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1,
			EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2,
			EDeviceMSPBoardType_DelayedQueue,
			EDeviceMSPBoardType_Count,
		};

		enum EMSPGetFunction
		{
			EMSPGetFunction_OK = 0,
			EMSPGetFunction_UnknownError,
			EMSPGetFunction_EnumTypeError,
			EMSPGetFunction_NullptrError,
		};

		enum EMSPSetFunction
		{
			EMSPSetFunction_OK = 0,
			EMSPSetFunction_UnknownError,
			EMSPSetFunction_AlreadyInitializedError,
			EMSPSetFunction_WriteToDatabaseError,
			EMSPSetFunction_NotSupportedError,
			EMSPSetFunction_WriteToDeviceError,
			EMSPSetFunction_NotFindCommandError,
			EMSPSetFunction_NotSelectFeatureError,
			EMSPSetFunction_ReadOnDeviceError,
			EMSPSetFunction_ReadOnDatabaseError,
		};

		enum EDeviceEncoderDirection
		{
			EDeviceEncoderDirection_CW = 0,
			EDeviceEncoderDirection_CCW,
			EDeviceEncoderDirection_Both,
			EDeviceEncoderDirection_Count,
		};
	}
}









































//Ezi Servo plusE
namespace Ravid
{
	namespace Device
	{
		enum EDeviceParameterEziPlusEDio
		{
			EDeviceParameterEziPlusEDio_DeviceID = 0,
			EDeviceParameterEziPlusEDio_IPAddress,
			EDeviceParameterEziPlusEDio_InputStatus,
			EDeviceParameterEziPlusEDio_UseInputNo0,
			EDeviceParameterEziPlusEDio_UseInputNo1,
			EDeviceParameterEziPlusEDio_UseInputNo2,
			EDeviceParameterEziPlusEDio_UseInputNo3,
			EDeviceParameterEziPlusEDio_UseInputNo4,
			EDeviceParameterEziPlusEDio_UseInputNo5,
			EDeviceParameterEziPlusEDio_UseInputNo6,
			EDeviceParameterEziPlusEDio_UseInputNo7,
			EDeviceParameterEziPlusEDio_UseInputNo8,
			EDeviceParameterEziPlusEDio_UseInputNo9,
			EDeviceParameterEziPlusEDio_UseInputNo10,
			EDeviceParameterEziPlusEDio_UseInputNo11,
			EDeviceParameterEziPlusEDio_UseInputNo12,
			EDeviceParameterEziPlusEDio_UseInputNo13,
			EDeviceParameterEziPlusEDio_UseInputNo14,
			EDeviceParameterEziPlusEDio_UseInputNo15,
			EDeviceParameterEziPlusEDio_OutputStatus,
			EDeviceParameterEziPlusEDio_UseOutputNo0,
			EDeviceParameterEziPlusEDio_UseOutputNo1,
			EDeviceParameterEziPlusEDio_UseOutputNo2,
			EDeviceParameterEziPlusEDio_UseOutputNo3,
			EDeviceParameterEziPlusEDio_UseOutputNo4,
			EDeviceParameterEziPlusEDio_UseOutputNo5,
			EDeviceParameterEziPlusEDio_UseOutputNo6,
			EDeviceParameterEziPlusEDio_UseOutputNo7,
			EDeviceParameterEziPlusEDio_UseOutputNo8,
			EDeviceParameterEziPlusEDio_UseOutputNo9,
			EDeviceParameterEziPlusEDio_UseOutputNo10,
			EDeviceParameterEziPlusEDio_UseOutputNo11,
			EDeviceParameterEziPlusEDio_UseOutputNo12,
			EDeviceParameterEziPlusEDio_UseOutputNo13,
			EDeviceParameterEziPlusEDio_UseOutputNo14,
			EDeviceParameterEziPlusEDio_UseOutputNo15,
			EDeviceParameterEziPlusEDio_InputName,
			EDeviceParameterEziPlusEDio_InputNo0Name,
			EDeviceParameterEziPlusEDio_InputNo1Name,
			EDeviceParameterEziPlusEDio_InputNo2Name,
			EDeviceParameterEziPlusEDio_InputNo3Name,
			EDeviceParameterEziPlusEDio_InputNo4Name,
			EDeviceParameterEziPlusEDio_InputNo5Name,
			EDeviceParameterEziPlusEDio_InputNo6Name,
			EDeviceParameterEziPlusEDio_InputNo7Name,
			EDeviceParameterEziPlusEDio_InputNo8Name,
			EDeviceParameterEziPlusEDio_InputNo9Name,
			EDeviceParameterEziPlusEDio_InputNo10Name,
			EDeviceParameterEziPlusEDio_InputNo11Name,
			EDeviceParameterEziPlusEDio_InputNo12Name,
			EDeviceParameterEziPlusEDio_InputNo13Name,
			EDeviceParameterEziPlusEDio_InputNo14Name,
			EDeviceParameterEziPlusEDio_InputNo15Name,
			EDeviceParameterEziPlusEDio_OutputName,
			EDeviceParameterEziPlusEDio_OutputNo0Name,
			EDeviceParameterEziPlusEDio_OutputNo1Name,
			EDeviceParameterEziPlusEDio_OutputNo2Name,
			EDeviceParameterEziPlusEDio_OutputNo3Name,
			EDeviceParameterEziPlusEDio_OutputNo4Name,
			EDeviceParameterEziPlusEDio_OutputNo5Name,
			EDeviceParameterEziPlusEDio_OutputNo6Name,
			EDeviceParameterEziPlusEDio_OutputNo7Name,
			EDeviceParameterEziPlusEDio_OutputNo8Name,
			EDeviceParameterEziPlusEDio_OutputNo9Name,
			EDeviceParameterEziPlusEDio_OutputNo10Name,
			EDeviceParameterEziPlusEDio_OutputNo11Name,
			EDeviceParameterEziPlusEDio_OutputNo12Name,
			EDeviceParameterEziPlusEDio_OutputNo13Name,
			EDeviceParameterEziPlusEDio_OutputNo14Name,
			EDeviceParameterEziPlusEDio_OutputNo15Name,
			EDeviceParameterEziPlusEDio_Count,
		};

		enum EEziPlusEDioGetFunction
		{
			EEziPlusEDioGetFunction_OK = 0,
			EEziPlusEDioGetFunction_UnknownError,
			EEziPlusEDioGetFunction_EnumTypeError,
			EEziPlusEDioGetFunction_NullptrError,
		};

		enum EEziPlusEDioSetFunction
		{
			EEziPlusEDioSetFunction_OK = 0,
			EEziPlusEDioSetFunction_UnknownError,
			EEziPlusEDioSetFunction_AlreadyInitializedError,
			EEziPlusEDioSetFunction_WriteToDatabaseError,
			EEziPlusEDioSetFunction_NotSupportedError,
			EEziPlusEDioSetFunction_WriteToDeviceError,
			EEziPlusEDioSetFunction_NotFindCommandError,
			EEziPlusEDioSetFunction_NotSelectFeatureError,
			EEziPlusEDioSetFunction_ReadOnDeviceError,
			EEziPlusEDioSetFunction_ReadOnDatabaseError,
		};
	}
}



//Ezi Servo plusE
namespace Ravid
{
	namespace Device
	{
		enum EDeviceParameterEziServo2PlusE
		{
			EDeviceParameterEziServo2PlusE_DeviceID = 0,
			EDeviceParameterEziServo2PlusE_IPAddress,
			EDeviceParameterEziServo2PlusE_MotionType,
			EDeviceParameterEziServo2PlusE_Servo2,
			EDeviceParameterEziServo2PlusE_Servo2_PulsePerRevolution,
			EDeviceParameterEziServo2PlusE_Servo2_AxisMaxSpeed,
			EDeviceParameterEziServo2PlusE_Servo2_AxisStartSpeed,
			EDeviceParameterEziServo2PlusE_Servo2_AxisAccTime,
			EDeviceParameterEziServo2PlusE_Servo2_AxisDecTime,
			EDeviceParameterEziServo2PlusE_Servo2_SpeedOverride,
			EDeviceParameterEziServo2PlusE_Servo2_JogSpeed,
			EDeviceParameterEziServo2PlusE_Servo2_JogStartSpeed,
			EDeviceParameterEziServo2PlusE_Servo2_JogAccDecTime,
			EDeviceParameterEziServo2PlusE_Servo2_SWLimitPlusValue,
			EDeviceParameterEziServo2PlusE_Servo2_SWLimitMinusValue,
			EDeviceParameterEziServo2PlusE_Servo2_SWLimitStopMethod,
			EDeviceParameterEziServo2PlusE_Servo2_HWLimitStopMethod,
			EDeviceParameterEziServo2PlusE_Servo2_LimitSensorLogic,
			EDeviceParameterEziServo2PlusE_Servo2_OrgSpeed,
			EDeviceParameterEziServo2PlusE_Servo2_OrgSearchSpeed,
			EDeviceParameterEziServo2PlusE_Servo2_OrgAccDecTime,
			EDeviceParameterEziServo2PlusE_Servo2_OrgMethod,
			EDeviceParameterEziServo2PlusE_Servo2_OrgDir,
			EDeviceParameterEziServo2PlusE_Servo2_OrgOffSet,
			EDeviceParameterEziServo2PlusE_Servo2_OrgPositionSet,
			EDeviceParameterEziServo2PlusE_Servo2_OrgSensorLogic,
			EDeviceParameterEziServo2PlusE_Servo2_PositionLoopGain,
			EDeviceParameterEziServo2PlusE_Servo2_InposValue,
			EDeviceParameterEziServo2PlusE_Servo2_PosTrackingLimit,
			EDeviceParameterEziServo2PlusE_Servo2_MotionDir,
			EDeviceParameterEziServo2PlusE_Servo2_LimitSensorDir,
			EDeviceParameterEziServo2PlusE_Servo2_OrgTorqueRatio,
			EDeviceParameterEziServo2PlusE_Servo2_PosErrorOverflowLimit,
			EDeviceParameterEziServo2PlusE_Servo2_BrakeDelayTime,
			EDeviceParameterEziServo2PlusE_MotionLink2,
			EDeviceParameterEziServo2PlusE_MotionLink2_EncoderMultiply,
			EDeviceParameterEziServo2PlusE_MotionLink2_AxisMaxSpeed,
			EDeviceParameterEziServo2PlusE_MotionLink2_AxisStartSpeed,
			EDeviceParameterEziServo2PlusE_MotionLink2_AxisAccTime,
			EDeviceParameterEziServo2PlusE_MotionLink2_AxisDecTime,
			EDeviceParameterEziServo2PlusE_MotionLink2_SpeedOverride,
			EDeviceParameterEziServo2PlusE_MotionLink2_JogSpeed,
			EDeviceParameterEziServo2PlusE_MotionLink2_JogStartSpeed,
			EDeviceParameterEziServo2PlusE_MotionLink2_JogAccDecTime,
			EDeviceParameterEziServo2PlusE_MotionLink2_SWLimitPlusValue,
			EDeviceParameterEziServo2PlusE_MotionLink2_SWLimitMinusValue,
			EDeviceParameterEziServo2PlusE_MotionLink2_SWLimitStopMethod,
			EDeviceParameterEziServo2PlusE_MotionLink2_HWLimitStopMethod,
			EDeviceParameterEziServo2PlusE_MotionLink2_LimitSensorLogic,
			EDeviceParameterEziServo2PlusE_MotionLink2_OrgSpeed,
			EDeviceParameterEziServo2PlusE_MotionLink2_OrgSearchSpeed,
			EDeviceParameterEziServo2PlusE_MotionLink2_OrgAccDecTime,
			EDeviceParameterEziServo2PlusE_MotionLink2_OrgMethod,
			EDeviceParameterEziServo2PlusE_MotionLink2_OrgDir,
			EDeviceParameterEziServo2PlusE_MotionLink2_OrgOffSet,
			EDeviceParameterEziServo2PlusE_MotionLink2_OrgPositionSet,
			EDeviceParameterEziServo2PlusE_MotionLink2_OrgSensorLogic,
			EDeviceParameterEziServo2PlusE_MotionLink2_LimitSensorDir,
			EDeviceParameterEziServo2PlusE_MotionLink2_PulseType,
			EDeviceParameterEziServo2PlusE_MotionLink2_EncoderDir,
			EDeviceParameterEziServo2PlusE_MotionLink2_MotionDir,
			EDeviceParameterEziServo2PlusE_MotionLink2_ServoAlarmresetLogic,
			EDeviceParameterEziServo2PlusE_MotionLink2_ServoOnOutputLogic,
			EDeviceParameterEziServo2PlusE_MotionLink2_ServoAlarmLogic,
			EDeviceParameterEziServo2PlusE_MotionLink2_ServoInpositionLogic,
			EDeviceParameterEziServo2PlusE_InputStatus,
			EDeviceParameterEziServo2PlusE_UseInputNo0,
			EDeviceParameterEziServo2PlusE_UseInputNo1,
			EDeviceParameterEziServo2PlusE_UseInputNo2,
			EDeviceParameterEziServo2PlusE_UseInputNo3,
			EDeviceParameterEziServo2PlusE_UseInputNo4,
			EDeviceParameterEziServo2PlusE_UseInputNo5,
			EDeviceParameterEziServo2PlusE_UseInputNo6,
			EDeviceParameterEziServo2PlusE_UseInputNo7,
			EDeviceParameterEziServo2PlusE_UseInputNo8,
			EDeviceParameterEziServo2PlusE_OutputStatus,
			EDeviceParameterEziServo2PlusE_UseOutputNo0,
			EDeviceParameterEziServo2PlusE_UseOutputNo1,
			EDeviceParameterEziServo2PlusE_UseOutputNo2,
			EDeviceParameterEziServo2PlusE_UseOutputNo3,
			EDeviceParameterEziServo2PlusE_UseOutputNo4,
			EDeviceParameterEziServo2PlusE_UseOutputNo5,
			EDeviceParameterEziServo2PlusE_UseOutputNo6,
			EDeviceParameterEziServo2PlusE_UseOutputNo7,
			EDeviceParameterEziServo2PlusE_UseOutputNo8,
			EDeviceParameterEziServo2PlusE_InputName,
			EDeviceParameterEziServo2PlusE_InputNo0Name,
			EDeviceParameterEziServo2PlusE_InputNo1Name,
			EDeviceParameterEziServo2PlusE_InputNo2Name,
			EDeviceParameterEziServo2PlusE_InputNo3Name,
			EDeviceParameterEziServo2PlusE_InputNo4Name,
			EDeviceParameterEziServo2PlusE_InputNo5Name,
			EDeviceParameterEziServo2PlusE_InputNo6Name,
			EDeviceParameterEziServo2PlusE_InputNo7Name,
			EDeviceParameterEziServo2PlusE_InputNo8Name,
			EDeviceParameterEziServo2PlusE_OutputName,
			EDeviceParameterEziServo2PlusE_OutputNo0Name,
			EDeviceParameterEziServo2PlusE_OutputNo1Name,
			EDeviceParameterEziServo2PlusE_OutputNo2Name,
			EDeviceParameterEziServo2PlusE_OutputNo3Name,
			EDeviceParameterEziServo2PlusE_OutputNo4Name,
			EDeviceParameterEziServo2PlusE_OutputNo5Name,
			EDeviceParameterEziServo2PlusE_OutputNo6Name,
			EDeviceParameterEziServo2PlusE_OutputNo7Name,
			EDeviceParameterEziServo2PlusE_OutputNo8Name,
			EDeviceParameterEziServo2PlusE_Count,
		};

		enum EEziServo2PlusEGetFunction
		{
			EEziServo2PlusEGetFunction_OK = 0,
			EEziServo2PlusEGetFunction_UnknownError,
			EEziServo2PlusEGetFunction_EnumTypeError,
			EEziServo2PlusEGetFunction_NullptrError,
		};

		enum EEziServo2PlusESetFunction
		{
			EEziServo2PlusESetFunction_OK = 0,
			EEziServo2PlusESetFunction_UnknownError,
			EEziServo2PlusESetFunction_AlreadyInitializedError,
			EEziServo2PlusESetFunction_WriteToDatabaseError,
			EEziServo2PlusESetFunction_NotSupportedError,
			EEziServo2PlusESetFunction_WriteToDeviceError,
			EEziServo2PlusESetFunction_NotFindCommandError,
			EEziServo2PlusESetFunction_NotSelectFeatureError,
			EEziServo2PlusESetFunction_ReadOnDeviceError,
			EEziServo2PlusESetFunction_ReadOnDatabaseError,
		};

		enum EEziServo2PlusEMotionType
		{
			EEziServo2PlusEMotionType_Servo2 = 0,
			EEziServo2PlusEMotionType_MotionLink2,
			EEziServo2PlusEMotionType_Count,
		};

		enum EEziServo2PlusESWLimitStopMethod
		{
			EEziServo2PlusESWLimitStopMethod_EmergencyStop = 0,
			EEziServo2PlusESWLimitStopMethod_SoftStop,
			EEziServo2PlusESWLimitStopMethod_NotUsedSWLimit,
			EEziServo2PlusESWLimitStopMethod_Count,
		};

		enum EEziServo2PlusEHWLimitStopMethod
		{
			EEziServo2PlusEHWLimitStopMethod_EmergencyStop = 0,
			EEziServo2PlusEHWLimitStopMethod_SoftStop,
			EEziServo2PlusEHWLimitStopMethod_Count,
		};

		enum EEziServo2PlusELogic
		{
			EEziServo2PlusELogic_Low = 0,
			EEziServo2PlusELogic_High,
			EEziServo2PlusELogic_Count,
		};

		enum EEziServo2PlusEOrgMethod
		{
			EEziServo2PlusEOrgMethod_OrgSpeedToHomeOrgSearchSpeedToHome = 0,
			EEziServo2PlusEOrgMethod_OrgSpeedToHomeOrgSearchSpeedToZPulse,
			EEziServo2PlusEOrgMethod_OrgSpeedToLimit,
			EEziServo2PlusEOrgMethod_OrgSpeedToLimitOrgSearchSpeedToZPulse,
			EEziServo2PlusEOrgMethod_SetHome,
			EEziServo2PlusEOrgMethod_OrgSearchSpeedToZPulse,
			EEziServo2PlusEOrgMethod_OrgSpeedToObject,
			EEziServo2PlusEOrgMethod_OrgSpeedToObjectOrgSearchSpeedToZPulse,
			EEziServo2PlusEOrgMethod_Count,
		};

		enum EEziServo2PlusEDir
		{
			EEziServo2PlusEDir_CW = 0,
			EEziServo2PlusEDir_CCW,
			EEziServo2PlusEDir_Count,
		};

		enum EEziServo2PlusSwitch
		{
			EEziServo2PlusSwitch_Off = 0,
			EEziServo2PlusSwitch_On,
			EEziServo2PlusSwitch_Count,
		};

		enum EEziServo2PlusEMotionLinek2OrgMethod
		{
			EEziServo2PlusEMotionLinek2OrgMethod_OrgSpeedToHomeOrgSearchSpeedToHome = 0,
			EEziServo2PlusEMotionLinek2OrgMethod_OrgSpeedToHomeOrgSearchSpeedToZPulse,
			EEziServo2PlusEMotionLinek2OrgMethod_OrgSpeedToLimit,
			EEziServo2PlusEMotionLinek2OrgMethod_OrgSpeedToLimitOrgSearchSpeedToZPulse,
			EEziServo2PlusEMotionLinek2OrgMethod_OrgSearchSpeedToZPulse,
			EEziServo2PlusEMotionLinek2OrgMethod_SetHome,
			EEziServo2PlusEMotionLinek2OrgMethod_Count,
		};

		enum EEziServo2PlusEPulseType
		{
			EEziServo2PlusEPulseType_PulseDir = 0,
			EEziServo2PlusEPulseType_CWCCW,
			EEziServo2PlusEPulseType_Count,
		};

		enum EEziServo2PlusEEncoderMethod
		{
			EEziServo2PlusEEncoderMethod_NotUsed = 0,
			EEziServo2PlusEEncoderMethod_Sqr1Mode,
			EEziServo2PlusEEncoderMethod_Sqr2Mode,
			EEziServo2PlusEEncoderMethod_Sqr4Mode,
			EEziServo2PlusEEncoderMethod_Count,
		};
	}
}

// AxlTrigger
namespace Ravid
{
	namespace Device
	{
		enum EAxlTriggerGetFunction
		{
			EAxlTriggerGetFunction_OK = 0,
			EAxlTriggerGetFunction_UnknownError,
			EAxlTriggerGetFunction_EnumTypeError,
			EAxlTriggerGetFunction_NullptrError,
		};

		enum EAxlTriggerSetFunction
		{
			EAxlTriggerSetFunction_OK = 0,
			EAxlTriggerSetFunction_UnknownError,
			EAxlTriggerSetFunction_NotFoundApiError,
			EAxlTriggerSetFunction_AlreadyInitializedError,
			EAxlTriggerSetFunction_NotInitializedError,
			EAxlTriggerSetFunction_NotFindCommandError, // _CommandDoesNotExist
			EAxlTriggerSetFunction_NotSelectModeError,
			EAxlTriggerSetFunction_DeviceNullptrError,
			EAxlTriggerSetFunction_NotSupportError,
			EAxlTriggerSetFunction_FunctionLockedError,
			EAxlTriggerSetFunction_WriteToDeviceError,
			EAxlTriggerSetFunction_WriteToDatabaseError,
			EAxlTriggerSetFunction_ReadOnDatabaseError,
			EAxlTriggerSetFunction_ReadOnDeviceError,
			EAxlTriggerSetFunction_ActivatedDeviceError,
		};

		enum EDeviceAxlTriggerFunction
		{
			EDeviceAxlTriggerFunction_PeriodAndAbsolute = 0,
			EDeviceAxlTriggerFunction_Count,
		};

		enum EDeviceAxlTriggerEncoderInputMethod
		{
			EDeviceAxlTriggerEncoderInputMethod_UpDown = 0,
			EDeviceAxlTriggerEncoderInputMethod_Sqr1,
			EDeviceAxlTriggerEncoderInputMethod_Sqr2,
			EDeviceAxlTriggerEncoderInputMethod_Sqr4,
			EDeviceAxlTriggerEncoderInputMethod_Count,
		};

		enum EDeviceAxlTriggerEncoderSource
		{
			EDeviceAxlTriggerEncoderSource_AB = 0,
			EDeviceAxlTriggerEncoderSource_Z,
			EDeviceAxlTriggerEncoderSource_Count,
		};

		enum EDeviceAxlTriggerActiveLevel
		{
			EDeviceAxlTriggerActiveLevel_Low = 0,
			EDeviceAxlTriggerActiveLevel_High,
			EDeviceAxlTriggerActiveLevel_Count,
		};

		enum EDeviceAxlTriggerMode
		{
			EDeviceAxlTriggerMode_Absolute = 0,
			EDeviceAxlTriggerMode_Periodic,
			EDeviceAxlTriggerMode_Count,
		};

		enum EDeviceAxlTriggerDirectionCheck
		{
			EDeviceAxlTriggerDirectionCheck_Both = 0,
			EDeviceAxlTriggerDirectionCheck_CW,
			EDeviceAxlTriggerDirectionCheck_CCW,
			EDeviceAxlTriggerDirectionCheck_Count,
		};

		enum EDeviceAxlTriggerSwitch
		{
			EDeviceAxlTriggerSwitch_Off = 0,
			EDeviceAxlTriggerSwitch_On,
			EDeviceAxlTriggerSwitch_Count,
		};
	}
}

namespace Ravid
{
	namespace Device
	{
		enum EDeviceBaslerUSBPixelFormat
		{
			EDeviceBaslerUSBPixelFormat_Mono8 = 0,
			EDeviceBaslerUSBPixelFormat_Mono10,
			EDeviceBaslerUSBPixelFormat_Mono10p,
			EDeviceBaslerUSBPixelFormat_BayerGR8,
			EDeviceBaslerUSBPixelFormat_BayerRG8,
			EDeviceBaslerUSBPixelFormat_BayerGB8,
			EDeviceBaslerUSBPixelFormat_BayerBG8,
			EDeviceBaslerUSBPixelFormat_BayerGR10,
			EDeviceBaslerUSBPixelFormat_BayerGR10p,
			EDeviceBaslerUSBPixelFormat_BayerRG10,
			EDeviceBaslerUSBPixelFormat_BayerRG10p,
			EDeviceBaslerUSBPixelFormat_BayerGB10,
			EDeviceBaslerUSBPixelFormat_BayerGB10p,
			EDeviceBaslerUSBPixelFormat_BayerBG10,
			EDeviceBaslerUSBPixelFormat_BayerBG10p,
			EDeviceBaslerUSBPixelFormat_RGB8,
			EDeviceBaslerUSBPixelFormat_BGR8,
			EDeviceBaslerUSBPixelFormat_YCbCr422_8,
			EDeviceBaslerUSBPixelFormat_Count,
		};

		enum EDeviceBaslerUSBGainAuto
		{
			EDeviceBaslerUSBGainAuto_Off = 0,
			EDeviceBaslerUSBGainAuto_Once,
			EDeviceBaslerUSBGainAuto_Continuous,
			EDeviceBaslerUSBGainAuto_Count,
		};

		enum EDeviceBaslerUSBGainSelector
		{
			EDeviceBaslerUSBGainSelector_All = 0,
			EDeviceBaslerUSBGainSelector_Count,
		};

		enum EDeviceBaslerUSBBlackLevelSelector
		{
			EDeviceBaslerUSBBlackLevelSelector_All = 0,
			EDeviceBaslerUSBBlackLevelSelector_Count,
		};

		enum EDeviceBaslerUSBShutterMode
		{
			EDeviceBaslerUSBShutterMode_Global = 0,
			EDeviceBaslerUSBShutterMode_Rolling,
			EDeviceBaslerUSBShutterMode_GlobalResetRelease,
			EDeviceBaslerUSBShutterMode_Count,
		};

		enum EDeviceBaslerUSBExposureAuto
		{
			EDeviceBaslerUSBExposureAuto_Off = 0,
			EDeviceBaslerUSBExposureAuto_Once,
			EDeviceBaslerUSBExposureAuto_Continuous,
			EDeviceBaslerUSBExposureAuto_Count,
		};

		enum EDeviceBaslerUSBExposureMode
		{
			EDeviceBaslerUSBExposureMode_Timed = 0,
			EDeviceBaslerUSBExposureMode_TriggerWidth,
			EDeviceBaslerUSBExposureMode_Count,
		};

		enum EDeviceBaslerUSBTriggerSelector
		{
			EDeviceBaslerUSBTriggerSelector_FrameStart = 0,
			EDeviceBaslerUSBTriggerSelector_FrameBurstStart,
			EDeviceBaslerUSBTriggerSelector_Count,
		};

		enum EDeviceBaslerUSBTriggerMode
		{
			EDeviceBaslerUSBTriggerMode_Off = 0,
			EDeviceBaslerUSBTriggerMode_On,
			EDeviceBaslerUSBTriggerMode_Count,
		};

		enum EDeviceBaslerUSBTriggerSource
		{
			EDeviceBaslerUSBTriggerSource_Software = 0,
			EDeviceBaslerUSBTriggerSource_Line1,
			EDeviceBaslerUSBTriggerSource_Line2,
			EDeviceBaslerUSBTriggerSource_Line3,
			EDeviceBaslerUSBTriggerSource_Line4,
			EDeviceBaslerUSBTriggerSource_SoftwareSignal1,
			EDeviceBaslerUSBTriggerSource_SoftwareSignal2,
			EDeviceBaslerUSBTriggerSource_SoftwareSignal3,
			EDeviceBaslerUSBTriggerSource_Count,
		};

		enum EDeviceBaslerUSBTriggerActivation
		{
			EDeviceBaslerUSBTriggerActivation_RisingEdge = 0,
			EDeviceBaslerUSBTriggerActivation_FallingEdge,
			EDeviceBaslerUSBTriggerActivation_AnyEdge,
			EDeviceBaslerUSBTriggerActivation_LevelHigh,
			EDeviceBaslerUSBTriggerActivation_LevelLow,
			EDeviceBaslerUSBTriggerActivation_Count,
		};

		enum EDeviceBaslerUSBAcquisitionStatusSelector
		{
			EDeviceBaslerUSBAcquisitionStatusSelector_FrameTriggerWait = 0,
			EDeviceBaslerUSBAcquisitionStatusSelector_FrameBurstTriggerWait,
			EDeviceBaslerUSBAcquisitionStatusSelector_FrameBurstTriggerActive,
			EDeviceBaslerUSBAcquisitionStatusSelector_FrameBurstTriggerTransfer,
			EDeviceBaslerUSBAcquisitionStatusSelector_FrameActive,
			EDeviceBaslerUSBAcquisitionStatusSelector_FrameTransfer,
			EDeviceBaslerUSBAcquisitionStatusSelector_ExposureActive,
			EDeviceBaslerUSBAcquisitionStatusSelector_Count,
		};

		enum EDeviceBaslerUSBLineSelector
		{
			EDeviceBaslerUSBLineSelector_Line1 = 0,
			EDeviceBaslerUSBLineSelector_Line2,
			EDeviceBaslerUSBLineSelector_Line3,
			EDeviceBaslerUSBLineSelector_Line4,
			EDeviceBaslerUSBLineSelector_Count,
		};

		enum EDeviceBaslerUSBLineMode
		{
			EDeviceBaslerUSBLineMode_Input = 0,
			EDeviceBaslerUSBLineMode_Output,
			EDeviceBaslerUSBLineMode_Count,
		};

		enum EDeviceBaslerUSBLineSource
		{
			EDeviceBaslerUSBLineSource_ExposureActive = 0,
			EDeviceBaslerUSBLineSource_FrameTriggerWait,
			EDeviceBaslerUSBLineSource_FrameBurstTriggerWait,
			EDeviceBaslerUSBLineSource_Timer1Active,
			EDeviceBaslerUSBLineSource_UserOutput0,
			EDeviceBaslerUSBLineSource_UserOutput1,
			EDeviceBaslerUSBLineSource_UserOutput2,
			EDeviceBaslerUSBLineSource_UserOutput3,
			EDeviceBaslerUSBLineSource_FlashWindow,
			EDeviceBaslerUSBLineSource_Count,
		};

		enum EDeviceBaslerUSBSwitch
		{
			EDeviceBaslerUSBSwitch_Off = 0,
			EDeviceBaslerUSBSwitch_On,
			EDeviceBaslerUSBSwitch_Count,
		};
	}
}



namespace Ravid
{
	namespace Device
	{
		enum EGoogolGtsGetFunction
		{
			EGoogolGtsGetFunction_OK = 0,
			EGoogolGtsGetFunction_UnknownError,
			EGoogolGtsGetFunction_EnumTypeError,
			EGoogolGtsGetFunction_NullptrError,
		};

		enum EGoogolGtsSetFunction
		{
			EGoogolGtsSetFunction_OK = 0,
			EGoogolGtsSetFunction_UnknownError,
			EGoogolGtsSetFunction_AlreadyGrabError,
			EGoogolGtsSetFunction_InitializedDeviceError,
			EGoogolGtsSetFunction_WriteToDatabaseError,
			EGoogolGtsSetFunction_NotSupportedError,
			EGoogolGtsSetFunction_WriteToDeviceError,
			EGoogolGtsSetFunction_NotFindCommandError,
			EGoogolGtsSetFunction_NotSelectFeatureError,
			EGoogolGtsSetFunction_ReadOnDeviceError,
			EGoogolGtsSetFunction_ReadOnDatabaseError,
		};

		enum EGoogolGtsSwitch
		{
			EGoogolGtsSwitch_Off = 0,
			EGoogolGtsSwitch_On,
			EGoogolGtsSwitch_Count,
		};

		enum EGoogolGtsInitializeType
		{
			EGoogolGtsInitializeType_NotExtMode = 0,
			EGoogolGtsInitializeType_OnlyExtMode,
			EGoogolGtsInitializeType_MotionAndExtMode,
			EGoogolGtsInitializeType_Count,
		};
	}
}



namespace Ravid
{
	namespace Device
	{
		enum EMSPDioGetFunction
		{
			EMSPDioGetFunction_OK = 0,
			EMSPDioGetFunction_UnknownError,
			EMSPDioGetFunction_EnumTypeError,
			EMSPDioGetFunction_NullptrError,
		};

		enum EMSPDioSetFunction
		{
			EMSPDioSetFunction_OK = 0,
			EMSPDioSetFunction_UnknownError,
			EMSPDioSetFunction_AlreadyInitializedError,
			EMSPDioSetFunction_WriteToDatabaseError,
			EMSPDioSetFunction_NotSupportedError,
			EMSPDioSetFunction_WriteToDeviceError,
			EMSPDioSetFunction_NotFindCommandError,
			EMSPDioSetFunction_NotSelectFeatureError,
			EMSPDioSetFunction_ReadOnDeviceError,
			EMSPDioSetFunction_ReadOnDatabaseError,
		};
	}
}

namespace Ravid
{
	namespace Device
	{
		enum EDeviceParameterSensovationGigE
		{
			EDeviceParameterSensovationGigE_DeviceID = 0,
			EDeviceParameterSensovationGigE_CameraSetting,
			EDeviceParameterSensovationGigE_GrabCount,
			EDeviceParameterSensovationGigE_OffsetX,
			EDeviceParameterSensovationGigE_OffsetY,
			EDeviceParameterSensovationGigE_CanvasWidth,
			EDeviceParameterSensovationGigE_CanvasHeight,
			EDeviceParameterSensovationGigE_Binning,
			EDeviceParameterSensovationGigE_StaticWidth,
			EDeviceParameterSensovationGigE_StaticHeight,
			EDeviceParameterSensovationGigE_BaseSetting,
			EDeviceParameterSensovationGigE_Gain,
			EDeviceParameterSensovationGigE_ExposureTime,
			EDeviceParameterSensovationGigE_TriggerMode,
			EDeviceParameterSensovationGigE_Count,
		};

		enum ESensovationGetFunction
		{
			ESensovationGetFunction_OK = 0,
			ESensovationGetFunction_UnknownError,
			ESensovationGetFunction_NullptrError,
			ESensovationGetFunction_EnumTypeError,
		};

		enum ESensovationSetFunction
		{
			ESensovationSetFunction_OK = 0,
			ESensovationSetFunction_UnknownError,
			ESensovationSetFunction_NotFoundApiError,
			ESensovationSetFunction_AlreadyInitializedError,
			ESensovationSetFunction_NotInitializedError,
			ESensovationSetFunction_NotFindCommandError,
			ESensovationSetFunction_DeviceNullptrError,
			ESensovationSetFunction_NotSupportError,
			ESensovationSetFunction_FunctionLockedError,
			ESensovationSetFunction_WriteToDeviceError,
			ESensovationSetFunction_WriteToDatabaseError,
			ESensovationSetFunction_ReadOnDatabaseError,
			ESensovationSetFunction_ReadOnDeviceError,
			ESensovationSetFunction_ActivatedDeviceError,
		};

		enum EDeviceSensovationGigESwitch
		{
			EDeviceSensovationGigESwitch_Off = 0,
			EDeviceSensovationGigESwitch_On,
			EDeviceSensovationGigESwitch_Count,
		};

		enum EDeviceSensovationGigEBinning
		{
			EDeviceSensovationGigEBinning_1 = 0,
			EDeviceSensovationGigEBinning_2,
			EDeviceSensovationGigEBinning_4,
			EDeviceSensovationGigEBinning_Count,
		};

		enum EDeviceSensovationGigETriggerMode
		{
			EDeviceSensovationGigETriggerMode_FreeRunning = 0,
			EDeviceSensovationGigETriggerMode_Extern,
			EDeviceSensovationGigETriggerMode_Software,
			EDeviceSensovationGigETriggerMode_Count,
		};
	}
}

namespace Ravid
{
	namespace Device
	{
		enum ESoftservoGetFunction
		{
			ESoftservoGetFunction_OK = 0,
			ESoftservoGetFunction_UnknownError,
			ESoftservoGetFunction_NullptrError,
		};

		enum ESoftservoSetFunction
		{
			ESoftservoSetFunction_OK = 0,
			ESoftservoSetFunction_UnknownError,
			ESoftservoSetFunction_AlreadyInitializedError,
			ESoftservoSetFunction_ReadOnDatabaseError,
			ESoftservoSetFunction_WriteToDatabaseError,
			ESoftservoSetFunction_ReadOnDeviceError,
			ESoftservoSetFunction_WriteToDeviceError,
			ESoftservoSetFunction_NotFoundCommandError,
		};

		enum EDeviceSoftservoWMX2HomeType
		{
			EDeviceSoftservoWMX2HomeType_HS = 0,
			EDeviceSoftservoWMX2HomeType_HSIndexPulse,
			EDeviceSoftservoWMX2HomeType_Count,
		};

		enum EDeviceSoftservoWMX2HomeDirection
		{
			EDeviceSoftservoWMX2HomeDirection_Normal = 0,
			EDeviceSoftservoWMX2HomeDirection_Reverse,
			EDeviceSoftservoWMX2HomeDirection_Count,
		};

		enum EDeviceSoftservoWMX2LevelMethod
		{
			EDeviceSoftservoWMX2LevelMethod_Low = 0,
			EDeviceSoftservoWMX2LevelMethod_High,
			EDeviceSoftservoWMX2LevelMethod_Count,
		};

		enum EDeviceSoftservoWMX3HomeType
		{
			EDeviceSoftservoWMX3HomeType_CurrentPos = 0,
			EDeviceSoftservoWMX3HomeType_ZPulse,
			EDeviceSoftservoWMX3HomeType_HS,
			EDeviceSoftservoWMX3HomeType_HSHS,
			EDeviceSoftservoWMX3HomeType_HSZPulse,
			EDeviceSoftservoWMX3HomeType_HSReverseZPulse,
			EDeviceSoftservoWMX3HomeType_HSOff,
			EDeviceSoftservoWMX3HomeType_HSOffZPulse,
			EDeviceSoftservoWMX3HomeType_HSOffReverseZPulse,
			EDeviceSoftservoWMX3HomeType_LSReverseZPulse,
			EDeviceSoftservoWMX3HomeType_NearLSReverseZPulse,
			EDeviceSoftservoWMX3HomeType_ExternalLSReverseZPulse,
			EDeviceSoftservoWMX3HomeType_TrqLimit,
			EDeviceSoftservoWMX3HomeType_TouchProbe,
			EDeviceSoftservoWMX3HomeType_HSTouchProbe,
			EDeviceSoftservoWMX3HomeType_LS,
			EDeviceSoftservoWMX3HomeType_NearLS,
			EDeviceSoftservoWMX3HomeType_ExternalLS,
			EDeviceSoftservoWMX3HomeType_MechanicalEndDetection,
			EDeviceSoftservoWMX3HomeType_MechanicalEndDetectionHS,
			EDeviceSoftservoWMX3HomeType_MechanicalEndDetectionLS,
			EDeviceSoftservoWMX3HomeType_MechanicalEndDetectionReverseZPuls,
			EDeviceSoftservoWMX3HomeType_Count,
		};

		enum EDeviceSoftservoWMX3HomeDirection
		{
			EDeviceSoftservoWMX3HomeDirection_Positive = 0,
			EDeviceSoftservoWMX3HomeDirection_Negative,
			EDeviceSoftservoWMX3HomeDirection_Count,
		};

		enum EDeviceSoftservoWMX3LevelMethod
		{
			EDeviceSoftservoWMX3LevelMethod_Low = 0,
			EDeviceSoftservoWMX3LevelMethod_High,
			EDeviceSoftservoWMX3LevelMethod_Count,
		};

		enum EDeviceSoftservoWMX3InitializeType
		{
			EDeviceSoftservoWMX3InitializeType_Motion = 0,
			EDeviceSoftservoWMX3InitializeType_Param,
			EDeviceSoftservoWMX3InitializeType_Count,
		};
	}
}

namespace Ravid
{
	namespace Device
	{
		enum EDeviceParameterPhotonfocus3D
		{
			EDeviceParameterPhotonfocus3D_DeviceID = 0,
			EDeviceParameterPhotonfocus3D_CameraSetting,
			EDeviceParameterPhotonfocus3D_GrabCount,
			EDeviceParameterPhotonfocus3D_Viewertype,
			EDeviceParameterPhotonfocus3D_3DImageHeight,
			EDeviceParameterPhotonfocus3D_CanvasWidth,
			EDeviceParameterPhotonfocus3D_CanvasHeight,
			EDeviceParameterPhotonfocus3D_OffsetX,
			EDeviceParameterPhotonfocus3D_OffsetY,
			EDeviceParameterPhotonfocus3D_BufferCount,
			EDeviceParameterPhotonfocus3D_GrabWaitingTime,
			EDeviceParameterPhotonfocus3D_DetectTimeout,
			EDeviceParameterPhotonfocus3D_AcquisitionControl,
			EDeviceParameterPhotonfocus3D_AcquisitionMode,
			EDeviceParameterPhotonfocus3D_AcquisitionFrameCount,
			EDeviceParameterPhotonfocus3D_TriggerSelector,
			EDeviceParameterPhotonfocus3D_TriggerMode,
			EDeviceParameterPhotonfocus3D_TriggerSource,
			EDeviceParameterPhotonfocus3D_TriggerActivation,
			EDeviceParameterPhotonfocus3D_TriggerDelay,
			EDeviceParameterPhotonfocus3D_TriggerDivider,
			EDeviceParameterPhotonfocus3D_ABTriggerDebounce,
			EDeviceParameterPhotonfocus3D_ABTriggerMode,
			EDeviceParameterPhotonfocus3D_ABTriggerDirection,
			EDeviceParameterPhotonfocus3D_ABTriggerDivider,
			EDeviceParameterPhotonfocus3D_ABEncoderPosition,
			EDeviceParameterPhotonfocus3D_ExposureMode,
			EDeviceParameterPhotonfocus3D_ExposureTime,
			EDeviceParameterPhotonfocus3D_TimerControl,
			EDeviceParameterPhotonfocus3D_TimerSelector,
			EDeviceParameterPhotonfocus3D_TimerDurationRaw,
			EDeviceParameterPhotonfocus3D_TimerDelayRaw,
			EDeviceParameterPhotonfocus3D_TimerTriggerSource,
			EDeviceParameterPhotonfocus3D_TimerTriggerActivation,
			EDeviceParameterPhotonfocus3D_TimerGranularityFactor,
			EDeviceParameterPhotonfocus3D_AnalogControl,
			EDeviceParameterPhotonfocus3D_GainSelector,
			EDeviceParameterPhotonfocus3D_Gain,
			EDeviceParameterPhotonfocus3D_BlackLevelSelector,
			EDeviceParameterPhotonfocus3D_BlackLevel,
			EDeviceParameterPhotonfocus3D_DigitalOffset,
			EDeviceParameterPhotonfocus3D_PeakDetector,
			EDeviceParameterPhotonfocus3D_Peak0,
			EDeviceParameterPhotonfocus3D_Peak0_3D,
			EDeviceParameterPhotonfocus3D_Peak0_Threshold,
			EDeviceParameterPhotonfocus3D_Peak0_3DY,
			EDeviceParameterPhotonfocus3D_Peak0_3DH,
			EDeviceParameterPhotonfocus3D_Peak0_Mirror,
			EDeviceParameterPhotonfocus3D_Peak0_2D,
			EDeviceParameterPhotonfocus3D_Peak0_2DY,
			EDeviceParameterPhotonfocus3D_Peak0_Gain,
			EDeviceParameterPhotonfocus3D_Peak0_DigitalOffset,
			EDeviceParameterPhotonfocus3D_Peak0_PeakFilter,
			EDeviceParameterPhotonfocus3D_Peak0_EnPeakFilter,
			EDeviceParameterPhotonfocus3D_Peak0_PeakFilterHeightMin,
			EDeviceParameterPhotonfocus3D_Peak0_PeakFilterHeightMax,
			EDeviceParameterPhotonfocus3D_Peak0_PeakFilterWidthMin,
			EDeviceParameterPhotonfocus3D_Peak0_PeakFilterWidthMax,
			EDeviceParameterPhotonfocus3D_Peak0_MovingROI,
			EDeviceParameterPhotonfocus3D_Peak0_EnMovingROI,
			EDeviceParameterPhotonfocus3D_Peak0_MovingROIEnLaserFinder,
			EDeviceParameterPhotonfocus3D_Peak0_MovingROIRangeY,
			EDeviceParameterPhotonfocus3D_Peak0_MovingROIRangeH,
			EDeviceParameterPhotonfocus3D_Peak0_MovingROIMinCol,
			EDeviceParameterPhotonfocus3D_Peak0_MovingROIMinDistance,
			EDeviceParameterPhotonfocus3D_Peak0_MovingROISmoothing,
			EDeviceParameterPhotonfocus3D_Peak0_EnAbsCoordinate,
			EDeviceParameterPhotonfocus3D_Peak0_AbsCoordinateBase,
			EDeviceParameterPhotonfocus3D_Peak1,
			EDeviceParameterPhotonfocus3D_Peak1_3D,
			EDeviceParameterPhotonfocus3D_Peak1_Threshold,
			EDeviceParameterPhotonfocus3D_Peak1_3DY,
			EDeviceParameterPhotonfocus3D_Peak1_3DH,
			EDeviceParameterPhotonfocus3D_Peak1_Mirror,
			EDeviceParameterPhotonfocus3D_Peak1_2D,
			EDeviceParameterPhotonfocus3D_Peak1_2DY,
			EDeviceParameterPhotonfocus3D_Peak1_Gain,
			EDeviceParameterPhotonfocus3D_Peak1_DigitalOffset,
			EDeviceParameterPhotonfocus3D_Peak1_PeakFilter,
			EDeviceParameterPhotonfocus3D_Peak1_EnPeakFilter,
			EDeviceParameterPhotonfocus3D_Peak1_PeakFilterHeightMin,
			EDeviceParameterPhotonfocus3D_Peak1_PeakFilterHeightMax,
			EDeviceParameterPhotonfocus3D_Peak1_PeakFilterWidthMin,
			EDeviceParameterPhotonfocus3D_Peak1_PeakFilterWidthMax,
			EDeviceParameterPhotonfocus3D_Peak1_EnAbsCoordinate,
			EDeviceParameterPhotonfocus3D_Peak1_AbsCoordinateBase,
			EDeviceParameterPhotonfocus3D_PeakDetector_NrOfPeaks,
			EDeviceParameterPhotonfocus3D_PeakDetector_DataFormat3D,
			EDeviceParameterPhotonfocus3D_PeakDetector_HighSpeed,
			EDeviceParameterPhotonfocus3D_Count,
		};

		enum EPhotonfocus3DGetFunction
		{
			EPhotonfocus3DGetFunction_OK = 0,
			EPhotonfocus3DGetFunction_UnknownError,
			EPhotonfocus3DGetFunction_NullptrError,
			EPhotonfocus3DGetFunction_EnumTypeError, //범위 벗어나는 경우
		};

		enum EPhotonfocus3DSetFunction
		{
			EPhotonfocus3DSetFunction_OK = 0,
			EPhotonfocus3DSetFunction_UnknownError,
			EPhotonfocus3DSetFunction_NotFoundApiError,
			EPhotonfocus3DSetFunction_AlreadyInitializedError,
			EPhotonfocus3DSetFunction_NotInitializedError,
			EPhotonfocus3DSetFunction_NotFindCommandError,
			EPhotonfocus3DSetFunction_DeviceNullptrError,
			EPhotonfocus3DSetFunction_NotSupportError,
			EPhotonfocus3DSetFunction_FunctionLockedError,
			EPhotonfocus3DSetFunction_WriteToDeviceError,
			EPhotonfocus3DSetFunction_WriteToDatabaseError,
			EPhotonfocus3DSetFunction_ReadOnDatabaseError,
			EPhotonfocus3DSetFunction_ReadOnDeviceError,
			EPhotonfocus3DSetFunction_ActivatedDeviceError,
		};

		enum EDevicePhotonfocus3DViewerType
		{
			EDevicePhotonfocus3DViewerType_2DOnly = 0,
			EDevicePhotonfocus3DViewerType_2DAnd3D,
			EDevicePhotonfocus3DViewerType_3DOnly,
			EDevicePhotonfocus3DViewerType_Count,
		};

		enum EDevicePhotonfocus3DAcquisitionMode
		{
			EDevicePhotonfocus3DAcquisitionMode_Continuous = 0,
			EDevicePhotonfocus3DAcquisitionMode_SingleFrame,
			EDevicePhotonfocus3DAcquisitionMode_MultiFrame,
			EDevicePhotonfocus3DAcquisitionMode_ContinuousRecording,
			EDevicePhotonfocus3DAcquisitionMode_ContinuousReadout,
			EDevicePhotonfocus3DAcquisitionMode_SingleFrameRecording,
			EDevicePhotonfocus3DAcquisitionMode_SingleFrameReadout,
			EDevicePhotonfocus3DAcquisitionMode_Count,
		};

		enum EDevicePhotonfocus3DFlag
		{
			EDevicePhotonfocus3DFlag_False = 0,
			EDevicePhotonfocus3DFlag_True,
			EDevicePhotonfocus3DFlag_Count,
		};

		enum EDevicePhotonfocus3DTriggerSelector
		{
			EDevicePhotonfocus3DTriggerSelector_FrameStart = 0,
			EDevicePhotonfocus3DTriggerSelector_Count,
		};

		enum EDevicePhotonfocus3DSwitch
		{
			EDevicePhotonfocus3DSwitch_Off = 0,
			EDevicePhotonfocus3DSwitch_On,
			EDevicePhotonfocus3DSwitch_Count,
		};

		enum EDevicePhotonfocus3DTriggerSource
		{
			EDevicePhotonfocus3DTriggerSource_Software = 0,
			EDevicePhotonfocus3DTriggerSource_Line1,
			EDevicePhotonfocus3DTriggerSource_PLC_Q4,
			EDevicePhotonfocus3DTriggerSource_ABTrigger,
			EDevicePhotonfocus3DTriggerSource_Count,
		};

		enum EDevicePhotonfocus3DTriggerActivation
		{
			EDevicePhotonfocus3DTriggerActivation_RisingEdge = 0,
			EDevicePhotonfocus3DTriggerActivation_FallingEdge,
			EDevicePhotonfocus3DTriggerActivation_Count,
		};

		enum EDevicePhotonfocus3DABTriggerMode
		{
			EDevicePhotonfocus3DABTriggerMode_Single = 0,
			EDevicePhotonfocus3DABTriggerMode_Dual,
			EDevicePhotonfocus3DABTriggerMode_Quad,
			EDevicePhotonfocus3DABTriggerMode_Count,
		};

		enum EDevicePhotonfocus3DABTriggerDirection
		{
			EDevicePhotonfocus3DABTriggerDirection_Forward = 0,
			EDevicePhotonfocus3DABTriggerDirection_Backward,
			EDevicePhotonfocus3DABTriggerDirection_ForwardBackward,
			EDevicePhotonfocus3DABTriggerDirection_Count,
		};

		enum EDevicePhotonfocus3DExposureMode
		{
			EDevicePhotonfocus3DExposureMode_Timed = 0,
			EDevicePhotonfocus3DExposureMode_Count,
		};

		enum EDevicePhotonfocus3DGainSelector
		{
			EDevicePhotonfocus3DGainSelector_DigitalAll = 0,
			EDevicePhotonfocus3DGainSelector_Count,
		};

		enum EDevicePhotonfocus3DBlackLevelSelector
		{
			EDevicePhotonfocus3DBlackLevelSelector_All = 0,
			EDevicePhotonfocus3DBlackLevelSelector_Count,
		};

		enum EDevicePhotonfocus3DMovingROISmoothing
		{
			EDevicePhotonfocus3DMovingROISmoothing_None = 0,
			EDevicePhotonfocus3DMovingROISmoothing_MA8,
			EDevicePhotonfocus3DMovingROISmoothing_MA16,
			EDevicePhotonfocus3DMovingROISmoothing_Count,
		};
	}
}

namespace Ravid
{
	namespace Device
	{
		enum EDeviceParameterBaslerTOFGigE
		{
			EDeviceParameterBaslerTOFGigE_DeviceID = 0,

			EDeviceParameterBaslerTOFGigE_DrawPoint,					// Check
			EDeviceParameterBaslerTOFGigE_DrawImage,					// Combo

			// Device Control

			EDeviceParameterBaslerTOFGigE_DeviceControl,				// None
			EDeviceParameterBaslerTOFGigE_DeviceChannel,				// Integer 0 - 3
			EDeviceParameterBaslerTOFGigE_CalibrationRangeOffset,		// Integer -255 - 255
			EDeviceParameterBaslerTOFGigE_RectificationControl,				// None
			EDeviceParameterBaslerTOFGigE_Rectification,					// Enumeration
			EDeviceParameterBaslerTOFGigE_DeviceTemperatureSelector,	// Enumeration

			// Image Control

			EDeviceParameterBaslerTOFGigE_ImageControl,					// None
			EDeviceParameterBaslerTOFGigE_Binning,						// Bool
			EDeviceParameterBaslerTOFGigE_CanvasWidth,					// Integer
			EDeviceParameterBaslerTOFGigE_CanvasHeight,					// Integer
			EDeviceParameterBaslerTOFGigE_OffsetX,						// Integer
			EDeviceParameterBaslerTOFGigE_OffsetY,						// Integer
			EDeviceParameterBaslerTOFGigE_MinimumDepth_mm,				// Integer
			EDeviceParameterBaslerTOFGigE_MaximumDepth_mm,				// Integer
			EDeviceParameterBaslerTOFGigE_ComponentSelector,			// Enumeration //
//			EDeviceParameterBaslerTOFGigE_PixelFormat,					// Enumeration //

			// Acquisition Control

			EDeviceParameterBaslerTOFGigE_AcquisitionControl,			// None
			EDeviceParameterBaslerTOFGigE_LEDDisable,					// Bool
			EDeviceParameterBaslerTOFGigE_ProcessingMode,				// Enumeration
			EDeviceParameterBaslerTOFGigE_AcquisitionFrameRate,			// Float
			EDeviceParameterBaslerTOFGigE_ExposureAuto,					// Enumeration	// Processing Mode 연결
			EDeviceParameterBaslerTOFGigE_Agility,						// Float		// Exposure Auto Continuous
			EDeviceParameterBaslerTOFGigE_Delay,						// Integer		// Exposure Auto Continuous
			EDeviceParameterBaslerTOFGigE_ExposureTimeSelector,			// Integer
			EDeviceParameterBaslerTOFGigE_ExposureTime_us,				// Integer		// Exposure Auto Continuous 아닐 경우에 살아남~
			EDeviceParameterBaslerTOFGigE_TriggerMode,					// Enumeration
			EDeviceParameterBaslerTOFGigE_TriggerSource,				// Enumeration
			EDeviceParameterBaslerTOFGigE_TriggerDelay_us,				// Float

			EDeviceParameterBaslerTOFGigE_SyncTimerControl,				// None
			EDeviceParameterBaslerTOFGigE_TimestampLatch,				// Command (bool) 1회성
			EDeviceParameterBaslerTOFGigE_TimestampLow_ns,				// Read only
			EDeviceParameterBaslerTOFGigE_TimestampHigh_ns,				// Read only
			EDeviceParameterBaslerTOFGigE_StartTimeLow_us,				// Integer
			EDeviceParameterBaslerTOFGigE_StartTimeHigh_us,				// Integer
			EDeviceParameterBaslerTOFGigE_TriggerRat_Hz,				// Float
			EDeviceParameterBaslerTOFGigE_Update,						// Command (bool) 1회성

			EDeviceParameterBaslerTOFGigE_ImageQualityControl,			// None
			EDeviceParameterBaslerTOFGigE_ConfidenceThreshold,			// Integer		// 0 - 65536
			EDeviceParameterBaslerTOFGigE_SpatialFilter,				// Bool
			EDeviceParameterBaslerTOFGigE_TemporalFilter,				// Bool
			EDeviceParameterBaslerTOFGigE_Strength,						// Integer		// 50 - 240
			EDeviceParameterBaslerTOFGigE_OutlierTolerance,				// Integer		// 0 - 65536
			EDeviceParameterBaslerTOFGigE_RangeFilter,				// Bool

			EDeviceParameterBaslerTOFGigE_DigitalIOControl,					// None
			EDeviceParameterBaslerTOFGigE_LineSelector,						// Enumerate
			EDeviceParameterBaslerTOFGigE_InputLine1,						// None
			EDeviceParameterBaslerTOFGigE_Line1Inverter,					// bool
			EDeviceParameterBaslerTOFGigE_Line1DebouncerTime_us,			// float
			EDeviceParameterBaslerTOFGigE_InputLine2,						// None
			EDeviceParameterBaslerTOFGigE_Line2Inverter,					// bool
			EDeviceParameterBaslerTOFGigE_Line2DebouncerTime_us,			// float
			EDeviceParameterBaslerTOFGigE_OutputLine1,						// None
			EDeviceParameterBaslerTOFGigE_OutputLine1Source,				// Enum
			EDeviceParameterBaslerTOFGigE_OutputLine1Inverter,				// bool
			EDeviceParameterBaslerTOFGigE_OutputLine1OutputPulseMinWidth,	//float
			EDeviceParameterBaslerTOFGigE_OutputLine2,						// None
			EDeviceParameterBaslerTOFGigE_OutputLine2Source,				// enum
			EDeviceParameterBaslerTOFGigE_OutputLine2Inverter,				// bool
			EDeviceParameterBaslerTOFGigE_OutputLine2OutputPulseMinWidth,	//float
			EDeviceParameterBaslerTOFGigE_UserOutputSelector,				// Enumerate
			EDeviceParameterBaslerTOFGigE_UserOutput1,						// None
			EDeviceParameterBaslerTOFGigE_UserOutput1Value,					// bool
			EDeviceParameterBaslerTOFGigE_UserOutput2,						// None
			EDeviceParameterBaslerTOFGigE_UserOutput2Value,					// bool
			EDeviceParameterBaslerTOFGigE_UserOutputValueAll,				// integer

			EDeviceParameterBaslerTOFGigE_Count,
		};

		enum EBaslerTOFGetFunction
		{
			EBaslerTOFGetFunction_OK = 0,
			EBaslerTOFGetFunction_UnknownError,
			EBaslerTOFGetFunction_EnumTypeError,
			EBaslerTOFGetFunction_NullptrError,
		};

		enum EBaslerTOFSetFunction
		{
			EBaslerTOFSetFunction_OK = 0,
			EBaslerTOFSetFunction_UnknownError,
			EBaslerTOFSetFunction_AlreadyInitializedError,
			EBaslerTOFSetFunction_WriteToDatabaseError,
			EBaslerTOFSetFunction_NotSupportedError,
			EBaslerTOFSetFunction_WriteToDeviceError,
			EBaslerTOFSetFunction_NotFindCommandError,
			EBaslerTOFSetFunction_NotSelectFeatureError,
			EBaslerTOFSetFunction_ReadOnDeviceError,
			EBaslerTOFSetFunction_ReadOnDatabaseError,
			EBaslerTOFSetFunction_ActiveDeviceError,
		};

		enum EDeviceBaslerTOFGigEExposureAuto
		{
			EDeviceBaslerTOFGigEExposureAuto_Off = 0,
			EDeviceBaslerTOFGigEExposureAuto_Continuous,
			EDeviceBaslerTOFGigEExposureAuto_Count,
		};

		enum EDeviceBaslerTOFGigESwitch
		{
			EDeviceBaslerTOFGigESwitch_Off = 0,
			EDeviceBaslerTOFGigESwitch_On,
			EDeviceBaslerTOFGigESwitch_Count,
		};

		enum EDeviceBaslerTOFGigETemperatureSelector
		{
			EDeviceBaslerTOFGigETemperatureSelector_SensorBoard = 0,
			EDeviceBaslerTOFGigETemperatureSelector_LEDBoard,
			EDeviceBaslerTOFGigETemperatureSelector_Count,
		};

		enum EDeviceBaslerTOFGigEComponentSelector
		{
			EDeviceBaslerTOFGigEComponentSelector_Range = 0,
			EDeviceBaslerTOFGigEComponentSelector_Intensity,
			EDeviceBaslerTOFGigEComponentSelector_Confidence,
			EDeviceBaslerTOFGigEComponentSelector_Count,
		};

		enum EDeviceBaslerTOFGigEProcessingMode
		{
			EDeviceBaslerTOFGigEProcessingMode_Standard = 0,
			EDeviceBaslerTOFGigEProcessingMode_HDR,
			EDeviceBaslerTOFGigEProcessingMode_Count,
		};

		enum EDeviceBaslerTOFGigETriggerSource
		{
			EDeviceBaslerTOFGigETriggerSource_Software = 0,
			EDeviceBaslerTOFGigETriggerSource_Line1,
			EDeviceBaslerTOFGigETriggerSource_Line2,
			EDeviceBaslerTOFGigETriggerSource_SyncTimer,
			EDeviceBaslerTOFGigETriggerSource_Count,
		};

		enum EDeviceBaslerTOFGigELineSelector
		{
			EDeviceBaslerTOFGigELineSelector_Line1 = 0,
			EDeviceBaslerTOFGigELineSelector_Line2,
			EDeviceBaslerTOFGigELineSelector_OutputLine1,
			EDeviceBaslerTOFGigELineSelector_OutputLine2,
			EDeviceBaslerTOFGigELineSelector_Count,
		};

		enum EDeviceBaslerTOFGigELineSource
		{
			EDeviceBaslerTOFGigELineSource_ExposureActive = 0,
			EDeviceBaslerTOFGigELineSource_UserOutput,
			EDeviceBaslerTOFGigELineSource_AcquisitionTriggerWait,
			EDeviceBaslerTOFGigELineSource_Count,
		};

		enum EDeviceBaslerTOFGigEUserOutputSelector
		{
			EDeviceBaslerTOFGigEUserOutputSelector_UserOutput1 = 0,
			EDeviceBaslerTOFGigEUserOutputSelector_UserOutput2,
			EDeviceBaslerTOFGigEUserOutputSelector_Count,
		};

		enum EDeviceBaslerTOFGigESelectImage
		{
			EDeviceBaslerTOFGigESelectImage_Intensity = 0,
			EDeviceBaslerTOFGigESelectImage_Confidence,
			EDeviceBaslerTOFGigESelectImage_3DData,
			EDeviceBaslerTOFGigESelectImage_Count,
		};

		enum EDeviceBaslerTOFGigEDeviceChannel
		{
			EDeviceBaslerTOFGigEDeviceChannel_0 = 0,
			EDeviceBaslerTOFGigEDeviceChannel_1,
			EDeviceBaslerTOFGigEDeviceChannel_2,
			EDeviceBaslerTOFGigEDeviceChannel_3,
			EDeviceBaslerTOFGigEDeviceChannel_Count,
		};
	}
}

namespace Ravid
{
	namespace Device
	{
		enum EDeviceParameterMeereTOFGigE
		{
			EDeviceParameterMeereTOFGigE_DeviceID = 0,

			EDeviceParameterMeereTOFGigE_DrawPoint,					// Check

			EDeviceParameterMeereTOFGigE_InitType,					// Enum

			// Device Control
			EDeviceParameterMeereTOFGigE_IntegrationTime,
			EDeviceParameterMeereTOFGigE_Offset,					// int -255 ~ 255
			EDeviceParameterMeereTOFGigE_DepthThreshold,			// int 0 ~ 255
			EDeviceParameterMeereTOFGigE_ScatterThreshold,			// int 0 ~ 255
			EDeviceParameterMeereTOFGigE_MinDepth,					// int 
			EDeviceParameterMeereTOFGigE_MaxDepth,					// int 

			EDeviceParameterMeereTOFGigE_FlipHorizontal,			// bool
			EDeviceParameterMeereTOFGigE_FlipVertical,				// bool

			EDeviceParameterMeereTOFGigE_MultiSyncMode,				// bool

			EDeviceParameterMeereTOFGigE_FPSDelay,					// int

			EDeviceParameterMeereTOFGigE_RemoveEdge,				// bool

			EDeviceParameterMeereTOFGigE_Undistortion,				// Enum 
			EDeviceParameterMeereTOFGigE_Orthogonal,				// bool

			EDeviceParameterMeereTOFGigE_LensIntrinsicParam,		// nul
			EDeviceParameterMeereTOFGigE_FocalLengthX,				// float
			EDeviceParameterMeereTOFGigE_FocalLengthY,				// float
			EDeviceParameterMeereTOFGigE_PrincipalPointX,			// float
			EDeviceParameterMeereTOFGigE_PrincipalPointY,			// float
			EDeviceParameterMeereTOFGigE_LensDistortionParam,		// nul
			EDeviceParameterMeereTOFGigE_DistortionK1,				// float
			EDeviceParameterMeereTOFGigE_DistortionK2,				// float
			EDeviceParameterMeereTOFGigE_DistortionK3,				// float
			EDeviceParameterMeereTOFGigE_DistortionP1,				// float
			EDeviceParameterMeereTOFGigE_DistortionP2,				// float
			EDeviceParameterMeereTOFGigE_Skew,						// float

			EDeviceParameterMeereTOFGigE_RGBLensIntrinsicParam,		// nul
			EDeviceParameterMeereTOFGigE_RGBFocalLengthX,			// float
			EDeviceParameterMeereTOFGigE_RGBFocalLengthY,			// float
			EDeviceParameterMeereTOFGigE_RGBPrincipalPointX,		// float
			EDeviceParameterMeereTOFGigE_RGBPrincipalPointY,		// float
			EDeviceParameterMeereTOFGigE_RGBLensDistortionParam,	// nul
			EDeviceParameterMeereTOFGigE_RGBDistortionK1,			// float
			EDeviceParameterMeereTOFGigE_RGBDistortionK2,			// float
			EDeviceParameterMeereTOFGigE_RGBDistortionK3,			// float
			EDeviceParameterMeereTOFGigE_RGBDistortionP1,			// float
			EDeviceParameterMeereTOFGigE_RGBDistortionP2,			// float
			EDeviceParameterMeereTOFGigE_RGBSkew,					// float

			EDeviceParameterMeereTOFGigE_ExtrinsicParam,			// nul(only depth-rgb camera model)
			EDeviceParameterMeereTOFGigE_ExtrinsicR11,					// float
			EDeviceParameterMeereTOFGigE_ExtrinsicR12,					// float
			EDeviceParameterMeereTOFGigE_ExtrinsicR13,					// float
			EDeviceParameterMeereTOFGigE_ExtrinsicR21,					// float
			EDeviceParameterMeereTOFGigE_ExtrinsicR22,					// float
			EDeviceParameterMeereTOFGigE_ExtrinsicR23,					// float
			EDeviceParameterMeereTOFGigE_ExtrinsicR31,					// float
			EDeviceParameterMeereTOFGigE_ExtrinsicR32,					// float
			EDeviceParameterMeereTOFGigE_ExtrinsicR33,					// float
			EDeviceParameterMeereTOFGigE_ExtrinsicTx,					// float
			EDeviceParameterMeereTOFGigE_ExtrinsicTy,					// float
			EDeviceParameterMeereTOFGigE_ExtrinsicTz,					// float


			EDeviceParameterMeereTOFGigE_Count,
		};

		enum EMeereTOFGetFunction
		{
			EMeereTOFGetFunction_OK = 0,
			EMeereTOFGetFunction_UnknownError,
			EMeereTOFGetFunction_EnumTypeError,
			EMeereTOFGetFunction_NullptrError,
		};

		enum EMeereTOFSetFunction
		{
			EMeereTOFSetFunction_OK = 0,
			EMeereTOFSetFunction_UnknownError,
			EMeereTOFSetFunction_AlreadyInitializedError,
			EMeereTOFSetFunction_WriteToDatabaseError,
			EMeereTOFSetFunction_NotSupportedError,
			EMeereTOFSetFunction_WriteToDeviceError,
			EMeereTOFSetFunction_NotFindCommandError,
			EMeereTOFSetFunction_NotSelectFeatureError,
			EMeereTOFSetFunction_ReadOnDeviceError,
			EMeereTOFSetFunction_ReadOnDatabaseError,
			EMeereTOFSetFunction_ActiveDeviceError,
		};

		enum EDeviceMeereTOFInitType
		{
			EDeviceMeereTOFInitType_OnlyParam = 0,
			EDeviceMeereTOFInitType_NotParam,
			EDeviceMeereTOFInitType_Count,
		};

		enum EDeviceMeereTOFUndistortion
		{
			EDeviceMeereTOFUndistortion_Off = 0,
			EDeviceMeereTOFUndistortion_On_Inter_Edge_On,
			EDeviceMeereTOFUndistortion_On_Inter_On,
			EDeviceMeereTOFUndistortion_On_Inter_Off,
			EDeviceMeereTOFUndistortion_Count,
		};

		enum EDeviceMeereTOFSwitch
		{
			EDeviceMeereTOFSwitch_Off = 0,
			EDeviceMeereTOFSwitch_On,
			EDeviceMeereTOFSwitch_Count,
		};
	}
}

namespace Ravid
{
	namespace Device
	{
		enum EDeviceParameterAlliedGigE
		{
			EDeviceParameterAlliedGigE_DeviceID = 0,
			EDeviceParameterAlliedGigE_DeviceFindWaittingTime,
			EDeviceParameterAlliedGigE_GrabWaittingTime,
			EDeviceParameterAlliedGigE_InitializeType,
			EDeviceParameterAlliedGigE_Width,
			EDeviceParameterAlliedGigE_Height,
			EDeviceParameterAlliedGigE_RegionX,
			EDeviceParameterAlliedGigE_RegionY,
			EDeviceParameterAlliedGigE_BinningX,
			EDeviceParameterAlliedGigE_BinningY,
			EDeviceParameterAlliedGigE_DecimationHorizontal,
			EDeviceParameterAlliedGigE_DecimationVertical,
			EDeviceParameterAlliedGigE_PixelFormat,
			EDeviceParameterAlliedGigE_PacketSize,
			EDeviceParameterAlliedGigE_AcquisitionInfo,
			EDeviceParameterAlliedGigE_Acquisition,
			EDeviceParameterAlliedGigE_AcqEndTriggerEvent,
			EDeviceParameterAlliedGigE_AcqEndTriggerMode,
			EDeviceParameterAlliedGigE_AcqRecTriggerEvent,
			EDeviceParameterAlliedGigE_AcqRecTriggerMode,
			EDeviceParameterAlliedGigE_AcqStartTriggerEvent,
			EDeviceParameterAlliedGigE_AcqStartTriggerMode,
			EDeviceParameterAlliedGigE_FrameRate,
			EDeviceParameterAlliedGigE_FrameStartTriggerDelay,
			EDeviceParameterAlliedGigE_FrameStartTriggerEvent,
			EDeviceParameterAlliedGigE_FrameStartTriggerMode,
			EDeviceParameterAlliedGigE_FrameStartTriggerOverlap,
			EDeviceParameterAlliedGigE_AcquisitionFrameCount,
			EDeviceParameterAlliedGigE_AcquisitionMode,
			EDeviceParameterAlliedGigE_RecorderPreEventCount,
			EDeviceParameterAlliedGigE_ControlInfo,
			EDeviceParameterAlliedGigE_Color,
			EDeviceParameterAlliedGigE_ColorTransformationMode,
			EDeviceParameterAlliedGigE_ColorTransformationValueBB,
			EDeviceParameterAlliedGigE_ColorTransformationValueBG,
			EDeviceParameterAlliedGigE_ColorTransformationValueBR,
			EDeviceParameterAlliedGigE_ColorTransformationValueGB,
			EDeviceParameterAlliedGigE_ColorTransformationValueGG,
			EDeviceParameterAlliedGigE_ColorTransformationValueGR,
			EDeviceParameterAlliedGigE_ColorTransformationValueRB,
			EDeviceParameterAlliedGigE_ColorTransformationValueRG,
			EDeviceParameterAlliedGigE_ColorTransformationValueRR,
			EDeviceParameterAlliedGigE_DSP,
			EDeviceParameterAlliedGigE_DSPSubregionBottom,
			EDeviceParameterAlliedGigE_DSPSubregionLeft,
			EDeviceParameterAlliedGigE_DSPSubregionRight,
			EDeviceParameterAlliedGigE_DSPSubregionTop,
			EDeviceParameterAlliedGigE_Exposure,
			EDeviceParameterAlliedGigE_ExposureMode,
			EDeviceParameterAlliedGigE_ExposureValue,
			EDeviceParameterAlliedGigE_ExposureAutoAdjustTol,
			EDeviceParameterAlliedGigE_ExposureAutoAlg,
			EDeviceParameterAlliedGigE_ExposureAutoMax,
			EDeviceParameterAlliedGigE_ExposureAutoMin,
			EDeviceParameterAlliedGigE_ExposureAutoOutliers,
			EDeviceParameterAlliedGigE_ExposureAutoRate,
			EDeviceParameterAlliedGigE_ExposureAutoTarget,
			EDeviceParameterAlliedGigE_Gain,
			EDeviceParameterAlliedGigE_GainMode,
			EDeviceParameterAlliedGigE_GainValue,
			EDeviceParameterAlliedGigE_GainAutoAdjustTol,
			EDeviceParameterAlliedGigE_GainAutoMax,
			EDeviceParameterAlliedGigE_GainAutoMin,
			EDeviceParameterAlliedGigE_GainAutoOutliers,
			EDeviceParameterAlliedGigE_GainAutoRate,
			EDeviceParameterAlliedGigE_GainAutoTarget,
			EDeviceParameterAlliedGigE_Iris,
			EDeviceParameterAlliedGigE_IrisAutoTarget,
			EDeviceParameterAlliedGigE_IrisMode,
			EDeviceParameterAlliedGigE_IrisVideoLevelMax,
			EDeviceParameterAlliedGigE_IrisVideoLevelMin,
			EDeviceParameterAlliedGigE_LUTControl,
			EDeviceParameterAlliedGigE_LUTEnable,
			EDeviceParameterAlliedGigE_LUTIndex,
			EDeviceParameterAlliedGigE_LUTMode,
			EDeviceParameterAlliedGigE_LUTSelector,
			EDeviceParameterAlliedGigE_LUTValue,
			EDeviceParameterAlliedGigE_ETC,
			EDeviceParameterAlliedGigE_EdgeFilter,
			EDeviceParameterAlliedGigE_Gamma,
			EDeviceParameterAlliedGigE_Hue,
			EDeviceParameterAlliedGigE_OffsetValue,
			EDeviceParameterAlliedGigE_Saturation,
			EDeviceParameterAlliedGigE_Whitebalance,
			EDeviceParameterAlliedGigE_WhitebalMode,
			EDeviceParameterAlliedGigE_WhitebalValueBlue,
			EDeviceParameterAlliedGigE_WhitebalValueRed,
			EDeviceParameterAlliedGigE_WhitebalAutoAdjustTol,
			EDeviceParameterAlliedGigE_WhitebalAutoRate,
			EDeviceParameterAlliedGigE_GigEInfo,
			EDeviceParameterAlliedGigE_BandwidthCtrlMode,
			EDeviceParameterAlliedGigE_HeartbeatInterval,
			EDeviceParameterAlliedGigE_HeartbeatTimeout,
			EDeviceParameterAlliedGigE_GVSP,
			EDeviceParameterAlliedGigE_GvcpRetries,
			EDeviceParameterAlliedGigE_GvspLookbackWindow,
			EDeviceParameterAlliedGigE_GvspResendPercent,
			EDeviceParameterAlliedGigE_GvspRetries,
			EDeviceParameterAlliedGigE_GvspSocketBuffersCount,
			EDeviceParameterAlliedGigE_GvspTimeout,
			EDeviceParameterAlliedGigE_IOInfo,
			EDeviceParameterAlliedGigE_Strobe,
			EDeviceParameterAlliedGigE_Strobe1ControlledDuration,
			EDeviceParameterAlliedGigE_Strobe1Delay,
			EDeviceParameterAlliedGigE_Strobe1Duration,
			EDeviceParameterAlliedGigE_Strobe1Mode,
			EDeviceParameterAlliedGigE_Sync,
			EDeviceParameterAlliedGigE_SyncIn1GlitchFilter,
			EDeviceParameterAlliedGigE_SyncIn2GlitchFilter,
			EDeviceParameterAlliedGigE_SyncOut1Invert,
			EDeviceParameterAlliedGigE_SyncOut1Mode,
			EDeviceParameterAlliedGigE_SyncOut2Invert,
			EDeviceParameterAlliedGigE_SyncOut2Mode,
			EDeviceParameterAlliedGigE_SyncOutGpoLevels,
			EDeviceParameterAlliedGigE_StreamBytesPerSecond,
			EDeviceParameterAlliedGigE_StreamFrameRateConstrain,
			EDeviceParameterAlliedGigE_StreamHoldEnable,
			EDeviceParameterAlliedGigE_Count,
		};

		enum EAlliedGigEGetFunction
		{
			EAlliedGigEGetFunction_OK = 0,
			EAlliedGigEGetFunction_UnknownError,
			EAlliedGigEGetFunction_EnumTypeError,
			EAlliedGigEGetFunction_NullptrError,
		};

		enum EAlliedGigESetFunction
		{
			EAlliedGigESetFunction_OK = 0,
			EAlliedGigESetFunction_UnknownError,
			EAlliedGigESetFunction_AlreadyInitializedError,
			EAlliedGigESetFunction_WriteToDatabaseError,
			EAlliedGigESetFunction_NotSupportedError,
			EAlliedGigESetFunction_WriteToDeviceError,
			EAlliedGigESetFunction_NotFindCommandError,
			EAlliedGigESetFunction_NotSelectFeatureError,
			EAlliedGigESetFunction_ReadOnDeviceError,
			EAlliedGigESetFunction_ReadOnDatabaseError,
			EAlliedGigESetFunction_ActiveDeviceError,
		};

		enum EDeviceAlliedGigEInitializeType
		{
			EDeviceAlliedGigEInitializeType_OnlyParam = 0,
			EDeviceAlliedGigEInitializeType_NotParam,
			EDeviceAlliedGigEInitializeType_Count,
		};

		enum EDeviceAlliedGigEPixelFormat
		{
			EDeviceAlliedGigEPixelFormat_Mono8 = 0,
			EDeviceAlliedGigEPixelFormat_Mono16,
			EDeviceAlliedGigEPixelFormat_Bayer8,
			EDeviceAlliedGigEPixelFormat_Bayer16,
			EDeviceAlliedGigEPixelFormat_Rgb24,
			EDeviceAlliedGigEPixelFormat_Yuv411,
			EDeviceAlliedGigEPixelFormat_Yuv422,
			EDeviceAlliedGigEPixelFormat_Yuv444,
			EDeviceAlliedGigEPixelFormat_Bgr24,
			EDeviceAlliedGigEPixelFormat_Bayer12Packed,
			EDeviceAlliedGigEPixelFormat_Count,
		};

		enum EDeviceAlliedGigETriggerEvent
		{
			EDeviceAlliedGigETriggerEvent_EdgeRising = 0,
			EDeviceAlliedGigETriggerEvent_EdgeFalling,
			EDeviceAlliedGigETriggerEvent_EdgeAny,
			EDeviceAlliedGigETriggerEvent_LevelHigh,
			EDeviceAlliedGigETriggerEvent_LevelLow,
			EDeviceAlliedGigETriggerEvent_Count,
		};

		enum EDeviceAlliedGigETriggerMode
		{
			EDeviceAlliedGigETriggerMode_Disabled = 0,
			EDeviceAlliedGigETriggerMode_SyncIn1,
			EDeviceAlliedGigETriggerMode_SyncIn2,
			EDeviceAlliedGigETriggerMode_Count,
		};

		enum EDeviceAlliedGigEFrameMode
		{
			EDeviceAlliedGigEFrameMode_Freerun = 0,
			EDeviceAlliedGigEFrameMode_SyncIn1,
			EDeviceAlliedGigEFrameMode_SyncIn2,
			EDeviceAlliedGigEFrameMode_FixedRate,
			EDeviceAlliedGigEFrameMode_Software,
			EDeviceAlliedGigEFrameMode_Count,
		};

		enum EDeviceAlliedGigETriggerOverlap
		{
			EDeviceAlliedGigETriggerOverlap_Off = 0,
			EDeviceAlliedGigETriggerOverlap_PreviousFrame,
			EDeviceAlliedGigETriggerOverlap_Count,
		};

		enum EDeviceAlliedGigEAcquisitionMode
		{
			EDeviceAlliedGigEAcquisitionMode_Continuous = 0,
			EDeviceAlliedGigEAcquisitionMode_SingleFrame,
			EDeviceAlliedGigEAcquisitionMode_MultiFrame,
			EDeviceAlliedGigEAcquisitionMode_Recorder,
			EDeviceAlliedGigEAcquisitionMode_Count,
		};

		enum EDeviceAlliedGigEColorTransformationMode
		{
			EDeviceAlliedGigEColorTransformationMode_Off = 0,
			EDeviceAlliedGigEColorTransformationMode_Manual,
			EDeviceAlliedGigEColorTransformationMode_Temp6500K,
			EDeviceAlliedGigEColorTransformationMode_Count,
		};

		enum EDeviceAlliedGigEExposureMode
		{
			EDeviceAlliedGigEExposureMode_Manual = 0,
			EDeviceAlliedGigEExposureMode_AutoOnce,
			EDeviceAlliedGigEExposureMode_Auto,
			EDeviceAlliedGigEExposureMode_External,
			EDeviceAlliedGigEExposureMode_Count,
		};

		enum EDeviceAlliedGigEExposureAutoAlg
		{
			EDeviceAlliedGigEExposureAutoAlg_Mean = 0,
			EDeviceAlliedGigEExposureAutoAlg_FitRange,
			EDeviceAlliedGigEExposureAutoAlg_Count,
		};

		enum EDeviceAlliedGigEGainMode
		{
			EDeviceAlliedGigEGainMode_Manual = 0,
			EDeviceAlliedGigEGainMode_AutoOnce,
			EDeviceAlliedGigEGainMode_Auto,
			EDeviceAlliedGigEGainMode_Count,
		};

		enum EDeviceAlliedGigEIrisMode
		{
			EDeviceAlliedGigEIrisMode_Disabled = 0,
			EDeviceAlliedGigEIrisMode_Video,
			EDeviceAlliedGigEIrisMode_VideoOpen,
			EDeviceAlliedGigEIrisMode_VideoClose,
			EDeviceAlliedGigEIrisMode_Count,
		};

		enum EDeviceAlliedGigELUTMode
		{
			EDeviceAlliedGigELUTMode_Luminance = 0,
			EDeviceAlliedGigELUTMode_Red,
			EDeviceAlliedGigELUTMode_Green,
			EDeviceAlliedGigELUTMode_Blue,
			EDeviceAlliedGigELUTMode_Count,
		};

		enum EDeviceAlliedGigELUTSelector
		{
			EDeviceAlliedGigELUTSelector_LUT1 = 0,
			EDeviceAlliedGigELUTSelector_LUT2,
			EDeviceAlliedGigELUTSelector_LUT3,
			EDeviceAlliedGigELUTSelector_Count,
		};

		enum EDeviceAlliedGigEEdgeFilter
		{
			EDeviceAlliedGigEEdgeFilter_Smooth2 = 0,
			EDeviceAlliedGigEEdgeFilter_Smooth1,
			EDeviceAlliedGigEEdgeFilter_Off,
			EDeviceAlliedGigEEdgeFilter_Sharpen1,
			EDeviceAlliedGigEEdgeFilter_Sharpen2,
			EDeviceAlliedGigEEdgeFilter_Count,
		};

		enum EDeviceAlliedGigEWhitebalMode
		{
			EDeviceAlliedGigEWhitebalMode_Manual = 0,
			EDeviceAlliedGigEWhitebalMode_AutoOnce,
			EDeviceAlliedGigEWhitebalMode_Auto,
			EDeviceAlliedGigEWhitebalMode_Count,
		};

		enum EDeviceAlliedGigEBandwidthCtrlMode
		{
			EDeviceAlliedGigEBandwidthCtrlMode_StreamBytesPerSecond = 0,
			EDeviceAlliedGigEBandwidthCtrlMode_SCPD,
			EDeviceAlliedGigEBandwidthCtrlMode_Both,
			EDeviceAlliedGigEBandwidthCtrlMode_Count,
		};

		enum EDeviceAlliedGigEGvspSocketBuffersCount
		{
			EDeviceAlliedGigEGvspSocketBuffersCount_256 = 0,
			EDeviceAlliedGigEGvspSocketBuffersCount_512,
			EDeviceAlliedGigEGvspSocketBuffersCount_1024,
			EDeviceAlliedGigEGvspSocketBuffersCount_2048,
			EDeviceAlliedGigEGvspSocketBuffersCount_4096,
			EDeviceAlliedGigEGvspSocketBuffersCount_8192,
			EDeviceAlliedGigEGvspSocketBuffersCount_Count,
		};

		enum EDeviceAlliedGigEStrobeMode
		{
			EDeviceAlliedGigEStrobeMode_AcquisitionTriggerReady = 0,
			EDeviceAlliedGigEStrobeMode_FrameTriggerReady,
			EDeviceAlliedGigEStrobeMode_FrameTrigger,
			EDeviceAlliedGigEStrobeMode_Exposing,
			EDeviceAlliedGigEStrobeMode_FrameReadout,
			EDeviceAlliedGigEStrobeMode_Acquiring,
			EDeviceAlliedGigEStrobeMode_SyncIn1,
			EDeviceAlliedGigEStrobeMode_SyncIn2,
			EDeviceAlliedGigEStrobeMode_Count,
		};


		enum EDeviceAlliedGigESyncOutMode
		{
			EDeviceAlliedGigESyncOutMode_GPO = 0,
			EDeviceAlliedGigESyncOutMode_AcquisitionTriggerReady,
			EDeviceAlliedGigESyncOutMode_FrameTriggerReady,
			EDeviceAlliedGigESyncOutMode_FrameTrigger,
			EDeviceAlliedGigESyncOutMode_Exposing,
			EDeviceAlliedGigESyncOutMode_FrameReadout,
			EDeviceAlliedGigESyncOutMode_Imaging,
			EDeviceAlliedGigESyncOutMode_Acquiring,
			EDeviceAlliedGigESyncOutMode_SyncIn1,
			EDeviceAlliedGigESyncOutMode_SyncIn2,
			EDeviceAlliedGigESyncOutMode_Strobe1,
			EDeviceAlliedGigESyncOutMode_Count,
		};


		enum EDeviceAlliedGigESwitch
		{
			EDeviceAlliedGigESwitch_Off = 0,
			EDeviceAlliedGigESwitch_On,
			EDeviceAlliedGigESwitch_Count,
		};
	}
}

namespace Ravid
{
	namespace Device
	{
		enum EDeviceParameterMNETG
		{
			EDeviceParameterMNETG_DeviceID = 0,
			EDeviceParameterMNETG_Channel,
			EDeviceParameterMNETG_Station,
			EDeviceParameterMNETG_Network,
			EDeviceParameterMNETG_Count,
		};
	}
}

namespace Ravid
{
	namespace Device
	{
		enum ECrevisGigEGetFunction
		{
			ECrevisGigEGetFunction_OK = 0,
			ECrevisGigEGetFunction_UnknownError,
			ECrevisGigEGetFunction_EnumTypeError,
			ECrevisGigEGetFunction_NullptrError,
		};

		enum ECrevisGigESetFunction
		{
			ECrevisGigESetFunction_OK = 0,
			ECrevisGigESetFunction_UnknownError,
			ECrevisGigESetFunction_AlreadyGrabError,
			ECrevisGigESetFunction_NotFoundApiError,
			ECrevisGigESetFunction_AlreadyInitializedError,
			ECrevisGigESetFunction_NotInitializedError,
			ECrevisGigESetFunction_NotFindCommandError,
			ECrevisGigESetFunction_DeviceNullptrError,
			ECrevisGigESetFunction_NotSupportError,
			ECrevisGigESetFunction_FunctionLockedError,
			ECrevisGigESetFunction_WriteToDeviceError,
			ECrevisGigESetFunction_WriteToDatabaseError,
			ECrevisGigESetFunction_ReadOnDatabaseError,
			ECrevisGigESetFunction_ReadOnDeviceError,
			ECrevisGigESetFunction_ActivatedDeviceError,
		};

		enum EDeviceParameterCrevisGigE
		{
			EDeviceParameterCrevisGigE_DeviceID = 0,
			EDeviceParameterCrevisGigE_GeneralParameter,
			EDeviceParameterCrevisGigE_InitializeMode,
			EDeviceParameterCrevisGigE_AcqInvalidTime,
			EDeviceParameterCrevisGigE_GrabTimeout,
			EDeviceParameterCrevisGigE_DetailedLog,
			EDeviceParameterCrevisGigE_GevHeartbeatTimeout,
			EDeviceParameterCrevisGigE_GevGVCPHeartbeatDisable,
			EDeviceParameterCrevisGigE_GevSCPSPacketSize,
			EDeviceParameterCrevisGigE_ImageControl,
			EDeviceParameterCrevisGigE_Width,
			EDeviceParameterCrevisGigE_Height,
			EDeviceParameterCrevisGigE_PixelFormat,
			EDeviceParameterCrevisGigE_OffsetX,
			EDeviceParameterCrevisGigE_OffsetY,
			EDeviceParameterCrevisGigE_TestPattern,
			EDeviceParameterCrevisGigE_AcquisitionControl,
			EDeviceParameterCrevisGigE_AcquisitionMode,
			EDeviceParameterCrevisGigE_AcquisitionFrameCount,
			EDeviceParameterCrevisGigE_TriggerSelector,
			EDeviceParameterCrevisGigE_TriggerSource,
			EDeviceParameterCrevisGigE_Line1,
			EDeviceParameterCrevisGigE_Line1TriggerMode,
			EDeviceParameterCrevisGigE_Line1TriggerActivation,
			EDeviceParameterCrevisGigE_Line1TriggerDelay,
			EDeviceParameterCrevisGigE_Software,
			EDeviceParameterCrevisGigE_SoftwareTriggerMode,
			EDeviceParameterCrevisGigE_SoftwareTriggerActivation,
			EDeviceParameterCrevisGigE_SoftwareTriggerDelay,
			EDeviceParameterCrevisGigE_ExposureMode,
			EDeviceParameterCrevisGigE_ExposureAuto,
			EDeviceParameterCrevisGigE_ExposureTime,
			EDeviceParameterCrevisGigE_AcquisitionFrameRateEnable,
			EDeviceParameterCrevisGigE_AcquisitionFrameRate,
			EDeviceParameterCrevisGigE_AutoExposureTarget,
			EDeviceParameterCrevisGigE_DigitalIOControl,
			EDeviceParameterCrevisGigE_LineSelector,
			EDeviceParameterCrevisGigE_LineInverter,
			EDeviceParameterCrevisGigE_LineSource,
			EDeviceParameterCrevisGigE_UserOutputSelector,
			EDeviceParameterCrevisGigE_TimerSelector,
			EDeviceParameterCrevisGigE_TimerDuration,
			EDeviceParameterCrevisGigE_TimerDelay,
			EDeviceParameterCrevisGigE_LUTControl,
			EDeviceParameterCrevisGigE_LUTEnable,
			EDeviceParameterCrevisGigE_LUTIndex,
			EDeviceParameterCrevisGigE_LUTValue,
			EDeviceParameterCrevisGigE_AnalogControl,
			EDeviceParameterCrevisGigE_Gainselector,
			EDeviceParameterCrevisGigE_GainRaw,
			EDeviceParameterCrevisGigE_GainAuto,
			EDeviceParameterCrevisGigE_BlackLevelSelector,
			EDeviceParameterCrevisGigE_BlackLevelRaw,
			EDeviceParameterCrevisGigE_BalanceRatioSelector,
			EDeviceParameterCrevisGigE_ColorOnly,
			EDeviceParameterCrevisGigE_BalanceRatio,
			EDeviceParameterCrevisGigE_BalanceWhiteAuto,
			EDeviceParameterCrevisGigE_UserControl,
			EDeviceParameterCrevisGigE_UserSetSelector,
			EDeviceParameterCrevisGigE_DeviceOptionControl,
			EDeviceParameterCrevisGigE_DeviceFilterDriverMode,
			EDeviceParameterCrevisGigE_DeviceCommandTimeout,
			EDeviceParameterCrevisGigE_DeviceCommandRetryNumber,
			EDeviceParameterCrevisGigE_DeviceMissingPacketReceive,
			EDeviceParameterCrevisGigE_DevicePacketResend,
			EDeviceParameterCrevisGigE_DeviceMaxPacketResendCount,
			EDeviceParameterCrevisGigE_ColorTransControl,
			EDeviceParameterCrevisGigE_ColorTransformationEnable,
			EDeviceParameterCrevisGigE_ColorTransformationValueSelector,
			EDeviceParameterCrevisGigE_ColorTransformationValue,

			EDeviceParameterCrevisGigE_Count,
		};

		enum EDeviceCrevisGigEInitMode
		{
			EDeviceCrevisGigEInitMode_DeviceOnly = 0,
			EDeviceCrevisGigEInitMode_Parameter,
			EDeviceCrevisGigEInitMode_Count,
		};

		enum EDeviceCrevisGigEPixelFormat
		{
			EDeviceCrevisGigEPixelFormat_Mono8 = 0,
			EDeviceCrevisGigEPixelFormat_Mono10,
			EDeviceCrevisGigEPixelFormat_Mono12,
			EDeviceCrevisGigEPixelFormat_Mono14,
			EDeviceCrevisGigEPixelFormat_Mono10Packed,
			EDeviceCrevisGigEPixelFormat_Mono12Packed,
			EDeviceCrevisGigEPixelFormat_BayerBG8,
			EDeviceCrevisGigEPixelFormat_BayerBG10,
			EDeviceCrevisGigEPixelFormat_BayerBG12,
			EDeviceCrevisGigEPixelFormat_BayerBG10Packed,
			EDeviceCrevisGigEPixelFormat_BayerBG12Packed,
			EDeviceCrevisGigEPixelFormat_BayerRG8,
			EDeviceCrevisGigEPixelFormat_BayerRG10,
			EDeviceCrevisGigEPixelFormat_BayerRG12,
			EDeviceCrevisGigEPixelFormat_BayerRG10Packed,
			EDeviceCrevisGigEPixelFormat_BayerRG12Packed,
			EDeviceCrevisGigEPixelFormat_BayerGR8,
			EDeviceCrevisGigEPixelFormat_BayerGR10,
			EDeviceCrevisGigEPixelFormat_BayerGR12,
			EDeviceCrevisGigEPixelFormat_BayerGR10Packed,
			EDeviceCrevisGigEPixelFormat_BayerGR12Packed,
			EDeviceCrevisGigEPixelFormat_BayerGB8,
			EDeviceCrevisGigEPixelFormat_BayerGB10,
			EDeviceCrevisGigEPixelFormat_BayerGB12,
			EDeviceCrevisGigEPixelFormat_BayerGB10Packed,
			EDeviceCrevisGigEPixelFormat_BayerGB12Packed,
			EDeviceCrevisGigEPixelFormat_Yuv422Packed,
			EDeviceCrevisGigEPixelFormat_RGB8Packed,
			EDeviceCrevisGigEPixelFormat_BGR8Packed,
			EDeviceCrevisGigEPixelFormat_Count,
		};

		enum EDeviceCrevisGigETestPattern
		{
			EDeviceCrevisGigETestPattern_Off = 0,
			EDeviceCrevisGigETestPattern_GreyHorizontalRamp,
			EDeviceCrevisGigETestPattern_GreyVericalRamp,
			EDeviceCrevisGigETestPattern_Count,
		};

		enum EDeviceCrevisGigEAcquistionMode
		{
			EDeviceCrevisGigEAcquistionMode_Continuous = 0,
			EDeviceCrevisGigEAcquistionMode_SingleFrame,
			EDeviceCrevisGigEAcquistionMode_MultiFrame,
			EDeviceCrevisGigEAcquistionMode_Count,
		};

		enum EDeviceCrevisGigETriggerSelector
		{
			EDeviceCrevisGigETriggerSelector_FrameStart = 0,
			EDeviceCrevisGigETriggerSelector_Count,
		};

		enum EDeviceCrevisGigETriggerMode
		{
			EDeviceCrevisGigETriggerMode_Off = 0,
			EDeviceCrevisGigETriggerMode_On,
			EDeviceCrevisGigETriggerMode_Count,
		};

		enum EDeviceCrevisGigETriggerSource
		{
			EDeviceCrevisGigETriggerSource_Line1 = 0,
			EDeviceCrevisGigETriggerSource_Software,
			EDeviceCrevisGigETriggerSource_Count,
		};

		enum EDeviceCrevisGigETriggerActivation
		{
			EDeviceCrevisGigETriggerActivation_RisingEdge = 0,
			EDeviceCrevisGigETriggerActivation_FallingEdge,
			EDeviceCrevisGigETriggerActivation_LevelLow,
			EDeviceCrevisGigETriggerActivation_LevelHigh,
			EDeviceCrevisGigETriggerActivation_Count,
		};

		enum EDeviceCrevisGigEExposureMode
		{
			EDeviceCrevisGigEExposureMode_Timed = 0,
			EDeviceCrevisGigEExposureMode_TriggerWidth,
			EDeviceCrevisGigEExposureMode_Count,
		};

		enum EDeviceCrevisGigEExposureAuto
		{
			EDeviceCrevisGigEExposureAuto_Off = 0,
			EDeviceCrevisGigEExposureAuto_Once,
			EDeviceCrevisGigEExposureAuto_Continuous,
			EDeviceCrevisGigEExposureAuto_Count,
		};

		enum EDeviceCrevisGigEAcquistionFramerateEnable
		{
			EDeviceCrevisGigEAcquistionFramerateEnable_Off = 0,
			EDeviceCrevisGigEAcquistionFramerateEnable_On,
			EDeviceCrevisGigEAcquistionFramerateEnable_Count,
		};

		enum EDeviceCrevisGigELineSelector
		{
			EDeviceCrevisGigELineSelector_Line1 = 0,
			EDeviceCrevisGigELineSelector_Line2,
			EDeviceCrevisGigELineSelector_Count,
		};

		enum EDeviceCrevisGigELineMode
		{
			EDeviceCrevisGigELineMode_Input = 0,
			EDeviceCrevisGigELineMode_Output,
			EDeviceCrevisGigELineMode_Count,
		};

		enum EDeviceCrevisGigELineSource
		{
			EDeviceCrevisGigELineSource_Off = 0,
			EDeviceCrevisGigELineSource_ExposureActive,
			EDeviceCrevisGigELineSource_TimerActive,
			EDeviceCrevisGigELineSource_UserOutput1,
			EDeviceCrevisGigELineSource_Count,
		};

		enum EDeviceCrevisGigEUserOutputSelector
		{
			EDeviceCrevisGigEUserOutputSelector_UserOutput1 = 0,
			EDeviceCrevisGigEUserOutputSelector_Count,
		};

		enum EDeviceCrevisGigETimerSelector
		{
			EDeviceCrevisGigETimerSelector_Timer1 = 0,
			EDeviceCrevisGigETimerSelector_Count,
		};

		enum EDeviceCrevisGigEGainSelector
		{
			EDeviceCrevisGigEGainSelector_All = 0,
			EDeviceCrevisGigEGainSelector_Count,
		};

		enum EDeviceCrevisGigEGainAuto
		{
			EDeviceCrevisGigEGainAuto_Off = 0,
			EDeviceCrevisGigEGainAuto_Once,
			EDeviceCrevisGigEGainAuto_Continuous,
			EDeviceCrevisGigEGainAuto_Count,
		};

		enum EDeviceCrevisGigEBlackLevelSelector
		{
			EDeviceCrevisGigEBlackLevelSelector_All = 0,
			EDeviceCrevisGigEBlackLevelSelector_Count,
		};

		enum EDeviceCrevisGigEBalanceRatioSelector
		{
			EDeviceCrevisGigEBalanceRatioSelector_Red = 0,
			EDeviceCrevisGigEBalanceRatioSelector_Green,
			EDeviceCrevisGigEBalanceRatioSelector_Blue,
			EDeviceCrevisGigEBalanceRatioSelector_Count,
		};

		enum EDeviceCrevisGigEBalanceWhiteAuto
		{
			EDeviceCrevisGigEBalanceWhiteAuto_Off = 0,
			EDeviceCrevisGigEBalanceWhiteAuto_Once,
			EDeviceCrevisGigEBalanceWhiteAuto_Continuous,
			EDeviceCrevisGigEBalanceWhiteAuto_Count,
		};

		enum EDeviceCrevisGigEGevDeviceModeCharacterSet
		{
			EDeviceCrevisGigEGevDeviceModeCharacterSet_UTF8 = 0,
			EDeviceCrevisGigEGevDeviceModeCharacterSet_Count,
		};

		enum EDeviceCrevisGigEGevSupportedOptionSelector
		{
			EDeviceCrevisGigEGevSupportedOptionSelector_UserDefinedName = 0,
			EDeviceCrevisGigEGevSupportedOptionSelector_SerialNumber,
			EDeviceCrevisGigEGevSupportedOptionSelector_HeartBeatDisable,
			EDeviceCrevisGigEGevSupportedOptionSelector_LinkSpeed,
			EDeviceCrevisGigEGevSupportedOptionSelector_CCPApplicationSocket,
			EDeviceCrevisGigEGevSupportedOptionSelector_ManifestTable,
			EDeviceCrevisGigEGevSupportedOptionSelector_TestData,
			EDeviceCrevisGigEGevSupportedOptionSelector_DiscoveryAckDelay,
			EDeviceCrevisGigEGevSupportedOptionSelector_DiscoveryAckDelayWritable,
			EDeviceCrevisGigEGevSupportedOptionSelector_ExtendedStatusCodes,
			EDeviceCrevisGigEGevSupportedOptionSelector_PrimaryApplicationSwitchOver,
			EDeviceCrevisGigEGevSupportedOptionSelector_Action,
			EDeviceCrevisGigEGevSupportedOptionSelector_PendingAck,
			EDeviceCrevisGigEGevSupportedOptionSelector_EventData,
			EDeviceCrevisGigEGevSupportedOptionSelector_Event,
			EDeviceCrevisGigEGevSupportedOptionSelector_PacketResend,
			EDeviceCrevisGigEGevSupportedOptionSelector_WriteMem,
			EDeviceCrevisGigEGevSupportedOptionSelector_CommandsConcatenation,
			EDeviceCrevisGigEGevSupportedOptionSelector_IpConfigrationLLA,
			EDeviceCrevisGigEGevSupportedOptionSelector_IpConfigrationDHCP,
			EDeviceCrevisGigEGevSupportedOptionSelector_IpConfigrationPersistentIP,
			EDeviceCrevisGigEGevSupportedOptionSelector_StreamChannelSourceSocket,
			EDeviceCrevisGigEGevSupportedOptionSelector_MessageChannelSourceSocket,
			EDeviceCrevisGigEGevSupportedOptionSelector_StreamChnnel0BigAndLittleEndian,
			EDeviceCrevisGigEGevSupportedOptionSelector_StreamChnnel0IPReassembly,
			EDeviceCrevisGigEGevSupportedOptionSelector_StreamChnnel0UnconditionalStreaming,
			EDeviceCrevisGigEGevSupportedOptionSelector_StreamChnnel0ExtendedChunkData,
			EDeviceCrevisGigEGevSupportedOptionSelector_Count,
		};

		enum EDeviceCrevisGigEGevCCP
		{
			EDeviceCrevisGigEGevCCP_OpenAccess = 0,
			EDeviceCrevisGigEGevCCP_ExclusiveAccess,
			EDeviceCrevisGigEGevCCP_ControlAccess,
			EDeviceCrevisGigEGevCCP_Count,
		};

		enum EDeviceCrevisGigEUserSetSelector
		{
			EDeviceCrevisGigEUserSetSelector_Default = 0,
			EDeviceCrevisGigEUserSetSelector_UserSet1,
			EDeviceCrevisGigEUserSetSelector_UserSet2,
			EDeviceCrevisGigEUserSetSelector_UserSet3,
			EDeviceCrevisGigEUserSetSelector_Count,
		};

		enum EDeviceCrevisGigEColorTransformationValueSeletor
		{
			EDeviceCrevisGigEColorTransformationValueSeletor_HUEBYGP = 0,
			EDeviceCrevisGigEColorTransformationValueSeletor_HUEBYGN,
			EDeviceCrevisGigEColorTransformationValueSeletor_HUEBYHP,
			EDeviceCrevisGigEColorTransformationValueSeletor_HUEBYHN,
			EDeviceCrevisGigEColorTransformationValueSeletor_HUERYGP,
			EDeviceCrevisGigEColorTransformationValueSeletor_HUERYGN,
			EDeviceCrevisGigEColorTransformationValueSeletor_HUERYHP,
			EDeviceCrevisGigEColorTransformationValueSeletor_HUERYHN,
			EDeviceCrevisGigEColorTransformationValueSeletor_HUECG,
			EDeviceCrevisGigEColorTransformationValueSeletor_Count,
		};

		enum EDeviceCrevisGigEDeviceFilterDriverMode
		{
			EDeviceCrevisGigEDeviceFilterDriverMode_Off = 0,
			EDeviceCrevisGigEDeviceFilterDriverMode_On,
			EDeviceCrevisGigEDeviceFilterDriverMode_Count,
		};

		enum EDeviceCrevisGigEDeviceMissingPackedReceive
		{
			EDeviceCrevisGigEDeviceMissingPackedReceive_Off = 0,
			EDeviceCrevisGigEDeviceMissingPackedReceive_On,
			EDeviceCrevisGigEDeviceMissingPackedReceive_Count,
		};

		enum EDeviceCrevisGigESwitch
		{
			EDeviceCrevisGigESwitch_Off = 0,
			EDeviceCrevisGigESwitch_On,
			EDeviceCrevisGigESwitch_Count,
		};

		enum EDeviceCrevisGigEColorModel
		{
			EDeviceCrevisGigEColorModel_MVKH20G = 0,
			EDeviceCrevisGigEColorModel_MGD500C,
			EDeviceCrevisGigEColorModel_MGD200CS,
			EDeviceCrevisGigEColorModel_MGD200C,
			EDeviceCrevisGigEColorModel_MGD130C,
			EDeviceCrevisGigEColorModel_MGD030C,
			EDeviceCrevisGigEColorModel_MGD030CS,
			EDeviceCrevisGigEColorModel_MVKQ60GP,
			EDeviceCrevisGigEColorModel_MGA320K35,
			EDeviceCrevisGigEColorModel_MGA500K22,
			EDeviceCrevisGigEColorModel_MGA121K9,
			EDeviceCrevisGigEColorModel_MGA121L9,
			EDeviceCrevisGigEColorModel_MGA201L5,
			EDeviceCrevisGigEColorModel_MGA040K280,
			EDeviceCrevisGigEColorModel_MGA160K72,
			EDeviceCrevisGigEColorModel_Count,
		};
	}
}

namespace Ravid
{
	namespace Device
	{
		enum EDeviceParameterGenICam
		{
			EDeviceParameterGenICam_DeviceID = 0,
			EDeviceParameterGenICam_InitializeMode,
			EDeviceParameterGenICam_CtiFilePath,
			EDeviceParameterGenICam_AnalogControl,
			EDeviceParameterGenICam_GainAuto,
			EDeviceParameterGenICam_GainSelector,
			EDeviceParameterGenICam_GainRaw,
			EDeviceParameterGenICam_BlackLevelSelector,
			EDeviceParameterGenICam_BlackLevelRaw,
			EDeviceParameterGenICam_BalanceWhiteAuto,
			EDeviceParameterGenICam_BalanceRatioSelector,
			EDeviceParameterGenICam_BalanceRatioAbs,
			EDeviceParameterGenICam_BalanceRatioRaw,
			EDeviceParameterGenICam_GammaEnable,
			EDeviceParameterGenICam_Gamma,
			EDeviceParameterGenICam_DigitalShift,
			EDeviceParameterGenICam_ImageFormat,
			EDeviceParameterGenICam_PixelFormat,
			EDeviceParameterGenICam_ReverseX,
			EDeviceParameterGenICam_ReverseY,
			EDeviceParameterGenICam_SensorWidth,
			EDeviceParameterGenICam_SensorHeight,
			EDeviceParameterGenICam_AOI,
			EDeviceParameterGenICam_Width,
			EDeviceParameterGenICam_Height,
			EDeviceParameterGenICam_OffsetX,
			EDeviceParameterGenICam_OffsetY,
			EDeviceParameterGenICam_BinningVertical,
			EDeviceParameterGenICam_BinningHorizontal,
			EDeviceParameterGenICam_LegacyBinningVertical,
			EDeviceParameterGenICam_AcquisitionTrigger,
			EDeviceParameterGenICam_EnableBurstAcquisition,
			EDeviceParameterGenICam_AcquisitionMode,
			//////////////////////////////////////////////////////////////////////////
			// 트리거 종류에 따라 아래 갯수 늘어남
			EDeviceParameterGenICam_TriggerSelector,
			EDeviceParameterGenICam_TriggerMode,
			EDeviceParameterGenICam_TriggerSource,
			EDeviceParameterGenICam_TriggerActivation,
			EDeviceParameterGenICam_TriggerDelayAbs,
			//////////////////////////////////////////////////////////////////////////
			EDeviceParameterGenICam_ExposureAuto,
			EDeviceParameterGenICam_ExposureMode,
			EDeviceParameterGenICam_ExposureTimeRaw,
			EDeviceParameterGenICam_ExposureTimeAbs,
			EDeviceParameterGenICam_ExposureTimeBaseAbs,
			EDeviceParameterGenICam_ExposureTimeBaseAbsEnable,
			EDeviceParameterGenICam_AcquisitionLineRateRaw,
			EDeviceParameterGenICam_AcquisitionLineRateAbs,
			EDeviceParameterGenICam_ResultingLineRateAbs,
			EDeviceParameterGenICam_AcquisitionFrameRate,
			EDeviceParameterGenICam_AcquisitionFrameRateEnable,
			EDeviceParameterGenICam_ResultingFrameRateAbs,
			EDeviceParameterGenICam_AcquisitionStatusSelector,
			EDeviceParameterGenICam_AcquisitionStatus,
			EDeviceParameterGenICam_DigitalIO,
			//////////////////////////////////////////////////////////////////////////
			// Line 종류에 따라 아래 수가 달라짐
			EDeviceParameterGenICam_LineSelector,
			EDeviceParameterGenICam_Line1,
			EDeviceParameterGenICam_Line1LineMode,
			EDeviceParameterGenICam_Line1LineSource,
			EDeviceParameterGenICam_Line1LineFormat,
			EDeviceParameterGenICam_Line1LineInverter,
			EDeviceParameterGenICam_Line1LineTermination,
			EDeviceParameterGenICam_Line1LineDebouncerTimeRaw,
			EDeviceParameterGenICam_Line1LineDebouncerTimeAbs,
			EDeviceParameterGenICam_Line2,
			EDeviceParameterGenICam_Line2LineMode,
			EDeviceParameterGenICam_Line2LineSource,
			EDeviceParameterGenICam_Line2LineFormat,
			EDeviceParameterGenICam_Line2LineInverter,
			EDeviceParameterGenICam_Line2LineTermination,
			EDeviceParameterGenICam_Line2LineDebouncerTimeRaw,
			EDeviceParameterGenICam_Line2LineDebouncerTimeAbs,
			EDeviceParameterGenICam_Out1,
			EDeviceParameterGenICam_Out1LineMode,
			EDeviceParameterGenICam_Out1LineSource,
			EDeviceParameterGenICam_Out1LineFormat,
			EDeviceParameterGenICam_Out1LineInverter,
			EDeviceParameterGenICam_Out1LineTermination,
			EDeviceParameterGenICam_Out1LineDebouncerTimeRaw,
			EDeviceParameterGenICam_Out1LineDebouncerTimeAbs,
			EDeviceParameterGenICam_Out2,
			EDeviceParameterGenICam_Out2LineMode,
			EDeviceParameterGenICam_Out2LineSource,
			EDeviceParameterGenICam_Out2LineFormat,
			EDeviceParameterGenICam_Out2LineInverter,
			EDeviceParameterGenICam_Out2LineTermination,
			EDeviceParameterGenICam_Out2LineDebouncerTimeRaw,
			EDeviceParameterGenICam_Out2LineDebouncerTimeAbs,
			EDeviceParameterGenICam_Out3,
			EDeviceParameterGenICam_Out3LineMode,
			EDeviceParameterGenICam_Out3LineSource,
			EDeviceParameterGenICam_Out3LineFormat,
			EDeviceParameterGenICam_Out3LineInverter,
			EDeviceParameterGenICam_Out3LineTermination,
			EDeviceParameterGenICam_Out3LineDebouncerTimeRaw,
			EDeviceParameterGenICam_Out3LineDebouncerTimeAbs,
			EDeviceParameterGenICam_Out4,
			EDeviceParameterGenICam_Out4LineMode,
			EDeviceParameterGenICam_Out4LineSource,
			EDeviceParameterGenICam_Out4LineFormat,
			EDeviceParameterGenICam_Out4LineInverter,
			EDeviceParameterGenICam_Out4LineTermination,
			EDeviceParameterGenICam_Out4LineDebouncerTimeRaw,
			EDeviceParameterGenICam_Out4LineDebouncerTimeAbs,
			//////////////////////////////////////////////////////////////////////////
			EDeviceParameterGenICam_UserOutputSelector,
			EDeviceParameterGenICam_TimerControls,
			EDeviceParameterGenICam_TimerDurationTimebaseAbs,
			EDeviceParameterGenICam_TimerDelayTimebaseAbs,
			EDeviceParameterGenICam_TimerSelector,
			EDeviceParameterGenICam_TimerDurationAbs,
			EDeviceParameterGenICam_TimerDurationRaw,
			EDeviceParameterGenICam_TimerDelayAbs,
			EDeviceParameterGenICam_TimerDelayRaw,
			EDeviceParameterGenICam_TimerTriggerSource,
			EDeviceParameterGenICam_TimerTriggerActivation,
			EDeviceParameterGenICam_TimerSequence,
			EDeviceParameterGenICam_TimerSequenceEnable,
			EDeviceParameterGenICam_TimerSequenceLastEntryIndex,
			EDeviceParameterGenICam_TimerSequenceCurrentEntryIndex,
			EDeviceParameterGenICam_TimerSequenceEntrySelector,
			EDeviceParameterGenICam_TimerSequenceTimerSelector,
			EDeviceParameterGenICam_TimerSequenceTimerEnable,
			EDeviceParameterGenICam_TimerSequenceTimerInverter,
			EDeviceParameterGenICam_TimerSequenceTimerDelayRaw,
			EDeviceParameterGenICam_TimerSequenceTimerDurationRaw,
			EDeviceParameterGenICam_CounterSelector,
			EDeviceParameterGenICam_CounterEventSource,
			EDeviceParameterGenICam_CounterResetSource,
			EDeviceParameterGenICam_LUTControls,
			EDeviceParameterGenICam_LUTEnable,
			EDeviceParameterGenICam_LUTIndex,
			EDeviceParameterGenICam_LUTValue,
			EDeviceParameterGenICam_LUTSelector,
			EDeviceParameterGenICam_TransportLayer,
			EDeviceParameterGenICam_GevLinkSpeed,
			EDeviceParameterGenICam_GevHeartbeatTimeout,
			EDeviceParameterGenICam_GevSCPSPacketSize,
			EDeviceParameterGenICam_Count,
		};

		enum EDeviceGenICamInitMode
		{
			EDeviceGenICamInitMode_DeviceOnly = 0,
			EDeviceGenICamInitMode_Parameter,
			EDeviceGenICamInitMode_Count,
		};

		enum EDeviceGenICamGainAuto
		{
			EDeviceGenICamGainAuto_Off = 0,
			EDeviceGenICamGainAuto_Once,
			EDeviceGenICamGainAuto_Continuous,
			EDeviceGenICamGainAuto_Count,
		};

		enum EDeviceGenICamGainSelector
		{
			EDeviceGenICamGainSelector_All = 0,
			EDeviceGenICamGainSelector_Red,
			EDeviceGenICamGainSelector_Green,
			EDeviceGenICamGainSelector_Blue,
			EDeviceGenICamGainSelector_Y,
			EDeviceGenICamGainSelector_U,
			EDeviceGenICamGainSelector_V,
			EDeviceGenICamGainSelector_Tap1,
			EDeviceGenICamGainSelector_Tap2,
			EDeviceGenICamGainSelector_AnalogAll,
			EDeviceGenICamGainSelector_AnalogRed,
			EDeviceGenICamGainSelector_AnalogGreen,
			EDeviceGenICamGainSelector_AnalogBlue,
			EDeviceGenICamGainSelector_AnalogY,
			EDeviceGenICamGainSelector_AnalogU,
			EDeviceGenICamGainSelector_AnalogV,
			EDeviceGenICamGainSelector_AnalogTap1,
			EDeviceGenICamGainSelector_AnalogTap2,
			EDeviceGenICamGainSelector_DigitalAll,
			EDeviceGenICamGainSelector_DigitalRed,
			EDeviceGenICamGainSelector_DigitalGreen,
			EDeviceGenICamGainSelector_DigitalBlue,
			EDeviceGenICamGainSelector_DigitalY,
			EDeviceGenICamGainSelector_DigitalU,
			EDeviceGenICamGainSelector_DigitalV,
			EDeviceGenICamGainSelector_DigitalTap1,
			EDeviceGenICamGainSelector_DigitalTap2,
			EDeviceGenICamGainSelector_Count,
		};

		enum EDeviceGenICamBlackLevelSelector
		{
			EDeviceGenICamBlackLevelSelector_All = 0,
			EDeviceGenICamBlackLevelSelector_Red,
			EDeviceGenICamBlackLevelSelector_Green,
			EDeviceGenICamBlackLevelSelector_Blue,
			EDeviceGenICamBlackLevelSelector_Y,
			EDeviceGenICamBlackLevelSelector_U,
			EDeviceGenICamBlackLevelSelector_V,
			EDeviceGenICamBlackLevelSelector_Tap1,
			EDeviceGenICamBlackLevelSelector_Tap2,
			EDeviceGenICamBlackLevelSelector_Count,
		};

		enum EDeviceGenICamBalanceWhiteAuto
		{
			EDeviceGenICamBalanceWhiteAuto_Off = 0,
			EDeviceGenICamBalanceWhiteAuto_Once,
			EDeviceGenICamBalanceWhiteAuto_Continuous,
			EDeviceGenICamBalanceWhiteAuto_Count,
		};

		enum EDeviceGenICamBalanceRatioSelector
		{
			EDeviceGenICamBalanceRatioSelector_All = 0,
			EDeviceGenICamBalanceRatioSelector_Red,
			EDeviceGenICamBalanceRatioSelector_Green,
			EDeviceGenICamBalanceRatioSelector_Blue,
			EDeviceGenICamBalanceRatioSelector_Y,
			EDeviceGenICamBalanceRatioSelector_U,
			EDeviceGenICamBalanceRatioSelector_V,
			EDeviceGenICamBalanceRatioSelector_Tap1,
			EDeviceGenICamBalanceRatioSelector_Tap2,
			EDeviceGenICamBalanceRatioSelector_Count,
		};

		enum EDeviceGenICamGammaEnable
		{
			EDeviceGenICamGammaEnable_True = 0,
			EDeviceGenICamGammaEnable_False,
			EDeviceGenICamGammaEnable_Count,
		};

		enum EDeviceGenICamPixelFormat
		{
			EDeviceGenICamPixelFormat_Mono8 = 0,
			EDeviceGenICamPixelFormat_Mono10,
			EDeviceGenICamPixelFormat_Mono12,
			EDeviceGenICamPixelFormat_Mono14,
			EDeviceGenICamPixelFormat_Mono16,
			EDeviceGenICamPixelFormat_Mono10Packed,
			EDeviceGenICamPixelFormat_Mono12Packed,
			EDeviceGenICamPixelFormat_R8,
			EDeviceGenICamPixelFormat_G8,
			EDeviceGenICamPixelFormat_B8,
			EDeviceGenICamPixelFormat_RGB8,
			EDeviceGenICamPixelFormat_RGBa8,
			EDeviceGenICamPixelFormat_RGB10,
			EDeviceGenICamPixelFormat_RGB12,
			EDeviceGenICamPixelFormat_RGB16,
			EDeviceGenICamPixelFormat_BGR10,
			EDeviceGenICamPixelFormat_BGR12,
			EDeviceGenICamPixelFormat_BGR16,
			EDeviceGenICamPixelFormat_BGR8,
			EDeviceGenICamPixelFormat_BGRa8,
			EDeviceGenICamPixelFormat_YUV422_8,
			EDeviceGenICamPixelFormat_YCbCr411_8,
			EDeviceGenICamPixelFormat_YCbCr422_8,
			EDeviceGenICamPixelFormat_YCbCr601_422_8,
			EDeviceGenICamPixelFormat_YCbCr709_422_8,
			EDeviceGenICamPixelFormat_YCbCr8,
			EDeviceGenICamPixelFormat_BayerBG8,
			EDeviceGenICamPixelFormat_BayerGB8,
			EDeviceGenICamPixelFormat_BayerGR8,
			EDeviceGenICamPixelFormat_BayerRG8,
			EDeviceGenICamPixelFormat_BayerBG10,
			EDeviceGenICamPixelFormat_BayerGB10,
			EDeviceGenICamPixelFormat_BayerGR10,
			EDeviceGenICamPixelFormat_BayerRG10,
			EDeviceGenICamPixelFormat_BayerBG12,
			EDeviceGenICamPixelFormat_BayerGB12,
			EDeviceGenICamPixelFormat_BayerGR12,
			EDeviceGenICamPixelFormat_BayerRG12,
			EDeviceGenICamPixelFormat_BayerBG16,
			EDeviceGenICamPixelFormat_BayerGB16,
			EDeviceGenICamPixelFormat_BayerGR16,
			EDeviceGenICamPixelFormat_BayerRG16,
			EDeviceGenICamPixelFormat_Raw8,
			EDeviceGenICamPixelFormat_Raw16,
			EDeviceGenICamPixelFormat_BayerGR10Packed,
			EDeviceGenICamPixelFormat_BayerRG10Packed,
			EDeviceGenICamPixelFormat_BayerGB10Packed,
			EDeviceGenICamPixelFormat_BayerBG10Packed,
			EDeviceGenICamPixelFormat_BayerGR12Packed,
			EDeviceGenICamPixelFormat_BayerRG12Packed,
			EDeviceGenICamPixelFormat_BayerGB12Packed,
			EDeviceGenICamPixelFormat_BayerBG12Packed,
			EDeviceGenICamPixelFormat_RGB10V1Packed,
			EDeviceGenICamPixelFormat_RGB12V1Packed,
			EDeviceGenICamPixelFormat_RGB8Packed,
			EDeviceGenICamPixelFormat_BGR8Packed,
			EDeviceGenICamPixelFormat_RGBA8Packed,
			EDeviceGenICamPixelFormat_BGRA8Packed,
			EDeviceGenICamPixelFormat_RGB10Packed,
			EDeviceGenICamPixelFormat_BGR10Packed,
			EDeviceGenICamPixelFormat_RGB12Packed,
			EDeviceGenICamPixelFormat_BGR12Packed,
			EDeviceGenICamPixelFormat_RGB16Packed,
			EDeviceGenICamPixelFormat_BGR16Packed,
			EDeviceGenICamPixelFormat_RGB10V2Packed,
			EDeviceGenICamPixelFormat_BGR10V2Packed,
			EDeviceGenICamPixelFormat_RGB565Packed,
			EDeviceGenICamPixelFormat_BGR565Packed,
			EDeviceGenICamPixelFormat_YUV411Packed,
			EDeviceGenICamPixelFormat_YUV422Packed,
			EDeviceGenICamPixelFormat_YUV444Packed,
			EDeviceGenICamPixelFormat_YUYVPacked,
			EDeviceGenICamPixelFormat_Count,
		};

		enum EDeviceGenICamLegacyBinningVertical
		{
			EDeviceGenICamLegacyBinningVertical_Off = 0,
			EDeviceGenICamLegacyBinningVertical_Two_Rows,
			EDeviceGenICamLegacyBinningVertical_Count,
		};

		enum EDeviceGenICamAcquisitionMode
		{
			EDeviceGenICamAcquisitionMode_Continuous = 0,
			EDeviceGenICamAcquisitionMode_SingleFrame,
			EDeviceGenICamAcquisitionMode_MultiFrame,
			EDeviceGenICamAcquisitionMode_Count,
		};

		enum EDeviceGenICamTriggerSelector
		{
			EDeviceGenICamTriggerSelector_AcquisitionStart = 0,
			EDeviceGenICamTriggerSelector_AcquisitionEnd,
			EDeviceGenICamTriggerSelector_AcquisitionActive,
			EDeviceGenICamTriggerSelector_FrameStart,
			EDeviceGenICamTriggerSelector_FrameEnd,
			EDeviceGenICamTriggerSelector_FrameActive,
			EDeviceGenICamTriggerSelector_FrameBurstStart,
			EDeviceGenICamTriggerSelector_FrameBurstEnd,
			EDeviceGenICamTriggerSelector_FrameBurstActive,
			EDeviceGenICamTriggerSelector_LineStart,
			EDeviceGenICamTriggerSelector_ExposureStart,
			EDeviceGenICamTriggerSelector_ExposureEnd,
			EDeviceGenICamTriggerSelector_ExposureActive,
			EDeviceGenICamTriggerSelector_MultiSlopeExposureLimit1,
			EDeviceGenICamTriggerSelector_Count,
		};

		enum EDeviceGenICamTriggerMode
		{
			EDeviceGenICamTriggerMode_Off = 0,
			EDeviceGenICamTriggerMode_On,
			EDeviceGenICamTriggerMode_Count,
		};

		enum EDeviceGenICamTriggerSource
		{
			EDeviceGenICamTriggerSource_Line1 = 0,
			EDeviceGenICamTriggerSource_Line2,
			EDeviceGenICamTriggerSource_Line3,
			EDeviceGenICamTriggerSource_Line4,
			EDeviceGenICamTriggerSource_Software,
			EDeviceGenICamTriggerSource_Count,
		};

		enum EDeviceGenICamTriggerActivation
		{
			EDeviceGenICamTriggerActivation_RisingEdge = 0,
			EDeviceGenICamTriggerActivation_FallingEdge,
			EDeviceGenICamTriggerActivation_AnyEdge,
			EDeviceGenICamTriggerActivation_LevelHigh,
			EDeviceGenICamTriggerActivation_LevelLow,
			EDeviceGenICamTriggerActivation_Count,
		};

		enum EDeviceGenICamExposureAuto
		{
			EDeviceGenICamExposureAuto_Off = 0,
			EDeviceGenICamExposureAuto_Once,
			EDeviceGenICamExposureAuto_Continuous,
			EDeviceGenICamExposureAuto_Count,
		};

		enum EDeviceGenICamExposureMode
		{
			EDeviceGenICamExposureMode_Off = 0,
			EDeviceGenICamExposureMode_Timed,
			EDeviceGenICamExposureMode_TriggerWidth,
			EDeviceGenICamExposureMode_TriggerControlled,
			EDeviceGenICamExposureMode_Count,
		};

		enum EDeviceGenICamAcquisitionStatusSelector
		{
			EDeviceGenICamAcquisitionStatusSelector_AcquisitionTriggerWait = 0,
			EDeviceGenICamAcquisitionStatusSelector_AcquisitionActive,
			EDeviceGenICamAcquisitionStatusSelector_AcquisitionTransfer,
			EDeviceGenICamAcquisitionStatusSelector_FrameTriggerWait,
			EDeviceGenICamAcquisitionStatusSelector_FrameActive,
			EDeviceGenICamAcquisitionStatusSelector_FrameTransfer,
			EDeviceGenICamAcquisitionStatusSelector_ExposureActive,
			EDeviceGenICamAcquisitionStatusSelector_Count,
		};

		enum EDeviceGenICamLineSelector
		{
			EDeviceGenICamLineSelector_Line1 = 0,
			EDeviceGenICamLineSelector_Line2,
			EDeviceGenICamLineSelector_Out1,
			EDeviceGenICamLineSelector_Out2,
			EDeviceGenICamLineSelector_Out3,
			EDeviceGenICamLineSelector_Out4,
			EDeviceGenICamLineSelector_Count,
		};

		enum EDeviceGenICamLineMode
		{
			EDeviceGenICamLineMode_Input = 0,
			EDeviceGenICamLineMode_Output,
			EDeviceGenICamLineMode_Count,
		};

		enum EDeviceGenICamLineSource
		{
			EDeviceGenICamLineSource_Off = 0,
			EDeviceGenICamLineSource_UserOutput0,
			EDeviceGenICamLineSource_UserOutput1,
			EDeviceGenICamLineSource_UserOutput2,
			EDeviceGenICamLineSource_AcquisitionTriggerWait,
			EDeviceGenICamLineSource_AcquisitionTrigger,
			EDeviceGenICamLineSource_AcquisitionTriggerMissed,
			EDeviceGenICamLineSource_AcquisitionActive,
			EDeviceGenICamLineSource_FrameTriggerWait,
			EDeviceGenICamLineSource_FrameTrigger,
			EDeviceGenICamLineSource_FrameTriggerMissed,
			EDeviceGenICamLineSource_FrameActive,
			EDeviceGenICamLineSource_ExposureActive,
			EDeviceGenICamLineSource_LineTriggerWait,
			EDeviceGenICamLineSource_LineTrigger,
			EDeviceGenICamLineSource_LineTriggerMissed,
			EDeviceGenICamLineSource_LineActive,
			EDeviceGenICamLineSource_Counter0Active,
			EDeviceGenICamLineSource_Counter1Active,
			EDeviceGenICamLineSource_Counter2Active,
			EDeviceGenICamLineSource_Timer0Active,
			EDeviceGenICamLineSource_Timer1Active,
			EDeviceGenICamLineSource_Timer2Active,
			EDeviceGenICamLineSource_Encoder0,
			EDeviceGenICamLineSource_Encoder1,
			EDeviceGenICamLineSource_Encoder2,
			EDeviceGenICamLineSource_LogicBlock0,
			EDeviceGenICamLineSource_LogicBlock1,
			EDeviceGenICamLineSource_LogicBlock2,
			EDeviceGenICamLineSource_SoftwareSignal0,
			EDeviceGenICamLineSource_SoftwareSignal1,
			EDeviceGenICamLineSource_SoftwareSignal2,
			EDeviceGenICamLineSource_Stream0TransferActive,
			EDeviceGenICamLineSource_Stream1TransferActive,
			EDeviceGenICamLineSource_Stream2TransferActive,
			EDeviceGenICamLineSource_Stream0TransferPaused,
			EDeviceGenICamLineSource_Stream1TransferPaused,
			EDeviceGenICamLineSource_Stream2TransferPaused,
			EDeviceGenICamLineSource_Stream0TransferStopping,
			EDeviceGenICamLineSource_Stream1TransferStopping,
			EDeviceGenICamLineSource_Stream2TransferStopping,
			EDeviceGenICamLineSource_Stream0TransferStopped,
			EDeviceGenICamLineSource_Stream1TransferStopped,
			EDeviceGenICamLineSource_Stream2TransferStopped,
			EDeviceGenICamLineSource_Stream0TransferOverflow,
			EDeviceGenICamLineSource_Stream1TransferOverflow,
			EDeviceGenICamLineSource_Stream2TransferOverflow,
			EDeviceGenICamLineSource_Count,
		};

		enum EDeviceGenICamLineFormat
		{
			EDeviceGenICamLineFormat_NoConnect = 0,
			EDeviceGenICamLineFormat_TriState,
			EDeviceGenICamLineFormat_TTL,
			EDeviceGenICamLineFormat_LVDS,
			EDeviceGenICamLineFormat_RS422,
			EDeviceGenICamLineFormat_OptoCoupled,
			EDeviceGenICamLineFormat_OpenDrain,
			EDeviceGenICamLineFormat_Count,
		};

		enum EDeviceGenICamUserOutputSelector
		{
			EDeviceGenICamUserOutputSelector_UserOutput1 = 0,
			EDeviceGenICamUserOutputSelector_UserOutput2,
			EDeviceGenICamUserOutputSelector_UserOutput3,
			EDeviceGenICamUserOutputSelector_UserOutput4,
			EDeviceGenICamUserOutputSelector_Count,
		};

		enum EDeviceGenICamTimerSelector
		{
			EDeviceGenICamTimerSelector_Timer1 = 0,
			EDeviceGenICamTimerSelector_Timer2,
			EDeviceGenICamTimerSelector_Timer3,
			EDeviceGenICamTimerSelector_Timer4,
			EDeviceGenICamTimerSelector_Count,
		};

		enum EDeviceGenICamTimerTriggerSource
		{
			EDeviceGenICamTimerTriggerSource_Off = 0,
			EDeviceGenICamTimerTriggerSource_AcquisitionTrigger,
			EDeviceGenICamTimerTriggerSource_AcquisitionTriggerMissed,
			EDeviceGenICamTimerTriggerSource_AcquisitionStart,
			EDeviceGenICamTimerTriggerSource_AcquisitionEnd,
			EDeviceGenICamTimerTriggerSource_FrameTrigger,
			EDeviceGenICamTimerTriggerSource_FrameTriggerMissed,
			EDeviceGenICamTimerTriggerSource_FrameStart,
			EDeviceGenICamTimerTriggerSource_FrameEnd,
			EDeviceGenICamTimerTriggerSource_FrameBurstStart,
			EDeviceGenICamTimerTriggerSource_FrameBurstEnd,
			EDeviceGenICamTimerTriggerSource_LineTrigger,
			EDeviceGenICamTimerTriggerSource_LineTriggerMissed,
			EDeviceGenICamTimerTriggerSource_LineStart,
			EDeviceGenICamTimerTriggerSource_LineEnd,
			EDeviceGenICamTimerTriggerSource_ExposureStart,
			EDeviceGenICamTimerTriggerSource_ExposureEnd,
			EDeviceGenICamTimerTriggerSource_Line0,
			EDeviceGenICamTimerTriggerSource_Line1,
			EDeviceGenICamTimerTriggerSource_Line2,
			EDeviceGenICamTimerTriggerSource_UserOutput0,
			EDeviceGenICamTimerTriggerSource_UserOutput1,
			EDeviceGenICamTimerTriggerSource_UserOutput2,
			EDeviceGenICamTimerTriggerSource_Counter0Start,
			EDeviceGenICamTimerTriggerSource_Counter1Start,
			EDeviceGenICamTimerTriggerSource_Counter2Start,
			EDeviceGenICamTimerTriggerSource_Counter0End,
			EDeviceGenICamTimerTriggerSource_Counter1End,
			EDeviceGenICamTimerTriggerSource_Counter2End,
			EDeviceGenICamTimerTriggerSource_Timer0Start,
			EDeviceGenICamTimerTriggerSource_Timer1Start,
			EDeviceGenICamTimerTriggerSource_Timer2Start,
			EDeviceGenICamTimerTriggerSource_Timer0End,
			EDeviceGenICamTimerTriggerSource_Timer1End,
			EDeviceGenICamTimerTriggerSource_Timer2End,
			EDeviceGenICamTimerTriggerSource_Encoder0,
			EDeviceGenICamTimerTriggerSource_Encoder1,
			EDeviceGenICamTimerTriggerSource_Encoder2,
			EDeviceGenICamTimerTriggerSource_LogicBlock0,
			EDeviceGenICamTimerTriggerSource_LogicBlock1,
			EDeviceGenICamTimerTriggerSource_LogicBlock2,
			EDeviceGenICamTimerTriggerSource_SoftwareSignal0,
			EDeviceGenICamTimerTriggerSource_SoftwareSignal1,
			EDeviceGenICamTimerTriggerSource_SoftwareSignal2,
			EDeviceGenICamTimerTriggerSource_Action0,
			EDeviceGenICamTimerTriggerSource_Action1,
			EDeviceGenICamTimerTriggerSource_Action2,
			EDeviceGenICamTimerTriggerSource_LinkTrigger0,
			EDeviceGenICamTimerTriggerSource_LinkTrigger1,
			EDeviceGenICamTimerTriggerSource_LinkTrigger2,
			EDeviceGenICamTimerTriggerSource_Count,
		};

		enum EDeviceGenICamTimerTriggerActivation
		{
			EDeviceGenICamTimerTriggerActivation_RisingEdge = 0,
			EDeviceGenICamTimerTriggerActivation_FallingEdge,
			EDeviceGenICamTimerTriggerActivation_AnyEdge,
			EDeviceGenICamTimerTriggerActivation_LevelHigh,
			EDeviceGenICamTimerTriggerActivation_LevelLow,
			EDeviceGenICamTimerTriggerActivation_Count,
		};

		enum EDeviceGenICamTimerSequenceEntrySelector
		{
			EDeviceGenICamTimerSequenceEntrySelector_Entry1 = 0,
			EDeviceGenICamTimerSequenceEntrySelector_Entry2,
			EDeviceGenICamTimerSequenceEntrySelector_Entry3,
			EDeviceGenICamTimerSequenceEntrySelector_Entry4,
			EDeviceGenICamTimerSequenceEntrySelector_Entry5,
			EDeviceGenICamTimerSequenceEntrySelector_Entry6,
			EDeviceGenICamTimerSequenceEntrySelector_Entry7,
			EDeviceGenICamTimerSequenceEntrySelector_Entry8,
			EDeviceGenICamTimerSequenceEntrySelector_Entry9,
			EDeviceGenICamTimerSequenceEntrySelector_Entry10,
			EDeviceGenICamTimerSequenceEntrySelector_Entry11,
			EDeviceGenICamTimerSequenceEntrySelector_Entry12,
			EDeviceGenICamTimerSequenceEntrySelector_Entry13,
			EDeviceGenICamTimerSequenceEntrySelector_Entry14,
			EDeviceGenICamTimerSequenceEntrySelector_Entry15,
			EDeviceGenICamTimerSequenceEntrySelector_Entry16,
			EDeviceGenICamTimerSequenceEntrySelector_Count,
		};

		enum EDeviceGenICamTimerSequenceTimerSelector
		{
			EDeviceGenICamTimerSequenceTimerSelector_Timer1 = 0,
			EDeviceGenICamTimerSequenceTimerSelector_Timer2,
			EDeviceGenICamTimerSequenceTimerSelector_Timer3,
			EDeviceGenICamTimerSequenceTimerSelector_Timer4,
			EDeviceGenICamTimerSequenceTimerSelector_Count,
		};

		enum EDeviceGenICamCounterSelector
		{
			EDeviceGenICamCounterSelector_Counter1 = 0,
			EDeviceGenICamCounterSelector_Counter2,
			EDeviceGenICamCounterSelector_Count,
		};

		enum EDeviceGenICamCounterEventSource
		{
			EDeviceGenICamCounterEventSource_Off = 0,
			EDeviceGenICamCounterEventSource_AcquisitionTrigger,
			EDeviceGenICamCounterEventSource_AcquisitionTriggerMissed,
			EDeviceGenICamCounterEventSource_AcquisitionStart,
			EDeviceGenICamCounterEventSource_AcquisitionEnd,
			EDeviceGenICamCounterEventSource_FrameTrigger,
			EDeviceGenICamCounterEventSource_FrameTriggerMissed,
			EDeviceGenICamCounterEventSource_FrameStart,
			EDeviceGenICamCounterEventSource_FrameEnd,
			EDeviceGenICamCounterEventSource_FrameBurstStart,
			EDeviceGenICamCounterEventSource_FrameBurstEnd,
			EDeviceGenICamCounterEventSource_LineTrigger,
			EDeviceGenICamCounterEventSource_LineTriggerMissed,
			EDeviceGenICamCounterEventSource_LineStart,
			EDeviceGenICamCounterEventSource_LineEnd,
			EDeviceGenICamCounterEventSource_ExposureStart,
			EDeviceGenICamCounterEventSource_ExposureEnd,
			EDeviceGenICamCounterEventSource_Line0,
			EDeviceGenICamCounterEventSource_Line1,
			EDeviceGenICamCounterEventSource_Line2,
			EDeviceGenICamCounterEventSource_Counter0Start,
			EDeviceGenICamCounterEventSource_Counter1Start,
			EDeviceGenICamCounterEventSource_Counter2Start,
			EDeviceGenICamCounterEventSource_Counter0End,
			EDeviceGenICamCounterEventSource_Counter1End,
			EDeviceGenICamCounterEventSource_Counter2End,
			EDeviceGenICamCounterEventSource_Timer0Start,
			EDeviceGenICamCounterEventSource_Timer1Start,
			EDeviceGenICamCounterEventSource_Timer2Start,
			EDeviceGenICamCounterEventSource_Timer0End,
			EDeviceGenICamCounterEventSource_Timer1End,
			EDeviceGenICamCounterEventSource_Timer2End,
			EDeviceGenICamCounterEventSource_Encoder0,
			EDeviceGenICamCounterEventSource_Encoder1,
			EDeviceGenICamCounterEventSource_Encoder2,
			EDeviceGenICamCounterEventSource_LogicBlock0,
			EDeviceGenICamCounterEventSource_LogicBlock1,
			EDeviceGenICamCounterEventSource_LogicBlock2,
			EDeviceGenICamCounterEventSource_SoftwareSignal0,
			EDeviceGenICamCounterEventSource_SoftwareSignal1,
			EDeviceGenICamCounterEventSource_SoftwareSignal2,
			EDeviceGenICamCounterEventSource_Action0,
			EDeviceGenICamCounterEventSource_Action1,
			EDeviceGenICamCounterEventSource_Action2,
			EDeviceGenICamCounterEventSource_LinkTrigger0,
			EDeviceGenICamCounterEventSource_LinkTrigger1,
			EDeviceGenICamCounterEventSource_LinkTrigger2,
			EDeviceGenICamCounterEventSource_TimestampTick,
			EDeviceGenICamCounterEventSource_Count,
		};

		enum EDeviceGenICamCounterResetSource
		{
			EDeviceGenICamCounterResetSource_Off = 0,
			EDeviceGenICamCounterResetSource_CounterTrigger,
			EDeviceGenICamCounterResetSource_AcquisitionTrigger,
			EDeviceGenICamCounterResetSource_AcquisitionTriggerMissed,
			EDeviceGenICamCounterResetSource_AcquisitionStart,
			EDeviceGenICamCounterResetSource_AcquisitionEnd,
			EDeviceGenICamCounterResetSource_FrameTrigger,
			EDeviceGenICamCounterResetSource_FrameTriggerMissed,
			EDeviceGenICamCounterResetSource_FrameStart,
			EDeviceGenICamCounterResetSource_FrameEnd,
			EDeviceGenICamCounterResetSource_LineTrigger,
			EDeviceGenICamCounterResetSource_LineTriggerMissed,
			EDeviceGenICamCounterResetSource_LineStart,
			EDeviceGenICamCounterResetSource_LineEnd,
			EDeviceGenICamCounterResetSource_ExposureStart,
			EDeviceGenICamCounterResetSource_ExposureEnd,
			EDeviceGenICamCounterResetSource_Line0,
			EDeviceGenICamCounterResetSource_Line1,
			EDeviceGenICamCounterResetSource_Line2,
			EDeviceGenICamCounterResetSource_UserOutput0,
			EDeviceGenICamCounterResetSource_UserOutput1,
			EDeviceGenICamCounterResetSource_UserOutput2,
			EDeviceGenICamCounterResetSource_Counter0Start,
			EDeviceGenICamCounterResetSource_Counter1Start,
			EDeviceGenICamCounterResetSource_Counter2Start,
			EDeviceGenICamCounterResetSource_Counter0End,
			EDeviceGenICamCounterResetSource_Counter1End,
			EDeviceGenICamCounterResetSource_Counter2End,
			EDeviceGenICamCounterResetSource_Timer0Start,
			EDeviceGenICamCounterResetSource_Timer1Start,
			EDeviceGenICamCounterResetSource_Timer2Start,
			EDeviceGenICamCounterResetSource_Timer0End,
			EDeviceGenICamCounterResetSource_Timer1End,
			EDeviceGenICamCounterResetSource_Timer2End,
			EDeviceGenICamCounterResetSource_Encoder0,
			EDeviceGenICamCounterResetSource_Encoder1,
			EDeviceGenICamCounterResetSource_Encoder2,
			EDeviceGenICamCounterResetSource_LogicBlock0,
			EDeviceGenICamCounterResetSource_LogicBlock1,
			EDeviceGenICamCounterResetSource_LogicBlock2,
			EDeviceGenICamCounterResetSource_SoftwareSignal0,
			EDeviceGenICamCounterResetSource_SoftwareSignal1,
			EDeviceGenICamCounterResetSource_SoftwareSignal2,
			EDeviceGenICamCounterResetSource_Action0,
			EDeviceGenICamCounterResetSource_Action1,
			EDeviceGenICamCounterResetSource_Action2,
			EDeviceGenICamCounterResetSource_LinkTrigger0,
			EDeviceGenICamCounterResetSource_LinkTrigger1,
			EDeviceGenICamCounterResetSource_LinkTrigger2,
			EDeviceGenICamCounterResetSource_Count,
		};

		enum EDeviceGenICamLUTSelector
		{
			EDeviceGenICamLUTSelector_Luminance = 0,
			EDeviceGenICamLUTSelector_Red,
			EDeviceGenICamLUTSelector_Green,
			EDeviceGenICamLUTSelector_Blue,
			EDeviceGenICamLUTSelector_Count,
		};

		enum EDeviceGenICamGetFunction
		{
			EDeviceGenICamGetFunction_OK = 0,
			EDeviceGenICamGetFunction_UnknownError,
			EDeviceGenICamGetFunction_EnumTypeError,
			EDeviceGenICamGetFunction_NullptrError,
		};

		enum EDeviceGenICamSetFunction
		{
			EDeviceGenICamSetFunction_OK = 0,
			EDeviceGenICamSetFunction_UnknownError,
			EDeviceGenICamSetFunction_AlreadyGrabError,
			EDeviceGenICamSetFunction_NotFoundApiError,
			EDeviceGenICamSetFunction_AlreadyInitializedError,
			EDeviceGenICamSetFunction_NotInitializedError,
			EDeviceGenICamSetFunction_NotFindCommandError,
			EDeviceGenICamSetFunction_DeviceNullptrError,
			EDeviceGenICamSetFunction_NotSupportError,
			EDeviceGenICamSetFunction_FunctionLockedError,
			EDeviceGenICamSetFunction_WriteToDeviceError,
			EDeviceGenICamSetFunction_WriteToDatabaseError,
			EDeviceGenICamSetFunction_ReadOnDatabaseError,
			EDeviceGenICamSetFunction_ReadOnDeviceError,
			EDeviceGenICamSetFunction_ActivatedDeviceError,
		};
	}
}


namespace Ravid
{
	namespace Device
	{
		enum EDeviceParameterCoaxlink
		{
			EDeviceParameterCoaxlink_DeviceID = 0,
			EDeviceParameterCoaxlink_SubUnitID,
			EDeviceParameterCoaxlink_EuresysFolderPath,
			EDeviceParameterCoaxlink_CamfileName,
			EDeviceParameterCoaxlink_BufferCount,
			EDeviceParameterCoaxlink_Count,
		};
	}
}

namespace Ravid
{
	namespace Device
	{
		enum EDeviceEGrabberInitializeType
		{
			EDeviceEGrabberInitializeType_Camera,
			EDeviceEGrabberInitializeType_Param,
			EDeviceEGrabberInitializeType_Count,
		};

		enum EDeviceEGrabberPixelFormat
		{
			EDeviceEGrabberPixelFormat_Mono8,
			EDeviceEGrabberPixelFormat_Mono10,
			EDeviceEGrabberPixelFormat_Count,
		};

	}
}

namespace Ravid
{
	namespace Device
	{		
		enum EGetFunctionMFB_I
		{
			EGetFunctionMFB_I_OK = 0,
			EGetFunctionMFB_I_UnknownError,
			EGetFunctionMFB_I_EnumTypeError,
			EGetFunctionMFB_I_NullptrError,
		};

		enum ESetFunctionMFB_I
		{
			ESetFunctionMFB_I_OK = 0,
			ESetFunctionMFB_I_UnknownError,
			ESetFunctionMFB_I_AlreadyInitializedError,
			ESetFunctionMFB_I_WriteToDatabaseError,
			ESetFunctionMFB_I_NotSupportedError,
			ESetFunctionMFB_I_WriteToDeviceError,
			ESetFunctionMFB_I_NotFindCommandError,
			ESetFunctionMFB_I_NotSelectFeatureError,
			ESetFunctionMFB_I_ReadOnDeviceError,
			ESetFunctionMFB_I_ReadOnDatabaseError,
		};

		enum EDeviceParameterMFB_I
		{
			EDeviceParameterMFB_I_DeviceID = 0,
			EDeviceParameterMFB_I_InputStatus,
			EDeviceParameterMFB_I_UseInputNo0,
			EDeviceParameterMFB_I_UseInputNo1,
			EDeviceParameterMFB_I_UseInputNo2,
			EDeviceParameterMFB_I_UseInputNo3,
			EDeviceParameterMFB_I_UseInputNo4,
			EDeviceParameterMFB_I_UseInputNo5,
			EDeviceParameterMFB_I_UseInputNo6,
			EDeviceParameterMFB_I_UseInputNo7,
			EDeviceParameterMFB_I_UseInputNo8,
			EDeviceParameterMFB_I_UseInputNo9,
			EDeviceParameterMFB_I_UseInputNo10,
			EDeviceParameterMFB_I_UseInputNo11,
			EDeviceParameterMFB_I_UseInputNo12,
			EDeviceParameterMFB_I_UseInputNo13,
			EDeviceParameterMFB_I_UseInputNo14,
			EDeviceParameterMFB_I_UseInputNo15,
			EDeviceParameterMFB_I_UseInputNo16,
			EDeviceParameterMFB_I_UseInputNo17,
			EDeviceParameterMFB_I_UseInputNo18,
			EDeviceParameterMFB_I_UseInputNo19,
			EDeviceParameterMFB_I_UseInputNo20,
			EDeviceParameterMFB_I_UseInputNo21,
			EDeviceParameterMFB_I_UseInputNo22,
			EDeviceParameterMFB_I_UseInputNo23,
			EDeviceParameterMFB_I_UseInputNo24,
			EDeviceParameterMFB_I_UseInputNo25,
			EDeviceParameterMFB_I_UseInputNo26,
			EDeviceParameterMFB_I_UseInputNo27,
			EDeviceParameterMFB_I_UseInputNo28,
			EDeviceParameterMFB_I_UseInputNo29,
			EDeviceParameterMFB_I_UseInputNo30,
			EDeviceParameterMFB_I_UseInputNo31,
			EDeviceParameterMFB_I_OutputStatus,
			EDeviceParameterMFB_I_UseOutputNo0,
			EDeviceParameterMFB_I_UseOutputNo1,
			EDeviceParameterMFB_I_UseOutputNo2,
			EDeviceParameterMFB_I_UseOutputNo3,
			EDeviceParameterMFB_I_UseOutputNo4,
			EDeviceParameterMFB_I_UseOutputNo5,
			EDeviceParameterMFB_I_UseOutputNo6,
			EDeviceParameterMFB_I_UseOutputNo7,
			EDeviceParameterMFB_I_UseOutputNo8,
			EDeviceParameterMFB_I_UseOutputNo9,
			EDeviceParameterMFB_I_UseOutputNo10,
			EDeviceParameterMFB_I_UseOutputNo11,
			EDeviceParameterMFB_I_UseOutputNo12,
			EDeviceParameterMFB_I_UseOutputNo13,
			EDeviceParameterMFB_I_UseOutputNo14,
			EDeviceParameterMFB_I_UseOutputNo15,
			EDeviceParameterMFB_I_UseOutputNo16,
			EDeviceParameterMFB_I_UseOutputNo17,
			EDeviceParameterMFB_I_UseOutputNo18,
			EDeviceParameterMFB_I_UseOutputNo19,
			EDeviceParameterMFB_I_UseOutputNo20,
			EDeviceParameterMFB_I_UseOutputNo21,
			EDeviceParameterMFB_I_UseOutputNo22,
			EDeviceParameterMFB_I_UseOutputNo23,
			EDeviceParameterMFB_I_UseOutputNo24,
			EDeviceParameterMFB_I_UseOutputNo25,
			EDeviceParameterMFB_I_UseOutputNo26,
			EDeviceParameterMFB_I_UseOutputNo27,
			EDeviceParameterMFB_I_UseOutputNo28,
			EDeviceParameterMFB_I_UseOutputNo29,
			EDeviceParameterMFB_I_UseOutputNo30,
			EDeviceParameterMFB_I_UseOutputNo31,
			EDeviceParameterMFB_I_InputName,
			EDeviceParameterMFB_I_InputNo0Name,
			EDeviceParameterMFB_I_InputNo1Name,
			EDeviceParameterMFB_I_InputNo2Name,
			EDeviceParameterMFB_I_InputNo3Name,
			EDeviceParameterMFB_I_InputNo4Name,
			EDeviceParameterMFB_I_InputNo5Name,
			EDeviceParameterMFB_I_InputNo6Name,
			EDeviceParameterMFB_I_InputNo7Name,
			EDeviceParameterMFB_I_InputNo8Name,
			EDeviceParameterMFB_I_InputNo9Name,
			EDeviceParameterMFB_I_InputNo10Name,
			EDeviceParameterMFB_I_InputNo11Name,
			EDeviceParameterMFB_I_InputNo12Name,
			EDeviceParameterMFB_I_InputNo13Name,
			EDeviceParameterMFB_I_InputNo14Name,
			EDeviceParameterMFB_I_InputNo15Name,
			EDeviceParameterMFB_I_InputNo16Name,
			EDeviceParameterMFB_I_InputNo17Name,
			EDeviceParameterMFB_I_InputNo18Name,
			EDeviceParameterMFB_I_InputNo19Name,
			EDeviceParameterMFB_I_InputNo20Name,
			EDeviceParameterMFB_I_InputNo21Name,
			EDeviceParameterMFB_I_InputNo22Name,
			EDeviceParameterMFB_I_InputNo23Name,
			EDeviceParameterMFB_I_InputNo24Name,
			EDeviceParameterMFB_I_InputNo25Name,
			EDeviceParameterMFB_I_InputNo26Name,
			EDeviceParameterMFB_I_InputNo27Name,
			EDeviceParameterMFB_I_InputNo28Name,
			EDeviceParameterMFB_I_InputNo29Name,
			EDeviceParameterMFB_I_InputNo30Name,
			EDeviceParameterMFB_I_InputNo31Name,
			EDeviceParameterMFB_I_OutputName,
			EDeviceParameterMFB_I_OutputNo0Name,
			EDeviceParameterMFB_I_OutputNo1Name,
			EDeviceParameterMFB_I_OutputNo2Name,
			EDeviceParameterMFB_I_OutputNo3Name,
			EDeviceParameterMFB_I_OutputNo4Name,
			EDeviceParameterMFB_I_OutputNo5Name,
			EDeviceParameterMFB_I_OutputNo6Name,
			EDeviceParameterMFB_I_OutputNo7Name,
			EDeviceParameterMFB_I_OutputNo8Name,
			EDeviceParameterMFB_I_OutputNo9Name,
			EDeviceParameterMFB_I_OutputNo10Name,
			EDeviceParameterMFB_I_OutputNo11Name,
			EDeviceParameterMFB_I_OutputNo12Name,
			EDeviceParameterMFB_I_OutputNo13Name,
			EDeviceParameterMFB_I_OutputNo14Name,
			EDeviceParameterMFB_I_OutputNo15Name,
			EDeviceParameterMFB_I_OutputNo16Name,
			EDeviceParameterMFB_I_OutputNo17Name,
			EDeviceParameterMFB_I_OutputNo18Name,
			EDeviceParameterMFB_I_OutputNo19Name,
			EDeviceParameterMFB_I_OutputNo20Name,
			EDeviceParameterMFB_I_OutputNo21Name,
			EDeviceParameterMFB_I_OutputNo22Name,
			EDeviceParameterMFB_I_OutputNo23Name,
			EDeviceParameterMFB_I_OutputNo24Name,
			EDeviceParameterMFB_I_OutputNo25Name,
			EDeviceParameterMFB_I_OutputNo26Name,
			EDeviceParameterMFB_I_OutputNo27Name,
			EDeviceParameterMFB_I_OutputNo28Name,
			EDeviceParameterMFB_I_OutputNo29Name,
			EDeviceParameterMFB_I_OutputNo30Name,
			EDeviceParameterMFB_I_OutputNo31Name,
			EDeviceParameterMFB_I_UseClearInitialize,
			EDeviceParameterMFB_I_UseClearTerminate,

			EDeviceParameterMFB_I_Startposition,
			EDeviceParameterMFB_I_Endposition,
			EDeviceParameterMFB_I_Triggercycle,
			EDeviceParameterMFB_I_Triggerpulsedurationtime,
			EDeviceParameterMFB_I_Encoderincreasementdirection,
			EDeviceParameterMFB_I_Triggercountingdirection,

			EDeviceParameterMFB_I_Count,
		};

		enum EGetFunctionMFB_T
		{
			EGetFunctionMFB_T_OK = 0,
			EGetFunctionMFB_T_UnknownError,
			EGetFunctionMFB_T_EnumTypeError,
			EGetFunctionMFB_T_NullptrError,
		};

		enum ESetFunctionMFB_T
		{
			ESetFunctionMFB_T_OK = 0,
			ESetFunctionMFB_T_UnknownError,
			ESetFunctionMFB_T_AlreadyInitializedError,
			ESetFunctionMFB_T_WriteToDatabaseError,
			ESetFunctionMFB_T_NotSupportedError,
			ESetFunctionMFB_T_WriteToDeviceError,
			ESetFunctionMFB_T_NotFindCommandError,
			ESetFunctionMFB_T_NotSelectFeatureError,
			ESetFunctionMFB_T_ReadOnDeviceError,
			ESetFunctionMFB_T_ReadOnDatabaseError,
		};
		enum EDeviceParameterMFB_T
		{
			EDeviceParameterMFB_T_DeviceID = 0,
			EDeviceParameterMFB_T_BoardType,
			EDeviceParameterMFB_T_UIInfo,
			EDeviceParameterMFB_T_ShowDIO,
			EDeviceParameterMFB_T_ShowTrgEnc,
			EDeviceParameterMFB_T_ShowQueueCH1,
			EDeviceParameterMFB_T_ShowQueueCH2,
			EDeviceParameterMFB_T_ShowID,
			EDeviceParameterMFB_T_TriggerInfo,
			EDeviceParameterMFB_T_Channel1,
			EDeviceParameterMFB_T_EncoderDirectionCH1,
			EDeviceParameterMFB_T_StartPositionCH1,
			EDeviceParameterMFB_T_EndPositionCH1,
			EDeviceParameterMFB_T_TriggerOnTimeCH1,
			EDeviceParameterMFB_T_TriggerCycleCH1,
			EDeviceParameterMFB_T_StrobeTimeCH1,
			EDeviceParameterMFB_T_Channel2,
			EDeviceParameterMFB_T_EncoderDirectionCH2,
			EDeviceParameterMFB_T_StartPositionCH2,
			EDeviceParameterMFB_T_EndPositionCH2,
			EDeviceParameterMFB_T_TriggerOnTimeCH2,
			EDeviceParameterMFB_T_TriggerCycleCH2,
			EDeviceParameterMFB_T_StrobeTimeCH2,
			EDeviceParameterMFB_T_UseInput,
			EDeviceParameterMFB_T_UseInputNo0,
			EDeviceParameterMFB_T_UseInputNo1,
			EDeviceParameterMFB_T_UseInputNo2,
			EDeviceParameterMFB_T_UseInputNo3,
			EDeviceParameterMFB_T_UseInputNo4,
			EDeviceParameterMFB_T_UseInputNo5,
			EDeviceParameterMFB_T_UseInputNo6,
			EDeviceParameterMFB_T_UseInputNo7,
			EDeviceParameterMFB_T_UseInputNo8,
			EDeviceParameterMFB_T_UseInputNo9,
			EDeviceParameterMFB_T_UseInputNo10,
			EDeviceParameterMFB_T_UseInputNo11,
			EDeviceParameterMFB_T_UseInputNo12,
			EDeviceParameterMFB_T_UseInputNo13,
			EDeviceParameterMFB_T_UseInputNo14,
			EDeviceParameterMFB_T_UseInputNo15,
			EDeviceParameterMFB_T_UseOutput,
			EDeviceParameterMFB_T_UseOutputNo0,
			EDeviceParameterMFB_T_UseOutputNo1,
			EDeviceParameterMFB_T_UseOutputNo2,
			EDeviceParameterMFB_T_UseOutputNo3,
			EDeviceParameterMFB_T_UseOutputNo4,
			EDeviceParameterMFB_T_UseOutputNo5,
			EDeviceParameterMFB_T_UseOutputNo6,
			EDeviceParameterMFB_T_UseOutputNo7,
			EDeviceParameterMFB_T_UseOutputNo8,
			EDeviceParameterMFB_T_UseOutputNo9,
			EDeviceParameterMFB_T_UseOutputNo10,
			EDeviceParameterMFB_T_UseOutputNo11,
			EDeviceParameterMFB_T_UseOutputNo12,
			EDeviceParameterMFB_T_UseOutputNo13,
			EDeviceParameterMFB_T_UseOutputNo14,
			EDeviceParameterMFB_T_UseOutputNo15,
			EDeviceParameterMFB_T_InputName,
			EDeviceParameterMFB_T_InputNo0Name,
			EDeviceParameterMFB_T_InputNo1Name,
			EDeviceParameterMFB_T_InputNo2Name,
			EDeviceParameterMFB_T_InputNo3Name,
			EDeviceParameterMFB_T_InputNo4Name,
			EDeviceParameterMFB_T_InputNo5Name,
			EDeviceParameterMFB_T_InputNo6Name,
			EDeviceParameterMFB_T_InputNo7Name,
			EDeviceParameterMFB_T_InputNo8Name,
			EDeviceParameterMFB_T_InputNo9Name,
			EDeviceParameterMFB_T_InputNo10Name,
			EDeviceParameterMFB_T_InputNo11Name,
			EDeviceParameterMFB_T_InputNo12Name,
			EDeviceParameterMFB_T_InputNo13Name,
			EDeviceParameterMFB_T_InputNo14Name,
			EDeviceParameterMFB_T_InputNo15Name,
			EDeviceParameterMFB_T_OutputName,
			EDeviceParameterMFB_T_OutputNo0Name,
			EDeviceParameterMFB_T_OutputNo1Name,
			EDeviceParameterMFB_T_OutputNo2Name,
			EDeviceParameterMFB_T_OutputNo3Name,
			EDeviceParameterMFB_T_OutputNo4Name,
			EDeviceParameterMFB_T_OutputNo5Name,
			EDeviceParameterMFB_T_OutputNo6Name,
			EDeviceParameterMFB_T_OutputNo7Name,
			EDeviceParameterMFB_T_OutputNo8Name,
			EDeviceParameterMFB_T_OutputNo9Name,
			EDeviceParameterMFB_T_OutputNo10Name,
			EDeviceParameterMFB_T_OutputNo11Name,
			EDeviceParameterMFB_T_OutputNo12Name,
			EDeviceParameterMFB_T_OutputNo13Name,
			EDeviceParameterMFB_T_OutputNo14Name,
			EDeviceParameterMFB_T_OutputNo15Name,
			EDeviceParameterMFB_T_UseClearInitialize,
			EDeviceParameterMFB_T_UseClearTerminate,
			EDeviceParameterMFB_T_Count,
		};

	}
}

namespace Ravid
{
	namespace Device
	{
		enum EGetFunctionMVTechDio
		{
			EGetFunctionMVTechDio_OK = 0,
			EGetFunctionMVTechDio_UnknownError,
			EGetFunctionMVTechDio_EnumTypeError,
			EGetFunctionMVTechDio_NullptrError,
		};

		enum ESetFunctionMVTechDio
		{
			ESetFunctionMVTechDio_OK = 0,
			ESetFunctionMVTechDio_UnknownError,
			ESetFunctionMVTechDio_AlreadyInitializedError,
			ESetFunctionMVTechDio_WriteToDatabaseError,
			ESetFunctionMVTechDio_NotSupportedError,
			ESetFunctionMVTechDio_WriteToDeviceError,
			ESetFunctionMVTechDio_NotFindCommandError,
			ESetFunctionMVTechDio_NotSelectFeatureError,
			ESetFunctionMVTechDio_ReadOnDeviceError,
			ESetFunctionMVTechDio_ReadOnDatabaseError,
		};
	}
}

namespace Ravid
{
	namespace Device
	{
		enum EDalsaFrameGrabbersGetFunction
		{
			EDalsaFrameGrabbersGetFunction_OK = 0,
			EDalsaFrameGrabbersGetFunction_UnknownError,
			EDalsaFrameGrabbersGetFunction_EnumTypeError,
			EDalsaFrameGrabbersGetFunction_NullptrError,
		};

		enum EDalsaFrameGrabbersSetFunction
		{
			EDalsaFrameGrabbersSetFunction_OK = 0,
			EDalsaFrameGrabbersSetFunction_UnknownError,
			EDalsaFrameGrabbersSetFunction_AlreadyGrabError,
			EDalsaFrameGrabbersSetFunction_NotFoundApiError,
			EDalsaFrameGrabbersSetFunction_AlreadyInitializedError,
			EDalsaFrameGrabbersSetFunction_NotInitializedError,
			EDalsaFrameGrabbersSetFunction_NotFindCommandError, 
			EDalsaFrameGrabbersSetFunction_DeviceNullptrError,
			EDalsaFrameGrabbersSetFunction_NotSupportError,
			EDalsaFrameGrabbersSetFunction_FunctionLockedError,
			EDalsaFrameGrabbersSetFunction_WriteToDeviceError,
			EDalsaFrameGrabbersSetFunction_WriteToDatabaseError,
			EDalsaFrameGrabbersSetFunction_ReadOnDatabaseError,
			EDalsaFrameGrabbersSetFunction_ReadOnDeviceError,
			EDalsaFrameGrabbersSetFunction_ActivatedDeviceError,
		};

		enum EDeviceParameterDalsaFrameGrabbers
		{
			EDeviceParameterDalsaFrameGrabbers_DeviceID = 0,
			EDeviceParameterDalsaFrameGrabbers_SubUnitID,
			EDeviceParameterDalsaFrameGrabbers_CamfileName,
			EDeviceParameterDalsaFrameGrabbers_Count,
		};
	}
}

namespace Ravid
{
	namespace Device
	{
		enum EDeviceParameterMicroview
		{
			EDeviceParameterMicroview_DeviceID = 0,
			EDeviceParameterMicroview_Count,
		};
	}
}


// MVTechMFB_I Parameter List
namespace Ravid
{
	namespace Device
	{
		enum EDeviceParameterMVTechMFB_I
		{
			EDeviceParameterMVTechMFB_I_DeviceID = 0,
			EDeviceParameterMVTechMFB_I_UseClearInitialize,
			EDeviceParameterMVTechMFB_I_UseClearTerminate,
			EDeviceParameterMVTechMFB_I_TriggerInfo,
			EDeviceParameterMVTechMFB_I_Startposition,
			EDeviceParameterMVTechMFB_I_Endposition,
			EDeviceParameterMVTechMFB_I_TriggerCycle,
			EDeviceParameterMVTechMFB_I_TriggerOntime,
			EDeviceParameterMVTechMFB_I_EncoderDirection,
			EDeviceParameterMVTechMFB_I_InputStatus,
			EDeviceParameterMVTechMFB_I_UseInputNo0,
			EDeviceParameterMVTechMFB_I_UseInputNo1,
			EDeviceParameterMVTechMFB_I_UseInputNo2,
			EDeviceParameterMVTechMFB_I_UseInputNo3,
			EDeviceParameterMVTechMFB_I_UseInputNo4,
			EDeviceParameterMVTechMFB_I_UseInputNo5,
			EDeviceParameterMVTechMFB_I_UseInputNo6,
			EDeviceParameterMVTechMFB_I_UseInputNo7,
			EDeviceParameterMVTechMFB_I_UseInputNo8,
			EDeviceParameterMVTechMFB_I_UseInputNo9,
			EDeviceParameterMVTechMFB_I_UseInputNo10,
			EDeviceParameterMVTechMFB_I_UseInputNo11,
			EDeviceParameterMVTechMFB_I_UseInputNo12,
			EDeviceParameterMVTechMFB_I_UseInputNo13,
			EDeviceParameterMVTechMFB_I_UseInputNo14,
			EDeviceParameterMVTechMFB_I_UseInputNo15,
			EDeviceParameterMVTechMFB_I_UseInputNo16,
			EDeviceParameterMVTechMFB_I_UseInputNo17,
			EDeviceParameterMVTechMFB_I_UseInputNo18,
			EDeviceParameterMVTechMFB_I_UseInputNo19,
			EDeviceParameterMVTechMFB_I_UseInputNo20,
			EDeviceParameterMVTechMFB_I_UseInputNo21,
			EDeviceParameterMVTechMFB_I_UseInputNo22,
			EDeviceParameterMVTechMFB_I_UseInputNo23,
			EDeviceParameterMVTechMFB_I_UseInputNo24,
			EDeviceParameterMVTechMFB_I_UseInputNo25,
			EDeviceParameterMVTechMFB_I_UseInputNo26,
			EDeviceParameterMVTechMFB_I_UseInputNo27,
			EDeviceParameterMVTechMFB_I_UseInputNo28,
			EDeviceParameterMVTechMFB_I_UseInputNo29,
			EDeviceParameterMVTechMFB_I_UseInputNo30,
			EDeviceParameterMVTechMFB_I_UseInputNo31,
			EDeviceParameterMVTechMFB_I_OutputStatus,
			EDeviceParameterMVTechMFB_I_UseOutputNo0,
			EDeviceParameterMVTechMFB_I_UseOutputNo1,
			EDeviceParameterMVTechMFB_I_UseOutputNo2,
			EDeviceParameterMVTechMFB_I_UseOutputNo3,
			EDeviceParameterMVTechMFB_I_UseOutputNo4,
			EDeviceParameterMVTechMFB_I_UseOutputNo5,
			EDeviceParameterMVTechMFB_I_UseOutputNo6,
			EDeviceParameterMVTechMFB_I_UseOutputNo7,
			EDeviceParameterMVTechMFB_I_UseOutputNo8,
			EDeviceParameterMVTechMFB_I_UseOutputNo9,
			EDeviceParameterMVTechMFB_I_UseOutputNo10,
			EDeviceParameterMVTechMFB_I_UseOutputNo11,
			EDeviceParameterMVTechMFB_I_UseOutputNo12,
			EDeviceParameterMVTechMFB_I_UseOutputNo13,
			EDeviceParameterMVTechMFB_I_UseOutputNo14,
			EDeviceParameterMVTechMFB_I_UseOutputNo15,
			EDeviceParameterMVTechMFB_I_UseOutputNo16,
			EDeviceParameterMVTechMFB_I_UseOutputNo17,
			EDeviceParameterMVTechMFB_I_UseOutputNo18,
			EDeviceParameterMVTechMFB_I_UseOutputNo19,
			EDeviceParameterMVTechMFB_I_UseOutputNo20,
			EDeviceParameterMVTechMFB_I_UseOutputNo21,
			EDeviceParameterMVTechMFB_I_UseOutputNo22,
			EDeviceParameterMVTechMFB_I_UseOutputNo23,
			EDeviceParameterMVTechMFB_I_UseOutputNo24,
			EDeviceParameterMVTechMFB_I_UseOutputNo25,
			EDeviceParameterMVTechMFB_I_UseOutputNo26,
			EDeviceParameterMVTechMFB_I_UseOutputNo27,
			EDeviceParameterMVTechMFB_I_UseOutputNo28,
			EDeviceParameterMVTechMFB_I_UseOutputNo29,
			EDeviceParameterMVTechMFB_I_UseOutputNo30,
			EDeviceParameterMVTechMFB_I_UseOutputNo31,
			EDeviceParameterMVTechMFB_I_InputName,
			EDeviceParameterMVTechMFB_I_InputNo0Name,
			EDeviceParameterMVTechMFB_I_InputNo1Name,
			EDeviceParameterMVTechMFB_I_InputNo2Name,
			EDeviceParameterMVTechMFB_I_InputNo3Name,
			EDeviceParameterMVTechMFB_I_InputNo4Name,
			EDeviceParameterMVTechMFB_I_InputNo5Name,
			EDeviceParameterMVTechMFB_I_InputNo6Name,
			EDeviceParameterMVTechMFB_I_InputNo7Name,
			EDeviceParameterMVTechMFB_I_InputNo8Name,
			EDeviceParameterMVTechMFB_I_InputNo9Name,
			EDeviceParameterMVTechMFB_I_InputNo10Name,
			EDeviceParameterMVTechMFB_I_InputNo11Name,
			EDeviceParameterMVTechMFB_I_InputNo12Name,
			EDeviceParameterMVTechMFB_I_InputNo13Name,
			EDeviceParameterMVTechMFB_I_InputNo14Name,
			EDeviceParameterMVTechMFB_I_InputNo15Name,
			EDeviceParameterMVTechMFB_I_InputNo16Name,
			EDeviceParameterMVTechMFB_I_InputNo17Name,
			EDeviceParameterMVTechMFB_I_InputNo18Name,
			EDeviceParameterMVTechMFB_I_InputNo19Name,
			EDeviceParameterMVTechMFB_I_InputNo20Name,
			EDeviceParameterMVTechMFB_I_InputNo21Name,
			EDeviceParameterMVTechMFB_I_InputNo22Name,
			EDeviceParameterMVTechMFB_I_InputNo23Name,
			EDeviceParameterMVTechMFB_I_InputNo24Name,
			EDeviceParameterMVTechMFB_I_InputNo25Name,
			EDeviceParameterMVTechMFB_I_InputNo26Name,
			EDeviceParameterMVTechMFB_I_InputNo27Name,
			EDeviceParameterMVTechMFB_I_InputNo28Name,
			EDeviceParameterMVTechMFB_I_InputNo29Name,
			EDeviceParameterMVTechMFB_I_InputNo30Name,
			EDeviceParameterMVTechMFB_I_InputNo31Name,
			EDeviceParameterMVTechMFB_I_OutputName,
			EDeviceParameterMVTechMFB_I_OutputNo0Name,
			EDeviceParameterMVTechMFB_I_OutputNo1Name,
			EDeviceParameterMVTechMFB_I_OutputNo2Name,
			EDeviceParameterMVTechMFB_I_OutputNo3Name,
			EDeviceParameterMVTechMFB_I_OutputNo4Name,
			EDeviceParameterMVTechMFB_I_OutputNo5Name,
			EDeviceParameterMVTechMFB_I_OutputNo6Name,
			EDeviceParameterMVTechMFB_I_OutputNo7Name,
			EDeviceParameterMVTechMFB_I_OutputNo8Name,
			EDeviceParameterMVTechMFB_I_OutputNo9Name,
			EDeviceParameterMVTechMFB_I_OutputNo10Name,
			EDeviceParameterMVTechMFB_I_OutputNo11Name,
			EDeviceParameterMVTechMFB_I_OutputNo12Name,
			EDeviceParameterMVTechMFB_I_OutputNo13Name,
			EDeviceParameterMVTechMFB_I_OutputNo14Name,
			EDeviceParameterMVTechMFB_I_OutputNo15Name,
			EDeviceParameterMVTechMFB_I_OutputNo16Name,
			EDeviceParameterMVTechMFB_I_OutputNo17Name,
			EDeviceParameterMVTechMFB_I_OutputNo18Name,
			EDeviceParameterMVTechMFB_I_OutputNo19Name,
			EDeviceParameterMVTechMFB_I_OutputNo20Name,
			EDeviceParameterMVTechMFB_I_OutputNo21Name,
			EDeviceParameterMVTechMFB_I_OutputNo22Name,
			EDeviceParameterMVTechMFB_I_OutputNo23Name,
			EDeviceParameterMVTechMFB_I_OutputNo24Name,
			EDeviceParameterMVTechMFB_I_OutputNo25Name,
			EDeviceParameterMVTechMFB_I_OutputNo26Name,
			EDeviceParameterMVTechMFB_I_OutputNo27Name,
			EDeviceParameterMVTechMFB_I_OutputNo28Name,
			EDeviceParameterMVTechMFB_I_OutputNo29Name,
			EDeviceParameterMVTechMFB_I_OutputNo30Name,
			EDeviceParameterMVTechMFB_I_OutputNo31Name,
			
			EDeviceParameterMVTechMFB_I_Count,
		};
	}
}

// MVTechMFB_T Parameter List
namespace Ravid
{
	namespace Device
	{
		enum EDeviceParameterMVTechMFB_T
		{
			EDeviceParameterMVTechMFB_T_DeviceID = 0,
			EDeviceParameterMVTechMFB_T_UseClearInitialize,
			EDeviceParameterMVTechMFB_T_UseClearTerminate,
			EDeviceParameterMVTechMFB_T_TriggerInfo,
			EDeviceParameterMVTechMFB_T_TriggerDirection,
			EDeviceParameterMVTechMFB_T_EncoderDirection,
			EDeviceParameterMVTechMFB_T_UseInput,
			EDeviceParameterMVTechMFB_T_UseInputNo0,
			EDeviceParameterMVTechMFB_T_UseInputNo1,
			EDeviceParameterMVTechMFB_T_UseInputNo2,
			EDeviceParameterMVTechMFB_T_UseInputNo3,
			EDeviceParameterMVTechMFB_T_UseInputNo4,
			EDeviceParameterMVTechMFB_T_UseInputNo5,
			EDeviceParameterMVTechMFB_T_UseInputNo6,
			EDeviceParameterMVTechMFB_T_UseInputNo7,
			EDeviceParameterMVTechMFB_T_UseInputNo8,
			EDeviceParameterMVTechMFB_T_UseInputNo9,
			EDeviceParameterMVTechMFB_T_UseInputNo10,
			EDeviceParameterMVTechMFB_T_UseInputNo11,
			EDeviceParameterMVTechMFB_T_UseInputNo12,
			EDeviceParameterMVTechMFB_T_UseInputNo13,
			EDeviceParameterMVTechMFB_T_UseInputNo14,
			EDeviceParameterMVTechMFB_T_UseInputNo15,
			EDeviceParameterMVTechMFB_T_UseOutput,
			EDeviceParameterMVTechMFB_T_UseOutputNo0,
			EDeviceParameterMVTechMFB_T_UseOutputNo1,
			EDeviceParameterMVTechMFB_T_UseOutputNo2,
			EDeviceParameterMVTechMFB_T_UseOutputNo3,
			EDeviceParameterMVTechMFB_T_UseOutputNo4,
			EDeviceParameterMVTechMFB_T_UseOutputNo5,
			EDeviceParameterMVTechMFB_T_UseOutputNo6,
			EDeviceParameterMVTechMFB_T_UseOutputNo7,
			EDeviceParameterMVTechMFB_T_UseOutputNo8,
			EDeviceParameterMVTechMFB_T_UseOutputNo9,
			EDeviceParameterMVTechMFB_T_UseOutputNo10,
			EDeviceParameterMVTechMFB_T_UseOutputNo11,
			EDeviceParameterMVTechMFB_T_UseOutputNo12,
			EDeviceParameterMVTechMFB_T_UseOutputNo13,
			EDeviceParameterMVTechMFB_T_UseOutputNo14,
			EDeviceParameterMVTechMFB_T_UseOutputNo15,
			EDeviceParameterMVTechMFB_T_InputName,
			EDeviceParameterMVTechMFB_T_InputNo0Name,
			EDeviceParameterMVTechMFB_T_InputNo1Name,
			EDeviceParameterMVTechMFB_T_InputNo2Name,
			EDeviceParameterMVTechMFB_T_InputNo3Name,
			EDeviceParameterMVTechMFB_T_InputNo4Name,
			EDeviceParameterMVTechMFB_T_InputNo5Name,
			EDeviceParameterMVTechMFB_T_InputNo6Name,
			EDeviceParameterMVTechMFB_T_InputNo7Name,
			EDeviceParameterMVTechMFB_T_InputNo8Name,
			EDeviceParameterMVTechMFB_T_InputNo9Name,
			EDeviceParameterMVTechMFB_T_InputNo10Name,
			EDeviceParameterMVTechMFB_T_InputNo11Name,
			EDeviceParameterMVTechMFB_T_InputNo12Name,
			EDeviceParameterMVTechMFB_T_InputNo13Name,
			EDeviceParameterMVTechMFB_T_InputNo14Name,
			EDeviceParameterMVTechMFB_T_InputNo15Name,
			EDeviceParameterMVTechMFB_T_OutputName,
			EDeviceParameterMVTechMFB_T_OutputNo0Name,
			EDeviceParameterMVTechMFB_T_OutputNo1Name,
			EDeviceParameterMVTechMFB_T_OutputNo2Name,
			EDeviceParameterMVTechMFB_T_OutputNo3Name,
			EDeviceParameterMVTechMFB_T_OutputNo4Name,
			EDeviceParameterMVTechMFB_T_OutputNo5Name,
			EDeviceParameterMVTechMFB_T_OutputNo6Name,
			EDeviceParameterMVTechMFB_T_OutputNo7Name,
			EDeviceParameterMVTechMFB_T_OutputNo8Name,
			EDeviceParameterMVTechMFB_T_OutputNo9Name,
			EDeviceParameterMVTechMFB_T_OutputNo10Name,
			EDeviceParameterMVTechMFB_T_OutputNo11Name,
			EDeviceParameterMVTechMFB_T_OutputNo12Name,
			EDeviceParameterMVTechMFB_T_OutputNo13Name,
			EDeviceParameterMVTechMFB_T_OutputNo14Name,
			EDeviceParameterMVTechMFB_T_OutputNo15Name,

			EDeviceParameterMVTechMFB_T_Count,
		};
	}
}

// Public List
namespace Ravid
{
	namespace Device
	{
		enum EGetFunction
		{
			EGetFunction_OK = 0,
			EGetFunction_UnknownError,
			EGetFunction_EnumTypeError,
			EGetFunction_NullptrError,
		};

		enum ESetFunction
		{
			ESetFunction_OK = 0,
			ESetFunction_UnknownError,
			ESetFunction_AlreadyInitializedError,
			ESetFunction_WriteToDatabaseError,
			ESetFunction_NotSupportedError,
			ESetFunction_WriteToDeviceError,
			ESetFunction_NotFindCommandError,
			ESetFunction_NotSelectFeatureError,
			ESetFunction_ReadOnDeviceError,
			ESetFunction_ReadOnDatabaseError,
		};

		enum EDeviceSimplexDirection
		{
			EDeviceSimplexDirection_CW = 0,
			EDeviceSimplexDirection_CCW,
			EDeviceSimplexDirection_Count,
		};

		enum EDeviceDuplexDirection
		{
			EDeviceDuplexDirection_CW = 0,
			EDeviceDuplexDirection_CCW,
			EDeviceDuplexDirection_BOTH,
			EDeviceDuplexDirection_Count,
		};

		enum EDeviceSelect
		{
			EDeviceSelect_FALSE = 0,
			EDeviceSelect_TRUE,
			EDeviceSelect_Count,
		};

		enum EDeviceSwitch
		{
			EDeviceSwitch_OFF = 0,
			EDeviceSwitch_ON,
			EDeviceSwitch_Count,
		};
	}
}