#include "stdafx.h"

#include "DevicePhotonfocus3D.h"
#include "RavidImage3DProfilePhotonFocus.h"

#ifdef USING_DEVICE

#include "../RavidCore/RavidRect.h"
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
 
 #pragma warning(push)
// #pragma warning(disable:4083)
 #pragma warning(disable:4473)
 #pragma warning(disable:4477)

#include "../Libraries/Includes/PhotonFocus/PvDeviceFinderWnd.h"
#include "../Libraries/Includes/PhotonFocus/PvDevice.h"
#include "../Libraries/Includes/PhotonFocus/PvSampleUtils.h"
#include "../Libraries/Includes/PhotonFocus/PvStream.h"
#include "../Libraries/Includes/PhotonFocus/PvPipeline.h"

#ifndef _WIN64
// pvsystem.dll
// pvbase.dll
// pvbuffer.dll
// pvstream.dll
// pvgenicam.dll
// pvdevice.dll
// pvstreamraw.dll
#pragma comment(lib, COMMONLIB_PREFIX "Photonfocus/PvBase.lib")
#pragma comment(lib, COMMONLIB_PREFIX "Photonfocus/PvBuffer.lib")
#pragma comment(lib, COMMONLIB_PREFIX "Photonfocus/PvDevice.lib")
#pragma comment(lib, COMMONLIB_PREFIX "Photonfocus/PvGenICam.lib")
#pragma comment(lib, COMMONLIB_PREFIX "Photonfocus/PvStream.lib")
#pragma comment(lib, COMMONLIB_PREFIX "Photonfocus/PvStreamRaw.lib")
#else
// pvsystem64.dll
// pvbase64.dll
// pvbuffer64.dll
// pvstream64.dll
// pvgenicam64.dll
// pvdevice64.dll
// pvstreamraw64.dll
#pragma comment(lib, COMMONLIB_PREFIX "Photonfocus/PvBase64.lib")
#pragma comment(lib, COMMONLIB_PREFIX "Photonfocus/PvBuffer64.lib")
#pragma comment(lib, COMMONLIB_PREFIX "Photonfocus/PvDevice64.lib")
#pragma comment(lib, COMMONLIB_PREFIX "Photonfocus/PvGenICam64.lib")
#pragma comment(lib, COMMONLIB_PREFIX "Photonfocus/PvStream64.lib")
#pragma comment(lib, COMMONLIB_PREFIX "Photonfocus/PvStreamRaw64.lib")
#endif

#pragma warning(pop)

using namespace Ravid;
using namespace Ravid::Device;
using namespace Ravid::Database;
using namespace Ravid::Framework;
using namespace Ravid::Algorithms;
using namespace Ravid::Miscellaneous;

typedef struct sLaserData
{
	float fPeak;
	BYTE cPeak;
	BYTE cWidth;
	BYTE cQuality;
}
SLaserData, *LPLaserData;

IMPLEMENT_DYNAMIC(CDevicePhotonfocus3D, CDeviceCamera)

BEGIN_MESSAGE_MAP(CDevicePhotonfocus3D, CDeviceCamera)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static LPCTSTR g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_Count] =
{
	_T("DeviceID"),
	_T("Camera Setting"),
	_T("Grab count"),
	_T("Viewer type"),
	_T("3D Image Height"),
	_T("Canvas width"),
	_T("Canvas height"),
	_T("Offset X"),
	_T("Offset Y"),
	_T("Buffer count"),
	_T("Grab Waiting Time[ms]"),
	_T("Detect Timeout"),
	_T("Acquisition Control"),
	_T("Acquisition Mode"),
	_T("Acquisition Frame Count"),
	_T("Trigger Selector"),
	_T("Trigger Mode"),
	_T("Trigger Source"),
	_T("Trigger Activation"),
	_T("Trigger Delay[us]"),
	_T("Trigger Divider"),
	_T("ABTrigger Debounce"),
	_T("ABTrigger Mode"),
	_T("ABTrigger Direction"),
	_T("ABTrigger Divider"),
	_T("ABEncoder Position"),
	_T("Exposure Mode"),
	_T("Exposure Time[us]"),
	_T("Timer Control"),
	_T("Timer Selector"),
	_T("Timer Duration"),
	_T("Timer Delay"),
	_T("Timer Trigger Source"),
	_T("Timer Trigger Activation"),
	_T("Timer Granularity Factor"),
	_T("Analog Control"),
	_T("Gain Selector"),
	_T("Gain"),
	_T("Black Level Selector"),
	_T("Black Level"),
	_T("Digital Offset"),
	_T("PeakDetector"),
	_T("Peak0"),
	_T("Peak0_3D"),
	_T("Peak0_Threshold"),
	_T("Peak0_3DY"),
	_T("Peak0_3DH"),
	_T("Peak0_Mirror"),
	_T("Peak0_2D"),
	_T("Peak0_2DY"),
	_T("Peak0_Gain"),
	_T("Peak0_DigitalOffset"),
	_T("Peak0_PeakFilter"),
	_T("Peak0_EnPeakFilter"),
	_T("Peak0_PeakFilterHeightMin"),
	_T("Peak0_PeakFilterHeightMax"),
	_T("Peak0_PeakFilterWidthMin"),
	_T("Peak0_PeakFilterWidthMax"),
	_T("Peak0_MovingROI"),
	_T("Peak0_EnMovingROI"),
	_T("Peak0_MovingROIEnLaserFinder"),
	_T("Peak0_MovingROIRangeY"),
	_T("Peak0_MovingROIRangeH"),
	_T("Peak0_MovingROIMinCol"),
	_T("Peak0_MovingROIMinDistance"),
	_T("Peak0_MovingROISmoothing"),
	_T("Peak0_EnAbsCoordinate"),
	_T("Peak0_AbsCoordinateBase"),
	_T("Peak1"),
	_T("Peak1_3D"),
	_T("Peak1_Threshold"),
	_T("Peak1_3DY"),
	_T("Peak1_3DH"),
	_T("Peak1_Mirror"),
	_T("Peak1_2D"),
	_T("Peak1_2DY"),
	_T("Peak1_Gain"),
	_T("Peak1_DigitalOffset"),
	_T("Peak1_PeakFilter"),
	_T("Peak1_EnPeakFilter"),
	_T("Peak1_PeakFilterHeightMin"),
	_T("Peak1_PeakFilterHeightMax"),
	_T("Peak1_PeakFilterWidthMin"),
	_T("Peak1_PeakFilterWidthMax"),
	_T("Peak1_EnAbsCoordinate"),
	_T("Peak1_AbsCoordinateBase"),
	_T("PeakDetector_NrOfPeaks"),
	_T("PeakDetector_DataFormat3D"),
	_T("PeakDetector_HighSpeed"),
};

static LPCTSTR g_lpszPhotonfocus3DViewerType[EDevicePhotonfocus3DViewerType_Count] =
{
	_T("2D only"),
	_T("2D and 3D"),
	_T("3D only"),
};

static LPCTSTR g_lpszPhotonfocus3DAcquisitionMode[EDevicePhotonfocus3DAcquisitionMode_Count] =
{
	_T("Continuous"),
	_T("SingleFrame"),
	_T("MultiFrame"),
	_T("ContinuousRecording"),
	_T("ContinuousReadout"),
	_T("SingleFrameRecording"),
	_T("SingleFrameReadout"),
};

static LPCTSTR g_lpszPhotonfocus3DFlag[EDevicePhotonfocus3DFlag_Count] =
{
	_T("False"),
	_T("True"),
};

static LPCTSTR g_lpszPhotonfocus3DTriggerSelector[EDevicePhotonfocus3DTriggerSelector_Count] =
{
	_T("FrameStart"),
};

static LPCTSTR g_lpszPhotonfocus3DSwitch[EDevicePhotonfocus3DSwitch_Count] =
{
	_T("Off"),
	_T("On"),
};

static LPCTSTR g_lpszPhotonfocus3DTriggerSource[EDevicePhotonfocus3DTriggerSource_Count] =
{
	_T("Software"),
	_T("Line1"),
	_T("PLC_Q4"),
	_T("ABTrigger"),
};

static LPCTSTR g_lpszPhotonfocus3DActivation[EDevicePhotonfocus3DTriggerActivation_Count] =
{
	_T("RisingEdge"),
	_T("FallingEdge"),
};

static LPCTSTR g_lpszPhotonfocus3DABTriggerMode[EDevicePhotonfocus3DABTriggerMode_Count] =
{
	_T("Single"),
	_T("Dual"),
	_T("Quad"),
};

static LPCTSTR g_lpszPhotonfocus3DABTriggerDirection[EDevicePhotonfocus3DABTriggerDirection_Count] =
{
	_T("Forward"),
	_T("Backward"),
	_T("ForwardBackward"),
};

static LPCTSTR g_lpszPhotonfocus3DExposureMode[EDevicePhotonfocus3DExposureMode_Count] =
{
	_T("Timed"),
};

static LPCTSTR g_lpszPhotonfocus3DGainSelector[EDevicePhotonfocus3DGainSelector_Count] =
{
	_T("DigitalAll"),
};

static LPCTSTR g_lpszPhotonfocus3DBlackLevelSelector[EDevicePhotonfocus3DBlackLevelSelector_Count] =
{
	_T("All"),
};

static LPCTSTR g_lpszPhotonfocus3DMovingROISmoothing[EDevicePhotonfocus3DMovingROISmoothing_Count] =
{
	_T("None"),
	_T("MA8"),
	_T("MA16"),
};

CDevicePhotonfocus3D::CDevicePhotonfocus3D()
{
	m_pCamera = nullptr;
	m_pStream = nullptr;
	m_pPipeline = nullptr;
	m_llGrabCount = 0;
	m_nLastOffsetX = 0;
	m_nLastOffsetY = 0;
}


CDevicePhotonfocus3D::~CDevicePhotonfocus3D()
{
	Terminate();
}

EDeviceInitializeResult CDevicePhotonfocus3D::Initialize()
{
	EDeviceInitializeResult eReturn = EDeviceInitializeResult_UnknownError;

	CString strMessage, strStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("Photonfocus3D"));
		
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
		
		bool bFindCamera = false;

		PvSystem lSystem;
		PvDeviceInfo* pDeviceInfo = nullptr;

		int nTimeout = 2000;
		if(GetDetectTimeout(&nTimeout))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Detect timeout"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		lSystem.SetDetectionTimeout(nTimeout);

		if(!lSystem.Find().IsOK())
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Device find"));
			eReturn = EDeviceInitializeResult_NotFoundDeviceInfo;
			break;
		}

		PvUInt32 lInterFaceCount = lSystem.GetInterfaceCount();

		for(PvUInt32 i = 0; i < lInterFaceCount; ++i)
		{
			PvInterface* pInterFace = lSystem.GetInterface(i);

			PvUInt32 lDeviceCount = pInterFace->GetDeviceCount();

			for(PvUInt32 j = 0; j < lDeviceCount; ++j)
			{
				pDeviceInfo = pInterFace->GetDeviceInfo(j);
				PvString strTemp = pDeviceInfo->GetModel();
				if(!strcmp(strTemp, "MV1-D2048x1088-3D03-760-G2-10"))
				{
					strTemp = pDeviceInfo->GetSerialNumber().GetAscii();
					if(GetDeviceID())
					{
						if(!strcmp(strTemp, CStringA(GetDeviceID())))
						{
							bFindCamera = true;
							break;
						}
					}
				}
			}

			if(bFindCamera)
				break;
		}

		if(!bFindCamera)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtofindthedevice);
			eReturn = EDeviceInitializeResult_NotFoundDeviceError;
			break;
		}

		m_pCamera = new PvDevice;

		if(!m_pCamera || !m_pCamera->Connect(pDeviceInfo).IsOK())
		{
			if(m_pCamera)
			{
				delete m_pCamera;
				m_pCamera = nullptr;
			}

			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Device Info"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		m_pStream = new PvStream;
		if(!m_pStream || !m_pStream->Open(pDeviceInfo->GetIPAddress()).IsOK())
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Stream"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		m_pPipeline = new PvPipeline(m_pStream);
		if(!m_pPipeline)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Pipeline"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}
		
		PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

		if(!pDeviceParams)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("ParameterSetting"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		EDevicePhotonfocus3DViewerType eType = EDevicePhotonfocus3DViewerType_Count;

		if(GetViewerType(&eType))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ViewerType"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(!pDeviceParams->SetEnumValue("PeakDetector_Mode", eType).IsOK())
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ViewerType"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		int nPeakSize = 1;
		{
			if(GetPeakDetector_NrOfPeaks(&nPeakSize))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("PeakSize"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(!pDeviceParams->SetIntegerValue("PeakDetector_NrOfPeaks", nPeakSize).IsOK())
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("PeakSize"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}
		}

		int nWidth = 0;
		if(GetCanvasWidth(&nWidth))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("CanvasWidth"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(!nWidth)
		{
			PvInt64 pvWidth = 0;
			if(!pDeviceParams->GetIntegerValue("SensorWidth", pvWidth).IsOK())
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("SensorWidth"));
				eReturn = EDeviceInitializeResult_ReadOnDeviceError;
				break;
			}

			nWidth = pvWidth;

			if(SetCanvasWidth(nWidth))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("CanvasWidth"));
				eReturn = EDeviceInitializeResult_WriteToDatabaseError;
				break;
			}
		}

		if(!pDeviceParams->SetIntegerValue("Width", nWidth).IsOK())
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("CanvasWidth"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		int nHeight = 0;

		bool bErrorFind = true;

		switch(eType)
		{
		case EDevicePhotonfocus3DViewerType_2DOnly:
		case EDevicePhotonfocus3DViewerType_2DAnd3D:
			{
				if(GetCanvasHeight(&nHeight))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("CanvasHeight"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(!nHeight)
				{
					PvInt64 pvHeight = 0;

					if(!pDeviceParams->GetIntegerValue("SensorHeight", pvHeight).IsOK())
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("SensorHeight"));
						eReturn = EDeviceInitializeResult_ReadOnDeviceError;
						break;
					}

					nHeight = pvHeight;

					if(SetCanvasHeight(nHeight))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("CanvasHeight"));
						eReturn = EDeviceInitializeResult_WriteToDatabaseError;
						break;
					}
				}

				if(!pDeviceParams->SetIntegerValue("Height", nHeight).IsOK())
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("CanvasHeight"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				bErrorFind = false;
			}
			break;
		case EDevicePhotonfocus3DViewerType_3DOnly:
			bErrorFind = false;
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		// AcquisitionControl
		//////////////////////////////////////////////////////////////////////////
				
		{
			EDevicePhotonfocus3DAcquisitionMode eAcquisitionMode = EDevicePhotonfocus3DAcquisitionMode_Count;
			if(GetAcquisitionMode(&eAcquisitionMode))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AcquisitionMode"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(!pDeviceParams->SetEnumValue("AcquisitionMode", eAcquisitionMode).IsOK())
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("AcquisitionMode"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}
		}
		
		{
			int nAcquisitionFrameCount = 0;
			if(GetAcquisitionFrameCount(&nAcquisitionFrameCount))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AcquisitionFrameCount"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(!pDeviceParams->SetIntegerValue("AcquisitionFrameCount", nAcquisitionFrameCount).IsOK())
			{
				long long llValue = 0;

				if(!pDeviceParams->GetIntegerValue("AcquisitionFrameCount", llValue).IsOK())
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("AcquisitionFrameCount"));
					eReturn = EDeviceInitializeResult_ReadOnDeviceError;
					break;
				}

				nAcquisitionFrameCount = llValue;

				if(SetAcquisitionFrameCount(nAcquisitionFrameCount))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("AcquisitionFrameCount"));
					eReturn = EDeviceInitializeResult_WriteToDatabaseError;
					break;
				}

				if(!pDeviceParams->SetIntegerValue("AcquisitionFrameCount", nAcquisitionFrameCount).IsOK())
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("AcquisitionFrameCount"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}
				
			}
		}
		
		{
			EDevicePhotonfocus3DTriggerSelector eTriggerSelector = EDevicePhotonfocus3DTriggerSelector_Count;
			if(GetTriggerSelector(&eTriggerSelector))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TriggerSelector"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			PvInt32 nSet = -1;

			switch(eTriggerSelector)
			{
			case EDevicePhotonfocus3DTriggerSelector_FrameStart:
				nSet = 3;
				break;
			}

			if(nSet == -1 || !pDeviceParams->SetEnumValue("TriggerSelector", nSet).IsOK())
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TriggerSelector"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}
		}

		{
			EDevicePhotonfocus3DSwitch eTriggerMode = EDevicePhotonfocus3DSwitch_Count;
			if(GetTriggerMode(&eTriggerMode))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TriggerMode"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(!pDeviceParams->SetEnumValue("TriggerMode", eTriggerMode).IsOK())
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TriggerMode"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}
		}

		{
			EDevicePhotonfocus3DTriggerSource eTriggerSource = EDevicePhotonfocus3DTriggerSource_Count;
			if(GetTriggerSource(&eTriggerSource))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TriggerSource"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			PvInt32 nSet = -1;

			switch(eTriggerSource)
			{
			case EDevicePhotonfocus3DTriggerSource_Software:
				nSet = 0;
				break;
			case EDevicePhotonfocus3DTriggerSource_Line1:
				nSet = 2;
				break;
			case EDevicePhotonfocus3DTriggerSource_PLC_Q4:
				nSet = 25;
				break;
			case EDevicePhotonfocus3DTriggerSource_ABTrigger:
				nSet = 32;
				break;
			}

			if(nSet == -1 || !pDeviceParams->SetEnumValue("TriggerSource", nSet).IsOK())
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TriggerSource"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}
		}

		{
			EDevicePhotonfocus3DTriggerActivation eTriggerActivation = EDevicePhotonfocus3DTriggerActivation_Count;
			if(GetTriggerActivation(&eTriggerActivation))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TriggerActivation"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(!pDeviceParams->SetEnumValue("TriggerActivation", eTriggerActivation).IsOK())
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TriggerActivation"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}
		}

		{
			double dblTriggerDelay = 0.;
			if(GetTriggerDelay(&dblTriggerDelay))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TriggerDelay"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(!pDeviceParams->SetFloatValue("TriggerDelay", dblTriggerDelay).IsOK())
			{
				if(!pDeviceParams->GetFloatValue("TriggerDelay", dblTriggerDelay).IsOK())
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("TriggerDelay"));
					eReturn = EDeviceInitializeResult_ReadOnDeviceError;
					break;
				}

				if(SetTriggerDelay(dblTriggerDelay))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("TriggerDelay"));
					eReturn = EDeviceInitializeResult_WriteToDatabaseError;
					break;
				}

				if(!pDeviceParams->SetFloatValue("TriggerDelay", dblTriggerDelay).IsOK())
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TriggerDelay"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}
			}
		}

		{
			int nTriggerDivider = 1;
			if(GetTriggerDivider(&nTriggerDivider))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TriggerDivider"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(!pDeviceParams->SetIntegerValue("TriggerDivider", nTriggerDivider).IsOK())
			{
				long long llValue = 0;

				if(!pDeviceParams->GetIntegerValue("TriggerDivider", llValue).IsOK())
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("TriggerDivider"));
					eReturn = EDeviceInitializeResult_ReadOnDeviceError;
					break;
				}

				nTriggerDivider = llValue;

				if(SetTriggerDivider(nTriggerDivider))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("TriggerDivider"));
					eReturn = EDeviceInitializeResult_WriteToDatabaseError;
					break;
				}

				if(!pDeviceParams->SetIntegerValue("TriggerDivider", nTriggerDivider).IsOK())
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TriggerDivider"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}
			}
		}

		{
			EDevicePhotonfocus3DFlag eABTriggerDebounce = EDevicePhotonfocus3DFlag_Count;
			if(GetABTriggerDebounce(&eABTriggerDebounce))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ABTriggerDebounce"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(!pDeviceParams->SetBooleanValue("ABTriggerDebounce", eABTriggerDebounce).IsOK())
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ABTriggerDebounce"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}
		}

		{
			EDevicePhotonfocus3DABTriggerMode eABTriggerMode = EDevicePhotonfocus3DABTriggerMode_Count;
			if(GetABTriggerMode(&eABTriggerMode))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ABTriggerMode"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(!pDeviceParams->SetEnumValue("ABTriggerMode", eABTriggerMode).IsOK())
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ABTriggerMode"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}
		}

		{
			EDevicePhotonfocus3DABTriggerDirection eABTriggerDirection = EDevicePhotonfocus3DABTriggerDirection_Count;
			if(GetABTriggerDirection(&eABTriggerDirection))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ABTriggerDirection"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(!pDeviceParams->SetEnumValue("ABTriggerDirection", eABTriggerDirection).IsOK())
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ABTriggerDirection"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}
		}

		{
			int nABTriggerDivider = 1;
			if(GetABTriggerDivider(&nABTriggerDivider))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ABTriggerDivider"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(!pDeviceParams->SetIntegerValue("ABTriggerDivider", nABTriggerDivider).IsOK())
			{
				long long llValue = 0;

				if(!pDeviceParams->GetIntegerValue("ABTriggerDivider", llValue).IsOK())
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("ABTriggerDivider"));
					eReturn = EDeviceInitializeResult_ReadOnDeviceError;
					break;
				}

				nABTriggerDivider = llValue;

				if(SetABTriggerDivider(nABTriggerDivider))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("ABTriggerDivider"));
					eReturn = EDeviceInitializeResult_WriteToDatabaseError;
					break;
				}

				if(!pDeviceParams->SetIntegerValue("ABTriggerDivider", nABTriggerDivider).IsOK())
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ABTriggerDivider"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}				
			}
		}

		{
			int nABEncoderPosition = 1;
			if(GetABEncoderPosition(&nABEncoderPosition))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ABEncoderPosition"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(!pDeviceParams->SetIntegerValue("ABEncoderPosition", nABEncoderPosition).IsOK())
			{
				long long llValue = 0;

				if(!pDeviceParams->GetIntegerValue("ABEncoderPosition", llValue).IsOK())
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("ABEncoderPosition"));
					eReturn = EDeviceInitializeResult_ReadOnDeviceError;
					break;
				}

				nABEncoderPosition = llValue;

				if(SetABEncoderPosition(nABEncoderPosition))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("ABEncoderPosition"));
					eReturn = EDeviceInitializeResult_WriteToDatabaseError;
					break;
				}

				if(!pDeviceParams->SetIntegerValue("ABEncoderPosition", nABEncoderPosition).IsOK())
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ABEncoderPosition"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}
			}
			
			pDeviceParams->ExecuteCommand(" ABEncoderPositionUpdate");
		}

		{
			EDevicePhotonfocus3DExposureMode eExposureMode = EDevicePhotonfocus3DExposureMode_Count;
			if(GetExposureMode(&eExposureMode))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ExposureMode"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			PvInt32 nSet = -1;

			switch(eExposureMode)
			{
			case EDevicePhotonfocus3DExposureMode_Timed:
				nSet = 1;
				break;
			}

			if(nSet == -1 || !pDeviceParams->SetEnumValue("ExposureMode", nSet).IsOK())
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ExposureMode"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}
		}

		{
			double dblExposureTime = 0.;
			if(GetExposureTime(&dblExposureTime))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ExposureTime"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(!pDeviceParams->SetFloatValue("ExposureTime", dblExposureTime).IsOK())
			{
				if(!pDeviceParams->GetFloatValue("ExposureTime", dblExposureTime).IsOK())
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("ExposureTime"));
					eReturn = EDeviceInitializeResult_ReadOnDeviceError;
					break;
				}

				if(SetExposureTime(dblExposureTime))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("ExposureTime"));
					eReturn = EDeviceInitializeResult_WriteToDatabaseError;
					break;
				}

				if(!pDeviceParams->SetFloatValue("ExposureTime", dblExposureTime).IsOK())
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ExposureTime"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}				
			}
		}
		//////////////////////////////////////////////////////////////////////////
		// AnalogControl
		//////////////////////////////////////////////////////////////////////////
		{
			EDevicePhotonfocus3DGainSelector eGainSelector = EDevicePhotonfocus3DGainSelector_Count;
			if(GetGainSelector(&eGainSelector))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("GainSelector"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			PvInt32 nSet = -1;

			switch(eGainSelector)
			{
			case EDevicePhotonfocus3DGainSelector_DigitalAll:
				nSet = 18;
				break;
			}

			if(nSet == -1 || !pDeviceParams->SetEnumValue("GainSelector", nSet).IsOK())
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("GainSelector"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}
		}

		{
			double dblGain = 0.;
			if(GetGain(&dblGain))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Gain"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(!pDeviceParams->SetFloatValue("Gain", dblGain).IsOK())
			{
				if(!pDeviceParams->GetFloatValue("Gain", dblGain).IsOK())
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Gain"));
					eReturn = EDeviceInitializeResult_ReadOnDeviceError;
					break;
				}

				if(SetGain(dblGain))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Gain"));
					eReturn = EDeviceInitializeResult_WriteToDatabaseError;
					break;
				}

				if(!pDeviceParams->SetFloatValue("Gain", dblGain).IsOK())
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Gain"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}
			}
		}

		{
			EDevicePhotonfocus3DBlackLevelSelector eBlackLevelSelector = EDevicePhotonfocus3DBlackLevelSelector_Count;
			if(GetBlackLevelSelector(&eBlackLevelSelector))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("GainSelector"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			PvInt32 nSet = -1;

			switch(eBlackLevelSelector)
			{
			case EDevicePhotonfocus3DBlackLevelSelector_All:
				nSet = 0;
				break;
			}

			if(nSet == -1 || !pDeviceParams->SetEnumValue("BlackLevelSelector", nSet).IsOK())
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("BlackLevelSelector"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}
		}

		{
			double dblBlackLevel = 0.;
			if(GetBlackLevel(&dblBlackLevel))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("BlackLevel"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(!pDeviceParams->SetFloatValue("BlackLevel", dblBlackLevel).IsOK())
			{
				if(!pDeviceParams->GetFloatValue("BlackLevel", dblBlackLevel).IsOK())
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("BlackLevel"));
					eReturn = EDeviceInitializeResult_ReadOnDeviceError;
					break;
				}

				if(SetBlackLevel(dblBlackLevel))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("BlackLevel"));
					eReturn = EDeviceInitializeResult_WriteToDatabaseError;
					break;
				}
				
				if(!pDeviceParams->SetFloatValue("BlackLevel", dblBlackLevel).IsOK())
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("BlackLevel"));
					eReturn = EDeviceInitializeResult_WriteToDatabaseError;
					break;
				}
			}
		}

		{
			int nDigitalOffset = 1;
			if(GetDigitalOffset(&nDigitalOffset))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("DigitalOffset"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(!pDeviceParams->SetIntegerValue("DigitalOffset", nDigitalOffset).IsOK())
			{
				long long llValue = 0;

				if(!pDeviceParams->GetIntegerValue("DigitalOffset", llValue).IsOK())
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("DigitalOffset"));
					eReturn = EDeviceInitializeResult_ReadOnDeviceError;
					break;
				}

				nDigitalOffset = llValue;

				if(SetDigitalOffset(nDigitalOffset))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("DigitalOffset"));
					eReturn = EDeviceInitializeResult_WriteToDatabaseError;
					break;
				}

				if(!pDeviceParams->SetIntegerValue("DigitalOffset", nDigitalOffset).IsOK())
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("DigitalOffset"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// PeakDetector
		//////////////////////////////////////////////////////////////////////////

		if(eType != EDevicePhotonfocus3DViewerType_2DOnly)
		{
			{
				int nDataFormat3D = 0;
				if(GetPeakDetector_DataFormat3D(&nDataFormat3D))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("PeakDetector_DataFormat3D"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(!pDeviceParams->SetIntegerValue("PeakDetector_DataFormat3D", nDataFormat3D).IsOK())
				{
					long long llValue = 0;

					if(!pDeviceParams->GetIntegerValue("PeakDetector_DataFormat3D", llValue).IsOK())
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("PeakDetector_DataFormat3D"));
						eReturn = EDeviceInitializeResult_ReadOnDeviceError;
						break;
					}

					nDataFormat3D = llValue;

					if(SetPeakDetector_DataFormat3D(nDataFormat3D))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("PeakDetector_DataFormat3D"));
						eReturn = EDeviceInitializeResult_WriteToDatabaseError;
						break;
					}

					if(!pDeviceParams->SetIntegerValue("PeakDetector_DataFormat3D", nDataFormat3D).IsOK())
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("PeakDetector_DataFormat3D"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}
				}
			}

			EDevicePhotonfocus3DFlag eHighSpeed = EDevicePhotonfocus3DFlag_Count;
			{
				if(GetPeakDetector_HighSpeed(&eHighSpeed))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("PeakDetector_HighSpeed"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(!pDeviceParams->SetBooleanValue("PeakDetector_HighSpeed", eHighSpeed).IsOK())
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("PeakDetector_HighSpeed"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}
			}

			{
				int nPeak0_Threshold = 0;
				if(GetPeak0_Threshold(&nPeak0_Threshold))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Peak0_Threshold"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(!pDeviceParams->SetIntegerValue("Peak0_Threshold", nPeak0_Threshold).IsOK())
				{
					long long llValue = 0;

					if(!pDeviceParams->GetIntegerValue("Peak0_Threshold", llValue).IsOK())
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Peak0_Threshold"));
						eReturn = EDeviceInitializeResult_ReadOnDeviceError;
						break;
					}

					nPeak0_Threshold = llValue;

					if(SetPeak0_Threshold(nPeak0_Threshold))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Peak0_Threshold"));
						eReturn = EDeviceInitializeResult_WriteToDatabaseError;
						break;
					}

					if(!pDeviceParams->SetIntegerValue("Peak0_Threshold", nPeak0_Threshold).IsOK())
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Peak0_Threshold"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}
				}
			}

			{
				int nPeak0_3DY = 0;
				if(GetPeak0_3DY(&nPeak0_3DY))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Peak0_3DY"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(!pDeviceParams->SetIntegerValue("Peak0_3DY", nPeak0_3DY).IsOK())
				{
					long long llValue = 0;

					if(!pDeviceParams->GetIntegerValue("Peak0_3DY", llValue).IsOK())
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Peak0_3DY"));
						eReturn = EDeviceInitializeResult_ReadOnDeviceError;
						break;
					}

					nPeak0_3DY = llValue;

					if(SetPeak0_3DY(nPeak0_3DY))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Peak0_3DY"));
						eReturn = EDeviceInitializeResult_WriteToDatabaseError;
						break;
					}

					if(!pDeviceParams->SetIntegerValue("Peak0_3DY", nPeak0_3DY).IsOK())
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Peak0_3DY"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}
				}
			}

			{
				int nPeak0_3DH = 0;
				if(GetPeak0_3DH(&nPeak0_3DH))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Peak0_3DH"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(!pDeviceParams->SetIntegerValue("Peak0_3DH", nPeak0_3DH).IsOK())
				{
					long long llValue = 0;

					if(!pDeviceParams->GetIntegerValue("Peak0_3DH", llValue).IsOK())
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Peak0_3DH"));
						eReturn = EDeviceInitializeResult_ReadOnDeviceError;
						break;
					}

					nPeak0_3DH = llValue;

					if(SetPeak0_3DH(nPeak0_3DH))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Peak0_3DH"));
						eReturn = EDeviceInitializeResult_WriteToDatabaseError;
						break;
					}

					if(!pDeviceParams->SetIntegerValue("Peak0_3DH", nPeak0_3DH).IsOK())
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Peak0_3DH"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}
				}
			}
						
			{
				EDevicePhotonfocus3DFlag ePeak0_Mirror = EDevicePhotonfocus3DFlag_Count;
				if(GetPeak0_Mirror(&ePeak0_Mirror))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Peak0_Mirror"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(!pDeviceParams->SetBooleanValue("Peak0_Mirror", ePeak0_Mirror).IsOK())
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Peak0_Mirror"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}
			}

			{
				int nPeak0_2DY = 0;
				if(GetPeak0_2DY(&nPeak0_2DY))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Peak0_2DY"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(!pDeviceParams->SetIntegerValue("Peak0_2DY", nPeak0_2DY).IsOK())
				{
					long long llValue = 0;

					if(!pDeviceParams->GetIntegerValue("Peak0_2DY", llValue).IsOK())
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Peak0_2DY"));
						eReturn = EDeviceInitializeResult_ReadOnDeviceError;
						break;
					}

					nPeak0_2DY = llValue;

					if(SetPeak0_2DY(nPeak0_2DY))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Peak0_2DY"));
						eReturn = EDeviceInitializeResult_WriteToDatabaseError;
						break;
					}

					if(!pDeviceParams->SetIntegerValue("Peak0_2DY", nPeak0_2DY).IsOK())
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Peak0_2DY"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}
				}
			}

			{
				double dblPeak0_Gain = 0.;
				if(GetPeak0_Gain(&dblPeak0_Gain))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Peak0_Gain"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(!pDeviceParams->SetFloatValue("Peak0_Gain", dblPeak0_Gain).IsOK())
				{
					if(!pDeviceParams->GetFloatValue("Peak0_Gain", dblPeak0_Gain).IsOK())
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Peak0_Gain"));
						eReturn = EDeviceInitializeResult_ReadOnDeviceError;
						break;
					}

					if(SetPeak0_Gain(dblPeak0_Gain))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Peak0_Gain"));
						eReturn = EDeviceInitializeResult_WriteToDatabaseError;
						break;
					}

					if(!pDeviceParams->SetFloatValue("Peak0_Gain", dblPeak0_Gain).IsOK())
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Peak0_Gain"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}
				}
			}

			{
				int nPeak0_DigitalOffset = 0;
				if(GetPeak0_DigitalOffset(&nPeak0_DigitalOffset))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Peak0_DigitalOffset"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(!pDeviceParams->SetIntegerValue("Peak0_DigitalOffset", nPeak0_DigitalOffset).IsOK())
				{
					long long llValue = 0;

					if(!pDeviceParams->GetIntegerValue("Peak0_DigitalOffset", llValue).IsOK())
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Peak0_DigitalOffset"));
						eReturn = EDeviceInitializeResult_ReadOnDeviceError;
						break;
					}

					nPeak0_DigitalOffset = llValue;

					if(SetPeak0_DigitalOffset(nPeak0_DigitalOffset))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Peak0_DigitalOffset"));
						eReturn = EDeviceInitializeResult_WriteToDatabaseError;
						break;
					}

					if(!pDeviceParams->SetIntegerValue("Peak0_DigitalOffset", nPeak0_DigitalOffset).IsOK())
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Peak0_DigitalOffset"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}					
				}
			}

			{
				EDevicePhotonfocus3DFlag ePeak0_EnPeakFilter = EDevicePhotonfocus3DFlag_Count;
				if(GetPeak0_EnPeakFilter(&ePeak0_EnPeakFilter))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Peak0_EnPeakFilter"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(!pDeviceParams->SetBooleanValue("Peak0_EnPeakFilter", ePeak0_EnPeakFilter).IsOK())
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Peak0_EnPeakFilter"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}
			}

			{
				int nPeak0_PeakFilterHeightMin = 0;
				if(GetPeak0_PeakFilterHeightMin(&nPeak0_PeakFilterHeightMin))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Peak0_PeakFilterHeightMin"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(!pDeviceParams->SetIntegerValue("Peak0_PeakFilterHeightMin", nPeak0_PeakFilterHeightMin).IsOK())
				{
					long long llValue = 0;

					if(!pDeviceParams->GetIntegerValue("Peak0_PeakFilterHeightMin", llValue).IsOK())
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Peak0_PeakFilterHeightMin"));
						eReturn = EDeviceInitializeResult_ReadOnDeviceError;
						break;
					}

					nPeak0_PeakFilterHeightMin = llValue;

					if(SetPeak0_PeakFilterHeightMin(nPeak0_PeakFilterHeightMin))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Peak0_PeakFilterHeightMin"));
						eReturn = EDeviceInitializeResult_WriteToDatabaseError;
						break;
					}

					if(!pDeviceParams->SetIntegerValue("Peak0_PeakFilterHeightMin", nPeak0_PeakFilterHeightMin).IsOK())
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Peak0_PeakFilterHeightMin"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}
				}
			}

			{
				int nPeak0_PeakFilterHeightMax = 0;
				if(GetPeak0_PeakFilterHeightMax(&nPeak0_PeakFilterHeightMax))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Peak0_PeakFilterHeightMax"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(!pDeviceParams->SetIntegerValue("Peak0_PeakFilterHeightMax", nPeak0_PeakFilterHeightMax).IsOK())
				{
					long long llValue = 0;

					if(!pDeviceParams->GetIntegerValue("Peak0_PeakFilterHeightMax", llValue).IsOK())
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Peak0_PeakFilterHeightMax"));
						eReturn = EDeviceInitializeResult_ReadOnDeviceError;
						break;
					}

					nPeak0_PeakFilterHeightMax = llValue;

					if(SetPeak0_PeakFilterHeightMax(nPeak0_PeakFilterHeightMax))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Peak0_PeakFilterHeightMax"));
						eReturn = EDeviceInitializeResult_WriteToDatabaseError;
						break;
					}

					if(!pDeviceParams->SetIntegerValue("Peak0_PeakFilterHeightMax", nPeak0_PeakFilterHeightMax).IsOK())
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Peak0_PeakFilterHeightMax"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}
				}
			}

			{
				int nPeak0_PeakFilterWidthMin = 0;
				if(GetPeak0_PeakFilterWidthMin(&nPeak0_PeakFilterWidthMin))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Peak0_PeakFilterWidthMin"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(!pDeviceParams->SetIntegerValue("Peak0_PeakFilterWidthMin", nPeak0_PeakFilterWidthMin).IsOK())
				{
					long long llValue = 0;

					if(!pDeviceParams->GetIntegerValue("Peak0_PeakFilterWidthMin", llValue).IsOK())
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Peak0_PeakFilterWidthMin"));
						eReturn = EDeviceInitializeResult_ReadOnDeviceError;
						break;
					}

					nPeak0_PeakFilterWidthMin = llValue;

					if(SetPeak0_PeakFilterWidthMin(nPeak0_PeakFilterWidthMin))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Peak0_PeakFilterWidthMin"));
						eReturn = EDeviceInitializeResult_WriteToDatabaseError;
						break;
					}

					if(!pDeviceParams->SetIntegerValue("Peak0_PeakFilterWidthMin", nPeak0_PeakFilterWidthMin).IsOK())
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Peak0_PeakFilterWidthMin"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}
				}
			}

			{
				int nPeak0_PeakFilterWidthMax = 0;
				if(GetPeak0_PeakFilterWidthMax(&nPeak0_PeakFilterWidthMax))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Peak0_PeakFilterWidthMax"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(!pDeviceParams->SetIntegerValue("Peak0_PeakFilterWidthMax", nPeak0_PeakFilterWidthMax).IsOK())
				{
					long long llValue = 0;

					if(!pDeviceParams->GetIntegerValue("Peak0_PeakFilterWidthMax", llValue).IsOK())
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Peak0_PeakFilterWidthMax"));
						eReturn = EDeviceInitializeResult_ReadOnDeviceError;
						break;
					}

					nPeak0_PeakFilterWidthMax = llValue;

					if(SetPeak0_PeakFilterWidthMax(nPeak0_PeakFilterWidthMax))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Peak0_PeakFilterWidthMax"));
						eReturn = EDeviceInitializeResult_WriteToDatabaseError;
						break;
					}

					if(!pDeviceParams->SetIntegerValue("Peak0_PeakFilterWidthMax", nPeak0_PeakFilterWidthMax).IsOK())
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Peak0_PeakFilterWidthMax"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}
				}
			}

			{
				if(eHighSpeed != EDevicePhotonfocus3DFlag_True)
				{
					EDevicePhotonfocus3DFlag ePeak0_EnMovingROI = EDevicePhotonfocus3DFlag_Count;
					if(GetPeak0_EnMovingROI(&ePeak0_EnMovingROI))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Peak0_EnMovingROI"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(!pDeviceParams->SetBooleanValue("Peak0_EnMovingROI", ePeak0_EnMovingROI).IsOK())
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Peak0_EnMovingROI"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}
				}
			}

			{
				EDevicePhotonfocus3DFlag ePeak0_MovingROIEnLaserFinder = EDevicePhotonfocus3DFlag_Count;
				if(GetPeak0_MovingROIEnLaserFinder(&ePeak0_MovingROIEnLaserFinder))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Peak0_MovingROIEnLaserFinder"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(!pDeviceParams->SetBooleanValue("Peak0_MovingROIEnLaserFinder", ePeak0_MovingROIEnLaserFinder).IsOK())
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Peak0_MovingROIEnLaserFinder"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}
			}

			{
				int nPeak0_MovingROIRangeY = 0;
				if(GetPeak0_MovingROIRangeY(&nPeak0_MovingROIRangeY))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Peak0_MovingROIRangeY"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(!pDeviceParams->SetIntegerValue("Peak0_MovingROIRangeY", nPeak0_MovingROIRangeY).IsOK())
				{
					long long llValue = 0;

					if(!pDeviceParams->GetIntegerValue("Peak0_MovingROIRangeY", llValue).IsOK())
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Peak0_MovingROIRangeY"));
						eReturn = EDeviceInitializeResult_ReadOnDeviceError;
						break;
					}

					nPeak0_MovingROIRangeY = llValue;

					if(SetPeak0_MovingROIRangeY(nPeak0_MovingROIRangeY))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Peak0_MovingROIRangeY"));
						eReturn = EDeviceInitializeResult_WriteToDatabaseError;
						break;
					}

					if(!pDeviceParams->SetIntegerValue("Peak0_MovingROIRangeY", nPeak0_MovingROIRangeY).IsOK())
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Peak0_MovingROIRangeY"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}
				}
			}

			int nPeak0_MovingROIRangeH = 0;
			{
				if(GetPeak0_MovingROIRangeH(&nPeak0_MovingROIRangeH))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Peak0_MovingROIRangeH"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(!pDeviceParams->SetIntegerValue("Peak0_MovingROIRangeH", nPeak0_MovingROIRangeH).IsOK())
				{
					long long llValue = 0;

					if(!pDeviceParams->GetIntegerValue("Peak0_MovingROIRangeH", llValue).IsOK())
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Peak0_MovingROIRangeH"));
						eReturn = EDeviceInitializeResult_ReadOnDeviceError;
						break;
					}

					nPeak0_MovingROIRangeH = llValue;

					if(SetPeak0_MovingROIRangeH(nPeak0_MovingROIRangeH))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Peak0_MovingROIRangeH"));
						eReturn = EDeviceInitializeResult_WriteToDatabaseError;
						break;
					}

					if(!pDeviceParams->SetIntegerValue("Peak0_MovingROIRangeH", nPeak0_MovingROIRangeH).IsOK())
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Peak0_MovingROIRangeH"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}
				}
			}

			{
				int nPeak0_MovingROIMinCol = 0;
				if(GetPeak0_MovingROIMinCol(&nPeak0_MovingROIMinCol))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Peak0_MovingROIMinCol"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(!pDeviceParams->SetIntegerValue("Peak0_MovingROIMinCol", nPeak0_MovingROIMinCol).IsOK())
				{
					long long llValue = 0;

					if(!pDeviceParams->GetIntegerValue("Peak0_MovingROIMinCol", llValue).IsOK())
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Peak0_MovingROIMinCol"));
						eReturn = EDeviceInitializeResult_ReadOnDeviceError;
						break;
					}

					nPeak0_MovingROIMinCol = llValue;

					if(SetPeak0_MovingROIMinCol(nPeak0_MovingROIMinCol))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Peak0_MovingROIMinCol"));
						eReturn = EDeviceInitializeResult_WriteToDatabaseError;
						break;
					}

					if(!pDeviceParams->SetIntegerValue("Peak0_MovingROIMinCol", nPeak0_MovingROIMinCol).IsOK())
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Peak0_MovingROIMinCol"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}
				}
			}

			{
				int nPeak0_MovingROIMinDistance = 0;
				if(GetPeak0_MovingROIMinDistance(&nPeak0_MovingROIMinDistance))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Peak0_MovingROIMinDistance"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(!pDeviceParams->SetIntegerValue("Peak0_MovingROIMinDistance", nPeak0_MovingROIMinDistance).IsOK())
				{
					long long llValue = 0;

					if(!pDeviceParams->GetIntegerValue("Peak0_MovingROIMinDistance", llValue).IsOK())
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Peak0_MovingROIMinDistance"));
						eReturn = EDeviceInitializeResult_ReadOnDeviceError;
						break;
					}

					nPeak0_MovingROIMinDistance = llValue;

					if(SetPeak0_MovingROIMinDistance(nPeak0_MovingROIMinDistance))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Peak0_MovingROIMinDistance"));
						eReturn = EDeviceInitializeResult_WriteToDatabaseError;
						break;
					}

					if(!pDeviceParams->SetIntegerValue("Peak0_MovingROIMinDistance", nPeak0_MovingROIMinDistance).IsOK())
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Peak0_MovingROIMinDistance"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}					
				}
			}

			{
				EDevicePhotonfocus3DMovingROISmoothing ePeak0_MovingROISmoothing = EDevicePhotonfocus3DMovingROISmoothing_Count;
				if(GetPeak0_MovingROISmoothing(&ePeak0_MovingROISmoothing))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Peak0_MovingROISmoothing"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(!pDeviceParams->SetEnumValue("Peak0_MovingROISmoothing", ePeak0_MovingROISmoothing).IsOK())
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Peak0_MovingROISmoothing"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}
			}

			{
				EDevicePhotonfocus3DFlag ePeak0_EnAbsCoordinate = EDevicePhotonfocus3DFlag_Count;
				if(GetPeak0_EnAbsCoordinate(&ePeak0_EnAbsCoordinate))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Peak0_EnAbsCoordinate"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(!pDeviceParams->SetBooleanValue("Peak0_EnAbsCoordinate", ePeak0_EnAbsCoordinate).IsOK())
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Peak0_EnAbsCoordinate"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}
			}

			{
				int nPeak0_AbsCoordinateBase = 0;
				if(GetPeak0_AbsCoordinateBase(&nPeak0_AbsCoordinateBase))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Peak0_AbsCoordinateBase"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(!pDeviceParams->SetIntegerValue("Peak0_AbsCoordinateBase", nPeak0_AbsCoordinateBase).IsOK())
				{
					long long llValue = 0;

					if(!pDeviceParams->GetIntegerValue("Peak0_AbsCoordinateBase", llValue).IsOK())
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Peak0_AbsCoordinateBase"));
						eReturn = EDeviceInitializeResult_ReadOnDeviceError;
						break;
					}

					nPeak0_AbsCoordinateBase = llValue;

					if(SetPeak0_AbsCoordinateBase(nPeak0_AbsCoordinateBase))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Peak0_AbsCoordinateBase"));
						eReturn = EDeviceInitializeResult_WriteToDatabaseError;
						break;
					}

					if(!pDeviceParams->SetIntegerValue("Peak0_AbsCoordinateBase", nPeak0_AbsCoordinateBase).IsOK())
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Peak0_AbsCoordinateBase"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}					
				}
			}

			if(nPeakSize == 2)
			{
				{
					int nPeak1_Threshold = 0;
					if(GetPeak1_Threshold(&nPeak1_Threshold))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Peak1_Threshold"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(!pDeviceParams->SetIntegerValue("Peak1_Threshold", nPeak1_Threshold).IsOK())
					{
						long long llValue = 0;

						if(!pDeviceParams->GetIntegerValue("Peak1_Threshold", llValue).IsOK())
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Peak1_Threshold"));
							eReturn = EDeviceInitializeResult_ReadOnDeviceError;
							break;
						}

						nPeak1_Threshold = llValue;

						if(SetPeak1_Threshold(nPeak1_Threshold))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Peak1_Threshold"));
							eReturn = EDeviceInitializeResult_WriteToDatabaseError;
							break;
						}

						if(!pDeviceParams->SetIntegerValue("Peak1_Threshold", nPeak1_Threshold).IsOK())
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Peak1_Threshold"));
							eReturn = EDeviceInitializeResult_WriteToDeviceError;
							break;
						}
					}
				}

				{
					int nPeak1_3DY = 0;
					if(GetPeak1_3DY(&nPeak1_3DY))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Peak1_3DY"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(!pDeviceParams->SetIntegerValue("Peak1_3DY", nPeak1_3DY).IsOK())
					{
						long long llValue = 0;

						if(!pDeviceParams->GetIntegerValue("Peak1_3DY", llValue).IsOK())
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Peak1_3DY"));
							eReturn = EDeviceInitializeResult_ReadOnDeviceError;
							break;
						}

						nPeak1_3DY = llValue;

						if(SetPeak1_3DY(nPeak1_3DY))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Peak1_3DY"));
							eReturn = EDeviceInitializeResult_WriteToDatabaseError;
							break;
						}

						if(!pDeviceParams->SetIntegerValue("Peak1_3DY", nPeak1_3DY).IsOK())
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Peak1_3DY"));
							eReturn = EDeviceInitializeResult_WriteToDeviceError;
							break;
						}
					}
				}

				{
					int nPeak1_3DH = 0;
					if(GetPeak1_3DH(&nPeak1_3DH))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Peak1_3DH"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(!pDeviceParams->SetIntegerValue("Peak1_3DH", nPeak1_3DH).IsOK())
					{
						long long llValue = 0;

						if(!pDeviceParams->GetIntegerValue("Peak1_3DH", llValue).IsOK())
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Peak1_3DH"));
							eReturn = EDeviceInitializeResult_ReadOnDeviceError;
							break;
						}

						nPeak1_3DH = llValue;

						if(SetPeak1_3DH(nPeak1_3DH))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Peak1_3DH"));
							eReturn = EDeviceInitializeResult_WriteToDatabaseError;
							break;
						}

						if(!pDeviceParams->SetIntegerValue("Peak1_3DH", nPeak1_3DH).IsOK())
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Peak1_3DH"));
							eReturn = EDeviceInitializeResult_WriteToDeviceError;
							break;
						}
					}
				}

				{
					EDevicePhotonfocus3DFlag ePeak1_Mirror = EDevicePhotonfocus3DFlag_Count;
					if(GetPeak1_Mirror(&ePeak1_Mirror))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Peak1_Mirror"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(!pDeviceParams->SetBooleanValue("Peak1_Mirror", ePeak1_Mirror).IsOK())
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Peak1_Mirror"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}
				}

				{
					int nPeak1_2DY = 0;
					if(GetPeak1_2DY(&nPeak1_2DY))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Peak1_2DY"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(!pDeviceParams->SetIntegerValue("Peak1_2DY", nPeak1_2DY).IsOK())
					{
						long long llValue = 0;

						if(!pDeviceParams->GetIntegerValue("Peak1_2DY", llValue).IsOK())
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Peak1_2DY"));
							eReturn = EDeviceInitializeResult_ReadOnDeviceError;
							break;
						}

						nPeak1_2DY = llValue;

						if(SetPeak1_2DY(nPeak1_2DY))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Peak1_2DY"));
							eReturn = EDeviceInitializeResult_WriteToDatabaseError;
							break;
						}

						if(!pDeviceParams->SetIntegerValue("Peak1_2DY", nPeak1_2DY).IsOK())
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Peak1_2DY"));
							eReturn = EDeviceInitializeResult_WriteToDeviceError;
							break;
						}
					}
				}

				{
					double dblPeak1_Gain = 0.;
					if(GetPeak1_Gain(&dblPeak1_Gain))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Peak1_Gain"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(!pDeviceParams->SetFloatValue("Peak1_Gain", dblPeak1_Gain).IsOK())
					{
						if(!pDeviceParams->GetFloatValue("Peak1_Gain", dblPeak1_Gain).IsOK())
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Peak1_Gain"));
							eReturn = EDeviceInitializeResult_ReadOnDeviceError;
							break;
						}

						if(SetPeak1_Gain(dblPeak1_Gain))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Peak1_Gain"));
							eReturn = EDeviceInitializeResult_WriteToDatabaseError;
							break;
						}

						if(!pDeviceParams->SetFloatValue("Peak1_Gain", dblPeak1_Gain).IsOK())
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Peak1_Gain"));
							eReturn = EDeviceInitializeResult_WriteToDeviceError;
							break;
						}
					}
				}

				{
					int nPeak1_DigitalOffset = 0;
					if(GetPeak1_DigitalOffset(&nPeak1_DigitalOffset))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Peak1_DigitalOffset"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(!pDeviceParams->SetIntegerValue("Peak1_DigitalOffset", nPeak1_DigitalOffset).IsOK())
					{
						long long llValue = 0;

						if(!pDeviceParams->GetIntegerValue("Peak1_DigitalOffset", llValue).IsOK())
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Peak1_DigitalOffset"));
							eReturn = EDeviceInitializeResult_ReadOnDeviceError;
							break;
						}

						nPeak1_DigitalOffset = llValue;

						if(SetPeak1_DigitalOffset(nPeak1_DigitalOffset))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Peak1_DigitalOffset"));
							eReturn = EDeviceInitializeResult_WriteToDatabaseError;
							break;
						}

						if(!pDeviceParams->SetIntegerValue("Peak1_DigitalOffset", nPeak1_DigitalOffset).IsOK())
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Peak1_DigitalOffset"));
							eReturn = EDeviceInitializeResult_WriteToDeviceError;
							break;
						}
					}
				}

				{
					EDevicePhotonfocus3DFlag ePeak1_EnPeakFilter = EDevicePhotonfocus3DFlag_Count;
					if(GetPeak1_EnPeakFilter(&ePeak1_EnPeakFilter))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Peak1_EnPeakFilter"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(!pDeviceParams->SetBooleanValue("Peak1_EnPeakFilter", ePeak1_EnPeakFilter).IsOK())
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Peak1_EnPeakFilter"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}
				}

				{
					int nPeak1_PeakFilterHeightMin = 0;
					if(GetPeak1_PeakFilterHeightMin(&nPeak1_PeakFilterHeightMin))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Peak1_PeakFilterHeightMin"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(!pDeviceParams->SetIntegerValue("Peak1_PeakFilterHeightMin", nPeak1_PeakFilterHeightMin).IsOK())
					{
						long long llValue = 0;

						if(!pDeviceParams->GetIntegerValue("Peak1_PeakFilterHeightMin", llValue).IsOK())
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Peak1_PeakFilterHeightMin"));
							eReturn = EDeviceInitializeResult_ReadOnDeviceError;
							break;
						}

						nPeak1_PeakFilterHeightMin = llValue;

						if(SetPeak1_PeakFilterHeightMin(nPeak1_PeakFilterHeightMin))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Peak1_PeakFilterHeightMin"));
							eReturn = EDeviceInitializeResult_WriteToDatabaseError;
							break;
						}

						if(!pDeviceParams->SetIntegerValue("Peak1_PeakFilterHeightMin", nPeak1_PeakFilterHeightMin).IsOK())
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Peak1_PeakFilterHeightMin"));
							eReturn = EDeviceInitializeResult_WriteToDeviceError;
							break;
						}
					}
				}

				{
					int nPeak1_PeakFilterHeightMax = 0;
					if(GetPeak1_PeakFilterHeightMax(&nPeak1_PeakFilterHeightMax))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Peak1_PeakFilterHeightMax"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(!pDeviceParams->SetIntegerValue("Peak1_PeakFilterHeightMax", nPeak1_PeakFilterHeightMax).IsOK())
					{
						long long llValue = 0;

						if(!pDeviceParams->GetIntegerValue("Peak1_PeakFilterHeightMax", llValue).IsOK())
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Peak1_PeakFilterHeightMax"));
							eReturn = EDeviceInitializeResult_ReadOnDeviceError;
							break;
						}

						nPeak1_PeakFilterHeightMax = llValue;

						if(SetPeak1_PeakFilterHeightMax(nPeak1_PeakFilterHeightMax))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Peak1_PeakFilterHeightMax"));
							eReturn = EDeviceInitializeResult_WriteToDatabaseError;
							break;
						}

						if(!pDeviceParams->SetIntegerValue("Peak1_PeakFilterHeightMax", nPeak1_PeakFilterHeightMax).IsOK())
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Peak1_PeakFilterHeightMax"));
							eReturn = EDeviceInitializeResult_WriteToDeviceError;
							break;
						}
					}
				}

				{
					int nPeak1_PeakFilterWidthMin = 0;
					if(GetPeak1_PeakFilterWidthMin(&nPeak1_PeakFilterWidthMin))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Peak1_PeakFilterWidthMin"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(!pDeviceParams->SetIntegerValue("Peak1_PeakFilterWidthMin", nPeak1_PeakFilterWidthMin).IsOK())
					{
						long long llValue = 0;

						if(!pDeviceParams->GetIntegerValue("Peak1_PeakFilterWidthMin", llValue).IsOK())
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Peak1_PeakFilterWidthMin"));
							eReturn = EDeviceInitializeResult_ReadOnDeviceError;
							break;
						}

						nPeak1_PeakFilterWidthMin = llValue;

						if(SetPeak1_PeakFilterWidthMin(nPeak1_PeakFilterWidthMin))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Peak1_PeakFilterWidthMin"));
							eReturn = EDeviceInitializeResult_WriteToDatabaseError;
							break;
						}

						if(!pDeviceParams->SetIntegerValue("Peak1_PeakFilterWidthMin", nPeak1_PeakFilterWidthMin).IsOK())
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Peak1_PeakFilterWidthMin"));
							eReturn = EDeviceInitializeResult_WriteToDeviceError;
							break;
						}
					}
				}

				{
					int nPeak1_PeakFilterWidthMax = 0;
					if(GetPeak1_PeakFilterWidthMax(&nPeak1_PeakFilterWidthMax))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Peak1_PeakFilterWidthMax"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(!pDeviceParams->SetIntegerValue("Peak1_PeakFilterWidthMax", nPeak1_PeakFilterWidthMax).IsOK())
					{
						long long llValue = 0;

						if(!pDeviceParams->GetIntegerValue("Peak1_PeakFilterWidthMax", llValue).IsOK())
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Peak1_PeakFilterWidthMax"));
							eReturn = EDeviceInitializeResult_ReadOnDeviceError;
							break;
						}

						nPeak1_PeakFilterWidthMax = llValue;

						if(SetPeak1_PeakFilterWidthMax(nPeak1_PeakFilterWidthMax))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Peak1_PeakFilterWidthMax"));
							eReturn = EDeviceInitializeResult_WriteToDatabaseError;
							break;
						}

						if(!pDeviceParams->SetIntegerValue("Peak1_PeakFilterWidthMax", nPeak1_PeakFilterWidthMax).IsOK())
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Peak1_PeakFilterWidthMax"));
							eReturn = EDeviceInitializeResult_WriteToDeviceError;
							break;
						}
					}
				}

				{
					EDevicePhotonfocus3DFlag ePeak1_EnAbsCoordinate = EDevicePhotonfocus3DFlag_Count;
					if(GetPeak1_EnAbsCoordinate(&ePeak1_EnAbsCoordinate))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Peak1_EnAbsCoordinate"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(!pDeviceParams->SetBooleanValue("Peak1_EnAbsCoordinate", ePeak1_EnAbsCoordinate).IsOK())
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Peak1_EnAbsCoordinate"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}
				}

				{
					int nPeak1_AbsCoordinateBase = 0;
					if(GetPeak1_AbsCoordinateBase(&nPeak1_AbsCoordinateBase))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Peak1_AbsCoordinateBase"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(!pDeviceParams->SetIntegerValue("Peak1_AbsCoordinateBase", nPeak1_AbsCoordinateBase).IsOK())
					{
						long long llValue = 0;

						if(!pDeviceParams->GetIntegerValue("Peak1_AbsCoordinateBase", llValue).IsOK())
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Peak1_AbsCoordinateBase"));
							eReturn = EDeviceInitializeResult_ReadOnDeviceError;
							break;
						}

						nPeak1_AbsCoordinateBase = llValue;

						if(SetPeak1_AbsCoordinateBase(nPeak1_AbsCoordinateBase))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Peak1_AbsCoordinateBase"));
							eReturn = EDeviceInitializeResult_WriteToDatabaseError;
							break;
						}

						if(!pDeviceParams->SetIntegerValue("Peak1_AbsCoordinateBase", nPeak1_AbsCoordinateBase).IsOK())
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Peak1_AbsCoordinateBase"));
							eReturn = EDeviceInitializeResult_WriteToDeviceError;
							break;
						}
					}
				}

			}

		}

		//////////////////////////////////////////////////////////////////////////

		long long llSize = 0;

		if(!pDeviceParams->GetIntegerValue("HeightInterface", llSize).IsOK())
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("HeightInterface"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		if(!pDeviceParams->SetIntegerValue("Height", llSize).IsOK())
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Height"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		if(!m_pCamera->NegotiatePacketSize().IsOK())
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Packet size"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		long long llPayloadSize = 0;

		if(!pDeviceParams->GetIntegerValue("PayloadSize", llPayloadSize).IsOK())
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("PayloadSize"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}
		
		int nBufferCount = 1;

		if(GetBufferCount(&nBufferCount))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("BufferCount"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		m_pPipeline->SetBufferSize(static_cast<UINT>(llPayloadSize));

		if(!m_pPipeline->SetBufferCount(nBufferCount).IsOK())
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("BufferCount"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		if(!m_pCamera->SetStreamDestination(m_pStream->GetLocalIPAddress(), m_pStream->GetLocalPort()).IsOK())
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("StreamPort"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		if(m_pImage)
		{
			m_pImage->Clear();

			delete m_pImage;
			m_pImage = nullptr;
		}

		if(!pDeviceParams->GetIntegerValue("HeightInterface", llSize).IsOK())
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("HeightInterface"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		nHeight = llSize;

		long long llGrabCount = 0;

		Get3DImageHeight(&llGrabCount);

		if(!llGrabCount)
		{
			if(eType != EDevicePhotonfocus3DViewerType_2DOnly)
			{
				strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtocreateimagebuffer);
				eReturn = EDeviceInitializeResult_NotCreateImagebuffer;
				break;
			}
		}

		switch(eType)
		{
		case EDevicePhotonfocus3DViewerType_2DOnly:
			{
				m_pImage = new CRavidImage3DProfilePhotonFocus();
				m_pImage->CreateImageBuffer(nWidth, nHeight, 0xff, Ravid::Algorithms::CRavidImage::MakeValueFormat(1, 8));
			}
			break;
		case EDevicePhotonfocus3DViewerType_2DAnd3D:
		case EDevicePhotonfocus3DViewerType_3DOnly:
			{
				int nBackSize = nPeakSize * 4;

				m_pImage = new CRavidImage3DProfilePhotonFocus();

				BYTE* pData = new BYTE[llPayloadSize];
				memset(pData, 255, llPayloadSize);

				m_pImage->SetImagePtr(pData, nWidth, nHeight, Ravid::Algorithms::CRavidImage::EValueFormat_1C_U8, 0, 4, false);
				m_pImage->Set3DOffset((nHeight - nBackSize) * nWidth);
				m_pImage->Initialize(nWidth, 1);
			}
 			break;
		default:
			break;
		}

		if(!m_pImage)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtocreateimagebuffer);
			eReturn = EDeviceInitializeResult_NotCreateImagebuffer;
			break;
		}
		
		CRavidImageView* pCurView = GetImageView();

		if(pCurView)
			pCurView->SetImageInfo(m_pImage);

		m_bIsLive = false;
		m_bIsGrabAvailable = true;

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

EDeviceTerminateResult CDevicePhotonfocus3D::Terminate()
{
	EDeviceTerminateResult eReturn = EDeviceTerminateResult_UnknownError;

	CString strMessage;

	SetStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("Photonfocus3D"));

		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);

		return EDeviceTerminateResult_NotFoundApiError;
	}

	do 
	{
		if(IsLive() || !IsGrabAvailable())
			Stop();

		if(m_pPipeline)
		{
			if(m_pPipeline->IsStarted())
			{
				PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

				if(pDeviceParams)
				{
					pDeviceParams->ExecuteCommand("AcquisitionStop");
					pDeviceParams->SetIntegerValue("TLParamsLocked", 0);
				}
				if(m_pPipeline->IsStarted())
					m_pPipeline->Stop();
			}
			delete m_pPipeline;
			m_pPipeline = nullptr;
		}

		if(m_pStream)
		{
			if(m_pStream->IsOpen())
				m_pStream->Close();
			delete m_pStream;
			m_pStream = nullptr;
		}

		if(m_pCamera)
		{
			if(m_pCamera->IsConnected())
				m_pCamera->Disconnect();

			delete m_pCamera;
			m_pCamera = nullptr;
		}

		if(m_pImage)
		{
			CRavidImageView* pCurView = GetImageView();

			if(pCurView)
			{
				if(pCurView->GetImageInfo() == m_pImage && pCurView->GetSafeHwnd())
					pCurView->SetImageInfo(nullptr);
			}

			m_pImage->Terminate();

			m_pImage->Clear();

			delete m_pImage;
			m_pImage = nullptr;
		}

		m_bIsLive = false;
		m_bIsInitialized = false;
		m_bIsGrabAvailable = true;

		strMessage = CMultiLanguageManager::GetString(ELanguageParameter_TheDevicehasbeensuccessfullyterminated);

		eReturn = EDeviceTerminateResult_OK;
				
		CEventHandlerManager::BroadcastOnDeviceTerminated(this);
	} 
	while(false);
	
	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDevicePhotonfocus3D::LoadSettings()
{
	bool bReturn = false;

	m_vctParameterFieldConfigurations.clear();

	do
	{
		CString strTime;
		strTime.Format(_T("%d"), INT_MAX);
		
 		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_DeviceID, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_DeviceID], _T("0"), EParameterFieldType_Edit);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_CameraSetting, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_CameraSetting], _T("1"), EParameterFieldType_None);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_GrabCount, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_GrabCount], _T("1"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_Viewertype, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_Viewertype], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszPhotonfocus3DViewerType, EDevicePhotonfocus3DViewerType_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_3DImageHeight, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_3DImageHeight], _T("2500"), EParameterFieldType_Edit, nullptr, nullptr, 1);
 		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_CanvasWidth, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_CanvasWidth], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
 		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_CanvasHeight, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_CanvasHeight], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
 		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_OffsetX, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_OffsetX], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_OffsetY, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_OffsetY], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_GrabWaitingTime, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_GrabWaitingTime], _T("2048"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_DetectTimeout, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_DetectTimeout], _T("2000"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_BufferCount, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_BufferCount], _T("2"), EParameterFieldType_Edit, nullptr, nullptr, 1);


		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_AcquisitionControl, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_AcquisitionControl], _T("1"), EParameterFieldType_None);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_AcquisitionMode, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_AcquisitionMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszPhotonfocus3DAcquisitionMode, EDevicePhotonfocus3DAcquisitionMode_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_AcquisitionFrameCount, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_AcquisitionFrameCount], _T("1"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_TriggerSelector, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_TriggerSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszPhotonfocus3DTriggerSelector, EDevicePhotonfocus3DTriggerSelector_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_TriggerMode, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_TriggerMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszPhotonfocus3DSwitch, EDevicePhotonfocus3DSwitch_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_TriggerSource, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_TriggerSource], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszPhotonfocus3DTriggerSource, EDevicePhotonfocus3DTriggerSource_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_TriggerActivation, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_TriggerActivation], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszPhotonfocus3DActivation, EDevicePhotonfocus3DTriggerActivation_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_TriggerDelay, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_TriggerDelay], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_TriggerDivider, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_TriggerDivider], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_ABTriggerDebounce, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_ABTriggerDebounce], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszPhotonfocus3DFlag, EDevicePhotonfocus3DFlag_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_ABTriggerMode, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_ABTriggerMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszPhotonfocus3DABTriggerMode, EDevicePhotonfocus3DABTriggerMode_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_ABTriggerDirection, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_ABTriggerDirection], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszPhotonfocus3DABTriggerDirection, EDevicePhotonfocus3DABTriggerDirection_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_ABTriggerDivider, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_ABTriggerDivider], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_ABEncoderPosition, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_ABEncoderPosition], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_ExposureMode, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_ExposureMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszPhotonfocus3DExposureMode, EDevicePhotonfocus3DExposureMode_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_ExposureTime, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_ExposureTime], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);


		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_TimerControl, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_TimerControl], _T("1"), EParameterFieldType_None);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_TimerDurationRaw, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_TimerDurationRaw], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_TimerDelayRaw, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_TimerDelayRaw], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_TimerTriggerSource, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_TimerTriggerSource], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_TimerTriggerActivation, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_TimerTriggerActivation], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_TimerGranularityFactor, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_TimerGranularityFactor], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_AnalogControl, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_AnalogControl], _T("1"), EParameterFieldType_None);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_GainSelector, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_GainSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszPhotonfocus3DGainSelector, EDevicePhotonfocus3DGainSelector_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_Gain, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_Gain], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_BlackLevelSelector, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_BlackLevelSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszPhotonfocus3DBlackLevelSelector, EDevicePhotonfocus3DBlackLevelSelector_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_BlackLevel, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_BlackLevel], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_DigitalOffset, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_DigitalOffset], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_PeakDetector, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_PeakDetector], _T("0"), EParameterFieldType_None, nullptr, nullptr, 0);

		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_Peak0, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_Peak0], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_Peak0_3D, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_Peak0_3D], _T("0"), EParameterFieldType_None, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_Peak0_Threshold, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_Peak0_Threshold], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_Peak0_3DY, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_Peak0_3DY], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_Peak0_3DH, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_Peak0_3DH], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_Peak0_Mirror, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_Peak0_Mirror], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszPhotonfocus3DFlag, EDevicePhotonfocus3DFlag_Count), nullptr, 3);

		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_Peak0_2D, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_Peak0_2D], _T("0"), EParameterFieldType_None, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_Peak0_2DY, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_Peak0_2DY], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_Peak0_Gain, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_Peak0_Gain], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_Peak0_DigitalOffset, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_Peak0_DigitalOffset], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 3);

		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_Peak0_PeakFilter, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_Peak0_PeakFilter], _T("0"), EParameterFieldType_None, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_Peak0_EnPeakFilter, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_Peak0_EnPeakFilter], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszPhotonfocus3DFlag, EDevicePhotonfocus3DFlag_Count), nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_Peak0_PeakFilterHeightMin, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_Peak0_PeakFilterHeightMin], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_Peak0_PeakFilterHeightMax, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_Peak0_PeakFilterHeightMax], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_Peak0_PeakFilterWidthMin, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_Peak0_PeakFilterWidthMin], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_Peak0_PeakFilterWidthMax, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_Peak0_PeakFilterWidthMax], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 3);

		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_Peak0_MovingROI, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_Peak0_MovingROI], _T("0"), EParameterFieldType_None, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_Peak0_MovingROIEnLaserFinder, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_Peak0_MovingROIEnLaserFinder], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszPhotonfocus3DFlag, EDevicePhotonfocus3DFlag_Count), nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_Peak0_MovingROIRangeY, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_Peak0_MovingROIRangeY], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_Peak0_MovingROIRangeH, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_Peak0_MovingROIRangeH], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_Peak0_MovingROIMinCol, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_Peak0_MovingROIMinCol], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_Peak0_MovingROIMinDistance, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_Peak0_MovingROIMinDistance], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_Peak0_MovingROISmoothing, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_Peak0_MovingROISmoothing], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszPhotonfocus3DMovingROISmoothing, EDevicePhotonfocus3DMovingROISmoothing_Count), nullptr, 3);

		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_Peak0_EnAbsCoordinate, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_Peak0_EnAbsCoordinate], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszPhotonfocus3DFlag, EDevicePhotonfocus3DFlag_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_Peak0_AbsCoordinateBase, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_Peak0_AbsCoordinateBase], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_Peak1, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_Peak1], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_Peak1_3D, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_Peak1_3D], _T("0"), EParameterFieldType_None, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_Peak1_Threshold, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_Peak1_Threshold], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_Peak1_3DY, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_Peak1_3DY], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_Peak1_3DH, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_Peak1_3DH], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_Peak1_Mirror, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_Peak1_Mirror], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszPhotonfocus3DFlag, EDevicePhotonfocus3DFlag_Count), nullptr, 3);

		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_Peak1_2D, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_Peak1_2D], _T("0"), EParameterFieldType_None, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_Peak1_2DY, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_Peak1_2DY], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_Peak1_Gain, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_Peak1_Gain], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_Peak1_DigitalOffset, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_Peak1_DigitalOffset], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 3);

		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_Peak1_PeakFilter, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_Peak1_PeakFilter], _T("0"), EParameterFieldType_None, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_Peak1_EnPeakFilter, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_Peak1_EnPeakFilter], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszPhotonfocus3DFlag, EDevicePhotonfocus3DFlag_Count), nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_Peak1_PeakFilterHeightMin, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_Peak1_PeakFilterHeightMin], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_Peak1_PeakFilterHeightMax, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_Peak1_PeakFilterHeightMax], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_Peak1_PeakFilterWidthMin, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_Peak1_PeakFilterWidthMin], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 3);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_Peak1_PeakFilterWidthMax, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_Peak1_PeakFilterWidthMax], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 3);

		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_Peak1_EnAbsCoordinate, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_Peak1_EnAbsCoordinate], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszPhotonfocus3DFlag, EDevicePhotonfocus3DFlag_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_Peak1_AbsCoordinateBase, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_Peak1_AbsCoordinateBase], _T("1020"), EParameterFieldType_Edit, nullptr, nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_PeakDetector_NrOfPeaks, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_PeakDetector_NrOfPeaks], _T("1"), EParameterFieldType_Edit, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_PeakDetector_DataFormat3D, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_PeakDetector_DataFormat3D], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterPhotonfocus3D_PeakDetector_HighSpeed, g_lpszParamDevicePhotonfocus3D[EDeviceParameterPhotonfocus3D_PeakDetector_HighSpeed], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszPhotonfocus3DFlag, EDevicePhotonfocus3DFlag_Count), nullptr, 2);

		bReturn = true;
	}
	while(false);

	return bReturn & __super::LoadSettings();
}

EDeviceGrabResult CDevicePhotonfocus3D::Grab()
{
	EDeviceGrabResult eReturn = EDeviceGrabResult_UnknownError;

	CString strMessage;

	do
	{
		m_llGrabCount = 0;

		long long llGrabCount = 0;

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

		if(GetGrabCount(&llGrabCount))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("GrabCount"));
			eReturn = EDeviceGrabResult_ReadOnDatabaseError;
			break;
		}

		if(llGrabCount <= 0)
		{
			m_llGrabCount = ULONGLONG_MAX;
			m_bIsLive = true;
		}
		else
		{
			m_llGrabCount = llGrabCount;
			m_bIsGrabAvailable = false;
		}

		PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

		m_pPipeline->Start();
		pDeviceParams->SetIntegerValue("TLParamsLocked", 1);
		pDeviceParams->ExecuteCommand("GevTimestampControlReset");
		pDeviceParams->ExecuteCommand("AcquisitionStart");

		m_pLiveThread = AfxBeginThread(CDevicePhotonfocus3D::CallbackFunction, this, 0, 0U, CREATE_SUSPENDED);

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

EDeviceLiveResult CDevicePhotonfocus3D::Live()
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
		m_llGrabCount = LONGLONG_MAX;
		m_bIsLive = true;

		PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

		m_pPipeline->Start();
		pDeviceParams->SetIntegerValue("TLParamsLocked", 1);
		pDeviceParams->ExecuteCommand("GevTimestampControlReset");
		pDeviceParams->ExecuteCommand("AcquisitionStart");

		m_pLiveThread = AfxBeginThread(CDevicePhotonfocus3D::CallbackFunction, this, 0, 0U, CREATE_SUSPENDED);

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

EDeviceStopResult CDevicePhotonfocus3D::Stop()
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

		PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

		if(!pDeviceParams)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Parameter"));
			eReturn = EDeviceStopResult_WriteToDeviceError;
			break;
		}

		pDeviceParams->ExecuteCommand("AcquisitionStop");
		pDeviceParams->SetIntegerValue("TLParamsLocked", 0);
		if(m_pPipeline->IsStarted())
			m_pPipeline->Stop();
		
		if(WaitForSingleObject(m_pLiveThread->m_hThread, 5000) == WAIT_TIMEOUT)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("StopThread"));
			eReturn = EDeviceStopResult_WriteToDeviceError;
			break;
		}

		m_bIsLive = false;
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

EDeviceTriggerResult CDevicePhotonfocus3D::Trigger()
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

		PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

		if(!pDeviceParams)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Parameter"));
			eReturn = EDeviceTriggerResult_WriteToDeviceError;
			break;
		}

		pDeviceParams->ExecuteCommand("TriggerSoftware");

		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("Trigger"));

		eReturn = EDeviceTriggerResult_OK;
	} 
	while (false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetViewerType(EDevicePhotonfocus3DViewerType * pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = (EDevicePhotonfocus3DViewerType)_ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_Viewertype));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetViewerType(EDevicePhotonfocus3DViewerType eParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_Viewertype;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDevicePhotonfocus3DViewerType)0 || eParam > EDevicePhotonfocus3DViewerType_Count)
		{
			eReturn = EPhotonfocus3DSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], g_lpszPhotonfocus3DViewerType[nPreValue], g_lpszPhotonfocus3DViewerType[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetGrabCount(long long * pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoll(GetParamValue(EDeviceParameterPhotonfocus3D_GrabCount));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetGrabCount(long long ullParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_GrabCount;

	long long ullPreValue = _ttoll(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%lld"), ullParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter %s from %lld to %lld"), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], ullPreValue, ullParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetCanvasWidth(int * pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do 
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_CanvasWidth));

		eReturn = EPhotonfocus3DGetFunction_OK;
	} 
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetCanvasWidth(int nParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_CanvasWidth;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::Get3DImageHeight(long long * pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoll(GetParamValue(EDeviceParameterPhotonfocus3D_3DImageHeight));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::Set3DImageHeight(long long ullParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_3DImageHeight;

	long long ullPreValue = _ttoll(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%lld"), ullParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter %s from %lld to %lld"), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], ullPreValue, ullParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetCanvasHeight(int * pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_CanvasHeight));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetCanvasHeight(int nParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_CanvasHeight;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetOffsetX(int * pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_OffsetX));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetOffsetX(int nParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_OffsetX;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetIntegerValue("OffsetX", nParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetOffsetY(int * pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_OffsetY));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetOffsetY(int nParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_OffsetY;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetIntegerValue("OffsetY", nParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetBufferCount(int * pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_BufferCount));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetBufferCount(int nParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_BufferCount;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetGrabWaitingTime(unsigned long long * pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoll(GetParamValue(EDeviceParameterPhotonfocus3D_GrabWaitingTime));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetGrabWaitingTime(unsigned long long ullParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_GrabWaitingTime;

	unsigned long long ullPreValue = _ttoll(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%llu"), ullParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter %s from %llu to %llu"), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], ullPreValue, ullParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetDetectTimeout(int * pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_DetectTimeout));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetDetectTimeout(int nParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_DetectTimeout;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetAcquisitionMode(EDevicePhotonfocus3DAcquisitionMode * pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = (EDevicePhotonfocus3DAcquisitionMode)_ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_AcquisitionMode));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetAcquisitionMode(EDevicePhotonfocus3DAcquisitionMode eParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_AcquisitionMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDevicePhotonfocus3DAcquisitionMode)0 || eParam > EDevicePhotonfocus3DAcquisitionMode_Count)
		{
			eReturn = EPhotonfocus3DSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetEnumValue("AcquisitionMode", eParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], g_lpszPhotonfocus3DAcquisitionMode[nPreValue], g_lpszPhotonfocus3DAcquisitionMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetAcquisitionFrameCount(int * pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_AcquisitionFrameCount));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetAcquisitionFrameCount(int nParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_AcquisitionFrameCount;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(nParam < 1 || nParam > 255)
		{
			eReturn = EPhotonfocus3DSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetIntegerValue("AcquisitionFrameCount", nParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetTriggerSelector(EDevicePhotonfocus3DTriggerSelector * pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = (EDevicePhotonfocus3DTriggerSelector)_ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_TriggerSelector));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetTriggerSelector(EDevicePhotonfocus3DTriggerSelector eParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_TriggerSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDevicePhotonfocus3DTriggerSelector)0 || eParam > EDevicePhotonfocus3DTriggerSelector_Count)
		{
			eReturn = EPhotonfocus3DSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			PvInt32 nSet = -1;

			switch(eParam)
			{
			case EDevicePhotonfocus3DTriggerSelector_FrameStart:
				nSet = 3;
				break;
			default:
				break;
			}

			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(nSet == -1 || !pDeviceParams->SetEnumValue("TriggerSelector", nSet).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], g_lpszPhotonfocus3DTriggerSelector[nPreValue], g_lpszPhotonfocus3DTriggerSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetTriggerMode(EDevicePhotonfocus3DSwitch * pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = (EDevicePhotonfocus3DSwitch)_ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_TriggerMode));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetTriggerMode(EDevicePhotonfocus3DSwitch eParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_TriggerMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDevicePhotonfocus3DSwitch)0 || eParam > EDevicePhotonfocus3DSwitch_Count)
		{
			eReturn = EPhotonfocus3DSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetEnumValue("TriggerMode", eParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], g_lpszPhotonfocus3DSwitch[nPreValue], g_lpszPhotonfocus3DSwitch[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetTriggerSource(EDevicePhotonfocus3DTriggerSource * pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = (EDevicePhotonfocus3DTriggerSource)_ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_TriggerSource));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetTriggerSource(EDevicePhotonfocus3DTriggerSource eParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_TriggerSource;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDevicePhotonfocus3DTriggerSource)0 || eParam > EDevicePhotonfocus3DTriggerSource_Count)
		{
			eReturn = EPhotonfocus3DSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			PvInt32 nSet = -1;

			switch(eParam)
			{
			case EDevicePhotonfocus3DTriggerSource_Software:
				nSet = 0;
				break;
			case EDevicePhotonfocus3DTriggerSource_Line1:
				nSet = 2;
				break;
			case EDevicePhotonfocus3DTriggerSource_PLC_Q4:
				nSet = 25;
				break;
			case EDevicePhotonfocus3DTriggerSource_ABTrigger:
				nSet = 32;
				break;
			default:
				break;
			}

			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(nSet == -1 || !pDeviceParams->SetEnumValue("TriggerSource", nSet).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], g_lpszPhotonfocus3DTriggerSource[nPreValue], g_lpszPhotonfocus3DTriggerSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetTriggerActivation(EDevicePhotonfocus3DTriggerActivation * pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = (EDevicePhotonfocus3DTriggerActivation)_ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_TriggerActivation));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetTriggerActivation(EDevicePhotonfocus3DTriggerActivation eParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_TriggerActivation;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDevicePhotonfocus3DTriggerActivation)0 || eParam > EDevicePhotonfocus3DTriggerActivation_Count)
		{
			eReturn = EPhotonfocus3DSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetEnumValue("TriggerActivation", eParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], g_lpszPhotonfocus3DActivation[nPreValue], g_lpszPhotonfocus3DActivation[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetTriggerDelay(double * pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterPhotonfocus3D_TriggerDelay));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetTriggerDelay(double dblParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_TriggerDelay;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(dblParam < 0. || dblParam > 349000.)
		{
			eReturn = EPhotonfocus3DSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetFloatValue("TriggerDelay", dblParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetTriggerDivider(int * pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_TriggerDivider));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetTriggerDivider(int nParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_TriggerDivider;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(nParam < 1 || nParam > 256)
		{
			eReturn = EPhotonfocus3DSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetIntegerValue("TriggerDivider", nParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetABTriggerDebounce(EDevicePhotonfocus3DFlag * pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = (EDevicePhotonfocus3DFlag)_ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_ABTriggerDebounce));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetABTriggerDebounce(EDevicePhotonfocus3DFlag eParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_ABTriggerDebounce;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDevicePhotonfocus3DFlag)0 || eParam > EDevicePhotonfocus3DFlag_Count)
		{
			eReturn = EPhotonfocus3DSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetBooleanValue("ABTriggerDebounce", eParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], g_lpszPhotonfocus3DFlag[nPreValue], g_lpszPhotonfocus3DFlag[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetABTriggerMode(EDevicePhotonfocus3DABTriggerMode * pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = (EDevicePhotonfocus3DABTriggerMode)_ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_ABTriggerMode));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetABTriggerMode(EDevicePhotonfocus3DABTriggerMode eParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_ABTriggerMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDevicePhotonfocus3DABTriggerMode)0 || eParam > EDevicePhotonfocus3DABTriggerMode_Count)
		{
			eReturn = EPhotonfocus3DSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetEnumValue("ABTriggerMode", eParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], g_lpszPhotonfocus3DABTriggerMode[nPreValue], g_lpszPhotonfocus3DABTriggerMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetABTriggerDirection(EDevicePhotonfocus3DABTriggerDirection * pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = (EDevicePhotonfocus3DABTriggerDirection)_ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_ABTriggerDirection));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetABTriggerDirection(EDevicePhotonfocus3DABTriggerDirection eParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_ABTriggerDirection;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDevicePhotonfocus3DABTriggerDirection)0 || eParam > EDevicePhotonfocus3DABTriggerDirection_Count)
		{
			eReturn = EPhotonfocus3DSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetEnumValue("ABTriggerDirection", eParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], g_lpszPhotonfocus3DABTriggerDirection[nPreValue], g_lpszPhotonfocus3DABTriggerDirection[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetABTriggerDivider(int * pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_ABTriggerDivider));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetABTriggerDivider(int nParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_ABTriggerDivider;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(nParam < 1 || nParam > 65536)
		{
			eReturn = EPhotonfocus3DSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetIntegerValue("ABTriggerDivider", nParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetABEncoderPosition(int * pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_ABEncoderPosition));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetABEncoderPosition(int nParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_ABEncoderPosition;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetIntegerValue("ABEncoderPosition", nParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetExposureMode(EDevicePhotonfocus3DExposureMode * pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = (EDevicePhotonfocus3DExposureMode)_ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_ExposureMode));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetExposureMode(EDevicePhotonfocus3DExposureMode eParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_ExposureMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDevicePhotonfocus3DExposureMode)0 || eParam > EDevicePhotonfocus3DExposureMode_Count)
		{
			eReturn = EPhotonfocus3DSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			PvInt32 nSet = -1;

			switch(eParam)
			{
			case EDevicePhotonfocus3DExposureMode_Timed:
				nSet = 1;
				break;
			default:
				break;
			}

			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(nSet == -1 || !pDeviceParams->SetEnumValue("ExposureMode", nSet).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], g_lpszPhotonfocus3DExposureMode[nPreValue], g_lpszPhotonfocus3DExposureMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetExposureTime(double * pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterPhotonfocus3D_ExposureTime));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetExposureTime(double dblParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_ExposureTime;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(dblParam < 1. || dblParam > 349000.)
		{
			eReturn = EPhotonfocus3DSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetFloatValue("ExposureTime", dblParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetGainSelector(EDevicePhotonfocus3DGainSelector * pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = (EDevicePhotonfocus3DGainSelector)_ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_GainSelector));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetGainSelector(EDevicePhotonfocus3DGainSelector eParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_GainSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDevicePhotonfocus3DGainSelector)0 || eParam > EDevicePhotonfocus3DGainSelector_Count)
		{
			eReturn = EPhotonfocus3DSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			PvInt32 nSet = -1;

			switch(eParam)
			{
			case EDevicePhotonfocus3DGainSelector_DigitalAll:
				nSet = 18;
				break;
			default:
				break;
			}

			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(nSet == -1 || !pDeviceParams->SetEnumValue("GainSelector", nSet).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], g_lpszPhotonfocus3DGainSelector[nPreValue], g_lpszPhotonfocus3DGainSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetGain(double * pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterPhotonfocus3D_Gain));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetGain(double dblParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_Gain;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(dblParam < 0.1 || dblParam > 15.9999)
		{
			eReturn = EPhotonfocus3DSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetFloatValue("Gain", dblParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetBlackLevelSelector(EDevicePhotonfocus3DBlackLevelSelector * pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = (EDevicePhotonfocus3DBlackLevelSelector)_ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_BlackLevelSelector));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetBlackLevelSelector(EDevicePhotonfocus3DBlackLevelSelector eParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_BlackLevelSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDevicePhotonfocus3DBlackLevelSelector)0 || eParam > EDevicePhotonfocus3DBlackLevelSelector_Count)
		{
			eReturn = EPhotonfocus3DSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			PvInt32 nSet = -1;

			switch(eParam)
			{
			case EDevicePhotonfocus3DBlackLevelSelector_All:
				nSet = 0;
				break;
			default:
				break;
			}

			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(nSet == -1 || !pDeviceParams->SetEnumValue("BlackLevelSelector", nSet).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}
		 
		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], g_lpszPhotonfocus3DBlackLevelSelector[nPreValue], g_lpszPhotonfocus3DBlackLevelSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetBlackLevel(double * pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterPhotonfocus3D_BlackLevel));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetBlackLevel(double dblParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_BlackLevel;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(dblParam < 0 || dblParam > 16383)
		{
			eReturn = EPhotonfocus3DSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetFloatValue("BlackLevel", dblParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetDigitalOffset(int * pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_DigitalOffset));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetDigitalOffset(int nParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_DigitalOffset;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetIntegerValue("DigitalOffset", nParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetPeak0_Threshold(int* pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_Peak0_Threshold));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetPeak0_Threshold(int nParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_Peak0_Threshold;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetIntegerValue("Peak0_Threshold", nParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetPeak0_3DY(int* pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_Peak0_3DY));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetPeak0_3DY(int nParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_Peak0_3DY;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetIntegerValue("Peak0_3DY", nParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetPeak0_3DH(int* pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_Peak0_3DH));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetPeak0_3DH(int nParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_Peak0_3DH;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetIntegerValue("Peak0_3DH", nParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetPeak0_Mirror(EDevicePhotonfocus3DFlag* pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = (EDevicePhotonfocus3DFlag)_ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_Peak0_Mirror));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetPeak0_Mirror(EDevicePhotonfocus3DFlag eParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_Peak0_Mirror;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDevicePhotonfocus3DFlag)0 || eParam > EDevicePhotonfocus3DFlag_Count)
		{
			eReturn = EPhotonfocus3DSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetBooleanValue("Peak0_Mirror", eParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], g_lpszPhotonfocus3DFlag[nPreValue], g_lpszPhotonfocus3DFlag[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetPeak0_2DY(int* pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_Peak0_2DY));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetPeak0_2DY(int nParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_Peak0_2DY;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetIntegerValue("Peak0_2DY", nParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetPeak0_Gain(double* pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_Peak0_Gain));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetPeak0_Gain(double dblParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_Peak0_Gain;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(dblParam < 0 || dblParam > 16383)
		{
			eReturn = EPhotonfocus3DSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetFloatValue("Peak0_Gain", dblParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetPeak0_DigitalOffset(int* pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_Peak0_DigitalOffset));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetPeak0_DigitalOffset(int nParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_Peak0_DigitalOffset;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetIntegerValue("Peak0_DigitalOffset", nParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetPeak0_EnPeakFilter(EDevicePhotonfocus3DFlag* pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = (EDevicePhotonfocus3DFlag)_ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_Peak0_EnPeakFilter));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetPeak0_EnPeakFilter(EDevicePhotonfocus3DFlag eParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_Peak0_EnPeakFilter;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDevicePhotonfocus3DFlag)0 || eParam > EDevicePhotonfocus3DFlag_Count)
		{
			eReturn = EPhotonfocus3DSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetBooleanValue("Peak0_EnPeakFilter", eParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], g_lpszPhotonfocus3DFlag[nPreValue], g_lpszPhotonfocus3DFlag[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetPeak0_PeakFilterHeightMin(int* pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_Peak0_PeakFilterHeightMin));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetPeak0_PeakFilterHeightMin(int nParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_Peak0_PeakFilterHeightMin;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetIntegerValue("Peak0_PeakFilterHeightMin", nParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetPeak0_PeakFilterHeightMax(int* pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_Peak0_PeakFilterHeightMax));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetPeak0_PeakFilterHeightMax(int nParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_Peak0_PeakFilterHeightMax;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetIntegerValue("Peak0_PeakFilterHeightMax", nParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetPeak0_PeakFilterWidthMin(int* pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_Peak0_PeakFilterWidthMin));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetPeak0_PeakFilterWidthMin(int nParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_Peak0_PeakFilterWidthMin;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetIntegerValue("Peak0_PeakFilterWidthMin", nParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetPeak0_PeakFilterWidthMax(int* pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_Peak0_PeakFilterWidthMax));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetPeak0_PeakFilterWidthMax(int nParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_Peak0_PeakFilterWidthMax;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetIntegerValue("Peak0_PeakFilterWidthMax", nParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetPeak0_EnMovingROI(EDevicePhotonfocus3DFlag* pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = (EDevicePhotonfocus3DFlag)_ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_Peak0_EnMovingROI));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetPeak0_EnMovingROI(EDevicePhotonfocus3DFlag eParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_Peak0_EnMovingROI;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDevicePhotonfocus3DFlag)0 || eParam > EDevicePhotonfocus3DFlag_Count)
		{
			eReturn = EPhotonfocus3DSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetBooleanValue("Peak0_EnMovingROI", eParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], g_lpszPhotonfocus3DFlag[nPreValue], g_lpszPhotonfocus3DFlag[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetPeak0_MovingROIEnLaserFinder(EDevicePhotonfocus3DFlag* pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = (EDevicePhotonfocus3DFlag)_ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_Peak0_MovingROIEnLaserFinder));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetPeak0_MovingROIEnLaserFinder(EDevicePhotonfocus3DFlag eParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_Peak0_MovingROIEnLaserFinder;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDevicePhotonfocus3DFlag)0 || eParam > EDevicePhotonfocus3DFlag_Count)
		{
			eReturn = EPhotonfocus3DSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetBooleanValue("Peak0_MovingROIEnLaserFinder", eParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], g_lpszPhotonfocus3DFlag[nPreValue], g_lpszPhotonfocus3DFlag[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetPeak0_MovingROIRangeY(int* pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_Peak0_MovingROIRangeY));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetPeak0_MovingROIRangeY(int nParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_Peak0_MovingROIRangeY;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetIntegerValue("Peak0_MovingROIRangeY", nParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetPeak0_MovingROIRangeH(int* pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_Peak0_MovingROIRangeH));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetPeak0_MovingROIRangeH(int nParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_Peak0_MovingROIRangeH;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetIntegerValue("Peak0_MovingROIRangeH", nParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetPeak0_MovingROIMinCol(int* pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_Peak0_MovingROIMinCol));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetPeak0_MovingROIMinCol(int nParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_Peak0_MovingROIMinCol;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetIntegerValue("Peak0_MovingROIMinCol", nParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetPeak0_MovingROIMinDistance(int* pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_Peak0_MovingROIMinDistance));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetPeak0_MovingROIMinDistance(int nParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_Peak0_MovingROIMinDistance;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetIntegerValue("Peak0_MovingROIMinDistance", nParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetPeak0_MovingROISmoothing(EDevicePhotonfocus3DMovingROISmoothing* pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = (EDevicePhotonfocus3DMovingROISmoothing)_ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_Peak0_MovingROISmoothing));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetPeak0_MovingROISmoothing(EDevicePhotonfocus3DMovingROISmoothing eParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_Peak0_MovingROISmoothing;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDevicePhotonfocus3DAcquisitionMode)0 || eParam > EDevicePhotonfocus3DAcquisitionMode_Count)
		{
			eReturn = EPhotonfocus3DSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetEnumValue("Peak0_MovingROISmoothing", eParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], g_lpszPhotonfocus3DMovingROISmoothing[nPreValue], g_lpszPhotonfocus3DMovingROISmoothing[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetPeak0_EnAbsCoordinate(EDevicePhotonfocus3DFlag* pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = (EDevicePhotonfocus3DFlag)_ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_Peak0_EnAbsCoordinate));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetPeak0_EnAbsCoordinate(EDevicePhotonfocus3DFlag eParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_Peak0_EnAbsCoordinate;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDevicePhotonfocus3DFlag)0 || eParam > EDevicePhotonfocus3DFlag_Count)
		{
			eReturn = EPhotonfocus3DSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetBooleanValue("Peak0_EnAbsCoordinate", eParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], g_lpszPhotonfocus3DFlag[nPreValue], g_lpszPhotonfocus3DFlag[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetPeak0_AbsCoordinateBase(int* pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_Peak0_AbsCoordinateBase));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetPeak0_AbsCoordinateBase(int nParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_Peak0_AbsCoordinateBase;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetIntegerValue("Peak0_AbsCoordinateBase", nParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetPeak1_Threshold(int* pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_Peak1_Threshold));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetPeak1_Threshold(int nParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_Peak1_Threshold;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetIntegerValue("Peak1_Threshold", nParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetPeak1_3DY(int* pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_Peak1_3DY));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetPeak1_3DY(int nParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_Peak1_3DY;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetIntegerValue("Peak1_3DY", nParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetPeak1_3DH(int* pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_Peak1_3DH));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetPeak1_3DH(int nParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_Peak1_3DH;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetIntegerValue("Peak1_3DH", nParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetPeak1_Mirror(EDevicePhotonfocus3DFlag* pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = (EDevicePhotonfocus3DFlag)_ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_Peak1_Mirror));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetPeak1_Mirror(EDevicePhotonfocus3DFlag eParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_Peak1_Mirror;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDevicePhotonfocus3DFlag)0 || eParam > EDevicePhotonfocus3DFlag_Count)
		{
			eReturn = EPhotonfocus3DSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetBooleanValue("Peak1_Mirror", eParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], g_lpszPhotonfocus3DFlag[nPreValue], g_lpszPhotonfocus3DFlag[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetPeak1_2DY(int* pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_Peak1_2DY));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetPeak1_2DY(int nParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_Peak1_2DY;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetIntegerValue("Peak1_2DY", nParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetPeak1_Gain(double* pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_Peak1_Gain));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetPeak1_Gain(double dblParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_Peak1_Gain;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(dblParam < 0 || dblParam > 16383)
		{
			eReturn = EPhotonfocus3DSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetFloatValue("Peak1_Gain", dblParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetPeak1_DigitalOffset(int* pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_Peak1_DigitalOffset));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetPeak1_DigitalOffset(int nParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_Peak1_DigitalOffset;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetIntegerValue("Peak1_DigitalOffset", nParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetPeak1_EnPeakFilter(EDevicePhotonfocus3DFlag* pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = (EDevicePhotonfocus3DFlag)_ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_Peak1_EnPeakFilter));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetPeak1_EnPeakFilter(EDevicePhotonfocus3DFlag eParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_Peak1_EnPeakFilter;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDevicePhotonfocus3DFlag)0 || eParam > EDevicePhotonfocus3DFlag_Count)
		{
			eReturn = EPhotonfocus3DSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetBooleanValue("Peak1_EnPeakFilter", eParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], g_lpszPhotonfocus3DFlag[nPreValue], g_lpszPhotonfocus3DFlag[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetPeak1_PeakFilterHeightMin(int* pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_Peak1_PeakFilterHeightMin));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetPeak1_PeakFilterHeightMin(int nParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_Peak1_PeakFilterHeightMin;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetIntegerValue("Peak1_PeakFilterHeightMin", nParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetPeak1_PeakFilterHeightMax(int* pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_Peak1_PeakFilterHeightMax));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetPeak1_PeakFilterHeightMax(int nParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_Peak1_PeakFilterHeightMax;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetIntegerValue("Peak1_PeakFilterHeightMax", nParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetPeak1_PeakFilterWidthMin(int* pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_Peak1_PeakFilterWidthMin));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetPeak1_PeakFilterWidthMin(int nParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_Peak1_PeakFilterWidthMin;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetIntegerValue("Peak1_PeakFilterWidthMin", nParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetPeak1_PeakFilterWidthMax(int* pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_Peak1_PeakFilterWidthMax));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetPeak1_PeakFilterWidthMax(int nParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_Peak1_PeakFilterWidthMax;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetIntegerValue("Peak1_PeakFilterWidthMax", nParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetPeak1_EnAbsCoordinate(EDevicePhotonfocus3DFlag* pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = (EDevicePhotonfocus3DFlag)_ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_Peak1_EnAbsCoordinate));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetPeak1_EnAbsCoordinate(EDevicePhotonfocus3DFlag eParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_Peak1_EnAbsCoordinate;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDevicePhotonfocus3DFlag)0 || eParam > EDevicePhotonfocus3DFlag_Count)
		{
			eReturn = EPhotonfocus3DSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetBooleanValue("Peak1_EnAbsCoordinate", eParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], g_lpszPhotonfocus3DFlag[nPreValue], g_lpszPhotonfocus3DFlag[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetPeak1_AbsCoordinateBase(int* pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_Peak1_AbsCoordinateBase));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetPeak1_AbsCoordinateBase(int nParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_Peak1_AbsCoordinateBase;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetIntegerValue("Peak1_AbsCoordinateBase", nParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetPeakDetector_NrOfPeaks(int * pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_PeakDetector_NrOfPeaks));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetPeakDetector_NrOfPeaks(int nParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_PeakDetector_NrOfPeaks;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(nParam < 0 || nParam > 1)
		{
			eReturn = EPhotonfocus3DSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetIntegerValue("PeakDetector_NrOfPeaks", nParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetPeakDetector_DataFormat3D(int * pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_PeakDetector_DataFormat3D));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetPeakDetector_DataFormat3D(int nParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = (EDeviceParameterPhotonfocus3D_PeakDetector_DataFormat3D);

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(nParam < 0 || nParam > 1)
		{
			eReturn = EPhotonfocus3DSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetIntegerValue("PeakDetector_DataFormat3D", nParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPhotonfocus3DGetFunction CDevicePhotonfocus3D::GetPeakDetector_HighSpeed(EDevicePhotonfocus3DFlag * pParam)
{
	EPhotonfocus3DGetFunction eReturn = EPhotonfocus3DGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPhotonfocus3DGetFunction_NullptrError;
			break;
		}

		*pParam = (EDevicePhotonfocus3DFlag)_ttoi(GetParamValue(EDeviceParameterPhotonfocus3D_PeakDetector_HighSpeed));

		eReturn = EPhotonfocus3DGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPhotonfocus3DSetFunction CDevicePhotonfocus3D::SetPeakDetector_HighSpeed(EDevicePhotonfocus3DFlag eParam)
{
	EPhotonfocus3DSetFunction eReturn = EPhotonfocus3DSetFunction_UnknownError;

	EDeviceParameterPhotonfocus3D eSaveID = EDeviceParameterPhotonfocus3D_PeakDetector_HighSpeed;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDevicePhotonfocus3DFlag)0 || eParam > EDevicePhotonfocus3DFlag_Count)
		{
			eReturn = EPhotonfocus3DSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			PvGenParameterArray* pDeviceParams = m_pCamera->GetGenParameters();

			if(!pDeviceParams->SetBooleanValue("PeakDetector_HighSpeed", eParam).IsOK())
			{
				eReturn = EPhotonfocus3DSetFunction_AlreadyInitializedError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EPhotonfocus3DSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPhotonfocus3DSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDevicePhotonfocus3D[eSaveID], g_lpszPhotonfocus3DFlag[nPreValue], g_lpszPhotonfocus3DFlag[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDevicePhotonfocus3D::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = false;
	bool bFoundID = true;

	do
	{
		switch(nParam)
		{
		case EDeviceParameterPhotonfocus3D_DeviceID:
			bReturn = !SetDeviceID(strValue);
			break;
		case EDeviceParameterPhotonfocus3D_Viewertype:
			bReturn = !SetViewerType((EDevicePhotonfocus3DViewerType)_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_GrabCount:
			bReturn = !SetGrabCount(_ttoll(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_3DImageHeight:
			bReturn = !Set3DImageHeight(_ttoll(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_CanvasWidth:
			bReturn = !SetCanvasWidth(_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_CanvasHeight:
			bReturn = !SetCanvasHeight(_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_OffsetX:
			bReturn = !SetOffsetX(_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_OffsetY:
			bReturn = !SetOffsetY(_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_BufferCount:
			bReturn = !SetBufferCount(_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_GrabWaitingTime:
			bReturn = !SetGrabWaitingTime(_ttoll(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_DetectTimeout:
			bReturn = !SetDetectTimeout(_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_AcquisitionMode:
			bReturn = !SetAcquisitionMode((EDevicePhotonfocus3DAcquisitionMode)_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_AcquisitionFrameCount:
			bReturn = !SetAcquisitionFrameCount(_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_TriggerSelector:
			bReturn = !SetTriggerSelector((EDevicePhotonfocus3DTriggerSelector)_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_TriggerMode:
			bReturn = !SetTriggerMode((EDevicePhotonfocus3DSwitch)_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_TriggerSource:
			bReturn = !SetTriggerSource((EDevicePhotonfocus3DTriggerSource)_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_TriggerActivation:
			bReturn = !SetTriggerActivation((EDevicePhotonfocus3DTriggerActivation)_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_TriggerDelay:
			bReturn = !SetTriggerDelay(_ttof(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_TriggerDivider:
			bReturn = !SetTriggerDivider(_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_ABTriggerDebounce:
			bReturn = !SetABTriggerDebounce((EDevicePhotonfocus3DFlag)_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_ABTriggerMode:
			bReturn = !SetABTriggerMode((EDevicePhotonfocus3DABTriggerMode)_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_ABTriggerDirection:
			bReturn = !SetABTriggerDirection((EDevicePhotonfocus3DABTriggerDirection)_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_ABTriggerDivider:
			bReturn = !SetABTriggerDivider(_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_ABEncoderPosition:
			bReturn = !SetABEncoderPosition(_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_ExposureMode:
			bReturn = !SetExposureMode((EDevicePhotonfocus3DExposureMode)_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_ExposureTime:
			bReturn = !SetExposureTime(_ttof(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_GainSelector:
			bReturn = !SetGainSelector((EDevicePhotonfocus3DGainSelector)_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_Gain:
			bReturn = !SetGain(_ttof(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_BlackLevelSelector:
			bReturn = !SetBlackLevelSelector((EDevicePhotonfocus3DBlackLevelSelector)_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_BlackLevel:
			bReturn = !SetBlackLevel(_ttof(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_DigitalOffset:
			bReturn = !SetDigitalOffset(_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_Peak0_Threshold:
			bReturn = !SetPeak0_Threshold(_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_Peak0_3DY:
			bReturn = !SetPeak0_3DY(_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_Peak0_3DH:
			bReturn = !SetPeak0_3DH(_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_Peak0_Mirror:
			bReturn = !SetPeak0_Mirror((EDevicePhotonfocus3DFlag)_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_Peak0_2DY:
			bReturn = !SetPeak0_2DY(_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_Peak0_Gain:
			bReturn = !SetPeak0_Gain(_ttof(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_Peak0_DigitalOffset:
			bReturn = !SetPeak0_DigitalOffset(_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_Peak0_EnPeakFilter:
			bReturn = !SetPeak0_EnPeakFilter((EDevicePhotonfocus3DFlag)_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_Peak0_PeakFilterHeightMin:
			bReturn = !SetPeak0_PeakFilterHeightMin(_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_Peak0_PeakFilterHeightMax:
			bReturn = !SetPeak0_PeakFilterHeightMax(_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_Peak0_PeakFilterWidthMin:
			bReturn = !SetPeak0_PeakFilterWidthMin(_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_Peak0_PeakFilterWidthMax:
			bReturn = !SetPeak0_PeakFilterWidthMax(_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_Peak0_EnMovingROI:
			bReturn = !SetPeak0_EnMovingROI((EDevicePhotonfocus3DFlag)_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_Peak0_MovingROIEnLaserFinder:
			bReturn = !SetPeak0_MovingROIEnLaserFinder((EDevicePhotonfocus3DFlag)_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_Peak0_MovingROIRangeY:
			bReturn = !SetPeak0_MovingROIRangeY(_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_Peak0_MovingROIRangeH:
			bReturn = !SetPeak0_MovingROIRangeH(_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_Peak0_MovingROIMinCol:
			bReturn = !SetPeak0_MovingROIMinCol(_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_Peak0_MovingROIMinDistance:
			bReturn = !SetPeak0_MovingROIMinDistance(_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_Peak0_MovingROISmoothing:
			bReturn = !SetPeak0_MovingROISmoothing((EDevicePhotonfocus3DMovingROISmoothing)_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_Peak0_EnAbsCoordinate:
			bReturn = !SetPeak0_EnAbsCoordinate((EDevicePhotonfocus3DFlag)_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_Peak0_AbsCoordinateBase:
			bReturn = !SetPeak0_AbsCoordinateBase(_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_Peak1_Threshold:
			bReturn = !SetPeak1_Threshold(_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_Peak1_3DY:
			bReturn = !SetPeak1_3DY(_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_Peak1_3DH:
			bReturn = !SetPeak1_3DH(_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_Peak1_Mirror:
			bReturn = !SetPeak1_Mirror((EDevicePhotonfocus3DFlag)_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_Peak1_2DY:
			bReturn = !SetPeak1_2DY(_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_Peak1_Gain:
			bReturn = !SetPeak1_Gain(_ttof(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_Peak1_DigitalOffset:
			bReturn = !SetPeak1_DigitalOffset(_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_Peak1_EnPeakFilter:
			bReturn = !SetPeak1_EnPeakFilter((EDevicePhotonfocus3DFlag)_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_Peak1_PeakFilterHeightMin:
			bReturn = !SetPeak1_PeakFilterHeightMin(_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_Peak1_PeakFilterHeightMax:
			bReturn = !SetPeak1_PeakFilterHeightMax(_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_Peak1_PeakFilterWidthMin:
			bReturn = !SetPeak1_PeakFilterWidthMin(_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_Peak1_PeakFilterWidthMax:
			bReturn = !SetPeak1_PeakFilterWidthMax(_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_Peak1_EnAbsCoordinate:
			bReturn = !SetPeak1_EnAbsCoordinate((EDevicePhotonfocus3DFlag)_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_Peak1_AbsCoordinateBase:
			bReturn = !SetPeak1_AbsCoordinateBase(_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_PeakDetector_NrOfPeaks:
			bReturn = !SetPeakDetector_NrOfPeaks(_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_PeakDetector_DataFormat3D:
			bReturn = !SetPeakDetector_DataFormat3D(_ttoi(strValue));
			break;
		case EDeviceParameterPhotonfocus3D_PeakDetector_HighSpeed:
			bReturn = !SetPeakDetector_HighSpeed((EDevicePhotonfocus3DFlag)_ttoi(strValue));
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

bool CDevicePhotonfocus3D::DoesModuleExist()
{
	bool bReturn = false;

	CString strModuleName;



	do
	{
#ifndef _WIN64
		strModuleName.Format(_T("PvDevice.dll"));
#else
		strModuleName.Format(_T("PvDevice64.dll"));
#endif
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

#ifndef _WIN64
		strModuleName.Format(_T("PvBuffer.dll"));
#else
		strModuleName.Format(_T("PvBuffer64.dll"));
#endif

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

		if(!bReturn)
			break;

#ifndef _WIN64
		strModuleName.Format(_T("PvBase.dll"));
#else
		strModuleName.Format(_T("PvBase64.dll"));
#endif
		
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

		if(!bReturn)
			break;

#ifndef _WIN64
		strModuleName.Format(_T("PvGeniCam.dll"));
#else
		strModuleName.Format(_T("PvGeniCam64.dll"));
#endif

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

		if(!bReturn)
			break;

#ifndef _WIN64
		strModuleName.Format(_T("PvStream.dll"));
#else
		strModuleName.Format(_T("PvStream64.dll"));
#endif

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

		if(!bReturn)
			break;

#ifndef _WIN64
		strModuleName.Format(_T("PvStreamRaw.dll"));
#else
		strModuleName.Format(_T("PvStreamRaw64.dll"));
#endif

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

UINT CDevicePhotonfocus3D::CallbackFunction(LPVOID pParam)
{
	CDevicePhotonfocus3D* pPhotonfocus3D = (CDevicePhotonfocus3D*)pParam;


	unsigned long long ullTimes = 0xffffff;

	do 
	{
		if(!pPhotonfocus3D)
			break;

		EDevicePhotonfocus3DViewerType EViewerType = EDevicePhotonfocus3DViewerType_Count;

		if(pPhotonfocus3D->GetViewerType(&EViewerType))
			break;
		
		if(!pPhotonfocus3D->m_pStream)
			break;

		if(!pPhotonfocus3D->m_pPipeline)
			break;

		do 
		{
			if(!pPhotonfocus3D->m_pStream->IsOpen())
				break;

			if(!pPhotonfocus3D->m_pPipeline->IsStarted())
				break;

			PvBuffer* pBuffer = nullptr;

			PvResult  lOperationResult;

			if(pPhotonfocus3D->m_pPipeline->RetrieveNextBuffer(&pBuffer, ullTimes, &lOperationResult).IsOK())
			{
				if(lOperationResult.IsOK())
				{
					PvImage* pImage = nullptr;
					BYTE* pNewBufferMan = nullptr;
					SLaserData* pResult = nullptr;

					do
					{
						if(!pBuffer)
							break;

						pImage = pBuffer->GetImage();

						if(!pImage)
							break;

						size_t szBufferSize = pImage->GetImageSize();

						if(szBufferSize <= 0)
							break;

						bool bMemoryCopy = true;

						switch(EViewerType)
						{
						case EDevicePhotonfocus3DViewerType_2DOnly:
							{
								memcpy(pPhotonfocus3D->m_pImage->GetBuffer(), pBuffer->GetDataPointer(), szBufferSize);
							}
							break;
						case EDevicePhotonfocus3DViewerType_2DAnd3D:
						case EDevicePhotonfocus3DViewerType_3DOnly:
							{
								memcpy(pPhotonfocus3D->m_pImage->GetBuffer(), pBuffer->GetDataPointer(), szBufferSize);
								pPhotonfocus3D->m_pImage->UpdateProfile();
							}
							break;
						default:
							bMemoryCopy = false;
							break;
						}

						if(bMemoryCopy)
						{
							CRavidImageView* pCurView = pPhotonfocus3D->GetImageView();

							if(pCurView)
								pCurView->Invalidate();

							CEventHandlerManager::BroadcastOnAcquisition(pPhotonfocus3D->m_pImage, pPhotonfocus3D->GetObjectID(), pPhotonfocus3D->GetImageView(), pPhotonfocus3D);
						}
					}
					while(false);

					if(pNewBufferMan)
					{
						delete[] pNewBufferMan;
						pNewBufferMan = nullptr;
					}

					if(pResult)
					{
						delete[] pResult;
						pResult = nullptr;
					}

					pImage = nullptr;
				}

				pPhotonfocus3D->m_pPipeline->ReleaseBuffer(pBuffer);
			}

			if(pPhotonfocus3D->m_llGrabCount)
				--pPhotonfocus3D->m_llGrabCount;

			if(pPhotonfocus3D->m_llGrabCount <= 0)
			{
				PvGenParameterArray* pDeviceParams = pPhotonfocus3D->m_pCamera->GetGenParameters();

				if(pDeviceParams)
				{
					pDeviceParams->ExecuteCommand("AcquisitionStop");
					pDeviceParams->SetIntegerValue("TLParamsLocked", 0);
					if(pPhotonfocus3D->m_pPipeline->IsStarted())
						pPhotonfocus3D->m_pPipeline->Stop();
				}

				pPhotonfocus3D->m_bIsLive = false;
				pPhotonfocus3D->m_bIsGrabAvailable = true;
			}
		} 
		while(pPhotonfocus3D->m_llGrabCount);		
	} 
	while(false);

	return 0;
}

#endif