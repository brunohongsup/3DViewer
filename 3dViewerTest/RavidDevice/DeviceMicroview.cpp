#include "stdafx.h"

#include "DeviceMicroview.h"

#ifdef USING_DEVICE

#include "../RavidCore/RavidImage.h"

#include "../RavidFramework/RavidImageView.h"

#include "../RavidFramework/LibraryManager.h"
#include "../RavidFramework/MultiLanguageManager.h"
#include "../RavidFramework/LogManager.h"
#include "../RavidFramework/EventHandlerManager.h"

#include "../Libraries/Includes/Microview/PvSystem.h"
#include "../Libraries/Includes/Microview/PvPipeline.h"
#include "../Libraries/Includes/Microview/pvdevicegev.h"
#include "../Libraries/Includes/Microview/PvStreamGEV.h"

// pvsystem.dll
// pvbase.dll
// pvbuffer.dll
// pvstream.dll
// pvgenicam.dll
// pvdevice.dll
#ifndef _WIN64
#pragma comment(lib, COMMONLIB_PREFIX "Microview/PvSystem.lib")
#pragma comment(lib, COMMONLIB_PREFIX "Microview/PvBase.lib")
#pragma comment(lib, COMMONLIB_PREFIX "Microview/PvBuffer.lib")
#pragma comment(lib, COMMONLIB_PREFIX "Microview/PvStream.lib")
#pragma comment(lib, COMMONLIB_PREFIX "Microview/PvGenICam.lib")
#pragma comment(lib, COMMONLIB_PREFIX "Microview/PvDevice.lib")
#else
// pvsystem64.dll
// pvbase64.dll
// pvbuffer64.dll
// pvstream64.dll
// pvgenicam64.dll
// pvdevice64.dll
#pragma comment(lib, COMMONLIB_PREFIX "Microview/PvSystem64.lib")
#pragma comment(lib, COMMONLIB_PREFIX "Microview/PvBase64.lib")
#pragma comment(lib, COMMONLIB_PREFIX "Microview/PvBuffer64.lib")
#pragma comment(lib, COMMONLIB_PREFIX "Microview/PvStream64.lib")
#pragma comment(lib, COMMONLIB_PREFIX "Microview/PvGenICam64.lib")
#pragma comment(lib, COMMONLIB_PREFIX "Microview/PvDevice64.lib")
#endif

using namespace Ravid;
using namespace Ravid::Device;
using namespace Ravid::Database;
using namespace Ravid::Framework;
using namespace Ravid::Algorithms;
using namespace Ravid::Miscellaneous;

#define BUFFER_COUNT ( 16 )

IMPLEMENT_DYNAMIC(CDeviceMicroview, CDeviceCamera)

BEGIN_MESSAGE_MAP(CDeviceMicroview, CDeviceCamera)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static LPCTSTR g_lpszParamMicroview[EDeviceParameterMicroview_Count] =
{
	_T("DeviceID"),
};

CDeviceMicroview::CDeviceMicroview()
{
}

CDeviceMicroview::~CDeviceMicroview()
{
}

EDeviceInitializeResult CDeviceMicroview::Initialize()
{
	EDeviceInitializeResult eReturn = EDeviceInitializeResult_UnknownError;

	CString strMessage, strStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("Microview"));

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

		PvSystem aSystem;

		const PvDeviceInfo* pDeviceInfo = nullptr;

		if(!aSystem.Find().IsOK())
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Device find"));
			eReturn = EDeviceInitializeResult_NotFoundDeviceInfo;
			break;
		}

		uint32_t lInterFaceCount = aSystem.GetInterfaceCount();

		for(uint32_t i = 0; i < lInterFaceCount; ++i)
		{
			const PvInterface* pInterFace = aSystem.GetInterface(i);

			uint32_t lDeviceCount = pInterFace->GetDeviceCount();

			for(uint32_t j = 0; j < lDeviceCount; ++j)
			{
				pDeviceInfo = pInterFace->GetDeviceInfo(j);

				PvString strTemp = pDeviceInfo->GetSerialNumber().GetAscii();

				if(!strcmp(strTemp, CStringA(GetDeviceID())))
				{
					bFindCamera = true;
					break;
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

		PvResult lResult;

		m_pCamera = PvDevice::CreateAndConnect(pDeviceInfo, &lResult);

		if(!lResult || !m_pCamera || !m_pCamera->IsConnected())
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

		lResult = NULL;

		m_pStream = PvStream::CreateAndOpen(pDeviceInfo, &lResult);

		if(!lResult || !m_pStream || !m_pStream->IsOpen())
		{
			if(m_pStream)
			{
				delete m_pStream;
				m_pStream = nullptr;
			}

			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Stream"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		PvDeviceGEV* lDeviceGEV = dynamic_cast<PvDeviceGEV *>(m_pCamera);
		if(lDeviceGEV != NULL)
		{
			PvStreamGEV *lStreamGEV = static_cast<PvStreamGEV *>(m_pStream);

			// Negotiate packet size
			lDeviceGEV->NegotiatePacketSize();

			// Configure device streaming destination
			if(!lDeviceGEV->SetStreamDestination(lStreamGEV->GetLocalIPAddress(), lStreamGEV->GetLocalPort()).IsOK())
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("StreamPort"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}
		}

		m_pPipeline = new PvPipeline(m_pStream);

		if(!m_pPipeline)
		{
			if(m_pPipeline)
			{
				delete m_pPipeline;
				m_pPipeline = nullptr;
			}

			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Pipeline"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		uint32_t lSize = m_pCamera->GetPayloadSize();

		// Set the Buffer count and the Buffer size
		m_pPipeline->SetBufferCount(BUFFER_COUNT);
		m_pPipeline->SetBufferSize(lSize);

		PvGenParameterArray *pDeviceParams = m_pCamera->GetParameters();

		if(!pDeviceParams)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("ParameterSetting"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		int64_t nWidth = 0;
		int64_t nHeight = 0;

		if(!pDeviceParams->GetIntegerValue("Width", nWidth).IsOK())
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Width"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		if(!pDeviceParams->GetIntegerValue("Height", nHeight).IsOK())
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Height"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		PvString lPixelFormat;

		if(!pDeviceParams->GetEnumValue("PixelFormat", lPixelFormat).IsOK())
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("PixelFormat"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		CString strPixelFormat = lPixelFormat;

		int nBpp = 8;
		int nAlignByte = 4;
		int nWidthStep = nWidth;

		bool bColor = strPixelFormat.Left(4).CompareNoCase(_T("Mono"));

		if(bColor)
			break;

		strPixelFormat.Delete(0, 4);

		switch(strPixelFormat[0])
		{
		case _T('1'):
			{
				switch(strPixelFormat[1])
				{
				case _T('2'):
					{
						nBpp = 12;
						nWidthStep *= 2;
					}
					break;

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

		if(strPixelFormat.GetLength() > 2)
		{
			strMessage.Format(_T("Not support PixelFormat"));
			eReturn = EDeviceInitializeResult_CanNotApplyPixelFormat;
			break;
		}

		int nMaxValue = (1 << nBpp) - 1;

		CMultipleVariable mv;

		for(int i = 0; i < 1; ++i)
			mv.AddValue(nMaxValue);

		this->InitBuffer(nWidth, nHeight, mv, CRavidImage::MakeValueFormat(1, nBpp), nWidthStep, nAlignByte);
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

EDeviceTerminateResult CDeviceMicroview::Terminate()
{
	EDeviceTerminateResult eReturn = EDeviceTerminateResult_UnknownError;

	CString strMessage;

	SetStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("Microview"));

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

		if(m_pPipeline)
		{
			if(m_pPipeline->IsStarted())
				m_pPipeline->Stop();

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

bool CDeviceMicroview::LoadSettings()
{
	bool bReturn = false;

	do
	{
		AddParameterFieldConfigurations(EDeviceParameterMicroview_DeviceID, g_lpszParamMicroview[EDeviceParameterMicroview_DeviceID], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 0);

		bReturn = true;
	}
	while(false);

	return bReturn & __super::LoadSettings();
}

EDeviceGrabResult CDeviceMicroview::Grab()
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

		if(!m_pCamera)
			break;

		if(!m_pPipeline)
			break;

		PvGenParameterArray *lDeviceParams = m_pCamera->GetParameters();

		if(!lDeviceParams)
			break;

		PvGenCommand *lStart = dynamic_cast<PvGenCommand *>(lDeviceParams->Get("AcquisitionStart"));

		if(!lStart)
			break;

		TryCatchBegin;
		m_pPipeline->Start();
		m_pCamera->StreamEnable();
		lStart->Execute();
		TryCatchException;
		TryCatchEnd;

		m_bIsGrabAvailable = false;

		m_pLiveThread = AfxBeginThread(CDeviceMicroview::CallbackFunction, this, 0, 0U, CREATE_SUSPENDED);

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

EDeviceLiveResult CDeviceMicroview::Live()
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

		if(!m_pCamera)
			break;

		if(!m_pPipeline)
			break;

		PvGenParameterArray *lDeviceParams = m_pCamera->GetParameters();

		if(!lDeviceParams)
			break;

		PvGenCommand *lStart = dynamic_cast<PvGenCommand *>(lDeviceParams->Get("AcquisitionStart"));

		if(!lStart)
			break;

		TryCatchBegin;
		m_pPipeline->Start();
		m_pCamera->StreamEnable();
		lStart->Execute();
		TryCatchException;
		TryCatchEnd;

		m_bIsLive = true;

		m_pLiveThread = AfxBeginThread(CDeviceMicroview::CallbackFunction, this, 0, 0U, CREATE_SUSPENDED);

		if(!m_pLiveThread)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtocreatethread);
			eReturn = EDeviceLiveResult_CreateThreadError;
			break;
		}

		m_pLiveThread->ResumeThread();

		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("Grab"));

		eReturn = EDeviceLiveResult_OK;

	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceStopResult CDeviceMicroview::Stop()
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

		if(!m_pCamera)
			break;

		if(!m_pPipeline)
			break;

		PvGenParameterArray *lDeviceParams = m_pCamera->GetParameters();

		if(!lDeviceParams)
			break;

		m_bIsLive = false;
		m_bIsGrabAvailable = true;

		PvGenCommand *lStop = dynamic_cast<PvGenCommand *>(lDeviceParams->Get("AcquisitionStop"));

		if(!lStop)
			break;

		TryCatchBegin;
		lStop->Execute();
		m_pCamera->StreamDisable();

		if(m_pPipeline->IsStarted())
			m_pPipeline->Stop();
		TryCatchException;
		TryCatchEnd;

		if(m_pLiveThread)
		{
			WaitForSingleObject(m_pLiveThread->m_hThread, 1000);
			m_pLiveThread = nullptr;
		}

		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("Stop"));

		eReturn = EDeviceStopResult_OK;
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceTriggerResult CDeviceMicroview::Trigger()
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

		PvGenParameterArray* pDeviceParams = m_pCamera->GetParameters();

		if(!pDeviceParams)
			break;

		if(!pDeviceParams->ExecuteCommand("TriggerSoftware").IsOK())
			break;

		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("Trigger"));

		eReturn = EDeviceTriggerResult_OK;
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceMicroview::DoesModuleExist()
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

	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, CMultiLanguageManager::GetString(ELanguageParameter__s_d_s_toload_s), GetClassNameStr(), GetObjectID(), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), strModuleName);

	return bReturn;
}

UINT CDeviceMicroview::CallbackFunction(LPVOID pParam)
{
	CDeviceMicroview* pInstance = (CDeviceMicroview*)pParam;

	unsigned long long ullTimes = 0xffffff;

	if(pInstance)
	{
		do
		{
			if(!pInstance->m_pCamera)
				break;

			if(!pInstance->m_pStream)
				break;

			if(!pInstance->m_pPipeline)
				break;

			CRavidImageView* pCurView = pInstance->GetImageView();

			do
			{
				if(!pInstance->m_pStream->IsOpen())
					break;

				if(!pInstance->m_pPipeline->IsStarted())
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

				PvBuffer *pBuffer = NULL;
				PvResult lOperationResult;

				PvResult lResult = pInstance->m_pPipeline->RetrieveNextBuffer(&pBuffer, ullTimes, &lOperationResult);

				if(!lResult.IsOK())
					break;

				if(!lOperationResult.IsOK())
					break;

				if(!pBuffer)
					break;

				PvImage* pImage = nullptr;

				do
				{
					pImage = pBuffer->GetImage();
					if(!pImage)
						break;

					size_t szBufferSize = pImage->GetImageSize();
					if(szBufferSize <= 0)
						break;

					memcpy(pCurrentBuffer, pBuffer->GetDataPointer(), szBufferSize);

					pInstance->ConnectImage(false);

					CEventHandlerManager::BroadcastOnAcquisition(pCurrentImage, pInstance->GetObjectID(), pInstance->GetImageView(), pInstance);
				}
				while(false);

				pInstance->m_pPipeline->ReleaseBuffer(pBuffer);

			}
			while(pInstance->IsInitialized() && pInstance->IsLive());

			PvGenParameterArray *lDeviceParams = pInstance->m_pCamera->GetParameters();

			if(lDeviceParams)
			{
				PvGenCommand *lStop = dynamic_cast<PvGenCommand *>(lDeviceParams->Get("AcquisitionStop"));

				if(lStop)
					lStop->Execute();
			}

			pInstance->m_pCamera->StreamDisable();

			if(pInstance->m_pPipeline->IsStarted())
				pInstance->m_pPipeline->Stop();

		}
		while(false);

		pInstance->m_bIsLive = false;
		pInstance->m_bIsGrabAvailable = true;
	}

	return 0;
}

bool CDeviceMicroview::GetIntegerValue(CString & strName, int64_t & aValue)
{
	bool bReturn = false;

	do
	{
		if(!m_pCamera)
			break;

		PvGenParameterArray *pDeviceParams = m_pCamera->GetParameters();

		if(!pDeviceParams)
			break;

		PvString lName(strName);

		if(!pDeviceParams->GetIntegerValue(lName, aValue).IsOK())
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceMicroview::SetIntegerValue(CString & strName, int64_t aValue)
{
	bool bReturn = false;

	do
	{
		if(!m_pCamera)
			break;

		PvGenParameterArray *pDeviceParams = m_pCamera->GetParameters();

		if(!pDeviceParams)
			break;

		PvString lName(strName);

		if(!pDeviceParams->SetIntegerValue(lName, aValue).IsOK())
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceMicroview::GetFloatValue(CString & strName, double & aValue)
{
	bool bReturn = false;

	do
	{
		if(!m_pCamera)
			break;

		PvGenParameterArray *pDeviceParams = m_pCamera->GetParameters();

		if(!pDeviceParams)
			break;

		PvString lName(strName);

		if(!pDeviceParams->GetFloatValue(lName, aValue).IsOK())
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceMicroview::SetFloatValue(CString & strName, double aValue)
{
	bool bReturn = false;

	do
	{
		if(!m_pCamera)
			break;

		PvGenParameterArray *pDeviceParams = m_pCamera->GetParameters();

		if(!pDeviceParams)
			break;

		PvString lName(strName);

		if(!pDeviceParams->SetFloatValue(lName, aValue).IsOK())
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceMicroview::GetEnumValue(CString & strName, CString & strValue)
{
	bool bReturn = false;

	do
	{
		if(!m_pCamera)
			break;

		PvGenParameterArray *pDeviceParams = m_pCamera->GetParameters();

		if(!pDeviceParams)
			break;

		PvString lName(strName);
		PvString lValue;

		if(!pDeviceParams->GetEnumValue(lName, lValue).IsOK())
			break;

		strValue = lValue.GetAscii();

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceMicroview::GetEnumValue(CString & strName, int64_t & aValue)
{
	bool bReturn = false;

	do
	{
		if(!m_pCamera)
			break;

		PvGenParameterArray *pDeviceParams = m_pCamera->GetParameters();

		if(!pDeviceParams)
			break;

		PvString lName(strName);

		if(!pDeviceParams->GetEnumValue(lName, aValue).IsOK())
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceMicroview::SetEnumValue(CString & strName, CString & aValue)
{
	bool bReturn = false;

	do
	{
		if(!m_pCamera)
			break;

		PvGenParameterArray *pDeviceParams = m_pCamera->GetParameters();

		if(!pDeviceParams)
			break;

		PvString lName(strName);
		PvString lValue(aValue);

		if(!pDeviceParams->SetEnumValue(lName, lValue).IsOK())
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceMicroview::SetEnumValue(CString & strName, int64_t aValue)
{
	bool bReturn = false;

	do
	{
		if(!m_pCamera)
			break;

		PvGenParameterArray *pDeviceParams = m_pCamera->GetParameters();

		if(!pDeviceParams)
			break;

		PvString lName(strName);

		if(!pDeviceParams->SetEnumValue(lName, aValue).IsOK())
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceMicroview::GetBooleanValue(CString & strName, bool & aValue)
{
	bool bReturn = false;

	do
	{
		if(!m_pCamera)
			break;

		PvGenParameterArray *pDeviceParams = m_pCamera->GetParameters();

		if(!pDeviceParams)
			break;

		PvString lName(strName);

		if(!pDeviceParams->GetBooleanValue(lName, aValue).IsOK())
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceMicroview::SetBooleanValue(CString & strName, bool aValue)
{
	bool bReturn = false;

	do
	{
		if(!m_pCamera)
			break;

		PvGenParameterArray *pDeviceParams = m_pCamera->GetParameters();

		if(!pDeviceParams)
			break;

		PvString lName(strName);

		if(!pDeviceParams->SetBooleanValue(lName, aValue).IsOK())
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceMicroview::GetStringValue(CString & strName, CString & aValue)
{
	bool bReturn = false;

	do
	{
		if(!m_pCamera)
			break;

		PvGenParameterArray *pDeviceParams = m_pCamera->GetParameters();

		if(!pDeviceParams)
			break;

		PvString lName(strName);
		PvString lValue;

		if(!pDeviceParams->GetStringValue(lName, lValue).IsOK())
			break;

		aValue = lValue.GetAscii();

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceMicroview::SetStringValue(CString & strName, CString & aValue)
{
	bool bReturn = false;

	do
	{
		if(!m_pCamera)
			break;

		PvGenParameterArray *pDeviceParams = m_pCamera->GetParameters();

		if(!pDeviceParams)
			break;

		PvString lName(strName);
		PvString lValue(aValue);

		if(!pDeviceParams->SetStringValue(lName, lValue).IsOK())
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceMicroview::ExecuteCommand(CString & strName)
{
	bool bReturn = false;

	do
	{
		if(!m_pCamera)
			break;

		PvGenParameterArray *pDeviceParams = m_pCamera->GetParameters();

		if(!pDeviceParams)
			break;

		PvString lName(strName);

		if(!pDeviceParams->ExecuteCommand(lName).IsOK())
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

#endif