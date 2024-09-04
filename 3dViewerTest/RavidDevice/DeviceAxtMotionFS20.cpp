#include "stdafx.h"

#include "DeviceAxtMotionFS20.h"

#ifdef USING_DEVICE

#include "DeviceAxtDio.h"
#include "DeviceAxtMotionFS10.h"

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
#include "../Libraries/Includes/AxtLib/AxtKeCamcFs20.h"
#include "../Libraries/Includes/AxtLib/AxtCAMCFS20.h"

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

IMPLEMENT_DYNAMIC(CDeviceAxtMotionFS20, CDeviceMotion)

BEGIN_MESSAGE_MAP(CDeviceAxtMotionFS20, CDeviceMotion)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static LPCTSTR g_lpszParamAxtMotionFS20[EDeviceParameterAxtMotionFS20_Count] =
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
	_T("Home attribute"),
	_T("Home steps"),
	_T("Step1 method"),
	_T("Step1 stop"),
	_T("Step1 direction"),
	_T("Step1 detect"),
	_T("Step1 velocity"),
	_T("Step1 accel"),
	_T("Step2 method"),
	_T("Step2 stop"),
	_T("Step2 Direction"),
	_T("Step2 detect"),
	_T("Step2 velocity"),
	_T("Step2 accel"),
	_T("Step3 method"),
	_T("Step3 stop"),
	_T("Step3 Direction"),
	_T("Step3 detect"),
	_T("Step3 velocity"),
	_T("Step3 accel"),
	_T("Step4 method"),
	_T("Step4 stop"),
	_T("Step4 Direction"),
	_T("Step4 detect"),
	_T("Step4 velocity"),
	_T("Step4 accel"),
};

static LPCTSTR g_lpszAxtMotionFS20PulseOutputMethod[EDeviceAxtMotionFS20PulseOutputMethod_Count] =
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

static LPCTSTR g_lpszAxtMotionFS20EncoderMethod[EDeviceAxtMotionFS20EncoderMethod_Count] =
{
	_T("UpDownMode"),
	_T("Sqr1Mode"),
	_T("Sqr2Mode"),
	_T("Sqr4Mode"),
};

static LPCTSTR g_lpszAxtMotionFS20LevelMethod[EDeviceAxtMotionFS20LevelMethod_Count] =
{
	_T("LOW"),
	_T("HIGH"),
};

static LPCTSTR g_lpszAxtMotionFS20DetectSignal[EDeviceAxtMotionFS20DetectSignal_Count] =
{
	_T("Pos.EndLimit FallingEdge"),
	_T("Neg.EndLimit FallingEdge"),
	_T("Pos.SlowdownLimit FallingEdge"),
	_T("Neg.SlowdownLimit FallingEdge"),
	_T("In0 Origin FallingEdge"),
	_T("In1 Zaxis FallingEdge"),
	_T("In2 General FallingEdge"),
	_T("In3 General FallingEdge"),
	_T("Pos.EndLimit RisingEdge"),
	_T("Neg.EndLimit RisingEdge"),
	_T("Pos.SlowdownLimit RisingEdge"),
	_T("Neg.SlowdownLimit RisingEdge"),
	_T("In0 Origin RisingEdge"),
	_T("In1 Zaxis RisingEdge"),
	_T("In2 General RisingEdge"),
	_T("In3 General RisingEdge"),
};

static LPCTSTR g_lpszAxtMotionFS20HomeSteps[EDeviceAxtMotionFS20HomeSteps_Count] =
{
	_T("1"),
	_T("2"),
	_T("3"),
	_T("4"),
};

static LPCTSTR g_lpszAxtMotionFS20StopMethod[EDeviceAxtMotionFS20StopMethod_Count] =
{
	_T("ReductionStop"),
	_T("EmergencyStop"),
};

static LPCTSTR g_lpszAxtMotionFS20DirectionMethod[EDeviceAxtMotionFS20DirectionMethod_Count] =
{
	_T("CW"),
	_T("CCW"),
};



CDeviceAxtMotionFS20::CDeviceAxtMotionFS20()
{
}

CDeviceAxtMotionFS20::~CDeviceAxtMotionFS20()
{
	Terminate();
}

EDeviceInitializeResult CDeviceAxtMotionFS20::Initialize()
{
	EDeviceInitializeResult eReturn = EDeviceInitializeResult_UnknownError;

	CString strMessage, strStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));
	
	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("AxtMotionFS20"));

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
			eReturn = EDeviceInitializeResult_NotFoundDeviceError;
			break;
		}

		int nShiftBit = 0;

		DWORD dwCycle = 0;

		bool bDeviceIntialized = true;

		if(!CFS20IsInitialized())
		{
			if(!InitializeCAMCFS20(true))
				bDeviceIntialized = false;
		}

		if(!bDeviceIntialized)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_FailedtoinitializeCAMCFS20module);
			eReturn = EDeviceInitializeResult_NotInitializeMotionError;
			break;
		}

		int nSubunitID = _ttoi(GetSubUnitID());

		INT16 nMaxAxis = CFS20get_numof_axes(nDeviceID);

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
		
		CFS20set_moveunit_perpulse(nSubunitID, dblUnitPulse);

		double dblStartSpeed = 0.;

		if(GetStartSpeed(&dblStartSpeed))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Start speed"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		CFS20set_startstop_speed(nSubunitID, dblStartSpeed);

		double dblMaxSpeed = 0.;

		if(GetMaxSpeed(&dblMaxSpeed))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Max speed"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(!CFS20set_max_speed(nSubunitID, dblMaxSpeed))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Max speed"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		EDeviceAxtMotionFS20PulseOutputMethod ePulseOutputMethod = EDeviceAxtMotionFS20PulseOutputMethod_Count;

		if(GetPulseOutputMethod(&ePulseOutputMethod))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Pulse output method"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(!CFS20set_pulse_out_method(nSubunitID, ePulseOutputMethod))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Pulse output method"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		EDeviceAxtMotionFS20EncoderMethod eEncoderMethod = EDeviceAxtMotionFS20EncoderMethod_Count;

		if(GetEncoderMethod(&eEncoderMethod))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Encoder method"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(!CFS20set_enc_input_method(nSubunitID, eEncoderMethod))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Encoder method"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		EDeviceAxtMotionFS20LevelMethod ePLimitLevel = EDeviceAxtMotionFS20LevelMethod_Count;

		if(GetPLimitLevel(&ePLimitLevel))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("P limit level"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}
		
		if(!CFS20set_pend_limit_level(nSubunitID, ePLimitLevel))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("P limit level"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		EDeviceAxtMotionFS20LevelMethod eNLimitLevel = EDeviceAxtMotionFS20LevelMethod_Count;

		if(GetNLimitLevel(&eNLimitLevel))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("N limit level"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(!CFS20set_nend_limit_level(nSubunitID, eNLimitLevel))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("N limit level"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		EDeviceAxtMotionFS20LevelMethod eInpositionInputLevel = EDeviceAxtMotionFS20LevelMethod_Count;

		if(GetInpositionInputLevel(&eInpositionInputLevel))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Inposition input level"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(!CFS20set_inposition_enable(nSubunitID, eInpositionInputLevel))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Inposition input level"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		EDeviceAxtMotionFS20LevelMethod eAlarmInputLevel = EDeviceAxtMotionFS20LevelMethod_Count;

		if(GetAlarmInputLevel(&eAlarmInputLevel))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Alarm input level"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(!CFS20set_alarm_level(nSubunitID, eAlarmInputLevel))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Alarm input level"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		CFS20set_pslow_limit_level(nSubunitID, 1);
		CFS20set_nslow_limit_level(nSubunitID, 1);

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

EDeviceTerminateResult CDeviceAxtMotionFS20::Terminate()
{
	EDeviceTerminateResult eReturn = EDeviceTerminateResult_UnknownError;

	CString strMessage;

	SetStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("AxtMotionFS20"));

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

				if(!pMotionFS10->IsInitialized())
					continue;

				bNeedTreminate = false;

				break;
			}

			CDeviceAxtMotionFS20* pMotionFS20 = dynamic_cast<CDeviceAxtMotionFS20*>(pDevicebase);

			if(pMotionFS20)
			{
				bMatchDevice = true;

				if(pMotionFS20 == this)
					continue;

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

bool CDeviceAxtMotionFS20::LoadSettings()
{
	bool bReturn = false;

	m_vctParameterFieldConfigurations.clear();

	do
	{
		AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS20_DeviceID, g_lpszParamAxtMotionFS20[EDeviceParameterAxtMotionFS20_DeviceID], _T("0"), EParameterFieldType_Edit, nullptr, _T("Board ID"));
		AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS20_SubUnitID, g_lpszParamAxtMotionFS20[EDeviceParameterAxtMotionFS20_SubUnitID], _T("0"), EParameterFieldType_Edit, nullptr, _T("Module ID"));
		AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS20_UnitPulse, g_lpszParamAxtMotionFS20[EDeviceParameterAxtMotionFS20_UnitPulse], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS20_StartSpeed, g_lpszParamAxtMotionFS20[EDeviceParameterAxtMotionFS20_StartSpeed], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS20_MaxSpeed, g_lpszParamAxtMotionFS20[EDeviceParameterAxtMotionFS20_MaxSpeed], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS20_PulseOutputMethod, g_lpszParamAxtMotionFS20[EDeviceParameterAxtMotionFS20_PulseOutputMethod], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxtMotionFS20PulseOutputMethod, EDeviceAxtMotionFS20PulseOutputMethod_Count), _T("ex) OneHighLowHigh = 1 pulse, PULSE(Active High), cw(DIR=Low)  / ccw(DIR=High)"), 0);
		AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS20_EncoderMethod, g_lpszParamAxtMotionFS20[EDeviceParameterAxtMotionFS20_EncoderMethod], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxtMotionFS20EncoderMethod, EDeviceAxtMotionFS20EncoderMethod_Count), _T("ex) Sqr1Mode   = 0x1    // 1Ã¼¹è"), 0);
		AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS20_PLimitLevel, g_lpszParamAxtMotionFS20[EDeviceParameterAxtMotionFS20_PLimitLevel], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxtMotionFS20LevelMethod, EDeviceAxtMotionFS20LevelMethod_Count), _T("+"), 0);
		AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS20_NLimitLevel, g_lpszParamAxtMotionFS20[EDeviceParameterAxtMotionFS20_NLimitLevel], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxtMotionFS20LevelMethod, EDeviceAxtMotionFS20LevelMethod_Count), _T("-"), 0);
		AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS20_AlarmInputLevel, g_lpszParamAxtMotionFS20[EDeviceParameterAxtMotionFS20_AlarmInputLevel], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxtMotionFS20LevelMethod, EDeviceAxtMotionFS20LevelMethod_Count), nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS20_InpositionInputLevel, g_lpszParamAxtMotionFS20[EDeviceParameterAxtMotionFS20_InpositionInputLevel], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxtMotionFS20LevelMethod, EDeviceAxtMotionFS20LevelMethod_Count), nullptr, 0);

		AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS20_HomeAttribute, g_lpszParamAxtMotionFS20[EDeviceParameterAxtMotionFS20_HomeAttribute], _T("0"), EParameterFieldType_None, nullptr, nullptr, 0);
		{
			AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS20_HomeSteps, g_lpszParamAxtMotionFS20[EDeviceParameterAxtMotionFS20_HomeSteps], _T("2"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxtMotionFS20HomeSteps, EDeviceAxtMotionFS20HomeSteps_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS20_Step1Method, g_lpszParamAxtMotionFS20[EDeviceParameterAxtMotionFS20_Step1Method], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);
			{
				AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS20_Step1Stop, g_lpszParamAxtMotionFS20[EDeviceParameterAxtMotionFS20_Step1Stop], _T("1"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxtMotionFS20StopMethod, EDeviceAxtMotionFS20StopMethod_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS20_Step1Direction, g_lpszParamAxtMotionFS20[EDeviceParameterAxtMotionFS20_Step1Direction], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxtMotionFS20DirectionMethod, EDeviceAxtMotionFS20DirectionMethod_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS20_Step1Detect, g_lpszParamAxtMotionFS20[EDeviceParameterAxtMotionFS20_Step1Detect], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxtMotionFS20DetectSignal, EDeviceAxtMotionFS20DetectSignal_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS20_Step1Velocity, g_lpszParamAxtMotionFS20[EDeviceParameterAxtMotionFS20_Step1Velocity], _T("100"), EParameterFieldType_Edit, nullptr, nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS20_Step1Accel, g_lpszParamAxtMotionFS20[EDeviceParameterAxtMotionFS20_Step1Accel], _T("400"), EParameterFieldType_Edit, nullptr, nullptr, 2);
			}
			AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS20_Step2Method, g_lpszParamAxtMotionFS20[EDeviceParameterAxtMotionFS20_Step2Method], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);
			{
				AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS20_Step2Stop, g_lpszParamAxtMotionFS20[EDeviceParameterAxtMotionFS20_Step2Stop], _T("1"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxtMotionFS20StopMethod, EDeviceAxtMotionFS20StopMethod_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS20_Step2Direction, g_lpszParamAxtMotionFS20[EDeviceParameterAxtMotionFS20_Step2Direction], _T("1"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxtMotionFS20DirectionMethod, EDeviceAxtMotionFS20DirectionMethod_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS20_Step2Detect, g_lpszParamAxtMotionFS20[EDeviceParameterAxtMotionFS20_Step2Detect], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxtMotionFS20DetectSignal, EDeviceAxtMotionFS20DetectSignal_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS20_Step2Velocity, g_lpszParamAxtMotionFS20[EDeviceParameterAxtMotionFS20_Step2Velocity], _T("20"), EParameterFieldType_Edit, nullptr, nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS20_Step2Accel, g_lpszParamAxtMotionFS20[EDeviceParameterAxtMotionFS20_Step2Accel], _T("80"), EParameterFieldType_Edit, nullptr, nullptr, 2);
			}
			AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS20_Step3Method, g_lpszParamAxtMotionFS20[EDeviceParameterAxtMotionFS20_Step3Method], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);
			{
				AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS20_Step3Stop, g_lpszParamAxtMotionFS20[EDeviceParameterAxtMotionFS20_Step3Stop], _T("1"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxtMotionFS20StopMethod, EDeviceAxtMotionFS20StopMethod_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS20_Step3Direction, g_lpszParamAxtMotionFS20[EDeviceParameterAxtMotionFS20_Step3Direction], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxtMotionFS20DirectionMethod, EDeviceAxtMotionFS20DirectionMethod_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS20_Step3Detect, g_lpszParamAxtMotionFS20[EDeviceParameterAxtMotionFS20_Step3Detect], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxtMotionFS20DetectSignal, EDeviceAxtMotionFS20DetectSignal_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS20_Step3Velocity, g_lpszParamAxtMotionFS20[EDeviceParameterAxtMotionFS20_Step3Velocity], _T("1"), EParameterFieldType_Edit, nullptr, nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS20_Step3Accel, g_lpszParamAxtMotionFS20[EDeviceParameterAxtMotionFS20_Step3Accel], _T("4"), EParameterFieldType_Edit, nullptr, nullptr, 2);
			}
			AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS20_Step4Method, g_lpszParamAxtMotionFS20[EDeviceParameterAxtMotionFS20_Step4Method], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);
			{
				AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS20_Step4Stop, g_lpszParamAxtMotionFS20[EDeviceParameterAxtMotionFS20_Step4Stop], _T("1"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxtMotionFS20StopMethod, EDeviceAxtMotionFS20StopMethod_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS20_Step4Direction, g_lpszParamAxtMotionFS20[EDeviceParameterAxtMotionFS20_Step4Direction], _T("1"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxtMotionFS20DirectionMethod, EDeviceAxtMotionFS20DirectionMethod_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS20_Step4Detect, g_lpszParamAxtMotionFS20[EDeviceParameterAxtMotionFS20_Step4Detect], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxtMotionFS20DetectSignal, EDeviceAxtMotionFS20DetectSignal_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS20_Step4Velocity, g_lpszParamAxtMotionFS20[EDeviceParameterAxtMotionFS20_Step4Velocity], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterAxtMotionFS20_Step4Accel, g_lpszParamAxtMotionFS20[EDeviceParameterAxtMotionFS20_Step4Accel], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
			}
		}		

		bReturn = true;
	}
	while(false);

	return bReturn & __super::LoadSettings();
}

EFS20GetFunction CDeviceAxtMotionFS20::GetUnitPulse(double* pParam)
{
	EFS20GetFunction eReturn = EFS20GetFunction_UnknownError;

	do 
	{
		if(!pParam)
		{
			eReturn = EFS20GetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterAxtMotionFS20_UnitPulse));
		
		eReturn = EFS20GetFunction_OK;
	} 
	while(false);

	return eReturn;
}

EFS20SetFunction CDeviceAxtMotionFS20::SetUnitPulse(double dblParam)
{
	EFS20SetFunction eReturn = EFS20SetFunction_UnknownError;

	EDeviceParameterAxtMotionFS20 eSaveID = EDeviceParameterAxtMotionFS20_UnitPulse;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
			CFS20set_moveunit_perpulse(_ttoi(GetSubUnitID()), dblParam);

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EFS20SetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxtMotionFS20[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EFS20GetFunction CDeviceAxtMotionFS20::GetStartSpeed(double* pParam)
{
	EFS20GetFunction eReturn = EFS20GetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EFS20GetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterAxtMotionFS20_StartSpeed));

		eReturn = EFS20GetFunction_OK;
	}
	while(false);

	return eReturn;
}

EFS20SetFunction CDeviceAxtMotionFS20::SetStartSpeed(double dblParam)
{
	EFS20SetFunction eReturn = EFS20SetFunction_UnknownError;

	EDeviceParameterAxtMotionFS20 eSaveID = EDeviceParameterAxtMotionFS20_StartSpeed;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
			CFS20set_startstop_speed(_ttoi(GetSubUnitID()), dblParam);

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EFS20SetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxtMotionFS20[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EFS20GetFunction CDeviceAxtMotionFS20::GetMaxSpeed(double* pParam)
{
	EFS20GetFunction eReturn = EFS20GetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EFS20GetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterAxtMotionFS20_MaxSpeed));

		eReturn = EFS20GetFunction_OK;
	}
	while(false);

	return eReturn;
}

EFS20SetFunction CDeviceAxtMotionFS20::SetMaxSpeed(double dblParam)
{
	EFS20SetFunction eReturn = EFS20SetFunction_UnknownError;

	EDeviceParameterAxtMotionFS20 eSaveID = EDeviceParameterAxtMotionFS20_MaxSpeed;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
			CFS20set_max_speed(_ttoi(GetSubUnitID()), dblParam);

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EFS20SetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxtMotionFS20[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EFS20GetFunction CDeviceAxtMotionFS20::GetPulseOutputMethod(EDeviceAxtMotionFS20PulseOutputMethod* pParam)
{
	EFS20GetFunction eReturn = EFS20GetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EFS20GetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxtMotionFS20PulseOutputMethod)_ttoi(GetParamValue(EDeviceParameterAxtMotionFS20_PulseOutputMethod));

		eReturn = EFS20GetFunction_OK;
	}
	while(false);

	return eReturn;
}

EFS20SetFunction CDeviceAxtMotionFS20::SetPulseOutputMethod(EDeviceAxtMotionFS20PulseOutputMethod eParam)
{
	EFS20SetFunction eReturn = EFS20SetFunction_UnknownError;

	EDeviceParameterAxtMotionFS20 eSaveID = EDeviceParameterAxtMotionFS20_PulseOutputMethod;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxtMotionFS20PulseOutputMethod_Count)
		{
			eReturn = EFS20SetFunction_NotFoundCommandError;
			break;
		}

		if(IsInitialized())
			CFS20set_pulse_out_method(_ttoi(GetSubUnitID()), (int)eParam);

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EFS20SetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxtMotionFS20[eSaveID], g_lpszAxtMotionFS20PulseOutputMethod[nPreValue], g_lpszAxtMotionFS20PulseOutputMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EFS20GetFunction CDeviceAxtMotionFS20::GetEncoderMethod(EDeviceAxtMotionFS20EncoderMethod* pParam)
{
	EFS20GetFunction eReturn = EFS20GetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EFS20GetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxtMotionFS20EncoderMethod)_ttoi(GetParamValue(EDeviceParameterAxtMotionFS20_EncoderMethod));

		eReturn = EFS20GetFunction_OK;
	}
	while(false);

	return eReturn;
}

EFS20SetFunction CDeviceAxtMotionFS20::SetEncoderMethod(EDeviceAxtMotionFS20EncoderMethod eParam)
{
	EFS20SetFunction eReturn = EFS20SetFunction_UnknownError;

	EDeviceParameterAxtMotionFS20 eSaveID = EDeviceParameterAxtMotionFS20_EncoderMethod;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxtMotionFS20EncoderMethod_Count)
		{
			eReturn = EFS20SetFunction_NotFoundCommandError;
			break;
		}

		if(IsInitialized())
			CFS20set_enc_input_method(_ttoi(GetSubUnitID()), (int)eParam);

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EFS20SetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxtMotionFS20[eSaveID], g_lpszAxtMotionFS20EncoderMethod[nPreValue], g_lpszAxtMotionFS20EncoderMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EFS20GetFunction CDeviceAxtMotionFS20::GetPLimitLevel(EDeviceAxtMotionFS20LevelMethod* pParam)
{
	EFS20GetFunction eReturn = EFS20GetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EFS20GetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxtMotionFS20LevelMethod)_ttoi(GetParamValue(EDeviceParameterAxtMotionFS20_PLimitLevel));

		eReturn = EFS20GetFunction_OK;
	}
	while(false);

	return eReturn;
}

EFS20SetFunction CDeviceAxtMotionFS20::SetPLimitLevel(EDeviceAxtMotionFS20LevelMethod eParam)
{
	EFS20SetFunction eReturn = EFS20SetFunction_UnknownError;

	EDeviceParameterAxtMotionFS20 eSaveID = EDeviceParameterAxtMotionFS20_PLimitLevel;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxtMotionFS20PulseOutputMethod_Count)
		{
			eReturn = EFS20SetFunction_NotFoundCommandError;
			break;
		}

		if(IsInitialized())
			CFS20set_pend_limit_level(_ttoi(GetSubUnitID()), (int)eParam);

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EFS20SetFunction_OK;
	}
	while(false);

	CString strMessage;
strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxtMotionFS20[eSaveID], g_lpszAxtMotionFS20LevelMethod[nPreValue], g_lpszAxtMotionFS20LevelMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EFS20GetFunction CDeviceAxtMotionFS20::GetNLimitLevel(EDeviceAxtMotionFS20LevelMethod* pParam)
{
	EFS20GetFunction eReturn = EFS20GetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EFS20GetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxtMotionFS20LevelMethod)_ttoi(GetParamValue(EDeviceParameterAxtMotionFS20_NLimitLevel));

		eReturn = EFS20GetFunction_OK;
	}
	while(false);

	return eReturn;
}

EFS20SetFunction CDeviceAxtMotionFS20::SetNLimitLevel(EDeviceAxtMotionFS20LevelMethod eParam)
{
	EFS20SetFunction eReturn = EFS20SetFunction_UnknownError;

	EDeviceParameterAxtMotionFS20 eSaveID = EDeviceParameterAxtMotionFS20_NLimitLevel;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxtMotionFS20PulseOutputMethod_Count)
		{
			eReturn = EFS20SetFunction_NotFoundCommandError;
			break;
		}

		if(IsInitialized())
			CFS20set_nend_limit_level(_ttoi(GetSubUnitID()), (int)eParam);

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EFS20SetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxtMotionFS20[eSaveID], g_lpszAxtMotionFS20LevelMethod[nPreValue], g_lpszAxtMotionFS20LevelMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EFS20GetFunction CDeviceAxtMotionFS20::GetAlarmInputLevel(EDeviceAxtMotionFS20LevelMethod* pParam)
{
	EFS20GetFunction eReturn = EFS20GetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EFS20GetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxtMotionFS20LevelMethod)_ttoi(GetParamValue(EDeviceParameterAxtMotionFS20_AlarmInputLevel));

		eReturn = EFS20GetFunction_OK;
	}
	while(false);

	return eReturn;
}

EFS20SetFunction CDeviceAxtMotionFS20::SetAlarmInputLevel(EDeviceAxtMotionFS20LevelMethod eParam)
{
	EFS20SetFunction eReturn = EFS20SetFunction_UnknownError;

	EDeviceParameterAxtMotionFS20 eSaveID = EDeviceParameterAxtMotionFS20_AlarmInputLevel;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxtMotionFS20PulseOutputMethod_Count)
		{
			eReturn = EFS20SetFunction_NotFoundCommandError;
			break;
		}

		if(IsInitialized())
			CFS20set_alarm_level(_ttoi(GetSubUnitID()), (int)eParam);

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EFS20SetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxtMotionFS20[eSaveID], g_lpszAxtMotionFS20LevelMethod[nPreValue], g_lpszAxtMotionFS20LevelMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EFS20GetFunction CDeviceAxtMotionFS20::GetInpositionInputLevel(EDeviceAxtMotionFS20LevelMethod* pParam)
{
	EFS20GetFunction eReturn = EFS20GetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EFS20GetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxtMotionFS20LevelMethod)_ttoi(GetParamValue(EDeviceParameterAxtMotionFS20_InpositionInputLevel));

		eReturn = EFS20GetFunction_OK;
	}
	while(false);

	return eReturn;
}

EFS20SetFunction CDeviceAxtMotionFS20::SetInpositionInputLevel(EDeviceAxtMotionFS20LevelMethod eParam)
{
	EFS20SetFunction eReturn = EFS20SetFunction_UnknownError;

	EDeviceParameterAxtMotionFS20 eSaveID = EDeviceParameterAxtMotionFS20_InpositionInputLevel;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxtMotionFS20PulseOutputMethod_Count)
		{
			eReturn = EFS20SetFunction_NotFoundCommandError;
			break;
		}

		if(IsInitialized())
			CFS20set_inposition_enable(_ttoi(GetSubUnitID()), (int)eParam);

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EFS20SetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxtMotionFS20[eSaveID], g_lpszAxtMotionFS20LevelMethod[nPreValue], g_lpszAxtMotionFS20LevelMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EFS20GetFunction CDeviceAxtMotionFS20::GetHomesteps(EDeviceAxtMotionFS20HomeSteps* pParam)
{
	EFS20GetFunction eReturn = EFS20GetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EFS20GetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxtMotionFS20HomeSteps)_ttoi(GetParamValue(EDeviceParameterAxtMotionFS20_HomeSteps));

		eReturn = EFS20GetFunction_OK;
	}
	while(false);

	return eReturn;
}

EFS20SetFunction CDeviceAxtMotionFS20::SetHomesteps(EDeviceAxtMotionFS20HomeSteps eParam)
{
	EFS20SetFunction eReturn = EFS20SetFunction_UnknownError;

	EDeviceParameterAxtMotionFS20 eSaveID = EDeviceParameterAxtMotionFS20_HomeSteps;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxtMotionFS20HomeSteps_Count)
		{
			eReturn = EFS20SetFunction_NotFoundCommandError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EFS20SetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxtMotionFS20[eSaveID], g_lpszAxtMotionFS20HomeSteps[nPreValue], g_lpszAxtMotionFS20HomeSteps[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EFS20GetFunction CDeviceAxtMotionFS20::GetStep1Stop(EDeviceAxtMotionFS20StopMethod* pParam)
{
	EFS20GetFunction eReturn = EFS20GetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EFS20GetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxtMotionFS20StopMethod)_ttoi(GetParamValue(EDeviceParameterAxtMotionFS20_Step1Stop));

		eReturn = EFS20GetFunction_OK;
	}
	while(false);

	return eReturn;
}

EFS20SetFunction CDeviceAxtMotionFS20::SetStep1Stop(EDeviceAxtMotionFS20StopMethod eParam)
{
	EFS20SetFunction eReturn = EFS20SetFunction_UnknownError;

	EDeviceParameterAxtMotionFS20 eSaveID = EDeviceParameterAxtMotionFS20_Step1Stop;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxtMotionFS20StopMethod_Count)
		{
			eReturn = EFS20SetFunction_NotFoundCommandError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EFS20SetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxtMotionFS20[eSaveID], g_lpszAxtMotionFS20StopMethod[nPreValue], g_lpszAxtMotionFS20StopMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EFS20GetFunction CDeviceAxtMotionFS20::GetStep1Direction(EDeviceAxtMotionFS20DirectionMethod* pParam)
{
	EFS20GetFunction eReturn = EFS20GetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EFS20GetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxtMotionFS20DirectionMethod)_ttoi(GetParamValue(EDeviceParameterAxtMotionFS20_Step1Direction));

		eReturn = EFS20GetFunction_OK;
	}
	while(false);

	return eReturn;
}

EFS20SetFunction CDeviceAxtMotionFS20::SetStep1Direction(EDeviceAxtMotionFS20DirectionMethod eParam)
{
	EFS20SetFunction eReturn = EFS20SetFunction_UnknownError;

	EDeviceParameterAxtMotionFS20 eSaveID = EDeviceParameterAxtMotionFS20_Step1Direction;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxtMotionFS20DirectionMethod_Count)
		{
			eReturn = EFS20SetFunction_NotFoundCommandError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EFS20SetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxtMotionFS20[eSaveID], g_lpszAxtMotionFS20DirectionMethod[nPreValue], g_lpszAxtMotionFS20DirectionMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EFS20GetFunction CDeviceAxtMotionFS20::GetStep1Detect(EDeviceAxtMotionFS20DetectSignal* pParam)
{
	EFS20GetFunction eReturn = EFS20GetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EFS20GetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxtMotionFS20DetectSignal)_ttoi(GetParamValue(EDeviceParameterAxtMotionFS20_Step1Detect));

		eReturn = EFS20GetFunction_OK;
	}
	while(false);

	return eReturn;
}

EFS20SetFunction CDeviceAxtMotionFS20::SetStep1Detect(EDeviceAxtMotionFS20DetectSignal eParam)
{
	EFS20SetFunction eReturn = EFS20SetFunction_UnknownError;

	EDeviceParameterAxtMotionFS20 eSaveID = EDeviceParameterAxtMotionFS20_Step1Detect;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxtMotionFS20DetectSignal_Count)
		{
			eReturn = EFS20SetFunction_NotFoundCommandError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EFS20SetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxtMotionFS20[eSaveID], g_lpszAxtMotionFS20DetectSignal[nPreValue], g_lpszAxtMotionFS20DetectSignal[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EFS20GetFunction CDeviceAxtMotionFS20::GetStep1Velocity(double* pParam)
{
	EFS20GetFunction eReturn = EFS20GetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EFS20GetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterAxtMotionFS20_Step1Velocity));

		eReturn = EFS20GetFunction_OK;
	}
	while(false);

	return eReturn;
}

EFS20SetFunction CDeviceAxtMotionFS20::SetStep1Velocity(double dblParam)
{
	EFS20SetFunction eReturn = EFS20SetFunction_UnknownError;

	EDeviceParameterAxtMotionFS20 eSaveID = EDeviceParameterAxtMotionFS20_Step1Velocity;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EFS20SetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxtMotionFS20[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EFS20GetFunction CDeviceAxtMotionFS20::GetStep1Accel(double* pParam)
{
	EFS20GetFunction eReturn = EFS20GetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EFS20GetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterAxtMotionFS20_Step1Accel));

		eReturn = EFS20GetFunction_OK;
	}
	while(false);

	return eReturn;
}

EFS20SetFunction CDeviceAxtMotionFS20::SetStep1Accel(double dblParam)
{
	EFS20SetFunction eReturn = EFS20SetFunction_UnknownError;

	EDeviceParameterAxtMotionFS20 eSaveID = EDeviceParameterAxtMotionFS20_Step1Accel;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EFS20SetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxtMotionFS20[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EFS20GetFunction CDeviceAxtMotionFS20::GetStep2Stop(EDeviceAxtMotionFS20StopMethod* pParam)
{
	EFS20GetFunction eReturn = EFS20GetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EFS20GetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxtMotionFS20StopMethod)_ttoi(GetParamValue(EDeviceParameterAxtMotionFS20_Step2Stop));

		eReturn = EFS20GetFunction_OK;
	}
	while(false);

	return eReturn;
}

EFS20SetFunction CDeviceAxtMotionFS20::SetStep2Stop(EDeviceAxtMotionFS20StopMethod eParam)
{
	EFS20SetFunction eReturn = EFS20SetFunction_UnknownError;

	EDeviceParameterAxtMotionFS20 eSaveID = EDeviceParameterAxtMotionFS20_Step2Stop;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxtMotionFS20StopMethod_Count)
		{
			eReturn = EFS20SetFunction_NotFoundCommandError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EFS20SetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxtMotionFS20[eSaveID], g_lpszAxtMotionFS20StopMethod[nPreValue], g_lpszAxtMotionFS20StopMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EFS20GetFunction CDeviceAxtMotionFS20::GetStep2Direction(EDeviceAxtMotionFS20DirectionMethod* pParam)
{
	EFS20GetFunction eReturn = EFS20GetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EFS20GetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxtMotionFS20DirectionMethod)_ttoi(GetParamValue(EDeviceParameterAxtMotionFS20_Step2Direction));

		eReturn = EFS20GetFunction_OK;
	}
	while(false);

	return eReturn;
}

EFS20SetFunction CDeviceAxtMotionFS20::SetStep2Direction(EDeviceAxtMotionFS20DirectionMethod eParam)
{
	EFS20SetFunction eReturn = EFS20SetFunction_UnknownError;

	EDeviceParameterAxtMotionFS20 eSaveID = EDeviceParameterAxtMotionFS20_Step2Direction;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxtMotionFS20DirectionMethod_Count)
		{
			eReturn = EFS20SetFunction_NotFoundCommandError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EFS20SetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszParamAxtMotionFS20[eSaveID], g_lpszAxtMotionFS20DirectionMethod[nPreValue], g_lpszAxtMotionFS20DirectionMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EFS20GetFunction CDeviceAxtMotionFS20::GetStep2Detect(EDeviceAxtMotionFS20DetectSignal* pParam)
{
	EFS20GetFunction eReturn = EFS20GetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EFS20GetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxtMotionFS20DetectSignal)_ttoi(GetParamValue(EDeviceParameterAxtMotionFS20_Step2Detect));

		eReturn = EFS20GetFunction_OK;
	}
	while(false);

	return eReturn;
}

EFS20SetFunction CDeviceAxtMotionFS20::SetStep2Detect(EDeviceAxtMotionFS20DetectSignal eParam)
{
	EFS20SetFunction eReturn = EFS20SetFunction_UnknownError;

	EDeviceParameterAxtMotionFS20 eSaveID = EDeviceParameterAxtMotionFS20_Step2Detect;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxtMotionFS20DetectSignal_Count)
		{
			eReturn = EFS20SetFunction_NotFoundCommandError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EFS20SetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxtMotionFS20[eSaveID], g_lpszAxtMotionFS20DetectSignal[nPreValue], g_lpszAxtMotionFS20DetectSignal[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EFS20GetFunction CDeviceAxtMotionFS20::GetStep2Velocity(double* pParam)
{
	EFS20GetFunction eReturn = EFS20GetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EFS20GetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterAxtMotionFS20_Step2Velocity));

		eReturn = EFS20GetFunction_OK;
	}
	while(false);

	return eReturn;
}

EFS20SetFunction CDeviceAxtMotionFS20::SetStep2Velocity(double dblParam)
{
	EFS20SetFunction eReturn = EFS20SetFunction_UnknownError;

	EDeviceParameterAxtMotionFS20 eSaveID = EDeviceParameterAxtMotionFS20_Step2Velocity;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EFS20SetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxtMotionFS20[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EFS20GetFunction CDeviceAxtMotionFS20::GetStep2Accel(double* pParam)
{
	EFS20GetFunction eReturn = EFS20GetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EFS20GetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterAxtMotionFS20_Step2Accel));

		eReturn = EFS20GetFunction_OK;
	}
	while(false);

	return eReturn;
}

EFS20SetFunction CDeviceAxtMotionFS20::SetStep2Accel(double dblParam)
{
	EFS20SetFunction eReturn = EFS20SetFunction_UnknownError;

	EDeviceParameterAxtMotionFS20 eSaveID = EDeviceParameterAxtMotionFS20_Step2Accel;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EFS20SetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxtMotionFS20[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EFS20GetFunction CDeviceAxtMotionFS20::GetStep3Stop(EDeviceAxtMotionFS20StopMethod* pParam)
{
	EFS20GetFunction eReturn = EFS20GetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EFS20GetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxtMotionFS20StopMethod)_ttoi(GetParamValue(EDeviceParameterAxtMotionFS20_Step3Stop));

		eReturn = EFS20GetFunction_OK;
	}
	while(false);

	return eReturn;
}

EFS20SetFunction CDeviceAxtMotionFS20::SetStep3Stop(EDeviceAxtMotionFS20StopMethod eParam)
{
	EFS20SetFunction eReturn = EFS20SetFunction_UnknownError;

	EDeviceParameterAxtMotionFS20 eSaveID = EDeviceParameterAxtMotionFS20_Step3Stop;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxtMotionFS20StopMethod_Count)
		{
			eReturn = EFS20SetFunction_NotFoundCommandError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EFS20SetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxtMotionFS20[eSaveID], g_lpszAxtMotionFS20StopMethod[nPreValue], g_lpszAxtMotionFS20StopMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EFS20GetFunction CDeviceAxtMotionFS20::GetStep3Direction(EDeviceAxtMotionFS20DirectionMethod* pParam)
{
	EFS20GetFunction eReturn = EFS20GetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EFS20GetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxtMotionFS20DirectionMethod)_ttoi(GetParamValue(EDeviceParameterAxtMotionFS20_Step3Direction));

		eReturn = EFS20GetFunction_OK;
	}
	while(false);

	return eReturn;
}

EFS20SetFunction CDeviceAxtMotionFS20::SetStep3Direction(EDeviceAxtMotionFS20DirectionMethod eParam)
{
	EFS20SetFunction eReturn = EFS20SetFunction_UnknownError;

	EDeviceParameterAxtMotionFS20 eSaveID = EDeviceParameterAxtMotionFS20_Step3Direction;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxtMotionFS20DirectionMethod_Count)
		{
			eReturn = EFS20SetFunction_NotFoundCommandError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EFS20SetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxtMotionFS20[eSaveID], g_lpszAxtMotionFS20DirectionMethod[nPreValue], g_lpszAxtMotionFS20DirectionMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EFS20GetFunction CDeviceAxtMotionFS20::GetStep3Detect(EDeviceAxtMotionFS20DetectSignal* pParam)
{
	EFS20GetFunction eReturn = EFS20GetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EFS20GetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxtMotionFS20DetectSignal)_ttoi(GetParamValue(EDeviceParameterAxtMotionFS20_Step3Detect));

		eReturn = EFS20GetFunction_OK;
	}
	while(false);

	return eReturn;
}

EFS20SetFunction CDeviceAxtMotionFS20::SetStep3Detect(EDeviceAxtMotionFS20DetectSignal eParam)
{
	EFS20SetFunction eReturn = EFS20SetFunction_UnknownError;

	EDeviceParameterAxtMotionFS20 eSaveID = EDeviceParameterAxtMotionFS20_Step3Detect;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxtMotionFS20DetectSignal_Count)
		{
			eReturn = EFS20SetFunction_NotFoundCommandError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EFS20SetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxtMotionFS20[eSaveID], g_lpszAxtMotionFS20DetectSignal[nPreValue], g_lpszAxtMotionFS20DetectSignal[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EFS20GetFunction CDeviceAxtMotionFS20::GetStep3Velocity(double* pParam)
{
	EFS20GetFunction eReturn = EFS20GetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EFS20GetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterAxtMotionFS20_Step3Velocity));

		eReturn = EFS20GetFunction_OK;
	}
	while(false);

	return eReturn;
}

EFS20SetFunction CDeviceAxtMotionFS20::SetStep3Velocity(double dblParam)
{
	EFS20SetFunction eReturn = EFS20SetFunction_UnknownError;

	EDeviceParameterAxtMotionFS20 eSaveID = EDeviceParameterAxtMotionFS20_Step3Velocity;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EFS20SetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxtMotionFS20[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EFS20GetFunction CDeviceAxtMotionFS20::GetStep3Accel(double* pParam)
{
	EFS20GetFunction eReturn = EFS20GetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EFS20GetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterAxtMotionFS20_Step3Accel));

		eReturn = EFS20GetFunction_OK;
	}
	while(false);

	return eReturn;
}

EFS20SetFunction CDeviceAxtMotionFS20::SetStep3Accel(double dblParam)
{
	EFS20SetFunction eReturn = EFS20SetFunction_UnknownError;

	EDeviceParameterAxtMotionFS20 eSaveID = EDeviceParameterAxtMotionFS20_Step3Accel;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EFS20SetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxtMotionFS20[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EFS20GetFunction CDeviceAxtMotionFS20::GetStep4Stop(EDeviceAxtMotionFS20StopMethod* pParam)
{
	EFS20GetFunction eReturn = EFS20GetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EFS20GetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxtMotionFS20StopMethod)_ttoi(GetParamValue(EDeviceParameterAxtMotionFS20_Step4Stop));

		eReturn = EFS20GetFunction_OK;
	}
	while(false);

	return eReturn;
}

EFS20SetFunction CDeviceAxtMotionFS20::SetStep4Stop(EDeviceAxtMotionFS20StopMethod eParam)
{
	EFS20SetFunction eReturn = EFS20SetFunction_UnknownError;

	EDeviceParameterAxtMotionFS20 eSaveID = EDeviceParameterAxtMotionFS20_Step4Stop;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxtMotionFS20StopMethod_Count)
		{
			eReturn = EFS20SetFunction_NotFoundCommandError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EFS20SetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxtMotionFS20[eSaveID], g_lpszAxtMotionFS20StopMethod[nPreValue], g_lpszAxtMotionFS20StopMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EFS20GetFunction CDeviceAxtMotionFS20::GetStep4Direction(EDeviceAxtMotionFS20DirectionMethod* pParam)
{
	EFS20GetFunction eReturn = EFS20GetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EFS20GetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxtMotionFS20DirectionMethod)_ttoi(GetParamValue(EDeviceParameterAxtMotionFS20_Step4Direction));

		eReturn = EFS20GetFunction_OK;
	}
	while(false);

	return eReturn;
}

EFS20SetFunction CDeviceAxtMotionFS20::SetStep4Direction(EDeviceAxtMotionFS20DirectionMethod eParam)
{
	EFS20SetFunction eReturn = EFS20SetFunction_UnknownError;

	EDeviceParameterAxtMotionFS20 eSaveID = EDeviceParameterAxtMotionFS20_Step4Direction;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxtMotionFS20DirectionMethod_Count)
		{
			eReturn = EFS20SetFunction_NotFoundCommandError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EFS20SetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxtMotionFS20[eSaveID], g_lpszAxtMotionFS20DirectionMethod[nPreValue], g_lpszAxtMotionFS20DirectionMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EFS20GetFunction CDeviceAxtMotionFS20::GetStep4Detect(EDeviceAxtMotionFS20DetectSignal* pParam)
{
	EFS20GetFunction eReturn = EFS20GetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EFS20GetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxtMotionFS20DetectSignal)_ttoi(GetParamValue(EDeviceParameterAxtMotionFS20_Step4Detect));

		eReturn = EFS20GetFunction_OK;
	}
	while(false);

	return eReturn;
}

EFS20SetFunction CDeviceAxtMotionFS20::SetStep4Detect(EDeviceAxtMotionFS20DetectSignal eParam)
{
	EFS20SetFunction eReturn = EFS20SetFunction_UnknownError;

	EDeviceParameterAxtMotionFS20 eSaveID = EDeviceParameterAxtMotionFS20_Step4Detect;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxtMotionFS20DetectSignal_Count)
		{
			eReturn = EFS20SetFunction_NotFoundCommandError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EFS20SetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxtMotionFS20[eSaveID], g_lpszAxtMotionFS20DetectSignal[nPreValue], g_lpszAxtMotionFS20DetectSignal[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EFS20GetFunction CDeviceAxtMotionFS20::GetStep4Velocity(double* pParam)
{
	EFS20GetFunction eReturn = EFS20GetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EFS20GetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterAxtMotionFS20_Step4Velocity));

		eReturn = EFS20GetFunction_OK;
	}
	while(false);

	return eReturn;
}

EFS20SetFunction CDeviceAxtMotionFS20::SetStep4Velocity(double dblParam)
{
	EFS20SetFunction eReturn = EFS20SetFunction_UnknownError;

	EDeviceParameterAxtMotionFS20 eSaveID = EDeviceParameterAxtMotionFS20_Step4Velocity;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EFS20SetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxtMotionFS20[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EFS20GetFunction CDeviceAxtMotionFS20::GetStep4Accel(double* pParam)
{
	EFS20GetFunction eReturn = EFS20GetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EFS20GetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterAxtMotionFS20_Step4Accel));

		eReturn = EFS20GetFunction_OK;
	}
	while(false);

	return eReturn;
}

EFS20SetFunction CDeviceAxtMotionFS20::SetStep4Accel(double dblParam)
{
	EFS20SetFunction eReturn = EFS20SetFunction_UnknownError;

	EDeviceParameterAxtMotionFS20 eSaveID = EDeviceParameterAxtMotionFS20_Step4Accel;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EFS20SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EFS20SetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxtMotionFS20[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceAxtMotionFS20::GetServoStatus()
{
	bool bReturn = false;

	do 
	{
		if(!IsInitialized())
			break;

		bReturn = CFS20get_servo_enable(_ttoi(GetSubUnitID()));
	} 
	while(false);

	return bReturn;
}

bool CDeviceAxtMotionFS20::SetServoOn(bool bOn)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		bReturn = CFS20set_servo_enable(_ttoi(GetSubUnitID()), bOn);

		if(!bReturn)
			break;

		m_bServo = bOn ? true : false;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxtMotionFS20::GetCommandPosition(double* pPos)
{
	bool bReturn = false;

	do 
	{
		if(!pPos)
			break;

		if(!IsInitialized())
			break;

		*pPos = CFS20get_command_position(_ttoi(GetSubUnitID()));

		bReturn = true;
	} 
	while(false);

	return bReturn;
}

bool CDeviceAxtMotionFS20::SetCommandPositionClear()
{
	bool bReturn = false;

	do 
	{
		if(!IsInitialized())
			break;

		CFS20set_command_position(_ttoi(GetSubUnitID()), 0.0f);

		bReturn = true;
	} 
	while(false);
	
	return bReturn;
}

bool CDeviceAxtMotionFS20::GetActualPosition(double* pPos)
{
	bool bReturn = false;

	do
	{
		if(!pPos)
			break;

		if(!IsInitialized())
			break;

		*pPos = CFS20get_actual_position(_ttoi(GetSubUnitID()));

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxtMotionFS20::SetActualPositionClear()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		CFS20set_actual_position(_ttoi(GetSubUnitID()), 0.0f);

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxtMotionFS20::GetAlarmStatus()
{
	bool bReturn = false;

	do 
	{
		if(!IsInitialized())
			break;
		
		bReturn = CFS20get_mechanical_signal(_ttoi(GetSubUnitID()))  & 0x10;
	} 
	while(false);

	return bReturn;
}

bool CDeviceAxtMotionFS20::SetAlarmClear()
{
	bool bReturn = false;

	do 
	{
		if(!IsInitialized())
			break;

		if(!CFS20reset_output_bit(_ttoi(GetSubUnitID()), 1))
			break;
		
		if(!CFS20set_output_bit(_ttoi(GetSubUnitID()), 1))
			break;

		bReturn = true;
	} 
	while(false);

	return bReturn;
}

bool CDeviceAxtMotionFS20::GetInposition()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		bReturn = CFS20get_mechanical_signal(_ttoi(GetSubUnitID()))  & 0x20;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxtMotionFS20::GetLimitSensorN()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		bReturn = CFS20get_mechanical_signal(_ttoi(GetSubUnitID()))  & 0x02;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxtMotionFS20::GetLimitSensorP()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		bReturn = CFS20get_mechanical_signal(_ttoi(GetSubUnitID()))  & 0x01;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxtMotionFS20::GetHomeSensor()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		bReturn = CFS20input_bit_on(_ttoi(GetSubUnitID()), 0);
	}
	while(false);

	return bReturn;
}

bool CDeviceAxtMotionFS20::MovePosition(double dPos, double dVel, double dAcc, bool bSCurve)
{
	bool bReturn = false;

	do 
	{
		if(!IsInitialized())
			break;

		if(!bSCurve)
			bReturn = CFS20start_move(_ttoi(GetSubUnitID()), dPos, dVel, dAcc);
		else
			bReturn = CFS20start_s_move(_ttoi(GetSubUnitID()), dPos, dVel, dAcc);
	} 
	while(false);
	
	return bReturn;
}

bool CDeviceAxtMotionFS20::MoveDistance(double dDist, double dVel, double dAcc, bool bSCurve)
{
	bool bReturn = false;

	do 
	{
		if(!IsInitialized())
			break;

		if(!bSCurve)
			bReturn = CFS20start_r_move(_ttoi(GetSubUnitID()), dDist, dVel, dAcc);
		else
			bReturn = CFS20start_rs_move(_ttoi(GetSubUnitID()), dDist, dVel, dAcc);
	} 
	while(false);
	
	return bReturn;
}

bool CDeviceAxtMotionFS20::MoveVelocity(double dVel, double dAcc, bool bSCurve)
{
	return false;
}

bool CDeviceAxtMotionFS20::MoveJog(double dVel, double dAcl, BOOL bSCurve)
{
	bool bReturn = false;

	do 
	{
		if(!IsInitialized())
			break;

		if(!bSCurve)
			bReturn = CFS20v_move(_ttoi(GetSubUnitID()), dVel, dAcl);
		else
			bReturn = CFS20v_s_move(_ttoi(GetSubUnitID()), dVel, dAcl);
	} 
	while(false);
	
	return bReturn;
}

bool CDeviceAxtMotionFS20::StopJog()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		bReturn = CFS20set_stop(_ttoi(GetSubUnitID()));
	}
	while(false);

	return bReturn;
}

bool CDeviceAxtMotionFS20::IsMotionDone()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		bReturn = CFS20motion_done(_ttoi(GetSubUnitID()));
	}
	while(false);

	return bReturn;
}

bool CDeviceAxtMotionFS20::MotorStop()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		bReturn = CFS20set_stop(_ttoi(GetSubUnitID()));
	}
	while(false);

	return bReturn;
}

bool CDeviceAxtMotionFS20::MotorEStop()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		bReturn = CFS20set_e_stop(_ttoi(GetSubUnitID()));
	}
	while(false);

	return bReturn;
}

bool CDeviceAxtMotionFS20::ChangeSpeed(double dSpeed)
{
	return false;
}

bool CDeviceAxtMotionFS20::WriteGenOutBit(int nBit, bool bOn)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(bOn)
			bReturn = CFS20set_output_bit(_ttoi(GetSubUnitID()), nBit);
		else
			bReturn = CFS20reset_output_bit(_ttoi(GetSubUnitID()), nBit);
	}
	while(false);

	return bReturn;
}

bool CDeviceAxtMotionFS20::ReadGenOutBit(int nBit)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		bReturn = CFS20output_bit_on(_ttoi(GetSubUnitID()), nBit);
	}
	while(false);

	return bReturn;
}

bool CDeviceAxtMotionFS20::ReadGenInputBit(int nBit)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		bReturn = CFS20input_bit_on(_ttoi(GetSubUnitID()), nBit);
	}
	while(false);

	return bReturn;
}

bool CDeviceAxtMotionFS20::MoveToHome()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;
		
		int nSubunitID = _ttoi(GetSubUnitID());

		if(!CFS20get_home_done(nSubunitID))
			break;
		
		EDeviceAxtMotionFS20HomeSteps eHomeStep = EDeviceAxtMotionFS20HomeSteps_Count;

		if(GetHomesteps(&eHomeStep))
			break;

		switch(eHomeStep)
		{
		case EDeviceAxtMotionFS20HomeSteps_1:
			{
				UINT8 pMethods[1] = { 1 };
				double pVelocities[1] = { 0. };
				double pAccel[1] = { 0. };

				EDeviceAxtMotionFS20StopMethod eStopMethod = EDeviceAxtMotionFS20StopMethod_Count;
				EDeviceAxtMotionFS20DirectionMethod eDirMethod = EDeviceAxtMotionFS20DirectionMethod_Count;
				EDeviceAxtMotionFS20DetectSignal eDetectMethod = EDeviceAxtMotionFS20DetectSignal_Count;

				if(GetStep1Stop(&eStopMethod))
					break;

				if(GetStep1Direction(&eDirMethod))
					break;

				if(GetStep1Detect(&eDetectMethod))
					break;

				*(pMethods) |= (eStopMethod << 2) | (eDirMethod << 3) | (eDetectMethod << 4);

				if(CFS20input_bit_on(nSubunitID, 0))
					*(pMethods) = 0b00000000;

				if(GetStep1Velocity(pVelocities))
					break;

				if(GetStep1Accel(pAccel))
					break;

				CFS20set_home_method(nSubunitID, 1, pMethods);

				CFS20set_max_speed(nSubunitID, *(pVelocities));

				CFS20set_home_velocity(nSubunitID, 1, pVelocities);

				CFS20set_home_acceleration(nSubunitID, 1, pAccel);

			}
			break;
		case EDeviceAxtMotionFS20HomeSteps_2:
			{
				UINT8 pMethods[2] = { 1, 1 };
				double pVelocities[2] = { 0., 0. };
				double pAccel[2] = { 0., 0. };

				EDeviceAxtMotionFS20StopMethod eStopMethod = EDeviceAxtMotionFS20StopMethod_Count;
				EDeviceAxtMotionFS20DirectionMethod eDirMethod = EDeviceAxtMotionFS20DirectionMethod_Count;
				EDeviceAxtMotionFS20DetectSignal eDetectMethod = EDeviceAxtMotionFS20DetectSignal_Count;

				if(GetStep1Stop(&eStopMethod))
					break;

				if(GetStep1Direction(&eDirMethod))
					break;

				if(GetStep1Detect(&eDetectMethod))
					break;

				*(pMethods) |= (eStopMethod << 2) | (eDirMethod << 3) | (eDetectMethod << 4);

				eStopMethod = EDeviceAxtMotionFS20StopMethod_Count;
				eDirMethod = EDeviceAxtMotionFS20DirectionMethod_Count;
				eDetectMethod = EDeviceAxtMotionFS20DetectSignal_Count;

				if(GetStep2Stop(&eStopMethod))
					break;

				if(GetStep2Direction(&eDirMethod))
					break;

				if(GetStep2Detect(&eDetectMethod))
					break;

				*(pMethods + 1) |= (eStopMethod << 2) | (eDirMethod << 3) | (eDetectMethod << 4);
				
				if(CFS20input_bit_on(nSubunitID, 0))
					*(pMethods) = 0b00000000;

				if(GetStep1Velocity(pVelocities))
					break;

				if(GetStep2Velocity(pVelocities + 1))
					break;

				if(GetStep1Accel(pAccel))
					break;

				if(GetStep2Accel(pAccel + 1))
					break;
				
				CFS20set_home_method(nSubunitID, 2, pMethods);

				CFS20set_max_speed(nSubunitID, *(pVelocities));

				CFS20set_home_velocity(nSubunitID, 2, pVelocities);

				CFS20set_home_acceleration(nSubunitID, 2, pAccel);
			}
			break;
		case EDeviceAxtMotionFS20HomeSteps_3:
			{
				UINT8 pMethods[3] = { 1, 1, 1 };
				double pVelocities[3] = { 0., 0., 0. };
				double pAccel[3] = { 0., 0., 0. };

				EDeviceAxtMotionFS20StopMethod eStopMethod = EDeviceAxtMotionFS20StopMethod_Count;
				EDeviceAxtMotionFS20DirectionMethod eDirMethod = EDeviceAxtMotionFS20DirectionMethod_Count;
				EDeviceAxtMotionFS20DetectSignal eDetectMethod = EDeviceAxtMotionFS20DetectSignal_Count;

				if(GetStep1Stop(&eStopMethod))
					break;

				if(GetStep1Direction(&eDirMethod))
					break;

				if(GetStep1Detect(&eDetectMethod))
					break;

				*(pMethods) |= (eStopMethod << 2) | (eDirMethod << 3) | (eDetectMethod << 4);

				eStopMethod = EDeviceAxtMotionFS20StopMethod_Count;
				eDirMethod = EDeviceAxtMotionFS20DirectionMethod_Count;
				eDetectMethod = EDeviceAxtMotionFS20DetectSignal_Count;

				if(GetStep2Stop(&eStopMethod))
					break;

				if(GetStep2Direction(&eDirMethod))
					break;

				if(GetStep2Detect(&eDetectMethod))
					break;

				*(pMethods + 1) |= (eStopMethod << 2) | (eDirMethod << 3) | (eDetectMethod << 4);

				eStopMethod = EDeviceAxtMotionFS20StopMethod_Count;
				eDirMethod = EDeviceAxtMotionFS20DirectionMethod_Count;
				eDetectMethod = EDeviceAxtMotionFS20DetectSignal_Count;

				if(GetStep3Stop(&eStopMethod))
					break;

				if(GetStep3Direction(&eDirMethod))
					break;

				if(GetStep3Detect(&eDetectMethod))
					break;

				*(pMethods + 2) |= (eStopMethod << 2) | (eDirMethod << 3) | (eDetectMethod << 4);

				if(CFS20input_bit_on(nSubunitID, 0))
					*(pMethods) = 0b00000000;


				if(GetStep1Velocity(pVelocities))
					break;

				if(GetStep2Velocity(pVelocities + 1))
					break;

				if(GetStep3Velocity(pVelocities + 2))
					break;

				if(GetStep1Accel(pAccel))
					break;

				if(GetStep2Accel(pAccel + 1))
					break;

				if(GetStep3Accel(pAccel + 2))
					break;


				CFS20set_home_method(nSubunitID, 3, pMethods);

				CFS20set_max_speed(nSubunitID, *(pVelocities));

				CFS20set_home_velocity(nSubunitID, 3, pVelocities);

				CFS20set_home_acceleration(nSubunitID, 3, pAccel);
			}
			break;
		case EDeviceAxtMotionFS20HomeSteps_4:
			{
				UINT8 pMethods[4] = { 1, 1, 1, 1 };
				double pVelocities[4] = { 0., 0., 0., 0. };
				double pAccel[4] = { 0., 0., 0., 0. };

				EDeviceAxtMotionFS20StopMethod eStopMethod = EDeviceAxtMotionFS20StopMethod_Count;
				EDeviceAxtMotionFS20DirectionMethod eDirMethod = EDeviceAxtMotionFS20DirectionMethod_Count;
				EDeviceAxtMotionFS20DetectSignal eDetectMethod = EDeviceAxtMotionFS20DetectSignal_Count;

				if(GetStep1Stop(&eStopMethod))
					break;

				if(GetStep1Direction(&eDirMethod))
					break;

				if(GetStep1Detect(&eDetectMethod))
					break;

				*(pMethods) |= (eStopMethod << 2) | (eDirMethod << 3) | (eDetectMethod << 4);

				eStopMethod = EDeviceAxtMotionFS20StopMethod_Count;
				eDirMethod = EDeviceAxtMotionFS20DirectionMethod_Count;
				eDetectMethod = EDeviceAxtMotionFS20DetectSignal_Count;

				if(GetStep2Stop(&eStopMethod))
					break;

				if(GetStep2Direction(&eDirMethod))
					break;

				if(GetStep2Detect(&eDetectMethod))
					break;

				*(pMethods + 1) |= (eStopMethod << 2) | (eDirMethod << 3) | (eDetectMethod << 4);

				eStopMethod = EDeviceAxtMotionFS20StopMethod_Count;
				eDirMethod = EDeviceAxtMotionFS20DirectionMethod_Count;
				eDetectMethod = EDeviceAxtMotionFS20DetectSignal_Count;

				if(GetStep3Stop(&eStopMethod))
					break;

				if(GetStep3Direction(&eDirMethod))
					break;

				if(GetStep3Detect(&eDetectMethod))
					break;

				*(pMethods + 2) |= (eStopMethod << 2) | (eDirMethod << 3) | (eDetectMethod << 4);

				eStopMethod = EDeviceAxtMotionFS20StopMethod_Count;
				eDirMethod = EDeviceAxtMotionFS20DirectionMethod_Count;
				eDetectMethod = EDeviceAxtMotionFS20DetectSignal_Count;

				if(GetStep4Stop(&eStopMethod))
					break;

				if(GetStep4Direction(&eDirMethod))
					break;

				if(GetStep4Detect(&eDetectMethod))
					break;

				*(pMethods + 3) |= (eStopMethod << 2) | (eDirMethod << 3) | (eDetectMethod << 4);

				
				if(CFS20input_bit_on(nSubunitID, 0))
					*(pMethods) = 0b00000000;


				if(GetStep1Velocity(pVelocities))
					break;

				if(GetStep2Velocity(pVelocities + 1))
					break;

				if(GetStep3Velocity(pVelocities + 2))
					break;

				if(GetStep4Velocity(pVelocities + 3))
					break;

				if(GetStep1Accel(pAccel))
					break;

				if(GetStep2Accel(pAccel + 1))
					break;

				if(GetStep3Accel(pAccel + 2))
					break;

				if(GetStep4Accel(pAccel + 3))
					break;
						

				CFS20set_home_method(nSubunitID, 4, pMethods);

				CFS20set_max_speed(nSubunitID, *(pVelocities));

				CFS20set_home_velocity(nSubunitID, 4, pVelocities);

				CFS20set_home_acceleration(nSubunitID, 4, pAccel);
			}
			break;
		default:
			break;
		}
		
		if(!CFS20home_search(nSubunitID))
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

EDeviceMotionHommingStatus CDeviceAxtMotionFS20::GetHommingStatus()
{
	return EDeviceMotionHommingStatus();
}

bool CDeviceAxtMotionFS20::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = true;

	bool bFoundID = true;

	do
	{
		switch(nParam)
		{
		case EDeviceParameterAxtMotionFS20_DeviceID:
			bReturn = !SetDeviceID(strValue);
			break;
		case EDeviceParameterAxtMotionFS20_SubUnitID:
			bReturn = !SetSubUnitID(strValue);
			break;
		case EDeviceParameterAxtMotionFS20_UnitPulse:
			bReturn = !SetUnitPulse(_ttof(strValue));
			break;
		case EDeviceParameterAxtMotionFS20_StartSpeed:
			bReturn = !SetStartSpeed(_ttof(strValue));
			break;
		case EDeviceParameterAxtMotionFS20_MaxSpeed:
			bReturn = !SetMaxSpeed(_ttof(strValue));
			break;
		case EDeviceParameterAxtMotionFS20_PulseOutputMethod:
			bReturn = !SetPulseOutputMethod((EDeviceAxtMotionFS20PulseOutputMethod)_ttoi(strValue));
			break;
		case EDeviceParameterAxtMotionFS20_EncoderMethod:
			bReturn = !SetEncoderMethod((EDeviceAxtMotionFS20EncoderMethod)_ttoi(strValue));
			break;
		case EDeviceParameterAxtMotionFS20_PLimitLevel:
			bReturn = !SetPLimitLevel((EDeviceAxtMotionFS20LevelMethod)_ttoi(strValue));
			break;
		case EDeviceParameterAxtMotionFS20_NLimitLevel:
			bReturn = !SetNLimitLevel((EDeviceAxtMotionFS20LevelMethod)_ttoi(strValue));
			break;
		case EDeviceParameterAxtMotionFS20_AlarmInputLevel:
			bReturn = !SetAlarmInputLevel((EDeviceAxtMotionFS20LevelMethod)_ttoi(strValue));
			break;
		case EDeviceParameterAxtMotionFS20_InpositionInputLevel:
			bReturn = !SetInpositionInputLevel((EDeviceAxtMotionFS20LevelMethod)_ttoi(strValue));
			break;
		case EDeviceParameterAxtMotionFS20_HomeSteps:
			bReturn = !SetHomesteps((EDeviceAxtMotionFS20HomeSteps)_ttoi(strValue));
			break;
		case EDeviceParameterAxtMotionFS20_Step1Stop:
			bReturn = !SetStep1Stop((EDeviceAxtMotionFS20StopMethod)_ttoi(strValue));
			break;
		case EDeviceParameterAxtMotionFS20_Step1Direction:
			bReturn = !SetStep1Direction((EDeviceAxtMotionFS20DirectionMethod)_ttoi(strValue));
			break;
		case EDeviceParameterAxtMotionFS20_Step1Detect:
			bReturn = !SetStep1Detect((EDeviceAxtMotionFS20DetectSignal)_ttoi(strValue));
			break;
		case EDeviceParameterAxtMotionFS20_Step1Velocity:
			bReturn = !SetStep1Velocity(_ttof(strValue));
			break;
		case EDeviceParameterAxtMotionFS20_Step1Accel:
			bReturn = !SetStep1Accel(_ttof(strValue));
			break;
		case EDeviceParameterAxtMotionFS20_Step2Stop:
			bReturn = !SetStep2Stop((EDeviceAxtMotionFS20StopMethod)_ttoi(strValue));
			break;
		case EDeviceParameterAxtMotionFS20_Step2Direction:
			bReturn = !SetStep2Direction((EDeviceAxtMotionFS20DirectionMethod)_ttoi(strValue));
			break;
		case EDeviceParameterAxtMotionFS20_Step2Detect:
			bReturn = !SetStep2Detect((EDeviceAxtMotionFS20DetectSignal)_ttoi(strValue));
			break;
		case EDeviceParameterAxtMotionFS20_Step2Velocity:
			bReturn = !SetStep2Velocity(_ttof(strValue));
			break;
		case EDeviceParameterAxtMotionFS20_Step2Accel:
			bReturn = !SetStep2Accel(_ttof(strValue));
			break;
		case EDeviceParameterAxtMotionFS20_Step3Stop:
			bReturn = !SetStep3Stop((EDeviceAxtMotionFS20StopMethod)_ttoi(strValue));
			break;
		case EDeviceParameterAxtMotionFS20_Step3Direction:
			bReturn = !SetStep3Direction((EDeviceAxtMotionFS20DirectionMethod)_ttoi(strValue));
			break;
		case EDeviceParameterAxtMotionFS20_Step3Detect:
			bReturn = !SetStep3Detect((EDeviceAxtMotionFS20DetectSignal)_ttoi(strValue));
			break;
		case EDeviceParameterAxtMotionFS20_Step3Velocity:
			bReturn = !SetStep3Velocity(_ttof(strValue));
			break;
		case EDeviceParameterAxtMotionFS20_Step3Accel:
			bReturn = !SetStep3Accel(_ttof(strValue));
			break;
		case EDeviceParameterAxtMotionFS20_Step4Stop:
			bReturn = !SetStep4Stop((EDeviceAxtMotionFS20StopMethod)_ttoi(strValue));
			break;
		case EDeviceParameterAxtMotionFS20_Step4Direction:
			bReturn = !SetStep4Direction((EDeviceAxtMotionFS20DirectionMethod)_ttoi(strValue));
			break;
		case EDeviceParameterAxtMotionFS20_Step4Detect:
			bReturn = !SetStep4Detect((EDeviceAxtMotionFS20DetectSignal)_ttoi(strValue));
			break;
		case EDeviceParameterAxtMotionFS20_Step4Velocity:
			bReturn = !SetStep4Velocity(_ttof(strValue));
			break;
		case EDeviceParameterAxtMotionFS20_Step4Accel:
			bReturn = !SetStep4Accel(_ttof(strValue));
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

bool CDeviceAxtMotionFS20::DoesModuleExist()
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