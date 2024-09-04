#include "stdafx.h"

#include "DeviceMVTechMFB_I.h"

#ifdef USING_DEVICE

#include "../RavidCore/RavidPoint.h"
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

#include "../RavidFramework/MessageBase.h"
#include "../RavidFramework/RavidLedCtrl.h"

#include "../Libraries/Includes/PlxApi/PlxApi.h"

// plxapi650.dll
// plxapi650_x64.dll
#pragma comment(lib, COMMONLIB_PREFIX "PlxApi/PlxApi.lib")

#define CTOINT32(x) int32_t(x)

using namespace Ravid;
using namespace Ravid::Device;
using namespace Ravid::Database;
using namespace Ravid::Framework;
using namespace Ravid::Algorithms;
using namespace Ravid::Miscellaneous;

IMPLEMENT_DYNAMIC(CDeviceMVTechMFB_I, CDeviceTrigger)

BEGIN_MESSAGE_MAP(CDeviceMVTechMFB_I, CDeviceTrigger)
	ON_COMMAND_RANGE(EDeviceAddEnumeratedControlID_OutputLedStart, EDeviceAddEnumeratedControlID_OutputLedEnd, OnBnClickedOutLed)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static LPCTSTR g_lpszDeviceMFB_I[EDeviceParameterMVTechMFB_I_Count] =
{
	_T("DeviceID"),
	_T("Use Clear Initalize"),
	_T("Use Clear Terminate"),
	_T("Trigger Infomation"),
	_T("Start Position"),
	_T("End Position"),
	_T("Trigger Cycle"),
	_T("Trigger On time"),
	_T("Encoder Direction"),
	_T("Input Status"),
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
	_T("Output Status"),
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
};

static LPCTSTR g_lpszSwitch[EDeviceSwitch_Count] =
{
	_T("OFF"),
	_T("ON"),
};

static LPCTSTR g_lpszSimplexDirection[EDeviceSimplexDirection_Count] =
{
	_T("CW"),
	_T("CCW"),
};


CDeviceMVTechMFB_I::CDeviceMVTechMFB_I() :
	m_bTriggerEnabled(false),
	m_dwInStatus(0),
	m_dwOutStatus(0),
	m_pUsedInput(nullptr),
	m_pUsedOutput(nullptr),
	m_pStrInputName(nullptr),
	m_pStrOutputName(nullptr),
	m_pDwCS(nullptr),
	m_pDwCSLen(nullptr),
	m_pDeviceObject(nullptr),
	m_vendorId(0x10B8),
	m_deviceId(0x9030)
{
	const int32_t i32UserCount = CTOINT32(MFB_I::BoardInfo::MAX_BASE_ADDR_NUMS);

	m_pDwCS = new DWORD[i32UserCount];
	memset(m_pDwCS, 0, sizeof(DWORD) * i32UserCount);

	m_pDwCSLen = new DWORD[i32UserCount];
	memset(m_pDwCSLen, 0, sizeof(DWORD) * i32UserCount);
}

CDeviceMVTechMFB_I::~CDeviceMVTechMFB_I()
{
	FreeIO();

	if(m_pDwCS)
	{
		delete[] m_pDwCS;
		m_pDwCS = nullptr;
	}

	if(m_pDwCSLen)
	{
		delete[] m_pDwCSLen;
		m_pDwCSLen = nullptr;
	}

	if(m_pDeviceObject)
	{
		delete m_pDeviceObject;
		m_pDeviceObject = nullptr;
	}
}

EDeviceInitializeResult CDeviceMVTechMFB_I::Initialize()
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

		const int32_t nRegCount = CTOINT32(MFB_I::BoardInfo::RegisterCount);

		PLX_STATUS rc;
		PLX_DEVICE_KEY plxDeviceKey;

		DWORD dwRegValue[nRegCount];

		memset(m_pDwCS, 0, sizeof(m_pDwCS));
		memset(m_pDwCSLen, 0, sizeof(m_pDwCSLen));
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

		for(int i = 0; i < nRegCount; i++)
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

		m_pDwCS[0] = dwRegValue[5] & ~0x00000001;
		m_pDwCS[1] = dwRegValue[6] & ~0x00000001;
		m_pDwCS[2] = dwRegValue[7] & ~0x00000001;
		m_pDwCS[3] = dwRegValue[8] & ~0x00000001;

		m_pDwCSLen[0] = dwRegValue[0] >> 4;
		m_pDwCSLen[1] = dwRegValue[1] >> 4;
		m_pDwCSLen[2] = dwRegValue[2] >> 4;
		m_pDwCSLen[3] = dwRegValue[3] >> 4;

		m_bIsInitialized = true;

		if(true)
		{
			int nStart = 1;
			this->GetStartPosition(&nStart);

			int nEnd = INT_MAX;
			this->GetEndPosition(&nEnd);

			int nCycle = 0;
			this->GetTriggerCycle(&nCycle);

			int nTime = 0;
			this->GetTriggerOnTime(&nTime);

			EDeviceSimplexDirection eType = EDeviceSimplexDirection_CW;
			this->GetEncoderDirection(&eType);

			const int32_t i32UserArea = CTOINT32(MFB_I::BoardInfo::PCI_BASE_CS0);

			{
				const DWORD dwAddress = DWORD(CTOINT32(MFB_I::AddressMap::EncoderBeginPosition));
				PCIWrite(i32UserArea, dwAddress, DWORD(nStart));
			}
			{
				const DWORD dwAddress = DWORD(CTOINT32(MFB_I::AddressMap::EncoderEndPosition));
				PCIWrite(i32UserArea, dwAddress, DWORD(nEnd));
			}
			{
				const DWORD dwAddress = DWORD(CTOINT32(MFB_I::AddressMap::SettingTriggerEvent));
				DWORD dwCommand = nTime << 8 | nCycle;
				PCIWrite(i32UserArea, dwAddress, dwCommand);
			}
			{
				const DWORD dwAddress = DWORD(CTOINT32(MFB_I::AddressMap::EncoderDirection));
				DWORD dwCommand = eType == EDeviceSimplexDirection_CW ? 0x00 : 0x01;				
				PCIWrite(i32UserArea, dwAddress, dwCommand);
			}				
		}
		
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

EDeviceTerminateResult CDeviceMVTechMFB_I::Terminate()
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
		bool bClear = false;
		GetUseClearTerminate(&bClear);
		if(bClear)
			ClearOutPort();

		m_bIsInitialized = false;

		if(m_pDeviceObject)
		{
			DisableTrigger();

			ResetCounter();

			PlxPci_DeviceClose((PLX_DEVICE_OBJECT*)m_pDeviceObject);

			delete m_pDeviceObject;
			m_pDeviceObject = nullptr;
		} 


		strMessage = CMultiLanguageManager::GetString(ELanguageParameter_TheDevicehasbeensuccessfullyterminated);

		CEventHandlerManager::BroadcastOnDeviceTerminated(this);
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceMVTechMFB_I::LoadSettings()
{
	bool bReturn = false;

	m_vctParameterFieldConfigurations.clear();

	do
	{
		AddParameterFieldConfigurations(EDeviceParameterMVTechMFB_I_DeviceID, g_lpszDeviceMFB_I[EDeviceParameterMVTechMFB_I_DeviceID], _T("0"), EParameterFieldType_Edit);
		AddParameterFieldConfigurations(EDeviceParameterMVTechMFB_I_UseClearInitialize, g_lpszDeviceMFB_I[EDeviceParameterMVTechMFB_I_UseClearInitialize], _T("1"), EParameterFieldType_Check);
		AddParameterFieldConfigurations(EDeviceParameterMVTechMFB_I_UseClearTerminate, g_lpszDeviceMFB_I[EDeviceParameterMVTechMFB_I_UseClearTerminate], _T("1"), EParameterFieldType_Check);

		AddParameterFieldConfigurations(EDeviceParameterMVTechMFB_I_TriggerInfo, g_lpszDeviceMFB_I[EDeviceParameterMVTechMFB_I_TriggerInfo], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);

		AddParameterFieldConfigurations(EDeviceParameterMVTechMFB_I_Startposition, g_lpszDeviceMFB_I[EDeviceParameterMVTechMFB_I_Startposition], _T("0"), EParameterFieldType_Edit, nullptr, _T("Start Encoder"), 1);
 		AddParameterFieldConfigurations(EDeviceParameterMVTechMFB_I_Endposition, g_lpszDeviceMFB_I[EDeviceParameterMVTechMFB_I_Endposition], _T("0"), EParameterFieldType_Edit, nullptr, _T("Not Support"), 1);
 		AddParameterFieldConfigurations(EDeviceParameterMVTechMFB_I_TriggerCycle, g_lpszDeviceMFB_I[EDeviceParameterMVTechMFB_I_TriggerCycle], _T("0"), EParameterFieldType_Edit, nullptr, _T("0 ~ 255"), 1);
 		AddParameterFieldConfigurations(EDeviceParameterMVTechMFB_I_TriggerOntime, g_lpszDeviceMFB_I[EDeviceParameterMVTechMFB_I_TriggerOntime], _T("0"), EParameterFieldType_Edit, nullptr, _T("0 ~ 255"), 1);
 		AddParameterFieldConfigurations(EDeviceParameterMVTechMFB_I_EncoderDirection, g_lpszDeviceMFB_I[EDeviceParameterMVTechMFB_I_EncoderDirection], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszSimplexDirection, EDeviceSimplexDirection_Count), nullptr, 1);
 		
 		AddParameterFieldConfigurations(EDeviceParameterMVTechMFB_I_InputStatus, g_lpszDeviceMFB_I[EDeviceParameterMVTechMFB_I_InputStatus], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
 
 		for(int i = 0; i < CTOINT32(MFB_I::BoardInfo::InportCount); ++i)
 			AddParameterFieldConfigurations(EDeviceParameterMVTechMFB_I_UseInputNo0 + i, g_lpszDeviceMFB_I[EDeviceParameterMVTechMFB_I_UseInputNo0 + i], _T("1"), EParameterFieldType_Check, nullptr, nullptr, 1);
 
 		AddParameterFieldConfigurations(EDeviceParameterMVTechMFB_I_OutputStatus, g_lpszDeviceMFB_I[EDeviceParameterMVTechMFB_I_OutputStatus], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
 
 		for(int i = 0; i < CTOINT32(MFB_I::BoardInfo::OutportCount); ++i)
 			AddParameterFieldConfigurations(EDeviceParameterMVTechMFB_I_UseOutputNo0 + i, g_lpszDeviceMFB_I[EDeviceParameterMVTechMFB_I_UseOutputNo0 + i], _T("1"), EParameterFieldType_Check, nullptr, nullptr, 1);
 
 		AddParameterFieldConfigurations(EDeviceParameterMVTechMFB_I_InputName, g_lpszDeviceMFB_I[EDeviceParameterMVTechMFB_I_InputName], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
 
 		for(int i = 0; i < CTOINT32(MFB_I::BoardInfo::InportCount); ++i)
 			AddParameterFieldConfigurations(EDeviceParameterMVTechMFB_I_InputNo0Name + i, g_lpszDeviceMFB_I[EDeviceParameterMVTechMFB_I_InputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);
 
 		AddParameterFieldConfigurations(EDeviceParameterMVTechMFB_I_OutputName, g_lpszDeviceMFB_I[EDeviceParameterMVTechMFB_I_OutputName], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
 
 		for(int i = 0; i < CTOINT32(MFB_I::BoardInfo::OutportCount); ++i)
 			AddParameterFieldConfigurations(EDeviceParameterMVTechMFB_I_OutputNo0Name + i, g_lpszDeviceMFB_I[EDeviceParameterMVTechMFB_I_OutputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);

 		bReturn = __super::LoadSettings();

		AllocateIO(CTOINT32(MFB_I::BoardInfo::InportCount), CTOINT32(MFB_I::BoardInfo::OutportCount));

		for(auto iter = m_vctParameterFieldConfigurations.begin(); iter != m_vctParameterFieldConfigurations.end(); ++iter)
		{
			if(iter->strParameterName.Find(_T("Use Input No. ")) >= 0)
			{
				int nNumber = _ttoi(iter->strParameterName.Mid(14, 2));

				m_pUsedInput[nNumber] = _ttoi(iter->strParameterValue);
			}
			else if(iter->strParameterName.Find(_T("Use Output No. ")) >= 0)
			{
				int nNumber = _ttoi(iter->strParameterName.Mid(15, 2));

				m_pUsedOutput[nNumber] = _ttoi(iter->strParameterValue);
			}
			else if(iter->strParameterName.Find(_T("Input No. ")) >= 0)
			{
				int nNumber = _ttoi(iter->strParameterName.Mid(10, 2));

				m_pStrInputName[nNumber] = iter->strParameterValue;
			}
			else if(iter->strParameterName.Find(_T("Output No. ")) >= 0)
			{
				int nNumber = _ttoi(iter->strParameterName.Mid(11, 2));

				m_pStrOutputName[nNumber] = iter->strParameterValue;
			}
			else
				bReturn = false;
		}
	}
	while(false);

	return bReturn;
}

bool CDeviceMVTechMFB_I::PCIWrite(DWORD dwAddr, DWORD dwData)
{
	bool bReturn = false;

	do
	{
		PLX_STATUS rc;

		rc = PlxPci_PciBarSpaceWrite((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, dwAddr, &dwData, sizeof(dwData), BitSize32, true);

		if(rc != ApiSuccess)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}


DWORD CDeviceMVTechMFB_I::PCIRead(DWORD dwAddr)
{
	DWORD dwData = 0;

	do
	{
		PLX_STATUS rc;

		rc = PlxPci_PciBarSpaceRead((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, dwAddr, &dwData, sizeof(dwData), BitSize32, true);

		if(rc != ApiSuccess)
			dwData = 0;
	}
	while(false);

	return dwData;
}


DWORD CDeviceMVTechMFB_I::PciReadReg(DWORD dwOffset)
{
	DWORD dwRegValue = 0;

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

	return dwRegValue;
}


bool CDeviceMVTechMFB_I::PciWriteReg(DWORD dwOffset, DWORD dwData)
{
	bool bReturn = false;

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

	return bReturn;
}

bool CDeviceMVTechMFB_I::PCIWrite(DWORD dwCS, DWORD dwAddr, DWORD dwData)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		const int32_t i32UserCount = CTOINT32(MFB_I::BoardInfo::MAX_BASE_ADDR_NUMS);
		if(dwCS >= i32UserCount)
			break;

		bReturn = PCIWrite(m_pDwCS[dwCS] + dwAddr, dwData);
	}
	while(false);

	return bReturn;
}


DWORD CDeviceMVTechMFB_I::PCIRead(DWORD dwCS, DWORD dwAddr)
{
	DWORD dwReturn = 0;

	do
	{
		if(!IsInitialized())
			break;

		const int32_t i32UserCount = CTOINT32(MFB_I::BoardInfo::MAX_BASE_ADDR_NUMS);
		if(dwCS >= i32UserCount)
			break;

		dwReturn = PCIRead(m_pDwCS[dwCS] + dwAddr);
	}
	while(false);

	return dwReturn;
}

bool CDeviceMVTechMFB_I::ReadOutBit(int nBit)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(nBit >= CTOINT32(MFB_I::BoardInfo::OutportCount) || nBit < 0)
			break;

		const int32_t i32UserArea = CTOINT32(MFB_I::BoardInfo::PCI_BASE_CS0);
		const DWORD dwAddress = DWORD(CTOINT32(MFB_I::AddressMap::OutputRead));

		PLX_STATUS rc = PlxPci_PciBarSpaceRead((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_pDwCS[i32UserArea] + dwAddress, &m_dwOutStatus, sizeof(m_dwOutStatus), BitSize32, true);
		if(rc != ApiSuccess)
			break;

		bReturn = (m_dwOutStatus >> nBit) & 0x01;
	}
	while(false);

	return bReturn;
}

bool CDeviceMVTechMFB_I::ReadInBit(int nBit)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(nBit >= CTOINT32(MFB_I::BoardInfo::InportCount) || nBit < 0)
			break;

		const int32_t i32UserArea = CTOINT32(MFB_I::BoardInfo::PCI_BASE_CS0);
		const DWORD dwAddress = DWORD(CTOINT32(MFB_I::AddressMap::Input));

		PLX_STATUS rc = PlxPci_PciBarSpaceRead((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_pDwCS[i32UserArea] + dwAddress, &m_dwInStatus, sizeof(m_dwInStatus), BitSize32, true);
		if(rc != ApiSuccess)
			break;

		bReturn = (m_dwInStatus >> nBit) & 0x01;
	}
	while(false);

	return bReturn;
}

BYTE CDeviceMVTechMFB_I::ReadOutByte()
{
	BYTE cReturn = 0;

	do
	{
		if(!IsInitialized())
			break;

		const int32_t i32UserArea = CTOINT32(MFB_I::BoardInfo::PCI_BASE_CS0);
		const DWORD dwAddress = DWORD(CTOINT32(MFB_I::AddressMap::OutputRead));

		PLX_STATUS rc = PlxPci_PciBarSpaceRead((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_pDwCS[i32UserArea] + dwAddress, &m_dwOutStatus, sizeof(m_dwOutStatus), BitSize32, true);
		if(rc != ApiSuccess)
			break;

		cReturn = m_dwOutStatus & BYTE_MAX;
	}
	while(false);

	return cReturn;
}

BYTE CDeviceMVTechMFB_I::ReadInByte()
{
	BYTE cReturn = 0;
	
	Lock();

	do
	{
		if(!IsInitialized())
			break;

		const int32_t i32UserArea = CTOINT32(MFB_I::BoardInfo::PCI_BASE_CS0);
		const DWORD dwAddress = DWORD(CTOINT32(MFB_I::AddressMap::Input));

		PLX_STATUS rc = PlxPci_PciBarSpaceRead((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_pDwCS[i32UserArea] + dwAddress, &m_dwInStatus, sizeof(m_dwInStatus), BitSize32, true);
		if(rc != ApiSuccess)
			break;

		cReturn = m_dwInStatus & BYTE_MAX;
	}
	while(false);

	Unlock();

	return cReturn;
}

WORD CDeviceMVTechMFB_I::ReadOutWord()
{
	WORD wReturn = 0;
	
	Lock();

	do
	{
		if(!IsInitialized())
			break;

		const int32_t i32UserArea = CTOINT32(MFB_I::BoardInfo::PCI_BASE_CS0);
		const DWORD dwAddress = DWORD(CTOINT32(MFB_I::AddressMap::OutputRead));

		PLX_STATUS rc = PlxPci_PciBarSpaceRead((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_pDwCS[i32UserArea] + dwAddress, &m_dwOutStatus, sizeof(m_dwOutStatus), BitSize32, true);
		if(rc != ApiSuccess)
			break;

		wReturn = m_dwOutStatus & WORD_MAX;
	}
	while(false);

	Unlock();

	return wReturn;
}

WORD CDeviceMVTechMFB_I::ReadInWord()
{
	WORD wReturn = 0;
	
	Lock();

	do
	{
		if(!IsInitialized())
			break;

		const int32_t i32UserArea = CTOINT32(MFB_I::BoardInfo::PCI_BASE_CS0);
		const DWORD dwAddress = DWORD(CTOINT32(MFB_I::AddressMap::Input));

		PLX_STATUS rc = PlxPci_PciBarSpaceRead((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_pDwCS[i32UserArea] + dwAddress, &m_dwInStatus, sizeof(m_dwInStatus), BitSize32, true);
		if(rc != ApiSuccess)
			break;

		wReturn = m_dwInStatus & WORD_MAX;
	}
	while(false);

	Unlock();

	return wReturn;
}

DWORD CDeviceMVTechMFB_I::ReadOutDword()
{
	DWORD dwReturn = 0;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		const int32_t i32UserArea = CTOINT32(MFB_I::BoardInfo::PCI_BASE_CS0);
		const DWORD dwAddress = DWORD(CTOINT32(MFB_I::AddressMap::OutputRead));

		PLX_STATUS rc = PlxPci_PciBarSpaceRead((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_pDwCS[i32UserArea] + dwAddress, &m_dwOutStatus, sizeof(m_dwOutStatus), BitSize32, true);
		if(rc != ApiSuccess)
			break;

		dwReturn = m_dwOutStatus & DWORD_MAX;
	}
	while(false);

	Unlock();

	return dwReturn;
}

DWORD CDeviceMVTechMFB_I::ReadInDword()
{
	DWORD dwReturn = 0;
	
	Lock();

	do
	{
		if(!IsInitialized())
			break;

		const int32_t i32UserArea = CTOINT32(MFB_I::BoardInfo::PCI_BASE_CS0);
		const DWORD dwAddress = DWORD(CTOINT32(MFB_I::AddressMap::Input));

		PLX_STATUS rc = PlxPci_PciBarSpaceRead((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_pDwCS[i32UserArea] + dwAddress, &m_dwInStatus, sizeof(m_dwInStatus), BitSize32, true);
		if(rc != ApiSuccess)
			break;

		dwReturn = m_dwInStatus;
	}
	while(false);

	Unlock();

	return dwReturn;
}

bool CDeviceMVTechMFB_I::WriteOutBit(int nBit, bool bOn)
{
	bool bReturn = false;
	
	do
	{
		if(!IsInitialized())
			break;

		if(nBit >= CTOINT32(MFB_I::BoardInfo::OutportCount) || nBit < 0)
			break;

		DWORD dwCommand = m_dwOutStatus;

		if(bOn)
			dwCommand |= 1 << nBit;
		else
			dwCommand &= ~(1 << nBit);

		const int32_t i32UserArea = CTOINT32(MFB_I::BoardInfo::PCI_BASE_CS0);
		const DWORD dwAddress = DWORD(CTOINT32(MFB_I::AddressMap::Output));

		PLX_STATUS rc = PlxPci_PciBarSpaceWrite((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_pDwCS[i32UserArea] + dwAddress, &dwCommand, sizeof(dwCommand), BitSize32, true);
		if(rc != ApiSuccess)
			break;

		m_dwOutStatus = dwCommand;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceMVTechMFB_I::WriteOutByte(BYTE cByte)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		DWORD dwCommand = m_dwOutStatus & 0xFFFFFF00;
		dwCommand += cByte;

		const int32_t i32UserArea = CTOINT32(MFB_I::BoardInfo::PCI_BASE_CS0);
		const DWORD dwAddress = DWORD(CTOINT32(MFB_I::AddressMap::Output));

		PLX_STATUS rc = PlxPci_PciBarSpaceWrite((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_pDwCS[i32UserArea] + dwAddress, &dwCommand, sizeof(dwCommand), BitSize32, true);
		if(rc != ApiSuccess)
			break;

		m_dwOutStatus = dwCommand;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceMVTechMFB_I::WriteOutWord(WORD wWord)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		DWORD dwCommand = m_dwOutStatus & 0xFFFF0000;
		dwCommand += wWord;

		const int32_t i32UserArea = CTOINT32(MFB_I::BoardInfo::PCI_BASE_CS0);
		const DWORD dwAddress = DWORD(CTOINT32(MFB_I::AddressMap::Output));

		PLX_STATUS rc = PlxPci_PciBarSpaceWrite((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_pDwCS[i32UserArea] + dwAddress, &dwCommand, sizeof(dwCommand), BitSize32, true);
		if(rc != ApiSuccess)
			break;

		m_dwOutStatus = dwCommand;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceMVTechMFB_I::WriteOutDword(DWORD dwDword)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		const int32_t i32UserArea = CTOINT32(MFB_I::BoardInfo::PCI_BASE_CS0);
		const DWORD dwAddress = DWORD(CTOINT32(MFB_I::AddressMap::Output));

		PLX_STATUS rc = PlxPci_PciBarSpaceWrite((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_pDwCS[i32UserArea] + dwAddress, &dwDword, sizeof(dwDword), BitSize32, true);
		if(rc != ApiSuccess)
			break;

		m_dwOutStatus = dwDword;
		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceMVTechMFB_I::ClearOutPort()
{
	return WriteOutDword(0);
}

bool CDeviceMVTechMFB_I::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = false;

	bool bFoundID = false;

	CString strMessage;

	do
	{
		switch(nParam)
		{
		case EDeviceParameterMVTechMFB_I_DeviceID:
			bReturn = !SetDeviceID(strValue);
			bFoundID = true;
			break;
		case EDeviceParameterMVTechMFB_I_UseClearInitialize:
			bReturn = !SetUseClearInitalize(_ttoi(strValue));
			bFoundID = true;
			break;
		case EDeviceParameterMVTechMFB_I_UseClearTerminate:
			bReturn = !SetUseClearTerminate(_ttoi(strValue));
			bFoundID = true;
			break;
		case EDeviceParameterMVTechMFB_I_Startposition:
			bReturn = !SetStartPosition(_ttoi(strValue));
			bFoundID = true;
			break;
		case EDeviceParameterMVTechMFB_I_Endposition:
			bReturn = !SetEndPosition(_ttoi(strValue));
			bFoundID = true;
			break;
		case EDeviceParameterMVTechMFB_I_TriggerCycle:
			bReturn = !SetTriggerCycle(_ttoi(strValue));
			bFoundID = true;
			break;
		case EDeviceParameterMVTechMFB_I_TriggerOntime:
			bReturn = !SetTriggerOnTime(_ttoi(strValue));
			bFoundID = true;
			break;
		case EDeviceParameterMVTechMFB_I_EncoderDirection:
			bReturn = !SetEncoderDirection((EDeviceSimplexDirection)_ttoi(strValue));
			bFoundID = true;
			break;
		case EDeviceParameterMVTechMFB_I_UseInputNo0:
		case EDeviceParameterMVTechMFB_I_UseInputNo1:
		case EDeviceParameterMVTechMFB_I_UseInputNo2:
		case EDeviceParameterMVTechMFB_I_UseInputNo3:
		case EDeviceParameterMVTechMFB_I_UseInputNo4:
		case EDeviceParameterMVTechMFB_I_UseInputNo5:
		case EDeviceParameterMVTechMFB_I_UseInputNo6:
		case EDeviceParameterMVTechMFB_I_UseInputNo7:
		case EDeviceParameterMVTechMFB_I_UseInputNo8:
		case EDeviceParameterMVTechMFB_I_UseInputNo9:
		case EDeviceParameterMVTechMFB_I_UseInputNo10:
		case EDeviceParameterMVTechMFB_I_UseInputNo11:
		case EDeviceParameterMVTechMFB_I_UseInputNo12:
		case EDeviceParameterMVTechMFB_I_UseInputNo13:
		case EDeviceParameterMVTechMFB_I_UseInputNo14:
		case EDeviceParameterMVTechMFB_I_UseInputNo15:
		case EDeviceParameterMVTechMFB_I_UseInputNo16:
		case EDeviceParameterMVTechMFB_I_UseInputNo17:
		case EDeviceParameterMVTechMFB_I_UseInputNo18:
		case EDeviceParameterMVTechMFB_I_UseInputNo19:
		case EDeviceParameterMVTechMFB_I_UseInputNo20:
		case EDeviceParameterMVTechMFB_I_UseInputNo21:
		case EDeviceParameterMVTechMFB_I_UseInputNo22:
		case EDeviceParameterMVTechMFB_I_UseInputNo23:
		case EDeviceParameterMVTechMFB_I_UseInputNo24:
		case EDeviceParameterMVTechMFB_I_UseInputNo25:
		case EDeviceParameterMVTechMFB_I_UseInputNo26:
		case EDeviceParameterMVTechMFB_I_UseInputNo27:
		case EDeviceParameterMVTechMFB_I_UseInputNo28:
		case EDeviceParameterMVTechMFB_I_UseInputNo29:
		case EDeviceParameterMVTechMFB_I_UseInputNo30:
		case EDeviceParameterMVTechMFB_I_UseInputNo31:
			{
				int nPreValue = _ttoi(GetParamValue(nParam));
				int nValue = _ttoi(strValue);

				if(SetParamValue(nParam, strValue))
				{
					m_pUsedInput[nParam - EDeviceParameterMVTechMFB_I_UseInputNo0] = _ttoi(strValue);
					bReturn = SaveSettings(nParam);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMFB_I[nParam], g_lpszSwitch[nPreValue], g_lpszSwitch[nValue]);
			}
			break;
		case EDeviceParameterMVTechMFB_I_UseOutputNo0:
		case EDeviceParameterMVTechMFB_I_UseOutputNo1:
		case EDeviceParameterMVTechMFB_I_UseOutputNo2:
		case EDeviceParameterMVTechMFB_I_UseOutputNo3:
		case EDeviceParameterMVTechMFB_I_UseOutputNo4:
		case EDeviceParameterMVTechMFB_I_UseOutputNo5:
		case EDeviceParameterMVTechMFB_I_UseOutputNo6:
		case EDeviceParameterMVTechMFB_I_UseOutputNo7:
		case EDeviceParameterMVTechMFB_I_UseOutputNo8:
		case EDeviceParameterMVTechMFB_I_UseOutputNo9:
		case EDeviceParameterMVTechMFB_I_UseOutputNo10:
		case EDeviceParameterMVTechMFB_I_UseOutputNo11:
		case EDeviceParameterMVTechMFB_I_UseOutputNo12:
		case EDeviceParameterMVTechMFB_I_UseOutputNo13:
		case EDeviceParameterMVTechMFB_I_UseOutputNo14:
		case EDeviceParameterMVTechMFB_I_UseOutputNo15:
		case EDeviceParameterMVTechMFB_I_UseOutputNo16:
		case EDeviceParameterMVTechMFB_I_UseOutputNo17:
		case EDeviceParameterMVTechMFB_I_UseOutputNo18:
		case EDeviceParameterMVTechMFB_I_UseOutputNo19:
		case EDeviceParameterMVTechMFB_I_UseOutputNo20:
		case EDeviceParameterMVTechMFB_I_UseOutputNo21:
		case EDeviceParameterMVTechMFB_I_UseOutputNo22:
		case EDeviceParameterMVTechMFB_I_UseOutputNo23:
		case EDeviceParameterMVTechMFB_I_UseOutputNo24:
		case EDeviceParameterMVTechMFB_I_UseOutputNo25:
		case EDeviceParameterMVTechMFB_I_UseOutputNo26:
		case EDeviceParameterMVTechMFB_I_UseOutputNo27:
		case EDeviceParameterMVTechMFB_I_UseOutputNo28:
		case EDeviceParameterMVTechMFB_I_UseOutputNo29:
		case EDeviceParameterMVTechMFB_I_UseOutputNo30:
		case EDeviceParameterMVTechMFB_I_UseOutputNo31:
			{
				int nPreValue = _ttoi(GetParamValue(nParam));
				int nValue = _ttoi(strValue);

				if(SetParamValue(nParam, strValue))
				{
					m_pUsedOutput[nParam - EDeviceParameterMVTechMFB_I_UseOutputNo0] = _ttoi(strValue);
					bReturn = SaveSettings(nParam);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMFB_I[nParam], g_lpszSwitch[nPreValue], g_lpszSwitch[nValue]);
			}
			break;
		case EDeviceParameterMVTechMFB_I_InputNo0Name:
		case EDeviceParameterMVTechMFB_I_InputNo1Name:
		case EDeviceParameterMVTechMFB_I_InputNo2Name:
		case EDeviceParameterMVTechMFB_I_InputNo3Name:
		case EDeviceParameterMVTechMFB_I_InputNo4Name:
		case EDeviceParameterMVTechMFB_I_InputNo5Name:
		case EDeviceParameterMVTechMFB_I_InputNo6Name:
		case EDeviceParameterMVTechMFB_I_InputNo7Name:
		case EDeviceParameterMVTechMFB_I_InputNo8Name:
		case EDeviceParameterMVTechMFB_I_InputNo9Name:
		case EDeviceParameterMVTechMFB_I_InputNo10Name:
		case EDeviceParameterMVTechMFB_I_InputNo11Name:
		case EDeviceParameterMVTechMFB_I_InputNo12Name:
		case EDeviceParameterMVTechMFB_I_InputNo13Name:
		case EDeviceParameterMVTechMFB_I_InputNo14Name:
		case EDeviceParameterMVTechMFB_I_InputNo15Name:
		case EDeviceParameterMVTechMFB_I_InputNo16Name:
		case EDeviceParameterMVTechMFB_I_InputNo17Name:
		case EDeviceParameterMVTechMFB_I_InputNo18Name:
		case EDeviceParameterMVTechMFB_I_InputNo19Name:
		case EDeviceParameterMVTechMFB_I_InputNo20Name:
		case EDeviceParameterMVTechMFB_I_InputNo21Name:
		case EDeviceParameterMVTechMFB_I_InputNo22Name:
		case EDeviceParameterMVTechMFB_I_InputNo23Name:
		case EDeviceParameterMVTechMFB_I_InputNo24Name:
		case EDeviceParameterMVTechMFB_I_InputNo25Name:
		case EDeviceParameterMVTechMFB_I_InputNo26Name:
		case EDeviceParameterMVTechMFB_I_InputNo27Name:
		case EDeviceParameterMVTechMFB_I_InputNo28Name:
		case EDeviceParameterMVTechMFB_I_InputNo29Name:
		case EDeviceParameterMVTechMFB_I_InputNo30Name:
		case EDeviceParameterMVTechMFB_I_InputNo31Name:
			{
				CString strPreValue = GetParamValue(nParam);

				if(SetParamValue(nParam, strValue))
				{
					m_pStrInputName[nParam - EDeviceParameterMVTechMFB_I_InputNo0Name] = strValue;
					bReturn = SaveSettings(nParam);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMFB_I[nParam], strPreValue, strValue);
			}
			break;
		case EDeviceParameterMVTechMFB_I_OutputNo0Name:
		case EDeviceParameterMVTechMFB_I_OutputNo1Name:
		case EDeviceParameterMVTechMFB_I_OutputNo2Name:
		case EDeviceParameterMVTechMFB_I_OutputNo3Name:
		case EDeviceParameterMVTechMFB_I_OutputNo4Name:
		case EDeviceParameterMVTechMFB_I_OutputNo5Name:
		case EDeviceParameterMVTechMFB_I_OutputNo6Name:
		case EDeviceParameterMVTechMFB_I_OutputNo7Name:
		case EDeviceParameterMVTechMFB_I_OutputNo8Name:
		case EDeviceParameterMVTechMFB_I_OutputNo9Name:
		case EDeviceParameterMVTechMFB_I_OutputNo10Name:
		case EDeviceParameterMVTechMFB_I_OutputNo11Name:
		case EDeviceParameterMVTechMFB_I_OutputNo12Name:
		case EDeviceParameterMVTechMFB_I_OutputNo13Name:
		case EDeviceParameterMVTechMFB_I_OutputNo14Name:
		case EDeviceParameterMVTechMFB_I_OutputNo15Name:
		case EDeviceParameterMVTechMFB_I_OutputNo16Name:
		case EDeviceParameterMVTechMFB_I_OutputNo17Name:
		case EDeviceParameterMVTechMFB_I_OutputNo18Name:
		case EDeviceParameterMVTechMFB_I_OutputNo19Name:
		case EDeviceParameterMVTechMFB_I_OutputNo20Name:
		case EDeviceParameterMVTechMFB_I_OutputNo21Name:
		case EDeviceParameterMVTechMFB_I_OutputNo22Name:
		case EDeviceParameterMVTechMFB_I_OutputNo23Name:
		case EDeviceParameterMVTechMFB_I_OutputNo24Name:
		case EDeviceParameterMVTechMFB_I_OutputNo25Name:
		case EDeviceParameterMVTechMFB_I_OutputNo26Name:
		case EDeviceParameterMVTechMFB_I_OutputNo27Name:
		case EDeviceParameterMVTechMFB_I_OutputNo28Name:
		case EDeviceParameterMVTechMFB_I_OutputNo29Name:
		case EDeviceParameterMVTechMFB_I_OutputNo30Name:
		case EDeviceParameterMVTechMFB_I_OutputNo31Name:
			{
				CString strPreValue = GetParamValue(nParam);

				if(SetParamValue(nParam, strValue))
				{
					m_pStrOutputName[nParam - EDeviceParameterMVTechMFB_I_OutputNo0Name] = strValue;
					bReturn = SaveSettings(nParam);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMFB_I[nParam], strPreValue, strValue);
			}
			break;
		default:
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtofindtheparameter);
			break;
		}

		if(EDeviceParameterMVTechMFB_I_InputStatus < nParam && EDeviceParameterMVTechMFB_I_OutputNo31Name >= nParam)
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

ptrdiff_t CDeviceMVTechMFB_I::OnMessage(CMessageBase * pMessage)
{
	__super::OnMessage(pMessage);

	do
	{
		if(!pMessage)
			break;

		if(pMessage->GetMessage() != EMessage_LanguageChange)
			break;

		if(!IsWindow(GetSafeHwnd()))
			break;

		CWnd* pCtrlUI = nullptr;
		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 2);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_Input));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 3);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_Output));
	}
	while(false);

	return 0;
}

EGetFunction CDeviceMVTechMFB_I::GetUseClearInitalize(bool * pParam)
{
	EGetFunction eReturn = EGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMVTechMFB_I_UseClearInitialize));

		eReturn = EGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESetFunction CDeviceMVTechMFB_I::SetUseClearInitalize(bool bParam)
{
	ESetFunction eReturn = ESetFunction_UnknownError;

	EDeviceParameterMVTechMFB_I eSaveNum = EDeviceParameterMVTechMFB_I_UseClearInitialize;

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMFB_I[eSaveNum], g_lpszSwitch[bPreValue], g_lpszSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGetFunction CDeviceMVTechMFB_I::GetUseClearTerminate(bool * pParam)
{
	EGetFunction eReturn = EGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMVTechMFB_I_UseClearTerminate));

		eReturn = EGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESetFunction CDeviceMVTechMFB_I::SetUseClearTerminate(bool bParam)
{
	ESetFunction eReturn = ESetFunction_UnknownError;

	EDeviceParameterMVTechMFB_I eSaveNum = EDeviceParameterMVTechMFB_I_UseClearTerminate;

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMFB_I[eSaveNum], g_lpszSwitch[bPreValue], g_lpszSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGetFunction CDeviceMVTechMFB_I::GetStartPosition(int * pParam)
{
	EGetFunction eReturn = EGetFunction_UnknownError;

	do
	{
		if (!pParam)
		{
			eReturn = EGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMVTechMFB_I_Startposition));

		eReturn = EGetFunction_OK;
	} while (false);

	return eReturn;
}

ESetFunction CDeviceMVTechMFB_I::SetStartPosition(int nParam)
{
	ESetFunction eReturn = ESetFunction_UnknownError;

	EDeviceParameterMVTechMFB_I eSaveID = EDeviceParameterMVTechMFB_I_Startposition;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if (IsInitialized())
		{
			const int32_t i32UserArea = CTOINT32(MFB_I::BoardInfo::PCI_BASE_CS0);
			const DWORD dwAddress = DWORD(CTOINT32(MFB_I::AddressMap::EncoderBeginPosition));
			DWORD dwCommand = nParam;

			if(!PCIWrite(i32UserArea, dwAddress, dwCommand))
			{
				eReturn = ESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if (!SetParamValue(eSaveID, strSave))
		{
			eReturn = ESetFunction_WriteToDatabaseError;
			break;
		}

		if (!SaveSettings(eSaveID))
		{
			eReturn = ESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ESetFunction_OK;
	} while (false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMFB_I[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGetFunction CDeviceMVTechMFB_I::GetEndPosition(int * pParam)
{
	EGetFunction eReturn = EGetFunction_UnknownError;

	do
	{
		if (!pParam)
		{
			eReturn = EGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMVTechMFB_I_Endposition));

		eReturn = EGetFunction_OK;
	} while (false);

	return eReturn;
}

ESetFunction CDeviceMVTechMFB_I::SetEndPosition(int nParam)
{
	ESetFunction eReturn = ESetFunction_UnknownError;

	EDeviceParameterMVTechMFB_I eSaveID = EDeviceParameterMVTechMFB_I_Endposition;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if (IsInitialized())
		{
			const int32_t i32UserArea = CTOINT32(MFB_I::BoardInfo::PCI_BASE_CS0);
			const DWORD dwAddress = DWORD(CTOINT32(MFB_I::AddressMap::EncoderEndPosition));
			DWORD dwCommand = nParam;

			if(!PCIWrite(i32UserArea, dwAddress, dwCommand))
			{
				eReturn = ESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if (!SetParamValue(eSaveID, strSave))
		{
			eReturn = ESetFunction_WriteToDatabaseError;
			break;
		}

		if (!SaveSettings(eSaveID))
		{
			eReturn = ESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ESetFunction_OK;
	} while (false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMFB_I[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGetFunction CDeviceMVTechMFB_I::GetTriggerCycle(int * pParam)
{
	EGetFunction eReturn = EGetFunction_UnknownError;

	do
	{
		if (!pParam)
		{
			eReturn = EGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMVTechMFB_I_TriggerCycle));

		eReturn = EGetFunction_OK;
	} while (false);

	return eReturn;
}

ESetFunction CDeviceMVTechMFB_I::SetTriggerCycle(int nParam)
{
	ESetFunction eReturn = ESetFunction_UnknownError;

	EDeviceParameterMVTechMFB_I eSaveID = EDeviceParameterMVTechMFB_I_TriggerCycle;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if (IsInitialized())
		{
			int nDuration = 0;
			if (GetTriggerOnTime(&nDuration))
			{
				eReturn = ESetFunction_ReadOnDatabaseError;
				break;
			}

			int nValue = nDuration << 8 | nParam;

			const int32_t i32UserArea = CTOINT32(MFB_I::BoardInfo::PCI_BASE_CS0);
			const DWORD dwAddress = DWORD(CTOINT32(MFB_I::AddressMap::SettingTriggerEvent));
			DWORD dwCommand = nValue;

			if(!PCIWrite(i32UserArea, dwAddress, dwCommand))
			{
				eReturn = ESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if (!SetParamValue(eSaveID, strSave))
		{
			eReturn = ESetFunction_WriteToDatabaseError;
			break;
		}

		if (!SaveSettings(eSaveID))
		{
			eReturn = ESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ESetFunction_OK;
	} while (false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMFB_I[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGetFunction CDeviceMVTechMFB_I::GetTriggerOnTime(int * pParam)
{
	EGetFunction eReturn = EGetFunction_UnknownError;

	do
	{
		if (!pParam)
		{
			eReturn = EGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMVTechMFB_I_TriggerOntime));

		eReturn = EGetFunction_OK;
	} while (false);

	return eReturn;
}

ESetFunction CDeviceMVTechMFB_I::SetTriggerOnTime(int nParam)
{
	ESetFunction eReturn = ESetFunction_UnknownError;

	EDeviceParameterMVTechMFB_I eSaveID = EDeviceParameterMVTechMFB_I_TriggerOntime;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if (IsInitialized())
		{
			int nCycle = 0;

			if (GetTriggerCycle(&nCycle))
			{
				eReturn = ESetFunction_WriteToDatabaseError;
				break;
			}

			int nValue = nParam << 8 | nCycle;

			const int32_t i32UserArea = CTOINT32(MFB_I::BoardInfo::PCI_BASE_CS0);
			const DWORD dwAddress = DWORD(CTOINT32(MFB_I::AddressMap::SettingTriggerEvent));
			DWORD dwCommand = nValue;

			if(!PCIWrite(i32UserArea, dwAddress, dwCommand))
			{
				eReturn = ESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if (!SetParamValue(eSaveID, strSave))
		{
			eReturn = ESetFunction_WriteToDatabaseError;
			break;
		}

		if (!SaveSettings(eSaveID))
		{
			eReturn = ESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ESetFunction_OK;
	} while (false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMFB_I[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGetFunction CDeviceMVTechMFB_I::GetEncoderDirection(EDeviceSimplexDirection* pParam)
{
	EGetFunction eReturn = EGetFunction_UnknownError;

	do
	{
		if (!pParam)
		{
			eReturn = EGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceSimplexDirection)_ttoi(GetParamValue(EDeviceParameterMVTechMFB_I_EncoderDirection));

		eReturn = EGetFunction_OK;
	} while (false);

	return eReturn;
}

ESetFunction CDeviceMVTechMFB_I::SetEncoderDirection(EDeviceSimplexDirection eParam)
{
	ESetFunction eReturn = ESetFunction_UnknownError;

	EDeviceParameterMVTechMFB_I eSaveID = EDeviceParameterMVTechMFB_I_EncoderDirection;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if (IsInitialized())
		{
			const int32_t i32UserArea = CTOINT32(MFB_I::BoardInfo::PCI_BASE_CS0);
			const DWORD dwAddress = DWORD(CTOINT32(MFB_I::AddressMap::EncoderDirection));
			DWORD dwCommand = 0x00;
			if(eParam == EDeviceSimplexDirection_CCW)
				dwCommand = 0x01;

			if(!PCIWrite(i32UserArea, dwAddress, dwCommand))
			{
				eReturn = ESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if (!SetParamValue(eSaveID, strValue))
		{
			eReturn = ESetFunction_WriteToDatabaseError;
			break;
		}

		if (!SaveSettings(eSaveID))
		{
			eReturn = ESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ESetFunction_OK;
	} while (false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMFB_I[eSaveID], g_lpszSimplexDirection[nPreValue], g_lpszSimplexDirection[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceMVTechMFB_I::EnableTrigger()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(IsTriggerEnabled())
			break;

		const int32_t i32UserArea = CTOINT32(MFB_I::BoardInfo::PCI_BASE_CS0);
		const DWORD dwAddress = DWORD(CTOINT32(MFB_I::AddressMap::TriggerEnable));

		const DWORD dwCommand = 0b00000001;
		bReturn = PCIWrite(i32UserArea, dwAddress, dwCommand);

		if(bReturn)
			m_bTriggerEnabled = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceMVTechMFB_I::DisableTrigger()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		const int32_t i32UserArea = CTOINT32(MFB_I::BoardInfo::PCI_BASE_CS0);
		const DWORD dwAddress = DWORD(CTOINT32(MFB_I::AddressMap::TriggerEnable));

		const DWORD dwCommandOff = 0b00000000;
		bReturn = PCIWrite(i32UserArea, dwAddress, dwCommandOff);

		m_bTriggerEnabled = false;
	}
	while(false);

	return bReturn;
}

bool CDeviceMVTechMFB_I::IsTriggerEnabled()
{
	return m_bTriggerEnabled;
}

long CDeviceMVTechMFB_I::GetEncoderCount()
{
	long lReturn = 0;

	do
	{
		if(!IsInitialized())
			break;

		const int32_t i32UserArea = CTOINT32(MFB_I::BoardInfo::PCI_BASE_CS0);
		const DWORD dwAddress = DWORD(CTOINT32(MFB_I::AddressMap::EncoderRead));

		lReturn = PCIRead(i32UserArea, dwAddress);
	}
	while(false);

	return lReturn;
}

long CDeviceMVTechMFB_I::GetTriggerCount()
{
	long lReturn = 0;

	do
	{
		if(!IsInitialized())
			break;

		const int32_t i32UserArea = CTOINT32(MFB_I::BoardInfo::PCI_BASE_CS0);
		const DWORD dwAddress = DWORD(CTOINT32(MFB_I::AddressMap::TriggerRead));

		lReturn = PCIRead(i32UserArea, dwAddress);
	}
	while(false);

	return lReturn;
}

bool CDeviceMVTechMFB_I::ResetCounter()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		const int32_t i32UserArea = CTOINT32(MFB_I::BoardInfo::PCI_BASE_CS0);
		const DWORD dwAddress = DWORD(CTOINT32(MFB_I::AddressMap::ResetCounter));

		const DWORD dwCommandOff = 0b00000000;
		const DWORD dwCommandOn = 0b00000001;

		bReturn = true;

		bReturn &= PCIWrite(i32UserArea, dwAddress, dwCommandOff);
		Sleep(1);
		bReturn &= PCIWrite(i32UserArea, dwAddress, dwCommandOn);
		Sleep(1);
		bReturn &= PCIWrite(i32UserArea, dwAddress, dwCommandOff);
	}
	while(false);

	return bReturn;
}

void CDeviceMVTechMFB_I::SetPCIVendorID(unsigned short vendorId)
{
	m_vendorId = vendorId;
}

void CDeviceMVTechMFB_I::SetPCIDeviceID(unsigned short deviceId)
{
	m_deviceId = deviceId;
}

void CDeviceMVTechMFB_I::OnTimer(UINT_PTR nIDEvent)
{
	if(IsWindowVisible())
	{
		if(nIDEvent == RAVID_TIMER_DIO)
		{
			this->ReadInDword();
			this->ReadOutDword();

			const int32_t i32InputMaxLed = CTOINT32(MFB_I::BoardInfo::InportCount);
			const int32_t i32OutputMaxLed = CTOINT32(MFB_I::BoardInfo::OutportCount);

			for(int32_t i = 0; i < i32InputMaxLed; ++i)
			{
				CRavidLedCtrl* pLed = (CRavidLedCtrl*)GetDlgItem(EDeviceAddEnumeratedControlID_InputLedStart + i);
				if(pLed)
				{
					if(pLed->GetActivate() != this->IsInitialized())
						pLed->SetActivate(this->IsInitialized());

					if((m_dwInStatus >> i) & 0x01)
						pLed->On();
					else
						pLed->Off();
				}
			}

			for(int32_t i = 0; i < i32OutputMaxLed; ++i)
			{
				CRavidLedCtrl* pLed = (CRavidLedCtrl*)GetDlgItem(EDeviceAddEnumeratedControlID_OutputLedStart + i);
				if(pLed)
				{
					if(pLed->GetActivate() != this->IsInitialized())
						pLed->SetActivate(this->IsInitialized());

					if((m_dwOutStatus >> i) & 0x01)
						pLed->On();
					else
						pLed->Off();
				}
			}
		}
	}

	CDeviceTrigger::OnTimer(nIDEvent);
}

bool CDeviceMVTechMFB_I::DoesModuleExist()
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

bool CDeviceMVTechMFB_I::AddControls()
{
	__super::AddControls();

	bool bReturn = false;
	CDC* pDC = nullptr;
	CFont* pOldFont = nullptr;

	do
	{
		pDC = GetDC();

		if(!pDC)
			break;
		
		CWnd* pWndParamTree = GetDlgItem(int32_t(RavidInterfaceControlID::DeviceSheetParameter));
		if(!pWndParamTree)
			break;

		CWnd* pWndInitBtn = GetDlgItem(int32_t(RavidInterfaceControlID::DeviceButtonInitialize));
		if(!pWndInitBtn)
			break;

		CFont* pBaseFont = pWndInitBtn->GetFont();
		if(!pBaseFont)
			break;

		double dblDPIScale = DPISCALE;

		pOldFont = pDC->SelectObject(pBaseFont);

		CRavidRect<int> rrDlg, rrSheet, rrInitBtn, rrTerBtn;
		GetClientRect(rrDlg);

		pWndParamTree->GetWindowRect(rrSheet);
		ScreenToClient(rrSheet);

		pWndInitBtn->GetWindowRect(rrInitBtn);
		ScreenToClient(rrInitBtn);

		int nBtnOffset = 5 * dblDPIScale;
		int nBtnWidth = rrInitBtn.GetWidth();
		int nBtnHeight = rrInitBtn.GetHeight();

		int nDialogEdgeMargin = 10 * dblDPIScale;

		AllocateIO(CTOINT32(MFB_I::BoardInfo::InportCount), CTOINT32(MFB_I::BoardInfo::OutportCount));

		CRect rrResultGroupBox;

		CPoint rpSheetLeftTop(rrSheet.right + nDialogEdgeMargin, rrInitBtn.bottom);
		CPoint rpSheetRightBottom(rpSheetLeftTop.x + rrSheet.right - rrSheet.left, rrSheet.bottom + nDialogEdgeMargin);

		CreateLedArray(dblDPIScale, pBaseFont, rpSheetLeftTop, rpSheetRightBottom, rrResultGroupBox);

		rrDlg.right = __max(rrSheet.right, rrResultGroupBox.right) + nDialogEdgeMargin;
		rrDlg.bottom = __max(rrDlg.bottom, rrResultGroupBox.bottom + nDialogEdgeMargin);

		SetDefaultDialogRect(rrDlg);

		RecalcSheet();

		bReturn = true;
	}
	while(false);

	if(pDC)
	{
		pDC->SelectObject(pOldFont);
		ReleaseDC(pDC);
	}

	return bReturn;
}

bool CDeviceMVTechMFB_I::CreateLedArray(double dblDPIScale, CFont * pBaseFont, CPoint rpLeftTopPt, CPoint rpRightTopPt, CRect& rrResultGroupBox)
{
	bool bReturn = false;

	LockWindowUpdate();

	do
	{
		CButton* pGroupBox[2];

		for(int i = 0; i < 2; ++i)
			pGroupBox[i] = new CButton;

		CRavidRect<int> rrGroupBox[2];

		rrGroupBox[0].SetRect(rpLeftTopPt.x, rpLeftTopPt.y, (rpLeftTopPt.x + rpRightTopPt.x) / 2., rpRightTopPt.y + 100 * dblDPIScale);
		rrGroupBox[1].SetRect((rpLeftTopPt.x + rpRightTopPt.x) / 2., rpLeftTopPt.y, rpRightTopPt.x, rpRightTopPt.y + 100 * dblDPIScale);

		CRavidPoint<int> rpLedBasePos(rrGroupBox[0].left + 10 * dblDPIScale, rrGroupBox[0].top + 20 * dblDPIScale);
		CRavidRect<int> rrLedRect(rpLedBasePos.x, rpLedBasePos.y, rpLedBasePos.x + 15 * dblDPIScale, rpLedBasePos.y + 15 * dblDPIScale);
		CRavidPoint<int> rpLedYGap(0, 20 * dblDPIScale);
		CRavidPoint<int> rpLedXGap(20 * dblDPIScale, 0);

		const int nInputMaxLed = CTOINT32(MFB_I::BoardInfo::InportCount);
		const int nOutputMaxLed = CTOINT32(MFB_I::BoardInfo::OutportCount);

		int nInputLedCreatedCount = 0;
		int nOutputLedCreatedCount = 0;

		int nRealPos = 0;

		for(int i = 0; i < nInputMaxLed; ++i)
		{
			if(!m_pUsedInput[i])
				continue;

			CRavidLedCtrl* pLed = new CRavidLedCtrl;
			pLed->Create(nullptr, WS_VISIBLE | WS_CHILD | SS_NOTIFY, rrLedRect + rpLedYGap * nRealPos, this, EDeviceAddEnumeratedControlID_InputLedStart + i);
			pLed->SetFont(pBaseFont);
			m_vctCtrl.push_back(pLed);

			CStatic* pLabel = new CStatic;
			CRavidRect<int> rrLabelPosition(rrLedRect + rpLedYGap * nRealPos);
			rrLabelPosition += rpLedXGap;
			rrLabelPosition.right = rrGroupBox[0].right - 10 * dblDPIScale;

			CString strName;
			strName.Format(_T("%d : %s"), i, m_pStrInputName[i]);
			pLabel->Create(strName, WS_VISIBLE | WS_CHILD | SS_NOTIFY, rrLabelPosition, this, EDeviceAddEnumeratedControlID_InputLedLabelStart + i);
			pLabel->SetFont(pBaseFont);
			m_vctCtrl.push_back(pLabel);

			++nRealPos;
		}

		nInputLedCreatedCount = nRealPos;

		rpLedBasePos.SetPoint(rrGroupBox[1].left + 10 * dblDPIScale, rrGroupBox[1].top + 20 * dblDPIScale);
		rrLedRect.SetRect(rpLedBasePos.x, rpLedBasePos.y, rpLedBasePos.x + 15 * dblDPIScale, rpLedBasePos.y + 15 * dblDPIScale);

		nRealPos = 0;

		for(int i = 0; i < nOutputMaxLed; ++i)
		{
			if(!m_pUsedOutput[i])
				continue;

			CRavidLedCtrl* pLed = new CRavidLedCtrl;
			pLed->Create(nullptr, WS_VISIBLE | WS_CHILD | SS_NOTIFY, rrLedRect + rpLedYGap * nRealPos, this, EDeviceAddEnumeratedControlID_OutputLedStart + i);
			pLed->SetFont(pBaseFont);
			m_vctCtrl.push_back(pLed);

			CStatic* pLabel = new CStatic;
			CRavidRect<int> rrLabelPosition(rrLedRect + rpLedYGap * nRealPos);
			rrLabelPosition += rpLedXGap;
			rrLabelPosition.right = rrGroupBox[1].right - 10 * dblDPIScale;
			CString strName;
			strName.Format(_T("%d : %s"), i, m_pStrOutputName[i]);
			pLabel->Create(strName, WS_VISIBLE | WS_CHILD | SS_NOTIFY, rrLabelPosition, this, EDeviceAddEnumeratedControlID_OutputLedLabelStart + i);
			pLabel->SetFont(pBaseFont);
			m_vctCtrl.push_back(pLabel);

			++nRealPos;
		}

		if(nInputLedCreatedCount || nRealPos)
		{
			nOutputLedCreatedCount = nRealPos;

			rrGroupBox[0].bottom = (rrLedRect + rpLedYGap * (__max(nInputLedCreatedCount, nOutputLedCreatedCount) - 1)).bottom + 10 * dblDPIScale;
			rrGroupBox[1].bottom = (rrLedRect + rpLedYGap * (__max(nInputLedCreatedCount, nOutputLedCreatedCount) - 1)).bottom + 10 * dblDPIScale;

			for(int i = 0; i < 2; ++i)
			{
				pGroupBox[i]->Create(i ? CMultiLanguageManager::GetString(ELanguageParameter_Output) : CMultiLanguageManager::GetString(ELanguageParameter_Input), BS_GROUPBOX | WS_CHILD | WS_VISIBLE, rrGroupBox[i], this, EDeviceAddEnumeratedControlID_StaticStart + i + 2);
				pGroupBox[i]->SetFont(pBaseFont);
				m_vctCtrl.push_back(pGroupBox[i]);
			}

			rrResultGroupBox.right = __max(rrGroupBox[0].right, rrGroupBox[1].right);
			rrResultGroupBox.bottom = __max(rrGroupBox[0].bottom, rrGroupBox[1].bottom);
		}
		else
		{
			for(int i = 0; i < 2; ++i)
				delete pGroupBox[i];
		}

		bReturn = true;
	}
	while(false);

	UnlockWindowUpdate();

	return bReturn;
}

void CDeviceMVTechMFB_I::OnBnClickedOutLed(UINT nID)
{
	do
	{
		int nNumber = nID - EDeviceAddEnumeratedControlID_OutputLedStart;

		if((CAuthorityManager::GetDeviceControlPermission() <= CAuthorityManager::GetCurrentUserType()) && !CSequenceManager::IsRunning())
		{
			WriteOutBit(nNumber, !ReadOutBit(nNumber));
		}
	}
	while(false);
}

BOOL CDeviceMVTechMFB_I::OnInitDialog()
{
	CDeviceTrigger::OnInitDialog();

	SetTimer(RAVID_TIMER_DIO, 10, nullptr);

	return TRUE;
}

void CDeviceMVTechMFB_I::OnDestroy()
{
	CDeviceTrigger::OnDestroy();

	KillTimer(RAVID_TIMER_DIO);
}

void CDeviceMVTechMFB_I::AllocateIO(long nInputSize, long nOutputSize)
{
	if(!m_pUsedInput)
	{
		m_pUsedInput = new bool[nInputSize];
		memset(m_pUsedInput, false, nInputSize);
	}

	if(!m_pUsedOutput)
	{
		m_pUsedOutput = new bool[nOutputSize];
		memset(m_pUsedOutput, false, nOutputSize);
	}

	if(!m_pStrInputName)
		m_pStrInputName = new CString[nInputSize];

	if(!m_pStrOutputName)
		m_pStrOutputName = new CString[nOutputSize];
}

void CDeviceMVTechMFB_I::FreeIO()
{
	if(m_pUsedInput)
	{
		delete[] m_pUsedInput;
		m_pUsedInput = nullptr;
	}

	if(m_pUsedOutput)
	{
		delete[] m_pUsedOutput;
		m_pUsedOutput = nullptr;
	}

	if(m_pStrInputName)
	{
		delete[] m_pStrInputName;
		m_pStrInputName = nullptr;
	}

	if(m_pStrOutputName)
	{
		delete[] m_pStrOutputName;
		m_pStrOutputName = nullptr;
	}
}

#endif