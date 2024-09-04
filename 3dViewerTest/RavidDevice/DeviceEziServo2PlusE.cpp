#include "stdafx.h"

#include "DeviceEziServo2PlusE.h"

#ifdef USING_DEVICE

#include "../RavidCore/RavidDatabase.h"
#include "../RavidCore/RavidRecordset.h"
#include "../RavidCore/RavidPoint.h"

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

#include "../Libraries/Includes/EziServo2PlusE/FAS_EziMOTIONPlusE.h"

// ezimotionpluse.dll
#pragma comment(lib, COMMONLIB_PREFIX "EziServo2PlusE/EziMOTIONPlusE.lib")

using namespace Ravid;
using namespace Ravid::Device;
using namespace Ravid::Database;
using namespace Ravid::Framework;
using namespace Ravid::Algorithms;
using namespace Ravid::Miscellaneous;

enum
{
	OutputPort = 9,
	InputPort = 9,
};

enum
{
	ReadOutputShift = 15,
	ReadInputHighShift = 26,
	ReadInputLowShift = 3,
};

IMPLEMENT_DYNAMIC(CDeviceEziServo2PlusE, CDeviceMotion)

BEGIN_MESSAGE_MAP(CDeviceEziServo2PlusE, CDeviceMotion)
	ON_COMMAND_RANGE(EDeviceAddEnumeratedControlID_OutputLedStart, EDeviceAddEnumeratedControlID_OutputLedEnd, OnBnClickedOutLed)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

 static LPCTSTR g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_Count] =
 {
	_T("DeviceID"),
	_T("IP Address"),
	_T("Motion Type"),
	_T("Servo2 Param"),
	_T("[S2]Pulse Per Revolution"),
	_T("[S2]Axis Max Speed"),
	_T("[S2]Axis Start Speed"),
	_T("[S2]Axis Acc Time"),
	_T("[S2]Axis Dec Time"),
	_T("[S2]Speed Override"),
	_T("[S2]Jog Speed"),
	_T("[S2]Jog Start Speed"),
	_T("[S2]Jog Acc Dec Time"),
	_T("[S2]SW Limit Plus Value"),
	_T("[S2]SW Limit Minus Value"),
	_T("[S2]SW Limit Stop Method"),
	_T("[S2]HW Limit Stop Method"),
	_T("[S2]Limit Sensor Logic"),
	_T("[S2]Org Speed"),
	_T("[S2]Org Search Speed"),
	_T("[S2]Org Acc Dec Time"),
	_T("[S2]Org Method"),
	_T("[S2]Org Dir"),
	_T("[S2]Org OffSet"),
	_T("[S2]Org Position Set"),
	_T("[S2]Org Sensor Logic"),
	_T("[S2]Position Loop Gain"),
	_T("[S2]Inpos Value"),
	_T("[S2]Pos Tracking Limit"),
	_T("[S2]Motion Dir"),
	_T("[S2]Limit Sensor Dir"),
	_T("[S2]Org Torque Ratio"),
	_T("[S2]Pos Error Overflow Limit"),
	_T("[S2]Brake Delay Time"),
	_T("MotionLink2 Param"),
	_T("[ML2]Encoder Multiply"),
	_T("[ML2]Axis Max Speed"),
	_T("[ML2]Axis Start Speed"),
	_T("[ML2]Axis Acc Time"),
	_T("[ML2]Axis Dec Time"),
	_T("[ML2]Speed Override"),
	_T("[ML2]Jog Speed"),
	_T("[ML2]Jog Start Speed"),
	_T("[ML2]Jog Acc Dec Time"),
	_T("[ML2]SW Limit Plus Value"),
	_T("[ML2]SW Limit Minus Value"),
	_T("[ML2]SW Limit Stop Method"),
	_T("[ML2]HW Limit Stop Method"),
	_T("[ML2]Limit Sensor Logic"),
	_T("[ML2]Org Speed"),
	_T("[ML2]Org Search Speed"),
	_T("[ML2]Org Acc Dec Time"),
	_T("[ML2]Org Method"),
	_T("[ML2]Org Dir"),
	_T("[ML2]Org OffSet"),
	_T("[ML2]Org Position Set"),
	_T("[ML2]Org Sensor Logic"),
	_T("[ML2]Limit Sensor Dir"),
	_T("[ML2]Pulse Type"),
	_T("[ML2]Encoder Dir"),
	_T("[ML2]Motion Dir"),
	_T("[ML2]Servo Alarmreset Logic"),
	_T("[ML2]Servo OnOutput Logic"),
	_T("[ML2]Servo Alarm Logic"),
	_T("[ML2]Servo Inposition Logic"),
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
 };

 static LPCTSTR g_lpszEziServo2PlusESWLimitStopMethod[EEziServo2PlusESWLimitStopMethod_Count] =
 {
	 _T("EmergencyStop"),
	 _T("SoftStop"),
	 _T("NotUsedSWLimit"),
};

 static LPCTSTR g_lpszEziServo2PlusEHWLimitStopMethod[EEziServo2PlusEHWLimitStopMethod_Count] =
 {
	 _T("EmergencyStop"),
	 _T("SoftStop"),
 };

 static LPCTSTR g_lpszEziServo2PlusELogic[EEziServo2PlusELogic_Count] =
 {
	 _T("Low"),
	 _T("High"),
 };

 static LPCTSTR g_lpszEziServo2PlusEOrgMethod[EEziServo2PlusEOrgMethod_Count] =
 {
	 _T("OrgSpeedToHomeOrgSearchSpeedToHome"),
	 _T("OrgSpeedToHomeOrgSearchSpeedToZPulse"),
	 _T("OrgSpeedToLimit"),
	 _T("OrgSpeedToLimitOrgSearchSpeedToZPulse"),
	 _T("SetHome"),
	 _T("OrgSearchSpeedToZPulse"),
	 _T("OrgSpeedToObject"),
	 _T("OrgSpeedToObjectOrgSearchSpeedToZPulse"),
 };

 static LPCTSTR g_lpszEziServo2PlusEDir[EEziServo2PlusEDir_Count] =
 {
	 _T("CW"),
	 _T("CCW"),
 };

 static LPCTSTR g_lpszEziServo2PlusESwitch[EEziServo2PlusSwitch_Count] =
 {
	 _T("Off"),
	 _T("On"),
 };

static LPCTSTR g_lpszEziServo2PlusEMotionType[EEziServo2PlusEMotionType_Count] =
{
	_T("Servo2"),
	_T("Motion Link2"),
};

static LPCTSTR g_lpszEziServo2PlusEMotionLink2OrgMethod[EEziServo2PlusEMotionLinek2OrgMethod_Count] =
{
	_T("OrgSpeedToHomeOrgSearchSpeedToHome"),
	_T("OrgSpeedToHomeOrgSearchSpeedToZPulse"),
	_T("OrgSpeedToLimit"),
	_T("OrgSpeedToLimitOrgSearchSpeedToZPulse"),
	_T("OrgSearchSpeedToZPulse"),
	_T("SetHome"),
};

static LPCTSTR g_lpszEziServo2PlusEPulseType[EEziServo2PlusEPulseType_Count] =
{
	_T("Pulse/Direction"),
	_T("CW/CCW"),
};

static LPCTSTR g_lpszEziServo2PlusEEncoderMethod[EEziServo2PlusEEncoderMethod_Count] =
{
	_T("UpDownMode"),
	_T("Sqr1Mode"),
	_T("Sqr2Mode"),
	_T("Sqr4Mode"),
};


CDeviceEziServo2PlusE::CDeviceEziServo2PlusE()
{
}

CDeviceEziServo2PlusE::~CDeviceEziServo2PlusE()
{
	Terminate();
	FreeIO();
}

ptrdiff_t CDeviceEziServo2PlusE::OnMessage(CMessageBase * pMessage)
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

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_Input));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 1);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_Output));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 2);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_MotorControlling));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 3);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_SensorStatus));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 4);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_ServoMode));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 5);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_Home));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 6);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_Velocity) + _T(" :"));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 7);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_Acceleration) + _T(" :"));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 8);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_Distance) + _T(" :"));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 9);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_Position) + _T(" :"));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 10);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_LimitPlus));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 11);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_Home));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 12);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_LimitMinus));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 13);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_Alarm));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 14);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_Inposition));

		pCtrlUI = GetDlgItem(EDeviceAddControlID_BtnHomming);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_Home));

		pCtrlUI = GetDlgItem(EDeviceAddControlID_BtnServoModeOff);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_Off));

		pCtrlUI = GetDlgItem(EDeviceAddControlID_BtnServoModeOn);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_On));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 15);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_CommandPosition));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 16);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_ActualPosition));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 17);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_Alarm));

		pCtrlUI = GetDlgItem(EDeviceAddControlID_BtnCommandReset);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_Reset));

		pCtrlUI = GetDlgItem(EDeviceAddControlID_ActualPosition);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_Value) + _T(" : 0.0000"));

		pCtrlUI = GetDlgItem(EDeviceAddControlID_BtnActualReset);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_Reset));

		pCtrlUI = GetDlgItem(EDeviceAddControlID_BtnAlarmReset);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_Reset));
	}
	while(false);

	return 0;
}

EDeviceInitializeResult CDeviceEziServo2PlusE::Initialize()
{
	EDeviceInitializeResult eReturn = EDeviceInitializeResult_UnknownError;

	CString strMessage, strStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("EziServo2PlusE"));

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

		int nDeviceID = _ttoi(GetDeviceID());

		CString strIPAddress;
		if(GetIPAddress(&strIPAddress))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("IP Address"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}
		
		BYTE arrForm[4] = { 0, };

		for(int i = 0; i < 3; ++i)
		{
			int nPos = strIPAddress.Find('.');
			arrForm[i] = _ttoi(strIPAddress.Left(nPos));
			strIPAddress = strIPAddress.Right(strIPAddress.GetLength() - nPos - 1);
		}
		arrForm[3] = _ttoi(strIPAddress);

		if(!PE::FAS_Connect(arrForm[0], arrForm[1], arrForm[2], arrForm[3], nDeviceID))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("IP Address"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		EEziServo2PlusEMotionType eMotionType = EEziServo2PlusEMotionType_Count;
		if(GetMotionType(&eMotionType))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Motion Type"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		m_eCurrentMode = eMotionType;

		switch(eMotionType)
		{
		case EEziServo2PlusEMotionType_Servo2:
			{
				int nPulsePerRevolution = 0;
				if(GetServo2PulsePerRevolution(&nPulsePerRevolution))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Pulse Per Revolution"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, SERVO2_PULSEPERREVOLUTION, nPulsePerRevolution))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Pulse Per Revolution"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				int nAxisMaxSpeed = 0;
				if(GetServo2AxisMaxSpeed(&nAxisMaxSpeed))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Max Speed"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, SERVO2_AXISMAXSPEED, nAxisMaxSpeed))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Max Speed"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				int nAxisStartSpeed = 0;
				if(GetServo2AxisStartSpeed(&nAxisStartSpeed))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Start Speed"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, SERVO2_AXISSTARTSPEED, nAxisStartSpeed))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Start Speed"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				int nAxisAccTime = 0;
				if(GetServo2AxisAccTime(&nAxisAccTime))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Acc Time"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, SERVO2_AXISACCTIME, nAxisAccTime))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Acc Time"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				int nAxisDecTime = 0;
				if(GetServo2AxisDecTime(&nAxisDecTime))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Dec Time"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, SERVO2_AXISDECTIME, nAxisDecTime))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Dec Time"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				int nSpeedOverride = 0;
				if(GetServo2SpeedOverride(&nSpeedOverride))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Speed Override"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, SERVO2_SPEEDOVERRIDE, nSpeedOverride))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Speed Override"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				int nJogSpeed = 0;
				if(GetServo2JogSpeed(&nJogSpeed))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Jog Speed"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, SERVO2_JOGHIGHSPEED, nJogSpeed))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Jog Speed"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				int nJogStartSpeed = 0;
				if(GetServo2JogStartSpeed(&nJogStartSpeed))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Jog Start Speed"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, SERVO2_JOGLOWSPEED, nJogStartSpeed))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Jog Start Speed"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				int nJogAccDecTime = 0;
				if(GetServo2JogAccDecTime(&nJogAccDecTime))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Jog Start Acc"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, SERVO2_JOGACCDECTIME, nJogAccDecTime))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Jog Start Acc"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				int nSWLimitPlusValue = 0;
				if(GetServo2SWLimitPlusValue(&nSWLimitPlusValue))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("SW Limit Plus Value"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, SERVO2_SWLMTPLUSVALUE, nSWLimitPlusValue))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("SW Limit Plus Value"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				int nSWLimitMinusValue = 0;
				if(GetServo2SWLimitMinusValue(&nSWLimitMinusValue))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("SW Limit Minus Value"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, SERVO2_SWLMTMINUSVALUE, nSWLimitMinusValue))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("SW Limit Minus Value"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				EEziServo2PlusESWLimitStopMethod eSWLimitStopMethod = EEziServo2PlusESWLimitStopMethod_Count;
				if(GetServo2SWLimitStopMethod(&eSWLimitStopMethod))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("SW Limit Stop Method"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, SERVO2_SOFTLMTSTOPMETHOD, (long)eSWLimitStopMethod))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("SW Limit Stop Method"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				EEziServo2PlusEHWLimitStopMethod eHWLimitStopMethod = EEziServo2PlusEHWLimitStopMethod_Count;
				if(GetServo2HWLimitStopMethod(&eHWLimitStopMethod))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("HW Limit Stop Method"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, SERVO2_HARDLMTSTOPMETHOD, (long)eHWLimitStopMethod))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("HW Limit Stop Method"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				EEziServo2PlusELogic eSensorLogic = EEziServo2PlusELogic_Count;
				if(GetServo2LimitSensorLogic(&eSensorLogic))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Limit Sensor Logic"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, SERVO2_LIMITSENSORLOGIC, (long)eSensorLogic))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Limit Sensor Logic"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				int nOrgSpeed = 0;
				if(GetServo2OrgSpeed(&nOrgSpeed))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Org Speed"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, SERVO2_ORGSPEED, nOrgSpeed))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Org Speed"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				int nOrgSearchSpeed = 0;
				if(GetServo2OrgSearchSpeed(&nOrgSearchSpeed))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Org Search Speed"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, SERVO2_ORGSEARCHSPEED, nOrgSearchSpeed))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Org Search Speed"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				int nOrgAccDecTime = 0;
				if(GetServo2OrgAccDecTime(&nOrgAccDecTime))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Org Acc Dec Time"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, SERVO2_ORGACCDECTIME, nOrgAccDecTime))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Org Acc Dec Time"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				EEziServo2PlusEOrgMethod eOrgMethod = EEziServo2PlusEOrgMethod_Count;
				if(GetServo2OrgMethod(&eOrgMethod))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Org Method"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, SERVO2_ORGMETHOD, (long)eOrgMethod))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Org Method"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				EEziServo2PlusEDir eOrgDir = EEziServo2PlusEDir_Count;
				if(GetServo2OrgDir(&eOrgDir))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Org Dir"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, SERVO2_ORGDIR, (long)eOrgDir))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Org Dir"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				int nOrgOffSet = 0;
				if(GetServo2OrgOffSet(&nOrgOffSet))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Org OffSet"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, SERVO2_ORGOFFSET, nOrgOffSet))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Org OffSet"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				int nOrgPositionSet = 0;
				if(GetServo2OrgPositionSet(&nOrgPositionSet))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Org Position Set"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, SERVO2_ORGPOSITIONSET, nOrgPositionSet))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Org Position Set"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				EEziServo2PlusELogic eOrgSensorLogic = EEziServo2PlusELogic_Count;
				if(GetServo2OrgSensorLogic(&eOrgSensorLogic))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Org Sensor Logic"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, SERVO2_ORGSENSORLOGIC, (long)eOrgSensorLogic))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Org Sensor Logic"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				int nPositionLoopGain = 0;
				if(GetServo2PositionLoopGain(&nPositionLoopGain))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Position Loop Gain"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, SERVO2_POSITIONLOOPGAIN, nPositionLoopGain))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Position Loop Gain"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				int nInposValue = 0;
				if(GetServo2InposValue(&nInposValue))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Inpos Value"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, SERVO2_INPOSITIONVALUE, nInposValue))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Inpos Value"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				int nPosTrackingLimit = 0;
				if(GetServo2PosTrackingLimit(&nPosTrackingLimit))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Pos Tracking Limit"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, SERVO2_POSTRACKINGLIMIT, nPosTrackingLimit))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Pos Tracking Limit"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				EEziServo2PlusEDir eMotionDir = EEziServo2PlusEDir_Count;
				if(GetServo2MotionDir(&eMotionDir))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Motion Dir"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, SERVO2_MOTIONDIR, (long)eMotionDir))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Motion Dir"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				EEziServo2PlusEDir eLimitSensorDir = EEziServo2PlusEDir_Count;
				if(GetServo2LimitSensorDir(&eLimitSensorDir))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Limit Sensor Dir"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, SERVO2_LIMITSENSORDIR, (long)eLimitSensorDir))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Limit Sensor Dir"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				int nOrgTorqueRatio = 0;
				if(GetServo2OrgTorqueRatio(&nOrgTorqueRatio))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Org Torque Ratio"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, SERVO2_ORGTORQUERATIO, nOrgTorqueRatio))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Org Torque Ratio"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				int nPosErrorOverflowLimit = 0;
				if(GetServo2PosErrorOverflowLimit(&nPosErrorOverflowLimit))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Pos Error Overflow Limit"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, SERVO2_POSERROVERFLOWLIMIT, nPosErrorOverflowLimit))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Pos Error Overflow Limit"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				int nBrakeDelayTime = 0;
				if(GetServo2BrakeDelayTime(&nBrakeDelayTime))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Brake Delay Time"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, SERVO2_BRAKEDELAYTIME, nBrakeDelayTime))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Brake Delay Time"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}
			}
			break;
		case EEziServo2PlusEMotionType_MotionLink2:
			{
				EEziServo2PlusEEncoderMethod eEncoderMultiply = EEziServo2PlusEEncoderMethod_Count;
				if(GetMotionLink2EncoderMultiply(&eEncoderMultiply))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Encoder Multiply"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, MOTIONLINK2_ENCODERMULTIPLY, eEncoderMultiply))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Encoder Multiply"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				int nAxisMaxSpeed = -1;
				if(GetMotionLink2AxisMaxSpeed(&nAxisMaxSpeed))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Axis Max Speed"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, MOTIONLINK2_AXISMAXSPEED, nAxisMaxSpeed))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Axis Max Speed"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				int nAxisStartSpeed = -1;
				if(GetMotionLink2AxisStartSpeed(&nAxisStartSpeed))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Axis Start Speed"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, MOTIONLINK2_AXISSTARTSPEED, nAxisStartSpeed))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Axis Start Speed"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				int nAxisAccTime = -1;
				if(GetMotionLink2AxisAccTime(&nAxisAccTime))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Axis Acc Time"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, MOTIONLINK2_AXISACCTIME, nAxisAccTime))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Axis Acc Time"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				int nAxisDecTime = -1;
				if(GetMotionLink2AxisDecTime(&nAxisDecTime))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Axis Dec Time"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, MOTIONLINK2_AXISDECTIME, nAxisDecTime))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Axis Dec Time"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				int nSpeedOverride = -1;
				if(GetMotionLink2SpeedOverride(&nSpeedOverride))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Speed Override"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, MOTIONLINK2_SPEEDOVERRIDE, nSpeedOverride))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Speed Override"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				int nJogSpeed = -1;
				if(GetMotionLink2JogSpeed(&nJogSpeed))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Jog Speed"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, MOTIONLINK2_JOGHIGHSPEED, nJogSpeed))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Jog Speed"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				int nJogStartSpeed = -1;
				if(GetMotionLink2JogStartSpeed(&nJogStartSpeed))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Jog Start Speed"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, MOTIONLINK2_JOGLOWSPEED, nJogStartSpeed))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Jog Start Speed"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				int nJogAccDecTime = -1;
				if(GetMotionLink2JogAccDecTime(&nJogAccDecTime))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Jog AccDec Time"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, MOTIONLINK2_JOGACCDECTIME, nJogAccDecTime))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Jog AccDec Time"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				int nSWLimitPlusValue = -1;
				if(GetMotionLink2SWLimitPlusValue(&nSWLimitPlusValue))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("SW Limit Plus Value"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, MOTIONLINK2_SWLMTPLUSVALUE, nSWLimitPlusValue))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("SW Limit Plus Value"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				int nSWLimitMinusValue = -1;
				if(GetMotionLink2SWLimitMinusValue(&nSWLimitMinusValue))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("SW Limit Minus Value"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, MOTIONLINK2_SWLMTMINUSVALUE, nSWLimitMinusValue))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("SW Limit Minus Value"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				EEziServo2PlusESWLimitStopMethod eSWLimitStopMethod = EEziServo2PlusESWLimitStopMethod_Count;
				if(GetMotionLink2SWLimitStopMethod(&eSWLimitStopMethod))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("SW Limit Stop Method"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, MOTIONLINK2_SOFTLMTSTOPMETHOD, eSWLimitStopMethod))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("SW Limit Stop Method"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				EEziServo2PlusESWLimitStopMethod eHWLimitStopMethod = EEziServo2PlusESWLimitStopMethod_Count;
				if(GetMotionLink2HWLimitStopMethod(&eHWLimitStopMethod))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("HW Limit Stop Method"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, MOTIONLINK2_HARDLMTSTOPMETHOD, eHWLimitStopMethod))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("HW Limit Stop Method"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				EEziServo2PlusELogic eLimitSensorLogic = EEziServo2PlusELogic_Count;
				if(GetMotionLink2LimitSensorLogic(&eLimitSensorLogic))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Limit Sensor Logic"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, MOTIONLINK2_LIMITSENSORLOGIC, eLimitSensorLogic))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Limit Sensor Logic"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				int nOrgSpeed = -1;
				if(GetMotionLink2OrgSpeed(&nOrgSpeed))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Org Speed"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, MOTIONLINK2_ORGSPEED, nOrgSpeed))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Org Speed"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				int nOrgSearchSpeed = -1;
				if(GetMotionLink2OrgSearchSpeed(&nOrgSearchSpeed))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Org Search Speed"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, MOTIONLINK2_ORGSEARCHSPEED, nOrgSearchSpeed))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Org Search Speed"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				int nOrgAccDecTime = -1;
				if(GetMotionLink2OrgAccDecTime(&nOrgAccDecTime))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Org AccDec Time"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, MOTIONLINK2_ORGACCDECTIME, nOrgAccDecTime))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Org AccDec Time"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				EEziServo2PlusEMotionLinek2OrgMethod eOrgMethod = EEziServo2PlusEMotionLinek2OrgMethod_Count;
				if(GetMotionLink2OrgMethod(&eOrgMethod))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Org Method"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, MOTIONLINK2_ORGMETHOD, eOrgMethod))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Org Method"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				EEziServo2PlusEDir eOrgDir = EEziServo2PlusEDir_Count;
				if(GetMotionLink2OrgDir(&eOrgDir))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Org Dir"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, MOTIONLINK2_ORGDIR, eOrgDir))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Org Dir"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				int nOrgOffSet = -1;
				if(GetMotionLink2OrgOffSet(&nOrgOffSet))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Org OffSet"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, MOTIONLINK2_ORGOFFSET, nOrgOffSet))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Org OffSet"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				int nOrgPositionSet = -1;
				if(GetMotionLink2OrgPositionSet(&nOrgPositionSet))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Org Position Set"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, MOTIONLINK2_ORGPOSITIONSET, nOrgPositionSet))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Org Position Set"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				EEziServo2PlusELogic eOrgSensorLogic = EEziServo2PlusELogic_Count;
				if(GetMotionLink2OrgSensorLogic(&eOrgSensorLogic))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Org Sensor Logic"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, MOTIONLINK2_ORGSENSORLOGIC, eOrgSensorLogic))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Org Sensor Logic"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				EEziServo2PlusEDir eLimitSensorDir = EEziServo2PlusEDir_Count;
				if(GetMotionLink2LimitSensorDir(&eLimitSensorDir))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Limit Sensor Dir"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, MOTIONLINK2_LIMITSENSORDIR, eLimitSensorDir))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Limit Sensor Dir"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				EEziServo2PlusEPulseType ePulseType = EEziServo2PlusEPulseType_Count;
				if(GetMotionLink2PulseType(&ePulseType))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Pulse Type"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, MOTIONLINK2_PULSETYPE, ePulseType))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Pulse Type"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				EEziServo2PlusEDir eEncoderDir = EEziServo2PlusEDir_Count;
				if(GetMotionLink2EncoderDir(&eEncoderDir))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Encoder Dir"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, MOTIONLINK2_ENCODERDIR, eEncoderDir))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Encoder Dir"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				EEziServo2PlusEDir eMotionDir = EEziServo2PlusEDir_Count;
				if(GetMotionLink2MotionDir(&eMotionDir))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Motion Dir"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, MOTIONLINK2_MOTIONDIR, eMotionDir))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Motion Dir"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				EEziServo2PlusELogic eServoAlarmresetLogic = EEziServo2PlusELogic_Count;
				if(GetMotionLink2ServoAlarmresetLogic(&eServoAlarmresetLogic))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Servo Alarmreset Logic"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, MOTIONLINK2_SERVOALARMRESETLOGIC, eServoAlarmresetLogic))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Servo Alarmreset Logic"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				EEziServo2PlusELogic eServoOnOutputLogic = EEziServo2PlusELogic_Count;
				if(GetMotionLink2ServoOnOutputLogic(&eServoOnOutputLogic))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Servo On Output Logic"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, MOTIONLINK2_SERVOONOUTPUTLOGIC, eServoOnOutputLogic))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Servo On Output Logic"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				EEziServo2PlusELogic eServoAlarmLogic = EEziServo2PlusELogic_Count;
				if(GetMotionLink2ServoAlarmLogic(&eServoAlarmLogic))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Servo Alarm Logic"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, MOTIONLINK2_SERVOALARMLOGIC, eServoAlarmLogic))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Servo Alarm Logic"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				EEziServo2PlusELogic eServoInpositionLogic = EEziServo2PlusELogic_Count;
				if(GetMotionLink2ServoInpositionLogic(&eServoInpositionLogic))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Servo Inposition Logic"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
				if(PE::FAS_SetParameter(nDeviceID, MOTIONLINK2_SERVOINPOSLOGIC, eServoInpositionLogic))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Servo Inposition Logic"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}
			}
			break;
		}

		if(PE::FAS_SaveAllParameters(nDeviceID))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("All setting save"));
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

EDeviceTerminateResult CDeviceEziServo2PlusE::Terminate()
{
	EDeviceTerminateResult eReturn = EDeviceTerminateResult_UnknownError;

	CString strMessage;

	SetStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("EziServo2PlusE"));

		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);

		return EDeviceTerminateResult_NotFoundApiError;
	}

	do
	{
		SetServoOn(false);

		if(IsInitialized())
			PE::FAS_Close(_ttoi(GetDeviceID()));

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

bool CDeviceEziServo2PlusE::LoadSettings()
{
	bool bReturn = false;

	m_vctParameterFieldConfigurations.clear();

	do
	{
		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_DeviceID, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_DeviceID], _T("0"), EParameterFieldType_Edit, nullptr, _T("Primary Drive Key"), 0);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_IPAddress, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_IPAddress], _T("192.168.0.2"), EParameterFieldType_IPAddress, nullptr, _T("192.168.0.[0 ~ 255]"), 0);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_MotionType, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_MotionType], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszEziServo2PlusEMotionType, EEziServo2PlusEMotionType_Count), nullptr, 0);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_Servo2, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_Servo2], _T("0"), EParameterFieldType_None, nullptr, nullptr, 0);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_Servo2_PulsePerRevolution, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_Servo2_PulsePerRevolution], _T("8"), EParameterFieldType_Edit, nullptr, _T("0 ~ 8"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_Servo2_AxisMaxSpeed, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_Servo2_AxisMaxSpeed], _T("500000"), EParameterFieldType_Edit, nullptr, _T("1 ~ 2500000 pps"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_Servo2_AxisStartSpeed, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_Servo2_AxisStartSpeed], _T("1"), EParameterFieldType_Edit, nullptr, _T("1 ~ 35000 pps"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_Servo2_AxisAccTime, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_Servo2_AxisAccTime], _T("100"), EParameterFieldType_Edit, nullptr, _T("1 ~ 9999 msec"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_Servo2_AxisDecTime, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_Servo2_AxisDecTime], _T("100"), EParameterFieldType_Edit, nullptr, _T("1 ~ 9999 msec"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_Servo2_SpeedOverride, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_Servo2_SpeedOverride], _T("100"), EParameterFieldType_Edit, nullptr, _T("1 ~ 500 %"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_Servo2_JogSpeed, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_Servo2_JogSpeed], _T("5000"), EParameterFieldType_Edit, nullptr, _T("1 ~ 2500000 pps"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_Servo2_JogStartSpeed, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_Servo2_JogStartSpeed], _T("1"), EParameterFieldType_Edit, nullptr, _T("1 ~ 35000 pps"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_Servo2_JogAccDecTime, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_Servo2_JogAccDecTime], _T("100"), EParameterFieldType_Edit, nullptr, _T("1 ~ 9999 msec"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_Servo2_SWLimitPlusValue, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_Servo2_SWLimitPlusValue], _T("134217727"), EParameterFieldType_Edit, nullptr, _T("-134217728 ~ 134217727 pulse"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_Servo2_SWLimitMinusValue, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_Servo2_SWLimitMinusValue], _T("-134217728"), EParameterFieldType_Edit, nullptr, _T("-134217728 ~ 134217727 pulse"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_Servo2_SWLimitStopMethod, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_Servo2_SWLimitStopMethod], _T("2"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszEziServo2PlusESWLimitStopMethod, EEziServo2PlusESWLimitStopMethod_Count), _T("0 ~ 2"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_Servo2_HWLimitStopMethod, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_Servo2_HWLimitStopMethod], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszEziServo2PlusEHWLimitStopMethod, EEziServo2PlusEHWLimitStopMethod_Count), _T("0 ~ 1"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_Servo2_LimitSensorLogic, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_Servo2_LimitSensorLogic], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszEziServo2PlusELogic, EEziServo2PlusELogic_Count), _T("0 ~ 1"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_Servo2_OrgSpeed, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_Servo2_OrgSpeed], _T("5000"), EParameterFieldType_Edit, nullptr, _T("1 ~ 500000 pps"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_Servo2_OrgSearchSpeed, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_Servo2_OrgSearchSpeed], _T("1000"), EParameterFieldType_Edit, nullptr, _T("1 ~ 500000 pps"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_Servo2_OrgAccDecTime, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_Servo2_OrgAccDecTime], _T("50"), EParameterFieldType_Edit, nullptr, _T("1 ~ 9999 msec"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_Servo2_OrgMethod, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_Servo2_OrgMethod], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszEziServo2PlusEOrgMethod, EEziServo2PlusEOrgMethod_Count), _T("0 ~ 7"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_Servo2_OrgDir, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_Servo2_OrgDir], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszEziServo2PlusEDir, EEziServo2PlusEDir_Count), _T("0 ~ 1"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_Servo2_OrgOffSet, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_Servo2_OrgOffSet], _T("0"), EParameterFieldType_Edit, nullptr, _T("-134217728 ~ 134217727 pulse"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_Servo2_OrgPositionSet, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_Servo2_OrgPositionSet], _T("0"), EParameterFieldType_Edit, nullptr, _T("-134217728 ~ 134217727 pulse"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_Servo2_OrgSensorLogic, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_Servo2_OrgSensorLogic], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszEziServo2PlusELogic, EEziServo2PlusELogic_Count), _T("0 ~ 1"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_Servo2_PositionLoopGain, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_Servo2_PositionLoopGain], _T("4"), EParameterFieldType_Edit, nullptr, _T("0 ~ 63"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_Servo2_InposValue, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_Servo2_InposValue], _T("0"), EParameterFieldType_Edit, nullptr, _T("0 ~ 127"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_Servo2_PosTrackingLimit, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_Servo2_PosTrackingLimit], _T("5000"), EParameterFieldType_Edit, nullptr, _T("1 ~ 134217727 pulse"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_Servo2_MotionDir, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_Servo2_MotionDir], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszEziServo2PlusEDir, EEziServo2PlusEDir_Count), _T("0 ~ 1"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_Servo2_LimitSensorDir, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_Servo2_LimitSensorDir], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszEziServo2PlusEDir, EEziServo2PlusEDir_Count), _T("0 ~ 1"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_Servo2_OrgTorqueRatio, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_Servo2_OrgTorqueRatio], _T("50"), EParameterFieldType_Edit, nullptr, _T("20 ~ 90 %"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_Servo2_PosErrorOverflowLimit, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_Servo2_PosErrorOverflowLimit], _T("5000"), EParameterFieldType_Edit, nullptr, _T("1 ~ 134217727 pulse"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_Servo2_BrakeDelayTime, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_Servo2_BrakeDelayTime], _T("200"), EParameterFieldType_Edit, nullptr, _T("10 ~ 5000 msec"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_MotionLink2, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_MotionLink2], _T("0"), EParameterFieldType_None, nullptr, nullptr, 0);

		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_MotionLink2_EncoderMultiply, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_MotionLink2_EncoderMultiply], _T("3"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszEziServo2PlusEEncoderMethod, EEziServo2PlusEEncoderMethod_Count), _T("0 ~ 3  0 : Not used, 1 : 1 inter, 2 : 2 inter, 3 : 4 inter"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_MotionLink2_AxisMaxSpeed, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_MotionLink2_AxisMaxSpeed], _T("500000"), EParameterFieldType_Edit, nullptr, _T("1 ~ 2500000 pps"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_MotionLink2_AxisStartSpeed, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_MotionLink2_AxisStartSpeed], _T("1"), EParameterFieldType_Edit, nullptr, _T("1 ~ 35000 pps"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_MotionLink2_AxisAccTime, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_MotionLink2_AxisAccTime], _T("100"), EParameterFieldType_Edit, nullptr, _T("1 ~ 9999 msec"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_MotionLink2_AxisDecTime, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_MotionLink2_AxisDecTime], _T("100"), EParameterFieldType_Edit, nullptr, _T("1 ~ 9999 msec"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_MotionLink2_SpeedOverride, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_MotionLink2_SpeedOverride], _T("100"), EParameterFieldType_Edit, nullptr, _T("1 ~ 500 %"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_MotionLink2_JogSpeed, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_MotionLink2_JogSpeed], _T("5000"), EParameterFieldType_Edit, nullptr, _T("1 ~ 2500000 pps"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_MotionLink2_JogStartSpeed, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_MotionLink2_JogStartSpeed], _T("1"), EParameterFieldType_Edit, nullptr, _T("1 ~ 35000 pps"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_MotionLink2_JogAccDecTime, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_MotionLink2_JogAccDecTime], _T("100"), EParameterFieldType_Edit, nullptr, _T("1 ~ 9999 msec"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_MotionLink2_SWLimitPlusValue, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_MotionLink2_SWLimitPlusValue], _T("134217727"), EParameterFieldType_Edit, nullptr, _T("-134217728 ~ 134217727 pulse"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_MotionLink2_SWLimitMinusValue, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_MotionLink2_SWLimitMinusValue], _T("-134217728"), EParameterFieldType_Edit, nullptr, _T("-134217728 ~ 134217727 pulse"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_MotionLink2_SWLimitStopMethod, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_MotionLink2_SWLimitStopMethod], _T("2"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszEziServo2PlusESWLimitStopMethod, EEziServo2PlusESWLimitStopMethod_Count), _T("0 ~ 2"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_MotionLink2_HWLimitStopMethod, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_MotionLink2_HWLimitStopMethod], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszEziServo2PlusEHWLimitStopMethod, EEziServo2PlusEHWLimitStopMethod_Count), _T("0 ~ 1"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_MotionLink2_LimitSensorLogic, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_MotionLink2_LimitSensorLogic], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszEziServo2PlusELogic, EEziServo2PlusELogic_Count), _T("0 ~ 1"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_MotionLink2_OrgSpeed, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_MotionLink2_OrgSpeed], _T("5000"), EParameterFieldType_Edit, nullptr, _T("1 ~ 500000 pps"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_MotionLink2_OrgSearchSpeed, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_MotionLink2_OrgSearchSpeed], _T("1000"), EParameterFieldType_Edit, nullptr, _T("1 ~ 500000 pps"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_MotionLink2_OrgAccDecTime, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_MotionLink2_OrgAccDecTime], _T("50"), EParameterFieldType_Edit, nullptr, _T("1 ~ 9999 msec"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_MotionLink2_OrgMethod, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_MotionLink2_OrgMethod], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszEziServo2PlusEMotionLink2OrgMethod, EEziServo2PlusEMotionLinek2OrgMethod_Count), _T("0 ~ 5"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_MotionLink2_OrgDir, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_MotionLink2_OrgDir], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszEziServo2PlusEDir, EEziServo2PlusEDir_Count), _T("0 ~ 1"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_MotionLink2_OrgOffSet, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_MotionLink2_OrgOffSet], _T("0"), EParameterFieldType_Edit, nullptr, _T("-134217728 ~ 134217727 pulse"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_MotionLink2_OrgPositionSet, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_MotionLink2_OrgPositionSet], _T("0"), EParameterFieldType_Edit, nullptr, _T("-134217728 ~ 134217727 pulse"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_MotionLink2_OrgSensorLogic, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_MotionLink2_OrgSensorLogic], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszEziServo2PlusELogic, EEziServo2PlusELogic_Count), _T("0 ~ 1"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_MotionLink2_LimitSensorDir, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_MotionLink2_LimitSensorDir], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszEziServo2PlusEDir, EEziServo2PlusEDir_Count), _T("0 ~ 1"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_MotionLink2_PulseType, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_MotionLink2_PulseType], _T("1"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszEziServo2PlusEPulseType, EEziServo2PlusEPulseType_Count), _T("0 ~ 1"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_MotionLink2_EncoderDir, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_MotionLink2_EncoderDir], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszEziServo2PlusEDir, EEziServo2PlusEDir_Count), _T("0 ~ 1"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_MotionLink2_MotionDir, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_MotionLink2_MotionDir], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszEziServo2PlusEDir, EEziServo2PlusEDir_Count), _T("0 ~ 1"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_MotionLink2_ServoAlarmresetLogic, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_MotionLink2_ServoAlarmresetLogic], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszEziServo2PlusELogic, EEziServo2PlusELogic_Count), _T("0 ~ 1"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_MotionLink2_ServoOnOutputLogic, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_MotionLink2_ServoOnOutputLogic], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszEziServo2PlusELogic, EEziServo2PlusELogic_Count), _T("0 ~ 1"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_MotionLink2_ServoAlarmLogic, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_MotionLink2_ServoAlarmLogic], _T("1"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszEziServo2PlusELogic, EEziServo2PlusELogic_Count), _T("0 ~ 1"), 1);

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_MotionLink2_ServoInpositionLogic, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_MotionLink2_ServoInpositionLogic], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszEziServo2PlusELogic, EEziServo2PlusELogic_Count), _T("0 ~ 1"), 1);

		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////


		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_InputStatus, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_InputStatus], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);

		for(int i = 0; i < InputPort; ++i)
		{
			AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_UseInputNo0 + i, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_UseInputNo0 + i], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 1);
		}

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_OutputStatus, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_OutputStatus], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);

		for(int i = 0; i < OutputPort; ++i)
		{
			AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_UseOutputNo0 + i, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_UseOutputNo0 + i], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 1);
		}

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_InputName, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_InputName], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);

		for(int i = 0; i < InputPort; ++i)
		{
			AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_InputNo0Name + i, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_InputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);
		}

		AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_OutputName, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_OutputName], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);

		for(int i = 0; i < OutputPort; ++i)
		{
			AddParameterFieldConfigurations(EDeviceParameterEziServo2PlusE_OutputNo0Name + i, g_lpszParamEziServo2PlusE[EDeviceParameterEziServo2PlusE_OutputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);
		}

		bReturn = __super::LoadSettings();

		AllocateIO(InputPort, OutputPort);

		for(auto iter = m_vctParameterFieldConfigurations.begin(); iter != m_vctParameterFieldConfigurations.end(); ++iter)
		{
			if(iter->strParameterName.Find(_T("Use Input No. ")) >= 0)
			{
				int nNumber = _ttoi(iter->strParameterName.Mid(14, 2));

				m_pBUsingInputArray[nNumber] = _ttoi(iter->strParameterValue);
			}
			else if(iter->strParameterName.Find(_T("Use Output No. ")) >= 0)
			{
				int nNumber = _ttoi(iter->strParameterName.Mid(15, 2));

				m_pBUsingOutputArray [nNumber] = _ttoi(iter->strParameterValue);
			}
			else if(iter->strParameterName.Find(_T("Input No. ")) >= 0)
			{
				int nNumber = _ttoi(iter->strParameterName.Mid(10, 2));

				m_pStrInputNameArray[nNumber] = iter->strParameterValue;
			}
			else if(iter->strParameterName.Find(_T("Output No. ")) >= 0)
			{
				int nNumber = _ttoi(iter->strParameterName.Mid(11, 2));

				m_pStrOutputNameArray[nNumber] = iter->strParameterValue;
			}
			else
				bReturn = false;
		}

		GetMotionType(&m_eCurrentMode);
		
		bReturn = true;
	}
	while(false);

	return bReturn;
}


EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetIPAddress(CString* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = GetParamValue(EDeviceParameterEziServo2PlusE_IPAddress);

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetIPAddress(CString strParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_IPAddress;

	CString strPreValue = GetParamValue(eSaveNum);

	do
	{
		if(IsInitialized())
		{
			eReturn = EEziServo2PlusESetFunction_AlreadyInitializedError;
			break;
		}

		if(!SetParamValue(eSaveNum, strParam))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}
		
		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], strPreValue, strParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetMotionType(EEziServo2PlusEMotionType* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = (EEziServo2PlusEMotionType)_ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_MotionType));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetMotionType(EEziServo2PlusEMotionType eParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_MotionType;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < 0 || eParam >= EEziServo2PlusEMotionType_Count)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EEziServo2PlusESetFunction_AlreadyInitializedError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(eParam == EEziServo2PlusEMotionType_Servo2)
		{
			if(!SetParamValue(EDeviceParameterEziServo2PlusE_Servo2, _T("1")))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
				break;
			}

			if(!SaveSettings(EDeviceParameterEziServo2PlusE_Servo2))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
				break;
			}

			if(!SetParamValue(EDeviceParameterEziServo2PlusE_MotionLink2, _T("0")))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
				break;
			}

			if(!SaveSettings(EDeviceParameterEziServo2PlusE_MotionLink2))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
				break;
			}
		}
		else
		{
			if(!SetParamValue(EDeviceParameterEziServo2PlusE_Servo2, _T("0")))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
				break;
			}

			if(!SaveSettings(EDeviceParameterEziServo2PlusE_Servo2))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
				break;
			}

			if(!SetParamValue(EDeviceParameterEziServo2PlusE_MotionLink2, _T("1")))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
				break;
			}

			if(!SaveSettings(EDeviceParameterEziServo2PlusE_MotionLink2))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
				break;
			}
		}

		m_eCurrentMode = eParam;

		UpdateExpandSheet(EDeviceParameterEziServo2PlusE_Servo2);
		UpdateExpandSheet(EDeviceParameterEziServo2PlusE_MotionLink2);

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], g_lpszEziServo2PlusEMotionType[nPreValue], g_lpszEziServo2PlusEMotionType[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetServo2PulsePerRevolution(int* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_Servo2_PulsePerRevolution));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetServo2PulsePerRevolution(int nParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_Servo2_PulsePerRevolution;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_Servo2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), SERVO2_PULSEPERREVOLUTION, nParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}


EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetServo2AxisMaxSpeed(int* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_Servo2_AxisMaxSpeed));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetServo2AxisMaxSpeed(int nParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_Servo2_AxisMaxSpeed;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_Servo2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), SERVO2_AXISMAXSPEED, nParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetServo2AxisStartSpeed(int* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_Servo2_AxisStartSpeed));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetServo2AxisStartSpeed(int nParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_Servo2_AxisStartSpeed;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_Servo2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), SERVO2_AXISSTARTSPEED, nParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}


EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetServo2AxisAccTime(int* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_Servo2_AxisAccTime));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetServo2AxisAccTime(int nParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_Servo2_AxisAccTime;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_Servo2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), SERVO2_AXISACCTIME, nParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetServo2AxisDecTime(int* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_Servo2_AxisDecTime));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetServo2AxisDecTime(int nParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_Servo2_AxisDecTime;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_Servo2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), SERVO2_AXISDECTIME, nParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetServo2SpeedOverride(int* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_Servo2_SpeedOverride));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetServo2SpeedOverride(int nParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_Servo2_SpeedOverride;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_Servo2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), SERVO2_SPEEDOVERRIDE, nParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetServo2JogSpeed(int* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_Servo2_JogSpeed));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetServo2JogSpeed(int nParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_Servo2_JogSpeed;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_Servo2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), SERVO2_JOGHIGHSPEED, nParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetServo2JogStartSpeed(int* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_Servo2_JogStartSpeed));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetServo2JogStartSpeed(int nParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_Servo2_JogStartSpeed;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_Servo2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), SERVO2_JOGLOWSPEED, nParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetServo2JogAccDecTime(int* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_Servo2_JogAccDecTime));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetServo2JogAccDecTime(int nParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_Servo2_JogAccDecTime;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_Servo2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), SERVO2_JOGACCDECTIME, nParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetServo2SWLimitPlusValue(int* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_Servo2_SWLimitPlusValue));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetServo2SWLimitPlusValue(int nParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_Servo2_SWLimitPlusValue;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_Servo2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), SERVO2_SWLMTPLUSVALUE, nParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetServo2SWLimitMinusValue(int* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_Servo2_SWLimitMinusValue));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetServo2SWLimitMinusValue(int nParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_Servo2_SWLimitMinusValue;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_Servo2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), SERVO2_SWLMTMINUSVALUE, nParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetServo2SWLimitStopMethod(EEziServo2PlusESWLimitStopMethod* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = (EEziServo2PlusESWLimitStopMethod)_ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_Servo2_SWLimitStopMethod));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetServo2SWLimitStopMethod(EEziServo2PlusESWLimitStopMethod eParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_Servo2_SWLimitStopMethod;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_Servo2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(eParam < 0 || eParam >= EEziServo2PlusESWLimitStopMethod_Count)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), SERVO2_SOFTLMTSTOPMETHOD, eParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], g_lpszEziServo2PlusESWLimitStopMethod[nPreValue], g_lpszEziServo2PlusESWLimitStopMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetServo2HWLimitStopMethod(EEziServo2PlusEHWLimitStopMethod* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = (EEziServo2PlusEHWLimitStopMethod)_ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_Servo2_HWLimitStopMethod));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetServo2HWLimitStopMethod(EEziServo2PlusEHWLimitStopMethod eParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_Servo2_HWLimitStopMethod;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_Servo2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(eParam < 0 || eParam >= EEziServo2PlusEHWLimitStopMethod_Count)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), SERVO2_HARDLMTSTOPMETHOD, eParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], g_lpszEziServo2PlusEHWLimitStopMethod[nPreValue], g_lpszEziServo2PlusEHWLimitStopMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetServo2LimitSensorLogic(EEziServo2PlusELogic* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = (EEziServo2PlusELogic)_ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_Servo2_LimitSensorLogic));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetServo2LimitSensorLogic(EEziServo2PlusELogic eParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_Servo2_LimitSensorLogic;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_Servo2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(eParam < 0 || eParam >= EEziServo2PlusELogic_Count)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), SERVO2_LIMITSENSORLOGIC, eParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], g_lpszEziServo2PlusELogic[nPreValue], g_lpszEziServo2PlusELogic[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetServo2OrgSpeed(int* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_Servo2_OrgSpeed));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetServo2OrgSpeed(int nParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_Servo2_OrgSpeed;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_Servo2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), SERVO2_ORGSPEED, nParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetServo2OrgSearchSpeed(int* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_Servo2_OrgSearchSpeed));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetServo2OrgSearchSpeed(int nParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_Servo2_OrgSearchSpeed;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_Servo2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), SERVO2_ORGSEARCHSPEED, nParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetServo2OrgAccDecTime(int* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_Servo2_OrgAccDecTime));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetServo2OrgAccDecTime(int nParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_Servo2_OrgAccDecTime;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_Servo2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), SERVO2_ORGACCDECTIME, nParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetServo2OrgMethod(EEziServo2PlusEOrgMethod* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = (EEziServo2PlusEOrgMethod)_ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_Servo2_OrgMethod));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetServo2OrgMethod(EEziServo2PlusEOrgMethod eParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_Servo2_OrgMethod;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_Servo2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(eParam < 0 || eParam >= EEziServo2PlusEOrgMethod_Count)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), SERVO2_ORGMETHOD, eParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], g_lpszEziServo2PlusEOrgMethod[nPreValue], g_lpszEziServo2PlusEOrgMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetServo2OrgDir(EEziServo2PlusEDir* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = (EEziServo2PlusEDir)_ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_Servo2_OrgDir));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetServo2OrgDir(EEziServo2PlusEDir eParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_Servo2_OrgDir;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_Servo2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(eParam < 0 || eParam >= EEziServo2PlusEDir_Count)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), SERVO2_ORGDIR, eParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], g_lpszEziServo2PlusEDir[nPreValue], g_lpszEziServo2PlusEDir[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetServo2OrgOffSet(int* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_Servo2_OrgOffSet));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetServo2OrgOffSet(int nParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_Servo2_OrgOffSet;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_Servo2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), SERVO2_ORGOFFSET, nParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetServo2OrgPositionSet(int* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_Servo2_OrgPositionSet));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetServo2OrgPositionSet(int nParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_Servo2_OrgPositionSet;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_Servo2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), SERVO2_ORGPOSITIONSET, nParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetServo2OrgSensorLogic(EEziServo2PlusELogic* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = (EEziServo2PlusELogic)_ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_Servo2_OrgSensorLogic));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetServo2OrgSensorLogic(EEziServo2PlusELogic eParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_Servo2_OrgSensorLogic;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_Servo2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(eParam < 0 || eParam >= EEziServo2PlusELogic_Count)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), SERVO2_ORGSENSORLOGIC, eParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], g_lpszEziServo2PlusELogic[nPreValue], g_lpszEziServo2PlusELogic[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetServo2PositionLoopGain(int* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_Servo2_PositionLoopGain));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetServo2PositionLoopGain(int nParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_Servo2_PositionLoopGain;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_Servo2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), SERVO2_POSITIONLOOPGAIN, nParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetServo2InposValue(int* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_Servo2_InposValue));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetServo2InposValue(int nParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_Servo2_InposValue;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_Servo2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), SERVO2_INPOSITIONVALUE, nParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetServo2PosTrackingLimit(int* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_Servo2_PosTrackingLimit));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetServo2PosTrackingLimit(int nParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_Servo2_PosTrackingLimit;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_Servo2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), SERVO2_POSTRACKINGLIMIT, nParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetServo2MotionDir(EEziServo2PlusEDir* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = (EEziServo2PlusEDir)_ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_Servo2_MotionDir));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetServo2MotionDir(EEziServo2PlusEDir eParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_Servo2_MotionDir;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_Servo2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(eParam < 0 || eParam >= EEziServo2PlusEDir_Count)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), SERVO2_MOTIONDIR, eParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], g_lpszEziServo2PlusEDir[nPreValue], g_lpszEziServo2PlusEDir[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetServo2LimitSensorDir(EEziServo2PlusEDir* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = (EEziServo2PlusEDir)_ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_Servo2_LimitSensorDir));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetServo2LimitSensorDir(EEziServo2PlusEDir eParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_Servo2_LimitSensorDir;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_Servo2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(eParam < 0 || eParam >= EEziServo2PlusEDir_Count)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), SERVO2_LIMITSENSORDIR, eParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], g_lpszEziServo2PlusEDir[nPreValue], g_lpszEziServo2PlusEDir[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetServo2OrgTorqueRatio(int* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_Servo2_OrgTorqueRatio));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetServo2OrgTorqueRatio(int nParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_Servo2_OrgTorqueRatio;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_Servo2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), SERVO2_ORGTORQUERATIO, nParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetServo2PosErrorOverflowLimit(int* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_Servo2_PosErrorOverflowLimit));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetServo2PosErrorOverflowLimit(int nParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_Servo2_PosErrorOverflowLimit;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_Servo2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), SERVO2_POSERROVERFLOWLIMIT, nParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}


EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetServo2BrakeDelayTime(int* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_Servo2_BrakeDelayTime));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetServo2BrakeDelayTime(int nParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_Servo2_BrakeDelayTime;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_Servo2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), SERVO2_BRAKEDELAYTIME, nParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetMotionLink2EncoderMultiply(_Out_ EEziServo2PlusEEncoderMethod* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = (EEziServo2PlusEEncoderMethod)_ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_MotionLink2_EncoderMultiply));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetMotionLink2EncoderMultiply(_In_ EEziServo2PlusEEncoderMethod eParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_MotionLink2_EncoderMultiply;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_MotionLink2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(eParam < 0 || eParam >= EEziServo2PlusEEncoderMethod_Count)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), MOTIONLINK2_ENCODERMULTIPLY, eParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], g_lpszEziServo2PlusEEncoderMethod[nPreValue], g_lpszEziServo2PlusEEncoderMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetMotionLink2AxisMaxSpeed(_Out_ int* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_MotionLink2_AxisMaxSpeed));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetMotionLink2AxisMaxSpeed(_In_ int nParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_MotionLink2_AxisMaxSpeed;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_MotionLink2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), MOTIONLINK2_AXISMAXSPEED, nParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetMotionLink2AxisStartSpeed(_Out_ int* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_MotionLink2_AxisStartSpeed));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetMotionLink2AxisStartSpeed(_In_ int nParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_MotionLink2_AxisStartSpeed;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_MotionLink2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), MOTIONLINK2_AXISSTARTSPEED, nParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetMotionLink2AxisAccTime(_Out_ int* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_MotionLink2_AxisAccTime));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetMotionLink2AxisAccTime(_In_ int nParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_MotionLink2_AxisAccTime;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_MotionLink2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), MOTIONLINK2_AXISACCTIME, nParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetMotionLink2AxisDecTime(_Out_ int* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_MotionLink2_AxisDecTime));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetMotionLink2AxisDecTime(_In_ int nParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_MotionLink2_AxisDecTime;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_MotionLink2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), MOTIONLINK2_AXISDECTIME, nParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetMotionLink2SpeedOverride(_Out_ int* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_MotionLink2_SpeedOverride));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetMotionLink2SpeedOverride(_In_ int nParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_MotionLink2_SpeedOverride;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_MotionLink2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), MOTIONLINK2_SPEEDOVERRIDE, nParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetMotionLink2JogSpeed(_Out_ int* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_MotionLink2_JogSpeed));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetMotionLink2JogSpeed(_In_ int nParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_MotionLink2_JogSpeed;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_MotionLink2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), MOTIONLINK2_JOGHIGHSPEED, nParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetMotionLink2JogStartSpeed(_Out_ int* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_MotionLink2_JogStartSpeed));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetMotionLink2JogStartSpeed(_In_ int nParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_MotionLink2_JogStartSpeed;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_MotionLink2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), MOTIONLINK2_JOGLOWSPEED, nParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetMotionLink2JogAccDecTime(_Out_ int* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_MotionLink2_JogAccDecTime));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetMotionLink2JogAccDecTime(_In_ int nParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_MotionLink2_JogAccDecTime;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_MotionLink2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), MOTIONLINK2_JOGACCDECTIME, nParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetMotionLink2SWLimitPlusValue(_Out_ int* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_MotionLink2_SWLimitPlusValue));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetMotionLink2SWLimitPlusValue(_In_ int nParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_MotionLink2_SWLimitPlusValue;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_MotionLink2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), MOTIONLINK2_SWLMTPLUSVALUE, nParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetMotionLink2SWLimitMinusValue(_Out_ int* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_MotionLink2_SWLimitMinusValue));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetMotionLink2SWLimitMinusValue(_In_ int nParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_MotionLink2_SWLimitMinusValue;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_MotionLink2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), MOTIONLINK2_SWLMTMINUSVALUE, nParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetMotionLink2SWLimitStopMethod(_Out_ EEziServo2PlusESWLimitStopMethod* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = (EEziServo2PlusESWLimitStopMethod)_ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_MotionLink2_SWLimitStopMethod));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetMotionLink2SWLimitStopMethod(_In_ EEziServo2PlusESWLimitStopMethod eParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_MotionLink2_SWLimitStopMethod;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_MotionLink2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(eParam < 0 || eParam >= EEziServo2PlusESWLimitStopMethod_Count)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), MOTIONLINK2_SOFTLMTSTOPMETHOD, eParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], g_lpszEziServo2PlusESWLimitStopMethod[nPreValue], g_lpszEziServo2PlusESWLimitStopMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetMotionLink2HWLimitStopMethod(_Out_ EEziServo2PlusESWLimitStopMethod* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = (EEziServo2PlusESWLimitStopMethod)_ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_MotionLink2_HWLimitStopMethod));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetMotionLink2HWLimitStopMethod(_In_ EEziServo2PlusESWLimitStopMethod eParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_MotionLink2_HWLimitStopMethod;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_MotionLink2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(eParam < 0 || eParam >= EEziServo2PlusESWLimitStopMethod_Count)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), MOTIONLINK2_HARDLMTSTOPMETHOD, eParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], g_lpszEziServo2PlusESWLimitStopMethod[nPreValue], g_lpszEziServo2PlusESWLimitStopMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetMotionLink2LimitSensorLogic(_Out_ EEziServo2PlusELogic* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = (EEziServo2PlusELogic)_ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_MotionLink2_LimitSensorLogic));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetMotionLink2LimitSensorLogic(_In_ EEziServo2PlusELogic eParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_MotionLink2_LimitSensorLogic;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_MotionLink2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(eParam < 0 || eParam >= EEziServo2PlusELogic_Count)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), MOTIONLINK2_LIMITSENSORLOGIC, eParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], g_lpszEziServo2PlusELogic[nPreValue], g_lpszEziServo2PlusELogic[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetMotionLink2OrgSpeed(_Out_ int* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_MotionLink2_OrgSpeed));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetMotionLink2OrgSpeed(_In_ int nParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_MotionLink2_OrgSpeed;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_MotionLink2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), MOTIONLINK2_ORGSPEED, nParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetMotionLink2OrgSearchSpeed(_Out_ int* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_MotionLink2_OrgSearchSpeed));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetMotionLink2OrgSearchSpeed(_In_ int nParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_MotionLink2_OrgSearchSpeed;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_MotionLink2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), MOTIONLINK2_ORGSEARCHSPEED, nParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetMotionLink2OrgAccDecTime(_Out_ int* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_MotionLink2_OrgAccDecTime));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetMotionLink2OrgAccDecTime(_In_ int nParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_MotionLink2_OrgAccDecTime;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_MotionLink2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), MOTIONLINK2_ORGACCDECTIME, nParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetMotionLink2OrgMethod(_Out_ EEziServo2PlusEMotionLinek2OrgMethod* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = (EEziServo2PlusEMotionLinek2OrgMethod)_ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_MotionLink2_OrgMethod));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetMotionLink2OrgMethod(_In_ EEziServo2PlusEMotionLinek2OrgMethod eParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_MotionLink2_OrgMethod;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_MotionLink2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(eParam < 0 || eParam >= EEziServo2PlusEMotionLinek2OrgMethod_Count)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), MOTIONLINK2_ORGMETHOD, eParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], g_lpszEziServo2PlusEMotionLink2OrgMethod[nPreValue], g_lpszEziServo2PlusEMotionLink2OrgMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetMotionLink2OrgDir(_Out_ EEziServo2PlusEDir* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = (EEziServo2PlusEDir)_ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_MotionLink2_OrgDir));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetMotionLink2OrgDir(_In_ EEziServo2PlusEDir eParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_MotionLink2_OrgDir;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_MotionLink2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(eParam < 0 || eParam >= EEziServo2PlusEDir_Count)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), MOTIONLINK2_ORGDIR, eParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], g_lpszEziServo2PlusEDir[nPreValue], g_lpszEziServo2PlusEDir[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetMotionLink2OrgOffSet(_Out_ int* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_MotionLink2_OrgOffSet));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetMotionLink2OrgOffSet(_In_ int nParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_MotionLink2_OrgOffSet;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_MotionLink2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), MOTIONLINK2_ORGOFFSET, nParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetMotionLink2OrgPositionSet(_Out_ int* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_MotionLink2_OrgPositionSet));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetMotionLink2OrgPositionSet(_In_ int nParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_MotionLink2_OrgPositionSet;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_MotionLink2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), MOTIONLINK2_ORGPOSITIONSET, nParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetMotionLink2OrgSensorLogic(_Out_ EEziServo2PlusELogic* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = (EEziServo2PlusELogic)_ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_MotionLink2_OrgSensorLogic));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetMotionLink2OrgSensorLogic(_In_ EEziServo2PlusELogic eParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_MotionLink2_OrgSensorLogic;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_MotionLink2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(eParam < 0 || eParam >= EEziServo2PlusELogic_Count)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), MOTIONLINK2_ORGSENSORLOGIC, eParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], g_lpszEziServo2PlusELogic[nPreValue], g_lpszEziServo2PlusELogic[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetMotionLink2LimitSensorDir(_Out_ EEziServo2PlusEDir* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = (EEziServo2PlusEDir)_ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_MotionLink2_LimitSensorDir));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetMotionLink2LimitSensorDir(_In_ EEziServo2PlusEDir eParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_MotionLink2_LimitSensorDir;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_MotionLink2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(eParam < 0 || eParam >= EEziServo2PlusEDir_Count)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), MOTIONLINK2_LIMITSENSORDIR, eParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], g_lpszEziServo2PlusEDir[nPreValue], g_lpszEziServo2PlusEDir[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetMotionLink2PulseType(_Out_ EEziServo2PlusEPulseType* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = (EEziServo2PlusEPulseType)_ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_MotionLink2_PulseType));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetMotionLink2PulseType(_In_ EEziServo2PlusEPulseType eParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_MotionLink2_PulseType;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_MotionLink2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(eParam < 0 || eParam >= EEziServo2PlusEPulseType_Count)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), MOTIONLINK2_PULSETYPE, eParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], g_lpszEziServo2PlusEPulseType[nPreValue], g_lpszEziServo2PlusEPulseType[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetMotionLink2EncoderDir(_Out_ EEziServo2PlusEDir* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = (EEziServo2PlusEDir)_ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_MotionLink2_EncoderDir));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetMotionLink2EncoderDir(_In_ EEziServo2PlusEDir eParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_MotionLink2_EncoderDir;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_MotionLink2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(eParam < 0 || eParam >= EEziServo2PlusEDir_Count)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), MOTIONLINK2_ENCODERDIR, eParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], g_lpszEziServo2PlusEDir[nPreValue], g_lpszEziServo2PlusEDir[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetMotionLink2MotionDir(_Out_ EEziServo2PlusEDir* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = (EEziServo2PlusEDir)_ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_MotionLink2_MotionDir));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetMotionLink2MotionDir(_In_ EEziServo2PlusEDir eParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_MotionLink2_MotionDir;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_MotionLink2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(eParam < 0 || eParam >= EEziServo2PlusEDir_Count)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), MOTIONLINK2_MOTIONDIR, eParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], g_lpszEziServo2PlusEDir[nPreValue], g_lpszEziServo2PlusEDir[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetMotionLink2ServoAlarmresetLogic(_Out_ EEziServo2PlusELogic* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = (EEziServo2PlusELogic)_ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_MotionLink2_ServoAlarmresetLogic));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetMotionLink2ServoAlarmresetLogic(_In_ EEziServo2PlusELogic eParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_MotionLink2_ServoAlarmresetLogic;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_MotionLink2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(eParam < 0 || eParam >= EEziServo2PlusELogic_Count)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), MOTIONLINK2_SERVOALARMRESETLOGIC, eParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], g_lpszEziServo2PlusELogic[nPreValue], g_lpszEziServo2PlusELogic[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetMotionLink2ServoOnOutputLogic(_Out_ EEziServo2PlusELogic* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = (EEziServo2PlusELogic)_ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_MotionLink2_ServoOnOutputLogic));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetMotionLink2ServoOnOutputLogic(_In_ EEziServo2PlusELogic eParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_MotionLink2_ServoOnOutputLogic;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_MotionLink2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(eParam < 0 || eParam >= EEziServo2PlusELogic_Count)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), MOTIONLINK2_SERVOONOUTPUTLOGIC, eParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], g_lpszEziServo2PlusELogic[nPreValue], g_lpszEziServo2PlusELogic[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetMotionLink2ServoAlarmLogic(_Out_ EEziServo2PlusELogic* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = (EEziServo2PlusELogic)_ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_MotionLink2_ServoAlarmLogic));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetMotionLink2ServoAlarmLogic(_In_ EEziServo2PlusELogic eParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_MotionLink2_ServoAlarmLogic;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_MotionLink2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(eParam < 0 || eParam >= EEziServo2PlusELogic_Count)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), MOTIONLINK2_SERVOALARMLOGIC, eParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], g_lpszEziServo2PlusELogic[nPreValue], g_lpszEziServo2PlusELogic[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEziServo2PlusEGetFunction CDeviceEziServo2PlusE::GetMotionLink2ServoInpositionLogic(_Out_ EEziServo2PlusELogic* pParam)
{
	EEziServo2PlusEGetFunction eReturn = EEziServo2PlusEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEziServo2PlusEGetFunction_NullptrError;
			break;
		}

		*pParam = (EEziServo2PlusELogic)_ttoi(GetParamValue(EDeviceParameterEziServo2PlusE_MotionLink2_ServoInpositionLogic));

		eReturn = EEziServo2PlusEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEziServo2PlusESetFunction CDeviceEziServo2PlusE::SetMotionLink2ServoInpositionLogic(_In_ EEziServo2PlusELogic eParam)
{
	EEziServo2PlusESetFunction eReturn = EEziServo2PlusESetFunction_UnknownError;

	EDeviceParameterEziServo2PlusE eSaveNum = EDeviceParameterEziServo2PlusE_MotionLink2_ServoInpositionLogic;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(m_eCurrentMode != EEziServo2PlusEMotionType_MotionLink2)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(eParam < 0 || eParam >= EEziServo2PlusELogic_Count)
		{
			eReturn = EEziServo2PlusESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			if(PE::FAS_SetParameter(_ttoi(GetDeviceID()), MOTIONLINK2_SERVOINPOSLOGIC, eParam))
			{
				eReturn = EEziServo2PlusESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EEziServo2PlusESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEziServo2PlusESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[eSaveNum], g_lpszEziServo2PlusELogic[nPreValue], g_lpszEziServo2PlusELogic[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceEziServo2PlusE::GetServoStatus()
{
	bool bReturn = false;

	do 
	{
		if(!IsInitialized())
			break;

		DWORD dwOutput = 0;
		if(PE::FAS_GetIOOutput(_ttoi(GetDeviceID()), &dwOutput) != FMM_OK)
			break;

		EEziServo2PlusEMotionType eType = EEziServo2PlusEMotionType_Count;

		if(GetMotionType(&eType))
			break;

		DWORD dwMask = 0x00000000;

		switch(eType)
		{
		case EEziServo2PlusEMotionType_Servo2:
			dwMask = SERVO2_OUT_BITMASK_SERVOREADY;
			break;
		case EEziServo2PlusEMotionType_MotionLink2:
			dwMask = STEP2_OUT_BITMASK_STEPREADY;
			break;
		default:
			break;
		}

		bReturn = (bool)(dwOutput & dwMask);		
	} 
	while(false);

	return bReturn;
}

bool CDeviceEziServo2PlusE::SetServoOn(bool bOn)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(PE::FAS_ServoEnable(_ttoi(GetDeviceID()), bOn) != FMM_OK)
			break;
				
		m_bServo = bOn;
		
		bReturn = true;		
	}
	while(false);

	return bReturn;
}

bool CDeviceEziServo2PlusE::GetCommandPosition(double* pPos)
{
	bool bReturn = false;

	do 
	{
		if(!pPos)
			break;

		if(!IsInitialized())
			break;

		long nPos = 0;

		if(PE::FAS_GetCommandPos(_ttoi(GetDeviceID()), &nPos) != FMM_OK)
			break;

		*(pPos) = nPos;
		
		bReturn = true;		
	} 
	while(false);

	return bReturn;
}

bool CDeviceEziServo2PlusE::SetCommandPositionClear()
{
	bool bReturn = false;

	do 
	{
		if(!IsInitialized())
			break;

		if(PE::FAS_SetCommandPos(_ttoi(GetDeviceID()), 0) != FMM_OK)
			break;

		bReturn = true;
	} 
	while(false);
	
	return bReturn;
}

bool CDeviceEziServo2PlusE::GetActualPosition(double* pPos)
{
	bool bReturn = false;

	do
	{
		if(!pPos)
			break;

		if(!IsInitialized())
			break;

		long nPos = 0;

		if(PE::FAS_GetActualPos(_ttoi(GetDeviceID()), &nPos) != FMM_OK)
			break;

		*(pPos) = nPos;
		
		bReturn = true;		
	}
	while(false);

	return bReturn;
}

bool CDeviceEziServo2PlusE::SetActualPositionClear()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(PE::FAS_SetActualPos(_ttoi(GetDeviceID()), 0) != FMM_OK)
			break;
		
		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceEziServo2PlusE::GetAlarmStatus()
{
	bool bReturn = false;

	do 
	{
		if(!IsInitialized())
			break;

		EZISERVO2_AXISSTATUS AxisStatus;
		; 
		if(PE::FAS_GetAxisStatus(_ttoi(GetDeviceID()), &(AxisStatus.dwValue)) != FMM_OK)
			break;



		if(AxisStatus.FFLAG_ERRORALL || AxisStatus.FFLAG_ERROVERCURRENT || AxisStatus.FFLAG_ERROVERLOAD)
			bReturn = true;
	} 
	while(false);

	return bReturn;
}

bool CDeviceEziServo2PlusE::SetAlarmClear()
{
	bool bReturn = false;

	do 
	{
		if(!IsInitialized())
			break;

		if(PE::FAS_ServoAlarmReset(_ttoi(GetDeviceID())) != FMM_OK)
			break;

		bReturn = true;
	} 
	while(false);

	return bReturn;
}

bool CDeviceEziServo2PlusE::GetInposition()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		DWORD dwOutput = 0;
		if(PE::FAS_GetIOOutput(_ttoi(GetDeviceID()), &dwOutput) != FMM_OK)
			break;

		EEziServo2PlusEMotionType eType = EEziServo2PlusEMotionType_Count;

		if(GetMotionType(&eType))
			break;

		DWORD dwMask = 0x00000000;

		switch(eType)
		{
		case EEziServo2PlusEMotionType_Servo2:
			dwMask = SERVO2_OUT_BITMASK_INPOSITION;
			break;
		case EEziServo2PlusEMotionType_MotionLink2:
			dwMask = STEP2_OUT_BITMASK_STEPREADY;
			break;
		default:
			break;
		}

		bReturn = (bool)(dwOutput & dwMask);
	}
	while(false);

	return bReturn;
}

bool CDeviceEziServo2PlusE::GetLimitSensorN()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		DWORD dwInput = 0;

		if(PE::FAS_GetIOInput(_ttoi(GetDeviceID()), &dwInput) != FMM_OK)
			break;

		EEziServo2PlusEMotionType eType = EEziServo2PlusEMotionType_Count;

		if(GetMotionType(&eType))
			break;

		DWORD dwMask = 0x00000000;

		switch(eType)
		{
		case EEziServo2PlusEMotionType_Servo2:
			dwMask = SERVO2_IN_BITMASK_LIMITN;
			break;
		case EEziServo2PlusEMotionType_MotionLink2:
			dwMask = STEP2_IN_BITMASK_LIMITN;
			break;
		default:
			break;
		}

		bReturn = (bool)(dwInput & dwMask);
	}
	while(false);

	return bReturn;
}

bool CDeviceEziServo2PlusE::GetLimitSensorP()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		DWORD dwInput = 0;

		if(PE::FAS_GetIOInput(_ttoi(GetDeviceID()), &dwInput) != FMM_OK)
			break;

		EEziServo2PlusEMotionType eType = EEziServo2PlusEMotionType_Count;

		if(GetMotionType(&eType))
			break;

		DWORD dwMask = 0x00000000;

		switch(eType)
		{
		case EEziServo2PlusEMotionType_Servo2:
			dwMask = SERVO2_IN_BITMASK_LIMITP;
			break;
		case EEziServo2PlusEMotionType_MotionLink2:
			dwMask = STEP2_IN_BITMASK_LIMITP;
			break;
		default:
			break;
		}

		bReturn = (bool)(dwInput & dwMask);
	}
	while(false);

	return bReturn;
}

bool CDeviceEziServo2PlusE::GetHomeSensor()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		DWORD dwInput = 0;

		if(PE::FAS_GetIOInput(_ttoi(GetDeviceID()), &dwInput) != FMM_OK)
			break;

		EEziServo2PlusEMotionType eType = EEziServo2PlusEMotionType_Count;

		if(GetMotionType(&eType))
			break;

		DWORD dwMask = 0x00000000;

		switch(eType)
		{
		case EEziServo2PlusEMotionType_Servo2:
			dwMask = SERVO2_IN_BITMASK_ORIGIN;
			break;
		case EEziServo2PlusEMotionType_MotionLink2:
			dwMask = STEP2_IN_BITMASK_ORIGIN;
			break;
		default:
			break;
		}
		
		bReturn = (bool)(dwInput & dwMask);
	}
	while(false);

	return bReturn;
}

bool CDeviceEziServo2PlusE::MovePosition(double dPos, double dVel, double dAcc, bool bSCurve)
{
	bool bReturn = false;

	do 
	{
		if(!IsInitialized())
			break;

		if(PE::FAS_MoveSingleAxisAbsPos(_ttoi(GetDeviceID()), (long)dPos, (DWORD)dVel) != FMM_OK)
			break;

		bReturn = true;
	} 
	while(false);
	
	return bReturn;
}

bool CDeviceEziServo2PlusE::MoveDistance(double dDist, double dVel, double dAcc, bool bSCurve)
{
	bool bReturn = false;

	do 
	{
		if(!IsInitialized())
			break;

		if(PE::FAS_MoveSingleAxisIncPos(_ttoi(GetDeviceID()), (long)dDist, (DWORD)dVel) != FMM_OK)
			break;

		bReturn = true;
	} 
	while(false);
	
	return bReturn;
}

bool CDeviceEziServo2PlusE::MoveVelocity(double dVel, double dAcc, bool bSCurve)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(dVel >= 0)
		{
			if(PE::FAS_MoveVelocity(_ttoi(GetDeviceID()), (DWORD)fabs(dVel), DIR_INC) != FMM_OK)
				break;
		}
		else
		{
			if(PE::FAS_MoveVelocity(_ttoi(GetDeviceID()), (DWORD)fabs(dVel), DIR_DEC) != FMM_OK)
				break;
		}

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceEziServo2PlusE::MoveJog(double dVel, double dAcl, BOOL bSCurve)
{
	bool bReturn = false;

	do 
	{
		if(!IsInitialized())
			break;

		if(dVel >= 0)
		{
			if(PE::FAS_MoveVelocity(_ttoi(GetDeviceID()), (DWORD)fabs(dVel), DIR_INC) != FMM_OK)
				break;
		}
		else
		{
			if(PE::FAS_MoveVelocity(_ttoi(GetDeviceID()), (DWORD)fabs(dVel), DIR_DEC) != FMM_OK)
				break;
		}

		bReturn = true;
	} 
	while(false);
	
	return bReturn;
}

bool CDeviceEziServo2PlusE::StopJog()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(PE::FAS_MoveStop(_ttoi(GetDeviceID())) != FMM_OK)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceEziServo2PlusE::IsMotionDone()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		EZISTEP_AXISSTATUS stAxisStatus;

		PE::FAS_GetAxisStatus(_ttoi(GetDeviceID()), &stAxisStatus.dwValue);

		bReturn = !stAxisStatus.FFLAG_MOTIONING;
	}
	while(false);

	return bReturn;
}

bool CDeviceEziServo2PlusE::MotorStop()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(PE::FAS_MoveStop(_ttoi(GetDeviceID())) != FMM_OK)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceEziServo2PlusE::MotorEStop()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(PE::FAS_EmergencyStop(_ttoi(GetDeviceID())) != FMM_OK)
			break;
		
		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceEziServo2PlusE::ChangeSpeed(double dSpeed)
{
	return false;
}

bool CDeviceEziServo2PlusE::WriteGenOutBit(int nBit, bool bOn)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		DWORD dwBitManOn = 0;
		DWORD dwBitManOff = 0;

		if(bOn)
			dwBitManOn = (1 << ReadOutputShift) << nBit;
		else
			dwBitManOff = (1 << ReadOutputShift) << nBit;

		if(PE::FAS_SetIOOutput(_ttoi(GetDeviceID()), dwBitManOn, dwBitManOff) != FMM_OK)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceEziServo2PlusE::ReadGenOutBit(int nBit)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;


		DWORD dwBitMan = 0;

		if(PE::FAS_GetIOOutput(_ttoi(GetDeviceID()), &dwBitMan) != FMM_OK)
			break;

		bReturn = ((dwBitMan >> ReadOutputShift) >> nBit) & 0x01;
	}
	while(false);

	return bReturn;
}

bool CDeviceEziServo2PlusE::ReadGenInputBit(int nBit)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		DWORD dwBitMan = 0;

		if(PE::FAS_GetIOInput(_ttoi(GetDeviceID()), &dwBitMan) != FMM_OK)
			break;
				
		if(dwBitMan > 2049)
			bReturn = ((dwBitMan >> ReadInputHighShift) >> nBit) & 0x01;
		else
			bReturn = ((dwBitMan >> ReadInputLowShift) >> nBit) & 0x01;
	}
	while(false);

	return bReturn;
}

bool CDeviceEziServo2PlusE::MoveToHome()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;
	
		if(PE::FAS_MoveOriginSingleAxis(_ttoi(GetDeviceID())) != FMM_OK)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

EDeviceMotionHommingStatus CDeviceEziServo2PlusE::GetHommingStatus()
{
	return EDeviceMotionHommingStatus();
}

long CDeviceEziServo2PlusE::GetOutputPortCount()
{
	return OutputPort;
}

long CDeviceEziServo2PlusE::GetInputPortCount()
{
	return InputPort;
}

bool CDeviceEziServo2PlusE::GetTriggerOutputStatus()
{
	bool bReturn = false;

	do 
	{
		if(!IsInitialized())
			break;

		BYTE cTriggerStatus = 0;

		if(PE::FAS_TriggerOutput_Status(_ttoi(GetDeviceID()), &cTriggerStatus) != FMM_OK)
			break;

		bReturn = cTriggerStatus;
	} 
	while(false);

	return bReturn;
}

bool CDeviceEziServo2PlusE::EnableTriggerOutput(long lStartPos, DWORD dwPeriod, DWORD dwPulseWidth)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(PE::FAS_TriggerOutput_RunA(_ttoi(GetDeviceID()), true, lStartPos, dwPeriod, dwPulseWidth) != FMM_OK)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceEziServo2PlusE::DisableTriggerOutput()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(PE::FAS_TriggerOutput_RunA(_ttoi(GetDeviceID()), false, 0, 0, 0) != FMM_OK)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceEziServo2PlusE::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = true;

	bool bFoundID = true;

	CString strMessage;

	do
	{
		switch(nParam)
		{
			case EDeviceParameterEziServo2PlusE_DeviceID:
				bReturn = !SetDeviceID(strValue);
				break;
			case EDeviceParameterEziServo2PlusE_IPAddress:
				bReturn = !SetIPAddress(strValue);
				break;
			case EDeviceParameterEziServo2PlusE_MotionType:
				bReturn = !SetMotionType((EEziServo2PlusEMotionType)_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_Servo2_PulsePerRevolution:
				bReturn = !SetServo2PulsePerRevolution(_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_Servo2_AxisMaxSpeed:
				bReturn = !SetServo2AxisMaxSpeed(_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_Servo2_AxisStartSpeed:
				bReturn = !SetServo2AxisStartSpeed(_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_Servo2_AxisAccTime:
				bReturn = !SetServo2AxisAccTime(_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_Servo2_AxisDecTime:
				bReturn = !SetServo2AxisDecTime(_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_Servo2_SpeedOverride:
				bReturn = !SetServo2SpeedOverride(_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_Servo2_JogSpeed:
				bReturn = !SetServo2JogSpeed(_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_Servo2_JogStartSpeed:
				bReturn = !SetServo2JogStartSpeed(_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_Servo2_JogAccDecTime:
				bReturn = !SetServo2JogAccDecTime(_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_Servo2_SWLimitPlusValue:
				bReturn = !SetServo2SWLimitPlusValue(_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_Servo2_SWLimitMinusValue:
				bReturn = !SetServo2SWLimitMinusValue(_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_Servo2_SWLimitStopMethod:
				bReturn = !SetServo2SWLimitStopMethod((EEziServo2PlusESWLimitStopMethod)_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_Servo2_HWLimitStopMethod:
				bReturn = !SetServo2HWLimitStopMethod((EEziServo2PlusEHWLimitStopMethod)_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_Servo2_LimitSensorLogic:
				bReturn = !SetServo2LimitSensorLogic((EEziServo2PlusELogic)_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_Servo2_OrgSpeed:
				bReturn = !SetServo2OrgSpeed(_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_Servo2_OrgSearchSpeed:
				bReturn = !SetServo2OrgSearchSpeed(_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_Servo2_OrgAccDecTime:
				bReturn = !SetServo2OrgAccDecTime(_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_Servo2_OrgMethod:
				bReturn = !SetServo2OrgMethod((EEziServo2PlusEOrgMethod)_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_Servo2_OrgDir:
				bReturn = !SetServo2OrgDir((EEziServo2PlusEDir)_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_Servo2_OrgOffSet:
				bReturn = !SetServo2OrgOffSet(_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_Servo2_OrgPositionSet:
				bReturn = !SetServo2OrgPositionSet(_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_Servo2_OrgSensorLogic:
				bReturn = !SetServo2OrgSensorLogic((EEziServo2PlusELogic)_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_Servo2_PositionLoopGain:
				bReturn = !SetServo2PositionLoopGain(_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_Servo2_InposValue:
				bReturn = !SetServo2InposValue(_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_Servo2_PosTrackingLimit:
				bReturn = !SetServo2PosTrackingLimit(_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_Servo2_MotionDir:
				bReturn = !SetServo2MotionDir((EEziServo2PlusEDir)_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_Servo2_LimitSensorDir:
				bReturn = !SetServo2LimitSensorDir((EEziServo2PlusEDir)_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_Servo2_OrgTorqueRatio:
				bReturn = !SetServo2OrgTorqueRatio(_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_Servo2_PosErrorOverflowLimit:
				bReturn = !SetServo2PosErrorOverflowLimit(_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_Servo2_BrakeDelayTime:
				bReturn = !SetServo2BrakeDelayTime(_ttoi(strValue));
				break; 
			case EDeviceParameterEziServo2PlusE_MotionLink2_EncoderMultiply:
				bReturn = !SetMotionLink2EncoderMultiply((EEziServo2PlusEEncoderMethod)_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_MotionLink2_AxisMaxSpeed:
				bReturn = !SetMotionLink2AxisMaxSpeed(_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_MotionLink2_AxisStartSpeed:
				bReturn = !SetMotionLink2AxisStartSpeed(_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_MotionLink2_AxisAccTime:
				bReturn = !SetMotionLink2AxisAccTime(_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_MotionLink2_AxisDecTime:
				bReturn = !SetMotionLink2AxisDecTime(_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_MotionLink2_SpeedOverride:
				bReturn = !SetMotionLink2SpeedOverride(_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_MotionLink2_JogSpeed:
				bReturn = !SetMotionLink2JogSpeed(_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_MotionLink2_JogStartSpeed:
				bReturn = !SetMotionLink2JogStartSpeed(_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_MotionLink2_JogAccDecTime:
				bReturn = !SetMotionLink2JogAccDecTime(_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_MotionLink2_SWLimitPlusValue:
				bReturn = !SetMotionLink2SWLimitPlusValue(_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_MotionLink2_SWLimitMinusValue:
				bReturn = !SetMotionLink2SWLimitMinusValue(_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_MotionLink2_SWLimitStopMethod:
				bReturn = !SetMotionLink2SWLimitStopMethod((EEziServo2PlusESWLimitStopMethod)_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_MotionLink2_HWLimitStopMethod:
				bReturn = !SetMotionLink2HWLimitStopMethod((EEziServo2PlusESWLimitStopMethod)_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_MotionLink2_LimitSensorLogic:
				bReturn = !SetMotionLink2LimitSensorLogic((EEziServo2PlusELogic)_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_MotionLink2_OrgSpeed:
				bReturn = !SetMotionLink2OrgSpeed(_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_MotionLink2_OrgSearchSpeed:
				bReturn = !SetMotionLink2OrgSearchSpeed(_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_MotionLink2_OrgAccDecTime:
				bReturn = !SetMotionLink2OrgAccDecTime(_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_MotionLink2_OrgMethod:
				bReturn = !SetMotionLink2OrgMethod((EEziServo2PlusEMotionLinek2OrgMethod)_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_MotionLink2_OrgDir:
				bReturn = !SetMotionLink2OrgDir((EEziServo2PlusEDir)_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_MotionLink2_OrgOffSet:
				bReturn = !SetMotionLink2OrgOffSet(_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_MotionLink2_OrgPositionSet:
				bReturn = !SetMotionLink2OrgPositionSet(_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_MotionLink2_OrgSensorLogic:
				bReturn = !SetMotionLink2OrgSensorLogic((EEziServo2PlusELogic)_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_MotionLink2_LimitSensorDir:
				bReturn = !SetMotionLink2LimitSensorDir((EEziServo2PlusEDir)_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_MotionLink2_PulseType:
				bReturn = !SetMotionLink2PulseType((EEziServo2PlusEPulseType)_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_MotionLink2_EncoderDir:
				bReturn = !SetMotionLink2EncoderDir((EEziServo2PlusEDir)_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_MotionLink2_MotionDir:
				bReturn = !SetMotionLink2MotionDir((EEziServo2PlusEDir)_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_MotionLink2_ServoAlarmresetLogic:
				bReturn = !SetMotionLink2ServoAlarmresetLogic((EEziServo2PlusELogic)_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_MotionLink2_ServoOnOutputLogic:
				bReturn = !SetMotionLink2ServoOnOutputLogic((EEziServo2PlusELogic)_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_MotionLink2_ServoAlarmLogic:
				bReturn = !SetMotionLink2ServoAlarmLogic((EEziServo2PlusELogic)_ttoi(strValue));
				break;
			case EDeviceParameterEziServo2PlusE_MotionLink2_ServoInpositionLogic:
				bReturn = !SetMotionLink2ServoInpositionLogic((EEziServo2PlusELogic)_ttoi(strValue));
				break;							   				 
			case EDeviceParameterEziServo2PlusE_UseInputNo0:
			case EDeviceParameterEziServo2PlusE_UseInputNo1:
			case EDeviceParameterEziServo2PlusE_UseInputNo2:
			case EDeviceParameterEziServo2PlusE_UseInputNo3:
			case EDeviceParameterEziServo2PlusE_UseInputNo4:
			case EDeviceParameterEziServo2PlusE_UseInputNo5:
			case EDeviceParameterEziServo2PlusE_UseInputNo6:
			case EDeviceParameterEziServo2PlusE_UseInputNo7:
			case EDeviceParameterEziServo2PlusE_UseInputNo8:
				{
					int nPreValue = _ttoi(GetParamValue(nParam));
					int nValue = _ttoi(strValue);

					if(SetParamValue(nParam, strValue))
					{
						m_pBUsingInputArray[nParam - EDeviceParameterEziServo2PlusE_UseInputNo0] = _ttoi(strValue);
						bReturn = true;

						bReturn = SaveSettings(nParam);
					}
					bFoundID = false;

					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[nParam], g_lpszEziServo2PlusESwitch[nPreValue], g_lpszEziServo2PlusESwitch[nValue]);
				}
				break;
			case EDeviceParameterEziServo2PlusE_UseOutputNo0:
			case EDeviceParameterEziServo2PlusE_UseOutputNo1:
			case EDeviceParameterEziServo2PlusE_UseOutputNo2:
			case EDeviceParameterEziServo2PlusE_UseOutputNo3:
			case EDeviceParameterEziServo2PlusE_UseOutputNo4:
			case EDeviceParameterEziServo2PlusE_UseOutputNo5:
			case EDeviceParameterEziServo2PlusE_UseOutputNo6:
			case EDeviceParameterEziServo2PlusE_UseOutputNo7:
			case EDeviceParameterEziServo2PlusE_UseOutputNo8:
				{
					int nPreValue = _ttoi(GetParamValue(nParam));
					int nValue = _ttoi(strValue);

					if(SetParamValue(nParam, strValue))
					{
						m_pBUsingOutputArray [nParam - EDeviceParameterEziServo2PlusE_UseOutputNo0] = _ttoi(strValue);

						bReturn = SaveSettings(nParam);
					}
					bFoundID = false;

					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[nParam], g_lpszEziServo2PlusESwitch[nPreValue], g_lpszEziServo2PlusESwitch[nValue]);
				}
				break;
			case EDeviceParameterEziServo2PlusE_InputNo0Name:
			case EDeviceParameterEziServo2PlusE_InputNo1Name:
			case EDeviceParameterEziServo2PlusE_InputNo2Name:
			case EDeviceParameterEziServo2PlusE_InputNo3Name:
			case EDeviceParameterEziServo2PlusE_InputNo4Name:
			case EDeviceParameterEziServo2PlusE_InputNo5Name:
			case EDeviceParameterEziServo2PlusE_InputNo6Name:
			case EDeviceParameterEziServo2PlusE_InputNo7Name:
			case EDeviceParameterEziServo2PlusE_InputNo8Name:
				{
					CString strPreValue = GetParamValue(nParam);

					if(SetParamValue(nParam, strValue))
					{
						m_pStrInputNameArray[nParam - EDeviceParameterEziServo2PlusE_InputNo0Name] = strValue;

						bReturn = SaveSettings(nParam);
					}
					bFoundID = false;

					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[nParam], strPreValue, strValue);
				}
				break;
			case EDeviceParameterEziServo2PlusE_OutputNo0Name:
			case EDeviceParameterEziServo2PlusE_OutputNo1Name:
			case EDeviceParameterEziServo2PlusE_OutputNo2Name:
			case EDeviceParameterEziServo2PlusE_OutputNo3Name:
			case EDeviceParameterEziServo2PlusE_OutputNo4Name:
			case EDeviceParameterEziServo2PlusE_OutputNo5Name:
			case EDeviceParameterEziServo2PlusE_OutputNo6Name:
			case EDeviceParameterEziServo2PlusE_OutputNo7Name:
			case EDeviceParameterEziServo2PlusE_OutputNo8Name:
				{
					CString strPreValue = GetParamValue(nParam);

					if(SetParamValue(nParam, strValue))
					{
						m_pStrOutputNameArray[nParam - EDeviceParameterEziServo2PlusE_OutputNo0Name] = strValue;

						bReturn = SaveSettings(nParam);
					}
					bFoundID = false;

					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamEziServo2PlusE[nParam], strPreValue, strValue);
				}
				break;
			default:
				bFoundID = false;
				strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtofindtheparameter);
				break;
		}
		
		if(EDeviceParameterEziServo2PlusE_UseInputNo0 <= nParam && EDeviceParameterMVTechDio_Count > nParam)
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

bool CDeviceEziServo2PlusE::DoesModuleExist()
{
	bool bReturn = false;

	CString strModuleName;

	do
	{
		strModuleName.Format(_T("EziMOTIONPlusE.dll"));
		
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

bool CDeviceEziServo2PlusE::AddControls()
{
	bool bReturn = false;
	CDC* pDC = nullptr;
	CFont* pOldFont = nullptr;

	do
	{
		pDC = GetDC();

		if(!pDC)
			break;

		for(auto iter = m_vctCtrl.begin(); iter != m_vctCtrl.end(); ++iter)
			delete *iter;

		m_vctCtrl.clear();

		CWnd* pWndParamTree = GetDlgItem(int32_t(RavidInterfaceControlID::DeviceSheetParameter));
		if(!pWndParamTree)
			break;

		CWnd* pWndInitBtn = GetDlgItem(int32_t(RavidInterfaceControlID::DeviceButtonInitialize));
		if(!pWndInitBtn)
			break;

		CWnd* pWndTerBtn = GetDlgItem(int32_t(RavidInterfaceControlID::DeviceButtonTerminate));
		if(!pWndTerBtn)
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

		pWndTerBtn->GetWindowRect(rrTerBtn);
		ScreenToClient(rrTerBtn);

		rrSheet.bottom = rrSheet.top + 250 * dblDPIScale;

		pWndParamTree->SetWindowPos(nullptr, 0, 0, rrSheet.GetWidth(), rrSheet.GetHeight(), SWP_NOMOVE);

		int nInputMaxLed = GetInputPortCount();
		int nOutputMaxLed = GetOutputPortCount();

		AllocateIO(nInputMaxLed, nOutputMaxLed);

		int nInputEnable = 0;
		int nOutputEnable = 0;

		for(int i = 0; i < nInputMaxLed; ++i)
		{
			if(m_pBUsingInputArray[i])
				++nInputEnable;
		}

		for(int i = 0; i < nOutputMaxLed; ++i)
		{
			if(m_pBUsingOutputArray [i])
				++nOutputEnable;
		}

		int nDialogEdgeMargin = 10 * dblDPIScale;

		CRavidRect<int> rrGroupMotion;

		CRavidPoint<int> rpSheetLT;
		CRavidPoint<int> rpSheetRB;

		if(!nInputEnable && !nOutputEnable)
		{
			rpSheetLT.SetPoint(rrSheet.left, rrSheet.bottom + nDialogEdgeMargin);
			rpSheetRB.SetPoint(rpSheetLT.x + rrSheet.right - rrSheet.left, rrSheet.bottom + nDialogEdgeMargin);
		}
		else
		{
			rpSheetLT.SetPoint(rrSheet.right + nDialogEdgeMargin, rrSheet.top - nDialogEdgeMargin);
			rpSheetRB.SetPoint(rpSheetLT.x + rrSheet.right - rrSheet.left, rrSheet.bottom + nDialogEdgeMargin);

			CRavidRect<int> rrResultGroupBox;

			CreateLedArray(dblDPIScale, pBaseFont, rpSheetLT, rpSheetRB, rrResultGroupBox);


			rpSheetLT.SetPoint(rrSheet.right + nDialogEdgeMargin, rrResultGroupBox.bottom + nDialogEdgeMargin);
			rpSheetRB.SetPoint(rpSheetLT.x + rrSheet.right - rrSheet.left, rpSheetLT.y + nDialogEdgeMargin);
		}
			   		 	  
		CreateMotionControl(dblDPIScale, pBaseFont, rpSheetLT, rpSheetRB, rrGroupMotion);

		rrDlg.right = rrGroupMotion.right + nDialogEdgeMargin;
		rrDlg.bottom = rrGroupMotion.bottom + nDialogEdgeMargin;		

		if(nInputEnable || nOutputEnable)
			pWndParamTree->SetWindowPos(nullptr, 0, 0, rrSheet.GetWidth(), rrDlg.bottom - rrSheet.top - nDialogEdgeMargin, SWP_NOMOVE);

		RecalcSheet();

		SetDefaultDialogRect(rrDlg);

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

bool CDeviceEziServo2PlusE::CreateMotionControl(double dblDPIScale, CFont* pBaseFont, CRavidPoint<int> rpLeftTopPt, CRavidPoint<int> rpRightTopPt, CRavidRect<int>& rrResultGroupBox)
{
	bool bReturn = false;

	LockWindowUpdate();

	do 
	{
		int nDialogEdgeMargin = 10 * dblDPIScale;
		int nButtonInterval = 5 * dblDPIScale;
		int nInfoTextHeight = 20 * dblDPIScale;
		int nGroupBoxVerticalMargin = 25 * dblDPIScale;
		int nMoveHeight = 45 * dblDPIScale;
		int nLedLength = 15 * dblDPIScale;

		CRavidRect<int> rrGrpControl, rrGrpStatus, rrGrpCmdPos, rrGrpActPos, rrGrpAlarmReset;
		CRavidRect<int> rrBtnMinus, rrBtnPause, rrBtnPlus;
		CRavidRect<int> rrRadioJog, rrRadioMove, rrRadioPosition;
		CRavidRect<int> rrCaptionVel, rrCaptionAccel, rrCaptionDistance, rrCaptionPosition;
		CRavidRect<int> rrEditVel, rrEditAccel, rrEditDistance, rrEditPosition;
		CRavidRect<int> rrLedRect(0, 0, nLedLength, nLedLength);
		CRavidRect<int> rrCaptionStatus[5];
		CRavidRect<int> rrCaptionCmdValue, rrCaptionActValue;
		CRavidRect<int> rrBtnCmdReset, rrBtnActReset, rrBtnAlarmReset;
		CRavidRect<int> rrBtnServoOn, rrBtnServoOff, rrGrpServo, rrBtnHome, rrGrpHome;

		int nGroupIntrvalWidth = (rpRightTopPt.x - rpLeftTopPt.x - (nDialogEdgeMargin * 2)) / 10;
		int nGroupIntrvalWidth2 = nGroupIntrvalWidth * 4.125;

		rrGrpControl.left = rpLeftTopPt.x;
		rrGrpControl.top = rpLeftTopPt.y;
		rrGrpControl.right = rpRightTopPt.x;
		rrGrpControl.bottom = rrGrpControl.top + (nGroupBoxVerticalMargin + nInfoTextHeight * 4.0);

		rrGrpStatus.left = rpLeftTopPt.x;
		rrGrpStatus.top = rrGrpControl.bottom + nDialogEdgeMargin;
		rrGrpStatus.right = rrGrpStatus.left + nGroupIntrvalWidth * 6 + nDialogEdgeMargin;
		rrGrpStatus.bottom = rrGrpStatus.top + (nGroupBoxVerticalMargin + nInfoTextHeight * 2);

		rrGrpHome.left = rrGrpStatus.right + nDialogEdgeMargin;
		rrGrpHome.top = rrGrpStatus.top;
		rrGrpHome.right = rrGrpHome.left + nGroupIntrvalWidth * 2;
		rrGrpHome.bottom = rrGrpStatus.bottom;

		rrGrpServo.left = rrGrpHome.right + nDialogEdgeMargin;
		rrGrpServo.top = rrGrpStatus.top;
		rrGrpServo.right = rpRightTopPt.x;
		rrGrpServo.bottom = rrGrpStatus.bottom;

		//////////////////////////////////////////////////////////////////////////
		// move button

		int nBtnSize = 48 * dblDPIScale;

		rrBtnMinus.left = rpLeftTopPt.x + nDialogEdgeMargin;
		rrBtnMinus.top = rrGrpControl.top + 32 * dblDPIScale;
		rrBtnMinus.right = rrBtnMinus.left + nBtnSize;
		rrBtnMinus.bottom = rrBtnMinus.top + nBtnSize;

		rrBtnPause.left = rrBtnMinus.right + nDialogEdgeMargin;
		rrBtnPause.top = rrBtnMinus.top;
		rrBtnPause.right = rrBtnPause.left + nBtnSize;
		rrBtnPause.bottom = rrBtnMinus.bottom;

		rrBtnPlus.left = rrBtnPause.right + nDialogEdgeMargin;
		rrBtnPlus.top = rrBtnMinus.top;
		rrBtnPlus.right = rrBtnPlus.left + nBtnSize;
		rrBtnPlus.bottom = rrBtnMinus.bottom;

		//////////////////////////////////////////////////////////////////////////
		// Radio button
			   		 
		rrRadioJog.left = rrBtnPlus.right + nDialogEdgeMargin;
		rrRadioJog.top = rrGrpControl.top + 23 * dblDPIScale;
		rrRadioJog.right = rrRadioJog.left + nBtnSize + 10;
		rrRadioJog.bottom = rrRadioJog.top + nInfoTextHeight;

		rrRadioMove.left = rrRadioJog.left;
		rrRadioMove.top = rrRadioJog.bottom + 5 * dblDPIScale;
		rrRadioMove.right = rrRadioJog.right;
		rrRadioMove.bottom = rrRadioMove.top + nInfoTextHeight;

		rrRadioPosition.left = rrRadioJog.left;
		rrRadioPosition.top = rrRadioMove.bottom + 5 * dblDPIScale;
		rrRadioPosition.right = rrRadioJog.right;
		rrRadioPosition.bottom = rrRadioPosition.top + nInfoTextHeight;

		//////////////////////////////////////////////////////////////////////////
		// vel, acc, dis caption

		int nCaptionWidth = 70 * dblDPIScale;

		rrCaptionVel.left = rrRadioJog.right + nDialogEdgeMargin;
		rrCaptionVel.top = rrGrpControl.top + 14 * dblDPIScale;
		rrCaptionVel.right = rrCaptionVel.left + nCaptionWidth;
		rrCaptionVel.bottom = rrCaptionVel.top + nInfoTextHeight;

		rrCaptionAccel.left = rrCaptionVel.left;
		rrCaptionAccel.top = rrCaptionVel.bottom + 2 * dblDPIScale;
		rrCaptionAccel.right = rrCaptionVel.right;
		rrCaptionAccel.bottom = rrCaptionAccel.top + nInfoTextHeight;

		rrCaptionDistance.left = rrCaptionVel.left;
		rrCaptionDistance.top = rrCaptionAccel.bottom + 2 * dblDPIScale;
		rrCaptionDistance.right = rrCaptionVel.right;
		rrCaptionDistance.bottom = rrCaptionDistance.top + nInfoTextHeight;

		rrCaptionPosition.left = rrCaptionVel.left;
		rrCaptionPosition.top = rrCaptionDistance.bottom + 2 * dblDPIScale;
		rrCaptionPosition.right = rrCaptionVel.right;
		rrCaptionPosition.bottom = rrCaptionPosition.top + nInfoTextHeight;

		//////////////////////////////////////////////////////////////////////////
		// vel, acc, dis editbox

		int nEditWidth = 120 * dblDPIScale;
		int nEditInterval = 2 * dblDPIScale;

		rrEditVel.left = rrCaptionVel.right + nDialogEdgeMargin;
		rrEditVel.top = rrCaptionVel.top - nEditInterval;
		rrEditVel.right = rrGrpControl.right - nDialogEdgeMargin;
		rrEditVel.bottom = rrCaptionVel.bottom - nEditInterval;

		rrEditAccel.left = rrEditVel.left;
		rrEditAccel.top = rrCaptionAccel.top - nEditInterval;
		rrEditAccel.right = rrEditVel.right;
		rrEditAccel.bottom = rrCaptionAccel.bottom - nEditInterval;

		rrEditDistance.left = rrEditVel.left;
		rrEditDistance.top = rrCaptionDistance.top - nEditInterval;
		rrEditDistance.right = rrEditVel.right;
		rrEditDistance.bottom = rrCaptionDistance.bottom - nEditInterval;

		rrEditPosition.left = rrEditVel.left;
		rrEditPosition.top = rrCaptionPosition.top - nEditInterval;
		rrEditPosition.right = rrEditVel.right;
		rrEditPosition.bottom = rrCaptionPosition.bottom - nEditInterval;

		CButton* pBtnGroupCtrl = new CButton;

		if(!pBtnGroupCtrl)
			break;

		pBtnGroupCtrl->Create(CMultiLanguageManager::GetString(ELanguageParameter_MotorControlling), BS_GROUPBOX | WS_CHILD | WS_VISIBLE, rrGrpControl, this, EDeviceAddEnumeratedControlID_StaticStart + 2);
		pBtnGroupCtrl->SetFont(pBaseFont);
		m_vctCtrl.push_back(pBtnGroupCtrl);

		CButton* pBtnGroupStatus = new CButton;

		if(!pBtnGroupStatus)
			break;

		pBtnGroupStatus->Create(CMultiLanguageManager::GetString(ELanguageParameter_SensorStatus), BS_GROUPBOX | WS_CHILD | WS_VISIBLE, rrGrpStatus, this, EDeviceAddEnumeratedControlID_StaticStart + 3);
		pBtnGroupStatus->SetFont(pBaseFont);
		m_vctCtrl.push_back(pBtnGroupStatus);

		CButton* pBtnGroupServo = new CButton;

		if(!pBtnGroupServo)
			break;

		pBtnGroupServo->Create(CMultiLanguageManager::GetString(ELanguageParameter_ServoMode), BS_GROUPBOX | WS_CHILD | WS_VISIBLE, rrGrpServo, this, EDeviceAddEnumeratedControlID_StaticStart + 4);
		pBtnGroupServo->SetFont(pBaseFont);
		m_vctCtrl.push_back(pBtnGroupServo);


		CButton* pBtnGroupHome = new CButton;

		if(!pBtnGroupHome)
			break;

		pBtnGroupHome->Create(CMultiLanguageManager::GetString(ELanguageParameter_Home), BS_GROUPBOX | WS_CHILD | WS_VISIBLE, rrGrpHome, this, EDeviceAddEnumeratedControlID_StaticStart + 5);
		pBtnGroupHome->SetFont(pBaseFont);
		m_vctCtrl.push_back(pBtnGroupHome);


		CButton* pBtnMinus = new CButton;

		if(!pBtnMinus)
			break;

		pBtnMinus->Create(_T("¢¸"), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_FLAT, rrBtnMinus, this, EDeviceAddControlID_BtnMinus);
		pBtnMinus->SetFont(pBaseFont);
		m_vctCtrl.push_back(pBtnMinus);

		CButton* pBtnPause = new CButton;

		if(!pBtnPause)
			break;

		pBtnPause->Create(_T("¡á"), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_FLAT, rrBtnPause, this, EDeviceAddControlID_BtnPause);
		pBtnPause->SetFont(pBaseFont);
		m_vctCtrl.push_back(pBtnPause);

		CButton* pBtnPlus = new CButton;

		if(!pBtnPlus)
			break;

		pBtnPlus->Create(_T("¢º"), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_FLAT, rrBtnPlus, this, EDeviceAddControlID_BtnPlus);
		pBtnPlus->SetFont(pBaseFont);
		m_vctCtrl.push_back(pBtnPlus);

		CButton* pBtnJog = new CButton;

		if(!pBtnJog)
			break;

		pBtnJog->Create(_T("Jog"), WS_CHILD | WS_VISIBLE | WS_GROUP | BS_AUTORADIOBUTTON | WS_TABSTOP, rrRadioJog, this, EDeviceAddControlID_BtnJog);
		pBtnJog->SetFont(pBaseFont);
		pBtnJog->SetCheck(true);
		m_vctCtrl.push_back(pBtnJog);

		CButton* pBtnMove = new CButton;

		if(!pBtnMove)
			break;

		pBtnMove->Create(_T("Move"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_TABSTOP, rrRadioMove, this, EDeviceAddControlID_BtnMove);
		pBtnMove->SetFont(pBaseFont);
		m_vctCtrl.push_back(pBtnMove);

		CButton* pBtnPosition = new CButton;

		if(!pBtnMove)
			break;

		pBtnPosition->Create(_T("Position"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_TABSTOP, rrRadioPosition, this, EDeviceAddControlID_BtnPosition);
		pBtnPosition->SetFont(pBaseFont);
		m_vctCtrl.push_back(pBtnPosition);

		CStatic* pCtnVel = new CStatic;

		if(!pCtnVel)
			break;

		pCtnVel->Create(CMultiLanguageManager::GetString(ELanguageParameter_Velocity) + _T(" :"), WS_CHILD | WS_VISIBLE | DT_RIGHT, rrCaptionVel, this, EDeviceAddEnumeratedControlID_StaticStart + 6);
		pCtnVel->SetFont(pBaseFont);
		m_vctCtrl.push_back(pCtnVel);

		CStatic* pCtnAccel = new CStatic;

		if(!pCtnAccel)
			break;

		pCtnAccel->Create(CMultiLanguageManager::GetString(ELanguageParameter_Acceleration) + _T(" :"), WS_CHILD | WS_VISIBLE | DT_RIGHT, rrCaptionAccel, this, EDeviceAddEnumeratedControlID_StaticStart + 7);
		pCtnAccel->SetFont(pBaseFont);
		m_vctCtrl.push_back(pCtnAccel);

		CStatic* pCtnDistance = new CStatic;

		if(!pCtnDistance)
			break;

		pCtnDistance->Create(CMultiLanguageManager::GetString(ELanguageParameter_Distance) + _T(" :"), WS_CHILD | WS_VISIBLE | DT_RIGHT, rrCaptionDistance, this, EDeviceAddEnumeratedControlID_StaticStart + 8);
		pCtnDistance->SetFont(pBaseFont);
		m_vctCtrl.push_back(pCtnDistance);

		CStatic* pCtnPosition= new CStatic;

		if(!pCtnPosition)
			break;

		pCtnPosition->Create(CMultiLanguageManager::GetString(ELanguageParameter_Position) + _T(" :"), WS_CHILD | WS_VISIBLE | DT_RIGHT, rrCaptionPosition, this, EDeviceAddEnumeratedControlID_StaticStart + 9);
		pCtnPosition->SetFont(pBaseFont);
		m_vctCtrl.push_back(pCtnPosition);

		CEdit* pEditVel = new CEdit;

		if(!pEditVel)
			break;

		pEditVel->Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_LEFT, rrEditVel, this, EDeviceAddControlID_EditVelocity);
		pEditVel->SetFont(pBaseFont);
		pEditVel->SetWindowTextW(_T("0.000000"));
		m_vctCtrl.push_back(pEditVel);

		CEdit* pEditAccel = new CEdit;

		if(!pEditAccel)
			break;

		pEditAccel->Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_LEFT, rrEditAccel, this, EDeviceAddControlID_EditAcceleration);
		pEditAccel->SetFont(pBaseFont);
		pEditAccel->SetWindowTextW(_T("0.000000"));
		m_vctCtrl.push_back(pEditAccel);

		CEdit* pEditDistance = new CEdit;

		if(!pEditDistance)
			break;

		pEditDistance->Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_LEFT, rrEditDistance, this, EDeviceAddControlID_EditDistance);
		pEditDistance->SetFont(pBaseFont);
		pEditDistance->SetWindowTextW(_T("0.000000"));
		m_vctCtrl.push_back(pEditDistance);

		CEdit* pEditPosition = new CEdit;

		if(!pEditPosition)
			break;

		pEditPosition->Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_LEFT, rrEditPosition, this, EDeviceAddControlID_EditPosition);
		pEditPosition->SetFont(pBaseFont);
		pEditPosition->SetWindowTextW(_T("0.000000"));
		m_vctCtrl.push_back(pEditPosition);

		//////////////////////////////////////////////////////////////////////////
		// LED Box

		bool bError = false;
		int nLedPosY = 22 * dblDPIScale;
		int nLedIntervalWidth = (rrGrpStatus.right - rrGrpStatus.left) * 0.1;
		int nLedWidth = nLedIntervalWidth * 2;
		int nHalfLedSize = 7 * dblDPIScale;

		rrLedRect.Offset(rrGrpStatus.left + nLedIntervalWidth - nHalfLedSize, rrGrpStatus.top + nLedPosY);

		int nCaptionStart = nBtnSize * 0.5 - nHalfLedSize;

		for(int i = 0; i < 5; ++i)
		{
			CRavidLedCtrl* pLedCtrl = new CRavidLedCtrl;

			if(!pLedCtrl)
			{
				bError = true;
				break;
			}

			pLedCtrl->Create(nullptr, WS_VISIBLE | WS_CHILD | SS_NOTIFY, rrLedRect, this, EDeviceAddControlID_LedLimitPlus + i);
			pLedCtrl->SetFont(pBaseFont);
			m_vctCtrl.push_back(pLedCtrl);

			rrCaptionStatus[i].left = rrLedRect.left - nCaptionStart + 1;
			rrCaptionStatus[i].top = rrLedRect.bottom + nButtonInterval;
			rrCaptionStatus[i].right = rrCaptionStatus[i].left + nBtnSize;
			rrCaptionStatus[i].bottom = rrCaptionStatus[i].top + nInfoTextHeight;

			rrLedRect.left += nLedWidth;
			rrLedRect.right += nLedWidth;
		}

		if(bError)
			break;

		CStatic* pCtnStatusPluse = new CStatic;

		if(!pCtnStatusPluse)
			break;

		pCtnStatusPluse->Create(CMultiLanguageManager::GetString(ELanguageParameter_LimitPlus), WS_CHILD | WS_VISIBLE | ES_CENTER, rrCaptionStatus[0], this, EDeviceAddEnumeratedControlID_StaticStart + 9);
		pCtnStatusPluse->SetFont(pBaseFont);
		m_vctCtrl.push_back(pCtnStatusPluse);

		CStatic* pCtnStatusHome = new CStatic;

		if(!pCtnStatusHome)
			break;

		pCtnStatusHome->Create(CMultiLanguageManager::GetString(ELanguageParameter_Home), WS_CHILD | WS_VISIBLE | ES_CENTER, rrCaptionStatus[1], this, EDeviceAddEnumeratedControlID_StaticStart + 11);
		pCtnStatusHome->SetFont(pBaseFont);
		m_vctCtrl.push_back(pCtnStatusHome);

		CStatic* pCtnStatusMinus = new CStatic;

		if(!pCtnStatusMinus)
			break;

		pCtnStatusMinus->Create(CMultiLanguageManager::GetString(ELanguageParameter_LimitMinus), WS_CHILD | WS_VISIBLE | ES_CENTER, rrCaptionStatus[2], this, EDeviceAddEnumeratedControlID_StaticStart + 12);
		pCtnStatusMinus->SetFont(pBaseFont);
		m_vctCtrl.push_back(pCtnStatusMinus);

		CStatic* pCtnStatusAlarm = new CStatic;

		if(!pCtnStatusAlarm)
			break;

		pCtnStatusAlarm->Create(CMultiLanguageManager::GetString(ELanguageParameter_Alarm), WS_CHILD | WS_VISIBLE | ES_CENTER, rrCaptionStatus[3], this, EDeviceAddEnumeratedControlID_StaticStart + 13);
		pCtnStatusAlarm->SetFont(pBaseFont);
		m_vctCtrl.push_back(pCtnStatusAlarm);

		CStatic* pCtnStatusInPos = new CStatic;

		if(!pCtnStatusInPos)
			break;

		pCtnStatusInPos->Create(CMultiLanguageManager::GetString(ELanguageParameter_Inposition), WS_CHILD | WS_VISIBLE | ES_CENTER, rrCaptionStatus[4], this, EDeviceAddEnumeratedControlID_StaticStart + 14);
		pCtnStatusInPos->SetFont(pBaseFont);
		m_vctCtrl.push_back(pCtnStatusInPos);

		rrBtnHome.left = rrGrpHome.left + nDialogEdgeMargin;
		rrBtnHome.top = rrGrpControl.bottom + nDialogEdgeMargin + nInfoTextHeight;
		rrBtnHome.right = rrGrpHome.right - nDialogEdgeMargin;
		rrBtnHome.bottom = rrGrpHome.bottom - nDialogEdgeMargin;

		int nHalfServoWidth = ((rrGrpServo.right - nDialogEdgeMargin) - (rrGrpServo.left + nDialogEdgeMargin)) / 2.;

		rrBtnServoOff.left = rrGrpServo.left + nDialogEdgeMargin;
		rrBtnServoOff.top = rrGrpControl.bottom + nDialogEdgeMargin + nInfoTextHeight;
		rrBtnServoOff.right = rrBtnServoOff.left + nHalfServoWidth;
		rrBtnServoOff.bottom = rrGrpServo.bottom - nDialogEdgeMargin;

		rrBtnServoOn.left = rrBtnServoOff.right;
		rrBtnServoOn.top = rrBtnServoOff.top;
		rrBtnServoOn.right = rrBtnServoOn.left + nHalfServoWidth;
		rrBtnServoOn.bottom = rrBtnServoOff.bottom;

		CButton* pBtnHome = new CButton;

		if(!pBtnHome)
			break;

		pBtnHome->Create(CMultiLanguageManager::GetString(ELanguageParameter_Home), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_FLAT, rrBtnHome, this, EDeviceAddControlID_BtnHomming);
		pBtnHome->SetFont(pBaseFont);
		m_vctCtrl.push_back(pBtnHome);

		CButton* pBtnServoOff = new CButton;

		if(!pBtnServoOff)
			break;

		pBtnServoOff->Create(CMultiLanguageManager::GetString(ELanguageParameter_Off), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_FLAT, rrBtnServoOff, this, EDeviceAddControlID_BtnServoModeOff);
		pBtnServoOff->SetFont(pBaseFont);
		m_vctCtrl.push_back(pBtnServoOff);

		CButton* pBtnServoOn = new CButton;

		if(!pBtnServoOn)
			break;

		pBtnServoOn->Create(CMultiLanguageManager::GetString(ELanguageParameter_On), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_FLAT, rrBtnServoOn, this, EDeviceAddControlID_BtnServoModeOn);
		pBtnServoOn->SetFont(pBaseFont);
		m_vctCtrl.push_back(pBtnServoOn);


		//////////////////////////////////////////////////////////////////////////
		// command Group

		rrGrpCmdPos.left = rpLeftTopPt.x;
		rrGrpCmdPos.top = rrGrpStatus.bottom + nDialogEdgeMargin;
		rrGrpCmdPos.right = rrGrpCmdPos.left + nGroupIntrvalWidth2;
		rrGrpCmdPos.bottom = rrGrpCmdPos.top + (nGroupBoxVerticalMargin + nInfoTextHeight * 2.2);

		rrGrpActPos.left = rrGrpCmdPos.right + nDialogEdgeMargin;
		rrGrpActPos.top = rrGrpCmdPos.top;
		rrGrpActPos.right = rrGrpActPos.left + nGroupIntrvalWidth2;
		rrGrpActPos.bottom = rrGrpCmdPos.bottom;

		rrGrpAlarmReset.left = rrGrpActPos.right + nDialogEdgeMargin;
		rrGrpAlarmReset.top = rrGrpCmdPos.top;
		rrGrpAlarmReset.right = rpRightTopPt.x;
		rrGrpAlarmReset.bottom = rrGrpCmdPos.bottom;

		//////////////////////////////////////////////////////////////////////////
		// button caption

		rrCaptionCmdValue.left = rrGrpCmdPos.left + nDialogEdgeMargin;
		rrCaptionCmdValue.top = rrGrpCmdPos.top + nInfoTextHeight;
		rrCaptionCmdValue.right = rrGrpCmdPos.right - nDialogEdgeMargin;
		rrCaptionCmdValue.bottom = rrCaptionCmdValue.top + nInfoTextHeight;

		rrBtnCmdReset.left = rrCaptionCmdValue.left;
		rrBtnCmdReset.top = rrCaptionCmdValue.bottom;
		rrBtnCmdReset.right = rrCaptionCmdValue.right;
		rrBtnCmdReset.bottom = rrBtnCmdReset.top + nInfoTextHeight;

		rrCaptionActValue.left = rrGrpActPos.left + nDialogEdgeMargin;
		rrCaptionActValue.top = rrGrpActPos.top + nInfoTextHeight;
		rrCaptionActValue.right = rrGrpActPos.right - nDialogEdgeMargin;
		rrCaptionActValue.bottom = rrCaptionActValue.top + nInfoTextHeight;

		rrBtnActReset.left = rrCaptionActValue.left;
		rrBtnActReset.top = rrCaptionActValue.bottom;
		rrBtnActReset.right = rrCaptionActValue.right;
		rrBtnActReset.bottom = rrBtnActReset.top + nInfoTextHeight;

		rrBtnAlarmReset.left = rrGrpAlarmReset.left + nDialogEdgeMargin;
		rrBtnAlarmReset.top = rrCaptionActValue.top;
		rrBtnAlarmReset.right = rrGrpAlarmReset.right - nDialogEdgeMargin;
		rrBtnAlarmReset.bottom = rrBtnActReset.bottom;

		CButton* pBtnGroupCmdPos = new CButton;

		if(!pBtnGroupCmdPos)
			break;

		pBtnGroupCmdPos->Create(CMultiLanguageManager::GetString(ELanguageParameter_CommandPosition), BS_GROUPBOX | WS_CHILD | WS_VISIBLE, rrGrpCmdPos, this, EDeviceAddEnumeratedControlID_StaticStart + 15);
		pBtnGroupCmdPos->SetFont(pBaseFont);
		m_vctCtrl.push_back(pBtnGroupCmdPos);

		CButton* pBtnGroupActPos = new CButton;

		if(!pBtnGroupActPos)
			break;

		pBtnGroupActPos->Create(CMultiLanguageManager::GetString(ELanguageParameter_ActualPosition), BS_GROUPBOX | WS_CHILD | WS_VISIBLE, rrGrpActPos, this, EDeviceAddEnumeratedControlID_StaticStart + 16);
		pBtnGroupActPos->SetFont(pBaseFont);
		m_vctCtrl.push_back(pBtnGroupActPos);

		CButton* pBtnGroupAlarm = new CButton;

		if(!pBtnGroupAlarm)
			break;

		pBtnGroupAlarm->Create(CMultiLanguageManager::GetString(ELanguageParameter_Alarm), BS_GROUPBOX | WS_CHILD | WS_VISIBLE, rrGrpAlarmReset, this, EDeviceAddEnumeratedControlID_StaticStart + 17);
		pBtnGroupAlarm->SetFont(pBaseFont);
		m_vctCtrl.push_back(pBtnGroupAlarm);

		CStatic* pSttCommandPosition = new CStatic;

		if(!pSttCommandPosition)
			break;

		pSttCommandPosition->Create(CMultiLanguageManager::GetString(ELanguageParameter_Value) + _T(" : 0.0000"), WS_CHILD | WS_VISIBLE, rrCaptionCmdValue, this, EDeviceAddControlID_CommandPosition);
		pSttCommandPosition->SetFont(pBaseFont);
		m_vctCtrl.push_back(pSttCommandPosition);

		CButton* pBtnCommandReset = new CButton;

		if(!pBtnCommandReset)
			break;

		pBtnCommandReset->Create(CMultiLanguageManager::GetString(ELanguageParameter_Reset), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_FLAT, rrBtnCmdReset, this, EDeviceAddControlID_BtnCommandReset);
		pBtnCommandReset->SetFont(pBaseFont);
		m_vctCtrl.push_back(pBtnCommandReset);

		CStatic* pSttActualPosition = new CStatic;

		if(!pSttActualPosition)
			break;

		pSttActualPosition->Create(CMultiLanguageManager::GetString(ELanguageParameter_Value) + _T(" : 0.0000"), WS_CHILD | WS_VISIBLE, rrCaptionActValue, this, EDeviceAddControlID_ActualPosition);
		pSttActualPosition->SetFont(pBaseFont);
		m_vctCtrl.push_back(pSttActualPosition);

		CButton* pBtnActualReset = new CButton;

		if(!pBtnActualReset)
			break;

		pBtnActualReset->Create(CMultiLanguageManager::GetString(ELanguageParameter_Reset), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_FLAT, rrBtnActReset, this, EDeviceAddControlID_BtnActualReset);
		pBtnActualReset->SetFont(pBaseFont);
		m_vctCtrl.push_back(pBtnActualReset);

		CButton* pBtnAlarmReset = new CButton;

		if(!pBtnAlarmReset)
			break;

		pBtnAlarmReset->Create(CMultiLanguageManager::GetString(ELanguageParameter_Reset), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_FLAT, rrBtnAlarmReset, this, EDeviceAddControlID_BtnAlarmReset);
		pBtnAlarmReset->SetFont(pBaseFont);
		m_vctCtrl.push_back(pBtnAlarmReset);

		rrResultGroupBox = rrGrpAlarmReset;
	}
	while(false);

	UnlockWindowUpdate();

	return bReturn;
}

bool CDeviceEziServo2PlusE::CreateLedArray(double dblDPIScale, CFont* pBaseFont, CRavidPoint<int> rpLeftTopPt, CRavidPoint<int> rpRightTopPt, CRavidRect<int>& rrResultGroupBox)
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

		int nInputMaxLed = GetInputPortCount();
		int nOutputMaxLed = GetOutputPortCount();

		int nInputLedCreatedCount = 0;
		int nOutputLedCreatedCount = 0;

		int nRealPos = 0;

		for(int i = 0; i < nInputMaxLed; ++i)
		{
			if(!m_pBUsingInputArray[i])
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
			strName.Format(_T("%d : %s"), i, m_pStrInputNameArray[i]);
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
			if(!m_pBUsingOutputArray [i])
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
			strName.Format(_T("%d : %s"), i, m_pStrOutputNameArray[i]);
			pLabel->Create(strName, WS_VISIBLE | WS_CHILD | SS_NOTIFY, rrLabelPosition, this, EDeviceAddEnumeratedControlID_OutputLedLabelStart + i);
			pLabel->SetFont(pBaseFont);
			m_vctCtrl.push_back(pLabel);

			++nRealPos;
		}

		nOutputLedCreatedCount = nRealPos;

		rrGroupBox[0].bottom = (rrLedRect + rpLedYGap * (__max(nInputLedCreatedCount, nOutputLedCreatedCount) - 1)).bottom + 10 * dblDPIScale;
		rrGroupBox[1].bottom = (rrLedRect + rpLedYGap * (__max(nInputLedCreatedCount, nOutputLedCreatedCount) - 1)).bottom + 10 * dblDPIScale;

		for(int i = 0; i < 2; ++i)
		{
			pGroupBox[i]->Create(i ? CMultiLanguageManager::GetString(ELanguageParameter_Output) : CMultiLanguageManager::GetString(ELanguageParameter_Input), BS_GROUPBOX | WS_CHILD | WS_VISIBLE, rrGroupBox[i], this, EDeviceAddEnumeratedControlID_StaticStart + i);
			pGroupBox[i]->SetFont(pBaseFont);
			m_vctCtrl.push_back(pGroupBox[i]);
		}

		rrResultGroupBox = rrGroupBox[1];

		bReturn = true;
	}
	while(false);

	UnlockWindowUpdate();

	return bReturn;
}

void CDeviceEziServo2PlusE::AllocateIO(long nInputSize, long nOutputSize)
{
	if(!m_pBUsingInputArray)
	{
		m_pBUsingInputArray = new bool[nInputSize];
		memset(m_pBUsingInputArray, 0, nInputSize);
	}

	if(!m_pBUsingOutputArray )
	{
		m_pBUsingOutputArray  = new bool[nOutputSize];
		memset(m_pBUsingOutputArray , 0, nOutputSize);
	}

	if(!m_pStrInputNameArray)
		m_pStrInputNameArray = new CString[nInputSize];

	if(!m_pStrOutputNameArray)
		m_pStrOutputNameArray = new CString[nOutputSize];
}
void CDeviceEziServo2PlusE::FreeIO()
{
	if(m_pBUsingInputArray)
	{
		delete[] m_pBUsingInputArray;
		m_pBUsingInputArray = nullptr;
	}

	if(m_pBUsingOutputArray )
	{
		delete[] m_pBUsingOutputArray ;
		m_pBUsingOutputArray  = nullptr;
	}

	if(m_pStrInputNameArray)
	{
		delete[] m_pStrInputNameArray;
		m_pStrInputNameArray = nullptr;
	}

	if(m_pStrOutputNameArray)
	{
		delete[] m_pStrOutputNameArray;
		m_pStrOutputNameArray = nullptr;
	}
}

void CDeviceEziServo2PlusE::OnBnClickedOutLed(UINT nID)
{
	do
	{
		int nNumber = nID - EDeviceAddEnumeratedControlID_OutputLedStart;

		if((CAuthorityManager::GetDeviceControlPermission() <= CAuthorityManager::GetCurrentUserType()) && !CSequenceManager::IsRunning())
		{
			bool bBit = ReadGenOutBit(nNumber);

			WriteGenOutBit(nNumber, !bBit);
		}
	}
	while(false);
}

BOOL CDeviceEziServo2PlusE::OnInitDialog()
{
	CDeviceMotion::OnInitDialog();

	SetTimer(RAVID_TIMER_DIO, 10, nullptr);

	return TRUE;
}

void CDeviceEziServo2PlusE::OnDestroy()
{
	CDeviceMotion::OnDestroy();

	KillTimer(RAVID_TIMER_DIO);
}


void CDeviceEziServo2PlusE::OnTimer(UINT_PTR nIDEvent)
{
	if(IsWindowVisible())
	{
		if(nIDEvent == RAVID_TIMER_DIO)
		{
			for(int i = 0; i < 32; ++i)
			{
				if(m_pBUsingInputArray[i])
				{
					CRavidLedCtrl* pLed = (CRavidLedCtrl*)GetDlgItem(EDeviceAddEnumeratedControlID_InputLedStart + i);

					if(pLed)
					{
						pLed->SetActivate(IsInitialized());

						if(ReadGenInputBit(i))
							pLed->On();
						else
							pLed->Off();
					}
				}

				if(m_pBUsingOutputArray[i])
				{
					CRavidLedCtrl* pLed = (CRavidLedCtrl*)GetDlgItem(EDeviceAddEnumeratedControlID_OutputLedStart + i);

					if(pLed)
					{
						pLed->SetActivate(IsInitialized());

						if(ReadGenOutBit(i))
							pLed->On();
						else
							pLed->Off();
					}
				}
			}
		}
	}

	CDeviceMotion::OnTimer(nIDEvent);
}

#endif