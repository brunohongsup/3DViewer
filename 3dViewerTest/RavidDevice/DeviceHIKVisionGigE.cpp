#include "stdafx.h"

#include "DeviceHIKVisionGigE.h"

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

#include "../Libraries/Includes/HikVision/TlFactory.h"
#include "../Libraries/Includes/HikVision/CameraParams.h"
#include "../Libraries/Includes/HikVision/MvGigEDevice.h"
#include "../Libraries/Includes/HikVision/MvCameraControl.h"

#ifndef _WIN64
#pragma comment(lib, COMMONLIB_PREFIX "HikVision/win32/MvCameraControl.lib")
#else
#pragma comment(lib, COMMONLIB_PREFIX "HikVision/win64/MvCameraControl.lib")
#endif

/////////////////////////////////////////////////////////////////////
//
// Title : Device - HIKVision
// Author : Dong-hee Han
// Creation Date : Jan 22, 2019
// Detail : HIKVisionDevice Add Features
//			Supported Model :   Model Name - MV-CA050-10GM
//								Model Name - MV-CA013-20GM
//          Only Supported 1Channel 8Depth, 10Depth, 12Depth    when 10PackedDepth, 12PackedDepth : 10PackedDepth -> 8Depth,  12PackedDepth -> 8Depth 
///////////////////////////////////////////////////////

using namespace Ravid;
using namespace Ravid::Device;
using namespace Ravid::Database;
using namespace Ravid::Framework;
using namespace Ravid::Algorithms;
using namespace Ravid::Miscellaneous;

IMPLEMENT_DYNAMIC(CDeviceHIKVisionGigE, CDeviceCamera)

BEGIN_MESSAGE_MAP(CDeviceHIKVisionGigE, CDeviceCamera)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static LPCTSTR g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_Count] = //화면 다이알로그 띄울 배열
{
	_T("DeviceID"),
	_T("Initialize Mode"),
	_T("Device Control"),
	_T("Device Stram Channel Packet Size"),
	_T("Device Grab Count"),
	_T("Image Format Control"),
	_T("Width"),
	_T("Height"),
	_T("Offset X"),
	_T("Offset Y"),
	_T("Reverse X"),
	_T("Reverse Y"),
	_T("Pixel Format"),
	_T("Acquisition Control"),
	_T("Acquisition Mode"),
	_T("Acquisition Burst Frame Count"),
	_T("Acquisition Frame Rate(Fps)"),
	_T("Acquisition Frame Rate Control Enable"),
	_T("Acquisition Trigger Selector"),
	_T("Acquisition TriggerMode"),
	_T("Acquisition TriggerSource"),
	_T("Acquisition TriggerActivation"),
	_T("Acquisition Trigger Delay(us)"),
	_T("Acquisition Exposure Mode"),
	_T("Acquisition Exposure Time(us)"),
	_T("Acquisition Exposure Auto"),
	_T("AnalogControl"),
	_T("Gain(dB)"),
	_T("Gain Auto"),
	_T("Digital Shift"),
	_T("Digital Shift Enable"),
	_T("Black Level"),
	_T("Black Level Enable"),
	_T("DigitalIOControl"),
	_T("Line Selector"),
	_T("Line0"),
	_T("Line0 Only Input"),
	_T("Line0 DebouncerTime(us)"),
	_T("Line1"),
	_T("Line1 Only Strob"),
	_T("Line1 Inverter"),
	_T("Line1 Source"),
	_T("Line1 Strobe Enable"),
	_T("Line1 Strobe Line Duration(us)"),
	_T("Line1 Strobe Line Delay(us)"),
	_T("Line1 Strobe Line PreDelay(us)"),
	_T("Line1 Strobe Hard Trigger Active Mode"),
	_T("Line1 Strobe Hard Trigger Source"),
	_T("Line1 Strobe Hard Trigger Activation"),
	_T("Line2"),
	_T("Line2 Mode"),
	_T("Line2 Input Mode"),
	_T("Line2 DebouncerTime(us)"),
	_T("Line2 Strob Mode"),
	_T("Line2 Inverter"),
	_T("Line2 Source"),
	_T("Line2 Strobe Enable"),
	_T("Line2 Strobe Line Duration(us)"),
	_T("Line2 Strobe Line Delay(us)"),
	_T("Line2 Strobe Line PreDelay(us)"),
	_T("Line2 Strobe Hard Trigger Active Mode"),
	_T("Line2 Strobe Hard Trigger Source"),
	_T("Line2 Strobe Hard Trigger Activation"),
};

static LPCTSTR g_lpszHIKVisionGigEInitMode[EDeviceHIKVisionGigEInitMode_Count] =
{
	_T("Device Only"),
	_T("Parameter"),
};

static const char *ModelName[EDeviceHIKVisionGigEModel_Count] =
{
	"MV-CA050-10GM",
	"MV-CA013-20GM",
};

static LPCTSTR g_lpszHIKVisionGigTriggerAcivation[EDeviceHIKVisionGigETriggerActivation_Count] =
{
	_T("Rising Edge"),
	_T("Falling Edge"),
};

static LPCTSTR g_lpszHIKVisionGigAcquisitionMode[EDeviceHIKVisionAcquisitionMode_Count] =
{
	_T("Continuos"),
	_T("SingleFrame"),
};

static LPCTSTR g_lpszHIKVisionGigEPixelFormat[EDeviceHIKVisionGigEPixelFormat_Count] =
{
	_T("Mono8"),
	_T("Mono10"),
	_T("Mono10Packed"),
	_T("Mono12"),
	_T("Mono12Packed"),
	_T("RGB8packed"),
	_T("YUV422_YUYV_Packed"),
	_T("YUV422Packed"),
	_T("BayerRG8"),
	_T("BayerRG10"),
	_T("BayerRG10Packed"),
	_T("BayerRG12"),
	_T("BayerRG12Packed"),

};

static LPCTSTR g_lpszHIKVisionGigETriggerSource[EDeviceHIKVisionGigETriggerSource_Count] =
{
	_T("SoftWare"),
	_T("Line0"),
	_T("Line2"),
	_T("Counter 0"),
};

static LPCTSTR g_lpszHIKVisionGigEExposureMode[EDeviceHIKVisionGigEAcquisitionExposureMode_Count] =
{
	_T("Timed"),
};

static LPCTSTR g_lpszHIKVisionGigESwitch[EDeviceHIKVisionGigESwitch_Count] =
{
	_T("Off"),
	_T("On")
};

static LPCTSTR g_lpszHIKVisionGigELineSelect[EDeviceHIKVisionGigELineSelect_Count] =
{
	_T("Line0"),
	_T("Line1"),
	_T("Line2")
};

static LPCTSTR g_lpszHIKVisionGigEHardTriggerLineSelect[EDeviceHIKVisionGigEHardTriggerLineSelect_Count] =
{
	_T("Line0"),
	_T("Line2")
};

static LPCTSTR g_lpszHIKVisionGigEAuto[EDeviceHIKVisionGigEGainAuto_Count] =
{
	_T("Off"),
	_T("Once"),
	_T("Continuous")
};

static LPCTSTR g_lpszHIKVisionGigELineMode[EDeviceHIKVisionGigELineMode_Count] =
{
	_T("Input"),
	_T("Strobe"),
};


static LPCTSTR g_lpszHIKVisionGigEAcquisitionTriggerSelector[EDeviceHIKVisionGigEAcquisitionTriggerSelector_Count] =
{
	_T("Frame Burst Start"),
};

static LPCTSTR g_lpszHIKVisionGigELineSource[EDeviceHIKVisionGigELineSource_Count] =
{
	_T("ExposureStartActive"),
	_T("AcquisitionStartActive"),
	_T("AcquisitionStopActive"),
	_T("FrameBurstStartActive"),
	_T("FrameBurstEndActive"),
	_T("SoftTriggerActive"),
	_T("HardTriggerActive"),
	_T("CounterActive"),
	_T("TimerActive"),
};

void __stdcall CallbackFunction(unsigned char * pData, MV_FRAME_OUT_INFO* pFrameInfo, void* pUser)
{
	CDeviceHIKVisionGigE* pInstance = (CDeviceHIKVisionGigE*)pUser;

	unsigned char* pImage = nullptr;

	do 
	{
		if(!pFrameInfo)
			break;

		pInstance->NextImageIndex();

		CRavidImage* pCurrentImage = pInstance->GetImageInfo();

		BYTE* pCurrentBuffer = pCurrentImage->GetBuffer();
		BYTE** ppCurrentOffsetY = pCurrentImage->GetYOffsetTable();

		const int64_t i64Width = (int64_t)pCurrentImage->GetSizeX();
		const int64_t i64Height = (int64_t)pCurrentImage->GetSizeY();
		const int64_t i64WidthStep = (int64_t)pCurrentImage->GetWidthStep();
		const int64_t i64ImgChannels = (int64_t)pCurrentImage->GetChannels();
		const int64_t i64ImageSizeByte = (int64_t)pCurrentImage->GetImageSizeByte();
		const int64_t i64PixelSizeByte = (int64_t)pCurrentImage->GetPixelSizeByte();

		if(!pInstance->GetImageInfo()->IsColor())
		{
			switch(pFrameInfo->enPixelType)
			{
			case PixelType_Gvsp_Mono8:
			case PixelType_Gvsp_Mono10:
				{

					for(int64_t i = 0; i < i64Height; ++i)
						memcpy(*(ppCurrentOffsetY + i), (BYTE*)(pData)+(i * i64Width * i64PixelSizeByte), i64WidthStep);
				}
				break;

			case PixelType_Gvsp_Mono10_Packed:
				{
					MV_CC_PIXEL_CONVERT_PARAM stParam;
					memset(&stParam, 0, sizeof(MV_CC_PIXEL_CONVERT_PARAM));

					stParam.pSrcData = pData;
					stParam.nSrcDataLen = pFrameInfo->nFrameLen;
					stParam.enSrcPixelType = pFrameInfo->enPixelType;
					stParam.nWidth = pFrameInfo->nWidth;
					stParam.nHeight = pFrameInfo->nHeight;
					stParam.enDstPixelType = PixelType_Gvsp_Mono8;
					stParam.nDstBufferSize = pFrameInfo->nWidth * pFrameInfo->nHeight;
					pImage = (unsigned char*)malloc(stParam.nDstBufferSize);
					stParam.pDstBuffer = pImage;

					if(MV_CC_ConvertPixelType(pInstance->GetDeviceHandler(), &stParam))
						break;

					pData = pImage;

					for(int64_t i = 0; i < i64Height; ++i)
						memcpy(*(ppCurrentOffsetY + i), (BYTE*)(pData)+(i * i64Width * i64PixelSizeByte), i64WidthStep);
				}
				break;
			case PixelType_Gvsp_Mono12:
				{
					for(int64_t i = 0; i < i64Height; ++i)
						memcpy(*(ppCurrentOffsetY + i), (BYTE*)(pData)+(i * i64Width * i64PixelSizeByte), i64WidthStep);
				}
				break;
			case PixelType_Gvsp_Mono12_Packed:
				{
					MV_CC_PIXEL_CONVERT_PARAM stParam;
					memset(&stParam, 0, sizeof(MV_CC_PIXEL_CONVERT_PARAM));

					stParam.pSrcData = pData;
					stParam.nSrcDataLen = pFrameInfo->nFrameLen;
					stParam.enSrcPixelType = pFrameInfo->enPixelType;
					stParam.nWidth = pFrameInfo->nWidth;
					stParam.nHeight = pFrameInfo->nHeight;
					stParam.enDstPixelType = PixelType_Gvsp_Mono8;
					stParam.nDstBufferSize = pFrameInfo->nWidth * pFrameInfo->nHeight * i64ImgChannels;
					pImage = (unsigned char*)malloc(stParam.nDstBufferSize);
					stParam.pDstBuffer = pImage;

					if(MV_CC_ConvertPixelType(pInstance->GetDeviceHandler(), &stParam))
						break;

					int nDstLen = stParam.nDstLen;
					pData = pImage;

					for(int64_t i = 0; i < i64Height; ++i)
						memcpy(*(ppCurrentOffsetY + i), (BYTE*)(pData)+(i * i64Width * i64PixelSizeByte), i64WidthStep);
				}
				break;
			}

			pInstance->IncreaseGrabCount();

		}
		else
		{
			switch(pFrameInfo->enPixelType)
			{
			case PixelType_Gvsp_BayerGR8:
			case PixelType_Gvsp_BayerRG8:
			case PixelType_Gvsp_BayerGB8:
			case PixelType_Gvsp_BayerBG8:
			case PixelType_Gvsp_BayerGR10:
			case PixelType_Gvsp_BayerRG10:
			case PixelType_Gvsp_BayerGB10:
			case PixelType_Gvsp_BayerBG10:
			case PixelType_Gvsp_BayerGR12:
			case PixelType_Gvsp_BayerRG12:
			case PixelType_Gvsp_BayerGB12:
			case PixelType_Gvsp_BayerBG12:
			case PixelType_Gvsp_BayerGR10_Packed:
			case PixelType_Gvsp_BayerRG10_Packed:
			case PixelType_Gvsp_BayerGB10_Packed:
			case PixelType_Gvsp_BayerBG10_Packed:
			case PixelType_Gvsp_BayerGR12_Packed:
			case PixelType_Gvsp_BayerRG12_Packed:
			case PixelType_Gvsp_BayerGB12_Packed:
			case PixelType_Gvsp_BayerBG12_Packed:
			case PixelType_Gvsp_BayerGR16:
			case PixelType_Gvsp_BayerRG16:
			case PixelType_Gvsp_BayerGB16:
			case PixelType_Gvsp_BayerBG16:
			case PixelType_Gvsp_RGB8_Packed:
			case PixelType_Gvsp_BGR8_Packed:
			case PixelType_Gvsp_RGBA8_Packed:
			case PixelType_Gvsp_BGRA8_Packed:
			case PixelType_Gvsp_RGB10_Packed:
			case PixelType_Gvsp_BGR10_Packed:
			case PixelType_Gvsp_RGB12_Packed:
			case PixelType_Gvsp_BGR12_Packed:
			case PixelType_Gvsp_RGB16_Packed:
			case PixelType_Gvsp_RGB10V1_Packed:
			case PixelType_Gvsp_RGB10V2_Packed:
			case PixelType_Gvsp_RGB12V1_Packed:
			case PixelType_Gvsp_RGB565_Packed:
			case PixelType_Gvsp_BGR565_Packed:
			case PixelType_Gvsp_YUV411_Packed:
			case PixelType_Gvsp_YUV422_Packed:
			case PixelType_Gvsp_YUV422_YUYV_Packed:
			case PixelType_Gvsp_YUV444_Packed:
			case PixelType_Gvsp_YCBCR8_CBYCR:
			case PixelType_Gvsp_YCBCR422_8:
			case PixelType_Gvsp_YCBCR422_8_CBYCRY:
			case PixelType_Gvsp_YCBCR411_8_CBYYCRYY:
			case PixelType_Gvsp_YCBCR601_8_CBYCR:
			case PixelType_Gvsp_YCBCR601_422_8:
			case PixelType_Gvsp_YCBCR601_422_8_CBYCRY:
			case PixelType_Gvsp_YCBCR601_411_8_CBYYCRYY:
			case PixelType_Gvsp_YCBCR709_8_CBYCR:
			case PixelType_Gvsp_YCBCR709_422_8:
			case PixelType_Gvsp_YCBCR709_422_8_CBYCRY:
			case PixelType_Gvsp_YCBCR709_411_8_CBYYCRYY:
				{
					MV_CC_PIXEL_CONVERT_PARAM stParam;
					memset(&stParam, 0, sizeof(MV_CC_PIXEL_CONVERT_PARAM));

					stParam.pSrcData = pData;
					stParam.nSrcDataLen = pFrameInfo->nFrameLen;
					stParam.enSrcPixelType = pFrameInfo->enPixelType;
					stParam.nWidth = pFrameInfo->nWidth;
					stParam.nHeight = pFrameInfo->nHeight;
					stParam.enDstPixelType = PixelType_Gvsp_BGR8_Packed;
					stParam.nDstBufferSize = pFrameInfo->nWidth * pFrameInfo->nHeight * 3;
					pImage = (unsigned char*)malloc(stParam.nDstBufferSize);
					stParam.pDstBuffer = pImage;

					if(MV_CC_ConvertPixelType(pInstance->GetDeviceHandler(), &stParam))
						break;

					int nDstLen = stParam.nDstLen;
					pData = pImage;

					for(int64_t i = 0; i < i64Height; ++i)
						memcpy(*(ppCurrentOffsetY + i), (BYTE*)(pData)+(i * i64Width * i64PixelSizeByte), i64WidthStep);
				}
				break;
			}

			pInstance->IncreaseGrabCount();
		}

		pInstance->ConnectImage(false);

		CEventHandlerManager::BroadcastOnAcquisition(pCurrentImage, pInstance->GetObjectID(), pInstance->GetImageView(), pInstance);
	} 
	while(false);

	if(pImage)
		free(pImage);

	if(!pInstance->IsLive())
	{
		int nRequsetGrabCount = 0;
		pInstance->GetGrabCount(&nRequsetGrabCount);
		if(nRequsetGrabCount == pInstance->GetIncreaseGrabCount())
			pInstance->Stop();
	}

}

CDeviceHIKVisionGigE::CDeviceHIKVisionGigE()
{
}

CDeviceHIKVisionGigE::~CDeviceHIKVisionGigE()
{
	Terminate();
}

/////////////////////////////////////////////////////////////////////
// Function name : Initialize
// Revision : 1
// Author : Dong-Hee Han
// Creation Date : Fab 11, 2019
// Detail : Code for safety of Initialize function		    
/////////////////////////////////////////////////////////////////////
EDeviceInitializeResult CDeviceHIKVisionGigE::Initialize()
{
	EDeviceInitializeResult eReturn = EDeviceInitializeResult_UnknownError;

	CString strMessage, strStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("HIKVision"));

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

		unsigned int nTLayerType = MV_GIGE_DEVICE | MV_USB_DEVICE;
		MV_CC_DEVICE_INFO_LIST m_stDevList = { 0 };
		int nRet = MV_CC_EnumDevices(nTLayerType, &m_stDevList);

		if(MV_OK != nRet)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtofindthedevice);
			eReturn = EDeviceInitializeResult_EnumerateDeviceError;
			break;
		}


		if(m_stDevList.nDeviceNum == 0)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Device Channel"));
			eReturn = EDeviceInitializeResult_NotFoundDeviceError;
			break;
		}

		CString strDeviceSerialnumber = GetDeviceID();
		CString strInputDeviceSerialnumber;

		int nSelDeviceNumber = -1;

		bool bWidthMax = false;
		bool bHeightMax = false;

		for(unsigned int i = 0; i < m_stDevList.nDeviceNum; ++i)
		{
			strInputDeviceSerialnumber = m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.chSerialNumber;

			if(strInputDeviceSerialnumber == strDeviceSerialnumber)
			{
				nSelDeviceNumber = i;
				break;
			}
		}

		if(nSelDeviceNumber < 0)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("SerialNumber"));
			eReturn = EDeviceInitializeResult_NotFoundSerialNumber;
			break;
		}

		int nDeviceIndex = nSelDeviceNumber;

		MV_CC_DEVICE_INFO stDevInfo = { 0 };
		memcpy(&stDevInfo, m_stDevList.pDeviceInfo[nDeviceIndex], sizeof(MV_CC_DEVICE_INFO));

		if(MV_CC_CreateHandle(&m_pHikDevice, &stDevInfo))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Create Handle"));
			eReturn = EDeviceInitializeResult_NotCreateDeviceError;
			break;
		}

		unsigned int nAccessMode = MV_ACCESS_Exclusive;
		unsigned short nSwitchoverKey = nSelDeviceNumber;

		if(MV_CC_OpenDevice(m_pHikDevice, nAccessMode, nSwitchoverKey))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Open Device"));
			eReturn = EDeviceInitializeResult_NotOpenedDeviceError;
			break;
		}

		MVCC_STRINGVALUE mvccstrDeviceName;

		if(MV_CC_GetStringValue(m_pHikDevice, "DeviceModelName", &mvccstrDeviceName))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Device Name"));
			eReturn = EDeviceInitializeResult_NotFoundDeviceInfo;
			break;
		}

		int nModelNameNumber = 0;
		EDeviceHIKVisionGigEModel eModelName;
		for(int nModelNameNumber = 0; nModelNameNumber < EDeviceHIKVisionGigEModel_Count; ++nModelNameNumber)
		{
			if(strcmp(ModelName[nModelNameNumber], mvccstrDeviceName.chCurValue) == 0)
			{
				eModelName = EDeviceHIKVisionGigEModel(nModelNameNumber);
				break;
			}
		}

		int nWidth = 1024;
		int nHeight = 1024;
		CString strFormat = _T("");
		CString strError = _T("");		

		EDeviceHIKVisionGigEInitMode eInitType = EDeviceHIKVisionGigEInitMode_DeviceOnly;

		if(GetInitializeMode(&eInitType))
			break;

		unsigned int AcqisionMode = 2;
		float GaindB = 0;

		bool bParamOK = false;

		switch(eInitType)
		{
		case EDeviceHIKVisionGigEInitMode_DeviceOnly:
			{
				UpdateDeviceToParameter();

				strError = _T("Width");
				int nParam = 0;
				if(GetCanvasWidth(&nParam))
				{
					strMessage.Format(CMultiLanguageManager::GetString  (ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Width"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				nWidth = nParam;
	
				strError = _T("Height");
				nParam = 0;
				if(GetCanvasHeight(&nParam))
				{
					strMessage.Format(CMultiLanguageManager::GetString  (ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Height"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				nHeight = nParam;

				strError = _T("PixelFormat");
				EDeviceHIKVisionGigEPixelFormat ePFParam = EDeviceHIKVisionGigEPixelFormat_Mono8;
				if(GetPixelFormat(&ePFParam))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("PixelFormat"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				strFormat = g_lpszHIKVisionGigEPixelFormat[ePFParam];

				bParamOK = true;
			}
			break;
		case EDeviceHIKVisionGigEInitMode_Parameter:
			{
				MVCC_INTVALUE MaxLength;
				if(MV_CC_GetIntValue(m_pHikDevice, "WidthMax", &MaxLength))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Width Max"));
					eReturn = EDeviceInitializeResult_WidthMaxGetFailed;
					break;
				}
				nWidth = 0;
				if(GetCanvasWidth(&nWidth))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Width"));
					eReturn = EDeviceInitializeResult_CanNotReadDBCanvasWidth;
					break;
				}

				if((unsigned int)nWidth > MaxLength.nCurValue)
				{
					nWidth = MaxLength.nCurValue;
					bWidthMax = true;
					SetCanvasWidth(nWidth);
				}

				if(MV_CC_SetWidth(m_pHikDevice, nWidth))
				{
					if(MV_CC_SetWidth(m_pHikDevice, MaxLength.nCurValue))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Width"));
						eReturn = EDeviceInitializeResult_CanNotApplyWidth;
						break;
					}
					else
					{
						nWidth = MaxLength.nCurValue;
						bWidthMax = true;
						if(SetCanvasWidth(MaxLength.nCurValue))
						{
							eReturn = EDeviceInitializeResult_CanNotApplyWidth;
							break;
						}
					}
				}

				nHeight = 0;
				if(GetCanvasHeight(&nHeight))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Height"));
					eReturn = EDeviceInitializeResult_CanNotReadDBCanvasHeight;
					break;
				}

				if(MV_CC_GetIntValue(m_pHikDevice, "HeightMax", &MaxLength))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Height Max"));
					eReturn = EDeviceInitializeResult_HeightMaxGetFailed;
					break;
				}
				if((unsigned int)nHeight > MaxLength.nCurValue)
				{
					nHeight = MaxLength.nCurValue;
					bHeightMax = true;
					if(SetCanvasHeight(nHeight))
					{
						eReturn = EDeviceInitializeResult_CanNotApplyHeight;
						break;
					}
				}

				if(MV_CC_SetHeight(m_pHikDevice, nHeight))
				{
					if(MV_CC_SetHeight(m_pHikDevice, MaxLength.nCurValue))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Height"));
						eReturn = EDeviceInitializeResult_CanNotApplyHeight;
						break;
					}
					else
					{
						if(SetCanvasHeight(MaxLength.nCurValue))
						{
							eReturn = EDeviceInitializeResult_CanNotApplyHeight;
							break;
						}
						nHeight = MaxLength.nCurValue;
						bHeightMax = true;
					}
				}

				int nDeviceStreamChannelPacketSize = 0;

				if(GetDeviceStreamChannelPackSize(&nDeviceStreamChannelPacketSize))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("PackSize"));
					eReturn = EDeviceInitializeResult_CanNotReadDBPacketSize;
					break;
				}

				if(!SetCommandInt32("DeviceStreamChannelPacketSize", nDeviceStreamChannelPacketSize))
				{
					if(GetCommandInt32("DeviceStreamChannelPacketSize", &nDeviceStreamChannelPacketSize))
					{
						if(SetDeviceStreamChannelPackSize(nDeviceStreamChannelPacketSize))
						{
							eReturn = EDeviceInitializeResult_CanNotWriteDBPacketSize;
							break;
						}
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

				if(!SetCommandInt32("OffsetX", nOffsetX))
				{
					if(GetCommandInt32("OffsetX", &nOffsetX))
					{
						if(SetOffsetX(nOffsetX))
						{
							eReturn = EDeviceInitializeResult_CanNotApplyOffsetX;
							break;
						}
					}
				}

				int nOffsetY = 0;
				if(GetOffsetY(&nOffsetY))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("OffSetY"));
					eReturn = EDeviceInitializeResult_CanNotReadDBOffsetY;
					break;
				}

				if(bHeightMax)
					nOffsetY = 0;

				if(!SetCommandInt32("OffsetY", nOffsetY))
				{
					if(GetCommandInt32("OffsetY", &nOffsetY))
					{
						if(SetOffsetY(nOffsetY))
						{
							eReturn = EDeviceInitializeResult_CanNotApplyOffsetY;
							break;
						}
					}
				}

				bool bReverseX = false;
				if(GetReverseX(&bReverseX))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ReverseX"));
					eReturn = EDeviceInitializeResult_CanNotReadDBRevereX;
					break;
				}
				if(!SetCommandBool("ReverseX", bReverseX))
				{
					if(GetCommandBool("ReverseX", &bReverseX))
					{
						if(SetReverseX(bReverseX))
						{
							eReturn = EDeviceInitializeResult_CanNotApplyRevereX;
							break;
						}
					}
				}

				if(eModelName != EDeviceHIKVisionGigEModel_MV_CA013_20GM)
				{
					bool bReverseY = false;
					if(GetReverseY(&bReverseY))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ReverseY"));
						eReturn = EDeviceInitializeResult_CanNotReadDBRevereY;
						break;
					}
					if(!SetCommandBool("ReverseY", bReverseY))
					{
						if(GetCommandBool("ReverseY", &bReverseY))
						{
							if(SetReverseY(bReverseY))
							{
								eReturn = EDeviceInitializeResult_CanNotApplyRevereY;
								break;
							}
						}
					}
				}

				if(GetAnalogGaindB(&GaindB))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("GaindB"));
					eReturn = EDeviceInitializeResult_CanNotReadDBPixelFormat;
					break;
				}

				switch(_ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_AcquisitionMode)))
				{
				case 0:
					AcqisionMode = 2;
					break;
				case 1:
					AcqisionMode = 0;
					break;
				}

				MVCC_ENUMVALUE eNumValue;
				if(MV_CC_GetEnumValue(m_pHikDevice, "GainAuto", &eNumValue))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Gain"));
					strMessage.Format(_T("Get GainAutoMode fail"));
					eReturn = EDeviceInitializeResult_CanNotReadDBGainAuto;
					break;
				}
				if(!eNumValue.nCurValue)
				{
					if(MV_CC_SetFloatValue(m_pHikDevice, "Gain", GaindB))
					{
						strMessage.Format(CMultiLanguageManager::GetString	(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Gain"));
						eReturn = EDeviceInitializeResult_CanNotApplyGain;
						break;
					}
				}
	
				EDeviceHIKVisionGigEPixelFormat ePixelFormat;
	
				if(GetImagePixelFormat(&ePixelFormat))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("PixelFormat"));
					eReturn = EDeviceInitializeResult_CanNotReadDBPixelFormat;
					break;
				}

				strFormat = g_lpszHIKVisionGigEPixelFormat[ePixelFormat];
	
				switch(ePixelFormat)
				{
				case EDeviceHIKVisionGigEPixelFormat_Mono8:
					nRet = MV_CC_SetEnumValue(m_pHikDevice, "PixelFormat",	EImagePixelFormat::eMono8);
					break;
				case EDeviceHIKVisionGigEPixelFormat_Mono10:
					nRet = MV_CC_SetEnumValue(m_pHikDevice, "PixelFormat",	EImagePixelFormat::eMono10);
					break;
				case EDeviceHIKVisionGigEPixelFormat_Mono10packed:
					nRet = MV_CC_SetEnumValue(m_pHikDevice, "PixelFormat",	EImagePixelFormat::eMono10Packed);
					break;
				case EDeviceHIKVisionGigEPixelFormat_Mono12:
					nRet = MV_CC_SetEnumValue(m_pHikDevice, "PixelFormat",	EImagePixelFormat::eMono12);
					break;
				case EDeviceHIKVisionGigEPixelFormat_Mono12packed:
					nRet = MV_CC_SetEnumValue(m_pHikDevice, "PixelFormat",	EImagePixelFormat::eMono12Packed);
					break;
				case EDeviceHIKVisionGigEPixelFormat_RGB8packed:
					nRet = MV_CC_SetEnumValue(m_pHikDevice, "PixelFormat",	EImagePixelFormat::eRGB8packed);
					break;
				case EDeviceHIKVisionGigEPixelFormat_YUV422_YUYV_Packed:
					nRet = MV_CC_SetEnumValue(m_pHikDevice, "PixelFormat",	EImagePixelFormat::eYUV422_YUYV_Packed);
					break;
				case EDeviceHIKVisionGigEPixelFormat_YUV422Packed:
					nRet = MV_CC_SetEnumValue(m_pHikDevice, "PixelFormat",	EImagePixelFormat::eYUV422Packed);
					break;
				case EDeviceHIKVisionGigEPixelFormat_BayerRG8:
					nRet = MV_CC_SetEnumValue(m_pHikDevice, "PixelFormat",	EImagePixelFormat::eBayerRG8);
					break;
				case EDeviceHIKVisionGigEPixelFormat_BayerRG10:
					nRet = MV_CC_SetEnumValue(m_pHikDevice, "PixelFormat",	EImagePixelFormat::eBayerRG10);
					break;
				case EDeviceHIKVisionGigEPixelFormat_BayerRG10Packed:
					nRet = MV_CC_SetEnumValue(m_pHikDevice, "PixelFormat",	EImagePixelFormat::eBayerRG10Packed);
					break;
				case EDeviceHIKVisionGigEPixelFormat_BayerRG12:
					nRet = MV_CC_SetEnumValue(m_pHikDevice, "PixelFormat",	EImagePixelFormat::eBayerRG12);
					break;
				case EDeviceHIKVisionGigEPixelFormat_BayerRG12Packed:
					nRet = MV_CC_SetEnumValue(m_pHikDevice, "PixelFormat",	EImagePixelFormat::eBayerRG12Packed);
					break;
				}

				if(MV_OK != nRet)
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("PixelFormat"));
					eReturn = EDeviceInitializeResult_CanNotApplyPixelFormat;
					break;
				}

				if(MV_CC_SetEnumValue(m_pHikDevice, "AcquisitionMode", AcqisionMode))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("AcquisitionMode"));
					eReturn = EDeviceInitializeResult_CanNotApplyAcquisitionMode;
					break;
				}

				int nFrameCount = 0;
				if(GetAcquisitionBurstFrameCount(&nFrameCount))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("BurstFrameCount"));
					eReturn = EDeviceInitializeResult_CanNotReadDBAcquisitionBurstFrameCount;
					break;
				}

				if(!SetCommandInt32("AcquisitionBurstFrameCount", nFrameCount))
				{
					if(GetCommandInt32("AcquisitionBurstFrameCount", &nFrameCount))
					{
						if(SetAcquisitionBurstFrameCount(nFrameCount))
						{
							eReturn = EDeviceInitializeResult_CanNotApplyAcquisitionBurstFrameCount;
							break;
						}
					}
				}


				bool bFrameRateControlEnable = false;
				if(GetAcquisitionFrameRateControlEnable(&bFrameRateControlEnable))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("FrameRateControlEnable"));
					eReturn = EDeviceInitializeResult_CanNotReadDBAcquisitionFramRateControlEnable;
					break;
				}

				if(!SetCommandBool("AcquisitionFrameRateEnable", bFrameRateControlEnable))
				{
					if(GetCommandBool("AcquisitionFrameRateEnable", &bFrameRateControlEnable))
					{
						if(SetAcquisitionFrameRateControlEnable(bFrameRateControlEnable))
						{
							eReturn = EDeviceInitializeResult_CanNotApplyAcquisitionFramRateControlEnable;
							break;
						}
					}
				}

				if(bFrameRateControlEnable)
				{
					float fFrameRate = 0;
					if(GetAcquisitionFrameRate(&fFrameRate))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("FrameRate"));
						eReturn = EDeviceInitializeResult_CanNotReadDBAcquisitionBurstFrameRate;
						break;
					}
					if(MV_CC_SetFloatValue(m_pHikDevice, "AcquisitionFrameRate", fFrameRate))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("FrameRate"));
						eReturn = EDeviceInitializeResult_CanNotApplyAcquisitionBurstFrameRate;
						break;
					}
				}


				EDeviceHIKVisionGigEAcquisitionTriggerSelector eParam;
				if(GetAcquisitionTriggerSelector(&eParam))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TriggerSelector"));
					eReturn = EDeviceInitializeResult_CanNotReadDBAcquisitionStartTriggerSelector;
					break;
				}

				int nTemp = 0;

				if(eParam == 0)
					nTemp = 6;

				if(MV_CC_SetEnumValue(m_pHikDevice, "TriggerSelector", nTemp))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TriggerSelector"));
					eReturn = EDeviceInitializeResult_CanNotApplyAcquisitionStartTriggerSelector;
					break;
				}

				EDeviceHIKVisionGigESwitch eTriggerMode;
				EDeviceHIKVisionGigEAuto eAutoMode;

				if(GetAcquisitionTriggerMode(&eTriggerMode))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TriggerMode"));
					eReturn = EDeviceInitializeResult_CanNotReadDBAcquisitionStartTriggerMode;
					break;
				}
				if(MV_CC_SetEnumValue(m_pHikDevice, "TriggerMode", eTriggerMode))
				{
					MVCC_ENUMVALUE eEnumValue;
					if(!MV_CC_GetEnumValue(m_pHikDevice, "TriggerMode", &eEnumValue))
					{
						if(SetAcquisitionTriggerMode(EDeviceHIKVisionGigESwitch(eEnumValue.nCurValue)))
						{
							eReturn = EDeviceInitializeResult_CanNotApplyAcquisitionTriggerMode;
							break;
						}
					}
				}

				EDeviceHIKVisionGigEAcquisitionTriggerSource eTriggerSource;
				if(GetAcquisitionTriggerSource(&eTriggerSource))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TriggerSource"));
					eReturn = EDeviceInitializeResult_CanNotReadDBAcquisitionTriggerSource;
					break;
				}

				switch(eTriggerSource)
				{
				case EDeviceHIKVisionGigETriggerSource_SoftWare:
					nTemp = 7;
					break;
				case EDeviceHIKVisionGigETriggerSource_Line0:
					nTemp = 0;
					break;
				case EDeviceHIKVisionGigETriggerSource_Line2:
					nTemp = 2;
					break;
				case EDeviceHIKVisionGigETriggerSource_Counter0:
					nTemp = 4;
					break;
				default:
					break;
				}

				if(MV_CC_SetEnumValue(m_pHikDevice, "TriggerSource", nTemp))
				{
					MVCC_ENUMVALUE eEnumValue;

					if(!MV_CC_GetEnumValue(m_pHikDevice, "TriggerSource", &eEnumValue))
					{
						switch(eEnumValue.nCurValue)
						{
						case 7:
							eTriggerSource = EDeviceHIKVisionGigETriggerSource_SoftWare;
							break;
						case 0:
							eTriggerSource = EDeviceHIKVisionGigETriggerSource_Line0;
							break;
						case 2:
							eTriggerSource = EDeviceHIKVisionGigETriggerSource_Line2;
							break;
						case 4:
							eTriggerSource = EDeviceHIKVisionGigETriggerSource_Counter0;
							break;
						default:
							break;
						}

						if(SetAcquisitionTriggerSource(eTriggerSource))
						{
							eReturn = EDeviceInitializeResult_CanNotApplyAcquisitionTriggerSource;
							break;
						}
					}

				}


				if(eTriggerMode == EDeviceHIKVisionGigESwitch_On)
				{
					EDeviceHIKVisionGigEAcquisitionTriggerActivation eTriggerActivation;
					if(eTriggerSource != EDeviceHIKVisionGigETriggerSource_SoftWare)
					{
						if(GetAcquisitionTriggerActivation(&eTriggerActivation))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TriggerActivation"));
							eReturn = EDeviceInitializeResult_CanNotReadDBAcquisitionTriggerActivation;
							break;
						}
						if(MV_CC_SetEnumValue(m_pHikDevice, "TriggerActivation", eTriggerActivation))
						{
							MVCC_ENUMVALUE eEnumValue;
							if(MV_CC_GetEnumValue(m_pHikDevice, "TriggerActivation", &eEnumValue))
							{
								if(SetAcquisitionTriggerActivation(EDeviceHIKVisionGigEAcquisitionTriggerActivation(eEnumValue.nCurValue)))
								{
									eReturn = EDeviceInitializeResult_CanNotApplyAcquisitionTriggerActivation;
									break;
								}
							}

						}
					}

				}
				else if(eTriggerMode == EDeviceHIKVisionGigESwitch_Off)
				{
					if(GetAcquisitionExposureAuto(&eAutoMode))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("ExposureAuto"));
						eReturn = EDeviceInitializeResult_CanNotReadDBAcquisitionExposureAuto;
						break;
					}
					if(SetAcquisitionExposureAuto(eAutoMode))
					{
						eReturn = EDeviceInitializeResult_CanNotApplyAcquisitionExposureAuto;
						break;
					}
				}

				if(eAutoMode != EDeviceHIKVisionGigEAuto_Continuous)
				{
					float fExposure = 0;
					if(GetAcquisitionExposureTime(&fExposure))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ExposureTime"));
						eReturn = EDeviceInitializeResult_CanNotReadDBAcquisitionExposureTime;
						break;
					}

					if(!SetCommandFloat("ExposureTime", fExposure))
					{
						double dblExposureTime = 0;
						if(GetCommandFloat("ExposureTime", &dblExposureTime))
						{
							if(SetAcquisitionExposureTime(dblExposureTime))
							{
								eReturn = EDeviceInitializeResult_CanNotApplyAcquisitionExposureTime;
								break;
							}
						}
					}
				}


				float fTriggerDelay = 0;
				if(GetAcquisitionTriggerDelay(&fTriggerDelay))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TriggerDelay"));
					eReturn = EDeviceInitializeResult_CanNotReadDBAcquisitionTriggerDelay;
					break;
				}

				if(!SetCommandFloat("TriggerDelay", fTriggerDelay))
				{
					double dblTriggerDelay = fTriggerDelay;
					if(GetCommandFloat("TriggerDelay", &dblTriggerDelay))
					{
						if(SetAcquisitionTriggerDelay(dblTriggerDelay))
						{
							eReturn = EDeviceInitializeResult_CanNotApplyAcquisitionTriggerDelay;
							break;
						}
					}
				}

				EDeviceHIKVisionGigEAcquisitionExposureMode eExposureMode;
				if(GetAcquisitionExposureMode(&eExposureMode))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ExposureMode"));
					eReturn = EDeviceInitializeResult_CanNotReadDBAcquisitionExposureMode;
					break;
				}
				if(MV_CC_SetEnumValue(m_pHikDevice, "ExposureMode", eExposureMode))
				{
					MVCC_ENUMVALUE eEnumValue;
					if(!MV_CC_GetEnumValue(m_pHikDevice, "ExposureMode", &eEnumValue))
					{
						if(SetAcquisitionExposureMode(EDeviceHIKVisionGigEAcquisitionExposureMode(eEnumValue.nCurValue)))
						{
							eReturn = EDeviceInitializeResult_CanNotApplyAcquisitionExposureMode;
							break;
						}
					}
				}


				bool bBlackLevelEnable = false;

				if(GetBlackLevelEnable(&bBlackLevelEnable))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("BlackLevelEnable"));
					eReturn = EDeviceInitializeResult_CanNotReadDBBlackLevelEnable;
					break;
				}
				if(!SetCommandBool("BlackLevelEnable", bBlackLevelEnable))
				{
					if(GetCommandBool("BlackLevelEnable", &bBlackLevelEnable))
					{
						if(SetBlackLevelEnable(bBlackLevelEnable))
						{
							eReturn = EDeviceInitializeResult_CanNotApplyBlackLevelEnable;
							break;
						}
					}
				}

				if(bBlackLevelEnable)
				{
					int nBlackLevel = 0;
					if(GetBlackLevel(&nBlackLevel))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("BlackLevel"));
						eReturn = EDeviceInitializeResult_CanNotReadDBBlackLevel;
						break;
					}

					if(!SetCommandInt32("BlackLevel", nBlackLevel))
					{
						if(GetCommandInt32("BlackLevel", &nBlackLevel))
						{
							if(SetBlackLevel(nBlackLevel))
							{
								eReturn = EDeviceInitializeResult_CanNotApplyBlackLevel;
								break;
							}
						}
					}
				}

				bool bDigitalShiftEnable = false;
				if(GetDigitalShiftEnable(&bDigitalShiftEnable))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("DigitalShiftEnable"));
					eReturn = EDeviceInitializeResult_CanNotReadDBDigitalShiftEnable;
					break;
				}
				if(!SetCommandBool("DigitalShiftEnable", bDigitalShiftEnable))
				{
					if(GetCommandBool("DigitalShiftEnable", &bDigitalShiftEnable))
					{
						if(SetDigitalShiftEnable(bDigitalShiftEnable))
						{
							eReturn = EDeviceInitializeResult_CanNotApplyDigitalShiftEnable;
							break;
						}
					}
				}


				if(bDigitalShiftEnable)
				{
					float fDigitalShift = 0;
					if(GetDigitalShift(&fDigitalShift))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("DigitalShift"));
						eReturn = EDeviceInitializeResult_CanNotReadDBDigitalShift;
						break;
					}

					if(!SetCommandFloat("DigitalShift", fDigitalShift))
					{
						double dblDigitalShift = fDigitalShift;
						if(GetCommandFloat("DigitalShift", &dblDigitalShift))
						{
							if(SetDigitalShift(dblDigitalShift))
							{
								eReturn = EDeviceInitializeResult_CanNotApplyDigitalShift;
								break;
							}
						}
					}
				}

				EDeviceHIKVisionGigELineSelect eLineSel;
				if(GetLineSelector(&eLineSel))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineSelector"));
					eReturn = EDeviceInitializeResult_CanNotReadDBLineSelector;
					break;
				}
				if(MV_CC_SetEnumValue(m_pHikDevice, "LineSelector", eParam))
				{
					MVCC_ENUMVALUE eEnumValue;
					if(!MV_CC_GetEnumValue(m_pHikDevice, "LineSelector", &eEnumValue))
					{
						if(SetLineSelector(EDeviceHIKVisionGigELineSelect(eEnumValue.nCurValue)))
						{
							eReturn = EDeviceInitializeResult_CanNotApplyLineSelector;
							break;
						}
					}

				}

				switch(eLineSel)
				{
				case EDeviceHIKVisionGigELineSelect_eLine0:
				{
					int nDebouncerTime = 0;
					if(GetLine0DebouncerTime(&nDebouncerTime))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line0 DebouncerTime"));
						eReturn = EDeviceInitializeResult_CanNotReadDBLineDebouncerTime;
						break;
					}

					if(!SetCommandInt32("LineDebouncerTime", nDebouncerTime))
					{
						if(GetCommandInt32("LineDebouncerTime", &nDebouncerTime))
						{
							if(SetLine0DebouncerTime(nDebouncerTime))
							{
								eReturn = EDeviceInitializeResult_CanNotApplyLineDebouncerTime;
								break;
							}
						}
					}
				}
				break;
				case EDeviceHIKVisionGigELineSelect_eLine1:
				{
					bool bLineInverter = false;
					if(GetLine1Inverter(&bLineInverter))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line1 Inverter"));
						eReturn = EDeviceInitializeResult_CanNotReadDBLineInverterStatus;
						break;
					}

					if(!SetCommandBool("LineInverter", bLineInverter))
					{
						if(SetCommandBool("LineInverter", &bLineInverter))
						{
							if(SetLine1Inverter(bLineInverter))
							{
								eReturn = EDeviceInitializeResult_CanNotApplyLineInverterStatus;
								break;
							}
						}
					}

					EDeviceHIKVisionGigELineSource eLineSource;
					if(GetLine1Source(&eLineSource))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line1 Source"));
						eReturn = EDeviceInitializeResult_CanNotReadDBLineSource;
						break;
					}
					if(SetLine1Source(eLineSource))
					{
						eReturn = EDeviceInitializeResult_CanNotApplyLineSource;
						break;
					}

					bool bStrobeEnable = false;
					if(GetLine1StrobEnable(&bStrobeEnable))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line1 StrobEnable"));
						eReturn = EDeviceInitializeResult_CanNotReadDBLineStrobEnable;
						break;
					}

					if(!SetCommandBool("StrobeEnable", bStrobeEnable))
					{
						if(SetCommandBool("StrobeEnable", &bStrobeEnable))
						{
							if(SetLine1StrobEnable(bStrobeEnable))
							{
								eReturn = EDeviceInitializeResult_CanNotApplyLineStrobEnable;
								break;
							}
						}
					}

					int StrobeLine1Duration = 0;
					if(GetLine1StrobeLineDuration(&StrobeLine1Duration))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line1 Duration"));
						eReturn = EDeviceInitializeResult_CanNotReadDBLineStrobeLineDuration;
						break;
					}

					if(!SetCommandInt32("StrobeLineDuration", StrobeLine1Duration))
					{
						if(GetCommandInt32("StrobeLineDuration", &StrobeLine1Duration))
						{
							if(SetLine1StrobeLineDuration(StrobeLine1Duration))
							{
								eReturn = EDeviceInitializeResult_CanNotApplyLineStrobeLineDuration;
								break;
							}
						}
					}

					int StrobeLine1Delay = 0;
					if(GetLine1StrobeLineDelay(&StrobeLine1Delay))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line1 Delay"));
						eReturn = EDeviceInitializeResult_CanNotReadDBLineStrobeLineDelay;
						break;
					}

					if(!SetCommandInt32("StrobeLineDelay", StrobeLine1Delay))
					{
						if(GetCommandInt32("StrobeLineDelay", &StrobeLine1Delay))
						{
							if(SetLine1StrobeLineDelay(StrobeLine1Delay))
							{
								eReturn = EDeviceInitializeResult_CanNotApplyLineStrobeLineDelay;
								break;
							}
						}
					}



					int StrobeLine1PreDelay = 0;
					if(GetLine1StrobeLinePreDelay(&StrobeLine1PreDelay))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line1 Duration"));
						eReturn = EDeviceInitializeResult_CanNotReadDBLineStrobeLinePreDelay;
						break;
					}

					if(!SetCommandInt32("StrobeLinePreDelay", StrobeLine1PreDelay))
					{
						if(GetCommandInt32("StrobeLinePreDelay", &StrobeLine1PreDelay))
						{
							if(SetLine1StrobeLinePreDelay(StrobeLine1PreDelay))
							{
								eReturn = EDeviceInitializeResult_CanNotApplyLineStrobeLinePreDelay;
								break;
							}
						}
					}

					if(eLineSource == EDeviceHIKVisionGigELineSource_HardTriggerActive)
					{
						EDeviceHIKVisionGigETriggerActivation eTriggerActivation;
						if(GetLine1HardWareTriggerActivation(&eTriggerActivation))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line1 H/W Trigger Activation"));
							eReturn = EDeviceInitializeResult_CanNotReadDBHardWareTriggerActivaion;
							break;
						}
						if(SetLine1HardWareTriggerActivation(eTriggerActivation))
						{
							eReturn = EDeviceInitializeResult_CanNotApplyHardWareTriggerActivaion;
							break;
						}
					}

				}
				break;
				case EDeviceHIKVisionGigELineSelect_eLine2:
				{
					EDeviceHIKVisionGigELineMode eLineMode;
					if(GetLine2Mode(&eLineMode))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line 2Mode"));
						eReturn = EDeviceInitializeResult_CanNotReadDBLineMode;
						break;
					}
					if(SetLine2Mode(eLineMode))
					{
						eReturn = EDeviceInitializeResult_CanNotApplyLineMode;
						break;
					}

					switch(eLineMode)
					{
					case EDeviceHIKVisionGigELineMode_Input:
					{
						int nDebouncerTime = 0;
						if(GetLine2DebouncerTime(&nDebouncerTime))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line2 DebouncerTime"));
							eReturn = EDeviceInitializeResult_CanNotReadDBLineDebouncerTime;
							break;
						}

						if(!SetCommandInt32("LineDebouncerTime", nDebouncerTime))
						{
							if(GetCommandInt32("LineDebouncerTime", &nDebouncerTime))
							{
								if(SetLine2DebouncerTime(nDebouncerTime))
								{
									eReturn = EDeviceInitializeResult_CanNotApplyLineDebouncerTime;
									break;
								}
							}
						}
					}
					break;
					case EDeviceHIKVisionGigELineMode_Strob:
						bool bLineInverter = false;
						if(GetLine2Inverter(&bLineInverter))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line2 InverterStatus"));
							eReturn = EDeviceInitializeResult_CanNotReadDBLineInverterStatus;
							break;
						}

						if(!SetCommandBool("LineInverter", bLineInverter))
						{
							if(SetCommandBool("LineInverter", &bLineInverter))
							{
								if(SetLine2Inverter(bLineInverter))
								{
									eReturn = EDeviceInitializeResult_CanNotApplyLineInverterStatus;
									break;
								}
							}
						}

						EDeviceHIKVisionGigELineSource eLineSource;
						if(GetLine2Source(&eLineSource))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line2 Source"));
							eReturn = EDeviceInitializeResult_CanNotReadDBLineSource;
							break;
						}
						if(SetLine2Source(eLineSource))
						{
							eReturn = EDeviceInitializeResult_CanNotApplyLineSource;
							break;
						}

						bool bStrobeEnable = false;
						if(GetLine2StrobEnable(&bStrobeEnable))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line2 StrobEnable"));
							eReturn = EDeviceInitializeResult_CanNotReadDBLineStrobEnable;
							break;
						}

						if(!SetCommandBool("StrobeEnable", bStrobeEnable))
						{
							if(SetCommandBool("StrobeEnable", &bStrobeEnable))
							{
								if(SetLine2StrobEnable(bStrobeEnable))
								{
									eReturn = EDeviceInitializeResult_CanNotApplyLineStrobEnable;
									break;
								}
							}
						}



						int StrobeLine1Duration = 0;
						if(GetLine2StrobeLineDuration(&StrobeLine1Duration))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line2 Duration"));
							eReturn = EDeviceInitializeResult_CanNotReadDBLineStrobeLineDuration;
							break;
						}


						if(!SetCommandInt32("StrobeLineDuration", StrobeLine1Duration))
						{
							if(GetCommandInt32("StrobeLineDuration", &StrobeLine1Duration))
							{
								if(SetLine2StrobeLineDuration(StrobeLine1Duration))
								{
									eReturn = EDeviceInitializeResult_CanNotApplyLineStrobeLineDuration;
									break;
								}
							}
						}

						int StrobeLine1Delay = 0;
						if(GetLine2StrobeLineDelay(&StrobeLine1Delay))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line2 Delay"));
							eReturn = EDeviceInitializeResult_CanNotReadDBLineStrobeLineDelay;
							break;
						}

						if(!SetCommandInt32("StrobeLineDelay", StrobeLine1Delay))
						{
							if(GetCommandInt32("StrobeLineDelay", &StrobeLine1Delay))
							{
								if(SetLine2StrobeLineDelay(StrobeLine1Delay))
								{
									eReturn = EDeviceInitializeResult_CanNotApplyLineStrobeLineDelay;
									break;
								}
							}
						}

						int StrobeLine1PreDelay = 0;
						if(GetLine2StrobeLinePreDelay(&StrobeLine1PreDelay))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line2 PreDelay"));
							eReturn = EDeviceInitializeResult_CanNotReadDBLineStrobeLinePreDelay;
							break;
						}

						if(!SetCommandInt32("StrobeLinePreDelay", StrobeLine1PreDelay))
						{
							if(GetCommandInt32("StrobeLinePreDelay", &StrobeLine1PreDelay))
							{
								if(SetLine2StrobeLinePreDelay(StrobeLine1PreDelay))
								{
									eReturn = EDeviceInitializeResult_CanNotApplyLineStrobeLinePreDelay;
									break;
								}
							}
						}


						if(eLineSource == EDeviceHIKVisionGigELineSource_HardTriggerActive)
						{
							EDeviceHIKVisionGigETriggerActivation eTriggerActivation;
							if(GetLine2HardWareTriggerActivation(&eTriggerActivation))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line2 H/W Trigger Activation"));
								eReturn = EDeviceInitializeResult_CanNotReadDBHardWareTriggerActivaion;
								break;
							}
							if(SetLine2HardWareTriggerActivation(eTriggerActivation))
							{
								eReturn = EDeviceInitializeResult_CanNotApplyHardWareTriggerActivaion;
								break;
							}
						}
						break;
					}
				}
					break;
					}

				bParamOK = true;
			}
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
				nBpp = 8;
				nAlignByte = 4;
				nWidthStep = nWidth;
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

		nRet = MV_CC_RegisterImageCallBack(m_pHikDevice, CallbackFunction, this);

		if(nRet != MV_OK)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("CallbackFunction"));
			eReturn = EDeviceInitializeResult_CanNotApplyImageCallBack;
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

	return eReturn;
}

EDeviceTerminateResult CDeviceHIKVisionGigE::Terminate()
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
		if(IsLive() || !IsGrabAvailable())
		{
			if(Stop())
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Stop"));
				eReturn = EDeviceTerminateResult_FailedToStopError;
				break;
			}
		}

		m_bIsInitialized = false;

		if(m_pHikDevice)
		{
			MV_CC_CloseDevice(m_pHikDevice);
			MV_CC_DestroyHandle(m_pHikDevice);
		}

		m_pHikDevice = nullptr;
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

bool CDeviceHIKVisionGigE::LoadSettings()
{
	bool bReturn = false;

	m_vctParameterFieldConfigurations.clear();

	do
	{

		CString strTime;
		strTime.Format(_T("%d"), INT_MAX);


		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_DeviceID, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_DeviceID], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 0);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_InitializeMode, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_InitializeMode], _T("1"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszHIKVisionGigEInitMode, EDeviceHIKVisionGigEInitMode_Count), nullptr, 0);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_DeviceControl, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_DeviceControl], _T("0"), EParameterFieldType_None, nullptr, nullptr, 0);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_DeviceStreamChannelPackSize, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_DeviceStreamChannelPackSize], _T("1500"), EParameterFieldType_Edit, nullptr, nullptr, 0);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_DeviceGrabCount, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_DeviceGrabCount], _T("1"), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_ImageFormatControl, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_ImageFormatControl], _T("0"), EParameterFieldType_None, nullptr, nullptr, 0);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_Width, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_Width], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_Height, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_Height], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_OffsetX, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_OffsetX], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_OffsetY, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_OffsetY], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_ReverseX, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_ReverseX], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_ReverseY, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_ReverseY], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_PixelFormat, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_PixelFormat], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszHIKVisionGigEPixelFormat, EDeviceHIKVisionGigEPixelFormat_Count), nullptr, 1);

		//Acquisition Control
		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_AcquisitionControl, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_AcquisitionControl], _T("0"), EParameterFieldType_None, nullptr, nullptr, 0);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_AcquisitionMode, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_AcquisitionMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszHIKVisionGigAcquisitionMode, EDeviceHIKVisionAcquisitionMode_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_AcquisitionBurstFrameCount, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_AcquisitionBurstFrameCount], _T("1"), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_AcquisitionFrameRate, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_AcquisitionFrameRate], _T("15"), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_AcquisitionFrameRateControlEnable, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_AcquisitionFrameRateControlEnable], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_AcquisitionTriggerSelector, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_AcquisitionTriggerSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszHIKVisionGigEAcquisitionTriggerSelector, EDeviceHIKVisionGigEAcquisitionTriggerSelector_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_AcquisitionTriggerMode, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_AcquisitionTriggerMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszHIKVisionGigESwitch, EDeviceHIKVisionGigESwitch_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_AcquisitionTriggerSource, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_AcquisitionTriggerSource], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszHIKVisionGigETriggerSource, EDeviceHIKVisionGigETriggerSource_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_AcquisitionTriggerActivation, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_AcquisitionTriggerActivation], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszHIKVisionGigTriggerAcivation, EDeviceHIKVisionGigETriggerActivation_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_AcquisitionTriggerDelay, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_AcquisitionTriggerDelay], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_AcquisitionExposureMode, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_AcquisitionExposureMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszHIKVisionGigEExposureMode, EDeviceHIKVisionGigEAcquisitionExposureMode_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_AcquisitionExposureTime, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_AcquisitionExposureTime], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_AcquisitionExposureAuto, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_AcquisitionExposureAuto], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszHIKVisionGigEAuto, EDeviceHIKVisionGigEGainAuto_Count), nullptr, 1);

		//Analog Control
		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_AnalogControl, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_AnalogControl], _T("0"), EParameterFieldType_None, nullptr, nullptr, 0);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_AnalogGaindB, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_AnalogGaindB], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_AnalogGainAuto, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_AnalogGainAuto], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszHIKVisionGigEAuto, EDeviceHIKVisionGigEAuto_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_AnalogDigitalShift, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_AnalogDigitalShift], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_AnalogDigitalShiftEnable, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_AnalogDigitalShiftEnable], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_AnalogBlackLevel, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_AnalogBlackLevel], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_AnalogBlackLevelEnable, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_AnalogBlackLevelEnable], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_DigitalIOControl, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_DigitalIOControl], _T("0"), EParameterFieldType_None, nullptr, nullptr, 0);

		//여기서 Line1~3개중 1개 고른다. 
		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_DigitalLineSelector, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_DigitalLineSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszHIKVisionGigELineSelect, EDeviceHIKVisionGigELineSelect_Count), nullptr, 0);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_DigitalLine0, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_DigitalLine0], _T("0"), EParameterFieldType_None, nullptr, nullptr, 0);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_DigitalLine0Mode, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_DigitalLine0Mode], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_DigitalLine0DebouncerTime, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_DigitalLine0DebouncerTime], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_DigitalLine1, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_DigitalLine1], _T("0"), EParameterFieldType_None);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_DigitalLine1Mode, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_DigitalLine1Mode], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_DigitalLine1Inverter, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_DigitalLine1Inverter], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_DigitalLine1Source, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_DigitalLine1Source], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszHIKVisionGigELineSource, EDeviceHIKVisionGigELineSource_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_DigitalLine1StrobeEnable, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_DigitalLine1StrobeEnable], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_DigitalLine1StrobeLineDuration, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_DigitalLine1StrobeLineDuration], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_DigitalLine1StrobeLineDelay, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_DigitalLine1StrobeLineDelay], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_DigitalLine1StrobeLinePreDelay, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_DigitalLine1StrobeLinePreDelay], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_DigitalLine1StrobeHardTriggerActiveMode, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_DigitalLine1StrobeHardTriggerActiveMode], _T("0"), EParameterFieldType_None, nullptr, nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_DigitalLine1StrobeHardTriggerSource, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_DigitalLine1StrobeHardTriggerSource], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszHIKVisionGigEHardTriggerLineSelect, EDeviceHIKVisionGigEHardTriggerLineSelect_Count), nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_DigitalLine1StrobeHardTriggerActivaion, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_DigitalLine1StrobeHardTriggerActivaion], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszHIKVisionGigTriggerAcivation, EDeviceHIKVisionGigETriggerActivation_Count), nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_DigitalLine2, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_DigitalLine2], _T("0"), EParameterFieldType_None, nullptr, nullptr, -1);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_DigitalLine2Mode, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_DigitalLine2Mode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszHIKVisionGigELineMode, EDeviceHIKVisionGigELineMode_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_DigitalLine2Input, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_DigitalLine2Input], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_DigitalLine2DebouncerTime, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_DigitalLine2DebouncerTime], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_DigitalLine2Strob, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_DigitalLine2Strob], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_DigitalLine2Inverter, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_DigitalLine2Inverter], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_DigitalLine2Source, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_DigitalLine2Source], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszHIKVisionGigELineSource, EDeviceHIKVisionGigELineSource_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_DigitalLine2StrobeEnable, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_DigitalLine2StrobeEnable], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_DigitalLine2StrobeLineDuration, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_DigitalLine2StrobeLineDuration], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_DigitalLine2StrobeLineDelay, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_DigitalLine2StrobeLineDelay], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_DigitalLine2StrobeLinePreDelay, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_DigitalLine2StrobeLinePreDelay], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_DigitalLine2StrobeHardTriggerActiveMode, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_DigitalLine2StrobeHardTriggerActiveMode], _T("0"), EParameterFieldType_None, nullptr, nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_DigitalLine2StrobeHardTriggerSource, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_DigitalLine2StrobeHardTriggerSource], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszHIKVisionGigEHardTriggerLineSelect, EDeviceHIKVisionGigEHardTriggerLineSelect_Count), nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterHIKVisionGigE_DigitalLine2StrobeHardTriggerActivaion, g_lpszParamHIKVisionGigE[EDeviceParameterHIKVisionGigE_DigitalLine2StrobeHardTriggerActivaion], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszHIKVisionGigTriggerAcivation, EDeviceHIKVisionGigETriggerActivation_Count), nullptr, 2);

		bReturn = true;
	}
	while(false);

	return bReturn & __super::LoadSettings();
}

EDeviceGrabResult CDeviceHIKVisionGigE::Grab()
{
	EDeviceGrabResult eReturn = EDeviceGrabResult_UnknownError;

	CString strMessage;

	do
	{
		m_nGrabCount = 0;


		if(GetAcquisitionBurstFrameCount(&m_nRequestGrabCount))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Grab Count"));
			eReturn = EDeviceGrabResult_ReadOnDeviceError;
			break;
		}
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

		if(m_nRequestGrabCount <= 0)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Grab Count"));
			eReturn = EDeviceGrabResult_ReadOnDeviceError;
			break;
		}

		m_bIsGrabAvailable = false;

		if(MV_CC_StartGrabbing(m_pHikDevice))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("StartGrab"));
			break;
		}

		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("Grab"));

		eReturn = EDeviceGrabResult_OK;
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceLiveResult CDeviceHIKVisionGigE::Live()
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

		if(MV_CC_StartGrabbing(m_pHikDevice))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("StartGrab"));
			break;
		}

		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("Live"));

		eReturn = EDeviceLiveResult_OK;
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceStopResult CDeviceHIKVisionGigE::Stop()
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

		if(m_pHikDevice)
			MV_CC_StopGrabbing(m_pHikDevice);

		m_bIsGrabAvailable = true;

		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("Stop"));

		eReturn = EDeviceStopResult_OK;
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

void CDeviceHIKVisionGigE::IncreaseGrabCount()
{
	m_nGrabCount++;
}

int CDeviceHIKVisionGigE::GetIncreaseGrabCount()
{
	return m_nGrabCount;
}

void* CDeviceHIKVisionGigE::GetDeviceHandler()
{
	return m_pHikDevice;
}

EDeviceTriggerResult CDeviceHIKVisionGigE::Trigger()
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

		if(MV_CC_SetCommandValue(m_pHikDevice, "TriggerSoftware"))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Trigger"));
			eReturn = EDeviceTriggerResult_UnknownError;
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

EHIKVisionGetFunction CDeviceHIKVisionGigE::GetBlackLevel(int* pParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_AnalogBlackLevel));

		eReturn = EHIKVisionGetFunction_OK;
	}
	while(false);

	return eReturn;
}
EHIKVisionSetFunction CDeviceHIKVisionGigE::SetBlackLevel(int nParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_AnalogBlackLevel;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(!_ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_AnalogBlackLevelEnable)))
			break;

		if(m_pHikDevice)
		{
			if(!SetCommandInt32("BlackLevel", nParam))
			{
				eReturn = EHIKVisionSetFunction_WriteToDeviceError;
				break;
			}
		}
		CString strOffset;
		strOffset.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strOffset))
		{
			eReturn = EHIKVisionSetFunction_WriteToDeviceError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDeviceError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EHIKVisionGetFunction CDeviceHIKVisionGigE::GetBlackLevelEnable(bool* bParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!bParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}

		*bParam = _ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_AnalogBlackLevelEnable));

		eReturn = EHIKVisionGetFunction_OK;
	}
	while(false);

	return eReturn;
}
EHIKVisionSetFunction CDeviceHIKVisionGigE::SetBlackLevelEnable(bool bParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_AnalogBlackLevelEnable;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(m_pHikDevice)
		{
			if(!SetCommandBool("BlackLevelEnable", bParam))
			{
				eReturn = EHIKVisionSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strOffset;
		strOffset.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strOffset))
		{
			eReturn = EHIKVisionSetFunction_WriteToDeviceError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDeviceError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], g_lpszHIKVisionGigESwitch[nPreValue], g_lpszHIKVisionGigESwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EHIKVisionGetFunction CDeviceHIKVisionGigE::GetDigitalShift(float* pParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_AnalogDigitalShift));

		eReturn = EHIKVisionGetFunction_OK;
	}
	while(false);

	return eReturn;
}
EHIKVisionSetFunction CDeviceHIKVisionGigE::SetDigitalShift(float fParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_AnalogDigitalShift;

	float fPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(!_ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_AnalogDigitalShiftEnable)))
			break;

		if(fParam >= 6)
			fParam = 6.0;

		if(m_pHikDevice)
		{
			if(!SetCommandFloat("DigitalShift", fParam))
			{
				eReturn = EHIKVisionSetFunction_WriteToDeviceError;
				break;
			}
		}
		CString strOffset;
		strOffset.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strOffset))
		{
			eReturn = EHIKVisionSetFunction_WriteToDeviceError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDeviceError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EHIKVisionGetFunction CDeviceHIKVisionGigE::GetDigitalShiftEnable(bool* bParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!bParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}

		*bParam = _ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_AnalogDigitalShiftEnable));

		eReturn = EHIKVisionGetFunction_OK;
	}
	while(false);

	return eReturn;
}
EHIKVisionSetFunction CDeviceHIKVisionGigE::SetDigitalShiftEnable(bool bParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_AnalogDigitalShiftEnable;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(m_pHikDevice)
		{
			if(!SetCommandBool("DigitalShiftEnable", bParam))
			{
				eReturn = EHIKVisionSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strOffset;
		strOffset.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strOffset))
		{
			eReturn = EHIKVisionSetFunction_WriteToDeviceError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDeviceError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], g_lpszHIKVisionGigESwitch[nPreValue], g_lpszHIKVisionGigESwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EHIKVisionGetFunction CDeviceHIKVisionGigE::GetAnalogGaindB(float* pParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_AnalogGaindB));

		eReturn = EHIKVisionGetFunction_OK;
	}
	while(false);

	return eReturn;
}
EHIKVisionSetFunction CDeviceHIKVisionGigE::SetAnalogGaindB(float fParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_AnalogGaindB;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(m_pHikDevice)
		{
			if(!SetCommandFloat("Gain", fParam))
			{
				eReturn = EHIKVisionSetFunction_WriteToDeviceError;
				break;
			}
		}
		CString strOffset;
		strOffset.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strOffset))
		{
			eReturn = EHIKVisionSetFunction_WriteToDeviceError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDeviceError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EHIKVisionGetFunction CDeviceHIKVisionGigE::GetAnalogGainAuto(EDeviceHIKVisionGigEGainAuto* pParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}

		*pParam = EDeviceHIKVisionGigEGainAuto(_ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_AnalogGainAuto)));
		eReturn = EHIKVisionGetFunction_OK;
	}
	while(false);

	return eReturn;
}
EHIKVisionSetFunction CDeviceHIKVisionGigE::SetAnalogGainAuto(EDeviceHIKVisionGigEGainAuto eParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_AnalogGainAuto;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < EDeviceHIKVisionGigEGainAuto_Off || eParam >= EDeviceHIKVisionGigEGainAuto_Count)
		{
			eReturn = EHIKVisionSetFunction_NotSupportError;
			break;
		}

		CString strValue;

		if(m_pHikDevice)
		{
			if(MV_CC_SetEnumValue(m_pHikDevice, "GainAuto", eParam))
			{
				eReturn = EHIKVisionSetFunction_WriteToDeviceError;
				break;
			}

			MVCC_FLOATVALUE MvccTemp;

			if(!MV_CC_GetFloatValue(m_pHikDevice, "Gain", &MvccTemp))
			{

				CString strData;
				strData.Format(_T("%f"), MvccTemp.fCurValue);
				if(!SetParamValue(EDeviceParameterHIKVisionGigE_AnalogGaindB, strData))
				{
					eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
					break;
				}

				if(!SaveSettings(EDeviceParameterHIKVisionGigE_AnalogGaindB))
				{
					eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
					break;
				}
			}
			/*SetAnalogGaindB(fTemp);

			if(nRnt != MV_OK)
				break;*/
		}

		/*if(eParam <= 1)
			eParam = EDeviceHIKVisionGigEGainAuto::EDeviceHIKVisionGigEGainAuto_Off;*/

		if(eParam == EDeviceHIKVisionGigEGainAuto::EDeviceHIKVisionGigEGainAuto_Once)
			eParam = EDeviceHIKVisionGigEGainAuto::EDeviceHIKVisionGigEGainAuto_Off;

		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], g_lpszHIKVisionGigEAuto[nPreValue], g_lpszHIKVisionGigEAuto[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EHIKVisionGetFunction CDeviceHIKVisionGigE::GetAcquisitionMode(EDeviceHIKVisionAcquisitionMode* pParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}

		switch(_ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_AcquisitionMode)))
		{
		case 0:
			{
				*pParam = EDeviceHIKVisionAcquisitionMode::EDeviceHIKVisionAcquisitionMode_eContinuous;
				eReturn = EHIKVisionGetFunction_OK;
			}
			break;
		case 1:
			{
				*pParam = EDeviceHIKVisionAcquisitionMode::EDeviceHIKVisionAcquisitionMode_eSingleFrame;
				eReturn = EHIKVisionGetFunction_OK;
			}
			break;
		default:
			eReturn = EHIKVisionGetFunction_UnknownError;
			break;
		}

	}
	while(false);

	return eReturn;
}
EHIKVisionSetFunction CDeviceHIKVisionGigE::SetAcquisitionMode(EDeviceHIKVisionAcquisitionMode eParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_AcquisitionMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < EDeviceHIKVisionAcquisitionMode_eContinuous || eParam >= EDeviceHIKVisionAcquisitionMode_Count)
		{
			eReturn = EHIKVisionSetFunction_NotSupportError;
			break;
		}


		if(IsInitialized())
		{
			unsigned int unTemp = 0;

			if(eParam == EDeviceHIKVisionAcquisitionMode_eContinuous)
				unTemp = 2;
			else if(eParam == EDeviceHIKVisionAcquisitionMode_eSingleFrame)
				unTemp = 0;

			if(m_pHikDevice)
			{
				if(!MV_CC_SetEnumValue(m_pHikDevice, "AcquisitionMode", unTemp))
				{
					eReturn = EHIKVisionSetFunction_WriteToDeviceError;
					break;
				}
			}
		}
		

		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], g_lpszHIKVisionGigAcquisitionMode[nPreValue], g_lpszHIKVisionGigAcquisitionMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EHIKVisionGetFunction CDeviceHIKVisionGigE::GetImagePixelFormat(EDeviceHIKVisionGigEPixelFormat* pParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceHIKVisionGigEPixelFormat)_ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_PixelFormat));

		eReturn = EHIKVisionGetFunction_OK;
	}
	while(false);

	return eReturn;
}
EHIKVisionSetFunction CDeviceHIKVisionGigE::SetImagePixelFormat(EDeviceHIKVisionGigEPixelFormat eParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_PixelFormat;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < EDeviceHIKVisionGigEPixelFormat_Mono8 || eParam >= EDeviceHIKVisionGigEPixelFormat_Count)
		{
			eReturn = EHIKVisionSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EHIKVisionSetFunction_AlreadyInitializedError;
			break;
		}

		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], g_lpszHIKVisionGigEPixelFormat[nPreValue], g_lpszHIKVisionGigEPixelFormat[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceHIKVisionGigEGetFunction CDeviceHIKVisionGigE::GetInitializeMode(EDeviceHIKVisionGigEInitMode * pParam)
{
	EDeviceHIKVisionGigEGetFunction eReturn = EDeviceHIKVisionGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceHIKVisionGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceHIKVisionGigEInitMode)_ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_InitializeMode));

		eReturn = EDeviceHIKVisionGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceHIKVisionGigEGetFunction CDeviceHIKVisionGigE::SetInitializeMode(EDeviceHIKVisionGigEInitMode eParam)
{
	EDeviceHIKVisionGigEGetFunction eReturn = EDeviceHIKVisionGigEGetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_InitializeMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			break;
		}

		if(eParam < EDeviceHIKVisionGigEInitMode_DeviceOnly || eParam >= EDeviceHIKVisionGigEInitMode_Count)
		{
			eReturn = EDeviceHIKVisionGigEGetFunction_EnumTypeError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceHIKVisionGigEGetFunction_EnumTypeError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceHIKVisionGigEGetFunction_EnumTypeError;
			break;
		}

		eReturn = EDeviceHIKVisionGigEGetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], g_lpszHIKVisionGigEInitMode[nPreValue], g_lpszHIKVisionGigEInitMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EHIKVisionGetFunction CDeviceHIKVisionGigE::GetCanvasWidth(int* pParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_Width));

		eReturn = EHIKVisionGetFunction_OK;
	}
	while(false);

	return eReturn;
}
EHIKVisionSetFunction CDeviceHIKVisionGigE::SetCanvasWidth(int nParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_Width;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EHIKVisionSetFunction_AlreadyInitializedError;
			break;
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EHIKVisionGetFunction CDeviceHIKVisionGigE::GetCanvasHeight(int* pParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_Height));

		eReturn = EHIKVisionGetFunction_OK;
	}
	while(false);

	return eReturn;
}
EHIKVisionSetFunction CDeviceHIKVisionGigE::SetCanvasHeight(int nParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_Height;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EHIKVisionSetFunction_AlreadyInitializedError;
			break;
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EHIKVisionGetFunction CDeviceHIKVisionGigE::GetOffsetX(int* pParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_OffsetX));

		eReturn = EHIKVisionGetFunction_OK;
	}
	while(false);

	return eReturn;
}
EHIKVisionSetFunction CDeviceHIKVisionGigE::SetOffsetX(int nParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_OffsetX;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(m_pHikDevice)
		{
			if(!SetCommandInt32("OffsetX", nParam))
			{
				eReturn = EHIKVisionSetFunction_WriteToDeviceError;
				break;
			}
		}
		CString strOffset;
		strOffset.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strOffset))
		{
			eReturn = EHIKVisionSetFunction_WriteToDeviceError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDeviceError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EHIKVisionGetFunction CDeviceHIKVisionGigE::GetOffsetY(int* pParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_OffsetY));

		eReturn = EHIKVisionGetFunction_OK;
	}
	while(false);

	return eReturn;
}
EHIKVisionSetFunction CDeviceHIKVisionGigE::SetOffsetY(int nParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_OffsetY;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
			//if(!PylonDeviceFeatureIsWritable(m_hDevice, "OffsetY"))
			//{
			//	eReturn = EHIKVisionSetFunction_NotSupportError; // NotSupportedError
			//	break;
			//}
		if(m_pHikDevice)
		{
			if(!SetCommandInt32("OffsetY", nParam))
			{
				eReturn = EHIKVisionSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strOffset;
		strOffset.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strOffset))
		{
			eReturn = EHIKVisionSetFunction_WriteToDeviceError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDeviceError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EHIKVisionSetFunction CDeviceHIKVisionGigE::SetReverseX(bool bParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_ReverseX;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
			//if(!PylonDeviceFeatureIsWritable(m_hDevice, "OffsetY"))
			//{
			//	eReturn = EHIKVisionSetFunction_NotSupportError; // NotSupportedError
			//	break;
			//}

		if(m_pHikDevice)
		{
			if(!SetCommandBool("ReverseX", bParam))
			{
				eReturn = EHIKVisionSetFunction_WriteToDeviceError;
				break;
			}
		}


		CString strOffset;
		strOffset.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strOffset))
		{
			eReturn = EHIKVisionSetFunction_WriteToDeviceError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDeviceError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], g_lpszHIKVisionGigESwitch[nPreValue], g_lpszHIKVisionGigESwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}
EHIKVisionGetFunction CDeviceHIKVisionGigE::GetReverseX(bool* bParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!bParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}

		*bParam = _ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_ReverseX));

		eReturn = EHIKVisionGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EHIKVisionSetFunction CDeviceHIKVisionGigE::SetReverseY(bool bParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_ReverseY;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
			//if(!PylonDeviceFeatureIsWritable(m_hDevice, "OffsetY"))
			//{
			//	eReturn = EHIKVisionSetFunction_NotSupportError; // NotSupportedError
			//	break;
			//}
		if(m_pHikDevice)
		{
			if(!SetCommandBool("ReverseY", bParam))
			{
				eReturn = EHIKVisionSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strOffset;
		strOffset.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strOffset))
		{
			eReturn = EHIKVisionSetFunction_WriteToDeviceError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDeviceError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], g_lpszHIKVisionGigESwitch[nPreValue], g_lpszHIKVisionGigESwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}
EHIKVisionGetFunction CDeviceHIKVisionGigE::GetReverseY(bool* bParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!bParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}

		*bParam = _ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_ReverseY));

		eReturn = EHIKVisionGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EHIKVisionGetFunction CDeviceHIKVisionGigE::GetDeviceStreamChannelPackSize(int* pParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_DeviceStreamChannelPackSize));

		eReturn = EHIKVisionGetFunction_OK;
	}
	while(false);

	return eReturn;
}
EHIKVisionSetFunction CDeviceHIKVisionGigE::SetDeviceStreamChannelPackSize(int nParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_DeviceStreamChannelPackSize;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	if(nParam > 9156)
		nParam = 9156;
	else if(nParam < 220)
		nParam = 220;

	do
	{
		if(IsInitialized())
			break;

		if(!IsInitialized() && m_pHikDevice)
		{
			if(!SetCommandInt32("DeviceStreamChannelPacketSize", nParam))
			{
				eReturn = EHIKVisionSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EHIKVisionSetFunction CDeviceHIKVisionGigE::SetLineSelector(EDeviceHIKVisionGigELineSelect eParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_DigitalLineSelector;
	int nPreValue = _ttoi(GetParamValue(eSaveID));
	do
	{
		if(eParam < EDeviceHIKVisionGigELineSelect_eLine0 || eParam >= EDeviceHIKVisionGigELineSelect_Count)
		{
			eReturn = EHIKVisionSetFunction_NotSupportError;
			break;
		}

		if(m_pHikDevice)
		{
			if(MV_CC_SetEnumValue(m_pHikDevice, "LineSelector", eParam))
			{
				eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], g_lpszHIKVisionGigELineSelect[nPreValue], g_lpszHIKVisionGigELineSelect[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}
EHIKVisionGetFunction CDeviceHIKVisionGigE::GetLineSelector(EDeviceHIKVisionGigELineSelect *eParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!eParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}

		*eParam = EDeviceHIKVisionGigELineSelect(_ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_DigitalLineSelector)));

		eReturn = EHIKVisionGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EHIKVisionGetFunction CDeviceHIKVisionGigE::GetLine0DebouncerTime(int *pParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_DigitalLine0DebouncerTime));

		eReturn = EHIKVisionGetFunction_OK;
	}
	while(false);

	return eReturn;
}
EHIKVisionSetFunction CDeviceHIKVisionGigE::SetLine0DebouncerTime(int nParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_DigitalLine0DebouncerTime;



	int nPreValue = _ttoi(GetParamValue(eSaveID));
	do
	{
		EDeviceHIKVisionGigELineSelect eLineSel;
		if(GetLineSelector(&eLineSel))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(eLineSel != EDeviceHIKVisionGigELineSelect_eLine0)
		{
			eReturn = EHIKVisionSetFunction_ReadOnDatabaseError;
			break;
		}

		if(m_pHikDevice)
		{
			if(!SetCommandInt32("LineDebouncerTime", nParam))
			{
				eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EHIKVisionGetFunction CDeviceHIKVisionGigE::GetLine2DebouncerTime(int *pParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_DigitalLine2DebouncerTime));

		eReturn = EHIKVisionGetFunction_OK;
	}
	while(false);

	return eReturn;
}
EHIKVisionSetFunction CDeviceHIKVisionGigE::SetLine2DebouncerTime(int nParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_DigitalLine2DebouncerTime;

	int nPreValue = _ttoi(GetParamValue(eSaveID));
	do
	{
		EDeviceHIKVisionGigELineSelect eLineSel;
		if(GetLineSelector(&eLineSel))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(eLineSel != EDeviceHIKVisionGigELineSelect_eLine2)
		{
			eReturn = EHIKVisionSetFunction_ReadOnDatabaseError;
			break;
		}

		EDeviceHIKVisionGigELineMode eLine2Mode;
		if(GetLine2Mode(&eLine2Mode))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}
		if(eLine2Mode != EDeviceHIKVisionGigELineMode_Input)
		{
			eReturn = EHIKVisionSetFunction_ReadOnDatabaseError;
			break;
		}

		if(m_pHikDevice)
		{
			if(!SetCommandInt32("LineDebouncerTime", nParam))
			{
				eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EHIKVisionGetFunction CDeviceHIKVisionGigE::GetLine1Inverter(bool *pParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_DigitalLine1Inverter));

		eReturn = EHIKVisionGetFunction_OK;
	}
	while(false);

	return eReturn;
}
EHIKVisionSetFunction CDeviceHIKVisionGigE::SetLine1Inverter(bool bParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_DigitalLine1Inverter;

	int nPreValue = _ttoi(GetParamValue(eSaveID));
	do
	{
		EDeviceHIKVisionGigELineSelect eLineSel;
		if(GetLineSelector(&eLineSel))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(eLineSel != EDeviceHIKVisionGigELineSelect_eLine1)
		{
			eReturn = EHIKVisionSetFunction_ReadOnDatabaseError;
			break;
		}

		if(m_pHikDevice)
		{
			if(!SetCommandBool("LineInverter", bParam))
			{
				eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], g_lpszHIKVisionGigESwitch[nPreValue], g_lpszHIKVisionGigESwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EHIKVisionGetFunction CDeviceHIKVisionGigE::GetLine2Inverter(bool *pParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_DigitalLine2Inverter));

		eReturn = EHIKVisionGetFunction_OK;
	}
	while(false);

	return eReturn;
}
EHIKVisionSetFunction CDeviceHIKVisionGigE::SetLine2Inverter(bool bParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_DigitalLine2Inverter;

	int nPreValue = _ttoi(GetParamValue(eSaveID));
	do
	{
		EDeviceHIKVisionGigELineSelect eLineSel;
		if(GetLineSelector(&eLineSel))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(eLineSel != EDeviceHIKVisionGigELineSelect_eLine2)
		{
			eReturn = EHIKVisionSetFunction_ReadOnDatabaseError;
			break;
		}

		EDeviceHIKVisionGigELineMode eLine2Mode;
		if(GetLine2Mode(&eLine2Mode))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}
		if(eLine2Mode != EDeviceHIKVisionGigELineMode_Strob)
		{
			eReturn = EHIKVisionSetFunction_ReadOnDatabaseError;
			break;
		}

		if(m_pHikDevice)
		{
			if(!SetCommandBool("LineInverter", bParam))
			{
				eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], g_lpszHIKVisionGigESwitch[nPreValue], g_lpszHIKVisionGigESwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EHIKVisionSetFunction CDeviceHIKVisionGigE::SetLine1Source(EDeviceHIKVisionGigELineSource eParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_DigitalLine1Source;
	int nPreValue = _ttoi(GetParamValue(eSaveID));
	do
	{
		if(eParam < EDeviceHIKVisionGigELineSource_ExposureStartActive || eParam >= EDeviceHIKVisionGigELineSource_Count)
		{
			eReturn = EHIKVisionSetFunction_NotSupportError;
			break;
		}

		EDeviceHIKVisionGigELineSelect eLineSel;
		if(GetLineSelector(&eLineSel))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(eLineSel != EDeviceHIKVisionGigELineSelect_eLine1)
		{
			eReturn = EHIKVisionSetFunction_ReadOnDatabaseError;
			break;
		}

		if(m_pHikDevice)
		{
			if(MV_CC_SetEnumValue(m_pHikDevice, "LineSource", eParam))
			{
				eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], g_lpszHIKVisionGigELineSource[nPreValue], g_lpszHIKVisionGigELineSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}
EHIKVisionGetFunction CDeviceHIKVisionGigE::GetLine1Source(EDeviceHIKVisionGigELineSource *pParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}

		*pParam = EDeviceHIKVisionGigELineSource(_ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_DigitalLine1Source)));

		eReturn = EHIKVisionGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EHIKVisionSetFunction CDeviceHIKVisionGigE::SetLine2Source(EDeviceHIKVisionGigELineSource eParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_DigitalLine2Source;
	int nPreValue = _ttoi(GetParamValue(eSaveID));
	do
	{
		if(eParam < EDeviceHIKVisionGigELineSource_ExposureStartActive || eParam >= EDeviceHIKVisionGigELineSource_Count)
		{
			eReturn = EHIKVisionSetFunction_NotSupportError;
			break;
		}

		EDeviceHIKVisionGigELineSelect eLineSel;
		if(GetLineSelector(&eLineSel))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(eLineSel != EDeviceHIKVisionGigELineSelect_eLine2)
		{
			eReturn = EHIKVisionSetFunction_ReadOnDatabaseError;
			break;
		}

		EDeviceHIKVisionGigELineMode eLine2Mode;
		if(GetLine2Mode(&eLine2Mode))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}
		if(eLine2Mode != EDeviceHIKVisionGigELineMode_Strob)
		{
			eReturn = EHIKVisionSetFunction_ReadOnDatabaseError;
			break;
		}

		if(m_pHikDevice)
		{
			if(MV_CC_SetEnumValue(m_pHikDevice, "LineSource", eParam))
			{
				eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], g_lpszHIKVisionGigELineSource[nPreValue], g_lpszHIKVisionGigELineSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}
EHIKVisionGetFunction CDeviceHIKVisionGigE::GetLine2Source(EDeviceHIKVisionGigELineSource *pParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}

		*pParam = EDeviceHIKVisionGigELineSource(_ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_DigitalLine2Source)));

		eReturn = EHIKVisionGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EHIKVisionGetFunction CDeviceHIKVisionGigE::GetLine1StrobEnable(bool *pParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_DigitalLine1StrobeEnable));

		eReturn = EHIKVisionGetFunction_OK;
	}
	while(false);

	return eReturn;
}
EHIKVisionSetFunction CDeviceHIKVisionGigE::SetLine1StrobEnable(bool bParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_DigitalLine1StrobeEnable;

	int nPreValue = _ttoi(GetParamValue(eSaveID));
	do
	{
		EDeviceHIKVisionGigELineSelect eLineSel;
		if(GetLineSelector(&eLineSel))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(eLineSel != EDeviceHIKVisionGigELineSelect_eLine1)
		{
			eReturn = EHIKVisionSetFunction_ReadOnDatabaseError;
			break;
		}

		if(m_pHikDevice)
		{
			if(!SetCommandBool("StrobeEnable", bParam))
			{
				eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], g_lpszHIKVisionGigESwitch[nPreValue], g_lpszHIKVisionGigESwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EHIKVisionGetFunction CDeviceHIKVisionGigE::GetLine2StrobEnable(bool *pParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_DigitalLine2StrobeEnable));

		eReturn = EHIKVisionGetFunction_OK;
	}
	while(false);

	return eReturn;
}
EHIKVisionSetFunction CDeviceHIKVisionGigE::SetLine2StrobEnable(bool bParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_DigitalLine2StrobeEnable;

	int nPreValue = _ttoi(GetParamValue(eSaveID));
	do
	{
		EDeviceHIKVisionGigELineSelect eLineSel;
		if(GetLineSelector(&eLineSel))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(eLineSel != EDeviceHIKVisionGigELineSelect_eLine2)
		{
			eReturn = EHIKVisionSetFunction_ReadOnDatabaseError;
			break;

		}

		EDeviceHIKVisionGigELineMode eLine2Mode;
		if(GetLine2Mode(&eLine2Mode))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}
		if(eLine2Mode != EDeviceHIKVisionGigELineMode_Strob)
		{
			eReturn = EHIKVisionSetFunction_ReadOnDatabaseError;
			break;
		}


		if(m_pHikDevice)
		{
			if(!SetCommandBool("StrobeEnable", bParam))
			{
				eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], g_lpszHIKVisionGigESwitch[nPreValue], g_lpszHIKVisionGigESwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EHIKVisionGetFunction CDeviceHIKVisionGigE::GetLine2StrobeLineDuration(int* pParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_DigitalLine2StrobeLineDuration));

		eReturn = EHIKVisionGetFunction_OK;
	}
	while(false);

	return eReturn;
}
EHIKVisionSetFunction CDeviceHIKVisionGigE::SetLine2StrobeLineDuration(int nParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_DigitalLine2StrobeLineDuration;

	int nPreValue = _ttoi(GetParamValue(eSaveID));
	do
	{
		EDeviceHIKVisionGigELineSelect eLineSel;
		if(GetLineSelector(&eLineSel))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(eLineSel != EDeviceHIKVisionGigELineSelect_eLine2)
		{
			eReturn = EHIKVisionSetFunction_ReadOnDatabaseError;
			break;
		}

		EDeviceHIKVisionGigELineMode eLine2Mode;
		if(GetLine2Mode(&eLine2Mode))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}
		if(eLine2Mode != EDeviceHIKVisionGigELineMode_Strob)
		{
			eReturn = EHIKVisionSetFunction_ReadOnDatabaseError;
			break;
		}


		if(m_pHikDevice)
		{
			if(!SetCommandInt32("StrobeLineDuration", nParam))
			{
				eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EHIKVisionGetFunction CDeviceHIKVisionGigE::GetLine1StrobeLineDuration(int* pParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_DigitalLine1StrobeLineDuration));

		eReturn = EHIKVisionGetFunction_OK;
	}
	while(false);

	return eReturn;
}
EHIKVisionSetFunction CDeviceHIKVisionGigE::SetLine1StrobeLineDuration(int nParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_DigitalLine1StrobeLineDuration;

	int nPreValue = _ttoi(GetParamValue(eSaveID));
	do
	{
		EDeviceHIKVisionGigELineSelect eLineSel;
		if(GetLineSelector(&eLineSel))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(eLineSel != EDeviceHIKVisionGigELineSelect_eLine1)
		{
			eReturn = EHIKVisionSetFunction_ReadOnDatabaseError;
			break;
		}

		EDeviceHIKVisionGigELineMode eLine2Mode;
		if(GetLine2Mode(&eLine2Mode))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}
		if(eLine2Mode != EDeviceHIKVisionGigELineMode_Strob)
		{
			eReturn = EHIKVisionSetFunction_ReadOnDatabaseError;
			break;
		}


		if(m_pHikDevice)
		{
			if(!SetCommandInt32("StrobeLineDuration", nParam))
			{
				eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EHIKVisionGetFunction CDeviceHIKVisionGigE::GetLine2StrobeLineDelay(int* pParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_DigitalLine2StrobeLineDelay));

		eReturn = EHIKVisionGetFunction_OK;
	}
	while(false);

	return eReturn;
}
EHIKVisionSetFunction CDeviceHIKVisionGigE::SetLine2StrobeLineDelay(int nParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_DigitalLine2StrobeLineDelay;

	int nPreValue = _ttoi(GetParamValue(eSaveID));
	do
	{
		EDeviceHIKVisionGigELineSelect eLineSel;
		if(GetLineSelector(&eLineSel))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(eLineSel != EDeviceHIKVisionGigELineSelect_eLine2)
		{
			eReturn = EHIKVisionSetFunction_ReadOnDatabaseError;
			break;
		}

		EDeviceHIKVisionGigELineMode eLine2Mode;
		if(GetLine2Mode(&eLine2Mode))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}
		if(eLine2Mode != EDeviceHIKVisionGigELineMode_Strob)
		{
			eReturn = EHIKVisionSetFunction_ReadOnDatabaseError;
			break;
		}


		if(m_pHikDevice)
		{
			if(!SetCommandInt32("StrobeLineDelay", nParam))
			{
				eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EHIKVisionGetFunction CDeviceHIKVisionGigE::GetLine2StrobeLinePreDelay(int* pParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_DigitalLine2StrobeLinePreDelay));

		eReturn = EHIKVisionGetFunction_OK;
	}
	while(false);

	return eReturn;
}
EHIKVisionSetFunction CDeviceHIKVisionGigE::SetLine2StrobeLinePreDelay(int nParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_DigitalLine2StrobeLinePreDelay;

	int nPreValue = _ttoi(GetParamValue(eSaveID));
	do
	{
		EDeviceHIKVisionGigELineSelect eLineSel;
		if(GetLineSelector(&eLineSel))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(eLineSel != EDeviceHIKVisionGigELineSelect_eLine2)
		{
			eReturn = EHIKVisionSetFunction_ReadOnDatabaseError;
			break;
		}

		EDeviceHIKVisionGigELineMode eLine2Mode;
		if(GetLine2Mode(&eLine2Mode))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}
		if(eLine2Mode != EDeviceHIKVisionGigELineMode_Strob)
		{
			eReturn = EHIKVisionSetFunction_ReadOnDatabaseError;
			break;
		}


		if(m_pHikDevice)
		{
			if(!SetCommandInt32("StrobeLinePreDelay", nParam))
			{
				eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EHIKVisionGetFunction CDeviceHIKVisionGigE::GetLine1StrobeLineDelay(int* pParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_DigitalLine1StrobeLineDelay));

		eReturn = EHIKVisionGetFunction_OK;
	}
	while(false);

	return eReturn;
}
EHIKVisionSetFunction CDeviceHIKVisionGigE::SetLine1StrobeLineDelay(int nParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_DigitalLine1StrobeLineDelay;

	int nPreValue = _ttoi(GetParamValue(eSaveID));
	do
	{
		EDeviceHIKVisionGigELineSelect eLineSel;
		if(GetLineSelector(&eLineSel))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(eLineSel != EDeviceHIKVisionGigELineSelect_eLine1)
		{
			eReturn = EHIKVisionSetFunction_ReadOnDatabaseError;
			break;
		}

		if(m_pHikDevice)
		{
			if(!SetCommandInt32("StrobeLineDelay", nParam))
			{
				eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EHIKVisionGetFunction CDeviceHIKVisionGigE::GetLine1StrobeLinePreDelay(int* pParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_DigitalLine1StrobeLinePreDelay));

		eReturn = EHIKVisionGetFunction_OK;
	}
	while(false);

	return eReturn;
}
EHIKVisionSetFunction CDeviceHIKVisionGigE::SetLine1StrobeLinePreDelay(int nParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_DigitalLine1StrobeLinePreDelay;

	int nPreValue = _ttoi(GetParamValue(eSaveID));
	do
	{
		EDeviceHIKVisionGigELineSelect eLineSel;
		if(GetLineSelector(&eLineSel))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(eLineSel != EDeviceHIKVisionGigELineSelect_eLine1)
		{
			eReturn = EHIKVisionSetFunction_ReadOnDatabaseError;
			break;
		}

		if(m_pHikDevice)
		{
			if(!SetCommandInt32("StrobeLinePreDelay", nParam))
			{
				eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EHIKVisionSetFunction CDeviceHIKVisionGigE::SetLine1HardWareTriggerSource(EDeviceHIKVisionGigEHardTriggerLineSelect eParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_DigitalLine1StrobeHardTriggerSource;
	int nPreValue = _ttoi(GetParamValue(eSaveID));

	unsigned int nInputValue = 0;
	if(eParam == 1)
		nInputValue = 2;

	do
	{
		if(eParam < EDeviceHIKVisionGigEHardTriggerLineSelect_Line0 || eParam >= EDeviceHIKVisionGigEHardTriggerLineSelect_Count)
		{
			eReturn = EHIKVisionSetFunction_NotSupportError;
			break;
		}

		EDeviceHIKVisionGigELineSelect eLineSel;
		if(GetLineSelector(&eLineSel))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(eLineSel != EDeviceHIKVisionGigELineSelect_eLine1)
		{
			eReturn = EHIKVisionSetFunction_ReadOnDatabaseError;
			break;
		}

		EDeviceHIKVisionGigELineMode eLine2Mode;
		if(GetLine2Mode(&eLine2Mode))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}
		if(eLine2Mode != EDeviceHIKVisionGigELineMode_Input)
		{
			eReturn = EHIKVisionSetFunction_ReadOnDatabaseError;
			break;
		}


		if(m_pHikDevice)
		{
			if(MV_CC_SetEnumValue(m_pHikDevice, "HardwareTriggerSource", nInputValue))
			{
				eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], g_lpszHIKVisionGigEHardTriggerLineSelect[nPreValue], g_lpszHIKVisionGigEHardTriggerLineSelect[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}
EHIKVisionGetFunction CDeviceHIKVisionGigE::GetLine1HardWareTriggerSource(EDeviceHIKVisionGigEHardTriggerLineSelect *eParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!eParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}
		*eParam = EDeviceHIKVisionGigEHardTriggerLineSelect(_ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_DigitalLine1StrobeHardTriggerSource)));

		eReturn = EHIKVisionGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EHIKVisionSetFunction CDeviceHIKVisionGigE::SetLine2HardWareTriggerSource(EDeviceHIKVisionGigEHardTriggerLineSelect eParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_DigitalLine1StrobeHardTriggerSource;
	int nPreValue = _ttoi(GetParamValue(eSaveID));

	unsigned int nInputValue = 0;
	if(eParam == 1)
		nInputValue = 2;

	do
	{
		if(eParam < EDeviceHIKVisionGigEHardTriggerLineSelect_Line0 || eParam >= EDeviceHIKVisionGigEHardTriggerLineSelect_Count)
		{
			eReturn = EHIKVisionSetFunction_NotSupportError;
			break;
		}

		EDeviceHIKVisionGigELineSelect eLineSel;
		if(GetLineSelector(&eLineSel))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(eLineSel != EDeviceHIKVisionGigELineSelect_eLine2)
		{
			eReturn = EHIKVisionSetFunction_ReadOnDatabaseError;
			break;
		}

		EDeviceHIKVisionGigELineMode eLine2Mode;
		if(GetLine2Mode(&eLine2Mode))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}
		if(eLine2Mode != EDeviceHIKVisionGigELineMode_Strob)
		{
			eReturn = EHIKVisionSetFunction_ReadOnDatabaseError;
			break;
		}

		if(m_pHikDevice)
		{
			if(MV_CC_SetEnumValue(m_pHikDevice, "HardwareTriggerSource", nInputValue))
			{
				eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], g_lpszHIKVisionGigEHardTriggerLineSelect[nPreValue], g_lpszHIKVisionGigEHardTriggerLineSelect[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}
EHIKVisionGetFunction CDeviceHIKVisionGigE::GetLine2HardWareTriggerSource(EDeviceHIKVisionGigEHardTriggerLineSelect *eParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!eParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}

		*eParam = EDeviceHIKVisionGigEHardTriggerLineSelect(_ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_DigitalLine1StrobeHardTriggerSource)));

		eReturn = EHIKVisionGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EHIKVisionSetFunction CDeviceHIKVisionGigE::SetLine1HardWareTriggerActivation(EDeviceHIKVisionGigETriggerActivation eParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_DigitalLine1StrobeHardTriggerActivaion;
	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < EDeviceHIKVisionGigETriggerActivation_RisingEdge || eParam >= EDeviceHIKVisionGigETriggerActivation_Count)
		{
			eReturn = EHIKVisionSetFunction_NotSupportError;
			break;
		}

		EDeviceHIKVisionGigELineSelect eLineSel;
		if(GetLineSelector(&eLineSel))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(eLineSel != EDeviceHIKVisionGigELineSelect_eLine1)
		{
			eReturn = EHIKVisionSetFunction_ReadOnDatabaseError;
			break;
		}

		if(m_pHikDevice)
		{
			if(MV_CC_SetEnumValue(m_pHikDevice, "HardwareTriggerActivation", int(eParam)))
			{
				eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], g_lpszHIKVisionGigTriggerAcivation[nPreValue], g_lpszHIKVisionGigTriggerAcivation[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}
EHIKVisionGetFunction CDeviceHIKVisionGigE::GetLine1HardWareTriggerActivation(EDeviceHIKVisionGigETriggerActivation *eParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!eParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}

		*eParam = EDeviceHIKVisionGigETriggerActivation(_ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_DigitalLine1StrobeHardTriggerActivaion)));

		eReturn = EHIKVisionGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EHIKVisionSetFunction CDeviceHIKVisionGigE::SetLine2HardWareTriggerActivation(EDeviceHIKVisionGigETriggerActivation eParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_DigitalLine2StrobeHardTriggerActivaion;
	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < EDeviceHIKVisionGigETriggerActivation_RisingEdge || eParam >= EDeviceHIKVisionGigETriggerActivation_Count)
		{
			eReturn = EHIKVisionSetFunction_NotSupportError;
			break;
		}

		EDeviceHIKVisionGigELineSelect eLineSel;
		if(GetLineSelector(&eLineSel))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(eLineSel != EDeviceHIKVisionGigELineSelect_eLine2)
		{
			eReturn = EHIKVisionSetFunction_ReadOnDatabaseError;
			break;
		}

		EDeviceHIKVisionGigELineMode eLine2Mode;
		if(GetLine2Mode(&eLine2Mode))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}
		if(eLine2Mode != EDeviceHIKVisionGigELineMode_Strob)
		{
			eReturn = EHIKVisionSetFunction_ReadOnDatabaseError;
			break;
		}

		if(m_pHikDevice)
		{
			if(MV_CC_SetEnumValue(m_pHikDevice, "HardwareTriggerActivation", int(eParam)))
			{
				eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], g_lpszHIKVisionGigTriggerAcivation[nPreValue], g_lpszHIKVisionGigTriggerAcivation[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}
EHIKVisionGetFunction CDeviceHIKVisionGigE::GetLine2HardWareTriggerActivation(EDeviceHIKVisionGigETriggerActivation *eParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!eParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}

		*eParam = EDeviceHIKVisionGigETriggerActivation(_ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_DigitalLine2StrobeHardTriggerActivaion)));

		eReturn = EHIKVisionGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EHIKVisionSetFunction CDeviceHIKVisionGigE::SetLine2Mode(EDeviceHIKVisionGigELineMode eParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_DigitalLine2Mode;
	int nPreValue = _ttoi(GetParamValue(eSaveID));

	unsigned int unTemp = 0;

	if(eParam == 1)
		unTemp = 8;

	do
	{
		if(eParam < EDeviceHIKVisionGigELineMode_Input || eParam >= EDeviceHIKVisionGigELineMode_Count)
		{
			eReturn = EHIKVisionSetFunction_NotSupportError;
			break;
		}

		EDeviceHIKVisionGigELineSelect eLineSel;
		if(GetLineSelector(&eLineSel))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(eLineSel != EDeviceHIKVisionGigELineSelect_eLine2)
		{
			eReturn = EHIKVisionSetFunction_ReadOnDatabaseError;
			break;
		}

		if(m_pHikDevice)
		{
			if(MV_CC_SetEnumValue(m_pHikDevice, "LineMode", unTemp))
			{
				eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], g_lpszHIKVisionGigELineMode[nPreValue], g_lpszHIKVisionGigELineMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}
EHIKVisionGetFunction CDeviceHIKVisionGigE::GetLine2Mode(EDeviceHIKVisionGigELineMode* pParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}

		*pParam = EDeviceHIKVisionGigELineMode(_ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_DigitalLine2Mode)));

		eReturn = EHIKVisionGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EHIKVisionGetFunction CDeviceHIKVisionGigE::GetAcquisitionBurstFrameCount(int* pParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_AcquisitionBurstFrameCount));

		eReturn = EHIKVisionGetFunction_OK;
	}
	while(false);

	return eReturn;
}
EHIKVisionSetFunction CDeviceHIKVisionGigE::SetAcquisitionBurstFrameCount(int nParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_AcquisitionBurstFrameCount;

	int nPreValue = _ttoi(GetParamValue(eSaveID));
	do
	{
		if(m_pHikDevice)
		{
			if(!SetCommandInt32("AcquisitionBurstFrameCount", nParam))
			{
				eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EHIKVisionGetFunction CDeviceHIKVisionGigE::GetAcquisitionFrameRate(float* pParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterHIKVisionGigE_AcquisitionFrameRate));

		eReturn = EHIKVisionGetFunction_OK;
	}
	while(false);

	return eReturn;
}
EHIKVisionSetFunction CDeviceHIKVisionGigE::SetAcquisitionFrameRate(float fParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_AcquisitionFrameRate;

	float fPreValue = _ttof(GetParamValue(eSaveID));
	do
	{
		if(m_pHikDevice)
		{
			if(!SetCommandFloat("AcquisitionFrameRate", fParam))
			{
				eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EHIKVisionGetFunction CDeviceHIKVisionGigE::GetAcquisitionFrameRateControlEnable(bool *pParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_AcquisitionFrameRateControlEnable));

		eReturn = EHIKVisionGetFunction_OK;
	}
	while(false);

	return eReturn;
}
EHIKVisionSetFunction CDeviceHIKVisionGigE::SetAcquisitionFrameRateControlEnable(bool bParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_AcquisitionFrameRateControlEnable;

	int nPreValue = _ttoi(GetParamValue(eSaveID));
	do
	{
		if(m_pHikDevice)
		{
			if(!SetCommandBool("AcquisitionFrameRateEnable", bParam))
			{
				eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], g_lpszHIKVisionGigESwitch[nPreValue], g_lpszHIKVisionGigESwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EHIKVisionGetFunction CDeviceHIKVisionGigE::GetAcquisitionTriggerSelector(EDeviceHIKVisionGigEAcquisitionTriggerSelector *pParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}

		*pParam = EDeviceHIKVisionGigEAcquisitionTriggerSelector(_ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_AcquisitionTriggerSelector)));

		eReturn = EHIKVisionGetFunction_OK;
	}
	while(false);

	return eReturn;
}
EHIKVisionSetFunction CDeviceHIKVisionGigE::SetAcquisitionTriggerSelector(EDeviceHIKVisionGigEAcquisitionTriggerSelector eParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_AcquisitionTriggerSelector;
	int nPreValue = _ttoi(GetParamValue(eSaveID));


	do
	{
		if(eParam < EDeviceHIKVisionGigEAcquisitionTriggerSelector_EFrameBurstStart || eParam >= EDeviceHIKVisionGigEAcquisitionTriggerSelector_Count)
		{
			eReturn = EHIKVisionSetFunction_NotSupportError;
			break;
		}

		unsigned int unTemp = 0;

		if(eParam == EDeviceHIKVisionGigEAcquisitionTriggerSelector_EFrameBurstStart)
			unTemp = 6;

		if(m_pHikDevice)
		{
			if(MV_CC_SetEnumValue(m_pHikDevice, "TriggerSelector", unTemp))
			{
				eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], g_lpszHIKVisionGigEAcquisitionTriggerSelector[nPreValue], g_lpszHIKVisionGigEAcquisitionTriggerSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EHIKVisionGetFunction CDeviceHIKVisionGigE::GetAcquisitionTriggerMode(EDeviceHIKVisionGigESwitch* pParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}

		*pParam = EDeviceHIKVisionGigESwitch(_ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_AcquisitionTriggerMode)));

		eReturn = EHIKVisionGetFunction_OK;
	}
	while(false);

	return eReturn;
}
EHIKVisionSetFunction CDeviceHIKVisionGigE::SetAcquisitionTriggerMode(EDeviceHIKVisionGigESwitch eParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_AcquisitionTriggerMode;
	int nPreValue = _ttoi(GetParamValue(eSaveID));

	unsigned int unTemp = 0;

	do
	{
		if(eParam < EDeviceHIKVisionGigESwitch_Off || eParam >= EDeviceHIKVisionGigESwitch_Count)
		{
			eReturn = EHIKVisionSetFunction_NotSupportError;
			break;
		}

		if(m_pHikDevice)
		{
			int nRet = MV_CC_SetEnumValue(m_pHikDevice, "TriggerMode", eParam);
			if(MV_OK != nRet)
			{
				eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], g_lpszHIKVisionGigESwitch[nPreValue], g_lpszHIKVisionGigESwitch[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EHIKVisionGetFunction CDeviceHIKVisionGigE::GetAcquisitionTriggerSource(EDeviceHIKVisionGigEAcquisitionTriggerSource* pParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}

		*pParam = EDeviceHIKVisionGigEAcquisitionTriggerSource(_ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_AcquisitionTriggerSource)));

		eReturn = EHIKVisionGetFunction_OK;
	}
	while(false);

	return eReturn;
}
EHIKVisionSetFunction CDeviceHIKVisionGigE::SetAcquisitionTriggerSource(EDeviceHIKVisionGigEAcquisitionTriggerSource eParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_AcquisitionTriggerSource;
	int nPreValue = _ttoi(GetParamValue(eSaveID));

	unsigned int unTemp = 0;

	do
	{
		switch(eParam)
		{
		case EDeviceHIKVisionGigETriggerSource_SoftWare:
			unTemp = 7;
			break;
		case EDeviceHIKVisionGigETriggerSource_Line0:
			unTemp = 0;
			break;
		case EDeviceHIKVisionGigETriggerSource_Line2:
			unTemp = 2;
			break;
		case EDeviceHIKVisionGigETriggerSource_Counter0:
			unTemp = 4;
			break;
		default:
			eReturn = EHIKVisionSetFunction_NotSupportError;
			break;
		}

		if(eReturn == EHIKVisionSetFunction_NotSupportError)
			break;

		if(m_pHikDevice)
		{

			if(MV_CC_SetEnumValue(m_pHikDevice, "TriggerSource", unTemp))
			{
				eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], g_lpszHIKVisionGigETriggerSource[nPreValue], g_lpszHIKVisionGigETriggerSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EHIKVisionGetFunction CDeviceHIKVisionGigE::GetAcquisitionTriggerActivation(EDeviceHIKVisionGigEAcquisitionTriggerActivation* pParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}

		*pParam = EDeviceHIKVisionGigEAcquisitionTriggerActivation(_ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_AcquisitionTriggerActivation)));

		eReturn = EHIKVisionGetFunction_OK;
	}
	while(false);

	return eReturn;
}
EHIKVisionSetFunction CDeviceHIKVisionGigE::SetAcquisitionTriggerActivation(EDeviceHIKVisionGigEAcquisitionTriggerActivation eParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_AcquisitionTriggerActivation;
	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < EDeviceHIKVisionGigEAcquisitionTriggerActivation_RisingEdge || eParam >= EDeviceHIKVisionGigEAcquisitionTriggerActivation_Count)
		{
			eReturn = EHIKVisionSetFunction_NotSupportError;
			break;
		}

		if(m_pHikDevice)
		{
			if(MV_CC_SetEnumValue(m_pHikDevice, "TriggerActivation", eParam))
			{
				eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], g_lpszHIKVisionGigTriggerAcivation[nPreValue], g_lpszHIKVisionGigTriggerAcivation[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EHIKVisionGetFunction CDeviceHIKVisionGigE::GetAcquisitionTriggerDelay(float* pParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterHIKVisionGigE_AcquisitionTriggerDelay));

		eReturn = EHIKVisionGetFunction_OK;
	}
	while(false);

	return eReturn;
}
EHIKVisionSetFunction CDeviceHIKVisionGigE::SetAcquisitionTriggerDelay(float fParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_AcquisitionTriggerDelay;

	float fPreValue = _ttof(GetParamValue(eSaveID));
	do
	{
		if(m_pHikDevice)
		{
			if(!SetCommandFloat("TriggerDelay", fParam))
			{
				eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EHIKVisionGetFunction CDeviceHIKVisionGigE::GetAcquisitionExposureMode(EDeviceHIKVisionGigEAcquisitionExposureMode* pParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}

		*pParam = EDeviceHIKVisionGigEAcquisitionExposureMode(_ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_AcquisitionExposureMode)));

		eReturn = EHIKVisionGetFunction_OK;
	}
	while(false);

	return eReturn;
}
EHIKVisionSetFunction CDeviceHIKVisionGigE::SetAcquisitionExposureMode(EDeviceHIKVisionGigEAcquisitionExposureMode eParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_AcquisitionExposureMode;
	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < EDeviceHIKVisionGigEAcquisitionExposureMode_Timed || eParam >= EDeviceHIKVisionGigEAcquisitionExposureMode_Count)
		{
			eReturn = EHIKVisionSetFunction_NotSupportError;
			break;
		}

		if(m_pHikDevice)
		{
			if(MV_CC_SetEnumValue(m_pHikDevice, "ExposureMode", eParam))
			{
				eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], g_lpszHIKVisionGigEExposureMode[nPreValue], g_lpszHIKVisionGigEExposureMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EHIKVisionGetFunction CDeviceHIKVisionGigE::GetAcquisitionExposureTime(float *pParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterHIKVisionGigE_AcquisitionExposureTime));

		eReturn = EHIKVisionGetFunction_OK;
	}
	while(false);

	return eReturn;
}
EHIKVisionSetFunction CDeviceHIKVisionGigE::SetAcquisitionExposureTime(float fParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_AcquisitionExposureTime;

	float fPreValue = _ttof(GetParamValue(eSaveID));
	do
	{
		if(fParam < 34)
			fParam = 34;
		else if(fParam > 9.9995e+6)
			fParam = 9.9995e+6;

		if(m_pHikDevice)
		{

			if(!SetCommandFloat("ExposureTime", fParam))
			{
				eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
				break;
			}

		}

		CString strValue;
		strValue.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EHIKVisionGetFunction CDeviceHIKVisionGigE::GetAcquisitionExposureAuto(EDeviceHIKVisionGigEAuto *pParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}

		*pParam = EDeviceHIKVisionGigEAuto(_ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_AcquisitionExposureAuto)));

		eReturn = EHIKVisionGetFunction_OK;
	}
	while(false);

	return eReturn;
}
EHIKVisionSetFunction CDeviceHIKVisionGigE::SetAcquisitionExposureAuto(EDeviceHIKVisionGigEAuto eParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_AcquisitionExposureAuto;
	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < EDeviceHIKVisionGigEAuto_Off || eParam >= EDeviceHIKVisionGigEAuto_Count)
		{
			eReturn = EHIKVisionSetFunction_NotSupportError;
			break;
		}

		if(m_pHikDevice)
		{
			if(MV_CC_SetEnumValue(m_pHikDevice, "ExposureAuto", eParam))
			{
				eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
				break;
			}
		}

		if(eParam == EDeviceHIKVisionGigEAuto::EDeviceHIKVisionGigEAuto_Once || eParam == EDeviceHIKVisionGigEAuto::EDeviceHIKVisionGigEAuto_Continuous)
		{
			float fPreValue = 0;

			if(GetAcquisitionExposureTime(&fPreValue))
				break;

			float fValue = fPreValue;

			if(m_pHikDevice)
			{
				MVCC_FLOATVALUE mvccTemp;
				if(MV_CC_GetFloatValue(m_pHikDevice, "ExposureTime", &mvccTemp))
					break;

				fValue = mvccTemp.fCurValue;
			}

			if(eParam == EDeviceHIKVisionGigEAuto::EDeviceHIKVisionGigEAuto_Once)
			{
				SetAcquisitionExposureTime(fValue);
				eParam = EDeviceHIKVisionGigEAuto::EDeviceHIKVisionGigEAuto_Off;
			}
		}



		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], g_lpszHIKVisionGigEAuto[nPreValue], g_lpszHIKVisionGigEAuto[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EHIKVisionGetFunction CDeviceHIKVisionGigE::GetGrabCount(int *pParam)
{
	EHIKVisionGetFunction eReturn = EHIKVisionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EHIKVisionGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_DeviceGrabCount));

		eReturn = EHIKVisionGetFunction_OK;
	}
	while(false);

	return eReturn;
}
EHIKVisionSetFunction CDeviceHIKVisionGigE::SetGrabCount(int nParam)
{
	EHIKVisionSetFunction eReturn = EHIKVisionSetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_DeviceGrabCount;

	int nPreValue = _ttoi(GetParamValue(eSaveID));
	do
	{
		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EHIKVisionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EHIKVisionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;

}

EDeviceHIKVisionGigEGetFunction CDeviceHIKVisionGigE::GetPixelFormat(EDeviceHIKVisionGigEPixelFormat * pParam)
{
	EDeviceHIKVisionGigEGetFunction eReturn = EDeviceHIKVisionGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceHIKVisionGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceHIKVisionGigEPixelFormat)_ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_PixelFormat));

		eReturn = EDeviceHIKVisionGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceHIKVisionGigEGetFunction CDeviceHIKVisionGigE::SetPixelFormat(EDeviceHIKVisionGigEPixelFormat eParam)
{
	EDeviceHIKVisionGigEGetFunction eReturn = EDeviceHIKVisionGigEGetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_PixelFormat;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < EDeviceHIKVisionGigEPixelFormat_Mono8 || eParam >= EDeviceHIKVisionGigEPixelFormat_Count)
		{
			eReturn = EDeviceHIKVisionGigEGetFunction_EnumTypeError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EDeviceHIKVisionGigEGetFunction_EnumTypeError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceHIKVisionGigEGetFunction_EnumTypeError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceHIKVisionGigEGetFunction_EnumTypeError;
			break;
		}

		eReturn = EDeviceHIKVisionGigEGetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], g_lpszHIKVisionGigEPixelFormat[nPreValue], g_lpszHIKVisionGigEPixelFormat[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceHIKVisionGigEGetFunction CDeviceHIKVisionGigE::GetWidth(int * pParam)
{
	EDeviceHIKVisionGigEGetFunction eReturn = EDeviceHIKVisionGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceHIKVisionGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_Width));

		eReturn = EDeviceHIKVisionGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDeviceHIKVisionGigEGetFunction CDeviceHIKVisionGigE::SetWidth(int nParam)
{
	EDeviceHIKVisionGigEGetFunction eReturn = EDeviceHIKVisionGigEGetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_Width;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceHIKVisionGigEGetFunction_EnumTypeError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceHIKVisionGigEGetFunction_EnumTypeError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceHIKVisionGigEGetFunction_EnumTypeError;
			break;
		}

		eReturn = EDeviceHIKVisionGigEGetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceHIKVisionGigEGetFunction CDeviceHIKVisionGigE::GetHeight(int * pParam)
{
	EDeviceHIKVisionGigEGetFunction eReturn = EDeviceHIKVisionGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDeviceHIKVisionGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterHIKVisionGigE_Height));

		eReturn = EDeviceHIKVisionGigEGetFunction_OK;
	}
	while(false);

	return eReturn;;
}

EDeviceHIKVisionGigEGetFunction CDeviceHIKVisionGigE::SetHeight(int nParam)
{
	EDeviceHIKVisionGigEGetFunction eReturn = EDeviceHIKVisionGigEGetFunction_UnknownError;

	EDeviceParameterHIKVisionGigE eSaveID = EDeviceParameterHIKVisionGigE_Height;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDeviceHIKVisionGigEGetFunction_EnumTypeError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EDeviceHIKVisionGigEGetFunction_EnumTypeError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDeviceHIKVisionGigEGetFunction_EnumTypeError;
			break;
		}

		eReturn = EDeviceHIKVisionGigEGetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamHIKVisionGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}
EDeviceHIKVisionGigEGetFunction CDeviceHIKVisionGigE::SetHeartbeatTimeout_ms(_In_ unsigned int unParam)
{
	EDeviceHIKVisionGigEGetFunction eReturn = EDeviceHIKVisionGigEGetFunction_UnknownError;

	do 
	{
		if(!m_pHikDevice)
			break;

		try
		{
			if(MV_GIGE_SetGvcpTimeout(m_pHikDevice, unParam) == 0)
			{
				eReturn = EDeviceHIKVisionGigEGetFunction_OK;
			}
		}
		catch(...) { }
	} 
	while(false);

	return eReturn;
}

bool CDeviceHIKVisionGigE::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = false;
	bool bFoundID = true;

	do
	{
		switch(nParam)
		{
		case EDeviceParameterHIKVisionGigE_DeviceID:
			{
				bReturn = !SetDeviceID(strValue);
			}
			break;
		case EDeviceParameterHIKVisionGigE_InitializeMode:
		{
			bReturn = !SetInitializeMode(EDeviceHIKVisionGigEInitMode((_ttoi(strValue))));
		}
		break;
		case EDeviceParameterHIKVisionGigE_DeviceStreamChannelPackSize:
			{
				bReturn = !SetDeviceStreamChannelPackSize(_ttoi(strValue));
			}
			break;
		case EDeviceParameterHIKVisionGigE_Width:
			{
				bReturn = !SetCanvasWidth(_ttoi(strValue));
			}
			break;
		case EDeviceParameterHIKVisionGigE_Height:
			{
				bReturn = !SetCanvasHeight(_ttoi(strValue));
			}
			break;
		case EDeviceParameterHIKVisionGigE_OffsetX:
			{
				bReturn = !SetOffsetX(_ttoi(strValue));
			}
			break;
		case EDeviceParameterHIKVisionGigE_OffsetY:
			{
				bReturn = !SetOffsetY(_ttoi(strValue));
			}
			break;
		case EDeviceParameterHIKVisionGigE_ReverseX:
			{
				bReturn = !SetReverseX(_ttoi(strValue));
			}
			break;
		case EDeviceParameterHIKVisionGigE_ReverseY:
			{
				bReturn = !SetReverseY(_ttoi(strValue));
			}
			break;
		case EDeviceParameterHIKVisionGigE_PixelFormat:
			{
				bReturn = !SetImagePixelFormat(EDeviceHIKVisionGigEPixelFormat((_ttoi(strValue))));
			}
			break;
		case EDeviceParameterHIKVisionGigE_AcquisitionMode:
			{
				bReturn = !SetAcquisitionMode(EDeviceHIKVisionAcquisitionMode(_ttoi(strValue)));
			}
			break;
		case EDeviceParameterHIKVisionGigE_AcquisitionBurstFrameCount:
			{
				bReturn = !SetAcquisitionBurstFrameCount((_ttoi(strValue)));
			}
			break;
		case EDeviceParameterHIKVisionGigE_AcquisitionFrameRate:
			{
				bReturn = !SetAcquisitionFrameRate((_ttof(strValue)));
			}
			break;
		case EDeviceParameterHIKVisionGigE_AcquisitionFrameRateControlEnable:
			{
				bReturn = !SetAcquisitionFrameRateControlEnable((_ttoi(strValue)));
			}
			break;
		case EDeviceParameterHIKVisionGigE_AcquisitionTriggerSelector:
			{
				bReturn = !SetAcquisitionTriggerSelector(EDeviceHIKVisionGigEAcquisitionTriggerSelector(_ttoi(strValue)));
			}
			break;
		case EDeviceParameterHIKVisionGigE_AcquisitionTriggerMode:
			{
				bReturn = !SetAcquisitionTriggerMode(EDeviceHIKVisionGigESwitch(_ttoi(strValue)));
			}
			break;
		case EDeviceParameterHIKVisionGigE_AcquisitionTriggerSource:
			{
				bReturn = !SetAcquisitionTriggerSource(EDeviceHIKVisionGigEAcquisitionTriggerSource(_ttoi(strValue)));
			}
			break;
		case EDeviceParameterHIKVisionGigE_AcquisitionTriggerActivation:
			{
				bReturn = !SetAcquisitionTriggerActivation(EDeviceHIKVisionGigEAcquisitionTriggerActivation(_ttoi(strValue)));
			}
			break;
		case EDeviceParameterHIKVisionGigE_AcquisitionTriggerDelay:
			{
				bReturn = !SetAcquisitionTriggerDelay(_ttof(strValue));
			}
			break;
		case EDeviceParameterHIKVisionGigE_AcquisitionExposureMode:
			{
				bReturn = !SetAcquisitionExposureMode(EDeviceHIKVisionGigEAcquisitionExposureMode(_ttoi(strValue)));
			}
			break;
		case EDeviceParameterHIKVisionGigE_AcquisitionExposureTime:
			{
				bReturn = !SetAcquisitionExposureTime(_ttof(strValue));
			}
			break;
		case EDeviceParameterHIKVisionGigE_AcquisitionExposureAuto:
			{
				bReturn = !SetAcquisitionExposureAuto(EDeviceHIKVisionGigEAuto(_ttoi(strValue)));
			}
			break;
		case EDeviceParameterHIKVisionGigE_AnalogGaindB:
			{
				bReturn = !SetAnalogGaindB(_ttof(strValue));
			}
			break;
		case EDeviceParameterHIKVisionGigE_AnalogGainAuto:
			{
				bReturn = !SetAnalogGainAuto(EDeviceHIKVisionGigEGainAuto(_ttoi(strValue)));
			}
			break;
		case EDeviceParameterHIKVisionGigE_AnalogDigitalShift:
			{
				bReturn = !SetDigitalShift(_ttof(strValue));
			}
			break;
		case EDeviceParameterHIKVisionGigE_AnalogDigitalShiftEnable:
			{
				bReturn = !SetDigitalShiftEnable(_ttof(strValue));
			}
			break;
		case EDeviceParameterHIKVisionGigE_AnalogBlackLevel:
			{
				bReturn = !SetBlackLevel(_ttof(strValue));
			}
			break;
		case EDeviceParameterHIKVisionGigE_AnalogBlackLevelEnable:
			{
				bReturn = !SetBlackLevelEnable(_ttof(strValue));
			}
			break;
		case EDeviceParameterHIKVisionGigE_DigitalLineSelector:
			{
				bReturn = !SetLineSelector(EDeviceHIKVisionGigELineSelect(_ttoi(strValue)));
			}
			break;
		case EDeviceParameterHIKVisionGigE_DigitalLine0DebouncerTime:
			{
				bReturn = !SetLine0DebouncerTime(_ttoi(strValue));
			}
			break;
		case EDeviceParameterHIKVisionGigE_DigitalLine2DebouncerTime:
			{
				bReturn = !SetLine2DebouncerTime(_ttoi(strValue));
			}
			break;
		case EDeviceParameterHIKVisionGigE_DigitalLine1Inverter:
			{
				bReturn = !SetLine1Inverter(_ttoi(strValue));
			}
			break;
		case EDeviceParameterHIKVisionGigE_DigitalLine2Inverter:
			{
				bReturn = !SetLine2Inverter(_ttoi(strValue));
			}
			break;
		case EDeviceParameterHIKVisionGigE_DigitalLine1Source:
			{
				bReturn = !SetLine1Source(EDeviceHIKVisionGigELineSource(_ttoi(strValue)));
			}
			break;
		case EDeviceParameterHIKVisionGigE_DigitalLine2Source:
			{
				bReturn = !SetLine2Source(EDeviceHIKVisionGigELineSource(_ttoi(strValue)));
			}
			break;
		case EDeviceParameterHIKVisionGigE_DigitalLine1StrobeEnable:
			{
				bReturn = !SetLine1StrobEnable(_ttoi(strValue));
			}
			break;
		case EDeviceParameterHIKVisionGigE_DigitalLine2StrobeEnable:
			{
				bReturn = !SetLine2StrobEnable(_ttoi(strValue));
			}
			break;
		case EDeviceParameterHIKVisionGigE_DigitalLine2StrobeLineDuration:
			{
				bReturn = !SetLine2StrobeLineDuration(_ttoi(strValue));
			}
			break;
		case EDeviceParameterHIKVisionGigE_DigitalLine2StrobeLineDelay:
			{
				bReturn = !SetLine2StrobeLineDelay(_ttoi(strValue));
			}
			break;
		case EDeviceParameterHIKVisionGigE_DigitalLine2StrobeLinePreDelay:
			{
				bReturn = !SetLine2StrobeLinePreDelay(_ttoi(strValue));
			}
			break;
		case EDeviceParameterHIKVisionGigE_DigitalLine1StrobeLineDuration:
			{
				bReturn = !SetLine1StrobeLineDuration(_ttoi(strValue));
			}
			break;
		case EDeviceParameterHIKVisionGigE_DigitalLine1StrobeLineDelay:
			{
				bReturn = !SetLine1StrobeLineDelay(_ttoi(strValue));
			}
			break;
		case EDeviceParameterHIKVisionGigE_DigitalLine1StrobeLinePreDelay:
			{
				bReturn = !SetLine1StrobeLinePreDelay(_ttoi(strValue));
			}
			break;
		case EDeviceParameterHIKVisionGigE_DigitalLine1StrobeHardTriggerSource:
			{
				bReturn = !SetLine1HardWareTriggerSource(EDeviceHIKVisionGigEHardTriggerLineSelect(_ttoi(strValue)));
			}
			break;
		case EDeviceParameterHIKVisionGigE_DigitalLine2StrobeHardTriggerSource:
			{
				bReturn = !SetLine2HardWareTriggerSource(EDeviceHIKVisionGigEHardTriggerLineSelect(_ttoi(strValue)));
			}
			break;
		case EDeviceParameterHIKVisionGigE_DigitalLine1StrobeHardTriggerActivaion:
			{
				bReturn = !SetLine1HardWareTriggerActivation(EDeviceHIKVisionGigETriggerActivation(_ttoi(strValue)));
			}
			break;
		case EDeviceParameterHIKVisionGigE_DigitalLine2StrobeHardTriggerActivaion:
			{
				bReturn = !SetLine2HardWareTriggerActivation(EDeviceHIKVisionGigETriggerActivation(_ttoi(strValue)));
			}
			break;
		case EDeviceParameterHIKVisionGigE_DigitalLine2Mode:
			{
				bReturn = !SetLine2Mode(EDeviceHIKVisionGigELineMode(_ttoi(strValue)));
			}
			break;
		case EDeviceParameterHIKVisionGigE_DeviceGrabCount:
			{
				bReturn = !SetGrabCount(_ttoi(strValue));
			}
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

void CDeviceHIKVisionGigE::UpdateDeviceToParameter()
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
			MVCC_INTVALUE MaxLength;

			if(!MV_CC_GetIntValue(m_pHikDevice, "Width", &MaxLength))
			{
				SetCanvasWidth(MaxLength.nCurValue);
			}
		}
		catch(...) { }

		try
		{
			MVCC_INTVALUE MaxLength;

			if(!MV_CC_GetIntValue(m_pHikDevice, "Height", &MaxLength))
			{
				SetCanvasHeight(MaxLength.nCurValue);
			}
		}
		catch(...) { }

		try
		{
			nParam = 0;
			if(GetCommandInt32("DeviceStreamChannelPacketSize", &nParam))
				SetDeviceStreamChannelPackSize(nParam);
		}
		catch(...) { }

		try
		{
			nParam = 0;
			if(GetCommandInt32("OffsetX", &nParam))
				SetOffsetX(nParam);
		}
		catch(...) { }

		try
		{
			nParam = 0;
			if(GetCommandInt32("OffsetY", &nParam))
				SetOffsetY(nParam);
		}
		catch(...) { }

		try
		{
			bParam = false;
			if(GetCommandBool("ReverseX", &bParam))
				SetReverseX(bParam);
		}
		catch(...) { }

		try
		{
			bParam = false;
			if(GetCommandBool("ReverseY", &bParam))
				SetReverseY(bParam);
		}
		catch(...) { }

		try
		{
			MVCC_ENUMVALUE eValue;
			if(!MV_CC_GetEnumValue(m_pHikDevice, "AcquisitionMode", &eValue))
			{

				EDeviceHIKVisionAcquisitionMode eMode = EDeviceHIKVisionAcquisitionMode_eSingleFrame;
				switch(eValue.nCurValue)
				{
				case 0:
					eMode = EDeviceHIKVisionAcquisitionMode_eSingleFrame;
					break;
				case 2:
					eMode = EDeviceHIKVisionAcquisitionMode_eContinuous;
					break;
				}
				SetAcquisitionMode(eMode);
			}
		}
		catch(...) { }

		try
		{
			nParam = 0;
			if(!GetCommandInt32("AcquisitionBurstFrameCount", &nParam))
			{
				SetAcquisitionBurstFrameCount(nParam);
			}

		}
		catch(...) { }

		try
		{
			bParam = false;
			if(GetCommandBool("AcquisitionFrameRateEnable", &bParam))
				SetAcquisitionFrameRateControlEnable(bParam);
		}
		catch(...) { }

		try
		{
			MVCC_FLOATVALUE eValue;
			if(!MV_CC_GetFloatValue(m_pHikDevice, "AcquisitionFrameRate", &eValue))
				SetAcquisitionFrameRate(eValue.fCurValue);
		}
		catch(...) { }

		try
		{
			MVCC_ENUMVALUE eValue;
			EDeviceHIKVisionGigEAcquisitionTriggerSelector eParam;
			if(!MV_CC_GetEnumValue(m_pHikDevice, "TriggerSelector", &eValue))
			{
				if(eValue.nCurValue == 6)
				{
					eParam = EDeviceHIKVisionGigEAcquisitionTriggerSelector_EFrameBurstStart;
					SetAcquisitionTriggerSelector(eParam);
				}
			}
		}
		catch(...) { }

		try
		{
			MVCC_ENUMVALUE eValue;
			if(!MV_CC_GetEnumValue(m_pHikDevice, "TriggerMode", &eValue))
				SetAcquisitionTriggerMode((EDeviceHIKVisionGigESwitch)eValue.nCurValue);
		}
		catch(...) { }


		try
		{
			MVCC_ENUMVALUE eValue;
			if(!MV_CC_GetEnumValue(m_pHikDevice, "TriggerActivation", &eValue))
				SetAcquisitionTriggerActivation(EDeviceHIKVisionGigEAcquisitionTriggerActivation(eValue.nCurValue));
		}
		catch(...) { }

		try
		{
			MVCC_ENUMVALUE eValue;
			EDeviceHIKVisionGigEAcquisitionTriggerSource eTriggerSource = EDeviceHIKVisionGigETriggerSource_SoftWare;
			if(!MV_CC_GetEnumValue(m_pHikDevice, "TriggerSource", &eValue))
			{
				switch(eValue.nCurValue)
				{
				case 7:
					eTriggerSource = EDeviceHIKVisionGigETriggerSource_SoftWare;
					break;
				case 0:
					eTriggerSource = EDeviceHIKVisionGigETriggerSource_Line0;
					break;
				case 2:
					eTriggerSource = EDeviceHIKVisionGigETriggerSource_Line2;
					break;
				case 4:
					eTriggerSource = EDeviceHIKVisionGigETriggerSource_Counter0;
					break;
				default:
					break;
				}

				SetAcquisitionTriggerSource(eTriggerSource);
			}
		}
		catch(...) { }

		try
		{
			dblParam = 0.0;
			if(GetCommandFloat("ExposureTime", &dblParam))
				SetAcquisitionExposureTime(dblParam);
		}
		catch(...) { }

		try
		{
			dblParam = 0.0;
			if(GetCommandFloat("TriggerDelay", &dblParam))
				SetAcquisitionTriggerDelay(dblParam);
		}
		catch(...) { }

		try
		{
			MVCC_ENUMVALUE eValue;
			if(!MV_CC_GetEnumValue(m_pHikDevice, "ExposureMode", &eValue))
				SetAcquisitionExposureMode((EDeviceHIKVisionGigEAcquisitionExposureMode)eValue.nCurValue);
		}
		catch(...) { }

		try
		{
			bParam = false;
			if(GetCommandBool("BlackLevelEnable", &bParam))
				SetBlackLevelEnable(bParam);
		}
		catch(...) { }

		try
		{
			nParam = 0;
			if(GetCommandInt32("BlackLevel", &nParam))
				SetBlackLevel(nParam);
		}
		catch(...) { }

		try
		{
			bParam = false;
			if(GetCommandBool("DigitalShiftEnable", &bParam))
				SetDigitalShiftEnable(bParam);
		}
		catch(...) { }

		try
		{
			dblParam = 0.0;
			if(GetCommandFloat("DigitalShift", &dblParam))
				SetDigitalShift(dblParam);
		}
		catch(...) { }

		try
		{
			MVCC_ENUMVALUE eValue;
			if(!MV_CC_GetEnumValue(m_pHikDevice, "LineSelector", &eValue))
			{
				EDeviceHIKVisionGigELineSelect eLineSel = (EDeviceHIKVisionGigELineSelect)eValue.nCurValue;
				SetLineSelector(eLineSel);

				switch(eLineSel)
				{
				case EDeviceHIKVisionGigELineSelect_eLine0:
					{
						try
						{
							nParam = 0;
							if(GetCommandInt32("LineDebouncerTime", &nParam))
								SetLine0DebouncerTime(nParam);
						}
						catch(...) { }
					}
					break;
				case EDeviceHIKVisionGigELineSelect_eLine1:
					{
						try
						{
							bParam = false;
							if(SetCommandBool("LineInverter", &bParam))
								SetLine1Inverter(bParam);
						}
						catch(...) { }

						try
						{
							bParam = false;
							if(SetCommandBool("StrobeEnable", &bParam))
								SetLine1StrobEnable(bParam);
						}
						catch(...) { }

						try
						{
							nParam = 0;
							if(GetCommandInt32("StrobeLineDuration", &nParam))
								SetLine1StrobeLineDuration(nParam);
						}
						catch(...) { }

						try
						{
							nParam = 0;
							if(GetCommandInt32("StrobeLineDelay", &nParam))
								SetLine1StrobeLineDelay(nParam);
						}
						catch(...) { }

						try
						{
							nParam = 0;
							if(GetCommandInt32("StrobeLinePreDelay", &nParam))
								SetLine1StrobeLinePreDelay(nParam);
						}
						catch(...) { }
					}
					break;
				case EDeviceHIKVisionGigELineSelect_eLine2:
					{
						nParam = 0;
						if(GetCommandInt32("Line2Mode", &nParam))
						{
							EDeviceHIKVisionGigELineMode eLineMode = (EDeviceHIKVisionGigELineMode)nParam;

							if(eLineMode == EDeviceHIKVisionGigELineMode_Input)
							{
								try
								{
									nParam = 0;
									if(GetCommandInt32("LineDebouncerTime", &nParam))
										SetLine2DebouncerTime(nParam);
								}
								catch(...) { }
							}
							else if(eLineMode == EDeviceHIKVisionGigELineMode_Strob)
							{
								try
								{
									bParam = false;
									if(SetCommandBool("LineInverter", &bParam))
										SetLine2Inverter(bParam);
								}
								catch(...) { }

								try
								{
									bParam = false;
									if(SetCommandBool("StrobeEnable", &bParam))
										SetLine2StrobEnable(bParam);
								}
								catch(...) { }

								try
								{
									nParam = 0;
									if(GetCommandInt32("StrobeLineDuration", &nParam))
										SetLine2StrobeLineDuration(nParam);
								}
								catch(...) { }

								try
								{
									nParam = 0;
									if(GetCommandInt32("StrobeLineDelay", &nParam))
										SetLine2StrobeLineDelay(nParam);
								}
								catch(...) { }

								try
								{
									nParam = 0;
									if(GetCommandInt32("StrobeLinePreDelay", &nParam))
										SetLine2StrobeLinePreDelay(nParam);
								}
								catch(...) { }
							}
						}
					}
					break;
				default:
					break;
				}
			}
		}
		catch(...) { }

		try
		{
			MVCC_ENUMVALUE eNumValue;
			
			if(!MV_CC_GetEnumValue(m_pHikDevice, "GainAuto", &eNumValue))
			{
				//eNumValue.nCurValue;
			}
				
		}
		catch(...) { }

		try
		{
			MVCC_FLOATVALUE eFloatValue;

			if(!MV_CC_GetFloatValue(m_pHikDevice, "Gain", &eFloatValue))
				SetAnalogGaindB(eFloatValue.fCurValue);
		}
		catch(...) { }

		try
		{
			MVCC_ENUMVALUE eNumValue;

			if(!MV_CC_GetEnumValue(m_pHikDevice, "PixelFormat", &eNumValue))
			{
				EDeviceHIKVisionGigEPixelFormat ePixelFormat = EDeviceHIKVisionGigEPixelFormat_Mono8;

				switch(eNumValue.nCurValue)
				{
				case EImagePixelFormat::eMono8:
					ePixelFormat = EDeviceHIKVisionGigEPixelFormat_Mono8;
					break;
				case EImagePixelFormat::eMono10:
					ePixelFormat = EDeviceHIKVisionGigEPixelFormat_Mono10;
					break;
				case EImagePixelFormat::eMono10Packed:
					ePixelFormat = EDeviceHIKVisionGigEPixelFormat_Mono10packed;
					break;
				case EImagePixelFormat::eMono12:
					ePixelFormat = EDeviceHIKVisionGigEPixelFormat_Mono12;
					break;
				case EImagePixelFormat::eMono12Packed:
					ePixelFormat = EDeviceHIKVisionGigEPixelFormat_Mono12packed;
					break;
				case EImagePixelFormat::eRGB8packed:
					ePixelFormat = EDeviceHIKVisionGigEPixelFormat_RGB8packed;
					break;
				case EImagePixelFormat::eYUV422_YUYV_Packed:
					ePixelFormat = EDeviceHIKVisionGigEPixelFormat_YUV422_YUYV_Packed;
					break;
				case EImagePixelFormat::eYUV422Packed:
					ePixelFormat = EDeviceHIKVisionGigEPixelFormat_YUV422Packed;
					break;
				case EImagePixelFormat::eBayerRG8:
					ePixelFormat = EDeviceHIKVisionGigEPixelFormat_BayerRG8;
					break;
				case EImagePixelFormat::eBayerRG10:
					ePixelFormat = EDeviceHIKVisionGigEPixelFormat_BayerRG10;
					break;
				case EImagePixelFormat::eBayerRG10Packed:
					ePixelFormat = EDeviceHIKVisionGigEPixelFormat_BayerRG10Packed;
					break;
				case EImagePixelFormat::eBayerRG12:
					ePixelFormat = EDeviceHIKVisionGigEPixelFormat_BayerRG12;
					break;
				case EImagePixelFormat::eBayerRG12Packed:
					ePixelFormat = EDeviceHIKVisionGigEPixelFormat_BayerRG12Packed;
					break;
				default:
					break;
				}
				SetImagePixelFormat(ePixelFormat);
			}

		}
		catch (...)	{ }

	} 
	while(false);
}

bool CDeviceHIKVisionGigE::DoesModuleExist()
{
	bool bReturn = false;

	CString strModuleName;

	do
	{
		EDeviceLibraryStatus eStatus = EDeviceLibraryStatus_NotFound;

		if(1)
		{
			strModuleName.Format(_T("MvCameraControl.dll"));

			eStatus = CLibraryManager::GetFoundLibrary(strModuleName);

			if(!eStatus)
			{
				HMODULE hModule = LoadLibraryEx(strModuleName, nullptr, DONT_RESOLVE_DLL_REFERENCES);

				if(!hModule)
				{
					CLibraryManager::SetFoundLibrary(strModuleName, false);
				}
				else
				{
					FreeLibrary(hModule);
					CLibraryManager::SetFoundLibrary(strModuleName, true);

					bReturn = true;
				}
			}
			else
				bReturn = eStatus == EDeviceLibraryStatus_FoundTrue ? true : false;
		}

		if(1)
		{
			bool bPaserRes = false;
			strModuleName.Format(_T("GCBase_MD_VC120_v3_0.dll"));

			eStatus = CLibraryManager::GetFoundLibrary(strModuleName);

			if(!eStatus)
			{
				HMODULE hModule = LoadLibraryEx(strModuleName, nullptr, DONT_RESOLVE_DLL_REFERENCES);

				if(!hModule)
				{
					CLibraryManager::SetFoundLibrary(strModuleName, false);
				}
				else
				{
					FreeLibrary(hModule);

					CLibraryManager::SetFoundLibrary(strModuleName, true);
					bPaserRes |= true;
				}
			}
			else
				bPaserRes |= eStatus == EDeviceLibraryStatus_FoundTrue ? true : false;

			if(!bPaserRes)
			{
				strModuleName.Format(_T("GCBase_MD_VC120_v3_0_MVS_v3_1_0.dll"));

				eStatus = CLibraryManager::GetFoundLibrary(strModuleName);

				if(!eStatus)
				{
					HMODULE hModule = LoadLibraryEx(strModuleName, nullptr, DONT_RESOLVE_DLL_REFERENCES);
					if(!hModule)
					{
						CLibraryManager::SetFoundLibrary(strModuleName, false);
					}
					else
					{
						CLibraryManager::SetFoundLibrary(strModuleName, true);
						FreeLibrary(hModule);
						bPaserRes |= true;
					}
				}
				else
					bPaserRes |= eStatus == EDeviceLibraryStatus_FoundTrue ? true : false;
			}

			bReturn &= bPaserRes;
		}
		
		if(1)
		{
			bool bPaserRes = false;

			strModuleName.Format(_T("GenApi_MD_VC120_v3_0.dll"));

			eStatus = CLibraryManager::GetFoundLibrary(strModuleName);

			if(!eStatus)
			{
				HMODULE hModule = LoadLibraryEx(strModuleName, nullptr, DONT_RESOLVE_DLL_REFERENCES);

				if(!hModule)
				{
					CLibraryManager::SetFoundLibrary(strModuleName, false);
				}
				else
				{
					FreeLibrary(hModule);

					CLibraryManager::SetFoundLibrary(strModuleName, true);
					bPaserRes |= true;
				}
			}
			else
				bPaserRes |= eStatus == EDeviceLibraryStatus_FoundTrue ? true : false;

			if(!bPaserRes)
			{
				strModuleName.Format(_T("GenApi_MD_VC120_v3_0_MVS_v3_1_0.dll"));

				eStatus = CLibraryManager::GetFoundLibrary(strModuleName);

				if(!eStatus)
				{
					HMODULE hModule = LoadLibraryEx(strModuleName, nullptr, DONT_RESOLVE_DLL_REFERENCES);
					if(!hModule)
					{
						CLibraryManager::SetFoundLibrary(strModuleName, false);
					}
					else
					{
						CLibraryManager::SetFoundLibrary(strModuleName, true);
						FreeLibrary(hModule);
						bPaserRes |= true;
					}
				}
				else
					bPaserRes |= eStatus == EDeviceLibraryStatus_FoundTrue ? true : false;
			}

			bReturn &= bPaserRes;
		}
		
		if(1)
		{
			bool bPaserRes = false;
			strModuleName.Format(_T("Log_MD_VC120_v3_0.dll"));

			eStatus = CLibraryManager::GetFoundLibrary(strModuleName);

			if(!eStatus)
			{
				HMODULE hModule = LoadLibraryEx(strModuleName, nullptr, DONT_RESOLVE_DLL_REFERENCES);

				if(!hModule)
				{
					CLibraryManager::SetFoundLibrary(strModuleName, false);
				}
				else
				{
					FreeLibrary(hModule);

					CLibraryManager::SetFoundLibrary(strModuleName, true);
					bPaserRes |= true;
				}
			}
			else
				bPaserRes |= eStatus == EDeviceLibraryStatus_FoundTrue ? true : false;

			if(!bPaserRes)
			{
				strModuleName.Format(_T("Log_MD_VC120_v3_0_MVS_v3_1_0.dll"));

				eStatus = CLibraryManager::GetFoundLibrary(strModuleName);

				if(!eStatus)
				{
					HMODULE hModule = LoadLibraryEx(strModuleName, nullptr, DONT_RESOLVE_DLL_REFERENCES);
					if(!hModule)
					{
						CLibraryManager::SetFoundLibrary(strModuleName, false);
					}
					else
					{
						CLibraryManager::SetFoundLibrary(strModuleName, true);
						FreeLibrary(hModule);
						bPaserRes |= true;
					}
				}
				else
					bPaserRes |= eStatus == EDeviceLibraryStatus_FoundTrue ? true : false;
			}

			bReturn &= bPaserRes;
		}
		

		if(1)
		{
			bool bPaserRes = false;

			strModuleName.Format(_T("MathParser_MD_VC120_v3_0.dll"));

			eStatus = CLibraryManager::GetFoundLibrary(strModuleName);
			if(!eStatus)
			{
				HMODULE hModule = LoadLibraryEx(strModuleName, nullptr, DONT_RESOLVE_DLL_REFERENCES);
				if(!hModule)
				{
					CLibraryManager::SetFoundLibrary(strModuleName, false);
				}
				else
				{
					CLibraryManager::SetFoundLibrary(strModuleName, true);
					FreeLibrary(hModule);
					bPaserRes |= true;
				}
			}
			else
				bPaserRes |= eStatus == EDeviceLibraryStatus_FoundTrue ? true : false;

			if(!bPaserRes)
			{
				strModuleName.Format(_T("MathParser_MD_VC120_v3_0_MVS_v3_1_0.dll"));

				eStatus = CLibraryManager::GetFoundLibrary(strModuleName);
				if(!eStatus)
				{
					HMODULE hModule = LoadLibraryEx(strModuleName, nullptr, DONT_RESOLVE_DLL_REFERENCES);
					if(!hModule)
					{
						CLibraryManager::SetFoundLibrary(strModuleName, false);
					}
					else
					{
						CLibraryManager::SetFoundLibrary(strModuleName, true);
						FreeLibrary(hModule);
						bPaserRes |= true;
					}
				}
				else
					bPaserRes |= eStatus == EDeviceLibraryStatus_FoundTrue ? true : false;
			}

			bReturn &= bPaserRes;
		}		

		if(1)
		{
			strModuleName.Format(_T("MvRender.dll"));

			eStatus = CLibraryManager::GetFoundLibrary(strModuleName);

			if(!eStatus)
			{
				HMODULE hModule = LoadLibraryEx(strModuleName, nullptr, DONT_RESOLVE_DLL_REFERENCES);
				if(!hModule)
				{
					CLibraryManager::SetFoundLibrary(strModuleName, false);

					bReturn &= false;
				}
				else
				{
					FreeLibrary(hModule);

					CLibraryManager::SetFoundLibrary(strModuleName, true);

					bReturn &= true;
				}
			}
			else
				bReturn &= eStatus == EDeviceLibraryStatus_FoundTrue ? true : false;
		}
		
		if(1)
		{
			bool bPaserRes = false;
			strModuleName.Format(_T("NodeMapData_MD_VC120_v3_0.dll"));

			eStatus = CLibraryManager::GetFoundLibrary(strModuleName);

			if(!eStatus)
			{
				HMODULE hModule = LoadLibraryEx(strModuleName, nullptr, DONT_RESOLVE_DLL_REFERENCES);

				if(!hModule)
				{
					CLibraryManager::SetFoundLibrary(strModuleName, false);
				}
				else
				{
					FreeLibrary(hModule);

					CLibraryManager::SetFoundLibrary(strModuleName, true);
					bPaserRes |= true;
				}
			}
			else
				bPaserRes |= eStatus == EDeviceLibraryStatus_FoundTrue ? true : false;

			if(!bPaserRes)
			{
				strModuleName.Format(_T("NodeMapData_MD_VC120_v3_0_MVS_v3_1_0.dll"));

				eStatus = CLibraryManager::GetFoundLibrary(strModuleName);

				if(!eStatus)
				{
					HMODULE hModule = LoadLibraryEx(strModuleName, nullptr, DONT_RESOLVE_DLL_REFERENCES);
					if(!hModule)
					{
						CLibraryManager::SetFoundLibrary(strModuleName, false);
					}
					else
					{
						CLibraryManager::SetFoundLibrary(strModuleName, true);
						FreeLibrary(hModule);
						bPaserRes |= true;
					}
				}
				else
					bPaserRes |= eStatus == EDeviceLibraryStatus_FoundTrue ? true : false;
			}

			bReturn &= bPaserRes;
		}
		
		if(1)
		{
			bool bPaserRes = false;
			strModuleName.Format(_T("XmlParser_MD_VC120_v3_0.dll"));

			eStatus = CLibraryManager::GetFoundLibrary(strModuleName);

			if(!eStatus)
			{
				HMODULE hModule = LoadLibraryEx(strModuleName, nullptr, DONT_RESOLVE_DLL_REFERENCES);

				if(!hModule)
				{
					CLibraryManager::SetFoundLibrary(strModuleName, false);
				}
				else
				{
					FreeLibrary(hModule);

					CLibraryManager::SetFoundLibrary(strModuleName, true);
					bPaserRes |= true;
				}
			}
			else
				bPaserRes |= eStatus == EDeviceLibraryStatus_FoundTrue ? true : false;

			if(!bPaserRes)
			{
				strModuleName.Format(_T("XmlParser_MD_VC120_v3_0_MVS_v3_1_0.dll"));

				eStatus = CLibraryManager::GetFoundLibrary(strModuleName);

				if(!eStatus)
				{
					HMODULE hModule = LoadLibraryEx(strModuleName, nullptr, DONT_RESOLVE_DLL_REFERENCES);
					if(!hModule)
					{
						CLibraryManager::SetFoundLibrary(strModuleName, false);
					}
					else
					{
						CLibraryManager::SetFoundLibrary(strModuleName, true);
						FreeLibrary(hModule);
						bPaserRes |= true;
					}
				}
				else
					bPaserRes |= eStatus == EDeviceLibraryStatus_FoundTrue ? true : false;
			}

			bReturn &= bPaserRes;
		}
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, CMultiLanguageManager::GetString(ELanguageParameter__s_d_s_toload_s), GetClassNameStr(), GetObjectID(), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), strModuleName);

	return bReturn;
}

bool CDeviceHIKVisionGigE::GetCommandInt32(const char* pCommnadString, int32_t* pGetValue)
{
	bool bReturn = false;

	do
	{
		if(!m_pHikDevice)
			break;

		if(!pCommnadString)
			break;
		MVCC_INTVALUE MVCCtemp;


		if(MV_CC_GetIntValue(m_pHikDevice, pCommnadString, &MVCCtemp) != S_OK)
			break;

		*pGetValue = MVCCtemp.nCurValue;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceHIKVisionGigE::SetCommandInt32(const char* pCommnadString, int32_t nSetValue)
{
	bool bReturn = false;

	do
	{
		if(!m_pHikDevice)
			break;

		if(!pCommnadString)
			break;

		if(MV_CC_SetIntValue(m_pHikDevice, pCommnadString, nSetValue) != MV_OK)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceHIKVisionGigE::GetCommandInt64(const char* pCommnadString, int64_t* pGetValue)
{
	bool bReturn = false;

	do
	{
		if(!m_pHikDevice)
			break;

		if(!pCommnadString)
			break;

		MVCC_INTVALUE_EX MvccTemp;

		if(MV_CC_GetIntValueEx(m_pHikDevice, pCommnadString, &MvccTemp) != MV_OK)
			break;

		*pGetValue = MvccTemp.nCurValue;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceHIKVisionGigE::SetCommandInt64(const char* pCommnadString, int64_t nSetValue)
{
	bool bReturn = false;

	do
	{
		if(!m_pHikDevice)
			break;

		if(!pCommnadString)
			break;

		if(MV_CC_SetIntValueEx(m_pHikDevice, pCommnadString, nSetValue) != MV_OK)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceHIKVisionGigE::GetCommandFloat(const char* pCommnadString, double* pGetValue)
{
	bool bReturn = false;

	do
	{
		if(!m_pHikDevice)
			break;

		if(!pCommnadString)
			break;

		MVCC_FLOATVALUE MvccTemp;
		if(MV_CC_GetFloatValue(m_pHikDevice, pCommnadString, &MvccTemp) != MV_OK)
			break;

		*pGetValue = MvccTemp.fCurValue;
		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceHIKVisionGigE::SetCommandFloat(const char* pCommnadString, double dblSetValue)
{
	bool bReturn = false;

	do
	{
		if(!m_pHikDevice)
			break;

		if(!pCommnadString)
			break;


		if(MV_CC_SetFloatValue(m_pHikDevice, pCommnadString, dblSetValue) != MV_OK)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceHIKVisionGigE::GetCommandBool(const char* pCommnadString, bool* pGetValue)
{
	bool bReturn = false;

	do
	{
		if(!m_pHikDevice)
			break;

		if(!pCommnadString)
			break;

		if(MV_CC_GetBoolValue(m_pHikDevice, pCommnadString, pGetValue) != MV_OK)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceHIKVisionGigE::SetCommandBool(const char* pCommnadString, bool bSetValue)
{
	bool bReturn = false;

	do
	{
		if(!m_pHikDevice)
			break;

		if(!pCommnadString)
			break;


		if(MV_CC_SetBoolValue(m_pHikDevice, pCommnadString, bSetValue) != MV_OK)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceHIKVisionGigE::GetCommandString(const char* pCommnadString, const char* pGetValue)
{
	bool bReturn = false;

	do
	{
		if(!m_pHikDevice)
			break;

		if(!pCommnadString)
			break;

		MVCC_STRINGVALUE MvccTemp;

		if(MV_CC_GetStringValue(m_pHikDevice, pCommnadString, &MvccTemp) != MV_OK)
			break;

		pGetValue = MvccTemp.chCurValue;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceHIKVisionGigE::SetCommandString(const char* pCommnadString, const char* pGetValue)
{
	bool bReturn = false;

	do
	{
		if(!m_pHikDevice)
			break;

		if(!pCommnadString)
			break;

		if(MV_CC_SetStringValue(m_pHikDevice, pCommnadString, pGetValue) != MV_OK)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

#endif