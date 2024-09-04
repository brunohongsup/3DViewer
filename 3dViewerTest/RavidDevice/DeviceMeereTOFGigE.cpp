#include "stdafx.h"
#include "DeviceMeereTOFGigE.h"

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

#include "../RavidFramework/MessageBase.h"
#include "../RavidFramework/RavidTreeView.h"
#include "../RavidFramework/RavidImageView.h"
#include "../RavidFramework/RavidImageViewLayer.h"


#include "../Libraries/Includes/MeereTOF/MTF_API.h"

// mtf_api_d.dll
// mtf_api[x64]_d.dll
#ifndef _WIN64
#ifdef DEBUG
#pragma comment(lib, COMMONLIB_PREFIX "MeereTOF/debug/MTF_API_d.lib")
#else// DEBUG
#pragma comment(lib, COMMONLIB_PREFIX "MeereTOF/release/MTF_API.lib")
#endif
#else
#ifdef DEBUG
#pragma comment(lib, COMMONLIB_PREFIX "MeereTOF/debug/MTF_API[x64]_d.lib")
#else// DEBUG
#pragma comment(lib, COMMONLIB_PREFIX "MeereTOF/release/MTF_API[x64].lib")
#endif
#endif

#define TOTAL 2

using namespace Ravid;
using namespace Ravid::Device;
using namespace Ravid::Database;
using namespace Ravid::Framework;
using namespace Ravid::Algorithms;
using namespace Ravid::Miscellaneous;

IMPLEMENT_DYNAMIC(CDeviceMeereTOFGigE, CDeviceCamera)

BEGIN_MESSAGE_MAP(CDeviceMeereTOFGigE, CDeviceCamera)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static LPCTSTR g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_Count] =
{
	_T("DeviceID"),
	_T("Draw Point"),
	_T("Init Type"),
	_T("Integration Time"),
	_T("Offset"),
	_T("Depth Threshold"),
	_T("Scatter Threshold"),
	_T("Min Depth"),
	_T("Max Depth"),
	_T("Flip Horizontal"),
	_T("Flip Vertical"),
	_T("MultiSync Mode"),
	_T("FPS Delay"),
	_T("Remove Edge"),
	_T("Undistortion"),
	_T("Orthogonal"),
	_T("Lens Intrinsic Param"),
	_T("Focal Length X"),
	_T("Focal Length Y"),
	_T("Principal Point X"),
	_T("Principal Point Y"),
	_T("Lens Distortion Param"),
	_T("Distortion K1"),
	_T("Distortion K2"),
	_T("Distortion K3"),
	_T("Distortion P1"),
	_T("Distortion P2"),
	_T("Skew"),
	_T("RGB LensIntrinsic Param"),
	_T("RGB Focal Length X"),
	_T("RGB Focal Length Y"),
	_T("RGB Principal Point X"),
	_T("RGB Principal Point Y"),
	_T("RGB LensDistortion Param"),
	_T("RGB Distortion K1"),
	_T("RGB Distortion K2"),
	_T("RGB Distortion K3"),
	_T("RGB Distortion P1"),
	_T("RGB Distortion P2"),
	_T("RGB Skew"),
	_T("Extrinsic Param"),
	_T("Extrinsic R11"),
	_T("Extrinsic R12"),
	_T("Extrinsic R13"),
	_T("Extrinsic R21"),
	_T("Extrinsic R22"),
	_T("Extrinsic R23"),
	_T("Extrinsic R31"),
	_T("Extrinsic R32"),
	_T("Extrinsic R33"),
	_T("Extrinsic Tx"),
	_T("Extrinsic Ty"),
	_T("Extrinsic Tz"),
};

static LPCTSTR g_lpszMeereTOFGigEInitType[EDeviceMeereTOFInitType_Count] =
{
	_T("OnlyParam"),
	_T("NotParam"),
};

static LPCTSTR g_lpszMeereTOFGigEUndistortion[EDeviceMeereTOFUndistortion_Count] =
{
	_T("Off"),
	_T("On & Interpol. & Edge Remove On"),
	_T("On & Interpol. On"),
	_T("On & Interpol. Off"),
};

static LPCTSTR g_lpszMeereTOFGigESwitch[EDeviceMeereTOFSwitch_Count] =
{
	_T("Off"),
	_T("On"),
};

CDeviceMeereTOFGigE::CDeviceMeereTOFGigE()
{
	m_nGrabCount = 1;

	m_nCanvasX = 320;
	m_nCanvasY = 240;
}

CDeviceMeereTOFGigE::~CDeviceMeereTOFGigE()
{
	Terminate();
}

ptrdiff_t CDeviceMeereTOFGigE::OnMessage(Ravid::Framework::CMessageBase * pMessage)
{
	__super::OnMessage(pMessage);

	do
	{
		if(!pMessage)
			break;

		if(pMessage->GetMessage() != EMessage_ImageViewMouseUp)
			break;

		if(!IsWindow(GetSafeHwnd()))
			break;
			   
		if(!IsInitialized())
			break;

		CRavidImageView* pCurView = GetImageView();

		if(pCurView->GetObjectID() != pMessage->GetParams())
			break;

		if(pCurView)
		{
			m_DrawPoint = pCurView->GetLastUpImagePoint();
			
			if(IsLive() || !IsGrabAvailable())
				break;

			DrawPixelInfo(9);
		}
	}
	while(false);

	return 0;
}

EDeviceInitializeResult CDeviceMeereTOFGigE::Initialize()
{
	EDeviceInitializeResult eReturn = EDeviceInitializeResult_UnknownError;

	CString strMessage, strStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("MeereTOF"));
		
		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);

		SetStatus(strStatus);

		return EDeviceInitializeResult_NotFoundApiError;
	}

	do 
	{
		MTF_API::mtfDeviceInfo	m_pDevInfo[MAX_DEVICE];	//device info

		int nDeviceCount = 0;

		if(MTF_API::mtfGetDeviceList(m_pDevInfo, &nDeviceCount) != MTF_API::ERROR_NO)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Device List"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		if(!nDeviceCount)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtofindthedevice));
			eReturn = EDeviceInitializeResult_NotFoundDeviceInfo;
			break;
		}

		CString strID = GetDeviceID();

		for(int i = 0; i < nDeviceCount; ++i)
		{
			CString strSerial;
			strSerial.Format(_T("%s"), CString(m_pDevInfo[i].szSerialNum));

			if(strSerial.CompareNoCase(strID))
				continue;

			m_pDeviceInfo = new MTF_API::mtfDeviceInfo(m_pDevInfo[i]);
			m_pFrameInfo = new MTF_API::mtfFrameInfo;

			m_hDevice = m_pDevInfo[i].mtfHnd;
			break;
		}

		if(m_hDevice == -1)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_CouldntfindthesameSN));
			eReturn = EDeviceInitializeResult_NotFoundSerialNumber;
			break;
		}

		if(MTF_API::mtfDeviceOpen(m_hDevice) != MTF_API::ERROR_NO)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntopenthedevice));
			eReturn = EDeviceInitializeResult_NotOpenedDeviceError;
			break;
		}

		EDeviceMeereTOFInitType eInittype = EDeviceMeereTOFInitType_OnlyParam;

		if(GetInitType(&eInittype))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Itit Type"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(GetDrawPoint(&m_bDrawPoint))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Draw point"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		switch(eInittype)
		{
// 		case EDeviceMeereTOFInitType_OnlyParam:
// 			{
// 				int nGetIntegrationTime = 0;
// 				if(GetIntegrationTime(&nGetIntegrationTime))
// 				{
// 					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("IntegrationTime"));
// 					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
// 					break;
// 				}
// 
// 				int nGetOffset = 0;
// 				if(GetOffset(&nGetOffset))
// 				{
// 					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Offset"));
// 					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
// 					break;
// 				}
// 				MTF_API::mtfSetOffset(m_hDevice, nGetOffset);
// 
// 				int nGetDepthThreshold = 0;
// 				if(GetDepthThreshold(&nGetDepthThreshold))
// 				{
// 					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Depth Threshold"));
// 					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
// 					break;
// 				}
// 				MTF_API::mtfSetCheckThreshold(m_hDevice, nGetDepthThreshold);
// 
// 				int nGetScatterThreshold = 0;
// 				if(GetScatterThreshold(&nGetScatterThreshold))
// 				{
// 					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Scatter Threshold"));
// 					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
// 					break;
// 				}
// 				MTF_API::mtfSetScatterThreshold(m_hDevice, nGetScatterThreshold);
// 
// 				int nMinDepth = 0, nMaxDepth = 0;
// 				if(GetMinDepth(&nMinDepth))
// 				{
// 					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Min Depth"));
// 					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
// 					break;
// 				}
// 				if(GetMaxDepth(&nMaxDepth))
// 				{
// 					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Max Depth"));
// 					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
// 					break;
// 				}
// 				MTF_API::mtfSetDepthRange(m_hDevice, nMinDepth, nMaxDepth);
// 
// 				bool bGetFlipHorizontal = false;
// 				if(GetFlipHorizontal(&bGetFlipHorizontal))
// 				{
// 					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Flip Horizontal"));
// 					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
// 					break;
// 				}
// 				MTF_API::mtfSetFlipHorizontal(m_hDevice, bGetFlipHorizontal);
// 
// 				bool bGetFlipVertical = false;
// 				if(GetFlipVertical(&bGetFlipVertical))
// 				{
// 					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Flip Vertical"));
// 					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
// 					break;
// 				}
// 				MTF_API::mtfSetFlipVertical(m_hDevice, bGetFlipVertical);
// 				
// 				bool bGetMultiSyncMode = false;
// 				if(GetMultiSyncMode(&bGetMultiSyncMode))
// 				{
// 					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Multi SyncMode"));
// 					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
// 					break;
// 				}
// 				MTF_API::mtfSetMultiSyncMode(m_hDevice, bGetMultiSyncMode);
// 
// 				int nFPSDelay = 0;
// 				if(GetFPSDelay(&nFPSDelay))
// 				{
// 					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("FPSDelay"));
// 					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
// 					break;
// 				}
// 				MTF_API::mtfSetFPSDelay(m_hDevice, nFPSDelay);
// 
// 				bool bGetRemoveEdge = false;
// 				if(GetRemoveEdge(&bGetRemoveEdge))
// 				{
// 					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Remove Edge"));
// 					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
// 					break;
// 				}
// 				MTF_API::mtfSetRemoveEdge(m_hDevice, bGetRemoveEdge);
// 
// 				EDeviceMeereTOFUndistortion eGetUndistortion = EDeviceMeereTOFUndistortion_Off;
// 				if(GetUndistortion(&eGetUndistortion))
// 				{
// 					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Undistortion"));
// 					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
// 					break;
// 				}
// 				MTF_API::mtfSetUndistortion(m_hDevice, (int)eGetUndistortion);
// 
// 				bool bGetOrthogonal = false;
// 				if(GetOrthogonal(&bGetOrthogonal))
// 				{
// 					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Orthogonal"));
// 					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
// 					break;
// 				}
// 				MTF_API::mtfSetOrthogonal(m_hDevice, bGetOrthogonal);
// 
// 				MTF_API::mtfIntrinsicParam stIntrinsicParam;
// 				MTF_API::mtfDistortionParam stDistortionParam;
// 
// 				if(GetFocalLengthX(&stIntrinsicParam.fFx))
// 				{
// 					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Focal Length X"));
// 					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
// 					break;
// 				}
// 				if(GetFocalLengthY(&stIntrinsicParam.fFy))
// 				{
// 					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Focal Length Y"));
// 					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
// 					break;
// 				}
// 				if(GetPrincipalPointX(&stIntrinsicParam.fCx))
// 				{
// 					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Principal Point X"));
// 					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
// 					break;
// 				}
// 				if(GetPrincipalPointY(&stIntrinsicParam.fCy))
// 				{
// 					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Principal Point Y"));
// 					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
// 					break;
// 				}
// 
// 
// 				if(GetDistortionK1(&stDistortionParam.fK1))
// 				{
// 					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Distortion K1"));
// 					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
// 					break;
// 				}
// 				if(GetDistortionK2(&stDistortionParam.fK2))
// 				{
// 					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Distortion K2"));
// 					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
// 					break;
// 				}
// 				if(GetDistortionK3(&stDistortionParam.fK3))
// 				{
// 					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Distortion K3"));
// 					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
// 					break;
// 				}
// 				if(GetDistortionP1(&stDistortionParam.fP1))
// 				{
// 					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Distortion P1"));
// 					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
// 					break;
// 				}
// 				if(GetDistortionP2(&stDistortionParam.fP2))
// 				{
// 					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Distortion P2"));
// 					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
// 					break;
// 				}
// 				if(GetSkew(&stDistortionParam.fSkew))
// 				{
// 					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Distortion Skew"));
// 					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
// 					break;
// 				}
// 				MTF_API::mtfSetLensParameter(m_hDevice, stIntrinsicParam, stDistortionParam);
// 
// 				MTF_API::mtfIntrinsicParam stRGBIntrinsicParam;
// 				MTF_API::mtfDistortionParam stRGBDistortionParam;
// 
// 				if(GetRGBFocalLengthX(&stRGBIntrinsicParam.fFx))
// 				{
// 					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("RGB Focal Length X"));
// 					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
// 					break;
// 				}
// 				if(GetRGBFocalLengthY(&stRGBIntrinsicParam.fFy))
// 				{
// 					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("RGB Focal Length Y"));
// 					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
// 					break;
// 				}
// 				if(GetRGBPrincipalPointX(&stRGBIntrinsicParam.fCx))
// 				{
// 					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("RGB Principal Point X"));
// 					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
// 					break;
// 				}
// 				if(GetRGBPrincipalPointY(&stRGBIntrinsicParam.fCy))
// 				{
// 					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("RGB Principal Point Y"));
// 					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
// 					break;
// 				}
// 
// 
// 				if(GetRGBDistortionK1(&stRGBDistortionParam.fK1))
// 				{
// 					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("RGB Distortion K1"));
// 					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
// 					break;
// 				}
// 				if(GetRGBDistortionK2(&stRGBDistortionParam.fK2))
// 				{
// 					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("RGB Distortion K2"));
// 					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
// 					break;
// 				}
// 				if(GetRGBDistortionK3(&stRGBDistortionParam.fK3))
// 				{
// 					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("RGB Distortion K3"));
// 					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
// 					break;
// 				}
// 				if(GetRGBDistortionP1(&stRGBDistortionParam.fP1))
// 				{
// 					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("RGB Distortion P1"));
// 					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
// 					break;
// 				}
// 				if(GetRGBDistortionP2(&stRGBDistortionParam.fP2))
// 				{
// 					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("RGB Distortion P2"));
// 					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
// 					break;
// 				}
// 				if(GetRGBSkew(&stRGBDistortionParam.fSkew))
// 				{
// 					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("RGB Distortion Skew"));
// 					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
// 					break;
// 				}
// 				MTF_API::mtfSetRGBLensParameter(m_hDevice, stRGBIntrinsicParam, stRGBDistortionParam);
// 
// 				MTF_API::mtfExtrinsicParam stExtrinsicParam;
// 
// 				if(GetExtrinsicR11(&stExtrinsicParam.fR11))
// 				{
// 					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Extrinsic R11"));
// 					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
// 					break;
// 				}
// 				if(GetExtrinsicR12(&stExtrinsicParam.fR12))
// 				{
// 					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Extrinsic R12"));
// 					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
// 					break;
// 				}
// 				if(GetExtrinsicR13(&stExtrinsicParam.fR13))
// 				{
// 					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Extrinsic R13"));
// 					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
// 					break;
// 				}
// 				if(GetExtrinsicR21(&stExtrinsicParam.fR21))
// 				{
// 					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Extrinsic R21"));
// 					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
// 					break;
// 				}
// 				if(GetExtrinsicR22(&stExtrinsicParam.fR22))
// 				{
// 					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Extrinsic R22"));
// 					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
// 					break;
// 				}
// 				if(GetExtrinsicR23(&stExtrinsicParam.fR23))
// 				{
// 					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Extrinsic R23"));
// 					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
// 					break;
// 				}
// 				if(GetExtrinsicR31(&stExtrinsicParam.fR31))
// 				{
// 					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Extrinsic R31"));
// 					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
// 					break;
// 				}
// 				if(GetExtrinsicR32(&stExtrinsicParam.fR32))
// 				{
// 					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Extrinsic R32"));
// 					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
// 					break;
// 				}
// 				if(GetExtrinsicR33(&stExtrinsicParam.fR33))
// 				{
// 					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Extrinsic R33"));
// 					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
// 					break;
// 				}
// 				if(GetExtrinsicTx(&stExtrinsicParam.fTx))
// 				{
// 					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Extrinsic Tx"));
// 					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
// 					break;
// 				}
// 				if(GetExtrinsicTy(&stExtrinsicParam.fTy))
// 				{
// 					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Extrinsic Ty"));
// 					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
// 					break;
// 				}
// 				if(GetExtrinsicTz(&stExtrinsicParam.fTz))
// 				{
// 					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Extrinsic Tz"));
// 					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
// 					break;
// 				}
// 				MTF_API::mtfSetExtrinsicParameter(m_hDevice, stExtrinsicParam);
// 			}
// 			break;
		case EDeviceMeereTOFInitType_NotParam:
			{
				int nmtfGetIntegrationTime = MTF_API::mtfGetIntegrationTime(m_hDevice);
				{
					CString strSave;
					strSave.Format(_T("%d"), nmtfGetIntegrationTime);

					SetParamValue(EDeviceParameterMeereTOFGigE_IntegrationTime, strSave);
					SaveSettings(EDeviceParameterMeereTOFGigE_IntegrationTime);
				}
				
				int nmtfGetOffset = MTF_API::mtfGetOffset(m_hDevice);
				{
					CString strSave;
					strSave.Format(_T("%d"), nmtfGetOffset);

					SetParamValue(EDeviceParameterMeereTOFGigE_Offset, strSave);
					SaveSettings(EDeviceParameterMeereTOFGigE_Offset);
				}

				unsigned char nmtfGetCheckThreshold = MTF_API::mtfGetCheckThreshold(m_hDevice);
				{
					CString strSave;
					strSave.Format(_T("%d"), nmtfGetCheckThreshold);

					SetParamValue(EDeviceParameterMeereTOFGigE_DepthThreshold, strSave);
					SaveSettings(EDeviceParameterMeereTOFGigE_DepthThreshold);
				}

				unsigned char nmtfGetScatterThreshold = MTF_API::mtfGetScatterThreshold(m_hDevice);
				{
					CString strSave;
					strSave.Format(_T("%d"), nmtfGetScatterThreshold);

					SetParamValue(EDeviceParameterMeereTOFGigE_ScatterThreshold, strSave);
					SaveSettings(EDeviceParameterMeereTOFGigE_ScatterThreshold);
				}

				int nMinDepth = 0, nMaxDepth = 0;
				if(MTF_API::mtfGetDepthRange(m_hDevice, &nMinDepth, &nMaxDepth) == MTF_API::ERROR_NO)
				{
					{
						CString strSave;
						strSave.Format(_T("%d"), nMinDepth);

						SetParamValue(EDeviceParameterMeereTOFGigE_MinDepth, strSave);
						SaveSettings(EDeviceParameterMeereTOFGigE_MinDepth);
					}
					{
						CString strSave;
						strSave.Format(_T("%d"), nMaxDepth);

						SetParamValue(EDeviceParameterMeereTOFGigE_MaxDepth, strSave);
						SaveSettings(EDeviceParameterMeereTOFGigE_MaxDepth);
					}
				}

				int nmtfGetFlipHorizontal = MTF_API::mtfGetFlipHorizontal(m_hDevice);
				{
					CString strSave;
					strSave.Format(_T("%d"), nmtfGetFlipHorizontal);

					SetParamValue(EDeviceParameterMeereTOFGigE_FlipHorizontal, strSave);
					SaveSettings(EDeviceParameterMeereTOFGigE_FlipHorizontal);
				}

				int nmtfGetFlipVertical = MTF_API::mtfGetFlipVertical(m_hDevice);
				{
					CString strSave;
					strSave.Format(_T("%d"), nmtfGetFlipVertical);

					SetParamValue(EDeviceParameterMeereTOFGigE_FlipVertical, strSave);
					SaveSettings(EDeviceParameterMeereTOFGigE_FlipVertical);
				}
				
				int nmtfGetMultiSyncMode = MTF_API::mtfGetMultiSyncMode(m_hDevice);
				{
					CString strSave;
					strSave.Format(_T("%d"), nmtfGetMultiSyncMode);

					SetParamValue(EDeviceParameterMeereTOFGigE_MultiSyncMode, strSave);
					SaveSettings(EDeviceParameterMeereTOFGigE_MultiSyncMode);
				}

				int nmtfGetFPSDelay = MTF_API::mtfGetFPSDelay(m_hDevice);
				{
					CString strSave;
					strSave.Format(_T("%d"), nmtfGetFPSDelay);

					SetParamValue(EDeviceParameterMeereTOFGigE_FPSDelay, strSave);
					SaveSettings(EDeviceParameterMeereTOFGigE_FPSDelay);
				}

				int nmtfGetRemoveEdge = MTF_API::mtfGetRemoveEdge(m_hDevice);
				{
					CString strSave;
					strSave.Format(_T("%d"), nmtfGetRemoveEdge);

					SetParamValue(EDeviceParameterMeereTOFGigE_RemoveEdge, strSave);
					SaveSettings(EDeviceParameterMeereTOFGigE_RemoveEdge);
				}

				int nmtfGetUndistortion = MTF_API::mtfGetUndistortion(m_hDevice);
				{
					CString strSave;
					strSave.Format(_T("%d"), nmtfGetUndistortion);

					SetParamValue(EDeviceParameterMeereTOFGigE_Undistortion, strSave);
					SaveSettings(EDeviceParameterMeereTOFGigE_Undistortion);
				}

				int nmtfGetOrthogonal = MTF_API::mtfGetOrthogonal(m_hDevice);
				{
					CString strSave;
					strSave.Format(_T("%d"), nmtfGetOrthogonal);

					SetParamValue(EDeviceParameterMeereTOFGigE_Orthogonal, strSave);
					SaveSettings(EDeviceParameterMeereTOFGigE_Orthogonal);
				}

				MTF_API::mtfIntrinsicParam stIntrinsicParam;
				MTF_API::mtfDistortionParam stDistortionParam;
				if(MTF_API::mtfGetLensParameter(m_hDevice, &stIntrinsicParam, &stDistortionParam) == MTF_API::ERROR_NO)
				{
					{
						CString strSave;
						strSave.Format(_T("%f"), stIntrinsicParam.fFx);

						SetParamValue(EDeviceParameterMeereTOFGigE_FocalLengthX, strSave);
						SaveSettings(EDeviceParameterMeereTOFGigE_FocalLengthX);
					}
					{
						CString strSave;
						strSave.Format(_T("%f"), stIntrinsicParam.fFy);

						SetParamValue(EDeviceParameterMeereTOFGigE_FocalLengthY, strSave);
						SaveSettings(EDeviceParameterMeereTOFGigE_FocalLengthY);
					}
					{
						CString strSave;
						strSave.Format(_T("%f"), stIntrinsicParam.fCx);

						SetParamValue(EDeviceParameterMeereTOFGigE_PrincipalPointX, strSave);
						SaveSettings(EDeviceParameterMeereTOFGigE_PrincipalPointX);
					}
					{
						CString strSave;
						strSave.Format(_T("%f"), stIntrinsicParam.fCy);

						SetParamValue(EDeviceParameterMeereTOFGigE_PrincipalPointY, strSave);
						SaveSettings(EDeviceParameterMeereTOFGigE_PrincipalPointY);
					}

					{
						CString strSave;
						strSave.Format(_T("%f"), stDistortionParam.fK1);

						SetParamValue(EDeviceParameterMeereTOFGigE_DistortionK1, strSave);
						SaveSettings(EDeviceParameterMeereTOFGigE_DistortionK1);
					}
					{
						CString strSave;
						strSave.Format(_T("%f"), stDistortionParam.fK2);

						SetParamValue(EDeviceParameterMeereTOFGigE_DistortionK2, strSave);
						SaveSettings(EDeviceParameterMeereTOFGigE_DistortionK2);
					}
					{
						CString strSave;
						strSave.Format(_T("%f"), stDistortionParam.fK3);

						SetParamValue(EDeviceParameterMeereTOFGigE_DistortionK3, strSave);
						SaveSettings(EDeviceParameterMeereTOFGigE_DistortionK3);
					}
					{
						CString strSave;
						strSave.Format(_T("%f"), stDistortionParam.fP1);

						SetParamValue(EDeviceParameterMeereTOFGigE_DistortionP1, strSave);
						SaveSettings(EDeviceParameterMeereTOFGigE_DistortionP1);
					}
					{
						CString strSave;
						strSave.Format(_T("%f"), stDistortionParam.fP2);

						SetParamValue(EDeviceParameterMeereTOFGigE_DistortionP2, strSave);
						SaveSettings(EDeviceParameterMeereTOFGigE_DistortionP2);
					}
					{
						CString strSave;
						strSave.Format(_T("%f"), stDistortionParam.fSkew);

						SetParamValue(EDeviceParameterMeereTOFGigE_Skew, strSave);
						SaveSettings(EDeviceParameterMeereTOFGigE_Skew);
					}
				}

				MTF_API::mtfIntrinsicParam stRGBIntrinsicParam;
				MTF_API::mtfDistortionParam stRGBDistortionParam;
				if(MTF_API::mtfGetRGBLensParameter(m_hDevice, &stRGBIntrinsicParam, &stRGBDistortionParam) == MTF_API::ERROR_NO)
				{
					{
						CString strSave;
						strSave.Format(_T("%f"), stRGBIntrinsicParam.fFx);

						SetParamValue(EDeviceParameterMeereTOFGigE_RGBFocalLengthX, strSave);
						SaveSettings(EDeviceParameterMeereTOFGigE_RGBFocalLengthX);
					}
					{
						CString strSave;
						strSave.Format(_T("%f"), stRGBIntrinsicParam.fFy);

						SetParamValue(EDeviceParameterMeereTOFGigE_RGBFocalLengthY, strSave);
						SaveSettings(EDeviceParameterMeereTOFGigE_RGBFocalLengthY);
					}
					{
						CString strSave;
						strSave.Format(_T("%f"), stRGBIntrinsicParam.fCx);

						SetParamValue(EDeviceParameterMeereTOFGigE_RGBPrincipalPointX, strSave);
						SaveSettings(EDeviceParameterMeereTOFGigE_RGBPrincipalPointX);
					}
					{
						CString strSave;
						strSave.Format(_T("%f"), stRGBIntrinsicParam.fCy);

						SetParamValue(EDeviceParameterMeereTOFGigE_RGBPrincipalPointY, strSave);
						SaveSettings(EDeviceParameterMeereTOFGigE_RGBPrincipalPointY);
					}

					{
						CString strSave;
						strSave.Format(_T("%f"), stRGBDistortionParam.fK1);

						SetParamValue(EDeviceParameterMeereTOFGigE_RGBDistortionK1, strSave);
						SaveSettings(EDeviceParameterMeereTOFGigE_RGBDistortionK1);
					}
					{
						CString strSave;
						strSave.Format(_T("%f"), stRGBDistortionParam.fK2);

						SetParamValue(EDeviceParameterMeereTOFGigE_RGBDistortionK2, strSave);
						SaveSettings(EDeviceParameterMeereTOFGigE_RGBDistortionK2);
					}
					{
						CString strSave;
						strSave.Format(_T("%f"), stRGBDistortionParam.fK3);

						SetParamValue(EDeviceParameterMeereTOFGigE_RGBDistortionK3, strSave);
						SaveSettings(EDeviceParameterMeereTOFGigE_RGBDistortionK3);
					}
					{
						CString strSave;
						strSave.Format(_T("%f"), stRGBDistortionParam.fP1);

						SetParamValue(EDeviceParameterMeereTOFGigE_RGBDistortionP1, strSave);
						SaveSettings(EDeviceParameterMeereTOFGigE_RGBDistortionP1);
					}
					{
						CString strSave;
						strSave.Format(_T("%f"), stRGBDistortionParam.fP2);

						SetParamValue(EDeviceParameterMeereTOFGigE_RGBDistortionP2, strSave);
						SaveSettings(EDeviceParameterMeereTOFGigE_RGBDistortionP2);
					}
					{
						CString strSave;
						strSave.Format(_T("%f"), stRGBDistortionParam.fSkew);

						SetParamValue(EDeviceParameterMeereTOFGigE_RGBSkew, strSave);
						SaveSettings(EDeviceParameterMeereTOFGigE_RGBSkew);
					}
				}

				MTF_API::mtfExtrinsicParam stExtrinsicParam;
				if(MTF_API::mtfGetExtrinsicParameter(m_hDevice, &stExtrinsicParam) == MTF_API::ERROR_NO)
				{
					{
						CString strSave;
						strSave.Format(_T("%f"), stExtrinsicParam.fR11);

						SetParamValue(EDeviceParameterMeereTOFGigE_ExtrinsicR11, strSave);
						SaveSettings(EDeviceParameterMeereTOFGigE_ExtrinsicR11);
					}
					{
						CString strSave;
						strSave.Format(_T("%f"), stExtrinsicParam.fR12);

						SetParamValue(EDeviceParameterMeereTOFGigE_ExtrinsicR12, strSave);
						SaveSettings(EDeviceParameterMeereTOFGigE_ExtrinsicR12);
					}
					{
						CString strSave;
						strSave.Format(_T("%f"), stExtrinsicParam.fR13);

						SetParamValue(EDeviceParameterMeereTOFGigE_ExtrinsicR13, strSave);
						SaveSettings(EDeviceParameterMeereTOFGigE_ExtrinsicR13);
					}
					{
						CString strSave;
						strSave.Format(_T("%f"), stExtrinsicParam.fR21);

						SetParamValue(EDeviceParameterMeereTOFGigE_ExtrinsicR21, strSave);
						SaveSettings(EDeviceParameterMeereTOFGigE_ExtrinsicR21);
					}
					{
						CString strSave;
						strSave.Format(_T("%f"), stExtrinsicParam.fR22);

						SetParamValue(EDeviceParameterMeereTOFGigE_ExtrinsicR22, strSave);
						SaveSettings(EDeviceParameterMeereTOFGigE_ExtrinsicR22);
					}
					{
						CString strSave;
						strSave.Format(_T("%f"), stExtrinsicParam.fR23);

						SetParamValue(EDeviceParameterMeereTOFGigE_ExtrinsicR23, strSave);
						SaveSettings(EDeviceParameterMeereTOFGigE_ExtrinsicR23);
					}
					{
						CString strSave;
						strSave.Format(_T("%f"), stExtrinsicParam.fR31);

						SetParamValue(EDeviceParameterMeereTOFGigE_ExtrinsicR31, strSave);
						SaveSettings(EDeviceParameterMeereTOFGigE_ExtrinsicR31);
					}
					{
						CString strSave;
						strSave.Format(_T("%f"), stExtrinsicParam.fR32);

						SetParamValue(EDeviceParameterMeereTOFGigE_ExtrinsicR32, strSave);
						SaveSettings(EDeviceParameterMeereTOFGigE_ExtrinsicR32);
					}
					{
						CString strSave;
						strSave.Format(_T("%f"), stExtrinsicParam.fR33);

						SetParamValue(EDeviceParameterMeereTOFGigE_ExtrinsicR33, strSave);
						SaveSettings(EDeviceParameterMeereTOFGigE_ExtrinsicR33);
					}
					{
						CString strSave;
						strSave.Format(_T("%f"), stExtrinsicParam.fTx);

						SetParamValue(EDeviceParameterMeereTOFGigE_ExtrinsicTx, strSave);
						SaveSettings(EDeviceParameterMeereTOFGigE_ExtrinsicTx);
					}
					{
						CString strSave;
						strSave.Format(_T("%f"), stExtrinsicParam.fTy);

						SetParamValue(EDeviceParameterMeereTOFGigE_ExtrinsicTy, strSave);
						SaveSettings(EDeviceParameterMeereTOFGigE_ExtrinsicTy);
					}
					{
						CString strSave;
						strSave.Format(_T("%f"), stExtrinsicParam.fTz);

						SetParamValue(EDeviceParameterMeereTOFGigE_ExtrinsicTz, strSave);
						SaveSettings(EDeviceParameterMeereTOFGigE_ExtrinsicTz);
					}
				}
			}
			break;
		}

		int nDepth = 12;

		int nWidth = m_nCanvasX;
		int nHeight = m_nCanvasY;
		int nMaxValue = (1 << nDepth) - 1;

		this->InitBuffer(nWidth, nHeight, nMaxValue, CRavidImage::MakeValueFormat(1, nDepth));
		this->ConnectImage();

		CRavidImageView* pCurView = GetImageView();
		if(pCurView)
			DrawPixelInfo(9);

		CMultipleVariable mv;

		for(int i = 0; i < 3; ++i)
			mv.AddValue(0.0);

		if(m_p3DXYZCoordinates)
		{
			m_p3DXYZCoordinates->Clear();

			delete m_p3DXYZCoordinates;
			m_p3DXYZCoordinates = nullptr;
		}

		m_p3DXYZCoordinates = new CRavidImage(nWidth, nHeight, mv, CRavidImage::MakeValueFormat(3, 32, Ravid::Algorithms::CRavidImage::EValueType_FloatingPoint));

		if(!m_p3DXYZCoordinates)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtocreateimagebuffer);
			eReturn = EDeviceInitializeResult_NotCreateImagebuffer;
			break;
		}

		int nBufInit = MTF_API::mtfReadBufInit(m_hDevice);

		m_bIsInitialized = true;

		m_pLiveThread = AfxBeginThread(CDeviceMeereTOFGigE::CallbackFunction, this, 0, 0U, 0);

		if(!m_pLiveThread)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtocreatethread);
			eReturn = EDeviceInitializeResult_CreateThreadError;
			m_bIsInitialized = false;
			break;
		}

		if(MTF_API::mtfGrabStart(m_hDevice) != MTF_API::ERROR_NO)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("GrabStart"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			m_bIsInitialized = false;
			break;
		}

//		m_pLiveThread->ResumeThread();
		//0919200fa00145

		strStatus = CMultiLanguageManager::GetString(ELanguageParameter_Initialized);

		strMessage = CMultiLanguageManager::GetString(ELanguageParameter_TheDevicehasbeensuccessfullyinitialized);

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

EDeviceTerminateResult CDeviceMeereTOFGigE::Terminate()
{
	EDeviceTerminateResult eReturn = EDeviceTerminateResult_UnknownError;

	CString strMessage;

	SetStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("MeereTOF"));

		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);

		return EDeviceTerminateResult_NotFoundApiError;
	}

	do
	{		
		if(!IsGrabAvailable() || IsLive())
			Stop();

		MTF_API::mtfGrabStop(m_hDevice);

		m_bIsLive = false;
		m_bIsInitialized = false;
		m_bIsGrabAvailable = true;

		if(m_pLiveThread)
		{
			WaitForSingleObject(m_pLiveThread->m_hThread, 500);
			m_pLiveThread = nullptr;
		}

		if(MTF_API::mtfDeviceIsOpen(m_hDevice))
			MTF_API::mtfDeviceClose(m_hDevice);

		m_hDevice = -1;

		strMessage = CMultiLanguageManager::GetString(ELanguageParameter_TheDevicehasbeensuccessfullyterminated);

		eReturn = EDeviceTerminateResult_OK;
				
		CEventHandlerManager::BroadcastOnDeviceTerminated(this);
	}
	while(false);
	
	if(m_pFrameInfo)
		delete m_pFrameInfo;
	m_pFrameInfo = nullptr;

	if(m_pDeviceInfo)
		delete m_pDeviceInfo;
	m_pDeviceInfo = nullptr;

	this->ClearBuffer();

	if(m_p3DXYZCoordinates)
	{
		m_p3DXYZCoordinates->Clear();

		delete m_p3DXYZCoordinates;
		m_p3DXYZCoordinates = nullptr;
	}

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceMeereTOFGigE::LoadSettings()
{
	bool bReturn = false;

	m_vctParameterFieldConfigurations.clear();

	do
	{
		CString strTime;
		strTime.Format(_T("%d"), INT_MAX);

		AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_DeviceID, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_DeviceID], _T("0"), EParameterFieldType_Edit);

		AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_DrawPoint, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_DrawPoint], _T("0"), EParameterFieldType_Check);

		AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_InitType, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_InitType], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMeereTOFGigEInitType, EDeviceMeereTOFInitType_Count), _T("Only param : param setting / Not param : Not used param"));


		AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_IntegrationTime, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_IntegrationTime], _T("2100"), EParameterFieldType_Edit, nullptr, _T("100 ~ 3000 us"));

		AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_Offset, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_Offset], _T("0"), EParameterFieldType_Edit, nullptr, _T("-255 ~ 255"));

		AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_DepthThreshold, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_DepthThreshold], _T("0"), EParameterFieldType_Edit, nullptr, _T("0 ~ 255"));

		AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_ScatterThreshold, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_ScatterThreshold], _T("0"), EParameterFieldType_Edit, nullptr, _T("0 ~ 255"));

		AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_MinDepth, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_MinDepth], _T("0"), EParameterFieldType_Edit, nullptr, _T("out of depth value is zero"));

		AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_MaxDepth, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_MaxDepth], _T("0"), EParameterFieldType_Edit, nullptr, _T("out of depth value is zero"));

		AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_FlipHorizontal, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_FlipHorizontal], _T("0"), EParameterFieldType_Check);

		AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_FlipVertical, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_FlipVertical], _T("0"), EParameterFieldType_Check);

		AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_MultiSyncMode, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_MultiSyncMode], _T("0"), EParameterFieldType_Check, nullptr, _T("For using multi cameras"));

		AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_FPSDelay, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_FPSDelay], _T("0"), EParameterFieldType_Edit, nullptr, _T("Delay time"));

		AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_RemoveEdge, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_RemoveEdge], _T("0"), EParameterFieldType_Check, nullptr, _T("enable/disable remove edge filter"));

		AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_Undistortion, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_Undistortion], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMeereTOFGigEUndistortion, EDeviceMeereTOFUndistortion_Count), _T("lens undistortion  mode"));

		AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_Orthogonal, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_Orthogonal], _T("0"), EParameterFieldType_Check, nullptr, _T("convert orthogonal z data from radial depth data"));

		AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_LensIntrinsicParam, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_LensIntrinsicParam], _T("0"), EParameterFieldType_None);
		{
			AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_FocalLengthX, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_FocalLengthX], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_FocalLengthY, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_FocalLengthY], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_PrincipalPointX, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_PrincipalPointX], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_PrincipalPointY, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_PrincipalPointY], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		}

		AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_LensDistortionParam, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_LensDistortionParam], _T("0"), EParameterFieldType_None);
		{
			AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_DistortionK1, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_DistortionK1], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_DistortionK2, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_DistortionK2], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_DistortionK3, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_DistortionK3], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_DistortionP1, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_DistortionP1], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_DistortionP2, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_DistortionP2], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_Skew, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_Skew], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		}

		AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_RGBLensIntrinsicParam, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_RGBLensIntrinsicParam], _T("0"), EParameterFieldType_None);
		{
			AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_RGBFocalLengthX, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_RGBFocalLengthX], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_RGBFocalLengthY, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_RGBFocalLengthY], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_RGBPrincipalPointX, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_RGBPrincipalPointX], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_RGBPrincipalPointY, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_RGBPrincipalPointY], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		}

		AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_RGBLensDistortionParam, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_RGBLensDistortionParam], _T("0"), EParameterFieldType_None);
		{
			AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_RGBDistortionK1, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_RGBDistortionK1], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_RGBDistortionK2, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_RGBDistortionK2], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_RGBDistortionK3, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_RGBDistortionK3], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_RGBDistortionP1, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_RGBDistortionP1], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_RGBDistortionP2, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_RGBDistortionP2], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_RGBSkew, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_RGBSkew], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		}

		AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_ExtrinsicParam, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_ExtrinsicParam], _T("0"), EParameterFieldType_None);
		{
			AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_ExtrinsicR11, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_ExtrinsicR11], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_ExtrinsicR12, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_ExtrinsicR12], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_ExtrinsicR13, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_ExtrinsicR13], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_ExtrinsicR21, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_ExtrinsicR21], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_ExtrinsicR22, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_ExtrinsicR22], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_ExtrinsicR23, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_ExtrinsicR23], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_ExtrinsicR31, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_ExtrinsicR31], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_ExtrinsicR32, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_ExtrinsicR32], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_ExtrinsicR33, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_ExtrinsicR33], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_ExtrinsicTx, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_ExtrinsicTx], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_ExtrinsicTy, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_ExtrinsicTy], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterMeereTOFGigE_ExtrinsicTz, g_lpszParamMeereTOFGigE[EDeviceParameterMeereTOFGigE_ExtrinsicTz], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		}

		bReturn = true;
	}
	while(false);

	return bReturn & __super::LoadSettings();
}

EDeviceGrabResult CDeviceMeereTOFGigE::Grab()
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

		m_nGrabCount = 1;

		m_bIsGrabAvailable = false;
		
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("Grab"));
		
		eReturn = EDeviceGrabResult_OK;
	}
	while(false);
	
	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceLiveResult CDeviceMeereTOFGigE::Live()
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

		m_nGrabCount = -1;

		m_bIsLive = true;

		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("Live"));

		eReturn = EDeviceLiveResult_OK;
	}
	while(false);
	
	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceStopResult CDeviceMeereTOFGigE::Stop()
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

		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("Stop"));

		eReturn = EDeviceStopResult_OK;
	} 
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceTriggerResult CDeviceMeereTOFGigE::Trigger()
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
		
		bool bFlag = false;

// 		try
// 		{
// 			GenApi::CCommandPtr ptrTrigger = m_pCamera->GetParameter("TriggerSoftware");
// 			ptrTrigger->Execute();
// 
// 			bFlag = true;
// 		}
// 		catch(...)
// 		{
// 
// 		}
		
		if(!bFlag)
			break;

		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("Trigger"));

		eReturn = EDeviceTriggerResult_OK;
	} 
	while (false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetDrawPoint(bool * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMeereTOFGigE_DrawPoint));

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetDrawPoint(bool bParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_DrawPoint;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		CString strSave;
		strSave.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		m_bDrawPoint = bParam;

		DrawPixelInfo(9);

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], g_lpszMeereTOFGigESwitch[nPreValue], g_lpszMeereTOFGigESwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetInitType(EDeviceMeereTOFInitType * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterMeereTOFGigE_InitType));

		if(nData < 0 || nData >= (int)EDeviceMeereTOFInitType_Count)
		{
			eReturn = EMeereTOFGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceMeereTOFInitType)nData;

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetInitType(EDeviceMeereTOFInitType eParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_InitType;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDeviceMeereTOFInitType)0 || eParam >= EDeviceMeereTOFInitType_Count)
		{
			eReturn = EMeereTOFSetFunction_NotFindCommandError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], g_lpszMeereTOFGigEInitType[nPreValue], g_lpszMeereTOFGigEInitType[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetIntegrationTime(int * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMeereTOFGigE_IntegrationTime));

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetIntegrationTime(int nParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_IntegrationTime;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMeereTOFSetFunction_AlreadyInitializedError;
			break;

// 			if(MTF_API::mtfSetIntegrationTime(m_hDevice, nParam) != MTF_API::ERROR_NO)
// 			{
// 				eReturn = EMeereTOFSetFunction_WriteToDeviceError;
// 				break;
// 			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetOffset(int * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMeereTOFGigE_Offset));

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetOffset(int nParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_Offset;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMeereTOFSetFunction_AlreadyInitializedError;
			break;
// 			if(MTF_API::mtfSetOffset(m_hDevice, nParam) != MTF_API::ERROR_NO)
// 			{
// 				eReturn = EMeereTOFSetFunction_WriteToDeviceError;
// 				break;
// 			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetDepthThreshold(int * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMeereTOFGigE_DepthThreshold));

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetDepthThreshold(int nParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_DepthThreshold;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMeereTOFSetFunction_AlreadyInitializedError;
			break;

// 			if(MTF_API::mtfSetCheckThreshold(m_hDevice, nParam) != MTF_API::ERROR_NO)
// 			{
// 				eReturn = EMeereTOFSetFunction_WriteToDeviceError;
// 				break;
// 			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetScatterThreshold(int * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMeereTOFGigE_ScatterThreshold));

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetScatterThreshold(int nParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_ScatterThreshold;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMeereTOFSetFunction_AlreadyInitializedError;
			break;
// 			if(MTF_API::mtfSetScatterThreshold(m_hDevice, nParam) != MTF_API::ERROR_NO)
// 			{
// 				eReturn = EMeereTOFSetFunction_WriteToDeviceError;
// 				break;
// 			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetMinDepth(int * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMeereTOFGigE_MinDepth));

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetMinDepth(int nParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_MinDepth;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMeereTOFSetFunction_AlreadyInitializedError;
			break;
// 			int nMaxDepth = 0;
// 			if(GetMaxDepth(&nMaxDepth))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			if(MTF_API::mtfSetDepthRange(m_hDevice, nParam, nMaxDepth) != MTF_API::ERROR_NO)
// 			{
// 				eReturn = EMeereTOFSetFunction_WriteToDeviceError;
// 				break;
// 			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetMaxDepth(int * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMeereTOFGigE_MaxDepth));

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetMaxDepth(int nParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_MaxDepth;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMeereTOFSetFunction_AlreadyInitializedError;
			break;
// 			int nMinDepth = 0;
// 			if(GetMinDepth(&nMinDepth))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			if(MTF_API::mtfSetDepthRange(m_hDevice, nMinDepth, nParam) != MTF_API::ERROR_NO)
// 			{
// 				eReturn = EMeereTOFSetFunction_WriteToDeviceError;
// 				break;
// 			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetFlipHorizontal(bool * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMeereTOFGigE_FlipHorizontal));

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetFlipHorizontal(bool bParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_FlipHorizontal;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMeereTOFSetFunction_AlreadyInitializedError;
			break;
// 			if(MTF_API::mtfSetFlipHorizontal(m_hDevice, bParam) != MTF_API::ERROR_NO)
// 			{
// 				eReturn = EMeereTOFSetFunction_WriteToDeviceError;
// 				break;
// 			}
		}

		CString strSave;
		strSave.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], g_lpszMeereTOFGigESwitch[nPreValue], g_lpszMeereTOFGigESwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetFlipVertical(bool * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMeereTOFGigE_FlipVertical));

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetFlipVertical(bool bParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_FlipVertical;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMeereTOFSetFunction_AlreadyInitializedError;
			break;
// 			if(MTF_API::mtfSetFlipVertical(m_hDevice, bParam) != MTF_API::ERROR_NO)
// 			{
// 				eReturn = EMeereTOFSetFunction_WriteToDeviceError;
// 				break;
// 			}
		}

		CString strSave;
		strSave.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], g_lpszMeereTOFGigESwitch[nPreValue], g_lpszMeereTOFGigESwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetMultiSyncMode(bool * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMeereTOFGigE_MultiSyncMode));

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetMultiSyncMode(bool bParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_MultiSyncMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMeereTOFSetFunction_AlreadyInitializedError;
			break;
// 			if(MTF_API::mtfSetMultiSyncMode(m_hDevice, bParam) != MTF_API::ERROR_NO)
// 			{
// 				eReturn = EMeereTOFSetFunction_WriteToDeviceError;
// 				break;
// 			}
		}

		CString strSave;
		strSave.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], g_lpszMeereTOFGigESwitch[nPreValue], g_lpszMeereTOFGigESwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetFPSDelay(int * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMeereTOFGigE_FPSDelay));

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetFPSDelay(int nParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_FPSDelay;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMeereTOFSetFunction_AlreadyInitializedError;
			break;
// 			if(MTF_API::mtfSetFPSDelay(m_hDevice, nParam) != MTF_API::ERROR_NO)
// 			{
// 				eReturn = EMeereTOFSetFunction_WriteToDeviceError;
// 				break;
// 			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetRemoveEdge(bool * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMeereTOFGigE_RemoveEdge));

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetRemoveEdge(bool bParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_RemoveEdge;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMeereTOFSetFunction_AlreadyInitializedError;
			break;
// 			if(MTF_API::mtfSetRemoveEdge(m_hDevice, bParam) != MTF_API::ERROR_NO)
// 			{
// 				eReturn = EMeereTOFSetFunction_WriteToDeviceError;
// 				break;
// 			}
		}

		CString strSave;
		strSave.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], g_lpszMeereTOFGigESwitch[nPreValue], g_lpszMeereTOFGigESwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetUndistortion(EDeviceMeereTOFUndistortion * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterMeereTOFGigE_Undistortion));

		if(nData < 0 || nData >= (int)EDeviceMeereTOFUndistortion_Count)
		{
			eReturn = EMeereTOFGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceMeereTOFUndistortion)nData;

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetUndistortion(EDeviceMeereTOFUndistortion eParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_Undistortion;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDeviceMeereTOFUndistortion)0 || eParam >= EDeviceMeereTOFUndistortion_Count)
		{
			eReturn = EMeereTOFSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMeereTOFSetFunction_AlreadyInitializedError;
			break;
// 			if(MTF_API::mtfSetUndistortion(m_hDevice, (int)eParam) != MTF_API::ERROR_NO)
// 			{
// 				eReturn = EMeereTOFSetFunction_WriteToDeviceError;
// 				break;
// 			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], g_lpszMeereTOFGigEUndistortion[nPreValue], g_lpszMeereTOFGigEUndistortion[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetOrthogonal(bool * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMeereTOFGigE_Orthogonal));

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetOrthogonal(bool bParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_Orthogonal;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMeereTOFSetFunction_AlreadyInitializedError;
			break;
// 			if(MTF_API::mtfSetOrthogonal(m_hDevice, bParam) != MTF_API::ERROR_NO)
// 			{
// 				eReturn = EMeereTOFSetFunction_WriteToDeviceError;
// 				break;
// 			}
		}

		CString strSave;
		strSave.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], g_lpszMeereTOFGigESwitch[nPreValue], g_lpszMeereTOFGigESwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetFocalLengthX(float * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterMeereTOFGigE_FocalLengthX));

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetFocalLengthX(float fParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_FocalLengthX;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMeereTOFSetFunction_AlreadyInitializedError;
			break;
// 			MTF_API::mtfIntrinsicParam stIntrinsicParam;
// 			MTF_API::mtfDistortionParam stDistortionParam;
// 
// 			stIntrinsicParam.fFx = fParam;
// 
// 			if(GetFocalLengthY(&stIntrinsicParam.fFy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetPrincipalPointX(&stIntrinsicParam.fCx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetPrincipalPointY(&stIntrinsicParam.fCy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 
// 			if(GetDistortionK1(&stDistortionParam.fK1))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetDistortionK2(&stDistortionParam.fK2))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetDistortionK3(&stDistortionParam.fK3))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetDistortionP1(&stDistortionParam.fP1))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetDistortionP2(&stDistortionParam.fP2))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetSkew(&stDistortionParam.fSkew))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			if(MTF_API::mtfSetLensParameter(m_hDevice, stIntrinsicParam, stDistortionParam) != MTF_API::ERROR_NO)
// 			{
// 				eReturn = EMeereTOFSetFunction_WriteToDeviceError;
// 				break;
// 			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetFocalLengthY(float * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterMeereTOFGigE_FocalLengthY));

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetFocalLengthY(float fParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_FocalLengthY;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMeereTOFSetFunction_AlreadyInitializedError;
			break;
// 			MTF_API::mtfIntrinsicParam stIntrinsicParam;
// 			MTF_API::mtfDistortionParam stDistortionParam;
// 
// 			stIntrinsicParam.fFy = fParam;
// 			if(GetFocalLengthX(&stIntrinsicParam.fFx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetPrincipalPointX(&stIntrinsicParam.fCx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetPrincipalPointY(&stIntrinsicParam.fCy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 
// 			if(GetDistortionK1(&stDistortionParam.fK1))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetDistortionK2(&stDistortionParam.fK2))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetDistortionK3(&stDistortionParam.fK3))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetDistortionP1(&stDistortionParam.fP1))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetDistortionP2(&stDistortionParam.fP2))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetSkew(&stDistortionParam.fSkew))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			if(MTF_API::mtfSetLensParameter(m_hDevice, stIntrinsicParam, stDistortionParam) != MTF_API::ERROR_NO)
// 			{
// 				eReturn = EMeereTOFSetFunction_WriteToDeviceError;
// 				break;
// 			}
		}
		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetPrincipalPointX(float * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterMeereTOFGigE_PrincipalPointX));

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetPrincipalPointX(float fParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_PrincipalPointX;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMeereTOFSetFunction_AlreadyInitializedError;
			break;
// 			MTF_API::mtfIntrinsicParam stIntrinsicParam;
// 			MTF_API::mtfDistortionParam stDistortionParam;
// 
// 			stIntrinsicParam.fCx = fParam;
// 
// 			if(GetFocalLengthX(&stIntrinsicParam.fFx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetFocalLengthY(&stIntrinsicParam.fFy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetPrincipalPointY(&stIntrinsicParam.fCy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 
// 			if(GetDistortionK1(&stDistortionParam.fK1))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetDistortionK2(&stDistortionParam.fK2))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetDistortionK3(&stDistortionParam.fK3))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetDistortionP1(&stDistortionParam.fP1))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetDistortionP2(&stDistortionParam.fP2))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetSkew(&stDistortionParam.fSkew))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			if(MTF_API::mtfSetLensParameter(m_hDevice, stIntrinsicParam, stDistortionParam) != MTF_API::ERROR_NO)
// 			{
// 				eReturn = EMeereTOFSetFunction_WriteToDeviceError;
// 				break;
// 			}
		}
		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetPrincipalPointY(float * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterMeereTOFGigE_PrincipalPointY));

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetPrincipalPointY(float fParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_PrincipalPointY;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMeereTOFSetFunction_AlreadyInitializedError;
			break;
// 			MTF_API::mtfIntrinsicParam stIntrinsicParam;
// 			MTF_API::mtfDistortionParam stDistortionParam;
// 
// 			stIntrinsicParam.fCy = fParam;
// 
// 			if(GetFocalLengthX(&stIntrinsicParam.fFx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetFocalLengthY(&stIntrinsicParam.fFy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetPrincipalPointX(&stIntrinsicParam.fCx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			if(GetDistortionK1(&stDistortionParam.fK1))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetDistortionK2(&stDistortionParam.fK2))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetDistortionK3(&stDistortionParam.fK3))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetDistortionP1(&stDistortionParam.fP1))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetDistortionP2(&stDistortionParam.fP2))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetSkew(&stDistortionParam.fSkew))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			if(MTF_API::mtfSetLensParameter(m_hDevice, stIntrinsicParam, stDistortionParam) != MTF_API::ERROR_NO)
// 			{
// 				eReturn = EMeereTOFSetFunction_WriteToDeviceError;
// 				break;
// 			}
		}
		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetDistortionK1(float * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterMeereTOFGigE_DistortionK1));

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetDistortionK1(float fParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_DistortionK1;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMeereTOFSetFunction_AlreadyInitializedError;
			break;
// 			MTF_API::mtfIntrinsicParam stIntrinsicParam;
// 			MTF_API::mtfDistortionParam stDistortionParam;
// 
// 			if(GetFocalLengthX(&stIntrinsicParam.fFx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetFocalLengthY(&stIntrinsicParam.fFy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetPrincipalPointX(&stIntrinsicParam.fCx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetPrincipalPointY(&stIntrinsicParam.fCy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			stDistortionParam.fK1 = fParam;
// 
// 			if(GetDistortionK2(&stDistortionParam.fK2))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetDistortionK3(&stDistortionParam.fK3))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetDistortionP1(&stDistortionParam.fP1))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetDistortionP2(&stDistortionParam.fP2))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetSkew(&stDistortionParam.fSkew))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			if(MTF_API::mtfSetLensParameter(m_hDevice, stIntrinsicParam, stDistortionParam) != MTF_API::ERROR_NO)
// 			{
// 				eReturn = EMeereTOFSetFunction_WriteToDeviceError;
// 				break;
// 			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetDistortionK2(float * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterMeereTOFGigE_DistortionK2));

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetDistortionK2(float fParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_DistortionK2;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMeereTOFSetFunction_AlreadyInitializedError;
			break;
// 			MTF_API::mtfIntrinsicParam stIntrinsicParam;
// 			MTF_API::mtfDistortionParam stDistortionParam;
// 
// 			if(GetFocalLengthX(&stIntrinsicParam.fFx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetFocalLengthY(&stIntrinsicParam.fFy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetPrincipalPointX(&stIntrinsicParam.fCx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetPrincipalPointY(&stIntrinsicParam.fCy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			stDistortionParam.fK2 = fParam;
// 
// 			if(GetDistortionK1(&stDistortionParam.fK1))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetDistortionK3(&stDistortionParam.fK3))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetDistortionP1(&stDistortionParam.fP1))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetDistortionP2(&stDistortionParam.fP2))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetSkew(&stDistortionParam.fSkew))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			if(MTF_API::mtfSetLensParameter(m_hDevice, stIntrinsicParam, stDistortionParam) != MTF_API::ERROR_NO)
// 			{
// 				eReturn = EMeereTOFSetFunction_WriteToDeviceError;
// 				break;
// 			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetDistortionK3(float * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterMeereTOFGigE_DistortionK3));

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetDistortionK3(float fParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_DistortionK3;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMeereTOFSetFunction_AlreadyInitializedError;
			break;
// 			MTF_API::mtfIntrinsicParam stIntrinsicParam;
// 			MTF_API::mtfDistortionParam stDistortionParam;
// 
// 			if(GetFocalLengthX(&stIntrinsicParam.fFx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetFocalLengthY(&stIntrinsicParam.fFy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetPrincipalPointX(&stIntrinsicParam.fCx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetPrincipalPointY(&stIntrinsicParam.fCy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			stDistortionParam.fK3 = fParam;
// 
// 			if(GetDistortionK1(&stDistortionParam.fK1))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetDistortionK2(&stDistortionParam.fK2))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetDistortionP1(&stDistortionParam.fP1))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetDistortionP2(&stDistortionParam.fP2))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetSkew(&stDistortionParam.fSkew))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			if(MTF_API::mtfSetLensParameter(m_hDevice, stIntrinsicParam, stDistortionParam) != MTF_API::ERROR_NO)
// 			{
// 				eReturn = EMeereTOFSetFunction_WriteToDeviceError;
// 				break;
// 			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetDistortionP1(float * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterMeereTOFGigE_DistortionP1));

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetDistortionP1(float fParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_DistortionP1;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMeereTOFSetFunction_AlreadyInitializedError;
			break;
// 			MTF_API::mtfIntrinsicParam stIntrinsicParam;
// 			MTF_API::mtfDistortionParam stDistortionParam;
// 
// 			if(GetFocalLengthX(&stIntrinsicParam.fFx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetFocalLengthY(&stIntrinsicParam.fFy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetPrincipalPointX(&stIntrinsicParam.fCx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetPrincipalPointY(&stIntrinsicParam.fCy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			stDistortionParam.fP1 = fParam;
// 
// 			if(GetDistortionK1(&stDistortionParam.fK1))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetDistortionK2(&stDistortionParam.fK2))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetDistortionK3(&stDistortionParam.fK3))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetDistortionP2(&stDistortionParam.fP2))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetSkew(&stDistortionParam.fSkew))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			if(MTF_API::mtfSetLensParameter(m_hDevice, stIntrinsicParam, stDistortionParam) != MTF_API::ERROR_NO)
// 			{
// 				eReturn = EMeereTOFSetFunction_WriteToDeviceError;
// 				break;
// 			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetDistortionP2(float * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterMeereTOFGigE_DistortionP2));

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetDistortionP2(float fParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_DistortionP2;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMeereTOFSetFunction_AlreadyInitializedError;
			break;
// 			MTF_API::mtfIntrinsicParam stIntrinsicParam;
// 			MTF_API::mtfDistortionParam stDistortionParam;
// 
// 			if(GetFocalLengthX(&stIntrinsicParam.fFx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetFocalLengthY(&stIntrinsicParam.fFy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetPrincipalPointX(&stIntrinsicParam.fCx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetPrincipalPointY(&stIntrinsicParam.fCy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			stDistortionParam.fP2 = fParam;
// 
// 			if(GetDistortionK1(&stDistortionParam.fK1))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetDistortionK2(&stDistortionParam.fK2))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetDistortionK3(&stDistortionParam.fK3))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetDistortionP1(&stDistortionParam.fP1))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetSkew(&stDistortionParam.fSkew))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			if(MTF_API::mtfSetLensParameter(m_hDevice, stIntrinsicParam, stDistortionParam) != MTF_API::ERROR_NO)
// 			{
// 				eReturn = EMeereTOFSetFunction_WriteToDeviceError;
// 				break;
// 			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetSkew(float * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterMeereTOFGigE_Skew));

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetSkew(float fParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_Skew;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMeereTOFSetFunction_AlreadyInitializedError;
			break;
// 			MTF_API::mtfIntrinsicParam stIntrinsicParam;
// 			MTF_API::mtfDistortionParam stDistortionParam;
// 
// 			if(GetFocalLengthX(&stIntrinsicParam.fFx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetFocalLengthY(&stIntrinsicParam.fFy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetPrincipalPointX(&stIntrinsicParam.fCx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetPrincipalPointY(&stIntrinsicParam.fCy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			stDistortionParam.fSkew = fParam;
// 
// 			if(GetDistortionK1(&stDistortionParam.fK1))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetDistortionK2(&stDistortionParam.fK2))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetDistortionK3(&stDistortionParam.fK3))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetDistortionP1(&stDistortionParam.fP1))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetDistortionP2(&stDistortionParam.fP2))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			if(MTF_API::mtfSetLensParameter(m_hDevice, stIntrinsicParam, stDistortionParam) != MTF_API::ERROR_NO)
// 			{
// 				eReturn = EMeereTOFSetFunction_WriteToDeviceError;
// 				break;
// 			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetRGBFocalLengthX(float * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterMeereTOFGigE_RGBFocalLengthX));

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetRGBFocalLengthX(float fParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_RGBFocalLengthX;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMeereTOFSetFunction_AlreadyInitializedError;
			break;
// 			MTF_API::mtfIntrinsicParam stRGBIntrinsicParam;
// 			MTF_API::mtfDistortionParam stRGBDistortionParam;
// 
// 			stRGBIntrinsicParam.fFx = fParam;
// 
// 			if(GetRGBFocalLengthY(&stRGBIntrinsicParam.fFy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBPrincipalPointX(&stRGBIntrinsicParam.fCx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBPrincipalPointY(&stRGBIntrinsicParam.fCy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 
// 			if(GetRGBDistortionK1(&stRGBDistortionParam.fK1))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBDistortionK2(&stRGBDistortionParam.fK2))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBDistortionK3(&stRGBDistortionParam.fK3))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBDistortionP1(&stRGBDistortionParam.fP1))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBDistortionP2(&stRGBDistortionParam.fP2))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBSkew(&stRGBDistortionParam.fSkew))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			if(MTF_API::mtfSetRGBLensParameter(m_hDevice, stRGBIntrinsicParam, stRGBDistortionParam) != MTF_API::ERROR_NO)
// 			{
// 				eReturn = EMeereTOFSetFunction_WriteToDeviceError;
// 				break;
// 			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetRGBFocalLengthY(float * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterMeereTOFGigE_RGBFocalLengthY));

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetRGBFocalLengthY(float fParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_RGBFocalLengthY;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMeereTOFSetFunction_AlreadyInitializedError;
			break;
// 			MTF_API::mtfIntrinsicParam stRGBIntrinsicParam;
// 			MTF_API::mtfDistortionParam stRGBDistortionParam;
// 
// 			stRGBIntrinsicParam.fFy = fParam;
// 
// 			if(GetRGBFocalLengthX(&stRGBIntrinsicParam.fFx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBPrincipalPointX(&stRGBIntrinsicParam.fCx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBPrincipalPointY(&stRGBIntrinsicParam.fCy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 
// 			if(GetRGBDistortionK1(&stRGBDistortionParam.fK1))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBDistortionK2(&stRGBDistortionParam.fK2))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBDistortionK3(&stRGBDistortionParam.fK3))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBDistortionP1(&stRGBDistortionParam.fP1))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBDistortionP2(&stRGBDistortionParam.fP2))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBSkew(&stRGBDistortionParam.fSkew))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			if(MTF_API::mtfSetRGBLensParameter(m_hDevice, stRGBIntrinsicParam, stRGBDistortionParam) != MTF_API::ERROR_NO)
// 			{
// 				eReturn = EMeereTOFSetFunction_WriteToDeviceError;
// 				break;
// 			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetRGBPrincipalPointX(float * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterMeereTOFGigE_RGBPrincipalPointX));

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetRGBPrincipalPointX(float fParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_RGBPrincipalPointX;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMeereTOFSetFunction_AlreadyInitializedError;
			break;
// 			MTF_API::mtfIntrinsicParam stRGBIntrinsicParam;
// 			MTF_API::mtfDistortionParam stRGBDistortionParam;
// 
// 			stRGBIntrinsicParam.fCx = fParam;
// 
// 			if(GetRGBFocalLengthX(&stRGBIntrinsicParam.fFx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBFocalLengthY(&stRGBIntrinsicParam.fFy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBPrincipalPointY(&stRGBIntrinsicParam.fCy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 
// 			if(GetRGBDistortionK1(&stRGBDistortionParam.fK1))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBDistortionK2(&stRGBDistortionParam.fK2))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBDistortionK3(&stRGBDistortionParam.fK3))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBDistortionP1(&stRGBDistortionParam.fP1))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBDistortionP2(&stRGBDistortionParam.fP2))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBSkew(&stRGBDistortionParam.fSkew))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			if(MTF_API::mtfSetRGBLensParameter(m_hDevice, stRGBIntrinsicParam, stRGBDistortionParam) != MTF_API::ERROR_NO)
// 			{
// 				eReturn = EMeereTOFSetFunction_WriteToDeviceError;
// 				break;
// 			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetRGBPrincipalPointY(float * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterMeereTOFGigE_RGBPrincipalPointY));

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetRGBPrincipalPointY(float fParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_RGBPrincipalPointY;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMeereTOFSetFunction_AlreadyInitializedError;
			break;
// 			MTF_API::mtfIntrinsicParam stRGBIntrinsicParam;
// 			MTF_API::mtfDistortionParam stRGBDistortionParam;
// 
// 			stRGBIntrinsicParam.fCy = fParam;
// 
// 			if(GetRGBFocalLengthX(&stRGBIntrinsicParam.fFx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBFocalLengthY(&stRGBIntrinsicParam.fFy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBPrincipalPointX(&stRGBIntrinsicParam.fCx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			if(GetRGBDistortionK1(&stRGBDistortionParam.fK1))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBDistortionK2(&stRGBDistortionParam.fK2))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBDistortionK3(&stRGBDistortionParam.fK3))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBDistortionP1(&stRGBDistortionParam.fP1))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBDistortionP2(&stRGBDistortionParam.fP2))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBSkew(&stRGBDistortionParam.fSkew))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			if(MTF_API::mtfSetRGBLensParameter(m_hDevice, stRGBIntrinsicParam, stRGBDistortionParam) != MTF_API::ERROR_NO)
// 			{
// 				eReturn = EMeereTOFSetFunction_WriteToDeviceError;
// 				break;
// 			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetRGBDistortionK1(float * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterMeereTOFGigE_RGBDistortionK1));

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetRGBDistortionK1(float fParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_RGBDistortionK1;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMeereTOFSetFunction_AlreadyInitializedError;
			break;
// 			MTF_API::mtfIntrinsicParam stRGBIntrinsicParam;
// 			MTF_API::mtfDistortionParam stRGBDistortionParam;
// 
// 			if(GetRGBFocalLengthX(&stRGBIntrinsicParam.fFx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBFocalLengthY(&stRGBIntrinsicParam.fFy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBPrincipalPointX(&stRGBIntrinsicParam.fCx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBPrincipalPointY(&stRGBIntrinsicParam.fCy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			stRGBDistortionParam.fK1 = fParam;
// 
// 			if(GetRGBDistortionK2(&stRGBDistortionParam.fK2))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBDistortionK3(&stRGBDistortionParam.fK3))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBDistortionP1(&stRGBDistortionParam.fP1))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBDistortionP2(&stRGBDistortionParam.fP2))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBSkew(&stRGBDistortionParam.fSkew))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			if(MTF_API::mtfSetRGBLensParameter(m_hDevice, stRGBIntrinsicParam, stRGBDistortionParam) != MTF_API::ERROR_NO)
// 			{
// 				eReturn = EMeereTOFSetFunction_WriteToDeviceError;
// 				break;
// 			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetRGBDistortionK2(float * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterMeereTOFGigE_RGBDistortionK2));

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetRGBDistortionK2(float fParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_RGBDistortionK2;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMeereTOFSetFunction_AlreadyInitializedError;
			break;
// 			MTF_API::mtfIntrinsicParam stRGBIntrinsicParam;
// 			MTF_API::mtfDistortionParam stRGBDistortionParam;
// 
// 			if(GetRGBFocalLengthX(&stRGBIntrinsicParam.fFx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBFocalLengthY(&stRGBIntrinsicParam.fFy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBPrincipalPointX(&stRGBIntrinsicParam.fCx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBPrincipalPointY(&stRGBIntrinsicParam.fCy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			stRGBDistortionParam.fK2 = fParam;
// 
// 			if(GetRGBDistortionK1(&stRGBDistortionParam.fK1))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBDistortionK3(&stRGBDistortionParam.fK3))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBDistortionP1(&stRGBDistortionParam.fP1))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBDistortionP2(&stRGBDistortionParam.fP2))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBSkew(&stRGBDistortionParam.fSkew))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			if(MTF_API::mtfSetRGBLensParameter(m_hDevice, stRGBIntrinsicParam, stRGBDistortionParam) != MTF_API::ERROR_NO)
// 			{
// 				eReturn = EMeereTOFSetFunction_WriteToDeviceError;
// 				break;
// 			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetRGBDistortionK3(float * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterMeereTOFGigE_RGBDistortionK3));

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetRGBDistortionK3(float fParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_RGBDistortionK3;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMeereTOFSetFunction_AlreadyInitializedError;
			break;
// 			MTF_API::mtfIntrinsicParam stRGBIntrinsicParam;
// 			MTF_API::mtfDistortionParam stRGBDistortionParam;
// 
// 			if(GetRGBFocalLengthX(&stRGBIntrinsicParam.fFx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBFocalLengthY(&stRGBIntrinsicParam.fFy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBPrincipalPointX(&stRGBIntrinsicParam.fCx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBPrincipalPointY(&stRGBIntrinsicParam.fCy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			stRGBDistortionParam.fK3 = fParam;
// 
// 			if(GetRGBDistortionK1(&stRGBDistortionParam.fK1))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBDistortionK2(&stRGBDistortionParam.fK2))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBDistortionP1(&stRGBDistortionParam.fP1))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBDistortionP2(&stRGBDistortionParam.fP2))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBSkew(&stRGBDistortionParam.fSkew))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			if(MTF_API::mtfSetRGBLensParameter(m_hDevice, stRGBIntrinsicParam, stRGBDistortionParam) != MTF_API::ERROR_NO)
// 			{
// 				eReturn = EMeereTOFSetFunction_WriteToDeviceError;
// 				break;
// 			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetRGBDistortionP1(float * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterMeereTOFGigE_RGBDistortionP1));

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetRGBDistortionP1(float fParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_RGBDistortionP1;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMeereTOFSetFunction_AlreadyInitializedError;
			break;
// 			MTF_API::mtfIntrinsicParam stRGBIntrinsicParam;
// 			MTF_API::mtfDistortionParam stRGBDistortionParam;
// 
// 			stRGBIntrinsicParam.fFx = fParam;
// 
// 			if(GetRGBFocalLengthX(&stRGBIntrinsicParam.fFx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBFocalLengthY(&stRGBIntrinsicParam.fFy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBPrincipalPointX(&stRGBIntrinsicParam.fCx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBPrincipalPointY(&stRGBIntrinsicParam.fCy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			stRGBDistortionParam.fP1 = fParam;
// 
// 			if(GetRGBDistortionK1(&stRGBDistortionParam.fK1))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBDistortionK2(&stRGBDistortionParam.fK2))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBDistortionK3(&stRGBDistortionParam.fK3))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBDistortionP2(&stRGBDistortionParam.fP2))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBSkew(&stRGBDistortionParam.fSkew))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			if(MTF_API::mtfSetRGBLensParameter(m_hDevice, stRGBIntrinsicParam, stRGBDistortionParam) != MTF_API::ERROR_NO)
// 			{
// 				eReturn = EMeereTOFSetFunction_WriteToDeviceError;
// 				break;
// 			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetRGBDistortionP2(float * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterMeereTOFGigE_RGBDistortionP2));

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetRGBDistortionP2(float fParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_RGBDistortionP2;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMeereTOFSetFunction_AlreadyInitializedError;
			break;
// 			MTF_API::mtfIntrinsicParam stRGBIntrinsicParam;
// 			MTF_API::mtfDistortionParam stRGBDistortionParam;
// 
// 			stRGBIntrinsicParam.fFx = fParam;
// 
// 			if(GetRGBFocalLengthX(&stRGBIntrinsicParam.fFx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBFocalLengthY(&stRGBIntrinsicParam.fFy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBPrincipalPointX(&stRGBIntrinsicParam.fCx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBPrincipalPointY(&stRGBIntrinsicParam.fCy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			stRGBDistortionParam.fP2 = fParam;
// 
// 			if(GetRGBDistortionK1(&stRGBDistortionParam.fK1))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBDistortionK2(&stRGBDistortionParam.fK2))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBDistortionK3(&stRGBDistortionParam.fK3))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBDistortionP1(&stRGBDistortionParam.fP1))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBSkew(&stRGBDistortionParam.fSkew))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			if(MTF_API::mtfSetRGBLensParameter(m_hDevice, stRGBIntrinsicParam, stRGBDistortionParam) != MTF_API::ERROR_NO)
// 			{
// 				eReturn = EMeereTOFSetFunction_WriteToDeviceError;
// 				break;
// 			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetRGBSkew(float * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterMeereTOFGigE_RGBSkew));

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetRGBSkew(float fParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_RGBSkew;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMeereTOFSetFunction_AlreadyInitializedError;
			break;
// 			MTF_API::mtfIntrinsicParam stRGBIntrinsicParam;
// 			MTF_API::mtfDistortionParam stRGBDistortionParam;
// 
// 			stRGBIntrinsicParam.fFx = fParam;
// 
// 			if(GetRGBFocalLengthX(&stRGBIntrinsicParam.fFx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBFocalLengthY(&stRGBIntrinsicParam.fFy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBPrincipalPointX(&stRGBIntrinsicParam.fCx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBPrincipalPointY(&stRGBIntrinsicParam.fCy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			stRGBDistortionParam.fSkew = fParam;
// 
// 			if(GetRGBDistortionK1(&stRGBDistortionParam.fK1))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBDistortionK2(&stRGBDistortionParam.fK2))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBDistortionK3(&stRGBDistortionParam.fK3))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBDistortionP1(&stRGBDistortionParam.fP1))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetRGBDistortionP2(&stRGBDistortionParam.fP2))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			if(MTF_API::mtfSetRGBLensParameter(m_hDevice, stRGBIntrinsicParam, stRGBDistortionParam) != MTF_API::ERROR_NO)
// 			{
// 				eReturn = EMeereTOFSetFunction_WriteToDeviceError;
// 				break;
// 			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetExtrinsicR11(float * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterMeereTOFGigE_ExtrinsicR11));

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetExtrinsicR11(float fParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_ExtrinsicR11;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMeereTOFSetFunction_AlreadyInitializedError;
			break;
// 			MTF_API::mtfExtrinsicParam stExtrinsicParam;
// 
// 			stExtrinsicParam.fR11 = fParam;
// 
// 			if(GetExtrinsicR12(&stExtrinsicParam.fR12))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR13(&stExtrinsicParam.fR13))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR21(&stExtrinsicParam.fR21))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR22(&stExtrinsicParam.fR22))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR23(&stExtrinsicParam.fR23))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR31(&stExtrinsicParam.fR31))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR32(&stExtrinsicParam.fR32))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR33(&stExtrinsicParam.fR33))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicTx(&stExtrinsicParam.fTx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicTy(&stExtrinsicParam.fTy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicTz(&stExtrinsicParam.fTz))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			if(MTF_API::mtfSetExtrinsicParameter(m_hDevice, stExtrinsicParam) != MTF_API::ERROR_NO)
// 			{
// 				eReturn = EMeereTOFSetFunction_WriteToDeviceError;
// 				break;
// 			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetExtrinsicR12(float * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterMeereTOFGigE_ExtrinsicR12));

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetExtrinsicR12(float fParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_ExtrinsicR12;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMeereTOFSetFunction_AlreadyInitializedError;
			break;
// 			MTF_API::mtfExtrinsicParam stExtrinsicParam;
// 
// 			stExtrinsicParam.fR12 = fParam;
// 
// 			if(GetExtrinsicR11(&stExtrinsicParam.fR11))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR13(&stExtrinsicParam.fR13))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR21(&stExtrinsicParam.fR21))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR22(&stExtrinsicParam.fR22))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR23(&stExtrinsicParam.fR23))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR31(&stExtrinsicParam.fR31))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR32(&stExtrinsicParam.fR32))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR33(&stExtrinsicParam.fR33))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicTx(&stExtrinsicParam.fTx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicTy(&stExtrinsicParam.fTy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicTz(&stExtrinsicParam.fTz))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			if(MTF_API::mtfSetExtrinsicParameter(m_hDevice, stExtrinsicParam) != MTF_API::ERROR_NO)
// 			{
// 				eReturn = EMeereTOFSetFunction_WriteToDeviceError;
// 				break;
// 			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetExtrinsicR13(float * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterMeereTOFGigE_ExtrinsicR13));

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetExtrinsicR13(float fParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_ExtrinsicR13;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMeereTOFSetFunction_AlreadyInitializedError;
			break;
// 			MTF_API::mtfExtrinsicParam stExtrinsicParam;
// 
// 			stExtrinsicParam.fR13 = fParam;
// 
// 			if(GetExtrinsicR11(&stExtrinsicParam.fR11))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR12(&stExtrinsicParam.fR12))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR21(&stExtrinsicParam.fR21))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR22(&stExtrinsicParam.fR22))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR23(&stExtrinsicParam.fR23))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR31(&stExtrinsicParam.fR31))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR32(&stExtrinsicParam.fR32))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR33(&stExtrinsicParam.fR33))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicTx(&stExtrinsicParam.fTx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicTy(&stExtrinsicParam.fTy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicTz(&stExtrinsicParam.fTz))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			if(MTF_API::mtfSetExtrinsicParameter(m_hDevice, stExtrinsicParam) != MTF_API::ERROR_NO)
// 			{
// 				eReturn = EMeereTOFSetFunction_WriteToDeviceError;
// 				break;
// 			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetExtrinsicR21(float * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterMeereTOFGigE_ExtrinsicR21));

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetExtrinsicR21(float fParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_ExtrinsicR21;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMeereTOFSetFunction_AlreadyInitializedError;
			break;
// 			MTF_API::mtfExtrinsicParam stExtrinsicParam;
// 
// 			stExtrinsicParam.fR21 = fParam;
// 
// 			if(GetExtrinsicR11(&stExtrinsicParam.fR11))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR12(&stExtrinsicParam.fR12))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR13(&stExtrinsicParam.fR13))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR22(&stExtrinsicParam.fR22))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR23(&stExtrinsicParam.fR23))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR31(&stExtrinsicParam.fR31))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR32(&stExtrinsicParam.fR32))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR33(&stExtrinsicParam.fR33))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicTx(&stExtrinsicParam.fTx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicTy(&stExtrinsicParam.fTy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicTz(&stExtrinsicParam.fTz))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			if(MTF_API::mtfSetExtrinsicParameter(m_hDevice, stExtrinsicParam) != MTF_API::ERROR_NO)
// 			{
// 				eReturn = EMeereTOFSetFunction_WriteToDeviceError;
// 				break;
// 			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetExtrinsicR22(float * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterMeereTOFGigE_ExtrinsicR22));

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetExtrinsicR22(float fParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_ExtrinsicR22;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMeereTOFSetFunction_AlreadyInitializedError;
			break;
// 			MTF_API::mtfExtrinsicParam stExtrinsicParam;
// 
// 			stExtrinsicParam.fR22 = fParam;
// 
// 			if(GetExtrinsicR11(&stExtrinsicParam.fR11))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR12(&stExtrinsicParam.fR12))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR13(&stExtrinsicParam.fR13))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR21(&stExtrinsicParam.fR21))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR23(&stExtrinsicParam.fR23))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR31(&stExtrinsicParam.fR31))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR32(&stExtrinsicParam.fR32))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR33(&stExtrinsicParam.fR33))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicTx(&stExtrinsicParam.fTx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicTy(&stExtrinsicParam.fTy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicTz(&stExtrinsicParam.fTz))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			if(MTF_API::mtfSetExtrinsicParameter(m_hDevice, stExtrinsicParam) != MTF_API::ERROR_NO)
// 			{
// 				eReturn = EMeereTOFSetFunction_WriteToDeviceError;
// 				break;
// 			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetExtrinsicR23(float * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterMeereTOFGigE_ExtrinsicR23));

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetExtrinsicR23(float fParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_ExtrinsicR23;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMeereTOFSetFunction_AlreadyInitializedError;
			break;
// 			MTF_API::mtfExtrinsicParam stExtrinsicParam;
// 
// 			stExtrinsicParam.fR23 = fParam;
// 
// 			if(GetExtrinsicR11(&stExtrinsicParam.fR11))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR12(&stExtrinsicParam.fR12))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR13(&stExtrinsicParam.fR13))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR21(&stExtrinsicParam.fR21))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR22(&stExtrinsicParam.fR22))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR31(&stExtrinsicParam.fR31))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR32(&stExtrinsicParam.fR32))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR33(&stExtrinsicParam.fR33))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicTx(&stExtrinsicParam.fTx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicTy(&stExtrinsicParam.fTy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicTz(&stExtrinsicParam.fTz))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			if(MTF_API::mtfSetExtrinsicParameter(m_hDevice, stExtrinsicParam) != MTF_API::ERROR_NO)
// 			{
// 				eReturn = EMeereTOFSetFunction_WriteToDeviceError;
// 				break;
// 			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetExtrinsicR31(float * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterMeereTOFGigE_ExtrinsicR31));

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetExtrinsicR31(float fParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_ExtrinsicR31;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMeereTOFSetFunction_AlreadyInitializedError;
			break;
// 			MTF_API::mtfExtrinsicParam stExtrinsicParam;
// 
// 			stExtrinsicParam.fR31 = fParam;
// 
// 			if(GetExtrinsicR11(&stExtrinsicParam.fR11))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR12(&stExtrinsicParam.fR12))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR13(&stExtrinsicParam.fR13))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR21(&stExtrinsicParam.fR21))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR22(&stExtrinsicParam.fR22))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR23(&stExtrinsicParam.fR23))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR32(&stExtrinsicParam.fR32))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR33(&stExtrinsicParam.fR33))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicTx(&stExtrinsicParam.fTx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicTy(&stExtrinsicParam.fTy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicTz(&stExtrinsicParam.fTz))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			if(MTF_API::mtfSetExtrinsicParameter(m_hDevice, stExtrinsicParam) != MTF_API::ERROR_NO)
// 			{
// 				eReturn = EMeereTOFSetFunction_WriteToDeviceError;
// 				break;
// 			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetExtrinsicR32(float * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterMeereTOFGigE_ExtrinsicR32));

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetExtrinsicR32(float fParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_ExtrinsicR32;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMeereTOFSetFunction_AlreadyInitializedError;
			break;
// 			MTF_API::mtfExtrinsicParam stExtrinsicParam;
// 
// 			stExtrinsicParam.fR32 = fParam;
// 
// 			if(GetExtrinsicR11(&stExtrinsicParam.fR11))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR12(&stExtrinsicParam.fR12))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR13(&stExtrinsicParam.fR13))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR21(&stExtrinsicParam.fR21))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR22(&stExtrinsicParam.fR22))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR23(&stExtrinsicParam.fR23))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR31(&stExtrinsicParam.fR31))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR33(&stExtrinsicParam.fR33))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicTx(&stExtrinsicParam.fTx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicTy(&stExtrinsicParam.fTy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicTz(&stExtrinsicParam.fTz))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			if(MTF_API::mtfSetExtrinsicParameter(m_hDevice, stExtrinsicParam) != MTF_API::ERROR_NO)
// 			{
// 				eReturn = EMeereTOFSetFunction_WriteToDeviceError;
// 				break;
// 			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetExtrinsicR33(float * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterMeereTOFGigE_ExtrinsicR33));

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetExtrinsicR33(float fParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_ExtrinsicR33;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMeereTOFSetFunction_AlreadyInitializedError;
			break;
// 			MTF_API::mtfExtrinsicParam stExtrinsicParam;
// 
// 			stExtrinsicParam.fR33 = fParam;
// 
// 			if(GetExtrinsicR11(&stExtrinsicParam.fR11))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR12(&stExtrinsicParam.fR12))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR13(&stExtrinsicParam.fR13))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR21(&stExtrinsicParam.fR21))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR22(&stExtrinsicParam.fR22))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR23(&stExtrinsicParam.fR23))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR31(&stExtrinsicParam.fR31))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR32(&stExtrinsicParam.fR32))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicTx(&stExtrinsicParam.fTx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicTy(&stExtrinsicParam.fTy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicTz(&stExtrinsicParam.fTz))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			if(MTF_API::mtfSetExtrinsicParameter(m_hDevice, stExtrinsicParam) != MTF_API::ERROR_NO)
// 			{
// 				eReturn = EMeereTOFSetFunction_WriteToDeviceError;
// 				break;
// 			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetExtrinsicTx(float * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterMeereTOFGigE_ExtrinsicTx));

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetExtrinsicTx(float fParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_ExtrinsicTx;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMeereTOFSetFunction_AlreadyInitializedError;
			break;
// 			MTF_API::mtfExtrinsicParam stExtrinsicParam;
// 
// 			stExtrinsicParam.fTx = fParam;
// 
// 			if(GetExtrinsicR11(&stExtrinsicParam.fR11))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR12(&stExtrinsicParam.fR12))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR13(&stExtrinsicParam.fR13))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR21(&stExtrinsicParam.fR21))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR22(&stExtrinsicParam.fR22))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR23(&stExtrinsicParam.fR23))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR31(&stExtrinsicParam.fR31))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR32(&stExtrinsicParam.fR32))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR33(&stExtrinsicParam.fR33))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicTy(&stExtrinsicParam.fTy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicTz(&stExtrinsicParam.fTz))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			if(MTF_API::mtfSetExtrinsicParameter(m_hDevice, stExtrinsicParam) != MTF_API::ERROR_NO)
// 			{
// 				eReturn = EMeereTOFSetFunction_WriteToDeviceError;
// 				break;
// 			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetExtrinsicTy(float * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterMeereTOFGigE_ExtrinsicTy));

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetExtrinsicTy(float fParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_ExtrinsicTy;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMeereTOFSetFunction_AlreadyInitializedError;
			break;
// 			MTF_API::mtfExtrinsicParam stExtrinsicParam;
// 
// 			stExtrinsicParam.fTy = fParam;
// 
// 			if(GetExtrinsicR11(&stExtrinsicParam.fR11))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR12(&stExtrinsicParam.fR12))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR13(&stExtrinsicParam.fR13))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR21(&stExtrinsicParam.fR21))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR22(&stExtrinsicParam.fR22))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR23(&stExtrinsicParam.fR23))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR31(&stExtrinsicParam.fR31))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR32(&stExtrinsicParam.fR32))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR33(&stExtrinsicParam.fR33))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicTx(&stExtrinsicParam.fTx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicTz(&stExtrinsicParam.fTz))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			if(MTF_API::mtfSetExtrinsicParameter(m_hDevice, stExtrinsicParam) != MTF_API::ERROR_NO)
// 			{
// 				eReturn = EMeereTOFSetFunction_WriteToDeviceError;
// 				break;
// 			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMeereTOFGetFunction CDeviceMeereTOFGigE::GetExtrinsicTz(float * pParam)
{
	EMeereTOFGetFunction eReturn = EMeereTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMeereTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterMeereTOFGigE_ExtrinsicTz));

		eReturn = EMeereTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMeereTOFSetFunction CDeviceMeereTOFGigE::SetExtrinsicTz(float fParam)
{
	EMeereTOFSetFunction eReturn = EMeereTOFSetFunction_UnknownError;

	EDeviceParameterMeereTOFGigE eSaveID = EDeviceParameterMeereTOFGigE_ExtrinsicTz;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EMeereTOFSetFunction_AlreadyInitializedError;
			break;
// 			MTF_API::mtfExtrinsicParam stExtrinsicParam;
// 
// 			stExtrinsicParam.fTz = fParam;
// 
// 			if(GetExtrinsicR11(&stExtrinsicParam.fR11))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR12(&stExtrinsicParam.fR12))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR13(&stExtrinsicParam.fR13))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR21(&stExtrinsicParam.fR21))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR22(&stExtrinsicParam.fR22))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR23(&stExtrinsicParam.fR23))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR31(&stExtrinsicParam.fR31))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR32(&stExtrinsicParam.fR32))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicR33(&stExtrinsicParam.fR33))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicTx(&stExtrinsicParam.fTx))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 			if(GetExtrinsicTy(&stExtrinsicParam.fTy))
// 			{
// 				eReturn = EMeereTOFSetFunction_ReadOnDatabaseError;
// 				break;
// 			}
// 
// 			if(MTF_API::mtfSetExtrinsicParameter(m_hDevice, stExtrinsicParam) != MTF_API::ERROR_NO)
// 			{
// 				eReturn = EMeereTOFSetFunction_WriteToDeviceError;
// 				break;
// 			}
		}

		CString strSave;
		strSave.Format(_T("%f"), fParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMeereTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMeereTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMeereTOFGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceMeereTOFGigE::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = false;
	bool bFoundID = true;

	do
	{
		switch(nParam)		
		{
		case EDeviceParameterBaslerTOFGigE_DeviceID:
			bReturn = !SetDeviceID(strValue);
			break;
		case EDeviceParameterMeereTOFGigE_DrawPoint:
			bReturn = !SetDrawPoint(_ttoi(strValue));
			break;
		case EDeviceParameterMeereTOFGigE_InitType:
			bReturn = !SetInitType((EDeviceMeereTOFInitType)_ttoi(strValue));
			break;
		case EDeviceParameterMeereTOFGigE_IntegrationTime:
			bReturn = !SetIntegrationTime(_ttoi(strValue));
			break;
		case EDeviceParameterMeereTOFGigE_Offset:
			bReturn = !SetOffset(_ttoi(strValue));
			break;
		case EDeviceParameterMeereTOFGigE_DepthThreshold:
			bReturn = !SetDepthThreshold(_ttoi(strValue));
			break;
		case EDeviceParameterMeereTOFGigE_ScatterThreshold:
			bReturn = !SetScatterThreshold(_ttoi(strValue));
			break;
		case EDeviceParameterMeereTOFGigE_MinDepth:
			bReturn = !SetMinDepth(_ttoi(strValue));
			break;
		case EDeviceParameterMeereTOFGigE_MaxDepth:
			bReturn = !SetMaxDepth(_ttoi(strValue));
			break;
		case EDeviceParameterMeereTOFGigE_FlipHorizontal:
			bReturn = !SetFlipHorizontal(_ttoi(strValue));
			break;
		case EDeviceParameterMeereTOFGigE_FlipVertical:
			bReturn = !SetFlipVertical(_ttoi(strValue));
			break;
		case EDeviceParameterMeereTOFGigE_MultiSyncMode:
			bReturn = !SetMultiSyncMode(_ttoi(strValue));
			break;
		case EDeviceParameterMeereTOFGigE_FPSDelay:
			bReturn = !SetFPSDelay(_ttoi(strValue));
			break;
		case EDeviceParameterMeereTOFGigE_RemoveEdge:
			bReturn = !SetRemoveEdge(_ttoi(strValue));
			break;
		case EDeviceParameterMeereTOFGigE_Undistortion:
			bReturn = !SetUndistortion((EDeviceMeereTOFUndistortion)_ttoi(strValue));
			break;
		case EDeviceParameterMeereTOFGigE_Orthogonal:
			bReturn = !SetOrthogonal(_ttoi(strValue));
			break;
		case EDeviceParameterMeereTOFGigE_FocalLengthX:
			bReturn = !SetFocalLengthX(_ttof(strValue));
			break;
		case EDeviceParameterMeereTOFGigE_FocalLengthY:
			bReturn = !SetFocalLengthY(_ttof(strValue));
			break;
		case EDeviceParameterMeereTOFGigE_PrincipalPointX:
			bReturn = !SetPrincipalPointX(_ttof(strValue));
			break;
		case EDeviceParameterMeereTOFGigE_PrincipalPointY:
			bReturn = !SetPrincipalPointY(_ttof(strValue));
			break;
		case EDeviceParameterMeereTOFGigE_DistortionK1:
			bReturn = !SetDistortionK1(_ttof(strValue));
			break;
		case EDeviceParameterMeereTOFGigE_DistortionK2:
			bReturn = !SetDistortionK2(_ttof(strValue));
			break;
		case EDeviceParameterMeereTOFGigE_DistortionK3:
			bReturn = !SetDistortionK3(_ttof(strValue));
			break;
		case EDeviceParameterMeereTOFGigE_DistortionP1:
			bReturn = !SetDistortionP1(_ttof(strValue));
			break;
		case EDeviceParameterMeereTOFGigE_DistortionP2:
			bReturn = !SetDistortionP2(_ttof(strValue));
			break;
		case EDeviceParameterMeereTOFGigE_Skew:
			bReturn = !SetSkew(_ttof(strValue));
			break;
		case EDeviceParameterMeereTOFGigE_RGBFocalLengthX:
			bReturn = !SetRGBFocalLengthX(_ttof(strValue));
			break;
		case EDeviceParameterMeereTOFGigE_RGBFocalLengthY:
			bReturn = !SetRGBFocalLengthY(_ttof(strValue));
			break;
		case EDeviceParameterMeereTOFGigE_RGBPrincipalPointX:
			bReturn = !SetRGBPrincipalPointX(_ttof(strValue));
			break;
		case EDeviceParameterMeereTOFGigE_RGBPrincipalPointY:
			bReturn = !SetRGBPrincipalPointY(_ttof(strValue));
			break;
		case EDeviceParameterMeereTOFGigE_RGBDistortionK1:
			bReturn = !SetRGBDistortionK1(_ttof(strValue));
			break;
		case EDeviceParameterMeereTOFGigE_RGBDistortionK2:
			bReturn = !SetRGBDistortionK2(_ttof(strValue));
			break;
		case EDeviceParameterMeereTOFGigE_RGBDistortionK3:
			bReturn = !SetRGBDistortionK3(_ttof(strValue));
			break;
		case EDeviceParameterMeereTOFGigE_RGBDistortionP1:
			bReturn = !SetRGBDistortionP1(_ttof(strValue));
			break;
		case EDeviceParameterMeereTOFGigE_RGBDistortionP2:
			bReturn = !SetRGBDistortionP2(_ttof(strValue));
			break;
		case EDeviceParameterMeereTOFGigE_RGBSkew:
			bReturn = !SetRGBSkew(_ttof(strValue));
			break;
		case EDeviceParameterMeereTOFGigE_ExtrinsicR11:
			bReturn = !SetExtrinsicR11(_ttof(strValue));
			break;
		case EDeviceParameterMeereTOFGigE_ExtrinsicR12:
			bReturn = !SetExtrinsicR12(_ttof(strValue));
			break;
		case EDeviceParameterMeereTOFGigE_ExtrinsicR13:
			bReturn = !SetExtrinsicR13(_ttof(strValue));
			break;
		case EDeviceParameterMeereTOFGigE_ExtrinsicR21:
			bReturn = !SetExtrinsicR21(_ttof(strValue));
			break;
		case EDeviceParameterMeereTOFGigE_ExtrinsicR22:
			bReturn = !SetExtrinsicR22(_ttof(strValue));
			break;
		case EDeviceParameterMeereTOFGigE_ExtrinsicR23:
			bReturn = !SetExtrinsicR23(_ttof(strValue));
			break;
		case EDeviceParameterMeereTOFGigE_ExtrinsicR31:
			bReturn = !SetExtrinsicR31(_ttof(strValue));
			break;
		case EDeviceParameterMeereTOFGigE_ExtrinsicR32:
			bReturn = !SetExtrinsicR32(_ttof(strValue));
			break;
		case EDeviceParameterMeereTOFGigE_ExtrinsicR33:
			bReturn = !SetExtrinsicR33(_ttof(strValue));
			break;
		case EDeviceParameterMeereTOFGigE_ExtrinsicTx:
			bReturn = !SetExtrinsicTx(_ttof(strValue));
			break;
		case EDeviceParameterMeereTOFGigE_ExtrinsicTy:
			bReturn = !SetExtrinsicTy(_ttof(strValue));
			break;
		case EDeviceParameterMeereTOFGigE_ExtrinsicTz:
			bReturn = !SetExtrinsicTz(_ttof(strValue));
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

CRavidImage * CDeviceMeereTOFGigE::Get3DXYZData()
{
	return m_p3DXYZCoordinates;
}

bool CDeviceMeereTOFGigE::GetInformation(ptrdiff_t x, ptrdiff_t y, float * pXCoordinate, float * pYCoordinate, float * pDistance)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(!m_p3DXYZCoordinates)
			break;

		if(!m_p3DXYZCoordinates->GetBuffer())
			break;

		if(!pXCoordinate || !pYCoordinate || !pDistance)
			break;

		if(x < 0 || y < 0)
			break;

		if(x >= m_nCanvasX || y >= m_nCanvasY)
			break;

		MakeInvalid(*pXCoordinate);
		*pYCoordinate = *pXCoordinate;
		*pDistance = *pXCoordinate;

		float* pDataBuffer = (float*)m_p3DXYZCoordinates->GetYOffsetTable()[y];
		pDataBuffer += (x * 3);

		if(pDataBuffer)
		{
			*pXCoordinate = *(pDataBuffer + 0);
			*pYCoordinate = *(pDataBuffer + 1);
			*pDistance = *(pDataBuffer + 2);
		}

		bReturn = true;

	}
	while(false);

	return bReturn;
}

void CDeviceMeereTOFGigE::DrawPixelInfo(int nDrawIndex)
{
	Lock();

	do
	{
		CRavidImageView* pView = GetImageView();
		if(!pView)
			break;

		CRavidImageViewLayer* pLayer = pView->GetLayer(nDrawIndex);
		if(!pLayer)
			break;


		pLayer->Clear();
		
		if(m_bDrawPoint)
		{
			pLayer->DrawShape(CRavidRect<double>(m_DrawPoint.x, m_DrawPoint.y, m_DrawPoint.x + 1, m_DrawPoint.y + 1), LIME, 3);

			float x = 0.f, y = 0.f, dis = 0.f;
			GetInformation(m_DrawPoint.x, m_DrawPoint.y, &x, &y, &dis);

			CString strUpdate;
			strUpdate.Format(_T("x : %.3f\ny : %.3f\nDist : %.3f"), x, y, dis);
			pLayer->DrawTextW(m_DrawPoint, strUpdate, LIME, BLACK, 13);
		}

		pView->Invalidate();
	}
	while(false);

	Unlock();
}

bool CDeviceMeereTOFGigE::DoesModuleExist()
{
	bool bReturn = false;

	CString strModuleName;

	do
	{
#ifndef _WIN64
#ifdef DEBUG
		strModuleName.Format(_T("MTF_API_d.dll"));
#else// DEBUG
		strModuleName.Format(_T("MTF_API.dll"));
#endif
#else
#ifdef DEBUG
		strModuleName.Format(_T("MTF_API[x64]_d.dll"));
#else// DEBUG
		strModuleName.Format(_T("MTF_API[x64].dll"));
#endif
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
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, CMultiLanguageManager::GetString(ELanguageParameter__s_d_s_toload_s), GetClassNameStr(), GetObjectID(), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), strModuleName);

	return bReturn;
}

UINT CDeviceMeereTOFGigE::CallbackFunction(LPVOID pParam)
{
	CDeviceMeereTOFGigE* pInstance = (CDeviceMeereTOFGigE*)pParam;

	if(pInstance)
	{
		unsigned short* pBuffer[TOTAL];
		
		MTF_API::mtfCameraSpacePoint* m_p3DXYZ = nullptr;

		int nWidth = pInstance->GetImageInfo()->GetSizeX();
		int nHeight = pInstance->GetImageInfo()->GetSizeY();

		bool bError = false;

		do 
		{
			for(int i = 0; i < TOTAL; i++)
				pBuffer[i] = (unsigned short*)malloc(sizeof(unsigned short) * nWidth * nHeight);

			m_p3DXYZ = new MTF_API::mtfCameraSpacePoint[nWidth * nHeight];;

			if(!m_p3DXYZ)
				break;

			while(1)
			{
				if(!pInstance->IsInitialized())
					break;

				if(MTF_API::mtfReadFromDevice(pInstance->m_hDevice, pBuffer, pInstance->m_pFrameInfo) != MTF_API::ERROR_NO)
					continue;

				if(!pBuffer[1])
					continue;

				if(!pInstance->IsLive() && pInstance->IsGrabAvailable())
					continue;

				pInstance->NextImageIndex();

				CRavidImage* pCurrentImage = pInstance->GetImageInfo();

				BYTE* pCurrentBuffer = pCurrentImage->GetBuffer();

				const int64_t i64Width = (int64_t)pCurrentImage->GetSizeX();
				const int64_t i64Height = (int64_t)pCurrentImage->GetSizeY();
				const int64_t i64ImageSizeByte = (int64_t)pCurrentImage->GetImageSizeByte();

				memcpy(pCurrentBuffer, pBuffer[1], i64ImageSizeByte);

				MTF_API::mtfDepthFrameToCameraSpace((unsigned short*)pCurrentBuffer, m_p3DXYZ);
				memcpy(pInstance->m_p3DXYZCoordinates->GetBuffer(), m_p3DXYZ, pInstance->m_p3DXYZCoordinates->GetImageSizeByte());

				pInstance->ConnectImage(false);

				CRavidImageView* pCurView = pInstance->GetImageView();
				if(pCurView)
				{
					pInstance->DrawPixelInfo(9);
					pCurView->Invalidate();
				}

				CEventHandlerManager::BroadcastOnAcquisition(pCurrentImage, pInstance->GetObjectID(), pInstance->GetImageView(), pInstance);

				if(!pInstance->IsGrabAvailable())
					pInstance->m_bIsGrabAvailable = true;

				Sleep(1);
			}
		} 
		while(false);

		for(int i = 0; i < TOTAL; i++)
		{
			if(pBuffer[i])
				delete[] pBuffer[i];
			pBuffer[i] = nullptr;
		}
		

		if(m_p3DXYZ)
			delete[] m_p3DXYZ;
	}

	return 0;
}

#endif