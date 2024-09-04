#include "stdafx.h"

#include "DeviceMFB_I.h"

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

IMPLEMENT_DYNAMIC(CDeviceMFB_I, CDeviceDio)

BEGIN_MESSAGE_MAP(CDeviceMFB_I, CDeviceDio)
	ON_COMMAND_RANGE(EDeviceAddEnumeratedControlID_OutputLedStart, EDeviceAddEnumeratedControlID_OutputLedEnd, OnBnClickedOutLed)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static LPCTSTR g_lpszDeviceMFB_I[EDeviceParameterMFB_I_Count] =
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
};

static LPCTSTR g_lpszMFB_ISwitch[2] =
{
	_T("Off"),
	_T("On")
};


CDeviceMFB_I::CDeviceMFB_I()
{
	m_parrDwCS = new DWORD[CDeviceMFB_I_DIO::MFB_MAX_ADDR_NUMS];
	m_parrDwCSLen = new DWORD[CDeviceMFB_I_DIO::MFB_MAX_ADDR_NUMS];

	memset(m_parrDwCS, 0, sizeof(DWORD) * CDeviceMFB_I_DIO::MFB_MAX_ADDR_NUMS);
	memset(m_parrDwCSLen, 0, sizeof(DWORD) * CDeviceMFB_I_DIO::MFB_MAX_ADDR_NUMS);
}


CDeviceMFB_I::~CDeviceMFB_I()
{
	Terminate();

	if(m_pDeviceObject)
	{
		delete m_pDeviceObject;
		m_pDeviceObject = nullptr;
	}

	if(m_parrDwCS)
	{
		delete[] m_parrDwCS;
		m_parrDwCS = nullptr;
	}

	if(m_parrDwCSLen)
	{
		delete[] m_parrDwCSLen;
		m_parrDwCSLen = nullptr;
	}
}

EDeviceInitializeResult CDeviceMFB_I::Initialize()
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

		DWORD dwRegValue[CDeviceMFB_I_DIO::MFB_LOCAL_CONFIG_REGCNT];

		memset(m_parrDwCS, 0, sizeof(m_parrDwCS));
		memset(m_parrDwCSLen, 0, sizeof(m_parrDwCSLen));
		memset(&plxDeviceKey, PCI_FIELD_IGNORE, sizeof(PLX_DEVICE_KEY));

		plxDeviceKey.VendorId = m_vendorId;
		plxDeviceKey.DeviceId = m_deviceId;

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

		for(int i = 0; i < CDeviceMFB_I_DIO::MFB_LOCAL_CONFIG_REGCNT; i++)
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

		m_parrDwCS[0] = dwRegValue[5] & ~0x00000001;
		m_parrDwCS[1] = dwRegValue[6] & ~0x00000001;
		m_parrDwCS[2] = dwRegValue[7] & ~0x00000001;
		m_parrDwCS[3] = dwRegValue[8] & ~0x00000001;

		m_parrDwCSLen[0] = dwRegValue[0] >> 4;
		m_parrDwCSLen[1] = dwRegValue[1] >> 4;
		m_parrDwCSLen[2] = dwRegValue[2] >> 4;
		m_parrDwCSLen[3] = dwRegValue[3] >> 4;

		m_bIsInitialized = true;

		bool bClear = false;
		GetUseClearInitalize(&bClear);

		if(bClear)
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

EDeviceTerminateResult CDeviceMFB_I::Terminate()
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

bool CDeviceMFB_I::LoadSettings()
{
	bool bReturn = false;

	m_vctParameterFieldConfigurations.clear();

	do
	{
		AddParameterFieldConfigurations(EDeviceParameterMFB_I_DeviceID, g_lpszDeviceMFB_I[EDeviceParameterMFB_I_DeviceID], _T("0"), EParameterFieldType_Edit);
		AddParameterFieldConfigurations(EDeviceParameterMFB_I_UseClearInitialize, g_lpszDeviceMFB_I[EDeviceParameterMFB_I_UseClearInitialize], _T("0"), EParameterFieldType_Check);
		AddParameterFieldConfigurations(EDeviceParameterMFB_I_UseClearTerminate, g_lpszDeviceMFB_I[EDeviceParameterMFB_I_UseClearTerminate], _T("0"), EParameterFieldType_Check);

		AddParameterFieldConfigurations(EDeviceParameterMFB_I_InputStatus, g_lpszDeviceMFB_I[EDeviceParameterMFB_I_InputStatus], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);

		for(int i = 0; i < CDeviceMFB_I_DIO::MFB_IO_IN_PORT; ++i)
		{
			AddParameterFieldConfigurations(EDeviceParameterMFB_I_UseInputNo0 + i, g_lpszDeviceMFB_I[EDeviceParameterMFB_I_UseInputNo0 + i], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 1);
		}

		AddParameterFieldConfigurations(EDeviceParameterMFB_I_OutputStatus, g_lpszDeviceMFB_I[EDeviceParameterMFB_I_OutputStatus], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);

		for(int i = 0; i < CDeviceMFB_I_DIO::MFB_IO_OUT_PORT; ++i)
		{
			AddParameterFieldConfigurations(EDeviceParameterMFB_I_UseOutputNo0 + i, g_lpszDeviceMFB_I[EDeviceParameterMFB_I_UseOutputNo0 + i], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 1);
		}

		AddParameterFieldConfigurations(EDeviceParameterMFB_I_InputName, g_lpszDeviceMFB_I[EDeviceParameterMFB_I_InputName], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);

		for(int i = 0; i < CDeviceMFB_I_DIO::MFB_IO_IN_PORT; ++i)
		{
			AddParameterFieldConfigurations(EDeviceParameterMFB_I_InputNo0Name + i, g_lpszDeviceMFB_I[EDeviceParameterMFB_I_InputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);
		}

		AddParameterFieldConfigurations(EDeviceParameterMFB_I_OutputName, g_lpszDeviceMFB_I[EDeviceParameterMFB_I_OutputName], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);

		for(int i = 0; i < CDeviceMFB_I_DIO::MFB_IO_OUT_PORT; ++i)
		{
			AddParameterFieldConfigurations(EDeviceParameterMFB_I_OutputNo0Name + i, g_lpszDeviceMFB_I[EDeviceParameterMFB_I_OutputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);
		}

		bReturn = __super::LoadSettings();

		AllocateIO(CDeviceMFB_I_DIO::MFB_IO_IN_PORT, CDeviceMFB_I_DIO::MFB_IO_OUT_PORT);

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

bool CDeviceMFB_I::PCIWrite(DWORD dwAddr, DWORD dwData)
{
	bool bReturn = false;

	Lock();

	do
	{
		PLX_STATUS rc;

		rc = PlxPci_PciBarSpaceWrite((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, dwAddr, &dwData, sizeof(dwData), BitSize32, true);

		if(rc != ApiSuccess)
			break;

		bReturn = true;
	}
	while(false);

	Unlock();

	return bReturn;
}


DWORD CDeviceMFB_I::PCIRead(DWORD dwAddr)
{
	DWORD dwData = 0;

	Lock();

	do
	{
		PLX_STATUS rc;

		rc = PlxPci_PciBarSpaceRead((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, dwAddr, &dwData, sizeof(dwData), BitSize32, true);

		if(rc != ApiSuccess)
			dwData = 0;
	}
	while(false);

	Unlock();

	return dwData;
}


DWORD CDeviceMFB_I::PciReadReg(DWORD dwOffset)
{
	DWORD dwRegValue = 0;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		PLX_STATUS rc;

		dwRegValue = PlxPci_PlxRegisterRead((PLX_DEVICE_OBJECT*)m_pDeviceObject, dwOffset, &rc);

		if(rc != ApiSuccess)
			dwRegValue = 0;
	}
	while(false);

	Unlock();

	return dwRegValue;
}


bool CDeviceMFB_I::PciWriteReg(DWORD dwOffset, DWORD dwData)
{
	bool bReturn = false;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		PLX_STATUS rc;

		rc = PlxPci_PlxRegisterWrite((PLX_DEVICE_OBJECT*)m_pDeviceObject, dwOffset, dwData);

		if(rc != ApiSuccess)
			break;

		bReturn = true;
	}
	while(false);

	Unlock();

	return bReturn;
}

bool CDeviceMFB_I::PCIWrite(DWORD dwCS, DWORD dwAddr, DWORD dwData)
{
	bool bReturn = false;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		if(dwCS >= CDeviceMFB_I_DIO::MFB_MAX_ADDR_NUMS)
			break;

		bReturn = PCIWrite(m_parrDwCS[dwCS] + dwAddr, dwData);
	}
	while(false);

	Unlock();

	return bReturn;
}


DWORD CDeviceMFB_I::PCIRead(DWORD dwCS, DWORD dwAddr)
{
	DWORD dwReturn = 0;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		if(dwCS >= CDeviceMFB_I_DIO::MFB_MAX_ADDR_NUMS)
			break;

		dwReturn = PCIRead(m_parrDwCS[dwCS] + dwAddr);
	}
	while(false);

	Unlock();

	return dwReturn;
}

bool CDeviceMFB_I::ReadOutBit(int nBit)
{
	bool bReturn = false;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		if(nBit >= CDeviceMFB_I_DIO::MFB_IO_OUT_PORT || nBit < 0)
			break;

		PLX_STATUS rc = PlxPci_PciBarSpaceRead((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_parrDwCS[CDeviceMFB_I_DIO::MFB_PCI_BASE_CS0] + CDeviceMFB_I_DIO::MFB_IO_OUT_READ, &m_dwOutStatus, sizeof(m_dwOutStatus), BitSize32, true);

		if(rc != ApiSuccess)
			break;

		bReturn = (m_dwOutStatus >> nBit) & 0x01;
	}
	while(false);

	Unlock();

	return bReturn;
}

bool CDeviceMFB_I::ReadInBit(int nBit)
{
	bool bReturn = false;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		if(nBit >= CDeviceMFB_I_DIO::MFB_IO_IN_PORT || nBit < 0)
			break;

		PLX_STATUS rc = PlxPci_PciBarSpaceRead((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_parrDwCS[CDeviceMFB_I_DIO::MFB_PCI_BASE_CS0] + CDeviceMFB_I_DIO::MFB_IO_IN_READ, &m_dwInStatus, sizeof(m_dwInStatus), BitSize32, true);

		if(rc != ApiSuccess)
			break;

		bReturn = (m_dwInStatus >> nBit) & 0x01;
	}
	while(false);

	Unlock();

	return bReturn;
}

BYTE CDeviceMFB_I::ReadOutByte()
{
	BYTE cReturn = 0;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		PLX_STATUS rc = PlxPci_PciBarSpaceRead((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_parrDwCS[CDeviceMFB_I_DIO::MFB_PCI_BASE_CS0] + CDeviceMFB_I_DIO::MFB_IO_OUT_READ, &m_dwOutStatus, sizeof(m_dwOutStatus), BitSize32, true);

		if(rc != ApiSuccess)
			break;

		cReturn = m_dwOutStatus & BYTE_MAX;
	}
	while(false);

	Unlock();

	return cReturn;
}

BYTE CDeviceMFB_I::ReadInByte()
{
	BYTE cReturn = 0;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		PLX_STATUS rc = PlxPci_PciBarSpaceRead((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_parrDwCS[CDeviceMFB_I_DIO::MFB_PCI_BASE_CS0] + CDeviceMFB_I_DIO::MFB_IO_IN_READ, &m_dwInStatus, sizeof(m_dwInStatus), BitSize32, true);

		if(rc != ApiSuccess)
			break;

		cReturn = m_dwInStatus & BYTE_MAX;
	}
	while(false);

	Unlock();

	return cReturn;
}

WORD CDeviceMFB_I::ReadOutWord()
{
	WORD wReturn = 0;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		PLX_STATUS rc = PlxPci_PciBarSpaceRead((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_parrDwCS[CDeviceMFB_I_DIO::MFB_PCI_BASE_CS0] + CDeviceMFB_I_DIO::MFB_IO_OUT_READ, &m_dwOutStatus, sizeof(m_dwOutStatus), BitSize32, true);

		if(rc != ApiSuccess)
			break;

		wReturn = m_dwOutStatus & WORD_MAX;
	}
	while(false);

	Unlock();

	return wReturn;
}

WORD CDeviceMFB_I::ReadInWord()
{
	WORD wReturn = 0;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		PLX_STATUS rc = PlxPci_PciBarSpaceRead((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_parrDwCS[CDeviceMFB_I_DIO::MFB_PCI_BASE_CS0] + CDeviceMFB_I_DIO::MFB_IO_IN_READ, &m_dwInStatus, sizeof(m_dwInStatus), BitSize32, true);

		if(rc != ApiSuccess)
			break;

		wReturn = m_dwInStatus & WORD_MAX;
	}
	while(false);

	Unlock();

	return wReturn;
}

DWORD CDeviceMFB_I::ReadOutDword()
{
	DWORD dwReturn = 0;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		PLX_STATUS rc = PlxPci_PciBarSpaceRead((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_parrDwCS[CDeviceMFB_I_DIO::MFB_PCI_BASE_CS0] + CDeviceMFB_I_DIO::MFB_IO_OUT_READ, &m_dwOutStatus, sizeof(m_dwOutStatus), BitSize32, true);

		if(rc != ApiSuccess)
			break;


		dwReturn = m_dwOutStatus & DWORD_MAX;
	}
	while(false);

	Unlock();

	return dwReturn;
}

DWORD CDeviceMFB_I::ReadInDword()
{
	DWORD dwReturn = 0;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		PLX_STATUS rc = PlxPci_PciBarSpaceRead((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_parrDwCS[CDeviceMFB_I_DIO::MFB_PCI_BASE_CS0] + CDeviceMFB_I_DIO::MFB_IO_IN_READ, &m_dwInStatus, sizeof(m_dwInStatus), BitSize32, true);

		if(rc != ApiSuccess)
			break;

		dwReturn = m_dwInStatus;
	}
	while(false);

	Unlock();

	return dwReturn;
}

bool CDeviceMFB_I::WriteOutBit(int nBit, bool bOn)
{
	bool bReturn = false;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		if(nBit >= CDeviceMFB_I_DIO::MFB_IO_OUT_PORT)
			break;

		if(bOn)
			m_dwOutStatus |= 1 << nBit;
		else
			m_dwOutStatus &= ~(1 << nBit);

		PLX_STATUS rc = PlxPci_PciBarSpaceWrite((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_parrDwCS[CDeviceMFB_I_DIO::MFB_PCI_BASE_CS0] + CDeviceMFB_I_DIO::MFB_IO_OUT_WRITE, &m_dwOutStatus, sizeof(m_dwOutStatus), BitSize32, true);

		bReturn = rc == ApiSuccess;
	}
	while(false);

	Unlock();

	return bReturn;
}

bool CDeviceMFB_I::WriteInBit(int nBit, bool bOn)
{
	return false;
}

bool CDeviceMFB_I::WriteOutByte(BYTE cByte)
{
	bool bReturn = false;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		m_dwOutStatus = cByte;

		PLX_STATUS rc = PlxPci_PciBarSpaceWrite((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_parrDwCS[CDeviceMFB_I_DIO::MFB_PCI_BASE_CS0] + CDeviceMFB_I_DIO::MFB_IO_OUT_WRITE, &m_dwOutStatus, sizeof(m_dwOutStatus), BitSize32, true);

		bReturn = rc == ApiSuccess;
	}
	while(false);

	Unlock();

	return bReturn;
}

bool CDeviceMFB_I::WriteInByte(BYTE cByte)
{
	return false;
}

bool CDeviceMFB_I::WriteOutWord(WORD wWord)
{
	bool bReturn = false;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		m_dwOutStatus = wWord;

		PLX_STATUS rc = PlxPci_PciBarSpaceWrite((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_parrDwCS[CDeviceMFB_I_DIO::MFB_PCI_BASE_CS0] + CDeviceMFB_I_DIO::MFB_IO_OUT_WRITE, &m_dwOutStatus, sizeof(m_dwOutStatus), BitSize32, true);

		bReturn = rc == ApiSuccess;
	}
	while(false);

	Unlock();

	return bReturn;
}

bool CDeviceMFB_I::WriteInWord(WORD wWord)
{
	return false;
}

bool CDeviceMFB_I::WriteOutDword(DWORD dwDword)
{
	bool bReturn = false;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		m_dwOutStatus = dwDword;

		PLX_STATUS rc = PlxPci_PciBarSpaceWrite((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_parrDwCS[CDeviceMFB_I_DIO::MFB_PCI_BASE_CS0] + CDeviceMFB_I_DIO::MFB_IO_OUT_WRITE, &m_dwOutStatus, sizeof(m_dwOutStatus), BitSize32, true);

		bReturn = rc == ApiSuccess;
	}
	while(false);

	Unlock();

	return bReturn;
}

bool CDeviceMFB_I::WriteInDword(DWORD dwDword)
{
	return false;
}

bool CDeviceMFB_I::ClearOutPort()
{
	return WriteOutDword(0);
}

bool CDeviceMFB_I::ClearInPort()
{
	return false;
}

long CDeviceMFB_I::GetOutputPortCount()
{
	return CDeviceMFB_I_DIO::MFB_IO_OUT_PORT;
}

long CDeviceMFB_I::GetInputPortCount()
{
	return CDeviceMFB_I_DIO::MFB_IO_IN_PORT;
}

bool CDeviceMFB_I::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = false;

	bool bFoundID = false;

	CString strMessage;

	do
	{
		switch(nParam)
		{
		case EDeviceParameterMFB_I_DeviceID:
			bReturn = !SetDeviceID(strValue);
			bFoundID = true;
			break;
		case EDeviceParameterMFB_I_UseClearInitialize:
			bReturn = !SetUseClearInitalize(_ttoi(strValue));
			bFoundID = true;
			break;
		case EDeviceParameterMFB_I_UseClearTerminate:
			bReturn = !SetUseClearTerminate(_ttoi(strValue));
			bFoundID = true;
			break;
		case EDeviceParameterMFB_I_UseInputNo0:
		case EDeviceParameterMFB_I_UseInputNo1:
		case EDeviceParameterMFB_I_UseInputNo2:
		case EDeviceParameterMFB_I_UseInputNo3:
		case EDeviceParameterMFB_I_UseInputNo4:
		case EDeviceParameterMFB_I_UseInputNo5:
		case EDeviceParameterMFB_I_UseInputNo6:
		case EDeviceParameterMFB_I_UseInputNo7:
		case EDeviceParameterMFB_I_UseInputNo8:
		case EDeviceParameterMFB_I_UseInputNo9:
		case EDeviceParameterMFB_I_UseInputNo10:
		case EDeviceParameterMFB_I_UseInputNo11:
		case EDeviceParameterMFB_I_UseInputNo12:
		case EDeviceParameterMFB_I_UseInputNo13:
		case EDeviceParameterMFB_I_UseInputNo14:
		case EDeviceParameterMFB_I_UseInputNo15:
		case EDeviceParameterMFB_I_UseInputNo16:
		case EDeviceParameterMFB_I_UseInputNo17:
		case EDeviceParameterMFB_I_UseInputNo18:
		case EDeviceParameterMFB_I_UseInputNo19:
		case EDeviceParameterMFB_I_UseInputNo20:
		case EDeviceParameterMFB_I_UseInputNo21:
		case EDeviceParameterMFB_I_UseInputNo22:
		case EDeviceParameterMFB_I_UseInputNo23:
		case EDeviceParameterMFB_I_UseInputNo24:
		case EDeviceParameterMFB_I_UseInputNo25:
		case EDeviceParameterMFB_I_UseInputNo26:
		case EDeviceParameterMFB_I_UseInputNo27:
		case EDeviceParameterMFB_I_UseInputNo28:
		case EDeviceParameterMFB_I_UseInputNo29:
		case EDeviceParameterMFB_I_UseInputNo30:
		case EDeviceParameterMFB_I_UseInputNo31:
			{
				int nPreValue = _ttoi(GetParamValue(nParam));
				int nValue = _ttoi(strValue);

				if(SetParamValue(nParam, strValue))
				{
					m_pBUsingInputArray[nParam - EDeviceParameterMFB_I_UseInputNo0] = _ttoi(strValue);
					bReturn = SaveSettings(nParam);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMFB_I[nParam], g_lpszMFB_ISwitch[nPreValue], g_lpszMFB_ISwitch[nValue]);
			}
			break;
		case EDeviceParameterMFB_I_UseOutputNo0:
		case EDeviceParameterMFB_I_UseOutputNo1:
		case EDeviceParameterMFB_I_UseOutputNo2:
		case EDeviceParameterMFB_I_UseOutputNo3:
		case EDeviceParameterMFB_I_UseOutputNo4:
		case EDeviceParameterMFB_I_UseOutputNo5:
		case EDeviceParameterMFB_I_UseOutputNo6:
		case EDeviceParameterMFB_I_UseOutputNo7:
		case EDeviceParameterMFB_I_UseOutputNo8:
		case EDeviceParameterMFB_I_UseOutputNo9:
		case EDeviceParameterMFB_I_UseOutputNo10:
		case EDeviceParameterMFB_I_UseOutputNo11:
		case EDeviceParameterMFB_I_UseOutputNo12:
		case EDeviceParameterMFB_I_UseOutputNo13:
		case EDeviceParameterMFB_I_UseOutputNo14:
		case EDeviceParameterMFB_I_UseOutputNo15:
		case EDeviceParameterMFB_I_UseOutputNo16:
		case EDeviceParameterMFB_I_UseOutputNo17:
		case EDeviceParameterMFB_I_UseOutputNo18:
		case EDeviceParameterMFB_I_UseOutputNo19:
		case EDeviceParameterMFB_I_UseOutputNo20:
		case EDeviceParameterMFB_I_UseOutputNo21:
		case EDeviceParameterMFB_I_UseOutputNo22:
		case EDeviceParameterMFB_I_UseOutputNo23:
		case EDeviceParameterMFB_I_UseOutputNo24:
		case EDeviceParameterMFB_I_UseOutputNo25:
		case EDeviceParameterMFB_I_UseOutputNo26:
		case EDeviceParameterMFB_I_UseOutputNo27:
		case EDeviceParameterMFB_I_UseOutputNo28:
		case EDeviceParameterMFB_I_UseOutputNo29:
		case EDeviceParameterMFB_I_UseOutputNo30:
		case EDeviceParameterMFB_I_UseOutputNo31:
			{
				int nPreValue = _ttoi(GetParamValue(nParam));
				int nValue = _ttoi(strValue);

				if(SetParamValue(nParam, strValue))
				{
					m_pBUsingOutputArray[nParam - EDeviceParameterMFB_I_UseOutputNo0] = _ttoi(strValue);
					bReturn = SaveSettings(nParam);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMFB_I[nParam], g_lpszMFB_ISwitch[nPreValue], g_lpszMFB_ISwitch[nValue]);
			}
			break;
		case EDeviceParameterMFB_I_InputNo0Name:
		case EDeviceParameterMFB_I_InputNo1Name:
		case EDeviceParameterMFB_I_InputNo2Name:
		case EDeviceParameterMFB_I_InputNo3Name:
		case EDeviceParameterMFB_I_InputNo4Name:
		case EDeviceParameterMFB_I_InputNo5Name:
		case EDeviceParameterMFB_I_InputNo6Name:
		case EDeviceParameterMFB_I_InputNo7Name:
		case EDeviceParameterMFB_I_InputNo8Name:
		case EDeviceParameterMFB_I_InputNo9Name:
		case EDeviceParameterMFB_I_InputNo10Name:
		case EDeviceParameterMFB_I_InputNo11Name:
		case EDeviceParameterMFB_I_InputNo12Name:
		case EDeviceParameterMFB_I_InputNo13Name:
		case EDeviceParameterMFB_I_InputNo14Name:
		case EDeviceParameterMFB_I_InputNo15Name:
		case EDeviceParameterMFB_I_InputNo16Name:
		case EDeviceParameterMFB_I_InputNo17Name:
		case EDeviceParameterMFB_I_InputNo18Name:
		case EDeviceParameterMFB_I_InputNo19Name:
		case EDeviceParameterMFB_I_InputNo20Name:
		case EDeviceParameterMFB_I_InputNo21Name:
		case EDeviceParameterMFB_I_InputNo22Name:
		case EDeviceParameterMFB_I_InputNo23Name:
		case EDeviceParameterMFB_I_InputNo24Name:
		case EDeviceParameterMFB_I_InputNo25Name:
		case EDeviceParameterMFB_I_InputNo26Name:
		case EDeviceParameterMFB_I_InputNo27Name:
		case EDeviceParameterMFB_I_InputNo28Name:
		case EDeviceParameterMFB_I_InputNo29Name:
		case EDeviceParameterMFB_I_InputNo30Name:
		case EDeviceParameterMFB_I_InputNo31Name:
			{
				CString strPreValue = GetParamValue(nParam);

				if(SetParamValue(nParam, strValue))
				{
					m_pStrInputNameArray[nParam - EDeviceParameterMFB_I_InputNo0Name] = strValue;
					bReturn = SaveSettings(nParam);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMFB_I[nParam], strPreValue, strValue);
			}
			break;
		case EDeviceParameterMFB_I_OutputNo0Name:
		case EDeviceParameterMFB_I_OutputNo1Name:
		case EDeviceParameterMFB_I_OutputNo2Name:
		case EDeviceParameterMFB_I_OutputNo3Name:
		case EDeviceParameterMFB_I_OutputNo4Name:
		case EDeviceParameterMFB_I_OutputNo5Name:
		case EDeviceParameterMFB_I_OutputNo6Name:
		case EDeviceParameterMFB_I_OutputNo7Name:
		case EDeviceParameterMFB_I_OutputNo8Name:
		case EDeviceParameterMFB_I_OutputNo9Name:
		case EDeviceParameterMFB_I_OutputNo10Name:
		case EDeviceParameterMFB_I_OutputNo11Name:
		case EDeviceParameterMFB_I_OutputNo12Name:
		case EDeviceParameterMFB_I_OutputNo13Name:
		case EDeviceParameterMFB_I_OutputNo14Name:
		case EDeviceParameterMFB_I_OutputNo15Name:
		case EDeviceParameterMFB_I_OutputNo16Name:
		case EDeviceParameterMFB_I_OutputNo17Name:
		case EDeviceParameterMFB_I_OutputNo18Name:
		case EDeviceParameterMFB_I_OutputNo19Name:
		case EDeviceParameterMFB_I_OutputNo20Name:
		case EDeviceParameterMFB_I_OutputNo21Name:
		case EDeviceParameterMFB_I_OutputNo22Name:
		case EDeviceParameterMFB_I_OutputNo23Name:
		case EDeviceParameterMFB_I_OutputNo24Name:
		case EDeviceParameterMFB_I_OutputNo25Name:
		case EDeviceParameterMFB_I_OutputNo26Name:
		case EDeviceParameterMFB_I_OutputNo27Name:
		case EDeviceParameterMFB_I_OutputNo28Name:
		case EDeviceParameterMFB_I_OutputNo29Name:
		case EDeviceParameterMFB_I_OutputNo30Name:
		case EDeviceParameterMFB_I_OutputNo31Name:
			{
				CString strPreValue = GetParamValue(nParam);

				if(SetParamValue(nParam, strValue))
				{
					m_pStrOutputNameArray[nParam - EDeviceParameterMFB_I_OutputNo0Name] = strValue;
					bReturn = SaveSettings(nParam);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMFB_I[nParam], strPreValue, strValue);
			}
			break;
		default:
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtofindtheparameter);
			break;
		}

		if(EDeviceParameterMFB_I_InputStatus < nParam && EDeviceParameterMFB_I_OutputNo31Name >= nParam)
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

void CDeviceMFB_I::OnBnClickedOutLed(UINT nID)
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


BOOL CDeviceMFB_I::OnInitDialog()
{
	CDeviceDio::OnInitDialog();

	SetTimer(RAVID_TIMER_DIO, 10, nullptr);

	return TRUE;
}

void CDeviceMFB_I::OnDestroy()
{
	CDeviceDio::OnDestroy();

	KillTimer(RAVID_TIMER_DIO);
}

EGetFunctionMFB_I CDeviceMFB_I::GetUseClearInitalize(bool * pParam)
{
	EGetFunctionMFB_I eReturn = EGetFunctionMFB_I_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGetFunctionMFB_I_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMFB_I_UseClearInitialize));

		eReturn = EGetFunctionMFB_I_OK;
	}
	while(false);

	return eReturn;
}

ESetFunctionMFB_I CDeviceMFB_I::SetUseClearInitalize(bool bParam)
{
	ESetFunctionMFB_I eReturn = ESetFunctionMFB_I_UnknownError;

	EDeviceParameterMFB_I eSaveNum = EDeviceParameterMFB_I_UseClearInitialize;

	bool bPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = ESetFunctionMFB_I_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = ESetFunctionMFB_I_WriteToDatabaseError;
			break;
		}

		eReturn = ESetFunctionMFB_I_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMFB_I[eSaveNum], g_lpszMFB_ISwitch[bPreValue], g_lpszMFB_ISwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGetFunctionMFB_I CDeviceMFB_I::GetUseClearTerminate(bool * pParam)
{
	EGetFunctionMFB_I eReturn = EGetFunctionMFB_I_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGetFunctionMFB_I_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMFB_I_UseClearTerminate));

		eReturn = EGetFunctionMFB_I_OK;
	}
	while(false);

	return eReturn;
}

ESetFunctionMFB_I CDeviceMFB_I::SetUseClearTerminate(bool bParam)
{
	ESetFunctionMFB_I eReturn = ESetFunctionMFB_I_UnknownError;

	EDeviceParameterMFB_I eSaveNum = EDeviceParameterMFB_I_UseClearTerminate;

	bool bPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = ESetFunctionMFB_I_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = ESetFunctionMFB_I_WriteToDatabaseError;
			break;
		}

		eReturn = ESetFunctionMFB_I_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMFB_I[eSaveNum], g_lpszMFB_ISwitch[bPreValue], g_lpszMFB_ISwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

void CDeviceMFB_I::SetPCIVendorID(unsigned short vendorId)
{
	m_vendorId = vendorId;
}

void CDeviceMFB_I::SetPCIDeviceID(unsigned short deviceId)
{
	m_deviceId = deviceId;
}

void CDeviceMFB_I::OnTimer(UINT_PTR nIDEvent)
{
	if(IsWindowVisible())
	{
		if(nIDEvent == RAVID_TIMER_DIO)
		{
			for(int i = 0; i < 32; ++i)
			{
				if(m_pBUsingInputArray[i])
				{
					CRavidLedCtrl* pLed = (CRavidLedCtrl*)GetDlgItem(EDeviceAddEnumeratedControlID_InputLedStart + i);

					if(pLed)
					{
						pLed->SetActivate(IsInitialized());

						if(ReadInBit(i))
							pLed->On();
						else
							pLed->Off();
					}
				}

				if(m_pBUsingOutputArray[i])
				{
					CRavidLedCtrl* pLed = (CRavidLedCtrl*)GetDlgItem(EDeviceAddEnumeratedControlID_OutputLedStart + i);

					if(pLed)
					{
						pLed->SetActivate(IsInitialized());

						if(ReadOutBit(i))
							pLed->On();
						else
							pLed->Off();
					}
				}
			}
		}		
	}

	CDeviceDio::OnTimer(nIDEvent);
}

bool CDeviceMFB_I::DoesModuleExist()
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