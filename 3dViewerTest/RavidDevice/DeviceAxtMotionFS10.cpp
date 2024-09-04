#include "stdafx.h"
#include "DeviceAxtMotionFS10.h"

#ifdef USING_DEVICE

#include "DeviceAxtDio.h"
#include "DeviceAxtMotionFS20.h"

#include "../RavidCore/RavidDatabase.h"
#include "../RavidCore/RavidRecordset.h"

#include "../RavidFramework/LogManager.h"
#include "../RavidFramework/DeviceManager.h"
#include "../RavidFramework/LibraryManager.h"
#include "../RavidFramework/FrameworkManager.h"
#include "../RavidFramework/EventHandlerManager.h"
#include "../RavidFramework/MultiLanguageManager.h"

#include "../RavidFramework/RavidLedCtrl.h"

#include "../Libraries/Includes/AxtLib/AXTLib.h"
#include "../Libraries/Includes/AxtLib/AxtCAMCFS.h"

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
	AXT_MODULE_MIN = 0,
	AXT_MODULE_MAX = 99,
};

IMPLEMENT_DYNAMIC(CDeviceAxtMotionFS10, CDeviceMotion)

BEGIN_MESSAGE_MAP(CDeviceAxtMotionFS10, CDeviceMotion)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static LPCTSTR g_lpszParamAxtMotionFS10[EDeviceParameterAxtMotionFS10_Count] =
{
	_T("DeviceID"),
	_T("SubUnitID"),
	_T("UnitPulse"),
	_T("StartSpeed"),
	_T("MaxSpeed"),
	_T("PulseOutputMethod"),
	_T("EncoderMethod"),
	_T("PLimitLevel"),
	_T("NLimitLevel"),
	_T("AlarmInputLevel"),
	_T("InpositionInputLevel"),
};

static LPCTSTR g_lpszAxtMotionFS10PulseOutputMethod[EDeviceAxtMotionFS10PulseOutputMethod_Count] =
{
	_T("OneHighLowHigh"),
	_T("OneHighHighLow"),
	_T("OneLowLowHigh"),
	_T("OneLowHighLow"),
	_T("TwoCcwCwHigh"),
	_T("TwoCcwCwLow"),
	_T("TwoCwCcwHigh"),
	_T("TwoCwCcwLow"),
};

static LPCTSTR g_lpszAxtMotionFS10EncoderMethod[EDeviceAxtMotionFS10EncoderMethod_Count] =
{
	_T("UpDownMode"),
	_T("Sqr1Mode"),
	_T("Sqr2Mode"),
	_T("Sqr4Mode"),
};

static LPCTSTR g_lpszAxtMotionFS10LevelMethod[EDeviceAxtMotionFS10LevelMethod_Count] =
{
	_T("LOW"),
	_T("HIGH"),
};

CDeviceAxtMotionFS10::CDeviceAxtMotionFS10()
{

}

CDeviceAxtMotionFS10::~CDeviceAxtMotionFS10()
{
	Terminate();
}

EDeviceInitializeResult CDeviceAxtMotionFS10::Initialize()
{
	EDeviceInitializeResult eReturn = EDeviceInitializeResult_UnknownError;

	CString strMessage, strStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));
	
	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("AxtMotionFS10"));

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

		UINT8 arrDeviceIDs[AXT_MODULE] = { 0, };

		INT16 nModuleCount = AxtGetModuleCounts(nDeviceID, arrDeviceIDs);

		if(!nModuleCount)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtofindthedevice);
			eReturn = EDeviceInitializeResult_NotOpenedDeviceError;
			break;
		}
		
		int nShiftBit = 0;

		DWORD dwCycle = 0;

		bool bDeviceIntialized = true;

		if(!CFSIsInitialized())
		{
			if(!InitializeCAMCFS(true))
				bDeviceIntialized = false;
		}

		if(!bDeviceIntialized)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_FailedtoinitializeCAMCFS10module);
			eReturn = EDeviceInitializeResult_NotInitializeMotionError;
			break;
		}

		int nSubunitID = _ttoi(GetSubUnitID());

		INT16 nMaxAxis = CFSget_numof_axes(nDeviceID);

		if(nSubunitID < 0 || nSubunitID >= nMaxAxis)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_DoesntsupporttheSubUnitID);
			eReturn = EDeviceInitializeResult_NotInitializeMotionError;
			break;
		}

		double dblUnitPulse = 0.;

		if(GetUnitPulse(&dblUnitPulse))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Unit pulse"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}
		
		CFSset_moveunit_perpulse(nSubunitID, dblUnitPulse);

		double dblStartSpeed = 0.;

		if(GetStartSpeed(&dblStartSpeed))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Start speed"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		CFSset_startstop_speed(nSubunitID, dblStartSpeed);

		double dblMaxSpeed = 0.;

		if(GetMaxSpeed(&dblMaxSpeed))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Max speed"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(!CFSset_max_speed(nSubunitID, dblMaxSpeed))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Max speed"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		EDeviceAxtMotionFS10PulseOutputMethod ePulseOutputMethod = EDeviceAxtMotionFS10PulseOutputMethod_Count;

		if(GetPulseOutputMethod(&ePulseOutputMethod))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Pulse output method"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(!CFSset_pulse_out_method(nSubunitID, ePulseOutputMethod))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Pulse output method"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		EDeviceAxtMotionFS10EncoderMethod eEncoderMethod = EDeviceAxtMotionFS10EncoderMethod_Count;

		if(GetEncoderMethod(&eEncoderMethod))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Encoder method"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(!CFSset_enc_input_method(nSubunitID, eEncoderMethod))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Encoder method"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		EDeviceAxtMotionFS10LevelMethod ePLimitLevel = EDeviceAxtMotionFS10LevelMethod_Count;

		if(GetPLimitLevel(&ePLimitLevel))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("P limit level"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}
		
		if(!CFSset_pend_limit_level(nSubunitID, ePLimitLevel))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("P limit level"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		EDeviceAxtMotionFS10LevelMethod eNLimitLevel = EDeviceAxtMotionFS10LevelMethod_Count;

		if(GetNLimitLevel(&eNLimitLevel))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("N limit level"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(!CFSset_nend_limit_level(nSubunitID, eNLimitLevel))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("N limit level"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		EDeviceAxtMotionFS10LevelMethod eInpositionInputLevel = EDeviceAxtMotionFS10LevelMethod_Count;

		if(GetInpositionInputLevel(&eInpositionInputLevel))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Inposition input level"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(!CFSset_inposition_enable(nSubunitID, eInpositionInputLevel))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Inposition input level"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		EDeviceAxtMotionFS10LevelMethod eAlarmInputLevel = EDeviceAxtMotionFS10LevelMethod_Count;

		if(GetAlarmInputLevel(&eAlarmInputLevel))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Alarm input level"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}
		
		if(!CFSset_alarm_level(nSubunitID, eAlarmInputLevel))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Alarm input level"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		CFSset_pslow_limit_level(nSubunitID, 1);
		CFSset_nslow_limit_level(nSubunitID, 1);

		m_bIsInitialized = true;

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

EDeviceTerminateResult CDeviceAxtMotionFS10::Terminate()
{
	EDeviceTerminateResult eReturn = EDeviceTerminateResult_UnknownError;

	CString strMessage;

	SetStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("AxtMotionFS10"));

		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);

		return EDeviceTerminateResult_NotFoundApiError;
	}

	do
	{
		SetServoOn(false);

		bool bNeedTreminate = true;

		size_t szDeviceCnt = CDeviceManager::GetDeviceCount();

		for(size_t i = 0; i < szDeviceCnt; ++i)
		{
			CDeviceBase* pDevicebase = CDeviceManager::GetDeviceByIndex(i);

			if(!pDevicebase)
				continue;

			bool bMatchDevice = false;

			CDeviceAxtDio* pDio = dynamic_cast<CDeviceAxtDio*>(pDevicebase);

			if(pDio)
			{
				bMatchDevice = true;

				if(!pDio->IsInitialized())
					continue;

				bNeedTreminate = false;

				break;
			}

			CDeviceAxtMotionFS10* pMotionFS10 = dynamic_cast<CDeviceAxtMotionFS10*>(pDevicebase);

			if(pMotionFS10)
			{
				bMatchDevice = true;

				if(pMotionFS10 == this)
					continue;

				if(!pMotionFS10->IsInitialized())
					continue;

				bNeedTreminate = false;

				break;
			}

			CDeviceAxtMotionFS20* pMotionFS20 = dynamic_cast<CDeviceAxtMotionFS20*>(pDevicebase);

			if(pMotionFS20)
			{
				bMatchDevice = true;

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

bool CDeviceAxtMotionFS10::LoadSettings()
{
	bool bReturn = false;

	m_vctParameterFieldConfigurations.clear();
	
	do
	{
		AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS10_DeviceID, g_lpszParamAxtMotionFS10[EDeviceParameterAxtMotionFS10_DeviceID], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS10_SubUnitID, g_lpszParamAxtMotionFS10[EDeviceParameterAxtMotionFS10_SubUnitID], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 0);

		AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS10_UnitPulse, g_lpszParamAxtMotionFS10[EDeviceParameterAxtMotionFS10_UnitPulse], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS10_StartSpeed, g_lpszParamAxtMotionFS10[EDeviceParameterAxtMotionFS10_StartSpeed], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS10_MaxSpeed, g_lpszParamAxtMotionFS10[EDeviceParameterAxtMotionFS10_MaxSpeed], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS10_PulseOutputMethod, g_lpszParamAxtMotionFS10[EDeviceParameterAxtMotionFS10_PulseOutputMethod], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxtMotionFS10PulseOutputMethod, EDeviceAxtMotionFS10PulseOutputMethod_Count), _T("ex) OneHighLowHigh = 1 pulse, PULSE(Active High), cw(DIR=Low)  / ccw(DIR=High)"));
		AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS10_EncoderMethod, g_lpszParamAxtMotionFS10[EDeviceParameterAxtMotionFS10_EncoderMethod], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxtMotionFS10EncoderMethod, EDeviceAxtMotionFS10EncoderMethod_Count), _T("ex) Sqr1Mode = 0x1  // 1Ã¼¹è"));
		AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS10_PLimitLevel, g_lpszParamAxtMotionFS10[EDeviceParameterAxtMotionFS10_PLimitLevel], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxtMotionFS10LevelMethod, EDeviceAxtMotionFS10LevelMethod_Count), nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS10_NLimitLevel, g_lpszParamAxtMotionFS10[EDeviceParameterAxtMotionFS10_NLimitLevel], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxtMotionFS10LevelMethod, EDeviceAxtMotionFS10LevelMethod_Count), nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS10_AlarmInputLevel, g_lpszParamAxtMotionFS10[EDeviceParameterAxtMotionFS10_AlarmInputLevel], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxtMotionFS10LevelMethod, EDeviceAxtMotionFS10LevelMethod_Count), nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS10_InpositionInputLevel, g_lpszParamAxtMotionFS10[EDeviceParameterAxtMotionFS10_InpositionInputLevel], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxtMotionFS10LevelMethod, EDeviceAxtMotionFS10LevelMethod_Count), nullptr, 0);

		bReturn = true;
	}
	while(false);

	return bReturn & __super::LoadSettings();
}

EFS10GetFunction CDeviceAxtMotionFS10::GetUnitPulse(double* pParam)
{
	EFS10GetFunction eReturn = EFS10GetFunction_UnknownError;

	do 
	{
		if(!pParam)
		{
			eReturn = EFS10GetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterAxtMotionFS10_UnitPulse));
		
		eReturn = EFS10GetFunction_OK;
	} 
	while(false);

	return eReturn;
}

EFS10SetFunction CDeviceAxtMotionFS10::SetUnitPulse(double dblParam)
{
	EFS10SetFunction eReturn = EFS10SetFunction_UnknownError;

	EDeviceParameterAxtMotionFS10 eSaveID = EDeviceParameterAxtMotionFS10_UnitPulse;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
			CFSset_moveunit_perpulse(_ttoi(GetSubUnitID()), dblParam);

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EFS10SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EFS10SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EFS10SetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxtMotionFS10[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EFS10GetFunction CDeviceAxtMotionFS10::GetStartSpeed(double* pParam)
{
	EFS10GetFunction eReturn = EFS10GetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EFS10GetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterAxtMotionFS10_StartSpeed));

		eReturn = EFS10GetFunction_OK;
	}
	while(false);

	return eReturn;
}

EFS10SetFunction CDeviceAxtMotionFS10::SetStartSpeed(double dblParam)
{
	EFS10SetFunction eReturn = EFS10SetFunction_UnknownError;

	EDeviceParameterAxtMotionFS10 eSaveID = EDeviceParameterAxtMotionFS10_StartSpeed;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
			CFSset_startstop_speed(_ttoi(GetSubUnitID()), dblParam);

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EFS10SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EFS10SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EFS10SetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxtMotionFS10[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EFS10GetFunction CDeviceAxtMotionFS10::GetMaxSpeed(double* pParam)
{
	EFS10GetFunction eReturn = EFS10GetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EFS10GetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterAxtMotionFS10_MaxSpeed));

		eReturn = EFS10GetFunction_OK;
	}
	while(false);

	return eReturn;
}

EFS10SetFunction CDeviceAxtMotionFS10::SetMaxSpeed(double dblParam)
{
	EFS10SetFunction eReturn = EFS10SetFunction_UnknownError;

	EDeviceParameterAxtMotionFS10 eSaveID = EDeviceParameterAxtMotionFS10_MaxSpeed;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
			CFSset_max_speed(_ttoi(GetSubUnitID()), dblParam);

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EFS10SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EFS10SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EFS10SetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxtMotionFS10[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EFS10GetFunction CDeviceAxtMotionFS10::GetPulseOutputMethod(EDeviceAxtMotionFS10PulseOutputMethod* pParam)
{
	EFS10GetFunction eReturn = EFS10GetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EFS10GetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxtMotionFS10PulseOutputMethod)_ttoi(GetParamValue(EDeviceParameterAxtMotionFS10_PulseOutputMethod));

		eReturn = EFS10GetFunction_OK;
	}
	while(false);

	return eReturn;
}

EFS10SetFunction CDeviceAxtMotionFS10::SetPulseOutputMethod(EDeviceAxtMotionFS10PulseOutputMethod eParam)
{
	EFS10SetFunction eReturn = EFS10SetFunction_UnknownError;

	EDeviceParameterAxtMotionFS10 eSaveID = EDeviceParameterAxtMotionFS10_PulseOutputMethod;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxtMotionFS10PulseOutputMethod_Count)
		{
			eReturn = EFS10SetFunction_NotFoundCommandError;
			break;
		}

		if(IsInitialized())
			CFSset_pulse_out_method(_ttoi(GetSubUnitID()), (int)eParam);

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EFS10SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EFS10SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EFS10SetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxtMotionFS10[eSaveID], g_lpszAxtMotionFS10PulseOutputMethod[nPreValue], g_lpszAxtMotionFS10PulseOutputMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EFS10GetFunction CDeviceAxtMotionFS10::GetEncoderMethod(EDeviceAxtMotionFS10EncoderMethod* pParam)
{
	EFS10GetFunction eReturn = EFS10GetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EFS10GetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxtMotionFS10EncoderMethod)_ttoi(GetParamValue(EDeviceParameterAxtMotionFS10_EncoderMethod));

		eReturn = EFS10GetFunction_OK;
	}
	while(false);

	return eReturn;
}

EFS10SetFunction CDeviceAxtMotionFS10::SetEncoderMethod(EDeviceAxtMotionFS10EncoderMethod eParam)
{
	EFS10SetFunction eReturn = EFS10SetFunction_UnknownError;

	EDeviceParameterAxtMotionFS10 eSaveID = EDeviceParameterAxtMotionFS10_EncoderMethod;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxtMotionFS10EncoderMethod_Count)
		{
			eReturn = EFS10SetFunction_NotFoundCommandError;
			break;
		}

		if(IsInitialized())
			CFSset_enc_input_method(_ttoi(GetSubUnitID()), (int)eParam);

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EFS10SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EFS10SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EFS10SetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxtMotionFS10[eSaveID], g_lpszAxtMotionFS10EncoderMethod[nPreValue], g_lpszAxtMotionFS10EncoderMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EFS10GetFunction CDeviceAxtMotionFS10::GetPLimitLevel(EDeviceAxtMotionFS10LevelMethod* pParam)
{
	EFS10GetFunction eReturn = EFS10GetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EFS10GetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxtMotionFS10LevelMethod)_ttoi(GetParamValue(EDeviceParameterAxtMotionFS10_PLimitLevel));

		eReturn = EFS10GetFunction_OK;
	}
	while(false);

	return eReturn;
}

EFS10SetFunction CDeviceAxtMotionFS10::SetPLimitLevel(EDeviceAxtMotionFS10LevelMethod eParam)
{
	EFS10SetFunction eReturn = EFS10SetFunction_UnknownError;

	EDeviceParameterAxtMotionFS10 eSaveID = EDeviceParameterAxtMotionFS10_PLimitLevel;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxtMotionFS10PulseOutputMethod_Count)
		{
			eReturn = EFS10SetFunction_NotFoundCommandError;
			break;
		}

		if(IsInitialized())
			CFSset_pend_limit_level(_ttoi(GetSubUnitID()), (int)eParam);

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EFS10SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EFS10SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EFS10SetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxtMotionFS10[eSaveID], g_lpszAxtMotionFS10LevelMethod[nPreValue], g_lpszAxtMotionFS10LevelMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EFS10GetFunction CDeviceAxtMotionFS10::GetNLimitLevel(EDeviceAxtMotionFS10LevelMethod* pParam)
{
	EFS10GetFunction eReturn = EFS10GetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EFS10GetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxtMotionFS10LevelMethod)_ttoi(GetParamValue(EDeviceParameterAxtMotionFS10_NLimitLevel));

		eReturn = EFS10GetFunction_OK;
	}
	while(false);

	return eReturn;
}

EFS10SetFunction CDeviceAxtMotionFS10::SetNLimitLevel(EDeviceAxtMotionFS10LevelMethod eParam)
{
	EFS10SetFunction eReturn = EFS10SetFunction_UnknownError;

	EDeviceParameterAxtMotionFS10 eSaveID = EDeviceParameterAxtMotionFS10_NLimitLevel;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxtMotionFS10PulseOutputMethod_Count)
		{
			eReturn = EFS10SetFunction_NotFoundCommandError;
			break;
		}

		if(IsInitialized())
			CFSset_nend_limit_level(_ttoi(GetSubUnitID()), (int)eParam);

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EFS10SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EFS10SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EFS10SetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxtMotionFS10[eSaveID], g_lpszAxtMotionFS10LevelMethod[nPreValue], g_lpszAxtMotionFS10LevelMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EFS10GetFunction CDeviceAxtMotionFS10::GetAlarmInputLevel(EDeviceAxtMotionFS10LevelMethod* pParam)
{
	EFS10GetFunction eReturn = EFS10GetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EFS10GetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxtMotionFS10LevelMethod)_ttoi(GetParamValue(EDeviceParameterAxtMotionFS10_AlarmInputLevel));

		eReturn = EFS10GetFunction_OK;
	}
	while(false);

	return eReturn;
}

EFS10SetFunction CDeviceAxtMotionFS10::SetAlarmInputLevel(EDeviceAxtMotionFS10LevelMethod eParam)
{
	EFS10SetFunction eReturn = EFS10SetFunction_UnknownError;

	EDeviceParameterAxtMotionFS10 eSaveID = EDeviceParameterAxtMotionFS10_AlarmInputLevel;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxtMotionFS10PulseOutputMethod_Count)
		{
			eReturn = EFS10SetFunction_NotFoundCommandError;
			break;
		}

		if(IsInitialized())
			CFSset_alarm_level(_ttoi(GetSubUnitID()), (int)eParam);

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EFS10SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EFS10SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EFS10SetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxtMotionFS10[eSaveID], g_lpszAxtMotionFS10LevelMethod[nPreValue], g_lpszAxtMotionFS10LevelMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EFS10GetFunction CDeviceAxtMotionFS10::GetInpositionInputLevel(EDeviceAxtMotionFS10LevelMethod* pParam)
{
	EFS10GetFunction eReturn = EFS10GetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EFS10GetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxtMotionFS10LevelMethod)_ttoi(GetParamValue(EDeviceParameterAxtMotionFS10_InpositionInputLevel));

		eReturn = EFS10GetFunction_OK;
	}
	while(false);

	return eReturn;
}

EFS10SetFunction CDeviceAxtMotionFS10::SetInpositionInputLevel(EDeviceAxtMotionFS10LevelMethod eParam)
{
	EFS10SetFunction eReturn = EFS10SetFunction_UnknownError;

	EDeviceParameterAxtMotionFS10 eSaveID = EDeviceParameterAxtMotionFS10_InpositionInputLevel;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxtMotionFS10PulseOutputMethod_Count)
		{
			eReturn = EFS10SetFunction_NotFoundCommandError;
			break;
		}

		if(IsInitialized())
			CFSset_inposition_enable(_ttoi(GetSubUnitID()), (int)eParam);

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EFS10SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EFS10SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EFS10SetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxtMotionFS10[eSaveID], g_lpszAxtMotionFS10LevelMethod[nPreValue], g_lpszAxtMotionFS10LevelMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceAxtMotionFS10::GetServoStatus()
{
	bool bReturn = false;

	do 
	{
		if(!IsInitialized())
			break;

		bReturn = CFSget_servo_enable(_ttoi(GetSubUnitID()));
	} 
	while(false);

	return bReturn;
}

bool CDeviceAxtMotionFS10::SetServoOn(bool bOn)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		bReturn = CFSset_servo_enable(_ttoi(GetSubUnitID()), bOn);

		if(!bReturn)
			break;

		m_bServo = bOn ? true : false;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxtMotionFS10::GetCommandPosition(double* pPos)
{
	bool bReturn = false;

	do 
	{
		if(!pPos)
			break;

		if(!IsInitialized())
			break;

		*pPos = CFSget_command_position(_ttoi(GetSubUnitID()));

		bReturn = true;
	} 
	while(false);

	return bReturn;
}

bool CDeviceAxtMotionFS10::SetCommandPositionClear()
{
	bool bReturn = false;

	do 
	{
		if(!IsInitialized())
			break;

		CFSset_command_position(_ttoi(GetSubUnitID()), 0.0f);

		bReturn = true;
	} 
	while(false);
	
	return bReturn;
}

bool CDeviceAxtMotionFS10::GetActualPosition(double * pPos)
{
	bool bReturn = false;

	do
	{
		if(!pPos)
			break;

		if(!IsInitialized())
			break;

		*pPos = CFSget_actual_position(_ttoi(GetSubUnitID()));

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxtMotionFS10::SetActualPositionClear()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		CFSset_actual_position(_ttoi(GetSubUnitID()), 0.0f);

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxtMotionFS10::GetAlarmStatus()
{
	bool bReturn = false;

	do 
	{
		if(!IsInitialized())
			break;

		bReturn = CFSget_mechanical_signal(_ttoi(GetSubUnitID()))  & 0x10;
	} 
	while(false);

	return bReturn;
}

bool CDeviceAxtMotionFS10::SetAlarmClear()
{
	return false;
}

bool CDeviceAxtMotionFS10::GetInposition()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		bReturn = CFSget_mechanical_signal(_ttoi(GetSubUnitID()))  & 0x20;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxtMotionFS10::GetLimitSensorN()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		bReturn = CFSget_mechanical_signal(_ttoi(GetSubUnitID()))  & 0x02;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxtMotionFS10::GetLimitSensorP()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		bReturn = CFSget_mechanical_signal(_ttoi(GetSubUnitID()))  & 0x01;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxtMotionFS10::GetHomeSensor()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		bReturn = CFSinput_bit_on(_ttoi(GetSubUnitID()), 0);
	}
	while(false);

	return bReturn;
}

bool CDeviceAxtMotionFS10::MovePosition(double dPos, double dVel, double dAcc, bool bSCurve)
{
	bool bReturn = false;

	do 
	{
		if(!IsInitialized())
			break;

		if(!bSCurve)
			bReturn = CFSstart_move(_ttoi(GetSubUnitID()), dPos, dVel, dAcc);
		else
			bReturn = CFSstart_s_move(_ttoi(GetSubUnitID()), dPos, dVel, dAcc);
	} 
	while(false);
	
	return bReturn;
}

bool CDeviceAxtMotionFS10::MoveDistance(double dDist, double dVel, double dAcc, bool bSCurve)
{
	bool bReturn = false;

	do 
	{
		if(!IsInitialized())
			break;

		if(!bSCurve)
			bReturn = CFSstart_r_move(_ttoi(GetSubUnitID()), dDist, dVel, dAcc);
		else
			bReturn = CFSstart_rs_move(_ttoi(GetSubUnitID()), dDist, dVel, dAcc);
	} 
	while(false);
	
	return bReturn;
}

bool CDeviceAxtMotionFS10::MoveVelocity(double dVel, double dAcc, bool bSCurve)
{
	return false;
}

bool CDeviceAxtMotionFS10::MoveJog(double dVel, double dAcl, BOOL bSCurve)
{
	bool bReturn = false;

	do 
	{
		if(!IsInitialized())
			break;

		if(!bSCurve)
			bReturn = CFSv_move(_ttoi(GetSubUnitID()), dVel, dAcl);
		else
			bReturn = CFSv_s_move(_ttoi(GetSubUnitID()), dVel, dAcl);
	} 
	while(false);
	
	return bReturn;
}

bool CDeviceAxtMotionFS10::StopJog()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		bReturn = CFSset_stop(_ttoi(GetSubUnitID()));
	}
	while(false);

	return bReturn;
}

bool CDeviceAxtMotionFS10::IsMotionDone()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		bReturn = CFSmotion_done(_ttoi(GetSubUnitID()));
	}
	while(false);

	return bReturn;
}

bool CDeviceAxtMotionFS10::MotorStop()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		bReturn = CFSset_stop(_ttoi(GetSubUnitID()));
	}
	while(false);

	return bReturn;
}

bool CDeviceAxtMotionFS10::MotorEStop()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		bReturn = CFSset_e_stop(_ttoi(GetSubUnitID()));
	}
	while(false);

	return bReturn;
}

bool CDeviceAxtMotionFS10::ChangeSpeed(double dSpeed)
{
	return false;
}

bool CDeviceAxtMotionFS10::WriteGenOutBit(int nBit, bool bOn)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(bOn)
			bReturn = CFSset_output_bit(_ttoi(GetSubUnitID()), nBit);
		else
			bReturn = CFSreset_output_bit(_ttoi(GetSubUnitID()), nBit);
	}
	while(false);

	return bReturn;
}

bool CDeviceAxtMotionFS10::ReadGenOutBit(int nBit)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		bReturn = CFSoutput_bit_on(_ttoi(GetSubUnitID()), nBit);
	}
	while(false);

	return bReturn;
}

bool CDeviceAxtMotionFS10::ReadGenInputBit(int nBit)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		bReturn = CFSinput_bit_on(_ttoi(GetSubUnitID()), nBit);
	}
	while(false);

	return bReturn;
}

bool CDeviceAxtMotionFS10::MoveToHome()
{
	return false;
}

EDeviceMotionHommingStatus CDeviceAxtMotionFS10::GetHommingStatus()
{
	return EDeviceMotionHommingStatus();
}

bool CDeviceAxtMotionFS10::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = true;

	bool bFoundID = true;

	do
	{
		switch(nParam)
		{
		case EDeviceParameterAxtMotionFS10_DeviceID:
			bReturn = !SetDeviceID(strValue);
			break;
		case EDeviceParameterAxtMotionFS10_SubUnitID:
			bReturn = !SetSubUnitID(strValue);
			break;
		case EDeviceParameterAxtMotionFS10_UnitPulse:
			bReturn = !SetUnitPulse(_ttof(strValue));
			break;
		case EDeviceParameterAxtMotionFS10_StartSpeed:
			bReturn = !SetStartSpeed(_ttof(strValue));
			break;
		case EDeviceParameterAxtMotionFS10_MaxSpeed:
			bReturn = !SetMaxSpeed(_ttof(strValue));
			break;
		case EDeviceParameterAxtMotionFS10_PulseOutputMethod:
			bReturn = !SetPulseOutputMethod((EDeviceAxtMotionFS10PulseOutputMethod)_ttoi(strValue));
			break;
		case EDeviceParameterAxtMotionFS10_EncoderMethod:
			bReturn = !SetEncoderMethod((EDeviceAxtMotionFS10EncoderMethod)_ttoi(strValue));
			break;
		case EDeviceParameterAxtMotionFS10_PLimitLevel:
			bReturn = !SetPLimitLevel((EDeviceAxtMotionFS10LevelMethod)_ttoi(strValue));
			break;
		case EDeviceParameterAxtMotionFS10_NLimitLevel:
			bReturn = !SetNLimitLevel((EDeviceAxtMotionFS10LevelMethod)_ttoi(strValue));
			break;
		case EDeviceParameterAxtMotionFS10_AlarmInputLevel:
			bReturn = !SetAlarmInputLevel((EDeviceAxtMotionFS10LevelMethod)_ttoi(strValue));
			break;
		case EDeviceParameterAxtMotionFS10_InpositionInputLevel:
			bReturn = !SetInpositionInputLevel((EDeviceAxtMotionFS10LevelMethod)_ttoi(strValue));
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

bool CDeviceAxtMotionFS10::DoesModuleExist()
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