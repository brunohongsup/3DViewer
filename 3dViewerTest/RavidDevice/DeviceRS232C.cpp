#include "stdafx.h"

#include "DeviceRS232C.h"

#include "../RavidCore/PerformanceCounter.h"

#include "../RavidFramework/LogManager.h"
#include "../RavidFramework/DeviceManager.h"
#include "../RavidFramework/SequenceManager.h"
#include "../RavidFramework/FrameworkManager.h"
#include "../RavidFramework/AuthorityManager.h"
#include "../RavidFramework/EventHandlerManager.h"
#include "../RavidFramework/MultiLanguageManager.h"

#include "../RavidFramework/Packet.h"
#include "../RavidFramework/MessageBase.h"
#include "../RavidFramework/RavidLedCtrl.h"

#include <bitset>
#include <Mstcpip.h>

using namespace Ravid;
using namespace Ravid::Device;
using namespace Ravid::Database;
using namespace Ravid::Framework;
using namespace Ravid::Algorithms;
using namespace Ravid::Miscellaneous;

IMPLEMENT_DYNAMIC(CDeviceRS232C, CDeviceBase)

BEGIN_MESSAGE_MAP(CDeviceRS232C, CDeviceBase)
	ON_COMMAND(EDeviceAddControlID_BtnSerialSend, OnBnClickedSend)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static LPCTSTR g_lpszDeviceRS232C[EDeviceParameterRS232C_Count] =
{
	_T("DeviceID"),
	_T("Transmission Settings"),
	_T("Baud Rate"),
	_T("Data Bits"),
	_T("Parity"),
	_T("Stop Bits"),
	_T("Tx Buffer Size"),
	_T("Rx Buffer Size"),
	_T("Display"),
	_T("Sending Format"),
	_T("Receiving View Format"),
};

static LPCTSTR g_lpszBaudRate[EDeviceRS232CBaudRate_Count] =
{
	_T("110"),
	_T("300"),
	_T("1200"),
	_T("2400"),
	_T("4800"),
	_T("9600"),
	_T("19200"),
	_T("38400"),
	_T("57600"),
	_T("115200"),
	_T("230400"),
	_T("460800"),
	_T("921600")
};

static LPCTSTR g_lpszDataBit[EDeviceRS232CDataBit_Count] =
{
	_T("5"),
	_T("6"),
	_T("7"),
	_T("8")
};

static LPCTSTR g_lpszParity[EDeviceRS232CParity_Count] =
{
	_T("None"),
	_T("Odd"),
	_T("Even"),
	_T("Mark"),
	_T("Space")
};

static LPCTSTR g_lpszStopBits[EDeviceRS232CStopBits_Count] =
{
	_T("1"),
	_T("1.5"),
	_T("2")
};

static LPCTSTR g_lpszFormat[EDeviceRS232CDataFormat_Count] =
{
	_T("Ascii"),
	_T("Binary"),
	_T("Oct"),
	_T("Hex")
};

CDeviceRS232C::CDeviceRS232C()
{
	ZeroMemory(&m_dcbParam, sizeof(m_dcbParam));
	ZeroMemory(&m_osRead, sizeof(m_osRead));
	ZeroMemory(&m_osWrite, sizeof(m_osWrite));
}


CDeviceRS232C::~CDeviceRS232C()
{
}

ptrdiff_t CDeviceRS232C::OnMessage(CMessageBase * pMessage)
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

		pCtrlUI = GetDlgItem(EDeviceAddControlID_StaticSendingStatus);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_waiting));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_StatusLED));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 1);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_SendingData) + _T(" :"));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 2);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_ReceivedData) + _T(" :"));

		pCtrlUI = GetDlgItem(EDeviceAddControlID_BtnSerialSend);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_Send));
	}
	while(false);

	return 0;
}

EDeviceInitializeResult CDeviceRS232C::Initialize()
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

		CString strPortName;
		strPortName.Format(_T("\\\\.\\COM%d"), _ttoi(GetParamValue(EDeviceParameterRS232C_DeviceID)));

		m_hPort = CreateFile(strPortName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);

		if(m_hPort == INVALID_HANDLE_VALUE)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtocreateport);
			eReturn = EDeviceInitializeResult_CreatePortError;
			break;
		}

		m_osRead.Offset = 0;
		m_osRead.OffsetHigh = 0;
		m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		m_osWrite.Offset = 0;
		m_osWrite.OffsetHigh = 0;
		m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

		if(!m_osRead.hEvent || !m_osWrite.hEvent)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_FailedtocreateIOevent);
			eReturn = EDeviceInitializeResult_CreateIOPortError;
			break;
		}

		if(!SetupComm(m_hPort, _ttoi(GetParamValue(EDeviceParameterRS232C_RxBufferSize)), _ttoi(GetParamValue(EDeviceParameterRS232C_TxBufferSize))))
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtoinitializeport);
			eReturn = EDeviceInitializeResult_InitializePortError;
			break;
		}

		if(!SetCommMask(m_hPort, EV_RXCHAR | EV_TXEMPTY))
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtosetportevent);
			eReturn = EDeviceInitializeResult_ChangeMaskPortError;
			break;
		}

		if(!PurgeComm(m_hPort, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR))
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtoclearport);
			eReturn = EDeviceInitializeResult_ClearPortError;
			break;
		}

		ZeroMemory(&m_dcbParam, sizeof(m_dcbParam));

		//CString strBaudrate;
		EDeviceRS232CBaudRate eBaudRate = EDeviceRS232CBaudRate_Count;

		if(GetBaudRate(&eBaudRate))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Baud Rate"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		m_dcbParam.DCBlength = sizeof(m_dcbParam);
		m_dcbParam.BaudRate = _ttoi(g_lpszBaudRate[eBaudRate]);

		//CString strParity;
		EDeviceRS232CParity eParity = EDeviceRS232CParity_Count;

		if(GetParity(&eParity))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Parity"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(eParity == 0)
		{
			m_dcbParam.fParity = 0;
			m_dcbParam.Parity = 0;
		}
		else
		{
			m_dcbParam.fParity = 1;

			if(eParity == 1)
				m_dcbParam.Parity = 1;
			else if(eParity == 2)
				m_dcbParam.Parity = 2;
			else if(eParity == 3)
				m_dcbParam.Parity = 3;
			else if(eParity == 4)
				m_dcbParam.Parity = 4;
			else
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Parity"));
				eReturn = EDeviceInitializeResult_NotSupportedDeviceError;
				break;
			}
		}

		//CString strDataBits;
		EDeviceRS232CDataBit eDataBits = EDeviceRS232CDataBit_Count;

		if(GetDataBit(&eDataBits))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Data Bit"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		m_dcbParam.ByteSize = _ttoi(g_lpszDataBit[eDataBits]);

		//CString strStopBits;
		EDeviceRS232CStopBits eStopBits = EDeviceRS232CStopBits_Count;

		if(GetStopBit(&eStopBits))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Stop Bit"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		double dblStopBits = _ttof(g_lpszStopBits[eStopBits]);

		m_dcbParam.StopBits = (dblStopBits == 1. ? 0 : (dblStopBits == 1.5 ? 1 : 2));

		if(!SetCommState(m_hPort, &m_dcbParam))
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtosetcommunicationparameter);
			eReturn = EDeviceInitializeResult_SetPortError;
			break;
		}

		COMMTIMEOUTS commTimeout;
		ZeroMemory(&commTimeout, sizeof(commTimeout));

		if(!GetCommTimeouts(m_hPort, &commTimeout))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Time out"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}


		commTimeout.ReadIntervalTimeout = 0xffffffff;
		commTimeout.WriteTotalTimeoutConstant = 5000;

		if(!SetCommTimeouts(m_hPort, &commTimeout))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Time out"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		CWinThread* pThread = ::AfxBeginThread(CDeviceRS232C::ListeningThread, this, 0, 0U, CREATE_SUSPENDED);

		if(!pThread)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtocreatethread);
			eReturn = EDeviceInitializeResult_CreateThreadError;
			break;
		}

		m_bIsInitialized = true;

		m_hListeningThread = pThread->m_hThread;

		pThread->ResumeThread();

		m_strSend = CMultiLanguageManager::GetString(ELanguageParameter_waiting);
		::PostMessage(this->GetSafeHwnd(), EDeviceRS232CMessage_SendingStatus, (WPARAM)GetDlgItem(EDeviceAddControlID_StaticSendingStatus), (LPARAM)&m_strSend);

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

EDeviceTerminateResult CDeviceRS232C::Terminate()
{
	EDeviceTerminateResult eReturn = EDeviceTerminateResult_UnknownError;

	bool bReturn = false;

	CString strMessage;

	SetStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	do
	{
		m_bIsInitialized = false;

		if(m_hPort == INVALID_HANDLE_VALUE)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("device handle"));
			eReturn = EDeviceTerminateResult_InitializePortError;
			break;
		}

		WaitForSingleObject(m_hListeningThread, 2000);

		CloseHandle(m_hPort);
		SetCommMask(m_hPort, 0);
		EscapeCommFunction(m_hPort, CLRDTR);
		PurgeComm(m_hPort, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);

		if(m_osRead.hEvent)
			CloseHandle(m_osRead.hEvent);

		if(m_osWrite.hEvent)
			CloseHandle(m_osWrite.hEvent);

		m_hPort = 0;
		m_osRead.hEvent = 0;
		m_osWrite.hEvent = 0;

		ZeroMemory(&m_dcbParam, sizeof(m_dcbParam));
		ZeroMemory(&m_osRead, sizeof(m_osRead));
		ZeroMemory(&m_osWrite, sizeof(m_osWrite));

		m_ullReceivedBytes = 0;
		m_ullSentBytes = 0;

		m_hListeningThread = 0;

		bReturn = true;

		if(m_vctCtrl.size())
		{
			if(Ravid::Framework::CFrameworkManager::GetFrameworkRun())
			{
				m_strSend = CMultiLanguageManager::GetString(ELanguageParameter_waiting);
				::PostMessage(this->GetSafeHwnd(), EDeviceRS232CMessage_SendingStatus, (WPARAM)GetDlgItem(EDeviceAddControlID_StaticSendingStatus), (LPARAM)&m_strSend);
			}			
		}

		strMessage = CMultiLanguageManager::GetString(ELanguageParameter_TheDevicehasbeensuccessfullyterminated);

		eReturn = EDeviceTerminateResult_OK;

		CEventHandlerManager::BroadcastOnDeviceTerminated(this);
	}
	while(false);
		
	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceRS232C::LoadSettings()
{
	bool bReturn = false;

	m_vctParameterFieldConfigurations.clear();

	do
	{
		AddParameterFieldConfigurations(EDeviceParameterRS232C_DeviceID, g_lpszDeviceRS232C[EDeviceParameterRS232C_DeviceID], _T("0"), EParameterFieldType_Edit);
		AddParameterFieldConfigurations(EDeviceParameterRS232C_TransmissionSettings, g_lpszDeviceRS232C[EDeviceParameterRS232C_TransmissionSettings], _T("1"), EParameterFieldType_None);
		AddParameterFieldConfigurations(EDeviceParameterRS232C_BaudRate, g_lpszDeviceRS232C[EDeviceParameterRS232C_BaudRate], _T("6"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaudRate, EDeviceRS232CBaudRate_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterRS232C_DataBits, g_lpszDeviceRS232C[EDeviceParameterRS232C_DataBits], _T("3"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDataBit, EDeviceRS232CDataBit_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterRS232C_Parity, g_lpszDeviceRS232C[EDeviceParameterRS232C_Parity], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszParity, EDeviceRS232CParity_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterRS232C_StopBits, g_lpszDeviceRS232C[EDeviceParameterRS232C_StopBits], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszStopBits, EDeviceRS232CStopBits_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterRS232C_TxBufferSize, g_lpszDeviceRS232C[EDeviceParameterRS232C_TxBufferSize], _T("16384"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterRS232C_RxBufferSize, g_lpszDeviceRS232C[EDeviceParameterRS232C_RxBufferSize], _T("16384"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterRS232C_Display, g_lpszDeviceRS232C[EDeviceParameterRS232C_Display], _T("1"), EParameterFieldType_None);
		AddParameterFieldConfigurations(EDeviceParameterRS232C_SendingFormat, g_lpszDeviceRS232C[EDeviceParameterRS232C_SendingFormat], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszFormat, EDeviceRS232CDataFormat_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterRS232C_ReceivingViewFormat, g_lpszDeviceRS232C[EDeviceParameterRS232C_ReceivingViewFormat], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszFormat, EDeviceRS232CDataFormat_Count), nullptr, 1);

		bReturn = true;
	}
	while(false);

	return bReturn &= __super::LoadSettings();
}

ECommSendResult CDeviceRS232C::Send(CPacket& packet)
{
	return Send(&packet);
}

ECommSendResult CDeviceRS232C::Send(CPacket* pPacket)
{
	ECommSendResult eReturn = ECommSendResult_UnknownError;
	
	do
	{
		DWORD dwErrorFlags;
		DWORD dwSentBytes;
		COMSTAT comStat;
		CString strError;
		DWORD dwLastError = 0;

		if(!pPacket)
		{
			eReturn = ECommSendResult_NullptrPacket;
			break;
		}

		size_t stLength = 0;
		char* pData = pPacket->GetData(stLength);

		if(!pData)
		{
			eReturn = ECommSendResult_NullptrData;
			break;
		}

		if(!stLength)
		{
			eReturn = ECommSendResult_PacketSizeZero;
			break;
		}

		if(!m_bIsInitialized)
		{
			eReturn = ECommSendResult_NotInitialized;
			break;
		}

		ClearCommError(m_hPort, &dwErrorFlags, &comStat);

		if(!WriteFile(m_hPort, pData, stLength, &dwSentBytes, &m_osWrite))
		{
			dwLastError = GetLastError();

			if(dwLastError == ERROR_IO_PENDING)
			{
				while(!GetOverlappedResult(m_hPort, &m_osWrite, &dwSentBytes, false))
				{
					dwLastError = GetLastError();

					if(dwLastError == ERROR_IO_INCOMPLETE)
					{
						Sleep(1);
						continue;
					}
					else
					{
						ClearCommError(m_hPort, &dwErrorFlags, &comStat);
						CLogManager::Write(EFrameworkLogTypes_Device, CMultiLanguageManager::GetString(ELanguageParameter_RS232CdatasendingerrorErrorcode_0x08x), dwLastError);

						eReturn = ECommSendResult_SendingError;
						break;
					}
				}

				m_pPcSending->Start();

				Lock();
				m_ullSentBytes += stLength;
				Unlock();

				eReturn = ECommSendResult_OK;
			}
			else
			{
				dwSentBytes = 0;

				ClearCommError(m_hPort, &dwErrorFlags, &comStat);
				CLogManager::Write(EFrameworkLogTypes_Device, CMultiLanguageManager::GetString(ELanguageParameter_RS232CdatasendingerrorErrorcode_0x08x), dwLastError);

				eReturn = ECommSendResult_SendingError;
			}
		}
		else
		{
			m_pPcSending->Start();

			Lock();
			m_ullSentBytes += stLength;
			Unlock();

			eReturn = ECommSendResult_OK;
		}
	}
	while(false);

	return eReturn;
}


ERs232GetFunction CDeviceRS232C::GetBufferSizeTx(long * pSize)
{
	ERs232GetFunction eReturn = ERs232GetFunction_UnknownError;

	do
	{
		if(!pSize)
		{
			eReturn = ERs232GetFunction_NullptrOutput;
			break;
		}

		*pSize = _ttoi(GetParamValue(EDeviceParameterRS232C_TxBufferSize));

		eReturn = ERs232GetFunction_OK;
	}
	while(false);

	return eReturn;
}

ERs232SetFunction CDeviceRS232C::SetBufferSizeTx(long nSize)
{
	ERs232SetFunction eReturn = ERs232SetFunction_UnknownError;

	EDeviceParameterRS232C eSaveID = EDeviceParameterRS232C_TxBufferSize;
	
	long nPreValue = _ttoi(GetParamValue(eSaveID));
	
	do
	{
		if(IsInitialized())
		{
			eReturn = ERs232SetFunction_InitializedError;
			break;
		}

		CString strData;
		strData.Format(_T("%ld"), nSize);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = ERs232SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ERs232SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ERs232SetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceRS232C[eSaveID], nPreValue, nSize);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ERs232GetFunction CDeviceRS232C::GetBufferSizeRx(long * pSize)
{
	ERs232GetFunction eReturn = ERs232GetFunction_UnknownError;

	do
	{
		if(!pSize)
		{
			eReturn = ERs232GetFunction_NullptrOutput;
			break;
		}

		*pSize = _ttoi(GetParamValue(EDeviceParameterRS232C_RxBufferSize));

		eReturn = ERs232GetFunction_OK;
	}
	while(false);

	return eReturn;
}

ERs232SetFunction CDeviceRS232C::SetBufferSizeRx(long nSize)
{
	ERs232SetFunction eReturn = ERs232SetFunction_UnknownError;

	EDeviceParameterRS232C eSaveID = EDeviceParameterRS232C_RxBufferSize;

	long nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = ERs232SetFunction_InitializedError;
			break;
		}

		if(0 > nSize || ALLOCATE_MEMORY_SIZE < nSize)
		{
			eReturn = ERs232SetFunction_WriteToDatabaseError;
			break;
		}

		CString strData;
		strData.Format(_T("%ld"), nSize);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = ERs232SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ERs232SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ERs232SetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceRS232C[eSaveID], nPreValue, nSize);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ERs232GetFunction CDeviceRS232C::GetBaudRate(EDeviceRS232CBaudRate * pParam)
{
	ERs232GetFunction eReturn = ERs232GetFunction_UnknownError;

	do 
	{
		if(!pParam)
		{
			eReturn = ERs232GetFunction_NullptrOutput;
			break;
		}

		*pParam = (EDeviceRS232CBaudRate)_ttoi(GetParamValue(EDeviceParameterRS232C_BaudRate));

		eReturn = ERs232GetFunction_OK;
	}
	while(false);

	return eReturn;
}

ERs232SetFunction CDeviceRS232C::SetBaudRate(EDeviceRS232CBaudRate eType)
{
	ERs232SetFunction eReturn = ERs232SetFunction_UnknownError;

	EDeviceParameterRS232C eSaveID = EDeviceParameterRS232C_BaudRate;
	
	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = ERs232SetFunction_InitializedError;
			break;
		}

		if(eType < 0 || eType >= EDeviceRS232CBaudRate_Count)
		{
			eReturn = ERs232SetFunction_NotFindCommand;
			break;
		}

		CString strData;
		strData.Format(_T("%d"),eType);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = ERs232SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ERs232SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ERs232SetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceRS232C[eSaveID], g_lpszBaudRate[nPreValue], g_lpszBaudRate[eType]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ERs232GetFunction CDeviceRS232C::GetDataBit(EDeviceRS232CDataBit * pParam)
{
	ERs232GetFunction eReturn = ERs232GetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ERs232GetFunction_NullptrOutput;
			break;
		}

		*pParam = (EDeviceRS232CDataBit)_ttoi(GetParamValue(EDeviceParameterRS232C_DataBits));

		eReturn = ERs232GetFunction_OK;
	}
	while(false);

	return eReturn;
}

ERs232SetFunction CDeviceRS232C::SetDataBit(EDeviceRS232CDataBit eType)
{
	ERs232SetFunction eReturn = ERs232SetFunction_UnknownError;

	EDeviceParameterRS232C eSaveID = EDeviceParameterRS232C_DataBits;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = ERs232SetFunction_InitializedError;
			break;
		}

		if(eType < 0 || eType >= EDeviceRS232CDataBit_Count)
		{
			eReturn = ERs232SetFunction_NotFindCommand;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), eType);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = ERs232SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ERs232SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ERs232SetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceRS232C[eSaveID], g_lpszDataBit[nPreValue], g_lpszDataBit[eType]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ERs232GetFunction CDeviceRS232C::GetParity(EDeviceRS232CParity * pParam)
{
	ERs232GetFunction eReturn = ERs232GetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ERs232GetFunction_NullptrOutput;
			break;
		}

		*pParam = (EDeviceRS232CParity)_ttoi(GetParamValue(EDeviceParameterRS232C_Parity));

		eReturn = ERs232GetFunction_OK;
	}
	while(false);

	return eReturn;
}

ERs232SetFunction CDeviceRS232C::SetParity(EDeviceRS232CParity eType)
{
	ERs232SetFunction eReturn = ERs232SetFunction_UnknownError;

	EDeviceParameterRS232C eSaveID = EDeviceParameterRS232C_Parity;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = ERs232SetFunction_InitializedError;
			break;
		}

		if(eType < 0 || eType >= EDeviceRS232CParity_Count)
		{
			eReturn = ERs232SetFunction_NotFindCommand;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), eType);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = ERs232SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ERs232SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ERs232SetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceRS232C[eSaveID], g_lpszParity[nPreValue], g_lpszParity[eType]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ERs232GetFunction CDeviceRS232C::GetStopBit(EDeviceRS232CStopBits * pParam)
{
	ERs232GetFunction eReturn = ERs232GetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ERs232GetFunction_NullptrOutput;
			break;
		}

		*pParam = (EDeviceRS232CStopBits)_ttoi(GetParamValue(EDeviceParameterRS232C_StopBits));

		eReturn = ERs232GetFunction_OK;
	}
	while(false);

	return eReturn;
}

ERs232SetFunction CDeviceRS232C::SetStopBit(EDeviceRS232CStopBits eType)
{
	ERs232SetFunction eReturn = ERs232SetFunction_UnknownError;

	EDeviceParameterRS232C eSaveID = EDeviceParameterRS232C_StopBits;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = ERs232SetFunction_InitializedError;
			break;
		}

		if(eType < 0 || eType >= EDeviceRS232CStopBits_Count)
		{
			eReturn = ERs232SetFunction_NotFindCommand;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), eType);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = ERs232SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ERs232SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ERs232SetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceRS232C[eSaveID], g_lpszStopBits[nPreValue], g_lpszStopBits[eType]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ERs232GetFunction CDeviceRS232C::GetReceivingViewFormat(EDeviceRS232CDataFormat * pParam)
{
	ERs232GetFunction eReturn = ERs232GetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ERs232GetFunction_NullptrOutput;
			break;
		}

		*pParam = (EDeviceRS232CDataFormat)_ttoi(GetParamValue(EDeviceParameterRS232C_ReceivingViewFormat));

		eReturn = ERs232GetFunction_OK;
	}
	while(false);

	return eReturn;
}

ERs232SetFunction CDeviceRS232C::SetReceivingViewFormat(EDeviceRS232CDataFormat eType)
{
	ERs232SetFunction eReturn = ERs232SetFunction_UnknownError;

	EDeviceParameterRS232C eSaveID = EDeviceParameterRS232C_ReceivingViewFormat;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eType < 0 || eType >= EDeviceRS232CDataFormat_Count)
		{
			eReturn = ERs232SetFunction_NotFindCommand;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), eType);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = ERs232SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ERs232SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ERs232SetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceRS232C[eSaveID], g_lpszFormat[nPreValue], g_lpszFormat[eType]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

ERs232GetFunction CDeviceRS232C::GetSendingFormat(EDeviceRS232CDataFormat * pParam)
{
	ERs232GetFunction eReturn = ERs232GetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = ERs232GetFunction_NullptrOutput;
			break;
		}

		*pParam = (EDeviceRS232CDataFormat)_ttoi(GetParamValue(EDeviceParameterRS232C_SendingFormat));

		eReturn = ERs232GetFunction_OK;
	}
	while(false);

	return eReturn;
}

ERs232SetFunction CDeviceRS232C::SetSendingFormat(EDeviceRS232CDataFormat eType)
{
	ERs232SetFunction eReturn = ERs232SetFunction_UnknownError;

	EDeviceParameterRS232C eSaveID = EDeviceParameterRS232C_SendingFormat;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eType < 0 || eType >= EDeviceRS232CDataFormat_Count)
		{
			eReturn = ERs232SetFunction_NotFindCommand;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), eType);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = ERs232SetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = ERs232SetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = ERs232SetFunction_OK;
	}
	while(false);

	SetFormatSample();

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceRS232C[eSaveID], g_lpszFormat[nPreValue], g_lpszFormat[eType]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceRS232C::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = false;

	bool bFoundID = true;

	do
	{
		switch(nParam)
		{
		case EDeviceParameterRS232C_DeviceID:
			bReturn = !SetDeviceID(strValue);
			break;
		case EDeviceParameterRS232C_BaudRate:
			bReturn = !SetBaudRate((EDeviceRS232CBaudRate)_ttoi(strValue));
			break;
		case EDeviceParameterRS232C_DataBits:
			bReturn = !SetDataBit((EDeviceRS232CDataBit)_ttoi(strValue));
			break;
		case EDeviceParameterRS232C_Parity:
			bReturn = !SetParity((EDeviceRS232CParity)_ttoi(strValue));
			break;
		case EDeviceParameterRS232C_StopBits:
			bReturn = !SetStopBit((EDeviceRS232CStopBits)_ttoi(strValue));
			break;
		case EDeviceParameterRS232C_TxBufferSize:
			bReturn = !SetBufferSizeTx(_ttoi(strValue));
			break;
		case EDeviceParameterRS232C_RxBufferSize:
			bReturn = !SetBufferSizeRx(_ttoi(strValue));
			break;
		case EDeviceParameterRS232C_SendingFormat:
			bReturn = !SetSendingFormat((EDeviceRS232CDataFormat)_ttoi(strValue));
			break;
		case EDeviceParameterRS232C_ReceivingViewFormat:
			bReturn = !SetReceivingViewFormat((EDeviceRS232CDataFormat)_ttoi(strValue));
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

bool CDeviceRS232C::DoesModuleExist()
{
	return true;
}

bool CDeviceRS232C::AddControls()
{
	bool bReturn = false;
	CDC* pDC = nullptr;
	CFont* pOldFont = nullptr;

	do
	{
		pDC = GetDC();

		if(!pDC)
			break;

		CString strPrevSend;
		CString strPrevReceivced;
		CString strPrevSample;

		if(m_vctCtrl.size())
		{
			GetDlgItemText(EDeviceAddControlID_EditSerialSend, strPrevSend);
			GetDlgItemText(EDeviceAddControlID_EditSerialReceived, strPrevReceivced);
			GetDlgItemText(EDeviceAddControlID_StaticSendingFormat, strPrevSample);
		}

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

		CRavidRect<int> rrCaptionSendData, rrEditSendData, rrBtnSendData;
		CRavidRect<int> rrCaptionReceivced, rrEditReceivced, rrStatusReceivced;

		CRavidRect<int> rrStatusLED;
		CRavidRect<int> rrLedRx, rrLedTx, rrRxCaption, rrTxCaption;
		CRavidRect<int> rrLedRect(0, 0, nLedLength, nLedLength);
		CRavidRect<int> rrSendingFormat, rrSendingStatus;

		rrCaptionSendData.left = rrSheet.left;
		rrCaptionSendData.top = rrSheet.bottom + nDialogEdgeMargin;
		rrCaptionSendData.right = rrCaptionSendData.left + (nButtonWidth * 1.5);
		rrCaptionSendData.bottom = rrCaptionSendData.top + nInfoTextHeight;

		rrEditSendData.left = rrCaptionSendData.right + nButtonInterval;
		rrEditSendData.top = rrCaptionSendData.top;
		rrEditSendData.right = rrEditSendData.left + (nButtonWidth * 4.5);
		rrEditSendData.bottom = rrCaptionSendData.bottom;

		rrSendingStatus.left = rrEditSendData.right + nButtonInterval;
		rrSendingStatus.top = rrCaptionSendData.top;
		rrSendingStatus.right = rrSendingStatus.left + nButtonWidth;
		rrSendingStatus.bottom = rrCaptionSendData.bottom;
		
		rrBtnSendData.left = rrSendingStatus.right + nButtonInterval;
		rrBtnSendData.top = rrCaptionSendData.top;
		rrBtnSendData.right = rrSheet.right;
		rrBtnSendData.bottom = rrCaptionSendData.bottom;

		rrStatusLED.left = rrBtnSendData.left;
		rrStatusLED.top = rrBtnSendData.bottom + nInfomationRows;
		rrStatusLED.right = rrBtnSendData.right;
		rrStatusLED.bottom = rrStatusLED.top + (nGroupBoxVerticalMargin + nInfoTextHeight * nInfomationRows);

		rrSendingFormat.left = rrEditSendData.left;
		rrSendingFormat.top = rrCaptionSendData.bottom + nDialogEdgeMargin;
		rrSendingFormat.right = rrSendingStatus.right;
		rrSendingFormat.bottom = rrSendingFormat.top + nInfoTextHeight;
		
		rrCaptionReceivced.left = rrCaptionSendData.left;
		rrCaptionReceivced.top = rrSendingFormat.bottom + nDialogEdgeMargin;
		rrCaptionReceivced.right = rrCaptionSendData.right;
		rrCaptionReceivced.bottom = rrCaptionReceivced.top + nInfoTextHeight;

		rrEditReceivced.left = rrEditSendData.left;
		rrEditReceivced.top = rrCaptionReceivced.top;
		rrEditReceivced.right = rrSendingStatus.right;
		rrEditReceivced.bottom = rrStatusLED.bottom;

		rrStatusReceivced.left = rrCaptionReceivced.left;
		rrStatusReceivced.top = rrCaptionReceivced.bottom + nDialogEdgeMargin;
		rrStatusReceivced.right = rrCaptionReceivced.right;
		rrStatusReceivced.bottom = rrStatusReceivced.top + nInfoTextHeight;

		int nStatusCenterX = (rrStatusLED.left + rrStatusLED.right) / 2;

		int nStatusCenterY = (rrStatusLED.top + rrStatusLED.bottom) / 2;

		rrRxCaption.left = nStatusCenterX - nInfoTextHeight - nButtonInterval;
		rrRxCaption.top = rrStatusLED.top + nInfoTextHeight;
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

		rrDlg.bottom = rrStatusLED.bottom + nDialogEdgeMargin;

		CStatic* pSttSendingStatus = new CStatic;

		if(!pSttSendingStatus)
			break;

		pSttSendingStatus->Create(CMultiLanguageManager::GetString(ELanguageParameter_waiting), WS_CHILD | WS_VISIBLE | SS_CENTER, rrSendingStatus, this, EDeviceAddControlID_StaticSendingStatus);
		pSttSendingStatus->SetFont(pBaseFont);
		m_vctCtrl.push_back(pSttSendingStatus);

		CStatic* pSttSendingFormat = new CStatic;

		if(!pSttSendingFormat)
			break;

		pSttSendingFormat->Create(_T("ex)"), WS_CHILD | WS_VISIBLE, rrSendingFormat, this, EDeviceAddControlID_StaticSendingFormat);
		pSttSendingFormat->SetFont(pBaseFont);
		pSttSendingFormat->SetWindowTextW(strPrevSample);
		m_vctCtrl.push_back(pSttSendingFormat);

		CButton* pBtnGroupStatus = new CButton;

		if(!pBtnGroupStatus)
			break;

		pBtnGroupStatus->Create(CMultiLanguageManager::GetString(ELanguageParameter_StatusLED), BS_GROUPBOX | WS_CHILD | WS_VISIBLE, rrStatusLED, this, EDeviceAddEnumeratedControlID_StaticStart);
		pBtnGroupStatus->SetFont(pBaseFont);
		m_vctCtrl.push_back(pBtnGroupStatus);

		CStatic* pSttSendCaption = new CStatic;

		if(!pSttSendCaption)
			break;

		pSttSendCaption->Create(CMultiLanguageManager::GetString(ELanguageParameter_SendingData) + _T(" :"), WS_CHILD | WS_VISIBLE, rrCaptionSendData, this, EDeviceAddEnumeratedControlID_StaticStart + 1);
		pSttSendCaption->SetFont(pBaseFont);
		m_vctCtrl.push_back(pSttSendCaption);

		CStatic* pSttReceivedCaption = new CStatic;

		if(!pSttReceivedCaption)
			break;

		pSttReceivedCaption->Create(CMultiLanguageManager::GetString(ELanguageParameter_ReceivedData) + _T(" : "), WS_CHILD | WS_VISIBLE, rrCaptionReceivced, this, EDeviceAddEnumeratedControlID_StaticStart + 2);
		pSttReceivedCaption->SetFont(pBaseFont);
		m_vctCtrl.push_back(pSttReceivedCaption);

		CEdit* pEditSendData = new CEdit;

		if(!pEditSendData)
			break;

		pEditSendData->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL, rrEditSendData, this, EDeviceAddControlID_EditSerialSend);
		pEditSendData->SetFont(pBaseFont);
		pEditSendData->SetWindowTextW(strPrevSend);
		m_vctCtrl.push_back(pEditSendData);

		CButton* pBtnSendData = new CButton;

		if(!pBtnSendData)
			break;

		pBtnSendData->Create(CMultiLanguageManager::GetString(ELanguageParameter_Send), WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_CENTER, rrBtnSendData, this, EDeviceAddControlID_BtnSerialSend);
		pBtnSendData->SetFont(pBaseFont);
		m_vctCtrl.push_back(pBtnSendData);

		CEdit* pEditReceivcedData = new CEdit;

		if(!pEditReceivcedData)
			break;

		//WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL
		//WS_CHILD | WS_VISIBLE | WS_BORDER | WS_DISABLED

		pEditReceivcedData->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY | ES_MULTILINE, rrEditReceivced, this, EDeviceAddControlID_EditSerialReceived);
		pEditReceivcedData->SetFont(pBaseFont);
		pEditReceivcedData->SetWindowTextW(strPrevReceivced);
		m_vctCtrl.push_back(pEditReceivcedData);

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

UINT CDeviceRS232C::ListeningThread(LPVOID pParam)
{
	CDeviceRS232C* pRS232C = (CDeviceRS232C*)pParam;

	if(pRS232C)
	{
		int nMaxBufferSize = _ttoi(pRS232C->GetParamValue(EDeviceParameterRS232C_RxBufferSize));
		char* pBuffer = new char[ALLOCATE_MEMORY_SIZE];

		DWORD dwLength = 0;
		DWORD dwRecvLength = 0;
		DWORD dwLastError = 0;
		DWORD dwErrorFlags = 0;
		COMSTAT comStat;

		if(pBuffer)
		{
			while(pRS232C->m_bIsInitialized)
			{
				ClearCommError(pRS232C->m_hPort, &dwErrorFlags, &comStat);
				dwLength = __min((unsigned int)nMaxBufferSize, comStat.cbInQue);

				if(dwLength > 0)
				{
					if(!ReadFile(pRS232C->m_hPort, pBuffer, dwLength, &dwRecvLength, &pRS232C->m_osRead))
					{
						dwLastError = GetLastError();

						if(dwLastError == ERROR_IO_PENDING)
						{
							while(!GetOverlappedResult(pRS232C->m_hPort, &pRS232C->m_osRead, &dwRecvLength, TRUE))
							{
								dwLastError = GetLastError();

								if(dwLastError == ERROR_IO_INCOMPLETE)
								{
									ClearCommError(pRS232C->m_hPort, &dwErrorFlags, &comStat);

									Sleep(1);

									continue;
								}
								else
								{
									CLogManager::Write(EFrameworkLogTypes_Device, CMultiLanguageManager::GetString(ELanguageParameter_RS232CdatasendingerrorErrorcode_0x08x), dwLastError);

									ClearCommError(pRS232C->m_hPort, &dwErrorFlags, &comStat);

									break;
								}
							}
						}
						else
						{
							dwRecvLength = 0;

							CLogManager::Write(EFrameworkLogTypes_Device, CMultiLanguageManager::GetString(ELanguageParameter_RS232CdatasendingerrorErrorcode_0x08x), dwLastError);

							ClearCommError(pRS232C->m_hPort, &dwErrorFlags, &comStat);
						}
					}

					if(dwRecvLength > 0)
					{
						pRS232C->m_pPcReceiving->Start();

						pBuffer[dwRecvLength] = 0;

						pRS232C->Lock();
						pRS232C->m_ullReceivedBytes += dwRecvLength;
						pRS232C->Unlock();

						CPacket packet;
						packet.SetSocket(0);
						packet.SetData(pBuffer, dwRecvLength);

						//CString strAscii;
						EDeviceRS232CDataFormat eAscii = EDeviceRS232CDataFormat_Count;

						pRS232C->GetReceivingViewFormat(&eAscii);

						pRS232C->m_strRecv.Empty();

						if(eAscii == EDeviceRS232CDataFormat_Binary)
						{
							unsigned int nFlag = 1;

							int nBits = sizeof(BYTE) * 8;

							for(int i = 0; i < dwRecvLength; ++i)
							{
								for(int j = nBits - 1; j >= 0; --j)
								{
									if(pBuffer[i] & (nFlag << j))
										pRS232C->m_strRecv.AppendFormat(_T("%d"), 1);
									else
										pRS232C->m_strRecv.AppendFormat(_T("%d"), 0);
								}

								pRS232C->m_strRecv.AppendFormat(_T(" "));
							}
						}
						else if(eAscii == EDeviceRS232CDataFormat_Oct)
						{
							pRS232C->m_strRecv.Format(_T("%o"), pBuffer[0]);

							for(int i = 1; i < dwRecvLength; ++i)
								pRS232C->m_strRecv.AppendFormat(_T(" %o"), pBuffer[i]);
						}
						else if(eAscii == EDeviceRS232CDataFormat_Hex)
						{
							pRS232C->m_strRecv.Format(_T("%X"), pBuffer[0]);

							for(int i = 1; i < dwRecvLength; ++i)
								pRS232C->m_strRecv.AppendFormat(_T(" %X"), pBuffer[i]);
						}
						else
						{
							pRS232C->m_strRecv = CString(CStringA(pBuffer), dwRecvLength);
						}

						::PostMessage(pRS232C->GetSafeHwnd(), EDeviceRS232CMessage_RecvData, (WPARAM)pRS232C->GetDlgItem(EDeviceAddControlID_EditSerialReceived), (LPARAM)&(pRS232C->m_strRecv));

						CEventHandlerManager::BroadcastOnReceivingPacket(&packet, pRS232C);
					}
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

	return 0;
}

void CDeviceRS232C::OnTimer(UINT_PTR nIDEvent)
{
	if(IsWindowVisible())
	{
		if(nIDEvent == RAVID_TIMER_COMM)
		{
			bool bEditEnable = false;
			if(CAuthorityManager::GetDeviceControlPermission() <= CAuthorityManager::GetCurrentUserType())
				bEditEnable = true;

			CWnd* pEditSend = GetDlgItem(EDeviceAddControlID_EditSerialSend);

			if(pEditSend)
				pEditSend->EnableWindow(IsInitialized() && bEditEnable && !CSequenceManager::IsRunning());

			CWnd* pBtnSend = GetDlgItem(EDeviceAddControlID_BtnSerialSend);

			if(pBtnSend)
				pBtnSend->EnableWindow(IsInitialized() && bEditEnable && !CSequenceManager::IsRunning());

			CWnd* pBtnRecv = GetDlgItem(EDeviceAddControlID_EditSerialReceived);

			if(pBtnRecv)
				pBtnRecv->EnableWindow(IsInitialized() && bEditEnable && !CSequenceManager::IsRunning());

			DisplayCounter();
		}
	}

	CDeviceBase::OnTimer(nIDEvent);
}


BOOL CDeviceRS232C::OnInitDialog()
{
	CDeviceBase::OnInitDialog();

	SetFormatSample();

	SetTimer(RAVID_TIMER_COMM, 100, nullptr);

	return TRUE;
}

void CDeviceRS232C::OnDestroy()
{
	CDeviceBase::OnDestroy();

	KillTimer(RAVID_TIMER_COMM);
}

bool CDeviceRS232C::DisplayCounter()
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

		if(pLedRx)
		{
			if(IsReceiving())
				pLedRx->On();
			else
				pLedRx->Off();
		}

		if(pLedTx)
		{
			if(IsSending())
				pLedTx->On();
			else
				pLedTx->Off();
		}
	}
	while(false);

	return bReturn;
}

bool CDeviceRS232C::SetFormatSample()
{
	bool bReturn = false;

	do 
	{
		EDeviceRS232CDataFormat eFormat = EDeviceRS232CDataFormat_Count;

		if(GetSendingFormat(&eFormat))
			break;	
	
		if(eFormat == EDeviceRS232CDataFormat_Count)
			break;

		CString strFormat;

		switch(eFormat)
		{
		case EDeviceRS232CDataFormat_Ascii:
				strFormat.Format(_T("ex) RAVID"));
			break;
		case EDeviceRS232CDataFormat_Binary:
				strFormat.Format(_T("ex) 11111111 00011010 10100111"));
			break;
		case EDeviceRS232CDataFormat_Oct:
				strFormat.Format(_T("ex) 077 615 247"));
			break;
		case EDeviceRS232CDataFormat_Hex:
				strFormat.Format(_T("ex) ff 1a a7"));
			break;
		default:
			break;
		}

		SetDlgItemText(EDeviceAddControlID_StaticSendingFormat, strFormat);
		
		bReturn = true;
	} 
	while(false);
	
	return bReturn;
}

bool CDeviceRS232C::ConvertFormat(CString strData, CString* pResult, EDeviceRS232CDataFormat eCurrentFormat, EDeviceRS232CDataFormat eTragetFormat)
{
	bool bReturn = false;

	do 
	{
		if(!pResult)
			break;

		pResult->Empty();

		if(!strData.GetLength())
			break;

		if(eCurrentFormat < 0 || eCurrentFormat >= EDeviceRS232CDataFormat_Count)
			break;

		if(eTragetFormat < 0 || eTragetFormat >= EDeviceRS232CDataFormat_Count)
			break;

		if(eCurrentFormat == eTragetFormat)
		{
			*pResult = strData;

			bReturn = true;

			break;
		}
		
		bool bError = false;
		
		switch(eCurrentFormat)
		{
		case EDeviceRS232CDataFormat_Ascii:
			{
				switch(eTragetFormat)
				{
				case EDeviceRS232CDataFormat_Binary:
					{
						unsigned int nFlag = 1;

						int nBits = sizeof(BYTE) * 8;

						for(int i = 0; i < strData.GetLength(); ++i)
						{
							for(int j = nBits - 1; j >= 0; --j)
							{
								if(strData[i] & (nFlag << j))
									pResult->AppendFormat(_T("%d"), 1);
								else
									pResult->AppendFormat(_T("%d"), 0);
							}

							pResult->AppendFormat(_T(" "));
						}
					}
					break;
				case EDeviceRS232CDataFormat_Oct:
					{
						pResult->Format(_T("%o"), strData[0]);

						for(int i = 1; i < strData.GetLength(); ++i)
							pResult->AppendFormat(_T(" %o"), strData[i]);
					}
					break;
				case EDeviceRS232CDataFormat_Hex:
					{
						pResult->Format(_T("%X"), strData[0]);

						for(int i = 1; i < strData.GetLength(); ++i)
							pResult->AppendFormat(_T(" %X"), strData[i]);
					}
					break;
				default:
					break;
				}
			}
			break;
		case EDeviceRS232CDataFormat_Binary:
			{
				std::vector<CString> vctData;

				if(!StringTokenize(strData, _T(" "), &vctData))
				{
					bError = true;
					break;
				}

				switch(eTragetFormat)
				{
				case EDeviceRS232CDataFormat_Ascii:
					{
						for(int i = 0; i < vctData.size(); ++i)
						{
							unsigned char cData = 0;

							int nLength = vctData[i].GetLength();

							if(nLength > 8 || nLength <= 0)
							{
								bError = true;
								break;
							}

							for(int j = 0; j < nLength; ++j)
							{
								int nData = (vctData[i][j] - 48);

								if(nData < 0 || nData > 1)
								{
									bError = true;
									break;
								}

								cData += (nData << (nLength - j - 1));
							}

							if(bError)
								break;

							pResult->AppendFormat(_T("%c"), cData);
						}
					}
					break;
				case EDeviceRS232CDataFormat_Oct:
					{
						for(int i = 0; i < vctData.size(); ++i)
						{
							unsigned char cData = 0;

							int nLength = vctData[i].GetLength();

							if(nLength > 8 || nLength <= 0)
							{
								bError = true;
								break;
							}

							for(int j = 0; j < nLength; ++j)
							{
								int nData = (vctData[i][j] - 48);

								if(nData < 0 || nData > 1)
								{
									bError = true;
									break;
								}

								cData += (nData << (nLength - j - 1));
							}

							if(bError)
								break;

							pResult->AppendFormat(_T("%o"), cData);
						}
					}
					break;
				case EDeviceRS232CDataFormat_Hex:
					{
						for(int i = 0; i < vctData.size(); ++i)
						{
							unsigned char cData = 0;

							int nLength = vctData[i].GetLength();

							if(nLength > 8 || nLength <= 0)
							{
								bError = true;
								break;
							}

							for(int j = 0; j < nLength; ++j)
							{
								int nData = (vctData[i][j] - 48);

								if(nData < 0 || nData > 1)
								{
									bError = true;
									break;
								}

								cData += (nData << (nLength - j - 1));
							}

							if(bError)
								break;

							pResult->AppendFormat(_T("%X"), cData);
						}
					}
					break;
					default:
						bError = true;
						break;
				}
			}
			break;
		case EDeviceRS232CDataFormat_Oct:
			{
				std::vector<CString> vctData;

				if(!StringTokenize(strData, _T(" "), &vctData))
					break;

				switch(eTragetFormat)
				{
				case EDeviceRS232CDataFormat_Ascii:
					{
						for(int i = 0; i < vctData.size(); ++i)
						{
							unsigned char cData = 0;

							int nLength = vctData[i].GetLength();

							if(nLength > 3 || nLength <= 0)
							{
								bError = true;
								break;
							}

							for(int j = 0; j < nLength; ++j)
							{
								int nData = (vctData[i][j] - 48);
								
								if(nData < 0 || nData > 7)
								{
									bError = true;
									break;
								}
																
								cData += (nData * pow(8, nLength - j - 1));
							}

							if(bError)
								break;

							pResult->AppendFormat(_T("%c"), cData);
						}
					}
					break;
				case EDeviceRS232CDataFormat_Binary:
					{
						for(int i = 0; i < vctData.size(); ++i)
						{
							unsigned char cData = 0;

							int nLength = vctData[i].GetLength();

							if(nLength > 3 || nLength <= 0)
							{
								bError = true;
								break;
							}

							for(int j = 0; j < nLength; ++j)
							{
								int nData = (vctData[i][j] - 48);

								if(nData < 0 || nData > 7)
								{
									bError = true;
									break;
								}

								cData += (nData * pow(8, nLength - j - 1));
							}

							if(bError)
								break;

							unsigned int nFlag = 1;

							int nBits = sizeof(BYTE) * 8;

							for(int j = nBits - 1; j >= 0; --j)
							{
								if(cData & (nFlag << j))
									pResult->AppendFormat(_T("%d"), 1);
								else
									pResult->AppendFormat(_T("%d"), 0);
							}

							pResult->AppendFormat(_T(" "));
						}
					}
					break;
				case EDeviceRS232CDataFormat_Hex:
					{
						for(int i = 0; i < vctData.size(); ++i)
						{
							unsigned char cData = 0;

							int nLength = vctData[i].GetLength();

							if(nLength > 3 || nLength <= 0)
							{
								bError = true;
								break;
							}

							for(int j = 0; j < nLength; ++j)
							{
								int nData = (vctData[i][j] - 48);

								if(nData < 0 || nData > 7)
								{
									bError = true;
									break;
								}

								cData += (nData * pow(8, nLength - j - 1));
							}

							if(bError)
								break;

							pResult->AppendFormat(_T("%X"), cData);
						}
					}
					break;
				default:
					break;
				}
			}
			break;
		case EDeviceRS232CDataFormat_Hex:
			{
				std::vector<CString> vctData;

				if(!StringTokenize(strData, _T(" "), &vctData))
					break;

				switch(eTragetFormat)
				{
				case EDeviceRS232CDataFormat_Ascii:
					{
						for(int i = 0; i < vctData.size(); ++i)
						{
							unsigned char cData = 0;

							int nLength = vctData[i].GetLength();

							if(nLength > 2 || nLength <= 0)
							{
								bError = true;
								break;
							}

							for(int j = 0; j < nLength; ++j)
							{
								int nData = (vctData[i][j] - 48);
								
								if(nData >= 0 && nData <= 9)
									cData += (nData * pow(16, nLength - j - 1));
								else if(nData >= 17 && nData <= 22)
									cData += ((nData - 7) * pow(16, nLength - j - 1));
								else if(nData >= 49 && nData <= 54)
									cData += ((nData - 39) * pow(16, nLength - j - 1));
								else
								{
									bError = true;
									break;
								}
							}

							if(bError)
								break;

							pResult->AppendFormat(_T("%c"), cData);
						}
					}
					break;
				case EDeviceRS232CDataFormat_Binary:
					{
						for(int i = 0; i < vctData.size(); ++i)
						{
							unsigned char cData = 0;

							int nLength = vctData[i].GetLength();

							if(nLength > 2 || nLength <= 0)
							{
								bError = true;
								break;
							}

							for(int j = 0; j < nLength; ++j)
							{
								int nData = (vctData[i][j] - 48);

								if(nData >= 0 && nData <= 9)
									cData += (nData * pow(16, nLength - j - 1));
								else if(nData >= 17 && nData <= 22)
									cData += ((nData - 7) * pow(16, nLength - j - 1));
								else if(nData >= 49 && nData <= 54)
									cData += ((nData - 39) * pow(16, nLength - j - 1));
								else
								{
									bError = true;
									break;
								}
							}

							if(bError)
								break;

							unsigned int nFlag = 1;

							int nBits = sizeof(BYTE) * 8;

							for(int j = nBits - 1; j >= 0; --j)
							{
								if(cData & (nFlag << j))
									pResult->AppendFormat(_T("%d"), 1);
								else
									pResult->AppendFormat(_T("%d"), 0);
							}

							pResult->AppendFormat(_T(" "));
						}
					}
					break;
				case EDeviceRS232CDataFormat_Oct:
					{
						for(int i = 0; i < vctData.size(); ++i)
						{
							unsigned char cData = 0;

							int nLength = vctData[i].GetLength();

							if(nLength > 2 || nLength <= 0)
							{
								bError = true;
								break;
							}

							for(int j = 0; j < nLength; ++j)
							{
								int nData = (vctData[i][j] - 48);

								if(nData >= 0 && nData <= 9)
									cData += (nData * pow(16, nLength - j - 1));
								else if(nData >= 17 && nData <= 22)
									cData += ((nData - 7) * pow(16, nLength - j - 1));
								else if(nData >= 49 && nData <= 54)
									cData += ((nData - 39) * pow(16, nLength - j - 1));
								else
								{
									bError = true;
									break;
								}
							}

							if(bError)
								break;

							pResult->AppendFormat(_T("%X"), cData);
						}
					}
					break;
				default:
					break;
				}
			}
			break;
		}

		if(bError)
		{
			pResult->Empty();
			break;
		}

		bReturn = true;
	} 
	while(false);
	
	return bReturn;
}

void CDeviceRS232C::OnBnClickedSend()
{
	if(IsInitialized())
	{
		m_strRecv.Empty();

		::PostMessage(this->GetSafeHwnd(), EDeviceRS232CMessage_RecvData, (WPARAM)GetDlgItem(EDeviceAddControlID_EditSerialReceived), (LPARAM)&m_strRecv);

		CString strData;
		GetDlgItemText(EDeviceAddControlID_EditSerialSend, strData);

		EDeviceRS232CDataFormat eFormat = EDeviceRS232CDataFormat_Count;

		GetSendingFormat(&eFormat);

		if(eFormat == EDeviceRS232CDataFormat_Count)
			eFormat = EDeviceRS232CDataFormat_Ascii;

		CString strFormat;
		
		bool bConvert = ConvertFormat(strData, &strFormat, eFormat, EDeviceRS232CDataFormat_Ascii);

		if(!bConvert)
		{
			m_strSend = CMultiLanguageManager::GetString(ELanguageParameter_failed);
			::PostMessage(this->GetSafeHwnd(), EDeviceRS232CMessage_SendingStatus, (WPARAM)GetDlgItem(EDeviceAddControlID_StaticSendingStatus), (LPARAM)&m_strSend);
			return;
		}
		m_strSend = CMultiLanguageManager::GetString(ELanguageParameter_succeed);
		::PostMessage(this->GetSafeHwnd(), EDeviceRS232CMessage_SendingStatus, (WPARAM)GetDlgItem(EDeviceAddControlID_StaticSendingStatus), (LPARAM)&m_strSend);

		CStringA straData(strFormat);
		size_t sz = straData.GetLength();

		CPacket packet;
		packet.SetData(LPSTR(LPCSTR(straData)), sz);
		Send(&packet);
	}
}

BOOL CDeviceRS232C::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN)
		{
			if(pMsg->hwnd == GetDlgItem(EDeviceAddControlID_EditSerialSend)->GetSafeHwnd())
			{
				OnBnClickedSend();
			}
		}
	}

	return CDeviceBase::PreTranslateMessage(pMsg);
}

LRESULT CDeviceRS232C::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if(EDeviceRS232CMessage_RecvData == message)
	{
		CWnd* pWnd = (CWnd*)wParam;
		if(pWnd)
		{
			CString* pstrText = (CString*)lParam;
			if(pstrText)
			{
				pWnd->SetWindowText(*pstrText);
			}
		}
	}

	if(EDeviceRS232CMessage_SendingStatus == message)
	{
		CWnd* pWnd = (CWnd*)wParam;
		if(pWnd)
		{
			CString* pstrText = (CString*)lParam;
			if(pstrText)
			{
				pWnd->SetWindowText(*pstrText);
			}
		}
	}

	return CDeviceComm::WindowProc(message, wParam, lParam);
}
