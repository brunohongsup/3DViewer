#include "stdafx.h"

#include "DeviceSoftservoWMX2.h"

#include "../RavidCore/RavidDatabase.h"
#include "../RavidCore/RavidRecordset.h"

#include "../RavidFramework/LogManager.h"
#include "../RavidFramework/DeviceManager.h"
#include "../RavidFramework/LibraryManager.h"
#include "../RavidFramework/FrameworkManager.h"
#include "../RavidFramework/EventHandlerManager.h"
#include "../RavidFramework/MultiLanguageManager.h"

#include "../RavidFramework/RavidLedCtrl.h"


// #pragma warning(push)
// #pragma warning(disable:4098)
// #pragma warning(disable:4217)
#ifndef _WIN64

#include "../Libraries/Includes/SoftServoWMX2/WMXLibrary.h"
#pragma comment(lib, COMMONLIB_PREFIX "SoftServoWMX2/WMXDLL.lib")
#pragma comment(lib, COMMONLIB_PREFIX "SoftServoWMX2/WMXLibrary.lib")

#endif
//#pragma warning(pop)

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

IMPLEMENT_DYNAMIC(CDeviceSoftservoWMX2, CDeviceMotion)

BEGIN_MESSAGE_MAP(CDeviceSoftservoWMX2, CDeviceMotion)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


static LPCTSTR g_lpszParamSofservoWMX2[EDeviceParameterSoftservoWMX2_Count] =
{
	_T("DeviceID"),
	_T("Motion info path"),
	_T("Unit pulse"),
	_T("Start speed"),
	_T("End speed"),
	_T("Home level"),
	_T("Polarity limit level"),
	_T("Pos limit level"),
	_T("Neg limit level"),
	_T("Gear Denominator"),
	_T("Gear Numerator"),
	_T("Inpos width"),
	_T("Pos completed width"),
	_T("Home"),
	_T("Home type"),
	_T("Home direction"),
	_T("Home fast velocity"),
	_T("Home fast accel"),
	_T("Home fast deceleration"),
	_T("Home Slow velocity"),
	_T("Home Slow accel"),
	_T("Home Slow deceleration"),
	_T("Home shift velocity"),
	_T("Home shift accel"),
	_T("Home shift deceleration"),
	_T("Home shift distance"),
};

static LPCTSTR g_lpszSoftservoWMX2HomeType[EDeviceSoftservoWMX2HomeType_Count] =
{
	_T("HS"),
	_T("HS index pulse"),
};

static LPCTSTR g_lpszSoftservoWMX2HomeDirection[EDeviceSoftservoWMX2HomeDirection_Count] =
{
	_T("Normal"),
	_T("Reverse"),
};

static LPCTSTR g_lpszSoftservoWMX2LevelMethod[EDeviceSoftservoWMX2LevelMethod_Count] =
{
	_T("Low"),
	_T("High"),
};



CDeviceSoftservoWMX2::CDeviceSoftservoWMX2()
{
}

CDeviceSoftservoWMX2::~CDeviceSoftservoWMX2()
{
	Terminate();
}

EDeviceInitializeResult CDeviceSoftservoWMX2::Initialize()
{
	EDeviceInitializeResult eReturn = EDeviceInitializeResult_UnknownError;

	CString strMessage, strStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("SoftservoWMX2"));

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

#ifndef _WIN64
		long nDeviceID = _ttoi(GetDeviceID());

		if(nDeviceID >= MAX_ALLAXES)
			break;

		bool bNeedInitialze = true;

		for(size_t i = 0; i < CDeviceManager::GetDeviceCount(); ++i)
		{
			CDeviceSoftservoWMX2* pSoftServo = dynamic_cast<CDeviceSoftservoWMX2*>(CDeviceManager::GetDeviceByIndex(i));

			if(!pSoftServo)
				continue;

			if(pSoftServo == this)
				continue;

			if(!pSoftServo->IsInitialized())
				continue;

			if(_ttoi(pSoftServo->GetDeviceID()) == nDeviceID)
				continue;

			bNeedInitialze = false;

			m_pLibModule = pSoftServo->GetDevice();

			break;
		}


		if(bNeedInitialze)
		{
			m_pLibModule = new wmxapi::WMXLib;

			CString strPath;

			if(GetMotionInfoPath(&strPath))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("MotionInfo Path"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}
			
			strPath.AppendFormat(_T("_%d"), nDeviceID);

			if(*(strPath.GetBuffer() + strPath.GetLength() - 1) != '\\')
				strPath.AppendChar('\\');

			if(m_pLibModule->CreateDevice(strPath.GetBuffer(), wmxapi::PlatformType::EtherCATEngine) != wmxapi::ErrorCode::None)
			{
				strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtogeneratethedevicehandle);
				eReturn = EDeviceInitializeResult_NotCreateDeviceError;
				break;
			}

			wmxapi::Status wmxStatus;
			
			if(m_pLibModule->GetStatus(&wmxStatus, nDeviceID) == wmxapi::ErrorCode::None)
			{
				if(wmxStatus.engineStatus == wmxapi::PEState::Communicating)
				{
					for(int i = 0; i < MAX_ALLAXES; ++i)
					{
						if(wmxStatus.servoOn[i])
							m_pLibModule->axisControl->SetServoOn(i, false);
					}
				}
			}
			else
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Engine status"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			m_pLibModule->StopCommunication();

			int nServoAxis = _ttoi(GetDeviceID());

			DWORD dwDenom = 0.;

			if(GetGearRatioDenominator(&dwDenom))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Gear Denominator"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			DWORD dwNumer = 0.;

			if(GetGearRatioNumerator(&dwNumer))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Gear Numerator"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(m_pLibModule->config->SetGearRatio(nServoAxis, dwNumer, dwDenom) != wmxapi::ErrorCode::None)
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Gear Ratio"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}

			if(m_pLibModule->StartCommunication() != wmxapi::ErrorCode::None)
			{
				strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntopenthedevice);
				eReturn = EDeviceInitializeResult_NotOpenedDeviceError;
				break;
			}
		}



		double dblFastVel = 0., dblFastAcc = 0, dblFastDec = 0.;
		double dblSlowVel = 0., dblSlowAcc = 0, dblSlowDec = 0.;
		double dblShiftVel = 0., dblShiftAcc = 0, dblShiftDec = 0., dblShiftDist = 0.;

		EDeviceSoftservoWMX2LevelMethod eHomeLevel = EDeviceSoftservoWMX2LevelMethod_Count;

		EDeviceSoftservoWMX2HomeType eHomeType = EDeviceSoftservoWMX2HomeType_Count;

		EDeviceSoftservoWMX2HomeDirection eHomeDirection = EDeviceSoftservoWMX2HomeDirection_Count;

		if(GetHomeLevel(&eHomeLevel))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("home level"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(GetHomeFastVelocity(&dblFastVel))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Home Fast Velocity"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(GetHomeFastAccel(&dblFastAcc))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Home Fast Accel"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(GetHomeFastDeceleration(&dblFastDec))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Home Fast Deceleration"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(GetHomeSlowVelocity(&dblSlowVel))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Home Slow Velocity"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(GetHomeSlowAccel(&dblSlowAcc))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Home Slow Accel"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(GetHomeSlowDeceleration(&dblSlowDec))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Home Slow Deceleration"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(GetHomeShiftVelocity(&dblShiftVel))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Home Shift Velocity"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(GetHomeShiftAccel(&dblShiftAcc))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Home Shift Accel"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(GetHomeShiftDeceleration(&dblShiftDec))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Home Shift Deceleration"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(GetHomeShiftDistance(&dblShiftDist))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Home Shift Distance"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(GetHomeType(&eHomeType))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Home Type"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(GetHomeDirection(&eHomeDirection))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Home Direction"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		wmxapi::common::HomeParam wmxHome;

		if(m_pLibModule->config->GetHomeParam(nDeviceID, &wmxHome) != wmxapi::ErrorCode::None)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("home param"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		wmxHome.hsLogic = eHomeLevel;

		wmxHome.homingVelocityFast = dblFastVel;
		wmxHome.homingVelocityFastAcc = dblFastAcc;
		wmxHome.homingVelocityFastDec = dblFastDec;

		wmxHome.homingVelocitySlow = dblSlowVel;
		wmxHome.homingVelocitySlowAcc = dblSlowAcc;
		wmxHome.homingVelocitySlowDec = dblSlowDec;

		wmxHome.homeShiftVelocity = dblShiftVel;
		wmxHome.homeShiftAcc = dblShiftAcc;
		wmxHome.homeShiftDec = dblShiftDec;
		wmxHome.homeShiftDistance = dblShiftDist;
		
		if(eHomeType == EDeviceSoftservoWMX2HomeType_HSIndexPulse)
			wmxHome.homeType = wmxapi::common::HomeType::HSIndexPulse;
		else
			wmxHome.homeType = wmxapi::common::HomeType::HS;
		
		if(eHomeDirection == EDeviceSoftservoWMX2HomeDirection_Reverse)
			wmxHome.homeDirection =wmxapi::common::HomeDirection::Reverse;
		else
			wmxHome.homeDirection =wmxapi::common::HomeDirection::Normal;

		if(m_pLibModule->config->SetHomeParam(nDeviceID, &wmxHome) != wmxapi::ErrorCode::None)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("home param"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		EDeviceSoftservoWMX2LevelMethod ePosLimitLevel = EDeviceSoftservoWMX2LevelMethod_Count;

		EDeviceSoftservoWMX2LevelMethod eNegLimitLevel = EDeviceSoftservoWMX2LevelMethod_Count;

		if(GetPLimitLevel(&ePosLimitLevel))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Positive limit level"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(GetNLimitLevel(&eNegLimitLevel))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Negative limit level"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		wmxapi::common::LimitParam wmxLimit;

		if(m_pLibModule->config->GetLimitParam(nDeviceID, &wmxLimit) != wmxapi::ErrorCode::None)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Limit param"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		wmxLimit.lsLogic = ePosLimitLevel | eNegLimitLevel;

		wmxLimit.nearNegativeLSLogic = eNegLimitLevel;
		wmxLimit.externalNegativeLSLogic = eNegLimitLevel;

		wmxLimit.nearPositiveLSLogic = ePosLimitLevel;
		wmxLimit.externalPositiveLSLogic = ePosLimitLevel;
		
		if(m_pLibModule->config->SetLimitParam(nDeviceID, &wmxLimit) != wmxapi::ErrorCode::None)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Limit param"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		double dblStartSpeed = 0., dblEndSpeed = 0., dblUnit = 1.;

		if(GetStartSpeed(&dblStartSpeed))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Start speed"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(GetEndSpeed(&dblEndSpeed))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("End speed"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(GetUnitPulse(&dblUnit))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Unit Pulse"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		wmxapi::common::MotionParam wmxMotion;

		if(m_pLibModule->config->GetMotionParam(nDeviceID, &wmxMotion) != wmxapi::ErrorCode::None)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Motion param"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}
		
		wmxMotion.startingVelocity = dblStartSpeed / dblUnit;
		wmxMotion.endVelocity = dblEndSpeed / dblUnit;

		if(m_pLibModule->config->SetMotionParam(nDeviceID, &wmxMotion) != wmxapi::ErrorCode::None)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Motion param"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		long nInposWidth = 0., nCompleteWidth = 0.;

		if(GetInposWidth(&nInposWidth))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Inpos Width"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(GetPosCompletedWidth(&nCompleteWidth))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Completed Width"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		wmxapi::common::FeedbackParam wmxFeed;

		if(m_pLibModule->config->GetFeedbackParam(nDeviceID, &wmxFeed) != wmxapi::ErrorCode::None)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Feedback param"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		wmxFeed.inPosWidth = nInposWidth;
		wmxFeed.posCompletedWidth = nCompleteWidth;

		if(m_pLibModule->config->SetFeedbackParam(nDeviceID, &wmxFeed) != wmxapi::ErrorCode::None)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Feedback param"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		m_bIsInitialized = true;

		strStatus = CMultiLanguageManager::GetString(ELanguageParameter_Initialized);

		strMessage = CMultiLanguageManager::GetString(ELanguageParameter_TheDevicehasbeensuccessfullyinitialized);

		eReturn = EDeviceInitializeResult_OK;

		CEventHandlerManager::BroadcastOnDeviceInitialized(this);
#endif
	}
	while(false);

	if(!IsInitialized())
		Terminate();

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	SetStatus(strStatus);

	return eReturn;
}

EDeviceTerminateResult CDeviceSoftservoWMX2::Terminate()
{
	EDeviceTerminateResult eReturn = EDeviceTerminateResult_UnknownError;

	CString strMessage;

	SetStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("SoftservoWMX2"));

		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);

		return EDeviceTerminateResult_NotFoundApiError;
	}

	do
	{
#ifndef _WIN64
		if(!m_pLibModule)
			break;

		int nDeviceID = _ttoi(GetDeviceID());

		size_t szDeviceCnt = CDeviceManager::GetDeviceCount();

		bool bNeedTerminate = true;

		for(size_t i = 0; i < szDeviceCnt; ++i)
		{
			CDeviceSoftservoWMX2* pSoftServo = dynamic_cast<CDeviceSoftservoWMX2*>(CDeviceManager::GetDeviceByIndex(i));
			
			if(!pSoftServo)
				continue;

			if(pSoftServo == this)
				continue;

			if(!pSoftServo->IsInitialized())
				continue;

			if(_ttoi(pSoftServo->GetDeviceID()) == nDeviceID)
				continue;

			bNeedTerminate = false;

			m_pLibModule = pSoftServo->GetDevice();

			break;
		}

		if(GetServoStatus())
			SetServoOn(false);

		if(bNeedTerminate)
		{
			wmxapi::Status wmxStatus;

			m_pLibModule->GetStatus(&wmxStatus);

			if(wmxStatus.engineStatus == wmxapi::PEState::Communicating)
				m_pLibModule->StopCommunication();

			m_pLibModule->CloseDevice();

			delete m_pLibModule;
			m_pLibModule = nullptr;
		}
		else
			m_pLibModule = nullptr;


		strMessage = CMultiLanguageManager::GetString(ELanguageParameter_TheDevicehasbeensuccessfullyterminated);

		eReturn = EDeviceTerminateResult_OK;

		m_bIsInitialized = false;

		CEventHandlerManager::BroadcastOnDeviceTerminated(this);
#endif
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceSoftservoWMX2::LoadSettings()
{
	bool bReturn = false;

	m_vctParameterFieldConfigurations.clear();

	do
	{
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX2_DeviceID, g_lpszParamSofservoWMX2[EDeviceParameterSoftservoWMX2_DeviceID], _T("0"), EParameterFieldType_Edit, nullptr, _T("axis number"));

		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX2_MotionInfoPath, g_lpszParamSofservoWMX2[EDeviceParameterSoftservoWMX2_MotionInfoPath], _T("0"), EParameterFieldType_Edit, nullptr, _T("The full path of the directory that contains the motion engines"));

		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX2_UnitPulse, g_lpszParamSofservoWMX2[EDeviceParameterSoftservoWMX2_UnitPulse], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX2_StartSpeed, g_lpszParamSofservoWMX2[EDeviceParameterSoftservoWMX2_StartSpeed], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX2_EndSpeed, g_lpszParamSofservoWMX2[EDeviceParameterSoftservoWMX2_EndSpeed], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX2_HomeLevel, g_lpszParamSofservoWMX2[EDeviceParameterSoftservoWMX2_HomeLevel], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszSoftservoWMX2LevelMethod, EDeviceSoftservoWMX2LevelMethod_Count), nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX2_PolarityLimitLevel, g_lpszParamSofservoWMX2[EDeviceParameterSoftservoWMX2_PolarityLimitLevel], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszSoftservoWMX2LevelMethod, EDeviceSoftservoWMX2LevelMethod_Count), nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX2_PosLimitLevel, g_lpszParamSofservoWMX2[EDeviceParameterSoftservoWMX2_PosLimitLevel], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszSoftservoWMX2LevelMethod, EDeviceSoftservoWMX2LevelMethod_Count), nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX2_NegLimitLevel, g_lpszParamSofservoWMX2[EDeviceParameterSoftservoWMX2_NegLimitLevel], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszSoftservoWMX2LevelMethod, EDeviceSoftservoWMX2LevelMethod_Count), nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX2_GearRatioDenominator, g_lpszParamSofservoWMX2[EDeviceParameterSoftservoWMX2_GearRatioDenominator], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX2_GearRatioNumerator, g_lpszParamSofservoWMX2[EDeviceParameterSoftservoWMX2_GearRatioNumerator], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX2_InposWidth, g_lpszParamSofservoWMX2[EDeviceParameterSoftservoWMX2_InposWidth], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX2_PosCompletedWidth, g_lpszParamSofservoWMX2[EDeviceParameterSoftservoWMX2_PosCompletedWidth], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 0);
		

		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX2_Home, g_lpszParamSofservoWMX2[EDeviceParameterSoftservoWMX2_Home], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX2_HomeType, g_lpszParamSofservoWMX2[EDeviceParameterSoftservoWMX2_HomeType], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszSoftservoWMX2HomeType, EDeviceSoftservoWMX2HomeType_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX2_HomeDirection, g_lpszParamSofservoWMX2[EDeviceParameterSoftservoWMX2_HomeDirection], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszSoftservoWMX2HomeDirection, EDeviceSoftservoWMX2HomeDirection_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX2_HomeFastVelocity, g_lpszParamSofservoWMX2[EDeviceParameterSoftservoWMX2_HomeFastVelocity], _T("0.000000"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX2_HomeFastAccel, g_lpszParamSofservoWMX2[EDeviceParameterSoftservoWMX2_HomeFastAccel], _T("0.000000"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX2_HomeFastDeceleration, g_lpszParamSofservoWMX2[EDeviceParameterSoftservoWMX2_HomeFastDeceleration], _T("0.000000"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX2_HomeSlowVelocity, g_lpszParamSofservoWMX2[EDeviceParameterSoftservoWMX2_HomeSlowVelocity], _T("0.000000"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX2_HomeSlowAccel, g_lpszParamSofservoWMX2[EDeviceParameterSoftservoWMX2_HomeSlowAccel], _T("0.000000"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX2_HomeSlowDeceleration, g_lpszParamSofservoWMX2[EDeviceParameterSoftservoWMX2_HomeSlowDeceleration], _T("0.000000"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX2_HomeShiftVelocity, g_lpszParamSofservoWMX2[EDeviceParameterSoftservoWMX2_HomeShiftVelocity], _T("0.000000"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX2_HomeShiftAccel, g_lpszParamSofservoWMX2[EDeviceParameterSoftservoWMX2_HomeShiftAccel], _T("0.000000"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX2_HomeShiftDeceleration, g_lpszParamSofservoWMX2[EDeviceParameterSoftservoWMX2_HomeShiftDeceleration], _T("0.000000"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX2_HomeShiftDistance, g_lpszParamSofservoWMX2[EDeviceParameterSoftservoWMX2_HomeShiftDistance], _T("0.000000"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		
		bReturn = true;
	}
	while(false);

	return bReturn & __super::LoadSettings();
}

wmxapi::WMXLib* CDeviceSoftservoWMX2::GetDevice()
{
	return m_pLibModule;
}

ESoftservoGetFunction CDeviceSoftservoWMX2::GetUnitPulse(double* pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterSoftservoWMX2_UnitPulse));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX2::SetUnitPulse(double dblParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX2 eSaveID = EDeviceParameterSoftservoWMX2_UnitPulse;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			int nDeviceID = _ttoi(GetDeviceID());

			double dblStart = 0., dblEnd = 0.;

			if(GetStartSpeed(&dblStart))
			{
				eReturn = ESoftservoSetFunction_ReadOnDatabaseError;
				break;
			}

			if(GetEndSpeed(&dblEnd))
			{
				eReturn = ESoftservoSetFunction_ReadOnDatabaseError;
				break;
			}


#ifndef _WIN64
			wmxapi::common::MotionParam wmxMotion;

			if(m_pLibModule->config->GetMotionParam(nDeviceID, &wmxMotion) != wmxapi::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_ReadOnDeviceError;
				break;
			}

			wmxMotion.startingVelocity = dblStart / dblParam;
			wmxMotion.endVelocity = dblEnd / dblParam;


			if(m_pLibModule->config->SetMotionParam(nDeviceID, &wmxMotion) != wmxapi::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_WriteToDeviceError;
				break;
			}
#endif
		}

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ESoftservoSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX2[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX2::GetStartSpeed(double* pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterSoftservoWMX2_StartSpeed));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX2::SetStartSpeed(double dblParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX2 eSaveID = EDeviceParameterSoftservoWMX2_StartSpeed;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			int nDeviceID = _ttoi(GetDeviceID());

			double dblUnit = 1.;

			if(GetUnitPulse(&dblUnit))
			{
				eReturn = ESoftservoSetFunction_ReadOnDatabaseError;
				break;
			}

#ifndef _WIN64
			wmxapi::common::MotionParam wmxMotion;

			if(m_pLibModule->config->GetMotionParam(nDeviceID, &wmxMotion) != wmxapi::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_ReadOnDeviceError;
				break;
			}

			wmxMotion.startingVelocity = dblParam / dblUnit;

			if(m_pLibModule->config->SetMotionParam(nDeviceID, &wmxMotion) != wmxapi::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_WriteToDeviceError;
				break;
			}
#endif
		}

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ESoftservoSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX2[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX2::GetEndSpeed(double* pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterSoftservoWMX2_EndSpeed));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX2::SetEndSpeed(double dblParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX2 eSaveID = EDeviceParameterSoftservoWMX2_EndSpeed;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			int nDeviceID = _ttoi(GetDeviceID());

			double dblUnit = 1.;

			if(GetUnitPulse(&dblUnit))
			{
				eReturn = ESoftservoSetFunction_ReadOnDatabaseError;
				break;
			}

#ifndef _WIN64
			wmxapi::common::MotionParam wmxMotion;

			if(m_pLibModule->config->GetMotionParam(nDeviceID, &wmxMotion) != wmxapi::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_ReadOnDeviceError;
				break;
			}

			wmxMotion.endVelocity = dblParam / dblUnit;

			if(m_pLibModule->config->SetMotionParam(nDeviceID, &wmxMotion) != wmxapi::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_WriteToDeviceError;
				break;
			}
#endif
		}

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ESoftservoSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX2[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX2::GetHomeLevel(EDeviceSoftservoWMX2LevelMethod* pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceSoftservoWMX2LevelMethod)_ttoi(GetParamValue(EDeviceParameterSoftservoWMX2_HomeLevel));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX2::SetHomeLevel(EDeviceSoftservoWMX2LevelMethod eParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX2 eSaveID = EDeviceParameterSoftservoWMX2_HomeLevel;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceSoftservoWMX2LevelMethod_Count)
		{
			eReturn = ESoftservoSetFunction_NotFoundCommandError;
			break;
		}

 		if(IsInitialized())
		{
			int nDeviceID = _ttoi(GetDeviceID());

#ifndef _WIN64
			wmxapi::common::HomeParam wmxHome;

			if(m_pLibModule->config->GetHomeParam(nDeviceID, &wmxHome) != wmxapi::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_ReadOnDeviceError;
				break;
			}

			wmxHome.hsLogic = eParam;

			if(m_pLibModule->config->SetHomeParam(nDeviceID, &wmxHome) != wmxapi::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_WriteToDeviceError;
				break;
			}
#endif
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ESoftservoSetFunction_OK;
	}
	while(false);

	CString strMessage;
 
 	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX2[eSaveID], g_lpszSoftservoWMX2LevelMethod[nPreValue], g_lpszSoftservoWMX2LevelMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX2::GetPolarityLimitLevel(EDeviceSoftservoWMX2LevelMethod* pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceSoftservoWMX2LevelMethod)_ttoi(GetParamValue(EDeviceParameterSoftservoWMX2_PolarityLimitLevel));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX2::SetPolarityLimitLevel(EDeviceSoftservoWMX2LevelMethod eParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX2 eSaveID = EDeviceParameterSoftservoWMX2_PolarityLimitLevel;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxtMotionFS20PulseOutputMethod_Count)
		{
			eReturn = ESoftservoSetFunction_NotFoundCommandError;
			break;
		}

		if(IsInitialized())
		{
			int nDeviceID = _ttoi(GetDeviceID());

#ifndef _WIN64
			wmxapi::common::LimitParam wmxLimit;

			if(m_pLibModule->config->GetLimitParam(nDeviceID, &wmxLimit) != wmxapi::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_ReadOnDeviceError;
				break;
			}

			wmxLimit.lsLogic = eParam;

			if(m_pLibModule->config->SetLimitParam(nDeviceID, &wmxLimit) != wmxapi::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_WriteToDeviceError;
				break;
			}
#endif
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ESoftservoSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX2[eSaveID], g_lpszSoftservoWMX2LevelMethod[nPreValue], g_lpszSoftservoWMX2LevelMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX2::GetPLimitLevel(EDeviceSoftservoWMX2LevelMethod* pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceSoftservoWMX2LevelMethod)_ttoi(GetParamValue(EDeviceParameterSoftservoWMX2_PosLimitLevel));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX2::SetPLimitLevel(EDeviceSoftservoWMX2LevelMethod eParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX2 eSaveID = EDeviceParameterSoftservoWMX2_PosLimitLevel;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxtMotionFS20PulseOutputMethod_Count)
		{
			eReturn = ESoftservoSetFunction_NotFoundCommandError;
			break;
		}

		if(IsInitialized())
		{
			int nDeviceID = _ttoi(GetDeviceID());

#ifndef _WIN64
			wmxapi::common::LimitParam wmxLimit;

			if(m_pLibModule->config->GetLimitParam(nDeviceID, &wmxLimit) != wmxapi::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_ReadOnDeviceError;
				break;
			}

			wmxLimit.nearPositiveLSLogic = eParam;
			wmxLimit.externalPositiveLSLogic = eParam;
			
			if(m_pLibModule->config->SetLimitParam(nDeviceID, &wmxLimit) != wmxapi::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_WriteToDeviceError;
				break;
			}
#endif
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ESoftservoSetFunction_OK;
	}
	while(false);

	CString strMessage;
 
 	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX2[eSaveID], g_lpszSoftservoWMX2LevelMethod[nPreValue], g_lpszSoftservoWMX2LevelMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX2::GetNLimitLevel(EDeviceSoftservoWMX2LevelMethod* pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceSoftservoWMX2LevelMethod)_ttoi(GetParamValue(EDeviceParameterSoftservoWMX2_NegLimitLevel));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX2::SetNLimitLevel(EDeviceSoftservoWMX2LevelMethod eParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX2 eSaveID = EDeviceParameterSoftservoWMX2_NegLimitLevel;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxtMotionFS20PulseOutputMethod_Count)
		{
			eReturn = ESoftservoSetFunction_NotFoundCommandError;
			break;
		}

 		if(IsInitialized())
		{
			int nDeviceID = _ttoi(GetDeviceID());

#ifndef _WIN64
			wmxapi::common::LimitParam wmxLimit;

			if(m_pLibModule->config->GetLimitParam(nDeviceID, &wmxLimit) != wmxapi::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_ReadOnDeviceError;
				break;
			}

			wmxLimit.nearNegativeLSLogic = eParam;
			wmxLimit.externalNegativeLSLogic = eParam;
			
			if(m_pLibModule->config->SetLimitParam(nDeviceID, &wmxLimit) != wmxapi::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_WriteToDeviceError;
				break;
			}
#endif
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ESoftservoSetFunction_OK;
	}
	while(false);

	CString strMessage;
 
 	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX2[eSaveID], g_lpszSoftservoWMX2LevelMethod[nPreValue], g_lpszSoftservoWMX2LevelMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX2::GetGearRatioDenominator(DWORD * pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = (DWORD)_ttoi(GetParamValue(EDeviceParameterSoftservoWMX2_GearRatioDenominator));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX2::SetGearRatioDenominator(DWORD dwParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX2 eSaveID = EDeviceParameterSoftservoWMX2_GearRatioDenominator;

	DWORD dwPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = ESoftservoSetFunction_AlreadyInitializedError;
			break;
		}

		CString strData;
		strData.Format(_T("%u"), dwParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ESoftservoSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_u_to_u), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX2[eSaveID], dwPreValue, dwParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX2::GetGearRatioNumerator(DWORD * pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = (DWORD)_ttoi(GetParamValue(EDeviceParameterSoftservoWMX2_GearRatioNumerator));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX2::SetGearRatioNumerator(DWORD dwParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX2 eSaveID = EDeviceParameterSoftservoWMX2_GearRatioNumerator;

	DWORD dwPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = ESoftservoSetFunction_AlreadyInitializedError;
			break;
		}

		CString strData;
		strData.Format(_T("%u"), dwParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ESoftservoSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_u_to_u), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX2[eSaveID], dwPreValue, dwParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX2::GetMotionInfoPath(CString * pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = GetParamValue(EDeviceParameterSoftservoWMX2_MotionInfoPath);

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX2::SetMotionInfoPath(CString strParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX2 eSaveID = EDeviceParameterSoftservoWMX2_MotionInfoPath;

	CString strPreValue = GetParamValue(eSaveID);

	do
	{
		if(IsInitialized())
		{
			eReturn = ESoftservoSetFunction_AlreadyInitializedError;
			break;
		}

		CString strData;
		strData.Format(_T("%s"), strParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ESoftservoSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX2[eSaveID], strPreValue, strParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX2::GetInposWidth(long * pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterSoftservoWMX2_InposWidth));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX2::SetInposWidth(long nParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX2 eSaveID = EDeviceParameterSoftservoWMX2_InposWidth;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			int nDeviceID = _ttoi(GetDeviceID());

#ifndef _WIN64
			wmxapi::common::FeedbackParam wmxFeed;

			if(m_pLibModule->config->GetFeedbackParam(nDeviceID, &wmxFeed) != wmxapi::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_ReadOnDeviceError;
				break;
			}

			wmxFeed.inPosWidth = nParam;

			if(m_pLibModule->config->SetFeedbackParam(nDeviceID, &wmxFeed) != wmxapi::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_WriteToDeviceError;
				break;
			}
#endif
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ESoftservoSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX2[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX2::GetPosCompletedWidth(long * pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterSoftservoWMX2_PosCompletedWidth));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX2::SetPosCompletedWidth(long nParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX2 eSaveID = EDeviceParameterSoftservoWMX2_PosCompletedWidth;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			int nDeviceID = _ttoi(GetDeviceID());

#ifndef _WIN64
			wmxapi::common::FeedbackParam wmxFeed;

			if(m_pLibModule->config->GetFeedbackParam(nDeviceID, &wmxFeed) != wmxapi::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_ReadOnDeviceError;
				break;
			}

			wmxFeed.posCompletedWidth = nParam;

			if(m_pLibModule->config->SetFeedbackParam(nDeviceID, &wmxFeed) != wmxapi::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_WriteToDeviceError;
				break;
			}
#endif
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ESoftservoSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX2[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX2::GetHomeType(EDeviceSoftservoWMX2HomeType * pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceSoftservoWMX2HomeType)_ttoi(GetParamValue(EDeviceParameterSoftservoWMX2_HomeType));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX2::SetHomeType(EDeviceSoftservoWMX2HomeType eParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX2 eSaveID = EDeviceParameterSoftservoWMX2_HomeType;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceSoftservoWMX2HomeType_Count)
		{
			eReturn = ESoftservoSetFunction_NotFoundCommandError;
			break;
		}

		if(IsInitialized())
		{
			int nDeviceID = _ttoi(GetDeviceID());

#ifndef _WIN64
			wmxapi::common::HomeParam wmxHome;

			if(m_pLibModule->config->GetHomeParam(nDeviceID, &wmxHome) != wmxapi::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_ReadOnDeviceError;
				break;
			}

			if(eParam == EDeviceSoftservoWMX2HomeType_HSIndexPulse)
				wmxHome.homeType = wmxapi::common::HomeType::HSIndexPulse;
			else
				wmxHome.homeType = wmxapi::common::HomeType::HS;

			if(m_pLibModule->config->SetHomeParam(nDeviceID, &wmxHome) != wmxapi::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_WriteToDeviceError;
				break;
			}
#endif
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ESoftservoSetFunction_OK;
	}
	while(false);

	CString strMessage;
 
 	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX2[eSaveID], g_lpszSoftservoWMX2HomeType[nPreValue], g_lpszSoftservoWMX2HomeType[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX2::GetHomeDirection(EDeviceSoftservoWMX2HomeDirection * pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceSoftservoWMX2HomeDirection)_ttoi(GetParamValue(EDeviceParameterSoftservoWMX2_HomeDirection));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX2::SetHomeDirection(EDeviceSoftservoWMX2HomeDirection eParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX2 eSaveID = EDeviceParameterSoftservoWMX2_HomeDirection;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceSoftservoWMX2HomeType_Count)
		{
			eReturn = ESoftservoSetFunction_NotFoundCommandError;
			break;
		}

		if(IsInitialized())
		{
			int nDeviceID = _ttoi(GetDeviceID());

#ifndef _WIN64
			wmxapi::common::HomeParam wmxHome;

			if(m_pLibModule->config->GetHomeParam(nDeviceID, &wmxHome) != wmxapi::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_ReadOnDeviceError;
				break;
			}

			if(eParam == EDeviceSoftservoWMX2HomeDirection_Reverse)
				wmxHome.homeDirection = wmxapi::common::HomeDirection::Reverse;
			else
				wmxHome.homeDirection = wmxapi::common::HomeDirection::Normal;

			if(m_pLibModule->config->SetHomeParam(nDeviceID, &wmxHome) != wmxapi::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_WriteToDeviceError;
				break;
			}
#endif
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ESoftservoSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX2[eSaveID], g_lpszSoftservoWMX2HomeDirection[nPreValue], g_lpszSoftservoWMX2HomeDirection[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX2::GetHomeFastVelocity(double * pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterSoftservoWMX2_HomeFastVelocity));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX2::SetHomeFastVelocity(double dblParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX2 eSaveID = EDeviceParameterSoftservoWMX2_HomeFastVelocity;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			int nDeviceID = _ttoi(GetDeviceID());

#ifndef _WIN64
			wmxapi::common::HomeParam wmxHome;

			if(m_pLibModule->config->GetHomeParam(nDeviceID, &wmxHome) != wmxapi::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_ReadOnDeviceError;
				break;
			}

			wmxHome.homingVelocityFast = dblParam;

			if(m_pLibModule->config->SetHomeParam(nDeviceID, &wmxHome) != wmxapi::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_WriteToDeviceError;
				break;
			}
#endif
		}

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ESoftservoSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX2[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX2::GetHomeFastAccel(double * pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterSoftservoWMX2_HomeFastAccel));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX2::SetHomeFastAccel(double dblParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX2 eSaveID = EDeviceParameterSoftservoWMX2_HomeFastAccel;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			int nDeviceID = _ttoi(GetDeviceID());

#ifndef _WIN64
			wmxapi::common::HomeParam wmxHome;

			if(m_pLibModule->config->GetHomeParam(nDeviceID, &wmxHome) != wmxapi::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_ReadOnDeviceError;
				break;
			}

			wmxHome.homingVelocityFastAcc = dblParam;

			if(m_pLibModule->config->SetHomeParam(nDeviceID, &wmxHome) != wmxapi::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_WriteToDeviceError;
				break;
			}
#endif
		}

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ESoftservoSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX2[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX2::GetHomeFastDeceleration(double * pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterSoftservoWMX2_HomeFastDeceleration));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX2::SetHomeFastDeceleration(double dblParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX2 eSaveID = EDeviceParameterSoftservoWMX2_HomeFastDeceleration;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			int nDeviceID = _ttoi(GetDeviceID());

#ifndef _WIN64
			wmxapi::common::HomeParam wmxHome;

			if(m_pLibModule->config->GetHomeParam(nDeviceID, &wmxHome) != wmxapi::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_ReadOnDeviceError;
				break;
			}

			wmxHome.homingVelocityFastDec = dblParam;

			if(m_pLibModule->config->SetHomeParam(nDeviceID, &wmxHome) != wmxapi::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_WriteToDeviceError;
				break;
			}
#endif
		}

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ESoftservoSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX2[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX2::GetHomeSlowVelocity(double * pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterSoftservoWMX2_HomeSlowVelocity));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX2::SetHomeSlowVelocity(double dblParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX2 eSaveID = EDeviceParameterSoftservoWMX2_HomeSlowVelocity;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			int nDeviceID = _ttoi(GetDeviceID());

#ifndef _WIN64
			wmxapi::common::HomeParam wmxHome;

			if(m_pLibModule->config->GetHomeParam(nDeviceID, &wmxHome) != wmxapi::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_ReadOnDeviceError;
				break;
			}

			wmxHome.homingVelocitySlow = dblParam;

			if(m_pLibModule->config->SetHomeParam(nDeviceID, &wmxHome) != wmxapi::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_WriteToDeviceError;
				break;
			}
#endif
		}

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ESoftservoSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX2[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX2::GetHomeSlowAccel(double * pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterSoftservoWMX2_HomeSlowAccel));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX2::SetHomeSlowAccel(double dblParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX2 eSaveID = EDeviceParameterSoftservoWMX2_HomeSlowAccel;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			int nDeviceID = _ttoi(GetDeviceID());

#ifndef _WIN64
			wmxapi::common::HomeParam wmxHome;

			if(m_pLibModule->config->GetHomeParam(nDeviceID, &wmxHome) != wmxapi::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_ReadOnDeviceError;
				break;
			}

			wmxHome.homingVelocitySlowAcc = dblParam;

			if(m_pLibModule->config->SetHomeParam(nDeviceID, &wmxHome) != wmxapi::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_WriteToDeviceError;
				break;
			}
#endif
		}

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ESoftservoSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX2[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX2::GetHomeSlowDeceleration(double * pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterSoftservoWMX2_HomeSlowDeceleration));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX2::SetHomeSlowDeceleration(double dblParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX2 eSaveID = EDeviceParameterSoftservoWMX2_HomeSlowDeceleration;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			int nDeviceID = _ttoi(GetDeviceID());

#ifndef _WIN64
			wmxapi::common::HomeParam wmxHome;

			if(m_pLibModule->config->GetHomeParam(nDeviceID, &wmxHome) != wmxapi::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_ReadOnDeviceError;
				break;
			}

			wmxHome.homingVelocitySlowDec = dblParam;

			if(m_pLibModule->config->SetHomeParam(nDeviceID, &wmxHome) != wmxapi::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_WriteToDeviceError;
				break;
			}
#endif
		}

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ESoftservoSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX2[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX2::GetHomeShiftVelocity(double * pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterSoftservoWMX2_HomeShiftVelocity));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX2::SetHomeShiftVelocity(double dblParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX2 eSaveID = EDeviceParameterSoftservoWMX2_HomeShiftVelocity;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			int nDeviceID = _ttoi(GetDeviceID());

#ifndef _WIN64
			wmxapi::common::HomeParam wmxHome;

			if(m_pLibModule->config->GetHomeParam(nDeviceID, &wmxHome) != wmxapi::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_ReadOnDeviceError;
				break;
			}

			wmxHome.homeShiftVelocity = dblParam;

			if(m_pLibModule->config->SetHomeParam(nDeviceID, &wmxHome) != wmxapi::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_WriteToDeviceError;
				break;
			}
#endif
		}

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ESoftservoSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX2[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX2::GetHomeShiftAccel(double * pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterSoftservoWMX2_HomeShiftAccel));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX2::SetHomeShiftAccel(double dblParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX2 eSaveID = EDeviceParameterSoftservoWMX2_HomeShiftAccel;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			int nDeviceID = _ttoi(GetDeviceID());

#ifndef _WIN64
			wmxapi::common::HomeParam wmxHome;

			if(m_pLibModule->config->GetHomeParam(nDeviceID, &wmxHome) != wmxapi::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_ReadOnDeviceError;
				break;
			}

			wmxHome.homeShiftAcc = dblParam;

			if(m_pLibModule->config->SetHomeParam(nDeviceID, &wmxHome) != wmxapi::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_WriteToDeviceError;
				break;
			}
#endif
		}

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ESoftservoSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX2[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX2::GetHomeShiftDeceleration(double * pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterSoftservoWMX2_HomeShiftDeceleration));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX2::SetHomeShiftDeceleration(double dblParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX2 eSaveID = EDeviceParameterSoftservoWMX2_HomeShiftDeceleration;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			int nDeviceID = _ttoi(GetDeviceID());

#ifndef _WIN64
			wmxapi::common::HomeParam wmxHome;

			if(m_pLibModule->config->GetHomeParam(nDeviceID, &wmxHome) != wmxapi::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_ReadOnDeviceError;
				break;
			}

			wmxHome.homeShiftDec = dblParam;

			if(m_pLibModule->config->SetHomeParam(nDeviceID, &wmxHome) != wmxapi::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_WriteToDeviceError;
				break;
			}
#endif
		}

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ESoftservoSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX2[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX2::GetHomeShiftDistance(double * pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterSoftservoWMX2_HomeShiftDistance));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX2::SetHomeShiftDistance(double dblParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX2 eSaveID = EDeviceParameterSoftservoWMX2_HomeShiftDistance;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			int nDeviceID = _ttoi(GetDeviceID());

#ifndef _WIN64
			wmxapi::common::HomeParam wmxHome;

			if(m_pLibModule->config->GetHomeParam(nDeviceID, &wmxHome) != wmxapi::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_ReadOnDeviceError;
				break;
			}

			wmxHome.homeShiftDistance = dblParam;

			if(m_pLibModule->config->SetHomeParam(nDeviceID, &wmxHome) != wmxapi::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_WriteToDeviceError;
				break;
			}
#endif
		}

		CString strData;
		strData.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ESoftservoSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ESoftservoSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX2[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceSoftservoWMX2::GetServoStatus()
{
	bool bReturn = false;

	do 
	{
		if(!IsInitialized())
			break;

#ifndef _WIN64
		if(!m_pLibModule)
			break;

		int nDeviceID = _ttoi(GetDeviceID());

		wmxapi::Status wmxStatus;

		if(m_pLibModule->GetStatus(&wmxStatus, nDeviceID) != wmxapi::ErrorCode::None)
			break;

		if(!wmxStatus.servoOn[nDeviceID])
			break;
#endif
		bReturn = true;
	} 
	while(false);

	return bReturn;
}

bool CDeviceSoftservoWMX2::SetServoOn(bool bOn)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

#ifndef _WIN64
		if(!m_pLibModule)
			break;

		if(m_pLibModule->axisControl->SetServoOn(_ttoi(GetDeviceID()), bOn) != wmxapi::ErrorCode::None)
			break;
#endif
		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceSoftservoWMX2::GetCommandPosition(double* pPos)
{
	bool bReturn = false;

	do 
	{
		if(!pPos)
			break;

		if(!IsInitialized())
			break;

#ifndef _WIN64
		if(!m_pLibModule)
			break;

		if(m_pLibModule->axisControl->GetPosCommand(_ttoi(GetDeviceID()), pPos) != wmxapi::ErrorCode::None)
			break;

		double dblUnit = 1.;
		
		if(GetUnitPulse(&dblUnit))
			break;

		(*pPos) *= dblUnit;
#endif
		bReturn = true;
	} 
	while(false);

	return bReturn;
}

bool CDeviceSoftservoWMX2::SetCommandPositionClear()
{
	bool bReturn = false;

	do 
	{
		if(!IsInitialized())
			break;

#ifndef _WIN64
		if(!m_pLibModule)
			break;

		if(m_pLibModule->home->SetCommandPos(_ttoi(GetDeviceID()), 0.) != wmxapi::ErrorCode::None)
			break;
#endif
		bReturn = true;
	} 
	while(false);
	
	return bReturn;
}

bool CDeviceSoftservoWMX2::GetActualPosition(double* pPos)
{
	bool bReturn = false;

	do
	{
		if(!pPos)
			break;

		if(!IsInitialized())
			break;

#ifndef _WIN64
		if(!m_pLibModule)
			break;

		if(m_pLibModule->axisControl->GetPosFeedback(_ttoi(GetDeviceID()), pPos) != wmxapi::ErrorCode::None)
			break;

		double dblUnit = 1.;

		if(GetUnitPulse(&dblUnit))
			break;

		(*pPos) *= dblUnit;
#endif
		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceSoftservoWMX2::SetActualPositionClear()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

#ifndef _WIN64
		if(!m_pLibModule)
			break;

		if(m_pLibModule->home->SetFeedbackPos(_ttoi(GetDeviceID()), 0.) != wmxapi::ErrorCode::None)
			break;
#endif
		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceSoftservoWMX2::GetAlarmStatus()
{
	bool bReturn = false;

	do 
	{
		if(!IsInitialized())
			break;

#ifndef _WIN64
		if(!m_pLibModule)
			break;

		int nDeviceID = _ttoi(GetDeviceID());

		wmxapi::Status wmxStatus;

		if(m_pLibModule->GetStatus(&wmxStatus, nDeviceID) != wmxapi::ErrorCode::None)
			break;

		if(!wmxStatus.ampAlarm[nDeviceID])
			break;
#endif
		bReturn = true;
	} 
	while(false);

	return bReturn;
}

bool CDeviceSoftservoWMX2::SetAlarmClear()
{
	bool bReturn = false;

	do 
	{
		if(!IsInitialized())
			break;

#ifndef _WIN64
		if(!m_pLibModule)
			break;

		int nDeviceID = _ttoi(GetDeviceID());

		if(m_pLibModule->axisControl->ClearAxisAlarm(nDeviceID) != wmxapi::ErrorCode::None)
			break;

		if(m_pLibModule->axisControl->ClearAmpAlarm(nDeviceID) != wmxapi::ErrorCode::None)
			break;
#endif
		bReturn = true;
	} 
	while(false);

	return bReturn;
}

bool CDeviceSoftservoWMX2::GetInposition()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

#ifndef _WIN64
		if(!m_pLibModule)
			break;

		int nDeviceID = _ttoi(GetDeviceID());

		wmxapi::Status wmxStatus;

		if(m_pLibModule->GetStatus(&wmxStatus, nDeviceID) != wmxapi::ErrorCode::None)
			break;

		if(!wmxStatus.inPos[nDeviceID])
			break;
#endif
		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceSoftservoWMX2::GetLimitSensorN()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

#ifndef _WIN64
		if(!m_pLibModule)
			break;

		int nDeviceID = _ttoi(GetDeviceID());

		wmxapi::Status wmxStatus;

		if(m_pLibModule->GetStatus(&wmxStatus, nDeviceID) != wmxapi::ErrorCode::None)
			break;

		if(!wmxStatus.hardNegativeLS[nDeviceID])
			break;
#endif
		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceSoftservoWMX2::GetLimitSensorP()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

#ifndef _WIN64
		if(!m_pLibModule)
			break;

		int nDeviceID = _ttoi(GetDeviceID());

		wmxapi::Status wmxStatus;

		if(m_pLibModule->GetStatus(&wmxStatus, nDeviceID) != wmxapi::ErrorCode::None)
			break;

		if(!wmxStatus.hardPositiveLS[nDeviceID])
			break;
#endif
		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceSoftservoWMX2::GetHomeSensor()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

#ifndef _WIN64
		if(!m_pLibModule)
			break;

		int nDeviceID = _ttoi(GetDeviceID());

		wmxapi::Status wmxStatus;

		if(m_pLibModule->GetStatus(&wmxStatus, nDeviceID) != wmxapi::ErrorCode::None)
			break;

		if(!wmxStatus.homeSwitch[nDeviceID])
			break;
#endif
		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceSoftservoWMX2::MovePosition(double dPos, double dVel, double dAcc, bool bSCurve)
{
	bool bReturn = false;

	do 
	{
		if(!IsInitialized())
			break;

#ifndef _WIN64
		if(!m_pLibModule)
			break;
		
		double dblUnit = 1.;

		if(GetUnitPulse(&dblUnit))
			break;

		if(!dblUnit)
			break;

		dPos /= dblUnit;
		dVel /= dblUnit;
		dAcc /= dblUnit;

		m_dblLastVelocity = dAcc;

		if(m_pLibModule->jerkRatioMotion->StartPos(_ttoi(GetDeviceID()), wmxapi::ProfileType::JerkLimited, dPos, dVel, dAcc, dAcc, 1.0, 1.0, 0., 0.) != wmxapi::ErrorCode::None)
			break;
#endif
		bReturn = true;
	} 
	while(false);
	
	return bReturn;
}

bool CDeviceSoftservoWMX2::MoveDistance(double dDist, double dVel, double dAcc, bool bSCurve)
{
	bool bReturn = false;

	do 
	{
		if(!IsInitialized())
			break;

#ifndef _WIN64
		if(!m_pLibModule)
			break;

		double dblUnit = 1.;

		if(GetUnitPulse(&dblUnit))
			break;

		if(!dblUnit)
			break;

		dDist /= dblUnit;
		dVel /= dblUnit;
		dAcc /= dblUnit;

		m_dblLastVelocity = dAcc;

		if(m_pLibModule->jerkMotion->StartMov(_ttoi(GetDeviceID()), wmxapi::ProfileType::Trapezoidal, dDist, dVel, dAcc, dAcc, 1.0, 1.0, 0., 0.) != wmxapi::ErrorCode::None)
			break;
#endif
		bReturn = true;
	} 
	while(false);
	
	return bReturn;
}

bool CDeviceSoftservoWMX2::MoveVelocity(double dVel, double dAcc, bool bSCurve)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

#ifndef _WIN64
		if(!m_pLibModule)
			break;

		double dblUnit = 1.;

		if(GetUnitPulse(&dblUnit))
			break;

		if(!dblUnit)
			break;

		dVel /= dblUnit;
		dAcc /= dblUnit;

		m_dblLastVelocity = dAcc;

		if(m_pLibModule->basicMotion->StartJog(_ttoi(GetDeviceID()), dVel, dAcc) != wmxapi::ErrorCode::None)
			break;
#endif
		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceSoftservoWMX2::MoveJog(double dVel, double dAcl, BOOL bSCurve)
{
	bool bReturn = false;

	do 
	{
		if(!IsInitialized())
			break;

#ifndef _WIN64
		if(!m_pLibModule)
			break;

		double dblUnit = 1.;

		if(GetUnitPulse(&dblUnit))
			break;

		if(!dblUnit)
			break;

		dVel /= dblUnit;
		dAcl /= dblUnit;

		m_dblLastVelocity = dAcl;

		if(m_pLibModule->jerkMotion->StartJog(_ttoi(GetDeviceID()), wmxapi::ProfileType::Trapezoidal, dVel, dAcl, dAcl) != wmxapi::ErrorCode::None)
			break;
#endif
		bReturn = true;
	} 
	while(false);
	
	return bReturn;
}

bool CDeviceSoftservoWMX2::StopJog()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

#ifndef _WIN64
		if(!m_pLibModule)
			break;

		int nDeviceID = _ttoi(GetDeviceID());

		wmxapi::common::MotionParam wmxMotion;

		if(m_pLibModule->config->GetMotionParam(nDeviceID, &wmxMotion) != wmxapi::ErrorCode::None)
			break;

		wmxMotion.quickStopDec = m_dblLastVelocity;

		if(m_pLibModule->config->SetMotionParam(nDeviceID, &wmxMotion) != wmxapi::ErrorCode::None)
			break;

		if(m_pLibModule->basicMotion->Stop(nDeviceID) != wmxapi::ErrorCode::None)
			break;
#endif
		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceSoftservoWMX2::IsMotionDone()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

#ifndef _WIN64
		if(!m_pLibModule)
			break;

		int nDeviceID = _ttoi(GetDeviceID());

		wmxapi::Status wmxStatus;

		if(m_pLibModule->GetStatus(&wmxStatus, nDeviceID) != wmxapi::ErrorCode::None)
			break;

		if(wmxStatus.opState[nDeviceID] != wmxapi::OperationState::Idle)
			break;
#endif
		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceSoftservoWMX2::MotorStop()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

#ifndef _WIN64
		if(!m_pLibModule)
			break;

		int nDeviceID = _ttoi(GetDeviceID());

		wmxapi::common::MotionParam wmxMotion;

		if(m_pLibModule->config->GetMotionParam(nDeviceID, &wmxMotion) != wmxapi::ErrorCode::None)
			break;

		wmxMotion.quickStopDec = m_dblLastVelocity;

		if(m_pLibModule->config->SetMotionParam(nDeviceID, &wmxMotion) != wmxapi::ErrorCode::None)
			break;

		if(m_pLibModule->basicMotion->Stop(nDeviceID) != wmxapi::ErrorCode::None)
			break;
#endif
		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceSoftservoWMX2::MotorEStop()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

#ifndef _WIN64
		if(!m_pLibModule)
			break;

		if(m_pLibModule->ExecEStop(wmxapi::EStopLevel::Level1) != wmxapi::ErrorCode::None)
			break;
#endif
		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceSoftservoWMX2::ChangeSpeed(double dSpeed)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

#ifndef _WIN64
		if(!m_pLibModule)
			break;

		double dblUnit = 1.;

		if(GetUnitPulse(&dblUnit))
			break;

		if(!dblUnit)
			break;

		dSpeed /= dblUnit;

		if(m_pLibModule->axisControl->GetVelCommand(_ttoi(GetDeviceID()), &dSpeed) != wmxapi::ErrorCode::None)
			break;
#endif
		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceSoftservoWMX2::WriteGenOutBit(int nBit, bool bOn)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceSoftservoWMX2::ReadGenOutBit(int nBit)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceSoftservoWMX2::ReadGenInputBit(int nBit)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceSoftservoWMX2::MoveToHome()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

#ifndef _WIN64
		if(!m_pLibModule)
			break;

		if(m_pLibModule->home->StartHome(_ttoi(GetDeviceID())) != wmxapi::ErrorCode::None)
			break;
#endif
		bReturn = true;
	}
	while(false);

	return bReturn;
}

EDeviceMotionHommingStatus CDeviceSoftservoWMX2::GetHommingStatus()
{
	EDeviceMotionHommingStatus eStatus = EDeviceMotionHommingStatus_Error;

	do
	{
		if(!IsInitialized())
			break;

#ifndef _WIN64
		if(!m_pLibModule)
			break;

		int nDeviceID = _ttoi(GetDeviceID());

		wmxapi::Status wmxStatus;

		if(m_pLibModule->GetStatus(&wmxStatus, nDeviceID) != wmxapi::ErrorCode::None)
			break;

		if(wmxStatus.homeDone[nDeviceID] == 1)
			eStatus = EDeviceMotionHommingStatus_Success;
		else
		{
			if(wmxStatus.homeError[nDeviceID] == wmxapi::HomeError::NoError)
				eStatus = EDeviceMotionHommingStatus_Searching;
		}
#endif
	}
	while(false);

	return eStatus;
}

bool CDeviceSoftservoWMX2::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = true;

	bool bFoundID = true;

	do
	{
		switch(nParam)
		{
		case EDeviceParameterSoftservoWMX2_DeviceID:
			bReturn = !SetDeviceID(strValue);
			break;
		case EDeviceParameterSoftservoWMX2_MotionInfoPath:
			bReturn = !SetMotionInfoPath(strValue);
			break;
		case EDeviceParameterSoftservoWMX2_UnitPulse:
			bReturn = !SetUnitPulse(_ttof(strValue));
			break;
		case EDeviceParameterSoftservoWMX2_StartSpeed:
			bReturn = !SetStartSpeed(_ttof(strValue));
			break;
		case EDeviceParameterSoftservoWMX2_EndSpeed:
			bReturn = !SetEndSpeed(_ttof(strValue));
			break;
		case EDeviceParameterSoftservoWMX2_HomeLevel:
			bReturn = !SetHomeLevel((EDeviceSoftservoWMX2LevelMethod)_ttoi(strValue));
			break;
		case EDeviceParameterSoftservoWMX2_PolarityLimitLevel:
			bReturn = !SetPolarityLimitLevel((EDeviceSoftservoWMX2LevelMethod)_ttoi(strValue));
			break;
		case EDeviceParameterSoftservoWMX2_PosLimitLevel:
			bReturn = !SetPLimitLevel((EDeviceSoftservoWMX2LevelMethod)_ttoi(strValue));
			break;
		case EDeviceParameterSoftservoWMX2_NegLimitLevel:
			bReturn = !SetNLimitLevel((EDeviceSoftservoWMX2LevelMethod)_ttoi(strValue));
			break;
		case EDeviceParameterSoftservoWMX2_GearRatioDenominator:
			bReturn = !SetGearRatioDenominator((DWORD)_ttoi(strValue));
			break;
		case EDeviceParameterSoftservoWMX2_GearRatioNumerator:
			bReturn = !SetGearRatioNumerator((DWORD)_ttoi(strValue));
			break;
		case EDeviceParameterSoftservoWMX2_InposWidth:
			bReturn = !SetInposWidth(_ttoi(strValue));
			break;
		case EDeviceParameterSoftservoWMX2_PosCompletedWidth:
			bReturn = !SetPosCompletedWidth(_ttoi(strValue));
			break;
		case EDeviceParameterSoftservoWMX2_HomeType:
			bReturn = !SetHomeType((EDeviceSoftservoWMX2HomeType)_ttoi(strValue));
			break;
		case EDeviceParameterSoftservoWMX2_HomeDirection:
			bReturn = !SetHomeDirection((EDeviceSoftservoWMX2HomeDirection)_ttoi(strValue));
			break;
		case EDeviceParameterSoftservoWMX2_HomeFastVelocity:
			bReturn = !SetHomeFastVelocity(_ttof(strValue));
			break;
		case EDeviceParameterSoftservoWMX2_HomeFastAccel:
			bReturn = !SetHomeFastAccel(_ttof(strValue));
			break;
		case EDeviceParameterSoftservoWMX2_HomeFastDeceleration:
			bReturn = !SetHomeFastDeceleration(_ttof(strValue));
			break;
		case EDeviceParameterSoftservoWMX2_HomeSlowVelocity:
			bReturn = !SetHomeSlowVelocity(_ttof(strValue));
			break;
		case EDeviceParameterSoftservoWMX2_HomeSlowAccel:
			bReturn = !SetHomeSlowAccel(_ttof(strValue));
			break;
		case EDeviceParameterSoftservoWMX2_HomeSlowDeceleration:
			bReturn = !SetHomeSlowDeceleration(_ttof(strValue));
			break;
		case EDeviceParameterSoftservoWMX2_HomeShiftVelocity:
			bReturn = !SetHomeShiftVelocity(_ttof(strValue));
			break;
		case EDeviceParameterSoftservoWMX2_HomeShiftAccel:
			bReturn = !SetHomeShiftAccel(_ttof(strValue));
			break;
		case EDeviceParameterSoftservoWMX2_HomeShiftDeceleration:
			bReturn = !SetHomeShiftDeceleration(_ttof(strValue));
			break;
		case EDeviceParameterSoftservoWMX2_HomeShiftDistance:
			bReturn = !SetHomeShiftDistance(_ttof(strValue));
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

bool CDeviceSoftservoWMX2::DoesModuleExist()
{
	bool bReturn = false;

	CString strModuleName;

	do
	{
		strModuleName.Format(_T("WMXDLL.dll"));

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