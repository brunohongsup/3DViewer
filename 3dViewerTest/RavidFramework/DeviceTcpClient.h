#pragma once

#include "DeviceComm.h"

namespace Ravid
{
	namespace Miscellaneous
	{
		class CPerformanceCounter;
	}

	namespace Device
	{
		class AFX_EXT_CLASS CDeviceTcpClient : public CDeviceComm
		{
		public:
			RavidUseDynamicCreation();

			RavidPreventCopySelf(CDeviceTcpClient);

			DECLARE_DYNAMIC(CDeviceTcpClient)

			CDeviceTcpClient();
			virtual ~CDeviceTcpClient();

			virtual ptrdiff_t OnMessage(_In_ Ravid::Framework::CMessageBase* pMessage);

			virtual EDeviceInitializeResult Initialize();
			virtual EDeviceTerminateResult Terminate();

			virtual bool LoadSettings();

			virtual ECommSendResult Send(_In_ Framework::CPacket& packet);
			virtual ECommSendResult Send(_In_ Framework::CPacket* pPacket);

			virtual ETcpGetFunction GetConnectingServerIPAddress(_Out_ CString* pAddress);
			virtual ETcpSetFunction SetConnectingServerIPAddress(_In_ CString strAddress);

			virtual ETcpGetFunction GetConnectingServerPort(_Out_ int* pPort);
			virtual ETcpSetFunction SetConnectingServerPort(_In_ long nPort);

			virtual ETcpGetFunction GetConnectingTimeout(_Out_ int* pTimeout);
			virtual ETcpSetFunction SetConnectingTimeout(_In_ long nTimeout);


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

			static UINT PingThread(_In_ LPVOID pParam);
			static UINT ListeningThread(_In_ LPVOID pParam);

		protected:
			SOCKET m_hSocket = 0;
			WSADATA m_wsaData;
			SOCKADDR_IN m_servAddr;
			HANDLE m_hListeningThread = 0;
			HANDLE m_hPingThread = 0;
			SOCKET m_hPingCheck = 0;
		};
	}
}

