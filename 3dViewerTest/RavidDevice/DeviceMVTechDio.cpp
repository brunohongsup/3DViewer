#include "stdafx.h"

#include "DeviceMVTechDio.h"

#ifdef USING_DEVICE

#include "../RavidCore/RavidDatabase.h"
#include "../RavidCore/RavidRecordset.h"
#include "../RavidCore/PerformanceCounter.h"

#include "../RavidFramework/LogManager.h"
#include "../RavidFramework/DeviceManager.h"
#include "../RavidFramework/LibraryManager.h"
#include "../RavidFramework/SequenceManager.h"
#include "../RavidFramework/AuthorityManager.h"
#include "../RavidFramework/FrameworkManager.h"
#include "../RavidFramework/EventHandlerManager.h"
#include "../RavidFramework/MultiLanguageManager.h"

#include "../RavidFramework/RavidLedCtrl.h"

#include "../Libraries/Includes/PlxApi/PlxApi.h"

// plxapi650.dll
// plxapi650_x64.dll
#pragma comment(lib, COMMONLIB_PREFIX "PlxApi/PlxApi.lib")

using namespace Ravid;
using namespace Ravid::Device;
using namespace Ravid::Database;
using namespace Ravid::Framework;
using namespace Ravid::Algorithms;
using namespace Ravid::Miscellaneous;

IMPLEMENT_DYNAMIC(CDeviceMVTechDio, CDeviceDio)

BEGIN_MESSAGE_MAP(CDeviceMVTechDio, CDeviceDio)
	ON_COMMAND_RANGE(EDeviceAddEnumeratedControlID_OutputLedStart, EDeviceAddEnumeratedControlID_OutputLedEnd, OnBnClickedOutLed)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static LPCTSTR g_lpszDeviceMVTechDio[EDeviceParameterMVTechDio_Count] =
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
	_T("Use Clear Initalize"),
	_T("Use Clear Terminate"),
	_T("Check Port From Board"),
};

static LPCTSTR g_lpszMVTechDioSwitch[EDeviceDIOSwitch_Count] =
{
	_T("Off"),
	_T("On")
};


CDeviceMVTechDio::CDeviceMVTechDio()
{
	m_arrDwCS = new DWORD[DeviceMVTechDio::MAX_BASE_ADDR_NUMS];
	m_arrDwCSLen = new DWORD[DeviceMVTechDio::MAX_BASE_ADDR_NUMS];

	memset(m_arrDwCS, 0, sizeof(DWORD) * DeviceMVTechDio::MAX_BASE_ADDR_NUMS);
	memset(m_arrDwCSLen, 0, sizeof(DWORD) * DeviceMVTechDio::MAX_BASE_ADDR_NUMS);
}


CDeviceMVTechDio::~CDeviceMVTechDio()
{
	Terminate();
		
	if(m_pDeviceObject)
	{
		delete m_pDeviceObject;
		m_pDeviceObject = nullptr;
	}

	if(m_arrDwCS)
	{
		delete[] m_arrDwCS;
		m_arrDwCS = nullptr;
	}

	if(m_arrDwCSLen)
	{
		delete[] m_arrDwCSLen;
		m_arrDwCSLen = nullptr;
	}
}

EDeviceInitializeResult CDeviceMVTechDio::Initialize()
{
	EDeviceInitializeResult eReturn = EDeviceInitializeResult_UnknownError;

	CString strMessage, strStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("MVTechDIO"));

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

		PLX_STATUS rc;
		PLX_DEVICE_KEY plxDeviceKey;

		DWORD dwRegValue[DeviceMVTechDio::LOCAL_CONFIG_REG_CNT];

		memset(m_arrDwCS, 0, sizeof(m_arrDwCS));
		memset(m_arrDwCSLen, 0, sizeof(m_arrDwCSLen));
		memset(&plxDeviceKey, PCI_FIELD_IGNORE, sizeof(PLX_DEVICE_KEY));

		plxDeviceKey.VendorId = 0x10b6;
		plxDeviceKey.DeviceId = 0x9030;

		if(m_pDeviceObject)
		{
			delete m_pDeviceObject;
			m_pDeviceObject = nullptr;
		}

		m_pDeviceObject = new PLX_DEVICE_OBJECT;

		int nDeviceNum = _ttoi(GetDeviceID());

		rc = PlxPci_DeviceFind(&plxDeviceKey, nDeviceNum);

		if(rc != ApiSuccess)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtofindthedevice);
			eReturn = EDeviceInitializeResult_NotFoundDeviceError;
			break;
		}

		rc = PlxPci_DeviceOpen(&plxDeviceKey, (PLX_DEVICE_OBJECT*)m_pDeviceObject);

		if(rc != ApiSuccess)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntopenthedevice);
			eReturn = EDeviceInitializeResult_NotOpenedDeviceError;
			break;
		}

		for(int i = 0; i < DeviceMVTechDio::LOCAL_CONFIG_REG_CNT; i++)
		{
			dwRegValue[i] = PlxPci_PlxRegisterRead((PLX_DEVICE_OBJECT*)m_pDeviceObject, 0x00 + (i * 4), &rc);

			if(rc != ApiSuccess)
				break;
		}

		if(rc != ApiSuccess)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("apiRegister"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		m_arrDwCS[0] = dwRegValue[5] & ~0x00000001;
		m_arrDwCS[1] = dwRegValue[6] & ~0x00000001;
		m_arrDwCS[2] = dwRegValue[7] & ~0x00000001;
		m_arrDwCS[3] = dwRegValue[8] & ~0x00000001;

		m_arrDwCSLen[0] = dwRegValue[0] >> 4;
		m_arrDwCSLen[1] = dwRegValue[1] >> 4;
		m_arrDwCSLen[2] = dwRegValue[2] >> 4;
		m_arrDwCSLen[3] = dwRegValue[3] >> 4;

		m_bIsInitialized = true;

		bool bClear = false;
		GetUseClearInitalize(&bClear);

		if(bClear)
			ClearOutPort();

		m_bCheckFromBoard = false;
		GetCheckPortFromBoard(&m_bCheckFromBoard);

		ReadOutWord();

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

EDeviceTerminateResult CDeviceMVTechDio::Terminate()
{
	EDeviceTerminateResult eReturn = EDeviceTerminateResult_UnknownError;

	CString strMessage;

	SetStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("MVTechDIO"));

		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);

		return EDeviceTerminateResult_NotFoundApiError;
	}

	do
	{
		if(m_pDeviceObject)
		{
			bool bClear = false;
			GetUseClearTerminate(&bClear);

			if(bClear)
				ClearOutPort();

			PlxPci_DeviceClose((PLX_DEVICE_OBJECT*)m_pDeviceObject);

			delete m_pDeviceObject;
			m_pDeviceObject = nullptr;
		} 

		m_bIsInitialized = false;

		strMessage = CMultiLanguageManager::GetString(ELanguageParameter_TheDevicehasbeensuccessfullyterminated);

		CEventHandlerManager::BroadcastOnDeviceTerminated(this);
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceMVTechDio::LoadSettings()
{
	bool bReturn = false;

	m_vctParameterFieldConfigurations.clear();

	do
	{
		AddParameterFieldConfigurations(EDeviceParameterMVTechDio_DeviceID, g_lpszDeviceMVTechDio[EDeviceParameterMVTechDio_DeviceID], _T("0"), EParameterFieldType_Edit);
		AddParameterFieldConfigurations(EDeviceParameterMVTechDio_UseClearInitialize, g_lpszDeviceMVTechDio[EDeviceParameterMVTechDio_UseClearInitialize], _T("0"), EParameterFieldType_Check);
		AddParameterFieldConfigurations(EDeviceParameterMVTechDio_UseClearTerminate, g_lpszDeviceMVTechDio[EDeviceParameterMVTechDio_UseClearTerminate], _T("0"), EParameterFieldType_Check);
		AddParameterFieldConfigurations(EDeviceParameterMVTechDio_CheckPortFromBoard, g_lpszDeviceMVTechDio[EDeviceParameterMVTechDio_CheckPortFromBoard], _T("0"), EParameterFieldType_Check);

		AddParameterFieldConfigurations(EDeviceParameterMVTechDio_InputStatus, g_lpszDeviceMVTechDio[EDeviceParameterMVTechDio_InputStatus], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);

		for(int i = 0; i < DeviceMVTechDio::MIO_IN_PORT; ++i)
		{
			AddParameterFieldConfigurations(EDeviceParameterMVTechDio_UseInputNo0 + i, g_lpszDeviceMVTechDio[EDeviceParameterMVTechDio_UseInputNo0 + i], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 1);
		}

		AddParameterFieldConfigurations(EDeviceParameterMVTechDio_OutputStatus, g_lpszDeviceMVTechDio[EDeviceParameterMVTechDio_OutputStatus], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);

		for(int i = 0; i < DeviceMVTechDio::MIO_OUT_PORT; ++i)
		{
			AddParameterFieldConfigurations(EDeviceParameterMVTechDio_UseOutputNo0 + i, g_lpszDeviceMVTechDio[EDeviceParameterMVTechDio_UseOutputNo0 + i], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 1);
		}

		AddParameterFieldConfigurations(EDeviceParameterMVTechDio_InputName, g_lpszDeviceMVTechDio[EDeviceParameterMVTechDio_InputName], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);

		for(int i = 0; i < DeviceMVTechDio::MIO_IN_PORT; ++i)
		{
			AddParameterFieldConfigurations(EDeviceParameterMVTechDio_InputNo0Name + i, g_lpszDeviceMVTechDio[EDeviceParameterMVTechDio_InputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);
		}

		AddParameterFieldConfigurations(EDeviceParameterMVTechDio_OutputName, g_lpszDeviceMVTechDio[EDeviceParameterMVTechDio_OutputName], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);

		for(int i = 0; i < DeviceMVTechDio::MIO_OUT_PORT; ++i)
		{
			AddParameterFieldConfigurations(EDeviceParameterMVTechDio_OutputNo0Name + i, g_lpszDeviceMVTechDio[EDeviceParameterMVTechDio_OutputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);
		}

		bReturn = __super::LoadSettings();

		AllocateIO(DeviceMVTechDio::MIO_IN_PORT, DeviceMVTechDio::MIO_OUT_PORT);

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
	}
	while(false);

	return bReturn;
}

bool CDeviceMVTechDio::ReadOutBit(int nBit)
{
	bool bReturn = false;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		if(nBit >= DeviceMVTechDio::MIO_OUT_PORT || nBit < 0)
			break;

		if(m_bCheckFromBoard)
		{
			DWORD dwOutStatus = 0;

			PLX_STATUS rc = PlxPci_PciBarSpaceRead((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_arrDwCS[DeviceMVTechDio::PCI_BASE_CS0] + DeviceMVTechDio::IO_OUT_READ, &dwOutStatus, sizeof(dwOutStatus), BitSize32, true);
			if(rc != ApiSuccess)
				break;

			m_dwOutStatus = dwOutStatus;
			bReturn = (dwOutStatus >> nBit) & 0x01;
		}
		else
			bReturn = (m_dwOutStatus >> nBit) & 0x01;
	}
	while(false);

	Unlock();

	return bReturn;
}

bool CDeviceMVTechDio::ReadInBit(int nBit)
{
	bool bReturn = false;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		if(nBit >= DeviceMVTechDio::MIO_IN_PORT || nBit < 0)
			break;

		PLX_STATUS rc = PlxPci_PciBarSpaceRead((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_arrDwCS[DeviceMVTechDio::PCI_BASE_CS0] + DeviceMVTechDio::IO_READ, &m_dwInStatus, sizeof(m_dwInStatus), BitSize32, true);

		if(rc != ApiSuccess)
			break;

		bReturn = (m_dwInStatus >> nBit) & 0x01;
	}
	while(false);

	Unlock();

	return bReturn;
}

BYTE CDeviceMVTechDio::ReadOutByte()
{
	BYTE cReturn = 0;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		if(m_bCheckFromBoard)
		{
			DWORD dwOutStatus = 0;

			PLX_STATUS rc = PlxPci_PciBarSpaceRead((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_arrDwCS[DeviceMVTechDio::PCI_BASE_CS0] + DeviceMVTechDio::IO_OUT_READ, &dwOutStatus, sizeof(dwOutStatus), BitSize32, true);
			if(rc != ApiSuccess)
				break;

			m_dwOutStatus = dwOutStatus;
			cReturn = dwOutStatus & BYTE_MAX;
		}
		else
			cReturn = m_dwOutStatus & BYTE_MAX;		
	}
	while(false);

	Unlock();

	return cReturn;
}

BYTE CDeviceMVTechDio::ReadInByte()
{
	BYTE cReturn = 0;
	
	Lock();

	do
	{
		if(!IsInitialized())
			break;

		PLX_STATUS rc = PlxPci_PciBarSpaceRead((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_arrDwCS[DeviceMVTechDio::PCI_BASE_CS0] + DeviceMVTechDio::IO_READ, &m_dwInStatus, sizeof(m_dwInStatus), BitSize32, true);

		if(rc != ApiSuccess)
			break;

		cReturn = m_dwInStatus & BYTE_MAX;
	}
	while(false);

	Unlock();

	return cReturn;
}

WORD CDeviceMVTechDio::ReadOutWord()
{
	WORD wReturn = 0;
	
	Lock();

	do
	{
		if(!IsInitialized())
			break;

		if(m_bCheckFromBoard)
		{
			DWORD dwOutStatus = 0;

			PLX_STATUS rc = PlxPci_PciBarSpaceRead((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_arrDwCS[DeviceMVTechDio::PCI_BASE_CS0] + DeviceMVTechDio::IO_OUT_READ, &dwOutStatus, sizeof(dwOutStatus), BitSize32, true);
			if(rc != ApiSuccess)
				break;

			m_dwOutStatus = dwOutStatus;
			wReturn = dwOutStatus & WORD_MAX;
		}
		else
			wReturn = m_dwOutStatus & WORD_MAX;
	}
	while(false);

	Unlock();

	return wReturn;
}

WORD CDeviceMVTechDio::ReadInWord()
{
	WORD wReturn = 0;
	
	Lock();

	do
	{
		if(!IsInitialized())
			break;

		PLX_STATUS rc = PlxPci_PciBarSpaceRead((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_arrDwCS[DeviceMVTechDio::PCI_BASE_CS0] + DeviceMVTechDio::IO_READ, &m_dwInStatus, sizeof(m_dwInStatus), BitSize32, true);

		if(rc != ApiSuccess)
			break;

		wReturn = m_dwInStatus & WORD_MAX;
	}
	while(false);

	Unlock();

	return wReturn;
}

DWORD CDeviceMVTechDio::ReadOutDword()
{
	DWORD dwReturn = 0;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		if(m_bCheckFromBoard)
		{
			DWORD dwOutStatus = 0;

			PLX_STATUS rc = PlxPci_PciBarSpaceRead((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_arrDwCS[DeviceMVTechDio::PCI_BASE_CS0] + DeviceMVTechDio::IO_OUT_READ, &dwOutStatus, sizeof(dwOutStatus), BitSize32, true);
			if(rc != ApiSuccess)
				break;

			m_dwOutStatus = dwOutStatus;
			dwReturn = dwOutStatus & DWORD_MAX;
		}
		else
			dwReturn = m_dwOutStatus & DWORD_MAX;
	}
	while(false);

	Unlock();

	return dwReturn;
}

DWORD CDeviceMVTechDio::ReadInDword()
{
	DWORD dwReturn = 0;
	
	Lock();

	do
	{
		if(!IsInitialized())
			break;

		PLX_STATUS rc = PlxPci_PciBarSpaceRead((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_arrDwCS[DeviceMVTechDio::PCI_BASE_CS0] + DeviceMVTechDio::IO_READ, &m_dwInStatus, sizeof(m_dwInStatus), BitSize32, true);

		if(rc != ApiSuccess)
			break;

		dwReturn = m_dwInStatus;
	}
	while(false);

	Unlock();

	return dwReturn;
}

bool CDeviceMVTechDio::WriteOutBit(int nBit, bool bOn)
{
	bool bReturn = false;
	
	Lock();

	do
	{
		if(!IsInitialized())
			break;

		if(nBit >= DeviceMVTechDio::MIO_OUT_PORT)
			break;

		if(bOn)
			m_dwOutStatus |= 1 << nBit;
		else
			m_dwOutStatus &= ~(1 << nBit);

		PLX_STATUS rc = PlxPci_PciBarSpaceWrite((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_arrDwCS[DeviceMVTechDio::PCI_BASE_CS0] + DeviceMVTechDio::IO_WRITE, &m_dwOutStatus, sizeof(m_dwOutStatus), BitSize32, true);

		bReturn = rc == ApiSuccess;
	}
	while(false);

	Unlock();

	return bReturn;
}

bool CDeviceMVTechDio::WriteInBit(int nBit, bool bOn)
{
	return false;
}

bool CDeviceMVTechDio::WriteOutByte(BYTE cByte)
{
	bool bReturn = false;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		m_dwOutStatus = cByte;

		PLX_STATUS rc = PlxPci_PciBarSpaceWrite((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_arrDwCS[DeviceMVTechDio::PCI_BASE_CS0] + DeviceMVTechDio::IO_WRITE, &m_dwOutStatus, sizeof(m_dwOutStatus), BitSize32, true);

		bReturn = rc == ApiSuccess;
	}
	while(false);

	Unlock();

	return bReturn;
}

bool CDeviceMVTechDio::WriteInByte(BYTE cByte)
{
	return false;
}

bool CDeviceMVTechDio::WriteOutWord(WORD wWord)
{
	bool bReturn = false;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		m_dwOutStatus = wWord;

		PLX_STATUS rc = PlxPci_PciBarSpaceWrite((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_arrDwCS[DeviceMVTechDio::PCI_BASE_CS0] + DeviceMVTechDio::IO_WRITE, &m_dwOutStatus, sizeof(m_dwOutStatus), BitSize32, true);

		bReturn = rc == ApiSuccess;
	}
	while(false);

	Unlock();

	return bReturn;
}

bool CDeviceMVTechDio::WriteInWord(WORD wWord)
{
	return false;
}

bool CDeviceMVTechDio::WriteOutDword(DWORD dwDword)
{
	bool bReturn = false;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		m_dwOutStatus = dwDword;

		PLX_STATUS rc = PlxPci_PciBarSpaceWrite((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_arrDwCS[DeviceMVTechDio::PCI_BASE_CS0] + DeviceMVTechDio::IO_WRITE, &m_dwOutStatus, sizeof(m_dwOutStatus), BitSize32, true);

		bReturn = rc == ApiSuccess;
	}
	while(false);

	Unlock();

	return bReturn;
}

bool CDeviceMVTechDio::WriteInDword(DWORD dwDword)
{
	return false;
}

bool CDeviceMVTechDio::ClearOutPort()
{
	return WriteOutDword(0);
}

bool CDeviceMVTechDio::ClearInPort()
{
	return false;
}

long CDeviceMVTechDio::GetOutputPortCount()
{
	return DeviceMVTechDio::MIO_OUT_PORT;
}

long CDeviceMVTechDio::GetInputPortCount()
{
	return DeviceMVTechDio::MIO_IN_PORT;
}

EGetFunctionMVTechDio CDeviceMVTechDio::GetUseClearInitalize(bool * pParam)
{
	EGetFunctionMVTechDio eReturn = EGetFunctionMVTechDio_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGetFunctionMVTechDio_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMVTechDio_UseClearInitialize));

		eReturn = EGetFunctionMVTechDio_OK;
	}
	while(false);

	return eReturn;
}

ESetFunctionMVTechDio CDeviceMVTechDio::SetUseClearInitalize(bool bParam)
{
	ESetFunctionMVTechDio eReturn = ESetFunctionMVTechDio_UnknownError;

	EDeviceParameterMVTechDio eSaveNum = EDeviceParameterMVTechDio_UseClearInitialize;

	bool bPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = ESetFunctionMVTechDio_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = ESetFunctionMVTechDio_WriteToDatabaseError;
			break;
		}

		eReturn = ESetFunctionMVTechDio_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMVTechDio[eSaveNum], g_lpszMVTechDioSwitch[bPreValue], g_lpszMVTechDioSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGetFunctionMVTechDio CDeviceMVTechDio::GetUseClearTerminate(bool * pParam)
{
	EGetFunctionMVTechDio eReturn = EGetFunctionMVTechDio_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGetFunctionMVTechDio_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMVTechDio_UseClearTerminate));

		eReturn = EGetFunctionMVTechDio_OK;
	}
	while(false);

	return eReturn;
}

ESetFunctionMVTechDio CDeviceMVTechDio::SetUseClearTerminate(bool bParam)
{
	ESetFunctionMVTechDio eReturn = ESetFunctionMVTechDio_UnknownError;

	EDeviceParameterMVTechDio eSaveNum = EDeviceParameterMVTechDio_UseClearTerminate;

	bool bPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = ESetFunctionMVTechDio_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = ESetFunctionMVTechDio_WriteToDatabaseError;
			break;
		}

		eReturn = ESetFunctionMVTechDio_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMVTechDio[eSaveNum], g_lpszMVTechDioSwitch[bPreValue], g_lpszMVTechDioSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGetFunctionMVTechDio CDeviceMVTechDio::GetCheckPortFromBoard(bool * pParam)
{
	EGetFunctionMVTechDio eReturn = EGetFunctionMVTechDio_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGetFunctionMVTechDio_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMVTechDio_CheckPortFromBoard));

		eReturn = EGetFunctionMVTechDio_OK;
	}
	while(false);

	return eReturn;
}

ESetFunctionMVTechDio CDeviceMVTechDio::SetCheckPortFromBoard(bool bParam)
{
	ESetFunctionMVTechDio eReturn = ESetFunctionMVTechDio_UnknownError;

	EDeviceParameterMVTechDio eSaveNum = EDeviceParameterMVTechDio_CheckPortFromBoard;

	bool bPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = ESetFunctionMVTechDio_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = ESetFunctionMVTechDio_WriteToDatabaseError;
			break;
		}

		m_bCheckFromBoard = bParam;

		eReturn = ESetFunctionMVTechDio_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMVTechDio[eSaveNum], g_lpszMVTechDioSwitch[bPreValue], g_lpszMVTechDioSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceMVTechDio::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = false;

	bool bFoundID = false;

	CString strMessage;

	do
	{
		switch(nParam)
		{
		case EDeviceParameterMVTechDio_DeviceID:
				bReturn = !SetDeviceID(strValue);
				bFoundID = true;
			break;
		case EDeviceParameterMVTechDio_UseClearInitialize:
			bReturn = !SetUseClearInitalize(_ttoi(strValue));
			bFoundID = true;
			break;
		case EDeviceParameterMVTechDio_UseClearTerminate:
			bReturn = !SetUseClearTerminate(_ttoi(strValue));
			bFoundID = true;
			break;
		case EDeviceParameterMVTechDio_CheckPortFromBoard:
			bReturn = !SetCheckPortFromBoard(_ttoi(strValue));
			bFoundID = true;
			break;
		case EDeviceParameterMVTechDio_UseInputNo0:
		case EDeviceParameterMVTechDio_UseInputNo1:
		case EDeviceParameterMVTechDio_UseInputNo2:
		case EDeviceParameterMVTechDio_UseInputNo3:
		case EDeviceParameterMVTechDio_UseInputNo4:
		case EDeviceParameterMVTechDio_UseInputNo5:
		case EDeviceParameterMVTechDio_UseInputNo6:
		case EDeviceParameterMVTechDio_UseInputNo7:
		case EDeviceParameterMVTechDio_UseInputNo8:
		case EDeviceParameterMVTechDio_UseInputNo9:
		case EDeviceParameterMVTechDio_UseInputNo10:
		case EDeviceParameterMVTechDio_UseInputNo11:
		case EDeviceParameterMVTechDio_UseInputNo12:
		case EDeviceParameterMVTechDio_UseInputNo13:
		case EDeviceParameterMVTechDio_UseInputNo14:
		case EDeviceParameterMVTechDio_UseInputNo15:
		case EDeviceParameterMVTechDio_UseInputNo16:
		case EDeviceParameterMVTechDio_UseInputNo17:
		case EDeviceParameterMVTechDio_UseInputNo18:
		case EDeviceParameterMVTechDio_UseInputNo19:
		case EDeviceParameterMVTechDio_UseInputNo20:
		case EDeviceParameterMVTechDio_UseInputNo21:
		case EDeviceParameterMVTechDio_UseInputNo22:
		case EDeviceParameterMVTechDio_UseInputNo23:
		case EDeviceParameterMVTechDio_UseInputNo24:
		case EDeviceParameterMVTechDio_UseInputNo25:
		case EDeviceParameterMVTechDio_UseInputNo26:
		case EDeviceParameterMVTechDio_UseInputNo27:
		case EDeviceParameterMVTechDio_UseInputNo28:
		case EDeviceParameterMVTechDio_UseInputNo29:
		case EDeviceParameterMVTechDio_UseInputNo30:
		case EDeviceParameterMVTechDio_UseInputNo31:
			{
				int nPreValue = _ttoi(GetParamValue(nParam));
				int nValue = _ttoi(strValue);

				if(SetParamValue(nParam, strValue))
				{
					m_pBUsingInputArray[nParam - EDeviceParameterMVTechDio_UseInputNo0] = _ttoi(strValue);
					bReturn = SaveSettings(nParam);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMVTechDio[nParam], g_lpszMVTechDioSwitch[nPreValue], g_lpszMVTechDioSwitch[nValue]);
			}
			break;
		case EDeviceParameterMVTechDio_UseOutputNo0:
		case EDeviceParameterMVTechDio_UseOutputNo1:
		case EDeviceParameterMVTechDio_UseOutputNo2:
		case EDeviceParameterMVTechDio_UseOutputNo3:
		case EDeviceParameterMVTechDio_UseOutputNo4:
		case EDeviceParameterMVTechDio_UseOutputNo5:
		case EDeviceParameterMVTechDio_UseOutputNo6:
		case EDeviceParameterMVTechDio_UseOutputNo7:
		case EDeviceParameterMVTechDio_UseOutputNo8:
		case EDeviceParameterMVTechDio_UseOutputNo9:
		case EDeviceParameterMVTechDio_UseOutputNo10:
		case EDeviceParameterMVTechDio_UseOutputNo11:
		case EDeviceParameterMVTechDio_UseOutputNo12:
		case EDeviceParameterMVTechDio_UseOutputNo13:
		case EDeviceParameterMVTechDio_UseOutputNo14:
		case EDeviceParameterMVTechDio_UseOutputNo15:
		case EDeviceParameterMVTechDio_UseOutputNo16:
		case EDeviceParameterMVTechDio_UseOutputNo17:
		case EDeviceParameterMVTechDio_UseOutputNo18:
		case EDeviceParameterMVTechDio_UseOutputNo19:
		case EDeviceParameterMVTechDio_UseOutputNo20:
		case EDeviceParameterMVTechDio_UseOutputNo21:
		case EDeviceParameterMVTechDio_UseOutputNo22:
		case EDeviceParameterMVTechDio_UseOutputNo23:
		case EDeviceParameterMVTechDio_UseOutputNo24:
		case EDeviceParameterMVTechDio_UseOutputNo25:
		case EDeviceParameterMVTechDio_UseOutputNo26:
		case EDeviceParameterMVTechDio_UseOutputNo27:
		case EDeviceParameterMVTechDio_UseOutputNo28:
		case EDeviceParameterMVTechDio_UseOutputNo29:
		case EDeviceParameterMVTechDio_UseOutputNo30:
		case EDeviceParameterMVTechDio_UseOutputNo31:
			{
				int nPreValue = _ttoi(GetParamValue(nParam));
				int nValue = _ttoi(strValue);

				if(SetParamValue(nParam, strValue))
				{
					m_pBUsingOutputArray[nParam - EDeviceParameterMVTechDio_UseOutputNo0] = _ttoi(strValue);
					bReturn = SaveSettings(nParam);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMVTechDio[nParam], g_lpszMVTechDioSwitch[nPreValue], g_lpszMVTechDioSwitch[nValue]);
			}
			break;
		case EDeviceParameterMVTechDio_InputNo0Name:
		case EDeviceParameterMVTechDio_InputNo1Name:
		case EDeviceParameterMVTechDio_InputNo2Name:
		case EDeviceParameterMVTechDio_InputNo3Name:
		case EDeviceParameterMVTechDio_InputNo4Name:
		case EDeviceParameterMVTechDio_InputNo5Name:
		case EDeviceParameterMVTechDio_InputNo6Name:
		case EDeviceParameterMVTechDio_InputNo7Name:
		case EDeviceParameterMVTechDio_InputNo8Name:
		case EDeviceParameterMVTechDio_InputNo9Name:
		case EDeviceParameterMVTechDio_InputNo10Name:
		case EDeviceParameterMVTechDio_InputNo11Name:
		case EDeviceParameterMVTechDio_InputNo12Name:
		case EDeviceParameterMVTechDio_InputNo13Name:
		case EDeviceParameterMVTechDio_InputNo14Name:
		case EDeviceParameterMVTechDio_InputNo15Name:
		case EDeviceParameterMVTechDio_InputNo16Name:
		case EDeviceParameterMVTechDio_InputNo17Name:
		case EDeviceParameterMVTechDio_InputNo18Name:
		case EDeviceParameterMVTechDio_InputNo19Name:
		case EDeviceParameterMVTechDio_InputNo20Name:
		case EDeviceParameterMVTechDio_InputNo21Name:
		case EDeviceParameterMVTechDio_InputNo22Name:
		case EDeviceParameterMVTechDio_InputNo23Name:
		case EDeviceParameterMVTechDio_InputNo24Name:
		case EDeviceParameterMVTechDio_InputNo25Name:
		case EDeviceParameterMVTechDio_InputNo26Name:
		case EDeviceParameterMVTechDio_InputNo27Name:
		case EDeviceParameterMVTechDio_InputNo28Name:
		case EDeviceParameterMVTechDio_InputNo29Name:
		case EDeviceParameterMVTechDio_InputNo30Name:
		case EDeviceParameterMVTechDio_InputNo31Name:
			{
				CString strPreValue = GetParamValue(nParam);

				if(SetParamValue(nParam, strValue))
				{
					m_pStrInputNameArray[nParam - EDeviceParameterMVTechDio_InputNo0Name] = strValue;
					bReturn = SaveSettings(nParam);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMVTechDio[nParam], strPreValue, strValue);
			}
			break;
		case EDeviceParameterMVTechDio_OutputNo0Name:
		case EDeviceParameterMVTechDio_OutputNo1Name:
		case EDeviceParameterMVTechDio_OutputNo2Name:
		case EDeviceParameterMVTechDio_OutputNo3Name:
		case EDeviceParameterMVTechDio_OutputNo4Name:
		case EDeviceParameterMVTechDio_OutputNo5Name:
		case EDeviceParameterMVTechDio_OutputNo6Name:
		case EDeviceParameterMVTechDio_OutputNo7Name:
		case EDeviceParameterMVTechDio_OutputNo8Name:
		case EDeviceParameterMVTechDio_OutputNo9Name:
		case EDeviceParameterMVTechDio_OutputNo10Name:
		case EDeviceParameterMVTechDio_OutputNo11Name:
		case EDeviceParameterMVTechDio_OutputNo12Name:
		case EDeviceParameterMVTechDio_OutputNo13Name:
		case EDeviceParameterMVTechDio_OutputNo14Name:
		case EDeviceParameterMVTechDio_OutputNo15Name:
		case EDeviceParameterMVTechDio_OutputNo16Name:
		case EDeviceParameterMVTechDio_OutputNo17Name:
		case EDeviceParameterMVTechDio_OutputNo18Name:
		case EDeviceParameterMVTechDio_OutputNo19Name:
		case EDeviceParameterMVTechDio_OutputNo20Name:
		case EDeviceParameterMVTechDio_OutputNo21Name:
		case EDeviceParameterMVTechDio_OutputNo22Name:
		case EDeviceParameterMVTechDio_OutputNo23Name:
		case EDeviceParameterMVTechDio_OutputNo24Name:
		case EDeviceParameterMVTechDio_OutputNo25Name:
		case EDeviceParameterMVTechDio_OutputNo26Name:
		case EDeviceParameterMVTechDio_OutputNo27Name:
		case EDeviceParameterMVTechDio_OutputNo28Name:
		case EDeviceParameterMVTechDio_OutputNo29Name:
		case EDeviceParameterMVTechDio_OutputNo30Name:
		case EDeviceParameterMVTechDio_OutputNo31Name:
			{
				CString strPreValue = GetParamValue(nParam);

				if(SetParamValue(nParam, strValue))
				{
					m_pStrOutputNameArray[nParam - EDeviceParameterMVTechDio_OutputNo0Name] = strValue;
					bReturn = SaveSettings(nParam);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMVTechDio[nParam], strPreValue, strValue);
			}
			break;
		default:
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtofindtheparameter);
			break;
		}

		if(EDeviceParameterMVTechDio_InputStatus < nParam && EDeviceParameterMVTechDio_OutputNo31Name >= nParam)
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

void CDeviceMVTechDio::OnBnClickedOutLed(UINT nID)
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

bool CDeviceMVTechDio::DoesModuleExist()
{
	bool bReturn = false;

	CString strModuleName;

	do
	{
#ifdef _WIN64
		strModuleName.Format(_T("PlxApi650_x64.dll"));
#else
		strModuleName.Format(_T("PlxApi650.dll"));
#endif

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