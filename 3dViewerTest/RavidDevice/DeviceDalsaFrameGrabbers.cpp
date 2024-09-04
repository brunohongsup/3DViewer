#include "stdafx.h"

#include "DeviceDalsaFrameGrabbers.h"

#ifdef USING_DEVICE

#include "../RavidCore/RavidRect.h"
#include "../RavidCore/RavidImage.h"
#include "../RavidCore/RavidDatabase.h"
#include "../RavidCore/RavidRecordset.h"
#include "../RavidCore/PerformanceCounter.h"

#include "../RavidCore/RavidImage.h"

#include "../RavidFramework/UIManager.h"
#include "../RavidFramework/LogManager.h"
#include "../RavidFramework/DeviceManager.h"
#include "../RavidFramework/LibraryManager.h"
#include "../RavidFramework/FrameworkManager.h"
#include "../RavidFramework/EventHandlerManager.h" 
#include "../RavidFramework/MultiLanguageManager.h"

#include "../RavidFramework/RavidImageView.h"

#include "../Libraries/Includes/DalsaXtiumMX4/SapClassBasic.h"

using namespace Ravid;
using namespace Ravid::Device;
using namespace Ravid::Database;
using namespace Ravid::Framework;
using namespace Ravid::Algorithms;
using namespace Ravid::Miscellaneous;

IMPLEMENT_DYNAMIC(CDeviceDalsaFrameGrabbers, CDeviceFrameGrabber)

BEGIN_MESSAGE_MAP(CDeviceDalsaFrameGrabbers, CDeviceFrameGrabber)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static LPCTSTR g_lpszParamDalsaFrameGrabbers[EDeviceParameterDalsaFrameGrabbers_Count] =
{
	_T("DeviceID"),
	_T("SubUnitID"),
	_T("CamFile Name"),
};

CDeviceDalsaFrameGrabbers::CDeviceDalsaFrameGrabbers()
{
}


CDeviceDalsaFrameGrabbers::~CDeviceDalsaFrameGrabbers()
{
}

EDeviceInitializeResult CDeviceDalsaFrameGrabbers::Initialize()
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


		SapManager::Open();

		strMessage = CMultiLanguageManager::GetString(ELanguageParameter_TheDevicehasbeensuccessfullyinitialized);

		eReturn = EDeviceInitializeResult_OK;
	}
	while(false);

	SetStatus(strStatus);

	SetMessage(strMessage);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	return eReturn;
}

EDeviceTerminateResult CDeviceDalsaFrameGrabbers::Terminate()
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
		if(!IsGrabAvailable())
			Stop();

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

bool CDeviceDalsaFrameGrabbers::LoadSettings()
{
	bool bReturn = false;

	do
	{
		int nParam = EDeviceParameterDalsaFrameGrabbers_DeviceID;

		AddParameterFieldConfigurations(nParam, g_lpszParamDalsaFrameGrabbers[nParam], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(++nParam, g_lpszParamDalsaFrameGrabbers[nParam], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(++nParam, g_lpszParamDalsaFrameGrabbers[nParam], _T("DalsaFrameGrabbers.ccf"), EParameterFieldType_Edit, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(++nParam, g_lpszParamDalsaFrameGrabbers[nParam], _T("DalsaFrameGrabbersCamera.ccf"), EParameterFieldType_Edit, nullptr, nullptr, 0);

		bReturn = true;
	}
	while(false);

	return bReturn;
}

EDeviceGrabResult CDeviceDalsaFrameGrabbers::Grab()
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

		m_bIsGrabAvailable = false;

		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("Grab"));

		eReturn = EDeviceGrabResult_OK;
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);
		
	return eReturn;
}

EDeviceLiveResult CDeviceDalsaFrameGrabbers::Live()
{
	return EDeviceLiveResult_DoNotSupport;
}

EDeviceStopResult CDeviceDalsaFrameGrabbers::Stop()
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

		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("Stop"));

		eReturn = EDeviceStopResult_OK;
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceTriggerResult CDeviceDalsaFrameGrabbers::Trigger()
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

		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("Trigger"));

		eReturn = EDeviceTriggerResult_OK;
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDalsaFrameGrabbersGetFunction CDeviceDalsaFrameGrabbers::GetCamfileName(CString * pParam)
{
	EDalsaFrameGrabbersGetFunction eReturn = EDalsaFrameGrabbersGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDalsaFrameGrabbersGetFunction_NullptrError;
			break;
		}

		*pParam = GetParamValue(EDeviceParameterDalsaFrameGrabbers_CamfileName);

		eReturn = EDalsaFrameGrabbersGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDalsaFrameGrabbersSetFunction CDeviceDalsaFrameGrabbers::SetCamfileName(CString strParam)
{
	EDalsaFrameGrabbersSetFunction eReturn = EDalsaFrameGrabbersSetFunction_UnknownError;

	EDeviceParameterDalsaFrameGrabbers eSaveID = EDeviceParameterDalsaFrameGrabbers_CamfileName;

	CString strPreValue = GetParamValue(eSaveID);

	do
	{
		if(IsInitialized())
		{
			eReturn = EDalsaFrameGrabbersSetFunction_AlreadyInitializedError;
			break;
		}

		if(!SetParamValue(eSaveID, strParam))
		{
			eReturn = EDalsaFrameGrabbersSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDalsaFrameGrabbersSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDalsaFrameGrabbersSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDalsaFrameGrabbers[eSaveID], strPreValue, strParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceDalsaFrameGrabbers::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = false;

	do
	{
	}
	while(false);

	return bReturn;
}

bool CDeviceDalsaFrameGrabbers::DoesModuleExist()
{
	bool bReturn = false;

	CString strModuleName;

	do
	{
		strModuleName.Format(_T("SapClassBasic83.dll"));

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