#include "stdafx.h"
#include "DeviceSocketMNETG.h"

#ifdef USING_DEVICE

#include "../RavidFramework/UIManager.h"
#include "../RavidFramework/LogManager.h"
#include "../RavidFramework/DeviceManager.h"
#include "../RavidFramework/LibraryManager.h"
#include "../RavidFramework/FrameworkManager.h"
#include "../RavidFramework/EventHandlerManager.h"
#include "../RavidFramework/MultiLanguageManager.h"

#include "../Libraries/Includes/MNETG/Mdfunc.h"

// mmscl32.dll
// mdfunc32.dll
#pragma comment(lib, COMMONLIB_PREFIX "MNETG/MdFunc32.lib")

using namespace Ravid;
using namespace Ravid::Device;
using namespace Ravid::Database;
using namespace Ravid::Framework;
using namespace Ravid::Algorithms;
using namespace Ravid::Miscellaneous;

IMPLEMENT_DYNAMIC(CDeviceSocketMNETG, CDeviceComm)

BEGIN_MESSAGE_MAP(CDeviceSocketMNETG, CDeviceComm)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static LPCTSTR g_lpszParamMNETG[EDeviceParameterMNETG_Count] =
{
	_T("DeviceID"),
	_T("Channel"),
	_T("Station"),
	_T("Network"),
};

CDeviceSocketMNETG::CDeviceSocketMNETG()
{
}
CDeviceSocketMNETG::~CDeviceSocketMNETG()
{
	if(!IsInitialized())
		Terminate();
}

EDeviceInitializeResult CDeviceSocketMNETG::Initialize()
{
	EDeviceInitializeResult eReturn = EDeviceInitializeResult_UnknownError;

	CString strMessage, strStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));
	
	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("MNETG"));

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

		//멤버변수 초기화를 위한 파라미터 함수 호출.
		GetChannel();
		GetStationNo();
		GetNetworkNo();

		bool bSettingIsValid = (m_nChannel != -1) && (m_nNetworkNo != -1) && (m_nStationNo != -1);

		VERIFY(bSettingIsValid);

		if(!bSettingIsValid)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_InvalidParameter);
			eReturn = EDeviceInitializeResult_RangeOfParameterError;
			break;
		}

		long nResult = mdOpen(m_nChannel, -1, &m_nPath);

		if(nResult)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntopenthedevice);

			strStatus.Format(_T(", ErrCode : %d, path : %d"), nResult, m_nPath);
			strMessage.Append(strStatus);

			eReturn = EDeviceInitializeResult_NotOpenedDeviceError;
			break;
		}

		strMessage = CMultiLanguageManager::GetString(ELanguageParameter_TheDevicehasbeensuccessfullyinitialized);

		strStatus = CMultiLanguageManager::GetString(ELanguageParameter_Initialized);

		m_bIsInitialized = true;

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
EDeviceTerminateResult CDeviceSocketMNETG::Terminate()
{
	EDeviceTerminateResult eReturn = EDeviceTerminateResult_UnknownError;

	CString strMessage;

	SetStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("MNETG"));

		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);

		return EDeviceTerminateResult_NotFoundApiError;
	}

	do
	{
		if(IsInitialized())
		{
			mdClose(m_nPath);
			m_nPath = 0;
		}

		strMessage = CMultiLanguageManager::GetString(ELanguageParameter_TheDevicehasbeensuccessfullyterminated);

		m_bIsInitialized = false;

		eReturn = EDeviceTerminateResult_OK;
				
		CEventHandlerManager::BroadcastOnDeviceTerminated(this);
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceSocketMNETG::LoadSettings()
{
	bool bReturn = false;

	m_vctParameterFieldConfigurations.clear();

	do
	{
		AddParameterFieldConfigurations(EDeviceParameterMNETG_DeviceID, g_lpszParamMNETG[EDeviceParameterMNETG_DeviceID], _T("0"), EParameterFieldType_Edit);
		AddParameterFieldConfigurations(EDeviceParameterMNETG_Channel, g_lpszParamMNETG[EDeviceParameterMNETG_Channel], _T("0"), EParameterFieldType_Edit);
		AddParameterFieldConfigurations(EDeviceParameterMNETG_Station, g_lpszParamMNETG[EDeviceParameterMNETG_Station], _T("0"), EParameterFieldType_Edit);
		AddParameterFieldConfigurations(EDeviceParameterMNETG_Network, g_lpszParamMNETG[EDeviceParameterMNETG_Network], _T("0"), EParameterFieldType_Edit);

		bReturn = true;
	}
	while(false);

	return bReturn & __super::LoadSettings();
}

ECommSendResult CDeviceSocketMNETG::Send(Framework::CPacket & packet)
{
	return ECommSendResult_SendingError;
}

ECommSendResult CDeviceSocketMNETG::Send(Framework::CPacket * pPacket)
{
	return ECommSendResult_SendingError;
}

BOOL CDeviceSocketMNETG::OnInitDialog()
{
	CDeviceBase::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	SetTimer(RAVID_TIMER_COMM, 10, nullptr);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDeviceSocketMNETG::OnDestroy()
{
	CDeviceBase::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	KillTimer(RAVID_TIMER_COMM);
}
void CDeviceSocketMNETG::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(IsWindowVisible())
	{
		if(nIDEvent == RAVID_TIMER_COMM)
		{

		}
	}
	
	CDeviceBase::OnTimer(nIDEvent);
}

bool CDeviceSocketMNETG::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = false;

	bool bFoundID = false;

	CString strMessage;

	do
	{
		switch(nParam)
		{
		case EDeviceParameterMNETG_DeviceID:
			{
				bReturn = !SetDeviceID(strValue);
				bFoundID = true;
			}
			break;
		case EDeviceParameterMNETG_Channel:
			{
				bReturn = SetChannel(_ttoi(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterMNETG_Station:
			{
				bReturn = SetStationNo(_ttoi(strValue));
				bFoundID = true;
			}
			break;
		case EDeviceParameterMNETG_Network:
			{
				bReturn = SetNetworkNo(_ttoi(strValue));
				bFoundID = true;
			}
			break;

		default:
				strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtofindtheparameter);
			break;
		}

		if(EDeviceParameterMNETG_Channel < nParam && EDeviceParameterMNETG_Count > nParam)
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

bool CDeviceSocketMNETG::SendData(LPCTSTR lpszAddr, short sDevType, long nLength, short* lpsData)
{
	bool bReturn = false;

	if(!IsInitialized())
		return false;

	if(!lpszAddr || !_tcslen(lpszAddr))
		return false;

	Lock();

	nLength *= sizeof(short);

	int nDevType = sDevType;//GetDeviceTypeNumber(lpszAddr);
	int nDevNo = _ttoi(lpszAddr + 1);

	//////////////////////////////////////////////////////////////////////////
// 	int nStNo = 255; 
// // 
// 	switch( nDevNo )
// 	{
// 	case 400: nDevNo = 1024; break;
// 	case 410: nDevNo = 1040; break;
// 	case 420: nDevNo = 1056; break;
// 	case 430: nDevNo = 1072; break;
// 	default: break;
// 	}


	//nDevNo = 1024;
	//////////////////////////////////////////////////////////////////////////

	int nStNo = m_nStationNo | ((m_nNetworkNo << 8) & 0xFF00);
	short nWriteSize = nLength;
	CString strMsg;
	int nResult = mdSend(m_nPath, nStNo, nDevType, nDevNo, &nWriteSize, lpsData);

	bReturn = !nResult;

	Unlock();

	return bReturn;
}
bool CDeviceSocketMNETG::SendData(LPCTSTR lpszAddr, short sDevType, long nLength, long* lplData)
{
	bool bReturn = false;

	if(!IsInitialized())
		return false;

	Lock();

	nLength *= sizeof(long);

	int nDevType = sDevType;//GetDeviceTypeNumber(lpszAddr);
	int nDevNo = _ttoi(lpszAddr + 1);

	//////////////////////////////////////////////////////////////////////////
// 	int nStNo = 255;
// 	nDevNo = 1024;
	//////////////////////////////////////////////////////////////////////////

	int nStNo = m_nStationNo | ((m_nNetworkNo << 8) & 0xFF00);

	short nWriteSize = nLength;

	int nResult = mdSend(m_nPath, nStNo, nDevType, nDevNo, &nWriteSize, lplData);


	bReturn = !nResult;

	Unlock();

	return bReturn;
}
bool CDeviceSocketMNETG::SendData(short sDevType, short sDevNo, long nLength, short* lpsData)
{
	bool bReturn = false;

	if(!IsInitialized())
		return false;

	Lock();

	nLength *= sizeof(short);

	int nStNo = m_nStationNo | ((m_nNetworkNo << 8) & 0xFF00);
	short sWriteSize = nLength;

	int nResult = mdSend(m_nPath, nStNo, sDevType, sDevNo, &sWriteSize, lpsData);

	bReturn = !nResult;

	Unlock();

	return bReturn;
}
bool CDeviceSocketMNETG::SendData(short sDevType, short sDevNo, long nLength, long* lplData)
{
	bool bReturn = false;

	if(!IsInitialized())
		return false;

	Lock();

	nLength *= sizeof(long);

	int nStNo = m_nStationNo | ((m_nNetworkNo << 8) & 0xFF00);
	short sWriteSize = nLength;

	int nResult = mdSend(m_nPath, nStNo, sDevType, sDevNo, &sWriteSize, lplData);

	bReturn = !nResult;

	Unlock();

	return bReturn;
}
bool CDeviceSocketMNETG::SendData(long nDevType, long nDevNo, long nLength, short* lpsData)
{
	bool bReturn = false;

	if(!IsInitialized())
		return false;

	Lock();

	nLength *= sizeof(short);

	long nWriteSize = nLength;

	int nResult = mdSendEx(m_nPath, m_nNetworkNo, m_nStationNo, nDevType, nDevNo, &nWriteSize, lpsData);

	bReturn = !nResult;

	Unlock();

	return bReturn;
}

bool CDeviceSocketMNETG::RecvData(LPCTSTR lpszAddr, short sDevType, long nLength, short* lpsData)
{
	bool bReturn = false;

	if(!IsInitialized())
		return false;

	Lock();

	nLength *= sizeof(short);

	int nDevType = sDevType;//GetDeviceTypeNumber(lpszAddr);
	int nDevNo = _ttoi(lpszAddr + 1);

	//////////////////////////////////////////////////////////////////////////
// 	int nStNo = 255; 
// 	switch( nDevNo )
// 	{
// 	case 130 : nDevNo = 304; break;
// 	case 140 : nDevNo = 320; break;
// 	default: break;
// 	}
	//////////////////////////////////////////////////////////////////////////

	int nStNo = m_nStationNo | ((m_nNetworkNo << 8) & 0xFF00);
	short nReadSize = nLength;

	int nResult = mdReceive(m_nPath, nStNo, nDevType, nDevNo, &nReadSize, lpsData);

	bReturn = !nResult;

	Unlock();

	return bReturn;
}
bool CDeviceSocketMNETG::RecvData(LPCTSTR lpszAddr, short sDevType, long nLength, long* lplData)
{
	bool bReturn = false;

	if(!IsInitialized())
		return false;

	Lock();

	nLength *= sizeof(long);

	int nDevType = sDevType;//GetDeviceTypeNumber(lpszAddr);
	int nDevNo = _ttoi(lpszAddr + 1);

	//////////////////////////////////////////////////////////////////////////
// 	int nStNo = 255; 
// 	switch( nDevNo )
// 	{
// 	case 110 : nDevNo = 272; break;
// 	case 112 : nDevNo = 274; break;
// 	case 114 : nDevNo = 276; break;
// 	default: break;
// 	}
	//////////////////////////////////////////////////////////////////////////

	int nStNo = m_nStationNo | ((m_nNetworkNo << 8) & 0xFF00);
	short nReadSize = nLength;

	int nResult = mdReceive(m_nPath, nStNo, nDevType, nDevNo, &nReadSize, lplData);

	bReturn = !nResult;

	return bReturn;
}
bool CDeviceSocketMNETG::RecvData(short sDevType, short sDevNo, long nLength, short* lpsData)
{
	bool bReturn = false;

	if(!IsInitialized())
		return false;

	Lock();

	nLength *= sizeof(short);

	int nStNo = m_nStationNo | ((m_nNetworkNo << 8) & 0xFF00);
	short sReadSize = nLength;

	int nResult = mdReceive(m_nPath, nStNo, sDevType, sDevNo, &sReadSize, lpsData);

	bReturn = !nResult;

	Unlock();
	
	return bReturn;
}
bool CDeviceSocketMNETG::RecvData(short sDevType, short sDevNo, long nLength, long* lplData)
{
	bool bReturn = false;

	if(!IsInitialized())
		return false;

	Lock();

	nLength *= sizeof(long);

	int nStNo = m_nStationNo | ((m_nNetworkNo << 8) & 0xFF00);
	short sReadSize = nLength;

	int nResult = mdReceive(m_nPath, nStNo, sDevType, sDevNo, &sReadSize, lplData);

	bReturn = !nResult;

	Unlock();

	return bReturn;
}
bool CDeviceSocketMNETG::RecvData(long nDevType, long nDevNo, long nLength, short* lpsData)
{
	bool bReturn = false;

	if(!IsInitialized())
		return false;

	Lock();

	nLength *= sizeof(short);

	long nReadSize = nLength;

	int nResult = mdReceiveEx(m_nPath, m_nNetworkNo, m_nStationNo, nDevType, nDevNo, &nReadSize, lpsData);

	bReturn = !nResult;

	Unlock();

	return bReturn;
}

bool CDeviceSocketMNETG::SetChannel(int nChannel)
{
	bool bReturn = false;

	CString strParamName(g_lpszParamMNETG[EDeviceParameterMNETG_Channel]);

	CString strPreValue;

	CString strNewValue;
	strNewValue.Format(_T("%d"), nChannel);

	do
	{
		if(IsInitialized())
			break;

		auto iter = std::find_if(m_vctParameterFieldConfigurations.begin(), m_vctParameterFieldConfigurations.end(),
								 [&](SDeviceParameterFieldConfigurations sEle) -> bool
		{
			return !sEle.strParameterName.CompareNoCase(strParamName);
		});

		if(iter == m_vctParameterFieldConfigurations.end())
			break;

		strPreValue = GetParamValue(iter->nPrimaryKey);

		if(!SetParamValue(iter->nPrimaryKey, strNewValue))
			break;

		if(!SaveSettings(iter->nPrimaryKey))
			break;

		iter->strParameterValue = strNewValue;

		m_nChannel = nChannel;

		bReturn = true;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s to '%s'"), bReturn ? _T("Succeeded") : _T("Failed"), strParamName, strPreValue, strNewValue);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return bReturn;
}
int CDeviceSocketMNETG::GetChannel()
{
	int nChannel = 0;

	do
	{
		auto iter = std::find_if(m_vctParameterFieldConfigurations.begin(), m_vctParameterFieldConfigurations.end(),
								 [&](SDeviceParameterFieldConfigurations sEle) -> bool
		{
			return !sEle.strParameterName.CompareNoCase(g_lpszParamMNETG[EDeviceParameterMNETG_Channel]);
		});

		if(iter == m_vctParameterFieldConfigurations.end())
			break;

		nChannel = _ttoi(iter->strParameterValue);
	}
	while(false);

	if(m_nChannel != nChannel)
		m_nChannel = nChannel;

	return m_nChannel;
}

bool CDeviceSocketMNETG::SetStationNo(int nStationNo)
{
	bool bReturn = false;

	CString strParamName(g_lpszParamMNETG[EDeviceParameterMNETG_Station]);

	CString strPreValue;

	CString strNewValue;
	strNewValue.Format(_T("%d"), nStationNo);

	do
	{
		if(IsInitialized())
			break;

		auto iter = std::find_if(m_vctParameterFieldConfigurations.begin(), m_vctParameterFieldConfigurations.end(),
								 [&](SDeviceParameterFieldConfigurations sEle) -> bool
		{
			return !sEle.strParameterName.CompareNoCase(strParamName);
		});

		if(iter == m_vctParameterFieldConfigurations.end())
			break;

		strPreValue = GetParamValue(iter->nPrimaryKey);

		if(!SetParamValue(iter->nPrimaryKey, strNewValue))
			break;

		if(!SaveSettings(iter->nPrimaryKey))
			break;

		iter->strParameterValue = strNewValue;

		m_nStationNo = nStationNo;

		bReturn = true;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s to '%s'"), bReturn ? _T("Succeeded") : _T("Failed"), strParamName, strPreValue, strNewValue);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return bReturn;
}
int CDeviceSocketMNETG::GetStationNo()
{
	int nStationNo = 0;

	do
	{
		auto iter = std::find_if(m_vctParameterFieldConfigurations.begin(), m_vctParameterFieldConfigurations.end(),
								 [&](SDeviceParameterFieldConfigurations sEle) -> bool
		{
			return !sEle.strParameterName.CompareNoCase(g_lpszParamMNETG[EDeviceParameterMNETG_Station]);
		});

		if(iter == m_vctParameterFieldConfigurations.end())
			break;

		nStationNo = _ttoi(iter->strParameterValue);
	}
	while(false);

	if(m_nStationNo != nStationNo)
		m_nStationNo = nStationNo;

	return m_nStationNo;
}

bool CDeviceSocketMNETG::SetNetworkNo(int nNetworkNo)
{
	bool bReturn = false;

	CString strParamName(g_lpszParamMNETG[EDeviceParameterMNETG_Network]);

	CString strPreValue;

	CString strNewValue;
	strNewValue.Format(_T("%d"), nNetworkNo);

	do
	{
		if(IsInitialized())
			break;

		auto iter = std::find_if(m_vctParameterFieldConfigurations.begin(), m_vctParameterFieldConfigurations.end(),
								 [&](SDeviceParameterFieldConfigurations sEle) -> bool
		{
			return !sEle.strParameterName.CompareNoCase(strParamName);
		});

		if(iter == m_vctParameterFieldConfigurations.end())
			break;

		strPreValue = GetParamValue(iter->nPrimaryKey);

		if(!SetParamValue(iter->nPrimaryKey, strNewValue))
			break;

		if(!SaveSettings(iter->nPrimaryKey))
			break;

		iter->strParameterValue = strNewValue;

		m_nNetworkNo = nNetworkNo;

		bReturn = true;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s to '%s'"), bReturn ? _T("Succeeded") : _T("Failed"), strParamName, strPreValue, strNewValue);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return bReturn;
}
int CDeviceSocketMNETG::GetNetworkNo()
{
	int nNetworkNo = 0;

	do
	{
		auto iter = std::find_if(m_vctParameterFieldConfigurations.begin(), m_vctParameterFieldConfigurations.end(),
								 [&](SDeviceParameterFieldConfigurations sEle) -> bool
		{
			return !sEle.strParameterName.CompareNoCase(g_lpszParamMNETG[EDeviceParameterMNETG_Network]);
		});

		if(iter == m_vctParameterFieldConfigurations.end())
			break;

		nNetworkNo = _ttoi(iter->strParameterValue);
	}
	while(false);

	if(m_nNetworkNo != nNetworkNo)
		m_nNetworkNo = nNetworkNo;

	return m_nNetworkNo;
}

long CDeviceSocketMNETG::GetDeviceTypeNumber(LPCTSTR lpszAddr)
{
	long nResult = 0;

	switch(lpszAddr[0])
	{
	case _T('D'):
	case _T('d'):
		nResult = DevD;
		break;
	case _T('W'):
	case _T('w'):
		nResult = DevW;
		break;
	case _T('B'):
	case _T('b'):
		nResult = DevB;
		break;
	case _T('X'):
	case _T('x'):
		nResult = DevX;
		break;
	case _T('Y'):
	case _T('y'):
		nResult = DevY;
		break;
	case _T('Z'):
	case _T('z'):
		nResult = DevZ;
		break;
	case _T('R'):
	case _T('r'):
		nResult = DevR;
		break;
	case _T('V'):
	case _T('v'):
		nResult = DevV;
		break;
	default:
		VERIFY(false);		// 너무 많아서 다 추가 못했으므로 여기 걸린사람들은 알아서 위를 참조하여 추가하도록 하시오. 20130723 jhsong
		break;
	}

	return nResult;
}

bool CDeviceSocketMNETG::DoesModuleExist()
{
	bool bReturn = false;

	CString strModuleName;

	do
	{
		strModuleName.Format(_T("MdFunc32.dll"));

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

#endif