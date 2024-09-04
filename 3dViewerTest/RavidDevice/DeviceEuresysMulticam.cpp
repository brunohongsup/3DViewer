#include "stdafx.h"

#include "DeviceEuresysMulticam.h"

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

#include "../RavidFramework/RavidImageView.h"

#include "../Libraries/Includes/MultiCam/multicam.h"

// multicam.dll
#pragma comment(lib, COMMONLIB_PREFIX "MultiCam/MultiCam.lib")

using namespace Ravid;
using namespace Ravid::Device;
using namespace Ravid::Database;
using namespace Ravid::Framework;
using namespace Ravid::Algorithms;
using namespace Ravid::Miscellaneous;

IMPLEMENT_DYNAMIC(CDeviceEuresysMulticam, CDeviceFrameGrabber)

static LPCTSTR g_lpszParamMulticam[EDeviceParameterMulticam_Count] =
{
	_T("DeviceID"),
	_T("Initialize Type"),
	_T("Camfile Path"),
	_T("Camfile Match"),
};

static LPCTSTR g_lpszMulticamInitializeType[EDeviceMulticamInitializeType_Count] =
{
	_T("Camfile"),
	_T("Parameter"),
};

static LPCTSTR g_lpszMulticamSwitch[EDeviceMulticamSwitch_Count] =
{
	_T("Off"),
	_T("On"),
};

CDeviceEuresysMulticam::CDeviceEuresysMulticam()
{
}


CDeviceEuresysMulticam::~CDeviceEuresysMulticam()
{
}

EDeviceInitializeResult CDeviceEuresysMulticam::Initialize()
{
	CString strMessage, strStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("EuresysMulticam"));
		
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

		if(McOpenDriver(nullptr) != MC_OK)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntopenthelibrary);
			eReturn = EDeviceInitializeResult_NotOpenedDeviceError;
			break;
		}

		int nCount = 0;

		if(McGetParamInt(MC_CONFIGURATION, MC_BoardCount, &nCount) != MC_OK)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntsupportthe_s), _T("BoardCount"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		if(!nCount)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtofindthemoduleofdevice);
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		m_nConnectBoard = nCount;

		strMessage = CMultiLanguageManager::GetString(ELanguageParameter_TheDevicehasbeensuccessfullyinitialized);

		eReturn = EDeviceInitializeResult_OK;
	}
	while(false);

	SetStatus(strStatus);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceTerminateResult CDeviceEuresysMulticam::Terminate()
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
		if(m_hDevice)
		{
			if(!IsGrabAvailable())
				Stop();

			m_bIsInitialized = false;
			m_bIsGrabAvailable = true;
			m_nConnectBoard = 0;

			if(McSetParamInt(m_hDevice, MC_ChannelState, MC_ChannelState_IDLE) != MC_OK)
			{
				strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntcontrolthedevice);
				eReturn = EDeviceTerminateResult_WriteToDeviceError;
			}
			else
				eReturn = EDeviceTerminateResult_OK;

			McDelete(m_hDevice);

			m_hDevice = -1;
			m_nDriveIndex = -1;
			m_nCurrentCount = -1;
		}
		
		strMessage = CMultiLanguageManager::GetString(ELanguageParameter_TheDevicehasbeensuccessfullyterminated);

		CEventHandlerManager::BroadcastOnDeviceTerminated(this);
	}
	while(false);

	this->ClearBuffer();

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceEuresysMulticam::LoadSettings()
{
	bool bReturn = false;

	do
	{
		AddParameterFieldConfigurations(EDeviceParameterMulticam_DeviceID, g_lpszParamMulticam[EDeviceParameterMulticam_DeviceID], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 0);

		AddParameterFieldConfigurations(EDeviceParameterMulticam_InitializeType, g_lpszParamMulticam[EDeviceParameterMulticam_InitializeType], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszMulticamInitializeType, EDeviceMulticamInitializeType_Count), nullptr, 0);

		AddParameterFieldConfigurations(EDeviceParameterMulticam_CamfilePath, g_lpszParamMulticam[EDeviceParameterMulticam_CamfilePath], _T("-"), EParameterFieldType_Edit, nullptr, nullptr, 0);

		AddParameterFieldConfigurations(EDeviceParameterMulticam_CamfileMatch, g_lpszParamMulticam[EDeviceParameterMulticam_CamfileMatch], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 0);

		bReturn = true;
	}
	while(false);

	return bReturn;
}

EDeviceGrabResult CDeviceEuresysMulticam::Grab()
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

		int nCount = -1;

		McGetParamInt(m_hDevice, MC_GrabCount, &nCount);

		if(nCount < 0)
			nCount = INT_MAX;

		m_nCurrentCount = nCount;

		if(McSetParamInt(m_hDevice, MC_ChannelState, MC_ChannelState_ACTIVE) != MC_OK)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Grab count"));
			eReturn = EDeviceGrabResult_WriteToDeviceError;
			break;
		}

		m_bIsGrabAvailable = false;

		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("Grab"));

		eReturn = EDeviceGrabResult_OK;
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);
		
	return eReturn;
}

EDeviceLiveResult CDeviceEuresysMulticam::Live()
{
	return EDeviceLiveResult_DoNotSupport;
}

EDeviceStopResult CDeviceEuresysMulticam::Stop()
{
	EDeviceStopResult eReturn = EDeviceStopResult_UnknownError;

	CString strMessage;

	m_bIsGrabAvailable = true;

	do
	{
		if(!IsInitialized())
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Ithasnotbeeninitializedyet);
			eReturn = EDeviceStopResult_NotInitializedError;
			break;
		}

		if(McSetParamInt(m_hDevice, MC_ChannelState, MC_ChannelState_IDLE) != MC_OK)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Channel Idle"));
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

EDeviceTriggerResult CDeviceEuresysMulticam::Trigger()
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

		if(McSetParamInt(m_hDevice, MC_SoftTrig, 1) != MC_OK)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TriggerSoftware"));
			eReturn = EDeviceTriggerResult_WriteToDeviceError;
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

EEuresysGetFunction CDeviceEuresysMulticam::GetInitializetype(EDeviceMulticamInitializeType * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMulticamInitializeType)_ttoi(GetParamValue(EDeviceParameterMulticam_InitializeType));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysMulticam::SetInitializetype(EDeviceMulticamInitializeType eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterMulticam eSaveID = EDeviceParameterMulticam_InitializeType;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceMulticamInitializeType_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMulticam[eSaveID], g_lpszMulticamInitializeType[nPreValue], g_lpszMulticamInitializeType[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysMulticam::GetCamfilePath(CString * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = GetParamValue(EDeviceParameterMulticam_CamfilePath);

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysMulticam::SetCamfilePath(CString strParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterMulticam eSaveID = EDeviceParameterMulticam_CamfilePath;

	CString strPreValue = GetParamValue(eSaveID);

	do
	{
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMulticam[eSaveID], strPreValue, strParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysMulticam::GetCamfileMatch(bool * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMulticam_CamfileMatch));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysMulticam::SetCamfileMatch(bool bParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterMulticam eSaveID = EDeviceParameterMulticam_CamfileMatch;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		CString strSave;
		strSave.Format(_T("%d"), (int)bParam);

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMulticam[eSaveID], g_lpszMulticamSwitch[nPreValue], g_lpszMulticamSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceEuresysMulticam::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = false;

	do
	{
		switch(nParam)
		{
		case EDeviceParameterMulticam_DeviceID:
			bReturn = !SetDeviceID(strValue);
			break;
		case EDeviceParameterMulticam_InitializeType:
			bReturn = !SetInitializetype((EDeviceMulticamInitializeType)_ttoi(strValue));
			break;
		case EDeviceParameterMulticam_CamfilePath:
			bReturn = !SetCamfilePath(strValue);
			break;
		case EDeviceParameterMulticam_CamfileMatch:
			bReturn = !SetCamfileMatch(_ttoi(strValue));
			break;
		default:
			break;
		}
	}
	while(false);

	return bReturn;
}

bool CDeviceEuresysMulticam::DoesModuleExist()
{
	bool bReturn = false;

	CString strModuleName;

	do
	{
		strModuleName.Format(_T("MultiCam.dll"));

		EDeviceLibraryStatus eStatus = CLibraryManager::GetFoundLibrary(strModuleName);

		if(!eStatus)
		{
			HMODULE hModule = LoadLibrary(strModuleName);

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