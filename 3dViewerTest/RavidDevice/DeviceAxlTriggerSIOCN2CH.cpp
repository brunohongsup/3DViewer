#include "stdafx.h"
#include "DeviceAxlTriggerSIOCN2CH.h"

#ifdef USING_DEVICE

#include "DeviceAxlDIO.h"
#include "DeviceAxlMotion.h"
#include "DeviceAxlMotionEthercat.h"

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

#include "../Libraries/Includes/AxlLib/AXHS.h"
#include "../Libraries/Includes/AxlLib/AXL.h"
#include "../Libraries/Includes/AxlLib/AXC.h"

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

IMPLEMENT_DYNAMIC(CDeviceAxlTriggerSIOCN2CH, CDeviceTrigger)

BEGIN_MESSAGE_MAP(CDeviceAxlTriggerSIOCN2CH, CDeviceTrigger)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static LPCTSTR g_lpszParamAxlTriggerSIOCN2CH[EDeviceParameterAxlTriggerSIOCN2CH_Count] =
{
	_T("DeviceID"),
	_T("SubUnitID"),
	_T("Unit Per Pulse"),
	_T("Trigger Function"),
	_T("Signal Setting"),
	_T("Encoder Input Method"),
	_T("Encoder Source"),
	_T("Encoder Reverse"),
	_T("Trigger Setting"),
	_T("Trigger Mode"),
	_T("Trigger Active Level"),
	_T("Direction Check"),
	_T("Pulse Width"),
	_T("Periodic"),
	_T("Start Position"),
	_T("End Position"),
	_T("Trigger Period"),
	_T("Open No Reset")
};

static LPCTSTR g_lpszAxlTriggerSIOCN2CHTriggerFunction[EDeviceAxlTriggerSIOCN2CTriggerFunction_Count] =
{
	_T("Period and Absolute"),
};

static LPCTSTR g_lpszAxlTriggerSIOCN2CHEncoderInputMethod[EDeviceAxlTriggerSIOCN2CHEncoderInputMethod_Count] =
{
	_T("UpDonw"),
	_T("Sqr1"),
	_T("Sqr2"),
	_T("Sqr4"),
};

static LPCTSTR g_lpszAxlTriggerSIOCN2CHEncoderSource[EDeviceAxlTriggerSIOCN2CHEncoderSource_Count] =
{
	_T("AB"),
	_T("Z"),
};

static LPCTSTR g_lpszAxlTriggerSIOCN2CHDirectionMethod[EDeviceAxlTriggerSIOCN2CHTriggerDirectionCheck_Count] =
{
	_T("Both"),
	_T("CW"),
	_T("CCW"),
};

static LPCTSTR g_lpszAxlTriggerSIOCN2CHLevelMethod[EDeviceAxlTriggerSIOCN2CHActiveLevel_Count] =
{
	_T("LOW"),
	_T("HIGH"),
};

static LPCTSTR g_lpszAxlTriggerSIOCN2CHTriggerMode[EDeviceAxlTriggerSIOCN2CHTriggerMode_Count] =
{
	_T("Absolute"),
	_T("Periodic"),
};

static LPCTSTR g_lpszSwitch[EDeviceAxlTriggerSIOCN2CHSwitch_Count] =
{
	_T("Off"),
	_T("On"),
};

CDeviceAxlTriggerSIOCN2CH::CDeviceAxlTriggerSIOCN2CH()
{
}


CDeviceAxlTriggerSIOCN2CH::~CDeviceAxlTriggerSIOCN2CH()
{	
}

EDeviceInitializeResult CDeviceAxlTriggerSIOCN2CH::Initialize()
{
	EDeviceInitializeResult eReturn = EDeviceInitializeResult_UnknownError;

	CString strMessage, strStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("AxlTriggerSIOCN2CH"));

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

		int nChannelID = _ttoi(GetSubUnitID());

		DWORD dwStatus = STATUS_NOTEXIST;
		if(AxcInfoIsCNTModule(&dwStatus) != AXT_RT_SUCCESS)
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

		long i32ChannelCount = 0;
		if(AxcInfoGetTotalChannelCount(&i32ChannelCount) != AXT_RT_SUCCESS)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Channel Count"));
			eReturn = EDeviceInitializeResult_NotFoundDeviceError;
			break;
		}

		if(0 > nChannelID || i32ChannelCount <= nChannelID)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_DoesntsupporttheDeviceID);
			eReturn = EDeviceInitializeResult_NotInitializeMotionError;
			break;
		}

		double dblUnitperPulse = 0.;
		if(GetUnitPerPulse(&dblUnitperPulse))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Unit Per Pulse"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(AxcMotSetMoveUnitPerPulse(nChannelID, dblUnitperPulse) != AXT_RT_SUCCESS)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Unit Per Pulse"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		bool bEncoderReverse = false;
		if(GetEncoderReverse(&bEncoderReverse))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Encoder Reverse"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(AxcSignalSetEncReverse(nChannelID, bEncoderReverse) != AXT_RT_SUCCESS)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Encoder Reverse"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		EDeviceAxlTriggerSIOCN2CHEncoderInputMethod eEncoderInputMethod = EDeviceAxlTriggerSIOCN2CHEncoderInputMethod_Count;
		if(GetEncoderInputMethod(&eEncoderInputMethod))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Encoder Input Method"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(AxcSignalSetEncInputMethod(nChannelID, eEncoderInputMethod) != AXT_RT_SUCCESS)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Encoder Input Method"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		EDeviceAxlTriggerSIOCN2CHEncoderSource eEncoderSource = EDeviceAxlTriggerSIOCN2CHEncoderSource_Count;
		if(GetEncoderSource(&eEncoderSource))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Encoder Source"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(AxcSignalSetEncSource(nChannelID, eEncoderSource) != AXT_RT_SUCCESS)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Encoder Source"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}
		
		EDeviceAxlTriggerSIOCN2CHTriggerMode eTrigMode = EDeviceAxlTriggerSIOCN2CHTriggerMode_Count;
		if(GetTriggerMode(&eTrigMode))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TriggerMode"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(AxcTriggerSetFunction(nChannelID, eTrigMode) != AXT_RT_SUCCESS)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TriggerMode"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		EDeviceAxlTriggerSIOCN2CHActiveLevel eActiveLevel = EDeviceAxlTriggerSIOCN2CHActiveLevel_Count;
		if(GetTriggerActiveLevel(&eActiveLevel))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Active Level"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(AxcTriggerSetLevel(nChannelID, eActiveLevel) != AXT_RT_SUCCESS)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Active Level"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}
			   
		EDeviceAxlTriggerSIOCN2CHTriggerDirectionCheck eTriggerDirectionCheck = EDeviceAxlTriggerSIOCN2CHTriggerDirectionCheck_Count;
		if(GetTriggerDirectionCheck(&eTriggerDirectionCheck))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Direction Check"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(AxcTriggerSetDirectionCheck(nChannelID, eTriggerDirectionCheck) != AXT_RT_SUCCESS)
		{
			DWORD dwMan = 0;

			if(AxcTriggerReadAbsRamData(nChannelID, HpcCh1TriggerModeRead, &dwMan) != AXT_RT_SUCCESS)
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("AbsRamData"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			switch(eTriggerDirectionCheck)
			{
			case EDeviceAxlTriggerSIOCN2CHTriggerDirectionCheck_CW:
				{
					dwMan |= 0x02;
					dwMan &= (0xff - 0x04);
				}
				break;
			case EDeviceAxlTriggerSIOCN2CHTriggerDirectionCheck_CCW:
				{
					dwMan |= 0x02;
					dwMan |= 0x04;
				}
				break;
			case EDeviceAxlTriggerSIOCN2CHTriggerDirectionCheck_Both:
				{
					dwMan &= (0xff - 0x02);
					dwMan &= (0xff - 0x04);
				}
				break;
			}

			if(AxcTriggerWriteAbsRamData(nChannelID, HpcCh1TriggerModeWrite, dwMan) != AXT_RT_SUCCESS)
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("AbsRamData"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}
		}

		double dblPulseWidth = 0.;
		if(GetTriggerPulseWidth(&dblPulseWidth))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("PulseWidth"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(AxcTriggerSetTime(nChannelID, dblPulseWidth) != AXT_RT_SUCCESS)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("PulseWidth"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		switch(eTrigMode)
		{
		case EDeviceAxlTriggerSIOCN2CHTriggerMode_Periodic:
			{
				double dblStartPos = 0.;
				if(GetStartPosition(&dblStartPos))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("StartPosition"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(AxcTriggerSetBlockLowerPos(nChannelID, dblStartPos) != AXT_RT_SUCCESS)
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("StartPosition"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				double dblEndPos = 0.;
				if(GetEndPosition(&dblEndPos))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("EndPosition"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(AxcTriggerSetBlockUpperPos(nChannelID, dblEndPos) != AXT_RT_SUCCESS)
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("EndPosition"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				double dblPeriod = 0.;
				if(GetTriggerPeriod(&dblPeriod))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TriggerPeriod"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(AxcTriggerSetPosPeriod(nChannelID, dblPeriod) != AXT_RT_SUCCESS)
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TriggerPeriod"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}
			}
			break;
		case EDeviceAxlTriggerSIOCN2CHTriggerMode_Absolute:
			break;
		default:
			break;
		}

		if(AxcStatusSetActPos(nChannelID, 0.) != AXT_RT_SUCCESS)
		{
			m_bIsInitialized = false;

			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ActualPosition"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

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

EDeviceTerminateResult CDeviceAxlTriggerSIOCN2CH::Terminate()
{
	CString strMessage;

	SetStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("AxlTriggerSIOCN2CH"));

		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);

		return EDeviceTerminateResult_NotFoundApiError;
	}

	EDeviceTerminateResult eReturn = EDeviceTerminateResult_UnknownError;

	do
	{
		ResetCounter();

		DisableTrigger();

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

		m_bIsInitialized = false;

		SetMessage(CMultiLanguageManager::GetString(ELanguageParameter_TheDevicehasbeensuccessfullyterminated));

		CEventHandlerManager::BroadcastOnDeviceTerminated(this);
	}
	while(false);

	return eReturn;
}

bool CDeviceAxlTriggerSIOCN2CH::LoadSettings()
{
	bool bReturn = false;

	m_vctParameterFieldConfigurations.clear();

	do
	{
		AddParameterFieldConfigurations(EDeviceParameterAxlTriggerSIOCN2CH_DeviceID, g_lpszParamAxlTriggerSIOCN2CH[EDeviceParameterAxlTriggerSIOCN2CH_DeviceID], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterAxlTriggerSIOCN2CH_SubUnitID, g_lpszParamAxlTriggerSIOCN2CH[EDeviceParameterAxlTriggerSIOCN2CH_SubUnitID], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterAxlTriggerSIOCN2CH_OpenNoReset, g_lpszParamAxlTriggerSIOCN2CH[EDeviceParameterAxlTriggerSIOCN2CH_OpenNoReset], _T("0"), EParameterFieldType_Check, nullptr, _T("Initializes the hardware when initializing the device."), 0);

		AddParameterFieldConfigurations(EDeviceParameterAxlTriggerSIOCN2CH_UnitPerPulse, g_lpszParamAxlTriggerSIOCN2CH[EDeviceParameterAxlTriggerSIOCN2CH_UnitPerPulse], _T("0.001000"), EParameterFieldType_Edit, nullptr, nullptr, 0);

		AddParameterFieldConfigurations(EDeviceParameterAxlTriggerSIOCN2CH_TriggerFunction, g_lpszParamAxlTriggerSIOCN2CH[EDeviceParameterAxlTriggerSIOCN2CH_TriggerFunction], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxlTriggerSIOCN2CHTriggerFunction, EDeviceAxlTriggerSIOCN2CTriggerFunction_Count), nullptr, 0);

		AddParameterFieldConfigurations(EDeviceParameterAxlTriggerSIOCN2CH_SignalSetting, g_lpszParamAxlTriggerSIOCN2CH[EDeviceParameterAxlTriggerSIOCN2CH_SignalSetting], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		{
			AddParameterFieldConfigurations(EDeviceParameterAxlTriggerSIOCN2CH_EncoderInputMethod, g_lpszParamAxlTriggerSIOCN2CH[EDeviceParameterAxlTriggerSIOCN2CH_EncoderInputMethod], _T("3"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxlTriggerSIOCN2CHEncoderInputMethod, EDeviceAxlTriggerSIOCN2CHEncoderInputMethod_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterAxlTriggerSIOCN2CH_EncoderSource, g_lpszParamAxlTriggerSIOCN2CH[EDeviceParameterAxlTriggerSIOCN2CH_EncoderSource], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxlTriggerSIOCN2CHEncoderSource, EDeviceAxlTriggerSIOCN2CHEncoderSource_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterAxlTriggerSIOCN2CH_EncoderReverse, g_lpszParamAxlTriggerSIOCN2CH[EDeviceParameterAxlTriggerSIOCN2CH_EncoderReverse], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 1);

		}

		AddParameterFieldConfigurations(EDeviceParameterAxlTriggerSIOCN2CH_TriggerSetting, g_lpszParamAxlTriggerSIOCN2CH[EDeviceParameterAxlTriggerSIOCN2CH_TriggerSetting], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		{
			AddParameterFieldConfigurations(EDeviceParameterAxlTriggerSIOCN2CH_TriggerMode, g_lpszParamAxlTriggerSIOCN2CH[EDeviceParameterAxlTriggerSIOCN2CH_TriggerMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxlTriggerSIOCN2CHTriggerMode, EDeviceAxlTriggerSIOCN2CHTriggerMode_Count), _T("Board Type"), 1);

			AddParameterFieldConfigurations(EDeviceParameterAxlTriggerSIOCN2CH_TriggerActiveLevel, g_lpszParamAxlTriggerSIOCN2CH[EDeviceParameterAxlTriggerSIOCN2CH_TriggerActiveLevel], _T("1"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxlTriggerSIOCN2CHLevelMethod, EDeviceAxlTriggerSIOCN2CHActiveLevel_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterAxlTriggerSIOCN2CH_TriggerDirectionCheck, g_lpszParamAxlTriggerSIOCN2CH[EDeviceParameterAxlTriggerSIOCN2CH_TriggerDirectionCheck], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAxlTriggerSIOCN2CHDirectionMethod, EDeviceAxlTriggerSIOCN2CHTriggerDirectionCheck_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterAxlTriggerSIOCN2CH_TriggerPulseWidth, g_lpszParamAxlTriggerSIOCN2CH[EDeviceParameterAxlTriggerSIOCN2CH_TriggerPulseWidth], _T("200"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterAxlTriggerSIOCN2CH_Periodic, g_lpszParamAxlTriggerSIOCN2CH[EDeviceParameterAxlTriggerSIOCN2CH_Periodic], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
			{
				AddParameterFieldConfigurations(EDeviceParameterAxlTriggerSIOCN2CH_StartPosition, g_lpszParamAxlTriggerSIOCN2CH[EDeviceParameterAxlTriggerSIOCN2CH_StartPosition], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);

				AddParameterFieldConfigurations(EDeviceParameterAxlTriggerSIOCN2CH_EndPosition, g_lpszParamAxlTriggerSIOCN2CH[EDeviceParameterAxlTriggerSIOCN2CH_EndPosition], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);

				AddParameterFieldConfigurations(EDeviceParameterAxlTriggerSIOCN2CH_TriggerPeriod, g_lpszParamAxlTriggerSIOCN2CH[EDeviceParameterAxlTriggerSIOCN2CH_TriggerPeriod], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
			}
		}

		bReturn = true;
	}
	while(false);

	return bReturn & __super::LoadSettings();
}

bool CDeviceAxlTriggerSIOCN2CH::EnableTrigger()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		int nChannelID = _ttoi(GetSubUnitID());

		if(AxcTriggerSetEnable(nChannelID, true) != AXT_RT_SUCCESS)
			break;

		m_bTriggerEnabled = true;

		bReturn = true;
	}
	while(false);

	if(IsTriggerEnabled())
		SetMessage(CMultiLanguageManager::GetString(ELanguageParameter_Triggerisenabled));

	return bReturn;
}

bool CDeviceAxlTriggerSIOCN2CH::DisableTrigger()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		int nChannelID = _ttoi(GetSubUnitID());

		if(AxcTriggerSetEnable(nChannelID, false) != AXT_RT_SUCCESS)
			break;

		m_bTriggerEnabled = false;

		bReturn = true;
	}
	while(false);

	if(!IsTriggerEnabled())
		SetMessage(CMultiLanguageManager::GetString(ELanguageParameter_Triggerisdisabled));

	return bReturn;
}

bool CDeviceAxlTriggerSIOCN2CH::IsTriggerEnabled()
{
	return m_bTriggerEnabled;
}

long CDeviceAxlTriggerSIOCN2CH::GetEncoderCount()
{
	long nReturn = 0;

	do
	{
		if(!IsInitialized())
			break;

		int nChannelID = _ttoi(GetSubUnitID());

		double dblCount = 0.;

		if(AxcStatusGetActPos(nChannelID, &dblCount) != AXT_RT_SUCCESS)
			break;

		nReturn = dblCount;
	}
	while(false);

	return nReturn;
}

long CDeviceAxlTriggerSIOCN2CH::GetTriggerCount()
{
	return false;
}

bool CDeviceAxlTriggerSIOCN2CH::ResetCounter()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		int nChannelID = _ttoi(GetSubUnitID());

		if(AxcStatusSetActPos(nChannelID, 0.) != AXT_RT_SUCCESS)
			break;

		bReturn = true;
	}
	while(false);

	if(bReturn)
		SetMessage(CMultiLanguageManager::GetString(ELanguageParameter_Counterisreset));

	return bReturn;
}

EAxlTriggerSIOCN2CHGetFunction CDeviceAxlTriggerSIOCN2CH::GetUnitPerPulse(double* pParam)
{
	EAxlTriggerSIOCN2CHGetFunction eReturn = EAxlTriggerSIOCN2CHGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAxlTriggerSIOCN2CHGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterAxlTriggerSIOCN2CH_UnitPerPulse));

		eReturn = EAxlTriggerSIOCN2CHGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAxlTriggerSIOCN2CHSetFunction CDeviceAxlTriggerSIOCN2CH::SetUnitPerPulse(double dblParam)
{
	EAxlTriggerSIOCN2CHSetFunction eReturn = EAxlTriggerSIOCN2CHSetFunction_UnknownError;

	EDeviceParameterAxlTriggerSIOCN2CH eSaveID = EDeviceParameterAxlTriggerSIOCN2CH_UnitPerPulse;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			int nChannelID = _ttoi(GetSubUnitID());


			if(AxcMotSetMoveUnitPerPulse(nChannelID, dblParam) != AXT_RT_SUCCESS)
			{
				eReturn = EAxlTriggerSIOCN2CHSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EAxlTriggerSIOCN2CHSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAxlTriggerSIOCN2CHSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAxlTriggerSIOCN2CHSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlTriggerSIOCN2CH[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAxlTriggerSIOCN2CHGetFunction CDeviceAxlTriggerSIOCN2CH::GetTriggerFunction(EDeviceAxlTriggerSIOCN2CTriggerFunction* pParam)
{
	EAxlTriggerSIOCN2CHGetFunction eReturn = EAxlTriggerSIOCN2CHGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAxlTriggerSIOCN2CHGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxlTriggerSIOCN2CTriggerFunction)_ttoi(GetParamValue(EDeviceParameterAxlTriggerSIOCN2CH_TriggerFunction));

		eReturn = EAxlTriggerSIOCN2CHGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAxlTriggerSIOCN2CHSetFunction CDeviceAxlTriggerSIOCN2CH::SetTriggerFunction(EDeviceAxlTriggerSIOCN2CTriggerFunction eParam)
{
	EAxlTriggerSIOCN2CHSetFunction eReturn = EAxlTriggerSIOCN2CHSetFunction_UnknownError;

	EDeviceParameterAxlTriggerSIOCN2CH eSaveID = EDeviceParameterAxlTriggerSIOCN2CH_TriggerFunction;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxlTriggerSIOCN2CTriggerFunction_Count)
		{
			eReturn = EAxlTriggerSIOCN2CHSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			int nChannelID = _ttoi(GetSubUnitID());

			if(AxcTriggerSetFunction(nChannelID, eParam) != AXT_RT_SUCCESS)
			{
				eReturn = EAxlTriggerSIOCN2CHSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EAxlTriggerSIOCN2CHSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAxlTriggerSIOCN2CHSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAxlTriggerSIOCN2CHSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlTriggerSIOCN2CH[eSaveID], g_lpszAxlTriggerSIOCN2CHTriggerFunction[nPreValue], g_lpszAxlTriggerSIOCN2CHTriggerFunction[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAxlTriggerSIOCN2CHGetFunction CDeviceAxlTriggerSIOCN2CH::GetEncoderInputMethod(EDeviceAxlTriggerSIOCN2CHEncoderInputMethod* pParam)
{
	EAxlTriggerSIOCN2CHGetFunction eReturn = EAxlTriggerSIOCN2CHGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAxlTriggerSIOCN2CHGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxlTriggerSIOCN2CHEncoderInputMethod)_ttoi(GetParamValue(EDeviceParameterAxlTriggerSIOCN2CH_EncoderInputMethod));

		eReturn = EAxlTriggerSIOCN2CHGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAxlTriggerSIOCN2CHSetFunction CDeviceAxlTriggerSIOCN2CH::SetEncoderInputMethod(EDeviceAxlTriggerSIOCN2CHEncoderInputMethod eParam)
{
	EAxlTriggerSIOCN2CHSetFunction eReturn = EAxlTriggerSIOCN2CHSetFunction_UnknownError;

	EDeviceParameterAxlTriggerSIOCN2CH eSaveID = EDeviceParameterAxlTriggerSIOCN2CH_EncoderInputMethod;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxlTriggerSIOCN2CHEncoderInputMethod_Count)
		{
			eReturn = EAxlTriggerSIOCN2CHSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			int nChannelID = _ttoi(GetSubUnitID());

			if(AxcSignalSetEncInputMethod(nChannelID, (int)eParam) != AXT_RT_SUCCESS)
			{
				eReturn = EAxlTriggerSIOCN2CHSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EAxlTriggerSIOCN2CHSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAxlTriggerSIOCN2CHSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAxlTriggerSIOCN2CHSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlTriggerSIOCN2CH[eSaveID], g_lpszAxlTriggerSIOCN2CHEncoderInputMethod[nPreValue], g_lpszAxlTriggerSIOCN2CHEncoderInputMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAxlTriggerSIOCN2CHGetFunction CDeviceAxlTriggerSIOCN2CH::GetEncoderSource(EDeviceAxlTriggerSIOCN2CHEncoderSource* pParam)
{
	EAxlTriggerSIOCN2CHGetFunction eReturn = EAxlTriggerSIOCN2CHGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAxlTriggerSIOCN2CHGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxlTriggerSIOCN2CHEncoderSource)_ttoi(GetParamValue(EDeviceParameterAxlTriggerSIOCN2CH_EncoderSource));

		eReturn = EAxlTriggerSIOCN2CHGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAxlTriggerSIOCN2CHSetFunction CDeviceAxlTriggerSIOCN2CH::SetEncoderSource(EDeviceAxlTriggerSIOCN2CHEncoderSource eParam)
{
	EAxlTriggerSIOCN2CHSetFunction eReturn = EAxlTriggerSIOCN2CHSetFunction_UnknownError;

	EDeviceParameterAxlTriggerSIOCN2CH eSaveID = EDeviceParameterAxlTriggerSIOCN2CH_EncoderSource;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxlTriggerSIOCN2CHEncoderSource_Count)
		{
			eReturn = EAxlTriggerSIOCN2CHSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			int nChannelID = _ttoi(GetSubUnitID());

			if(AxcSignalSetEncSource(nChannelID, (int)eParam) != AXT_RT_SUCCESS)
			{
				eReturn = EAxlTriggerSIOCN2CHSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EAxlTriggerSIOCN2CHSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAxlTriggerSIOCN2CHSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAxlTriggerSIOCN2CHSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlTriggerSIOCN2CH[eSaveID], g_lpszAxlTriggerSIOCN2CHEncoderSource[nPreValue], g_lpszAxlTriggerSIOCN2CHEncoderSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAxlTriggerSIOCN2CHGetFunction CDeviceAxlTriggerSIOCN2CH::GetEncoderReverse(bool* pParam)
{
	EAxlTriggerSIOCN2CHGetFunction eReturn = EAxlTriggerSIOCN2CHGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAxlTriggerSIOCN2CHGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAxlTriggerSIOCN2CH_EncoderReverse));

		eReturn = EAxlTriggerSIOCN2CHGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAxlTriggerSIOCN2CHSetFunction CDeviceAxlTriggerSIOCN2CH::SetEncoderReverse(bool bParam)
{
	EAxlTriggerSIOCN2CHSetFunction eReturn = EAxlTriggerSIOCN2CHSetFunction_UnknownError;

	EDeviceParameterAxlTriggerSIOCN2CH eSaveID = EDeviceParameterAxlTriggerSIOCN2CH_EncoderReverse;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			int nChannelID = _ttoi(GetSubUnitID());

			if(AxcSignalSetEncReverse(nChannelID, bParam) != AXT_RT_SUCCESS)
			{
				eReturn = EAxlTriggerSIOCN2CHSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EAxlTriggerSIOCN2CHSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAxlTriggerSIOCN2CHSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAxlTriggerSIOCN2CHSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlTriggerSIOCN2CH[eSaveID], g_lpszSwitch[nPreValue], g_lpszSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAxlTriggerSIOCN2CHGetFunction CDeviceAxlTriggerSIOCN2CH::GetTriggerMode(EDeviceAxlTriggerSIOCN2CHTriggerMode* pParam)
{
	EAxlTriggerSIOCN2CHGetFunction eReturn = EAxlTriggerSIOCN2CHGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAxlTriggerSIOCN2CHGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxlTriggerSIOCN2CHTriggerMode)_ttoi(GetParamValue(EDeviceParameterAxlTriggerSIOCN2CH_TriggerMode));

		eReturn = EAxlTriggerSIOCN2CHGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAxlTriggerSIOCN2CHSetFunction CDeviceAxlTriggerSIOCN2CH::SetTriggerMode(EDeviceAxlTriggerSIOCN2CHTriggerMode eParam)
{
	EAxlTriggerSIOCN2CHSetFunction eReturn = EAxlTriggerSIOCN2CHSetFunction_UnknownError;

	EDeviceParameterAxlTriggerSIOCN2CH eSaveID = EDeviceParameterAxlTriggerSIOCN2CH_TriggerMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxlTriggerSIOCN2CHTriggerMode_Count)
		{
			eReturn = EAxlTriggerSIOCN2CHSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			int nChannelID = _ttoi(GetSubUnitID());

			if(AxcTriggerSetFunction(nChannelID, eParam) != AXT_RT_SUCCESS)
			{
				eReturn = EAxlTriggerSIOCN2CHSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EAxlTriggerSIOCN2CHSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAxlTriggerSIOCN2CHSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAxlTriggerSIOCN2CHSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlTriggerSIOCN2CH[eSaveID], g_lpszAxlTriggerSIOCN2CHTriggerMode[nPreValue], g_lpszAxlTriggerSIOCN2CHTriggerMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAxlTriggerSIOCN2CHGetFunction CDeviceAxlTriggerSIOCN2CH::GetTriggerActiveLevel(EDeviceAxlTriggerSIOCN2CHActiveLevel* pParam)
{
	EAxlTriggerSIOCN2CHGetFunction eReturn = EAxlTriggerSIOCN2CHGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAxlTriggerSIOCN2CHGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxlTriggerSIOCN2CHActiveLevel)_ttoi(GetParamValue(EDeviceParameterAxlTriggerSIOCN2CH_TriggerActiveLevel));

		eReturn = EAxlTriggerSIOCN2CHGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAxlTriggerSIOCN2CHSetFunction CDeviceAxlTriggerSIOCN2CH::SetTriggerActiveLevel(EDeviceAxlTriggerSIOCN2CHActiveLevel eParam)
{
	EAxlTriggerSIOCN2CHSetFunction eReturn = EAxlTriggerSIOCN2CHSetFunction_UnknownError;

	EDeviceParameterAxlTriggerSIOCN2CH eSaveID = EDeviceParameterAxlTriggerSIOCN2CH_TriggerActiveLevel;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxlTriggerSIOCN2CHActiveLevel_Count)
		{
			eReturn = EAxlTriggerSIOCN2CHSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			int nChannelID = _ttoi(GetSubUnitID());

			if(AxcTriggerSetLevel(nChannelID, eParam) != AXT_RT_SUCCESS)
			{
				eReturn = EAxlTriggerSIOCN2CHSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EAxlTriggerSIOCN2CHSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAxlTriggerSIOCN2CHSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAxlTriggerSIOCN2CHSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlTriggerSIOCN2CH[eSaveID], g_lpszAxlTriggerSIOCN2CHLevelMethod[nPreValue], g_lpszAxlTriggerSIOCN2CHLevelMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAxlTriggerSIOCN2CHGetFunction CDeviceAxlTriggerSIOCN2CH::GetTriggerDirectionCheck(EDeviceAxlTriggerSIOCN2CHTriggerDirectionCheck* pParam)
{
	EAxlTriggerSIOCN2CHGetFunction eReturn = EAxlTriggerSIOCN2CHGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAxlTriggerSIOCN2CHGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceAxlTriggerSIOCN2CHTriggerDirectionCheck)_ttoi(GetParamValue(EDeviceParameterAxlTriggerSIOCN2CH_TriggerDirectionCheck));

		eReturn = EAxlTriggerSIOCN2CHGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAxlTriggerSIOCN2CHSetFunction CDeviceAxlTriggerSIOCN2CH::SetTriggerDirectionCheck(EDeviceAxlTriggerSIOCN2CHTriggerDirectionCheck eParam)
{
	EAxlTriggerSIOCN2CHSetFunction eReturn = EAxlTriggerSIOCN2CHSetFunction_UnknownError;

	EDeviceParameterAxlTriggerSIOCN2CH eSaveID = EDeviceParameterAxlTriggerSIOCN2CH_TriggerDirectionCheck;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceAxlTriggerSIOCN2CHTriggerDirectionCheck_Count)
		{
			eReturn = EAxlTriggerSIOCN2CHSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			int nChannelID = _ttoi(GetSubUnitID());


			if(AxcTriggerSetDirectionCheck(nChannelID, (int)eParam) != AXT_RT_SUCCESS)
			{
				DWORD dwMan = 0;

				if(AxcTriggerReadAbsRamData(nChannelID, HpcCh1TriggerModeRead, &dwMan) != AXT_RT_SUCCESS)
				{
					eReturn = EAxlTriggerSIOCN2CHSetFunction_ReadOnDeviceError;
					break;
				}

				switch(eParam)
				{
				case EDeviceAxlTriggerSIOCN2CHTriggerDirectionCheck_CW:
					{
						dwMan |= 0x02;
						dwMan &= (0xff - 0x04);
					}
					break;
				case EDeviceAxlTriggerSIOCN2CHTriggerDirectionCheck_CCW:
					{
						dwMan |= 0x02;
						dwMan |= 0x04;
					}
					break;
				case EDeviceAxlTriggerSIOCN2CHTriggerDirectionCheck_Both:
					{
						dwMan &= (0xff - 0x02);
						dwMan &= (0xff - 0x04);
					}
					break;
				}

				if(AxcTriggerWriteAbsRamData(nChannelID, HpcCh1TriggerModeWrite, dwMan) != AXT_RT_SUCCESS)
				{
					eReturn = EAxlTriggerSIOCN2CHSetFunction_WriteToDeviceError;
					break;
				}
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EAxlTriggerSIOCN2CHSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAxlTriggerSIOCN2CHSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAxlTriggerSIOCN2CHSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlTriggerSIOCN2CH[eSaveID], g_lpszAxlTriggerSIOCN2CHDirectionMethod[nPreValue], g_lpszAxlTriggerSIOCN2CHDirectionMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAxlTriggerSIOCN2CHGetFunction CDeviceAxlTriggerSIOCN2CH::GetTriggerPulseWidth(double* pParam)
{
	EAxlTriggerSIOCN2CHGetFunction eReturn = EAxlTriggerSIOCN2CHGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAxlTriggerSIOCN2CHGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterAxlTriggerSIOCN2CH_TriggerPulseWidth));

		eReturn = EAxlTriggerSIOCN2CHGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAxlTriggerSIOCN2CHSetFunction CDeviceAxlTriggerSIOCN2CH::SetTriggerPulseWidth(double dblParam)
{
	EAxlTriggerSIOCN2CHSetFunction eReturn = EAxlTriggerSIOCN2CHSetFunction_UnknownError;

	EDeviceParameterAxlTriggerSIOCN2CH eSaveID = EDeviceParameterAxlTriggerSIOCN2CH_TriggerPulseWidth;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			int nChannelID = _ttoi(GetSubUnitID());


			if(AxcTriggerSetTime(nChannelID, dblParam) != AXT_RT_SUCCESS)
			{
				eReturn = EAxlTriggerSIOCN2CHSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EAxlTriggerSIOCN2CHSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAxlTriggerSIOCN2CHSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAxlTriggerSIOCN2CHSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlTriggerSIOCN2CH[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAxlTriggerSIOCN2CHGetFunction CDeviceAxlTriggerSIOCN2CH::GetStartPosition(double* pParam)
{
	EAxlTriggerSIOCN2CHGetFunction eReturn = EAxlTriggerSIOCN2CHGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAxlTriggerSIOCN2CHGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterAxlTriggerSIOCN2CH_StartPosition));

		eReturn = EAxlTriggerSIOCN2CHGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAxlTriggerSIOCN2CHSetFunction CDeviceAxlTriggerSIOCN2CH::SetStartPosition(double dblParam)
{
	EAxlTriggerSIOCN2CHSetFunction eReturn = EAxlTriggerSIOCN2CHSetFunction_UnknownError;

	EDeviceParameterAxlTriggerSIOCN2CH eSaveID = EDeviceParameterAxlTriggerSIOCN2CH_StartPosition;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			EDeviceAxlTriggerSIOCN2CHTriggerMode eTrigMode = EDeviceAxlTriggerSIOCN2CHTriggerMode_Count;

			if(GetTriggerMode(&eTrigMode))
			{
				eReturn = EAxlTriggerSIOCN2CHSetFunction_ReadOnDatabaseError;
				break;
			}

			if(eTrigMode != EDeviceAxlTriggerSIOCN2CHTriggerMode_Periodic)
			{
				eReturn = EAxlTriggerSIOCN2CHSetFunction_NotSelectModeError;
				break;
			}

			int nChannelID = _ttoi(GetSubUnitID());


			if(AxcTriggerSetBlockLowerPos(nChannelID, dblParam) != AXT_RT_SUCCESS)
			{
				eReturn = EAxlTriggerSIOCN2CHSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EAxlTriggerSIOCN2CHSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAxlTriggerSIOCN2CHSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAxlTriggerSIOCN2CHSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlTriggerSIOCN2CH[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAxlTriggerSIOCN2CHGetFunction CDeviceAxlTriggerSIOCN2CH::GetEndPosition(double* pParam)
{
	EAxlTriggerSIOCN2CHGetFunction eReturn = EAxlTriggerSIOCN2CHGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAxlTriggerSIOCN2CHGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterAxlTriggerSIOCN2CH_EndPosition));

		eReturn = EAxlTriggerSIOCN2CHGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAxlTriggerSIOCN2CHSetFunction CDeviceAxlTriggerSIOCN2CH::SetEndPosition(double dblParam)
{
	EAxlTriggerSIOCN2CHSetFunction eReturn = EAxlTriggerSIOCN2CHSetFunction_UnknownError;

	EDeviceParameterAxlTriggerSIOCN2CH eSaveID = EDeviceParameterAxlTriggerSIOCN2CH_EndPosition;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			EDeviceAxlTriggerSIOCN2CHTriggerMode eTrigMode = EDeviceAxlTriggerSIOCN2CHTriggerMode_Count;

			if(GetTriggerMode(&eTrigMode))
			{
				eReturn = EAxlTriggerSIOCN2CHSetFunction_ReadOnDatabaseError;
				break;
			}

			if(eTrigMode != EDeviceAxlTriggerSIOCN2CHTriggerMode_Periodic)
			{
				eReturn = EAxlTriggerSIOCN2CHSetFunction_NotSelectModeError;
				break;
			}

			int nChannelID = _ttoi(GetSubUnitID());

			if(AxcTriggerSetBlockUpperPos(nChannelID, dblParam) != AXT_RT_SUCCESS)
			{
				eReturn = EAxlTriggerSIOCN2CHSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EAxlTriggerSIOCN2CHSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAxlTriggerSIOCN2CHSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAxlTriggerSIOCN2CHSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlTriggerSIOCN2CH[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EAxlTriggerSIOCN2CHGetFunction CDeviceAxlTriggerSIOCN2CH::GetTriggerPeriod(double* pParam)
{
	EAxlTriggerSIOCN2CHGetFunction eReturn = EAxlTriggerSIOCN2CHGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EAxlTriggerSIOCN2CHGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterAxlTriggerSIOCN2CH_TriggerPeriod));

		eReturn = EAxlTriggerSIOCN2CHGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EAxlTriggerSIOCN2CHSetFunction CDeviceAxlTriggerSIOCN2CH::SetTriggerPeriod(double dblParam)
{
	EAxlTriggerSIOCN2CHSetFunction eReturn = EAxlTriggerSIOCN2CHSetFunction_UnknownError;

	EDeviceParameterAxlTriggerSIOCN2CH eSaveID = EDeviceParameterAxlTriggerSIOCN2CH_TriggerPeriod;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			EDeviceAxlTriggerSIOCN2CHTriggerMode eTrigMode = EDeviceAxlTriggerSIOCN2CHTriggerMode_Count;

			if(GetTriggerMode(&eTrigMode))
			{
				eReturn = EAxlTriggerSIOCN2CHSetFunction_ReadOnDatabaseError;
				break;
			}

			if(eTrigMode != EDeviceAxlTriggerSIOCN2CHTriggerMode_Periodic)
			{
				eReturn = EAxlTriggerSIOCN2CHSetFunction_NotSelectModeError;
				break;
			}

			int nChannelID = _ttoi(GetSubUnitID());

			if(AxcTriggerSetPosPeriod(nChannelID, dblParam) != AXT_RT_SUCCESS)
			{
				eReturn = EAxlTriggerSIOCN2CHSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EAxlTriggerSIOCN2CHSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EAxlTriggerSIOCN2CHSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EAxlTriggerSIOCN2CHSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlTriggerSIOCN2CH[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceAxlTriggerSIOCN2CH::ClearAbsolutePosition()
{
	bool bReturn = false;

	do 
	{
		if(!IsInitialized())
			break;

		int nChannelID = _ttoi(GetSubUnitID());

		DWORD	dwABSPos[512];
		memset(dwABSPos, 0x00, sizeof(DWORD) * 512);

		if(AxcTriggerSetAbs(nChannelID, 0, dwABSPos, 0) != AXT_RT_SUCCESS)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxlTriggerSIOCN2CH::SetAbsoluteTriggerPosition(std::vector<DWORD>& refvctPositions)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		int nChannelID = _ttoi(GetSubUnitID());

		DWORD	dwABSPos[512];
		memset(dwABSPos, 0x00, sizeof(DWORD) * 512);

		const DWORD dwCopySize = __min(512, (DWORD)refvctPositions.size());
		for(DWORD i = 0; i < dwCopySize; ++i)
			dwABSPos[i] = refvctPositions[i];

		if(AxcTriggerSetAbs(nChannelID, dwCopySize, dwABSPos, 0) != AXT_RT_SUCCESS)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxlTriggerSIOCN2CH::SetAbsoluteTriggerPosition(std::vector<DWORD>* pvctPositions)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(!pvctPositions)
			break;

		int nChannelID = _ttoi(GetSubUnitID());

		DWORD	dwABSPos[512];
		memset(dwABSPos, 0x00, sizeof(DWORD) * 512);

		const DWORD dwCopySize = __min(512, (DWORD)pvctPositions->size());
		for(DWORD i = 0; i < dwCopySize; ++i)
			dwABSPos[i] = pvctPositions->at(i);

		if(AxcTriggerSetAbs(nChannelID, dwCopySize, dwABSPos, 0) != AXT_RT_SUCCESS)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceAxlTriggerSIOCN2CH::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = true;

	bool bFoundID = true;

	do
	{
		switch(nParam)
		{
		case EDeviceParameterAxlTriggerSIOCN2CH_DeviceID:
			bReturn = !SetDeviceID(strValue);
			break;
		case EDeviceParameterAxlTriggerSIOCN2CH_SubUnitID:
			bReturn = !SetSubUnitID(strValue);
			break;
		case EDeviceParameterAxlTriggerSIOCN2CH_UnitPerPulse:
			bReturn = !SetUnitPerPulse(_ttof(strValue));
			break;
		case EDeviceParameterAxlTriggerSIOCN2CH_TriggerFunction:
			bReturn = !SetTriggerFunction((EDeviceAxlTriggerSIOCN2CTriggerFunction)_ttoi(strValue));
			break;
		case EDeviceParameterAxlTriggerSIOCN2CH_EncoderInputMethod:
			bReturn = !SetEncoderInputMethod((EDeviceAxlTriggerSIOCN2CHEncoderInputMethod)_ttoi(strValue));
			break;
		case EDeviceParameterAxlTriggerSIOCN2CH_EncoderSource:
			bReturn = !SetEncoderSource((EDeviceAxlTriggerSIOCN2CHEncoderSource)_ttoi(strValue));
			break;
		case EDeviceParameterAxlTriggerSIOCN2CH_EncoderReverse:
			bReturn = !SetEncoderReverse(_ttoi(strValue));
			break;
		case EDeviceParameterAxlTriggerSIOCN2CH_TriggerMode:
			bReturn = !SetTriggerMode((EDeviceAxlTriggerSIOCN2CHTriggerMode)_ttoi(strValue));
			break;
		case EDeviceParameterAxlTriggerSIOCN2CH_TriggerActiveLevel:
			bReturn = !SetTriggerActiveLevel((EDeviceAxlTriggerSIOCN2CHActiveLevel)_ttoi(strValue));
			break;
		case EDeviceParameterAxlTriggerSIOCN2CH_TriggerDirectionCheck:
			bReturn = !SetTriggerDirectionCheck((EDeviceAxlTriggerSIOCN2CHTriggerDirectionCheck)_ttoi(strValue));
			break;
		case EDeviceParameterAxlTriggerSIOCN2CH_TriggerPulseWidth:
			bReturn = !SetTriggerPulseWidth(_ttof(strValue));
			break;
		case EDeviceParameterAxlTriggerSIOCN2CH_StartPosition:
			bReturn = !SetStartPosition(_ttof(strValue));
			break;
		case EDeviceParameterAxlTriggerSIOCN2CH_EndPosition:
			bReturn = !SetEndPosition(_ttof(strValue));
			break;
		case EDeviceParameterAxlTriggerSIOCN2CH_TriggerPeriod:
			bReturn = !SetTriggerPeriod(_ttof(strValue));
			break;
		case EDeviceParameterAxlTriggerSIOCN2CH_OpenNoReset:
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
		CString strMessage;

		strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtofindtheparameter);

		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);
	}

	return bReturn;
}

bool CDeviceAxlTriggerSIOCN2CH::DoesModuleExist()
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

EGetFunction CDeviceAxlTriggerSIOCN2CH::GetOpenNoHardware(bool * pParam)
{
	EGetFunction eReturn = EGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterAxlTriggerSIOCN2CH_OpenNoReset));

		eReturn = EGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ESetFunction CDeviceAxlTriggerSIOCN2CH::SetOpenNoHardware(bool bParam)
{
	ESetFunction eReturn = ESetFunction_UnknownError;

	EDeviceParameterAxlTriggerSIOCN2CH eSaveNum = EDeviceParameterAxlTriggerSIOCN2CH_OpenNoReset;

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamAxlTriggerSIOCN2CH[eSaveNum], g_lpszSwitch[bPreValue], g_lpszSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

#endif