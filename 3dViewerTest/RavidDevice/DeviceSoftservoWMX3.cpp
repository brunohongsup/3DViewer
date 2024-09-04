#include "stdafx.h"

#include "DeviceSoftservoWMX3.h"

#ifdef USING_DEVICE

#include "../RavidCore/RavidDatabase.h"
#include "../RavidCore/RavidRecordset.h"

#include "../RavidFramework/LogManager.h"
#include "../RavidFramework/DeviceManager.h"
#include "../RavidFramework/LibraryManager.h"
#include "../RavidFramework/FrameworkManager.h"
#include "../RavidFramework/EventHandlerManager.h"
#include "../RavidFramework/MultiLanguageManager.h"

#include "../RavidFramework/RavidLedCtrl.h"

#include "../Libraries/Includes/SoftServoWMX3/EcApi.h"
#include "../Libraries/Includes/SoftServoWMX3/WMX3Api.h"
#include "../Libraries/Includes/SoftServoWMX3/CoreMotionApi.h"

#pragma comment(lib, "legacy_stdio_definitions.lib")
// imdll.dll
#pragma comment(lib, COMMONLIB_PREFIX "SoftServoWMX3/IMDll.lib")
#pragma comment(lib, COMMONLIB_PREFIX "SoftServoWMX3/CoreMotionApi.lib")
#pragma comment(lib, COMMONLIB_PREFIX "SoftServoWMX3/WMX3Api.lib")
#pragma comment(lib, COMMONLIB_PREFIX "SoftServoWMX3/EcApi.lib")

#define MAX_ALLAXES wmx3Api::constants::maxAxes

using namespace Ravid;
using namespace Ravid::Device;
using namespace Ravid::Database;
using namespace Ravid::Framework;
using namespace Ravid::Algorithms;
using namespace Ravid::Miscellaneous;

IMPLEMENT_DYNAMIC(CDeviceSoftservoWMX3, CDeviceMotion)

BEGIN_MESSAGE_MAP(CDeviceSoftservoWMX3, CDeviceMotion)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static LPCTSTR g_lpszParamSofservoWMX3[EDeviceParameterSoftservoWMX3_Count] =
{
	_T("DeviceID"),
	_T("Parameter"),
	_T("Motion info path"),
	_T("Parameter path"),
	_T("Start speed"),
	_T("End speed"),
	_T("Level"),
	_T("Home level"),
	_T("Pos limit level"),
	_T("Near Pos limit level"),
	_T("Extern Pos limit level"),
	_T("Neg limit level"),
	_T("Near Neg limit level"),
	_T("Extern Neg limit level"),
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

static LPCTSTR g_lpszSoftservoWMX3HomeType[EDeviceSoftservoWMX3HomeType_Count] =
{
	_T("CurrentPos"),
	_T("ZPulse"),
	_T("HS"),
	_T("HSHS"),
	_T("HSZPulse"),
	_T("HSReverseZPulse"),
	_T("HSOff"),
	_T("HSOffZPulse"),
	_T("HSOffReverseZPulse"),
	_T("LSReverseZPulse"),
	_T("NearLSReverseZPulse"),
	_T("ExternalLSReverseZPulse"),
	_T("TrqLimit"),
	_T("TouchProbe"),
	_T("HSTouchProbe"),
	_T("LS"),
	_T("NearLS"),
	_T("ExternalLS"),
	_T("MechanicalEndDetection"),
	_T("MechanicalEndDetectionHS"),
	_T("MechanicalEndDetectionLS"),
	_T("MechanicalEndDetectionReverseZPuls"),
};

static LPCTSTR g_lpszSoftservoWMX3HomeDirection[EDeviceSoftservoWMX3HomeDirection_Count] =
{
	_T("Positive"),
	_T("Negative"),
};

static LPCTSTR g_lpszSoftservoWMX3LevelMethod[EDeviceSoftservoWMX3LevelMethod_Count] =
{
	_T("Low"),
	_T("High"),
};

static LPCTSTR g_lpszSoftservoWMX3InitializeType[EDeviceSoftservoWMX3InitializeType_Count] =
{
	_T("Motion Parameter"),
	_T("Ravid Parameter"),
};


CDeviceSoftservoWMX3::CDeviceSoftservoWMX3()
{
}

CDeviceSoftservoWMX3::~CDeviceSoftservoWMX3()
{
	Terminate();
}

EDeviceInitializeResult CDeviceSoftservoWMX3::Initialize()
{
	EDeviceInitializeResult eReturn = EDeviceInitializeResult_UnknownError;

	CString strMessage, strStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("SoftservoWMX3"));

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

		if(nDeviceID >= MAX_ALLAXES)
			break;

		bool bNeedInitialze = true;

		for(size_t i = 0; i < CDeviceManager::GetDeviceCount(); ++i)
		{
			CDeviceSoftservoWMX3* pSoftServo = dynamic_cast<CDeviceSoftservoWMX3*>(CDeviceManager::GetDeviceByIndex(i));

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
			m_pCMModule = pSoftServo->GetCoreMotion();

			break;
		}

		EDeviceSoftservoWMX3InitializeType eInitType = EDeviceSoftservoWMX3InitializeType_Count;
		GetInitializetype(&eInitType);

		if(bNeedInitialze)
		{
			m_pLibModule = new wmx3Api::WMX3Api;

			CString strPath;

			if(GetMotionInfoPath(&strPath) != ESoftservoGetFunction_OK)
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("MotionInfo Path"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			long nError = m_pLibModule->CreateDevice(strPath.GetBuffer(), wmx3Api::DeviceType::DeviceTypeNormal);
			if(nError != wmx3Api::ErrorCode::None)
			{
				strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtogeneratethedevicehandle);
				eReturn = EDeviceInitializeResult_NotCreateDeviceError;
				break;
			}

			m_pLibModule->SetDeviceName("Ravid");

			wmx3Api::EngineStatus wmxStatus;
			m_pCMModule = new wmx3Api::CoreMotion(m_pLibModule);

			if(m_pLibModule->GetEngineStatus(&wmxStatus) == wmx3Api::ErrorCode::None)
			{
				if(wmxStatus.state == wmx3Api::EngineState::Communicating)
				{
					wmx3Api::CoreMotionStatus cmStatus;
					for(int i = 0; i < MAX_ALLAXES; ++i)
					{
						m_pCMModule->GetStatus(&cmStatus);
						if(cmStatus.axesStatus[i].servoOn)
							m_pCMModule->axisControl->SetServoOn(i, false);
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
			Sleep(50);

			if(m_pLibModule->StartCommunication() != wmx3Api::ErrorCode::None)
			{
				strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntopenthedevice);
				eReturn = EDeviceInitializeResult_NotOpenedDeviceError;
				break;
			}

			CString strParam;
			if(ESoftservoGetFunction_OK != GetParamPath(&strParam))
			{
				strMessage = _T("Failed to read parampath.");
				eReturn = EDeviceInitializeResult_UnknownError;
				break;
			}

			if(strParam.IsEmpty() || (strParam.Find(_T(".xml")) == -1))
			{
				strMessage = _T("Parampath is invaild.");
				eReturn = EDeviceInitializeResult_UnknownError;
				break;
			}

			nError = m_pCMModule->config->ImportAndSetAll(strParam.GetBuffer());
			if(nError != wmx3Api::ErrorCode::None)
			{
				char errString[256];
				m_pLibModule->ErrorToString(nError, errString, sizeof(errString));
				CString str;
				str = (LPSTR)errString;
				strMessage.Format(str);
				eReturn = EDeviceInitializeResult_UnknownError;
				break;
			}
		}

		wmx3Api::Config::HomeParam wmxHome;
		if(m_pCMModule->config->GetHomeParam(nDeviceID, &wmxHome) != wmx3Api::ErrorCode::None)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("home param"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		wmx3Api::Config::LimitParam wmxLimit;
		if(m_pCMModule->config->GetLimitParam(nDeviceID, &wmxLimit) != wmx3Api::ErrorCode::None)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Limit param"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		wmx3Api::Config::MotionParam wmxMotion;
		if(m_pCMModule->config->GetMotionParam(nDeviceID, &wmxMotion) != wmx3Api::ErrorCode::None)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Motion param"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		wmx3Api::Config::FeedbackParam wmxFeed;
		if(m_pCMModule->config->GetFeedbackParam(nDeviceID, &wmxFeed) != wmx3Api::ErrorCode::None)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Feedback param"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		switch(eInitType)
		{
		case EDeviceSoftservoWMX3InitializeType_Motion:
			{
				double dblDenom = 0.;
				double dblNumer = 0.;

				if(m_pCMModule->config->GetGearRatio(nDeviceID, &dblNumer, &dblDenom) != wmx3Api::ErrorCode::None)
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Gear Ratio"));
					eReturn = EDeviceInitializeResult_ReadOnDeviceError;
					break;
				}

				if(SetGearRatioDenominator(dblDenom))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Gear Denominator"));
					eReturn = EDeviceInitializeResult_WriteToDatabaseError;
					break;
				}

				if(SetGearRatioNumerator(dblNumer))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Gear Numerator"));
					eReturn = EDeviceInitializeResult_WriteToDatabaseError;
					break;
				}

				if(SetHomeLevel((EDeviceSoftservoWMX3LevelMethod)wmxHome.invertHSPolarity))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("home level"));
					eReturn = EDeviceInitializeResult_WriteToDatabaseError;
					break;
				}

				if(SetHomeFastVelocity(wmxHome.homingVelocityFast))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Home Fast Velocity"));
					eReturn = EDeviceInitializeResult_WriteToDatabaseError;
					break;
				}

				if(SetHomeFastAccel(wmxHome.homingVelocityFastAcc))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Home Fast Accel"));
					eReturn = EDeviceInitializeResult_WriteToDatabaseError;
					break;
				}

				if(SetHomeFastDeceleration(wmxHome.homingVelocityFastDec))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Home Fast Deceleration"));
					eReturn = EDeviceInitializeResult_WriteToDatabaseError;
					break;
				}

				if(SetHomeSlowVelocity(wmxHome.homingVelocitySlow))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Home Slow Velocity"));
					eReturn = EDeviceInitializeResult_WriteToDatabaseError;
					break;
				}

				if(SetHomeSlowAccel(wmxHome.homingVelocitySlowAcc))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Home Slow Accel"));
					eReturn = EDeviceInitializeResult_WriteToDatabaseError;
					break;
				}

				if(SetHomeSlowDeceleration(wmxHome.homingVelocitySlowDec))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Home Slow Deceleration"));
					eReturn = EDeviceInitializeResult_WriteToDatabaseError;
					break;
				}

				if(SetHomeShiftVelocity(wmxHome.homeShiftVelocity))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Home Shift Velocity"));
					eReturn = EDeviceInitializeResult_WriteToDatabaseError;
					break;
				}

				if(SetHomeShiftAccel(wmxHome.homeShiftAcc))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Home Shift Accel"));
					eReturn = EDeviceInitializeResult_WriteToDatabaseError;
					break;
				}

				if(SetHomeShiftDeceleration(wmxHome.homeShiftDec))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Home Shift Deceleration"));
					eReturn = EDeviceInitializeResult_WriteToDatabaseError;
					break;
				}

				if(SetHomeShiftDistance(wmxHome.homeShiftDistance))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Home Shift Distance"));
					eReturn = EDeviceInitializeResult_WriteToDatabaseError;
					break;
				}

				if(SetHomeType((EDeviceSoftservoWMX3HomeType)(wmxHome.homeType)))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Home Type"));
					eReturn = EDeviceInitializeResult_WriteToDatabaseError;
					break;
				}

				if(SetHomeDirection((EDeviceSoftservoWMX3HomeDirection)wmxHome.homeDirection))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Home Direction"));
					eReturn = EDeviceInitializeResult_WriteToDatabaseError;
					break;
				}

				if(SetPLimitLevel((EDeviceSoftservoWMX3LevelMethod)wmxLimit.invertPositiveLSPolarity))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Positive limit level"));
					eReturn = EDeviceInitializeResult_WriteToDatabaseError;
					break;
				}

				if(SetNearPLimitLevel((EDeviceSoftservoWMX3LevelMethod)wmxLimit.invertNearPositiveLSPolarity))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Near positive limit level"));
					eReturn = EDeviceInitializeResult_WriteToDatabaseError;
					break;
				}

				if(SetExternPLimitLevel((EDeviceSoftservoWMX3LevelMethod)wmxLimit.invertExternalPositiveLSPolarity))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Extern positive limit level"));
					eReturn = EDeviceInitializeResult_WriteToDatabaseError;
					break;
				}

				if(SetNLimitLevel((EDeviceSoftservoWMX3LevelMethod)wmxLimit.invertNegativeLSPolarity))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Negative limit level"));
					eReturn = EDeviceInitializeResult_WriteToDatabaseError;
					break;
				}

				if(SetNearNLimitLevel((EDeviceSoftservoWMX3LevelMethod)wmxLimit.invertNearNegativeLSPolarity))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Near negative limit level"));
					eReturn = EDeviceInitializeResult_WriteToDatabaseError;
					break;
				}

				if(SetExternNLimitLevel((EDeviceSoftservoWMX3LevelMethod)wmxLimit.invertExternalNegativeLSPolarity))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Extern negative limit level"));
					eReturn = EDeviceInitializeResult_WriteToDatabaseError;
					break;
				}

				if(SetStartSpeed(wmxMotion.globalStartingVelocity))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Start speed"));
					eReturn = EDeviceInitializeResult_WriteToDatabaseError;
					break;
				}

				if(SetEndSpeed(wmxMotion.globalEndVelocity))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("End speed"));
					eReturn = EDeviceInitializeResult_WriteToDatabaseError;
					break;
				}

				if(SetInposWidth((long)(wmxFeed.inPosWidth)))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Inpos Width"));
					eReturn = EDeviceInitializeResult_WriteToDatabaseError;
					break;
				}

				if(SetPosCompletedWidth((long)(wmxFeed.posSetWidth)))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Completed Width"));
					eReturn = EDeviceInitializeResult_WriteToDatabaseError;
					break;
				}

				m_bIsInitialized = true;
			}
			break;

		case EDeviceSoftservoWMX3InitializeType_Param:
			{
				DWORD dwDenom = 0.;
				DWORD dwNumer = 0.;

				if(GetGearRatioDenominator(&dwDenom))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Gear Denominator"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(GetGearRatioNumerator(&dwNumer))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Gear Numerator"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(m_pCMModule->config->SetGearRatio(nDeviceID, dwNumer, dwDenom) != wmx3Api::ErrorCode::None)
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Gear Ratio"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				double dblFastVel = 0., dblFastAcc = 0, dblFastDec = 0.;
				double dblSlowVel = 0., dblSlowAcc = 0, dblSlowDec = 0.;
				double dblShiftVel = 0., dblShiftAcc = 0, dblShiftDec = 0., dblShiftDist = 0.;

				EDeviceSoftservoWMX3LevelMethod eHomeLevel = EDeviceSoftservoWMX3LevelMethod_Count;

				EDeviceSoftservoWMX3HomeType eHomeType = EDeviceSoftservoWMX3HomeType_Count;

				EDeviceSoftservoWMX3HomeDirection eHomeDirection = EDeviceSoftservoWMX3HomeDirection_Count;

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

				wmxHome.invertHSPolarity = eHomeLevel;

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

				wmxHome.homeType = (wmx3Api::Config::HomeType::T)eHomeType;
				wmxHome.homeDirection = (wmx3Api::Config::HomeDirection::T)eHomeDirection;

				if(m_pCMModule->config->SetHomeParam(nDeviceID, &wmxHome) != wmx3Api::ErrorCode::None)
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("home param"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				EDeviceSoftservoWMX3LevelMethod ePosLimitLevel = EDeviceSoftservoWMX3LevelMethod_Count;
				EDeviceSoftservoWMX3LevelMethod eNearPosLimitLevel = EDeviceSoftservoWMX3LevelMethod_Count;
				EDeviceSoftservoWMX3LevelMethod eExternPosLimitLevel = EDeviceSoftservoWMX3LevelMethod_Count;

				EDeviceSoftservoWMX3LevelMethod eNegLimitLevel = EDeviceSoftservoWMX3LevelMethod_Count;
				EDeviceSoftservoWMX3LevelMethod eNearNegLimitLevel = EDeviceSoftservoWMX3LevelMethod_Count;
				EDeviceSoftservoWMX3LevelMethod eExternNegLimitLevel = EDeviceSoftservoWMX3LevelMethod_Count;

				if(GetPLimitLevel(&ePosLimitLevel))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Positive limit level"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(GetNearPLimitLevel(&eNearPosLimitLevel))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Near positive limit level"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(GetExternPLimitLevel(&eExternPosLimitLevel))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Extern positive limit level"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(GetNLimitLevel(&eNegLimitLevel))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Negative limit level"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(GetNearNLimitLevel(&eNearNegLimitLevel))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Near negative limit level"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(GetExternNLimitLevel(&eExternNegLimitLevel))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Extern negative limit level"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				wmxLimit.invertPositiveLSPolarity = (bool)ePosLimitLevel;
				wmxLimit.invertNegativeLSPolarity = (bool)eNegLimitLevel;

				wmxLimit.invertNearNegativeLSPolarity = (bool)eNearNegLimitLevel;
				wmxLimit.invertExternalNegativeLSPolarity = (bool)eExternNegLimitLevel;

				wmxLimit.invertNearPositiveLSPolarity = (bool)eNearPosLimitLevel;
				wmxLimit.invertExternalPositiveLSPolarity = (bool)eExternPosLimitLevel;

				if(m_pCMModule->config->SetLimitParam(nDeviceID, &wmxLimit) != wmx3Api::ErrorCode::None)
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Limit param"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				double dblStartSpeed = 0., dblEndSpeed = 0.;

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

				wmxMotion.globalStartingVelocity = dblStartSpeed;
				wmxMotion.globalEndVelocity = dblEndSpeed;

				if(m_pCMModule->config->SetMotionParam(nDeviceID, &wmxMotion) != wmx3Api::ErrorCode::None)
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Motion param"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				long nInposWidth = 0., nSetWidth = 0.;

				if(GetInposWidth(&nInposWidth))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Inpos Width"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(GetPosCompletedWidth(&nSetWidth))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Completed Width"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				wmxFeed.inPosWidth = nInposWidth;
				wmxFeed.posSetWidth = nSetWidth;

				if(m_pCMModule->config->SetFeedbackParam(nDeviceID, &wmxFeed) != wmx3Api::ErrorCode::None)
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Feedback param"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				m_bIsInitialized = true;
			}
			break;
		}

		if(m_bIsInitialized)
		{
			strStatus = CMultiLanguageManager::GetString(ELanguageParameter_Initialized);

			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_TheDevicehasbeensuccessfullyinitialized);

			eReturn = EDeviceInitializeResult_OK;

			CEventHandlerManager::BroadcastOnDeviceInitialized(this);
		}
	}
	while(false);

	if(!IsInitialized())
		Terminate();

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	SetStatus(strStatus);

	return eReturn;
}

EDeviceTerminateResult CDeviceSoftservoWMX3::Terminate()
{
	EDeviceTerminateResult eReturn = EDeviceTerminateResult_UnknownError;

	CString strMessage;

	SetStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("SoftservoWMX3"));

		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);

		return EDeviceTerminateResult_NotFoundApiError;
	}

	do
	{
		if(!m_pLibModule)
			break;

		int nDeviceID = _ttoi(GetDeviceID());

		size_t szDeviceCnt = CDeviceManager::GetDeviceCount();

		bool bNeedTerminate = true;

		for(size_t i = 0; i < szDeviceCnt; ++i)
		{
			CDeviceSoftservoWMX3* pSoftServo = dynamic_cast<CDeviceSoftservoWMX3*>(CDeviceManager::GetDeviceByIndex(i));

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
			m_pCMModule = pSoftServo->GetCoreMotion();

			break;
		}

		if(GetServoStatus())
			SetServoOn(false);

		if(bNeedTerminate)
		{
			wmx3Api::EngineStatus wmxStatus;
			m_pLibModule->GetEngineStatus(&wmxStatus);

			if(wmxStatus.state == wmx3Api::EngineState::Communicating)
				m_pLibModule->StopCommunication();

			m_pLibModule->CloseDevice();

			if(m_pCMModule)
			{
				delete m_pCMModule;
				m_pCMModule = nullptr;
			}

			if(m_pLibModule)
			{
				delete m_pLibModule;
				m_pLibModule = nullptr;
			}
		}
		else
		{
			m_pCMModule = nullptr;
			m_pLibModule = nullptr;
		}

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

bool CDeviceSoftservoWMX3::LoadSettings()
{
	bool bReturn = false;

	m_vctParameterFieldConfigurations.clear();

	do
	{
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX3_DeviceID, g_lpszParamSofservoWMX3[EDeviceParameterSoftservoWMX3_DeviceID], _T("0"), EParameterFieldType_Edit, nullptr, _T("axis number"));
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX3_InitializeType, g_lpszParamSofservoWMX3[EDeviceParameterSoftservoWMX3_InitializeType], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszSoftservoWMX3InitializeType, EDeviceSoftservoWMX3InitializeType_Count), _T("Choose Paramter"));

		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX3_MotionInfoPath, g_lpszParamSofservoWMX3[EDeviceParameterSoftservoWMX3_MotionInfoPath], _T("0"), EParameterFieldType_Edit, nullptr, _T("The full path of the directory that contains the motion engines"));
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX3_ParamPath, g_lpszParamSofservoWMX3[EDeviceParameterSoftservoWMX3_ParamPath], _T("0"), EParameterFieldType_Edit, nullptr, _T("The full path of the directory that contains the motion Parameters"));

		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX3_StartSpeed, g_lpszParamSofservoWMX3[EDeviceParameterSoftservoWMX3_StartSpeed], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX3_EndSpeed, g_lpszParamSofservoWMX3[EDeviceParameterSoftservoWMX3_EndSpeed], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX3_Level, g_lpszParamSofservoWMX3[EDeviceParameterSoftservoWMX3_Level], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX3_HomeLevel, g_lpszParamSofservoWMX3[EDeviceParameterSoftservoWMX3_HomeLevel], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszSoftservoWMX3LevelMethod, EDeviceSoftservoWMX3LevelMethod_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX3_PosLimitLevel, g_lpszParamSofservoWMX3[EDeviceParameterSoftservoWMX3_PosLimitLevel], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszSoftservoWMX3LevelMethod, EDeviceSoftservoWMX3LevelMethod_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX3_NearPosLimitLevel, g_lpszParamSofservoWMX3[EDeviceParameterSoftservoWMX3_NearPosLimitLevel], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszSoftservoWMX3LevelMethod, EDeviceSoftservoWMX3LevelMethod_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX3_ExternPosLimitLevel, g_lpszParamSofservoWMX3[EDeviceParameterSoftservoWMX3_ExternPosLimitLevel], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszSoftservoWMX3LevelMethod, EDeviceSoftservoWMX3LevelMethod_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX3_NegLimitLevel, g_lpszParamSofservoWMX3[EDeviceParameterSoftservoWMX3_NegLimitLevel], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszSoftservoWMX3LevelMethod, EDeviceSoftservoWMX3LevelMethod_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX3_NearNegLimitLevel, g_lpszParamSofservoWMX3[EDeviceParameterSoftservoWMX3_NearNegLimitLevel], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszSoftservoWMX3LevelMethod, EDeviceSoftservoWMX3LevelMethod_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX3_ExternNegLimitLevel, g_lpszParamSofservoWMX3[EDeviceParameterSoftservoWMX3_ExternNegLimitLevel], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszSoftservoWMX3LevelMethod, EDeviceSoftservoWMX3LevelMethod_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX3_GearRatioDenominator, g_lpszParamSofservoWMX3[EDeviceParameterSoftservoWMX3_GearRatioDenominator], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX3_GearRatioNumerator, g_lpszParamSofservoWMX3[EDeviceParameterSoftservoWMX3_GearRatioNumerator], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX3_InposWidth, g_lpszParamSofservoWMX3[EDeviceParameterSoftservoWMX3_InposWidth], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX3_PosCompletedWidth, g_lpszParamSofservoWMX3[EDeviceParameterSoftservoWMX3_PosCompletedWidth], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 0);


		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX3_Home, g_lpszParamSofservoWMX3[EDeviceParameterSoftservoWMX3_Home], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX3_HomeType, g_lpszParamSofservoWMX3[EDeviceParameterSoftservoWMX3_HomeType], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszSoftservoWMX3HomeType, EDeviceSoftservoWMX3HomeType_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX3_HomeDirection, g_lpszParamSofservoWMX3[EDeviceParameterSoftservoWMX3_HomeDirection], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszSoftservoWMX3HomeDirection, EDeviceSoftservoWMX3HomeDirection_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX3_HomeFastVelocity, g_lpszParamSofservoWMX3[EDeviceParameterSoftservoWMX3_HomeFastVelocity], _T("0.000000"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX3_HomeFastAccel, g_lpszParamSofservoWMX3[EDeviceParameterSoftservoWMX3_HomeFastAccel], _T("0.000000"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX3_HomeFastDeceleration, g_lpszParamSofservoWMX3[EDeviceParameterSoftservoWMX3_HomeFastDeceleration], _T("0.000000"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX3_HomeSlowVelocity, g_lpszParamSofservoWMX3[EDeviceParameterSoftservoWMX3_HomeSlowVelocity], _T("0.000000"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX3_HomeSlowAccel, g_lpszParamSofservoWMX3[EDeviceParameterSoftservoWMX3_HomeSlowAccel], _T("0.000000"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX3_HomeSlowDeceleration, g_lpszParamSofservoWMX3[EDeviceParameterSoftservoWMX3_HomeSlowDeceleration], _T("0.000000"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX3_HomeShiftVelocity, g_lpszParamSofservoWMX3[EDeviceParameterSoftservoWMX3_HomeShiftVelocity], _T("0.000000"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX3_HomeShiftAccel, g_lpszParamSofservoWMX3[EDeviceParameterSoftservoWMX3_HomeShiftAccel], _T("0.000000"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX3_HomeShiftDeceleration, g_lpszParamSofservoWMX3[EDeviceParameterSoftservoWMX3_HomeShiftDeceleration], _T("0.000000"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterSoftservoWMX3_HomeShiftDistance, g_lpszParamSofservoWMX3[EDeviceParameterSoftservoWMX3_HomeShiftDistance], _T("0.000000"), EParameterFieldType_Edit, nullptr, nullptr, 1);

		bReturn = true;
	}
	while(false);

	return bReturn & __super::LoadSettings();
}

ESoftservoGetFunction CDeviceSoftservoWMX3::GetInitializetype(EDeviceSoftservoWMX3InitializeType * pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceSoftservoWMX3InitializeType)_ttoi(GetParamValue(EDeviceParameterSoftservoWMX3_InitializeType));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX3::SetInitializetype(EDeviceSoftservoWMX3InitializeType eParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX3 eSaveID = EDeviceParameterSoftservoWMX3_InitializeType;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceSoftservoWMX3InitializeType_Count)
		{
			eReturn = ESoftservoSetFunction_NotFoundCommandError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX3[eSaveID], g_lpszSoftservoWMX3InitializeType[nPreValue], g_lpszSoftservoWMX3InitializeType[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

wmx3Api::WMX3Api* CDeviceSoftservoWMX3::GetDevice()
{
	return m_pLibModule;
}

wmx3Api::CoreMotion* CDeviceSoftservoWMX3::GetCoreMotion()
{
	return m_pCMModule;
}

ESoftservoGetFunction CDeviceSoftservoWMX3::GetStartSpeed(double* pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterSoftservoWMX3_StartSpeed));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX3::SetStartSpeed(double dblParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX3 eSaveID = EDeviceParameterSoftservoWMX3_StartSpeed;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			int nDeviceID = _ttoi(GetDeviceID());

			wmx3Api::Config::MotionParam wmxMotion;

			if(m_pCMModule->config->GetMotionParam(nDeviceID, &wmxMotion) != wmx3Api::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_ReadOnDeviceError;
				break;
			}

			wmxMotion.globalStartingVelocity = dblParam;

			if(m_pCMModule->config->SetMotionParam(nDeviceID, &wmxMotion) != wmx3Api::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX3[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX3::GetEndSpeed(double* pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterSoftservoWMX3_EndSpeed));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX3::SetEndSpeed(double dblParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX3 eSaveID = EDeviceParameterSoftservoWMX3_EndSpeed;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			int nDeviceID = _ttoi(GetDeviceID());

			wmx3Api::Config::MotionParam wmxMotion;

			if(m_pCMModule->config->GetMotionParam(nDeviceID, &wmxMotion) != wmx3Api::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_ReadOnDeviceError;
				break;
			}

			wmxMotion.globalEndVelocity = dblParam;

			if(m_pCMModule->config->SetMotionParam(nDeviceID, &wmxMotion) != wmx3Api::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX3[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX3::GetHomeLevel(EDeviceSoftservoWMX3LevelMethod* pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceSoftservoWMX3LevelMethod)_ttoi(GetParamValue(EDeviceParameterSoftservoWMX3_HomeLevel));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX3::SetHomeLevel(EDeviceSoftservoWMX3LevelMethod eParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX3 eSaveID = EDeviceParameterSoftservoWMX3_HomeLevel;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceSoftservoWMX3LevelMethod_Count)
		{
			eReturn = ESoftservoSetFunction_NotFoundCommandError;
			break;
		}

		if(IsInitialized())
		{
			int nDeviceID = _ttoi(GetDeviceID());

			wmx3Api::Config::HomeParam wmxHome;

			if(m_pCMModule->config->GetHomeParam(nDeviceID, &wmxHome) != wmx3Api::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_ReadOnDeviceError;
				break;
			}

			wmxHome.invertHSPolarity = (bool)eParam;

			if(m_pCMModule->config->SetHomeParam(nDeviceID, &wmxHome) != wmx3Api::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX3[eSaveID], g_lpszSoftservoWMX3LevelMethod[nPreValue], g_lpszSoftservoWMX3LevelMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX3::GetPLimitLevel(EDeviceSoftservoWMX3LevelMethod* pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceSoftservoWMX3LevelMethod)_ttoi(GetParamValue(EDeviceParameterSoftservoWMX3_PosLimitLevel));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX3::SetPLimitLevel(EDeviceSoftservoWMX3LevelMethod eParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX3 eSaveID = EDeviceParameterSoftservoWMX3_PosLimitLevel;

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

			wmx3Api::Config::LimitParam wmxLimit;

			if(m_pCMModule->config->GetLimitParam(nDeviceID, &wmxLimit) != wmx3Api::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_ReadOnDeviceError;
				break;
			}

			wmxLimit.invertPositiveLSPolarity = eParam;

			if(m_pCMModule->config->SetLimitParam(nDeviceID, &wmxLimit) != wmx3Api::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX3[eSaveID], g_lpszSoftservoWMX3LevelMethod[nPreValue], g_lpszSoftservoWMX3LevelMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX3::GetNearPLimitLevel(EDeviceSoftservoWMX3LevelMethod * pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceSoftservoWMX3LevelMethod)_ttoi(GetParamValue(EDeviceParameterSoftservoWMX3_NearPosLimitLevel));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX3::SetNearPLimitLevel(EDeviceSoftservoWMX3LevelMethod eParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX3 eSaveID = EDeviceParameterSoftservoWMX3_NearPosLimitLevel;

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

			wmx3Api::Config::LimitParam wmxLimit;

			if(m_pCMModule->config->GetLimitParam(nDeviceID, &wmxLimit) != wmx3Api::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_ReadOnDeviceError;
				break;
			}

			wmxLimit.invertNearPositiveLSPolarity = eParam;

			if(m_pCMModule->config->SetLimitParam(nDeviceID, &wmxLimit) != wmx3Api::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX3[eSaveID], g_lpszSoftservoWMX3LevelMethod[nPreValue], g_lpszSoftservoWMX3LevelMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX3::GetExternPLimitLevel(EDeviceSoftservoWMX3LevelMethod * pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceSoftservoWMX3LevelMethod)_ttoi(GetParamValue(EDeviceParameterSoftservoWMX3_ExternPosLimitLevel));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX3::SetExternPLimitLevel(EDeviceSoftservoWMX3LevelMethod eParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX3 eSaveID = EDeviceParameterSoftservoWMX3_ExternPosLimitLevel;

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

			wmx3Api::Config::LimitParam wmxLimit;

			if(m_pCMModule->config->GetLimitParam(nDeviceID, &wmxLimit) != wmx3Api::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_ReadOnDeviceError;
				break;
			}

			wmxLimit.invertExternalPositiveLSPolarity = eParam;

			if(m_pCMModule->config->SetLimitParam(nDeviceID, &wmxLimit) != wmx3Api::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX3[eSaveID], g_lpszSoftservoWMX3LevelMethod[nPreValue], g_lpszSoftservoWMX3LevelMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX3::GetNLimitLevel(EDeviceSoftservoWMX3LevelMethod* pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceSoftservoWMX3LevelMethod)_ttoi(GetParamValue(EDeviceParameterSoftservoWMX3_NegLimitLevel));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX3::SetNLimitLevel(EDeviceSoftservoWMX3LevelMethod eParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX3 eSaveID = EDeviceParameterSoftservoWMX3_NegLimitLevel;

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

			wmx3Api::Config::LimitParam wmxLimit;

			if(m_pCMModule->config->GetLimitParam(nDeviceID, &wmxLimit) != wmx3Api::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_ReadOnDeviceError;
				break;
			}

			wmxLimit.invertNegativeLSPolarity = eParam;

			if(m_pCMModule->config->SetLimitParam(nDeviceID, &wmxLimit) != wmx3Api::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX3[eSaveID], g_lpszSoftservoWMX3LevelMethod[nPreValue], g_lpszSoftservoWMX3LevelMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX3::GetNearNLimitLevel(EDeviceSoftservoWMX3LevelMethod * pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceSoftservoWMX3LevelMethod)_ttoi(GetParamValue(EDeviceParameterSoftservoWMX3_NearNegLimitLevel));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX3::SetNearNLimitLevel(EDeviceSoftservoWMX3LevelMethod eParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX3 eSaveID = EDeviceParameterSoftservoWMX3_NearNegLimitLevel;

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

			wmx3Api::Config::LimitParam wmxLimit;

			if(m_pCMModule->config->GetLimitParam(nDeviceID, &wmxLimit) != wmx3Api::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_ReadOnDeviceError;
				break;
			}

			wmxLimit.invertNearNegativeLSPolarity = eParam;

			if(m_pCMModule->config->SetLimitParam(nDeviceID, &wmxLimit) != wmx3Api::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX3[eSaveID], g_lpszSoftservoWMX3LevelMethod[nPreValue], g_lpszSoftservoWMX3LevelMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX3::GetExternNLimitLevel(EDeviceSoftservoWMX3LevelMethod * pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceSoftservoWMX3LevelMethod)_ttoi(GetParamValue(EDeviceParameterSoftservoWMX3_ExternNegLimitLevel));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX3::SetExternNLimitLevel(EDeviceSoftservoWMX3LevelMethod eParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX3 eSaveID = EDeviceParameterSoftservoWMX3_ExternNegLimitLevel;

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

			wmx3Api::Config::LimitParam wmxLimit;

			if(m_pCMModule->config->GetLimitParam(nDeviceID, &wmxLimit) != wmx3Api::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_ReadOnDeviceError;
				break;
			}

			wmxLimit.invertExternalNegativeLSPolarity = eParam;

			if(m_pCMModule->config->SetLimitParam(nDeviceID, &wmxLimit) != wmx3Api::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX3[eSaveID], g_lpszSoftservoWMX3LevelMethod[nPreValue], g_lpszSoftservoWMX3LevelMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX3::GetGearRatioDenominator(DWORD * pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = (DWORD)_ttoi(GetParamValue(EDeviceParameterSoftservoWMX3_GearRatioDenominator));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX3::SetGearRatioDenominator(DWORD dwParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX3 eSaveID = EDeviceParameterSoftservoWMX3_GearRatioDenominator;

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_u_to_u), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX3[eSaveID], dwPreValue, dwParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX3::GetGearRatioNumerator(DWORD * pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = (DWORD)_ttoi(GetParamValue(EDeviceParameterSoftservoWMX3_GearRatioNumerator));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX3::SetGearRatioNumerator(DWORD dwParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX3 eSaveID = EDeviceParameterSoftservoWMX3_GearRatioNumerator;

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_u_to_u), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX3[eSaveID], dwPreValue, dwParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX3::GetMotionInfoPath(CString * pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = GetParamValue(EDeviceParameterSoftservoWMX3_MotionInfoPath);

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX3::SetMotionInfoPath(CString strParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX3 eSaveID = EDeviceParameterSoftservoWMX3_MotionInfoPath;

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX3[eSaveID], strPreValue, strParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX3::GetParamPath(CString * pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = GetParamValue(EDeviceParameterSoftservoWMX3_ParamPath);

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX3::SetParamPath(CString strParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX3 eSaveID = EDeviceParameterSoftservoWMX3_ParamPath;

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX3[eSaveID], strPreValue, strParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX3::GetInposWidth(long * pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterSoftservoWMX3_InposWidth));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX3::SetInposWidth(long nParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX3 eSaveID = EDeviceParameterSoftservoWMX3_InposWidth;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			int nDeviceID = _ttoi(GetDeviceID());

			wmx3Api::Config::FeedbackParam wmxFeed;

			if(m_pCMModule->config->GetFeedbackParam(nDeviceID, &wmxFeed) != wmx3Api::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_ReadOnDeviceError;
				break;
			}

			wmxFeed.inPosWidth = nParam;

			if(m_pCMModule->config->SetFeedbackParam(nDeviceID, &wmxFeed) != wmx3Api::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX3[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX3::GetPosCompletedWidth(long * pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterSoftservoWMX3_PosCompletedWidth));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX3::SetPosCompletedWidth(long nParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX3 eSaveID = EDeviceParameterSoftservoWMX3_PosCompletedWidth;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			int nDeviceID = _ttoi(GetDeviceID());

			wmx3Api::Config::FeedbackParam wmxFeed;

			if(m_pCMModule->config->GetFeedbackParam(nDeviceID, &wmxFeed) != wmx3Api::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_ReadOnDeviceError;
				break;
			}

			wmxFeed.posSetWidth = nParam;

			if(m_pCMModule->config->SetFeedbackParam(nDeviceID, &wmxFeed) != wmx3Api::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX3[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX3::GetHomeType(EDeviceSoftservoWMX3HomeType * pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceSoftservoWMX3HomeType)_ttoi(GetParamValue(EDeviceParameterSoftservoWMX3_HomeType));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX3::SetHomeType(EDeviceSoftservoWMX3HomeType eParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX3 eSaveID = EDeviceParameterSoftservoWMX3_HomeType;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceSoftservoWMX3HomeType_Count)
		{
			eReturn = ESoftservoSetFunction_NotFoundCommandError;
			break;
		}

		if(IsInitialized())
		{
			int nDeviceID = _ttoi(GetDeviceID());

			wmx3Api::Config::HomeParam wmxHome;

			if(m_pCMModule->config->GetHomeParam(nDeviceID, &wmxHome) != wmx3Api::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_ReadOnDeviceError;
				break;
			}

			wmxHome.homeType = (wmx3Api::Config::HomeType::T)eParam;

			if(m_pCMModule->config->SetHomeParam(nDeviceID, &wmxHome) != wmx3Api::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX3[eSaveID], g_lpszSoftservoWMX3HomeType[nPreValue], g_lpszSoftservoWMX3HomeType[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX3::GetHomeDirection(EDeviceSoftservoWMX3HomeDirection * pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceSoftservoWMX3HomeDirection)_ttoi(GetParamValue(EDeviceParameterSoftservoWMX3_HomeDirection));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX3::SetHomeDirection(EDeviceSoftservoWMX3HomeDirection eParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX3 eSaveID = EDeviceParameterSoftservoWMX3_HomeDirection;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceSoftservoWMX3HomeType_Count)
		{
			eReturn = ESoftservoSetFunction_NotFoundCommandError;
			break;
		}

		if(IsInitialized())
		{
			int nDeviceID = _ttoi(GetDeviceID());

			wmx3Api::Config::HomeParam wmxHome;

			if(m_pCMModule->config->GetHomeParam(nDeviceID, &wmxHome) != wmx3Api::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_ReadOnDeviceError;
				break;
			}

			wmxHome.homeDirection = (wmx3Api::Config::HomeDirection::T)eParam;

			if(m_pCMModule->config->SetHomeParam(nDeviceID, &wmxHome) != wmx3Api::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX3[eSaveID], g_lpszSoftservoWMX3HomeDirection[nPreValue], g_lpszSoftservoWMX3HomeDirection[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX3::GetHomeFastVelocity(double * pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterSoftservoWMX3_HomeFastVelocity));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX3::SetHomeFastVelocity(double dblParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX3 eSaveID = EDeviceParameterSoftservoWMX3_HomeFastVelocity;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			int nDeviceID = _ttoi(GetDeviceID());

			wmx3Api::Config::HomeParam wmxHome;

			if(m_pCMModule->config->GetHomeParam(nDeviceID, &wmxHome) != wmx3Api::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_ReadOnDeviceError;
				break;
			}

			wmxHome.homingVelocityFast = dblParam;

			if(m_pCMModule->config->SetHomeParam(nDeviceID, &wmxHome) != wmx3Api::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX3[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX3::GetHomeFastAccel(double * pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterSoftservoWMX3_HomeFastAccel));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX3::SetHomeFastAccel(double dblParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX3 eSaveID = EDeviceParameterSoftservoWMX3_HomeFastAccel;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			int nDeviceID = _ttoi(GetDeviceID());

			wmx3Api::Config::HomeParam wmxHome;

			if(m_pCMModule->config->GetHomeParam(nDeviceID, &wmxHome) != wmx3Api::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_ReadOnDeviceError;
				break;
			}

			wmxHome.homingVelocityFastAcc = dblParam;

			if(m_pCMModule->config->SetHomeParam(nDeviceID, &wmxHome) != wmx3Api::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX3[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX3::GetHomeFastDeceleration(double * pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterSoftservoWMX3_HomeFastDeceleration));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX3::SetHomeFastDeceleration(double dblParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX3 eSaveID = EDeviceParameterSoftservoWMX3_HomeFastDeceleration;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			int nDeviceID = _ttoi(GetDeviceID());

			wmx3Api::Config::HomeParam wmxHome;

			if(m_pCMModule->config->GetHomeParam(nDeviceID, &wmxHome) != wmx3Api::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_ReadOnDeviceError;
				break;
			}

			wmxHome.homingVelocityFastDec = dblParam;

			if(m_pCMModule->config->SetHomeParam(nDeviceID, &wmxHome) != wmx3Api::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX3[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX3::GetHomeSlowVelocity(double * pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterSoftservoWMX3_HomeSlowVelocity));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX3::SetHomeSlowVelocity(double dblParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX3 eSaveID = EDeviceParameterSoftservoWMX3_HomeSlowVelocity;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			int nDeviceID = _ttoi(GetDeviceID());

			wmx3Api::Config::HomeParam wmxHome;

			if(m_pCMModule->config->GetHomeParam(nDeviceID, &wmxHome) != wmx3Api::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_ReadOnDeviceError;
				break;
			}

			wmxHome.homingVelocitySlow = dblParam;

			if(m_pCMModule->config->SetHomeParam(nDeviceID, &wmxHome) != wmx3Api::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX3[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX3::GetHomeSlowAccel(double * pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterSoftservoWMX3_HomeSlowAccel));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX3::SetHomeSlowAccel(double dblParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX3 eSaveID = EDeviceParameterSoftservoWMX3_HomeSlowAccel;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			int nDeviceID = _ttoi(GetDeviceID());

			wmx3Api::Config::HomeParam wmxHome;

			if(m_pCMModule->config->GetHomeParam(nDeviceID, &wmxHome) != wmx3Api::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_ReadOnDeviceError;
				break;
			}

			wmxHome.homingVelocitySlowAcc = dblParam;

			if(m_pCMModule->config->SetHomeParam(nDeviceID, &wmxHome) != wmx3Api::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX3[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX3::GetHomeSlowDeceleration(double * pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterSoftservoWMX3_HomeSlowDeceleration));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX3::SetHomeSlowDeceleration(double dblParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX3 eSaveID = EDeviceParameterSoftservoWMX3_HomeSlowDeceleration;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			int nDeviceID = _ttoi(GetDeviceID());

			wmx3Api::Config::HomeParam wmxHome;

			if(m_pCMModule->config->GetHomeParam(nDeviceID, &wmxHome) != wmx3Api::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_ReadOnDeviceError;
				break;
			}

			wmxHome.homingVelocitySlowDec = dblParam;

			if(m_pCMModule->config->SetHomeParam(nDeviceID, &wmxHome) != wmx3Api::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX3[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX3::GetHomeShiftVelocity(double * pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterSoftservoWMX3_HomeShiftVelocity));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX3::SetHomeShiftVelocity(double dblParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX3 eSaveID = EDeviceParameterSoftservoWMX3_HomeShiftVelocity;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			int nDeviceID = _ttoi(GetDeviceID());

			wmx3Api::Config::HomeParam wmxHome;

			if(m_pCMModule->config->GetHomeParam(nDeviceID, &wmxHome) != wmx3Api::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_ReadOnDeviceError;
				break;
			}

			wmxHome.homeShiftVelocity = dblParam;

			if(m_pCMModule->config->SetHomeParam(nDeviceID, &wmxHome) != wmx3Api::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX3[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX3::GetHomeShiftAccel(double * pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterSoftservoWMX3_HomeShiftAccel));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX3::SetHomeShiftAccel(double dblParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX3 eSaveID = EDeviceParameterSoftservoWMX3_HomeShiftAccel;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			int nDeviceID = _ttoi(GetDeviceID());

			wmx3Api::Config::HomeParam wmxHome;

			if(m_pCMModule->config->GetHomeParam(nDeviceID, &wmxHome) != wmx3Api::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_ReadOnDeviceError;
				break;
			}

			wmxHome.homeShiftAcc = dblParam;

			if(m_pCMModule->config->SetHomeParam(nDeviceID, &wmxHome) != wmx3Api::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX3[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX3::GetHomeShiftDeceleration(double * pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterSoftservoWMX3_HomeShiftDeceleration));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX3::SetHomeShiftDeceleration(double dblParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX3 eSaveID = EDeviceParameterSoftservoWMX3_HomeShiftDeceleration;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			int nDeviceID = _ttoi(GetDeviceID());

			wmx3Api::Config::HomeParam wmxHome;

			if(m_pCMModule->config->GetHomeParam(nDeviceID, &wmxHome) != wmx3Api::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_ReadOnDeviceError;
				break;
			}

			wmxHome.homeShiftDec = dblParam;

			if(m_pCMModule->config->SetHomeParam(nDeviceID, &wmxHome) != wmx3Api::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX3[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ESoftservoGetFunction CDeviceSoftservoWMX3::GetHomeShiftDistance(double * pParam)
{
	ESoftservoGetFunction eReturn = ESoftservoGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ESoftservoGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterSoftservoWMX3_HomeShiftDistance));

		eReturn = ESoftservoGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESoftservoSetFunction CDeviceSoftservoWMX3::SetHomeShiftDistance(double dblParam)
{
	ESoftservoSetFunction eReturn = ESoftservoSetFunction_UnknownError;

	EDeviceParameterSoftservoWMX3 eSaveID = EDeviceParameterSoftservoWMX3_HomeShiftDistance;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			int nDeviceID = _ttoi(GetDeviceID());

			wmx3Api::Config::HomeParam wmxHome;

			if(m_pCMModule->config->GetHomeParam(nDeviceID, &wmxHome) != wmx3Api::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_ReadOnDeviceError;
				break;
			}

			wmxHome.homeShiftDistance = dblParam;

			if(m_pCMModule->config->SetHomeParam(nDeviceID, &wmxHome) != wmx3Api::ErrorCode::None)
			{
				eReturn = ESoftservoSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamSofservoWMX3[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceSoftservoWMX3::GetServoStatus()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(!m_pCMModule)
			break;

		int nDeviceID = _ttoi(GetDeviceID());

		wmx3Api::CoreMotionStatus wmxStatus;

		if(m_pCMModule->GetStatus(&wmxStatus) != wmx3Api::ErrorCode::None)
			break;

		if(!wmxStatus.axesStatus[nDeviceID].servoOn)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceSoftservoWMX3::SetServoOn(bool bOn)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(!m_pCMModule)
			break;

		if(m_pCMModule->axisControl->SetServoOn(_ttoi(GetDeviceID()), bOn) != wmx3Api::ErrorCode::None)
			break;

		m_bServo = bOn;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceSoftservoWMX3::GetCommandPosition(double* pPos)
{
	bool bReturn = false;

	do
	{
		if(!pPos)
			break;

		if(!IsInitialized())
			break;

		if(!m_pCMModule)
			break;

		if(m_pCMModule->axisControl->GetPosCommand(_ttoi(GetDeviceID()), pPos) != wmx3Api::ErrorCode::None)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceSoftservoWMX3::SetCommandPositionClear()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(!m_pCMModule)
			break;

		if(m_pCMModule->home->SetCommandPos(_ttoi(GetDeviceID()), 0.) != wmx3Api::ErrorCode::None)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceSoftservoWMX3::GetActualPosition(double* pPos)
{
	bool bReturn = false;

	do
	{
		if(!pPos)
			break;

		if(!IsInitialized())
			break;

		if(!m_pCMModule)
			break;

		if(m_pCMModule->axisControl->GetPosFeedback(_ttoi(GetDeviceID()), pPos) != wmx3Api::ErrorCode::None)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceSoftservoWMX3::SetActualPositionClear()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(!m_pCMModule)
			break;

		if(m_pCMModule->home->SetFeedbackPos(_ttoi(GetDeviceID()), 0.) != wmx3Api::ErrorCode::None)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceSoftservoWMX3::GetAlarmStatus()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(!m_pCMModule)
			break;

		int nDeviceID = _ttoi(GetDeviceID());

		wmx3Api::CoreMotionStatus wmxStatus;

		if(m_pCMModule->GetStatus(&wmxStatus) != wmx3Api::ErrorCode::None)
			break;

		if(!wmxStatus.axesStatus[nDeviceID].ampAlarm)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceSoftservoWMX3::SetAlarmClear()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(!m_pCMModule)
			break;

		int nDeviceID = _ttoi(GetDeviceID());

		if(m_pCMModule->axisControl->ClearAxisAlarm(nDeviceID) != wmx3Api::ErrorCode::None)
			break;

		if(m_pCMModule->axisControl->ClearAmpAlarm(nDeviceID) != wmx3Api::ErrorCode::None)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceSoftservoWMX3::GetInposition()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(!m_pCMModule)
			break;

		int nDeviceID = _ttoi(GetDeviceID());

		wmx3Api::CoreMotionStatus wmxStatus;

		if(m_pCMModule->GetStatus(&wmxStatus) != wmx3Api::ErrorCode::None)
			break;

		if(!wmxStatus.axesStatus[nDeviceID].inPos)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceSoftservoWMX3::GetLimitSensorN()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(!m_pCMModule)
			break;

		int nDeviceID = _ttoi(GetDeviceID());

		wmx3Api::CoreMotionStatus wmxStatus;

		if(m_pCMModule->GetStatus(&wmxStatus) != wmx3Api::ErrorCode::None)
			break;

		if(!wmxStatus.axesStatus[nDeviceID].negativeLS)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceSoftservoWMX3::GetLimitSensorP()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(!m_pCMModule)
			break;

		int nDeviceID = _ttoi(GetDeviceID());

		wmx3Api::CoreMotionStatus wmxStatus;

		if(m_pCMModule->GetStatus(&wmxStatus) != wmx3Api::ErrorCode::None)
			break;

		if(!wmxStatus.axesStatus[nDeviceID].positiveLS)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceSoftservoWMX3::GetHomeSensor()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(!m_pCMModule)
			break;

		int nDeviceID = _ttoi(GetDeviceID());

		wmx3Api::CoreMotionStatus wmxStatus;

		if(m_pCMModule->GetStatus(&wmxStatus) != wmx3Api::ErrorCode::None)
			break;

		if(!wmxStatus.axesStatus[nDeviceID].homeSwitch)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceSoftservoWMX3::MovePosition(double dblPos, double dblVel, double dblAcc, bool bSCurve)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(!m_pCMModule)
			break;

		m_dblLastVelocity = dblAcc;

		wmx3Api::Motion::PosCommand wmxPos;
		wmxPos.axis = _ttoi(GetDeviceID());
		wmxPos.profile.type = wmx3Api::ProfileType::Trapezoidal;
		wmxPos.profile.velocity = dblVel;
		wmxPos.profile.acc = dblAcc;
		wmxPos.profile.dec = dblAcc;
		wmxPos.profile.jerkAcc = 1.;
		wmxPos.profile.jerkDec = 1.;
		wmxPos.profile.startingVelocity = 0.;
		wmxPos.profile.endVelocity = 0.;
		wmxPos.target = dblPos;

		if(m_pCMModule->motion->StartPos(&wmxPos) != wmx3Api::ErrorCode::None)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceSoftservoWMX3::MoveDistance(double dblDist, double dblVel, double dblAcc, bool bSCurve)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(!m_pCMModule)
			break;

		m_dblLastVelocity = dblAcc;

		wmx3Api::Motion::PosCommand wmxPos;
		wmxPos.axis = _ttoi(GetDeviceID());
		wmxPos.profile.type = wmx3Api::ProfileType::Trapezoidal;
		wmxPos.profile.velocity = dblVel;
		wmxPos.profile.acc = dblAcc;
		wmxPos.profile.dec = dblAcc;
		wmxPos.profile.jerkAcc = 1.;
		wmxPos.profile.jerkDec = 1.;
		wmxPos.profile.startingVelocity = 0.;
		wmxPos.profile.endVelocity = 0.;
		wmxPos.target = dblDist;

		if(m_pCMModule->motion->StartMov(&wmxPos) != wmx3Api::ErrorCode::None)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceSoftservoWMX3::MoveVelocity(double dblVel, double dblAcc, bool bSCurve)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(!m_pCMModule)
			break;

		m_dblLastVelocity = dblAcc;

		wmx3Api::Motion::JogCommand wmxJog;
		wmxJog.axis = _ttoi(GetDeviceID());
		wmxJog.profile.type = wmx3Api::ProfileType::Trapezoidal;
		wmxJog.profile.velocity = dblVel;
		wmxJog.profile.acc = dblAcc;
		wmxJog.profile.dec = dblAcc;

		if(m_pCMModule->motion->StartJog(&wmxJog) != wmx3Api::ErrorCode::None)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceSoftservoWMX3::MoveJog(double dblVel, double dblAcc, BOOL bSCurve)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(!m_pCMModule)
			break;

		m_dblLastVelocity = dblAcc;

		wmx3Api::Motion::JogCommand wmxJog;
		wmxJog.axis = _ttoi(GetDeviceID());
		wmxJog.profile.type = wmx3Api::ProfileType::Trapezoidal;
		wmxJog.profile.velocity = dblVel;
		wmxJog.profile.acc = dblAcc;
		wmxJog.profile.dec = dblAcc;

		if(m_pCMModule->motion->StartJog(&wmxJog) != wmx3Api::ErrorCode::None)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceSoftservoWMX3::StopJog()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(!m_pCMModule)
			break;

		int nDeviceID = _ttoi(GetDeviceID());

		wmx3Api::Config::MotionParam wmxMotion;

		if(m_pCMModule->config->GetMotionParam(nDeviceID, &wmxMotion) != wmx3Api::ErrorCode::None)
			break;

		wmxMotion.quickStopDec = m_dblLastVelocity;

		if(m_pCMModule->config->SetMotionParam(nDeviceID, &wmxMotion) != wmx3Api::ErrorCode::None)
			break;

		if(m_pCMModule->motion->Stop(nDeviceID) != wmx3Api::ErrorCode::None)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceSoftservoWMX3::IsMotionDone()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(!m_pCMModule)
			break;

		int nDeviceID = _ttoi(GetDeviceID());

		wmx3Api::CoreMotionStatus wmxStatus;

		if(m_pCMModule->GetStatus(&wmxStatus) != wmx3Api::ErrorCode::None)
			break;

		if(wmxStatus.axesStatus[nDeviceID].opState != wmx3Api::OperationState::Idle)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceSoftservoWMX3::MotorStop()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(!m_pCMModule)
			break;

		int nDeviceID = _ttoi(GetDeviceID());

		wmx3Api::Config::MotionParam wmxMotion;

		if(m_pCMModule->config->GetMotionParam(nDeviceID, &wmxMotion) != wmx3Api::ErrorCode::None)
			break;

		wmxMotion.quickStopDec = m_dblLastVelocity;

		if(m_pCMModule->config->SetMotionParam(nDeviceID, &wmxMotion) != wmx3Api::ErrorCode::None)
			break;

		if(m_pCMModule->motion->Stop(nDeviceID) != wmx3Api::ErrorCode::None)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceSoftservoWMX3::MotorEStop()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(!m_pCMModule)
			break;

		if(m_pCMModule->ExecEStop(wmx3Api::EStopLevel::Level1) != wmx3Api::ErrorCode::None)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceSoftservoWMX3::ChangeSpeed(double dSpeed)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(!m_pCMModule)
			break;

		if(m_pCMModule->axisControl->GetVelCommand(_ttoi(GetDeviceID()), &dSpeed) != wmx3Api::ErrorCode::None)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceSoftservoWMX3::WriteGenOutBit(int nBit, bool bOn)
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

bool CDeviceSoftservoWMX3::ReadGenOutBit(int nBit)
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

bool CDeviceSoftservoWMX3::ReadGenInputBit(int nBit)
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

bool CDeviceSoftservoWMX3::MoveToHome()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(!m_pCMModule)
			break;

		if(m_pCMModule->home->StartHome(_ttoi(GetDeviceID())) != wmx3Api::ErrorCode::None)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

EDeviceMotionHommingStatus CDeviceSoftservoWMX3::GetHommingStatus()
{
	EDeviceMotionHommingStatus eStatus = EDeviceMotionHommingStatus_Error;

	do
	{
		if(!IsInitialized())
			break;

		if(!m_pCMModule)
			break;

		int nDeviceID = _ttoi(GetDeviceID());

		wmx3Api::CoreMotionStatus wmxStatus;

		if(m_pCMModule->GetStatus(&wmxStatus) != wmx3Api::ErrorCode::None)
			break;

		if(wmxStatus.axesStatus[nDeviceID].homeDone == 1)
			eStatus = EDeviceMotionHommingStatus_Success;
		else
		{
			if(wmxStatus.axesStatus[nDeviceID].homeError == wmx3Api::HomeError::NoError)
				eStatus = EDeviceMotionHommingStatus_Searching;
		}
	}
	while(false);

	return eStatus;
}

bool CDeviceSoftservoWMX3::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = true;

	bool bFoundID = true;

	CString strMessage;

	do
	{
		switch(nParam)
		{
		case EDeviceParameterSoftservoWMX3_DeviceID:
			bReturn = !SetDeviceID(strValue);
			break;
		case EDeviceParameterSoftservoWMX3_MotionInfoPath:
			bReturn = !SetMotionInfoPath(strValue);
			break;
		case EDeviceParameterSoftservoWMX3_ParamPath:
			bReturn = !SetParamPath(strValue);
			break;
		case EDeviceParameterSoftservoWMX3_StartSpeed:
			bReturn = !SetStartSpeed(_ttof(strValue));
			break;
		case EDeviceParameterSoftservoWMX3_EndSpeed:
			bReturn = !SetEndSpeed(_ttof(strValue));
			break;
		case EDeviceParameterSoftservoWMX3_HomeLevel:
			bReturn = !SetHomeLevel((EDeviceSoftservoWMX3LevelMethod)_ttoi(strValue));
			break;
		case EDeviceParameterSoftservoWMX3_PosLimitLevel:
			bReturn = !SetPLimitLevel((EDeviceSoftservoWMX3LevelMethod)_ttoi(strValue));
			break;
		case EDeviceParameterSoftservoWMX3_NearPosLimitLevel:
			bReturn = !SetNearPLimitLevel((EDeviceSoftservoWMX3LevelMethod)_ttoi(strValue));
			break;
		case EDeviceParameterSoftservoWMX3_ExternPosLimitLevel:
			bReturn = !SetExternPLimitLevel((EDeviceSoftservoWMX3LevelMethod)_ttoi(strValue));
			break;
		case EDeviceParameterSoftservoWMX3_NegLimitLevel:
			bReturn = !SetNLimitLevel((EDeviceSoftservoWMX3LevelMethod)_ttoi(strValue));
			break;
		case EDeviceParameterSoftservoWMX3_NearNegLimitLevel:
			bReturn = !SetNearNLimitLevel((EDeviceSoftservoWMX3LevelMethod)_ttoi(strValue));
			break;
		case EDeviceParameterSoftservoWMX3_ExternNegLimitLevel:
			bReturn = !SetExternNLimitLevel((EDeviceSoftservoWMX3LevelMethod)_ttoi(strValue));
			break;
		case EDeviceParameterSoftservoWMX3_GearRatioDenominator:
			bReturn = !SetGearRatioDenominator((DWORD)_ttoi(strValue));
			break;
		case EDeviceParameterSoftservoWMX3_GearRatioNumerator:
			bReturn = !SetGearRatioNumerator((DWORD)_ttoi(strValue));
			break;
		case EDeviceParameterSoftservoWMX3_InposWidth:
			bReturn = !SetInposWidth(_ttoi(strValue));
			break;
		case EDeviceParameterSoftservoWMX3_PosCompletedWidth:
			bReturn = !SetPosCompletedWidth(_ttoi(strValue));
			break;
		case EDeviceParameterSoftservoWMX3_HomeType:
			bReturn = !SetHomeType((EDeviceSoftservoWMX3HomeType)_ttoi(strValue));
			break;
		case EDeviceParameterSoftservoWMX3_HomeDirection:
			bReturn = !SetHomeDirection((EDeviceSoftservoWMX3HomeDirection)_ttoi(strValue));
			break;
		case EDeviceParameterSoftservoWMX3_HomeFastVelocity:
			bReturn = !SetHomeFastVelocity(_ttof(strValue));
			break;
		case EDeviceParameterSoftservoWMX3_HomeFastAccel:
			bReturn = !SetHomeFastAccel(_ttof(strValue));
			break;
		case EDeviceParameterSoftservoWMX3_HomeFastDeceleration:
			bReturn = !SetHomeFastDeceleration(_ttof(strValue));
			break;
		case EDeviceParameterSoftservoWMX3_HomeSlowVelocity:
			bReturn = !SetHomeSlowVelocity(_ttof(strValue));
			break;
		case EDeviceParameterSoftservoWMX3_HomeSlowAccel:
			bReturn = !SetHomeSlowAccel(_ttof(strValue));
			break;
		case EDeviceParameterSoftservoWMX3_HomeSlowDeceleration:
			bReturn = !SetHomeSlowDeceleration(_ttof(strValue));
			break;
		case EDeviceParameterSoftservoWMX3_HomeShiftVelocity:
			bReturn = !SetHomeShiftVelocity(_ttof(strValue));
			break;
		case EDeviceParameterSoftservoWMX3_HomeShiftAccel:
			bReturn = !SetHomeShiftAccel(_ttof(strValue));
			break;
		case EDeviceParameterSoftservoWMX3_HomeShiftDeceleration:
			bReturn = !SetHomeShiftDeceleration(_ttof(strValue));
			break;
		case EDeviceParameterSoftservoWMX3_HomeShiftDistance:
			bReturn = !SetHomeShiftDistance(_ttof(strValue));
			break;
		default:
			bFoundID = false;
			break;
		}

		if(bReturn)
		{
			if(m_pLibModule)
			{
				CString strParam;
				if(ESoftservoGetFunction_OK != GetParamPath(&strParam))
				{
					strMessage = _T("Failed to read parampath.");
					bReturn = false;
					break;
				}

				if(strParam.IsEmpty() || (strParam.Find(_T(".xml")) == -1))
				{
					strMessage = _T("Parampath is invaild.");
					bReturn = false;
					break;
				}

				int nError = m_pCMModule->config->GetAndExportAll(strParam.GetBuffer());
				if(nError != wmx3Api::ErrorCode::None)
				{
					char errString[256];
					m_pLibModule->ErrorToString(nError, errString, sizeof(errString));
					strMessage = (LPSTR)errString;
					bReturn = false;
					break;
				}
			}
		}
	}
	while(false);

	if(!bFoundID)
	{
		CString strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtofindtheparameter);

		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);
	}

	if(!bReturn)
	{
		CString strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtofindtheparameter);

		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);
	}

	return bReturn;
}

bool CDeviceSoftservoWMX3::GetOnlineAxisCount(int * pAxisCount)
{
	bool bReturn = false;

	do
	{
		if(!pAxisCount)
			break;

		wmx3Api::ecApi::Ecat ecLib(m_pLibModule);
		wmx3Api::ecApi::EcMasterInfo sMI;

		if(ecLib.ScanNetwork())
			break;

		if(ecLib.GetMasterInfo(&sMI))
			break;

		if((int)sMI.numOfSlaves < 0)
			break;

		*pAxisCount = (int)(sMI.numOfSlaves + 1); // + 1 for master module;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartEngine(char * path, unsigned int waitTimeMilliseconds, int core, DWORD_PTR affinityMask)
{
	return m_pLibModule->StartEngine(path, waitTimeMilliseconds, core, affinityMask);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartEngine(wchar_t * path, unsigned int waitTimeMilliseconds, int core, DWORD_PTR affinityMask)
{
	return m_pLibModule->StartEngine(path, waitTimeMilliseconds, core, affinityMask);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__RestartEngine(char * path, unsigned int waitTimeMilliseconds, int core, DWORD_PTR affinityMask)
{
	return m_pLibModule->RestartEngine(path, waitTimeMilliseconds, core, affinityMask);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__RestartEngine(wchar_t * path, unsigned int waitTimeMilliseconds, int core, DWORD_PTR affinityMask)
{
	return m_pLibModule->RestartEngine(path, waitTimeMilliseconds, core, affinityMask);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StopEngine(unsigned int waitTimeMilliseconds)
{
	return m_pLibModule->StopEngine(waitTimeMilliseconds);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__CreateDevice(char * path, __DeviceType::T type, unsigned int waitTimeMilliseconds, int core, DWORD_PTR affinityMask)
{
	return m_pLibModule->CreateDevice(path, (wmx3Api::DeviceType::T)type, waitTimeMilliseconds, core, affinityMask);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__CreateDevice(wchar_t * path, __DeviceType::T type, unsigned int waitTimeMilliseconds, int core, DWORD_PTR affinityMask)
{
	return m_pLibModule->CreateDevice(path, (wmx3Api::DeviceType::T)type, waitTimeMilliseconds, core, affinityMask);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__CloseDevice()
{
	return m_pLibModule->CloseDevice();
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetDeviceID(int * id)
{
	return m_pLibModule->GetDeviceID(id);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__AutoQuitWithoutActiveDev(bool quit)
{
	return m_pLibModule->AutoQuitWithoutActiveDev(quit);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SetWatchdog(unsigned int watchdog)
{
	return m_pLibModule->SetWatchdog(watchdog);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetWatchdog(unsigned int * watchdog, unsigned int * watchdogCount)
{
	return m_pLibModule->GetWatchdog(watchdog, watchdogCount);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__ResetWatchdogTimer()
{
	return m_pLibModule->ResetWatchdogTimer();
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SetStatistic(bool enable)
{
	return m_pLibModule->SetStatistic(enable);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetStatistic(__DeviceStatistics *statistics)
{
	return m_pLibModule->GetStatistic((wmx3Api::DeviceStatistics*)statistics);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SetInterruptId(unsigned int interruptId)
{
	return m_pLibModule->SetInterruptId(interruptId);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetInterruptId(unsigned int * interruptId)
{
	return m_pLibModule->GetInterruptId(interruptId);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartCommunication(unsigned int waitTimeMilliseconds)
{
	return m_pLibModule->StartCommunication(waitTimeMilliseconds);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StopCommunication(unsigned int waitTimeMilliseconds)
{
	return m_pLibModule->StopCommunication(waitTimeMilliseconds);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetModulesInfo(__ModulesInfoA * modules)
{
	return m_pLibModule->GetModulesInfo((wmx3Api::ModulesInfoA*)modules);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetModulesInfo(__ModulesInfoW * modules)
{
	return m_pLibModule->GetModulesInfo((wmx3Api::ModulesInfoW*)modules);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetModuleInfo(int moduleId, __ModuleInfoA * module)
{
	return m_pLibModule->GetModuleInfo(moduleId, (wmx3Api::ModuleInfoA*)module);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetModuleInfo(int moduleId, __ModuleInfoW * module)
{
	return m_pLibModule->GetModuleInfo(moduleId, (wmx3Api::ModuleInfoW*) module);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetEngineStatus(__EngineStatus * status)
{
	return m_pLibModule->GetEngineStatus((wmx3Api::EngineStatus*)status);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetAllDevices(__DevicesInfoA * devices)
{
	return m_pLibModule->GetAllDevices((wmx3Api::DevicesInfoA*)devices);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetAllDevices(__DevicesInfoW * devices)
{
	return m_pLibModule->GetAllDevices((wmx3Api::DevicesInfoW*)devices);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SetDeviceName(char * name)
{
	return m_pLibModule->SetDeviceName(name);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SetDeviceName(wchar_t * name)
{
	return m_pLibModule->SetDeviceName(name);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetDeviceName(char * nameBuf, unsigned int bufSize)
{
	return m_pLibModule->GetDeviceName(nameBuf, bufSize);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetDeviceName(wchar_t * nameBuf, unsigned int bufSize)
{
	return m_pLibModule->GetDeviceName(nameBuf, bufSize);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SetDeviceWaitEvent(int errCode)
{
	return m_pLibModule->SetDeviceWaitEvent(errCode);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__ResetDeviceWaitEvent()
{
	return m_pLibModule->ResetDeviceWaitEvent();
}

WMX3APIFUNC CDeviceSoftservoWMX3::__WaitForDeviceWaitEvent(unsigned int waitTime, int * errCode)
{
	return m_pLibModule->WaitForDeviceWaitEvent(waitTime, errCode);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SetModuleSuspend(__ModuleSuspend * suspend)
{
	return m_pLibModule->SetModuleSuspend((wmx3Api::ModuleSuspend*)suspend);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetModuleSuspend(__ModuleSuspend * suspend)
{
	return m_pLibModule->GetModuleSuspend((wmx3Api::ModuleSuspend*)suspend);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__AbortModuleSuspend()
{
	return m_pLibModule->AbortModuleSuspend();
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SleepAtEngine(unsigned long microSecond)
{
	return m_pLibModule->SleepAtEngine(microSecond);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__RecordWindowsUpdates(unsigned int waitTimeMilliseconds)
{
	return m_pLibModule->RecordWindowsUpdates(waitTimeMilliseconds);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__CompareWindowsUpdates(unsigned int waitTimeMilliseconds)
{
	return m_pLibModule->CompareWindowsUpdates(waitTimeMilliseconds);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetStatus(__CoreMotionStatus * status)
{
	return m_pCMModule->GetStatus((wmx3Api::CoreMotionStatus*)status);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__ExecEStop(__EStopLevel::T level)
{
	return m_pCMModule->ExecEStop((wmx3Api::EStopLevel::T)level);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__ReleaseEStop()
{
	return m_pCMModule->ReleaseEStop();
}

WMX3APIFUNC CDeviceSoftservoWMX3::__TriggerFlightRecorder()
{
	return m_pCMModule->TriggerFlightRecorder();
}

WMX3APIFUNC CDeviceSoftservoWMX3::__ResetFlightRecorder()
{
	return m_pCMModule->ResetFlightRecorder();
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SetServoOn(int axis, int newStatus)
{
	return m_pCMModule->axisControl->SetServoOn(axis, newStatus);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SetServoOn(__AxisSelection * pAxisSelection, int newStatus)
{
	return m_pCMModule->axisControl->SetServoOn((wmx3Api::AxisSelection*)pAxisSelection, newStatus);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__ClearAmpAlarm(int axis)
{
	return m_pCMModule->axisControl->ClearAmpAlarm(axis);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__ClearAmpAlarm(__AxisSelection * pAxisSelection)
{
	return m_pCMModule->axisControl->ClearAmpAlarm((wmx3Api::AxisSelection*)pAxisSelection);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__ClearAxisAlarm(int axis)
{
	return m_pCMModule->axisControl->ClearAxisAlarm(axis);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__ClearAxisAlarm(__AxisSelection * pAxisSelection)
{
	return m_pCMModule->axisControl->ClearAxisAlarm((wmx3Api::AxisSelection*)pAxisSelection);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SetAxisCommandMode(int axis, __AxisCommandMode::T mode)
{
	return m_pCMModule->axisControl->SetAxisCommandMode(axis, (wmx3Api::AxisCommandMode::T)mode);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SetAxisCommandMode(__AxisSelection * pAxisSelection, __AxisCommandMode::T mode)
{
	return m_pCMModule->axisControl->SetAxisCommandMode((wmx3Api::AxisSelection*)pAxisSelection, (wmx3Api::AxisCommandMode::T)mode);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetAxisCommandMode(int axis, __AxisCommandMode::T * pMode)
{
	return m_pCMModule->axisControl->GetAxisCommandMode(axis, (wmx3Api::AxisCommandMode::T*)pMode);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetPosCommand(int axis, double * pPosition)
{
	return m_pCMModule->axisControl->GetPosCommand(axis, pPosition);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetPosFeedback(int axis, double * pPosition)
{
	return m_pCMModule->axisControl->GetPosFeedback(axis, pPosition);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetVelCommand(int axis, double * pVelocity)
{
	return m_pCMModule->axisControl->GetVelCommand(axis, pVelocity);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetVelFeedback(int axis, double * pVelocity)
{
	return m_pCMModule->axisControl->GetVelFeedback(axis, pVelocity);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SetParam(__SystemParam * pParam, __SystemParam * pParamError)
{
	return m_pCMModule->config->SetParam((wmx3Api::Config::SystemParam*)pParam, (wmx3Api::Config::SystemParam*)pParamError);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetParam(__SystemParam * pParam)
{
	return m_pCMModule->config->GetParam((wmx3Api::Config::SystemParam*)pParam);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SetParam(int axis, __SystemParam * pParam, __SystemParam * pParamError)
{
	return m_pCMModule->config->SetParam((wmx3Api::Config::SystemParam*)pParam, (wmx3Api::Config::SystemParam*)pParamError);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetParam(int axis, __SystemParam * pParam)
{
	return m_pCMModule->config->GetParam((wmx3Api::Config::SystemParam*)pParam);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SetAxisParam(__AxisParam * pParam, __AxisParam * pParamError)
{
	return m_pCMModule->config->SetAxisParam((wmx3Api::Config::AxisParam*)pParam, (wmx3Api::Config::AxisParam*)pParamError);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetAxisParam(__AxisParam * pParam)
{
	return m_pCMModule->config->GetAxisParam((wmx3Api::Config::AxisParam*)pParam);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SetAxisParam(int axis, __AxisParam * pParam, __AxisParam * pParamError)
{
	return m_pCMModule->config->SetAxisParam(axis, (wmx3Api::Config::AxisParam*)pParamError, (wmx3Api::Config::AxisParam*)pParamError);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetAxisParam(int axis, __AxisParam * pParam)
{
	return m_pCMModule->config->GetAxisParam(axis, (wmx3Api::Config::AxisParam*)pParam);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SetGearRatio(int axis, double numerator, double denominator)
{
	return m_pCMModule->config->SetGearRatio(axis, numerator, denominator);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SetSingleTurn(int axis, bool enable, unsigned int encoderCount)
{
	return m_pCMModule->config->SetSingleTurn(axis, enable, encoderCount);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SetMovingAverageProfileTime(int axis, double milliseconds)
{
	return m_pCMModule->config->SetMovingAverageProfileTime(axis, milliseconds);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SetAxisUnit(int axis, double unit)
{
	return m_pCMModule->config->SetAxisUnit(axis, unit);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SetVelocityFeedforwardGain(int axis, double gain)
{
	return m_pCMModule->config->SetVelocityFeedforwardGain(axis, gain);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SetAxisPolarity(int axis, char polarity)
{
	return m_pCMModule->config->SetAxisPolarity(axis, polarity);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SetAbsoluteEncoderMode(int axis, bool enable)
{
	return m_pCMModule->config->SetAbsoluteEncoderMode(axis, enable);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SetAbsoluteEncoderHomeOffset(int axis, double offset)
{
	return m_pCMModule->config->SetAbsoluteEncoderHomeOffset(axis, offset);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetGearRatio(int axis, double * pNumerator, double * pDenominator)
{
	return m_pCMModule->config->GetGearRatio(axis, pNumerator, pDenominator);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetSingleTurn(int axis, bool * pEnable, unsigned int * pEncoderCount)
{
	return m_pCMModule->config->GetSingleTurn(axis, pEnable, pEncoderCount);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetMovingAverageProfileTime(int axis, double * pMilliseconds)
{
	return m_pCMModule->config->GetMovingAverageProfileTime(axis, pMilliseconds);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetAxisUnit(int axis, double * pUnit)
{
	return m_pCMModule->config->GetAxisUnit(axis, pUnit);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetVelocityFeedforwardGain(int axis, double * pGain)
{
	return m_pCMModule->config->GetVelocityFeedforwardGain(axis, pGain);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetAxisPolarity(int axis, char * pPolarity)
{
	return m_pCMModule->config->GetAxisPolarity(axis, pPolarity);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetAbsoluteEncoderMode(int axis, bool * pEnable)
{
	return m_pCMModule->config->GetAbsoluteEncoderMode(axis, pEnable);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetAbsoluteEncoderHomeOffset(int axis, double * pOffset)
{
	return m_pCMModule->config->GetAbsoluteEncoderHomeOffset(axis, pOffset);
}
WMX3APIFUNC CDeviceSoftservoWMX3::__SetFeedbackParam(int axis, __FeedbackParam * pParam, __FeedbackParam * pParamError)
{
	return m_pCMModule->config->SetFeedbackParam(axis, (wmx3Api::Config::FeedbackParam*)pParam, (wmx3Api::Config::FeedbackParam*)pParamError);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SetHomeParam(int axis, __HomeParam * pParam, __HomeParam * pParamError)
{
	return m_pCMModule->config->SetHomeParam(axis, (wmx3Api::Config::HomeParam*)pParam, (wmx3Api::Config::HomeParam*)pParamError);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SetLimitParam(int axis, __LimitParam * pParam, __LimitParam * pParamError)
{
	return m_pCMModule->config->SetLimitParam(axis, (wmx3Api::Config::LimitParam*)pParam, (wmx3Api::Config::LimitParam*)pParamError);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SetMotionParam(int axis, __MotionParam * pParam, __MotionParam * pParamError)
{
	return m_pCMModule->config->SetMotionParam(axis, (wmx3Api::Config::MotionParam*)pParam, (wmx3Api::Config::MotionParam*)pParamError);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SetAlarmParam(int axis, __AlarmParam * pParam, __AlarmParam * pParamError)
{
	return m_pCMModule->config->SetAlarmParam(axis, (wmx3Api::Config::AlarmParam*)pParam, (wmx3Api::Config::AlarmParam*)pParamError);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SetSyncParam(int axis, __SyncParam * pParam, __SyncParam * pParamError)
{
	return m_pCMModule->config->SetSyncParam(axis, (wmx3Api::Config::SyncParam*)pParam, (wmx3Api::Config::SyncParam*)pParamError);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SetFlightRecorderParam(__FlightRecorderParam * pParam, __FlightRecorderParam * pParamError)
{
	return m_pCMModule->config->SetFlightRecorderParam((wmx3Api::Config::FlightRecorderParam*)pParam, (wmx3Api::Config::FlightRecorderParam*)pParamError);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SetFlightRecorderPath(char * pPath)
{
	return m_pCMModule->config->SetFlightRecorderPath(pPath);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SetFlightRecorderPath(wchar_t * pPath)
{
	return m_pCMModule->config->SetFlightRecorderPath(pPath);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SetEmergencyStopParam(__EmergencyStopParam * pParam, __EmergencyStopParam * pParamError)
{
	return m_pCMModule->config->SetEmergencyStopParam((wmx3Api::Config::EmergencyStopParam*)pParam, (wmx3Api::Config::EmergencyStopParam*)pParamError);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetFeedbackParam(int axis, __FeedbackParam * pParam)
{
	return m_pCMModule->config->GetFeedbackParam(axis, (wmx3Api::Config::FeedbackParam*)pParam);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetHomeParam(int axis, __HomeParam * pParam)
{
	return m_pCMModule->config->GetHomeParam(axis, (wmx3Api::Config::HomeParam*)pParam);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetLimitParam(int axis, __LimitParam * pParam)
{
	return m_pCMModule->config->GetLimitParam(axis, (wmx3Api::Config::LimitParam*)pParam);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetMotionParam(int axis, __MotionParam * pParam)
{
	return m_pCMModule->config->GetMotionParam(axis, (wmx3Api::Config::MotionParam*)pParam);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetAlarmParam(int axis, __AlarmParam * pParam)
{
	return m_pCMModule->config->GetAlarmParam(axis, (wmx3Api::Config::AlarmParam*)pParam);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetSyncParam(int axis, __SyncParam * pParam)
{
	return m_pCMModule->config->GetSyncParam(axis, (wmx3Api::Config::SyncParam*)pParam);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetFlightRecorderParam(__FlightRecorderParam * pParam)
{
	return m_pCMModule->config->GetFlightRecorderParam((wmx3Api::Config::FlightRecorderParam*)pParam);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetEmergencyStopParam(__EmergencyStopParam * pParam)
{
	return m_pCMModule->config->GetEmergencyStopParam((wmx3Api::Config::EmergencyStopParam*)pParam);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetDefaultParam(__SystemParam * pParam)
{
	return m_pCMModule->config->GetDefaultParam((wmx3Api::Config::SystemParam*)pParam);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetDefaultAxisParam(__AxisParam * pAxisParam)
{
	return m_pCMModule->config->GetDefaultAxisParam((wmx3Api::Config::AxisParam*)pAxisParam);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__Export(char * pPath, __SystemParam * pParam)
{
	return m_pCMModule->config->Export(pPath, (wmx3Api::Config::SystemParam*)pParam);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__Export(wchar_t * pPath, __SystemParam * pParam)
{
	return m_pCMModule->config->Export(pPath, (wmx3Api::Config::SystemParam*)pParam);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__Export(char * pPath, __AxisParam * pAxisParam)
{
	return m_pCMModule->config->Export(pPath, (wmx3Api::Config::AxisParam*)pAxisParam);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__Export(wchar_t * pPath, __AxisParam * pAxisParam)
{
	return m_pCMModule->config->Export(pPath, (wmx3Api::Config::AxisParam*) pAxisParam);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__Export(char * pPath, __SystemParam * pParam, __AxisParam * pAxisParam)
{
	return m_pCMModule->config->Export(pPath, (wmx3Api::Config::SystemParam*)pParam, (wmx3Api::Config::AxisParam*)pAxisParam);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__Export(wchar_t * pPath, __SystemParam * pParam, __AxisParam * pAxisParam)
{
	return m_pCMModule->config->Export(pPath, (wmx3Api::Config::SystemParam*)pParam, (wmx3Api::Config::AxisParam*)pAxisParam);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__Export(char * pPath, __SystemParam * pParam, int axis)
{
	return m_pCMModule->config->Export(pPath, (wmx3Api::Config::SystemParam*)pParam, axis);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__Export(wchar_t * pPath, __SystemParam * pParam, int axis)
{
	return m_pCMModule->config->Export(pPath, (wmx3Api::Config::SystemParam*)pParam, axis);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__Export(char * pPath, __AxisParam * pAxisParam, int axis)
{
	return m_pCMModule->config->Export(pPath, (wmx3Api::Config::AxisParam*)pAxisParam, axis);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__Export(wchar_t * pPath, __AxisParam * pAxisParam, int axis)
{
	return m_pCMModule->config->Export(pPath, (wmx3Api::Config::AxisParam*)pAxisParam, axis);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__Export(char * pPath, __SystemParam * pParam, __AxisParam * pAxisParam, int axis)
{
	return m_pCMModule->config->Export(pPath, (wmx3Api::Config::SystemParam*)pParam, (wmx3Api::Config::AxisParam*)pAxisParam, axis);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__Export(wchar_t * pPath, __SystemParam * pParam, __AxisParam * pAxisParam, int axis)
{
	return m_pCMModule->config->Export(pPath, (wmx3Api::Config::SystemParam*)pParam, (wmx3Api::Config::AxisParam*)pAxisParam, axis);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__Import(char * pPath, __SystemParam * pParam)
{
	return m_pCMModule->config->Import(pPath, (wmx3Api::Config::SystemParam*)pParam);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__Import(wchar_t * pPath, __SystemParam * pParam)
{
	return m_pCMModule->config->Import(pPath, (wmx3Api::Config::SystemParam*)pParam);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__Import(char * pPath, __AxisParam * pAxisParam)
{
	return m_pCMModule->config->Import(pPath, (wmx3Api::Config::AxisParam*)pAxisParam);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__Import(wchar_t * pPath, __AxisParam * pAxisParam)
{
	return m_pCMModule->config->Import(pPath, (wmx3Api::Config::AxisParam*)pAxisParam);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__Import(char * pPath, __SystemParam * pParam, __AxisParam * pAxisParam)
{
	return m_pCMModule->config->Import(pPath, (wmx3Api::Config::SystemParam*)pParam, (wmx3Api::Config::AxisParam*)pAxisParam);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__Import(wchar_t * pPath, __SystemParam * pParam, __AxisParam * pAxisParam)
{
	return m_pCMModule->config->Import(pPath, (wmx3Api::Config::SystemParam*)pParam, (wmx3Api::Config::AxisParam*)pAxisParam);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__Import(char * pPath, __SystemParam * pParam, int axis, __AxisSelection * copyParamToAxes)
{
	return m_pCMModule->config->Import(pPath, (wmx3Api::Config::SystemParam*)pParam, axis, (wmx3Api::AxisSelection*)copyParamToAxes);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__Import(wchar_t * pPath, __SystemParam * pParam, int axis, __AxisSelection * copyParamToAxes)
{
	return m_pCMModule->config->Import(pPath, (wmx3Api::Config::SystemParam*)pParam, axis, (wmx3Api::AxisSelection*)copyParamToAxes);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__Import(char * pPath, __AxisParam * pAxisParam, int axis, __AxisSelection * copyParamToAxes)
{
	return m_pCMModule->config->Import(pPath, (wmx3Api::Config::AxisParam*)pAxisParam, axis, (wmx3Api::AxisSelection*)copyParamToAxes);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__Import(wchar_t * pPath, __AxisParam * pAxisParam, int axis, __AxisSelection * copyParamToAxes)
{
	return m_pCMModule->config->Import(pPath, (wmx3Api::Config::AxisParam*)pAxisParam, axis, (wmx3Api::AxisSelection*)copyParamToAxes);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__Import(char * pPath, __SystemParam * pParam, __AxisParam * pAxisParam, int axis, __AxisSelection * copyParamToAxes)
{
	return m_pCMModule->config->Import(pPath, (wmx3Api::Config::SystemParam*)pParam, (wmx3Api::Config::AxisParam*)pAxisParam, axis, (wmx3Api::AxisSelection*)copyParamToAxes);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__Import(wchar_t * pPath, __SystemParam * pParam, __AxisParam * pAxisParam, int axis, __AxisSelection * copyParamToAxes)
{
	return m_pCMModule->config->Import(pPath, (wmx3Api::Config::SystemParam*)pParam, (wmx3Api::Config::AxisParam*)pAxisParam, axis, (wmx3Api::AxisSelection*)copyParamToAxes);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetAndExportAll(char * pPath)
{
	return m_pCMModule->config->GetAndExportAll(pPath);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetAndExportAll(wchar_t * pPath)
{
	return m_pCMModule->config->GetAndExportAll(pPath);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__ImportAndSetAll(char * pPath, __SystemParam * pParamError, __AxisParam * pAxisParamError)
{
	return m_pCMModule->config->ImportAndSetAll(pPath, (wmx3Api::Config::SystemParam*)pParamError, (wmx3Api::Config::AxisParam*)pAxisParamError);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__ImportAndSetAll(wchar_t * pPath, __SystemParam * pParamError, __AxisParam * pAxisParamError)
{
	return m_pCMModule->config->ImportAndSetAll(pPath, (wmx3Api::Config::SystemParam*)pParamError, (wmx3Api::Config::AxisParam*)pAxisParamError);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SetSyncMasterSlave(int masterAxis, int slaveAxis, __SyncOptions * pSyncOptions)
{
	return m_pCMModule->sync->SetSyncMasterSlave(masterAxis, slaveAxis, (wmx3Api::Sync::SyncOptions*)pSyncOptions);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SetSyncCombine(int masterAxis1, int masterAxis2, int slaveAxis, __SyncCombineOptions * pSyncCombineOptions)
{
	return m_pCMModule->sync->SetSyncCombine(masterAxis1, masterAxis2, slaveAxis, (wmx3Api::Sync::SyncCombineOptions*)pSyncCombineOptions);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SetAbsoluteSyncPhase(int slaveAxis, double phase, __Profile * pProfile)
{
	return m_pCMModule->sync->SetAbsoluteSyncPhase(slaveAxis, phase, (wmx3Api::Profile*)pProfile);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__AddRelativeSyncPhase(int slaveAxis, double phase, __Profile * pProfile)
{
	return m_pCMModule->sync->AddRelativeSyncPhase(slaveAxis, phase, (wmx3Api::Profile*)pProfile);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SetSyncGearRatio(int slaveAxis, double gearRatio, __Profile * pProfile)
{
	return m_pCMModule->sync->SetSyncGearRatio(slaveAxis, gearRatio, (wmx3Api::Profile*)pProfile);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SetSyncGearRatio(int masterAxis, int slaveAxis, double gearRatio, __Profile * pProfile)
{
	return m_pCMModule->sync->SetSyncGearRatio(masterAxis, slaveAxis, gearRatio, (wmx3Api::Profile*)pProfile);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SyncToJog(int slaveAxis, __Profile * pProfile)
{
	return m_pCMModule->sync->SyncToJog(slaveAxis, (wmx3Api::Profile*)pProfile);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SyncToJog(int slaveAxis)
{
	return m_pCMModule->sync->SyncToJog(slaveAxis);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__ResolveSync(int slaveAxis)
{
	return m_pCMModule->sync->ResolveSync(slaveAxis);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SetSyncGroup(int groupId, __SyncGroup syncGroup)
{
	wmx3Api::Sync::SyncGroup sg;
	memcpy(&sg, &syncGroup, sizeof(wmx3Api::Sync::SyncGroup));

	return m_pCMModule->sync->SetSyncGroup(groupId, sg);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetSyncGroup(int groupId, __SyncGroup * pSyncGroup)
{
	return m_pCMModule->sync->GetSyncGroup(groupId, (wmx3Api::Sync::SyncGroup*)pSyncGroup);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__AddAxisToSyncGroup(int groupId, int axis, unsigned char isMaster)
{
	return m_pCMModule->sync->AddAxisToSyncGroup(groupId, axis, isMaster);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__RemoveAxisFromSyncGroup(int groupId, int axis)
{
	return m_pCMModule->sync->RemoveAxisFromSyncGroup(groupId, axis);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__EnableSyncGroup(int groupId, unsigned char enable)
{
	return m_pCMModule->sync->EnableSyncGroup(groupId, enable);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetSyncGroupStatus(int groupId, __SyncGroupStatus * pStatus)
{
	return m_pCMModule->sync->GetSyncGroupStatus(groupId, (wmx3Api::Sync::SyncGroupStatus*)pStatus);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartHome(int axis)
{
	return m_pCMModule->home->StartHome(axis);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartHome(__AxisSelection * pAxisSelection)
{
	return m_pCMModule->home->StartHome((wmx3Api::AxisSelection*)pAxisSelection);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__Continue(int axis)
{
	return m_pCMModule->home->Continue(axis);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__Continue(__AxisSelection * pAxisSelection)
{
	return m_pCMModule->home->Continue((wmx3Api::AxisSelection*)pAxisSelection);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__Cancel(int axis)
{
	return m_pCMModule->home->Cancel(axis);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__Cancel(__AxisSelection * pAxisSelection)
{
	return m_pCMModule->home->Cancel((wmx3Api::AxisSelection*)pAxisSelection);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SetCommandPos(int axis, double position)
{
	return m_pCMModule->home->SetCommandPos(axis, position);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SetFeedbackPos(int axis, double position)
{
	return m_pCMModule->home->SetFeedbackPos(axis, position);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SetHomeDone(int axis, unsigned char value)
{
	return m_pCMModule->home->SetHomeDone(axis, value);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SetCommandPosToFeedbackPos(int axis, double posChangePerCycle)
{
	return m_pCMModule->home->SetCommandPosToFeedbackPos(axis, posChangePerCycle);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetHomeData(__HomeData * pHomeData)
{
	return m_pCMModule->home->GetHomeData((wmx3Api::Home::HomeData*)pHomeData);
}






////////////////////////////////////////////////////






WMX3APIFUNC CDeviceSoftservoWMX3::__StartVel(__VelCommand * pVelCommand)
{
	return m_pCMModule->velocity->StartVel((wmx3Api::Velocity::VelCommand*)pVelCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartVel(__TimedVelCommand * pTimedVelCommand)
{
	return m_pCMModule->velocity->StartVel((wmx3Api::Velocity::TimedVelCommand*)pTimedVelCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartVel(__VelCommand * pVelCommand, double maxTrqLimit)
{
	return m_pCMModule->velocity->StartVel((wmx3Api::Velocity::VelCommand*)pVelCommand, maxTrqLimit);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartVel(unsigned int numCommands, __VelCommand * pVelCommand)
{
	return m_pCMModule->velocity->StartVel(numCommands, (wmx3Api::Velocity::VelCommand*)pVelCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartVel(unsigned int numCommands, __TimedVelCommand * pTimedVelCommand)
{
	return m_pCMModule->velocity->StartVel(numCommands, (wmx3Api::Velocity::TimedVelCommand*)pTimedVelCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StopVel(int axis)
{
	return m_pCMModule->velocity->Stop(axis);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StopVel(__AxisSelection * pAxisSelection)
{
	return m_pCMModule->velocity->Stop((wmx3Api::AxisSelection*)pAxisSelection);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__ExecQuickStopVel(int axis)
{
	return m_pCMModule->velocity->ExecQuickStop(axis);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__ExecQuickStopVel(__AxisSelection * pAxisSelection)
{
	return m_pCMModule->velocity->ExecQuickStop((wmx3Api::AxisSelection*)pAxisSelection);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SetMaxMotorSpeed(int axis, double speed)
{
	return m_pCMModule->velocity->SetMaxMotorSpeed(axis, speed);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetMaxMotorSpeed(int axis, double * pSpeed)
{
	return m_pCMModule->velocity->GetMaxMotorSpeed(axis, pSpeed);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__OverrideVel(__VelCommand * pVelCommand)
{
	return m_pCMModule->velocity->OverrideVel((wmx3Api::Velocity::VelCommand*)pVelCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__OverrideVel(unsigned int numCommands, __VelCommand * pVelCommand)
{
	return m_pCMModule->velocity->OverrideVel(numCommands, (wmx3Api::Velocity::VelCommand*)pVelCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartPosToVel(__TriggerVelCommand * pTriggerVelCommand)
{
	return m_pCMModule->velocity->StartPosToVel((wmx3Api::Velocity::TriggerVelCommand *)pTriggerVelCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartPosToVel(unsigned int numCommands, __TriggerVelCommand * pTriggerVelCommand)
{
	return m_pCMModule->velocity->StartPosToVel(numCommands, (wmx3Api::Velocity::TriggerVelCommand *)pTriggerVelCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartPosToVel(__VelCommand * pVelCommand, __TriggerEvents * pTriggerEvents)
{
	return m_pCMModule->velocity->StartPosToVel((wmx3Api::Velocity::VelCommand*)pVelCommand, (wmx3Api::TriggerEvents*)pTriggerEvents);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartPosToVel(unsigned int numCommands, __VelCommand * pVelCommand, __TriggerEvents * pTriggerEvents)
{
	return m_pCMModule->velocity->StartPosToVel(numCommands, (wmx3Api::Velocity::VelCommand *)pVelCommand, (wmx3Api::TriggerEvents *)pTriggerEvents);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SetMaxTrqLimit(int axis, double torque)
{
	return m_pCMModule->torque->SetMaxTrqLimit(axis, torque);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetMaxTrqLimit(int axis, double * pTorque)
{
	return m_pCMModule->torque->GetMaxTrqLimit(axis, pTorque);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SetPositiveTrqLimit(int axis, double torque)
{
	return m_pCMModule->torque->SetPositiveTrqLimit(axis, torque);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetPositiveTrqLimit(int axis, double * pTorque)
{
	return m_pCMModule->torque->GetPositiveTrqLimit(axis, pTorque);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SetNegativeTrqLimit(int axis, double torque)
{
	return m_pCMModule->torque->SetNegativeTrqLimit(axis, torque);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__GetNegativeTrqLimit(int axis, double * pTorque)
{
	return m_pCMModule->torque->GetNegativeTrqLimit(axis, pTorque);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartTrq(__TrqCommand * pTrqCommand)
{
	return m_pCMModule->torque->StartTrq((wmx3Api::Torque::TrqCommand *)pTrqCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartTrq(unsigned int numCommands, __TrqCommand * pTrqCommand)
{
	return m_pCMModule->torque->StartTrq(numCommands, (wmx3Api::Torque::TrqCommand *)pTrqCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartTrq(__TrqCommand * pTrqCommand, double maxMotorSpeed)
{
	return m_pCMModule->torque->StartTrq((wmx3Api::Torque::TrqCommand *)pTrqCommand, maxMotorSpeed);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartRampTimeTrq(__TrqCommand * pTrqCommand, unsigned int rampCycleTime)
{
	return m_pCMModule->torque->StartRampTimeTrq((wmx3Api::Torque::TrqCommand *)pTrqCommand, rampCycleTime);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartRampTimeTrq(__TrqCommand * pTrqCommand, unsigned int rampCycleTime, double maxMotorSpeed)
{
	return m_pCMModule->torque->StartRampTimeTrq((wmx3Api::Torque::TrqCommand *)pTrqCommand, rampCycleTime, maxMotorSpeed);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartRampRateTrq(__TrqCommand * pTrqCommand, double rampRate)
{
	return m_pCMModule->torque->StartRampRateTrq((wmx3Api::Torque::TrqCommand *)pTrqCommand, rampRate);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartRampRateTrq(__TrqCommand * pTrqCommand, double rampRate, double maxMotorSpeed)
{
	return m_pCMModule->torque->StartRampRateTrq((wmx3Api::Torque::TrqCommand *)pTrqCommand, rampRate, maxMotorSpeed);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StopTrq(int axis)
{
	return m_pCMModule->torque->StopTrq(axis);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StopTrq(__AxisSelection * pAxisSelection)
{
	return m_pCMModule->torque->StopTrq((wmx3Api::AxisSelection *)pAxisSelection);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartPosToTrq(__TriggerTrqCommand * pTriggerTrqCommand)
{
	return m_pCMModule->torque->StartPosToTrq((wmx3Api::Torque::TriggerTrqCommand *)pTriggerTrqCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartPosToTrq(unsigned int numCommands, __TriggerTrqCommand * pTriggerTrqCommand)
{
	return m_pCMModule->torque->StartPosToTrq(numCommands, (wmx3Api::Torque::TriggerTrqCommand *)pTriggerTrqCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartPosToTrq(__TrqCommand * pTrqCommand, __TriggerEvents * pTriggerEvents)
{
	return m_pCMModule->torque->StartPosToTrq((wmx3Api::Torque::TrqCommand *)pTrqCommand, (wmx3Api::TriggerEvents *)pTriggerEvents);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartPosToTrq(unsigned int numCommands, __TrqCommand * pTrqCommand, __TriggerEvents * pTriggerEvents)
{
	return m_pCMModule->torque->StartPosToTrq(numCommands, (wmx3Api::Torque::TrqCommand*)pTrqCommand, (wmx3Api::TriggerEvents *)pTriggerEvents);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartVelToTrq(__TriggerTrqCommand * pTriggerTrqCommand)
{
	return m_pCMModule->torque->StartVelToTrq((wmx3Api::Torque::TriggerTrqCommand *)pTriggerTrqCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartVelToTrq(unsigned int numCommands, __TriggerTrqCommand * pTriggerTrqCommand)
{
	return m_pCMModule->torque->StartVelToTrq(numCommands, (wmx3Api::Torque::TriggerTrqCommand *)pTriggerTrqCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartVelToTrq(__TrqCommand * pTrqCommand, __TriggerEvents * pTriggerEvents)
{
	return m_pCMModule->torque->StartVelToTrq((wmx3Api::Torque::TrqCommand *)pTrqCommand, (wmx3Api::TriggerEvents *)pTriggerEvents);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartVelToTrq(unsigned int numCommands, __TrqCommand * pTrqCommand, __TriggerEvents * pTriggerEvents)
{
	return m_pCMModule->torque->StartVelToTrq(numCommands, (wmx3Api::Torque::TrqCommand *)pTrqCommand, (wmx3Api::TriggerEvents *)pTriggerEvents);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartPos(__PosCommand * pPosCommand)
{
	return m_pCMModule->motion->StartPos((wmx3Api::Motion::PosCommand *)pPosCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartMov(__PosCommand * pPosCommand)
{
	return m_pCMModule->motion->StartMov((wmx3Api::Motion::PosCommand *)pPosCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartPos(__TriggerPosCommand * pPosCommand)
{
	return m_pCMModule->motion->StartPos((wmx3Api::Motion::TriggerPosCommand *)pPosCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartMov(__TriggerPosCommand * pPosCommand)
{
	return m_pCMModule->motion->StartMov((wmx3Api::Motion::TriggerPosCommand *)pPosCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartPos(__PosCommand * pPosCommand, __TriggerEvents * pTriggerEvents)
{
	return m_pCMModule->motion->StartPos((wmx3Api::Motion::PosCommand *)pPosCommand, (wmx3Api::TriggerEvents *)pTriggerEvents);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartMov(__PosCommand * pPosCommand, __TriggerEvents * pTriggerEvents)
{
	return m_pCMModule->motion->StartMov((wmx3Api::Motion::PosCommand *)pPosCommand, (wmx3Api::TriggerEvents *)pTriggerEvents);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartPos(unsigned int numCommands, __PosCommand * pPosCommand)
{
	return m_pCMModule->motion->StartPos(numCommands, (wmx3Api::Motion::PosCommand *)pPosCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartMov(unsigned int numCommands, __PosCommand * pPosCommand)
{
	return m_pCMModule->motion->StartMov(numCommands, (wmx3Api::Motion::PosCommand *)pPosCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartPos(unsigned int numCommands, __TriggerPosCommand * pPosCommand)
{
	return m_pCMModule->motion->StartPos(numCommands, (wmx3Api::Motion::TriggerPosCommand *)pPosCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartMov(unsigned int numCommands, __TriggerPosCommand * pPosCommand)
{
	return m_pCMModule->motion->StartMov(numCommands, (wmx3Api::Motion::TriggerPosCommand *)pPosCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartPos(unsigned int numCommands, __PosCommand * pPosCommand, __TriggerEvents * pTriggerEvents)
{
	return m_pCMModule->motion->StartPos(numCommands, (wmx3Api::Motion::PosCommand *)pPosCommand, (wmx3Api::TriggerEvents *)pTriggerEvents);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartMov(unsigned int numCommands, __PosCommand * pPosCommand, __TriggerEvents * pTriggerEvents)
{
	return m_pCMModule->motion->StartMov(numCommands, (wmx3Api::Motion::PosCommand *)pPosCommand, (wmx3Api::TriggerEvents *)pTriggerEvents);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartLinearIntplPos(__LinearIntplCommand * pLinearIntplCommand)
{
	return m_pCMModule->motion->StartLinearIntplPos((wmx3Api::Motion::LinearIntplCommand *)pLinearIntplCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartLinearIntplMov(__LinearIntplCommand * pLinearIntplCommand)
{
	return m_pCMModule->motion->StartLinearIntplMov((wmx3Api::Motion::LinearIntplCommand *)pLinearIntplCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartLinearIntplPos(__LinearIntplCommand * pLinearIntplCommand, __Trigger * pTrigger)
{
	return m_pCMModule->motion->StartLinearIntplPos((wmx3Api::Motion::LinearIntplCommand *)pLinearIntplCommand, (wmx3Api::Trigger *)pTrigger);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartLinearIntplMov(__LinearIntplCommand * pLinearIntplCommand, __Trigger * pTrigger)
{
	return m_pCMModule->motion->StartLinearIntplMov((wmx3Api::Motion::LinearIntplCommand *)pLinearIntplCommand, (wmx3Api::Trigger *)pTrigger);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartLinearIntplPos(__LinearIntplCommand * pLinearIntplCommand, __TriggerEvents * pTriggerEvents)
{
	return m_pCMModule->motion->StartLinearIntplPos((wmx3Api::Motion::LinearIntplCommand *)pLinearIntplCommand, (wmx3Api::TriggerEvents *)pTriggerEvents);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartLinearIntplMov(__LinearIntplCommand * pLinearIntplCommand, __TriggerEvents * pTriggerEvents)
{
	return m_pCMModule->motion->StartLinearIntplMov((wmx3Api::Motion::LinearIntplCommand *)pLinearIntplCommand, (wmx3Api::TriggerEvents *)pTriggerEvents);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartCircularIntplPos(__CenterAndLengthCircularIntplCommand * pCircularIntplCommand)
{
	return m_pCMModule->motion->StartCircularIntplPos((wmx3Api::Motion::CenterAndLengthCircularIntplCommand *)pCircularIntplCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartCircularIntplMov(__CenterAndLengthCircularIntplCommand * pCircularIntplCommand)
{
	return m_pCMModule->motion->StartCircularIntplMov((wmx3Api::Motion::CenterAndLengthCircularIntplCommand *)pCircularIntplCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartCircularIntplPos(__CenterAndEndCircularIntplCommand * pCircularIntplCommand)
{
	return m_pCMModule->motion->StartCircularIntplPos((wmx3Api::Motion::CenterAndEndCircularIntplCommand *)pCircularIntplCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartCircularIntplMov(__CenterAndEndCircularIntplCommand * pCircularIntplCommand)
{
	return m_pCMModule->motion->StartCircularIntplMov((wmx3Api::Motion::CenterAndEndCircularIntplCommand *)pCircularIntplCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartCircularIntplPos(__ThroughAndEndCircularIntplCommand * pCircularIntplCommand)
{
	return m_pCMModule->motion->StartCircularIntplPos((wmx3Api::Motion::ThroughAndEndCircularIntplCommand *)pCircularIntplCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartCircularIntplMov(__ThroughAndEndCircularIntplCommand * pCircularIntplCommand)
{
	return m_pCMModule->motion->StartCircularIntplMov((wmx3Api::Motion::ThroughAndEndCircularIntplCommand *)pCircularIntplCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartCircularIntplPos(__LengthAndEndCircularIntplCommand * pCircularIntplCommand)
{
	return m_pCMModule->motion->StartCircularIntplPos((wmx3Api::Motion::LengthAndEndCircularIntplCommand *)pCircularIntplCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartCircularIntplMov(__LengthAndEndCircularIntplCommand * pCircularIntplCommand)
{
	return m_pCMModule->motion->StartCircularIntplMov((wmx3Api::Motion::LengthAndEndCircularIntplCommand *)pCircularIntplCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartCircularIntplPos(__RadiusAndEndCircularIntplCommand * pCircularIntplCommand)
{
	return m_pCMModule->motion->StartCircularIntplPos((wmx3Api::Motion::RadiusAndEndCircularIntplCommand *)pCircularIntplCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartCircularIntplMov(__RadiusAndEndCircularIntplCommand * pCircularIntplCommand)
{
	return m_pCMModule->motion->StartCircularIntplMov((wmx3Api::Motion::RadiusAndEndCircularIntplCommand *)pCircularIntplCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartCircularIntplPos(__ThroughAndEnd3DCircularIntplCommand * pCircularIntplCommand)
{
	return m_pCMModule->motion->StartCircularIntplPos((wmx3Api::Motion::ThroughAndEnd3DCircularIntplCommand*)pCircularIntplCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartCircularIntplMov(__ThroughAndEnd3DCircularIntplCommand * pCircularIntplCommand)
{
	return m_pCMModule->motion->StartCircularIntplMov((wmx3Api::Motion::ThroughAndEnd3DCircularIntplCommand *)pCircularIntplCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartCircularIntplPos(__CenterAndLengthCircularIntplCommand * pCircularIntplCommand, __Trigger * pTrigger)
{
	return m_pCMModule->motion->StartCircularIntplMov((wmx3Api::Motion::CenterAndLengthCircularIntplCommand*)pCircularIntplCommand, (wmx3Api::Trigger *)pTrigger);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartCircularIntplMov(__CenterAndLengthCircularIntplCommand * pCircularIntplCommand, __Trigger * pTrigger)
{
	return m_pCMModule->motion->StartCircularIntplMov((wmx3Api::Motion::CenterAndLengthCircularIntplCommand*)pCircularIntplCommand, (wmx3Api::Trigger *)pTrigger);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartCircularIntplPos(__CenterAndEndCircularIntplCommand * pCircularIntplCommand, __Trigger * pTrigger)
{
	return m_pCMModule->motion->StartCircularIntplMov((wmx3Api::Motion::CenterAndEndCircularIntplCommand*)pCircularIntplCommand, (wmx3Api::Trigger *)pTrigger);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartCircularIntplMov(__CenterAndEndCircularIntplCommand * pCircularIntplCommand, __Trigger * pTrigger)
{
	return m_pCMModule->motion->StartCircularIntplMov((wmx3Api::Motion::CenterAndEndCircularIntplCommand*)pCircularIntplCommand, (wmx3Api::Trigger *)pTrigger);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartCircularIntplPos(__ThroughAndEndCircularIntplCommand * pCircularIntplCommand, __Trigger * pTrigger)
{
	return m_pCMModule->motion->StartCircularIntplMov((wmx3Api::Motion::ThroughAndEndCircularIntplCommand*)pCircularIntplCommand, (wmx3Api::Trigger *)pTrigger);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartCircularIntplMov(__ThroughAndEndCircularIntplCommand * pCircularIntplCommand, __Trigger * pTrigger)
{
	return m_pCMModule->motion->StartCircularIntplMov((wmx3Api::Motion::ThroughAndEndCircularIntplCommand*)pCircularIntplCommand, (wmx3Api::Trigger *)pTrigger);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartCircularIntplPos(__LengthAndEndCircularIntplCommand * pCircularIntplCommand, __Trigger * pTrigger)
{
	return m_pCMModule->motion->StartCircularIntplMov((wmx3Api::Motion::LengthAndEndCircularIntplCommand*)pCircularIntplCommand, (wmx3Api::Trigger *)pTrigger);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartCircularIntplMov(__LengthAndEndCircularIntplCommand * pCircularIntplCommand, __Trigger * pTrigger)
{
	return m_pCMModule->motion->StartCircularIntplMov((wmx3Api::Motion::LengthAndEndCircularIntplCommand*)pCircularIntplCommand, (wmx3Api::Trigger *)pTrigger);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartCircularIntplPos(__RadiusAndEndCircularIntplCommand * pCircularIntplCommand, __Trigger * pTrigger)
{
	return m_pCMModule->motion->StartCircularIntplMov((wmx3Api::Motion::RadiusAndEndCircularIntplCommand*)pCircularIntplCommand, (wmx3Api::Trigger *)pTrigger);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartCircularIntplMov(__RadiusAndEndCircularIntplCommand * pCircularIntplCommand, __Trigger * pTrigger)
{
	return m_pCMModule->motion->StartCircularIntplMov((wmx3Api::Motion::RadiusAndEndCircularIntplCommand*)pCircularIntplCommand, (wmx3Api::Trigger *)pTrigger);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartCircularIntplPos(__ThroughAndEnd3DCircularIntplCommand * pCircularIntplCommand, __Trigger * pTrigger)
{
	return m_pCMModule->motion->StartCircularIntplMov((wmx3Api::Motion::ThroughAndEnd3DCircularIntplCommand*)pCircularIntplCommand, (wmx3Api::Trigger *)pTrigger);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartCircularIntplMov(__ThroughAndEnd3DCircularIntplCommand * pCircularIntplCommand, __Trigger * pTrigger)
{
	return m_pCMModule->motion->StartCircularIntplMov((wmx3Api::Motion::ThroughAndEnd3DCircularIntplCommand*)pCircularIntplCommand, (wmx3Api::Trigger *)pTrigger);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartCircularIntplPos(__CenterAndLengthCircularIntplCommand * pCircularIntplCommand, __TriggerEvents * pTriggerEvents)
{
	return m_pCMModule->motion->StartCircularIntplMov((wmx3Api::Motion::CenterAndLengthCircularIntplCommand*)pCircularIntplCommand, (wmx3Api::TriggerEvents *)pTriggerEvents);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartCircularIntplMov(__CenterAndLengthCircularIntplCommand * pCircularIntplCommand, __TriggerEvents * pTriggerEvents)
{
	return m_pCMModule->motion->StartCircularIntplMov((wmx3Api::Motion::CenterAndLengthCircularIntplCommand*)pCircularIntplCommand, (wmx3Api::TriggerEvents *)pTriggerEvents);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartCircularIntplPos(__CenterAndEndCircularIntplCommand * pCircularIntplCommand, __TriggerEvents * pTriggerEvents)
{
	return m_pCMModule->motion->StartCircularIntplMov((wmx3Api::Motion::CenterAndEndCircularIntplCommand*)pCircularIntplCommand, (wmx3Api::TriggerEvents *)pTriggerEvents);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartCircularIntplMov(__CenterAndEndCircularIntplCommand * pCircularIntplCommand, __TriggerEvents * pTriggerEvents)
{
	return m_pCMModule->motion->StartCircularIntplMov((wmx3Api::Motion::CenterAndEndCircularIntplCommand*)pCircularIntplCommand, (wmx3Api::TriggerEvents *)pTriggerEvents);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartCircularIntplPos(__ThroughAndEndCircularIntplCommand * pCircularIntplCommand, __TriggerEvents * pTriggerEvents)
{
	return m_pCMModule->motion->StartCircularIntplMov((wmx3Api::Motion::ThroughAndEndCircularIntplCommand*)pCircularIntplCommand, (wmx3Api::TriggerEvents *)pTriggerEvents);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartCircularIntplMov(__LengthAndEndCircularIntplCommand * pCircularIntplCommand, __TriggerEvents * pTriggerEvents)
{
	return m_pCMModule->motion->StartCircularIntplMov((wmx3Api::Motion::LengthAndEndCircularIntplCommand*)pCircularIntplCommand, (wmx3Api::TriggerEvents *)pTriggerEvents);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartCircularIntplPos(__RadiusAndEndCircularIntplCommand * pCircularIntplCommand, __TriggerEvents * pTriggerEvents)
{
	return m_pCMModule->motion->StartCircularIntplMov((wmx3Api::Motion::RadiusAndEndCircularIntplCommand*)pCircularIntplCommand, (wmx3Api::TriggerEvents *)pTriggerEvents);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartCircularIntplMov(__RadiusAndEndCircularIntplCommand * pCircularIntplCommand, __TriggerEvents * pTriggerEvents)
{
	return m_pCMModule->motion->StartCircularIntplMov((wmx3Api::Motion::RadiusAndEndCircularIntplCommand*)pCircularIntplCommand, (wmx3Api::TriggerEvents *)pTriggerEvents);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartCircularIntplPos(__ThroughAndEnd3DCircularIntplCommand * pCircularIntplCommand, __TriggerEvents * pTriggerEvents)
{
	return m_pCMModule->motion->StartCircularIntplMov((wmx3Api::Motion::ThroughAndEnd3DCircularIntplCommand*)pCircularIntplCommand, (wmx3Api::TriggerEvents *)pTriggerEvents);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartCircularIntplMov(__ThroughAndEnd3DCircularIntplCommand * pCircularIntplCommand, __TriggerEvents * pTriggerEvents)
{
	return m_pCMModule->motion->StartCircularIntplMov((wmx3Api::Motion::ThroughAndEnd3DCircularIntplCommand*)pCircularIntplCommand, (wmx3Api::TriggerEvents *)pTriggerEvents);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartHelicalIntplPos(__HelicalIntplCommand * pHelicalIntplCommand)
{
	return m_pCMModule->motion->StartHelicalIntplPos((wmx3Api::Motion::HelicalIntplCommand*)pHelicalIntplCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartHelicalIntplMov(__HelicalIntplCommand * pHelicalIntplCommand)
{
	return m_pCMModule->motion->StartHelicalIntplMov((wmx3Api::Motion::HelicalIntplCommand*)pHelicalIntplCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartHelicalIntplPos(__HelicalIntplCommand * pHelicalIntplCommand, __Trigger * pTrigger)
{
	return m_pCMModule->motion->StartHelicalIntplPos((wmx3Api::Motion::HelicalIntplCommand*)pHelicalIntplCommand, (wmx3Api::Trigger *)pTrigger);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartHelicalIntplMov(__HelicalIntplCommand * pHelicalIntplCommand, __Trigger * pTrigger)
{
	return m_pCMModule->motion->StartHelicalIntplMov((wmx3Api::Motion::HelicalIntplCommand*)pHelicalIntplCommand, (wmx3Api::Trigger *)pTrigger);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartHelicalIntplPos(__HelicalIntplCommand * pHelicalIntplCommand, __TriggerEvents * pTriggerEvents)
{
	return m_pCMModule->motion->StartHelicalIntplPos((wmx3Api::Motion::HelicalIntplCommand*)pHelicalIntplCommand, (wmx3Api::TriggerEvents *)pTriggerEvents);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartHelicalIntplMov(__HelicalIntplCommand * pHelicalIntplCommand, __TriggerEvents * pTriggerEvents)
{
	return m_pCMModule->motion->StartHelicalIntplMov((wmx3Api::Motion::HelicalIntplCommand*)pHelicalIntplCommand, (wmx3Api::TriggerEvents *)pTriggerEvents);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartJog(__JogCommand * pJogCommand)
{
	return m_pCMModule->motion->StartJog((wmx3Api::Motion::JogCommand*)pJogCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartJog(__TimedJogCommand * pTimedJogCommand)
{
	return m_pCMModule->motion->StartJog((wmx3Api::Motion::TimedJogCommand*)pTimedJogCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartJog(unsigned int numCommands, __JogCommand * pJogCommand)
{
	return m_pCMModule->motion->StartJog(numCommands, (wmx3Api::Motion::JogCommand*)pJogCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartJog(unsigned int numCommands, __TimedJogCommand * pTimedJogCommand)
{
	return m_pCMModule->motion->StartJog(numCommands, (wmx3Api::Motion::TimedJogCommand*)pTimedJogCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartPosToJog(__TriggerJogCommand * pTriggerJogCommand)
{
	return m_pCMModule->motion->StartPosToJog((wmx3Api::Motion::TriggerJogCommand*)pTriggerJogCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartPosToJog(unsigned int numCommands, __TriggerJogCommand * pTriggerJogCommand)
{
	return m_pCMModule->motion->StartPosToJog(numCommands, (wmx3Api::Motion::TriggerJogCommand*)pTriggerJogCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartPosToJog(__JogCommand * pJogCommand, __TriggerEvents * pTriggerEvents)
{
	return m_pCMModule->motion->StartPosToJog((wmx3Api::Motion::JogCommand*)pJogCommand, (wmx3Api::TriggerEvents *)pTriggerEvents);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartPosToJog(unsigned int numCommands, __JogCommand * pJogCommand, __TriggerEvents * pTriggerEvents)
{
	return m_pCMModule->motion->StartPosToJog(numCommands, (wmx3Api::Motion::JogCommand*)pJogCommand, (wmx3Api::TriggerEvents *)pTriggerEvents);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartPosToJog(__PosToJogCommand * pPosToJogCommand)
{
	return m_pCMModule->motion->StartPosToJog((wmx3Api::Motion::PosToJogCommand*)pPosToJogCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartPosToJog(unsigned int numCommands, __PosToJogCommand * pPosToJogCommand)
{
	return m_pCMModule->motion->StartPosToJog(numCommands, (wmx3Api::Motion::PosToJogCommand*)pPosToJogCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartMovToJog(__PosToJogCommand * pPosToJogCommand)
{
	return m_pCMModule->motion->StartMovToJog((wmx3Api::Motion::PosToJogCommand*)pPosToJogCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StartMovToJog(unsigned int numCommands, __PosToJogCommand * pPosToJogCommand)
{
	return m_pCMModule->motion->StartMovToJog(numCommands, (wmx3Api::Motion::PosToJogCommand*)pPosToJogCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__Stop(int axis)
{
	return m_pCMModule->motion->Stop(axis);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__Stop(__AxisSelection * pAxisSelection)
{
	return m_pCMModule->motion->Stop((wmx3Api::AxisSelection *)pAxisSelection);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__Stop(int axis, double dec)
{
	return m_pCMModule->motion->Stop(axis, dec);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__Stop(unsigned int numCommands, __StopCommand * pStopCommand)
{
	return m_pCMModule->motion->Stop(numCommands, (wmx3Api::Motion::StopCommand*)pStopCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__ExecQuickStop(int axis)
{
	return m_pCMModule->motion->ExecQuickStop(axis);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__ExecQuickStop(__AxisSelection * pAxisSelection)
{
	return m_pCMModule->motion->ExecQuickStop((wmx3Api::AxisSelection *)pAxisSelection);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__ExecTimedStop(int axis, double timeMilliseconds)
{
	return m_pCMModule->motion->ExecTimedStop(axis, timeMilliseconds);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__ExecTimedStop(__AxisSelection * pAxisSelection, double timeMilliseconds)
{
	return m_pCMModule->motion->ExecTimedStop((wmx3Api::AxisSelection *)pAxisSelection, timeMilliseconds);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__ExecTimedStop(unsigned int numCommands, __TimeCommand * pTimeCommand)
{
	return m_pCMModule->motion->ExecTimedStop(numCommands, (wmx3Api::Motion::TimeCommand*)pTimeCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__Wait(int axis)
{
	return m_pCMModule->motion->Wait(axis);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__Wait(__AxisSelection * pAxisSelection)
{
	return m_pCMModule->motion->Wait((wmx3Api::AxisSelection *)pAxisSelection);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__Wait(__WaitCondition * pWaitCondition)
{
	return m_pCMModule->motion->Wait((wmx3Api::Motion::WaitCondition*)pWaitCondition);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__Wait(int axis, unsigned int waitTimeMilliseconds)
{
	return m_pCMModule->motion->Wait(axis, waitTimeMilliseconds);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__Wait(__AxisSelection * pAxisSelection, unsigned int waitTimeMilliseconds)
{
	return m_pCMModule->motion->Wait((wmx3Api::AxisSelection *)pAxisSelection, waitTimeMilliseconds);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__Wait(__WaitCondition * pWaitCondition, unsigned int waitTimeMilliseconds)
{
	return m_pCMModule->motion->Wait((wmx3Api::Motion::WaitCondition*)pWaitCondition, waitTimeMilliseconds);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__Pause(int axis)
{
	return m_pCMModule->motion->Pause(axis);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__Pause(__AxisSelection * pAxisSelection)
{
	return m_pCMModule->motion->Pause((wmx3Api::AxisSelection *)pAxisSelection);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__Resume(int axis)
{
	return m_pCMModule->motion->Resume(axis);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__Resume(__AxisSelection * pAxisSelection)
{
	return m_pCMModule->motion->Resume((wmx3Api::AxisSelection *)pAxisSelection);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__OverridePos(__PosCommand * pPosCommand)
{
	return m_pCMModule->motion->OverridePos((wmx3Api::Motion::PosCommand*)pPosCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__OverrideMov(__PosCommand * pPosCommand)
{
	return m_pCMModule->motion->OverrideMov((wmx3Api::Motion::PosCommand*)pPosCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__OverrideVel(__PosCommand * pPosCommand)
{
	return m_pCMModule->motion->OverrideVel((wmx3Api::Motion::PosCommand*)pPosCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__OverrideAcc(__PosCommand * pPosCommand)
{
	return m_pCMModule->motion->OverrideAcc((wmx3Api::Motion::PosCommand*)pPosCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__OverrideDec(__PosCommand * pPosCommand)
{
	return m_pCMModule->motion->OverrideDec((wmx3Api::Motion::PosCommand*)pPosCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__OverrideJerkAcc(__PosCommand * pPosCommand)
{
	return m_pCMModule->motion->OverrideJerkAcc((wmx3Api::Motion::PosCommand*)pPosCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__OverrideJerkDec(__PosCommand * pPosCommand)
{
	return m_pCMModule->motion->OverrideJerkDec((wmx3Api::Motion::PosCommand*)pPosCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__OverrideProfile(__PosCommand * pPosCommand)
{
	return m_pCMModule->motion->OverrideProfile((wmx3Api::Motion::PosCommand*)pPosCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__OverridePos(unsigned int numCommands, __PosCommand * pPosCommand)
{
	return m_pCMModule->motion->OverridePos(numCommands, (wmx3Api::Motion::PosCommand*)pPosCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__OverrideMov(unsigned int numCommands, __PosCommand * pPosCommand)
{
	return m_pCMModule->motion->OverrideMov(numCommands, (wmx3Api::Motion::PosCommand*)pPosCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__OverrideVel(unsigned int numCommands, __PosCommand * pPosCommand)
{
	return m_pCMModule->motion->OverrideVel(numCommands, (wmx3Api::Motion::PosCommand*)pPosCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__OverrideAcc(unsigned int numCommands, __PosCommand * pPosCommand)
{
	return m_pCMModule->motion->OverrideAcc(numCommands, (wmx3Api::Motion::PosCommand*)pPosCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__OverrideDec(unsigned int numCommands, __PosCommand * pPosCommand)
{
	return m_pCMModule->motion->OverrideDec(numCommands, (wmx3Api::Motion::PosCommand*)pPosCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__OverrideJerkAcc(unsigned int numCommands, __PosCommand * pPosCommand)
{
	return m_pCMModule->motion->OverrideJerkAcc(numCommands, (wmx3Api::Motion::PosCommand*)pPosCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__OverrideJerkDec(unsigned int numCommands, __PosCommand * pPosCommand)
{
	return m_pCMModule->motion->OverrideDec(numCommands, (wmx3Api::Motion::PosCommand*)pPosCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__OverrideProfile(unsigned int numCommands, __PosCommand * pPosCommand)
{
	return m_pCMModule->motion->OverrideProfile(numCommands, (wmx3Api::Motion::PosCommand*)pPosCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StopJogAtPos(__PosCommand * pPosCommand)
{
	return m_pCMModule->motion->StopJogAtPos((wmx3Api::Motion::PosCommand*)pPosCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StopJogAtPos(unsigned int numCommands, __PosCommand * pPosCommand)
{
	return m_pCMModule->motion->StopJogAtPos(numCommands, (wmx3Api::Motion::PosCommand*)pPosCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SuperimposeMov(__PosCommand * pPosCommand)
{
	return m_pCMModule->motion->SuperimposeMov((wmx3Api::Motion::PosCommand*)pPosCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SuperimposeMov(unsigned int numCommands, __PosCommand * pPosCommand)
{
	return m_pCMModule->motion->SuperimposeMov(numCommands, (wmx3Api::Motion::PosCommand*)pPosCommand);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StopSuperimpose(int axis)
{
	return m_pCMModule->motion->StopSuperimpose(axis);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__StopSuperimpose(__AxisSelection * pAxisSelection)
{
	return m_pCMModule->motion->StopSuperimpose((wmx3Api::AxisSelection*)pAxisSelection);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SimulatePos(__SimulatePosCommand * pSimulatePosCommand, double * pPeakVelocity, double * pTotalTimeMilliseconds, double * pAccelerationTimeMilliseconds, double * pCruiseTimeMilliseconds, double * pDecelerationTimeMilliseconds)
{
	return m_pCMModule->motion->SimulatePos((wmx3Api::Motion::SimulatePosCommand*)pSimulatePosCommand, pPeakVelocity, pTotalTimeMilliseconds, pAccelerationTimeMilliseconds, pCruiseTimeMilliseconds, pDecelerationTimeMilliseconds);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SimulateLinearIntplPos(__SimulateLinearIntplCommand * pSimulateLinearIntplCommand, double * pPeakVelocity, double * pTotalTimeMilliseconds, double * pAccelerationTimeMilliseconds, double * pCruiseTimeMilliseconds, double * pDecelerationTimeMilliseconds)
{
	return m_pCMModule->motion->SimulateLinearIntplPos((wmx3Api::Motion::SimulateLinearIntplCommand*)pSimulateLinearIntplCommand, pPeakVelocity, pTotalTimeMilliseconds, pAccelerationTimeMilliseconds, pCruiseTimeMilliseconds, pDecelerationTimeMilliseconds);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SimulatePosAtTime(__SimulatePosCommand * pSimulatePosCommand, int returnType, double timeMilliseconds, double * pCalculatedPos, double * pCalculatedVel)
{
	return m_pCMModule->motion->SimulatePosAtTime((wmx3Api::Motion::SimulatePosCommand*)pSimulatePosCommand, returnType, timeMilliseconds, pCalculatedPos, pCalculatedVel);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SimulatePosAtTime(__SimulateLinearIntplCommand * pSimulateLinearIntplCommand, double timeMilliseconds, double * pPosArray, double * pMoveDistance, double * pRemainDistance, double * pTotalDistance)
{
	return m_pCMModule->motion->SimulatePosAtTime((wmx3Api::Motion::SimulateLinearIntplCommand*)pSimulateLinearIntplCommand, timeMilliseconds, pPosArray, pMoveDistance, pRemainDistance, pTotalDistance);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SimulateTimeAtPos(__SimulatePosCommand * pSimulatePosCommand, double specificPos, double * pMoveTimeMilliseconds, double * pRemainTimeMilliseconds)
{
	return m_pCMModule->motion->SimulateTimeAtPos((wmx3Api::Motion::SimulatePosCommand*)pSimulatePosCommand, specificPos, pMoveTimeMilliseconds, pRemainTimeMilliseconds);
}

WMX3APIFUNC CDeviceSoftservoWMX3::__SimulateTimeAtDist(__SimulateLinearIntplCommand * pSimulateLinearIntplCommand, double specificDistance, double * pMoveTimeMilliseconds, double * pRemainTimeMilliseconds, double * pTotalTimeMilliseconds)
{
	return m_pCMModule->motion->SimulateTimeAtDist((wmx3Api::Motion::SimulateLinearIntplCommand*)pSimulateLinearIntplCommand, specificDistance, pMoveTimeMilliseconds, pRemainTimeMilliseconds, pTotalTimeMilliseconds);
}

ECAPIFUNC CDeviceSoftservoWMX3::__ScanNetwork()
{
	wmx3Api::ecApi::Ecat ecLib(m_pLibModule);
	return ecLib.ScanNetwork();
}

ECAPIFUNC CDeviceSoftservoWMX3::__GetMasterInfo(__EcMasterInfo * pMasterInfo)
{
	wmx3Api::ecApi::Ecat ecLib(m_pLibModule);
	return ecLib.GetMasterInfo((wmx3Api::ecApi::EcMasterInfo*)pMasterInfo);
}

bool CDeviceSoftservoWMX3::DoesModuleExist()
{
	bool bReturn = false;

	CString strModuleName;

	do
	{
		strModuleName.Format(_T("WMX3Api_CLRLib.dll"));

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

		strModuleName.Format(_T("CoreMotionApi_CLRLib.dll"));

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

		if(!bReturn)
			break;

		strModuleName.Format(_T("IMDll.dll"));

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

		if(!bReturn)
			break;

		strModuleName.Format(_T("EcApi_CLRLib.dll"));

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

		if(!bReturn)
			break;
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, CMultiLanguageManager::GetString(ELanguageParameter__s_d_s_toload_s), GetClassNameStr(), GetObjectID(), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), strModuleName);

	return bReturn;
}

#endif