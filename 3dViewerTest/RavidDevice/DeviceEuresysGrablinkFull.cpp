#include "stdafx.h"

#include "DeviceEuresysGrablinkFull.h"

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

IMPLEMENT_DYNAMIC(CDeviceEuresysGrablinkFull, CDeviceFrameGrabber)

BEGIN_MESSAGE_MAP(CDeviceEuresysGrablinkFull, CDeviceFrameGrabber)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static LPCTSTR g_lpszParamGrablinkFull[EDeviceParameterEuresysGrablinkFull_Count - EDeviceParameterMulticam_Count] =
{
	_T("Topology"),
	_T("Connector"),
};

static LPCTSTR g_lpszEuresysFullTopology[EDeviceEuresysFullTopology_Count] =
{
	_T("DECA"),
	_T("MONO"),
	_T("MONO_DECA"),
	_T("MONO_DECA_AFE"),
	_T("MONO_DECA_AFE_SE1"),
	_T("MONO_DECA_DFE"),
	_T("MONO_DECA_EF1"),
	_T("MONO_DECA_EXT1"),
	_T("MONO_DECA_OPT1"),
	_T("MONO_EF1"),
	_T("MONO_EXT1"),
	_T("MONO_From20MHzTo35MHz"),
	_T("MONO_From30MHzTo70MHz"),
	_T("MONO_From60MHzTo85MHz"),
	_T("MONO_OPT1"),
	_T("MONO_SLOW"),
};

static LPCTSTR g_lpszEuresysFullConnector[EDeviceEuresysFullConnector_Count] =
{
	_T("M"),
};

static LPCTSTR g_lpszGrablinkTapConfiguration[EDeviceGrablinkTapConfiguration_Count] =
{
		_T("BASE_1T10"),
		_T("BASE_1T12"),
		_T("BASE_1T14"),
		_T("BASE_1T16"),
		_T("BASE_1T24"),
		_T("BASE_1T24B3"),
		_T("BASE_1T30B2"),
		_T("BASE_1T30B3"),
		_T("BASE_1T36B2"),
		_T("BASE_1T36B3"),
		_T("BASE_1T42B2"),
		_T("BASE_1T42B3"),
		_T("BASE_1T48B2"),
		_T("BASE_1T48B3"),
		_T("BASE_1T8"),
		_T("BASE_2T10"),
		_T("BASE_2T12"),
		_T("BASE_2T14B2"),
		_T("BASE_2T16B2"),
		_T("BASE_2T24B2"),
		_T("BASE_2T8"),
		_T("BASE_3T10B2"),
		_T("BASE_3T12B2"),
		_T("BASE_3T14B2"),
		_T("BASE_3T16B2"),
		_T("BASE_3T8"),
		_T("BASE_4T10B2"),
		_T("BASE_4T12B2"),
		_T("BASE_4T8B2"),
		_T("DECA_10T8"),
		_T("DECA_10T8C"),
		_T("DECA_3T24"),
		_T("DECA_9T8"),
		_T("FULL_8T8"),
		_T("FULL_8T8C"),
		_T("INVALID"),
		_T("LITE_1T10"),
		_T("LITE_1T8"),
		_T("MEDIUM_1T30"),
		_T("MEDIUM_1T36"),
		_T("MEDIUM_1T42"),
		_T("MEDIUM_1T48"),
		_T("MEDIUM_2T14"),
		_T("MEDIUM_2T16"),
		_T("MEDIUM_2T24"),
		_T("MEDIUM_3T10"),
		_T("MEDIUM_3T12"),
		_T("MEDIUM_3T14"),
		_T("MEDIUM_3T16"),
		_T("MEDIUM_4T10"),
		_T("MEDIUM_4T12"),
		_T("MEDIUM_4T8"),
		_T("MEDIUM_6T8"),
		_T("MEDIUM_8T8B2"),
};

CDeviceEuresysGrablinkFull::CDeviceEuresysGrablinkFull()
{
	m_bIsFramegrabber = true;
}


CDeviceEuresysGrablinkFull::~CDeviceEuresysGrablinkFull()
{
}

EDeviceInitializeResult CDeviceEuresysGrablinkFull::Initialize()
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

		int nDeviceID = _ttoi(GetDeviceID());

		m_nDriveIndex = -1;

		for(int i = 0; i < m_nConnectBoard; i++)
		{
			int nDevType = 0;

			if(McGetParamInt(MC_BOARD + i, MC_BoardType, &nDevType) != MC_OK)
				break;

			if(nDevType == MC_BoardType_GRABLINK_FULL)
			{
				if(nFindCount != nDeviceID)
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

		EDeviceEuresysFullTopology eTopology = EDeviceEuresysFullTopology_Count;

		if(GetTopology(&eTopology))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Topology"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(McSetParamStr(MC_BOARD + m_nDriveIndex, MC_BoardTopology, CStringA(g_lpszEuresysFullTopology[eTopology])) != MC_OK)
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

		EDeviceEuresysFullConnector eConnector = EDeviceEuresysFullConnector_Count;

		if(GetConnector(&eConnector))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Connector"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(McSetParamStr(m_hDevice, MC_Connector, CStringA(g_lpszEuresysFullConnector[eConnector])) != MC_OK)
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

// 		int nSaveDB = -1;
// 
// 		if(McGetParamInt(m_hDevice, MC_ImagePixelSize, &nHeight) != MC_OK)
// 		{
// 			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("ImageSizeY"));
// 			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
// 			break;
// 		}


		if(nWidth < 1 || nHeight < 1)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("ImageSizeX or ImageSizeY"));
			eReturn = EDeviceInitializeResult_WriteToDatabaseError;
			break;
		}

		//임시 체크인
		{
			unsigned int unTapConfigBit = 8;
			EDeviceGrablinkTapConfiguration eParam;

			GetTapConfiguration(&eParam);

			CString strTapConfig;
			strTapConfig.Format(g_lpszGrablinkTapConfiguration[eParam]);

			if(strTapConfig.Find(_T("_")) != -1)
			{
				strTapConfig.Delete(0, strTapConfig.Find(_T("_")) + 1);
				if(strTapConfig.Find(_T("T")) != -1)
				{
					strTapConfig.Delete(0, strTapConfig.Find(_T("T")) + 1);
					if(strTapConfig.Find(_T("B")) != -1)
						strTapConfig.Delete(strTapConfig.Find(_T("B")), strTapConfig.GetLength() - 1);

					unTapConfigBit = _ttoi(strTapConfig);
				}
			}

			if(!unTapConfigBit)
				unTapConfigBit = 8;

			unsigned int unMaxValue = 1;
			for(unsigned int i = 0; i < unTapConfigBit; ++i)
				unMaxValue <<= 1;
			--unMaxValue;

			int64_t i64Channels = m_bIsColor ? 3 : 1;

			CMultipleVariable mv;
			for(int i = 0; i < i64Channels; ++i)
				mv.AddValue(unMaxValue);

			this->InitBuffer(nWidth, nHeight, mv, CRavidImage::MakeValueFormat(!m_bIsColor ? 1 : 3, unTapConfigBit));
			this->ConnectImage();
		}
		
		if(McRegisterCallback(m_hDevice, CDeviceEuresysGrablinkFull::CallbackFunction, this) != MC_OK)
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

EDeviceTerminateResult CDeviceEuresysGrablinkFull::Terminate()
{
	return __super::Terminate();
}

bool CDeviceEuresysGrablinkFull::LoadSettings()
{
	bool bReturn = false;

	m_vctParameterFieldConfigurations.clear();

	do
	{
		CDeviceEuresysMulticam::LoadSettings();

		AddParameterFieldConfigurations(EDeviceParameterEuresysGrablinkFull_Topology, g_lpszParamGrablinkFull[EDeviceParameterEuresysGrablinkFull_Topology - EDeviceParameterMulticam_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszEuresysFullTopology, EDeviceEuresysFullTopology_Count), nullptr, 0);

		AddParameterFieldConfigurations(EDeviceParameterEuresysGrablinkFull_Connector, g_lpszParamGrablinkFull[EDeviceParameterEuresysGrablinkFull_Connector - EDeviceParameterMulticam_Count], _T("M"), EParameterFieldType_Static, ConvertStringArrayToComboElement(g_lpszEuresysFullConnector, EDeviceEuresysFullConnector_Count), nullptr, 0);

		__super::LoadSettings();

		bReturn = true;
	}
	while(false);

	return bReturn & CDeviceBase::LoadSettings();
}

EEuresysGetFunction CDeviceEuresysGrablinkFull::GetTopology(EDeviceEuresysFullTopology * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceEuresysFullTopology)_ttoi(GetParamValue(EDeviceParameterEuresysGrablinkFull_Topology));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablinkFull::SetTopology(EDeviceEuresysFullTopology eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablinkFull eSaveID = EDeviceParameterEuresysGrablinkFull_Topology;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceEuresysFullTopology_Count)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablinkFull[eSaveID - EDeviceParameterMulticam_Count], g_lpszEuresysFullTopology[nPreValue], g_lpszEuresysFullTopology[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablinkFull::GetConnector(EDeviceEuresysFullConnector * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceEuresysFullConnector)_ttoi(GetParamValue(EDeviceParameterEuresysGrablinkFull_Connector));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablinkFull::SetConnector(EDeviceEuresysFullConnector eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablinkFull eSaveID = EDeviceParameterEuresysGrablinkFull_Connector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceEuresysFullConnector_Count)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablinkFull[eSaveID - EDeviceParameterMulticam_Count], g_lpszEuresysFullConnector[nPreValue], g_lpszEuresysFullConnector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceEuresysGrablinkFull::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = false;

	if(__super::OnParameterChanged(nParam, strValue))
		return true;

	bool bFoundID = true;

	do
	{
		switch(nParam)
		{
		case EDeviceParameterEuresysGrablinkFull_Topology:
			bReturn = !SetTopology((EDeviceEuresysFullTopology)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablinkFull_Connector:
			bReturn = !SetConnector((EDeviceEuresysFullConnector)_ttoi(strValue));
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