#include "stdafx.h"
#include "DeviceGenICam.h"

#ifdef USING_DEVICE

#include "../RavidFramework/RavidImageView.h"

#include "../RavidFramework/MultiLanguageManager.h"
#include "../RavidFramework/LogManager.h"
#include "../RavidFramework/DeviceManager.h"
#include "../RavidFramework/LibraryManager.h"
#include "../RavidFramework/EventHandlerManager.h"

#include "../RavidCore/RavidImage.h"

#include "../Libraries/Includes/GenICam/GenTL.h"
#include "../Libraries/Includes/GenICam/GenApi/IPort.h"
#include "../Libraries/Includes/GenICam/GenApi/NodeMapRef.h"
#include "../Libraries/Includes/GenICam/GenApi/ICommand.h"
#include "../Libraries/Includes/GenICam/GenApi/IBoolean.h"
// gcbase_md_vc141_v3_2.dll
// genapi_md_vc141_v3_2.dll
#pragma comment(lib, COMMONLIB_PREFIX "GenICam/GCBase_MD_VC141_v3_2.lib")
#pragma comment(lib, COMMONLIB_PREFIX "GenICam/GenApi_MD_VC141_v3_2.lib")

using namespace Ravid;
using namespace Ravid::Device;
using namespace Ravid::Database;
using namespace Ravid::Framework;
using namespace Ravid::Algorithms;
using namespace Ravid::Miscellaneous;

#define BUFF_COUNT 4

class CPort
	: public GENAPI_NAMESPACE::IPort
{
public:
	CPort(GenTL::PORT_HANDLE hPort, HMODULE hModule)
	{
		m_hPort = hPort;
		m_hModule = hModule;
	}
	virtual ~CPort(void)
	{

	}

	virtual void Read(void *pBuffer, int64_t Address, int64_t Length)
	{
		GenTL::PGCReadPort GCReadPort(reinterpret_cast<GenTL::PGCReadPort>(reinterpret_cast<void *>(GetProcAddress(m_hModule, ("GCReadPort")))));

		size_t iSize = (size_t)Length;
		GenTL::GC_ERROR status = GenTL::GC_ERR_SUCCESS;
		status = GCReadPort(m_hPort, Address, pBuffer, &iSize);
	}

	virtual void Write(const void *pBuffer, int64_t Address, int64_t Length)
	{
		GenTL::PGCWritePort GCWritePort(reinterpret_cast<GenTL::PGCWritePort>(reinterpret_cast<void *>(GetProcAddress(m_hModule, ("GCWritePort")))));

		size_t iSize = (size_t)Length;
		GenTL::GC_ERROR status = GenTL::GC_ERR_SUCCESS;
		status = GCWritePort(m_hPort, Address, pBuffer, &iSize);
	}

	virtual GenApi::EAccessMode GetAccessMode(void) const
	{
		return GenApi::RW;
	}

private:
	GenTL::PORT_HANDLE m_hPort = GENTL_INVALID_HANDLE;

	HMODULE m_hModule = nullptr;
};

GenTL::TL_HANDLE m_hTL = GENTL_INVALID_HANDLE;

GenTL::IF_HANDLE m_hIF = GENTL_INVALID_HANDLE;

GenTL::DEV_HANDLE m_hDEV = GENTL_INVALID_HANDLE;

GenTL::DS_HANDLE m_hDS = GENTL_INVALID_HANDLE;

GenTL::EVENT_HANDLE	m_hNewEvent = GENTL_INVALID_HANDLE;

void* m_pImageBuffer[BUFF_COUNT];

CPort* m_pPort = NULL;

IMPLEMENT_DYNAMIC(CDeviceGenICam, CDeviceCamera)

BEGIN_MESSAGE_MAP(CDeviceGenICam, CDeviceCamera)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static LPCTSTR g_lpszParamGenICam[EDeviceParameterGenICam_Count] =
{
	_T("DeviceID"),
	_T("Initialize Mode"),
	_T("Cti File Path"),
	_T("Analog Control"),
	_T("Gain Auto"),
	_T("Gain Selector"),
	_T("Gain Raw"),
	_T("Black Level Selector"),
	_T("Black Level Raw"),
	_T("Balance White Auto"),
	_T("Balance Ratio Selector"),
	_T("Balance Ratio Abs"),
	_T("Balance Ratio Raw"),
	_T("Gamma Enable"),
	_T("Gamma"),
	_T("Digital Shift"),
	_T("Image Format"),
	_T("Pixel Format"),
	_T("ReverseX"),
	_T("ReverseY"),
	_T("Sensor Width"),
	_T("Sensor Height"),
	_T("AOI"),
	_T("Width"),
	_T("Height"),
	_T("OffsetX"),
	_T("OffsetY"),
	_T("Binning Vertical"),
	_T("Binning Horizontal"),
	_T("Legacy Binning Vertical"),
	_T("Acquisition Trigger"),
	_T("Enable Burst Acquisition"),
	_T("Acquisition Mode"),
	_T("Trigger Selector"),
	_T("Trigger Mode"),
	_T("Trigger Source"),
	_T("Trigger Activation"),
	_T("Trigger Delay Abs"),
	_T("Exposure Auto"),
	_T("Exposure Mode"),
	_T("Exposure Time Raw"),
	_T("Exposure Time Abs"),
	_T("Exposure Time Base Abs"),
	_T("Exposure Time Base Abs Enable"),
	_T("Acquisition Line Rate Raw"),
	_T("Acquisition Line Rate Abs"),
	_T("Resulting Line Rate Abs"),
	_T("Acquisition Frame Rate"),
	_T("Acquisition Frame Rate Enable"),
	_T("Resulting Frame Rate Abs"),
	_T("Acquisition Status Selector"),
	_T("Acquisition Status"),
	_T("Digital IO"),
	_T("Line Selector"),
	_T("Line1"),
	_T("Line1 Line Mode"),
	_T("Line1 Line Source"),
	_T("Line1 Line Format"),
	_T("Line1 Line Inverter"),
	_T("Line1 Line Termination"),
	_T("Line1 Line Debouncer Time Raw"),
	_T("Line1 Line Debouncer Time Abs"),
	_T("Line2"),
	_T("Line2 Line Mode"),
	_T("Line2 Line Source"),
	_T("Line2 Line Format"),
	_T("Line2 Line Inverter"),
	_T("Line2 Line Termination"),
	_T("Line2 Line Debouncer Time Raw"),
	_T("Line2 Line Debouncer Time Abs"),
	_T("Out1"),
	_T("Out1 Line Mode"),
	_T("Out1 Line Source"),
	_T("Out1 Line Format"),
	_T("Out1 Line Inverter"),
	_T("Out1 Line Termination"),
	_T("Out1 Line Debouncer Time Raw"),
	_T("Out1 Line Debouncer Time Abs"),
	_T("Out2"),
	_T("Out2 Line Mode"),
	_T("Out2 Line Source"),
	_T("Out2 Line Format"),
	_T("Out2 Line Inverter"),
	_T("Out2 Line Termination"),
	_T("Out2 Line Debouncer Time Raw"),
	_T("Out2 Line Debouncer Time Abs"),
	_T("Out3"),
	_T("Out3 Line Mode"),
	_T("Out3 Line Source"),
	_T("Out3 Line Format"),
	_T("Out3 Line Inverter"),
	_T("Out3 Line Termination"),
	_T("Out3 Line Debouncer Time Raw"),
	_T("Out3 Line Debouncer Time Abs"),
	_T("Out4"),
	_T("Out4 Line Mode"),
	_T("Out4 Line Source"),
	_T("Out4 Line Format"),
	_T("Out4 Line Inverter"),
	_T("Out4 Line Termination"),
	_T("Out4 Line Debouncer Time Raw"),
	_T("Out4 Line Debouncer Time Abs"),
	_T("User Output Selector"),
	_T("Timer Controls"),
	_T("Timer Duration Timebase Abs"),
	_T("Timer Delay Timebase Abs"),
	_T("Timer Selector"),
	_T("Timer Duration Abs"),
	_T("Timer Duration Raw"),
	_T("Timer Delay Abs"),
	_T("Timer Delay Raw"),
	_T("Timer Trigger Source"),
	_T("Timer Trigger Activation"),
	_T("Timer Sequence"),
	_T("Timer Sequence Enable"),
	_T("Timer Sequence Last Entry Index"),
	_T("Timer Sequence Current Entry Index"),
	_T("Timer Sequence Entry Selector"),
	_T("Timer Sequence Timer Selector"),
	_T("Timer Sequence Timer Enable"),
	_T("Timer Sequence Timer Inverter"),
	_T("Timer Sequence Timer Delay Raw"),
	_T("Timer Sequence Timer Duration Raw"),
	_T("Counter Selector"),
	_T("Counter Event Source"),
	_T("Counter Reset Source"),
	_T("LUT Controls"),
	_T("LUT Enable"),
	_T("LUT Index"),
	_T("LUT Value"),
	_T("LUT Selector"),
	_T("Transport Layer"),
	_T("Gev Link Speed"),
	_T("Gev Heartbeat Timeout"),
	_T("Gev SCPSPacket Size"),
};

static LPCTSTR g_lpszGenICamInitMode[EDeviceGenICamInitMode_Count] =
{
	_T("DeviceOnly"),
	_T("Parameter"),
};

static LPCTSTR g_lpszGenICamGainAuto[EDeviceGenICamGainAuto_Count] =
{
	_T("Off"),
	_T("Once"),
	_T("Continuous"),
};

static LPCTSTR g_lpszGenICamGainSelector[EDeviceGenICamGainSelector_Count] =
{
	_T("All"),
	_T("Red"),
	_T("Green"),
	_T("Blue"),
	_T("Y"),
	_T("U"),
	_T("V"),
	_T("Tap1"),
	_T("Tap2"),
	_T("AnalogAll"),
	_T("AnalogRed"),
	_T("AnalogGreen"),
	_T("AnalogBlue"),
	_T("AnalogY"),
	_T("AnalogU"),
	_T("AnalogV"),
	_T("AnalogTap1"),
	_T("AnalogTap2"),
	_T("DigitalAll"),
	_T("DigitalRed"),
	_T("DigitalGreen"),
	_T("DigitalBlue"),
	_T("DigitalY"),
	_T("DigitalU"),
	_T("DigitalV"),
	_T("DigitalTap1"),
	_T("DigitalTap2"),
};

static LPCTSTR g_lpszGenICamBlackLevelSelector[EDeviceGenICamBlackLevelSelector_Count] =
{
	_T("All"),
	_T("Red"),
	_T("Green"),
	_T("Blue"),
	_T("Y"),
	_T("U"),
	_T("V"),
	_T("Tap1"),
	_T("Tap2"),
};

static LPCTSTR g_lpszGenICamBalanceWhiteAuto[EDeviceGenICamBalanceWhiteAuto_Count] =
{
	_T("Off"),
	_T("Once"),
	_T("Continuous"),
};

static LPCTSTR g_lpszGenICamBalanceRatioSelector[EDeviceGenICamBalanceRatioSelector_Count] =
{
	_T("All"),
	_T("Red"),
	_T("Green"),
	_T("Blue"),
	_T("Y"),
	_T("U"),
	_T("V"),
	_T("Tap1"),
	_T("Tap2"),
};

static LPCTSTR g_lpszGenICamGammaEnable[EDeviceGenICamGammaEnable_Count] =
{
	_T("False"),
	_T("True"),
};

static LPCTSTR g_lpszGenICamPixelFormat[EDeviceGenICamPixelFormat_Count] =
{
	_T("Mono8"),
	_T("Mono10"),
	_T("Mono12"),
	_T("Mono14"),
	_T("Mono16"),
	_T("Mono10Packed"),
	_T("Mono12Packed"),
	_T("R8"),
	_T("G8"),
	_T("B8"),
	_T("RGB8"),
	_T("RGBa8"),
	_T("RGB10"),
	_T("RGB12"),
	_T("RGB16"),
	_T("BGR10"),
	_T("BGR12"),
	_T("BGR16"),
	_T("BGR8"),
	_T("BGRa8"),
	_T("YUV422_8"),
	_T("YCbCr411_8"),
	_T("YCbCr422_8"),
	_T("YCbCr601_422_8"),
	_T("YCbCr709_422_8"),
	_T("YCbCr8"),
	_T("BayerBG8"),
	_T("BayerGB8"),
	_T("BayerGR8"),
	_T("BayerRG8"),
	_T("BayerBG10"),
	_T("BayerGB10"),
	_T("BayerGR10"),
	_T("BayerRG10"),
	_T("BayerBG12"),
	_T("BayerGB12"),
	_T("BayerGR12"),
	_T("BayerRG12"),
	_T("BayerBG16"),
	_T("BayerGB16"),
	_T("BayerGR16"),
	_T("BayerRG16"),
	_T("Raw8"),
	_T("Raw16"),
	_T("BayerGR10Packed"),
	_T("BayerRG10Packed"),
	_T("BayerGB10Packed"),
	_T("BayerBG10Packed"),
	_T("BayerGR12Packed"),
	_T("BayerRG12Packed"),
	_T("BayerGB12Packed"),
	_T("BayerBG12Packed"),
	_T("RGB10V1Packed"),
	_T("RGB12V1Packed"),
	_T("RGB8Packed"),
	_T("BGR8Packed"),
	_T("RGBA8Packed"),
	_T("BGRA8Packed"),
	_T("RGB10Packed"),
	_T("BGR10Packed"),
	_T("RGB12Packed"),
	_T("BGR12Packed"),
	_T("RGB16Packed"),
	_T("BGR16Packed"),
	_T("RGB10V2Packed"),
	_T("BGR10V2Packed"),
	_T("RGB565Packed"),
	_T("BGR565Packed"),
	_T("YUV411Packed"),
	_T("YUV422Packed"),
	_T("YUV444Packed"),
	_T("YUYVPacked"),
};

static LPCTSTR g_lpszGenICamLegacyBinningVertical[EDeviceGenICamLegacyBinningVertical_Count] =
{
	_T("Off"),
	_T("Two_Rows"),
};

static LPCTSTR g_lpszGenICamAcquisitionMode[EDeviceGenICamAcquisitionMode_Count] =
{
	_T("Continuous"),
	_T("SingleFrame"),
	_T("MultiFrame"),
};

static LPCTSTR g_lpszGenICamTriggerSelector[EDeviceGenICamTriggerSelector_Count] =
{
	_T("AcquisitionStart"),
	_T("AcquisitionEnd"),
	_T("AcquisitionActive"),
	_T("FrameStart"),
	_T("FrameEnd"),
	_T("FrameActive"),
	_T("FrameBurstStart"),
	_T("FrameBurstEnd"),
	_T("FrameBurstActive"),
	_T("LineStart"),
	_T("ExposureStart"),
	_T("ExposureEnd"),
	_T("ExposureActive"),
	_T("MultiSlopeExposureLimit1"),
};

static LPCTSTR g_lpszGenICamTriggerMode[EDeviceGenICamTriggerMode_Count] =
{
	_T("Off"),
	_T("On"),
};

static LPCTSTR g_lpszGenICamTriggerSource[EDeviceGenICamTriggerSource_Count] =
{
	_T("Line1"),
	_T("Line2"),
	_T("Line3"),
	_T("Line4"),
	_T("Software"),
};

static LPCTSTR g_lpszGenICamTriggerActivation[EDeviceGenICamTriggerActivation_Count] =
{
	_T("RisingEdge"),
	_T("FallingEdge"),
	_T("AnyEdge"),
	_T("LevelHigh"),
	_T("LevelLow"),
};

static LPCTSTR g_lpszGenICamExposureAuto[EDeviceGenICamExposureAuto_Count] =
{
	_T("Off"),
	_T("Once"),
	_T("Continuous"),
};

static LPCTSTR g_lpszGenICamExposureMode[EDeviceGenICamExposureMode_Count] =
{
	_T("Off"),
	_T("Timed"),
	_T("TriggerWidth"),
	_T("TriggerControlled"),
};

static LPCTSTR g_lpszGenICamAcquisitionStatusSelector[EDeviceGenICamAcquisitionStatusSelector_Count] =
{
	_T("AcquisitionTriggerWait"),
	_T("AcquisitionActive"),
	_T("AcquisitionTransfer"),
	_T("FrameTriggerWait"),
	_T("FrameActive"),
	_T("FrameTransfer"),
	_T("ExposureActive"),
};

static LPCTSTR g_lpszGenICamLineSelector[EDeviceGenICamLineSelector_Count] =
{
	_T("Line1"),
	_T("Line2"),
	_T("Out1"),
	_T("Out2"),
	_T("Out3"),
	_T("Out4"),
};

static LPCTSTR g_lpszGenICamLineMode[EDeviceGenICamLineMode_Count] =
{
	_T("Inout"),
	_T("Output"),
};

static LPCTSTR g_lpszGenICamLineSource[EDeviceGenICamLineSource_Count] =
{
	_T("Off"),
	_T("UserOutput0"),
	_T("UserOutput1"),
	_T("UserOutput2"),
	_T("AcquisitionTriggerWait"),
	_T("AcquisitionTrigger"),
	_T("AcquisitionTriggerMissed"),
	_T("AcquisitionActive"),
	_T("FrameTriggerWait"),
	_T("FrameTrigger"),
	_T("FrameTriggerMissed"),
	_T("FrameActive"),
	_T("ExposureActive"),
	_T("LineTriggerWait"),
	_T("LineTrigger"),
	_T("LineTriggerMissed"),
	_T("LineActive"),
	_T("Counter0Active"),
	_T("Counter1Active"),
	_T("Counter2Active"),
	_T("Timer0Active"),
	_T("Timer1Active"),
	_T("Timer2Active"),
	_T("Encoder0"),
	_T("Encoder1"),
	_T("Encoder2"),
	_T("LogicBlock0"),
	_T("LogicBlock1"),
	_T("LogicBlock2"),
	_T("SoftwareSignal0"),
	_T("SoftwareSignal1"),
	_T("SoftwareSignal2"),
	_T("Stream0TransferActive"),
	_T("Stream1TransferActive"),
	_T("Stream2TransferActive"),
	_T("Stream0TransferPaused"),
	_T("Stream1TransferPaused"),
	_T("Stream2TransferPaused"),
	_T("Stream0TransferStopping"),
	_T("Stream1TransferStopping"),
	_T("Stream2TransferStopping"),
	_T("Stream0TransferStopped"),
	_T("Stream1TransferStopped"),
	_T("Stream2TransferStopped"),
	_T("Stream0TransferOverflow"),
	_T("Stream1TransferOverflow"),
	_T("Stream2TransferOverflow"),
};

static LPCTSTR g_lpszGenICamLineFormat[EDeviceGenICamLineFormat_Count] =
{
	_T("NoConnect"),
	_T("TriState"),
	_T("TTL"),
	_T("LVDS"),
	_T("RS422"),
	_T("OptoCoupled"),
	_T("OpenDrain"),
};

static LPCTSTR g_lpszGenICamUserOutputSelector[EDeviceGenICamUserOutputSelector_Count] =
{
	_T("UserOutput1"),
	_T("UserOutput2"),
	_T("UserOutput3"),
	_T("UserOutput4"),
};

static LPCTSTR g_lpszGenICamTimerSelector[EDeviceGenICamTimerSelector_Count] =
{
	_T("Timer1"),
	_T("Timer2"),
	_T("Timer3"),
	_T("Timer4"),
};

static LPCTSTR g_lpszGenICamTimerTriggerSource[EDeviceGenICamTimerTriggerSource_Count] =
{
	_T("Off"),
	_T("AcquisitionTrigger"),
	_T("AcquisitionTriggerMissed"),
	_T("AcquisitionStart"),
	_T("AcquisitionEnd"),
	_T("FrameTrigger"),
	_T("FrameTriggerMissed"),
	_T("FrameStart"),
	_T("FrameEnd"),
	_T("FrameBurstStart"),
	_T("FrameBurstEnd"),
	_T("LineTrigger"),
	_T("LineTriggerMissed"),
	_T("LineStart"),
	_T("LineEnd"),
	_T("ExposureStart"),
	_T("ExposureEnd"),
	_T("Line0"),
	_T("Line1"),
	_T("Line2"),
	_T("UserOutput0"),
	_T("UserOutput1"),
	_T("UserOutput2"),
	_T("Counter0Start"),
	_T("Counter1Start"),
	_T("Counter2Start"),
	_T("Counter0End"),
	_T("Counter1End"),
	_T("Counter2End"),
	_T("Timer0Start"),
	_T("Timer1Start"),
	_T("Timer2Start"),
	_T("Timer0End"),
	_T("Timer1End"),
	_T("Timer2End"),
	_T("Encoder0"),
	_T("Encoder1"),
	_T("Encoder2"),
	_T("LogicBlock0"),
	_T("LogicBlock1"),
	_T("LogicBlock2"),
	_T("SoftwareSignal0"),
	_T("SoftwareSignal1"),
	_T("SoftwareSignal2"),
	_T("Action0"),
	_T("Action1"),
	_T("Action2"),
	_T("LinkTrigger0"),
	_T("LinkTrigger1"),
	_T("LinkTrigger2"),
};

static LPCTSTR g_lpszGenICamTimerTriggerActivation[EDeviceGenICamTimerTriggerActivation_Count] =
{
	_T("RisingEdge"),
	_T("FallingEdge"),
	_T("AnyEdge"),
	_T("LevelHigh"),
	_T("LevelLow"),
};

static LPCTSTR g_lpszGenICamTimerSequenceEntrySelector[EDeviceGenICamTimerSequenceEntrySelector_Count] =
{
	_T("Entry1"),
	_T("Entry2"),
	_T("Entry3"),
	_T("Entry4"),
	_T("Entry5"),
	_T("Entry6"),
	_T("Entry7"),
	_T("Entry8"),
	_T("Entry9"),
	_T("Entry10"),
	_T("Entry11"),
	_T("Entry12"),
	_T("Entry13"),
	_T("Entry14"),
	_T("Entry15"),
	_T("Entry16"),
};

static LPCTSTR g_lpszGenICamTimerSequenceTimerSelector[EDeviceGenICamTimerSequenceTimerSelector_Count] =
{
	_T("Timer1"),
	_T("Timer2"),
	_T("Timer3"),
	_T("Timer4"),
};

static LPCTSTR g_lpszGenICamCounterSelector[EDeviceGenICamCounterSelector_Count] =
{
	_T("Counter1"),
	_T("Counter2"),
};

static LPCTSTR g_lpszGenICamCounterEventSource[EDeviceGenICamCounterEventSource_Count] =
{
	_T("Off"),
	_T("AcquisitionTrigger"),
	_T("AcquisitionTriggerMissed"),
	_T("AcquisitionStart"),
	_T("AcquisitionEnd"),
	_T("FrameTrigger"),
	_T("FrameTriggerMissed"),
	_T("FrameStart"),
	_T("FrameEnd"),
	_T("FrameBurstStart"),
	_T("FrameBurstEnd"),
	_T("LineTrigger"),
	_T("LineTriggerMissed"),
	_T("LineStart"),
	_T("LineEnd"),
	_T("ExposureStart"),
	_T("ExposureEnd"),
	_T("Line0"),
	_T("Line1"),
	_T("Line2"),
	_T("Counter0Start"),
	_T("Counter1Start"),
	_T("Counter2Start"),
	_T("Counter0End"),
	_T("Counter1End"),
	_T("Counter2End"),
	_T("Timer0Start"),
	_T("Timer1Start"),
	_T("Timer2Start"),
	_T("Timer0End"),
	_T("Timer1End"),
	_T("Timer2End"),
	_T("Encoder0"),
	_T("Encoder1"),
	_T("Encoder2"),
	_T("LogicBlock0"),
	_T("LogicBlock1"),
	_T("LogicBlock2"),
	_T("SoftwareSignal0"),
	_T("SoftwareSignal1"),
	_T("SoftwareSignal2"),
	_T("Action0"),
	_T("Action1"),
	_T("Action2"),
	_T("LinkTrigger0"),
	_T("LinkTrigger1"),
	_T("LinkTrigger2"),
	_T("TimestampTick"),
};

static LPCTSTR g_lpszGenICamCounterResetSource[EDeviceGenICamCounterResetSource_Count] =
{
	_T("Off"),
	_T("CounterTrigger"),
	_T("AcquisitionTrigger"),
	_T("AcquisitionTriggerMissed"),
	_T("AcquisitionStart"),
	_T("AcquisitionEnd"),
	_T("FrameTrigger"),
	_T("FrameTriggerMissed"),
	_T("FrameStart"),
	_T("FrameEnd"),
	_T("LineTrigger"),
	_T("LineTriggerMissed"),
	_T("LineStart"),
	_T("LineEnd"),
	_T("ExposureStart"),
	_T("ExposureEnd"),
	_T("Line0"),
	_T("Line1"),
	_T("Line2"),
	_T("UserOutput0"),
	_T("UserOutput1"),
	_T("UserOutput2"),
	_T("Counter0Start"),
	_T("Counter1Start"),
	_T("Counter2Start"),
	_T("Counter0End"),
	_T("Counter1End"),
	_T("Counter2End"),
	_T("Timer0Start"),
	_T("Timer1Start"),
	_T("Timer2Start"),
	_T("Timer0End"),
	_T("Timer1End"),
	_T("Timer2End"),
	_T("Encoder0"),
	_T("Encoder1"),
	_T("Encoder2"),
	_T("LogicBlock0"),
	_T("LogicBlock1"),
	_T("LogicBlock2"),
	_T("SoftwareSignal0"),
	_T("SoftwareSignal1"),
	_T("SoftwareSignal2"),
	_T("Action0"),
	_T("Action1"),
	_T("Action2"),
	_T("LinkTrigger0"),
	_T("LinkTrigger1"),
	_T("LinkTrigger2"),
};

static LPCTSTR g_lpszGenICamLUTSelector[EDeviceGenICamLUTSelector_Count] =
{
	_T("Luminance"),
	_T("Red"),
	_T("Green"),
	_T("Blue"),
};

CDeviceGenICam::CDeviceGenICam()
{
}


CDeviceGenICam::~CDeviceGenICam()
{
	if(m_hModule)
	{
		FreeLibrary(m_hModule);
		m_hModule = nullptr;
	}

	if(m_pNodeMapDevice)
	{
		delete m_pNodeMapDevice;
		m_pNodeMapDevice = nullptr;
	}

	if(m_pPort)
	{
		delete m_pPort;
		m_pPort = nullptr;
	}
}

EDeviceInitializeResult CDeviceGenICam::Initialize()
{
	EDeviceInitializeResult eReturn = EDeviceInitializeResult_UnknownError;

	CString strMessage, strStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("GenICam"));

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

		CString strSerialNumber = GetDeviceID();

		CString strCtiFilePath = _T("");
		GetCtiFilePath(&strCtiFilePath);

		if(m_hModule)
		{
			FreeLibrary(m_hModule);
			m_hModule = nullptr;
		}

		m_hModule = LoadLibrary(strCtiFilePath);

		if(!m_hModule)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntopenthelibrary);
			eReturn = EDeviceInitializeResult_NotFoundLibraries;
			break;
		}

		GenTL::GC_ERROR status = GenTL::GC_ERR_SUCCESS;

		do
		{
			GenTL::PGCInitLib GCInitLib(reinterpret_cast<GenTL::PGCInitLib>(reinterpret_cast<void *>(GetProcAddress(m_hModule, ("GCInitLib")))));

			GenTL::PTLOpen TLOpen(reinterpret_cast<GenTL::PTLOpen>(reinterpret_cast<void *>(GetProcAddress(m_hModule, ("TLOpen")))));

			GenTL::PTLUpdateInterfaceList TLUpdateInterfaceList(reinterpret_cast<GenTL::PTLUpdateInterfaceList>(reinterpret_cast<void *>(GetProcAddress(m_hModule, ("TLUpdateInterfaceList")))));

			GenTL::PTLGetNumInterfaces TLGetNumInterfaces(reinterpret_cast<GenTL::PTLGetNumInterfaces>(reinterpret_cast<void *>(GetProcAddress(m_hModule, ("TLGetNumInterfaces")))));

			GenTL::PTLGetInterfaceID TLGetInterfaceID(reinterpret_cast<GenTL::PTLGetInterfaceID>(reinterpret_cast<void *>(GetProcAddress(m_hModule, ("TLGetInterfaceID")))));

			GenTL::PTLOpenInterface TLOpenInterface(reinterpret_cast<GenTL::PTLOpenInterface>(reinterpret_cast<void *>(GetProcAddress(m_hModule, ("TLOpenInterface")))));

			GenTL::PIFUpdateDeviceList IFUpdateDeviceList(reinterpret_cast<GenTL::PIFUpdateDeviceList>(reinterpret_cast<void *>(GetProcAddress(m_hModule, ("IFUpdateDeviceList")))));

			GenTL::PIFGetNumDevices IFGetNumDevices(reinterpret_cast<GenTL::PIFGetNumDevices>(reinterpret_cast<void *>(GetProcAddress(m_hModule, ("IFGetNumDevices")))));

			GenTL::PIFGetDeviceID IFGetDeviceID(reinterpret_cast<GenTL::PIFGetDeviceID>(reinterpret_cast<void *>(GetProcAddress(m_hModule, ("IFGetDeviceID")))));

			GenTL::PIFOpenDevice IFOpenDevice(reinterpret_cast<GenTL::PIFOpenDevice>(reinterpret_cast<void *>(GetProcAddress(m_hModule, ("IFOpenDevice")))));

			GenTL::PDevGetPort DevGetPort(reinterpret_cast<GenTL::PDevGetPort>(reinterpret_cast<void *>(GetProcAddress(m_hModule, ("DevGetPort")))));

			GenTL::PDevGetNumDataStreams DevGetNumDataStreams(reinterpret_cast<GenTL::PDevGetNumDataStreams>(reinterpret_cast<void *>(GetProcAddress(m_hModule, ("DevGetNumDataStreams")))));

			GenTL::PDevGetDataStreamID DevGetDataStreamID(reinterpret_cast<GenTL::PDevGetDataStreamID>(reinterpret_cast<void *>(GetProcAddress(m_hModule, ("DevGetDataStreamID")))));

			GenTL::PDevOpenDataStream DevOpenDataStream(reinterpret_cast<GenTL::PDevOpenDataStream>(reinterpret_cast<void *>(GetProcAddress(m_hModule, ("DevOpenDataStream")))));

			GenTL::PDSAnnounceBuffer DSAnnounceBuffer(reinterpret_cast<GenTL::PDSAnnounceBuffer>(reinterpret_cast<void *>(GetProcAddress(m_hModule, ("DSAnnounceBuffer")))));

			GenTL::PDSQueueBuffer DSQueueBuffer(reinterpret_cast<GenTL::PDSQueueBuffer>(reinterpret_cast<void *>(GetProcAddress(m_hModule, ("DSQueueBuffer")))));

			GenTL::PGCRegisterEvent GCRegisterEvent(reinterpret_cast<GenTL::PGCRegisterEvent>(reinterpret_cast<void *>(GetProcAddress(m_hModule, ("GCRegisterEvent")))));

			GenTL::PDSStartAcquisition DSStartAcquisition(reinterpret_cast<GenTL::PDSStartAcquisition>(reinterpret_cast<void *>(GetProcAddress(m_hModule, ("DSStartAcquisition")))));

			GenTL::PGCGetNumPortURLs GCGetNumPortURLs(reinterpret_cast<GenTL::PGCGetNumPortURLs>(reinterpret_cast<void *>(GetProcAddress(m_hModule, ("GCGetNumPortURLs")))));

			GenTL::PGCGetPortURLInfo GCGetPortURLInfo(reinterpret_cast<GenTL::PGCGetPortURLInfo>(reinterpret_cast<void *>(GetProcAddress(m_hModule, ("GCGetPortURLInfo")))));

			GenTL::PGCReadPort GCReadPort(reinterpret_cast<GenTL::PGCReadPort>(reinterpret_cast<void *>(GetProcAddress(m_hModule, ("GCReadPort")))));

			GenTL::PIFClose IFClose(reinterpret_cast<GenTL::PIFClose>(reinterpret_cast<void *>(GetProcAddress(m_hModule, ("IFClose")))));

			m_hTL = GENTL_INVALID_HANDLE;

			m_hIF = GENTL_INVALID_HANDLE;

			m_hDEV = GENTL_INVALID_HANDLE;

			m_hDS = GENTL_INVALID_HANDLE;

			m_hNewEvent = GENTL_INVALID_HANDLE;

			status = GCInitLib();

			if(status != GenTL::GC_ERR_SUCCESS)
			{
				strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntopenthelibrary);
				eReturn = EDeviceInitializeResult_NotFoundLibraries;
				break;
			}

			status = TLOpen(&m_hTL);

			if(status != GenTL::GC_ERR_SUCCESS)
			{
				strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntopenthelibrary);
				eReturn = EDeviceInitializeResult_NotFoundLibraries;
				break;
			}

			bool8_t bChanged = false;

			status = TLUpdateInterfaceList(m_hTL, &bChanged, 100);

			if(status != GenTL::GC_ERR_SUCCESS)
			{
				strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntopenthelibrary);
				eReturn = EDeviceInitializeResult_NotFoundLibraries;
				break;
			}

			uint32_t uiInterfaces = 0;

			status = TLGetNumInterfaces(m_hTL, &uiInterfaces);

			if(status != GenTL::GC_ERR_SUCCESS)
			{
				strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntopenthelibrary);
				eReturn = EDeviceInitializeResult_NotFoundLibraries;
				break;
			}

			char szBuf[1024];
			size_t stBufSize = 1024;

			GenTL::IF_HANDLE hItf2 = GENTL_INVALID_HANDLE;

			for(uint32_t i = 0; i < uiInterfaces; ++i)
			{
				char szBufIfaceID[1024];
				size_t stSize = 1024;

				status = TLGetInterfaceID(m_hTL, i, szBufIfaceID, &stSize);

				if(status != GenTL::GC_ERR_SUCCESS)
					continue;

				status = TLOpenInterface(m_hTL, szBufIfaceID, &hItf2);

				if(status != GenTL::GC_ERR_SUCCESS)
				{
					IFClose(hItf2);
					continue;
				}

				bool hasChanged = false;

				status = IFUpdateDeviceList(hItf2, &hasChanged, 500);

				if(status != GenTL::GC_ERR_SUCCESS)
				{
					IFClose(hItf2);
					continue;
				}

				uint32_t uiDevs = 0;

				status = IFGetNumDevices(hItf2, &uiDevs);

				if(status != GenTL::GC_ERR_SUCCESS)
				{
					IFClose(hItf2);
					continue;
				}

				for(uint32_t j = 0; j < uiDevs; ++j)
				{
					status = IFGetDeviceID(hItf2, j, szBuf, &stBufSize);

					if(status != GenTL::GC_ERR_SUCCESS)
						break;

					CString strDeviceID = _T("");
					strDeviceID = szBuf;

					if(strDeviceID.Find(strSerialNumber) != -1)
					{
						m_hIF = hItf2;
						break;
					}
				}

				if(m_hIF != GENTL_INVALID_HANDLE)
					break;

				IFClose(hItf2);
			}

			if(m_hIF == GENTL_INVALID_HANDLE)
			{
				strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtofindthedevice);
				eReturn = EDeviceInitializeResult_NotFoundDeviceError;
				break;
			}

			status = IFOpenDevice(m_hIF, szBuf, GenTL::DEVICE_ACCESS_EXCLUSIVE, &m_hDEV);

			if(status != GenTL::GC_ERR_SUCCESS)
			{
				strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntopenthedevice);
				eReturn = EDeviceInitializeResult_NotOpenedDeviceError;
				break;
			}

			GenTL::PORT_HANDLE hPort = GENTL_INVALID_HANDLE;

			status = DevGetPort(m_hDEV, &hPort);

			if(status != GenTL::GC_ERR_SUCCESS)
			{
				strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntopenthedevice);
				eReturn = EDeviceInitializeResult_NotOpenedDeviceError;
				break;
			}

			bool bIsZipped = false;

			std::vector<char> vctXmlData;

			uint32_t numURLs = 0;

			GenTL::GC_ERROR status = GCGetNumPortURLs(hPort, &numURLs);

			if(status != GenTL::GC_ERR_SUCCESS)
			{
				strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntopenthedevice);
				eReturn = EDeviceInitializeResult_NotOpenedDeviceError;
				break;
			}

			char szBufUrl[2048];
			size_t stUrlBufSize = 1024;
			const uint32_t uiIdxUrl = 0;
			GenTL::INFO_DATATYPE dataType = GenTL::INFO_DATATYPE_UNKNOWN;

			status = GCGetPortURLInfo(hPort, uiIdxUrl, GenTL::URL_INFO_URL, &dataType, szBufUrl, &stUrlBufSize);

			if(status != GenTL::GC_ERR_SUCCESS)
			{
				strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntopenthedevice);
				eReturn = EDeviceInitializeResult_NotOpenedDeviceError;
				break;
			}

			std::string strXMLAddress(szBufUrl);
			std::string strLocation = strXMLAddress.substr(0, 6);
			std::transform(strLocation.begin(), strLocation.end(), strLocation.begin(), (int(*)(int)) toupper);

			if(strLocation == "LOCAL:")
			{
				std::string strFullFilename;
				uint64_t uiAddr = 0;
				size_t szXmlSize = 0;

				if(!SplitLocalUrl(strXMLAddress.substr(6), strFullFilename, uiAddr, szXmlSize))
					break;

				vctXmlData.resize(szXmlSize + 1);

				status = GCReadPort(hPort, uiAddr, &vctXmlData[0], &szXmlSize);

				if(status != GenTL::GC_ERR_SUCCESS)
					break;

				vctXmlData[szXmlSize] = '\0';
			}

			bIsZipped = strXMLAddress.find(".zip") != std::string::npos;

			if(!vctXmlData.size())
			{
				strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntopenthedevice);
				eReturn = EDeviceInitializeResult_NotOpenedDeviceError;
				break;
			}

			if(m_pNodeMapDevice)
			{
				delete m_pNodeMapDevice;
				m_pNodeMapDevice = nullptr;
			}

			if(!m_pNodeMapDevice)
				m_pNodeMapDevice = new GENAPI_NAMESPACE::CNodeMapRef;

			if(bIsZipped)
			{
				m_pNodeMapDevice->_LoadXMLFromZIPData(&vctXmlData[0], vctXmlData.size());
			}
			else
			{
				m_pNodeMapDevice->_LoadXMLFromString(GENICAM_NAMESPACE::gcstring((const char*)&vctXmlData[0], strlen(&vctXmlData[0])));
			}

			if(m_pPort)
			{
				delete m_pPort;
				m_pPort = nullptr;
			}

			if(!m_pPort)
				m_pPort = new CPort(hPort, m_hModule);

			m_pNodeMapDevice->_Connect(m_pPort);

			uint32_t uiNumStreams = 0;

			status = DevGetNumDataStreams(m_hDEV, &uiNumStreams);

			if(status != GenTL::GC_ERR_SUCCESS)
			{
				strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntopenthedevice);
				eReturn = EDeviceInitializeResult_NotOpenedDeviceError;
				break;
			}

			const uint32_t uiIdxDataStream = 0;
			std::vector<char> vctBufID(512);
			size_t szBufIDSize = vctBufID.size();

			status = DevGetDataStreamID(m_hDEV, uiIdxDataStream, &vctBufID[0], &szBufIDSize);

			if(status != GenTL::GC_ERR_SUCCESS)
			{
				strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntopenthedevice);
				eReturn = EDeviceInitializeResult_NotOpenedDeviceError;
				break;
			}

			status = DevOpenDataStream(m_hDEV, &vctBufID[0], &m_hDS);

			if(status != GenTL::GC_ERR_SUCCESS)
			{
				strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntopenthedevice);
				eReturn = EDeviceInitializeResult_NotOpenedDeviceError;
				break;
			}

			EDeviceGenICamInitMode eInitType = EDeviceGenICamInitMode_DeviceOnly;

			if(GetInitializeMode(&eInitType))
				break;

			int nHeight = 0;
			int nWidth = 0;
			CString strFormat = _T("");

			bool bParamOK = false;

			switch(eInitType)
			{
			case EDeviceGenICamInitMode_DeviceOnly:
				{
					UpdateDeviceToParameter();

					int nParam = 0;
					if(GetWidth(&nParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Width"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
					nWidth = nParam;

					nParam = 0;
					if(GetHeight(&nParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Height"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
					nHeight = nParam;

					EDeviceGenICamPixelFormat ePixelFormat;

					if(GetPixelFormat(&ePixelFormat))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("PixelFormat"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(ePixelFormat >= EDeviceGenICamPixelFormat_R8)
					{
						strMessage.Format(_T("Not support PixelFormat"));
						eReturn = EDeviceInitializeResult_CanNotApplyPixelFormat;
						break;
					}

					if(ePixelFormat < 0 || ePixelFormat >= EDeviceGenICamPixelFormat_Count)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("PixelFormat"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					strFormat = g_lpszGenICamPixelFormat[ePixelFormat];

					bParamOK = true;
				}
				break;
			case EDeviceGenICamInitMode_Parameter:
				{
					GenApi::CIntegerPtr ptrOffsetX = m_pNodeMapDevice->_GetNode("OffsetX");

					if(ptrOffsetX.IsValid() && GenApi::IsWritable(ptrOffsetX))
						ptrOffsetX->SetValue(0);

					GenApi::CIntegerPtr ptrOffsetY = m_pNodeMapDevice->_GetNode("OffsetY");

					if(ptrOffsetY.IsValid() && GenApi::IsWritable(ptrOffsetY))
						ptrOffsetY->SetValue(0);

					int nWidthMax = INT_MAX;
					bool bWidthMax = false;

					GenApi::CIntegerPtr ptrWidthMax = m_pNodeMapDevice->_GetNode("WidthMax");

					if(ptrWidthMax.IsValid() && GenApi::IsReadable(ptrWidthMax))
						nWidthMax = ptrWidthMax->GetValue();

					nWidth = 0;

					if(GetWidth(&nWidth))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Width"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(nWidth > nWidthMax)
					{
						nWidth = nWidthMax;
						SetWidth(nWidth);
						bWidthMax = true;
					}

					GenApi::CIntegerPtr ptrWidth = m_pNodeMapDevice->_GetNode("Width");

					if(ptrWidth.IsValid() && GenApi::IsWritable(ptrWidth))
					{
						try
						{
							ptrWidth->SetValue(nWidth);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Width"));
							break;
						}
					}

					int nHeightMax = INT_MAX;
					bool bHeightMax = false;

					GenApi::CIntegerPtr ptrHeightMax = m_pNodeMapDevice->_GetNode("HeightMax");

					if(ptrHeightMax.IsValid() && GenApi::IsReadable(ptrHeightMax))
						nHeightMax = ptrHeightMax->GetValue();

					nHeight = 0;

					if(GetHeight(&nHeight))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Height"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(nHeight > nHeightMax)
					{
						nHeight = nHeightMax;
						SetHeight(nHeight);
						bHeightMax = true;
					}

					GenApi::CIntegerPtr ptrHeight = m_pNodeMapDevice->_GetNode("Height");

					if(ptrHeight.IsValid() && GenApi::IsWritable(ptrHeight))
					{
						try
						{
							ptrHeight->SetValue(nHeight);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Height"));
							break;
						}
					}

					int nOffsetX = 0;

					if(GetOffsetX(&nOffsetX))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("OffSetX"));
						eReturn = EDeviceInitializeResult_CanNotReadDBOffsetX;
						break;
					}

					if(bWidthMax)
						nOffsetX = 0;

					if(ptrOffsetX.IsValid() && GenApi::IsWritable(ptrOffsetX))
					{
						try
						{
							ptrOffsetX->SetValue(nOffsetX);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("OffsetX"));
							break;
						}
					}

					int nOffsetY = 0;

					if(GetOffsetY(&nOffsetY))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("OffsetY"));
						eReturn = EDeviceInitializeResult_CanNotReadDBOffsetY;
						break;
					}

					if(bWidthMax)
						nOffsetY = 0;

					if(ptrOffsetY.IsValid() && GenApi::IsWritable(ptrOffsetY))
					{
						try
						{
							ptrOffsetY->SetValue(nOffsetY);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("OffsetY"));
							break;
						}
					}

					EDeviceGenICamGainAuto eGainAuto;

					if(GetGainAuto(&eGainAuto))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("GainAuto"));
						eReturn = EDeviceInitializeResult_CanNotReadDBGainAuto;
						break;
					}

					if(eGainAuto < 0 || eGainAuto >= EDeviceGenICamGainAuto_Count)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("GainAuto"));
						eReturn = EDeviceInitializeResult_CanNotReadDBGainAuto;
						break;
					}

					CStringA strGainAuto(g_lpszGenICamGainAuto[eGainAuto]);

					GenApi::CEnumerationPtr ptrGainAuto = m_pNodeMapDevice->_GetNode("GainAuto");

					if(ptrGainAuto.IsValid() && GenApi::IsWritable(ptrGainAuto))
					{
						try
						{
							GENICAM_NAMESPACE::gcstring gcstr(strGainAuto);

							ptrGainAuto->FromString(gcstr);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("GainAuto"));
							break;
						}
					}

					int nGainRaw = 0;

					if(GetGainRaw(&nGainRaw))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("GainRaw"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					GenApi::CIntegerPtr ptrGainRaw = m_pNodeMapDevice->_GetNode("GainRaw");

					if(ptrGainRaw.IsValid() && GenApi::IsWritable(ptrGainRaw))
					{
						try
						{
							ptrGainRaw->SetValue(nGainRaw);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("GainRaw"));
							break;
						}
					}

					EDeviceGenICamGainSelector eGainSelector;

					if(GetGainSelector(&eGainSelector))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("GainSelector"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(eGainSelector < 0 || eGainSelector >= EDeviceGenICamGainSelector_Count)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("GainSelector"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					CStringA strGainSelector(g_lpszGenICamGainSelector[eGainSelector]);

					GenApi::CEnumerationPtr ptrGainSelector = m_pNodeMapDevice->_GetNode("GainSelector");

					if(ptrGainSelector.IsValid() && GenApi::IsWritable(ptrGainSelector))
					{
						try
						{
							GENICAM_NAMESPACE::gcstring gcstr(strGainSelector);

							ptrGainSelector->FromString(gcstr);

						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("GainSelector"));
							break;
						}
					}

					int nBlackLevelRaw = 0;

					if(GetBlackLevelRaw(&nBlackLevelRaw))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("BlackLevelRaw"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					GenApi::CIntegerPtr ptrBlackLevelRaw = m_pNodeMapDevice->_GetNode("BlackLevelRaw");

					if(ptrBlackLevelRaw.IsValid() && GenApi::IsWritable(ptrBlackLevelRaw))
					{
						try
						{
							ptrBlackLevelRaw->SetValue(nBlackLevelRaw);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("BlackLevelRaw"));
							break;
						}
					}

					EDeviceGenICamBlackLevelSelector eBlackLevelSelector;

					if(GetBlackLevelSelector(&eBlackLevelSelector))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("BlackLevelSelector"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(eBlackLevelSelector < 0 || eBlackLevelSelector >= EDeviceGenICamBlackLevelSelector_Count)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("BlackLevelSelector"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					CStringA strBlackLevelSelector(g_lpszGenICamBlackLevelSelector[eBlackLevelSelector]);

					GenApi::CEnumerationPtr ptrBlackLevelSelector = m_pNodeMapDevice->_GetNode("BlackLevelSelector");

					if(ptrBlackLevelSelector.IsValid() && GenApi::IsWritable(ptrBlackLevelSelector))
					{
						try
						{
							GENICAM_NAMESPACE::gcstring gcstr(strBlackLevelSelector);

							ptrBlackLevelSelector->FromString(gcstr);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("BlackLevelSelector"));
							break;
						}
					}

					EDeviceGenICamBalanceWhiteAuto eBalanceWhiteAuto;

					if(GetBalanceWhiteAuto(&eBalanceWhiteAuto))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("BalanceWhiteAuto"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(eBalanceWhiteAuto < 0 || eBalanceWhiteAuto >= EDeviceGenICamBalanceWhiteAuto_Count)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("BalanceWhiteAuto"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					CStringA strBalanceWhiteAuto(g_lpszGenICamBalanceWhiteAuto[eBalanceWhiteAuto]);

					GenApi::CEnumerationPtr ptrBalanceWhiteAuto = m_pNodeMapDevice->_GetNode("BalanceWhiteAuto");

					if(ptrBalanceWhiteAuto.IsValid() && GenApi::IsWritable(ptrBalanceWhiteAuto))
					{
						try
						{
							GENICAM_NAMESPACE::gcstring gcstr(strBalanceWhiteAuto);

							ptrBalanceWhiteAuto->FromString(gcstr);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("BalanceWhiteAuto"));
							break;
						}
					}

					EDeviceGenICamBalanceRatioSelector eBalanceRatioSelector;

					if(GetBalanceRatioSelector(&eBalanceRatioSelector))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("BalanceRatioSelector"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(eBalanceRatioSelector < 0 || eBalanceRatioSelector >= EDeviceGenICamBalanceRatioSelector_Count)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("BalanceRatioSelector"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					CStringA strBalanceRatioSelector(g_lpszGenICamBalanceRatioSelector[eBalanceRatioSelector]);

					GenApi::CEnumerationPtr ptrBalanceRatioSelector = m_pNodeMapDevice->_GetNode("BalanceRatioSelector");

					if(ptrBalanceRatioSelector.IsValid() && GenApi::IsWritable(ptrBalanceRatioSelector))
					{
						try
						{
							GENICAM_NAMESPACE::gcstring gcstr(strBalanceRatioSelector);

							ptrBalanceRatioSelector->FromString(gcstr);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("BalanceRatioSelector"));
							break;
						}
					}

					float fBalanceRatioAbs = 0.;

					if(GetBalanceRatioAbs(&fBalanceRatioAbs))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("BalanceRatioAbs"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					GenApi::CFloatPtr ptrBalanceRatioAbs = m_pNodeMapDevice->_GetNode("BalanceRatioAbs");

					if(ptrBalanceRatioAbs.IsValid() && GenApi::IsWritable(ptrBalanceRatioAbs))
					{
						try
						{
							ptrBalanceRatioAbs->SetValue(fBalanceRatioAbs);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("BalanceRatioAbs"));
							break;
						}
					}

					int nBalanceRatioRaw = 0;

					if(GetBalanceRatioRaw(&nBalanceRatioRaw))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("BalanceRatioRaw"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					GenApi::CIntegerPtr ptrBalanceRatioRaw = m_pNodeMapDevice->_GetNode("BalanceRatioRaw");

					if(ptrBalanceRatioRaw.IsValid() && GenApi::IsWritable(ptrBalanceRatioRaw))
					{
						try
						{
							ptrBalanceRatioRaw->SetValue(nBalanceRatioRaw);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("BalanceRatioRaw"));
							break;
						}
					}

					bool bGammaEnable = 0;

					if(GetGammaEnable(&bGammaEnable))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("GammaEnable"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					GenApi::CBooleanPtr ptrGammaEnable = m_pNodeMapDevice->_GetNode("GammaEnable");

					if(ptrGammaEnable.IsValid() && GenApi::IsWritable(ptrGammaEnable))
						ptrGammaEnable->SetValue(bGammaEnable);

					float fGamma = 0.;

					if(GetGamma(&fGamma))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Gamma"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					GenApi::CFloatPtr ptrGamma = m_pNodeMapDevice->_GetNode("Gamma");

					if(ptrGamma.IsValid() && GenApi::IsWritable(ptrGamma))
					{
						try
						{
							ptrGamma->SetValue(fGamma);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Gamma"));
							break;
						}
					}

					int nDigitalShift = 0;

					if(GetDigitalShift(&nDigitalShift))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("DigitalShift"));
						eReturn = EDeviceInitializeResult_CanNotReadDBDigitalShift;
						break;
					}

					GenApi::CIntegerPtr ptrDigitalShift = m_pNodeMapDevice->_GetNode("DigitalShift");

					if(ptrDigitalShift.IsValid() && GenApi::IsWritable(ptrDigitalShift))
					{
						try
						{
							ptrDigitalShift->SetValue(nDigitalShift);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("DigitalShift"));
							break;
						}
					}

					EDeviceGenICamPixelFormat ePixelFormat;

					if(GetPixelFormat(&ePixelFormat))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("PixelFormat"));
						eReturn = EDeviceInitializeResult_CanNotReadDBPixelFormat;
						break;
					}

					if(ePixelFormat >= EDeviceGenICamPixelFormat_R8)
					{
						strMessage.Format(_T("Not support PixelFormat"));
						eReturn = EDeviceInitializeResult_CanNotApplyPixelFormat;
						break;
					}

					if(ePixelFormat < 0 || ePixelFormat >= EDeviceGenICamPixelFormat_Count)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("PixelFormat"));
						eReturn = EDeviceInitializeResult_CanNotReadDBPixelFormat;
						break;
					}

					CStringA strPixelFormat(g_lpszGenICamPixelFormat[ePixelFormat]);

					GenApi::CEnumerationPtr ptrPixelFormat = m_pNodeMapDevice->_GetNode("PixelFormat");

					if(ptrPixelFormat.IsValid() && GenApi::IsWritable(ptrPixelFormat))
					{
						try
						{
							GENICAM_NAMESPACE::gcstring gcstr(strPixelFormat);

							ptrPixelFormat->FromString(gcstr);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("PixelFormat"));
							break;
						}
					}

					strFormat = strPixelFormat;

					bool bReverseX = 0;

					if(GetReverseX(&bReverseX))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ReverseX"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					GenApi::CBooleanPtr ptrReverseX = m_pNodeMapDevice->_GetNode("ReverseX");

					if(ptrReverseX.IsValid() && GenApi::IsWritable(ptrReverseX))
						ptrReverseX->SetValue(bReverseX);

					bool bReverseY = 0;

					if(GetReverseY(&bReverseY))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ReverseY"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					GenApi::CBooleanPtr ptrReverseY = m_pNodeMapDevice->_GetNode("ReverseY");

					if(ptrReverseY.IsValid() && GenApi::IsWritable(ptrReverseY))
						ptrReverseY->SetValue(bReverseY);

					int nSensorWidth = 0;

					if(GetSensorWidth(&nSensorWidth))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("SensorWidth"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					GenApi::CIntegerPtr ptrSensorWidth = m_pNodeMapDevice->_GetNode("SensorWidth");

					if(ptrSensorWidth.IsValid() && GenApi::IsWritable(ptrSensorWidth))
					{
						try
						{
							ptrSensorWidth->SetValue(nSensorWidth);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("SensorWidth"));
							break;

						}
					}

					int nBinningVertical = 0;

					if(GetBinningVertical(&nBinningVertical))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("BinningVertical"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					GenApi::CIntegerPtr ptrBinningVertical = m_pNodeMapDevice->_GetNode("BinningVertical");

					if(ptrBinningVertical.IsValid() && GenApi::IsWritable(ptrBinningVertical))
					{
						try
						{
							ptrBinningVertical->SetValue(nBinningVertical);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("BinningVertical"));
							break;
						}
					}

					int nBinningHorizontal = 0;

					if(GetBinningHorizontal(&nBinningHorizontal))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("BinningHorizontal"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					GenApi::CIntegerPtr ptrBinningHorizontal = m_pNodeMapDevice->_GetNode("BinningHorizontal");

					if(ptrBinningHorizontal.IsValid() && GenApi::IsWritable(ptrBinningHorizontal))
					{
						try
						{
							ptrBinningHorizontal->SetValue(nBinningHorizontal);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("BinningHorizontal"));
							break;
						}
					}

					int nSensorHeight = 0;

					if(GetSensorHeight(&nSensorHeight))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("SensorHeight"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					GenApi::CIntegerPtr ptrSensorHeight = m_pNodeMapDevice->_GetNode("SensorHeight");

					if(ptrSensorHeight.IsValid() && GenApi::IsWritable(ptrSensorHeight))
					{
						try
						{
							ptrSensorHeight->SetValue(nSensorHeight);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("SensorHeight"));
							break;
						}
					}

					EDeviceGenICamLegacyBinningVertical eLegacyBinningVertical;

					if(GetLegacyBinningVertical(&eLegacyBinningVertical))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LegacyBinningVertical"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(eLegacyBinningVertical < 0 || eLegacyBinningVertical >= EDeviceGenICamLegacyBinningVertical_Count)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LegacyBinningVertical"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					CStringA strLegacyBinningVertical(g_lpszGenICamLegacyBinningVertical[eLegacyBinningVertical]);

					GenApi::CEnumerationPtr ptrLegacyBinningVertical = m_pNodeMapDevice->_GetNode("LegacyBinningVertical");

					if(ptrLegacyBinningVertical.IsValid() && GenApi::IsWritable(ptrLegacyBinningVertical))
					{
						try
						{
							GENICAM_NAMESPACE::gcstring gcstr(strLegacyBinningVertical);

							ptrLegacyBinningVertical->FromString(gcstr);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LegacyBinningVertical"));
							break;
						}
					}

					bool bEnableBurstAcquisition = 0;

					if(GetEnableBurstAcquisition(&bEnableBurstAcquisition))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("EnableBurstAcquisition"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					GenApi::CBooleanPtr ptrEnableBurstAcquisition = m_pNodeMapDevice->_GetNode("EnableBurstAcquisition");

					if(ptrEnableBurstAcquisition.IsValid() && GenApi::IsWritable(ptrEnableBurstAcquisition))
						ptrEnableBurstAcquisition->SetValue(bEnableBurstAcquisition);

					EDeviceGenICamAcquisitionMode eAcquisitionMode;

					if(GetAcquisitionMode(&eAcquisitionMode))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AcquisitionMode"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(eAcquisitionMode < 0 || eAcquisitionMode >= EDeviceGenICamAcquisitionMode_Count)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AcquisitionMode"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					CStringA strAcquisitionMode(g_lpszGenICamAcquisitionMode[eAcquisitionMode]);

					GenApi::CEnumerationPtr ptrAcquisitionMode = m_pNodeMapDevice->_GetNode("AcquisitionMode");

					if(ptrAcquisitionMode.IsValid() && GenApi::IsWritable(ptrAcquisitionMode))
					{
						try
						{
							GENICAM_NAMESPACE::gcstring gcstr(strAcquisitionMode);

							ptrAcquisitionMode->FromString(gcstr);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("AcquisitionMode"));
							break;
						}
					}

					EDeviceGenICamTriggerSelector eTriggerSelector;

					if(GetTriggerSelector(&eTriggerSelector))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TriggerSelector"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(eTriggerSelector < 0 || eTriggerSelector >= EDeviceGenICamTriggerSelector_Count)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TriggerSelector"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					CStringA strTriggerSelector(g_lpszGenICamTriggerSelector[eTriggerSelector]);

					GenApi::CEnumerationPtr ptrTriggerSelector = m_pNodeMapDevice->_GetNode("TriggerSelector");

					if(ptrTriggerSelector.IsValid() && GenApi::IsWritable(ptrTriggerSelector))
					{
						try
						{
							GENICAM_NAMESPACE::gcstring gcstr(strTriggerSelector);

							ptrTriggerSelector->FromString(gcstr);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TriggerSelector"));
							break;
						}
					}

					EDeviceGenICamTriggerMode eTriggerMode;

					if(GetTriggerMode(&eTriggerMode))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TriggerMode"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(eTriggerMode < 0 || eTriggerMode >= EDeviceGenICamTriggerMode_Count)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TriggerMode"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					CStringA strTriggerMode(g_lpszGenICamTriggerMode[eTriggerMode]);

					GenApi::CEnumerationPtr ptrTriggerMode = m_pNodeMapDevice->_GetNode("TriggerMode");

					if(ptrTriggerMode.IsValid() && GenApi::IsWritable(ptrTriggerMode))
					{
						try
						{
							GENICAM_NAMESPACE::gcstring gcstr(strTriggerMode);

							ptrTriggerMode->FromString(gcstr);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TriggerMode"));
							break;
						}
					}

					EDeviceGenICamTriggerSource eTriggerSource;

					if(GetTriggerSource(&eTriggerSource))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TriggerSource"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(eTriggerSource < 0 || eTriggerSource >= EDeviceGenICamTriggerSource_Count)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TriggerSource"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					CStringA strTriggerSource(g_lpszGenICamTriggerSource[eTriggerSource]);

					GenApi::CEnumerationPtr ptrTriggerSource = m_pNodeMapDevice->_GetNode("TriggerSource");

					if(ptrTriggerSource.IsValid() && GenApi::IsWritable(ptrTriggerSource))
					{
						try
						{
							GENICAM_NAMESPACE::gcstring gcstr(strTriggerSource);

							ptrTriggerSource->FromString(gcstr);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TriggerSource"));
							break;
						}
					}

					EDeviceGenICamTriggerActivation eTriggerActivation;

					if(GetTriggerActivation(&eTriggerActivation))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TriggerActivation"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(eTriggerActivation < 0 || eTriggerActivation >= EDeviceGenICamTriggerActivation_Count)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TriggerActivation"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					CStringA strTriggerActivation(g_lpszGenICamTriggerActivation[eTriggerActivation]);

					GenApi::CEnumerationPtr ptrTriggerActivation = m_pNodeMapDevice->_GetNode("TriggerActivation");

					if(ptrTriggerActivation.IsValid() && GenApi::IsWritable(ptrTriggerActivation))
					{
						try
						{
							GENICAM_NAMESPACE::gcstring gcstr(strTriggerActivation);

							ptrTriggerActivation->FromString(gcstr);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TriggerActivation"));
							break;
						}
					}

					float fTriggerDelayAbs = 0.;

					if(GetTriggerDelayAbs(&fTriggerDelayAbs))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TriggerDelayAbs"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					GenApi::CFloatPtr ptrTriggerDelayAbs = m_pNodeMapDevice->_GetNode("TriggerDelayAbs");

					if(ptrTriggerDelayAbs.IsValid() && GenApi::IsWritable(ptrTriggerDelayAbs))
					{
						try
						{
							ptrTriggerDelayAbs->SetValue(fTriggerDelayAbs);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TriggerDelayAbs"));
							break;
						}
					}

					EDeviceGenICamExposureAuto eExposureAuto;

					if(GetExposureAuto(&eExposureAuto))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ExposureAuto"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(eExposureAuto < 0 || eExposureAuto >= EDeviceGenICamExposureAuto_Count)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ExposureAuto"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					CStringA strExposureAuto(g_lpszGenICamExposureAuto[eExposureAuto]);

					GenApi::CEnumerationPtr ptrExposureAuto = m_pNodeMapDevice->_GetNode("ExposureAuto");

					if(ptrExposureAuto.IsValid() && GenApi::IsWritable(ptrExposureAuto))
					{
						try
						{
							GENICAM_NAMESPACE::gcstring gcstr(strExposureAuto);

							ptrExposureAuto->FromString(gcstr);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ExposureAuto"));
							break;
						}
					}

					EDeviceGenICamExposureMode eExposureMode;

					if(GetExposureMode(&eExposureMode))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ExposureMode"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(eExposureMode < 0 || eExposureMode >= EDeviceGenICamExposureMode_Count)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ExposureMode"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					CStringA strExposureMode(g_lpszGenICamExposureMode[eExposureMode]);

					GenApi::CEnumerationPtr ptrExposureMode = m_pNodeMapDevice->_GetNode("ExposureMode");

					if(ptrExposureMode.IsValid() && GenApi::IsWritable(ptrExposureMode))
					{
						try
						{
							GENICAM_NAMESPACE::gcstring gcstr(strExposureMode);

							ptrExposureMode->FromString(gcstr);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ExposureMode"));
							break;
						}
					}

					int nExposureTimeRaw = 0;

					if(GetExposureTimeRaw(&nExposureTimeRaw))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ExposureTimeRaw"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					GenApi::CIntegerPtr ptrExposureTimeRaw = m_pNodeMapDevice->_GetNode("ExposureTimeRaw");

					if(ptrExposureTimeRaw.IsValid() && GenApi::IsWritable(ptrExposureTimeRaw))
					{
						try
						{
							ptrExposureTimeRaw->SetValue(nExposureTimeRaw);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ExposureTimeRaw"));
							break;
						}
					}

					float fExposureTimeAbs = 0.;

					if(GetExposureTimeAbs(&fExposureTimeAbs))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ExposureTimeAbs"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					GenApi::CFloatPtr ptrExposureTimeAbs = m_pNodeMapDevice->_GetNode("ExposureTimeAbs");

					if(ptrExposureTimeAbs.IsValid() && GenApi::IsWritable(ptrExposureTimeAbs))
					{
						try
						{
							ptrExposureTimeAbs->SetValue(fExposureTimeAbs);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ExposureTimeAbs"));
							break;
						}
					}

					float fExposureTimeBaseAbs = 0.;

					if(GetExposureTimeBaseAbs(&fExposureTimeBaseAbs))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ExposureTimeBaseAbs"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					GenApi::CFloatPtr ptrExposureTimeBaseAbs = m_pNodeMapDevice->_GetNode("ExposureTimeBaseAbs");

					if(ptrExposureTimeBaseAbs.IsValid() && GenApi::IsWritable(ptrExposureTimeBaseAbs))
					{
						try
						{
							ptrExposureTimeBaseAbs->SetValue(fExposureTimeBaseAbs);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ExposureTimeBaseAbs"));
							break;
						}
					}

					bool bExposureTimeBaseAbsEnable = 0;

					if(GetExposureTimeBaseAbsEnable(&bExposureTimeBaseAbsEnable))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ExposureTimeBaseAbsEnable"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					GenApi::CBooleanPtr ptrExposureTimeBaseAbsEnable = m_pNodeMapDevice->_GetNode("ExposureTimeBaseAbsEnable");

					if(ptrExposureTimeBaseAbsEnable.IsValid() && GenApi::IsWritable(ptrExposureTimeBaseAbsEnable))
						ptrExposureTimeBaseAbsEnable->SetValue(bExposureTimeBaseAbsEnable);

					int nAcquisitionLineRateRaw = 0;

					if(GetAcquisitionLineRateRaw(&nAcquisitionLineRateRaw))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AcquisitionLineRateRaw"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					GenApi::CIntegerPtr ptrAcquisitionLineRateRaw = m_pNodeMapDevice->_GetNode("AcquisitionLineRateRaw");

					if(ptrAcquisitionLineRateRaw.IsValid() && GenApi::IsWritable(ptrAcquisitionLineRateRaw))
					{
						try
						{
							ptrAcquisitionLineRateRaw->SetValue(nAcquisitionLineRateRaw);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("AcquisitionLineRateRaw"));
							break;
						}
					}

					float fAcquisitionLineRateAbs = 0.;

					if(GetAcquisitionLineRateAbs(&fAcquisitionLineRateAbs))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AcquisitionLineRateAbs"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					GenApi::CFloatPtr ptrAcquisitionLineRateAbs = m_pNodeMapDevice->_GetNode("AcquisitionLineRateAbs");

					if(ptrAcquisitionLineRateAbs.IsValid() && GenApi::IsWritable(ptrAcquisitionLineRateAbs))
					{
						try
						{
							ptrAcquisitionLineRateAbs->SetValue(fAcquisitionLineRateAbs);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("AcquisitionLineRateAbs"));
							break;
						}
					}

					float fResultingLineRateAbs = 0.;

					if(GetResultingLineRateAbs(&fResultingLineRateAbs))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ResultingLineRateAbs"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					GenApi::CFloatPtr ptrResultingLineRateAbs = m_pNodeMapDevice->_GetNode("ResultingLineRateAbs");

					if(ptrResultingLineRateAbs.IsValid() && GenApi::IsWritable(ptrResultingLineRateAbs))
					{
						try
						{
							ptrResultingLineRateAbs->SetValue(fResultingLineRateAbs);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ResultingLineRateAbs"));
							break;
						}
					}

					float fAcquisitionFrameRate = 0.;

					if(GetAcquisitionFrameRate(&fAcquisitionFrameRate))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AcquisitionFrameRate"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					GenApi::CFloatPtr ptrAcquisitionFrameRate = m_pNodeMapDevice->_GetNode("AcquisitionFrameRate");

					if(ptrAcquisitionFrameRate.IsValid() && GenApi::IsWritable(ptrAcquisitionFrameRate))
					{
						try
						{
							ptrAcquisitionFrameRate->SetValue(fAcquisitionFrameRate);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("AcquisitionFrameRate"));
							break;
						}
					}

					bool bAcquisitionFrameRateEnable = 0;

					if(GetAcquisitionFrameRateEnable(&bAcquisitionFrameRateEnable))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AcquisitionFrameRateEnable"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					GenApi::CBooleanPtr ptrAcquisitionFrameRateEnable = m_pNodeMapDevice->_GetNode("AcquisitionFrameRateEnable");

					if(ptrAcquisitionFrameRateEnable.IsValid() && GenApi::IsWritable(ptrAcquisitionFrameRateEnable))
						ptrAcquisitionFrameRateEnable->SetValue(bAcquisitionFrameRateEnable);

					float fResultingFrameRateAbs = 0.;

					if(GetResultingFrameRateAbs(&fResultingFrameRateAbs))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ResultingFrameRateAbs"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					GenApi::CFloatPtr ptrResultingFrameRateAbs = m_pNodeMapDevice->_GetNode("ResultingFrameRateAbs");

					if(ptrResultingFrameRateAbs.IsValid() && GenApi::IsWritable(ptrResultingFrameRateAbs))
					{
						try
						{
							ptrResultingFrameRateAbs->SetValue(fResultingFrameRateAbs);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ResultingFrameRateAbs"));
							break;
						}
					}

					EDeviceGenICamAcquisitionStatusSelector eAcquisitionStatusSelector;

					if(GetAcquisitionStatusSelector(&eAcquisitionStatusSelector))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AcquisitionStatusSelector"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(eAcquisitionStatusSelector < 0 || eAcquisitionStatusSelector >= EDeviceGenICamAcquisitionStatusSelector_Count)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AcquisitionStatusSelector"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					CStringA strAcquisitionStatusSelector(g_lpszGenICamAcquisitionStatusSelector[eAcquisitionStatusSelector]);

					GenApi::CEnumerationPtr ptrAcquisitionStatusSelector = m_pNodeMapDevice->_GetNode("AcquisitionStatusSelector");

					if(ptrAcquisitionStatusSelector.IsValid() && GenApi::IsWritable(ptrAcquisitionStatusSelector))
					{
						try
						{
							GENICAM_NAMESPACE::gcstring gcstr(strAcquisitionStatusSelector);

							ptrAcquisitionStatusSelector->FromString(gcstr);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("AcquisitionStatusSelector"));
							break;
						}
					}

					bool bAcquisitionStatus = 0;

					if(GetAcquisitionStatus(&bAcquisitionStatus))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AcquisitionStatus"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					GenApi::CBooleanPtr ptrAcquisitionStatus = m_pNodeMapDevice->_GetNode("AcquisitionStatus");

					if(ptrAcquisitionStatus.IsValid() && GenApi::IsWritable(ptrAcquisitionStatus))
						ptrAcquisitionStatus->SetValue(bAcquisitionStatus);

					EDeviceGenICamLineSelector eLineSelector;

					if(GetLineSelector(&eLineSelector))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineSelector"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(eLineSelector < 0 || eLineSelector >= EDeviceGenICamLineSelector_Count)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineSelector"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					CStringA strLineSelector(g_lpszGenICamLineSelector[eLineSelector]);

					GenApi::CEnumerationPtr ptrLineSelector = m_pNodeMapDevice->_GetNode("LineSelector");

					if(ptrLineSelector.IsValid() && GenApi::IsWritable(ptrLineSelector))
					{
						try
						{
							GENICAM_NAMESPACE::gcstring gcstr(strLineSelector);

							ptrLineSelector->FromString(gcstr);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineSelector"));
							break;
						}
					}

					switch(eLineSelector)
					{
					case EDeviceGenICamLineSelector_Line1:
						{
							EDeviceGenICamLineMode eLineMode;

							if(GetLineMode_Line1(&eLineMode))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineMode"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							if(eLineMode < 0 || eLineMode >= EDeviceGenICamLineMode_Count)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineMode"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							CStringA strLineMode(g_lpszGenICamLineMode[eLineMode]);

							GenApi::CEnumerationPtr ptrLineMode = m_pNodeMapDevice->_GetNode("LineMode");

							if(ptrLineMode.IsValid() && GenApi::IsWritable(ptrLineMode))
							{
								try
								{
									GENICAM_NAMESPACE::gcstring gcstr(strLineMode);

									ptrLineMode->FromString(gcstr);
								}
								catch(...)
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineMode"));
									break;
								}
							}

							EDeviceGenICamLineSource eLineSource;

							if(GetLineSource_Line1(&eLineSource))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineSource"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							if(eLineSource < 0 || eLineSource >= EDeviceGenICamLineSource_Count)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineSource"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							CStringA strLineSource(g_lpszGenICamLineSource[eLineSource]);

							GenApi::CEnumerationPtr ptrLineSource = m_pNodeMapDevice->_GetNode("LineSource");

							if(ptrLineSource.IsValid() && GenApi::IsWritable(ptrLineSource))
							{
								try
								{
									GENICAM_NAMESPACE::gcstring gcstr(strLineSource);

									ptrLineSource->FromString(gcstr);
								}
								catch(...)
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineSource"));
									break;
								}
							}

							EDeviceGenICamLineFormat eLineFormat;

							if(GetLineFormat_Line1(&eLineFormat))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineFormat"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							if(eLineFormat < 0 || eLineFormat >= EDeviceGenICamLineFormat_Count)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineFormat"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							CStringA strLineFormat(g_lpszGenICamLineFormat[eLineFormat]);

							GenApi::CEnumerationPtr ptrLineFormat = m_pNodeMapDevice->_GetNode("LineFormat");

							if(ptrLineFormat.IsValid() && GenApi::IsWritable(ptrLineFormat))
							{
								try
								{
									GENICAM_NAMESPACE::gcstring gcstr(strLineFormat);

									ptrLineFormat->FromString(gcstr);
								}
								catch(...)
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineFormat"));
									break;
								}
							}

							bool bLineInverter = 0;

							if(GetLineInverter_Line1(&bLineInverter))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineInverter"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							GenApi::CBooleanPtr ptrLineInverter = m_pNodeMapDevice->_GetNode("LineInverter");

							if(ptrLineInverter.IsValid() && GenApi::IsWritable(ptrLineInverter))
								ptrLineInverter->SetValue(bLineInverter);

							bool bLineTermination = 0;

							if(GetLineTermination_Line1(&bLineTermination))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineTermination"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							GenApi::CBooleanPtr ptrLineTermination = m_pNodeMapDevice->_GetNode("LineTermination");

							if(ptrLineTermination.IsValid() && GenApi::IsWritable(ptrLineTermination))
								ptrLineTermination->SetValue(bLineTermination);

							int nLineDebouncerTimeRaw = 0;

							if(GetLineDebouncerTimeRaw_Line1(&nLineDebouncerTimeRaw))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineDebouncerTimeRaw"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							GenApi::CIntegerPtr ptrLineDebouncerTimeRaw = m_pNodeMapDevice->_GetNode("LineDebouncerTimeRaw");

							if(ptrLineDebouncerTimeRaw.IsValid() && GenApi::IsWritable(ptrLineDebouncerTimeRaw))
							{
								try
								{
									ptrLineDebouncerTimeRaw->SetValue(nLineDebouncerTimeRaw);
								}
								catch(...)
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineDebouncerTimeRaw"));
									break;
								}
							}

							float fLineDebouncerTimeAbs = 0.;

							if(GetLineDebouncerTimeAbs_Line1(&fLineDebouncerTimeAbs))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineDebouncerTimeAbs"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							GenApi::CFloatPtr ptrLineDebouncerTimeAbs = m_pNodeMapDevice->_GetNode("LineDebouncerTimeAbs");

							if(ptrLineDebouncerTimeAbs.IsValid() && GenApi::IsWritable(ptrLineDebouncerTimeAbs))
							{
								try
								{
									ptrLineDebouncerTimeAbs->SetValue(fLineDebouncerTimeAbs);
								}
								catch(...)
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineDebouncerTimeAbs"));
									break;
								}
							}
						}
						break;
					case EDeviceGenICamLineSelector_Line2:
						{
							EDeviceGenICamLineMode eLineMode;

							if(GetLineMode_Line2(&eLineMode))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineMode"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							if(eLineMode < 0 || eLineMode >= EDeviceGenICamLineMode_Count)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineMode"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							CStringA strLineMode(g_lpszGenICamLineMode[eLineMode]);

							GenApi::CEnumerationPtr ptrLineMode = m_pNodeMapDevice->_GetNode("LineMode");

							if(ptrLineMode.IsValid() && GenApi::IsWritable(ptrLineMode))
							{
								try
								{
									GENICAM_NAMESPACE::gcstring gcstr(strLineMode);

									ptrLineMode->FromString(gcstr);
								}
								catch(...)
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineMode"));
									break;
								}
							}

							EDeviceGenICamLineSource eLineSource;

							if(GetLineSource_Line2(&eLineSource))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineSource"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							if(eLineSource < 0 || eLineSource >= EDeviceGenICamLineSource_Count)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineSource"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							CStringA strLineSource(g_lpszGenICamLineSource[eLineSource]);

							GenApi::CEnumerationPtr ptrLineSource = m_pNodeMapDevice->_GetNode("LineSource");

							if(ptrLineSource.IsValid() && GenApi::IsWritable(ptrLineSource))
							{
								try
								{
									GENICAM_NAMESPACE::gcstring gcstr(strLineSource);

									ptrLineSource->FromString(gcstr);
								}
								catch(...)
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineSource"));
									break;
								}
							}

							EDeviceGenICamLineFormat eLineFormat;

							if(GetLineFormat_Line2(&eLineFormat))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineFormat"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							if(eLineFormat < 0 || eLineFormat >= EDeviceGenICamLineFormat_Count)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineFormat"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							CStringA strLineFormat(g_lpszGenICamLineFormat[eLineFormat]);

							GenApi::CEnumerationPtr ptrLineFormat = m_pNodeMapDevice->_GetNode("LineFormat");

							if(ptrLineFormat.IsValid() && GenApi::IsWritable(ptrLineFormat))
							{
								try
								{
									GENICAM_NAMESPACE::gcstring gcstr(strLineFormat);

									ptrLineFormat->FromString(gcstr);
								}
								catch(...)
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineFormat"));
									break;
								}
							}

							bool bLineInverter = 0;

							if(GetLineInverter_Line2(&bLineInverter))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineInverter"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							GenApi::CBooleanPtr ptrLineInverter = m_pNodeMapDevice->_GetNode("LineInverter");

							if(ptrLineInverter.IsValid() && GenApi::IsWritable(ptrLineInverter))
								ptrLineInverter->SetValue(bLineInverter);

							bool bLineTermination = 0;

							if(GetLineTermination_Line2(&bLineTermination))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineTermination"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							GenApi::CBooleanPtr ptrLineTermination = m_pNodeMapDevice->_GetNode("LineTermination");

							if(ptrLineTermination.IsValid() && GenApi::IsWritable(ptrLineTermination))
								ptrLineTermination->SetValue(bLineTermination);

							int nLineDebouncerTimeRaw = 0;

							if(GetLineDebouncerTimeRaw_Line2(&nLineDebouncerTimeRaw))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineDebouncerTimeRaw"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							GenApi::CIntegerPtr ptrLineDebouncerTimeRaw = m_pNodeMapDevice->_GetNode("LineDebouncerTimeRaw");

							if(ptrLineDebouncerTimeRaw.IsValid() && GenApi::IsWritable(ptrLineDebouncerTimeRaw))
							{
								try
								{
									ptrLineDebouncerTimeRaw->SetValue(nLineDebouncerTimeRaw);
								}
								catch(...)
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineDebouncerTimeRaw"));
									break;
								}
							}

							float fLineDebouncerTimeAbs = 0.;

							if(GetLineDebouncerTimeAbs_Line2(&fLineDebouncerTimeAbs))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineDebouncerTimeAbs"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							GenApi::CFloatPtr ptrLineDebouncerTimeAbs = m_pNodeMapDevice->_GetNode("LineDebouncerTimeAbs");

							if(ptrLineDebouncerTimeAbs.IsValid() && GenApi::IsWritable(ptrLineDebouncerTimeAbs))
							{
								try
								{
									ptrLineDebouncerTimeAbs->SetValue(fLineDebouncerTimeAbs);
								}
								catch(...)
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineDebouncerTimeAbs"));
									break;
								}
							}
						}
						break;
					case EDeviceGenICamLineSelector_Out1:
						{
							EDeviceGenICamLineMode eLineMode;

							if(GetLineMode_Out1(&eLineMode))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineMode"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							if(eLineMode < 0 || eLineMode >= EDeviceGenICamLineMode_Count)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineMode"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							CStringA strLineMode(g_lpszGenICamLineMode[eLineMode]);

							GenApi::CEnumerationPtr ptrLineMode = m_pNodeMapDevice->_GetNode("LineMode");

							if(ptrLineMode.IsValid() && GenApi::IsWritable(ptrLineMode))
							{
								try
								{
									GENICAM_NAMESPACE::gcstring gcstr(strLineMode);

									ptrLineMode->FromString(gcstr);
								}
								catch(...)
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineMode"));
									break;
								}
							}

							EDeviceGenICamLineSource eLineSource;

							if(GetLineSource_Out1(&eLineSource))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineSource"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							if(eLineSource < 0 || eLineSource >= EDeviceGenICamLineSource_Count)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineSource"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							CStringA strLineSource(g_lpszGenICamLineSource[eLineSource]);

							GenApi::CEnumerationPtr ptrLineSource = m_pNodeMapDevice->_GetNode("LineSource");

							if(ptrLineSource.IsValid() && GenApi::IsWritable(ptrLineSource))
							{
								try
								{
									GENICAM_NAMESPACE::gcstring gcstr(strLineSource);

									ptrLineSource->FromString(gcstr);
								}
								catch(...)
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineSource"));
									break;
								}
							}

							EDeviceGenICamLineFormat eLineFormat;

							if(GetLineFormat_Out1(&eLineFormat))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineFormat"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							if(eLineFormat < 0 || eLineFormat >= EDeviceGenICamLineFormat_Count)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineFormat"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							CStringA strLineFormat(g_lpszGenICamLineFormat[eLineFormat]);

							GenApi::CEnumerationPtr ptrLineFormat = m_pNodeMapDevice->_GetNode("LineFormat");

							if(ptrLineFormat.IsValid() && GenApi::IsWritable(ptrLineFormat))
							{
								try
								{
									GENICAM_NAMESPACE::gcstring gcstr(strLineFormat);

									ptrLineFormat->FromString(gcstr);
								}
								catch(...)
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineFormat"));
									break;
								}
							}

							bool bLineInverter = 0;

							if(GetLineInverter_Out1(&bLineInverter))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineInverter"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							GenApi::CBooleanPtr ptrLineInverter = m_pNodeMapDevice->_GetNode("LineInverter");

							if(ptrLineInverter.IsValid() && GenApi::IsWritable(ptrLineInverter))
								ptrLineInverter->SetValue(bLineInverter);

							bool bLineTermination = 0;

							if(GetLineTermination_Out1(&bLineTermination))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineTermination"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							GenApi::CBooleanPtr ptrLineTermination = m_pNodeMapDevice->_GetNode("LineTermination");

							if(ptrLineTermination.IsValid() && GenApi::IsWritable(ptrLineTermination))
								ptrLineTermination->SetValue(bLineTermination);

							int nLineDebouncerTimeRaw = 0;

							if(GetLineDebouncerTimeRaw_Out1(&nLineDebouncerTimeRaw))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineDebouncerTimeRaw"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							GenApi::CIntegerPtr ptrLineDebouncerTimeRaw = m_pNodeMapDevice->_GetNode("LineDebouncerTimeRaw");

							if(ptrLineDebouncerTimeRaw.IsValid() && GenApi::IsWritable(ptrLineDebouncerTimeRaw))
							{
								try
								{
									ptrLineDebouncerTimeRaw->SetValue(nLineDebouncerTimeRaw);
								}
								catch(...)
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineDebouncerTimeRaw"));
									break;
								}
							}

							float fLineDebouncerTimeAbs = 0.;

							if(GetLineDebouncerTimeAbs_Out1(&fLineDebouncerTimeAbs))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineDebouncerTimeAbs"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							GenApi::CFloatPtr ptrLineDebouncerTimeAbs = m_pNodeMapDevice->_GetNode("LineDebouncerTimeAbs");

							if(ptrLineDebouncerTimeAbs.IsValid() && GenApi::IsWritable(ptrLineDebouncerTimeAbs))
							{
								try
								{
									ptrLineDebouncerTimeAbs->SetValue(fLineDebouncerTimeAbs);
								}
								catch(...)
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineDebouncerTimeAbs"));
									break;
								}
							}
						}
						break;
					case EDeviceGenICamLineSelector_Out2:
						{
							EDeviceGenICamLineMode eLineMode;

							if(GetLineMode_Out2(&eLineMode))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineMode"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							if(eLineMode < 0 || eLineMode >= EDeviceGenICamLineMode_Count)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineMode"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							CStringA strLineMode(g_lpszGenICamLineMode[eLineMode]);

							GenApi::CEnumerationPtr ptrLineMode = m_pNodeMapDevice->_GetNode("LineMode");

							if(ptrLineMode.IsValid() && GenApi::IsWritable(ptrLineMode))
							{
								try
								{
									GENICAM_NAMESPACE::gcstring gcstr(strLineMode);

									ptrLineMode->FromString(gcstr);
								}
								catch(...)
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineMode"));
									break;
								}
							}

							EDeviceGenICamLineSource eLineSource;

							if(GetLineSource_Out2(&eLineSource))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineSource"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							if(eLineSource < 0 || eLineSource >= EDeviceGenICamLineSource_Count)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineSource"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							CStringA strLineSource(g_lpszGenICamLineSource[eLineSource]);

							GenApi::CEnumerationPtr ptrLineSource = m_pNodeMapDevice->_GetNode("LineSource");

							if(ptrLineSource.IsValid() && GenApi::IsWritable(ptrLineSource))
							{
								try
								{
									GENICAM_NAMESPACE::gcstring gcstr(strLineSource);

									ptrLineSource->FromString(gcstr);
								}
								catch(...)
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineSource"));
									break;
								}
							}

							EDeviceGenICamLineFormat eLineFormat;

							if(GetLineFormat_Out2(&eLineFormat))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineFormat"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							if(eLineFormat < 0 || eLineFormat >= EDeviceGenICamLineFormat_Count)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineFormat"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							CStringA strLineFormat(g_lpszGenICamLineFormat[eLineFormat]);

							GenApi::CEnumerationPtr ptrLineFormat = m_pNodeMapDevice->_GetNode("LineFormat");

							if(ptrLineFormat.IsValid() && GenApi::IsWritable(ptrLineFormat))
							{
								try
								{
									GENICAM_NAMESPACE::gcstring gcstr(strLineFormat);

									ptrLineFormat->FromString(gcstr);
								}
								catch(...)
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineFormat"));
									break;
								}
							}

							bool bLineInverter = 0;

							if(GetLineInverter_Out2(&bLineInverter))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineInverter"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							GenApi::CBooleanPtr ptrLineInverter = m_pNodeMapDevice->_GetNode("LineInverter");

							if(ptrLineInverter.IsValid() && GenApi::IsWritable(ptrLineInverter))
								ptrLineInverter->SetValue(bLineInverter);

							bool bLineTermination = 0;

							if(GetLineTermination_Out2(&bLineTermination))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineTermination"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							GenApi::CBooleanPtr ptrLineTermination = m_pNodeMapDevice->_GetNode("LineTermination");

							if(ptrLineTermination.IsValid() && GenApi::IsWritable(ptrLineTermination))
								ptrLineTermination->SetValue(bLineTermination);

							int nLineDebouncerTimeRaw = 0;

							if(GetLineDebouncerTimeRaw_Out2(&nLineDebouncerTimeRaw))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineDebouncerTimeRaw"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							GenApi::CIntegerPtr ptrLineDebouncerTimeRaw = m_pNodeMapDevice->_GetNode("LineDebouncerTimeRaw");

							if(ptrLineDebouncerTimeRaw.IsValid() && GenApi::IsWritable(ptrLineDebouncerTimeRaw))
							{
								try
								{
									ptrLineDebouncerTimeRaw->SetValue(nLineDebouncerTimeRaw);
								}
								catch(...)
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineDebouncerTimeRaw"));
									break;
								}
							}

							float fLineDebouncerTimeAbs = 0.;

							if(GetLineDebouncerTimeAbs_Out2(&fLineDebouncerTimeAbs))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineDebouncerTimeAbs"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							GenApi::CFloatPtr ptrLineDebouncerTimeAbs = m_pNodeMapDevice->_GetNode("LineDebouncerTimeAbs");

							if(ptrLineDebouncerTimeAbs.IsValid() && GenApi::IsWritable(ptrLineDebouncerTimeAbs))
							{
								try
								{
									ptrLineDebouncerTimeAbs->SetValue(fLineDebouncerTimeAbs);
								}
								catch(...)
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineDebouncerTimeAbs"));
									break;
								}
							}
						}
						break;
					case EDeviceGenICamLineSelector_Out3:
						{
							EDeviceGenICamLineMode eLineMode;

							if(GetLineMode_Out3(&eLineMode))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineMode"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							if(eLineMode < 0 || eLineMode >= EDeviceGenICamLineMode_Count)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineMode"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							CStringA strLineMode(g_lpszGenICamLineMode[eLineMode]);

							GenApi::CEnumerationPtr ptrLineMode = m_pNodeMapDevice->_GetNode("LineMode");

							if(ptrLineMode.IsValid() && GenApi::IsWritable(ptrLineMode))
							{
								try
								{
									GENICAM_NAMESPACE::gcstring gcstr(strLineMode);

									ptrLineMode->FromString(gcstr);
								}
								catch(...)
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineMode"));
									break;
								}
							}

							EDeviceGenICamLineSource eLineSource;

							if(GetLineSource_Out3(&eLineSource))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineSource"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							if(eLineSource < 0 || eLineSource >= EDeviceGenICamLineSource_Count)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineSource"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							CStringA strLineSource(g_lpszGenICamLineSource[eLineSource]);

							GenApi::CEnumerationPtr ptrLineSource = m_pNodeMapDevice->_GetNode("LineSource");

							if(ptrLineSource.IsValid() && GenApi::IsWritable(ptrLineSource))
							{
								try
								{
									GENICAM_NAMESPACE::gcstring gcstr(strLineSource);

									ptrLineSource->FromString(gcstr);
								}
								catch(...)
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineSource"));
									break;
								}
							}

							EDeviceGenICamLineFormat eLineFormat;

							if(GetLineFormat_Out3(&eLineFormat))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineFormat"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							if(eLineFormat < 0 || eLineFormat >= EDeviceGenICamLineFormat_Count)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineFormat"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							CStringA strLineFormat(g_lpszGenICamLineFormat[eLineFormat]);

							GenApi::CEnumerationPtr ptrLineFormat = m_pNodeMapDevice->_GetNode("LineFormat");

							if(ptrLineFormat.IsValid() && GenApi::IsWritable(ptrLineFormat))
							{
								try
								{
									GENICAM_NAMESPACE::gcstring gcstr(strLineFormat);

									ptrLineFormat->FromString(gcstr);
								}
								catch(...)
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineFormat"));
									break;
								}
							}

							bool bLineInverter = 0;

							if(GetLineInverter_Out3(&bLineInverter))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineInverter"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							GenApi::CBooleanPtr ptrLineInverter = m_pNodeMapDevice->_GetNode("LineInverter");

							if(ptrLineInverter.IsValid() && GenApi::IsWritable(ptrLineInverter))
								ptrLineInverter->SetValue(bLineInverter);

							bool bLineTermination = 0;

							if(GetLineTermination_Out3(&bLineTermination))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineTermination"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							GenApi::CBooleanPtr ptrLineTermination = m_pNodeMapDevice->_GetNode("LineTermination");

							if(ptrLineTermination.IsValid() && GenApi::IsWritable(ptrLineTermination))
								ptrLineTermination->SetValue(bLineTermination);

							int nLineDebouncerTimeRaw = 0;

							if(GetLineDebouncerTimeRaw_Out3(&nLineDebouncerTimeRaw))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineDebouncerTimeRaw"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							GenApi::CIntegerPtr ptrLineDebouncerTimeRaw = m_pNodeMapDevice->_GetNode("LineDebouncerTimeRaw");

							if(ptrLineDebouncerTimeRaw.IsValid() && GenApi::IsWritable(ptrLineDebouncerTimeRaw))
							{
								try
								{
									ptrLineDebouncerTimeRaw->SetValue(nLineDebouncerTimeRaw);
								}
								catch(...)
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineDebouncerTimeRaw"));
									break;
								}
							}

							float fLineDebouncerTimeAbs = 0.;

							if(GetLineDebouncerTimeAbs_Out3(&fLineDebouncerTimeAbs))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineDebouncerTimeAbs"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							GenApi::CFloatPtr ptrLineDebouncerTimeAbs = m_pNodeMapDevice->_GetNode("LineDebouncerTimeAbs");

							if(ptrLineDebouncerTimeAbs.IsValid() && GenApi::IsWritable(ptrLineDebouncerTimeAbs))
							{
								try
								{
									ptrLineDebouncerTimeAbs->SetValue(fLineDebouncerTimeAbs);
								}
								catch(...)
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineDebouncerTimeAbs"));
									break;
								}
							}
						}
						break;
					case EDeviceGenICamLineSelector_Out4:
						{
							EDeviceGenICamLineMode eLineMode;

							if(GetLineMode_Out4(&eLineMode))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineMode"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							if(eLineMode < 0 || eLineMode >= EDeviceGenICamLineMode_Count)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineMode"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							CStringA strLineMode(g_lpszGenICamLineMode[eLineMode]);

							GenApi::CEnumerationPtr ptrLineMode = m_pNodeMapDevice->_GetNode("LineMode");

							if(ptrLineMode.IsValid() && GenApi::IsWritable(ptrLineMode))
							{
								try
								{
									GENICAM_NAMESPACE::gcstring gcstr(strLineMode);

									ptrLineMode->FromString(gcstr);
								}
								catch(...)
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineMode"));
									break;
								}
							}

							EDeviceGenICamLineSource eLineSource;

							if(GetLineSource_Out4(&eLineSource))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineSource"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							if(eLineSource < 0 || eLineSource >= EDeviceGenICamLineSource_Count)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineSource"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							CStringA strLineSource(g_lpszGenICamLineSource[eLineSource]);

							GenApi::CEnumerationPtr ptrLineSource = m_pNodeMapDevice->_GetNode("LineSource");

							if(ptrLineSource.IsValid() && GenApi::IsWritable(ptrLineSource))
							{
								try
								{
									GENICAM_NAMESPACE::gcstring gcstr(strLineSource);

									ptrLineSource->FromString(gcstr);
								}
								catch(...)
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineSource"));
									break;
								}
							}

							EDeviceGenICamLineFormat eLineFormat;

							if(GetLineFormat_Out4(&eLineFormat))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineFormat"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							if(eLineFormat < 0 || eLineFormat >= EDeviceGenICamLineFormat_Count)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineFormat"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							CStringA strLineFormat(g_lpszGenICamLineFormat[eLineFormat]);

							GenApi::CEnumerationPtr ptrLineFormat = m_pNodeMapDevice->_GetNode("LineFormat");

							if(ptrLineFormat.IsValid() && GenApi::IsWritable(ptrLineFormat))
							{
								try
								{
									GENICAM_NAMESPACE::gcstring gcstr(strLineFormat);

									ptrLineFormat->FromString(gcstr);
								}
								catch(...)
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineFormat"));
									break;
								}
							}

							bool bLineInverter = 0;

							if(GetLineInverter_Out4(&bLineInverter))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineInverter"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							GenApi::CBooleanPtr ptrLineInverter = m_pNodeMapDevice->_GetNode("LineInverter");

							if(ptrLineInverter.IsValid() && GenApi::IsWritable(ptrLineInverter))
								ptrLineInverter->SetValue(bLineInverter);

							bool bLineTermination = 0;

							if(GetLineTermination_Out4(&bLineTermination))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineTermination"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							GenApi::CBooleanPtr ptrLineTermination = m_pNodeMapDevice->_GetNode("LineTermination");

							if(ptrLineTermination.IsValid() && GenApi::IsWritable(ptrLineTermination))
								ptrLineTermination->SetValue(bLineTermination);

							int nLineDebouncerTimeRaw = 0;

							if(GetLineDebouncerTimeRaw_Out4(&nLineDebouncerTimeRaw))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineDebouncerTimeRaw"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							GenApi::CIntegerPtr ptrLineDebouncerTimeRaw = m_pNodeMapDevice->_GetNode("LineDebouncerTimeRaw");

							if(ptrLineDebouncerTimeRaw.IsValid() && GenApi::IsWritable(ptrLineDebouncerTimeRaw))
							{
								if(nLineDebouncerTimeRaw < ptrLineDebouncerTimeRaw->GetMin() || nLineDebouncerTimeRaw > ptrLineDebouncerTimeRaw->GetMax())
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineDebouncerTimeRaw"));
									break;
								}

								ptrLineDebouncerTimeRaw->SetValue(nLineDebouncerTimeRaw);
							}

							float fLineDebouncerTimeAbs = 0.;

							if(GetLineDebouncerTimeAbs_Out4(&fLineDebouncerTimeAbs))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineDebouncerTimeAbs"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							GenApi::CFloatPtr ptrLineDebouncerTimeAbs = m_pNodeMapDevice->_GetNode("LineDebouncerTimeAbs");

							if(ptrLineDebouncerTimeAbs.IsValid() && GenApi::IsWritable(ptrLineDebouncerTimeAbs))
							{
								try
								{
									ptrLineDebouncerTimeAbs->SetValue(fLineDebouncerTimeAbs);
								}
								catch(...)
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineDebouncerTimeAbs"));
									break;
								}
							}
						}
						break;
					case EDeviceGenICamLineSelector_Count:
						break;
					}

					EDeviceGenICamUserOutputSelector eUserOutputSelector;

					if(GetUserOutputSelector(&eUserOutputSelector))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("UserOutputSelector"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(eUserOutputSelector < 0 || eUserOutputSelector >= EDeviceGenICamUserOutputSelector_Count)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("UserOutputSelector"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					CStringA strUserOutputSelector(g_lpszGenICamUserOutputSelector[eUserOutputSelector]);

					GenApi::CEnumerationPtr ptrUserOutputSelector = m_pNodeMapDevice->_GetNode("UserOutputSelector");

					if(ptrUserOutputSelector.IsValid() && GenApi::IsWritable(ptrUserOutputSelector))
					{
						try
						{
							GENICAM_NAMESPACE::gcstring gcstr(strUserOutputSelector);

							ptrUserOutputSelector->FromString(gcstr);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("UserOutputSelector"));
							break;
						}
					}

					float fTimerDurationTimebaseAbs = 0.;

					if(GetTimerDurationTimebaseAbs(&fTimerDurationTimebaseAbs))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TimerDurationTimebaseAbs"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					GenApi::CFloatPtr ptrTimerDurationTimebaseAbs = m_pNodeMapDevice->_GetNode("TimerDurationTimebaseAbs");

					if(ptrTimerDurationTimebaseAbs.IsValid() && GenApi::IsWritable(ptrTimerDurationTimebaseAbs))
					{
						try
						{
							ptrTimerDurationTimebaseAbs->SetValue(fTimerDurationTimebaseAbs);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TimerDurationTimebaseAbs"));
							break;
						}
					}

					float fTimerDelayTimebaseAbs = 0.;

					if(GetTimerDelayTimebaseAbs(&fTimerDelayTimebaseAbs))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TimerDelayTimebaseAbs"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					GenApi::CFloatPtr ptrTimerDelayTimebaseAbs = m_pNodeMapDevice->_GetNode("TimerDelayTimebaseAbs");

					if(ptrTimerDelayTimebaseAbs.IsValid() && GenApi::IsWritable(ptrTimerDelayTimebaseAbs))
					{
						try
						{
							ptrTimerDelayTimebaseAbs->SetValue(fTimerDelayTimebaseAbs);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TimerDelayTimebaseAbs"));
							break;
						}
					}

					EDeviceGenICamTimerSelector eTimerSelector;

					if(GetTimerSelector(&eTimerSelector))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TimerSelector"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(eTimerSelector < 0 || eTimerSelector >= EDeviceGenICamTimerSelector_Count)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TimerSelector"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					CStringA strTimerSelector(g_lpszGenICamTimerSelector[eTimerSelector]);

					GenApi::CEnumerationPtr ptrTimerSelector = m_pNodeMapDevice->_GetNode("TimerSelector");

					if(ptrTimerSelector.IsValid() && GenApi::IsWritable(ptrTimerSelector))
					{
						try
						{
							GENICAM_NAMESPACE::gcstring gcstr(strTimerSelector);

							ptrTimerSelector->FromString(gcstr);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TimerSelector"));
							break;
						}
					}

					float fTimerDurationAbs = 0.;

					if(GetTimerDurationAbs(&fTimerDurationAbs))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TimerDurationAbs"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					GenApi::CFloatPtr ptrTimerDurationAbs = m_pNodeMapDevice->_GetNode("TimerDurationAbs");

					if(ptrTimerDurationAbs.IsValid() && GenApi::IsWritable(ptrTimerDurationAbs))
					{
						try
						{
							ptrTimerDurationAbs->SetValue(fTimerDurationAbs);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TimerDurationAbs"));
							break;
						}
					}

					int nTimerDurationRaw = 0;

					if(GetTimerDurationRaw(&nTimerDurationRaw))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TimerDurationRaw"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					GenApi::CIntegerPtr ptrTimerDurationRaw = m_pNodeMapDevice->_GetNode("TimerDurationRaw");

					if(ptrTimerDurationRaw.IsValid() && GenApi::IsWritable(ptrTimerDurationRaw))
					{
						try
						{
							ptrTimerDurationRaw->SetValue(nTimerDurationRaw);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TimerDurationRaw"));
							break;
						}
					}

					float fTimerDelayAbs = 0.;

					if(GetTimerDelayAbs(&fTimerDelayAbs))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TimerDelayAbs"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					GenApi::CFloatPtr ptrTimerDelayAbs = m_pNodeMapDevice->_GetNode("TimerDelayAbs");

					if(ptrTimerDelayAbs.IsValid() && GenApi::IsWritable(ptrTimerDelayAbs))
					{
						try
						{
							ptrTimerDelayAbs->SetValue(fTimerDelayAbs);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TimerDelayAbs"));
							break;
						}
					}

					int nTimerDelayRaw = 0;

					if(GetTimerDelayRaw(&nTimerDelayRaw))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TimerDelayRaw"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					GenApi::CIntegerPtr ptrTimerDelayRaw = m_pNodeMapDevice->_GetNode("TimerDelayRaw");

					if(ptrTimerDelayRaw.IsValid() && GenApi::IsWritable(ptrTimerDelayRaw))
					{
						try
						{
							ptrTimerDelayRaw->SetValue(nTimerDelayRaw);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TimerDelayRaw"));
							break;
						}
					}

					EDeviceGenICamTimerTriggerSource eTimerTriggerSource;

					if(GetTimerTriggerSource(&eTimerTriggerSource))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TimerTriggerSource"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(eTimerTriggerSource < 0 || eTimerTriggerSource >= EDeviceGenICamTimerTriggerSource_Count)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TimerTriggerSource"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					CStringA strTimerTriggerSource(g_lpszGenICamTimerTriggerSource[eTimerTriggerSource]);

					GenApi::CEnumerationPtr ptrTimerTriggerSource = m_pNodeMapDevice->_GetNode("TimerTriggerSource");

					if(ptrTimerTriggerSource.IsValid() && GenApi::IsWritable(ptrTimerTriggerSource))
					{
						try
						{
							GENICAM_NAMESPACE::gcstring gcstr(strTimerTriggerSource);

							ptrTimerTriggerSource->FromString(gcstr);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TimerTriggerSource"));
							break;
						}
					}

					EDeviceGenICamTimerTriggerActivation eTimerTriggerActivation;

					if(GetTimerTriggerActivation(&eTimerTriggerActivation))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TimerTriggerActivation"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(eTimerTriggerActivation < 0 || eTimerTriggerActivation >= EDeviceGenICamTimerTriggerActivation_Count)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TimerTriggerActivation"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					CStringA strTimerTriggerActivation(g_lpszGenICamTimerTriggerActivation[eTimerTriggerActivation]);

					GenApi::CEnumerationPtr ptrTimerTriggerActivation = m_pNodeMapDevice->_GetNode("TimerTriggerActivation");

					if(ptrTimerTriggerActivation.IsValid() && GenApi::IsWritable(ptrTimerTriggerActivation))
					{
						try
						{
							GENICAM_NAMESPACE::gcstring gcstr(strTimerTriggerActivation);

							ptrTimerTriggerActivation->FromString(gcstr);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TimerTriggerActivation"));
							break;
						}
					}

					bool bTimerSequenceEnable = 0;

					if(GetTimerSequenceEnable(&bTimerSequenceEnable))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TimerSequenceEnable"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					GenApi::CBooleanPtr ptrTimerSequenceEnable = m_pNodeMapDevice->_GetNode("TimerSequenceEnable");

					if(ptrTimerSequenceEnable.IsValid() && GenApi::IsWritable(ptrTimerSequenceEnable))
						ptrTimerSequenceEnable->SetValue(bTimerSequenceEnable);

					int nTimerSequenceLastEntryIndex = 0;

					if(GetTimerSequenceLastEntryIndex(&nTimerSequenceLastEntryIndex))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TimerSequenceLastEntryIndex"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					GenApi::CIntegerPtr ptrTimerSequenceLastEntryIndex = m_pNodeMapDevice->_GetNode("TimerSequenceLastEntryIndex");

					if(ptrTimerSequenceLastEntryIndex.IsValid() && GenApi::IsWritable(ptrTimerSequenceLastEntryIndex))
					{
						try
						{
							ptrTimerSequenceLastEntryIndex->SetValue(nTimerSequenceLastEntryIndex);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TimerSequenceLastEntryIndex"));
							break;
						}
					}

					int nTimerSequenceCurrentEntryIndex = 0;

					if(GetTimerSequenceCurrentEntryIndex(&nTimerSequenceCurrentEntryIndex))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TimerSequenceCurrentEntryIndex"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					GenApi::CIntegerPtr ptrTimerSequenceCurrentEntryIndex = m_pNodeMapDevice->_GetNode("TimerSequenceCurrentEntryIndex");

					if(ptrTimerSequenceCurrentEntryIndex.IsValid() && GenApi::IsWritable(ptrTimerSequenceCurrentEntryIndex))
					{
						try
						{
							ptrTimerSequenceCurrentEntryIndex->SetValue(nTimerSequenceCurrentEntryIndex);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TimerSequenceCurrentEntryIndex"));
							break;
						}
					}

					EDeviceGenICamTimerSequenceEntrySelector eTimerSequenceEntrySelector;

					if(GetTimerSequenceEntrySelector(&eTimerSequenceEntrySelector))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TimerSequenceEntrySelector"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(eTimerSequenceEntrySelector < 0 || eTimerSequenceEntrySelector >= EDeviceGenICamTimerSequenceEntrySelector_Count)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TimerSequenceEntrySelector"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					CStringA strTimerSequenceEntrySelector(g_lpszGenICamTimerSequenceEntrySelector[eTimerSequenceEntrySelector]);

					GenApi::CEnumerationPtr ptrTimerSequenceEntrySelector = m_pNodeMapDevice->_GetNode("TimerSequenceEntrySelector");

					if(ptrTimerSequenceEntrySelector.IsValid() && GenApi::IsWritable(ptrTimerSequenceEntrySelector))
					{
						try
						{
							GENICAM_NAMESPACE::gcstring gcstr(strTimerSequenceEntrySelector);

							ptrTimerSequenceEntrySelector->FromString(gcstr);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TimerSequenceEntrySelector"));
							break;
						}
					}

					EDeviceGenICamTimerSequenceTimerSelector eTimerSequenceTimerSelector;

					if(GetTimerSequenceTimerSelector(&eTimerSequenceTimerSelector))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TimerSequenceTimerSelector"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(eTimerSequenceTimerSelector < 0 || eTimerSequenceTimerSelector >= EDeviceGenICamTimerSequenceTimerSelector_Count)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TimerSequenceTimerSelector"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					CStringA strTimerSequenceTimerSelector(g_lpszGenICamTimerSequenceTimerSelector[eTimerSequenceTimerSelector]);

					GenApi::CEnumerationPtr ptrTimerSequenceTimerSelector = m_pNodeMapDevice->_GetNode("TimerSequenceTimerSelector");

					if(ptrTimerSequenceTimerSelector.IsValid() && GenApi::IsWritable(ptrTimerSequenceTimerSelector))
					{
						try
						{
							GENICAM_NAMESPACE::gcstring gcstr(strTimerSequenceTimerSelector);

							ptrTimerSequenceTimerSelector->FromString(gcstr);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TimerSequenceTimerSelector"));
							break;
						}
					}

					bool bTimerSequenceTimerEnable = 0;

					if(GetTimerSequenceTimerEnable(&bTimerSequenceTimerEnable))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TimerSequenceTimerEnable"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					GenApi::CBooleanPtr ptrTimerSequenceTimerEnable = m_pNodeMapDevice->_GetNode("TimerSequenceTimerEnable");

					if(ptrTimerSequenceTimerEnable.IsValid() && GenApi::IsWritable(ptrTimerSequenceTimerEnable))
						ptrTimerSequenceTimerEnable->SetValue(bTimerSequenceTimerEnable);

					bool bTimerSequenceTimerInverter = 0;

					if(GetTimerSequenceTimerInverter(&bTimerSequenceTimerInverter))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TimerSequenceTimerInverter"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					GenApi::CBooleanPtr ptrTimerSequenceTimerInverter = m_pNodeMapDevice->_GetNode("TimerSequenceTimerInverter");

					if(ptrTimerSequenceTimerInverter.IsValid() && GenApi::IsWritable(ptrTimerSequenceTimerInverter))
						ptrTimerSequenceTimerInverter->SetValue(bTimerSequenceTimerInverter);

					int nTimerSequenceTimerDelayRaw = 0;

					if(GetTimerSequenceTimerDelayRaw(&nTimerSequenceTimerDelayRaw))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TimerSequenceTimerDelayRaw"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					GenApi::CIntegerPtr ptrTimerSequenceTimerDelayRaw = m_pNodeMapDevice->_GetNode("TimerSequenceTimerDelayRaw");

					if(ptrTimerSequenceTimerDelayRaw.IsValid() && GenApi::IsWritable(ptrTimerSequenceTimerDelayRaw))
					{
						try
						{
							ptrTimerSequenceTimerDelayRaw->SetValue(nTimerSequenceTimerDelayRaw);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TimerSequenceTimerDelayRaw"));
							break;
						}
					}

					int nTimerSequenceTimerDurationRaw = 0;

					if(GetTimerSequenceTimerDurationRaw(&nTimerSequenceTimerDurationRaw))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TimerSequenceTimerDurationRaw"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					GenApi::CIntegerPtr ptrTimerSequenceTimerDurationRaw = m_pNodeMapDevice->_GetNode("TimerSequenceTimerDurationRaw");

					if(ptrTimerSequenceTimerDurationRaw.IsValid() && GenApi::IsWritable(ptrTimerSequenceTimerDurationRaw))
					{
						try
						{
							ptrTimerSequenceTimerDurationRaw->SetValue(nTimerSequenceTimerDurationRaw);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TimerSequenceTimerDurationRaw"));
							break;
						}
					}

					EDeviceGenICamCounterSelector eCounterSelector;

					if(GetCounterSelector(&eCounterSelector))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("CounterSelector"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(eCounterSelector < 0 || eCounterSelector >= EDeviceGenICamCounterSelector_Count)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("CounterSelector"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					CStringA strCounterSelector(g_lpszGenICamCounterSelector[eCounterSelector]);

					GenApi::CEnumerationPtr ptrCounterSelector = m_pNodeMapDevice->_GetNode("CounterSelector");

					if(ptrCounterSelector.IsValid() && GenApi::IsWritable(ptrCounterSelector))
					{
						try
						{
							GENICAM_NAMESPACE::gcstring gcstr(strCounterSelector);

							ptrCounterSelector->FromString(gcstr);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("CounterSelector"));
							break;
						}
					}

					EDeviceGenICamCounterEventSource eCounterEventSource;

					if(GetCounterEventSource(&eCounterEventSource))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("CounterEventSource"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(eCounterEventSource < 0 || eCounterEventSource >= EDeviceGenICamCounterEventSource_Count)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("CounterEventSource"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					CStringA strCounterEventSource(g_lpszGenICamCounterEventSource[eCounterEventSource]);

					GenApi::CEnumerationPtr ptrCounterEventSource = m_pNodeMapDevice->_GetNode("CounterEventSource");

					if(ptrCounterEventSource.IsValid() && GenApi::IsWritable(ptrCounterEventSource))
					{
						try
						{
							GENICAM_NAMESPACE::gcstring gcstr(strCounterEventSource);

							ptrCounterEventSource->FromString(gcstr);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("CounterEventSource"));
							break;
						}
					}

					EDeviceGenICamCounterResetSource eCounterResetSource;

					if(GetCounterResetSource(&eCounterResetSource))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("CounterResetSource"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(eCounterResetSource < 0 || eCounterResetSource >= EDeviceGenICamCounterResetSource_Count)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("CounterResetSource"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					CStringA strCounterResetSource(g_lpszGenICamCounterResetSource[eCounterResetSource]);

					GenApi::CEnumerationPtr ptrCounterResetSource = m_pNodeMapDevice->_GetNode("CounterResetSource");

					if(ptrCounterResetSource.IsValid() && GenApi::IsWritable(ptrCounterResetSource))
					{
						try
						{
							GENICAM_NAMESPACE::gcstring gcstr(strCounterResetSource);

							ptrCounterResetSource->FromString(gcstr);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("CounterResetSource"));
							break;
						}
					}

					bool bLUTEnable = 0;

					if(GetLUTEnable(&bLUTEnable))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LUTEnable"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					GenApi::CBooleanPtr ptrLUTEnable = m_pNodeMapDevice->_GetNode("LUTEnable");

					if(ptrLUTEnable.IsValid() && GenApi::IsWritable(ptrLUTEnable))
						ptrLUTEnable->SetValue(bLUTEnable);

					int nLUTIndex = 0;

					if(GetLUTIndex(&nLUTIndex))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LUTIndex"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					GenApi::CIntegerPtr ptrLUTIndex = m_pNodeMapDevice->_GetNode("LUTIndex");

					if(ptrLUTIndex.IsValid() && GenApi::IsWritable(ptrLUTIndex))
					{
						try
						{
							ptrLUTIndex->SetValue(nLUTIndex);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LUTIndex"));
							break;
						}
					}

					int nLUTValue = 0;

					if(GetLUTValue(&nLUTValue))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LUTValue"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					GenApi::CIntegerPtr ptrLUTValue = m_pNodeMapDevice->_GetNode("LUTValue");

					if(ptrLUTValue.IsValid() && GenApi::IsWritable(ptrLUTValue))
					{
						try
						{
							ptrLUTValue->SetValue(nLUTValue);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LUTValue"));
							break;
						}
					}

					EDeviceGenICamLUTSelector eLUTSelector;

					if(GetLUTSelector(&eLUTSelector))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LUTSelector"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(eLUTSelector < 0 || eLUTSelector >= EDeviceGenICamLUTSelector_Count)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LUTSelector"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					CStringA strLUTSelector(g_lpszGenICamLUTSelector[eLUTSelector]);

					GenApi::CEnumerationPtr ptrLUTSelector = m_pNodeMapDevice->_GetNode("LUTSelector");

					if(ptrLUTSelector.IsValid() && GenApi::IsWritable(ptrLUTSelector))
					{
						try
						{
							GENICAM_NAMESPACE::gcstring gcstr(strLUTSelector);

							ptrLUTSelector->FromString(gcstr);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LUTSelector"));
							break;
						}
					}

					int nGevLinkSpeed = 0;

					if(GetGevLinkSpeed(&nGevLinkSpeed))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("GevLinkSpeed"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					GenApi::CIntegerPtr ptrGevLinkSpeed = m_pNodeMapDevice->_GetNode("GevLinkSpeed");

					if(ptrGevLinkSpeed.IsValid() && GenApi::IsWritable(ptrGevLinkSpeed))
					{
						try
						{
							ptrGevLinkSpeed->SetValue(nGevLinkSpeed);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("GevLinkSpeed"));
							break;
						}
					}

					int nGevHeartbeatTimeout = 0;

					if(GetGevHeartbeatTimeout(&nGevHeartbeatTimeout))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("GevHeartbeatTimeout"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					GenApi::CIntegerPtr ptrGevHeartbeatTimeout = m_pNodeMapDevice->_GetNode("GevHeartbeatTimeout");

					if(ptrGevHeartbeatTimeout.IsValid() && GenApi::IsWritable(ptrGevHeartbeatTimeout))
					{
						try
						{
							ptrGevHeartbeatTimeout->SetValue(nGevHeartbeatTimeout);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("GevHeartbeatTimeout"));
							break;
						}
					}

					int nGevSCPSPacketSize = 0;

					if(GetGevSCPSPacketSize(&nGevSCPSPacketSize))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("GevSCPSPacketSize"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					GenApi::CIntegerPtr ptrGevSCPSPacketSize = m_pNodeMapDevice->_GetNode("GevSCPSPacketSize");

					if(ptrGevSCPSPacketSize.IsValid() && GenApi::IsWritable(ptrGevSCPSPacketSize))
					{
						try
						{
							ptrGevSCPSPacketSize->SetValue(nGevSCPSPacketSize);
						}
						catch(...)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("GevSCPSPacketSize"));
							break;
						}
					}

					bParamOK = true;
				}
				break;
			default:
				break;
			}

			if(!bParamOK)
				break;

			int nBpp = 8;
			int nAlignByte = 4;
			int nWidthStep = nWidth;

			bool bColor = strFormat.Left(4).CompareNoCase(_T("Mono"));

			int nChannel = 1;

			if(!bColor)
			{
				strFormat.Delete(0, 4);

				switch(strFormat[0])
				{
				case _T('1'):
					{
						switch(strFormat[1])
						{
						case _T('0'):
							{
								nBpp = 10;
								nWidthStep *= 2;
							}
							break;
						case _T('2'):
							{
								nBpp = 12;
								nWidthStep *= 2;
							}
							break;
						case _T('4'):
							{
								nBpp = 14;
								nWidthStep *= 2;
							}
							break;
						case _T('6'):
							{
								nBpp = 16;
								nWidthStep *= 2;
							}
						default:
							break;
						}
					}
					break;
				case _T('8'):
					{
						nBpp = 8;
					}
					break;
				default:
					break;
				}

				if((nWidthStep % 4))
					nWidthStep += (nAlignByte - (nWidthStep % nAlignByte));

				if(strFormat.GetLength() > 2)
				{
					//nBpp = 8;
					nAlignByte = 1;
					//nWidthStep = nWidth;
				}
			}
			else
			{
				nBpp = 8;
				nAlignByte = 4;
				nChannel = 3;
				nWidthStep = nWidth * 3;
			}

			int nMaxValue = (1 << nBpp) - 1;

			CMultipleVariable mv;

			for(int i = 0; i < nChannel; ++i)
				mv.AddValue(nMaxValue);

			this->InitBuffer(nWidth, nHeight, mv, CRavidImage::MakeValueFormat(nChannel, nBpp), nWidthStep, nAlignByte);
			this->ConnectImage();

			int nImageSize = nWidth * nHeight * GetImageInfo()->GetPixelSizeByte();

			GenTL::BUFFER_HANDLE hBuffer = NULL;

			memset(m_pImageBuffer, 0, sizeof(m_pImageBuffer));

			for(int i = 0; i < BUFF_COUNT; i++)
			{
				m_pImageBuffer[i] = malloc(nImageSize);

				status = DSAnnounceBuffer(m_hDS, m_pImageBuffer[i], nImageSize, (void *)i, &hBuffer);

				if(status != GenTL::GC_ERR_SUCCESS)
					break;

				status = DSQueueBuffer(m_hDS, hBuffer);

				if(status != GenTL::GC_ERR_SUCCESS)
					break;
			}

			if(status != GenTL::GC_ERR_SUCCESS)
			{
				strMessage.Format(_T("Failed to announce buffer"));
				eReturn = EDeviceInitializeResult_NotInitializeDevice;
				break;
			}

			status = GCRegisterEvent(m_hDS, GenTL::EVENT_NEW_BUFFER, &m_hNewEvent);

			if(status != GenTL::GC_ERR_SUCCESS)
			{
				strMessage.Format(_T("Failed to register event"));
				eReturn = EDeviceInitializeResult_NotInitializeDevice;
				break;
			}

			int nLocked = 0;

			GenApi::CIntegerPtr ptrLocked = m_pNodeMapDevice->_Ptr->GetNode("TLParamsLocked");

			if(ptrLocked.IsValid() && GenApi::IsWritable(ptrLocked))
			{
				ptrLocked->SetValue(1);
			}
			else
			{
				strMessage.Format(_T("Failed to locked params"));
				eReturn = EDeviceInitializeResult_NotInitializeDevice;
				break;
			}

			uint64_t AcqFrameCount = 0xffffffffffffffff;

			status = DSStartAcquisition(m_hDS, GenTL::ACQ_START_FLAGS_DEFAULT, AcqFrameCount);

			if(status != GenTL::GC_ERR_SUCCESS)
			{
				strMessage.Format(_T("Failed to start acquisition"));
				eReturn = EDeviceInitializeResult_NotInitializeDevice;
				break;
			}

			strStatus = CMultiLanguageManager::GetString(ELanguageParameter_Initialized);

			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_TheDevicehasbeensuccessfullyinitialized);

			m_bIsInitialized = true;

			eReturn = EDeviceInitializeResult_OK;

			CEventHandlerManager::BroadcastOnDeviceInitialized(this);
		}
		while(false);

		if(!IsInitialized())
			Terminate();

		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);

		SetStatus(strStatus);
	}
	while(false);

	return eReturn;
}

EDeviceTerminateResult CDeviceGenICam::Terminate()
{
	EDeviceTerminateResult eReturn = EDeviceTerminateResult_UnknownError;

	CString strMessage;

	SetStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("HIKVision"));
		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);
		SetMessage(strMessage);

		return EDeviceTerminateResult_NotFoundApiError;
	}

	do
	{
		if(!m_hModule)
			break;

		if(!m_pNodeMapDevice)
			break;

		if(IsLive() || !IsGrabAvailable())
			Stop();

		GenTL::PGCCloseLib GCCloseLib(reinterpret_cast<GenTL::PGCCloseLib>(reinterpret_cast<void *>(GetProcAddress(m_hModule, ("GCCloseLib")))));

		GenTL::PDSClose DSClose(reinterpret_cast<GenTL::PDSClose>(reinterpret_cast<void *>(GetProcAddress(m_hModule, ("DSClose")))));

		GenTL::PDevClose DevClose(reinterpret_cast<GenTL::PDevClose>(reinterpret_cast<void *>(GetProcAddress(m_hModule, ("DevClose")))));

		GenTL::PTLClose TLClose(reinterpret_cast<GenTL::PTLClose>(reinterpret_cast<void *>(GetProcAddress(m_hModule, ("TLClose")))));

		GenTL::PIFClose IFClose(reinterpret_cast<GenTL::PIFClose>(reinterpret_cast<void *>(GetProcAddress(m_hModule, ("IFClose")))));

		GenTL::PDSFlushQueue DSFlushQueue(reinterpret_cast<GenTL::PDSFlushQueue>(reinterpret_cast<void *>(GetProcAddress(m_hModule, ("DSFlushQueue")))));

		GenTL::PGCUnregisterEvent GCUnregisterEvent(reinterpret_cast<GenTL::PGCUnregisterEvent>(reinterpret_cast<void *>(GetProcAddress(m_hModule, ("GCUnregisterEvent")))));

		GenTL::PDSStopAcquisition DSStopAcquisition(reinterpret_cast<GenTL::PDSStopAcquisition>(reinterpret_cast<void *>(GetProcAddress(m_hModule, ("DSStopAcquisition")))));

		if(IsInitialized())
		{
			GenApi::CCommandPtr ptrAcquisitionStop = m_pNodeMapDevice->_Ptr->GetNode("AcquisitionStop");

			ptrAcquisitionStop->Execute();

			GenApi::CIntegerPtr ptrLocked = m_pNodeMapDevice->_Ptr->GetNode("TLParamsLocked");

			if(ptrLocked.IsValid() && GenApi::IsWritable(ptrLocked))
				ptrLocked->SetValue(0);
		}

		for(int j = 0; j < BUFF_COUNT; j++)
		{
			if(m_pImageBuffer[j])
				free(m_pImageBuffer[j]);
			m_pImageBuffer[j] = NULL;
		}

		DSStopAcquisition(m_hDS, GenTL::ACQ_STOP_FLAGS_DEFAULT);

		GCUnregisterEvent(m_hDS, GenTL::EVENT_NEW_BUFFER);

		DSFlushQueue(m_hDS, GenTL::ACQ_QUEUE_INPUT_TO_OUTPUT);
		DSFlushQueue(m_hDS, GenTL::ACQ_QUEUE_OUTPUT_DISCARD);

		DSClose(m_hDS);
		DevClose(m_hDEV);

		IFClose(m_hIF);
		TLClose(m_hTL);
		GCCloseLib();

		m_hTL = GENTL_INVALID_HANDLE;

		m_hIF = GENTL_INVALID_HANDLE;

		m_hDEV = GENTL_INVALID_HANDLE;

		m_hDS = GENTL_INVALID_HANDLE;

		m_hNewEvent = GENTL_INVALID_HANDLE;

		m_bIsLive = false;
		m_bIsInitialized = false;
		m_bIsGrabAvailable = true;

		strMessage = CMultiLanguageManager::GetString(ELanguageParameter_TheDevicehasbeensuccessfullyterminated);

		eReturn = EDeviceTerminateResult_OK;

		CEventHandlerManager::BroadcastOnDeviceTerminated(this);
	}
	while(false);

	this->ClearBuffer();

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceGenICam::LoadSettings()
{
	bool bReturn = false;

	m_vctParameterFieldConfigurations.clear();

	do
	{
		AddParameterFieldConfigurations(EDeviceParameterGenICam_DeviceID, g_lpszParamGenICam[EDeviceParameterGenICam_DeviceID], _T("0"), EParameterFieldType_Edit);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_InitializeMode, g_lpszParamGenICam[EDeviceParameterGenICam_InitializeMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamInitMode, EDeviceGenICamInitMode_Count), nullptr, 0);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_CtiFilePath, g_lpszParamGenICam[EDeviceParameterGenICam_CtiFilePath], _T("C:\\\\"), EParameterFieldType_Edit);

		///////////////////////////////////////////////////////////
		AddParameterFieldConfigurations(EDeviceParameterGenICam_AnalogControl, g_lpszParamGenICam[EDeviceParameterGenICam_AnalogControl], _T("1"), EParameterFieldType_None);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_GainAuto, g_lpszParamGenICam[EDeviceParameterGenICam_GainAuto], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamGainAuto, EDeviceGenICamGainAuto_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_GainSelector, g_lpszParamGenICam[EDeviceParameterGenICam_GainSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamGainSelector, EDeviceGenICamGainSelector_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_GainRaw, g_lpszParamGenICam[EDeviceParameterGenICam_GainRaw], _T("0"), EParameterFieldType_Edit, nullptr, _T("integer"), 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_BlackLevelSelector, g_lpszParamGenICam[EDeviceParameterGenICam_BlackLevelSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamBlackLevelSelector, EDeviceGenICamBlackLevelSelector_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_BlackLevelRaw, g_lpszParamGenICam[EDeviceParameterGenICam_BlackLevelRaw], _T("0"), EParameterFieldType_Edit, nullptr, _T("integer"), 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_BalanceWhiteAuto, g_lpszParamGenICam[EDeviceParameterGenICam_BalanceWhiteAuto], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamBalanceWhiteAuto, EDeviceGenICamBalanceWhiteAuto_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_BalanceRatioSelector, g_lpszParamGenICam[EDeviceParameterGenICam_BalanceRatioSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamBalanceRatioSelector, EDeviceGenICamBalanceRatioSelector_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_BalanceRatioAbs, g_lpszParamGenICam[EDeviceParameterGenICam_BalanceRatioAbs], _T("0"), EParameterFieldType_Edit, nullptr, _T("float"), 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_BalanceRatioRaw, g_lpszParamGenICam[EDeviceParameterGenICam_BalanceRatioRaw], _T("0"), EParameterFieldType_Edit, nullptr, _T("integer"), 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_GammaEnable, g_lpszParamGenICam[EDeviceParameterGenICam_GammaEnable], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamGammaEnable, EDeviceGenICamGammaEnable_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_Gamma, g_lpszParamGenICam[EDeviceParameterGenICam_Gamma], _T("0"), EParameterFieldType_Edit, nullptr, _T("float"), 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_DigitalShift, g_lpszParamGenICam[EDeviceParameterGenICam_DigitalShift], _T("0"), EParameterFieldType_Edit, nullptr, _T("integer"), 1);

		/////////////////////////////////////////////////////////
		AddParameterFieldConfigurations(EDeviceParameterGenICam_ImageFormat, g_lpszParamGenICam[EDeviceParameterGenICam_ImageFormat], _T("1"), EParameterFieldType_None);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_PixelFormat, g_lpszParamGenICam[EDeviceParameterGenICam_PixelFormat], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamPixelFormat, EDeviceGenICamPixelFormat_Count), _T("Only support Mono, RGB, Mono 12 Packed, Mono 10 Packed"), 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_ReverseX, g_lpszParamGenICam[EDeviceParameterGenICam_ReverseX], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamGammaEnable, EDeviceGenICamGammaEnable_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_ReverseY, g_lpszParamGenICam[EDeviceParameterGenICam_ReverseY], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamGammaEnable, EDeviceGenICamGammaEnable_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_SensorWidth, g_lpszParamGenICam[EDeviceParameterGenICam_SensorWidth], _T("0"), EParameterFieldType_Edit, nullptr, _T("integer"), 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_SensorHeight, g_lpszParamGenICam[EDeviceParameterGenICam_SensorHeight], _T("0"), EParameterFieldType_Edit, nullptr, _T("integer"), 1);

		/////////////////////////////////////////////////////////
		AddParameterFieldConfigurations(EDeviceParameterGenICam_AOI, g_lpszParamGenICam[EDeviceParameterGenICam_AOI], _T("1"), EParameterFieldType_None);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_Width, g_lpszParamGenICam[EDeviceParameterGenICam_Width], _T("0"), EParameterFieldType_Edit, nullptr, _T("integer"), 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_Height, g_lpszParamGenICam[EDeviceParameterGenICam_Height], _T("0"), EParameterFieldType_Edit, nullptr, _T("integer"), 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_OffsetX, g_lpszParamGenICam[EDeviceParameterGenICam_OffsetX], _T("0"), EParameterFieldType_Edit, nullptr, _T("integer"), 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_OffsetY, g_lpszParamGenICam[EDeviceParameterGenICam_OffsetY], _T("0"), EParameterFieldType_Edit, nullptr, _T("integer"), 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_BinningVertical, g_lpszParamGenICam[EDeviceParameterGenICam_BinningVertical], _T("0"), EParameterFieldType_Edit, nullptr, _T("integer"), 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_BinningHorizontal, g_lpszParamGenICam[EDeviceParameterGenICam_BinningHorizontal], _T("0"), EParameterFieldType_Edit, nullptr, _T("integer"), 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_LegacyBinningVertical, g_lpszParamGenICam[EDeviceParameterGenICam_LegacyBinningVertical], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamLegacyBinningVertical, EDeviceGenICamLegacyBinningVertical_Count), nullptr, 1);

		/////////////////////////////////////////////////////////
		AddParameterFieldConfigurations(EDeviceParameterGenICam_AcquisitionTrigger, g_lpszParamGenICam[EDeviceParameterGenICam_AcquisitionTrigger], _T("1"), EParameterFieldType_None);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_EnableBurstAcquisition, g_lpszParamGenICam[EDeviceParameterGenICam_EnableBurstAcquisition], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamGammaEnable, EDeviceGenICamGammaEnable_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_AcquisitionMode, g_lpszParamGenICam[EDeviceParameterGenICam_AcquisitionMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamAcquisitionMode, EDeviceGenICamAcquisitionMode_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_TriggerSelector, g_lpszParamGenICam[EDeviceParameterGenICam_TriggerSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamTriggerSelector, EDeviceGenICamTriggerSelector_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_TriggerMode, g_lpszParamGenICam[EDeviceParameterGenICam_TriggerMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamTriggerMode, EDeviceGenICamTriggerMode_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_TriggerSource, g_lpszParamGenICam[EDeviceParameterGenICam_TriggerSource], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamTriggerSource, EDeviceGenICamTriggerSource_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_TriggerActivation, g_lpszParamGenICam[EDeviceParameterGenICam_TriggerActivation], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamTriggerActivation, EDeviceGenICamTriggerActivation_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_TriggerDelayAbs, g_lpszParamGenICam[EDeviceParameterGenICam_TriggerDelayAbs], _T("0"), EParameterFieldType_Edit, nullptr, _T("float"), 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_ExposureAuto, g_lpszParamGenICam[EDeviceParameterGenICam_ExposureAuto], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamExposureAuto, EDeviceGenICamExposureAuto_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_ExposureMode, g_lpszParamGenICam[EDeviceParameterGenICam_ExposureMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamExposureMode, EDeviceGenICamExposureMode_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_ExposureTimeRaw, g_lpszParamGenICam[EDeviceParameterGenICam_ExposureTimeRaw], _T("0"), EParameterFieldType_Edit, nullptr, _T("integer"), 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_ExposureTimeAbs, g_lpszParamGenICam[EDeviceParameterGenICam_ExposureTimeAbs], _T("0"), EParameterFieldType_Edit, nullptr, _T("float"), 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_ExposureTimeBaseAbs, g_lpszParamGenICam[EDeviceParameterGenICam_ExposureTimeBaseAbs], _T("0"), EParameterFieldType_Edit, nullptr, _T("float"), 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_ExposureTimeBaseAbsEnable, g_lpszParamGenICam[EDeviceParameterGenICam_ExposureTimeBaseAbsEnable], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamGammaEnable, EDeviceGenICamGammaEnable_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_AcquisitionLineRateRaw, g_lpszParamGenICam[EDeviceParameterGenICam_AcquisitionLineRateRaw], _T("0"), EParameterFieldType_Edit, nullptr, _T("integer"), 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_AcquisitionLineRateAbs, g_lpszParamGenICam[EDeviceParameterGenICam_AcquisitionLineRateAbs], _T("0"), EParameterFieldType_Edit, nullptr, _T("float"), 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_ResultingLineRateAbs, g_lpszParamGenICam[EDeviceParameterGenICam_ResultingLineRateAbs], _T("0"), EParameterFieldType_Edit, nullptr, _T("float"), 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_AcquisitionFrameRate, g_lpszParamGenICam[EDeviceParameterGenICam_AcquisitionFrameRate], _T("0"), EParameterFieldType_Edit, nullptr, _T("float"), 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_AcquisitionFrameRateEnable, g_lpszParamGenICam[EDeviceParameterGenICam_AcquisitionFrameRateEnable], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamGammaEnable, EDeviceGenICamGammaEnable_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_ResultingFrameRateAbs, g_lpszParamGenICam[EDeviceParameterGenICam_ResultingFrameRateAbs], _T("0"), EParameterFieldType_Edit, nullptr, _T("float"), 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_AcquisitionStatusSelector, g_lpszParamGenICam[EDeviceParameterGenICam_AcquisitionStatusSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamAcquisitionStatusSelector, EDeviceGenICamAcquisitionStatusSelector_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_AcquisitionStatus, g_lpszParamGenICam[EDeviceParameterGenICam_AcquisitionStatus], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamGammaEnable, EDeviceGenICamGammaEnable_Count), nullptr, 1);

		/////////////////////////////////////////////////////////
		AddParameterFieldConfigurations(EDeviceParameterGenICam_DigitalIO, g_lpszParamGenICam[EDeviceParameterGenICam_DigitalIO], _T("1"), EParameterFieldType_None);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_LineSelector, g_lpszParamGenICam[EDeviceParameterGenICam_LineSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamLineSelector, EDeviceGenICamLineSelector_Count), nullptr, 1);

		//
		AddParameterFieldConfigurations(EDeviceParameterGenICam_Line1, g_lpszParamGenICam[EDeviceParameterGenICam_Line1], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_Line1LineMode, g_lpszParamGenICam[EDeviceParameterGenICam_Line1LineMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamLineMode, EDeviceGenICamLineMode_Count), nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_Line1LineSource, g_lpszParamGenICam[EDeviceParameterGenICam_Line1LineSource], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamLineSource, EDeviceGenICamLineSource_Count), nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_Line1LineFormat, g_lpszParamGenICam[EDeviceParameterGenICam_Line1LineFormat], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamLineFormat, EDeviceGenICamLineFormat_Count), nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_Line1LineInverter, g_lpszParamGenICam[EDeviceParameterGenICam_Line1LineInverter], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamGammaEnable, EDeviceGenICamGammaEnable_Count), nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_Line1LineTermination, g_lpszParamGenICam[EDeviceParameterGenICam_Line1LineTermination], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamGammaEnable, EDeviceGenICamGammaEnable_Count), nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_Line1LineDebouncerTimeRaw, g_lpszParamGenICam[EDeviceParameterGenICam_Line1LineDebouncerTimeRaw], _T("0"), EParameterFieldType_Edit, nullptr, _T("integer"), 2);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_Line1LineDebouncerTimeAbs, g_lpszParamGenICam[EDeviceParameterGenICam_Line1LineDebouncerTimeAbs], _T("0"), EParameterFieldType_Edit, nullptr, _T("float"), 2);
		//
		AddParameterFieldConfigurations(EDeviceParameterGenICam_Line2, g_lpszParamGenICam[EDeviceParameterGenICam_Line2], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_Line2LineMode, g_lpszParamGenICam[EDeviceParameterGenICam_Line2LineMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamLineMode, EDeviceGenICamLineMode_Count), nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_Line2LineSource, g_lpszParamGenICam[EDeviceParameterGenICam_Line2LineSource], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamLineSource, EDeviceGenICamLineSource_Count), nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_Line2LineFormat, g_lpszParamGenICam[EDeviceParameterGenICam_Line2LineFormat], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamLineFormat, EDeviceGenICamLineFormat_Count), nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_Line2LineInverter, g_lpszParamGenICam[EDeviceParameterGenICam_Line2LineInverter], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamGammaEnable, EDeviceGenICamGammaEnable_Count), nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_Line2LineTermination, g_lpszParamGenICam[EDeviceParameterGenICam_Line2LineTermination], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamGammaEnable, EDeviceGenICamGammaEnable_Count), nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_Line2LineDebouncerTimeRaw, g_lpszParamGenICam[EDeviceParameterGenICam_Line2LineDebouncerTimeRaw], _T("0"), EParameterFieldType_Edit, nullptr, _T("integer"), 2);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_Line2LineDebouncerTimeAbs, g_lpszParamGenICam[EDeviceParameterGenICam_Line2LineDebouncerTimeAbs], _T("0"), EParameterFieldType_Edit, nullptr, _T("float"), 2);
		//
		AddParameterFieldConfigurations(EDeviceParameterGenICam_Out1, g_lpszParamGenICam[EDeviceParameterGenICam_Out1], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_Out1LineMode, g_lpszParamGenICam[EDeviceParameterGenICam_Out1LineMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamLineMode, EDeviceGenICamLineMode_Count), nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_Out1LineSource, g_lpszParamGenICam[EDeviceParameterGenICam_Out1LineSource], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamLineSource, EDeviceGenICamLineSource_Count), nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_Out1LineFormat, g_lpszParamGenICam[EDeviceParameterGenICam_Out1LineFormat], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamLineFormat, EDeviceGenICamLineFormat_Count), nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_Out1LineInverter, g_lpszParamGenICam[EDeviceParameterGenICam_Out1LineInverter], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamGammaEnable, EDeviceGenICamGammaEnable_Count), nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_Out1LineTermination, g_lpszParamGenICam[EDeviceParameterGenICam_Out1LineTermination], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamGammaEnable, EDeviceGenICamGammaEnable_Count), nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_Out1LineDebouncerTimeRaw, g_lpszParamGenICam[EDeviceParameterGenICam_Out1LineDebouncerTimeRaw], _T("0"), EParameterFieldType_Edit, nullptr, _T("integer"), 2);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_Out1LineDebouncerTimeAbs, g_lpszParamGenICam[EDeviceParameterGenICam_Out1LineDebouncerTimeAbs], _T("0"), EParameterFieldType_Edit, nullptr, _T("float"), 2);
		//
		AddParameterFieldConfigurations(EDeviceParameterGenICam_Out2, g_lpszParamGenICam[EDeviceParameterGenICam_Out2], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_Out2LineMode, g_lpszParamGenICam[EDeviceParameterGenICam_Out2LineMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamLineMode, EDeviceGenICamLineMode_Count), nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_Out2LineSource, g_lpszParamGenICam[EDeviceParameterGenICam_Out2LineSource], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamLineSource, EDeviceGenICamLineSource_Count), nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_Out2LineFormat, g_lpszParamGenICam[EDeviceParameterGenICam_Out2LineFormat], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamLineFormat, EDeviceGenICamLineFormat_Count), nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_Out2LineInverter, g_lpszParamGenICam[EDeviceParameterGenICam_Out2LineInverter], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamGammaEnable, EDeviceGenICamGammaEnable_Count), nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_Out2LineTermination, g_lpszParamGenICam[EDeviceParameterGenICam_Out2LineTermination], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamGammaEnable, EDeviceGenICamGammaEnable_Count), nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_Out2LineDebouncerTimeRaw, g_lpszParamGenICam[EDeviceParameterGenICam_Out2LineDebouncerTimeRaw], _T("0"), EParameterFieldType_Edit, nullptr, _T("integer"), 2);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_Out2LineDebouncerTimeAbs, g_lpszParamGenICam[EDeviceParameterGenICam_Out2LineDebouncerTimeAbs], _T("0"), EParameterFieldType_Edit, nullptr, _T("float"), 2);
		//
		AddParameterFieldConfigurations(EDeviceParameterGenICam_Out3, g_lpszParamGenICam[EDeviceParameterGenICam_Out3], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_Out3LineMode, g_lpszParamGenICam[EDeviceParameterGenICam_Out3LineMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamLineMode, EDeviceGenICamLineMode_Count), nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_Out3LineSource, g_lpszParamGenICam[EDeviceParameterGenICam_Out3LineSource], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamLineSource, EDeviceGenICamLineSource_Count), nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_Out3LineFormat, g_lpszParamGenICam[EDeviceParameterGenICam_Out3LineFormat], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamLineFormat, EDeviceGenICamLineFormat_Count), nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_Out3LineInverter, g_lpszParamGenICam[EDeviceParameterGenICam_Out3LineInverter], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamGammaEnable, EDeviceGenICamGammaEnable_Count), nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_Out3LineTermination, g_lpszParamGenICam[EDeviceParameterGenICam_Out3LineTermination], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamGammaEnable, EDeviceGenICamGammaEnable_Count), nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_Out3LineDebouncerTimeRaw, g_lpszParamGenICam[EDeviceParameterGenICam_Out3LineDebouncerTimeRaw], _T("0"), EParameterFieldType_Edit, nullptr, _T("integer"), 2);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_Out3LineDebouncerTimeAbs, g_lpszParamGenICam[EDeviceParameterGenICam_Out3LineDebouncerTimeAbs], _T("0"), EParameterFieldType_Edit, nullptr, _T("float"), 2);
		//
		AddParameterFieldConfigurations(EDeviceParameterGenICam_Out4, g_lpszParamGenICam[EDeviceParameterGenICam_Out4], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_Out4LineMode, g_lpszParamGenICam[EDeviceParameterGenICam_Out4LineMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamLineMode, EDeviceGenICamLineMode_Count), nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_Out4LineSource, g_lpszParamGenICam[EDeviceParameterGenICam_Out4LineSource], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamLineSource, EDeviceGenICamLineSource_Count), nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_Out4LineFormat, g_lpszParamGenICam[EDeviceParameterGenICam_Out4LineFormat], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamLineFormat, EDeviceGenICamLineFormat_Count), nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_Out4LineInverter, g_lpszParamGenICam[EDeviceParameterGenICam_Out4LineInverter], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamGammaEnable, EDeviceGenICamGammaEnable_Count), nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_Out4LineTermination, g_lpszParamGenICam[EDeviceParameterGenICam_Out4LineTermination], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamGammaEnable, EDeviceGenICamGammaEnable_Count), nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_Out4LineDebouncerTimeRaw, g_lpszParamGenICam[EDeviceParameterGenICam_Out4LineDebouncerTimeRaw], _T("0"), EParameterFieldType_Edit, nullptr, _T("integer"), 2);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_Out4LineDebouncerTimeAbs, g_lpszParamGenICam[EDeviceParameterGenICam_Out4LineDebouncerTimeAbs], _T("0"), EParameterFieldType_Edit, nullptr, _T("float"), 2);
		//

		AddParameterFieldConfigurations(EDeviceParameterGenICam_UserOutputSelector, g_lpszParamGenICam[EDeviceParameterGenICam_UserOutputSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamUserOutputSelector, EDeviceGenICamUserOutputSelector_Count), nullptr, 1);

		/////////////////////////////////////////////////////////
		AddParameterFieldConfigurations(EDeviceParameterGenICam_TimerControls, g_lpszParamGenICam[EDeviceParameterGenICam_TimerControls], _T("1"), EParameterFieldType_None);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_TimerDurationTimebaseAbs, g_lpszParamGenICam[EDeviceParameterGenICam_TimerDurationTimebaseAbs], _T("0"), EParameterFieldType_Edit, nullptr, _T("float"), 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_TimerDelayTimebaseAbs, g_lpszParamGenICam[EDeviceParameterGenICam_TimerDelayTimebaseAbs], _T("0"), EParameterFieldType_Edit, nullptr, _T("float"), 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_TimerSelector, g_lpszParamGenICam[EDeviceParameterGenICam_TimerSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamTimerSelector, EDeviceGenICamTimerSelector_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_TimerDurationAbs, g_lpszParamGenICam[EDeviceParameterGenICam_TimerDurationAbs], _T("0"), EParameterFieldType_Edit, nullptr, _T("float"), 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_TimerDurationRaw, g_lpszParamGenICam[EDeviceParameterGenICam_TimerDurationRaw], _T("0"), EParameterFieldType_Edit, nullptr, _T("integer"), 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_TimerDelayAbs, g_lpszParamGenICam[EDeviceParameterGenICam_TimerDelayAbs], _T("0"), EParameterFieldType_Edit, nullptr, _T("float"), 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_TimerDelayRaw, g_lpszParamGenICam[EDeviceParameterGenICam_TimerDelayRaw], _T("0"), EParameterFieldType_Edit, nullptr, _T("integer"), 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_TimerTriggerSource, g_lpszParamGenICam[EDeviceParameterGenICam_TimerTriggerSource], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamTimerTriggerSource, EDeviceGenICamTimerTriggerSource_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_TimerTriggerActivation, g_lpszParamGenICam[EDeviceParameterGenICam_TimerTriggerActivation], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamTimerTriggerActivation, EDeviceGenICamTimerTriggerActivation_Count), nullptr, 1);

		//////////////////////////////////////////////////////////
		AddParameterFieldConfigurations(EDeviceParameterGenICam_TimerSequence, g_lpszParamGenICam[EDeviceParameterGenICam_TimerSequence], _T("1"), EParameterFieldType_None);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_TimerSequenceEnable, g_lpszParamGenICam[EDeviceParameterGenICam_TimerSequenceEnable], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamGammaEnable, EDeviceGenICamGammaEnable_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_TimerSequenceLastEntryIndex, g_lpszParamGenICam[EDeviceParameterGenICam_TimerSequenceLastEntryIndex], _T("0"), EParameterFieldType_Edit, nullptr, _T("integer"), 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_TimerSequenceCurrentEntryIndex, g_lpszParamGenICam[EDeviceParameterGenICam_TimerSequenceCurrentEntryIndex], _T("0"), EParameterFieldType_Edit, nullptr, _T("integer"), 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_TimerSequenceEntrySelector, g_lpszParamGenICam[EDeviceParameterGenICam_TimerSequenceEntrySelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamTimerSequenceEntrySelector, EDeviceGenICamTimerSequenceEntrySelector_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_TimerSequenceTimerSelector, g_lpszParamGenICam[EDeviceParameterGenICam_TimerSequenceTimerSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamTimerSequenceTimerSelector, EDeviceGenICamTimerSequenceTimerSelector_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_TimerSequenceTimerEnable, g_lpszParamGenICam[EDeviceParameterGenICam_TimerSequenceTimerEnable], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamGammaEnable, EDeviceGenICamGammaEnable_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_TimerSequenceTimerInverter, g_lpszParamGenICam[EDeviceParameterGenICam_TimerSequenceTimerInverter], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamGammaEnable, EDeviceGenICamGammaEnable_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_TimerSequenceTimerDelayRaw, g_lpszParamGenICam[EDeviceParameterGenICam_TimerSequenceTimerDelayRaw], _T("0"), EParameterFieldType_Edit, nullptr, _T("integer"), 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_TimerSequenceTimerDurationRaw, g_lpszParamGenICam[EDeviceParameterGenICam_TimerSequenceTimerDurationRaw], _T("0"), EParameterFieldType_Edit, nullptr, _T("integer"), 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_CounterSelector, g_lpszParamGenICam[EDeviceParameterGenICam_CounterSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamCounterSelector, EDeviceGenICamCounterSelector_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_CounterEventSource, g_lpszParamGenICam[EDeviceParameterGenICam_CounterEventSource], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamCounterEventSource, EDeviceGenICamCounterEventSource_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_CounterResetSource, g_lpszParamGenICam[EDeviceParameterGenICam_CounterResetSource], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamCounterResetSource, EDeviceGenICamCounterResetSource_Count), nullptr, 1);

		/////////////////////////////////////////////////////////
		AddParameterFieldConfigurations(EDeviceParameterGenICam_LUTControls, g_lpszParamGenICam[EDeviceParameterGenICam_LUTControls], _T("1"), EParameterFieldType_None);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_LUTEnable, g_lpszParamGenICam[EDeviceParameterGenICam_LUTEnable], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamGammaEnable, EDeviceGenICamGammaEnable_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_LUTIndex, g_lpszParamGenICam[EDeviceParameterGenICam_LUTIndex], _T("0"), EParameterFieldType_Edit, nullptr, _T("integer"), 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_LUTValue, g_lpszParamGenICam[EDeviceParameterGenICam_LUTValue], _T("0"), EParameterFieldType_Edit, nullptr, _T("integer"), 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_LUTSelector, g_lpszParamGenICam[EDeviceParameterGenICam_LUTSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGenICamLUTSelector, EDeviceGenICamLUTSelector_Count), nullptr, 1);

		/////////////////////////////////////////////////////////
		AddParameterFieldConfigurations(EDeviceParameterGenICam_TransportLayer, g_lpszParamGenICam[EDeviceParameterGenICam_TransportLayer], _T("1"), EParameterFieldType_None);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_GevLinkSpeed, g_lpszParamGenICam[EDeviceParameterGenICam_GevLinkSpeed], _T("0"), EParameterFieldType_Edit, nullptr, _T("integer"), 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_GevHeartbeatTimeout, g_lpszParamGenICam[EDeviceParameterGenICam_GevHeartbeatTimeout], _T("0"), EParameterFieldType_Edit, nullptr, _T("integer"), 1);

		AddParameterFieldConfigurations(EDeviceParameterGenICam_GevSCPSPacketSize, g_lpszParamGenICam[EDeviceParameterGenICam_GevSCPSPacketSize], _T("0"), EParameterFieldType_Edit, nullptr, _T("integer"), 1);

		/////////////////////////////////////////////////////////

		bReturn = true;
	}
	while(false);

	return bReturn & __super::LoadSettings();
}

EDeviceGrabResult CDeviceGenICam::Grab()
{
	EDeviceGrabResult eReturn = EDeviceGrabResult_UnknownError;

	CString strMessage;

	do
	{
		if(!IsInitialized())
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Ithasnotbeeninitializedyet);
			eReturn = EDeviceGrabResult_AlreadyGrabError;
			break;
		}

		if(IsLive() || !IsGrabAvailable())
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Ithasbeenlivingorgrabbing);
			eReturn = EDeviceGrabResult_AlreadyGrabError;
			break;
		}

		m_bIsGrabAvailable = false;

		m_pLiveThread = AfxBeginThread(CDeviceGenICam::CallbackFunction, this, 0, 0U, CREATE_SUSPENDED);

		if(!m_pLiveThread)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtocreatethread);
			eReturn = EDeviceGrabResult_CreateThreadError;
			break;
		}

		m_pLiveThread->ResumeThread();

		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("Grab"));

		eReturn = EDeviceGrabResult_OK;
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceLiveResult CDeviceGenICam::Live()
{
	EDeviceLiveResult eReturn = EDeviceLiveResult_UnknownError;

	CString strMessage;

	do
	{
		if(!IsInitialized())
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Ithasnotbeeninitializedyet);
			eReturn = EDeviceLiveResult_NotInitializedError;
			break;
		}

		if(IsLive() || !IsGrabAvailable())
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Ithasbeenlivingorgrabbing);
			eReturn = EDeviceLiveResult_AlreadyGrabError;
			break;
		}

		m_bIsLive = true;

		m_pLiveThread = AfxBeginThread(CDeviceGenICam::CallbackFunction, this, 0, 0U, CREATE_SUSPENDED);

		if(!m_pLiveThread)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtocreatethread);
			eReturn = EDeviceLiveResult_CreateThreadError;
			break;
		}

		m_pLiveThread->ResumeThread();

		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("Live"));

		eReturn = EDeviceLiveResult_OK;
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceStopResult CDeviceGenICam::Stop()
{
	EDeviceStopResult eReturn = EDeviceStopResult_UnknownError;

	CString strMessage;

	do
	{
		if(!m_pNodeMapDevice)
			break;

		if(!IsInitialized())
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Ithasnotbeeninitializedyet);
			eReturn = EDeviceStopResult_NotInitializedError;
			break;
		}
		if(!IsLive() && IsGrabAvailable())
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Thedevicewasalreadystop);
			eReturn = EDeviceStopResult_AlreadyStopError;
			break;
		}

		m_bIsLive = false;
		m_bIsGrabAvailable = true;

		if(m_pLiveThread)
		{
			GenApi::CCommandPtr ptrAcquisitionStop = m_pNodeMapDevice->_Ptr->GetNode("AcquisitionStop");

			ptrAcquisitionStop->Execute();
		}

		if(WaitForSingleObject(m_pLiveThread->m_hThread, 1000) == WAIT_TIMEOUT)
		{


		}

		m_pLiveThread = nullptr;

		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("Stop"));

		eReturn = EDeviceStopResult_OK;
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceTriggerResult CDeviceGenICam::Trigger()
{
	return EDeviceTriggerResult();
}

bool CDeviceGenICam::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = false;
	bool bFoundID = true;

	do
	{
		switch(nParam)
		{
		case EDeviceParameterGenICam_DeviceID:
			bReturn = !SetDeviceID(strValue);
			break;
		case EDeviceParameterGenICam_InitializeMode:
			bReturn = !SetInitializeMode(EDeviceGenICamInitMode(_ttoi(strValue)));
			break;
		case EDeviceParameterGenICam_CtiFilePath:
			bReturn = !SetCtiFilePath(strValue);
			break;
		case EDeviceParameterGenICam_GainAuto:
			bReturn = !SetGainAuto(EDeviceGenICamGainAuto(_ttoi(strValue)));
			break;
		case EDeviceParameterGenICam_GainRaw:
			bReturn = !SetGainRaw(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_GainSelector:
			bReturn = !SetGainSelector(EDeviceGenICamGainSelector(_ttoi(strValue)));
			break;
		case EDeviceParameterGenICam_BlackLevelRaw:
			bReturn = !SetBlackLevelRaw(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_BlackLevelSelector:
			bReturn = !SetBlackLevelSelector(EDeviceGenICamBlackLevelSelector(_ttoi(strValue)));
			break;
		case EDeviceParameterGenICam_BalanceWhiteAuto:
			bReturn = !SetBalanceWhiteAuto(EDeviceGenICamBalanceWhiteAuto(_ttoi(strValue)));
			break;
		case EDeviceParameterGenICam_BalanceRatioSelector:
			bReturn = !SetBalanceRatioSelector(EDeviceGenICamBalanceRatioSelector(_ttoi(strValue)));
			break;
		case EDeviceParameterGenICam_BalanceRatioAbs:
			bReturn = !SetBalanceRatioAbs(_ttof(strValue));
			break;
		case EDeviceParameterGenICam_BalanceRatioRaw:
			bReturn = !SetBalanceRatioRaw(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_GammaEnable:
			bReturn = !SetGammaEnable(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_Gamma:
			bReturn = !SetGamma(_ttof(strValue));
			break;
		case EDeviceParameterGenICam_DigitalShift:
			bReturn = !SetDigitalShift(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_PixelFormat:
			bReturn = !SetPixelFormat(EDeviceGenICamPixelFormat(_ttoi(strValue)));
			break;
		case EDeviceParameterGenICam_ReverseX:
			bReturn = !SetReverseX(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_ReverseY:
			bReturn = !SetReverseY(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_SensorWidth:
			bReturn = !SetSensorWidth(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_Width:
			bReturn = !SetWidth(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_Height:
			bReturn = !SetHeight(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_OffsetX:
			bReturn = !SetOffsetX(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_OffsetY:
			bReturn = !SetOffsetY(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_BinningVertical:
			bReturn = !SetBinningVertical(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_BinningHorizontal:
			bReturn = !SetBinningHorizontal(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_SensorHeight:
			bReturn = !SetSensorHeight(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_LegacyBinningVertical:
			bReturn = !SetLegacyBinningVertical(EDeviceGenICamLegacyBinningVertical(_ttoi(strValue)));
			break;
		case EDeviceParameterGenICam_EnableBurstAcquisition:
			bReturn = !SetEnableBurstAcquisition(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_AcquisitionMode:
			bReturn = !SetAcquisitionMode(EDeviceGenICamAcquisitionMode(_ttoi(strValue)));
			break;
		case EDeviceParameterGenICam_TriggerSelector:
			bReturn = !SetTriggerSelector(EDeviceGenICamTriggerSelector(_ttoi(strValue)));
			break;
		case EDeviceParameterGenICam_TriggerMode:
			bReturn = !SetTriggerMode(EDeviceGenICamTriggerMode(_ttoi(strValue)));
			break;
		case EDeviceParameterGenICam_TriggerSource:
			bReturn = !SetTriggerSource(EDeviceGenICamTriggerSource(_ttoi(strValue)));
			break;
		case EDeviceParameterGenICam_TriggerActivation:
			bReturn = !SetTriggerActivation(EDeviceGenICamTriggerActivation(_ttoi(strValue)));
			break;
		case EDeviceParameterGenICam_TriggerDelayAbs:
			bReturn = !SetTriggerDelayAbs(_ttof(strValue));
			break;
		case EDeviceParameterGenICam_ExposureAuto:
			bReturn = !SetExposureAuto(EDeviceGenICamExposureAuto(_ttoi(strValue)));
			break;
		case EDeviceParameterGenICam_ExposureMode:
			bReturn = !SetExposureMode(EDeviceGenICamExposureMode(_ttoi(strValue)));
			break;
		case EDeviceParameterGenICam_ExposureTimeRaw:
			bReturn = !SetExposureTimeRaw(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_ExposureTimeAbs:
			bReturn = !SetExposureTimeAbs(_ttof(strValue));
			break;
		case EDeviceParameterGenICam_ExposureTimeBaseAbs:
			bReturn = !SetExposureTimeBaseAbs(_ttof(strValue));
			break;
		case EDeviceParameterGenICam_ExposureTimeBaseAbsEnable:
			bReturn = !SetExposureTimeBaseAbsEnable(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_AcquisitionLineRateRaw:
			bReturn = !SetAcquisitionLineRateRaw(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_AcquisitionLineRateAbs:
			bReturn = !SetAcquisitionLineRateAbs(_ttof(strValue));
			break;
		case EDeviceParameterGenICam_ResultingLineRateAbs:
			bReturn = !SetResultingLineRateAbs(_ttof(strValue));
			break;
		case EDeviceParameterGenICam_AcquisitionFrameRate:
			bReturn = !SetAcquisitionFrameRate(_ttof(strValue));
			break;
		case EDeviceParameterGenICam_AcquisitionFrameRateEnable:
			bReturn = !SetAcquisitionFrameRateEnable(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_ResultingFrameRateAbs:
			bReturn = !SetResultingFrameRateAbs(_ttof(strValue));
			break;
		case EDeviceParameterGenICam_AcquisitionStatusSelector:
			bReturn = !SetAcquisitionStatusSelector(EDeviceGenICamAcquisitionStatusSelector(_ttoi(strValue)));
			break;
		case EDeviceParameterGenICam_AcquisitionStatus:
			bReturn = !SetAcquisitionStatus(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_LineSelector:
			bReturn = !SetLineSelector(EDeviceGenICamLineSelector(_ttoi(strValue)));
			break;
		case EDeviceParameterGenICam_Line1LineMode:
			bReturn = !SetLineMode_Line1(EDeviceGenICamLineMode(_ttoi(strValue)));
			break;
		case EDeviceParameterGenICam_Line1LineSource:
			bReturn = !SetLineSource_Line1(EDeviceGenICamLineSource(_ttoi(strValue)));
			break;
		case EDeviceParameterGenICam_Line1LineFormat:
			bReturn = !SetLineFormat_Line1(EDeviceGenICamLineFormat(_ttoi(strValue)));
			break;
		case EDeviceParameterGenICam_Line1LineInverter:
			bReturn = !SetLineInverter_Line1(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_Line1LineTermination:
			bReturn = !SetLineTermination_Line1(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_Line1LineDebouncerTimeRaw:
			bReturn = !SetLineDebouncerTimeRaw_Line1(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_Line1LineDebouncerTimeAbs:
			bReturn = !SetLineDebouncerTimeAbs_Line1(_ttof(strValue));
			break;
		case EDeviceParameterGenICam_Line2LineMode:
			bReturn = !SetLineMode_Line2(EDeviceGenICamLineMode(_ttoi(strValue)));
			break;
		case EDeviceParameterGenICam_Line2LineSource:
			bReturn = !SetLineSource_Line2(EDeviceGenICamLineSource(_ttoi(strValue)));
			break;
		case EDeviceParameterGenICam_Line2LineFormat:
			bReturn = !SetLineFormat_Line2(EDeviceGenICamLineFormat(_ttoi(strValue)));
			break;
		case EDeviceParameterGenICam_Line2LineInverter:
			bReturn = !SetLineInverter_Line2(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_Line2LineTermination:
			bReturn = !SetLineTermination_Line2(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_Line2LineDebouncerTimeRaw:
			bReturn = !SetLineDebouncerTimeRaw_Line2(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_Line2LineDebouncerTimeAbs:
			bReturn = !SetLineDebouncerTimeAbs_Line2(_ttof(strValue));
			break;
		case EDeviceParameterGenICam_Out1LineMode:
			bReturn = !SetLineMode_Out1(EDeviceGenICamLineMode(_ttoi(strValue)));
			break;
		case EDeviceParameterGenICam_Out1LineSource:
			bReturn = !SetLineSource_Out1(EDeviceGenICamLineSource(_ttoi(strValue)));
			break;
		case EDeviceParameterGenICam_Out1LineFormat:
			bReturn = !SetLineFormat_Out1(EDeviceGenICamLineFormat(_ttoi(strValue)));
			break;
		case EDeviceParameterGenICam_Out1LineInverter:
			bReturn = !SetLineInverter_Out1(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_Out1LineTermination:
			bReturn = !SetLineTermination_Out1(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_Out1LineDebouncerTimeRaw:
			bReturn = !SetLineDebouncerTimeRaw_Out1(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_Out1LineDebouncerTimeAbs:
			bReturn = !SetLineDebouncerTimeAbs_Out1(_ttof(strValue));
			break;
		case EDeviceParameterGenICam_Out2LineMode:
			bReturn = !SetLineMode_Out2(EDeviceGenICamLineMode(_ttoi(strValue)));
			break;
		case EDeviceParameterGenICam_Out2LineSource:
			bReturn = !SetLineSource_Out2(EDeviceGenICamLineSource(_ttoi(strValue)));
			break;
		case EDeviceParameterGenICam_Out2LineFormat:
			bReturn = !SetLineFormat_Out2(EDeviceGenICamLineFormat(_ttoi(strValue)));
			break;
		case EDeviceParameterGenICam_Out2LineInverter:
			bReturn = !SetLineInverter_Out2(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_Out2LineTermination:
			bReturn = !SetLineTermination_Out2(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_Out2LineDebouncerTimeRaw:
			bReturn = !SetLineDebouncerTimeRaw_Out2(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_Out2LineDebouncerTimeAbs:
			bReturn = !SetLineDebouncerTimeAbs_Out2(_ttof(strValue));
			break;
		case EDeviceParameterGenICam_Out3LineMode:
			bReturn = !SetLineMode_Out3(EDeviceGenICamLineMode(_ttoi(strValue)));
			break;
		case EDeviceParameterGenICam_Out3LineSource:
			bReturn = !SetLineSource_Out3(EDeviceGenICamLineSource(_ttoi(strValue)));
			break;
		case EDeviceParameterGenICam_Out3LineFormat:
			bReturn = !SetLineFormat_Out3(EDeviceGenICamLineFormat(_ttoi(strValue)));
			break;
		case EDeviceParameterGenICam_Out3LineInverter:
			bReturn = !SetLineInverter_Out3(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_Out3LineTermination:
			bReturn = !SetLineTermination_Out3(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_Out3LineDebouncerTimeRaw:
			bReturn = !SetLineDebouncerTimeRaw_Out3(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_Out3LineDebouncerTimeAbs:
			bReturn = !SetLineDebouncerTimeAbs_Out3(_ttof(strValue));
			break;
		case EDeviceParameterGenICam_Out4LineMode:
			bReturn = !SetLineMode_Out4(EDeviceGenICamLineMode(_ttoi(strValue)));
			break;
		case EDeviceParameterGenICam_Out4LineSource:
			bReturn = !SetLineSource_Out4(EDeviceGenICamLineSource(_ttoi(strValue)));
			break;
		case EDeviceParameterGenICam_Out4LineFormat:
			bReturn = !SetLineFormat_Out4(EDeviceGenICamLineFormat(_ttoi(strValue)));
			break;
		case EDeviceParameterGenICam_Out4LineInverter:
			bReturn = !SetLineInverter_Out4(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_Out4LineTermination:
			bReturn = !SetLineTermination_Out4(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_Out4LineDebouncerTimeRaw:
			bReturn = !SetLineDebouncerTimeRaw_Out4(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_Out4LineDebouncerTimeAbs:
			bReturn = !SetLineDebouncerTimeAbs_Out4(_ttof(strValue));
			break;
		case EDeviceParameterGenICam_UserOutputSelector:
			bReturn = !SetUserOutputSelector(EDeviceGenICamUserOutputSelector(_ttoi(strValue)));
			break;
		case EDeviceParameterGenICam_TimerDurationTimebaseAbs:
			bReturn = !SetTimerDurationTimebaseAbs(_ttof(strValue));
			break;
		case EDeviceParameterGenICam_TimerDelayTimebaseAbs:
			bReturn = !SetTimerDelayTimebaseAbs(_ttof(strValue));
			break;
		case EDeviceParameterGenICam_TimerSelector:
			bReturn = !SetTimerSelector(EDeviceGenICamTimerSelector(_ttoi(strValue)));
			break;
		case EDeviceParameterGenICam_TimerDurationAbs:
			bReturn = !SetTimerDurationAbs(_ttof(strValue));
			break;
		case EDeviceParameterGenICam_TimerDurationRaw:
			bReturn = !SetTimerDurationRaw(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_TimerDelayAbs:
			bReturn = !SetTimerDelayAbs(_ttof(strValue));
			break;
		case EDeviceParameterGenICam_TimerDelayRaw:
			bReturn = !SetTimerDelayRaw(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_TimerTriggerSource:
			bReturn = !SetTimerTriggerSource(EDeviceGenICamTimerTriggerSource(_ttoi(strValue)));
			break;
		case EDeviceParameterGenICam_TimerTriggerActivation:
			bReturn = !SetTimerTriggerActivation(EDeviceGenICamTimerTriggerActivation(_ttoi(strValue)));
			break;
		case EDeviceParameterGenICam_TimerSequenceEnable:
			bReturn = !SetTimerSequenceEnable(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_TimerSequenceLastEntryIndex:
			bReturn = !SetTimerSequenceLastEntryIndex(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_TimerSequenceCurrentEntryIndex:
			bReturn = !SetTimerSequenceCurrentEntryIndex(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_TimerSequenceEntrySelector:
			bReturn = !SetTimerSequenceEntrySelector(EDeviceGenICamTimerSequenceEntrySelector(_ttoi(strValue)));
			break;
		case EDeviceParameterGenICam_TimerSequenceTimerSelector:
			bReturn = !SetTimerSequenceTimerSelector(EDeviceGenICamTimerSequenceTimerSelector(_ttoi(strValue)));
			break;
		case EDeviceParameterGenICam_TimerSequenceTimerEnable:
			bReturn = !SetTimerSequenceTimerEnable(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_TimerSequenceTimerInverter:
			bReturn = !SetTimerSequenceTimerInverter(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_TimerSequenceTimerDelayRaw:
			bReturn = !SetTimerSequenceTimerDelayRaw(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_TimerSequenceTimerDurationRaw:
			bReturn = !SetTimerSequenceTimerDurationRaw(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_CounterSelector:
			bReturn = !SetCounterSelector(EDeviceGenICamCounterSelector(_ttoi(strValue)));
			break;
		case EDeviceParameterGenICam_CounterEventSource:
			bReturn = !SetCounterEventSource(EDeviceGenICamCounterEventSource(_ttoi(strValue)));
			break;
		case EDeviceParameterGenICam_CounterResetSource:
			bReturn = !SetCounterResetSource(EDeviceGenICamCounterResetSource(_ttoi(strValue)));
			break;
		case EDeviceParameterGenICam_LUTEnable:
			bReturn = !SetLUTEnable(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_LUTIndex:
			bReturn = !SetLUTIndex(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_LUTValue:
			bReturn = !SetLUTValue(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_LUTSelector:
			bReturn = !SetLUTSelector(EDeviceGenICamLUTSelector(_ttoi(strValue)));
			break;
		case EDeviceParameterGenICam_GevLinkSpeed:
			bReturn = !SetGevLinkSpeed(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_GevHeartbeatTimeout:
			bReturn = !SetGevHeartbeatTimeout(_ttoi(strValue));
			break;
		case EDeviceParameterGenICam_GevSCPSPacketSize:
			bReturn = !SetGevSCPSPacketSize(_ttoi(strValue));
			break;
		default:
			bFoundID = false;
			break;
		}
	}
	while(false);

	if(!bFoundID)
	{
		CString strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtofindtheparameter);

		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);
	}

	return bReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetCtiFilePath(CString * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = GetParamValue(EDeviceParameterGenICam_CtiFilePath);

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetCtiFilePath(CString strParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eCtiFilePath = EDeviceParameterGenICam_CtiFilePath;

	CString strPreValue = GetParamValue(eCtiFilePath);

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;
			break;
		}

		if(!SetParamValue(eCtiFilePath, strParam))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eCtiFilePath))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eCtiFilePath], strPreValue, strParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetGainAuto(EDeviceGenICamGainAuto * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGenICamGainAuto)_ttoi(GetParamValue(EDeviceParameterGenICam_GainAuto));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetGainAuto(EDeviceGenICamGainAuto eParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSaveID = EDeviceParameterGenICam_GainAuto;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGenICamGainAuto_Count)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				CStringA strGainAuto(g_lpszGenICamGainAuto[eParam]);

				GenApi::CEnumerationPtr ptrGainAuto = m_pNodeMapDevice->_GetNode("GainAuto");

				if(ptrGainAuto.IsValid() && GenApi::IsWritable(ptrGainAuto))
				{
					GENICAM_NAMESPACE::gcstring gcstr(strGainAuto);

					ptrGainAuto->FromString(gcstr);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSaveID], g_lpszGenICamGainAuto[nPreValue], g_lpszGenICamGainAuto[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetGainRaw(int * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_GainRaw));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetGainRaw(int nParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eGainRaw = EDeviceParameterGenICam_GainRaw;

	int nPreValue = _ttoi(GetParamValue(eGainRaw));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CIntegerPtr ptrGainRaw = m_pNodeMapDevice->_GetNode("GainRaw");

				if(ptrGainRaw.IsValid() && GenApi::IsWritable(ptrGainRaw))
				{
					ptrGainRaw->SetValue(nParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eGainRaw, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eGainRaw))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eGainRaw], nPreValue, nParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetGainSelector(EDeviceGenICamGainSelector * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGenICamGainSelector)_ttoi(GetParamValue(EDeviceParameterGenICam_GainSelector));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetGainSelector(EDeviceGenICamGainSelector eParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSaveID = EDeviceParameterGenICam_GainSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGenICamGainSelector_Count)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				CStringA strGainSelector(g_lpszGenICamGainSelector[eParam]);

				GenApi::CEnumerationPtr ptrGainSelector = m_pNodeMapDevice->_GetNode("GainSelector");

				if(ptrGainSelector.IsValid() && GenApi::IsWritable(ptrGainSelector))
				{
					GENICAM_NAMESPACE::gcstring gcstr(strGainSelector);

					ptrGainSelector->FromString(gcstr);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSaveID], g_lpszGenICamGainSelector[nPreValue], g_lpszGenICamGainSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetBlackLevelRaw(int * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_BlackLevelRaw));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetBlackLevelRaw(int nParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eBlackLevelRaw = EDeviceParameterGenICam_BlackLevelRaw;

	int nPreValue = _ttoi(GetParamValue(eBlackLevelRaw));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CIntegerPtr ptrBlackLevelRaw = m_pNodeMapDevice->_GetNode("BlackLevelRaw");

				if(ptrBlackLevelRaw.IsValid() && GenApi::IsWritable(ptrBlackLevelRaw))
				{
					ptrBlackLevelRaw->SetValue(nParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eBlackLevelRaw, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eBlackLevelRaw))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eBlackLevelRaw], nPreValue, nParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetBlackLevelSelector(EDeviceGenICamBlackLevelSelector * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGenICamBlackLevelSelector)_ttoi(GetParamValue(EDeviceParameterGenICam_BlackLevelSelector));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetBlackLevelSelector(EDeviceGenICamBlackLevelSelector eParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSaveID = EDeviceParameterGenICam_BlackLevelSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGenICamBlackLevelSelector_Count)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				CStringA strBlackLevelSelector(g_lpszGenICamBlackLevelSelector[eParam]);

				GenApi::CEnumerationPtr ptrBlackLevelSelector = m_pNodeMapDevice->_GetNode("BlackLevelSelector");

				if(ptrBlackLevelSelector.IsValid() && GenApi::IsWritable(ptrBlackLevelSelector))
				{
					GENICAM_NAMESPACE::gcstring gcstr(strBlackLevelSelector);

					ptrBlackLevelSelector->FromString(gcstr);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSaveID], g_lpszGenICamBlackLevelSelector[nPreValue], g_lpszGenICamBlackLevelSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetBalanceWhiteAuto(EDeviceGenICamBalanceWhiteAuto * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGenICamBalanceWhiteAuto)_ttoi(GetParamValue(EDeviceParameterGenICam_BalanceWhiteAuto));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetBalanceWhiteAuto(EDeviceGenICamBalanceWhiteAuto eParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSaveID = EDeviceParameterGenICam_BalanceWhiteAuto;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGenICamBalanceWhiteAuto_Count)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				CStringA strBalanceWhiteAuto(g_lpszGenICamBalanceWhiteAuto[eParam]);

				GenApi::CEnumerationPtr ptrBalanceWhiteAuto = m_pNodeMapDevice->_GetNode("BalanceWhiteAuto");

				if(ptrBalanceWhiteAuto.IsValid() && GenApi::IsWritable(ptrBalanceWhiteAuto))
				{
					GENICAM_NAMESPACE::gcstring gcstr(strBalanceWhiteAuto);

					ptrBalanceWhiteAuto->FromString(gcstr);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSaveID], g_lpszGenICamBalanceWhiteAuto[nPreValue], g_lpszGenICamBalanceWhiteAuto[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetBalanceRatioSelector(EDeviceGenICamBalanceRatioSelector * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGenICamBalanceRatioSelector)_ttoi(GetParamValue(EDeviceParameterGenICam_BalanceRatioSelector));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetBalanceRatioSelector(EDeviceGenICamBalanceRatioSelector eParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSaveID = EDeviceParameterGenICam_BalanceRatioSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGenICamBalanceRatioSelector_Count)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				CStringA strBalanceRatioSelector(g_lpszGenICamBalanceRatioSelector[eParam]);

				GenApi::CEnumerationPtr ptrBalanceRatioSelector = m_pNodeMapDevice->_GetNode("BalanceRatioSelector");

				if(ptrBalanceRatioSelector.IsValid() && GenApi::IsWritable(ptrBalanceRatioSelector))
				{
					GENICAM_NAMESPACE::gcstring gcstr(strBalanceRatioSelector);

					ptrBalanceRatioSelector->FromString(gcstr);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSaveID], g_lpszGenICamBalanceRatioSelector[nPreValue], g_lpszGenICamBalanceRatioSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetBalanceRatioAbs(float * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterGenICam_BalanceRatioAbs));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetBalanceRatioAbs(float fParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eBalanceRatioAbs = EDeviceParameterGenICam_BalanceRatioAbs;

	float fPreValue = _ttof(GetParamValue(eBalanceRatioAbs));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CFloatPtr ptrBalanceRatioAbs = m_pNodeMapDevice->_GetNode("BalanceRatioAbs");

				if(ptrBalanceRatioAbs.IsValid() && GenApi::IsWritable(ptrBalanceRatioAbs))
				{
					ptrBalanceRatioAbs->SetValue(fParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), fParam);

		if(!SetParamValue(eBalanceRatioAbs, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eBalanceRatioAbs))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eBalanceRatioAbs], fPreValue, fParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetBalanceRatioRaw(int * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_BalanceRatioRaw));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetBalanceRatioRaw(int nParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eBalanceRatioRaw = EDeviceParameterGenICam_BalanceRatioRaw;

	int nPreValue = _ttoi(GetParamValue(eBalanceRatioRaw));

	do
	{
		if(IsInitialized())
		{
			try
			{
				GenApi::CIntegerPtr ptrBalanceRatioRaw = m_pNodeMapDevice->_GetNode("BalanceRatioRaw");

				if(ptrBalanceRatioRaw.IsValid() && GenApi::IsWritable(ptrBalanceRatioRaw))
				{
					ptrBalanceRatioRaw->SetValue(nParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eBalanceRatioRaw, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eBalanceRatioRaw))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eBalanceRatioRaw], nPreValue, nParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetGammaEnable(bool * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_GammaEnable));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetGammaEnable(bool bParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eGammaEnable = EDeviceParameterGenICam_GammaEnable;

	int nPreValue = _ttoi(GetParamValue(eGammaEnable));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CBooleanPtr ptrGammaEnable = m_pNodeMapDevice->_GetNode("GammaEnable");

				if(ptrGammaEnable.IsValid() && GenApi::IsWritable(ptrGammaEnable))
				{
					ptrGammaEnable->SetValue(bParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eGammaEnable, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eGammaEnable))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eGammaEnable], g_lpszGenICamGammaEnable[nPreValue], g_lpszGenICamGammaEnable[bParam]);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetGamma(float * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterGenICam_Gamma));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetGamma(float fParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eGamma = EDeviceParameterGenICam_Gamma;

	float fPreValue = _ttof(GetParamValue(eGamma));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CFloatPtr ptrGamma = m_pNodeMapDevice->_GetNode("Gamma");

				if(ptrGamma.IsValid() && GenApi::IsWritable(ptrGamma))
				{
					ptrGamma->SetValue(fParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), fParam);

		if(!SetParamValue(eGamma, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eGamma))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eGamma], fPreValue, fParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetDigitalShift(int * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_DigitalShift));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetDigitalShift(int nParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eDigitalShift = EDeviceParameterGenICam_DigitalShift;

	int nPreValue = _ttoi(GetParamValue(eDigitalShift));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CIntegerPtr ptrDigitalShift = m_pNodeMapDevice->_GetNode("DigitalShift");

				if(ptrDigitalShift.IsValid() && GenApi::IsWritable(ptrDigitalShift))
				{
					ptrDigitalShift->SetValue(nParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eDigitalShift, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eDigitalShift))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eDigitalShift], nPreValue, nParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetPixelFormat(EDeviceGenICamPixelFormat * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGenICamPixelFormat)_ttoi(GetParamValue(EDeviceParameterGenICam_PixelFormat));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetPixelFormat(EDeviceGenICamPixelFormat eParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSaveID = EDeviceParameterGenICam_PixelFormat;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;
			break;
		}

		if(eParam < 0 || eParam >= EDeviceGenICamPixelFormat_R8)
		{
			eReturn = EDeviceGenICamSetFunction_NotSupportError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSaveID], g_lpszGenICamPixelFormat[nPreValue], g_lpszGenICamPixelFormat[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetReverseX(bool * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_ReverseX));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetReverseX(bool bParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eReverseX = EDeviceParameterGenICam_ReverseX;

	int nPreValue = _ttoi(GetParamValue(eReverseX));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;
			break;
		}

		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eReverseX, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eReverseX))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eReverseX], g_lpszGenICamGammaEnable[nPreValue], g_lpszGenICamGammaEnable[bParam]);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetReverseY(bool * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_ReverseY));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetReverseY(bool bParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eReverseY = EDeviceParameterGenICam_ReverseY;

	int nPreValue = _ttoi(GetParamValue(eReverseY));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;
			break;
		}

		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eReverseY, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eReverseY))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eReverseY], g_lpszGenICamGammaEnable[nPreValue], g_lpszGenICamGammaEnable[bParam]);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetSensorWidth(int * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_SensorWidth));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetSensorWidth(int nParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSensorWidth = EDeviceParameterGenICam_SensorWidth;

	int nPreValue = _ttoi(GetParamValue(eSensorWidth));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;
			break;
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eSensorWidth, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSensorWidth))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSensorWidth], nPreValue, nParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetWidth(int * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_Width));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetWidth(int nParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eWidth = EDeviceParameterGenICam_Width;

	int nPreValue = _ttoi(GetParamValue(eWidth));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;
			break;
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eWidth, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eWidth))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eWidth], nPreValue, nParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetHeight(int * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_Height));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetHeight(int nParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eHeight = EDeviceParameterGenICam_Height;

	int nPreValue = _ttoi(GetParamValue(eHeight));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;
			break;
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eHeight, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eHeight))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eHeight], nPreValue, nParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetOffsetX(int * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_OffsetX));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetOffsetX(int nParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eOffsetX = EDeviceParameterGenICam_OffsetX;

	int nPreValue = _ttoi(GetParamValue(eOffsetX));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;
			break;
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eOffsetX, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eOffsetX))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eOffsetX], nPreValue, nParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetOffsetY(int * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_OffsetY));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetOffsetY(int nParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eOffsetY = EDeviceParameterGenICam_OffsetY;

	int nPreValue = _ttoi(GetParamValue(eOffsetY));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;
			break;
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eOffsetY, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eOffsetY))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eOffsetY], nPreValue, nParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetBinningVertical(int * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_BinningVertical));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetBinningVertical(int nParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eBinningVertical = EDeviceParameterGenICam_BinningVertical;

	int nPreValue = _ttoi(GetParamValue(eBinningVertical));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;
			break;
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eBinningVertical, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eBinningVertical))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eBinningVertical], nPreValue, nParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetBinningHorizontal(int * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_BinningHorizontal));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetBinningHorizontal(int nParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eBinningHorizontal = EDeviceParameterGenICam_BinningHorizontal;

	int nPreValue = _ttoi(GetParamValue(eBinningHorizontal));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;
			break;
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eBinningHorizontal, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eBinningHorizontal))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eBinningHorizontal], nPreValue, nParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetSensorHeight(int * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_SensorHeight));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetSensorHeight(int nParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSensorHeight = EDeviceParameterGenICam_SensorHeight;

	int nPreValue = _ttoi(GetParamValue(eSensorHeight));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;
			break;
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eSensorHeight, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSensorHeight))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSensorHeight], nPreValue, nParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLegacyBinningVertical(EDeviceGenICamLegacyBinningVertical * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGenICamLegacyBinningVertical)_ttoi(GetParamValue(EDeviceParameterGenICam_LegacyBinningVertical));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLegacyBinningVertical(EDeviceGenICamLegacyBinningVertical eParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSaveID = EDeviceParameterGenICam_LegacyBinningVertical;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;
			break;
		}

		if(eParam < 0 || eParam >= EDeviceGenICamLegacyBinningVertical_Count)
		{
			eReturn = EDeviceGenICamSetFunction_UnknownError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSaveID], g_lpszGenICamLegacyBinningVertical[nPreValue], g_lpszGenICamLegacyBinningVertical[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetEnableBurstAcquisition(bool * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_EnableBurstAcquisition));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetEnableBurstAcquisition(bool bParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eEnableBurstAcquisition = EDeviceParameterGenICam_EnableBurstAcquisition;

	int nPreValue = _ttoi(GetParamValue(eEnableBurstAcquisition));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CBooleanPtr ptrEnableBurstAcquisition = m_pNodeMapDevice->_GetNode("EnableBurstAcquisition");

				if(ptrEnableBurstAcquisition.IsValid() && GenApi::IsWritable(ptrEnableBurstAcquisition))
				{
					ptrEnableBurstAcquisition->SetValue(bParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eEnableBurstAcquisition, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eEnableBurstAcquisition))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eEnableBurstAcquisition], g_lpszGenICamGammaEnable[nPreValue], g_lpszGenICamGammaEnable[bParam]);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetAcquisitionMode(EDeviceGenICamAcquisitionMode * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGenICamAcquisitionMode)_ttoi(GetParamValue(EDeviceParameterGenICam_AcquisitionMode));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetAcquisitionMode(EDeviceGenICamAcquisitionMode eParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSaveID = EDeviceParameterGenICam_AcquisitionMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGenICamAcquisitionMode_Count)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				CStringA strAcquisitionMode(g_lpszGenICamAcquisitionMode[eParam]);

				GenApi::CEnumerationPtr ptrAcquisitionMode = m_pNodeMapDevice->_GetNode("AcquisitionMode");

				if(ptrAcquisitionMode.IsValid() && GenApi::IsWritable(ptrAcquisitionMode))
				{
					GENICAM_NAMESPACE::gcstring gcstr(strAcquisitionMode);

					ptrAcquisitionMode->FromString(gcstr);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSaveID], g_lpszGenICamAcquisitionMode[nPreValue], g_lpszGenICamAcquisitionMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetTriggerSelector(EDeviceGenICamTriggerSelector * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGenICamTriggerSelector)_ttoi(GetParamValue(EDeviceParameterGenICam_TriggerSelector));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetTriggerSelector(EDeviceGenICamTriggerSelector eParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSaveID = EDeviceParameterGenICam_TriggerSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGenICamTriggerSelector_Count)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				CStringA strTriggerSelector(g_lpszGenICamTriggerSelector[eParam]);

				GenApi::CEnumerationPtr ptrTriggerSelector = m_pNodeMapDevice->_GetNode("TriggerSelector");

				if(ptrTriggerSelector.IsValid() && GenApi::IsWritable(ptrTriggerSelector))
				{
					GENICAM_NAMESPACE::gcstring gcstr(strTriggerSelector);

					ptrTriggerSelector->FromString(gcstr);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSaveID], g_lpszGenICamTriggerSelector[nPreValue], g_lpszGenICamTriggerSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetTriggerMode(EDeviceGenICamTriggerMode * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGenICamTriggerMode)_ttoi(GetParamValue(EDeviceParameterGenICam_TriggerMode));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetTriggerMode(EDeviceGenICamTriggerMode eParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSaveID = EDeviceParameterGenICam_TriggerMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGenICamTriggerMode_Count)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				CStringA strTriggerMode(g_lpszGenICamTriggerMode[eParam]);

				GenApi::CEnumerationPtr ptrTriggerMode = m_pNodeMapDevice->_GetNode("TriggerMode");

				if(ptrTriggerMode.IsValid() && GenApi::IsWritable(ptrTriggerMode))
				{
					GENICAM_NAMESPACE::gcstring gcstr(strTriggerMode);

					ptrTriggerMode->FromString(gcstr);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSaveID], g_lpszGenICamTriggerMode[nPreValue], g_lpszGenICamTriggerMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetTriggerSource(EDeviceGenICamTriggerSource * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGenICamTriggerSource)_ttoi(GetParamValue(EDeviceParameterGenICam_TriggerSource));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetTriggerSource(EDeviceGenICamTriggerSource eParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSaveID = EDeviceParameterGenICam_TriggerSource;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGenICamTriggerSource_Count)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				CStringA strTriggerSource(g_lpszGenICamTriggerSource[eParam]);

				GenApi::CEnumerationPtr ptrTriggerSource = m_pNodeMapDevice->_GetNode("TriggerSource");

				if(ptrTriggerSource.IsValid() && GenApi::IsWritable(ptrTriggerSource))
				{
					GENICAM_NAMESPACE::gcstring gcstr(strTriggerSource);

					ptrTriggerSource->FromString(gcstr);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSaveID], g_lpszGenICamTriggerSource[nPreValue], g_lpszGenICamTriggerSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetTriggerActivation(EDeviceGenICamTriggerActivation * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGenICamTriggerActivation)_ttoi(GetParamValue(EDeviceParameterGenICam_TriggerActivation));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetTriggerActivation(EDeviceGenICamTriggerActivation eParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSaveID = EDeviceParameterGenICam_TriggerActivation;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGenICamTriggerActivation_Count)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				CStringA strTriggerActivation(g_lpszGenICamTriggerActivation[eParam]);

				GenApi::CEnumerationPtr ptrTriggerActivation = m_pNodeMapDevice->_GetNode("TriggerActivation");

				if(ptrTriggerActivation.IsValid() && GenApi::IsWritable(ptrTriggerActivation))
				{
					GENICAM_NAMESPACE::gcstring gcstr(strTriggerActivation);

					ptrTriggerActivation->FromString(gcstr);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSaveID], g_lpszGenICamTriggerActivation[nPreValue], g_lpszGenICamTriggerActivation[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetTriggerDelayAbs(float * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterGenICam_TriggerDelayAbs));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetTriggerDelayAbs(float fParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eTriggerDelayAbs = EDeviceParameterGenICam_TriggerDelayAbs;

	float fPreValue = _ttof(GetParamValue(eTriggerDelayAbs));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CFloatPtr ptrTriggerDelayAbs = m_pNodeMapDevice->_GetNode("TriggerDelayAbs");

				if(ptrTriggerDelayAbs.IsValid() && GenApi::IsWritable(ptrTriggerDelayAbs))
				{
					ptrTriggerDelayAbs->SetValue(fParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), fParam);

		if(!SetParamValue(eTriggerDelayAbs, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eTriggerDelayAbs))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eTriggerDelayAbs], fPreValue, fParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetExposureAuto(EDeviceGenICamExposureAuto * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGenICamExposureAuto)_ttoi(GetParamValue(EDeviceParameterGenICam_ExposureAuto));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetExposureAuto(EDeviceGenICamExposureAuto eParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSaveID = EDeviceParameterGenICam_ExposureAuto;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGenICamExposureAuto_Count)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				CStringA strExposureAuto(g_lpszGenICamExposureAuto[eParam]);

				GenApi::CEnumerationPtr ptrExposureAuto = m_pNodeMapDevice->_GetNode("ExposureAuto");

				if(ptrExposureAuto.IsValid() && GenApi::IsWritable(ptrExposureAuto))
				{
					GENICAM_NAMESPACE::gcstring gcstr(strExposureAuto);

					ptrExposureAuto->FromString(gcstr);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSaveID], g_lpszGenICamExposureAuto[nPreValue], g_lpszGenICamExposureAuto[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetExposureMode(EDeviceGenICamExposureMode * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGenICamExposureMode)_ttoi(GetParamValue(EDeviceParameterGenICam_ExposureMode));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetExposureMode(EDeviceGenICamExposureMode eParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSaveID = EDeviceParameterGenICam_ExposureMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGenICamExposureMode_Count)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				CStringA strExposureMode(g_lpszGenICamExposureMode[eParam]);

				GenApi::CEnumerationPtr ptrExposureMode = m_pNodeMapDevice->_GetNode("ExposureMode");

				if(ptrExposureMode.IsValid() && GenApi::IsWritable(ptrExposureMode))
				{
					GENICAM_NAMESPACE::gcstring gcstr(strExposureMode);

					ptrExposureMode->FromString(gcstr);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSaveID], g_lpszGenICamExposureMode[nPreValue], g_lpszGenICamExposureMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetExposureTimeRaw(int * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_ExposureTimeRaw));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetExposureTimeRaw(int nParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eExposureTimeRaw = EDeviceParameterGenICam_ExposureTimeRaw;

	int nPreValue = _ttoi(GetParamValue(eExposureTimeRaw));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CIntegerPtr ptrExposureTimeRaw = m_pNodeMapDevice->_GetNode("ExposureTimeRaw");

				if(ptrExposureTimeRaw.IsValid() && GenApi::IsWritable(ptrExposureTimeRaw))
				{
					ptrExposureTimeRaw->SetValue(nParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eExposureTimeRaw, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eExposureTimeRaw))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eExposureTimeRaw], nPreValue, nParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetExposureTimeAbs(float * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterGenICam_ExposureTimeAbs));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetExposureTimeAbs(float fParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eExposureTimeAbs = EDeviceParameterGenICam_ExposureTimeAbs;

	float fPreValue = _ttof(GetParamValue(eExposureTimeAbs));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CFloatPtr ptrExposureTimeAbs = m_pNodeMapDevice->_GetNode("ExposureTimeAbs");

				if(ptrExposureTimeAbs.IsValid() && GenApi::IsWritable(ptrExposureTimeAbs))
				{
					ptrExposureTimeAbs->SetValue(fParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), fParam);

		if(!SetParamValue(eExposureTimeAbs, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eExposureTimeAbs))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eExposureTimeAbs], fPreValue, fParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetExposureTimeBaseAbs(float * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterGenICam_ExposureTimeBaseAbs));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetExposureTimeBaseAbs(float fParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eExposureTimeBaseAbs = EDeviceParameterGenICam_ExposureTimeBaseAbs;

	float fPreValue = _ttof(GetParamValue(eExposureTimeBaseAbs));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CFloatPtr ptrExposureTimeBaseAbs = m_pNodeMapDevice->_GetNode("ExposureTimeBaseAbs");

				if(ptrExposureTimeBaseAbs.IsValid() && GenApi::IsWritable(ptrExposureTimeBaseAbs))
				{
					ptrExposureTimeBaseAbs->SetValue(fParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), fParam);

		if(!SetParamValue(eExposureTimeBaseAbs, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eExposureTimeBaseAbs))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eExposureTimeBaseAbs], fPreValue, fParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetExposureTimeBaseAbsEnable(bool * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_ExposureTimeBaseAbsEnable));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetExposureTimeBaseAbsEnable(bool bParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eExposureTimeBaseAbsEnable = EDeviceParameterGenICam_ExposureTimeBaseAbsEnable;

	int nPreValue = _ttoi(GetParamValue(eExposureTimeBaseAbsEnable));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CBooleanPtr ptrExposureTimeBaseAbsEnable = m_pNodeMapDevice->_GetNode("ExposureTimeBaseAbsEnable");

				if(ptrExposureTimeBaseAbsEnable.IsValid() && GenApi::IsWritable(ptrExposureTimeBaseAbsEnable))
				{
					ptrExposureTimeBaseAbsEnable->SetValue(bParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eExposureTimeBaseAbsEnable, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eExposureTimeBaseAbsEnable))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eExposureTimeBaseAbsEnable], g_lpszGenICamGammaEnable[nPreValue], g_lpszGenICamGammaEnable[bParam]);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetAcquisitionLineRateRaw(int * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_AcquisitionLineRateRaw));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetAcquisitionLineRateRaw(int nParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eAcquisitionLineRateRaw = EDeviceParameterGenICam_AcquisitionLineRateRaw;

	int nPreValue = _ttoi(GetParamValue(eAcquisitionLineRateRaw));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CIntegerPtr ptrAcquisitionLineRateRaw = m_pNodeMapDevice->_GetNode("AcquisitionLineRateRaw");

				if(ptrAcquisitionLineRateRaw.IsValid() && GenApi::IsWritable(ptrAcquisitionLineRateRaw))
				{
					ptrAcquisitionLineRateRaw->SetValue(nParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eAcquisitionLineRateRaw, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eAcquisitionLineRateRaw))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eAcquisitionLineRateRaw], nPreValue, nParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetAcquisitionLineRateAbs(float * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterGenICam_AcquisitionLineRateAbs));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetAcquisitionLineRateAbs(float fParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eAcquisitionLineRateAbs = EDeviceParameterGenICam_AcquisitionLineRateAbs;

	float fPreValue = _ttof(GetParamValue(eAcquisitionLineRateAbs));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CFloatPtr ptrAcquisitionLineRateAbs = m_pNodeMapDevice->_GetNode("AcquisitionLineRateAbs");

				if(ptrAcquisitionLineRateAbs.IsValid() && GenApi::IsWritable(ptrAcquisitionLineRateAbs))
				{
					ptrAcquisitionLineRateAbs->SetValue(fParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), fParam);

		if(!SetParamValue(eAcquisitionLineRateAbs, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eAcquisitionLineRateAbs))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eAcquisitionLineRateAbs], fPreValue, fParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetResultingLineRateAbs(float * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterGenICam_ResultingLineRateAbs));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetResultingLineRateAbs(float fParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eResultingLineRateAbs = EDeviceParameterGenICam_ResultingLineRateAbs;

	float fPreValue = _ttof(GetParamValue(eResultingLineRateAbs));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CFloatPtr ptrResultingLineRateAbs = m_pNodeMapDevice->_GetNode("ResultingLineRateAbs");

				if(ptrResultingLineRateAbs.IsValid() && GenApi::IsWritable(ptrResultingLineRateAbs))
				{
					ptrResultingLineRateAbs->SetValue(fParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), fParam);

		if(!SetParamValue(eResultingLineRateAbs, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eResultingLineRateAbs))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eResultingLineRateAbs], fPreValue, fParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetAcquisitionFrameRate(float * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterGenICam_AcquisitionFrameRate));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetAcquisitionFrameRate(float fParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eAcquisitionFrameRate = EDeviceParameterGenICam_AcquisitionFrameRate;

	float fPreValue = _ttof(GetParamValue(eAcquisitionFrameRate));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CFloatPtr ptrAcquisitionFrameRate = m_pNodeMapDevice->_GetNode("AcquisitionFrameRate");

				if(ptrAcquisitionFrameRate.IsValid() && GenApi::IsWritable(ptrAcquisitionFrameRate))
				{
					ptrAcquisitionFrameRate->SetValue(fParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), fParam);

		if(!SetParamValue(eAcquisitionFrameRate, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eAcquisitionFrameRate))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eAcquisitionFrameRate], fPreValue, fParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetAcquisitionFrameRateEnable(bool * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_AcquisitionFrameRateEnable));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetAcquisitionFrameRateEnable(bool bParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eAcquisitionFrameRateEnable = EDeviceParameterGenICam_AcquisitionFrameRateEnable;

	int nPreValue = _ttoi(GetParamValue(eAcquisitionFrameRateEnable));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CBooleanPtr ptrAcquisitionFrameRateEnable = m_pNodeMapDevice->_GetNode("AcquisitionFrameRateEnable");

				if(ptrAcquisitionFrameRateEnable.IsValid() && GenApi::IsWritable(ptrAcquisitionFrameRateEnable))
				{
					ptrAcquisitionFrameRateEnable->SetValue(bParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eAcquisitionFrameRateEnable, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eAcquisitionFrameRateEnable))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eAcquisitionFrameRateEnable], g_lpszGenICamGammaEnable[nPreValue], g_lpszGenICamGammaEnable[bParam]);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetResultingFrameRateAbs(float * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterGenICam_ResultingFrameRateAbs));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetResultingFrameRateAbs(float fParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eResultingFrameRateAbs = EDeviceParameterGenICam_ResultingFrameRateAbs;

	float fPreValue = _ttof(GetParamValue(eResultingFrameRateAbs));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CFloatPtr ptrResultingFrameRateAbs = m_pNodeMapDevice->_GetNode("ResultingFrameRateAbs");

				if(ptrResultingFrameRateAbs.IsValid() && GenApi::IsWritable(ptrResultingFrameRateAbs))
				{
					ptrResultingFrameRateAbs->SetValue(fParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), fParam);

		if(!SetParamValue(eResultingFrameRateAbs, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eResultingFrameRateAbs))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eResultingFrameRateAbs], fPreValue, fParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetAcquisitionStatusSelector(EDeviceGenICamAcquisitionStatusSelector * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGenICamAcquisitionStatusSelector)_ttoi(GetParamValue(EDeviceParameterGenICam_AcquisitionStatusSelector));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetAcquisitionStatusSelector(EDeviceGenICamAcquisitionStatusSelector eParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSaveID = EDeviceParameterGenICam_AcquisitionStatusSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGenICamAcquisitionStatusSelector_Count)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				CStringA strAcquisitionStatusSelector(g_lpszGenICamAcquisitionStatusSelector[eParam]);

				GenApi::CEnumerationPtr ptrAcquisitionStatusSelector = m_pNodeMapDevice->_GetNode("AcquisitionStatusSelector");

				if(ptrAcquisitionStatusSelector.IsValid() && GenApi::IsWritable(ptrAcquisitionStatusSelector))
				{
					GENICAM_NAMESPACE::gcstring gcstr(strAcquisitionStatusSelector);

					ptrAcquisitionStatusSelector->FromString(gcstr);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSaveID], g_lpszGenICamAcquisitionStatusSelector[nPreValue], g_lpszGenICamAcquisitionStatusSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetAcquisitionStatus(bool * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_AcquisitionStatus));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetAcquisitionStatus(bool bParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eAcquisitionStatus = EDeviceParameterGenICam_AcquisitionStatus;

	int nPreValue = _ttoi(GetParamValue(eAcquisitionStatus));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CBooleanPtr ptrAcquisitionStatus = m_pNodeMapDevice->_GetNode("AcquisitionStatus");

				if(ptrAcquisitionStatus.IsValid() && GenApi::IsWritable(ptrAcquisitionStatus))
				{
					ptrAcquisitionStatus->SetValue(bParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eAcquisitionStatus, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eAcquisitionStatus))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eAcquisitionStatus], g_lpszGenICamGammaEnable[nPreValue], g_lpszGenICamGammaEnable[bParam]);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLineSelector(EDeviceGenICamLineSelector * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGenICamLineSelector)_ttoi(GetParamValue(EDeviceParameterGenICam_LineSelector));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLineSelector(EDeviceGenICamLineSelector eParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSaveID = EDeviceParameterGenICam_LineSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGenICamLineSelector_Count)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				CStringA strLineSelector(g_lpszGenICamLineSelector[eParam]);

				GenApi::CEnumerationPtr ptrLineSelector = m_pNodeMapDevice->_GetNode("LineSelector");

				if(ptrLineSelector.IsValid() && GenApi::IsWritable(ptrLineSelector))
				{
					GENICAM_NAMESPACE::gcstring gcstr(strLineSelector);

					ptrLineSelector->FromString(gcstr);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSaveID], g_lpszGenICamLineSelector[nPreValue], g_lpszGenICamLineSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLineMode_Line1(EDeviceGenICamLineMode * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGenICamLineMode)_ttoi(GetParamValue(EDeviceParameterGenICam_Line1LineMode));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLineMode_Line1(EDeviceGenICamLineMode eParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSaveID = EDeviceParameterGenICam_Line1LineMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGenICamLineMode_Count)
			break;

		EDeviceGenICamLineSelector eLineSelector;
		GetLineSelector(&eLineSelector);

		if(eLineSelector != EDeviceGenICamLineSelector_Line1)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				CStringA strLineMode(g_lpszGenICamLineMode[eParam]);

				GenApi::CEnumerationPtr ptrLineMode = m_pNodeMapDevice->_GetNode("LineMode");

				if(ptrLineMode.IsValid() && GenApi::IsWritable(ptrLineMode))
				{
					GENICAM_NAMESPACE::gcstring gcstr(strLineMode);

					ptrLineMode->FromString(gcstr);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSaveID], g_lpszGenICamLineMode[nPreValue], g_lpszGenICamLineMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLineSource_Line1(EDeviceGenICamLineSource * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGenICamLineSource)_ttoi(GetParamValue(EDeviceParameterGenICam_Line1LineSource));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLineSource_Line1(EDeviceGenICamLineSource eParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSaveID = EDeviceParameterGenICam_Line1LineSource;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGenICamLineSource_Count)
			break;

		EDeviceGenICamLineSelector eLineSelector;
		GetLineSelector(&eLineSelector);

		if(eLineSelector != EDeviceGenICamLineSelector_Line1)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				CStringA strLineSource(g_lpszGenICamLineSource[eParam]);

				GenApi::CEnumerationPtr ptrLineSource = m_pNodeMapDevice->_GetNode("LineSource");

				if(ptrLineSource.IsValid() && GenApi::IsWritable(ptrLineSource))
				{
					GENICAM_NAMESPACE::gcstring gcstr(strLineSource);

					ptrLineSource->FromString(gcstr);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSaveID], g_lpszGenICamLineSource[nPreValue], g_lpszGenICamLineSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLineFormat_Line1(EDeviceGenICamLineFormat * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGenICamLineFormat)_ttoi(GetParamValue(EDeviceParameterGenICam_Line1LineFormat));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLineFormat_Line1(EDeviceGenICamLineFormat eParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSaveID = EDeviceParameterGenICam_Line1LineFormat;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGenICamLineFormat_Count)
			break;

		EDeviceGenICamLineSelector eLineSelector;
		GetLineSelector(&eLineSelector);

		if(eLineSelector != EDeviceGenICamLineSelector_Line1)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				CStringA strLineFormat(g_lpszGenICamLineFormat[eParam]);

				GenApi::CEnumerationPtr ptrLineFormat = m_pNodeMapDevice->_GetNode("LineFormat");

				if(ptrLineFormat.IsValid() && GenApi::IsWritable(ptrLineFormat))
				{
					GENICAM_NAMESPACE::gcstring gcstr(strLineFormat);

					ptrLineFormat->FromString(gcstr);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSaveID], g_lpszGenICamLineFormat[nPreValue], g_lpszGenICamLineFormat[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLineInverter_Line1(bool * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_Line1LineInverter));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLineInverter_Line1(bool bParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eLine1LineInverter = EDeviceParameterGenICam_Line1LineInverter;

	int nPreValue = _ttoi(GetParamValue(eLine1LineInverter));

	do
	{
		EDeviceGenICamLineSelector eLineSelector;
		GetLineSelector(&eLineSelector);

		if(eLineSelector != EDeviceGenICamLineSelector_Line1)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CBooleanPtr ptrLineInverter = m_pNodeMapDevice->_GetNode("LineInverter");

				if(ptrLineInverter.IsValid() && GenApi::IsWritable(ptrLineInverter))
				{
					ptrLineInverter->SetValue(bParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eLine1LineInverter, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eLine1LineInverter))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eLine1LineInverter], g_lpszGenICamGammaEnable[nPreValue], g_lpszGenICamGammaEnable[bParam]);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLineTermination_Line1(bool * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_Line1LineTermination));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLineTermination_Line1(bool bParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eLine1LineTermination = EDeviceParameterGenICam_Line1LineTermination;

	int nPreValue = _ttoi(GetParamValue(eLine1LineTermination));

	do
	{
		EDeviceGenICamLineSelector eLineSelector;
		GetLineSelector(&eLineSelector);

		if(eLineSelector != EDeviceGenICamLineSelector_Line1)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CBooleanPtr ptrLineTermination = m_pNodeMapDevice->_GetNode("LineTermination");

				if(ptrLineTermination.IsValid() && GenApi::IsWritable(ptrLineTermination))
				{
					ptrLineTermination->SetValue(bParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eLine1LineTermination, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eLine1LineTermination))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eLine1LineTermination], g_lpszGenICamGammaEnable[nPreValue], g_lpszGenICamGammaEnable[bParam]);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLineDebouncerTimeRaw_Line1(int * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_Line1LineDebouncerTimeRaw));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLineDebouncerTimeRaw_Line1(int nParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eLine1LineDebouncerTimeRaw = EDeviceParameterGenICam_Line1LineDebouncerTimeRaw;

	int nPreValue = _ttoi(GetParamValue(eLine1LineDebouncerTimeRaw));

	do
	{
		EDeviceGenICamLineSelector eLineSelector;
		GetLineSelector(&eLineSelector);

		if(eLineSelector != EDeviceGenICamLineSelector_Line1)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CIntegerPtr ptrLineDebouncerTimeRaw = m_pNodeMapDevice->_GetNode("LineDebouncerTimeRaw");

				if(ptrLineDebouncerTimeRaw.IsValid() && GenApi::IsWritable(ptrLineDebouncerTimeRaw))
				{
					ptrLineDebouncerTimeRaw->SetValue(nParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eLine1LineDebouncerTimeRaw, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eLine1LineDebouncerTimeRaw))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eLine1LineDebouncerTimeRaw], nPreValue, nParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLineDebouncerTimeAbs_Line1(float * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterGenICam_Line1LineDebouncerTimeAbs));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLineDebouncerTimeAbs_Line1(float fParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eLine1LineDebouncerTimeAbs = EDeviceParameterGenICam_Line1LineDebouncerTimeAbs;

	float fPreValue = _ttof(GetParamValue(eLine1LineDebouncerTimeAbs));

	do
	{
		EDeviceGenICamLineSelector eLineSelector;
		GetLineSelector(&eLineSelector);

		if(eLineSelector != EDeviceGenICamLineSelector_Line1)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CFloatPtr ptrLineDebouncerTimeAbs = m_pNodeMapDevice->_GetNode("LineDebouncerTimeAbs");

				if(ptrLineDebouncerTimeAbs.IsValid() && GenApi::IsWritable(ptrLineDebouncerTimeAbs))
				{
					ptrLineDebouncerTimeAbs->SetValue(fParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), fParam);

		if(!SetParamValue(eLine1LineDebouncerTimeAbs, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eLine1LineDebouncerTimeAbs))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eLine1LineDebouncerTimeAbs], fPreValue, fParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLineMode_Line2(EDeviceGenICamLineMode * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGenICamLineMode)_ttoi(GetParamValue(EDeviceParameterGenICam_Line2LineMode));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLineMode_Line2(EDeviceGenICamLineMode eParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSaveID = EDeviceParameterGenICam_Line2LineMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGenICamLineMode_Count)
			break;

		EDeviceGenICamLineSelector eLineSelector;
		GetLineSelector(&eLineSelector);

		if(eLineSelector != EDeviceGenICamLineSelector_Line2)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				CStringA strLineMode(g_lpszGenICamLineMode[eParam]);

				GenApi::CEnumerationPtr ptrLineMode = m_pNodeMapDevice->_GetNode("LineMode");

				if(ptrLineMode.IsValid() && GenApi::IsWritable(ptrLineMode))
				{
					GENICAM_NAMESPACE::gcstring gcstr(strLineMode);

					ptrLineMode->FromString(gcstr);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSaveID], g_lpszGenICamLineMode[nPreValue], g_lpszGenICamLineMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLineSource_Line2(EDeviceGenICamLineSource * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGenICamLineSource)_ttoi(GetParamValue(EDeviceParameterGenICam_Line2LineSource));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLineSource_Line2(EDeviceGenICamLineSource eParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSaveID = EDeviceParameterGenICam_Line2LineSource;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGenICamLineSource_Count)
			break;

		EDeviceGenICamLineSelector eLineSelector;
		GetLineSelector(&eLineSelector);

		if(eLineSelector != EDeviceGenICamLineSelector_Line2)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				CStringA strLineSource(g_lpszGenICamLineSource[eParam]);

				GenApi::CEnumerationPtr ptrLineSource = m_pNodeMapDevice->_GetNode("LineSource");

				if(ptrLineSource.IsValid() && GenApi::IsWritable(ptrLineSource))
				{
					GENICAM_NAMESPACE::gcstring gcstr(strLineSource);

					ptrLineSource->FromString(gcstr);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSaveID], g_lpszGenICamLineSource[nPreValue], g_lpszGenICamLineSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLineFormat_Line2(EDeviceGenICamLineFormat * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGenICamLineFormat)_ttoi(GetParamValue(EDeviceParameterGenICam_Line2LineFormat));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLineFormat_Line2(EDeviceGenICamLineFormat eParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSaveID = EDeviceParameterGenICam_Line2LineFormat;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGenICamLineFormat_Count)
			break;

		EDeviceGenICamLineSelector eLineSelector;
		GetLineSelector(&eLineSelector);

		if(eLineSelector != EDeviceGenICamLineSelector_Line2)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				CStringA strLineFormat(g_lpszGenICamLineFormat[eParam]);

				GenApi::CEnumerationPtr ptrLineFormat = m_pNodeMapDevice->_GetNode("LineFormat");

				if(ptrLineFormat.IsValid() && GenApi::IsWritable(ptrLineFormat))
				{
					GENICAM_NAMESPACE::gcstring gcstr(strLineFormat);

					ptrLineFormat->FromString(gcstr);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSaveID], g_lpszGenICamLineFormat[nPreValue], g_lpszGenICamLineFormat[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLineInverter_Line2(bool * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_Line2LineInverter));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLineInverter_Line2(bool bParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eLine2LineInverter = EDeviceParameterGenICam_Line2LineInverter;

	int nPreValue = _ttoi(GetParamValue(eLine2LineInverter));

	do
	{
		EDeviceGenICamLineSelector eLineSelector;
		GetLineSelector(&eLineSelector);

		if(eLineSelector != EDeviceGenICamLineSelector_Line2)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CBooleanPtr ptrLineInverter = m_pNodeMapDevice->_GetNode("LineInverter");

				if(ptrLineInverter.IsValid() && GenApi::IsWritable(ptrLineInverter))
				{
					ptrLineInverter->SetValue(bParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eLine2LineInverter, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eLine2LineInverter))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eLine2LineInverter], g_lpszGenICamGammaEnable[nPreValue], g_lpszGenICamGammaEnable[bParam]);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLineTermination_Line2(bool * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_Line2LineTermination));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLineTermination_Line2(bool bParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eLine2LineTermination = EDeviceParameterGenICam_Line2LineTermination;

	int nPreValue = _ttoi(GetParamValue(eLine2LineTermination));

	do
	{
		EDeviceGenICamLineSelector eLineSelector;
		GetLineSelector(&eLineSelector);

		if(eLineSelector != EDeviceGenICamLineSelector_Line2)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CBooleanPtr ptrLineTermination = m_pNodeMapDevice->_GetNode("LineTermination");

				if(ptrLineTermination.IsValid() && GenApi::IsWritable(ptrLineTermination))
				{
					ptrLineTermination->SetValue(bParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eLine2LineTermination, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eLine2LineTermination))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eLine2LineTermination], g_lpszGenICamGammaEnable[nPreValue], g_lpszGenICamGammaEnable[bParam]);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLineDebouncerTimeRaw_Line2(int * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_Line2LineDebouncerTimeRaw));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLineDebouncerTimeRaw_Line2(int nParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eLine2LineDebouncerTimeRaw = EDeviceParameterGenICam_Line2LineDebouncerTimeRaw;

	int nPreValue = _ttoi(GetParamValue(eLine2LineDebouncerTimeRaw));

	do
	{
		EDeviceGenICamLineSelector eLineSelector;
		GetLineSelector(&eLineSelector);

		if(eLineSelector != EDeviceGenICamLineSelector_Line2)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CIntegerPtr ptrLineDebouncerTimeRaw = m_pNodeMapDevice->_GetNode("LineDebouncerTimeRaw");

				if(ptrLineDebouncerTimeRaw.IsValid() && GenApi::IsWritable(ptrLineDebouncerTimeRaw))
				{
					ptrLineDebouncerTimeRaw->SetValue(nParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eLine2LineDebouncerTimeRaw, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eLine2LineDebouncerTimeRaw))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eLine2LineDebouncerTimeRaw], nPreValue, nParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLineDebouncerTimeAbs_Line2(float * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterGenICam_Line2LineDebouncerTimeAbs));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLineDebouncerTimeAbs_Line2(float fParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eLine2LineDebouncerTimeAbs = EDeviceParameterGenICam_Line2LineDebouncerTimeAbs;

	float fPreValue = _ttof(GetParamValue(eLine2LineDebouncerTimeAbs));

	do
	{
		EDeviceGenICamLineSelector eLineSelector;
		GetLineSelector(&eLineSelector);

		if(eLineSelector != EDeviceGenICamLineSelector_Line2)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CFloatPtr ptrLineDebouncerTimeAbs = m_pNodeMapDevice->_GetNode("LineDebouncerTimeAbs");

				if(ptrLineDebouncerTimeAbs.IsValid() && GenApi::IsWritable(ptrLineDebouncerTimeAbs))
				{
					ptrLineDebouncerTimeAbs->SetValue(fParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), fParam);

		if(!SetParamValue(eLine2LineDebouncerTimeAbs, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eLine2LineDebouncerTimeAbs))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eLine2LineDebouncerTimeAbs], fPreValue, fParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLineMode_Out1(EDeviceGenICamLineMode * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGenICamLineMode)_ttoi(GetParamValue(EDeviceParameterGenICam_Out1LineMode));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLineMode_Out1(EDeviceGenICamLineMode eParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSaveID = EDeviceParameterGenICam_Out1LineMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGenICamLineMode_Count)
			break;

		EDeviceGenICamLineSelector eLineSelector;
		GetLineSelector(&eLineSelector);

		if(eLineSelector != EDeviceGenICamLineSelector_Out1)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				CStringA strLineMode(g_lpszGenICamLineMode[eParam]);

				GenApi::CEnumerationPtr ptrLineMode = m_pNodeMapDevice->_GetNode("LineMode");

				if(ptrLineMode.IsValid() && GenApi::IsWritable(ptrLineMode))
				{
					GENICAM_NAMESPACE::gcstring gcstr(strLineMode);

					ptrLineMode->FromString(gcstr);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSaveID], g_lpszGenICamLineMode[nPreValue], g_lpszGenICamLineMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLineSource_Out1(EDeviceGenICamLineSource * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGenICamLineSource)_ttoi(GetParamValue(EDeviceParameterGenICam_Out1LineSource));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLineSource_Out1(EDeviceGenICamLineSource eParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSaveID = EDeviceParameterGenICam_Out1LineSource;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGenICamLineSource_Count)
			break;

		EDeviceGenICamLineSelector eLineSelector;
		GetLineSelector(&eLineSelector);

		if(eLineSelector != EDeviceGenICamLineSelector_Out1)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				CStringA strLineSource(g_lpszGenICamLineSource[eParam]);

				GenApi::CEnumerationPtr ptrLineSource = m_pNodeMapDevice->_GetNode("LineSource");

				if(ptrLineSource.IsValid() && GenApi::IsWritable(ptrLineSource))
				{
					GENICAM_NAMESPACE::gcstring gcstr(strLineSource);

					ptrLineSource->FromString(gcstr);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSaveID], g_lpszGenICamLineSource[nPreValue], g_lpszGenICamLineSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLineFormat_Out1(EDeviceGenICamLineFormat * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGenICamLineFormat)_ttoi(GetParamValue(EDeviceParameterGenICam_Out1LineFormat));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLineFormat_Out1(EDeviceGenICamLineFormat eParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSaveID = EDeviceParameterGenICam_Out1LineFormat;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGenICamLineFormat_Count)
			break;

		EDeviceGenICamLineSelector eLineSelector;
		GetLineSelector(&eLineSelector);

		if(eLineSelector != EDeviceGenICamLineSelector_Out1)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				CStringA strLineFormat(g_lpszGenICamLineFormat[eParam]);

				GenApi::CEnumerationPtr ptrLineFormat = m_pNodeMapDevice->_GetNode("LineFormat");

				if(ptrLineFormat.IsValid() && GenApi::IsWritable(ptrLineFormat))
				{
					GENICAM_NAMESPACE::gcstring gcstr(strLineFormat);

					ptrLineFormat->FromString(gcstr);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSaveID], g_lpszGenICamLineFormat[nPreValue], g_lpszGenICamLineFormat[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLineInverter_Out1(bool * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_Out1LineInverter));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLineInverter_Out1(bool bParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eOut1LineInverter = EDeviceParameterGenICam_Out1LineInverter;

	int nPreValue = _ttoi(GetParamValue(eOut1LineInverter));

	do
	{
		EDeviceGenICamLineSelector eLineSelector;
		GetLineSelector(&eLineSelector);

		if(eLineSelector != EDeviceGenICamLineSelector_Out1)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CBooleanPtr ptrLineInverter = m_pNodeMapDevice->_GetNode("LineInverter");

				if(ptrLineInverter.IsValid() && GenApi::IsWritable(ptrLineInverter))
				{
					ptrLineInverter->SetValue(bParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eOut1LineInverter, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eOut1LineInverter))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eOut1LineInverter], g_lpszGenICamGammaEnable[nPreValue], g_lpszGenICamGammaEnable[bParam]);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLineTermination_Out1(bool * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_Out1LineTermination));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLineTermination_Out1(bool bParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eOut1LineTermination = EDeviceParameterGenICam_Out1LineTermination;

	int nPreValue = _ttoi(GetParamValue(eOut1LineTermination));

	do
	{
		EDeviceGenICamLineSelector eLineSelector;
		GetLineSelector(&eLineSelector);

		if(eLineSelector != EDeviceGenICamLineSelector_Out1)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CBooleanPtr ptrLineTermination = m_pNodeMapDevice->_GetNode("LineTermination");

				if(ptrLineTermination.IsValid() && GenApi::IsWritable(ptrLineTermination))
				{
					ptrLineTermination->SetValue(bParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eOut1LineTermination, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eOut1LineTermination))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eOut1LineTermination], g_lpszGenICamGammaEnable[nPreValue], g_lpszGenICamGammaEnable[bParam]);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLineDebouncerTimeRaw_Out1(int * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_Out1LineDebouncerTimeRaw));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLineDebouncerTimeRaw_Out1(int nParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eOut1LineDebouncerTimeRaw = EDeviceParameterGenICam_Out1LineDebouncerTimeRaw;

	int nPreValue = _ttoi(GetParamValue(eOut1LineDebouncerTimeRaw));

	do
	{
		EDeviceGenICamLineSelector eLineSelector;
		GetLineSelector(&eLineSelector);

		if(eLineSelector != EDeviceGenICamLineSelector_Out1)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CIntegerPtr ptrLineDebouncerTimeRaw = m_pNodeMapDevice->_GetNode("LineDebouncerTimeRaw");

				if(ptrLineDebouncerTimeRaw.IsValid() && GenApi::IsWritable(ptrLineDebouncerTimeRaw))
				{
					ptrLineDebouncerTimeRaw->SetValue(nParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eOut1LineDebouncerTimeRaw, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eOut1LineDebouncerTimeRaw))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eOut1LineDebouncerTimeRaw], nPreValue, nParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLineDebouncerTimeAbs_Out1(float * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterGenICam_Out1LineDebouncerTimeAbs));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLineDebouncerTimeAbs_Out1(float fParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eOut1LineDebouncerTimeAbs = EDeviceParameterGenICam_Out1LineDebouncerTimeAbs;

	float fPreValue = _ttof(GetParamValue(eOut1LineDebouncerTimeAbs));

	do
	{
		EDeviceGenICamLineSelector eLineSelector;
		GetLineSelector(&eLineSelector);

		if(eLineSelector != EDeviceGenICamLineSelector_Out1)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CFloatPtr ptrLineDebouncerTimeAbs = m_pNodeMapDevice->_GetNode("LineDebouncerTimeAbs");

				if(ptrLineDebouncerTimeAbs.IsValid() && GenApi::IsWritable(ptrLineDebouncerTimeAbs))
				{
					ptrLineDebouncerTimeAbs->SetValue(fParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), fParam);

		if(!SetParamValue(eOut1LineDebouncerTimeAbs, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eOut1LineDebouncerTimeAbs))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eOut1LineDebouncerTimeAbs], fPreValue, fParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLineMode_Out2(EDeviceGenICamLineMode * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGenICamLineMode)_ttoi(GetParamValue(EDeviceParameterGenICam_Out2LineMode));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLineMode_Out2(EDeviceGenICamLineMode eParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSaveID = EDeviceParameterGenICam_Out2LineMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGenICamLineMode_Count)
			break;

		EDeviceGenICamLineSelector eLineSelector;
		GetLineSelector(&eLineSelector);

		if(eLineSelector != EDeviceGenICamLineSelector_Out2)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				CStringA strLineMode(g_lpszGenICamLineMode[eParam]);

				GenApi::CEnumerationPtr ptrLineMode = m_pNodeMapDevice->_GetNode("LineMode");

				if(ptrLineMode.IsValid() && GenApi::IsWritable(ptrLineMode))
				{
					GENICAM_NAMESPACE::gcstring gcstr(strLineMode);

					ptrLineMode->FromString(gcstr);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSaveID], g_lpszGenICamLineMode[nPreValue], g_lpszGenICamLineMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLineSource_Out2(EDeviceGenICamLineSource * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGenICamLineSource)_ttoi(GetParamValue(EDeviceParameterGenICam_Out2LineSource));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLineSource_Out2(EDeviceGenICamLineSource eParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSaveID = EDeviceParameterGenICam_Out2LineSource;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGenICamLineSource_Count)
			break;

		EDeviceGenICamLineSelector eLineSelector;
		GetLineSelector(&eLineSelector);

		if(eLineSelector != EDeviceGenICamLineSelector_Out2)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				CStringA strLineSource(g_lpszGenICamLineSource[eParam]);

				GenApi::CEnumerationPtr ptrLineSource = m_pNodeMapDevice->_GetNode("LineSource");

				if(ptrLineSource.IsValid() && GenApi::IsWritable(ptrLineSource))
				{
					GENICAM_NAMESPACE::gcstring gcstr(strLineSource);

					ptrLineSource->FromString(gcstr);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSaveID], g_lpszGenICamLineSource[nPreValue], g_lpszGenICamLineSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLineFormat_Out2(EDeviceGenICamLineFormat * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGenICamLineFormat)_ttoi(GetParamValue(EDeviceParameterGenICam_Out2LineFormat));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLineFormat_Out2(EDeviceGenICamLineFormat eParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSaveID = EDeviceParameterGenICam_Out2LineFormat;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGenICamLineFormat_Count)
			break;

		EDeviceGenICamLineSelector eLineSelector;
		GetLineSelector(&eLineSelector);

		if(eLineSelector != EDeviceGenICamLineSelector_Out2)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				CStringA strLineFormat(g_lpszGenICamLineFormat[eParam]);

				GenApi::CEnumerationPtr ptrLineFormat = m_pNodeMapDevice->_GetNode("LineFormat");

				if(ptrLineFormat.IsValid() && GenApi::IsWritable(ptrLineFormat))
				{
					GENICAM_NAMESPACE::gcstring gcstr(strLineFormat);

					ptrLineFormat->FromString(gcstr);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSaveID], g_lpszGenICamLineFormat[nPreValue], g_lpszGenICamLineFormat[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLineInverter_Out2(bool * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_Out2LineInverter));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLineInverter_Out2(bool bParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eOut2LineInverter = EDeviceParameterGenICam_Out2LineInverter;

	int nPreValue = _ttoi(GetParamValue(eOut2LineInverter));

	do
	{
		EDeviceGenICamLineSelector eLineSelector;
		GetLineSelector(&eLineSelector);

		if(eLineSelector != EDeviceGenICamLineSelector_Out2)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CBooleanPtr ptrLineInverter = m_pNodeMapDevice->_GetNode("LineInverter");

				if(ptrLineInverter.IsValid() && GenApi::IsWritable(ptrLineInverter))
				{
					ptrLineInverter->SetValue(bParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eOut2LineInverter, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eOut2LineInverter))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eOut2LineInverter], g_lpszGenICamGammaEnable[nPreValue], g_lpszGenICamGammaEnable[bParam]);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLineTermination_Out2(bool * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_Out2LineTermination));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLineTermination_Out2(bool bParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eOut2LineTermination = EDeviceParameterGenICam_Out2LineTermination;

	int nPreValue = _ttoi(GetParamValue(eOut2LineTermination));

	do
	{
		EDeviceGenICamLineSelector eLineSelector;
		GetLineSelector(&eLineSelector);

		if(eLineSelector != EDeviceGenICamLineSelector_Out2)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CBooleanPtr ptrLineTermination = m_pNodeMapDevice->_GetNode("LineTermination");

				if(ptrLineTermination.IsValid() && GenApi::IsWritable(ptrLineTermination))
				{
					ptrLineTermination->SetValue(bParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eOut2LineTermination, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eOut2LineTermination))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eOut2LineTermination], g_lpszGenICamGammaEnable[nPreValue], g_lpszGenICamGammaEnable[bParam]);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLineDebouncerTimeRaw_Out2(int * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_Out2LineDebouncerTimeRaw));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLineDebouncerTimeRaw_Out2(int nParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eOut2LineDebouncerTimeRaw = EDeviceParameterGenICam_Out2LineDebouncerTimeRaw;

	int nPreValue = _ttoi(GetParamValue(eOut2LineDebouncerTimeRaw));

	do
	{
		EDeviceGenICamLineSelector eLineSelector;
		GetLineSelector(&eLineSelector);

		if(eLineSelector != EDeviceGenICamLineSelector_Out2)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CIntegerPtr ptrLineDebouncerTimeRaw = m_pNodeMapDevice->_GetNode("LineDebouncerTimeRaw");

				if(ptrLineDebouncerTimeRaw.IsValid() && GenApi::IsWritable(ptrLineDebouncerTimeRaw))
				{
					ptrLineDebouncerTimeRaw->SetValue(nParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eOut2LineDebouncerTimeRaw, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eOut2LineDebouncerTimeRaw))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eOut2LineDebouncerTimeRaw], nPreValue, nParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLineDebouncerTimeAbs_Out2(float * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterGenICam_Out2LineDebouncerTimeAbs));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLineDebouncerTimeAbs_Out2(float fParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eOut2LineDebouncerTimeAbs = EDeviceParameterGenICam_Out2LineDebouncerTimeAbs;

	float fPreValue = _ttof(GetParamValue(eOut2LineDebouncerTimeAbs));

	do
	{
		EDeviceGenICamLineSelector eLineSelector;
		GetLineSelector(&eLineSelector);

		if(eLineSelector != EDeviceGenICamLineSelector_Out2)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CFloatPtr ptrLineDebouncerTimeAbs = m_pNodeMapDevice->_GetNode("LineDebouncerTimeAbs");

				if(ptrLineDebouncerTimeAbs.IsValid() && GenApi::IsWritable(ptrLineDebouncerTimeAbs))
				{
					ptrLineDebouncerTimeAbs->SetValue(fParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), fParam);

		if(!SetParamValue(eOut2LineDebouncerTimeAbs, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eOut2LineDebouncerTimeAbs))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eOut2LineDebouncerTimeAbs], fPreValue, fParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLineMode_Out3(EDeviceGenICamLineMode * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGenICamLineMode)_ttoi(GetParamValue(EDeviceParameterGenICam_Out3LineMode));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLineMode_Out3(EDeviceGenICamLineMode eParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSaveID = EDeviceParameterGenICam_Out3LineMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGenICamLineMode_Count)
			break;

		EDeviceGenICamLineSelector eLineSelector;
		GetLineSelector(&eLineSelector);

		if(eLineSelector != EDeviceGenICamLineSelector_Out3)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				CStringA strLineMode(g_lpszGenICamLineMode[eParam]);

				GenApi::CEnumerationPtr ptrLineMode = m_pNodeMapDevice->_GetNode("LineMode");

				if(ptrLineMode.IsValid() && GenApi::IsWritable(ptrLineMode))
				{
					GENICAM_NAMESPACE::gcstring gcstr(strLineMode);

					ptrLineMode->FromString(gcstr);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSaveID], g_lpszGenICamLineMode[nPreValue], g_lpszGenICamLineMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLineSource_Out3(EDeviceGenICamLineSource * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGenICamLineSource)_ttoi(GetParamValue(EDeviceParameterGenICam_Out3LineSource));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLineSource_Out3(EDeviceGenICamLineSource eParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSaveID = EDeviceParameterGenICam_Out3LineSource;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGenICamLineSource_Count)
			break;

		EDeviceGenICamLineSelector eLineSelector;
		GetLineSelector(&eLineSelector);

		if(eLineSelector != EDeviceGenICamLineSelector_Out3)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				CStringA strLineSource(g_lpszGenICamLineSource[eParam]);

				GenApi::CEnumerationPtr ptrLineSource = m_pNodeMapDevice->_GetNode("LineSource");

				if(ptrLineSource.IsValid() && GenApi::IsWritable(ptrLineSource))
				{
					GENICAM_NAMESPACE::gcstring gcstr(strLineSource);

					ptrLineSource->FromString(gcstr);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSaveID], g_lpszGenICamLineSource[nPreValue], g_lpszGenICamLineSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLineFormat_Out3(EDeviceGenICamLineFormat * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGenICamLineFormat)_ttoi(GetParamValue(EDeviceParameterGenICam_Out3LineFormat));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLineFormat_Out3(EDeviceGenICamLineFormat eParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSaveID = EDeviceParameterGenICam_Out3LineFormat;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGenICamLineFormat_Count)
			break;

		EDeviceGenICamLineSelector eLineSelector;
		GetLineSelector(&eLineSelector);

		if(eLineSelector != EDeviceGenICamLineSelector_Out3)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				CStringA strLineFormat(g_lpszGenICamLineFormat[eParam]);

				GenApi::CEnumerationPtr ptrLineFormat = m_pNodeMapDevice->_GetNode("LineFormat");

				if(ptrLineFormat.IsValid() && GenApi::IsWritable(ptrLineFormat))
				{
					GENICAM_NAMESPACE::gcstring gcstr(strLineFormat);

					ptrLineFormat->FromString(gcstr);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSaveID], g_lpszGenICamLineFormat[nPreValue], g_lpszGenICamLineFormat[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLineInverter_Out3(bool * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_Out3LineInverter));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLineInverter_Out3(bool bParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eOut3LineInverter = EDeviceParameterGenICam_Out3LineInverter;

	int nPreValue = _ttoi(GetParamValue(eOut3LineInverter));

	do
	{
		EDeviceGenICamLineSelector eLineSelector;
		GetLineSelector(&eLineSelector);

		if(eLineSelector != EDeviceGenICamLineSelector_Out3)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CBooleanPtr ptrLineInverter = m_pNodeMapDevice->_GetNode("LineInverter");

				if(ptrLineInverter.IsValid() && GenApi::IsWritable(ptrLineInverter))
				{
					ptrLineInverter->SetValue(bParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eOut3LineInverter, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eOut3LineInverter))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eOut3LineInverter], g_lpszGenICamGammaEnable[nPreValue], g_lpszGenICamGammaEnable[bParam]);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLineTermination_Out3(bool * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_Out3LineTermination));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLineTermination_Out3(bool bParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eOut3LineTermination = EDeviceParameterGenICam_Out3LineTermination;

	int nPreValue = _ttoi(GetParamValue(eOut3LineTermination));

	do
	{
		EDeviceGenICamLineSelector eLineSelector;
		GetLineSelector(&eLineSelector);

		if(eLineSelector != EDeviceGenICamLineSelector_Out3)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CBooleanPtr ptrLineTermination = m_pNodeMapDevice->_GetNode("LineTermination");

				if(ptrLineTermination.IsValid() && GenApi::IsWritable(ptrLineTermination))
				{
					ptrLineTermination->SetValue(bParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eOut3LineTermination, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eOut3LineTermination))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eOut3LineTermination], g_lpszGenICamGammaEnable[nPreValue], g_lpszGenICamGammaEnable[bParam]);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLineDebouncerTimeRaw_Out3(int * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_Out3LineDebouncerTimeRaw));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLineDebouncerTimeRaw_Out3(int nParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eOut3LineDebouncerTimeRaw = EDeviceParameterGenICam_Out3LineDebouncerTimeRaw;

	int nPreValue = _ttoi(GetParamValue(eOut3LineDebouncerTimeRaw));

	do
	{
		EDeviceGenICamLineSelector eLineSelector;
		GetLineSelector(&eLineSelector);

		if(eLineSelector != EDeviceGenICamLineSelector_Out3)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CIntegerPtr ptrLineDebouncerTimeRaw = m_pNodeMapDevice->_GetNode("LineDebouncerTimeRaw");

				if(ptrLineDebouncerTimeRaw.IsValid() && GenApi::IsWritable(ptrLineDebouncerTimeRaw))
				{
					ptrLineDebouncerTimeRaw->SetValue(nParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eOut3LineDebouncerTimeRaw, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eOut3LineDebouncerTimeRaw))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eOut3LineDebouncerTimeRaw], nPreValue, nParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLineDebouncerTimeAbs_Out3(float * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterGenICam_Out3LineDebouncerTimeAbs));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLineDebouncerTimeAbs_Out3(float fParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eOut3LineDebouncerTimeAbs = EDeviceParameterGenICam_Out3LineDebouncerTimeAbs;

	float fPreValue = _ttof(GetParamValue(eOut3LineDebouncerTimeAbs));

	do
	{
		EDeviceGenICamLineSelector eLineSelector;
		GetLineSelector(&eLineSelector);

		if(eLineSelector != EDeviceGenICamLineSelector_Out3)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CFloatPtr ptrLineDebouncerTimeAbs = m_pNodeMapDevice->_GetNode("LineDebouncerTimeAbs");

				if(ptrLineDebouncerTimeAbs.IsValid() && GenApi::IsWritable(ptrLineDebouncerTimeAbs))
				{
					ptrLineDebouncerTimeAbs->SetValue(fParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), fParam);

		if(!SetParamValue(eOut3LineDebouncerTimeAbs, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eOut3LineDebouncerTimeAbs))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eOut3LineDebouncerTimeAbs], fPreValue, fParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLineMode_Out4(EDeviceGenICamLineMode * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGenICamLineMode)_ttoi(GetParamValue(EDeviceParameterGenICam_Out4LineMode));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLineMode_Out4(EDeviceGenICamLineMode eParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSaveID = EDeviceParameterGenICam_Out4LineMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGenICamLineMode_Count)
			break;

		EDeviceGenICamLineSelector eLineSelector;
		GetLineSelector(&eLineSelector);

		if(eLineSelector != EDeviceGenICamLineSelector_Out4)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				CStringA strLineMode(g_lpszGenICamLineMode[eParam]);

				GenApi::CEnumerationPtr ptrLineMode = m_pNodeMapDevice->_GetNode("LineMode");

				if(ptrLineMode.IsValid() && GenApi::IsWritable(ptrLineMode))
				{
					GENICAM_NAMESPACE::gcstring gcstr(strLineMode);

					ptrLineMode->FromString(gcstr);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSaveID], g_lpszGenICamLineMode[nPreValue], g_lpszGenICamLineMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLineSource_Out4(EDeviceGenICamLineSource * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGenICamLineSource)_ttoi(GetParamValue(EDeviceParameterGenICam_Out4LineSource));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLineSource_Out4(EDeviceGenICamLineSource eParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSaveID = EDeviceParameterGenICam_Out4LineSource;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGenICamLineSource_Count)
			break;

		EDeviceGenICamLineSelector eLineSelector;
		GetLineSelector(&eLineSelector);

		if(eLineSelector != EDeviceGenICamLineSelector_Out4)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				CStringA strLineSource(g_lpszGenICamLineSource[eParam]);

				GenApi::CEnumerationPtr ptrLineSource = m_pNodeMapDevice->_GetNode("LineSource");

				if(ptrLineSource.IsValid() && GenApi::IsWritable(ptrLineSource))
				{
					GENICAM_NAMESPACE::gcstring gcstr(strLineSource);

					ptrLineSource->FromString(gcstr);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSaveID], g_lpszGenICamLineSource[nPreValue], g_lpszGenICamLineSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLineFormat_Out4(EDeviceGenICamLineFormat * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGenICamLineFormat)_ttoi(GetParamValue(EDeviceParameterGenICam_Out4LineFormat));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLineFormat_Out4(EDeviceGenICamLineFormat eParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSaveID = EDeviceParameterGenICam_Out4LineFormat;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGenICamLineFormat_Count)
			break;

		EDeviceGenICamLineSelector eLineSelector;
		GetLineSelector(&eLineSelector);

		if(eLineSelector != EDeviceGenICamLineSelector_Out4)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				CStringA strLineFormat(g_lpszGenICamLineFormat[eParam]);

				GenApi::CEnumerationPtr ptrLineFormat = m_pNodeMapDevice->_GetNode("LineFormat");

				if(ptrLineFormat.IsValid() && GenApi::IsWritable(ptrLineFormat))
				{
					GENICAM_NAMESPACE::gcstring gcstr(strLineFormat);

					ptrLineFormat->FromString(gcstr);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSaveID], g_lpszGenICamLineFormat[nPreValue], g_lpszGenICamLineFormat[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLineInverter_Out4(bool * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_Out4LineInverter));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLineInverter_Out4(bool bParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eOut4LineInverter = EDeviceParameterGenICam_Out4LineInverter;

	int nPreValue = _ttoi(GetParamValue(eOut4LineInverter));

	do
	{
		EDeviceGenICamLineSelector eLineSelector;
		GetLineSelector(&eLineSelector);

		if(eLineSelector != EDeviceGenICamLineSelector_Out4)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CBooleanPtr ptrLineInverter = m_pNodeMapDevice->_GetNode("LineInverter");

				if(ptrLineInverter.IsValid() && GenApi::IsWritable(ptrLineInverter))
				{
					ptrLineInverter->SetValue(bParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eOut4LineInverter, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eOut4LineInverter))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eOut4LineInverter], g_lpszGenICamGammaEnable[nPreValue], g_lpszGenICamGammaEnable[bParam]);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLineTermination_Out4(bool * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_Out4LineTermination));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLineTermination_Out4(bool bParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eOut4LineTermination = EDeviceParameterGenICam_Out4LineTermination;

	int nPreValue = _ttoi(GetParamValue(eOut4LineTermination));

	do
	{
		EDeviceGenICamLineSelector eLineSelector;
		GetLineSelector(&eLineSelector);

		if(eLineSelector != EDeviceGenICamLineSelector_Out4)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CBooleanPtr ptrLineTermination = m_pNodeMapDevice->_GetNode("LineTermination");

				if(ptrLineTermination.IsValid() && GenApi::IsWritable(ptrLineTermination))
				{
					ptrLineTermination->SetValue(bParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eOut4LineTermination, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eOut4LineTermination))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eOut4LineTermination], g_lpszGenICamGammaEnable[nPreValue], g_lpszGenICamGammaEnable[bParam]);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLineDebouncerTimeRaw_Out4(int * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_Out4LineDebouncerTimeRaw));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLineDebouncerTimeRaw_Out4(int nParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eOut4LineDebouncerTimeRaw = EDeviceParameterGenICam_Out4LineDebouncerTimeRaw;

	int nPreValue = _ttoi(GetParamValue(eOut4LineDebouncerTimeRaw));

	do
	{
		EDeviceGenICamLineSelector eLineSelector;
		GetLineSelector(&eLineSelector);

		if(eLineSelector != EDeviceGenICamLineSelector_Out4)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CIntegerPtr ptrLineDebouncerTimeRaw = m_pNodeMapDevice->_GetNode("LineDebouncerTimeRaw");

				if(ptrLineDebouncerTimeRaw.IsValid() && GenApi::IsWritable(ptrLineDebouncerTimeRaw))
				{
					ptrLineDebouncerTimeRaw->SetValue(nParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eOut4LineDebouncerTimeRaw, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eOut4LineDebouncerTimeRaw))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eOut4LineDebouncerTimeRaw], nPreValue, nParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLineDebouncerTimeAbs_Out4(float * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterGenICam_Out4LineDebouncerTimeAbs));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLineDebouncerTimeAbs_Out4(float fParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eOut4LineDebouncerTimeAbs = EDeviceParameterGenICam_Out4LineDebouncerTimeAbs;

	float fPreValue = _ttof(GetParamValue(eOut4LineDebouncerTimeAbs));

	do
	{
		EDeviceGenICamLineSelector eLineSelector;
		GetLineSelector(&eLineSelector);

		if(eLineSelector != EDeviceGenICamLineSelector_Out4)
			break;

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CFloatPtr ptrLineDebouncerTimeAbs = m_pNodeMapDevice->_GetNode("LineDebouncerTimeAbs");

				if(ptrLineDebouncerTimeAbs.IsValid() && GenApi::IsWritable(ptrLineDebouncerTimeAbs))
				{
					ptrLineDebouncerTimeAbs->SetValue(fParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), fParam);

		if(!SetParamValue(eOut4LineDebouncerTimeAbs, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eOut4LineDebouncerTimeAbs))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eOut4LineDebouncerTimeAbs], fPreValue, fParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetUserOutputSelector(EDeviceGenICamUserOutputSelector * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGenICamUserOutputSelector)_ttoi(GetParamValue(EDeviceParameterGenICam_UserOutputSelector));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetUserOutputSelector(EDeviceGenICamUserOutputSelector eParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSaveID = EDeviceParameterGenICam_UserOutputSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGenICamUserOutputSelector_Count)
		{
			eReturn = EDeviceGenICamSetFunction_UnknownError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				CStringA strUserOutputSelector(g_lpszGenICamUserOutputSelector[eParam]);

				GenApi::CEnumerationPtr ptrUserOutputSelector = m_pNodeMapDevice->_GetNode("UserOutputSelector");

				if(ptrUserOutputSelector.IsValid() && GenApi::IsWritable(ptrUserOutputSelector))
				{
					GENICAM_NAMESPACE::gcstring gcstr(strUserOutputSelector);

					ptrUserOutputSelector->FromString(gcstr);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSaveID], g_lpszGenICamUserOutputSelector[nPreValue], g_lpszGenICamUserOutputSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetTimerDurationTimebaseAbs(float * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterGenICam_TimerDurationTimebaseAbs));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetTimerDurationTimebaseAbs(float fParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eTimerDurationTimebaseAbs = EDeviceParameterGenICam_TimerDurationTimebaseAbs;

	float fPreValue = _ttof(GetParamValue(eTimerDurationTimebaseAbs));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CFloatPtr ptrTimerDurationTimebaseAbs = m_pNodeMapDevice->_GetNode("TimerDurationTimebaseAbs");

				if(ptrTimerDurationTimebaseAbs.IsValid() && GenApi::IsWritable(ptrTimerDurationTimebaseAbs))
				{
					ptrTimerDurationTimebaseAbs->SetValue(fParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), fParam);

		if(!SetParamValue(eTimerDurationTimebaseAbs, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eTimerDurationTimebaseAbs))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eTimerDurationTimebaseAbs], fPreValue, fParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetTimerDelayTimebaseAbs(float * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterGenICam_TimerDelayTimebaseAbs));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetTimerDelayTimebaseAbs(float fParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eTimerDelayTimebaseAbs = EDeviceParameterGenICam_TimerDelayTimebaseAbs;

	float fPreValue = _ttof(GetParamValue(eTimerDelayTimebaseAbs));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CFloatPtr ptrTimerDelayTimebaseAbs = m_pNodeMapDevice->_GetNode("TimerDelayTimebaseAbs");

				if(ptrTimerDelayTimebaseAbs.IsValid() && GenApi::IsWritable(ptrTimerDelayTimebaseAbs))
				{
					ptrTimerDelayTimebaseAbs->SetValue(fParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), fParam);

		if(!SetParamValue(eTimerDelayTimebaseAbs, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eTimerDelayTimebaseAbs))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eTimerDelayTimebaseAbs], fPreValue, fParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetTimerSelector(EDeviceGenICamTimerSelector * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGenICamTimerSelector)_ttoi(GetParamValue(EDeviceParameterGenICam_TimerSelector));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetTimerSelector(EDeviceGenICamTimerSelector eParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSaveID = EDeviceParameterGenICam_TimerSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				CStringA strTimerSelector(g_lpszGenICamTimerSelector[eParam]);

				GenApi::CEnumerationPtr ptrTimerSelector = m_pNodeMapDevice->_GetNode("TimerSelector");

				if(ptrTimerSelector.IsValid() && GenApi::IsWritable(ptrTimerSelector))
				{
					GENICAM_NAMESPACE::gcstring gcstr(strTimerSelector);

					ptrTimerSelector->FromString(gcstr);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		if(eParam < 0 || eParam >= EDeviceGenICamTimerSelector_Count)
		{
			eReturn = EDeviceGenICamSetFunction_UnknownError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSaveID], g_lpszGenICamTimerSelector[nPreValue], g_lpszGenICamTimerSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetTimerDurationAbs(float * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterGenICam_TimerDurationAbs));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetTimerDurationAbs(float fParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eTimerDurationAbs = EDeviceParameterGenICam_TimerDurationAbs;

	float fPreValue = _ttof(GetParamValue(eTimerDurationAbs));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CFloatPtr ptrTimerDurationAbs = m_pNodeMapDevice->_GetNode("TimerDurationAbs");

				if(ptrTimerDurationAbs.IsValid() && GenApi::IsWritable(ptrTimerDurationAbs))
				{
					ptrTimerDurationAbs->SetValue(fParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), fParam);

		if(!SetParamValue(eTimerDurationAbs, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eTimerDurationAbs))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eTimerDurationAbs], fPreValue, fParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetTimerDurationRaw(int * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_TimerDurationRaw));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetTimerDurationRaw(int nParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eTimerDurationRaw = EDeviceParameterGenICam_TimerDurationRaw;

	int nPreValue = _ttoi(GetParamValue(eTimerDurationRaw));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CIntegerPtr ptrTimerDurationRaw = m_pNodeMapDevice->_GetNode("TimerDurationRaw");

				if(ptrTimerDurationRaw.IsValid() && GenApi::IsWritable(ptrTimerDurationRaw))
				{
					ptrTimerDurationRaw->SetValue(nParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eTimerDurationRaw, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eTimerDurationRaw))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eTimerDurationRaw], nPreValue, nParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetTimerDelayAbs(float * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterGenICam_TimerDelayAbs));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetTimerDelayAbs(float fParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eTimerDelayAbs = EDeviceParameterGenICam_TimerDelayAbs;

	float fPreValue = _ttof(GetParamValue(eTimerDelayAbs));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CFloatPtr ptrTimerDelayAbs = m_pNodeMapDevice->_GetNode("TimerDelayAbs");

				if(ptrTimerDelayAbs.IsValid() && GenApi::IsWritable(ptrTimerDelayAbs))
				{
					ptrTimerDelayAbs->SetValue(fParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), fParam);

		if(!SetParamValue(eTimerDelayAbs, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eTimerDelayAbs))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eTimerDelayAbs], fPreValue, fParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetTimerDelayRaw(int * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_TimerDelayRaw));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetTimerDelayRaw(int nParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eTimerDelayRaw = EDeviceParameterGenICam_TimerDelayRaw;

	int nPreValue = _ttoi(GetParamValue(eTimerDelayRaw));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CIntegerPtr ptrTimerDelayRaw = m_pNodeMapDevice->_GetNode("TimerDelayRaw");

				if(ptrTimerDelayRaw.IsValid() && GenApi::IsWritable(ptrTimerDelayRaw))
				{
					ptrTimerDelayRaw->SetValue(nParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eTimerDelayRaw, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eTimerDelayRaw))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eTimerDelayRaw], nPreValue, nParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetTimerTriggerSource(EDeviceGenICamTimerTriggerSource * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGenICamTimerTriggerSource)_ttoi(GetParamValue(EDeviceParameterGenICam_TimerTriggerSource));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetTimerTriggerSource(EDeviceGenICamTimerTriggerSource eParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSaveID = EDeviceParameterGenICam_TimerTriggerSource;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGenICamTimerTriggerSource_Count)
		{
			eReturn = EDeviceGenICamSetFunction_UnknownError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				CStringA strTimerTriggerSource(g_lpszGenICamTimerTriggerSource[eParam]);

				GenApi::CEnumerationPtr ptrTimerTriggerSource = m_pNodeMapDevice->_GetNode("TimerTriggerSource");

				if(ptrTimerTriggerSource.IsValid() && GenApi::IsWritable(ptrTimerTriggerSource))
				{
					GENICAM_NAMESPACE::gcstring gcstr(strTimerTriggerSource);

					ptrTimerTriggerSource->FromString(gcstr);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSaveID], g_lpszGenICamTimerTriggerSource[nPreValue], g_lpszGenICamTimerTriggerSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetTimerTriggerActivation(EDeviceGenICamTimerTriggerActivation * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGenICamTimerTriggerActivation)_ttoi(GetParamValue(EDeviceParameterGenICam_TimerTriggerActivation));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetTimerTriggerActivation(EDeviceGenICamTimerTriggerActivation eParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSaveID = EDeviceParameterGenICam_TimerTriggerActivation;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGenICamTimerTriggerActivation_Count)
		{
			eReturn = EDeviceGenICamSetFunction_UnknownError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				CStringA strTimerTriggerActivation(g_lpszGenICamTimerTriggerActivation[eParam]);

				GenApi::CEnumerationPtr ptrTimerTriggerActivation = m_pNodeMapDevice->_GetNode("TimerTriggerActivation");

				if(ptrTimerTriggerActivation.IsValid() && GenApi::IsWritable(ptrTimerTriggerActivation))
				{
					GENICAM_NAMESPACE::gcstring gcstr(strTimerTriggerActivation);

					ptrTimerTriggerActivation->FromString(gcstr);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSaveID], g_lpszGenICamTimerTriggerActivation[nPreValue], g_lpszGenICamTimerTriggerActivation[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetTimerSequenceEnable(bool * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_TimerSequenceEnable));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetTimerSequenceEnable(bool bParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eTimerSequenceEnable = EDeviceParameterGenICam_TimerSequenceEnable;

	int nPreValue = _ttoi(GetParamValue(eTimerSequenceEnable));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CBooleanPtr ptrTimerSequenceEnable = m_pNodeMapDevice->_GetNode("TimerSequenceEnable");

				if(ptrTimerSequenceEnable.IsValid() && GenApi::IsWritable(ptrTimerSequenceEnable))
				{
					ptrTimerSequenceEnable->SetValue(bParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eTimerSequenceEnable, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eTimerSequenceEnable))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eTimerSequenceEnable], g_lpszGenICamGammaEnable[nPreValue], g_lpszGenICamGammaEnable[bParam]);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetTimerSequenceLastEntryIndex(int * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_TimerSequenceLastEntryIndex));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetTimerSequenceLastEntryIndex(int nParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eTimerSequenceLastEntryIndex = EDeviceParameterGenICam_TimerSequenceLastEntryIndex;

	int nPreValue = _ttoi(GetParamValue(eTimerSequenceLastEntryIndex));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CIntegerPtr ptrTimerSequenceLastEntryIndex = m_pNodeMapDevice->_GetNode("TimerSequenceLastEntryIndex");

				if(ptrTimerSequenceLastEntryIndex.IsValid() && GenApi::IsWritable(ptrTimerSequenceLastEntryIndex))
				{
					ptrTimerSequenceLastEntryIndex->SetValue(nParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eTimerSequenceLastEntryIndex, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eTimerSequenceLastEntryIndex))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eTimerSequenceLastEntryIndex], nPreValue, nParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetTimerSequenceCurrentEntryIndex(int * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_TimerSequenceCurrentEntryIndex));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetTimerSequenceCurrentEntryIndex(int nParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eTimerSequenceCurrentEntryIndex = EDeviceParameterGenICam_TimerSequenceCurrentEntryIndex;

	int nPreValue = _ttoi(GetParamValue(eTimerSequenceCurrentEntryIndex));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CIntegerPtr ptrTimerSequenceCurrentEntryIndex = m_pNodeMapDevice->_GetNode("TimerSequenceCurrentEntryIndex");

				if(ptrTimerSequenceCurrentEntryIndex.IsValid() && GenApi::IsWritable(ptrTimerSequenceCurrentEntryIndex))
				{
					ptrTimerSequenceCurrentEntryIndex->SetValue(nParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eTimerSequenceCurrentEntryIndex, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eTimerSequenceCurrentEntryIndex))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eTimerSequenceCurrentEntryIndex], nPreValue, nParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetTimerSequenceEntrySelector(EDeviceGenICamTimerSequenceEntrySelector * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGenICamTimerSequenceEntrySelector)_ttoi(GetParamValue(EDeviceParameterGenICam_TimerSequenceEntrySelector));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetTimerSequenceEntrySelector(EDeviceGenICamTimerSequenceEntrySelector eParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSaveID = EDeviceParameterGenICam_TimerSequenceEntrySelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGenICamTimerSequenceEntrySelector_Count)
		{
			eReturn = EDeviceGenICamSetFunction_UnknownError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				CStringA strTimerSequenceEntrySelector(g_lpszGenICamTimerSequenceEntrySelector[eParam]);

				GenApi::CEnumerationPtr ptrTimerSequenceEntrySelector = m_pNodeMapDevice->_GetNode("TimerSequenceEntrySelector");

				if(ptrTimerSequenceEntrySelector.IsValid() && GenApi::IsWritable(ptrTimerSequenceEntrySelector))
				{
					GENICAM_NAMESPACE::gcstring gcstr(strTimerSequenceEntrySelector);

					ptrTimerSequenceEntrySelector->FromString(gcstr);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSaveID], g_lpszGenICamTimerSequenceEntrySelector[nPreValue], g_lpszGenICamTimerSequenceEntrySelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetTimerSequenceTimerSelector(EDeviceGenICamTimerSequenceTimerSelector * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGenICamTimerSequenceTimerSelector)_ttoi(GetParamValue(EDeviceParameterGenICam_TimerSequenceTimerSelector));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetTimerSequenceTimerSelector(EDeviceGenICamTimerSequenceTimerSelector eParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSaveID = EDeviceParameterGenICam_TimerSequenceTimerSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGenICamTimerSequenceTimerSelector_Count)
		{
			eReturn = EDeviceGenICamSetFunction_UnknownError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				CStringA strTimerSequenceTimerSelector(g_lpszGenICamTimerSequenceTimerSelector[eParam]);

				GenApi::CEnumerationPtr ptrTimerSequenceTimerSelector = m_pNodeMapDevice->_GetNode("TimerSequenceTimerSelector");

				if(ptrTimerSequenceTimerSelector.IsValid() && GenApi::IsWritable(ptrTimerSequenceTimerSelector))
				{
					GENICAM_NAMESPACE::gcstring gcstr(strTimerSequenceTimerSelector);

					ptrTimerSequenceTimerSelector->FromString(gcstr);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSaveID], g_lpszGenICamTimerSequenceTimerSelector[nPreValue], g_lpszGenICamTimerSequenceTimerSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetTimerSequenceTimerEnable(bool * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_TimerSequenceTimerEnable));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetTimerSequenceTimerEnable(bool bParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eTimerSequenceTimerEnable = EDeviceParameterGenICam_TimerSequenceTimerEnable;

	int nPreValue = _ttoi(GetParamValue(eTimerSequenceTimerEnable));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CBooleanPtr ptrTimerSequenceTimerEnable = m_pNodeMapDevice->_GetNode("TimerSequenceTimerEnable");

				if(ptrTimerSequenceTimerEnable.IsValid() && GenApi::IsWritable(ptrTimerSequenceTimerEnable))
				{
					ptrTimerSequenceTimerEnable->SetValue(bParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eTimerSequenceTimerEnable, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eTimerSequenceTimerEnable))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eTimerSequenceTimerEnable], g_lpszGenICamGammaEnable[nPreValue], g_lpszGenICamGammaEnable[bParam]);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetTimerSequenceTimerInverter(bool * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_TimerSequenceTimerInverter));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetTimerSequenceTimerInverter(bool bParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eTimerSequenceTimerInverter = EDeviceParameterGenICam_TimerSequenceTimerInverter;

	int nPreValue = _ttoi(GetParamValue(eTimerSequenceTimerInverter));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CBooleanPtr ptrTimerSequenceTimerInverter = m_pNodeMapDevice->_GetNode("TimerSequenceTimerInverter");

				if(ptrTimerSequenceTimerInverter.IsValid() && GenApi::IsWritable(ptrTimerSequenceTimerInverter))
				{
					ptrTimerSequenceTimerInverter->SetValue(bParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eTimerSequenceTimerInverter, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eTimerSequenceTimerInverter))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eTimerSequenceTimerInverter], g_lpszGenICamGammaEnable[nPreValue], g_lpszGenICamGammaEnable[bParam]);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetTimerSequenceTimerDelayRaw(int * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_TimerSequenceTimerDelayRaw));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetTimerSequenceTimerDelayRaw(int nParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eTimerSequenceTimerDelayRaw = EDeviceParameterGenICam_TimerSequenceTimerDelayRaw;

	int nPreValue = _ttoi(GetParamValue(eTimerSequenceTimerDelayRaw));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CIntegerPtr ptrTimerSequenceTimerDelayRaw = m_pNodeMapDevice->_GetNode("TimerSequenceTimerDelayRaw");

				if(ptrTimerSequenceTimerDelayRaw.IsValid() && GenApi::IsWritable(ptrTimerSequenceTimerDelayRaw))
				{
					ptrTimerSequenceTimerDelayRaw->SetValue(nParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eTimerSequenceTimerDelayRaw, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eTimerSequenceTimerDelayRaw))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eTimerSequenceTimerDelayRaw], nPreValue, nParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetTimerSequenceTimerDurationRaw(int * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_TimerSequenceTimerDurationRaw));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetTimerSequenceTimerDurationRaw(int nParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eTimerSequenceTimerDurationRaw = EDeviceParameterGenICam_TimerSequenceTimerDurationRaw;

	int nPreValue = _ttoi(GetParamValue(eTimerSequenceTimerDurationRaw));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CIntegerPtr ptrTimerSequenceTimerDurationRaw = m_pNodeMapDevice->_GetNode("TimerSequenceTimerDurationRaw");

				if(ptrTimerSequenceTimerDurationRaw.IsValid() && GenApi::IsWritable(ptrTimerSequenceTimerDurationRaw))
				{
					ptrTimerSequenceTimerDurationRaw->SetValue(nParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eTimerSequenceTimerDurationRaw, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eTimerSequenceTimerDurationRaw))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eTimerSequenceTimerDurationRaw], nPreValue, nParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetCounterSelector(EDeviceGenICamCounterSelector * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGenICamCounterSelector)_ttoi(GetParamValue(EDeviceParameterGenICam_CounterSelector));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetCounterSelector(EDeviceGenICamCounterSelector eParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSaveID = EDeviceParameterGenICam_CounterSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGenICamCounterSelector_Count)
		{
			eReturn = EDeviceGenICamSetFunction_UnknownError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				CStringA strCounterSelector(g_lpszGenICamCounterSelector[eParam]);

				GenApi::CEnumerationPtr ptrCounterSelector = m_pNodeMapDevice->_GetNode("CounterSelector");

				if(ptrCounterSelector.IsValid() && GenApi::IsWritable(ptrCounterSelector))
				{
					GENICAM_NAMESPACE::gcstring gcstr(strCounterSelector);

					ptrCounterSelector->FromString(gcstr);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSaveID], g_lpszGenICamCounterSelector[nPreValue], g_lpszGenICamCounterSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetCounterEventSource(EDeviceGenICamCounterEventSource * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGenICamCounterEventSource)_ttoi(GetParamValue(EDeviceParameterGenICam_CounterEventSource));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetCounterEventSource(EDeviceGenICamCounterEventSource eParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSaveID = EDeviceParameterGenICam_CounterEventSource;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGenICamCounterEventSource_Count)
		{
			eReturn = EDeviceGenICamSetFunction_UnknownError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				CStringA strCounterEventSource(g_lpszGenICamCounterEventSource[eParam]);

				GenApi::CEnumerationPtr ptrCounterEventSource = m_pNodeMapDevice->_GetNode("CounterEventSource");

				if(ptrCounterEventSource.IsValid() && GenApi::IsWritable(ptrCounterEventSource))
				{
					GENICAM_NAMESPACE::gcstring gcstr(strCounterEventSource);

					ptrCounterEventSource->FromString(gcstr);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSaveID], g_lpszGenICamCounterEventSource[nPreValue], g_lpszGenICamCounterEventSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetCounterResetSource(EDeviceGenICamCounterResetSource * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGenICamCounterResetSource)_ttoi(GetParamValue(EDeviceParameterGenICam_CounterResetSource));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetCounterResetSource(EDeviceGenICamCounterResetSource eParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSaveID = EDeviceParameterGenICam_CounterResetSource;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGenICamCounterResetSource_Count)
		{
			eReturn = EDeviceGenICamSetFunction_UnknownError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				CStringA strCounterResetSource(g_lpszGenICamCounterResetSource[eParam]);

				GenApi::CEnumerationPtr ptrCounterResetSource = m_pNodeMapDevice->_GetNode("CounterResetSource");

				if(ptrCounterResetSource.IsValid() && GenApi::IsWritable(ptrCounterResetSource))
				{
					GENICAM_NAMESPACE::gcstring gcstr(strCounterResetSource);

					ptrCounterResetSource->FromString(gcstr);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSaveID], g_lpszGenICamCounterResetSource[nPreValue], g_lpszGenICamCounterResetSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLUTEnable(bool * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_LUTEnable));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLUTEnable(bool bParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eLUTEnable = EDeviceParameterGenICam_LUTEnable;

	int nPreValue = _ttoi(GetParamValue(eLUTEnable));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CBooleanPtr ptrLUTEnable = m_pNodeMapDevice->_GetNode("LUTEnable");

				if(ptrLUTEnable.IsValid() && GenApi::IsWritable(ptrLUTEnable))
				{
					ptrLUTEnable->SetValue(bParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eLUTEnable, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eLUTEnable))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eLUTEnable], g_lpszGenICamGammaEnable[nPreValue], g_lpszGenICamGammaEnable[bParam]);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLUTIndex(int * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_LUTIndex));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLUTIndex(int nParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eLUTIndex = EDeviceParameterGenICam_LUTIndex;

	int nPreValue = _ttoi(GetParamValue(eLUTIndex));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CIntegerPtr ptrLUTIndex = m_pNodeMapDevice->_GetNode("LUTIndex");

				if(ptrLUTIndex.IsValid() && GenApi::IsWritable(ptrLUTIndex))
				{
					ptrLUTIndex->SetValue(nParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eLUTIndex, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eLUTIndex))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eLUTIndex], nPreValue, nParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLUTValue(int * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_LUTValue));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLUTValue(int nParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eLUTValue = EDeviceParameterGenICam_LUTValue;

	int nPreValue = _ttoi(GetParamValue(eLUTValue));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CIntegerPtr ptrLUTValue = m_pNodeMapDevice->_GetNode("LUTValue");

				if(ptrLUTValue.IsValid() && GenApi::IsWritable(ptrLUTValue))
				{
					ptrLUTValue->SetValue(nParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eLUTValue, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eLUTValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eLUTValue], nPreValue, nParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetLUTSelector(EDeviceGenICamLUTSelector * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGenICamLUTSelector)_ttoi(GetParamValue(EDeviceParameterGenICam_LUTSelector));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetLUTSelector(EDeviceGenICamLUTSelector eParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSaveID = EDeviceParameterGenICam_LUTSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				CStringA strLUTSelector(g_lpszGenICamLUTSelector[eParam]);

				GenApi::CEnumerationPtr ptrLUTSelector = m_pNodeMapDevice->_GetNode("LUTSelector");

				if(ptrLUTSelector.IsValid() && GenApi::IsWritable(ptrLUTSelector))
				{
					GENICAM_NAMESPACE::gcstring gcstr(strLUTSelector);

					ptrLUTSelector->FromString(gcstr);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		if(eParam < 0 || eParam >= EDeviceGenICamLUTSelector_Count)
		{
			eReturn = EDeviceGenICamSetFunction_UnknownError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSaveID], g_lpszGenICamLUTSelector[nPreValue], g_lpszGenICamLUTSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetGevLinkSpeed(int * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_GevLinkSpeed));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetGevLinkSpeed(int nParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eGevLinkSpeed = EDeviceParameterGenICam_GevLinkSpeed;

	int nPreValue = _ttoi(GetParamValue(eGevLinkSpeed));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CIntegerPtr ptrGevLinkSpeed = m_pNodeMapDevice->_GetNode("GevLinkSpeed");

				if(ptrGevLinkSpeed.IsValid() && GenApi::IsWritable(ptrGevLinkSpeed))
				{
					ptrGevLinkSpeed->SetValue(nParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eGevLinkSpeed, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eGevLinkSpeed))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eGevLinkSpeed], nPreValue, nParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetGevHeartbeatTimeout(int * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_GevHeartbeatTimeout));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetGevHeartbeatTimeout(int nParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eGevHeartbeatTimeout = EDeviceParameterGenICam_GevHeartbeatTimeout;

	int nPreValue = _ttoi(GetParamValue(eGevHeartbeatTimeout));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CIntegerPtr ptrGevHeartbeatTimeout = m_pNodeMapDevice->_GetNode("GevHeartbeatTimeout");

				if(ptrGevHeartbeatTimeout.IsValid() && GenApi::IsWritable(ptrGevHeartbeatTimeout))
				{
					ptrGevHeartbeatTimeout->SetValue(nParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eGevHeartbeatTimeout, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eGevHeartbeatTimeout))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eGevHeartbeatTimeout], nPreValue, nParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetGevSCPSPacketSize(int * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterGenICam_GevSCPSPacketSize));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetGevSCPSPacketSize(int nParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eGevSCPSPacketSize = EDeviceParameterGenICam_GevSCPSPacketSize;

	int nPreValue = _ttoi(GetParamValue(eGevSCPSPacketSize));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;

			if(!m_pNodeMapDevice)
				break;

			try
			{
				GenApi::CIntegerPtr ptrGevSCPSPacketSize = m_pNodeMapDevice->_GetNode("GevSCPSPacketSize");

				if(ptrGevSCPSPacketSize.IsValid() && GenApi::IsWritable(ptrGevSCPSPacketSize))
				{
					ptrGevSCPSPacketSize->SetValue(nParam);
				}
				else
				{
					eReturn = EDeviceGenICamSetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = EDeviceGenICamSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eGevSCPSPacketSize, strValue))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eGevSCPSPacketSize))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eGevSCPSPacketSize], nPreValue, nParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceGenICamGetFunction CDeviceGenICam::GetInitializeMode(EDeviceGenICamInitMode * pParam)
{
	EDeviceGenICamGetFunction eReturn = EDeviceGenICamGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceGenICamGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGenICamInitMode)_ttoi(GetParamValue(EDeviceParameterGenICam_InitializeMode));

		eReturn = EDeviceGenICamGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceGenICamSetFunction CDeviceGenICam::SetInitializeMode(EDeviceGenICamInitMode eParam)
{
	EDeviceGenICamSetFunction eReturn = EDeviceGenICamSetFunction_UnknownError;

	EDeviceParameterGenICam eSaveID = EDeviceParameterGenICam_InitializeMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceGenICamSetFunction_AlreadyInitializedError;
			break;
		}

		if(eParam < EDeviceGenICamInitMode_DeviceOnly || eParam >= EDeviceGenICamInitMode_Count)
		{
			eReturn = EDeviceGenICamSetFunction_UnknownError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceGenICamSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDeviceGenICamSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGenICam[eSaveID], g_lpszGenICamInitMode[nPreValue], g_lpszGenICamInitMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

void CDeviceGenICam::UpdateDeviceToParameter()
{
	do
	{
		if(!m_pNodeMapDevice)
			break;

		GenApi::CEnumerationPtr ptrGainAuto = m_pNodeMapDevice->_GetNode("GainAuto");

		if(ptrGainAuto.IsValid() && IsReadable(ptrGainAuto))
		{
			GENICAM_NAMESPACE::gcstring gcstrGainAuto = ptrGainAuto->ToString();

			CStringA strGainAutoA(gcstrGainAuto.c_str());
			CString strGainAuto = CString(strGainAutoA);

			strGainAuto.Replace(_T(" "), NULL);

			int nIdx = EDeviceGenICamGainAuto_Count;

			for(int i = 0; i < EDeviceGenICamGainAuto_Count; ++i)
			{
				if(!strGainAuto.CompareNoCase(g_lpszGenICamGainAuto[i]))
				{
					nIdx = i;
					break;
				}
			}

			SetGainAuto(EDeviceGenICamGainAuto(nIdx));
		}

		try
		{
			GenApi::CIntegerPtr ptrGainRaw = m_pNodeMapDevice->_GetNode("GainRaw");

			if(ptrGainRaw.IsValid() && IsReadable(ptrGainRaw))
			{
				int nGainRaw = ptrGainRaw->GetValue();

				SetGainRaw(nGainRaw);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CEnumerationPtr ptrGainSelector = m_pNodeMapDevice->_GetNode("GainSelector");

			if(ptrGainSelector.IsValid() && IsReadable(ptrGainSelector))
			{
				GENICAM_NAMESPACE::gcstring gcstrGainSelector = ptrGainSelector->ToString();

				CStringA strGainSelectorA(gcstrGainSelector.c_str());
				CString strGainSelector = CString(strGainSelectorA);

				strGainSelector.Replace(_T(" "), NULL);

				int nIdx = EDeviceGenICamGainSelector_Count;

				for(int i = 0; i < EDeviceGenICamGainSelector_Count; ++i)
				{
					if(!strGainSelector.CompareNoCase(g_lpszGenICamGainSelector[i]))
					{
						nIdx = i;
						break;
					}
				}

				SetGainSelector(EDeviceGenICamGainSelector(nIdx));
			}
		}
		catch(...) { }

		try
		{
			GenApi::CIntegerPtr ptrBlackLevelRaw = m_pNodeMapDevice->_GetNode("BlackLevelRaw");

			if(ptrBlackLevelRaw.IsValid() && IsReadable(ptrBlackLevelRaw))
			{
				int nBlackLevelRaw = ptrBlackLevelRaw->GetValue();

				SetBlackLevelRaw(nBlackLevelRaw);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CEnumerationPtr ptrBlackLevelSelector = m_pNodeMapDevice->_GetNode("BlackLevelSelector");

			if(ptrBlackLevelSelector.IsValid() && IsReadable(ptrBlackLevelSelector))
			{
				GENICAM_NAMESPACE::gcstring gcstrBlackLevelSelector = ptrBlackLevelSelector->ToString();

				CStringA strBlackLevelSelectorA(gcstrBlackLevelSelector.c_str());
				CString strBlackLevelSelector = CString(strBlackLevelSelectorA);

				strBlackLevelSelector.Replace(_T(" "), NULL);

				int nIdx = EDeviceGenICamBlackLevelSelector_Count;

				for(int i = 0; i < EDeviceGenICamBlackLevelSelector_Count; ++i)
				{
					if(!strBlackLevelSelector.CompareNoCase(g_lpszGenICamBlackLevelSelector[i]))
					{
						nIdx = i;
						break;
					}
				}

				SetBlackLevelSelector(EDeviceGenICamBlackLevelSelector(nIdx));
			}
		}
		catch(...) { }

		try
		{
			GenApi::CEnumerationPtr ptrBalanceWhiteAuto = m_pNodeMapDevice->_GetNode("BalanceWhiteAuto");

			if(ptrBalanceWhiteAuto.IsValid() && IsReadable(ptrBalanceWhiteAuto))
			{
				GENICAM_NAMESPACE::gcstring gcstrBalanceWhiteAuto = ptrBalanceWhiteAuto->ToString();

				CStringA strBalanceWhiteAutoA(gcstrBalanceWhiteAuto.c_str());
				CString strBalanceWhiteAuto = CString(strBalanceWhiteAutoA);

				strBalanceWhiteAuto.Replace(_T(" "), NULL);

				int nIdx = EDeviceGenICamBalanceWhiteAuto_Count;

				for(int i = 0; i < EDeviceGenICamBalanceWhiteAuto_Count; ++i)
				{
					if(!strBalanceWhiteAuto.CompareNoCase(g_lpszGenICamBalanceWhiteAuto[i]))
					{
						nIdx = i;
						break;
					}
				}

				SetBalanceWhiteAuto(EDeviceGenICamBalanceWhiteAuto(nIdx));
			}
		}
		catch(...) { }

		try
		{
			GenApi::CEnumerationPtr ptrBalanceRatioSelector = m_pNodeMapDevice->_GetNode("BalanceRatioSelector");

			if(ptrBalanceRatioSelector.IsValid() && IsReadable(ptrBalanceRatioSelector))
			{
				GENICAM_NAMESPACE::gcstring gcstrBalanceRatioSelector = ptrBalanceRatioSelector->ToString();

				CStringA strBalanceRatioSelectorA(gcstrBalanceRatioSelector.c_str());
				CString strBalanceRatioSelector = CString(strBalanceRatioSelectorA);

				strBalanceRatioSelector.Replace(_T(" "), NULL);

				int nIdx = EDeviceGenICamBalanceRatioSelector_Count;

				for(int i = 0; i < EDeviceGenICamBalanceRatioSelector_Count; ++i)
				{
					if(!strBalanceRatioSelector.CompareNoCase(g_lpszGenICamBalanceRatioSelector[i]))
					{
						nIdx = i;
						break;
					}
				}

				SetBalanceRatioSelector(EDeviceGenICamBalanceRatioSelector(nIdx));
			}
		}
		catch(...) { }

		try
		{
			GenApi::CFloatPtr ptrBalanceRatioAbs = m_pNodeMapDevice->_GetNode("BalanceRatioAbs");

			if(ptrBalanceRatioAbs.IsValid() && IsReadable(ptrBalanceRatioAbs))
			{
				float fBalanceRatioAbs = ptrBalanceRatioAbs->GetValue();

				SetBalanceRatioAbs(fBalanceRatioAbs);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CIntegerPtr ptrBalanceRatioRaw = m_pNodeMapDevice->_GetNode("BalanceRatioRaw");

			if(ptrBalanceRatioRaw.IsValid() && IsReadable(ptrBalanceRatioRaw))
			{
				int nBalanceRatioRaw = ptrBalanceRatioRaw->GetValue();

				SetBalanceRatioRaw(nBalanceRatioRaw);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CBooleanPtr ptrGammaEnable = m_pNodeMapDevice->_GetNode("GammaEnable");

			if(ptrGammaEnable.IsValid() && IsReadable(ptrGammaEnable))
			{
				bool bGammaEnable = ptrGammaEnable->GetValue();

				SetGammaEnable(bGammaEnable);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CFloatPtr ptrGamma = m_pNodeMapDevice->_GetNode("Gamma");

			if(ptrGamma.IsValid() && IsReadable(ptrGamma))
			{
				float fGamma = ptrGamma->GetValue();

				SetGamma(fGamma);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CIntegerPtr ptrDigitalShift = m_pNodeMapDevice->_GetNode("DigitalShift");

			if(ptrDigitalShift.IsValid() && IsReadable(ptrDigitalShift))
			{
				int nDigitalShift = ptrDigitalShift->GetValue();

				SetDigitalShift(nDigitalShift);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CEnumerationPtr ptrPixelFormat = m_pNodeMapDevice->_GetNode("PixelFormat");

			if(ptrPixelFormat.IsValid() && IsReadable(ptrPixelFormat))
			{
				GENICAM_NAMESPACE::gcstring gcstrPixelFormat = ptrPixelFormat->ToString();

				CStringA strPixelFormatA(gcstrPixelFormat.c_str());
				CString strPixelFormat = CString(strPixelFormatA);

				strPixelFormat.Replace(_T(" "), NULL);

				int nIdx = EDeviceGenICamPixelFormat_Count;

				for(int i = 0; i < EDeviceGenICamPixelFormat_Count; ++i)
				{
					if(!strPixelFormat.CompareNoCase(g_lpszGenICamPixelFormat[i]))
					{
						nIdx = i;
						break;
					}
				}

				SetPixelFormat(EDeviceGenICamPixelFormat(nIdx));
			}
		}
		catch(...) { }

		try
		{
			GenApi::CBooleanPtr ptrReverseX = m_pNodeMapDevice->_GetNode("ReverseX");

			if(ptrReverseX.IsValid() && IsReadable(ptrReverseX))
			{
				bool bReverseX = ptrReverseX->GetValue();

				SetReverseX(bReverseX);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CBooleanPtr ptrReverseY = m_pNodeMapDevice->_GetNode("ReverseY");

			if(ptrReverseY.IsValid() && IsReadable(ptrReverseY))
			{
				bool bReverseY = ptrReverseY->GetValue();

				SetReverseY(bReverseY);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CIntegerPtr ptrSensorWidth = m_pNodeMapDevice->_GetNode("SensorWidth");

			if(ptrSensorWidth.IsValid() && IsReadable(ptrSensorWidth))
			{
				int nSensorWidth = ptrSensorWidth->GetValue();

				SetSensorWidth(nSensorWidth);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CIntegerPtr ptrHeight = m_pNodeMapDevice->_GetNode("Height");

			if(ptrHeight.IsValid() && IsReadable(ptrHeight))
			{
				int nHeight = ptrHeight->GetValue();
				SetHeight(nHeight);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CIntegerPtr ptrWidth = m_pNodeMapDevice->_GetNode("Width");

			if(ptrWidth.IsValid() && IsReadable(ptrWidth))
			{
				int nWidth = ptrWidth->GetValue();
				SetWidth(nWidth);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CIntegerPtr ptrOffsetX = m_pNodeMapDevice->_GetNode("OffsetX");

			if(ptrOffsetX.IsValid() && IsReadable(ptrOffsetX))
			{
				int nOffsetX = ptrOffsetX->GetValue();

				SetOffsetX(nOffsetX);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CIntegerPtr ptrOffsetY = m_pNodeMapDevice->_GetNode("OffsetY");

			if(ptrOffsetY.IsValid() && IsReadable(ptrOffsetY))
			{
				int nOffsetY = ptrOffsetY->GetValue();

				SetOffsetY(nOffsetY);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CIntegerPtr ptrBinningVertical = m_pNodeMapDevice->_GetNode("BinningVertical");

			if(ptrBinningVertical.IsValid() && IsReadable(ptrBinningVertical))
			{
				int nBinningVertical = ptrBinningVertical->GetValue();

				SetBinningVertical(nBinningVertical);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CIntegerPtr ptrBinningHorizontal = m_pNodeMapDevice->_GetNode("BinningHorizontal");

			if(ptrBinningHorizontal.IsValid() && IsReadable(ptrBinningHorizontal))
			{
				int nBinningHorizontal = ptrBinningHorizontal->GetValue();

				SetBinningHorizontal(nBinningHorizontal);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CIntegerPtr ptrSensorHeight = m_pNodeMapDevice->_GetNode("SensorHeight");

			if(ptrSensorHeight.IsValid() && IsReadable(ptrSensorHeight))
			{
				int nSensorHeight = ptrSensorHeight->GetValue();

				SetSensorHeight(nSensorHeight);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CEnumerationPtr ptrLegacyBinningVertical = m_pNodeMapDevice->_GetNode("LegacyBinningVertical");

			if(ptrLegacyBinningVertical.IsValid() && IsReadable(ptrLegacyBinningVertical))
			{
				GENICAM_NAMESPACE::gcstring gcstrLegacyBinningVertical = ptrLegacyBinningVertical->ToString();

				CStringA strLegacyBinningVerticalA(gcstrLegacyBinningVertical.c_str());
				CString strLegacyBinningVertical = CString(strLegacyBinningVerticalA);

				strLegacyBinningVertical.Replace(_T(" "), NULL);

				int nIdx = EDeviceGenICamLegacyBinningVertical_Count;

				for(int i = 0; i < EDeviceGenICamLegacyBinningVertical_Count; ++i)
				{
					if(!strLegacyBinningVertical.CompareNoCase(g_lpszGenICamLegacyBinningVertical[i]))
					{
						nIdx = i;
						break;
					}
				}

				SetLegacyBinningVertical(EDeviceGenICamLegacyBinningVertical(nIdx));
			}
		}
		catch(...) { }

		try
		{
			GenApi::CBooleanPtr ptrEnableBurstAcquisition = m_pNodeMapDevice->_GetNode("EnableBurstAcquisition");

			if(ptrEnableBurstAcquisition.IsValid() && IsReadable(ptrEnableBurstAcquisition))
			{
				bool bEnableBurstAcquisition = ptrEnableBurstAcquisition->GetValue();

				SetEnableBurstAcquisition(bEnableBurstAcquisition);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CEnumerationPtr ptrAcquisitionMode = m_pNodeMapDevice->_GetNode("AcquisitionMode");

			if(ptrAcquisitionMode.IsValid() && IsReadable(ptrAcquisitionMode))
			{
				GENICAM_NAMESPACE::gcstring gcstrAcquisitionMode = ptrAcquisitionMode->ToString();

				CStringA strAcquisitionModeA(gcstrAcquisitionMode.c_str());
				CString strAcquisitionMode = CString(strAcquisitionModeA);

				strAcquisitionMode.Replace(_T(" "), NULL);

				int nIdx = EDeviceGenICamAcquisitionMode_Count;

				for(int i = 0; i < EDeviceGenICamAcquisitionMode_Count; ++i)
				{
					if(!strAcquisitionMode.CompareNoCase(g_lpszGenICamAcquisitionMode[i]))
					{
						nIdx = i;
						break;
					}
				}

				SetAcquisitionMode(EDeviceGenICamAcquisitionMode(nIdx));
			}
		}
		catch(...) { }

		try
		{
			GenApi::CEnumerationPtr ptrTriggerSelector = m_pNodeMapDevice->_GetNode("TriggerSelector");

			if(ptrTriggerSelector.IsValid() && IsReadable(ptrTriggerSelector))
			{
				GENICAM_NAMESPACE::gcstring gcstrTriggerSelector = ptrTriggerSelector->ToString();

				CStringA strTriggerSelectorA(gcstrTriggerSelector.c_str());
				CString strTriggerSelector = CString(strTriggerSelectorA);

				strTriggerSelector.Replace(_T(" "), NULL);

				int nIdx = EDeviceGenICamTriggerSelector_Count;

				for(int i = 0; i < EDeviceGenICamTriggerSelector_Count; ++i)
				{
					if(!strTriggerSelector.CompareNoCase(g_lpszGenICamTriggerSelector[i]))
					{
						nIdx = i;
						break;
					}
				}

				SetTriggerSelector(EDeviceGenICamTriggerSelector(nIdx));
			}
		}
		catch(...) { }

		try
		{
			GenApi::CEnumerationPtr ptrTriggerMode = m_pNodeMapDevice->_GetNode("TriggerMode");

			if(ptrTriggerMode.IsValid() && IsReadable(ptrTriggerMode))
			{
				GENICAM_NAMESPACE::gcstring gcstrTriggerMode = ptrTriggerMode->ToString();

				CStringA strTriggerModeA(gcstrTriggerMode.c_str());
				CString strTriggerMode = CString(strTriggerModeA);

				strTriggerMode.Replace(_T(" "), NULL);

				int nIdx = EDeviceGenICamTriggerMode_Count;

				for(int i = 0; i < EDeviceGenICamTriggerMode_Count; ++i)
				{
					if(!strTriggerMode.CompareNoCase(g_lpszGenICamTriggerMode[i]))
					{
						nIdx = i;
						break;
					}
				}

				SetTriggerMode(EDeviceGenICamTriggerMode(nIdx));
			}
		}
		catch(...) { }

		try
		{
			GenApi::CEnumerationPtr ptrTriggerSource = m_pNodeMapDevice->_GetNode("TriggerSource");

			if(ptrTriggerSource.IsValid() && IsReadable(ptrTriggerSource))
			{
				GENICAM_NAMESPACE::gcstring gcstrTriggerSource = ptrTriggerSource->ToString();

				CStringA strTriggerSourceA(gcstrTriggerSource.c_str());
				CString strTriggerSource = CString(strTriggerSourceA);

				strTriggerSource.Replace(_T(" "), NULL);

				int nIdx = EDeviceGenICamTriggerSource_Count;

				for(int i = 0; i < EDeviceGenICamTriggerSource_Count; ++i)
				{
					if(!strTriggerSource.CompareNoCase(g_lpszGenICamTriggerSource[i]))
					{
						nIdx = i;
						break;
					}
				}

				SetTriggerSource(EDeviceGenICamTriggerSource(nIdx));
			}
		}
		catch(...) { }

		try
		{
			GenApi::CEnumerationPtr ptrTriggerActivation = m_pNodeMapDevice->_GetNode("TriggerActivation");

			if(ptrTriggerActivation.IsValid() && IsReadable(ptrTriggerActivation))
			{
				GENICAM_NAMESPACE::gcstring gcstrTriggerActivation = ptrTriggerActivation->ToString();

				CStringA strTriggerActivationA(gcstrTriggerActivation.c_str());
				CString strTriggerActivation = CString(strTriggerActivationA);

				strTriggerActivation.Replace(_T(" "), NULL);

				int nIdx = EDeviceGenICamTriggerActivation_Count;

				for(int i = 0; i < EDeviceGenICamTriggerActivation_Count; ++i)
				{
					if(!strTriggerActivation.CompareNoCase(g_lpszGenICamTriggerActivation[i]))
					{
						nIdx = i;
						break;
					}
				}

				SetTriggerActivation(EDeviceGenICamTriggerActivation(nIdx));
			}
		}
		catch(...) { }

		try
		{
			GenApi::CFloatPtr ptrTriggerDelayAbs = m_pNodeMapDevice->_GetNode("TriggerDelayAbs");

			if(ptrTriggerDelayAbs.IsValid() && IsReadable(ptrTriggerDelayAbs))
			{
				float fTriggerDelayAbs = ptrTriggerDelayAbs->GetValue();

				SetTriggerDelayAbs(fTriggerDelayAbs);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CEnumerationPtr ptrExposureAuto = m_pNodeMapDevice->_GetNode("ExposureAuto");

			if(ptrExposureAuto.IsValid() && IsReadable(ptrExposureAuto))
			{
				GENICAM_NAMESPACE::gcstring gcstrExposureAuto = ptrExposureAuto->ToString();

				CStringA strExposureAutoA(gcstrExposureAuto.c_str());
				CString strExposureAuto = CString(strExposureAutoA);

				strExposureAuto.Replace(_T(" "), NULL);

				int nIdx = EDeviceGenICamExposureAuto_Count;

				for(int i = 0; i < EDeviceGenICamExposureAuto_Count; ++i)
				{
					if(!strExposureAuto.CompareNoCase(g_lpszGenICamExposureAuto[i]))
					{
						nIdx = i;
						break;
					}
				}

				SetExposureAuto(EDeviceGenICamExposureAuto(nIdx));
			}
		}
		catch(...) { }

		try
		{
			GenApi::CEnumerationPtr ptrExposureMode = m_pNodeMapDevice->_GetNode("ExposureMode");

			if(ptrExposureMode.IsValid() && IsReadable(ptrExposureMode))
			{
				GENICAM_NAMESPACE::gcstring gcstrExposureMode = ptrExposureMode->ToString();

				CStringA strExposureModeA(gcstrExposureMode.c_str());
				CString strExposureMode = CString(strExposureModeA);

				strExposureMode.Replace(_T(" "), NULL);

				int nIdx = EDeviceGenICamExposureMode_Count;

				for(int i = 0; i < EDeviceGenICamExposureMode_Count; ++i)
				{
					if(!strExposureMode.CompareNoCase(g_lpszGenICamExposureMode[i]))
					{
						nIdx = i;
						break;
					}
				}

				SetExposureMode(EDeviceGenICamExposureMode(nIdx));
			}
		}
		catch(...) { }

		try
		{
			GenApi::CIntegerPtr ptrExposureTimeRaw = m_pNodeMapDevice->_GetNode("ExposureTimeRaw");

			if(ptrExposureTimeRaw.IsValid() && IsReadable(ptrExposureTimeRaw))
			{
				int nExposureTimeRaw = ptrExposureTimeRaw->GetValue();

				SetExposureTimeRaw(nExposureTimeRaw);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CFloatPtr ptrExposureTimeAbs = m_pNodeMapDevice->_GetNode("ExposureTimeAbs");

			if(ptrExposureTimeAbs.IsValid() && IsReadable(ptrExposureTimeAbs))
			{
				float fExposureTimeAbs = ptrExposureTimeAbs->GetValue();

				SetExposureTimeAbs(fExposureTimeAbs);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CFloatPtr ptrExposureTimeBaseAbs = m_pNodeMapDevice->_GetNode("ExposureTimeBaseAbs");

			if(ptrExposureTimeBaseAbs.IsValid() && IsReadable(ptrExposureTimeBaseAbs))
			{
				float fExposureTimeBaseAbs = ptrExposureTimeBaseAbs->GetValue();

				SetExposureTimeBaseAbs(fExposureTimeBaseAbs);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CBooleanPtr ptrExposureTimeBaseAbsEnable = m_pNodeMapDevice->_GetNode("ExposureTimeBaseAbsEnable");

			if(ptrExposureTimeBaseAbsEnable.IsValid() && IsReadable(ptrExposureTimeBaseAbsEnable))
			{
				bool bExposureTimeBaseAbsEnable = ptrExposureTimeBaseAbsEnable->GetValue();

				SetExposureTimeBaseAbsEnable(bExposureTimeBaseAbsEnable);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CIntegerPtr ptrAcquisitionLineRateRaw = m_pNodeMapDevice->_GetNode("AcquisitionLineRateRaw");

			if(ptrAcquisitionLineRateRaw.IsValid() && IsReadable(ptrAcquisitionLineRateRaw))
			{
				int nAcquisitionLineRateRaw = ptrAcquisitionLineRateRaw->GetValue();

				SetAcquisitionLineRateRaw(nAcquisitionLineRateRaw);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CFloatPtr ptrAcquisitionLineRateAbs = m_pNodeMapDevice->_GetNode("AcquisitionLineRateAbs");

			if(ptrAcquisitionLineRateAbs.IsValid() && IsReadable(ptrAcquisitionLineRateAbs))
			{
				float fAcquisitionLineRateAbs = ptrAcquisitionLineRateAbs->GetValue();

				SetAcquisitionLineRateAbs(fAcquisitionLineRateAbs);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CFloatPtr ptrResultingLineRateAbs = m_pNodeMapDevice->_GetNode("ResultingLineRateAbs");

			if(ptrResultingLineRateAbs.IsValid() && IsReadable(ptrResultingLineRateAbs))
			{
				float fResultingLineRateAbs = ptrResultingLineRateAbs->GetValue();

				SetResultingLineRateAbs(fResultingLineRateAbs);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CFloatPtr ptrAcquisitionFrameRate = m_pNodeMapDevice->_GetNode("AcquisitionFrameRate");

			if(ptrAcquisitionFrameRate.IsValid() && IsReadable(ptrAcquisitionFrameRate))
			{
				float fAcquisitionFrameRate = ptrAcquisitionFrameRate->GetValue();

				SetAcquisitionFrameRate(fAcquisitionFrameRate);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CBooleanPtr ptrAcquisitionFrameRateEnable = m_pNodeMapDevice->_GetNode("AcquisitionFrameRateEnable");

			if(ptrAcquisitionFrameRateEnable.IsValid() && IsReadable(ptrAcquisitionFrameRateEnable))
			{
				bool bAcquisitionFrameRateEnable = ptrAcquisitionFrameRateEnable->GetValue();

				SetAcquisitionFrameRateEnable(bAcquisitionFrameRateEnable);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CFloatPtr ptrResultingFrameRateAbs = m_pNodeMapDevice->_GetNode("ResultingFrameRateAbs");

			if(ptrResultingFrameRateAbs.IsValid() && IsReadable(ptrResultingFrameRateAbs))
			{
				float fResultingFrameRateAbs = ptrResultingFrameRateAbs->GetValue();

				SetResultingFrameRateAbs(fResultingFrameRateAbs);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CEnumerationPtr ptrAcquisitionStatusSelector = m_pNodeMapDevice->_GetNode("AcquisitionStatusSelector");

			if(ptrAcquisitionStatusSelector.IsValid() && IsReadable(ptrAcquisitionStatusSelector))
			{
				GENICAM_NAMESPACE::gcstring gcstrAcquisitionStatusSelector = ptrAcquisitionStatusSelector->ToString();

				CStringA strAcquisitionStatusSelectorA(gcstrAcquisitionStatusSelector.c_str());
				CString strAcquisitionStatusSelector = CString(strAcquisitionStatusSelectorA);

				strAcquisitionStatusSelector.Replace(_T(" "), NULL);

				int nIdx = EDeviceGenICamAcquisitionStatusSelector_Count;

				for(int i = 0; i < EDeviceGenICamAcquisitionStatusSelector_Count; ++i)
				{
					if(!strAcquisitionStatusSelector.CompareNoCase(g_lpszGenICamAcquisitionStatusSelector[i]))
					{
						nIdx = i;
						break;
					}
				}

				SetAcquisitionStatusSelector(EDeviceGenICamAcquisitionStatusSelector(nIdx));
			}
		}
		catch(...) { }

		try
		{
			GenApi::CBooleanPtr ptrAcquisitionStatus = m_pNodeMapDevice->_GetNode("AcquisitionStatus");

			if(ptrAcquisitionStatus.IsValid() && IsReadable(ptrAcquisitionStatus))
			{
				bool bAcquisitionStatus = ptrAcquisitionStatus->GetValue();

				SetAcquisitionStatus(bAcquisitionStatus);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CEnumerationPtr ptrLineSelector = m_pNodeMapDevice->_GetNode("LineSelector");

			if(ptrLineSelector.IsValid() && IsReadable(ptrLineSelector))
			{
				GENICAM_NAMESPACE::gcstring gcstrLineSelector = ptrLineSelector->ToString();

				CStringA strLineSelectorA(gcstrLineSelector.c_str());
				CString strLineSelector = CString(strLineSelectorA);

				strLineSelector.Replace(_T(" "), NULL);

				int nIdx = EDeviceGenICamLineSelector_Count;

				for(int i = 0; i < EDeviceGenICamLineSelector_Count; ++i)
				{
					if(!strLineSelector.CompareNoCase(g_lpszGenICamLineSelector[i]))
					{
						nIdx = i;
						break;
					}
				}

				SetLineSelector(EDeviceGenICamLineSelector(nIdx));
			}
		}
		catch(...) { }

		EDeviceGenICamLineSelector eLineSelect;
		GetLineSelector(&eLineSelect);

		if(eLineSelect == EDeviceGenICamLineSelector_Line1)
		{
			try
			{
				GenApi::CEnumerationPtr ptrLineMode = m_pNodeMapDevice->_GetNode("LineMode");

				if(ptrLineMode.IsValid() && IsReadable(ptrLineMode))
				{
					GENICAM_NAMESPACE::gcstring gcstrLineMode = ptrLineMode->ToString();

					CStringA strLineModeA(gcstrLineMode.c_str());
					CString strLineMode = CString(strLineModeA);

					strLineMode.Replace(_T(" "), NULL);

					int nIdx = EDeviceGenICamLineMode_Count;

					for(int i = 0; i < EDeviceGenICamLineMode_Count; ++i)
					{
						if(!strLineMode.CompareNoCase(g_lpszGenICamLineMode[i]))
						{
							nIdx = i;
							break;
						}
					}

					SetLineMode_Line1(EDeviceGenICamLineMode(nIdx));
				}
			}
			catch(...) { }

			try
			{
				GenApi::CEnumerationPtr ptrLineSource = m_pNodeMapDevice->_GetNode("LineSource");

				if(ptrLineSource.IsValid() && IsReadable(ptrLineSource))
				{
					GENICAM_NAMESPACE::gcstring gcstrLineSource = ptrLineSource->ToString();

					CStringA strLineSourceA(gcstrLineSource.c_str());
					CString strLineSource = CString(strLineSourceA);

					strLineSource.Replace(_T(" "), NULL);

					int nIdx = EDeviceGenICamLineSource_Count;

					for(int i = 0; i < EDeviceGenICamLineSource_Count; ++i)
					{
						if(!strLineSource.CompareNoCase(g_lpszGenICamLineSource[i]))
						{
							nIdx = i;
							break;
						}
					}

					SetLineSource_Line1(EDeviceGenICamLineSource(nIdx));
				}
			}
			catch(...) { }

			try
			{
				GenApi::CEnumerationPtr ptrLineFormat = m_pNodeMapDevice->_GetNode("LineFormat");

				if(ptrLineFormat.IsValid() && IsReadable(ptrLineFormat))
				{
					GENICAM_NAMESPACE::gcstring gcstrLineFormat = ptrLineFormat->ToString();

					CStringA strLineFormatA(gcstrLineFormat.c_str());
					CString strLineFormat = CString(strLineFormatA);

					strLineFormat.Replace(_T(" "), NULL);

					int nIdx = EDeviceGenICamLineFormat_Count;

					for(int i = 0; i < EDeviceGenICamLineFormat_Count; ++i)
					{
						if(!strLineFormat.CompareNoCase(g_lpszGenICamLineFormat[i]))
						{
							nIdx = i;
							break;
						}
					}

					SetLineFormat_Line1(EDeviceGenICamLineFormat(nIdx));
				}
			}
			catch(...) { }

			try
			{
				GenApi::CBooleanPtr ptrLineInverter = m_pNodeMapDevice->_GetNode("LineInverter");

				if(ptrLineInverter.IsValid() && IsReadable(ptrLineInverter))
				{
					bool bLineInverter = ptrLineInverter->GetValue();

					SetLineInverter_Line1(bLineInverter);
				}
			}
			catch(...) { }

			try
			{
				GenApi::CBooleanPtr ptrLineTermination = m_pNodeMapDevice->_GetNode("LineTermination");

				if(ptrLineTermination.IsValid() && IsReadable(ptrLineTermination))
				{
					bool bLineTermination = ptrLineTermination->GetValue();

					SetLineTermination_Line1(bLineTermination);
				}
			}
			catch(...) { }

			try
			{
				GenApi::CIntegerPtr ptrLineDebouncerTimeRaw = m_pNodeMapDevice->_GetNode("LineDebouncerTimeRaw");

				if(ptrLineDebouncerTimeRaw.IsValid() && IsReadable(ptrLineDebouncerTimeRaw))
				{
					int nLineDebouncerTimeRaw = ptrLineDebouncerTimeRaw->GetValue();

					SetLineDebouncerTimeRaw_Line1(nLineDebouncerTimeRaw);
				}
			}
			catch(...) { }

			try
			{
				GenApi::CFloatPtr ptrLineDebouncerTimeAbs = m_pNodeMapDevice->_GetNode("LineDebouncerTimeAbs");

				if(ptrLineDebouncerTimeAbs.IsValid() && IsReadable(ptrLineDebouncerTimeAbs))
				{
					float fLineDebouncerTimeAbs = ptrLineDebouncerTimeAbs->GetValue();

					SetLineDebouncerTimeAbs_Line1(fLineDebouncerTimeAbs);
				}
			}
			catch(...) { }
		}

		if(eLineSelect == EDeviceGenICamLineSelector_Line2)
		{
			try
			{
				GenApi::CEnumerationPtr ptrLineMode = m_pNodeMapDevice->_GetNode("LineMode");

				if(ptrLineMode.IsValid() && IsReadable(ptrLineMode))
				{
					GENICAM_NAMESPACE::gcstring gcstrLineMode = ptrLineMode->ToString();

					CStringA strLineModeA(gcstrLineMode.c_str());
					CString strLineMode = CString(strLineModeA);

					strLineMode.Replace(_T(" "), NULL);

					int nIdx = EDeviceGenICamLineMode_Count;

					for(int i = 0; i < EDeviceGenICamLineMode_Count; ++i)
					{
						if(!strLineMode.CompareNoCase(g_lpszGenICamLineMode[i]))
						{
							nIdx = i;
							break;
						}
					}

					SetLineMode_Line2(EDeviceGenICamLineMode(nIdx));
				}
			}
			catch(...) { }

			try
			{
				GenApi::CEnumerationPtr ptrLineSource = m_pNodeMapDevice->_GetNode("LineSource");

				if(ptrLineSource.IsValid() && IsReadable(ptrLineSource))
				{
					GENICAM_NAMESPACE::gcstring gcstrLineSource = ptrLineSource->ToString();

					CStringA strLineSourceA(gcstrLineSource.c_str());
					CString strLineSource = CString(strLineSourceA);

					strLineSource.Replace(_T(" "), NULL);

					int nIdx = EDeviceGenICamLineSource_Count;

					for(int i = 0; i < EDeviceGenICamLineSource_Count; ++i)
					{
						if(!strLineSource.CompareNoCase(g_lpszGenICamLineSource[i]))
						{
							nIdx = i;
							break;
						}
					}

					SetLineSource_Line2(EDeviceGenICamLineSource(nIdx));
				}
			}
			catch(...) { }

			try
			{
				GenApi::CEnumerationPtr ptrLineFormat = m_pNodeMapDevice->_GetNode("LineFormat");

				if(ptrLineFormat.IsValid() && IsReadable(ptrLineFormat))
				{
					GENICAM_NAMESPACE::gcstring gcstrLineFormat = ptrLineFormat->ToString();

					CStringA strLineFormatA(gcstrLineFormat.c_str());
					CString strLineFormat = CString(strLineFormatA);

					strLineFormat.Replace(_T(" "), NULL);

					int nIdx = EDeviceGenICamLineFormat_Count;

					for(int i = 0; i < EDeviceGenICamLineFormat_Count; ++i)
					{
						if(!strLineFormat.CompareNoCase(g_lpszGenICamLineFormat[i]))
						{
							nIdx = i;
							break;
						}
					}

					SetLineFormat_Line2(EDeviceGenICamLineFormat(nIdx));
				}
			}
			catch(...) { }

			try
			{
				GenApi::CBooleanPtr ptrLineInverter = m_pNodeMapDevice->_GetNode("LineInverter");

				if(ptrLineInverter.IsValid() && IsReadable(ptrLineInverter))
				{
					bool bLineInverter = ptrLineInverter->GetValue();

					SetLineInverter_Line2(bLineInverter);
				}
			}
			catch(...) { }

			try
			{
				GenApi::CBooleanPtr ptrLineTermination = m_pNodeMapDevice->_GetNode("LineTermination");

				if(ptrLineTermination.IsValid() && IsReadable(ptrLineTermination))
				{
					bool bLineTermination = ptrLineTermination->GetValue();

					SetLineTermination_Line2(bLineTermination);
				}
			}
			catch(...) { }

			try
			{
				GenApi::CIntegerPtr ptrLineDebouncerTimeRaw = m_pNodeMapDevice->_GetNode("LineDebouncerTimeRaw");

				if(ptrLineDebouncerTimeRaw.IsValid() && IsReadable(ptrLineDebouncerTimeRaw))
				{
					int nLineDebouncerTimeRaw = ptrLineDebouncerTimeRaw->GetValue();

					SetLineDebouncerTimeRaw_Line2(nLineDebouncerTimeRaw);
				}
			}
			catch(...) { }

			try
			{
				GenApi::CFloatPtr ptrLineDebouncerTimeAbs = m_pNodeMapDevice->_GetNode("LineDebouncerTimeAbs");

				if(ptrLineDebouncerTimeAbs.IsValid() && IsReadable(ptrLineDebouncerTimeAbs))
				{
					float fLineDebouncerTimeAbs = ptrLineDebouncerTimeAbs->GetValue();

					SetLineDebouncerTimeAbs_Line2(fLineDebouncerTimeAbs);
				}
			}
			catch(...) { }
		}

		if(eLineSelect == EDeviceGenICamLineSelector_Out1)
		{
			try
			{
				GenApi::CEnumerationPtr ptrLineMode = m_pNodeMapDevice->_GetNode("LineMode");

				if(ptrLineMode.IsValid() && IsReadable(ptrLineMode))
				{
					GENICAM_NAMESPACE::gcstring gcstrLineMode = ptrLineMode->ToString();

					CStringA strLineModeA(gcstrLineMode.c_str());
					CString strLineMode = CString(strLineModeA);

					strLineMode.Replace(_T(" "), NULL);

					int nIdx = EDeviceGenICamLineMode_Count;

					for(int i = 0; i < EDeviceGenICamLineMode_Count; ++i)
					{
						if(!strLineMode.CompareNoCase(g_lpszGenICamLineMode[i]))
						{
							nIdx = i;
							break;
						}
					}

					SetLineMode_Out1(EDeviceGenICamLineMode(nIdx));
				}
			}
			catch(...) { }

			try
			{
				GenApi::CEnumerationPtr ptrLineSource = m_pNodeMapDevice->_GetNode("LineSource");

				if(ptrLineSource.IsValid() && IsReadable(ptrLineSource))
				{
					GENICAM_NAMESPACE::gcstring gcstrLineSource = ptrLineSource->ToString();

					CStringA strLineSourceA(gcstrLineSource.c_str());
					CString strLineSource = CString(strLineSourceA);

					strLineSource.Replace(_T(" "), NULL);

					int nIdx = EDeviceGenICamLineSource_Count;

					for(int i = 0; i < EDeviceGenICamLineSource_Count; ++i)
					{
						if(!strLineSource.CompareNoCase(g_lpszGenICamLineSource[i]))
						{
							nIdx = i;
							break;
						}
					}

					SetLineSource_Out1(EDeviceGenICamLineSource(nIdx));
				}
			}
			catch(...) { }

			try
			{
				GenApi::CEnumerationPtr ptrLineFormat = m_pNodeMapDevice->_GetNode("LineFormat");

				if(ptrLineFormat.IsValid() && IsReadable(ptrLineFormat))
				{
					GENICAM_NAMESPACE::gcstring gcstrLineFormat = ptrLineFormat->ToString();

					CStringA strLineFormatA(gcstrLineFormat.c_str());
					CString strLineFormat = CString(strLineFormatA);

					strLineFormat.Replace(_T(" "), NULL);

					int nIdx = EDeviceGenICamLineFormat_Count;

					for(int i = 0; i < EDeviceGenICamLineFormat_Count; ++i)
					{
						if(!strLineFormat.CompareNoCase(g_lpszGenICamLineFormat[i]))
						{
							nIdx = i;
							break;
						}
					}

					SetLineFormat_Out1(EDeviceGenICamLineFormat(nIdx));
				}
			}
			catch(...) { }

			try
			{
				GenApi::CBooleanPtr ptrLineInverter = m_pNodeMapDevice->_GetNode("LineInverter");

				if(ptrLineInverter.IsValid() && IsReadable(ptrLineInverter))
				{
					bool bLineInverter = ptrLineInverter->GetValue();

					SetLineInverter_Out1(bLineInverter);
				}
			}
			catch(...) { }

			try
			{
				GenApi::CBooleanPtr ptrLineTermination = m_pNodeMapDevice->_GetNode("LineTermination");

				if(ptrLineTermination.IsValid() && IsReadable(ptrLineTermination))
				{
					bool bLineTermination = ptrLineTermination->GetValue();

					SetLineTermination_Out1(bLineTermination);
				}
			}
			catch(...) { }

			try
			{
				GenApi::CIntegerPtr ptrLineDebouncerTimeRaw = m_pNodeMapDevice->_GetNode("LineDebouncerTimeRaw");

				if(ptrLineDebouncerTimeRaw.IsValid() && IsReadable(ptrLineDebouncerTimeRaw))
				{
					int nLineDebouncerTimeRaw = ptrLineDebouncerTimeRaw->GetValue();

					SetLineDebouncerTimeRaw_Out1(nLineDebouncerTimeRaw);
				}
			}
			catch(...) { }

			try
			{
				GenApi::CFloatPtr ptrLineDebouncerTimeAbs = m_pNodeMapDevice->_GetNode("LineDebouncerTimeAbs");

				if(ptrLineDebouncerTimeAbs.IsValid() && IsReadable(ptrLineDebouncerTimeAbs))
				{
					float fLineDebouncerTimeAbs = ptrLineDebouncerTimeAbs->GetValue();

					SetLineDebouncerTimeAbs_Out1(fLineDebouncerTimeAbs);
				}
			}
			catch(...) { }
		}

		if(eLineSelect == EDeviceGenICamLineSelector_Out2)
		{
			try
			{
				GenApi::CEnumerationPtr ptrLineMode = m_pNodeMapDevice->_GetNode("LineMode");

				if(ptrLineMode.IsValid() && IsReadable(ptrLineMode))
				{
					GENICAM_NAMESPACE::gcstring gcstrLineMode = ptrLineMode->ToString();

					CStringA strLineModeA(gcstrLineMode.c_str());
					CString strLineMode = CString(strLineModeA);

					strLineMode.Replace(_T(" "), NULL);

					int nIdx = EDeviceGenICamLineMode_Count;

					for(int i = 0; i < EDeviceGenICamLineMode_Count; ++i)
					{
						if(!strLineMode.CompareNoCase(g_lpszGenICamLineMode[i]))
						{
							nIdx = i;
							break;
						}
					}

					SetLineMode_Out2(EDeviceGenICamLineMode(nIdx));
				}
			}
			catch(...) { }

			try
			{
				GenApi::CEnumerationPtr ptrLineSource = m_pNodeMapDevice->_GetNode("LineSource");

				if(ptrLineSource.IsValid() && IsReadable(ptrLineSource))
				{
					GENICAM_NAMESPACE::gcstring gcstrLineSource = ptrLineSource->ToString();

					CStringA strLineSourceA(gcstrLineSource.c_str());
					CString strLineSource = CString(strLineSourceA);

					strLineSource.Replace(_T(" "), NULL);

					int nIdx = EDeviceGenICamLineSource_Count;

					for(int i = 0; i < EDeviceGenICamLineSource_Count; ++i)
					{
						if(!strLineSource.CompareNoCase(g_lpszGenICamLineSource[i]))
						{
							nIdx = i;
							break;
						}
					}

					SetLineSource_Out2(EDeviceGenICamLineSource(nIdx));
				}
			}
			catch(...) { }

			try
			{
				GenApi::CEnumerationPtr ptrLineFormat = m_pNodeMapDevice->_GetNode("LineFormat");

				if(ptrLineFormat.IsValid() && IsReadable(ptrLineFormat))
				{
					GENICAM_NAMESPACE::gcstring gcstrLineFormat = ptrLineFormat->ToString();

					CStringA strLineFormatA(gcstrLineFormat.c_str());
					CString strLineFormat = CString(strLineFormatA);

					strLineFormat.Replace(_T(" "), NULL);

					int nIdx = EDeviceGenICamLineFormat_Count;

					for(int i = 0; i < EDeviceGenICamLineFormat_Count; ++i)
					{
						if(!strLineFormat.CompareNoCase(g_lpszGenICamLineFormat[i]))
						{
							nIdx = i;
							break;
						}
					}

					SetLineFormat_Out2(EDeviceGenICamLineFormat(nIdx));
				}
			}
			catch(...) { }

			try
			{
				GenApi::CBooleanPtr ptrLineInverter = m_pNodeMapDevice->_GetNode("LineInverter");

				if(ptrLineInverter.IsValid() && IsReadable(ptrLineInverter))
				{
					bool bLineInverter = ptrLineInverter->GetValue();

					SetLineInverter_Out2(bLineInverter);
				}
			}
			catch(...) { }

			try
			{
				GenApi::CBooleanPtr ptrLineTermination = m_pNodeMapDevice->_GetNode("LineTermination");

				if(ptrLineTermination.IsValid() && IsReadable(ptrLineTermination))
				{
					bool bLineTermination = ptrLineTermination->GetValue();

					SetLineTermination_Out2(bLineTermination);
				}
			}
			catch(...) { }

			try
			{
				GenApi::CIntegerPtr ptrLineDebouncerTimeRaw = m_pNodeMapDevice->_GetNode("LineDebouncerTimeRaw");

				if(ptrLineDebouncerTimeRaw.IsValid() && IsReadable(ptrLineDebouncerTimeRaw))
				{
					int nLineDebouncerTimeRaw = ptrLineDebouncerTimeRaw->GetValue();

					SetLineDebouncerTimeRaw_Out2(nLineDebouncerTimeRaw);
				}
			}
			catch(...) { }

			try
			{
				GenApi::CFloatPtr ptrLineDebouncerTimeAbs = m_pNodeMapDevice->_GetNode("LineDebouncerTimeAbs");

				if(ptrLineDebouncerTimeAbs.IsValid() && IsReadable(ptrLineDebouncerTimeAbs))
				{
					float fLineDebouncerTimeAbs = ptrLineDebouncerTimeAbs->GetValue();

					SetLineDebouncerTimeAbs_Out2(fLineDebouncerTimeAbs);
				}
			}
			catch(...) { }
		}

		if(eLineSelect == EDeviceGenICamLineSelector_Out3)
		{
			try
			{
				GenApi::CEnumerationPtr ptrLineMode = m_pNodeMapDevice->_GetNode("LineMode");

				if(ptrLineMode.IsValid() && IsReadable(ptrLineMode))
				{
					GENICAM_NAMESPACE::gcstring gcstrLineMode = ptrLineMode->ToString();

					CStringA strLineModeA(gcstrLineMode.c_str());
					CString strLineMode = CString(strLineModeA);

					strLineMode.Replace(_T(" "), NULL);

					int nIdx = EDeviceGenICamLineMode_Count;

					for(int i = 0; i < EDeviceGenICamLineMode_Count; ++i)
					{
						if(!strLineMode.CompareNoCase(g_lpszGenICamLineMode[i]))
						{
							nIdx = i;
							break;
						}
					}

					SetLineMode_Out3(EDeviceGenICamLineMode(nIdx));
				}
			}
			catch(...) { }

			try
			{
				GenApi::CEnumerationPtr ptrLineSource = m_pNodeMapDevice->_GetNode("LineSource");

				if(ptrLineSource.IsValid() && IsReadable(ptrLineSource))
				{
					GENICAM_NAMESPACE::gcstring gcstrLineSource = ptrLineSource->ToString();

					CStringA strLineSourceA(gcstrLineSource.c_str());
					CString strLineSource = CString(strLineSourceA);

					strLineSource.Replace(_T(" "), NULL);

					int nIdx = EDeviceGenICamLineSource_Count;

					for(int i = 0; i < EDeviceGenICamLineSource_Count; ++i)
					{
						if(!strLineSource.CompareNoCase(g_lpszGenICamLineSource[i]))
						{
							nIdx = i;
							break;
						}
					}

					SetLineSource_Out3(EDeviceGenICamLineSource(nIdx));
				}
			}
			catch(...) { }

			try
			{
				GenApi::CEnumerationPtr ptrLineFormat = m_pNodeMapDevice->_GetNode("LineFormat");

				if(ptrLineFormat.IsValid() && IsReadable(ptrLineFormat))
				{
					GENICAM_NAMESPACE::gcstring gcstrLineFormat = ptrLineFormat->ToString();

					CStringA strLineFormatA(gcstrLineFormat.c_str());
					CString strLineFormat = CString(strLineFormatA);

					strLineFormat.Replace(_T(" "), NULL);

					int nIdx = EDeviceGenICamLineFormat_Count;

					for(int i = 0; i < EDeviceGenICamLineFormat_Count; ++i)
					{
						if(!strLineFormat.CompareNoCase(g_lpszGenICamLineFormat[i]))
						{
							nIdx = i;
							break;
						}
					}

					SetLineFormat_Out3(EDeviceGenICamLineFormat(nIdx));
				}
			}
			catch(...) { }

			try
			{
				GenApi::CBooleanPtr ptrLineInverter = m_pNodeMapDevice->_GetNode("LineInverter");

				if(ptrLineInverter.IsValid() && IsReadable(ptrLineInverter))
				{
					bool bLineInverter = ptrLineInverter->GetValue();

					SetLineInverter_Out3(bLineInverter);
				}
			}
			catch(...) { }

			try
			{
				GenApi::CBooleanPtr ptrLineTermination = m_pNodeMapDevice->_GetNode("LineTermination");

				if(ptrLineTermination.IsValid() && IsReadable(ptrLineTermination))
				{
					bool bLineTermination = ptrLineTermination->GetValue();

					SetLineTermination_Out3(bLineTermination);
				}
			}
			catch(...) { }

			try
			{
				GenApi::CIntegerPtr ptrLineDebouncerTimeRaw = m_pNodeMapDevice->_GetNode("LineDebouncerTimeRaw");

				if(ptrLineDebouncerTimeRaw.IsValid() && IsReadable(ptrLineDebouncerTimeRaw))
				{
					int nLineDebouncerTimeRaw = ptrLineDebouncerTimeRaw->GetValue();

					SetLineDebouncerTimeRaw_Out3(nLineDebouncerTimeRaw);
				}
			}
			catch(...) { }

			try
			{
				GenApi::CFloatPtr ptrLineDebouncerTimeAbs = m_pNodeMapDevice->_GetNode("LineDebouncerTimeAbs");

				if(ptrLineDebouncerTimeAbs.IsValid() && IsReadable(ptrLineDebouncerTimeAbs))
				{
					float fLineDebouncerTimeAbs = ptrLineDebouncerTimeAbs->GetValue();

					SetLineDebouncerTimeAbs_Out3(fLineDebouncerTimeAbs);
				}
			}
			catch(...) { }
		}

		if(eLineSelect == EDeviceGenICamLineSelector_Out4)
		{
			try
			{
				GenApi::CEnumerationPtr ptrLineMode = m_pNodeMapDevice->_GetNode("LineMode");

				if(ptrLineMode.IsValid() && IsReadable(ptrLineMode))
				{
					GENICAM_NAMESPACE::gcstring gcstrLineMode = ptrLineMode->ToString();

					CStringA strLineModeA(gcstrLineMode.c_str());
					CString strLineMode = CString(strLineModeA);

					strLineMode.Replace(_T(" "), NULL);

					int nIdx = EDeviceGenICamLineMode_Count;

					for(int i = 0; i < EDeviceGenICamLineMode_Count; ++i)
					{
						if(!strLineMode.CompareNoCase(g_lpszGenICamLineMode[i]))
						{
							nIdx = i;
							break;
						}
					}

					SetLineMode_Out4(EDeviceGenICamLineMode(nIdx));
				}
			}
			catch(...) { }

			try
			{
				GenApi::CEnumerationPtr ptrLineSource = m_pNodeMapDevice->_GetNode("LineSource");

				if(ptrLineSource.IsValid() && IsReadable(ptrLineSource))
				{
					GENICAM_NAMESPACE::gcstring gcstrLineSource = ptrLineSource->ToString();

					CStringA strLineSourceA(gcstrLineSource.c_str());
					CString strLineSource = CString(strLineSourceA);

					strLineSource.Replace(_T(" "), NULL);

					int nIdx = EDeviceGenICamLineSource_Count;

					for(int i = 0; i < EDeviceGenICamLineSource_Count; ++i)
					{
						if(!strLineSource.CompareNoCase(g_lpszGenICamLineSource[i]))
						{
							nIdx = i;
							break;
						}
					}

					SetLineSource_Out4(EDeviceGenICamLineSource(nIdx));
				}
			}
			catch(...) { }

			try
			{
				GenApi::CEnumerationPtr ptrLineFormat = m_pNodeMapDevice->_GetNode("LineFormat");

				if(ptrLineFormat.IsValid() && IsReadable(ptrLineFormat))
				{
					GENICAM_NAMESPACE::gcstring gcstrLineFormat = ptrLineFormat->ToString();

					CStringA strLineFormatA(gcstrLineFormat.c_str());
					CString strLineFormat = CString(strLineFormatA);

					strLineFormat.Replace(_T(" "), NULL);

					int nIdx = EDeviceGenICamLineFormat_Count;

					for(int i = 0; i < EDeviceGenICamLineFormat_Count; ++i)
					{
						if(!strLineFormat.CompareNoCase(g_lpszGenICamLineFormat[i]))
						{
							nIdx = i;
							break;
						}
					}

					SetLineFormat_Out4(EDeviceGenICamLineFormat(nIdx));
				}
			}
			catch(...) { }

			try
			{
				GenApi::CBooleanPtr ptrLineInverter = m_pNodeMapDevice->_GetNode("LineInverter");

				if(ptrLineInverter.IsValid() && IsReadable(ptrLineInverter))
				{
					bool bLineInverter = ptrLineInverter->GetValue();

					SetLineInverter_Out4(bLineInverter);
				}
			}
			catch(...) { }

			try
			{
				GenApi::CBooleanPtr ptrLineTermination = m_pNodeMapDevice->_GetNode("LineTermination");

				if(ptrLineTermination.IsValid() && IsReadable(ptrLineTermination))
				{
					bool bLineTermination = ptrLineTermination->GetValue();

					SetLineTermination_Out4(bLineTermination);
				}
			}
			catch(...) { }

			try
			{
				GenApi::CIntegerPtr ptrLineDebouncerTimeRaw = m_pNodeMapDevice->_GetNode("LineDebouncerTimeRaw");

				if(ptrLineDebouncerTimeRaw.IsValid() && IsReadable(ptrLineDebouncerTimeRaw))
				{
					int nLineDebouncerTimeRaw = ptrLineDebouncerTimeRaw->GetValue();

					SetLineDebouncerTimeRaw_Out4(nLineDebouncerTimeRaw);
				}
			}
			catch(...) { }

			try
			{
				GenApi::CFloatPtr ptrLineDebouncerTimeAbs = m_pNodeMapDevice->_GetNode("LineDebouncerTimeAbs");

				if(ptrLineDebouncerTimeAbs.IsValid() && IsReadable(ptrLineDebouncerTimeAbs))
				{
					float fLineDebouncerTimeAbs = ptrLineDebouncerTimeAbs->GetValue();

					SetLineDebouncerTimeAbs_Out4(fLineDebouncerTimeAbs);
				}
			}
			catch(...) { }
		}

		try
		{
			GenApi::CEnumerationPtr ptrUserOutputSelector = m_pNodeMapDevice->_GetNode("UserOutputSelector");

			if(ptrUserOutputSelector.IsValid() && IsReadable(ptrUserOutputSelector))
			{
				GENICAM_NAMESPACE::gcstring gcstrUserOutputSelector = ptrUserOutputSelector->ToString();

				CStringA strUserOutputSelectorA(gcstrUserOutputSelector.c_str());
				CString strUserOutputSelector = CString(strUserOutputSelectorA);

				strUserOutputSelector.Replace(_T(" "), NULL);

				int nIdx = EDeviceGenICamUserOutputSelector_Count;

				for(int i = 0; i < EDeviceGenICamUserOutputSelector_Count; ++i)
				{
					if(!strUserOutputSelector.CompareNoCase(g_lpszGenICamUserOutputSelector[i]))
					{
						nIdx = i;
						break;
					}
				}

				SetUserOutputSelector(EDeviceGenICamUserOutputSelector(nIdx));
			}
		}
		catch(...) { }

		try
		{
			GenApi::CFloatPtr ptrTimerDurationTimebaseAbs = m_pNodeMapDevice->_GetNode("TimerDurationTimebaseAbs");

			if(ptrTimerDurationTimebaseAbs.IsValid() && IsReadable(ptrTimerDurationTimebaseAbs))
			{
				float fTimerDurationTimebaseAbs = ptrTimerDurationTimebaseAbs->GetValue();

				SetTimerDurationTimebaseAbs(fTimerDurationTimebaseAbs);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CFloatPtr ptrTimerDelayTimebaseAbs = m_pNodeMapDevice->_GetNode("TimerDelayTimebaseAbs");

			if(ptrTimerDelayTimebaseAbs.IsValid() && IsReadable(ptrTimerDelayTimebaseAbs))
			{
				float fTimerDelayTimebaseAbs = ptrTimerDelayTimebaseAbs->GetValue();

				SetTimerDelayTimebaseAbs(fTimerDelayTimebaseAbs);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CEnumerationPtr ptrTimerSelector = m_pNodeMapDevice->_GetNode("TimerSelector");

			if(ptrTimerSelector.IsValid() && IsReadable(ptrTimerSelector))
			{
				GENICAM_NAMESPACE::gcstring gcstrTimerSelector = ptrTimerSelector->ToString();

				CStringA strTimerSelectorA(gcstrTimerSelector.c_str());
				CString strTimerSelector = CString(strTimerSelectorA);

				strTimerSelector.Replace(_T(" "), NULL);

				int nIdx = EDeviceGenICamTimerSelector_Count;

				for(int i = 0; i < EDeviceGenICamTimerSelector_Count; ++i)
				{
					if(!strTimerSelector.CompareNoCase(g_lpszGenICamTimerSelector[i]))
					{
						nIdx = i;
						break;
					}
				}

				SetTimerSelector(EDeviceGenICamTimerSelector(nIdx));
			}
		}
		catch(...) { }

		try
		{
			GenApi::CFloatPtr ptrTimerDurationAbs = m_pNodeMapDevice->_GetNode("TimerDurationAbs");

			if(ptrTimerDurationAbs.IsValid() && IsReadable(ptrTimerDurationAbs))
			{
				float fTimerDurationAbs = ptrTimerDurationAbs->GetValue();

				SetTimerDurationAbs(fTimerDurationAbs);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CIntegerPtr ptrTimerDurationRaw = m_pNodeMapDevice->_GetNode("TimerDurationRaw");

			if(ptrTimerDurationRaw.IsValid() && IsReadable(ptrTimerDurationRaw))
			{
				int nTimerDurationRaw = ptrTimerDurationRaw->GetValue();

				SetTimerDurationRaw(nTimerDurationRaw);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CFloatPtr ptrTimerDelayAbs = m_pNodeMapDevice->_GetNode("TimerDelayAbs");

			if(ptrTimerDelayAbs.IsValid() && IsReadable(ptrTimerDelayAbs))
			{
				float fTimerDelayAbs = ptrTimerDelayAbs->GetValue();

				SetTimerDelayAbs(fTimerDelayAbs);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CIntegerPtr ptrTimerDelayRaw = m_pNodeMapDevice->_GetNode("TimerDelayRaw");

			if(ptrTimerDelayRaw.IsValid() && IsReadable(ptrTimerDelayRaw))
			{
				int nTimerDelayRaw = ptrTimerDelayRaw->GetValue();

				SetTimerDelayRaw(nTimerDelayRaw);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CEnumerationPtr ptrTimerTriggerSource = m_pNodeMapDevice->_GetNode("TimerTriggerSource");

			if(ptrTimerTriggerSource.IsValid() && IsReadable(ptrTimerTriggerSource))
			{
				GENICAM_NAMESPACE::gcstring gcstrTimerTriggerSource = ptrTimerTriggerSource->ToString();

				CStringA strTimerTriggerSourceA(gcstrTimerTriggerSource.c_str());
				CString strTimerTriggerSource = CString(strTimerTriggerSourceA);

				strTimerTriggerSource.Replace(_T(" "), NULL);

				int nIdx = EDeviceGenICamTimerTriggerSource_Count;

				for(int i = 0; i < EDeviceGenICamTimerTriggerSource_Count; ++i)
				{
					if(!strTimerTriggerSource.CompareNoCase(g_lpszGenICamTimerTriggerSource[i]))
					{
						nIdx = i;
						break;
					}
				}

				SetTimerTriggerSource(EDeviceGenICamTimerTriggerSource(nIdx));
			}
		}
		catch(...) { }

		try
		{
			GenApi::CEnumerationPtr ptrTimerTriggerActivation = m_pNodeMapDevice->_GetNode("TimerTriggerActivation");

			if(ptrTimerTriggerActivation.IsValid() && IsReadable(ptrTimerTriggerActivation))
			{
				GENICAM_NAMESPACE::gcstring gcstrTimerTriggerActivation = ptrTimerTriggerActivation->ToString();

				CStringA strTimerTriggerActivationA(gcstrTimerTriggerActivation.c_str());
				CString strTimerTriggerActivation = CString(strTimerTriggerActivationA);

				strTimerTriggerActivation.Replace(_T(" "), NULL);

				int nIdx = EDeviceGenICamTimerTriggerActivation_Count;

				for(int i = 0; i < EDeviceGenICamTimerTriggerActivation_Count; ++i)
				{
					if(!strTimerTriggerActivation.CompareNoCase(g_lpszGenICamTimerTriggerActivation[i]))
					{
						nIdx = i;
						break;
					}
				}

				SetTimerTriggerActivation(EDeviceGenICamTimerTriggerActivation(nIdx));
			}
		}
		catch(...) { }

		try
		{
			GenApi::CBooleanPtr ptrTimerSequenceEnable = m_pNodeMapDevice->_GetNode("TimerSequenceEnable");

			if(ptrTimerSequenceEnable.IsValid() && IsReadable(ptrTimerSequenceEnable))
			{
				bool bTimerSequenceEnable = ptrTimerSequenceEnable->GetValue();

				SetTimerSequenceEnable(bTimerSequenceEnable);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CIntegerPtr ptrTimerSequenceLastEntryIndex = m_pNodeMapDevice->_GetNode("TimerSequenceLastEntryIndex");

			if(ptrTimerSequenceLastEntryIndex.IsValid() && IsReadable(ptrTimerSequenceLastEntryIndex))
			{
				int nTimerSequenceLastEntryIndex = ptrTimerSequenceLastEntryIndex->GetValue();

				SetTimerSequenceLastEntryIndex(nTimerSequenceLastEntryIndex);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CIntegerPtr ptrTimerSequenceCurrentEntryIndex = m_pNodeMapDevice->_GetNode("TimerSequenceCurrentEntryIndex");

			if(ptrTimerSequenceCurrentEntryIndex.IsValid() && IsReadable(ptrTimerSequenceCurrentEntryIndex))
			{
				int nTimerSequenceCurrentEntryIndex = ptrTimerSequenceCurrentEntryIndex->GetValue();

				SetTimerSequenceCurrentEntryIndex(nTimerSequenceCurrentEntryIndex);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CEnumerationPtr ptrTimerSequenceEntrySelector = m_pNodeMapDevice->_GetNode("TimerSequenceEntrySelector");

			if(ptrTimerSequenceEntrySelector.IsValid() && IsReadable(ptrTimerSequenceEntrySelector))
			{
				GENICAM_NAMESPACE::gcstring gcstrTimerSequenceEntrySelector = ptrTimerSequenceEntrySelector->ToString();

				CStringA strTimerSequenceEntrySelectorA(gcstrTimerSequenceEntrySelector.c_str());
				CString strTimerSequenceEntrySelector = CString(strTimerSequenceEntrySelectorA);

				strTimerSequenceEntrySelector.Replace(_T(" "), NULL);

				int nIdx = EDeviceGenICamTimerSequenceEntrySelector_Count;

				for(int i = 0; i < EDeviceGenICamTimerSequenceEntrySelector_Count; ++i)
				{
					if(!strTimerSequenceEntrySelector.CompareNoCase(g_lpszGenICamTimerSequenceEntrySelector[i]))
					{
						nIdx = i;
						break;
					}
				}

				SetTimerSequenceEntrySelector(EDeviceGenICamTimerSequenceEntrySelector(nIdx));
			}
		}
		catch(...) { }

		try
		{
			GenApi::CEnumerationPtr ptrTimerSequenceTimerSelector = m_pNodeMapDevice->_GetNode("TimerSequenceTimerSelector");

			if(ptrTimerSequenceTimerSelector.IsValid() && IsReadable(ptrTimerSequenceTimerSelector))
			{
				GENICAM_NAMESPACE::gcstring gcstrTimerSequenceTimerSelector = ptrTimerSequenceTimerSelector->ToString();

				CStringA strTimerSequenceTimerSelectorA(gcstrTimerSequenceTimerSelector.c_str());
				CString strTimerSequenceTimerSelector = CString(strTimerSequenceTimerSelectorA);

				strTimerSequenceTimerSelector.Replace(_T(" "), NULL);

				int nIdx = EDeviceGenICamTimerSequenceTimerSelector_Count;

				for(int i = 0; i < EDeviceGenICamTimerSequenceTimerSelector_Count; ++i)
				{
					if(!strTimerSequenceTimerSelector.CompareNoCase(g_lpszGenICamTimerSequenceTimerSelector[i]))
					{
						nIdx = i;
						break;
					}
				}

				SetTimerSequenceTimerSelector(EDeviceGenICamTimerSequenceTimerSelector(nIdx));
			}
		}
		catch(...) { }

		try
		{
			GenApi::CBooleanPtr ptrTimerSequenceTimerEnable = m_pNodeMapDevice->_GetNode("TimerSequenceTimerEnable");

			if(ptrTimerSequenceTimerEnable.IsValid() && IsReadable(ptrTimerSequenceTimerEnable))
			{
				bool bTimerSequenceTimerEnable = ptrTimerSequenceTimerEnable->GetValue();

				SetTimerSequenceTimerEnable(bTimerSequenceTimerEnable);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CBooleanPtr ptrTimerSequenceTimerInverter = m_pNodeMapDevice->_GetNode("TimerSequenceTimerInverter");

			if(ptrTimerSequenceTimerInverter.IsValid() && IsReadable(ptrTimerSequenceTimerInverter))
			{
				bool bTimerSequenceTimerInverter = ptrTimerSequenceTimerInverter->GetValue();

				SetTimerSequenceTimerInverter(bTimerSequenceTimerInverter);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CIntegerPtr ptrTimerSequenceTimerDelayRaw = m_pNodeMapDevice->_GetNode("TimerSequenceTimerDelayRaw");

			if(ptrTimerSequenceTimerDelayRaw.IsValid() && IsReadable(ptrTimerSequenceTimerDelayRaw))
			{
				int nTimerSequenceTimerDelayRaw = ptrTimerSequenceTimerDelayRaw->GetValue();

				SetTimerSequenceTimerDelayRaw(nTimerSequenceTimerDelayRaw);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CIntegerPtr ptrTimerSequenceTimerDurationRaw = m_pNodeMapDevice->_GetNode("TimerSequenceTimerDurationRaw");

			if(ptrTimerSequenceTimerDurationRaw.IsValid() && IsReadable(ptrTimerSequenceTimerDurationRaw))
			{
				int nTimerSequenceTimerDurationRaw = ptrTimerSequenceTimerDurationRaw->GetValue();

				SetTimerSequenceTimerDurationRaw(nTimerSequenceTimerDurationRaw);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CEnumerationPtr ptrCounterSelector = m_pNodeMapDevice->_GetNode("CounterSelector");

			if(ptrCounterSelector.IsValid() && IsReadable(ptrCounterSelector))
			{
				GENICAM_NAMESPACE::gcstring gcstrCounterSelector = ptrCounterSelector->ToString();

				CStringA strCounterSelectorA(gcstrCounterSelector.c_str());
				CString strCounterSelector = CString(strCounterSelectorA);

				strCounterSelector.Replace(_T(" "), NULL);

				int nIdx = EDeviceGenICamCounterSelector_Count;

				for(int i = 0; i < EDeviceGenICamCounterSelector_Count; ++i)
				{
					if(!strCounterSelector.CompareNoCase(g_lpszGenICamCounterSelector[i]))
					{
						nIdx = i;
						break;
					}
				}

				SetCounterSelector(EDeviceGenICamCounterSelector(nIdx));
			}
		}
		catch(...) { }

		try
		{
			GenApi::CEnumerationPtr ptrCounterEventSource = m_pNodeMapDevice->_GetNode("CounterEventSource");

			if(ptrCounterEventSource.IsValid() && IsReadable(ptrCounterEventSource))
			{
				GENICAM_NAMESPACE::gcstring gcstrCounterEventSource = ptrCounterEventSource->ToString();

				CStringA strCounterEventSourceA(gcstrCounterEventSource.c_str());
				CString strCounterEventSource = CString(strCounterEventSourceA);

				strCounterEventSource.Replace(_T(" "), NULL);

				int nIdx = EDeviceGenICamCounterEventSource_Count;

				for(int i = 0; i < EDeviceGenICamCounterEventSource_Count; ++i)
				{
					if(!strCounterEventSource.CompareNoCase(g_lpszGenICamCounterEventSource[i]))
					{
						nIdx = i;
						break;
					}
				}

				SetCounterEventSource(EDeviceGenICamCounterEventSource(nIdx));
			}
		}
		catch(...) { }

		try
		{
			GenApi::CEnumerationPtr ptrCounterResetSource = m_pNodeMapDevice->_GetNode("CounterResetSource");

			if(ptrCounterResetSource.IsValid() && IsReadable(ptrCounterResetSource))
			{
				GENICAM_NAMESPACE::gcstring gcstrCounterResetSource = ptrCounterResetSource->ToString();

				CStringA strCounterResetSourceA(gcstrCounterResetSource.c_str());
				CString strCounterResetSource = CString(strCounterResetSourceA);

				strCounterResetSource.Replace(_T(" "), NULL);

				int nIdx = EDeviceGenICamCounterResetSource_Count;

				for(int i = 0; i < EDeviceGenICamCounterResetSource_Count; ++i)
				{
					if(!strCounterResetSource.CompareNoCase(g_lpszGenICamCounterResetSource[i]))
					{
						nIdx = i;
						break;
					}
				}

				SetCounterResetSource(EDeviceGenICamCounterResetSource(nIdx));
			}
		}
		catch(...) { }

		try
		{
			GenApi::CBooleanPtr ptrLUTEnable = m_pNodeMapDevice->_GetNode("LUTEnable");

			if(ptrLUTEnable.IsValid() && IsReadable(ptrLUTEnable))
			{
				bool bLUTEnable = ptrLUTEnable->GetValue();

				SetLUTEnable(bLUTEnable);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CIntegerPtr ptrLUTIndex = m_pNodeMapDevice->_GetNode("LUTIndex");

			if(ptrLUTIndex.IsValid() && IsReadable(ptrLUTIndex))
			{
				int nLUTIndex = ptrLUTIndex->GetValue();

				SetLUTIndex(nLUTIndex);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CIntegerPtr ptrLUTValue = m_pNodeMapDevice->_GetNode("LUTValue");

			if(ptrLUTValue.IsValid() && IsReadable(ptrLUTValue))
			{
				int nLUTValue = ptrLUTValue->GetValue();

				SetLUTValue(nLUTValue);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CEnumerationPtr ptrLUTSelector = m_pNodeMapDevice->_GetNode("LUTSelector");

			if(ptrLUTSelector.IsValid() && IsReadable(ptrLUTSelector))
			{
				GENICAM_NAMESPACE::gcstring gcstrLUTSelector = ptrLUTSelector->ToString();

				CStringA strLUTSelectorA(gcstrLUTSelector.c_str());
				CString strLUTSelector = CString(strLUTSelectorA);

				strLUTSelector.Replace(_T(" "), NULL);

				int nIdx = EDeviceGenICamLUTSelector_Count;

				for(int i = 0; i < EDeviceGenICamLUTSelector_Count; ++i)
				{
					if(!strLUTSelector.CompareNoCase(g_lpszGenICamLUTSelector[i]))
					{
						nIdx = i;
						break;
					}
				}

				SetLUTSelector(EDeviceGenICamLUTSelector(nIdx));
			}
		}
		catch(...) { }

		try
		{
			GenApi::CIntegerPtr ptrGevLinkSpeed = m_pNodeMapDevice->_GetNode("GevLinkSpeed");

			if(ptrGevLinkSpeed.IsValid() && IsReadable(ptrGevLinkSpeed))
			{
				int nGevLinkSpeed = ptrGevLinkSpeed->GetValue();

				SetGevLinkSpeed(nGevLinkSpeed);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CIntegerPtr ptrGevHeartbeatTimeout = m_pNodeMapDevice->_GetNode("GevHeartbeatTimeout");

			if(ptrGevHeartbeatTimeout.IsValid() && IsReadable(ptrGevHeartbeatTimeout))
			{
				int nGevHeartbeatTimeout = ptrGevHeartbeatTimeout->GetValue();

				SetGevHeartbeatTimeout(nGevHeartbeatTimeout);
			}
		}
		catch(...) { }

		try
		{
			GenApi::CIntegerPtr ptrGevSCPSPacketSize = m_pNodeMapDevice->_GetNode("GevSCPSPacketSize");

			if(ptrGevSCPSPacketSize.IsValid() && IsReadable(ptrGevSCPSPacketSize))
			{
				int nGevSCPSPacketSize = ptrGevSCPSPacketSize->GetValue();

				SetGevSCPSPacketSize(nGevSCPSPacketSize);
			}
		}
		catch(...) { }

	}
	while(false);
}

bool CDeviceGenICam::DoesModuleExist()
{
	bool bReturn = false;

	CString strModuleName;

	do
	{
		strModuleName.Format(_T("GCBase_MD_VC141_v3_2.dll"));

		EDeviceLibraryStatus eStatus = CLibraryManager::GetFoundLibrary(strModuleName);

		if(!eStatus)
		{
			HMODULE hModule = LoadLibraryEx(strModuleName, nullptr, DONT_RESOLVE_DLL_REFERENCES);

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

		strModuleName.Format(_T("GenApi_MD_VC141_v3_2.dll"));

		eStatus = CLibraryManager::GetFoundLibrary(strModuleName);

		if(!eStatus)
		{
			HMODULE hModule = LoadLibraryEx(strModuleName, nullptr, DONT_RESOLVE_DLL_REFERENCES);

			if(!hModule)
			{
				CLibraryManager::SetFoundLibrary(strModuleName, false);

				bReturn = false;

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

bool CDeviceGenICam::SplitLocalUrl(const std::string & strLocal, std::string & fullFilename, uint64_t & address, size_t & length)
{
	std::stringstream ss;

	std::string slashes("///");
	if(strLocal.find(slashes) == std::string::npos)
		ss << strLocal;
	else
		ss << strLocal.substr(slashes.length());

	std::getline(ss, fullFilename, ';');

	uint64_t addr = 0;
	size_t len = 0;

	ss >> std::hex >> addr;

	if(ss.peek() == ';')
		ss.ignore();
	else
		return false;

	ss >> len;

	if(ss.fail())
		return false;

	address = addr;
	length = len;

	return true;
}

UINT CDeviceGenICam::CallbackFunction(LPVOID pParam)
{
	CDeviceGenICam* pInstance = (CDeviceGenICam*)pParam;

	if(pInstance)
	{
		do
		{
			if(!pInstance->m_hModule)
				break;

			if(!pInstance->m_pNodeMapDevice)
				break;

			GenTL::PDSGetBufferInfo DSGetBufferInfo(reinterpret_cast<GenTL::PDSGetBufferInfo>(reinterpret_cast<void *>(GetProcAddress(pInstance->m_hModule, ("DSGetBufferInfo")))));

			GenTL::PEventGetData EventGetData(reinterpret_cast<GenTL::PEventGetData>(reinterpret_cast<void *>(GetProcAddress(pInstance->m_hModule, ("EventGetData")))));

			GenTL::PDSQueueBuffer DSQueueBuffer(reinterpret_cast<GenTL::PDSQueueBuffer>(reinterpret_cast<void *>(GetProcAddress(pInstance->m_hModule, ("DSQueueBuffer")))));

			GenTL::PDSFlushQueue DSFlushQueue(reinterpret_cast<GenTL::PDSFlushQueue>(reinterpret_cast<void *>(GetProcAddress(pInstance->m_hModule, ("DSFlushQueue")))));

			GenTL::GC_ERROR status = GenTL::GC_ERR_SUCCESS;

			GenApi::CCommandPtr ptrAcquisitionStart = pInstance->m_pNodeMapDevice->_Ptr->GetNode("AcquisitionStart");

			ptrAcquisitionStart->Execute();

			do
			{
				GenTL::EVENT_NEW_BUFFER_DATA bufferData;

				DSFlushQueue(m_hDS, GenTL::ACQ_QUEUE_ALL_TO_INPUT);
				DSFlushQueue(m_hDS, GenTL::ACQ_QUEUE_OUTPUT_DISCARD);

				size_t evtSize = sizeof(bufferData);

				status = EventGetData(m_hNewEvent, &bufferData, &evtSize, 1000);

				if(status == GenTL::GC_ERR_SUCCESS)
				{
					pInstance->NextImageIndex();

					CRavidImage* pCurrentImage = pInstance->GetImageInfo();

					BYTE* pCurrentBuffer = pCurrentImage->GetBuffer();
					BYTE** ppCurrentOffsetY = pCurrentImage->GetYOffsetTable();

					const int64_t i64Width = (int64_t)pCurrentImage->GetSizeX();
					const int64_t i64Height = (int64_t)pCurrentImage->GetSizeY();
					const int64_t i64WidthStep = (int64_t)pCurrentImage->GetWidthStep();
					const int64_t i64ImageSizeByte = (int64_t)pCurrentImage->GetImageSizeByte();
					const int64_t i64PixelSizeByte = (int64_t)pCurrentImage->GetPixelSizeByte();

					GenTL::INFO_DATATYPE iType = GenTL::INFO_DATATYPE_UNKNOWN;

					void* pImageData = nullptr;
					size_t iSize = sizeof(pImageData);

					status = DSGetBufferInfo(m_hDS, bufferData.BufferHandle, GenTL::BUFFER_INFO_BASE, &iType, &pImageData, &iSize);

					if(status < 0)
						break;

					EDeviceGenICamPixelFormat ePixelFormat;
					pInstance->GetPixelFormat(&ePixelFormat);

					switch(ePixelFormat)
					{
					case EDeviceGenICamPixelFormat_Mono8:
					case EDeviceGenICamPixelFormat_Mono10:
					case EDeviceGenICamPixelFormat_Mono12:
					case EDeviceGenICamPixelFormat_Mono14:
					case EDeviceGenICamPixelFormat_Mono16:
					case EDeviceGenICamPixelFormat_RGB8:
					case EDeviceGenICamPixelFormat_RGB10:
					case EDeviceGenICamPixelFormat_RGB12:
					case EDeviceGenICamPixelFormat_RGB16:
						{
							for(int64_t i = 0; i < i64Height; ++i)
								memcpy(pCurrentBuffer + i * i64WidthStep, ((char*)pImageData) + i * i64Width * i64PixelSizeByte, i64Width * i64PixelSizeByte);
						}
						break;
					case EDeviceGenICamPixelFormat_Mono10Packed:
						{
							if(!(i64Width % 2) && !(i64Height % 2))
							{
								BYTE* pSrc = (BYTE*)pImageData;
								BYTE* pSrcEnd = (BYTE*)pImageData + ((i64Width / 2 * 3) * i64Height);
								USHORT* pDest = (USHORT*)pCurrentBuffer;
								USHORT* pBufferEnd = (USHORT*)(pCurrentBuffer + i64ImageSizeByte);

								USHORT pixel;

								while(pSrc < pSrcEnd)
								{
									for(ULONG i = 0; i < (ULONG)i64Width && pSrc < pSrcEnd; i += 2)
									{
										BYTE LByte = (*(pSrc));
										BYTE MByte = (*(pSrc + 1));
										BYTE UByte = (*(pSrc + 2));

										if(pDest < pBufferEnd)
										{
											pixel = ((USHORT)MByte & 0xC0) << 2;
											pixel += ((USHORT)LByte);
											*(pDest++) = pixel;

											if(pDest < pBufferEnd)
											{
												pixel = ((USHORT)MByte & 0x0C) << 6;
												pixel += ((USHORT)UByte);
												*(pDest++) = pixel;
											}
										}

										pSrc += 3;
									}
								}
							}
						}
						break;
					case EDeviceGenICamPixelFormat_Mono12Packed:
						{
							if(!(i64Width % 2) && !(i64Height % 2))
							{
								BYTE* pSrc = (BYTE*)pImageData;
								BYTE* pSrcEnd = (BYTE*)pImageData + ((i64Width / 2 * 3) * i64Height);
								USHORT* pDest = (USHORT*)pCurrentBuffer;
								USHORT* pBufferEnd = (USHORT*)(pCurrentBuffer + i64ImageSizeByte);

								USHORT pixel;

								while(pSrc < pSrcEnd)
								{
									for(ULONG i = 0; i < (ULONG)i64Width && pSrc < pSrcEnd; i += 2)
									{
										BYTE LByte = (*(pSrc));
										BYTE MByte = (*(pSrc + 1));
										BYTE UByte = (*(pSrc + 2));

										if(pDest < pBufferEnd)
										{
											pixel = ((USHORT)LByte) << 4;
											pixel += ((USHORT)MByte & 0xF0) >> 4;
											*(pDest++) = pixel;

											if(pDest < pBufferEnd)
											{
												pixel = ((USHORT)UByte) << 4;
												pixel += ((USHORT)MByte & 0x0F);
												*(pDest++) = pixel;
											}
										}

										pSrc += 3;
									}
								}
							}
						}
						break;
					default:
						break;
					}

					CRavidImageView* pCurView = pInstance->GetImageView();
					if(pCurView)
						pCurView->Invalidate(false);

					status = DSQueueBuffer(m_hDS, bufferData.BufferHandle);

					if(status < 0)
						break;
				}
			}
			while(pInstance->IsInitialized() && pInstance->IsLive());

			GenApi::CCommandPtr ptrAcquisitionStop = pInstance->m_pNodeMapDevice->_Ptr->GetNode("AcquisitionStop");

			ptrAcquisitionStop->Execute();
		}
		while(false);

		pInstance->m_bIsLive = false;
		pInstance->m_bIsGrabAvailable = true;
	}

	return 0;
}

#endif