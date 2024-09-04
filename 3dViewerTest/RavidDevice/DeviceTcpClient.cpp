#include "stdafx.h"

#include "DeviceTcpClient.h"

#include "../RavidCore/ThreadHelper.h"
#include "../RavidCore/PerformanceCounter.h"

#include "../RavidFramework/LogManager.h"
#include "../RavidFramework/EventHandlerManager.h"
#include "../RavidFramework/MultiLanguageManager.h"

#include "../RavidFramework/Packet.h"
#include "../RavidFramework/MessageBase.h"
#include "../RavidFramework/RavidLedCtrl.h"

#include <WinInet.h>

#include <WS2tcpip.h>

using namespace Ravid;
using namespace Ravid::Device;
using namespace Ravid::Database;
using namespace Ravid::Framework;
using namespace Ravid::Algorithms;
using namespace Ravid::Miscellaneous;

const char* g_pClientPingDatas = "R@v!D&";
size_t g_stClientPingDatasLength = 6;

extern const char* g_pServerPingDatas;
extern size_t g_stServerPingDatasLength;

IMPLEMENT_DYNAMIC(CDeviceTcpClient, CDeviceBase)

BEGIN_MESSAGE_MAP(CDeviceTcpClient, CDeviceBase)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static LPCTSTR g_lpszDeviceTcpClient[EDeviceParameterTcpClient_Count] =
{
	_T("DeviceID"),
	_T("Connection settings"),
	_T("Connecting Server IP Address"),
	_T("Connecting Server Port"),
	_T("Connecting Timeout"),
	_T("Transmission settings"),
	_T("Tx Buffer Size"),
	_T("Rx Buffer Size"),
	_T("Connection validity"),
	_T("Use checking alive"),
	_T("Check alive interval"),
	_T("Check alive timeout"),
};

static LPCTSTR g_lpszFormat[EDeviceSocketDisplayUnit_Count] =
{
	_T("%.00lf %s"),
	_T("%.03lf %s"),
	_T("%.06lf %s"),
	_T("%.07lf %s"),
	_T("%.08lf %s"),
	_T("%.09lf %s")
};

static LPCTSTR g_lpszUnit[EDeviceSocketDisplayUnit_Count] =
{
	_T("Bytes"),
	_T("KBytes"),
	_T("MBytes"),
	_T("GBytes"),
	_T("TBytes"),
	_T("EBytes")
};

static LPCTSTR g_strCheck[2] =
{
	_T("Off"),
	_T("On")
};

CDeviceTcpClient::CDeviceTcpClient()
{
	ZeroMemory(&m_wsaData, sizeof(m_wsaData));
	ZeroMemory(&m_servAddr, sizeof(m_servAddr));
}


CDeviceTcpClient::~CDeviceTcpClient()
{
}

ptrdiff_t CDeviceTcpClient::OnMessage(CMessageBase * pMessage)
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
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_NetworkStatus));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 1);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_StatusLED));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 2);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_TotalReceivedByte) + _T(" :"));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 3);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_TotalSendByte) + _T(" :"));
	}
	while(false);

	return 0;
}

EDeviceInitializeResult CDeviceTcpClient::Initialize()
{
	EDeviceInitializeResult eReturn = EDeviceInitializeResult_UnknownError;

	CString strMessage, strStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	do
	{
		if(IsInitialized())
		{
			strStatus = CMultiLanguageManager::GetString(ELanguageParameter_Initialized);
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Thedevicehasbeenalreadyinitialized);
			eReturn = EDeviceInitializeResult_AlreadyInitializedError;
			break;
		}

		WSADATA wsaData;

		if(::WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntopenthelibrary);
			eReturn = EDeviceInitializeResult_InitializeSocketError;
			break;
		}

		m_hSocket = ::socket(PF_INET, SOCK_STREAM, 0);

		if(m_hSocket == INVALID_SOCKET)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtogeneratethedevicehandle);
			eReturn = EDeviceInitializeResult_InvalidSocketError;
			break;
		}

		CString strIPaddress = GetParamValue(EDeviceParameterTcpClient_ConnectingServerIPAddress);
		CString strIPport = GetParamValue(EDeviceParameterTcpClient_ConnectingServerPort);

		ZeroMemory(&m_servAddr, sizeof(m_servAddr));
		InetPton(AF_INET, CString(strIPaddress), &(m_servAddr.sin_addr.s_addr));
		m_servAddr.sin_family = AF_INET;
		m_servAddr.sin_port = ::htons(_ttoi(strIPport));

		CString strServerInfo;
		strServerInfo.Format(CMultiLanguageManager::GetString(ELanguageParameter_ServerIP_s_Port_s), strIPaddress, strIPport);

		strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Tryingtoconnecttoserver) + strServerInfo;

		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		volatile bool bSuccess = false;
		volatile bool bCompleted = false;

		CreateRavidThreadContextCaptureRef(pContext)
		{
			if(connect(m_hSocket, (SOCKADDR*)&m_servAddr, sizeof(m_servAddr)) != SOCKET_ERROR)
				bSuccess = true;

			bCompleted = true;
		};

		CThreadHelper::Run(pContext);

		CPerformanceCounter pc;

		long nTimeOut = _ttoi(GetParamValue(EDeviceParameterTcpClient_ConnectingTimeout));

		while(!bCompleted)
		{
			if(pc.GetElapsedTime() > nTimeOut)
				break;

			Sleep(1);
		}

		ULONG nMode = 1;
		ioctlsocket(m_hSocket, FIONBIO, &nMode);

		if(!bCompleted)
		{
			closesocket(m_hSocket);
			m_hSocket = 0;

			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtoconnecttoserver) + CMultiLanguageManager::GetString(ELanguageParameter_Timeout);
			eReturn = EDeviceInitializeResult_ConnectSocketError;
			CThreadHelper::Wait(pContext, 100);

			break;
		}
		else if(!bSuccess)
		{
			closesocket(m_hSocket);
			m_hSocket = 0;

			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtoconnecttoserver);
			eReturn = EDeviceInitializeResult_ConnectSocketError;

			break;
		}


		CWinThread* pThread = ::AfxBeginThread(CDeviceTcpClient::ListeningThread, this, 0, 0U, CREATE_SUSPENDED);

		if(!pThread)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtocreatethread);
			eReturn = EDeviceInitializeResult_CreateThreadError;
			break;
		}

		m_bIsInitialized = true;

		m_hListeningThread = pThread->m_hThread;
		pThread->ResumeThread();

		pThread = ::AfxBeginThread(CDeviceTcpClient::PingThread, this, 0, 0U, CREATE_SUSPENDED);

		if(!pThread)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtocreatethread);
			eReturn = EDeviceInitializeResult_CreateThreadError;
			break;
		}

		m_hPingThread = pThread->m_hThread;
		pThread->ResumeThread();

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

EDeviceTerminateResult CDeviceTcpClient::Terminate()
{
	EDeviceTerminateResult eReturn = EDeviceTerminateResult_UnknownError;

	CString strMessage;

	SetStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	do
	{
		m_bIsInitialized = false;

		::shutdown(m_hSocket, SD_BOTH);
		closesocket(m_hSocket);

		WaitForSingleObject(m_hListeningThread, 2000);
		WaitForSingleObject(m_hPingThread, 2000);

		ZeroMemory(&m_wsaData, sizeof(m_wsaData));
		ZeroMemory(&m_servAddr, sizeof(m_servAddr));

		m_ullReceivedBytes = 0;
		m_ullSentBytes = 0;

		strMessage = CMultiLanguageManager::GetString(ELanguageParameter_TheDevicehasbeensuccessfullyterminated);

		eReturn = EDeviceTerminateResult_OK;

		CEventHandlerManager::BroadcastOnDeviceTerminated(this);
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceTcpClient::LoadSettings()
{
	bool bReturn = false;

	m_vctParameterFieldConfigurations.clear();

	do
	{
		AddParameterFieldConfigurations(EDeviceParameterTcpClient_DeviceID, g_lpszDeviceTcpClient[EDeviceParameterTcpClient_DeviceID], _T("0"), EParameterFieldType_Edit);
		AddParameterFieldConfigurations(EDeviceParameterTcpClient_Connectionsettings, g_lpszDeviceTcpClient[EDeviceParameterTcpClient_Connectionsettings], _T("1"), EParameterFieldType_None);
		AddParameterFieldConfigurations(EDeviceParameterTcpClient_ConnectingServerIPAddress, g_lpszDeviceTcpClient[EDeviceParameterTcpClient_ConnectingServerIPAddress], _T("127.0.0.1"), EParameterFieldType_IPAddress, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterTcpClient_ConnectingServerPort, g_lpszDeviceTcpClient[EDeviceParameterTcpClient_ConnectingServerPort], _T("5230"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterTcpClient_ConnectingTimeout, g_lpszDeviceTcpClient[EDeviceParameterTcpClient_ConnectingTimeout], _T("2000"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterTcpClient_Transmissionsettings, g_lpszDeviceTcpClient[EDeviceParameterTcpClient_Transmissionsettings], _T("1"), EParameterFieldType_None);
		AddParameterFieldConfigurations(EDeviceParameterTcpClient_TxBufferSize, g_lpszDeviceTcpClient[EDeviceParameterTcpClient_TxBufferSize], _T("1048576"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterTcpClient_RxBufferSize, g_lpszDeviceTcpClient[EDeviceParameterTcpClient_RxBufferSize], _T("1048576"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterTcpClient_Connectionvalidity, g_lpszDeviceTcpClient[EDeviceParameterTcpClient_Connectionvalidity], _T("1"), EParameterFieldType_None);
		AddParameterFieldConfigurations(EDeviceParameterTcpClient_Usecheckingalive, g_lpszDeviceTcpClient[EDeviceParameterTcpClient_Usecheckingalive], _T("1"), EParameterFieldType_Check, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterTcpClient_Checkaliveinterval, g_lpszDeviceTcpClient[EDeviceParameterTcpClient_Checkaliveinterval], _T("5000"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterTcpClient_Checkalivetimeout, g_lpszDeviceTcpClient[EDeviceParameterTcpClient_Checkalivetimeout], _T("3000"), EParameterFieldType_Edit, nullptr, nullptr, 1);


		bReturn = true;
	}
	while(false);

	return bReturn &= __super::LoadSettings();
}

ECommSendResult CDeviceTcpClient::Send(CPacket& packet)
{
	return Send(&packet);
}

ECommSendResult CDeviceTcpClient::Send(CPacket* pPacket)
{
	ECommSendResult eReturn = ECommSendResult_UnknownError;

	Lock();

	CString strMessage;

	do
	{
		if(!IsInitialized())
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Ithasnotbeeninitializedyet);
			eReturn = ECommSendResult_NotInitialized;
			break;
		}

		if(!pPacket)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntreaddata);
			eReturn = ECommSendResult_NullptrPacket;
			break;
		}

		size_t stLength = 0;
		char* pData = pPacket->GetData(stLength);

		if(!pData)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntreaddata);
			eReturn = ECommSendResult_NullptrData;
			break;
		}

		if(!stLength)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntreaddata);
			eReturn = ECommSendResult_PacketSizeZero;
			break;
		}

		if(::send(m_hSocket, pData, (int)stLength, 0) == SOCKET_ERROR)
		{
			m_ullSentBytes += stLength;
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtosendapackettotheserver);
			eReturn = ECommSendResult_SendingError;
			break;
		}
		else
			m_pPcSending->Start();

		strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Sendapackettotheserverhasbeencompleted);

		eReturn = ECommSendResult_OK;
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	Unlock();

	return eReturn;
}

ETcpGetFunction CDeviceTcpClient::GetConnectingServerIPAddress(CString* pAddress)
{
	ETcpGetFunction eReturn = ETcpGetFunction_UnknownError;

	do
	{
		if(!pAddress)
		{
			eReturn = ETcpGetFunction_NullptrOutput;
			break;
		}

		*pAddress = GetParamValue(EDeviceParameterTcpClient_ConnectingServerIPAddress);

		eReturn = ETcpGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ETcpSetFunction CDeviceTcpClient::SetConnectingServerIPAddress(CString strAddress)
{
	ETcpSetFunction eReturn = ETcpSetFunction_UnknownError;

	EDeviceParameterTcpClient eSaveNum = EDeviceParameterTcpClient_ConnectingServerIPAddress;

	CString strPreValue = GetParamValue(eSaveNum);

	do
	{
		if(IsInitialized())
		{
			eReturn = ETcpSetFunction_InitializedError;
			break;
		}

		if(!SetParamValue(eSaveNum, strAddress))
		{
			eReturn = ETcpSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = ETcpSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ETcpSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceTcpClient[eSaveNum], strPreValue, strAddress);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ETcpGetFunction CDeviceTcpClient::GetConnectingServerPort(int * pPort)
{
	ETcpGetFunction eReturn = ETcpGetFunction_UnknownError;

	do
	{
		if(!pPort)
		{
			eReturn = ETcpGetFunction_NullptrOutput;
			break;
		}

		*pPort = _ttoi(GetParamValue(EDeviceParameterTcpClient_ConnectingServerPort));

		eReturn = ETcpGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ETcpSetFunction CDeviceTcpClient::SetConnectingServerPort(long nPort)
{
	ETcpSetFunction eReturn = ETcpSetFunction_UnknownError;

	EDeviceParameterTcpClient eSaveNum = EDeviceParameterTcpClient_ConnectingServerPort;

	long nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(IsInitialized())
		{
			eReturn = ETcpSetFunction_InitializedError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), nPort);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = ETcpSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = ETcpSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ETcpSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceTcpClient[eSaveNum], nPreValue, nPort);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ETcpGetFunction CDeviceTcpClient::GetConnectingTimeout(int* pTimeout)
{
	ETcpGetFunction eReturn = ETcpGetFunction_UnknownError;

	do
	{
		if(!pTimeout)
		{
			eReturn = ETcpGetFunction_NullptrOutput;
			break;
		}

		*pTimeout = _ttoi(GetParamValue(EDeviceParameterTcpClient_ConnectingTimeout));

		eReturn = ETcpGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ETcpSetFunction CDeviceTcpClient::SetConnectingTimeout(long nTimeout)
{
	ETcpSetFunction eReturn = ETcpSetFunction_UnknownError;

	EDeviceParameterTcpClient eSaveNum = EDeviceParameterTcpClient_ConnectingTimeout;

	long nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(IsInitialized())
		{
			eReturn = ETcpSetFunction_InitializedError;
			break;
		}

		if(nTimeout < 0)
		{
			eReturn = ETcpSetFunction_OutOfRangeError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), nTimeout);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = ETcpSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = ETcpSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ETcpSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceTcpClient[eSaveNum], nPreValue, nTimeout);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ETcpGetFunction CDeviceTcpClient::GetBufferSizeTx(long * pSize)
{
	ETcpGetFunction eReturn = ETcpGetFunction_UnknownError;

	do
	{
		if(!pSize)
		{
			eReturn = ETcpGetFunction_NullptrOutput;
			break;
		}

		*pSize = _ttoi(GetParamValue(EDeviceParameterTcpClient_TxBufferSize));

		eReturn = ETcpGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ETcpSetFunction CDeviceTcpClient::SetBufferSizeTx(long nSize)
{
	ETcpSetFunction eReturn = ETcpSetFunction_UnknownError;

	EDeviceParameterTcpClient eSaveNum = EDeviceParameterTcpClient_TxBufferSize;

	long nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(IsInitialized())
		{
			eReturn = ETcpSetFunction_InitializedError;
			break;
		}

		CString strData;
		strData.Format(_T("%ld"), nSize);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = ETcpSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = ETcpSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ETcpSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceTcpClient[eSaveNum], nPreValue, nSize);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ETcpGetFunction CDeviceTcpClient::GetBufferSizeRx(long * pSize)
{
	ETcpGetFunction eReturn = ETcpGetFunction_UnknownError;

	do
	{
		if(!pSize)
		{
			eReturn = ETcpGetFunction_NullptrOutput;
			break;
		}

		*pSize = _ttoi(GetParamValue(EDeviceParameterTcpClient_RxBufferSize));

		eReturn = ETcpGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ETcpSetFunction CDeviceTcpClient::SetBufferSizeRx(long nSize)
{
	ETcpSetFunction eReturn = ETcpSetFunction_UnknownError;

	EDeviceParameterTcpClient eSaveNum = EDeviceParameterTcpClient_RxBufferSize;

	long nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(IsInitialized())
		{
			eReturn = ETcpSetFunction_InitializedError;
			break;
		}

		CString strData;
		strData.Format(_T("%ld"), nSize);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = ETcpSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = ETcpSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ETcpSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceTcpClient[eSaveNum], nPreValue, nSize);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ETcpGetFunction CDeviceTcpClient::GetUseCheckingAlive(bool* pUse)
{
	ETcpGetFunction eReturn = ETcpGetFunction_UnknownError;

	do
	{
		if(!pUse)
		{
			eReturn = ETcpGetFunction_NullptrOutput;
			break;
		}

		*pUse = _ttoi(GetParamValue(EDeviceParameterTcpClient_Usecheckingalive));

		eReturn = ETcpGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ETcpSetFunction CDeviceTcpClient::SetUseCheckingAlive(bool bUse)
{
	ETcpSetFunction eReturn = ETcpSetFunction_UnknownError;

	EDeviceParameterTcpClient eSaveNum = EDeviceParameterTcpClient_Usecheckingalive;

	bool bParam = _ttoi(GetParamValue(eSaveNum));

	do
	{
		CString strData;
		strData.Format(_T("%d"), bUse);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = ETcpSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = ETcpSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ETcpSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceTcpClient[eSaveNum], g_strCheck[bParam], g_strCheck[bUse]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ETcpGetFunction CDeviceTcpClient::GetCheckAliveInterval(long * pInterval)
{
	ETcpGetFunction eReturn = ETcpGetFunction_UnknownError;

	do
	{
		if(!pInterval)
		{
			eReturn = ETcpGetFunction_NullptrOutput;
			break;
		}

		*pInterval = _ttoi(GetParamValue(EDeviceParameterTcpClient_Checkaliveinterval));

		eReturn = ETcpGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ETcpSetFunction CDeviceTcpClient::SetCheckAliveInterval(long nInterval)
{
	ETcpSetFunction eReturn = ETcpSetFunction_UnknownError;

	EDeviceParameterTcpClient eSaveNum = EDeviceParameterTcpClient_Checkaliveinterval;

	long nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(nInterval < 1000)
		{
			eReturn = ETcpSetFunction_OutOfRangeError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), nInterval);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = ETcpSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = ETcpSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ETcpSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceTcpClient[eSaveNum], nPreValue, nInterval);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ETcpGetFunction CDeviceTcpClient::GetCheckAliveTimeout(long * pTimeout)
{
	ETcpGetFunction eReturn = ETcpGetFunction_UnknownError;

	do
	{
		if(!pTimeout)
		{
			eReturn = ETcpGetFunction_NullptrOutput;
			break;
		}

		*pTimeout = _ttoi(GetParamValue(EDeviceParameterTcpClient_Checkalivetimeout));

		eReturn = ETcpGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ETcpSetFunction CDeviceTcpClient::SetCheckAliveTimeout(long nTimeout)
{
	ETcpSetFunction eReturn = ETcpSetFunction_UnknownError;

	EDeviceParameterTcpClient eSaveNum = EDeviceParameterTcpClient_Checkalivetimeout;

	long nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(nTimeout < 1000)
		{
			eReturn = ETcpSetFunction_OutOfRangeError;
			break;
		}

		CString strData;
		strData.Format(_T("%ld"), nTimeout);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = ETcpSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = ETcpSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ETcpSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceTcpClient[eSaveNum], nPreValue, nTimeout);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}


bool CDeviceTcpClient::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = false;

	bool bFoundID = true;

	do
	{
		switch(nParam)
		{
		case EDeviceParameterTcpClient_DeviceID:
			bReturn = !SetDeviceID(strValue);
			break;
		case EDeviceParameterTcpClient_ConnectingServerIPAddress:
			bReturn = !SetConnectingServerIPAddress(strValue);
			break;
		case EDeviceParameterTcpClient_ConnectingServerPort:
			bReturn = !SetConnectingServerPort(_ttoi(strValue));
			break;
		case EDeviceParameterTcpClient_ConnectingTimeout:
			bReturn = !SetConnectingTimeout(_ttoi(strValue));
			break;
		case EDeviceParameterTcpClient_TxBufferSize:
			bReturn = !SetBufferSizeTx(_ttoi(strValue));
			break;
		case EDeviceParameterTcpClient_RxBufferSize:
			bReturn = !SetBufferSizeRx(_ttoi(strValue));
			break;
		case EDeviceParameterTcpClient_Usecheckingalive:
			bReturn = !SetUseCheckingAlive(_ttoi(strValue));
			break;
		case EDeviceParameterTcpClient_Checkaliveinterval:
			bReturn = !SetCheckAliveInterval(_ttoi(strValue));
			break;
		case EDeviceParameterTcpClient_Checkalivetimeout:
			bReturn = !SetCheckAliveTimeout(_ttoi(strValue));
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

bool CDeviceTcpClient::DoesModuleExist()
{
	return true;
}

bool CDeviceTcpClient::AddControls()
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

		rrSheet.bottom = rrSheet.top + 285 * dblDPIScale;

		pWndParamTree->SetWindowPos(nullptr, 0, 0, rrSheet.GetWidth(), rrSheet.GetHeight(), SWP_NOMOVE);

		int nDialogEdgeMargin = 10 * dblDPIScale;
		int nButtonInterval = 5 * dblDPIScale;
		int nLabelCaption = 100 * dblDPIScale;
		int nInfoTextHeight = 20 * dblDPIScale;
		int nInfomationRows = 2 * dblDPIScale;
		int nGroupBoxVerticalMargin = 25 * dblDPIScale;
		int nLedLength = 15 * dblDPIScale;
		int nLedHeightInterval = 18 * dblDPIScale;

		int nUseWidth = (rrSheet.right - rrSheet.left) - (nButtonInterval * 3);
		int nButtonWidth = (nUseWidth / 8);

		CRavidRect<int> rrNetworkStatus, rrStatusLED;
		CRavidRect<int> rrReceivedCaption, rrSendCaption;
		CRavidRect<int> rrReceivedData, rrSendData;
		CRavidRect<int> rrLedRx, rrLedTx, rrRxCaption, rrTxCaption;
		CRavidRect<int> rrLedRect(0, 0, nLedLength, nLedLength);

		rrNetworkStatus.left = rrSheet.left;
		rrNetworkStatus.top = rrSheet.bottom + nDialogEdgeMargin;
		rrNetworkStatus.right = rrNetworkStatus.left + (nButtonWidth * 7) + nButtonInterval;
		rrNetworkStatus.bottom = rrNetworkStatus.top + (nGroupBoxVerticalMargin + nInfoTextHeight * nInfomationRows);

		int nCaptionHeight = (rrNetworkStatus.bottom - rrNetworkStatus.top) / 3;

		rrReceivedCaption.left = rrNetworkStatus.left + nDialogEdgeMargin * 2;
		rrReceivedCaption.top = rrNetworkStatus.top + nCaptionHeight;
		rrReceivedCaption.right = rrNetworkStatus.right - nLabelCaption;
		rrReceivedCaption.bottom = rrReceivedCaption.top + rrInitBtn.GetHeight();

		rrReceivedData.left = rrReceivedCaption.right;
		rrReceivedData.top = rrReceivedCaption.top;
		rrReceivedData.right = rrNetworkStatus.right - nDialogEdgeMargin;
		rrReceivedData.bottom = rrReceivedCaption.bottom;

		rrSendCaption.left = rrReceivedCaption.left;
		rrSendCaption.top = rrNetworkStatus.top + nCaptionHeight * 2;
		rrSendCaption.right = rrReceivedCaption.right;
		rrSendCaption.bottom = rrSendCaption.top + rrInitBtn.GetHeight() - 1;

		rrSendData.left = rrReceivedData.left;
		rrSendData.top = rrSendCaption.top;
		rrSendData.right = rrReceivedData.right;
		rrSendData.bottom = rrSendCaption.bottom;

		rrStatusLED.left = rrNetworkStatus.right + nButtonInterval;
		rrStatusLED.top = rrNetworkStatus.top;
		rrStatusLED.right = rrSheet.right;
		rrStatusLED.bottom = rrNetworkStatus.bottom;

		int nStatusCenterX = (rrStatusLED.left + rrStatusLED.right) / 2;

		rrRxCaption.left = nStatusCenterX - nInfoTextHeight - nButtonInterval;
		rrRxCaption.top = rrReceivedCaption.top;
		rrRxCaption.right = rrRxCaption.left + nInfoTextHeight;
		rrRxCaption.bottom = rrRxCaption.top + nLedLength;

		rrTxCaption.left = nStatusCenterX + nButtonInterval;
		rrTxCaption.top = rrRxCaption.top;
		rrTxCaption.right = rrTxCaption.left + nInfoTextHeight;
		rrTxCaption.bottom = rrRxCaption.bottom;

		rrLedRx.left = rrRxCaption.left;
		rrLedRx.top = rrRxCaption.bottom;
		rrLedRx.right = rrRxCaption.right;
		rrLedRx.bottom = rrLedRx.top + nInfoTextHeight;

		rrLedTx.left = rrTxCaption.left;
		rrLedTx.top = rrLedRx.top;
		rrLedTx.right = rrTxCaption.right;
		rrLedTx.bottom = rrLedRx.bottom;

		rrDlg.bottom = rrNetworkStatus.bottom + nDialogEdgeMargin;

		CButton* pBtnGroupInfomation = new CButton;

		if(!pBtnGroupInfomation)
			break;

		pBtnGroupInfomation->Create(CMultiLanguageManager::GetString(ELanguageParameter_NetworkStatus), BS_GROUPBOX | WS_CHILD | WS_VISIBLE, rrNetworkStatus, this, EDeviceAddEnumeratedControlID_StaticStart);
		pBtnGroupInfomation->SetFont(pBaseFont);
		m_vctCtrl.push_back(pBtnGroupInfomation);

		CButton* pBtnGroupStatus = new CButton;

		if(!pBtnGroupStatus)
			break;

		pBtnGroupStatus->Create(CMultiLanguageManager::GetString(ELanguageParameter_StatusLED), BS_GROUPBOX | WS_CHILD | WS_VISIBLE, rrStatusLED, this, EDeviceAddEnumeratedControlID_StaticStart + 1);
		pBtnGroupStatus->SetFont(pBaseFont);
		m_vctCtrl.push_back(pBtnGroupStatus);

		CStatic* pSttReceivedByte = new CStatic;

		if(!pSttReceivedByte)
			break;

		pSttReceivedByte->Create(CMultiLanguageManager::GetString(ELanguageParameter_TotalReceivedByte) + _T(" :"), WS_CHILD | WS_VISIBLE, rrReceivedCaption, this, EDeviceAddEnumeratedControlID_StaticStart + 2);
		pSttReceivedByte->SetFont(pBaseFont);
		m_vctCtrl.push_back(pSttReceivedByte);

		CStatic* pSttReceivedValue = new CStatic;

		if(!pSttReceivedValue)
			break;

		pSttReceivedValue->Create(_T("0.0 Byte(s)"), WS_CHILD | WS_VISIBLE | ES_RIGHT, rrReceivedData, this, EDeviceAddControlID_ReceivceData);
		pSttReceivedValue->SetFont(pBaseFont);
		m_vctCtrl.push_back(pSttReceivedValue);


		CStatic* pSttRxCaption = new CStatic;

		if(!pSttRxCaption)
			break;

		pSttRxCaption->Create(_T("Rx"), WS_CHILD | WS_VISIBLE | ES_CENTER, rrRxCaption, this, EDeviceAddControlID_StaticCaption);
		pSttRxCaption->SetFont(pBaseFont);
		m_vctCtrl.push_back(pSttRxCaption);

		CStatic* pSttTxCaption = new CStatic;

		if(!pSttTxCaption)
			break;

		pSttTxCaption->Create(_T("Tx"), WS_CHILD | WS_VISIBLE | ES_CENTER, rrTxCaption, this, EDeviceAddControlID_StaticCaption);
		pSttTxCaption->SetFont(pBaseFont);
		m_vctCtrl.push_back(pSttTxCaption);

		CStatic* pSttSendByte = new CStatic;

		if(!pSttSendByte)
			break;

		pSttSendByte->Create(CMultiLanguageManager::GetString(ELanguageParameter_TotalSendByte) + _T(" :"), WS_CHILD | WS_VISIBLE | WS_TABSTOP, rrSendCaption, this, EDeviceAddEnumeratedControlID_StaticStart + 3);
		pSttSendByte->SetFont(pBaseFont);
		m_vctCtrl.push_back(pSttSendByte);

		CStatic* pSttSendValue = new CStatic;

		if(!pSttSendValue)
			break;

		pSttSendValue->Create(_T("0.0 Byte(s)"), WS_CHILD | WS_VISIBLE | ES_RIGHT, rrSendData, this, EDeviceAddControlID_SendData);
		pSttSendValue->SetFont(pBaseFont);
		m_vctCtrl.push_back(pSttSendValue);

		CRavidLedCtrl* pLedRx = new CRavidLedCtrl;

		if(!pLedRx)
			break;

		pLedRx->Create(nullptr, WS_VISIBLE | WS_CHILD | SS_NOTIFY, rrLedRx, this, EDeviceAddControlID_LedRx);
		pLedRx->SetFont(pBaseFont);
		m_vctCtrl.push_back(pLedRx);

		CRavidLedCtrl* pLedTx = new CRavidLedCtrl;

		if(!pLedTx)
			break;

		pLedTx->Create(nullptr, WS_VISIBLE | WS_CHILD | SS_NOTIFY, rrLedTx, this, EDeviceAddControlID_LedTx);
		pLedTx->SetFont(pBaseFont);
		m_vctCtrl.push_back(pLedTx);

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

UINT CDeviceTcpClient::ListeningThread(LPVOID pParam)
{
	CDeviceTcpClient* pClient = (CDeviceTcpClient*)pParam;

	if(pClient)
	{
		const int nMaxBufferSize = 1024 * 1024;

		char* pBuffer = new char[nMaxBufferSize];

		if(pBuffer)
		{
			float fPingCheckTime = 5000.f;
			float fPingTimeOut = 3000.f;

			CPerformanceCounter pcPingTime;

			CString strMask;
			strMask.Format(_T("[%s,%d] "), pClient->GetClassNameStr(), pClient->GetObjectID());

			CString strMessage;
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_ServerIP_s_Port_s), pClient->GetParamValue(EDeviceParameterTcpClient_ConnectingServerIPAddress), pClient->GetParamValue(EDeviceParameterTcpClient_ConnectingServerPort));

			CLogManager::Write(EFrameworkLogTypes_Device, strMask + CMultiLanguageManager::GetString(ELanguageParameter_Connectedtotheserversuccessfully) + strMessage);

			pClient->m_pPcReceiving->Start();
			pClient->m_pPcSending->Start();

			while(pClient->m_bIsInitialized)
			{
				DWORD dwFlag;
				TCHAR szName[256];

				::InternetGetConnectedStateEx(&dwFlag, szName, 256, 0);

				if(!(dwFlag & 0xf))
				{
					CLogManager::Write(EFrameworkLogTypes_Device, strMask + CMultiLanguageManager::GetString(ELanguageParameter_Serverconnectionislost) + CMultiLanguageManager::GetString(ELanguageParameter_Unplugged) + strMessage);
					break;
				}
				int nResult = ::recv(pClient->m_hSocket, pBuffer, nMaxBufferSize, 0);

				if(nResult > 0)
				{
					pClient->m_pPcReceiving->Start();

					pClient->Lock();
					pClient->m_ullReceivedBytes += nResult;
					pClient->Unlock();

					if(nResult == g_stClientPingDatasLength && !memcmp(pBuffer, g_pClientPingDatas, g_stClientPingDatasLength))
					{
						pClient->Lock();

						pClient->m_hPingCheck = 0;

						pClient->Unlock();
					}
					else if(nResult == g_stServerPingDatasLength && !memcmp(pBuffer, g_pServerPingDatas, g_stServerPingDatasLength))
					{
						pClient->m_pPcSending->Start();
						pClient->m_ullSentBytes += g_stServerPingDatasLength;

						if(::send(pClient->m_hSocket, g_pServerPingDatas, (int)g_stServerPingDatasLength, 0) == SOCKET_ERROR)
							break;
					}
					else
					{
						CPacket packet;
						packet.SetData(pBuffer, nResult);
						packet.SetSocket(pClient->m_hSocket);

						CEventHandlerManager::BroadcastOnReceivingPacket(&packet, pClient);
					}
				}
				else if(!nResult)
				{
					CLogManager::Write(EFrameworkLogTypes_Device, strMask + CMultiLanguageManager::GetString(ELanguageParameter_Serverconnectionislost) + strMessage);
					break;
				}
				else
				{
					int nLastError = WSAGetLastError();

					bool bBreak = true;

					switch(nLastError)
					{
					case WSAEWOULDBLOCK:
						{
							Sleep(1);
							bBreak = false;
						}
						break;
					case WSAENOBUFS:
						{
							CLogManager::Write(EFrameworkLogTypes_Device, strMask + CMultiLanguageManager::GetString(ELanguageParameter_Bufferisntenough) + strMessage);
						}
						break;
					case WSAECONNRESET:
						{
							CLogManager::Write(EFrameworkLogTypes_Device, strMask + CMultiLanguageManager::GetString(ELanguageParameter_Connectionreset) + strMessage);
						}
						break;
					case WSAECONNABORTED:
						{
							CLogManager::Write(EFrameworkLogTypes_Device, strMask + CMultiLanguageManager::GetString(ELanguageParameter_Connectionaborted) + strMessage);
						}
						break;
					case WSAENOTSOCK:
						{
							CLogManager::Write(EFrameworkLogTypes_Device, strMask + CMultiLanguageManager::GetString(ELanguageParameter_Invalidsocket) + strMessage);
						}
						break;
					case WSAENOTCONN:
						{
							CLogManager::Write(EFrameworkLogTypes_Device, strMask + CMultiLanguageManager::GetString(ELanguageParameter_Itsnotconnected) + strMessage);
						}
						break;
					default:
						{
							CString strError;
							strError.Format(CMultiLanguageManager::GetString(ELanguageParameter_Unknwonerroroccurscode_0x08x), nLastError);
							CLogManager::Write(EFrameworkLogTypes_Device, strMask + strError + strMessage);
						}
						break;
					}

					if(bBreak)
						break;
				}

				Sleep(1);
			}
		}

		if(pBuffer)
		{
			delete[] pBuffer;
			pBuffer = nullptr;
		}
	}

	pClient->PostMessage(EDeviceBaseMessage_Terminate);

	return 0;
}

UINT CDeviceTcpClient::PingThread(LPVOID pParam)
{
	CDeviceTcpClient* pClient = (CDeviceTcpClient*)pParam;

	CPerformanceCounter pcPingTimeOut;
	volatile bool bPing = false;

	while(pClient->m_bIsInitialized && pClient->m_hSocket)
	{
		bool bUseAlive = false;

		pClient->GetUseCheckingAlive(&bUseAlive);

		if(bUseAlive)
		{
			long nPingCheckTime = 5000;
			long nPingTimeOut = 1000;

			pClient->GetCheckAliveInterval(&nPingCheckTime);
			pClient->GetCheckAliveTimeout(&nPingTimeOut);

			if(!bPing && (pClient->m_pPcReceiving->GetElapsedTime() > nPingCheckTime && pClient->m_pPcSending->GetElapsedTime() > nPingCheckTime))
			{
				pClient->Lock();

				pClient->m_pPcSending->Start();
				pClient->m_ullSentBytes += g_stClientPingDatasLength;

				if(::send(pClient->m_hSocket, g_pClientPingDatas, (int)g_stClientPingDatasLength, 0) == SOCKET_ERROR)
				{
					shutdown(pClient->m_hSocket, SD_BOTH);
					closesocket(pClient->m_hSocket);
					pClient->m_hSocket = 0;
				}
				else
				{
					pClient->m_hPingCheck = pClient->m_hSocket;
					bPing = true;
					pcPingTimeOut.Start();
				}

				pClient->Unlock();
			}

			if(bPing && pcPingTimeOut.GetElapsedTime() > nPingTimeOut && pClient->m_hSocket)
			{
				pClient->Lock();

				if(pClient->m_hPingCheck)
				{
					shutdown(pClient->m_hPingCheck, SD_BOTH);
					closesocket(pClient->m_hPingCheck);
					pClient->m_hPingCheck = 0;
				}

				pClient->Unlock();

				bPing = false;
			}
		}

		Sleep(100);
	}

	return 0;
}

void CDeviceTcpClient::OnTimer(UINT_PTR nIDEvent)
{
	if(IsWindowVisible())
	{
		if(nIDEvent == RAVID_TIMER_COMM)
			DisplayCounter();
	}

	CDeviceBase::OnTimer(nIDEvent);
}


BOOL CDeviceTcpClient::OnInitDialog()
{
	CDeviceBase::OnInitDialog();

	SetTimer(RAVID_TIMER_COMM, 100, nullptr);

	return TRUE;
}

void CDeviceTcpClient::OnDestroy()
{
	CDeviceBase::OnDestroy();

	KillTimer(RAVID_TIMER_COMM);
}


bool CDeviceTcpClient::DisplayCounter()
{
	bool bReturn = false;

	do
	{
		CRavidLedCtrl* pLedRx = (CRavidLedCtrl*)GetDlgItem(EDeviceAddControlID_LedRx);

		if(pLedRx)
			pLedRx->SetActivate(IsInitialized());

		CRavidLedCtrl* pLedTx = (CRavidLedCtrl*)GetDlgItem(EDeviceAddControlID_LedTx);

		if(pLedTx)
			pLedTx->SetActivate(IsInitialized());

		unsigned long long nRxCount = GetTotalReceivedBytes();
		unsigned long long nTxCount = GetTotalSentBytes();

		CWnd* pWnd = GetDlgItem(EDeviceAddControlID_ReceivceData);

		if(pWnd)
		{
			int nLevel = 0;

			double dblReceivedBytes = nRxCount;

			for(nLevel = 0; dblReceivedBytes > 1024. && nLevel < 5; )
			{
				dblReceivedBytes /= 1024.;
				++nLevel;
			}

			CString strCounter;

			strCounter.Format(g_lpszFormat[nLevel], dblReceivedBytes, g_lpszUnit[nLevel]);

			CString strOrigCounter;

			pWnd->GetWindowText(strOrigCounter);

			if(strCounter.Compare(strOrigCounter))
				pWnd->SetWindowText(strCounter);

			if(pLedRx)
			{
				if(IsReceiving())
					pLedRx->On();
				else
					pLedRx->Off();
			}
		}

		pWnd = GetDlgItem(EDeviceAddControlID_SendData);

		if(pWnd)
		{
			int nLevel = 0;

			double dblSendBytes = nTxCount;

			for(nLevel = 0; dblSendBytes > 1024. && nLevel < 5; )
			{
				dblSendBytes /= 1024.;
				++nLevel;
			}

			CString strCounter;

			strCounter.Format(g_lpszFormat[nLevel], dblSendBytes, g_lpszUnit[nLevel]);

			CString strOrigCounter;

			pWnd->GetWindowText(strOrigCounter);

			if(strCounter.Compare(strOrigCounter))
				pWnd->SetWindowText(strCounter);

			if(pLedTx)
			{
				if(IsSending())
					pLedTx->On();
				else
					pLedTx->Off();
			}
		}
	}
	while(false);

	return bReturn;
}