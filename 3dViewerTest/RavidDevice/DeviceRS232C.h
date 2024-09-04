#pragma once

#include "../RavidFramework/DeviceComm.h"
#include "../RavidFramework/DefinitionsDevice.h"

#define ALLOCATE_MEMORY_SIZE 16384

namespace Ravid
{
	namespace Miscellaneous
	{
		class CPerformanceCounter;
	}

	namespace Device
	{
		enum EDeviceRS232CMessage
		{
			EDeviceRS232CMessage_RecvData = 0x11111,
			EDeviceRS232CMessage_SendingStatus,

		};


		class CDeviceRS232C : public CDeviceComm
		{
		public:
			RavidUseDynamicCreation();

			RavidPreventCopySelf(CDeviceRS232C);

			DECLARE_DYNAMIC(CDeviceRS232C)

			CDeviceRS232C();
			virtual ~CDeviceRS232C();

			virtual ptrdiff_t OnMessage(_In_ Ravid::Framework::CMessageBase* pMessage);

			virtual EDeviceInitializeResult Initialize();
			virtual EDeviceTerminateResult Terminate();

			virtual bool LoadSettings();

			virtual ECommSendResult Send(_In_ Framework::CPacket& packet);
			virtual ECommSendResult Send(_In_ Framework::CPacket* pPacket);

			virtual ERs232GetFunction GetBaudRate(_Out_ EDeviceRS232CBaudRate* pParam);
			virtual ERs232SetFunction SetBaudRate(_In_ EDeviceRS232CBaudRate eType);

			virtual ERs232GetFunction GetDataBit(_Out_ EDeviceRS232CDataBit* pParam);
			virtual ERs232SetFunction SetDataBit(_In_ EDeviceRS232CDataBit eType);

			virtual ERs232GetFunction GetParity(_Out_ EDeviceRS232CParity* pParam);
			virtual ERs232SetFunction SetParity(_In_ EDeviceRS232CParity eType);

			virtual ERs232GetFunction GetStopBit(_Out_ EDeviceRS232CStopBits* pParam);
			virtual ERs232SetFunction SetStopBit(_In_ EDeviceRS232CStopBits eType);

			virtual ERs232GetFunction GetReceivingViewFormat(_Out_ EDeviceRS232CDataFormat* pParam);
			virtual ERs232SetFunction SetReceivingViewFormat(_In_ EDeviceRS232CDataFormat eType);

			virtual ERs232GetFunction GetSendingFormat(_Out_ EDeviceRS232CDataFormat* pParam);
			virtual ERs232SetFunction SetSendingFormat(_In_ EDeviceRS232CDataFormat eType);

			virtual ERs232GetFunction GetBufferSizeTx(_Out_ long* pSize);
			virtual ERs232SetFunction SetBufferSizeTx(_In_ long nSize);

			virtual ERs232GetFunction GetBufferSizeRx(_Out_ long* pSize);
			virtual ERs232SetFunction SetBufferSizeRx(_In_ long nSize);

			virtual bool ConvertFormat(_In_ CString strData, _Out_ CString* pResult, _In_ EDeviceRS232CDataFormat eCurrentFormat = EDeviceRS232CDataFormat_Ascii, _In_ EDeviceRS232CDataFormat eTargetFormat = EDeviceRS232CDataFormat_Ascii);

			virtual void OnBnClickedSend();
			virtual BOOL OnInitDialog();
			virtual BOOL PreTranslateMessage(MSG* pMsg);

			afx_msg void OnTimer(UINT_PTR nIDEvent);
			afx_msg void OnDestroy();

			virtual bool OnParameterChanged(_In_ int nParam, _In_ CString strValue) override;

			virtual bool DoesModuleExist();
		public:
			DECLARE_MESSAGE_MAP()

		protected:
			virtual bool AddControls();

			virtual bool DisplayCounter();
			
			virtual bool SetFormatSample();
			
			static UINT ListeningThread(_In_ _In_ LPVOID pParam);

		protected:
			HANDLE m_hPort = 0;
			DCB m_dcbParam;

			OVERLAPPED m_osRead;
			OVERLAPPED m_osWrite;

			CString m_strRecv;
			CString m_strSend;

			HANDLE m_hListeningThread = 0;
			virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
		};
	}
}

