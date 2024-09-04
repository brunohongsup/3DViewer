#include "stdafx.h"

#include "DeviceEziPlusEDio.h"

#ifdef USING_DEVICE

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

#include "../Libraries/Includes/EziServo2PlusE/COMM_Define.h"
#include "../Libraries/Includes/EziServo2PlusE/MOTION_DEFINE.h"
#include "../Libraries/Includes/EziServo2PlusE/FAS_EziMOTIONPlusE.h"
#include "../Libraries/Includes/EziServo2PlusE/ReturnCodes_Define.h"

// ezimotionpluse.dll
#pragma comment(lib, COMMONLIB_PREFIX "EziServo2PlusE/EziMOTIONPlusE.lib")

using namespace Ravid;
using namespace Ravid::Device;
using namespace Ravid::Database;
using namespace Ravid::Framework;
using namespace Ravid::Algorithms;
using namespace Ravid::Miscellaneous;

enum
{
	OutputPort = 16,
	InputPort = 16,
};

IMPLEMENT_DYNAMIC(CDeviceEziPlusEDio, CDeviceDio)

BEGIN_MESSAGE_MAP(CDeviceEziPlusEDio, CDeviceDio)
	ON_COMMAND_RANGE(EDeviceAddEnumeratedControlID_OutputLedStart, EDeviceAddEnumeratedControlID_OutputLedEnd, OnBnClickedOutLed)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static LPCTSTR g_lpszParamEziPlusEDio[EDeviceParameterEziPlusEDio_Count] =
{
	_T("DeviceID"),
	_T("IP Address"),
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
};

static LPCTSTR g_lpszEziPlusEDioSwitch[EDeviceDIOSwitch_Count] =
{
	_T("Off"),
	_T("On")
};

CDeviceEziPlusEDio::CDeviceEziPlusEDio()
{
}


CDeviceEziPlusEDio::~CDeviceEziPlusEDio()
{
	Terminate();
}

EDeviceInitializeResult CDeviceEziPlusEDio::Initialize()
{
	EDeviceInitializeResult eReturn = EDeviceInitializeResult_UnknownError;

	CString strMessage, strStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("EziServo2PlusE"));

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

		int nDeviceID = _ttoi(GetDeviceID());

		CString strIPAddress;
		if(GetIPAddress(&strIPAddress))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("IP Address"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		BYTE arrForm[4] = { 0, };

		for(int i = 0; i < 3; ++i)
		{
			int nPos = strIPAddress.Find('.');
			arrForm[i] = _ttoi(strIPAddress.Left(nPos));
			strIPAddress = strIPAddress.Right(strIPAddress.GetLength() - nPos - 1);
		}
		arrForm[3] = _ttoi(strIPAddress);

		if(!PE::FAS_Connect(arrForm[0], arrForm[1], arrForm[2], arrForm[3], nDeviceID))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("IP Address"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		m_bIsInitialized = true;

		ClearOutPort();

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

EDeviceTerminateResult CDeviceEziPlusEDio::Terminate()
{
	EDeviceTerminateResult eReturn = EDeviceTerminateResult_UnknownError;

	CString strMessage;

	SetStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("EziServo2PlusE"));
		
		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);

		return EDeviceTerminateResult_NotFoundApiError;
	}

	do
	{
		ClearOutPort();
		
		if(IsInitialized())
			PE::FAS_Close(_ttoi(GetDeviceID()));

		strMessage = CMultiLanguageManager::GetString(ELanguageParameter_TheDevicehasbeensuccessfullyterminated);

		eReturn = EDeviceTerminateResult_OK;

		m_bIsInitialized = false;

		CEventHandlerManager::BroadcastOnDeviceTerminated(this);
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceEziPlusEDio::LoadSettings()
{
	bool bReturn = false;

	m_vctParameterFieldConfigurations.clear();

	do
	{
		AddParameterFieldConfigurations(EDeviceParameterEziPlusEDio_DeviceID, g_lpszParamEziPlusEDio[EDeviceParameterEziPlusEDio_DeviceID], _T("0"), EParameterFieldType_Edit);
		AddParameterFieldConfigurations(EDeviceParameterEziPlusEDio_IPAddress, g_lpszParamEziPlusEDio[EDeviceParameterEziPlusEDio_IPAddress], _T("192.168.0.3"), EParameterFieldType_Edit);
		AddParameterFieldConfigurations(EDeviceParameterEziPlusEDio_InputStatus, g_lpszParamEziPlusEDio[EDeviceParameterEziPlusEDio_InputStatus], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);

		for(int i = 0; i < InputPort; ++i)
			AddParameterFieldConfigurations(EDeviceParameterEziPlusEDio_UseInputNo0 + i, g_lpszParamEziPlusEDio[EDeviceParameterEziPlusEDio_UseInputNo0 + i], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterEziPlusEDio_OutputStatus, g_lpszParamEziPlusEDio[EDeviceParameterEziPlusEDio_OutputStatus], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);

		for(int i = 0; i < OutputPort; ++i)
			AddParameterFieldConfigurations(EDeviceParameterEziPlusEDio_UseOutputNo0 + i, g_lpszParamEziPlusEDio[EDeviceParameterEziPlusEDio_UseOutputNo0 + i], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterEziPlusEDio_InputName, g_lpszParamEziPlusEDio[EDeviceParameterEziPlusEDio_InputName], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);

		for(int i = 0; i < InputPort; ++i)
			AddParameterFieldConfigurations(EDeviceParameterEziPlusEDio_InputNo0Name + i, g_lpszParamEziPlusEDio[EDeviceParameterEziPlusEDio_InputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterEziPlusEDio_OutputName, g_lpszParamEziPlusEDio[EDeviceParameterEziPlusEDio_OutputName], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);

		for(int i = 0; i < OutputPort; ++i)
			AddParameterFieldConfigurations(EDeviceParameterEziPlusEDio_OutputNo0Name + i, g_lpszParamEziPlusEDio[EDeviceParameterEziPlusEDio_OutputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);

		bReturn = __super::LoadSettings();

		AllocateIO(InputPort, OutputPort);

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
			else if(iter->strParameterName.Find(_T("Invert")) >= 0)
				m_bInvert = _ttoi(iter->strParameterValue);
			else
				bReturn = false;
		}

		bReturn = true;
	}
	while(false);

	return bReturn;
}

EEziPlusEDioGetFunction CDeviceEziPlusEDio::GetIPAddress(CString* pParam)
{
	EEziPlusEDioGetFunction eReturn = EEziPlusEDioGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziPlusEDioGetFunction_NullptrError;
			break;
		}

		*pParam = GetParamValue(EDeviceParameterEziPlusEDio_IPAddress);

		eReturn = EEziPlusEDioGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziPlusEDioSetFunction CDeviceEziPlusEDio::SetIPAddress(CString strParam)
{
	EEziPlusEDioSetFunction eReturn = EEziPlusEDioSetFunction_UnknownError;

	EDeviceParameterEziPlusEDio eSaveNum = EDeviceParameterEziPlusEDio_IPAddress;

	CString strPreValue = GetParamValue(eSaveNum);

	do
	{
		if(IsInitialized())
		{
			eReturn = EEziPlusEDioSetFunction_AlreadyInitializedError;
			break;
		}

		if(!SetParamValue(eSaveNum, strParam))
		{
			eReturn = EEziPlusEDioSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziPlusEDioSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziPlusEDioSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziPlusEDio[eSaveNum], strPreValue, strParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceEziPlusEDio::ReadOutBit(int nBit)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		DWORD dwIO = 0;
		DWORD dwIOStatus = 0;

		Lock();

		if(PE::FAS_GetOutput(_ttoi(GetDeviceID()), &dwIO, &dwIOStatus) == FMM_OK)
			bReturn = (dwIO >> nBit) & 0x01;

		Unlock();
	}
	while(false);

	return bReturn;
}

bool CDeviceEziPlusEDio::ReadInBit(int nBit)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		DWORD dwIO = 0;
		DWORD dwIOStatus = 0;

		Lock();

		if(PE::FAS_GetInput(_ttoi(GetDeviceID()), &dwIO, &dwIOStatus) == FMM_OK)
			bReturn = (dwIO >> nBit) & 0x01;

		Unlock();
	}
	while(false);

	return bReturn;
}

BYTE CDeviceEziPlusEDio::ReadOutByte()
{
	BYTE cReturn = 0;

	do
	{
		if(!IsInitialized())
			break;
	}
	while(false);

	return cReturn;
}

BYTE CDeviceEziPlusEDio::ReadInByte()
{
	BYTE cReturn = 0;

	do
	{
		if(!IsInitialized())
			break;
	}
	while(false);

	return cReturn;
}

WORD CDeviceEziPlusEDio::ReadOutWord()
{
	WORD wReturn = 0;

	do
	{
		if(!IsInitialized())
			break;
	}
	while(false);

	return wReturn;
}

WORD CDeviceEziPlusEDio::ReadInWord()
{
	WORD wReturn = 0;

	do
	{
		if(!IsInitialized())
			break;
	}
	while(false);

	return wReturn;
}

DWORD CDeviceEziPlusEDio::ReadOutDword()
{
	DWORD dwReturn = 0;

	do
	{
		if(!IsInitialized())
			break;
	}
	while(false);

	return dwReturn;
}

DWORD CDeviceEziPlusEDio::ReadInDword()
{
	DWORD dwReturn = 0;

	do
	{
		if(!IsInitialized())
			break;
	}
	while(false);

	return dwReturn;
}

bool CDeviceEziPlusEDio::WriteOutBit(int nBit, bool bOn)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		DWORD dwBitManOn = 0;
		DWORD dwBitManOff = 0;

		if(bOn)
			dwBitManOn = 1 << nBit;
		else
			dwBitManOff = 1 << nBit;

		Lock();

		if(PE::FAS_SetOutput(_ttoi(GetDeviceID()), dwBitManOn, dwBitManOff) == FMM_OK)
			bReturn = true;

		Unlock();
	}
	while(false);

	return bReturn;
}

bool CDeviceEziPlusEDio::WriteInBit(int nBit, bool bOn)
{
	return false;
}

bool CDeviceEziPlusEDio::WriteOutByte(BYTE cByte)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;
	}
	while(false);

	return bReturn;
}

bool CDeviceEziPlusEDio::WriteInByte(BYTE cByte)
{
	return false;
}

bool CDeviceEziPlusEDio::WriteOutWord(WORD wWord)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;
	}
	while(false);

	return bReturn;
}

bool CDeviceEziPlusEDio::WriteInWord(WORD wWord)
{
	return false;
}

bool CDeviceEziPlusEDio::WriteOutDword(DWORD dwDword)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;
	}
	while(false);

	return bReturn;
}

bool CDeviceEziPlusEDio::WriteInDword(DWORD dwDword)
{
	return false;
}

bool CDeviceEziPlusEDio::ClearOutPort()
{
	bool bReturn = false;

	do 
	{
		bReturn = true;

		for(int i = 0; i < OutputPort; ++i)
			bReturn &= WriteOutBit(i, false);
	} 
	while(false);

	return bReturn;
}

bool CDeviceEziPlusEDio::ClearInPort()
{
	return false;
}

long CDeviceEziPlusEDio::GetOutputPortCount()
{
	return OutputPort;
}

long CDeviceEziPlusEDio::GetInputPortCount()
{
	return InputPort;
}

bool CDeviceEziPlusEDio::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = false;

	bool bFoundID = false;

	CString strMessage;

	do
	{
		switch(nParam)
		{
		case EDeviceParameterEziPlusEDio_DeviceID:
			bReturn = !SetDeviceID(strValue);
			bFoundID = true;
			break;
		case EDeviceParameterEziPlusEDio_IPAddress:
			bReturn = !SetIPAddress(strValue);
			bFoundID = true;
			break;
		case EDeviceParameterEziPlusEDio_UseInputNo0:
		case EDeviceParameterEziPlusEDio_UseInputNo1:
		case EDeviceParameterEziPlusEDio_UseInputNo2:
		case EDeviceParameterEziPlusEDio_UseInputNo3:
		case EDeviceParameterEziPlusEDio_UseInputNo4:
		case EDeviceParameterEziPlusEDio_UseInputNo5:
		case EDeviceParameterEziPlusEDio_UseInputNo6:
		case EDeviceParameterEziPlusEDio_UseInputNo7:
		case EDeviceParameterEziPlusEDio_UseInputNo8:
		case EDeviceParameterEziPlusEDio_UseInputNo9:
		case EDeviceParameterEziPlusEDio_UseInputNo10:
		case EDeviceParameterEziPlusEDio_UseInputNo11:
		case EDeviceParameterEziPlusEDio_UseInputNo12:
		case EDeviceParameterEziPlusEDio_UseInputNo13:
		case EDeviceParameterEziPlusEDio_UseInputNo14:
		case EDeviceParameterEziPlusEDio_UseInputNo15:
			{
				int nPreValue = _ttoi(GetParamValue(nParam));
				int nValue = _ttoi(strValue);

				if(SetParamValue(nParam, strValue))
				{
					m_pBUsingInputArray[nParam - EDeviceParameterEziPlusEDio_UseInputNo0] = nValue;

					bReturn = SaveSettings(nParam);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziPlusEDio[nParam], g_lpszEziPlusEDioSwitch[nPreValue], g_lpszEziPlusEDioSwitch[nValue]);
			}
			break;
		case EDeviceParameterEziPlusEDio_UseOutputNo0:
		case EDeviceParameterEziPlusEDio_UseOutputNo1:
		case EDeviceParameterEziPlusEDio_UseOutputNo2:
		case EDeviceParameterEziPlusEDio_UseOutputNo3:
		case EDeviceParameterEziPlusEDio_UseOutputNo4:
		case EDeviceParameterEziPlusEDio_UseOutputNo5:
		case EDeviceParameterEziPlusEDio_UseOutputNo6:
		case EDeviceParameterEziPlusEDio_UseOutputNo7:
		case EDeviceParameterEziPlusEDio_UseOutputNo8:
		case EDeviceParameterEziPlusEDio_UseOutputNo9:
		case EDeviceParameterEziPlusEDio_UseOutputNo10:
		case EDeviceParameterEziPlusEDio_UseOutputNo11:
		case EDeviceParameterEziPlusEDio_UseOutputNo12:
		case EDeviceParameterEziPlusEDio_UseOutputNo13:
		case EDeviceParameterEziPlusEDio_UseOutputNo14:
		case EDeviceParameterEziPlusEDio_UseOutputNo15:
			{
				int nPreValue = _ttoi(GetParamValue(nParam));
				int nValue = _ttoi(strValue);

				if(SetParamValue(nParam, strValue))
				{
					m_pBUsingOutputArray[nParam - EDeviceParameterEziPlusEDio_UseOutputNo0] = nValue;

					bReturn = SaveSettings(nParam);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziPlusEDio[nParam], g_lpszEziPlusEDioSwitch[nPreValue], g_lpszEziPlusEDioSwitch[nValue]);
			}
			break;
		case EDeviceParameterEziPlusEDio_InputNo0Name:
		case EDeviceParameterEziPlusEDio_InputNo1Name:
		case EDeviceParameterEziPlusEDio_InputNo2Name:
		case EDeviceParameterEziPlusEDio_InputNo3Name:
		case EDeviceParameterEziPlusEDio_InputNo4Name:
		case EDeviceParameterEziPlusEDio_InputNo5Name:
		case EDeviceParameterEziPlusEDio_InputNo6Name:
		case EDeviceParameterEziPlusEDio_InputNo7Name:
		case EDeviceParameterEziPlusEDio_InputNo8Name:
		case EDeviceParameterEziPlusEDio_InputNo9Name:
		case EDeviceParameterEziPlusEDio_InputNo10Name:
		case EDeviceParameterEziPlusEDio_InputNo11Name:
		case EDeviceParameterEziPlusEDio_InputNo12Name:
		case EDeviceParameterEziPlusEDio_InputNo13Name:
		case EDeviceParameterEziPlusEDio_InputNo14Name:
		case EDeviceParameterEziPlusEDio_InputNo15Name:
			{
				CString strPreValue = GetParamValue(nParam);

				if(SetParamValue(nParam, strValue))
				{
					m_pStrInputNameArray[nParam - EDeviceParameterEziPlusEDio_InputNo0Name] = strValue;

					bReturn = SaveSettings(nParam);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziPlusEDio[nParam], strPreValue, strValue);
			}
			break;
		case EDeviceParameterEziPlusEDio_OutputNo0Name:
		case EDeviceParameterEziPlusEDio_OutputNo1Name:
		case EDeviceParameterEziPlusEDio_OutputNo2Name:
		case EDeviceParameterEziPlusEDio_OutputNo3Name:
		case EDeviceParameterEziPlusEDio_OutputNo4Name:
		case EDeviceParameterEziPlusEDio_OutputNo5Name:
		case EDeviceParameterEziPlusEDio_OutputNo6Name:
		case EDeviceParameterEziPlusEDio_OutputNo7Name:
		case EDeviceParameterEziPlusEDio_OutputNo8Name:
		case EDeviceParameterEziPlusEDio_OutputNo9Name:
		case EDeviceParameterEziPlusEDio_OutputNo10Name:
		case EDeviceParameterEziPlusEDio_OutputNo11Name:
		case EDeviceParameterEziPlusEDio_OutputNo12Name:
		case EDeviceParameterEziPlusEDio_OutputNo13Name:
		case EDeviceParameterEziPlusEDio_OutputNo14Name:
		case EDeviceParameterEziPlusEDio_OutputNo15Name:
			{
				CString strPreValue = GetParamValue(nParam);

				if(SetParamValue(nParam, strValue))
				{
					m_pStrOutputNameArray[nParam - EDeviceParameterEziPlusEDio_OutputNo0Name] = strValue;

					bReturn = SaveSettings(nParam);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziPlusEDio[nParam], strPreValue, strValue);
			}
			break;
		default:
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtofindtheparameter);
			break;
		}

		if(EDeviceParameterEziPlusEDio_InputStatus < nParam && EDeviceParameterEziPlusEDio_Count > nParam)
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

void CDeviceEziPlusEDio::OnBnClickedOutLed(UINT nID)
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

bool CDeviceEziPlusEDio::DoesModuleExist()
{
	bool bReturn = false;

	CString strModuleName;

	do
	{
		strModuleName.Format(_T("EziMOTIONPlusE.dll"));

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

#endif