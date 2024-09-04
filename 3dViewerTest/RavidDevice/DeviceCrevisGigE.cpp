#include "stdafx.h"

#include "DeviceCrevisGigE.h"

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
#include "../RavidFramework/RavidImageView.h"
#include "../RavidFramework/SequenceManager.h"
#include "../RavidFramework/AuthorityManager.h"
#include "../RavidFramework/FrameworkManager.h"
#include "../RavidFramework/EventHandlerManager.h" 
#include "../RavidFramework/MultiLanguageManager.h"

#include "../RavidFramework/RavidTreeView.h"
#include "../RavidFramework/RavidImageView.h"

#include "../Libraries/Includes/CrevisGigE/VirtualFG40.h"

// virtualfg40.dll
#pragma comment(lib, COMMONLIB_PREFIX "CrevisGigE/VirtualFG40.lib")

using namespace Ravid;
using namespace Ravid::Device;
using namespace Ravid::Database;
using namespace Ravid::Framework;
using namespace Ravid::Algorithms;
using namespace Ravid::Miscellaneous;

IMPLEMENT_DYNAMIC(CDeviceCrevisGigE, CDeviceCamera)

BEGIN_MESSAGE_MAP(CDeviceCrevisGigE, CDeviceCamera)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static LPCTSTR g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_Count] =
{
	_T("DeviceID"),
	_T("General Parameter"),
	_T("Initialize Mode"),
	_T("AcqInvalid Time"),
	_T("Grab Timeout"),
	_T("Detailed Log"),
	_T("GevHeartbeat Timeout"),
	_T("GevGVCP HeartbeatDisable"),
	_T("GevSCPS PacketSize"),
	_T("ImageControl"),
	_T("Width"),
	_T("Height"),
	_T("Pixel Format"),
	_T("OffsetX"),
	_T("OffsetY"),
	_T("Test Pattern"),
	_T("Acquisition Control"),
	_T("Acquisition Mode"),
	_T("Acquisition FrameCount"),
	_T("Trigger Selector"),
	_T("Trigger Source"),
	_T("Line1"),
	_T("Line1 TriggerMode"),
	_T("Line1 TriggerActivation"),
	_T("Line1 TriggerDelay"),
	_T("Software"),
	_T("Software TriggerMode"),
	_T("Software TriggerActivation"),
	_T("Software TriggerDelay"),
	_T("Exposure Mode"),
	_T("Exposure Auto"),
	_T("Exposure Time"),
	_T("Acquisition FrameRateEnable"),
	_T("Acquisition FrameRate"),
	_T("AutoExposure Target"),
	_T("Digital IO Control"),
	_T("Line Selector"),
	_T("Line Inverter"),
	_T("Line Source"),
	_T("UserOutput Selector"),
	_T("Timer Selector"),
	_T("Timer Duration"),
	_T("Timer Delay"),
	_T("LUT Control"),
	_T("LUT Enable"),
	_T("LUT Index"),
	_T("LUT Value"),
	_T("Analog Control"),
	_T("Gain Selector"),
	_T("Gain Raw"),
	_T("Gain Auto"),
	_T("BlackLevel Selector"),
	_T("BlackLevel Raw"),
	_T("BalanceRatio Selector"),
	_T("Color Only"),
	_T("Balance Ratio"),
	_T("BalanceWhite Auto"),
	_T("User Control"),
	_T("UserSet Selector"),
	_T("Device Option Control"),
	_T("DeviceFilter DriverMode"),
	_T("DeviceCommand Timeout"),
	_T("DeviceCommand RetryNumber"),
	_T("DeviceMissing PacketReceive"),
	_T("Device PacketResend"),
	_T("Device MaxPacketResendCount"),
	_T("ColorTrans Control"),
	_T("ColorTransformation Enable"),
	_T("ColorTransformation ValueSelector"),
	_T("ColorTransformation Value"),
};

static LPCTSTR g_lpszCrevisGigEInitMode[EDeviceCrevisGigEInitMode_Count] =
{
	_T("Device Only"),
	_T("Parameter"),
};

static LPCTSTR g_lpszCrevisGigEPixelFormat[EDeviceCrevisGigEPixelFormat_Count] =
{
	_T("Mono8"),
	_T("Mono10"),
	_T("Mono12"),
	_T("Mono14"),
	_T("Mono10Packed"),
	_T("Mono12Packed"),
	_T("BayerBG8"),
	_T("BayerBG10"),
	_T("BayerBG12"),
	_T("BayerBG10Packed"),
	_T("BayerBG12Packed"),
	_T("BayerRG8"),
	_T("BayerRG10"),
	_T("BayerRG12"),
	_T("BayerRG10Packed"),
	_T("BayerRG12Packed"),
	_T("BayerGR8"),
	_T("BayerGR10"),
	_T("BayerGR12"),
	_T("BayerGR10Packed"),
	_T("BayerGR12Packed"),
	_T("BayerGB8"),
	_T("BayerGB10"),
	_T("BayerGB12"),
	_T("BayerGB10Packed"),
	_T("BayerGB12Packed"),
	_T("YUV422Packed"),
	_T("RGB8Packed"),
	_T("BGR8Packed"),
};

static LPCTSTR g_lpszCrevisGigETestPattern[EDeviceCrevisGigETestPattern_Count] =
{
	_T("Off"),
	_T("GreyHorizontalRamp"),
	_T("GreyVerticalRamp"),
};

static LPCTSTR g_lpszCrevisGigEAcquistionMode[EDeviceCrevisGigEAcquistionMode_Count] =
{
	_T("Continuous"),
	_T("SingleFrame"),
	_T("MultiFrame"),
};

static LPCTSTR g_lpszCrevisGigETriggerSelector[EDeviceCrevisGigETriggerSelector_Count] =
{
	_T("FrameStart"),
};

static LPCTSTR g_lpszCrevisGigETriggerMode[EDeviceCrevisGigETriggerMode_Count] =
{
	_T("Off"),
	_T("On"),
};

static LPCTSTR g_lpszCrevisGigETriggerSource[EDeviceCrevisGigETriggerSource_Count] =
{
	_T("Line1"),
	_T("Software"),
};

static LPCTSTR g_lpszCrevisGigETriggerActivation[EDeviceCrevisGigETriggerActivation_Count] =
{
	_T("RisingEdge"),
	_T("FallingEdge"),
	_T("LevelLow"),
	_T("LevelHigh"),
};

static LPCTSTR g_lpszCrevisGigEExposureMode[EDeviceCrevisGigEExposureMode_Count] =
{
	_T("Timed"),
	_T("TriggerWidth"),
};

static LPCTSTR g_lpszCrevisGigEExposureAuto[EDeviceCrevisGigEExposureAuto_Count] =
{
	_T("Off"),
	_T("Once"),
	_T("Continuous"),
};

static LPCTSTR g_lpszCrevisGigEAcquistionFramerateEnable[EDeviceCrevisGigEAcquistionFramerateEnable_Count] =
{
	_T("Off"),
	_T("On"),
};

static LPCTSTR g_lpszCrevisGigELineSelector[EDeviceCrevisGigELineSelector_Count] =
{
	_T("Line1"),
	_T("Line2"),
};

static LPCTSTR g_lpszCrevisGigELineMode[EDeviceCrevisGigELineMode_Count] =
{
	_T("Input"),
	_T("Output"),
};

static LPCTSTR g_lpszCrevisGigELineSource[EDeviceCrevisGigELineSource_Count] =
{
	_T("Off"),
	_T("ExposureActive"),
	_T("TimerActive"),
	_T("UserOutput1"),
};

static LPCTSTR g_lpszCrevisGigEUserOutputSelector[EDeviceCrevisGigEUserOutputSelector_Count] =
{
	_T("UserOutput1"),
};

static LPCTSTR g_lpszCrevisGigETimerSelector[EDeviceCrevisGigETimerSelector_Count] =
{
	_T("Timer1"),
};

static LPCTSTR g_lpszCrevisGigEGainSelector[EDeviceCrevisGigEGainSelector_Count] =
{
	_T("All"),
};

static LPCTSTR g_lpszCrevisGigEGainAuto[EDeviceCrevisGigEGainAuto_Count] =
{
	_T("Off"),
	_T("Once"),
	_T("Continuous"),
};

static LPCTSTR g_lpszCrevisGigEBlackLevelSelector[EDeviceCrevisGigEBlackLevelSelector_Count] =
{
	_T("All"),
};

static LPCTSTR g_lpszCrevisGigEBalanceRatioSelector[EDeviceCrevisGigEBalanceRatioSelector_Count] =
{
	_T("Red"),
	_T("Green"),
	_T("Blue"),
};

static LPCTSTR g_lpszCrevisGigEBalanceWhiteAuto[EDeviceCrevisGigEBalanceWhiteAuto_Count] =
{
	_T("Off"),
	_T("Once"),
	_T("Continuous"),
};

static LPCTSTR g_lpszCrevisGigEGevDeviceModeCharacterSet[EDeviceCrevisGigEGevDeviceModeCharacterSet_Count] =
{
	_T("UTF8"),
};

static LPCTSTR g_lpszCrevisGigEGevSupportedOptionSelector[EDeviceCrevisGigEGevSupportedOptionSelector_Count] =
{
	_T("UserDefinedName"),
	_T("SerialNumber"),
	_T("HeartbeatDisable"),
	_T("LinkSpeed"),
	_T("CCPApplicationSocket"),
	_T("ManifestTable"),
	_T("TestData"),
	_T("DiscoveryAckDelay"),
	_T("DiscoveryAckDelayWritable"),
	_T("ExtendedStatusCodes"),
	_T("PrimaryApplicationSwitchover"),
	_T("Action"),
	_T("PendingAck"),
	_T("EventData"),
	_T("Event"),
	_T("PacketResend"),
	_T("WriteMem"),
	_T("CommandsConcatenation"),
	_T("IPConfigurationLLA"),
	_T("IPConfigurationDHCP"),
	_T("IPConfigurationPersistentIP"),
	_T("StreamChannelSourceSocket"),
	_T("MessageChannelSourceSocket"),
	_T("StreamChannel0BigAndLittleEndian"),
	_T("StreamChannel0IPReassembly"),
	_T("StreamChannel0UnconditionalStreaming"),
	_T("StreamChannel0ExtendedChunkData"),
};

static LPCTSTR g_lpszCrevisGigEGevCCP[EDeviceCrevisGigEGevCCP_Count] =
{
	_T("OpenAccess"),
	_T("ExclusiveAccess"),
	_T("ControlAccess"),
};

static LPCTSTR g_lpszCrevisGigEUserSetSelector[EDeviceCrevisGigEUserSetSelector_Count] =
{
	_T("Default"),
	_T("UserSet1"),
	_T("UserSet2"),
	_T("UserSet3"),
};

static LPCTSTR g_lpszCrevisGigEColorTransformationValueSeletor[EDeviceCrevisGigEColorTransformationValueSeletor_Count] =
{
	_T("HUEBYGP"),
	_T("HUEBYGN"),
	_T("HUEBYHP"),
	_T("HUEBYHN"),
	_T("HUERYGP"),
	_T("HUERYGN"),
	_T("HUERYHP"),
	_T("HUERYHN"),
	_T("HUECG"),
};

static LPCTSTR g_lpszCrevisGigEDeviceFilterDriverMode[EDeviceCrevisGigEDeviceFilterDriverMode_Count] =
{
	_T("Off"),
	_T("On"),
};

static LPCTSTR g_lpszCrevisGigEDeviceMissingPackedReceive[EDeviceCrevisGigEDeviceMissingPackedReceive_Count] =
{
	_T("Off"),
	_T("On"),
};

static LPCTSTR g_lpszCrevisGigESwitch[EDeviceCrevisGigESwitch_Count] =
{
	_T("Off"),
	_T("On")
};

static LPCTSTR g_lpszCrevisGigEColorModel[EDeviceCrevisGigEColorModel_Count] = 
{
	_T("MV-KH20G"),
	_T("MG-D500C"),
	_T("MG-D200C-S"),
	_T("MG-D200C"),
	_T("MG-D130C"),
	_T("MG-D030C"),
	_T("MG-D030C-S"),
	_T("MV-KQ60G-P"),
	_T("MG-A320K-35"),
	_T("MG-A500K-22"),
	_T("MG-A121K-9"),
	_T("MG-A121L-9"),
	_T("MG-A201L-5"),
	_T("MG-A040K-280"),
	_T("MG-A160K-72"),
};

CDeviceCrevisGigE::CDeviceCrevisGigE()
{
}


CDeviceCrevisGigE::~CDeviceCrevisGigE()
{
	Terminate();
}

EDeviceInitializeResult CDeviceCrevisGigE::Initialize()
{
	EDeviceInitializeResult eReturn = EDeviceInitializeResult_UnknownError;

	CString strMessage, strStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("CrevisGigE"));

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

		bool bNeedInitialize = true;

		for(int i = 0; i < (int)CDeviceManager::GetDeviceCount(); ++i)
		{
			CDeviceCrevisGigE *pDevice = dynamic_cast<CDeviceCrevisGigE*>(CDeviceManager::GetDeviceByIndex(typeid(CDeviceCrevisGigE), i));
			if(!pDevice)
				continue;

			if(pDevice == this)
				continue;

			if(!pDevice->IsInitialized())
				continue;

			if(pDevice->GetDeviceID() == GetDeviceID())
				continue;

			bNeedInitialize = false;

			break;
		}

		if(bNeedInitialize)
		{
			if(ST_InitSystem())
			{
				strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntopenthelibrary);
				eReturn = EDeviceInitializeResult_NotFoundLibraries;
				break;
			}
		}

		bool bUsed = false;
		if(ST_IsInitSystem(&bUsed))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Init System"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		if(!bUsed)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntopenthelibrary);
			eReturn = EDeviceInitializeResult_NotFoundLibraries;
			break;
		}

		if(ST_UpdateDevice())
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Update Device"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		unsigned int unNum = 0;

		if(ST_GetAvailableCameraNum(&unNum))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Camera Num"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		CString strBuffer;
		CString strSerialCode;
		unsigned int unBufferSize = 256;
		char* pBuffer = new char[256];

		m_bColorCamera = false;

		for(unsigned int i = 0; i < unNum; ++i)
		{
			unBufferSize = 256;
			memset(pBuffer, 0, sizeof(char) * unBufferSize);

			if(ST_GetEnumDeviceInfo(i, MCAM_DEVICEINFO_SERIAL_NUMBER, pBuffer, &unBufferSize))
				continue;

			if(!unBufferSize)
				continue;

			strBuffer = pBuffer;

			strSerialCode = strBuffer.Mid(3);// remove SN_

			if(strSerialCode.CompareNoCase(GetDeviceID()))
				continue;

			bool bOpen = false;

			for(int i = 0; i < (int)CDeviceManager::GetDeviceCount(); ++i)
			{
				CDeviceCrevisGigE *pDevice = dynamic_cast<CDeviceCrevisGigE*>(CDeviceManager::GetDeviceByIndex(typeid(CDeviceCrevisGigE), i));
				if(!pDevice)
					continue;

				if(pDevice == this)
					continue;

				if(pDevice->GetDeviceID() == GetDeviceID())
					continue;

				if(!pDevice->IsInitialized())
					continue;

				bOpen = true;

				break;
			}

			if(bOpen)
				break;

			int nError = ST_OpenDevice(i, &m_hDevice);
			if(nError)
				break;

			if(ST_IsOpenDevice(m_hDevice, &bOpen))
				break;

			memset(pBuffer, 0, unBufferSize);
			if(ST_GetEnumDeviceInfo(i, MCAM_DEVICEINFO_MODEL_NAME, pBuffer, &unBufferSize))
				continue;

			if(!unBufferSize)
				continue;

			strBuffer = pBuffer;

			for(int j = 0; j < EDeviceCrevisGigEColorModel_Count; ++j)
			{
				if(!strBuffer.CompareNoCase(g_lpszCrevisGigEColorModel[j]))
					m_bColorCamera = true;
			}

			break;
		}

		if(pBuffer)
		{
			delete pBuffer;
			pBuffer = nullptr;
		}

		if(m_hDevice < 0)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtofindthedevice);
			eReturn = EDeviceInitializeResult_NotFoundDeviceError;
			break;
		}

		EDeviceCrevisGigEInitMode eInitType = EDeviceCrevisGigEInitMode_DeviceOnly;

		if(GetInitializeMode(&eInitType))
			break;

		bool bParamOK = false;

		int nWidth = 1024;
		int nHeight = 1024;
		CString strFormat = _T("");

		CString strError = _T("");

		switch(eInitType)
		{
		case EDeviceCrevisGigEInitMode_DeviceOnly:
			{
				UpdateDeviceToParameter();

				strError = _T("Width");
				int nParam = 0;
				if(GetWidth(&nParam))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Width"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				nWidth = nParam;

				strError = _T("Height");
				nParam = 0;
				if(GetHeight(&nParam))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Height"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				nHeight = nParam;

				strError = _T("PixelFormat");
				EDeviceCrevisGigEPixelFormat ePFParam = EDeviceCrevisGigEPixelFormat_Mono8;
				if(GetPixelFormat(&ePFParam))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("PixelFormat"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				strFormat = g_lpszCrevisGigEPixelFormat[ePFParam];

				bParamOK = true;
			}
			break;
		case EDeviceCrevisGigEInitMode_Parameter:
			{
				try
				{
					strError = _T("AcqInvalidTime");
					int nParam = 0;
					if(GetAcqInvalidTime(&nParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AcqInvalidTime"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
					if(ST_SetAcqInvalidTime(m_hDevice, nParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("AcqInvalidTime"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					strError = _T("GrabTimeout");
					nParam = 0;
					if(GetGrabTimeout(&nParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("GrabTimeout"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
					if(ST_SetGrabTimeout(m_hDevice, nParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("GrabTimeout"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					strError = _T("DetailedLog");
					bool bParam = 0;
					if(GetDetailedLog(&bParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("DetailedLog"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
					if(ST_SetDetailedLog(m_hDevice, bParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("DetailedLog"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					//reset offset
					strError = _T("OffsetX reset");
					nParam = 0;
					if(ST_SetIntReg(m_hDevice, "OffsetX", nParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("OffsetX reset"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}
					strError = _T("OffsetY reset");
					if(ST_SetIntReg(m_hDevice, "OffsetY", nParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("OffsetY reset"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					strError = _T("Width");
					nParam = 0;
					if(GetWidth(&nParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Width"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
					if(ST_SetIntReg(m_hDevice, "Width", nParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Width"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}
					nWidth = nParam;

					strError = _T("Height");
					nParam = 0;
					if(GetHeight(&nParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Height"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
					if(ST_SetIntReg(m_hDevice, "Height", nParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Height"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}
					nHeight = nParam;

					strError = _T("PixelFormat");
					EDeviceCrevisGigEPixelFormat ePFParam = EDeviceCrevisGigEPixelFormat_Mono8;
					if(GetPixelFormat(&ePFParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("PixelFormat"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
					CStringA strTemp(g_lpszCrevisGigEPixelFormat[ePFParam]);
					if(ST_SetEnumReg(m_hDevice, "PixelFormat", (LPSTR)(LPCSTR)(strTemp)))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("PixelFormat"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}
					strFormat = g_lpszCrevisGigEPixelFormat[ePFParam];

					strError = _T("OffsetX");
					nParam = 0;
					if(GetOffsetX(&nParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("OffsetX"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
					if(ST_SetIntReg(m_hDevice, "OffsetX", nParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("OffsetX"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					strError = _T("OffsetY");
					nParam = 0;
					if(GetOffsetY(&nParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("OffsetY"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
					if(ST_SetIntReg(m_hDevice, "OffsetY", nParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("OffsetY"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					strError = _T("TestPattern");
					EDeviceCrevisGigETestPattern eTPParam = EDeviceCrevisGigETestPattern_Off;
					if(GetTestPattern(&eTPParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TestPattern"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
					strTemp = g_lpszCrevisGigETestPattern[eTPParam];
					if(ST_SetEnumReg(m_hDevice, "TestPattern", (LPSTR)(LPCSTR)(strTemp)))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TestPattern"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					//Acquisition control
					strError = _T("AcquisitionMode");
					EDeviceCrevisGigEAcquistionMode eAMParam = EDeviceCrevisGigEAcquistionMode_Continuous;
					if(GetAcquisitionMode(&eAMParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AcquisitionMode"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
					strTemp = g_lpszCrevisGigEAcquistionMode[eAMParam];
					if(ST_SetEnumReg(m_hDevice, "AcquisitionMode", (LPSTR)(LPCSTR)(strTemp)))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("AcquisitionMode"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					strError = _T("AcquisitionFrameCount");
					nParam = 0;
					if(GetAcquisitionFrameCount(&nParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AcquisitionFrameCount"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
					if(ST_SetIntReg(m_hDevice, "AcquisitionFrameCount", nParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("AcquisitionFrameCount"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					strError = _T("TriggerSelector");
					EDeviceCrevisGigETriggerSelector eTSParam = EDeviceCrevisGigETriggerSelector_FrameStart;
					if(GetTriggerSelector(&eTSParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TriggerSelector"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
					strTemp = g_lpszCrevisGigETriggerSelector[eTSParam];
					if(ST_SetEnumReg(m_hDevice, "TriggerSelector", (LPSTR)(LPCSTR)(strTemp)))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TriggerSelector"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					EDeviceCrevisGigETriggerSource eCurrentTSrcParam = EDeviceCrevisGigETriggerSource_Line1;
					if(GetTriggerSource(&eCurrentTSrcParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TriggerSource"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					EDeviceCrevisGigETriggerSource eTSrcParam = EDeviceCrevisGigETriggerSource_Line1;
					EDeviceCrevisGigETriggerMode eTMParam = EDeviceCrevisGigETriggerMode_Off;
					EDeviceCrevisGigETriggerActivation eTA = EDeviceCrevisGigETriggerActivation_RisingEdge;
					float fParam = 0.f;

					eReturn = EDeviceInitializeResult_OK;
					for(int i = 0; i < (int)EDeviceCrevisGigETriggerSource_Count; ++i)
					{
						switch(eTSrcParam)
						{
						case EDeviceCrevisGigETriggerSource_Line1:
							{
								strError = _T("Line1TriggerSource");
								strTemp = g_lpszCrevisGigETriggerSource[eTSrcParam];
								if(ST_SetEnumReg(m_hDevice, "TriggerSource", (LPSTR)(LPCSTR)(strTemp)))
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TriggerSource"));
									eReturn = EDeviceInitializeResult_WriteToDeviceError;
									break;
								}

								strError = _T("Line1TriggerMode");
								if(GetLine1TriggerMode(&eTMParam))
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TriggerMode"));
									eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
									break;
								}
								strTemp = g_lpszCrevisGigETriggerMode[eTMParam];
								if(ST_SetEnumReg(m_hDevice, "TriggerMode", (LPSTR)(LPCSTR)(strTemp)))
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TriggerMode"));
									eReturn = EDeviceInitializeResult_WriteToDeviceError;
									break;
								}

								strError = _T("Line1TriggerActivation");
								if(GetLine1TriggerActivation(&eTA))
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TriggerActivation"));
									eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
									break;
								}
								strTemp = g_lpszCrevisGigETriggerActivation[eTA];
								if(ST_SetEnumReg(m_hDevice, "TriggerActivation", (LPSTR)(LPCSTR)(strTemp)))
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TriggerActivation"));
									eReturn = EDeviceInitializeResult_WriteToDeviceError;
									break;
								}

								strError = _T("Line1TriggerDelay");
								if(GetLine1TriggerDelay(&fParam))
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TriggerDelay"));
									eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
									break;
								}
								if(ST_SetFloatReg(m_hDevice, "TriggerDelay", fParam))
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TriggerDelay"));
									eReturn = EDeviceInitializeResult_WriteToDeviceError;
									break;
								}

								eTSrcParam = EDeviceCrevisGigETriggerSource_Software;
							}
							break;
						case EDeviceCrevisGigETriggerSource_Software:
							{
								strError = _T("SWTriggerSource");
								strTemp = g_lpszCrevisGigETriggerSource[eTSrcParam];
								if(ST_SetEnumReg(m_hDevice, "TriggerSource", (LPSTR)(LPCSTR)(strTemp)))
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TriggerSource"));
									eReturn = EDeviceInitializeResult_WriteToDeviceError;
									break;
								}

								strError = _T("SWTriggerMode");
								if(GetSoftwareTriggerMode(&eTMParam))
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TriggerMode"));
									eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
									break;
								}
								strTemp = g_lpszCrevisGigETriggerMode[eTMParam];
								if(ST_SetEnumReg(m_hDevice, "TriggerMode", (LPSTR)(LPCSTR)(strTemp)))
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TriggerMode"));
									eReturn = EDeviceInitializeResult_WriteToDeviceError;
									break;
								}

								strError = _T("SWTriggerActivation");
								if(GetSoftwareTriggerActivation(&eTA))
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TriggerActivation"));
									eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
									break;
								}
								strTemp = g_lpszCrevisGigETriggerActivation[eTA];
								if(ST_SetEnumReg(m_hDevice, "TriggerActivation", (LPSTR)(LPCSTR)(strTemp)))
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TriggerActivation"));
									eReturn = EDeviceInitializeResult_WriteToDeviceError;
									break;
								}

								strError = _T("SWTriggerDelay");
								if(GetSoftwareTriggerDelay(&fParam))
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TriggerDelay"));
									eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
									break;
								}
								if(ST_SetFloatReg(m_hDevice, "TriggerDelay", fParam))
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TriggerDelay"));
									eReturn = EDeviceInitializeResult_WriteToDeviceError;
									break;
								}

								eTSrcParam = eCurrentTSrcParam;
							}
						default://SetCurrentSource
							{
								strError = _T("CurrentTriggerSource");
								strTemp = g_lpszCrevisGigETriggerSource[eTSrcParam];
								if(ST_SetEnumReg(m_hDevice, "TriggerSource", (LPSTR)(LPCSTR)(strTemp)))
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TriggerSource"));
									eReturn = EDeviceInitializeResult_WriteToDeviceError;
									break;
								}
							}
							break;
						}

						if(eReturn != EDeviceInitializeResult_OK)
							break;
					}

					if(eReturn != EDeviceInitializeResult_OK)
						break;

					strError = _T("ExposureMode");
					EDeviceCrevisGigEExposureMode eEMParam = EDeviceCrevisGigEExposureMode_Timed;
					if(GetExposureMode(&eEMParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ExposureMode"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
					strTemp = g_lpszCrevisGigEExposureMode[eEMParam];
					if(ST_SetEnumReg(m_hDevice, "ExposureMode", (LPSTR)(LPCSTR)(strTemp)))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ExposureMode"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					strError = _T("ExposureAuto");
					EDeviceCrevisGigEExposureAuto eEAParam = EDeviceCrevisGigEExposureAuto_Off;
					if(GetExposureAuto(&eEAParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ExposureAuto"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
					strTemp = g_lpszCrevisGigEExposureAuto[eEAParam];
					if(ST_SetEnumReg(m_hDevice, "ExposureAuto", (LPSTR)(LPCSTR)(strTemp)))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ExposureAuto"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					strError = _T("ExposureTime");
					fParam = 0.f;
					if(GetExposureTime(&fParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ExposureTime"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
					int nError = (ST_SetFloatReg(m_hDevice, "ExposureTime", fParam));
					if(nError)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ExposureTime"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					strError = _T("AcquisitionFrameRateEnable");
					EDeviceCrevisGigEAcquistionFramerateEnable eAFEParam = EDeviceCrevisGigEAcquistionFramerateEnable_Off;
					if(GetAcquisitionFrameRateEnable(&eAFEParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AcquisitionFrameRateEnable"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
					strTemp = g_lpszCrevisGigEAcquistionFramerateEnable[eAFEParam];
					if(ST_SetEnumReg(m_hDevice, "AcquisitionFrameRateEnable", (LPSTR)(LPCSTR)(strTemp)))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("AcquisitionFrameRateEnable"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					strError = _T("AcquisitionFrameRate");
					fParam = 0.f;
					if(GetAcquisitionFrameRate(&fParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AcquisitionFrameRate"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
					if(ST_SetFloatReg(m_hDevice, "AcquisitionFrameRate", fParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("AcquisitionFrameRate"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					strError = _T("AutoExposureTarget");
					nParam = 0;
					if(GetAutoExposureTarget(&nParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AutoExposureTarget"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
					if(ST_SetIntReg(m_hDevice, "AutoExposureTarget", nParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("AutoExposureTarget"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					//Digital IO Control
					strError = _T("LineSelector");
					EDeviceCrevisGigELineSelector eLSParam = EDeviceCrevisGigELineSelector_Line1;
					if(GetLineSelector(&eLSParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineSelector"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
					strTemp = g_lpszCrevisGigELineSelector[eLSParam];
					if(ST_SetEnumReg(m_hDevice, "LineSelector", (LPSTR)(LPCSTR)(strTemp)))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineSelector"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					strError = _T("LineInverter");
					bParam = false;
					if(GetLineInverter(&bParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineInverter"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
					if(ST_SetBoolReg(m_hDevice, "LineInverter", (int)bParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineInverter"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					strError = _T("LineSource");
					EDeviceCrevisGigELineSource eLSrcParam = EDeviceCrevisGigELineSource_Off;
					if(GetLineSource(&eLSrcParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineSource"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
					if(eLSrcParam != EDeviceCrevisGigELineSource_Off)
					{
						strTemp = g_lpszCrevisGigELineSource[eLSrcParam];
						if(ST_SetEnumReg(m_hDevice, "LineSource", (LPSTR)(LPCSTR)(strTemp)))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineSource"));
							eReturn = EDeviceInitializeResult_WriteToDeviceError;
							break;
						}

						strError = _T("UserOutputSelector");
						EDeviceCrevisGigEUserOutputSelector eLUOSParam = EDeviceCrevisGigEUserOutputSelector_UserOutput1;
						if(GetUserOutputSelector(&eLUOSParam))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("UserOutputSelector"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(ST_SetEnumReg(m_hDevice, "UserOutputSelector", (LPSTR)(LPCSTR)(g_lpszCrevisGigEUserOutputSelector[eLUOSParam])))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("UserOutputSelector"));
							eReturn = EDeviceInitializeResult_WriteToDeviceError;
							break;
						}
					}

					strError = _T("TimerSelector");
					EDeviceCrevisGigETimerSelector eTimerSParam = EDeviceCrevisGigETimerSelector_Timer1;
					if(GetTimerSelector(&eTimerSParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TimerSelector"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
					strTemp = g_lpszCrevisGigETimerSelector[eTimerSParam];
					if(ST_SetEnumReg(m_hDevice, "TimerSelector", (LPSTR)(LPCSTR)(strTemp)))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TimerSelector"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					strError = _T("TimerDuration");
					fParam = 0.f;
					if(GetTimerDuration(&fParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TimerDuration"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
					if(ST_SetFloatReg(m_hDevice, "TimerDuration", fParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TimerDuration"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					strError = _T("TimerDelay");
					fParam = 0.f;
					if(GetTimerDelay(&fParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TimerDelay"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
					if(ST_SetFloatReg(m_hDevice, "TimerDelay", fParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TimerDelay"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					//LUT Control
					strError = _T("LUTEnable");
					bParam = false;
					if(GetLUTEnable(&bParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LUTEnable"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
					if(ST_SetBoolReg(m_hDevice, "LUTEnable", (int)bParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LUTEnable"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					strError = _T("LUTIndex");
					nParam = 0;
					if(GetLUTIndex(&nParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LUTIndex"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
					if(ST_SetIntReg(m_hDevice, "LUTIndex", nParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LUTIndex"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					strError = _T("LUTValue");
					nParam = 0;
					if(GetLUTValue(&nParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LUTValue"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
					if(ST_SetIntReg(m_hDevice, "LUTValue", nParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LUTValue"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					//Analog Control
					strError = _T("Gainselector");
					EDeviceCrevisGigEGainSelector eGSParam = EDeviceCrevisGigEGainSelector_All;
					if(GetGainselector(&eGSParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Gainselector"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
					strTemp = g_lpszCrevisGigEGainSelector[eGSParam];
					if(ST_SetEnumReg(m_hDevice, "GainSelector", (LPSTR)(LPCSTR)(strTemp)))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Gainselector"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					strError = _T("GainRaw");
					nParam = 0;
					if(GetGainRaw(&nParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("GainRaw"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
					if(ST_SetIntReg(m_hDevice, "GainRaw", nParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("GainRaw"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					strError = _T("GainAuto");
					EDeviceCrevisGigEGainAuto eGAParam = EDeviceCrevisGigEGainAuto_Off;
					if(GetGainAuto(&eGAParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("GainAuto"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
					strTemp = g_lpszCrevisGigEGainAuto[eGAParam];
					if(ST_SetEnumReg(m_hDevice, "GainAuto", (LPSTR)(LPCSTR)(strTemp)))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("GainAuto"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					strError = _T("BlackLevelSelector");
					EDeviceCrevisGigEBlackLevelSelector eBLSParam = EDeviceCrevisGigEBlackLevelSelector_All;
					if(GetBlackLevelSelector(&eBLSParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("BlackLevelSelector"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
					strTemp = g_lpszCrevisGigEBlackLevelSelector[eBLSParam];
					if(ST_SetEnumReg(m_hDevice, "BlackLevelSelector", (LPSTR)(LPCSTR)(strTemp)))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("BlackLevelSelector"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					strError = _T("BlackLevelRaw");
					nParam = 0;
					if(GetBlackLevelRaw(&nParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("BlackLevelRaw"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
					if(ST_SetIntReg(m_hDevice, "BlackLevelRaw", nParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("BlackLevelRaw"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					//color only
					if(m_bColorCamera)
					{
						strError = _T("BalanceRatioSelector");
						EDeviceCrevisGigEBalanceRatioSelector eBRSParam = EDeviceCrevisGigEBalanceRatioSelector_Red;
						if(GetBalanceRatioSelector(&eBRSParam))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("BalanceRatioSelector"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						strTemp = g_lpszCrevisGigEBalanceRatioSelector[eBRSParam];
						if(ST_SetEnumReg(m_hDevice, "BalanceRatioSelector", (LPSTR)(LPCSTR)(strTemp)))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("BalanceRatioSelector"));
							eReturn = EDeviceInitializeResult_WriteToDeviceError;
							break;
						}

						strError = _T("BalanceRatio");
						fParam = 0.f;
						if(GetBalanceRatio(&fParam))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("BalanceRatio"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(ST_SetFloatReg(m_hDevice, "BalanceRatio", fParam))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("BalanceRatio"));
							eReturn = EDeviceInitializeResult_WriteToDeviceError;
							break;
						}

						strError = _T("BalanceWhiteAuto");
						EDeviceCrevisGigEBalanceWhiteAuto eBWAParam = EDeviceCrevisGigEBalanceWhiteAuto_Off;
						if(GetBalanceWhiteAuto(&eBWAParam))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("BalanceWhiteAuto"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						strTemp = g_lpszCrevisGigEBalanceWhiteAuto[eBWAParam];
						if(ST_SetEnumReg(m_hDevice, "BalanceWhiteAuto", (LPSTR)(LPCSTR)(strTemp)))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("BalanceWhiteAuto"));
							eReturn = EDeviceInitializeResult_WriteToDeviceError;
							break;
						}
					}

					//Transfer control
					strError = _T("GevHeartbeatTimeout");
					nParam = 0;
					if(GetGevHeartbeatTimeout(&nParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("GevHeartbeatTimeout"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
					if(ST_SetIntReg(m_hDevice, "GevHeartbeatTimeout", nParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("GevHeartbeatTimeout"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					strError = _T("GevGVCPHeartbeatDisable");
					bParam = false;
					if(GetGevGVCPHearteatDisable(&bParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("GevGVCPHearteatDisable"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
					if(ST_SetBoolReg(m_hDevice, "GevGVCPHeartbeatDisable", bParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("GevGVCPHeartbeatDisable"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					strError = _T("GevSCPSPacketSize");
					nParam = 0;
					if(GetGevSCPSPacketSize(&nParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("GevSCPSPacketSize"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
					if(ST_SetIntReg(m_hDevice, "GevSCPSPacketSize", nParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("GevSCPSPacketSize"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					//User control
					strError = _T("UserSetSelector");
					EDeviceCrevisGigEUserSetSelector eUSSParam = EDeviceCrevisGigEUserSetSelector_Default;
					if(GetUserSetSelector(&eUSSParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("UserSetSelector"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
					strTemp = g_lpszCrevisGigEUserSetSelector[eUSSParam];
					if(ST_SetEnumReg(m_hDevice, "UserSetSelector", (LPSTR)(LPCSTR)(strTemp)))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("UserSetSelector"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					//Device Option Control
					strError = _T("DeviceFilterDriverMode");
					EDeviceCrevisGigEDeviceFilterDriverMode eDFDParam = EDeviceCrevisGigEDeviceFilterDriverMode_Off;
					if(GetDeviceFilterDriverMode(&eDFDParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("DeviceFilterDriverMode"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
					strTemp = g_lpszCrevisGigEDeviceFilterDriverMode[eDFDParam];
					if(ST_SetEnumReg(m_hDevice, "DeviceFilterDriverMode", (LPSTR)(LPCSTR)(strTemp)))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("DeviceFilterDriverMode"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					strError = _T("DeviceCommandTimeout");
					nParam = 0;
					if(GetDeviceCommandTimeout(&nParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("DeviceCommandTimeout"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
					if(ST_SetIntReg(m_hDevice, "DeviceCommandTimeout", nParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("DeviceCommandTimeout"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					strError = _T("DeviceCommandRetryNumber");
					nParam = 0;
					if(GetDeviceCommandRetryNumber(&nParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("DeviceCommandRetryNumber"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
					if(ST_SetIntReg(m_hDevice, "DeviceCommandRetryNumber", nParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("DeviceCommandRetryNumber"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					strError = _T("DeviceMissingPacketReceive");
					EDeviceCrevisGigEDeviceMissingPackedReceive eDMPRParam = EDeviceCrevisGigEDeviceMissingPackedReceive_Off;
					if(GetDeviceMissingPacketReceive(&eDMPRParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("DeviceMissingPacketReceive"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
					strTemp = g_lpszCrevisGigEDeviceMissingPackedReceive[eDMPRParam];
					if(ST_SetEnumReg(m_hDevice, "DeviceMissingPacketReceive", (LPSTR)(LPCSTR)(strTemp)))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("DeviceMissingPacketReceive"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					strError = _T("DevicePacketResend");
					bParam = false;
					if(GetDevicePacketResend(&bParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("DevicePacketResend"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
					if(ST_SetBoolReg(m_hDevice, "DevicePacketResend", bParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("DevicePacketResend"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					strError = _T("DeviceMaxPacketResendCount");
					nParam = 0;
					if(GetDeviceMaxPacketResendCount(&nParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("DeviceMaxPacketResendCount"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
					if(ST_SetIntReg(m_hDevice, "DeviceMaxPacketResendCount", nParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("DeviceMaxPacketResendCount"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					//Color Transformation Control
					if(m_bColorCamera)
					{
						strError = _T("ColorTransformationEnable");
						bParam = false;
						if(GetColorTransformationEnable(&bParam))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ColorTransformationEnable"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(ST_SetBoolReg(m_hDevice, "ColorTransformationEnable", bParam))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ColorTransformationEnable"));
							eReturn = EDeviceInitializeResult_WriteToDeviceError;
							break;
						}

						strError = _T("ColorTransformationValueSelector");
						EDeviceCrevisGigEColorTransformationValueSeletor eCTVSParam = EDeviceCrevisGigEColorTransformationValueSeletor_HUEBYGP;
						if(GetColorTransformationValueSelector(&eCTVSParam))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ColorTransformationValueSelector"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						strTemp = g_lpszCrevisGigEColorTransformationValueSeletor[eCTVSParam];
						if(ST_SetEnumReg(m_hDevice, "ColorTransformationValueSelector", (LPSTR)(LPCSTR)(strTemp)))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ColorTransformationValueSelector"));
							eReturn = EDeviceInitializeResult_WriteToDeviceError;
							break;
						}

						strError = _T("ColorTransformationValue");
						fParam = 0.f;
						if(GetColorTransformationValue(&fParam))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ColorTransformationValue"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(ST_SetFloatReg(m_hDevice, "ColorTransformationValue", fParam))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ColorTransformationValue"));
							eReturn = EDeviceInitializeResult_WriteToDeviceError;
							break;
						}
					}

					bParamOK = true;
				}
				catch(...)
				{
					char cBuffer[1024] = { 0, };
					ST_GetLastError(m_hDevice, cBuffer);

					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), strError);
				}
			}
			break;
		}

		if(!bParamOK)
			break;

		int nBpp = 8;

		int nAlignByte = 4;

		int nWidthStep = nWidth;

		bool bColor = strFormat.Left(4).CompareNoCase(_T("Mono"));

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
				nBpp = 8;
				nAlignByte = 1;
				nWidthStep = nWidth;
			}
		}
		else
		{
			nWidthStep *= 3;

			if(!m_bColorCamera)
			{
				nBpp = 8;
				nAlignByte = 1;
				nWidthStep = nWidth;
				bColor = false;
			}
		}

		int nChannel = bColor ? 3 : 1;

		int nMaxValue = (1 << nBpp) - 1;

		CMultipleVariable mv;
		for(int i = 0; i < nChannel; ++i)
			mv.AddValue(nMaxValue);

		this->InitBuffer(nWidth, nHeight, mv, CRavidImage::MakeValueFormat(nChannel, nBpp), nWidthStep, nAlignByte);
		this->ConnectImage();

		ST_SetCallbackFunction(m_hDevice, EVENT_NEW_IMAGE, CDeviceCrevisGigE::CallbackFunction, this);

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

	return eReturn;
}

EDeviceTerminateResult CDeviceCrevisGigE::Terminate()
{
	EDeviceTerminateResult eReturn = EDeviceTerminateResult_UnknownError;

	CString strMessage;

	SetStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("Basler"));

		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);

		return EDeviceTerminateResult_NotFoundApiError;
	}

	do
	{
		m_bIsLive = false;
		m_bIsInitialized = false;
		m_bIsGrabAvailable = true;

		ST_CloseDevice(m_hDevice);

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

bool CDeviceCrevisGigE::LoadSettings()
{
	bool bReturn = false;

	m_vctParameterFieldConfigurations.clear();

	do
	{
		CString strTime;
		strTime.Format(_T("%d"), INT_MAX);

		AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_DeviceID, g_lpszParamCrevisGigE[EDeviceParameterBaslerGigE_DeviceID], _T("0"), EParameterFieldType_Edit, nullptr, nullptr);

		AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_GeneralParameter, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_GeneralParameter], _T("0"), EParameterFieldType_None, nullptr, nullptr);
		{
			AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_InitializeMode, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_InitializeMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszCrevisGigEInitMode, EDeviceCrevisGigEInitMode_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_AcqInvalidTime, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_AcqInvalidTime], _T("0"), EParameterFieldType_Edit, nullptr, _T("integer"), 1);
			AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_GrabTimeout, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_GrabTimeout], _T("0"), EParameterFieldType_Edit, nullptr, _T("integer"), 1);
			AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_DetailedLog, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_DetailedLog], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszCrevisGigESwitch, EDeviceCrevisGigESwitch_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_GevHeartbeatTimeout, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_GevHeartbeatTimeout], _T("3000"), EParameterFieldType_Edit, nullptr, _T("integer"), 1);
			AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_GevGVCPHeartbeatDisable, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_GevGVCPHeartbeatDisable], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszCrevisGigESwitch, EDeviceCrevisGigESwitch_Count), _T("Disable Heartbeat. If camera is disconnected when the Parameter is on, you need to reconnect the camera manually"), 1);
			AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_GevSCPSPacketSize, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_GevSCPSPacketSize], _T("1436"), EParameterFieldType_Edit, nullptr, _T("integer"), 1);

		}

		AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_ImageControl, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_ImageControl], _T("0"), EParameterFieldType_None, nullptr, nullptr);
		{
			AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_Width, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_Width], _T("5496"), EParameterFieldType_Edit, nullptr, _T("integer"), 1);
			AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_Height, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_Height], _T("3672"), EParameterFieldType_Edit, nullptr, _T("integer"), 1);
			AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_PixelFormat, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_PixelFormat], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszCrevisGigEPixelFormat, EDeviceCrevisGigEPixelFormat_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_OffsetX, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_OffsetX], _T("0"), EParameterFieldType_Edit, nullptr, _T("integer"), 1);
			AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_OffsetY, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_OffsetY], _T("0"), EParameterFieldType_Edit, nullptr, _T("integer"), 1);
			AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_TestPattern, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_TestPattern], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszCrevisGigETestPattern, EDeviceCrevisGigETestPattern_Count), nullptr, 1);
		}

		AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_AcquisitionControl, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_AcquisitionControl], _T("0"), EParameterFieldType_None, nullptr, nullptr);
		{
			AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_AcquisitionMode, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_AcquisitionMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszCrevisGigEAcquistionMode, EDeviceCrevisGigEAcquistionMode_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_AcquisitionFrameCount, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_AcquisitionFrameCount], _T("0"), EParameterFieldType_Edit, nullptr, _T("integer"), 1);
			AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_TriggerSelector, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_TriggerSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszCrevisGigETriggerSelector, EDeviceCrevisGigETriggerSelector_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_TriggerSource, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_TriggerSource], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszCrevisGigETriggerSource, EDeviceCrevisGigETriggerSource_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_Line1, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_Line1], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);
			{
				AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_Line1TriggerMode, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_Line1TriggerMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszCrevisGigETriggerMode, EDeviceCrevisGigETriggerMode_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_Line1TriggerActivation, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_Line1TriggerActivation], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszCrevisGigETriggerActivation, EDeviceCrevisGigETriggerActivation_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_Line1TriggerDelay, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_Line1TriggerDelay], _T("0"), EParameterFieldType_Edit, nullptr, _T("float"), 2);
			}

			AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_Software, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_Software], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);
			{
				AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_SoftwareTriggerMode, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_SoftwareTriggerMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszCrevisGigETriggerMode, EDeviceCrevisGigETriggerMode_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_SoftwareTriggerActivation, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_SoftwareTriggerActivation], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszCrevisGigETriggerActivation, EDeviceCrevisGigETriggerActivation_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_SoftwareTriggerDelay, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_SoftwareTriggerDelay], _T("0"), EParameterFieldType_Edit, nullptr, _T("float"), 2);
			}

			AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_ExposureMode, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_ExposureMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszCrevisGigEExposureMode, EDeviceCrevisGigEExposureMode_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_ExposureAuto, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_ExposureAuto], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszCrevisGigEExposureAuto, EDeviceCrevisGigEExposureAuto_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_ExposureTime, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_ExposureTime], _T("166260.000000"), EParameterFieldType_Edit, nullptr, _T("float"), 1);
			AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_AcquisitionFrameRateEnable, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_AcquisitionFrameRateEnable], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszCrevisGigEAcquistionFramerateEnable, EDeviceCrevisGigEAcquistionFramerateEnable_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_AcquisitionFrameRate, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_AcquisitionFrameRate], _T("5.278654"), EParameterFieldType_Edit, nullptr, _T("float"), 1);
			AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_AutoExposureTarget, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_AutoExposureTarget], _T("128"), EParameterFieldType_Edit, nullptr, _T("integer"), 1);

			AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_DigitalIOControl, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_DigitalIOControl], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);
			{
				AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_LineSelector, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_LineSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszCrevisGigELineSelector, EDeviceCrevisGigELineSelector_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_LineInverter, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_LineInverter], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszCrevisGigESwitch, EDeviceCrevisGigESwitch_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_LineSource, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_LineSource], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszCrevisGigELineSource, EDeviceCrevisGigELineSource_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_UserOutputSelector, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_UserOutputSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszCrevisGigEUserOutputSelector, EDeviceCrevisGigEUserOutputSelector_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_TimerSelector, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_TimerSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszCrevisGigETimerSelector, EDeviceCrevisGigETimerSelector_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_TimerDuration, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_TimerDuration], _T("0"), EParameterFieldType_Edit, nullptr, _T("float"), 2);
				AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_TimerDelay, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_TimerDelay], _T("0"), EParameterFieldType_Edit, nullptr, _T("float"), 2);
			}

			AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_LUTControl, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_LUTControl], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);
			{
				AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_LUTEnable, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_LUTEnable], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszCrevisGigESwitch, EDeviceCrevisGigESwitch_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_LUTIndex, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_LUTIndex], _T("0"), EParameterFieldType_Edit, nullptr, _T("integer"), 2);
				AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_LUTValue, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_LUTValue], _T("0"), EParameterFieldType_Edit, nullptr, _T("integer"), 2);
			}

			AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_AnalogControl, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_AnalogControl], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);
			{
				AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_Gainselector, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_Gainselector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszCrevisGigEGainSelector, EDeviceCrevisGigEGainSelector_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_GainRaw, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_GainRaw], _T("0"), EParameterFieldType_Edit, nullptr, _T("integer"), 2);
				AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_GainAuto, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_GainAuto], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszCrevisGigEGainAuto, EDeviceCrevisGigEGainAuto_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_BlackLevelSelector, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_BlackLevelSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszCrevisGigEBlackLevelSelector, EDeviceCrevisGigEBlackLevelSelector_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_BlackLevelRaw, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_BlackLevelRaw], _T("0"), EParameterFieldType_Edit, nullptr, _T("integer"), 2);
				AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_BalanceRatioSelector, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_BalanceRatioSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszCrevisGigEBalanceRatioSelector, EDeviceCrevisGigEBalanceRatioSelector_Count), nullptr, 2);

				AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_ColorOnly, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_ColorOnly], _T("0"), EParameterFieldType_None, nullptr, nullptr, 2);
				{
					AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_BalanceRatio, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_BalanceRatio], _T("0.1"), EParameterFieldType_Edit, nullptr, _T("float"), 3);
					AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_BalanceWhiteAuto, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_BalanceWhiteAuto], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszCrevisGigEBalanceWhiteAuto, EDeviceCrevisGigEBalanceWhiteAuto_Count), nullptr, 3);
				}
			}

			AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_UserControl, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_UserControl], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);
			{
				AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_UserSetSelector, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_UserSetSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszCrevisGigEUserSetSelector, EDeviceCrevisGigEUserSetSelector_Count), nullptr, 2);
			}

			AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_DeviceOptionControl, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_DeviceOptionControl], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);
			{
				AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_DeviceFilterDriverMode, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_DeviceFilterDriverMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszCrevisGigEDeviceFilterDriverMode, EDeviceCrevisGigEDeviceFilterDriverMode_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_DeviceCommandTimeout, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_DeviceCommandTimeout], _T("100"), EParameterFieldType_Edit, nullptr, _T("integer"), 2);
				AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_DeviceCommandRetryNumber, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_DeviceCommandRetryNumber], _T("1"), EParameterFieldType_Edit, nullptr, _T("integer"), 2);
				AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_DeviceMissingPacketReceive, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_DeviceMissingPacketReceive], _T("100"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszCrevisGigEDeviceMissingPackedReceive, EDeviceCrevisGigEDeviceMissingPackedReceive_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_DevicePacketResend, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_DevicePacketResend], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszCrevisGigESwitch, EDeviceCrevisGigESwitch_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_DeviceMaxPacketResendCount, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_DeviceMaxPacketResendCount], _T("255"), EParameterFieldType_Edit, nullptr, _T("integer"), 2);
			}

			AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_ColorTransControl, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_ColorTransControl], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);
			{
				AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_ColorTransformationEnable, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_ColorTransformationEnable], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszCrevisGigESwitch, EDeviceCrevisGigESwitch_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_ColorTransformationValueSelector, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_ColorTransformationValueSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszCrevisGigEColorTransformationValueSeletor, EDeviceCrevisGigEColorTransformationValueSeletor_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterCrevisGigE_ColorTransformationValue, g_lpszParamCrevisGigE[EDeviceParameterCrevisGigE_ColorTransformationValue], _T("0"), EParameterFieldType_Edit, nullptr, _T("float"), 2);
			}
		}

		bReturn = true;
	}
	while(false);

	return bReturn & __super::LoadSettings();
}

EDeviceGrabResult CDeviceCrevisGigE::Grab()
{
	EDeviceGrabResult eReturn = EDeviceGrabResult_UnknownError;

	CString strMessage;

	do
	{
		m_nGrabCount = 0;

		int nRequestGrabCount = 0;

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

		eReturn = EDeviceGrabResult_UnknownError;
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntsupportthe_s), _T("Grab"));
		break;

		try
		{
			m_bIsGrabAvailable = false;

			if(ST_AcqStart(m_hDevice))
				break;
		}
		catch(...)
		{
			break;
		}

		m_nGrabCount = nRequestGrabCount;

		m_bIsGrabAvailable = false;

		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("Grab"));

		eReturn = EDeviceGrabResult_OK;
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceLiveResult CDeviceCrevisGigE::Live()
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

		try
		{
			if(ST_SetContinuousGrabbing(m_hDevice, 1))
				break;

			if(ST_AcqStart(m_hDevice))
				break;
		}
		catch(...)
		{
			break;
		}

		m_bIsLive = true;

		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("Live"));

		eReturn = EDeviceLiveResult_OK;
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceStopResult CDeviceCrevisGigE::Stop()
{
	EDeviceStopResult eReturn = EDeviceStopResult_UnknownError;

	CString strMessage;

	do
	{
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

		try
		{
			if(ST_AcqStop(m_hDevice))
				break;
		}
		catch (...)
		{
		}

		m_bIsLive = false;
		m_bIsGrabAvailable = true;

		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("Stop"));

		eReturn = EDeviceStopResult_OK;
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceTriggerResult CDeviceCrevisGigE::Trigger()
{
	EDeviceTriggerResult eReturn = EDeviceTriggerResult_UnknownError;

	CString strMessage;

	do
	{
		if(!IsInitialized())
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Ithasnotbeeninitializedyet);
			eReturn = EDeviceTriggerResult_NotInitializedError;
			break;
		}

		try
		{
			if(ST_SetCmdReg(m_hDevice, MCAM_TRIGGER_SOFTWARE))
				break;
		}
		catch (...)
		{
			
		}

		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("Trigger"));

		eReturn = EDeviceTriggerResult_OK;
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetInitializeMode(EDeviceCrevisGigEInitMode *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceCrevisGigEInitMode)_ttoi(GetParamValue(EDeviceParameterCrevisGigE_InitializeMode));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetInitializeMode(EDeviceCrevisGigEInitMode eParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_InitializeMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = ECrevisGigESetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], g_lpszCrevisGigEInitMode[nPreValue], g_lpszCrevisGigEInitMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetAcqInvalidTime(int *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterCrevisGigE_AcqInvalidTime));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetAcqInvalidTime(int nParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_AcqInvalidTime;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(!IsGrabAvailable())
		{
			eReturn = ECrevisGigESetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = ECrevisGigESetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetGrabTimeout(int *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterCrevisGigE_GrabTimeout));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetGrabTimeout(int nParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_GrabTimeout;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(!IsGrabAvailable())
		{
			eReturn = ECrevisGigESetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			try
			{
				if(ST_SetGrabTimeout(m_hDevice, nParam))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...) 
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;				
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetDetailedLog(bool *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterCrevisGigE_DetailedLog));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetDetailedLog(bool bParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_DetailedLog;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(!IsGrabAvailable())
		{
			eReturn = ECrevisGigESetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			try
			{
				if(ST_SetDetailedLog(m_hDevice, bParam))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strOffset;
		strOffset.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strOffset))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], g_lpszCrevisGigESwitch[nPreValue], g_lpszCrevisGigESwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetGevHeartbeatTimeout(int *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterCrevisGigE_GevHeartbeatTimeout));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetGevHeartbeatTimeout(int nParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_GevHeartbeatTimeout;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(!IsGrabAvailable())
		{
			eReturn = ECrevisGigESetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			try
			{
				if(ST_SetIntReg(m_hDevice, GEV_HEARTBEAT_TIMEOUT, nParam))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetGevGVCPHearteatDisable(bool *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterCrevisGigE_GevGVCPHeartbeatDisable));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetGevGVCPHearteatDisable(bool bParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_GevGVCPHeartbeatDisable;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			try
			{
				if(ST_SetBoolReg(m_hDevice, GEV_GVCP_HEARTBEAT_DISABLE, bParam))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strOffset;
		strOffset.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strOffset))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], g_lpszCrevisGigESwitch[nPreValue], g_lpszCrevisGigESwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetGevSCPSPacketSize(int *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterCrevisGigE_GevSCPSPacketSize));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetGevSCPSPacketSize(int nParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_GevSCPSPacketSize;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(!IsGrabAvailable())
		{
			eReturn = ECrevisGigESetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			try
			{
				if(ST_SetIntReg(m_hDevice, GEV_SCPS_PACKETSIZE, nParam))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetWidth(int *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterCrevisGigE_Width));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetWidth(int nParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_Width;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = ECrevisGigESetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetHeight(int *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterCrevisGigE_Height));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetHeight(int nParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_Height;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = ECrevisGigESetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetPixelFormat(EDeviceCrevisGigEPixelFormat *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceCrevisGigEPixelFormat)_ttoi(GetParamValue(EDeviceParameterCrevisGigE_PixelFormat));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetPixelFormat(EDeviceCrevisGigEPixelFormat eParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_PixelFormat;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = ECrevisGigESetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], g_lpszCrevisGigEPixelFormat[nPreValue], g_lpszCrevisGigEPixelFormat[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetOffsetX(int *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterCrevisGigE_OffsetX));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetOffsetX(int nParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_OffsetX;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			try
			{
				if(ST_SetIntReg(m_hDevice, MCAM_OFFSET_X, nParam))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetOffsetY(int *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterCrevisGigE_OffsetY));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetOffsetY(int nParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_OffsetY;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			try
			{
				if(ST_SetIntReg(m_hDevice, MCAM_OFFSET_Y, nParam))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetTestPattern(EDeviceCrevisGigETestPattern *pParam)
{

	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceCrevisGigETestPattern)_ttoi(GetParamValue(EDeviceParameterCrevisGigE_TestPattern));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetTestPattern(EDeviceCrevisGigETestPattern eParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_TestPattern;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			try
			{
				CStringA strTemp(g_lpszCrevisGigETestPattern[eParam]);
				if(ST_SetEnumReg(m_hDevice, MCAM_TEST_PATTERN, (LPSTR)(LPCSTR)(strTemp)))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...) 
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], g_lpszCrevisGigETestPattern[nPreValue], g_lpszCrevisGigETestPattern[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetAcquisitionMode(EDeviceCrevisGigEAcquistionMode *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceCrevisGigEAcquistionMode)_ttoi(GetParamValue(EDeviceParameterCrevisGigE_AcquisitionMode));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetAcquisitionMode(EDeviceCrevisGigEAcquistionMode eParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_AcquisitionMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			try
			{
				CStringA strTemp(g_lpszCrevisGigEAcquistionMode[eParam]);
				if(ST_SetEnumReg(m_hDevice, MCAM_ACQUISITION_MODE, (LPSTR)(LPCSTR)(strTemp)))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], g_lpszCrevisGigEAcquistionMode[nPreValue], g_lpszCrevisGigEAcquistionMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetAcquisitionFrameCount(int *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterCrevisGigE_AcquisitionFrameCount));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetAcquisitionFrameCount(int nParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_AcquisitionFrameCount;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			try
			{
				if(ST_SetIntReg(m_hDevice, MCAM_ACQUISITION_FRAME_COUNT, nParam))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetTriggerSelector(EDeviceCrevisGigETriggerSelector *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceCrevisGigETriggerSelector)_ttoi(GetParamValue(EDeviceParameterCrevisGigE_TriggerSelector));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetTriggerSelector(EDeviceCrevisGigETriggerSelector eParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_TriggerSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			try
			{
				CStringA strTemp(g_lpszCrevisGigETriggerSelector[eParam]);
				if(ST_SetEnumReg(m_hDevice, MCAM_TRIGGER_SELECTOR, (LPSTR)(LPCSTR)(strTemp)))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], g_lpszCrevisGigETriggerSelector[nPreValue], g_lpszCrevisGigETriggerSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetTriggerSource(EDeviceCrevisGigETriggerSource *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceCrevisGigETriggerSource)_ttoi(GetParamValue(EDeviceParameterCrevisGigE_TriggerSource));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetTriggerSource(EDeviceCrevisGigETriggerSource eParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_TriggerSource;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			try
			{
				CStringA strTemp(g_lpszCrevisGigETriggerSource[eParam]);
				if(ST_SetEnumReg(m_hDevice, MCAM_TRIGGER_SOURCE, (LPSTR)(LPCSTR)(strTemp)))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], g_lpszCrevisGigETriggerSource[nPreValue], g_lpszCrevisGigETriggerSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetLine1TriggerMode(EDeviceCrevisGigETriggerMode *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceCrevisGigETriggerMode)_ttoi(GetParamValue(EDeviceParameterCrevisGigE_Line1TriggerMode));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetLine1TriggerMode(EDeviceCrevisGigETriggerMode eParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_Line1TriggerMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			EDeviceCrevisGigETriggerSource eTriggerSource = EDeviceCrevisGigETriggerSource_Count;
			
			try
			{
				unsigned int unParamSize = 256;
				char arrParam[256] = { 0, };
				memset(arrParam, 0, sizeof(char) * unParamSize);
				if(!ST_GetEnumReg(m_hDevice, MCAM_TRIGGER_SOURCE, arrParam, &unParamSize))
				{
					CString str;
					str = arrParam;

					for(int i = 0; i < EDeviceCrevisGigETriggerSource_Count; ++i)
					{
						if(str.CompareNoCase(g_lpszCrevisGigETriggerSource[i]))
							continue;
						eTriggerSource = (EDeviceCrevisGigETriggerSource)i;
						break;
					}
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}

			if(eTriggerSource != EDeviceCrevisGigETriggerSource_Line1)
			{
				eReturn = ECrevisGigESetFunction_NotFindCommandError;
				break;
			}

			try
			{
				CStringA strTemp(g_lpszCrevisGigETriggerMode[eParam]);
				if(ST_SetEnumReg(m_hDevice, MCAM_TRIGGER_MODE, (LPSTR)(LPCSTR)(strTemp)))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], g_lpszCrevisGigETriggerMode[nPreValue], g_lpszCrevisGigETriggerMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetLine1TriggerActivation(EDeviceCrevisGigETriggerActivation *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceCrevisGigETriggerActivation)_ttoi(GetParamValue(EDeviceParameterCrevisGigE_Line1TriggerActivation));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetLine1TriggerActivation(EDeviceCrevisGigETriggerActivation eParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_Line1TriggerActivation;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			EDeviceCrevisGigETriggerSource eTriggerSource = EDeviceCrevisGigETriggerSource_Count;

			try
			{
				unsigned int unParamSize = 256;
				char arrParam[256] = { 0, };
				memset(arrParam, 0, sizeof(char) * unParamSize);
				if(!ST_GetEnumReg(m_hDevice, MCAM_TRIGGER_SOURCE, arrParam, &unParamSize))
				{
					CString str;
					str = arrParam;

					for(int i = 0; i < EDeviceCrevisGigETriggerSource_Count; ++i)
					{
						if(str.CompareNoCase(g_lpszCrevisGigETriggerSource[i]))
							continue;
						eTriggerSource = (EDeviceCrevisGigETriggerSource)i;
						break;
					}
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}

			if(eTriggerSource != EDeviceCrevisGigETriggerSource_Line1)
			{
				eReturn = ECrevisGigESetFunction_NotFindCommandError;
				break;
			}

			try
			{
				CStringA strTemp(g_lpszCrevisGigETriggerActivation[eParam]);
				if(ST_SetEnumReg(m_hDevice, MCAM_TRIGGER_ACTIVATION, (LPSTR)(LPCSTR)(strTemp)))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], g_lpszCrevisGigETriggerActivation[nPreValue], g_lpszCrevisGigETriggerActivation[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetLine1TriggerDelay(float *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterCrevisGigE_Line1TriggerDelay));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetLine1TriggerDelay(float fParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_Line1TriggerDelay;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			EDeviceCrevisGigETriggerSource eTriggerSource = EDeviceCrevisGigETriggerSource_Count;

			try
			{
				unsigned int unParamSize = 256;
				char arrParam[256] = { 0, };
				memset(arrParam, 0, sizeof(char) * unParamSize);
				if(!ST_GetEnumReg(m_hDevice, MCAM_TRIGGER_SOURCE, arrParam, &unParamSize))
				{
					CString str;
					str = arrParam;

					for(int i = 0; i < EDeviceCrevisGigETriggerSource_Count; ++i)
					{
						if(str.CompareNoCase(g_lpszCrevisGigETriggerSource[i]))
							continue;
						eTriggerSource = (EDeviceCrevisGigETriggerSource)i;
						break;
					}
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}

			if(eTriggerSource != EDeviceCrevisGigETriggerSource_Line1)
			{
				eReturn = ECrevisGigESetFunction_NotFindCommandError;
				break;
			}

			try
			{
				if(ST_SetFloatReg(m_hDevice, MCAM_TRIGGER_DELAY, fParam))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetSoftwareTriggerMode(EDeviceCrevisGigETriggerMode *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceCrevisGigETriggerMode)_ttoi(GetParamValue(EDeviceParameterCrevisGigE_SoftwareTriggerMode));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetSoftwareTriggerMode(EDeviceCrevisGigETriggerMode eParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_SoftwareTriggerMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			EDeviceCrevisGigETriggerSource eTriggerSource = EDeviceCrevisGigETriggerSource_Count;

			try
			{
				unsigned int unParamSize = 256;
				char arrParam[256] = { 0, };
				memset(arrParam, 0, sizeof(char) * unParamSize);
				if(!ST_GetEnumReg(m_hDevice, MCAM_TRIGGER_SOURCE, arrParam, &unParamSize))
				{
					CString str;
					str = arrParam;

					for(int i = 0; i < EDeviceCrevisGigETriggerSource_Count; ++i)
					{
						if(str.CompareNoCase(g_lpszCrevisGigETriggerSource[i]))
							continue;
						eTriggerSource = (EDeviceCrevisGigETriggerSource)i;
						break;
					}
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}

			if(eTriggerSource != EDeviceCrevisGigETriggerSource_Software)
			{
				eReturn = ECrevisGigESetFunction_NotFindCommandError;
				break;
			}

			try
			{
				CStringA strTemp(g_lpszCrevisGigETriggerMode[eParam]);
				if(ST_SetEnumReg(m_hDevice, MCAM_TRIGGER_MODE, (LPSTR)(LPCSTR)(strTemp)))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], g_lpszCrevisGigETriggerMode[nPreValue], g_lpszCrevisGigETriggerMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetSoftwareTriggerActivation(EDeviceCrevisGigETriggerActivation *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceCrevisGigETriggerActivation)_ttoi(GetParamValue(EDeviceParameterCrevisGigE_SoftwareTriggerActivation));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetSoftwareTriggerActivation(EDeviceCrevisGigETriggerActivation eParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_SoftwareTriggerActivation;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			EDeviceCrevisGigETriggerSource eTriggerSource = EDeviceCrevisGigETriggerSource_Count;

			try
			{
				unsigned int unParamSize = 256;
				char arrParam[256] = { 0, };
				memset(arrParam, 0, sizeof(char) * unParamSize);
				if(!ST_GetEnumReg(m_hDevice, MCAM_TRIGGER_SOURCE, arrParam, &unParamSize))
				{
					CString str;
					str = arrParam;

					for(int i = 0; i < EDeviceCrevisGigETriggerSource_Count; ++i)
					{
						if(str.CompareNoCase(g_lpszCrevisGigETriggerSource[i]))
							continue;
						eTriggerSource = (EDeviceCrevisGigETriggerSource)i;
						break;
					}
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}

			if(eTriggerSource != EDeviceCrevisGigETriggerSource_Software)
			{
				eReturn = ECrevisGigESetFunction_NotFindCommandError;
				break;
			}

			try
			{
				CStringA strTemp(g_lpszCrevisGigETriggerActivation[eParam]);
				if(ST_SetEnumReg(m_hDevice, MCAM_TRIGGER_ACTIVATION, (LPSTR)(LPCSTR)(strTemp)))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], g_lpszCrevisGigETriggerActivation[nPreValue], g_lpszCrevisGigETriggerActivation[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetSoftwareTriggerDelay(float *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterCrevisGigE_SoftwareTriggerDelay));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetSoftwareTriggerDelay(float fParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_SoftwareTriggerDelay;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			EDeviceCrevisGigETriggerSource eTriggerSource = EDeviceCrevisGigETriggerSource_Count;

			try
			{
				unsigned int unParamSize = 256;
				char arrParam[256] = { 0, };
				memset(arrParam, 0, sizeof(char) * unParamSize);
				if(!ST_GetEnumReg(m_hDevice, MCAM_TRIGGER_SOURCE, arrParam, &unParamSize))
				{
					CString str;
					str = arrParam;

					for(int i = 0; i < EDeviceCrevisGigETriggerSource_Count; ++i)
					{
						if(str.CompareNoCase(g_lpszCrevisGigETriggerSource[i]))
							continue;
						eTriggerSource = (EDeviceCrevisGigETriggerSource)i;
						break;
					}
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}

			if(eTriggerSource != EDeviceCrevisGigETriggerSource_Software)
			{
				eReturn = ECrevisGigESetFunction_NotFindCommandError;
				break;
			}

			try
			{
				if(ST_SetFloatReg(m_hDevice, MCAM_TRIGGER_DELAY, fParam))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetExposureMode(EDeviceCrevisGigEExposureMode *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceCrevisGigEExposureMode)_ttoi(GetParamValue(EDeviceParameterCrevisGigE_ExposureMode));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetExposureMode(EDeviceCrevisGigEExposureMode eParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_ExposureMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			try
			{
				CStringA strTemp(g_lpszCrevisGigEExposureMode[eParam]);
				if(ST_SetEnumReg(m_hDevice, MCAM_EXPOSURE_MODE, (LPSTR)(LPCSTR)(strTemp)))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], g_lpszCrevisGigEExposureMode[nPreValue], g_lpszCrevisGigEExposureMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetExposureAuto(EDeviceCrevisGigEExposureAuto *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceCrevisGigEExposureAuto)_ttoi(GetParamValue(EDeviceParameterCrevisGigE_ExposureAuto));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetExposureAuto(EDeviceCrevisGigEExposureAuto eParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_ExposureAuto;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			try
			{
				CStringA strTemp(g_lpszCrevisGigEExposureAuto[eParam]);
				if(ST_SetEnumReg(m_hDevice, MCAM_EXPOSURE_AUTO, (LPSTR)(LPCSTR)(strTemp)))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], g_lpszCrevisGigEExposureAuto[nPreValue], g_lpszCrevisGigEExposureAuto[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetExposureTime(float *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterCrevisGigE_ExposureTime));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetExposureTime(float fParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_ExposureTime;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			try
			{
				if(ST_SetFloatReg(m_hDevice, MCAM_EXPOSURE_TIME, fParam))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetAcquisitionFrameRateEnable(EDeviceCrevisGigEAcquistionFramerateEnable *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceCrevisGigEAcquistionFramerateEnable)_ttoi(GetParamValue(EDeviceParameterCrevisGigE_AcquisitionFrameRateEnable));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetAcquisitionFrameRateEnable(EDeviceCrevisGigEAcquistionFramerateEnable eParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_AcquisitionFrameRateEnable;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			try
			{
				CStringA strTemp(g_lpszCrevisGigEAcquistionFramerateEnable[eParam]);
				if(ST_SetEnumReg(m_hDevice, MCAM_ACQUISITION_FRAMERATE_ENABLE, (LPSTR)(LPCSTR)(strTemp)))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], g_lpszCrevisGigEAcquistionFramerateEnable[nPreValue], g_lpszCrevisGigEAcquistionFramerateEnable[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetAcquisitionFrameRate(float *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterCrevisGigE_AcquisitionFrameRate));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetAcquisitionFrameRate(float fParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_AcquisitionFrameRate;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			try
			{
				if(ST_SetFloatReg(m_hDevice, MCAM_ACQUISITIONF_RAMERATE, fParam))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetAutoExposureTarget(int *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterCrevisGigE_AutoExposureTarget));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetAutoExposureTarget(int nParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_AutoExposureTarget;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			try
			{
				if(ST_SetIntReg(m_hDevice, MCAM_AUTO_EXPOSURE_TARGET, nParam))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetLineSelector(EDeviceCrevisGigELineSelector *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceCrevisGigELineSelector)_ttoi(GetParamValue(EDeviceParameterCrevisGigE_LineSelector));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetLineSelector(EDeviceCrevisGigELineSelector eParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_LineSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			try
			{
				CStringA strTemp(g_lpszCrevisGigELineSelector[eParam]);
				if(ST_SetEnumReg(m_hDevice, MCAM_LINE_SELECTOR, (LPSTR)(LPCSTR)(strTemp)))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], g_lpszCrevisGigELineSelector[nPreValue], g_lpszCrevisGigELineSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetLineInverter(bool *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterCrevisGigE_LineInverter));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetLineInverter(bool bParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_LineInverter;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			try
			{
				if(ST_SetBoolReg(m_hDevice, MCAM_LINE_INVERTER, bParam))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strOffset;
		strOffset.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strOffset))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], g_lpszCrevisGigESwitch[nPreValue], g_lpszCrevisGigESwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetLineSource(EDeviceCrevisGigELineSource *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceCrevisGigELineSource)_ttoi(GetParamValue(EDeviceParameterCrevisGigE_LineSource));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetLineSource(EDeviceCrevisGigELineSource eParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_LineSource;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			try
			{
				CStringA strTemp(g_lpszCrevisGigELineSource[eParam]);
				if(ST_SetEnumReg(m_hDevice, MCAM_LINE_SOURCE, (LPSTR)(LPCSTR)(strTemp)))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], g_lpszCrevisGigELineSource[nPreValue], g_lpszCrevisGigELineSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetUserOutputSelector(EDeviceCrevisGigEUserOutputSelector *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceCrevisGigEUserOutputSelector)_ttoi(GetParamValue(EDeviceParameterCrevisGigE_UserOutputSelector));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetUserOutputSelector(EDeviceCrevisGigEUserOutputSelector eParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_UserOutputSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			try
			{
				CStringA strTemp(g_lpszCrevisGigEUserOutputSelector[eParam]);
				if(ST_SetEnumReg(m_hDevice, MCAM_USER_OUTPUT_SELECTOR, (LPSTR)(LPCSTR)(strTemp)))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], g_lpszCrevisGigEUserOutputSelector[nPreValue], g_lpszCrevisGigEUserOutputSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetTimerSelector(EDeviceCrevisGigETimerSelector *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceCrevisGigETimerSelector)_ttoi(GetParamValue(EDeviceParameterCrevisGigE_TimerSelector));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetTimerSelector(EDeviceCrevisGigETimerSelector eParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_TimerSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			try
			{
				CStringA strTemp(g_lpszCrevisGigETimerSelector[eParam]);
				if(ST_SetEnumReg(m_hDevice, MCAM_TIMER_SELECTOR, (LPSTR)(LPCSTR)(strTemp)))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], g_lpszCrevisGigETimerSelector[nPreValue], g_lpszCrevisGigETimerSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetTimerDuration(float *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterCrevisGigE_TimerDuration));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetTimerDuration(float fParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_TimerDuration;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			try
			{
				if(ST_SetFloatReg(m_hDevice, MCAM_TIMER_DURATION, fParam))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetTimerDelay(float *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterCrevisGigE_TimerDelay));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetTimerDelay(float fParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_TimerDelay;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			try
			{
				if(ST_SetFloatReg(m_hDevice, MCAM_TIMER_DELAY, fParam))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetLUTEnable(bool *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterCrevisGigE_LUTEnable));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetLUTEnable(bool bParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_LUTEnable;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			try
			{
				if(ST_SetBoolReg(m_hDevice, MCAM_LUT_ENABLE, bParam))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strOffset;
		strOffset.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strOffset))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], g_lpszCrevisGigESwitch[nPreValue], g_lpszCrevisGigESwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetLUTIndex(int *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterCrevisGigE_LUTIndex));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetLUTIndex(int nParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_LUTIndex;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			try
			{
				if(ST_SetIntReg(m_hDevice, MCAM_LUT_INDEX, nParam))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetLUTValue(int *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterCrevisGigE_LUTValue));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetLUTValue(int nParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_LUTValue;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			try
			{
				if(ST_SetIntReg(m_hDevice, MCAM_LUT_VALUE, nParam))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetGainselector(EDeviceCrevisGigEGainSelector *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceCrevisGigEGainSelector)_ttoi(GetParamValue(EDeviceParameterCrevisGigE_Gainselector));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetGainselector(EDeviceCrevisGigEGainSelector eParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_Gainselector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			try
			{
				CStringA strTemp(g_lpszCrevisGigEGainSelector[eParam]);
				if(ST_SetEnumReg(m_hDevice, MCAM_GAIN_SELECTOR, (LPSTR)(LPCSTR)(strTemp)))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], g_lpszCrevisGigEGainSelector[nPreValue], g_lpszCrevisGigEGainSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetGainRaw(int *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterCrevisGigE_GainRaw));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetGainRaw(int nParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_GainRaw;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			try
			{
				if(ST_SetIntReg(m_hDevice, MCAM_GAIN_RAW, nParam))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetGainAuto(EDeviceCrevisGigEGainAuto *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceCrevisGigEGainAuto)_ttoi(GetParamValue(EDeviceParameterCrevisGigE_GainAuto));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetGainAuto(EDeviceCrevisGigEGainAuto eParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_GainAuto;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			try
			{
				CStringA strTemp(g_lpszCrevisGigEGainAuto[eParam]);
				if(ST_SetEnumReg(m_hDevice, MCAM_GAIN_AUTO, (LPSTR)(LPCSTR)(strTemp)))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], g_lpszCrevisGigEGainAuto[nPreValue], g_lpszCrevisGigEGainAuto[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetBlackLevelSelector(EDeviceCrevisGigEBlackLevelSelector *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceCrevisGigEBlackLevelSelector)_ttoi(GetParamValue(EDeviceParameterCrevisGigE_BlackLevelSelector));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetBlackLevelSelector(EDeviceCrevisGigEBlackLevelSelector eParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_BlackLevelSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			try
			{
				CStringA strTemp(g_lpszCrevisGigEBlackLevelSelector[eParam]);
				if(ST_SetEnumReg(m_hDevice, MCAM_BLACK_LEVEL_SELECTOR, (LPSTR)(LPCSTR)(strTemp)))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], g_lpszCrevisGigEBlackLevelSelector[nPreValue], g_lpszCrevisGigEBlackLevelSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetBlackLevelRaw(int *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterCrevisGigE_BlackLevelRaw));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetBlackLevelRaw(int nParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_BlackLevelRaw;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			try
			{
				if(ST_SetIntReg(m_hDevice, MCAM_BLACK_LEVEL_RAW, nParam))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetBalanceRatioSelector(EDeviceCrevisGigEBalanceRatioSelector *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceCrevisGigEBalanceRatioSelector)_ttoi(GetParamValue(EDeviceParameterCrevisGigE_BalanceRatioSelector));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetBalanceRatioSelector(EDeviceCrevisGigEBalanceRatioSelector eParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_BalanceRatioSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			try
			{
				CStringA strTemp(g_lpszCrevisGigEBalanceRatioSelector[eParam]);
				if(ST_SetEnumReg(m_hDevice, MCAM_BALANCE_RATIO_SELECTOR, (LPSTR)(LPCSTR)(strTemp)))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], g_lpszCrevisGigEBalanceRatioSelector[nPreValue], g_lpszCrevisGigEBalanceRatioSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetBalanceRatio(float *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterCrevisGigE_BalanceRatio));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetBalanceRatio(float fParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_BalanceRatio;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			try
			{
				if(ST_SetFloatReg(m_hDevice, MCAM_BALANCE_RATIO, fParam))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetBalanceWhiteAuto(EDeviceCrevisGigEBalanceWhiteAuto *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceCrevisGigEBalanceWhiteAuto)_ttoi(GetParamValue(EDeviceParameterCrevisGigE_BalanceWhiteAuto));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetBalanceWhiteAuto(EDeviceCrevisGigEBalanceWhiteAuto eParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_BalanceWhiteAuto;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			try
			{
				CStringA strTemp(g_lpszCrevisGigEBalanceWhiteAuto[eParam]);
				if(ST_SetEnumReg(m_hDevice, MCAM_BALANCE_WHITE_AUTO, (LPSTR)(LPCSTR)(strTemp)))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], g_lpszCrevisGigEBalanceWhiteAuto[nPreValue], g_lpszCrevisGigEBalanceWhiteAuto[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetUserSetSelector(EDeviceCrevisGigEUserSetSelector *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceCrevisGigEUserSetSelector)_ttoi(GetParamValue(EDeviceParameterCrevisGigE_UserSetSelector));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetUserSetSelector(EDeviceCrevisGigEUserSetSelector eParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_UserSetSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			try
			{
				CStringA strTemp(g_lpszCrevisGigEUserSetSelector[eParam]);
				if(ST_SetEnumReg(m_hDevice, USER_SET_SELECTOR, (LPSTR)(LPCSTR)(strTemp)))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], g_lpszCrevisGigEUserSetSelector[nPreValue], g_lpszCrevisGigEUserSetSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetDeviceFilterDriverMode(EDeviceCrevisGigEDeviceFilterDriverMode *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceCrevisGigEDeviceFilterDriverMode)_ttoi(GetParamValue(EDeviceParameterCrevisGigE_DeviceFilterDriverMode));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetDeviceFilterDriverMode(EDeviceCrevisGigEDeviceFilterDriverMode eParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_DeviceFilterDriverMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			try
			{
				CStringA strTemp(g_lpszCrevisGigEDeviceFilterDriverMode[eParam]);
				if(ST_SetEnumReg(m_hDevice, MCAM_DEVICE_FILTER_DRIVER_MODE, (LPSTR)(LPCSTR)(strTemp)))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], g_lpszCrevisGigEDeviceFilterDriverMode[nPreValue], g_lpszCrevisGigEDeviceFilterDriverMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetDeviceCommandTimeout(int *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterCrevisGigE_DeviceCommandTimeout));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetDeviceCommandTimeout(int nParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_DeviceCommandTimeout;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			try
			{
				if(ST_SetIntReg(m_hDevice, MCAM_DEVICE_COMMAND_TIMEOUT, nParam))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetDeviceCommandRetryNumber(int *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterCrevisGigE_DeviceCommandRetryNumber));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetDeviceCommandRetryNumber(int nParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_DeviceCommandRetryNumber;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			try
			{
				if(ST_SetIntReg(m_hDevice, MCAM_DEVICE_COMMAND_RETRY_NUMBER, nParam))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetDeviceMissingPacketReceive(EDeviceCrevisGigEDeviceMissingPackedReceive *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceCrevisGigEDeviceMissingPackedReceive)_ttoi(GetParamValue(EDeviceParameterCrevisGigE_DeviceMissingPacketReceive));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetDeviceMissingPacketReceive(EDeviceCrevisGigEDeviceMissingPackedReceive eParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_DeviceMissingPacketReceive;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			try
			{
				CStringA strTemp(g_lpszCrevisGigEDeviceMissingPackedReceive[eParam]);
				if(ST_SetEnumReg(m_hDevice, MCAM_DEVICE_MISSING_PACKET_RECEIVE, (LPSTR)(LPCSTR)(strTemp)))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], g_lpszCrevisGigEDeviceMissingPackedReceive[nPreValue], g_lpszCrevisGigEDeviceMissingPackedReceive[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetDevicePacketResend(bool *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterCrevisGigE_DevicePacketResend));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetDevicePacketResend(bool bParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_DevicePacketResend;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			try
			{
				if(ST_SetBoolReg(m_hDevice, MCAM_DEVICE_PACKET_RESEND, bParam))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strOffset;
		strOffset.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strOffset))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], g_lpszCrevisGigESwitch[nPreValue], g_lpszCrevisGigESwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetDeviceMaxPacketResendCount(int *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterCrevisGigE_DeviceMaxPacketResendCount));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetDeviceMaxPacketResendCount(int nParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_DeviceMaxPacketResendCount;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			try
			{
				if(ST_SetIntReg(m_hDevice, MCAM_DEVICE_MAX_PACKET_RESEND_COUNT, nParam))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetColorTransformationEnable(bool *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterCrevisGigE_ColorTransformationEnable));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetColorTransformationEnable(bool bParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_ColorTransformationEnable;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			try
			{
				if(ST_SetBoolReg(m_hDevice, MCAM_COLOR_TRANS_FORMATION_ENABLE, bParam))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strOffset;
		strOffset.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strOffset))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], g_lpszCrevisGigESwitch[nPreValue], g_lpszCrevisGigESwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetColorTransformationValueSelector(EDeviceCrevisGigEColorTransformationValueSeletor *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceCrevisGigEColorTransformationValueSeletor)_ttoi(GetParamValue(EDeviceParameterCrevisGigE_ColorTransformationValueSelector));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetColorTransformationValueSelector(EDeviceCrevisGigEColorTransformationValueSeletor eParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_ColorTransformationValueSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			try
			{
				CStringA strTemp(g_lpszCrevisGigEColorTransformationValueSeletor[eParam]);
				if(ST_SetEnumReg(m_hDevice, MCAM_COLOR_TRANS_FORMATION_VALUE_SELECTOR, (LPSTR)(LPCSTR)(strTemp)))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], g_lpszCrevisGigEColorTransformationValueSeletor[nPreValue], g_lpszCrevisGigEColorTransformationValueSeletor[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ECrevisGigEGetFunction CDeviceCrevisGigE::GetColorTransformationValue(float *pParam)
{
	ECrevisGigEGetFunction eReturn = ECrevisGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ECrevisGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterCrevisGigE_ColorTransformationValue));

		eReturn = ECrevisGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ECrevisGigESetFunction CDeviceCrevisGigE::SetColorTransformationValue(float fParam)
{
	ECrevisGigESetFunction eReturn = ECrevisGigESetFunction_UnknownError;

	EDeviceParameterCrevisGigE eSaveID = EDeviceParameterCrevisGigE_ColorTransformationValue;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			try
			{
				if(ST_SetFloatReg(m_hDevice, MCAM_COLOR_TRANS_FORMATION_VALUE, fParam))
				{
					eReturn = ECrevisGigESetFunction_WriteToDeviceError;
					break;
				}
			}
			catch(...)
			{
				eReturn = ECrevisGigESetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ECrevisGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ECrevisGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCrevisGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceCrevisGigE::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = false;
	bool bFoundID = true;

	do
	{
		switch(nParam)
		{
		case EDeviceParameterCrevisGigE_DeviceID:
			bReturn = !SetDeviceID(strValue);
			break;
		case EDeviceParameterCrevisGigE_InitializeMode:
			bReturn = !SetInitializeMode((EDeviceCrevisGigEInitMode)_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_AcqInvalidTime:
			bReturn = !SetAcqInvalidTime(_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_GrabTimeout:
			bReturn = !SetGrabTimeout(_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_DetailedLog:
			bReturn = !SetDetailedLog(_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_GevHeartbeatTimeout:
			bReturn = !SetGevHeartbeatTimeout(_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_GevGVCPHeartbeatDisable:
			bReturn = !SetGevGVCPHearteatDisable(_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_GevSCPSPacketSize:
			bReturn = !SetGevSCPSPacketSize(_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_Width:
			bReturn = !SetWidth(_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_Height:
			bReturn = !SetHeight(_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_PixelFormat:
			bReturn = !SetPixelFormat((EDeviceCrevisGigEPixelFormat)_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_OffsetX:
			bReturn = !SetOffsetX(_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_OffsetY:
			bReturn = !SetOffsetY(_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_TestPattern:
			bReturn = !SetTestPattern((EDeviceCrevisGigETestPattern)_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_AcquisitionMode:
			bReturn = !SetAcquisitionMode((EDeviceCrevisGigEAcquistionMode)_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_AcquisitionFrameCount:
			bReturn = !SetAcquisitionFrameCount(_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_TriggerSelector:
			bReturn = !SetTriggerSelector((EDeviceCrevisGigETriggerSelector)_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_TriggerSource:
			bReturn = !SetTriggerSource((EDeviceCrevisGigETriggerSource)_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_Line1TriggerMode:
			bReturn = !SetLine1TriggerMode((EDeviceCrevisGigETriggerMode)_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_Line1TriggerActivation:
			bReturn = !SetLine1TriggerActivation((EDeviceCrevisGigETriggerActivation)_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_Line1TriggerDelay:
			bReturn = !SetLine1TriggerDelay(_ttof(strValue));
			break;
		case EDeviceParameterCrevisGigE_SoftwareTriggerMode:
			bReturn = !SetSoftwareTriggerMode((EDeviceCrevisGigETriggerMode)_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_SoftwareTriggerActivation:
			bReturn = !SetSoftwareTriggerActivation((EDeviceCrevisGigETriggerActivation)_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_SoftwareTriggerDelay:
			bReturn = !SetSoftwareTriggerDelay(_ttof(strValue));
			break;
		case EDeviceParameterCrevisGigE_ExposureMode:
			bReturn = !SetExposureMode((EDeviceCrevisGigEExposureMode)_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_ExposureAuto:
			bReturn = !SetExposureAuto((EDeviceCrevisGigEExposureAuto)_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_ExposureTime:
			bReturn = !SetExposureTime(_ttof(strValue));
			break;
		case EDeviceParameterCrevisGigE_AcquisitionFrameRateEnable:
			bReturn = !SetAcquisitionFrameRateEnable((EDeviceCrevisGigEAcquistionFramerateEnable)_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_AcquisitionFrameRate:
			bReturn = !SetAcquisitionFrameRate(_ttof(strValue));
			break;
		case EDeviceParameterCrevisGigE_AutoExposureTarget:
			bReturn = !SetAutoExposureTarget(_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_LineSelector:
			bReturn = !SetLineSelector((EDeviceCrevisGigELineSelector)_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_LineInverter:
			bReturn = !SetLineInverter(_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_LineSource:
			bReturn = !SetLineSource((EDeviceCrevisGigELineSource)_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_UserOutputSelector:
			bReturn = !SetUserOutputSelector((EDeviceCrevisGigEUserOutputSelector)_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_TimerSelector:
			bReturn = !SetTimerSelector((EDeviceCrevisGigETimerSelector)_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_TimerDuration:
			bReturn = !SetTimerDuration(_ttof(strValue));
			break;
		case EDeviceParameterCrevisGigE_TimerDelay:
			bReturn = !SetTimerDelay(_ttof(strValue));
			break;
		case EDeviceParameterCrevisGigE_LUTEnable:
			bReturn = !SetLUTEnable(_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_LUTIndex:
			bReturn = !SetLUTIndex(_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_LUTValue:
			bReturn = !SetLUTValue(_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_Gainselector:
			bReturn = !SetGainselector((EDeviceCrevisGigEGainSelector)_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_GainRaw:
			bReturn = !SetGainRaw(_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_GainAuto:
			bReturn = !SetGainAuto((EDeviceCrevisGigEGainAuto)_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_BlackLevelSelector:
			bReturn = !SetBlackLevelSelector((EDeviceCrevisGigEBlackLevelSelector)_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_BlackLevelRaw:
			bReturn = !SetBlackLevelRaw(_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_BalanceRatioSelector:
			bReturn = !SetBalanceRatioSelector((EDeviceCrevisGigEBalanceRatioSelector)_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_BalanceRatio:
			bReturn = !SetBalanceRatio(_ttof(strValue));
			break;
		case EDeviceParameterCrevisGigE_BalanceWhiteAuto:
			bReturn = !SetBalanceWhiteAuto((EDeviceCrevisGigEBalanceWhiteAuto)_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_UserSetSelector:
			bReturn = !SetUserSetSelector((EDeviceCrevisGigEUserSetSelector)_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_DeviceFilterDriverMode:
			bReturn = !SetDeviceFilterDriverMode((EDeviceCrevisGigEDeviceFilterDriverMode)_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_DeviceCommandTimeout:
			bReturn = !SetDeviceCommandTimeout(_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_DeviceCommandRetryNumber:
			bReturn = !SetDeviceCommandRetryNumber(_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_DeviceMissingPacketReceive:
			bReturn = !SetDeviceMissingPacketReceive((EDeviceCrevisGigEDeviceMissingPackedReceive)_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_DevicePacketResend:
			bReturn = !SetDevicePacketResend(_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_DeviceMaxPacketResendCount:
			bReturn = !SetDeviceMaxPacketResendCount(_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_ColorTransformationEnable:
			bReturn = !SetColorTransformationEnable(_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_ColorTransformationValueSelector:
			bReturn = !SetColorTransformationValueSelector((EDeviceCrevisGigEColorTransformationValueSeletor)_ttoi(strValue));
			break;
		case EDeviceParameterCrevisGigE_ColorTransformationValue:
			bReturn = !SetColorTransformationValue(_ttof(strValue));
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

bool CDeviceCrevisGigE::DoesModuleExist()
{
	bool bReturn = false;

	CString strModuleName;

	do
	{
		strModuleName.Format(_T("VirtualFG40.dll"));

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

		strModuleName.Format(_T("Syscam40.dll"));

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

			bReturn &= true;
		}
		else
			bReturn &= eStatus == EDeviceLibraryStatus_FoundTrue ? true : false;

		strModuleName.Format(_T("U3VLib.dll"));

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

			bReturn &= true;
		}
		else
			bReturn &= eStatus == EDeviceLibraryStatus_FoundTrue ? true : false;

		strModuleName.Format(_T("SphinxLib.dll"));

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

			bReturn &= true;
		}
		else
			bReturn &= eStatus == EDeviceLibraryStatus_FoundTrue ? true : false;

		strModuleName.Format(_T("libusbK.dll"));

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

			bReturn &= true;
		}
		else
			bReturn &= eStatus == EDeviceLibraryStatus_FoundTrue ? true : false;

		strModuleName.Format(_T("libxml2.dll"));

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

			bReturn &= true;
		}
		else
			bReturn &= eStatus == EDeviceLibraryStatus_FoundTrue ? true : false;

		strModuleName.Format(_T("libxslt.dll"));

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

			bReturn &= true;
		}
		else
			bReturn &= eStatus == EDeviceLibraryStatus_FoundTrue ? true : false;
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, CMultiLanguageManager::GetString(ELanguageParameter__s_d_s_toload_s), GetClassNameStr(), GetObjectID(), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), strModuleName);

	return bReturn;
}

__int32 CDeviceCrevisGigE::CallbackFunction(__int32 event, void *pImage, void *pUserDefine)
{
	do 
	{
		CDeviceCrevisGigE* pInstance = (CDeviceCrevisGigE*)(pUserDefine);
		if(!pInstance)
			break;

		if(!pImage)
			break;

		pInstance->NextImageIndex();

		CRavidImage* pCurrentImage = pInstance->GetImageInfo();

		BYTE* pCurrentBuffer = pCurrentImage->GetBuffer();
		BYTE** ppCurrentOffsetY = pCurrentImage->GetYOffsetTable();

		const int64_t i64Width = (int64_t)pCurrentImage->GetSizeX();
		const int64_t i64Height = (int64_t)pCurrentImage->GetSizeY();
		const int64_t i64WidthStep = (int64_t)pCurrentImage->GetWidthStep();
		const int64_t i64ImageSizeByte = (int64_t)pCurrentImage->GetImageSizeByte();
		const int64_t i64PixelSizeByte = (int64_t)pCurrentImage->GetPixelSizeByte();

		switch(event)
		{
		case EVENT_NEW_IMAGE:
			{
				for(int64_t i = 0; i < i64Height; ++i)
					memcpy(*(ppCurrentOffsetY + i), (BYTE*)(pImage) + (i * i64Width * i64PixelSizeByte), i64Width);

				pInstance->ConnectImage(false);

				CEventHandlerManager::BroadcastOnAcquisition(pCurrentImage, pInstance->GetObjectID(), pInstance->GetImageView(), pInstance);
				
				if(pInstance->IsLive())
					break;

				if(!pInstance->IsGrabAvailable())
					pInstance->Stop();
			}
			break;
		case EVENT_GRAB_ERROR:
			break;
		}
	}
	while(false);

	return 0;
}

void CDeviceCrevisGigE::UpdateDeviceToParameter()
{
	do
	{
		unsigned int unParamSize = 256;
		char arrParam[256] = { 0, };

		bool bParam = 0;
		int nParam = 0;
		unsigned int unParam = 0;
		double dblParam = 0.0;
		try
		{
			unParam = 0;
			if(!ST_GetAcqInvalidTime(m_hDevice, &unParam))
				SetAcqInvalidTime(unParam);
		}
		catch(...) { }


		try
		{
			unParam = 0;
			if(!ST_GetGrabTimeout(m_hDevice, &unParam))
				SetGrabTimeout(unParam);
		}
		catch(...) { }

		try
		{
			bParam = 0;
			if(!ST_GetDetailedLog(m_hDevice, &bParam))
				SetDetailedLog(bParam);
		}
		catch(...) { }

		try
		{
			nParam = 0;
			if(!ST_GetIntReg(m_hDevice, GEV_HEARTBEAT_TIMEOUT, &nParam))
				SetGevHeartbeatTimeout(nParam);
		}
		catch(...) { }

		try
		{
			bParam = 0;
			if(!ST_GetBoolReg(m_hDevice, GEV_GVCP_HEARTBEAT_DISABLE, &bParam))
				SetGevGVCPHearteatDisable(bParam);
		}
		catch(...) { }

		try
		{
			nParam = 0;
			if(!ST_GetIntReg(m_hDevice, GEV_SCPS_PACKETSIZE, &nParam))
				SetGevSCPSPacketSize(nParam);
		}
		catch(...) { }

		try
		{
			nParam = 0;
			if(!ST_GetIntReg(m_hDevice, MCAM_WIDTH, &nParam))
				SetWidth(nParam);
		}
		catch(...) { }

		try
		{
			nParam = 0;
			if(!ST_GetIntReg(m_hDevice, MCAM_HEIGHT, &nParam))
				SetHeight(nParam);
		}
		catch(...) { }

		try
		{
			unParamSize = 256;
			memset(arrParam, 0, sizeof(char) * unParamSize);
			if(!ST_GetEnumReg(m_hDevice, MCAM_PIXEL_FORMAT, arrParam, &unParamSize))
			{
				CString str;
				str = arrParam;

				for(int i = 0; i < EDeviceCrevisGigEPixelFormat_Count; ++i)
				{
					if(str.CompareNoCase(g_lpszCrevisGigEPixelFormat[i]))
						continue;
					SetPixelFormat((EDeviceCrevisGigEPixelFormat)i);
					break;
				}
			}
		}
		catch(...) { }

		try
		{
			nParam = 0;
			if(!ST_GetIntReg(m_hDevice, MCAM_OFFSET_X, &nParam))
				SetOffsetX(nParam);
		}
		catch(...) { }

		try
		{
			nParam = 0;
			if(!ST_GetIntReg(m_hDevice, MCAM_OFFSET_Y, &nParam))
				SetOffsetY(nParam);
		}
		catch(...) { }

		try
		{
			unParamSize = 256;
			memset(arrParam, 0, sizeof(char) * unParamSize);
			if(!ST_GetEnumReg(m_hDevice, MCAM_TEST_PATTERN, arrParam, &unParamSize))
			{
				CString str;
				str = arrParam;

				for(int i = 0; i < EDeviceCrevisGigETestPattern_Count; ++i)
				{
					if(str.CompareNoCase(g_lpszCrevisGigETestPattern[i]))
						continue;
					SetTestPattern((EDeviceCrevisGigETestPattern)i);
					break;
				}
			}
		}
		catch(...) { }

		try
		{
			unParamSize = 256;
			memset(arrParam, 0, sizeof(char) * unParamSize);
			if(!ST_GetEnumReg(m_hDevice, MCAM_ACQUISITION_MODE, arrParam, &unParamSize))
			{
				CString str;
				str = arrParam;

				for(int i = 0; i < EDeviceCrevisGigEAcquistionMode_Count; ++i)
				{
					if(str.CompareNoCase(g_lpszCrevisGigEAcquistionMode[i]))
						continue;
					SetAcquisitionMode((EDeviceCrevisGigEAcquistionMode)i);
					break;
				}
			}
		}
		catch(...) { }

		try
		{
			nParam = 0;
			if(!ST_GetIntReg(m_hDevice, MCAM_ACQUISITION_FRAME_COUNT, &nParam))
				SetAcquisitionFrameCount(nParam);
		}
		catch(...) { }

		try
		{
			unParamSize = 256;
			memset(arrParam, 0, sizeof(char) * unParamSize);
			if(!ST_GetEnumReg(m_hDevice, MCAM_TRIGGER_SELECTOR, arrParam, &unParamSize))
			{
				CString str;
				str = arrParam;

				for(int i = 0; i < EDeviceCrevisGigETriggerSelector_Count; ++i)
				{
					if(str.CompareNoCase(g_lpszCrevisGigETriggerSelector[i]))
						continue;
					SetTriggerSelector((EDeviceCrevisGigETriggerSelector)i);
					break;
				}
			}
		}
		catch(...) { }

		EDeviceCrevisGigETriggerSource eTriggerSource = EDeviceCrevisGigETriggerSource_Count;

		try
		{
			unParamSize = 256;
			memset(arrParam, 0, sizeof(char) * unParamSize);
			if(!ST_GetEnumReg(m_hDevice, MCAM_TRIGGER_SOURCE, arrParam, &unParamSize))
			{
				CString str;
				str = arrParam;

				for(int i = 0; i < EDeviceCrevisGigETriggerSource_Count; ++i)
				{
					if(str.CompareNoCase(g_lpszCrevisGigETriggerSource[i]))
						continue;
					eTriggerSource = (EDeviceCrevisGigETriggerSource)i;
					SetTriggerSource(eTriggerSource);
					break;
				}
			}
		}
		catch(...) { }

		try
		{
			unParamSize = 256;
			memset(arrParam, 0, sizeof(char) * unParamSize);
			if(!ST_GetEnumReg(m_hDevice, MCAM_TRIGGER_MODE, arrParam, &unParamSize))
			{
				CString str;
				str = arrParam;

				for(int i = 0; i < EDeviceCrevisGigETriggerMode_Count; ++i)
				{
					if(str.CompareNoCase(g_lpszCrevisGigETriggerMode[i]))
						continue;
					if(eTriggerSource == EDeviceCrevisGigETriggerSource_Line1)
						SetLine1TriggerMode((EDeviceCrevisGigETriggerMode)i);
					else if(eTriggerSource == EDeviceCrevisGigETriggerSource_Software)
						SetSoftwareTriggerMode((EDeviceCrevisGigETriggerMode)i);
					break;
				}
			}
		}
		catch(...) { }

		try
		{
			unParamSize = 256;
			memset(arrParam, 0, sizeof(char) * unParamSize);
			if(!ST_GetEnumReg(m_hDevice, MCAM_TRIGGER_ACTIVATION, arrParam, &unParamSize))
			{
				CString str;
				str = arrParam;

				for(int i = 0; i < EDeviceCrevisGigETriggerActivation_Count; ++i)
				{
					if(str.CompareNoCase(g_lpszCrevisGigETriggerActivation[i]))
						continue;
					if(eTriggerSource == EDeviceCrevisGigETriggerSource_Line1)
						SetLine1TriggerActivation((EDeviceCrevisGigETriggerActivation)i);
					else if(eTriggerSource == EDeviceCrevisGigETriggerSource_Software)
						SetSoftwareTriggerActivation((EDeviceCrevisGigETriggerActivation)i);
					break;
				}
			}
		}
		catch(...) { }
		
		try
		{
			dblParam = 0.0;
			if(!ST_GetFloatReg(m_hDevice, MCAM_TRIGGER_DELAY, &dblParam))
			{
				if(eTriggerSource == EDeviceCrevisGigETriggerSource_Line1)
					SetLine1TriggerDelay(dblParam);
				else if(eTriggerSource == EDeviceCrevisGigETriggerSource_Software)
					SetSoftwareTriggerDelay(dblParam);
			}
		}
		catch(...) { }

		try
		{
			unParamSize = 256;
			memset(arrParam, 0, sizeof(char) * unParamSize);
			if(!ST_GetEnumReg(m_hDevice, MCAM_EXPOSURE_MODE, arrParam, &unParamSize))
			{
				CString str;
				str = arrParam;

				for(int i = 0; i < EDeviceCrevisGigEExposureMode_Count; ++i)
				{
					if(str.CompareNoCase(g_lpszCrevisGigEExposureMode[i]))
						continue;
					SetExposureMode((EDeviceCrevisGigEExposureMode)i);
					break;
				}
			}
		}
		catch(...) { }

		try
		{
			unParamSize = 256;
			memset(arrParam, 0, sizeof(char) * unParamSize);
			if(!ST_GetEnumReg(m_hDevice, MCAM_EXPOSURE_AUTO, arrParam, &unParamSize))
			{
				CString str;
				str = arrParam;

				for(int i = 0; i < EDeviceCrevisGigEExposureAuto_Count; ++i)
				{
					if(str.CompareNoCase(g_lpszCrevisGigEExposureAuto[i]))
						continue;
					SetExposureAuto((EDeviceCrevisGigEExposureAuto)i);
					break;
				}
			}
		}
		catch(...) { }

		try
		{
			dblParam = 0.0;
			if(!ST_GetFloatReg(m_hDevice, MCAM_EXPOSURE_TIME, &dblParam))
				SetExposureTime(dblParam);
		}
		catch(...) { }

		try
		{
			unParamSize = 256;
			memset(arrParam, 0, sizeof(char) * unParamSize);
			if(!ST_GetEnumReg(m_hDevice, MCAM_ACQUISITION_FRAMERATE_ENABLE, arrParam, &unParamSize))
			{
				CString str;
				str = arrParam;

				for(int i = 0; i < EDeviceCrevisGigEAcquistionFramerateEnable_Count; ++i)
				{
					if(str.CompareNoCase(g_lpszCrevisGigEAcquistionFramerateEnable[i]))
						continue;
					SetAcquisitionFrameRateEnable((EDeviceCrevisGigEAcquistionFramerateEnable)i);
					break;
				}
			}
		}
		catch(...) { }

		try
		{
			dblParam = 0.0;
			if(!ST_GetFloatReg(m_hDevice, MCAM_ACQUISITIONF_RAMERATE, &dblParam))
				SetAcquisitionFrameRate(dblParam);
		}
		catch(...) { }

		try
		{
			nParam = 0;
			if(!ST_GetIntReg(m_hDevice, MCAM_AUTO_EXPOSURE_TARGET, &nParam))
				SetAutoExposureTarget(nParam);
		}
		catch(...) { }

		try
		{
			unParamSize = 256;
			memset(arrParam, 0, sizeof(char) * unParamSize);
			if(!ST_GetEnumReg(m_hDevice, MCAM_LINE_SELECTOR, arrParam, &unParamSize))
			{
				CString str;
				str = arrParam;

				for(int i = 0; i < EDeviceCrevisGigELineSelector_Count; ++i)
				{
					if(str.CompareNoCase(g_lpszCrevisGigELineSelector[i]))
						continue;
					SetLineSelector((EDeviceCrevisGigELineSelector)i);
					break;
				}
			}
		}
		catch(...) { }

		try
		{
			bParam = 0;
			if(!ST_GetBoolReg(m_hDevice, MCAM_LINE_INVERTER, &bParam))
				SetLineInverter(bParam);
		}
		catch(...) { }

		try
		{
			unParamSize = 256;
			memset(arrParam, 0, sizeof(char) * unParamSize);
			if(!ST_GetEnumReg(m_hDevice, MCAM_LINE_SOURCE, arrParam, &unParamSize))
			{
				CString str;
				str = arrParam;

				for(int i = 0; i < EDeviceCrevisGigELineSource_Count; ++i)
				{
					if(str.CompareNoCase(g_lpszCrevisGigELineSource[i]))
						continue;
					SetLineSource((EDeviceCrevisGigELineSource)i);
					break;
				}
			}
		}
		catch(...) { }

		try
		{
			unParamSize = 256;
			memset(arrParam, 0, sizeof(char) * unParamSize);
			if(!ST_GetEnumReg(m_hDevice, MCAM_USER_OUTPUT_SELECTOR, arrParam, &unParamSize))
			{
				CString str;
				str = arrParam;

				for(int i = 0; i < EDeviceCrevisGigEUserOutputSelector_Count; ++i)
				{
					if(str.CompareNoCase(g_lpszCrevisGigEUserOutputSelector[i]))
						continue;
					SetUserOutputSelector((EDeviceCrevisGigEUserOutputSelector)i);
					break;
				}
			}
		}
		catch(...) { }

		try
		{
			unParamSize = 256;
			memset(arrParam, 0, sizeof(char) * unParamSize);
			if(!ST_GetEnumReg(m_hDevice, MCAM_TIMER_SELECTOR, arrParam, &unParamSize))
			{
				CString str;
				str = arrParam;

				for(int i = 0; i < EDeviceCrevisGigETimerSelector_Count; ++i)
				{
					if(str.CompareNoCase(g_lpszCrevisGigETimerSelector[i]))
						continue;
					SetTimerSelector((EDeviceCrevisGigETimerSelector)i);
					break;
				}
			}
		}
		catch(...) { }

		try
		{
			dblParam = 0.0;
			if(!ST_GetFloatReg(m_hDevice, MCAM_TIMER_DURATION, &dblParam))
				SetTimerDuration(dblParam);
		}
		catch(...) { }

		try
		{
			dblParam = 0.0;
			if(!ST_GetFloatReg(m_hDevice, MCAM_TIMER_DELAY, &dblParam))
				SetTimerDelay(dblParam);
		}
		catch(...) { }

		try
		{
			bParam = 0;
			if(!ST_GetBoolReg(m_hDevice, MCAM_LUT_ENABLE, &bParam))
				SetLUTEnable(bParam);
		}
		catch(...) { }

		try
		{
			nParam = 0;
			if(!ST_GetIntReg(m_hDevice, MCAM_LUT_INDEX, &nParam))
				SetLUTIndex(nParam);
		}
		catch(...) { }

		try
		{
			nParam = 0;
			if(!ST_GetIntReg(m_hDevice, MCAM_LUT_VALUE, &nParam))
				SetLUTValue(nParam);
		}
		catch(...) { }

		try
		{
			unParamSize = 256;
			memset(arrParam, 0, sizeof(char) * unParamSize);
			if(!ST_GetEnumReg(m_hDevice, MCAM_GAIN_SELECTOR, arrParam, &unParamSize))
			{
				CString str;
				str = arrParam;

				for(int i = 0; i < EDeviceCrevisGigEGainSelector_Count; ++i)
				{
					if(str.CompareNoCase(g_lpszCrevisGigEGainSelector[i]))
						continue;
					SetGainselector((EDeviceCrevisGigEGainSelector)i);
					break;
				}
			}
		}
		catch(...) { }

		try
		{
			nParam = 0;
			if(!ST_GetIntReg(m_hDevice, MCAM_GAIN_RAW, &nParam))
				SetGainRaw(nParam);
		}
		catch(...) { }

		try
		{
			unParamSize = 256;
			memset(arrParam, 0, sizeof(char) * unParamSize);
			if(!ST_GetEnumReg(m_hDevice, MCAM_GAIN_AUTO, arrParam, &unParamSize))
			{
				CString str;
				str = arrParam;

				for(int i = 0; i < EDeviceCrevisGigEGainAuto_Count; ++i)
				{
					if(str.CompareNoCase(g_lpszCrevisGigEGainAuto[i]))
						continue;
					SetGainAuto((EDeviceCrevisGigEGainAuto)i);
					break;
				}
			}
		}
		catch(...) { }

		try
		{
			unParamSize = 256;
			memset(arrParam, 0, sizeof(char) * unParamSize);
			if(!ST_GetEnumReg(m_hDevice, MCAM_BLACK_LEVEL_SELECTOR, arrParam, &unParamSize))
			{
				CString str;
				str = arrParam;

				for(int i = 0; i < EDeviceCrevisGigEBlackLevelSelector_Count; ++i)
				{
					if(str.CompareNoCase(g_lpszCrevisGigEBlackLevelSelector[i]))
						continue;
					SetBlackLevelSelector((EDeviceCrevisGigEBlackLevelSelector)i);
					break;
				}
			}
		}
		catch(...) { }

		try
		{
			nParam = 0;
			if(!ST_GetIntReg(m_hDevice, MCAM_BLACK_LEVEL_RAW, &nParam))
				SetBlackLevelRaw(nParam);
		}
		catch(...) { }

		try
		{
			unParamSize = 256;
			memset(arrParam, 0, sizeof(char) * unParamSize);
			if(!ST_GetEnumReg(m_hDevice, MCAM_BALANCE_RATIO_SELECTOR, arrParam, &unParamSize))
			{
				CString str;
				str = arrParam;

				for(int i = 0; i < EDeviceCrevisGigEBalanceRatioSelector_Count; ++i)
				{
					if(str.CompareNoCase(g_lpszCrevisGigEBalanceRatioSelector[i]))
						continue;
					SetBalanceRatioSelector((EDeviceCrevisGigEBalanceRatioSelector)i);
					break;
				}
			}
		}
		catch(...) { }

		try
		{
			dblParam = 0.0;
			if(!ST_GetFloatReg(m_hDevice, MCAM_BALANCE_RATIO, &dblParam))
				SetBalanceRatio(dblParam);
		}
		catch(...) { }

		try
		{
			unParamSize = 256;
			memset(arrParam, 0, sizeof(char) * unParamSize);
			if(!ST_GetEnumReg(m_hDevice, MCAM_BALANCE_WHITE_AUTO, arrParam, &unParamSize))
			{
				CString str;
				str = arrParam;

				for(int i = 0; i < EDeviceCrevisGigEBalanceWhiteAuto_Count; ++i)
				{
					if(str.CompareNoCase(g_lpszCrevisGigEBalanceWhiteAuto[i]))
						continue;
					SetBalanceWhiteAuto((EDeviceCrevisGigEBalanceWhiteAuto)i);
					break;
				}
			}
		}
		catch(...) { }

		try
		{
			unParamSize = 256;
			memset(arrParam, 0, sizeof(char) * unParamSize);
			if(!ST_GetEnumReg(m_hDevice, USER_SET_SELECTOR, arrParam, &unParamSize))
			{
				CString str;
				str = arrParam;

				for(int i = 0; i < EDeviceCrevisGigEUserSetSelector_Count; ++i)
				{
					if(str.CompareNoCase(g_lpszCrevisGigEUserSetSelector[i]))
						continue;
					SetUserSetSelector((EDeviceCrevisGigEUserSetSelector)i);
					break;
				}
			}
		}
		catch(...) { }

		try
		{
			unParamSize = 256;
			memset(arrParam, 0, sizeof(char) * unParamSize);
			if(!ST_GetEnumReg(m_hDevice, MCAM_DEVICE_FILTER_DRIVER_MODE, arrParam, &unParamSize))
			{
				CString str;
				str = arrParam;

				for(int i = 0; i < EDeviceCrevisGigEDeviceFilterDriverMode_Count; ++i)
				{
					if(str.CompareNoCase(g_lpszCrevisGigEDeviceFilterDriverMode[i]))
						continue;
					SetDeviceFilterDriverMode((EDeviceCrevisGigEDeviceFilterDriverMode)i);
					break;
				}
			}
		}
		catch(...) { }

		try
		{
			nParam = 0;
			if(!ST_GetIntReg(m_hDevice, MCAM_DEVICE_COMMAND_TIMEOUT, &nParam))
				SetDeviceCommandTimeout(nParam);
		}
		catch(...) { }

		try
		{
			nParam = 0;
			if(!ST_GetIntReg(m_hDevice, MCAM_DEVICE_COMMAND_RETRY_NUMBER, &nParam))
				SetDeviceCommandRetryNumber(nParam);
		}
		catch(...) { }

		try
		{
			unParamSize = 256;
			memset(arrParam, 0, sizeof(char) * unParamSize);
			if(!ST_GetEnumReg(m_hDevice, MCAM_DEVICE_MISSING_PACKET_RECEIVE, arrParam, &unParamSize))
			{
				CString str;
				str = arrParam;

				for(int i = 0; i < EDeviceCrevisGigEDeviceMissingPackedReceive_Count; ++i)
				{
					if(str.CompareNoCase(g_lpszCrevisGigEDeviceMissingPackedReceive[i]))
						continue;
					SetDeviceMissingPacketReceive((EDeviceCrevisGigEDeviceMissingPackedReceive)i);
					break;
				}
			}
		}
		catch(...) { }

		try
		{
			bParam = 0;
			if(!ST_GetBoolReg(m_hDevice, MCAM_DEVICE_PACKET_RESEND, &bParam))
				SetDevicePacketResend(bParam);
		}
		catch(...) { }

		try
		{
			nParam = 0;
			if(!ST_GetIntReg(m_hDevice, MCAM_DEVICE_MAX_PACKET_RESEND_COUNT, &nParam))
				SetDeviceMaxPacketResendCount(nParam);
		}
		catch(...) { }

		try
		{
			bParam = 0;
			if(!ST_GetBoolReg(m_hDevice, MCAM_COLOR_TRANS_FORMATION_ENABLE, &bParam))
				SetColorTransformationEnable(bParam);
		}
		catch(...) { }

		try
		{
			unParamSize = 256;
			memset(arrParam, 0, sizeof(char) * unParamSize);
			if(!ST_GetEnumReg(m_hDevice, MCAM_COLOR_TRANS_FORMATION_VALUE_SELECTOR, arrParam, &unParamSize))
			{
				CString str;
				str = arrParam;

				for(int i = 0; i < EDeviceCrevisGigEColorTransformationValueSeletor_Count; ++i)
				{
					if(str.CompareNoCase(g_lpszCrevisGigEColorTransformationValueSeletor[i]))
						continue;
					SetColorTransformationValueSelector((EDeviceCrevisGigEColorTransformationValueSeletor)i);
					break;
				}
			}
		}
		catch(...) { }

		try
		{
			dblParam = 0.0;
			if(!ST_GetFloatReg(m_hDevice, MCAM_COLOR_TRANS_FORMATION_VALUE, &dblParam))
				SetColorTransformationValue(dblParam);
		}
		catch(...) { }
	}
	while(false);
}

int CDeviceCrevisGigE::__ST_SetIntReg(const char * NodeName, __int32 val)
{
	return ST_SetIntReg(m_hDevice, NodeName, val);
}

int CDeviceCrevisGigE::__ST_GetIntReg(const char * NodeName, __int32 * pVal)
{
	return ST_GetIntReg(m_hDevice, NodeName, pVal);
}

int CDeviceCrevisGigE::__ST_SetFloatReg(const char * NodeName, double fVal)
{
	return ST_SetFloatReg(m_hDevice, NodeName, fVal);
}

int CDeviceCrevisGigE::__ST_GetFloatReg(const char * NodeName, double * pFval)
{
	return ST_GetFloatReg(m_hDevice, NodeName, pFval);
}

int CDeviceCrevisGigE::__ST_SetBoolReg(const char * NodeName, bool bVal)
{
	return ST_SetBoolReg(m_hDevice, NodeName, bVal);
}

int CDeviceCrevisGigE::__ST_GetBoolReg(const char * NodeName, bool * pBval)
{
	return ST_GetBoolReg(m_hDevice, NodeName, pBval);
}

int CDeviceCrevisGigE::__ST_SetEnumReg(const char * NodeName, char * val)
{
	return ST_SetEnumReg(m_hDevice, NodeName, val);
}

int CDeviceCrevisGigE::__ST_GetEnumReg(const char * NodeName, char * pInfo, unsigned __int32 * pSize)
{
	return ST_GetEnumReg(m_hDevice, NodeName, pInfo, pSize);
}

int CDeviceCrevisGigE::__ST_SetStrReg(const char * NodeName, char * val)
{
	return ST_SetStrReg(m_hDevice, NodeName, val);
}

int CDeviceCrevisGigE::__ST_GetStrReg(const char * NodeName, char * pInfo, unsigned __int32 * pSize)
{
	return ST_GetStrReg(m_hDevice, NodeName, pInfo, pSize);
}

int CDeviceCrevisGigE::__ST_SetCmdReg(const char * NodeName)
{
	return ST_SetCmdReg(m_hDevice, NodeName);
}

#endif