#include "stdafx.h"

#include "DeviceMatroxIntellicam.h"

#ifdef USING_DEVICE

#include "../RavidCore/RavidRect.h"
#include "../RavidCore/RavidImage.h"
#include "../RavidCore/RavidDatabase.h"
#include "../RavidCore/RavidRecordset.h"
#include "../RavidCore/PerformanceCounter.h"

#include "../RavidFramework/UIManager.h"
#include "../RavidFramework/LogManager.h"
#include "../RavidFramework/DeviceManager.h"
#include "../RavidFramework/LibraryManager.h"
#include "../RavidFramework/FrameworkManager.h" 
#include "../RavidFramework/EventHandlerManager.h"
#include "../RavidFramework/MultiLanguageManager.h"

#include "../RavidFramework/RavidImageView.h"

#include "../Libraries/Includes/MatroxIntellicam/Mil.h"
#include "../Libraries/Includes/MatroxIntellicam/MILDyn/milim.h"

// mil.dll
#pragma comment(lib, COMMONLIB_PREFIX "MatroxIntellicam/mil.lib")

using namespace Ravid;
using namespace Ravid::Device;
using namespace Ravid::Database;
using namespace Ravid::Framework;
using namespace Ravid::Algorithms;
using namespace Ravid::Miscellaneous;

MIL_ID CDeviceMatroxIntellicam::m_hMilApplication;

IMPLEMENT_DYNAMIC(CDeviceMatroxIntellicam, CDeviceFrameGrabber)

static LPCTSTR g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Count] =
{
	_T("DeviceID"),
	_T("SubUnitID"),
	_T("Camera name"),
	_T("Initialize Type"),
	_T("Camfile path"),
	_T("Parameter match"),
	_T("Grab Count"),
	_T("MIL Image Count"),
	_T("MIL Buffer Count"),

	_T("Grab Setting"),
	_T("Grab Direction X"),
	_T("Grab Direction Y"),
	_T("Grab Scale"),
	_T("Grab Scale X"),
	_T("Grab Scale Y"),
	_T("Source Offset X"),
	_T("Source Offset Y"),
	_T("Source Size X"),
	_T("Source Size Y"),
	_T("Grab Trigger Setting"),
	_T("Grab Trigger Continuoues End Trigger"),
	_T("Grab Trigger Activation"),
	_T("Grab Trigger Soursce"),
	_T("Grab Trigger State"),
// 	_T("Timer Setting"),
// 	_T("Timer Selector"),
// 	_T("Timer1"),
// 	_T("Timer1 ClockSource"),
// 	_T("Timer1 Delay"),
// 	_T("Timer1 Duration"),
// 	_T("Timer1 Output Inverter"),
// 	_T("Timer1 State"),
// 	_T("Timer1 Trigger Activation"),
// 	_T("Timer1 Trigger Missed"),
// 	_T("Timer1 Trigger Rate Divider"),
// 	_T("Timer1 Trigger Source"),
// 	_T("Timer2"),
// 	_T("Timer2 Clock Source"),
// 	_T("Timer2 Delay"),
// 	_T("Timer2 Duration"),
// 	_T("Timer2 Output Inverter"),
// 	_T("Timer2 State"),
// 	_T("Timer2 Trigger Activation"),
// 	_T("Timer2 Trigger Missed"),
// 	_T("Timer2 Trigger Rate Divider"),
// 	_T("Timer2 Trigger Source"),
// 	_T("Timer3"),
// 	_T("Timer3 Clock Source"),
// 	_T("Timer3 Delay"),
// 	_T("Timer3 Duration"),
// 	_T("Timer3 Output Inverter"),
// 	_T("Timer3 State"),
// 	_T("Timer3 Trigger Activation"),
// 	_T("Timer3 Trigger Missed"),
// 	_T("Timer3 Trigger Rate Divider"),
// 	_T("Timer3 Trigger Source"),
// 	_T("Timer4"),
// 	_T("Timer4 Clock Source"),
// 	_T("Timer4 Delay"),
// 	_T("Timer4 Duration"),
// 	_T("Timer4 Output Inverter"),
// 	_T("Timer4 State"),
// 	_T("Timer4 Trigger Activation"),
// 	_T("Timer4 Trigger Missed"),
// 	_T("Timer4 Trigger Rate Divider"),
// 	_T("Timer4 Trigger Source"),
	
	_T("Camera"),
	_T("Camera Settings"),
	_T("Camera Type"),
	_T("Camera Number of taps"),
	_T("Camera Bayer mode"),
	_T("Camera Link Configuration"),
	_T("Configuration Type"),
	_T("Camera Mode"),
	_T("Tap Configuration"),
	_T("Regions"),
	_T("Regions X"),
	_T("Regions Y"),
	_T("Adjacent Pixels"),
	_T("Adjacent X"),
	_T("Adjacent Y"),
	_T("Taps Direction"),
	_T("Tap1"),
	_T("Tap2"),
	_T("Tap3"),
	_T("Tap4"),
	_T("Tap5"),
	_T("Tap6"),
	_T("Tap7"),
	_T("Tap8"),	
	_T("Video Signal"),
	_T("Video Signal Infomation"),
	_T("Video Signal Type"),
	_T("Video Signal Databus width"),
	_T("Video Signal Standard"),
	_T("Video Signal MIL channel/input"),
	_T("Digital Video Signal Infomation"),
	_T("Digital Video Signal Format"),
	_T("VideoTiming"),
	_T("Valid Signal Source"),
	_T("Use Frame Grabber"),
	_T("Timings"),
	_T("Timings Vertical"),
	_T("Pixel clock frq. Auto-adjust"),
	_T("Horizontal"),
	_T("Horizontal Sync [pclk]"),
	_T("Horizontal BPorch [pclk]"),
	_T("Horizontal Active [pclk]"),
	_T("Horizontal FPorch [pclk]"),
	_T("Horizontal Frequency [KHz]"),
	_T("Horizontal Lock Activeand Total"),
	_T("Vertical"),
	_T("Vertical Sync [Lines]"),
	_T("Vertical BPorch [Lines]"),
	_T("Vertical Active [Lines]"),
	_T("Vertical FPorch [Lines]"),
	_T("Vertical Frequency [Hz]"),
	_T("Vertical Lock Activeand Total"),
	_T("Use Camera"),
	_T("Cameral Link Settings"),
	_T("Imagesize X"),
	_T("Imagesize Y"),
	_T("Delay X"),
	_T("Delay Y"),
	_T("PixelClock"),
	_T("PixelClock Frequency"),
	_T("PixelClock Frequency [MHz]"),
	_T("Auto-adjust in the Video Timings page"),
	_T("IO Characteristics"),
	_T("Sendtoexternal circuit [other than camera]"),
	_T("External Clock Signal"),
	_T("EXT Input"),
	_T("EXT Input Frequency"),
	_T("EXT Input Format"),
	_T("EXT Input Polarity"),
	_T("EXT Output"),
	_T("EXT Output Frequency"),
	_T("EXT Output Format"),
	_T("EXT Output Polarity"),
	_T("EXT IO Delay"),
	_T("Exposure Signal"),
	_T("Exp Timer1"),
	_T("Timer1 Generation"),
	_T("Timer1 Mode"),
	_T("Timer1 Exp Signal"),
	_T("Timer1 Exp Format"),
	_T("Timer1 Exp Polarity"),
	_T("Timer1 Trigger Info"),
	_T("Timer1 Trg Format"),
	_T("Timer1 Trg Signal"),
	_T("Timer1 Trg Polarity"),
	_T("Timer1 Pulse"),
	_T("Timer1 Pulse1 [clk]"),
	_T("Timer1 Delay1 [clk]"),
	_T("Timer1 Pulse2 [clk]"),
	_T("Timer1 Delay2 [clk]"),
	_T("Timer1 Exposure Clock"),
	_T("Timer1 Type"),
	_T("Timer1 Division factor"),
	_T("Timer1 Base Clock Frequency"),
	_T("Exp Timer2"),
	_T("Timer2 Generation"),
	_T("Timer2 Mode"),
	_T("Timer2 Exp Signal Info"),
	_T("Timer2 Exp Format"),
	_T("Timer2 Exp Polarity"),
	_T("Timer2 Trigger Info"),
	_T("Timer2 Trg Format"),
	_T("Timer2 Trg Signal"),
	_T("Timer2 Trg Polarity"),
	_T("Timer2 Pulse"),
	_T("Timer2 Pulse1 [clk]"),
	_T("Timer2 Delay1 [clk]"),
	_T("Timer2 Pulse2 [clk]"),
	_T("Timer2 Delay2 [clk]"),
	_T("Timer2 Exposure Clock"),
	_T("Timer2 Type"),
	_T("Timer2 Division factor"),
	_T("Timer2 Base Clock Frequency"),
	_T("Timer1 Advanced"),
	_T("Timer1 Options"),
	_T("Timer1 Timer output signal"),
	_T("Timer1 Capture a trigger"),
	_T("Timer1 Trigger Arm"),
	_T("Timer1 Trg. Arm Mode"),
	_T("Timer1 Trg. Arm Format"),
	_T("Timer1 Trg. Arm Source"),
	_T("Timer1 Trg. Arm Polarity"),
	_T("Timer2 Advanced"),
	_T("Timer2 Options"),
	_T("Timer2 Timer output signal"),
	_T("Timer2 Capture a trigger"),
	_T("Timer2 Trigger Arm"),
	_T("Timer2 Trg. Arm Mode"),
	_T("Timer2 Trg. Arm Format"),
	_T("Timer2 Trg. Arm Source"),
	_T("Timer2 Trg. Arm Polarity"),
	_T("Grab Mode"),
	_T("Grab Characteristics"),
	_T("Grab Characteristics Mode"),
	_T("Grab Activation mode"),
	_T("Trigger Signal Characteristics"),
	_T("Grab Trg. Format"),
	_T("Grab Trg. Signal"),
	_T("Grab Trg. Polarity"),
	_T("Trigger Arm Characteristics"),
	_T("Grab Trg. Arm Format"),
	_T("Grab Trg. Arm Signal"),
	_T("Grab Trg. Arm Polarity"),
	_T("Sync.Signal"),
	_T("Source Specification"),
	_T("Spc Source"),
	_T("Spc Format"),
	_T("Spc Cameraoutput/Input signal latency"),
	_T("Spc Synchronization signal available"),
	_T("Spc Block synchronization type enable"),
	_T("Spc External VSYNC signalis enable [fromext.circuit,notcamera]"),
	_T("Digital Synchro."),
	_T("HSync"),
	_T("HSync Input activce"),
	_T("HSync Input Format"),
	_T("HSync Input Polarity"),
	_T("HSync Output activce"),
	_T("HSync Output Format"),
	_T("HSync Output Polarity"),
	_T("VSync"),
	_T("VSync Input activce"),
	_T("VSync Input Format"),
	_T("VSync Input Polarity"),
	_T("VSync Output activce"),
	_T("VSync Output Format"),
	_T("VSync Output Polarity"),
	_T("CSync"),
	_T("CSync Input activce"),
	_T("CSync Input Format"),
	_T("CSync Input Polarity"),
	_T("CSync Output activce"),
	_T("CSync Output Format"),
	_T("CSync Output Polarity"),
	_T("CSync Serration pulse"),
	_T("CSync Equalization pulse"),
	_T("Other"),
	_T("Camera Link Control Bits"),
	_T("Camera Control Bits Sources"),
	_T("Control1"),
	_T("Control2"),
	_T("Control3"),
	_T("Control4"),
	_T("Signals Output"),
	_T("Enable CCoutputs on connector1"),
	_T("Enable CCoutputs on connector2"),
};

static LPCTSTR g_lpszMatrox_InitializeType[EDeviceMatrox_InitilizeType_Count] =
{
	_T("Camfile"),
	_T("Parameter"),
};

static LPCTSTR g_lpszMatrox_CameraType[EDeviceMatrox_CameraType_Count] =
{
	_T("Frame Scan"),
	_T("Line Scan"),
};

static LPCTSTR g_lpszMatrox_Numberoftaps[EDeviceMatrox_NumberOfTaps_Count] =
{
	_T("1 Tap"),
	_T("2 Taps"),
	_T("4 Taps"),
	_T("8 Taps"),
	_T("10 Taps"),
};

static LPCTSTR g_lpszMatrox_Bayermode[EDeviceMatrox_BayerMode_Count] =
{
	_T("DISABLE"),
	_T("Blue - Green"),
	_T("Green - Blue"),
	_T("Green - Red"),
	_T("Red - Green"),
};

static LPCTSTR g_lpszMatrox_ConfigurationType[EDeviceMatrox_ConfigurationType_Count] =
{
	_T("base"),
	_T("medium"),
	_T("full"),
};

static LPCTSTR g_lpszMatrox_CameraMode[EDeviceMatrox_CameraTapsMode_Count] =
{
	_T("1 Tap 8-16 Bits"),
	_T("2 Taps 8 Bits"),
	_T("2 Taps 10/12 Bits"),
	_T("2 Taps 14/16 Bits Time Multiplexed"),
	_T("2 Taps 14/16 Bits (ABCD)"),
	_T("2 Taps 14/16 Bits (ABDE)"),
	_T("4 Taps 8 Bits Time Multiplexed"),
	_T("4 Taps 10/12 Bits Time Multiplexed"),
	_T("4 Taps 8 Bits"),
	_T("4 Taps 10/12 Bits"),
	_T("4 Taps 14/16 Bits"),
	_T("8 Taps 8 Bits Time Multiplexed"),
	_T("8 Taps 8 Bits"),
	_T("10 Taps 8 Bits"),
	_T("3 Taps 8Bits RGB"),
	_T("3 Taps 10-12Bits RGB"),
	_T("3 Taps 14-16Bits RGB"),
	_T("6 Taps 8Bits Time Multiplexed 2xRGB"),
	_T("6 Taps 8Bits 2xRGB ABC_DEF_ Non-ADJ"),
	_T("6 Taps 8Bits 2xRGB ABCDEFGH ADJ"),
};

static LPCTSTR g_lpszMatrox_TapsSelector[EDeviceMatrox_TapsSelector_Count] =
{
	_T("1"),
	_T("2"),
	_T("4"),
	_T("8"),
	_T("10"),
};

static LPCTSTR g_lpszMatrox_TapsDirection[EDeviceMatrox_TapsDirection_Count] =
{
	_T("LeftTop"),
	_T("RightTop"),
	_T("LeftBottom"),
	_T("RightBottom"),
};

static LPCTSTR g_lpszMatrox_VideoSignalType[EDeviceMatrox_VideoSignalType_Count] =
{
	_T("Digital"),
	_T("Analog"),
};

static LPCTSTR g_lpszMatrox_Databuswidth[EDeviceMatrox_DataBusWidth_Count] =
{
	_T("8 bits"),
	_T("10 bits"),
	_T("12 bits"),
	_T("14 bits"),
	_T("16 bits"),
	_T("24 bits"),
	_T("32 bits"),
	_T("64 bits"),
};

static LPCTSTR g_lpszMatrox_Standard[EDeviceMatrox_Standard_Count] =
{
	_T("Monochrome"),
	_T("RGB Color"),
	_T("RGB Pack"),
	_T("RGB Alpha"),
	_T("SVID"),
	_T("YUVVID"),
};

static LPCTSTR g_lpszMatrox_MILChannel[EDeviceMatrox_MILChannel_Count] =
{
	_T("0"),
	_T("1"),
	_T("2"),
	_T("3"),
};

static LPCTSTR g_lpszMatrox_InoutFormat[EDeviceMatrox_InoutFormat_Count] =
{
	_T("TTL"),
	_T("422"),
	_T("OPTO"),
	_T("LVDS"),
};

static LPCTSTR g_lpszMatrox_InoutFormatDefault[EDeviceMatrox_InoutFormatDefault_Count] =
{
	_T("TTL"),
	_T("422"),
	_T("OPTO"),
	_T("LVDS"),
	_T("Default"),
};

static LPCTSTR g_lpszMatrox_InoutPolarity[EDeviceMatrox_InoutPolarity_Count] =
{
	_T("Pos. Edge Trig."),
	_T("Neg. Edge Trig."),
};

static LPCTSTR g_lpszMatrox_ValidSignalSource[EDeviceMatrox_ValidSignalSource_Count] =
{
	_T("Use Frame Grabber"),
	_T("Use Camera"),
};

static LPCTSTR g_lpszMatrox_InterlacedSelector[EDeviceMatrox_InterlacedSelector_Count] =
{
	_T("Interlaced"),
	_T("Non-Interlaced"),
};

static LPCTSTR g_lpszMatrox_AdjustIntheVideoTimingsPage[EDeviceMatrox_AdjustInTheVideoTimingsPage_Count] =
{
	_T("uSec"),
	_T("Pclk"),
};

static LPCTSTR g_lpszMatrox_ExternalClockSignal[EDeviceMatrox_ExternalClockSignal_Count] =
{
	_T("No Clock Exchange"),
	_T("Generated by Camera"),
	_T("Generated by Digitizer"),
	_T("Generated by Digitizer and Returned by Camera"),
};

static LPCTSTR g_lpszMatrox_PixelClockFrequency[EDeviceMatrox_PixelClockFrequency_Count] =
{
	_T("1 * Pixel Clock"),
};

static LPCTSTR g_lpszMatrox_GenerationMode[EDeviceMatrox_GenerationMode_Count] =
{
	_T("Disable"),
	_T("Periodic"),
	_T("On Trigger Event"),
};

static LPCTSTR g_lpszMatrox_ExposureClockType[EDeviceMatrox_ExposureClockType_Count] =
{
	_T("Synchronous to Pixel Clock"),
	_T("Clock Generator"),
	_T("HSync Clock"),
	_T("VSync Clock"),
	_T("Timer 2 Output"),
	_T("Aux1 Input LVDS"),
};

static LPCTSTR g_lpszMatrox_ExposureTriggerSignal[EDeviceMatrox_ExposureTriggerSignal_Count] =
{
	_T("PSG HSync"),
	_T("PSG VSync"),
	_T("Software"),
	_T("Timer 2 Output"),
	_T("Auxiliary IO 0 [common]"),
	_T("Auxiliary IO 1 [common]"),
	_T("Auxiliary IO 2 [common]"),
	_T("Auxiliary IO 3 [common]"),
	_T("Auxiliary IO 4 [common]"),
	_T("Auxiliary IO 5 [common]"),
	_T("Auxiliary IO 6"),
	_T("Auxiliary IO 7"),
	_T("Auxiliary IO 8"),
	_T("Auxiliary IO 9"),
	_T("Auxiliary IO 10"),
	_T("Auxiliary IO 11"),
	_T("Rotary Encoder Foward Trigger"),
	_T("Rotary Encoder Reversed Trigger"),
};

static LPCTSTR g_lpszMatrox_TriggerArmSource[EDeviceMatrox_TriggerArmSource_Count] =
{
	_T("Timer 1 = 0"),
	_T("Software Arm"),
	_T("PSG HSync"),
	_T("PSG VSync"),
	_T("Timer Output"),
	_T("Auxiliary IO 0 [common]"),
	_T("Auxiliary IO 1 [common]"),
	_T("Auxiliary IO 2 [common]"),
	_T("Auxiliary IO 3 [common]"),
	_T("Auxiliary IO 4 [common]"),
	_T("Auxiliary IO 5 [common]"),
	_T("Auxiliary IO 6"),
	_T("Auxiliary IO 7"),
	_T("Auxiliary IO 8"),
	_T("Auxiliary IO 9"),
	_T("Auxiliary IO 10"),
	_T("Auxiliary IO 11"),
	_T("Rotary Encoder Foward Trigger"),
	_T("Rotary Encoder Reversed Trigger"),
};

static LPCTSTR g_lpszMatrox_TriggerArmSignal[EDeviceMatrox_TriggerArmSignal_Count] =
{
	_T("FrameRetrigger"),
	_T("HardwareIgnoreFrameRetrigger"),
	_T("SoftwareIgnoreFrameRetrigger"),
	_T("FrameRetriggerLatched"),
};


static LPCTSTR g_lpszMatrox_DivisionFactor[EDeviceMatrox_DivisionFactor_Count] =
{
	_T("Base Clock / 1"),
	_T("Base Clock / 2"),
	_T("Base Clock / 4"),
	_T("Base Clock / 8"),
	_T("Base Clock / 16"),
};

static LPCTSTR g_lpszMatrox_TimerOutputSignal[EDeviceMatrox_TimerOutputSignal_Count] =
{
	_T("Timer1"),
	_T("Timer1 XOR Timer2"),
	_T("Timer2"),
};

static LPCTSTR g_lpszMatrox_Captureatriggereverytrigger[EDeviceMatrox_CaptureATriggerEveryTrigger_Count] =
{
	_T("1"),
	_T("2"),
	_T("4"),
	_T("8"),
	_T("16"),
};

static LPCTSTR g_lpszMatrox_TriggerArmMode[EDeviceMatrox_TriggerArmMode_Count] =
{
	_T("Enable"),
	_T("Disable"),
};

static LPCTSTR g_lpszMatrox_GrabMode[EDeviceMatrox_GrabMode_Count] =
{
	_T("Continuous"),
	_T("Software Trigger"),
	_T("Hardware Trigger"),
};

static LPCTSTR g_lpszMatrox_GrabModeActivation[EDeviceMatrox_GrabModeActivation_Count] =
{
	_T("Next Valid Frame [or Field]"),
	_T("Asynchronous Reset"),
	_T("Asynchronous Reset Delayed 1 Frame"),
};

static LPCTSTR g_lpszMatrox_GrabModeSignal[EDeviceMatrox_GrabModeSignal_Count] =
{
	_T("PSH VSync"),
	_T("Timer 1 Output"),
	_T("Timer 2 Output"),
	_T("Auxiliary IO 0 [common]"),
	_T("Auxiliary IO 1 [common]"),
	_T("Auxiliary IO 2 [common]"),
	_T("Auxiliary IO 3 [common]"),
	_T("Auxiliary IO 4 [common]"),
	_T("Auxiliary IO 5 [common]"),
	_T("Auxiliary IO 6"),
	_T("Auxiliary IO 7"),
	_T("Auxiliary IO 8"),
	_T("Auxiliary IO 9"),
	_T("Auxiliary IO 10"),
	_T("Auxiliary IO 11"),
	_T("Rotary Encoder Foward Trigger"),
	_T("Rotary Encoder Reversed Trigger"),
};

static LPCTSTR g_lpszMatrox_SyncSignalSource[EDeviceMatrox_SyncSignalSource_Count] =
{
	_T("Digitizer"),
	_T("Camera"),
};

static LPCTSTR g_lpszMatrox_SyncSignalFormat[EDeviceMatrox_SyncSignalFormat_Count] =
{
	_T("Digital"),
	_T("Analog"),
};

static LPCTSTR g_lpszMatrox_SynchronizationSignalAvailable[EDeviceMatrox_SynchronizAtionSignalAvailable_Count] =
{
	_T("CSync"),
	_T("HSync Only"),
	_T("VSync Only"),
	_T("HSync & VSync"),
};

static LPCTSTR g_lpszMatrox_CameraBitsSourcesControl[EDeviceMatrox_CameraBitsSourcesControl_Count] =
{
	_T("Timer 1 Output"),
	_T("Timer 2 Output"),
	_T("User 0 Output HIGH"),
	_T("User 0 Output LOW"),
	_T("User 1 Output HIGH"),
	_T("User 1 Output LOW"),
	_T("PSG VSYNC"),
	_T("PSG HSYNC"),
	_T("Pixel Clock"),
};

static LPCTSTR g_lpszMatrox_DirectionSwitch[EDeviceMatrox_DirectionSwitch_Count] =
{
	_T("Default"),
	_T("Forward"),
	_T("Reverse"),
};

static LPCTSTR g_lpszMatrox_ActiveSwitch[EDeviceMatrox_ActiveSwitch_Count] =
{
	_T("Default"),
	_T("Disable"),
	_T("Enable"),
};

static LPCTSTR g_lpszMatrox_ActivationSelect[EDeviceMatrox_ActivationSelect_Count] =
{
	_T("Default"),
	_T("Edge Falling"),
	_T("Edge Rising"),
	_T("Level High"),
	_T("Level Low"),
};

static LPCTSTR g_lpszMatrox_TriggerSourceSelect[EDeviceMatrox_TriggerSourceSelect_Count] =
{
	_T("AUXIO0"),
	_T("AUXIO1"),
	_T("AUXIO2"),
	_T("AUXIO3"),
	_T("AUXIO4"),
	_T("AUXIO5"),
	_T("AUXIO6"),
	_T("AUXIO7"),
	_T("AUXIO8"),
	_T("AUXIO9"),
	_T("AUXIO10"),
	_T("AUXIO11"),
	_T("CCIO1"),
	_T("CCIO2"),
	_T("CCIO3"),
	_T("CCIO4"),
	_T("Default"),
	_T("Hsync"),
	_T("RotaryEncoder"),
	_T("Software"),
	_T("Timer0"),
	_T("Timer1"),
	_T("Timer2"),
	_T("Timer3"),
	_T("Timer4"),
	_T("Timer5"),
	_T("Timer6"),
	_T("Timer7"),
	_T("Timer8"),
	_T("Timer9"),
	_T("Timer10"),
	_T("Timer11"),
	_T("Timer12"),
	_T("Timer13"),
	_T("Timer14"),
	_T("Timer15"),
	_T("Timer16"),
	_T("Vsync"),
};

static LPCTSTR g_lpszMatrox_TimerSelect[EDeviceMatrox_TimerSelect_Count] =
{
	_T("NULL"),
	_T("Timer1"),
	_T("Timer2"),
	_T("Timer3"),
	_T("Timer4"),
};


static LPCTSTR g_lpszMatrox_Switch[EDeviceMatrox_Switch_Count] =
{
	_T("Off"),
	_T("On"),
};



CDeviceMatroxIntellicam::CDeviceMatroxIntellicam()
{
	m_hMilSystem = M_NULL;
	m_hMilDigitizer = M_NULL;
	m_hMilImage = M_NULL;
	m_pMilImageList = nullptr;
}

CDeviceMatroxIntellicam::~CDeviceMatroxIntellicam()
{
	Terminate();
}

EDeviceInitializeResult CDeviceMatroxIntellicam::Initialize()
{
	EDeviceInitializeResult eReturn = EDeviceInitializeResult_UnknownError;

	CString strMessage, strStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("MatroxIntellicam"));
		
		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);

		SetStatus(strStatus);

		return EDeviceInitializeResult_NotFoundApiError;
	}

	do
	{
		if(IsInitialized())
		{
			strStatus = CMultiLanguageManager::GetString(ELanguageParameter_Initialized);

			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Thedevicehasbeenalreadyinitialized);

			eReturn = EDeviceInitializeResult_AlreadyInitializedError;

			break;
		}

		if(GetObjectID() < 0)
		{
			strMessage.Format(_T("Couldn't support ObjectID"));
			eReturn = EDeviceInitializeResult_NotSupportedObjectID;
			break;
		}

		if(!m_hMilApplication)
		{
			bool bAlreadyBeenAllocated = false;

			for(int i = 0; i < (int)CDeviceManager::GetDeviceCount(); ++i)
			{
				CDeviceBase* pInstance = CDeviceManager::GetDeviceByIndex(i);

				if(!pInstance)
					continue;

				CDeviceMatroxIntellicam* pMatrox = dynamic_cast<CDeviceMatroxIntellicam*>(pInstance);

				if(!pMatrox)
					continue;

				if(pMatrox == this)
					continue;

				if(pMatrox->m_hMilApplication != M_NULL)
				{
					m_hMilApplication = pMatrox->m_hMilApplication;
					bAlreadyBeenAllocated = true;
					break;
				}
			}

			if(!bAlreadyBeenAllocated)
				MappAlloc(M_DEFAULT, &m_hMilApplication);

			if(!m_hMilApplication)
			{
				strMessage.Format(_T("Couldn't allocate Mil application"));
				eReturn = EDeviceInitializeResult_AllocateMemoryError;
				break;
			}
		}

		strMessage.Format(_T("Succeeded to Initialize Matrox Intellicam"));

		eReturn = EDeviceInitializeResult_OK;
	}
	while(false);

	SetStatus(strStatus);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceTerminateResult CDeviceMatroxIntellicam::Terminate()
{
	EDeviceTerminateResult eReturn = EDeviceTerminateResult_UnknownError;

	CString strMessage;

	SetStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("MatroxIntellicam"));

		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);

		return EDeviceTerminateResult_NotFoundApiError;
	}

	do
	{
		if(!IsGrabAvailable())
			Stop();

		m_bIsInitialized = false;
		m_bIsGrabAvailable = true;
		m_nConnectBoard = 0;

		m_nDriveIndex = -1;
		m_nCurrentCount = -1;

		if(m_pMilImageList)
		{
			int nCount = 0;

			GetMILImageCount(&nCount);

			for(int i = 0; i < nCount; ++i)
			{
				if(m_pMilImageList[i] != M_NULL)
					MbufFree(m_pMilImageList[i]);
			}

			delete[] m_pMilImageList;
			m_pMilImageList = nullptr;
		}

		if(m_hMilImage)
		{
			MbufFree(m_hMilImage);
			m_hMilImage = M_NULL;
		}

		if(m_hMilDigitizer)
		{
			MdigFree(m_hMilDigitizer);
			m_hMilDigitizer = M_NULL;
		}

		if(m_hMilSystem)
		{
			bool bNeedSystemFree = true;

			for(int i = 0; i < (int)CDeviceManager::GetDeviceCount(); ++i)
			{
				CDeviceBase* pInstance = CDeviceManager::GetDeviceByIndex(i);

				if(!pInstance)
					continue;

				CDeviceMatroxIntellicam* pMatrox = dynamic_cast<CDeviceMatroxIntellicam*>(pInstance);

				if(!pMatrox)
					continue;

				if(pMatrox == this)
					continue;

				if(_ttoi(GetDeviceID()) != _ttoi(pMatrox->GetDeviceID()))
					continue;

				if(!pMatrox->IsInitialized())
					continue;

				if(pMatrox->m_hMilApplication == M_NULL)
					continue;

				bNeedSystemFree = false;
				break;				
			}

			if(bNeedSystemFree)
				MsysFree(m_hMilSystem);

			m_hMilSystem = M_NULL;
		}

		if(m_hMilApplication)
		{
			bool bNeedApplicationFree = true;

			for(int i = 0; i < (int)CDeviceManager::GetDeviceCount(); ++i)
			{
				CDeviceBase* pInstance = CDeviceManager::GetDeviceByIndex(i);

				if(!pInstance)
					continue;

				CDeviceMatroxIntellicam* pMatrox = dynamic_cast<CDeviceMatroxIntellicam*>(pInstance);

				if(!pMatrox)
					continue;

				if(pMatrox == this)
					continue;

				if(!pMatrox->IsInitialized())
					continue;

				if(pMatrox->m_hMilApplication == M_NULL)
					continue;
				
				bNeedApplicationFree = false;
				break;
			}

			if(bNeedApplicationFree)
			{
				MappFree(m_hMilApplication);
				m_hMilApplication = M_NULL;
			}
		}

		strMessage = _T("Succeeded to terminate Device");

		CEventHandlerManager::BroadcastOnDeviceTerminated(this);
	}
	while(false);

	this->ClearBuffer();

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceMatroxIntellicam::LoadSettings()
{
	bool bReturn = false;

	do
	{
		AddParameterFieldConfigurations(EDeviceParameterMatrox_DeviceID, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_DeviceID], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_SubUnitID, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_SubUnitID], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_CameraName, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_CameraName], _T("SampleCamera"), EParameterFieldType_Edit, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_InitializeType, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_InitializeType], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_InitializeType, EDeviceMatrox_InitilizeType_Count), nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Camfilepath, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Camfilepath], _T("-"), EParameterFieldType_Edit, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Parametermatch, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Parametermatch], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_GrabCount, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_GrabCount], _T("1"), EParameterFieldType_Edit, nullptr, _T("Count of grab command"), 0);
		
		AddParameterFieldConfigurations(EDeviceParameterMatrox_MILImageCount, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_MILImageCount], _T("1"), EParameterFieldType_Edit, nullptr, _T("Count of Temporary save MIL images"), 0);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_MILBufferCount, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_MILBufferCount], _T("1"), EParameterFieldType_Edit, nullptr, _T("Count of MIL image buffers"), 0);

		AddParameterFieldConfigurations(EDeviceParameterMatrox_GrabSetting, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_GrabSetting], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_GrabDirectionX, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_GrabDirectionX], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_DirectionSwitch, EDeviceMatrox_DirectionSwitch_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_GrabDirectionY, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_GrabDirectionY], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_DirectionSwitch, EDeviceMatrox_DirectionSwitch_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_GrabScale, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_GrabScale], _T("1.000000"), EParameterFieldType_Edit, nullptr, _T("0.000000 ~ 1.000000"), 1);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_GrabScaleX, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_GrabScaleX], _T("1.000000"), EParameterFieldType_Edit, nullptr, _T("0.000000 ~ 1.000000"), 1);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_GrabScaleY, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_GrabScaleY], _T("1.000000"), EParameterFieldType_Edit, nullptr, _T("0.000000 ~ 1.000000"), 1);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_SourceOffsetX, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_SourceOffsetX], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_SourceOffsetY, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_SourceOffsetY], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_SourceSizeX, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_SourceSizeX], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_SourceSizeY, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_SourceSizeY], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterMatrox_GrabTriggerSetting, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_GrabTriggerSetting], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_GrabTriggerContinuouesEndTrigger, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_GrabTriggerContinuouesEndTrigger], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_ActiveSwitch, EDeviceMatrox_ActiveSwitch_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_GrabTriggerActivation, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_GrabTriggerActivation], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_ActivationSelect, EDeviceMatrox_ActivationSelect_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_GrabTriggerSoursce, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_GrabTriggerSoursce], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_TriggerSourceSelect, EDeviceMatrox_TriggerSourceSelect_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_GrabTriggerState, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_GrabTriggerState], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_ActiveSwitch, EDeviceMatrox_ActiveSwitch_Count), nullptr, 1);

// 		AddParameterFieldConfigurations(EDeviceParameterMatrox_TimerSetting, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_TimerSetting], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
// 		AddParameterFieldConfigurations(EDeviceParameterMatrox_TimerSelector, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_TimerSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_TimerSelect, EDeviceMatrox_TimerSelect_Count), nullptr, 1);
// 
// 		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer1, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Timer1], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
// 
// 		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer2, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Timer2], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
// 
// 		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer3, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Timer3], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
// 
// 		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer4, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Timer4], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
// 
// 		
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Camera, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Camera], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_CameraSettings, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_CameraSettings], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_CameraType, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_CameraType], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_CameraType, EDeviceMatrox_CameraType_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_CameraNumberOfTaps, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_CameraNumberOfTaps], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_Numberoftaps, EDeviceMatrox_NumberOfTaps_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_CameraBayerMode, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_CameraBayerMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_Bayermode, EDeviceMatrox_BayerMode_Count), nullptr, 2);
		
		AddParameterFieldConfigurations(EDeviceParameterMatrox_CameraLinkConfiguration, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_CameraLinkConfiguration], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_ConfigurationType, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_ConfigurationType], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_ConfigurationType, EDeviceMatrox_ConfigurationType_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_CameraMode, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_CameraMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_CameraMode, EDeviceMatrox_CameraTapsMode_Count), nullptr, 2);
		
		AddParameterFieldConfigurations(EDeviceParameterMatrox_TapConfiguration, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_TapConfiguration], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Regions, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Regions], _T("1"), EParameterFieldType_None, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_RegionsX, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_RegionsX], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_TapsSelector, EDeviceMatrox_TapsSelector_Count), nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_RegionsY, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_RegionsY], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_TapsSelector, EDeviceMatrox_TapsSelector_Count), nullptr, 3);
		
		AddParameterFieldConfigurations(EDeviceParameterMatrox_AdjacentPixels, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_AdjacentPixels], _T("1"), EParameterFieldType_None, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_AdjacentX, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_AdjacentX], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_TapsSelector, EDeviceMatrox_TapsSelector_Count), nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_AdjacentY, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_AdjacentY], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_TapsSelector, EDeviceMatrox_TapsSelector_Count), nullptr, 3);
		
		AddParameterFieldConfigurations(EDeviceParameterMatrox_TapsDirection, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_TapsDirection], _T("1"), EParameterFieldType_None, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Tap1, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Tap1], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_TapsDirection, EDeviceMatrox_TapsDirection_Count), nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Tap2, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Tap2], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_TapsDirection, EDeviceMatrox_TapsDirection_Count), nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Tap3, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Tap3], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_TapsDirection, EDeviceMatrox_TapsDirection_Count), nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Tap4, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Tap4], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_TapsDirection, EDeviceMatrox_TapsDirection_Count), nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Tap5, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Tap5], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_TapsDirection, EDeviceMatrox_TapsDirection_Count), nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Tap6, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Tap6], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_TapsDirection, EDeviceMatrox_TapsDirection_Count), nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Tap7, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Tap7], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_TapsDirection, EDeviceMatrox_TapsDirection_Count), nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Tap8, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Tap8], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_TapsDirection, EDeviceMatrox_TapsDirection_Count), nullptr, 3);
		
		AddParameterFieldConfigurations(EDeviceParameterMatrox_VideoSignal, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_VideoSignal], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_VideoSignalInformation, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_VideoSignalInformation], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_VideoSignalType, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_VideoSignalType], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_VideoSignalType, EDeviceMatrox_VideoSignalType_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_VideoSignalDatabusWidth, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_VideoSignalDatabusWidth], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_Databuswidth, EDeviceMatrox_DataBusWidth_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_VideoSignalStandard, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_VideoSignalStandard], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_Standard, EDeviceMatrox_Standard_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_VideoSignalMILChannelInput, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_VideoSignalMILChannelInput], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_MILChannel, EDeviceMatrox_MILChannel_Count), nullptr, 2);
		
		AddParameterFieldConfigurations(EDeviceParameterMatrox_DigitalVideoSignalInformation, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_DigitalVideoSignalInformation], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_DigitalVideoSignalFormat, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_DigitalVideoSignalFormat], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_InoutFormat, EDeviceMatrox_InoutFormat_Count), nullptr, 2);
		
		AddParameterFieldConfigurations(EDeviceParameterMatrox_VideoTiming, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_VideoTiming], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_ValidSignalSource, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_ValidSignalSource], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_ValidSignalSource, EDeviceMatrox_ValidSignalSource_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterMatrox_UseFrameGrabber, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_UseFrameGrabber], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timings, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Timings], _T("1"), EParameterFieldType_None, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_TimingsVertical, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_TimingsVertical], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_InterlacedSelector, EDeviceMatrox_InterlacedSelector_Count), nullptr, 3);
		
		AddParameterFieldConfigurations(EDeviceParameterMatrox_PixelClockFrqAutoAdjust, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_PixelClockFrqAutoAdjust], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterMatrox_Horizontal, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Horizontal], _T("1"), EParameterFieldType_None, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_HorizontalSyncClk, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_HorizontalSyncClk], _T("0"), EParameterFieldType_Edit, nullptr, _T("int"), 3);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_HorizontalBPorchClk, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_HorizontalBPorchClk], _T("0"), EParameterFieldType_Edit, nullptr, _T("int"), 3);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_HorizontalActiveClk, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_HorizontalActiveClk], _T("0"), EParameterFieldType_Edit, nullptr, _T("int"), 3);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_HorizontalFPorchClk, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_HorizontalFPorchClk], _T("0"), EParameterFieldType_Edit, nullptr, _T("int"), 3);

		AddParameterFieldConfigurations(EDeviceParameterMatrox_HorizontalFrequencyHz, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_HorizontalFrequencyHz], _T("0.000000"), EParameterFieldType_Edit, nullptr, _T("double"), 3);

		AddParameterFieldConfigurations(EDeviceParameterMatrox_HorizontalLockActiveAndTotal, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_HorizontalLockActiveAndTotal], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Vertical, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Vertical], _T("1"), EParameterFieldType_None, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_VerticalSyncLine, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_VerticalSyncLine], _T("0"), EParameterFieldType_Edit, nullptr, _T("int"), 3);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_VerticalBPorchLine, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_VerticalBPorchLine], _T("0"), EParameterFieldType_Edit, nullptr, _T("int"), 3);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_VerticalActiveLine, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_VerticalActiveLine], _T("0"), EParameterFieldType_Edit, nullptr, _T("int"), 3);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_VerticalFPorchLine, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_VerticalFPorchLine], _T("0"), EParameterFieldType_Edit, nullptr, _T("int"), 3);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_VerticalFrequencyHz, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_VerticalFrequencyHz], _T("0.000000"), EParameterFieldType_Edit, nullptr, _T("double"), 3);

		AddParameterFieldConfigurations(EDeviceParameterMatrox_VerticalLockActiveAndTotal, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_VerticalLockActiveAndTotal], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 3);
		
		AddParameterFieldConfigurations(EDeviceParameterMatrox_UseCamera, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_UseCamera], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_CameralLinkSettings, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_CameralLinkSettings], _T("1"), EParameterFieldType_None, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_ImagesizeX, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_ImagesizeX], _T("0"), EParameterFieldType_Edit, nullptr, _T("int"), 3);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_ImagesizeY, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_ImagesizeY], _T("0"), EParameterFieldType_Edit, nullptr, _T("int"), 3);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_DelayX, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_DelayX], _T("0"), EParameterFieldType_Edit, nullptr, _T("int"), 3);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_DelayY, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_DelayY], _T("0"), EParameterFieldType_Edit, nullptr, _T("int"), 3);
		
		AddParameterFieldConfigurations(EDeviceParameterMatrox_PixelClock, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_PixelClock], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_PixelClockFrequency, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_PixelClockFrequency], _T("1.000000"), EParameterFieldType_None, nullptr, _T("double"), 1);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_PixelClockFrequencyMHz, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_PixelClockFrequencyMHz], _T("0.000000"), EParameterFieldType_Edit, nullptr, _T("double"), 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_AutoAdjustVideoTimingsPage, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_AutoAdjustVideoTimingsPage], _T("0"), EParameterFieldType_Radio, ConvertStringArrayToComboElement(
					g_lpszMatrox_AdjustIntheVideoTimingsPage, EDeviceMatrox_AdjustInTheVideoTimingsPage_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_IOCharacteristics, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_IOCharacteristics], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_SendToExternalCircuit, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_SendToExternalCircuit], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_ExternalClockSignal, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_ExternalClockSignal], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_ExternalClockSignal, EDeviceMatrox_ExternalClockSignal_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_EXTInput, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_EXTInput], _T("1"), EParameterFieldType_None, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_EXTInputFrequency, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_EXTInputFrequency], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_PixelClockFrequency, EDeviceMatrox_PixelClockFrequency_Count), nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_EXTInputFormat, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_EXTInputFormat], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_InoutFormat, EDeviceMatrox_InoutFormat_Count), nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_EXTInputPolarity, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_EXTInputPolarity], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_InoutPolarity, EDeviceMatrox_InoutPolarity_Count), nullptr, 3);
		
		AddParameterFieldConfigurations(EDeviceParameterMatrox_EXTOutput, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_EXTOutput], _T("1"), EParameterFieldType_None, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_EXTOutputFrequency, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_EXTOutputFrequency], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_PixelClockFrequency, EDeviceMatrox_PixelClockFrequency_Count), nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_EXTOutputFormat, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_EXTOutputFormat], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_InoutFormat, EDeviceMatrox_InoutFormat_Count), nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_EXTOutputPolarity, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_EXTOutputPolarity], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_InoutPolarity, EDeviceMatrox_InoutPolarity_Count), nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_EXTIODelay, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_EXTIODelay], _T("10000"), EParameterFieldType_Edit, nullptr, nullptr, 3);
		
		AddParameterFieldConfigurations(EDeviceParameterMatrox_ExposureSignal, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_ExposureSignal], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_ExpTimer1, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_ExpTimer1], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer1Generation, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Timer1Generation], _T("1"), EParameterFieldType_None, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer1Mode, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Timer1Mode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_GenerationMode, EDeviceMatrox_GenerationMode_Count), nullptr, 3);

		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer1ExpSignal, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Timer1ExpSignal], _T("1"), EParameterFieldType_None, nullptr, nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer1ExpFormat, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Timer1ExpFormat], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_InoutFormatDefault, EDeviceMatrox_InoutFormatDefault_Count), nullptr, 4);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer1ExpPolarity, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Timer1ExpPolarity], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_InoutPolarity, EDeviceMatrox_InoutPolarity_Count), nullptr, 4);
		
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer1TriggerInfo, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Timer1TriggerInfo], _T("1"), EParameterFieldType_None, nullptr, nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer1TrgFormat, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Timer1TrgFormat], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_InoutFormatDefault, EDeviceMatrox_InoutFormatDefault_Count), nullptr, 4);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer1TrgSignal, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Timer1TrgSignal], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_ExposureTriggerSignal, EDeviceMatrox_ExposureTriggerSignal_Count), nullptr, 4);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer1TrgPolarity, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Timer1TrgPolarity], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_InoutPolarity, EDeviceMatrox_InoutPolarity_Count), nullptr, 4);
		
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer1Pulse, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Timer1Pulse], _T("1"), EParameterFieldType_None, nullptr, nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer1Pulse1Clk, g_lpszDeviceParameterMatrox [EDeviceParameterMatrox_Timer1Pulse1Clk], _T("0"), EParameterFieldType_Edit, nullptr, _T("int"), 4);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer1Delay1Clk, g_lpszDeviceParameterMatrox [EDeviceParameterMatrox_Timer1Delay1Clk], _T("0"), EParameterFieldType_Edit, nullptr, _T("int"), 4);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer1Pulse2Clk, g_lpszDeviceParameterMatrox [EDeviceParameterMatrox_Timer1Pulse2Clk], _T("0"), EParameterFieldType_Edit, nullptr, _T("int"), 4);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer1Delay2Clk, g_lpszDeviceParameterMatrox [EDeviceParameterMatrox_Timer1Delay2Clk], _T("0"), EParameterFieldType_Edit, nullptr, _T("int"), 4);
		
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer1ExposureClock, g_lpszDeviceParameterMatrox [EDeviceParameterMatrox_Timer1ExposureClock], _T("1"), EParameterFieldType_None, nullptr, nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer1Type, g_lpszDeviceParameterMatrox  [EDeviceParameterMatrox_Timer1Type], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement   (g_lpszMatrox_ExposureClockType, EDeviceMatrox_ExposureClockType_Count), nullptr, 4);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer1DivisionFactor, g_lpszDeviceParameterMatrox	[EDeviceParameterMatrox_Timer1DivisionFactor], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement   (g_lpszMatrox_DivisionFactor, EDeviceMatrox_DivisionFactor_Count), nullptr, 4);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer1BaseClockFrequency, g_lpszDeviceParameterMatrox [EDeviceParameterMatrox_Timer1BaseClockFrequency], _T("0.000000"), EParameterFieldType_Edit, nullptr, _T("double"), 4);
		
		AddParameterFieldConfigurations(EDeviceParameterMatrox_ExpTimer2, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_ExpTimer2], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer2Generation, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Timer2Generation], _T("1"), EParameterFieldType_None, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer2Mode, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Timer2Mode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_GenerationMode, EDeviceMatrox_GenerationMode_Count), nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer2ExpSignal, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Timer2ExpSignal], _T("1"), EParameterFieldType_None, nullptr, nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer2ExpFormat, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Timer2ExpFormat], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_InoutFormatDefault, EDeviceMatrox_InoutFormatDefault_Count), nullptr, 4);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer2ExpPolarity, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Timer2ExpPolarity], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_InoutPolarity, EDeviceMatrox_InoutPolarity_Count), nullptr, 4);
		
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer2TriggerInfo, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Timer2TriggerInfo], _T("1"), EParameterFieldType_None, nullptr, nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer2TrgFormat, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Timer2TrgFormat], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_InoutFormatDefault, EDeviceMatrox_InoutFormatDefault_Count), nullptr, 4);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer2TrgSignal, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Timer2TrgSignal], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_ExposureTriggerSignal, EDeviceMatrox_ExposureTriggerSignal_Count), nullptr, 4);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer2TrgPolarity, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Timer2TrgPolarity], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_InoutPolarity, EDeviceMatrox_InoutPolarity_Count), nullptr, 4);
		
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer2Pulse, g_lpszDeviceParameterMatrox [EDeviceParameterMatrox_Timer2Pulse], _T("1"), EParameterFieldType_None, nullptr, nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer2Pulse1Clk, g_lpszDeviceParameterMatrox [EDeviceParameterMatrox_Timer2Pulse1Clk], _T("0"), EParameterFieldType_Edit, nullptr, _T("int"), 4);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer2Delay1Clk, g_lpszDeviceParameterMatrox [EDeviceParameterMatrox_Timer2Delay1Clk], _T("0"), EParameterFieldType_Edit, nullptr, _T("int"), 4);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer2Pulse2Clk, g_lpszDeviceParameterMatrox [EDeviceParameterMatrox_Timer2Pulse2Clk], _T("0"), EParameterFieldType_Edit, nullptr, _T("int"), 4);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer2Delay2Clk, g_lpszDeviceParameterMatrox [EDeviceParameterMatrox_Timer2Delay2Clk], _T("0"), EParameterFieldType_Edit, nullptr, _T("int"), 4);
		
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer2ExposureClock, g_lpszDeviceParameterMatrox [EDeviceParameterMatrox_Timer2ExposureClock], _T("1"), EParameterFieldType_None, nullptr, nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer2Type, g_lpszDeviceParameterMatrox  [EDeviceParameterMatrox_Timer2Type], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement   (g_lpszMatrox_ExposureClockType, EDeviceMatrox_ExposureClockType_Count), nullptr, 4);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer2DivisionFactor, g_lpszDeviceParameterMatrox	[EDeviceParameterMatrox_Timer2DivisionFactor], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement   (g_lpszMatrox_DivisionFactor, EDeviceMatrox_DivisionFactor_Count), nullptr, 4);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer2BaseClockFrequency, g_lpszDeviceParameterMatrox	[EDeviceParameterMatrox_Timer2BaseClockFrequency], _T("0.000000"), EParameterFieldType_Edit, nullptr, nullptr, 4);
				
		AddParameterFieldConfigurations(EDeviceParameterMatrox_ExpTimer1Advanced, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_ExpTimer1Advanced], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer1Options, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Timer1Options], _T("1"), EParameterFieldType_None, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer1TimerOutputSignal, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Timer1TimerOutputSignal], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_TimerOutputSignal, EDeviceMatrox_TimerOutputSignal_Count), nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer1CaptureTrigger, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Timer1CaptureTrigger], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_Captureatriggereverytrigger, EDeviceMatrox_CaptureATriggerEveryTrigger_Count), nullptr, 3);
		
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer1TriggerArm, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Timer1TriggerArm], _T("1"), EParameterFieldType_None, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer1TrgArmMode, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Timer1TrgArmMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_TriggerArmMode, EDeviceMatrox_TriggerArmMode_Count), nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer1TrgArmFormat, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Timer1TrgArmFormat], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_InoutFormatDefault, EDeviceMatrox_InoutFormatDefault_Count), nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer1TrgArmSource, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Timer1TrgArmSource], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_TriggerArmSource, EDeviceMatrox_TriggerArmSource_Count), nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer1TrgArmPolarity, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Timer1TrgArmPolarity], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_InoutPolarity, EDeviceMatrox_InoutPolarity_Count), nullptr, 3);
		
		AddParameterFieldConfigurations(EDeviceParameterMatrox_ExpTimer2Advanced, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_ExpTimer2Advanced], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer2Options, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Timer2Options], _T("1"), EParameterFieldType_None, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer2TimerOutputSignal, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Timer2TimerOutputSignal], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_TimerOutputSignal, EDeviceMatrox_TimerOutputSignal_Count), nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer2CaptureTrigger, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Timer2CaptureTrigger], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_Captureatriggereverytrigger, EDeviceMatrox_CaptureATriggerEveryTrigger_Count), nullptr, 3);
		
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer2TriggerArm, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Timer2TriggerArm], _T("1"), EParameterFieldType_None, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer2TrgArmMode, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Timer2TrgArmMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_TriggerArmMode, EDeviceMatrox_TriggerArmMode_Count), nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer2TrgArmFormat, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Timer2TrgArmFormat], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_InoutFormatDefault, EDeviceMatrox_InoutFormatDefault_Count), nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer2TrgArmSource, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Timer2TrgArmSource], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_TriggerArmSource, EDeviceMatrox_TriggerArmSource_Count), nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Timer2TrgArmPolarity, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Timer2TrgArmPolarity], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_InoutPolarity, EDeviceMatrox_InoutPolarity_Count), nullptr, 3);
		
		AddParameterFieldConfigurations(EDeviceParameterMatrox_GrabMode, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_GrabMode], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_GrabCharacteristics, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_GrabCharacteristics], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_GrabCharacteristicsMode, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_GrabCharacteristicsMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_GrabMode, EDeviceMatrox_GrabMode_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_GrabActivationMode, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_GrabActivationMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_GrabModeActivation, EDeviceMatrox_GrabModeActivation_Count), nullptr, 2);
		
		AddParameterFieldConfigurations(EDeviceParameterMatrox_TriggerSignalCharacteristics, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_TriggerSignalCharacteristics], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_GrabTrgFormat, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_GrabTrgFormat], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_InoutFormatDefault, EDeviceMatrox_InoutFormatDefault_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_GrabTrgSignal, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_GrabTrgSignal], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_GrabModeSignal, EDeviceMatrox_GrabModeSignal_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterMatrox_GrabTrgPolarity, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_GrabTrgPolarity], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_InoutPolarity, EDeviceMatrox_InoutPolarity_Count), nullptr, 2);
		
		AddParameterFieldConfigurations(EDeviceParameterMatrox_TriggerArmCharacteristics, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_TriggerArmCharacteristics], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_GrabTrgArmFormat, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_GrabTrgArmFormat], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_InoutFormatDefault, EDeviceMatrox_InoutFormatDefault_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_GrabTrgArmSignal, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_GrabTrgArmSignal], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_TriggerArmSignal, EDeviceMatrox_TriggerArmSignal_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_GrabTrgArmPolarity, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_GrabTrgArmPolarity], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_InoutPolarity, EDeviceMatrox_InoutPolarity_Count), nullptr, 2);
		
		//////////////////////////////////////////////////////////////////////////
		AddParameterFieldConfigurations(EDeviceParameterMatrox_SyncSignal, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_SyncSignal], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_SourceSpecification, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_SourceSpecification], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_SpcSource, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_SpcSource], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_SyncSignalSource, EDeviceMatrox_SyncSignalSource_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_SpcFormat, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_SpcFormat], _T("0"), EParameterFieldType_Radio, ConvertStringArrayToComboElement(g_lpszMatrox_SyncSignalFormat, EDeviceMatrox_SyncSignalFormat_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_SpcCameraOutInSignalLatency, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_SpcCameraOutInSignalLatency], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_SpcSyncSignalAvailable, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_SpcSyncSignalAvailable], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_SynchronizationSignalAvailable, EDeviceMatrox_SynchronizAtionSignalAvailable_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_SpcBlockSyncTypeEnable, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_SpcBlockSyncTypeEnable], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_SpcExternalVSYNCEnable, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_SpcExternalVSYNCEnable], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 2);
		
		AddParameterFieldConfigurations(EDeviceParameterMatrox_DigitalSynchro, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_DigitalSynchro], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_HSync, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_HSync], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_HSyncInputActive, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_HSyncInputActive], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_HSyncInputFormat, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_HSyncInputFormat], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_InoutFormat, EDeviceMatrox_InoutFormat_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_HSyncInputPolarity, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_HSyncInputPolarity], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_InoutPolarity, EDeviceMatrox_InoutPolarity_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_HSyncOutputActive, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_HSyncOutputActive], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_HSyncOutputFormat, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_HSyncOutputFormat], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_InoutFormat, EDeviceMatrox_InoutFormat_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_HSyncOutputPolarity, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_HSyncOutputPolarity], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_InoutPolarity, EDeviceMatrox_InoutPolarity_Count), nullptr, 2);
		
		AddParameterFieldConfigurations(EDeviceParameterMatrox_VSync, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_VSync], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_VSyncInputActive, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_VSyncInputActive], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_VSyncInputFormat, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_VSyncInputFormat], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_InoutFormat, EDeviceMatrox_InoutFormat_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_VSyncInputPolarity, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_VSyncInputPolarity], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_InoutPolarity, EDeviceMatrox_InoutPolarity_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_VSyncOutputActive, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_VSyncOutputActive], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_VSyncOutputFormat, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_VSyncOutputFormat], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_InoutFormat, EDeviceMatrox_InoutFormat_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_VSyncOutputPolarity, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_VSyncOutputPolarity], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_InoutPolarity, EDeviceMatrox_InoutPolarity_Count), nullptr, 2);
		
		AddParameterFieldConfigurations(EDeviceParameterMatrox_CSync, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_CSync], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_CSyncInputActive, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_CSyncInputActive], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_CSyncInputFormat, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_CSyncInputFormat], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_InoutFormat, EDeviceMatrox_InoutFormat_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_CSyncInputPolarity, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_CSyncInputPolarity], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_InoutPolarity, EDeviceMatrox_InoutPolarity_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_CSyncOutputActive, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_CSyncOutputActive], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_CSyncOutputFormat, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_CSyncOutputFormat], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_InoutFormat, EDeviceMatrox_InoutFormat_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_CSyncOutputPolarity, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_CSyncOutputPolarity], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_InoutPolarity, EDeviceMatrox_InoutPolarity_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_CSyncSerrationPulse, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_CSyncSerrationPulse], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_CSyncEqualizationPulse, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_CSyncEqualizationPulse], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 2);
		
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Other, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Other], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_CameraLinkControlBits, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_CameraLinkControlBits], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_CameraControlBitsSources, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_CameraControlBitsSources], _T("1"), EParameterFieldType_None, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Control1, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Control1], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_CameraBitsSourcesControl, EDeviceMatrox_CameraBitsSourcesControl_Count), nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Control2, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Control2], _T("1"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_CameraBitsSourcesControl, EDeviceMatrox_CameraBitsSourcesControl_Count), nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Control3, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Control3], _T("2"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_CameraBitsSourcesControl, EDeviceMatrox_CameraBitsSourcesControl_Count), nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_Control4, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_Control4], _T("3"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMatrox_CameraBitsSourcesControl, EDeviceMatrox_CameraBitsSourcesControl_Count), nullptr, 3);
		
		AddParameterFieldConfigurations(EDeviceParameterMatrox_SignalsOutput, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_SignalsOutput], _T("1"), EParameterFieldType_None, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_EnableCCoutputsConnector1, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_EnableCCoutputsConnector1], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterMatrox_EnableCCoutputsConnector2, g_lpszDeviceParameterMatrox[EDeviceParameterMatrox_EnableCCoutputsConnector2], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 3);
		
		bReturn = true;
	}
	while(false);

	return bReturn;
}

EDeviceGrabResult CDeviceMatroxIntellicam::Grab()
{
	EDeviceGrabResult eReturn = EDeviceGrabResult_UnknownError;

	CString strMessage;

	do
	{
		if(!IsInitialized())
		{
			strMessage.Format(_T("Failed to initialize the device."));
			eReturn = EDeviceGrabResult_NotInitializedError;
			break;
		}

		if(!IsGrabAvailable())
		{
			strMessage.Format(_T("The device is ready grab image."));
			eReturn = EDeviceGrabResult_AlreadyGrabError;
			break;
		}

		int nGrab = 0;

		if(GetGrabCount(&nGrab))
		{
			strMessage.Format(_T("Couldn't read 'GetGrabCount' from the database"));
			eReturn = EDeviceGrabResult_ReadOnDatabaseError;
			break;
		}

		if(nGrab < 1)
			nGrab = LONG_MAX;

		int nGrabBuffer = 0;

		if(GetMILBufferCount(&nGrabBuffer))
		{
			strMessage.Format(_T("Couldn't read 'GetMILBufferCount' from the database"));
			eReturn = EDeviceGrabResult_ReadOnDatabaseError;
			break;
		}

		if(nGrabBuffer < 1)
		{
			strMessage.Format(_T("Couldn't set 'GetMILBufferCount' to the device"));
			eReturn = EDeviceGrabResult_WriteToDeviceError;
			break;
		}

		m_nCurrentCount = nGrab;

		MdigProcess(m_hMilDigitizer, m_pMilImageList, nGrabBuffer, M_SEQUENCE + M_FRAMES_PER_TRIGGER(nGrab), M_ASYNCHRONOUS + M_TRIGGER_FOR_FIRST_GRAB, CDeviceMatroxIntellicam::CallbackFunction, this);

		if(IsError())
		{
			strMessage.Format(_T("Failed to write to device"));
			eReturn = EDeviceGrabResult_WriteToDeviceError;
			break;
		}

		m_bIsGrabAvailable = false;

		eReturn = EDeviceGrabResult_OK;

		strMessage.Format(_T("Succeeded to execute command 'Grab'."));
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceLiveResult CDeviceMatroxIntellicam::Live()
{
	return EDeviceLiveResult_DoNotSupport;
}

EDeviceStopResult CDeviceMatroxIntellicam::Stop()
{
	EDeviceStopResult eReturn = EDeviceStopResult_UnknownError;

	CString strMessage;

	m_bIsGrabAvailable = true;

	do
	{
		if(!IsInitialized())
		{
			strMessage.Format(_T("Failed to initialize the device"));
			eReturn = EDeviceStopResult_NotInitializedError;
			break;
		}

		int nGrabBuffer = 0;

		if(GetMILBufferCount(&nGrabBuffer))
		{
			strMessage.Format(_T("Couldn't read 'GetMILBufferCount' from the database"));
			eReturn = EDeviceStopResult_ReadOnDatabaseError;
			break;
		}

		MdigProcess(m_hMilDigitizer, m_pMilImageList, nGrabBuffer, M_STOP, M_DEFAULT, CDeviceMatroxIntellicam::CallbackFunction, this);

		if(IsError())
		{
			strMessage.Format(_T("Failed to write 'Stop process' to the device"));
			eReturn = EDeviceStopResult_WriteToDeviceError;
			break;
		}

		strMessage.Format(_T("Succeeded to execute command 'Stop'"));

		eReturn = EDeviceStopResult_OK;
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceTriggerResult CDeviceMatroxIntellicam::Trigger()
{
	EDeviceTriggerResult eReturn = EDeviceTriggerResult_UnknownError;

	CString strMessage;

	do
	{
		if(!IsInitialized())
		{
			strMessage.Format(_T("Failed to initialize the device."));
			eReturn = EDeviceTriggerResult_NotInitializedError;
			break;
		}


		MdigControl(m_hMilDigitizer, M_GRAB_TRIGGER_SOFTWARE, 1);

		if(IsError())
		{
			strMessage.Format(_T("Failed to write 'Trigger software' to the device"));
			eReturn = EDeviceTriggerResult_WriteToDeviceError;
			break;
		}
		strMessage.Format(_T("Succeeded to execute command 'Trigger'"));

		eReturn = EDeviceTriggerResult_OK;
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetCameraName(CString * pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = GetParamValue(EDeviceParameterMatrox_CameraName);

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetCameraName(CString strParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_CameraName;

	CString strPreValue = GetParamValue(eSaveID);

	do
	{
		CString strSave;
		strSave.Format(_T("%s"), strParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], strPreValue, strParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetInitilizeType(EDeviceMatrox_InitilizeType * pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_InitilizeType)_ttoi(GetParamValue(EDeviceParameterMatrox_InitializeType));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetInitilizeType(EDeviceMatrox_InitilizeType eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_InitializeType;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceMatrox_InitilizeType_Count)
		{
			eReturn = EMatroxSetFunction_NotFindCommandError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_InitializeType[nPreValue], g_lpszMatrox_InitializeType[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetCamfilePath(CString * pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}
		
		*pParam = GetParamValue(EDeviceParameterMatrox_Camfilepath);

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetCamfilePath(CString strParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Camfilepath;

	CString strPreValue = GetParamValue(eSaveID);

	do
	{
		if(!SetParamValue(eSaveID, strParam))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], strPreValue, strParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetParameterMatch(bool * pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMatrox_Parametermatch));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetParameterMatch(bool bParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Parametermatch;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		CString strSave;
		strSave.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_Switch[nPreValue], g_lpszMatrox_Switch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetGrabCount(int * pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMatrox_GrabCount));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetGrabCount(int nParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_GrabCount;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsLive() || !IsGrabAvailable())
		{
			eReturn = EMatroxSetFunction_ActivatedDeviceError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%d' to '%d'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetMILImageCount(int * pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMatrox_MILImageCount));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetMILImageCount(int nParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_MILImageCount;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsLive() || !IsGrabAvailable())
		{
			eReturn = EMatroxSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}
		
		int nGrabcount = 0;
		if(GetMILBufferCount(&nGrabcount))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(nParam < 0 || nGrabcount < 0 || nGrabcount > nParam)
		{
			eReturn = EMatroxSetFunction_RangeOverError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%d' to '%d'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetMILBufferCount(int * pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMatrox_MILBufferCount));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetMILBufferCount(int nParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_MILBufferCount;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		int nGrabcount = 0;
		if(GetMILImageCount(&nGrabcount))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(nParam < 0 || nGrabcount < 0 || nGrabcount < nParam)
		{
			eReturn = EMatroxSetFunction_RangeOverError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%d' to '%d'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetGrabDirectionX(EDeviceMatrox_DirectionSwitch * pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_DirectionSwitch)_ttoi(GetParamValue(EDeviceParameterMatrox_GrabDirectionX));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetGrabDirectionX(EDeviceMatrox_DirectionSwitch eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_GrabDirectionX;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceMatrox_DirectionSwitch_Count)
		{
			eReturn = EMatroxSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			MappControl(M_GLOBAL, M_CLEAR_ERROR);

			if(eParam != EDeviceMatrox_DirectionSwitch_Default)
				MdigControl(m_hMilDigitizer, M_GRAB_DIRECTION_X, (MIL_INT)eParam);
			else
				MdigControl(m_hMilDigitizer, M_GRAB_DIRECTION_X, M_DEFAULT);
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_DirectionSwitch[nPreValue], g_lpszMatrox_DirectionSwitch[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetGrabDirectionY(EDeviceMatrox_DirectionSwitch * pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_DirectionSwitch)_ttoi(GetParamValue(EDeviceParameterMatrox_GrabDirectionY));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetGrabDirectionY(EDeviceMatrox_DirectionSwitch eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_GrabDirectionY;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceMatrox_DirectionSwitch_Count)
		{
			eReturn = EMatroxSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(eParam != EDeviceMatrox_DirectionSwitch_Default)
				MdigControl(m_hMilDigitizer, M_GRAB_DIRECTION_Y, (MIL_INT)eParam);
			else
				MdigControl(m_hMilDigitizer, M_GRAB_DIRECTION_Y, M_DEFAULT);
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_DirectionSwitch[nPreValue], g_lpszMatrox_DirectionSwitch[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetGrabScale(double * pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterMatrox_GrabScale));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetGrabScale(double dblParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_GrabScale;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(dblParam < 0. || dblParam > 1.)
		{
			eReturn = EMatroxSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			MdigControl(m_hMilDigitizer, M_GRAB_SCALE, (MIL_DOUBLE)dblParam);
		}

		CString strSave;
		strSave.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%f' to '%f'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetGrabScaleX(double * pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterMatrox_GrabScaleX));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetGrabScaleX(double dblParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_GrabScaleX;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(dblParam < 0. || dblParam > 1.)
		{
			eReturn = EMatroxSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			MdigControl(m_hMilDigitizer, M_GRAB_SCALE_X, (MIL_DOUBLE)dblParam);
		}

		CString strSave;
		strSave.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%f' to '%f'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetGrabScaleY(double * pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterMatrox_GrabScaleY));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetGrabScaleY(double dblParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_GrabScaleY;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(dblParam < 0. || dblParam > 1.)
		{
			eReturn = EMatroxSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			MdigControl(m_hMilDigitizer, M_GRAB_SCALE_Y, (MIL_DOUBLE)dblParam);
		}

		CString strSave;
		strSave.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%f' to '%f'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetSourceOffsetX(int * pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMatrox_SourceOffsetX));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetSourceOffsetX(int nParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_SourceOffsetX;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			MdigControl(m_hMilDigitizer, M_SOURCE_OFFSET_X, (MIL_INT)nParam);
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%d' to '%d'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetSourceOffsetY(int * pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMatrox_SourceOffsetY));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetSourceOffsetY(int nParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_SourceOffsetY;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			MdigControl(m_hMilDigitizer, M_SOURCE_OFFSET_Y, (MIL_INT)nParam);
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%d' to '%d'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetSourceSizeX(int * pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMatrox_SourceSizeX));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetSourceSizeX(int nParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_SourceSizeX;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			MdigControl(m_hMilDigitizer, M_SOURCE_SIZE_X, (MIL_INT)nParam);
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%d' to '%d'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetSourceSizeY(int * pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMatrox_SourceSizeY));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetSourceSizeY(int nParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_SourceSizeY;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			MdigControl(m_hMilDigitizer, M_SOURCE_SIZE_Y, (MIL_INT)nParam);
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%d' to '%d'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetbTriggerContinuouesEndTrigger(EDeviceMatrox_ActiveSwitch * pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_ActiveSwitch)_ttoi(GetParamValue(EDeviceParameterMatrox_GrabTriggerContinuouesEndTrigger));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetbTriggerContinuouesEndTrigger(EDeviceMatrox_ActiveSwitch eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_GrabTriggerContinuouesEndTrigger;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(eParam != EDeviceMatrox_ActiveSwitch_Default)
				MdigControl(m_hMilDigitizer, M_GRAB_CONTINUOUS_END_TRIGGER, (MIL_INT)eParam);
			else
				MdigControl(m_hMilDigitizer, M_GRAB_CONTINUOUS_END_TRIGGER, M_DEFAULT);
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_ActiveSwitch[nPreValue], g_lpszMatrox_ActiveSwitch[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetGrabTriggerActivation(EDeviceMatrox_ActivationSelect * pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_ActivationSelect)_ttoi(GetParamValue(EDeviceParameterMatrox_GrabTriggerActivation));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetGrabTriggerActivation(EDeviceMatrox_ActivationSelect eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_GrabTriggerActivation;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(eParam != EDeviceMatrox_ActivationSelect_Default)
				MdigControl(m_hMilDigitizer, M_GRAB_TRIGGER_ACTIVATION, (MIL_INT)(eParam + 11L));
			else
				MdigControl(m_hMilDigitizer, M_GRAB_TRIGGER_ACTIVATION, M_DEFAULT);
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_ActivationSelect[nPreValue], g_lpszMatrox_ActivationSelect[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetGrabTriggerSoursce(EDeviceMatrox_TriggerSourceSelect * pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_TriggerSourceSelect)_ttoi(GetParamValue(EDeviceParameterMatrox_GrabTriggerSoursce));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetGrabTriggerSoursce(EDeviceMatrox_TriggerSourceSelect eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_GrabTriggerSoursce;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			switch(eParam)
			{
			case EDeviceMatrox_TriggerSourceSelect_AUXIO0:
			case EDeviceMatrox_TriggerSourceSelect_AUXIO1:
			case EDeviceMatrox_TriggerSourceSelect_AUXIO2:
			case EDeviceMatrox_TriggerSourceSelect_AUXIO3:
			case EDeviceMatrox_TriggerSourceSelect_AUXIO4:
			case EDeviceMatrox_TriggerSourceSelect_AUXIO5:
			case EDeviceMatrox_TriggerSourceSelect_AUXIO6:
			case EDeviceMatrox_TriggerSourceSelect_AUXIO7:
			case EDeviceMatrox_TriggerSourceSelect_AUXIO8:
			case EDeviceMatrox_TriggerSourceSelect_AUXIO9:
			case EDeviceMatrox_TriggerSourceSelect_AUXIO10:
			case EDeviceMatrox_TriggerSourceSelect_AUXIO11:
				{
					MdigControl(m_hMilDigitizer, M_GRAB_TRIGGER_SOURCE, (MIL_INT)(eParam + 0x500));
				}
				break;
			case EDeviceMatrox_TriggerSourceSelect_CCIO1:
			case EDeviceMatrox_TriggerSourceSelect_CCIO2:
			case EDeviceMatrox_TriggerSourceSelect_CCIO3:
			case EDeviceMatrox_TriggerSourceSelect_CCIO4:
				{
					MdigControl(m_hMilDigitizer, M_GRAB_TRIGGER_SOURCE, (MIL_INT)(eParam + 0x100));
				}
				break;
			case EDeviceMatrox_TriggerSourceSelect_Default:
				{
					MdigControl(m_hMilDigitizer, M_GRAB_TRIGGER_SOURCE, M_DEFAULT);
				}
				break;
			case EDeviceMatrox_TriggerSourceSelect_Hsync:
				{
					MdigControl(m_hMilDigitizer, M_GRAB_TRIGGER_SOURCE, M_HSYNC);
				}
				break;
			case EDeviceMatrox_TriggerSourceSelect_RotaryEncoder:
				{
					MdigControl(m_hMilDigitizer, M_GRAB_TRIGGER_SOURCE, M_ROTARY_ENCODER);
				}
				break;
			case EDeviceMatrox_TriggerSourceSelect_Software:
				{
					MdigControl(m_hMilDigitizer, M_GRAB_TRIGGER_SOURCE, M_SOFTWARE);
				}
			case EDeviceMatrox_TriggerSourceSelect_Timer0:
			case EDeviceMatrox_TriggerSourceSelect_Timer1:
			case EDeviceMatrox_TriggerSourceSelect_Timer2:
			case EDeviceMatrox_TriggerSourceSelect_Timer3:
			case EDeviceMatrox_TriggerSourceSelect_Timer4:
			case EDeviceMatrox_TriggerSourceSelect_Timer5:
			case EDeviceMatrox_TriggerSourceSelect_Timer6:
			case EDeviceMatrox_TriggerSourceSelect_Timer7:
			case EDeviceMatrox_TriggerSourceSelect_Timer8:
			case EDeviceMatrox_TriggerSourceSelect_Timer9:
			case EDeviceMatrox_TriggerSourceSelect_Timer10:
			case EDeviceMatrox_TriggerSourceSelect_Timer11:
			case EDeviceMatrox_TriggerSourceSelect_Timer12:
			case EDeviceMatrox_TriggerSourceSelect_Timer13:
			case EDeviceMatrox_TriggerSourceSelect_Timer14:
			case EDeviceMatrox_TriggerSourceSelect_Timer15:
			case EDeviceMatrox_TriggerSourceSelect_Timer16:
				{
					MdigControl(m_hMilDigitizer, M_GRAB_TRIGGER_SOURCE, (MIL_INT)(eParam - EDeviceMatrox_TriggerSourceSelect_Timer0));
				}
				break;
			case EDeviceMatrox_TriggerSourceSelect_Vsync:
				{
					MdigControl(m_hMilDigitizer, M_GRAB_TRIGGER_SOURCE, M_VSYNC);
				}
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_TriggerSourceSelect[nPreValue], g_lpszMatrox_TriggerSourceSelect[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetGrabTriggerState(EDeviceMatrox_ActiveSwitch * pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_ActiveSwitch)_ttoi(GetParamValue(EDeviceParameterMatrox_GrabTriggerState));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetGrabTriggerState(EDeviceMatrox_ActiveSwitch eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_GrabTriggerState;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(eParam != EDeviceMatrox_ActiveSwitch_Default)
				MdigControl(m_hMilDigitizer, M_GRAB_TRIGGER_STATE, (MIL_INT)(eParam + 11L));
			else
				MdigControl(m_hMilDigitizer, M_GRAB_TRIGGER_STATE, M_DEFAULT);
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_ActiveSwitch[nPreValue], g_lpszMatrox_ActiveSwitch[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}
// 
// EMatroxGetFunction CDeviceMatroxIntellicam::GetTimerSelector(EDeviceMatrox_TimerSelect * pParam)
// {
// 	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;
// 
// 	do
// 	{
// 		if(!pParam)
// 		{
// 			eReturn = EMatroxGetFunction_NullptrError;
// 			break;
// 		}
// 
// 		*pParam = (EDeviceMatrox_TimerSelect)_ttoi(GetParamValue(EDeviceParameterMatrox_TimerSelector));
// 
// 		eReturn = EMatroxGetFunction_OK;
// 	}
// 	while(false);
// 
// 	return eReturn;
// }
// 
// EMatroxSetFunction CDeviceMatroxIntellicam::SetTimerSelector(EDeviceMatrox_TimerSelect eParam)
// {
// 	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;
// 
// 	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_TimerSelector;
// 
// 	int nPreValue = _ttoi(GetParamValue(eSaveID));
// 
// 	do
// 	{
// 		CString strSave;
// 		strSave.Format(_T("%d"), eParam);
// 
// 		if(!SetParamValue(eSaveID, strSave))
// 		{
// 			eReturn = EMatroxSetFunction_WriteToDatabaseError;
// 			break;
// 		}
// 
// 		if(!SaveSettings(eSaveID))
// 		{
// 			eReturn = EMatroxSetFunction_WriteToDatabaseError;
// 			break;
// 		}
// 
// 		eReturn = EMatroxSetFunction_OK;
// 	}
// 	while(false);
// 
// 	CString strMessage;
// 
// 	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_TimerSelect[nPreValue], g_lpszMatrox_TimerSelect[eParam]);
// 
// 	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);
// 
// 	SetMessage(strMessage);
// 
// 	return eReturn;
// }


EMatroxGetFunction CDeviceMatroxIntellicam::GetCameraType(EDeviceMatrox_CameraType* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_CameraType)_ttoi(GetParamValue(EDeviceParameterMatrox_CameraType));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetCameraType(EDeviceMatrox_CameraType eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_CameraType;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceMatrox_CameraType_Count)
		{
			eReturn = EMatroxSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_CameraType[nPreValue], g_lpszMatrox_CameraType[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetCameraNumberOfTaps(EDeviceMatrox_NumberOfTaps* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_NumberOfTaps)_ttoi(GetParamValue(EDeviceParameterMatrox_CameraNumberOfTaps));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetCameraNumberOfTaps(EDeviceMatrox_NumberOfTaps eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_CameraNumberOfTaps;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_Numberoftaps[nPreValue], g_lpszMatrox_Numberoftaps[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}


EMatroxGetFunction CDeviceMatroxIntellicam::GetCameraBayerMode(EDeviceMatrox_BayerMode* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_BayerMode)_ttoi(GetParamValue(EDeviceParameterMatrox_CameraBayerMode));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetCameraBayerMode(EDeviceMatrox_BayerMode eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_CameraBayerMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceMatrox_BayerMode_Count)
		{
			eReturn = EMatroxSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(eParam)
			{
				MdigControl(m_hMilDigitizer, M_BAYER_CONVERSION, M_ENABLE);

				if(IsError())
				{
					eReturn = EMatroxSetFunction_WriteToDeviceError;
					break;
				}
			}
			else
			{
				MdigControl(m_hMilDigitizer, M_BAYER_CONVERSION, M_DISABLE);

				if(IsError())
				{
					eReturn = EMatroxSetFunction_WriteToDeviceError;
					break;
				}
			}

			MdigControl(m_hMilDigitizer, M_BAYER_PATTERN, eParam << 5);

			if(IsError())
			{
				eReturn = EMatroxSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_Bayermode[nPreValue], g_lpszMatrox_Bayermode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetCameraLinkConfiguration(EDeviceMatrox_ConfigurationType*pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_ConfigurationType)_ttoi(GetParamValue(EDeviceParameterMatrox_ConfigurationType));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetCameraLinkConfiguration(EDeviceMatrox_ConfigurationType eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_ConfigurationType;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_ConfigurationType[nPreValue], g_lpszMatrox_ConfigurationType[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}


EMatroxGetFunction CDeviceMatroxIntellicam::GetCameraMode(EDeviceMatrox_CameraTapsMode* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_CameraTapsMode)_ttoi(GetParamValue(EDeviceParameterMatrox_CameraMode));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetCameraMode(EDeviceMatrox_CameraTapsMode eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_CameraMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_CameraMode[nPreValue], g_lpszMatrox_CameraMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}


EMatroxGetFunction CDeviceMatroxIntellicam::GetRegionsX(EDeviceMatrox_TapsSelector* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_TapsSelector)_ttoi(GetParamValue(EDeviceParameterMatrox_RegionsX));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetRegionsX(EDeviceMatrox_TapsSelector eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_RegionsX;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_TapsSelector[nPreValue], g_lpszMatrox_TapsSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetRegionsY(EDeviceMatrox_TapsSelector* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_TapsSelector)_ttoi(GetParamValue(EDeviceParameterMatrox_RegionsY));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetRegionsY(EDeviceMatrox_TapsSelector eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_RegionsY;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_TapsSelector[nPreValue], g_lpszMatrox_TapsSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetAdjacentPixelsX(EDeviceMatrox_TapsSelector* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_TapsSelector)_ttoi(GetParamValue(EDeviceParameterMatrox_AdjacentX));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetAdjacentPixelsX(EDeviceMatrox_TapsSelector eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_AdjacentX;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_TapsSelector[nPreValue], g_lpszMatrox_TapsSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetAdjacentPixelsY(EDeviceMatrox_TapsSelector* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_TapsSelector)_ttoi(GetParamValue(EDeviceParameterMatrox_AdjacentY));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetAdjacentPixelsY(EDeviceMatrox_TapsSelector eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_AdjacentY;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_TapsSelector[nPreValue], g_lpszMatrox_TapsSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTaps1Direction(_Out_ EDeviceMatrox_TapsDirection* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_TapsDirection)_ttoi(GetParamValue(EDeviceParameterMatrox_Tap1));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}
EMatroxSetFunction CDeviceMatroxIntellicam::SetTaps1Direction(_In_ EDeviceMatrox_TapsDirection eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Tap1;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_TapsDirection[nPreValue], g_lpszMatrox_TapsDirection[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTaps2Direction(_Out_ EDeviceMatrox_TapsDirection* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_TapsDirection)_ttoi(GetParamValue(EDeviceParameterMatrox_Tap2));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}
EMatroxSetFunction CDeviceMatroxIntellicam::SetTaps2Direction(_In_ EDeviceMatrox_TapsDirection eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Tap2;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_TapsDirection[nPreValue], g_lpszMatrox_TapsDirection[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTaps3Direction(_Out_ EDeviceMatrox_TapsDirection* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_TapsDirection)_ttoi(GetParamValue(EDeviceParameterMatrox_Tap3));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}
EMatroxSetFunction CDeviceMatroxIntellicam::SetTaps3Direction(_In_ EDeviceMatrox_TapsDirection eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Tap3;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_TapsDirection[nPreValue], g_lpszMatrox_TapsDirection[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTaps4Direction(_Out_ EDeviceMatrox_TapsDirection* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_TapsDirection)_ttoi(GetParamValue(EDeviceParameterMatrox_Tap4));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}
EMatroxSetFunction CDeviceMatroxIntellicam::SetTaps4Direction(_In_ EDeviceMatrox_TapsDirection eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Tap4;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_TapsDirection[nPreValue], g_lpszMatrox_TapsDirection[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTaps5Direction(_Out_ EDeviceMatrox_TapsDirection* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_TapsDirection)_ttoi(GetParamValue(EDeviceParameterMatrox_Tap5));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}
EMatroxSetFunction CDeviceMatroxIntellicam::SetTaps5Direction(_In_ EDeviceMatrox_TapsDirection eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Tap5;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_TapsDirection[nPreValue], g_lpszMatrox_TapsDirection[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTaps6Direction(_Out_ EDeviceMatrox_TapsDirection* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_TapsDirection)_ttoi(GetParamValue(EDeviceParameterMatrox_Tap6));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}
EMatroxSetFunction CDeviceMatroxIntellicam::SetTaps6Direction(_In_ EDeviceMatrox_TapsDirection eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Tap6;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_TapsDirection[nPreValue], g_lpszMatrox_TapsDirection[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTaps7Direction(_Out_ EDeviceMatrox_TapsDirection* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_TapsDirection)_ttoi(GetParamValue(EDeviceParameterMatrox_Tap7));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}
EMatroxSetFunction CDeviceMatroxIntellicam::SetTaps7Direction(_In_ EDeviceMatrox_TapsDirection eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Tap7;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_TapsDirection[nPreValue], g_lpszMatrox_TapsDirection[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTaps8Direction(_Out_ EDeviceMatrox_TapsDirection* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_TapsDirection)_ttoi(GetParamValue(EDeviceParameterMatrox_Tap8));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}
EMatroxSetFunction CDeviceMatroxIntellicam::SetTaps8Direction(_In_ EDeviceMatrox_TapsDirection eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Tap8;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_TapsDirection[nPreValue], g_lpszMatrox_TapsDirection[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetVideoSignalType(EDeviceMatrox_VideoSignalType* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_VideoSignalType)_ttoi(GetParamValue(EDeviceParameterMatrox_VideoSignalType));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetVideoSignalType(EDeviceMatrox_VideoSignalType eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_VideoSignalType;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_VideoSignalType[nPreValue], g_lpszMatrox_VideoSignalType[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetDataBusWidth(EDeviceMatrox_DataBusWidth* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_DataBusWidth)_ttoi(GetParamValue(EDeviceParameterMatrox_VideoSignalDatabusWidth));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetDataBusWidth(EDeviceMatrox_DataBusWidth eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_VideoSignalDatabusWidth;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_Databuswidth[nPreValue], g_lpszMatrox_Databuswidth[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetVideoSignalStandard(EDeviceMatrox_Standard* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_Standard)_ttoi(GetParamValue(EDeviceParameterMatrox_VideoSignalStandard));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetVideoSignalStandard(EDeviceMatrox_Standard eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_VideoSignalStandard;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_Standard[nPreValue], g_lpszMatrox_Standard[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetMILChannel(EDeviceMatrox_MILChannel* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_MILChannel)_ttoi(GetParamValue(EDeviceParameterMatrox_VideoSignalMILChannelInput));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetMILChannel(EDeviceMatrox_MILChannel eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_VideoSignalMILChannelInput;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_MILChannel[nPreValue], g_lpszMatrox_MILChannel[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetVideoSignalFormat(EDeviceMatrox_InoutFormat* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_InoutFormat)_ttoi(GetParamValue(EDeviceParameterMatrox_DigitalVideoSignalFormat));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetVideoSignalFormat(EDeviceMatrox_InoutFormat eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_DigitalVideoSignalFormat;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_InoutFormat[nPreValue], g_lpszMatrox_InoutFormat[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetVideoSignalSource(EDeviceMatrox_ValidSignalSource* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_ValidSignalSource)_ttoi(GetParamValue(EDeviceParameterMatrox_ValidSignalSource));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetVideoSignalSource(EDeviceMatrox_ValidSignalSource eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_ValidSignalSource;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_ValidSignalSource[nPreValue], g_lpszMatrox_ValidSignalSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTimingsVertical(EDeviceMatrox_InterlacedSelector* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_InterlacedSelector)_ttoi(GetParamValue(EDeviceParameterMatrox_TimingsVertical));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetTimingsVertical(EDeviceMatrox_InterlacedSelector eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_TimingsVertical;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_ValidSignalSource eSignalSrc = EDeviceMatrox_ValidSignalSource_Count;

		if(GetVideoSignalSource(&eSignalSrc))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eSignalSrc != EDeviceMatrox_ValidSignalSource_UseFrameGrabber)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_InterlacedSelector[nPreValue], g_lpszMatrox_InterlacedSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetVideoTimingAutoAdjust(bool* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterMatrox_PixelClockFrqAutoAdjust));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetVideoTimingAutoAdjust(bool bParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_PixelClockFrqAutoAdjust;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_ValidSignalSource eSignalSrc = EDeviceMatrox_ValidSignalSource_Count;

		if(GetVideoSignalSource(&eSignalSrc))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eSignalSrc != EDeviceMatrox_ValidSignalSource_UseFrameGrabber)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_Switch[nPreValue], g_lpszMatrox_Switch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetHorizontalSyncClk(int* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMatrox_HorizontalSyncClk));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetHorizontalSyncClk(int nParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_HorizontalSyncClk;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_ValidSignalSource eSignalSrc = EDeviceMatrox_ValidSignalSource_Count;

		if(GetVideoSignalSource(&eSignalSrc))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eSignalSrc != EDeviceMatrox_ValidSignalSource_UseFrameGrabber)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%d' to '%d'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetHorizontalBPorchClk(int* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMatrox_HorizontalBPorchClk));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetHorizontalBPorchClk(int nParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_HorizontalBPorchClk;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_ValidSignalSource eSignalSrc = EDeviceMatrox_ValidSignalSource_Count;

		if(GetVideoSignalSource(&eSignalSrc))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eSignalSrc != EDeviceMatrox_ValidSignalSource_UseFrameGrabber)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%d' to '%d'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetHorizontalActiveClk(int* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMatrox_HorizontalActiveClk));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetHorizontalActiveClk(int nParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_HorizontalActiveClk;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_ValidSignalSource eSignalSrc = EDeviceMatrox_ValidSignalSource_Count;

		if(GetVideoSignalSource(&eSignalSrc))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eSignalSrc != EDeviceMatrox_ValidSignalSource_UseFrameGrabber)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%d' to '%d'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetHorizontalFPorchClk(int* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMatrox_HorizontalFPorchClk));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetHorizontalFPorchClk(int nParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_HorizontalFPorchClk;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_ValidSignalSource eSignalSrc = EDeviceMatrox_ValidSignalSource_Count;

		if(GetVideoSignalSource(&eSignalSrc))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eSignalSrc != EDeviceMatrox_ValidSignalSource_UseFrameGrabber)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%d' to '%d'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetHorizontalFrequencyHz(double* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterMatrox_HorizontalFrequencyHz));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetHorizontalFrequencyHz(double dblParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_HorizontalFrequencyHz;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_ValidSignalSource eSignalSrc = EDeviceMatrox_ValidSignalSource_Count;

		if(GetVideoSignalSource(&eSignalSrc))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eSignalSrc != EDeviceMatrox_ValidSignalSource_UseFrameGrabber)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%f' to '%f'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetHorizontalLockActiveAndTotal(bool* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMatrox_HorizontalLockActiveAndTotal));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetHorizontalLockActiveAndTotal(bool bParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_HorizontalLockActiveAndTotal;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_ValidSignalSource eSignalSrc = EDeviceMatrox_ValidSignalSource_Count;

		if(GetVideoSignalSource(&eSignalSrc))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eSignalSrc != EDeviceMatrox_ValidSignalSource_UseFrameGrabber)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}


		CString strSave;
		strSave.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_Switch[nPreValue], g_lpszMatrox_Switch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetVerticalSyncLines(int* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMatrox_VerticalSyncLine));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetVerticalSyncLines(int nParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_VerticalSyncLine;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_ValidSignalSource eSignalSrc = EDeviceMatrox_ValidSignalSource_Count;

		if(GetVideoSignalSource(&eSignalSrc))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eSignalSrc != EDeviceMatrox_ValidSignalSource_UseFrameGrabber)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsLive() || !IsGrabAvailable())
		{
			eReturn = EMatroxSetFunction_ActivatedDeviceError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%d' to '%d'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetVerticalBPorchLines(int* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMatrox_VerticalBPorchLine));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetVerticalBPorchLines(int nParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_VerticalBPorchLine;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_ValidSignalSource eSignalSrc = EDeviceMatrox_ValidSignalSource_Count;

		if(GetVideoSignalSource(&eSignalSrc))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eSignalSrc != EDeviceMatrox_ValidSignalSource_UseFrameGrabber)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsLive() || !IsGrabAvailable())
		{
			eReturn = EMatroxSetFunction_ActivatedDeviceError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%d' to '%d'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetVerticalActiveLines(int* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMatrox_VerticalActiveLine));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetVerticalActiveLines(int nParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_VerticalActiveLine;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_ValidSignalSource eSignalSrc = EDeviceMatrox_ValidSignalSource_Count;

		if(GetVideoSignalSource(&eSignalSrc))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eSignalSrc != EDeviceMatrox_ValidSignalSource_UseFrameGrabber)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsLive() || !IsGrabAvailable())
		{
			eReturn = EMatroxSetFunction_ActivatedDeviceError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%d' to '%d'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetVerticalFPorchLines(int* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMatrox_VerticalFPorchLine));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetVerticalFPorchLines(int nParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_VerticalFPorchLine;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_ValidSignalSource eSignalSrc = EDeviceMatrox_ValidSignalSource_Count;

		if(GetVideoSignalSource(&eSignalSrc))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eSignalSrc != EDeviceMatrox_ValidSignalSource_UseFrameGrabber)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsLive() || !IsGrabAvailable())
		{
			eReturn = EMatroxSetFunction_ActivatedDeviceError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%d' to '%d'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetVerticalFrequencyHz(double* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterMatrox_VerticalFrequencyHz));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetVerticalFrequencyHz(double dblParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_VerticalFrequencyHz;

	double dblPreValue = _ttof(GetParamValue(eSaveID));
	
	do
	{
		EDeviceMatrox_ValidSignalSource eSignalSrc = EDeviceMatrox_ValidSignalSource_Count;

		if(GetVideoSignalSource(&eSignalSrc))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eSignalSrc != EDeviceMatrox_ValidSignalSource_UseFrameGrabber)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%f' to '%f'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetVerticalLockActiveAndTotal(bool* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMatrox_VerticalLockActiveAndTotal));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetVerticalLockActiveAndTotal(bool bParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_VerticalLockActiveAndTotal;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_ValidSignalSource eSignalSrc = EDeviceMatrox_ValidSignalSource_Count;

		if(GetVideoSignalSource(&eSignalSrc))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eSignalSrc != EDeviceMatrox_ValidSignalSource_UseFrameGrabber)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}


		CString strSave;
		strSave.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_Switch[nPreValue], g_lpszMatrox_Switch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetVideoTimingImagesizeX(int* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMatrox_ImagesizeX));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetVideoTimingImagesizeX(int nParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_ImagesizeX;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_ValidSignalSource eSignalSrc = EDeviceMatrox_ValidSignalSource_Count;

		if(GetVideoSignalSource(&eSignalSrc))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eSignalSrc != EDeviceMatrox_ValidSignalSource_UseCamera)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%d' to '%d'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetVideoTimingImagesizeY(int* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMatrox_ImagesizeY));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetVideoTimingImagesizeY(int nParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_ImagesizeY;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_ValidSignalSource eSignalSrc = EDeviceMatrox_ValidSignalSource_Count;

		if(GetVideoSignalSource(&eSignalSrc))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eSignalSrc != EDeviceMatrox_ValidSignalSource_UseCamera)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%d' to '%d'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetVideoTimingDelayX(int* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMatrox_DelayX));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetVideoTimingDelayX(int nParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_DelayX;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_ValidSignalSource eSignalSrc = EDeviceMatrox_ValidSignalSource_Count;

		if(GetVideoSignalSource(&eSignalSrc))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eSignalSrc != EDeviceMatrox_ValidSignalSource_UseCamera)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%d' to '%d'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetVideoTimingDelayY(int* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMatrox_DelayY));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetVideoTimingDelayY(int nParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_DelayY;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_ValidSignalSource eSignalSrc = EDeviceMatrox_ValidSignalSource_Count;

		if(GetVideoSignalSource(&eSignalSrc))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eSignalSrc != EDeviceMatrox_ValidSignalSource_UseCamera)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%d' to '%d'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetPixelClockFrequency(double* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterMatrox_PixelClockFrequencyMHz));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetPixelClockFrequency(double dblParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_PixelClockFrequencyMHz;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%f' to '%f'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetAutoAdjustInTheVideoTimingsPage(EDeviceMatrox_AdjustInTheVideoTimingsPage* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_AdjustInTheVideoTimingsPage)_ttoi(GetParamValue(EDeviceParameterMatrox_AutoAdjustVideoTimingsPage));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetAutoAdjustInTheVideoTimingsPage(EDeviceMatrox_AdjustInTheVideoTimingsPage eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_AutoAdjustVideoTimingsPage;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_AdjustIntheVideoTimingsPage[nPreValue], g_lpszMatrox_AdjustIntheVideoTimingsPage[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetSendToExternalCircuitOtherThanCamera(bool* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMatrox_SendToExternalCircuit));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetSendToExternalCircuitOtherThanCamera(bool bParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_SendToExternalCircuit;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_Switch[nPreValue], g_lpszMatrox_Switch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetExternalClockSignal(EDeviceMatrox_ExternalClockSignal* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_ExternalClockSignal)_ttoi(GetParamValue(EDeviceParameterMatrox_ExternalClockSignal));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetExternalClockSignal(EDeviceMatrox_ExternalClockSignal eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_ExternalClockSignal;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_ExternalClockSignal[nPreValue], g_lpszMatrox_ExternalClockSignal[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetPixelClockInputFrequency(EDeviceMatrox_PixelClockFrequency* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_PixelClockFrequency)_ttoi(GetParamValue(EDeviceParameterMatrox_EXTInputFrequency));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetPixelClockInputFrequency(EDeviceMatrox_PixelClockFrequency eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_EXTInputFrequency;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_ExternalClockSignal eRxtClkSignal = EDeviceMatrox_ExternalClockSignal_Count;

		if(GetExternalClockSignal(&eRxtClkSignal))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eRxtClkSignal != EDeviceMatrox_ExternalClockSignal_GeneratedByCamera)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_PixelClockFrequency[nPreValue], g_lpszMatrox_PixelClockFrequency[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetPixelClockInputFormat(EDeviceMatrox_InoutFormat* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_InoutFormat)_ttoi(GetParamValue(EDeviceParameterMatrox_EXTInputFormat));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetPixelClockInputFormat(EDeviceMatrox_InoutFormat eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_EXTInputFormat;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_ExternalClockSignal eRxtClkSignal = EDeviceMatrox_ExternalClockSignal_Count;

		if(GetExternalClockSignal(&eRxtClkSignal))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eRxtClkSignal != EDeviceMatrox_ExternalClockSignal_GeneratedByCamera)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_InoutFormat[nPreValue], g_lpszMatrox_InoutFormat[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetPixelClockInputPolarity(EDeviceMatrox_InoutPolarity* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_InoutPolarity)_ttoi(GetParamValue(EDeviceParameterMatrox_EXTInputPolarity));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetPixelClockInputPolarity(EDeviceMatrox_InoutPolarity eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_EXTInputPolarity;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_ExternalClockSignal eRxtClkSignal = EDeviceMatrox_ExternalClockSignal_Count;

		if(GetExternalClockSignal(&eRxtClkSignal))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eRxtClkSignal != EDeviceMatrox_ExternalClockSignal_GeneratedByCamera)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_InoutPolarity[nPreValue], g_lpszMatrox_InoutPolarity[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetPixelClockOutputFrequency(EDeviceMatrox_PixelClockFrequency* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_PixelClockFrequency)_ttoi(GetParamValue(EDeviceParameterMatrox_EXTOutputFrequency));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetPixelClockOutputFrequency(EDeviceMatrox_PixelClockFrequency eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_EXTOutputFrequency;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_ExternalClockSignal eRxtClkSignal = EDeviceMatrox_ExternalClockSignal_Count;

		if(GetExternalClockSignal(&eRxtClkSignal))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eRxtClkSignal == EDeviceMatrox_ExternalClockSignal_GeneratedByCamera || eRxtClkSignal == EDeviceMatrox_ExternalClockSignal_NoClockExchange)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_PixelClockFrequency[nPreValue], g_lpszMatrox_PixelClockFrequency[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetPixelClockOutputFormat(EDeviceMatrox_InoutFormat* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_InoutFormat)_ttoi(GetParamValue(EDeviceParameterMatrox_EXTOutputFormat));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetPixelClockOutputFormat(EDeviceMatrox_InoutFormat eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_EXTOutputFormat;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_ExternalClockSignal eRxtClkSignal = EDeviceMatrox_ExternalClockSignal_Count;

		if(GetExternalClockSignal(&eRxtClkSignal))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eRxtClkSignal == EDeviceMatrox_ExternalClockSignal_GeneratedByCamera || eRxtClkSignal == EDeviceMatrox_ExternalClockSignal_NoClockExchange)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_InoutFormat[nPreValue], g_lpszMatrox_InoutFormat[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetPixelClockOutputPolarity(EDeviceMatrox_InoutPolarity* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_InoutPolarity)_ttoi(GetParamValue(EDeviceParameterMatrox_EXTOutputPolarity));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetPixelClockOutputPolarity(EDeviceMatrox_InoutPolarity eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_EXTOutputPolarity;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_ExternalClockSignal eRxtClkSignal = EDeviceMatrox_ExternalClockSignal_Count;

		if(GetExternalClockSignal(&eRxtClkSignal))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eRxtClkSignal == EDeviceMatrox_ExternalClockSignal_GeneratedByCamera || eRxtClkSignal == EDeviceMatrox_ExternalClockSignal_NoClockExchange)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_InoutPolarity[nPreValue], g_lpszMatrox_InoutPolarity[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetPixelClockDelay(int* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMatrox_EXTIODelay));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetPixelClockDelay(int nParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_EXTIODelay;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_ExternalClockSignal eRxtClkSignal = EDeviceMatrox_ExternalClockSignal_Count;

		if(GetExternalClockSignal(&eRxtClkSignal))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eRxtClkSignal == EDeviceMatrox_ExternalClockSignal_GeneratedByCamera || eRxtClkSignal == EDeviceMatrox_ExternalClockSignal_NoClockExchange)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%d' to '%d'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTimer1Mode(EDeviceMatrox_GenerationMode* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_GenerationMode)_ttoi(GetParamValue(EDeviceParameterMatrox_Timer1Mode));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetTimer1Mode(EDeviceMatrox_GenerationMode eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Timer1Mode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_GenerationMode[nPreValue], g_lpszMatrox_GenerationMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTimer1Format(EDeviceMatrox_InoutFormatDefault* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_InoutFormatDefault)_ttoi(GetParamValue(EDeviceParameterMatrox_Timer1ExpFormat));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetTimer1Format(EDeviceMatrox_InoutFormatDefault eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Timer1ExpFormat;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_GenerationMode eTimerMode = EDeviceMatrox_GenerationMode_Count;

		if(GetTimer1Mode(&eTimerMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eTimerMode == EDeviceMatrox_GenerationMode_Disable)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_InoutFormatDefault[nPreValue], g_lpszMatrox_InoutFormatDefault[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTimer1Polarity(EDeviceMatrox_InoutPolarity* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_InoutPolarity)_ttoi(GetParamValue(EDeviceParameterMatrox_Timer1ExpPolarity));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetTimer1Polarity(EDeviceMatrox_InoutPolarity eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Timer1ExpPolarity;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_GenerationMode eTimerMode = EDeviceMatrox_GenerationMode_Count;

		if(GetTimer1Mode(&eTimerMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eTimerMode == EDeviceMatrox_GenerationMode_Disable)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_InoutPolarity[nPreValue], g_lpszMatrox_InoutPolarity[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTimer1TrgFormat(EDeviceMatrox_InoutFormatDefault* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_InoutFormatDefault)_ttoi(GetParamValue(EDeviceParameterMatrox_Timer1TrgFormat));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetTimer1TrgFormat(EDeviceMatrox_InoutFormatDefault eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Timer1TrgFormat;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_GenerationMode eTimerMode = EDeviceMatrox_GenerationMode_Count;

		if(GetTimer1Mode(&eTimerMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eTimerMode != EDeviceMatrox_GenerationMode_OnTriggerEvent)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_InoutFormatDefault[nPreValue], g_lpszMatrox_InoutFormatDefault[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTimer1TrgSignal(EDeviceMatrox_ExposureTriggerSignal* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_ExposureTriggerSignal)_ttoi(GetParamValue(EDeviceParameterMatrox_Timer1TrgSignal));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetTimer1TrgSignal(EDeviceMatrox_ExposureTriggerSignal eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Timer1TrgSignal;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_GenerationMode eTimerMode = EDeviceMatrox_GenerationMode_Count;

		if(GetTimer1Mode(&eTimerMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eTimerMode != EDeviceMatrox_GenerationMode_OnTriggerEvent)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_ExposureTriggerSignal[nPreValue], g_lpszMatrox_ExposureTriggerSignal[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTimer1TrgPolarity(EDeviceMatrox_InoutPolarity* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_InoutPolarity)_ttoi(GetParamValue(EDeviceParameterMatrox_Timer1TrgPolarity));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetTimer1TrgPolarity(EDeviceMatrox_InoutPolarity eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Timer1TrgPolarity;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_GenerationMode eTimerMode = EDeviceMatrox_GenerationMode_Count;

		if(GetTimer1Mode(&eTimerMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eTimerMode != EDeviceMatrox_GenerationMode_OnTriggerEvent)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_InoutPolarity[nPreValue], g_lpszMatrox_InoutPolarity[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTimer1Pulse1Clk(int* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMatrox_Timer1Pulse1Clk));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetTimer1Pulse1Clk(int nParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Timer1Pulse1Clk;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_GenerationMode eTimerMode = EDeviceMatrox_GenerationMode_Count;

		if(GetTimer1Mode(&eTimerMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eTimerMode == EDeviceMatrox_GenerationMode_Disable)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%d' to '%d'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTimer1Delay1Clk(int* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMatrox_Timer1Delay1Clk));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetTimer1Delay1Clk(int nParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Timer1Delay1Clk;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_GenerationMode eTimerMode = EDeviceMatrox_GenerationMode_Count;

		if(GetTimer1Mode(&eTimerMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eTimerMode == EDeviceMatrox_GenerationMode_Disable)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsLive() || !IsGrabAvailable())
		{
			eReturn = EMatroxSetFunction_ActivatedDeviceError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%d' to '%d'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTimer1Pulse2Clk(int* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMatrox_Timer1Pulse2Clk));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetTimer1Pulse2Clk(int nParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Timer1Pulse2Clk;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_GenerationMode eTimerMode = EDeviceMatrox_GenerationMode_Count;

		if(GetTimer1Mode(&eTimerMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eTimerMode == EDeviceMatrox_GenerationMode_Disable)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%d' to '%d'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTimer1Delay2Clk(int* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMatrox_Timer1Delay2Clk));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetTimer1Delay2Clk(int nParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Timer1Delay2Clk;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_GenerationMode eTimerMode = EDeviceMatrox_GenerationMode_Count;

		if(GetTimer1Mode(&eTimerMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eTimerMode == EDeviceMatrox_GenerationMode_Disable)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%d' to '%d'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTimer1ExposureClockType(EDeviceMatrox_ExposureClockType* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_ExposureClockType)_ttoi(GetParamValue(EDeviceParameterMatrox_Timer1Type));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetTimer1ExposureClockType(EDeviceMatrox_ExposureClockType eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Timer1Type;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_GenerationMode eTimerMode = EDeviceMatrox_GenerationMode_Count;

		if(GetTimer1Mode(&eTimerMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eTimerMode == EDeviceMatrox_GenerationMode_Disable)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_ExposureClockType[nPreValue], g_lpszMatrox_ExposureClockType[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTimer1ExposureClockDivisionFactor(EDeviceMatrox_DivisionFactor* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_DivisionFactor)_ttoi(GetParamValue(EDeviceParameterMatrox_Timer1DivisionFactor));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetTimer1ExposureClockDivisionFactor(EDeviceMatrox_DivisionFactor eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Timer1DivisionFactor;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_GenerationMode eTimerMode = EDeviceMatrox_GenerationMode_Count;

		if(GetTimer1Mode(&eTimerMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eTimerMode == EDeviceMatrox_GenerationMode_Disable)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_DivisionFactor[nPreValue], g_lpszMatrox_DivisionFactor[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTimer1ExposureClockBaseClockFrequency(double* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterMatrox_Timer1BaseClockFrequency));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetTimer1ExposureClockBaseClockFrequency(double dblParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Timer1BaseClockFrequency;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_GenerationMode eTimerMode = EDeviceMatrox_GenerationMode_Count;

		if(GetTimer1Mode(&eTimerMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eTimerMode == EDeviceMatrox_GenerationMode_Disable)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		EDeviceMatrox_ExposureClockType eBaseClockType = EDeviceMatrox_ExposureClockType_Count;

		if(GetTimer1ExposureClockType(&eBaseClockType))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eBaseClockType != EDeviceMatrox_ExposureClockType_ClockGenerator && eBaseClockType != EDeviceMatrox_ExposureClockType_Aux1_Input_LVDS)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%f' to '%f'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTimer2Mode(EDeviceMatrox_GenerationMode* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_GenerationMode)_ttoi(GetParamValue(EDeviceParameterMatrox_Timer2Mode));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetTimer2Mode(EDeviceMatrox_GenerationMode eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Timer2Mode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_GenerationMode[nPreValue], g_lpszMatrox_GenerationMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTimer2Format(EDeviceMatrox_InoutFormatDefault* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_InoutFormatDefault)_ttoi(GetParamValue(EDeviceParameterMatrox_Timer2ExpFormat));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetTimer2Format(EDeviceMatrox_InoutFormatDefault eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Timer2ExpFormat;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_GenerationMode eTimerMode = EDeviceMatrox_GenerationMode_Count;

		if(GetTimer2Mode(&eTimerMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eTimerMode == EDeviceMatrox_GenerationMode_Disable)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_InoutFormatDefault[nPreValue], g_lpszMatrox_InoutFormatDefault[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTimer2Polarity(EDeviceMatrox_InoutPolarity* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_InoutPolarity)_ttoi(GetParamValue(EDeviceParameterMatrox_Timer2ExpPolarity));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetTimer2Polarity(EDeviceMatrox_InoutPolarity eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Timer2ExpPolarity;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_GenerationMode eTimerMode = EDeviceMatrox_GenerationMode_Count;

		if(GetTimer2Mode(&eTimerMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eTimerMode == EDeviceMatrox_GenerationMode_Disable)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_InoutPolarity[nPreValue], g_lpszMatrox_InoutPolarity[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTimer2TrgFormat(EDeviceMatrox_InoutFormatDefault* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_InoutFormatDefault)_ttoi(GetParamValue(EDeviceParameterMatrox_Timer2TrgFormat));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetTimer2TrgFormat(EDeviceMatrox_InoutFormatDefault eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Timer2TrgFormat;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_GenerationMode eTimerMode = EDeviceMatrox_GenerationMode_Count;

		if(GetTimer2Mode(&eTimerMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eTimerMode == EDeviceMatrox_GenerationMode_Disable)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_InoutFormatDefault[nPreValue], g_lpszMatrox_InoutFormatDefault[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTimer2TrgSignal(EDeviceMatrox_ExposureTriggerSignal* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_ExposureTriggerSignal)_ttoi(GetParamValue(EDeviceParameterMatrox_Timer2TrgSignal));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetTimer2TrgSignal(EDeviceMatrox_ExposureTriggerSignal eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Timer2TrgSignal;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_GenerationMode eTimerMode = EDeviceMatrox_GenerationMode_Count;

		if(GetTimer2Mode(&eTimerMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eTimerMode == EDeviceMatrox_GenerationMode_OnTriggerEvent)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_ExposureTriggerSignal[nPreValue], g_lpszMatrox_ExposureTriggerSignal[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTimer2TrgPolarity(EDeviceMatrox_InoutPolarity* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_InoutPolarity)_ttoi(GetParamValue(EDeviceParameterMatrox_Timer2TrgPolarity));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetTimer2TrgPolarity(EDeviceMatrox_InoutPolarity eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Timer2TrgPolarity;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_GenerationMode eTimerMode = EDeviceMatrox_GenerationMode_Count;

		if(GetTimer2Mode(&eTimerMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eTimerMode == EDeviceMatrox_GenerationMode_OnTriggerEvent)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_InoutPolarity[nPreValue], g_lpszMatrox_InoutPolarity[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTimer2Pulse1Clk(int* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMatrox_Timer2Pulse1Clk));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetTimer2Pulse1Clk(int nParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Timer2Pulse1Clk;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_GenerationMode eTimerMode = EDeviceMatrox_GenerationMode_Count;

		if(GetTimer2Mode(&eTimerMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eTimerMode == EDeviceMatrox_GenerationMode_Disable)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsLive() || !IsGrabAvailable())
		{
			eReturn = EMatroxSetFunction_ActivatedDeviceError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%d' to '%d'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTimer2Delay1Clk(int* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMatrox_Timer2Delay1Clk));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetTimer2Delay1Clk(int nParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Timer2Delay1Clk;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_GenerationMode eTimerMode = EDeviceMatrox_GenerationMode_Count;

		if(GetTimer2Mode(&eTimerMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eTimerMode == EDeviceMatrox_GenerationMode_Disable)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsLive() || !IsGrabAvailable())
		{
			eReturn = EMatroxSetFunction_ActivatedDeviceError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%d' to '%d'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTimer2Pulse2Clk(int* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMatrox_Timer2Pulse2Clk));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetTimer2Pulse2Clk(int nParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Timer2Pulse2Clk;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_GenerationMode eTimerMode = EDeviceMatrox_GenerationMode_Count;

		if(GetTimer2Mode(&eTimerMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eTimerMode == EDeviceMatrox_GenerationMode_Disable)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsLive() || !IsGrabAvailable())
		{
			eReturn = EMatroxSetFunction_ActivatedDeviceError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%d' to '%d'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTimer2Delay2Clk(int* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMatrox_Timer2Delay2Clk));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetTimer2Delay2Clk(int nParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Timer2Delay2Clk;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_GenerationMode eTimerMode = EDeviceMatrox_GenerationMode_Count;

		if(GetTimer2Mode(&eTimerMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eTimerMode == EDeviceMatrox_GenerationMode_Disable)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsLive() || !IsGrabAvailable())
		{
			eReturn = EMatroxSetFunction_ActivatedDeviceError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%d' to '%d'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTimer2ExposureClockType(EDeviceMatrox_ExposureClockType* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_ExposureClockType)_ttoi(GetParamValue(EDeviceParameterMatrox_Timer2Type));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetTimer2ExposureClockType(EDeviceMatrox_ExposureClockType eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Timer2Type;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_GenerationMode eTimerMode = EDeviceMatrox_GenerationMode_Count;

		if(GetTimer2Mode(&eTimerMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eTimerMode == EDeviceMatrox_GenerationMode_Disable)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_ExposureClockType[nPreValue], g_lpszMatrox_ExposureClockType[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTimer2ExposureClockDivisionFactor(EDeviceMatrox_DivisionFactor* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_DivisionFactor)_ttoi(GetParamValue(EDeviceParameterMatrox_Timer2DivisionFactor));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetTimer2ExposureClockDivisionFactor(EDeviceMatrox_DivisionFactor eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Timer2DivisionFactor;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_GenerationMode eTimerMode = EDeviceMatrox_GenerationMode_Count;

		if(GetTimer2Mode(&eTimerMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eTimerMode == EDeviceMatrox_GenerationMode_Disable)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_DivisionFactor[nPreValue], g_lpszMatrox_DivisionFactor[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTimer2ExposureClockBaseClockFrequency(double* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterMatrox_Timer2BaseClockFrequency));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetTimer2ExposureClockBaseClockFrequency(double dblParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Timer2BaseClockFrequency;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_GenerationMode eTimerMode = EDeviceMatrox_GenerationMode_Count;

		if(GetTimer2Mode(&eTimerMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eTimerMode == EDeviceMatrox_GenerationMode_Disable)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		EDeviceMatrox_ExposureClockType eBaseClockType = EDeviceMatrox_ExposureClockType_Count;

		if(GetTimer2ExposureClockType(&eBaseClockType))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eBaseClockType != EDeviceMatrox_ExposureClockType_ClockGenerator && eBaseClockType != EDeviceMatrox_ExposureClockType_Aux1_Input_LVDS)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%f' to '%f'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTimer1AdvancedTimerOutputSignal(EDeviceMatrox_TimerOutputSignal* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_TimerOutputSignal)_ttoi(GetParamValue(EDeviceParameterMatrox_Timer1TimerOutputSignal));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetTimer1AdvancedTimerOutputSignal(EDeviceMatrox_TimerOutputSignal eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Timer1TimerOutputSignal;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_GenerationMode eTimerMode = EDeviceMatrox_GenerationMode_Count;

		if(GetTimer1Mode(&eTimerMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eTimerMode == EDeviceMatrox_GenerationMode_Disable)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}
		
		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_TimerOutputSignal[nPreValue], g_lpszMatrox_TimerOutputSignal[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTimer1AdvancedCaptureTrigger(EDeviceMatrox_CaptureATriggerEveryTrigger* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_CaptureATriggerEveryTrigger)_ttoi(GetParamValue(EDeviceParameterMatrox_Timer1CaptureTrigger));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetTimer1AdvancedCaptureTrigger(EDeviceMatrox_CaptureATriggerEveryTrigger eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Timer1CaptureTrigger;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_GenerationMode eTimerMode = EDeviceMatrox_GenerationMode_Count;

		if(GetTimer1Mode(&eTimerMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eTimerMode == EDeviceMatrox_GenerationMode_Disable)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}
		
		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_Captureatriggereverytrigger[nPreValue], g_lpszMatrox_Captureatriggereverytrigger[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTimer1AdvancedTriggerArmMode(EDeviceMatrox_TriggerArmMode* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_TriggerArmMode)_ttoi(GetParamValue(EDeviceParameterMatrox_Timer1TrgArmMode));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetTimer1AdvancedTriggerArmMode(EDeviceMatrox_TriggerArmMode eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Timer1TrgArmMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_GenerationMode eTimerMode = EDeviceMatrox_GenerationMode_Count;

		if(GetTimer1Mode(&eTimerMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eTimerMode == EDeviceMatrox_GenerationMode_Disable)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}
		
		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_TriggerArmMode[nPreValue], g_lpszMatrox_TriggerArmMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTimer1AdvancedTriggerArmFormat(EDeviceMatrox_InoutFormatDefault* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_InoutFormatDefault)_ttoi(GetParamValue(EDeviceParameterMatrox_Timer1TrgArmFormat));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetTimer1AdvancedTriggerArmFormat(EDeviceMatrox_InoutFormatDefault eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Timer1TrgArmFormat;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_GenerationMode eTimerMode = EDeviceMatrox_GenerationMode_Count;

		if(GetTimer1Mode(&eTimerMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eTimerMode == EDeviceMatrox_GenerationMode_Disable)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}
		
		EDeviceMatrox_TriggerArmMode eTrgArmMode = EDeviceMatrox_TriggerArmMode_Count;

		if(GetTimer1AdvancedTriggerArmMode(&eTrgArmMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eTrgArmMode == EDeviceMatrox_TriggerArmMode_Disable)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_InoutFormatDefault[nPreValue], g_lpszMatrox_InoutFormatDefault[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTimer1AdvancedTriggerArmSource(EDeviceMatrox_TriggerArmSource* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_TriggerArmSource)_ttoi(GetParamValue(EDeviceParameterMatrox_Timer1TrgArmSource));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetTimer1AdvancedTriggerArmSource(EDeviceMatrox_TriggerArmSource eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Timer1TrgArmSource;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_GenerationMode eTimerMode = EDeviceMatrox_GenerationMode_Count;

		if(GetTimer1Mode(&eTimerMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eTimerMode == EDeviceMatrox_GenerationMode_Disable)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}
		
		EDeviceMatrox_TriggerArmMode eTrgArmMode = EDeviceMatrox_TriggerArmMode_Count;

		if(GetTimer1AdvancedTriggerArmMode(&eTrgArmMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eTrgArmMode == EDeviceMatrox_TriggerArmMode_Disable)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_TriggerArmSource[nPreValue], g_lpszMatrox_TriggerArmSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTimer1AdvancedTriggerArmPolarity(EDeviceMatrox_InoutPolarity* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_InoutPolarity)_ttoi(GetParamValue(EDeviceParameterMatrox_Timer1TrgArmPolarity));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetTimer1AdvancedTriggerArmPolarity(EDeviceMatrox_InoutPolarity eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Timer1TrgArmPolarity;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_GenerationMode eTimerMode = EDeviceMatrox_GenerationMode_Count;

		if(GetTimer1Mode(&eTimerMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eTimerMode == EDeviceMatrox_GenerationMode_Disable)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}
		
		EDeviceMatrox_TriggerArmMode eTrgArmMode = EDeviceMatrox_TriggerArmMode_Count;

		if(GetTimer1AdvancedTriggerArmMode(&eTrgArmMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eTrgArmMode == EDeviceMatrox_TriggerArmMode_Disable)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_InoutPolarity[nPreValue], g_lpszMatrox_InoutPolarity[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTimer2AdvancedTimerOutputSignal(EDeviceMatrox_TimerOutputSignal* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_TimerOutputSignal)_ttoi(GetParamValue(EDeviceParameterMatrox_Timer2TimerOutputSignal));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetTimer2AdvancedTimerOutputSignal(EDeviceMatrox_TimerOutputSignal eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Timer2TimerOutputSignal;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_GenerationMode eTimerMode = EDeviceMatrox_GenerationMode_Count;

		if(GetTimer2Mode(&eTimerMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eTimerMode == EDeviceMatrox_GenerationMode_Disable)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}
		
		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_TimerOutputSignal[nPreValue], g_lpszMatrox_TimerOutputSignal[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTimer2AdvancedCaptureATrigger(EDeviceMatrox_CaptureATriggerEveryTrigger* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_CaptureATriggerEveryTrigger)_ttoi(GetParamValue(EDeviceParameterMatrox_Timer2CaptureTrigger));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetTimer2AdvancedCaptureATrigger(EDeviceMatrox_CaptureATriggerEveryTrigger eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Timer2CaptureTrigger;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_GenerationMode eTimerMode = EDeviceMatrox_GenerationMode_Count;

		if(GetTimer2Mode(&eTimerMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eTimerMode == EDeviceMatrox_GenerationMode_Disable)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}
		
		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_Captureatriggereverytrigger[nPreValue], g_lpszMatrox_Captureatriggereverytrigger[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTimer2AdvancedTriggerArmMode(EDeviceMatrox_TriggerArmMode* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_TriggerArmMode)_ttoi(GetParamValue(EDeviceParameterMatrox_Timer2TrgArmMode));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetTimer2AdvancedTriggerArmMode(EDeviceMatrox_TriggerArmMode eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Timer2TrgArmMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_GenerationMode eTimerMode = EDeviceMatrox_GenerationMode_Count;

		if(GetTimer2Mode(&eTimerMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eTimerMode == EDeviceMatrox_GenerationMode_Disable)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}
		
		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_TriggerArmMode[nPreValue], g_lpszMatrox_TriggerArmMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTimer2AdvancedTriggerArmFormat(EDeviceMatrox_InoutFormatDefault* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_InoutFormatDefault)_ttoi(GetParamValue(EDeviceParameterMatrox_Timer2TrgArmFormat));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetTimer2AdvancedTriggerArmFormat(EDeviceMatrox_InoutFormatDefault eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Timer2TrgArmFormat;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_GenerationMode eTimerMode = EDeviceMatrox_GenerationMode_Count;

		if(GetTimer2Mode(&eTimerMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eTimerMode == EDeviceMatrox_GenerationMode_Disable)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		EDeviceMatrox_TriggerArmMode eTrgArmMode = EDeviceMatrox_TriggerArmMode_Count;

		if(GetTimer2AdvancedTriggerArmMode(&eTrgArmMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eTrgArmMode == EDeviceMatrox_TriggerArmMode_Disable)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_InoutFormatDefault[nPreValue], g_lpszMatrox_InoutFormatDefault[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTimer2AdvancedTriggerArmSource(EDeviceMatrox_TriggerArmSource* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_TriggerArmSource)_ttoi(GetParamValue(EDeviceParameterMatrox_Timer2TrgArmSource));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetTimer2AdvancedTriggerArmSource(EDeviceMatrox_TriggerArmSource eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Timer2TrgArmSource;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_GenerationMode eTimerMode = EDeviceMatrox_GenerationMode_Count;

		if(GetTimer2Mode(&eTimerMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eTimerMode == EDeviceMatrox_GenerationMode_Disable)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		EDeviceMatrox_TriggerArmMode eTrgArmMode = EDeviceMatrox_TriggerArmMode_Count;

		if(GetTimer2AdvancedTriggerArmMode(&eTrgArmMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eTrgArmMode == EDeviceMatrox_TriggerArmMode_Disable)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_TriggerArmSource[nPreValue], g_lpszMatrox_TriggerArmSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetTimer2AdvancedTriggerArmPolarity(EDeviceMatrox_InoutPolarity* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_InoutPolarity)_ttoi(GetParamValue(EDeviceParameterMatrox_Timer2TrgArmPolarity));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetTimer2AdvancedTriggerArmPolarity(EDeviceMatrox_InoutPolarity eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Timer2TrgArmPolarity;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_GenerationMode eTimerMode = EDeviceMatrox_GenerationMode_Count;

		if(GetTimer2Mode(&eTimerMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eTimerMode == EDeviceMatrox_GenerationMode_Disable)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		EDeviceMatrox_TriggerArmMode eTrgArmMode = EDeviceMatrox_TriggerArmMode_Count;

		if(GetTimer2AdvancedTriggerArmMode(&eTrgArmMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eTrgArmMode == EDeviceMatrox_TriggerArmMode_Disable)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_InoutPolarity[nPreValue], g_lpszMatrox_InoutPolarity[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetGrabMode(EDeviceMatrox_GrabMode* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_GrabMode)_ttoi(GetParamValue(EDeviceParameterMatrox_GrabCharacteristicsMode));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetGrabMode(EDeviceMatrox_GrabMode eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_GrabCharacteristicsMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_GrabMode[nPreValue], g_lpszMatrox_GrabMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetGrabActivationMode(EDeviceMatrox_GrabModeActivation* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_GrabModeActivation)_ttoi(GetParamValue(EDeviceParameterMatrox_GrabActivationMode));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetGrabActivationMode(EDeviceMatrox_GrabModeActivation eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_GrabActivationMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_GrabMode eGrabMode = EDeviceMatrox_GrabMode_Count;

		if(GetGrabMode(&eGrabMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eGrabMode == EDeviceMatrox_GrabMode_Continuous)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_GrabModeActivation[nPreValue], g_lpszMatrox_GrabModeActivation[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetGrabFormat(EDeviceMatrox_InoutFormatDefault* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_InoutFormatDefault)_ttoi(GetParamValue(EDeviceParameterMatrox_GrabTrgFormat));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetGrabFormat(EDeviceMatrox_InoutFormatDefault eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_GrabTrgFormat;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_GrabMode eGrabMode = EDeviceMatrox_GrabMode_Count;

		if(GetGrabMode(&eGrabMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eGrabMode == EDeviceMatrox_GrabMode_Continuous)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_InoutFormatDefault[nPreValue], g_lpszMatrox_InoutFormatDefault[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetGrabSignal(EDeviceMatrox_GrabModeSignal* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_GrabModeSignal)_ttoi(GetParamValue(EDeviceParameterMatrox_GrabTrgSignal));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetGrabSignal(EDeviceMatrox_GrabModeSignal eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_GrabTrgSignal;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_GrabMode eGrabMode = EDeviceMatrox_GrabMode_Count;

		if(GetGrabMode(&eGrabMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eGrabMode == EDeviceMatrox_GrabMode_Continuous)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_GrabModeSignal[nPreValue], g_lpszMatrox_GrabModeSignal[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetGrabPolarity(EDeviceMatrox_InoutPolarity* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_InoutPolarity)_ttoi(GetParamValue(EDeviceParameterMatrox_GrabTrgPolarity));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetGrabPolarity(EDeviceMatrox_InoutPolarity eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_GrabTrgPolarity;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_GrabMode eGrabMode = EDeviceMatrox_GrabMode_Count;

		if(GetGrabMode(&eGrabMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eGrabMode == EDeviceMatrox_GrabMode_Continuous)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_InoutPolarity[nPreValue], g_lpszMatrox_InoutPolarity[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetGrabTriggerArmFormat(EDeviceMatrox_InoutFormatDefault* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_InoutFormatDefault)_ttoi(GetParamValue(EDeviceParameterMatrox_GrabTrgArmFormat));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetGrabTriggerArmFormat(EDeviceMatrox_InoutFormatDefault eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_GrabTrgArmFormat;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_GrabMode eGrabMode = EDeviceMatrox_GrabMode_Count;

		if(GetGrabMode(&eGrabMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eGrabMode == EDeviceMatrox_GrabMode_Continuous)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_InoutFormatDefault[nPreValue], g_lpszMatrox_InoutFormatDefault[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetGrabTriggerArmSignal(EDeviceMatrox_TriggerArmSignal* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_TriggerArmSignal)_ttoi(GetParamValue(EDeviceParameterMatrox_GrabTrgArmSignal));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetGrabTriggerArmSignal(EDeviceMatrox_TriggerArmSignal eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_GrabTrgArmSignal;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_GrabMode eGrabMode = EDeviceMatrox_GrabMode_Count;

		if(GetGrabMode(&eGrabMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eGrabMode == EDeviceMatrox_GrabMode_Continuous)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_TriggerArmSignal[nPreValue], g_lpszMatrox_TriggerArmSignal[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetGrabTriggerArmPolarity(EDeviceMatrox_InoutPolarity* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_InoutPolarity)_ttoi(GetParamValue(EDeviceParameterMatrox_GrabTrgArmPolarity));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetGrabTriggerArmPolarity(EDeviceMatrox_InoutPolarity eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_GrabTrgArmPolarity;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_GrabMode eGrabMode = EDeviceMatrox_GrabMode_Count;

		if(GetGrabMode(&eGrabMode))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eGrabMode == EDeviceMatrox_GrabMode_Continuous)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_InoutPolarity[nPreValue], g_lpszMatrox_InoutPolarity[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetSyncSignalSource(EDeviceMatrox_SyncSignalSource* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_SyncSignalSource)_ttoi(GetParamValue(EDeviceParameterMatrox_SpcSource));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetSyncSignalSource(EDeviceMatrox_SyncSignalSource eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_SpcSource;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_SyncSignalSource[nPreValue], g_lpszMatrox_SyncSignalSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetSyncSignalFormat(EDeviceMatrox_SyncSignalFormat* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_SyncSignalFormat)_ttoi(GetParamValue(EDeviceParameterMatrox_SpcFormat));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetSyncSignalFormat(EDeviceMatrox_SyncSignalFormat eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_SpcFormat;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_SyncSignalFormat[nPreValue], g_lpszMatrox_SyncSignalFormat[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetSyncSignalLatency(int* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMatrox_SpcCameraOutInSignalLatency));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetSyncSignalLatency(int nParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_SpcCameraOutInSignalLatency;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%d' to '%d'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetSyncSignalSynchronizAtionSignalAvailable(EDeviceMatrox_SynchronizAtionSignalAvailable* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_SynchronizAtionSignalAvailable)_ttoi(GetParamValue(EDeviceParameterMatrox_SpcSyncSignalAvailable));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetSyncSignalSynchronizAtionSignalAvailable(EDeviceMatrox_SynchronizAtionSignalAvailable eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_SpcSyncSignalAvailable;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_SynchronizationSignalAvailable[nPreValue], g_lpszMatrox_SynchronizationSignalAvailable[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetSyncSignalBlockSynchronizationTypeEnable(bool* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMatrox_SpcBlockSyncTypeEnable));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetSyncSignalBlockSynchronizationTypeEnable(bool bParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_SpcBlockSyncTypeEnable;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}


		CString strSave;
		strSave.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_Switch[nPreValue], g_lpszMatrox_Switch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetSyncSignalExternalVSYNCSignalIsEnable(bool* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMatrox_SpcExternalVSYNCEnable));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetSyncSignalExternalVSYNCSignalIsEnable(bool bParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_SpcExternalVSYNCEnable;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}


		CString strSave;
		strSave.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_Switch[nPreValue], g_lpszMatrox_Switch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetHSyncInputActivce(bool* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMatrox_HSyncInputActive));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetHSyncInputActivce(bool bParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_HSyncInputActive;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_SynchronizAtionSignalAvailable eSignalAvailable = EDeviceMatrox_SynchronizAtionSignalAvailable_Count;

		if(GetSyncSignalSynchronizAtionSignalAvailable(&eSignalAvailable))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eSignalAvailable != EDeviceMatrox_SynchronizAtionSignalAvailable_HSync_Only && eSignalAvailable != EDeviceMatrox_SynchronizAtionSignalAvailable_HSync_And_VSync)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}


		CString strSave;
		strSave.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_Switch[nPreValue], g_lpszMatrox_Switch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetHSyncInputFormat(EDeviceMatrox_InoutFormat* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_InoutFormat)_ttoi(GetParamValue(EDeviceParameterMatrox_HSyncInputFormat));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetHSyncInputFormat(EDeviceMatrox_InoutFormat eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_HSyncInputFormat;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_SynchronizAtionSignalAvailable eSignalAvailable = EDeviceMatrox_SynchronizAtionSignalAvailable_Count;

		if(GetSyncSignalSynchronizAtionSignalAvailable(&eSignalAvailable))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eSignalAvailable != EDeviceMatrox_SynchronizAtionSignalAvailable_HSync_Only && eSignalAvailable != EDeviceMatrox_SynchronizAtionSignalAvailable_HSync_And_VSync)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		bool bActive = false;

		if(GetHSyncInputActivce(&bActive))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(!bActive)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_InoutFormat[nPreValue], g_lpszMatrox_InoutFormat[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetHSyncInputPolarity(EDeviceMatrox_InoutPolarity* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_InoutPolarity)_ttoi(GetParamValue(EDeviceParameterMatrox_HSyncInputPolarity));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetHSyncInputPolarity(EDeviceMatrox_InoutPolarity eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_HSyncInputPolarity;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_SynchronizAtionSignalAvailable eSignalAvailable = EDeviceMatrox_SynchronizAtionSignalAvailable_Count;

		if(GetSyncSignalSynchronizAtionSignalAvailable(&eSignalAvailable))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eSignalAvailable != EDeviceMatrox_SynchronizAtionSignalAvailable_HSync_Only && eSignalAvailable != EDeviceMatrox_SynchronizAtionSignalAvailable_HSync_And_VSync)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		bool bActive = false;

		if(GetHSyncInputActivce(&bActive))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(!bActive)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_InoutPolarity[nPreValue], g_lpszMatrox_InoutPolarity[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetHSyncOutputActivce(bool* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMatrox_HSyncOutputActive));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetHSyncOutputActivce(bool bParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_HSyncOutputActive;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}
		
		CString strSave;
		strSave.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_Switch[nPreValue], g_lpszMatrox_Switch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetHSyncOutputFormat(EDeviceMatrox_InoutFormat* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_InoutFormat)_ttoi(GetParamValue(EDeviceParameterMatrox_HSyncOutputFormat));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetHSyncOutputFormat(EDeviceMatrox_InoutFormat eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_HSyncOutputFormat;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		bool bActive = false;

		if(GetHSyncOutputActivce(&bActive))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(!bActive)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_InoutFormat[nPreValue], g_lpszMatrox_InoutFormat[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetHSyncOutputPolarity(EDeviceMatrox_InoutPolarity* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_InoutPolarity)_ttoi(GetParamValue(EDeviceParameterMatrox_HSyncOutputPolarity));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetHSyncOutputPolarity(EDeviceMatrox_InoutPolarity eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_HSyncOutputPolarity;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		bool bActive = false;

		if(GetHSyncOutputActivce(&bActive))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(!bActive)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_InoutPolarity[nPreValue], g_lpszMatrox_InoutPolarity[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetVSyncInputActivce(bool* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMatrox_VSyncInputActive));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetVSyncInputActivce(bool bParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_VSyncInputActive;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_SynchronizAtionSignalAvailable eSignalAvailable = EDeviceMatrox_SynchronizAtionSignalAvailable_Count;

		if(GetSyncSignalSynchronizAtionSignalAvailable(&eSignalAvailable))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eSignalAvailable != EDeviceMatrox_SynchronizAtionSignalAvailable_VSync_Only && eSignalAvailable != EDeviceMatrox_SynchronizAtionSignalAvailable_HSync_And_VSync)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}


		CString strSave;
		strSave.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_Switch[nPreValue], g_lpszMatrox_Switch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetVSyncInputFormat(EDeviceMatrox_InoutFormat* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_InoutFormat)_ttoi(GetParamValue(EDeviceParameterMatrox_VSyncInputFormat));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetVSyncInputFormat(EDeviceMatrox_InoutFormat eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_VSyncInputFormat;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_SynchronizAtionSignalAvailable eSignalAvailable = EDeviceMatrox_SynchronizAtionSignalAvailable_Count;

		if(GetSyncSignalSynchronizAtionSignalAvailable(&eSignalAvailable))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eSignalAvailable != EDeviceMatrox_SynchronizAtionSignalAvailable_VSync_Only && eSignalAvailable != EDeviceMatrox_SynchronizAtionSignalAvailable_HSync_And_VSync)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		bool bActive = false;

		if(GetVSyncInputActivce(&bActive))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(!bActive)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_InoutFormat[nPreValue], g_lpszMatrox_InoutFormat[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetVSyncInputPolarity(EDeviceMatrox_InoutPolarity* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_InoutPolarity)_ttoi(GetParamValue(EDeviceParameterMatrox_VSyncInputPolarity));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetVSyncInputPolarity(EDeviceMatrox_InoutPolarity eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_VSyncInputPolarity;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_SynchronizAtionSignalAvailable eSignalAvailable = EDeviceMatrox_SynchronizAtionSignalAvailable_Count;

		if(GetSyncSignalSynchronizAtionSignalAvailable(&eSignalAvailable))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eSignalAvailable != EDeviceMatrox_SynchronizAtionSignalAvailable_VSync_Only && eSignalAvailable != EDeviceMatrox_SynchronizAtionSignalAvailable_HSync_And_VSync)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		bool bActive = false;

		if(GetVSyncInputActivce(&bActive))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(!bActive)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_InoutPolarity[nPreValue], g_lpszMatrox_InoutPolarity[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetVSyncOutputActivce(bool* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMatrox_VSyncOutputActive));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetVSyncOutputActivce(bool bParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_VSyncOutputActive;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}


		CString strSave;
		strSave.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_Switch[nPreValue], g_lpszMatrox_Switch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetVSyncOutputFormat(EDeviceMatrox_InoutFormat* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_InoutFormat)_ttoi(GetParamValue(EDeviceParameterMatrox_VSyncOutputFormat));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetVSyncOutputFormat(EDeviceMatrox_InoutFormat eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_VSyncOutputFormat;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		bool bActive = false;

		if(GetVSyncOutputActivce(&bActive))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(!bActive)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_InoutFormat[nPreValue], g_lpszMatrox_InoutFormat[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetVSyncOutputPolarity(EDeviceMatrox_InoutPolarity* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_InoutPolarity)_ttoi(GetParamValue(EDeviceParameterMatrox_VSyncOutputPolarity));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetVSyncOutputPolarity(EDeviceMatrox_InoutPolarity eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_VSyncOutputPolarity;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		bool bActive = false;

		if(GetVSyncOutputActivce(&bActive))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(!bActive)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_InoutPolarity[nPreValue], g_lpszMatrox_InoutPolarity[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetCSyncInputActivce(bool* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMatrox_CSyncInputActive));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetCSyncInputActivce(bool bParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_CSyncInputActive;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_SynchronizAtionSignalAvailable eSignalAvailable = EDeviceMatrox_SynchronizAtionSignalAvailable_Count;

		if(GetSyncSignalSynchronizAtionSignalAvailable(&eSignalAvailable))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eSignalAvailable != EDeviceMatrox_SynchronizAtionSignalAvailable_CSync)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}


		CString strSave;
		strSave.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_Switch[nPreValue], g_lpszMatrox_Switch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetCSyncInputFormat(EDeviceMatrox_InoutFormat* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_InoutFormat)_ttoi(GetParamValue(EDeviceParameterMatrox_CSyncInputFormat));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetCSyncInputFormat(EDeviceMatrox_InoutFormat eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_CSyncInputFormat;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_SynchronizAtionSignalAvailable eSignalAvailable = EDeviceMatrox_SynchronizAtionSignalAvailable_Count;

		if(GetSyncSignalSynchronizAtionSignalAvailable(&eSignalAvailable))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eSignalAvailable != EDeviceMatrox_SynchronizAtionSignalAvailable_CSync)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		bool bActive = false;

		if(GetCSyncInputActivce(&bActive))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(!bActive)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_InoutFormat[nPreValue], g_lpszMatrox_InoutFormat[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetCSyncInputPolarity(EDeviceMatrox_InoutPolarity* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_InoutPolarity)_ttoi(GetParamValue(EDeviceParameterMatrox_CSyncInputPolarity));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetCSyncInputPolarity(EDeviceMatrox_InoutPolarity eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_CSyncInputPolarity;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDeviceMatrox_SynchronizAtionSignalAvailable eSignalAvailable = EDeviceMatrox_SynchronizAtionSignalAvailable_Count;

		if(GetSyncSignalSynchronizAtionSignalAvailable(&eSignalAvailable))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eSignalAvailable != EDeviceMatrox_SynchronizAtionSignalAvailable_CSync)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		bool bActive = false;

		if(GetCSyncInputActivce(&bActive))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(!bActive)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_InoutPolarity[nPreValue], g_lpszMatrox_InoutPolarity[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetCSyncOutputActivce(bool* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMatrox_CSyncOutputActive));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetCSyncOutputActivce(bool bParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_CSyncOutputActive;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(true)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_Switch[nPreValue], g_lpszMatrox_Switch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetCSyncOutputFormat(EDeviceMatrox_InoutFormat* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_InoutFormat)_ttoi(GetParamValue(EDeviceParameterMatrox_CSyncOutputFormat));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetCSyncOutputFormat(EDeviceMatrox_InoutFormat eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_CSyncOutputFormat;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(true)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_InoutFormat[nPreValue], g_lpszMatrox_InoutFormat[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetCSyncOutputPolarity(EDeviceMatrox_InoutPolarity* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_InoutPolarity)_ttoi(GetParamValue(EDeviceParameterMatrox_CSyncOutputPolarity));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetCSyncOutputPolarity(EDeviceMatrox_InoutPolarity eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_CSyncOutputPolarity;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(true)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_InoutPolarity[nPreValue], g_lpszMatrox_InoutPolarity[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetCSyncSerrationPulse(bool* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMatrox_CSyncSerrationPulse));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetCSyncSerrationPulse(bool bParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_CSyncSerrationPulse;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(true)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}


		CString strSave;
		strSave.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_Switch[nPreValue], g_lpszMatrox_Switch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetCSyncEqualizationPulse(bool* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMatrox_CSyncEqualizationPulse));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetCSyncEqualizationPulse(bool bParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_CSyncEqualizationPulse;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(true)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}
		
		CString strSave;
		strSave.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_Switch[nPreValue], g_lpszMatrox_Switch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetCameraControlBitsSourcesControl1(EDeviceMatrox_CameraBitsSourcesControl* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_CameraBitsSourcesControl)_ttoi(GetParamValue(EDeviceParameterMatrox_Control1));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetCameraControlBitsSourcesControl1(EDeviceMatrox_CameraBitsSourcesControl eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Control1;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		bool bControl = false;

		if(GetEnableCCOutputsOnConnector1(&bControl))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(!bControl)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_CameraBitsSourcesControl[nPreValue], g_lpszMatrox_CameraBitsSourcesControl[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetCameraControlBitsSourcesControl2(EDeviceMatrox_CameraBitsSourcesControl* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_CameraBitsSourcesControl)_ttoi(GetParamValue(EDeviceParameterMatrox_Control2));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetCameraControlBitsSourcesControl2(EDeviceMatrox_CameraBitsSourcesControl eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Control2;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		bool bControl = false;

		if(GetEnableCCOutputsOnConnector1(&bControl))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(!bControl)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_CameraBitsSourcesControl[nPreValue], g_lpszMatrox_CameraBitsSourcesControl[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetCameraControlBitsSourcesControl3(EDeviceMatrox_CameraBitsSourcesControl* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_CameraBitsSourcesControl)_ttoi(GetParamValue(EDeviceParameterMatrox_Control3));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetCameraControlBitsSourcesControl3(EDeviceMatrox_CameraBitsSourcesControl eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Control3;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		bool bControl = false;

		if(GetEnableCCOutputsOnConnector1(&bControl))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(!bControl)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_CameraBitsSourcesControl[nPreValue], g_lpszMatrox_CameraBitsSourcesControl[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetCameraControlBitsSourcesControl4(EDeviceMatrox_CameraBitsSourcesControl* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMatrox_CameraBitsSourcesControl)_ttoi(GetParamValue(EDeviceParameterMatrox_Control4));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetCameraControlBitsSourcesControl4(EDeviceMatrox_CameraBitsSourcesControl eParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_Control4;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		bool bControl = false;

		if(GetEnableCCOutputsOnConnector1(&bControl))
		{
			eReturn = EMatroxSetFunction_ReadOnDatabaseError;
			break;
		}

		if(!bControl)
		{
			eReturn = EMatroxSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_CameraBitsSourcesControl[nPreValue], g_lpszMatrox_CameraBitsSourcesControl[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetEnableCCOutputsOnConnector1(bool* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMatrox_EnableCCoutputsConnector1));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetEnableCCOutputsOnConnector1(bool bParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_EnableCCoutputsConnector1;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}


		CString strSave;
		strSave.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_Switch[nPreValue], g_lpszMatrox_Switch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMatroxGetFunction CDeviceMatroxIntellicam::GetEnableCCOutputsOnConnector2(bool* pParam)
{
	EMatroxGetFunction eReturn = EMatroxGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMatroxGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMatrox_EnableCCoutputsConnector2));

		eReturn = EMatroxGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMatroxSetFunction CDeviceMatroxIntellicam::SetEnableCCOutputsOnConnector2(bool bParam)
{
	EMatroxSetFunction eReturn = EMatroxSetFunction_UnknownError;

	EDeviceParameterMatrox eSaveID = EDeviceParameterMatrox_EnableCCoutputsConnector2;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMatroxSetFunction_AlreadyInitializedError;
			break;
		}


		CString strSave;
		strSave.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMatroxSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMatroxSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDeviceParameterMatrox[eSaveID], g_lpszMatrox_Switch[nPreValue], g_lpszMatrox_Switch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

MIL_INT MFTYPE CDeviceMatroxIntellicam::CallbackFunction(MIL_INT HookType, MIL_ID HookId, void* HookDataPtr)
{
	MIL_INT milReturn = -1;

	do
	{
		CDeviceMatroxIntellicam* pInstance = (CDeviceMatroxIntellicam*)HookDataPtr;

		if(!pInstance)
			break;

		--pInstance->m_nCurrentCount;

		int nGrabCount = 0;

		if(pInstance->GetGrabCount(&nGrabCount))
			break;
		
		if(nGrabCount < 1)
			nGrabCount = LONG_MAX;

		int nGrab = nGrabCount;

		--nGrabCount;

		nGrabCount -= pInstance->m_nCurrentCount;

		if(nGrabCount < 0)
			break;

		int nGrabBufferCount = 0;

		if(pInstance->GetMILBufferCount(&nGrabBufferCount))
			break;

		pInstance->NextImageIndex();

		CRavidImage* pCurrentImage = pInstance->GetImageInfo();

		BYTE* pCurrentBuffer = pCurrentImage->GetBuffer();

		int nSelectImage = nGrabCount % nGrabBufferCount;

		MbufGet(pInstance->m_pMilImageList[nSelectImage], pCurrentBuffer);

		pInstance->ConnectImage(false);

		CEventHandlerManager::BroadcastOnAcquisition(pCurrentImage, pInstance->GetObjectID(), pInstance->GetImageView(), pInstance);

		if(pInstance->m_nCurrentCount <= 0)
		{
			MdigProcess(pInstance->m_hMilDigitizer, pInstance->m_pMilImageList, nGrabBufferCount, M_STOP, M_DEFAULT, CDeviceMatroxIntellicam::CallbackFunction, pInstance);
			pInstance->m_bIsGrabAvailable = true;
		}

		milReturn = 0;
	}
	while(false);

	return milReturn;
}

bool CDeviceMatroxIntellicam::IsError()
{
	return (MappGetError(M_GLOBAL, M_NULL) != M_NULL_ERROR);
}

bool CDeviceMatroxIntellicam::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = false;

	bool bFoundID = true;

	do 
	{
 		switch(nParam)
 		{
		case EDeviceParameterMatrox_DeviceID:
			{
				bReturn = !SetDeviceID(strValue);
			}
			break;
		case EDeviceParameterMatrox_SubUnitID:
			{
				bReturn = !SetSubUnitID(strValue);
			}
			break;
		case EDeviceParameterMatrox_CameraName:
			{
				bReturn = !SetCameraName(strValue);
			}
			break;
		case EDeviceParameterMatrox_InitializeType:
			{
				bReturn = !SetInitilizeType((EDeviceMatrox_InitilizeType)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_Camfilepath:
			{
				bReturn = !SetCamfilePath(strValue);
			}
			break;
		case EDeviceParameterMatrox_Parametermatch:
			{
				bReturn = !SetParameterMatch(_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_GrabCount:
			{
				bReturn = !SetGrabCount(_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_MILImageCount:
			{
				bReturn = !SetMILImageCount(_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_MILBufferCount:
			{
				bReturn = !SetMILBufferCount(_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_GrabDirectionX:
			{
				bReturn = !SetGrabDirectionX((EDeviceMatrox_DirectionSwitch)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_GrabDirectionY:
			{
				bReturn = !SetGrabDirectionY((EDeviceMatrox_DirectionSwitch)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_GrabScale:
			{
				bReturn = !SetGrabScale(_ttof(strValue));
			}
			break;
		case EDeviceParameterMatrox_GrabScaleX:
			{
				bReturn = !SetGrabScaleX(_ttof(strValue));
			}
			break;
		case EDeviceParameterMatrox_GrabScaleY:
			{
				bReturn = !SetGrabScaleY(_ttof(strValue));
			}
			break;
		case EDeviceParameterMatrox_SourceOffsetX:
			{
				bReturn = !SetSourceOffsetX(_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_SourceOffsetY:
			{
				bReturn = !SetSourceOffsetY(_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_SourceSizeX:
			{
				bReturn = !SetSourceSizeX(_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_SourceSizeY:
			{
				bReturn = !SetSourceSizeY(_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_GrabTriggerContinuouesEndTrigger:
			{
				bReturn = !SetbTriggerContinuouesEndTrigger((EDeviceMatrox_ActiveSwitch)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_GrabTriggerActivation:
			{
				bReturn = !SetGrabTriggerActivation((EDeviceMatrox_ActivationSelect)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_GrabTriggerSoursce:
			{
				bReturn = !SetGrabTriggerSoursce((EDeviceMatrox_TriggerSourceSelect)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_GrabTriggerState:
			{
				bReturn = !SetGrabTriggerState((EDeviceMatrox_ActiveSwitch)_ttoi(strValue));
			}
			break;
// 		case EDeviceParameterMatrox_TimerSelector:
// 			{
// 				bReturn = !SetTimerSelector((EDeviceMatrox_TimerSelect)_ttoi(strValue));
// 			}
// 			break;
		case EDeviceParameterMatrox_CameraType:
			{
				bReturn = !SetCameraType((EDeviceMatrox_CameraType)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_CameraNumberOfTaps:
			{
				bReturn = !SetCameraNumberOfTaps((EDeviceMatrox_NumberOfTaps)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_CameraBayerMode:
			{
				bReturn = !SetCameraBayerMode((EDeviceMatrox_BayerMode)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_ConfigurationType:
			{
				bReturn = !SetCameraLinkConfiguration((EDeviceMatrox_ConfigurationType)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_CameraMode:
			{
				bReturn = !SetCameraMode((EDeviceMatrox_CameraTapsMode)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_RegionsX:
			{
				bReturn = !SetRegionsX((EDeviceMatrox_TapsSelector)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_RegionsY:
			{
				bReturn = !SetRegionsY((EDeviceMatrox_TapsSelector)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_AdjacentX:
			{
				bReturn = !SetAdjacentPixelsX((EDeviceMatrox_TapsSelector)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_AdjacentY:
			{
				bReturn = !SetAdjacentPixelsY((EDeviceMatrox_TapsSelector)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_Tap1:
			{
				bReturn = !SetTaps1Direction((EDeviceMatrox_TapsDirection)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_Tap2:
			{
				bReturn = !SetTaps2Direction((EDeviceMatrox_TapsDirection)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_Tap3:
			{
				bReturn = !SetTaps3Direction((EDeviceMatrox_TapsDirection)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_Tap4:
			{
				bReturn = !SetTaps4Direction((EDeviceMatrox_TapsDirection)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_Tap5:
			{
				bReturn = !SetTaps5Direction((EDeviceMatrox_TapsDirection)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_Tap6:
			{
				bReturn = !SetTaps6Direction((EDeviceMatrox_TapsDirection)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_Tap7:
			{
				bReturn = !SetTaps7Direction((EDeviceMatrox_TapsDirection)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_Tap8:
			{
				bReturn = !SetTaps8Direction((EDeviceMatrox_TapsDirection)_ttoi(strValue));
			}
			break;
			
		case EDeviceParameterMatrox_VideoSignalType:
			{
				bReturn = !SetVideoSignalType((EDeviceMatrox_VideoSignalType)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_VideoSignalDatabusWidth:
			{
				bReturn = !SetDataBusWidth((EDeviceMatrox_DataBusWidth)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_VideoSignalStandard:
			{
				bReturn = !SetVideoSignalStandard((EDeviceMatrox_Standard)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_VideoSignalMILChannelInput:
			{
				bReturn = !SetMILChannel((EDeviceMatrox_MILChannel)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_DigitalVideoSignalFormat:
			{
				bReturn = !SetVideoSignalFormat((EDeviceMatrox_InoutFormat)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_ValidSignalSource:
			{
				bReturn = !SetVideoSignalSource((EDeviceMatrox_ValidSignalSource)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_TimingsVertical:
			{
				bReturn = !SetTimingsVertical((EDeviceMatrox_InterlacedSelector)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_PixelClockFrqAutoAdjust:
			{
				bReturn = !SetVideoTimingAutoAdjust(_ttoi(strValue));
			}
			break;
			
		case EDeviceParameterMatrox_HorizontalSyncClk:
			{
				bReturn = !SetHorizontalSyncClk(_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_HorizontalBPorchClk:
			{
				bReturn = !SetHorizontalBPorchClk(_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_HorizontalActiveClk:
			{
				bReturn = !SetHorizontalActiveClk(_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_HorizontalFPorchClk:
			{
				bReturn = !SetHorizontalFPorchClk(_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_HorizontalFrequencyHz:
			{
				bReturn = !SetHorizontalFrequencyHz(_ttof(strValue));
			}
			break;
		case EDeviceParameterMatrox_HorizontalLockActiveAndTotal:
			{
				bReturn = !SetHorizontalLockActiveAndTotal(_ttoi(strValue));
			}
			break;
			
		case EDeviceParameterMatrox_VerticalSyncLine:
			{
				bReturn = !SetVerticalSyncLines(_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_VerticalBPorchLine:
			{
				bReturn = !SetVerticalBPorchLines(_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_VerticalActiveLine:
			{
				bReturn = !SetVerticalActiveLines(_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_VerticalFPorchLine:
			{
				bReturn = !SetVerticalFPorchLines(_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_VerticalFrequencyHz:
			{
				bReturn = !SetVerticalFrequencyHz(_ttof(strValue));
			}
			break;
		case EDeviceParameterMatrox_VerticalLockActiveAndTotal:
			{
				bReturn = !SetVerticalLockActiveAndTotal(_ttoi(strValue));
			}
			break;
			
		case EDeviceParameterMatrox_ImagesizeX:
			{
				bReturn = !SetVideoTimingImagesizeX(_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_ImagesizeY:
			{
				bReturn = !SetVideoTimingImagesizeY(_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_DelayX:
			{
				bReturn = !SetVideoTimingDelayX(_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_DelayY:
			{
				bReturn = !SetVideoTimingDelayY(_ttoi(strValue));
			}
			break;
			
		case EDeviceParameterMatrox_PixelClockFrequencyMHz:
			{
				bReturn = !SetPixelClockFrequency(_ttof(strValue));
			}
			break;
		case EDeviceParameterMatrox_AutoAdjustVideoTimingsPage:
			{
				bReturn = !SetAutoAdjustInTheVideoTimingsPage((EDeviceMatrox_AdjustInTheVideoTimingsPage)_ttoi(strValue));
			}
			break;

		case EDeviceParameterMatrox_SendToExternalCircuit:
			{
				bReturn = !SetSendToExternalCircuitOtherThanCamera(_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_ExternalClockSignal:
			{
				bReturn = !SetExternalClockSignal((EDeviceMatrox_ExternalClockSignal)_ttoi(strValue));
			}
			break;
			
		case EDeviceParameterMatrox_EXTInputFrequency:
			{
				bReturn = !SetPixelClockInputFrequency((EDeviceMatrox_PixelClockFrequency)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_EXTInputFormat:
			{
				bReturn = !SetPixelClockInputFormat((EDeviceMatrox_InoutFormat)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_EXTInputPolarity:
			{
				bReturn = !SetPixelClockInputPolarity((EDeviceMatrox_InoutPolarity)_ttoi(strValue));
			}
			break;

		case EDeviceParameterMatrox_EXTOutputFrequency:
			{
				bReturn = !SetPixelClockOutputFrequency((EDeviceMatrox_PixelClockFrequency)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_EXTOutputFormat:
			{
				bReturn = !SetPixelClockOutputFormat((EDeviceMatrox_InoutFormat)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_EXTOutputPolarity:
			{
				bReturn = !SetPixelClockOutputPolarity((EDeviceMatrox_InoutPolarity)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_EXTIODelay:
			{
				bReturn = !SetPixelClockDelay(_ttoi(strValue));
			}
			break;
			
		case EDeviceParameterMatrox_Timer1Mode:
			{
				bReturn = !SetTimer1Mode((EDeviceMatrox_GenerationMode)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_Timer1ExpFormat:
			{
				bReturn = !SetTimer1Format((EDeviceMatrox_InoutFormatDefault)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_Timer1ExpPolarity:
			{
				bReturn = !SetTimer1Polarity((EDeviceMatrox_InoutPolarity)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_Timer1TrgFormat:
			{
				bReturn = !SetTimer1TrgFormat((EDeviceMatrox_InoutFormatDefault)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_Timer1TrgSignal:
			{
				bReturn = !SetTimer1TrgSignal((EDeviceMatrox_ExposureTriggerSignal)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_Timer1TrgPolarity:
			{
				bReturn = !SetTimer1TrgPolarity((EDeviceMatrox_InoutPolarity)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_Timer1Pulse1Clk:
			{
				bReturn = !SetTimer1Pulse1Clk(_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_Timer1Delay1Clk:
			{
				bReturn = !SetTimer1Delay1Clk(_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_Timer1Pulse2Clk:
			{
				bReturn = !SetTimer1Pulse2Clk(_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_Timer1Delay2Clk:
			{
				bReturn = !SetTimer1Delay2Clk(_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_Timer1Type:
			{
				bReturn = !SetTimer1ExposureClockType((EDeviceMatrox_ExposureClockType)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_Timer1DivisionFactor:
			{
				bReturn = !SetTimer1ExposureClockDivisionFactor((EDeviceMatrox_DivisionFactor)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_Timer1BaseClockFrequency:
			{
				bReturn = !SetTimer1ExposureClockBaseClockFrequency(_ttof(strValue));
			}
			break;
		case EDeviceParameterMatrox_Timer2Mode:
			{
				bReturn = !SetTimer2Mode((EDeviceMatrox_GenerationMode)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_Timer2ExpFormat:
			{
				bReturn = !SetTimer2Format((EDeviceMatrox_InoutFormatDefault)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_Timer2ExpPolarity:
			{
				bReturn = !SetTimer2Polarity((EDeviceMatrox_InoutPolarity)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_Timer2TrgFormat:
			{
				bReturn = !SetTimer2TrgFormat((EDeviceMatrox_InoutFormatDefault)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_Timer2TrgSignal:
			{
				bReturn = !SetTimer2TrgSignal((EDeviceMatrox_ExposureTriggerSignal)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_Timer2TrgPolarity:
			{
				bReturn = !SetTimer2TrgPolarity((EDeviceMatrox_InoutPolarity)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_Timer2Pulse1Clk:
			{
				bReturn = !SetTimer2Pulse1Clk(_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_Timer2Delay1Clk:
			{
				bReturn = !SetTimer2Delay1Clk(_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_Timer2Pulse2Clk:
			{
				bReturn = !SetTimer2Pulse2Clk(_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_Timer2Delay2Clk:
			{
				bReturn = !SetTimer2Delay2Clk(_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_Timer2Type:
			{
				bReturn = !SetTimer2ExposureClockType((EDeviceMatrox_ExposureClockType)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_Timer2DivisionFactor:
			{
				bReturn = !SetTimer2ExposureClockDivisionFactor((EDeviceMatrox_DivisionFactor)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_Timer2BaseClockFrequency:
			{
				bReturn = !SetTimer2ExposureClockBaseClockFrequency(_ttof(strValue));
			}
			break;
		case EDeviceParameterMatrox_Timer1TimerOutputSignal:
			{
				bReturn = !SetTimer1AdvancedTimerOutputSignal((EDeviceMatrox_TimerOutputSignal)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_Timer1CaptureTrigger:
			{
				bReturn = !SetTimer1AdvancedCaptureTrigger((EDeviceMatrox_CaptureATriggerEveryTrigger)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_Timer1TrgArmMode:
			{
				bReturn = !SetTimer1AdvancedTriggerArmMode((EDeviceMatrox_TriggerArmMode)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_Timer1TrgArmFormat:
			{
				bReturn = !SetTimer1AdvancedTriggerArmFormat((EDeviceMatrox_InoutFormatDefault)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_Timer1TrgArmSource:
			{
				bReturn = !SetTimer1AdvancedTriggerArmSource((EDeviceMatrox_TriggerArmSource)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_Timer1TrgArmPolarity:
			{
				bReturn = !SetTimer1AdvancedTriggerArmPolarity((EDeviceMatrox_InoutPolarity)_ttoi(strValue));
			}
			break;
			
		case EDeviceParameterMatrox_Timer2TimerOutputSignal:
			{
				bReturn = !SetTimer2AdvancedTimerOutputSignal((EDeviceMatrox_TimerOutputSignal)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_Timer2CaptureTrigger:
			{
				bReturn = !SetTimer2AdvancedCaptureATrigger((EDeviceMatrox_CaptureATriggerEveryTrigger)_ttoi(strValue));
			}
			break;
			
		case EDeviceParameterMatrox_Timer2TrgArmMode:
			{
				bReturn = !SetTimer2AdvancedTriggerArmMode((EDeviceMatrox_TriggerArmMode)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_Timer2TrgArmFormat:
			{
				bReturn = !SetTimer2AdvancedTriggerArmFormat((EDeviceMatrox_InoutFormatDefault)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_Timer2TrgArmSource:
			{
				bReturn = !SetTimer2AdvancedTriggerArmSource((EDeviceMatrox_TriggerArmSource)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_Timer2TrgArmPolarity:
			{
				bReturn = !SetTimer2AdvancedTriggerArmPolarity((EDeviceMatrox_InoutPolarity)_ttoi(strValue));
			}
			break;
			
		case EDeviceParameterMatrox_GrabCharacteristicsMode:
			{
				bReturn = !SetGrabMode((EDeviceMatrox_GrabMode)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_GrabActivationMode:
			{
				bReturn = !SetGrabActivationMode((EDeviceMatrox_GrabModeActivation)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_GrabTrgFormat:
			{
				bReturn = !SetGrabFormat((EDeviceMatrox_InoutFormatDefault)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_GrabTrgSignal:
			{
				bReturn = !SetGrabSignal((EDeviceMatrox_GrabModeSignal)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_GrabTrgPolarity:
			{
				bReturn = !SetGrabPolarity((EDeviceMatrox_InoutPolarity)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_GrabTrgArmFormat:
			{
				bReturn = !SetGrabTriggerArmFormat((EDeviceMatrox_InoutFormatDefault)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_GrabTrgArmSignal:
			{
				bReturn = !SetGrabTriggerArmSignal((EDeviceMatrox_TriggerArmSignal)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_GrabTrgArmPolarity:
			{
				bReturn = !SetGrabTriggerArmPolarity((EDeviceMatrox_InoutPolarity)_ttoi(strValue));
			}
			break;
			
		case EDeviceParameterMatrox_SpcSource:
			{
				bReturn = !SetSyncSignalSource((EDeviceMatrox_SyncSignalSource)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_SpcFormat:
			{
				bReturn = !SetSyncSignalFormat((EDeviceMatrox_SyncSignalFormat)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_SpcCameraOutInSignalLatency:
			{
				bReturn = !SetSyncSignalLatency(_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_SpcSyncSignalAvailable:
			{
				bReturn = !SetSyncSignalSynchronizAtionSignalAvailable((EDeviceMatrox_SynchronizAtionSignalAvailable)_ttoi(strValue));
			}
			break;			
		case EDeviceParameterMatrox_SpcBlockSyncTypeEnable:
			{
				bReturn = !SetSyncSignalBlockSynchronizationTypeEnable(_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_SpcExternalVSYNCEnable:
			{
				bReturn = !SetSyncSignalExternalVSYNCSignalIsEnable(_ttoi(strValue));
			}
			break;

		case EDeviceParameterMatrox_HSyncInputActive:
			{
				bReturn = !SetHSyncInputActivce(_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_HSyncInputFormat:
			{
				bReturn = !SetHSyncInputFormat((EDeviceMatrox_InoutFormat)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_HSyncInputPolarity:
			{
				bReturn = !SetHSyncInputPolarity((EDeviceMatrox_InoutPolarity)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_HSyncOutputActive:
			{
				bReturn = !SetHSyncOutputActivce(_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_HSyncOutputFormat:
			{
				bReturn = !SetHSyncOutputFormat((EDeviceMatrox_InoutFormat)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_HSyncOutputPolarity:
			{
				bReturn = !SetHSyncOutputPolarity((EDeviceMatrox_InoutPolarity)_ttoi(strValue));
			}
			break;
			
		case EDeviceParameterMatrox_VSyncInputActive:
			{
				bReturn = !SetVSyncInputActivce(_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_VSyncInputFormat:
			{
				bReturn = !SetVSyncInputFormat((EDeviceMatrox_InoutFormat)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_VSyncInputPolarity:
			{
				bReturn = !SetVSyncInputPolarity((EDeviceMatrox_InoutPolarity)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_VSyncOutputActive:
			{
				bReturn = !SetVSyncOutputActivce(_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_VSyncOutputFormat:
			{
				bReturn = !SetVSyncOutputFormat((EDeviceMatrox_InoutFormat)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_VSyncOutputPolarity:
			{
				bReturn = !SetVSyncOutputPolarity((EDeviceMatrox_InoutPolarity)_ttoi(strValue));
			}
			break;
			
		case EDeviceParameterMatrox_CSyncInputActive:
			{
				bReturn = !SetCSyncInputActivce(_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_CSyncInputFormat:
			{
				bReturn = !SetCSyncInputFormat((EDeviceMatrox_InoutFormat)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_CSyncInputPolarity:
			{
				bReturn = !SetCSyncInputPolarity((EDeviceMatrox_InoutPolarity)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_CSyncOutputActive:
			{
				bReturn = !SetCSyncOutputActivce(_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_CSyncOutputFormat:
			{
				bReturn = !SetCSyncOutputFormat((EDeviceMatrox_InoutFormat)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_CSyncOutputPolarity:
			{
				bReturn = !SetCSyncOutputPolarity((EDeviceMatrox_InoutPolarity)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_CSyncSerrationPulse:
			{
				bReturn = !SetCSyncSerrationPulse(_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_CSyncEqualizationPulse:
			{
				bReturn = !SetCSyncEqualizationPulse(_ttoi(strValue));
			}
			break;
			
		case EDeviceParameterMatrox_Control1:
			{
				bReturn = !SetCameraControlBitsSourcesControl1((EDeviceMatrox_CameraBitsSourcesControl)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_Control2:
			{
				bReturn = !SetCameraControlBitsSourcesControl2((EDeviceMatrox_CameraBitsSourcesControl)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_Control3:
			{
				bReturn = !SetCameraControlBitsSourcesControl3((EDeviceMatrox_CameraBitsSourcesControl)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_Control4:
			{
				bReturn = !SetCameraControlBitsSourcesControl4((EDeviceMatrox_CameraBitsSourcesControl)_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_EnableCCoutputsConnector1:
			{
				bReturn = !SetEnableCCOutputsOnConnector1(_ttoi(strValue));
			}
			break;
		case EDeviceParameterMatrox_EnableCCoutputsConnector2:
			{
				bReturn = !SetEnableCCOutputsOnConnector2(_ttoi(strValue));
			}
			break;
		default:
			{
				bFoundID = false;
			}
			break;
		}



	} 
	while(false);

	if(!bFoundID)
	{
		CString strMessage;

		strMessage.Format(_T("Failed to find the parameter"));

		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);
	}

	return bReturn;
}

bool CDeviceMatroxIntellicam::DoesModuleExist()
{
	bool bReturn = false;

	CString strModuleName;

	do
	{
		strModuleName.Format(_T("mil.dll"));

		EDeviceLibraryStatus eStatus = CLibraryManager::GetFoundLibrary(strModuleName);

		if(!eStatus)
		{
			HMODULE hModule = LoadLibrary(strModuleName);

			if(!hModule)
			{
				CLibraryManager::SetFoundLibrary(strModuleName, false);
				break;
			}

			FreeLibrary(hModule);

			CLibraryManager::SetFoundLibrary(strModuleName, true);

			bReturn = true;
		}
		else
			bReturn = eStatus == EDeviceLibraryStatus_FoundTrue ? true : false;

		if(!bReturn)
			break;

		strModuleName.Format(_T("milmp.dll"));

		eStatus = CLibraryManager::GetFoundLibrary(strModuleName);

		if(!eStatus)
		{
			HMODULE hModule = LoadLibrary(strModuleName);

			if(!hModule)
			{
				CLibraryManager::SetFoundLibrary(strModuleName, false);
				break;
			}

			FreeLibrary(hModule);

			CLibraryManager::SetFoundLibrary(strModuleName, true);

			bReturn = true;
		}
		else
			bReturn = eStatus == EDeviceLibraryStatus_FoundTrue ? true : false;

		if(!bReturn)
			break;

		strModuleName.Format(_T("milcacheinfo.dll"));

		eStatus = CLibraryManager::GetFoundLibrary(strModuleName);

		if(!eStatus)
		{
			HMODULE hModule = LoadLibrary(strModuleName);

			if(!hModule)
			{
				CLibraryManager::SetFoundLibrary(strModuleName, false);
				break;
			}

			FreeLibrary(hModule);

			CLibraryManager::SetFoundLibrary(strModuleName, true);

			bReturn = true;
		}
		else
			bReturn = eStatus == EDeviceLibraryStatus_FoundTrue ? true : false;
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, CMultiLanguageManager::GetString(ELanguageParameter__s_d_s_toload_s), GetClassNameStr(), GetObjectID(), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), strModuleName);

	return bReturn;
}

#endif