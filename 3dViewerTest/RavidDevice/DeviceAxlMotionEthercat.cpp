#include "stdafx.h"

#include "DeviceAxlMotionEthercat.h"

#ifdef USING_DEVICE

#include "DeviceAxlDio.h"
#include "DeviceAxlMotion.h"
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

IMPLEMENT_DYNAMIC(CDeviceAxlMotionEthercat, CDeviceMotion)

BEGIN_MESSAGE_MAP(CDeviceAxlMotionEthercat, CDeviceMotion)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static LPCTSTR g_lpszParamAxlMotionEthercat[EDeviceParameterAxlMotionEthercat_Count] =
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

static LPCTSTR g_lpszAxlMotionEthercatPulseOutputMethod[EDeviceAxlMotionEthercatPulseOutputMethod_Count] =
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

static LPCTSTR g_lpszAxlMotionEthercatEncoderMethod[EDeviceAxlMotionEthercatEncoderMethod_Count] =
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

static LPCTSTR g_lpszAxlMotionEthercatLevelMethod[EDeviceAxlMotionEthercatLevelMethod_Count] =
{
	_T("LOW"),
	_T("HIGH"),
};

static LPCTSTR g_lpszAxlMotionEthercatDirection[EDeviceAxlMotionEthercatDirection_Count] =
{
	_T("CCW"),
	_T("CW"),
};

static LPCTSTR g_lpszAxlMotionEthercatZPhase[EDeviceAxlMotionEthercatZPhase_Count] =
{
	_T("NotUsed"),
	_T("ReverseHomeDir"),
	_T("HomeDir"),
};

static LPCTSTR g_lpszAxlMotionEthercatSearchSensor[EDeviceAxlMotionEthercatSearchSensor_Count] =
{
	_T("Pos Limit"),
	_T("Neg Limit"),
	_T("HomeSensor"),
	_T("EncoderZPhase"),
};

static LPCTSTR g_lpszAxlMotionEthercatEnableDisable[EDeviceAxlMotionEthercatEnableDisable_Count] =
{
	_T("Disable"),
	_T("Enable"),
};

static LPCTSTR g_lpszAxlMotionEthercatStopMode[EDeviceAxlMotionEthercatStopMode_Count] =
{
	_T("Emergency Stop"),
	_T("Slowdown Stop"),
};

static LPCTSTR g_lpszAxlMotionEthercatCounterSelection[EDeviceAxlMotionEthercatCounterSelection_Count] =
{
	_T("Command Pos"),
	_T("Actual Pos"),
};

static LPCTSTR g_lpszSwitch[EDeviceDIOSwitch_Count] =
{
	_T("Off"),
	_T("On")
};




CDeviceAxlMotionEthercat::CDeviceAxlMotionEthercat()
{
}


CDeviceAxlMotionEthercat::~CDeviceAxlMotionEthercat()
{
}

EDeviceInitializeResult CDeviceAxlMotionEthercat::Initialize()
{
	EDeviceInitializeResult eReturn = EDeviceInitializeResult_UnknownError;

	CString strMessage, strStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("AxlMotionEthercat"));

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

		EDeviceAxlMotionEthercatLevelMethod eLevelHome = EDeviceAxlMotionEthercatLevelMethod_Count;

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

		EDeviceAxlMotionEthercatLevelMethod ePLimitLevel = EDeviceAxlMotionEthercatLevelMethod_Count;

		if(GetPLimitLevel(&ePLimitLevel))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("P limit level"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		EDeviceAxlMotionEthercatLevelMethod eNLimitLevel = EDeviceAxlMotionEthercatLevelMethod_Count;

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

		EDeviceAxlMotionEthercatLevelMethod eInpositionInputLevel = EDeviceAxlMotionEthercatLevelMethod_Count;

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

		EDeviceAxlMotionEthercatLevelMethod eAlarmInputLevel = EDeviceAxlMotionEthercatLevelMethod_Count;

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

		bool bSoftwareLimit = false;

		if(GetEnableSoftwareLimit(&bSoftwareLimit))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("EnableSoftLimit"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(bSoftwareLimit)
		{
			EDeviceAxlMotionEthercatStopMode eStopMode = EDeviceAxlMotionEthercatStopMode_EmergencyStop;

			EDeviceAxlMotionEthercatCounterSelection eCounterSelection = EDeviceAxlMotionEthercatCounterSelection_Command;

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

		bool bServoOn = false;


		if(0)
			AxmSignalServoOn(nModuleID, bServoOn);

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

EDeviceTerminateResult CDeviceAxlMotionEthercat::Terminate()
{
	EDeviceTerminateResult eReturn = EDeviceTerminateResult_UnknownError;

	CString strMessage;

	SetStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("AxlMotionEthercat"));

		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);

		return EDeviceTerminateResult_NotFoundApiError;
	}

	do
	{
		if(0)
			SetServoOn(false);

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

bool CDeviceAxlMotionEthercat::LoadSettings()
{
	bool bReturn = false;

	m_vctParameterFieldConfigurations.clear();

	do
	{
		AddParameterFieldConfigurations(EDeviceParameterAxlMotionEthercat_DeviceID, g_lpszParamAxlMotionEthercat[EDeviceParameterAxlMotionEthercat_DeviceID], _T("0"), EParameterFieldType_Edit);
		AddParameterFieldConfigurations(EDeviceParameterAxlMotionEthercat_SubUnitID, g_lpszParamAxlMotionEthercat[EDeviceParameterAxlMotionEthercat_SubUnitID], _T("0"), EParameterFieldType_Edit);
		AddParameterFieldConfigurations(EDeviceParameterAxlMotionEthercat_OpenNoReset, g_lpszParamAxlMotionEthercat[EDeviceParameterAxlMotionEthercat_OpenNoReset], _T("0"), EParameterFieldType_Check, nullptr, _T("Initializes the hardware when initializing the device."), 0);

		AddParameterFieldConfigurations(EDeviceParameterAxlMotionEthercat_UnitPulse, g_lpszParamAxlMotionEthercat[EDeviceParameterAxlMotionEthercat_UnitPulse], _T("0"), EParameterFieldType_Edit, nullptr, _T("double type"));
		AddParameterFieldConfigurations(EDeviceParameterAxlMotionEthercat_StartSpeed, g_lpszParamAxlMotionEthercat[EDeviceParameterAxlMotionEthercat_StartSpeed], _T("0"), EParameterFieldType_Edit, nullptr, _T("double type"));
		AddParameterFieldConfigurations(EDeviceParameterAxlMotionEthercat_MaxSpeed, g_lpszParamAxlMotionEthercat[EDeviceParameterAxlMotionEthercat_MaxSpeed], _T("0"), EParameterFieldType_Edit, nullptr, _T("double type"));

		AddParameterFieldConfigurations(EDeviceParameterAxlMotionEthercat_PulseOutputMethod, g_lpszParamAxlMotionEthercat[EDeviceParameterAxlMotionEthercat_PulseOutputMethod], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxlMotionEthercatPulseOutputMethod, EDeviceAxlMotionEthercatPulseOutputMethod_Count), _T("ex) OneHighLowHigh = 1 pulse, PULSE(Active High), cw(DIR=Low)  / ccw(DIR=High)"));
		AddParameterFieldConfigurations(EDeviceParameterAxlMotionEthercat_EncoderMethod, g_lpszParamAxlMotionEthercat[EDeviceParameterAxlMotionEthercat_EncoderMethod], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxlMotionEthercatEncoderMethod, EDeviceAxlMotionEthercatEncoderMethod_Count), nullptr);

		AddParameterFieldConfigurations(EDeviceParameterAxlMotionEthercat_ServoLevel, g_lpszParamAxlMotionEthercat[EDeviceParameterAxlMotionEthercat_ServoLevel], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxlMotionEthercatLevelMethod, EDeviceAxlMotionEthercatLevelMethod_Count), _T("integer type"));
		AddParameterFieldConfigurations(EDeviceParameterAxlMotionEthercat_HomeLevel, g_lpszParamAxlMotionEthercat[EDeviceParameterAxlMotionEthercat_HomeLevel], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxlMotionEthercatLevelMethod, EDeviceAxlMotionEthercatLevelMethod_Count), _T("integer type"));
		AddParameterFieldConfigurations(EDeviceParameterAxlMotionEthercat_PLimitLevel, g_lpszParamAxlMotionEthercat[EDeviceParameterAxlMotionEthercat_PLimitLevel], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxlMotionEthercatLevelMethod, EDeviceAxlMotionEthercatLevelMethod_Count), _T("integer type"));
		AddParameterFieldConfigurations(EDeviceParameterAxlMotionEthercat_NLimitLevel, g_lpszParamAxlMotionEthercat[EDeviceParameterAxlMotionEthercat_NLimitLevel], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxlMotionEthercatLevelMethod, EDeviceAxlMotionEthercatLevelMethod_Count), _T("integer type"));
		AddParameterFieldConfigurations(EDeviceParameterAxlMotionEthercat_AlarmInputLevel, g_lpszParamAxlMotionEthercat[EDeviceParameterAxlMotionEthercat_AlarmInputLevel], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxlMotionEthercatLevelMethod, EDeviceAxlMotionEthercatLevelMethod_Count), _T("integer type"));
		AddParameterFieldConfigurations(EDeviceParameterAxlMotionEthercat_AlarmResetLevel, g_lpszParamAxlMotionEthercat[EDeviceParameterAxlMotionEthercat_AlarmResetLevel], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxlMotionEthercatLevelMethod, EDeviceAxlMotionEthercatLevelMethod_Count), _T("integer type"));
		AddParameterFieldConfigurations(EDeviceParameterAxlMotionEthercat_InpositionInputLevel, g_lpszParamAxlMotionEthercat[EDeviceParameterAxlMotionEthercat_InpositionInputLevel], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxlMotionEthercatLevelMethod, EDeviceAxlMotionEthercatLevelMethod_Count), _T("integer type"));

		AddParameterFieldConfigurations(EDeviceParameterAxlMotionEthercat_LimitParameter, g_lpszParamAxlMotionEthercat[EDeviceParameterAxlMotionEthercat_LimitParameter], _T("0"), EParameterFieldType_None, nullptr, nullptr, 0);
		{
			AddParameterFieldConfigurations(EDeviceParameterAxlMotionEthercat_UseSoftwareLimit, g_lpszParamAxlMotionEthercat[EDeviceParameterAxlMotionEthercat_UseSoftwareLimit], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterAxlMotionEthercat_StopMode, g_lpszParamAxlMotionEthercat[EDeviceParameterAxlMotionEthercat_StopMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxlMotionEthercatStopMode, EDeviceAxlMotionEthercatStopMode_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterAxlMotionEthercat_CounterSelection, g_lpszParamAxlMotionEthercat[EDeviceParameterAxlMotionEthercat_CounterSelection], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxlMotionEthercatCounterSelection, EDeviceAxlMotionEthercatCounterSelection_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterAxlMotionEthercat_PosLimitValue, g_lpszParamAxlMotionEthercat[EDeviceParameterAxlMotionEthercat_PosLimitValue], _T("0"), EParameterFieldType_Edit, nullptr, _T("double type"), 1);
			AddParameterFieldConfigurations(EDeviceParameterAxlMotionEthercat_NegLimitValue, g_lpszParamAxlMotionEthercat[EDeviceParameterAxlMotionEthercat_NegLimitValue], _T("0"), EParameterFieldType_Edit, nullptr, _T("double type"), 1);
		}

		AddParameterFieldConfigurations(EDeviceParameterAxlMotionEthercat_HomeParameter, g_lpszParamAxlMotionEthercat[EDeviceParameterAxlMotionEthercat_HomeParameter], _T("0"), EParameterFieldType_None, nullptr, nullptr, 0);
		{
			AddParameterFieldConfigurations(EDeviceParameterAxlMotionEthercat_HomeSearchSensor, g_lpszParamAxlMotionEthercat[EDeviceParameterAxlMotionEthercat_HomeSearchSensor], _T("2"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxlMotionEthercatSearchSensor, EDeviceAxlMotionEthercatSearchSensor_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterAxlMotionEthercat_HomeDirection, g_lpszParamAxlMotionEthercat[EDeviceParameterAxlMotionEthercat_HomeDirection], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxlMotionEthercatDirection, EDeviceAxlMotionEthercatDirection_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterAxlMotionEthercat_HomeZphase, g_lpszParamAxlMotionEthercat[EDeviceParameterAxlMotionEthercat_HomeZphase], _T("1"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxlMotionEthercatZPhase, EDeviceAxlMotionEthercatZPhase_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterAxlMotionEthercat_HomeHomeClearTime, g_lpszParamAxlMotionEthercat[EDeviceParameterAxlMotionEthercat_HomeHomeClearTime], _T("1000.000000"), EParameterFieldType_Edit, nullptr, _T("double"), 1);
			AddParameterFieldConfigurations(EDeviceParameterAxlMotionEthercat_HomeOffset, g_lpszParamAxlMotionEthercat[EDeviceParameterAxlMotionEthercat_HomeOffset], _T("0.000000"), EParameterFieldType_Edit, nullptr, _T("double"), 1);

			AddParameterFieldConfigurations(EDeviceParameterAxlMotionEthercat_HomeFirstVel, g_lpszParamAxlMotionEthercat[EDeviceParameterAxlMotionEthercat_HomeFirstVel], _T("40.000000"), EParameterFieldType_Edit, nullptr, _T("double"), 1);
			AddParameterFieldConfigurations(EDeviceParameterAxlMotionEthercat_HomeSecondVel, g_lpszParamAxlMotionEthercat[EDeviceParameterAxlMotionEthercat_HomeSecondVel], _T("20.000000"), EParameterFieldType_Edit, nullptr, _T("double"), 1);
			AddParameterFieldConfigurations(EDeviceParameterAxlMotionEthercat_HomeThirdVel, g_lpszParamAxlMotionEthercat[EDeviceParameterAxlMotionEthercat_HomeThirdVel], _T("10.000000"), EParameterFieldType_Edit, nullptr, _T("double"), 1);
			AddParameterFieldConfigurations(EDeviceParameterAxlMotionEthercat_HomeLastVel, g_lpszParamAxlMotionEthercat[EDeviceParameterAxlMotionEthercat_HomeLastVel], _T("5.000000"), EParameterFieldType_Edit, nullptr, _T("double"), 1);
			AddParameterFieldConfigurations(EDeviceParameterAxlMotionEthercat_HomeStartAcc, g_lpszParamAxlMotionEthercat[EDeviceParameterAxlMotionEthercat_HomeStartAcc], _T("40.000000"), EParameterFieldType_Edit, nullptr, _T("double"), 1);
			AddParameterFieldConfigurations(EDeviceParameterAxlMotionEthercat_HomeEndAcc, g_lpszParamAxlMotionEthercat[EDeviceParameterAxlMotionEthercat_HomeEndAcc], _T("10.000000"), EParameterFieldType_Edit, nullptr, _T("double"), 1);
		}

		bReturn = true;
	}
	while(false);

	return bReturn & __super::LoadSettings();
}

AxlMotionEthercatGetFunction CDeviceAxlMotionEthercat::GetUnitPulse(double* pParam)
{
	AxlMotionEthercatGetFunction eReturn = AxlMotionEthercatGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = AxlMotionEthercatGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterAxlMotionEthercat_UnitPulse));

		eReturn = AxlMotionEthercatGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionEthercatSetFunction CDeviceAxlMotionEthercat::SetUnitPulse(double dblParam)
{
	AxlMotionEthercatSetFunction eReturn = AxlMotionEthercatSetFunction_UnknownError;

	EDeviceParameterAxlMotionEthercat eSaveID = EDeviceParameterAxlMotionEthercat_UnitPulse;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
			AxmMotSetMoveUnitPerPulse(_ttoi(GetSubUnitID()), dblParam, 1);

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionEthercatSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotionEthercat[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionEthercatGetFunction CDeviceAxlMotionEthercat::GetStartSpeed(double* pParam)
{
	AxlMotionEthercatGetFunction eReturn = AxlMotionEthercatGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = AxlMotionEthercatGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterAxlMotionEthercat_StartSpeed));

		eReturn = AxlMotionEthercatGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionEthercatSetFunction CDeviceAxlMotionEthercat::SetStartSpeed(double dblParam)
{
	AxlMotionEthercatSetFunction eReturn = AxlMotionEthercatSetFunction_UnknownError;

	EDeviceParameterAxlMotionEthercat eSaveID = EDeviceParameterAxlMotionEthercat_StartSpeed;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
			AxmMotSetMinVel(_ttoi(GetSubUnitID()), dblParam);

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionEthercatSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotionEthercat[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionEthercatGetFunction CDeviceAxlMotionEthercat::GetMaxSpeed(double* pParam)
{
	AxlMotionEthercatGetFunction eReturn = AxlMotionEthercatGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = AxlMotionEthercatGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterAxlMotionEthercat_MaxSpeed));

		eReturn = AxlMotionEthercatGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionEthercatSetFunction CDeviceAxlMotionEthercat::SetMaxSpeed(double dblParam)
{
	AxlMotionEthercatSetFunction eReturn = AxlMotionEthercatSetFunction_UnknownError;

	EDeviceParameterAxlMotionEthercat eSaveID = EDeviceParameterAxlMotionEthercat_MaxSpeed;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
			AxmMotSetMaxVel(_ttoi(GetSubUnitID()), dblParam);

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionEthercatSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotionEthercat[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionEthercatGetFunction CDeviceAxlMotionEthercat::GetPulseOutputMethod(EDeviceAxlMotionEthercatPulseOutputMethod* pParam)
{
	AxlMotionEthercatGetFunction eReturn = AxlMotionEthercatGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = AxlMotionEthercatGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxlMotionEthercatPulseOutputMethod)_ttoi(GetParamValue(EDeviceParameterAxlMotionEthercat_PulseOutputMethod));

		eReturn = AxlMotionEthercatGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionEthercatSetFunction CDeviceAxlMotionEthercat::SetPulseOutputMethod(EDeviceAxlMotionEthercatPulseOutputMethod eParam)
{
	AxlMotionEthercatSetFunction eReturn = AxlMotionEthercatSetFunction_UnknownError;

	EDeviceParameterAxlMotionEthercat eSaveID = EDeviceParameterAxlMotionEthercat_PulseOutputMethod;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxlMotionEthercatPulseOutputMethod_Count)
		{
			eReturn = AxlMotionEthercatSetFunction_NotFoundCommandError;
			break;
		}

		if(IsInitialized())
			AxmMotSetPulseOutMethod(_ttoi(GetSubUnitID()), (int)eParam);

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionEthercatSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotionEthercat[eSaveID], g_lpszAxlMotionEthercatPulseOutputMethod[nPreValue], g_lpszAxlMotionEthercatPulseOutputMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionEthercatGetFunction CDeviceAxlMotionEthercat::GetEncoderMethod(EDeviceAxlMotionEthercatEncoderMethod* pParam)
{
	AxlMotionEthercatGetFunction eReturn = AxlMotionEthercatGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = AxlMotionEthercatGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxlMotionEthercatEncoderMethod)_ttoi(GetParamValue(EDeviceParameterAxlMotionEthercat_EncoderMethod));

		eReturn = AxlMotionEthercatGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionEthercatSetFunction CDeviceAxlMotionEthercat::SetEncoderMethod(EDeviceAxlMotionEthercatEncoderMethod eParam)
{
	AxlMotionEthercatSetFunction eReturn = AxlMotionEthercatSetFunction_UnknownError;

	EDeviceParameterAxlMotionEthercat eSaveID = EDeviceParameterAxlMotionEthercat_EncoderMethod;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxlMotionEthercatEncoderMethod_Count)
		{
			eReturn = AxlMotionEthercatSetFunction_NotFoundCommandError;
			break;
		}

		if(IsInitialized())
			AxmMotSetEncInputMethod(_ttoi(GetSubUnitID()), (int)eParam);

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionEthercatSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotionEthercat[eSaveID], g_lpszAxlMotionEthercatEncoderMethod[nPreValue], g_lpszAxlMotionEthercatEncoderMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionEthercatGetFunction CDeviceAxlMotionEthercat::GetServoLevel(EDeviceAxlMotionEthercatLevelMethod* pParam)
{
	AxlMotionEthercatGetFunction eReturn = AxlMotionEthercatGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = AxlMotionEthercatGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxlMotionEthercatLevelMethod)_ttoi(GetParamValue(EDeviceParameterAxlMotionEthercat_ServoLevel));

		eReturn = AxlMotionEthercatGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionEthercatSetFunction CDeviceAxlMotionEthercat::SetServoLevel(EDeviceAxlMotionEthercatLevelMethod eParam)
{
	AxlMotionEthercatSetFunction eReturn = AxlMotionEthercatSetFunction_UnknownError;

	EDeviceParameterAxlMotionEthercat eSaveID = EDeviceParameterAxlMotionEthercat_ServoLevel;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxlMotionEthercatLevelMethod_Count)
		{
			eReturn = AxlMotionEthercatSetFunction_NotFoundCommandError;
			break;
		}

		if(IsInitialized())
			AxmSignalSetServoOnLevel(_ttoi(GetSubUnitID()), (int)eParam);

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionEthercatSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotionEthercat[eSaveID], g_lpszAxlMotionEthercatLevelMethod[nPreValue], g_lpszAxlMotionEthercatLevelMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionEthercatGetFunction CDeviceAxlMotionEthercat::GetHomeLevel(EDeviceAxlMotionEthercatLevelMethod* pParam)
{
	AxlMotionEthercatGetFunction eReturn = AxlMotionEthercatGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = AxlMotionEthercatGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxlMotionEthercatLevelMethod)_ttoi(GetParamValue(EDeviceParameterAxlMotionEthercat_HomeLevel));

		eReturn = AxlMotionEthercatGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionEthercatSetFunction CDeviceAxlMotionEthercat::SetHomeLevel(EDeviceAxlMotionEthercatLevelMethod eParam)
{
	AxlMotionEthercatSetFunction eReturn = AxlMotionEthercatSetFunction_UnknownError;

	EDeviceParameterAxlMotionEthercat eSaveID = EDeviceParameterAxlMotionEthercat_HomeLevel;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxlMotionEthercatLevelMethod_Count)
		{
			eReturn = AxlMotionEthercatSetFunction_NotFoundCommandError;
			break;
		}

		if(IsInitialized())
			AxmHomeSetSignalLevel(_ttoi(GetSubUnitID()), (int)eParam);

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionEthercatSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotionEthercat[eSaveID], g_lpszAxlMotionEthercatLevelMethod[nPreValue], g_lpszAxlMotionEthercatLevelMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionEthercatGetFunction CDeviceAxlMotionEthercat::GetPLimitLevel(EDeviceAxlMotionEthercatLevelMethod* pParam)
{
	AxlMotionEthercatGetFunction eReturn = AxlMotionEthercatGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = AxlMotionEthercatGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxlMotionEthercatLevelMethod)_ttoi(GetParamValue(EDeviceParameterAxlMotionEthercat_PLimitLevel));

		eReturn = AxlMotionEthercatGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionEthercatSetFunction CDeviceAxlMotionEthercat::SetPLimitLevel(EDeviceAxlMotionEthercatLevelMethod eParam)
{
	AxlMotionEthercatSetFunction eReturn = AxlMotionEthercatSetFunction_UnknownError;

	EDeviceParameterAxlMotionEthercat eSaveID = EDeviceParameterAxlMotionEthercat_PLimitLevel;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxlMotionEthercatLevelMethod_Count)
		{
			eReturn = AxlMotionEthercatSetFunction_NotFoundCommandError;
			break;
		}

		if(IsInitialized())
		{
			EDeviceAxlMotionEthercatLevelMethod eLevelMethod = EDeviceAxlMotionEthercatLevelMethod_Count;

			if(GetNLimitLevel(&eLevelMethod))
			{
				eReturn = AxlMotionEthercatSetFunction_ReadOnDatabaseError;
				break;
			}

			if(eLevelMethod == EDeviceAxlMotionEthercatLevelMethod_Count)
			{
				eReturn = AxlMotionEthercatSetFunction_ReadOnDatabaseError;
				break;
			}

			AxmSignalSetLimit(_ttoi(GetSubUnitID()), EMERGENCY_STOP, (int)eParam, eLevelMethod);
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionEthercatSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotionEthercat[eSaveID], g_lpszAxlMotionEthercatLevelMethod[nPreValue], g_lpszAxlMotionEthercatLevelMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionEthercatGetFunction CDeviceAxlMotionEthercat::GetNLimitLevel(EDeviceAxlMotionEthercatLevelMethod* pParam)
{
	AxlMotionEthercatGetFunction eReturn = AxlMotionEthercatGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = AxlMotionEthercatGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxlMotionEthercatLevelMethod)_ttoi(GetParamValue(EDeviceParameterAxlMotionEthercat_NLimitLevel));

		eReturn = AxlMotionEthercatGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionEthercatSetFunction CDeviceAxlMotionEthercat::SetNLimitLevel(EDeviceAxlMotionEthercatLevelMethod eParam)
{
	AxlMotionEthercatSetFunction eReturn = AxlMotionEthercatSetFunction_UnknownError;

	EDeviceParameterAxlMotionEthercat eSaveID = EDeviceParameterAxlMotionEthercat_NLimitLevel;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxlMotionEthercatLevelMethod_Count)
		{
			eReturn = AxlMotionEthercatSetFunction_NotFoundCommandError;
			break;
		}

		if(IsInitialized())
		{
			EDeviceAxlMotionEthercatLevelMethod eLevelMethod = EDeviceAxlMotionEthercatLevelMethod_Count;

			if(GetPLimitLevel(&eLevelMethod))
			{
				eReturn = AxlMotionEthercatSetFunction_ReadOnDatabaseError;
				break;
			}

			AxmSignalSetLimit(_ttoi(GetSubUnitID()), EMERGENCY_STOP, eLevelMethod, (int)eParam);
		}
		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionEthercatSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotionEthercat[eSaveID], g_lpszAxlMotionEthercatLevelMethod[nPreValue], g_lpszAxlMotionEthercatLevelMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionEthercatGetFunction CDeviceAxlMotionEthercat::GetAlarmInputLevel(EDeviceAxlMotionEthercatLevelMethod* pParam)
{
	AxlMotionEthercatGetFunction eReturn = AxlMotionEthercatGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = AxlMotionEthercatGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxlMotionEthercatLevelMethod)_ttoi(GetParamValue(EDeviceParameterAxlMotionEthercat_AlarmInputLevel));

		eReturn = AxlMotionEthercatGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionEthercatSetFunction CDeviceAxlMotionEthercat::SetAlarmInputLevel(EDeviceAxlMotionEthercatLevelMethod eParam)
{
	AxlMotionEthercatSetFunction eReturn = AxlMotionEthercatSetFunction_UnknownError;

	EDeviceParameterAxlMotionEthercat eSaveID = EDeviceParameterAxlMotionEthercat_AlarmInputLevel;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxlMotionEthercatLevelMethod_Count)
		{
			eReturn = AxlMotionEthercatSetFunction_NotFoundCommandError;
			break;
		}

		if(IsInitialized())
			AxmSignalSetServoAlarm(_ttoi(GetSubUnitID()), (int)eParam);

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionEthercatSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotionEthercat[eSaveID], g_lpszAxlMotionEthercatLevelMethod[nPreValue], g_lpszAxlMotionEthercatLevelMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionEthercatGetFunction CDeviceAxlMotionEthercat::GetAlarmResetLevel(EDeviceAxlMotionEthercatLevelMethod* pParam)
{
	AxlMotionEthercatGetFunction eReturn = AxlMotionEthercatGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = AxlMotionEthercatGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxlMotionEthercatLevelMethod)_ttoi(GetParamValue(EDeviceParameterAxlMotionEthercat_AlarmResetLevel));

		eReturn = AxlMotionEthercatGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionEthercatSetFunction CDeviceAxlMotionEthercat::SetAlarmResetLevel(EDeviceAxlMotionEthercatLevelMethod eParam)
{
	AxlMotionEthercatSetFunction eReturn = AxlMotionEthercatSetFunction_UnknownError;

	EDeviceParameterAxlMotionEthercat eSaveID = EDeviceParameterAxlMotionEthercat_AlarmResetLevel;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxlMotionEthercatLevelMethod_Count)
		{
			eReturn = AxlMotionEthercatSetFunction_NotFoundCommandError;
			break;
		}

		if(IsInitialized())
			AxmSignalSetServoAlarmResetLevel(_ttoi(GetSubUnitID()), (int)eParam);

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionEthercatSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotionEthercat[eSaveID], g_lpszAxlMotionEthercatLevelMethod[nPreValue], g_lpszAxlMotionEthercatLevelMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionEthercatGetFunction CDeviceAxlMotionEthercat::GetInpositionInputLevel(EDeviceAxlMotionEthercatLevelMethod* pParam)
{
	AxlMotionEthercatGetFunction eReturn = AxlMotionEthercatGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = AxlMotionEthercatGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxlMotionEthercatLevelMethod)_ttoi(GetParamValue(EDeviceParameterAxlMotionEthercat_InpositionInputLevel));

		eReturn = AxlMotionEthercatGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionEthercatSetFunction CDeviceAxlMotionEthercat::SetInpositionInputLevel(EDeviceAxlMotionEthercatLevelMethod eParam)
{
	AxlMotionEthercatSetFunction eReturn = AxlMotionEthercatSetFunction_UnknownError;

	EDeviceParameterAxlMotionEthercat eSaveID = EDeviceParameterAxlMotionEthercat_InpositionInputLevel;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxlMotionEthercatLevelMethod_Count)
		{
			eReturn = AxlMotionEthercatSetFunction_NotFoundCommandError;
			break;
		}

		if(IsInitialized())
			AxmSignalSetInpos(_ttoi(GetSubUnitID()), (int)eParam);

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionEthercatSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotionEthercat[eSaveID], g_lpszAxlMotionEthercatLevelMethod[nPreValue], g_lpszAxlMotionEthercatLevelMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionEthercatGetFunction CDeviceAxlMotionEthercat::GetEnableSoftwareLimit(bool* pParam)
{
	AxlMotionEthercatGetFunction eReturn = AxlMotionEthercatGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = AxlMotionEthercatGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAxlMotionEthercat_UseSoftwareLimit));

		eReturn = AxlMotionEthercatGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionEthercatSetFunction CDeviceAxlMotionEthercat::SetEnableSoftwareLimit(bool bParam)
{
	AxlMotionEthercatSetFunction eReturn = AxlMotionEthercatSetFunction_UnknownError;

	EDeviceParameterAxlMotionEthercat eSaveID = EDeviceParameterAxlMotionEthercat_UseSoftwareLimit;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			EDeviceAxlMotionEthercatStopMode eStopMode = EDeviceAxlMotionEthercatStopMode_EmergencyStop;

			EDeviceAxlMotionEthercatCounterSelection eCounterSelection = EDeviceAxlMotionEthercatCounterSelection_Command;

			double dblPositive = 0.0, dblNegative = 0.0;

			if(GetSoftwareStopMode(&eStopMode))
			{
				eReturn = AxlMotionEthercatSetFunction_ReadOnDatabaseError;
				break;
			}

			if(GetSoftwareCounterSelection(&eCounterSelection))
			{
				eReturn = AxlMotionEthercatSetFunction_ReadOnDatabaseError;
				break;
			}

			if(GetPositiveLimit(&dblPositive))
			{
				eReturn = AxlMotionEthercatSetFunction_ReadOnDatabaseError;
				break;
			}

			if(GetNegativeLimit(&dblNegative))
			{
				eReturn = AxlMotionEthercatSetFunction_ReadOnDatabaseError;
				break;
			}

			AxmSignalSetSoftLimit(_ttoi(GetSubUnitID()), bParam, (DWORD)eStopMode, (DWORD)eCounterSelection, dblPositive, dblNegative);
		}

		CString strData;
		strData.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionEthercatSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotionEthercat[eSaveID], g_lpszAxlMotionEthercatEnableDisable[nPreValue], g_lpszAxlMotionEthercatEnableDisable[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionEthercatGetFunction CDeviceAxlMotionEthercat::GetSoftwareStopMode(EDeviceAxlMotionEthercatStopMode * pParam)
{
	AxlMotionEthercatGetFunction eReturn = AxlMotionEthercatGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = AxlMotionEthercatGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxlMotionEthercatStopMode)_ttoi(GetParamValue(EDeviceParameterAxlMotionEthercat_StopMode));

		eReturn = AxlMotionEthercatGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionEthercatSetFunction CDeviceAxlMotionEthercat::SetSoftwareStopMode(EDeviceAxlMotionEthercatStopMode eParam)
{
	AxlMotionEthercatSetFunction eReturn = AxlMotionEthercatSetFunction_UnknownError;

	EDeviceParameterAxlMotionEthercat eSaveID = EDeviceParameterAxlMotionEthercat_StopMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxlMotionEthercatStopMode_Count)
		{
			eReturn = AxlMotionEthercatSetFunction_NotFoundCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(IsInitialized())
			{
				bool bEnable = false;

				EDeviceAxlMotionEthercatCounterSelection eCounterSelection = EDeviceAxlMotionEthercatCounterSelection_Command;

				double dblPositive = 0.0, dblNegative = 0.0;

				if(GetEnableSoftwareLimit(&bEnable))
				{
					eReturn = AxlMotionEthercatSetFunction_ReadOnDatabaseError;
					break;
				}

				if(GetSoftwareCounterSelection(&eCounterSelection))
				{
					eReturn = AxlMotionEthercatSetFunction_ReadOnDatabaseError;
					break;
				}

				if(GetPositiveLimit(&dblPositive))
				{
					eReturn = AxlMotionEthercatSetFunction_ReadOnDatabaseError;
					break;
				}

				if(GetNegativeLimit(&dblNegative))
				{
					eReturn = AxlMotionEthercatSetFunction_ReadOnDatabaseError;
					break;
				}

				AxmSignalSetSoftLimit(_ttoi(GetSubUnitID()), bEnable, (DWORD)eParam, (DWORD)eCounterSelection, dblPositive, dblNegative);
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionEthercatSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotionEthercat[eSaveID], g_lpszAxlMotionEthercatStopMode[nPreValue], g_lpszAxlMotionEthercatStopMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionEthercatGetFunction CDeviceAxlMotionEthercat::GetSoftwareCounterSelection(EDeviceAxlMotionEthercatCounterSelection * pParam)
{
	AxlMotionEthercatGetFunction eReturn = AxlMotionEthercatGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = AxlMotionEthercatGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxlMotionEthercatCounterSelection)_ttoi(GetParamValue(EDeviceParameterAxlMotionEthercat_CounterSelection));

		eReturn = AxlMotionEthercatGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionEthercatSetFunction CDeviceAxlMotionEthercat::SetSoftwareCounterSelection(EDeviceAxlMotionEthercatCounterSelection eParam)
{
	AxlMotionEthercatSetFunction eReturn = AxlMotionEthercatSetFunction_UnknownError;

	EDeviceParameterAxlMotionEthercat eSaveID = EDeviceParameterAxlMotionEthercat_CounterSelection;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxlMotionEthercatCounterSelection_Count)
		{
			eReturn = AxlMotionEthercatSetFunction_NotFoundCommandError;
			break;
		}

		if(IsInitialized())
		{
			bool bEnable = false;

			EDeviceAxlMotionEthercatStopMode eStopMode = EDeviceAxlMotionEthercatStopMode_EmergencyStop;

			double dblPositive = 0.0, dblNegative = 0.0;

			if(GetEnableSoftwareLimit(&bEnable))
			{
				eReturn = AxlMotionEthercatSetFunction_ReadOnDatabaseError;
				break;
			}

			if(GetSoftwareStopMode(&eStopMode))
			{
				eReturn = AxlMotionEthercatSetFunction_ReadOnDatabaseError;
				break;
			}

			if(GetPositiveLimit(&dblPositive))
			{
				eReturn = AxlMotionEthercatSetFunction_ReadOnDatabaseError;
				break;
			}

			if(GetNegativeLimit(&dblNegative))
			{
				eReturn = AxlMotionEthercatSetFunction_ReadOnDatabaseError;
				break;
			}

			AxmSignalSetSoftLimit(_ttoi(GetSubUnitID()), bEnable, (DWORD)eStopMode, (DWORD)eParam, dblPositive, dblNegative);
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionEthercatSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotionEthercat[eSaveID], g_lpszAxlMotionEthercatCounterSelection[nPreValue], g_lpszAxlMotionEthercatCounterSelection[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionEthercatGetFunction CDeviceAxlMotionEthercat::GetPositiveLimit(double* pParam)
{
	AxlMotionEthercatGetFunction eReturn = AxlMotionEthercatGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = AxlMotionEthercatGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterAxlMotionEthercat_PosLimitValue));

		eReturn = AxlMotionEthercatGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionEthercatSetFunction CDeviceAxlMotionEthercat::SetPositiveLimit(double dblParam)
{
	AxlMotionEthercatSetFunction eReturn = AxlMotionEthercatSetFunction_UnknownError;

	EDeviceParameterAxlMotionEthercat eSaveID = EDeviceParameterAxlMotionEthercat_PosLimitValue;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			bool bEnable = false;

			EDeviceAxlMotionEthercatStopMode eStopMode = EDeviceAxlMotionEthercatStopMode_EmergencyStop;

			EDeviceAxlMotionEthercatCounterSelection eCount = EDeviceAxlMotionEthercatCounterSelection_Command;

			double dblNegative = 0.0;

			if(GetEnableSoftwareLimit(&bEnable))
			{
				eReturn = AxlMotionEthercatSetFunction_ReadOnDatabaseError;
				break;
			}

			if(GetSoftwareStopMode(&eStopMode))
			{
				eReturn = AxlMotionEthercatSetFunction_ReadOnDatabaseError;
				break;
			}

			if(GetSoftwareCounterSelection(&eCount))
			{
				eReturn = AxlMotionEthercatSetFunction_ReadOnDatabaseError;
				break;
			}

			if(GetNegativeLimit(&dblNegative))
			{
				eReturn = AxlMotionEthercatSetFunction_ReadOnDatabaseError;
				break;
			}

			AxmSignalSetSoftLimit(_ttoi(GetSubUnitID()), bEnable, (DWORD)eStopMode, (DWORD)eCount, dblParam, dblNegative);
		}

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionEthercatSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotionEthercat[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionEthercatGetFunction CDeviceAxlMotionEthercat::GetNegativeLimit(double* pParam)
{
	AxlMotionEthercatGetFunction eReturn = AxlMotionEthercatGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = AxlMotionEthercatGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterAxlMotionEthercat_NegLimitValue));

		eReturn = AxlMotionEthercatGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionEthercatSetFunction CDeviceAxlMotionEthercat::SetNegativeLimit(double dblParam)
{
	AxlMotionEthercatSetFunction eReturn = AxlMotionEthercatSetFunction_UnknownError;

	EDeviceParameterAxlMotionEthercat eSaveID = EDeviceParameterAxlMotionEthercat_NegLimitValue;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			bool bEnable = false;

			EDeviceAxlMotionEthercatStopMode eStopMode = EDeviceAxlMotionEthercatStopMode_EmergencyStop;

			EDeviceAxlMotionEthercatCounterSelection eCount = EDeviceAxlMotionEthercatCounterSelection_Command;

			double dblPositive = 0.0;

			if(GetEnableSoftwareLimit(&bEnable))
			{
				eReturn = AxlMotionEthercatSetFunction_ReadOnDatabaseError;
				break;
			}

			if(GetSoftwareStopMode(&eStopMode))
			{
				eReturn = AxlMotionEthercatSetFunction_ReadOnDatabaseError;
				break;
			}

			if(GetSoftwareCounterSelection(&eCount))
			{
				eReturn = AxlMotionEthercatSetFunction_ReadOnDatabaseError;
				break;
			}

			if(GetPositiveLimit(&dblPositive))
			{
				eReturn = AxlMotionEthercatSetFunction_ReadOnDatabaseError;
				break;
			}

			AxmSignalSetSoftLimit(_ttoi(GetSubUnitID()), bEnable, (DWORD)eStopMode, (DWORD)eCount, dblPositive, dblParam);
		}

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionEthercatSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotionEthercat[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionEthercatGetFunction CDeviceAxlMotionEthercat::GetHomeSearchSensor(_Out_ EDeviceAxlMotionEthercatSearchSensor* pParam)
{
	AxlMotionEthercatGetFunction eReturn = AxlMotionEthercatGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = AxlMotionEthercatGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxlMotionEthercatSearchSensor)_ttoi(GetParamValue(EDeviceParameterAxlMotionEthercat_HomeSearchSensor));

		eReturn = AxlMotionEthercatGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionEthercatSetFunction CDeviceAxlMotionEthercat::SetHomeSearchSensor(_In_ EDeviceAxlMotionEthercatSearchSensor eParam)
{
	AxlMotionEthercatSetFunction eReturn = AxlMotionEthercatSetFunction_UnknownError;

	EDeviceParameterAxlMotionEthercat eSaveID = EDeviceParameterAxlMotionEthercat_HomeSearchSensor;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxlMotionEthercatSearchSensor_Count)
		{
			eReturn = AxlMotionEthercatSetFunction_NotFoundCommandError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionEthercatSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotionEthercat[eSaveID], g_lpszAxlMotionEthercatSearchSensor[nPreValue], g_lpszAxlMotionEthercatSearchSensor[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}


AxlMotionEthercatGetFunction CDeviceAxlMotionEthercat::GetHomeDirection(_Out_ EDeviceAxlMotionEthercatDirection* pParam)
{
	AxlMotionEthercatGetFunction eReturn = AxlMotionEthercatGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = AxlMotionEthercatGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxlMotionEthercatDirection)_ttoi(GetParamValue(EDeviceParameterAxlMotionEthercat_HomeDirection));

		eReturn = AxlMotionEthercatGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionEthercatSetFunction CDeviceAxlMotionEthercat::SetHomeDirection(_In_ EDeviceAxlMotionEthercatDirection eParam)
{
	AxlMotionEthercatSetFunction eReturn = AxlMotionEthercatSetFunction_UnknownError;

	EDeviceParameterAxlMotionEthercat eSaveID = EDeviceParameterAxlMotionEthercat_HomeDirection;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxlMotionEthercatDirection_Count)
		{
			eReturn = AxlMotionEthercatSetFunction_NotFoundCommandError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionEthercatSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotionEthercat[eSaveID], g_lpszAxlMotionEthercatDirection[nPreValue], g_lpszAxlMotionEthercatDirection[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionEthercatGetFunction CDeviceAxlMotionEthercat::GetHomeZphase(_Out_ EDeviceAxlMotionEthercatZPhase* pParam)
{
	AxlMotionEthercatGetFunction eReturn = AxlMotionEthercatGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = AxlMotionEthercatGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxlMotionEthercatZPhase)_ttoi(GetParamValue(EDeviceParameterAxlMotionEthercat_HomeZphase));

		eReturn = AxlMotionEthercatGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionEthercatSetFunction CDeviceAxlMotionEthercat::SetHomeZphase(_In_ EDeviceAxlMotionEthercatZPhase eParam)
{
	AxlMotionEthercatSetFunction eReturn = AxlMotionEthercatSetFunction_UnknownError;

	EDeviceParameterAxlMotionEthercat eSaveID = EDeviceParameterAxlMotionEthercat_HomeZphase;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxlMotionEthercatZPhase_Count)
		{
			eReturn = AxlMotionEthercatSetFunction_NotFoundCommandError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionEthercatSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotionEthercat[eSaveID], g_lpszAxlMotionEthercatZPhase[nPreValue], g_lpszAxlMotionEthercatZPhase[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionEthercatGetFunction CDeviceAxlMotionEthercat::GetHomeClearTime(_Out_ double* pData)
{
	AxlMotionEthercatGetFunction eReturn = AxlMotionEthercatGetFunction_UnknownError;

	do
	{
		if(!pData)
		{
			eReturn = AxlMotionEthercatGetFunction_NullptrError;
			break;
		}

		*pData = _ttof(GetParamValue(EDeviceParameterAxlMotionEthercat_HomeHomeClearTime));

		eReturn = AxlMotionEthercatGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionEthercatSetFunction CDeviceAxlMotionEthercat::SetHomeClearTime(_In_ double dblData)
{
	AxlMotionEthercatSetFunction eReturn = AxlMotionEthercatSetFunction_UnknownError;

	EDeviceParameterAxlMotionEthercat eSaveID = EDeviceParameterAxlMotionEthercat_HomeHomeClearTime;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%f"), dblData);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionEthercatSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotionEthercat[eSaveID], dblPreValue, dblData);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionEthercatGetFunction CDeviceAxlMotionEthercat::GetHomeOffset(_Out_ double* pData)
{
	AxlMotionEthercatGetFunction eReturn = AxlMotionEthercatGetFunction_UnknownError;

	do
	{
		if(!pData)
		{
			eReturn = AxlMotionEthercatGetFunction_NullptrError;
			break;
		}

		*pData = _ttof(GetParamValue(EDeviceParameterAxlMotionEthercat_HomeOffset));

		eReturn = AxlMotionEthercatGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionEthercatSetFunction CDeviceAxlMotionEthercat::SetHomeOffset(_In_ double dblData)
{
	AxlMotionEthercatSetFunction eReturn = AxlMotionEthercatSetFunction_UnknownError;

	EDeviceParameterAxlMotionEthercat eSaveID = EDeviceParameterAxlMotionEthercat_HomeOffset;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%f"), dblData);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionEthercatSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotionEthercat[eSaveID], dblPreValue, dblData);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionEthercatGetFunction CDeviceAxlMotionEthercat::GetHomeFirstVelocity(_Out_ double* pData)
{
	AxlMotionEthercatGetFunction eReturn = AxlMotionEthercatGetFunction_UnknownError;

	do
	{
		if(!pData)
		{
			eReturn = AxlMotionEthercatGetFunction_NullptrError;
			break;
		}

		*pData = _ttof(GetParamValue(EDeviceParameterAxlMotionEthercat_HomeFirstVel));

		eReturn = AxlMotionEthercatGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionEthercatSetFunction CDeviceAxlMotionEthercat::SetHomeFirstVelocity(_In_ double dblData)
{
	AxlMotionEthercatSetFunction eReturn = AxlMotionEthercatSetFunction_UnknownError;

	EDeviceParameterAxlMotionEthercat eSaveID = EDeviceParameterAxlMotionEthercat_HomeFirstVel;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%f"), dblData);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionEthercatSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotionEthercat[eSaveID], dblPreValue, dblData);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionEthercatGetFunction CDeviceAxlMotionEthercat::GetHomeSecondVelocity(_Out_ double* pData)
{
	AxlMotionEthercatGetFunction eReturn = AxlMotionEthercatGetFunction_UnknownError;

	do
	{
		if(!pData)
		{
			eReturn = AxlMotionEthercatGetFunction_NullptrError;
			break;
		}

		*pData = _ttof(GetParamValue(EDeviceParameterAxlMotionEthercat_HomeSecondVel));

		eReturn = AxlMotionEthercatGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionEthercatSetFunction CDeviceAxlMotionEthercat::SetHomeSecondVelocity(_In_ double dblData)
{
	AxlMotionEthercatSetFunction eReturn = AxlMotionEthercatSetFunction_UnknownError;

	EDeviceParameterAxlMotionEthercat eSaveID = EDeviceParameterAxlMotionEthercat_HomeSecondVel;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%f"), dblData);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionEthercatSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotionEthercat[eSaveID], dblPreValue, dblData);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionEthercatGetFunction CDeviceAxlMotionEthercat::GetHomeThirdVelocity(_Out_ double* pData)
{
	AxlMotionEthercatGetFunction eReturn = AxlMotionEthercatGetFunction_UnknownError;

	do
	{
		if(!pData)
		{
			eReturn = AxlMotionEthercatGetFunction_NullptrError;
			break;
		}

		*pData = _ttof(GetParamValue(EDeviceParameterAxlMotionEthercat_HomeThirdVel));

		eReturn = AxlMotionEthercatGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionEthercatSetFunction CDeviceAxlMotionEthercat::SetHomeThirdVelocity(_In_ double dblData)
{
	AxlMotionEthercatSetFunction eReturn = AxlMotionEthercatSetFunction_UnknownError;

	EDeviceParameterAxlMotionEthercat eSaveID = EDeviceParameterAxlMotionEthercat_HomeThirdVel;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%f"), dblData);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionEthercatSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotionEthercat[eSaveID], dblPreValue, dblData);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionEthercatGetFunction CDeviceAxlMotionEthercat::GetHomeLastVelocity(_Out_ double* pData)
{
	AxlMotionEthercatGetFunction eReturn = AxlMotionEthercatGetFunction_UnknownError;

	do
	{
		if(!pData)
		{
			eReturn = AxlMotionEthercatGetFunction_NullptrError;
			break;
		}

		*pData = _ttof(GetParamValue(EDeviceParameterAxlMotionEthercat_HomeLastVel));

		eReturn = AxlMotionEthercatGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionEthercatSetFunction CDeviceAxlMotionEthercat::SetHomeLastVelocity(_In_ double dblData)
{
	AxlMotionEthercatSetFunction eReturn = AxlMotionEthercatSetFunction_UnknownError;

	EDeviceParameterAxlMotionEthercat eSaveID = EDeviceParameterAxlMotionEthercat_HomeLastVel;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%f"), dblData);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionEthercatSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotionEthercat[eSaveID], dblPreValue, dblData);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionEthercatGetFunction CDeviceAxlMotionEthercat::GetHomeStartAcc(_Out_ double* pData)
{
	AxlMotionEthercatGetFunction eReturn = AxlMotionEthercatGetFunction_UnknownError;

	do
	{
		if(!pData)
		{
			eReturn = AxlMotionEthercatGetFunction_NullptrError;
			break;
		}

		*pData = _ttof(GetParamValue(EDeviceParameterAxlMotionEthercat_HomeStartAcc));

		eReturn = AxlMotionEthercatGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionEthercatSetFunction CDeviceAxlMotionEthercat::SetHomeStartAcc(_In_ double dblData)
{
	AxlMotionEthercatSetFunction eReturn = AxlMotionEthercatSetFunction_UnknownError;

	EDeviceParameterAxlMotionEthercat eSaveID = EDeviceParameterAxlMotionEthercat_HomeStartAcc;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%f"), dblData);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionEthercatSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotionEthercat[eSaveID], dblPreValue, dblData);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

AxlMotionEthercatGetFunction CDeviceAxlMotionEthercat::GetHomeEndAcc(_Out_ double* pData)
{
	AxlMotionEthercatGetFunction eReturn = AxlMotionEthercatGetFunction_UnknownError;

	do
	{
		if(!pData)
		{
			eReturn = AxlMotionEthercatGetFunction_NullptrError;
			break;
		}

		*pData = _ttof(GetParamValue(EDeviceParameterAxlMotionEthercat_HomeEndAcc));

		eReturn = AxlMotionEthercatGetFunction_OK;
	}
	while(false);

	return eReturn;
}

AxlMotionEthercatSetFunction CDeviceAxlMotionEthercat::SetHomeEndAcc(_In_ double dblData)
{
	AxlMotionEthercatSetFunction eReturn = AxlMotionEthercatSetFunction_UnknownError;

	EDeviceParameterAxlMotionEthercat eSaveID = EDeviceParameterAxlMotionEthercat_HomeEndAcc;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%f"), dblData);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = AxlMotionEthercatSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = AxlMotionEthercatSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotionEthercat[eSaveID], dblPreValue, dblData);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceAxlMotionEthercat::GetServoStatus()
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

bool CDeviceAxlMotionEthercat::SetServoOn(bool bOn)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		int nModuleID = _ttoi(GetSubUnitID());

		bReturn = (AxmSignalServoOn(nModuleID, bOn) == AXT_RT_SUCCESS) ? true : false;

		if(bReturn)
			m_bServo = bOn;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxlMotionEthercat::GetCommandPosition(double* pPos)
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

bool CDeviceAxlMotionEthercat::SetCommandPositionClear()
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

bool CDeviceAxlMotionEthercat::GetActualPosition(double * pPos)
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

bool CDeviceAxlMotionEthercat::SetActualPositionClear()
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

bool CDeviceAxlMotionEthercat::GetAlarmStatus()
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

bool CDeviceAxlMotionEthercat::SetAlarmClear()
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

bool CDeviceAxlMotionEthercat::GetInposition()
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

bool CDeviceAxlMotionEthercat::GetLimitSensorN()
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

bool CDeviceAxlMotionEthercat::GetLimitSensorP()
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

bool CDeviceAxlMotionEthercat::GetHomeSensor()
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

bool CDeviceAxlMotionEthercat::MovePosition(double dPos, double dVel, double dAcc, bool bSCurve)
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

bool CDeviceAxlMotionEthercat::MoveDistance(double dDist, double dVel, double dAcc, bool bSCurve)
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

bool CDeviceAxlMotionEthercat::MoveVelocity(double dVel, double dAcc, bool bSCurve)
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

bool CDeviceAxlMotionEthercat::MoveJog(double dVel, double dAcl, BOOL bSCurve)
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

bool CDeviceAxlMotionEthercat::StopJog()
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

bool CDeviceAxlMotionEthercat::IsMotionDone()
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

bool CDeviceAxlMotionEthercat::MotorStop()
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

bool CDeviceAxlMotionEthercat::MotorEStop()
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

bool CDeviceAxlMotionEthercat::ChangeSpeed(double dSpeed)
{
	return false;
}

bool CDeviceAxlMotionEthercat::WriteGenOutBit(int nBit, bool bOn)
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

bool CDeviceAxlMotionEthercat::ReadGenOutBit(int nBit)
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

bool CDeviceAxlMotionEthercat::ReadGenInputBit(int nBit)
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

bool CDeviceAxlMotionEthercat::MoveToHome()
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

		EDeviceAxlMotionEthercatZPhase eZphase = EDeviceAxlMotionEthercatZPhase_Count;
		EDeviceAxlMotionEthercatDirection eDir = EDeviceAxlMotionEthercatDirection_Count;
		EDeviceAxlMotionEthercatSearchSensor eSensor = EDeviceAxlMotionEthercatSearchSensor_Count;

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

		DWORD uHomeSignal = eSensor == EDeviceAxlMotionEthercatSearchSensor_HomeSensor ? HomeSensor : (eSensor == EDeviceAxlMotionEthercatSearchSensor_EncoderZPhase ? EncodZPhase : eSensor);

		dwRtn += AxmHomeSetMethod(_ttoi(GetSubUnitID()), nHmDir, uHomeSignal, uZphase, dblHomeClrTime, dblHomeOffset);

		dwRtn += AxmHomeSetVel(_ttoi(GetSubUnitID()), dblFirstVel, dblSecondVel, dblThirdVel, dblLastVel, dblStartAcc, dblEndAcc);

		dwRtn += AxmHomeSetStart(_ttoi(GetSubUnitID()));

		bReturn = (dwRtn == 0) ? true : false;
	}
	while(false);

	return bReturn;
}

EDeviceMotionHommingStatus CDeviceAxlMotionEthercat::GetHommingStatus()
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

bool CDeviceAxlMotionEthercat::MoveToSignal(double dblVel, double dblAcc, ESearchSignal eDetectSignal, ESearchEdge eSignalEdge, ESearchMethod eSignalMethod)
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

bool CDeviceAxlMotionEthercat::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = true;

	bool bFoundID = true;

	do
	{
		switch(nParam)
		{
		case EDeviceParameterAxlMotionEthercat_DeviceID:
			bReturn = !SetDeviceID(strValue);
			break;
		case EDeviceParameterAxlMotionEthercat_SubUnitID:
			bReturn = !SetSubUnitID(strValue);
			break;
		case EDeviceParameterAxlMotionEthercat_UnitPulse:
			bReturn = !SetUnitPulse(_ttof(strValue));
			break;
		case EDeviceParameterAxlMotionEthercat_StartSpeed:
			bReturn = !SetStartSpeed(_ttof(strValue));
			break;
		case EDeviceParameterAxlMotionEthercat_MaxSpeed:
			bReturn = !SetMaxSpeed(_ttoi(strValue));
			break;
		case EDeviceParameterAxlMotionEthercat_PulseOutputMethod:
			bReturn = !SetPulseOutputMethod((EDeviceAxlMotionEthercatPulseOutputMethod)_ttoi(strValue));
			break;
		case EDeviceParameterAxlMotionEthercat_EncoderMethod:
			bReturn = !SetEncoderMethod((EDeviceAxlMotionEthercatEncoderMethod)_ttoi(strValue));
			break;
		case EDeviceParameterAxlMotionEthercat_ServoLevel:
			bReturn = !SetServoLevel((EDeviceAxlMotionEthercatLevelMethod)_ttoi(strValue));
			break;
		case EDeviceParameterAxlMotionEthercat_HomeLevel:
			bReturn = !SetHomeLevel((EDeviceAxlMotionEthercatLevelMethod)_ttoi(strValue));
			break;
		case EDeviceParameterAxlMotionEthercat_PLimitLevel:
			bReturn = !SetPLimitLevel((EDeviceAxlMotionEthercatLevelMethod)_ttoi(strValue));
			break;
		case EDeviceParameterAxlMotionEthercat_NLimitLevel:
			bReturn = !SetNLimitLevel((EDeviceAxlMotionEthercatLevelMethod)_ttoi(strValue));
			break;
		case EDeviceParameterAxlMotionEthercat_AlarmInputLevel:
			bReturn = !SetAlarmInputLevel((EDeviceAxlMotionEthercatLevelMethod)_ttoi(strValue));
			break;
		case EDeviceParameterAxlMotionEthercat_AlarmResetLevel:
			bReturn = !SetAlarmResetLevel((EDeviceAxlMotionEthercatLevelMethod)_ttoi(strValue));
			break;
		case EDeviceParameterAxlMotionEthercat_InpositionInputLevel:
			bReturn = !SetInpositionInputLevel((EDeviceAxlMotionEthercatLevelMethod)_ttoi(strValue));
			break;
		case EDeviceParameterAxlMotionEthercat_UseSoftwareLimit:
			bReturn = !SetEnableSoftwareLimit((bool)_ttoi(strValue));
			break;
		case EDeviceParameterAxlMotionEthercat_StopMode:
			bReturn = !SetSoftwareStopMode((EDeviceAxlMotionEthercatStopMode)_ttoi(strValue));
			break;
		case EDeviceParameterAxlMotionEthercat_CounterSelection:
			bReturn = !SetSoftwareCounterSelection((EDeviceAxlMotionEthercatCounterSelection)_ttoi(strValue));
			break;
		case EDeviceParameterAxlMotionEthercat_PosLimitValue:
			bReturn = !SetPositiveLimit(_ttof(strValue));
			break;
		case EDeviceParameterAxlMotionEthercat_NegLimitValue:
			bReturn = !SetNegativeLimit(_ttof(strValue));
			break;
		case EDeviceParameterAxlMotionEthercat_HomeSearchSensor:
			bReturn = !SetHomeSearchSensor((EDeviceAxlMotionEthercatSearchSensor)_ttoi(strValue));
			break;
		case EDeviceParameterAxlMotionEthercat_HomeDirection:
			bReturn = !SetHomeDirection((EDeviceAxlMotionEthercatDirection)_ttoi(strValue));
			break;
		case EDeviceParameterAxlMotionEthercat_HomeZphase:
			bReturn = !SetHomeZphase((EDeviceAxlMotionEthercatZPhase)_ttoi(strValue));
			break;
		case EDeviceParameterAxlMotionEthercat_HomeHomeClearTime:
			bReturn = !SetHomeClearTime(_ttof(strValue));
			break;
		case EDeviceParameterAxlMotionEthercat_HomeOffset:
			bReturn = !SetHomeOffset(_ttof(strValue));
			break;
		case EDeviceParameterAxlMotionEthercat_HomeFirstVel:
			bReturn = !SetHomeFirstVelocity(_ttof(strValue));
			break;
		case EDeviceParameterAxlMotionEthercat_HomeSecondVel:
			bReturn = !SetHomeSecondVelocity(_ttof(strValue));
			break;
		case EDeviceParameterAxlMotionEthercat_HomeThirdVel:
			bReturn = !SetHomeThirdVelocity(_ttof(strValue));
			break;
		case EDeviceParameterAxlMotionEthercat_HomeLastVel:
			bReturn = !SetHomeLastVelocity(_ttof(strValue));
			break;
		case EDeviceParameterAxlMotionEthercat_HomeStartAcc:
			bReturn = !SetHomeStartAcc(_ttof(strValue));
			break;
		case EDeviceParameterAxlMotionEthercat_HomeEndAcc:
			bReturn = !SetHomeEndAcc(_ttof(strValue));
			break;
		case EDeviceParameterAxlMotionEthercat_OpenNoReset:
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

bool CDeviceAxlMotionEthercat::DoesModuleExist()
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


DWORD CDeviceAxlMotionEthercat::__AxmInfoGetAxis(long lAxisNo, long *lpBoardNo, long *lpModulePos, DWORD *upModuleID)
{
	return AxmInfoGetAxis(lAxisNo, lpBoardNo, lpModulePos, upModuleID);
}

DWORD CDeviceAxlMotionEthercat::__AxmInfoIsMotionModule(DWORD *upStatus)
{
	return AxmInfoIsMotionModule(upStatus);
}

DWORD CDeviceAxlMotionEthercat::__AxmInfoIsInvalidAxisNo(long lAxisNo)
{
	return AxmInfoIsInvalidAxisNo(lAxisNo);
}

DWORD CDeviceAxlMotionEthercat::__AxmInfoGetAxisStatus(long lAxisNo)
{
	return AxmInfoGetAxisStatus(lAxisNo);
}

DWORD CDeviceAxlMotionEthercat::__AxmInfoGetAxisCount(long *lpAxisCount)
{
	return AxmInfoGetAxisCount(lpAxisCount);
}

DWORD CDeviceAxlMotionEthercat::__AxmInfoGetFirstAxisNo(long lBoardNo, long lModulePos, long *lpAxisNo)
{
	return AxmInfoGetFirstAxisNo(lBoardNo, lModulePos, lpAxisNo);
}

DWORD CDeviceAxlMotionEthercat::__AxmInfoGetBoardFirstAxisNo(long lBoardNo, long lModulePos, long *lpAxisNo)
{
	return AxmInfoGetBoardFirstAxisNo(lBoardNo, lModulePos, lpAxisNo);
}


DWORD CDeviceAxlMotionEthercat::__AxmVirtualSetAxisNoMap(long lRealAxisNo, long lVirtualAxisNo)
{
	return AxmVirtualSetAxisNoMap(lRealAxisNo, lVirtualAxisNo);
}

DWORD CDeviceAxlMotionEthercat::__AxmVirtualGetAxisNoMap(long lRealAxisNo, long *lpVirtualAxisNo)
{
	return AxmVirtualGetAxisNoMap(lRealAxisNo, lpVirtualAxisNo);
}

DWORD CDeviceAxlMotionEthercat::__AxmVirtualSetMultiAxisNoMap(long lSize, long *lpRealAxesNo, long *lpVirtualAxesNo)
{
	return AxmVirtualSetMultiAxisNoMap(lSize, lpRealAxesNo, lpVirtualAxesNo);
}

DWORD CDeviceAxlMotionEthercat::__AxmVirtualGetMultiAxisNoMap(long lSize, long *lpRealAxesNo, long *lpVirtualAxesNo)
{
	return AxmVirtualGetMultiAxisNoMap(lSize, lpRealAxesNo, lpVirtualAxesNo);
}

DWORD CDeviceAxlMotionEthercat::__AxmVirtualResetAxisMap()
{
	return AxmVirtualResetAxisMap();
}


DWORD CDeviceAxlMotionEthercat::__AxmInterruptSetAxisEnable(long lAxisNo, DWORD uUse)
{
	return AxmInterruptSetAxisEnable(lAxisNo, uUse);
}

DWORD CDeviceAxlMotionEthercat::__AxmInterruptGetAxisEnable(long lAxisNo, DWORD *upUse)
{
	return AxmInterruptGetAxisEnable(lAxisNo, upUse);
}

DWORD CDeviceAxlMotionEthercat::__AxmInterruptRead(long *lpAxisNo, DWORD *upFlag)
{
	return AxmInterruptRead(lpAxisNo, upFlag);
}

DWORD CDeviceAxlMotionEthercat::__AxmInterruptReadAxisFlag(long lAxisNo, long lBank, DWORD *upFlag)
{
	return AxmInterruptReadAxisFlag(lAxisNo, lBank, upFlag);
}

DWORD CDeviceAxlMotionEthercat::__AxmInterruptSetUserEnable(long lAxisNo, long lBank, DWORD uInterruptNum)
{
	return AxmInterruptSetUserEnable(lAxisNo, lBank, uInterruptNum);
}

DWORD CDeviceAxlMotionEthercat::__AxmInterruptGetUserEnable(long lAxisNo, long lBank, DWORD *upInterruptNum)
{
	return AxmInterruptGetUserEnable(lAxisNo, lBank, upInterruptNum);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotLoadParaAll(char *szFilePath)
{
	return AxmMotLoadParaAll(szFilePath);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotSaveParaAll(char *szFilePath)
{
	return AxmMotSaveParaAll(szFilePath);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotSetParaLoad(long lAxisNo, double dInitPos, double dInitVel, double dInitAccel, double dInitDecel)
{
	return AxmMotSetParaLoad(lAxisNo, dInitPos, dInitVel, dInitAccel, dInitDecel);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotGetParaLoad(long lAxisNo, double *dpInitPos, double *dpInitVel, double *dpInitAccel, double *dpInitDecel)
{
	return AxmMotGetParaLoad(lAxisNo, dpInitPos, dpInitVel, dpInitAccel, dpInitDecel);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotSetPulseOutMethod(long lAxisNo, DWORD uMethod)
{
	return AxmMotSetPulseOutMethod(lAxisNo, uMethod);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotGetPulseOutMethod(long lAxisNo, DWORD *upMethod)
{
	return AxmMotGetPulseOutMethod(lAxisNo, upMethod);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotSetEncInputMethod(long lAxisNo, DWORD uMethod)
{
	return AxmMotSetEncInputMethod(lAxisNo, uMethod);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotGetEncInputMethod(long lAxisNo, DWORD *upMethod)
{
	return AxmMotGetEncInputMethod(lAxisNo, upMethod);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotSetMoveUnitPerPulse(long lAxisNo, double dUnit, long lPulse)
{
	return AxmMotSetMoveUnitPerPulse(lAxisNo, dUnit, lPulse);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotGetMoveUnitPerPulse(long lAxisNo, double *dpUnit, long *lpPulse)
{
	return AxmMotGetMoveUnitPerPulse(lAxisNo, dpUnit, lpPulse);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotSetDecelMode(long lAxisNo, DWORD uMethod)
{
	return AxmMotSetDecelMode(lAxisNo, uMethod);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotGetDecelMode(long lAxisNo, DWORD *upMethod)
{
	return AxmMotGetDecelMode(lAxisNo, upMethod);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotSetRemainPulse(long lAxisNo, DWORD uData)
{
	return AxmMotSetRemainPulse(lAxisNo, uData);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotGetRemainPulse(long lAxisNo, DWORD *upData)
{
	return AxmMotGetRemainPulse(lAxisNo, upData);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotSetMaxVel(long lAxisNo, double dVel)
{
	return AxmMotSetMaxVel(lAxisNo, dVel);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotGetMaxVel(long lAxisNo, double *dpVel)
{
	return AxmMotGetMaxVel(lAxisNo, dpVel);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotSetAbsRelMode(long lAxisNo, DWORD uAbsRelMode)
{
	return AxmMotSetAbsRelMode(lAxisNo, uAbsRelMode);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotGetAbsRelMode(long lAxisNo, DWORD *upAbsRelMode)
{
	return AxmMotGetAbsRelMode(lAxisNo, upAbsRelMode);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotSetProfileMode(long lAxisNo, DWORD uProfileMode)
{
	return AxmMotSetProfileMode(lAxisNo, uProfileMode);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotGetProfileMode(long lAxisNo, DWORD *upProfileMode)
{
	return AxmMotGetProfileMode(lAxisNo, upProfileMode);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotSetAccelUnit(long lAxisNo, DWORD uAccelUnit)
{
	return AxmMotSetAccelUnit(lAxisNo, uAccelUnit);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotGetAccelUnit(long lAxisNo, DWORD *upAccelUnit)
{
	return AxmMotGetAccelUnit(lAxisNo, upAccelUnit);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotSetMinVel(long lAxisNo, double dMinVel)
{
	return AxmMotSetMinVel(lAxisNo, dMinVel);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotGetMinVel(long lAxisNo, double *dpMinVel)
{
	return AxmMotGetMinVel(lAxisNo, dpMinVel);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotSetAccelJerk(long lAxisNo, double dAccelJerk)
{
	return AxmMotSetAccelJerk(lAxisNo, dAccelJerk);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotGetAccelJerk(long lAxisNo, double *dpAccelJerk)
{
	return AxmMotGetAccelJerk(lAxisNo, dpAccelJerk);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotSetDecelJerk(long lAxisNo, double dDecelJerk)
{
	return AxmMotSetDecelJerk(lAxisNo, dDecelJerk);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotGetDecelJerk(long lAxisNo, double *dpDecelJerk)
{
	return AxmMotGetDecelJerk(lAxisNo, dpDecelJerk);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotSetProfilePriority(long lAxisNo, DWORD uPriority)
{
	return AxmMotSetProfilePriority(lAxisNo, uPriority);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotGetProfilePriority(long lAxisNo, DWORD *upPriority)
{
	return AxmMotGetProfilePriority(lAxisNo, upPriority);
}

DWORD CDeviceAxlMotionEthercat::__AxmSignalSetZphaseLevel(long lAxisNo, DWORD uLevel)
{
	return AxmSignalSetZphaseLevel(lAxisNo, uLevel);
}

DWORD CDeviceAxlMotionEthercat::__AxmSignalGetZphaseLevel(long lAxisNo, DWORD *upLevel)
{
	return AxmSignalGetZphaseLevel(lAxisNo, upLevel);
}

DWORD CDeviceAxlMotionEthercat::__AxmSignalSetServoOnLevel(long lAxisNo, DWORD uLevel)
{
	return AxmSignalSetServoOnLevel(lAxisNo, uLevel);
}

DWORD CDeviceAxlMotionEthercat::__AxmSignalGetServoOnLevel(long lAxisNo, DWORD *upLevel)
{
	return AxmSignalGetServoOnLevel(lAxisNo, upLevel);
}

DWORD CDeviceAxlMotionEthercat::__AxmSignalSetServoAlarmResetLevel(long lAxisNo, DWORD uLevel)
{
	return AxmSignalSetServoAlarmResetLevel(lAxisNo, uLevel);
}

DWORD CDeviceAxlMotionEthercat::__AxmSignalGetServoAlarmResetLevel(long lAxisNo, DWORD *upLevel)
{
	return AxmSignalGetServoAlarmResetLevel(lAxisNo, upLevel);
}

DWORD CDeviceAxlMotionEthercat::__AxmSignalSetInpos(long lAxisNo, DWORD uUse)
{
	return AxmSignalSetInpos(lAxisNo, uUse);
}

DWORD CDeviceAxlMotionEthercat::__AxmSignalGetInpos(long lAxisNo, DWORD *upUse)
{
	return AxmSignalGetInpos(lAxisNo, upUse);
}

DWORD CDeviceAxlMotionEthercat::__AxmSignalReadInpos(long lAxisNo, DWORD *upStatus)
{
	return AxmSignalReadInpos(lAxisNo, upStatus);
}

DWORD CDeviceAxlMotionEthercat::__AxmSignalSetServoAlarm(long lAxisNo, DWORD uUse)
{
	return AxmSignalSetServoAlarm(lAxisNo, uUse);
}

DWORD CDeviceAxlMotionEthercat::__AxmSignalGetServoAlarm(long lAxisNo, DWORD *upUse)
{
	return AxmSignalGetServoAlarm(lAxisNo, upUse);
}

DWORD CDeviceAxlMotionEthercat::__AxmSignalReadServoAlarm(long lAxisNo, DWORD *upStatus)
{
	return AxmSignalReadServoAlarm(lAxisNo, upStatus);
}

DWORD CDeviceAxlMotionEthercat::__AxmSignalSetLimit(long lAxisNo, DWORD uStopMode, DWORD uPositiveLevel, DWORD uNegativeLevel)
{
	return AxmSignalSetLimit(lAxisNo, uStopMode, uPositiveLevel, uNegativeLevel);
}

DWORD CDeviceAxlMotionEthercat::__AxmSignalGetLimit(long lAxisNo, DWORD *upStopMode, DWORD *upPositiveLevel, DWORD *upNegativeLevel)
{
	return AxmSignalGetLimit(lAxisNo, upStopMode, upPositiveLevel, upNegativeLevel);
}

DWORD CDeviceAxlMotionEthercat::__AxmSignalReadLimit(long lAxisNo, DWORD *upPositiveStatus, DWORD *upNegativeStatus)
{
	return AxmSignalReadLimit(lAxisNo, upPositiveStatus, upNegativeStatus);
}

DWORD CDeviceAxlMotionEthercat::__AxmSignalSetSoftLimit(long lAxisNo, DWORD uUse, DWORD uStopMode, DWORD uSelection, double dPositivePos, double dNegativePos)
{
	return AxmSignalSetSoftLimit(lAxisNo, uUse, uStopMode, uSelection, dPositivePos, dNegativePos);
}

DWORD CDeviceAxlMotionEthercat::__AxmSignalGetSoftLimit(long lAxisNo, DWORD *upUse, DWORD *upStopMode, DWORD *upSelection, double *dpPositivePos, double *dpNegativePos)
{
	return AxmSignalGetSoftLimit(lAxisNo, upUse, upStopMode, upSelection, dpPositivePos, dpNegativePos);
}


DWORD CDeviceAxlMotionEthercat::__AxmSignalReadSoftLimit(long lAxisNo, DWORD *upPositiveStatus, DWORD *upNegativeStatus)
{
	return AxmSignalReadSoftLimit(lAxisNo, upPositiveStatus, upNegativeStatus);
}

DWORD CDeviceAxlMotionEthercat::__AxmSignalSetStop(long lAxisNo, DWORD uStopMode, DWORD uLevel)
{
	return AxmSignalSetStop(lAxisNo, uStopMode, uLevel);
}

DWORD CDeviceAxlMotionEthercat::__AxmSignalGetStop(long lAxisNo, DWORD *upStopMode, DWORD *upLevel)
{
	return AxmSignalGetStop(lAxisNo, upStopMode, upLevel);
}

DWORD CDeviceAxlMotionEthercat::__AxmSignalReadStop(long lAxisNo, DWORD *upStatus)
{
	return AxmSignalReadStop(lAxisNo, upStatus);
}

DWORD CDeviceAxlMotionEthercat::__AxmSignalServoOn(long lAxisNo, DWORD uOnOff)
{
	return AxmSignalServoOn(lAxisNo, uOnOff);
}

DWORD CDeviceAxlMotionEthercat::__AxmSignalIsServoOn(long lAxisNo, DWORD *upOnOff)
{
	return AxmSignalIsServoOn(lAxisNo, upOnOff);
}

DWORD CDeviceAxlMotionEthercat::__AxmSignalServoAlarmReset(long lAxisNo, DWORD uOnOff)
{
	return AxmSignalServoAlarmReset(lAxisNo, uOnOff);
}

DWORD CDeviceAxlMotionEthercat::__AxmSignalWriteOutput(long lAxisNo, DWORD uValue)
{
	return AxmSignalWriteOutput(lAxisNo, uValue);
}

DWORD CDeviceAxlMotionEthercat::__AxmSignalReadOutput(long lAxisNo, DWORD *upValue)
{
	return AxmSignalReadOutput(lAxisNo, upValue);
}

DWORD CDeviceAxlMotionEthercat::__AxmSignalReadBrakeOn(long lAxisNo, DWORD *upOnOff)
{
	return AxmSignalReadBrakeOn(lAxisNo, upOnOff);
}

DWORD CDeviceAxlMotionEthercat::__AxmSignalWriteOutputBit(long lAxisNo, long lBitNo, DWORD uOnOff)
{
	return AxmSignalWriteOutputBit(lAxisNo, lBitNo, uOnOff);
}

DWORD CDeviceAxlMotionEthercat::__AxmSignalReadOutputBit(long lAxisNo, long lBitNo, DWORD *upOnOff)
{
	return AxmSignalReadOutputBit(lAxisNo, lBitNo, upOnOff);
}

DWORD CDeviceAxlMotionEthercat::__AxmSignalReadInput(long lAxisNo, DWORD *upValue)
{
	return AxmSignalReadInput(lAxisNo, upValue);
}

DWORD CDeviceAxlMotionEthercat::__AxmSignalReadInputBit(long lAxisNo, long lBitNo, DWORD *upOn)
{
	return AxmSignalReadInputBit(lAxisNo, lBitNo, upOn);
}

DWORD CDeviceAxlMotionEthercat::__AxmSignalSetFilterBandwidth(long lAxisNo, DWORD uSignal, double dBandwidthUsec)
{
	return AxmSignalSetFilterBandwidth(lAxisNo, uSignal, dBandwidthUsec);
}

DWORD CDeviceAxlMotionEthercat::__AxmStatusReadInMotion(long lAxisNo, DWORD *upStatus)
{
	return AxmStatusReadInMotion(lAxisNo, upStatus);
}

DWORD CDeviceAxlMotionEthercat::__AxmStatusReadDrivePulseCount(long lAxisNo, long *lpPulse)
{
	return AxmStatusReadDrivePulseCount(lAxisNo, lpPulse);
}

DWORD CDeviceAxlMotionEthercat::__AxmStatusReadMotion(long lAxisNo, DWORD *upStatus)
{
	return AxmStatusReadMotion(lAxisNo, upStatus);
}

DWORD CDeviceAxlMotionEthercat::__AxmStatusReadStop(long lAxisNo, DWORD *upStatus)
{
	return AxmStatusReadStop(lAxisNo, upStatus);
}

DWORD CDeviceAxlMotionEthercat::__AxmStatusReadMechanical(long lAxisNo, DWORD *upStatus)
{
	return AxmStatusReadMechanical(lAxisNo, upStatus);
}

DWORD CDeviceAxlMotionEthercat::__AxmStatusReadVel(long lAxisNo, double *dpVel)
{
	return AxmStatusReadVel(lAxisNo, dpVel);
}

DWORD CDeviceAxlMotionEthercat::__AxmStatusReadPosError(long lAxisNo, double *dpError)
{
	return AxmStatusReadPosError(lAxisNo, dpError);
}

DWORD CDeviceAxlMotionEthercat::__AxmStatusReadDriveDistance(long lAxisNo, double *dpUnit)
{
	return AxmStatusReadDriveDistance(lAxisNo, dpUnit);
}

DWORD CDeviceAxlMotionEthercat::__AxmStatusSetPosType(long lAxisNo, DWORD uPosType, double dPositivePos, double dNegativePos)
{
	return AxmStatusSetPosType(lAxisNo, uPosType, dPositivePos, dNegativePos);
}

DWORD CDeviceAxlMotionEthercat::__AxmStatusGetPosType(long lAxisNo, DWORD *upPosType, double *dpPositivePos, double *dpNegativePos)
{
	return AxmStatusGetPosType(lAxisNo, upPosType, dpPositivePos, dpNegativePos);
}

DWORD CDeviceAxlMotionEthercat::__AxmStatusSetAbsOrgOffset(long lAxisNo, double dOrgOffsetPos)
{
	return AxmStatusSetAbsOrgOffset(lAxisNo, dOrgOffsetPos);
}

DWORD CDeviceAxlMotionEthercat::__AxmStatusSetActPos(long lAxisNo, double dPos)
{
	return AxmStatusSetActPos(lAxisNo, dPos);
}

DWORD CDeviceAxlMotionEthercat::__AxmStatusGetActPos(long lAxisNo, double *dpPos)
{
	return AxmStatusGetActPos(lAxisNo, dpPos);
}

DWORD CDeviceAxlMotionEthercat::__AxmStatusSetCmdPos(long lAxisNo, double dPos)
{
	return AxmStatusSetCmdPos(lAxisNo, dPos);
}

DWORD CDeviceAxlMotionEthercat::__AxmStatusGetCmdPos(long lAxisNo, double *dpPos)
{
	return AxmStatusGetCmdPos(lAxisNo, dpPos);
}

DWORD CDeviceAxlMotionEthercat::__AxmStatusSetPosMatch(long lAxisNo, double dPos)
{
	return AxmStatusSetPosMatch(lAxisNo, dPos);
}

DWORD CDeviceAxlMotionEthercat::__AxmStatusReadMotionInfo(long lAxisNo, PMOTION_INFO pMI)
{
	return AxmStatusReadMotionInfo(lAxisNo, pMI);
}

DWORD CDeviceAxlMotionEthercat::__AxmStatusRequestServoAlarm(long lAxisNo)
{
	return AxmStatusRequestServoAlarm(lAxisNo);
}

DWORD CDeviceAxlMotionEthercat::__AxmStatusReadServoAlarm(long lAxisNo, DWORD uReturnMode, DWORD *upAlarmCode)
{
	return AxmStatusReadServoAlarm(lAxisNo, uReturnMode, upAlarmCode);
}

DWORD CDeviceAxlMotionEthercat::__AxmStatusGetServoAlarmString(long lAxisNo, DWORD uAlarmCode, long lAlarmStringSize, char *szAlarmString)
{
	return AxmStatusGetServoAlarmString(lAxisNo, uAlarmCode, lAlarmStringSize, szAlarmString);
}

DWORD CDeviceAxlMotionEthercat::__AxmStatusRequestServoAlarmHistory(long lAxisNo)
{
	return AxmStatusRequestServoAlarmHistory(lAxisNo);
}

DWORD CDeviceAxlMotionEthercat::__AxmStatusReadServoAlarmHistory(long lAxisNo, DWORD uReturnMode, long *lpCount, DWORD *upAlarmCode)
{
	return AxmStatusReadServoAlarmHistory(lAxisNo, uReturnMode, lpCount, upAlarmCode);
}

DWORD CDeviceAxlMotionEthercat::__AxmStatusClearServoAlarmHistory(long lAxisNo)
{
	return AxmStatusClearServoAlarmHistory(lAxisNo);
}

DWORD CDeviceAxlMotionEthercat::__AxmHomeSetSignalLevel(long lAxisNo, DWORD uLevel)
{
	return AxmHomeSetSignalLevel(lAxisNo, uLevel);
}

DWORD CDeviceAxlMotionEthercat::__AxmHomeGetSignalLevel(long lAxisNo, DWORD *upLevel)
{
	return AxmHomeGetSignalLevel(lAxisNo, upLevel);
}

DWORD CDeviceAxlMotionEthercat::__AxmHomeReadSignal(long lAxisNo, DWORD *upStatus)
{
	return AxmHomeReadSignal(lAxisNo, upStatus);
}


DWORD CDeviceAxlMotionEthercat::__AxmHomeSetMethod(long lAxisNo, long lHmDir, DWORD uHomeSignal, DWORD uZphas, double dHomeClrTime, double dHomeOffset)
{
	return AxmHomeSetMethod(lAxisNo, lHmDir, uHomeSignal, uZphas, dHomeClrTime, dHomeOffset);
}

DWORD CDeviceAxlMotionEthercat::__AxmHomeGetMethod(long lAxisNo, long *lpHmDir, DWORD *upHomeSignal, DWORD *upZphas, double *dpHomeClrTime, double *dpHomeOffset)
{
	return AxmHomeGetMethod(lAxisNo, lpHmDir, upHomeSignal, upZphas, dpHomeClrTime, dpHomeOffset);
}

DWORD CDeviceAxlMotionEthercat::__AxmHomeSetFineAdjust(long lAxisNo, double dHomeDogLength, long lLevelScanTime, DWORD uFineSearchUse, DWORD uHomeClrUse)
{
	return AxmHomeSetFineAdjust(lAxisNo, dHomeDogLength, lLevelScanTime, uFineSearchUse, uHomeClrUse);
}

DWORD CDeviceAxlMotionEthercat::__AxmHomeGetFineAdjust(long lAxisNo, double *dpHomeDogLength, long *lpLevelScanTime, DWORD *upFineSearchUse, DWORD *upHomeClrUse)
{
	return AxmHomeGetFineAdjust(lAxisNo, dpHomeDogLength, lpLevelScanTime, upFineSearchUse, upHomeClrUse);
}

DWORD CDeviceAxlMotionEthercat::__AxmHomeSetVel(long lAxisNo, double dVelFirst, double dVelSecond, double dVelThird, double dVelLast, double dAccFirst, double dAccSecond)
{
	return AxmHomeSetVel(lAxisNo, dVelFirst, dVelSecond, dVelThird, dVelLast, dAccFirst, dAccSecond);
}

DWORD CDeviceAxlMotionEthercat::__AxmHomeGetVel(long lAxisNo, double *dpVelFirst, double *dpVelSecond, double *dpVelThird, double *dpVelLast, double *dpAccFirst, double *dpAccSecond)
{
	return AxmHomeGetVel(lAxisNo, dpVelFirst, dpVelSecond, dpVelThird, dpVelLast, dpAccFirst, dpAccSecond);
}

DWORD CDeviceAxlMotionEthercat::__AxmHomeSetStart(long lAxisNo)
{
	return AxmHomeSetStart(lAxisNo);
}

DWORD CDeviceAxlMotionEthercat::__AxmHomeSetResult(long lAxisNo, DWORD uHomeResult)
{
	return AxmHomeSetResult(lAxisNo, uHomeResult);
}

DWORD CDeviceAxlMotionEthercat::__AxmHomeGetResult(long lAxisNo, DWORD *upHomeResult)
{
	return AxmHomeGetResult(lAxisNo, upHomeResult);
}

DWORD CDeviceAxlMotionEthercat::__AxmHomeGetRate(long lAxisNo, DWORD *upHomeMainStepNumber, DWORD *upHomeStepNumber)
{
	return AxmHomeGetRate(lAxisNo, upHomeMainStepNumber, upHomeStepNumber);
}

DWORD CDeviceAxlMotionEthercat::__AxmMoveStartPos(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel)
{
	return AxmMoveStartPos(lAxisNo, dPos, dVel, dAccel, dDecel);
}

DWORD CDeviceAxlMotionEthercat::__AxmMovePos(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel)
{
	return AxmMovePos(lAxisNo, dPos, dVel, dAccel, dDecel);
}

DWORD CDeviceAxlMotionEthercat::__AxmMoveVel(long lAxisNo, double dVel, double dAccel, double dDecel)
{
	return AxmMoveVel(lAxisNo, dVel, dAccel, dDecel);
}

DWORD CDeviceAxlMotionEthercat::__AxmMoveStartMultiVel(long lArraySize, long *lpAxesNo, double *dpVel, double *dpAccel, double *dpDecel)
{
	return AxmMoveStartMultiVel(lArraySize, lpAxesNo, dpVel, dpAccel, dpDecel);
}


DWORD CDeviceAxlMotionEthercat::__AxmMoveStartMultiVelEx(long lArraySize, long *lpAxesNo, double *dpVel, double *dpAccel, double *dpDecel, DWORD dwSyncMode)
{
	return AxmMoveStartMultiVelEx(lArraySize, lpAxesNo, dpVel, dpAccel, dpDecel, dwSyncMode);
}


DWORD CDeviceAxlMotionEthercat::__AxmMoveStartLineVel(long lArraySize, long *lpAxesNo, double *dpDis, double dVel, double dAccel, double dDecel)
{
	return AxmMoveStartLineVel(lArraySize, lpAxesNo, dpDis, dVel, dAccel, dDecel);
}


DWORD CDeviceAxlMotionEthercat::__AxmMoveSignalSearch(long lAxisNo, double dVel, double dAccel, long lDetectSignal, long lSignalEdge, long lSignalMethod)
{
	return AxmMoveSignalSearch(lAxisNo, dVel, dAccel, lDetectSignal, lSignalEdge, lSignalMethod);
}

DWORD CDeviceAxlMotionEthercat::__AxmMoveSignalSearchAtDis(long lAxisNo, double dVel, double dAccel, double dDecel, long lDetectSignal, double dDis)
{
	return AxmMoveSignalSearchAtDis(lAxisNo, dVel, dAccel, dDecel, lDetectSignal, dDis);
}

DWORD CDeviceAxlMotionEthercat::__AxmMoveSignalCapture(long lAxisNo, double dVel, double dAccel, long lDetectSignal, long lSignalEdge, long lTarget, long lSignalMethod)
{
	return AxmMoveSignalCapture(lAxisNo, dVel, dAccel, lDetectSignal, lSignalEdge, lTarget, lSignalMethod);
}


DWORD CDeviceAxlMotionEthercat::__AxmMoveGetCapturePos(long lAxisNo, double *dpCapPotition)
{
	return AxmMoveGetCapturePos(lAxisNo, dpCapPotition);
}

DWORD CDeviceAxlMotionEthercat::__AxmMoveStartMultiPos(long lArraySize, long *lpAxisNo, double *dpPos, double *dpVel, double *dpAccel, double *dpDecel)
{
	return AxmMoveStartMultiPos(lArraySize, lpAxisNo, dpPos, dpVel, dpAccel, dpDecel);
}

DWORD CDeviceAxlMotionEthercat::__AxmMoveStartTorque(long lAxisNo, double dTorque, double dVel, DWORD dwAccFilterSel, DWORD dwGainSel, DWORD dwSpdLoopSel)
{
	return AxmMoveStartTorque(lAxisNo, dTorque, dVel, dwAccFilterSel, dwGainSel, dwSpdLoopSel);
}

DWORD CDeviceAxlMotionEthercat::__AxmMoveTorqueStop(long lAxisNo, DWORD dwMethod)
{
	return AxmMoveTorqueStop(lAxisNo, dwMethod);
}


DWORD CDeviceAxlMotionEthercat::__AxmMoveStartPosWithList(long lAxisNo, double dPosition, double *dpVel, double *dpAccel, double *dpDecel, long lListNum)
{
	return AxmMoveStartPosWithList(lAxisNo, dPosition, dpVel, dpAccel, dpDecel, lListNum);
}

DWORD CDeviceAxlMotionEthercat::__AxmMoveStartPosWithPosEvent(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel, long lEventAxisNo, double dComparePosition, DWORD uPositionSource)
{
	return AxmMoveStartPosWithPosEvent(lAxisNo, dPos, dVel, dAccel, dDecel, lEventAxisNo, dComparePosition, uPositionSource);
}

DWORD CDeviceAxlMotionEthercat::__AxmMoveStop(long lAxisNo, double dDecel)
{
	return AxmMoveStop(lAxisNo, dDecel);
}

DWORD CDeviceAxlMotionEthercat::__AxmMoveStopEx(long lAxisNo, double dDecel)
{
	return AxmMoveStopEx(lAxisNo, dDecel);
}

DWORD CDeviceAxlMotionEthercat::__AxmMoveEStop(long lAxisNo)
{
	return AxmMoveEStop(lAxisNo);
}

DWORD CDeviceAxlMotionEthercat::__AxmMoveSStop(long lAxisNo)
{
	return AxmMoveSStop(lAxisNo);
}

DWORD CDeviceAxlMotionEthercat::__AxmOverridePos(long lAxisNo, double dOverridePos)
{
	return AxmOverridePos(lAxisNo, dOverridePos);
}

DWORD CDeviceAxlMotionEthercat::__AxmOverrideSetMaxVel(long lAxisNo, double dOverrideMaxVel)
{
	return AxmOverrideSetMaxVel(lAxisNo, dOverrideMaxVel);
}

DWORD CDeviceAxlMotionEthercat::__AxmOverrideVel(long lAxisNo, double dOverrideVel)
{
	return AxmOverrideVel(lAxisNo, dOverrideVel);
}

DWORD CDeviceAxlMotionEthercat::__AxmOverrideAccelVelDecel(long lAxisNo, double dOverrideVelocity, double dMaxAccel, double dMaxDecel)
{
	return AxmOverrideAccelVelDecel(lAxisNo, dOverrideVelocity, dMaxAccel, dMaxDecel);
}


DWORD CDeviceAxlMotionEthercat::__AxmOverrideVelAtPos(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel, double dOverridePos, double dOverrideVel, long lTarget)
{
	return AxmOverrideVelAtPos(lAxisNo, dPos, dVel, dAccel, dDecel, dOverridePos, dOverrideVel, lTarget);
}

DWORD CDeviceAxlMotionEthercat::__AxmOverrideVelAtMultiPos(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel, long lArraySize, double* dpOverridePos, double* dpOverrideVel, long lTarget, DWORD dwOverrideMode)
{
	return AxmOverrideVelAtMultiPos(lAxisNo, dPos, dVel, dAccel, dDecel, lArraySize, dpOverridePos, dpOverrideVel, lTarget, dwOverrideMode);
}

DWORD CDeviceAxlMotionEthercat::__AxmOverrideVelAtMultiPos2(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel, long lArraySize, double* dpOverridePos, double* dpOverrideVel, double* dpOverrideAccelDecel, long lTarget, DWORD dwOverrideMode)
{
	return AxmOverrideVelAtMultiPos2(lAxisNo, dPos, dVel, dAccel, dDecel, lArraySize, dpOverridePos, dpOverrideVel, dpOverrideAccelDecel, lTarget, dwOverrideMode);
}


DWORD CDeviceAxlMotionEthercat::__AxmLinkSetMode(long lMasterAxisNo, long lSlaveAxisNo, double dSlaveRatio)
{
	return AxmLinkSetMode(lMasterAxisNo, lSlaveAxisNo, dSlaveRatio);
}

DWORD CDeviceAxlMotionEthercat::__AxmLinkGetMode(long lMasterAxisNo, long *lpSlaveAxisNo, double *dpGearRatio)
{
	return AxmLinkGetMode(lMasterAxisNo, lpSlaveAxisNo, dpGearRatio);
}

DWORD CDeviceAxlMotionEthercat::__AxmLinkResetMode(long lMasterAxisNo)
{
	return AxmLinkResetMode(lMasterAxisNo);
}

DWORD CDeviceAxlMotionEthercat::__AxmGantrySetEnable(long lMasterAxisNo, long lSlaveAxisNo, DWORD uSlHomeUse, double dSlOffset, double dSlOffsetRange)
{
	return AxmGantrySetEnable(lMasterAxisNo, lSlaveAxisNo, uSlHomeUse, dSlOffset, dSlOffsetRange);
}

DWORD CDeviceAxlMotionEthercat::__AxmGantryGetEnable(long lMasterAxisNo, DWORD *upSlHomeUse, double *dpSlOffset, double *dpSlORange, DWORD *upGatryOn)
{
	return AxmGantryGetEnable(lMasterAxisNo, upSlHomeUse, dpSlOffset, dpSlORange, upGatryOn);
}

DWORD CDeviceAxlMotionEthercat::__AxmGantrySetDisable(long lMasterAxisNo, long lSlaveAxisNo)
{
	return AxmGantrySetDisable(lMasterAxisNo, lSlaveAxisNo);
}

DWORD CDeviceAxlMotionEthercat::__AxmGantrySetCompensationGain(long lMasterAxisNo, long lMasterGain, long lSlaveGain)
{
	return AxmGantrySetCompensationGain(lMasterAxisNo, lMasterGain, lSlaveGain);
}

DWORD CDeviceAxlMotionEthercat::__AxmGantryGetCompensationGain(long lMasterAxisNo, long *lpMasterGain, long *lpSlaveGain)
{
	return AxmGantryGetCompensationGain(lMasterAxisNo, lpMasterGain, lpSlaveGain);
}

DWORD CDeviceAxlMotionEthercat::__AxmGantrySetErrorRange(long lMasterAxisNo, double dErrorRange, DWORD uUse)
{
	return AxmGantrySetErrorRange(lMasterAxisNo, dErrorRange, uUse);
}

DWORD CDeviceAxlMotionEthercat::__AxmGantryGetErrorRange(long lMasterAxisNo, double *dpErrorRange, DWORD *upUse)
{
	return AxmGantryGetErrorRange(lMasterAxisNo, dpErrorRange, upUse);
}

DWORD CDeviceAxlMotionEthercat::__AxmGantryReadErrorRangeStatus(long lMasterAxisNo, DWORD *dwpStatus)
{
	return AxmGantryReadErrorRangeStatus(lMasterAxisNo, dwpStatus);
}

DWORD CDeviceAxlMotionEthercat::__AxmGantryReadErrorRangeComparePos(long lMasterAxisNo, double *dpComparePos)
{
	return AxmGantryReadErrorRangeComparePos(lMasterAxisNo, dpComparePos);
}

DWORD CDeviceAxlMotionEthercat::__AxmLineMove(long lCoord, double *dpEndPos, double dVel, double dAccel, double dDecel)
{
	return AxmLineMove(lCoord, dpEndPos, dVel, dAccel, dDecel);
}

DWORD CDeviceAxlMotionEthercat::__AxmLineMoveEx2(long lCoord, double *dpEndPos, double dVel, double dAccel, double dDecel)
{
	return AxmLineMoveEx2(lCoord, dpEndPos, dVel, dAccel, dDecel);
}

DWORD CDeviceAxlMotionEthercat::__AxmCircleCenterMove(long lCoord, long *lAxisNo, double *dCenterPos, double *dEndPos, double dVel, double dAccel, double dDecel, DWORD uCWDir)
{
	return AxmCircleCenterMove(lCoord, lAxisNo, dCenterPos, dEndPos, dVel, dAccel, dDecel, uCWDir);
}

DWORD CDeviceAxlMotionEthercat::__AxmCirclePointMove(long lCoord, long *lAxisNo, double *dMidPos, double *dEndPos, double dVel, double dAccel, double dDecel, long lArcCircle)
{
	return AxmCirclePointMove(lCoord, lAxisNo, dMidPos, dEndPos, dVel, dAccel, dDecel, lArcCircle);
}

DWORD CDeviceAxlMotionEthercat::__AxmCircleRadiusMove(long lCoord, long *lAxisNo, double dRadius, double *dEndPos, double dVel, double dAccel, double dDecel, DWORD uCWDir, DWORD uShortDistance)
{
	return AxmCircleRadiusMove(lCoord, lAxisNo, dRadius, dEndPos, dVel, dAccel, dDecel, uCWDir, uShortDistance);
}

DWORD CDeviceAxlMotionEthercat::__AxmCircleAngleMove(long lCoord, long *lAxisNo, double *dCenterPos, double dAngle, double dVel, double dAccel, double dDecel, DWORD uCWDir)
{
	return AxmCircleAngleMove(lCoord, lAxisNo, dCenterPos, dAngle, dVel, dAccel, dDecel, uCWDir);
}

DWORD CDeviceAxlMotionEthercat::__AxmContiSetAxisMap(long lCoord, long lSize, long *lpAxesNo)
{
	return AxmContiSetAxisMap(lCoord, lSize, lpAxesNo);
}

DWORD CDeviceAxlMotionEthercat::__AxmContiGetAxisMap(long lCoord, long *lpSize, long *lpAxesNo)
{
	return AxmContiGetAxisMap(lCoord, lpSize, lpAxesNo);
}

DWORD CDeviceAxlMotionEthercat::__AxmContiSetAbsRelMode(long lCoord, DWORD uAbsRelMode)
{
	return AxmContiSetAbsRelMode(lCoord, uAbsRelMode);
}

DWORD CDeviceAxlMotionEthercat::__AxmContiGetAbsRelMode(long lCoord, DWORD *upAbsRelMode)
{
	return AxmContiGetAbsRelMode(lCoord, upAbsRelMode);
}

DWORD CDeviceAxlMotionEthercat::__AxmContiReadFree(long lCoord, DWORD *upQueueFree)
{
	return AxmContiReadFree(lCoord, upQueueFree);
}

DWORD CDeviceAxlMotionEthercat::__AxmContiReadIndex(long lCoord, long *lpQueueIndex)
{
	return AxmContiReadIndex(lCoord, lpQueueIndex);
}

DWORD CDeviceAxlMotionEthercat::__AxmContiWriteClear(long lCoord)
{
	return AxmContiWriteClear(lCoord);
}

DWORD CDeviceAxlMotionEthercat::__AxmContiBeginNode(long lCoord)
{
	return AxmContiBeginNode(lCoord);
}

DWORD CDeviceAxlMotionEthercat::__AxmContiEndNode(long lCoord)
{
	return AxmContiEndNode(lCoord);
}

DWORD CDeviceAxlMotionEthercat::__AxmContiStart(long lCoord, DWORD dwProfileset, long lAngle)
{
	return AxmContiStart(lCoord, dwProfileset, lAngle);
}

DWORD CDeviceAxlMotionEthercat::__AxmContiIsMotion(long lCoord, DWORD *upInMotion)
{
	return AxmContiIsMotion(lCoord, upInMotion);
}

DWORD CDeviceAxlMotionEthercat::__AxmContiGetNodeNum(long lCoord, long *lpNodeNum)
{
	return AxmContiGetNodeNum(lCoord, lpNodeNum);
}

DWORD CDeviceAxlMotionEthercat::__AxmContiGetTotalNodeNum(long lCoord, long *lpNodeNum)
{
	return AxmContiGetTotalNodeNum(lCoord, lpNodeNum);
}

DWORD CDeviceAxlMotionEthercat::__AxmTriggerSetTimeLevel(long lAxisNo, double dTrigTime, DWORD uTriggerLevel, DWORD uSelect, DWORD uInterrupt)
{
	return AxmTriggerSetTimeLevel(lAxisNo, dTrigTime, uTriggerLevel, uSelect, uInterrupt);
}

DWORD CDeviceAxlMotionEthercat::__AxmTriggerGetTimeLevel(long lAxisNo, double *dpTrigTime, DWORD *upTriggerLevel, DWORD *upSelect, DWORD *upInterrupt)
{
	return AxmTriggerGetTimeLevel(lAxisNo, dpTrigTime, upTriggerLevel, upSelect, upInterrupt);
}

DWORD CDeviceAxlMotionEthercat::__AxmTriggerSetAbsPeriod(long lAxisNo, DWORD uMethod, double dPos)
{
	return AxmTriggerSetAbsPeriod(lAxisNo, uMethod, dPos);
}

DWORD CDeviceAxlMotionEthercat::__AxmTriggerGetAbsPeriod(long lAxisNo, DWORD *upMethod, double *dpPos)
{
	return AxmTriggerGetAbsPeriod(lAxisNo, upMethod, dpPos);
}

DWORD CDeviceAxlMotionEthercat::__AxmTriggerSetBlock(long lAxisNo, double dStartPos, double dEndPos, double dPeriodPos)
{
	return AxmTriggerSetBlock(lAxisNo, dStartPos, dEndPos, dPeriodPos);
}

DWORD CDeviceAxlMotionEthercat::__AxmTriggerGetBlock(long lAxisNo, double *dpStartPos, double *dpEndPos, double *dpPeriodPos)
{
	return AxmTriggerGetBlock(lAxisNo, dpStartPos, dpEndPos, dpPeriodPos);
}

DWORD CDeviceAxlMotionEthercat::__AxmTriggerOneShot(long lAxisNo)
{
	return AxmTriggerOneShot(lAxisNo);
}

DWORD CDeviceAxlMotionEthercat::__AxmTriggerSetTimerOneshot(long lAxisNo, long lmSec)
{
	return AxmTriggerSetTimerOneshot(lAxisNo, lmSec);
}

DWORD CDeviceAxlMotionEthercat::__AxmTriggerOnlyAbs(long lAxisNo, long lTrigNum, double* dpTrigPos)
{
	return AxmTriggerOnlyAbs(lAxisNo, lTrigNum, dpTrigPos);
}

DWORD CDeviceAxlMotionEthercat::__AxmTriggerSetReset(long lAxisNo)
{
	return AxmTriggerSetReset(lAxisNo);
}

DWORD CDeviceAxlMotionEthercat::__AxmCrcSetMaskLevel(long lAxisNo, DWORD uLevel, DWORD uMethod)
{
	return AxmCrcSetMaskLevel(lAxisNo, uLevel, uMethod);
}

DWORD CDeviceAxlMotionEthercat::__AxmCrcGetMaskLevel(long lAxisNo, DWORD *upLevel, DWORD *upMethod)
{
	return AxmCrcGetMaskLevel(lAxisNo, upLevel, upMethod);
}

DWORD CDeviceAxlMotionEthercat::__AxmCrcSetOutput(long lAxisNo, DWORD uOnOff)
{
	return AxmCrcSetOutput(lAxisNo, uOnOff);
}

DWORD CDeviceAxlMotionEthercat::__AxmCrcGetOutput(long lAxisNo, DWORD *upOnOff)
{
	return AxmCrcGetOutput(lAxisNo, upOnOff);
}

DWORD CDeviceAxlMotionEthercat::__AxmMPGSetEnable(long lAxisNo, long lInputMethod, long lDriveMode, double dMPGPos, double dVel, double dAccel)
{
	return AxmMPGSetEnable(lAxisNo, lInputMethod, lDriveMode, dMPGPos, dVel, dAccel);
}


DWORD CDeviceAxlMotionEthercat::__AxmMPGGetEnable(long lAxisNo, long *lpInputMethod, long *lpDriveMode, double *dpMPGPos, double *dpVel, double *dAccel)
{
	return AxmMPGGetEnable(lAxisNo, lpInputMethod, lpDriveMode, dpMPGPos, dpVel, dAccel);
}


DWORD CDeviceAxlMotionEthercat::__AxmMPGSetRatio(long lAxisNo, DWORD uMPGnumerator, DWORD uMPGdenominator)
{
	return AxmMPGSetRatio(lAxisNo, uMPGnumerator, uMPGdenominator);
}

DWORD CDeviceAxlMotionEthercat::__AxmMPGGetRatio(long lAxisNo, DWORD *upMPGnumerator, DWORD *upMPGdenominator)
{
	return AxmMPGGetRatio(lAxisNo, upMPGnumerator, upMPGdenominator);
}

DWORD CDeviceAxlMotionEthercat::__AxmMPGReset(long lAxisNo)
{
	return AxmMPGReset(lAxisNo);
}

DWORD CDeviceAxlMotionEthercat::__AxmHelixCenterMove(long lCoord, double dCenterXPos, double dCenterYPos, double dEndXPos, double dEndYPos, double dZPos, double dVel, double dAccel, double dDecel, DWORD uCWDir)
{
	return AxmHelixCenterMove(lCoord, dCenterXPos, dCenterYPos, dEndXPos, dEndYPos, dZPos, dVel, dAccel, dDecel, uCWDir);
}

DWORD CDeviceAxlMotionEthercat::__AxmHelixPointMove(long lCoord, double dMidXPos, double dMidYPos, double dEndXPos, double dEndYPos, double dZPos, double dVel, double dAccel, double dDecel)
{
	return AxmHelixPointMove(lCoord, dMidXPos, dMidYPos, dEndXPos, dEndYPos, dZPos, dVel, dAccel, dDecel);
}

DWORD CDeviceAxlMotionEthercat::__AxmHelixRadiusMove(long lCoord, double dRadius, double dEndXPos, double dEndYPos, double dZPos, double dVel, double dAccel, double dDecel, DWORD uCWDir, DWORD uShortDistance)
{
	return AxmHelixRadiusMove(lCoord, dRadius, dEndXPos, dEndYPos, dZPos, dVel, dAccel, dDecel, uCWDir, uShortDistance);
}

DWORD CDeviceAxlMotionEthercat::__AxmHelixAngleMove(long lCoord, double dCenterXPos, double dCenterYPos, double dAngle, double dZPos, double dVel, double dAccel, double dDecel, DWORD uCWDir)
{
	return AxmHelixAngleMove(lCoord, dCenterXPos, dCenterYPos, dAngle, dZPos, dVel, dAccel, dDecel, uCWDir);
}

DWORD CDeviceAxlMotionEthercat::__AxmSplineWrite(long lCoord, long lPosSize, double *dpPosX, double *dpPosY, double dVel, double dAccel, double dDecel, double dPosZ, long lPointFactor)
{
	return AxmSplineWrite(lCoord, lPosSize, dpPosX, dpPosY, dVel, dAccel, dDecel, dPosZ, lPointFactor);
}

DWORD CDeviceAxlMotionEthercat::__AxmCompensationSet(long lAxisNo, long lNumEntry, double dStartPos, double *dpPosition, double *dpCorrection, DWORD dwRollOver)
{
	return AxmCompensationSet(lAxisNo, lNumEntry, dStartPos, dpPosition, dpCorrection, dwRollOver);
}

DWORD CDeviceAxlMotionEthercat::__AxmCompensationGet(long lAxisNo, long *lpNumEntry, double *dpStartPos, double *dpPosition, double *dpCorrection, DWORD *dwpRollOver)
{
	return AxmCompensationGet(lAxisNo, lpNumEntry, dpStartPos, dpPosition, dpCorrection, dwpRollOver);
}

DWORD CDeviceAxlMotionEthercat::__AxmCompensationEnable(long lAxisNo, DWORD dwEnable)
{
	return AxmCompensationEnable(lAxisNo, dwEnable);
}

DWORD CDeviceAxlMotionEthercat::__AxmCompensationIsEnable(long lAxisNo, DWORD *dwpEnable)
{
	return AxmCompensationIsEnable(lAxisNo, dwpEnable);
}

DWORD CDeviceAxlMotionEthercat::__AxmCompensationGetCorrection(long lAxisNo, double *dpCorrection)
{
	return AxmCompensationGetCorrection(lAxisNo, dpCorrection);
}

DWORD CDeviceAxlMotionEthercat::__AxmCompensationSetBacklash(long lAxisNo, long lBacklashDir, double dBacklash)
{
	return AxmCompensationSetBacklash(lAxisNo, lBacklashDir, dBacklash);
}

DWORD CDeviceAxlMotionEthercat::__AxmCompensationGetBacklash(long lAxisNo, long *lpBacklashDir, double *dpBacklash)
{
	return AxmCompensationGetBacklash(lAxisNo, lpBacklashDir, dpBacklash);
}

DWORD CDeviceAxlMotionEthercat::__AxmCompensationEnableBacklash(long lAxisNo, DWORD dwEnable)
{
	return AxmCompensationEnableBacklash(lAxisNo, dwEnable);
}

DWORD CDeviceAxlMotionEthercat::__AxmCompensationIsEnableBacklash(long lAxisNo, DWORD *dwpEnable)
{
	return AxmCompensationIsEnableBacklash(lAxisNo, dwpEnable);
}

DWORD CDeviceAxlMotionEthercat::__AxmCompensationSetLocating(long lAxisNo, double dVel, double dAccel, double dDecel, double dWaitTime)
{
	return AxmCompensationSetLocating(lAxisNo, dVel, dAccel, dDecel, dWaitTime);
}


DWORD CDeviceAxlMotionEthercat::__AxmEcamSet(long lAxisNo, long lMasterAxis, long lNumEntry, double dMasterStartPos, double *dpMasterPos, double *dpSlavePos)
{
	return AxmEcamSet(lAxisNo, lMasterAxis, lNumEntry, dMasterStartPos, dpMasterPos, dpSlavePos);
}

DWORD CDeviceAxlMotionEthercat::__AxmEcamSetWithSource(long lAxisNo, long lMasterAxis, long lNumEntry, double dMasterStartPos, double *dpMasterPos, double *dpSlavePos, DWORD dwSource)
{
	return AxmEcamSetWithSource(lAxisNo, lMasterAxis, lNumEntry, dMasterStartPos, dpMasterPos, dpSlavePos, dwSource);
}

DWORD CDeviceAxlMotionEthercat::__AxmEcamGet(long lAxisNo, long *lpMasterAxis, long *lpNumEntry, double *dpMasterStartPos, double *dpMasterPos, double *dpSlavePos)
{
	return AxmEcamGet(lAxisNo, lpMasterAxis, lpNumEntry, dpMasterStartPos, dpMasterPos, dpSlavePos);
}

DWORD CDeviceAxlMotionEthercat::__AxmEcamGetWithSource(long lAxisNo, long *lpMasterAxis, long *lpNumEntry, double *dpMasterStartPos, double *dpMasterPos, double *dpSlavePos, DWORD *dwpSource)
{
	return AxmEcamGetWithSource(lAxisNo, lpMasterAxis, lpNumEntry, dpMasterStartPos, dpMasterPos, dpSlavePos, dwpSource);
}

DWORD CDeviceAxlMotionEthercat::__AxmEcamEnableBySlave(long lAxisNo, DWORD dwEnable)
{
	return AxmEcamEnableBySlave(lAxisNo, dwEnable);
}

DWORD CDeviceAxlMotionEthercat::__AxmEcamEnableByMaster(long lAxisNo, DWORD dwEnable)
{
	return AxmEcamEnableByMaster(lAxisNo, dwEnable);
}

DWORD CDeviceAxlMotionEthercat::__AxmEcamIsSlaveEnable(long lAxisNo, DWORD *dwpEnable)
{
	return AxmEcamIsSlaveEnable(lAxisNo, dwpEnable);
}

DWORD CDeviceAxlMotionEthercat::__AxmStatusSetServoMonitor(long lAxisNo, DWORD dwSelMon, double dActionValue, DWORD dwAction)
{
	return AxmStatusSetServoMonitor(lAxisNo, dwSelMon, dActionValue, dwAction);
}

DWORD CDeviceAxlMotionEthercat::__AxmStatusGetServoMonitor(long lAxisNo, DWORD dwSelMon, double *dpActionValue, DWORD *dwpAction)
{
	return AxmStatusGetServoMonitor(lAxisNo, dwSelMon, dpActionValue, dwpAction);
}

DWORD CDeviceAxlMotionEthercat::__AxmStatusSetServoMonitorEnable(long lAxisNo, DWORD dwEnable)
{
	return AxmStatusSetServoMonitorEnable(lAxisNo, dwEnable);
}

DWORD CDeviceAxlMotionEthercat::__AxmStatusGetServoMonitorEnable(long lAxisNo, DWORD *dwpEnable)
{
	return AxmStatusGetServoMonitorEnable(lAxisNo, dwpEnable);
}

DWORD CDeviceAxlMotionEthercat::__AxmStatusReadServoMonitorFlag(long lAxisNo, DWORD dwSelMon, DWORD *dwpMonitorFlag, double *dpMonitorValue)
{
	return AxmStatusReadServoMonitorFlag(lAxisNo, dwSelMon, dwpMonitorFlag, dpMonitorValue);
}

DWORD CDeviceAxlMotionEthercat::__AxmStatusReadServoMonitorValue(long lAxisNo, DWORD dwSelMon, double *dpMonitorValue)
{
	return AxmStatusReadServoMonitorValue(lAxisNo, dwSelMon, dpMonitorValue);
}

DWORD CDeviceAxlMotionEthercat::__AxmStatusSetReadServoLoadRatio(long lAxisNo, DWORD dwSelMon)
{
	return AxmStatusSetReadServoLoadRatio(lAxisNo, dwSelMon);
}

DWORD CDeviceAxlMotionEthercat::__AxmStatusReadServoLoadRatio(long lAxisNo, double *dpMonitorValue)
{
	return AxmStatusReadServoLoadRatio(lAxisNo, dpMonitorValue);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotSetScaleCoeff(long lAxisNo, long lScaleCoeff)
{
	return AxmMotSetScaleCoeff(lAxisNo, lScaleCoeff);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotGetScaleCoeff(long lAxisNo, long *lpScaleCoeff)
{
	return AxmMotGetScaleCoeff(lAxisNo, lpScaleCoeff);
}


DWORD CDeviceAxlMotionEthercat::__AxmMoveSignalSearchEx(long lAxisNo, double dVel, double dAccel, long lDetectSignal, long lSignalEdge, long lSignalMethod)
{
	return AxmMoveSignalSearchEx(lAxisNo, dVel, dAccel, lDetectSignal, lSignalEdge, lSignalMethod);
}


DWORD CDeviceAxlMotionEthercat::__AxmMoveToAbsPos(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel)
{
	return AxmMoveToAbsPos(lAxisNo, dPos, dVel, dAccel, dDecel);
}

DWORD CDeviceAxlMotionEthercat::__AxmStatusReadVelEx(long lAxisNo, double *dpVel)
{
	return AxmStatusReadVelEx(lAxisNo, dpVel);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotSetElectricGearRatio(long lAxisNo, long lNumerator, long lDenominator)
{
	return AxmMotSetElectricGearRatio(lAxisNo, lNumerator, lDenominator);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotGetElectricGearRatio(long lAxisNo, long *lpNumerator, long *lpDenominator)
{
	return AxmMotGetElectricGearRatio(lAxisNo, lpNumerator, lpDenominator);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotSetTorqueLimit(long lAxisNo, double dbPlusDirTorqueLimit, double dbMinusDirTorqueLimit)
{
	return AxmMotSetTorqueLimit(lAxisNo, dbPlusDirTorqueLimit, dbMinusDirTorqueLimit);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotGetTorqueLimit(long lAxisNo, double* dbpPlusDirTorqueLimit, double* dbpMinusDirTorqueLimit)
{
	return AxmMotGetTorqueLimit(lAxisNo, dbpPlusDirTorqueLimit, dbpMinusDirTorqueLimit);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotSetTorqueLimitEx(long lAxisNo, double dbPlusDirTorqueLimit, double dbMinusDirTorqueLimit)
{
	return AxmMotSetTorqueLimitEx(lAxisNo, dbPlusDirTorqueLimit, dbMinusDirTorqueLimit);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotGetTorqueLimitEx(long lAxisNo, double* dbpPlusDirTorqueLimit, double* dbpMinusDirTorqueLimit)
{
	return AxmMotGetTorqueLimitEx(lAxisNo, dbpPlusDirTorqueLimit, dbpMinusDirTorqueLimit);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotSetTorqueLimitAtPos(long lAxisNo, double dbPlusDirTorqueLimit, double dbMinusDirTorqueLimit, double dPosition, long lTarget)
{
	return AxmMotSetTorqueLimitAtPos(lAxisNo, dbPlusDirTorqueLimit, dbMinusDirTorqueLimit, dPosition, lTarget);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotGetTorqueLimitAtPos(long lAxisNo, double* dbpPlusDirTorqueLimit, double* dbpMinusDirTorqueLimit, double* dpPosition, long* lpTarget)
{
	return AxmMotGetTorqueLimitAtPos(lAxisNo, dbpPlusDirTorqueLimit, dbpMinusDirTorqueLimit, dpPosition, lpTarget);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotSetTorqueLimitEnable(long lAxisNo, DWORD uUse)
{
	return AxmMotSetTorqueLimitEnable(lAxisNo, uUse);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotGetTorqueLimitEnable(long lAxisNo, DWORD* upUse)
{
	return AxmMotGetTorqueLimitEnable(lAxisNo, upUse);
}

DWORD CDeviceAxlMotionEthercat::__AxmOverridePosSetFunction(long lAxisNo, DWORD dwUsage, long lDecelPosRatio, double dReserved)
{
	return AxmOverridePosSetFunction(lAxisNo, dwUsage, lDecelPosRatio, dReserved);
}

DWORD CDeviceAxlMotionEthercat::__AxmOverridePosGetFunction(long lAxisNo, DWORD *dwpUsage, long *lpDecelPosRatio, double *dpReserved)
{
	return AxmOverridePosGetFunction(lAxisNo, dwpUsage, lpDecelPosRatio, dpReserved);
}

DWORD CDeviceAxlMotionEthercat::__AxmSignalSetWriteOutputBitAtPos(long lAxisNo, long lModuleNo, long lOffset, DWORD uValue, double dPosition, long lTarget)
{
	return AxmSignalSetWriteOutputBitAtPos(lAxisNo, lModuleNo, lOffset, uValue, dPosition, lTarget);
}

DWORD CDeviceAxlMotionEthercat::__AxmSignalGetWriteOutputBitAtPos(long lAxisNo, long* lpModuleNo, long* lOffset, DWORD* upValue, double* dpPosition, long* lpTarget)
{
	return AxmSignalGetWriteOutputBitAtPos(lAxisNo, lpModuleNo, lOffset, upValue, dpPosition, lpTarget);
}

DWORD CDeviceAxlMotionEthercat::__AxmAdvVSTSetParameter(long lCoord, DWORD dwISTSize, double* dbpFrequency, double* dbpDampingRatio, DWORD* dwpImpulseCount)
{
	return AxmAdvVSTSetParameter(lCoord, dwISTSize, dbpFrequency, dbpDampingRatio, dwpImpulseCount);
}

DWORD CDeviceAxlMotionEthercat::__AxmAdvVSTGetParameter(long lCoord, DWORD* dwpISTSize, double* dbpFrequency, double* dbpDampingRatio, DWORD* dwpImpulseCount)
{
	return AxmAdvVSTGetParameter(lCoord, dwpISTSize, dbpFrequency, dbpDampingRatio, dwpImpulseCount);
}

DWORD CDeviceAxlMotionEthercat::__AxmAdvVSTSetEnabele(long lCoord, DWORD dwISTEnable)
{
	return AxmAdvVSTSetEnabele(lCoord, dwISTEnable);
}

DWORD CDeviceAxlMotionEthercat::__AxmAdvVSTGetEnabele(long lCoord, DWORD* dwpISTEnable)
{
	return AxmAdvVSTGetEnabele(lCoord, dwpISTEnable);
}

DWORD CDeviceAxlMotionEthercat::__AxmAdvLineMove(long lCoordinate, double *dPosition, double dMaxVelocity, double dStartVel, double dStopVel, double dMaxAccel, double dMaxDecel)
{
	return AxmAdvLineMove(lCoordinate, dPosition, dMaxVelocity, dStartVel, dStopVel, dMaxAccel, dMaxDecel);
}

DWORD CDeviceAxlMotionEthercat::__AxmAdvOvrLineMove(long lCoordinate, double *dPosition, double dMaxVelocity, double dStartVel, double dStopVel, double dMaxAccel, double dMaxDecel, long lOverrideMode)
{
	return AxmAdvOvrLineMove(lCoordinate, dPosition, dMaxVelocity, dStartVel, dStopVel, dMaxAccel, dMaxDecel, lOverrideMode);
}

DWORD CDeviceAxlMotionEthercat::__AxmAdvCircleCenterMove(long lCoord, long *lAxisNo, double *dCenterPos, double *dEndPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir)
{
	return AxmAdvCircleCenterMove(lCoord, lAxisNo, dCenterPos, dEndPos, dVel, dStartVel, dStopVel, dAccel, dDecel, uCWDir);
}

DWORD CDeviceAxlMotionEthercat::__AxmAdvCirclePointMove(long lCoord, long *lAxisNo, double *dMidPos, double *dEndPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, long lArcCircle)
{
	return AxmAdvCirclePointMove(lCoord, lAxisNo, dMidPos, dEndPos, dVel, dStartVel, dStopVel, dAccel, dDecel, lArcCircle);
}

DWORD CDeviceAxlMotionEthercat::__AxmAdvCircleAngleMove(long lCoord, long *lAxisNo, double *dCenterPos, double dAngle, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir)
{
	return AxmAdvCircleAngleMove(lCoord, lAxisNo, dCenterPos, dAngle, dVel, dStartVel, dStopVel, dAccel, dDecel, uCWDir);
}

DWORD CDeviceAxlMotionEthercat::__AxmAdvCircleRadiusMove(long lCoord, long *lAxisNo, double dRadius, double *dEndPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir, DWORD uShortDistance)
{
	return AxmAdvCircleRadiusMove(lCoord, lAxisNo, dRadius, dEndPos, dVel, dStartVel, dStopVel, dAccel, dDecel, uCWDir, uShortDistance);
}

DWORD CDeviceAxlMotionEthercat::__AxmAdvOvrCircleRadiusMove(long lCoord, long *lAxisNo, double dRadius, double *dEndPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir, DWORD uShortDistance, long lOverrideMode)
{
	return AxmAdvOvrCircleRadiusMove(lCoord, lAxisNo, dRadius, dEndPos, dVel, dStartVel, dStopVel, dAccel, dDecel, uCWDir, uShortDistance, lOverrideMode);
}

DWORD CDeviceAxlMotionEthercat::__AxmAdvHelixCenterMove(long lCoord, double dCenterXPos, double dCenterYPos, double dEndXPos, double dEndYPos, double dZPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir)
{
	return AxmAdvHelixCenterMove(lCoord, dCenterXPos, dCenterYPos, dEndXPos, dEndYPos, dZPos, dVel, dStartVel, dStopVel, dAccel, dDecel, uCWDir);
}

DWORD CDeviceAxlMotionEthercat::__AxmAdvHelixPointMove(long lCoord, double dMidXPos, double dMidYPos, double dEndXPos, double dEndYPos, double dZPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel)
{
	return AxmAdvHelixPointMove(lCoord, dMidXPos, dMidYPos, dEndXPos, dEndYPos, dZPos, dVel, dStartVel, dStopVel, dAccel, dDecel);
}

DWORD CDeviceAxlMotionEthercat::__AxmAdvHelixAngleMove(long lCoord, double dCenterXPos, double dCenterYPos, double dAngle, double dZPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir)
{
	return AxmAdvHelixAngleMove(lCoord, dCenterXPos, dCenterYPos, dAngle, dZPos, dVel, dStartVel, dStopVel, dAccel, dDecel, uCWDir);
}

DWORD CDeviceAxlMotionEthercat::__AxmAdvHelixRadiusMove(long lCoord, double dRadius, double dEndXPos, double dEndYPos, double dZPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir, DWORD uShortDistance)
{
	return AxmAdvHelixRadiusMove(lCoord, dRadius, dEndXPos, dEndYPos, dZPos, dVel, dStartVel, dStopVel, dAccel, dDecel, uCWDir, uShortDistance);
}

DWORD CDeviceAxlMotionEthercat::__AxmAdvOvrHelixRadiusMove(long lCoord, double dRadius, double dEndXPos, double dEndYPos, double dZPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir, DWORD uShortDistance, long lOverrideMode)
{
	return AxmAdvOvrHelixRadiusMove(lCoord, dRadius, dEndXPos, dEndYPos, dZPos, dVel, dStartVel, dStopVel, dAccel, dDecel, uCWDir, uShortDistance, lOverrideMode);
}

DWORD CDeviceAxlMotionEthercat::__AxmAdvScriptLineMove(long lCoordinate, double *dPosition, double dMaxVelocity, double dStartVel, double dStopVel, double dMaxAccel, double dMaxDecel, DWORD dwScript, long lScirptAxisNo, double dScriptPos)
{
	return AxmAdvScriptLineMove(lCoordinate, dPosition, dMaxVelocity, dStartVel, dStopVel, dMaxAccel, dMaxDecel, dwScript, lScirptAxisNo, dScriptPos);
}

DWORD CDeviceAxlMotionEthercat::__AxmAdvScriptOvrLineMove(long lCoordinate, double *dPosition, double dMaxVelocity, double dStartVel, double dStopVel, double dMaxAccel, double dMaxDecel, long lOverrideMode, DWORD dwScript, long lScirptAxisNo, double dScriptPos)
{
	return AxmAdvScriptOvrLineMove(lCoordinate, dPosition, dMaxVelocity, dStartVel, dStopVel, dMaxAccel, dMaxDecel, lOverrideMode, dwScript, lScirptAxisNo, dScriptPos);
}

DWORD CDeviceAxlMotionEthercat::__AxmAdvScriptCircleCenterMove(long lCoord, long *lAxisNo, double *dCenterPos, double *dEndPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir, DWORD dwScript, long lScirptAxisNo, double dScriptPos)
{
	return AxmAdvScriptCircleCenterMove(lCoord, lAxisNo, dCenterPos, dEndPos, dVel, dStartVel, dStopVel, dAccel, dDecel, uCWDir, dwScript, lScirptAxisNo, dScriptPos);
}

DWORD CDeviceAxlMotionEthercat::__AxmAdvScriptCirclePointMove(long lCoord, long *lAxisNo, double *dMidPos, double *dEndPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, long lArcCircle, DWORD dwScript, long lScirptAxisNo, double dScriptPos)
{
	return AxmAdvScriptCirclePointMove(lCoord, lAxisNo, dMidPos, dEndPos, dVel, dStartVel, dStopVel, dAccel, dDecel, lArcCircle, dwScript, lScirptAxisNo, dScriptPos);
}

DWORD CDeviceAxlMotionEthercat::__AxmAdvScriptCircleAngleMove(long lCoord, long *lAxisNo, double *dCenterPos, double dAngle, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir, DWORD dwScript, long lScirptAxisNo, double dScriptPos)
{
	return AxmAdvScriptCircleAngleMove(lCoord, lAxisNo, dCenterPos, dAngle, dVel, dStartVel, dStopVel, dAccel, dDecel, uCWDir, dwScript, lScirptAxisNo, dScriptPos);
}

DWORD CDeviceAxlMotionEthercat::__AxmAdvScriptCircleRadiusMove(long lCoord, long *lAxisNo, double dRadius, double *dEndPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir, DWORD uShortDistance, DWORD dwScript, long lScirptAxisNo, double dScriptPos)
{
	return AxmAdvScriptCircleRadiusMove(lCoord, lAxisNo, dRadius, dEndPos, dVel, dStartVel, dStopVel, dAccel, dDecel, uCWDir, uShortDistance, dwScript, lScirptAxisNo, dScriptPos);
}

DWORD CDeviceAxlMotionEthercat::__AxmAdvScriptOvrCircleRadiusMove(long lCoord, long *lAxisNo, double dRadius, double *dEndPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir, DWORD uShortDistance, long lOverrideMode, DWORD dwScript, long lScirptAxisNo, double dScriptPos)
{
	return AxmAdvScriptOvrCircleRadiusMove(lCoord, lAxisNo, dRadius, dEndPos, dVel, dStartVel, dStopVel, dAccel, dDecel, uCWDir, uShortDistance, lOverrideMode, dwScript, lScirptAxisNo, dScriptPos);
}

DWORD CDeviceAxlMotionEthercat::__AxmAdvScriptHelixCenterMove(long lCoord, double dCenterXPos, double dCenterYPos, double dEndXPos, double dEndYPos, double dZPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir, DWORD dwScript, long lScirptAxisNo, double dScriptPos)
{
	return AxmAdvScriptHelixCenterMove(lCoord, dCenterXPos, dCenterYPos, dEndXPos, dEndYPos, dZPos, dVel, dStartVel, dStopVel, dAccel, dDecel, uCWDir, dwScript, lScirptAxisNo, dScriptPos);
}

DWORD CDeviceAxlMotionEthercat::__AxmAdvScriptHelixPointMove(long lCoord, double dMidXPos, double dMidYPos, double dEndXPos, double dEndYPos, double dZPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD dwScript, long lScirptAxisNo, double dScriptPos)
{
	return AxmAdvScriptHelixPointMove(lCoord, dMidXPos, dMidYPos, dEndXPos, dEndYPos, dZPos, dVel, dStartVel, dStopVel, dAccel, dDecel, dwScript, lScirptAxisNo, dScriptPos);
}

DWORD CDeviceAxlMotionEthercat::__AxmAdvScriptHelixAngleMove(long lCoord, double dCenterXPos, double dCenterYPos, double dAngle, double dZPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir, DWORD dwScript, long lScirptAxisNo, double dScriptPos)
{
	return AxmAdvScriptHelixAngleMove(lCoord, dCenterXPos, dCenterYPos, dAngle, dZPos, dVel, dStartVel, dStopVel, dAccel, dDecel, uCWDir, dwScript, lScirptAxisNo, dScriptPos);
}

DWORD CDeviceAxlMotionEthercat::__AxmAdvScriptHelixRadiusMove(long lCoord, double dRadius, double dEndXPos, double dEndYPos, double dZPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir, DWORD uShortDistance, DWORD dwScript, long lScirptAxisNo, double dScriptPos)
{
	return AxmAdvScriptHelixRadiusMove(lCoord, dRadius, dEndXPos, dEndYPos, dZPos, dVel, dStartVel, dStopVel, dAccel, dDecel, uCWDir, uShortDistance, dwScript, lScirptAxisNo, dScriptPos);
}

DWORD CDeviceAxlMotionEthercat::__AxmAdvScriptOvrHelixRadiusMove(long lCoord, double dRadius, double dEndXPos, double dEndYPos, double dZPos, double dVel, double dStartVel, double dStopVel, double dAccel, double dDecel, DWORD uCWDir, DWORD uShortDistance, long lOverrideMode, DWORD dwScript, long lScirptAxisNo, double dScriptPos)
{
	return AxmAdvScriptOvrHelixRadiusMove(lCoord, dRadius, dEndXPos, dEndYPos, dZPos, dVel, dStartVel, dStopVel, dAccel, dDecel, uCWDir, uShortDistance, lOverrideMode, dwScript, lScirptAxisNo, dScriptPos);
}


DWORD CDeviceAxlMotionEthercat::__AxmAdvContiGetNodeNum(long lCoordinate, long *lpNodeNum)
{
	return AxmAdvContiGetNodeNum(lCoordinate, lpNodeNum);
}

DWORD CDeviceAxlMotionEthercat::__AxmAdvContiGetTotalNodeNum(long lCoordinate, long *lpNodeNum)
{
	return AxmAdvContiGetTotalNodeNum(lCoordinate, lpNodeNum);
}

DWORD CDeviceAxlMotionEthercat::__AxmAdvContiReadIndex(long lCoordinate, long *lpQueueIndex)
{
	return AxmAdvContiReadIndex(lCoordinate, lpQueueIndex);
}

DWORD CDeviceAxlMotionEthercat::__AxmAdvContiReadFree(long lCoordinate, DWORD *upQueueFree)
{
	return AxmAdvContiReadFree(lCoordinate, upQueueFree);
}

DWORD CDeviceAxlMotionEthercat::__AxmAdvContiWriteClear(long lCoordinate)
{
	return AxmAdvContiWriteClear(lCoordinate);
}

DWORD CDeviceAxlMotionEthercat::__AxmAdvOvrContiWriteClear(long lCoordinate)
{
	return AxmAdvOvrContiWriteClear(lCoordinate);
}

DWORD CDeviceAxlMotionEthercat::__AxmAdvContiStart(long lCoord, DWORD dwProfileset, long lAngle)
{
	return AxmAdvContiStart(lCoord, dwProfileset, lAngle);
}

DWORD CDeviceAxlMotionEthercat::__AxmAdvContiStop(long lCoordinate, double dDecel)
{
	return AxmAdvContiStop(lCoordinate, dDecel);
}

DWORD CDeviceAxlMotionEthercat::__AxmAdvContiSetAxisMap(long lCoord, long lSize, long *lpAxesNo)
{
	return AxmAdvContiSetAxisMap(lCoord, lSize, lpAxesNo);
}

DWORD CDeviceAxlMotionEthercat::__AxmAdvContiGetAxisMap(long lCoord, long *lpSize, long *lpAxesNo)
{
	return AxmAdvContiGetAxisMap(lCoord, lpSize, lpAxesNo);
}

DWORD CDeviceAxlMotionEthercat::__AxmAdvContiSetAbsRelMode(long lCoord, DWORD uAbsRelMode)
{
	return AxmAdvContiSetAbsRelMode(lCoord, uAbsRelMode);
}

DWORD CDeviceAxlMotionEthercat::__AxmAdvContiGetAbsRelMode(long lCoord, DWORD *uAbsRelMode)
{
	return AxmAdvContiGetAbsRelMode(lCoord, uAbsRelMode);
}

DWORD CDeviceAxlMotionEthercat::__AxmAdvContiIsMotion(long lCoordinate, DWORD *upInMotion)
{
	return AxmAdvContiIsMotion(lCoordinate, upInMotion);
}

DWORD CDeviceAxlMotionEthercat::__AxmAdvContiBeginNode(long lCoord)
{
	return AxmAdvContiBeginNode(lCoord);
}

DWORD CDeviceAxlMotionEthercat::__AxmAdvContiEndNode(long lCoord)
{
	return AxmAdvContiEndNode(lCoord);
}

DWORD CDeviceAxlMotionEthercat::__AxmMoveMultiStop(long lArraySize, long *lpAxesNo, double *dMaxDecel)
{
	return AxmMoveMultiStop(lArraySize, lpAxesNo, dMaxDecel);
}

DWORD CDeviceAxlMotionEthercat::__AxmMoveMultiEStop(long lArraySize, long *lpAxesNo)
{
	return AxmMoveMultiEStop(lArraySize, lpAxesNo);
}

DWORD CDeviceAxlMotionEthercat::__AxmMoveMultiSStop(long lArraySize, long *lpAxesNo)
{
	return AxmMoveMultiSStop(lArraySize, lpAxesNo);
}

DWORD CDeviceAxlMotionEthercat::__AxmStatusReadActVel(long lAxisNo, double *dpVel)
{
	return AxmStatusReadActVel(lAxisNo, dpVel);
}

DWORD CDeviceAxlMotionEthercat::__AxmStatusReadServoCmdStat(long lAxisNo, DWORD *upStatus)
{
	return AxmStatusReadServoCmdStat(lAxisNo, upStatus);
}

DWORD CDeviceAxlMotionEthercat::__AxmStatusReadServoCmdCtrl(long lAxisNo, DWORD *upStatus)
{
	return AxmStatusReadServoCmdCtrl(lAxisNo, upStatus);
}

DWORD CDeviceAxlMotionEthercat::__AxmGantryGetMstToSlvOverDist(long lAxisNo, double *dpPosition)
{
	return AxmGantryGetMstToSlvOverDist(lAxisNo, dpPosition);
}

DWORD CDeviceAxlMotionEthercat::__AxmGantrySetMstToSlvOverDist(long lAxisNo, double dPosition)
{
	return AxmGantrySetMstToSlvOverDist(lAxisNo, dPosition);
}

DWORD CDeviceAxlMotionEthercat::__AxmSignalReadServoAlarmCode(long lAxisNo, WORD *upCodeStatus)
{
	return AxmSignalReadServoAlarmCode(lAxisNo, upCodeStatus);
}

DWORD CDeviceAxlMotionEthercat::__AxmM3ServoCoordinatesSet(long lAxisNo, DWORD dwPosData, DWORD dwPos_sel, DWORD dwRefe)
{
	return AxmM3ServoCoordinatesSet(lAxisNo, dwPosData, dwPos_sel, dwRefe);
}

DWORD CDeviceAxlMotionEthercat::__AxmM3ServoBreakOn(long lAxisNo)
{
	return AxmM3ServoBreakOn(lAxisNo);
}

DWORD CDeviceAxlMotionEthercat::__AxmM3ServoBreakOff(long lAxisNo)
{
	return AxmM3ServoBreakOff(lAxisNo);
}

DWORD CDeviceAxlMotionEthercat::__AxmM3ServoConfig(long lAxisNo, DWORD dwCfMode)
{
	return AxmM3ServoConfig(lAxisNo, dwCfMode);
}

DWORD CDeviceAxlMotionEthercat::__AxmM3ServoSensOn(long lAxisNo)
{
	return AxmM3ServoSensOn(lAxisNo);
}

DWORD CDeviceAxlMotionEthercat::__AxmM3ServoSensOff(long lAxisNo)
{
	return AxmM3ServoSensOff(lAxisNo);
}

DWORD CDeviceAxlMotionEthercat::__AxmM3ServoSmon(long lAxisNo)
{
	return AxmM3ServoSmon(lAxisNo);
}

DWORD CDeviceAxlMotionEthercat::__AxmM3ServoGetSmon(long lAxisNo, BYTE *pbParam)
{
	return AxmM3ServoGetSmon(lAxisNo, pbParam);
}

DWORD CDeviceAxlMotionEthercat::__AxmM3ServoSvOn(long lAxisNo)
{
	return AxmM3ServoSvOn(lAxisNo);
}

DWORD CDeviceAxlMotionEthercat::__AxmM3ServoSvOff(long lAxisNo)
{
	return AxmM3ServoSvOff(lAxisNo);
}

DWORD CDeviceAxlMotionEthercat::__AxmM3ServoInterpolate(long lAxisNo, DWORD dwTPOS, DWORD dwVFF, DWORD dwTFF, DWORD dwTLIM)
{
	return AxmM3ServoInterpolate(lAxisNo, dwTPOS, dwVFF, dwTFF, dwTLIM);
}

DWORD CDeviceAxlMotionEthercat::__AxmM3ServoPosing(long lAxisNo, DWORD dwTPOS, DWORD dwSPD, DWORD dwACCR, DWORD dwDECR, DWORD dwTLIM)
{
	return AxmM3ServoPosing(lAxisNo, dwTPOS, dwSPD, dwACCR, dwDECR, dwTLIM);
}

DWORD CDeviceAxlMotionEthercat::__AxmM3ServoFeed(long lAxisNo, long lSPD, DWORD dwACCR, DWORD dwDECR, DWORD dwTLIM)
{
	return AxmM3ServoFeed(lAxisNo, lSPD, dwACCR, dwDECR, dwTLIM);
}


DWORD CDeviceAxlMotionEthercat::__AxmM3ServoExFeed(long lAxisNo, long lSPD, DWORD dwACCR, DWORD dwDECR, DWORD dwTLIM, DWORD dwExSig1, DWORD dwExSig2)
{
	return AxmM3ServoExFeed(lAxisNo, lSPD, dwACCR, dwDECR, dwTLIM, dwExSig1, dwExSig2);
}

DWORD CDeviceAxlMotionEthercat::__AxmM3ServoExPosing(long lAxisNo, DWORD dwTPOS, DWORD dwSPD, DWORD dwACCR, DWORD dwDECR, DWORD dwTLIM, DWORD dwExSig1, DWORD dwExSig2)
{
	return AxmM3ServoExPosing(lAxisNo, dwTPOS, dwSPD, dwACCR, dwDECR, dwTLIM, dwExSig1, dwExSig2);
}

DWORD CDeviceAxlMotionEthercat::__AxmM3ServoZret(long lAxisNo, DWORD dwSPD, DWORD dwACCR, DWORD dwDECR, DWORD dwTLIM, DWORD dwExSig1, DWORD dwExSig2, BYTE bHomeDir, BYTE bHomeType)
{
	return AxmM3ServoZret(lAxisNo, dwSPD, dwACCR, dwDECR, dwTLIM, dwExSig1, dwExSig2, bHomeDir, bHomeType);
}


DWORD CDeviceAxlMotionEthercat::__AxmM3ServoVelctrl(long lAxisNo, DWORD dwTFF, DWORD dwVREF, DWORD dwACCR, DWORD dwDECR, DWORD dwTLIM)
{
	return AxmM3ServoVelctrl(lAxisNo, dwTFF, dwVREF, dwACCR, dwDECR, dwTLIM);
}

DWORD CDeviceAxlMotionEthercat::__AxmM3ServoTrqctrl(long lAxisNo, DWORD dwVLIM, long lTQREF)
{
	return AxmM3ServoTrqctrl(lAxisNo, dwVLIM, lTQREF);
}

DWORD CDeviceAxlMotionEthercat::__AxmM3ServoGetParameter(long lAxisNo, WORD wNo, BYTE bSize, BYTE bMode, BYTE *pbParam)
{
	return AxmM3ServoGetParameter(lAxisNo, wNo, bSize, bMode, pbParam);
}

DWORD CDeviceAxlMotionEthercat::__AxmM3ServoSetParameter(long lAxisNo, WORD wNo, BYTE bSize, BYTE bMode, BYTE *pbParam)
{
	return AxmM3ServoSetParameter(lAxisNo, wNo, bSize, bMode, pbParam);
}

DWORD CDeviceAxlMotionEthercat::__AxmServoCmdExecution(long lAxisNo, DWORD dwCommand, DWORD dwSize, DWORD *pdwExcData)
{
	return AxmServoCmdExecution(lAxisNo, dwCommand, dwSize, pdwExcData);
}

DWORD CDeviceAxlMotionEthercat::__AxmM3ServoGetTorqLimit(long lAxisNo, DWORD *dwpTorqLimit)
{
	return AxmM3ServoGetTorqLimit(lAxisNo, dwpTorqLimit);
}

DWORD CDeviceAxlMotionEthercat::__AxmM3ServoSetTorqLimit(long lAxisNo, DWORD dwTorqLimit)
{
	return AxmM3ServoSetTorqLimit(lAxisNo, dwTorqLimit);
}

DWORD CDeviceAxlMotionEthercat::__AxmM3ServoGetSendSvCmdIOOutput(long lAxisNo, DWORD *dwData)
{
	return AxmM3ServoGetSendSvCmdIOOutput(lAxisNo, dwData);
}

DWORD CDeviceAxlMotionEthercat::__AxmM3ServoSetSendSvCmdIOOutput(long lAxisNo, DWORD dwData)
{
	return AxmM3ServoSetSendSvCmdIOOutput(lAxisNo, dwData);
}

DWORD CDeviceAxlMotionEthercat::__AxmM3ServoGetSvCmdCtrl(long lAxisNo, DWORD *dwData)
{
	return AxmM3ServoGetSvCmdCtrl(lAxisNo, dwData);
}

DWORD CDeviceAxlMotionEthercat::__AxmM3ServoSetSvCmdCtrl(long lAxisNo, DWORD dwData)
{
	return AxmM3ServoSetSvCmdCtrl(lAxisNo, dwData);
}

DWORD CDeviceAxlMotionEthercat::__AxmM3AdjustmentOperation(long lAxisNo, DWORD dwReqCode)
{
	return AxmM3AdjustmentOperation(lAxisNo, dwReqCode);
}

DWORD CDeviceAxlMotionEthercat::__AxmM3ServoSetMonSel(long lAxisNo, DWORD dwMon0, DWORD dwMon1, DWORD dwMon2)
{
	return AxmM3ServoSetMonSel(lAxisNo, dwMon0, dwMon1, dwMon2);
}

DWORD CDeviceAxlMotionEthercat::__AxmM3ServoGetMonSel(long lAxisNo, DWORD *upMon0, DWORD *upMon1, DWORD *upMon2)
{
	return AxmM3ServoGetMonSel(lAxisNo, upMon0, upMon1, upMon2);
}

DWORD CDeviceAxlMotionEthercat::__AxmM3ServoReadMonData(long lAxisNo, DWORD dwMonSel, DWORD *dwpMonData)
{
	return AxmM3ServoReadMonData(lAxisNo, dwMonSel, dwpMonData);
}

DWORD CDeviceAxlMotionEthercat::__AxmAdvTorqueContiSetAxisMap(long lCoord, long lSize, long *lpAxesNo, DWORD dwTLIM, DWORD dwConMode)
{
	return AxmAdvTorqueContiSetAxisMap(lCoord, lSize, lpAxesNo, dwTLIM, dwConMode);
}

DWORD CDeviceAxlMotionEthercat::__AxmM3ServoSetTorqProfile(long lCoord, long lAxisNo, long TorqueSign, DWORD dwVLIM, DWORD dwProfileMode, DWORD dwStdTorq, DWORD dwStopTorq)
{
	return AxmM3ServoSetTorqProfile(lCoord, lAxisNo, TorqueSign, dwVLIM, dwProfileMode, dwStdTorq, dwStopTorq);
}

DWORD CDeviceAxlMotionEthercat::__AxmM3ServoGetTorqProfile(long lCoord, long lAxisNo, long *lpTorqueSign, DWORD *updwVLIM, DWORD *upProfileMode, DWORD *upStdTorq, DWORD *upStopTorq)
{
	return AxmM3ServoGetTorqProfile(lCoord, lAxisNo, lpTorqueSign, updwVLIM, upProfileMode, upStdTorq, upStopTorq);
}


DWORD CDeviceAxlMotionEthercat::__AxmSignalSetInposRange(long lAxisNo, double dInposRange)
{
	return AxmSignalSetInposRange(lAxisNo, dInposRange);
}

DWORD CDeviceAxlMotionEthercat::__AxmSignalGetInposRange(long lAxisNo, double *dpInposRange)
{
	return AxmSignalGetInposRange(lAxisNo, dpInposRange);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotSetOverridePosMode(long lAxisNo, DWORD dwAbsRelMode)
{
	return AxmMotSetOverridePosMode(lAxisNo, dwAbsRelMode);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotGetOverridePosMode(long lAxisNo, DWORD *dwpAbsRelMode)
{
	return AxmMotGetOverridePosMode(lAxisNo, dwpAbsRelMode);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotSetOverrideLinePosMode(long lCoordNo, DWORD dwAbsRelMode)
{
	return AxmMotSetOverrideLinePosMode(lCoordNo, dwAbsRelMode);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotGetOverrideLinePosMode(long lCoordNo, DWORD *dwpAbsRelMode)
{
	return AxmMotGetOverrideLinePosMode(lCoordNo, dwpAbsRelMode);
}

DWORD CDeviceAxlMotionEthercat::__AxmMoveStartPosEx(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel, double dEndVel)
{
	return AxmMoveStartPosEx(lAxisNo, dPos, dVel, dAccel, dDecel, dEndVel);
}

DWORD CDeviceAxlMotionEthercat::__AxmMovePosEx(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel, double dEndVel)
{
	return AxmMovePosEx(lAxisNo, dPos, dVel, dAccel, dDecel, dEndVel);
}

DWORD CDeviceAxlMotionEthercat::__AxmMoveCoordStop(long lCoordNo, double dDecel)
{
	return AxmMoveCoordStop(lCoordNo, dDecel);
}

DWORD CDeviceAxlMotionEthercat::__AxmMoveCoordEStop(long lCoordNo)
{
	return AxmMoveCoordEStop(lCoordNo);
}

DWORD CDeviceAxlMotionEthercat::__AxmMoveCoordSStop(long lCoordNo)
{
	return AxmMoveCoordSStop(lCoordNo);
}

DWORD CDeviceAxlMotionEthercat::__AxmOverrideLinePos(long lCoordNo, double *dpOverridePos)
{
	return AxmOverrideLinePos(lCoordNo, dpOverridePos);
}

DWORD CDeviceAxlMotionEthercat::__AxmOverrideLineVel(long lCoordNo, double dOverrideVel, double *dpDistance)
{
	return AxmOverrideLineVel(lCoordNo, dOverrideVel, dpDistance);
}


DWORD CDeviceAxlMotionEthercat::__AxmOverrideLineAccelVelDecel(long lCoordNo, double dOverrideVelocity, double dMaxAccel, double dMaxDecel, double *dpDistance)
{
	return AxmOverrideLineAccelVelDecel(lCoordNo, dOverrideVelocity, dMaxAccel, dMaxDecel, dpDistance);
}

DWORD CDeviceAxlMotionEthercat::__AxmOverrideAccelVelDecelAtPos(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel, double dOverridePos, double dOverrideVel, double dOverrideAccel, double dOverrideDecel, long lTarget)
{
	return AxmOverrideAccelVelDecelAtPos(lAxisNo, dPos, dVel, dAccel, dDecel, dOverridePos, dOverrideVel, dOverrideAccel, dOverrideDecel, lTarget);
}


DWORD CDeviceAxlMotionEthercat::__AxmEGearSet(long lMasterAxisNo, long lSize, long* lpSlaveAxisNo, double* dpGearRatio)
{
	return AxmEGearSet(lMasterAxisNo, lSize, lpSlaveAxisNo, dpGearRatio);
}

DWORD CDeviceAxlMotionEthercat::__AxmEGearGet(long lMasterAxisNo, long* lpSize, long* lpSlaveAxisNo, double* dpGearRatio)
{
	return AxmEGearGet(lMasterAxisNo, lpSize, lpSlaveAxisNo, dpGearRatio);
}

DWORD CDeviceAxlMotionEthercat::__AxmEGearReset(long lMasterAxisNo)
{
	return AxmEGearReset(lMasterAxisNo);
}

DWORD CDeviceAxlMotionEthercat::__AxmEGearEnable(long lMasterAxisNo, DWORD dwEnable)
{
	return AxmEGearEnable(lMasterAxisNo, dwEnable);
}

DWORD CDeviceAxlMotionEthercat::__AxmEGearIsEnable(long lMasterAxisNo, DWORD *dwpEnable)
{
	return AxmEGearIsEnable(lMasterAxisNo, dwpEnable);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotSetEndVel(long lAxisNo, double dEndVelocity)
{
	return AxmMotSetEndVel(lAxisNo, dEndVelocity);
}

DWORD CDeviceAxlMotionEthercat::__AxmMotGetEndVel(long lAxisNo, double *dpEndVelocity)
{
	return AxmMotGetEndVel(lAxisNo, dpEndVelocity);
}

EGetFunction CDeviceAxlMotionEthercat::GetOpenNoHardware(bool * pParam)
{
	EGetFunction eReturn = EGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAxlMotionEthercat_OpenNoReset));

		eReturn = EGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESetFunction CDeviceAxlMotionEthercat::SetOpenNoHardware(bool bParam)
{
	ESetFunction eReturn = ESetFunction_UnknownError;

	EDeviceParameterAxlMotionEthercat eSaveNum = EDeviceParameterAxlMotionEthercat_OpenNoReset;

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlMotionEthercat[eSaveNum], g_lpszSwitch[bPreValue], g_lpszSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

#endif