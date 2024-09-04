#include "stdafx.h"

#include "DeviceBaslerUSB.h"

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

#include "../RavidFramework/RavidTreeView.h"
#include "../RavidFramework/RavidImageView.h"

#include "../Libraries/Includes/BaslerPylon/pylonc/PylonC.h"

// pylonc_md_vc120_v5_0.dll
#pragma comment(lib, COMMONLIB_PREFIX "BaslerPylon/PylonC_MD_vc120.lib")

using namespace Ravid;
using namespace Ravid::Device;
using namespace Ravid::Database;
using namespace Ravid::Framework;
using namespace Ravid::Algorithms;
using namespace Ravid::Miscellaneous;

IMPLEMENT_DYNAMIC(CDeviceBaslerUSB, CDeviceCamera)

BEGIN_MESSAGE_MAP(CDeviceBaslerUSB, CDeviceCamera)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


static LPCTSTR g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_Count] =
{
	_T("DeviceID"),
	_T("Camera Setting"),
	_T("Grab count"),
	_T("Canvas width"),
	_T("Canvas height"),
	_T("Offset Center X"),
	_T("Offset Center Y"),
	_T("Offset X"),
	_T("Offset Y"),
	_T("Reverse X"),
	_T("Reverse Y"),
	_T("Grab Waiting Time[ms]"),
	_T("PixelFormat"),
	_T("Analog Control"),
	_T("Gain Auto"),
	_T("Gain Selector"),
	_T("Gain[dB]"),
	_T("Black Level Selector"),
	_T("Black Level[DN]"),
	_T("Gamma"),
	_T("Acquisition Control"),
	_T("Shutter Mode"),
	_T("Exposure Auto"),
	_T("Exposure Mode"),
	_T("Exposure Time[us]"),
	_T("Trigger Selector"),
	_T("FrameStart"),
	_T("FrameStart Trigger Mode"),
	_T("FrameStart Trigger Source"),
	_T("FrameStart Trigger Activation"),
	_T("FrameStart Trigger Delay[us]"),
	_T("FrameBurstStart"),
	_T("FrameBurstStart Trigger Mode"),
	_T("FrameBurstStart Trigger Source"),
	_T("FrameBurstStart Trigger Activation"),
	_T("FrameBurstStart Trigger Delay[us]"),
	_T("Enable Acquisition Frame Rate"),
	_T("Acquisition Frame Rate[Hz]"),
	_T("Acquisition Status Selector"),
	_T("DigitalIO Conrol"),
	_T("LineSelector"),
	_T("Line1"),
	_T("Line1Mode"),
	_T("Line1Source"),
	_T("Line1Inverter"),
	_T("Line1DebouncerTime [us]"),
	_T("Line1MinimumOuputPulseWidth [us]"),
	_T("Line2"),
	_T("Line2Mode"),
	_T("Line2Source"),
	_T("Line2Inverter"),
	_T("Line2DebouncerTime [us]"),
	_T("Line2MinimumOuputPulseWidth [us]"),
	_T("Line3"),
	_T("Line3Mode"),
	_T("Line3Source"),
	_T("Line3Inverter"),
	_T("Line3DebouncerTime [us]"),
	_T("Line3MinimumOuputPulseWidth [us]"),
	_T("Line4"),
	_T("Line4Mode"),
	_T("Line4Source"),
	_T("Line4Inverter"),
	_T("Line4DebouncerTime [us]"),
	_T("Line4MinimumOuputPulseWidth [us]"),
};

static LPCTSTR g_lpszBaslerUSBPixelFormat[EDeviceBaslerUSBPixelFormat_Count] =
{
	_T("Mono8"),
	_T("Mono10"),
	_T("Mono10p"),
	_T("BayerGR8"),
	_T("BayerRG8"),
	_T("BayerGB8"),
	_T("BayerBG8"),
	_T("BayerGR10"),
	_T("BayerGR10p"),
	_T("BayerRG10"),
	_T("BayerRG10p"),
	_T("BayerGB10"),
	_T("BayerGB10p"),
	_T("BayerBG10"),
	_T("BayerBG10p"),
	_T("RGB8"),
	_T("BGR8"),
	_T("YCbCr422_8"),
};

static LPCTSTR g_lpszBaslerUSBGainAuto[EDeviceBaslerUSBGainAuto_Count] =
{
	_T("Off"),
	_T("Once"),
	_T("Continuous"),
};

static LPCTSTR g_lpszBaslerUSBGainSelector[EDeviceBaslerUSBGainSelector_Count] =
{
	_T("All"),
};

static LPCTSTR g_lpszBaslerUSBBlackLevelSelector[EDeviceBaslerUSBBlackLevelSelector_Count] =
{
	_T("All"),
};

static LPCTSTR g_lpszBaslerUSBShutterMode[EDeviceBaslerUSBShutterMode_Count] =
{
	_T("Global"),
	_T("Rolling"),
	_T("GlobalResetRelease"),
};

static LPCTSTR g_lpszBaslerUSBExposureAuto[EDeviceBaslerUSBExposureAuto_Count] =
{
	_T("Off"),
	_T("Once"),
	_T("Continuous"),
};

static LPCTSTR g_lpszBaslerUSBExposureMode[EDeviceBaslerUSBExposureMode_Count] =
{
	_T("Timed"),
	_T("TriggerWidth"),
};

static LPCTSTR g_lpszBaslerUSBTriggerSelector[EDeviceBaslerUSBTriggerSelector_Count] =
{
	_T("FrameStart"),
	_T("FrameBurstStart"),
};

static LPCTSTR g_lpszBaslerUSBTriggerMode[EDeviceBaslerUSBTriggerMode_Count] =
{
	_T("Off"),
	_T("On"),
};

static LPCTSTR g_lpszBaslerUSBTriggerSource[EDeviceBaslerUSBTriggerSource_Count] =
{
	_T("Software"),
	_T("Line1"),
	_T("Line2"),
	_T("Line3"),
	_T("Line4"),
	_T("SoftwareSignal1"),
	_T("SoftwareSignal2"),
	_T("SoftwareSignal3"),
};

static LPCTSTR g_lpszBaslerUSBTriggerActivation[EDeviceBaslerUSBTriggerActivation_Count] =
{
	_T("RisingEdge"),
	_T("FallingEdge"),
	_T("AnyEdge"),
	_T("LevelHigh"),
	_T("LevelLow"),
};

static LPCTSTR g_lpszBaslerUSBAcquisitionStatusSelector[EDeviceBaslerUSBAcquisitionStatusSelector_Count] =
{
	_T("FrameTriggerWait"),
	_T("FrameBurstTriggerWait"),
	_T("FrameBurstTriggerActive"),
	_T("FrameBurstTriggerTransfer"),
	_T("FrameActive"),
	_T("FrameTransfer"),
	_T("ExposureActive"),
};

static LPCTSTR g_lpszBaslerUSBLineSelector[EDeviceBaslerUSBLineSelector_Count] =
{
	_T("Line1"),
	_T("Line2"),
	_T("Line3"),
	_T("Line4"),
};

static LPCTSTR g_lpszBaslerUSBLineMode[EDeviceBaslerUSBLineMode_Count] =
{
	_T("Input"),
	_T("Output"),
};

static LPCTSTR g_lpszBaslerUSBLineSource[EDeviceBaslerUSBLineSource_Count] =
{
	_T("ExposureActive"),
	_T("FrameTriggerWait"),
	_T("FrameBurstTriggerWait"),
	_T("Timer1Active"),
	_T("UserOutput0"),
	_T("UserOutput1"),
	_T("UserOutput2"),
	_T("UserOutput3"),
	_T("FlashWindow"),
};

static LPCTSTR g_lpszBaslerUSBSwitch[EDeviceBaslerUSBSwitch_Count] =
{
	_T("Off"),
	_T("On"),
};


CDeviceBaslerUSB::CDeviceBaslerUSB()
{
}


CDeviceBaslerUSB::~CDeviceBaslerUSB()
{
	Terminate();
}

EDeviceInitializeResult CDeviceBaslerUSB::Initialize()
{
	EDeviceInitializeResult eReturn = EDeviceInitializeResult_UnknownError;

	CString strMessage, strStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("Basler"));

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

		if(PylonInitialize() != S_OK)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntopenthelibrary);
			eReturn = EDeviceInitializeResult_AlreadyInitializedError;
			break;
		}

		size_t numDevices = 0;

		if(PylonEnumerateDevices((size_t*)&numDevices) != S_OK)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Module info"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		if(!numDevices)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtofindthedevice);
			eReturn = EDeviceInitializeResult_NotFoundDeviceError;
			break;
		}

		PylonDeviceInfo_t pylonDeviceInfo;

		CString strSerialNumber = GetDeviceID();

		for(size_t i = 0; i < numDevices; ++i)
		{
			if(PylonGetDeviceInfo(i, &pylonDeviceInfo) != S_OK)
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Device info"));
				eReturn = EDeviceInitializeResult_NotFoundDeviceInfo;
				break;
			}

			if(strSerialNumber.CompareNoCase(CString(pylonDeviceInfo.SerialNumber)))
				continue;

			if(PylonCreateDeviceByIndex(i, &m_hDevice) != S_OK)
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Device index"));
				eReturn = EDeviceInitializeResult_DeviceHandleFailedGenerate;
				break;
			}

			break;
		}

		if(!m_hDevice)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtogeneratethedevicehandle);
			eReturn = EDeviceInitializeResult_DeviceHandleFailedGenerate;
			break;
		}

		if(PylonDeviceOpen(m_hDevice, PYLONC_ACCESS_MODE_CONTROL | PYLONC_ACCESS_MODE_STREAM) != S_OK)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntopenthedevice);
			eReturn = EDeviceInitializeResult_NotOpenedDeviceError;
			break;
		}

		int nTime = 0;

		if(GetGrabWaitingTime(&nTime))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Waiting Time"));
			eReturn = EDeviceInitializeResult_CanNotReadDBWaitingTime;
			break;
		}

		m_nWaitTime = nTime;

		if(!m_nWaitTime)
		{
			if(SetGrabWaitingTime(INT_MAX))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Waiting Time"));
				eReturn = EDeviceInitializeResult_CanNotWriteDBWaitingTime;
				break;
			}
		}

		int nLength = (int)strlen(pylonDeviceInfo.ModelName);

		if(pylonDeviceInfo.ModelName[nLength - 1] == 'm' || pylonDeviceInfo.ModelName[nLength - 1] == 'M')
			m_bColorCamera = false;
		else
			m_bColorCamera = true;
		
		EDeviceBaslerUSBPixelFormat ePixelFormat = EDeviceBaslerUSBPixelFormat_Count;

		if(GetPixelFormat(&ePixelFormat))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("PixelFormat"));
			eReturn = EDeviceInitializeResult_CanNotReadDBPixelFormat;
			break;
		}

		if(PylonDeviceFeatureIsWritable(m_hDevice, "PixelFormat"))
		{
			if(PylonDeviceFeatureFromString(m_hDevice, "PixelFormat", CStringA(g_lpszBaslerUSBPixelFormat[ePixelFormat])) != S_OK)
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("PixelFormat"));
				eReturn = EDeviceInitializeResult_CanNotApplyPixelFormat;
				break;
			}
		}

		if(PylonDeviceFeatureIsWritable(m_hDevice, "OffsetX"))
		{
			if(!SetCommandInt32("OffsetX", 0))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("OffsetX"));
				eReturn = EDeviceInitializeResult_CanNotApplyOffsetX;
				break;
			}
		}

		if(PylonDeviceFeatureIsWritable(m_hDevice, "OffsetY"))
		{
			if(!SetCommandInt32("OffsetY", 0))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("OffsetY"));
				eReturn = EDeviceInitializeResult_CanNotApplyOffsetX;
				break;
			}
		}

		int nWidth = 0;

		if(PylonDeviceFeatureIsWritable(m_hDevice, "Width"))
		{
			if(GetCanvasWidth(&nWidth))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("CanvasWidth"));
				eReturn = EDeviceInitializeResult_CanNotReadDBCanvasWidth;
				break;
			}

			bool bError = false;

			if(!SetCommandInt32("Width", nWidth))
			{
				bError = true;

				if(PylonDeviceFeatureIsReadable(m_hDevice, "WidthMax"))
				{
					if(GetCommandInt32("WidthMax", &nWidth))
					{
						if(SetCommandInt32("Width", nWidth))
						{
							if(!SetCanvasWidth(nWidth))
								bError = false;
						}						
					}
				}
			}

			if(bError)
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Width"));
				eReturn = EDeviceInitializeResult_CanNotApplyWidth;
				break;
			}
		}

		int nHeight = 0;

		if(PylonDeviceFeatureIsWritable(m_hDevice, "Height"))
		{
			if(GetCanvasHeight(&nHeight))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("CanvasHeight"));
				eReturn = EDeviceInitializeResult_CanNotReadDBCanvasHeight;
				break;
			}

			bool bError = false;

			if(!SetCommandInt32("Height", nHeight))
			{
				bError = true;

				if(PylonDeviceFeatureIsReadable(m_hDevice, "HeightMax"))
				{
					if(GetCommandInt32("HeightMax", &nHeight))
					{
						if(SetCommandInt32("Height", nHeight))
						{
							if(!SetCanvasHeight(nHeight))
								bError = false;
						}						
					}
				}
			}

			if(bError)
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Height"));
				eReturn = EDeviceInitializeResult_CanNotApplyWidth;
				break;
			}
		}

		if(PylonDeviceFeatureIsWritable(m_hDevice, "CenterX"))
		{
			bool bCenter = false;

			if(GetOffsetCenterX(&bCenter))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Offset Center X"));
				eReturn = EDeviceInitializeResult_CanNotReadDBOffsetCenterX;
				break;
			}

			if(PylonDeviceSetBooleanFeature(m_hDevice, "CenterX", bCenter))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Offset Center X"));
				eReturn = EDeviceInitializeResult_CanNotApplyCenterX;
				break;
			}
		}

		if(PylonDeviceFeatureIsWritable(m_hDevice, "OffsetX"))
		{
			int nParam = 0;

			if(GetOffsetX(&nParam))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Offset X"));
				eReturn = EDeviceInitializeResult_CanNotReadDBOffsetX;
				break;
			}

			bool bError = false;

			if(!SetCommandInt32("OffsetX", nParam))
			{
				bError = true;

				if(PylonDeviceFeatureIsReadable(m_hDevice, "OffsetX"))
				{
					if(GetCommandInt32("OffsetX", &nParam))
					{
						if(!SetOffsetX(nParam))
							bError = false;
					}
				}
			}

			if(bError)
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Offset X"));
				eReturn = EDeviceInitializeResult_CanNotApplyWidth;
				break;
			}
		}
		if(PylonDeviceFeatureIsWritable(m_hDevice, "CenterY"))
		{
			bool bCenter = false;

			if(GetOffsetCenterY(&bCenter))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Offset Center Y"));
				eReturn = EDeviceInitializeResult_CanNotReadDBOffsetCenterY;
				break;
			}

			if(PylonDeviceSetBooleanFeature(m_hDevice, "CenterY", bCenter))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Offset Center Y"));
				eReturn = EDeviceInitializeResult_CanNotApplyCenterY;
				break;
			}
		}

		if(PylonDeviceFeatureIsWritable(m_hDevice, "OffsetY"))
		{
			int nParam = 0;
			if(GetOffsetY(&nParam))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Offset Y"));
				eReturn = EDeviceInitializeResult_CanNotReadDBOffsetY;
				break;
			}

			bool bError = false;

			if(!SetCommandInt32("OffsetY", nParam))
			{
				bError = true;

				if(PylonDeviceFeatureIsReadable(m_hDevice, "OffsetY"))
				{
					if(GetCommandInt32("OffsetY", &nParam))
					{
						if(!SetOffsetY(nParam))
							bError = false;
					}
				}
			}

			if(bError)
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Offset Y"));
				eReturn = EDeviceInitializeResult_CanNotApplyWidth;
				break;
			}
		}

		if(PylonDeviceFeatureIsWritable(m_hDevice, "ReverseX"))
		{
			bool bReverse = false;

			if(GetReverseX(&bReverse))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Reverse X"));
				strMessage.Format(_T("Couldn't read 'Reverse X' from the database"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(PylonDeviceSetBooleanFeature(m_hDevice, "ReverseX", bReverse) != S_OK)
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Reverse X"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}
		}

		if(PylonDeviceFeatureIsWritable(m_hDevice, "ReverseY"))
		{
			bool bReverse = false;

			if(GetReverseY(&bReverse))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Reverse Y"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(PylonDeviceSetBooleanFeature(m_hDevice, "ReverseY", bReverse) != S_OK)
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Reverse Y"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}
		}
			

		if(PylonDeviceFeatureIsWritable(m_hDevice, "ExposureAuto"))
		{
			EDeviceBaslerUSBExposureAuto eExposureAuto = EDeviceBaslerUSBExposureAuto_Count;

			if(GetExposureAuto(&eExposureAuto))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ExposureAuto"));
				eReturn = EDeviceInitializeResult_CanNotReadDBExposureAuto;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "ExposureAuto", CStringA(g_lpszBaslerUSBExposureAuto[eExposureAuto])) != S_OK)
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ExposureAuto"));
				eReturn = EDeviceInitializeResult_CanNotApplyExposureAuto;
				break;
			}
		}

		if(PylonDeviceFeatureIsWritable(m_hDevice, "ExposureTime"))
		{
			double dblExposure = 0;

			if(GetExposureTime(&dblExposure))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ExposureTime"));
				eReturn = EDeviceInitializeResult_CanNotReadDBExposure;
				break;
			}

			if(PylonDeviceSetFloatFeature(m_hDevice, "ExposureTime", dblExposure) != S_OK)
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ExposureTime"));
				eReturn = EDeviceInitializeResult_CanNotApplyExposureTimeRaw;
				break;
			}
		}

		if(PylonDeviceFeatureIsWritable(m_hDevice, "GainAuto"))
		{
			EDeviceBaslerUSBGainAuto eGainAuto = EDeviceBaslerUSBGainAuto_Count;

			if(GetGainAuto(&eGainAuto))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("GainAuto"));
				eReturn = EDeviceInitializeResult_CanNotReadDBGainAuto;
				break;
			}

			if(eGainAuto == EDeviceBaslerUSBGainAuto_Count)
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("GainAuto"));
				eReturn = EDeviceInitializeResult_CanNotReadDBGainAuto;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "GainAuto", CStringA(g_lpszBaslerUSBGainAuto[eGainAuto])) != S_OK)
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("GainAuto"));
				eReturn = EDeviceInitializeResult_CanNotApplyGainAuto;
				break;
			}
		}

		if(PylonDeviceFeatureIsWritable(m_hDevice, "GainSelector"))
		{
			if(PylonDeviceFeatureFromString(m_hDevice, "GainSelector", "All") == S_OK)
			{
				double dblGain = 0.;

				if(GetAllGain(&dblGain))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AllGain"));
					eReturn = EDeviceInitializeResult_CanNotReadDBAllGain;
					break;
				}
				
				if(PylonDeviceSetFloatFeature(m_hDevice, "Gain", dblGain) != S_OK)
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("AllGain"));
					eReturn = EDeviceInitializeResult_CanNotApplyAllGainRaw;
					break;
				}
			}
		}

		if(PylonDeviceFeatureIsWritable(m_hDevice, "TriggerSelector"))
		{
			if(PylonDeviceFeatureFromString(m_hDevice, "TriggerSelector", "FrameStart") == S_OK)
			{
				EDeviceBaslerUSBTriggerMode eFrameTrigMode = EDeviceBaslerUSBTriggerMode_Count;

				if(GetFrameStartTriggerMode(&eFrameTrigMode))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("FrameStartMode"));
					eReturn = EDeviceInitializeResult_CanNotReadDBFrameStartMode;
					break;
				}

				if(PylonDeviceFeatureIsWritable(m_hDevice, "TriggerMode"))
				{
					if(PylonDeviceFeatureFromString(m_hDevice, "TriggerMode", CStringA(g_lpszBaslerUSBTriggerMode[eFrameTrigMode])) != S_OK)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("FrameStartMode"));
						eReturn = EDeviceInitializeResult_CanNotApplyFrameStartTriggerMode;
						break;
					}
				}

				if(PylonDeviceFeatureIsWritable(m_hDevice, "TriggerSource"))
				{
					EDeviceBaslerUSBTriggerSource eFrameTrigSource = EDeviceBaslerUSBTriggerSource_Count;

					if(GetFrameStartTriggerSource(&eFrameTrigSource))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("FrameStartTriggerSource"));
						eReturn = EDeviceInitializeResult_CanNotReadDBFrameStartTriggerSource;
						break;
					}

					if(PylonDeviceFeatureFromString(m_hDevice, "TriggerSource", CStringA(g_lpszBaslerUSBTriggerSource[eFrameTrigSource])) != S_OK)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("FrameStartTriggerSource"));
						eReturn = EDeviceInitializeResult_CanNotApplyTriggerSource;
						break;
					}

					if(PylonDeviceFeatureIsWritable(m_hDevice, "TriggerActivation"))
					{
						EDeviceBaslerUSBTriggerActivation eFrameTrigActivation = EDeviceBaslerUSBTriggerActivation_Count;

						if(GetFrameStartTriggerActivation(&eFrameTrigActivation))
						{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("FrameStartTriggerActivation"));
							eReturn = EDeviceInitializeResult_CanNotReadDBFrameStartTriggerSource;
							break;
						}

						if(PylonDeviceFeatureFromString(m_hDevice, "TriggerActivation", CStringA(g_lpszBaslerUSBTriggerActivation[eFrameTrigActivation])) != S_OK)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("FrameStartTriggerActivation"));
							eReturn = EDeviceInitializeResult_CanNotApplyTriggerSource;
							break;
						}
					}
				}
			}		

			if(PylonDeviceFeatureFromString(m_hDevice, "TriggerSelector", "FrameBurstStart") == S_OK)
			{
				EDeviceBaslerUSBTriggerMode eTrigMode = EDeviceBaslerUSBTriggerMode_Count;

				if(GetFrameBurstStartTriggerMode(&eTrigMode))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("FrameBurstStartMode"));
					eReturn = EDeviceInitializeResult_CanNotReadDBFrameStartMode;
					break;
				}

				if(PylonDeviceFeatureIsWritable(m_hDevice, "TriggerMode"))
				{
					if(PylonDeviceFeatureFromString(m_hDevice, "TriggerMode", CStringA(g_lpszBaslerUSBTriggerMode[eTrigMode])) != S_OK)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("FrameBurstStartMode"));
						eReturn = EDeviceInitializeResult_CanNotApplyFrameStartTriggerMode;
						break;
					}
				}

				if(PylonDeviceFeatureIsWritable(m_hDevice, "TriggerSource"))
				{
					EDeviceBaslerUSBTriggerSource eTrigSource = EDeviceBaslerUSBTriggerSource_Count;

					if(GetFrameStartTriggerSource(&eTrigSource))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("FrameStartTriggerSource"));
						eReturn = EDeviceInitializeResult_CanNotReadDBFrameStartTriggerSource;
						break;
					}

					if(PylonDeviceFeatureFromString(m_hDevice, "TriggerSource", CStringA(g_lpszBaslerUSBTriggerSource[eTrigSource])) != S_OK)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("FrameStartTriggerSource"));
						eReturn = EDeviceInitializeResult_CanNotApplyTriggerSource;
						break;
					}

					if(PylonDeviceFeatureIsWritable(m_hDevice, "TriggerActivation"))
					{
						EDeviceBaslerUSBTriggerActivation eTrigActivation = EDeviceBaslerUSBTriggerActivation_Count;

						if(GetFrameStartTriggerActivation(&eTrigActivation))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("FrameStartTriggerActivation"));
							eReturn = EDeviceInitializeResult_CanNotReadDBFrameStartTriggerSource;
							break;
						}

						if(PylonDeviceFeatureFromString(m_hDevice, "TriggerActivation", CStringA(g_lpszBaslerUSBTriggerActivation[eTrigActivation])) != S_OK)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("FrameStartTriggerActivation"));
							eReturn = EDeviceInitializeResult_CanNotApplyTriggerSource;
							break;
						}
					}
				}
			}

			EDeviceBaslerUSBTriggerSelector eTriggerSelector = EDeviceBaslerUSBTriggerSelector_Count;

			if(GetTriggerSelector(&eTriggerSelector))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TriggerSelector"));
				eReturn = EDeviceInitializeResult_CanNotReadDBTriggerSelector;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "TriggerSelector", CStringA(g_lpszBaslerUSBTriggerSelector[eTriggerSelector])) != S_OK)
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TriggerSelector"));
				eReturn = EDeviceInitializeResult_CanNotApplyTriggerSelector;
				break;
			}
		}
		
		if(PylonDeviceFeatureIsWritable(m_hDevice, "BlackLevelSelector"))
		{
			EDeviceBaslerUSBBlackLevelSelector eBlackLevelSelector = EDeviceBaslerUSBBlackLevelSelector_Count;

			if(GetBlackLevelSelector(&eBlackLevelSelector))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("BlackLevelSelector"));
				eReturn = EDeviceInitializeResult_CanNotReadDBTriggerSelector;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "BlackLevelSelector", CStringA(g_lpszBaslerUSBBlackLevelSelector[eBlackLevelSelector])) != S_OK)
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("BlackLevelSelector"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}

			if(PylonDeviceFeatureIsWritable(m_hDevice, "BlackLevel"))
			{
				double dblBlackLevel = 0.;

				if(GetBlackLevel(&dblBlackLevel))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("BlackLevel"));
					eReturn = EDeviceInitializeResult_CanNotReadDBTriggerSelector;
					break;
				}

				if(PylonDeviceSetFloatFeature(m_hDevice, "BlackLevel", dblBlackLevel) != S_OK)
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("BlackLevel"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}
			}
		}
		
		if(PylonDeviceFeatureIsWritable(m_hDevice, "Gamma"))
		{
			double dblGamma = 0.;

			if(GetGamma(&dblGamma))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Gamma"));
				eReturn = EDeviceInitializeResult_CanNotReadDBTriggerSelector;
				break;
			}

			if(PylonDeviceSetFloatFeature(m_hDevice, "Gamma", dblGamma) != S_OK)
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Gamma"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}
		}
		
		if(PylonDeviceFeatureIsWritable(m_hDevice, "LineSelector"))
		{
			EDeviceBaslerUSBLineSelector eLineSelector = EDeviceBaslerUSBLineSelector_Count;

			if(GetLineSelector(&eLineSelector))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineSelector"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "LineSelector", CStringA(g_lpszBaslerUSBLineSelector[EDeviceBaslerUSBLineSelector_Line1])) == S_OK)
			{
				EDeviceBaslerUSBLineMode eLineMode = EDeviceBaslerUSBLineMode_Count;

				if(GetLine1Mode(&eLineMode))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line1Mode"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(PylonDeviceFeatureFromString(m_hDevice, "LineMode", CStringA(g_lpszBaslerUSBLineMode[eLineMode])) == S_OK)
				{
					bool bInverter = false;

					if(GetLine1Inverter(&bInverter))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line1Inverter"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					switch(eLineMode)
					{
					case EDeviceBaslerUSBLineMode_Input:
						{
							double dblDebouncerTime = 0.;

							if(GetLine1DebouncerTime(&dblDebouncerTime))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line1DebouncerTime"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							if(PylonDeviceSetFloatFeature(m_hDevice, "LineDebouncerTime", dblDebouncerTime) != S_OK)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineDebouncerTime"));
								eReturn = EDeviceInitializeResult_WriteToDeviceError;
								break;
							}
						}
						break;
					case EDeviceBaslerUSBLineMode_Output:
						{
							EDeviceBaslerUSBLineSource eSource = EDeviceBaslerUSBLineSource_Count;

							if(GetLine1Source(&eSource))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line1Source"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							if(PylonDeviceFeatureFromString(m_hDevice, "LineSource", CStringA(g_lpszBaslerUSBLineSource[eSource])) != S_OK)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineSource"));
								eReturn = EDeviceInitializeResult_WriteToDeviceError;
								break;
							}

							double dblMinPulseWidth = 0.;

							if(GetLine1MinOutputPulseWidth(&dblMinPulseWidth))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line1MinOutputPulseWidth"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							if(PylonDeviceSetFloatFeature(m_hDevice, "LineMinimumOutputPulseWidth", dblMinPulseWidth) != S_OK)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Line1MinOutputPulseWidth"));
								eReturn = EDeviceInitializeResult_WriteToDeviceError;
								break;
							}
						}
						break;
					}
				}
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "LineSelector", CStringA(g_lpszBaslerUSBLineSelector[EDeviceBaslerUSBLineSelector_Line2])) == S_OK)
			{
				EDeviceBaslerUSBLineMode eLineMode = EDeviceBaslerUSBLineMode_Count;

				if(GetLine2Mode(&eLineMode))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line2Mode"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(PylonDeviceFeatureFromString(m_hDevice, "LineMode", CStringA(g_lpszBaslerUSBLineMode[eLineMode])) == S_OK)
				{
					bool bInverter = false;

					if(GetLine2Inverter(&bInverter))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line2Inverter"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					switch(eLineMode)
					{
					case EDeviceBaslerUSBLineMode_Input:
						{
							double dblDebouncerTime = 0.;

							if(GetLine2DebouncerTime(&dblDebouncerTime))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line2DebouncerTime"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							if(PylonDeviceSetFloatFeature(m_hDevice, "LineDebouncerTime", dblDebouncerTime) != S_OK)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Line2DebouncerTime"));
								eReturn = EDeviceInitializeResult_WriteToDeviceError;
								break;
							}
						}
						break;
					case EDeviceBaslerUSBLineMode_Output:
						{
							EDeviceBaslerUSBLineSource eSource = EDeviceBaslerUSBLineSource_Count;

							if(GetLine2Source(&eSource))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line2Source"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							if(PylonDeviceFeatureFromString(m_hDevice, "LineSource", CStringA(g_lpszBaslerUSBLineSource[eSource])) != S_OK)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Line2Source"));
								eReturn = EDeviceInitializeResult_WriteToDeviceError;
								break;
							}

							double dblMinPulseWidth = 0.;

							if(GetLine2MinOutputPulseWidth(&dblMinPulseWidth))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line2MinOutputPulseWidth"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							if(PylonDeviceSetFloatFeature(m_hDevice, "LineMinimumOutputPulseWidth", dblMinPulseWidth) != S_OK)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Line2MinOutputPulseWidth"));
								eReturn = EDeviceInitializeResult_WriteToDeviceError;
								break;
							}
						}
						break;
					}
				}
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "LineSelector", CStringA(g_lpszBaslerUSBLineSelector[EDeviceBaslerUSBLineSelector_Line3])) == S_OK)
			{
				EDeviceBaslerUSBLineMode eLineMode = EDeviceBaslerUSBLineMode_Count;

				if(GetLine3Mode(&eLineMode))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line3Mode"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(PylonDeviceFeatureFromString(m_hDevice, "LineMode", CStringA(g_lpszBaslerUSBLineMode[eLineMode])) == S_OK)
				{
					bool bInverter = false;

					if(GetLine3Inverter(&bInverter))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line3Inverter"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					switch(eLineMode)
					{
					case EDeviceBaslerUSBLineMode_Input:
						{
							double dblDebouncerTime = 0.;

							if(GetLine3DebouncerTime(&dblDebouncerTime))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line3DebouncerTime"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							if(PylonDeviceSetFloatFeature(m_hDevice, "LineDebouncerTime", dblDebouncerTime) != S_OK)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Line3DebouncerTime"));
								eReturn = EDeviceInitializeResult_WriteToDeviceError;
								break;
							}
						}
						break;
					case EDeviceBaslerUSBLineMode_Output:
						{
							EDeviceBaslerUSBLineSource eSource = EDeviceBaslerUSBLineSource_Count;

							if(GetLine3Source(&eSource))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line3Source"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							if(PylonDeviceFeatureFromString(m_hDevice, "LineSource", CStringA(g_lpszBaslerUSBLineSource[eSource])) != S_OK)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Line3Source"));
								eReturn = EDeviceInitializeResult_WriteToDeviceError;
								break;
							}

							double dblMinPulseWidth = 0.;

							if(GetLine3MinOutputPulseWidth(&dblMinPulseWidth))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line3MinOutputPulseWidth"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							if(PylonDeviceSetFloatFeature(m_hDevice, "LineMinimumOutputPulseWidth", dblMinPulseWidth) != S_OK)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Line3MinOutputPulseWidth"));
								eReturn = EDeviceInitializeResult_WriteToDeviceError;
								break;
							}
						}
						break;
					}
				}
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "LineSelector", CStringA(g_lpszBaslerUSBLineSelector[EDeviceBaslerUSBLineSelector_Line4])) == S_OK)
			{
				EDeviceBaslerUSBLineMode eLineMode = EDeviceBaslerUSBLineMode_Count;

				if(GetLine4Mode(&eLineMode))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line4Mode"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(PylonDeviceFeatureFromString(m_hDevice, "LineMode", CStringA(g_lpszBaslerUSBLineMode[eLineMode])) == S_OK)
				{
					bool bInverter = false;

					if(GetLine4Inverter(&bInverter))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line4Inverter"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					switch(eLineMode)
					{
					case EDeviceBaslerUSBLineMode_Input:
						{
							double dblDebouncerTime = 0.;

							if(GetLine4DebouncerTime(&dblDebouncerTime))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line4DebouncerTime"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							if(PylonDeviceSetFloatFeature(m_hDevice, "LineDebouncerTime", dblDebouncerTime) != S_OK)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Line4DebouncerTime"));
								eReturn = EDeviceInitializeResult_WriteToDeviceError;
								break;
							}
						}
						break;
					case EDeviceBaslerUSBLineMode_Output:
						{
							EDeviceBaslerUSBLineSource eSource = EDeviceBaslerUSBLineSource_Count;

							if(GetLine4Source(&eSource))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line4Source"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							if(PylonDeviceFeatureFromString(m_hDevice, "LineSource", CStringA(g_lpszBaslerUSBLineSource[eSource])) != S_OK)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Line4Source"));
								eReturn = EDeviceInitializeResult_WriteToDeviceError;
								break;
							}

							double dblMinPulseWidth = 0.;

							if(GetLine4MinOutputPulseWidth(&dblMinPulseWidth))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line4MinOutputPulseWidth"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							if(PylonDeviceSetFloatFeature(m_hDevice, "LineMinimumOutputPulseWidth", dblMinPulseWidth) != S_OK)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Line4MinOutputPulseWidth"));
								eReturn = EDeviceInitializeResult_WriteToDeviceError;
								break;
							}
						}
						break;
					}
				}
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "LineSelector", CStringA(g_lpszBaslerUSBLineSelector[eLineSelector])) != S_OK)
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineSelector"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}
		}

		CString strFormat = g_lpszBaslerUSBPixelFormat[ePixelFormat];

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

		this->InitBuffer(nWidth, nHeight, mv, CRavidImage::MakeValueFormat(bColor ? 3 : 1, nBpp), nWidthStep, nAlignByte);
		this->ConnectImage();

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

EDeviceTerminateResult CDeviceBaslerUSB::Terminate()
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
		if(IsLive() || !IsGrabAvailable())
			Stop();

		m_bIsInitialized = false;
		m_bIsGrabAvailable = true;

		if(m_hDevice)
		{
			if(PylonDeviceClose(m_hDevice) != S_OK)
			{
				strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntclosethedevice);
				eReturn = EDeviceTerminateResult_NotClosedDeviceError;
				break;
			}

			if(PylonDestroyDevice(m_hDevice) != S_OK)
			{
				strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtofreethedevice);
				eReturn = EDeviceTerminateResult_NotDestroyedDeviceError;
				break;
			}

			PylonTerminate();

			m_hDevice = nullptr;
		}

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

bool CDeviceBaslerUSB::LoadSettings()
{
	bool bReturn = false;

	m_vctParameterFieldConfigurations.clear();

	do
	{
		CString strTime;
		strTime.Format(_T("%d"), INT_MAX);
		
		AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_DeviceID, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_DeviceID], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 0);

		AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_CameraSetting, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_CameraSetting], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		{
			AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_GrabCount, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_GrabCount], _T("1"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_CanvasWidth, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_CanvasWidth], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_CanvasHeight, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_CanvasHeight], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_OffsetCenterX, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_OffsetCenterX], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_OffsetCenterY, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_OffsetCenterY], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_OffsetX, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_OffsetX], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_OffsetY, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_OffsetY], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_ReverseX, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_ReverseX], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_ReverseY, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_ReverseY], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_GrabWaitingTime, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_GrabWaitingTime], strTime, EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_PixelFormat, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_PixelFormat], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerUSBPixelFormat, EDeviceBaslerUSBPixelFormat_Count), nullptr, 1);
		}

		AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_AnalogControl, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_AnalogControl], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		{
			AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_GainAuto, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_GainAuto], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerUSBGainAuto, EDeviceBaslerUSBGainAuto_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_GainSelector, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_GainSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerUSBGainSelector, EDeviceBaslerUSBGainSelector_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_Gain, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_Gain], _T("0"), EParameterFieldType_Edit, nullptr, _T("double"), 1);

			AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_BlackLevelSelector, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_BlackLevelSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerUSBBlackLevelSelector, EDeviceBaslerUSBBlackLevelSelector_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_BlackLevel, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_BlackLevel], _T("1.0"), EParameterFieldType_Edit, nullptr, _T("double"), 1);

			AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_Gamma, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_Gamma], _T("1.0"), EParameterFieldType_Edit, nullptr, _T("double"), 1);
		}

		AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_AcquisitionControl, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_AcquisitionControl], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		{
			AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_ShutterMode, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_ShutterMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerUSBShutterMode, EDeviceBaslerUSBShutterMode_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_ExposureAuto, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_ExposureAuto], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerUSBExposureAuto, EDeviceBaslerUSBExposureAuto_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_ExposureMode, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_ExposureMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerUSBExposureMode, EDeviceBaslerUSBExposureMode_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_ExposureTime, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_ExposureTime], _T("5000.0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_TriggerSelector, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_TriggerSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerUSBTriggerSelector, EDeviceBaslerUSBTriggerSelector_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_FrameStart, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_FrameStart], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
			{
				AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_FrameStartTriggerMode, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_FrameStartTriggerMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerUSBTriggerMode, EDeviceBaslerUSBTriggerMode_Count), nullptr, 2);

				AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_FrameStartTriggerSource, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_FrameStartTriggerSource], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerUSBTriggerSource, EDeviceBaslerUSBTriggerSource_Count), nullptr, 2);

				AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_FrameStartTriggerActivation, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_FrameStartTriggerActivation], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerUSBTriggerActivation, EDeviceBaslerUSBTriggerActivation_Count), nullptr, 2);

				AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_FrameStartTriggerDelay, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_FrameStartTriggerDelay], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
			}

			AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_FrameBurstStart, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_FrameBurstStart], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
			{
				AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_FrameBurstStartTriggerMode, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_FrameBurstStartTriggerMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerUSBTriggerMode, EDeviceBaslerUSBTriggerMode_Count), nullptr, 2);

				AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_FrameBurstStartTriggerSource, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_FrameBurstStartTriggerSource], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerUSBTriggerSource, EDeviceBaslerUSBTriggerSource_Count), nullptr, 2);

				AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_FrameBurstStartTriggerActivation, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_FrameBurstStartTriggerActivation], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerUSBTriggerActivation, EDeviceBaslerUSBTriggerActivation_Count), nullptr, 2);

				AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_FrameBurstStartTriggerDelay, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_FrameBurstStartTriggerDelay], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
			}
			
			AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_EnableAcquisitionFrameRate, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_EnableAcquisitionFrameRate], _T("0"), EParameterFieldType_Check, nullptr, _T("double"), 1);

			AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_AcquisitionFrameRate, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_AcquisitionFrameRate], _T("200.0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_AcquisitionStatusSelector, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_AcquisitionStatusSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerUSBAcquisitionStatusSelector, EDeviceBaslerUSBAcquisitionStatusSelector_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_DigitalIOConrol, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_DigitalIOConrol], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
			{
				AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_LineSelector, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_LineSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerUSBLineSelector, EDeviceBaslerUSBLineSelector_Count), nullptr, 1);

				AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_Line1, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_Line1], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
				{
					AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_Line1Mode, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_Line1Mode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerUSBLineMode, EDeviceBaslerUSBLineMode_Count), nullptr, 2);

					AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_Line1Source, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_Line1Source], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerUSBLineSource, EDeviceBaslerUSBLineSource_Count), nullptr, 2);

					AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_Line1Inverter, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_Line1Inverter], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 2);

					AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_Line1DebouncerTime_us, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_Line1DebouncerTime_us], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);

					AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_Line1MinimumOuputPulseWidth_us, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_Line1MinimumOuputPulseWidth_us], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
				}

				AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_Line2, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_Line2], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
				{
					AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_Line2Mode, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_Line2Mode], _T("1"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerUSBLineMode, EDeviceBaslerUSBLineMode_Count), nullptr, 2);

					AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_Line2Source, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_Line2Source], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerUSBLineSource, EDeviceBaslerUSBLineSource_Count), nullptr, 2);

					AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_Line2Inverter, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_Line2Inverter], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 2);

					AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_Line2DebouncerTime_us, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_Line2DebouncerTime_us], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);

					AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_Line2MinimumOuputPulseWidth_us, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_Line2MinimumOuputPulseWidth_us], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
				}

				AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_Line3, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_Line3], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
				{
					AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_Line3Mode, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_Line3Mode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerUSBLineMode, EDeviceBaslerUSBLineMode_Count), nullptr, 2);

					AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_Line3Source, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_Line3Source], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerUSBLineSource, EDeviceBaslerUSBLineSource_Count), nullptr, 2);

					AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_Line3Inverter, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_Line3Inverter], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 2);

					AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_Line3DebouncerTime_us, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_Line3DebouncerTime_us], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);

					AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_Line3MinimumOuputPulseWidth_us, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_Line3MinimumOuputPulseWidth_us], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
				}

				AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_Line4, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_Line4], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
				{
					AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_Line4Mode, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_Line4Mode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerUSBLineMode, EDeviceBaslerUSBLineMode_Count), nullptr, 2);

					AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_Line4Source, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_Line4Source], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerUSBLineSource, EDeviceBaslerUSBLineSource_Count), nullptr, 2);

					AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_Line4Inverter, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_Line4Inverter], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 2);

					AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_Line4DebouncerTime_us, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_Line4DebouncerTime_us], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);

					AddParameterFieldConfigurations(EDeviceParameterBaslerUSB_Line4MinimumOuputPulseWidth_us, g_lpszParamBaslerUSB[EDeviceParameterBaslerUSB_Line4MinimumOuputPulseWidth_us], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
				}
			}
		}

		bReturn = true;
	}
	while(false);

	return bReturn & __super::LoadSettings();
}

EDeviceGrabResult CDeviceBaslerUSB::Grab()
{
	EDeviceGrabResult eReturn = EDeviceGrabResult_UnknownError;

	CString strMessage;

	do
	{
		m_nGrabCount = 0;

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

		int nGrabCount = 0;

		if(GetGrabCount(&nGrabCount))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Grab count"));
			eReturn = EDeviceGrabResult_ReadOnDatabaseError;
			break;
		}

		if(!nGrabCount)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Grab count"));
			eReturn = EDeviceGrabResult_ReadOnDeviceError;
			break;
		}

		m_bIsGrabAvailable = false;

		m_pLiveThread = AfxBeginThread(CDeviceBaslerUSB::CallbackFunction, this, 0, 0U, CREATE_SUSPENDED);

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

EDeviceLiveResult CDeviceBaslerUSB::Live()
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

		m_pLiveThread = AfxBeginThread(CDeviceBaslerUSB::CallbackFunction, this, 0, 0U, CREATE_SUSPENDED);

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

EDeviceStopResult CDeviceBaslerUSB::Stop()
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

		m_bIsLive = false;

		if(WaitForSingleObject(m_pLiveThread->m_hThread, 1000) == WAIT_TIMEOUT)
		{
			if(PylonDeviceClose(m_hDevice) != S_OK)
			{
				strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntclosethedevice);
				eReturn = EDeviceStopResult_WriteToDeviceError;
				break;
			}

			if(PylonDeviceOpen(m_hDevice, PYLONC_ACCESS_MODE_CONTROL | PYLONC_ACCESS_MODE_STREAM) != S_OK)
			{
				strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntopenthedevice);
				eReturn = EDeviceStopResult_WriteToDeviceError;
				break;
			}
		}

		m_bIsGrabAvailable = true;
		m_pLiveThread = nullptr;

		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("Stop"));

		eReturn = EDeviceStopResult_OK;
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceTriggerResult CDeviceBaslerUSB::Trigger()
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

		if(PylonDeviceExecuteCommandFeature(m_hDevice, "TriggerSoftware") != GENAPI_E_OK)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TriggerSoftware"));
			eReturn = EDeviceTriggerResult_WriteToDeviceError;
			break;
		}

		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("Trigger"));

		eReturn = EDeviceTriggerResult_OK;
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetGrabCount(int* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do 
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerUSB_GrabCount));

		eReturn = EBaslerGetFunction_OK;
	} 
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetGrabCount(int nParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_GrabCount;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do 
	{
		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	} 
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetCanvasWidth(int* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_CanvasWidth));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetCanvasWidth(int nParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_CanvasWidth;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EBaslerSetFunction_AlreadyInitializedError;
			break;
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetCanvasHeight(int* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_CanvasHeight));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetCanvasHeight(int nParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_CanvasHeight;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EBaslerSetFunction_AlreadyInitializedError;
			break;
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetOffsetCenterX(bool* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerUSB_OffsetCenterX));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetOffsetCenterX(bool bParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_OffsetCenterX;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "CenterX"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceSetBooleanFeature(m_hDevice, "CenterX", bParam) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}

			if(PylonDeviceFeatureIsReadable(m_hDevice, "OffsetX"))
			{
				int64_t nOffset = 0;

				if(PylonDeviceGetIntegerFeature(m_hDevice, "OffsetX", &nOffset) == S_OK)
					SetOffsetX(nOffset);
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], g_lpszBaslerUSBSwitch[nPreValue], g_lpszBaslerUSBSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetOffsetCenterY(bool* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerUSB_OffsetCenterY));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetOffsetCenterY(bool bParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_OffsetCenterY;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "CenterY"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceSetBooleanFeature(m_hDevice, "CenterY", bParam) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}

			if(PylonDeviceFeatureIsReadable(m_hDevice, "OffsetY"))
			{
				int64_t nOffset = 0;

				if(PylonDeviceGetIntegerFeature(m_hDevice, "OffsetY", &nOffset) == S_OK)
					SetOffsetY(nOffset);
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], g_lpszBaslerUSBSwitch[nPreValue], g_lpszBaslerUSBSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetOffsetX(int* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerUSB_OffsetX));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetOffsetX(int nParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_OffsetX;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "OffsetX"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceSetIntegerFeature(m_hDevice, "OffsetX", nParam) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDeviceError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDeviceError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetOffsetY(int* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerUSB_OffsetY));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetOffsetY(int nParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_OffsetY;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "OffsetY"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceSetIntegerFeature(m_hDevice, "OffsetY", nParam) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDeviceError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDeviceError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetReverseX(bool * pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerUSB_ReverseX));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetReverseX(bool bParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_ReverseX;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "ReverseX"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceSetBooleanFeature(m_hDevice, "ReverseX", bParam) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], g_lpszBaslerUSBSwitch[nPreValue], g_lpszBaslerUSBSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetReverseY(bool * pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerUSB_ReverseY));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetReverseY(bool bParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_ReverseY;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "ReverseY"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceSetBooleanFeature(m_hDevice, "ReverseY", bParam) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], g_lpszBaslerUSBSwitch[nPreValue], g_lpszBaslerUSBSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetGrabWaitingTime(int* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerUSB_GrabWaitingTime));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetGrabWaitingTime(int nParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_GrabWaitingTime;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsLive() || !IsGrabAvailable())
		{
			eReturn = EBaslerSetFunction_ActivatedDeviceError;
			break;
		}

		m_nWaitTime = nParam;

		CString strTime;
		strTime.Format(_T("%d"), nParam);

		if(!SetParamValue(EDeviceParameterBaslerUSB_GrabWaitingTime, strTime))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(EDeviceParameterBaslerUSB_GrabWaitingTime))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetPixelFormat(EDeviceBaslerUSBPixelFormat* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceBaslerUSBPixelFormat)_ttoi(GetParamValue(EDeviceParameterBaslerUSB_PixelFormat));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetPixelFormat(EDeviceBaslerUSBPixelFormat eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_PixelFormat;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam > EDeviceBaslerUSBPixelFormat_Count)
		{
			eReturn = EBaslerSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EBaslerSetFunction_AlreadyInitializedError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], g_lpszBaslerUSBPixelFormat[nPreValue], g_lpszBaslerUSBPixelFormat[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetGainAuto(EDeviceBaslerUSBGainAuto* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceBaslerUSBGainAuto)_ttoi(GetParamValue(EDeviceParameterBaslerUSB_GainAuto));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetGainAuto(EDeviceBaslerUSBGainAuto eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_GainAuto;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceBaslerUSBGainAuto_Count)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "GainAuto"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "GainAuto", CStringA(g_lpszBaslerUSBGainAuto[eParam])) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}

			if(eParam == EDeviceBaslerUSBGainAuto_Off)
			{
				if(PylonDeviceFeatureIsWritable(m_hDevice, "GainSelector"))
				{
					if(PylonDeviceFeatureFromString(m_hDevice, "GainSelector", "All") == S_OK)
					{
						double dblGain = 0.;

						if(GetAllGain(&dblGain))
						{
							eReturn = EBaslerSetFunction_ReadOnDatabaseError;
							break;
						}

						if(PylonDeviceSetFloatFeature(m_hDevice, "Gain", dblGain) != S_OK)
						{
							eReturn = EBaslerSetFunction_WriteToDeviceError;
							break;
						}
					}
				}
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDeviceError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], g_lpszBaslerUSBGainAuto[nPreValue], g_lpszBaslerUSBGainAuto[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetGainSelector(EDeviceBaslerUSBGainSelector* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceBaslerUSBGainSelector)_ttoi(GetParamValue(EDeviceParameterBaslerUSB_GainSelector));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetGainSelector(EDeviceBaslerUSBGainSelector eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_GainSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceBaslerUSBGainSelector_Count)
		{
			eReturn = EBaslerSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "GainSelector"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "GainSelector", CStringA(g_lpszBaslerUSBGainSelector[eParam])) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], g_lpszBaslerUSBGainSelector[nPreValue], g_lpszBaslerUSBGainSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetAllGain(double* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}
		
		*pParam = _ttof(GetParamValue(EDeviceParameterBaslerUSB_Gain));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetAllGain(double dblParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_Gain;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PylonDeviceFeatureFromString(m_hDevice, "GainSelector", "All") != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}

			if(PylonDeviceSetFloatFeature(m_hDevice, "Gain", dblParam) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetBlackLevelSelector(EDeviceBaslerUSBBlackLevelSelector* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceBaslerUSBBlackLevelSelector)_ttoi(GetParamValue(EDeviceParameterBaslerUSB_BlackLevelSelector));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetBlackLevelSelector(EDeviceBaslerUSBBlackLevelSelector eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_BlackLevelSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceBaslerUSBBlackLevelSelector_Count)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "BlackLevelSelector"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "BlackLevelSelector", CStringA(g_lpszBaslerUSBBlackLevelSelector[eParam])) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], g_lpszBaslerUSBBlackLevelSelector[nPreValue], g_lpszBaslerUSBBlackLevelSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetBlackLevel(double* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterBaslerUSB_BlackLevel));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetBlackLevel(double dblParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_BlackLevel;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "BlackLevelSelector"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "BlackLevelSelector", "All") != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;

				break;
			}

			if(PylonDeviceSetIntegerFeature(m_hDevice, "BlackLevel", dblParam) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetGamma(double* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterBaslerUSB_Gamma));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetGamma(double dblParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_Gamma;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "Gamma"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceSetIntegerFeature(m_hDevice, "Gamma", dblParam) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetShutterMode(EDeviceBaslerUSBShutterMode* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceBaslerUSBShutterMode)_ttoi(GetParamValue(EDeviceParameterBaslerUSB_ShutterMode));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetShutterMode(EDeviceBaslerUSBShutterMode eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_ShutterMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceBaslerUSBShutterMode_Count)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "ShutterMode"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "ShutterMode", CStringA(g_lpszBaslerUSBShutterMode[eParam])) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], g_lpszBaslerUSBShutterMode[nPreValue], g_lpszBaslerUSBShutterMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetExposureAuto(EDeviceBaslerUSBExposureAuto* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceBaslerUSBExposureAuto)_ttoi(GetParamValue(EDeviceParameterBaslerUSB_ExposureAuto));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetExposureAuto(EDeviceBaslerUSBExposureAuto eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_ExposureAuto;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceBaslerUSBExposureAuto_Count)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "ExposureAuto"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "ExposureAuto", CStringA(g_lpszBaslerUSBExposureAuto[eParam])) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], g_lpszBaslerUSBExposureAuto[nPreValue], g_lpszBaslerUSBExposureAuto[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetExposureMode(EDeviceBaslerUSBExposureMode* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceBaslerUSBExposureMode)_ttoi(GetParamValue(EDeviceParameterBaslerUSB_ExposureMode));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetExposureMode(EDeviceBaslerUSBExposureMode eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_ExposureMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceBaslerUSBExposureMode_Count)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "ExposureMode"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "ExposureMode", CStringA(g_lpszBaslerUSBExposureMode[eParam])) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], g_lpszBaslerUSBExposureMode[nPreValue], g_lpszBaslerUSBExposureMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetExposureTime(double* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterBaslerUSB_ExposureTime));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetExposureTime(double dblParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_ExposureTime;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "ExposureTime"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceSetFloatFeature(m_hDevice, "ExposureTime", dblParam) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetTriggerSelector(EDeviceBaslerUSBTriggerSelector* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceBaslerUSBTriggerSelector)_ttoi(GetParamValue(EDeviceParameterBaslerUSB_TriggerSelector));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetTriggerSelector(EDeviceBaslerUSBTriggerSelector eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_TriggerSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceBaslerUSBTriggerSelector_Count)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "TriggerSelector"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "TriggerSelector", CStringA(g_lpszBaslerUSBTriggerSelector[eParam])) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], g_lpszBaslerUSBTriggerSelector[nPreValue], g_lpszBaslerUSBTriggerSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetFrameStartTriggerMode(EDeviceBaslerUSBTriggerMode* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceBaslerUSBTriggerMode)_ttoi(GetParamValue(EDeviceParameterBaslerUSB_FrameStartTriggerMode));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetFrameStartTriggerMode(EDeviceBaslerUSBTriggerMode eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_FrameStartTriggerMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceBaslerUSBTriggerMode_Count)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(PylonDeviceFeatureIsWritable(m_hDevice, "TriggerSelector"))
			{
				if(PylonDeviceFeatureFromString(m_hDevice, "TriggerSelector", "FrameStart") != S_OK)
				{
					eReturn = EBaslerSetFunction_NotSupportError;
					break;
				}

				if(!PylonDeviceFeatureIsWritable(m_hDevice, "TriggerMode"))
				{
					eReturn = EBaslerSetFunction_NotSupportError;
					break;
				}

				if(PylonDeviceFeatureFromString(m_hDevice, "TriggerMode", CStringA(g_lpszBaslerUSBTriggerMode[eParam])) != S_OK)
				{
					eReturn = EBaslerSetFunction_WriteToDeviceError;
					break;
				}
			}
		}
		
		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], g_lpszBaslerUSBTriggerMode[nPreValue], g_lpszBaslerUSBTriggerMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetFrameStartTriggerSource(EDeviceBaslerUSBTriggerSource* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceBaslerUSBTriggerSource)_ttoi(GetParamValue(EDeviceParameterBaslerUSB_FrameStartTriggerSource));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetFrameStartTriggerSource(EDeviceBaslerUSBTriggerSource eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_FrameStartTriggerSource;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceBaslerUSBTriggerSource_Count)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(PylonDeviceFeatureIsWritable(m_hDevice, "TriggerSelector"))
			{
				if(PylonDeviceFeatureFromString(m_hDevice, "TriggerSelector", "FrameStart") != S_OK)
				{
					eReturn = EBaslerSetFunction_NotSupportError;
					break;
				}

				if(!PylonDeviceFeatureIsWritable(m_hDevice, "TriggerSource"))
				{
					eReturn = EBaslerSetFunction_NotSupportError;
					break;
				}

				if(PylonDeviceFeatureFromString(m_hDevice, "TriggerSource", CStringA(g_lpszBaslerUSBTriggerSource[eParam])) != S_OK)
				{
					eReturn = EBaslerSetFunction_WriteToDeviceError;
					break;
				}
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], g_lpszBaslerUSBTriggerSource[nPreValue], g_lpszBaslerUSBTriggerSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetFrameStartTriggerActivation(EDeviceBaslerUSBTriggerActivation* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceBaslerUSBTriggerActivation)_ttoi(GetParamValue(EDeviceParameterBaslerUSB_FrameStartTriggerActivation));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetFrameStartTriggerActivation(EDeviceBaslerUSBTriggerActivation eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_FrameStartTriggerActivation;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceBaslerUSBTriggerActivation_Count)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(PylonDeviceFeatureIsWritable(m_hDevice, "TriggerSelector"))
			{
				if(PylonDeviceFeatureFromString(m_hDevice, "TriggerSelector", "FrameStart") != S_OK)
				{
					eReturn = EBaslerSetFunction_NotSupportError;
					break;
				}

				if(!PylonDeviceFeatureIsWritable(m_hDevice, "TriggerActivation"))
				{
					eReturn = EBaslerSetFunction_NotSupportError;
					break;
				}

				if(PylonDeviceFeatureFromString(m_hDevice, "TriggerActivation", CStringA(g_lpszBaslerUSBTriggerActivation[eParam])) != S_OK)
				{
					eReturn = EBaslerSetFunction_WriteToDeviceError;
					break;
				}
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], g_lpszBaslerUSBTriggerActivation[nPreValue], g_lpszBaslerUSBTriggerActivation[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetFrameStartTriggerDelay(double* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterBaslerUSB_FrameStartTriggerDelay));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetFrameStartTriggerDelay(double dblParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_FrameStartTriggerDelay;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PylonDeviceFeatureIsWritable(m_hDevice, "TriggerSelector"))
			{
				if(PylonDeviceFeatureFromString(m_hDevice, "TriggerSelector", "FrameStart") != S_OK)
				{
					eReturn = EBaslerSetFunction_NotSupportError;
					break;
				}

				if(!PylonDeviceFeatureIsWritable(m_hDevice, "TriggerDelay"))
				{
					eReturn = EBaslerSetFunction_NotSupportError;
					break;
				}

				if(PylonDeviceSetFloatFeature(m_hDevice, "TriggerDelay", dblParam) != S_OK)
				{
					eReturn = EBaslerSetFunction_WriteToDeviceError;
					break;
				}
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetFrameBurstStartTriggerMode(EDeviceBaslerUSBTriggerMode* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceBaslerUSBTriggerMode)_ttoi(GetParamValue(EDeviceParameterBaslerUSB_FrameBurstStartTriggerMode));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetFrameBurstStartTriggerMode(EDeviceBaslerUSBTriggerMode eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_FrameBurstStartTriggerMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceBaslerUSBTriggerMode_Count)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(PylonDeviceFeatureIsWritable(m_hDevice, "TriggerSelector"))
			{
				if(PylonDeviceFeatureFromString(m_hDevice, "TriggerSelector", "FrameBurstStart") != S_OK)
				{
					eReturn = EBaslerSetFunction_NotSupportError;
					break;
				}

				if(!PylonDeviceFeatureIsWritable(m_hDevice, "TriggerMode"))
				{
					eReturn = EBaslerSetFunction_NotSupportError;
					break;
				}

				if(PylonDeviceFeatureFromString(m_hDevice, "TriggerMode", CStringA(g_lpszBaslerUSBTriggerMode[eParam])) != S_OK)
				{
					eReturn = EBaslerSetFunction_WriteToDeviceError;
					break;
				}
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], g_lpszBaslerUSBTriggerMode[nPreValue], g_lpszBaslerUSBTriggerMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetFrameBurstStartTriggerSource(EDeviceBaslerUSBTriggerSource* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceBaslerUSBTriggerSource)_ttoi(GetParamValue(EDeviceParameterBaslerUSB_FrameBurstStartTriggerSource));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetFrameBurstStartTriggerSource(EDeviceBaslerUSBTriggerSource eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_FrameBurstStartTriggerSource;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceBaslerUSBTriggerSource_Count)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(PylonDeviceFeatureIsWritable(m_hDevice, "TriggerSelector"))
			{
				if(PylonDeviceFeatureFromString(m_hDevice, "TriggerSelector", "FrameBurstStart") != S_OK)
				{
					eReturn = EBaslerSetFunction_NotSupportError;
					break;
				}

				if(!PylonDeviceFeatureIsWritable(m_hDevice, "TriggerSource"))
				{
					eReturn = EBaslerSetFunction_NotSupportError;
					break;
				}

				if(PylonDeviceFeatureFromString(m_hDevice, "TriggerSource", CStringA(g_lpszBaslerUSBTriggerSource[eParam])) != S_OK)
				{
					eReturn = EBaslerSetFunction_WriteToDeviceError;
					break;
				}
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], g_lpszBaslerUSBTriggerSource[nPreValue], g_lpszBaslerUSBTriggerSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetFrameBurstStartTriggerActivation(EDeviceBaslerUSBTriggerActivation* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceBaslerUSBTriggerActivation)_ttoi(GetParamValue(EDeviceParameterBaslerUSB_FrameBurstStartTriggerActivation));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetFrameBurstStartTriggerActivation(EDeviceBaslerUSBTriggerActivation eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_FrameBurstStartTriggerActivation;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceBaslerUSBTriggerActivation_Count)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(PylonDeviceFeatureIsWritable(m_hDevice, "TriggerSelector"))
			{
				if(PylonDeviceFeatureFromString(m_hDevice, "TriggerSelector", "FrameBurstStart") != S_OK)
				{
					eReturn = EBaslerSetFunction_NotSupportError;
					break;
				}

				if(!PylonDeviceFeatureIsWritable(m_hDevice, "TriggerActivation"))
				{
					eReturn = EBaslerSetFunction_NotSupportError;
					break;
				}

				if(PylonDeviceFeatureFromString(m_hDevice, "TriggerActivation", CStringA(g_lpszBaslerUSBTriggerActivation[eParam])) != S_OK)
				{
					eReturn = EBaslerSetFunction_WriteToDeviceError;
					break;
				}
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], g_lpszBaslerUSBTriggerActivation[nPreValue], g_lpszBaslerUSBTriggerActivation[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetFrameBurstStartTriggerDelay(double* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterBaslerUSB_FrameBurstStartTriggerDelay));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetFrameBurstStartTriggerDelay(double dblParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_FrameBurstStartTriggerDelay;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PylonDeviceFeatureIsWritable(m_hDevice, "TriggerSelector"))
			{
				if(PylonDeviceFeatureFromString(m_hDevice, "TriggerSelector", "FrameBurstStart") != S_OK)
				{
					eReturn = EBaslerSetFunction_NotSupportError;
					break;
				}

				if(!PylonDeviceFeatureIsWritable(m_hDevice, "TriggerDelay"))
				{
					eReturn = EBaslerSetFunction_NotSupportError;
					break;
				}

				if(PylonDeviceSetFloatFeature(m_hDevice, "TriggerDelay", dblParam) != S_OK)
				{
					eReturn = EBaslerSetFunction_WriteToDeviceError;
					break;
				}
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetEnableAcquisitionFrameRate(bool* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerUSB_EnableAcquisitionFrameRate));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetEnableAcquisitionFrameRate(bool bParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_EnableAcquisitionFrameRate;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "AcquisitionFrameRateEnable"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceSetBooleanFeature(m_hDevice, "AcquisitionFrameRateEnable", bParam) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], g_lpszBaslerUSBSwitch[nPreValue], g_lpszBaslerUSBSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetAcquisitionFrameRate(double* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterBaslerUSB_AcquisitionFrameRate));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetAcquisitionFrameRate(double dblParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_AcquisitionFrameRate;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "AcquisitionFrameRate"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceSetFloatFeature(m_hDevice, "AcquisitionFrameRate", dblParam) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetAcquisitionStatusSelector(EDeviceBaslerUSBAcquisitionStatusSelector* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceBaslerUSBAcquisitionStatusSelector)_ttoi(GetParamValue(EDeviceParameterBaslerUSB_AcquisitionStatusSelector));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetAcquisitionStatusSelector(EDeviceBaslerUSBAcquisitionStatusSelector eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_AcquisitionStatusSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceBaslerUSBAcquisitionStatusSelector_Count)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "AcquisitionStatusSelector"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "AcquisitionStatusSelector", CStringA(g_lpszBaslerUSBAcquisitionStatusSelector[eParam])) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], g_lpszBaslerUSBAcquisitionStatusSelector[nPreValue], g_lpszBaslerUSBAcquisitionStatusSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetLineSelector(EDeviceBaslerUSBLineSelector * pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceBaslerUSBLineSelector)_ttoi(GetParamValue(EDeviceParameterBaslerUSB_LineSelector));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetLineSelector(EDeviceBaslerUSBLineSelector eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_LineSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceBaslerUSBLineSelector_Count)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "LineSelector"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "LineSelector", CStringA(g_lpszBaslerUSBLineSelector[eParam])) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], g_lpszBaslerUSBLineSelector[nPreValue], g_lpszBaslerUSBLineSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetLine1Mode(EDeviceBaslerUSBLineMode * pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceBaslerUSBLineMode)_ttoi(GetParamValue(EDeviceParameterBaslerUSB_Line1Mode));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetLine1Mode(EDeviceBaslerUSBLineMode eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_Line1Mode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceBaslerUSBLineMode_Count)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		int nLineSelect = _ttoi(GetParamValue(EDeviceParameterBaslerUSB_LineSelector));

		if(nLineSelect != EDeviceBaslerUSBLineSelector_Line1)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "LineMode"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "LineMode", CStringA(g_lpszBaslerUSBLineMode[eParam])) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], g_lpszBaslerUSBLineMode[nPreValue], g_lpszBaslerUSBLineMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetLine1Source(EDeviceBaslerUSBLineSource * pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceBaslerUSBLineSource)_ttoi(GetParamValue(EDeviceParameterBaslerUSB_Line1Source));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetLine1Source(EDeviceBaslerUSBLineSource eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_Line1Source;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceBaslerUSBLineSource_Count)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		int nLineSelect = _ttoi(GetParamValue(EDeviceParameterBaslerUSB_LineSelector));

		if(nLineSelect != EDeviceBaslerUSBLineSelector_Line1)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		int nLineMode = _ttoi(GetParamValue(EDeviceParameterBaslerUSB_Line1Mode));

		if(nLineMode != EDeviceBaslerUSBLineMode_Output)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "LineSource"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "LineSource", CStringA(g_lpszBaslerUSBLineSource[eParam])) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], g_lpszBaslerUSBLineSource[nPreValue], g_lpszBaslerUSBLineSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetLine1Inverter(bool * pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerUSB_Line1Inverter));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetLine1Inverter(bool bParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_Line1Inverter;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		int nLineSelect = _ttoi(GetParamValue(EDeviceParameterBaslerUSB_LineSelector));

		if(nLineSelect != EDeviceBaslerUSBLineSelector_Line1)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "LineInverter"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceSetBooleanFeature(m_hDevice, "LineInverter", bParam) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], g_lpszBaslerUSBSwitch[nPreValue], g_lpszBaslerUSBSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetLine1DebouncerTime(double * pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterBaslerUSB_Line1DebouncerTime_us));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetLine1DebouncerTime(double dblParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_Line1DebouncerTime_us;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		int nLineSelect = _ttoi(GetParamValue(EDeviceParameterBaslerUSB_LineSelector));

		if(nLineSelect != EDeviceBaslerUSBLineSelector_Line1)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		int nLineMode = _ttoi(GetParamValue(EDeviceParameterBaslerUSB_Line1Mode));

		if(nLineMode != EDeviceBaslerUSBLineMode_Input)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "LineDebouncerTime"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceSetFloatFeature(m_hDevice, "LineDebouncerTime", dblParam) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetLine1MinOutputPulseWidth(double * pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterBaslerUSB_Line1MinimumOuputPulseWidth_us));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetLine1MinOutputPulseWidth(double dblParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_Line1MinimumOuputPulseWidth_us;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		int nLineSelect = _ttoi(GetParamValue(EDeviceParameterBaslerUSB_LineSelector));

		if(nLineSelect != EDeviceBaslerUSBLineSelector_Line1)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		int nLineMode = _ttoi(GetParamValue(EDeviceParameterBaslerUSB_Line1Mode));

		if(nLineMode != EDeviceBaslerUSBLineMode_Output)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "LineMinimumOutputPulseWidth"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceSetFloatFeature(m_hDevice, "LineMinimumOutputPulseWidth", dblParam) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetLine2Mode(EDeviceBaslerUSBLineMode * pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceBaslerUSBLineMode)_ttoi(GetParamValue(EDeviceParameterBaslerUSB_Line2Mode));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetLine2Mode(EDeviceBaslerUSBLineMode eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_Line2Mode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceBaslerUSBLineMode_Count)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		int nLineSelect = _ttoi(GetParamValue(EDeviceParameterBaslerUSB_LineSelector));

		if(nLineSelect != EDeviceBaslerUSBLineSelector_Line2)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "LineMode"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "LineMode", CStringA(g_lpszBaslerUSBLineMode[eParam])) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], g_lpszBaslerUSBLineMode[nPreValue], g_lpszBaslerUSBLineMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetLine2Source(EDeviceBaslerUSBLineSource * pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceBaslerUSBLineSource)_ttoi(GetParamValue(EDeviceParameterBaslerUSB_Line2Source));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetLine2Source(EDeviceBaslerUSBLineSource eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_Line2Source;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceBaslerUSBLineSource_Count)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		int nLineSelect = _ttoi(GetParamValue(EDeviceParameterBaslerUSB_LineSelector));

		if(nLineSelect != EDeviceBaslerUSBLineSelector_Line2)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		int nLineMode = _ttoi(GetParamValue(EDeviceParameterBaslerUSB_Line2Mode));

		if(nLineMode != EDeviceBaslerUSBLineMode_Output)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "LineSource"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "LineSource", CStringA(g_lpszBaslerUSBLineSource[eParam])) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], g_lpszBaslerUSBLineSource[nPreValue], g_lpszBaslerUSBLineSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetLine2Inverter(bool * pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerUSB_Line2Inverter));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetLine2Inverter(bool bParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_Line2Inverter;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		int nLineSelect = _ttoi(GetParamValue(EDeviceParameterBaslerUSB_LineSelector));

		if(nLineSelect != EDeviceBaslerUSBLineSelector_Line2)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "LineInverter"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceSetBooleanFeature(m_hDevice, "LineInverter", bParam) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], g_lpszBaslerUSBSwitch[nPreValue], g_lpszBaslerUSBSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetLine2DebouncerTime(double * pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterBaslerUSB_Line2DebouncerTime_us));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetLine2DebouncerTime(double dblParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_Line2DebouncerTime_us;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		int nLineSelect = _ttoi(GetParamValue(EDeviceParameterBaslerUSB_LineSelector));

		if(nLineSelect != EDeviceBaslerUSBLineSelector_Line2)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		int nLineMode = _ttoi(GetParamValue(EDeviceParameterBaslerUSB_Line2Mode));

		if(nLineMode != EDeviceBaslerUSBLineMode_Input)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "LineDebouncerTime"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceSetFloatFeature(m_hDevice, "LineDebouncerTime", dblParam) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetLine2MinOutputPulseWidth(double * pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterBaslerUSB_Line2MinimumOuputPulseWidth_us));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetLine2MinOutputPulseWidth(double dblParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_Line2MinimumOuputPulseWidth_us;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		int nLineSelect = _ttoi(GetParamValue(EDeviceParameterBaslerUSB_LineSelector));

		if(nLineSelect != EDeviceBaslerUSBLineSelector_Line2)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		int nLineMode = _ttoi(GetParamValue(EDeviceParameterBaslerUSB_Line2Mode));

		if(nLineMode != EDeviceBaslerUSBLineMode_Output)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "LineMinimumOutputPulseWidth"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceSetFloatFeature(m_hDevice, "LineMinimumOutputPulseWidth", dblParam) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetLine3Mode(EDeviceBaslerUSBLineMode * pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceBaslerUSBLineMode)_ttoi(GetParamValue(EDeviceParameterBaslerUSB_Line3Mode));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetLine3Mode(EDeviceBaslerUSBLineMode eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_Line3Mode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceBaslerUSBLineMode_Count)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		int nLineSelect = _ttoi(GetParamValue(EDeviceParameterBaslerUSB_LineSelector));

		if(nLineSelect != EDeviceBaslerUSBLineSelector_Line3)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "LineMode"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "LineMode", CStringA(g_lpszBaslerUSBLineMode[eParam])) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], g_lpszBaslerUSBLineMode[nPreValue], g_lpszBaslerUSBLineMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetLine3Source(EDeviceBaslerUSBLineSource * pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceBaslerUSBLineSource)_ttoi(GetParamValue(EDeviceParameterBaslerUSB_Line3Source));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetLine3Source(EDeviceBaslerUSBLineSource eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_Line3Source;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceBaslerUSBLineSource_Count)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		int nLineSelect = _ttoi(GetParamValue(EDeviceParameterBaslerUSB_LineSelector));

		if(nLineSelect != EDeviceBaslerUSBLineSelector_Line3)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		int nLineMode = _ttoi(GetParamValue(EDeviceParameterBaslerUSB_Line3Mode));

		if(nLineMode != EDeviceBaslerUSBLineMode_Output)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "LineSource"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "LineSource", CStringA(g_lpszBaslerUSBLineSource[eParam])) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], g_lpszBaslerUSBLineSource[nPreValue], g_lpszBaslerUSBLineSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetLine3Inverter(bool * pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerUSB_Line3Inverter));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetLine3Inverter(bool bParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_Line3Inverter;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		int nLineSelect = _ttoi(GetParamValue(EDeviceParameterBaslerUSB_LineSelector));

		if(nLineSelect != EDeviceBaslerUSBLineSelector_Line3)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "LineInverter"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceSetBooleanFeature(m_hDevice, "LineInverter", bParam) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], g_lpszBaslerUSBSwitch[nPreValue], g_lpszBaslerUSBSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetLine3DebouncerTime(double * pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterBaslerUSB_Line3DebouncerTime_us));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetLine3DebouncerTime(double dblParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_Line3DebouncerTime_us;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		int nLineSelect = _ttoi(GetParamValue(EDeviceParameterBaslerUSB_LineSelector));

		if(nLineSelect != EDeviceBaslerUSBLineSelector_Line3)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		int nLineMode = _ttoi(GetParamValue(EDeviceParameterBaslerUSB_Line3Mode));

		if(nLineMode != EDeviceBaslerUSBLineMode_Input)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "LineDebouncerTime"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceSetFloatFeature(m_hDevice, "LineDebouncerTime", dblParam) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetLine3MinOutputPulseWidth(double * pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterBaslerUSB_Line3MinimumOuputPulseWidth_us));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetLine3MinOutputPulseWidth(double dblParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_Line3MinimumOuputPulseWidth_us;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		int nLineSelect = _ttoi(GetParamValue(EDeviceParameterBaslerUSB_LineSelector));

		if(nLineSelect != EDeviceBaslerUSBLineSelector_Line3)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		int nLineMode = _ttoi(GetParamValue(EDeviceParameterBaslerUSB_Line3Mode));

		if(nLineMode != EDeviceBaslerUSBLineMode_Output)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "LineMinimumOutputPulseWidth"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceSetFloatFeature(m_hDevice, "LineMinimumOutputPulseWidth", dblParam) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetLine4Mode(EDeviceBaslerUSBLineMode * pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceBaslerUSBLineMode)_ttoi(GetParamValue(EDeviceParameterBaslerUSB_Line4Mode));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetLine4Mode(EDeviceBaslerUSBLineMode eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_Line4Mode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceBaslerUSBLineMode_Count)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		int nLineSelect = _ttoi(GetParamValue(EDeviceParameterBaslerUSB_LineSelector));

		if(nLineSelect != EDeviceBaslerUSBLineSelector_Line4)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "LineMode"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "LineMode", CStringA(g_lpszBaslerUSBLineMode[eParam])) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], g_lpszBaslerUSBLineMode[nPreValue], g_lpszBaslerUSBLineMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetLine4Source(EDeviceBaslerUSBLineSource * pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceBaslerUSBLineSource)_ttoi(GetParamValue(EDeviceParameterBaslerUSB_Line4Source));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetLine4Source(EDeviceBaslerUSBLineSource eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_Line4Source;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceBaslerUSBLineSource_Count)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		int nLineSelect = _ttoi(GetParamValue(EDeviceParameterBaslerUSB_LineSelector));

		if(nLineSelect != EDeviceBaslerUSBLineSelector_Line4)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		int nLineMode = _ttoi(GetParamValue(EDeviceParameterBaslerUSB_Line4Mode));

		if(nLineMode != EDeviceBaslerUSBLineMode_Output)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "LineSource"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "LineSource", CStringA(g_lpszBaslerUSBLineSource[eParam])) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], g_lpszBaslerUSBLineSource[nPreValue], g_lpszBaslerUSBLineSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetLine4Inverter(bool * pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerUSB_Line4Inverter));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetLine4Inverter(bool bParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_Line4Inverter;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		int nLineSelect = _ttoi(GetParamValue(EDeviceParameterBaslerUSB_LineSelector));

		if(nLineSelect != EDeviceBaslerUSBLineSelector_Line4)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "LineInverter"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceSetBooleanFeature(m_hDevice, "LineInverter", bParam) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], g_lpszBaslerUSBSwitch[nPreValue], g_lpszBaslerUSBSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetLine4DebouncerTime(double * pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterBaslerUSB_Line4DebouncerTime_us));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetLine4DebouncerTime(double dblParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_Line4DebouncerTime_us;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		int nLineSelect = _ttoi(GetParamValue(EDeviceParameterBaslerUSB_LineSelector));

		if(nLineSelect != EDeviceBaslerUSBLineSelector_Line4)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		int nLineMode = _ttoi(GetParamValue(EDeviceParameterBaslerUSB_Line4Mode));

		if(nLineMode != EDeviceBaslerUSBLineMode_Input)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "LineDebouncerTime"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceSetFloatFeature(m_hDevice, "LineDebouncerTime", dblParam) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerUSB::GetLine4MinOutputPulseWidth(double * pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterBaslerUSB_Line4MinimumOuputPulseWidth_us));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerUSB::SetLine4MinOutputPulseWidth(double dblParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerUSB eSaveID = EDeviceParameterBaslerUSB_Line4MinimumOuputPulseWidth_us;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		int nLineSelect = _ttoi(GetParamValue(EDeviceParameterBaslerUSB_LineSelector));

		if(nLineSelect != EDeviceBaslerUSBLineSelector_Line4)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		int nLineMode = _ttoi(GetParamValue(EDeviceParameterBaslerUSB_Line4Mode));

		if(nLineMode != EDeviceBaslerUSBLineMode_Output)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "LineMinimumOutputPulseWidth"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceSetFloatFeature(m_hDevice, "LineMinimumOutputPulseWidth", dblParam) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerUSB[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}





bool CDeviceBaslerUSB::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = false;

	bool bFoundID = true;

	do
	{
		switch(nParam)
		{
		case EDeviceParameterBaslerUSB_DeviceID:
			bReturn = !SetDeviceID(strValue);
			break;
		case EDeviceParameterBaslerUSB_GrabCount:
			bReturn = !SetGrabCount(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerUSB_CanvasWidth:
			bReturn = !SetCanvasWidth(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerUSB_CanvasHeight:
			bReturn = !SetCanvasHeight(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerUSB_OffsetCenterX:
			bReturn = !SetOffsetCenterX(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerUSB_OffsetCenterY:
			bReturn = !SetOffsetCenterY(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerUSB_OffsetX:
			bReturn = !SetOffsetX(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerUSB_OffsetY:
			bReturn = !SetOffsetY(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerUSB_ReverseX:
			bReturn = !SetReverseX(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerUSB_ReverseY:
			bReturn = !SetReverseY(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerUSB_GrabWaitingTime:
			bReturn = !SetGrabWaitingTime(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerUSB_PixelFormat:
			bReturn = !SetPixelFormat((EDeviceBaslerUSBPixelFormat)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerUSB_GainAuto:
			bReturn = !SetGainAuto((EDeviceBaslerUSBGainAuto)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerUSB_GainSelector:
			bReturn = !SetGainSelector((EDeviceBaslerUSBGainSelector)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerUSB_Gain:
			bReturn = !SetAllGain(_ttof(strValue));
			break;
		case EDeviceParameterBaslerUSB_BlackLevelSelector:
			bReturn = !SetBlackLevelSelector((EDeviceBaslerUSBBlackLevelSelector)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerUSB_BlackLevel:
			bReturn = !SetBlackLevel(_ttof(strValue));
			break;
		case EDeviceParameterBaslerUSB_Gamma:
			bReturn = !SetGamma(_ttof(strValue));
			break;
		case EDeviceParameterBaslerUSB_ShutterMode:
			bReturn = !SetShutterMode((EDeviceBaslerUSBShutterMode)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerUSB_ExposureAuto:
			bReturn = !SetExposureAuto((EDeviceBaslerUSBExposureAuto)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerUSB_ExposureMode:
			bReturn = !SetExposureMode((EDeviceBaslerUSBExposureMode)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerUSB_ExposureTime:
			bReturn = !SetExposureTime(_ttof(strValue));
			break;
		case EDeviceParameterBaslerUSB_TriggerSelector:
			bReturn = !SetTriggerSelector((EDeviceBaslerUSBTriggerSelector)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerUSB_FrameStartTriggerMode:
			bReturn = !SetFrameStartTriggerMode((EDeviceBaslerUSBTriggerMode)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerUSB_FrameStartTriggerSource:
			bReturn = !SetFrameStartTriggerSource((EDeviceBaslerUSBTriggerSource)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerUSB_FrameStartTriggerActivation:
			bReturn = !SetFrameStartTriggerActivation((EDeviceBaslerUSBTriggerActivation)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerUSB_FrameStartTriggerDelay:
			bReturn = !SetFrameStartTriggerDelay(_ttof(strValue));
			break;
		case EDeviceParameterBaslerUSB_FrameBurstStartTriggerMode:
			bReturn = !SetFrameBurstStartTriggerMode((EDeviceBaslerUSBTriggerMode)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerUSB_FrameBurstStartTriggerSource:
			bReturn = !SetFrameBurstStartTriggerSource((EDeviceBaslerUSBTriggerSource)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerUSB_FrameBurstStartTriggerActivation:
			bReturn = !SetFrameBurstStartTriggerActivation((EDeviceBaslerUSBTriggerActivation)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerUSB_FrameBurstStartTriggerDelay:
			bReturn = !SetFrameBurstStartTriggerDelay(_ttof(strValue));
			break;
		case EDeviceParameterBaslerUSB_EnableAcquisitionFrameRate:
			bReturn = !SetEnableAcquisitionFrameRate(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerUSB_AcquisitionFrameRate:
			bReturn = !SetAcquisitionFrameRate(_ttof(strValue));
			break;
		case EDeviceParameterBaslerUSB_AcquisitionStatusSelector:
			bReturn = !SetAcquisitionStatusSelector((EDeviceBaslerUSBAcquisitionStatusSelector)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerUSB_LineSelector:
			bReturn = !SetLineSelector((EDeviceBaslerUSBLineSelector)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerUSB_Line1Mode:
			bReturn = !SetLine1Mode((EDeviceBaslerUSBLineMode)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerUSB_Line1Source:
			bReturn = !SetLine1Source((EDeviceBaslerUSBLineSource)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerUSB_Line1Inverter:
			bReturn = !SetLine1Inverter(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerUSB_Line1DebouncerTime_us:
			bReturn = !SetLine1DebouncerTime(_ttof(strValue));
			break;
		case EDeviceParameterBaslerUSB_Line1MinimumOuputPulseWidth_us:
			bReturn = !SetLine1MinOutputPulseWidth(_ttof(strValue));
			break;
		case EDeviceParameterBaslerUSB_Line2Mode:
			bReturn = !SetLine2Mode((EDeviceBaslerUSBLineMode)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerUSB_Line2Source:
			bReturn = !SetLine2Source((EDeviceBaslerUSBLineSource)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerUSB_Line2Inverter:
			bReturn = !SetLine2Inverter(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerUSB_Line2DebouncerTime_us:
			bReturn = !SetLine2DebouncerTime(_ttof(strValue));
			break;
		case EDeviceParameterBaslerUSB_Line2MinimumOuputPulseWidth_us:
			bReturn = !SetLine2MinOutputPulseWidth(_ttof(strValue));
			break;
		case EDeviceParameterBaslerUSB_Line3Mode:
			bReturn = !SetLine3Mode((EDeviceBaslerUSBLineMode)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerUSB_Line3Source:
			bReturn = !SetLine3Source((EDeviceBaslerUSBLineSource)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerUSB_Line3Inverter:
			bReturn = !SetLine3Inverter(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerUSB_Line3DebouncerTime_us:
			bReturn = !SetLine3DebouncerTime(_ttof(strValue));
			break;
		case EDeviceParameterBaslerUSB_Line3MinimumOuputPulseWidth_us:
			bReturn = !SetLine3MinOutputPulseWidth(_ttof(strValue));
			break;
		case EDeviceParameterBaslerUSB_Line4Mode:
			bReturn = !SetLine4Mode((EDeviceBaslerUSBLineMode)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerUSB_Line4Source:
			bReturn = !SetLine4Source((EDeviceBaslerUSBLineSource)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerUSB_Line4Inverter:
			bReturn = !SetLine4Inverter(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerUSB_Line4DebouncerTime_us:			
			bReturn = !SetLine4DebouncerTime(_ttof(strValue));			
			break;
		case EDeviceParameterBaslerUSB_Line4MinimumOuputPulseWidth_us:
			bReturn = !SetLine4MinOutputPulseWidth(_ttof(strValue));
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

bool CDeviceBaslerUSB::DoesModuleExist()
{
	bool bReturn = false;

	CString strModuleName;

	do
	{
		strModuleName.Format(_T("PylonC_MD_VC120_v5_0.dll"));

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
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, CMultiLanguageManager::GetString(ELanguageParameter__s_d_s_toload_s), GetClassNameStr(), GetObjectID(), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), strModuleName);

	return bReturn;
}

bool CDeviceBaslerUSB::GetCommandInt32(const char * pCommnadString, int32_t* pGetValue)
{
	bool bReturn = false;

	do
	{
		if(!m_hDevice)
			break;

		if(!pCommnadString)
			break;

		if(PylonDeviceGetIntegerFeatureInt32(m_hDevice, pCommnadString, pGetValue) != S_OK)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceBaslerUSB::SetCommandInt32(const char * pCommnadString, int32_t nSetValue)
{
	bool bReturn = false;

	do
	{
		if(!m_hDevice)
			break;

		if(!pCommnadString)
			break;

		if(PylonDeviceSetIntegerFeatureInt32(m_hDevice, pCommnadString, nSetValue) != S_OK)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceBaslerUSB::GetCommandInt64(const char * pCommnadString, int64_t* pGetValue)
{
	bool bReturn = false;

	do
	{
		if(!m_hDevice)
			break;

		if(!pCommnadString)
			break;

		if(PylonDeviceGetIntegerFeature(m_hDevice, pCommnadString, pGetValue) != S_OK)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceBaslerUSB::SetCommandInt64(const char * pCommnadString, int64_t nSetValue)
{
	bool bReturn = false;

	do
	{
		if(!m_hDevice)
			break;

		if(!pCommnadString)
			break;

		if(PylonDeviceSetIntegerFeature(m_hDevice, pCommnadString, nSetValue) != S_OK)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

UINT CDeviceBaslerUSB::CallbackFunction(LPVOID pParam)
{
	CDeviceBaslerUSB* pInstance = (CDeviceBaslerUSB*)pParam;

	if(pInstance)
	{
		BYTE* pDstImg = nullptr;

		do
		{
			int nSize = 0;

			if(PylonDeviceGetIntegerFeatureInt32(pInstance->m_hDevice, "PayloadSize", &nSize) != S_OK)
			{
				pInstance->SetMessage(_T("Failed to get payload size"));
				break;
			}

			size_t nStreams = 0;

			if(PylonDeviceGetNumStreamGrabberChannels(pInstance->m_hDevice, &nStreams) != S_OK)
				break;

			if(nStreams < 1)
				break;

			if(PylonDeviceGetStreamGrabber(pInstance->m_hDevice, 0, &pInstance->m_hGrabber) != S_OK)
				break;

			if(PylonStreamGrabberOpen(pInstance->m_hGrabber) != S_OK)
				break;

			if(PylonStreamGrabberGetWaitObject(pInstance->m_hGrabber, &pInstance->m_hWait) != S_OK)
				break;

			if(PylonStreamGrabberSetMaxNumBuffer(pInstance->m_hGrabber, 1) != S_OK)
				break;

			if(PylonStreamGrabberSetMaxBufferSize(pInstance->m_hGrabber, nSize) != S_OK)
				break;

			if(PylonStreamGrabberPrepareGrab(pInstance->m_hGrabber) != S_OK)
				break;

			if(pInstance->GetImageInfo()->GetChannels() == 1)
			{
				pDstImg = new BYTE[nSize];
				
				if(PylonStreamGrabberRegisterBuffer(pInstance->m_hGrabber, pDstImg, nSize, &pInstance->m_hGrabBuffer) != S_OK)
					break;
			}
			else
			{
				pDstImg = new BYTE[nSize];

				if(PylonStreamGrabberRegisterBuffer(pInstance->m_hGrabber, pDstImg, nSize, &pInstance->m_hGrabBuffer) != S_OK)
					break;
			}

			if(PylonStreamGrabberQueueBuffer(pInstance->m_hGrabber, pInstance->m_hGrabBuffer, (void*)0) != S_OK)
				break;

			if(PylonDeviceExecuteCommandFeature(pInstance->m_hDevice, "AcquisitionStart") != S_OK)
				break;

			bool isReady = false;

			PylonGrabResult_t grabResult;

			if(!pInstance->IsInitialized())
				break;

			int nCount = 0;

 			if(pInstance->GetGrabCount(&nCount))
 				break;

			if(pInstance->GetImageInfo()->GetChannels() == 1)
			{
				do
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

					if(PylonWaitObjectWait(pInstance->m_hWait, pInstance->m_nWaitTime, &isReady) != S_OK)
						break;

					if(!isReady)
						break;

					if(PylonStreamGrabberRetrieveResult(pInstance->m_hGrabber, &grabResult, &isReady) != S_OK)
						break;

					if(!isReady)
						break;

					switch(grabResult.PixelType)
					{
					case PixelType_Mono8:
					case PixelType_Mono10:
					case PixelType_Mono10p:
					case PixelType_Mono12:
					case PixelType_Mono12p:
					case PixelType_Mono16:
						for(int i = 0; i < i64Height; ++i)
							memcpy(*(ppCurrentOffsetY + i), (BYTE*)(grabResult.pBuffer) + (i * grabResult.SizeX * i64PixelSizeByte), i64WidthStep);
						break;
					default:
						{
							bool bSuccess = false;

							PYLON_IMAGE_FORMAT_CONVERTER_HANDLE hFormat;

							if(PylonImageFormatConverterCreate(&hFormat) == S_OK)
							{
								if(PylonImageFormatConverterSetOutputPixelFormat(hFormat, PixelType_Mono8) == S_OK)
								{
									bSuccess = (PylonImageFormatConverterConvert(hFormat, pCurrentBuffer, i64ImageSizeByte, grabResult.pBuffer, grabResult.PayloadSize, grabResult.PixelType, grabResult.SizeX, grabResult.SizeY, grabResult.PaddingX, ImageOrientation_TopDown) == S_OK);
								}

								PylonImageFormatConverterDestroy(hFormat);
							}
						}
						break;
					}

					if(grabResult.Status == Grabbed)
					{
						pInstance->ConnectImage(false);

						CEventHandlerManager::BroadcastOnAcquisition(pCurrentImage, pInstance->GetObjectID(), pInstance->GetImageView(), pInstance);
					}

					if(PylonStreamGrabberQueueBuffer(pInstance->m_hGrabber, grabResult.hBuffer, (void*)0) != S_OK)
						break;

					++pInstance->m_nGrabCount;
				}
				while(pInstance->IsInitialized() && (pInstance->IsLive() || (unsigned long)nCount > pInstance->m_nGrabCount));
			}
			else
			{
				do
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

					if(PylonWaitObjectWait(pInstance->m_hWait, pInstance->m_nWaitTime, &isReady) != S_OK)
						break;

					if(!isReady)
						break;

					if(PylonStreamGrabberRetrieveResult(pInstance->m_hGrabber, &grabResult, &isReady) != S_OK)
						break;

					if(!isReady)
						break;

					if(grabResult.Status == Grabbed)
					{
						bool bSuccess = false;

						PYLON_IMAGE_FORMAT_CONVERTER_HANDLE hFormat;

						if(PylonImageFormatConverterCreate(&hFormat) == S_OK)
						{
							if(PylonImageFormatConverterSetOutputPixelFormat(hFormat, PixelType_RGB8packed) == S_OK)
							{
								bSuccess = (PylonImageFormatConverterConvert(hFormat, pCurrentBuffer, i64ImageSizeByte, grabResult.pBuffer, grabResult.PayloadSize, grabResult.PixelType, grabResult.SizeX, grabResult.SizeY, grabResult.PaddingX, ImageOrientation_TopDown) == S_OK);
							}

							PylonImageFormatConverterDestroy(hFormat);
						}

						if(bSuccess)
						{
							pInstance->ConnectImage(false);

							CEventHandlerManager::BroadcastOnAcquisition(pCurrentImage, pInstance->GetObjectID(), pInstance->GetImageView(), pInstance);
						}
					}

					if(PylonStreamGrabberQueueBuffer(pInstance->m_hGrabber, grabResult.hBuffer, (void*)0) != S_OK)
						break;

					++pInstance->m_nGrabCount;
				}
				while(pInstance->IsInitialized() && (pInstance->IsLive() || (unsigned long)nCount > pInstance->m_nGrabCount));
			}


			if(PylonDeviceExecuteCommandFeature(pInstance->m_hDevice, "AcquisitionStop") != S_OK)
				break;

			if(PylonStreamGrabberCancelGrab(pInstance->m_hGrabber) != S_OK)
				break;

			do
			{
				if(PylonStreamGrabberRetrieveResult(pInstance->m_hGrabber, &grabResult, &isReady) != S_OK)
					break;
			}
			while(isReady);

			if(PylonStreamGrabberDeregisterBuffer(pInstance->m_hGrabber, pInstance->m_hGrabBuffer) != S_OK)
				break;

			if(PylonStreamGrabberFinishGrab(pInstance->m_hGrabber) != S_OK)
				break;

			if(PylonStreamGrabberClose(pInstance->m_hGrabber) != S_OK)
				break;
		}
		while(false);

		if(pDstImg)
		{
			delete[] pDstImg;
			pDstImg = nullptr;
		}

		pInstance->m_bIsLive = false;
		pInstance->m_bIsGrabAvailable = true;
	}

	return 0;
}

#endif