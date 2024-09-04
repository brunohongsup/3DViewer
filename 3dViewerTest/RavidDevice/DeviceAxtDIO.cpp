#include "stdafx.h"
#include "DeviceAxtDio.h"

#ifdef USING_DEVICE

#include "DeviceAxtMotionFS10.h"
#include "DeviceAxtMotionFS20.h"

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

#include "../Libraries/Includes/AxtLib/AxtLIB.h"
#include "../Libraries/Includes/AxtLib/AxtDIO.h"

// axllib.dll
#pragma comment(lib, COMMONLIB_PREFIX "AxtLib/AXTLib.lib")

using namespace Ravid;
using namespace Ravid::Device;
using namespace Ravid::Database;
using namespace Ravid::Framework;
using namespace Ravid::Algorithms;
using namespace Ravid::Miscellaneous;

enum
{
	AXT_DIO_IN_PORT = 16,
	AXT_DIO_OUT_PORT = 16,
};
enum
{
	AXT_MODULE_MIN = 0,
	AXT_MODULE_MAX = 99,
};

IMPLEMENT_DYNAMIC(CDeviceAxtDio, CDeviceDio)

BEGIN_MESSAGE_MAP(CDeviceAxtDio, CDeviceDio)
	ON_COMMAND_RANGE(EDeviceAddEnumeratedControlID_OutputLedStart, EDeviceAddEnumeratedControlID_OutputLedEnd, OnBnClickedOutLed)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static LPCTSTR g_lpszParamAxtDio[EDeviceParameterAxtDio_Count] =
{
	_T("DeviceID"),
	_T("SubUnitID"),
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

static LPCTSTR g_lpszAxtDioSwitch[EDeviceDIOSwitch_Count] =
{
	_T("Off"),
	_T("On")
};

CDeviceAxtDio::CDeviceAxtDio()
{
}


CDeviceAxtDio::~CDeviceAxtDio()
{
	Terminate();
}

EDeviceInitializeResult CDeviceAxtDio::Initialize()
{
	EDeviceInitializeResult eReturn = EDeviceInitializeResult_UnknownError;

	CString strMessage, strStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("AxtDio"));

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

		long nDeviceID = _ttoi(GetDeviceID());

		if(!AxtIsInitialized())
		{
			if(!AxtInitialize(m_hWnd, NULL))
			{
				strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntopenthelibrary);
				eReturn = EDeviceInitializeResult_AlreadyInitializedError;
				break;
			}
		}
		
		if(!AxtIsInitializedBus(BUSTYPE_PCI))
		{
			if(!AxtOpenDeviceAuto(BUSTYPE_PCI))
			{
				strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntopenthedevice);
				eReturn = EDeviceInitializeResult_NotOpenedDeviceError;
				break;
			}
		}

		UINT8 arrDeviceIDs[AXT_MODULE] = {0,};

		INT16 nModuleCount = AxtGetModuleCounts(nDeviceID, arrDeviceIDs);

		if(!nModuleCount)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtofindthedevice);
			eReturn = EDeviceInitializeResult_NotOpenedDeviceError;
			break;
		}

		if(!DIOIsInitialized())
		{
			if(!InitializeDIO())
			{
				strMessage = CMultiLanguageManager::GetString(ELanguageParameter_CouldntinitializetheIOmodule);
				eReturn = EDeviceInitializeResult_NotInitializeDIOError;
				m_bIsInitialized = false;
				break;
			}
		}

		bool bAlreadyInit = true;

		size_t szDeviceCnt = CDeviceManager::GetDeviceCount();

		for(size_t i = 0; i < szDeviceCnt; ++i)
		{
			bool bMatchDevice = false;

			CDeviceBase* pDevicebase = CDeviceManager::GetDeviceByIndex(i);

			if(!pDevicebase)
				continue;

			if(GetDeviceID().CompareNoCase(pDevicebase->GetDeviceID()))
				continue;

			CDeviceAxtDio* pDio = dynamic_cast<CDeviceAxtDio*>(pDevicebase);

			if(!pDio)
				continue;
			
			if(pDio == this)
				continue;

			if(pDio->GetDeviceID().CompareNoCase(GetDeviceID()))
				continue;

			if(pDio->GetSubUnitID().CompareNoCase(GetSubUnitID()))
				continue;

			if(!pDio->IsInitialized())
				continue;

			bAlreadyInit = false;

			break;
		}

		if(!bAlreadyInit)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Ithasalreadybeeninitializedtoanotherdevice);
			eReturn = EDeviceInitializeResult_OtherDeviceInitError;
			break;
		}

		if(!DIOIsOpenModule(_ttoi(GetSubUnitID())))
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_CouldntopentheIOmodule);
			eReturn = EDeviceInitializeResult_NotInitializeDIOError;
			m_bIsInitialized = false;
			break;
		}

		m_bIsInitialized = true;

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

EDeviceTerminateResult CDeviceAxtDio::Terminate()
{
	EDeviceTerminateResult eReturn = EDeviceTerminateResult_UnknownError;

	CString strMessage;

	SetStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("AxtDio"));

		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);
				
		return EDeviceTerminateResult_NotFoundApiError;
	}

	do 
	{
		ClearOutPort();

		bool bNeedTreminate = true;

		size_t szDeviceCnt = CDeviceManager::GetDeviceCount();

		for(size_t i = 0; i < szDeviceCnt; ++i)
		{
			CDeviceBase* pDevicebase = CDeviceManager::GetDeviceByIndex(i);

			if(!pDevicebase)
				continue;

			CDeviceAxtDio* pDio = dynamic_cast<CDeviceAxtDio*>(pDevicebase);

			if(pDio)
			{
				if(pDio == this)
					continue;

				if(!pDio->IsInitialized())
					continue;
				
				bNeedTreminate = false;

				break;
			}

			CDeviceAxtMotionFS10* pMotionFS10 = dynamic_cast<CDeviceAxtMotionFS10*>(pDevicebase);

			if(pMotionFS10)
			{
				if(!pMotionFS10->IsInitialized())
					continue;

				bNeedTreminate = false;

				break;
			}

			CDeviceAxtMotionFS20* pMotionFS20 = dynamic_cast<CDeviceAxtMotionFS20*>(pDevicebase);

			if(pMotionFS20)
			{
				if(!pMotionFS20->IsInitialized())
					continue;

				bNeedTreminate = false;

				break;
			}
		}

		if(bNeedTreminate)
			AxtClose();

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

bool CDeviceAxtDio::LoadSettings()
{
	bool bReturn = false;

	m_vctParameterFieldConfigurations.clear();

	do
	{
		AddParameterFieldConfigurations(EDeviceParameterAxtDio_DeviceID, g_lpszParamAxtDio[EDeviceParameterAxtDio_DeviceID], _T("0"), EParameterFieldType_Edit);
		AddParameterFieldConfigurations(EDeviceParameterAxtDio_SubUnitID, g_lpszParamAxtDio[EDeviceParameterAxtDio_SubUnitID], _T("0"), EParameterFieldType_Edit);
		AddParameterFieldConfigurations(EDeviceParameterAxtDio_InputStatus, g_lpszParamAxtDio[EDeviceParameterAxtDio_InputStatus], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);

		for(int i = 0; i < AXT_DIO_IN_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterAxtDio_UseInputNo0 + i, g_lpszParamAxtDio[EDeviceParameterAxtDio_UseInputNo0 + i], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 1);
		
		AddParameterFieldConfigurations(EDeviceParameterAxtDio_OutputStatus, g_lpszParamAxtDio[EDeviceParameterAxtDio_OutputStatus], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);

		for(int i = 0; i < AXT_DIO_OUT_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterAxtDio_UseOutputNo0 + i, g_lpszParamAxtDio[EDeviceParameterAxtDio_UseOutputNo0 + i], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 1);
		
		AddParameterFieldConfigurations(EDeviceParameterAxtDio_InputName, g_lpszParamAxtDio[EDeviceParameterAxtDio_InputName], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);

		for(int i = 0; i < AXT_DIO_IN_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterAxtDio_InputNo0Name + i, g_lpszParamAxtDio[EDeviceParameterAxtDio_InputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);
		
		AddParameterFieldConfigurations(EDeviceParameterAxtDio_OutputName, g_lpszParamAxtDio[EDeviceParameterAxtDio_OutputName], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);

		for(int i = 0; i < AXT_DIO_OUT_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterAxtDio_OutputNo0Name + i, g_lpszParamAxtDio[EDeviceParameterAxtDio_OutputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);
		
		bReturn = __super::LoadSettings();

		AllocateIO(AXT_DIO_IN_PORT, AXT_DIO_OUT_PORT);

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

bool CDeviceAxtDio::ReadOutBit(int nBit)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(nBit >= AXT_DIO_OUT_PORT || nBit < 0)
			break;

		long nSubUnitID = _ttoi(GetSubUnitID());

		bReturn = DIOread_outport_bit(nSubUnitID, nBit);
	}
	while(false);

	return bReturn;
}

bool CDeviceAxtDio::ReadInBit(int nBit)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(nBit >= AXT_DIO_IN_PORT || nBit < 0)
			break;

		long nSubUnitID = _ttoi(GetSubUnitID());

		bReturn = DIOread_inport_bit(nSubUnitID, nBit);
	}
	while(false);

	return bReturn;
}

BYTE CDeviceAxtDio::ReadOutByte()
{
	BYTE cReturn = 0;

	do
	{
		if(!IsInitialized())
			break;

		long nSubUnitID = _ttoi(GetSubUnitID());

		cReturn = DIOread_outport_byte(nSubUnitID, 0);
	}
	while(false);

	return cReturn;
}

BYTE CDeviceAxtDio::ReadInByte()
{
	BYTE cReturn = 0;

	do
	{
		if(!IsInitialized())
			break;

		long nSubUnitID = _ttoi(GetSubUnitID());

		cReturn = DIOread_inport_byte(nSubUnitID, 0);
	}
	while(false);

	return cReturn;
}

WORD CDeviceAxtDio::ReadOutWord()
{
	WORD wReturn = 0;

	do
	{
		if(!IsInitialized())
			break;

		long nSubUnitID = _ttoi(GetSubUnitID());

		wReturn = DIOread_outport_word(nSubUnitID, 0);
	}
	while(false);

	return wReturn;
}

WORD CDeviceAxtDio::ReadInWord()
{
	WORD wReturn = 0;

	do
	{
		if(!IsInitialized())
			break;

		long nSubUnitID = _ttoi(GetSubUnitID());

		wReturn = DIOread_inport_word(nSubUnitID, 0);
	}
	while(false);

	return wReturn;
}

DWORD CDeviceAxtDio::ReadOutDword()
{
	DWORD dwReturn = 0;

	do
	{
		if(!IsInitialized())
			break;

		long nSubUnitID = _ttoi(GetSubUnitID());

		dwReturn = DIOread_outport_dword(nSubUnitID, 0);
	}
	while(false);

	return dwReturn;
}

DWORD CDeviceAxtDio::ReadInDword()
{
	DWORD dwReturn = 0;

	do
	{
		if(!IsInitialized())
			break;

		long nSubUnitID = _ttoi(GetSubUnitID());

		dwReturn = DIOread_inport_dword(nSubUnitID, 0);
	}
	while(false);

	return dwReturn;
}

bool CDeviceAxtDio::WriteOutBit(int nBit, bool bOn)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(nBit >= AXT_DIO_OUT_PORT || nBit < 0)
			break;

		long nSubUnitID = _ttoi(GetSubUnitID());

		DIOwrite_outport_bit(nSubUnitID, nBit, bOn);

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxtDio::WriteInBit(int nBit, bool bOn)
{
	return false;
}

bool CDeviceAxtDio::WriteOutByte(BYTE cByte)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		long nSubUnitID = _ttoi(GetSubUnitID());

		DIOwrite_outport_byte(nSubUnitID, 0, cByte);

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxtDio::WriteInByte(BYTE cByte)
{
	return false;
}

bool CDeviceAxtDio::WriteOutWord(WORD wWord)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		long nSubUnitID = _ttoi(GetSubUnitID());

		DIOwrite_outport_word(nSubUnitID, 0, wWord);

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxtDio::WriteInWord(WORD wWord)
{
	return false;
}

bool CDeviceAxtDio::WriteOutDword(DWORD dwDword)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		long nSubUnitID = _ttoi(GetSubUnitID());

		DIOwrite_outport_dword(nSubUnitID, 0, dwDword);

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxtDio::WriteInDword(DWORD dwDword)
{
	return false;
}

bool CDeviceAxtDio::ClearOutPort()
{
	return WriteOutDword(0);
}

bool CDeviceAxtDio::ClearInPort()
{
	return false;
}

long CDeviceAxtDio::GetOutputPortCount()
{
	return AXT_DIO_OUT_PORT;
}

long CDeviceAxtDio::GetInputPortCount()
{
	return AXT_DIO_IN_PORT;
}

bool CDeviceAxtDio::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = false;

	bool bFoundID = false;

	CString strMessage;

	do
	{
		switch(nParam)
		{
		case EDeviceParameterAxtDio_DeviceID:
			{
				bReturn = !SetDeviceID(strValue);

				bFoundID = true;
			}
			break;
		case EDeviceParameterAxtDio_SubUnitID:
			{
				bReturn = !SetSubUnitID(strValue);

				bFoundID = true;
			}
			break;
		case EDeviceParameterAxtDio_UseInputNo0:
		case EDeviceParameterAxtDio_UseInputNo1:
		case EDeviceParameterAxtDio_UseInputNo2:
		case EDeviceParameterAxtDio_UseInputNo3:
		case EDeviceParameterAxtDio_UseInputNo4:
		case EDeviceParameterAxtDio_UseInputNo5:
		case EDeviceParameterAxtDio_UseInputNo6:
		case EDeviceParameterAxtDio_UseInputNo7:
		case EDeviceParameterAxtDio_UseInputNo8:
		case EDeviceParameterAxtDio_UseInputNo9:
		case EDeviceParameterAxtDio_UseInputNo10:
		case EDeviceParameterAxtDio_UseInputNo11:
		case EDeviceParameterAxtDio_UseInputNo12:
		case EDeviceParameterAxtDio_UseInputNo13:
		case EDeviceParameterAxtDio_UseInputNo14:
		case EDeviceParameterAxtDio_UseInputNo15:
			{
				int nPreValue = _ttoi(GetParamValue(nParam));
				int nValue = _ttoi(strValue);

				if(SetParamValue(nParam, strValue))
				{
					m_pBUsingInputArray[nParam - EDeviceParameterAxtDio_UseInputNo0] = nValue;
					
					bReturn = SaveSettings(nParam);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxtDio[nParam], g_lpszAxtDioSwitch[nPreValue], g_lpszAxtDioSwitch[nValue]);
			}
			break;
		case EDeviceParameterAxtDio_UseOutputNo0:
		case EDeviceParameterAxtDio_UseOutputNo1:
		case EDeviceParameterAxtDio_UseOutputNo2:
		case EDeviceParameterAxtDio_UseOutputNo3:
		case EDeviceParameterAxtDio_UseOutputNo4:
		case EDeviceParameterAxtDio_UseOutputNo5:
		case EDeviceParameterAxtDio_UseOutputNo6:
		case EDeviceParameterAxtDio_UseOutputNo7:
		case EDeviceParameterAxtDio_UseOutputNo8:
		case EDeviceParameterAxtDio_UseOutputNo9:
		case EDeviceParameterAxtDio_UseOutputNo10:
		case EDeviceParameterAxtDio_UseOutputNo11:
		case EDeviceParameterAxtDio_UseOutputNo12:
		case EDeviceParameterAxtDio_UseOutputNo13:
		case EDeviceParameterAxtDio_UseOutputNo14:
		case EDeviceParameterAxtDio_UseOutputNo15:
			{
				int nPreValue = _ttoi(GetParamValue(nParam));
				int nValue = _ttoi(strValue);

				if(SetParamValue(nParam, strValue))
				{
					m_pBUsingOutputArray[nParam - EDeviceParameterAxtDio_UseOutputNo0] = nValue;
					
					bReturn = SaveSettings(nParam);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxtDio[nParam], g_lpszAxtDioSwitch[nPreValue], g_lpszAxtDioSwitch[nValue]);
			}
			break;
		case EDeviceParameterAxtDio_InputNo0Name:
		case EDeviceParameterAxtDio_InputNo1Name:
		case EDeviceParameterAxtDio_InputNo2Name:
		case EDeviceParameterAxtDio_InputNo3Name:
		case EDeviceParameterAxtDio_InputNo4Name:
		case EDeviceParameterAxtDio_InputNo5Name:
		case EDeviceParameterAxtDio_InputNo6Name:
		case EDeviceParameterAxtDio_InputNo7Name:
		case EDeviceParameterAxtDio_InputNo8Name:
		case EDeviceParameterAxtDio_InputNo9Name:
		case EDeviceParameterAxtDio_InputNo10Name:
		case EDeviceParameterAxtDio_InputNo11Name:
		case EDeviceParameterAxtDio_InputNo12Name:
		case EDeviceParameterAxtDio_InputNo13Name:
		case EDeviceParameterAxtDio_InputNo14Name:
		case EDeviceParameterAxtDio_InputNo15Name:
			{
				CString strPreValue = GetParamValue(nParam);

				if(SetParamValue(nParam, strValue))
				{
					m_pStrInputNameArray[nParam - EDeviceParameterAxtDio_InputNo0Name] = strValue;
					
					bReturn = SaveSettings(nParam);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxtDio[nParam], strPreValue, strValue);
			}
			break;
		case EDeviceParameterAxtDio_OutputNo0Name:
		case EDeviceParameterAxtDio_OutputNo1Name:
		case EDeviceParameterAxtDio_OutputNo2Name:
		case EDeviceParameterAxtDio_OutputNo3Name:
		case EDeviceParameterAxtDio_OutputNo4Name:
		case EDeviceParameterAxtDio_OutputNo5Name:
		case EDeviceParameterAxtDio_OutputNo6Name:
		case EDeviceParameterAxtDio_OutputNo7Name:
		case EDeviceParameterAxtDio_OutputNo8Name:
		case EDeviceParameterAxtDio_OutputNo9Name:
		case EDeviceParameterAxtDio_OutputNo10Name:
		case EDeviceParameterAxtDio_OutputNo11Name:
		case EDeviceParameterAxtDio_OutputNo12Name:
		case EDeviceParameterAxtDio_OutputNo13Name:
		case EDeviceParameterAxtDio_OutputNo14Name:
		case EDeviceParameterAxtDio_OutputNo15Name:
			{
				CString strPreValue = GetParamValue(nParam);

				if(SetParamValue(nParam, strValue))
				{
					m_pStrOutputNameArray[nParam - EDeviceParameterAxtDio_OutputNo0Name] = strValue;
					
					bReturn = SaveSettings(nParam);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxtDio[nParam], strPreValue, strValue);
			}
			break;
		default:
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtofindtheparameter);
			break;
		}

		if(EDeviceParameterAxtDio_InputStatus < nParam && EDeviceParameterAxtDio_Count > nParam)
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

void CDeviceAxtDio::OnBnClickedOutLed(UINT nID)
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

bool CDeviceAxtDio::DoesModuleExist()
{
	bool bReturn = false;

	CString strModuleName;

	do
	{
		strModuleName.Format(_T("AXTLIB.dll"));

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