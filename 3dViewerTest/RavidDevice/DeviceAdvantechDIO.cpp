#include "stdafx.h"

#include "DeviceAdvantechDIO.h"

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

#include "../Libraries/Includes/Advantech/bdaqctrl.h"

using namespace Ravid;
using namespace Ravid::Device;
using namespace Ravid::Database;
using namespace Ravid::Framework;
using namespace Ravid::Algorithms;
using namespace Ravid::Miscellaneous;

enum
{
	ADX_MODULE_MIN = 0,
	ADX_MODULE_MAX = 99,
};

enum
{
	ADX_DIO_OUT_PORT = 16,
	ADX_DIO_IN_PORT = 16,
};

IMPLEMENT_DYNAMIC(CDeviceAdvantechDio, CDeviceDio)

BEGIN_MESSAGE_MAP(CDeviceAdvantechDio, CDeviceDio)
	ON_COMMAND_RANGE(EDeviceAddEnumeratedControlID_OutputLedStart, EDeviceAddEnumeratedControlID_OutputLedEnd, OnBnClickedOutLed)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static LPCTSTR g_lpszParamAdvantechDio[EDeviceParameterAdvantechDio_Count] =
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

static LPCTSTR g_lpszAdvantechDioSwitch[EDeviceDIOSwitch_Count] =
{
	_T("Off"),
	_T("On")
};

CDeviceAdvantechDio::CDeviceAdvantechDio()
{
}


CDeviceAdvantechDio::~CDeviceAdvantechDio()
{
	Terminate();
}

EDeviceInitializeResult CDeviceAdvantechDio::Initialize()
{
	EDeviceInitializeResult eReturn = EDeviceInitializeResult_UnknownError;

	CString strMessage, strStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));
	
	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("AdvantechDio"));

		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);
		SetStatus(strStatus);

		return EDeviceInitializeResult_NotFoundApiError;
	}

	Automation::BDaq::ICollection<Automation::BDaq::DeviceTreeNode>* pCltDo = nullptr;
	Automation::BDaq::ICollection<Automation::BDaq::DeviceTreeNode>* pCltDi = nullptr;

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

		if(ADX_MODULE_MIN > nDeviceID || ADX_MODULE_MAX < nDeviceID)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_DoesntsupporttheDeviceID);
			eReturn = EDeviceInitializeResult_RangeOfDeviceIDError;
			break;
		}
		
		if(!m_pDoCtrl)
		{
			m_pDoCtrl = Automation::BDaq::InstantDoCtrl_Create();

			if(!m_pDoCtrl)
			{
				strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntcreatetheautomationoutput);
				eReturn = EDeviceInitializeResult_CreateOutputError;
				break;
			}
		}

		if(!m_pDiCtrl)
		{
			m_pDiCtrl = Automation::BDaq::InstantDiCtrl_Create();

			if(!m_pDiCtrl)
			{
				strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntcreatetheautomationinput);
				eReturn = EDeviceInitializeResult_CreateInputError;
				break;
			}
		}

		pCltDo = m_pDoCtrl->getSupportedDevices();

		if(!pCltDo)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntsupporttheoutput);
			eReturn = EDeviceInitializeResult_SupportOutputError;
			break;
		}

		pCltDi = m_pDiCtrl->getSupportedDevices();

		if(!pCltDi)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntsupporttheinput);
			eReturn = EDeviceInitializeResult_SupportInputError;
			break;
		}

		bool bFindDoDevice = false;
		bool bFindDiDevice = false;

		int nOutCount = pCltDo->getCount();
		int nInCount = pCltDi->getCount();

		if(!nOutCount || !nInCount)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_CouldntfindIOport);
			eReturn = EDeviceInitializeResult_UseIOCountError;
			break;
		}

		for(int i = 0; i < nOutCount; ++i)
		{
			if(pCltDo->getItem(i).DeviceNumber != nDeviceID)
				continue;

			bFindDoDevice = true;

			break;			
		}

		if(!bFindDoDevice)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntfindthedeviceofoutput);
			eReturn = EDeviceInitializeResult_NotFoundDeviceError;
			break;
		}

		for(int i = 0; i < nInCount; ++i)
		{
			if(pCltDi->getItem(i).DeviceNumber != nDeviceID)
				continue;

			bFindDiDevice = true;

			break;
		}

		if(!bFindDiDevice)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntfindthedeviceofinput);
			eReturn = EDeviceInitializeResult_NotFoundDeviceError;
			break;
		}
		
		Automation::BDaq::DeviceInformation devInfo(nDeviceID);

		if(m_pDoCtrl->setSelectedDevice(devInfo) != Automation::BDaq::Success)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtoselectdeviceofoutput);
			eReturn = EDeviceInitializeResult_SelectOutputError;
			break;
		}

		if(m_pDiCtrl->setSelectedDevice(devInfo) != Automation::BDaq::Success)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtoselectdeviceofinput);
			eReturn = EDeviceInitializeResult_SelectInputError;
			break;
		}
		
		DWORD dwDword = 0;

		Automation::BDaq::ErrorCode eCode = Automation::BDaq::Success;

		Lock();
		eCode = m_pDoCtrl->Write(0, sizeof(dwDword), (BYTE*)&dwDword);
		Unlock();

		if(eCode != Automation::BDaq::Success)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Clear output"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}
		
		strMessage = CMultiLanguageManager::GetString(ELanguageParameter_TheDevicehasbeensuccessfullyinitialized);

		strStatus = CMultiLanguageManager::GetString(ELanguageParameter_Initialized);

		m_bIsInitialized = true;

		eReturn = EDeviceInitializeResult_OK;
				
		CEventHandlerManager::BroadcastOnDeviceInitialized(this);
	}
	while(false);

	if(pCltDo)
	{
		pCltDo->Dispose();
		pCltDo = nullptr;
	}

	if(pCltDi)
	{
		pCltDi->Dispose();
		pCltDi = nullptr;
	}

	if(!IsInitialized())
		Terminate();

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	SetStatus(strStatus);

	return eReturn;
}

EDeviceTerminateResult CDeviceAdvantechDio::Terminate()
{
	EDeviceTerminateResult eReturn = EDeviceTerminateResult_UnknownError;

	CString strMessage;

	SetStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("AdvantechDio"));

		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);

		return EDeviceTerminateResult_NotFoundApiError;
	}

	do
	{
		ClearOutPort();

		if(m_pDoCtrl)
		{
			m_pDoCtrl->Cleanup();
			m_pDoCtrl->Dispose();
			m_pDoCtrl = nullptr;
		}

		if(m_pDiCtrl)
		{
			m_pDiCtrl->Cleanup();
			m_pDiCtrl->Dispose();
			m_pDiCtrl = nullptr;
		}

		strMessage = CMultiLanguageManager::GetString(ELanguageParameter_TheDevicehasbeensuccessfullyterminated);

		m_bIsInitialized = false;

		eReturn = EDeviceTerminateResult_OK;
				
		CEventHandlerManager::BroadcastOnDeviceTerminated(this);
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceAdvantechDio::LoadSettings()
{
	bool bReturn = false;

	m_vctParameterFieldConfigurations.clear();

	do
	{
		AddParameterFieldConfigurations(EDeviceParameterAdvantechDio_DeviceID, g_lpszParamAdvantechDio[EDeviceParameterAdvantechDio_DeviceID], _T("0"), EParameterFieldType_Edit);
		AddParameterFieldConfigurations(EDeviceParameterAdvantechDio_InputStatus, g_lpszParamAdvantechDio[EDeviceParameterAdvantechDio_InputStatus], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);

		for(int i = 0; i < ADX_DIO_IN_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterAdvantechDio_UseInputNo0 + i, g_lpszParamAdvantechDio[EDeviceParameterAdvantechDio_UseInputNo0 + i], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterAdvantechDio_OutputStatus, g_lpszParamAdvantechDio[EDeviceParameterAdvantechDio_OutputStatus], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);

		for(int i = 0; i < ADX_DIO_OUT_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterAdvantechDio_UseOutputNo0 + i, g_lpszParamAdvantechDio[EDeviceParameterAdvantechDio_UseOutputNo0 + i], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterAdvantechDio_InputName, g_lpszParamAdvantechDio[EDeviceParameterAdvantechDio_InputName], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);

		for(int i = 0; i < ADX_DIO_IN_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterAdvantechDio_InputNo0Name + i, g_lpszParamAdvantechDio[EDeviceParameterAdvantechDio_InputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterAdvantechDio_OutputName, g_lpszParamAdvantechDio[EDeviceParameterAdvantechDio_OutputName], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);

		for(int i = 0; i < ADX_DIO_OUT_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterAdvantechDio_OutputNo0Name + i, g_lpszParamAdvantechDio[EDeviceParameterAdvantechDio_OutputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);
		
		bReturn = __super::LoadSettings();

		AllocateIO(ADX_DIO_IN_PORT, ADX_DIO_OUT_PORT);

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

bool CDeviceAdvantechDio::ReadOutBit(int nBit)
{
	bool bReturn = false;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		if(nBit >= ADX_DIO_OUT_PORT || nBit < 0)
			break;

		BYTE cData = 0;

		long nPort = nBit / 8;
		long nReadBit = nBit - (nPort * 8);

		if(m_pDoCtrl->ReadBit(nPort, nReadBit, &cData) != Automation::BDaq::Success)
			break;

		bReturn = (bool)cData;
	}
	while(false);

	Unlock();

	return bReturn;
}

bool CDeviceAdvantechDio::ReadInBit(int nBit)
{
	bool bReturn = false;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		if(nBit >= ADX_DIO_IN_PORT || nBit < 0)
			break;

		BYTE cData = 0;

		long nPort = nBit / 8;
		long nReadBit = nBit - (nPort * 8);

		if(m_pDiCtrl->ReadBit(nPort, nReadBit, &cData) != Automation::BDaq::Success)
			break;

		bReturn = (bool)cData;
	}
	while(false);

	Unlock();

	return bReturn;
}

BYTE CDeviceAdvantechDio::ReadOutByte()
{
	BYTE cReturn = 0;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		if(m_pDoCtrl->Read(0, cReturn) != Automation::BDaq::Success)
			cReturn = 0;
	}
	while(false);

	Unlock();

	return cReturn;
}

BYTE CDeviceAdvantechDio::ReadInByte()
{
	BYTE cReturn = 0;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		if(m_pDiCtrl->Read(0, cReturn) != Automation::BDaq::Success)
			cReturn = 0;
	}
	while(false);

	Unlock();

	return cReturn;
}

WORD CDeviceAdvantechDio::ReadOutWord()
{
	WORD wReturn = 0;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		if(m_pDoCtrl->Read(0, sizeof(wReturn), (BYTE*)&wReturn) != Automation::BDaq::Success)
			wReturn = 0;
	}
	while(false);

	Unlock();

	return wReturn;
}

WORD CDeviceAdvantechDio::ReadInWord()
{
	WORD wReturn = 0;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		if(m_pDiCtrl->Read(0, sizeof(wReturn), (BYTE*)&wReturn) != Automation::BDaq::Success)
			wReturn = 0;
	}
	while(false);

	Unlock();

	return wReturn;
}

DWORD CDeviceAdvantechDio::ReadOutDword()
{
	DWORD dwReturn = 0;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		if(m_pDoCtrl->Read(0, sizeof(dwReturn), (BYTE*)&dwReturn) != Automation::BDaq::Success)
			dwReturn = 0;
	}
	while(false);

	Unlock();

	return dwReturn;
}

DWORD CDeviceAdvantechDio::ReadInDword()
{
	DWORD dwReturn = 0;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		if(m_pDiCtrl->Read(0, sizeof(dwReturn), (BYTE*)&dwReturn) != Automation::BDaq::Success)
			dwReturn = 0;
	}
	while(false);

	Unlock();

	return dwReturn;
}

bool CDeviceAdvantechDio::WriteOutBit(int nBit, bool bOn)
{
	bool bReturn = false;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		if(nBit >= ADX_DIO_OUT_PORT || nBit < 0)
			break;

		long nPort = nBit / 8;
		long nWriteBit = nBit - (nPort * 8);

		

		if(m_pDoCtrl->WriteBit(nPort, nWriteBit, (BYTE)bOn) != Automation::BDaq::Success)
			break;

		bReturn = true;
	}
	while(false);

	Unlock();

	return bReturn;
}

bool CDeviceAdvantechDio::WriteInBit(int nBit, bool bOn)
{
	ASSERT(0);
	return false;
}

bool CDeviceAdvantechDio::WriteOutByte(BYTE cByte)
{
	bool bReturn = false;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		if(m_pDoCtrl->Write(0, cByte) != Automation::BDaq::Success)
			break;

		bReturn = true;
	}
	while(false);

	Unlock();

	return bReturn;
}

bool CDeviceAdvantechDio::WriteInByte(BYTE cByte)
{
	ASSERT(0);
	return false;
}

bool CDeviceAdvantechDio::WriteOutWord(WORD wWord)
{
	bool bReturn = false;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		if(m_pDoCtrl->Write(0, sizeof(wWord), (BYTE*)&wWord) != Automation::BDaq::Success)
			break;

		bReturn = true;
	}
	while(false);

	Unlock();

	return bReturn;
}

bool CDeviceAdvantechDio::WriteInWord(WORD wWord)
{
	ASSERT(0);
	return false;
}

bool CDeviceAdvantechDio::WriteOutDword(DWORD dwDword)
{
	bool bReturn = false;
	
	Lock();
	
	do
	{
		if(!IsInitialized())
			break;

		if(m_pDoCtrl->Write(0, sizeof(dwDword), (BYTE*)&dwDword) != Automation::BDaq::Success)
			break;

		bReturn = true;
	}
	while(false);

	Unlock();

	return bReturn;
}

bool CDeviceAdvantechDio::WriteInDword(DWORD dwDword)
{
	ASSERT(0);
	return false;
}

bool CDeviceAdvantechDio::ClearOutPort()
{
	return WriteOutDword(0);
}

bool CDeviceAdvantechDio::ClearInPort()
{
	ASSERT(0);
	return false;
}

long CDeviceAdvantechDio::GetOutputPortCount()
{
	return ADX_DIO_OUT_PORT;
}

long CDeviceAdvantechDio::GetInputPortCount()
{
	return ADX_DIO_IN_PORT;
}

bool CDeviceAdvantechDio::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = false;

	bool bFoundID = false;

	CString strMessage;

	do
	{
		switch(nParam)
		{
		case EDeviceParameterAdvantechDio_DeviceID:
			{
				bReturn = !SetDeviceID(strValue);
				bFoundID = true;
			}
			break;
		case EDeviceParameterAdvantechDio_UseInputNo0:
		case EDeviceParameterAdvantechDio_UseInputNo1:
		case EDeviceParameterAdvantechDio_UseInputNo2:
		case EDeviceParameterAdvantechDio_UseInputNo3:
		case EDeviceParameterAdvantechDio_UseInputNo4:
		case EDeviceParameterAdvantechDio_UseInputNo5:
		case EDeviceParameterAdvantechDio_UseInputNo6:
		case EDeviceParameterAdvantechDio_UseInputNo7:
		case EDeviceParameterAdvantechDio_UseInputNo8:
		case EDeviceParameterAdvantechDio_UseInputNo9:
		case EDeviceParameterAdvantechDio_UseInputNo10:
		case EDeviceParameterAdvantechDio_UseInputNo11:
		case EDeviceParameterAdvantechDio_UseInputNo12:
		case EDeviceParameterAdvantechDio_UseInputNo13:
		case EDeviceParameterAdvantechDio_UseInputNo14:
		case EDeviceParameterAdvantechDio_UseInputNo15:
			{
				int nPreValue = _ttoi(GetParamValue(nParam));
				int nValue = _ttoi(strValue);

				if(SetParamValue(nParam, strValue))
				{
					m_pBUsingInputArray[nParam - EDeviceParameterAdvantechDio_UseInputNo0] = nValue;
					
					bReturn = SaveSettings(nParam);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAdvantechDio[nParam], g_lpszAdvantechDioSwitch[nPreValue], g_lpszAdvantechDioSwitch[nValue]);
			}
			break;
		case EDeviceParameterAdvantechDio_UseOutputNo0:
		case EDeviceParameterAdvantechDio_UseOutputNo1:
		case EDeviceParameterAdvantechDio_UseOutputNo2:
		case EDeviceParameterAdvantechDio_UseOutputNo3:
		case EDeviceParameterAdvantechDio_UseOutputNo4:
		case EDeviceParameterAdvantechDio_UseOutputNo5:
		case EDeviceParameterAdvantechDio_UseOutputNo6:
		case EDeviceParameterAdvantechDio_UseOutputNo7:
		case EDeviceParameterAdvantechDio_UseOutputNo8:
		case EDeviceParameterAdvantechDio_UseOutputNo9:
		case EDeviceParameterAdvantechDio_UseOutputNo10:
		case EDeviceParameterAdvantechDio_UseOutputNo11:
		case EDeviceParameterAdvantechDio_UseOutputNo12:
		case EDeviceParameterAdvantechDio_UseOutputNo13:
		case EDeviceParameterAdvantechDio_UseOutputNo14:
		case EDeviceParameterAdvantechDio_UseOutputNo15:
			{
				int nPreValue = _ttoi(GetParamValue(nParam));
				int nValue = _ttoi(strValue);

				if(SetParamValue(nParam, strValue))
				{
					m_pBUsingOutputArray[nParam - EDeviceParameterAdvantechDio_UseOutputNo0] = nValue;
					
					bReturn = SaveSettings(nParam);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAdvantechDio[nParam], g_lpszAdvantechDioSwitch[nPreValue], g_lpszAdvantechDioSwitch[nValue]);
			}
			break;
		case EDeviceParameterAdvantechDio_InputNo0Name:
		case EDeviceParameterAdvantechDio_InputNo1Name:
		case EDeviceParameterAdvantechDio_InputNo2Name:
		case EDeviceParameterAdvantechDio_InputNo3Name:
		case EDeviceParameterAdvantechDio_InputNo4Name:
		case EDeviceParameterAdvantechDio_InputNo5Name:
		case EDeviceParameterAdvantechDio_InputNo6Name:
		case EDeviceParameterAdvantechDio_InputNo7Name:
		case EDeviceParameterAdvantechDio_InputNo8Name:
		case EDeviceParameterAdvantechDio_InputNo9Name:
		case EDeviceParameterAdvantechDio_InputNo10Name:
		case EDeviceParameterAdvantechDio_InputNo11Name:
		case EDeviceParameterAdvantechDio_InputNo12Name:
		case EDeviceParameterAdvantechDio_InputNo13Name:
		case EDeviceParameterAdvantechDio_InputNo14Name:
		case EDeviceParameterAdvantechDio_InputNo15Name:
			{
				CString strPreValue = GetParamValue(nParam);

				if(SetParamValue(nParam, strValue))
				{
					m_pStrInputNameArray[nParam - EDeviceParameterAdvantechDio_InputNo0Name] = strValue;
					
					bReturn = SaveSettings(nParam);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAdvantechDio[nParam], strPreValue, strValue);
			}
			break;
		case EDeviceParameterAdvantechDio_OutputNo0Name:
		case EDeviceParameterAdvantechDio_OutputNo1Name:
		case EDeviceParameterAdvantechDio_OutputNo2Name:
		case EDeviceParameterAdvantechDio_OutputNo3Name:
		case EDeviceParameterAdvantechDio_OutputNo4Name:
		case EDeviceParameterAdvantechDio_OutputNo5Name:
		case EDeviceParameterAdvantechDio_OutputNo6Name:
		case EDeviceParameterAdvantechDio_OutputNo7Name:
		case EDeviceParameterAdvantechDio_OutputNo8Name:
		case EDeviceParameterAdvantechDio_OutputNo9Name:
		case EDeviceParameterAdvantechDio_OutputNo10Name:
		case EDeviceParameterAdvantechDio_OutputNo11Name:
		case EDeviceParameterAdvantechDio_OutputNo12Name:
		case EDeviceParameterAdvantechDio_OutputNo13Name:
		case EDeviceParameterAdvantechDio_OutputNo14Name:
		case EDeviceParameterAdvantechDio_OutputNo15Name:
			{
				CString strPreValue = GetParamValue(nParam);

				if(SetParamValue(nParam, strValue))
				{
					m_pStrOutputNameArray[nParam - EDeviceParameterAdvantechDio_OutputNo0Name] = strValue;
					
					bReturn = SaveSettings(nParam);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAdvantechDio[nParam], strPreValue, strValue);
			}
			break;
		default:
				strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtofindtheparameter);
			break;
		}

		if(EDeviceParameterAdvantechDio_InputStatus < nParam && EDeviceParameterAdvantechDio_Count > nParam)
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

void CDeviceAdvantechDio::OnBnClickedOutLed(UINT nID)
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

bool CDeviceAdvantechDio::DoesModuleExist()
{
	bool bReturn = false;

	CString strModuleName;

	do
	{
		strModuleName.Format(_T("biodaq.dll"));

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