#include "stdafx.h"

#include "DeviceDahuaGigE.h"

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

#include "../Libraries/Includes/Dahua/SDK.h"

// mvsdkmd.dll
#pragma comment(lib, COMMONLIB_PREFIX "Dahua/MVSDKmd.lib")

using namespace Ravid;
using namespace Ravid::Device;
using namespace Ravid::Database;
using namespace Ravid::Framework;
using namespace Ravid::Algorithms;
using namespace Ravid::Miscellaneous;

IMPLEMENT_DYNAMIC(CDeviceDahuaGigE, CDeviceCamera)

BEGIN_MESSAGE_MAP(CDeviceDahuaGigE, CDeviceCamera)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static LPCTSTR g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_Count] =
{
	_T("DeviceID"),
	_T("Grab Count"),
	_T("Image Format Control"),
		_T("Canvas Width"),
		_T("Canvas Height"),
		_T("Offset X"),
		_T("Offset Y"),
		_T("Reverse X"),
		_T("Reverse Y"),
		_T("Pixel Format"),

	_T("Acquisition Control"),
		_T("Acquisition Mode"),
		_T("Acquisition Frame Count"),
		_T("Acquisition Frame Rate"),
		_T("Acquisition Frame RateEnable"),
		_T("Acquisition Status Selector"),
		_T("Trigger Selector"),

		_T("FrameStart"),
			_T("FrameStart Trigger Mode"),
			_T("FrameStart Trigger Source"),
			_T("FrameStart Trigger Activation"),
			_T("FrameStart Trigger Delay"),

		_T("AcquisitionStart"),
			_T("AcquisitionStart Trigger Mode"),
			_T("AcquisitionStart Trigger Source"),
			_T("AcquisitionStart Trigger Activation"),
			_T("AcquisitionStart Trigger Delay"),

		_T("Light Trigger Delay"),
		_T("Exposure Mode"),
		_T("Exposure Time"),
		_T("Exposure Auto"),

	_T("Analog Control"),
		_T("Gain Selector"),
		_T("AllGain Raw"),
		_T("Black Level Selector"),
		_T("Black Level"),
		_T("Black Level Auto"),
		_T("Gamma"),

	_T("Digital IO Control"),
		_T("Line Selector"),

		_T("Line0"),
			_T("Line0 Inverter"),
			_T("Line0 Source"),
			_T("Line0 Format"),

		_T("Line1"),
			_T("Line1 Inverter"),
			_T("Line1 Source"),
			_T("Line1 Format"),	
};

static LPCTSTR g_lpszPixelFormat[EDeviceDahuaGigEPixelFormat_Count]
{
	_T("Mono8"),
	_T("Mono10"),
	_T("Mono10Packed"),
};

static LPCTSTR g_lpszAcquisitionMode[EDeviceDahuaGigEAcquisitionMode_Count]
{
	_T("Continuous"),
	_T("SingleFrame"),
	_T("MultiFrame"),
};

static LPCTSTR g_lpszAcquisitionSelector[EDeviceDahuaGigEAcquisitionSelector_Count]
{
	_T("AcquisitionTriggerWait"),
	_T("FrameTriggerWait"),
};

static LPCTSTR g_lpszTriggerSelector[EDeviceDahuaGigETriggerSelector_Count]
{
	_T("AcquisitionStart"),
	_T("FrameStart"),
};

static LPCTSTR g_lpszTriggerMode[EDeviceDahuaGigETriggerMode_Count]
{
	_T("Off"),
	_T("On"),
};

static LPCTSTR g_lpszTriggerSource[EDeviceDahuaGigETriggerSource_Count]
{
	_T("Software"),
	_T("Line1"),
};

static LPCTSTR g_lpszTriggerActivation[EDeviceDahuaGigETriggerActivation_Count]
{
	_T("RisingEdge"),
	_T("FallingEdge"),
};

static LPCTSTR g_lpszExposureMode[EDeviceDahuaGigEExposureMode_Count]
{
	_T("Timed"),
};

static LPCTSTR g_lpszExposureAuto[EDeviceDahuaGigEExposureAuto_Count]
{
	_T("Off"),
	_T("Once"),
	_T("Continuous"),
};

static LPCTSTR g_lpszGainSelector[EDeviceDahuaGigEGainSelector_Count]
{
	_T("All"),
};

static LPCTSTR g_lpszBlackSelector[EDeviceDahuaGigEBlackLevelSelector_Count]
{
	_T("All"),
};

static LPCTSTR g_lpszBlackLevelAuto[EDeviceDahuaGigEBlackLevelAuto_Count]
{
	_T("Off"),
	_T("Once"),
	_T("Continuous"),
};

static LPCTSTR g_lpszLineSelector[EDeviceDahuaGigELineSelector_Count]
{
	_T("Line0"),
	_T("Line1"),
};

static LPCTSTR g_lpszLineInverter[EDeviceDahuaGigELineInverter_Count]
{
	_T("False"),
	_T("True"),
};

static LPCTSTR g_lpszLineSource[EDeviceDahuaGigELineSource_Count]
{
	_T("ExposureActive"),
	_T("FrameTriggerWait"),
	_T("Timer0Active"),
	_T("UserOutput0"),
	_T("AcquisitionTriggerWait"),
	_T("LightTrigger"),
};

static LPCTSTR g_lpszLineFormat[EDeviceDahuaGigELineFormat_Count]
{
	_T("OptoCoupled"),
};

static LPCTSTR g_lpszSwitch[EDeviceDahuaGigESwitch_Count]
{
	_T("Off"),
	_T("On"),
};




CDeviceDahuaGigE::CDeviceDahuaGigE()
{
}


CDeviceDahuaGigE::~CDeviceDahuaGigE()
{
	Terminate();
}

EDeviceInitializeResult CDeviceDahuaGigE::Initialize()
{
	EDeviceInitializeResult eReturn = EDeviceInitializeResult_UnknownError;

	CString strMessage, strStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("Dahua"));

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
		
		uint32_t cameraCnt = 0;

		GENICAM_System* pSystem = NULL;
		GENICAM_Camera* pCameraList = NULL;
		
		if(GENICAM_getSystemInstance(&pSystem))
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntopenthelibrary);
			eReturn = EDeviceInitializeResult_NotFoundDeviceError;
			break;
		}

		if(pSystem->discovery(pSystem, &pCameraList, &cameraCnt, typeGige))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Device list"));
			eReturn = EDeviceInitializeResult_NotFoundDeviceError;
			break;
		}

		if(cameraCnt < 1)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtofindthedevice);
			eReturn = EDeviceInitializeResult_NotFoundDeviceError;
			break;
		}
		
		bool bFindDevice = false;

		for(int i = 0; i < (int)cameraCnt; ++i)
		{
			m_pCamera = &pCameraList[i];

			CString strSerial(m_pCamera->getSerialNumber(m_pCamera));

			if(strSerial.CompareNoCase(GetDeviceID()))
				continue;

			bFindDevice = true;

			break;
		}

		if(!bFindDevice)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_CouldntfindthesameSN);
			eReturn = EDeviceInitializeResult_NotFoundSerialNumber;
			break;
		}

		if(m_pCamera->connect(m_pCamera, accessPermissionControl))
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtogeneratethedevicehandle);
			eReturn = EDeviceInitializeResult_DeviceHandleFailedGenerate;
			break;
		}

		int nWidth = 0;

		if(GetCanvasWidth(&nWidth))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Width"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(!nWidth)
		{
			if(!GetDeviceValueInt(_T("WidthMax"), &nWidth))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Width"));
				eReturn = EDeviceInitializeResult_ReadOnDeviceError;
				break;
			}

			if(SetCanvasWidth(nWidth))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Width"));
				eReturn = EDeviceInitializeResult_WriteToDatabaseError;
				break;
			}
		}

		if(!SetDeviceValueInt(_T("Width"), nWidth))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Width"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		int nHeight = 0;

		if(GetCanvasHeight(&nHeight))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Height"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(!nHeight)
		{
			if(!GetDeviceValueInt(_T("HeightMax"), &nHeight))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Height"));
				eReturn = EDeviceInitializeResult_ReadOnDeviceError;
				break;
			}

			if(SetCanvasHeight(nHeight))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Height"));
				eReturn = EDeviceInitializeResult_WriteToDatabaseError;
				break;
			}
		}

		if(!SetDeviceValueInt(_T("Height"), nHeight))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Height"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		int nOffsetX = 0;

		if(GetOffsetX(&nOffsetX))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("OffsetX"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(nOffsetX)
		{
			if(!SetDeviceValueInt(_T("OffsetX"), nOffsetX))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("OffsetX"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}
		}

		int nOffsetY = 0;

		if(GetOffsetY(&nOffsetY))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("OffsetY"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(nOffsetY)
		{
			if(!SetDeviceValueInt(_T("OffsetY"), nOffsetY))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("OffsetY"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}
		}

		bool bReverseX = false;

		if(GetReverseX(&bReverseX))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ReverseX"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(!SetDeviceValueBool(_T("ReverseX"), bReverseX))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ReverseX"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		bool bReverseY = false;

		if(GetReverseY(&bReverseY))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ReverseY"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(!SetDeviceValueBool(_T("ReverseY"), bReverseY))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ReverseY"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}
		
		EDeviceDahuaGigEPixelFormat ePixelFormat = EDeviceDahuaGigEPixelFormat_Count;
		
		if(GetPixelFormat(&ePixelFormat))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("PixelFormat"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(!SetDeviceValueEnum(_T("PixelFormat"), g_lpszPixelFormat[ePixelFormat]))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("PixelFormat"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}
		
		EDeviceDahuaGigEAcquisitionMode eAcquisitionMode = EDeviceDahuaGigEAcquisitionMode_Count;
		
		if(GetAcquisitionMode(&eAcquisitionMode))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AcquisitionMode"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(!SetDeviceValueEnum(_T("AcquisitionMode"), g_lpszAcquisitionMode[eAcquisitionMode]))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("AcquisitionMode"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		int nAcquisitionFrameCount = 0;

		if(GetAcquisitionFrameCount(&nAcquisitionFrameCount))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AcquisitionFrameCount"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(!SetDeviceValueInt(_T("AcquisitionFrameCount"), nAcquisitionFrameCount))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("AcquisitionFrameCount"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		float fAcquisitionFrameRate = 0.;

		if(GetAcquisitionFrameRate(&fAcquisitionFrameRate))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AcquisitionFrameRate"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(!SetDeviceValueDouble(_T("AcquisitionFrameRate"), fAcquisitionFrameRate))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("AcquisitionFrameRate"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		bool bAcquisitionFrameRateEnable = false;

		if(GetAcquisitionFrameRateEnable(&bAcquisitionFrameRateEnable))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AcquisitionFrameRateEnable"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(!SetDeviceValueBool(_T("AcquisitionFrameRateEnable"), bAcquisitionFrameRateEnable))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("AcquisitionFrameRateEnable"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		EDeviceDahuaGigEAcquisitionSelector eAcquisitionStatusSelector = EDeviceDahuaGigEAcquisitionSelector_Count;

		if(GetAcquisitionStatusSelector(&eAcquisitionStatusSelector))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AcquisitionStatusSelector"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}
		
		if(!SetDeviceValueEnum(_T("AcquisitionStatusSelector"), g_lpszAcquisitionSelector[eAcquisitionMode]))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("AcquisitionStatusSelector"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}


		if(SetDeviceValueEnum(_T("TriggerSelector"), g_lpszTriggerSelector[EDeviceDahuaGigETriggerSelector_FrameStart]))
		{
			EDeviceDahuaGigETriggerMode eTriggerMode = EDeviceDahuaGigETriggerMode_Count;

			if(GetFrameStartTriggerMode(&eTriggerMode))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("FrameStartTriggerMode"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(!SetDeviceValueEnum(_T("TriggerMode"), g_lpszTriggerMode[eTriggerMode]))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("FrameStartTriggerMode"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}

			EDeviceDahuaGigETriggerSource eTriggerSource = EDeviceDahuaGigETriggerSource_Count;

			if(GetFrameStartTriggerSource(&eTriggerSource))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("FrameStartTriggerSource"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(!SetDeviceValueEnum(_T("TriggerSource"), g_lpszTriggerSource[eTriggerSource]))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("FrameStartTriggerSource"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}

			if(eTriggerSource != EDeviceDahuaGigETriggerSource_Software)
			{
				EDeviceDahuaGigETriggerActivation eTriggerActivation = EDeviceDahuaGigETriggerActivation_Count;

				if(GetFrameStartTriggerActivation(&eTriggerActivation))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("FrameStartTriggerActivation"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(!SetDeviceValueEnum(_T("TriggerActivation"), g_lpszTriggerActivation[eTriggerActivation]))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("FrameStartTriggerActivation"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}
			}

			float fTriggerDelay = 0.;

			if(GetFrameStartTriggerDelay(&fTriggerDelay))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("FrameStartTriggerDelay"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(!SetDeviceValueDouble(_T("TriggerDelay"), fTriggerDelay))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("FrameStartTriggerDelay"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}
		}

		if(SetDeviceValueEnum(_T("TriggerSelector"), g_lpszTriggerSelector[EDeviceDahuaGigETriggerSelector_AcquisitionStart]))
		{
			EDeviceDahuaGigETriggerMode eTriggerMode = EDeviceDahuaGigETriggerMode_Count;

			if(GetAcquisitionStartTriggerMode(&eTriggerMode))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AcquisitionStartTriggerMode"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(!SetDeviceValueEnum(_T("TriggerMode"), g_lpszTriggerMode[eTriggerMode]))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("AcquisitionStartTriggerMode"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}

			EDeviceDahuaGigETriggerSource eTriggerSource = EDeviceDahuaGigETriggerSource_Count;

			if(GetAcquisitionStartTriggerSource(&eTriggerSource))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AcquisitionStartTriggerSource"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(!SetDeviceValueEnum(_T("TriggerSource"), g_lpszTriggerSource[eTriggerSource]))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("AcquisitionStartTriggerSource"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}

			if(eTriggerSource != EDeviceDahuaGigETriggerSource_Software)
			{
				EDeviceDahuaGigETriggerActivation eTriggerActivation = EDeviceDahuaGigETriggerActivation_Count;

				if(GetAcquisitionStartTriggerActivation(&eTriggerActivation))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AcquisitionStartTriggerActivation"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(!SetDeviceValueEnum(_T("TriggerActivation"), g_lpszTriggerActivation[eTriggerActivation]))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("AcquisitionStartTriggerActivation"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}
			}

			float fTriggerDelay = 0.;

			if(GetAcquisitionStartTriggerDelay(&fTriggerDelay))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AcquisitionStartTriggerDelay"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(!SetDeviceValueDouble(_T("TriggerDelay"), fTriggerDelay))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("AcquisitionStartTriggerDelay"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}
		}

		EDeviceDahuaGigETriggerSelector eTriggerSelector = EDeviceDahuaGigETriggerSelector_Count;

		if(GetTriggerSelector(&eTriggerSelector))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TriggerSelector"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(!SetDeviceValueEnum(_T("TriggerSelector"), g_lpszTriggerSelector[eTriggerSelector]))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TriggerSelector"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		float fLightTriggerDelay = 0.;

		if(GetLightTriggerDelay(&fLightTriggerDelay))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LightTriggerDelay"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(!SetDeviceValueDouble(_T("LightTriggerDelay"), fLightTriggerDelay))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LightTriggerDelay"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		EDeviceDahuaGigEExposureMode eExposureMode = EDeviceDahuaGigEExposureMode_Count;

		if(GetExposureMode(&eExposureMode))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ExposureMode"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(!SetDeviceValueEnum(_T("ExposureMode"), g_lpszExposureMode[eExposureMode]))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ExposureMode"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		if(eExposureMode == EDeviceDahuaGigEExposureMode_Timed)
		{
			EDeviceDahuaGigEExposureAuto eExposureAuto = EDeviceDahuaGigEExposureAuto_Count;

			if(GetExposureAuto(&eExposureAuto))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ExposureAuto"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(!SetDeviceValueEnum(_T("ExposureAuto"), g_lpszExposureAuto[eExposureAuto]))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ExposureAuto"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}

			if(eExposureAuto == EDeviceDahuaGigEExposureAuto_Off)
			{
				float fExposureTime = 0.;

				if(GetExposureTime(&fExposureTime))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ExposureTime"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(!SetDeviceValueDouble(_T("ExposureTime"), fExposureTime))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ExposureTime"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}
			}
		}
		
		EDeviceDahuaGigEGainSelector eGainSelector = EDeviceDahuaGigEGainSelector_Count;

		if(GetGainSelector(&eGainSelector))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("GainSelector"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(!SetDeviceValueEnum(_T("GainSelector"), g_lpszGainSelector[eGainSelector]))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("GainSelector"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		float fAllGainRaw = 0.;

		if(GetAllGainRaw(&fAllGainRaw))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AllGainRaw"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(!SetDeviceValueDouble(_T("GainRaw"), fAllGainRaw))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("AllGainRaw"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		EDeviceDahuaGigEBlackLevelSelector eBlackLevelSelector = EDeviceDahuaGigEBlackLevelSelector_Count;

		if(GetBlackLevelSelector(&eBlackLevelSelector))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("BlackLevelSelector"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(!SetDeviceValueEnum(_T("BlackLevelSelector"), g_lpszBlackSelector[eBlackLevelSelector]))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("BlackLevelSelector"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		EDeviceDahuaGigEBlackLevelAuto eBlackLevelAuto = EDeviceDahuaGigEBlackLevelAuto_Count;

		if(GetBlackLevelAuto(&eBlackLevelAuto))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("BlackLevelAuto"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(!SetDeviceValueEnum(_T("BlackLevelAuto"), g_lpszBlackLevelAuto[eBlackLevelAuto]))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("BlackLevelAuto"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		if(eBlackLevelAuto == EDeviceDahuaGigEBlackLevelAuto_Off)
		{
			int nBlackLevel = 0;

			if(GetBlackLevel(&nBlackLevel))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("BlackLevel"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(!SetDeviceValueInt(_T("BlackLevel"), nBlackLevel))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("BlackLevel"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}
		}

		float fGamma = 0.;

		if(GetGamma(&fGamma))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Gamma"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(!SetDeviceValueDouble(_T("Gamma"), fGamma))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Gamma"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		if(SetDeviceValueEnum(_T("LineSelector"), g_lpszLineSelector[EDeviceDahuaGigELineSelector_Line0]))
		{
			EDeviceDahuaGigELineInverter eLineInverter = EDeviceDahuaGigELineInverter_Count;

			if(GetLine0Inverter(&eLineInverter))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line0Inverter"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(!SetDeviceValueBool(_T("LineInverter"), eLineInverter))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Line0Inverter"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}

			CString strLineMode;

			if(!GetDeviceValueEnum(_T("LineMode"), &strLineMode))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("LineMode"));
				eReturn = EDeviceInitializeResult_ReadOnDeviceError;
				break;
			}

			if(!strLineMode.CompareNoCase(_T("Output")))
			{
				EDeviceDahuaGigELineSource eLineSource = EDeviceDahuaGigELineSource_Count;

				if(GetLine0Source(&eLineSource))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line0Source"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(!SetDeviceValueEnum(_T("LineSource"), g_lpszLineSource[eLineSource]))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Line0Source"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}
			}

			EDeviceDahuaGigELineFormat eLineFormat = EDeviceDahuaGigELineFormat_Count;

			if(GetLine0Format(&eLineFormat))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line0Format"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(!SetDeviceValueEnum(_T("LineFormat"), g_lpszLineFormat[eLineFormat]))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Line0Format"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}
		}

		if(SetDeviceValueEnum(_T("LineSelector"), g_lpszLineSelector[EDeviceDahuaGigELineSelector_Line1]))
		{
			EDeviceDahuaGigELineInverter eLineInverter = EDeviceDahuaGigELineInverter_Count;

			if(GetLine1Inverter(&eLineInverter))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line1Inverter"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(!SetDeviceValueBool(_T("LineInverter"), eLineInverter))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Line1Inverter"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}

			CString strLineMode;

			if(!GetDeviceValueEnum(_T("LineMode"), &strLineMode))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Line1Mode"));
				eReturn = EDeviceInitializeResult_ReadOnDeviceError;
				break;
			}

			if(!strLineMode.CompareNoCase(_T("Output")))
			{
				EDeviceDahuaGigELineSource eLineSource = EDeviceDahuaGigELineSource_Count;

				if(GetLine1Source(&eLineSource))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line1Source"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(!SetDeviceValueEnum(_T("LineSource"), g_lpszLineSource[eLineSource]))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Line1Source"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				EDeviceDahuaGigELineFormat eLineFormat = EDeviceDahuaGigELineFormat_Count;

				if(GetLine1Format(&eLineFormat))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line1Format"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(!SetDeviceValueEnum(_T("LineFormat"), g_lpszLineFormat[eLineFormat]))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Line1Format"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}
			}
			else if(!strLineMode.CompareNoCase(_T("Input")))
			{
				EDeviceDahuaGigELineFormat eLineFormat = EDeviceDahuaGigELineFormat_Count;

				if(GetLine1Format(&eLineFormat))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line1Format"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(!SetDeviceValueEnum(_T("LineFormat"), g_lpszLineFormat[eLineFormat]))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Line1Format"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}
			}
		}
		
		EDeviceDahuaGigELineSelector eLineSelector = EDeviceDahuaGigELineSelector_Count;

		if(GetLineSelector(&eLineSelector))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineSelector"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(!SetDeviceValueEnum(_T("LineSelector"), g_lpszLineSelector[eLineSelector]))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineSelector"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		GENICAM_StreamSourceInfo stStreamSourceInfo;

		stStreamSourceInfo.channelId = 0;
		stStreamSourceInfo.pCamera = m_pCamera;

		if(GENICAM_createStreamSource(&stStreamSourceInfo, &m_pStreamSource))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("stream source"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		if(NULL == m_pStreamSource)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("stream source"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}
		
		int nDepth = 8;
		int nFillValue = 255;
		int nWidthStep = nWidth;

		switch(ePixelFormat)
		{
		case EDeviceDahuaGigEPixelFormat_Mono8:
			{
				nDepth = 8;
				nFillValue = (1 << 8) - 1;
				nWidthStep = nWidth;
			}
			break;
		case EDeviceDahuaGigEPixelFormat_Mono10:
		case EDeviceDahuaGigEPixelFormat_Mono10Packed:
			{
				nDepth = 10;
				nFillValue = (1 << 10) - 1;
				nWidthStep = nWidth * 2;
			}
			break;
		default:
			break;
		}

		int nChannel = 1;

		CMultipleVariable mv;
		for(int i = 0; i < nChannel; ++i)
			mv.AddValue(nFillValue);

		this->InitBuffer(nWidth, nHeight, mv, CRavidImage::MakeValueFormat(nChannel, nDepth), nWidthStep);
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

EDeviceTerminateResult CDeviceDahuaGigE::Terminate()
{
	EDeviceTerminateResult eReturn = EDeviceTerminateResult_UnknownError;

	CString strMessage;

	SetStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("Dahua"));
		
		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);

		return EDeviceTerminateResult_NotFoundApiError;
	}

	do
	{
		if(m_pCamera)
		{
			if(IsLive() || !IsGrabAvailable())
				Stop();

			if(m_pCamera)
				m_pCamera->disConnect(m_pCamera);

			m_pCamera = nullptr;
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

bool CDeviceDahuaGigE::LoadSettings()
{
	bool bReturn = false;

	m_vctParameterFieldConfigurations.clear();

	do
	{
		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_DeviceID, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_DeviceID], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_GrabCount, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_GrabCount], _T("1"), EParameterFieldType_Edit, nullptr, nullptr, 0);

		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_ImageFormatControl, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_ImageFormatControl], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);

		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_CanvasWidth, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_CanvasWidth], _T("0"), EParameterFieldType_Edit, nullptr, _T("integer"), 1);
		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_CanvasHeight, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_CanvasHeight], _T("0"), EParameterFieldType_Edit, nullptr, _T("integer"), 1);
		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_OffsetX, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_OffsetX], _T("0"), EParameterFieldType_Edit, nullptr, _T("integer"), 1);
		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_OffsetY, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_OffsetY], _T("0"), EParameterFieldType_Edit, nullptr, _T("integer"), 1);
		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_ReverseX, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_ReverseX], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_ReverseY, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_ReverseY], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_PixelFormat, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_PixelFormat], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszPixelFormat, EDeviceDahuaGigEPixelFormat_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_AcquisitionControl, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_AcquisitionControl], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);

		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_AcquisitionMode, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_AcquisitionMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAcquisitionMode, EDeviceDahuaGigEAcquisitionMode_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_AcquisitionFrameCount, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_AcquisitionFrameCount], _T("1"), EParameterFieldType_Edit, nullptr, _T("integer"), 1);
		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_AcquisitionFrameRate, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_AcquisitionFrameRate], _T("75.000000"), EParameterFieldType_Edit, nullptr, _T("float"), 1);
		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_AcquisitionFrameRateEnable, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_AcquisitionFrameRateEnable], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_AcquisitionStatusSelector, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_AcquisitionStatusSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAcquisitionSelector, EDeviceDahuaGigEAcquisitionSelector_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_TriggerSelector, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_TriggerSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszTriggerSelector, EDeviceDahuaGigETriggerSelector_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_FrameStart, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_FrameStart], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_FrameStartTriggerMode, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_FrameStartTriggerMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszTriggerMode, EDeviceDahuaGigETriggerMode_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_FrameStartTriggerSource, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_FrameStartTriggerSource], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszTriggerSource, EDeviceDahuaGigETriggerSource_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_FrameStartTriggerActivation, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_FrameStartTriggerActivation], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszTriggerActivation, EDeviceDahuaGigETriggerActivation_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_FrameStartTriggerDelay, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_FrameStartTriggerDelay], _T("0.000000"), EParameterFieldType_Edit, nullptr, _T("float"), 2);

		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_AcquisitionStart, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_AcquisitionStart], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_AcquisitionStartTriggerMode, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_AcquisitionStartTriggerMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszTriggerMode, EDeviceDahuaGigETriggerMode_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_AcquisitionStartTriggerSource, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_AcquisitionStartTriggerSource], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszTriggerSource, EDeviceDahuaGigETriggerSource_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_AcquisitionStartTriggerActivation, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_AcquisitionStartTriggerActivation], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszTriggerActivation, EDeviceDahuaGigETriggerActivation_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_AcquisitionStartTriggerDelay, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_AcquisitionStartTriggerDelay], _T("0.000000"), EParameterFieldType_Edit, nullptr, _T("float"), 2);

		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_LightTriggerDelay, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_LightTriggerDelay], _T("0.000000"), EParameterFieldType_Edit, nullptr, _T("float"), 1);
		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_ExposureMode, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_ExposureMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszExposureMode, EDeviceDahuaGigEExposureMode_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_ExposureTime, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_ExposureTime], _T("5000.000000"), EParameterFieldType_Edit, nullptr, _T("float"), 1);
		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_ExposureAuto, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_ExposureAuto], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszExposureAuto, EDeviceDahuaGigEExposureAuto_Count), nullptr, 1);
		
		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_AnalogControl, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_AnalogControl], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_GainSelector, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_GainSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGainSelector, EDeviceDahuaGigEGainSelector_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_AllGainRaw, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_AllGainRaw], _T("1.000000"), EParameterFieldType_Edit, nullptr, _T("float"), 2);
		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_BlackLevelSelector, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_BlackLevelSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBlackSelector, EDeviceDahuaGigEBlackLevelSelector_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_BlackLevel, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_BlackLevel], _T("0"), EParameterFieldType_Edit, nullptr, _T("integer"), 2);
		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_BlackLevelAuto, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_BlackLevelAuto], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBlackLevelAuto, EDeviceDahuaGigEBlackLevelAuto_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_Gamma, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_Gamma], _T("1.000000"), EParameterFieldType_Edit, nullptr, _T("float"), 2);

		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_DigitalIOControl, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_DigitalIOControl], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_LineSelector, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_LineSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszLineSelector, EDeviceDahuaGigELineSelector_Count), nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_Line0, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_Line0], _T("1"), EParameterFieldType_None, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_Line0Inverter, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_Line0Inverter], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszLineInverter, EDeviceDahuaGigELineInverter_Count), nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_Line0Source, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_Line0Source], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszLineSource, EDeviceDahuaGigELineSource_Count), nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_Line0Format, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_Line0Format], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszLineFormat, EDeviceDahuaGigELineFormat_Count), nullptr, 3);

		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_Line1, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_Line1], _T("1"), EParameterFieldType_None, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_Line1Inverter, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_Line1Inverter], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszLineInverter, EDeviceDahuaGigELineInverter_Count), nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_Line1Source, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_Line1Source], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszLineSource, EDeviceDahuaGigELineSource_Count), nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterDahuaGigE_Line1Format, g_lpszParamDahuaGigE[EDeviceParameterDahuaGigE_Line1Format], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszLineFormat, EDeviceDahuaGigELineFormat_Count), nullptr, 3);
				
		bReturn = true;
	}
	while(false);

	return bReturn & __super::LoadSettings();
}

EDeviceGrabResult CDeviceDahuaGigE::Grab()
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

		m_pLiveThread = AfxBeginThread(CDeviceDahuaGigE::CallbackFunction, this, 0, 0U, CREATE_SUSPENDED);

		if(!m_pLiveThread)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtocreatethread);
			eReturn = EDeviceGrabResult_CreateThreadError;
			break;
		}

		GENICAM_EGrabStrategy eGrabStrategy;

		eGrabStrategy = grabStrartegySequential;
	
		if(m_pStreamSource->startGrabbing(m_pStreamSource, 0, eGrabStrategy) != 0)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Ithasbeenlivingorgrabbing);
			eReturn = EDeviceGrabResult_AlreadyGrabError;
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

EDeviceLiveResult CDeviceDahuaGigE::Live()
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

		m_pLiveThread = AfxBeginThread(CDeviceDahuaGigE::CallbackFunction, this, 0, 0U, CREATE_SUSPENDED);

		if(!m_pLiveThread)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtocreatethread);
			eReturn = EDeviceLiveResult_CreateThreadError;
			break;
		}

		GENICAM_EGrabStrategy eGrabStrategy;

		eGrabStrategy = grabStrartegySequential;

		if(m_pStreamSource->startGrabbing(m_pStreamSource, 0, eGrabStrategy) != 0)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Grab Stop"));
			eReturn = EDeviceLiveResult_AlreadyGrabError;
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

EDeviceStopResult CDeviceDahuaGigE::Stop()
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

		m_bIsGrabAvailable = true;
		m_bIsLive = false;

		m_pStreamSource->stopGrabbing(m_pStreamSource);

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

EDeviceTriggerResult CDeviceDahuaGigE::Trigger()
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

		GENICAM_CmdNode *pNode = NULL;
		GENICAM_CmdNodeInfo genNodeInfo = { 0 };
		
		genNodeInfo.pCamera = m_pCamera;
		memcpy(genNodeInfo.attrName, "TriggerSoftware", sizeof("TriggerSoftware"));

		if(GENICAM_createCmdNode(&genNodeInfo, &pNode))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("command trigger"));
			eReturn = EDeviceTriggerResult_ReadOnDeviceError;
			break;
		}

		if(pNode->execute(pNode))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TriggerSoftware"));
			eReturn = EDeviceTriggerResult_ReadOnDeviceError;
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

EDahuaGigEGetFunction CDeviceDahuaGigE::GetGrabCount(int * pParam)
{
	EDahuaGigEGetFunction eReturn = EDahuaGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDahuaGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterDahuaGigE_GrabCount));

		eReturn = EDahuaGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDahuaGigESetFunction CDeviceDahuaGigE::SetGrabCount(int nParam)
{
	EDahuaGigESetFunction eReturn = EDahuaGigESetFunction_UnknownError;

	EDeviceParameterDahuaGigE eSaveID = EDeviceParameterDahuaGigE_GrabCount;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsLive() || !IsGrabAvailable())
		{
			eReturn = EDahuaGigESetFunction_AlreadyGrabError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDahuaGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDahuaGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDahuaGigEGetFunction CDeviceDahuaGigE::GetCanvasWidth(int * pParam)
{
	EDahuaGigEGetFunction eReturn = EDahuaGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDahuaGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterDahuaGigE_CanvasWidth));

		eReturn = EDahuaGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDahuaGigESetFunction CDeviceDahuaGigE::SetCanvasWidth(int nParam)
{
	EDahuaGigESetFunction eReturn = EDahuaGigESetFunction_UnknownError;

	EDeviceParameterDahuaGigE eSaveID = EDeviceParameterDahuaGigE_CanvasWidth;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDahuaGigESetFunction_InitializedDeviceError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDahuaGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDahuaGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDahuaGigEGetFunction CDeviceDahuaGigE::GetCanvasHeight(int * pParam)
{
	EDahuaGigEGetFunction eReturn = EDahuaGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDahuaGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterDahuaGigE_CanvasHeight));

		eReturn = EDahuaGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDahuaGigESetFunction CDeviceDahuaGigE::SetCanvasHeight(int nParam)
{
	EDahuaGigESetFunction eReturn = EDahuaGigESetFunction_UnknownError;

	EDeviceParameterDahuaGigE eSaveID = EDeviceParameterDahuaGigE_CanvasHeight;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDahuaGigESetFunction_InitializedDeviceError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDahuaGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDahuaGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDahuaGigEGetFunction CDeviceDahuaGigE::GetOffsetX(int * pParam)
{
	EDahuaGigEGetFunction eReturn = EDahuaGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDahuaGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterDahuaGigE_OffsetX));

		eReturn = EDahuaGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDahuaGigESetFunction CDeviceDahuaGigE::SetOffsetX(int nParam)
{
	EDahuaGigESetFunction eReturn = EDahuaGigESetFunction_UnknownError;

	EDeviceParameterDahuaGigE eSaveID = EDeviceParameterDahuaGigE_OffsetX;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(!SetDeviceValueInt(_T("OffsetX"), nParam))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDahuaGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDahuaGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDahuaGigEGetFunction CDeviceDahuaGigE::GetOffsetY(int * pParam)
{
	EDahuaGigEGetFunction eReturn = EDahuaGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDahuaGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterDahuaGigE_OffsetY));

		eReturn = EDahuaGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDahuaGigESetFunction CDeviceDahuaGigE::SetOffsetY(int nParam)
{
	EDahuaGigESetFunction eReturn = EDahuaGigESetFunction_UnknownError;

	EDeviceParameterDahuaGigE eSaveID = EDeviceParameterDahuaGigE_OffsetY;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(!SetDeviceValueInt(_T("OffsetY"), nParam))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDahuaGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDahuaGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDahuaGigEGetFunction CDeviceDahuaGigE::GetReverseX(bool * pParam)
{
	EDahuaGigEGetFunction eReturn = EDahuaGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDahuaGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterDahuaGigE_ReverseX));

		eReturn = EDahuaGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDahuaGigESetFunction CDeviceDahuaGigE::SetReverseX(bool bParam)
{
	EDahuaGigESetFunction eReturn = EDahuaGigESetFunction_UnknownError;

	EDeviceParameterDahuaGigE eSaveID = EDeviceParameterDahuaGigE_ReverseX;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(!SetDeviceValueBool(_T("ReverseX"), bParam))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDahuaGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDahuaGigE[eSaveID], g_lpszSwitch[nPreValue], g_lpszSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDahuaGigEGetFunction CDeviceDahuaGigE::GetReverseY(bool * pParam)
{
	EDahuaGigEGetFunction eReturn = EDahuaGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDahuaGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterDahuaGigE_ReverseY));

		eReturn = EDahuaGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDahuaGigESetFunction CDeviceDahuaGigE::SetReverseY(bool bParam)
{
	EDahuaGigESetFunction eReturn = EDahuaGigESetFunction_UnknownError;

	EDeviceParameterDahuaGigE eSaveID = EDeviceParameterDahuaGigE_ReverseY;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(!SetDeviceValueBool(_T("ReverseY"), bParam))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDahuaGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDahuaGigE[eSaveID], g_lpszSwitch[nPreValue], g_lpszSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDahuaGigEGetFunction CDeviceDahuaGigE::GetPixelFormat(EDeviceDahuaGigEPixelFormat * pParam)
{
	EDahuaGigEGetFunction eReturn = EDahuaGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDahuaGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceDahuaGigEPixelFormat)_ttoi(GetParamValue(EDeviceParameterDahuaGigE_PixelFormat));

		eReturn = EDahuaGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDahuaGigESetFunction CDeviceDahuaGigE::SetPixelFormat(EDeviceDahuaGigEPixelFormat eParam)
{
	EDahuaGigESetFunction eReturn = EDahuaGigESetFunction_UnknownError;

	EDeviceParameterDahuaGigE eSaveID = EDeviceParameterDahuaGigE_PixelFormat;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDahuaGigEPixelFormat_Count)
		{
			eReturn = EDahuaGigESetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EDahuaGigESetFunction_InitializedDeviceError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDahuaGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDahuaGigE[eSaveID], g_lpszPixelFormat[nPreValue], g_lpszPixelFormat[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDahuaGigEGetFunction CDeviceDahuaGigE::GetAcquisitionMode(EDeviceDahuaGigEAcquisitionMode * pParam)
{
	EDahuaGigEGetFunction eReturn = EDahuaGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDahuaGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceDahuaGigEAcquisitionMode)_ttoi(GetParamValue(EDeviceParameterDahuaGigE_AcquisitionMode));

		eReturn = EDahuaGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDahuaGigESetFunction CDeviceDahuaGigE::SetAcquisitionMode(EDeviceDahuaGigEAcquisitionMode eParam)
{
	EDahuaGigESetFunction eReturn = EDahuaGigESetFunction_UnknownError;

	EDeviceParameterDahuaGigE eSaveID = EDeviceParameterDahuaGigE_AcquisitionMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDahuaGigEAcquisitionMode_Count)
		{
			eReturn = EDahuaGigESetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(!SetDeviceValueEnum(_T("AcquisitionMode"), g_lpszAcquisitionMode[eParam]))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDahuaGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDahuaGigE[eSaveID], g_lpszAcquisitionMode[nPreValue], g_lpszAcquisitionMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDahuaGigEGetFunction CDeviceDahuaGigE::GetAcquisitionFrameCount(int * pParam)
{
	EDahuaGigEGetFunction eReturn = EDahuaGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDahuaGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterDahuaGigE_AcquisitionFrameCount));

		eReturn = EDahuaGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDahuaGigESetFunction CDeviceDahuaGigE::SetAcquisitionFrameCount(int nParam)
{
	EDahuaGigESetFunction eReturn = EDahuaGigESetFunction_UnknownError;

	EDeviceParameterDahuaGigE eSaveID = EDeviceParameterDahuaGigE_AcquisitionFrameCount;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsLive() || !IsGrabAvailable())
		{
			eReturn = EDahuaGigESetFunction_AlreadyGrabError;
			break;
		}

		if(IsInitialized())
		{
			if(!SetDeviceValueInt(_T("AcquisitionFrameCount"), nParam))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDahuaGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDahuaGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDahuaGigEGetFunction CDeviceDahuaGigE::GetAcquisitionFrameRate(float * pParam)
{
	EDahuaGigEGetFunction eReturn = EDahuaGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDahuaGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterDahuaGigE_AcquisitionFrameRate));

		eReturn = EDahuaGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDahuaGigESetFunction CDeviceDahuaGigE::SetAcquisitionFrameRate(float fParam)
{
	EDahuaGigESetFunction eReturn = EDahuaGigESetFunction_UnknownError;

	EDeviceParameterDahuaGigE eSaveID = EDeviceParameterDahuaGigE_AcquisitionFrameRate;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(!SetDeviceValueDouble(_T("AcquisitionFrameRate"), fParam))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDahuaGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDahuaGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDahuaGigEGetFunction CDeviceDahuaGigE::GetAcquisitionFrameRateEnable(bool * pParam)
{
	EDahuaGigEGetFunction eReturn = EDahuaGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDahuaGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterDahuaGigE_AcquisitionFrameRateEnable));

		eReturn = EDahuaGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDahuaGigESetFunction CDeviceDahuaGigE::SetAcquisitionFrameRateEnable(bool bParam)
{
	EDahuaGigESetFunction eReturn = EDahuaGigESetFunction_UnknownError;

	EDeviceParameterDahuaGigE eSaveID = EDeviceParameterDahuaGigE_AcquisitionFrameRateEnable;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(!SetDeviceValueBool(_T("AcquisitionFrameRateEnable"), bParam))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDahuaGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDahuaGigE[eSaveID], g_lpszSwitch[nPreValue], g_lpszSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDahuaGigEGetFunction CDeviceDahuaGigE::GetAcquisitionStatusSelector(EDeviceDahuaGigEAcquisitionSelector * pParam)
{
	EDahuaGigEGetFunction eReturn = EDahuaGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDahuaGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceDahuaGigEAcquisitionSelector)_ttoi(GetParamValue(EDeviceParameterDahuaGigE_AcquisitionStatusSelector));

		eReturn = EDahuaGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDahuaGigESetFunction CDeviceDahuaGigE::SetAcquisitionStatusSelector(EDeviceDahuaGigEAcquisitionSelector eParam)
{
	EDahuaGigESetFunction eReturn = EDahuaGigESetFunction_UnknownError;

	EDeviceParameterDahuaGigE eSaveID = EDeviceParameterDahuaGigE_AcquisitionStatusSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDahuaGigEAcquisitionSelector_Count)
		{
			eReturn = EDahuaGigESetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(!SetDeviceValueEnum(_T("AcquisitionStatusSelector"), g_lpszAcquisitionSelector[eParam]))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDahuaGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDahuaGigE[eSaveID], g_lpszAcquisitionSelector[nPreValue], g_lpszAcquisitionSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDahuaGigEGetFunction CDeviceDahuaGigE::GetTriggerSelector(EDeviceDahuaGigETriggerSelector * pParam)
{
	EDahuaGigEGetFunction eReturn = EDahuaGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDahuaGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceDahuaGigETriggerSelector)_ttoi(GetParamValue(EDeviceParameterDahuaGigE_TriggerSelector));

		eReturn = EDahuaGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDahuaGigESetFunction CDeviceDahuaGigE::SetTriggerSelector(EDeviceDahuaGigETriggerSelector eParam)
{
	EDahuaGigESetFunction eReturn = EDahuaGigESetFunction_UnknownError;

	EDeviceParameterDahuaGigE eSaveID = EDeviceParameterDahuaGigE_TriggerSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDahuaGigETriggerSelector_Count)
		{
			eReturn = EDahuaGigESetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(!SetDeviceValueEnum(_T("TriggerSelector"), g_lpszTriggerSelector[eParam]))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDahuaGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDahuaGigE[eSaveID], g_lpszTriggerSelector[nPreValue], g_lpszTriggerSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDahuaGigEGetFunction CDeviceDahuaGigE::GetFrameStartTriggerMode(EDeviceDahuaGigETriggerMode * pParam)
{
	EDahuaGigEGetFunction eReturn = EDahuaGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDahuaGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceDahuaGigETriggerMode)_ttoi(GetParamValue(EDeviceParameterDahuaGigE_FrameStartTriggerMode));

		eReturn = EDahuaGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDahuaGigESetFunction CDeviceDahuaGigE::SetFrameStartTriggerMode(EDeviceDahuaGigETriggerMode eParam)
{
	EDahuaGigESetFunction eReturn = EDahuaGigESetFunction_UnknownError;

	EDeviceParameterDahuaGigE eSaveID = EDeviceParameterDahuaGigE_FrameStartTriggerMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDahuaGigETriggerMode_Count)
		{
			eReturn = EDahuaGigESetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			CString strSelect;

			if(!GetDeviceValueEnum(_T("TriggerSelector"), &strSelect))
			{
				eReturn = EDahuaGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(strSelect.CompareNoCase(g_lpszTriggerSelector[EDeviceDahuaGigETriggerSelector_FrameStart]))
			{
				eReturn = EDahuaGigESetFunction_NotSelectFeatureError;
				break;
			}

			if(!SetDeviceValueEnum(_T("TriggerMode"), g_lpszTriggerMode[eParam]))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDahuaGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDahuaGigE[eSaveID], g_lpszTriggerMode[nPreValue], g_lpszTriggerMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDahuaGigEGetFunction CDeviceDahuaGigE::GetFrameStartTriggerSource(EDeviceDahuaGigETriggerSource * pParam)
{
	EDahuaGigEGetFunction eReturn = EDahuaGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDahuaGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceDahuaGigETriggerSource)_ttoi(GetParamValue(EDeviceParameterDahuaGigE_FrameStartTriggerSource));

		eReturn = EDahuaGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDahuaGigESetFunction CDeviceDahuaGigE::SetFrameStartTriggerSource(EDeviceDahuaGigETriggerSource eParam)
{
	EDahuaGigESetFunction eReturn = EDahuaGigESetFunction_UnknownError;

	EDeviceParameterDahuaGigE eSaveID = EDeviceParameterDahuaGigE_FrameStartTriggerSource;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDahuaGigETriggerSource_Count)
		{
			eReturn = EDahuaGigESetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			CString strSelect;

			if(!GetDeviceValueEnum(_T("TriggerSelector"), &strSelect))
			{
				eReturn = EDahuaGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(strSelect.CompareNoCase(g_lpszTriggerSelector[EDeviceDahuaGigETriggerSelector_FrameStart]))
			{
				eReturn = EDahuaGigESetFunction_NotSelectFeatureError;
				break;
			}

			if(!SetDeviceValueEnum(_T("TriggerSource"), g_lpszTriggerSource[eParam]))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDahuaGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDahuaGigE[eSaveID], g_lpszTriggerSource[nPreValue], g_lpszTriggerSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDahuaGigEGetFunction CDeviceDahuaGigE::GetFrameStartTriggerActivation(EDeviceDahuaGigETriggerActivation * pParam)
{
	EDahuaGigEGetFunction eReturn = EDahuaGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDahuaGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceDahuaGigETriggerActivation)_ttoi(GetParamValue(EDeviceParameterDahuaGigE_FrameStartTriggerActivation));

		eReturn = EDahuaGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDahuaGigESetFunction CDeviceDahuaGigE::SetFrameStartTriggerActivation(EDeviceDahuaGigETriggerActivation eParam)
{
	EDahuaGigESetFunction eReturn = EDahuaGigESetFunction_UnknownError;

	EDeviceParameterDahuaGigE eSaveID = EDeviceParameterDahuaGigE_FrameStartTriggerActivation;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDahuaGigETriggerActivation_Count)
		{
			eReturn = EDahuaGigESetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			CString strSelect;

			if(!GetDeviceValueEnum(_T("TriggerSelector"), &strSelect))
			{
				eReturn = EDahuaGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(strSelect.CompareNoCase(g_lpszTriggerSelector[EDeviceDahuaGigETriggerSelector_FrameStart]))
			{
				eReturn = EDahuaGigESetFunction_NotSelectFeatureError;
				break;
			}

			CString strSource;

			if(!GetDeviceValueEnum(_T("TriggerSource"), &strSource))
			{
				eReturn = EDahuaGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(!strSource.CompareNoCase(g_lpszTriggerSource[EDeviceDahuaGigETriggerSource_Software]))
			{
				eReturn = EDahuaGigESetFunction_NotSelectFeatureError;
				break;
			}
			
			if(!SetDeviceValueEnum(_T("TriggerActivation"), g_lpszTriggerActivation[eParam]))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDahuaGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDahuaGigE[eSaveID], g_lpszTriggerActivation[nPreValue], g_lpszTriggerActivation[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDahuaGigEGetFunction CDeviceDahuaGigE::GetFrameStartTriggerDelay(float * pParam)
{
	EDahuaGigEGetFunction eReturn = EDahuaGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDahuaGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterDahuaGigE_FrameStartTriggerDelay));

		eReturn = EDahuaGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDahuaGigESetFunction CDeviceDahuaGigE::SetFrameStartTriggerDelay(float fParam)
{
	EDahuaGigESetFunction eReturn = EDahuaGigESetFunction_UnknownError;

	EDeviceParameterDahuaGigE eSaveID = EDeviceParameterDahuaGigE_FrameStartTriggerDelay;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			CString strSelect;

			if(!GetDeviceValueEnum(_T("TriggerSelector"), &strSelect))
			{
				eReturn = EDahuaGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(strSelect.CompareNoCase(g_lpszTriggerSelector[EDeviceDahuaGigETriggerSelector_FrameStart]))
			{
				eReturn = EDahuaGigESetFunction_NotSelectFeatureError;
				break;
			}

			if(!SetDeviceValueDouble(_T("TriggerDelay"), fParam))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDahuaGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDahuaGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDahuaGigEGetFunction CDeviceDahuaGigE::GetAcquisitionStartTriggerMode(EDeviceDahuaGigETriggerMode * pParam)
{
	EDahuaGigEGetFunction eReturn = EDahuaGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDahuaGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceDahuaGigETriggerMode)_ttoi(GetParamValue(EDeviceParameterDahuaGigE_AcquisitionStartTriggerMode));

		eReturn = EDahuaGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDahuaGigESetFunction CDeviceDahuaGigE::SetAcquisitionStartTriggerMode(EDeviceDahuaGigETriggerMode eParam)
{
	EDahuaGigESetFunction eReturn = EDahuaGigESetFunction_UnknownError;

	EDeviceParameterDahuaGigE eSaveID = EDeviceParameterDahuaGigE_AcquisitionStartTriggerMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDahuaGigETriggerMode_Count)
		{
			eReturn = EDahuaGigESetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			CString strSelect;

			if(!GetDeviceValueEnum(_T("TriggerSelector"), &strSelect))
			{
				eReturn = EDahuaGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(strSelect.CompareNoCase(g_lpszTriggerSelector[EDeviceDahuaGigETriggerSelector_AcquisitionStart]))
			{
				eReturn = EDahuaGigESetFunction_NotSelectFeatureError;
				break;
			}

			if(!SetDeviceValueEnum(_T("TriggerMode"), g_lpszTriggerMode[eParam]))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDahuaGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDahuaGigE[eSaveID], g_lpszTriggerMode[nPreValue], g_lpszTriggerMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDahuaGigEGetFunction CDeviceDahuaGigE::GetAcquisitionStartTriggerSource(EDeviceDahuaGigETriggerSource * pParam)
{
	EDahuaGigEGetFunction eReturn = EDahuaGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDahuaGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceDahuaGigETriggerSource)_ttoi(GetParamValue(EDeviceParameterDahuaGigE_AcquisitionStartTriggerSource));

		eReturn = EDahuaGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDahuaGigESetFunction CDeviceDahuaGigE::SetAcquisitionStartTriggerSource(EDeviceDahuaGigETriggerSource eParam)
{
	EDahuaGigESetFunction eReturn = EDahuaGigESetFunction_UnknownError;

	EDeviceParameterDahuaGigE eSaveID = EDeviceParameterDahuaGigE_AcquisitionStartTriggerSource;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDahuaGigETriggerSource_Count)
		{
			eReturn = EDahuaGigESetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			CString strSelect;

			if(!GetDeviceValueEnum(_T("TriggerSelector"), &strSelect))
			{
				eReturn = EDahuaGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(strSelect.CompareNoCase(g_lpszTriggerSelector[EDeviceDahuaGigETriggerSelector_AcquisitionStart]))
			{
				eReturn = EDahuaGigESetFunction_NotSelectFeatureError;
				break;
			}

			if(!SetDeviceValueEnum(_T("TriggerSource"), g_lpszTriggerSource[eParam]))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDahuaGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDahuaGigE[eSaveID], g_lpszTriggerSource[nPreValue], g_lpszTriggerSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDahuaGigEGetFunction CDeviceDahuaGigE::GetAcquisitionStartTriggerActivation(EDeviceDahuaGigETriggerActivation * pParam)
{
	EDahuaGigEGetFunction eReturn = EDahuaGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDahuaGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceDahuaGigETriggerActivation)_ttoi(GetParamValue(EDeviceParameterDahuaGigE_AcquisitionStartTriggerActivation));

		eReturn = EDahuaGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDahuaGigESetFunction CDeviceDahuaGigE::SetAcquisitionStartTriggerActivation(EDeviceDahuaGigETriggerActivation eParam)
{
	EDahuaGigESetFunction eReturn = EDahuaGigESetFunction_UnknownError;

	EDeviceParameterDahuaGigE eSaveID = EDeviceParameterDahuaGigE_AcquisitionStartTriggerActivation;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDahuaGigETriggerActivation_Count)
		{
			eReturn = EDahuaGigESetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			CString strSelect;

			if(!GetDeviceValueEnum(_T("TriggerSelector"), &strSelect))
			{
				eReturn = EDahuaGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(strSelect.CompareNoCase(g_lpszTriggerSelector[EDeviceDahuaGigETriggerSelector_AcquisitionStart]))
			{
				eReturn = EDahuaGigESetFunction_NotSelectFeatureError;
				break;
			}

			CString strSource;

			if(!GetDeviceValueEnum(_T("TriggerSource"), &strSource))
			{
				eReturn = EDahuaGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(!strSource.CompareNoCase(g_lpszTriggerSource[EDeviceDahuaGigETriggerSource_Software]))
			{
				eReturn = EDahuaGigESetFunction_NotSelectFeatureError;
				break;
			}

			if(!SetDeviceValueEnum(_T("TriggerActivation"), g_lpszTriggerActivation[eParam]))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDahuaGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDahuaGigE[eSaveID], g_lpszTriggerActivation[nPreValue], g_lpszTriggerActivation[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDahuaGigEGetFunction CDeviceDahuaGigE::GetAcquisitionStartTriggerDelay(float * pParam)
{
	EDahuaGigEGetFunction eReturn = EDahuaGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDahuaGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterDahuaGigE_AcquisitionStartTriggerDelay));

		eReturn = EDahuaGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDahuaGigESetFunction CDeviceDahuaGigE::SetAcquisitionStartTriggerDelay(float fParam)
{
	EDahuaGigESetFunction eReturn = EDahuaGigESetFunction_UnknownError;

	EDeviceParameterDahuaGigE eSaveID = EDeviceParameterDahuaGigE_AcquisitionStartTriggerDelay;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			CString strSelect;

			if(!GetDeviceValueEnum(_T("TriggerSelector"), &strSelect))
			{
				eReturn = EDahuaGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(strSelect.CompareNoCase(g_lpszTriggerSelector[EDeviceDahuaGigETriggerSelector_AcquisitionStart]))
			{
				eReturn = EDahuaGigESetFunction_NotSelectFeatureError;
				break;
			}

			if(!SetDeviceValueDouble(_T("TriggerDelay"), fParam))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDahuaGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDahuaGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDahuaGigEGetFunction CDeviceDahuaGigE::GetLightTriggerDelay(float * pParam)
{
	EDahuaGigEGetFunction eReturn = EDahuaGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDahuaGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterDahuaGigE_LightTriggerDelay));

		eReturn = EDahuaGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDahuaGigESetFunction CDeviceDahuaGigE::SetLightTriggerDelay(float fParam)
{
	EDahuaGigESetFunction eReturn = EDahuaGigESetFunction_UnknownError;

	EDeviceParameterDahuaGigE eSaveID = EDeviceParameterDahuaGigE_LightTriggerDelay;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(!SetDeviceValueDouble(_T("LightTriggerDelay"), fParam))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDahuaGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDahuaGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDahuaGigEGetFunction CDeviceDahuaGigE::GetExposureMode(EDeviceDahuaGigEExposureMode * pParam)
{
	EDahuaGigEGetFunction eReturn = EDahuaGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDahuaGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceDahuaGigEExposureMode)_ttoi(GetParamValue(EDeviceParameterDahuaGigE_ExposureMode));

		eReturn = EDahuaGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDahuaGigESetFunction CDeviceDahuaGigE::SetExposureMode(EDeviceDahuaGigEExposureMode eParam)
{
	EDahuaGigESetFunction eReturn = EDahuaGigESetFunction_UnknownError;

	EDeviceParameterDahuaGigE eSaveID = EDeviceParameterDahuaGigE_ExposureMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDahuaGigEExposureMode_Count)
		{
			eReturn = EDahuaGigESetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(!SetDeviceValueEnum(_T("ExposureMode"), g_lpszExposureMode[eParam]))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDahuaGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDahuaGigE[eSaveID], g_lpszExposureMode[nPreValue], g_lpszExposureMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDahuaGigEGetFunction CDeviceDahuaGigE::GetExposureTime(float * pParam)
{
	EDahuaGigEGetFunction eReturn = EDahuaGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDahuaGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterDahuaGigE_ExposureTime));

		eReturn = EDahuaGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDahuaGigESetFunction CDeviceDahuaGigE::SetExposureTime(float fParam)
{
	EDahuaGigESetFunction eReturn = EDahuaGigESetFunction_UnknownError;

	EDeviceParameterDahuaGigE eSaveID = EDeviceParameterDahuaGigE_ExposureTime;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			CString strMode;

			if(!GetDeviceValueEnum(_T("ExposureMode"), &strMode))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}

			if(strMode.CompareNoCase(g_lpszExposureMode[EDeviceDahuaGigEExposureMode_Timed]))
			{
				eReturn = EDahuaGigESetFunction_NotSelectFeatureError;
				break;
			}

			CString strAuto;

			if(!GetDeviceValueEnum(_T("ExposureAuto"), &strAuto))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}

			if(strAuto.CompareNoCase(g_lpszExposureAuto[EDeviceDahuaGigEExposureAuto_Off]))
			{
				eReturn = EDahuaGigESetFunction_NotSelectFeatureError;
				break;
			}

			if(!SetDeviceValueDouble(_T("ExposureTime"), fParam))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDahuaGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDahuaGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDahuaGigEGetFunction CDeviceDahuaGigE::GetExposureAuto(EDeviceDahuaGigEExposureAuto * pParam)
{
	EDahuaGigEGetFunction eReturn = EDahuaGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDahuaGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceDahuaGigEExposureAuto)_ttoi(GetParamValue(EDeviceParameterDahuaGigE_ExposureAuto));

		eReturn = EDahuaGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDahuaGigESetFunction CDeviceDahuaGigE::SetExposureAuto(EDeviceDahuaGigEExposureAuto eParam)
{
	EDahuaGigESetFunction eReturn = EDahuaGigESetFunction_UnknownError;

	EDeviceParameterDahuaGigE eSaveID = EDeviceParameterDahuaGigE_ExposureAuto;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDahuaGigEExposureAuto_Count)
		{
			eReturn = EDahuaGigESetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			CString strMode;

			if(!GetDeviceValueEnum(_T("ExposureMode"), &strMode))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}

			if(strMode.CompareNoCase(g_lpszExposureMode[EDeviceDahuaGigEExposureMode_Timed]))
			{
				eReturn = EDahuaGigESetFunction_NotSelectFeatureError;
				break;
			}

			if(!SetDeviceValueEnum(_T("ExposureAuto"), g_lpszExposureAuto[eParam]))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDahuaGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDahuaGigE[eSaveID], g_lpszExposureAuto[nPreValue], g_lpszExposureAuto[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDahuaGigEGetFunction CDeviceDahuaGigE::GetGainSelector(EDeviceDahuaGigEGainSelector * pParam)
{
	EDahuaGigEGetFunction eReturn = EDahuaGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDahuaGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceDahuaGigEGainSelector)_ttoi(GetParamValue(EDeviceParameterDahuaGigE_GainSelector));

		eReturn = EDahuaGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDahuaGigESetFunction CDeviceDahuaGigE::SetGainSelector(EDeviceDahuaGigEGainSelector eParam)
{
	EDahuaGigESetFunction eReturn = EDahuaGigESetFunction_UnknownError;

	EDeviceParameterDahuaGigE eSaveID = EDeviceParameterDahuaGigE_GainSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDahuaGigEGainSelector_Count)
		{
			eReturn = EDahuaGigESetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(!SetDeviceValueEnum(_T("GainSelector"), g_lpszGainSelector[eParam]))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDahuaGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDahuaGigE[eSaveID], g_lpszGainSelector[nPreValue], g_lpszGainSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDahuaGigEGetFunction CDeviceDahuaGigE::GetAllGainRaw(float * pParam)
{
	EDahuaGigEGetFunction eReturn = EDahuaGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDahuaGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterDahuaGigE_AllGainRaw));

		eReturn = EDahuaGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDahuaGigESetFunction CDeviceDahuaGigE::SetAllGainRaw(float fParam)
{
	EDahuaGigESetFunction eReturn = EDahuaGigESetFunction_UnknownError;

	EDeviceParameterDahuaGigE eSaveID = EDeviceParameterDahuaGigE_AllGainRaw;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(!SetDeviceValueDouble(_T("GainRaw"), fParam))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDahuaGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDahuaGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDahuaGigEGetFunction CDeviceDahuaGigE::GetBlackLevelSelector(EDeviceDahuaGigEBlackLevelSelector * pParam)
{
	EDahuaGigEGetFunction eReturn = EDahuaGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDahuaGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceDahuaGigEBlackLevelSelector)_ttoi(GetParamValue(EDeviceParameterDahuaGigE_BlackLevelSelector));

		eReturn = EDahuaGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDahuaGigESetFunction CDeviceDahuaGigE::SetBlackLevelSelector(EDeviceDahuaGigEBlackLevelSelector eParam)
{
	EDahuaGigESetFunction eReturn = EDahuaGigESetFunction_UnknownError;

	EDeviceParameterDahuaGigE eSaveID = EDeviceParameterDahuaGigE_BlackLevelSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDahuaGigEBlackLevelSelector_Count)
		{
			eReturn = EDahuaGigESetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(!SetDeviceValueEnum(_T("BlackLevelSelector"), g_lpszBlackSelector[eParam]))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDahuaGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDahuaGigE[eSaveID], g_lpszBlackSelector[nPreValue], g_lpszBlackSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDahuaGigEGetFunction CDeviceDahuaGigE::GetBlackLevel(int * pParam)
{
	EDahuaGigEGetFunction eReturn = EDahuaGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDahuaGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterDahuaGigE_BlackLevel));

		eReturn = EDahuaGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDahuaGigESetFunction CDeviceDahuaGigE::SetBlackLevel(int nParam)
{
	EDahuaGigESetFunction eReturn = EDahuaGigESetFunction_UnknownError;

	EDeviceParameterDahuaGigE eSaveID = EDeviceParameterDahuaGigE_BlackLevel;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			CString strAuto;

			if(!GetDeviceValueEnum(_T("BlackLevelAuto"), &strAuto))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}

			if(strAuto.CompareNoCase(g_lpszBlackLevelAuto[EDeviceDahuaGigEBlackLevelAuto_Off]))
			{
				eReturn = EDahuaGigESetFunction_NotSelectFeatureError;
				break;
			}

			if(!SetDeviceValueInt(_T("BlackLevel"), nParam))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDahuaGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDahuaGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDahuaGigEGetFunction CDeviceDahuaGigE::GetBlackLevelAuto(EDeviceDahuaGigEBlackLevelAuto * pParam)
{
	EDahuaGigEGetFunction eReturn = EDahuaGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDahuaGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceDahuaGigEBlackLevelAuto)_ttoi(GetParamValue(EDeviceParameterDahuaGigE_BlackLevelAuto));

		eReturn = EDahuaGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDahuaGigESetFunction CDeviceDahuaGigE::SetBlackLevelAuto(EDeviceDahuaGigEBlackLevelAuto eParam)
{
	EDahuaGigESetFunction eReturn = EDahuaGigESetFunction_UnknownError;

	EDeviceParameterDahuaGigE eSaveID = EDeviceParameterDahuaGigE_BlackLevelAuto;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDahuaGigEBlackLevelAuto_Count)
		{
			eReturn = EDahuaGigESetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(!SetDeviceValueEnum(_T("BlackLevelAuto"), g_lpszBlackLevelAuto[eParam]))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDahuaGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDahuaGigE[eSaveID], g_lpszBlackLevelAuto[nPreValue], g_lpszBlackLevelAuto[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDahuaGigEGetFunction CDeviceDahuaGigE::GetGamma(float * pParam)
{
	EDahuaGigEGetFunction eReturn = EDahuaGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDahuaGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterDahuaGigE_Gamma));

		eReturn = EDahuaGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDahuaGigESetFunction CDeviceDahuaGigE::SetGamma(float fParam)
{
	EDahuaGigESetFunction eReturn = EDahuaGigESetFunction_UnknownError;

	EDeviceParameterDahuaGigE eSaveID = EDeviceParameterDahuaGigE_Gamma;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(!SetDeviceValueDouble(_T("Gamma"), fParam))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDahuaGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDahuaGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDahuaGigEGetFunction CDeviceDahuaGigE::GetLineSelector(EDeviceDahuaGigELineSelector * pParam)
{
	EDahuaGigEGetFunction eReturn = EDahuaGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDahuaGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceDahuaGigELineSelector)_ttoi(GetParamValue(EDeviceParameterDahuaGigE_LineSelector));

		eReturn = EDahuaGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDahuaGigESetFunction CDeviceDahuaGigE::SetLineSelector(EDeviceDahuaGigELineSelector eParam)
{
	EDahuaGigESetFunction eReturn = EDahuaGigESetFunction_UnknownError;

	EDeviceParameterDahuaGigE eSaveID = EDeviceParameterDahuaGigE_LineSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDahuaGigELineSelector_Count)
		{
			eReturn = EDahuaGigESetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(!SetDeviceValueEnum(_T("LineSelector"), g_lpszLineSelector[eParam]))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDahuaGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDahuaGigE[eSaveID], g_lpszLineSelector[nPreValue], g_lpszLineSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDahuaGigEGetFunction CDeviceDahuaGigE::GetLine0Inverter(EDeviceDahuaGigELineInverter * pParam)
{
	EDahuaGigEGetFunction eReturn = EDahuaGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDahuaGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceDahuaGigELineInverter)_ttoi(GetParamValue(EDeviceParameterDahuaGigE_Line0Inverter));

		eReturn = EDahuaGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDahuaGigESetFunction CDeviceDahuaGigE::SetLine0Inverter(EDeviceDahuaGigELineInverter eParam)
{
	EDahuaGigESetFunction eReturn = EDahuaGigESetFunction_UnknownError;

	EDeviceParameterDahuaGigE eSaveID = EDeviceParameterDahuaGigE_Line0Inverter;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDahuaGigELineInverter_Count)
		{
			eReturn = EDahuaGigESetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			CString strSelect;

			if(GetDeviceValueEnum(_T("LineSelector"), &strSelect))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}

			if(strSelect.CompareNoCase(g_lpszLineSelector[EDeviceDahuaGigELineSelector_Line0]))
			{
				eReturn = EDahuaGigESetFunction_NotSelectFeatureError;
				break;
			}
			
			if(!SetDeviceValueBool(_T("LineInverter"), eParam))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDahuaGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDahuaGigE[eSaveID], g_lpszLineInverter[nPreValue], g_lpszLineInverter[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDahuaGigEGetFunction CDeviceDahuaGigE::GetLine0Source(EDeviceDahuaGigELineSource * pParam)
{
	EDahuaGigEGetFunction eReturn = EDahuaGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDahuaGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceDahuaGigELineSource)_ttoi(GetParamValue(EDeviceParameterDahuaGigE_Line0Source));

		eReturn = EDahuaGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDahuaGigESetFunction CDeviceDahuaGigE::SetLine0Source(EDeviceDahuaGigELineSource eParam)
{
	EDahuaGigESetFunction eReturn = EDahuaGigESetFunction_UnknownError;

	EDeviceParameterDahuaGigE eSaveID = EDeviceParameterDahuaGigE_Line0Source;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDahuaGigELineSource_Count)
		{
			eReturn = EDahuaGigESetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			CString strSelect;

			if(GetDeviceValueEnum(_T("LineSelector"), &strSelect))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}

			if(strSelect.CompareNoCase(g_lpszLineSelector[EDeviceDahuaGigELineSelector_Line0]))
			{
				eReturn = EDahuaGigESetFunction_NotSelectFeatureError;
				break;
			}

			CString strLineMode;

			if(!GetDeviceValueEnum(_T("LineMode"), &strLineMode))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}
			
			if(strLineMode.CompareNoCase(_T("Output")))
			{
				eReturn = EDahuaGigESetFunction_NotSelectFeatureError;
				break;
			}

			if(!SetDeviceValueEnum(_T("LineSource"), g_lpszLineSource[eParam]))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDahuaGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDahuaGigE[eSaveID], g_lpszLineSource[nPreValue], g_lpszLineSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDahuaGigEGetFunction CDeviceDahuaGigE::GetLine0Format(EDeviceDahuaGigELineFormat * pParam)
{
	EDahuaGigEGetFunction eReturn = EDahuaGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDahuaGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceDahuaGigELineFormat)_ttoi(GetParamValue(EDeviceParameterDahuaGigE_Line0Format));

		eReturn = EDahuaGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDahuaGigESetFunction CDeviceDahuaGigE::SetLine0Format(EDeviceDahuaGigELineFormat eParam)
{
	EDahuaGigESetFunction eReturn = EDahuaGigESetFunction_UnknownError;

	EDeviceParameterDahuaGigE eSaveID = EDeviceParameterDahuaGigE_Line0Format;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDahuaGigELineFormat_Count)
		{
			eReturn = EDahuaGigESetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			CString strSelect;

			if(GetDeviceValueEnum(_T("LineSelector"), &strSelect))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}

			if(strSelect.CompareNoCase(g_lpszLineSelector[EDeviceDahuaGigELineSelector_Line0]))
			{
				eReturn = EDahuaGigESetFunction_NotSelectFeatureError;
				break;
			}

			if(!SetDeviceValueEnum(_T("LineFormat"), g_lpszLineFormat[eParam]))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDahuaGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDahuaGigE[eSaveID], g_lpszLineFormat[nPreValue], g_lpszLineFormat[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDahuaGigEGetFunction CDeviceDahuaGigE::GetLine1Inverter(EDeviceDahuaGigELineInverter * pParam)
{
	EDahuaGigEGetFunction eReturn = EDahuaGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDahuaGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceDahuaGigELineInverter)_ttoi(GetParamValue(EDeviceParameterDahuaGigE_Line1Inverter));

		eReturn = EDahuaGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDahuaGigESetFunction CDeviceDahuaGigE::SetLine1Inverter(EDeviceDahuaGigELineInverter eParam)
{
	EDahuaGigESetFunction eReturn = EDahuaGigESetFunction_UnknownError;

	EDeviceParameterDahuaGigE eSaveID = EDeviceParameterDahuaGigE_Line1Inverter;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDahuaGigELineInverter_Count)
		{
			eReturn = EDahuaGigESetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			CString strSelect;

			if(GetDeviceValueEnum(_T("LineSelector"), &strSelect))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}

			if(strSelect.CompareNoCase(g_lpszLineSelector[EDeviceDahuaGigELineSelector_Line1]))
			{
				eReturn = EDahuaGigESetFunction_NotSelectFeatureError;
				break;
			}

			if(!SetDeviceValueBool(_T("LineInverter"), eParam))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDahuaGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDahuaGigE[eSaveID], g_lpszLineInverter[nPreValue], g_lpszLineInverter[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDahuaGigEGetFunction CDeviceDahuaGigE::GetLine1Source(EDeviceDahuaGigELineSource * pParam)
{
	EDahuaGigEGetFunction eReturn = EDahuaGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDahuaGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceDahuaGigELineSource)_ttoi(GetParamValue(EDeviceParameterDahuaGigE_Line1Source));

		eReturn = EDahuaGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDahuaGigESetFunction CDeviceDahuaGigE::SetLine1Source(EDeviceDahuaGigELineSource eParam)
{
	EDahuaGigESetFunction eReturn = EDahuaGigESetFunction_UnknownError;

	EDeviceParameterDahuaGigE eSaveID = EDeviceParameterDahuaGigE_Line1Source;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDahuaGigELineSource_Count)
		{
			eReturn = EDahuaGigESetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			CString strSelect;

			if(GetDeviceValueEnum(_T("LineSelector"), &strSelect))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}

			if(strSelect.CompareNoCase(g_lpszLineSelector[EDeviceDahuaGigELineSelector_Line1]))
			{
				eReturn = EDahuaGigESetFunction_NotSelectFeatureError;
				break;
			}

			CString strLineMode;

			if(!GetDeviceValueEnum(_T("LineMode"), &strLineMode))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}

			if(strLineMode.CompareNoCase(_T("Output")))
			{
				eReturn = EDahuaGigESetFunction_NotSelectFeatureError;
				break;
			}

			if(!SetDeviceValueEnum(_T("LineSource"), g_lpszLineSource[eParam]))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDahuaGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDahuaGigE[eSaveID], g_lpszLineSource[nPreValue], g_lpszLineSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDahuaGigEGetFunction CDeviceDahuaGigE::GetLine1Format(EDeviceDahuaGigELineFormat * pParam)
{
	EDahuaGigEGetFunction eReturn = EDahuaGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDahuaGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceDahuaGigELineFormat)_ttoi(GetParamValue(EDeviceParameterDahuaGigE_Line1Format));

		eReturn = EDahuaGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDahuaGigESetFunction CDeviceDahuaGigE::SetLine1Format(EDeviceDahuaGigELineFormat eParam)
{
	EDahuaGigESetFunction eReturn = EDahuaGigESetFunction_UnknownError;

	EDeviceParameterDahuaGigE eSaveID = EDeviceParameterDahuaGigE_Line1Format;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDahuaGigELineFormat_Count)
		{
			eReturn = EDahuaGigESetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			CString strSelect;

			if(GetDeviceValueEnum(_T("LineSelector"), &strSelect))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}

			if(strSelect.CompareNoCase(g_lpszLineSelector[EDeviceDahuaGigELineSelector_Line1]))
			{
				eReturn = EDahuaGigESetFunction_NotSelectFeatureError;
				break;
			}

			if(!SetDeviceValueEnum(_T("LineFormat"), g_lpszLineFormat[eParam]))
			{
				eReturn = EDahuaGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDahuaGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDahuaGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDahuaGigE[eSaveID], g_lpszLineFormat[nPreValue], g_lpszLineFormat[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceDahuaGigE::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = false;

	bool bFoundID = true;

	do
	{
		switch(nParam)
		{
		case EDeviceParameterDahuaGigE_DeviceID:
			bReturn = !SetDeviceID(strValue);
			break;
		case EDeviceParameterDahuaGigE_GrabCount:
			bReturn = !SetGrabCount(_ttoi(strValue));
			break;
		case EDeviceParameterDahuaGigE_CanvasWidth:
			bReturn = !SetCanvasWidth(_ttoi(strValue));
			break;
		case EDeviceParameterDahuaGigE_CanvasHeight:
			bReturn = !SetCanvasHeight(_ttoi(strValue));
			break;
		case EDeviceParameterDahuaGigE_OffsetX:
			bReturn = !SetOffsetX(_ttoi(strValue));
			break;
		case EDeviceParameterDahuaGigE_OffsetY:
			bReturn = !SetOffsetY(_ttoi(strValue));
			break;
		case EDeviceParameterDahuaGigE_ReverseX:
			bReturn = !SetReverseX(_ttoi(strValue));
			break;
		case EDeviceParameterDahuaGigE_ReverseY:
			bReturn = !SetReverseY(_ttoi(strValue));
			break;
		case EDeviceParameterDahuaGigE_PixelFormat:
			bReturn = !SetPixelFormat((EDeviceDahuaGigEPixelFormat)_ttoi(strValue));
			break;
		case EDeviceParameterDahuaGigE_AcquisitionMode:
			bReturn = !SetAcquisitionMode((EDeviceDahuaGigEAcquisitionMode)_ttoi(strValue));
			break;
		case EDeviceParameterDahuaGigE_AcquisitionFrameCount:
			bReturn = !SetAcquisitionFrameCount(_ttoi(strValue));
			break;
		case EDeviceParameterDahuaGigE_AcquisitionFrameRate:
			bReturn = !SetAcquisitionFrameRate(_ttof(strValue));
			break;
		case EDeviceParameterDahuaGigE_AcquisitionFrameRateEnable:
			bReturn = !SetAcquisitionFrameRateEnable(_ttoi(strValue));
			break;
		case EDeviceParameterDahuaGigE_AcquisitionStatusSelector:
			bReturn = !SetAcquisitionStatusSelector((EDeviceDahuaGigEAcquisitionSelector)_ttoi(strValue));
			break;
		case EDeviceParameterDahuaGigE_TriggerSelector:
			bReturn = !SetTriggerSelector((EDeviceDahuaGigETriggerSelector)_ttoi(strValue));
			break;
		case EDeviceParameterDahuaGigE_FrameStartTriggerMode:
			bReturn = !SetFrameStartTriggerMode((EDeviceDahuaGigETriggerMode)_ttoi(strValue));
			break;
		case EDeviceParameterDahuaGigE_FrameStartTriggerSource:
			bReturn = !SetFrameStartTriggerSource((EDeviceDahuaGigETriggerSource)_ttoi(strValue));
			break;
		case EDeviceParameterDahuaGigE_FrameStartTriggerDelay:
			bReturn = !SetFrameStartTriggerDelay(_ttof(strValue));
			break;
		case EDeviceParameterDahuaGigE_AcquisitionStartTriggerMode:
			bReturn = !SetAcquisitionStartTriggerMode((EDeviceDahuaGigETriggerMode)_ttoi(strValue));
			break;
		case EDeviceParameterDahuaGigE_AcquisitionStartTriggerSource:
			bReturn = !SetAcquisitionStartTriggerSource((EDeviceDahuaGigETriggerSource)_ttoi(strValue));
			break;
		case EDeviceParameterDahuaGigE_AcquisitionStartTriggerDelay:
			bReturn = !SetAcquisitionStartTriggerDelay(_ttof(strValue));
			break;
		case EDeviceParameterDahuaGigE_LightTriggerDelay:
			bReturn = !SetLightTriggerDelay(_ttof(strValue));
			break;
		case EDeviceParameterDahuaGigE_ExposureMode:
			bReturn = !SetExposureMode((EDeviceDahuaGigEExposureMode)_ttoi(strValue));
			break;
		case EDeviceParameterDahuaGigE_ExposureTime:
			bReturn = !SetExposureTime(_ttof(strValue));
			break;
		case EDeviceParameterDahuaGigE_ExposureAuto:
			bReturn = !SetExposureAuto((EDeviceDahuaGigEExposureAuto)_ttoi(strValue));
			break;
		case EDeviceParameterDahuaGigE_GainSelector:
			bReturn = !SetGainSelector((EDeviceDahuaGigEGainSelector)_ttoi(strValue));
			break;
		case EDeviceParameterDahuaGigE_AllGainRaw:
			bReturn = !SetAllGainRaw(_ttof(strValue));
			break;
		case EDeviceParameterDahuaGigE_BlackLevelSelector:
			bReturn = !SetBlackLevelSelector((EDeviceDahuaGigEBlackLevelSelector)_ttoi(strValue));
			break;
		case EDeviceParameterDahuaGigE_BlackLevel:
			bReturn = !SetBlackLevel(_ttoi(strValue));
			break;
		case EDeviceParameterDahuaGigE_BlackLevelAuto:
			bReturn = !SetBlackLevelAuto((EDeviceDahuaGigEBlackLevelAuto)_ttoi(strValue));
			break;
		case EDeviceParameterDahuaGigE_Gamma:
			bReturn = !SetGamma(_ttof(strValue));
			break;
		case EDeviceParameterDahuaGigE_LineSelector:
			bReturn = !SetLineSelector((EDeviceDahuaGigELineSelector)_ttoi(strValue));
			break;
		case EDeviceParameterDahuaGigE_Line0Inverter:
			bReturn = !SetLine0Inverter((EDeviceDahuaGigELineInverter)_ttoi(strValue));
			break;
		case EDeviceParameterDahuaGigE_Line0Source:
			bReturn = !SetLine0Source((EDeviceDahuaGigELineSource)_ttoi(strValue));
			break;
		case EDeviceParameterDahuaGigE_Line0Format:
			bReturn = !SetLine0Format((EDeviceDahuaGigELineFormat)_ttoi(strValue));
			break;
		case EDeviceParameterDahuaGigE_Line1Inverter:
			bReturn = !SetLine1Inverter((EDeviceDahuaGigELineInverter)_ttoi(strValue));
			break;
		case EDeviceParameterDahuaGigE_Line1Source:
			bReturn = !SetLine1Source((EDeviceDahuaGigELineSource)_ttoi(strValue));
			break;
		case EDeviceParameterDahuaGigE_Line1Format:
			bReturn = !SetLine1Format((EDeviceDahuaGigELineFormat)_ttoi(strValue));
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

bool CDeviceDahuaGigE::DoesModuleExist()
{
	bool bReturn = false;

	CString strModuleName;

	do
	{
		strModuleName.Format(_T("MVSDKmd.dll"));

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

bool CDeviceDahuaGigE::GetDeviceValueBool(CString strCommand, bool * pData)
{
	bool bReturn = false;

	do
	{
		if(!pData)
			break;

		GENICAM_BoolNode *pNode = NULL;
		GENICAM_BoolNodeInfo genNodeInfo = { 0 };

		genNodeInfo.pCamera = m_pCamera;
		memcpy(genNodeInfo.attrName, CStringA(strCommand), strCommand.GetLength());

		if(GENICAM_createBoolNode(&genNodeInfo, &pNode))
			break;

		if(pNode->isReadable(pNode))
			break;

		uint32_t nValue = -1;

		if(pNode->getValue(pNode, &nValue))
			break;

		*pData = nValue;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceDahuaGigE::SetDeviceValueBool(CString strCommand, bool bData)
{
	bool bReturn = false;

	do
	{
		GENICAM_BoolNode *pNode = NULL;
		GENICAM_BoolNodeInfo genNodeInfo = { 0 };

		genNodeInfo.pCamera = m_pCamera;
		memcpy(genNodeInfo.attrName, CStringA(strCommand), strCommand.GetLength());

		if(GENICAM_createBoolNode(&genNodeInfo, &pNode))
			break;

		if(pNode->isWriteable(pNode))
			break;

		uint32_t nValue = bData;

		if(pNode->setValue(pNode, nValue))
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceDahuaGigE::GetDeviceValueInt(CString strCommand, int* pData)
{
	bool bReturn = false;

	do 
	{
		if(!pData)
			break;

		GENICAM_IntNode *pNode = NULL;
		GENICAM_IntNodeInfo genNodeInfo = { 0 };

		genNodeInfo.pCamera = m_pCamera;
		memcpy(genNodeInfo.attrName, CStringA(strCommand), strCommand.GetLength());

		if(GENICAM_createIntNode(&genNodeInfo, &pNode))
			break;

		if(pNode->isReadable(pNode))
			break;

		int64_t nValue = -1;

		if(pNode->getValue(pNode, &nValue))
			break;

		*pData = nValue;

		bReturn = true;
	} 
	while(false);
	
	return bReturn;
}

bool CDeviceDahuaGigE::SetDeviceValueInt(CString strCommand, int nData)
{
	bool bReturn = false;

	do
	{
		GENICAM_IntNode *pNode = NULL;
		GENICAM_IntNodeInfo genNodeInfo = { 0 };

		genNodeInfo.pCamera = m_pCamera;
		memcpy(genNodeInfo.attrName, CStringA(strCommand), strCommand.GetLength());

		if(GENICAM_createIntNode(&genNodeInfo, &pNode))
			break;

		if(pNode->isWriteable(pNode))
			break;

		int64_t nValue = nData;

		if(pNode->setValue(pNode, nValue))
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceDahuaGigE::GetDeviceValueDouble(CString strCommand, double* pData)
{
	bool bReturn = false;

	do
	{
		if(!pData)
			break;

		GENICAM_DoubleNode *pNode = NULL;
		GENICAM_DoubleNodeInfo genNodeInfo = { 0 };

		genNodeInfo.pCamera = m_pCamera;
		memcpy(genNodeInfo.attrName, CStringA(strCommand), strCommand.GetLength());

		if(GENICAM_createDoubleNode(&genNodeInfo, &pNode))
			break;

		if(pNode->isReadable(pNode))
			break;

		double dblValue = -1;

		if(pNode->getValue(pNode, &dblValue))
			break;

		*pData = dblValue;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceDahuaGigE::SetDeviceValueDouble(CString strCommand, double dblData)
{
	bool bReturn = false;

	do
	{
		GENICAM_DoubleNode *pNode = NULL;
		GENICAM_DoubleNodeInfo genNodeInfo = { 0 };

		genNodeInfo.pCamera = m_pCamera;
		memcpy(genNodeInfo.attrName, CStringA(strCommand), strCommand.GetLength());

		if(GENICAM_createDoubleNode(&genNodeInfo, &pNode))
			break;

		if(pNode->isWriteable(pNode))
			break;

		if(pNode->setValue(pNode, dblData))
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceDahuaGigE::GetDeviceValueEnum(CString strCommand, CString* pData)
{
	bool bReturn = false;

	do
	{
		if(!pData)
			break;

		GENICAM_EnumNode *pNode = NULL;
		GENICAM_EnumNodeInfo genNodeInfo = { 0 };

		genNodeInfo.pCamera = m_pCamera;
		memcpy(genNodeInfo.attrName, CStringA(strCommand), strCommand.GetLength());

		if(GENICAM_createEnumNode(&genNodeInfo, &pNode))
			break;

		if(pNode->isReadable(pNode))
			break;

		char arrValue[256] = { 0, };
		uint32_t nValue = 256;

		if(pNode->getValueSymbol(pNode, arrValue, &nValue))
			break;

		*pData = CString(arrValue);

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceDahuaGigE::SetDeviceValueEnum(CString strCommand, CString strData)
{
	bool bReturn = false;

	do
	{
		GENICAM_EnumNode *pNode = NULL;
		GENICAM_EnumNodeInfo genNodeInfo = { 0 };

		genNodeInfo.pCamera = m_pCamera;
		memcpy(genNodeInfo.attrName, CStringA(strCommand), strCommand.GetLength());

		if(GENICAM_createEnumNode(&genNodeInfo, &pNode))
			break;

		if(pNode->isWriteable(pNode))
			break;

		if(pNode->setValueBySymbol(pNode, CStringA(strData)))
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceDahuaGigE::GetDeviceValueString(CString strCommand, CString* pData)
{
	bool bReturn = false;

	do
	{
		if(!pData)
			break;

		GENICAM_StringNode *pNode = NULL;
		GENICAM_StringNodeInfo genNodeInfo = { 0 };

		genNodeInfo.pCamera = m_pCamera;
		memcpy(genNodeInfo.attrName, CStringA(strCommand), strCommand.GetLength());

		if(GENICAM_createStringNode(&genNodeInfo, &pNode))
			break;

		if(pNode->isReadable(pNode))
			break;

		char arrValue[256] = { 0, };
		uint32_t nValue = 256;

		if(pNode->getValue(pNode, arrValue, &nValue))
			break;

		*pData = CString(arrValue);

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceDahuaGigE::SetDeviceValueString(CString strCommand, CString strData)
{
	bool bReturn = false;

	do
	{
		GENICAM_StringNode *pNode = NULL;
		GENICAM_StringNodeInfo genNodeInfo = { 0 };

		genNodeInfo.pCamera = m_pCamera;
		memcpy(genNodeInfo.attrName, CStringA(strCommand), strCommand.GetLength());

		if(GENICAM_createStringNode(&genNodeInfo, &pNode))
			break;

		if(pNode->isWriteable(pNode))
			break;

		if(pNode->setValue(pNode, CStringA(strData)))
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

UINT CDeviceDahuaGigE::CallbackFunction(LPVOID pParam)
{
	CDeviceDahuaGigE* pInstance = (CDeviceDahuaGigE*)pParam;

	if(pInstance)
	{
		GENICAM_Frame* pFrame;

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

			--pInstance->m_nGrabCount;

			if(NULL == pInstance->m_pStreamSource)
				break;

			if(pInstance->m_pStreamSource->getFrame(pInstance->m_pStreamSource, &pFrame, INFINITE))
				break;

			if(pFrame->valid(pFrame))
			{
				pFrame->release(pFrame);

				continue;
			}
			memcpy(pCurrentBuffer, pFrame->getImage(pFrame), pFrame->getImageSize(pFrame));

			pFrame->release(pFrame);

			pInstance->ConnectImage(false);

			CEventHandlerManager::BroadcastOnAcquisition(pCurrentImage, pInstance->GetObjectID(), pInstance->GetImageView(), pInstance);

			if(pInstance->m_nGrabCount <= 0)
				pInstance->m_bIsGrabAvailable = true;
		} 
		while(pInstance->IsLive() || !pInstance->IsGrabAvailable());

		pInstance->m_pStreamSource->stopGrabbing(pInstance->m_pStreamSource);

		pInstance->m_bIsLive = false;
		pInstance->m_bIsGrabAvailable = true;
	}

	return 0;
}

#endif