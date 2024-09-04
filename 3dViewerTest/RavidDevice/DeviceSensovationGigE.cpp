#include "stdafx.h"

#include "DeviceSensovationGigE.h"

#ifdef USING_DEVICE

#include "../RavidCore/RavidRect.h"
#include "../RavidCore/RavidImage.h"
#include "../RavidCore/RavidDatabase.h"
#include "../RavidCore/RavidRecordset.h"

#include "../RavidFramework/UIManager.h"
#include "../RavidFramework/LogManager.h"
#include "../RavidFramework/DeviceManager.h"
#include "../RavidFramework/LibraryManager.h"
#include "../RavidFramework/FrameworkManager.h"
#include "../RavidFramework/EventHandlerManager.h"
#include "../RavidFramework/MultiLanguageManager.h"

#include "../RavidFramework/RavidTreeView.h" 
#include "../RavidFramework/RavidImageView.h"

#include "../Libraries/Includes/Sensovation/SVCameraApi.h"
#include "../Libraries/Includes/Sensovation/SVImageAPI.h"

#ifndef _WIN64
// svcameraapi.v2.0.dll
// svimageapi.v2.0.dll
#pragma comment(lib, COMMONLIB_PREFIX "Sensovation/SVCameraAPI.v2.0.lib")
#pragma comment(lib, COMMONLIB_PREFIX "Sensovation/SVImageAPI.v2.0.lib")
#endif

using namespace Ravid;
using namespace Ravid::Device;
using namespace Ravid::Database;
using namespace Ravid::Framework;
using namespace Ravid::Algorithms;
using namespace Ravid::Miscellaneous;

int CDeviceSensovationGigE::m_nCameraCount = 0;

IMPLEMENT_DYNAMIC(CDeviceSensovationGigE, CDeviceCamera)

BEGIN_MESSAGE_MAP(CDeviceSensovationGigE, CDeviceCamera)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static LPCTSTR g_lpszParamSensovationGigE[EDeviceParameterSensovationGigE_Count] =
{
	_T("DeviceID"),
	_T("Camera Setting"),
	_T("Grab count"),
	_T("Offset X"),
	_T("Offset Y"),
	_T("Canvas width"),
	_T("Canvas height"),
	_T("Binning"),
	_T("Real width"),
	_T("Real height"),	
	_T("Base Setting"),
	_T("Gain"),
	_T("Exposure time"),
	_T("Trigger mode"),	
};

static LPCTSTR g_lpszSensovationGigESwitch[EDeviceSensovationGigESwitch_Count] =
{
	_T("Off"),
	_T("On"),
};

static LPCTSTR g_lpszSensovationGigEBinning[EDeviceSensovationGigEBinning_Count] =
{
	_T("1"),
	_T("2"),
	_T("4"),
};

static LPCTSTR g_lpszSensovationGigETriggerMode[EDeviceSensovationGigETriggerMode_Count] =
{
	_T("Free running"),
	_T("External trigger"),
	_T("Software trigger"),
};
	

CDeviceSensovationGigE::CDeviceSensovationGigE()
{
}


CDeviceSensovationGigE::~CDeviceSensovationGigE()
{
	Terminate();

	if(m_pLiveThread)
	{
		delete m_pLiveThread;
		m_pLiveThread = nullptr;
	}
}

EDeviceInitializeResult CDeviceSensovationGigE::Initialize()
{
	EDeviceInitializeResult eReturn = EDeviceInitializeResult_UnknownError;

	CString strMessage, strStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("Sensovation"));
		
		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);

		SetStatus(strStatus);

		return EDeviceInitializeResult_NotFoundApiError;
	}

#ifndef _WIN64
	do 
	{
		if(IsInitialized())
		{
			strStatus = CMultiLanguageManager::GetString(ELanguageParameter_Initialized);

			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Thedevicehasbeenalreadyinitialized);

			eReturn = EDeviceInitializeResult_AlreadyInitializedError;

			break;
		}

		if(!m_nCameraCount)
		{
			if(SVCAM_InitLibrary() != SVAPI_OK)
			{
				strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntopenthelibrary);
				eReturn = EDeviceInitializeResult_NotInitLibraries;
				break;
			}
		}

		++m_nCameraCount;

		m_bLoadLib = true;

		UINT32 nDeviceCount = 0;

		if(SVCAM_GetNofDevices(nDeviceCount) != SVAPI_OK)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Devices Info"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		if(nDeviceCount < 1)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtofindthedevice);
			eReturn = EDeviceInitializeResult_NotFoundDeviceError;
			break;
		}

		bool bFoundDevice = false;

		for(UINT32 i = 0; i < nDeviceCount; ++i)
		{
			SSVDeviceInfo ssvDeviceInfo;
			ZeroMemory(&ssvDeviceInfo, sizeof(ssvDeviceInfo));

			if(SVCAM_GetDevice(i, ssvDeviceInfo) != SVAPI_OK)
				continue;
			
			char cName[64] = {};

			strcpy(cName, ssvDeviceInfo.szDeviceDesc);
			if(strcmp(cName, "Sensovation Samba CI Camera GigE"))
				continue;

			CStringA str = ssvDeviceInfo.szDeviceID;

			CStringA strDst = ssvDeviceInfo.szDeviceTypeByName;
			strDst += _T(":");
			strDst += GetDeviceID();

			if(str.CompareNoCase(strDst))
				continue;

			if(SVCAM_OpenCamera(ssvDeviceInfo.szDeviceLabel, nullptr, m_hCamera) != SVAPI_OK)
				continue;

			bFoundDevice = true;

			break;
		}

		if(!bFoundDevice)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntopenthedevice);
			eReturn = EDeviceInitializeResult_NotFoundDeviceError;
			break;
		}

		if(!m_hImage)
		{
			if(SVIMG_AllocImageObj(m_hImage) != SVAPI_OK)
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Allocate Image"));
				eReturn = EDeviceInitializeResult_NotCreateImagebuffer;
				break;
			}
		}



		double dblCurGain = 0., dblMinGain = 0., dblMaxGain = INT_MAX, dblGain = 0.;

		if(GetGain(&dblGain))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Gain"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(SVCAM_GetGain(m_hCamera, dblCurGain, &dblMinGain, &dblMaxGain) != SVAPI_OK)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Gain"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		if(dblGain == 0.)
		{
			if(SetGain(dblCurGain))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Gain"));
				eReturn = EDeviceInitializeResult_WriteToDatabaseError;
				break;
			}

			dblGain = dblCurGain;
		}


		if(dblGain < dblMinGain || dblGain > dblMaxGain)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("gain"));
			eReturn = EDeviceInitializeResult_RangeOfParameterError;
			break;
		}
		
		if(SVCAM_SetGain(m_hCamera, dblGain) != SVAPI_OK)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("gain"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}
		
		double dblCurExposure = 0., dblMinExposure = 0., dblMaxExposure = INT_MAX, dblExposure = 0.;

		if(GetExposureTime(&dblExposure))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ExposureTime"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(SVCAM_GetExposureTime(m_hCamera, dblCurExposure, &dblMinExposure, &dblMaxExposure) != SVAPI_OK)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("ExposureTime"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		if(dblExposure == 0.)
		{
			if(SetExposureTime(dblCurExposure))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("ExposureTime"));
				eReturn = EDeviceInitializeResult_WriteToDatabaseError;
				break;
			}

			dblExposure = dblCurExposure;
		}


		if(dblExposure < dblMinExposure || dblExposure > dblMaxExposure)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ExposureTime"));
			eReturn = EDeviceInitializeResult_RangeOfParameterError;
			break;
		}

		if(SVCAM_SetExposureTime(m_hCamera, dblExposure) != SVAPI_OK)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ExposureTime"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		EDeviceSensovationGigEBinning eBinning = EDeviceSensovationGigEBinning_Count;

		if(GetBinning(&eBinning))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Binning"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		DWORD dwBinning = pow(2, (int)eBinning);

		UINT32 dwBinningXY = 0;
		SSVCapArray* pValidValue = nullptr;

		if(::SVCAM_GetBinningXY(m_hCamera, dwBinningXY, &pValidValue) != SVAPI_OK)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Binning"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}
		
		bool bValidValue = false;

		DWORD dwSize = pValidValue->dwNofItems;

		for(DWORD i = 0; i < dwSize; ++i)
		{
			if(((INT32*)pValidValue->pItemList)[i] == dwBinning)
			{
				bValidValue = true;
				break;
			}
		}

		if(!bValidValue)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Binning value"));
			eReturn = EDeviceInitializeResult_NotSupportedDeviceError;
			break;
		}

		if(::SVCAM_SetBinningXY(m_hCamera, dwBinning) != SVAPI_OK)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Binning"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}
		
		int nOffsetX = 0;
		int nOffsetY = 0;

		int nSizeX = 0;
		int nSizeY = 0;

		if(GetCanvasWidth(&nSizeX))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("CanvasWidth"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(GetCanvasHeight(&nSizeY))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("CanvasHeight"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(GetOffsetX(&nOffsetX))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("OffsetX"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(GetOffsetY(&nOffsetY))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("OffsetY"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		SSVRect ssvROIRect;
		ZeroMemory(&ssvROIRect, sizeof(ssvROIRect));

		SIZE szMinInfo;
		ZeroMemory(&szMinInfo, sizeof(szMinInfo));

		SIZE szMaxInfo;
		ZeroMemory(&szMaxInfo, sizeof(szMaxInfo));

		if(SVCAM_GetROI(m_hCamera, ssvROIRect, &szMinInfo, &szMaxInfo) != SVAPI_OK)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("ROI info"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		if(nOffsetX < 0 || nOffsetX >= szMaxInfo.cx)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("offsetX"));
			eReturn = EDeviceInitializeResult_RangeOfParameterError;
			break;
		}

		if(nOffsetY < 0 || nOffsetY >= szMaxInfo.cy)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("offsetY"));
			eReturn = EDeviceInitializeResult_RangeOfParameterError;
			break;
		}

		if(!nSizeX)
			nSizeX = szMaxInfo.cx;

		if(!nSizeY)
			nSizeY = szMaxInfo.cy;

		int nConvertX = nSizeX + nOffsetX;
		int nConvertY = nSizeY + nOffsetY;

		if(nSizeX < szMinInfo.cx || nConvertX > szMaxInfo.cx)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Width"));
			eReturn = EDeviceInitializeResult_RangeOfParameterError;
			break;
		}

		if(nSizeY < szMinInfo.cy || nConvertY > szMaxInfo.cy)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Height"));
			eReturn = EDeviceInitializeResult_RangeOfParameterError;
			break;
		}

		ssvROIRect.ulStartX = nOffsetX;
		ssvROIRect.ulStartY = nOffsetY;
		ssvROIRect.ulWidth = nSizeX;
		ssvROIRect.ulHeight = nSizeY;

		if(SVCAM_SetROI(m_hCamera, ssvROIRect) != SVAPI_OK)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ROI Info"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		if(SetCanvasWidth(nSizeX))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Canvas Width"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(SetCanvasHeight(nSizeY))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Canvas Height"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		EDeviceSensovationGigETriggerMode eTrigMode = EDeviceSensovationGigETriggerMode_Count;

		if(GetTriggerMode(&eTrigMode))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TriggerMode"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		eSVTriggerModes eCurTrigMode = SV_TRIGGER_FREE_RUNNING;

		SSVCapArray * pValidValues = NULL;

		if(SVCAM_GetTriggerMode(m_hCamera, eCurTrigMode, &pValidValues) != SVAPI_OK)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("TriggerMode"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		bValidValue = false;

		for(DWORD i = 0; i < pValidValues->dwNofItems; ++i)
		{
 			if(((INT32*)pValidValues->pItemList)[i] == (eTrigMode))
 			{
 				bValidValue = true;
 				break;
			}
		}

		if(!bValidValue)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TriggerMode"));
			eReturn = EDeviceInitializeResult_NotSupportedDeviceError;
			break;
		}

		eCurTrigMode = (eSVTriggerModes)eTrigMode;

		if(::SVCAM_SetTriggerMode(m_hCamera, eCurTrigMode) != SVAPI_OK)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TriggerMode"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		SSVImageHeader ImgInfo;
		ZeroMemory(&ImgInfo, sizeof(ImgInfo));

		if(SVCAM_GetImageFormatInfo(m_hCamera, ImgInfo) != SVAPI_OK)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("ImageFormat"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		nSizeX = ImgInfo.dwWidthPix;
		nSizeY = ImgInfo.dwHeightPix;

		if(SetRealWidth(nSizeX))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Real Width"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(SetRealHeight(nSizeY))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Real Height"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		bool bColor = false;

		if(SVCAM_IsColor(m_hCamera, bColor) != SVAPI_OK)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("IsColorFormat"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		int nChannel = bColor ? 3 : 1;

		int nMaxValue = (1 << ImgInfo.dwNofBitsPerChannel) - 1;

		CMultipleVariable mv;
		for(int i = 0; i < ImgInfo.dwNofChannels; ++i)
			mv.AddValue(nMaxValue);

		this->InitBuffer(nSizeX, nSizeY, mv, CRavidImage::MakeValueFormat(ImgInfo.dwNofChannels, ImgInfo.dwNofBitsPerChannel), ImgInfo.dwNextLineBytes);

		this->ConnectImage();
	
		strStatus = CMultiLanguageManager::GetString(ELanguageParameter_Initialized);

		strMessage = CMultiLanguageManager::GetString(ELanguageParameter_TheDevicehasbeensuccessfullyinitialized);

		UpdateParameterList();

		m_bIsInitialized = true;

		eReturn = EDeviceInitializeResult_OK;
				
		CEventHandlerManager::BroadcastOnDeviceInitialized(this);
	}
	while(false);

	if(!IsInitialized())
		Terminate();
#else
	strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Itcantrunthex64process);
#endif
	
	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	SetStatus(strStatus);

	return eReturn;
}

EDeviceTerminateResult CDeviceSensovationGigE::Terminate()
{
	EDeviceTerminateResult eReturn = EDeviceTerminateResult_UnknownError;

	CString strMessage;

	SetStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{

		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("Sensovation"));

		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);

		return EDeviceTerminateResult_NotFoundApiError;
	}

#ifndef _WIN64
	do
	{
		m_bIsInitialized = false;
		m_bIsGrabAvailable = true;

		if(m_bLoadLib)
		{
			if(IsLive() || !IsGrabAvailable())
				Stop();

			if(m_hImage)
			{
				SVIMG_FreeImageObj(m_hImage);
				m_hImage = 0;
			}

			if(m_hCamera)
			{
				SVCAM_CloseCamera(m_hCamera);
				m_hCamera = 0;
			}

			--m_nCameraCount;

			m_bLoadLib = false;

			if(!m_nCameraCount)
				SVCAM_DeInitLibrary();
		}

		strMessage = CMultiLanguageManager::GetString(ELanguageParameter_TheDevicehasbeensuccessfullyterminated);

		eReturn = EDeviceTerminateResult_OK;
				
		CEventHandlerManager::BroadcastOnDeviceTerminated(this);
	}
	while(false);
#endif

	this->ClearBuffer();

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceSensovationGigE::LoadSettings()
{
	bool bReturn = false;

	m_vctParameterFieldConfigurations.clear();

	do
	{
		CString strTime;
		strTime.Format(_T("%d"), INT_MAX);
		
		AddParameterFieldConfigurations(EDeviceParameterSensovationGigE_DeviceID, g_lpszParamSensovationGigE[EDeviceParameterSensovationGigE_DeviceID], _T("0"), EParameterFieldType_Edit);
		AddParameterFieldConfigurations(EDeviceParameterSensovationGigE_CameraSetting, g_lpszParamSensovationGigE[EDeviceParameterSensovationGigE_CameraSetting], _T("1"), EParameterFieldType_None);
		AddParameterFieldConfigurations(EDeviceParameterSensovationGigE_GrabCount, g_lpszParamSensovationGigE[EDeviceParameterSensovationGigE_GrabCount], _T("1"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterSensovationGigE_OffsetX, g_lpszParamSensovationGigE[EDeviceParameterSensovationGigE_OffsetX], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterSensovationGigE_OffsetY, g_lpszParamSensovationGigE[EDeviceParameterSensovationGigE_OffsetY], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterSensovationGigE_CanvasWidth, g_lpszParamSensovationGigE[EDeviceParameterSensovationGigE_CanvasWidth], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterSensovationGigE_CanvasHeight, g_lpszParamSensovationGigE[EDeviceParameterSensovationGigE_CanvasHeight], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterSensovationGigE_Binning, g_lpszParamSensovationGigE[EDeviceParameterSensovationGigE_Binning], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszSensovationGigEBinning, EDeviceSensovationGigEBinning_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterSensovationGigE_StaticWidth, g_lpszParamSensovationGigE[EDeviceParameterSensovationGigE_StaticWidth], _T("0"), EParameterFieldType_Static, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterSensovationGigE_StaticHeight, g_lpszParamSensovationGigE[EDeviceParameterSensovationGigE_StaticHeight], _T("0"), EParameterFieldType_Static, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterSensovationGigE_BaseSetting, g_lpszParamSensovationGigE[EDeviceParameterSensovationGigE_BaseSetting], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterSensovationGigE_Gain, g_lpszParamSensovationGigE[EDeviceParameterSensovationGigE_Gain], _T("0.000000"), EParameterFieldType_Edit, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterSensovationGigE_ExposureTime, g_lpszParamSensovationGigE[EDeviceParameterSensovationGigE_ExposureTime], _T("0.000000"), EParameterFieldType_Edit, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterSensovationGigE_TriggerMode, g_lpszParamSensovationGigE[EDeviceParameterSensovationGigE_TriggerMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszSensovationGigETriggerMode, EDeviceSensovationGigETriggerMode_Count), nullptr, 2);
		

		bReturn = true;
	}
	while(false);

	return bReturn & __super::LoadSettings();
}

EDeviceGrabResult CDeviceSensovationGigE::Grab()
{
	EDeviceGrabResult eReturn = EDeviceGrabResult_UnknownError;

	CString strMessage;

#ifndef _WIN64
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

		if(SVCAM_StartGrab(m_hCamera) != SVAPI_OK)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("StartGrab"));
			eReturn = EDeviceGrabResult_WriteToDeviceError;
			break;
		}

		m_bIsGrabAvailable = false;

		m_pLiveThread = AfxBeginThread(CDeviceSensovationGigE::CallbackFunction, this, 0, 0U, CREATE_SUSPENDED);

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
#else
	strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Itcantrunthex64process);
#endif

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceLiveResult CDeviceSensovationGigE::Live()
{
	EDeviceLiveResult eReturn = EDeviceLiveResult_UnknownError;

	CString strMessage;

#ifndef _WIN64
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

		if(SVCAM_StartGrab(m_hCamera) != SVAPI_OK)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("StartGrab"));
			eReturn = EDeviceLiveResult_WriteToDeviceError;
			break;
		}

		m_bIsGrabAvailable = false;

		m_pLiveThread = AfxBeginThread(CDeviceSensovationGigE::CallbackFunction, this, 0, 0U, CREATE_SUSPENDED);

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
#else
	strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Itcantrunthex64process);
#endif
	
	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceStopResult CDeviceSensovationGigE::Stop()
{
	EDeviceStopResult eReturn = EDeviceStopResult_UnknownError;

	CString strMessage;

#ifndef _WIN64
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

		if(SVCAM_Freeze(m_hCamera) != SVAPI_OK)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("StopGrab"));
			eReturn = EDeviceStopResult_WriteToDeviceError;
			break;
		}


		if(WaitForSingleObject(m_pLiveThread->m_hThread, 1000) == WAIT_TIMEOUT)
		{
		}

		m_bIsGrabAvailable = true;
		m_pLiveThread = nullptr;

		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("Stop"));

		eReturn = EDeviceStopResult_OK;
	} 
	while(false);
#else
	strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Itcantrunthex64process);
#endif

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceTriggerResult CDeviceSensovationGigE::Trigger()
{
	EDeviceTriggerResult eReturn = EDeviceTriggerResult_UnknownError;

	CString strMessage;
#ifndef _WIN64
	do 
	{
		if(!IsInitialized())
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Ithasnotbeeninitializedyet);
			eReturn = EDeviceTriggerResult_NotInitializedError;
			break;
		}

		if(SVCAM_DoTrigger(m_hCamera) != SVAPI_OK)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Trigger"));
			break;
		}

		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("Trigger"));

		eReturn = EDeviceTriggerResult_OK;
	} 
	while(false);
#else
strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Itcantrunthex64process);
#endif

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESensovationGetFunction CDeviceSensovationGigE::GetGrabCount(int* pParam)
{
	ESensovationGetFunction eReturn = ESensovationGetFunction_UnknownError;

	do 
	{
		if(!pParam)
		{
			eReturn = ESensovationGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterSensovationGigE_GrabCount));

		eReturn = ESensovationGetFunction_OK;
	} 
	while(false);		

	return eReturn;
}

ESensovationSetFunction CDeviceSensovationGigE::SetGrabCount(int nParam)
{
	ESensovationSetFunction eReturn = ESensovationSetFunction_UnknownError;

	EDeviceParameterSensovationGigE eSaveID = EDeviceParameterSensovationGigE_GrabCount;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do 
	{
		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = ESensovationSetFunction_WriteToDatabaseError;
			break;
		}
		
		if(!SaveSettings(eSaveID))
		{
			eReturn = ESensovationSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ESensovationSetFunction_OK;
	} 
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSensovationGigE[eSaveID], nPreValue, nParam);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

ESensovationGetFunction CDeviceSensovationGigE::GetOffsetX(int* pParam)
{
	ESensovationGetFunction eReturn = ESensovationGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESensovationGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterSensovationGigE_OffsetX));

		eReturn = ESensovationGetFunction_OK;
	}
	while(false);		

	return eReturn;
}

ESensovationSetFunction CDeviceSensovationGigE::SetOffsetX(int nParam)
{
	ESensovationSetFunction eReturn = ESensovationSetFunction_UnknownError;

	EDeviceParameterSensovationGigE eSaveID = EDeviceParameterSensovationGigE_OffsetX;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do 
	{
		if(IsInitialized())
		{
			eReturn = ESensovationSetFunction_AlreadyInitializedError;
			break;
		}

		CString strOffset;
		strOffset.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strOffset))
		{
			eReturn = ESensovationSetFunction_WriteToDeviceError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ESensovationSetFunction_WriteToDeviceError;
			break;
		}

		eReturn = ESensovationSetFunction_OK;
	} 
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSensovationGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESensovationGetFunction CDeviceSensovationGigE::GetOffsetY(int* pParam)
{
	ESensovationGetFunction eReturn = ESensovationGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESensovationGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterSensovationGigE_OffsetY));

		eReturn = ESensovationGetFunction_OK;
	}
	while(false);		

	return eReturn;
}

ESensovationSetFunction CDeviceSensovationGigE::SetOffsetY(int nParam)
{
	ESensovationSetFunction eReturn = ESensovationSetFunction_UnknownError;

	EDeviceParameterSensovationGigE eSaveID = EDeviceParameterSensovationGigE_OffsetY;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = ESensovationSetFunction_AlreadyInitializedError;
			break;
		}

		CString strOffset;
		strOffset.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strOffset))
		{
			eReturn = ESensovationSetFunction_WriteToDeviceError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ESensovationSetFunction_WriteToDeviceError;
			break;
		}

		eReturn = ESensovationSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSensovationGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESensovationGetFunction CDeviceSensovationGigE::GetCanvasWidth(int* pParam)
{
	ESensovationGetFunction eReturn = ESensovationGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESensovationGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterSensovationGigE_CanvasWidth));

		eReturn = ESensovationGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESensovationSetFunction CDeviceSensovationGigE::SetCanvasWidth(int nParam)
{
	ESensovationSetFunction eReturn = ESensovationSetFunction_UnknownError;

	EDeviceParameterSensovationGigE eSaveID = EDeviceParameterSensovationGigE_CanvasWidth;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = ESensovationSetFunction_AlreadyInitializedError;
			break;
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = ESensovationSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ESensovationSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ESensovationSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSensovationGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESensovationGetFunction CDeviceSensovationGigE::GetCanvasHeight(int* pParam)
{
	ESensovationGetFunction eReturn = ESensovationGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESensovationGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterSensovationGigE_CanvasHeight));

		eReturn = ESensovationGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESensovationSetFunction CDeviceSensovationGigE::SetCanvasHeight(int nParam)
{
	ESensovationSetFunction eReturn = ESensovationSetFunction_UnknownError;

	EDeviceParameterSensovationGigE eSaveID = EDeviceParameterSensovationGigE_CanvasHeight;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = ESensovationSetFunction_AlreadyInitializedError;
			break;
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = ESensovationSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ESensovationSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ESensovationSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSensovationGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESensovationGetFunction CDeviceSensovationGigE::GetBinning(EDeviceSensovationGigEBinning * pParam)
{
	ESensovationGetFunction eReturn = ESensovationGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESensovationGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceSensovationGigEBinning)_ttoi(GetParamValue(EDeviceParameterSensovationGigE_Binning));

		eReturn = ESensovationGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESensovationSetFunction CDeviceSensovationGigE::SetBinning(EDeviceSensovationGigEBinning eParam)
{
	ESensovationSetFunction eReturn = ESensovationSetFunction_UnknownError;

	EDeviceParameterSensovationGigE eSaveID = EDeviceParameterSensovationGigE_Binning;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = ESensovationSetFunction_AlreadyInitializedError;
			break;
		}

		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = ESensovationSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ESensovationSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ESensovationSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSensovationGigE[eSaveID], g_lpszSensovationGigEBinning[nPreValue], g_lpszSensovationGigEBinning[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESensovationGetFunction CDeviceSensovationGigE::GetRealWidth(int* pParam)
{
	ESensovationGetFunction eReturn = ESensovationGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESensovationGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterSensovationGigE_StaticWidth));

		eReturn = ESensovationGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESensovationSetFunction CDeviceSensovationGigE::SetRealWidth(int nParam)
{
	ESensovationSetFunction eReturn = ESensovationSetFunction_UnknownError;

	EDeviceParameterSensovationGigE eSaveID = EDeviceParameterSensovationGigE_StaticWidth;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = ESensovationSetFunction_AlreadyInitializedError;
			break;
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = ESensovationSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ESensovationSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ESensovationSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSensovationGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESensovationGetFunction CDeviceSensovationGigE::GetRealHeight(int* pParam)
{
	ESensovationGetFunction eReturn = ESensovationGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESensovationGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterSensovationGigE_StaticHeight));

		eReturn = ESensovationGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESensovationSetFunction CDeviceSensovationGigE::SetRealHeight(int nParam)
{
	ESensovationSetFunction eReturn = ESensovationSetFunction_UnknownError;

	EDeviceParameterSensovationGigE eSaveID = EDeviceParameterSensovationGigE_StaticHeight;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = ESensovationSetFunction_AlreadyInitializedError;
			break;
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = ESensovationSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ESensovationSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ESensovationSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSensovationGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESensovationGetFunction CDeviceSensovationGigE::GetExposureTime(double* pParam)
{
	ESensovationGetFunction eReturn = ESensovationGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESensovationGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterSensovationGigE_ExposureTime));

		eReturn = ESensovationGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESensovationSetFunction CDeviceSensovationGigE::SetExposureTime(double dblParam)
{
	ESensovationSetFunction eReturn = ESensovationSetFunction_UnknownError;

	EDeviceParameterSensovationGigE eSaveID = EDeviceParameterSensovationGigE_ExposureTime;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
#ifndef _WIN64
			double dblCurExposure = 0., dblMinExposure = 0., dblMaxExposure = INT_MAX;

			if(SVCAM_GetExposureTime(m_hCamera, dblCurExposure, &dblMinExposure, &dblMaxExposure) != SVAPI_OK)
			{
				eReturn = ESensovationSetFunction_ReadOnDeviceError;
				break;
			}
			
			if(dblParam < dblMinExposure || dblParam > dblMaxExposure)
			{
				eReturn = ESensovationSetFunction_NotSupportError;
				break;
			}

			if(SVCAM_SetExposureTime(m_hCamera, dblParam) != SVAPI_OK)
			{
				eReturn = ESensovationSetFunction_WriteToDeviceError;
				break;
			}
#endif
		}

		CString strValue;
		strValue.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = ESensovationSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ESensovationSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ESensovationSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSensovationGigE[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESensovationGetFunction CDeviceSensovationGigE::GetGain(double* pParam)
{
	ESensovationGetFunction eReturn = ESensovationGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESensovationGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterSensovationGigE_Gain));

		eReturn = ESensovationGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESensovationSetFunction CDeviceSensovationGigE::SetGain(double dblParam)
{
	ESensovationSetFunction eReturn = ESensovationSetFunction_UnknownError;

	EDeviceParameterSensovationGigE eSaveID = EDeviceParameterSensovationGigE_Gain;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
#ifndef _WIN64
			double dblCurGain = 0., dblMinGain = 0., dblMaxGain = INT_MAX;

			if(SVCAM_GetGain(m_hCamera, dblCurGain, &dblMinGain, &dblMaxGain) != SVAPI_OK)
			{
				eReturn = ESensovationSetFunction_ReadOnDeviceError;
				break;
			}
			
			if(dblParam < dblMinGain || dblParam > dblMaxGain)
			{
				eReturn = ESensovationSetFunction_NotSupportError;
				break;
			}

			if(SVCAM_SetGain(m_hCamera, dblParam) != SVAPI_OK)
			{
				eReturn = ESensovationSetFunction_WriteToDeviceError;
				break;
			}
#endif
		}

		CString strValue;
		strValue.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = ESensovationSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ESensovationSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ESensovationSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSensovationGigE[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESensovationGetFunction CDeviceSensovationGigE::GetTriggerMode(EDeviceSensovationGigETriggerMode * pParam)
{
	ESensovationGetFunction eReturn = ESensovationGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESensovationGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceSensovationGigETriggerMode)_ttoi(GetParamValue(EDeviceParameterSensovationGigE_TriggerMode));

		eReturn = ESensovationGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESensovationSetFunction CDeviceSensovationGigE::SetTriggerMode(EDeviceSensovationGigETriggerMode eParam)
{
	ESensovationSetFunction eReturn = ESensovationSetFunction_UnknownError;

	EDeviceParameterSensovationGigE eSaveID = EDeviceParameterSensovationGigE_TriggerMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
#ifndef _WIN64
			eSVTriggerModes eCurTrigMode = SV_TRIGGER_FREE_RUNNING;

			SSVCapArray * pValidValues = NULL;

			if(SVCAM_GetTriggerMode(m_hCamera, eCurTrigMode, &pValidValues) != SVAPI_OK)
			{
				eReturn = ESensovationSetFunction_ReadOnDeviceError;
				break;
			}

			bool bValidValue = false;

			for(DWORD i = 0; i < pValidValues->dwNofItems; ++i)
			{
				if(((INT32*)pValidValues->pItemList)[i] == (eParam))
				{
					bValidValue = true;
					break;
				}
			}

			if(!bValidValue)
			{
				eReturn = ESensovationSetFunction_NotSupportError;
				break;
			}

			eCurTrigMode = (eSVTriggerModes)eParam;

			if(::SVCAM_SetTriggerMode(m_hCamera, eCurTrigMode) != SVAPI_OK)
			{
				eReturn = ESensovationSetFunction_WriteToDeviceError;
				break;
			}
#endif
		}

		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = ESensovationSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ESensovationSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ESensovationSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSensovationGigE[eSaveID], g_lpszSensovationGigETriggerMode[nPreValue], g_lpszSensovationGigETriggerMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceSensovationGigE::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = false;

	bool bFoundID = true;

	do
	{
		switch(nParam)
		{
		case EDeviceParameterSensovationGigE_DeviceID:
			bReturn = !SetDeviceID(strValue);
			break;
		case EDeviceParameterSensovationGigE_GrabCount:
			bReturn = !SetGrabCount(_ttoi(strValue));
			break;
		case EDeviceParameterSensovationGigE_CanvasWidth:
			bReturn = !SetCanvasWidth(_ttoi(strValue));
			break;
		case EDeviceParameterSensovationGigE_CanvasHeight:
			bReturn = !SetCanvasHeight(_ttoi(strValue));
			break;
		case EDeviceParameterSensovationGigE_OffsetX:
			bReturn = !SetOffsetX(_ttoi(strValue));
			break;
		case EDeviceParameterSensovationGigE_OffsetY:
			bReturn = !SetOffsetY(_ttoi(strValue));
			break;
		case EDeviceParameterSensovationGigE_Binning:
			bReturn = !SetBinning((EDeviceSensovationGigEBinning)_ttoi(strValue));
			break;
		case EDeviceParameterSensovationGigE_ExposureTime:
			bReturn = !SetExposureTime(_ttof(strValue));
			break;
		case EDeviceParameterSensovationGigE_Gain:
			bReturn = !SetGain(_ttof(strValue));
			break;
		case EDeviceParameterSensovationGigE_TriggerMode:
			bReturn = !SetTriggerMode((EDeviceSensovationGigETriggerMode)_ttoi(strValue));
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

bool CDeviceSensovationGigE::DoesModuleExist()
{
	bool bReturn = false;

	CString strModuleName;

	do
	{
		strModuleName.Format(_T("SVCameraAPI.v2.0.dll"));

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

		strModuleName.Format(_T("SVImageAPI.v2.0.dll"));

		eStatus = CLibraryManager::GetFoundLibrary(strModuleName);

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

UINT CDeviceSensovationGigE::CallbackFunction(LPVOID pParam)
{
	CDeviceSensovationGigE* pInstance = (CDeviceSensovationGigE*)pParam;

#ifndef _WIN64
	if(pInstance)
	{
		do
		{
			if(!pInstance->IsInitialized())
				break;

			UINT32 dwDeviceCount = 0;
			SSVImageData sImageData;
			SSVIAMAcquiredFrameInfo sFrameInfo;

			
			do
			{
				SSVCameraStatus sCamStatus;

				if(::SVCAM_GetCameraStatus(pInstance->m_hCamera, sCamStatus) != SVAPI_OK)
					continue;

				if(sCamStatus.dwCameraStatus != SV_CAM_STATUS_ACQ)
					break;

				if(sCamStatus.dwGrabNofPendingImages >= 1)
				{
					if(::SVCAM_WaitNextSVImage(pInstance->m_hCamera, INFINITE, sFrameInfo, pInstance->m_hImage) != SVAPI_OK)
						continue;

					if(::SVIMG_AccessImageData(pInstance->m_hImage, sImageData) == SVAPI_OK)
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


						DWORD dwHeight = sImageData.ImageInfo.dwHeightPix;
						DWORD dwWidth = sImageData.ImageInfo.dwWidthPix;
						DWORD dwBpp = sImageData.ImageInfo.dwNofBitsPerChannel;
						DWORD dwSize = sImageData.ImageInfo.dwImageSizeBytes;

						memcpy(pCurrentBuffer, sImageData.pData, sImageData.ImageInfo.dwImageSizeBytes);

						--pInstance->m_nGrabCount;

						pInstance->ConnectImage(false);

						CEventHandlerManager::BroadcastOnAcquisition(pCurrentImage, pInstance->GetObjectID(), pInstance->GetImageView(), pInstance);
					}

				}
			}
			while(pInstance->IsInitialized() && (pInstance->IsLive() || pInstance->m_nGrabCount != 0));
		}
		while(false);

		pInstance->m_bIsLive = false;
		pInstance->m_bIsGrabAvailable = true;
	}
#endif

	return 0;
}

#endif