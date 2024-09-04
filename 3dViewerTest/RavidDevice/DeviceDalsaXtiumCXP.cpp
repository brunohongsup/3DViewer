#include "stdafx.h"

#include "DeviceDalsaXtiumCXP.h"

#ifdef USING_DEVICE

#include "../RavidCore/RavidImage.h"

#include "../RavidFramework/LogManager.h"
#include "../RavidFramework/LibraryManager.h"
#include "../RavidFramework/EventHandlerManager.h" 
#include "../RavidFramework/MultiLanguageManager.h"

#include "../RavidFramework/RavidImageView.h"

#include "../Libraries/Includes/DalsaXtiumMX4/SapClassBasic.h"

// sapclassbasic84.dll
#pragma comment(lib, COMMONLIB_PREFIX "DalsaXtiumMX4/SapClassBasic.lib")
#pragma comment(lib, COMMONLIB_PREFIX "DalsaXtiumMX4/corapi.lib")
#ifndef _WIN64
#pragma comment(lib, COMMONLIB_PREFIX "DalsaXtiumMX4/corppapi.lib")
#endif

using namespace Ravid;
using namespace Ravid::Device;
using namespace Ravid::Database;
using namespace Ravid::Framework;
using namespace Ravid::Algorithms;
using namespace Ravid::Miscellaneous;

IMPLEMENT_DYNAMIC(CDeviceDalsaXtiumCXP, CDeviceFrameGrabber)

BEGIN_MESSAGE_MAP(CDeviceDalsaXtiumCXP, CDeviceFrameGrabber)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static LPCTSTR g_lpszParamDalsaXtiumCXP[EDeviceParameterDalsaFrameGrabbers_Count] =
{
	_T("DeviceID"),
	_T("SubUnitID"),
	_T("CamFile Name"),
};

CDeviceDalsaXtiumCXP::CDeviceDalsaXtiumCXP()
{
}


CDeviceDalsaXtiumCXP::~CDeviceDalsaXtiumCXP()
{
}

EDeviceInitializeResult CDeviceDalsaXtiumCXP::Initialize()
{
	CString strMessage, strStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("DalsaXtiumCXP"));

		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);

		SetStatus(strStatus);

		return EDeviceInitializeResult_NotFoundApiError;
	}

	EDeviceInitializeResult eReturn = EDeviceInitializeResult_UnknownError;

	do
	{
		if(IsInitialized())
		{
			strStatus = CMultiLanguageManager::GetString(ELanguageParameter_Initialized);
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Thedevicehasbeenalreadyinitialized);
			eReturn = EDeviceInitializeResult_AlreadyInitializedError;
			break;
		}

		if(_ttoi(GetDeviceID()) < 0)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntsupportthe_s), _T("DevicdID"));
			eReturn = EDeviceInitializeResult_NotSupportedObjectID;
			break;
		}

		SapManager::SetDisplayStatusMode(SapManager::StatusCustom);

		if(!SapManager::Open())
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntopenthelibrary);
			eReturn = EDeviceInitializeResult_NotFoundLibraries;
			break;
		}

		int serverCount = SapManager::GetServerCount();

		if(!serverCount)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtofindthedevice);
			eReturn = EDeviceInitializeResult_NotFoundDeviceError;
			break;
		}

		bool bError = false;

		CString strDeviceID = GetDeviceID();
		CString strSubUnitID = GetSubUnitID();
		int nSubUnitID = _ttoi(strSubUnitID);

		CString strCamfile = _T("");
		if(GetCamfileName(&strCamfile))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("CamFileName"));
			eReturn = EDeviceInitializeResult_NotSupportedObjectID;
			break;
		}

		if(!strCamfile.GetLength())
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("CamFileName"));
			eReturn = EDeviceInitializeResult_NotSupportedObjectID;
			break;
		}

		int nPos = strCamfile.ReverseFind(_T('\\'));
		bool bNoFolder = nPos < 0;

		if(bNoFolder)
		{
			CString strFileFolder = _T("");
			GetCurrentDirectory(MAX_PATH, strFileFolder.GetBuffer(MAX_PATH));
			strFileFolder.ReleaseBuffer();

			strCamfile = strFileFolder + _T("\\") + strCamfile;
		}

		if(_taccess(strCamfile, 0))
		{
			FILE* fp = _tfopen(strCamfile, _T("wt"));

			if(fp)
				fclose(fp);
		}

		char serverName[30];
		bool bSerialMatching = false;

		for(int serverIndex = 0; serverIndex < serverCount; serverIndex++)
		{
			int nResourceCount = SapManager::GetResourceCount(serverIndex, SapManager::ResourceAcq);

			if(nResourceCount != 0)
			{
				SapManager::GetServerName(serverIndex, serverName, sizeof(serverName));

				if(!SapManager::IsResourceAvailable(serverIndex, SapManager::ResourceAcq, nSubUnitID))
					continue;

				if(!strDeviceID.CompareNoCase(CString(serverName)))
				{
					bSerialMatching = true;

					SapLocation loc(serverName, nSubUnitID);

					m_pDevice = new SapAcquisition(loc, (CStringA)strCamfile);

					if(!m_pDevice || !m_pDevice->Create())
					{
						bError = true;
						break;
					}

					break;
				}
			}

			if(bSerialMatching)
				break;
		}

		if(!bSerialMatching || !m_pDevice || bError)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntopenthedevice);
			eReturn = EDeviceInitializeResult_NotFoundDeviceError;
			break;
		}

		int nWidth = 0;
		int nHeight = 0;

		if(m_pDevice->IsParameterValid(CORACQ_PRM_CROP_WIDTH))
		{
			if(!m_pDevice->GetParameter(CORACQ_PRM_CROP_WIDTH, &nWidth))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Width"));
				eReturn = EDeviceInitializeResult_ReadOnDeviceError;
				break;
			}
		}

		if(m_pDevice->IsParameterValid(CORACQ_PRM_CROP_HEIGHT))
		{
			if(!m_pDevice->GetParameter(CORACQ_PRM_CROP_HEIGHT, &nHeight))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Height"));
				eReturn = EDeviceInitializeResult_ReadOnDeviceError;
				break;
			}
		}

		int nBpp = -1;

		if(m_pDevice->IsParameterValid(CORACQ_PRM_PIXEL_DEPTH))
		{
			if(!m_pDevice->GetParameter(CORACQ_PRM_PIXEL_DEPTH, &nBpp))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Pixel depth"));
				eReturn = EDeviceInitializeResult_ReadOnDeviceError;
				break;
			}
		}

		if(nBpp < 0)
			break;

		m_pBuffers = new SapBuffer(2, m_pDevice);

		if(!m_pBuffers || !m_pBuffers->Create())
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("device-buffer"));
			eReturn = EDeviceInitializeResult_NotCreateDeviceError;
			if(m_pBuffers)
				delete m_pBuffers;
			m_pBuffers = nullptr;

			bError = true;
			break;
		}

#ifdef _WIN64
		m_pXfer = new SapAcqToBuf(m_pDevice, m_pBuffers, CallbackFunction, this);
#else
		m_pXfer = new SapAcqToBuf(m_pDevice, m_pBuffers, (SapXferCallback)CallbackFunction, this);
#endif

		m_pXfer->GetPair(0)->SetFramesOnBoard(4096);
		m_pXfer->GetPair(0)->SetFramesPerCallback(2);
		
		if(!m_pXfer || !m_pXfer->Create())
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("acq-buffer"));
			eReturn = EDeviceInitializeResult_NotCreateDeviceError;
			if(m_pXfer)
				delete m_pXfer;
			m_pXfer = nullptr;

			bError = true;
			break;
		}

		m_pXfer->SetAutoEmpty(TRUE);

		bool bColor = true;

		int nPixelFormat = -1;

		if(m_pDevice->IsParameterValid(CORACQ_PRM_OUTPUT_FORMAT))
		{
			if(!m_pDevice->GetParameter(CORACQ_PRM_OUTPUT_FORMAT, &nPixelFormat))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Pixel Format"));
				eReturn = EDeviceInitializeResult_ReadOnDeviceError;
				break;
			}
		}

		if(!(nPixelFormat == CORACQ_VAL_OUTPUT_FORMAT_MONO8 || nPixelFormat == CORACQ_VAL_OUTPUT_FORMAT_MONO16 || nPixelFormat == CORACQ_VAL_OUTPUT_FORMAT_RGB888 || nPixelFormat == CORACQ_VAL_OUTPUT_FORMAT_RGB8888))
		{
			strMessage.Format(_T("Not support PixelFormat"));
			eReturn = EDeviceInitializeResult_CanNotApplyPixelFormat;
			break;
		}

		if(nPixelFormat == CORACQ_VAL_OUTPUT_FORMAT_MONO8 || nPixelFormat == CORACQ_VAL_OUTPUT_FORMAT_MONO16)
			bColor = false;
		
		int nChannel = bColor ? 3 : 1;

		if(nPixelFormat == CORACQ_VAL_OUTPUT_FORMAT_RGB8888)
			nChannel = 4;

		int nMaxValue = (1 << nBpp) - 1;

		CMultipleVariable mv;

		for(int i = 0; i < nChannel; ++i)
			mv.AddValue(nMaxValue);

		this->InitBuffer(nWidth, nHeight, mv, CRavidImage::MakeValueFormat(nChannel, nBpp));
		this->ConnectImage();

		m_bIsInitialized = true;

		strStatus = CMultiLanguageManager::GetString(ELanguageParameter_Initialized);

		strMessage = CMultiLanguageManager::GetString(ELanguageParameter_TheDevicehasbeensuccessfullyinitialized);

		eReturn = EDeviceInitializeResult_OK;
	}
	while(false);

	SetStatus(strStatus);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	if(eReturn != EDeviceInitializeResult_OK)
		Terminate();

	return eReturn;
}

EDeviceTerminateResult CDeviceDalsaXtiumCXP::Terminate()
{
	EDeviceTerminateResult eReturn = EDeviceTerminateResult_UnknownError;

	CString strMessage;

	SetStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("EuresysMulticam"));

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

		if(m_pXfer && *m_pXfer)
			m_pXfer->Destroy();

		if(m_pBuffers && *m_pBuffers)
			m_pBuffers->Destroy();

		if(m_pDevice && *m_pDevice)
			m_pDevice->Destroy();

		if(m_pXfer)
			delete m_pXfer;
		m_pXfer = nullptr;

		if(m_pBuffers)
			delete m_pBuffers;
		m_pBuffers = nullptr;

		if(m_pDevice)
			delete m_pDevice;
		m_pDevice = nullptr;

		strMessage = CMultiLanguageManager::GetString(ELanguageParameter_TheDevicehasbeensuccessfullyterminated);

		CEventHandlerManager::BroadcastOnDeviceTerminated(this);
	}
	while(false);

	this->ClearBuffer();

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceDalsaXtiumCXP::LoadSettings()
{
	bool bReturn = false;

	do
	{
		int nParam = EDeviceParameterDalsaFrameGrabbers_DeviceID;

		AddParameterFieldConfigurations(nParam, g_lpszParamDalsaXtiumCXP[nParam], _T("Xtium2-CL_CXP_"), EParameterFieldType_Edit, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(++nParam, g_lpszParamDalsaXtiumCXP[nParam], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(++nParam, g_lpszParamDalsaXtiumCXP[nParam], _T("DalsaXtiumCXP.ccf"), EParameterFieldType_Edit, nullptr, nullptr, 0);

		bReturn = true;
	}
	while(false);

	return bReturn & CDeviceBase::LoadSettings();
}

EDeviceGrabResult CDeviceDalsaXtiumCXP::Grab()
{
	EDeviceGrabResult eReturn = EDeviceGrabResult_UnknownError;

	CString strMessage;

	do
	{
		if(!IsInitialized())
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Ithasnotbeeninitializedyet);
			eReturn = EDeviceGrabResult_NotInitializedError;
			break;
		}

		if(!IsGrabAvailable())
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Ithasbeenlivingorgrabbing);
			eReturn = EDeviceGrabResult_AlreadyGrabError;
			break;
		}

		TryCatchBegin;
		m_pXfer->Snap();
		TryCatchException;
		TryCatchEnd;

		m_bIsGrabAvailable = false;

		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("Grab"));

		eReturn = EDeviceGrabResult_OK;
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceLiveResult CDeviceDalsaXtiumCXP::Live()
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

		TryCatchBegin;
		m_pXfer->Grab();
		TryCatchException;
		TryCatchEnd;

		m_bIsLive = true;

		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("Live"));

		eReturn = EDeviceLiveResult_OK;
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceStopResult CDeviceDalsaXtiumCXP::Stop()
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

		TryCatchBegin;
		if(m_pXfer->Freeze())
			m_pXfer->Abort();
		TryCatchException;
		TryCatchEnd;

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

EDeviceTriggerResult CDeviceDalsaXtiumCXP::Trigger()
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

		if(IsGrabAvailable() && !IsLive())
			break;

		TryCatchBegin;
		m_pDevice->SoftwareTrigger(SapAcquisition::SoftwareTriggerExt);
		TryCatchException;
		TryCatchEnd;

		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("Trigger"));

		eReturn = EDeviceTriggerResult_OK;
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

BOOL CDeviceDalsaXtiumCXP::GetParameter(int param, void * pValue)
{
	bool bReturn = false;

	TryCatchBegin
		bReturn = m_pDevice->GetParameter(param, pValue);
	TryCatchException;
	TryCatchEnd;

	return bReturn;
}

BOOL CDeviceDalsaXtiumCXP::SetParameter(int param, int value, BOOL updateNow)
{
	bool bReturn = false;

	TryCatchBegin
		bReturn = m_pDevice->SetParameter(param, value, updateNow);
	TryCatchException;
	TryCatchEnd;

	return bReturn;
}

BOOL CDeviceDalsaXtiumCXP::SetParameter(int param, void * pValue, BOOL updateNow)
{
	bool bReturn = false;

	TryCatchBegin
		bReturn = m_pDevice->SetParameter(param, pValue, updateNow);
	TryCatchException;
	TryCatchEnd;

	return bReturn;
}

bool CDeviceDalsaXtiumCXP::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = false;

	do
	{
		switch(nParam)
		{
		case EDeviceParameterDalsaFrameGrabbers_DeviceID:
			bReturn = !SetDeviceID(strValue);
			break;
		case EDeviceParameterDalsaFrameGrabbers_SubUnitID:
			bReturn = !SetSubUnitID(strValue);
			break;
		case EDeviceParameterDalsaFrameGrabbers_CamfileName:
			bReturn = !SetCamfileName(strValue);
			break;
		default:
			break;
		}
	}
	while(false);

	return bReturn;
}

void CDeviceDalsaXtiumCXP::CallbackFunction(SapXferCallbackInfo * pInfo)
{
	CDeviceDalsaXtiumCXP *pInstance = (CDeviceDalsaXtiumCXP *)pInfo->GetContext();

	do
	{
		if(pInfo->IsTrash())
			break;

		if(!pInstance)
			break;

		if(!pInfo->GetTransfer()->IsGrabbing() && pInstance->IsGrabAvailable())
			break;

		if(!pInstance->IsLive() && pInstance->IsGrabAvailable())
			break;

		BYTE* pImageBuffer = nullptr;

		SapBuffer* pBuf = pInstance->m_pBuffers;
		pBuf->GetAddress((void**)&pImageBuffer);

		int nWidth = pBuf->GetWidth();
		int nHeight = pBuf->GetHeight();
		int nPitch = pBuf->GetPitch();

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

		for(int nY = 0; nY < nHeight; ++nY)
			memcpy(ppCurrentOffsetY[nY], pImageBuffer + nY * nPitch, nPitch);

		pInstance->ConnectImage(false);

		CEventHandlerManager::BroadcastOnAcquisition(pCurrentImage, pInstance->GetObjectID(), pInstance->GetImageView(), pInstance);

		pInstance->m_bIsGrabAvailable = true;
	}
	while(false);
}

bool CDeviceDalsaXtiumCXP::DoesModuleExist()
{
	bool bReturn = false;

	CString strModuleName;

	do
	{
		strModuleName.Format(_T("SapClassBasic84.dll"));

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

		strModuleName.Format(_T("corapi.dll"));

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

		strModuleName.Format(_T("CorUtility.dll"));

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

		strModuleName.Format(_T("CorLog.dll"));

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

#endif