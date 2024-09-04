#include "stdafx.h"

#include "DeviceMVTechTrigger.h"

#ifdef USING_DEVICE

#include "../RavidCore/RavidDatabase.h"
#include "../RavidCore/RavidRecordset.h"
#include "../RavidCore/PerformanceCounter.h"

#include "../RavidFramework/LogManager.h"
#include "../RavidFramework/DeviceManager.h"
#include "../RavidFramework/LibraryManager.h"
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

IMPLEMENT_DYNAMIC(CDeviceMVTechTrigger, CDeviceTrigger)

BEGIN_MESSAGE_MAP(CDeviceMVTechTrigger, CDeviceTrigger)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static LPCTSTR g_lpszDeviceMVtechTrigger[EDeviceParameterMVTechTrigger_Count] =
{
	_T("DeviceID"),
	_T("Start position"),
	_T("End position"),
	_T("Trigger cycle"),
	_T("Trigger pulse duration time"),
	_T("Encoder increasement direction"),
	_T("Trigger counting direction"),
};

static LPCTSTR g_lpszDeviceDirection[2] =
{
	_T("CW"),
	_T("CCW"),
};

CDeviceMVTechTrigger::CDeviceMVTechTrigger()
{
	m_arrDwCS = new DWORD[DeviceMVTechTrigger::MAX_BASE_ADDR_NUMS];
	m_arrDwCSLen = new DWORD[DeviceMVTechTrigger::MAX_BASE_ADDR_NUMS];

	memset(m_arrDwCS, 0, sizeof(DWORD) * DeviceMVTechTrigger::MAX_BASE_ADDR_NUMS);
	memset(m_arrDwCSLen, 0, sizeof(DWORD) * DeviceMVTechTrigger::MAX_BASE_ADDR_NUMS);
}


CDeviceMVTechTrigger::~CDeviceMVTechTrigger()
{
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

EDeviceInitializeResult CDeviceMVTechTrigger::Initialize()
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

		PLX_STATUS rc;
		PLX_DEVICE_KEY plxDeviceKey;

		DWORD dwRegValue[DeviceMVTechTrigger::LOCAL_CONFIG_REG_CNT];

		memset(m_arrDwCS, 0, sizeof(m_arrDwCS));
		memset(m_arrDwCSLen, 0, sizeof(m_arrDwCSLen));
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

		for(int i = 0; i < DeviceMVTechTrigger::LOCAL_CONFIG_REG_CNT; i++)
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

		int nSetData = 0;

		if(GetStartPosition(&nSetData))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("StartPosition"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		rc = PlxPci_PciBarSpaceWrite((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_arrDwCS[DeviceMVTechTrigger::PCI_BASE_CS0] + DeviceMVTechTrigger::SET_START_POS, &nSetData, sizeof(nSetData), BitSize32, true);

		if(rc != ApiSuccess)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("StartPosition"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		if(GetEndPosition(&nSetData))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("EndPosition"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		rc = PlxPci_PciBarSpaceWrite((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_arrDwCS[DeviceMVTechTrigger::PCI_BASE_CS0] + DeviceMVTechTrigger::SET_END_POS, &nSetData, sizeof(nSetData), BitSize32, true);

		if(rc != ApiSuccess)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("EndPosition"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		int nCycle = 0;

		if(GetTriggerCycle(&nCycle))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TriggerCycle"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		int nDuration = 0;

		if(GetTriggerPulseDurationTime(&nDuration))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("PulseDurationTime"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		int nValue = nDuration << 8 | nCycle;

		rc = PlxPci_PciBarSpaceWrite((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_arrDwCS[DeviceMVTechTrigger::PCI_BASE_CS0] + DeviceMVTechTrigger::SET_CYCLE, &nValue, sizeof(nValue), BitSize32, true);

		if(rc != ApiSuccess)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Trigger Info"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}
		
		nValue = nDuration << 8 | nCycle;
					
		rc = PlxPci_PciBarSpaceWrite((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_arrDwCS[DeviceMVTechTrigger::PCI_BASE_CS0] + DeviceMVTechTrigger::SET_CYCLE, &nValue, sizeof(nValue), BitSize32, true);

		if(rc != ApiSuccess)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Trigger Info"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		EDeviceMVTTriggerEncoderIncreasementDirection eED;

		if(GetEncoderIncreasementDirection(&eED))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Encoder IncreasementDirection"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		rc = PlxPci_PciBarSpaceWrite((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_arrDwCS[DeviceMVTechTrigger::PCI_BASE_CS0] + DeviceMVTechTrigger::ENCODER_DIR, &eED, sizeof(eED), BitSize32, true);

		if(rc != ApiSuccess)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Encoder IncreasementDirection"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		ResetCounter();
		
		m_bIsInitialized = true;

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

EDeviceTerminateResult CDeviceMVTechTrigger::Terminate()
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

bool CDeviceMVTechTrigger::LoadSettings()
{
	bool bReturn = false;

	m_vctParameterFieldConfigurations.clear();

	do 
	{
		AddParameterFieldConfigurations(EDeviceParameterMVTechMSPTrigger_DeviceID, g_lpszDeviceMVtechTrigger[EDeviceParameterMVTechMSPTrigger_DeviceID], _T("0"));
		AddParameterFieldConfigurations(EDeviceParameterMVTechTrigger_Startposition, g_lpszDeviceMVtechTrigger[EDeviceParameterMVTechTrigger_Startposition], _T("0"));
		AddParameterFieldConfigurations(EDeviceParameterMVTechTrigger_Endposition, g_lpszDeviceMVtechTrigger[EDeviceParameterMVTechTrigger_Endposition], _T("0"));
		AddParameterFieldConfigurations(EDeviceParameterMVTechTrigger_Triggercycle, g_lpszDeviceMVtechTrigger[EDeviceParameterMVTechTrigger_Triggercycle], _T("0"));
		AddParameterFieldConfigurations(EDeviceParameterMVTechTrigger_Triggerpulsedurationtime, g_lpszDeviceMVtechTrigger[EDeviceParameterMVTechTrigger_Triggerpulsedurationtime], _T("0"));
		AddParameterFieldConfigurations(EDeviceParameterMVTechTrigger_Encoderincreasementdirection, g_lpszDeviceMVtechTrigger[EDeviceParameterMVTechTrigger_Encoderincreasementdirection], _T("CW"), EParameterFieldType_Combo, _T("CW;CCW"));
		AddParameterFieldConfigurations(EDeviceParameterMVTechTrigger_Triggercountingdirection, g_lpszDeviceMVtechTrigger[EDeviceParameterMVTechTrigger_Triggercountingdirection], _T("CW"), EParameterFieldType_Combo, _T("CW;CCW"));

		bReturn = true;		
	}
	while(false);

	return bReturn & __super::LoadSettings();
}

bool CDeviceMVTechTrigger::EnableTrigger()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(IsTriggerEnabled())
			break;

		bool bCCW = _ttoi(GetParamValue(EDeviceParameterMVTechTrigger_Triggercountingdirection));
		
		long nValue = bCCW ? 0x02 : 0x01;

		PLX_STATUS rc = PlxPci_PciBarSpaceWrite((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_arrDwCS[DeviceMVTechTrigger::PCI_BASE_CS0] + DeviceMVTechTrigger::TRIG_ENABLE, &nValue, sizeof(nValue), BitSize32, true);

		bReturn = rc == ApiSuccess;

		if(!bReturn)
			break;

		m_bTriggerEnabled = true;
	}
	while(false);

	if(IsTriggerEnabled())
		SetMessage(CMultiLanguageManager::GetString(ELanguageParameter_Triggerisenabled));
		
	return bReturn;
}

bool CDeviceMVTechTrigger::DisableTrigger()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		long nValue = 0;

		PLX_STATUS rc = PlxPci_PciBarSpaceWrite((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_arrDwCS[DeviceMVTechTrigger::PCI_BASE_CS0] + DeviceMVTechTrigger::TRIG_ENABLE, &nValue, sizeof(nValue), BitSize32, true);

		bReturn = rc == ApiSuccess;

		if(!bReturn)
			break;

		m_bTriggerEnabled = false;
	}
	while(false);

	if(!IsTriggerEnabled())
		SetMessage(CMultiLanguageManager::GetString(ELanguageParameter_Triggerisdisabled));

	return bReturn;
}

bool CDeviceMVTechTrigger::IsTriggerEnabled()
{
	return m_bTriggerEnabled;
}

long CDeviceMVTechTrigger::GetEncoderCount()
{
	long nReturn = 0;

	do
	{
		if(!IsInitialized())
			break;

		PLX_STATUS rc = PlxPci_PciBarSpaceRead((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_arrDwCS[DeviceMVTechTrigger::PCI_BASE_CS0] + DeviceMVTechTrigger::GET_ENCODER, &nReturn, sizeof(nReturn), BitSize32, true);

		if(rc != ApiSuccess)
			break;
	}
	while(false);

	return nReturn;
}

long CDeviceMVTechTrigger::GetTriggerCount()
{
	long nReturn = 0;

	do
	{
		if(!IsInitialized())
			break;

		PLX_STATUS rc = PlxPci_PciBarSpaceRead((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_arrDwCS[DeviceMVTechTrigger::PCI_BASE_CS0] + DeviceMVTechTrigger::GET_TRIGGER, &nReturn, sizeof(nReturn), BitSize32, true);

		if(rc != ApiSuccess)
			break;
	}
	while(false);

	return nReturn;
}

bool CDeviceMVTechTrigger::ResetCounter()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		long nValue = 1;

		PLX_STATUS rc = PlxPci_PciBarSpaceWrite((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_arrDwCS[DeviceMVTechTrigger::PCI_BASE_CS0] + DeviceMVTechTrigger::COUNT_CLEAR, &nValue, sizeof(nValue), BitSize32, true);

		bReturn = rc == ApiSuccess;

		Sleep(1);

		nValue = 0;

		rc = PlxPci_PciBarSpaceWrite((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_arrDwCS[DeviceMVTechTrigger::PCI_BASE_CS0] + DeviceMVTechTrigger::COUNT_CLEAR, &nValue, sizeof(nValue), BitSize32, true);

		bReturn &= rc == ApiSuccess;
	}
	while(false);

	if(bReturn)
		SetMessage(CMultiLanguageManager::GetString(ELanguageParameter_Counterisreset));

	return bReturn;
}

EMVTTriggerGetFunction CDeviceMVTechTrigger::GetStartPosition(int * pParam)
{
	EMVTTriggerGetFunction eReturn = EMVTTriggerGetFunction_UnknownError;

	do 
	{
		if(!pParam)
		{
			eReturn = EMVTTriggerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMVTechTrigger_Startposition));

		eReturn = EMVTTriggerGetFunction_OK;
	} 
	while(false);

	return eReturn;
}

EMVTTriggerSetFunction CDeviceMVTechTrigger::SetStartPosition(int nParam)
{
	EMVTTriggerSetFunction eReturn = EMVTTriggerSetFunction_UnknownError;

	EDeviceParameterMVTechTrigger eSaveID = EDeviceParameterMVTechTrigger_Startposition;
	
	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do 
	{
		if(IsInitialized())
		{
			PLX_STATUS rc = PlxPci_PciBarSpaceWrite((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_arrDwCS[DeviceMVTechTrigger::PCI_BASE_CS0] + DeviceMVTechTrigger::SET_START_POS, &nParam, sizeof(nParam), BitSize32, true);
			
			if(rc != ApiSuccess)
			{
				eReturn = EMVTTriggerSetFunction_WriteToDeviceError;
				break;
			}
		}
		
		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMVTTriggerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMVTTriggerSetFunction_WriteToDatabaseError;
			break;
		}
		
		eReturn = EMVTTriggerSetFunction_OK;
	} 
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMVtechTrigger[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMVTTriggerGetFunction CDeviceMVTechTrigger::GetEndPosition(int * pParam)
{
	EMVTTriggerGetFunction eReturn = EMVTTriggerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMVTTriggerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMVTechTrigger_Endposition));

		eReturn = EMVTTriggerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMVTTriggerSetFunction CDeviceMVTechTrigger::SetEndPosition(int nParam)
{
	EMVTTriggerSetFunction eReturn = EMVTTriggerSetFunction_UnknownError;

	EDeviceParameterMVTechTrigger eSaveID = EDeviceParameterMVTechTrigger_Endposition;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			PLX_STATUS rc = PlxPci_PciBarSpaceWrite((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_arrDwCS[DeviceMVTechTrigger::PCI_BASE_CS0] + DeviceMVTechTrigger::SET_END_POS, &nParam, sizeof(nParam), BitSize32, true);

			if(rc != ApiSuccess)
			{
				eReturn = EMVTTriggerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMVTTriggerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMVTTriggerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMVTTriggerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMVtechTrigger[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMVTTriggerGetFunction CDeviceMVTechTrigger::GetTriggerCycle(int * pParam)
{
	EMVTTriggerGetFunction eReturn = EMVTTriggerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMVTTriggerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMVTechTrigger_Triggercycle));

		eReturn = EMVTTriggerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMVTTriggerSetFunction CDeviceMVTechTrigger::SetTriggerCycle(int nParam)
{
	EMVTTriggerSetFunction eReturn = EMVTTriggerSetFunction_UnknownError;

	EDeviceParameterMVTechTrigger eSaveID = EDeviceParameterMVTechTrigger_Triggercycle;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			int nDuration = 0;

			if(GetTriggerPulseDurationTime(&nDuration))
			{
				eReturn = EMVTTriggerSetFunction_ReadOnDatabaseError;
				break;
			}

			int nValue = nDuration << 8 | nParam;

			PLX_STATUS rc = PlxPci_PciBarSpaceWrite((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_arrDwCS[DeviceMVTechTrigger::PCI_BASE_CS0] + DeviceMVTechTrigger::SET_CYCLE, &nValue, sizeof(nValue), BitSize32, true);
			
			if(rc != ApiSuccess)
			{
				eReturn = EMVTTriggerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMVTTriggerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMVTTriggerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMVTTriggerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMVtechTrigger[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMVTTriggerGetFunction CDeviceMVTechTrigger::GetTriggerPulseDurationTime(int * pParam)
{
	EMVTTriggerGetFunction eReturn = EMVTTriggerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMVTTriggerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMVTechTrigger_Triggerpulsedurationtime));

		eReturn = EMVTTriggerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMVTTriggerSetFunction CDeviceMVTechTrigger::SetTriggerPulseDurationTime(int nParam)
{
	EMVTTriggerSetFunction eReturn = EMVTTriggerSetFunction_UnknownError;

	EDeviceParameterMVTechTrigger eSaveID = EDeviceParameterMVTechTrigger_Triggerpulsedurationtime;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			int nCycle = 0;

			if(GetTriggerCycle(&nCycle))
			{
				eReturn = EMVTTriggerSetFunction_WriteToDatabaseError;
				break;
			}
			
			int nValue = nParam << 8 | nCycle;

			PLX_STATUS rc = PlxPci_PciBarSpaceWrite((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_arrDwCS[DeviceMVTechTrigger::PCI_BASE_CS0] + DeviceMVTechTrigger::SET_CYCLE, &nValue, sizeof(nValue), BitSize32, true);
			
			if(rc != ApiSuccess)
			{
				eReturn = EMVTTriggerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EMVTTriggerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMVTTriggerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMVTTriggerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMVtechTrigger[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMVTTriggerGetFunction CDeviceMVTechTrigger::GetEncoderIncreasementDirection(EDeviceMVTTriggerEncoderIncreasementDirection* pParam)
{
	EMVTTriggerGetFunction eReturn = EMVTTriggerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMVTTriggerGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMVTTriggerEncoderIncreasementDirection)_ttoi(GetParamValue(EDeviceParameterMVTechTrigger_Encoderincreasementdirection));

		eReturn = EMVTTriggerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMVTTriggerSetFunction CDeviceMVTechTrigger::SetEncoderIncreasementDirection(EDeviceMVTTriggerEncoderIncreasementDirection eParam)
{
	EMVTTriggerSetFunction eReturn = EMVTTriggerSetFunction_UnknownError;

	EDeviceParameterMVTechTrigger eSaveID = EDeviceParameterMVTechTrigger_Encoderincreasementdirection;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			PLX_STATUS rc = PlxPci_PciBarSpaceWrite((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_arrDwCS[DeviceMVTechTrigger::PCI_BASE_CS0] + DeviceMVTechTrigger::ENCODER_DIR, &eParam, sizeof(eParam), BitSize32, true);

			if(rc != ApiSuccess)
			{
				eReturn = EMVTTriggerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EMVTTriggerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMVTTriggerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMVTTriggerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMVtechTrigger[eSaveID], g_lpszDeviceDirection[nPreValue], g_lpszDeviceDirection[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMVTTriggerGetFunction CDeviceMVTechTrigger::GetTriggerCountingDirection(EDeviceMVTTriggerTriggerCountingDirection * pParam)
{
	EMVTTriggerGetFunction eReturn = EMVTTriggerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMVTTriggerGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceMVTTriggerTriggerCountingDirection)_ttoi(GetParamValue(EDeviceParameterMVTechTrigger_Triggercountingdirection));

		eReturn = EMVTTriggerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMVTTriggerSetFunction CDeviceMVTechTrigger::SetTriggerCountingDirection(EDeviceMVTTriggerTriggerCountingDirection eParam)
{
	EMVTTriggerSetFunction eReturn = EMVTTriggerSetFunction_UnknownError;

	EDeviceParameterMVTechTrigger eSaveID = EDeviceParameterMVTechTrigger_Triggercountingdirection;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			bool bReturn = true;

			if(IsTriggerEnabled())
			{
				bReturn &= DisableTrigger();
				bReturn &= EnableTrigger();
			}

			if(!bReturn)
			{
				eReturn = EMVTTriggerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EMVTTriggerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMVTTriggerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMVTTriggerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMVtechTrigger[eSaveID], g_lpszDeviceDirection[nPreValue], g_lpszDeviceDirection[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceMVTechTrigger::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = false;

	bool bFoundID = true;

	do
	{
		switch(nParam)
		{
		case EDeviceParameterMVTechTrigger_DeviceID:
			bReturn = !SetDeviceID(strValue);
			break;
		case EDeviceParameterMVTechTrigger_Startposition:
			bReturn = !SetStartPosition(_ttoi(strValue));
			break;
		case EDeviceParameterMVTechTrigger_Endposition:
			bReturn = !SetEndPosition(_ttoi(strValue));
			break;
		case EDeviceParameterMVTechTrigger_Triggercycle:
			bReturn = !SetTriggerCycle(_ttoi(strValue));
			break;
		case EDeviceParameterMVTechTrigger_Triggerpulsedurationtime:
			bReturn = !SetTriggerPulseDurationTime(_ttoi(strValue));
			break;
		case EDeviceParameterMVTechTrigger_Encoderincreasementdirection:
			bReturn = !SetEncoderIncreasementDirection((EDeviceMVTTriggerEncoderIncreasementDirection)_ttoi(strValue));
			break;
		case EDeviceParameterMVTechTrigger_Triggercountingdirection:
			bReturn = !SetTriggerCountingDirection((EDeviceMVTTriggerTriggerCountingDirection)_ttoi(strValue));
			break;
		default:
			bFoundID = false;
			break;
		}
	}
	while(false);

	if(!bFoundID)
	{
		CString strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtofindtheparameter);

		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);
	}

	return bReturn;
}

bool CDeviceMVTechTrigger::DisplayCounter()
{
	bool bReturn = false;

	do
	{
		CRavidLedCtrl* pLedEncoder = (CRavidLedCtrl*)GetDlgItem(EDeviceAddControlID_LedEncoder);

		if(pLedEncoder)
			pLedEncoder->SetActivate(IsInitialized());

		CRavidLedCtrl* pLedTrigger = (CRavidLedCtrl*)GetDlgItem(EDeviceAddControlID_LedTrigger);

		if(pLedTrigger)
			pLedTrigger->SetActivate(IsInitialized());

		long nEncoderCount = GetEncoderCount();
		long nTriggerCount = GetTriggerCount();
		float fDuration = m_pRpc->GetElapsedTime();
		m_pRpc->Start();

		CWnd* pWnd = GetDlgItem(EDeviceAddControlID_EncoderCountValue);

		if(pWnd)
		{
			long nEncoderDiff = nEncoderCount - m_nBeforeEncoderValue;
			m_nBeforeEncoderValue = nEncoderCount;

			CString strCounter;

			strCounter.Format(_T("%d (%.03f KHz)"), nEncoderCount, nEncoderDiff / fDuration);

			CString strOrigCounter;

			pWnd->GetWindowText(strOrigCounter);

			if(strCounter.Compare(strOrigCounter))
			{
				pWnd->SetWindowText(strCounter);

				if(pLedEncoder)
					pLedEncoder->On();
			}
			else
			{
				if(pLedEncoder)
					pLedEncoder->Off();
			}
		}

		pWnd = GetDlgItem(EDeviceAddControlID_TriggerCountValue);

		if(pWnd)
		{
			long nTriggerDiff = nTriggerCount - m_nBeforeTriggerValue;
			m_nBeforeTriggerValue = nTriggerCount;

			CString strCounter;

			strCounter.Format(_T("%d (%.03f KHz)"), nTriggerCount, nTriggerDiff / fDuration);

			CString strOrigCounter;

			pWnd->GetWindowText(strOrigCounter);

			if(strCounter.Compare(strOrigCounter))
			{
				pWnd->SetWindowText(strCounter);

				if(pLedTrigger)
					pLedTrigger->On();
			}
			else
			{
				if(pLedTrigger)
					pLedTrigger->Off();
			}
		}
	}
	while(false);

	return bReturn;
}


void CDeviceMVTechTrigger::SetPCIVendorID(unsigned short vendorId)
{
	m_vendorId = vendorId;
}

void CDeviceMVTechTrigger::SetPCIDeviceID(unsigned short deviceId)
{
	m_deviceId = deviceId;
}

BOOL CDeviceMVTechTrigger::OnInitDialog()
{
	CDeviceTrigger::OnInitDialog();

	SetTimer(RAVID_TIMER_DIO, 100, nullptr);

	return TRUE;
}


void CDeviceMVTechTrigger::OnDestroy()
{
	CDeviceTrigger::OnDestroy();

	KillTimer(RAVID_TIMER_DIO);
}

void CDeviceMVTechTrigger::OnTimer(UINT_PTR nIDEvent)
{
	if(IsWindowVisible())
	{
		if(nIDEvent == RAVID_TIMER_DIO)
			DisplayCounter();
	}

	CDeviceTrigger::OnTimer(nIDEvent);
}

bool CDeviceMVTechTrigger::DoesModuleExist()
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