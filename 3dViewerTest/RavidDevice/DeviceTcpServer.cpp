#include "stdafx.h"

#include "DeviceTcpServer.h"

#include "../RavidCore/RavidDatabase.h"
#include "../RavidCore/RavidRecordset.h"
#include "../RavidCore/PerformanceCounter.h"

#include "../RavidFramework/LogManager.h"
#include "../RavidFramework/DeviceManager.h"
#include "../RavidFramework/FrameworkManager.h"
#include "../RavidFramework/EventHandlerManager.h"
#include "../RavidFramework/MultiLanguageManager.h"

#include "../RavidFramework/Packet.h"
#include "../RavidFramework/MessageBase.h"
#include "../RavidFramework/RavidLedCtrl.h"

#include <WinInet.h>

#include <WS2tcpip.h>

#include <set>
#include <algorithm>

using namespace Ravid;
using namespace Ravid::Device;
using namespace Ravid::Database;
using namespace Ravid::Framework;
using namespace Ravid::Algorithms;
using namespace Ravid::Miscellaneous;

const char* g_pServerPingDatas = "r@V!d$";
size_t g_stServerPingDatasLength = 6;

extern const char* g_pClientPingDatas;
extern size_t g_stClientPingDatasLength;

IMPLEMENT_DYNAMIC(CDeviceTcpServer, CDeviceBase)

BEGIN_MESSAGE_MAP(CDeviceTcpServer, CDeviceBase)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static LPCTSTR g_lpszDeviceTcpServer[EDeviceParameterTcpServer_Count] =
{
	_T("DeviceID"),
	_T("Connection settings"),
	_T("Binding IP Address"),
	_T("Binding Port"),
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

CDeviceTcpServer::CDeviceTcpServer()
{
	ZeroMemory(&m_wsaData, sizeof(m_wsaData));
	ZeroMemory(&m_servAddr, sizeof(m_servAddr));
}

CDeviceTcpServer::~CDeviceTcpServer()
{
}

ptrdiff_t CDeviceTcpServer::OnMessage(CMessageBase* pMessage)
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
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_ConnectionInfo));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 3);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_TotalReceivedByte) + _T(" :"));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 4);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_TotalSendByte) + _T(" :"));
	}
	while(false);

	return 0;
}

EDeviceInitializeResult CDeviceTcpServer::Initialize()
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

		m_hSocket = ::WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

		if(m_hSocket == INVALID_SOCKET)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtogeneratethedevicehandle);
			eReturn = EDeviceInitializeResult_InvalidSocketError;
			break;
		}

		CString strIPaddress = GetParamValue(EDeviceParameterTcpServer_BindingIPAddress);
		CString strIPport = GetParamValue(EDeviceParameterTcpServer_BindingPort);

		ZeroMemory(&m_servAddr, sizeof(m_servAddr));
		InetPton(AF_INET, CString(strIPaddress), &(m_servAddr.sin_addr.s_addr));


		m_servAddr.sin_family = AF_INET;
		//m_servAddr.sin_addr.s_addr = inet_addr(CStringA(strIPaddress));
		m_servAddr.sin_port = ::htons(_ttoi(strIPport));

		if(::bind(m_hSocket, (SOCKADDR*)&m_servAddr, sizeof(m_servAddr)) != NO_ERROR)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ip-address"));
			eReturn = EDeviceInitializeResult_BindSocketError;
			break;
		}

		ULONG nMode = 1;

		if(::ioctlsocket(m_hSocket, FIONBIO, &nMode) == SOCKET_ERROR)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("socket mode"));
			eReturn = EDeviceInitializeResult_ChangeModeSocketError;
			break;
		}

		if(::listen(m_hSocket, SOMAXCONN) == -1)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("listen socket"));
			eReturn = EDeviceInitializeResult_ListenSocketError;
			break;
		}

		CWinThread* pThread = ::AfxBeginThread(CDeviceTcpServer::ListeningThread, this, 0, 0U, CREATE_SUSPENDED);

		if(!pThread)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtocreatethread);
			eReturn = EDeviceInitializeResult_CreateThreadError;
			break;
		}

		m_bIsInitialized = true;

		m_hListeningThread = pThread->m_hThread;

		pThread->ResumeThread();


		pThread = ::AfxBeginThread(CDeviceTcpServer::PingThread, this, 0, 0U, CREATE_SUSPENDED);

		if(!pThread)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtocreatethread);
			eReturn = EDeviceInitializeResult_CreateThreadError;
			break;
		}

		m_hPingThread = pThread->m_hThread;

		pThread->ResumeThread();

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

EDeviceTerminateResult CDeviceTcpServer::Terminate()
{
	EDeviceTerminateResult eReturn = EDeviceTerminateResult_UnknownError;

	bool bReturn = false;

	CString strMessage;

	SetStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	do
	{
		m_bIsInitialized = false;

		WaitForSingleObject(m_hListeningThread, 2000);
		WaitForSingleObject(m_hPingThread, 2000);

		for(auto& iter : m_vctClientThreads)
		{
			::shutdown(iter.hSocket, SD_BOTH);
			::closesocket(iter.hSocket);
		}

		CPerformanceCounter pc;
		bool bWait = true;

		while(bWait && pc.GetElapsedTime() < 2000.f)
		{
			Lock();
			bWait = !m_vctClientThreads.empty();
			Unlock();
		}

		::closesocket(m_hSocket);
		::WSACleanup();

		m_hListeningThread = 0;
		m_hSocket = 0;

		ZeroMemory(&m_wsaData, sizeof(m_wsaData));
		ZeroMemory(&m_servAddr, sizeof(m_servAddr));

		m_ullReceivedBytes = 0;
		m_ullSentBytes = 0;
		m_hSocket = 0;

		bReturn = true;

		strMessage = CMultiLanguageManager::GetString(ELanguageParameter_TheDevicehasbeensuccessfullyterminated);

		eReturn = EDeviceTerminateResult_OK;

		CEventHandlerManager::BroadcastOnDeviceTerminated(this);
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceTcpServer::LoadSettings()
{
	bool bReturn = false;

	m_vctParameterFieldConfigurations.clear();

	do
	{
		AddParameterFieldConfigurations(EDeviceParameterTcpServer_DeviceID, g_lpszDeviceTcpServer[EDeviceParameterTcpServer_DeviceID], _T("0"), EParameterFieldType_Edit);
		AddParameterFieldConfigurations(EDeviceParameterTcpServer_Connectionsettings, g_lpszDeviceTcpServer[EDeviceParameterTcpServer_Connectionsettings], _T("1"), EParameterFieldType_None);
		AddParameterFieldConfigurations(EDeviceParameterTcpServer_BindingIPAddress, g_lpszDeviceTcpServer[EDeviceParameterTcpServer_BindingIPAddress], _T("127.0.0.1"), EParameterFieldType_IPAddress, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterTcpServer_BindingPort, g_lpszDeviceTcpServer[EDeviceParameterTcpServer_BindingPort], _T("5230"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterTcpServer_Transmissionsettings, g_lpszDeviceTcpServer[EDeviceParameterTcpServer_Transmissionsettings], _T("1"), EParameterFieldType_None);
		AddParameterFieldConfigurations(EDeviceParameterTcpServer_TxBufferSize, g_lpszDeviceTcpServer[EDeviceParameterTcpServer_TxBufferSize], _T("1048576"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterTcpServer_RxBufferSize, g_lpszDeviceTcpServer[EDeviceParameterTcpServer_RxBufferSize], _T("1048576"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterTcpServer_Connectionvalidity, g_lpszDeviceTcpServer[EDeviceParameterTcpServer_Connectionvalidity], _T("1"), EParameterFieldType_None);
		AddParameterFieldConfigurations(EDeviceParameterTcpServer_Usecheckingalive, g_lpszDeviceTcpServer[EDeviceParameterTcpServer_Usecheckingalive], _T("1"), EParameterFieldType_Check, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterTcpServer_Checkaliveinterval, g_lpszDeviceTcpServer[EDeviceParameterTcpServer_Checkaliveinterval], _T("5000"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterTcpServer_Checkalivetimeout, g_lpszDeviceTcpServer[EDeviceParameterTcpServer_Checkalivetimeout], _T("3000"), EParameterFieldType_Edit, nullptr, nullptr, 1);

		bReturn = true;
	}
	while(false);

	return bReturn &= __super::LoadSettings();
}

long CDeviceTcpServer::GetClientCount()
{
	return (long)m_vctClientThreads.size();
}

long CDeviceTcpServer::GetClientIndex(SOCKET hSocket)
{
	long nReturn = -1;

	Lock();

	do
	{
		if(!hSocket)
			break;

		auto iter = std::find_if(m_vctClientThreads.begin(), m_vctClientThreads.end(),
								 [&](SClientStruct& clientStruct) -> bool
		{
			return hSocket == clientStruct.hSocket;
		});

		if(iter == m_vctClientThreads.end())
			break;

		nReturn = std::distance(m_vctClientThreads.begin(), iter);
	}
	while(false);

	Unlock();

	return nReturn;
}

long CDeviceTcpServer::GetClientIndex(CString strClientIP)
{
	long nReturn = -1;

	Lock();

	do
	{
		if(strClientIP.IsEmpty())
			break;

		auto iter = std::find_if(m_vctClientThreads.begin(), m_vctClientThreads.end(),
								 [&](SClientStruct& clientStruct) -> bool
		{
			return !strClientIP.Compare(clientStruct.strIPAddr);
		});

		if(iter == m_vctClientThreads.end())
			break;

		nReturn = std::distance(m_vctClientThreads.begin(), iter);
	}
	while(false);

	Unlock();

	return nReturn;
}

SOCKET CDeviceTcpServer::GetClientSocket(long nIndex)
{
	SOCKET hResult = 0;

	Lock();

	do
	{
		if(nIndex >= m_vctClientThreads.size() || nIndex < 0)
			break;

		hResult = m_vctClientThreads[nIndex].hSocket;
	}
	while(false);

	Unlock();

	return hResult;
}

SOCKET CDeviceTcpServer::GetClientSocket(CString strClientIP)
{
	SOCKET hReturn = 0;

	Lock();

	do
	{
		if(strClientIP.IsEmpty())
			break;

		auto iter = std::find_if(m_vctClientThreads.begin(), m_vctClientThreads.end(),
								 [&](SClientStruct& clientStruct) -> bool
		{
			return !strClientIP.Compare(clientStruct.strIPAddr);
		});

		if(iter == m_vctClientThreads.end())
			break;

		hReturn = iter->hSocket;
	}
	while(false);

	Unlock();

	return hReturn;
}

CString CDeviceTcpServer::GetClientIP(long nIndex)
{
	CString strReturn;

	Lock();

	do
	{
		if(nIndex >= m_vctClientThreads.size() || nIndex < 0)
			break;

		strReturn = m_vctClientThreads[nIndex].strIPAddr;
	}
	while(false);

	Unlock();

	return strReturn;
}

CString CDeviceTcpServer::GetClientIP(SOCKET hSocket)
{
	CString strReturn;

	Lock();

	do
	{
		if(!hSocket)
			break;

		auto iter = std::find_if(m_vctClientThreads.begin(), m_vctClientThreads.end(),
								 [&](SClientStruct& clientStruct) -> bool
		{
			return hSocket == clientStruct.hSocket;
		});

		if(iter == m_vctClientThreads.end())
			break;

		strReturn = iter->strIPAddr;
	}
	while(false);

	Unlock();

	return strReturn;
}

ECommSendResult CDeviceTcpServer::Send(CPacket& packet)
{
	return Send(&packet);
}

ECommSendResult CDeviceTcpServer::Send(CPacket* pPacket)
{
	ECommSendResult eReturn = ECommSendResult_UnknownError;

	bool bReturn = false;

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

		m_pPcSending->Start();


		if(!pPacket->GetSocket())
		{
			m_pPcSending->Start();

			bReturn = !m_vctClientThreads.empty();

			int nSuccessCount = 0;

			for(auto& iter : m_vctClientThreads)
			{
				bool bSuccess = ::send(iter.hSocket, pData, (int)stLength, 0) != SOCKET_ERROR;

				bReturn &= bSuccess;

				if(bSuccess)
				{
					m_pPcSending->Start();
					m_ullSentBytes += stLength;
					++nSuccessCount;
				}
			}

			if(!nSuccessCount)
				strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtosendapackettoallclients);
			else if(!bReturn)
				strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtosendapackettoparticularclients);
			else
				strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Sendapackettoalloftheclientshasbeencompleted);

			eReturn = (bReturn & (bool)nSuccessCount) ? ECommSendResult_OK : ECommSendResult_SendingError;
		}
		else
		{
			bool bFailed = ::send(pPacket->GetSocket(), pData, (int)stLength, 0) == SOCKET_ERROR;

			if(bFailed)
			{
				strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtosendapackettotheclient);
				eReturn = ECommSendResult_SendingError;
				break;
			}
			else
				m_pPcSending->Start();

			m_ullSentBytes += stLength;
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Sendapackettotheclienthasbeencompleted);

			eReturn = ECommSendResult_OK;
		}

	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	Unlock();

	return eReturn;
}

ETcpGetFunction CDeviceTcpServer::GetBindingIPAddress(CString * pAddress)
{
	ETcpGetFunction eReturn = ETcpGetFunction_UnknownError;


	do
	{
		if(!pAddress)
		{
			eReturn = ETcpGetFunction_NullptrOutput;
			break;
		}

		*pAddress = GetParamValue(EDeviceParameterTcpServer_BindingIPAddress);

		eReturn = ETcpGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ETcpSetFunction CDeviceTcpServer::SetBindingIPAddress(CString strAddress)
{
	ETcpSetFunction eReturn = ETcpSetFunction_UnknownError;

	EDeviceParameterTcpServer eSaveNum = EDeviceParameterTcpServer_BindingIPAddress;

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceTcpServer[eSaveNum], strPreValue, strAddress);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ETcpGetFunction CDeviceTcpServer::GetBindingPort(long * pPort)
{
	ETcpGetFunction eReturn = ETcpGetFunction_UnknownError;

	EDeviceParameterTcpServer eSaveNum = EDeviceParameterTcpServer_BindingPort;

	do
	{
		if(!pPort)
		{
			eReturn = ETcpGetFunction_NullptrOutput;
			break;
		}

		*pPort = _ttoi(GetParamValue(eSaveNum));

		eReturn = ETcpGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ETcpSetFunction CDeviceTcpServer::SetBindingPort(long nPort)
{
	ETcpSetFunction eReturn = ETcpSetFunction_UnknownError;

	EDeviceParameterTcpServer eSaveNum = EDeviceParameterTcpServer_BindingPort;

	long nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(IsInitialized())
		{
			eReturn = ETcpSetFunction_InitializedError;
			break;
		}

		CString strData;
		strData.Format(_T("%ld"), nPort);

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceTcpServer[eSaveNum], nPreValue, nPort);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ETcpGetFunction CDeviceTcpServer::GetBufferSizeTx(long * pSize)
{
	ETcpGetFunction eReturn = ETcpGetFunction_UnknownError;

	do
	{
		if(!pSize)
		{
			eReturn = ETcpGetFunction_NullptrOutput;
			break;
		}

		*pSize = _ttoi(GetParamValue(EDeviceParameterTcpServer_TxBufferSize));

		eReturn = ETcpGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ETcpSetFunction CDeviceTcpServer::SetBufferSizeTx(long nSize)
{
	ETcpSetFunction eReturn = ETcpSetFunction_UnknownError;

	EDeviceParameterTcpServer eSaveNum = EDeviceParameterTcpServer_TxBufferSize;

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceTcpServer[eSaveNum], nPreValue, nSize);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ETcpGetFunction CDeviceTcpServer::GetBufferSizeRx(long * pSize)
{
	ETcpGetFunction eReturn = ETcpGetFunction_UnknownError;

	do
	{
		if(!pSize)
		{
			eReturn = ETcpGetFunction_NullptrOutput;
			break;
		}

		*pSize = _ttoi(GetParamValue(EDeviceParameterTcpServer_RxBufferSize));

		eReturn = ETcpGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ETcpSetFunction CDeviceTcpServer::SetBufferSizeRx(long nSize)
{
	ETcpSetFunction eReturn = ETcpSetFunction_UnknownError;

	EDeviceParameterTcpServer eSaveNum = EDeviceParameterTcpServer_RxBufferSize;

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceTcpServer[eSaveNum], nPreValue, nSize);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ETcpGetFunction CDeviceTcpServer::GetUseCheckingAlive(bool* pUse)
{
	ETcpGetFunction eReturn = ETcpGetFunction_UnknownError;

	do
	{
		if(!pUse)
		{
			eReturn = ETcpGetFunction_NullptrOutput;
			break;
		}

		*pUse = _ttoi(GetParamValue(EDeviceParameterTcpServer_Usecheckingalive));

		eReturn = ETcpGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ETcpSetFunction CDeviceTcpServer::SetUseCheckingAlive(bool bUse)
{
	ETcpSetFunction eReturn = ETcpSetFunction_UnknownError;

	EDeviceParameterTcpServer eSaveNum = EDeviceParameterTcpServer_Usecheckingalive;

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceTcpServer[eSaveNum], g_strCheck[bParam], g_strCheck[bUse]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ETcpGetFunction CDeviceTcpServer::GetCheckAliveInterval(long * pInterval)
{
	ETcpGetFunction eReturn = ETcpGetFunction_UnknownError;

	do
	{
		if(!pInterval)
		{
			eReturn = ETcpGetFunction_NullptrOutput;
			break;
		}

		*pInterval = _ttoi(GetParamValue(EDeviceParameterTcpServer_Checkaliveinterval));

		eReturn = ETcpGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ETcpSetFunction CDeviceTcpServer::SetCheckAliveInterval(long nInterval)
{
	ETcpSetFunction eReturn = ETcpSetFunction_UnknownError;

	EDeviceParameterTcpServer eSaveNum = EDeviceParameterTcpServer_Checkaliveinterval;

	long nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(nInterval < 1000)
		{
			eReturn = ETcpSetFunction_OutOfRangeError;
			break;
		}

		CString strData;
		strData.Format(_T("%ld"), nInterval);

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceTcpServer[eSaveNum], nPreValue, nInterval);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ETcpGetFunction CDeviceTcpServer::GetCheckAliveTimeout(long * pTimeout)
{
	ETcpGetFunction eReturn = ETcpGetFunction_UnknownError;

	do
	{
		if(!pTimeout)
		{
			eReturn = ETcpGetFunction_NullptrOutput;
			break;
		}

		*pTimeout = _ttoi(GetParamValue(EDeviceParameterTcpServer_Checkalivetimeout));

		eReturn = ETcpGetFunction_OK;
	}
	while(false);

	return eReturn;
}

ETcpSetFunction CDeviceTcpServer::SetCheckAliveTimeout(long nTimeout)
{
	ETcpSetFunction eReturn = ETcpSetFunction_UnknownError;

	EDeviceParameterTcpServer eSaveNum = EDeviceParameterTcpServer_Checkalivetimeout;

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceTcpServer[eSaveNum], nPreValue, nTimeout);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

UINT CDeviceTcpServer::ListeningThread(LPVOID pParam)
{
	CDeviceTcpServer* pServer = (CDeviceTcpServer*)pParam;

	SOCKET hClientSocket;
	SOCKADDR_IN sockAddrIn;
	int addrLen = sizeof(sockAddrIn);

	CString strMask;
	strMask.Format(_T("[%s,%d] "), pServer->GetClassNameStr(), pServer->GetObjectID());

	CString strBinding;
	strBinding.Format(CMultiLanguageManager::GetString(ELanguageParameter_BindingIP_s_BindingPort_s), pServer->GetParamValue(EDeviceParameterTcpServer_BindingIPAddress), pServer->GetParamValue(EDeviceParameterTcpServer_BindingPort));

	CLogManager::Write(EFrameworkLogTypes_Device, strMask + CMultiLanguageManager::GetString(ELanguageParameter_Theserverisestablishedsuccessfully) + strBinding);

	while(pServer->m_bIsInitialized)
	{
		DWORD dwFlag;
		TCHAR szName[256];

		::InternetGetConnectedStateEx(&dwFlag, szName, 256, 0);

		if(!(dwFlag & 0xf))
		{
			CLogManager::Write(EFrameworkLogTypes_Device, strMask + CMultiLanguageManager::GetString(ELanguageParameter_Serverhasdisconnected) + CMultiLanguageManager::GetString(ELanguageParameter_Unplugged));
			break;
		}
		else
		{
			hClientSocket = ::accept(pServer->m_hSocket, (SOCKADDR*)&sockAddrIn, &addrLen);

			if(hClientSocket == INVALID_SOCKET)
			{
				::Sleep(1);
				continue;
			}

			SClientStruct cs;
			cs.hSocket = hClientSocket;
			cs.pServer = pServer;
			cs.sockAddrIn = sockAddrIn;
			int nIPAddr[4];

			for(int i = 3; i >= 0; --i)
				nIPAddr[i] = (cs.sockAddrIn.sin_addr.s_addr >> (i * 8)) & 0xff;

			cs.strIPAddr.Format(_T("%d.%d.%d.%d"), nIPAddr[0], nIPAddr[1], nIPAddr[2], nIPAddr[3]);

			pServer->Lock();

			pServer->m_vctClientThreads.push_back(cs);
			auto iter = pServer->m_vctClientThreads.rbegin();

			CWinThread* pThread = ::AfxBeginThread(ClientThread, &*iter, 0, 0U, CREATE_SUSPENDED);

			if(pThread)
			{
				iter->hThread = pThread->m_hThread;
				pThread->ResumeThread();
			}
			else
				pServer->m_vctClientThreads.erase((++iter).base());

			pServer->Unlock();

			Sleep(1);
		}
	}

	CLogManager::Write(EFrameworkLogTypes_Device, strMask + CMultiLanguageManager::GetString(ELanguageParameter_Theserveristerminatedsuccessfully) + strBinding);

	pServer->PostMessage(EDeviceBaseMessage_Terminate);

	return 0;
}

UINT CDeviceTcpServer::ClientThread(LPVOID pParam)
{
	SClientStruct* pCs = (SClientStruct*)pParam;

	if(pCs)
	{
		SClientStruct cs = *pCs;

		const int nMaxBufferSize = 1024 * 1024;

		char* pBuffer = new char[nMaxBufferSize];

		if(pBuffer)
		{
			CPerformanceCounter pcPingTime;

			CString strMask;
			strMask.Format(_T("[%s,%d] "), cs.pServer->GetClassNameStr(), cs.pServer->GetObjectID());

			CString strMessage;
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_IP_s_Port_d), cs.strIPAddr, ::ntohs(cs.sockAddrIn.sin_port));

			CLogManager::Write(EFrameworkLogTypes_Device, strMask + CMultiLanguageManager::GetString(ELanguageParameter_Aclienthasconnected) + strMessage);

			cs.pServer->m_pPcReceiving->Start();
			cs.pServer->m_pPcSending->Start();

			while(cs.pServer->m_bIsInitialized)
			{
				cs.pServer->Lock();

				int nResult = ::recv(cs.hSocket, pBuffer, nMaxBufferSize, 0);

				cs.pServer->Unlock();

				if(nResult > 0)
				{
					cs.pServer->m_ullReceivedBytes += nResult;
					cs.pServer->m_pPcReceiving->Start();

					CPacket packet;

					if(nResult == g_stServerPingDatasLength && !memcmp(pBuffer, g_pServerPingDatas, g_stServerPingDatasLength))
					{
						cs.pServer->Lock();

						auto iter = cs.pServer->m_setPingCheck.find(cs.hSocket);

						if(cs.pServer->m_setPingCheck.end() != iter)
							cs.pServer->m_setPingCheck.erase(iter);

						cs.pServer->Unlock();
					}
					else if(nResult == g_stClientPingDatasLength && !memcmp(pBuffer, g_pClientPingDatas, g_stClientPingDatasLength))
					{
						cs.pServer->Lock();

						cs.pServer->m_pPcSending->Start();
						cs.pServer->m_ullSentBytes += g_stClientPingDatasLength;

						if(::send(cs.hSocket, g_pClientPingDatas, (int)g_stClientPingDatasLength, 0) == SOCKET_ERROR)
							break;

						cs.pServer->Unlock();
					}
					else
					{
						packet.SetData(pBuffer, nResult);
						packet.SetSocket(cs.hSocket);

						CEventHandlerManager::BroadcastOnReceivingPacket(&packet, cs.pServer);
					}
				}
				else if(!nResult)
				{
					CLogManager::Write(EFrameworkLogTypes_Device, strMask + CMultiLanguageManager::GetString(ELanguageParameter_Aclienthasdisconnected) + strMessage);
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
							CLogManager::Write(EFrameworkLogTypes_Device, strMask + CMultiLanguageManager::GetString(ELanguageParameter_Aclientbufferisntenough) + strMessage);
						}
						break;
					case WSAECONNRESET:
						{
							CLogManager::Write(EFrameworkLogTypes_Device, strMask + CMultiLanguageManager::GetString(ELanguageParameter_Aclientconnectionreset) + strMessage);
						}
						break;
					case WSAECONNABORTED:
						{
							CLogManager::Write(EFrameworkLogTypes_Device, strMask + CMultiLanguageManager::GetString(ELanguageParameter_Aclientconnectionaborted) + strMessage);
						}
						break;
					case WSAENOTSOCK:
						{
							CLogManager::Write(EFrameworkLogTypes_Device, strMask + CMultiLanguageManager::GetString(ELanguageParameter_Aclienthasinvalidsocket) + strMessage);
						}
						break;
					case WSAENOTCONN:
						{
							CLogManager::Write(EFrameworkLogTypes_Device, strMask + CMultiLanguageManager::GetString(ELanguageParameter_Aclientisntconnected) + strMessage);
						}
						break;
					default:
						{
							CString strCode;
							strCode.Format(CMultiLanguageManager::GetString(ELanguageParameter_Aclientunknwonerroroccurscode_0x08x), nLastError);
							CLogManager::Write(EFrameworkLogTypes_Device, strMask + strCode + strMessage);
						}
						break;
					}

					if(bBreak)
						break;
				}

				Sleep(1);
			}
		}

		cs.pServer->Lock();

		auto iter = std::find_if(cs.pServer->m_vctClientThreads.begin(), cs.pServer->m_vctClientThreads.end(),
								 [&](SClientStruct& contCs) -> bool
		{
			return contCs.hSocket == cs.hSocket;
		});

		if(iter != cs.pServer->m_vctClientThreads.end())
		{
			::shutdown(iter->hSocket, SD_BOTH);
			::closesocket(iter->hSocket);
			cs.pServer->m_vctClientThreads.erase(iter);
		}

		cs.pServer->Unlock();

		if(pBuffer)
		{
			delete[] pBuffer;
			pBuffer = nullptr;
		}
	}

	return 0;
}

UINT CDeviceTcpServer::PingThread(LPVOID pParam)
{
	CDeviceTcpServer* pServer = (CDeviceTcpServer*)pParam;

	CPerformanceCounter pcPingTimeOut;
	volatile bool bPing = false;

	while(pServer->m_bIsInitialized)
	{
		bool bUseAlive = false;

		pServer->GetUseCheckingAlive(&bUseAlive);

		if(bUseAlive)
		{
			long nPingCheckTime = 5000;
			long nPingTimeOut = 1000;

			pServer->GetCheckAliveInterval(&nPingCheckTime);
			pServer->GetCheckAliveTimeout(&nPingTimeOut);

			if(!bPing && (pServer->m_pPcReceiving->GetElapsedTime() > nPingCheckTime && pServer->m_pPcSending->GetElapsedTime() > nPingCheckTime))
			{
				pServer->Lock();

				std::vector<SOCKET> vctCloseList;

				for(auto& iter : pServer->m_vctClientThreads)
				{
					iter.pServer->m_pPcSending->Start();
					iter.pServer->m_ullSentBytes += g_stServerPingDatasLength;

					if(::send(iter.hSocket, g_pServerPingDatas, (int)g_stServerPingDatasLength, 0) == SOCKET_ERROR)
					{
						vctCloseList.push_back(iter.hSocket);
					}
					else
					{
						pServer->m_setPingCheck.insert(iter.hSocket);
					}
				}

				pServer->Unlock();

				for(auto& iter : vctCloseList)
				{
					shutdown(iter, SD_BOTH);
					closesocket(iter);
				}

				bPing = true;
				pcPingTimeOut.Start();
			}

			if(bPing && pcPingTimeOut.GetElapsedTime() > nPingTimeOut)
			{
				pServer->Lock();

				for(auto& iter : pServer->m_setPingCheck)
				{
					shutdown(iter, SD_BOTH);
					closesocket(iter);
				}

				pServer->Unlock();

				bPing = false;
			}
		}

		Sleep(100);
	}

	return 0;
}

bool CDeviceTcpServer::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = false;

	bool bFoundID = true;

	do
	{
		switch(nParam)
		{
		case EDeviceParameterTcpServer_DeviceID:
			bReturn = !SetDeviceID(strValue);
			break;
		case EDeviceParameterTcpServer_BindingIPAddress:
			bReturn = !SetBindingIPAddress(strValue);
			break;
		case EDeviceParameterTcpServer_BindingPort:
			bReturn = !SetBindingPort(_ttoi(strValue));
			break;
		case EDeviceParameterTcpServer_TxBufferSize:
			bReturn = !SetBufferSizeTx(_ttoi(strValue));
			break;
		case EDeviceParameterTcpServer_RxBufferSize:
			bReturn = !SetBufferSizeRx(_ttoi(strValue));
			break;
		case EDeviceParameterTcpServer_Usecheckingalive:
			bReturn = !SetUseCheckingAlive(_ttoi(strValue));
			break;
		case EDeviceParameterTcpServer_Checkaliveinterval:
			bReturn = !SetCheckAliveInterval(_ttoi(strValue));
			break;
		case EDeviceParameterTcpServer_Checkalivetimeout:
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

bool CDeviceTcpServer::DoesModuleExist()
{
	return true;
}

bool CDeviceTcpServer::AddControls()
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

		CRavidRect<int> rrNetworkStatus, rrLEDStatus, rrConnectStatus;
		CRavidRect<int> rrReceivedCaption, rrSendCaption, rrConnectCaption;
		CRavidRect<int> rrReceivedData, rrSendData, rrConnectData;
		CRavidRect<int> rrLedRx, rrLedTx, rrRxCaption, rrTxCaption;
		CRavidRect<int> rrLedRect(0, 0, nLedLength, nLedLength);

		rrNetworkStatus.left = rrSheet.left;
		rrNetworkStatus.top = rrSheet.bottom + nDialogEdgeMargin;
		rrNetworkStatus.right = rrNetworkStatus.left + (nButtonWidth * 5);
		rrNetworkStatus.bottom = rrNetworkStatus.top + (nGroupBoxVerticalMargin + nInfoTextHeight * nInfomationRows);

		rrConnectStatus.left = rrNetworkStatus.right + nButtonInterval;
		rrConnectStatus.top = rrNetworkStatus.top;
		rrConnectStatus.right = rrConnectStatus.left + (nButtonWidth * 2);
		rrConnectStatus.bottom = rrNetworkStatus.bottom;

		rrLEDStatus.left = rrConnectStatus.right + nButtonInterval;
		rrLEDStatus.top = rrNetworkStatus.top;
		rrLEDStatus.right = rrSheet.right;
		rrLEDStatus.bottom = rrNetworkStatus.bottom;

		int nCaptionHeight = (rrNetworkStatus.bottom - rrNetworkStatus.top) / 3;

		rrConnectCaption.left = rrConnectStatus.left + nDialogEdgeMargin;
		rrConnectCaption.top = rrConnectStatus.top + nCaptionHeight;
		rrConnectCaption.right = rrConnectStatus.right - nDialogEdgeMargin;
		rrConnectCaption.bottom = rrConnectStatus.bottom - nInfomationRows;

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

		int nStatusCenterX = (rrLEDStatus.left + rrLEDStatus.right) / 2;

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

		pBtnGroupStatus->Create(CMultiLanguageManager::GetString(ELanguageParameter_StatusLED), BS_GROUPBOX | WS_CHILD | WS_VISIBLE, rrLEDStatus, this, EDeviceAddEnumeratedControlID_StaticStart + 1);
		pBtnGroupStatus->SetFont(pBaseFont);
		m_vctCtrl.push_back(pBtnGroupStatus);

		CButton* pBtnGroupInfo = new CButton;

		if(!pBtnGroupInfo)
			break;

		pBtnGroupInfo->Create(CMultiLanguageManager::GetString(ELanguageParameter_ConnectionInfo), BS_GROUPBOX | WS_CHILD | WS_VISIBLE, rrConnectStatus, this, EDeviceAddEnumeratedControlID_StaticStart + 2);
		pBtnGroupInfo->SetFont(pBaseFont);
		m_vctCtrl.push_back(pBtnGroupInfo);

		CStatic* pSttConnectedCount = new CStatic;

		if(!pSttConnectedCount)
			break;

		pSttConnectedCount->Create(_T("0") + CMultiLanguageManager::GetString(ELanguageParameter_ClientConnected), WS_CHILD | WS_VISIBLE | ES_CENTER, rrConnectCaption, this, EDeviceAddControlID_ConnectedCount);
		pSttConnectedCount->SetFont(pBaseFont);
		m_vctCtrl.push_back(pSttConnectedCount);

		CStatic* pSttReceivedByte = new CStatic;

		if(!pSttReceivedByte)
			break;

		pSttReceivedByte->Create(CMultiLanguageManager::GetString(ELanguageParameter_TotalReceivedByte) + _T(" :"), WS_CHILD | WS_VISIBLE, rrReceivedCaption, this, EDeviceAddEnumeratedControlID_StaticStart + 3);
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

		pSttSendByte->Create(CMultiLanguageManager::GetString(ELanguageParameter_TotalSendByte) + _T(" :"), WS_CHILD | WS_VISIBLE, rrSendCaption, this, EDeviceAddEnumeratedControlID_StaticStart + 4);
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

void CDeviceTcpServer::OnTimer(UINT_PTR nIDEvent)
{
	if(IsWindowVisible())
	{
		if(nIDEvent == RAVID_TIMER_COMM)
			DisplayCounter();
	}

	CDeviceBase::OnTimer(nIDEvent);
}


BOOL CDeviceTcpServer::OnInitDialog()
{
	CDeviceBase::OnInitDialog();

	SetTimer(RAVID_TIMER_COMM, 100, nullptr);

	return TRUE;
}

void CDeviceTcpServer::OnDestroy()
{
	CDeviceBase::OnDestroy();

	KillTimer(RAVID_TIMER_COMM);
}

bool CDeviceTcpServer::DisplayCounter()
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

		pWnd = GetDlgItem(EDeviceAddControlID_ConnectedCount);

		if(pWnd)
		{
			CString strPost;
			pWnd->GetWindowTextW(strPost);

			CString strCount;
			strCount.Format(_T("%d %s"), GetClientCount(), CMultiLanguageManager::GetString(ELanguageParameter_ClientConnected));

			if(strPost.CompareNoCase(strCount))
				pWnd->SetWindowText(strCount);
		}
	}
	while(false);

	return bReturn;
}