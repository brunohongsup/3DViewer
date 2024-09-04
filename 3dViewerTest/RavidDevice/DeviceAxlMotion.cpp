#include "stdafx.h"

#include "DeviceAxlMotion.h"

#ifdef USING_DEVICE

#include "DeviceAxlDio.h"
#include "DeviceAxlMotionEthercat.h"
#include "DeviceAxlTriggerSIOCN2CH.h"

#include "../RavidCore/RavidDatabase.h"
#include "../RavidCore/RavidRecordset.h"

#include "../RavidFramework/LogManager.h"
#include "../RavidFramework/DeviceManager.h"
#include "../RavidFramework/LibraryManager.h"
#include "../RavidFramework/FrameworkManager.h"
#include "../RavidFramework/EventHandlerManager.h"
#include "../RavidFramework/MultiLanguageManager.h"

#include "../RavidFramework/RavidLedCtrl.h"

#include "../Libraries/Includes/AxlLib/AXHS.h"
#include "../Libraries/Includes/AxlLib/AXL.h"
#include "../Libraries/Includes/AxlLib/AXM.h"

// axl.dll
#pragma comment(lib, COMMONLIB_PREFIX "AxlLib/AXL.lib")

using namespace Ravid;
using namespace Ravid::Device;
using namespace Ravid::Database;
using namespace Ravid::Framework;
using namespace Ravid::Algorithms;
using namespace Ravid::Miscellaneous;

enum
{
	AXL_MODULE_MIN = 0,
	AXL_MODULE_MAX = 99,
};

IMPLEMENT_DYNAMIC(CDeviceAxlMotion, CDeviceMotion)

BEGIN_MESSAGE_MAP(CDeviceAxlMotion, CDeviceMotion)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static LPCTSTR g_lpszParamAxlMotion[EDeviceParameterAxlMotion_Count] =
{
	_T("DeviceID"),
	_T("SubUnitID"),
	_T("UnitPulse"),
	_T("StartSpeed"),
	_T("MaxSpeed"),
	_T("PulseOutputMethod"),
	_T("EncoderMethod"),
	_T("Servo Level"),
	_T("Home Level"),
	_T("PLimit Level"),
	_T("NLimit Level"),
	_T("Alarm Input Level"),
	_T("Alarm Reset Level"),
	_T("Inposition Input Level"),
	_T("Limit Parameter"),
	_T("Use Software Limit"),
	_T("Stop Mode"),
	_T("Counter Selection"),
	_T("Pos.Limit Value"),
	_T("Neg.Limit Value"),
	_T("Home Parameter"),
	_T("Home Search Sensor"),
	_T("Home Direction"),
	_T("Home Z phase"),
	_T("Home Clear Time"),
	_T("Home Offset"),
	_T("Home First Vel"),
	_T("Home Second Vel"),
	_T("Home Third Vel"),
	_T("Home Last Vel"),
	_T("Home Start Acc"),
	_T("Home End Acc"),
	_T("Open No Reset")
};

static LPCTSTR g_lpszAxlMotionPulseOutputMethod[EDeviceAxlMotionPulseOutputMethod_Count] =
{
	_T("OneHighLowHigh"),
	_T("OneHighHighLow"),
	_T("OneLowLowHigh"),
	_T("OneLowHighLow"),
	_T("TwoCcwCwHigh"),
	_T("TwoCcwCwLow"),
	_T("TwoCwCcwHigh"),
	_T("TwoCwCcwLow"),
	_T("TwoPhase"),
	_T("TwoPhaseReverse"),
};

static LPCTSTR g_lpszAxlMotionEncoderMethod[EDeviceAxlMotionEncoderMethod_Count] =
{
	_T("ObverseUpDownMode"),
	_T("ObverseSqr1Mode"),
	_T("ObverseSqr2Mode"),
	_T("ObverseSqr4Mode"),
	_T("ReverseUpDownMode"),
	_T("ReverseSqr1Mode"),
	_T("ReverseSqr2Mode"),
	_T("ReverseSqr4Mode"),
};

static LPCTSTR g_lpszAxlMotionLevelMethod[EDeviceAxlMotionLevelMethod_Count] =
{
	_T("LOW"),
	_T("HIGH"),
};

static LPCTSTR g_lpszAxlMotionDirection[EDeviceAxlMotionDirection_Count] =
{
	_T("CCW"),
	_T("CW"),
};

static LPCTSTR g_lpszAxlMotionZPhase[EDeviceAxlMotionZPhase_Count] =
{
	_T("NotUsed"),
	_T("ReverseHomeDir"),
	_T("HomeDir"),
};

static LPCTSTR g_lpszAxlMotionSearchSensor[EDeviceAxlMotionSearchSensor_Count] =
{
	_T("Pos Limit"),
	_T("Neg Limit"),
	_T("HomeSensor"),
};

static LPCTSTR g_lpszAxlMotionEnableDisable[EDeviceAxlMotionEnableDisable_Count] =
{
	_T("Disable"),
	_T("Enable"),
};

static LPCTSTR g_lpszAxlMotionStopMode[EDeviceAxlMotionStopMode_Count] =
{
	_T("Emergency Stop"),
	_T("Slowdown Stop"),
};

static LPCTSTR g_lpszAxlMotionCounterSelection[EDeviceAxlMotionCounterSelection_Count] =
{
	_T("Command Pos"),
	_T("Actual Pos"),
};

static LPCTSTR g_lpszSwitch[EDeviceDIOSwitch_Count] =
{
	_T("Off"),
	_T("On")
};




CDeviceAxlMotion::CDeviceAxlMotion()
{
}


CDeviceAxlMotion::~CDeviceAxlMotion()
{
}

EDeviceInitializeResult CDeviceAxlMotion::Initialize()
{
	EDeviceInitializeResult eReturn = EDeviceInitializeResult_UnknownError;

	CString strMessage, strStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("AxlMotion"));

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

		if(!AxlIsOpened())
		{
			bool bInitHardware = false;
			GetOpenNoHardware(&bInitHardware);

			if(bInitHardware != true)
			{
				if(AxlOpenNoReset(-1) != AXT_RT_SUCCESS)
				{
					strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntopenthelibrary);
					eReturn = EDeviceInitializeResult_AlreadyInitializedError;
					break;
				}
			}
			else
			{
				if(AxlOpen(-1) != AXT_RT_SUCCESS)
				{
					strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntopenthelibrary);
					eReturn = EDeviceInitializeResult_AlreadyInitializedError;
					break;
				}
			}
		}

		DWORD dwStatus = STATUS_NOTEXIST;
		if(AxmInfoIsMotionModule(&dwStatus) != AXT_RT_SUCCESS)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Module info"));
			eReturn = EDeviceInitializeResult_NotFoundDeviceError;
			break;
		}

		if(dwStatus != STATUS_EXIST)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_CouldntfindIOmodule);
			eReturn = EDeviceInitializeResult_NotFoundDeviceError;
			break;
		}
			   
		long i32ModulCount = 0;
		if(AxmInfoGetAxisCount(&i32ModulCount) != AXT_RT_SUCCESS)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Module Count"));
			eReturn = EDeviceInitializeResult_NotFoundDeviceError;
			break;
		}
			   
		int nModuleID = _ttoi(GetSubUnitID());
		if(0 > nModuleID && i32ModulCount <= nModuleID)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_DoesntsupporttheSubUnitID);
			eReturn = EDeviceInitializeResult_NotInitializeMotionError;
			break;
		}

		if(AxmInfoIsInvalidAxisNo(nModuleID) != AXT_RT_SUCCESS)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Theaxisisnotvalid);
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

		if(AxmMotSetMoveUnitPerPulse(nModuleID, dblUnitPulse, 1) != AXT_RT_SUCCESS)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Unit pulse"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		double dblStartSpeed = 0.;

		if(GetStartSpeed(&dblStartSpeed))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Start speed"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(AxmMotSetMinVel(nModuleID, dblStartSpeed) != AXT_RT_SUCCESS)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Start speed"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		double dblMaxSpeed = 0.;

		if(GetMaxSpeed(&dblMaxSpeed))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Max speed"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(AxmMotSetMaxVel(nModuleID, dblMaxSpeed) != AXT_RT_SUCCESS)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Max speed"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		EDeviceAxlMotionPulseOutputMethod ePulseOutputMethod = EDeviceAxlMotionPulseOutputMethod_Count;

		if(GetPulseOutputMethod(&ePulseOutputMethod))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Pulse output method"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(AxmMotSetPulseOutMethod(nModuleID, ePulseOutputMethod) != AXT_RT_SUCCESS)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Pulse output method"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		EDeviceAxlMotionEncoderMethod eEncoderMethod = EDeviceAxlMotionEncoderMethod_Count;

		if(GetEncoderMethod(&eEncoderMethod))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Encoder method"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(AxmMotSetEncInputMethod(nModuleID, eEncoderMethod) != AXT_RT_SUCCESS)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Encoder method"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		EDeviceAxlMotionLevelMethod eLevelServo = EDeviceAxlMotionLevelMethod_Count;

		if(GetServoLevel(&eLevelServo))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Servo level"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(AxmSignalSetServoOnLevel(nModuleID, eLevelServo) != AXT_RT_SUCCESS)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Home level"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		EDeviceAxlMotionLevelMethod eLevelHome = EDeviceAxlMotionLevelMethod_Count;

		if(GetHomeLevel(&eLevelHome))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Home level"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(AxmHomeSetSignalLevel(nModuleID, eLevelHome) != AXT_RT_SUCCESS)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Home level"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		EDeviceAxlMotionLevelMethod ePLimitLevel = EDeviceAxlMotionLevelMethod_Count;

		if(GetPLimitLevel(&ePLimitLevel))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("P limit level"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		EDeviceAxlMotionLevelMethod eNLimitLevel = EDeviceAxlMotionLevelMethod_Count;

		if(GetNLimitLevel(&eNLimitLevel))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("N limit level"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(AxmSignalSetLimit(nModuleID, EMERGENCY_STOP, ePLimitLevel, eNLimitLevel) != AXT_RT_SUCCESS)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("P/N limit level"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		EDeviceAxlMotionLevelMethod eInpositionInputLevel = EDeviceAxlMotionLevelMethod_Count;

		if(GetInpositionInputLevel(&eInpositionInputLevel))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Inposition input level"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(AxmSignalSetInpos(nModuleID, eInpositionInputLevel) != AXT_RT_SUCCESS)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Inposition input level"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		EDeviceAxlMotionLevelMethod eAlarmInputLevel = EDeviceAxlMotionLevelMethod_Count;

		if(GetAlarmInputLevel(&eAlarmInputLevel))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Alarm input level"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(AxmSignalSetServoAlarm(nModuleID, eAlarmInputLevel) != AXT_RT_SUCCESS)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Alarm input level"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		EDeviceAxlMotionLevelMethod eAlarmResetLevel = EDeviceAxlMotionLevelMethod_Count;

		if(GetAlarmResetLevel(&eAlarmResetLevel))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Alarm reset level"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(AxmSignalSetServoAlarmResetLevel(nModuleID, eAlarmResetLevel) != AXT_RT_SUCCESS)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Alarm reset level"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		bool bSoftwareLimit = false;

		if(GetEnableSoftwareLimit(&bSoftwareLimit))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("EnableSoftLimit"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(bSoftwareLimit)
		{
			EDeviceAxlMotionStopMode eStopMode = EDeviceAxlMotionStopMode_EmergencyStop;

			EDeviceAxlMotionCounterSelection eCounterSelection = EDeviceAxlMotionCounterSelection_Command;

			double dblPositive = 0.0, dblNegative = 0.0;

			if(GetSoftwareStopMode(&eStopMode))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Soft Stop Mode"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(GetSoftwareCounterSelection(&eCounterSelection))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Soft Counter Selection"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(GetPositiveLimit(&dblPositive))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Positive Limit"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(GetNegativeLimit(&dblNegative))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Negative Limit"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(AxmSignalSetSoftLimit(nModuleID, bSoftwareLimit, (DWORD)eStopMode, (DWORD)eCounterSelection, dblPositive, dblNegative) != AXT_RT_SUCCESS)
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Soft Limit"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}
		}
		else
		{
			if(AxmSignalSetSoftLimit(nModuleID, bSoftwareLimit, 0, 0, 0.0, 0.0) != AXT_RT_SUCCESS)
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Soft Limit"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}
		}

		AxmSignalSetStop(nModuleID, EMERGENCY_STOP, UNUSED);

		AxmSignalSetStop(nModuleID, SLOWDOWN_STOP, UNUSED);

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

EDeviceTerminateResult CDeviceAxlMotion::Terminate()
{
	EDeviceTerminateResult eReturn = EDeviceTerminateResult_UnknownError;

	CString strMessage;

	SetStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("AxlMotion"));

		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);

		return EDeviceTerminateResult_NotFoundApiError;
	}

	do
	{
		bool bNeedTreminate = true;
		size_t szDeviceCnt = CDeviceManager::GetDeviceCount();
		for(size_t i = 0; i < szDeviceCnt; ++i)
		{
			CDeviceBase* pDevicebase = CDeviceManager::GetDeviceByIndex(i);
			if(!pDevicebase)
				continue;

			if(this == pDevicebase)
				continue;

			CDeviceAxlMotion* pMotion = dynamic_cast<CDeviceAxlMotion*>(pDevicebase);
			if(pMotion)
			{
				if(!pMotion->IsInitialized())
					continue;

				bNeedTreminate = false;

				break;
			}

			CDeviceAxlMotionEthercat* pMotionEth = dynamic_cast<CDeviceAxlMotionEthercat*>(pDevicebase);
			if(pMotionEth)
			{
				if(!pMotionEth->IsInitialized())
					continue;

				bNeedTreminate = false;

				break;
			}

			CDeviceAxlDio* pDio = dynamic_cast<CDeviceAxlDio*>(pDevicebase);
			if(pDio)
			{
				if(!pDio->IsInitialized())
					continue;

				bNeedTreminate = false;

				break;
			}

			CDeviceAxlTriggerSIOCN2CH* pTrigger = dynamic_cast<CDeviceAxlTriggerSIOCN2CH*>(pDevicebase);
			if(pTrigger)
			{
				if(!pTrigger->IsInitialized())
					continue;

				bNeedTreminate = false;

				break;
			}
		}

		if(bNeedTreminate)
		{
			if(AxlIsOpened())
				AxlClose();
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

bool CDeviceAxlMotion::LoadSettings()
{
	bool bReturn = false;

	m_vctParameterFieldConfigurations.clear();

	do
	{
		AddParameterFieldConfigurations(EDeviceParameterAxlMotion_DeviceID, g_lpszParamAxlMotion[EDeviceParameterAxlMotion_DeviceID], _T("0"), EParameterFieldType_Edit);
		AddParameterFieldConfigurations(EDeviceParameterAxlMotion_SubUnitID, g_lpszParamAxlMotion[EDeviceParameterAxlMotion_SubUnitID], _T("0"), EParameterFieldType_Edit);
		AddParameterFieldConfigurations(EDeviceParameterAxlMotion_OpenNoReset, g_lpszParamAxlMotion[EDeviceParameterAxlMotion_OpenNoReset], _T("0"), EParameterFieldType_Check, nullptr, _T("Initializes the hardware when initializing the device."), 0);

		AddParameterFieldConfigurations(EDeviceParameterAxlMotion_UnitPulse, g_lpszParamAxlMotion[EDeviceParameterAxlMotion_UnitPulse], _T("0"), EParameterFieldType_Edit, nullptr, _T("double type"));
		AddParameterFieldConfigurations(EDeviceParameterAxlMotion_StartSpeed, g_lpszParamAxlMotion[EDeviceParameterAxlMotion_StartSpeed], _T("0"), EParameterFieldType_Edit, nullptr, _T("double type"));
		AddParameterFieldConfigurations(EDeviceParameterAxlMotion_MaxSpeed, g_lpszParamAxlMotion[EDeviceParameterAxlMotion_MaxSpeed], _T("0"), EParameterFieldType_Edit, nullptr, _T("double type"));

		AddParameterFieldConfigurations(EDeviceParameterAxlMotion_PulseOutputMethod, g_lpszParamAxlMotion[EDeviceParameterAxlMotion_PulseOutputMethod], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxlMotionPulseOutputMethod, EDeviceAxlMotionPulseOutputMethod_Count), _T("ex) OneHighLowHigh = 1 pulse, PULSE(Active High), cw(DIR=Low)  / ccw(DIR=High)"));
		AddParameterFieldConfigurations(EDeviceParameterAxlMotion_EncoderMethod, g_lpszParamAxlMotion[EDeviceParameterAxlMotion_EncoderMethod], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxlMotionEncoderMethod, EDeviceAxlMotionEncoderMethod_Count), nullptr);

		AddParameterFieldConfigurations(EDeviceParameterAxlMotion_ServoLevel, g_lpszParamAxlMotion[EDeviceParameterAxlMotion_ServoLevel], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxlMotionLevelMethod, EDeviceAxlMotionLevelMethod_Count), _T("integer type"));
		AddParameterFieldConfigurations(EDeviceParameterAxlMotion_HomeLevel, g_lpszParamAxlMotion[EDeviceParameterAxlMotion_HomeLevel], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxlMotionLevelMethod, EDeviceAxlMotionLevelMethod_Count), _T("integer type"));
		AddParameterFieldConfigurations(EDeviceParameterAxlMotion_PLimitLevel, g_lpszParamAxlMotion[EDeviceParameterAxlMotion_PLimitLevel], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxlMotionLevelMethod, EDeviceAxlMotionLevelMethod_Count), _T("integer type"));
		AddParameterFieldConfigurations(EDeviceParameterAxlMotion_NLimitLevel, g_lpszParamAxlMotion[EDeviceParameterAxlMotion_NLimitLevel], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxlMotionLevelMethod, EDeviceAxlMotionLevelMethod_Count), _T("integer type"));
		AddParameterFieldConfigurations(EDeviceParameterAxlMotion_AlarmInputLevel, g_lpszParamAxlMotion[EDeviceParameterAxlMotion_AlarmInputLevel], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxlMotionLevelMethod, EDeviceAxlMotionLevelMethod_Count), _T("integer type"));
		AddParameterFieldConfigurations(EDeviceParameterAxlMotion_AlarmResetLevel, g_lpszParamAxlMotion[EDeviceParameterAxlMotion_AlarmResetLevel], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxlMotionLevelMethod, EDeviceAxlMotionLevelMethod_Count), _T("integer type"));
		AddParameterFieldConfigurations(EDeviceParameterAxlMotion_InpositionInputLevel, g_lpszParamAxlMotion[EDeviceParameterAxlMotion_InpositionInputLevel], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxlMotionLevelMethod, EDeviceAxlMotionLevelMethod_Count), _T("integer type"));

		AddParameterFieldConfigurations(EDeviceParameterAxlMotion_LimitParameter, g_lpszParamAxlMotion[EDeviceParameterAxlMotion_LimitParameter], _T("0"), EParameterFieldType_None, nullptr, nullptr, 0);
		{
			AddParameterFieldConfigurations(EDeviceParameterAxlMotion_UseSoftwareLimit, g_lpszParamAxlMotion[EDeviceParameterAxlMotion_UseSoftwareLimit], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterAxlMotion_StopMode, g_lpszParamAxlMotion[EDeviceParameterAxlMotion_StopMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxlMotionStopMode, EDeviceAxlMotionStopMode_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterAxlMotion_CounterSelection, g_lpszParamAxlMotion[EDeviceParameterAxlMotion_CounterSelection], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxlMotionCounterSelection, EDeviceAxlMotionCounterSelection_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterAxlMotion_PosLimitValue, g_lpszParamAxlMotion[EDeviceParameterAxlMotion_PosLimitValue], _T("0"), EParameterFieldType_Edit, nullptr, _T("double type"), 1);
			AddParameterFieldConfigurations(EDeviceParameterAxlMotion_NegLimitValue, g_lpszParamAxlMotion[EDeviceParameterAxlMotion_NegLimitValue], _T("0"), EParameterFieldType_Edit, nullptr, _T("double type"), 1);
		}

		AddParameterFieldConfigurations(EDeviceParameterAxlMotion_HomeParameter, g_lpszParamAxlMotion[EDeviceParameterAxlMotion_HomeParameter], _T("0"), EParameterFieldType_None, nullptr, nullptr, 0);
		{
			AddParameterFieldConfigurations(EDeviceParameterAxlMotion_HomeSearchSensor, g_lpszParamAxlMotion[EDeviceParameterAxlMotion_HomeSearchSensor], _T("2"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxlMotionSearchSensor, EDeviceAxlMotionSearchSensor_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterAxlMotion_HomeDirection, g_lpszParamAxlMotion[EDeviceParameterAxlMotion_HomeDirection], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxlMotionDirection, EDeviceAxlMotionDirection_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterAxlMotion_HomeZphase, g_lpszParamAxlMotion[EDeviceParameterAxlMotion_HomeZphase], _T("1"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxlMotionZPhase, EDeviceAxlMotionZPhase_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterAxlMotion_HomeHomeClearTime, g_lpszParamAxlMotion[EDeviceParameterAxlMotion_HomeHomeClearTime], _T("1000.000000"), EParameterFieldType_Edit, nullptr, _T("double"), 1);
			AddParameterFieldConfigurations(EDeviceParameterAxlMotion_HomeOffset, g_lpszParamAxlMotion[EDeviceParameterAxlMotion_HomeOffset], _T("0.000000"), EParameterFieldType_Edit, nullptr, _T("double"), 1);

			AddParameterFieldConfigurations(EDeviceParameterAxlMotion_HomeFirstVel, g_lpszParamAxlMotion[EDeviceParameterAxlMotion_HomeFirstVel], _T("40.000000"), EParameterFieldType_Edit, nullptr, _T("double"), 1);
			AddParameterFieldConfigurations(EDeviceParameterAxlMotion_HomeSecondVel, g_lpszParamAxlMotion[EDeviceParameterAxlMotion_HomeSecondVel], _T("20.000000"), EParameterFieldType_Edit, nullptr, _T("double"), 1);
			AddParameterFieldConfigurations(EDeviceParameterAxlMotion_HomeThirdVel, g_lpszParamAxlMotion[EDeviceParameterAxlMotion_HomeThirdVel], _T("10.000000"), EParameterFieldType_Edit, nullptr, _T("double"), 1);
			AddParameterFieldConfigurations(EDeviceParameterAxlMotion_HomeLastVel, g_lpszParamAxlMotion[EDeviceParameterAxlMotion_HomeLastVel], _T("5.000000"), EParameterFieldType_Edit, nullptr, _T("double"), 1);
			AddParameterFieldConfigurations(EDeviceParameterAxlMotion_HomeStartAcc, g_lpszParamAxlMotion[EDeviceParameterAxlMotion_HomeStartAcc], _T("40.000000"), EParameterFieldType_Edit, nullptr, _T("double"), 1);
			AddParameterFieldConfigurations(EDeviceParameterAxlMotion_HomeEndAcc, g_lpszParamAxlMotion[EDeviceParameterAxlMotion_HomeEndAcc], _T("10.000000"), EParameterFieldType_Edit, nullptr, _T("double"), 1);
		}

		bReturn = true;
	}
	while(false);

	return bReturn & __super::LoadSettings();
}

AxlMotionGetFunction CDeviceAxlMotion::GetUnitPulse(double* pParam)
{
	AxlMotionGetFunction eReturn = AxlMotionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = AxlMotionGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterAxlMotion_UnitPulse));

		eReturn = AxlMotionGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionSetFunction CDeviceAxlMotion::SetUnitPulse(double dblParam)
{
	AxlMotionSetFunction eReturn = AxlMotionSetFunction_UnknownError;

	EDeviceParameterAxlMotion eSaveID = EDeviceParameterAxlMotion_UnitPulse;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
			AxmMotSetMoveUnitPerPulse(_ttoi(GetSubUnitID()), dblParam, 1);

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotion[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionGetFunction CDeviceAxlMotion::GetStartSpeed(double* pParam)
{
	AxlMotionGetFunction eReturn = AxlMotionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = AxlMotionGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterAxlMotion_StartSpeed));

		eReturn = AxlMotionGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionSetFunction CDeviceAxlMotion::SetStartSpeed(double dblParam)
{
	AxlMotionSetFunction eReturn = AxlMotionSetFunction_UnknownError;

	EDeviceParameterAxlMotion eSaveID = EDeviceParameterAxlMotion_StartSpeed;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
			AxmMotSetMinVel(_ttoi(GetSubUnitID()), dblParam);

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotion[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionGetFunction CDeviceAxlMotion::GetMaxSpeed(double* pParam)
{
	AxlMotionGetFunction eReturn = AxlMotionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = AxlMotionGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterAxlMotion_MaxSpeed));

		eReturn = AxlMotionGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionSetFunction CDeviceAxlMotion::SetMaxSpeed(double dblParam)
{
	AxlMotionSetFunction eReturn = AxlMotionSetFunction_UnknownError;

	EDeviceParameterAxlMotion eSaveID = EDeviceParameterAxlMotion_MaxSpeed;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
			AxmMotSetMaxVel(_ttoi(GetSubUnitID()), dblParam);

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotion[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionGetFunction CDeviceAxlMotion::GetPulseOutputMethod(EDeviceAxlMotionPulseOutputMethod* pParam)
{
	AxlMotionGetFunction eReturn = AxlMotionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = AxlMotionGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxlMotionPulseOutputMethod)_ttoi(GetParamValue(EDeviceParameterAxlMotion_PulseOutputMethod));

		eReturn = AxlMotionGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionSetFunction CDeviceAxlMotion::SetPulseOutputMethod(EDeviceAxlMotionPulseOutputMethod eParam)
{
	AxlMotionSetFunction eReturn = AxlMotionSetFunction_UnknownError;

	EDeviceParameterAxlMotion eSaveID = EDeviceParameterAxlMotion_PulseOutputMethod;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxlMotionPulseOutputMethod_Count)
		{
			eReturn = AxlMotionSetFunction_NotFoundCommandError;
			break;
		}

		if(IsInitialized())
			AxmMotSetPulseOutMethod(_ttoi(GetSubUnitID()), (int)eParam);

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotion[eSaveID], g_lpszAxlMotionPulseOutputMethod[nPreValue], g_lpszAxlMotionPulseOutputMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionGetFunction CDeviceAxlMotion::GetEncoderMethod(EDeviceAxlMotionEncoderMethod* pParam)
{
	AxlMotionGetFunction eReturn = AxlMotionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = AxlMotionGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxlMotionEncoderMethod)_ttoi(GetParamValue(EDeviceParameterAxlMotion_EncoderMethod));

		eReturn = AxlMotionGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionSetFunction CDeviceAxlMotion::SetEncoderMethod(EDeviceAxlMotionEncoderMethod eParam)
{
	AxlMotionSetFunction eReturn = AxlMotionSetFunction_UnknownError;

	EDeviceParameterAxlMotion eSaveID = EDeviceParameterAxlMotion_EncoderMethod;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxlMotionEncoderMethod_Count)
		{
			eReturn = AxlMotionSetFunction_NotFoundCommandError;
			break;
		}

		if(IsInitialized())
			AxmMotSetEncInputMethod(_ttoi(GetSubUnitID()), (int)eParam);

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotion[eSaveID], g_lpszAxlMotionEncoderMethod[nPreValue], g_lpszAxlMotionEncoderMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionGetFunction CDeviceAxlMotion::GetServoLevel(EDeviceAxlMotionLevelMethod* pParam)
{
	AxlMotionGetFunction eReturn = AxlMotionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = AxlMotionGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxlMotionLevelMethod)_ttoi(GetParamValue(EDeviceParameterAxlMotion_ServoLevel));

		eReturn = AxlMotionGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionSetFunction CDeviceAxlMotion::SetServoLevel(EDeviceAxlMotionLevelMethod eParam)
{
	AxlMotionSetFunction eReturn = AxlMotionSetFunction_UnknownError;

	EDeviceParameterAxlMotion eSaveID = EDeviceParameterAxlMotion_ServoLevel;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxlMotionLevelMethod_Count)
		{
			eReturn = AxlMotionSetFunction_NotFoundCommandError;
			break;
		}

		if(IsInitialized())
			AxmSignalSetServoOnLevel(_ttoi(GetSubUnitID()), (int)eParam);

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotion[eSaveID], g_lpszAxlMotionLevelMethod[nPreValue], g_lpszAxlMotionLevelMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionGetFunction CDeviceAxlMotion::GetHomeLevel(EDeviceAxlMotionLevelMethod* pParam)
{
	AxlMotionGetFunction eReturn = AxlMotionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = AxlMotionGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxlMotionLevelMethod)_ttoi(GetParamValue(EDeviceParameterAxlMotion_HomeLevel));

		eReturn = AxlMotionGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionSetFunction CDeviceAxlMotion::SetHomeLevel(EDeviceAxlMotionLevelMethod eParam)
{
	AxlMotionSetFunction eReturn = AxlMotionSetFunction_UnknownError;

	EDeviceParameterAxlMotion eSaveID = EDeviceParameterAxlMotion_HomeLevel;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxlMotionLevelMethod_Count)
		{
			eReturn = AxlMotionSetFunction_NotFoundCommandError;
			break;
		}

		if(IsInitialized())
			AxmHomeSetSignalLevel(_ttoi(GetSubUnitID()), (int)eParam);

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotion[eSaveID], g_lpszAxlMotionLevelMethod[nPreValue], g_lpszAxlMotionLevelMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionGetFunction CDeviceAxlMotion::GetPLimitLevel(EDeviceAxlMotionLevelMethod* pParam)
{
	AxlMotionGetFunction eReturn = AxlMotionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = AxlMotionGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxlMotionLevelMethod)_ttoi(GetParamValue(EDeviceParameterAxlMotion_PLimitLevel));

		eReturn = AxlMotionGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionSetFunction CDeviceAxlMotion::SetPLimitLevel(EDeviceAxlMotionLevelMethod eParam)
{
	AxlMotionSetFunction eReturn = AxlMotionSetFunction_UnknownError;

	EDeviceParameterAxlMotion eSaveID = EDeviceParameterAxlMotion_PLimitLevel;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxlMotionLevelMethod_Count)
		{
			eReturn = AxlMotionSetFunction_NotFoundCommandError;
			break;
		}

		if(IsInitialized())
		{
			EDeviceAxlMotionLevelMethod eLevelMethod = EDeviceAxlMotionLevelMethod_Count;

			if(GetNLimitLevel(&eLevelMethod))
			{
				eReturn = AxlMotionSetFunction_ReadOnDatabaseError;
				break;
			}

			if(eLevelMethod == EDeviceAxlMotionLevelMethod_Count)
			{
				eReturn = AxlMotionSetFunction_ReadOnDatabaseError;
				break;
			}

			AxmSignalSetLimit(_ttoi(GetSubUnitID()), EMERGENCY_STOP, (int)eParam, eLevelMethod);
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotion[eSaveID], g_lpszAxlMotionLevelMethod[nPreValue], g_lpszAxlMotionLevelMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionGetFunction CDeviceAxlMotion::GetNLimitLevel(EDeviceAxlMotionLevelMethod* pParam)
{
	AxlMotionGetFunction eReturn = AxlMotionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = AxlMotionGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxlMotionLevelMethod)_ttoi(GetParamValue(EDeviceParameterAxlMotion_NLimitLevel));

		eReturn = AxlMotionGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionSetFunction CDeviceAxlMotion::SetNLimitLevel(EDeviceAxlMotionLevelMethod eParam)
{
	AxlMotionSetFunction eReturn = AxlMotionSetFunction_UnknownError;

	EDeviceParameterAxlMotion eSaveID = EDeviceParameterAxlMotion_NLimitLevel;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxlMotionLevelMethod_Count)
		{
			eReturn = AxlMotionSetFunction_NotFoundCommandError;
			break;
		}

		if(IsInitialized())
		{
			EDeviceAxlMotionLevelMethod eLevelMethod = EDeviceAxlMotionLevelMethod_Count;

			if(GetPLimitLevel(&eLevelMethod))
			{
				eReturn = AxlMotionSetFunction_ReadOnDatabaseError;
				break;
			}

			AxmSignalSetLimit(_ttoi(GetSubUnitID()), EMERGENCY_STOP, eLevelMethod, (int)eParam);
		}
		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotion[eSaveID], g_lpszAxlMotionLevelMethod[nPreValue], g_lpszAxlMotionLevelMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionGetFunction CDeviceAxlMotion::GetAlarmInputLevel(EDeviceAxlMotionLevelMethod* pParam)
{
	AxlMotionGetFunction eReturn = AxlMotionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = AxlMotionGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxlMotionLevelMethod)_ttoi(GetParamValue(EDeviceParameterAxlMotion_AlarmInputLevel));

		eReturn = AxlMotionGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionSetFunction CDeviceAxlMotion::SetAlarmInputLevel(EDeviceAxlMotionLevelMethod eParam)
{
	AxlMotionSetFunction eReturn = AxlMotionSetFunction_UnknownError;

	EDeviceParameterAxlMotion eSaveID = EDeviceParameterAxlMotion_AlarmInputLevel;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxlMotionLevelMethod_Count)
		{
			eReturn = AxlMotionSetFunction_NotFoundCommandError;
			break;
		}

		if(IsInitialized())
			AxmSignalSetServoAlarm(_ttoi(GetSubUnitID()), (int)eParam);

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotion[eSaveID], g_lpszAxlMotionLevelMethod[nPreValue], g_lpszAxlMotionLevelMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionGetFunction CDeviceAxlMotion::GetAlarmResetLevel(EDeviceAxlMotionLevelMethod* pParam)
{
	AxlMotionGetFunction eReturn = AxlMotionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = AxlMotionGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxlMotionLevelMethod)_ttoi(GetParamValue(EDeviceParameterAxlMotion_AlarmResetLevel));

		eReturn = AxlMotionGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionSetFunction CDeviceAxlMotion::SetAlarmResetLevel(EDeviceAxlMotionLevelMethod eParam)
{
	AxlMotionSetFunction eReturn = AxlMotionSetFunction_UnknownError;

	EDeviceParameterAxlMotion eSaveID = EDeviceParameterAxlMotion_AlarmResetLevel;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxlMotionLevelMethod_Count)
		{
			eReturn = AxlMotionSetFunction_NotFoundCommandError;
			break;
		}

		if(IsInitialized())
			AxmSignalSetServoAlarmResetLevel(_ttoi(GetSubUnitID()), (int)eParam);

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotion[eSaveID], g_lpszAxlMotionLevelMethod[nPreValue], g_lpszAxlMotionLevelMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionGetFunction CDeviceAxlMotion::GetInpositionInputLevel(EDeviceAxlMotionLevelMethod* pParam)
{
	AxlMotionGetFunction eReturn = AxlMotionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = AxlMotionGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxlMotionLevelMethod)_ttoi(GetParamValue(EDeviceParameterAxlMotion_InpositionInputLevel));

		eReturn = AxlMotionGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionSetFunction CDeviceAxlMotion::SetInpositionInputLevel(EDeviceAxlMotionLevelMethod eParam)
{
	AxlMotionSetFunction eReturn = AxlMotionSetFunction_UnknownError;

	EDeviceParameterAxlMotion eSaveID = EDeviceParameterAxlMotion_InpositionInputLevel;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxlMotionLevelMethod_Count)
		{
			eReturn = AxlMotionSetFunction_NotFoundCommandError;
			break;
		}

		if(IsInitialized())
			AxmSignalSetInpos(_ttoi(GetSubUnitID()), (int)eParam);

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotion[eSaveID], g_lpszAxlMotionLevelMethod[nPreValue], g_lpszAxlMotionLevelMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionGetFunction CDeviceAxlMotion::GetEnableSoftwareLimit(bool* pParam)
{
	AxlMotionGetFunction eReturn = AxlMotionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = AxlMotionGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAxlMotion_UseSoftwareLimit));

		eReturn = AxlMotionGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionSetFunction CDeviceAxlMotion::SetEnableSoftwareLimit(bool bParam)
{
	AxlMotionSetFunction eReturn = AxlMotionSetFunction_UnknownError;

	EDeviceParameterAxlMotion eSaveID = EDeviceParameterAxlMotion_UseSoftwareLimit;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			EDeviceAxlMotionStopMode eStopMode = EDeviceAxlMotionStopMode_EmergencyStop;

			EDeviceAxlMotionCounterSelection eCounterSelection = EDeviceAxlMotionCounterSelection_Command;

			double dblPositive = 0.0, dblNegative = 0.0;

			if(GetSoftwareStopMode(&eStopMode))
			{
				eReturn = AxlMotionSetFunction_ReadOnDatabaseError;
				break;
			}

			if(GetSoftwareCounterSelection(&eCounterSelection))
			{
				eReturn = AxlMotionSetFunction_ReadOnDatabaseError;
				break;
			}

			if(GetPositiveLimit(&dblPositive))
			{
				eReturn = AxlMotionSetFunction_ReadOnDatabaseError;
				break;
			}

			if(GetNegativeLimit(&dblNegative))
			{
				eReturn = AxlMotionSetFunction_ReadOnDatabaseError;
				break;
			}

			AxmSignalSetSoftLimit(_ttoi(GetSubUnitID()), bParam, (DWORD)eStopMode, (DWORD)eCounterSelection, dblPositive, dblNegative);
		}

		CString strData;
		strData.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotion[eSaveID], g_lpszAxlMotionEnableDisable[nPreValue], g_lpszAxlMotionEnableDisable[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionGetFunction CDeviceAxlMotion::GetSoftwareStopMode(EDeviceAxlMotionStopMode * pParam)
{
	AxlMotionGetFunction eReturn = AxlMotionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = AxlMotionGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxlMotionStopMode)_ttoi(GetParamValue(EDeviceParameterAxlMotion_StopMode));

		eReturn = AxlMotionGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionSetFunction CDeviceAxlMotion::SetSoftwareStopMode(EDeviceAxlMotionStopMode eParam)
{
	AxlMotionSetFunction eReturn = AxlMotionSetFunction_UnknownError;

	EDeviceParameterAxlMotion eSaveID = EDeviceParameterAxlMotion_StopMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxlMotionStopMode_Count)
		{
			eReturn = AxlMotionSetFunction_NotFoundCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(IsInitialized())
			{
				bool bEnable = false;

				EDeviceAxlMotionCounterSelection eCounterSelection = EDeviceAxlMotionCounterSelection_Command;

				double dblPositive = 0.0, dblNegative = 0.0;

				if(GetEnableSoftwareLimit(&bEnable))
				{
					eReturn = AxlMotionSetFunction_ReadOnDatabaseError;
					break;
				}

				if(GetSoftwareCounterSelection(&eCounterSelection))
				{
					eReturn = AxlMotionSetFunction_ReadOnDatabaseError;
					break;
				}

				if(GetPositiveLimit(&dblPositive))
				{
					eReturn = AxlMotionSetFunction_ReadOnDatabaseError;
					break;
				}

				if(GetNegativeLimit(&dblNegative))
				{
					eReturn = AxlMotionSetFunction_ReadOnDatabaseError;
					break;
				}

				AxmSignalSetSoftLimit(_ttoi(GetSubUnitID()), bEnable, (DWORD)eParam, (DWORD)eCounterSelection, dblPositive, dblNegative);
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotion[eSaveID], g_lpszAxlMotionStopMode[nPreValue], g_lpszAxlMotionStopMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionGetFunction CDeviceAxlMotion::GetSoftwareCounterSelection(EDeviceAxlMotionCounterSelection * pParam)
{
	AxlMotionGetFunction eReturn = AxlMotionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = AxlMotionGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxlMotionCounterSelection)_ttoi(GetParamValue(EDeviceParameterAxlMotion_CounterSelection));

		eReturn = AxlMotionGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionSetFunction CDeviceAxlMotion::SetSoftwareCounterSelection(EDeviceAxlMotionCounterSelection eParam)
{
	AxlMotionSetFunction eReturn = AxlMotionSetFunction_UnknownError;

	EDeviceParameterAxlMotion eSaveID = EDeviceParameterAxlMotion_CounterSelection;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxlMotionCounterSelection_Count)
		{
			eReturn = AxlMotionSetFunction_NotFoundCommandError;
			break;
		}

		if(IsInitialized())
		{
			bool bEnable = false;

			EDeviceAxlMotionStopMode eStopMode = EDeviceAxlMotionStopMode_EmergencyStop;

			double dblPositive = 0.0, dblNegative = 0.0;

			if(GetEnableSoftwareLimit(&bEnable))
			{
				eReturn = AxlMotionSetFunction_ReadOnDatabaseError;
				break;
			}

			if(GetSoftwareStopMode(&eStopMode))
			{
				eReturn = AxlMotionSetFunction_ReadOnDatabaseError;
				break;
			}

			if(GetPositiveLimit(&dblPositive))
			{
				eReturn = AxlMotionSetFunction_ReadOnDatabaseError;
				break;
			}

			if(GetNegativeLimit(&dblNegative))
			{
				eReturn = AxlMotionSetFunction_ReadOnDatabaseError;
				break;
			}

			AxmSignalSetSoftLimit(_ttoi(GetSubUnitID()), bEnable, (DWORD)eStopMode, (DWORD)eParam, dblPositive, dblNegative);
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotion[eSaveID], g_lpszAxlMotionCounterSelection[nPreValue], g_lpszAxlMotionCounterSelection[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionGetFunction CDeviceAxlMotion::GetPositiveLimit(double* pParam)
{
	AxlMotionGetFunction eReturn = AxlMotionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = AxlMotionGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterAxlMotion_PosLimitValue));

		eReturn = AxlMotionGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionSetFunction CDeviceAxlMotion::SetPositiveLimit(double dblParam)
{
	AxlMotionSetFunction eReturn = AxlMotionSetFunction_UnknownError;

	EDeviceParameterAxlMotion eSaveID = EDeviceParameterAxlMotion_PosLimitValue;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			bool bEnable = false;

			EDeviceAxlMotionStopMode eStopMode = EDeviceAxlMotionStopMode_EmergencyStop;

			EDeviceAxlMotionCounterSelection eCount = EDeviceAxlMotionCounterSelection_Command;

			double dblNegative = 0.0;

			if(GetEnableSoftwareLimit(&bEnable))
			{
				eReturn = AxlMotionSetFunction_ReadOnDatabaseError;
				break;
			}

			if(GetSoftwareStopMode(&eStopMode))
			{
				eReturn = AxlMotionSetFunction_ReadOnDatabaseError;
				break;
			}

			if(GetSoftwareCounterSelection(&eCount))
			{
				eReturn = AxlMotionSetFunction_ReadOnDatabaseError;
				break;
			}

			if(GetNegativeLimit(&dblNegative))
			{
				eReturn = AxlMotionSetFunction_ReadOnDatabaseError;
				break;
			}

			AxmSignalSetSoftLimit(_ttoi(GetSubUnitID()), bEnable, (DWORD)eStopMode, (DWORD)eCount, dblParam, dblNegative);
		}

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotion[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionGetFunction CDeviceAxlMotion::GetNegativeLimit(double* pParam)
{
	AxlMotionGetFunction eReturn = AxlMotionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = AxlMotionGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterAxlMotion_NegLimitValue));

		eReturn = AxlMotionGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionSetFunction CDeviceAxlMotion::SetNegativeLimit(double dblParam)
{
	AxlMotionSetFunction eReturn = AxlMotionSetFunction_UnknownError;

	EDeviceParameterAxlMotion eSaveID = EDeviceParameterAxlMotion_NegLimitValue;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			bool bEnable = false;

			EDeviceAxlMotionStopMode eStopMode = EDeviceAxlMotionStopMode_EmergencyStop;

			EDeviceAxlMotionCounterSelection eCount = EDeviceAxlMotionCounterSelection_Command;

			double dblPositive = 0.0;

			if(GetEnableSoftwareLimit(&bEnable))
			{
				eReturn = AxlMotionSetFunction_ReadOnDatabaseError;
				break;
			}

			if(GetSoftwareStopMode(&eStopMode))
			{
				eReturn = AxlMotionSetFunction_ReadOnDatabaseError;
				break;
			}

			if(GetSoftwareCounterSelection(&eCount))
			{
				eReturn = AxlMotionSetFunction_ReadOnDatabaseError;
				break;
			}

			if(GetPositiveLimit(&dblPositive))
			{
				eReturn = AxlMotionSetFunction_ReadOnDatabaseError;
				break;
			}

			AxmSignalSetSoftLimit(_ttoi(GetSubUnitID()), bEnable, (DWORD)eStopMode, (DWORD)eCount, dblPositive, dblParam);
		}

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotion[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionGetFunction CDeviceAxlMotion::GetHomeSearchSensor(_Out_ EDeviceAxlMotionSearchSensor* pParam)
{
	AxlMotionGetFunction eReturn = AxlMotionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = AxlMotionGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxlMotionSearchSensor)_ttoi(GetParamValue(EDeviceParameterAxlMotion_HomeSearchSensor));

		eReturn = AxlMotionGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionSetFunction CDeviceAxlMotion::SetHomeSearchSensor(_In_ EDeviceAxlMotionSearchSensor eParam)
{
	AxlMotionSetFunction eReturn = AxlMotionSetFunction_UnknownError;

	EDeviceParameterAxlMotion eSaveID = EDeviceParameterAxlMotion_HomeSearchSensor;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxlMotionSearchSensor_Count)
		{
			eReturn = AxlMotionSetFunction_NotFoundCommandError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotion[eSaveID], g_lpszAxlMotionSearchSensor[nPreValue], g_lpszAxlMotionSearchSensor[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}


AxlMotionGetFunction CDeviceAxlMotion::GetHomeDirection(_Out_ EDeviceAxlMotionDirection* pParam)
{
	AxlMotionGetFunction eReturn = AxlMotionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = AxlMotionGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxlMotionDirection)_ttoi(GetParamValue(EDeviceParameterAxlMotion_HomeDirection));

		eReturn = AxlMotionGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionSetFunction CDeviceAxlMotion::SetHomeDirection(_In_ EDeviceAxlMotionDirection eParam)
{
	AxlMotionSetFunction eReturn = AxlMotionSetFunction_UnknownError;

	EDeviceParameterAxlMotion eSaveID = EDeviceParameterAxlMotion_HomeDirection;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxlMotionDirection_Count)
		{
			eReturn = AxlMotionSetFunction_NotFoundCommandError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotion[eSaveID], g_lpszAxlMotionDirection[nPreValue], g_lpszAxlMotionDirection[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionGetFunction CDeviceAxlMotion::GetHomeZphase(_Out_ EDeviceAxlMotionZPhase* pParam)
{
	AxlMotionGetFunction eReturn = AxlMotionGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = AxlMotionGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxlMotionZPhase)_ttoi(GetParamValue(EDeviceParameterAxlMotion_HomeZphase));

		eReturn = AxlMotionGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionSetFunction CDeviceAxlMotion::SetHomeZphase(_In_ EDeviceAxlMotionZPhase eParam)
{
	AxlMotionSetFunction eReturn = AxlMotionSetFunction_UnknownError;

	EDeviceParameterAxlMotion eSaveID = EDeviceParameterAxlMotion_HomeZphase;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxlMotionZPhase_Count)
		{
			eReturn = AxlMotionSetFunction_NotFoundCommandError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotion[eSaveID], g_lpszAxlMotionZPhase[nPreValue], g_lpszAxlMotionZPhase[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionGetFunction CDeviceAxlMotion::GetHomeClearTime(_Out_ double* pData)
{
	AxlMotionGetFunction eReturn = AxlMotionGetFunction_UnknownError;

	do
	{
		if(!pData)
		{
			eReturn = AxlMotionGetFunction_NullptrError;
			break;
		}

		*pData = _ttof(GetParamValue(EDeviceParameterAxlMotion_HomeHomeClearTime));

		eReturn = AxlMotionGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionSetFunction CDeviceAxlMotion::SetHomeClearTime(_In_ double dblData)
{
	AxlMotionSetFunction eReturn = AxlMotionSetFunction_UnknownError;

	EDeviceParameterAxlMotion eSaveID = EDeviceParameterAxlMotion_HomeHomeClearTime;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%f"), dblData);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotion[eSaveID], dblPreValue, dblData);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionGetFunction CDeviceAxlMotion::GetHomeOffset(_Out_ double* pData)
{
	AxlMotionGetFunction eReturn = AxlMotionGetFunction_UnknownError;

	do
	{
		if(!pData)
		{
			eReturn = AxlMotionGetFunction_NullptrError;
			break;
		}

		*pData = _ttof(GetParamValue(EDeviceParameterAxlMotion_HomeOffset));

		eReturn = AxlMotionGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionSetFunction CDeviceAxlMotion::SetHomeOffset(_In_ double dblData)
{
	AxlMotionSetFunction eReturn = AxlMotionSetFunction_UnknownError;

	EDeviceParameterAxlMotion eSaveID = EDeviceParameterAxlMotion_HomeOffset;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%f"), dblData);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotion[eSaveID], dblPreValue, dblData);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionGetFunction CDeviceAxlMotion::GetHomeFirstVelocity(_Out_ double* pData)
{
	AxlMotionGetFunction eReturn = AxlMotionGetFunction_UnknownError;

	do
	{
		if(!pData)
		{
			eReturn = AxlMotionGetFunction_NullptrError;
			break;
		}

		*pData = _ttof(GetParamValue(EDeviceParameterAxlMotion_HomeFirstVel));

		eReturn = AxlMotionGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionSetFunction CDeviceAxlMotion::SetHomeFirstVelocity(_In_ double dblData)
{
	AxlMotionSetFunction eReturn = AxlMotionSetFunction_UnknownError;

	EDeviceParameterAxlMotion eSaveID = EDeviceParameterAxlMotion_HomeFirstVel;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%f"), dblData);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotion[eSaveID], dblPreValue, dblData);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionGetFunction CDeviceAxlMotion::GetHomeSecondVelocity(_Out_ double* pData)
{
	AxlMotionGetFunction eReturn = AxlMotionGetFunction_UnknownError;

	do
	{
		if(!pData)
		{
			eReturn = AxlMotionGetFunction_NullptrError;
			break;
		}

		*pData = _ttof(GetParamValue(EDeviceParameterAxlMotion_HomeSecondVel));

		eReturn = AxlMotionGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionSetFunction CDeviceAxlMotion::SetHomeSecondVelocity(_In_ double dblData)
{
	AxlMotionSetFunction eReturn = AxlMotionSetFunction_UnknownError;

	EDeviceParameterAxlMotion eSaveID = EDeviceParameterAxlMotion_HomeSecondVel;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%f"), dblData);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotion[eSaveID], dblPreValue, dblData);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionGetFunction CDeviceAxlMotion::GetHomeThirdVelocity(_Out_ double* pData)
{
	AxlMotionGetFunction eReturn = AxlMotionGetFunction_UnknownError;

	do
	{
		if(!pData)
		{
			eReturn = AxlMotionGetFunction_NullptrError;
			break;
		}

		*pData = _ttof(GetParamValue(EDeviceParameterAxlMotion_HomeThirdVel));

		eReturn = AxlMotionGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionSetFunction CDeviceAxlMotion::SetHomeThirdVelocity(_In_ double dblData)
{
	AxlMotionSetFunction eReturn = AxlMotionSetFunction_UnknownError;

	EDeviceParameterAxlMotion eSaveID = EDeviceParameterAxlMotion_HomeThirdVel;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%f"), dblData);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotion[eSaveID], dblPreValue, dblData);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionGetFunction CDeviceAxlMotion::GetHomeLastVelocity(_Out_ double* pData)
{
	AxlMotionGetFunction eReturn = AxlMotionGetFunction_UnknownError;

	do
	{
		if(!pData)
		{
			eReturn = AxlMotionGetFunction_NullptrError;
			break;
		}

		*pData = _ttof(GetParamValue(EDeviceParameterAxlMotion_HomeLastVel));

		eReturn = AxlMotionGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionSetFunction CDeviceAxlMotion::SetHomeLastVelocity(_In_ double dblData)
{
	AxlMotionSetFunction eReturn = AxlMotionSetFunction_UnknownError;

	EDeviceParameterAxlMotion eSaveID = EDeviceParameterAxlMotion_HomeLastVel;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%f"), dblData);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotion[eSaveID], dblPreValue, dblData);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionGetFunction CDeviceAxlMotion::GetHomeStartAcc(_Out_ double* pData)
{
	AxlMotionGetFunction eReturn = AxlMotionGetFunction_UnknownError;

	do
	{
		if(!pData)
		{
			eReturn = AxlMotionGetFunction_NullptrError;
			break;
		}

		*pData = _ttof(GetParamValue(EDeviceParameterAxlMotion_HomeStartAcc));

		eReturn = AxlMotionGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionSetFunction CDeviceAxlMotion::SetHomeStartAcc(_In_ double dblData)
{
	AxlMotionSetFunction eReturn = AxlMotionSetFunction_UnknownError;

	EDeviceParameterAxlMotion eSaveID = EDeviceParameterAxlMotion_HomeStartAcc;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%f"), dblData);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotion[eSaveID], dblPreValue, dblData);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionGetFunction CDeviceAxlMotion::GetHomeEndAcc(_Out_ double* pData)
{
	AxlMotionGetFunction eReturn = AxlMotionGetFunction_UnknownError;

	do
	{
		if(!pData)
		{
			eReturn = AxlMotionGetFunction_NullptrError;
			break;
		}

		*pData = _ttof(GetParamValue(EDeviceParameterAxlMotion_HomeEndAcc));

		eReturn = AxlMotionGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionSetFunction CDeviceAxlMotion::SetHomeEndAcc(_In_ double dblData)
{
	AxlMotionSetFunction eReturn = AxlMotionSetFunction_UnknownError;

	EDeviceParameterAxlMotion eSaveID = EDeviceParameterAxlMotion_HomeEndAcc;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%f"), dblData);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotion[eSaveID], dblPreValue, dblData);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceAxlMotion::GetServoStatus()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		DWORD dwStatus = 0;

		int nModuleID = _ttoi(GetSubUnitID());

		AxmSignalIsServoOn(nModuleID, &dwStatus);

		bReturn = (dwStatus == 1) ? true : false;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxlMotion::SetServoOn(bool bOn)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		int nModuleID = _ttoi(GetSubUnitID());

		DWORD dwServoLevel = 0;

		AxmSignalGetServoOnLevel(nModuleID, &dwServoLevel);

		if(!dwServoLevel)
			bOn = !bOn;

		bReturn = (AxmSignalServoOn(nModuleID, bOn) == AXT_RT_SUCCESS) ? true : false;

		if(bReturn)
			m_bServo = bOn;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxlMotion::GetCommandPosition(double* pPos)
{
	bool bReturn = false;

	do
	{
		if(!pPos)
			break;

		if(!IsInitialized())
			break;

		int nModuleID = _ttoi(GetSubUnitID());

		DWORD dwRtn = AxmStatusGetCmdPos(nModuleID, pPos);

		bReturn = (dwRtn == 0) ? true : false;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxlMotion::SetCommandPositionClear()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		int nModuleID = _ttoi(GetSubUnitID());

		DWORD dwRtn = AxmStatusSetCmdPos(nModuleID, 0.0f);

		bReturn = (dwRtn == 0) ? true : false;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxlMotion::GetActualPosition(double * pPos)
{
	bool bReturn = false;

	do
	{
		if(!pPos)
			break;

		if(!IsInitialized())
			break;

		int nModuleID = _ttoi(GetSubUnitID());

		DWORD dwRtn = AxmStatusGetActPos(nModuleID, pPos);

		bReturn = (dwRtn == 0) ? true : false;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxlMotion::SetActualPositionClear()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		int nModuleID = _ttoi(GetSubUnitID());

		DWORD dwRtn = AxmStatusSetActPos(nModuleID, 0.0f);

		bReturn = (dwRtn == 0) ? true : false;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxlMotion::GetAlarmStatus()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		int nModuleID = _ttoi(GetSubUnitID());

		DWORD dwStatus = 0;

		AxmSignalReadServoAlarm(nModuleID, &dwStatus);

		bReturn = (dwStatus == 1) ? true : false;

	}
	while(false);

	return bReturn;
}

bool CDeviceAxlMotion::SetAlarmClear()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		int nModuleID = _ttoi(GetSubUnitID());

		DWORD dwRtn = 0;

		DWORD dwResetLevel = 0;

		AxmSignalGetServoAlarmResetLevel(nModuleID, &dwResetLevel);

		AxmSignalReadOutputBit(nModuleID, 1, &dwRtn);

		bool bEdge = dwResetLevel;

		AxmSignalServoAlarmReset(nModuleID, !bEdge);

		Sleep(100);

		AxmSignalServoAlarmReset(nModuleID, bEdge);

		Sleep(100);

		AxmSignalServoAlarmReset(nModuleID, !bEdge);

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxlMotion::GetInposition()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		DWORD dwStatus = 0;

		AxmSignalReadInpos(_ttoi(GetSubUnitID()), &dwStatus);

		bReturn = (dwStatus == 1) ? true : false;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxlMotion::GetLimitSensorN()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		DWORD dwRtn = 0;

		DWORD dwP = 0, dwN = 0;

		dwRtn = AxmSignalReadLimit(_ttoi(GetSubUnitID()), &dwP, &dwN);

		bReturn = (dwN == 1) ? true : false;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxlMotion::GetLimitSensorP()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		DWORD dwRtn = 0;

		DWORD dwP = 0, dwN = 0;

		dwRtn = AxmSignalReadLimit(_ttoi(GetSubUnitID()), &dwP, &dwN);

		bReturn = (dwP == 1) ? true : false;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxlMotion::GetHomeSensor()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		DWORD dwStatus = 0;

		AxmHomeReadSignal(_ttoi(GetSubUnitID()), &dwStatus);

		bReturn = (dwStatus == 1) ? true : false;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxlMotion::MovePosition(double dPos, double dVel, double dAcc, bool bSCurve)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		int nModuleID = _ttoi(GetSubUnitID());

		m_dblLastVelocity = dAcc;

		DWORD dwRtn = 0;

		dwRtn = AxmMotSetAbsRelMode(nModuleID, POS_ABS_MODE);

		if(dwRtn != AXT_RT_SUCCESS)
			break;

		if(!bSCurve)
			dwRtn = AxmMoveStartPos(nModuleID, dPos, dVel, dAcc, dAcc);
		else
			dwRtn = AxmMoveStartPos(nModuleID, dPos, dVel, dAcc, dAcc);

		bReturn = (dwRtn == AXT_RT_SUCCESS) ? true : false;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxlMotion::MoveDistance(double dDist, double dVel, double dAcc, bool bSCurve)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		int nModuleID = _ttoi(GetSubUnitID());

		m_dblLastVelocity = dAcc;

		DWORD dwRtn = 0;

		dwRtn = AxmMotSetAbsRelMode(nModuleID, POS_REL_MODE);

		if(dwRtn != AXT_RT_SUCCESS)
			break;

		if(!bSCurve)
			dwRtn = AxmMoveStartPos(nModuleID, dDist, dVel, dAcc, dAcc);
		else
			dwRtn = AxmMoveStartPos(nModuleID, dDist, dVel, dAcc, dAcc);

		bReturn = (dwRtn == AXT_RT_SUCCESS) ? true : false;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxlMotion::MoveVelocity(double dVel, double dAcc, bool bSCurve)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		int nModuleID = _ttoi(GetSubUnitID());

		m_dblLastVelocity = dAcc;

		DWORD dwRtn = 0;

		dwRtn = AxmMotSetAbsRelMode(nModuleID, POS_REL_MODE);

		if(dwRtn != AXT_RT_SUCCESS)
			break;

		if(!bSCurve)
			dwRtn = AxmMoveVel(nModuleID, dVel, dAcc, dAcc);
		else
			dwRtn = AxmMoveVel(nModuleID, dVel, dAcc, dAcc);

		bReturn = (dwRtn == 0) ? true : false;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxlMotion::MoveJog(double dVel, double dAcl, BOOL bSCurve)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		m_dblLastVelocity = dAcl;

		DWORD dwRtn = 0;

		if(!bSCurve)
			dwRtn = AxmMoveVel(_ttoi(GetSubUnitID()), dVel, dAcl, dAcl);
		else
			dwRtn = AxmMoveVel(_ttoi(GetSubUnitID()), dVel, dAcl, dAcl);

		bReturn = (dwRtn == 0) ? true : false;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxlMotion::StopJog()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		DWORD dwRtn = 0;

		dwRtn = AxmMoveStop(_ttoi(GetSubUnitID()), fabs(m_dblLastVelocity / 2));

		bReturn = (dwRtn == 0) ? true : false;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxlMotion::IsMotionDone()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		DWORD dwStatus = 0;

		AxmStatusReadInMotion(_ttoi(GetSubUnitID()), &dwStatus);

		bReturn = (dwStatus == 0) ? true : false;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxlMotion::MotorStop()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		DWORD dwRtn = 0;

		dwRtn = AxmMoveStop(_ttoi(GetSubUnitID()), fabs(m_dblLastVelocity));

		bReturn = (dwRtn == 0) ? true : false;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxlMotion::MotorEStop()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		DWORD dwRtn = 0;

		dwRtn = AxmMoveEStop(_ttoi(GetSubUnitID()));

		bReturn = (dwRtn == 0) ? true : false;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxlMotion::ChangeSpeed(double dSpeed)
{
	return false;
}

bool CDeviceAxlMotion::WriteGenOutBit(int nBit, bool bOn)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		DWORD dwRtn = 0;

		if(bOn)
			dwRtn = AxmSignalWriteOutputBit(_ttoi(GetSubUnitID()), nBit, TRUE);
		else
			dwRtn = AxmSignalWriteOutputBit(_ttoi(GetSubUnitID()), nBit, FALSE);

		bReturn = (dwRtn == 0) ? true : false;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxlMotion::ReadGenOutBit(int nBit)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		DWORD dwRtn = 0;
		DWORD dwStatus = 0;

		dwRtn = AxmSignalReadOutputBit(_ttoi(GetSubUnitID()), nBit, &dwStatus);

		bReturn = (dwStatus == 1) ? true : false;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxlMotion::ReadGenInputBit(int nBit)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		DWORD dwRtn = 0;
		DWORD dwStatus = 0;

		dwRtn = AxmSignalReadInputBit(_ttoi(GetSubUnitID()), nBit, &dwStatus);

		bReturn = (dwStatus == 1) ? true : false;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxlMotion::MoveToHome()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		DWORD dwRtn = 0;

		double dblFirstVel = 40.;
		double dblSecondVel = 20.;
		double dblThirdVel = 5.;
		double dblLastVel = 2.;

		double dblStartAcc = 80.;
		double dblEndAcc = 40.;

		double dblHomeClrTime = 1000.;
		double dblHomeOffset = 0.;

		EDeviceAxlMotionZPhase eZphase = EDeviceAxlMotionZPhase_Count;
		EDeviceAxlMotionDirection eDir = EDeviceAxlMotionDirection_Count;
		EDeviceAxlMotionSearchSensor eSensor = EDeviceAxlMotionSearchSensor_Count;

		if(GetHomeZphase(&eZphase))
			break;

		if(GetHomeDirection(&eDir))
			break;

		if(GetHomeSearchSensor(&eSensor))
			break;

		if(GetHomeOffset(&dblHomeOffset))
			break;

		if(GetHomeClearTime(&dblHomeClrTime))
			break;

		if(GetHomeFirstVelocity(&dblFirstVel))
			break;

		if(GetHomeSecondVelocity(&dblSecondVel))
			break;

		if(GetHomeThirdVelocity(&dblThirdVel))
			break;

		if(GetHomeLastVelocity(&dblLastVel))
			break;

		if(GetHomeStartAcc(&dblStartAcc))
			break;

		if(GetHomeEndAcc(&dblEndAcc))
			break;

		long nHmDir = (long)eDir;

		DWORD uZphase = (DWORD)eZphase;

		DWORD uHomeSignal = eSensor == EDeviceAxlMotionSearchSensor_HomeSensor ? HomeSensor : eSensor;

		dwRtn += AxmHomeSetMethod(_ttoi(GetSubUnitID()), nHmDir, uHomeSignal, uZphase, dblHomeClrTime, dblHomeOffset);

		dwRtn += AxmHomeSetVel(_ttoi(GetSubUnitID()), dblFirstVel, dblSecondVel, dblThirdVel, dblLastVel, dblStartAcc, dblEndAcc);

		dwRtn += AxmHomeSetStart(_ttoi(GetSubUnitID()));

		bReturn = (dwRtn == 0) ? true : false;
	}
	while(false);

	return bReturn;
}

EDeviceMotionHommingStatus CDeviceAxlMotion::GetHommingStatus()
{
	EDeviceMotionHommingStatus eStatus = EDeviceMotionHommingStatus_Error;

	do
	{
		if(!IsInitialized())
			break;

		DWORD dwHomeResult = 0;

		AxmHomeGetResult(_ttoi(GetSubUnitID()), &dwHomeResult);

		switch(dwHomeResult)
		{
		case HOME_SUCCESS:
			eStatus = EDeviceMotionHommingStatus_Success;
			break;
		case HOME_SEARCHING:
			eStatus = EDeviceMotionHommingStatus_Searching;
			break;
		default:
			eStatus = EDeviceMotionHommingStatus_Error;
			break;
		}
	}
	while(false);

	return eStatus;
}

bool CDeviceAxlMotion::MoveToSignal(double dblVel, double dblAcc, ESearchSignal eDetectSignal, ESearchEdge eSignalEdge, ESearchMethod eSignalMethod)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		bReturn = AxmMoveSignalSearch(_ttoi(GetSubUnitID()), dblVel, dblAcc, eDetectSignal, eSignalEdge, eSignalMethod);
	}
	while(false);

	return bReturn;
}

bool CDeviceAxlMotion::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = true;

	bool bFoundID = true;

	do
	{
		switch(nParam)
		{
		case EDeviceParameterAxlMotion_DeviceID:
			bReturn = !SetDeviceID(strValue);
			break;
		case EDeviceParameterAxlMotion_SubUnitID:
			bReturn = !SetSubUnitID(strValue);
			break;
		case EDeviceParameterAxlMotion_UnitPulse:
			bReturn = !SetUnitPulse(_ttof(strValue));
			break;
		case EDeviceParameterAxlMotion_StartSpeed:
			bReturn = !SetStartSpeed(_ttof(strValue));
			break;
		case EDeviceParameterAxlMotion_MaxSpeed:
			bReturn = !SetMaxSpeed(_ttoi(strValue));
			break;
		case EDeviceParameterAxlMotion_PulseOutputMethod:
			bReturn = !SetPulseOutputMethod((EDeviceAxlMotionPulseOutputMethod)_ttoi(strValue));
			break;
		case EDeviceParameterAxlMotion_EncoderMethod:
			bReturn = !SetEncoderMethod((EDeviceAxlMotionEncoderMethod)_ttoi(strValue));
			break;
		case EDeviceParameterAxlMotion_ServoLevel:
			bReturn = !SetServoLevel((EDeviceAxlMotionLevelMethod)_ttoi(strValue));
			break;
		case EDeviceParameterAxlMotion_HomeLevel:
			bReturn = !SetHomeLevel((EDeviceAxlMotionLevelMethod)_ttoi(strValue));
			break;
		case EDeviceParameterAxlMotion_PLimitLevel:
			bReturn = !SetPLimitLevel((EDeviceAxlMotionLevelMethod)_ttoi(strValue));
			break;
		case EDeviceParameterAxlMotion_NLimitLevel:
			bReturn = !SetNLimitLevel((EDeviceAxlMotionLevelMethod)_ttoi(strValue));
			break;
		case EDeviceParameterAxlMotion_AlarmInputLevel:
			bReturn = !SetAlarmInputLevel((EDeviceAxlMotionLevelMethod)_ttoi(strValue));
			break;
		case EDeviceParameterAxlMotion_AlarmResetLevel:
			bReturn = !SetAlarmResetLevel((EDeviceAxlMotionLevelMethod)_ttoi(strValue));
			break;
		case EDeviceParameterAxlMotion_InpositionInputLevel:
			bReturn = !SetInpositionInputLevel((EDeviceAxlMotionLevelMethod)_ttoi(strValue));
			break;
		case EDeviceParameterAxlMotion_UseSoftwareLimit:
			bReturn = !SetEnableSoftwareLimit((bool)_ttoi(strValue));
			break;
		case EDeviceParameterAxlMotion_StopMode:
			bReturn = !SetSoftwareStopMode((EDeviceAxlMotionStopMode)_ttoi(strValue));
			break;
		case EDeviceParameterAxlMotion_CounterSelection:
			bReturn = !SetSoftwareCounterSelection((EDeviceAxlMotionCounterSelection)_ttoi(strValue));
			break;
		case EDeviceParameterAxlMotion_PosLimitValue:
			bReturn = !SetPositiveLimit(_ttof(strValue));
			break;
		case EDeviceParameterAxlMotion_NegLimitValue:
			bReturn = !SetNegativeLimit(_ttof(strValue));
			break;
		case EDeviceParameterAxlMotion_HomeSearchSensor:
			bReturn = !SetHomeSearchSensor((EDeviceAxlMotionSearchSensor)_ttoi(strValue));
			break;
		case EDeviceParameterAxlMotion_HomeDirection:
			bReturn = !SetHomeDirection((EDeviceAxlMotionDirection)_ttoi(strValue));
			break;
		case EDeviceParameterAxlMotion_HomeZphase:
			bReturn = !SetHomeZphase((EDeviceAxlMotionZPhase)_ttoi(strValue));
			break;
		case EDeviceParameterAxlMotion_HomeHomeClearTime:
			bReturn = !SetHomeClearTime(_ttof(strValue));
			break;
		case EDeviceParameterAxlMotion_HomeOffset:
			bReturn = !SetHomeOffset(_ttof(strValue));
			break;
		case EDeviceParameterAxlMotion_HomeFirstVel:
			bReturn = !SetHomeFirstVelocity(_ttof(strValue));
			break;
		case EDeviceParameterAxlMotion_HomeSecondVel:
			bReturn = !SetHomeSecondVelocity(_ttof(strValue));
			break;
		case EDeviceParameterAxlMotion_HomeThirdVel:
			bReturn = !SetHomeThirdVelocity(_ttof(strValue));
			break;
		case EDeviceParameterAxlMotion_HomeLastVel:
			bReturn = !SetHomeLastVelocity(_ttof(strValue));
			break;
		case EDeviceParameterAxlMotion_HomeStartAcc:
			bReturn = !SetHomeStartAcc(_ttof(strValue));
			break;
		case EDeviceParameterAxlMotion_HomeEndAcc:
			bReturn = !SetHomeEndAcc(_ttof(strValue));
			break;
		case EDeviceParameterAxlMotion_OpenNoReset:
			bReturn = !SetOpenNoHardware(_ttoi(strValue));
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

bool CDeviceAxlMotion::DoesModuleExist()
{
	bool bReturn = false;

	CString strModuleName;

	do
	{
		strModuleName.Format(_T("AXL.dll"));

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

		if(!bReturn)
			break;

		strModuleName.Format(_T("EzBasicAxl.dll"));

		eStatus = CLibraryManager::GetFoundLibrary(strModuleName);

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


DWORD CDeviceAxlMotion::__AxmInfoGetAxis(long lAxisNo, long *lpBoardNo, long *lpModulePos, DWORD *upModuleID)
{
	return AxmInfoGetAxis(lAxisNo, lpBoardNo, lpModulePos, upModuleID);
}

DWORD CDeviceAxlMotion::__AxmInfoIsMotionModule(DWORD *upStatus)
{
	return AxmInfoIsMotionModule(upStatus);
}

DWORD CDeviceAxlMotion::__AxmInfoIsInvalidAxisNo(long lAxisNo)
{
	return AxmInfoIsInvalidAxisNo(lAxisNo);
}

DWORD CDeviceAxlMotion::__AxmInfoGetAxisStatus(long lAxisNo)
{
	return AxmInfoGetAxisStatus(lAxisNo);
}

DWORD CDeviceAxlMotion::__AxmInfoGetAxisCount(long *lpAxisCount)
{
	return AxmInfoGetAxisCount(lpAxisCount);
}

DWORD CDeviceAxlMotion::__AxmInfoGetFirstAxisNo(long lBoardNo, long lModulePos, long *lpAxisNo)
{
	return AxmInfoGetFirstAxisNo(lBoardNo, lModulePos, lpAxisNo);
}

DWORD CDeviceAxlMotion::__AxmInfoGetBoardFirstAxisNo(long lBoardNo, long lModulePos, long *lpAxisNo)
{
	return AxmInfoGetBoardFirstAxisNo(lBoardNo, lModulePos, lpAxisNo);
}


DWORD CDeviceAxlMotion::__AxmVirtualSetAxisNoMap(long lRealAxisNo, long lVirtualAxisNo)
{
	return AxmVirtualSetAxisNoMap(lRealAxisNo, lVirtualAxisNo);
}

DWORD CDeviceAxlMotion::__AxmVirtualGetAxisNoMap(long lRealAxisNo, long *lpVirtualAxisNo)
{
	return AxmVirtualGetAxisNoMap(lRealAxisNo, lpVirtualAxisNo);
}

DWORD CDeviceAxlMotion::__AxmVirtualSetMultiAxisNoMap(long lSize, long *lpRealAxesNo, long *lpVirtualAxesNo)
{
	return AxmVirtualSetMultiAxisNoMap(lSize, lpRealAxesNo, lpVirtualAxesNo);
}

DWORD CDeviceAxlMotion::__AxmVirtualGetMultiAxisNoMap(long lSize, long *lpRealAxesNo, long *lpVirtualAxesNo)
{
	return AxmVirtualGetMultiAxisNoMap(lSize, lpRealAxesNo, lpVirtualAxesNo);
}

DWORD CDeviceAxlMotion::__AxmVirtualResetAxisMap()
{
	return AxmVirtualResetAxisMap();
}


DWORD CDeviceAxlMotion::__AxmInterruptSetAxisEnable(long lAxisNo, DWORD uUse)
{
	return AxmInterruptSetAxisEnable(lAxisNo, uUse);
}

DWORD CDeviceAxlMotion::__AxmInterruptGetAxisEnable(long lAxisNo, DWORD *upUse)
{
	return AxmInterruptGetAxisEnable(lAxisNo, upUse);
}

DWORD CDeviceAxlMotion::__AxmInterruptRead(long *lpAxisNo, DWORD *upFlag)
{
	return AxmInterruptRead(lpAxisNo, upFlag);
}

DWORD CDeviceAxlMotion::__AxmInterruptReadAxisFlag(long lAxisNo, long lBank, DWORD *upFlag)
{
	return AxmInterruptReadAxisFlag(lAxisNo, lBank, upFlag);
}

DWORD CDeviceAxlMotion::__AxmInterruptSetUserEnable(long lAxisNo, long lBank, DWORD uInterruptNum)
{
	return AxmInterruptSetUserEnable(lAxisNo, lBank, uInterruptNum);
}

DWORD CDeviceAxlMotion::__AxmInterruptGetUserEnable(long lAxisNo, long lBank, DWORD *upInterruptNum)
{
	return AxmInterruptGetUserEnable(lAxisNo, lBank, upInterruptNum);
}

DWORD CDeviceAxlMotion::__AxmMotLoadParaAll(char *szFilePath)
{
	return AxmMotLoadParaAll(szFilePath);
}

DWORD CDeviceAxlMotion::__AxmMotSaveParaAll(char *szFilePath)
{
	return AxmMotSaveParaAll(szFilePath);
}

DWORD CDeviceAxlMotion::__AxmMotSetParaLoad(long lAxisNo, double dInitPos, double dInitVel, double dInitAccel, double dInitDecel)
{
	return AxmMotSetParaLoad(lAxisNo, dInitPos, dInitVel, dInitAccel, dInitDecel);
}

DWORD CDeviceAxlMotion::__AxmMotGetParaLoad(long lAxisNo, double *dpInitPos, double *dpInitVel, double *dpInitAccel, double *dpInitDecel)
{
	return AxmMotGetParaLoad(lAxisNo, dpInitPos, dpInitVel, dpInitAccel, dpInitDecel);
}

DWORD CDeviceAxlMotion::__AxmMotSetPulseOutMethod(long lAxisNo, DWORD uMethod)
{
	return AxmMotSetPulseOutMethod(lAxisNo, uMethod);
}

DWORD CDeviceAxlMotion::__AxmMotGetPulseOutMethod(long lAxisNo, DWORD *upMethod)
{
	return AxmMotGetPulseOutMethod(lAxisNo, upMethod);
}

DWORD CDeviceAxlMotion::__AxmMotSetEncInputMethod(long lAxisNo, DWORD uMethod)
{
	return AxmMotSetEncInputMethod(lAxisNo, uMethod);
}

DWORD CDeviceAxlMotion::__AxmMotGetEncInputMethod(long lAxisNo, DWORD *upMethod)
{
	return AxmMotGetEncInputMethod(lAxisNo, upMethod);
}

DWORD CDeviceAxlMotion::__AxmMotSetMoveUnitPerPulse(long lAxisNo, double dUnit, long lPulse)
{
	return AxmMotSetMoveUnitPerPulse(lAxisNo, dUnit, lPulse);
}

DWORD CDeviceAxlMotion::__AxmMotGetMoveUnitPerPulse(long lAxisNo, double *dpUnit, long *lpPulse)
{
	return AxmMotGetMoveUnitPerPulse(lAxisNo, dpUnit, lpPulse);
}

DWORD CDeviceAxlMotion::__AxmMotSetDecelMode(long lAxisNo, DWORD uMethod)
{
	return AxmMotSetDecelMode(lAxisNo, uMethod);
}

DWORD CDeviceAxlMotion::__AxmMotGetDecelMode(long lAxisNo, DWORD *upMethod)
{
	return AxmMotGetDecelMode(lAxisNo, upMethod);
}

DWORD CDeviceAxlMotion::__AxmMotSetRemainPulse(long lAxisNo, DWORD uData)
{
	return AxmMotSetRemainPulse(lAxisNo, uData);
}

DWORD CDeviceAxlMotion::__AxmMotGetRemainPulse(long lAxisNo, DWORD *upData)
{
	return AxmMotGetRemainPulse(lAxisNo, upData);
}

DWORD CDeviceAxlMotion::__AxmMotSetMaxVel(long lAxisNo, double dVel)
{
	return AxmMotSetMaxVel(lAxisNo, dVel);
}

DWORD CDeviceAxlMotion::__AxmMotGetMaxVel(long lAxisNo, double *dpVel)
{
	return AxmMotGetMaxVel(lAxisNo, dpVel);
}

DWORD CDeviceAxlMotion::__AxmMotSetAbsRelMode(long lAxisNo, DWORD uAbsRelMode)
{
	return AxmMotSetAbsRelMode(lAxisNo, uAbsRelMode);
}

DWORD CDeviceAxlMotion::__AxmMotGetAbsRelMode(long lAxisNo, DWORD *upAbsRelMode)
{
	return AxmMotGetAbsRelMode(lAxisNo, upAbsRelMode);
}

DWORD CDeviceAxlMotion::__AxmMotSetProfileMode(long lAxisNo, DWORD uProfileMode)
{
	return AxmMotSetProfileMode(lAxisNo, uProfileMode);
}

DWORD CDeviceAxlMotion::__AxmMotGetProfileMode(long lAxisNo, DWORD *upProfileMode)
{
	return AxmMotGetProfileMode(lAxisNo, upProfileMode);
}

DWORD CDeviceAxlMotion::__AxmMotSetAccelUnit(long lAxisNo, DWORD uAccelUnit)
{
	return AxmMotSetAccelUnit(lAxisNo, uAccelUnit);
}

DWORD CDeviceAxlMotion::__AxmMotGetAccelUnit(long lAxisNo, DWORD *upAccelUnit)
{
	return AxmMotGetAccelUnit(lAxisNo, upAccelUnit);
}

DWORD CDeviceAxlMotion::__AxmMotSetMinVel(long lAxisNo, double dMinVel)
{
	return AxmMotSetMinVel(lAxisNo, dMinVel);
}

DWORD CDeviceAxlMotion::__AxmMotGetMinVel(long lAxisNo, double *dpMinVel)
{
	return AxmMotGetMinVel(lAxisNo, dpMinVel);
}

DWORD CDeviceAxlMotion::__AxmMotSetAccelJerk(long lAxisNo, double dAccelJerk)
{
	return AxmMotSetAccelJerk(lAxisNo, dAccelJerk);
}

DWORD CDeviceAxlMotion::__AxmMotGetAccelJerk(long lAxisNo, double *dpAccelJerk)
{
	return AxmMotGetAccelJerk(lAxisNo, dpAccelJerk);
}

DWORD CDeviceAxlMotion::__AxmMotSetDecelJerk(long lAxisNo, double dDecelJerk)
{
	return AxmMotSetDecelJerk(lAxisNo, dDecelJerk);
}

DWORD CDeviceAxlMotion::__AxmMotGetDecelJerk(long lAxisNo, double *dpDecelJerk)
{
	return AxmMotGetDecelJerk(lAxisNo, dpDecelJerk);
}

DWORD CDeviceAxlMotion::__AxmMotSetProfilePriority(long lAxisNo, DWORD uPriority)
{
	return AxmMotSetProfilePriority(lAxisNo, uPriority);
}

DWORD CDeviceAxlMotion::__AxmMotGetProfilePriority(long lAxisNo, DWORD *upPriority)
{
	return AxmMotGetProfilePriority(lAxisNo, upPriority);
}

DWORD CDeviceAxlMotion::__AxmSignalSetZphaseLevel(long lAxisNo, DWORD uLevel)
{
	return AxmSignalSetZphaseLevel(lAxisNo, uLevel);
}

DWORD CDeviceAxlMotion::__AxmSignalGetZphaseLevel(long lAxisNo, DWORD *upLevel)
{
	return AxmSignalGetZphaseLevel(lAxisNo, upLevel);
}

DWORD CDeviceAxlMotion::__AxmSignalSetServoOnLevel(long lAxisNo, DWORD uLevel)
{
	return AxmSignalSetServoOnLevel(lAxisNo, uLevel);
}

DWORD CDeviceAxlMotion::__AxmSignalGetServoOnLevel(long lAxisNo, DWORD *upLevel)
{
	return AxmSignalGetServoOnLevel(lAxisNo, upLevel);
}

DWORD CDeviceAxlMotion::__AxmSignalSetServoAlarmResetLevel(long lAxisNo, DWORD uLevel)
{
	return AxmSignalSetServoAlarmResetLevel(lAxisNo, uLevel);
}

DWORD CDeviceAxlMotion::__AxmSignalGetServoAlarmResetLevel(long lAxisNo, DWORD *upLevel)
{
	return AxmSignalGetServoAlarmResetLevel(lAxisNo, upLevel);
}

DWORD CDeviceAxlMotion::__AxmSignalSetInpos(long lAxisNo, DWORD uUse)
{
	return AxmSignalSetInpos(lAxisNo, uUse);
}

DWORD CDeviceAxlMotion::__AxmSignalGetInpos(long lAxisNo, DWORD *upUse)
{
	return AxmSignalGetInpos(lAxisNo, upUse);
}

DWORD CDeviceAxlMotion::__AxmSignalReadInpos(long lAxisNo, DWORD *upStatus)
{
	return AxmSignalReadInpos(lAxisNo, upStatus);
}

DWORD CDeviceAxlMotion::__AxmSignalSetServoAlarm(long lAxisNo, DWORD uUse)
{
	return AxmSignalSetServoAlarm(lAxisNo, uUse);
}

DWORD CDeviceAxlMotion::__AxmSignalGetServoAlarm(long lAxisNo, DWORD *upUse)
{
	return AxmSignalGetServoAlarm(lAxisNo, upUse);
}

DWORD CDeviceAxlMotion::__AxmSignalReadServoAlarm(long lAxisNo, DWORD *upStatus)
{
	return AxmSignalReadServoAlarm(lAxisNo, upStatus);
}

DWORD CDeviceAxlMotion::__AxmSignalSetLimit(long lAxisNo, DWORD uStopMode, DWORD uPositiveLevel, DWORD uNegativeLevel)
{
	return AxmSignalSetLimit(lAxisNo, uStopMode, uPositiveLevel, uNegativeLevel);
}

DWORD CDeviceAxlMotion::__AxmSignalGetLimit(long lAxisNo, DWORD *upStopMode, DWORD *upPositiveLevel, DWORD *upNegativeLevel)
{
	return AxmSignalGetLimit(lAxisNo, upStopMode, upPositiveLevel, upNegativeLevel);
}

DWORD CDeviceAxlMotion::__AxmSignalReadLimit(long lAxisNo, DWORD *upPositiveStatus, DWORD *upNegativeStatus)
{
	return AxmSignalReadLimit(lAxisNo, upPositiveStatus, upNegativeStatus);
}

DWORD CDeviceAxlMotion::__AxmSignalSetSoftLimit(long lAxisNo, DWORD uUse, DWORD uStopMode, DWORD uSelection, double dPositivePos, double dNegativePos)
{
	return AxmSignalSetSoftLimit(lAxisNo, uUse, uStopMode, uSelection, dPositivePos, dNegativePos);
}

DWORD CDeviceAxlMotion::__AxmSignalGetSoftLimit(long lAxisNo, DWORD *upUse, DWORD *upStopMode, DWORD *upSelection, double *dpPositivePos, double *dpNegativePos)
{
	return AxmSignalGetSoftLimit(lAxisNo, upUse, upStopMode, upSelection, dpPositivePos, dpNegativePos);
}


DWORD CDeviceAxlMotion::__AxmSignalReadSoftLimit(long lAxisNo, DWORD *upPositiveStatus, DWORD *upNegativeStatus)
{
	return AxmSignalReadSoftLimit(lAxisNo, upPositiveStatus, upNegativeStatus);
}

DWORD CDeviceAxlMotion::__AxmSignalSetStop(long lAxisNo, DWORD uStopMode, DWORD uLevel)
{
	return AxmSignalSetStop(lAxisNo, uStopMode, uLevel);
}

DWORD CDeviceAxlMotion::__AxmSignalGetStop(long lAxisNo, DWORD *upStopMode, DWORD *upLevel)
{
	return AxmSignalGetStop(lAxisNo, upStopMode, upLevel);
}

DWORD CDeviceAxlMotion::__AxmSignalReadStop(long lAxisNo, DWORD *upStatus)
{
	return AxmSignalReadStop(lAxisNo, upStatus);
}

DWORD CDeviceAxlMotion::__AxmSignalServoOn(long lAxisNo, DWORD uOnOff)
{
	return AxmSignalServoOn(lAxisNo, uOnOff);
}

DWORD CDeviceAxlMotion::__AxmSignalIsServoOn(long lAxisNo, DWORD *upOnOff)
{
	return AxmSignalIsServoOn(lAxisNo, upOnOff);
}

DWORD CDeviceAxlMotion::__AxmSignalServoAlarmReset(long lAxisNo, DWORD uOnOff)
{
	return AxmSignalServoAlarmReset(lAxisNo, uOnOff);
}

DWORD CDeviceAxlMotion::__AxmSignalWriteOutput(long lAxisNo, DWORD uValue)
{
	return AxmSignalWriteOutput(lAxisNo, uValue);
}

DWORD CDeviceAxlMotion::__AxmSignalReadOutput(long lAxisNo, DWORD *upValue)
{
	return AxmSignalReadOutput(lAxisNo, upValue);
}

DWORD CDeviceAxlMotion::__AxmSignalReadBrakeOn(long lAxisNo, DWORD *upOnOff)
{
	return AxmSignalReadBrakeOn(lAxisNo, upOnOff);
}

DWORD CDeviceAxlMotion::__AxmSignalWriteOutputBit(long lAxisNo, long lBitNo, DWORD uOnOff)
{
	return AxmSignalWriteOutputBit(lAxisNo, lBitNo, uOnOff);
}

DWORD CDeviceAxlMotion::__AxmSignalReadOutputBit(long lAxisNo, long lBitNo, DWORD *upOnOff)
{
	return AxmSignalReadOutputBit(lAxisNo, lBitNo, upOnOff);
}

DWORD CDeviceAxlMotion::__AxmSignalReadInput(long lAxisNo, DWORD *upValue)
{
	return AxmSignalReadInput(lAxisNo, upValue);
}

DWORD CDeviceAxlMotion::__AxmSignalReadInputBit(long lAxisNo, long lBitNo, DWORD *upOn)
{
	return AxmSignalReadInputBit(lAxisNo, lBitNo, upOn);
}

DWORD CDeviceAxlMotion::__AxmSignalSetFilterBandwidth(long lAxisNo, DWORD uSignal, double dBandwidthUsec)
{
	return AxmSignalSetFilterBandwidth(lAxisNo, uSignal, dBandwidthUsec);
}

DWORD CDeviceAxlMotion::__AxmStatusReadInMotion(long lAxisNo, DWORD *upStatus)
{
	return AxmStatusReadInMotion(lAxisNo, upStatus);
}

DWORD CDeviceAxlMotion::__AxmStatusReadDrivePulseCount(long lAxisNo, long *lpPulse)
{
	return AxmStatusReadDrivePulseCount(lAxisNo, lpPulse);
}

DWORD CDeviceAxlMotion::__AxmStatusReadMotion(long lAxisNo, DWORD *upStatus)
{
	return AxmStatusReadMotion(lAxisNo, upStatus);
}

DWORD CDeviceAxlMotion::__AxmStatusReadStop(long lAxisNo, DWORD *upStatus)
{
	return AxmStatusReadStop(lAxisNo, upStatus);
}

DWORD CDeviceAxlMotion::__AxmStatusReadMechanical(long lAxisNo, DWORD *upStatus)
{
	return AxmStatusReadMechanical(lAxisNo, upStatus);
}

DWORD CDeviceAxlMotion::__AxmStatusReadVel(long lAxisNo, double *dpVel)
{
	return AxmStatusReadVel(lAxisNo, dpVel);
}

DWORD CDeviceAxlMotion::__AxmStatusReadPosError(long lAxisNo, double *dpError)
{
	return AxmStatusReadPosError(lAxisNo, dpError);
}

DWORD CDeviceAxlMotion::__AxmStatusReadDriveDistance(long lAxisNo, double *dpUnit)
{
	return AxmStatusReadDriveDistance(lAxisNo, dpUnit);
}

DWORD CDeviceAxlMotion::__AxmStatusSetPosType(long lAxisNo, DWORD uPosType, double dPositivePos, double dNegativePos)
{
	return AxmStatusSetPosType(lAxisNo, uPosType, dPositivePos, dNegativePos);
}

DWORD CDeviceAxlMotion::__AxmStatusGetPosType(long lAxisNo, DWORD *upPosType, double *dpPositivePos, double *dpNegativePos)
{
	return AxmStatusGetPosType(lAxisNo, upPosType, dpPositivePos, dpNegativePos);
}

DWORD CDeviceAxlMotion::__AxmStatusSetAbsOrgOffset(long lAxisNo, double dOrgOffsetPos)
{
	return AxmStatusSetAbsOrgOffset(lAxisNo, dOrgOffsetPos);
}

DWORD CDeviceAxlMotion::__AxmStatusSetActPos(long lAxisNo, double dPos)
{
	return AxmStatusSetActPos(lAxisNo, dPos);
}

DWORD CDeviceAxlMotion::__AxmStatusGetActPos(long lAxisNo, double *dpPos)
{
	return AxmStatusGetActPos(lAxisNo, dpPos);
}

DWORD CDeviceAxlMotion::__AxmStatusSetCmdPos(long lAxisNo, double dPos)
{
	return AxmStatusSetCmdPos(lAxisNo, dPos);
}

DWORD CDeviceAxlMotion::__AxmStatusGetCmdPos(long lAxisNo, double *dpPos)
{
	return AxmStatusGetCmdPos(lAxisNo, dpPos);
}

DWORD CDeviceAxlMotion::__AxmStatusSetPosMatch(long lAxisNo, double dPos)
{
	return AxmStatusSetPosMatch(lAxisNo, dPos);
}

DWORD CDeviceAxlMotion::__AxmStatusReadMotionInfo(long lAxisNo, PMOTION_INFO pMI)
{
	return AxmStatusReadMotionInfo(lAxisNo, pMI);
}

DWORD CDeviceAxlMotion::__AxmStatusRequestServoAlarm(long lAxisNo)
{
	return AxmStatusRequestServoAlarm(lAxisNo);
}

DWORD CDeviceAxlMotion::__AxmStatusReadServoAlarm(long lAxisNo, DWORD uReturnMode, DWORD *upAlarmCode)
{
	return AxmStatusReadServoAlarm(lAxisNo, uReturnMode, upAlarmCode);
}

DWORD CDeviceAxlMotion::__AxmStatusGetServoAlarmString(long lAxisNo, DWORD uAlarmCode, long lAlarmStringSize, char *szAlarmString)
{
	return AxmStatusGetServoAlarmString(lAxisNo, uAlarmCode, lAlarmStringSize, szAlarmString);
}

DWORD CDeviceAxlMotion::__AxmStatusRequestServoAlarmHistory(long lAxisNo)
{
	return AxmStatusRequestServoAlarmHistory(lAxisNo);
}

DWORD CDeviceAxlMotion::__AxmStatusReadServoAlarmHistory(long lAxisNo, DWORD uReturnMode, long *lpCount, DWORD *upAlarmCode)
{
	return AxmStatusReadServoAlarmHistory(lAxisNo, uReturnMode, lpCount, upAlarmCode);
}

DWORD CDeviceAxlMotion::__AxmStatusClearServoAlarmHistory(long lAxisNo)
{
	return AxmStatusClearServoAlarmHistory(lAxisNo);
}

DWORD CDeviceAxlMotion::__AxmHomeSetSignalLevel(long lAxisNo, DWORD uLevel)
{
	return AxmHomeSetSignalLevel(lAxisNo, uLevel);
}

DWORD CDeviceAxlMotion::__AxmHomeGetSignalLevel(long lAxisNo, DWORD *upLevel)
{
	return AxmHomeGetSignalLevel(lAxisNo, upLevel);
}

DWORD CDeviceAxlMotion::__AxmHomeReadSignal(long lAxisNo, DWORD *upStatus)
{
	return AxmHomeReadSignal(lAxisNo, upStatus);
}


DWORD CDeviceAxlMotion::__AxmHomeSetMethod(long lAxisNo, long lHmDir, DWORD uHomeSignal, DWORD uZphas, double dHomeClrTime, double dHomeOffset)
{
	return AxmHomeSetMethod(lAxisNo, lHmDir, uHomeSignal, uZphas, dHomeClrTime, dHomeOffset);
}

DWORD CDeviceAxlMotion::__AxmHomeGetMethod(long lAxisNo, long *lpHmDir, DWORD *upHomeSignal, DWORD *upZphas, double *dpHomeClrTime, double *dpHomeOffset)
{
	return AxmHomeGetMethod(lAxisNo, lpHmDir, upHomeSignal, upZphas, dpHomeClrTime, dpHomeOffset);
}

DWORD CDeviceAxlMotion::__AxmHomeSetFineAdjust(long lAxisNo, double dHomeDogLength, long lLevelScanTime, DWORD uFineSearchUse, DWORD uHomeClrUse)
{
	return AxmHomeSetFineAdjust(lAxisNo, dHomeDogLength, lLevelScanTime, uFineSearchUse, uHomeClrUse);
}

DWORD CDeviceAxlMotion::__AxmHomeGetFineAdjust(long lAxisNo, double *dpHomeDogLength, long *lpLevelScanTime, DWORD *upFineSearchUse, DWORD *upHomeClrUse)
{
	return AxmHomeGetFineAdjust(lAxisNo, dpHomeDogLength, lpLevelScanTime, upFineSearchUse, upHomeClrUse);
}

DWORD CDeviceAxlMotion::__AxmHomeSetVel(long lAxisNo, double dVelFirst, double dVelSecond, double dVelThird, double dVelLast, double dAccFirst, double dAccSecond)
{
	return AxmHomeSetVel(lAxisNo, dVelFirst, dVelSecond, dVelThird, dVelLast, dAccFirst, dAccSecond);
}

DWORD CDeviceAxlMotion::__AxmHomeGetVel(long lAxisNo, double *dpVelFirst, double *dpVelSecond, double *dpVelThird, double *dpVelLast, double *dpAccFirst, double *dpAccSecond)
{
	return AxmHomeGetVel(lAxisNo, dpVelFirst, dpVelSecond, dpVelThird, dpVelLast, dpAccFirst, dpAccSecond);
}

DWORD CDeviceAxlMotion::__AxmHomeSetStart(long lAxisNo)
{
	return AxmHomeSetStart(lAxisNo);
}

DWORD CDeviceAxlMotion::__AxmHomeSetResult(long lAxisNo, DWORD uHomeResult)
{
	return AxmHomeSetResult(lAxisNo, uHomeResult);
}

DWORD CDeviceAxlMotion::__AxmHomeGetResult(long lAxisNo, DWORD *upHomeResult)
{
	return AxmHomeGetResult(lAxisNo, upHomeResult);
}

DWORD CDeviceAxlMotion::__AxmHomeGetRate(long lAxisNo, DWORD *upHomeMainStepNumber, DWORD *upHomeStepNumber)
{
	return AxmHomeGetRate(lAxisNo, upHomeMainStepNumber, upHomeStepNumber);
}

DWORD CDeviceAxlMotion::__AxmMoveStartPos(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel)
{
	return AxmMoveStartPos(lAxisNo, dPos, dVel, dAccel, dDecel);
}

DWORD CDeviceAxlMotion::__AxmMovePos(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel)
{
	return AxmMovePos(lAxisNo, dPos, dVel, dAccel, dDecel);
}

DWORD CDeviceAxlMotion::__AxmMoveVel(long lAxisNo, double dVel, double dAccel, double dDecel)
{
	return AxmMoveVel(lAxisNo, dVel, dAccel, dDecel);
}

DWORD CDeviceAxlMotion::__AxmMoveStartMultiVel(long lArraySize, long *lpAxesNo, double *dpVel, double *dpAccel, double *dpDecel)
{
	return AxmMoveStartMultiVel(lArraySize, lpAxesNo, dpVel, dpAccel, dpDecel);
}


DWORD CDeviceAxlMotion::__AxmMoveStartMultiVelEx(long lArraySize, long *lpAxesNo, double *dpVel, double *dpAccel, double *dpDecel, DWORD dwSyncMode)
{
	return AxmMoveStartMultiVelEx(lArraySize, lpAxesNo, dpVel, dpAccel, dpDecel, dwSyncMode);
}


DWORD CDeviceAxlMotion::__AxmMoveStartLineVel(long lArraySize, long *lpAxesNo, double *dpDis, double dVel, double dAccel, double dDecel)
{
	return AxmMoveStartLineVel(lArraySize, lpAxesNo, dpDis, dVel, dAccel, dDecel);
}


DWORD CDeviceAxlMotion::__AxmMoveSignalSearch(long lAxisNo, double dVel, double dAccel, long lDetectSignal, long lSignalEdge, long lSignalMethod)
{
	return AxmMoveSignalSearch(lAxisNo, dVel, dAccel, lDetectSignal, lSignalEdge, lSignalMethod);
}

DWORD CDeviceAxlMotion::__AxmMoveSignalSearchAtDis(long lAxisNo, double dVel, double dAccel, double dDecel, long lDetectSignal, double dDis)
{
	return AxmMoveSignalSearchAtDis(lAxisNo, dVel, dAccel, dDecel, lDetectSignal, dDis);
}

DWORD CDeviceAxlMotion::__AxmMoveSignalCapture(long lAxisNo, double dVel, double dAccel, long lDetectSignal, long lSignalEdge, long lTarget, long lSignalMethod)
{
	return AxmMoveSignalCapture(lAxisNo, dVel, dAccel, lDetectSignal, lSignalEdge, lTarget, lSignalMethod);
}


DWORD CDeviceAxlMotion::__AxmMoveGetCapturePos(long lAxisNo, double *dpCapPotition)
{
	return AxmMoveGetCapturePos(lAxisNo, dpCapPotition);
}

DWORD CDeviceAxlMotion::__AxmMoveStartMultiPos(long lArraySize, long *lpAxisNo, double *dpPos, double *dpVel, double *dpAccel, double *dpDecel)
{
	return AxmMoveStartMultiPos(lArraySize, lpAxisNo, dpPos, dpVel, dpAccel, dpDecel);
}

DWORD CDeviceAxlMotion::__AxmMoveStartTorque(long lAxisNo, double dTorque, double dVel, DWORD dwAccFilterSel, DWORD dwGainSel, DWORD dwSpdLoopSel)
{
	return AxmMoveStartTorque(lAxisNo, dTorque, dVel, dwAccFilterSel, dwGainSel, dwSpdLoopSel);
}

DWORD CDeviceAxlMotion::__AxmMoveTorqueStop(long lAxisNo, DWORD dwMethod)
{
	return AxmMoveTorqueStop(lAxisNo, dwMethod);
}


DWORD CDeviceAxlMotion::__AxmMoveStartPosWithList(long lAxisNo, double dPosition, double *dpVel, double *dpAccel, double *dpDecel, long lListNum)
{
	return AxmMoveStartPosWithList(lAxisNo, dPosition, dpVel, dpAccel, dpDecel, lListNum);
}

DWORD CDeviceAxlMotion::__AxmMoveStartPosWithPosEvent(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel, long lEventAxisNo, double dComparePosition, DWORD uPositionSource)
{
	return AxmMoveStartPosWithPosEvent(lAxisNo, dPos, dVel, dAccel, dDecel, lEventAxisNo, dComparePosition, uPositionSource);
}

DWORD CDeviceAxlMotion::__AxmMoveStop(long lAxisNo, double dDecel)
{
	return AxmMoveStop(lAxisNo, dDecel);
}

DWORD CDeviceAxlMotion::__AxmMoveStopEx(long lAxisNo, double dDecel)
{
	return AxmMoveStopEx(lAxisNo, dDecel);
}

DWORD CDeviceAxlMotion::__AxmMoveEStop(long lAxisNo)
{
	return AxmMoveEStop(lAxisNo);
}

DWORD CDeviceAxlMotion::__AxmMoveSStop(long lAxisNo)
{
	return AxmMoveSStop(lAxisNo);
}

DWORD CDeviceAxlMotion::__AxmOverridePos(long lAxisNo, double dOverridePos)
{
	return AxmOverridePos(lAxisNo, dOverridePos);
}

DWORD CDeviceAxlMotion::__AxmOverrideSetMaxVel(long lAxisNo, double dOverrideMaxVel)
{
	return AxmOverrideSetMaxVel(lAxisNo, dOverrideMaxVel);
}

DWORD CDeviceAxlMotion::__AxmOverrideVel(long lAxisNo, double dOverrideVel)
{
	return AxmOverrideVel(lAxisNo, dOverrideVel);
}

DWORD CDeviceAxlMotion::__AxmOverrideAccelVelDecel(long lAxisNo, double dOverrideVelocity, double dMaxAccel, double dMaxDecel)
{
	return AxmOverrideAccelVelDecel(lAxisNo, dOverrideVelocity, dMaxAccel, dMaxDecel);
}


DWORD CDeviceAxlMotion::__AxmOverrideVelAtPos(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel, double dOverridePos, double dOverrideVel, long lTarget)
{
	return AxmOverrideVelAtPos(lAxisNo, dPos, dVel, dAccel, dDecel, dOverridePos, dOverrideVel, lTarget);
}

DWORD CDeviceAxlMotion::__AxmOverrideVelAtMultiPos(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel, long lArraySize, double* dpOverridePos, double* dpOverrideVel, long lTarget, DWORD dwOverrideMode)
{
	return AxmOverrideVelAtMultiPos(lAxisNo, dPos, dVel, dAccel, dDecel, lArraySize, dpOverridePos, dpOverrideVel, lTarget, dwOverrideMode);
}

DWORD CDeviceAxlMotion::__AxmOverrideVelAtMultiPos2(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel, long lArraySize, double* dpOverridePos, double* dpOverrideVel, double* dpOverrideAccelDecel, long lTarget, DWORD dwOverrideMode)
{
	return AxmOverrideVelAtMultiPos2(lAxisNo, dPos, dVel, dAccel, dDecel, lArraySize, dpOverridePos, dpOverrideVel, dpOverrideAccelDecel, lTarget, dwOverrideMode);
}


DWORD CDeviceAxlMotion::__AxmLinkSetMode(long lMasterAxisNo, long lSlaveAxisNo, double dSlaveRatio)
{
	return AxmLinkSetMode(lMasterAxisNo, lSlaveAxisNo, dSlaveRatio);
}

DWORD CDeviceAxlMotion::__AxmLinkGetMode(long lMasterAxisNo, long *lpSlaveAxisNo, double *dpGearRatio)
{
	return AxmLinkGetMode(lMasterAxisNo, lpSlaveAxisNo, dpGearRatio);
}

DWORD CDeviceAxlMotion::__AxmLinkResetMode(long lMasterAxisNo)
{
	return AxmLinkResetMode(lMasterAxisNo);
}

DWORD CDeviceAxlMotion::__AxmGantrySetEnable(long lMasterAxisNo, long lSlaveAxisNo, DWORD uSlHomeUse, double dSlOffset, double dSlOffsetRange)
{
	return AxmGantrySetEnable(lMasterAxisNo, lSlaveAxisNo, uSlHomeUse, dSlOffset, dSlOffsetRange);
}

DWORD CDeviceAxlMotion::__AxmGantryGetEnable(long lMasterAxisNo, DWORD *upSlHomeUse, double *dpSlOffset, double *dpSlORange, DWORD *upGatryOn)
{
	return AxmGantryGetEnable(lMasterAxisNo, upSlHomeUse, dpSlOffset, dpSlORange, upGatryOn);
}

DWORD CDeviceAxlMotion::__AxmGantrySetDisable(long lMasterAxisNo, long lSlaveAxisNo)
{
	return AxmGantrySetDisable(lMasterAxisNo, lSlaveAxisNo);
}

DWORD CDeviceAxlMotion::__AxmGantrySetCompensationGain(long lMasterAxisNo, long lMasterGain, long lSlaveGain)
{
	return AxmGantrySetCompensationGain(lMasterAxisNo, lMasterGain, lSlaveGain);
}

DWORD CDeviceAxlMotion::__AxmGantryGetCompensationGain(long lMasterAxisNo, long *lpMasterGain, long *lpSlaveGain)
{
	return AxmGantryGetCompensationGain(lMasterAxisNo, lpMasterGain, lpSlaveGain);
}

DWORD CDeviceAxlMotion::__AxmGantrySetErrorRange(long lMasterAxisNo, double dErrorRange, DWORD uUse)
{
	return AxmGantrySetErrorRange(lMasterAxisNo, dErrorRange, uUse);
}

DWORD CDeviceAxlMotion::__AxmGantryGetErrorRange(long lMasterAxisNo, double *dpErrorRange, DWORD *upUse)
{
	return AxmGantryGetErrorRange(lMasterAxisNo, dpErrorRange, upUse);
}

DWORD CDeviceAxlMotion::__AxmGantryReadErrorRangeStatus(long lMasterAxisNo, DWORD *dwpStatus)
{
	return AxmGantryReadErrorRangeStatus(lMasterAxisNo, dwpStatus);
}

DWORD CDeviceAxlMotion::__AxmGantryReadErrorRangeComparePos(long lMasterAxisNo, double *dpComparePos)
{
	return AxmGantryReadErrorRangeComparePos(lMasterAxisNo, dpComparePos);
}

DWORD CDeviceAxlMotion::__AxmLineMove(long lCoord, double *dpEndPos, double dVel, double dAccel, double dDecel)
{
	return AxmLineMove(lCoord, dpEndPos, dVel, dAccel, dDecel);
}

DWORD CDeviceAxlMotion::__AxmLineMoveEx2(long lCoord, double *dpEndPos, double dVel, double dAccel, double dDecel)
{
	return AxmLineMoveEx2(lCoord, dpEndPos, dVel, dAccel, dDecel);
}

DWORD CDeviceAxlMotion::__AxmCircleCenterMove(long lCoord, long *lAxisNo, double *dCenterPos, double *dEndPos, double dVel, double dAccel, double dDecel, DWORD uCWDir)
{
	return AxmCircleCenterMove(lCoord, lAxisNo, dCenterPos, dEndPos, dVel, dAccel, dDecel, uCWDir);
}

DWORD CDeviceAxlMotion::__AxmCirclePointMove(long lCoord, long *lAxisNo, double *dMidPos, double *dEndPos, double dVel, double dAccel, double dDecel, long lArcCircle)
{
	return AxmCirclePointMove(lCoord, lAxisNo, dMidPos, dEndPos, dVel, dAccel, dDecel, lArcCircle);
}

DWORD CDeviceAxlMotion::__AxmCircleRadiusMove(long lCoord, long *lAxisNo, double dRadius, double *dEndPos, double dVel, double dAccel, double dDecel, DWORD uCWDir, DWORD uShortDistance)
{
	return AxmCircleRadiusMove(lCoord, lAxisNo, dRadius, dEndPos, dVel, dAccel, dDecel, uCWDir, uShortDistance);
}

DWORD CDeviceAxlMotion::__AxmCircleAngleMove(long lCoord, long *lAxisNo, double *dCenterPos, double dAngle, double dVel, double dAccel, double dDecel, DWORD uCWDir)
{
	return AxmCircleAngleMove(lCoord, lAxisNo, dCenterPos, dAngle, dVel, dAccel, dDecel, uCWDir);
}

DWORD CDeviceAxlMotion::__AxmContiSetAxisMap(long lCoord, long lSize, long *lpAxesNo)
{
	return AxmContiSetAxisMap(lCoord, lSize, lpAxesNo);
}

DWORD CDeviceAxlMotion::__AxmContiGetAxisMap(long lCoord, long *lpSize, long *lpAxesNo)
{
	return AxmContiGetAxisMap(lCoord, lpSize, lpAxesNo);
}

DWORD CDeviceAxlMotion::__AxmContiSetAbsRelMode(long lCoord, DWORD uAbsRelMode)
{
	return AxmContiSetAbsRelMode(lCoord, uAbsRelMode);
}

DWORD CDeviceAxlMotion::__AxmContiGetAbsRelMode(long lCoord, DWORD *upAbsRelMode)
{
	return AxmContiGetAbsRelMode(lCoord, upAbsRelMode);
}

DWORD CDeviceAxlMotion::__AxmContiReadFree(long lCoord, DWORD *upQueueFree)
{
	return AxmContiReadFree(lCoord, upQueueFree);
}

DWORD CDeviceAxlMotion::__AxmContiReadIndex(long lCoord, long *lpQueueIndex)
{
	return AxmContiReadIndex(lCoord, lpQueueIndex);
}

DWORD CDeviceAxlMotion::__AxmContiWriteClear(long lCoord)
{
	return AxmContiWriteClear(lCoord);
}

DWORD CDeviceAxlMotion::__AxmContiBeginNode(long lCoord)
{
	return AxmContiBeginNode(lCoord);
}

DWORD CDeviceAxlMotion::__AxmContiEndNode(long lCoord)
{
	return AxmContiEndNode(lCoord);
}

DWORD CDeviceAxlMotion::__AxmContiStart(long lCoord, DWORD dwProfileset, long lAngle)
{
	return AxmContiStart(lCoord, dwProfileset, lAngle);
}

DWORD CDeviceAxlMotion::__AxmContiIsMotion(long lCoord, DWORD *upInMotion)
{
	return AxmContiIsMotion(lCoord, upInMotion);
}

DWORD CDeviceAxlMotion::__AxmContiGetNodeNum(long lCoord, long *lpNodeNum)
{
	return AxmContiGetNodeNum(lCoord, lpNodeNum);
}

DWORD CDeviceAxlMotion::__AxmContiGetTotalNodeNum(long lCoord, long *lpNodeNum)
{
	return AxmContiGetTotalNodeNum(lCoord, lpNodeNum);
}

DWORD CDeviceAxlMotion::__AxmTriggerSetTimeLevel(long lAxisNo, double dTrigTime, DWORD uTriggerLevel, DWORD uSelect, DWORD uInterrupt)
{
	return AxmTriggerSetTimeLevel(lAxisNo, dTrigTime, uTriggerLevel, uSelect, uInterrupt);
}

DWORD CDeviceAxlMotion::__AxmTriggerGetTimeLevel(long lAxisNo, double *dpTrigTime, DWORD *upTriggerLevel, DWORD *upSelect, DWORD *upInterrupt)
{
	return AxmTriggerGetTimeLevel(lAxisNo, dpTrigTime, upTriggerLevel, upSelect, upInterrupt);
}

DWORD CDeviceAxlMotion::__AxmTriggerSetAbsPeriod(long lAxisNo, DWORD uMethod, double dPos)
{
	return AxmTriggerSetAbsPeriod(lAxisNo, uMethod, dPos);
}

DWORD CDeviceAxlMotion::__AxmTriggerGetAbsPeriod(long lAxisNo, DWORD *upMethod, double *dpPos)
{
	return AxmTriggerGetAbsPeriod(lAxisNo, upMethod, dpPos);
}

DWORD CDeviceAxlMotion::__AxmTriggerSetBlock(long lAxisNo, double dStartPos, double dEndPos, double dPeriodPos)
{
	return AxmTriggerSetBlock(lAxisNo, dStartPos, dEndPos, dPeriodPos);
}

DWORD CDeviceAxlMotion::__AxmTriggerGetBlock(long lAxisNo, double *dpStartPos, double *dpEndPos, double *dpPeriodPos)
{
	return AxmTriggerGetBlock(lAxisNo, dpStartPos, dpEndPos, dpPeriodPos);
}

DWORD CDeviceAxlMotion::__AxmTriggerOneShot(long lAxisNo)
{
	return AxmTriggerOneShot(lAxisNo);
}

DWORD CDeviceAxlMotion::__AxmTriggerSetTimerOneshot(long lAxisNo, long lmSec)
{
	return AxmTriggerSetTimerOneshot(lAxisNo, lmSec);
}

DWORD CDeviceAxlMotion::__AxmTriggerOnlyAbs(long lAxisNo, long lTrigNum, double* dpTrigPos)
{
	return AxmTriggerOnlyAbs(lAxisNo, lTrigNum, dpTrigPos);
}

DWORD CDeviceAxlMotion::__AxmTriggerSetReset(long lAxisNo)
{
	return AxmTriggerSetReset(lAxisNo);
}

DWORD CDeviceAxlMotion::__AxmCrcSetMaskLevel(long lAxisNo, DWORD uLevel, DWORD uMethod)
{
	return AxmCrcSetMaskLevel(lAxisNo, uLevel, uMethod);
}

DWORD CDeviceAxlMotion::__AxmCrcGetMaskLevel(long lAxisNo, DWORD *upLevel, DWORD *upMethod)
{
	return AxmCrcGetMaskLevel(lAxisNo, upLevel, upMethod);
}

DWORD CDeviceAxlMotion::__AxmCrcSetOutput(long lAxisNo, DWORD uOnOff)
{
	return AxmCrcSetOutput(lAxisNo, uOnOff);
}

DWORD CDeviceAxlMotion::__AxmCrcGetOutput(long lAxisNo, DWORD *upOnOff)
{
	return AxmCrcGetOutput(lAxisNo, upOnOff);
}

DWORD CDeviceAxlMotion::__AxmMPGSetEnable(long lAxisNo, long lInputMethod, long lDriveMode, double dMPGPos, double dVel, double dAccel)
{
	return AxmMPGSetEnable(lAxisNo, lInputMethod, lDriveMode, dMPGPos, dVel, dAccel);
}


DWORD CDeviceAxlMotion::__AxmMPGGetEnable(long lAxisNo, long *lpInputMethod, long *lpDriveMode, double *dpMPGPos, double *dpVel, double *dAccel)
{
	return AxmMPGGetEnable(lAxisNo, lpInputMethod, lpDriveMode, dpMPGPos, dpVel, dAccel);
}


DWORD CDeviceAxlMotion::__AxmMPGSetRatio(long lAxisNo, DWORD uMPGnumerator, DWORD uMPGdenominator)
{
	return AxmMPGSetRatio(lAxisNo, uMPGnumerator, uMPGdenominator);
}

DWORD CDeviceAxlMotion::__AxmMPGGetRatio(long lAxisNo, DWORD *upMPGnumerator, DWORD *upMPGdenominator)
{
	return AxmMPGGetRatio(lAxisNo, upMPGnumerator, upMPGdenominator);
}

DWORD CDeviceAxlMotion::__AxmMPGReset(long lAxisNo)
{
	return AxmMPGReset(lAxisNo);
}

DWORD CDeviceAxlMotion::__AxmHelixCenterMove(long lCoord, double dCenterXPos, double dCenterYPos, double dEndXPos, double dEndYPos, double dZPos, double dVel, double dAccel, double dDecel, DWORD uCWDir)
{
	return AxmHelixCenterMove(lCoord, dCenterXPos, dCenterYPos, dEndXPos, dEndYPos, dZPos, dVel, dAccel, dDecel, uCWDir);
}

DWORD CDeviceAxlMotion::__AxmHelixPointMove(long lCoord, double dMidXPos, double dMidYPos, double dEndXPos, double dEndYPos, double dZPos, double dVel, double dAccel, double dDecel)
{
	return AxmHelixPointMove(lCoord, dMidXPos, dMidYPos, dEndXPos, dEndYPos, dZPos, dVel, dAccel, dDecel);
}

DWORD CDeviceAxlMotion::__AxmHelixRadiusMove(long lCoord, double dRadius, double dEndXPos, double dEndYPos, double dZPos, double dVel, double dAccel, double dDecel, DWORD uCWDir, DWORD uShortDistance)
{
	return AxmHelixRadiusMove(lCoord, dRadius, dEndXPos, dEndYPos, dZPos, dVel, dAccel, dDecel, uCWDir, uShortDistance);
}

DWORD CDeviceAxlMotion::__AxmHelixAngleMove(long lCoord, double dCenterXPos, double dCenterYPos, double dAngle, double dZPos, double dVel, double dAccel, double dDecel, DWORD uCWDir)
{
	return AxmHelixAngleMove(lCoord, dCenterXPos, dCenterYPos, dAngle, dZPos, dVel, dAccel, dDecel, uCWDir);
}

DWORD CDeviceAxlMotion::__AxmSplineWrite(long lCoord, long lPosSize, double *dpPosX, double *dpPosY, double dVel, double dAccel, double dDecel, double dPosZ, long lPointFactor)
{
	return AxmSplineWrite(lCoord, lPosSize, dpPosX, dpPosY, dVel, dAccel, dDecel, dPosZ, lPointFactor);
}

DWORD CDeviceAxlMotion::__AxmCompensationSet(long lAxisNo, long lNumEntry, double dStartPos, double *dpPosition, double *dpCorrection, DWORD dwRollOver)
{
	return AxmCompensationSet(lAxisNo, lNumEntry, dStartPos, dpPosition, dpCorrection, dwRollOver);
}

DWORD CDeviceAxlMotion::__AxmCompensationGet(long lAxisNo, long *lpNumEntry, double *dpStartPos, double *dpPosition, double *dpCorrection, DWORD *dwpRollOver)
{
	return AxmCompensationGet(lAxisNo, lpNumEntry, dpStartPos, dpPosition, dpCorrection, dwpRollOver);
}

DWORD CDeviceAxlMotion::__AxmCompensationEnable(long lAxisNo, DWORD dwEnable)
{
	return AxmCompensationEnable(lAxisNo, dwEnable);
}

DWORD CDeviceAxlMotion::__AxmCompensationIsEnable(long lAxisNo, DWORD *dwpEnable)
{
	return AxmCompensationIsEnable(lAxisNo, dwpEnable);
}

DWORD CDeviceAxlMotion::__AxmCompensationGetCorrection(long lAxisNo, double *dpCorrection)
{
	return AxmCompensationGetCorrection(lAxisNo, dpCorrection);
}

DWORD CDeviceAxlMotion::__AxmCompensationSetBacklash(long lAxisNo, long lBacklashDir, double dBacklash)
{
	return AxmCompensationSetBacklash(lAxisNo, lBacklashDir, dBacklash);
}

DWORD CDeviceAxlMotion::__AxmCompensationGetBacklash(long lAxisNo, long *lpBacklashDir, double *dpBacklash)
{
	return AxmCompensationGetBacklash(lAxisNo, lpBacklashDir, dpBacklash);
}

DWORD CDeviceAxlMotion::__AxmCompensationEnableBacklash(long lAxisNo, DWORD dwEnable)
{
	return AxmCompensationEnableBacklash(lAxisNo, dwEnable);
}

DWORD CDeviceAxlMotion::__AxmCompensationIsEnableBacklash(long lAxisNo, DWORD *dwpEnable)
{
	return AxmCompensationIsEnableBacklash(lAxisNo, dwpEnable);
}

DWORD CDeviceAxlMotion::__AxmCompensationSetLocating(long lAxisNo, double dVel, double dAccel, double dDecel, double dWaitTime)
{
	return AxmCompensationSetLocating(lAxisNo, dVel, dAccel, dDecel, dWaitTime);
}


DWORD CDeviceAxlMotion::__AxmEcamSet(long lAxisNo, long lMasterAxis, long lNumEntry, double dMasterStartPos, double *dpMasterPos, double *dpSlavePos)
{
	return AxmEcamSet(lAxisNo, lMasterAxis, lNumEntry, dMasterStartPos, dpMasterPos, dpSlavePos);
}

DWORD CDeviceAxlMotion::__AxmEcamSetWithSource(long lAxisNo, long lMasterAxis, long lNumEntry, double dMasterStartPos, double *dpMasterPos, double *dpSlavePos, DWORD dwSource)
{
	return AxmEcamSetWithSource(lAxisNo, lMasterAxis, lNumEntry, dMasterStartPos, dpMasterPos, dpSlavePos, dwSource);
}

DWORD CDeviceAxlMotion::__AxmEcamGet(long lAxisNo, long *lpMasterAxis, long *lpNumEntry, double *dpMasterStartPos, double *dpMasterPos, double *dpSlavePos)
{
	return AxmEcamGet(lAxisNo, lpMasterAxis, lpNumEntry, dpMasterStartPos, dpMasterPos, dpSlavePos);
}

DWORD CDeviceAxlMotion::__AxmEcamGetWithSource(long lAxisNo, long *lpMasterAxis, long *lpNumEntry, double *dpMasterStartPos, double *dpMasterPos, double *dpSlavePos, DWORD *dwpSource)
{
	return AxmEcamGetWithSource(lAxisNo, lpMasterAxis, lpNumEntry, dpMasterStartPos, dpMasterPos, dpSlavePos, dwpSource);
}

DWORD CDeviceAxlMotion::__AxmEcamEnableBySlave(long lAxisNo, DWORD dwEnable)
{
	return AxmEcamEnableBySlave(lAxisNo, dwEnable);
}

DWORD CDeviceAxlMotion::__AxmEcamEnableByMaster(long lAxisNo, DWORD dwEnable)
{
	return AxmEcamEnableByMaster(lAxisNo, dwEnable);
}

DWORD CDeviceAxlMotion::__AxmEcamIsSlaveEnable(long lAxisNo, DWORD *dwpEnable)
{
	return AxmEcamIsSlaveEnable(lAxisNo, dwpEnable);
}

DWORD CDeviceAxlMotion::__AxmStatusSetServoMonitor(long lAxisNo, DWORD dwSelMon, double dActionValue, DWORD dwAction)
{
	return AxmStatusSetServoMonitor(lAxisNo, dwSelMon, dActionValue, dwAction);
}

DWORD CDeviceAxlMotion::__AxmStatusGetServoMonitor(long lAxisNo, DWORD dwSelMon, double *dpActionValue, DWORD *dwpAction)
{
	return AxmStatusGetServoMonitor(lAxisNo, dwSelMon, dpActionValue, dwpAction);
}

DWORD CDeviceAxlMotion::__AxmStatusSetServoMonitorEnable(long lAxisNo, DWORD dwEnable)
{
	return AxmStatusSetServoMonitorEnable(lAxisNo, dwEnable);
}

DWORD CDeviceAxlMotion::__AxmStatusGetServoMonitorEnable(long lAxisNo, DWORD *dwpEnable)
{
	return AxmStatusGetServoMonitorEnable(lAxisNo, dwpEnable);
}

DWORD CDeviceAxlMotion::__AxmStatusReadServoMonitorFlag(long lAxisNo, DWORD dwSelMon, DWORD *dwpMonitorFlag, double *dpMonitorValue)
{
	return AxmStatusReadServoMonitorFlag(lAxisNo, dwSelMon, dwpMonitorFlag, dpMonitorValue);
}

DWORD CDeviceAxlMotion::__AxmStatusReadServoMonitorValue(long lAxisNo, DWORD dwSelMon, double *dpMonitorValue)
{
	return AxmStatusReadServoMonitorValue(lAxisNo, dwSelMon, dpMonitorValue);
}

DWORD CDeviceAxlMotion::__AxmStatusSetReadServoLoadRatio(long lAxisNo, DWORD dwSelMon)
{
	return AxmStatusSetReadServoLoadRatio(lAxisNo, dwSelMon);
}

DWORD CDeviceAxlMotion::__AxmStatusReadServoLoadRatio(long lAxisNo, double *dpMonitorValue)
{
	return AxmStatusReadServoLoadRatio(lAxisNo, dpMonitorValue);
}

DWORD CDeviceAxlMotion::__AxmMotSetScaleCoeff(long lAxisNo, long lScaleCoeff)
{
	return AxmMotSetScaleCoeff(lAxisNo, lScaleCoeff);
}

DWORD CDeviceAxlMotion::__AxmMotGetScaleCoeff(long lAxisNo, long *lpScaleCoeff)
{
	return AxmMotGetScaleCoeff(lAxisNo, lpScaleCoeff);
}


DWORD CDeviceAxlMotion::__AxmMoveSignalSearchEx(long lAxisNo, double dVel, double dAccel, long lDetectSignal, long lSignalEdge, long lSignalMethod)
{
	return AxmMoveSignalSearchEx(lAxisNo, dVel, dAccel, lDetectSignal, lSignalEdge, lSignalMethod);
}


DWORD CDeviceAxlMotion::__AxmMoveToAbsPos(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel)
{
	return AxmMoveToAbsPos(lAxisNo, dPos, dVel, dAccel, dDecel);
}

DWORD CDeviceAxlMotion::__AxmStatusReadVelEx(long lAxisNo, double *dpVel)
{
	return AxmStatusReadVelEx(lAxisNo, dpVel);
}

DWORD CDeviceAxlMotion::__AxmMotSetElectricGearRatio(long lAxisNo, long lNumerator, long lDenominator)
{
	return AxmMotSetElectricGearRatio(lAxisNo, lNumerator, lDenominator);
}

DWORD CDeviceAxlMotion::__AxmMotGetElectricGearRatio(long lAxisNo, long *lpNumerator, long *lpDenominator)
{
	return AxmMotGetElectricGearRatio(lAxisNo, lpNumerator, lpDenominator);
}

DWORD CDeviceAxlMotion::__AxmMotSetTorqueLimit(long lAxisNo, double dbPlusDirTorqueLimit, double dbMinusDirTorqueLimit)
{
	return AxmMotSetTorqueLimit(lAxisNo, dbPlusDirTorqueLimit, dbMinusDirTorqueLimit);
}

DWORD CDeviceAxlMotion::__AxmMotGetTorqueLimit(long lAxisNo, double* dbpPlusDirTorqueLimit, double* dbpMinusDirTorqueLimit)
{
	return AxmMotGetTorqueLimit(lAxisNo, dbpPlusDirTorqueLimit, dbpMinusDirTorqueLimit);
}

DWORD CDeviceAxlMotion::__AxmMotSetTorqueLimitEx(long lAxisNo, double dbPlusDirTorqueLimit, double dbMinusDirTorqueLimit)
{
	return AxmMotSetTorqueLimitEx(lAxisNo, dbPlusDirTorqueLimit, dbMinusDirTorqueLimit);
}

DWORD CDeviceAxlMotion::__AxmMotGetTorqueLimitEx(long lAxisNo, double* dbpPlusDirTorqueLimit, double* dbpMinusDirTorqueLimit)
{
	return AxmMotGetTorqueLimitEx(lAxisNo, dbpPlusDirTorqueLimit, dbpMinusDirTorqueLimit);
}

DWORD CDeviceAxlMotion::__AxmMotSetTorqueLimitAtPos(long lAxisNo, double dbPlusDirTorqueLimit, double dbMinusDirTorqueLimit, double dPosition, long lTarget)
{
	return AxmMotSetTorqueLimitAtPos(lAxisNo, dbPlusDirTorqueLimit, dbMinusDirTorqueLimit, dPosition, lTarget);
}

DWORD CDeviceAxlMotion::__AxmMotGetTorqueLimitAtPos(long lAxisNo, double* dbpPlusDirTorqueLimit, double* dbpMinusDirTorqueLimit, double* dpPosition, long* lpTarget)
{
	return AxmMotGetTorqueLimitAtPos(lAxisNo, dbpPlusDirTorqueLimit, dbpMinusDirTorqueLimit, dpPosition, lpTarget);
}

DWORD CDeviceAxlMotion::__AxmMotSetTorqueLimitEnable(long lAxisNo, DWORD uUse)
{
	return AxmMotSetTorqueLimitEnable(lAxisNo, uUse);
}

DWORD CDeviceAxlMotion::__AxmMotGetTorqueLimitEnable(long lAxisNo, DWORD* upUse)
{
	return AxmMotGetTorqueLimitEnable(lAxisNo, upUse);
}

DWORD CDeviceAxlMotion::__AxmOverridePosSetFunction(long lAxisNo, DWORD dwUsage, long lDecelPosRatio, double dReserved)
{
	return AxmOverridePosSetFunction(lAxisNo, dwUsage, lDecelPosRatio, dReserved);
}

DWORD CDeviceAxlMotion::__AxmOverridePosGetFunction(long lAxisNo, DWORD *dwpUsage, long *lpDecelPosRatio, double *dpReserved)
{
	return AxmOverridePosGetFunction(lAxisNo, dwpUsage, lpDecelPosRatio, dpReserved);
}

DWORD CDeviceAxlMotion::__AxmSignalSetWriteOutputBitAtPos(long lAxisNo, long lModuleNo, long lOffset, DWORD uValue, double dPosition, long lTarget)
{
	return AxmSignalSetWriteOutputBitAtPos(lAxisNo, lModuleNo, lOffset, uValue, dPosition, lTarget);
}

DWORD CDeviceAxlMotion::__AxmSignalGetWriteOutputBitAtPos(long lAxisNo, long* lpModuleNo, long* lOffset, DWORD* upValue, double* dpPosition, long* lpTarget)
{
	return AxmSignalGetWriteOutputBitAtPos(lAxisNo, lpModuleNo, lOffset, upValue, dpPosition, lpTarget);
}

DWORD CDeviceAxlMotion::__AxmAdvVSTSetParameter(long lCoord, DWORD dwISTSize, double* dbpFrequency, double* dbpDampingRatio, DWORD* dwpImpulseCount)
{
	return AxmAdvVSTSetParameter(lCoord, dwISTSize, dbpFrequency, dbpDampingRatio, dwpImpulseCount);
}

DWORD CDeviceAxlMotion::__AxmAdvVSTGetParameter(long lCoord, DWORD* dwpISTSize, double* dbpFrequency, double* dbpDampingRatio, DWORD* dwpImpulseCount)
{
	return AxmAdvVSTGetParameter(lCoord, dwpISTSize, dbpFrequency, dbpDampingRatio, dwpImpulseCount);
}

DWORD CDeviceAxlMotion::__AxmAdvVSTSetEnabele(long lCoord, DWORD dwISTEnable)
{
	return AxmAdvVSTSetEnabele(lCoord, dwISTEnable);
}

DWORD CDeviceAxlMotion::__AxmAdvVSTGetEnabele(long lCoord, DWORD* dwpISTEnable)
{
	return AxmAdvVSTGetEnabele(lCoord, dwpISTEnable);
}

DWORD CDeviceAxlMotion::__AxmAdvLineMove(long lCoordinate, double *dPosition, double dMaxVelocity, double dStartVel, double dStopVel, double dMaxAccel, double dMaxDecel)
{
	return AxmAdvLineMove(lCoordinate, dPosition, dMaxVelocity, dStartVel, dStopVel, dMaxAccel, dMaxDecel);
}

DWORD CDeviceAxlMotion::__AxmAdvOvrLineMove(long lCoordinate, double *dPosition, double dMaxVelocity, double dStartVel, double dStopVel, double dMaxAccel, double dMaxDecel, long lOverrideMode)
{
	return AxmAdvOvrLineMove(lCoordinate, dPosition, dMaxVelocity, dStartVel, dStopVel, dMaxAccel, dMaxDecel, lOverrideMode);
}

DWORD CDeviceAxlMotion::__AxmAdvCircleCenterMove(long lCoord, long *lAxisNo, double *dCenterPos, double *dEndPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir)
{
	return AxmAdvCircleCenterMove(lCoord, lAxisNo, dCenterPos, dEndPos, dVel, dStartVel, dStopVel, dAccel, dDecel, uCWDir);
}

DWORD CDeviceAxlMotion::__AxmAdvCirclePointMove(long lCoord, long *lAxisNo, double *dMidPos, double *dEndPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, long lArcCircle)
{
	return AxmAdvCirclePointMove(lCoord, lAxisNo, dMidPos, dEndPos, dVel, dStartVel, dStopVel, dAccel, dDecel, lArcCircle);
}

DWORD CDeviceAxlMotion::__AxmAdvCircleAngleMove(long lCoord, long *lAxisNo, double *dCenterPos, double dAngle, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir)
{
	return AxmAdvCircleAngleMove(lCoord, lAxisNo, dCenterPos, dAngle, dVel, dStartVel, dStopVel, dAccel, dDecel, uCWDir);
}

DWORD CDeviceAxlMotion::__AxmAdvCircleRadiusMove(long lCoord, long *lAxisNo, double dRadius, double *dEndPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir, DWORD uShortDistance)
{
	return AxmAdvCircleRadiusMove(lCoord, lAxisNo, dRadius, dEndPos, dVel, dStartVel, dStopVel, dAccel, dDecel, uCWDir, uShortDistance);
}

DWORD CDeviceAxlMotion::__AxmAdvOvrCircleRadiusMove(long lCoord, long *lAxisNo, double dRadius, double *dEndPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir, DWORD uShortDistance, long lOverrideMode)
{
	return AxmAdvOvrCircleRadiusMove(lCoord, lAxisNo, dRadius, dEndPos, dVel, dStartVel, dStopVel, dAccel, dDecel, uCWDir, uShortDistance, lOverrideMode);
}

DWORD CDeviceAxlMotion::__AxmAdvHelixCenterMove(long lCoord, double dCenterXPos, double dCenterYPos, double dEndXPos, double dEndYPos, double dZPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir)
{
	return AxmAdvHelixCenterMove(lCoord, dCenterXPos, dCenterYPos, dEndXPos, dEndYPos, dZPos, dVel, dStartVel, dStopVel, dAccel, dDecel, uCWDir);
}

DWORD CDeviceAxlMotion::__AxmAdvHelixPointMove(long lCoord, double dMidXPos, double dMidYPos, double dEndXPos, double dEndYPos, double dZPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel)
{
	return AxmAdvHelixPointMove(lCoord, dMidXPos, dMidYPos, dEndXPos, dEndYPos, dZPos, dVel, dStartVel, dStopVel, dAccel, dDecel);
}

DWORD CDeviceAxlMotion::__AxmAdvHelixAngleMove(long lCoord, double dCenterXPos, double dCenterYPos, double dAngle, double dZPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir)
{
	return AxmAdvHelixAngleMove(lCoord, dCenterXPos, dCenterYPos, dAngle, dZPos, dVel, dStartVel, dStopVel, dAccel, dDecel, uCWDir);
}

DWORD CDeviceAxlMotion::__AxmAdvHelixRadiusMove(long lCoord, double dRadius, double dEndXPos, double dEndYPos, double dZPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir, DWORD uShortDistance)
{
	return AxmAdvHelixRadiusMove(lCoord, dRadius, dEndXPos, dEndYPos, dZPos, dVel, dStartVel, dStopVel, dAccel, dDecel, uCWDir, uShortDistance);
}

DWORD CDeviceAxlMotion::__AxmAdvOvrHelixRadiusMove(long lCoord, double dRadius, double dEndXPos, double dEndYPos, double dZPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir, DWORD uShortDistance, long lOverrideMode)
{
	return AxmAdvOvrHelixRadiusMove(lCoord, dRadius, dEndXPos, dEndYPos, dZPos, dVel, dStartVel, dStopVel, dAccel, dDecel, uCWDir, uShortDistance, lOverrideMode);
}

DWORD CDeviceAxlMotion::__AxmAdvScriptLineMove(long lCoordinate, double *dPosition, double dMaxVelocity, double dStartVel, double dStopVel, double dMaxAccel, double dMaxDecel, DWORD dwScript, long lScirptAxisNo, double dScriptPos)
{
	return AxmAdvScriptLineMove(lCoordinate, dPosition, dMaxVelocity, dStartVel, dStopVel, dMaxAccel, dMaxDecel, dwScript, lScirptAxisNo, dScriptPos);
}

DWORD CDeviceAxlMotion::__AxmAdvScriptOvrLineMove(long lCoordinate, double *dPosition, double dMaxVelocity, double dStartVel, double dStopVel, double dMaxAccel, double dMaxDecel, long lOverrideMode, DWORD dwScript, long lScirptAxisNo, double dScriptPos)
{
	return AxmAdvScriptOvrLineMove(lCoordinate, dPosition, dMaxVelocity, dStartVel, dStopVel, dMaxAccel, dMaxDecel, lOverrideMode, dwScript, lScirptAxisNo, dScriptPos);
}

DWORD CDeviceAxlMotion::__AxmAdvScriptCircleCenterMove(long lCoord, long *lAxisNo, double *dCenterPos, double *dEndPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir, DWORD dwScript, long lScirptAxisNo, double dScriptPos)
{
	return AxmAdvScriptCircleCenterMove(lCoord, lAxisNo, dCenterPos, dEndPos, dVel, dStartVel, dStopVel, dAccel, dDecel, uCWDir, dwScript, lScirptAxisNo, dScriptPos);
}

DWORD CDeviceAxlMotion::__AxmAdvScriptCirclePointMove(long lCoord, long *lAxisNo, double *dMidPos, double *dEndPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, long lArcCircle, DWORD dwScript, long lScirptAxisNo, double dScriptPos)
{
	return AxmAdvScriptCirclePointMove(lCoord, lAxisNo, dMidPos, dEndPos, dVel, dStartVel, dStopVel, dAccel, dDecel, lArcCircle, dwScript, lScirptAxisNo, dScriptPos);
}

DWORD CDeviceAxlMotion::__AxmAdvScriptCircleAngleMove(long lCoord, long *lAxisNo, double *dCenterPos, double dAngle, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir, DWORD dwScript, long lScirptAxisNo, double dScriptPos)
{
	return AxmAdvScriptCircleAngleMove(lCoord, lAxisNo, dCenterPos, dAngle, dVel, dStartVel, dStopVel, dAccel, dDecel, uCWDir, dwScript, lScirptAxisNo, dScriptPos);
}

DWORD CDeviceAxlMotion::__AxmAdvScriptCircleRadiusMove(long lCoord, long *lAxisNo, double dRadius, double *dEndPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir, DWORD uShortDistance, DWORD dwScript, long lScirptAxisNo, double dScriptPos)
{
	return AxmAdvScriptCircleRadiusMove(lCoord, lAxisNo, dRadius, dEndPos, dVel, dStartVel, dStopVel, dAccel, dDecel, uCWDir, uShortDistance, dwScript, lScirptAxisNo, dScriptPos);
}

DWORD CDeviceAxlMotion::__AxmAdvScriptOvrCircleRadiusMove(long lCoord, long *lAxisNo, double dRadius, double *dEndPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir, DWORD uShortDistance, long lOverrideMode, DWORD dwScript, long lScirptAxisNo, double dScriptPos)
{
	return AxmAdvScriptOvrCircleRadiusMove(lCoord, lAxisNo, dRadius, dEndPos, dVel, dStartVel, dStopVel, dAccel, dDecel, uCWDir, uShortDistance, lOverrideMode, dwScript, lScirptAxisNo, dScriptPos);
}

DWORD CDeviceAxlMotion::__AxmAdvScriptHelixCenterMove(long lCoord, double dCenterXPos, double dCenterYPos, double dEndXPos, double dEndYPos, double dZPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir, DWORD dwScript, long lScirptAxisNo, double dScriptPos)
{
	return AxmAdvScriptHelixCenterMove(lCoord, dCenterXPos, dCenterYPos, dEndXPos, dEndYPos, dZPos, dVel, dStartVel, dStopVel, dAccel, dDecel, uCWDir, dwScript, lScirptAxisNo, dScriptPos);
}

DWORD CDeviceAxlMotion::__AxmAdvScriptHelixPointMove(long lCoord, double dMidXPos, double dMidYPos, double dEndXPos, double dEndYPos, double dZPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD dwScript, long lScirptAxisNo, double dScriptPos)
{
	return AxmAdvScriptHelixPointMove(lCoord, dMidXPos, dMidYPos, dEndXPos, dEndYPos, dZPos, dVel, dStartVel, dStopVel, dAccel, dDecel, dwScript, lScirptAxisNo, dScriptPos);
}

DWORD CDeviceAxlMotion::__AxmAdvScriptHelixAngleMove(long lCoord, double dCenterXPos, double dCenterYPos, double dAngle, double dZPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir, DWORD dwScript, long lScirptAxisNo, double dScriptPos)
{
	return AxmAdvScriptHelixAngleMove(lCoord, dCenterXPos, dCenterYPos, dAngle, dZPos, dVel, dStartVel, dStopVel, dAccel, dDecel, uCWDir, dwScript, lScirptAxisNo, dScriptPos);
}

DWORD CDeviceAxlMotion::__AxmAdvScriptHelixRadiusMove(long lCoord, double dRadius, double dEndXPos, double dEndYPos, double dZPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir, DWORD uShortDistance, DWORD dwScript, long lScirptAxisNo, double dScriptPos)
{
	return AxmAdvScriptHelixRadiusMove(lCoord, dRadius, dEndXPos, dEndYPos, dZPos, dVel, dStartVel, dStopVel, dAccel, dDecel, uCWDir, uShortDistance, dwScript, lScirptAxisNo, dScriptPos);
}

DWORD CDeviceAxlMotion::__AxmAdvScriptOvrHelixRadiusMove(long lCoord, double dRadius, double dEndXPos, double dEndYPos, double dZPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir, DWORD uShortDistance, long lOverrideMode, DWORD dwScript, long lScirptAxisNo, double dScriptPos)
{
	return AxmAdvScriptOvrHelixRadiusMove(lCoord, dRadius, dEndXPos, dEndYPos, dZPos, dVel, dStartVel, dStopVel, dAccel, dDecel, uCWDir, uShortDistance, lOverrideMode, dwScript, lScirptAxisNo, dScriptPos);
}


DWORD CDeviceAxlMotion::__AxmAdvContiGetNodeNum(long lCoordinate, long *lpNodeNum)
{
	return AxmAdvContiGetNodeNum(lCoordinate, lpNodeNum);
}

DWORD CDeviceAxlMotion::__AxmAdvContiGetTotalNodeNum(long lCoordinate, long *lpNodeNum)
{
	return AxmAdvContiGetTotalNodeNum(lCoordinate, lpNodeNum);
}

DWORD CDeviceAxlMotion::__AxmAdvContiReadIndex(long lCoordinate, long *lpQueueIndex)
{
	return AxmAdvContiReadIndex(lCoordinate, lpQueueIndex);
}

DWORD CDeviceAxlMotion::__AxmAdvContiReadFree(long lCoordinate, DWORD *upQueueFree)
{
	return AxmAdvContiReadFree(lCoordinate, upQueueFree);
}

DWORD CDeviceAxlMotion::__AxmAdvContiWriteClear(long lCoordinate)
{
	return AxmAdvContiWriteClear(lCoordinate);
}

DWORD CDeviceAxlMotion::__AxmAdvOvrContiWriteClear(long lCoordinate)
{
	return AxmAdvOvrContiWriteClear(lCoordinate);
}

DWORD CDeviceAxlMotion::__AxmAdvContiStart(long lCoord, DWORD dwProfileset, long lAngle)
{
	return AxmAdvContiStart(lCoord, dwProfileset, lAngle);
}

DWORD CDeviceAxlMotion::__AxmAdvContiStop(long lCoordinate, double dDecel)
{
	return AxmAdvContiStop(lCoordinate, dDecel);
}

DWORD CDeviceAxlMotion::__AxmAdvContiSetAxisMap(long lCoord, long lSize, long *lpAxesNo)
{
	return AxmAdvContiSetAxisMap(lCoord, lSize, lpAxesNo);
}

DWORD CDeviceAxlMotion::__AxmAdvContiGetAxisMap(long lCoord, long *lpSize, long *lpAxesNo)
{
	return AxmAdvContiGetAxisMap(lCoord, lpSize, lpAxesNo);
}

DWORD CDeviceAxlMotion::__AxmAdvContiSetAbsRelMode(long lCoord, DWORD uAbsRelMode)
{
	return AxmAdvContiSetAbsRelMode(lCoord, uAbsRelMode);
}

DWORD CDeviceAxlMotion::__AxmAdvContiGetAbsRelMode(long lCoord, DWORD *uAbsRelMode)
{
	return AxmAdvContiGetAbsRelMode(lCoord, uAbsRelMode);
}

DWORD CDeviceAxlMotion::__AxmAdvContiIsMotion(long lCoordinate, DWORD *upInMotion)
{
	return AxmAdvContiIsMotion(lCoordinate, upInMotion);
}

DWORD CDeviceAxlMotion::__AxmAdvContiBeginNode(long lCoord)
{
	return AxmAdvContiBeginNode(lCoord);
}

DWORD CDeviceAxlMotion::__AxmAdvContiEndNode(long lCoord)
{
	return AxmAdvContiEndNode(lCoord);
}

DWORD CDeviceAxlMotion::__AxmMoveMultiStop(long lArraySize, long *lpAxesNo, double *dMaxDecel)
{
	return AxmMoveMultiStop(lArraySize, lpAxesNo, dMaxDecel);
}

DWORD CDeviceAxlMotion::__AxmMoveMultiEStop(long lArraySize, long *lpAxesNo)
{
	return AxmMoveMultiEStop(lArraySize, lpAxesNo);
}

DWORD CDeviceAxlMotion::__AxmMoveMultiSStop(long lArraySize, long *lpAxesNo)
{
	return AxmMoveMultiSStop(lArraySize, lpAxesNo);
}

DWORD CDeviceAxlMotion::__AxmStatusReadActVel(long lAxisNo, double *dpVel)
{
	return AxmStatusReadActVel(lAxisNo, dpVel);
}

DWORD CDeviceAxlMotion::__AxmStatusReadServoCmdStat(long lAxisNo, DWORD *upStatus)
{
	return AxmStatusReadServoCmdStat(lAxisNo, upStatus);
}

DWORD CDeviceAxlMotion::__AxmStatusReadServoCmdCtrl(long lAxisNo, DWORD *upStatus)
{
	return AxmStatusReadServoCmdCtrl(lAxisNo, upStatus);
}

DWORD CDeviceAxlMotion::__AxmGantryGetMstToSlvOverDist(long lAxisNo, double *dpPosition)
{
	return AxmGantryGetMstToSlvOverDist(lAxisNo, dpPosition);
}

DWORD CDeviceAxlMotion::__AxmGantrySetMstToSlvOverDist(long lAxisNo, double dPosition)
{
	return AxmGantrySetMstToSlvOverDist(lAxisNo, dPosition);
}

DWORD CDeviceAxlMotion::__AxmSignalReadServoAlarmCode(long lAxisNo, WORD *upCodeStatus)
{
	return AxmSignalReadServoAlarmCode(lAxisNo, upCodeStatus);
}

DWORD CDeviceAxlMotion::__AxmM3ServoCoordinatesSet(long lAxisNo, DWORD dwPosData, DWORD dwPos_sel, DWORD dwRefe)
{
	return AxmM3ServoCoordinatesSet(lAxisNo, dwPosData, dwPos_sel, dwRefe);
}

DWORD CDeviceAxlMotion::__AxmM3ServoBreakOn(long lAxisNo)
{
	return AxmM3ServoBreakOn(lAxisNo);
}

DWORD CDeviceAxlMotion::__AxmM3ServoBreakOff(long lAxisNo)
{
	return AxmM3ServoBreakOff(lAxisNo);
}

DWORD CDeviceAxlMotion::__AxmM3ServoConfig(long lAxisNo, DWORD dwCfMode)
{
	return AxmM3ServoConfig(lAxisNo, dwCfMode);
}

DWORD CDeviceAxlMotion::__AxmM3ServoSensOn(long lAxisNo)
{
	return AxmM3ServoSensOn(lAxisNo);
}

DWORD CDeviceAxlMotion::__AxmM3ServoSensOff(long lAxisNo)
{
	return AxmM3ServoSensOff(lAxisNo);
}

DWORD CDeviceAxlMotion::__AxmM3ServoSmon(long lAxisNo)
{
	return AxmM3ServoSmon(lAxisNo);
}

DWORD CDeviceAxlMotion::__AxmM3ServoGetSmon(long lAxisNo, BYTE *pbParam)
{
	return AxmM3ServoGetSmon(lAxisNo, pbParam);
}

DWORD CDeviceAxlMotion::__AxmM3ServoSvOn(long lAxisNo)
{
	return AxmM3ServoSvOn(lAxisNo);
}

DWORD CDeviceAxlMotion::__AxmM3ServoSvOff(long lAxisNo)
{
	return AxmM3ServoSvOff(lAxisNo);
}

DWORD CDeviceAxlMotion::__AxmM3ServoInterpolate(long lAxisNo, DWORD dwTPOS, DWORD dwVFF, DWORD dwTFF, DWORD dwTLIM)
{
	return AxmM3ServoInterpolate(lAxisNo, dwTPOS, dwVFF, dwTFF, dwTLIM);
}

DWORD CDeviceAxlMotion::__AxmM3ServoPosing(long lAxisNo, DWORD dwTPOS, DWORD dwSPD, DWORD dwACCR, DWORD dwDECR, DWORD dwTLIM)
{
	return AxmM3ServoPosing(lAxisNo, dwTPOS, dwSPD, dwACCR, dwDECR, dwTLIM);
}

DWORD CDeviceAxlMotion::__AxmM3ServoFeed(long lAxisNo, long lSPD, DWORD dwACCR, DWORD dwDECR, DWORD dwTLIM)
{
	return AxmM3ServoFeed(lAxisNo, lSPD, dwACCR, dwDECR, dwTLIM);
}


DWORD CDeviceAxlMotion::__AxmM3ServoExFeed(long lAxisNo, long lSPD, DWORD dwACCR, DWORD dwDECR, DWORD dwTLIM, DWORD dwExSig1, DWORD dwExSig2)
{
	return AxmM3ServoExFeed(lAxisNo, lSPD, dwACCR, dwDECR, dwTLIM, dwExSig1, dwExSig2);
}

DWORD CDeviceAxlMotion::__AxmM3ServoExPosing(long lAxisNo, DWORD dwTPOS, DWORD dwSPD, DWORD dwACCR, DWORD dwDECR, DWORD dwTLIM, DWORD dwExSig1, DWORD dwExSig2)
{
	return AxmM3ServoExPosing(lAxisNo, dwTPOS, dwSPD, dwACCR, dwDECR, dwTLIM, dwExSig1, dwExSig2);
}

DWORD CDeviceAxlMotion::__AxmM3ServoZret(long lAxisNo, DWORD dwSPD, DWORD dwACCR, DWORD dwDECR, DWORD dwTLIM, DWORD dwExSig1, DWORD dwExSig2, BYTE bHomeDir, BYTE bHomeType)
{
	return AxmM3ServoZret(lAxisNo, dwSPD, dwACCR, dwDECR, dwTLIM, dwExSig1, dwExSig2, bHomeDir, bHomeType);
}


DWORD CDeviceAxlMotion::__AxmM3ServoVelctrl(long lAxisNo, DWORD dwTFF, DWORD dwVREF, DWORD dwACCR, DWORD dwDECR, DWORD dwTLIM)
{
	return AxmM3ServoVelctrl(lAxisNo, dwTFF, dwVREF, dwACCR, dwDECR, dwTLIM);
}

DWORD CDeviceAxlMotion::__AxmM3ServoTrqctrl(long lAxisNo, DWORD dwVLIM, long lTQREF)
{
	return AxmM3ServoTrqctrl(lAxisNo, dwVLIM, lTQREF);
}

DWORD CDeviceAxlMotion::__AxmM3ServoGetParameter(long lAxisNo, WORD wNo, BYTE bSize, BYTE bMode, BYTE *pbParam)
{
	return AxmM3ServoGetParameter(lAxisNo, wNo, bSize, bMode, pbParam);
}

DWORD CDeviceAxlMotion::__AxmM3ServoSetParameter(long lAxisNo, WORD wNo, BYTE bSize, BYTE bMode, BYTE *pbParam)
{
	return AxmM3ServoSetParameter(lAxisNo, wNo, bSize, bMode, pbParam);
}

DWORD CDeviceAxlMotion::__AxmServoCmdExecution(long lAxisNo, DWORD dwCommand, DWORD dwSize, DWORD *pdwExcData)
{
	return AxmServoCmdExecution(lAxisNo, dwCommand, dwSize, pdwExcData);
}

DWORD CDeviceAxlMotion::__AxmM3ServoGetTorqLimit(long lAxisNo, DWORD *dwpTorqLimit)
{
	return AxmM3ServoGetTorqLimit(lAxisNo, dwpTorqLimit);
}

DWORD CDeviceAxlMotion::__AxmM3ServoSetTorqLimit(long lAxisNo, DWORD dwTorqLimit)
{
	return AxmM3ServoSetTorqLimit(lAxisNo, dwTorqLimit);
}

DWORD CDeviceAxlMotion::__AxmM3ServoGetSendSvCmdIOOutput(long lAxisNo, DWORD *dwData)
{
	return AxmM3ServoGetSendSvCmdIOOutput(lAxisNo, dwData);
}

DWORD CDeviceAxlMotion::__AxmM3ServoSetSendSvCmdIOOutput(long lAxisNo, DWORD dwData)
{
	return AxmM3ServoSetSendSvCmdIOOutput(lAxisNo, dwData);
}

DWORD CDeviceAxlMotion::__AxmM3ServoGetSvCmdCtrl(long lAxisNo, DWORD *dwData)
{
	return AxmM3ServoGetSvCmdCtrl(lAxisNo, dwData);
}

DWORD CDeviceAxlMotion::__AxmM3ServoSetSvCmdCtrl(long lAxisNo, DWORD dwData)
{
	return AxmM3ServoSetSvCmdCtrl(lAxisNo, dwData);
}

DWORD CDeviceAxlMotion::__AxmM3AdjustmentOperation(long lAxisNo, DWORD dwReqCode)
{
	return AxmM3AdjustmentOperation(lAxisNo, dwReqCode);
}

DWORD CDeviceAxlMotion::__AxmM3ServoSetMonSel(long lAxisNo, DWORD dwMon0, DWORD dwMon1, DWORD dwMon2)
{
	return AxmM3ServoSetMonSel(lAxisNo, dwMon0, dwMon1, dwMon2);
}

DWORD CDeviceAxlMotion::__AxmM3ServoGetMonSel(long lAxisNo, DWORD *upMon0, DWORD *upMon1, DWORD *upMon2)
{
	return AxmM3ServoGetMonSel(lAxisNo, upMon0, upMon1, upMon2);
}

DWORD CDeviceAxlMotion::__AxmM3ServoReadMonData(long lAxisNo, DWORD dwMonSel, DWORD *dwpMonData)
{
	return AxmM3ServoReadMonData(lAxisNo, dwMonSel, dwpMonData);
}

DWORD CDeviceAxlMotion::__AxmAdvTorqueContiSetAxisMap(long lCoord, long lSize, long *lpAxesNo, DWORD dwTLIM, DWORD dwConMode)
{
	return AxmAdvTorqueContiSetAxisMap(lCoord, lSize, lpAxesNo, dwTLIM, dwConMode);
}

DWORD CDeviceAxlMotion::__AxmM3ServoSetTorqProfile(long lCoord, long lAxisNo, long TorqueSign, DWORD dwVLIM, DWORD dwProfileMode, DWORD dwStdTorq, DWORD dwStopTorq)
{
	return AxmM3ServoSetTorqProfile(lCoord, lAxisNo, TorqueSign, dwVLIM, dwProfileMode, dwStdTorq, dwStopTorq);
}

DWORD CDeviceAxlMotion::__AxmM3ServoGetTorqProfile(long lCoord, long lAxisNo, long *lpTorqueSign, DWORD *updwVLIM, DWORD *upProfileMode, DWORD *upStdTorq, DWORD *upStopTorq)
{
	return AxmM3ServoGetTorqProfile(lCoord, lAxisNo, lpTorqueSign, updwVLIM, upProfileMode, upStdTorq, upStopTorq);
}


DWORD CDeviceAxlMotion::__AxmSignalSetInposRange(long lAxisNo, double dInposRange)
{
	return AxmSignalSetInposRange(lAxisNo, dInposRange);
}

DWORD CDeviceAxlMotion::__AxmSignalGetInposRange(long lAxisNo, double *dpInposRange)
{
	return AxmSignalGetInposRange(lAxisNo, dpInposRange);
}

DWORD CDeviceAxlMotion::__AxmMotSetOverridePosMode(long lAxisNo, DWORD dwAbsRelMode)
{
	return AxmMotSetOverridePosMode(lAxisNo, dwAbsRelMode);
}

DWORD CDeviceAxlMotion::__AxmMotGetOverridePosMode(long lAxisNo, DWORD *dwpAbsRelMode)
{
	return AxmMotGetOverridePosMode(lAxisNo, dwpAbsRelMode);
}

DWORD CDeviceAxlMotion::__AxmMotSetOverrideLinePosMode(long lCoordNo, DWORD dwAbsRelMode)
{
	return AxmMotSetOverrideLinePosMode(lCoordNo, dwAbsRelMode);
}

DWORD CDeviceAxlMotion::__AxmMotGetOverrideLinePosMode(long lCoordNo, DWORD *dwpAbsRelMode)
{
	return AxmMotGetOverrideLinePosMode(lCoordNo, dwpAbsRelMode);
}

DWORD CDeviceAxlMotion::__AxmMoveStartPosEx(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel, double dEndVel)
{
	return AxmMoveStartPosEx(lAxisNo, dPos, dVel, dAccel, dDecel, dEndVel);
}

DWORD CDeviceAxlMotion::__AxmMovePosEx(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel, double dEndVel)
{
	return AxmMovePosEx(lAxisNo, dPos, dVel, dAccel, dDecel, dEndVel);
}

DWORD CDeviceAxlMotion::__AxmMoveCoordStop(long lCoordNo, double dDecel)
{
	return AxmMoveCoordStop(lCoordNo, dDecel);
}

DWORD CDeviceAxlMotion::__AxmMoveCoordEStop(long lCoordNo)
{
	return AxmMoveCoordEStop(lCoordNo);
}

DWORD CDeviceAxlMotion::__AxmMoveCoordSStop(long lCoordNo)
{
	return AxmMoveCoordSStop(lCoordNo);
}

DWORD CDeviceAxlMotion::__AxmOverrideLinePos(long lCoordNo, double *dpOverridePos)
{
	return AxmOverrideLinePos(lCoordNo, dpOverridePos);
}

DWORD CDeviceAxlMotion::__AxmOverrideLineVel(long lCoordNo, double dOverrideVel, double *dpDistance)
{
	return AxmOverrideLineVel(lCoordNo, dOverrideVel, dpDistance);
}


DWORD CDeviceAxlMotion::__AxmOverrideLineAccelVelDecel(long lCoordNo, double dOverrideVelocity, double dMaxAccel, double dMaxDecel, double *dpDistance)
{
	return AxmOverrideLineAccelVelDecel(lCoordNo, dOverrideVelocity, dMaxAccel, dMaxDecel, dpDistance);
}

DWORD CDeviceAxlMotion::__AxmOverrideAccelVelDecelAtPos(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel, double dOverridePos, double dOverrideVel, double dOverrideAccel, double dOverrideDecel, long lTarget)
{
	return AxmOverrideAccelVelDecelAtPos(lAxisNo, dPos, dVel, dAccel, dDecel, dOverridePos, dOverrideVel, dOverrideAccel, dOverrideDecel, lTarget);
}


DWORD CDeviceAxlMotion::__AxmEGearSet(long lMasterAxisNo, long lSize, long* lpSlaveAxisNo, double* dpGearRatio)
{
	return AxmEGearSet(lMasterAxisNo, lSize, lpSlaveAxisNo, dpGearRatio);
}

DWORD CDeviceAxlMotion::__AxmEGearGet(long lMasterAxisNo, long* lpSize, long* lpSlaveAxisNo, double* dpGearRatio)
{
	return AxmEGearGet(lMasterAxisNo, lpSize, lpSlaveAxisNo, dpGearRatio);
}

DWORD CDeviceAxlMotion::__AxmEGearReset(long lMasterAxisNo)
{
	return AxmEGearReset(lMasterAxisNo);
}

DWORD CDeviceAxlMotion::__AxmEGearEnable(long lMasterAxisNo, DWORD dwEnable)
{
	return AxmEGearEnable(lMasterAxisNo, dwEnable);
}

DWORD CDeviceAxlMotion::__AxmEGearIsEnable(long lMasterAxisNo, DWORD *dwpEnable)
{
	return AxmEGearIsEnable(lMasterAxisNo, dwpEnable);
}

DWORD CDeviceAxlMotion::__AxmMotSetEndVel(long lAxisNo, double dEndVelocity)
{
	return AxmMotSetEndVel(lAxisNo, dEndVelocity);
}

DWORD CDeviceAxlMotion::__AxmMotGetEndVel(long lAxisNo, double *dpEndVelocity)
{
	return AxmMotGetEndVel(lAxisNo, dpEndVelocity);
}

EGetFunction CDeviceAxlMotion::GetOpenNoHardware(bool * pParam)
{
	EGetFunction eReturn = EGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAxlMotion_OpenNoReset));

		eReturn = EGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESetFunction CDeviceAxlMotion::SetOpenNoHardware(bool bParam)
{
	ESetFunction eReturn = ESetFunction_UnknownError;

	EDeviceParameterAxlMotion eSaveNum = EDeviceParameterAxlMotion_OpenNoReset;

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotion[eSaveNum], g_lpszSwitch[bPreValue], g_lpszSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

#endif