#include "stdafx.h"

#include "DeviceEuresysCoaxlinkHX.h"

#ifdef USING_DEVICE

#include "../RavidCore/RavidImage.h"
#include "../RavidCore/OperatingSystemInfo.h"

#include "../RavidFramework/LogManager.h"
#include "../RavidFramework/DeviceManager.h"
#include "../RavidFramework/EventHandlerManager.h" 
#include "../RavidFramework/MultiLanguageManager.h"

#include "../RavidFramework/RavidImageView.h"

#include "../Libraries/Includes/Euresys/EGrabber.h"
#include "../Libraries/Includes/Euresys/FormatConverter.h"

using namespace Ravid;
using namespace Ravid::Device;
using namespace Ravid::Database;
using namespace Ravid::Framework;
using namespace Ravid::Algorithms;
using namespace Ravid::Miscellaneous;

using namespace Euresys;
using namespace GenTL;

IMPLEMENT_DYNAMIC(CDeviceEuresysCoaxlinkHX, CDeviceFrameGrabber)

BEGIN_MESSAGE_MAP(CDeviceEuresysCoaxlinkHX, CDeviceFrameGrabber)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

class Ravid::Device::MyGrabber : public EGrabber<>
{
public:
	MyGrabber(EGenTL &gentl, int nCardID, int nDeviceID, CStringA strScript, int nBufferCount = 4) : EGrabber<>(gentl, nCardID, nDeviceID)
	{
		runScript(strScript.GetBuffer());
		enableEvent<CicData>();
		reallocBuffers(nBufferCount);
	}
};

static LPCTSTR g_lpszParamCoaxlink[EDeviceParameterCoaxlink_Count] =
{
	_T("DeviceID"),
	_T("SubUnitID"),
	_T("Euresys Folder Path"),
	_T("CamFile Name"),
	_T("Buffer Count"),
};

static LPCTSTR g_lpszCoaxlinkInitializeType[EDeviceEGrabberInitializeType_Count] =
{
	_T("CamParam"),
	_T("RavidParam"),
};

static LPCTSTR g_lpszCoaxlinkHXPixelFormat[EDeviceEGrabberPixelFormat_Count] =
{
	_T("Mono8"),
	_T("Mono10"),
};

CDeviceEuresysCoaxlinkHX::CDeviceEuresysCoaxlinkHX()
{
}

CDeviceEuresysCoaxlinkHX::~CDeviceEuresysCoaxlinkHX()
{
}

EDeviceInitializeResult CDeviceEuresysCoaxlinkHX::Initialize()
{
	CString strMessage, strStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	COperatingSystemInfo osInfo;
	if(osInfo.IsX64OperatingSystem() && !osInfo.IsX64Process())
	{
		strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Terminated);

		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);

		SetStatus(strStatus);

		return EDeviceInitializeResult_NotSupportOS;
	}

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("EuresysEGrabber"));

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
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntsupportthe_s), _T("DeviceID"));
			eReturn = EDeviceInitializeResult_NotSupportedObjectID;
			break;
		}

		int nDeviceID = _ttoi(GetDeviceID());
		int nSubUnitID = _ttoi(GetSubUnitID());

		bool bNeedInitialize = true;
		bool bCreated = false;

		for(size_t i = 0; i < CDeviceManager::GetDeviceCount(); ++i)
		{
			CDeviceEuresysCoaxlinkHX* pDevice = dynamic_cast<CDeviceEuresysCoaxlinkHX*>(CDeviceManager::GetDeviceByIndex(i));

			if(!pDevice)
				continue;

			if(pDevice == this)
				continue;

			if(!pDevice->IsInitialized())
				continue;

			m_pTL = pDevice->GetDevice();
			bNeedInitialize = false;
			bCreated = true;

			break;
		}

		if(bNeedInitialize)
		{
			TryCatchBegin;
			m_pTL = new EGenTL();
			bCreated = true;
			TryCatchException;
			m_pTL = nullptr;
			TryCatchEnd;
		}

		if(!bCreated || !m_pTL)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtocreateport));
			eReturn = EDeviceInitializeResult_NotCreateDeviceError;
			break;
		}

		bCreated = false;

		CString strCamfile;
		TryCatchBegin;
		if(GetCamfileName(&strCamfile))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("CamFileName"));
			eReturn = EDeviceInitializeResult_NotSupportedObjectID;
			break;
		}
		TryCatchException;
		TryCatchEnd;

		if(!strCamfile.GetLength())
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("CamFileName"));
			eReturn = EDeviceInitializeResult_NotSupportedObjectID;
			break;
		}

		TryCatchBegin;
		if(_taccess(strCamfile, 0))
		{
			FILE* fp = _tfopen(strCamfile, _T("wt"));

			if(fp)
			{
				fputws(_T("for (var grabber of grabbers)\n{\n}\n"), fp);
				fclose(fp);
			}
		}

		int nBufferCount = 4;
		GetBufferCount(&nBufferCount);

		m_pEGrabber = new MyGrabber(*m_pTL, nDeviceID, nSubUnitID, CStringA(strCamfile), nBufferCount);
		bCreated = true;
		TryCatchException;
		m_pEGrabber = nullptr;
		TryCatchEnd;

		if(!bCreated || !m_pEGrabber)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtocreateport));
			eReturn = EDeviceInitializeResult_NotCreateDeviceError;
			break;
		}

		//라이브 도중 카메라 종료되거나 하면 스타트했다가 스톱해야 멈춤
		TryCatchBegin;
		m_pEGrabber->start();
		TryCatchException;
		TryCatchEnd;

		TryCatchBegin;
		m_pEGrabber->stop();
		TryCatchException;
		TryCatchEnd;

		int nWidth = 0;
		int nHeight = 0;

		int nParam = 0;
		double dblParam = 0.f;
		std::string szParam;
		CString strParam = _T("");
		int nPixelFormat = 0;

		TryCatchBegin;
		nWidth = (int)m_pEGrabber->getWidth();
		TryCatchException;
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Width"));
		eReturn = EDeviceInitializeResult_ReadOnDeviceError;
		break;
		TryCatchEnd;

		TryCatchBegin;
		nHeight = (int)m_pEGrabber->getHeight();
		TryCatchException;
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Height"));
		eReturn = EDeviceInitializeResult_ReadOnDeviceError;
		break;
		TryCatchEnd;

		TryCatchBegin;
		szParam = m_pEGrabber->getString<RemoteModule>("PixelFormat");
		strParam = szParam.c_str();

		for(int i = 0; i < EDeviceEGrabberPixelFormat_Count; ++i)
		{
			if(strParam.CompareNoCase(g_lpszCoaxlinkHXPixelFormat[i]))
				continue;

			nPixelFormat = i;

			break;
		}
		TryCatchException;
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("PixelFormat"));
		eReturn = EDeviceInitializeResult_ReadOnDeviceError;
		break;
		TryCatchEnd;

		if(eReturn == EDeviceInitializeResult_WriteToDatabaseError)
			break;

		CRavidImage::EValueFormat eVF = CRavidImage::EValueFormat_1C_U8;

		int nBpp = 8;
		int nChannel = 1;
		int nAlignByte = 4;
		int nWidthStep = nWidth;

		switch(nPixelFormat)
		{
		case EDeviceEGrabberPixelFormat_Mono8:
			{
				eVF = CRavidImage::EValueFormat_1C_U8;
				nBpp = 8;
				nChannel = 1;
				nWidthStep = nWidth;
			}
			break;
		case EDeviceEGrabberPixelFormat_Mono10:
			{
				eVF = CRavidImage::EValueFormat_1C_U10;
				nBpp = 10;
				nChannel = 1;
				nWidthStep *= 2;
			}
			break;
		}

		if((nWidthStep % 4))
			nWidthStep += (nAlignByte - (nWidthStep % nAlignByte));

		int nMaxValue = (1 << nBpp) - 1;

		CMultipleVariable mv;
		for(int i = 0; i < nChannel; ++i)
			mv.AddValue(nMaxValue);
		
		this->InitBuffer(nWidth, nHeight, mv, eVF);
		this->ConnectImage();

		m_bIsInitialized = true;

		strMessage = CMultiLanguageManager::GetString(ELanguageParameter_TheDevicehasbeensuccessfullyinitialized);

		strMessage = CMultiLanguageManager::GetString(ELanguageParameter_TheDevicehasbeensuccessfullyinitialized);

		eReturn = EDeviceInitializeResult_OK;

		CEventHandlerManager::BroadcastOnDeviceInitialized(this);
	}
	while(false);

	SetStatus(strStatus);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	if(eReturn != EDeviceInitializeResult_OK && eReturn != EDeviceInitializeResult_AlreadyInitializedError)
		Terminate();

	return eReturn;
}
EDeviceTerminateResult CDeviceEuresysCoaxlinkHX::Terminate()
{
	EDeviceTerminateResult eReturn = EDeviceTerminateResult_UnknownError;

	CString strMessage;

	SetStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	COperatingSystemInfo osInfo;
	if(osInfo.IsX64OperatingSystem() && !osInfo.IsX64Process())
	{
		strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Terminated);

		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);

		return EDeviceTerminateResult_NotSupportOS;
	}

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("EuresysEGrabber"));

		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);

		return EDeviceTerminateResult_NotFoundApiError;
	}

	do
	{
		if(!IsGrabAvailable())
			Stop();

		bool bNeedInitialize = true;
		int nDeviceID = _ttoi(GetDeviceID());

		for(size_t i = 0; i < CDeviceManager::GetDeviceCount(); ++i)
		{
			CDeviceEuresysCoaxlinkHX* pDevice = dynamic_cast<CDeviceEuresysCoaxlinkHX*>(CDeviceManager::GetDeviceByIndex(i));

			if(!pDevice)
				continue;

			if(pDevice == this)
				continue;

			if(!pDevice->IsInitialized())
				continue;

			if(_ttoi(pDevice->GetDeviceID()) == nDeviceID)
				continue;

			bNeedInitialize = false;

			break;
		}

		if(m_pEGrabber)
		{
			delete m_pEGrabber;
			m_pEGrabber = nullptr;
		}

		if(bNeedInitialize && m_pTL)
			delete m_pTL;

		m_pTL = nullptr;

		m_bIsInitialized = false;
		m_bIsGrabAvailable = true;

		strMessage = CMultiLanguageManager::GetString(ELanguageParameter_TheDevicehasbeensuccessfullyterminated);

		CEventHandlerManager::BroadcastOnDeviceTerminated(this);
	}
	while(false);

	this->ClearBuffer();

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceEuresysCoaxlinkHX::LoadSettings()
{
	bool bReturn = false;

	do
	{
		int nParam = EDeviceParameterCoaxlink_DeviceID;

		AddParameterFieldConfigurations(EDeviceParameterCoaxlink_DeviceID, g_lpszParamCoaxlink[EDeviceParameterCoaxlink_DeviceID], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterCoaxlink_SubUnitID, g_lpszParamCoaxlink[EDeviceParameterCoaxlink_SubUnitID], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterCoaxlink_EuresysFolderPath, g_lpszParamCoaxlink[EDeviceParameterCoaxlink_EuresysFolderPath], _T("C:\\Program Files\\Euresys\\Coaxlink"), EParameterFieldType_Edit, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterCoaxlink_CamfileName, g_lpszParamCoaxlink[EDeviceParameterCoaxlink_CamfileName], _T("EGrabber.js"), EParameterFieldType_Edit, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterCoaxlink_BufferCount, g_lpszParamCoaxlink[EDeviceParameterCoaxlink_BufferCount], _T("4"), EParameterFieldType_Edit, nullptr, nullptr, 0);

		bReturn = true;
	}
	while(false);

	return bReturn & CDeviceBase::LoadSettings();
}

EDeviceGrabResult CDeviceEuresysCoaxlinkHX::Grab()
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

		m_bIsGrabAvailable = false;

		TryCatchBegin;
		m_pEGrabber->start();
		TryCatchException;
		TryCatchEnd;

		m_pLiveThread = AfxBeginThread(CDeviceEuresysCoaxlinkHX::CallbackFunction, this, 0, 0U, CREATE_SUSPENDED);

		if(m_pLiveThread)
		{
			m_pLiveThread->ResumeThread();
			m_bIsGrabAvailable = false;
		}

		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("Grab"));

		eReturn = EDeviceGrabResult_OK;
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}
EDeviceLiveResult CDeviceEuresysCoaxlinkHX::Live()
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

		if(!IsGrabAvailable())
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Ithasbeenlivingorgrabbing);
			eReturn = EDeviceLiveResult_AlreadyGrabError;
			break;
		}

		m_bIsLive = true;

		TryCatchBegin;
		m_pEGrabber->start();
		TryCatchException;
		TryCatchEnd;

		m_pLiveThread = AfxBeginThread(CDeviceEuresysCoaxlinkHX::CallbackFunction, this, 0, 0U, CREATE_SUSPENDED);

		if(m_pLiveThread)
		{
			m_pLiveThread->ResumeThread();
			m_bIsGrabAvailable = false;
		}

		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("Live"));

		eReturn = EDeviceLiveResult_OK;
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}
EDeviceStopResult CDeviceEuresysCoaxlinkHX::Stop()
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

		m_bIsGrabAvailable = true;
		m_bIsLive = false;

		TryCatchBegin;
		m_pEGrabber->stop();
		TryCatchException;
		TryCatchEnd;

		TryCatchBegin;
		m_pEGrabber->resetBufferQueue();
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

EDeviceTriggerResult CDeviceEuresysCoaxlinkHX::Trigger()
{
	EDeviceTriggerResult eReturn = EDeviceTriggerResult_UnknownError;

	CString strMessage;

	do
	{
		if(!IsInitialized())
		{
			strMessage.Format(_T("Failed to initialize the device."));

			eReturn = EDeviceTriggerResult_NotInitializedError;

			break;
		}

		if(IsGrabAvailable() || IsLive())
			break;

		TryCatchBegin;
		const std::string str("TriggerSoftware");
		m_pEGrabber->execute<RemoteModule>(str);
		TryCatchException;
		TryCatchEnd;

		strMessage.Format(_T("Succeeded to execute command 'Trigger'"));

		eReturn = EDeviceTriggerResult_OK;
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysCoaxlinkHX::GetEuresysPath(CString * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = GetParamValue(EDeviceParameterCoaxlink_EuresysFolderPath);

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}
EEuresysSetFunction CDeviceEuresysCoaxlinkHX::SetEuresysPath(CString strParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterCoaxlink eSaveID = EDeviceParameterCoaxlink_EuresysFolderPath;

	CString strPreValue = GetParamValue(eSaveID);

	do
	{
		if(IsInitialized())
		{
			eReturn = EEuresysSetFunction_AlreadyInitializedError;
			break;
		}

		if(!SetParamValue(eSaveID, strParam))
		{
			eReturn = EEuresysSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EEuresysSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEuresysSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCoaxlink[eSaveID], strPreValue, strParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysCoaxlinkHX::GetCamfileName(CString * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = GetParamValue(EDeviceParameterCoaxlink_CamfileName);

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}
EEuresysSetFunction CDeviceEuresysCoaxlinkHX::SetCamfileName(CString strParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterCoaxlink eSaveID = EDeviceParameterCoaxlink_CamfileName;

	CString strPreValue = GetParamValue(eSaveID);

	do
	{
		if(IsInitialized())
		{
			eReturn = EEuresysSetFunction_AlreadyInitializedError;
			break;
		}

		if(!SetParamValue(eSaveID, strParam))
		{
			eReturn = EEuresysSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EEuresysSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEuresysSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCoaxlink[eSaveID], strPreValue, strParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction Ravid::Device::CDeviceEuresysCoaxlinkHX::GetBufferCount(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterCoaxlink_BufferCount));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction Ravid::Device::CDeviceEuresysCoaxlinkHX::SetBufferCount(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterCoaxlink eSaveID = EDeviceParameterCoaxlink_BufferCount;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EEuresysSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EEuresysSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EEuresysSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEuresysSetFunction_OK;
	}
	while(false);

	CString strMessage;


	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamCoaxlink[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGenTL * CDeviceEuresysCoaxlinkHX::GetDevice()
{
	return m_pTL;
}

bool CDeviceEuresysCoaxlinkHX::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = false;

	do
	{
		switch(nParam)
		{
		case EDeviceParameterCoaxlink_DeviceID:
			bReturn = !SetDeviceID(strValue);
			break;
		case EDeviceParameterCoaxlink_SubUnitID:
			bReturn = !SetSubUnitID(strValue);
			break;
		case EDeviceParameterCoaxlink_EuresysFolderPath:
			bReturn = !SetEuresysPath(strValue);
			break;
		case EDeviceParameterCoaxlink_CamfileName:
			bReturn = !SetCamfileName(strValue);
			break;
		case EDeviceParameterCoaxlink_BufferCount:
			bReturn = !SetBufferCount(_ttoi(strValue));
			break;
		default:
			break;
		}
	}
	while(false);

	return bReturn;
}

bool CDeviceEuresysCoaxlinkHX::DoesModuleExist()
{
	bool bReturn = false;

	CString strModuleName;

	do
	{
		CString strEuresysPath = _T("");
		GetEuresysPath(&strEuresysPath);
		if(strEuresysPath.IsEmpty())
		{
			strModuleName.Format(_T("File Path Error"));
			break;
		}

		COperatingSystemInfo osi;
		if(osi.IsX64Process())
		{
			strModuleName.Format(_T("%s\\cti\\x86_64\\coaxlink.cti"), strEuresysPath);
			if(_taccess(strModuleName, 0))
				break;

			strModuleName.Format(_T("%s\\cti\\x86_64\\coaxlink.genapi"), strEuresysPath);
			if(_taccess(strModuleName, 0))
				break;

			strModuleName.Format(_T("%s\\scripts\\configurator.js"), strEuresysPath);
			if(_taccess(strModuleName, 0))
				break;
		}
		else
		{
			strModuleName.Format(_T("%s\\cti\\x86\\coaxlink.cti"), strEuresysPath);
			if(_taccess(strModuleName, 0))
				break;

			strModuleName.Format(_T("%s\\cti\\x86\\coaxlink.genapi"), strEuresysPath);
			if(_taccess(strModuleName, 0))
				break;

			strModuleName.Format(_T("%s\\scripts\\configurator.js"), strEuresysPath);
			if(_taccess(strModuleName, 0))
				break;
		}

		bReturn = true;
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, CMultiLanguageManager::GetString(ELanguageParameter__s_d_s_toload_s), GetClassNameStr(), GetObjectID(), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), strModuleName);

	return bReturn;
}

bool CDeviceEuresysCoaxlinkHX::MakeNewBuffer(int nSizeX, int nSizeY, EDeviceEGrabberPixelFormat ePixelFormat)
{
	bool bReturn = false;

	do
	{
		this->ClearBuffer();

		CRavidImage* pCurrentImage = this->GetImageInfo();
		if(!pCurrentImage)
			break;
		
		int64_t i64Width = (int64_t)pCurrentImage->GetSizeX();
		int64_t i64Height = (int64_t)pCurrentImage->GetSizeY();
		int64_t i64Bpp = (int64_t)pCurrentImage->GetPixelSizeByte();
		int64_t i64Channels = (int64_t)pCurrentImage->GetChannels();
		int64_t i64AlignBytes = (int64_t)pCurrentImage->GetAlignBytes();
		int64_t i64WidthStep = (int64_t)pCurrentImage->GetWidthStep();

		CRavidImage::EValueFormat eVF = CRavidImage::EValueFormat_1C_U8;

		switch(ePixelFormat)
		{
		case EDeviceEGrabberPixelFormat_Mono8:
			{
				i64Channels = 1;
				eVF = CRavidImage::EValueFormat_1C_U8;
				i64Bpp = 8;
				i64WidthStep = nSizeX;
			}
			break;
		case EDeviceEGrabberPixelFormat_Mono10:
			{
				i64Channels = 1;
				eVF = CRavidImage::EValueFormat_1C_U10;
				i64Bpp = 10;
				i64WidthStep = nSizeX * 2;
			}
			break;
		}

		i64Width = nSizeX;
		i64Height = nSizeY;

		if((i64WidthStep % 4))
			i64WidthStep += (i64AlignBytes - (i64WidthStep % i64AlignBytes));

		int64_t i64MaxValue = (1 << i64Bpp) - 1;

		CMultipleVariable rmv;
		for(int64_t i = 0; i < i64Channels; ++i)
			rmv.AddValue(i64MaxValue);

		this->InitBuffer(nSizeX, nSizeY, rmv, eVF, i64WidthStep, i64AlignBytes);

		this->ConnectImage();

		bReturn = true;
	}
	while(false);

	return bReturn;
}

UINT CDeviceEuresysCoaxlinkHX::CallbackFunction(LPVOID pParam)
{
	CDeviceEuresysCoaxlinkHX* pInstance = (CDeviceEuresysCoaxlinkHX*)pParam;

	if(pInstance)
	{
		int nGrabCount = 0;

		do
		{
			bool bGrab = false;
			unsigned char* imagePtr = nullptr;
			size_t width = 0;
			size_t height = 0;

			TryCatchBegin
			{
				Euresys::ScopedBuffer buffer(*pInstance->m_pEGrabber);
				imagePtr = (BYTE*)buffer.getInfo<void*>(GenTL::BUFFER_INFO_BASE);

				width = buffer.getInfo<size_t>(gc::BUFFER_INFO_WIDTH);
				height = buffer.getInfo<size_t>(gc::BUFFER_INFO_DELIVERED_IMAGEHEIGHT);
				bGrab = true;
			}
			TryCatchException;
			TryCatchEnd;

			if(bGrab)
			{
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
				
				memcpy(pCurrentBuffer, imagePtr, width * height);

				pInstance->ConnectImage(false);

				CEventHandlerManager::BroadcastOnAcquisition(pCurrentImage, pInstance->GetObjectID(), pInstance->GetImageView(), pInstance);

				pInstance->m_bIsGrabAvailable = true;
			}
		}
		while(pInstance->IsInitialized() && (pInstance->IsLive() || (!pInstance->IsGrabAvailable())));

		TryCatchBegin
			pInstance->m_pEGrabber->stop();
		TryCatchException;
		TryCatchEnd;

		pInstance->m_bIsLive = false;
		pInstance->m_bIsGrabAvailable = true;
	}

	return 0;
}

int64_t CDeviceEuresysCoaxlinkHX::__getInteger(eCoaxlinkModuleType eType, std::string & feature)
{
	int64_t i64Return = -1;

	TryCatchBegin
		switch(eType)
		{
		case CDeviceEuresysCoaxlinkHX::eCoaxlinkModuleType_System:
			i64Return = m_pEGrabber->getInteger<SystemModule>(feature);
			break;
		case CDeviceEuresysCoaxlinkHX::eCoaxlinkModuleType_Interface:
			i64Return = m_pEGrabber->getInteger<InterfaceModule>(feature);
			break;
		case CDeviceEuresysCoaxlinkHX::eCoaxlinkModuleType_Device:
			i64Return = m_pEGrabber->getInteger<DeviceModule>(feature);
			break;
		case CDeviceEuresysCoaxlinkHX::eCoaxlinkModuleType_Stream:
			i64Return = m_pEGrabber->getInteger<StreamModule>(feature);
			break;
		case CDeviceEuresysCoaxlinkHX::eCoaxlinkModuleType_Remote:
			i64Return = m_pEGrabber->getInteger<RemoteModule>(feature);
			break;
		default:
			break;
		}
	TryCatchException;
	TryCatchEnd;

	return i64Return;
}
void CDeviceEuresysCoaxlinkHX::__setInteger(eCoaxlinkModuleType eType, std::string & feature, int64_t value)
{
	TryCatchBegin
		switch(eType)
		{
		case CDeviceEuresysCoaxlinkHX::eCoaxlinkModuleType_System:
			m_pEGrabber->setInteger<SystemModule>(feature, value);
			break;
		case CDeviceEuresysCoaxlinkHX::eCoaxlinkModuleType_Interface:
			m_pEGrabber->setInteger<InterfaceModule>(feature, value);
			break;
		case CDeviceEuresysCoaxlinkHX::eCoaxlinkModuleType_Device:
			m_pEGrabber->setInteger<DeviceModule>(feature, value);
			break;
		case CDeviceEuresysCoaxlinkHX::eCoaxlinkModuleType_Stream:
			m_pEGrabber->setInteger<StreamModule>(feature, value);
			break;
		case CDeviceEuresysCoaxlinkHX::eCoaxlinkModuleType_Remote:
			m_pEGrabber->setInteger<RemoteModule>(feature, value);
			break;
		default:
			break;
		}
	TryCatchException;
	TryCatchEnd;
}

double CDeviceEuresysCoaxlinkHX::__getFloat(eCoaxlinkModuleType eType, std::string & feature)
{
	double dblReturn = 0;

	TryCatchBegin
		switch(eType)
		{
		case CDeviceEuresysCoaxlinkHX::eCoaxlinkModuleType_System:
			dblReturn = m_pEGrabber->getFloat<SystemModule>(feature);
			break;
		case CDeviceEuresysCoaxlinkHX::eCoaxlinkModuleType_Interface:
			dblReturn = m_pEGrabber->getFloat<InterfaceModule>(feature);
			break;
		case CDeviceEuresysCoaxlinkHX::eCoaxlinkModuleType_Device:
			dblReturn = m_pEGrabber->getFloat<DeviceModule>(feature);
			break;
		case CDeviceEuresysCoaxlinkHX::eCoaxlinkModuleType_Stream:
			dblReturn = m_pEGrabber->getFloat<StreamModule>(feature);
			break;
		case CDeviceEuresysCoaxlinkHX::eCoaxlinkModuleType_Remote:
			dblReturn = m_pEGrabber->getFloat<RemoteModule>(feature);
			break;
		default:
			break;
		}
	TryCatchException;
	TryCatchEnd;

	return dblReturn;
}
void CDeviceEuresysCoaxlinkHX::__setFloat(eCoaxlinkModuleType eType, std::string & feature, double value)
{
	TryCatchBegin
		switch(eType)
		{
		case CDeviceEuresysCoaxlinkHX::eCoaxlinkModuleType_System:
			m_pEGrabber->setFloat<SystemModule>(feature, value);
			break;
		case CDeviceEuresysCoaxlinkHX::eCoaxlinkModuleType_Interface:
			m_pEGrabber->setFloat<InterfaceModule>(feature, value);
			break;
		case CDeviceEuresysCoaxlinkHX::eCoaxlinkModuleType_Device:
			m_pEGrabber->setFloat<DeviceModule>(feature, value);
			break;
		case CDeviceEuresysCoaxlinkHX::eCoaxlinkModuleType_Stream:
			m_pEGrabber->setFloat<StreamModule>(feature, value);
			break;
		case CDeviceEuresysCoaxlinkHX::eCoaxlinkModuleType_Remote:
			m_pEGrabber->setFloat<RemoteModule>(feature, value);
			break;
		default:
			break;
		}
	TryCatchException;
	TryCatchEnd;
}

std::string CDeviceEuresysCoaxlinkHX::__getString(eCoaxlinkModuleType eType, std::string & feature)
{
	std::string astring;

	TryCatchBegin
		switch(eType)
		{
		case CDeviceEuresysCoaxlinkHX::eCoaxlinkModuleType_System:
			astring = m_pEGrabber->getString<SystemModule>(feature);
			break;
		case CDeviceEuresysCoaxlinkHX::eCoaxlinkModuleType_Interface:
			astring = m_pEGrabber->getString<InterfaceModule>(feature);
			break;
		case CDeviceEuresysCoaxlinkHX::eCoaxlinkModuleType_Device:
			astring = m_pEGrabber->getString<DeviceModule>(feature);
			break;
		case CDeviceEuresysCoaxlinkHX::eCoaxlinkModuleType_Stream:
			astring = m_pEGrabber->getString<StreamModule>(feature);
			break;
		case CDeviceEuresysCoaxlinkHX::eCoaxlinkModuleType_Remote:
			astring = m_pEGrabber->getString<RemoteModule>(feature);
			break;
		default:
			break;
		}
	TryCatchException;
	TryCatchEnd;

	return astring;
}
void CDeviceEuresysCoaxlinkHX::__setString(eCoaxlinkModuleType eType, std::string & feature, std::string & value)
{
	TryCatchBegin
		switch(eType)
		{
		case CDeviceEuresysCoaxlinkHX::eCoaxlinkModuleType_System:
			m_pEGrabber->setString<SystemModule>(feature, value);
			break;
		case CDeviceEuresysCoaxlinkHX::eCoaxlinkModuleType_Interface:
			m_pEGrabber->setString<InterfaceModule>(feature, value);
			break;
		case CDeviceEuresysCoaxlinkHX::eCoaxlinkModuleType_Device:
			m_pEGrabber->setString<DeviceModule>(feature, value);
			break;
		case CDeviceEuresysCoaxlinkHX::eCoaxlinkModuleType_Stream:
			m_pEGrabber->setString<StreamModule>(feature, value);
			break;
		case CDeviceEuresysCoaxlinkHX::eCoaxlinkModuleType_Remote:
			m_pEGrabber->setString<RemoteModule>(feature, value);
			break;
		default:
			break;
		}
	TryCatchException;
	TryCatchEnd;
}

void CDeviceEuresysCoaxlinkHX::__execute(eCoaxlinkModuleType eType, std::string value)
{
	TryCatchBegin
		switch(eType)
		{
		case CDeviceEuresysCoaxlinkHX::eCoaxlinkModuleType_System:
			m_pEGrabber->execute<SystemModule>(value);
			break;
		case CDeviceEuresysCoaxlinkHX::eCoaxlinkModuleType_Interface:
			m_pEGrabber->execute<InterfaceModule>(value);
			break;
		case CDeviceEuresysCoaxlinkHX::eCoaxlinkModuleType_Device:
			m_pEGrabber->execute<DeviceModule>(value);
			break;
		case CDeviceEuresysCoaxlinkHX::eCoaxlinkModuleType_Stream:
			m_pEGrabber->execute<StreamModule>(value);
			break;
		case CDeviceEuresysCoaxlinkHX::eCoaxlinkModuleType_Remote:
			m_pEGrabber->execute<RemoteModule>(value);
			break;
		default:
			break;
		}
	TryCatchException;
	TryCatchEnd;
}

#endif