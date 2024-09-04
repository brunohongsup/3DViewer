#include "stdafx.h"

#include "DeviceMVTechMFB_T.h"

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

IMPLEMENT_DYNAMIC(CDeviceMVTechMFB_T, CDeviceTrigger)

BEGIN_MESSAGE_MAP(CDeviceMVTechMFB_T, CDeviceTrigger)
	ON_COMMAND_RANGE(EDeviceAddEnumeratedControlID_OutputLedStart, EDeviceAddEnumeratedControlID_OutputLedEnd, OnBnClickedOutLed)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static LPCTSTR g_lpszParamMFB_T[EDeviceParameterMVTechMFB_T_Count] =
{
	_T("DeviceID"),
	_T("Use Clear Initalize"),
	_T("Use Clear Terminate"),
	_T("Trigger infomation"),
	_T("Trigger Direction"),
	_T("Encoder Direction"),
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

static LPCTSTR g_lpszSimplexDirection[EDeviceSimplexDirection_Count] =
{
	_T("CW"),
	_T("CCW"),
};

static LPCTSTR g_lpszDuplexDirection[EDeviceDuplexDirection_Count] =
{
	_T("CW"),
	_T("CCW"),
	_T("BOTH"),
};

static LPCTSTR g_lpszSwitch[EDeviceSwitch_Count] =
{
	_T("OFF"),
	_T("ON"),
};

CDeviceMVTechMFB_T::CDeviceMVTechMFB_T() :
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
	m_vendorId(0x10B9),
	m_deviceId(0x9030)
{
	const int32_t i32UserCount = CTOINT32(MFB_T::BoardInfo::MAX_BASE_ADDR_NUMS);

	m_pDwCS = new DWORD[i32UserCount];
	memset(m_pDwCS, 0, sizeof(DWORD) * i32UserCount);

	m_pDwCSLen = new DWORD[i32UserCount];
	memset(m_pDwCSLen, 0, sizeof(DWORD) * i32UserCount);
}

CDeviceMVTechMFB_T::~CDeviceMVTechMFB_T()
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

ptrdiff_t CDeviceMVTechMFB_T::OnMessage(CMessageBase * pMessage)
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

EDeviceInitializeResult CDeviceMVTechMFB_T::Initialize()
{
	EDeviceInitializeResult eReturn = EDeviceInitializeResult_UnknownError;

	CString strMessage, strStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("MVTechTrigger"));

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

		const int32_t nRegCount = CTOINT32(MFB_T::BoardInfo::RegisterCount);

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

		rc = PlxPci_DeviceFind(&plxDeviceKey, _ttoi(GetDeviceID()));
		if(rc != ApiSuccess)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtofindthedevice);
			eReturn = EDeviceInitializeResult_NotFoundDeviceInfo;
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
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("RegisterAccess"));
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
			EDeviceSimplexDirection eType = EDeviceSimplexDirection_CW;
			GetEncoderDirection(&eType);

			const int32_t i32UserArea = CTOINT32(MFB_T::BoardInfo::PCI_BASE_CS0);
			DWORD dwAddress = DWORD(CTOINT32(MFB_T::AddressMap::EncoderDirection));

			DWORD dwCommand = eType == EDeviceSimplexDirection_CW ? 0x00 : 0x01;
			PCIWrite(i32UserArea, dwAddress, dwCommand);
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

EDeviceTerminateResult CDeviceMVTechMFB_T::Terminate()
{
	EDeviceTerminateResult eReturn = EDeviceTerminateResult_UnknownError;

	CString strMessage;

	SetStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("MVTechTrigger"));
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

bool CDeviceMVTechMFB_T::LoadSettings()
{
	bool bReturn = false;

	m_vctParameterFieldConfigurations.clear();

	AddParameterFieldConfigurations(EDeviceParameterMVTechMFB_T_DeviceID, g_lpszParamMFB_T[EDeviceParameterMVTechMFB_T_DeviceID], _T("0"), EParameterFieldType_Edit);

	AddParameterFieldConfigurations(EDeviceParameterMVTechMFB_T_UseClearInitialize, g_lpszParamMFB_T[EDeviceParameterMVTechMFB_T_UseClearInitialize], _T("1"), EParameterFieldType_Check, nullptr, nullptr, 0);
	AddParameterFieldConfigurations(EDeviceParameterMVTechMFB_T_UseClearTerminate, g_lpszParamMFB_T[EDeviceParameterMVTechMFB_T_UseClearTerminate], _T("1"), EParameterFieldType_Check, nullptr, nullptr, 0);

	AddParameterFieldConfigurations(EDeviceParameterMVTechMFB_T_TriggerInfo, g_lpszParamMFB_T[EDeviceParameterMVTechMFB_T_TriggerInfo], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);

	AddParameterFieldConfigurations(EDeviceParameterMVTechMFB_T_TriggerDirection, g_lpszParamMFB_T[EDeviceParameterMVTechMFB_T_TriggerDirection], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDuplexDirection, EDeviceDuplexDirection_Count), nullptr, 1);
	AddParameterFieldConfigurations(EDeviceParameterMVTechMFB_T_EncoderDirection, g_lpszParamMFB_T[EDeviceParameterMVTechMFB_T_EncoderDirection], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszSimplexDirection, EDeviceSimplexDirection_Count), nullptr, 1);

	AddParameterFieldConfigurations(EDeviceParameterMVTechMFB_T_UseInput, g_lpszParamMFB_T[EDeviceParameterMVTechMFB_T_UseInput], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);

	for(int i = 0; i < CTOINT32(MFB_T::BoardInfo::InportCount); ++i)
		AddParameterFieldConfigurations(EDeviceParameterMVTechMFB_T_UseInputNo0 + i, g_lpszParamMFB_T[EDeviceParameterMVTechMFB_T_UseInputNo0 + i], _T("1"), EParameterFieldType_Check, nullptr, nullptr, 1);

	AddParameterFieldConfigurations(EDeviceParameterMVTechMFB_T_UseOutput, g_lpszParamMFB_T[EDeviceParameterMVTechMFB_T_UseOutput], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);

	for(int i = 0; i < CTOINT32(MFB_T::BoardInfo::OutportCount); ++i)
		AddParameterFieldConfigurations(EDeviceParameterMVTechMFB_T_UseOutputNo0 + i, g_lpszParamMFB_T[EDeviceParameterMVTechMFB_T_UseOutputNo0 + i], _T("1"), EParameterFieldType_Check, nullptr, nullptr, 1);

	AddParameterFieldConfigurations(EDeviceParameterMVTechMFB_T_InputName, g_lpszParamMFB_T[EDeviceParameterMVTechMFB_T_InputName], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);

	for(int i = 0; i < CTOINT32(MFB_T::BoardInfo::InportCount); ++i)
		AddParameterFieldConfigurations(EDeviceParameterMVTechMFB_T_InputNo0Name + i, g_lpszParamMFB_T[EDeviceParameterMVTechMFB_T_InputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);

	AddParameterFieldConfigurations(EDeviceParameterMVTechMFB_T_OutputName, g_lpszParamMFB_T[EDeviceParameterMVTechMFB_T_OutputName], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);

	for(int i = 0; i < CTOINT32(MFB_T::BoardInfo::OutportCount); ++i)
		AddParameterFieldConfigurations(EDeviceParameterMVTechMFB_T_OutputNo0Name + i, g_lpszParamMFB_T[EDeviceParameterMVTechMFB_T_OutputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);

	bReturn = __super::LoadSettings();

	AllocateIO(CTOINT32(MFB_T::BoardInfo::InportCount), CTOINT32(MFB_T::BoardInfo::OutportCount));
	
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

	return bReturn;
}

bool CDeviceMVTechMFB_T::PCIWrite(DWORD dwAddr, DWORD dwData)
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

DWORD CDeviceMVTechMFB_T::PCIRead(DWORD dwAddr)
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

DWORD CDeviceMVTechMFB_T::PciReadReg(DWORD dwOffset)
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

bool CDeviceMVTechMFB_T::PciWriteReg(DWORD dwOffset, DWORD dwData)
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

bool CDeviceMVTechMFB_T::PCIWrite(DWORD dwCS, DWORD dwAddr, DWORD dwData)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		const int32_t i32UserCount = CTOINT32(MFB_T::BoardInfo::MAX_BASE_ADDR_NUMS);
		if(dwCS >= i32UserCount)
			break;

		bReturn = PCIWrite(m_pDwCS[dwCS] + dwAddr, dwData);
	}
	while(false);

	return bReturn;
}

DWORD CDeviceMVTechMFB_T::PCIRead(DWORD dwCS, DWORD dwAddr)
{
	DWORD dwReturn = 0;

	do
	{
		if(!IsInitialized())
			break;

		const int32_t i32UserCount = CTOINT32(MFB_T::BoardInfo::MAX_BASE_ADDR_NUMS);
		if(dwCS >= i32UserCount)
			break;

		dwReturn = PCIRead(m_pDwCS[dwCS] + dwAddr);
	}
	while(false);

	return dwReturn;
}

bool CDeviceMVTechMFB_T::EnableTrigger()
{
	bool bReturn = false;

	do 
	{
		if(!IsInitialized())
			break;

		if(IsTriggerEnabled())
			break;

		EDeviceDuplexDirection eType = EDeviceDuplexDirection_CW;
		GetTriggerDirection(&eType);

		const int32_t i32UserArea = CTOINT32(MFB_T::BoardInfo::PCI_BASE_CS0);
		const DWORD dwAddress = DWORD(CTOINT32(MFB_T::AddressMap::TriggerEnable));

		const DWORD dwCommandCW = 0b00000100;
		const DWORD dwCommandCCW = 0b00001000;
		const DWORD dwCommandBOTH = 0b00001100;

		switch(eType)
		{
		case EDeviceDuplexDirection_CW:
			bReturn = PCIWrite(i32UserArea, dwAddress, dwCommandCW);
			break;
		case EDeviceDuplexDirection_CCW:
			bReturn = PCIWrite(i32UserArea, dwAddress, dwCommandCCW);
			break;
		case EDeviceDuplexDirection_BOTH:
			bReturn = PCIWrite(i32UserArea, dwAddress, dwCommandBOTH);
			break;
		default:
			break;
		}

		if(bReturn)
			m_bTriggerEnabled = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceMVTechMFB_T::DisableTrigger()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		const int32_t i32UserArea = CTOINT32(MFB_T::BoardInfo::PCI_BASE_CS0);
		const DWORD dwAddress = DWORD(CTOINT32(MFB_T::AddressMap::TriggerEnable));

		const DWORD dwCommandOff = 0b00000000;
		bReturn = PCIWrite(i32UserArea, dwAddress, dwCommandOff);

		m_bTriggerEnabled = false;
	}
	while(false);

	return bReturn;
}

bool CDeviceMVTechMFB_T::IsTriggerEnabled()
{
	return m_bTriggerEnabled;
}

long CDeviceMVTechMFB_T::GetEncoderCount()
{
	long lReturn = 0;

	do 
	{
		if(!IsInitialized())
			break;

		const int32_t i32UserArea = CTOINT32(MFB_T::BoardInfo::PCI_BASE_CS0);
		const DWORD dwAddress = DWORD(CTOINT32(MFB_T::AddressMap::EncoderRead));

		lReturn = PCIRead(i32UserArea, dwAddress);
	}
	while(false);

	return lReturn;
}

long CDeviceMVTechMFB_T::GetTriggerCount()
{
	long lReturn = 0;

	do
	{
		if(!IsInitialized())
			break;

		const int32_t i32UserArea = CTOINT32(MFB_T::BoardInfo::PCI_BASE_CS0);
		const DWORD dwAddress = DWORD(CTOINT32(MFB_T::AddressMap::TriggerRead));

		lReturn = PCIRead(i32UserArea, dwAddress);
	}
	while(false);

	return lReturn;
}

bool CDeviceMVTechMFB_T::ResetCounter()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		const int32_t i32UserArea = CTOINT32(MFB_T::BoardInfo::PCI_BASE_CS0);
		const DWORD dwAddress = DWORD(CTOINT32(MFB_T::AddressMap::ResetCounter));

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

bool CDeviceMVTechMFB_T::ReadOutBit(int nBit)
{
	bool res = false;

	do
	{
		if(!IsInitialized())
			break;

		if(nBit >= CTOINT32(MFB_T::BoardInfo::OutportCount) || nBit < 0)
			break;

		res = (m_dwOutStatus >> nBit) & 0x01;
	}
	while(false);

	return res;
}

bool CDeviceMVTechMFB_T::ReadInBit(int nBit)
{
	bool res = false;

	do
	{
		if(!IsInitialized())
			break;

		if(nBit >= CTOINT32(MFB_T::BoardInfo::InportCount) || nBit < 0)
			break;

		const int32_t i32UserArea = CTOINT32(MFB_T::BoardInfo::PCI_BASE_CS0);
		const DWORD dwAddress = DWORD(CTOINT32(MFB_T::AddressMap::Input));

		PLX_STATUS rc = PlxPci_PciBarSpaceRead((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_pDwCS[i32UserArea] + dwAddress, &m_dwInStatus, sizeof(m_dwInStatus), BitSize32, true);
		if(rc != ApiSuccess)
			break;

		res = (m_dwInStatus >> nBit) & 0x01;
	}
	while(false);

	return res;
}

BYTE CDeviceMVTechMFB_T::ReadOutByte()
{
	BYTE res = 0;

	do
	{
		if(!IsInitialized())
			break;

		res = (m_dwOutStatus & BYTE_MAX);
	}
	while(false);

	return res;
}

BYTE CDeviceMVTechMFB_T::ReadInByte()
{
	BYTE res = 0;

	do
	{
		if(!IsInitialized())
			break;

		const int32_t i32UserArea = CTOINT32(MFB_T::BoardInfo::PCI_BASE_CS0);
		const DWORD dwAddress = DWORD(CTOINT32(MFB_T::AddressMap::Input));

		PLX_STATUS rc = PlxPci_PciBarSpaceRead((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_pDwCS[i32UserArea] + dwAddress, &m_dwInStatus, sizeof(m_dwInStatus), BitSize32, true);
		if(rc != ApiSuccess)
			break;

		res = (m_dwInStatus & BYTE_MAX);
	}
	while(false);

	return res;
}

WORD CDeviceMVTechMFB_T::ReadOutWord()
{
	WORD res = 0;

	do
	{
		if(!IsInitialized())
			break;

		res = (m_dwOutStatus & WORD_MAX);
	}
	while(false);

	return res;
}

WORD CDeviceMVTechMFB_T::ReadInWord()
{
	WORD res = 0;

	do
	{
		if(!IsInitialized())
			break;

		const int32_t i32UserArea = CTOINT32(MFB_T::BoardInfo::PCI_BASE_CS0);
		const DWORD dwAddress = DWORD(CTOINT32(MFB_T::AddressMap::Input));

		PLX_STATUS rc = PlxPci_PciBarSpaceRead((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_pDwCS[i32UserArea] + dwAddress, &m_dwInStatus, sizeof(m_dwInStatus), BitSize32, true);
		if(rc != ApiSuccess)
			break;

		res = (m_dwInStatus & WORD_MAX);
	}
	while(false);

	return res;
}

DWORD CDeviceMVTechMFB_T::ReadOutDword()
{
	DWORD res = 0;

	do
	{
		if(!IsInitialized())
			break;

		res = m_dwOutStatus;
	}
	while(false);

	return res;
}

DWORD CDeviceMVTechMFB_T::ReadInDword()
{
	DWORD res = 0;

	do
	{
		if(!IsInitialized())
			break;

		const int32_t i32UserArea = CTOINT32(MFB_T::BoardInfo::PCI_BASE_CS0);
		const DWORD dwAddress = DWORD(CTOINT32(MFB_T::AddressMap::Input));

		PLX_STATUS rc = PlxPci_PciBarSpaceRead((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_pDwCS[i32UserArea] + dwAddress, &m_dwInStatus, sizeof(m_dwInStatus), BitSize32, true);
		if(rc != ApiSuccess)
			break;

		res = m_dwInStatus;
	}
	while(false);

	return res;
}

bool CDeviceMVTechMFB_T::WriteOutBit(int nBit, bool bOn)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(nBit >= CTOINT32(MFB_T::BoardInfo::OutportCount) || nBit < 0)
			break;

		DWORD dwCommand = m_dwOutStatus;

		if(bOn)
			dwCommand |= 1 << nBit;
		else
			dwCommand &= ~(1 << nBit);

		const int32_t i32UserArea = CTOINT32(MFB_T::BoardInfo::PCI_BASE_CS0);
		const DWORD dwAddress = DWORD(CTOINT32(MFB_T::AddressMap::Output));

		PLX_STATUS rc = PlxPci_PciBarSpaceWrite((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_pDwCS[i32UserArea] + dwAddress, &dwCommand, sizeof(dwCommand), BitSize32, true);
		if(rc != ApiSuccess)
			break;

		m_dwOutStatus = dwCommand;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceMVTechMFB_T::WriteOutByte(BYTE cByte)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		DWORD dwCommand = m_dwOutStatus & 0xFFFFFF00;
		dwCommand += cByte;

		const int32_t i32UserArea = CTOINT32(MFB_T::BoardInfo::PCI_BASE_CS0);
		const DWORD dwAddress = DWORD(CTOINT32(MFB_T::AddressMap::Output));

		PLX_STATUS rc = PlxPci_PciBarSpaceWrite((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_pDwCS[i32UserArea] + dwAddress, &dwCommand, sizeof(dwCommand), BitSize32, true);
		if(rc != ApiSuccess)
			break;

		m_dwOutStatus = dwCommand;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceMVTechMFB_T::WriteOutWord(WORD wWord)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		DWORD dwCommand = m_dwOutStatus & 0xFFFF0000;
		dwCommand += wWord;

		const int32_t i32UserArea = CTOINT32(MFB_T::BoardInfo::PCI_BASE_CS0);
		const DWORD dwAddress = DWORD(CTOINT32(MFB_T::AddressMap::Output));

		PLX_STATUS rc = PlxPci_PciBarSpaceWrite((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_pDwCS[i32UserArea] + dwAddress, &dwCommand, sizeof(dwCommand), BitSize32, true);
		if(rc != ApiSuccess)
			break;

		m_dwOutStatus = dwCommand;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceMVTechMFB_T::WriteOutDword(DWORD dwCommand)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		const int32_t i32UserArea = CTOINT32(MFB_T::BoardInfo::PCI_BASE_CS0);
		const DWORD dwAddress = DWORD(CTOINT32(MFB_T::AddressMap::Output));

		PLX_STATUS rc = PlxPci_PciBarSpaceWrite((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_pDwCS[i32UserArea] + dwAddress, &dwCommand, sizeof(dwCommand), BitSize32, true);
		if(rc != ApiSuccess)
			break;

		m_dwOutStatus = dwCommand;
		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceMVTechMFB_T::ClearOutPort()
{
	return WriteOutDword(0);
}

DWORD CDeviceMVTechMFB_T::GetQueuePosition(int nIndex)
{
	DWORD dwReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(nIndex < 0 || nIndex >= CTOINT32(MFB_T::BoardInfo::QueueCount))
			break;


		DWORD dwAddress = 0;
		switch(nIndex)
		{
		case 0:		dwAddress = DWORD(CTOINT32(MFB_T::AddressMap::QueuePosition0));	break;
		case 1:		dwAddress = DWORD(CTOINT32(MFB_T::AddressMap::QueuePosition1));	break;
		case 2:		dwAddress = DWORD(CTOINT32(MFB_T::AddressMap::QueuePosition2));	break;
		case 3:		dwAddress = DWORD(CTOINT32(MFB_T::AddressMap::QueuePosition3));	break;
		case 4:		dwAddress = DWORD(CTOINT32(MFB_T::AddressMap::QueuePosition4));	break;
		case 5:		dwAddress = DWORD(CTOINT32(MFB_T::AddressMap::QueuePosition5));	break;
		case 6:		dwAddress = DWORD(CTOINT32(MFB_T::AddressMap::QueuePosition6));	break;
		case 7:		dwAddress = DWORD(CTOINT32(MFB_T::AddressMap::QueuePosition7));	break;
		}

		const int32_t i32UserArea = CTOINT32(MFB_T::BoardInfo::PCI_BASE_CS0);
		dwReturn = PCIRead(i32UserArea, dwAddress);
	}
	while(false);

	return dwReturn;
}

bool CDeviceMVTechMFB_T::SetQueuePosition(int nIndex, DWORD dwPos)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(nIndex < 0 || nIndex >= CTOINT32(MFB_T::BoardInfo::QueueCount))
			break;


		DWORD dwAddress = 0;
		switch(nIndex)
		{
		case 0:		dwAddress = DWORD(CTOINT32(MFB_T::AddressMap::QueuePosition0));	break;
		case 1:		dwAddress = DWORD(CTOINT32(MFB_T::AddressMap::QueuePosition1));	break;
		case 2:		dwAddress = DWORD(CTOINT32(MFB_T::AddressMap::QueuePosition2));	break;
		case 3:		dwAddress = DWORD(CTOINT32(MFB_T::AddressMap::QueuePosition3));	break;
		case 4:		dwAddress = DWORD(CTOINT32(MFB_T::AddressMap::QueuePosition4));	break;
		case 5:		dwAddress = DWORD(CTOINT32(MFB_T::AddressMap::QueuePosition5));	break;
		case 6:		dwAddress = DWORD(CTOINT32(MFB_T::AddressMap::QueuePosition6));	break;
		case 7:		dwAddress = DWORD(CTOINT32(MFB_T::AddressMap::QueuePosition7));	break;
		}

		const int32_t i32UserArea = CTOINT32(MFB_T::BoardInfo::PCI_BASE_CS0);		
		bReturn = PCIWrite(i32UserArea, dwAddress, dwPos);
	}
	while(false);

	return bReturn;
}

bool CDeviceMVTechMFB_T::SetQueueOrder(int nIndex, DWORD dwCommand)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(nIndex < 0 || nIndex >= CTOINT32(MFB_T::BoardInfo::QueueCount))
			break;
		
		DWORD dwAddress = 0;
		switch(nIndex)
		{
		case 0:		dwAddress = DWORD(CTOINT32(MFB_T::AddressMap::QueueOrder0));	break;
		case 1:		dwAddress = DWORD(CTOINT32(MFB_T::AddressMap::QueueOrder1));	break;
		case 2:		dwAddress = DWORD(CTOINT32(MFB_T::AddressMap::QueueOrder2));	break;
		case 3:		dwAddress = DWORD(CTOINT32(MFB_T::AddressMap::QueueOrder3));	break;
		case 4:		dwAddress = DWORD(CTOINT32(MFB_T::AddressMap::QueueOrder4));	break;
		case 5:		dwAddress = DWORD(CTOINT32(MFB_T::AddressMap::QueueOrder5));	break;
		case 6:		dwAddress = DWORD(CTOINT32(MFB_T::AddressMap::QueueOrder6));	break;
		case 7:		dwAddress = DWORD(CTOINT32(MFB_T::AddressMap::QueueOrder7));	break;
		}

		const int32_t i32UserArea = CTOINT32(MFB_T::BoardInfo::PCI_BASE_CS0);
		bReturn = PCIWrite(i32UserArea, dwAddress, dwCommand);
	}
	while(false);

	return bReturn;
}

bool CDeviceMVTechMFB_T::GetQueueStatus(int nIndex)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(nIndex < 0 || nIndex >= CTOINT32(MFB_T::BoardInfo::QueueCount))
			break;

		const int32_t i32UserArea = CTOINT32(MFB_T::BoardInfo::PCI_BASE_CS0);

		DWORD dwRes= PCIRead(i32UserArea, DWORD(CTOINT32(MFB_T::AddressMap::QueueStatus)));
		bReturn = (dwRes >> nIndex) & 0x01;
	}
	while(false);

	return bReturn;
}

DWORD CDeviceMVTechMFB_T::GetQueueStatus()
{
	DWORD dwReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		const int32_t i32UserArea = CTOINT32(MFB_T::BoardInfo::PCI_BASE_CS0);

		dwReturn = PCIRead(i32UserArea, DWORD(CTOINT32(MFB_T::AddressMap::QueueStatus)));
	}
	while(false);

	return dwReturn;
}

bool CDeviceMVTechMFB_T::WriteEncoder(DWORD dwCommand)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		const int32_t i32UserArea = CTOINT32(MFB_T::BoardInfo::PCI_BASE_CS0);
		const DWORD dwAddress = DWORD(CTOINT32(MFB_T::AddressMap::SetEncoder));
		bReturn = PCIWrite(i32UserArea, dwAddress, dwCommand);
	}
	while(false);

	return bReturn;
}

bool CDeviceMVTechMFB_T::WriteLive(DWORD dwCommand)
{
	bool bReturn = false;

	do 
	{
		if(!IsInitialized())
			break;

		const int32_t i32UserArea = CTOINT32(MFB_T::BoardInfo::PCI_BASE_CS0);
		const DWORD dwAddress = DWORD(CTOINT32(MFB_T::AddressMap::LiveMode));
		bReturn = PCIWrite(i32UserArea, dwAddress, dwCommand);
	}
	while(false);

	return bReturn;
}

EGetFunction CDeviceMVTechMFB_T::GetUseClearInitalize(bool * pParam)
{
	EGetFunction eReturn = EGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMVTechMFB_T_UseClearInitialize));

		eReturn = EGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESetFunction CDeviceMVTechMFB_T::SetUseClearInitalize(bool bParam)
{
	ESetFunction eReturn = ESetFunction_UnknownError;

	EDeviceParameterMVTechMFB_T eSaveNum = EDeviceParameterMVTechMFB_T_UseClearInitialize;

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMFB_T[eSaveNum], g_lpszSwitch[bPreValue], g_lpszSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGetFunction CDeviceMVTechMFB_T::GetUseClearTerminate(bool * pParam)
{
	EGetFunction eReturn = EGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMVTechMFB_T_UseClearTerminate));

		eReturn = EGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESetFunction CDeviceMVTechMFB_T::SetUseClearTerminate(bool bParam)
{
	ESetFunction eReturn = ESetFunction_UnknownError;

	EDeviceParameterMVTechMFB_T eSaveNum = EDeviceParameterMVTechMFB_T_UseClearTerminate;

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMFB_T[eSaveNum], g_lpszSwitch[bPreValue], g_lpszSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGetFunction CDeviceMVTechMFB_T::GetTriggerDirection(EDeviceDuplexDirection* pParam)
{
	EGetFunction eReturn = EGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGetFunction_NullptrError;
			break;
		}

		*pParam = EDeviceDuplexDirection(_ttoi(GetParamValue(EDeviceParameterMVTechMFB_T_TriggerDirection)));

		eReturn = EGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESetFunction CDeviceMVTechMFB_T::SetTriggerDirection(EDeviceDuplexDirection eParam)
{
	ESetFunction eReturn = ESetFunction_UnknownError;

	EDeviceParameterMVTechMFB_T eSaveID = EDeviceParameterMVTechMFB_T_TriggerDirection;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = ESetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = ESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMFB_T[eSaveID], g_lpszDuplexDirection[nPreValue], g_lpszDuplexDirection[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EGetFunction CDeviceMVTechMFB_T::GetEncoderDirection(EDeviceSimplexDirection* pParam)
{
	EGetFunction eReturn = EGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGetFunction_NullptrError;
			break;
		}

		*pParam = EDeviceSimplexDirection(_ttoi(GetParamValue(EDeviceParameterMVTechMFB_T_EncoderDirection)));

		eReturn = EGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESetFunction CDeviceMVTechMFB_T::SetEncoderDirection(EDeviceSimplexDirection eParam)
{
	ESetFunction eReturn = ESetFunction_UnknownError;

	EDeviceParameterMVTechMFB_T eSaveID = EDeviceParameterMVTechMFB_T_EncoderDirection;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			const int32_t i32UserArea = CTOINT32(MFB_T::BoardInfo::PCI_BASE_CS0);
			DWORD dwAddress = DWORD(CTOINT32(MFB_T::AddressMap::EncoderDirection));

			DWORD dwCommand = 0b00000000;
			if(eParam == EDeviceSimplexDirection_CCW)
				dwCommand = 0b00000001;

			if(!PCIWrite(i32UserArea, dwAddress, dwCommand))
			{
				eReturn = ESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = ESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMFB_T[eSaveID], g_lpszSimplexDirection[nPreValue], g_lpszSimplexDirection[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceMVTechMFB_T::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = false;

	bool bFoundID = true;

	CString strMessage;

	do
	{
		switch(nParam)
		{
		case EDeviceParameterMVTechMFB_T_DeviceID:
			bReturn = !SetDeviceID(strValue);
			break;
		case EDeviceParameterMVTechMFB_T_UseClearInitialize:
			bReturn = !SetUseClearInitalize(_ttoi(strValue));
			bFoundID = true;
			break;
		case EDeviceParameterMVTechMFB_T_UseClearTerminate:
			bReturn = !SetUseClearTerminate(_ttoi(strValue));
			bFoundID = true;
			break;
		case EDeviceParameterMVTechMFB_T_TriggerDirection:
			bReturn = !SetTriggerDirection(EDeviceDuplexDirection(_ttoi(strValue)));
			break;
		case EDeviceParameterMVTechMFB_T_EncoderDirection:
			bReturn = !SetEncoderDirection(EDeviceSimplexDirection(_ttoi(strValue)));
			break;
		case EDeviceParameterMVTechMFB_T_UseInputNo0:
		case EDeviceParameterMVTechMFB_T_UseInputNo1:
		case EDeviceParameterMVTechMFB_T_UseInputNo2:
		case EDeviceParameterMVTechMFB_T_UseInputNo3:
		case EDeviceParameterMVTechMFB_T_UseInputNo4:
		case EDeviceParameterMVTechMFB_T_UseInputNo5:
		case EDeviceParameterMVTechMFB_T_UseInputNo6:
		case EDeviceParameterMVTechMFB_T_UseInputNo7:
		case EDeviceParameterMVTechMFB_T_UseInputNo8:
		case EDeviceParameterMVTechMFB_T_UseInputNo9:
		case EDeviceParameterMVTechMFB_T_UseInputNo10:
		case EDeviceParameterMVTechMFB_T_UseInputNo11:
		case EDeviceParameterMVTechMFB_T_UseInputNo12:
		case EDeviceParameterMVTechMFB_T_UseInputNo13:
		case EDeviceParameterMVTechMFB_T_UseInputNo14:
		case EDeviceParameterMVTechMFB_T_UseInputNo15:
		{
			int nPreValue = _ttoi(GetParamValue(nParam));
			int nValue = _ttoi(strValue);

			if(SetParamValue(nParam, strValue))
			{
				m_pUsedInput[nParam - EDeviceParameterMVTechMFB_T_UseInputNo0] = nValue;

				bReturn = SaveSettings(nParam);
			}

			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMFB_T[nParam], g_lpszSwitch[nPreValue], g_lpszSwitch[nValue]);

			if(bReturn)
			{
				MakeViewThreadDisplayer(pDisp)
				{
					AddControls();
				};

				AddThreadDisplayer(pDisp);
			}
		}
		break;
		case EDeviceParameterMVTechMFB_T_UseOutputNo0:
		case EDeviceParameterMVTechMFB_T_UseOutputNo1:
		case EDeviceParameterMVTechMFB_T_UseOutputNo2:
		case EDeviceParameterMVTechMFB_T_UseOutputNo3:
		case EDeviceParameterMVTechMFB_T_UseOutputNo4:
		case EDeviceParameterMVTechMFB_T_UseOutputNo5:
		case EDeviceParameterMVTechMFB_T_UseOutputNo6:
		case EDeviceParameterMVTechMFB_T_UseOutputNo7:
		case EDeviceParameterMVTechMFB_T_UseOutputNo8:
		case EDeviceParameterMVTechMFB_T_UseOutputNo9:
		case EDeviceParameterMVTechMFB_T_UseOutputNo10:
		case EDeviceParameterMVTechMFB_T_UseOutputNo11:
		case EDeviceParameterMVTechMFB_T_UseOutputNo12:
		case EDeviceParameterMVTechMFB_T_UseOutputNo13:
		case EDeviceParameterMVTechMFB_T_UseOutputNo14:
		case EDeviceParameterMVTechMFB_T_UseOutputNo15:
		{
			int nPreValue = _ttoi(GetParamValue(nParam));
			int nValue = _ttoi(strValue);

			if(SetParamValue(nParam, strValue))
			{
				m_pUsedOutput[nParam - EDeviceParameterMVTechMFB_T_UseOutputNo0] = nValue;

				bReturn = SaveSettings(nParam);
			}

			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMFB_T[nParam], g_lpszSwitch[nPreValue], g_lpszSwitch[nValue]);

			if(bReturn)
			{
				MakeViewThreadDisplayer(pDisp)
				{
					AddControls();
				};

				AddThreadDisplayer(pDisp);
			}
		}
		break;
		case EDeviceParameterMVTechMFB_T_InputNo0Name:
		case EDeviceParameterMVTechMFB_T_InputNo1Name:
		case EDeviceParameterMVTechMFB_T_InputNo2Name:
		case EDeviceParameterMVTechMFB_T_InputNo3Name:
		case EDeviceParameterMVTechMFB_T_InputNo4Name:
		case EDeviceParameterMVTechMFB_T_InputNo5Name:
		case EDeviceParameterMVTechMFB_T_InputNo6Name:
		case EDeviceParameterMVTechMFB_T_InputNo7Name:
		case EDeviceParameterMVTechMFB_T_InputNo8Name:
		case EDeviceParameterMVTechMFB_T_InputNo9Name:
		case EDeviceParameterMVTechMFB_T_InputNo10Name:
		case EDeviceParameterMVTechMFB_T_InputNo11Name:
		case EDeviceParameterMVTechMFB_T_InputNo12Name:
		case EDeviceParameterMVTechMFB_T_InputNo13Name:
		case EDeviceParameterMVTechMFB_T_InputNo14Name:
		case EDeviceParameterMVTechMFB_T_InputNo15Name:
		{
			CString strPreValue = GetParamValue(nParam);

			if(SetParamValue(nParam, strValue))
			{
				int nIndex = nParam - EDeviceParameterMVTechMFB_T_InputNo0Name;

				m_pStrInputName[nIndex] = strValue;

				bReturn = SaveSettings(nParam);

				CString strSave;
				strSave.Format(_T("%d : %s"), nIndex, strValue);

				SetDlgItemTextW(EDeviceAddEnumeratedControlID_InputLedLabelStart + nIndex, strSave);
			}

			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMFB_T[nParam], strPreValue, strValue);

			bFoundID = false;
		}
		break;
		case EDeviceParameterMVTechMFB_T_OutputNo0Name:
		case EDeviceParameterMVTechMFB_T_OutputNo1Name:
		case EDeviceParameterMVTechMFB_T_OutputNo2Name:
		case EDeviceParameterMVTechMFB_T_OutputNo3Name:
		case EDeviceParameterMVTechMFB_T_OutputNo4Name:
		case EDeviceParameterMVTechMFB_T_OutputNo5Name:
		case EDeviceParameterMVTechMFB_T_OutputNo6Name:
		case EDeviceParameterMVTechMFB_T_OutputNo7Name:
		case EDeviceParameterMVTechMFB_T_OutputNo8Name:
		case EDeviceParameterMVTechMFB_T_OutputNo9Name:
		case EDeviceParameterMVTechMFB_T_OutputNo10Name:
		case EDeviceParameterMVTechMFB_T_OutputNo11Name:
		case EDeviceParameterMVTechMFB_T_OutputNo12Name:
		case EDeviceParameterMVTechMFB_T_OutputNo13Name:
		case EDeviceParameterMVTechMFB_T_OutputNo14Name:
		case EDeviceParameterMVTechMFB_T_OutputNo15Name:
		{
			CString strPreValue = GetParamValue(nParam);

			if(SetParamValue(nParam, strValue))
			{
				int nIndex = nParam - EDeviceParameterMVTechMFB_T_OutputNo0Name;

				m_pStrOutputName[nIndex] = strValue;

				bReturn = SaveSettings(nParam);

				CString strSave;
				strSave.Format(_T("%d : %s"), nIndex, strValue);

				SetDlgItemTextW(EDeviceAddEnumeratedControlID_OutputLedLabelStart + nIndex, strSave);
			}

			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamMFB_T[nParam], strPreValue, strValue);

			bFoundID = false;
		}
		break;
		default:
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtofindtheparameter);
			break;
		}
	}
	while(false);

	if(!bFoundID)
	{
		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);
	}

	return bReturn;
}

void CDeviceMVTechMFB_T::SetPCIVendorID(unsigned short vendorId)
{
	m_vendorId = vendorId;
}

void CDeviceMVTechMFB_T::SetPCIDeviceID(unsigned short deviceId)
{
	m_deviceId = deviceId;
}

void CDeviceMVTechMFB_T::OnTimer(UINT_PTR nIDEvent)
{
	if(IsWindowVisible())
	{
		if(nIDEvent == RAVID_TIMER_DIO)
		{
			this->ReadInDword();

			const int32_t i32InputMaxLed = CTOINT32(MFB_T::BoardInfo::InportCount);
			const int32_t i32OutputMaxLed = CTOINT32(MFB_T::BoardInfo::OutportCount);

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

		if(nIDEvent == RAVID_TIMER_TRIGGER)
		{
			const long nQueueCounts = CTOINT32(MFB_T::BoardInfo::QueueCount);

			DWORD dwStatus = this->GetQueueStatus();

			for(int i = 0; i < nQueueCounts; ++i)
			{
				CRavidLedCtrl* pLedTrigger = (CRavidLedCtrl*)GetDlgItem(EDeviceAddEnumeratedControlID_QueueLedStart + i);

				if(pLedTrigger)
				{
					pLedTrigger->SetActivate(IsInitialized());

					bool bQueueStatus = (dwStatus >> i) & 0x01;

					if(bQueueStatus)
						pLedTrigger->On();
					else
						pLedTrigger->Off();

					CStatic* pLabel = (CStatic*)GetDlgItem(EDeviceAddEnumeratedControlID_QueueLedLabelStart + i);
					if(pLabel)
					{
						CString strPreQueue;
						pLabel->GetWindowTextW(strPreQueue);

						CString strQueue;
						strQueue.Format(_T("%d"), this->GetQueuePosition(i));

						if(strPreQueue != strQueue)
							pLabel->SetWindowTextW(strQueue);
					}
				}
			}
		}
	}

	CDeviceTrigger::OnTimer(nIDEvent);
}

bool CDeviceMVTechMFB_T::DoesModuleExist()
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

bool CDeviceMVTechMFB_T::AddControls()
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

		const int nInputMaxLed = CTOINT32(MFB_T::BoardInfo::InportCount);
		const int nOutputMaxLed = CTOINT32(MFB_T::BoardInfo::OutportCount);

		AllocateIO(nInputMaxLed, nOutputMaxLed);

		CRect rrResultGroupBox;

		CPoint rpSheetLeftTop(rrSheet.right + nDialogEdgeMargin, rrInitBtn.bottom);
		CPoint rpSheetRightBottom(rpSheetLeftTop.x + rrSheet.right - rrSheet.left, rrSheet.bottom + nDialogEdgeMargin);

		CPoint rpLT(rrDlg.left, rrDlg.bottom);
		CPoint rpRB(rrSheet.right, rrDlg.bottom);
		CPoint rpRes;
		AddQueues(dblDPIScale, 0, pBaseFont, rpLT, rpRB, &rpRes);

		CreateLedArray(dblDPIScale, pBaseFont, rpSheetLeftTop, rpSheetRightBottom, rrResultGroupBox);

		rrDlg.right = __max(rrSheet.right, rrResultGroupBox.right) + nDialogEdgeMargin;
		rrDlg.bottom = __max(rpRes.y, rrResultGroupBox.bottom + nDialogEdgeMargin);

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

bool CDeviceMVTechMFB_T::AddQueues(double dblDPIScale, int nStartID, CFont* pBaseFont, CPoint cpLeftTopPt, CPoint cpRightBottomPt, CPoint* pResultGroupBox)
{
	bool bReturn = false;

	do
	{
		CButton* pGroupBox;

		pGroupBox = new CButton;

		CRavidRect<int> rrGroupBox;

		rrGroupBox.SetRect(cpLeftTopPt.x, cpLeftTopPt.y, cpRightBottomPt.x, cpRightBottomPt.y);

		CRavidPoint<int> rpLedBasePos(rrGroupBox.left + 10 * dblDPIScale, rrGroupBox.top + 20 * dblDPIScale);

		CRavidRect<int> rrLedRect(rpLedBasePos.x, rpLedBasePos.y, rpLedBasePos.x + 15 * dblDPIScale, rpLedBasePos.y + 15 * dblDPIScale);
		CRavidRect<int> rrQueueValue(0, 0, 65 * dblDPIScale, 15 * dblDPIScale);

		CRavidPoint<int> rpLedXGap(rrQueueValue.GetWidth() + 40 * dblDPIScale, 0);
		CRavidPoint<int> rpLedYGap(0, 22 * dblDPIScale);

		int nDialogEdgeMargin = 10 * dblDPIScale;

		int nRealPosX = 0;
		int nRealPosY = 0;

		const long nQueueCounts = CTOINT32(MFB_T::BoardInfo::QueueCount);

		for(int i = 0; i < nQueueCounts; ++i)
		{
			CRavidRect<int> rrLed(rrLedRect + rpLedYGap * nRealPosY + rpLedXGap * nRealPosX);
			CRavidRect<int> rrValue(rrQueueValue);
			rrValue += CRavidPoint<int>(rrLed.right, rrLed.top);
			rrValue.left += (5 * dblDPIScale);
			rrValue.right += (5 * dblDPIScale);

			CString strID;
			strID.Format(_T("%d"), i);

			CRavidLedCtrl* pLed = new CRavidLedCtrl;
			pLed->Create(strID, WS_VISIBLE | WS_CHILD | SS_NOTIFY, rrLed, this, EDeviceAddEnumeratedControlID_QueueLedStart + i + nStartID);
			pLed->SetFont(pBaseFont);
			m_vctCtrl.push_back(pLed);

			CStatic* pLabel = new CStatic;

			CString strName;
			strName.Format(_T("%d"), 0);
			pLabel->Create(strName, WS_VISIBLE | WS_CHILD | SS_NOTIFY | DT_RIGHT, rrValue, this, EDeviceAddEnumeratedControlID_QueueLedLabelStart + i + nStartID);
			pLabel->SetFont(pBaseFont);
			m_vctCtrl.push_back(pLabel);

			++nRealPosX;

			if(!(nRealPosX % 4))
			{
				++nRealPosY;
				nRealPosX = 0;
			}
		}

		rrGroupBox.bottom = __max(rrLedRect.bottom + rpLedYGap.y * (nRealPosY - 1) + nDialogEdgeMargin, rrGroupBox.bottom);

		int nCtrlIndex = !nStartID ? 0 : 1;
		pGroupBox->Create(CMultiLanguageManager::GetString(ELanguageParameter_QueueInfo), BS_GROUPBOX | WS_CHILD | WS_VISIBLE, rrGroupBox, this, EDeviceAddEnumeratedControlID_StaticStart + 17);

		pGroupBox->SetFont(pBaseFont);
		m_vctCtrl.push_back(pGroupBox);


		pResultGroupBox->x = rrGroupBox.right;
		pResultGroupBox->y = rrGroupBox.bottom;
	}
	while(false);

	return bReturn;
}

bool CDeviceMVTechMFB_T::CreateLedArray(double dblDPIScale, CFont * pBaseFont, CPoint rpLeftTopPt, CPoint rpRightTopPt, CRect& rrResultGroupBox)
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

		const int nInputMaxLed = CTOINT32(MFB_T::BoardInfo::InportCount);
		const int nOutputMaxLed = CTOINT32(MFB_T::BoardInfo::OutportCount);

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

void CDeviceMVTechMFB_T::OnBnClickedOutLed(UINT nID)
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

BOOL CDeviceMVTechMFB_T::OnInitDialog()
{
	CDeviceTrigger::OnInitDialog();

	SetTimer(RAVID_TIMER_DIO, 10, nullptr);

	return TRUE;
}

void CDeviceMVTechMFB_T::OnDestroy()
{
	CDeviceTrigger::OnDestroy();

	KillTimer(RAVID_TIMER_DIO);
}

void CDeviceMVTechMFB_T::AllocateIO(long nInputSize, long nOutputSize)
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

void CDeviceMVTechMFB_T::FreeIO()
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