#include "stdafx.h"

#include "DeviceAlliedGigE.h"

#ifdef USING_DEVICE

#include "../RavidCore/RavidRect.h"
#include "../RavidCore/RavidImage.h"
#include "../RavidCore/RavidDatabase.h"
#include "../RavidCore/RavidRecordset.h"
#include "../RavidCore/PerformanceCounter.h"
#include "../RavidCore/OperatingSystemInfo.h"

#include "../RavidFramework/LogManager.h" 
#include "../RavidFramework/DeviceManager.h"
#include "../RavidFramework/LibraryManager.h"
#include "../RavidFramework/FrameworkManager.h"
#include "../RavidFramework/EventHandlerManager.h"
#include "../RavidFramework/MultiLanguageManager.h"

#include "../RavidFramework/RavidTreeView.h"
#include "../RavidFramework/RavidImageView.h"

#include "../Libraries/Includes/AlliedGigE/PvApi.h"
#include "../Libraries/Includes/AlliedGigE/PvRegIo.h"
#include "../Libraries/Includes/AlliedGigE/ImageLib.h"

// pvapi.dll
#pragma comment(lib, COMMONLIB_PREFIX "AlliedGigE/PvAPI.lib")
#pragma comment(lib, COMMONLIB_PREFIX "AlliedGigE/ImageLib.lib")

#define MAX_CAMERA_SIZE 20
#define FRAMESCOUNT 3

#define ULONG_PADDING(x) (((x+3) & ~3) - x)

using namespace Ravid;
using namespace Ravid::Device;
using namespace Ravid::Database;
using namespace Ravid::Framework;
using namespace Ravid::Algorithms;
using namespace Ravid::Miscellaneous;

IMPLEMENT_DYNAMIC(CDeviceAlliedGigE, CDeviceCamera)

BEGIN_MESSAGE_MAP(CDeviceAlliedGigE, CDeviceCamera)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

inline void YUV2RGB(int y, int u, int v, int& r, int& g, int& b)
{
   // u and v are +-0.5
	u -= 128;
	v -= 128;

	// Conversion (clamped to 0..255)
	r = min(max(0, (int)(y + 1.370705 * (float)v)), 255);
	g = min(max(0, (int)(y - 0.698001 * (float)v - 0.337633 * (float)u)), 255);
	b = min(max(0, (int)(y + 1.732446 * (float)u)), 255);
}

typedef struct
{

	unsigned char LByte;
	unsigned char MByte;
	unsigned char UByte;

} Packed12BitsPixel_t;

static LPCTSTR g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_Count] =
{
	_T("DeviceID"),
	_T("Device Find Waitting time"),
	_T("Grab Waitting time"),
	_T("Initialize Type"),
	_T("Width"),
	_T("Height"),
	_T("RegionX"),
	_T("RegionY"),
	_T("BinningX"),
	_T("BinningY"),
	_T("DecimationHorizontal"),
	_T("DecimationVertical"),
	_T("PixelFormat"),
	_T("PacketSize"),

	_T("AcquisitionInfo"),						//none
	_T("Acquisition"),						//none
	_T("AcqEndTriggerEvent"),
	_T("AcqEndTriggerMode"),
	_T("AcqRecTriggerEvent"),
	_T("AcqRecTriggerMode"),
	_T("AcqStartTriggerEvent"),
	_T("AcqStartTriggerMode"),
	_T("FrameRate"),
	_T("FrameStartTriggerDelay"),
	_T("FrameStartTriggerEvent"),
	_T("FrameStartTriggerMode"),
	_T("FrameStartTriggerOverlap"),

	_T("AcquisitionFrameCount"),
	_T("AcquisitionMode"),
	_T("RecorderPreEventCount"),

	_T("ControlInfo"),							//none
	_T("Color"),							//none
	_T("ColorTransformationMode"),
	_T("ColorTransformationValueBB"),
	_T("ColorTransformationValueBG"),
	_T("ColorTransformationValueBR"),
	_T("ColorTransformationValueGB"),
	_T("ColorTransformationValueGG"),
	_T("ColorTransformationValueGR"),
	_T("ColorTransformationValueRB"),
	_T("ColorTransformationValueRG"),
	_T("ColorTransformationValueRR"),
	_T("DSP"),								//none
	_T("DSPSubregionBottom"),
	_T("DSPSubregionLeft"),
	_T("DSPSubregionRight"),
	_T("DSPSubregionTop"),
	_T("Exposure"),						//none
	_T("ExposureMode"),
	_T("ExposureValue"),
	_T("ExposureAutoAdjustTol"),
	_T("ExposureAutoAlg"),
	_T("ExposureAutoMax"),
	_T("ExposureAutoMin"),
	_T("ExposureAutoOutliers"),
	_T("ExposureAutoRate"),
	_T("ExposureAutoTarget"),
	_T("Gain"),							//none
	_T("GainMode"),
	_T("GainValue"),
	_T("GainAutoAdjustTol"),
	_T("GainAutoMax"),
	_T("GainAutoMin"),
	_T("GainAutoOutliers"),
	_T("GainAutoRate"),
	_T("GainAutoTarget"),
	_T("Iris"),							//none
	_T("IrisAutoTarget"),
	_T("IrisMode"),
	_T("IrisVideoLevelMax"),
	_T("IrisVideoLevelMin"),
	_T("LUTControl"),						//none
	_T("LUTEnable"),
	_T("LUTIndex"),
	_T("LUTMode"),
	_T("LUTSelector"),
	_T("LUTValue"),
	_T("ETC"),								//none
	_T("EdgeFilter"),
	_T("Gamma"),
	_T("Hue"),
	_T("OffsetValue"),
	_T("Saturation"),
	_T("Whitebalance"),					//none
	_T("WhitebalMode"),
	_T("WhitebalValueBlue"),
	_T("WhitebalValueRed"),
	_T("WhitebalAutoAdjustTol"),
	_T("WhitebalAutoRate"),
	_T("GigEInfo"),							//none
	_T("BandwidthCtrlMode"),
	_T("HeartbeatInterval"),
	_T("HeartbeatTimeout"),
	_T("GVSP"),							//none
	_T("GvcpRetries"),
	_T("GvspLookbackWindow"),
	_T("GvspResendPercent"),
	_T("GvspRetries"),
	_T("GvspSocketBuffersCount"),
	_T("GvspTimeout"),
	_T("IOInfo"),								//none
	_T("Strobe"),							//none
	_T("Strobe1ControlledDuration"),
	_T("Strobe1Delay"),
	_T("Strobe1Duration"),
	_T("Strobe1Mode"),
	_T("Sync"),							//none
	_T("SyncIn1GlitchFilter"),
	_T("SyncIn2GlitchFilter"),
	_T("SyncOut1Invert"),
	_T("SyncOut1Mode"),
	_T("SyncOut2Invert"),
	_T("SyncOut2Mode"),
	_T("SyncOutGpoLevels"),

	_T("StreamBytesPerSecond"), //114
	_T("StreamFrameRateConstrain"),
	_T("StreamHoldEnable"),
};

static LPCTSTR g_lpszAlliedGigEInitializeType[EDeviceAlliedGigEInitializeType_Count] =
{
	_T("Only param"),
	_T("Not param"),
};

static LPCTSTR g_lpszAlliedGigEPixelFormat[EDeviceAlliedGigEPixelFormat_Count] =
{
	_T("Mono8"),
	_T("Mono16"),
	_T("Bayer8"),
	_T("Bayer16"),
	_T("Rgb24"),
	_T("Yuv411"),
	_T("Yuv422"),
	_T("Yuv444"),
	_T("Bgr24"),
	_T("Bayer12Packed"),	
};

static LPCTSTR g_lpszAlliedGigETriggerEvent[EDeviceAlliedGigETriggerEvent_Count] =
{
	_T("EdgeRising"),
	_T("EdgeFalling"),
	_T("EdgeAny"),
	_T("LevelHigh"),
	_T("LevelLow"),
};

static LPCTSTR g_lpszAlliedGigETriggerMode[EDeviceAlliedGigETriggerMode_Count] =
{
	_T("Disabled"),
	_T("SyncIn1"),
	_T("SyncIn2"),
};

static LPCTSTR g_lpszAlliedGigEFrameMode[EDeviceAlliedGigEFrameMode_Count] =
{
	_T("Freerun"),
	_T("SyncIn1"),
	_T("SyncIn2"),
	_T("FixedRate"),
	_T("Software"),
};

static LPCTSTR g_lpszAlliedGigETriggerOverlap[EDeviceAlliedGigETriggerOverlap_Count] =
{
	_T("Off"),
	_T("PreviousFrame"),
};

static LPCTSTR g_lpszAlliedGigEAcquisitionMode[EDeviceAlliedGigEAcquisitionMode_Count] =
{
	_T("Continuous"),
	_T("SingleFrame"),
	_T("MultiFrame"),
	_T("Recorder"),
};

static LPCTSTR g_lpszAlliedGigEColorTransformationMode[EDeviceAlliedGigEColorTransformationMode_Count] =
{
	_T("Off"),
	_T("Manual"),
	_T("Temp6500K"),
};

static LPCTSTR g_lpszAlliedGigEExposureMode[EDeviceAlliedGigEExposureMode_Count] =
{
	_T("Manual"),
	_T("AutoOnce"),
	_T("Auto"),
	_T("External"),
};

static LPCTSTR g_lpszAlliedGigEExposureAutoAlg[EDeviceAlliedGigEExposureAutoAlg_Count] =
{
	_T("Mean"),
	_T("FitRange"),
};

static LPCTSTR g_lpszAlliedGigEGainMode[EDeviceAlliedGigEGainMode_Count] =
{
	_T("Manual"),
	_T("AutoOnce"),
	_T("Auto"),
};

static LPCTSTR g_lpszAlliedGigEIrisMode[EDeviceAlliedGigEIrisMode_Count] =
{
	_T("Disabled"),
	_T("Video"),
	_T("VideoOpen"),
	_T("VideoClose"),
};

static LPCTSTR g_lpszAlliedGigELUTMode[EDeviceAlliedGigELUTMode_Count] =
{
	_T("Luminance"),
	_T("Red"),
	_T("Green"),
	_T("Blue"),
};	

static LPCTSTR g_lpszAlliedGigELUTSelector[EDeviceAlliedGigELUTSelector_Count] =
{
	_T("LUT1"),
	_T("LUT2"),
	_T("LUT3"),
};

static LPCTSTR g_lpszAlliedGigEEdgeFilter[EDeviceAlliedGigEEdgeFilter_Count] =
{
	_T("Smooth2"),
	_T("Smooth1"),
	_T("Off"),
	_T("Sharpen1"),
	_T("Sharpen2"),
};

static LPCTSTR g_lpszAlliedGigEWhitebalMode[EDeviceAlliedGigEWhitebalMode_Count] =
{
	_T("Manual"),
	_T("AutoOnce"),
	_T("Auto"),
};

static LPCTSTR g_lpszAlliedGigEBandwidthCtrlMode[EDeviceAlliedGigEBandwidthCtrlMode_Count] =
{
	_T("StreamBytesPerSecond"),
	_T("SCPD"),
	_T("Both"),
};

static LPCTSTR g_lpszAlliedGigEGvspSocketBuffersCount[EDeviceAlliedGigEGvspSocketBuffersCount_Count] =
{
	_T("256"),
	_T("512"),
	_T("1024"),
	_T("2048"),
	_T("4096"),
	_T("8192"),
};

static LPCTSTR g_lpszAlliedGigEStrobeMode[EDeviceAlliedGigEStrobeMode_Count] =
{
	_T("AcquisitionTriggerReady"),
	_T("FrameTriggerReady"),
	_T("FrameTrigger"),
	_T("Exposing"),
	_T("FrameReadout"),
	_T("Acquiring"),
	_T("SyncIn1"),
	_T("SyncIn2"),
};

static LPCTSTR g_lpszAlliedGigESyncOutMode[EDeviceAlliedGigESyncOutMode_Count] =
{
	_T("GPO"),
	_T("AcquisitionTriggerReady"),
	_T("FrameTriggerReady"),
	_T("FrameTrigger"),
	_T("Exposing"),
	_T("FrameReadout"),
	_T("Imaging"),
	_T("Acquiring"),
	_T("SyncIn1"),
	_T("SyncIn2"),
	_T("Strobe1"),
};


static LPCTSTR g_lpszAlliedGigESwitch[EDeviceAlliedGigESwitch_Count] =
{
	_T("Off"),
	_T("On"),
};


CDeviceAlliedGigE::CDeviceAlliedGigE()
{
}

CDeviceAlliedGigE::~CDeviceAlliedGigE()
{
	Terminate();
}

EDeviceInitializeResult CDeviceAlliedGigE::Initialize()
{
	EDeviceInitializeResult eReturn = EDeviceInitializeResult_UnknownError;

	CString strMessage, strStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("Allied GigE"));
		
		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);

		SetStatus(strStatus);

		return EDeviceInitializeResult_NotFoundApiError;
	}

	tPvFrame* pFrames = nullptr; 

	do 
	{
		if(IsInitialized())
		{
			strStatus = CMultiLanguageManager::GetString(ELanguageParameter_Initialized);
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Thedevicehasbeenalreadyinitialized);
			eReturn = EDeviceInitializeResult_AlreadyInitializedError;
			break;
		}
		
		tPvErr tError = tPvErr::ePvErrSuccess;

		bool bNeedInitializeAPI = true;

		size_t szDeviceCount = CDeviceManager::GetDeviceCount();

		for(size_t i = 0; i < szDeviceCount; ++i)
		{
			CDeviceAlliedGigE* pDeviceCamera = dynamic_cast<CDeviceAlliedGigE*>(CDeviceManager::GetDeviceByIndex(typeid(CDeviceAlliedGigE), i));
			if(!pDeviceCamera)
				continue;

			if(!pDeviceCamera->IsInitialized())
				continue;

			bNeedInitializeAPI = false;
			break;
		}

		if(bNeedInitializeAPI)
			tError = PvInitialize();

		if(tError != tPvErr::ePvErrSuccess)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntopenthelibrary);
			eReturn = EDeviceInitializeResult_AlreadyInitializedError;
			break;
		}

		DWORD dwCounter = GetTickCount() + 500;

		while(dwCounter > GetTickCount());

		tPvUint32 nCount = PvCameraCount();
		tPvUint32 nConnected = 0;
		tPvCameraInfoEx list[MAX_CAMERA_SIZE];
		memset(list, 0, sizeof(tPvCameraInfoEx) * MAX_CAMERA_SIZE);

		//regardless if connected > 1, we only set UID of first camera in list
		nCount = PvCameraListEx(list, MAX_CAMERA_SIZE, &nConnected, sizeof(tPvCameraInfoEx));

		if(!nCount)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtofindthedevice);
			eReturn = EDeviceInitializeResult_NotFoundDeviceError;
			break;
		}

		CString strDeviceID = GetDeviceID();

		tPvUint32  nCunnectedID = 0;

		for(tPvUint32 i = 0; i < nCount; ++i)
		{
			CString str;
			str.Format(_T("%s"), CString(list[i].SerialNumber));

			if(strDeviceID.CompareNoCase(str))
				continue;

			nCunnectedID = list[i].UniqueId;
			break;
		}

		if(PvCameraOpen(nCunnectedID, ePvAccessMaster, &m_hDevice) != tPvErr::ePvErrSuccess)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntopenthedevice);
			eReturn = EDeviceInitializeResult_NotFoundDeviceError;
			break;
		}

//		사용 가능한 파라메터 확인 방법
//
// 		tPvUint32 lCount;
// 		tPvAttrListPtr lAttrs;
// 
// 		if(PvAttrList(m_hDevice, &lAttrs, &lCount) != ePvErrSuccess)
// 		{
// 			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Command"));
// 			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
// 			break;
// 		}
// 		typedef struct __stParams
// 		{
// 			CString strCommand;
// 			CString strType;
// 		}STParam;
// 
// 		std::vector<STParam> vctCommand;
// 
// 		for(tPvUint32 i = 0; i < lCount; i++)
// 		{
// 			STParam stData;
// 
// 			tPvAttributeInfo lInfo;
// 
// 			if(PvAttrInfo(m_hDevice, lAttrs[i], &lInfo) == ePvErrSuccess)
// 			{
// 				if(!(lInfo.Flags & ePvFlagWrite) || !(lInfo.Flags & ePvFlagRead))
// 					continue;
// 
// 				stData.strCommand = A2T(lAttrs[i]);
// 
// 				switch(lInfo.Datatype)
// 				{
// 				case ePvDatatypeString:
// 					stData.strType = _T("String");
// 					break;
// 				case ePvDatatypeEnum:
// 					stData.strType = _T("Enum");
// 					break;
// 				case ePvDatatypeUint32:
// 					stData.strType = _T("Uint32");
// 					break;
// 				case ePvDatatypeInt64:
// 					stData.strType = _T("Int64");
// 					break;
// 				case ePvDatatypeFloat32:
// 					stData.strType = _T("Float32");
// 					break;
// 				case ePvDatatypeBoolean:
// 					stData.strType = _T("Bool");
// 					break;
// 				default:
// 					break;
// 				}
// 
// 				if(!stData.strType.GetLength())
// 					continue;
// 
// 				vctCommand.push_back(stData);
//			}
//		}
//
//		FILE* fp = _tfopen(_T("C:\\Allied.csv"), _T("wb"));
//		if(fp)
//		{
//			for(auto iter : vctCommand)
//			{
//				fputs(CStringA(iter.strCommand), fp);
//				fputs(",", fp);
//				fputs(CStringA(iter.strType), fp);
//				fputs("\n", fp);
//			}
//
//			fclose(fp);
//		}

		EDeviceAlliedGigEInitializeType eInitializeType = EDeviceAlliedGigEInitializeType_NotParam;

		if(GetInitializeType(&eInitializeType))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("InitializeType"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}
				
		switch(eInitializeType)
		{
		case EDeviceAlliedGigEInitializeType_NotParam:
			{
				tPvUint32 nDeviceData = 0;
				if(PvAttrUint32Get(m_hDevice, "Width", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_Width, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_Width);
				}

				if(PvAttrUint32Get(m_hDevice, "Height", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_Height, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_Height);
				}

				if(PvAttrUint32Get(m_hDevice, "RegionX", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_RegionX, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_RegionX);
				}

				if(PvAttrUint32Get(m_hDevice, "RegionY", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_RegionY, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_RegionY);
				}

				if(PvAttrUint32Get(m_hDevice, "BinningX", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_BinningX, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_BinningX);
				}

				if(PvAttrUint32Get(m_hDevice, "BinningY", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_BinningY, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_BinningY);
				}

				if(PvAttrUint32Get(m_hDevice, "DecimationHorizontal", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_DecimationHorizontal, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_DecimationHorizontal);
				}

				if(PvAttrUint32Get(m_hDevice, "DecimationVertical", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_DecimationVertical, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_DecimationVertical);
				}

				unsigned long nBufferSize = 256;
				unsigned long nReadSize = 0;

				char pBuffer[256] = { 0, };
				memset(pBuffer, 0, sizeof(char) * 256);
				nReadSize = 0;

				if(PvAttrEnumGet(m_hDevice, "PixelFormat", pBuffer, nBufferSize, &nReadSize) == tPvErr::ePvErrSuccess)
				{
					CString strDeviceData(pBuffer);

					for(int i = 0; i < EDeviceAlliedGigEPixelFormat_Count; ++i)
					{
						if(strDeviceData.CompareNoCase(g_lpszAlliedGigEPixelFormat[i]))
							continue;

						CString strSave;
						strSave.Format(_T("%d"), i);

						SetParamValue(EDeviceParameterAlliedGigE_PixelFormat, strSave);
						SaveSettings(EDeviceParameterAlliedGigE_PixelFormat);
						
						break;
					}
				}

				if(PvAttrUint32Get(m_hDevice, "PacketSize", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_PacketSize, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_PacketSize);
				}

				memset(pBuffer, 0, sizeof(char) * 256);
				nReadSize = 0;

				if(PvAttrEnumGet(m_hDevice, "AcqEndTriggerEvent", pBuffer, nBufferSize, &nReadSize) == tPvErr::ePvErrSuccess)
				{
					CString strDeviceData(pBuffer);

					for(int i = 0; i < EDeviceAlliedGigETriggerEvent_Count; ++i)
					{
						if(strDeviceData.CompareNoCase(g_lpszAlliedGigETriggerEvent[i]))
							continue;

						CString strSave;
						strSave.Format(_T("%d"), i);

						SetParamValue(EDeviceParameterAlliedGigE_AcqEndTriggerEvent, strSave);
						SaveSettings(EDeviceParameterAlliedGigE_AcqEndTriggerEvent);
						
						break;
					}
				}

				memset(pBuffer, 0, sizeof(char) * 256);
				nReadSize = 0;

				if(PvAttrEnumGet(m_hDevice, "AcqEndTriggerMode", pBuffer, nBufferSize, &nReadSize) == tPvErr::ePvErrSuccess)
				{
					CString strDeviceData(pBuffer);

					for(int i = 0; i < EDeviceAlliedGigETriggerMode_Count; ++i)
					{
						if(strDeviceData.CompareNoCase(g_lpszAlliedGigETriggerMode[i]))
							continue;

						CString strSave;
						strSave.Format(_T("%d"), i);

						SetParamValue(EDeviceParameterAlliedGigE_AcqEndTriggerMode, strSave);
						SaveSettings(EDeviceParameterAlliedGigE_AcqEndTriggerMode);

						break;
					}
				}

				memset(pBuffer, 0, sizeof(char) * 256);
				nReadSize = 0;

				if(PvAttrEnumGet(m_hDevice, "AcqRecTriggerEvent", pBuffer, nBufferSize, &nReadSize) == tPvErr::ePvErrSuccess)
				{
					CString strDeviceData(pBuffer);

					for(int i = 0; i < EDeviceAlliedGigETriggerEvent_Count; ++i)
					{
						if(strDeviceData.CompareNoCase(g_lpszAlliedGigETriggerEvent[i]))
							continue;

						CString strSave;
						strSave.Format(_T("%d"), i);

						SetParamValue(EDeviceParameterAlliedGigE_AcqRecTriggerEvent, strSave);
						SaveSettings(EDeviceParameterAlliedGigE_AcqRecTriggerEvent);
						
						break;
					}
				}

				memset(pBuffer, 0, sizeof(char) * 256);
				nReadSize = 0;

				if(PvAttrEnumGet(m_hDevice, "AcqRecTriggerMode", pBuffer, nBufferSize, &nReadSize) == tPvErr::ePvErrSuccess)
				{
					CString strDeviceData(pBuffer);

					for(int i = 0; i < EDeviceAlliedGigETriggerMode_Count; ++i)
					{
						if(strDeviceData.CompareNoCase(g_lpszAlliedGigETriggerMode[i]))
							continue;

						CString strSave;
						strSave.Format(_T("%d"), i);

						SetParamValue(EDeviceParameterAlliedGigE_AcqRecTriggerMode, strSave);
						SaveSettings(EDeviceParameterAlliedGigE_AcqRecTriggerMode);

						break;
					}
				}

				memset(pBuffer, 0, sizeof(char) * 256);
				nReadSize = 0;

				if(PvAttrEnumGet(m_hDevice, "AcqStartTriggerEvent", pBuffer, nBufferSize, &nReadSize) == tPvErr::ePvErrSuccess)
				{
					CString strDeviceData(pBuffer);

					for(int i = 0; i < EDeviceAlliedGigETriggerEvent_Count; ++i)
					{
						if(strDeviceData.CompareNoCase(g_lpszAlliedGigETriggerEvent[i]))
							continue;

						CString strSave;
						strSave.Format(_T("%d"), i);

						SetParamValue(EDeviceParameterAlliedGigE_AcqStartTriggerEvent, strSave);
						SaveSettings(EDeviceParameterAlliedGigE_AcqStartTriggerEvent);

						break;
					}
				}

				memset(pBuffer, 0, sizeof(char) * 256);
				nReadSize = 0;

				if(PvAttrEnumGet(m_hDevice, "AcqStartTriggerMode", pBuffer, nBufferSize, &nReadSize) == tPvErr::ePvErrSuccess)
				{
					CString strDeviceData(pBuffer);

					for(int i = 0; i < EDeviceAlliedGigETriggerMode_Count; ++i)
					{
						if(strDeviceData.CompareNoCase(g_lpszAlliedGigETriggerMode[i]))
							continue;

						CString strSave;
						strSave.Format(_T("%d"), i);

						SetParamValue(EDeviceParameterAlliedGigE_AcqStartTriggerMode, strSave);
						SaveSettings(EDeviceParameterAlliedGigE_AcqStartTriggerMode);

						break;
					}
				}

				float fDeviceData = 0.0;

				if(PvAttrFloat32Get(m_hDevice, "FrameRate", &fDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%f"), fDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_FrameRate, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_FrameRate);
				}

				if(PvAttrUint32Get(m_hDevice, "FrameStartTriggerDelay", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_FrameStartTriggerDelay, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_FrameStartTriggerDelay);
				}

				memset(pBuffer, 0, sizeof(char) * 256);
				nReadSize = 0;

				if(PvAttrEnumGet(m_hDevice, "FrameStartTriggerEvent", pBuffer, nBufferSize, &nReadSize) == tPvErr::ePvErrSuccess)
				{
					CString strDeviceData(pBuffer);

					for(int i = 0; i < EDeviceAlliedGigETriggerEvent_Count; ++i)
					{
						if(strDeviceData.CompareNoCase(g_lpszAlliedGigETriggerEvent[i]))
							continue;

						CString strSave;
						strSave.Format(_T("%d"), i);

						SetParamValue(EDeviceParameterAlliedGigE_FrameStartTriggerEvent, strSave);
						SaveSettings(EDeviceParameterAlliedGigE_FrameStartTriggerEvent);

						break;
					}
				}

				memset(pBuffer, 0, sizeof(char) * 256);
				nReadSize = 0;

				if(PvAttrEnumGet(m_hDevice, "FrameStartTriggerMode", pBuffer, nBufferSize, &nReadSize) == tPvErr::ePvErrSuccess)
				{
					CString strDeviceData(pBuffer);

					for(int i = 0; i < EDeviceAlliedGigEFrameMode_Count; ++i)
					{
						if(strDeviceData.CompareNoCase(g_lpszAlliedGigEFrameMode[i]))
							continue;

						CString strSave;
						strSave.Format(_T("%d"), i);

						SetParamValue(EDeviceParameterAlliedGigE_FrameStartTriggerMode, strSave);
						SaveSettings(EDeviceParameterAlliedGigE_FrameStartTriggerMode);

						break;
					}
				}

				memset(pBuffer, 0, sizeof(char) * 256);
				nReadSize = 0;

				if(PvAttrEnumGet(m_hDevice, "FrameStartTriggerOverlap", pBuffer, nBufferSize, &nReadSize) == tPvErr::ePvErrSuccess)
				{
					CString strDeviceData(pBuffer);

					for(int i = 0; i < EDeviceAlliedGigETriggerOverlap_Count; ++i)
					{
						if(strDeviceData.CompareNoCase(g_lpszAlliedGigETriggerOverlap[i]))
							continue;

						CString strSave;
						strSave.Format(_T("%d"), i);

						SetParamValue(EDeviceParameterAlliedGigE_FrameStartTriggerOverlap, strSave);
						SaveSettings(EDeviceParameterAlliedGigE_FrameStartTriggerOverlap);

						break;
					}
				}

				if(PvAttrUint32Get(m_hDevice, "AcquisitionFrameCount", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_AcquisitionFrameCount, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_AcquisitionFrameCount);
				}

				memset(pBuffer, 0, sizeof(char) * 256);
				nReadSize = 0;

				if(PvAttrEnumGet(m_hDevice, "AcquisitionMode", pBuffer, nBufferSize, &nReadSize) == tPvErr::ePvErrSuccess)
				{
					CString strDeviceData(pBuffer);

					for(int i = 0; i < EDeviceAlliedGigEAcquisitionMode_Count; ++i)
					{
						if(strDeviceData.CompareNoCase(g_lpszAlliedGigEAcquisitionMode[i]))
							continue;

						CString strSave;
						strSave.Format(_T("%d"), i);

						SetParamValue(EDeviceParameterAlliedGigE_AcquisitionMode, strSave);
						SaveSettings(EDeviceParameterAlliedGigE_AcquisitionMode);

						break;
					}
				}

				if(PvAttrUint32Get(m_hDevice, "RecorderPreEventCount", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_RecorderPreEventCount, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_RecorderPreEventCount);
				}

				memset(pBuffer, 0, sizeof(char) * 256);
				nReadSize = 0;

				if(PvAttrEnumGet(m_hDevice, "ColorTransformationMode", pBuffer, nBufferSize, &nReadSize) == tPvErr::ePvErrSuccess)
				{
					CString strDeviceData(pBuffer);

					for(int i = 0; i < EDeviceAlliedGigEColorTransformationMode_Count; ++i)
					{
						if(strDeviceData.CompareNoCase(g_lpszAlliedGigEColorTransformationMode[i]))
							continue;

						CString strSave;
						strSave.Format(_T("%d"), i);

						SetParamValue(EDeviceParameterAlliedGigE_ColorTransformationMode, strSave);
						SaveSettings(EDeviceParameterAlliedGigE_ColorTransformationMode);

						break;
					}
				}

				if(PvAttrFloat32Get(m_hDevice, "ColorTransformationValueBB", &fDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%f"), fDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_ColorTransformationValueBB, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_ColorTransformationValueBB);
				}

				if(PvAttrFloat32Get(m_hDevice, "ColorTransformationValueBG", &fDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%f"), fDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_ColorTransformationValueBG, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_ColorTransformationValueBG);
				}

				if(PvAttrFloat32Get(m_hDevice, "ColorTransformationValueBR", &fDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%f"), fDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_ColorTransformationValueBR, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_ColorTransformationValueBR);
				}

				if(PvAttrFloat32Get(m_hDevice, "ColorTransformationValueGB", &fDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%f"), fDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_ColorTransformationValueGB, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_ColorTransformationValueGB);
				}

				if(PvAttrFloat32Get(m_hDevice, "ColorTransformationValueGG", &fDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%f"), fDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_ColorTransformationValueGG, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_ColorTransformationValueGG);
				}

				if(PvAttrFloat32Get(m_hDevice, "ColorTransformationValueGR", &fDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%f"), fDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_ColorTransformationValueGR, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_ColorTransformationValueGR);
				}

				if(PvAttrFloat32Get(m_hDevice, "ColorTransformationValueRB", &fDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%f"), fDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_ColorTransformationValueRB, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_ColorTransformationValueRB);
				}

				if(PvAttrFloat32Get(m_hDevice, "ColorTransformationValueRG", &fDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%f"), fDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_ColorTransformationValueRG, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_ColorTransformationValueRG);
				}

				if(PvAttrFloat32Get(m_hDevice, "ColorTransformationValueRR", &fDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%f"), fDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_ColorTransformationValueRR, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_ColorTransformationValueRR);
				}

				if(PvAttrUint32Get(m_hDevice, "DSPSubregionBottom", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_DSPSubregionBottom, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_DSPSubregionBottom);
				}

				if(PvAttrUint32Get(m_hDevice, "DSPSubregionLeft", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_DSPSubregionLeft, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_DSPSubregionLeft);
				}

				if(PvAttrUint32Get(m_hDevice, "DSPSubregionRight", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_DSPSubregionRight, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_DSPSubregionRight);
				}

				if(PvAttrUint32Get(m_hDevice, "DSPSubregionTop", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_DSPSubregionTop, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_DSPSubregionTop);
				}

				memset(pBuffer, 0, sizeof(char) * 256);
				nReadSize = 0;

				if(PvAttrEnumGet(m_hDevice, "ExposureMode", pBuffer, nBufferSize, &nReadSize) == tPvErr::ePvErrSuccess)
				{
					CString strDeviceData(pBuffer);

					for(int i = 0; i < EDeviceAlliedGigEExposureMode_Count; ++i)
					{
						if(strDeviceData.CompareNoCase(g_lpszAlliedGigEExposureMode[i]))
							continue;

						CString strSave;
						strSave.Format(_T("%d"), i);

						SetParamValue(EDeviceParameterAlliedGigE_ExposureMode, strSave);
						SaveSettings(EDeviceParameterAlliedGigE_ExposureMode);

						break;
					}
				}

				if(PvAttrUint32Get(m_hDevice, "ExposureValue", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_ExposureValue, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_ExposureValue);
				}

				if(PvAttrUint32Get(m_hDevice, "ExposureAutoAdjustTol", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_ExposureAutoAdjustTol, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_ExposureAutoAdjustTol);
				}

				memset(pBuffer, 0, sizeof(char) * 256);
				nReadSize = 0;

				if(PvAttrEnumGet(m_hDevice, "ExposureAutoAlg", pBuffer, nBufferSize, &nReadSize) == tPvErr::ePvErrSuccess)
				{
					CString strDeviceData(pBuffer);

					for(int i = 0; i < EDeviceAlliedGigEExposureAutoAlg_Count; ++i)
					{
						if(strDeviceData.CompareNoCase(g_lpszAlliedGigEExposureAutoAlg[i]))
							continue;

						CString strSave;
						strSave.Format(_T("%d"), i);

						SetParamValue(EDeviceParameterAlliedGigE_ExposureAutoAlg, strSave);
						SaveSettings(EDeviceParameterAlliedGigE_ExposureAutoAlg);

						break;
					}
				}

				if(PvAttrUint32Get(m_hDevice, "ExposureAutoMax", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_ExposureAutoMax, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_ExposureAutoMax);
				}

				if(PvAttrUint32Get(m_hDevice, "ExposureAutoMin", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_ExposureAutoMin, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_ExposureAutoMin);
				}

				if(PvAttrUint32Get(m_hDevice, "ExposureAutoOutliers", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_ExposureAutoOutliers, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_ExposureAutoOutliers);
				}

				if(PvAttrUint32Get(m_hDevice, "ExposureAutoRate", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_ExposureAutoRate, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_ExposureAutoRate);
				}

				if(PvAttrUint32Get(m_hDevice, "ExposureAutoTarget", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_ExposureAutoTarget, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_ExposureAutoTarget);
				}

				memset(pBuffer, 0, sizeof(char) * 256);
				nReadSize = 0;

				if(PvAttrEnumGet(m_hDevice, "GainMode", pBuffer, nBufferSize, &nReadSize) == tPvErr::ePvErrSuccess)
				{
					CString strDeviceData(pBuffer);

					for(int i = 0; i < EDeviceAlliedGigEGainMode_Count; ++i)
					{
						if(strDeviceData.CompareNoCase(g_lpszAlliedGigEGainMode[i]))
							continue;

						CString strSave;
						strSave.Format(_T("%d"), i);

						SetParamValue(EDeviceParameterAlliedGigE_GainMode, strSave);
						SaveSettings(EDeviceParameterAlliedGigE_GainMode);

						break;
					}
				}

				if(PvAttrUint32Get(m_hDevice, "GainValue", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_GainValue, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_GainValue);
				}

				if(PvAttrUint32Get(m_hDevice, "GainAutoAdjustTol", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_GainAutoAdjustTol, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_GainAutoAdjustTol);
				}

				if(PvAttrUint32Get(m_hDevice, "GainAutoMax", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_GainAutoMax, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_GainAutoMax);
				}

				if(PvAttrUint32Get(m_hDevice, "GainAutoMin", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_GainAutoMin, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_GainAutoMin);
				}

				if(PvAttrUint32Get(m_hDevice, "GainAutoOutliers", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_GainAutoOutliers, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_GainAutoOutliers);
				}

				if(PvAttrUint32Get(m_hDevice, "GainAutoRate", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_GainAutoRate, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_GainAutoRate);
				}

				if(PvAttrUint32Get(m_hDevice, "GainAutoTarget", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_GainAutoTarget, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_GainAutoTarget);
				}

				if(PvAttrUint32Get(m_hDevice, "IrisAutoTarget", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_IrisAutoTarget, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_IrisAutoTarget);
				}

				memset(pBuffer, 0, sizeof(char) * 256);
				nReadSize = 0;

				if(PvAttrEnumGet(m_hDevice, "IrisMode", pBuffer, nBufferSize, &nReadSize) == tPvErr::ePvErrSuccess)
				{
					CString strDeviceData(pBuffer);

					for(int i = 0; i < EDeviceAlliedGigEGainMode_Count; ++i)
					{
						if(strDeviceData.CompareNoCase(g_lpszAlliedGigEIrisMode[i]))
							continue;

						CString strSave;
						strSave.Format(_T("%d"), i);

						SetParamValue(EDeviceParameterAlliedGigE_IrisMode, strSave);
						SaveSettings(EDeviceParameterAlliedGigE_IrisMode);

						break;
					}
				}
				//////////////////////////////////////////////////////////////////////////
				//hjcho
				if(PvAttrUint32Get(m_hDevice, "IrisVideoLevelMax", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_IrisVideoLevelMax, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_IrisVideoLevelMax);
				}

				if(PvAttrUint32Get(m_hDevice, "IrisVideoLevelMin", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_IrisVideoLevelMin, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_IrisVideoLevelMin);
				}

				unsigned char ucDeviceData = false;

				if(PvAttrBooleanGet(m_hDevice, "LUTEnable", &ucDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), ucDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_LUTEnable, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_LUTEnable);
				}

				if(PvAttrUint32Get(m_hDevice, "LUTIndex", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_LUTIndex, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_LUTIndex);
				}

				memset(pBuffer, 0, sizeof(char) * 256);
				nReadSize = 0;

				if(PvAttrEnumGet(m_hDevice, "LUTMode", pBuffer, nBufferSize, &nReadSize) == tPvErr::ePvErrSuccess)
				{
					CString strDeviceData(pBuffer);

					for(int i = 0; i < EDeviceAlliedGigELUTMode_Count; ++i)
					{
						if(strDeviceData.CompareNoCase(g_lpszAlliedGigELUTMode[i]))
							continue;

						CString strSave;
						strSave.Format(_T("%d"), i);

						SetParamValue(EDeviceParameterAlliedGigE_LUTMode, strSave);
						SaveSettings(EDeviceParameterAlliedGigE_LUTMode);

						break;
					}
				}

				memset(pBuffer, 0, sizeof(char) * 256);
				nReadSize = 0;

				if(PvAttrEnumGet(m_hDevice, "LUTSelector", pBuffer, nBufferSize, &nReadSize) == tPvErr::ePvErrSuccess)
				{
					CString strDeviceData(pBuffer);

					for(int i = 0; i < EDeviceAlliedGigELUTSelector_Count; ++i)
					{
						if(strDeviceData.CompareNoCase(g_lpszAlliedGigELUTSelector[i]))
							continue;

						CString strSave;
						strSave.Format(_T("%d"), i);

						SetParamValue(EDeviceParameterAlliedGigE_LUTSelector, strSave);
						SaveSettings(EDeviceParameterAlliedGigE_LUTSelector);

						break;
					}
				}

				if(PvAttrUint32Get(m_hDevice, "LUTValue", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_LUTValue, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_LUTValue);
				}

				memset(pBuffer, 0, sizeof(char) * 256);
				nReadSize = 0;

				if(PvAttrEnumGet(m_hDevice, "EdgeFilter", pBuffer, nBufferSize, &nReadSize) == tPvErr::ePvErrSuccess)
				{
					CString strDeviceData(pBuffer);

					for(int i = 0; i < EDeviceAlliedGigEEdgeFilter_Count; ++i)
					{
						if(strDeviceData.CompareNoCase(g_lpszAlliedGigEEdgeFilter[i]))
							continue;

						CString strSave;
						strSave.Format(_T("%d"), i);

						SetParamValue(EDeviceParameterAlliedGigE_EdgeFilter, strSave);
						SaveSettings(EDeviceParameterAlliedGigE_EdgeFilter);

						break;
					}
				}

				if(PvAttrFloat32Get(m_hDevice, "Gamma", &fDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%f"), fDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_Gamma, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_Gamma);
				}

				if(PvAttrFloat32Get(m_hDevice, "Hue", &fDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%f"), fDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_Hue, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_Hue);
				}

				if(PvAttrUint32Get(m_hDevice, "OffsetValue", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_OffsetValue, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_OffsetValue);
				}

				if(PvAttrFloat32Get(m_hDevice, "Saturation", &fDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%f"), fDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_Saturation, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_Saturation);
				}

				memset(pBuffer, 0, sizeof(char) * 256);
				nReadSize = 0;

				if(PvAttrEnumGet(m_hDevice, "WhitebalMode", pBuffer, nBufferSize, &nReadSize) == tPvErr::ePvErrSuccess)
				{
					CString strDeviceData(pBuffer);

					for(int i = 0; i < EDeviceAlliedGigEWhitebalMode_Count; ++i)
					{
						if(strDeviceData.CompareNoCase(g_lpszAlliedGigEWhitebalMode[i]))
							continue;

						CString strSave;
						strSave.Format(_T("%d"), i);

						SetParamValue(EDeviceParameterAlliedGigE_WhitebalMode, strSave);
						SaveSettings(EDeviceParameterAlliedGigE_WhitebalMode);

						break;
					}
				}

				if(PvAttrUint32Get(m_hDevice, "WhitebalValueBlue", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_WhitebalValueBlue, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_WhitebalValueBlue);
				}

				if(PvAttrUint32Get(m_hDevice, "WhitebalValueRed", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_WhitebalValueRed, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_WhitebalValueRed);
				}

				if(PvAttrUint32Get(m_hDevice, "WhitebalAutoAdjustTol", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_WhitebalAutoAdjustTol, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_WhitebalAutoAdjustTol);
				}

				if(PvAttrUint32Get(m_hDevice, "WhitebalAutoRate", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_WhitebalAutoRate, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_WhitebalAutoRate);
				}

				memset(pBuffer, 0, sizeof(char) * 256);
				nReadSize = 0;

				if(PvAttrEnumGet(m_hDevice, "BandwidthCtrlMode", pBuffer, nBufferSize, &nReadSize) == tPvErr::ePvErrSuccess)
				{
					CString strDeviceData(pBuffer);

					for(int i = 0; i < EDeviceAlliedGigEBandwidthCtrlMode_Count; ++i)
					{
						if(strDeviceData.CompareNoCase(g_lpszAlliedGigEBandwidthCtrlMode[i]))
							continue;

						CString strSave;
						strSave.Format(_T("%d"), i);

						SetParamValue(EDeviceParameterAlliedGigE_BandwidthCtrlMode, strSave);
						SaveSettings(EDeviceParameterAlliedGigE_BandwidthCtrlMode);

						break;
					}
				}

				if(PvAttrUint32Get(m_hDevice, "HeartbeatInterval", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_HeartbeatInterval, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_HeartbeatInterval);
				}

				if(PvAttrUint32Get(m_hDevice, "HeartbeatTimeout", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_HeartbeatTimeout, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_HeartbeatTimeout);
				}

				if(PvAttrUint32Get(m_hDevice, "GvcpRetries", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_GvcpRetries, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_GvcpRetries);
				}

				if(PvAttrUint32Get(m_hDevice, "GvspLookbackWindow", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_GvspLookbackWindow, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_GvspLookbackWindow);
				}

				if(PvAttrFloat32Get(m_hDevice, "GvspResendPercent", &fDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%f"), fDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_GvspResendPercent, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_GvspResendPercent);
				}

				if(PvAttrUint32Get(m_hDevice, "GvspRetries", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_GvspRetries, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_GvspRetries);
				}

				memset(pBuffer, 0, sizeof(char) * 256);
				nReadSize = 0;

				if(PvAttrEnumGet(m_hDevice, "GvspSocketBuffersCount", pBuffer, nBufferSize, &nReadSize) == tPvErr::ePvErrSuccess)
				{
					CString strDeviceData(pBuffer);

					for(int i = 0; i < EDeviceAlliedGigEGvspSocketBuffersCount_Count; ++i)
					{
						if(strDeviceData.CompareNoCase(g_lpszAlliedGigEGvspSocketBuffersCount[i]))
							continue;

						CString strSave;
						strSave.Format(_T("%d"), i);

						SetParamValue(EDeviceParameterAlliedGigE_GvspSocketBuffersCount, strSave);
						SaveSettings(EDeviceParameterAlliedGigE_GvspSocketBuffersCount);

						break;
					}
				}

				if(PvAttrUint32Get(m_hDevice, "GvspTimeout", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_GvspTimeout, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_GvspTimeout);
				}

				memset(pBuffer, 0, sizeof(char) * 256);
				nReadSize = 0;

				if(PvAttrEnumGet(m_hDevice, "Strobe1ControlledDuration", pBuffer, nBufferSize, &nReadSize) == tPvErr::ePvErrSuccess)
				{
					CString strDeviceData(pBuffer);

					for(int i = 0; i < EDeviceAlliedGigESwitch_Count; ++i)
					{
						if(strDeviceData.CompareNoCase(g_lpszAlliedGigESwitch[i]))
							continue;

						CString strSave;
						strSave.Format(_T("%d"), i);

						SetParamValue(EDeviceParameterAlliedGigE_Strobe1ControlledDuration, strSave);
						SaveSettings(EDeviceParameterAlliedGigE_Strobe1ControlledDuration);

						break;
					}
				}

				if(PvAttrUint32Get(m_hDevice, "Strobe1Delay", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_Strobe1Delay, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_Strobe1Delay);
				}

				if(PvAttrUint32Get(m_hDevice, "Strobe1Duration", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_Strobe1Duration, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_Strobe1Duration);
				}

				memset(pBuffer, 0, sizeof(char) * 256);
				nReadSize = 0;

				if(PvAttrEnumGet(m_hDevice, "Strobe1Mode", pBuffer, nBufferSize, &nReadSize) == tPvErr::ePvErrSuccess)
				{
					CString strDeviceData(pBuffer);

					for(int i = 0; i < EDeviceAlliedGigEStrobeMode_Count; ++i)
					{
						if(strDeviceData.CompareNoCase(g_lpszAlliedGigEStrobeMode[i]))
							continue;

						CString strSave;
						strSave.Format(_T("%d"), i);

						SetParamValue(EDeviceParameterAlliedGigE_Strobe1Mode, strSave);
						SaveSettings(EDeviceParameterAlliedGigE_Strobe1Mode);

						break;
					}
				}

				if(PvAttrUint32Get(m_hDevice, "SyncIn1GlitchFilter", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_SyncIn1GlitchFilter, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_SyncIn1GlitchFilter);
				}

				if(PvAttrUint32Get(m_hDevice, "SyncIn2GlitchFilter", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_SyncIn2GlitchFilter, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_SyncIn2GlitchFilter);
				}

				memset(pBuffer, 0, sizeof(char) * 256);
				nReadSize = 0;

				if(PvAttrEnumGet(m_hDevice, "SyncOut1Invert", pBuffer, nBufferSize, &nReadSize) == tPvErr::ePvErrSuccess)
				{
					CString strDeviceData(pBuffer);

					for(int i = 0; i < EDeviceAlliedGigESwitch_Count; ++i)
					{
						if(strDeviceData.CompareNoCase(g_lpszAlliedGigESwitch[i]))
							continue;

						CString strSave;
						strSave.Format(_T("%d"), i);

						SetParamValue(EDeviceParameterAlliedGigE_SyncOut1Invert, strSave);
						SaveSettings(EDeviceParameterAlliedGigE_SyncOut1Invert);

						break;
					}
				}

				memset(pBuffer, 0, sizeof(char) * 256);
				nReadSize = 0;

				if(PvAttrEnumGet(m_hDevice, "SyncOut1Mode", pBuffer, nBufferSize, &nReadSize) == tPvErr::ePvErrSuccess)
				{
					CString strDeviceData(pBuffer);

					for(int i = 0; i < EDeviceAlliedGigESyncOutMode_Count; ++i)
					{
						if(strDeviceData.CompareNoCase(g_lpszAlliedGigESyncOutMode[i]))
							continue;

						CString strSave;
						strSave.Format(_T("%d"), i);

						SetParamValue(EDeviceParameterAlliedGigE_SyncOut1Mode, strSave);
						SaveSettings(EDeviceParameterAlliedGigE_SyncOut1Mode);

						break;
					}
				}

				memset(pBuffer, 0, sizeof(char) * 256);
				nReadSize = 0;

				if(PvAttrEnumGet(m_hDevice, "SyncOut2Invert", pBuffer, nBufferSize, &nReadSize) == tPvErr::ePvErrSuccess)
				{
					CString strDeviceData(pBuffer);

					for(int i = 0; i < EDeviceAlliedGigESwitch_Count; ++i)
					{
						if(strDeviceData.CompareNoCase(g_lpszAlliedGigESwitch[i]))
							continue;

						CString strSave;
						strSave.Format(_T("%d"), i);

						SetParamValue(EDeviceParameterAlliedGigE_SyncOut2Invert, strSave);
						SaveSettings(EDeviceParameterAlliedGigE_SyncOut2Invert);

						break;
					}
				}

				memset(pBuffer, 0, sizeof(char) * 256);
				nReadSize = 0;

				if(PvAttrEnumGet(m_hDevice, "SyncOut2Mode", pBuffer, nBufferSize, &nReadSize) == tPvErr::ePvErrSuccess)
				{
					CString strDeviceData(pBuffer);

					for(int i = 0; i < EDeviceAlliedGigESyncOutMode_Count; ++i)
					{
						if(strDeviceData.CompareNoCase(g_lpszAlliedGigESyncOutMode[i]))
							continue;

						CString strSave;
						strSave.Format(_T("%d"), i);

						SetParamValue(EDeviceParameterAlliedGigE_SyncOut2Mode, strSave);
						SaveSettings(EDeviceParameterAlliedGigE_SyncOut2Mode);

						break;
					}
				}

				if(PvAttrUint32Get(m_hDevice, "SyncOutGpoLevels", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_SyncOutGpoLevels, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_SyncOutGpoLevels);
				}

				if(PvAttrUint32Get(m_hDevice, "StreamBytesPerSecond", &nDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), nDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_StreamBytesPerSecond, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_StreamBytesPerSecond);
				}

				if(PvAttrBooleanGet(m_hDevice, "StreamFrameRateConstrain", &ucDeviceData) == tPvErr::ePvErrSuccess)
				{
					CString strSave;
					strSave.Format(_T("%d"), ucDeviceData);

					SetParamValue(EDeviceParameterAlliedGigE_StreamFrameRateConstrain, strSave);
					SaveSettings(EDeviceParameterAlliedGigE_StreamFrameRateConstrain);
				}

				memset(pBuffer, 0, sizeof(char) * 256);
				nReadSize = 0;

				if(PvAttrEnumGet(m_hDevice, "StreamHoldEnable", pBuffer, nBufferSize, &nReadSize) == tPvErr::ePvErrSuccess)
				{
					CString strDeviceData(pBuffer);

					for(int i = 0; i < EDeviceAlliedGigESwitch_Count; ++i)
					{
						if(strDeviceData.CompareNoCase(g_lpszAlliedGigESwitch[i]))
							continue;

						CString strSave;
						strSave.Format(_T("%d"), i);

						SetParamValue(EDeviceParameterAlliedGigE_StreamHoldEnable, strSave);
						SaveSettings(EDeviceParameterAlliedGigE_StreamHoldEnable);

						break;
					}
				}
			}
			break;
		case EDeviceAlliedGigEInitializeType_OnlyParam:
			{
				int nDatabaseData = 0;

				tPvUint32 nMaxWidth = 0, nMaxHeight = 0;
				tPvUint32 nBinningX = 0, nBinningY = 0;
				tPvUint32 nDecimationHorizontal = 0, nDecimationVertical = 0;
				
				if(PvAttrUint32Get(m_hDevice, "SensorWidth", &nMaxWidth) != tPvErr::ePvErrSuccess)
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Width Max"));
					eReturn = EDeviceInitializeResult_ReadOnDeviceError;
					break;
				}

				if(PvAttrUint32Get(m_hDevice, "SensorHeight", &nMaxHeight) != tPvErr::ePvErrSuccess)
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Height Max"));
					eReturn = EDeviceInitializeResult_ReadOnDeviceError;
					break;
				}
				
				nDatabaseData = 1;

				if(!PvAttrExists(m_hDevice, "BinningX"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "BinningX"))
					{
						if(GetBinningX(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("BinningX"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "BinningX", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 1;

							if(PvAttrUint32Set(m_hDevice, "BinningX", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("BinningX"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetBinningX(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("BinningX"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}					
				}
				
				nBinningX = nDatabaseData;

				nDatabaseData = 1;

				if(!PvAttrExists(m_hDevice, "BinningY"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "BinningY"))
					{
						if(GetBinningY(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("BinningY"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "BinningY", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 1;

							if(PvAttrUint32Set(m_hDevice, "BinningY", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("BinningY"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetBinningY(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("BinningY"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				nBinningY = nDatabaseData;

				nDatabaseData = 1;

				if(!PvAttrExists(m_hDevice, "DecimationHorizontal"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "DecimationHorizontal"))
					{
						if(GetDecimationHorizontal(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("DecimationHorizontal"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "DecimationHorizontal", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 1;

							if(PvAttrUint32Set(m_hDevice, "DecimationHorizontal", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("DecimationHorizontal"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetDecimationHorizontal(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("DecimationHorizontal"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				nDecimationHorizontal = nDatabaseData;

				nDatabaseData = 1;

				if(!PvAttrExists(m_hDevice, "DecimationVertical"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "DecimationVertical"))
					{
						if(GetDecimationVertical(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("DecimationVertical"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "DecimationVertical", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 1;

							if(PvAttrUint32Set(m_hDevice, "DecimationVertical", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("DecimationVertical"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetDecimationVertical(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("DecimationVertical"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				nDecimationVertical = nDatabaseData;

				if(!PvAttrExists(m_hDevice, "Width"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "Width"))
					{
						if(GetCanvasWidth(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Width"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "Width", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							tPvUint32 nDiv = nBinningX * nDecimationHorizontal;

							nDatabaseData = (nMaxWidth / nDiv);

							nDatabaseData = nDatabaseData - (nDatabaseData % 4);

							if(PvAttrUint32Set(m_hDevice, "Width", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Width"));
								eReturn = EDeviceInitializeResult_WriteToDeviceError;
								break;
							}
							if(SetCanvasWidth(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Width"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "Height"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "Height"))
					{
						if(GetCanvasHeight(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Height"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "Height", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							tPvUint32 nDiv = nBinningY * nDecimationVertical;

							nDatabaseData = (nMaxHeight / nDiv);

							nDatabaseData = nDatabaseData - (nDatabaseData % 4);

							if(PvAttrUint32Set(m_hDevice, "Height", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Height"));
								eReturn = EDeviceInitializeResult_WriteToDeviceError;
								break;
							}
							if(SetCanvasHeight(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Height"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "RegionX"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "RegionX"))
					{
						if(GetRegionX(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("RegionX"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "RegionX", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 0;

							if(PvAttrUint32Set(m_hDevice, "RegionX", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("RegionX"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetRegionX(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("RegionX"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "RegionY"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "RegionY"))
					{
						if(GetRegionY(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("RegionY"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "RegionY", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 0;

							if(PvAttrUint32Set(m_hDevice, "RegionY", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("RegionY"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetRegionY(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("RegionY"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				EDeviceAlliedGigEPixelFormat ePixelFormat = EDeviceAlliedGigEPixelFormat_Mono8;

				if(!PvAttrExists(m_hDevice, "PixelFormat"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "PixelFormat"))
					{
						if(GetPixelFormat(&ePixelFormat))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("PixelFormat"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrEnumSet(m_hDevice, "PixelFormat", CStringA(g_lpszAlliedGigEPixelFormat[ePixelFormat])) != tPvErr::ePvErrSuccess)
						{
							ePixelFormat = EDeviceAlliedGigEPixelFormat_Mono8;

							if(PvAttrEnumSet(m_hDevice, "PixelFormat", CStringA(g_lpszAlliedGigEPixelFormat[ePixelFormat])) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("PixelFormat"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetPixelFormat(ePixelFormat))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("PixelFormat"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "PacketSize"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "PacketSize"))
					{
						if(GetPacketSize(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("PacketSize"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "PacketSize", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 8228;

							if(PvAttrUint32Set(m_hDevice, "PacketSize", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("PacketSize"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetPacketSize(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("PacketSize"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "AcqEndTriggerEvent"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "AcqEndTriggerEvent"))
					{
						EDeviceAlliedGigETriggerEvent eAcqEndTriggerEvent = EDeviceAlliedGigETriggerEvent_EdgeRising;

						if(GetAcqEndTriggerEvent(&eAcqEndTriggerEvent))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AcqEndTriggerEvent"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrEnumSet(m_hDevice, "AcqEndTriggerEvent", CStringA(g_lpszAlliedGigETriggerEvent[eAcqEndTriggerEvent])) != tPvErr::ePvErrSuccess)
						{
							eAcqEndTriggerEvent = EDeviceAlliedGigETriggerEvent_EdgeRising;

							if(PvAttrEnumSet(m_hDevice, "AcqEndTriggerEvent", CStringA(g_lpszAlliedGigETriggerEvent[eAcqEndTriggerEvent])) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("AcqEndTriggerEvent"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetAcqEndTriggerEvent(eAcqEndTriggerEvent))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("AcqEndTriggerEvent"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "AcqEndTriggerMode"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "AcqEndTriggerMode"))
					{
						EDeviceAlliedGigETriggerMode eAcqEndTriggerMode = EDeviceAlliedGigETriggerMode_Disabled;

						if(GetAcqEndTriggerMode(&eAcqEndTriggerMode))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AcqEndTriggerMode"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrEnumSet(m_hDevice, "AcqEndTriggerMode", CStringA(g_lpszAlliedGigETriggerMode[eAcqEndTriggerMode])) != tPvErr::ePvErrSuccess)
						{
							eAcqEndTriggerMode = EDeviceAlliedGigETriggerMode_Disabled;

							if(PvAttrEnumSet(m_hDevice, "AcqEndTriggerMode", CStringA(g_lpszAlliedGigETriggerMode[eAcqEndTriggerMode])) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("AcqEndTriggerMode"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetAcqEndTriggerMode(eAcqEndTriggerMode))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("AcqEndTriggerMode"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "AcqRecTriggerEvent"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "AcqRecTriggerEvent"))
					{
						EDeviceAlliedGigETriggerEvent eAcqRecTriggerEvent = EDeviceAlliedGigETriggerEvent_EdgeRising;

						if(GetAcqRecTriggerEvent(&eAcqRecTriggerEvent))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AcqRecTriggerEvent"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrEnumSet(m_hDevice, "AcqRecTriggerEvent", CStringA(g_lpszAlliedGigETriggerEvent[eAcqRecTriggerEvent])) != tPvErr::ePvErrSuccess)
						{
							eAcqRecTriggerEvent = EDeviceAlliedGigETriggerEvent_EdgeRising;

							if(PvAttrEnumSet(m_hDevice, "AcqRecTriggerEvent", CStringA(g_lpszAlliedGigETriggerEvent[eAcqRecTriggerEvent])) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("AcqRecTriggerEvent"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetAcqRecTriggerEvent(eAcqRecTriggerEvent))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("AcqRecTriggerEvent"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "AcqRecTriggerMode"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "AcqRecTriggerMode"))
					{
						EDeviceAlliedGigETriggerMode eAcqRecTriggerMode = EDeviceAlliedGigETriggerMode_Disabled;

						if(GetAcqRecTriggerMode(&eAcqRecTriggerMode))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AcqRecTriggerMode"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrEnumSet(m_hDevice, "AcqRecTriggerMode", CStringA(g_lpszAlliedGigETriggerMode[eAcqRecTriggerMode])) != tPvErr::ePvErrSuccess)
						{
							eAcqRecTriggerMode = EDeviceAlliedGigETriggerMode_Disabled;

							if(PvAttrEnumSet(m_hDevice, "AcqRecTriggerMode", CStringA(g_lpszAlliedGigETriggerMode[eAcqRecTriggerMode])) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("AcqRecTriggerMode"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetAcqRecTriggerMode(eAcqRecTriggerMode))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("AcqRecTriggerMode"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "AcqStartTriggerEvent"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "AcqStartTriggerEvent"))
					{
						EDeviceAlliedGigETriggerEvent eAcqStartTriggerEvent = EDeviceAlliedGigETriggerEvent_EdgeRising;

						if(GetAcqStartTriggerEvent(&eAcqStartTriggerEvent))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AcqStartTriggerEvent"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrEnumSet(m_hDevice, "AcqStartTriggerEvent", CStringA(g_lpszAlliedGigETriggerEvent[eAcqStartTriggerEvent])) != tPvErr::ePvErrSuccess)
						{
							eAcqStartTriggerEvent = EDeviceAlliedGigETriggerEvent_EdgeRising;

							if(PvAttrEnumSet(m_hDevice, "AcqStartTriggerEvent", CStringA(g_lpszAlliedGigETriggerEvent[eAcqStartTriggerEvent])) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("AcqStartTriggerEvent"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetAcqStartTriggerEvent(eAcqStartTriggerEvent))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("AcqStartTriggerEvent"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "AcqStartTriggerMode"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "AcqStartTriggerMode"))
					{
						EDeviceAlliedGigETriggerMode eAcqStartTriggerMode = EDeviceAlliedGigETriggerMode_Disabled;

						if(GetAcqStartTriggerMode(&eAcqStartTriggerMode))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AcqStartTriggerMode"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrEnumSet(m_hDevice, "AcqStartTriggerMode", CStringA(g_lpszAlliedGigETriggerMode[eAcqStartTriggerMode])) != tPvErr::ePvErrSuccess)
						{
							eAcqStartTriggerMode = EDeviceAlliedGigETriggerMode_Disabled;

							if(PvAttrEnumSet(m_hDevice, "AcqStartTriggerMode", CStringA(g_lpszAlliedGigETriggerMode[eAcqStartTriggerMode])) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("AcqStartTriggerMode"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetAcqStartTriggerMode(eAcqStartTriggerMode))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("AcqStartTriggerMode"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				float fDatabaseData = 0.0;

				if(!PvAttrExists(m_hDevice, "FrameRate"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "FrameRate"))
					{
						if(GetFrameRate(&fDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("FrameRate"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrFloat32Set(m_hDevice, "FrameRate", fDatabaseData) != tPvErr::ePvErrSuccess)
						{
							fDatabaseData = 30.029;

							if(PvAttrFloat32Set(m_hDevice, "FrameRate", fDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("FrameRate"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetFrameRate(fDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("FrameRate"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "FrameStartTriggerDelay"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "FrameStartTriggerDelay"))
					{
						if(GetFrameStartTriggerDelay(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("FrameStartTriggerDelay"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "FrameStartTriggerDelay", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 0;

							if(PvAttrUint32Set(m_hDevice, "FrameStartTriggerDelay", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("FrameStartTriggerDelay"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetFrameStartTriggerDelay(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("FrameStartTriggerDelay"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "FrameStartTriggerEvent"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "FrameStartTriggerEvent"))
					{
						EDeviceAlliedGigETriggerEvent eFrameStartTriggerEvent = EDeviceAlliedGigETriggerEvent_EdgeRising;

						if(GetFrameStartTriggerEvent(&eFrameStartTriggerEvent))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("FrameStartTriggerEvent"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrEnumSet(m_hDevice, "FrameStartTriggerEvent", CStringA(g_lpszAlliedGigETriggerEvent[eFrameStartTriggerEvent])) != tPvErr::ePvErrSuccess)
						{
							eFrameStartTriggerEvent = EDeviceAlliedGigETriggerEvent_EdgeRising;

							if(PvAttrEnumSet(m_hDevice, "FrameStartTriggerEvent", CStringA(g_lpszAlliedGigETriggerEvent[eFrameStartTriggerEvent])) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("FrameStartTriggerEvent"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetFrameStartTriggerEvent(eFrameStartTriggerEvent))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("FrameStartTriggerEvent"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "FrameStartTriggerMode"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "FrameStartTriggerMode"))
					{
						EDeviceAlliedGigEFrameMode eFrameStartTriggerMode = EDeviceAlliedGigEFrameMode_Freerun;

						if(GetFrameStartTriggerMode(&eFrameStartTriggerMode))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("FrameStartTriggerMode"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrEnumSet(m_hDevice, "FrameStartTriggerMode", CStringA(g_lpszAlliedGigEFrameMode[eFrameStartTriggerMode])) != tPvErr::ePvErrSuccess)
						{
							eFrameStartTriggerMode = EDeviceAlliedGigEFrameMode_Freerun;

							if(PvAttrEnumSet(m_hDevice, "FrameStartTriggerMode", CStringA(g_lpszAlliedGigEFrameMode[eFrameStartTriggerMode])) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("FrameStartTriggerMode"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetFrameStartTriggerMode(eFrameStartTriggerMode))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("FrameStartTriggerMode"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "FrameStartTriggerOverlap"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "FrameStartTriggerOverlap"))
					{
						EDeviceAlliedGigETriggerOverlap eFrameStartTriggerOverlap = EDeviceAlliedGigETriggerOverlap_Off;

						if(GetFrameStartTriggerOverlap(&eFrameStartTriggerOverlap))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("FrameStartTriggerOverlap"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrEnumSet(m_hDevice, "FrameStartTriggerOverlap", CStringA(g_lpszAlliedGigETriggerOverlap[eFrameStartTriggerOverlap])) != tPvErr::ePvErrSuccess)
						{
							eFrameStartTriggerOverlap = EDeviceAlliedGigETriggerOverlap_Off;

							if(PvAttrEnumSet(m_hDevice, "FrameStartTriggerOverlap", CStringA(g_lpszAlliedGigETriggerOverlap[eFrameStartTriggerOverlap])) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("FrameStartTriggerOverlap"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetFrameStartTriggerOverlap(eFrameStartTriggerOverlap))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("FrameStartTriggerOverlap"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "AcquisitionFrameCount"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "AcquisitionFrameCount"))
					{
						if(GetAcquisitionFrameCount(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AcquisitionFrameCount"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "AcquisitionFrameCount", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 0;

							if(PvAttrUint32Set(m_hDevice, "AcquisitionFrameCount", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("AcquisitionFrameCount"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetAcquisitionFrameCount(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("AcquisitionFrameCount"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "AcquisitionMode"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "AcquisitionMode"))
					{
						EDeviceAlliedGigEAcquisitionMode eAcquisitionMode = EDeviceAlliedGigEAcquisitionMode_Continuous;

						if(GetAcquisitionMode(&eAcquisitionMode))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AcquisitionMode"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrEnumSet(m_hDevice, "AcquisitionMode", CStringA(g_lpszAlliedGigEAcquisitionMode[eAcquisitionMode])) != tPvErr::ePvErrSuccess)
						{
							eAcquisitionMode = EDeviceAlliedGigEAcquisitionMode_Continuous;

							if(PvAttrEnumSet(m_hDevice, "AcquisitionMode", CStringA(g_lpszAlliedGigEAcquisitionMode[eAcquisitionMode])) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("AcquisitionMode"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetAcquisitionMode(eAcquisitionMode))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("AcquisitionMode"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "RecorderPreEventCount"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "RecorderPreEventCount"))
					{
						if(GetRecorderPreEventCount(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("RecorderPreEventCount"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "RecorderPreEventCount", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 0;

							if(PvAttrUint32Set(m_hDevice, "RecorderPreEventCount", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("RecorderPreEventCount"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetRecorderPreEventCount(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("RecorderPreEventCount"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "ColorTransformationMode"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "ColorTransformationMode"))
					{
						EDeviceAlliedGigEColorTransformationMode eColorTransformationMode = EDeviceAlliedGigEColorTransformationMode_Off;

						if(GetColorTransformationMode(&eColorTransformationMode))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ColorTransformationMode"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrEnumSet(m_hDevice, "ColorTransformationMode", CStringA(g_lpszAlliedGigEColorTransformationMode[eColorTransformationMode])) != tPvErr::ePvErrSuccess)
						{
							eColorTransformationMode = EDeviceAlliedGigEColorTransformationMode_Off;

							if(PvAttrEnumSet(m_hDevice, "ColorTransformationMode", CStringA(g_lpszAlliedGigEColorTransformationMode[eColorTransformationMode])) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("ColorTransformationMode"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetColorTransformationMode(eColorTransformationMode))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("ColorTransformationMode"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "ColorTransformationValueBB"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "ColorTransformationValueBB"))
					{
						if(GetColorTransformationValueBB(&fDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ColorTransformationValueBB"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrFloat32Set(m_hDevice, "ColorTransformationValueBB", fDatabaseData) != tPvErr::ePvErrSuccess)
						{
							fDatabaseData = 1.400000;

							if(PvAttrFloat32Set(m_hDevice, "ColorTransformationValueBB", fDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("ColorTransformationValueBB"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetColorTransformationValueBB(fDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("ColorTransformationValueBB"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "ColorTransformationValueBG"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "ColorTransformationValueBG"))
					{
						if(GetColorTransformationValueBG(&fDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ColorTransformationValueBG"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrFloat32Set(m_hDevice, "ColorTransformationValueBG", fDatabaseData) != tPvErr::ePvErrSuccess)
						{
							fDatabaseData = -0.480000;

							if(PvAttrFloat32Set(m_hDevice, "ColorTransformationValueBG", fDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("ColorTransformationValueBG"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetColorTransformationValueBG(fDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("ColorTransformationValueBG"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "ColorTransformationValueBR"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "ColorTransformationValueBR"))
					{
						if(GetColorTransformationValueBR(&fDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ColorTransformationValueBR"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrFloat32Set(m_hDevice, "ColorTransformationValueBR", fDatabaseData) != tPvErr::ePvErrSuccess)
						{
							fDatabaseData = 0.080000;

							if(PvAttrFloat32Set(m_hDevice, "ColorTransformationValueBR", fDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("ColorTransformationValueBR"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetColorTransformationValueBR(fDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("ColorTransformationValueBR"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "ColorTransformationValueGB"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "ColorTransformationValueGB"))
					{
						if(GetColorTransformationValueGB(&fDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ColorTransformationValueGB"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrFloat32Set(m_hDevice, "ColorTransformationValueGB", fDatabaseData) != tPvErr::ePvErrSuccess)
						{
							fDatabaseData = -0.170000;

							if(PvAttrFloat32Set(m_hDevice, "ColorTransformationValueGB", fDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("ColorTransformationValueGB"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetColorTransformationValueGB(fDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("ColorTransformationValueGB"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "ColorTransformationValueGG"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "ColorTransformationValueGG"))
					{
						if(GetColorTransformationValueGG(&fDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ColorTransformationValueGG"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrFloat32Set(m_hDevice, "ColorTransformationValueGG", fDatabaseData) != tPvErr::ePvErrSuccess)
						{
							fDatabaseData = 1.180000;

							if(PvAttrFloat32Set(m_hDevice, "ColorTransformationValueGG", fDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("ColorTransformationValueGG"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetColorTransformationValueGG(fDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("ColorTransformationValueGG"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "ColorTransformationValueGR"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "ColorTransformationValueGR"))
					{
						if(GetColorTransformationValueGR(&fDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ColorTransformationValueGR"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrFloat32Set(m_hDevice, "ColorTransformationValueGR", fDatabaseData) != tPvErr::ePvErrSuccess)
						{
							fDatabaseData = -0.010000;

							if(PvAttrFloat32Set(m_hDevice, "ColorTransformationValueGR", fDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("ColorTransformationValueGR"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetColorTransformationValueGR(fDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("ColorTransformationValueGR"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "ColorTransformationValueRB"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "ColorTransformationValueRB"))
					{
						if(GetColorTransformationValueRB(&fDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ColorTransformationValueRB"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrFloat32Set(m_hDevice, "ColorTransformationValueRB", fDatabaseData) != tPvErr::ePvErrSuccess)
						{
							fDatabaseData = 0.180000;

							if(PvAttrFloat32Set(m_hDevice, "ColorTransformationValueRB", fDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("ColorTransformationValueRB"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetColorTransformationValueRB(fDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("ColorTransformationValueRB"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "ColorTransformationValueRG"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "ColorTransformationValueRG"))
					{
						if(GetColorTransformationValueRG(&fDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ColorTransformationValueRG"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrFloat32Set(m_hDevice, "ColorTransformationValueRG", fDatabaseData) != tPvErr::ePvErrSuccess)
						{
							fDatabaseData = -0.170000;

							if(PvAttrFloat32Set(m_hDevice, "ColorTransformationValueRG", fDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("ColorTransformationValueRG"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetColorTransformationValueRG(fDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("ColorTransformationValueRG"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "ColorTransformationValueRR"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "ColorTransformationValueRR"))
					{
						if(GetColorTransformationValueRR(&fDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ColorTransformationValueRR"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrFloat32Set(m_hDevice, "ColorTransformationValueRR", fDatabaseData) != tPvErr::ePvErrSuccess)
						{
							fDatabaseData = 0.990000;

							if(PvAttrFloat32Set(m_hDevice, "ColorTransformationValueRR", fDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("ColorTransformationValueRR"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetColorTransformationValueRR(fDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("ColorTransformationValueRR"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "DSPSubregionBottom"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "DSPSubregionBottom"))
					{
						if(GetDSPSubregionBottom(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("DSPSubregionBottom"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "DSPSubregionBottom", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 0;

							if(PvAttrUint32Set(m_hDevice, "DSPSubregionBottom", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("DSPSubregionBottom"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetDSPSubregionBottom(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("DSPSubregionBottom"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "DSPSubregionLeft"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "DSPSubregionLeft"))
					{
						if(GetDSPSubregionLeft(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("DSPSubregionLeft"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "DSPSubregionLeft", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 0;

							if(PvAttrUint32Set(m_hDevice, "DSPSubregionLeft", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("DSPSubregionLeft"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetDSPSubregionLeft(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("DSPSubregionLeft"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "DSPSubregionRight"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "DSPSubregionRight"))
					{
						if(GetDSPSubregionRight(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("DSPSubregionRight"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "DSPSubregionRight", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 0;

							if(PvAttrUint32Set(m_hDevice, "DSPSubregionRight", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("DSPSubregionRight"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetDSPSubregionRight(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("DSPSubregionRight"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "DSPSubregionTop"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "DSPSubregionTop"))
					{
						if(GetDSPSubregionTop(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("DSPSubregionTop"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "DSPSubregionTop", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 0;

							if(PvAttrUint32Set(m_hDevice, "DSPSubregionTop", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("DSPSubregionTop"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetDSPSubregionTop(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("DSPSubregionTop"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "ExposureMode"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "ExposureMode"))
					{
						EDeviceAlliedGigEExposureMode eExposureMode = EDeviceAlliedGigEExposureMode_Manual;

						if(GetExposureMode(&eExposureMode))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ExposureMode"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrEnumSet(m_hDevice, "ExposureMode", CStringA(g_lpszAlliedGigEExposureMode[eExposureMode])) != tPvErr::ePvErrSuccess)
						{
							eExposureMode = EDeviceAlliedGigEExposureMode_Manual;

							if(PvAttrEnumSet(m_hDevice, "ExposureMode", CStringA(g_lpszAlliedGigEExposureMode[eExposureMode])) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("ExposureMode"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetExposureMode(eExposureMode))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("ExposureMode"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "ExposureValue"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "ExposureValue"))
					{
						if(GetExposureValue(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ExposureValue"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "ExposureValue", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 0;

							if(PvAttrUint32Set(m_hDevice, "ExposureValue", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("ExposureValue"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetExposureValue(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("ExposureValue"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "ExposureAutoAdjustTol"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "ExposureAutoAdjustTol"))
					{
						if(GetExposureAutoAdjustTol(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ExposureAutoAdjustTol"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "ExposureAutoAdjustTol", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 0;

							if(PvAttrUint32Set(m_hDevice, "ExposureAutoAdjustTol", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("ExposureAutoAdjustTol"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetExposureAutoAdjustTol(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("ExposureAutoAdjustTol"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "ExposureAutoAlg"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "ExposureAutoAlg"))
					{
						EDeviceAlliedGigEExposureAutoAlg eExposureAutoAlg = EDeviceAlliedGigEExposureAutoAlg_Mean;

						if(GetExposureAutoAlg(&eExposureAutoAlg))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ExposureAutoAlg"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrEnumSet(m_hDevice, "ExposureAutoAlg", CStringA(g_lpszAlliedGigEExposureAutoAlg[eExposureAutoAlg])) != tPvErr::ePvErrSuccess)
						{
							eExposureAutoAlg = EDeviceAlliedGigEExposureAutoAlg_Mean;

							if(PvAttrEnumSet(m_hDevice, "ExposureAutoAlg", CStringA(g_lpszAlliedGigEExposureAutoAlg[eExposureAutoAlg])) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("ExposureAutoAlg"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetExposureAutoAlg(eExposureAutoAlg))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("ExposureAutoAlg"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "ExposureAutoMax"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "ExposureAutoMax"))
					{
						if(GetExposureAutoMax(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ExposureAutoMax"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "ExposureAutoMax", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 0;

							if(PvAttrUint32Set(m_hDevice, "ExposureAutoMax", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("ExposureAutoMax"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetExposureAutoMax(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("ExposureAutoMax"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "ExposureAutoMin"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "ExposureAutoMin"))
					{
						if(GetExposureAutoMin(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ExposureAutoMin"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "ExposureAutoMin", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 0;

							if(PvAttrUint32Set(m_hDevice, "ExposureAutoMin", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("ExposureAutoMin"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetExposureAutoMin(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("ExposureAutoMin"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "ExposureAutoOutliers"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "ExposureAutoOutliers"))
					{
						if(GetExposureAutoOutliers(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ExposureAutoOutliers"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "ExposureAutoOutliers", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 0;

							if(PvAttrUint32Set(m_hDevice, "ExposureAutoOutliers", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("ExposureAutoOutliers"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetExposureAutoOutliers(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("ExposureAutoOutliers"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "ExposureAutoRate"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "ExposureAutoRate"))
					{
						if(GetExposureAutoRate(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ExposureAutoRate"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "ExposureAutoRate", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 0;

							if(PvAttrUint32Set(m_hDevice, "ExposureAutoRate", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("ExposureAutoRate"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetExposureAutoRate(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("ExposureAutoRate"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "ExposureAutoTarget"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "ExposureAutoTarget"))
					{
						if(GetExposureAutoTarget(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ExposureAutoTarget"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "ExposureAutoTarget", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 0;

							if(PvAttrUint32Set(m_hDevice, "ExposureAutoTarget", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("ExposureAutoTarget"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetExposureAutoTarget(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("ExposureAutoTarget"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "GainMode"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "GainMode"))
					{
						EDeviceAlliedGigEGainMode eGainMode = EDeviceAlliedGigEGainMode_Manual;

						if(GetGainMode(&eGainMode))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("GainMode"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrEnumSet(m_hDevice, "GainMode", CStringA(g_lpszAlliedGigEGainMode[eGainMode])) != tPvErr::ePvErrSuccess)
						{
							eGainMode = EDeviceAlliedGigEGainMode_Manual;

							if(PvAttrEnumSet(m_hDevice, "GainMode", CStringA(g_lpszAlliedGigEGainMode[eGainMode])) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("GainMode"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetGainMode(eGainMode))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("GainMode"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "GainValue"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "GainValue"))
					{
						if(GetGainValue(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("GainValue"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "GainValue", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 0;

							if(PvAttrUint32Set(m_hDevice, "GainValue", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("GainValue"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetGainValue(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("GainValue"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "GainAutoAdjustTol"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "GainAutoAdjustTol"))
					{
						if(GetGainAutoAdjustTol(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("GainAutoAdjustTol"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "GainAutoAdjustTol", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 0;

							if(PvAttrUint32Set(m_hDevice, "GainAutoAdjustTol", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("GainAutoAdjustTol"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetGainAutoAdjustTol(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("GainAutoAdjustTol"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "GainAutoMax"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "GainAutoMax"))
					{
						if(GetGainAutoMax(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("GainAutoMax"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "GainAutoMax", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 0;

							if(PvAttrUint32Set(m_hDevice, "GainAutoMax", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("GainAutoMax"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetGainAutoMax(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("GainAutoMax"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "GainAutoMin"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "GainAutoMin"))
					{
						if(GetGainAutoMin(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("GainAutoMin"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "GainAutoMin", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 0;

							if(PvAttrUint32Set(m_hDevice, "GainAutoMin", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("GainAutoMin"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetGainAutoMin(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("GainAutoMin"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "GainAutoOutliers"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "GainAutoOutliers"))
					{
						if(GetGainAutoOutliers(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("GainAutoOutliers"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "GainAutoOutliers", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 0;

							if(PvAttrUint32Set(m_hDevice, "GainAutoOutliers", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("GainAutoOutliers"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetGainAutoOutliers(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("GainAutoOutliers"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "GainAutoRate"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "GainAutoRate"))
					{
						if(GetGainAutoRate(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("GainAutoRate"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "GainAutoRate", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 0;

							if(PvAttrUint32Set(m_hDevice, "GainAutoRate", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("GainAutoRate"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetGainAutoRate(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("GainAutoRate"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "GainAutoTarget"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "GainAutoTarget"))
					{
						if(GetGainAutoTarget(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("GainAutoTarget"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "GainAutoTarget", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 0;

							if(PvAttrUint32Set(m_hDevice, "GainAutoTarget", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("GainAutoTarget"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetGainAutoTarget(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("GainAutoTarget"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "IrisAutoTarget"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "IrisAutoTarget"))
					{
						if(GetIrisAutoTarget(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("IrisAutoTarget"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "IrisAutoTarget", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 0;

							if(PvAttrUint32Set(m_hDevice, "IrisAutoTarget", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("IrisAutoTarget"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetIrisAutoTarget(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("IrisAutoTarget"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "IrisMode"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "IrisMode"))
					{
						EDeviceAlliedGigEIrisMode eIrisMode = EDeviceAlliedGigEIrisMode_Disabled;

						if(GetIrisMode(&eIrisMode))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("IrisMode"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrEnumSet(m_hDevice, "IrisMode", CStringA(g_lpszAlliedGigEIrisMode[eIrisMode])) != tPvErr::ePvErrSuccess)
						{
							eIrisMode = EDeviceAlliedGigEIrisMode_Disabled;

							if(PvAttrEnumSet(m_hDevice, "IrisMode", CStringA(g_lpszAlliedGigEIrisMode[eIrisMode])) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("IrisMode"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetIrisMode(eIrisMode))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("IrisMode"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "IrisVideoLevelMax"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "IrisVideoLevelMax"))
					{
						if(GetIrisVideoLevelMax(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("IrisVideoLevelMax"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "IrisVideoLevelMax", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 0;

							if(PvAttrUint32Set(m_hDevice, "IrisVideoLevelMax", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("IrisVideoLevelMax"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetIrisVideoLevelMax(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("IrisVideoLevelMax"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "IrisVideoLevelMin"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "IrisVideoLevelMin"))
					{
						if(GetIrisVideoLevelMin(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("IrisVideoLevelMin"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "IrisVideoLevelMin", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 0;

							if(PvAttrUint32Set(m_hDevice, "IrisVideoLevelMin", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("IrisVideoLevelMin"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetIrisVideoLevelMin(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("IrisVideoLevelMin"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				bool bDatabaseData = false;

				if(!PvAttrExists(m_hDevice, "LUTEnable"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "LUTEnable"))
					{
						if(GetLUTEnable(&bDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LUTEnable"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrBooleanSet(m_hDevice, "LUTEnable", bDatabaseData) != tPvErr::ePvErrSuccess)
						{
							bDatabaseData = false;

							if(PvAttrBooleanSet(m_hDevice, "LUTEnable", bDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("LUTEnable"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetLUTEnable(bDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("LUTEnable"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "LUTIndex"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "LUTIndex"))
					{
						if(GetLUTIndex(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LUTIndex"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "LUTIndex", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 0;

							if(PvAttrUint32Set(m_hDevice, "LUTIndex", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("LUTIndex"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetLUTIndex(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("LUTIndex"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "LUTMode"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "LUTMode"))
					{
						EDeviceAlliedGigELUTMode eLUTMode = EDeviceAlliedGigELUTMode_Luminance;

						if(GetLUTMode(&eLUTMode))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LUTMode"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrEnumSet(m_hDevice, "LUTMode", CStringA(g_lpszAlliedGigELUTMode[eLUTMode])) != tPvErr::ePvErrSuccess)
						{
							eLUTMode = EDeviceAlliedGigELUTMode_Luminance;

							if(PvAttrEnumSet(m_hDevice, "LUTMode", CStringA(g_lpszAlliedGigELUTMode[eLUTMode])) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("LUTMode"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetLUTMode(eLUTMode))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("LUTMode"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "LUTSelector"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "LUTSelector"))
					{
						EDeviceAlliedGigELUTSelector eLUTSelector = EDeviceAlliedGigELUTSelector_LUT1;

						if(GetLUTSelector(&eLUTSelector))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LUTSelector"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrEnumSet(m_hDevice, "LUTSelector", CStringA(g_lpszAlliedGigELUTSelector[eLUTSelector])) != tPvErr::ePvErrSuccess)
						{
							eLUTSelector = EDeviceAlliedGigELUTSelector_LUT1;

							if(PvAttrEnumSet(m_hDevice, "LUTSelector", CStringA(g_lpszAlliedGigELUTSelector[eLUTSelector])) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("LUTSelector"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetLUTSelector(eLUTSelector))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("LUTSelector"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "LUTValue"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "LUTValue"))
					{
						if(GetLUTValue(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LUTValue"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "LUTValue", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 0;

							if(PvAttrUint32Set(m_hDevice, "LUTValue", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("LUTValue"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetLUTValue(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("LUTValue"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "EdgeFilter"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "EdgeFilter"))
					{
						EDeviceAlliedGigEEdgeFilter eEdgeFilter = EDeviceAlliedGigEEdgeFilter_Off;

						if(GetEdgeFilter(&eEdgeFilter))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("EdgeFilter"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrEnumSet(m_hDevice, "EdgeFilter", CStringA(g_lpszAlliedGigEEdgeFilter[eEdgeFilter])) != tPvErr::ePvErrSuccess)
						{
							eEdgeFilter = EDeviceAlliedGigEEdgeFilter_Off;

							if(PvAttrEnumSet(m_hDevice, "EdgeFilter", CStringA(g_lpszAlliedGigEEdgeFilter[eEdgeFilter])) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("EdgeFilter"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetEdgeFilter(eEdgeFilter))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("EdgeFilter"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "Gamma"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "Gamma"))
					{
						if(GetGamma(&fDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Gamma"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrFloat32Set(m_hDevice, "Gamma", fDatabaseData) != tPvErr::ePvErrSuccess)
						{
							fDatabaseData = 1.000000;

							if(PvAttrFloat32Set(m_hDevice, "Gamma", fDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Gamma"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetGamma(fDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Gamma"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "Hue"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "Hue"))
					{
						if(GetHue(&fDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Hue"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrFloat32Set(m_hDevice, "Hue", fDatabaseData) != tPvErr::ePvErrSuccess)
						{
							fDatabaseData = 0.000000;

							if(PvAttrFloat32Set(m_hDevice, "Hue", fDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Hue"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetHue(fDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Hue"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "OffsetValue"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "OffsetValue"))
					{
						if(GetOffsetValue(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("OffsetValue"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "OffsetValue", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 0;

							if(PvAttrUint32Set(m_hDevice, "OffsetValue", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("OffsetValue"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetOffsetValue(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("OffsetValue"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "Saturation"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "Saturation"))
					{
						if(GetSaturation(&fDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Saturation"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrFloat32Set(m_hDevice, "Saturation", fDatabaseData) != tPvErr::ePvErrSuccess)
						{
							fDatabaseData = 1.000000;

							if(PvAttrFloat32Set(m_hDevice, "Saturation", fDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Saturation"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetSaturation(fDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Saturation"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "WhitebalMode"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "WhitebalMode"))
					{
						EDeviceAlliedGigEWhitebalMode eWhitebalMode = EDeviceAlliedGigEWhitebalMode_Manual;

						if(GetWhitebalMode(&eWhitebalMode))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("WhitebalMode"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrEnumSet(m_hDevice, "WhitebalMode", CStringA(g_lpszAlliedGigEWhitebalMode[eWhitebalMode])) != tPvErr::ePvErrSuccess)
						{
							eWhitebalMode = EDeviceAlliedGigEWhitebalMode_Manual;

							if(PvAttrEnumSet(m_hDevice, "WhitebalMode", CStringA(g_lpszAlliedGigEWhitebalMode[eWhitebalMode])) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("WhitebalMode"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetWhitebalMode(eWhitebalMode))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("WhitebalMode"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "WhitebalValueBlue"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "WhitebalValueBlue"))
					{
						if(GetWhitebalValueBlue(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("WhitebalValueBlue"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "WhitebalValueBlue", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 190;

							if(PvAttrUint32Set(m_hDevice, "WhitebalValueBlue", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("WhitebalValueBlue"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetWhitebalValueBlue(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("WhitebalValueBlue"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "WhitebalValueRed"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "WhitebalValueRed"))
					{
						if(GetWhitebalValueRed(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("WhitebalValueRed"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "WhitebalValueRed", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 105;

							if(PvAttrUint32Set(m_hDevice, "WhitebalValueRed", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("WhitebalValueRed"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetWhitebalValueRed(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("WhitebalValueRed"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "WhitebalAutoAdjustTol"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "WhitebalAutoAdjustTol"))
					{
						if(GetWhitebalAutoAdjustTol(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("WhitebalAutoAdjustTol"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "WhitebalAutoAdjustTol", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 5;

							if(PvAttrUint32Set(m_hDevice, "WhitebalAutoAdjustTol", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("WhitebalAutoAdjustTol"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetWhitebalAutoAdjustTol(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("WhitebalAutoAdjustTol"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "WhitebalAutoRate"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "WhitebalAutoRate"))
					{
						if(GetWhitebalAutoRate(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("WhitebalAutoRate"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "WhitebalAutoRate", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 100;

							if(PvAttrUint32Set(m_hDevice, "WhitebalAutoRate", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("WhitebalAutoRate"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetWhitebalAutoRate(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("WhitebalAutoRate"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "BandwidthCtrlMode"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "BandwidthCtrlMode"))
					{
						EDeviceAlliedGigEBandwidthCtrlMode eBandwidthCtrlMode = EDeviceAlliedGigEBandwidthCtrlMode_StreamBytesPerSecond;

						if(GetBandwidthCtrlMode(&eBandwidthCtrlMode))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("BandwidthCtrlMode"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrEnumSet(m_hDevice, "BandwidthCtrlMode", CStringA(g_lpszAlliedGigEBandwidthCtrlMode[eBandwidthCtrlMode])) != tPvErr::ePvErrSuccess)
						{
							eBandwidthCtrlMode = EDeviceAlliedGigEBandwidthCtrlMode_StreamBytesPerSecond;

							if(PvAttrEnumSet(m_hDevice, "BandwidthCtrlMode", CStringA(g_lpszAlliedGigEBandwidthCtrlMode[eBandwidthCtrlMode])) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("BandwidthCtrlMode"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetBandwidthCtrlMode(eBandwidthCtrlMode))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("BandwidthCtrlMode"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "HeartbeatInterval"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "HeartbeatInterval"))
					{
						if(GetHeartbeatInterval(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("HeartbeatInterval"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "HeartbeatInterval", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 3100;

							if(PvAttrUint32Set(m_hDevice, "HeartbeatInterval", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("HeartbeatInterval"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetHeartbeatInterval(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("HeartbeatInterval"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "HeartbeatTimeout"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "HeartbeatTimeout"))
					{
						if(GetHeartbeatTimeout(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("HeartbeatTimeout"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "HeartbeatTimeout", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 3100;

							if(PvAttrUint32Set(m_hDevice, "HeartbeatTimeout", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("HeartbeatTimeout"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetHeartbeatTimeout(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("HeartbeatTimeout"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "GvcpRetries"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "GvcpRetries"))
					{
						if(GetGvcpRetries(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("GvcpRetries"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "GvcpRetries", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 3100;

							if(PvAttrUint32Set(m_hDevice, "GvcpRetries", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("GvcpRetries"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetGvcpRetries(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("GvcpRetries"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "GvspLookbackWindow"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "GvspLookbackWindow"))
					{
						if(GetGvspLookbackWindow(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("GvspLookbackWindow"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "GvspLookbackWindow", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 6000;

							if(PvAttrUint32Set(m_hDevice, "GvspLookbackWindow", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("GvspLookbackWindow"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetGvspLookbackWindow(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("GvspLookbackWindow"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "GvspResendPercent"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "GvspResendPercent"))
					{
						if(GetGvspResendPercent(&fDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("GvspResendPercent"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrFloat32Set(m_hDevice, "GvspResendPercent", fDatabaseData) != tPvErr::ePvErrSuccess)
						{
							fDatabaseData = 1.000000;

							if(PvAttrFloat32Set(m_hDevice, "GvspResendPercent", fDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("GvspResendPercent"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetGvspResendPercent(fDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("GvspResendPercent"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "GvspRetries"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "GvspRetries"))
					{
						if(GetGvspRetries(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("GvspRetries"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "GvspRetries", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 3100;

							if(PvAttrUint32Set(m_hDevice, "GvspRetries", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("GvspRetries"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetGvspRetries(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("GvspRetries"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "GvspSocketBuffersCount"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "GvspSocketBuffersCount"))
					{
						EDeviceAlliedGigEGvspSocketBuffersCount eGvspSocketBuffersCount = EDeviceAlliedGigEGvspSocketBuffersCount_256;

						if(GetGvspSocketBuffersCount(&eGvspSocketBuffersCount))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("GvspSocketBuffersCount"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrEnumSet(m_hDevice, "GvspSocketBuffersCount", CStringA(g_lpszAlliedGigEGvspSocketBuffersCount[eGvspSocketBuffersCount])) != tPvErr::ePvErrSuccess)
						{
							eGvspSocketBuffersCount = EDeviceAlliedGigEGvspSocketBuffersCount_256;

							if(PvAttrEnumSet(m_hDevice, "GvspSocketBuffersCount", CStringA(g_lpszAlliedGigEGvspSocketBuffersCount[eGvspSocketBuffersCount])) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("GvspSocketBuffersCount"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetGvspSocketBuffersCount(eGvspSocketBuffersCount))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("GvspSocketBuffersCount"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "GvspTimeout"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "GvspTimeout"))
					{
						if(GetGvspTimeout(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("GvspTimeout"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "GvspTimeout", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 1;

							if(PvAttrUint32Set(m_hDevice, "GvspTimeout", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("GvspTimeout"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetGvspTimeout(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("GvspTimeout"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "Strobe1ControlledDuration"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "Strobe1ControlledDuration"))
					{
						EDeviceAlliedGigESwitch eStrobe1ControlledDuration = EDeviceAlliedGigESwitch_Off;

						if(GetStrobe1ControlledDuration(&eStrobe1ControlledDuration))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Strobe1ControlledDuration"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrEnumSet(m_hDevice, "Strobe1ControlledDuration", CStringA(g_lpszAlliedGigESwitch[eStrobe1ControlledDuration])) != tPvErr::ePvErrSuccess)
						{
							eStrobe1ControlledDuration = EDeviceAlliedGigESwitch_Off;

							if(PvAttrEnumSet(m_hDevice, "Strobe1ControlledDuration", CStringA(g_lpszAlliedGigESwitch[eStrobe1ControlledDuration])) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Strobe1ControlledDuration"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetStrobe1ControlledDuration(eStrobe1ControlledDuration))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Strobe1ControlledDuration"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "Strobe1Delay"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "Strobe1Delay"))
					{
						if(GetStrobe1Delay(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Strobe1Delay"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "Strobe1Delay", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 0;

							if(PvAttrUint32Set(m_hDevice, "Strobe1Delay", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Strobe1Delay"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetStrobe1Delay(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Strobe1Delay"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "Strobe1Duration"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "Strobe1Duration"))
					{
						if(GetStrobe1Duration(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Strobe1Duration"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "Strobe1Duration", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 0;

							if(PvAttrUint32Set(m_hDevice, "Strobe1Duration", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Strobe1Duration"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetStrobe1Duration(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Strobe1Duration"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "Strobe1Mode"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "Strobe1Mode"))
					{
						EDeviceAlliedGigEStrobeMode eStrobe1Mode = EDeviceAlliedGigEStrobeMode_FrameTriggerReady;

						if(GetStrobe1Mode(&eStrobe1Mode))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Strobe1Mode"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrEnumSet(m_hDevice, "Strobe1Mode", CStringA(g_lpszAlliedGigEStrobeMode[eStrobe1Mode])) != tPvErr::ePvErrSuccess)
						{
							eStrobe1Mode = EDeviceAlliedGigEStrobeMode_FrameTriggerReady;

							if(PvAttrEnumSet(m_hDevice, "Strobe1Mode", CStringA(g_lpszAlliedGigEStrobeMode[eStrobe1Mode])) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Strobe1Mode"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetStrobe1Mode(eStrobe1Mode))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Strobe1Mode"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "SyncIn1GlitchFilter"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "SyncIn1GlitchFilter"))
					{
						if(GetSyncIn1GlitchFilter(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("SyncIn1GlitchFilter"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "SyncIn1GlitchFilter", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 0;

							if(PvAttrUint32Set(m_hDevice, "SyncIn1GlitchFilter", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("SyncIn1GlitchFilter"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetSyncIn1GlitchFilter(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("SyncIn1GlitchFilter"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "SyncIn2GlitchFilter"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "SyncIn2GlitchFilter"))
					{
						if(GetSyncIn2GlitchFilter(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("SyncIn2GlitchFilter"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "SyncIn2GlitchFilter", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 0;

							if(PvAttrUint32Set(m_hDevice, "SyncIn2GlitchFilter", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("SyncIn2GlitchFilter"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetSyncIn2GlitchFilter(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("SyncIn2GlitchFilter"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "SyncOut1Invert"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "SyncOut1Invert"))
					{
						EDeviceAlliedGigESwitch eSyncOut1Invert = EDeviceAlliedGigESwitch_Off;

						if(GetSyncOut1Invert(&eSyncOut1Invert))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("SyncOut1Invert"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrEnumSet(m_hDevice, "SyncOut1Invert", CStringA(g_lpszAlliedGigESwitch[eSyncOut1Invert])) != tPvErr::ePvErrSuccess)
						{
							eSyncOut1Invert = EDeviceAlliedGigESwitch_Off;

							if(PvAttrEnumSet(m_hDevice, "SyncOut1Invert", CStringA(g_lpszAlliedGigESwitch[eSyncOut1Invert])) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("SyncOut1Invert"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetSyncOut1Invert(eSyncOut1Invert))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("SyncOut1Invert"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "SyncOut1Mode"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "SyncOut1Mode"))
					{
						EDeviceAlliedGigESyncOutMode eSyncOut1Mode = EDeviceAlliedGigESyncOutMode_FrameTriggerReady;

						if(GetSyncOut1Mode(&eSyncOut1Mode))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("SyncOut1Mode"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrEnumSet(m_hDevice, "SyncOut1Mode", CStringA(g_lpszAlliedGigESyncOutMode[eSyncOut1Mode])) != tPvErr::ePvErrSuccess)
						{
							eSyncOut1Mode = EDeviceAlliedGigESyncOutMode_FrameTriggerReady;

							if(PvAttrEnumSet(m_hDevice, "SyncOut1Mode", CStringA(g_lpszAlliedGigESyncOutMode[eSyncOut1Mode])) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("SyncOut1Mode"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetSyncOut1Mode(eSyncOut1Mode))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("SyncOut1Mode"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "SyncOut2Invert"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "SyncOut2Invert"))
					{
						EDeviceAlliedGigESwitch eSyncOut2Invert = EDeviceAlliedGigESwitch_Off;

						if(GetSyncOut2Invert(&eSyncOut2Invert))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("SyncOut2Invert"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrEnumSet(m_hDevice, "SyncOut2Invert", CStringA(g_lpszAlliedGigESwitch[eSyncOut2Invert])) != tPvErr::ePvErrSuccess)
						{
							eSyncOut2Invert = EDeviceAlliedGigESwitch_Off;

							if(PvAttrEnumSet(m_hDevice, "SyncOut2Invert", CStringA(g_lpszAlliedGigESwitch[eSyncOut2Invert])) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("SyncOut2Invert"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetSyncOut2Invert(eSyncOut2Invert))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("SyncOut2Invert"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "SyncOut2Mode"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "SyncOut2Mode"))
					{
						EDeviceAlliedGigESyncOutMode eSyncOut2Mode = EDeviceAlliedGigESyncOutMode_FrameTriggerReady;

						if(GetSyncOut2Mode(&eSyncOut2Mode))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("SyncOut2Mode"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrEnumSet(m_hDevice, "SyncOut2Mode", CStringA(g_lpszAlliedGigESyncOutMode[eSyncOut2Mode])) != tPvErr::ePvErrSuccess)
						{
							eSyncOut2Mode = EDeviceAlliedGigESyncOutMode_FrameTriggerReady;

							if(PvAttrEnumSet(m_hDevice, "SyncOut2Mode", CStringA(g_lpszAlliedGigESyncOutMode[eSyncOut2Mode])) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("SyncOut2Mode"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetSyncOut2Mode(eSyncOut2Mode))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("SyncOut2Mode"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "SyncOutGpoLevels"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "SyncOutGpoLevels"))
					{
						if(GetSyncOutGpoLevels(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("SyncOutGpoLevels"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "SyncOutGpoLevels", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 0;

							if(PvAttrUint32Set(m_hDevice, "SyncOutGpoLevels", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("SyncOutGpoLevels"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetSyncOutGpoLevels(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("SyncOutGpoLevels"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "StreamBytesPerSecond"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "StreamBytesPerSecond"))
					{
						if(GetStreamBytesPerSecond(&nDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("StreamBytesPerSecond"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrUint32Set(m_hDevice, "StreamBytesPerSecond", nDatabaseData) != tPvErr::ePvErrSuccess)
						{
							nDatabaseData = 0;

							if(PvAttrUint32Set(m_hDevice, "StreamBytesPerSecond", nDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("StreamBytesPerSecond"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetStreamBytesPerSecond(nDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("StreamBytesPerSecond"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "StreamFrameRateConstrain"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "StreamFrameRateConstrain"))
					{
						if(GetStreamFrameRateConstrain(&bDatabaseData))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("StreamFrameRateConstrain"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrBooleanSet(m_hDevice, "StreamFrameRateConstrain", bDatabaseData) != tPvErr::ePvErrSuccess)
						{
							bDatabaseData = false;

							if(PvAttrBooleanSet(m_hDevice, "StreamFrameRateConstrain", bDatabaseData) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("StreamFrameRateConstrain"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetStreamFrameRateConstrain(bDatabaseData))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("StreamFrameRateConstrain"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}

				if(!PvAttrExists(m_hDevice, "StreamHoldEnable"))
				{
					if(!PvAttrIsAvailable(m_hDevice, "StreamHoldEnable"))
					{
						EDeviceAlliedGigESwitch eStreamHoldEnable = EDeviceAlliedGigESwitch_Off;

						if(GetStreamHoldEnable(&eStreamHoldEnable))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("StreamHoldEnable"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}
						if(PvAttrEnumSet(m_hDevice, "StreamHoldEnable", CStringA(g_lpszAlliedGigESwitch[eStreamHoldEnable])) != tPvErr::ePvErrSuccess)
						{
							eStreamHoldEnable = EDeviceAlliedGigESwitch_Off;

							if(PvAttrEnumSet(m_hDevice, "StreamHoldEnable", CStringA(g_lpszAlliedGigESwitch[eStreamHoldEnable])) != tPvErr::ePvErrSuccess)
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("StreamHoldEnable"));
								eReturn = EDeviceInitializeResult_ReadOnDeviceError;
								break;
							}
							if(SetStreamHoldEnable(eStreamHoldEnable))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("StreamHoldEnable"));
								eReturn = EDeviceInitializeResult_WriteToDatabaseError;
								break;
							}
						}
					}
				}
			}
			break;
		default:
			break;
		}

		if(eReturn != EDeviceInitializeResult_UnknownError)
			break;

		tPvUint32 nSizeX = 0, nSizeY = 0, nChannel = 1, nDepth = 8;

		unsigned long nFrameSize = 0;

		if(PvAttrUint32Get(m_hDevice, "TotalBytesPerFrame", &nFrameSize) != tPvErr::ePvErrSuccess)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("TotalBytesPerFrame"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		if(!pFrames)
			pFrames = new tPvFrame[FRAMESCOUNT];

		memset(pFrames, 0, sizeof(tPvFrame) * FRAMESCOUNT);

		bool bError = false;

		for(int i = 0; i < FRAMESCOUNT; i++)
		{
			pFrames[i].ImageBuffer = new char[nFrameSize];

			if(pFrames[i].ImageBuffer)
			{
				pFrames[i].ImageBufferSize = nFrameSize;
				continue;
			}

			bError = true;
			break;
		}

		if(bError)
		{
			if(pFrames)
			{
				for(int i = 0; i < FRAMESCOUNT; i++)
				{
					if(pFrames[i].ImageBuffer)
					{
						delete[] pFrames[i].ImageBuffer;
						pFrames[i].ImageBuffer = nullptr;
					}
				}

				delete[] pFrames;
				pFrames = nullptr;
			}
			

			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtocreateimagebuffer);
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		m_pFrames = pFrames;
			   
		if(PvAttrUint32Get(m_hDevice, "Width", &nSizeX) != tPvErr::ePvErrSuccess)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Width"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		if(PvAttrUint32Get(m_hDevice, "Height", &nSizeY) != tPvErr::ePvErrSuccess)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Height"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		EDeviceAlliedGigEPixelFormat ePixelFormat = EDeviceAlliedGigEPixelFormat_Count;

		unsigned long nBufferSize = 256;
		unsigned long nReadSize = 0;

		char pBuffer[256] = { 0, };
		memset(pBuffer, 0, sizeof(char) * 256);
		nReadSize = 0;

		if(PvAttrEnumGet(m_hDevice, "PixelFormat", pBuffer, nBufferSize, &nReadSize) == tPvErr::ePvErrSuccess)
		{
			CString strDeviceData(pBuffer);

			for(int i = 0; i < EDeviceAlliedGigEPixelFormat_Count; ++i)
			{
				if(strDeviceData.CompareNoCase(g_lpszAlliedGigEPixelFormat[i]))
					continue;

				ePixelFormat = (EDeviceAlliedGigEPixelFormat)i;

				break;
			}
		}

		if(ePixelFormat == EDeviceAlliedGigEPixelFormat_Count)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("PixelFormat"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		switch(ePixelFormat)
		{
		case EDeviceAlliedGigEPixelFormat_Mono8:
			nChannel = 1;
			nDepth = 8;
			break;
		case EDeviceAlliedGigEPixelFormat_Mono16:
			nChannel = 1;
			nDepth = 16;		
			break;
		case EDeviceAlliedGigEPixelFormat_Bayer8:
		case EDeviceAlliedGigEPixelFormat_Bayer16:
		case EDeviceAlliedGigEPixelFormat_Rgb24:
		case EDeviceAlliedGigEPixelFormat_Yuv411:
		case EDeviceAlliedGigEPixelFormat_Yuv422:
		case EDeviceAlliedGigEPixelFormat_Yuv444:
		case EDeviceAlliedGigEPixelFormat_Bgr24:
		case EDeviceAlliedGigEPixelFormat_Bayer12Packed:
			nChannel = 3;
			nDepth = 8;
			break;
		default:
			break;
		}

		int nMaxPixel = (1 << nDepth) - 1;

		CMultipleVariable mv;
		for(tPvUint32 i = 0; i < nChannel; ++i)
			mv.AddValue(nMaxPixel);
		
		PvCaptureStart(m_hDevice);

		this->InitBuffer(nSizeX, nSizeY, mv, CRavidImage::MakeValueFormat(nChannel, nDepth));
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

EDeviceTerminateResult CDeviceAlliedGigE::Terminate()
{
	EDeviceTerminateResult eReturn = EDeviceTerminateResult_UnknownError;

	CString strMessage;

	SetStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("DalsaGenieNano"));

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

		PvCaptureEnd(m_hDevice);

		tPvFrame* pFrames = (tPvFrame*)m_pFrames;

		if(pFrames)
		{
			for(int i = 0; i < FRAMESCOUNT; i++)
			{
				if(pFrames[i].ImageBuffer)
				{
					delete[] pFrames[i].ImageBuffer;
					pFrames[i].ImageBuffer = nullptr;
				}
			}

			delete[] pFrames;
			pFrames = nullptr;
		}

		m_pFrames = nullptr;

		if(m_hDevice)
		{
			PvCameraClose(m_hDevice);
			m_hDevice = nullptr;
		}

		bool bNeedInitializeAPI = true;

		size_t szDeviceCount = CDeviceManager::GetDeviceCount();

		for(size_t i = 0; i < szDeviceCount; ++i)
		{
			CDeviceAlliedGigE* pDeviceCamera = dynamic_cast<CDeviceAlliedGigE*>(CDeviceManager::GetDeviceByIndex(typeid(CDeviceAlliedGigE), i));
			if(!pDeviceCamera)
				continue;

			if(pDeviceCamera == this)
				continue;

			if(!pDeviceCamera->IsInitialized())
				continue;

			bNeedInitializeAPI = false;
			break;
		}

		if(bNeedInitializeAPI)
			PvUnInitialize();

		strMessage = CMultiLanguageManager::GetString(ELanguageParameter_TheDevicehasbeensuccessfullyterminated);

		CEventHandlerManager::BroadcastOnDeviceTerminated(this);
	}
	while(false);

	this->ClearBuffer();

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceAlliedGigE::LoadSettings()
{
	bool bReturn = false;

	m_vctParameterFieldConfigurations.clear();

	do
	{
		CString strLimit;
		strLimit.Format(_T("%d"), LONG_MAX);

		AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_DeviceID, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_DeviceID], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 0);
		
		AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_DeviceFindWaittingTime, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_DeviceFindWaittingTime], _T("300"), EParameterFieldType_Edit, nullptr, nullptr, 0);

		AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_GrabWaittingTime, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_GrabWaittingTime], strLimit, EParameterFieldType_Edit, nullptr, nullptr, 0);

		AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_InitializeType, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_InitializeType], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAlliedGigEInitializeType, EDeviceAlliedGigEInitializeType_Count), nullptr, 0);

		AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_Width, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_Width], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_Height, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_Height], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_RegionX, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_RegionX], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_RegionY, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_RegionY], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_BinningX, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_BinningX], _T("1"), EParameterFieldType_Edit, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_BinningY, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_BinningY], _T("1"), EParameterFieldType_Edit, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_DecimationHorizontal, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_DecimationHorizontal], _T("1"), EParameterFieldType_Edit, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_DecimationVertical, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_DecimationVertical], _T("1"), EParameterFieldType_Edit, nullptr, nullptr, 0);		
		AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_PixelFormat, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_PixelFormat], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAlliedGigEPixelFormat, EDeviceAlliedGigEPixelFormat_Count), nullptr, 0);		
		AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_PacketSize, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_PacketSize], _T("8228"), EParameterFieldType_Edit, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_AcquisitionInfo, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_AcquisitionInfo], _T("0"), EParameterFieldType_None, nullptr, nullptr, 0);
		{

			AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_Acquisition, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_Acquisition], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);
			{
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_AcqEndTriggerEvent, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_AcqEndTriggerEvent], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAlliedGigETriggerEvent, EDeviceAlliedGigETriggerEvent_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_AcqEndTriggerMode, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_AcqEndTriggerMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAlliedGigETriggerMode, EDeviceAlliedGigETriggerMode_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_AcqRecTriggerEvent, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_AcqRecTriggerEvent], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAlliedGigETriggerEvent, EDeviceAlliedGigETriggerEvent_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_AcqRecTriggerMode, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_AcqRecTriggerMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAlliedGigETriggerMode, EDeviceAlliedGigETriggerMode_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_AcqStartTriggerEvent, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_AcqStartTriggerEvent], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAlliedGigETriggerEvent, EDeviceAlliedGigETriggerEvent_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_AcqStartTriggerMode, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_AcqStartTriggerMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAlliedGigETriggerMode, EDeviceAlliedGigETriggerMode_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_FrameRate, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_FrameRate], _T("30.029"), EParameterFieldType_Edit, nullptr, _T("Float"), 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_FrameStartTriggerDelay, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_FrameStartTriggerDelay], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_FrameStartTriggerEvent, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_FrameStartTriggerEvent], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAlliedGigETriggerEvent, EDeviceAlliedGigETriggerEvent_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_FrameStartTriggerMode, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_FrameStartTriggerMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAlliedGigEFrameMode, EDeviceAlliedGigEFrameMode_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_FrameStartTriggerOverlap, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_FrameStartTriggerOverlap], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAlliedGigETriggerOverlap, EDeviceAlliedGigETriggerOverlap_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_AcquisitionFrameCount, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_AcquisitionFrameCount], _T("1"), EParameterFieldType_Edit, nullptr, nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_AcquisitionMode, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_AcquisitionMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAlliedGigEAcquisitionMode, EDeviceAlliedGigEAcquisitionMode_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_RecorderPreEventCount, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_RecorderPreEventCount], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
			}
		}

		AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_ControlInfo, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_ControlInfo], _T("0"), EParameterFieldType_None, nullptr, nullptr, 0);
		{
			AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_Color, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_Color], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);
			{
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_ColorTransformationMode, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_ColorTransformationMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAlliedGigEColorTransformationMode, EDeviceAlliedGigEColorTransformationMode_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_ColorTransformationValueBB, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_ColorTransformationValueBB], _T("1.400"), EParameterFieldType_Edit, nullptr, _T("Float"), 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_ColorTransformationValueBG, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_ColorTransformationValueBG], _T("-0.480"), EParameterFieldType_Edit, nullptr, _T("Float"), 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_ColorTransformationValueBR, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_ColorTransformationValueBR], _T("0.080"), EParameterFieldType_Edit, nullptr, _T("Float"), 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_ColorTransformationValueGB, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_ColorTransformationValueGB], _T("-0.170"), EParameterFieldType_Edit, nullptr, _T("Float"), 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_ColorTransformationValueGG, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_ColorTransformationValueGG], _T("1.180"), EParameterFieldType_Edit, nullptr, _T("Float"), 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_ColorTransformationValueGR, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_ColorTransformationValueGR], _T("-0.010"), EParameterFieldType_Edit, nullptr, _T("Float"), 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_ColorTransformationValueRB, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_ColorTransformationValueRB], _T("0.180"), EParameterFieldType_Edit, nullptr, _T("Float"), 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_ColorTransformationValueRG, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_ColorTransformationValueRG], _T("-0.170"), EParameterFieldType_Edit, nullptr, _T("Float"), 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_ColorTransformationValueRR, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_ColorTransformationValueRR], _T("0.990"), EParameterFieldType_Edit, nullptr, _T("Float"), 2);
			}

			AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_DSP, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_DSP], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);
			{
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_DSPSubregionBottom, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_DSPSubregionBottom], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_DSPSubregionLeft, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_DSPSubregionLeft], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_DSPSubregionRight, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_DSPSubregionRight], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_DSPSubregionTop, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_DSPSubregionTop], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
			}

			AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_Exposure, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_Exposure], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);
			{
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_ExposureMode, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_ExposureMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAlliedGigEExposureMode, EDeviceAlliedGigEExposureMode_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_ExposureValue, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_ExposureValue], _T("15000"), EParameterFieldType_Edit, nullptr, nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_ExposureAutoAdjustTol, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_ExposureAutoAdjustTol], _T("5"), EParameterFieldType_Edit, nullptr, nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_ExposureAutoAlg, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_ExposureAutoAlg], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAlliedGigEExposureAutoAlg, EDeviceAlliedGigEExposureAutoAlg_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_ExposureAutoMax, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_ExposureAutoMax], _T("500000"), EParameterFieldType_Edit, nullptr, nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_ExposureAutoMin, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_ExposureAutoMin], _T("10"), EParameterFieldType_Edit, nullptr, nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_ExposureAutoOutliers, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_ExposureAutoOutliers], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_ExposureAutoRate, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_ExposureAutoRate], _T("100"), EParameterFieldType_Edit, nullptr, nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_ExposureAutoTarget, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_ExposureAutoTarget], _T("50"), EParameterFieldType_Edit, nullptr, nullptr, 2);
			}

			AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_Gain, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_Gain], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);
			{
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_GainMode, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_GainMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAlliedGigEGainMode, EDeviceAlliedGigEGainMode_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_GainValue, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_GainValue], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_GainAutoAdjustTol, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_GainAutoAdjustTol], _T("5"), EParameterFieldType_Edit, nullptr, nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_GainAutoMax, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_GainAutoMax], _T("31"), EParameterFieldType_Edit, nullptr, nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_GainAutoMin, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_GainAutoMin], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_GainAutoOutliers, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_GainAutoOutliers], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_GainAutoRate, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_GainAutoRate], _T("100"), EParameterFieldType_Edit, nullptr, nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_GainAutoTarget, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_GainAutoTarget], _T("50"), EParameterFieldType_Edit, nullptr, nullptr, 2);
			}

			AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_Iris, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_Iris], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);
			{
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_IrisAutoTarget, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_IrisAutoTarget], _T("50"), EParameterFieldType_Edit, nullptr, nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_IrisMode, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_IrisMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAlliedGigEIrisMode, EDeviceAlliedGigEIrisMode_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_IrisVideoLevelMax, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_IrisVideoLevelMax], _T("110"), EParameterFieldType_Edit, nullptr, nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_IrisVideoLevelMin, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_IrisVideoLevelMin], _T("90"), EParameterFieldType_Edit, nullptr, nullptr, 2);
			}

			AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_LUTControl, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_LUTControl], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);
			{
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_LUTEnable, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_LUTEnable], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_LUTIndex, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_LUTIndex], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_LUTMode, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_LUTMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAlliedGigELUTMode, EDeviceAlliedGigELUTMode_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_LUTSelector, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_LUTSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAlliedGigELUTSelector, EDeviceAlliedGigELUTSelector_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_LUTValue, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_LUTValue], _T("4095"), EParameterFieldType_Edit, nullptr, nullptr, 2);
			}

			AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_ETC, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_ETC], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);
			{
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_EdgeFilter, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_EdgeFilter], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAlliedGigEEdgeFilter, EDeviceAlliedGigEEdgeFilter_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_Gamma, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_Gamma], _T("1.000"), EParameterFieldType_Edit, nullptr, _T("Float"), 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_Hue, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_Hue], _T("0.000"), EParameterFieldType_Edit, nullptr, _T("Float"), 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_OffsetValue, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_OffsetValue], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_Saturation, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_Saturation], _T("1.000"), EParameterFieldType_Edit, nullptr, _T("Float"), 2);
			}

			AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_Whitebalance, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_Whitebalance], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);
			{
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_WhitebalMode, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_WhitebalMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAlliedGigEWhitebalMode, EDeviceAlliedGigEWhitebalMode_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_WhitebalValueBlue, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_WhitebalValueBlue], _T("190"), EParameterFieldType_Edit, nullptr, nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_WhitebalValueRed, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_WhitebalValueRed], _T("105"), EParameterFieldType_Edit, nullptr, nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_WhitebalAutoAdjustTol, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_WhitebalAutoAdjustTol], _T("5"), EParameterFieldType_Edit, nullptr, nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_WhitebalAutoRate, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_WhitebalAutoRate], _T("100"), EParameterFieldType_Edit, nullptr, nullptr, 2);
			}
		}


		AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_GigEInfo, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_GigEInfo], _T("0"), EParameterFieldType_None, nullptr, nullptr, 0);
		{
			AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_BandwidthCtrlMode, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_BandwidthCtrlMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAlliedGigEBandwidthCtrlMode, EDeviceAlliedGigEBandwidthCtrlMode_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_HeartbeatInterval, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_HeartbeatInterval], _T("3500"), EParameterFieldType_Edit, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_HeartbeatTimeout, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_HeartbeatTimeout], _T("6000"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_GVSP, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_GVSP], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);
			{
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_GvcpRetries, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_GvcpRetries], _T("5"), EParameterFieldType_Edit, nullptr, nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_GvspLookbackWindow, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_GvspLookbackWindow], _T("25"), EParameterFieldType_Edit, nullptr, nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_GvspResendPercent, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_GvspResendPercent], _T("1.000"), EParameterFieldType_Edit, nullptr, _T("Float"), 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_GvspRetries, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_GvspRetries], _T("3"), EParameterFieldType_Edit, nullptr, nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_GvspSocketBuffersCount, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_GvspSocketBuffersCount], _T("1"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAlliedGigEGvspSocketBuffersCount, EDeviceAlliedGigEGvspSocketBuffersCount_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_GvspTimeout, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_GvspTimeout], _T("50"), EParameterFieldType_Edit, nullptr, nullptr, 2);
			}

			AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_StreamBytesPerSecond, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_StreamBytesPerSecond], _T("115000000"), EParameterFieldType_Edit, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_StreamFrameRateConstrain, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_StreamFrameRateConstrain], _T("1"), EParameterFieldType_Check, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_StreamHoldEnable, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_StreamHoldEnable], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAlliedGigESwitch, EDeviceAlliedGigESwitch_Count), nullptr, 1);
		}
		AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_IOInfo, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_IOInfo], _T("0"), EParameterFieldType_None, nullptr, nullptr, 0);
		{
			AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_Strobe, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_Strobe], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);
			{
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_Strobe1ControlledDuration, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_Strobe1ControlledDuration], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAlliedGigESwitch, EDeviceAlliedGigESwitch_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_Strobe1Delay, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_Strobe1Delay], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_Strobe1Duration, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_Strobe1Duration], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_Strobe1Mode, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_Strobe1Mode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAlliedGigEStrobeMode, EDeviceAlliedGigEStrobeMode_Count), nullptr, 2);
			}

			AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_Sync, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_Sync], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);
			{
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_SyncIn1GlitchFilter, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_SyncIn1GlitchFilter], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_SyncIn2GlitchFilter, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_SyncIn2GlitchFilter], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_SyncOut1Invert, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_SyncOut1Invert], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAlliedGigESwitch, EDeviceAlliedGigESwitch_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_SyncOut1Mode, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_SyncOut1Mode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAlliedGigESyncOutMode, EDeviceAlliedGigESyncOutMode_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_SyncOut2Invert, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_SyncOut2Invert], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAlliedGigESwitch, EDeviceAlliedGigESwitch_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_SyncOut2Mode, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_SyncOut2Mode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAlliedGigESyncOutMode, EDeviceAlliedGigESyncOutMode_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAlliedGigE_SyncOutGpoLevels, g_lpszParamAlliedGigE[EDeviceParameterAlliedGigE_SyncOutGpoLevels], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
			}
		}

		bReturn = true;
	}
	while(false);

	return bReturn & __super::LoadSettings();
}

EDeviceGrabResult CDeviceAlliedGigE::Grab()
{
	EDeviceGrabResult eReturn = EDeviceGrabResult_UnknownError;

	CString strMessage;

	do
	{
		m_nGrabCount = 0;

		unsigned int nRequestGrabCount = 0;

		if(!IsInitialized())
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Ithasnotbeeninitializedyet);
			eReturn = EDeviceGrabResult_NotInitializedError;
			break;
		}

		if(IsLive() || !IsGrabAvailable())
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Ithasbeenlivingorgrabbing);
			eReturn = EDeviceGrabResult_AlreadyGrabError;
			break;
		}

		tPvFrame* pFrames = (tPvFrame*)m_pFrames;
		if(!pFrames)
			break;

		bool bError = false;

		for(int i = 0; i < FRAMESCOUNT; i++)
		{
			if(PvCaptureQueueFrame(m_hDevice, &(pFrames[i]), NULL) != tPvErr::ePvErrSuccess)
			{
				bError = true;
				break;
			}
		}

		if(bError)
			break;

		if(PvCommandRun(m_hDevice,"AcquisitionStart") != tPvErr::ePvErrSuccess)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("AcquisitionStart"));
			eReturn = EDeviceGrabResult_AlreadyGrabError;
			break;
		}

		m_nGrabCount = nRequestGrabCount;

		m_bIsGrabAvailable = false;

		m_pLiveThread = AfxBeginThread(CDeviceAlliedGigE::CallbackFunction, this, 0, 0U, CREATE_SUSPENDED);

		if(!m_pLiveThread)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtocreatethread);
			eReturn = EDeviceGrabResult_CreateThreadError;
			break;
		}

		m_pLiveThread->ResumeThread();

		m_bIsGrabAvailable = false;

		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("Grab"));

		eReturn = EDeviceGrabResult_OK;
	}
	while(false);

	return eReturn;
}

EDeviceLiveResult CDeviceAlliedGigE::Live()
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

		tPvFrame* pFrames = (tPvFrame*)m_pFrames;
		if(!pFrames)
			break;

		bool bError = false;

		for(int i = 0; i < FRAMESCOUNT; i++)
		{
			if(PvCaptureQueueFrame(m_hDevice, &(pFrames[i]), NULL) != tPvErr::ePvErrSuccess)
			{
				bError = true;
				break;
			}
		}

		if(bError)
			break;

		if(PvCommandRun(m_hDevice, "AcquisitionStart") != tPvErr::ePvErrSuccess)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("AcquisitionStart"));
			eReturn = EDeviceLiveResult_AlreadyGrabError;
			break;
		}

		m_pLiveThread = AfxBeginThread(CDeviceAlliedGigE::CallbackFunction, this, 0, 0U, CREATE_SUSPENDED);

		if(!m_pLiveThread)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtocreatethread);
			eReturn = EDeviceLiveResult_CreateThreadError;
			break;
		}

		m_pLiveThread->ResumeThread();

		m_bIsLive = true;

		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("Live"));

		eReturn = EDeviceLiveResult_OK;
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceStopResult CDeviceAlliedGigE::Stop()
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

		m_bIsGrabAvailable = true;

		if(PvCommandRun(m_hDevice, "AcquisitionStop")!= tPvErr::ePvErrSuccess)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Acquisition Stop"));
			eReturn = EDeviceStopResult_WriteToDeviceError;
			break;
		}

		if(WaitForSingleObject(m_pLiveThread->m_hThread, 1000) == WAIT_TIMEOUT)
		{


		}
		
		if(PvCaptureQueueClear(m_hDevice) != ePvErrSuccess)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Queue Clear"));
			eReturn = EDeviceStopResult_WriteToDeviceError;
			break;
		}

		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("Stop"));

		eReturn = EDeviceStopResult_OK;
	} 
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceTriggerResult CDeviceAlliedGigE::Trigger()
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

		if(PvCommandRun(m_hDevice, "FrameStartTriggerSoftware") != tPvErr::ePvErrSuccess)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("FrameStartTirggerSoftware"));
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

EAlliedGigEGetFunction CDeviceAlliedGigE::GetDeviceFindWaittingTime(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_DeviceFindWaittingTime));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetDeviceFindWaittingTime(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_DeviceFindWaittingTime;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetGrabWaittingTime(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_GrabWaittingTime));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetGrabWaittingTime(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_GrabWaittingTime;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetInitializeType(EDeviceAlliedGigEInitializeType * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAlliedGigEInitializeType)_ttoi(GetParamValue(EDeviceParameterAlliedGigE_InitializeType));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetInitializeType(EDeviceAlliedGigEInitializeType eParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_InitializeType;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < EDeviceAlliedGigEInitializeType_OnlyParam || eParam >= EDeviceAlliedGigEInitializeType_Count)
		{
			eReturn = EAlliedGigESetFunction_NotSupportedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], g_lpszAlliedGigEInitializeType[nPreValue], g_lpszAlliedGigEInitializeType[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetCanvasWidth(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_Width));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetCanvasWidth(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_Width;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EAlliedGigESetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetCanvasHeight(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_Height));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetCanvasHeight(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_Height;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EAlliedGigESetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetRegionX(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_RegionX));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetRegionX(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_RegionX;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrUint32Set(m_hDevice, "RegionX", nParam) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetRegionY(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_RegionY));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetRegionY(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_RegionY;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrUint32Set(m_hDevice, "RegionY", nParam) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetBinningX(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_BinningX));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetBinningX(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_BinningX;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EAlliedGigESetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetBinningY(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_BinningY));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetBinningY(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_BinningY;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EAlliedGigESetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetDecimationHorizontal(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_DecimationHorizontal));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetDecimationHorizontal(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_DecimationHorizontal;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "DecimationHorizontal") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "DecimationHorizontal") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvUint32 nMin = 0, nMax = 0;

			if(PvAttrRangeUint32(m_hDevice, "DecimationHorizontal", &nMin, &nMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(nMin > (tPvUint32)nParam || nMax < (tPvUint32)nParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrUint32Set(m_hDevice, "DecimationHorizontal", nParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetDecimationVertical(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_DecimationVertical));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetDecimationVertical(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_DecimationVertical;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "DecimationVertical") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "DecimationVertical") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvUint32 nMin = 0, nMax = 0;

			if(PvAttrRangeUint32(m_hDevice, "DecimationVertical", &nMin, &nMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(nMin > (tPvUint32)nParam || nMax < (tPvUint32)nParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrUint32Set(m_hDevice, "DecimationVertical", nParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetPixelFormat(EDeviceAlliedGigEPixelFormat * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAlliedGigEPixelFormat)_ttoi(GetParamValue(EDeviceParameterAlliedGigE_PixelFormat));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetPixelFormat(EDeviceAlliedGigEPixelFormat eParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_PixelFormat;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < EDeviceAlliedGigEPixelFormat_Mono8 || eParam >= EDeviceAlliedGigEPixelFormat_Count)
		{
			eReturn = EAlliedGigESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EAlliedGigESetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], g_lpszAlliedGigEPixelFormat[nPreValue], g_lpszAlliedGigEPixelFormat[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetPacketSize(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_PacketSize));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetPacketSize(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_PacketSize;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "PacketSize") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "PacketSize") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvUint32 nMin = 0, nMax = 0;

			if(PvAttrRangeUint32(m_hDevice, "PacketSize", &nMin, &nMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(nMin > (tPvUint32)nParam || nMax < (tPvUint32)nParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrUint32Set(m_hDevice, "PacketSize", nParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetAcqEndTriggerEvent(EDeviceAlliedGigETriggerEvent * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAlliedGigETriggerEvent)_ttoi(GetParamValue(EDeviceParameterAlliedGigE_AcqEndTriggerEvent));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetAcqEndTriggerEvent(EDeviceAlliedGigETriggerEvent eParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_AcqEndTriggerEvent;

	int nPreValue = _ttoi(GetParamValue(eSaveID));
	
	do
	{
		if(eParam < EDeviceAlliedGigETriggerEvent_EdgeRising || eParam >= EDeviceAlliedGigETriggerEvent_Count)
		{
			eReturn = EAlliedGigESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "AcqEndTriggerEvent") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "AcqEndTriggerEvent") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrEnumSet(m_hDevice, "AcqEndTriggerEvent", CStringA(g_lpszAlliedGigETriggerEvent[eParam])))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], g_lpszAlliedGigETriggerEvent[nPreValue], g_lpszAlliedGigETriggerEvent[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetAcqEndTriggerMode(EDeviceAlliedGigETriggerMode * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAlliedGigETriggerMode)_ttoi(GetParamValue(EDeviceParameterAlliedGigE_AcqEndTriggerMode));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetAcqEndTriggerMode(EDeviceAlliedGigETriggerMode eParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_AcqEndTriggerMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < EDeviceAlliedGigETriggerMode_Disabled || eParam >= EDeviceAlliedGigETriggerMode_Count)
		{
			eReturn = EAlliedGigESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "AcqEndTriggerMode") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "AcqEndTriggerMode") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrEnumSet(m_hDevice, "AcqEndTriggerMode", CStringA(g_lpszAlliedGigETriggerMode[eParam])))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], g_lpszAlliedGigETriggerMode[nPreValue], g_lpszAlliedGigETriggerMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetAcqRecTriggerEvent(EDeviceAlliedGigETriggerEvent * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAlliedGigETriggerEvent)_ttoi(GetParamValue(EDeviceParameterAlliedGigE_AcqRecTriggerEvent));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetAcqRecTriggerEvent(EDeviceAlliedGigETriggerEvent eParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_AcqRecTriggerEvent;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < EDeviceAlliedGigETriggerEvent_EdgeRising || eParam >= EDeviceAlliedGigETriggerEvent_Count)
		{
			eReturn = EAlliedGigESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "AcqRecTriggerEvent") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "AcqRecTriggerEvent") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrEnumSet(m_hDevice, "AcqRecTriggerEvent", CStringA(g_lpszAlliedGigETriggerEvent[eParam])))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], g_lpszAlliedGigETriggerEvent[nPreValue], g_lpszAlliedGigETriggerEvent[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetAcqRecTriggerMode(EDeviceAlliedGigETriggerMode * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAlliedGigETriggerMode)_ttoi(GetParamValue(EDeviceParameterAlliedGigE_AcqRecTriggerMode));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetAcqRecTriggerMode(EDeviceAlliedGigETriggerMode eParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_AcqRecTriggerMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < EDeviceAlliedGigETriggerMode_Disabled || eParam >= EDeviceAlliedGigETriggerMode_Count)
		{
			eReturn = EAlliedGigESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "AcqRecTriggerMode") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "AcqRecTriggerMode") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrEnumSet(m_hDevice, "AcqRecTriggerMode", CStringA(g_lpszAlliedGigETriggerMode[eParam])))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], g_lpszAlliedGigETriggerMode[nPreValue], g_lpszAlliedGigETriggerMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetAcqStartTriggerEvent(EDeviceAlliedGigETriggerEvent * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAlliedGigETriggerEvent)_ttoi(GetParamValue(EDeviceParameterAlliedGigE_AcqStartTriggerEvent));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetAcqStartTriggerEvent(EDeviceAlliedGigETriggerEvent eParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_AcqStartTriggerEvent;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < EDeviceAlliedGigETriggerEvent_EdgeRising || eParam >= EDeviceAlliedGigETriggerEvent_Count)
		{
			eReturn = EAlliedGigESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "AcqStartTriggerEvent") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "AcqStartTriggerEvent") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrEnumSet(m_hDevice, "AcqStartTriggerEvent", CStringA(g_lpszAlliedGigETriggerEvent[eParam])))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], g_lpszAlliedGigETriggerEvent[nPreValue], g_lpszAlliedGigETriggerEvent[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetAcqStartTriggerMode(EDeviceAlliedGigETriggerMode * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAlliedGigETriggerMode)_ttoi(GetParamValue(EDeviceParameterAlliedGigE_AcqStartTriggerMode));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetAcqStartTriggerMode(EDeviceAlliedGigETriggerMode eParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_AcqStartTriggerMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < EDeviceAlliedGigETriggerMode_Disabled || eParam >= EDeviceAlliedGigETriggerMode_Count)
		{
			eReturn = EAlliedGigESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "AcqStartTriggerMode") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "AcqStartTriggerMode") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrEnumSet(m_hDevice, "AcqStartTriggerMode", CStringA(g_lpszAlliedGigETriggerMode[eParam])))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], g_lpszAlliedGigETriggerMode[nPreValue], g_lpszAlliedGigETriggerMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetFrameRate(float * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterAlliedGigE_FrameRate));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetFrameRate(float fParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_FrameRate;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "FrameRate") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "FrameRate") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvFloat32 fMin = 0, fMax = 0;

			if(PvAttrRangeFloat32(m_hDevice, "FrameRate", &fMin, &fMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(fMin > fParam || fMax < fParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrFloat32Set(m_hDevice, "FrameRate", fParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetFrameStartTriggerDelay(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_FrameStartTriggerDelay));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetFrameStartTriggerDelay(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_FrameStartTriggerDelay;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "FrameStartTriggerDelay") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "FrameStartTriggerDelay") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvUint32 nMin = 0, nMax = 0;

			if(PvAttrRangeUint32(m_hDevice, "FrameStartTriggerDelay", &nMin, &nMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(nMin > (tPvUint32)nParam || nMax < (tPvUint32)nParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrUint32Set(m_hDevice, "FrameStartTriggerDelay", nParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetFrameStartTriggerEvent(EDeviceAlliedGigETriggerEvent * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAlliedGigETriggerEvent)_ttoi(GetParamValue(EDeviceParameterAlliedGigE_FrameStartTriggerEvent));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetFrameStartTriggerEvent(EDeviceAlliedGigETriggerEvent eParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_FrameStartTriggerEvent;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < EDeviceAlliedGigETriggerEvent_EdgeRising || eParam >= EDeviceAlliedGigETriggerEvent_Count)
		{
			eReturn = EAlliedGigESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "FrameStartTriggerEvent") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "FrameStartTriggerEvent") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrEnumSet(m_hDevice, "FrameStartTriggerEvent", CStringA(g_lpszAlliedGigETriggerEvent[eParam])))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], g_lpszAlliedGigETriggerEvent[nPreValue], g_lpszAlliedGigETriggerEvent[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetFrameStartTriggerMode(EDeviceAlliedGigEFrameMode * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAlliedGigEFrameMode)_ttoi(GetParamValue(EDeviceParameterAlliedGigE_FrameStartTriggerMode));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetFrameStartTriggerMode(EDeviceAlliedGigEFrameMode eParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_FrameStartTriggerMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < EDeviceAlliedGigEFrameMode_Freerun || eParam >= EDeviceAlliedGigEFrameMode_Count)
		{
			eReturn = EAlliedGigESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "FrameStartTriggerMode") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "FrameStartTriggerMode") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrEnumSet(m_hDevice, "FrameStartTriggerMode", CStringA(g_lpszAlliedGigEFrameMode[eParam])))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], g_lpszAlliedGigEFrameMode[nPreValue], g_lpszAlliedGigEFrameMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetFrameStartTriggerOverlap(EDeviceAlliedGigETriggerOverlap * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAlliedGigETriggerOverlap)_ttoi(GetParamValue(EDeviceParameterAlliedGigE_FrameStartTriggerOverlap));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetFrameStartTriggerOverlap(EDeviceAlliedGigETriggerOverlap eParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_FrameStartTriggerOverlap;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < EDeviceAlliedGigETriggerOverlap_Off || eParam >= EDeviceAlliedGigETriggerOverlap_Count)
		{
			eReturn = EAlliedGigESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "FrameStartTriggerOverlap") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "FrameStartTriggerOverlap") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrEnumSet(m_hDevice, "FrameStartTriggerOverlap", CStringA(g_lpszAlliedGigETriggerOverlap[eParam])))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], g_lpszAlliedGigETriggerOverlap[nPreValue], g_lpszAlliedGigETriggerOverlap[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetAcquisitionFrameCount(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_AcquisitionFrameCount));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetAcquisitionFrameCount(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_AcquisitionFrameCount;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "AcquisitionFrameCount") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "AcquisitionFrameCount") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvUint32 nMin = 0, nMax = 0;

			if(PvAttrRangeUint32(m_hDevice, "AcquisitionFrameCount", &nMin, &nMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(nMin > (tPvUint32)nParam || nMax < (tPvUint32)nParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrUint32Set(m_hDevice, "AcquisitionFrameCount", nParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetAcquisitionMode(EDeviceAlliedGigEAcquisitionMode * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAlliedGigEAcquisitionMode)_ttoi(GetParamValue(EDeviceParameterAlliedGigE_AcquisitionMode));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetAcquisitionMode(EDeviceAlliedGigEAcquisitionMode eParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_AcquisitionMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < EDeviceAlliedGigEAcquisitionMode_Continuous || eParam >= EDeviceAlliedGigEAcquisitionMode_Count)
		{
			eReturn = EAlliedGigESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "AcquisitionMode") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "AcquisitionMode") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrEnumSet(m_hDevice, "AcquisitionMode", CStringA(g_lpszAlliedGigEAcquisitionMode[eParam])))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], g_lpszAlliedGigEAcquisitionMode[nPreValue], g_lpszAlliedGigEAcquisitionMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetRecorderPreEventCount(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_RecorderPreEventCount));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetRecorderPreEventCount(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_RecorderPreEventCount;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "RecorderPreEventCount") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "RecorderPreEventCount") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvUint32 nMin = 0, nMax = 0;

			if(PvAttrRangeUint32(m_hDevice, "RecorderPreEventCount", &nMin, &nMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(nMin > (tPvUint32)nParam || nMax < (tPvUint32)nParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrUint32Set(m_hDevice, "RecorderPreEventCount", nParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetColorTransformationMode(EDeviceAlliedGigEColorTransformationMode * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAlliedGigEColorTransformationMode)_ttoi(GetParamValue(EDeviceParameterAlliedGigE_ColorTransformationMode));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetColorTransformationMode(EDeviceAlliedGigEColorTransformationMode eParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_ColorTransformationMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < EDeviceAlliedGigEColorTransformationMode_Off || eParam >= EDeviceAlliedGigEColorTransformationMode_Count)
		{
			eReturn = EAlliedGigESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "ColorTransformationMode") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "ColorTransformationMode") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrEnumSet(m_hDevice, "ColorTransformationMode", CStringA(g_lpszAlliedGigEColorTransformationMode[eParam])))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], g_lpszAlliedGigEColorTransformationMode[nPreValue], g_lpszAlliedGigEColorTransformationMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetColorTransformationValueBB(float * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterAlliedGigE_ColorTransformationValueBB));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetColorTransformationValueBB(float fParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_ColorTransformationValueBB;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "ColorTransformationValueBB") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "ColorTransformationValueBB") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvFloat32 fMin = 0, fMax = 0;

			if(PvAttrRangeFloat32(m_hDevice, "ColorTransformationValueBB", &fMin, &fMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(fMin > fParam || fMax < fParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrFloat32Set(m_hDevice, "ColorTransformationValueBB", fParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetColorTransformationValueBG(float * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterAlliedGigE_ColorTransformationValueBG));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetColorTransformationValueBG(float fParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_ColorTransformationValueBG;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "ColorTransformationValueBG") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "ColorTransformationValueBG") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvFloat32 fMin = 0, fMax = 0;

			if(PvAttrRangeFloat32(m_hDevice, "ColorTransformationValueBG", &fMin, &fMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(fMin > fParam || fMax < fParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrFloat32Set(m_hDevice, "ColorTransformationValueBG", fParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetColorTransformationValueBR(float * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterAlliedGigE_ColorTransformationValueBR));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetColorTransformationValueBR(float fParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_ColorTransformationValueBR;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "ColorTransformationValueBR") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "ColorTransformationValueBR") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvFloat32 fMin = 0, fMax = 0;

			if(PvAttrRangeFloat32(m_hDevice, "ColorTransformationValueBR", &fMin, &fMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(fMin > fParam || fMax < fParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrFloat32Set(m_hDevice, "ColorTransformationValueBR", fParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetColorTransformationValueGB(float * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterAlliedGigE_ColorTransformationValueGB));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetColorTransformationValueGB(float fParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_ColorTransformationValueGB;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "ColorTransformationValueGB") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "ColorTransformationValueGB") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvFloat32 fMin = 0, fMax = 0;

			if(PvAttrRangeFloat32(m_hDevice, "ColorTransformationValueGB", &fMin, &fMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(fMin > fParam || fMax < fParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrFloat32Set(m_hDevice, "ColorTransformationValueGB", fParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetColorTransformationValueGG(float * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterAlliedGigE_ColorTransformationValueGG));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetColorTransformationValueGG(float fParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_ColorTransformationValueGG;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "ColorTransformationValueGG") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "ColorTransformationValueGG") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvFloat32 fMin = 0, fMax = 0;

			if(PvAttrRangeFloat32(m_hDevice, "ColorTransformationValueGG", &fMin, &fMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(fMin > fParam || fMax < fParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrFloat32Set(m_hDevice, "ColorTransformationValueGG", fParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetColorTransformationValueGR(float * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterAlliedGigE_ColorTransformationValueGR));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetColorTransformationValueGR(float fParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_ColorTransformationValueGR;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "ColorTransformationValueGR") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "ColorTransformationValueGR") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvFloat32 fMin = 0, fMax = 0;

			if(PvAttrRangeFloat32(m_hDevice, "ColorTransformationValueGR", &fMin, &fMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(fMin > fParam || fMax < fParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrFloat32Set(m_hDevice, "ColorTransformationValueGR", fParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetColorTransformationValueRB(float * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterAlliedGigE_ColorTransformationValueRB));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetColorTransformationValueRB(float fParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_ColorTransformationValueRB;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "ColorTransformationValueRB") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "ColorTransformationValueRB") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvFloat32 fMin = 0, fMax = 0;

			if(PvAttrRangeFloat32(m_hDevice, "ColorTransformationValueRB", &fMin, &fMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(fMin > fParam || fMax < fParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrFloat32Set(m_hDevice, "ColorTransformationValueRB", fParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetColorTransformationValueRG(float * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterAlliedGigE_ColorTransformationValueRG));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetColorTransformationValueRG(float fParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_ColorTransformationValueRG;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "ColorTransformationValueRG") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "ColorTransformationValueRG") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvFloat32 fMin = 0, fMax = 0;

			if(PvAttrRangeFloat32(m_hDevice, "ColorTransformationValueRG", &fMin, &fMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(fMin > fParam || fMax < fParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrFloat32Set(m_hDevice, "ColorTransformationValueRG", fParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetColorTransformationValueRR(float * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterAlliedGigE_ColorTransformationValueRR));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetColorTransformationValueRR(float fParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_ColorTransformationValueRR;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "ColorTransformationValueRR") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "ColorTransformationValueRR") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvFloat32 fMin = 0, fMax = 0;

			if(PvAttrRangeFloat32(m_hDevice, "ColorTransformationValueRR", &fMin, &fMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(fMin > fParam || fMax < fParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrFloat32Set(m_hDevice, "ColorTransformationValueRR", fParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetDSPSubregionBottom(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_DSPSubregionBottom));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetDSPSubregionBottom(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_DSPSubregionBottom;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "DSPSubregionBottom") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "DSPSubregionBottom") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvUint32 nMin = 0, nMax = 0;

			if(PvAttrRangeUint32(m_hDevice, "DSPSubregionBottom", &nMin, &nMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(nMin > (tPvUint32)nParam || nMax < (tPvUint32)nParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrUint32Set(m_hDevice, "DSPSubregionBottom", nParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetDSPSubregionLeft(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_DSPSubregionLeft));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetDSPSubregionLeft(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_DSPSubregionLeft;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "DSPSubregionLeft") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "DSPSubregionLeft") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvUint32 nMin = 0, nMax = 0;

			if(PvAttrRangeUint32(m_hDevice, "DSPSubregionLeft", &nMin, &nMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(nMin > (tPvUint32)nParam || nMax < (tPvUint32)nParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrUint32Set(m_hDevice, "DSPSubregionLeft", nParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetDSPSubregionRight(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_DSPSubregionRight));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetDSPSubregionRight(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_DSPSubregionRight;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "DSPSubregionRight") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "DSPSubregionRight") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvUint32 nMin = 0, nMax = 0;

			if(PvAttrRangeUint32(m_hDevice, "DSPSubregionRight", &nMin, &nMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(nMin > (tPvUint32)nParam || nMax < (tPvUint32)nParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrUint32Set(m_hDevice, "DSPSubregionRight", nParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetDSPSubregionTop(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_DSPSubregionTop));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetDSPSubregionTop(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_DSPSubregionTop;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "DSPSubregionTop") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "DSPSubregionTop") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvUint32 nMin = 0, nMax = 0;

			if(PvAttrRangeUint32(m_hDevice, "DSPSubregionTop", &nMin, &nMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(nMin > (tPvUint32)nParam || nMax < (tPvUint32)nParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrUint32Set(m_hDevice, "DSPSubregionTop", nParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetExposureMode(EDeviceAlliedGigEExposureMode * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAlliedGigEExposureMode)_ttoi(GetParamValue(EDeviceParameterAlliedGigE_ExposureMode));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}
 
EAlliedGigESetFunction CDeviceAlliedGigE::SetExposureMode(EDeviceAlliedGigEExposureMode eParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_ExposureMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < EDeviceAlliedGigEExposureMode_Manual || eParam >= EDeviceAlliedGigEExposureMode_Count)
		{
			eReturn = EAlliedGigESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "ExposureMode") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "ExposureMode") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrEnumSet(m_hDevice, "ExposureMode", CStringA(g_lpszAlliedGigEExposureMode[eParam])))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], g_lpszAlliedGigEExposureMode[nPreValue], g_lpszAlliedGigEExposureMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetExposureValue(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_ExposureValue));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetExposureValue(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_ExposureValue;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "ExposureValue") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "ExposureValue") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvUint32 nMin = 0, nMax = 0;

			if(PvAttrRangeUint32(m_hDevice, "ExposureValue", &nMin, &nMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(nMin > (tPvUint32)nParam || nMax < (tPvUint32)nParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrUint32Set(m_hDevice, "ExposureValue", nParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetExposureAutoAdjustTol(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_ExposureAutoAdjustTol));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetExposureAutoAdjustTol(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_ExposureAutoAdjustTol;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "ExposureAutoAdjustTol") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "ExposureAutoAdjustTol") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvUint32 nMin = 0, nMax = 0;

			if(PvAttrRangeUint32(m_hDevice, "ExposureAutoAdjustTol", &nMin, &nMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(nMin > (tPvUint32)nParam || nMax < (tPvUint32)nParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrUint32Set(m_hDevice, "ExposureAutoAdjustTol", nParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetExposureAutoAlg(EDeviceAlliedGigEExposureAutoAlg * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAlliedGigEExposureAutoAlg)_ttoi(GetParamValue(EDeviceParameterAlliedGigE_ExposureAutoAlg));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetExposureAutoAlg(EDeviceAlliedGigEExposureAutoAlg eParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_ExposureAutoAlg;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < EDeviceAlliedGigEExposureAutoAlg_Mean || eParam >= EDeviceAlliedGigEExposureAutoAlg_Count)
		{
			eReturn = EAlliedGigESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "ExposureAutoAlg") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "ExposureAutoAlg") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrEnumSet(m_hDevice, "ExposureAutoAlg", CStringA(g_lpszAlliedGigEExposureAutoAlg[eParam])))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], g_lpszAlliedGigEExposureAutoAlg[nPreValue], g_lpszAlliedGigEExposureAutoAlg[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetExposureAutoMax(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_ExposureAutoMax));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetExposureAutoMax(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_ExposureAutoMax;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "ExposureAutoMax") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "ExposureAutoMax") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvUint32 nMin = 0, nMax = 0;

			if(PvAttrRangeUint32(m_hDevice, "ExposureAutoMax", &nMin, &nMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(nMin > (tPvUint32)nParam || nMax < (tPvUint32)nParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrUint32Set(m_hDevice, "ExposureAutoMax", nParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetExposureAutoMin(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_ExposureAutoMin));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetExposureAutoMin(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_ExposureAutoMin;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "ExposureAutoMin") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "ExposureAutoMin") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvUint32 nMin = 0, nMax = 0;

			if(PvAttrRangeUint32(m_hDevice, "ExposureAutoMin", &nMin, &nMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(nMin > (tPvUint32)nParam || nMax < (tPvUint32)nParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrUint32Set(m_hDevice, "ExposureAutoMin", nParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetExposureAutoOutliers(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_ExposureAutoOutliers));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetExposureAutoOutliers(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_ExposureAutoOutliers;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "ExposureAutoOutliers") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "ExposureAutoOutliers") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvUint32 nMin = 0, nMax = 0;

			if(PvAttrRangeUint32(m_hDevice, "ExposureAutoOutliers", &nMin, &nMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(nMin > (tPvUint32)nParam || nMax < (tPvUint32)nParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrUint32Set(m_hDevice, "ExposureAutoOutliers", nParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetExposureAutoRate(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_ExposureAutoRate));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetExposureAutoRate(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_ExposureAutoRate;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "ExposureAutoRate") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "ExposureAutoRate") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvUint32 nMin = 0, nMax = 0;

			if(PvAttrRangeUint32(m_hDevice, "ExposureAutoRate", &nMin, &nMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(nMin > (tPvUint32)nParam || nMax < (tPvUint32)nParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrUint32Set(m_hDevice, "ExposureAutoRate", nParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetExposureAutoTarget(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_ExposureAutoTarget));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetExposureAutoTarget(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_ExposureAutoTarget;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "ExposureAutoTarget") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "ExposureAutoTarget") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvUint32 nMin = 0, nMax = 0;

			if(PvAttrRangeUint32(m_hDevice, "ExposureAutoTarget", &nMin, &nMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(nMin > (tPvUint32)nParam || nMax < (tPvUint32)nParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrUint32Set(m_hDevice, "ExposureAutoTarget", nParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetGainMode(EDeviceAlliedGigEGainMode * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAlliedGigEGainMode)_ttoi(GetParamValue(EDeviceParameterAlliedGigE_GainMode));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetGainMode(EDeviceAlliedGigEGainMode eParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_GainMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < EDeviceAlliedGigEGainMode_Manual || eParam >= EDeviceAlliedGigEGainMode_Count)
		{
			eReturn = EAlliedGigESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "GainMode") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "GainMode") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrEnumSet(m_hDevice, "GainMode", CStringA(g_lpszAlliedGigEGainMode[eParam])))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], g_lpszAlliedGigEGainMode[nPreValue], g_lpszAlliedGigEGainMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetGainValue(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_GainValue));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetGainValue(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_GainValue;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "GainValue") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "GainValue") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvUint32 nMin = 0, nMax = 0;

			if(PvAttrRangeUint32(m_hDevice, "GainValue", &nMin, &nMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(nMin > (tPvUint32)nParam || nMax < (tPvUint32)nParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrUint32Set(m_hDevice, "GainValue", nParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetGainAutoAdjustTol(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_GainAutoAdjustTol));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetGainAutoAdjustTol(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_GainAutoAdjustTol;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "GainAutoAdjustTol") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "GainAutoAdjustTol") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvUint32 nMin = 0, nMax = 0;

			if(PvAttrRangeUint32(m_hDevice, "GainAutoAdjustTol", &nMin, &nMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(nMin > (tPvUint32)nParam || nMax < (tPvUint32)nParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrUint32Set(m_hDevice, "GainAutoAdjustTol", nParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetGainAutoMax(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_GainAutoMax));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetGainAutoMax(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_GainAutoMax;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "GainAutoMax") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "GainAutoMax") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvUint32 nMin = 0, nMax = 0;

			if(PvAttrRangeUint32(m_hDevice, "GainAutoMax", &nMin, &nMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(nMin > (tPvUint32)nParam || nMax < (tPvUint32)nParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrUint32Set(m_hDevice, "GainAutoMax", nParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetGainAutoMin(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_GainAutoMin));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetGainAutoMin(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_GainAutoMin;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "GainAutoMin") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "GainAutoMin") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvUint32 nMin = 0, nMax = 0;

			if(PvAttrRangeUint32(m_hDevice, "GainAutoMin", &nMin, &nMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(nMin > (tPvUint32)nParam || nMax < (tPvUint32)nParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrUint32Set(m_hDevice, "GainAutoMin", nParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetGainAutoOutliers(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_GainAutoOutliers));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetGainAutoOutliers(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_GainAutoOutliers;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "GainAutoOutliers") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "GainAutoOutliers") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvUint32 nMin = 0, nMax = 0;

			if(PvAttrRangeUint32(m_hDevice, "GainAutoOutliers", &nMin, &nMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(nMin > (tPvUint32)nParam || nMax < (tPvUint32)nParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrUint32Set(m_hDevice, "GainAutoOutliers", nParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetGainAutoRate(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_GainAutoRate));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetGainAutoRate(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_GainAutoRate;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "GainAutoRate") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "GainAutoRate") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvUint32 nMin = 0, nMax = 0;

			if(PvAttrRangeUint32(m_hDevice, "GainAutoRate", &nMin, &nMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(nMin > (tPvUint32)nParam || nMax < (tPvUint32)nParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrUint32Set(m_hDevice, "GainAutoRate", nParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetGainAutoTarget(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_GainAutoTarget));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetGainAutoTarget(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_GainAutoTarget;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "GainAutoTarget") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "GainAutoTarget") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvUint32 nMin = 0, nMax = 0;

			if(PvAttrRangeUint32(m_hDevice, "GainAutoTarget", &nMin, &nMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(nMin > (tPvUint32)nParam || nMax < (tPvUint32)nParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrUint32Set(m_hDevice, "GainAutoTarget", nParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetIrisAutoTarget(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_IrisAutoTarget));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetIrisAutoTarget(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_IrisAutoTarget;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "IrisAutoTarget") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "IrisAutoTarget") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvUint32 nMin = 0, nMax = 0;

			if(PvAttrRangeUint32(m_hDevice, "IrisAutoTarget", &nMin, &nMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(nMin > (tPvUint32)nParam || nMax < (tPvUint32)nParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrUint32Set(m_hDevice, "IrisAutoTarget", nParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetIrisMode(EDeviceAlliedGigEIrisMode * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAlliedGigEIrisMode)_ttoi(GetParamValue(EDeviceParameterAlliedGigE_IrisMode));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetIrisMode(EDeviceAlliedGigEIrisMode eParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_IrisMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < EDeviceAlliedGigEIrisMode_Disabled || eParam >= EDeviceAlliedGigEIrisMode_Count)
		{
			eReturn = EAlliedGigESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "IrisMode") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "IrisMode") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrEnumSet(m_hDevice, "IrisMode", CStringA(g_lpszAlliedGigEIrisMode[eParam])))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], g_lpszAlliedGigEIrisMode[nPreValue], g_lpszAlliedGigEIrisMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetIrisVideoLevelMax(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_IrisVideoLevelMax));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetIrisVideoLevelMax(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_IrisVideoLevelMax;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "IrisVideoLevelMax") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "IrisVideoLevelMax") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvUint32 nMin = 0, nMax = 0;

			if(PvAttrRangeUint32(m_hDevice, "IrisVideoLevelMax", &nMin, &nMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(nMin > (tPvUint32)nParam || nMax < (tPvUint32)nParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrUint32Set(m_hDevice, "IrisVideoLevelMax", nParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetIrisVideoLevelMin(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_IrisVideoLevelMin));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetIrisVideoLevelMin(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_IrisVideoLevelMin;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "IrisVideoLevelMin") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "IrisVideoLevelMin") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvUint32 nMin = 0, nMax = 0;

			if(PvAttrRangeUint32(m_hDevice, "IrisVideoLevelMin", &nMin, &nMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(nMin > (tPvUint32)nParam || nMax < (tPvUint32)nParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrUint32Set(m_hDevice, "IrisVideoLevelMin", nParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetLUTEnable(bool * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_LUTEnable));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetLUTEnable(bool bParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_LUTEnable;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "LUTEnable") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "LUTEnable") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrBooleanSet(m_hDevice, "LUTEnable", bParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], g_lpszAlliedGigESwitch[nPreValue], g_lpszAlliedGigESwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetLUTIndex(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_LUTIndex));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetLUTIndex(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_LUTIndex;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "LUTIndex") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "LUTIndex") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvUint32 nMin = 0, nMax = 0;

			if(PvAttrRangeUint32(m_hDevice, "LUTIndex", &nMin, &nMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(nMin > (tPvUint32)nParam || nMax < (tPvUint32)nParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrUint32Set(m_hDevice, "LUTIndex", nParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetLUTMode(EDeviceAlliedGigELUTMode * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAlliedGigELUTMode)_ttoi(GetParamValue(EDeviceParameterAlliedGigE_LUTMode));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetLUTMode(EDeviceAlliedGigELUTMode eParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_LUTMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < EDeviceAlliedGigELUTMode_Luminance || eParam >= EDeviceAlliedGigELUTMode_Count)
		{
			eReturn = EAlliedGigESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "LUTMode") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "LUTMode") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrEnumSet(m_hDevice, "LUTMode", CStringA(g_lpszAlliedGigELUTMode[eParam])))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], g_lpszAlliedGigELUTMode[nPreValue], g_lpszAlliedGigELUTMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetLUTSelector(EDeviceAlliedGigELUTSelector * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAlliedGigELUTSelector)_ttoi(GetParamValue(EDeviceParameterAlliedGigE_LUTSelector));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetLUTSelector(EDeviceAlliedGigELUTSelector eParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_LUTSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < EDeviceAlliedGigELUTSelector_LUT1 || eParam >= EDeviceAlliedGigELUTSelector_Count)
		{
			eReturn = EAlliedGigESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "LUTSelector") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "LUTSelector") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrEnumSet(m_hDevice, "LUTSelector", CStringA(g_lpszAlliedGigELUTSelector[eParam])))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], g_lpszAlliedGigELUTSelector[nPreValue], g_lpszAlliedGigELUTSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetLUTValue(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_LUTValue));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetLUTValue(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_LUTValue;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "LUTValue") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "LUTValue") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvUint32 nMin = 0, nMax = 0;

			if(PvAttrRangeUint32(m_hDevice, "LUTValue", &nMin, &nMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(nMin > (tPvUint32)nParam || nMax < (tPvUint32)nParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrUint32Set(m_hDevice, "LUTValue", nParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetEdgeFilter(EDeviceAlliedGigEEdgeFilter * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAlliedGigEEdgeFilter)_ttoi(GetParamValue(EDeviceParameterAlliedGigE_EdgeFilter));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetEdgeFilter(EDeviceAlliedGigEEdgeFilter eParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_EdgeFilter;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < EDeviceAlliedGigEEdgeFilter_Smooth2 || eParam >= EDeviceAlliedGigEEdgeFilter_Count)
		{
			eReturn = EAlliedGigESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "EdgeFilter") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "EdgeFilter") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrEnumSet(m_hDevice, "EdgeFilter", CStringA(g_lpszAlliedGigEEdgeFilter[eParam])))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], g_lpszAlliedGigEEdgeFilter[nPreValue], g_lpszAlliedGigEEdgeFilter[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetGamma(float * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterAlliedGigE_Gamma));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetGamma(float fParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_Gamma;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "Gamma") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "Gamma") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvFloat32 fMin = 0, fMax = 0;

			if(PvAttrRangeFloat32(m_hDevice, "Gamma", &fMin, &fMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(fMin > fParam || fMax < fParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrFloat32Set(m_hDevice, "Gamma", fParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetHue(float * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterAlliedGigE_Hue));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetHue(float fParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_Hue;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "Hue") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "Hue") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvFloat32 fMin = 0, fMax = 0;

			if(PvAttrRangeFloat32(m_hDevice, "Hue", &fMin, &fMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(fMin > fParam || fMax < fParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrFloat32Set(m_hDevice, "Hue", fParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetOffsetValue(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_OffsetValue));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetOffsetValue(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_OffsetValue;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "OffsetValue") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "OffsetValue") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvUint32 nMin = 0, nMax = 0;

			if(PvAttrRangeUint32(m_hDevice, "OffsetValue", &nMin, &nMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(nMin > (tPvUint32)nParam || nMax < (tPvUint32)nParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrUint32Set(m_hDevice, "OffsetValue", nParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetSaturation(float * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterAlliedGigE_Saturation));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetSaturation(float fParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_Saturation;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "Saturation") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "Saturation") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvFloat32 fMin = 0, fMax = 0;

			if(PvAttrRangeFloat32(m_hDevice, "Saturation", &fMin, &fMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(fMin > fParam || fMax < fParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrFloat32Set(m_hDevice, "Saturation", fParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetWhitebalMode(EDeviceAlliedGigEWhitebalMode * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAlliedGigEWhitebalMode)_ttoi(GetParamValue(EDeviceParameterAlliedGigE_WhitebalMode));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetWhitebalMode(EDeviceAlliedGigEWhitebalMode eParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_WhitebalMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < EDeviceAlliedGigEWhitebalMode_Manual || eParam >= EDeviceAlliedGigEWhitebalMode_Count)
		{
			eReturn = EAlliedGigESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "WhitebalMode") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "WhitebalMode") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrEnumSet(m_hDevice, "WhitebalMode", CStringA(g_lpszAlliedGigEWhitebalMode[eParam])))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], g_lpszAlliedGigEWhitebalMode[nPreValue], g_lpszAlliedGigEWhitebalMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetWhitebalValueBlue(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_WhitebalValueBlue));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetWhitebalValueBlue(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_WhitebalValueBlue;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "WhitebalValueBlue") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "WhitebalValueBlue") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvUint32 nMin = 0, nMax = 0;

			if(PvAttrRangeUint32(m_hDevice, "WhitebalValueBlue", &nMin, &nMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(nMin > (tPvUint32)nParam || nMax < (tPvUint32)nParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrUint32Set(m_hDevice, "WhitebalValueBlue", nParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetWhitebalValueRed(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_WhitebalValueRed));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetWhitebalValueRed(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_WhitebalValueRed;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "WhitebalValueRed") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "WhitebalValueRed") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvUint32 nMin = 0, nMax = 0;

			if(PvAttrRangeUint32(m_hDevice, "WhitebalValueRed", &nMin, &nMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(nMin > (tPvUint32)nParam || nMax < (tPvUint32)nParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrUint32Set(m_hDevice, "WhitebalValueRed", nParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetWhitebalAutoAdjustTol(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_WhitebalAutoAdjustTol));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetWhitebalAutoAdjustTol(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_WhitebalAutoAdjustTol;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "WhitebalAutoAdjustTol") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "WhitebalAutoAdjustTol") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvUint32 nMin = 0, nMax = 0;

			if(PvAttrRangeUint32(m_hDevice, "WhitebalAutoAdjustTol", &nMin, &nMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(nMin > (tPvUint32)nParam || nMax < (tPvUint32)nParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrUint32Set(m_hDevice, "WhitebalAutoAdjustTol", nParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetWhitebalAutoRate(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_WhitebalAutoRate));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetWhitebalAutoRate(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_WhitebalAutoRate;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "WhitebalAutoRate") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "WhitebalAutoRate") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvUint32 nMin = 0, nMax = 0;

			if(PvAttrRangeUint32(m_hDevice, "WhitebalAutoRate", &nMin, &nMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(nMin > (tPvUint32)nParam || nMax < (tPvUint32)nParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrUint32Set(m_hDevice, "WhitebalAutoRate", nParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetBandwidthCtrlMode(EDeviceAlliedGigEBandwidthCtrlMode * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAlliedGigEBandwidthCtrlMode)_ttoi(GetParamValue(EDeviceParameterAlliedGigE_BandwidthCtrlMode));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetBandwidthCtrlMode(EDeviceAlliedGigEBandwidthCtrlMode eParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_BandwidthCtrlMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < EDeviceAlliedGigEBandwidthCtrlMode_StreamBytesPerSecond || eParam >= EDeviceAlliedGigEBandwidthCtrlMode_Count)
		{
			eReturn = EAlliedGigESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "BandwidthCtrlMode") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "BandwidthCtrlMode") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrEnumSet(m_hDevice, "BandwidthCtrlMode", CStringA(g_lpszAlliedGigEBandwidthCtrlMode[eParam])))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], g_lpszAlliedGigEBandwidthCtrlMode[nPreValue], g_lpszAlliedGigEBandwidthCtrlMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetHeartbeatInterval(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_HeartbeatInterval));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetHeartbeatInterval(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_HeartbeatInterval;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "HeartbeatInterval") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "HeartbeatInterval") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvUint32 nMin = 0, nMax = 0;

			if(PvAttrRangeUint32(m_hDevice, "HeartbeatInterval", &nMin, &nMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(nMin > (tPvUint32)nParam || nMax < (tPvUint32)nParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrUint32Set(m_hDevice, "HeartbeatInterval", nParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetHeartbeatTimeout(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_HeartbeatTimeout));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetHeartbeatTimeout(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_HeartbeatTimeout;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "HeartbeatTimeout") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "HeartbeatTimeout") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvUint32 nMin = 0, nMax = 0;

			if(PvAttrRangeUint32(m_hDevice, "HeartbeatTimeout", &nMin, &nMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(nMin > (tPvUint32)nParam || nMax < (tPvUint32)nParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrUint32Set(m_hDevice, "HeartbeatTimeout", nParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetGvcpRetries(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_GvcpRetries));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetGvcpRetries(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_GvcpRetries;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "GvcpRetries") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "GvcpRetries") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvUint32 nMin = 0, nMax = 0;

			if(PvAttrRangeUint32(m_hDevice, "GvcpRetries", &nMin, &nMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(nMin > (tPvUint32)nParam || nMax < (tPvUint32)nParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrUint32Set(m_hDevice, "GvcpRetries", nParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetGvspLookbackWindow(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_GvspLookbackWindow));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetGvspLookbackWindow(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_GvspLookbackWindow;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "GvspLookbackWindow") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "GvspLookbackWindow") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvUint32 nMin = 0, nMax = 0;

			if(PvAttrRangeUint32(m_hDevice, "GvspLookbackWindow", &nMin, &nMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(nMin > (tPvUint32)nParam || nMax < (tPvUint32)nParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrUint32Set(m_hDevice, "GvspLookbackWindow", nParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetGvspResendPercent(float * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterAlliedGigE_GvspResendPercent));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetGvspResendPercent(float fParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_GvspResendPercent;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "GvspResendPercent") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "GvspResendPercent") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvFloat32 fMin = 0, fMax = 0;

			if(PvAttrRangeFloat32(m_hDevice, "GvspResendPercent", &fMin, &fMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(fMin > fParam || fMax < fParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrFloat32Set(m_hDevice, "GvspResendPercent", fParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetGvspRetries(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_GvspRetries));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetGvspRetries(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_GvspRetries;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "GvspRetries") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "GvspRetries") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvUint32 nMin = 0, nMax = 0;

			if(PvAttrRangeUint32(m_hDevice, "GvspRetries", &nMin, &nMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(nMin > (tPvUint32)nParam || nMax < (tPvUint32)nParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrUint32Set(m_hDevice, "GvspRetries", nParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetGvspSocketBuffersCount(EDeviceAlliedGigEGvspSocketBuffersCount * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAlliedGigEGvspSocketBuffersCount)_ttoi(GetParamValue(EDeviceParameterAlliedGigE_GvspSocketBuffersCount));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetGvspSocketBuffersCount(EDeviceAlliedGigEGvspSocketBuffersCount eParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_GvspSocketBuffersCount;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < EDeviceAlliedGigEGvspSocketBuffersCount_256 || eParam >= EDeviceAlliedGigEGvspSocketBuffersCount_Count)
		{
			eReturn = EAlliedGigESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "GvspSocketBuffersCount") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "GvspSocketBuffersCount") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrEnumSet(m_hDevice, "GvspSocketBuffersCount", CStringA(g_lpszAlliedGigEGvspSocketBuffersCount[eParam])))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], g_lpszAlliedGigEGvspSocketBuffersCount[nPreValue], g_lpszAlliedGigEGvspSocketBuffersCount[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetGvspTimeout(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_GvspTimeout));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetGvspTimeout(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_GvspTimeout;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "GvspTimeout") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "GvspTimeout") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvUint32 nMin = 0, nMax = 0;

			if(PvAttrRangeUint32(m_hDevice, "GvspTimeout", &nMin, &nMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(nMin > (tPvUint32)nParam || nMax < (tPvUint32)nParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrUint32Set(m_hDevice, "GvspTimeout", nParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetStrobe1ControlledDuration(EDeviceAlliedGigESwitch * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAlliedGigESwitch)_ttoi(GetParamValue(EDeviceParameterAlliedGigE_Strobe1ControlledDuration));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetStrobe1ControlledDuration(EDeviceAlliedGigESwitch eParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_Strobe1ControlledDuration;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < EDeviceAlliedGigESwitch_Off || eParam >= EDeviceAlliedGigESwitch_Count)
		{
			eReturn = EAlliedGigESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "Strobe1ControlledDuration") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "Strobe1ControlledDuration") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrEnumSet(m_hDevice, "Strobe1ControlledDuration", CStringA(g_lpszAlliedGigESwitch[eParam])))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], g_lpszAlliedGigESwitch[nPreValue], g_lpszAlliedGigESwitch[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetStrobe1Delay(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_Strobe1Delay));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetStrobe1Delay(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_Strobe1Delay;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "Strobe1Delay") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "Strobe1Delay") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvUint32 nMin = 0, nMax = 0;

			if(PvAttrRangeUint32(m_hDevice, "Strobe1Delay", &nMin, &nMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(nMin > (tPvUint32)nParam || nMax < (tPvUint32)nParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrUint32Set(m_hDevice, "Strobe1Delay", nParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetStrobe1Duration(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_Strobe1Duration));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetStrobe1Duration(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_Strobe1Duration;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "Strobe1Duration") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "Strobe1Duration") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvUint32 nMin = 0, nMax = 0;

			if(PvAttrRangeUint32(m_hDevice, "Strobe1Duration", &nMin, &nMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(nMin > (tPvUint32)nParam || nMax < (tPvUint32)nParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrUint32Set(m_hDevice, "Strobe1Duration", nParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetStrobe1Mode(EDeviceAlliedGigEStrobeMode * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAlliedGigEStrobeMode)_ttoi(GetParamValue(EDeviceParameterAlliedGigE_Strobe1Mode));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetStrobe1Mode(EDeviceAlliedGigEStrobeMode eParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_Strobe1Mode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < EDeviceAlliedGigEStrobeMode_AcquisitionTriggerReady || eParam >= EDeviceAlliedGigEStrobeMode_Count)
		{
			eReturn = EAlliedGigESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "Strobe1Mode") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "Strobe1Mode") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrEnumSet(m_hDevice, "Strobe1Mode", CStringA(g_lpszAlliedGigEStrobeMode[eParam])))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], g_lpszAlliedGigEStrobeMode[nPreValue], g_lpszAlliedGigEStrobeMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetSyncIn1GlitchFilter(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_SyncIn1GlitchFilter));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetSyncIn1GlitchFilter(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_SyncIn1GlitchFilter;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "SyncIn1GlitchFilter") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "SyncIn1GlitchFilter") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvUint32 nMin = 0, nMax = 0;

			if(PvAttrRangeUint32(m_hDevice, "SyncIn1GlitchFilter", &nMin, &nMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(nMin > (tPvUint32)nParam || nMax < (tPvUint32)nParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrUint32Set(m_hDevice, "SyncIn1GlitchFilter", nParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetSyncIn2GlitchFilter(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_SyncIn2GlitchFilter));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetSyncIn2GlitchFilter(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_SyncIn2GlitchFilter;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "SyncIn2GlitchFilter") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "SyncIn2GlitchFilter") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvUint32 nMin = 0, nMax = 0;

			if(PvAttrRangeUint32(m_hDevice, "SyncIn2GlitchFilter", &nMin, &nMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(nMin > (tPvUint32)nParam || nMax < (tPvUint32)nParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrUint32Set(m_hDevice, "SyncIn2GlitchFilter", nParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetSyncOut1Invert(EDeviceAlliedGigESwitch * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAlliedGigESwitch)_ttoi(GetParamValue(EDeviceParameterAlliedGigE_SyncOut1Invert));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetSyncOut1Invert(EDeviceAlliedGigESwitch eParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_SyncOut1Invert;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < EDeviceAlliedGigESwitch_Off || eParam >= EDeviceAlliedGigESwitch_Count)
		{
			eReturn = EAlliedGigESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "SyncOut1Invert") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "SyncOut1Invert") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrEnumSet(m_hDevice, "SyncOut1Invert", CStringA(g_lpszAlliedGigESwitch[eParam])))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], g_lpszAlliedGigESwitch[nPreValue], g_lpszAlliedGigESwitch[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetSyncOut1Mode(EDeviceAlliedGigESyncOutMode * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAlliedGigESyncOutMode)_ttoi(GetParamValue(EDeviceParameterAlliedGigE_SyncOut1Mode));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetSyncOut1Mode(EDeviceAlliedGigESyncOutMode eParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_SyncOut1Mode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < EDeviceAlliedGigESyncOutMode_GPO || eParam >= EDeviceAlliedGigESyncOutMode_Count)
		{
			eReturn = EAlliedGigESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "SyncOut1Mode") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "SyncOut1Mode") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrEnumSet(m_hDevice, "SyncOut1Mode", CStringA(g_lpszAlliedGigESyncOutMode[eParam])))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], g_lpszAlliedGigESyncOutMode[nPreValue], g_lpszAlliedGigESyncOutMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetSyncOut2Invert(EDeviceAlliedGigESwitch * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAlliedGigESwitch)_ttoi(GetParamValue(EDeviceParameterAlliedGigE_SyncOut2Invert));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetSyncOut2Invert(EDeviceAlliedGigESwitch eParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_SyncOut2Invert;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < EDeviceAlliedGigESwitch_Off || eParam >= EDeviceAlliedGigESwitch_Count)
		{
			eReturn = EAlliedGigESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "SyncOut2Invert") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "SyncOut2Invert") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrEnumSet(m_hDevice, "SyncOut2Invert", CStringA(g_lpszAlliedGigESwitch[eParam])))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], g_lpszAlliedGigESwitch[nPreValue], g_lpszAlliedGigESwitch[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetSyncOut2Mode(EDeviceAlliedGigESyncOutMode * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAlliedGigESyncOutMode)_ttoi(GetParamValue(EDeviceParameterAlliedGigE_SyncOut2Mode));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetSyncOut2Mode(EDeviceAlliedGigESyncOutMode eParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_SyncOut2Mode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < EDeviceAlliedGigESyncOutMode_GPO || eParam >= EDeviceAlliedGigESyncOutMode_Count)
		{
			eReturn = EAlliedGigESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "SyncOut2Mode") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "SyncOut2Mode") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrEnumSet(m_hDevice, "SyncOut2Mode", CStringA(g_lpszAlliedGigESyncOutMode[eParam])))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], g_lpszAlliedGigESyncOutMode[nPreValue], g_lpszAlliedGigESyncOutMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetSyncOutGpoLevels(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_SyncOutGpoLevels));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetSyncOutGpoLevels(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_SyncOutGpoLevels;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "SyncOutGpoLevels") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "SyncOutGpoLevels") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvUint32 nMin = 0, nMax = 0;

			if(PvAttrRangeUint32(m_hDevice, "SyncOutGpoLevels", &nMin, &nMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(nMin > (tPvUint32)nParam || nMax < (tPvUint32)nParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrUint32Set(m_hDevice, "SyncOutGpoLevels", nParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetStreamBytesPerSecond(int * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_StreamBytesPerSecond));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetStreamBytesPerSecond(int nParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_StreamBytesPerSecond;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "StreamBytesPerSecond") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "StreamBytesPerSecond") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			tPvUint32 nMin = 0, nMax = 0;

			if(PvAttrRangeUint32(m_hDevice, "StreamBytesPerSecond", &nMin, &nMax) != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(nMin > (tPvUint32)nParam || nMax < (tPvUint32)nParam)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrUint32Set(m_hDevice, "StreamBytesPerSecond", nParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetStreamFrameRateConstrain(bool * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAlliedGigE_StreamFrameRateConstrain));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetStreamFrameRateConstrain(bool bParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_StreamFrameRateConstrain;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "StreamFrameRateConstrain") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "StreamFrameRateConstrain") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrBooleanSet(m_hDevice, "StreamFrameRateConstrain", bParam))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], g_lpszAlliedGigESwitch[nPreValue], g_lpszAlliedGigESwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAlliedGigEGetFunction CDeviceAlliedGigE::GetStreamHoldEnable(EDeviceAlliedGigESwitch * pParam)
{
	EAlliedGigEGetFunction eReturn = EAlliedGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAlliedGigEGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAlliedGigESwitch)_ttoi(GetParamValue(EDeviceParameterAlliedGigE_StreamHoldEnable));

		eReturn = EAlliedGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAlliedGigESetFunction CDeviceAlliedGigE::SetStreamHoldEnable(EDeviceAlliedGigESwitch eParam)
{
	EAlliedGigESetFunction eReturn = EAlliedGigESetFunction_UnknownError;

	EDeviceParameterAlliedGigE eSaveID = EDeviceParameterAlliedGigE_StreamHoldEnable;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < EDeviceAlliedGigESwitch_Off || eParam >= EDeviceAlliedGigESwitch_Count)
		{
			eReturn = EAlliedGigESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PvAttrExists(m_hDevice, "StreamHoldEnable") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrIsAvailable(m_hDevice, "StreamHoldEnable") != tPvErr::ePvErrSuccess)
			{
				eReturn = EAlliedGigESetFunction_NotSupportedError;
				break;
			}

			if(PvAttrEnumSet(m_hDevice, "StreamHoldEnable", CStringA(g_lpszAlliedGigESwitch[eParam])))
			{
				eReturn = EAlliedGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAlliedGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAlliedGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAlliedGigE[eSaveID], g_lpszAlliedGigESwitch[nPreValue], g_lpszAlliedGigESwitch[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceAlliedGigE::ConvertStringToVector(char * pString, int nStringSize, std::vector<CString>* pVctString)
{
	bool bReturn = false;

	char* pStrData = nullptr;

	do 
	{
		if(!pVctString)
			break;

		pVctString->clear();

		if(!pString)
			break;

		if(!nStringSize)
			break;

		pStrData = new char[nStringSize];
		memset(pStrData, 0, sizeof(char) * nStringSize);

		int nSaveIdx = 0;

		for(int i = 0; i < nStringSize; ++i)
		{
			pStrData[nSaveIdx] = pString[i];

			if(pString[i + 1] == ',')
			{
				pVctString->push_back(CString(pStrData));
				memset(pStrData, 0, sizeof(char) * nStringSize);
				nSaveIdx = 0;
				++i;
				continue;
			}

			if(i + 1 == nStringSize)
				pVctString->push_back(CString(pStrData));

			++nSaveIdx;
		}
	} 
	while(false);

	if(pStrData)
		delete[] pStrData;
	pStrData = nullptr;

	return bReturn;
}

bool CDeviceAlliedGigE::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = false;

	bool bFoundID = true;

	do
	{
		switch(nParam)
		{
		case EDeviceParameterAlliedGigE_DeviceID:
			bReturn = !SetDeviceID(strValue);
			break;
		case EDeviceParameterAlliedGigE_DeviceFindWaittingTime:
			bReturn = !SetDeviceFindWaittingTime(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_GrabWaittingTime:
			bReturn = !SetGrabWaittingTime(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_InitializeType:
			bReturn = !SetInitializeType((EDeviceAlliedGigEInitializeType)_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_Width:
			bReturn = !SetCanvasWidth(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_Height:
			bReturn = !SetCanvasHeight(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_RegionX:
			bReturn = !SetRegionX(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_RegionY:
			bReturn = !SetRegionY(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_BinningX:
			bReturn = !SetBinningX(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_BinningY:
			bReturn = !SetBinningY(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_DecimationHorizontal:
			bReturn = !SetDecimationHorizontal(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_DecimationVertical:
			bReturn = !SetDecimationVertical(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_PixelFormat:
			bReturn = !SetPixelFormat((EDeviceAlliedGigEPixelFormat)_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_PacketSize:
			bReturn = !SetPacketSize(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_AcqEndTriggerEvent:
			bReturn = !SetAcqEndTriggerEvent((EDeviceAlliedGigETriggerEvent)_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_AcqEndTriggerMode:
			bReturn = !SetAcqEndTriggerMode((EDeviceAlliedGigETriggerMode)_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_AcqRecTriggerEvent:
			bReturn = !SetAcqRecTriggerEvent((EDeviceAlliedGigETriggerEvent)_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_AcqRecTriggerMode:
			bReturn = !SetAcqRecTriggerMode((EDeviceAlliedGigETriggerMode)_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_AcqStartTriggerEvent:
			bReturn = !SetAcqStartTriggerEvent((EDeviceAlliedGigETriggerEvent)_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_AcqStartTriggerMode:
			bReturn = !SetAcqStartTriggerMode((EDeviceAlliedGigETriggerMode)_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_FrameRate:
			bReturn = !SetFrameRate(_ttof(strValue));
			break;
		case EDeviceParameterAlliedGigE_FrameStartTriggerDelay:
			bReturn = !SetFrameStartTriggerDelay(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_FrameStartTriggerEvent:
			bReturn = !SetFrameStartTriggerEvent((EDeviceAlliedGigETriggerEvent)_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_FrameStartTriggerMode:
			bReturn = !SetFrameStartTriggerMode((EDeviceAlliedGigEFrameMode)_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_FrameStartTriggerOverlap:
			bReturn = !SetFrameStartTriggerOverlap((EDeviceAlliedGigETriggerOverlap)_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_AcquisitionFrameCount:
			bReturn = !SetAcquisitionFrameCount(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_AcquisitionMode:
			bReturn = !SetAcquisitionMode((EDeviceAlliedGigEAcquisitionMode)_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_RecorderPreEventCount:
			bReturn = !SetRecorderPreEventCount(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_ColorTransformationMode:
			bReturn = !SetColorTransformationMode((EDeviceAlliedGigEColorTransformationMode)_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_ColorTransformationValueBB:
			bReturn = !SetColorTransformationValueBB(_ttof(strValue));
			break;
		case EDeviceParameterAlliedGigE_ColorTransformationValueBG:
			bReturn = !SetColorTransformationValueBG(_ttof(strValue));
			break;
		case EDeviceParameterAlliedGigE_ColorTransformationValueBR:
			bReturn = !SetColorTransformationValueBR(_ttof(strValue));
			break;
		case EDeviceParameterAlliedGigE_ColorTransformationValueGB:
			bReturn = !SetColorTransformationValueGB(_ttof(strValue));
			break;
		case EDeviceParameterAlliedGigE_ColorTransformationValueGG:
			bReturn = !SetColorTransformationValueGG(_ttof(strValue));
			break;
		case EDeviceParameterAlliedGigE_ColorTransformationValueGR:
			bReturn = !SetColorTransformationValueGR(_ttof(strValue));
			break;
		case EDeviceParameterAlliedGigE_ColorTransformationValueRB:
			bReturn = !SetColorTransformationValueRB(_ttof(strValue));
			break;
		case EDeviceParameterAlliedGigE_ColorTransformationValueRG:
			bReturn = !SetColorTransformationValueRG(_ttof(strValue));
			break;
		case EDeviceParameterAlliedGigE_ColorTransformationValueRR:
			bReturn = !SetColorTransformationValueRR(_ttof(strValue));
			break;
		case EDeviceParameterAlliedGigE_DSPSubregionBottom:
			bReturn = !SetDSPSubregionBottom(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_DSPSubregionLeft:
			bReturn = !SetDSPSubregionLeft(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_DSPSubregionRight:
			bReturn = !SetDSPSubregionRight(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_DSPSubregionTop:
			bReturn = !SetDSPSubregionTop(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_ExposureMode:
			bReturn = !SetExposureMode((EDeviceAlliedGigEExposureMode)_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_ExposureValue:
			bReturn = !SetExposureValue(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_ExposureAutoAdjustTol:
			bReturn = !SetExposureAutoAdjustTol(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_ExposureAutoAlg:
			bReturn = !SetExposureAutoAlg((EDeviceAlliedGigEExposureAutoAlg)_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_ExposureAutoMax:
			bReturn = !SetExposureAutoMax(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_ExposureAutoMin:
			bReturn = !SetExposureAutoMin(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_ExposureAutoOutliers:
			bReturn = !SetExposureAutoOutliers(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_ExposureAutoRate:
			bReturn = !SetExposureAutoRate(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_ExposureAutoTarget:
			bReturn = !SetExposureAutoTarget(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_GainMode:
			bReturn = !SetGainMode((EDeviceAlliedGigEGainMode)_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_GainValue:
			bReturn = !SetGainValue(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_GainAutoAdjustTol:
			bReturn = !SetGainAutoAdjustTol(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_GainAutoMax:
			bReturn = !SetGainAutoMax(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_GainAutoMin:
			bReturn = !SetGainAutoMin(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_GainAutoOutliers:
			bReturn = !SetGainAutoOutliers(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_GainAutoRate:
			bReturn = !SetGainAutoRate(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_GainAutoTarget:
			bReturn = !SetGainAutoTarget(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_IrisAutoTarget:
			bReturn = !SetIrisAutoTarget(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_IrisMode:
			bReturn = !SetIrisMode((EDeviceAlliedGigEIrisMode)_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_IrisVideoLevelMax:
			bReturn = !SetIrisVideoLevelMax(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_IrisVideoLevelMin:
			bReturn = !SetIrisVideoLevelMin(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_LUTEnable:
			bReturn = !SetLUTEnable(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_LUTIndex:
			bReturn = !SetLUTIndex(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_LUTMode:
			bReturn = !SetLUTMode((EDeviceAlliedGigELUTMode)_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_LUTSelector:
			bReturn = !SetLUTSelector((EDeviceAlliedGigELUTSelector)_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_LUTValue:
			bReturn = !SetLUTValue(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_EdgeFilter:
			bReturn = !SetEdgeFilter((EDeviceAlliedGigEEdgeFilter)_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_Gamma:
			bReturn = !SetGamma(_ttof(strValue));
			break;
		case EDeviceParameterAlliedGigE_Hue:
			bReturn = !SetHue(_ttof(strValue));
			break;
		case EDeviceParameterAlliedGigE_OffsetValue:
			bReturn = !SetOffsetValue(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_Saturation:
			bReturn = !SetSaturation(_ttof(strValue));
			break;
		case EDeviceParameterAlliedGigE_WhitebalMode:
			bReturn = !SetWhitebalMode((EDeviceAlliedGigEWhitebalMode)_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_WhitebalValueBlue:
			bReturn = !SetWhitebalValueBlue(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_WhitebalValueRed:
			bReturn = !SetWhitebalValueRed(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_WhitebalAutoAdjustTol:
			bReturn = !SetWhitebalAutoAdjustTol(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_WhitebalAutoRate:
			bReturn = !SetWhitebalAutoRate(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_BandwidthCtrlMode:
			bReturn = !SetBandwidthCtrlMode((EDeviceAlliedGigEBandwidthCtrlMode)_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_HeartbeatInterval:
			bReturn = !SetHeartbeatInterval(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_HeartbeatTimeout:
			bReturn = !SetHeartbeatTimeout(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_GvcpRetries:
			bReturn = !SetGvcpRetries(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_GvspLookbackWindow:
			bReturn = !SetGvspLookbackWindow(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_GvspResendPercent:
			bReturn = !SetGvspResendPercent(_ttof(strValue));
			break;
		case EDeviceParameterAlliedGigE_GvspRetries:
			bReturn = !SetGvspRetries(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_GvspSocketBuffersCount:
			bReturn = !SetGvspSocketBuffersCount((EDeviceAlliedGigEGvspSocketBuffersCount)_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_GvspTimeout:
			bReturn = !SetGvspTimeout(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_Strobe1ControlledDuration:
			bReturn = !SetStrobe1ControlledDuration((EDeviceAlliedGigESwitch)_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_Strobe1Delay:
			bReturn = !SetStrobe1Delay(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_Strobe1Duration:
			bReturn = !SetStrobe1Duration(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_Strobe1Mode:
			bReturn = !SetStrobe1Mode((EDeviceAlliedGigEStrobeMode)_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_SyncIn1GlitchFilter:
			bReturn = !SetSyncIn1GlitchFilter(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_SyncIn2GlitchFilter:
			bReturn = !SetSyncIn2GlitchFilter(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_SyncOut1Invert:
			bReturn = !SetSyncOut1Invert((EDeviceAlliedGigESwitch)_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_SyncOut1Mode:
			bReturn = !SetSyncOut1Mode((EDeviceAlliedGigESyncOutMode)_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_SyncOut2Invert:
			bReturn = !SetSyncOut2Invert((EDeviceAlliedGigESwitch)_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_SyncOut2Mode:
			bReturn = !SetSyncOut2Mode((EDeviceAlliedGigESyncOutMode)_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_SyncOutGpoLevels:
			bReturn = !SetSyncOutGpoLevels(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_StreamBytesPerSecond:
			bReturn = !SetStreamBytesPerSecond(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_StreamFrameRateConstrain:
			bReturn = !SetStreamFrameRateConstrain(_ttoi(strValue));
			break;
		case EDeviceParameterAlliedGigE_StreamHoldEnable:
			bReturn = !SetStreamHoldEnable((EDeviceAlliedGigESwitch)_ttoi(strValue));
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

bool CDeviceAlliedGigE::DoesModuleExist()
{
	bool bReturn = false;

	CString strModuleName;

	do
	{
		strModuleName.Format(_T("PvAPI.dll"));

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

UINT CDeviceAlliedGigE::CallbackFunction(LPVOID pParam)
{
	CDeviceAlliedGigE* pInstance = (CDeviceAlliedGigE*)pParam;
	if(!pInstance)
		return 0;

	bool bError = false;

	do
	{
		tPvFrame* pFrames = (tPvFrame*)pInstance->m_pFrames;
		if(!pFrames)
			break;

		int nGrabWaittingTime = 2000;
		if(pInstance->GetGrabWaittingTime(&nGrabWaittingTime))
			break;

		int nImageIdx = 0;

		do
		{
			pInstance->NextImageIndex();

			CRavidImage* pCurrentImage = pInstance->GetImageInfo();

			BYTE* pCurrentBuffer = pCurrentImage->GetBuffer();

			const int64_t i64Width = (int64_t)pCurrentImage->GetSizeX();
			const int64_t i64Height = (int64_t)pCurrentImage->GetSizeY();
			const int64_t i64ImageSizeByte = (int64_t)pCurrentImage->GetImageSizeByte();

			tPvErr tpvError = tPvErr::ePvErrSuccess;

			while(1)
			{
				tpvError = PvCaptureWaitForFrameDone(pInstance->m_hDevice, &pFrames[nImageIdx], nGrabWaittingTime);
				if(tpvError == tPvErr::ePvErrTimeout || tpvError == tPvErr::ePvErrSuccess)
					break;
			}

			if(tpvError != tPvErr::ePvErrSuccess)
				break;

			if(pFrames[nImageIdx].Status == tPvErr::ePvErrSuccess)
			{
				switch(pFrames[nImageIdx].Format)
				{
				case ePvFmtMono8:
				case ePvFmtMono16:
					memcpy(pCurrentBuffer, pFrames[nImageIdx].ImageBuffer, pFrames[nImageIdx].ImageBufferSize);
					break;// Monochrome, 16 bits, data is LSB aligned
				case ePvFmtBayer8:
					{
						ULONG remainder = ULONG_PADDING(i64Width * 3);

						PvUtilityColorInterpolate(&pFrames[nImageIdx], &pCurrentBuffer[2], &pCurrentBuffer[1], &pCurrentBuffer[0], 2, remainder);
					}
					break;
				case ePvFmtBayer16:
					{
						ULONG remainder = ULONG_PADDING(i64Width * 3);
						const USHORT* pSrc = (USHORT*)pFrames[nImageIdx].ImageBuffer;
						const USHORT* pSrcEnd = pSrc + (i64Width * i64Height);

						BYTE* pDest = (BYTE*)pFrames[nImageIdx].ImageBuffer;
						const BYTE bitshift = (BYTE)pFrames[nImageIdx].BitDepth - 8;

						while(pSrc < pSrcEnd)
							*(pDest++) = *(pSrc++) >> (bitshift);

						pFrames[nImageIdx].Format = ePvFmtBayer8;

						PvUtilityColorInterpolate(&pFrames[nImageIdx], &pCurrentBuffer[2], &pCurrentBuffer[1], &pCurrentBuffer[0], 2, remainder);
					}
					break;
				case ePvFmtRgb24:
					{
						ULONG remainder = ULONG_PADDING(i64Width * 3);

						const BYTE* pSrc = (BYTE*)pFrames[nImageIdx].ImageBuffer;
						const BYTE* pSrcEnd = pSrc + pFrames[nImageIdx].ImageBufferSize;

						while(pSrc < pSrcEnd)
						{
							for(ULONG i = 0; i < (ULONG)i64Width; i++)
							{
								const BYTE	red = *(pSrc++);
								const BYTE	green = *(pSrc++);
								const BYTE	blue = *(pSrc++);

								*(pCurrentBuffer++) = blue;
								*(pCurrentBuffer++) = green;
								*(pCurrentBuffer++) = red;
							}
							pCurrentBuffer += remainder;
						}
					}
					break;
				case ePvFmtRgb48:
					{
						ULONG remainder = ULONG_PADDING(i64Width * 3);

						const USHORT* pSrc = (USHORT*)pFrames[nImageIdx].ImageBuffer;
						const USHORT* pSrcEnd = (const USHORT*)((const BYTE*)pSrc + pFrames[nImageIdx].ImageBufferSize);
						const BYTE bitshift = (BYTE)pFrames[nImageIdx].BitDepth - 8;
						BYTE* pBufferEnd = pCurrentBuffer + i64ImageSizeByte;

						while(pSrc < pSrcEnd && pCurrentBuffer < pBufferEnd)
						{
							for(ULONG i = 0; i < (ULONG)i64Width && pCurrentBuffer < pBufferEnd; i++)
							{
								const BYTE	red = *(pSrc++) >> (bitshift);
								const BYTE	green = *(pSrc++) >> (bitshift);
								const BYTE	blue = *(pSrc++) >> (bitshift);

								if(pCurrentBuffer < pBufferEnd)
								{
									*(pCurrentBuffer++) = blue;
									if(pCurrentBuffer < pBufferEnd)
									{
										*(pCurrentBuffer++) = green;
										if(pCurrentBuffer < pBufferEnd)
											*(pCurrentBuffer++) = red;
									}
								}
							}
							pCurrentBuffer += remainder;
						}
					}
					break;
				case ePvFmtYuv411:
					{
						const BYTE* pSrc = (BYTE*)pFrames[nImageIdx].ImageBuffer;
						const BYTE* pSrcEnd = pSrc + (unsigned int)(i64Width * i64Height * 1.5);

						int y1, y2, y3, y4, u, v;
						int r, g, b;

						while(pSrc < pSrcEnd)
						{
							u = pSrc[0];
							y1 = pSrc[1];
							y2 = pSrc[2];
							v = pSrc[3];
							y3 = pSrc[4];
							y4 = pSrc[5];
							pSrc += 6;

							YUV2RGB(y1, u, v, r, g, b);
							*(pCurrentBuffer++) = (BYTE)b;
							*(pCurrentBuffer++) = (BYTE)g;
							*(pCurrentBuffer++) = (BYTE)r;
							YUV2RGB(y2, u, v, r, g, b);
							*(pCurrentBuffer++) = (BYTE)b;
							*(pCurrentBuffer++) = (BYTE)g;
							*(pCurrentBuffer++) = (BYTE)r;
							YUV2RGB(y3, u, v, r, g, b);
							*(pCurrentBuffer++) = (BYTE)b;
							*(pCurrentBuffer++) = (BYTE)g;
							*(pCurrentBuffer++) = (BYTE)r;
							YUV2RGB(y4, u, v, r, g, b);
							*(pCurrentBuffer++) = (BYTE)b;
							*(pCurrentBuffer++) = (BYTE)g;
							*(pCurrentBuffer++) = (BYTE)r;
						}
					}
					break;
				case ePvFmtYuv422:
					{
						const BYTE* pSrc = (BYTE*)pFrames[nImageIdx].ImageBuffer;
						const BYTE* pSrcEnd = pSrc + (i64Width * i64Height * 2);

						int y1, y2, u, v;
						int r, g, b;

						while(pSrc < pSrcEnd)
						{
							u = pSrc[0];
							y1 = pSrc[1];
							v = pSrc[2];
							y2 = pSrc[3];
							pSrc += 4;

							YUV2RGB(y1, u, v, r, g, b);
							*(pCurrentBuffer++) = (BYTE)b;
							*(pCurrentBuffer++) = (BYTE)g;
							*(pCurrentBuffer++) = (BYTE)r;
							YUV2RGB(y2, u, v, r, g, b);
							*(pCurrentBuffer++) = (BYTE)b;
							*(pCurrentBuffer++) = (BYTE)g;
							*(pCurrentBuffer++) = (BYTE)r;
						}
					}
					break;
				case ePvFmtYuv444:
					{
						const BYTE* pSrc = (BYTE*)pFrames[nImageIdx].ImageBuffer;
						const BYTE* pSrcEnd = pSrc + (i64Width * i64Height * 3);

						int y2, y1, u, v;
						int r, g, b;

						r = b = g = 0;

						while(pSrc < pSrcEnd)
						{
							u = pSrc[0];
							y1 = pSrc[1];
							v = pSrc[2];
							y2 = pSrc[4];
							pSrc += 6;

							YUV2RGB(y1, u, v, r, g, b);
							*(pCurrentBuffer++) = (BYTE)b;
							*(pCurrentBuffer++) = (BYTE)g;
							*(pCurrentBuffer++) = (BYTE)r;
							YUV2RGB(y2, u, v, r, g, b);
							*(pCurrentBuffer++) = (BYTE)b;
							*(pCurrentBuffer++) = (BYTE)g;
							*(pCurrentBuffer++) = (BYTE)r;
						}
					}
					break;
				case ePvFmtBgr24:
					{
						const ULONG rowlen = i64Width * 3;
						ULONG remainder = ULONG_PADDING(rowlen);

						const BYTE* pSrc = (BYTE*)pFrames[nImageIdx].ImageBuffer;
						const BYTE* pSrcEnd = pSrc + pFrames[nImageIdx].ImageBufferSize;

						while(pSrc < pSrcEnd)
						{
							memcpy(pCurrentBuffer, pSrc, rowlen);
							pSrc += rowlen;
							pCurrentBuffer += rowlen + remainder;
						}
					}
					break;
				case ePvFmtRgba32:
					{
						ULONG remainder = ULONG_PADDING(i64Width * 3);

						const BYTE* pSrc = (BYTE*)pFrames[nImageIdx].ImageBuffer;
						const BYTE* pSrcEnd = pSrc + pFrames[nImageIdx].ImageBufferSize;
						BYTE* pBufferEnd = pCurrentBuffer + i64ImageSizeByte;

						while(pSrc < pSrcEnd && pCurrentBuffer < pBufferEnd)
						{
							for(ULONG i = 0; i < (ULONG)i64Width && pCurrentBuffer < pBufferEnd; i++)
							{
								const BYTE	red = *(pSrc++);
								const BYTE	green = *(pSrc++);
								const BYTE	blue = *(pSrc++);

								// skip the alpha
								pSrc++;

								if(pCurrentBuffer < pBufferEnd)
								{
									*(pCurrentBuffer++) = blue;
									if(pCurrentBuffer < pBufferEnd)
									{
										*(pCurrentBuffer++) = green;
										if(pCurrentBuffer < pBufferEnd)
											*(pCurrentBuffer++) = red;
									}
								}
							}
							pCurrentBuffer += remainder;
						}
					}
					break;
				case ePvFmtBgra32:
					{
						ULONG remainder = ULONG_PADDING(i64Width * 3);

						const BYTE* pSrc = (BYTE*)pFrames[nImageIdx].ImageBuffer;
						const BYTE* pSrcEnd = pSrc + pFrames[nImageIdx].ImageBufferSize;
						BYTE* pBufferEnd = pCurrentBuffer + i64ImageSizeByte;

						while(pSrc < pSrcEnd && pCurrentBuffer < pBufferEnd)
						{
							for(ULONG i = 0; i < (ULONG)i64Width && pCurrentBuffer < pBufferEnd; i++)
							{
								if(pCurrentBuffer < pBufferEnd)
								{
									*(pCurrentBuffer++) = *(pSrc++);
									if(pCurrentBuffer < pBufferEnd)
									{
										*(pCurrentBuffer++) = *(pSrc++);
										if(pCurrentBuffer < pBufferEnd)
											*(pCurrentBuffer++) = *(pSrc++);
									}
								}

								// skip the alpha
								pSrc++;
							}
							pCurrentBuffer += remainder;
						}
					}
					break;
				case ePvFmtMono12Packed:
					{
						if(!(i64Width % 2) && !(i64Height % 2))
						{
							ULONG remainder = ULONG_PADDING(i64Width);

							const Packed12BitsPixel_t* pSrc = (const Packed12BitsPixel_t*)pFrames[nImageIdx].ImageBuffer;
							const Packed12BitsPixel_t* pSrcEnd = (const Packed12BitsPixel_t*)((unsigned char*)pFrames[nImageIdx].ImageBuffer + pFrames[nImageIdx].ImageSize);
							const BYTE bitshift = (BYTE)pFrames[nImageIdx].BitDepth - 8;
							BYTE* pBufferEnd = pCurrentBuffer + i64ImageSizeByte;

							USHORT pixel;

							while(pSrc < pSrcEnd)
							{
								for(ULONG i = 0; i < (ULONG)i64Width && pSrc < pSrcEnd; i += 2)
								{
									if(pCurrentBuffer < pBufferEnd)
									{
										pixel = (USHORT)pSrc->LByte << 4;
										pixel += ((USHORT)pSrc->MByte & 0xF0) >> 4;
										*(pCurrentBuffer++) = pixel >> (bitshift);

										if(pCurrentBuffer < pBufferEnd)
										{
											pixel = (USHORT)pSrc->UByte << 4;
											pixel += ((USHORT)pSrc->MByte & 0x0F) >> 4;
											*(pCurrentBuffer++) = pixel >> (bitshift);
										}
									}

									pSrc++;
								}

								pCurrentBuffer += remainder;
							}
						}
					}
					break;
				case ePvFmtBayer12Packed:
					{
						if(!(i64Width % 2) && !(i64Height % 2))
						{
							ULONG remainder = ULONG_PADDING(i64Width * 3);

							const Packed12BitsPixel_t* pSrc = (const Packed12BitsPixel_t*)pFrames[nImageIdx].ImageBuffer;
							const Packed12BitsPixel_t* pSrcEnd = (const Packed12BitsPixel_t*)((unsigned char*)pFrames[nImageIdx].ImageBuffer + pFrames[nImageIdx].ImageSize);

							BYTE* pDest = (BYTE*)pFrames[nImageIdx].ImageBuffer;
							BYTE* pDestEnd = pDest + pFrames[nImageIdx].ImageBufferSize;
							const BYTE bitshift = (BYTE)pFrames[nImageIdx].BitDepth - 8;
							USHORT pixel1, pixel2;

							while(pSrc < pSrcEnd && pDest < pDestEnd)
							{
								for(ULONG i = 0; i < (ULONG)i64Width && pSrc < pSrcEnd; i += 2)
								{
									pixel1 = (USHORT)pSrc->LByte << 4;
									pixel1 += ((USHORT)pSrc->MByte & 0xF0) >> 4;

									pixel2 = (USHORT)pSrc->UByte << 4;
									pixel2 += ((USHORT)pSrc->MByte & 0x0F) >> 4;

									if(pDest < pDestEnd)
									{
										*(pDest++) = pixel1 >> (bitshift);
										if(pDest < pDestEnd)
											*(pDest++) = pixel2 >> (bitshift);
									}

									pSrc++;
								}
							}

							if(pDest < pDestEnd)
							{
								pFrames[nImageIdx].Format = ePvFmtBayer8;

								PvUtilityColorInterpolate(&pFrames[nImageIdx], &pCurrentBuffer[2], &pCurrentBuffer[1], &pCurrentBuffer[0], 2, remainder);
							}
						}
					}
					break;// Bayer-color, 8 bits
				}

				pInstance->ConnectImage(false);

				CEventHandlerManager::BroadcastOnAcquisition(pCurrentImage, pInstance->GetObjectID(), pInstance->GetImageView(), pInstance);
				
				if(!pInstance->IsGrabAvailable())
					pInstance->m_bIsGrabAvailable = true;
			}

			if(PvCaptureQueueFrame(pInstance->m_hDevice, &pFrames[nImageIdx], NULL) != tPvErr::ePvErrSuccess)
				break;

			++nImageIdx;

			if(nImageIdx == FRAMESCOUNT)
				nImageIdx = 0;
		}
		while(pInstance->IsLive() || !pInstance->IsGrabAvailable());

		PvCommandRun(pInstance->m_hDevice, "AcquisitionStop");
		PvCaptureQueueClear(pInstance->m_hDevice);

		pInstance->m_bIsLive = false;
		pInstance->m_bIsGrabAvailable = true;
	}
	while(false);

	return 0;
}

#endif