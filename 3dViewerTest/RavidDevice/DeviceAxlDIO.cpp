#include "stdafx.h"
#include "DeviceAxlDio.h"

#ifdef USING_DEVICE

#include "DeviceAxlMotion.h"
#include "DeviceAxlMotionEthercat.h"
#include "DeviceAxlTriggerSIOCN2CH.h"

#include "../RavidCore/RavidDatabase.h"
#include "../RavidCore/RavidRecordset.h"

#include "../RavidFramework/LogManager.h"
#include "../RavidFramework/DeviceManager.h"
#include "../RavidFramework/LibraryManager.h"
#include "../RavidFramework/SequenceManager.h"
#include "../RavidFramework/AuthorityManager.h"
#include "../RavidFramework/FrameworkManager.h"
#include "../RavidFramework/EventHandlerManager.h"
#include "../RavidFramework/MultiLanguageManager.h"

#include "../RavidFramework/RavidLedCtrl.h"

#include "../Libraries/Includes/AxlLib/AXHS.h"
#include "../Libraries/Includes/AxlLib/AXL.h"
#include "../Libraries/Includes/AxlLib/AXD.h"

// axl.dll
#pragma comment(lib, COMMONLIB_PREFIX "AxlLib/AXL.lib")

using namespace Ravid;
using namespace Ravid::Device;
using namespace Ravid::Database;
using namespace Ravid::Framework;
using namespace Ravid::Algorithms;
using namespace Ravid::Miscellaneous;

enum
{
	AXL_DIO_IN_PORT = 32,
	AXL_DIO_OUT_PORT = 32,
};

enum
{
	AXL_MODULE_MIN = 0,
	AXL_MODULE_MAX = 99,
};

IMPLEMENT_DYNAMIC(CDeviceAxlDio, CDeviceDio)

BEGIN_MESSAGE_MAP(CDeviceAxlDio, CDeviceDio)
	ON_COMMAND_RANGE(EDeviceAddEnumeratedControlID_OutputLedStart, EDeviceAddEnumeratedControlID_OutputLedEnd, OnBnClickedOutLed)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static LPCTSTR g_lpszParamAxlDio[EDeviceParameterAxlDio_Count] =
{
	_T("DeviceID"),
	_T("Input status"),
	_T("Use Input No. 0"),
	_T("Use Input No. 1"),
	_T("Use Input No. 2"),
	_T("Use Input No. 3"),
	_T("Use Input No. 4"),
	_T("Use Input No. 5"),
	_T("Use Input No. 6"),
	_T("Use Input No. 7"),
	_T("Use Input No. 8"),
	_T("Use Input No. 9"),
	_T("Use Input No. 10"),
	_T("Use Input No. 11"),
	_T("Use Input No. 12"),
	_T("Use Input No. 13"),
	_T("Use Input No. 14"),
	_T("Use Input No. 15"),
	_T("Use Input No. 16"),
	_T("Use Input No. 17"),
	_T("Use Input No. 18"),
	_T("Use Input No. 19"),
	_T("Use Input No. 20"),
	_T("Use Input No. 21"),
	_T("Use Input No. 22"),
	_T("Use Input No. 23"),
	_T("Use Input No. 24"),
	_T("Use Input No. 25"),
	_T("Use Input No. 26"),
	_T("Use Input No. 27"),
	_T("Use Input No. 28"),
	_T("Use Input No. 29"),
	_T("Use Input No. 30"),
	_T("Use Input No. 31"),
	_T("Output status"),
	_T("Use Output No. 0"),
	_T("Use Output No. 1"),
	_T("Use Output No. 2"),
	_T("Use Output No. 3"),
	_T("Use Output No. 4"),
	_T("Use Output No. 5"),
	_T("Use Output No. 6"),
	_T("Use Output No. 7"),
	_T("Use Output No. 8"),
	_T("Use Output No. 9"),
	_T("Use Output No. 10"),
	_T("Use Output No. 11"),
	_T("Use Output No. 12"),
	_T("Use Output No. 13"),
	_T("Use Output No. 14"),
	_T("Use Output No. 15"),
	_T("Use Output No. 16"),
	_T("Use Output No. 17"),
	_T("Use Output No. 18"),
	_T("Use Output No. 19"),
	_T("Use Output No. 20"),
	_T("Use Output No. 21"),
	_T("Use Output No. 22"),
	_T("Use Output No. 23"),
	_T("Use Output No. 24"),
	_T("Use Output No. 25"),
	_T("Use Output No. 26"),
	_T("Use Output No. 27"),
	_T("Use Output No. 28"),
	_T("Use Output No. 29"),
	_T("Use Output No. 30"),
	_T("Use Output No. 31"),
	_T("Input Name"),
	_T("Input No. 0 Name"),
	_T("Input No. 1 Name"),
	_T("Input No. 2 Name"),
	_T("Input No. 3 Name"),
	_T("Input No. 4 Name"),
	_T("Input No. 5 Name"),
	_T("Input No. 6 Name"),
	_T("Input No. 7 Name"),
	_T("Input No. 8 Name"),
	_T("Input No. 9 Name"),
	_T("Input No. 10 Name"),
	_T("Input No. 11 Name"),
	_T("Input No. 12 Name"),
	_T("Input No. 13 Name"),
	_T("Input No. 14 Name"),
	_T("Input No. 15 Name"),
	_T("Input No. 16 Name"),
	_T("Input No. 17 Name"),
	_T("Input No. 18 Name"),
	_T("Input No. 19 Name"),
	_T("Input No. 20 Name"),
	_T("Input No. 21 Name"),
	_T("Input No. 22 Name"),
	_T("Input No. 23 Name"),
	_T("Input No. 24 Name"),
	_T("Input No. 25 Name"),
	_T("Input No. 26 Name"),
	_T("Input No. 27 Name"),
	_T("Input No. 28 Name"),
	_T("Input No. 29 Name"),
	_T("Input No. 30 Name"),
	_T("Input No. 31 Name"),
	_T("Output Name"),
	_T("Output No. 0 Name"),
	_T("Output No. 1 Name"),
	_T("Output No. 2 Name"),
	_T("Output No. 3 Name"),
	_T("Output No. 4 Name"),
	_T("Output No. 5 Name"),
	_T("Output No. 6 Name"),
	_T("Output No. 7 Name"),
	_T("Output No. 8 Name"),
	_T("Output No. 9 Name"),
	_T("Output No. 10 Name"),
	_T("Output No. 11 Name"),
	_T("Output No. 12 Name"),
	_T("Output No. 13 Name"),
	_T("Output No. 14 Name"),
	_T("Output No. 15 Name"),
	_T("Output No. 16 Name"),
	_T("Output No. 17 Name"),
	_T("Output No. 18 Name"),
	_T("Output No. 19 Name"),
	_T("Output No. 20 Name"),
	_T("Output No. 21 Name"),
	_T("Output No. 22 Name"),
	_T("Output No. 23 Name"),
	_T("Output No. 24 Name"),
	_T("Output No. 25 Name"),
	_T("Output No. 26 Name"),
	_T("Output No. 27 Name"),
	_T("Output No. 28 Name"),
	_T("Output No. 29 Name"),
	_T("Output No. 30 Name"),
	_T("Output No. 31 Name"),
	_T("Use Clean Initialization"),
	_T("Use Clean Termination"),
	_T("Open No Reset")
};

static LPCTSTR g_lpszSwitch[EDeviceDIOSwitch_Count] =
{
	_T("Off"),
	_T("On")
};

CDeviceAxlDio::CDeviceAxlDio()
{
}


CDeviceAxlDio::~CDeviceAxlDio()
{
	Terminate();
}

EDeviceInitializeResult CDeviceAxlDio::Initialize()
{
	EDeviceInitializeResult eReturn = EDeviceInitializeResult_UnknownError;

	CString strMessage, strStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("AxlDio"));
		
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

		if(!AxlIsOpened())
		{
			bool bInitHardware = false;
			GetOpenNoHardware(&bInitHardware);

			if(bInitHardware != true)
			{
				if(AxlOpenNoReset(-1) != AXT_RT_SUCCESS)
				{
					strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntopenthelibrary);
					eReturn = EDeviceInitializeResult_AlreadyInitializedError;
					break;
				}
			}
			else
			{
				if(AxlOpen(-1) != AXT_RT_SUCCESS)
				{
					strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntopenthelibrary);
					eReturn = EDeviceInitializeResult_AlreadyInitializedError;
					break;
				}
			}			
		}

		DWORD dwStatus = STATUS_NOTEXIST;
		if(AxdInfoIsDIOModule(&dwStatus) != AXT_RT_SUCCESS)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Module info"));
			eReturn = EDeviceInitializeResult_NotFoundDeviceError;
			break;
		}

		if(dwStatus != STATUS_EXIST)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_CouldntfindIOmodule);
			eReturn = EDeviceInitializeResult_NotFoundDeviceError;
			break;
		}

		long i32ModulCount = 0;
		if(AxdInfoGetModuleCount(&i32ModulCount) != AXT_RT_SUCCESS)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Module Count"));
			eReturn = EDeviceInitializeResult_NotFoundDeviceError;
			break;
		}

		int nModuleID = _ttoi(GetDeviceID());
		if(0 > nModuleID || i32ModulCount <= nModuleID)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_DoesntsupporttheDeviceID);
			eReturn = EDeviceInitializeResult_NotInitializeMotionError;
			break;
		}

		if(AxdInfoGetModuleStatus(nModuleID) != AXT_RT_SUCCESS)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Module info"));
			eReturn = EDeviceInitializeResult_NotFoundDeviceError;
			break;
		}
		
		m_bIsInitialized = true;

		bool bClean = false;
		GetUseClearInitalize(&bClean);

		if(bClean)
			ClearOutPort();

		strMessage = CMultiLanguageManager::GetString(ELanguageParameter_TheDevicehasbeensuccessfullyinitialized);

		strStatus = CMultiLanguageManager::GetString(ELanguageParameter_Initialized);

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

EDeviceTerminateResult CDeviceAxlDio::Terminate()
{
	EDeviceTerminateResult eReturn = EDeviceTerminateResult_UnknownError;

	CString strMessage;

	SetStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("AxlDio"));
		
		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);
				
		return EDeviceTerminateResult_NotFoundApiError;
	}

	do 
	{
		bool bClean = false;
		GetUseClearTerminate(&bClean);

		if(bClean)
			ClearOutPort();

		bool bNeedTreminate = true;
		size_t szDeviceCnt = CDeviceManager::GetDeviceCount();
		for(size_t i = 0; i < szDeviceCnt; ++i)
		{
			CDeviceBase* pDevicebase = CDeviceManager::GetDeviceByIndex(i);
			if(!pDevicebase)
				continue;

			if(this == pDevicebase)
				continue;

			CDeviceAxlMotion* pMotion = dynamic_cast<CDeviceAxlMotion*>(pDevicebase);
			if(pMotion)
			{
				if(!pMotion->IsInitialized())
					continue;

				bNeedTreminate = false;

				break;
			}

			CDeviceAxlMotionEthercat* pMotionEth = dynamic_cast<CDeviceAxlMotionEthercat*>(pDevicebase);
			if(pMotionEth)
			{
				if(!pMotionEth->IsInitialized())
					continue;

				bNeedTreminate = false;

				break;
			}

			CDeviceAxlDio* pDio = dynamic_cast<CDeviceAxlDio*>(pDevicebase);
			if(pDio)
			{
				if(!pDio->IsInitialized())
					continue;

				bNeedTreminate = false;

				break;
			}

			CDeviceAxlTriggerSIOCN2CH* pTrigger = dynamic_cast<CDeviceAxlTriggerSIOCN2CH*>(pDevicebase);
			if(pTrigger)
			{
				if(!pTrigger->IsInitialized())
					continue;

				bNeedTreminate = false;

				break;
			}
		}

		if(bNeedTreminate)
		{
			if(AxlIsOpened())
				AxlClose();
		}

		m_bIsInitialized = false;

		strMessage = CMultiLanguageManager::GetString(ELanguageParameter_TheDevicehasbeensuccessfullyterminated);

		eReturn = EDeviceTerminateResult_OK;

		CEventHandlerManager::BroadcastOnDeviceTerminated(this);
	} 
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceAxlDio::LoadSettings()
{
	bool bReturn = false;

	m_vctParameterFieldConfigurations.clear();

	do
	{
		AddParameterFieldConfigurations(EDeviceParameterAxlDio_DeviceID, g_lpszParamAxlDio[EDeviceParameterAxlDio_DeviceID], _T("0"), EParameterFieldType_Edit);

		AddParameterFieldConfigurations(EDeviceParameterAxlDio_UseClearInitialize, g_lpszParamAxlDio[EDeviceParameterAxlDio_UseClearInitialize], _T("1"), EParameterFieldType_Check, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterAxlDio_UseClearTerminate, g_lpszParamAxlDio[EDeviceParameterAxlDio_UseClearTerminate], _T("1"), EParameterFieldType_Check, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterAxlDio_OpenNoReset, g_lpszParamAxlDio[EDeviceParameterAxlDio_OpenNoReset], _T("0"), EParameterFieldType_Check, nullptr, _T("Initializes the hardware when initializing the device."), 0);


		AddParameterFieldConfigurations(EDeviceParameterAxlDio_InputStatus, g_lpszParamAxlDio[EDeviceParameterAxlDio_InputStatus], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);

		for(int i = 0; i < AXL_DIO_IN_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterAxlDio_UseInputNo0 + i, g_lpszParamAxlDio[EDeviceParameterAxlDio_UseInputNo0 + i], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 1);
		
		AddParameterFieldConfigurations(EDeviceParameterAxlDio_OutputStatus, g_lpszParamAxlDio[EDeviceParameterAxlDio_OutputStatus], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);

		for(int i = 0; i < AXL_DIO_OUT_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterAxlDio_UseOutputNo0 + i, g_lpszParamAxlDio[EDeviceParameterAxlDio_UseOutputNo0 + i], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 1);
		
		AddParameterFieldConfigurations(EDeviceParameterAxlDio_InputName, g_lpszParamAxlDio[EDeviceParameterAxlDio_InputName], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);

		for(int i = 0; i < AXL_DIO_IN_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterAxlDio_InputNo0Name + i, g_lpszParamAxlDio[EDeviceParameterAxlDio_InputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);
		
		AddParameterFieldConfigurations(EDeviceParameterAxlDio_OutputName, g_lpszParamAxlDio[EDeviceParameterAxlDio_OutputName], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);

		for(int i = 0; i < AXL_DIO_OUT_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterAxlDio_OutputNo0Name + i, g_lpszParamAxlDio[EDeviceParameterAxlDio_OutputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);
		
		bReturn = __super::LoadSettings();

		AllocateIO(AXL_DIO_IN_PORT, AXL_DIO_OUT_PORT);

		for(auto iter = m_vctParameterFieldConfigurations.begin(); iter != m_vctParameterFieldConfigurations.end(); ++iter)
		{
			if(iter->strParameterName.Find(_T("Use Input No. ")) >= 0)
			{
				int nNumber = _ttoi(iter->strParameterName.Mid(14, 2));

				m_pBUsingInputArray[nNumber] = _ttoi(iter->strParameterValue);
			}
			else if(iter->strParameterName.Find(_T("Use Output No. ")) >= 0)
			{
				int nNumber = _ttoi(iter->strParameterName.Mid(15, 2));

				m_pBUsingOutputArray[nNumber] = _ttoi(iter->strParameterValue);
			}
			else if(iter->strParameterName.Find(_T("Input No. ")) >= 0)
			{
				int nNumber = _ttoi(iter->strParameterName.Mid(10, 2));

				m_pStrInputNameArray[nNumber] = iter->strParameterValue;
			}
			else if(iter->strParameterName.Find(_T("Output No. ")) >= 0)
			{
				int nNumber = _ttoi(iter->strParameterName.Mid(11, 2));

				m_pStrOutputNameArray[nNumber] = iter->strParameterValue;
			}
			else
				bReturn = false;
		}

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxlDio::ReadOutBit(int nBit)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(nBit >= AXL_DIO_OUT_PORT || nBit < 0)
			break;

		int nModuleID = _ttoi(GetDeviceID());

		DWORD dwMode = 0;

		if(AxdoReadOutportBit(nModuleID, nBit, &dwMode) != AXT_RT_SUCCESS)
			break;

		bReturn = dwMode;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxlDio::ReadInBit(int nBit)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(nBit >= AXL_DIO_OUT_PORT || nBit < 0)
			break;

		int nModuleID = _ttoi(GetDeviceID());

		DWORD dwMode = 0;

		if(AxdiReadInportBit(nModuleID, nBit, &dwMode) != AXT_RT_SUCCESS)
			break;

		bReturn = dwMode;
	}
	while(false);

	return bReturn;
}

BYTE CDeviceAxlDio::ReadOutByte()
{
	BYTE cReturn = 0;

	do
	{
		if(!IsInitialized())
			break;

		int nModuleID = _ttoi(GetDeviceID());

		DWORD dwMode = 0;

		if(AxdoReadOutportByte(nModuleID, 0, &dwMode) != AXT_RT_SUCCESS)
			break;

		cReturn = dwMode;
	}
	while(false);

	return cReturn;
}

BYTE CDeviceAxlDio::ReadInByte()
{
	BYTE cReturn = 0;

	do
	{
		if(!IsInitialized())
			break;

		int nModuleID = _ttoi(GetDeviceID());

		DWORD dwMode = 0;

		if(AxdiReadInportByte(nModuleID, 0, &dwMode) != AXT_RT_SUCCESS)
			break;

		cReturn = dwMode;
	}
	while(false);

	return cReturn;
}

WORD CDeviceAxlDio::ReadOutWord()
{
	WORD wReturn = 0;

	do
	{
		if(!IsInitialized())
			break;

		int nModuleID = _ttoi(GetDeviceID());

		DWORD dwMode = 0;

		if(AxdoReadOutportWord(nModuleID, 0, &dwMode) != AXT_RT_SUCCESS)
			break;

		wReturn = dwMode;
	}
	while(false);

	return wReturn;
}

WORD CDeviceAxlDio::ReadInWord()
{
	WORD wReturn = 0;

	do
	{
		if(!IsInitialized())
			break;

		int nModuleID = _ttoi(GetDeviceID());

		DWORD dwMode = 0;

		if(AxdiReadInportWord(nModuleID, 0, &dwMode) != AXT_RT_SUCCESS)
			break;

		wReturn = dwMode;
	}
	while(false);

	return wReturn;
}

DWORD CDeviceAxlDio::ReadOutDword()
{
	DWORD dwReturn = 0;

	do
	{
		if(!IsInitialized())
			break;

		int nModuleID = _ttoi(GetDeviceID());

		DWORD dwMode = 0;

		if(AxdoReadOutportDword(nModuleID, 0, &dwMode) != AXT_RT_SUCCESS)
			break;

		dwReturn = dwMode;
	}
	while(false);

	return dwReturn;
}

DWORD CDeviceAxlDio::ReadInDword()
{
	DWORD dwReturn = 0;

	do
	{
		if(!IsInitialized())
			break;

		int nModuleID = _ttoi(GetDeviceID());

		DWORD dwMode = 0;

		if(AxdiReadInportDword(nModuleID, 0, &dwMode) != AXT_RT_SUCCESS)
			break;

		dwReturn = dwMode;
	}
	while(false);

	return dwReturn;
}

bool CDeviceAxlDio::WriteOutBit(int nBit, bool bOn)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(nBit >= AXL_DIO_OUT_PORT || nBit < 0)
			break;

		int nModuleID = _ttoi(GetDeviceID());

		if(AxdoWriteOutportBit(nModuleID, nBit, bOn) != AXT_RT_SUCCESS)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxlDio::WriteInBit(int nBit, bool bOn)
{
	return false;
}

bool CDeviceAxlDio::WriteOutByte(BYTE cByte)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		int nModuleID = _ttoi(GetDeviceID());

		if(AxdoWriteOutportByte(nModuleID, 0, cByte) != AXT_RT_SUCCESS)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxlDio::WriteInByte(BYTE cByte)
{
	return false;
}

bool CDeviceAxlDio::WriteOutWord(WORD wWord)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		int nModuleID = _ttoi(GetDeviceID());

		if(AxdoWriteOutportWord(nModuleID, 0, wWord) != AXT_RT_SUCCESS)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxlDio::WriteInWord(WORD wWord)
{
	return false;
}

bool CDeviceAxlDio::WriteOutDword(DWORD dwDword)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		int nModuleID = _ttoi(GetDeviceID());

		if(AxdoWriteOutportDword(nModuleID, 0, dwDword) != AXT_RT_SUCCESS)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxlDio::WriteInDword(DWORD dwDword)
{
	return false;
}

bool CDeviceAxlDio::ClearOutPort()
{
	return WriteOutDword(0);
}

bool CDeviceAxlDio::ClearInPort()
{
	return false;
}

long CDeviceAxlDio::GetOutputPortCount()
{
	return AXL_DIO_OUT_PORT;
}

long CDeviceAxlDio::GetInputPortCount()
{
	return AXL_DIO_IN_PORT;
}

bool CDeviceAxlDio::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = false;

	bool bFoundID = false;

	bool bUpdateInterfaec = true;

	CString strMessage;

	do
	{
		switch(nParam)
		{
		case EDeviceParameterAxlDio_DeviceID:
			{
				bReturn = !SetDeviceID(strValue);
				bFoundID = true;
			}
			break;
		case EDeviceParameterAxlDio_UseClearInitialize:
			{
				bReturn = !SetUseClearInitalize(_ttoi(strValue));
				bFoundID = true;
				bUpdateInterfaec = false;
			}
			break;
		case EDeviceParameterAxlDio_UseClearTerminate:
			{
				bReturn = !SetUseClearTerminate(_ttoi(strValue));
				bFoundID = true;
				bUpdateInterfaec = false;
			}
			break;
		case EDeviceParameterAxlDio_OpenNoReset:
			{
				bReturn = !SetOpenNoHardware(_ttoi(strValue));
				bFoundID = true;
				bUpdateInterfaec = false;
			}
			break;
		case EDeviceParameterAxlDio_UseInputNo0:
		case EDeviceParameterAxlDio_UseInputNo1:
		case EDeviceParameterAxlDio_UseInputNo2:
		case EDeviceParameterAxlDio_UseInputNo3:
		case EDeviceParameterAxlDio_UseInputNo4:
		case EDeviceParameterAxlDio_UseInputNo5:
		case EDeviceParameterAxlDio_UseInputNo6:
		case EDeviceParameterAxlDio_UseInputNo7:
		case EDeviceParameterAxlDio_UseInputNo8:
		case EDeviceParameterAxlDio_UseInputNo9:
		case EDeviceParameterAxlDio_UseInputNo10:
		case EDeviceParameterAxlDio_UseInputNo11:
		case EDeviceParameterAxlDio_UseInputNo12:
		case EDeviceParameterAxlDio_UseInputNo13:
		case EDeviceParameterAxlDio_UseInputNo14:
		case EDeviceParameterAxlDio_UseInputNo15:
		case EDeviceParameterAxlDio_UseInputNo16:
		case EDeviceParameterAxlDio_UseInputNo17:
		case EDeviceParameterAxlDio_UseInputNo18:
		case EDeviceParameterAxlDio_UseInputNo19:
		case EDeviceParameterAxlDio_UseInputNo20:
		case EDeviceParameterAxlDio_UseInputNo21:
		case EDeviceParameterAxlDio_UseInputNo22:
		case EDeviceParameterAxlDio_UseInputNo23:
		case EDeviceParameterAxlDio_UseInputNo24:
		case EDeviceParameterAxlDio_UseInputNo25:
		case EDeviceParameterAxlDio_UseInputNo26:
		case EDeviceParameterAxlDio_UseInputNo27:
		case EDeviceParameterAxlDio_UseInputNo28:
		case EDeviceParameterAxlDio_UseInputNo29:
		case EDeviceParameterAxlDio_UseInputNo30:
		case EDeviceParameterAxlDio_UseInputNo31:
			{
				int nPreValue = _ttoi(GetParamValue(nParam));
				int nValue = _ttoi(strValue);

				if(SetParamValue(nParam, strValue))
				{
					m_pBUsingInputArray[nParam - EDeviceParameterAxlDio_UseInputNo0] = nValue;
					
					bReturn = SaveSettings(nParam);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlDio[nParam], g_lpszSwitch[nPreValue], g_lpszSwitch[nValue]);
			}
			break;
		case EDeviceParameterAxlDio_UseOutputNo0:
		case EDeviceParameterAxlDio_UseOutputNo1:
		case EDeviceParameterAxlDio_UseOutputNo2:
		case EDeviceParameterAxlDio_UseOutputNo3:
		case EDeviceParameterAxlDio_UseOutputNo4:
		case EDeviceParameterAxlDio_UseOutputNo5:
		case EDeviceParameterAxlDio_UseOutputNo6:
		case EDeviceParameterAxlDio_UseOutputNo7:
		case EDeviceParameterAxlDio_UseOutputNo8:
		case EDeviceParameterAxlDio_UseOutputNo9:
		case EDeviceParameterAxlDio_UseOutputNo10:
		case EDeviceParameterAxlDio_UseOutputNo11:
		case EDeviceParameterAxlDio_UseOutputNo12:
		case EDeviceParameterAxlDio_UseOutputNo13:
		case EDeviceParameterAxlDio_UseOutputNo14:
		case EDeviceParameterAxlDio_UseOutputNo15:
		case EDeviceParameterAxlDio_UseOutputNo16:
		case EDeviceParameterAxlDio_UseOutputNo17:
		case EDeviceParameterAxlDio_UseOutputNo18:
		case EDeviceParameterAxlDio_UseOutputNo19:
		case EDeviceParameterAxlDio_UseOutputNo20:
		case EDeviceParameterAxlDio_UseOutputNo21:
		case EDeviceParameterAxlDio_UseOutputNo22:
		case EDeviceParameterAxlDio_UseOutputNo23:
		case EDeviceParameterAxlDio_UseOutputNo24:
		case EDeviceParameterAxlDio_UseOutputNo25:
		case EDeviceParameterAxlDio_UseOutputNo26:
		case EDeviceParameterAxlDio_UseOutputNo27:
		case EDeviceParameterAxlDio_UseOutputNo28:
		case EDeviceParameterAxlDio_UseOutputNo29:
		case EDeviceParameterAxlDio_UseOutputNo30:
		case EDeviceParameterAxlDio_UseOutputNo31:
			{
				int nPreValue = _ttoi(GetParamValue(nParam));
				int nValue = _ttoi(strValue);

				if(SetParamValue(nParam, strValue))
				{
					m_pBUsingOutputArray[nParam - EDeviceParameterAxlDio_UseOutputNo0] = nValue;
					
					bReturn = SaveSettings(nParam);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlDio[nParam], g_lpszSwitch[nPreValue], g_lpszSwitch[nValue]);
			}
			break;
		case EDeviceParameterAxlDio_InputNo0Name:
		case EDeviceParameterAxlDio_InputNo1Name:
		case EDeviceParameterAxlDio_InputNo2Name:
		case EDeviceParameterAxlDio_InputNo3Name:
		case EDeviceParameterAxlDio_InputNo4Name:
		case EDeviceParameterAxlDio_InputNo5Name:
		case EDeviceParameterAxlDio_InputNo6Name:
		case EDeviceParameterAxlDio_InputNo7Name:
		case EDeviceParameterAxlDio_InputNo8Name:
		case EDeviceParameterAxlDio_InputNo9Name:
		case EDeviceParameterAxlDio_InputNo10Name:
		case EDeviceParameterAxlDio_InputNo11Name:
		case EDeviceParameterAxlDio_InputNo12Name:
		case EDeviceParameterAxlDio_InputNo13Name:
		case EDeviceParameterAxlDio_InputNo14Name:
		case EDeviceParameterAxlDio_InputNo15Name:
		case EDeviceParameterAxlDio_InputNo16Name:
		case EDeviceParameterAxlDio_InputNo17Name:
		case EDeviceParameterAxlDio_InputNo18Name:
		case EDeviceParameterAxlDio_InputNo19Name:
		case EDeviceParameterAxlDio_InputNo20Name:
		case EDeviceParameterAxlDio_InputNo21Name:
		case EDeviceParameterAxlDio_InputNo22Name:
		case EDeviceParameterAxlDio_InputNo23Name:
		case EDeviceParameterAxlDio_InputNo24Name:
		case EDeviceParameterAxlDio_InputNo25Name:
		case EDeviceParameterAxlDio_InputNo26Name:
		case EDeviceParameterAxlDio_InputNo27Name:
		case EDeviceParameterAxlDio_InputNo28Name:
		case EDeviceParameterAxlDio_InputNo29Name:
		case EDeviceParameterAxlDio_InputNo30Name:
		case EDeviceParameterAxlDio_InputNo31Name:
			{
				CString strPreValue = GetParamValue(nParam);

				if(SetParamValue(nParam, strValue))
				{
					m_pStrInputNameArray[nParam - EDeviceParameterAxlDio_InputNo0Name] = strValue;
					
					bReturn = SaveSettings(nParam);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlDio[nParam], strPreValue, strValue);
			}
			break;
		case EDeviceParameterAxlDio_OutputNo0Name:
		case EDeviceParameterAxlDio_OutputNo1Name:
		case EDeviceParameterAxlDio_OutputNo2Name:
		case EDeviceParameterAxlDio_OutputNo3Name:
		case EDeviceParameterAxlDio_OutputNo4Name:
		case EDeviceParameterAxlDio_OutputNo5Name:
		case EDeviceParameterAxlDio_OutputNo6Name:
		case EDeviceParameterAxlDio_OutputNo7Name:
		case EDeviceParameterAxlDio_OutputNo8Name:
		case EDeviceParameterAxlDio_OutputNo9Name:
		case EDeviceParameterAxlDio_OutputNo10Name:
		case EDeviceParameterAxlDio_OutputNo11Name:
		case EDeviceParameterAxlDio_OutputNo12Name:
		case EDeviceParameterAxlDio_OutputNo13Name:
		case EDeviceParameterAxlDio_OutputNo14Name:
		case EDeviceParameterAxlDio_OutputNo15Name:
		case EDeviceParameterAxlDio_OutputNo16Name:
		case EDeviceParameterAxlDio_OutputNo17Name:
		case EDeviceParameterAxlDio_OutputNo18Name:
		case EDeviceParameterAxlDio_OutputNo19Name:
		case EDeviceParameterAxlDio_OutputNo20Name:
		case EDeviceParameterAxlDio_OutputNo21Name:
		case EDeviceParameterAxlDio_OutputNo22Name:
		case EDeviceParameterAxlDio_OutputNo23Name:
		case EDeviceParameterAxlDio_OutputNo24Name:
		case EDeviceParameterAxlDio_OutputNo25Name:
		case EDeviceParameterAxlDio_OutputNo26Name:
		case EDeviceParameterAxlDio_OutputNo27Name:
		case EDeviceParameterAxlDio_OutputNo28Name:
		case EDeviceParameterAxlDio_OutputNo29Name:
		case EDeviceParameterAxlDio_OutputNo30Name:
		case EDeviceParameterAxlDio_OutputNo31Name:
			{
				CString strPreValue = GetParamValue(nParam);

				if(SetParamValue(nParam, strValue))
				{
					m_pStrOutputNameArray[nParam - EDeviceParameterAxlDio_OutputNo0Name] = strValue;
					
					bReturn = SaveSettings(nParam);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlDio[nParam], strPreValue, strValue);
			}
			break;
		default:
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtofindtheparameter);
			break;
		}

		if(EDeviceParameterAxlDio_InputStatus < nParam && EDeviceParameterAxlDio_Count > nParam)
			AddControls();
	}
	while(false);

	if(!bFoundID)
	{
		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);
	}

	return bReturn;
}

void CDeviceAxlDio::OnBnClickedOutLed(UINT nID)
{
	do
	{
		int nNumber = nID - EDeviceAddEnumeratedControlID_OutputLedStart;
		
		if((CAuthorityManager::GetDeviceControlPermission() <= CAuthorityManager::GetCurrentUserType()) && !CSequenceManager::IsRunning())
		{
			bool bBit = ReadOutBit(nNumber);

			WriteOutBit(nNumber, !bBit);
		}
	}
	while(false);
}

bool CDeviceAxlDio::DoesModuleExist()
{
	bool bReturn = false;

	CString strModuleName;

	do
	{
		strModuleName.Format(_T("AXL.dll"));

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

		strModuleName.Format(_T("EzBasicAxl.dll"));

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

EGetFunction CDeviceAxlDio::GetUseClearInitalize(bool * pParam)
{
	EGetFunction eReturn = EGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAxlDio_UseClearInitialize));

		eReturn = EGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESetFunction CDeviceAxlDio::SetUseClearInitalize(bool bParam)
{
	ESetFunction eReturn = ESetFunction_UnknownError;

	EDeviceParameterAxlDio eSaveNum = EDeviceParameterAxlDio_UseClearInitialize;

	bool bPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = ESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = ESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlDio[eSaveNum], g_lpszSwitch[bPreValue], g_lpszSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGetFunction CDeviceAxlDio::GetUseClearTerminate(bool * pParam)
{
	EGetFunction eReturn = EGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAxlDio_UseClearTerminate));

		eReturn = EGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESetFunction CDeviceAxlDio::SetUseClearTerminate(bool bParam)
{
	ESetFunction eReturn = ESetFunction_UnknownError;

	EDeviceParameterAxlDio eSaveNum = EDeviceParameterAxlDio_UseClearTerminate;

	bool bPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = ESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = ESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlDio[eSaveNum], g_lpszSwitch[bPreValue], g_lpszSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGetFunction CDeviceAxlDio::GetOpenNoHardware(bool * pParam)
{
	EGetFunction eReturn = EGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAxlDio_OpenNoReset));

		eReturn = EGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESetFunction CDeviceAxlDio::SetOpenNoHardware(bool bParam)
{
	ESetFunction eReturn = ESetFunction_UnknownError;

	EDeviceParameterAxlDio eSaveNum = EDeviceParameterAxlDio_OpenNoReset;

	bool bPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = ESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = ESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlDio[eSaveNum], g_lpszSwitch[bPreValue], g_lpszSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

#endif