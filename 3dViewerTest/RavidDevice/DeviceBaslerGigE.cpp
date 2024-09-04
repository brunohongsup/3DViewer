#include "stdafx.h"

#include "DeviceBaslerGigE.h"

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

IMPLEMENT_DYNAMIC(CDeviceBaslerGigE, CDeviceCamera)

BEGIN_MESSAGE_MAP(CDeviceBaslerGigE, CDeviceCamera)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static LPCTSTR g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_Count] =
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
	_T("Grab Waiting Time[ms]"),
	_T("PixelFormat"),
	_T("Packet Size"),
	_T("Trigger Setting"),
	_T("Trigger Selector"),
	_T("FrameStart"),
	_T("FrameStart Trigger Mode"),
	_T("FrameStart Trigger Source"),
	_T("FrameStart Trigger Activation"),
	_T("LineStart"),
	_T("LineStart Trigger Mode"),
	_T("LineStart Trigger Source"),
	_T("LineStart Trigger Activation"),
	_T("AcquisitionStart"),
	_T("AcquisitionStart Trigger Mode"),
	_T("AcquisitionStart Trigger Source"),
	_T("AcquisitionStart Trigger Activation"),
	_T("Exposure and Gain Setting"),
	_T("Exposure Auto"),
	_T("Exposure"),
	_T("Gain Auto"),
	_T("Gain Selector"),
	_T("Gain"),
	_T("Analog Gain"),
	_T("Digital Gain"),
	_T("Tap1 Gain"),
	_T("Tap2 Gain"),

	_T("Color Improvements Control"),
	_T("Light Source Selector"),
	_T("Balance White Reset"),
	_T("Balance White Auto"),
	_T("[Red] Balance Ration Raw"),
	_T("[Green] Balance Ration Raw"),
	_T("[Blue] Balance Ration Raw"),
	
	_T("Color Trans Gain"),
	
	_T("[Gain00] Raw"),
	_T("[Gain01] Raw"),
	_T("[Gain02] Raw"),
	
	_T("[Gain10] Raw"),
	_T("[Gain11] Raw"),
	_T("[Gain12] Raw"),
	
	_T("[Gain20] Raw"),
	_T("[Gain21] Raw"),
	_T("[Gain22] Raw"),

	_T("Color Trans Matrix"),

	_T("MatrixFactor Raw"),

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

static LPCTSTR g_lpszBaslerGigEPixelFormat[EDeviceBaslerGigEPixelFormat_Count] =
{
	_T("Mono8"),
	_T("Mono10"),
	_T("Mono12"),
	_T("Mono12Packed"),
	_T("Mono16"),
	_T("BayerGR8"),
	_T("BayerRG8"),
	_T("BayerGB8"),
	_T("BayerBG8"),
	_T("BayerGR10"),
	_T("BayerRG10"),
	_T("BayerGB10"),
	_T("BayerBG10"),
	_T("BayerGR12"),
	_T("BayerRG12"),
	_T("BayerGB12"),
	_T("BayerBG12"),
	_T("RGB8Packed"),
	_T("BGR8Packed"),
	_T("YUV422Packed"),
	_T("YUV422_YUYV_Packed"),
	_T("BayerGB12Packed"),
	_T("BayerGR12Packed"),
	_T("BayerRG12Packed"),
	_T("BayerBG12Packed")
};

static LPCTSTR g_lpszBaslerGigETriggerSelect[EDeviceBaslerGigETriggerSelector_Count] =
{
	_T("FrameStart"),
	_T("LineStart"),
	_T("AcquisitionStart")
};

static LPCTSTR g_lpszBaslerGigESwitch[EDeviceBaslerGigESwitch_Count] =
{
	_T("Off"),
	_T("On")
};

static LPCTSTR g_lpszBaslerGigESource[EDeviceBaslerGigETriggerSource_Count] =
{
	_T("Software"),
	_T("Line1"),
	_T("Line2"),
	_T("Line3"),
	_T("Line4"),
	_T("Line5"),
	_T("Line6"),
	_T("Line7"),
	_T("Line8"),
	_T("CC1"),
	_T("CC2"),
	_T("CC3"),
	_T("CC4"),
	_T("Action1"),
	_T("Action2"),
	_T("Action3"),
	_T("Action4")
};

static LPCTSTR g_lpszBaslerGigEActivation[EDeviceBaslerGigETriggerActivation_Count] =
{
	_T("RisingEdge"),
	_T("FallingEdge"),
	_T("AnyEdge"),
	_T("LevelHigh"),
	_T("LevelLow")
};

static LPCTSTR g_lpszBaslerGigEAuto[EDeviceBaslerGigEAuto_Count] =
{
	_T("Off"),
	_T("Once"),
	_T("Continuous")
};

static LPCTSTR g_lpszBaslerGigEGainSelect[EDeviceBaslerGigEGainSelector_Count] =
{
	_T("All"),
	_T("AnalogAll"),
	_T("DigitalAll"),
	_T("Tap1"),
	_T("Tap2")
};

static LPCTSTR g_lpszBaslerGigELineSelector[EDeviceBaslerGigELineSelector_Count] =
{
	_T("Line1"),
	_T("Line2"),
	_T("Line3"),
	_T("Line4"),
};

static LPCTSTR g_lpszBaslerGigELineMode[EDeviceBaslerGigELineMode_Count] =
{
	_T("Input"),
	_T("Output"),
};

static LPCTSTR g_lpszBaslerGigELineSource[EDeviceBaslerGigELineSource_Count] =
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

static LPCTSTR g_lpszBaslerGigELightSourceSelector[EDeviceBaslerGigELightSourceSelector_Count] = 
{
	_T("Off"),
	_T("Custom"),
	_T("Daylight"),
	_T("Tungsten"),
	_T("Daylight6500K"),
};



CDeviceBaslerGigE::CDeviceBaslerGigE()
{
}


CDeviceBaslerGigE::~CDeviceBaslerGigE()
{
	Terminate();
}

EDeviceInitializeResult CDeviceBaslerGigE::Initialize()
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

		bool bNeedInitialize = true;
		for (int i = 0; i < (int)CDeviceManager::GetDeviceCount(); ++i)
		{
			CDeviceBaslerGigE* pDevice = dynamic_cast<CDeviceBaslerGigE*>(CDeviceManager::GetDeviceByIndex(typeid(CDeviceBaslerGigE), i));
			if (!pDevice)
				continue;

			if (pDevice == this)
				continue;

			if (!pDevice->IsInitialized())
				continue;

			bNeedInitialize = false;
			break;
		}

		if (bNeedInitialize)
		{
			if(PylonInitialize() != S_OK)
			{
				strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntopenthelibrary);
				eReturn = EDeviceInitializeResult_AlreadyInitializedError;
				break;
			}
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

		int nParam = 0;

		if(GetGrabWaitingTime(&nParam))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Waiting Time"));
			eReturn = EDeviceInitializeResult_CanNotReadDBWaitingTime;
			break;
		}

		m_nWaitTime = nParam;

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

		EDeviceBaslerGigEPixelFormat eFormat = EDeviceBaslerGigEPixelFormat_Count;

		if(GetPixelFormat(&eFormat))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("PixelFormat"));
			eReturn = EDeviceInitializeResult_CanNotReadDBPixelFormat;
			break;
		}

		if(PylonDeviceFeatureIsWritable(m_hDevice, "PixelFormat"))
		{
			if(PylonDeviceFeatureFromString(m_hDevice, "PixelFormat", CStringA(g_lpszBaslerGigEPixelFormat[eFormat])) != S_OK)
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

		if(PylonDeviceFeatureIsWritable(m_hDevice, "ExposureAuto"))
		{
			EDeviceBaslerGigEAuto eExposureAuto = EDeviceBaslerGigEAuto_Count;

			if(GetExposureAuto(&eExposureAuto))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ExposureAuto"));
				eReturn = EDeviceInitializeResult_CanNotReadDBExposureAuto;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "ExposureAuto", CStringA(g_lpszBaslerGigEAuto[eExposureAuto])) != S_OK)
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ExposureAuto"));
				eReturn = EDeviceInitializeResult_CanNotApplyExposureAuto;
				break;
			}

			if(PylonDeviceFeatureIsWritable(m_hDevice, "ExposureTimeRaw"))
			{
				int nParam = 0;

				if(GetExposure(&nParam))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Exposure"));
					eReturn = EDeviceInitializeResult_CanNotReadDBExposure;
					break;
				}

				bool bError = false;

				if(!SetCommandInt32("ExposureTimeRaw", nParam))
				{
					bError = true;
					
					if(GetCommandInt32("ExposureTimeRaw", &nParam))
					{
						if(!SetExposure(nParam))
							bError = false;
					}
				}

				if(bError)
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ExposureTimeRaw"));
					eReturn = EDeviceInitializeResult_CanNotApplyExposureTimeRaw;
					break;
				}
			}
		}
		
		if(PylonDeviceFeatureIsWritable(m_hDevice, "GainAuto"))
		{
			EDeviceBaslerGigEAuto eGainAuto = EDeviceBaslerGigEAuto_Count;

			if(GetGainAuto(&eGainAuto))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("GainAuto"));
				eReturn = EDeviceInitializeResult_CanNotReadDBGainAuto;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "GainAuto", CStringA(g_lpszBaslerGigEAuto[eGainAuto])) != S_OK)
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
				if(PylonDeviceFeatureIsWritable(m_hDevice, "GainRaw"))
				{
					int nParam = 0;

					if(GetAllGain(&nParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AllGain"));
						eReturn = EDeviceInitializeResult_CanNotReadDBAllGain;
						break;
					}

					bool bError = false;

					if(!SetCommandInt32("GainRaw", nParam))
					{
						bError = true;

						if(GetCommandInt32("GainRaw", &nParam))
						{
							if(!SetAllGain(nParam))
								bError = false;
						}
					}

					if(bError)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("AllGain"));
						eReturn = EDeviceInitializeResult_CanNotApplyAllGainRaw;
						break;
					}
				}
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "GainSelector", "AnalogAll") == S_OK)
			{
				if(PylonDeviceFeatureIsWritable(m_hDevice, "GainRaw"))
				{
					int nParam = 0;

					if(GetAnalogGain(&nParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AnalogGain"));
						eReturn = EDeviceInitializeResult_CanNotReadDBAnalogGain;
						break;
					}

					bool bError = false;

					if(!SetCommandInt32("GainRaw", nParam))
					{
						bError = true;

						if(GetCommandInt32("GainRaw", &nParam))
						{
							if(!SetAnalogGain(nParam))
								bError = false;
						}
					}

					if(bError)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("AnalogGain"));
						eReturn = EDeviceInitializeResult_CanNotApplyAnalogAllGainRaw;
						break;
					}
				}
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "GainSelector", "DigitalAll") == S_OK)
			{
				if(PylonDeviceFeatureIsWritable(m_hDevice, "GainRaw"))
				{
					int nParam = 0;

					if(GetDigitalGain(&nParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("DigitalGain"));
						eReturn = EDeviceInitializeResult_CanNotReadDBDigitalGain;
						break;
					}

					bool bError = false;

					if(!SetCommandInt32("GainRaw", nParam))
					{
						bError = true;

						if(GetCommandInt32("GainRaw", &nParam))
						{
							if(!SetDigitalGain(nParam))
								bError = false;
						}
					}

					if(bError)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("DigitalGain"));
						eReturn = EDeviceInitializeResult_CanNotApplyDigitalAllGainRaw;
						break;
					}
				}
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "GainSelector", "Tap1") == S_OK)
			{
				if(PylonDeviceFeatureIsWritable(m_hDevice, "GainRaw"))
				{
					int nParam = 0;

					if(GetTap1Gain(&nParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Tap1Gain"));
						eReturn = EDeviceInitializeResult_CanNotReadDBTap1Gain;
						break;
					}

					bool bError = false;

					if(!SetCommandInt32("GainRaw", nParam))
					{
						bError = true;

						if(GetCommandInt32("GainRaw", &nParam))
						{
							if(!SetTap1Gain(nParam))
								bError = false;
						}
					}

					if(bError)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Tap1Gain"));
						eReturn = EDeviceInitializeResult_CanNotApplyTap1GainRaw;
						break;
					}
				}
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "GainSelector", "Tap2") == S_OK)
			{
				if(PylonDeviceFeatureIsWritable(m_hDevice, "GainRaw"))
				{
					int nParam = 0;
					if(GetTap2Gain(&nParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Tap2Gain"));
						eReturn = EDeviceInitializeResult_CanNotReadDBTap2Gain;

						break;
					}

					bool bError = false;

					if(!SetCommandInt32("GainRaw", nParam))
					{
						bError = true;

						if(GetCommandInt32("GainRaw", &nParam))
						{
							if(!SetTap2Gain(nParam))
								bError = false;
						}
					}

					if(bError)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Tap2Gain"));
						eReturn = EDeviceInitializeResult_CanNotApplyTap2GainRaw;
						break;
					}
				}
			}
		}

		if(PylonDeviceFeatureIsWritable(m_hDevice, "TriggerSelector"))
		{
			if(PylonDeviceFeatureFromString(m_hDevice, "TriggerSelector", "FrameStart") == S_OK)
			{
				EDeviceBaslerGigESwitch eTriggerMode = EDeviceBaslerGigESwitch_Count;

				if(GetFrameStartMode(&eTriggerMode))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("FrameStartMode"));
					eReturn = EDeviceInitializeResult_CanNotReadDBFrameStartMode;
					break;
				}

				if(PylonDeviceFeatureIsWritable(m_hDevice, "TriggerMode"))
				{
					if(PylonDeviceFeatureFromString(m_hDevice, "TriggerMode", CStringA(g_lpszBaslerGigESwitch[eTriggerMode])) != S_OK)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("FrameStartMode"));
						eReturn = EDeviceInitializeResult_CanNotApplyFrameStartTriggerMode;
						break;
					}
				}

				if(PylonDeviceFeatureIsWritable(m_hDevice, "TriggerSource"))
				{
					EDeviceBaslerGigETriggerSource eSource = EDeviceBaslerGigETriggerSource_Count;
					if(GetFrameStartTriggerSource(&eSource))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("FrameStartTriggerSource"));
						eReturn = EDeviceInitializeResult_CanNotReadDBFrameStartTriggerSource;
						break;
					}

					if(PylonDeviceFeatureFromString(m_hDevice, "TriggerSource", CStringA(g_lpszBaslerGigESource[eSource])) != S_OK)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("FrameStartTriggerSource"));
						eReturn = EDeviceInitializeResult_CanNotApplyTriggerSource;
						break;
					}
				}
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "TriggerSelector", "LineStart") == S_OK)
			{
				EDeviceBaslerGigESwitch eTriggerMode = EDeviceBaslerGigESwitch_Count;

				if(GetLineStartMode(&eTriggerMode))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineStartMode"));
					eReturn = EDeviceInitializeResult_CanNotReadDBLineStartMode;
					break;
				}
				
				if(PylonDeviceFeatureIsWritable(m_hDevice, "TriggerMode"))
				{
					if(PylonDeviceFeatureFromString(m_hDevice, "TriggerMode", CStringA(g_lpszBaslerGigESwitch[eTriggerMode])) != S_OK)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineStartMode"));
						eReturn = EDeviceInitializeResult_CanNotApplyLineStartTriggerMode;
						break;
					}
				}

				if(PylonDeviceFeatureIsWritable(m_hDevice, "TriggerSource"))
				{
					EDeviceBaslerGigETriggerSource eSource = EDeviceBaslerGigETriggerSource_Count;

					if(GetLineStartTriggerSource(&eSource))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineStartTriggerSource"));
						eReturn = EDeviceInitializeResult_CanNotReadDBLineStartTriggerSource;
						break;
					}

					if(PylonDeviceFeatureFromString(m_hDevice, "TriggerSource", CStringA(g_lpszBaslerGigESource[eSource])) != S_OK)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineStartTriggerSource"));
						eReturn = EDeviceInitializeResult_CanNotApplyTriggerSource;
						break;
					}
				}
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "TriggerSelector", "AcquisitionStart") == S_OK)
			{
				EDeviceBaslerGigESwitch eTriggerMode = EDeviceBaslerGigESwitch_Count;

				if(GetAcquisitionStartMode(&eTriggerMode))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AcquisitionStartMode"));
					eReturn = EDeviceInitializeResult_CanNotReadDBAcquisitionStartMode;
					break;
				}
				
				if(PylonDeviceFeatureIsWritable(m_hDevice, "TriggerMode"))
				{
					if(PylonDeviceFeatureFromString(m_hDevice, "TriggerMode", CStringA(g_lpszBaslerGigESwitch[eTriggerMode])) != S_OK)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("AcquisitionStartMode"));
						eReturn = EDeviceInitializeResult_CanNotApplyAcquisitionStartTriggerMode;
						break;
					}
				}

				if(PylonDeviceFeatureIsWritable(m_hDevice, "TriggerSource"))
				{
					EDeviceBaslerGigETriggerSource eSource = EDeviceBaslerGigETriggerSource_Count;

					if(GetAcquisitionStartTriggerSource(&eSource))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AcquisitionStartTriggerSource"));
						eReturn = EDeviceInitializeResult_CanNotReadDBAcquisitionStartTriggerSource;
						break;
					}

					if(PylonDeviceFeatureFromString(m_hDevice, "TriggerSource", CStringA(g_lpszBaslerGigESource[eSource])) != S_OK)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("AcquisitionStartTriggerSource"));
						eReturn = EDeviceInitializeResult_CanNotApplyTriggerSource;
						break;
					}
				}
			}

			EDeviceBaslerGigETriggerSelector eTriggerSelect = EDeviceBaslerGigETriggerSelector_Count;

			if(GetTriggerSelector(&eTriggerSelect))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TriggerSelector"));
				eReturn = EDeviceInitializeResult_CanNotReadDBTriggerSelector;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "TriggerSelector", CStringA(g_lpszBaslerGigETriggerSelect[eTriggerSelect])) != S_OK)
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TriggerSelector"));
				eReturn = EDeviceInitializeResult_CanNotApplyTriggerSelector;
				break;
			}
		}
		
		if(PylonDeviceFeatureIsWritable(m_hDevice, "AcquisitionMode"))
		{
			if(PylonDeviceFeatureFromString(m_hDevice, "AcquisitionMode", "Continuous") != S_OK)
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("AcquisitionMode"));
				eReturn = EDeviceInitializeResult_CanNotApplyAcquisitionModeContinuous;
				break;
			}
		}

		if(PylonDeviceFeatureIsWritable(m_hDevice, "AcquisitionFrameCount"))
		{
			if(PylonDeviceSetIntegerFeature(m_hDevice, "AcquisitionFrameCount", 1) != S_OK) //180123¼öÁ¤
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("AcquisitionFrameCount"));
				eReturn = EDeviceInitializeResult_CanNotApplyAcquisitionModeContinuous;
				break;
			}
		}

		if(PylonDeviceFeatureIsWritable(m_hDevice, "LineSelector"))
		{
			EDeviceBaslerGigELineSelector eLineSelector = EDeviceBaslerGigELineSelector_Count;

			if(GetLineSelector(&eLineSelector))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineSelector"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "LineSelector", CStringA(g_lpszBaslerGigELineSelector[EDeviceBaslerGigELineSelector_Line1])) == S_OK)
			{
				EDeviceBaslerGigELineMode eLineMode = EDeviceBaslerGigELineMode_Count;

				if(GetLine1Mode(&eLineMode))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line1Mode"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(PylonDeviceFeatureFromString(m_hDevice, "LineMode", CStringA(g_lpszBaslerGigELineMode[eLineMode])) == S_OK)
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
					case EDeviceBaslerGigELineMode_Input:
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
					case EDeviceBaslerGigELineMode_Output:
						{
							EDeviceBaslerGigELineSource eSource = EDeviceBaslerGigELineSource_Count;

							if(GetLine1Source(&eSource))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line1Source"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							if(PylonDeviceFeatureFromString(m_hDevice, "LineSource", CStringA(g_lpszBaslerGigELineSource[eSource])) != S_OK)
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
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineMinimumOutputPulseWidth"));
								eReturn = EDeviceInitializeResult_WriteToDeviceError;
								break;
							}
						}
						break;
					}
				}
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "LineSelector", CStringA(g_lpszBaslerGigELineSelector[EDeviceBaslerGigELineSelector_Line2])) == S_OK)
			{
				EDeviceBaslerGigELineMode eLineMode = EDeviceBaslerGigELineMode_Count;

				if(GetLine2Mode(&eLineMode))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line2Mode"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(PylonDeviceFeatureFromString(m_hDevice, "LineMode", CStringA(g_lpszBaslerGigELineMode[eLineMode])) == S_OK)
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
					case EDeviceBaslerGigELineMode_Input:
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
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineDebouncerTime"));
								eReturn = EDeviceInitializeResult_WriteToDeviceError;
								break;
							}
						}
						break;
					case EDeviceBaslerGigELineMode_Output:
						{
							EDeviceBaslerGigELineSource eSource = EDeviceBaslerGigELineSource_Count;

							if(GetLine2Source(&eSource))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line2Source"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							if(PylonDeviceFeatureFromString(m_hDevice, "LineSource", CStringA(g_lpszBaslerGigELineSource[eSource])) != S_OK)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineSource"));
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
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineMinimumOutputPulseWidth"));
								eReturn = EDeviceInitializeResult_WriteToDeviceError;
								break;
							}
						}
						break;
					}
				}
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "LineSelector", CStringA(g_lpszBaslerGigELineSelector[EDeviceBaslerGigELineSelector_Line3])) == S_OK)
			{
				EDeviceBaslerGigELineMode eLineMode = EDeviceBaslerGigELineMode_Count;

				if(GetLine3Mode(&eLineMode))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line3Mode"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(PylonDeviceFeatureFromString(m_hDevice, "LineMode", CStringA(g_lpszBaslerGigELineMode[eLineMode])) == S_OK)
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
					case EDeviceBaslerGigELineMode_Input:
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
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineDebouncerTime"));
								eReturn = EDeviceInitializeResult_WriteToDeviceError;
								break;
							}
						}
						break;
					case EDeviceBaslerGigELineMode_Output:
						{
							EDeviceBaslerGigELineSource eSource = EDeviceBaslerGigELineSource_Count;

							if(GetLine3Source(&eSource))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line3Source"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							if(PylonDeviceFeatureFromString(m_hDevice, "LineSource", CStringA(g_lpszBaslerGigELineSource[eSource])) != S_OK)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineSource"));
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
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineMinimumOutputPulseWidth"));
								eReturn = EDeviceInitializeResult_WriteToDeviceError;
								break;
							}
						}
						break;
					}
				}
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "LineSelector", CStringA(g_lpszBaslerGigELineSelector[EDeviceBaslerGigELineSelector_Line4])) == S_OK)
			{
				EDeviceBaslerGigELineMode eLineMode = EDeviceBaslerGigELineMode_Count;

				if(GetLine4Mode(&eLineMode))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line4Mode"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(PylonDeviceFeatureFromString(m_hDevice, "LineMode", CStringA(g_lpszBaslerGigELineMode[eLineMode])) == S_OK)
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
					case EDeviceBaslerGigELineMode_Input:
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
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineDebouncerTime"));
								eReturn = EDeviceInitializeResult_WriteToDeviceError;
								break;
							}
						}
						break;
					case EDeviceBaslerGigELineMode_Output:
						{
							EDeviceBaslerGigELineSource eSource = EDeviceBaslerGigELineSource_Count;

							if(GetLine4Source(&eSource))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line4Source"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							if(PylonDeviceFeatureFromString(m_hDevice, "LineSource", CStringA(g_lpszBaslerGigELineSource[eSource])) != S_OK)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineSource"));
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
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineMinimumOutputPulseWidth"));
								eReturn = EDeviceInitializeResult_WriteToDeviceError;
								break;
							}
						}
						break;
					}
				}
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "LineSelector", CStringA(g_lpszBaslerGigELineSelector[eLineSelector])) != S_OK)
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineSelector"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}
		}

		if(PylonDeviceFeatureIsWritable(m_hDevice, "GevSCPSPacketSize"))
		{
			int nSize = 0;

			if(GetPacketSize(&nSize))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("GevSCPSPacketSize"));
				eReturn = EDeviceInitializeResult_CanNotReadDBPacketSize;
				break;
			}

			bool bError = false;

			if(!SetCommandInt32("GevSCPSPacketSize", nSize))
			{
				bError = true;

				if(GetCommandInt32("GevSCPSPacketSize", &nSize))
				{
					if(!SetPacketSize(nSize))
						bError = false;
				}
			}

			if(bError)
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("GevSCPSPacketSize"));
				eReturn = EDeviceInitializeResult_CanNotApplyGevSCPSPacketSize;
				break;
			}
		}

		//////////////////////////////////////////////////////////////////////////

		EDeviceBaslerGigELightSourceSelector eLightSource = EDeviceBaslerGigELightSourceSelector_Count;

		if(GetLightSourceSelector(&eLightSource))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Light Source Selector"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}
		
		if(PylonDeviceFeatureIsWritable(m_hDevice, "LightSourceSelector"))
		{
			if(PylonDeviceFeatureFromString(m_hDevice, "LightSourceSelector", CStringA(g_lpszBaslerGigELightSourceSelector[eLightSource])) != S_OK)
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Light Source Selector"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}
		}

		if(PylonDeviceFeatureIsWritable(m_hDevice, "BalanceWhiteAuto"))
		{
			EDeviceBaslerGigEAuto eBalanceAuto = EDeviceBaslerGigEAuto_Count;

			if(GetBalanceWhiteAuto(&eBalanceAuto))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Balance White Auto"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "BalanceWhiteAuto", CStringA(g_lpszBaslerGigEAuto[eBalanceAuto])) != S_OK)
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Balance White Auto"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}
		}
		
		if(PylonDeviceFeatureIsWritable(m_hDevice, "BalanceRatioSelector"))
		{
			if(PylonDeviceFeatureFromString(m_hDevice, "BalanceRatioSelector", "Red") == S_OK)
			{
				int nRedData = 0;

				if(GetBalanceWhiteRawRed(&nRedData))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Balance White Red"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(!SetCommandInt32("BalanceRatioRaw", nRedData))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Balance White Red"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "BalanceRatioSelector", "Green") == S_OK)
			{
				int nRedData = 0;

				if(GetBalanceWhiteRawGreen(&nRedData))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Balance White Green"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(!SetCommandInt32("BalanceRatioRaw", nRedData))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Balance White Green"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "BalanceRatioSelector", "Blue") == S_OK)
			{
				int nRedData = 0;

				if(GetBalanceWhiteRawBlue(&nRedData))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Balance White Blue"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(!SetCommandInt32("BalanceRatioRaw", nRedData))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Balance White Blue"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}
			}
		}

		switch(eLightSource)
		{
		case EDeviceBaslerGigELightSourceSelector_Custom:
			{
				if(PylonDeviceFeatureIsWritable(m_hDevice, "ColorTransformationSelector"))
				{
					if(PylonDeviceFeatureFromString(m_hDevice, "ColorTransformationSelector", "RGBtoRGB") == S_OK)
					{
						if(PylonDeviceFeatureIsWritable(m_hDevice, "ColorTransformationValueSelector"))
						{
							if(PylonDeviceFeatureFromString(m_hDevice, "ColorTransformationValueSelector", "Gain00") != S_OK)
							{
								int nGain = 0;

								if(GetColorTransRawGain00(&nGain))
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ColorTransRawGain00"));
									eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
									break;
								}

								if(!SetCommandInt32("ColorTransformationValueRaw", nParam))
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ColorTransRawGain00"));
									eReturn = EDeviceInitializeResult_WriteToDeviceError;
									break;
								}
							}

							if(PylonDeviceFeatureFromString(m_hDevice, "ColorTransformationValueSelector", "Gain01") != S_OK)
							{
								int nGain = 0;

								if(GetColorTransRawGain01(&nGain))
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ColorTransRawGain01"));
									eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
									break;
								}

								if(!SetCommandInt32("ColorTransformationValueRaw", nParam))
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ColorTransRawGain01"));
									eReturn = EDeviceInitializeResult_WriteToDeviceError;
									break;
								}
							}

							if(PylonDeviceFeatureFromString(m_hDevice, "ColorTransformationValueSelector", "Gain02") != S_OK)
							{
								int nGain = 0;

								if(GetColorTransRawGain02(&nGain))
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ColorTransRawGain02"));
									eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
									break;
								}

								if(!SetCommandInt32("ColorTransformationValueRaw", nParam))
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ColorTransRawGain02"));
									eReturn = EDeviceInitializeResult_WriteToDeviceError;
									break;
								}
							}

							if(PylonDeviceFeatureFromString(m_hDevice, "ColorTransformationValueSelector", "Gain10") != S_OK)
							{
								int nGain = 0;

								if(GetColorTransRawGain10(&nGain))
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ColorTransRawGain10"));
									eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
									break;
								}

								if(!SetCommandInt32("ColorTransformationValueRaw", nParam))
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ColorTransRawGain10"));
									eReturn = EDeviceInitializeResult_WriteToDeviceError;
									break;
								}
							}

							if(PylonDeviceFeatureFromString(m_hDevice, "ColorTransformationValueSelector", "Gain11") != S_OK)
							{
								int nGain = 0;

								if(GetColorTransRawGain11(&nGain))
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ColorTransRawGain11"));
									eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
									break;
								}

								if(!SetCommandInt32("ColorTransformationValueRaw", nParam))
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ColorTransRawGain11"));
									eReturn = EDeviceInitializeResult_WriteToDeviceError;
									break;
								}
							}

							if(PylonDeviceFeatureFromString(m_hDevice, "ColorTransformationValueSelector", "Gain12") != S_OK)
							{
								int nGain = 0;

								if(GetColorTransRawGain12(&nGain))
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ColorTransRawGain12"));
									eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
									break;
								}

								if(!SetCommandInt32("ColorTransformationValueRaw", nParam))
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ColorTransRawGain12"));
									eReturn = EDeviceInitializeResult_WriteToDeviceError;
									break;
								}
							}

							if(PylonDeviceFeatureFromString(m_hDevice, "ColorTransformationValueSelector", "Gain20") != S_OK)
							{
								int nGain = 0;

								if(GetColorTransRawGain20(&nGain))
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ColorTransRawGain20"));
									eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
									break;
								}

								if(!SetCommandInt32("ColorTransformationValueRaw", nParam))
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ColorTransRawGain20"));
									eReturn = EDeviceInitializeResult_WriteToDeviceError;
									break;
								}
							}

							if(PylonDeviceFeatureFromString(m_hDevice, "ColorTransformationValueSelector", "Gain21") != S_OK)
							{
								int nGain = 0;

								if(GetColorTransRawGain21(&nGain))
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ColorTransRawGain21"));
									eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
									break;
								}

								if(!SetCommandInt32("ColorTransformationValueRaw", nParam))
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ColorTransRawGain21"));
									eReturn = EDeviceInitializeResult_WriteToDeviceError;
									break;
								}
							}

							if(PylonDeviceFeatureFromString(m_hDevice, "ColorTransformationValueSelector", "Gain22") != S_OK)
							{
								int nGain = 0;

								if(GetColorTransRawGain22(&nGain))
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ColorTransRawGain22"));
									strMessage.Format(_T("Couldn't read 'ColorTransRawGain22' from the database"));
									eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
									break;
								}

								if(!SetCommandInt32("ColorTransformationValueRaw", nParam))
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ColorTransRawGain22"));
									eReturn = EDeviceInitializeResult_WriteToDeviceError;
									break;
								}
							}
						}
					}
				}
			}
			break;
		case EDeviceBaslerGigELightSourceSelector_Daylight:
		case EDeviceBaslerGigELightSourceSelector_Tungsten:
		case EDeviceBaslerGigELightSourceSelector_Daylight6500K:
			{
				int nMatrix = 0;

				if(GetColorTransMatrixFactorRaw(&nMatrix))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ColorTransMatrixFactorRaw"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(!SetCommandInt32("ColorTransformationMatrixFactorRaw", nMatrix))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ColorTransMatrixFactorRaw"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}
			}
			break;
		default:
			break;
		}

		//////////////////////////////////////////////////////////////////////////

		EDeviceBaslerGigEPixelFormat ePixelFormat = EDeviceBaslerGigEPixelFormat_Count;

		if(GetPixelFormat(&ePixelFormat))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("PixelFormat"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		CString strFormat = g_lpszBaslerGigEPixelFormat[ePixelFormat];

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

EDeviceTerminateResult CDeviceBaslerGigE::Terminate()
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
		if(m_hDevice)
		{
			if(IsLive() || !IsGrabAvailable())
				Stop();
			
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

			bool bNeedTerminate = true;
			for (int i = 0; i < (int)CDeviceManager::GetDeviceCount(); ++i)
			{
				CDeviceBaslerGigE* pDevice = dynamic_cast<CDeviceBaslerGigE*>(CDeviceManager::GetDeviceByIndex(typeid(CDeviceBaslerGigE), i));
				if (!pDevice)
					continue;

				if (pDevice == this)
					continue;

				if (!pDevice->IsInitialized())
					continue;

				bNeedTerminate = false;
				break;
			}

			if(bNeedTerminate)
				PylonTerminate();
			
			m_hDevice = nullptr;
		}

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

bool CDeviceBaslerGigE::LoadSettings()
{
	bool bReturn = false;

	m_vctParameterFieldConfigurations.clear();

	do
	{
		CString strTime;
		strTime.Format(_T("%d"), INT_MAX);
		
		AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_DeviceID, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_DeviceID], _T("0"), EParameterFieldType_Edit);
		AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_CameraSetting, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_CameraSetting], _T("1"), EParameterFieldType_None);
		AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_GrabCount, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_GrabCount], _T("1"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_CanvasWidth, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_CanvasWidth], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_CanvasHeight, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_CanvasHeight], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_OffsetCenterX, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_OffsetCenterX], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_OffsetCenterY, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_OffsetCenterY], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_OffsetX, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_OffsetX], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_OffsetY, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_OffsetY], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_GrabWaitingTime, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_GrabWaitingTime], strTime, EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_PixelFormat, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_PixelFormat], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerGigEPixelFormat, EDeviceBaslerGigEPixelFormat_Count), nullptr, 1);			  
		AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_PacketSize, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_PacketSize], _T("1500"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_TriggerSetting, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_TriggerSetting], _T("1"), EParameterFieldType_None);
		AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_TriggerSelector, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_TriggerSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerGigETriggerSelect, EDeviceBaslerGigETriggerSelector_Count), nullptr, 1);		  
		AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_FrameStart, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_FrameStart], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_FrameStartTriggerMode, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_FrameStartTriggerMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerGigESwitch, EDeviceBaslerGigESwitch_Count), nullptr, 2);				  
		AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_FrameStartTriggerSource, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_FrameStartTriggerSource], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerGigESource, EDeviceBaslerGigETriggerSource_Count), nullptr, 2);			  
		AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_FrameStartTriggerActivation, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_FrameStartTriggerActivation], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerGigEActivation, EDeviceBaslerGigETriggerActivation_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_LineStart, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_LineStart], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_LineStartTriggerMode, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_LineStartTriggerMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerGigESwitch, EDeviceBaslerGigESwitch_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_LineStartTriggerSource, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_LineStartTriggerSource], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerGigESource, EDeviceBaslerGigETriggerSource_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_LineStartTriggerActivation, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_LineStartTriggerActivation], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerGigEActivation, EDeviceBaslerGigETriggerActivation_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_AcquisitionStart, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_AcquisitionStart], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_AcquisitionStartTriggerMode, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_AcquisitionStartTriggerMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerGigESwitch, EDeviceBaslerGigESwitch_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_AcquisitionStartTriggerSource, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_AcquisitionStartTriggerSource], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerGigESource, EDeviceBaslerGigETriggerSource_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_AcquisitionStartTriggerActivation, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_AcquisitionStartTriggerActivation], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerGigEActivation, EDeviceBaslerGigETriggerActivation_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_ExposureAndGainSetting, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_ExposureAndGainSetting], _T("1"), EParameterFieldType_None);
		AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_ExposureAuto, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_ExposureAuto], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerGigEAuto, EDeviceBaslerGigEAuto_Count), nullptr, 1);											
		AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_Exposure, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_Exposure], _T("1000"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_GainAuto, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_GainAuto], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerGigEAuto, EDeviceBaslerGigEAuto_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_GainSelector, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_GainSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerGigEGainSelect, EDeviceBaslerGigEGainSelector_Count), nullptr, 1);			
		AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_Gain, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_Gain], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_AnalogGain, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_AnalogGain], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_DigitalGain, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_DigitalGain], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_Tap1Gain, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_Tap1Gain], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_Tap2Gain, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_Tap2Gain], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);


		AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_ColorImprovementsControl, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_ColorImprovementsControl], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		{
			AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_LightSourceSelector, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_LightSourceSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerGigELightSourceSelector, EDeviceBaslerGigELightSourceSelector_Count), nullptr, 1);
			
			AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_BalanceWhiteReset, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_BalanceWhiteReset], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_BalanceWhiteAuto, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_BalanceWhiteAuto], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerGigEAuto, EDeviceBaslerGigEAuto_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_BalanceRationRawRed, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_BalanceRationRawRed], _T("64"), EParameterFieldType_Edit, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_BalanceRationRawGreen, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_BalanceRationRawGreen], _T("64"), EParameterFieldType_Edit, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_BalanceRationRawBlue, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_BalanceRationRawBlue], _T("64"), EParameterFieldType_Edit, nullptr, nullptr, 1);


			AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_ColorTransformationGain, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_ColorTransformationGain], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_ColorTransValueRawGain00, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_ColorTransValueRawGain00], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
			AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_ColorTransValueRawGain01, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_ColorTransValueRawGain01], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
			AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_ColorTransValueRawGain02, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_ColorTransValueRawGain02], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);

			AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_ColorTransValueRawGain10, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_ColorTransValueRawGain10], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
			AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_ColorTransValueRawGain11, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_ColorTransValueRawGain11], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
			AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_ColorTransValueRawGain12, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_ColorTransValueRawGain12], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);

			AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_ColorTransValueRawGain20, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_ColorTransValueRawGain20], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
			AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_ColorTransValueRawGain21, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_ColorTransValueRawGain21], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
			AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_ColorTransValueRawGain22, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_ColorTransValueRawGain22], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);

			AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_ColorTransformationMatrix, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_ColorTransformationMatrix], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_ColorTransMatrixFactorRaw, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_ColorTransMatrixFactorRaw], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
		}


		AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_DigitalIOConrol, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_DigitalIOConrol], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		{
			AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_LineSelector, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_LineSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerGigELineSelector, EDeviceBaslerGigELineSelector_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_Line1, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_Line1], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
			{
				AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_Line1Mode, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_Line1Mode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerGigELineMode, EDeviceBaslerGigELineMode_Count), nullptr, 2);

				AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_Line1Source, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_Line1Source], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerGigELineSource, EDeviceBaslerGigELineSource_Count), nullptr, 2);

				AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_Line1Inverter, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_Line1Inverter], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 2);

				AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_Line1DebouncerTime_us, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_Line1DebouncerTime_us], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);

				AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_Line1MinimumOuputPulseWidth_us, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_Line1MinimumOuputPulseWidth_us], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
			}

			AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_Line2, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_Line2], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
			{
				AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_Line2Mode, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_Line2Mode], _T("1"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerGigELineMode, EDeviceBaslerGigELineMode_Count), nullptr, 2);

				AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_Line2Source, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_Line2Source], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerGigELineSource, EDeviceBaslerGigELineSource_Count), nullptr, 2);

				AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_Line2Inverter, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_Line2Inverter], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 2);

				AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_Line2DebouncerTime_us, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_Line2DebouncerTime_us], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);

				AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_Line2MinimumOuputPulseWidth_us, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_Line2MinimumOuputPulseWidth_us], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
			}

			AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_Line3, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_Line3], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
			{
				AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_Line3Mode, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_Line3Mode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerGigELineMode, EDeviceBaslerGigELineMode_Count), nullptr, 2);

				AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_Line3Source, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_Line3Source], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerGigELineSource, EDeviceBaslerGigELineSource_Count), nullptr, 2);

				AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_Line3Inverter, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_Line3Inverter], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 2);

				AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_Line3DebouncerTime_us, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_Line3DebouncerTime_us], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);

				AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_Line3MinimumOuputPulseWidth_us, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_Line3MinimumOuputPulseWidth_us], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
			}

			AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_Line4, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_Line4], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
			{
				AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_Line4Mode, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_Line4Mode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerGigELineMode, EDeviceBaslerGigELineMode_Count), nullptr, 2);

				AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_Line4Source, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_Line4Source], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerGigELineSource, EDeviceBaslerGigELineSource_Count), nullptr, 2);

				AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_Line4Inverter, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_Line4Inverter], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 2);

				AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_Line4DebouncerTime_us, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_Line4DebouncerTime_us], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);

				AddParameterFieldConfigurations(EDeviceParameterBaslerGigE_Line4MinimumOuputPulseWidth_us, g_lpszParamBaslerGigE[EDeviceParameterBaslerGigE_Line4MinimumOuputPulseWidth_us], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
			}
		}

		bReturn = true;
	}
	while(false);

	return bReturn & __super::LoadSettings();
}

EDeviceGrabResult CDeviceBaslerGigE::Grab()
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
		
		if(GetGrabCount(&nRequestGrabCount))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Grab count"));
			eReturn = EDeviceGrabResult_ReadOnDatabaseError;
			break;
		}

		if(nRequestGrabCount <= 0)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Grab count"));
			eReturn = EDeviceGrabResult_ReadOnDeviceError;
			break;
		}

		m_nGrabCount = nRequestGrabCount;

		m_bIsGrabAvailable = false;

		m_pLiveThread = AfxBeginThread(CDeviceBaslerGigE::CallbackFunction, this, 0, 0U, CREATE_SUSPENDED);

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

EDeviceLiveResult CDeviceBaslerGigE::Live()
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

		m_pLiveThread = AfxBeginThread(CDeviceBaslerGigE::CallbackFunction, this, 0, 0U, CREATE_SUSPENDED);

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

EDeviceStopResult CDeviceBaslerGigE::Stop()
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

EDeviceTriggerResult CDeviceBaslerGigE::Trigger()
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
	while (false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetGrabCount(int* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do 
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_GrabCount));

		eReturn = EBaslerGetFunction_OK;
	} 
	while(false);		

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetGrabCount(int nParam)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], nPreValue, nParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetCanvasWidth(int* pParam)
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

EBaslerSetFunction CDeviceBaslerGigE::SetCanvasWidth(int nParam)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetCanvasHeight(int* pParam)
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

EBaslerSetFunction CDeviceBaslerGigE::SetCanvasHeight(int nParam)
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

		if(!SetParamValue(eSaveID,  strValue))
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetOffsetCenterX(bool* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do 
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_OffsetCenterX));

		eReturn = EBaslerGetFunction_OK;
	} 
	while(false);		

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetOffsetCenterX(bool bParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_OffsetCenterX;

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
				int nParam = 0;

				if(GetCommandInt32("OffsetX", &nParam))
					SetOffsetX(nParam);
			}
		}

		CString strOffset;
		strOffset.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strOffset))
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], g_lpszBaslerGigESwitch[nPreValue], g_lpszBaslerGigESwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetOffsetCenterY(bool *pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_OffsetCenterY));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);		

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetOffsetCenterY(bool bParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_OffsetCenterY;

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
				int nParam = 0;

				if(GetCommandInt32("OffsetY", &nParam))
					SetOffsetX(nParam);
			}
		}

		CString strOffset;
		strOffset.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strOffset))
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], g_lpszBaslerGigESwitch[nPreValue], g_lpszBaslerGigESwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetOffsetX(int* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_OffsetX));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);		

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetOffsetX(int nParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_OffsetX;

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

			if(!SetCommandInt32("OffsetX", nParam))
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strOffset;
		strOffset.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strOffset))
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetOffsetY(int* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_OffsetY));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);		

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetOffsetY(int nParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_OffsetY;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "OffsetY"))
			{
				eReturn = EBaslerSetFunction_NotSupportError; // NotSupportedError
				break;
			}

			if(!SetCommandInt32("OffsetY", nParam))
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strOffset;
		strOffset.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strOffset))
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetGrabWaitingTime(int* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam =  _ttoi(GetParamValue(EDeviceParameterBaslerGigE_GrabWaitingTime));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);
	
	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetGrabWaitingTime(int nParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_GrabWaitingTime;

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

		if(!SetParamValue(eSaveID, strTime))
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);
	
	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetExposure(int* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_Exposure));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetExposure(int nParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_Exposure;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "ExposureTimeRaw"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(!SetCommandInt32("ExposureTimeRaw", nParam))
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetAllGain(int* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_Gain));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);
	
	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetAllGain(int nParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_Gain;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PylonDeviceFeatureFromString(m_hDevice, "GainSelector", "All") != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				
				break;
			}

			if(!SetCommandInt32("GainRaw", nParam))
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetAnalogGain(int* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_AnalogGain));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetAnalogGain(int nParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_AnalogGain;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PylonDeviceFeatureFromString(m_hDevice, "GainSelector", "AnalogAll") != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}

			if(!SetCommandInt32("GainRaw", nParam))
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetDigitalGain(int* pParam)
{

	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_DigitalGain));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);		

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetDigitalGain(int nParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_DigitalGain;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PylonDeviceFeatureFromString(m_hDevice, "GainSelector", "DigitalAll") != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}

			if(!SetCommandInt32("GainRaw", nParam))
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetTap1Gain(int* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_Tap1Gain));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);		

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetTap1Gain(int nParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_Tap1Gain;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PylonDeviceFeatureFromString(m_hDevice, "GainSelector", "Tap1") != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				
				break;
			}

			if(!SetCommandInt32("GainRaw", nParam))
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetTap2Gain(int* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_Tap2Gain));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);		

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetTap2Gain(int nParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_Tap2Gain;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PylonDeviceFeatureFromString(m_hDevice, "GainSelector", "Tap2") != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}

			if(!SetCommandInt32("GainRaw", nParam))
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetPacketSize(int* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_PacketSize));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetPacketSize(int nParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_PacketSize;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PylonDeviceFeatureIsWritable(m_hDevice, "GevSCPSPacketSize"))
			{
				if(!SetCommandInt32("GevSCPSPacketSize", nParam))
				{
					eReturn = EBaslerSetFunction_WriteToDeviceError;
					break;
				}
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetPixelFormat(EDeviceBaslerGigEPixelFormat* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceBaslerGigEPixelFormat)_ttoi(GetParamValue(EDeviceParameterBaslerGigE_PixelFormat));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);
		

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetPixelFormat(EDeviceBaslerGigEPixelFormat eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_PixelFormat;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDeviceBaslerGigEPixelFormat)0 || eParam > EDeviceBaslerGigEPixelFormat_Count)
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
		strData.Format(_T("%d"), (int)eParam);

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], g_lpszBaslerGigEPixelFormat[nPreValue], g_lpszBaslerGigEPixelFormat[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetTriggerSelector(EDeviceBaslerGigETriggerSelector* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceBaslerGigETriggerSelector)_ttoi(GetParamValue(EDeviceParameterBaslerGigE_TriggerSelector));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);		

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetTriggerSelector(EDeviceBaslerGigETriggerSelector eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_TriggerSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "TriggerSelector"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "TriggerSelector", CStringA(g_lpszBaslerGigETriggerSelect[eParam])) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
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
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], g_lpszBaslerGigETriggerSelect[nPreValue], g_lpszBaslerGigETriggerSelect[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetFrameStartMode(EDeviceBaslerGigESwitch* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_FrameStartTriggerMode));

		if(nData < 0 || nData >= (int)EDeviceBaslerGigESwitch_Count)
		{
			eReturn = EBaslerGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaslerGigESwitch)nData;

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetFrameStartMode(EDeviceBaslerGigESwitch eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_FrameStartTriggerMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		bool bParam = eParam == EDeviceBaslerGigESwitch_On;

		if(IsInitialized())
		{
			if(IsLive() || !IsGrabAvailable())
			{
				eReturn = EBaslerSetFunction_ActivatedDeviceError;
				break;
			}

			Trigger();

			if(!PylonDeviceFeatureIsWritable(m_hDevice, "TriggerSelector"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(bParam)
			{
				if(PylonDeviceFeatureFromString(m_hDevice, "TriggerSelector", "LineStart") == S_OK)
				{
					if(PylonDeviceFeatureFromString(m_hDevice, "TriggerMode", "Off") != S_OK)
					{
						eReturn = EBaslerSetFunction_WriteToDeviceError;
						break;
					}

					if(!SetParamValue(EDeviceParameterBaslerGigE_LineStartTriggerMode, _T("0")))
					{
						eReturn = EBaslerSetFunction_WriteToDatabaseError;
						break;
					}

					if(!SaveSettings(EDeviceParameterBaslerGigE_LineStartTriggerMode))
					{
						eReturn = EBaslerSetFunction_WriteToDatabaseError;
						break;
					}
				}

				if(PylonDeviceFeatureFromString(m_hDevice, "TriggerSelector", "AcquisitionStart") == S_OK)
				{
					if(PylonDeviceFeatureFromString(m_hDevice, "TriggerMode", "Off") != S_OK)
					{
						eReturn = EBaslerSetFunction_WriteToDeviceError;
						break;
					}

					if(!SetParamValue(EDeviceParameterBaslerGigE_AcquisitionStartTriggerMode, _T("0")))
					{
						eReturn = EBaslerSetFunction_WriteToDatabaseError;
						break;
					}

					if(!SaveSettings(EDeviceParameterBaslerGigE_AcquisitionStartTriggerMode))
					{
						eReturn = EBaslerSetFunction_WriteToDatabaseError;
						break;
					}
				}
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "TriggerSelector", "FrameStart") != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "TriggerMode", CStringA(g_lpszBaslerGigESwitch[eParam])) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
		}
		else
		{
			if(bParam)
			{
				if(!SetParamValue(EDeviceParameterBaslerGigE_LineStartTriggerMode, _T("0")))
				{
					eReturn = EBaslerSetFunction_WriteToDatabaseError;
					break;
				}

				if(!SaveSettings(EDeviceParameterBaslerGigE_LineStartTriggerMode))
				{
					eReturn = EBaslerSetFunction_WriteToDatabaseError;
					break;
				}

				if(!SetParamValue(EDeviceParameterBaslerGigE_AcquisitionStartTriggerMode, _T("0")))
				{
					eReturn = EBaslerSetFunction_WriteToDatabaseError;
					break;
				}

				if(!SaveSettings(EDeviceParameterBaslerGigE_AcquisitionStartTriggerMode))
				{
					eReturn = EBaslerSetFunction_WriteToDatabaseError;
					break;
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
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], g_lpszBaslerGigESwitch[nPreValue], g_lpszBaslerGigESwitch[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetLineStartMode(EDeviceBaslerGigESwitch* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceBaslerGigESwitch)_ttoi(GetParamValue(EDeviceParameterBaslerGigE_LineStartTriggerMode));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetLineStartMode(EDeviceBaslerGigESwitch eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_LineStartTriggerMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		bool bParam = eParam == EDeviceBaslerGigESwitch_On;

		if(IsInitialized())
		{
			if(IsLive() || !IsGrabAvailable())
			{
				eReturn = EBaslerSetFunction_ActivatedDeviceError;
				break;
			}

			Trigger();

			if(!PylonDeviceFeatureIsWritable(m_hDevice, "TriggerSelector"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(bParam)
			{
				if(PylonDeviceFeatureFromString(m_hDevice, "TriggerSelector", "FrameStart") == S_OK)
				{
					if(PylonDeviceFeatureFromString(m_hDevice, "TriggerMode", "Off") != S_OK)
					{
						eReturn = EBaslerSetFunction_WriteToDeviceError;
						break;
					}
					
					if(!SetParamValue(EDeviceParameterBaslerGigE_FrameStartTriggerMode, _T("0")))
					{
						eReturn = EBaslerSetFunction_WriteToDatabaseError;
						break;
					}

					if(!SaveSettings(EDeviceParameterBaslerGigE_FrameStartTriggerMode))
					{
						eReturn = EBaslerSetFunction_WriteToDatabaseError;
						break;
					}
				}

				if(PylonDeviceFeatureFromString(m_hDevice, "TriggerSelector", "AcquisitionStart") == S_OK)
				{
					if(PylonDeviceFeatureFromString(m_hDevice, "TriggerMode", "Off") != S_OK)
					{
						eReturn = EBaslerSetFunction_WriteToDeviceError;
						break;
					}

					if(!SetParamValue(EDeviceParameterBaslerGigE_AcquisitionStartTriggerMode, _T("0")))
					{
						eReturn = EBaslerSetFunction_WriteToDatabaseError;
						break;
					}

					if(!SaveSettings(EDeviceParameterBaslerGigE_AcquisitionStartTriggerMode))
					{
						eReturn = EBaslerSetFunction_WriteToDatabaseError;
						break;
					}
				}
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "TriggerSelector", "LineStart") != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "TriggerMode", CStringA(g_lpszBaslerGigESwitch[eParam])) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
		}
		else
		{
			if(bParam)
			{
				if(!SetParamValue(EDeviceParameterBaslerGigE_FrameStartTriggerMode, _T("0")))
				{
					eReturn = EBaslerSetFunction_WriteToDatabaseError;
					break;
				}

				if(!SaveSettings(EDeviceParameterBaslerGigE_FrameStartTriggerMode))
				{
					eReturn = EBaslerSetFunction_WriteToDatabaseError;
					break;
				}

				if(!SetParamValue(EDeviceParameterBaslerGigE_AcquisitionStartTriggerMode, _T("0")))
				{
					eReturn = EBaslerSetFunction_WriteToDatabaseError;
					break;
				}

				if(!SaveSettings(EDeviceParameterBaslerGigE_AcquisitionStartTriggerMode))
				{
					eReturn = EBaslerSetFunction_WriteToDatabaseError;
					break;
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
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);
	
	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], g_lpszBaslerGigESwitch[nPreValue], g_lpszBaslerGigESwitch[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetAcquisitionStartMode(EDeviceBaslerGigESwitch* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(_T("AcquisitionStart Trigger Mode")));

		if(nData < 0 || nData >= (int)EDeviceBaslerGigESwitch_Count)
		{
			eReturn = EBaslerGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaslerGigESwitch)nData;

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetAcquisitionStartMode(EDeviceBaslerGigESwitch eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_AcquisitionStartTriggerMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		bool bParam = eParam == EDeviceBaslerGigESwitch_On;

		if(IsInitialized())
		{
			if(IsLive() || !IsGrabAvailable())
			{
				eReturn = EBaslerSetFunction_ActivatedDeviceError;
				break;
			}

			Trigger();

			if(!PylonDeviceFeatureIsWritable(m_hDevice, "TriggerSelector"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(bParam)
			{
				if(PylonDeviceFeatureFromString(m_hDevice, "TriggerSelector", "FrameStart") == S_OK)
				{
					if(PylonDeviceFeatureFromString(m_hDevice, "TriggerMode", "Off") != S_OK)
					{
						eReturn = EBaslerSetFunction_WriteToDeviceError;
						break;
					}

					if(!SetParamValue(EDeviceParameterBaslerGigE_FrameStartTriggerMode, _T("0")))
					{
						eReturn = EBaslerSetFunction_WriteToDatabaseError;
						break;
					}

					if(!SaveSettings(EDeviceParameterBaslerGigE_FrameStartTriggerMode))
					{
						eReturn = EBaslerSetFunction_WriteToDatabaseError;
						break;
					}
				}

				if(PylonDeviceFeatureFromString(m_hDevice, "TriggerSelector", "LineStart") == S_OK)
				{
					if(PylonDeviceFeatureFromString(m_hDevice, "TriggerMode", "Off") != S_OK)
					{
						eReturn = EBaslerSetFunction_WriteToDeviceError;
						break;
					}

					if(!SetParamValue(EDeviceParameterBaslerGigE_LineStartTriggerMode, _T("0")))
					{
						eReturn = EBaslerSetFunction_WriteToDatabaseError;
						break;
					}

					if(!SaveSettings(EDeviceParameterBaslerGigE_LineStartTriggerMode))
					{
						eReturn = EBaslerSetFunction_WriteToDatabaseError;
						break;
					}
				}
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "TriggerSelector", "AcquisitionStart") != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "TriggerMode", CStringA(g_lpszBaslerGigESwitch[eParam])) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
		}
		else
		{
			if(bParam)
			{
				if(!SetParamValue(EDeviceParameterBaslerGigE_FrameStartTriggerMode, _T("0")))
				{
					eReturn = EBaslerSetFunction_WriteToDatabaseError;
					break;
				}

				if(!SaveSettings(EDeviceParameterBaslerGigE_FrameStartTriggerMode))
				{
					eReturn = EBaslerSetFunction_WriteToDatabaseError;
					break;
				}

				if(!SetParamValue(EDeviceParameterBaslerGigE_LineStartTriggerMode, _T("0")))
				{
					eReturn = EBaslerSetFunction_WriteToDatabaseError;
					break;
				}

				if(!SaveSettings(EDeviceParameterBaslerGigE_LineStartTriggerMode))
				{
					eReturn = EBaslerSetFunction_WriteToDatabaseError;
					break;
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
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], g_lpszBaslerGigESwitch[nPreValue], g_lpszBaslerGigESwitch[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetFrameStartTriggerSource(EDeviceBaslerGigETriggerSource* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceBaslerGigETriggerSource)_ttoi(GetParamValue(EDeviceParameterBaslerGigE_FrameStartTriggerSource));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetFrameStartTriggerSource(EDeviceBaslerGigETriggerSource eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_FrameStartTriggerSource;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PylonDeviceFeatureFromString(m_hDevice, "TriggerSelector", "FrameStart") != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "TriggerSource", CStringA(g_lpszBaslerGigESource[eParam])) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
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
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], g_lpszBaslerGigESource[nPreValue], g_lpszBaslerGigESource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetLineStartTriggerSource(EDeviceBaslerGigETriggerSource* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceBaslerGigETriggerSource)_ttoi(GetParamValue(EDeviceParameterBaslerGigE_LineStartTriggerSource));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetLineStartTriggerSource(EDeviceBaslerGigETriggerSource eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_LineStartTriggerSource;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PylonDeviceFeatureFromString(m_hDevice, "TriggerSelector", "LineStart") != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "TriggerSource", CStringA(g_lpszBaslerGigESource[eParam])) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
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
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], g_lpszBaslerGigESource[nPreValue], g_lpszBaslerGigESource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetAcquisitionStartTriggerSource(EDeviceBaslerGigETriggerSource* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceBaslerGigETriggerSource)_ttoi(GetParamValue(EDeviceParameterBaslerGigE_AcquisitionStartTriggerSource));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);
	
	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetAcquisitionStartTriggerSource(EDeviceBaslerGigETriggerSource eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_AcquisitionStartTriggerSource;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceBaslerGigETriggerSource_Count)
		{
			eReturn = EBaslerSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			if(PylonDeviceFeatureFromString(m_hDevice, "TriggerSelector", "AcquisitionStart") != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "TriggerSource", CStringA(g_lpszBaslerGigESource[eParam])) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
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
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], g_lpszBaslerGigESource[nPreValue], g_lpszBaslerGigESource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetFrameStartTriggerActivation(EDeviceBaslerGigETriggerActivation* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceBaslerGigETriggerActivation)_ttoi(GetParamValue(EDeviceParameterBaslerGigE_FrameStartTriggerActivation));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetFrameStartTriggerActivation(EDeviceBaslerGigETriggerActivation eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_FrameStartTriggerActivation;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PylonDeviceFeatureFromString(m_hDevice, "TriggerSelector", "FrameStart") != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}

			if(!PylonDeviceFeatureIsWritable(m_hDevice, "TriggerActivation"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "TriggerActivation", CStringA(g_lpszBaslerGigEActivation[eParam])) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
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
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], g_lpszBaslerGigEActivation[nPreValue], g_lpszBaslerGigEActivation[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetLineStartTriggerActivation(EDeviceBaslerGigETriggerActivation* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}
		
		*pParam = (EDeviceBaslerGigETriggerActivation) _ttoi(GetParamValue(EDeviceParameterBaslerGigE_LineStartTriggerActivation));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetLineStartTriggerActivation(EDeviceBaslerGigETriggerActivation eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_LineStartTriggerActivation;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceBaslerGigETriggerActivation_Count)
		{
			eReturn = EBaslerSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			if(PylonDeviceFeatureFromString(m_hDevice, "TriggerSelector", "LineStart") != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}

			if(!PylonDeviceFeatureIsWritable(m_hDevice, "TriggerActivation"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "TriggerActivation", CStringA(g_lpszBaslerGigEActivation[eParam])) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
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
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], g_lpszBaslerGigEActivation[nPreValue], g_lpszBaslerGigEActivation[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetAcquisitionStartTriggerActivation(EDeviceBaslerGigETriggerActivation* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(_T("AcquisitionStart Trigger Activation")));

		if(nData < 0 || nData >= (int)EDeviceBaslerGigETriggerActivation_Count)
		{
			eReturn = EBaslerGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaslerGigETriggerActivation)nData;

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetAcquisitionStartTriggerActivation(EDeviceBaslerGigETriggerActivation eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_AcquisitionStartTriggerActivation;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceBaslerGigETriggerActivation_Count)
		{
			eReturn = EBaslerSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			if(PylonDeviceFeatureFromString(m_hDevice, "TriggerSelector", "AcquisitionStart") != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}

			if(!PylonDeviceFeatureIsWritable(m_hDevice, "TriggerActivation"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "TriggerActivation", CStringA(g_lpszBaslerGigEActivation[eParam])) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
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
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], g_lpszBaslerGigEActivation[nPreValue], g_lpszBaslerGigEActivation[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetExposureAuto(EDeviceBaslerGigEAuto* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceBaslerGigEAuto)_ttoi(GetParamValue(EDeviceParameterBaslerGigE_ExposureAuto));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetExposureAuto(EDeviceBaslerGigEAuto eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_ExposureAuto;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceBaslerGigEAuto_Count)
		{
			eReturn = EBaslerSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "ExposureAuto"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "ExposureAuto", CStringA(g_lpszBaslerGigEAuto[eParam])) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
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
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], g_lpszBaslerGigEAuto[nPreValue], g_lpszBaslerGigEAuto[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetGainAuto(EDeviceBaslerGigEAuto* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}
		
		*pParam = (EDeviceBaslerGigEAuto)_ttoi(GetParamValue(EDeviceParameterBaslerGigE_GainAuto));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetGainAuto(EDeviceBaslerGigEAuto eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_GainAuto;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "GainAuto"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "GainAuto", CStringA(g_lpszBaslerGigEAuto[eParam])) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], g_lpszBaslerGigEAuto[nPreValue], g_lpszBaslerGigEAuto[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetGainSelector(EDeviceBaslerGigEGainSelector* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceBaslerGigEGainSelector)_ttoi(GetParamValue(EDeviceParameterBaslerGigE_GainSelector));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);
	
	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetGainSelector(EDeviceBaslerGigEGainSelector eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_GainSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceBaslerGigEGainSelector_Count)
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

			if(PylonDeviceFeatureFromString(m_hDevice, "GainSelector", CStringA(g_lpszBaslerGigEGainSelect[eParam])) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
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
			eReturn = EBaslerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], g_lpszBaslerGigEGainSelect[nPreValue], g_lpszBaslerGigEGainSelect[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetLineSelector(EDeviceBaslerGigELineSelector * pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceBaslerGigELineSelector)_ttoi(GetParamValue(EDeviceParameterBaslerGigE_LineSelector));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetLineSelector(EDeviceBaslerGigELineSelector eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_LineSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceBaslerGigELineSelector_Count)
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

			if(PylonDeviceFeatureFromString(m_hDevice, "LineSelector", CStringA(g_lpszBaslerGigELineSelector[eParam])) != S_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], g_lpszBaslerGigELineSelector[nPreValue], g_lpszBaslerGigELineSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetLine1Mode(EDeviceBaslerGigELineMode * pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceBaslerGigELineMode)_ttoi(GetParamValue(EDeviceParameterBaslerGigE_Line1Mode));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetLine1Mode(EDeviceBaslerGigELineMode eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_Line1Mode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceBaslerGigELineMode_Count)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		int nLineSelect = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_LineSelector));

		if(nLineSelect != EDeviceBaslerGigELineSelector_Line1)
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

			if(PylonDeviceFeatureFromString(m_hDevice, "LineMode", CStringA(g_lpszBaslerGigELineMode[eParam])) != S_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], g_lpszBaslerGigELineMode[nPreValue], g_lpszBaslerGigELineMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetLine1Source(EDeviceBaslerGigELineSource * pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceBaslerGigELineSource)_ttoi(GetParamValue(EDeviceParameterBaslerGigE_Line1Source));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetLine1Source(EDeviceBaslerGigELineSource eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_Line1Source;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceBaslerGigELineSource_Count)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		int nLineSelect = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_LineSelector));

		if(nLineSelect != EDeviceBaslerGigELineSelector_Line1)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		int nLineMode = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_Line1Mode));

		if(nLineMode != EDeviceBaslerGigELineMode_Output)
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

			if(PylonDeviceFeatureFromString(m_hDevice, "LineSource", CStringA(g_lpszBaslerGigELineSource[eParam])) != S_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], g_lpszBaslerGigELineSource[nPreValue], g_lpszBaslerGigELineSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetLine1Inverter(bool * pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_Line1Inverter));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetLine1Inverter(bool bParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_Line1Inverter;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		int nLineSelect = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_LineSelector));

		if(nLineSelect != EDeviceBaslerGigELineSelector_Line1)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], g_lpszBaslerGigESwitch[nPreValue], g_lpszBaslerGigESwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetLine1DebouncerTime(double * pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterBaslerGigE_Line1DebouncerTime_us));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetLine1DebouncerTime(double dblParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_Line1DebouncerTime_us;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		int nLineSelect = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_LineSelector));

		if(nLineSelect != EDeviceBaslerGigELineSelector_Line1)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		int nLineMode = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_Line1Mode));

		if(nLineMode != EDeviceBaslerGigELineMode_Input)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetLine1MinOutputPulseWidth(double * pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterBaslerGigE_Line1MinimumOuputPulseWidth_us));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetLine1MinOutputPulseWidth(double dblParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_Line1MinimumOuputPulseWidth_us;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		int nLineSelect = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_LineSelector));

		if(nLineSelect != EDeviceBaslerGigELineSelector_Line1)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		int nLineMode = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_Line1Mode));

		if(nLineMode != EDeviceBaslerGigELineMode_Output)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetLine2Mode(EDeviceBaslerGigELineMode * pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceBaslerGigELineMode)_ttoi(GetParamValue(EDeviceParameterBaslerGigE_Line2Mode));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetLine2Mode(EDeviceBaslerGigELineMode eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_Line2Mode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceBaslerGigELineMode_Count)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		int nLineSelect = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_LineSelector));

		if(nLineSelect != EDeviceBaslerGigELineSelector_Line2)
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

			if(PylonDeviceFeatureFromString(m_hDevice, "LineMode", CStringA(g_lpszBaslerGigELineMode[eParam])) != S_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], g_lpszBaslerGigELineMode[nPreValue], g_lpszBaslerGigELineMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetLine2Source(EDeviceBaslerGigELineSource * pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceBaslerGigELineSource)_ttoi(GetParamValue(EDeviceParameterBaslerGigE_Line2Source));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetLine2Source(EDeviceBaslerGigELineSource eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_Line2Source;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceBaslerGigELineSource_Count)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		int nLineSelect = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_LineSelector));

		if(nLineSelect != EDeviceBaslerGigELineSelector_Line2)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		int nLineMode = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_Line2Mode));

		if(nLineMode != EDeviceBaslerGigELineMode_Output)
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

			if(PylonDeviceFeatureFromString(m_hDevice, "LineSource", CStringA(g_lpszBaslerGigELineSource[eParam])) != S_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], g_lpszBaslerGigELineSource[nPreValue], g_lpszBaslerGigELineSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetLine2Inverter(bool * pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_Line2Inverter));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetLine2Inverter(bool bParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_Line2Inverter;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		int nLineSelect = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_LineSelector));

		if(nLineSelect != EDeviceBaslerGigELineSelector_Line2)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], g_lpszBaslerGigESwitch[nPreValue], g_lpszBaslerGigESwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetLine2DebouncerTime(double * pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterBaslerGigE_Line2DebouncerTime_us));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetLine2DebouncerTime(double dblParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_Line2DebouncerTime_us;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		int nLineSelect = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_LineSelector));

		if(nLineSelect != EDeviceBaslerGigELineSelector_Line2)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		int nLineMode = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_Line2Mode));

		if(nLineMode != EDeviceBaslerGigELineMode_Input)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetLine2MinOutputPulseWidth(double * pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterBaslerGigE_Line2MinimumOuputPulseWidth_us));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetLine2MinOutputPulseWidth(double dblParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_Line2MinimumOuputPulseWidth_us;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		int nLineSelect = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_LineSelector));

		if(nLineSelect != EDeviceBaslerGigELineSelector_Line2)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		int nLineMode = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_Line2Mode));

		if(nLineMode != EDeviceBaslerGigELineMode_Output)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetLine3Mode(EDeviceBaslerGigELineMode * pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceBaslerGigELineMode)_ttoi(GetParamValue(EDeviceParameterBaslerGigE_Line3Mode));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetLine3Mode(EDeviceBaslerGigELineMode eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_Line3Mode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceBaslerGigELineMode_Count)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		int nLineSelect = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_LineSelector));

		if(nLineSelect != EDeviceBaslerGigELineSelector_Line3)
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

			if(PylonDeviceFeatureFromString(m_hDevice, "LineMode", CStringA(g_lpszBaslerGigELineMode[eParam])) != S_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], g_lpszBaslerGigELineMode[nPreValue], g_lpszBaslerGigELineMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetLine3Source(EDeviceBaslerGigELineSource * pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceBaslerGigELineSource)_ttoi(GetParamValue(EDeviceParameterBaslerGigE_Line3Source));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetLine3Source(EDeviceBaslerGigELineSource eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_Line3Source;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceBaslerGigELineSource_Count)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		int nLineSelect = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_LineSelector));

		if(nLineSelect != EDeviceBaslerGigELineSelector_Line3)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		int nLineMode = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_Line3Mode));

		if(nLineMode != EDeviceBaslerGigELineMode_Output)
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

			if(PylonDeviceFeatureFromString(m_hDevice, "LineSource", CStringA(g_lpszBaslerGigELineSource[eParam])) != S_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], g_lpszBaslerGigELineSource[nPreValue], g_lpszBaslerGigELineSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetLine3Inverter(bool * pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_Line3Inverter));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetLine3Inverter(bool bParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_Line3Inverter;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		int nLineSelect = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_LineSelector));

		if(nLineSelect != EDeviceBaslerGigELineSelector_Line3)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], g_lpszBaslerGigESwitch[nPreValue], g_lpszBaslerGigESwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetLine3DebouncerTime(double * pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterBaslerGigE_Line3DebouncerTime_us));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetLine3DebouncerTime(double dblParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_Line3DebouncerTime_us;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		int nLineSelect = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_LineSelector));

		if(nLineSelect != EDeviceBaslerGigELineSelector_Line3)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		int nLineMode = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_Line3Mode));

		if(nLineMode != EDeviceBaslerGigELineMode_Input)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetLine3MinOutputPulseWidth(double * pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterBaslerGigE_Line3MinimumOuputPulseWidth_us));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetLine3MinOutputPulseWidth(double dblParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_Line3MinimumOuputPulseWidth_us;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		int nLineSelect = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_LineSelector));

		if(nLineSelect != EDeviceBaslerGigELineSelector_Line3)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		int nLineMode = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_Line3Mode));

		if(nLineMode != EDeviceBaslerGigELineMode_Output)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetLine4Mode(EDeviceBaslerGigELineMode * pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceBaslerGigELineMode)_ttoi(GetParamValue(EDeviceParameterBaslerGigE_Line4Mode));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetLine4Mode(EDeviceBaslerGigELineMode eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_Line4Mode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceBaslerGigELineMode_Count)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		int nLineSelect = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_LineSelector));

		if(nLineSelect != EDeviceBaslerGigELineSelector_Line4)
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

			if(PylonDeviceFeatureFromString(m_hDevice, "LineMode", CStringA(g_lpszBaslerGigELineMode[eParam])) != S_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], g_lpszBaslerGigELineMode[nPreValue], g_lpszBaslerGigELineMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetLine4Source(EDeviceBaslerGigELineSource * pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceBaslerGigELineSource)_ttoi(GetParamValue(EDeviceParameterBaslerGigE_Line4Source));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetLine4Source(EDeviceBaslerGigELineSource eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_Line4Source;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceBaslerGigELineSource_Count)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		int nLineSelect = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_LineSelector));

		if(nLineSelect != EDeviceBaslerGigELineSelector_Line4)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		int nLineMode = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_Line4Mode));

		if(nLineMode != EDeviceBaslerGigELineMode_Output)
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

			if(PylonDeviceFeatureFromString(m_hDevice, "LineSource", CStringA(g_lpszBaslerGigELineSource[eParam])) != S_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], g_lpszBaslerGigELineSource[nPreValue], g_lpszBaslerGigELineSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetLine4Inverter(bool * pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_Line4Inverter));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetLine4Inverter(bool bParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_Line4Inverter;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		int nLineSelect = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_LineSelector));

		if(nLineSelect != EDeviceBaslerGigELineSelector_Line4)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], g_lpszBaslerGigESwitch[nPreValue], g_lpszBaslerGigESwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetLine4DebouncerTime(double * pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterBaslerGigE_Line4DebouncerTime_us));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetLine4DebouncerTime(double dblParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_Line4DebouncerTime_us;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		int nLineSelect = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_LineSelector));

		if(nLineSelect != EDeviceBaslerGigELineSelector_Line4)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		int nLineMode = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_Line4Mode));

		if(nLineMode != EDeviceBaslerGigELineMode_Input)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetLine4MinOutputPulseWidth(double * pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterBaslerGigE_Line4MinimumOuputPulseWidth_us));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetLine4MinOutputPulseWidth(double dblParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_Line4MinimumOuputPulseWidth_us;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		int nLineSelect = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_LineSelector));

		if(nLineSelect != EDeviceBaslerGigELineSelector_Line4)
		{
			eReturn = EBaslerSetFunction_NotFindCommandError;
			break;
		}

		int nLineMode = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_Line4Mode));

		if(nLineMode != EDeviceBaslerGigELineMode_Output)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetLightSourceSelector(EDeviceBaslerGigELightSourceSelector* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceBaslerGigELightSourceSelector)_ttoi(GetParamValue(EDeviceParameterBaslerGigE_LightSourceSelector));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetLightSourceSelector(EDeviceBaslerGigELightSourceSelector eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_LightSourceSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "LightSourceSelector"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "LightSourceSelector", CStringA(g_lpszBaslerGigELightSourceSelector[eParam])) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}

			if(PylonDeviceFeatureIsWritable(m_hDevice, "BalanceRatioSelector"))
			{
				int nRaw = 0;
				CString strData;

				if(PylonDeviceFeatureFromString(m_hDevice, "BalanceRatioSelector", "Red") != S_OK)
				{
					eReturn = EBaslerSetFunction_WriteToDeviceError;
					break;
				}
				
				if(!GetCommandInt32("BalanceRatioRaw", &nRaw))
				{
					eReturn = EBaslerSetFunction_WriteToDeviceError;
					break;
				}

				strData.Format(_T("%d"), nRaw);

				if(!SetParamValue(EDeviceParameterBaslerGigE_BalanceRationRawRed, strData))
				{
					eReturn = EBaslerSetFunction_WriteToDatabaseError;
					break;
				}

				if(!SaveSettings(EDeviceParameterBaslerGigE_BalanceRationRawRed))
				{
					eReturn = EBaslerSetFunction_WriteToDatabaseError;
					break;
				}

				if(PylonDeviceFeatureFromString(m_hDevice, "BalanceRatioSelector", "Green") != S_OK)
				{
					eReturn = EBaslerSetFunction_WriteToDeviceError;
					break;
				}

				if(!GetCommandInt32("BalanceRatioRaw", &nRaw))
				{
					eReturn = EBaslerSetFunction_WriteToDeviceError;
					break;
				}

				strData.Format(_T("%d"), nRaw);

				if(!SetParamValue(EDeviceParameterBaslerGigE_BalanceRationRawGreen, strData))
				{
					eReturn = EBaslerSetFunction_WriteToDatabaseError;
					break;
				}

				if(!SaveSettings(EDeviceParameterBaslerGigE_BalanceRationRawGreen))
				{
					eReturn = EBaslerSetFunction_WriteToDatabaseError;
					break;
				}

				if(PylonDeviceFeatureFromString(m_hDevice, "BalanceRatioSelector", "Blue") != S_OK)
				{
					eReturn = EBaslerSetFunction_WriteToDeviceError;
					break;
				}

				if(!GetCommandInt32("BalanceRatioRaw", &nRaw))
				{
					eReturn = EBaslerSetFunction_WriteToDeviceError;
					break;
				}

				strData.Format(_T("%d"), nRaw);

				if(!SetParamValue(EDeviceParameterBaslerGigE_BalanceRationRawBlue, strData))
				{
					eReturn = EBaslerSetFunction_WriteToDatabaseError;
					break;
				}

				if(!SaveSettings(EDeviceParameterBaslerGigE_BalanceRationRawBlue))
				{
					eReturn = EBaslerSetFunction_WriteToDatabaseError;
					break;
				}
			}
		}

		CString strOffset;
		strOffset.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strOffset))
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], g_lpszBaslerGigELightSourceSelector[nPreValue], g_lpszBaslerGigELightSourceSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetBalanceWhiteReset(bool* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_BalanceWhiteReset));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetBalanceWhiteReset(bool bParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_BalanceWhiteReset;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PylonDeviceExecuteCommandFeature(m_hDevice, " BalanceWhiteReset") != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strOffset;
		strOffset.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strOffset))
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], g_lpszBaslerGigESwitch[nPreValue], g_lpszBaslerGigESwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetBalanceWhiteAuto(EDeviceBaslerGigEAuto* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceBaslerGigEAuto)_ttoi(GetParamValue(EDeviceParameterBaslerGigE_BalanceWhiteAuto));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetBalanceWhiteAuto(EDeviceBaslerGigEAuto eParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_BalanceWhiteAuto;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "BalanceWhiteAuto"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "BalanceWhiteAuto", CStringA(g_lpszBaslerGigEAuto[eParam])) != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strOffset;
		strOffset.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strOffset))
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], g_lpszBaslerGigEAuto[nPreValue], g_lpszBaslerGigEAuto[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetBalanceWhiteRawRed(int* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_BalanceRationRawRed));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetBalanceWhiteRawRed(int nParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_BalanceRationRawRed;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "BalanceRatioSelector"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "BalanceRatioSelector", "Red") != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}

			if(!SetCommandInt32("BalanceRatioRaw", nParam))
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetBalanceWhiteRawGreen(int* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_BalanceRationRawGreen));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetBalanceWhiteRawGreen(int nParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_BalanceRationRawGreen;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "BalanceRatioSelector"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "BalanceRatioSelector", "Green") != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}

			if(!SetCommandInt32("BalanceRatioRaw", nParam))
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetBalanceWhiteRawBlue(int* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_BalanceRationRawBlue));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetBalanceWhiteRawBlue(int nParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_BalanceRationRawBlue;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "BalanceRatioSelector"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "BalanceRatioSelector", "Blue") != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}

			if(!SetCommandInt32("BalanceRatioRaw", nParam))
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetColorTransRawGain00(int* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_ColorTransValueRawGain00));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetColorTransRawGain00(int nParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_ColorTransValueRawGain00;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "ColorTransformationSelector"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "ColorTransformationSelector", "RGBtoRGB") != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}

			if(!PylonDeviceFeatureIsWritable(m_hDevice, "ColorTransformationValueSelector"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "ColorTransformationValueSelector", "Gain00") != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}

			if(!SetCommandInt32("ColorTransformationValueRaw", nParam))
			{
				eReturn = EBaslerSetFunction_ReadOnDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetColorTransRawGain01(int* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_ColorTransValueRawGain01));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetColorTransRawGain01(int nParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_ColorTransValueRawGain01;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "ColorTransformationSelector"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "ColorTransformationSelector", "RGBtoRGB") != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}

			if(!PylonDeviceFeatureIsWritable(m_hDevice, "ColorTransformationValueSelector"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "ColorTransformationValueSelector", "Gain01") != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}

			if(!SetCommandInt32("ColorTransformationValueRaw", nParam))
			{
				eReturn = EBaslerSetFunction_ReadOnDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetColorTransRawGain02(int* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_ColorTransValueRawGain02));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetColorTransRawGain02(int nParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_ColorTransValueRawGain02;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "ColorTransformationSelector"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "ColorTransformationSelector", "RGBtoRGB") != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}

			if(!PylonDeviceFeatureIsWritable(m_hDevice, "ColorTransformationValueSelector"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "ColorTransformationValueSelector", "Gain02") != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}

			if(!SetCommandInt32("ColorTransformationValueRaw", nParam))
			{
				eReturn = EBaslerSetFunction_ReadOnDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetColorTransRawGain10(int* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_ColorTransValueRawGain10));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetColorTransRawGain10(int nParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_ColorTransValueRawGain10;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "ColorTransformationSelector"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "ColorTransformationSelector", "RGBtoRGB") != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}

			if(!PylonDeviceFeatureIsWritable(m_hDevice, "ColorTransformationValueSelector"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "ColorTransformationValueSelector", "Gain10") != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}

			if(!SetCommandInt32("ColorTransformationValueRaw", nParam))
			{
				eReturn = EBaslerSetFunction_ReadOnDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetColorTransRawGain11(int* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_ColorTransValueRawGain11));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetColorTransRawGain11(int nParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_ColorTransValueRawGain11;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "ColorTransformationSelector"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "ColorTransformationSelector", "RGBtoRGB") != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}

			if(!PylonDeviceFeatureIsWritable(m_hDevice, "ColorTransformationValueSelector"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "ColorTransformationValueSelector", "Gain11") != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}

			if(!SetCommandInt32("ColorTransformationValueRaw", nParam))
			{
				eReturn = EBaslerSetFunction_ReadOnDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetColorTransRawGain12(int* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_ColorTransValueRawGain12));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetColorTransRawGain12(int nParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_ColorTransValueRawGain12;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "ColorTransformationSelector"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "ColorTransformationSelector", "RGBtoRGB") != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}

			if(!PylonDeviceFeatureIsWritable(m_hDevice, "ColorTransformationValueSelector"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "ColorTransformationValueSelector", "Gain12") != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}

			if(!SetCommandInt32("ColorTransformationValueRaw", nParam))
			{
				eReturn = EBaslerSetFunction_ReadOnDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetColorTransRawGain20(int* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_ColorTransValueRawGain20));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetColorTransRawGain20(int nParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_ColorTransValueRawGain20;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "ColorTransformationSelector"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "ColorTransformationSelector", "RGBtoRGB") != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}

			if(!PylonDeviceFeatureIsWritable(m_hDevice, "ColorTransformationValueSelector"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "ColorTransformationValueSelector", "Gain20") != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}

			if(!SetCommandInt32("ColorTransformationValueRaw", nParam))
			{
				eReturn = EBaslerSetFunction_ReadOnDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetColorTransRawGain21(int* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_ColorTransValueRawGain21));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetColorTransRawGain21(int nParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_ColorTransValueRawGain21;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "ColorTransformationSelector"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "ColorTransformationSelector", "RGBtoRGB") != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}

			if(!PylonDeviceFeatureIsWritable(m_hDevice, "ColorTransformationValueSelector"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "ColorTransformationValueSelector", "Gain21") != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}

			if(!SetCommandInt32("ColorTransformationValueRaw", nParam))
			{
				eReturn = EBaslerSetFunction_ReadOnDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetColorTransRawGain22(int* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_ColorTransValueRawGain22));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetColorTransRawGain22(int nParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_ColorTransValueRawGain22;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(!PylonDeviceFeatureIsWritable(m_hDevice, "ColorTransformationSelector"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "ColorTransformationSelector", "RGBtoRGB") != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}

			if(!PylonDeviceFeatureIsWritable(m_hDevice, "ColorTransformationValueSelector"))
			{
				eReturn = EBaslerSetFunction_NotSupportError;
				break;
			}

			if(PylonDeviceFeatureFromString(m_hDevice, "ColorTransformationValueSelector", "Gain22") != S_OK)
			{
				eReturn = EBaslerSetFunction_WriteToDeviceError;
				break;
			}

			if(!SetCommandInt32("ColorTransformationValueRaw", nParam))
			{
				eReturn = EBaslerSetFunction_ReadOnDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerGetFunction CDeviceBaslerGigE::GetColorTransMatrixFactorRaw(int* pParam)
{
	EBaslerGetFunction eReturn = EBaslerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerGigE_ColorTransMatrixFactorRaw));

		eReturn = EBaslerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerSetFunction CDeviceBaslerGigE::SetColorTransMatrixFactorRaw(int nParam)
{
	EBaslerSetFunction eReturn = EBaslerSetFunction_UnknownError;

	EDeviceParameterBaslerGigE eSaveID = EDeviceParameterBaslerGigE_ColorTransMatrixFactorRaw;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(!SetCommandInt32("ColorTransformationMatrixFactorRaw", nParam))
			{
				eReturn = EBaslerSetFunction_ReadOnDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceBaslerGigE::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = false;
	bool bFoundID = true;

	do
	{
		switch(nParam)
		{
		case EDeviceParameterBaslerGigE_DeviceID:
			bReturn = !SetDeviceID(strValue);
			break;
		case EDeviceParameterBaslerGigE_GrabCount:
			bReturn = !SetGrabCount(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_CanvasWidth:
			bReturn = !SetCanvasWidth(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_CanvasHeight:
			bReturn = !SetCanvasHeight(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_OffsetCenterX:
			bReturn = !SetOffsetCenterX(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_OffsetCenterY:
			bReturn = !SetOffsetCenterY(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_OffsetX:
			bReturn = !SetOffsetX(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_OffsetY:
			bReturn = !SetOffsetY(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_GrabWaitingTime:
			bReturn = !SetGrabWaitingTime(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_PixelFormat:
			bReturn = !SetPixelFormat((EDeviceBaslerGigEPixelFormat)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_PacketSize:
			bReturn = !SetPacketSize(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_TriggerSelector:
			bReturn = !SetTriggerSelector((EDeviceBaslerGigETriggerSelector)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_FrameStartTriggerMode:
			bReturn = !SetFrameStartMode((EDeviceBaslerGigESwitch)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_FrameStartTriggerSource:
			bReturn = !SetFrameStartTriggerSource((EDeviceBaslerGigETriggerSource)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_FrameStartTriggerActivation:
			bReturn = !SetFrameStartTriggerActivation((EDeviceBaslerGigETriggerActivation)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_LineStartTriggerMode:
			bReturn = !SetLineStartMode((EDeviceBaslerGigESwitch)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_LineStartTriggerSource:
			bReturn = !SetLineStartTriggerSource((EDeviceBaslerGigETriggerSource)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_LineStartTriggerActivation:
			bReturn = !SetLineStartTriggerActivation((EDeviceBaslerGigETriggerActivation)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_AcquisitionStartTriggerMode:
			bReturn = !SetAcquisitionStartMode((EDeviceBaslerGigESwitch)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_AcquisitionStartTriggerSource:
			bReturn = !SetAcquisitionStartTriggerSource((EDeviceBaslerGigETriggerSource)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_AcquisitionStartTriggerActivation:
			bReturn = !SetAcquisitionStartTriggerActivation((EDeviceBaslerGigETriggerActivation)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_ExposureAuto:
			bReturn = !SetExposureAuto((EDeviceBaslerGigEAuto)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_Exposure:
			bReturn = !SetExposure(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_GainAuto:
			bReturn = !SetGainAuto((EDeviceBaslerGigEAuto)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_GainSelector:
			bReturn = !SetGainSelector((EDeviceBaslerGigEGainSelector)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_Gain:
			bReturn = !SetAllGain(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_AnalogGain:
			bReturn = !SetAnalogGain(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_DigitalGain:
			bReturn = !SetDigitalGain(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_Tap1Gain:
			bReturn = !SetTap1Gain(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_Tap2Gain:
			bReturn = !SetTap2Gain(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_LineSelector:
			bReturn = !SetLineSelector((EDeviceBaslerGigELineSelector)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_Line1Mode:
			bReturn = !SetLine1Mode((EDeviceBaslerGigELineMode)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_Line1Source:
			bReturn = !SetLine1Source((EDeviceBaslerGigELineSource)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_Line1Inverter:
			bReturn = !SetLine1Inverter(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_Line1DebouncerTime_us:
			bReturn = !SetLine1DebouncerTime(_ttof(strValue));
			break;
		case EDeviceParameterBaslerGigE_Line1MinimumOuputPulseWidth_us:
			bReturn = !SetLine1MinOutputPulseWidth(_ttof(strValue));
			break;
		case EDeviceParameterBaslerGigE_Line2Mode:
			bReturn = !SetLine2Mode((EDeviceBaslerGigELineMode)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_Line2Source:
			bReturn = !SetLine2Source((EDeviceBaslerGigELineSource)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_Line2Inverter:
			bReturn = !SetLine2Inverter(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_Line2DebouncerTime_us:
			bReturn = !SetLine2DebouncerTime(_ttof(strValue));
			break;
		case EDeviceParameterBaslerGigE_Line2MinimumOuputPulseWidth_us:
			bReturn = !SetLine2MinOutputPulseWidth(_ttof(strValue));
			break;
		case EDeviceParameterBaslerGigE_Line3Mode:
			bReturn = !SetLine3Mode((EDeviceBaslerGigELineMode)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_Line3Source:
			bReturn = !SetLine3Source((EDeviceBaslerGigELineSource)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_Line3Inverter:
			bReturn = !SetLine3Inverter(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_Line3DebouncerTime_us:
			bReturn = !SetLine3DebouncerTime(_ttof(strValue));
			break;
		case EDeviceParameterBaslerGigE_Line3MinimumOuputPulseWidth_us:
			bReturn = !SetLine3MinOutputPulseWidth(_ttof(strValue));
			break;
		case EDeviceParameterBaslerGigE_Line4Mode:
			bReturn = !SetLine4Mode((EDeviceBaslerGigELineMode)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_Line4Source:
			bReturn = !SetLine4Source((EDeviceBaslerGigELineSource)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_Line4Inverter:
			bReturn = !SetLine4Inverter(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_Line4DebouncerTime_us:
			bReturn = !SetLine4DebouncerTime(_ttof(strValue));			
			break;
		case EDeviceParameterBaslerGigE_Line4MinimumOuputPulseWidth_us:
			bReturn = !SetLine4MinOutputPulseWidth(_ttof(strValue));
			break;
		case EDeviceParameterBaslerGigE_LightSourceSelector:
			bReturn = !SetLightSourceSelector((EDeviceBaslerGigELightSourceSelector)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_BalanceWhiteReset:
			bReturn = !SetBalanceWhiteReset(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_BalanceWhiteAuto:
			bReturn = !SetBalanceWhiteAuto((EDeviceBaslerGigEAuto)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_BalanceRationRawRed:
			bReturn = !SetBalanceWhiteRawRed(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_BalanceRationRawGreen:
			bReturn = !SetBalanceWhiteRawGreen(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_BalanceRationRawBlue:
			bReturn = !SetBalanceWhiteRawBlue(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_ColorTransValueRawGain00:
			bReturn = !SetColorTransRawGain00(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_ColorTransValueRawGain01:
			bReturn = !SetColorTransRawGain01(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_ColorTransValueRawGain02:
			bReturn = !SetColorTransRawGain02(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_ColorTransValueRawGain10:
			bReturn = !SetColorTransRawGain10(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_ColorTransValueRawGain11:
			bReturn = !SetColorTransRawGain11(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_ColorTransValueRawGain12:
			bReturn = !SetColorTransRawGain12(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_ColorTransValueRawGain20:
			bReturn = !SetColorTransRawGain20(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_ColorTransValueRawGain21:
			bReturn = !SetColorTransRawGain21(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_ColorTransValueRawGain22:
			bReturn = !SetColorTransRawGain22(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerGigE_ColorTransMatrixFactorRaw:
			bReturn = !SetColorTransMatrixFactorRaw(_ttoi(strValue));
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

bool CDeviceBaslerGigE::DoesModuleExist()
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

bool CDeviceBaslerGigE::GetCommandBool(const char * pCommnadString, bool * pGetValue)
{
	bool bReturn = false;

	do
	{
		if(!m_hDevice)
			break;

		if(!pCommnadString)
			break;

		if(PylonDeviceGetBooleanFeature(m_hDevice, pCommnadString, pGetValue) != S_OK)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceBaslerGigE::SetCommandBool(const char * pCommnadString, bool bSetValue)
{
	bool bReturn = false;

	do
	{
		if(!m_hDevice)
			break;

		if(!pCommnadString)
			break;

		if(PylonDeviceSetBooleanFeature(m_hDevice, pCommnadString, bSetValue) != S_OK)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceBaslerGigE::GetCommandInt32(const char * pCommnadString, int32_t* pGetValue)
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

bool CDeviceBaslerGigE::SetCommandInt32(const char * pCommnadString, int32_t nSetValue)
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

bool CDeviceBaslerGigE::GetCommandInt64(const char * pCommnadString, int64_t* pGetValue)
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

bool CDeviceBaslerGigE::SetCommandInt64(const char * pCommnadString, int64_t nSetValue)
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

bool CDeviceBaslerGigE::GetCommandFloat(const char * pCommnadString, double * pGetValue)
{
	bool bReturn = false;

	do
	{
		if(!m_hDevice)
			break;

		if(!pCommnadString)
			break;

		if(PylonDeviceGetFloatFeature(m_hDevice, pCommnadString, pGetValue) != S_OK)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceBaslerGigE::SetCommandFloat(const char * pCommnadString, double dblSetValue)
{
	bool bReturn = false;

	do
	{
		if(!m_hDevice)
			break;

		if(!pCommnadString)
			break;

		if(PylonDeviceSetFloatFeature(m_hDevice, pCommnadString, dblSetValue) != S_OK)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

UINT CDeviceBaslerGigE::CallbackFunction(LPVOID pParam)
{
	CDeviceBaslerGigE* pInstance = (CDeviceBaslerGigE*)pParam;

	if(pInstance)
	{
		BYTE* pDstImg = nullptr;

		do
		{
			int nSize = 0;

			if(PylonDeviceGetIntegerFeatureInt32(pInstance->m_hDevice, "PayloadSize", &nSize) != GENAPI_E_OK)
			{
				pInstance->SetMessage(_T("Failed to get payload size"));
				break;
			}

			size_t nStreams = 0;

			if(PylonDeviceGetNumStreamGrabberChannels(pInstance->m_hDevice, &nStreams) != GENAPI_E_OK)
				break;

			if(nStreams < 1)
				break;

			if(PylonDeviceGetStreamGrabber(pInstance->m_hDevice, 0, &pInstance->m_hGrabber) != GENAPI_E_OK)
				break;

			if(PylonStreamGrabberOpen(pInstance->m_hGrabber) != GENAPI_E_OK)
				break;

			if(PylonStreamGrabberGetWaitObject(pInstance->m_hGrabber, &pInstance->m_hWait) != GENAPI_E_OK)
				break;

			if(PylonStreamGrabberSetMaxNumBuffer(pInstance->m_hGrabber, 1) != GENAPI_E_OK)
				break;

			if(PylonStreamGrabberSetMaxBufferSize(pInstance->m_hGrabber, nSize) != GENAPI_E_OK)
				break;

			if(PylonStreamGrabberPrepareGrab(pInstance->m_hGrabber) != GENAPI_E_OK)
				break;
			
			pDstImg = new BYTE[nSize];

			if(PylonStreamGrabberRegisterBuffer(pInstance->m_hGrabber, pDstImg, nSize, &pInstance->m_hGrabBuffer) != GENAPI_E_OK)
				break;
			
			if(PylonStreamGrabberQueueBuffer(pInstance->m_hGrabber, pInstance->m_hGrabBuffer, (void*)0) != GENAPI_E_OK)
				break;
			
			if(PylonDeviceExecuteCommandFeature(pInstance->m_hDevice, "AcquisitionStart") != GENAPI_E_OK)
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
						for(int64_t i = 0; i < i64Height; ++i)
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
							if(PylonImageFormatConverterSetOutputPixelFormat(hFormat, PixelType_BGR8packed) == S_OK)
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

			if(PylonStreamGrabberCancelGrab(pInstance->m_hGrabber)!= S_OK)
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
