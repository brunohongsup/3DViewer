#include "stdafx.h"

#include "DeviceEuresysDominoSymphony.h"

#ifdef USING_DEVICE

#include "../RavidCore/RavidRect.h"
#include "../RavidCore/RavidImage.h"
#include "../RavidCore/RavidDatabase.h"
#include "../RavidCore/RavidRecordset.h"
#include "../RavidCore/PerformanceCounter.h"

#include "../RavidFramework/LogManager.h"
#include "../RavidFramework/FrameworkManager.h"
#include "../RavidFramework/EventHandlerManager.h"
#include "../RavidFramework/MultiLanguageManager.h"

#include "../RavidFramework/RavidImageView.h"

#include "../Libraries/Includes/MultiCam/multicam.h"

// multicam.dll
#pragma comment(lib, COMMONLIB_PREFIX "MultiCam/MultiCam.lib")

using namespace Ravid;
using namespace Ravid::Device;
using namespace Ravid::Database;
using namespace Ravid::Framework;
using namespace Ravid::Algorithms;
using namespace Ravid::Miscellaneous;

IMPLEMENT_DYNAMIC(CDeviceEuresysDominoSymphony, CDeviceFrameGrabber)

BEGIN_MESSAGE_MAP(CDeviceEuresysDominoSymphony, CDeviceFrameGrabber)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static LPCTSTR g_lpszParamDominoSymphony[EDeviceParameterEuresysDominoSymphony_Count - EDeviceParameterMulticam_Count] =
{
	_T("Topology"),
	_T("Connector"),
};

static LPCTSTR g_lpszDominoSymphonyTopology[EDeviceDominoSymphonyTopology_Count] =
{
	_T("1_1_1_1"),
};


static LPCTSTR g_lpszDominoSymphonyConnector[EDeviceDominoSymphonyConnector_Count] =
{
	_T("A"),
	_T("B"),
	_T("C"),
	_T("D"),
};

CDeviceEuresysDominoSymphony::CDeviceEuresysDominoSymphony()
{
	m_bIsFramegrabber = true;
}


CDeviceEuresysDominoSymphony::~CDeviceEuresysDominoSymphony()
{
}

EDeviceInitializeResult CDeviceEuresysDominoSymphony::Initialize()
{
	EDeviceInitializeResult eReturn = EDeviceInitializeResult_UnknownError;

	eReturn = __super::Initialize();

	if(eReturn)
		return eReturn;
	
	eReturn = EDeviceInitializeResult_UnknownError;

	CString strMessage, strStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	do
	{
		int nFindCount = 0;

		int nObjectID = _ttoi(GetDeviceID());

		m_nDriveIndex = -1;

		for(int i = 0; i < m_nConnectBoard; i++)
		{
			int nDevType = 0;

			if(McGetParamInt(MC_BOARD + i, MC_BoardType, &nDevType) != MC_OK)
				break;

			if(nDevType == MC_BoardType_DOMINO_SYMPHONY || nDevType ==MC_BoardType_DOMINO_SYMPHONY_PCI || nDevType == MC_BoardType_DOMINO_SYMPHONY_PCIe)
			{
				if(nFindCount != nObjectID)
				{
					++nFindCount;
					continue;
				}

				m_nDriveIndex = i;
				break;
			}
		}

		if(m_nDriveIndex == -1)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtofindthedevice);
			eReturn = EDeviceInitializeResult_NotFoundDeviceError;
			break;
		}

		EDeviceDominoSymphonyTopology eTopology = EDeviceDominoSymphonyTopology_Count;

		if(GetTopology(&eTopology))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Topology"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(McSetParamStr(MC_BOARD + m_nDriveIndex, MC_BoardTopology, CStringA(g_lpszDominoSymphonyTopology[eTopology])) != MC_OK)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Topology"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		if(McCreate(MC_CHANNEL, &m_hDevice) != MC_OK)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtogeneratethedevicehandle);
			eReturn = EDeviceInitializeResult_NotCreateDeviceError;
			break;
		}

		if(McSetParamInt(m_hDevice, MC_DriverIndex, m_nDriveIndex) != MC_OK)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("DriverIndex"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		EDeviceDominoSymphonyConnector eConnector = EDeviceDominoSymphonyConnector_Count;

		if(GetConnector(&eConnector))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Connector"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(McSetParamStr(m_hDevice, MC_Connector, CStringA(g_lpszDominoSymphonyConnector[eConnector])) != MC_OK)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Connector"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		EDeviceMulticamInitializeType eInitType = EDeviceMulticamInitializeType_Camfile;

		GetInitializetype(&eInitType);

		switch(eInitType)
		{
		case EDeviceMulticamInitializeType_Camfile:
		{
			CString strCamPath;

			GetCamfilePath(&strCamPath);

			if((McSetParamStr(m_hDevice, MC_CamFile, CStringA(strCamPath))) != MC_OK)
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("CamFile"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}

			int nParam = -1;

			McGetParamInt(m_hDevice, MC_Imaging, &nParam);

			if(nParam == MC_Imaging_AREA)
				m_bIsLine = false;
			else
				m_bIsLine = true;

			nParam = -1;

			McGetParamInt(m_hDevice, MC_Spectrum, &nParam);

			if(nParam == MC_Spectrum_COLOR)
				m_bIsColor = true;
			else
				m_bIsColor = false;

			bool bCamMatch = false;

			GetCamfileMatch(&bCamMatch);

			if(bCamMatch)
				SetUpdateParameter();
		}
		break;
		case EDeviceMulticamInitializeType_Parameter:
			SetUpdateDevice();
			break;
		default:
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Initialize Type"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(eReturn != EDeviceInitializeResult_UnknownError)
			break;

		int nWidth = -1, nHeight = -1;

		if(McGetParamInt(m_hDevice, MC_ImageSizeX, &nWidth) != MC_OK)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("ImageSizeX"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		if(McGetParamInt(m_hDevice, MC_ImageSizeY, &nHeight) != MC_OK)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("ImageSizeY"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		if(nWidth < 1 || nHeight < 1)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("ImageSizeX or ImageSizeY"));
			eReturn = EDeviceInitializeResult_WriteToDatabaseError;
			break;
		}

		CMultipleVariable mv;
		for(int i = 0; i < 1; ++i)
			mv.AddValue(0xff);

		this->InitBuffer(nWidth, nHeight, mv, CRavidImage::MakeValueFormat(1, 8));
		this->ConnectImage();

		if(McRegisterCallback(m_hDevice, CDeviceEuresysDominoSymphony::CallbackFunction, this) != MC_OK)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtocreatethread);
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		if(McSetParamInt(m_hDevice, MC_ClusterMode, MC_ClusterMode_AUTO) != MC_OK)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ClusterMode"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		if(McSetParamInt(m_hDevice, MC_SignalEnable + MC_SIG_ACQUISITION_FAILURE, MC_SignalEnable_ON) != MC_OK)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ACQUISITION_FAILURE"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		if(McSetParamInt(m_hDevice, MC_SignalEnable + MC_SIG_SURFACE_FILLED, MC_SignalEnable_ON) != MC_OK)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("SURFACE_FILLED"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		if((McSetParamInt(m_hDevice, MC_ChannelState, MC_ChannelState_IDLE)) != MC_OK)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("IDLE"));
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

EDeviceTerminateResult CDeviceEuresysDominoSymphony::Terminate()
{
	return __super::Terminate();
}

bool CDeviceEuresysDominoSymphony::LoadSettings()
{
	bool bReturn = false;

	m_vctParameterFieldConfigurations.clear();

	do
	{
		CDeviceEuresysMulticam::LoadSettings();

		AddParameterFieldConfigurations(EDeviceParameterEuresysDominoSymphony_Topology, g_lpszParamDominoSymphony[EDeviceParameterEuresysDominoSymphony_Topology - EDeviceParameterMulticam_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoSymphonyTopology, EDeviceDominoSymphonyTopology_Count), nullptr, 0);

		AddParameterFieldConfigurations(EDeviceParameterEuresysDominoSymphony_Connector, g_lpszParamDominoSymphony[EDeviceParameterEuresysDominoSymphony_Connector - EDeviceParameterMulticam_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoSymphonyConnector, EDeviceDominoSymphonyConnector_Count), nullptr, 0);

		__super::LoadSettings();

		bReturn = true;
	}
	while(false);

	return bReturn & CDeviceBase::LoadSettings();
}

EEuresysGetFunction CDeviceEuresysDominoSymphony::GetTopology(EDeviceDominoSymphonyTopology * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceDominoSymphonyTopology)_ttoi(GetParamValue(EDeviceParameterEuresysDominoSymphony_Topology));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDominoSymphony::SetTopology(EDeviceDominoSymphonyTopology eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDominoSymphony eSaveID = EDeviceParameterEuresysDominoSymphony_Topology;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoSymphonyTopology_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EEuresysSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EEuresysSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EEuresysSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEuresysSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDominoSymphony[eSaveID - EDeviceParameterMulticam_Count], g_lpszDominoSymphonyTopology[nPreValue], g_lpszDominoSymphonyTopology[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDominoSymphony::GetConnector(EDeviceDominoSymphonyConnector * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if (!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceDominoSymphonyConnector)_ttoi(GetParamValue(EDeviceParameterEuresysDominoSymphony_Connector));

		eReturn = EEuresysGetFunction_OK;
	} while (false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDominoSymphony::SetConnector(EDeviceDominoSymphonyConnector eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDominoSymphony eSaveID = EDeviceParameterEuresysDominoSymphony_Connector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if (eParam < 0 || eParam >= EDeviceDominoSymphonyConnector_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if (IsInitialized())
		{
			eReturn = EEuresysSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if (!SetParamValue(eSaveID, strSave))
		{
			eReturn = EEuresysSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EEuresysSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EEuresysSetFunction_OK;
	} while (false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDominoSymphony[eSaveID - EDeviceParameterMulticam_Count], g_lpszDominoSymphonyConnector[nPreValue], g_lpszDominoSymphonyConnector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceEuresysDominoSymphony::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = false;

	if(__super::OnParameterChanged(nParam, strValue))
		return true;

	bool bFoundID = true;

	do
	{
		switch(nParam)
		{
		case EDeviceParameterEuresysDominoSymphony_Topology:
			bReturn = !SetTopology((EDeviceDominoSymphonyTopology)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDominoSymphony_Connector:
			bReturn = !SetConnector((EDeviceDominoSymphonyConnector)_ttoi(strValue));
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

#endif