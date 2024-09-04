#pragma once

#include "../RavidFramework/DeviceComm.h"
#include "../RavidFramework/DefinitionsDevice.h"

#include <set>

namespace Ravid
{
	namespace Device
	{
		class CDeviceTcpServer : public CDeviceComm
		{
		public:
			RavidUseDynamicCreation();

			RavidPreventCopySelf(CDeviceTcpServer);

			DECLARE_DYNAMIC(CDeviceTcpServer)

			typedef struct sClientStruct
			{
				SOCKET hSocket;
				HANDLE hThread;
				SOCKADDR_IN sockAddrIn;
				CDeviceTcpServer* pServer;
				CString strIPAddr;
			}
			SClientStruct;

			CDeviceTcpServer();
			virtual ~CDeviceTcpServer();

			virtual ptrdiff_t OnMessage(_In_ Ravid::Framework::CMessageBase* pMessage);

			virtual EDeviceInitializeResult Initialize();
			virtual EDeviceTerminateResult Terminate();

			virtual bool LoadSettings();

			virtual long GetClientCount();

			virtual long GetClientIndex(_In_ SOCKET hSocket);
			virtual long GetClientIndex(_In_ CString strClientIP);

			virtual SOCKET GetClientSocket(_In_ long nIndex);
			virtual SOCKET GetClientSocket(_In_ CString strClientIP);

			virtual CString GetClientIP(_In_ long nIndex);
			virtual CString GetClientIP(_In_ SOCKET hSocket);

			virtual ECommSendResult Send(_In_ Framework::CPacket& packet);
			virtual ECommSendResult Send(_In_ Framework::CPacket* pPacket);

			virtual ETcpGetFunction GetBindingIPAddress(_Out_ CString* pAddress);
			virtual ETcpSetFunction SetBindingIPAddress(_In_ CString strAddress);
			
			virtual ETcpGetFunction GetBindingPort(_Out_ long* pPort);
			virtual ETcpSetFunction SetBindingPort(_In_ long nPort);

			virtual ETcpGetFunction GetBufferSizeTx(_Out_ long* nSize);
			virtual ETcpSetFunction SetBufferSizeTx(_In_ long nSize);

			virtual ETcpGetFunction GetBufferSizeRx(_Out_ long* pSize);
			virtual ETcpSetFunction SetBufferSizeRx(_In_ long nSize);

			virtual ETcpGetFunction GetUseCheckingAlive(_Out_ bool* pUse);
			virtual ETcpSetFunction SetUseCheckingAlive(_In_ bool bUse);

			virtual ETcpGetFunction GetCheckAliveInterval(_Out_ long* pInterval);
			virtual ETcpSetFunction SetCheckAliveInterval(_In_ long nInterval);

			virtual ETcpGetFunction GetCheckAliveTimeout(_Out_ long* pTimeout);
			virtual ETcpSetFunction SetCheckAliveTimeout(_In_ long nTimeout);

			afx_msg void OnTimer(UINT_PTR nIDEvent);
			afx_msg void OnDestroy();
			virtual BOOL OnInitDialog();

			virtual bool OnParameterChanged(_In_ int nParam, _In_ CString strValue) override;

			virtual bool DoesModuleExist();
		public:
			DECLARE_MESSAGE_MAP()

		protected:
			virtual bool AddControls();

			virtual bool DisplayCounter();

		protected:
			static UINT ListeningThread(_In_ LPVOID pParam);
			static UINT ClientThread(_In_ LPVOID pParam);
			static UINT PingThread(_In_ LPVOID pParam);

		protected:
			SOCKET m_hSocket = 0;
			WSADATA m_wsaData;
			SOCKADDR_IN m_servAddr;
			HANDLE m_hListeningThread = 0;
			HANDLE m_hPingThread = 0;
			std::vector<CDeviceTcpServer::SClientStruct> m_vctClientThreads;
			std::set<SOCKET> m_setPingCheck;
		};
	}
}

