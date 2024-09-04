#include "stdafx.h"

#include "DeviceBaslerTOFGigE.h"

#ifdef USING_DEVICE

#include "../RavidCore/RavidRect.h"
#include "../RavidCore/RavidImage.h"
#include "../RavidCore/RavidDatabase.h"
#include "../RavidCore/RavidRecordset.h"
#include "../RavidCore/PerformanceCounter.h"

#include "../RavidFramework/UIManager.h"
#include "../RavidFramework/LogManager.h"
#include "../RavidFramework/DeviceManager.h"
#include "../RavidFramework/LibraryManager.h"
#include "../RavidFramework/RavidImageView.h" 
#include "../RavidFramework/FrameworkManager.h"
#include "../RavidFramework/EventHandlerManager.h"
#include "../RavidFramework/MultiLanguageManager.h"

#include "../RavidFramework/MessageBase.h"
#include "../RavidFramework/RavidTreeView.h"
#include "../RavidFramework/RavidImageView.h"
#include "../RavidFramework/RavidImageViewLayer.h"
							
#include "../Libraries/Includes/BaslerTOF/ConsumerImplHelper/ToFCamera.h"


// gcbase_md_vc120_v3_0_basler_pylon_v5_0.dll
// genapi_md_vc120_v3_0_basler_pylon_v5_0.dll
#pragma comment(lib, COMMONLIB_PREFIX "BaslerPylon/PylonC_MD_vc120.lib")
#pragma comment(lib, COMMONLIB_PREFIX "BaslerTOF/GCBase_MD_VC120_v3_0_Basler_pylon_v5_0.lib")
#pragma comment(lib, COMMONLIB_PREFIX "BaslerTOF/GenApi_MD_VC120_v3_0_Basler_pylon_v5_0.lib")

using namespace Ravid;
using namespace Ravid::Device;
using namespace Ravid::Database;
using namespace Ravid::Framework;
using namespace Ravid::Algorithms;
using namespace Ravid::Miscellaneous;

IMPLEMENT_DYNAMIC(CDeviceBaslerTOFGigE, CDeviceCamera)

BEGIN_MESSAGE_MAP(CDeviceBaslerTOFGigE, CDeviceCamera)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static LPCTSTR g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_Count] =
{
	_T("DeviceID"),
	_T("Draw Point"),
	_T("Draw Image"),
	_T("Device Control"),
	_T("Device Channel"),
	_T("Calibration Range Offset"),
	_T("Rectification Control"),
	_T("Rectification"),
	_T("Device Temperature Selector"),
	_T("Image Control"),
	_T("Binning"),
	_T("Cnavas Width"),
	_T("Canvas Height"),
	_T("OffsetX"),
	_T("OffsetY"),
	_T("Minimum Depth_mm"),
	_T("Maximum Depth_mm"),
	_T("Component Selector"),
//	_T("Pixel Format"),
	_T("Acquisition Control"),
	_T("LED Disable"),
	_T("Processing Mode"),
	_T("Acquisition Frame Rate"),
	_T("Exposure Auto"),
	_T("Agility"),
	_T("Delay"),
	_T("Exposure TimeSelector"),
	_T("Exposure Time_us"),
	_T("Trigger Mode"),
	_T("Trigger Source"),
	_T("Trigger Delay_us"),
	_T("Sync Timer Control"),
	_T("Timestamp Latch"),
	_T("Timestamp Low_ns"),
	_T("Timestamp High_ns"),
	_T("Start Time Low_us"),
	_T("Start Time High_us"),
	_T("Trigger Rat_Hz"),
	_T("Update"),
	_T("Image Quality Control"),
	_T("Confidence Threshold"),
	_T("Spatial Filter"),
	_T("Temporal Filter"),
	_T("Strength"),
	_T("Outlier Tolerance"),
	_T("Range Filter"),
	_T("Digital IO Control"),
	_T("Line Selector"),
	_T("Input Line1"),
	_T("Line1 Inverter"),
	_T("Line1 Debouncer Time_us"),
	_T("Input Line2"),
	_T("Line2 Inverter"),
	_T("Line2 Debouncer Time_us"),
	_T("Output Line1"),
	_T("Output Line1 Source"),
	_T("Output Line1 Inverter"),
	_T("Output Line1 Output Pulse Min Width"),
	_T("Output Line2"),
	_T("Output Line2 Source"),
	_T("Output Line2 Inverter"),
	_T("Output Line2 Output Pulse Min Width"),
	_T("User Output Selector"),
	_T("User Output1"),
	_T("User Output1 Value"),
	_T("User Output2"),
	_T("User Output2 Value"),
	_T("User Output Value All"),
};

static LPCTSTR g_lpszBaslerTOFSwitch[EDeviceBaslerTOFGigESwitch_Count] =
{
	_T("Off"),
	_T("On"),
};

static LPCTSTR g_lpszBaslerTOFTemperatureSelector[EDeviceBaslerTOFGigETemperatureSelector_Count] =
{
	_T("Sensor Board"),
	_T("LED Board"),
};

static LPCTSTR g_lpszBaslerTOFComponentSelector[EDeviceBaslerTOFGigEComponentSelector_Count] =
{
	_T("Range"),
	_T("Intensity"),
	_T("Confidence"),
};

static LPCTSTR g_lpszBaslerTOFProcessingMode[EDeviceBaslerTOFGigEProcessingMode_Count] =
{
	_T("Standard"),
	_T("HDR"),
};

static LPCTSTR g_lpszBaslerTOFTriggerSource[EDeviceBaslerTOFGigETriggerSource_Count] =
{
	_T("Soft ware"),
	_T("Line1"),
	_T("Line2"),
	_T("Sync Timer"),
};

static LPCTSTR g_lpszBaslerTOFLineSelector[EDeviceBaslerTOFGigELineSelector_Count] =
{
	_T("Input Line1"),
	_T("Input Line2"),
	_T("Output Line1"),
	_T("Output Line2"),
};

static LPCTSTR g_lpszBaslerTOFLineSource[EDeviceBaslerTOFGigELineSource_Count] =
{
	_T("Exposure Active"),
	_T("User Output"),
	_T("Acqusition Trigger Wait"),
};

static LPCTSTR g_lpszBaslerTOFUserOutputSelector[EDeviceBaslerTOFGigEUserOutputSelector_Count] =
{
	_T("User Output1"),
	_T("User Output2"),
};

static LPCTSTR g_lpszBaslerTOFSelectImage[EDeviceBaslerTOFGigESelectImage_Count] =
{
	_T("Intensity"),
	_T("Confidence map"),
	_T("3D Data"),
};

static LPCTSTR g_lpszBaslerTOFDeviceChannel[EDeviceBaslerTOFGigEDeviceChannel_Count] =
{
	_T("0"),
	_T("1"),
	_T("2"),
	_T("3"),
};

static LPCTSTR g_lpszBaslerTOFExposureAuto[EDeviceBaslerTOFGigEExposureAuto_Count] =
{
	_T("Off"),
	_T("Continuous"),
};


class CustomAllocator : public GenTLConsumerImplHelper::BufferAllocator
{
public:
	virtual void* AllocateBuffer(size_t size_by) { return new char[size_by]; }
	virtual void FreeBuffer(void* pBuffer) { delete[] static_cast<char*>(pBuffer); }
	virtual void Delete() { delete this; }
};


CDeviceBaslerTOFGigE::CDeviceBaslerTOFGigE()
{
	m_nGrabCount = 1;
}

CDeviceBaslerTOFGigE::~CDeviceBaslerTOFGigE()
{
	Terminate();

	if(m_pCamera)
	{
		delete m_pCamera;
		m_pCamera = nullptr;
	}

	if(m_p3DInfo)
	{
		delete m_p3DInfo;
		m_p3DInfo = nullptr;
	}

	if(m_pConfidenceMap)
	{
		delete m_pConfidenceMap;
		m_pConfidenceMap = nullptr;
	}
}

ptrdiff_t CDeviceBaslerTOFGigE::OnMessage(Ravid::Framework::CMessageBase * pMessage)
{
	__super::OnMessage(pMessage);

	do
	{
		if(!pMessage)
			break;

		if(pMessage->GetMessage() != EMessage_ImageViewMouseUp)
			break;

		if(!IsWindow(GetSafeHwnd()))
			break;
			   
		if(!IsInitialized())
			break;

		CRavidImageView* pCurView = GetImageView();

		if(pCurView->GetObjectID() != pMessage->GetParams())
			break;

		if(pCurView)
		{
			m_DrawPoint = pCurView->GetLastUpImagePoint();

			if(IsLive() || !IsGrabAvailable())
				break;

			DrawPixelInfo(9);
		}
	}
	while(false);

	return 0;
}

EDeviceInitializeResult CDeviceBaslerTOFGigE::Initialize()
{
	EDeviceInitializeResult eReturn = EDeviceInitializeResult_UnknownError;

	CString strMessage, strStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("Basler"));
		
		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);

		SetStatus(strStatus);

		return EDeviceInitializeResult_NotFoundApiError;
	}

	do 
	{
		if(!m_pCamera)
			m_pCamera = new GenTLConsumerImplHelper::CToFCamera;

		CString strDeviceID = GetDeviceID();
		std::string stringDeviceID = CT2A(strDeviceID);
		
		bool bError = true;

		try
		{
			GenTLConsumerImplHelper::CToFCamera::InitProducer();

			m_pCamera->Open(GenTLConsumerImplHelper::SerialNumber, stringDeviceID);

			bError = false;
		}
		catch(...){}

		if(bError)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtofindthedevice);
			eReturn = EDeviceInitializeResult_NotFoundDeviceError;
			break;
		}
		
		EDeviceBaslerTOFGigEDeviceChannel eChannel = EDeviceBaslerTOFGigEDeviceChannel_Count;

		if(GetDeviceChannel(&eChannel))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Device Channel"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		SetInteger("DeviceChannel", eChannel);

		int nCalibrationRangeOffset = 0;

		if(GetCalibrationRangeOffset(&nCalibrationRangeOffset))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Calibration Range Offset"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		SetInteger("DeviceCalibOffset", nCalibrationRangeOffset);

		EDeviceBaslerTOFGigESwitch eRectification = EDeviceBaslerTOFGigESwitch_Count;

		if(GetRectification(&eRectification))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Rectification"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		switch(eRectification)
		{
		case EDeviceBaslerTOFGigESwitch_Off:
			SetEnumeration("Rectification", "Off");
			break;
		case EDeviceBaslerTOFGigESwitch_On:
			SetEnumeration("Rectification", "On");
			break;
		}

		EDeviceBaslerTOFGigETemperatureSelector eTemperatureSelector = EDeviceBaslerTOFGigETemperatureSelector_Count;

		if(GetDeviceTemperatureSelector(&eTemperatureSelector))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Dev.Temperature Selector"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		switch(eTemperatureSelector)
		{
		case EDeviceBaslerTOFGigETemperatureSelector_SensorBoard:
			SetEnumeration("DeviceTemperatureSelector", "SensorBoard");
			break;
		case EDeviceBaslerTOFGigETemperatureSelector_LEDBoard:
			SetEnumeration("DeviceTemperatureSelector", "LEDBoard");
			break;
		}

		int nMinimumDepth_mm = 0;

		if(GetMinimumDepth_mm(&nMinimumDepth_mm))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("MinimumDepth"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		SetInteger("DepthMin", nMinimumDepth_mm);

		int nMaximumDepth_mm = 0;

		if(GetMaximumDepth_mm(&nMaximumDepth_mm))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("MaximumDepth"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		SetInteger("DepthMax", nMaximumDepth_mm);

		EDeviceBaslerTOFGigEComponentSelector eComponentSelector = EDeviceBaslerTOFGigEComponentSelector_Count;

		if(GetComponentSelector(&eComponentSelector))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ComponentSelector"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}
		
		switch(eComponentSelector)
		{
		case EDeviceBaslerTOFGigEComponentSelector_Range:
			SetEnumeration("ComponentSelector", "Range");
			break;
		case EDeviceBaslerTOFGigEComponentSelector_Intensity:
			SetEnumeration("ComponentSelector", "Intensity");
			break;
		case EDeviceBaslerTOFGigEComponentSelector_Confidence:
			SetEnumeration("ComponentSelector", "Confidence");
			break;
		}

		bool bLEDDisable = false;

		if(GetLEDDisable(&bLEDDisable))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LED Disable"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		SetBoolean("LedDisable", bLEDDisable);

		EDeviceBaslerTOFGigEProcessingMode eProcessingMode = EDeviceBaslerTOFGigEProcessingMode_Count;

		if(GetProcessingMode(&eProcessingMode))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Processing Mode"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		switch(eProcessingMode)
		{
		case EDeviceBaslerTOFGigEProcessingMode_Standard:
			SetEnumeration("ProcessingMode", "Standard");
			break;
		case EDeviceBaslerTOFGigEProcessingMode_HDR:
			SetEnumeration("ProcessingMode", "Hdr");
			break;
		}

		float fAcquisitionFrameRate = 0.0f;

		if(GetAcquisitionFrameRate(&fAcquisitionFrameRate))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Acquisition Frame rate"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		SetFloating("AcquisitionFrameRate", fAcquisitionFrameRate);

		EDeviceBaslerTOFGigEExposureAuto eExposureAuto = EDeviceBaslerTOFGigEExposureAuto_Count;

		if(GetExposureAuto(&eExposureAuto))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Exposure Auto"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		switch(eExposureAuto)
		{
		case EDeviceBaslerTOFGigEExposureAuto_Off:
			SetEnumeration("ExposureAuto", "Off");
			break;
		case EDeviceBaslerTOFGigEExposureAuto_Continuous:
			SetEnumeration("ExposureAuto", "Continuous");
			break;
		}

		float fAgility = 0.0f;

		if(GetAgility(&fAgility))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Agility"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		SetFloating("Agility", fAgility);

		float fDelay = 0.0f;

		if(GetDelay(&fDelay))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Delay"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		SetFloating("Delay", fDelay);

		int nExposureTimeSelector = 0;

		if(GetExposureTimeSelector(&nExposureTimeSelector))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Exposure Time selector"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		SetInteger("ExposureTimeSelector", nExposureTimeSelector);

		int nExposureTime_us = 0;

		if(GetExposureTime_us(&nExposureTime_us))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Exposure Time"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		SetInteger("ExposureTime", nExposureTime_us);

		EDeviceBaslerTOFGigESwitch eTriggerMode = EDeviceBaslerTOFGigESwitch_Count;

		if(GetTriggerMode(&eTriggerMode))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Trigger mode"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		switch(eTriggerMode)
		{
		case EDeviceBaslerTOFGigESwitch_Off:
			SetEnumeration("TriggerMode", "Off");
			break;
		case EDeviceBaslerTOFGigESwitch_On:
			SetEnumeration("TriggerMode", "On");
			break;
		}

		EDeviceBaslerTOFGigETriggerSource eTriggerSource = EDeviceBaslerTOFGigETriggerSource_Count;

		if(GetTriggerSource(&eTriggerSource))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Trigger source"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		switch(eTriggerSource)
		{
		case EDeviceBaslerTOFGigETriggerSource_Software:
			SetEnumeration("TriggerSource", "Software");
			break;
		case EDeviceBaslerTOFGigETriggerSource_Line1:
			SetEnumeration("TriggerSource", "Line1");
			break;
		case EDeviceBaslerTOFGigETriggerSource_Line2:
			SetEnumeration("TriggerSource", "Line2");
			break;
		case EDeviceBaslerTOFGigETriggerSource_SyncTimer:
			SetEnumeration("TriggerSource", "SyncTimer");
			break;
		}

		float fTriggerDelay_us = 0.0f;

		if(GetTriggerDelay_us(&fTriggerDelay_us))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Trigger delay"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		SetFloating("TriggerDelay", fTriggerDelay_us);

		int nConfidenceThreshold = 0;

		if(GetConfidenceThreshold(&nConfidenceThreshold))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Confidence Threshold"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		SetInteger("ConfidenceThreshold", nConfidenceThreshold);

		bool bSpatialFilter = false;

		if(GetSpatialFilter(&bSpatialFilter))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Spatial Filter"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		SetBoolean("FilterSpatial", bSpatialFilter);

		bool bTemporalFilter = false;
		
		if(GetTemporalFilter(&bTemporalFilter))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Temporal Filter"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		SetBoolean("FilterTemporal", bTemporalFilter);

		int nStrength = 0;

		if(GetStrength(&nStrength))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Strength"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		SetInteger("FilterStrength", nStrength);

		int nOutlierTolerance = 0;

		if(GetOutlierTolerance(&nOutlierTolerance))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Outlier Tolerance"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		SetInteger("OutlierTolerance", nOutlierTolerance);

		bool bRangeFilter = false;

		if(GetRangeFilter(&bRangeFilter))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Range Filter"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		SetBoolean("FilterRange", bRangeFilter);

		if(true)
		{
			SetEnumeration("LineSelector", "Line1");

			bool bLine1Inverter = false;

			if(GetLine1Inverter(&bLine1Inverter))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line1 Inverter"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			SetBoolean("LineInverter", bLine1Inverter);

			float fLine1DebouncerTime_us = 0.0f;

			if(GetLine1DebouncerTime_us(&fLine1DebouncerTime_us))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line1 Debouncer time"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			SetFloating("LineDebouncerTime", fLine1DebouncerTime_us);
		}

		if(true)
		{
			SetEnumeration("LineSelector", "Line2");

			bool bLine2Inverter = false;

			if(GetLine2Inverter(&bLine2Inverter))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line2 Inverter"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			SetBoolean("LineInverter", bLine2Inverter);

			float fLine2DebouncerTime_us = 0.0f;

			if(GetLine2DebouncerTime_us(&fLine2DebouncerTime_us))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line2 Debouncer time"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			SetFloating("LineDebouncerTime", fLine2DebouncerTime_us);
		}

		if(true)
		{
			SetEnumeration("LineSelector", "Out1");

			EDeviceBaslerTOFGigELineSource eLineSource = EDeviceBaslerTOFGigELineSource_Count;

			if(GetOutputLine1Source(&eLineSource))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Out1 source"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			switch(eLineSource)
			{
			case EDeviceBaslerTOFGigELineSource_ExposureActive:
				SetEnumeration("LineSource", "ExposureActive");
				break;
			case EDeviceBaslerTOFGigELineSource_UserOutput:
				SetEnumeration("LineSource", "UserOutput");
				break;
			case EDeviceBaslerTOFGigELineSource_AcquisitionTriggerWait:
				SetEnumeration("LineSource", "AcquisitionTriggerWait");
				break;
			}

			bool bOutputLine1Inverter = false;

			if(GetOutputLine1Inverter(&bOutputLine1Inverter))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Out1 Inverter"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			SetBoolean("LineInverter", bOutputLine1Inverter);

			float fOutputLine1OutputPulseMinWidth = 0.0f;

			if(GetOutputLine1OutputPulseMinWidth(&fOutputLine1OutputPulseMinWidth))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Out1 Pulse width"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			SetFloating("MinOutPulseWidth", fOutputLine1OutputPulseMinWidth);
		}

		if(true)
		{
			SetEnumeration("LineSelector", "Out2");

			EDeviceBaslerTOFGigELineSource eLineSource = EDeviceBaslerTOFGigELineSource_Count;

			if(GetOutputLine2Source(&eLineSource))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Out2 Source"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			switch(eLineSource)
			{
			case EDeviceBaslerTOFGigELineSource_ExposureActive:
				SetEnumeration("LineSource", "ExposureActive");
				break;
			case EDeviceBaslerTOFGigELineSource_UserOutput:
				SetEnumeration("LineSource", "UserOutput");
				break;
			case EDeviceBaslerTOFGigELineSource_AcquisitionTriggerWait:
				SetEnumeration("LineSource", "AcquisitionTriggerWait");
				break;
			}

			bool bOutputLine2Inverter = false;

			if(GetOutputLine2Inverter(&bOutputLine2Inverter))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Out2 Inverter"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			SetBoolean("LineInverter", bOutputLine2Inverter);

			float fOutputLine2OutputPulseMinWidth = 0.0f;

			if(GetOutputLine2OutputPulseMinWidth(&fOutputLine2OutputPulseMinWidth))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Out2 Pulse width"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			SetFloating("MinOutPulseWidth", fOutputLine2OutputPulseMinWidth);
		}

		EDeviceBaslerTOFGigELineSelector eLineSelector = EDeviceBaslerTOFGigELineSelector_Count;

		if(GetLineSelector(&eLineSelector))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line selector"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}
		
		switch(eLineSelector)
		{
		case EDeviceBaslerTOFGigELineSelector_Line1:
			SetEnumeration("LineSelector", "Line1");
			break;
		case EDeviceBaslerTOFGigELineSelector_Line2:
			SetEnumeration("LineSelector", "Line2");
			break;
		case EDeviceBaslerTOFGigELineSelector_OutputLine1:
			SetEnumeration("LineSelector", "Out1");
			break;
		case EDeviceBaslerTOFGigELineSelector_OutputLine2:
			SetEnumeration("LineSelector", "Out2");
			break;
		default:
			break;
		}

		if(true)
		{
			SetEnumeration("UserOutputSelector", "UserOutput1");

			bool bUserOutput1Value = false;

			if(GetUserOutput1Value(&bUserOutput1Value))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("User output1 value"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			SetBoolean("UserOutputValue", bUserOutput1Value);
		}

		if(true)
		{
			SetEnumeration("UserOutputSelector", "UserOutput2");

			bool bUserOutput2Value = false;

			if(GetUserOutput2Value(&bUserOutput2Value))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Uwer output2 value"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			SetBoolean("UserOutputValue", bUserOutput2Value);
		}
		
		EDeviceBaslerTOFGigEUserOutputSelector eUserOutputSelector = EDeviceBaslerTOFGigEUserOutputSelector_Count;

		if(GetUserOutputSelector(&eUserOutputSelector))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("User output selector"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		switch(eUserOutputSelector)
		{
		case EDeviceBaslerTOFGigEUserOutputSelector_UserOutput1:
			SetEnumeration("UserOutputSelector", "UserOutput1");
			break;
		case EDeviceBaslerTOFGigEUserOutputSelector_UserOutput2:
			SetEnumeration("UserOutputSelector", "UserOutput2");
			break;
		}

		float fUserOutputValueAll = 0.0f;

		if(GetUserOutputValueAll(&fUserOutputValueAll))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("User output value all"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		SetFloating("UserOutputValueAll", fUserOutputValueAll);

		try
		{
			int nWidth = 0;
			int nHeight = 0;
			int nMaxValue = (1 << 16) - 1;

			if(GetCanvasWidth(&nWidth))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Canvas Width"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(GetCanvasHeight(&nHeight))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Canvas Height"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(!nWidth)
			{
				GenApi::CIntegerPtr ptrWidthMax = m_pCamera->GetParameter("WidthMax");

				bool bValue = true;

				try
				{
					nWidth = ptrWidthMax->GetValue();		
					bValue = false;
				}
				catch(...){}
				
				if(bValue)
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Width"));
					eReturn = EDeviceInitializeResult_ReadOnDeviceError;
					break;
				}

				if(SetCanvasWidth(nWidth))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Canvas Width"));
					eReturn = EDeviceInitializeResult_WriteToDatabaseError;
					break;
				}
			}

			if(!nHeight)
			{
				GenApi::CIntegerPtr ptrHeightMax = m_pCamera->GetParameter("HeightMax");

				bool bValue = true;

				try
				{
					nHeight = ptrHeightMax->GetValue();
					bValue = false;
				}
				catch(...) { }

				if(bValue)
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Height"));
					eReturn = EDeviceInitializeResult_ReadOnDeviceError;
					break;
				}

				if(SetCanvasHeight(nHeight))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Canvas Height"));
					eReturn = EDeviceInitializeResult_WriteToDatabaseError;
					break;
				}
			}

			if(!SetInteger("Width", nWidth))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Width"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}

			if(!SetInteger("Height", nHeight))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Height"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}
			
			CMultipleVariable mv;
			mv.AddValue(0);
			mv.AddValue(0);
			mv.AddValue(0);

			this->InitBuffer(nWidth, nHeight, nMaxValue, CRavidImage::MakeValueFormat(1, 16));
			this->ConnectImage();
			
			m_p3DInfo = new CRavidImage(nWidth, nHeight, mv, CRavidImage::MakeValueFormat(3, 32, Ravid::Algorithms::CRavidImage::EValueType_FloatingPoint), nWidth * 12);
			m_pConfidenceMap = new CRavidImage(nWidth, nHeight, nMaxValue, CRavidImage::MakeValueFormat(1, 16));

			GenApi::CEnumerationPtr ptrComponentSelector = m_pCamera->GetParameter("ComponentSelector");
			GenApi::CBooleanPtr ptrComponentEnable = m_pCamera->GetParameter("ComponentEnable");
			GenApi::CEnumerationPtr ptrPixelFormat = m_pCamera->GetParameter("PixelFormat");

			// Enable range data
			ptrComponentSelector->FromString("Range");
			ptrComponentEnable->SetValue(true);
			// Range information can be sent either as a 16-bit grey value image or as 3D coordinates (point cloud). For this sample, we want to acquire 3D coordinates.
			// Note: To change the format of an image component, the Component Selector must first be set to the component
			// you want to configure (see above).
			// To use 16-bit integer depth information, choose "Coord3D_C16" instead of "Coord3D_ABC32f".
			ptrPixelFormat->FromString("Coord3D_ABC32f");

			ptrComponentSelector->FromString("Intensity");
			ptrComponentEnable->SetValue(true);

			ptrComponentSelector->FromString("Confidence");
			ptrComponentEnable->SetValue(true);

			m_pCamera->SetBufferAllocator(new CustomAllocator(), true);

			m_pCamera->PrepareAcquisition(3);

			for(size_t i = 0; i < 3; ++i)
				m_pCamera->QueueBuffer(i);

			m_pCamera->StartAcquisition();

			m_nCanvasX = nWidth;
			m_nCanvasY = nHeight;

			bError = false;
		}
		catch(...)
		{
		}

		if(bError)
			break;

		if(GetDrawPoint(&m_bDrawPoint))
			break;
		
		if(!GetImageInfo() || !m_p3DInfo || !m_pConfidenceMap)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtocreateimagebuffer);
			eReturn = EDeviceInitializeResult_NotCreateImagebuffer;
			break;
		}

		EDeviceBaslerTOFGigESelectImage eSelectImage = EDeviceBaslerTOFGigESelectImage_Intensity;

		if(GetDrawImage(&eSelectImage))
			break;

		CRavidImageView* pCurView = GetImageView();
		if(pCurView)
		{
			CRavidImage* pImg = nullptr;

			switch(eSelectImage)
			{
			case EDeviceBaslerTOFGigESelectImage_Intensity:
				pImg = GetImageInfo();
				break;
			case EDeviceBaslerTOFGigESelectImage_Confidence:
				pImg = m_pConfidenceMap;
				break;
			case EDeviceBaslerTOFGigESelectImage_3DData:
				pImg = m_p3DInfo;
				break;
			}

			pCurView->SetImageInfo(pImg);
		}

		strStatus = CMultiLanguageManager::GetString(ELanguageParameter_Initialized);

		strMessage = CMultiLanguageManager::GetString(ELanguageParameter_TheDevicehasbeensuccessfullyinitialized);

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

EDeviceTerminateResult CDeviceBaslerTOFGigE::Terminate()
{
	EDeviceTerminateResult eReturn = EDeviceTerminateResult_UnknownError;

	CString strMessage;

	SetStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("Basler"));

		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);

		return EDeviceTerminateResult_NotFoundApiError;
	}

	do
	{
		if(m_pCamera)
		{
			if(IsLive() || !IsGrabAvailable())
				Stop();
			
			try
			{
				m_pCamera->IssueAcquisitionStopCommand();
			}
			catch (...){}			

			try
			{
				m_pCamera->FinishAcquisition();
			}
			catch(...) { }

			try
			{
				if(m_pCamera->IsOpen())
					m_pCamera->Close();
			}
			catch(...) { }

			try
			{
				if(GenTLConsumerImplHelper::CToFCamera::IsProducerInitialized())
					GenTLConsumerImplHelper::CToFCamera::TerminateProducer();
			}
			catch(...){}
		}

		m_bIsLive = false;
		m_bIsInitialized = false;
		m_bIsGrabAvailable = true;

		strMessage = CMultiLanguageManager::GetString(ELanguageParameter_TheDevicehasbeensuccessfullyterminated);

		eReturn = EDeviceTerminateResult_OK;
				
		CEventHandlerManager::BroadcastOnDeviceTerminated(this);
	}
	while(false);
	
	CRavidImageView* pCurView = GetImageView();

	if(m_p3DInfo)
	{
		if(pCurView)
		{
			if(pCurView->GetImageInfo() == m_p3DInfo && pCurView->GetSafeHwnd())
				pCurView->SetImageInfo(nullptr);
		}

		m_p3DInfo->Clear();

		delete m_p3DInfo;
		m_p3DInfo = nullptr;
	}

	if(m_pConfidenceMap)
	{
		if(pCurView)
		{
			if(pCurView->GetImageInfo() == m_pConfidenceMap && pCurView->GetSafeHwnd())
				pCurView->SetImageInfo(nullptr);
		}

		m_pConfidenceMap->Clear();

		delete m_pConfidenceMap;
		m_pConfidenceMap = nullptr;
	}

	this->ClearBuffer();

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceBaslerTOFGigE::LoadSettings()
{
	bool bReturn = false;

	m_vctParameterFieldConfigurations.clear();

	do
	{
		CString strTime;
		strTime.Format(_T("%d"), INT_MAX);

		AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_DeviceID, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_DeviceID], _T("0"), EParameterFieldType_Edit);

		AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_DrawPoint, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_DrawPoint], _T("1"), EParameterFieldType_Check);
		AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_DrawImage, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_DrawImage], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerTOFSelectImage, EDeviceBaslerTOFGigESelectImage_Count));

		AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_DeviceControl, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_DeviceControl], _T("1"), EParameterFieldType_None);
		{
			AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_DeviceChannel, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_DeviceChannel], _T("3"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerTOFDeviceChannel, EDeviceBaslerTOFGigEDeviceChannel_Count), _T("0 ~ 3"), 1);
			AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_CalibrationRangeOffset, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_CalibrationRangeOffset], _T("0"), EParameterFieldType_Edit, nullptr, _T("-250 ~ 250"), 1);

			AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_RectificationControl, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_RectificationControl], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
			{
				AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_Rectification, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_Rectification], _T("1"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerTOFSwitch, EDeviceBaslerTOFGigESwitch_Count), nullptr, 2);
			}
			
			AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_DeviceTemperatureSelector, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_DeviceTemperatureSelector], _T("1"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerTOFTemperatureSelector, EDeviceBaslerTOFGigETemperatureSelector_Count), nullptr, 1);
		}
		
		AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_ImageControl, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_ImageControl], _T("1"), EParameterFieldType_None);
		{
			AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_Binning, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_Binning], _T("1"), EParameterFieldType_Check, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_CanvasWidth, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_CanvasWidth], _T("0"), EParameterFieldType_Edit, nullptr, _T("Integer"), 1);
			AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_CanvasHeight, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_CanvasHeight], _T("0"), EParameterFieldType_Edit, nullptr, _T("Integer"), 1);
			AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_OffsetX, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_OffsetX], _T("0"), EParameterFieldType_Edit, nullptr, _T("Integer"), 1);
			AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_OffsetY, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_OffsetY], _T("0"), EParameterFieldType_Edit, nullptr, _T("Integer"), 1);
			AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_MinimumDepth_mm, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_MinimumDepth_mm], _T("0"), EParameterFieldType_Edit, nullptr, _T("0 ~ 9572"), 1);
			AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_MaximumDepth_mm, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_MaximumDepth_mm], _T("0"), EParameterFieldType_Edit, nullptr, _T("0 ~ 13320"), 1);
			AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_ComponentSelector, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_ComponentSelector], _T("1"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerTOFComponentSelector, EDeviceBaslerTOFGigEComponentSelector_Count), nullptr, 1);
		}
		
		AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_AcquisitionControl, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_AcquisitionControl], _T("1"), EParameterFieldType_None);
		{
			AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_LEDDisable, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_LEDDisable], _T("1"), EParameterFieldType_Check, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_ProcessingMode, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_ProcessingMode], _T("1"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerTOFProcessingMode, EDeviceBaslerTOFGigEProcessingMode_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_AcquisitionFrameRate, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_AcquisitionFrameRate], _T("10.0"), EParameterFieldType_Edit, nullptr, _T("1.0 ~ 12.0"), 1);
			AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_ExposureAuto, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_ExposureAuto], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerTOFExposureAuto, EDeviceBaslerTOFGigEExposureAuto_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_Agility, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_Agility], _T("0.4"), EParameterFieldType_Edit, nullptr, _T("0.1 ~ 1.0"), 1);
			AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_Delay, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_Delay], _T("2"), EParameterFieldType_Edit, nullptr, _T("1 ~ 10"), 1);
			AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_ExposureTimeSelector, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_ExposureTimeSelector], _T("0"), EParameterFieldType_Edit, nullptr, _T("Standard : 0 / HDR : 0, 1"), 1);
			AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_ExposureTime_us, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_ExposureTime_us], _T("20000"), EParameterFieldType_Edit, nullptr, _T("Standard : 300 ~ 25000 / HDR : 300 ~ 20000"), 1);
			AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_TriggerMode, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_TriggerMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerTOFSwitch, EDeviceBaslerTOFGigESwitch_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_TriggerSource, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_TriggerSource], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerTOFTriggerSource, EDeviceBaslerTOFGigETriggerSource_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_TriggerDelay_us, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_TriggerDelay_us], _T("0.0"), EParameterFieldType_Edit, nullptr, _T("0.0 ~  ~ 10000000.0"), 1);

			AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_SyncTimerControl, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_SyncTimerControl], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
			{
				AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_TimestampLatch, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_TimestampLatch], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_TimestampLow_ns, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_TimestampLow_ns], _T("0"), EParameterFieldType_Static, nullptr, nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_TimestampHigh_ns, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_TimestampHigh_ns], _T("0"), EParameterFieldType_Static, nullptr, nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_StartTimeLow_us, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_StartTimeLow_us], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_StartTimeHigh_us, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_StartTimeHigh_us], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_TriggerRat_Hz, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_TriggerRat_Hz], _T("10.0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_Update, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_Update], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 2);
			}
		}

		AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_ImageQualityControl, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_ImageQualityControl], _T("1"), EParameterFieldType_None);
		{
			AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_ConfidenceThreshold, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_ConfidenceThreshold], _T("2048"), EParameterFieldType_Edit, nullptr, _T("0 ~ 65536"), 1);
			AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_SpatialFilter, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_SpatialFilter], _T("1"), EParameterFieldType_Check, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_TemporalFilter, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_TemporalFilter], _T("1"), EParameterFieldType_Check, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_Strength, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_Strength], _T("235"), EParameterFieldType_Edit, nullptr, _T("50 ~ 240"), 1);
			AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_OutlierTolerance, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_OutlierTolerance], _T("2048"), EParameterFieldType_Edit, nullptr, _T("0 ~ 65536"), 1);
			AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_RangeFilter, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_RangeFilter], _T("1"), EParameterFieldType_Check, nullptr, nullptr, 1);
		}

		AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_DigitalIOControl, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_DigitalIOControl], _T("0"), EParameterFieldType_None);
		{
			AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_LineSelector, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_LineSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerTOFLineSelector, EDeviceBaslerTOFGigELineSelector_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_InputLine1, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_InputLine1], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);
			{
				AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_Line1Inverter, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_Line1Inverter], _T("1"), EParameterFieldType_Check, nullptr, nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_Line1DebouncerTime_us, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_Line1DebouncerTime_us], _T("0.0"), EParameterFieldType_Edit, nullptr, _T("0 ~ 20000.0"), 2);
			}

			AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_InputLine2, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_InputLine2], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);
			{
				AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_Line2Inverter, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_Line2Inverter], _T("1"), EParameterFieldType_Check, nullptr, nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_Line2DebouncerTime_us, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_Line2DebouncerTime_us], _T("0.0"), EParameterFieldType_Edit, nullptr, _T("0 ~ 20000.0"), 2);
			}

			AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_OutputLine1, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_OutputLine1], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);
			{
				AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_OutputLine1Source, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_OutputLine1Source], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerTOFLineSource, EDeviceBaslerTOFGigELineSource_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_OutputLine1Inverter, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_OutputLine1Inverter], _T("1"), EParameterFieldType_Check, nullptr, nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_OutputLine1OutputPulseMinWidth, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_OutputLine1OutputPulseMinWidth], _T("0.0"), EParameterFieldType_Edit, nullptr, _T("0 ~ 20000.0"), 2);
			}

			AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_OutputLine2, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_OutputLine2], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);
			{
				AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_OutputLine2Source, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_OutputLine2Source], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerTOFLineSource, EDeviceBaslerTOFGigELineSource_Count), nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_OutputLine2Inverter, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_OutputLine2Inverter], _T("1"), EParameterFieldType_Check, nullptr, nullptr, 2);
				AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_OutputLine2OutputPulseMinWidth, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_OutputLine2OutputPulseMinWidth], _T("0.0"), EParameterFieldType_Edit, nullptr, _T("0 ~ 20000.0"), 2);
			}

			AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_UserOutputSelector, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_UserOutputSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszBaslerTOFUserOutputSelector, EDeviceBaslerTOFGigEUserOutputSelector_Count), nullptr, 1);
			
			AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_UserOutput1, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_UserOutput1], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);
			{
				AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_UserOutput1Value, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_UserOutput1Value], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 2);
			}

			AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_UserOutput2, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_UserOutput2], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);
			{
				AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_UserOutput2Value, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_UserOutput2Value], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 2);
			}

			AddParameterFieldConfigurations(EDeviceParameterBaslerTOFGigE_UserOutputValueAll, g_lpszParamBaslerTOFGigE[EDeviceParameterBaslerTOFGigE_UserOutputValueAll], _T("0.0"), EParameterFieldType_Edit, nullptr, _T("0 ~ 0x7FFF FFFF"), 1);
		}
		
		bReturn = true;
	}
	while(false);

	return bReturn & __super::LoadSettings();
}

EDeviceGrabResult CDeviceBaslerTOFGigE::Grab()
{
	EDeviceGrabResult eReturn = EDeviceGrabResult_UnknownError;

	CString strMessage;

	do
	{
		int nRequestGrabCount = 0;

		if(!IsInitialized())
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Ithasnotbeeninitializedyet);
			eReturn = EDeviceGrabResult_AlreadyGrabError;
			break;
		}

		if(IsLive() || !IsGrabAvailable())
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Ithasbeenlivingorgrabbing);
			eReturn = EDeviceGrabResult_AlreadyGrabError;
			break;
		}

		m_bIsGrabAvailable = false;
		
		m_pLiveThread = AfxBeginThread(CDeviceBaslerTOFGigE::CallbackFunction, this, 0, 0U, CREATE_SUSPENDED);

		if(!m_pLiveThread)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtocreatethread);
			eReturn = EDeviceGrabResult_CreateThreadError;
			break;
		}

		m_pLiveThread->ResumeThread();

		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("Grab"));
		
		eReturn = EDeviceGrabResult_OK;
	}
	while(false);
	
	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceLiveResult CDeviceBaslerTOFGigE::Live()
{
	EDeviceLiveResult eReturn = EDeviceLiveResult_UnknownError;

	CString strMessage;

	do
	{
		if(!IsInitialized())
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Ithasnotbeeninitializedyet);
			eReturn = EDeviceLiveResult_NotInitializedError;
			break;
		}

		if(IsLive() || !IsGrabAvailable())
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Ithasbeenlivingorgrabbing);
			eReturn = EDeviceLiveResult_AlreadyGrabError;
			break;
		}

		m_bIsLive = true;

		m_pLiveThread = AfxBeginThread(CDeviceBaslerTOFGigE::CallbackFunction, this, 0, 0U, CREATE_SUSPENDED);

		if(!m_pLiveThread)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtocreatethread);
			eReturn = EDeviceLiveResult_CreateThreadError;
			break;
		}
		
		m_pLiveThread->ResumeThread();

		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("Live"));

		eReturn = EDeviceLiveResult_OK;
	}
	while(false);
	
	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceStopResult CDeviceBaslerTOFGigE::Stop()
{
	EDeviceStopResult eReturn = EDeviceStopResult_UnknownError;

	CString strMessage;

	do 
	{
		if(!IsInitialized())
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Ithasnotbeeninitializedyet);
			eReturn = EDeviceStopResult_NotInitializedError;
			break;
		}
		if(!IsLive() && IsGrabAvailable())
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Thedevicewasalreadystop);
			eReturn = EDeviceStopResult_AlreadyStopError;
			break;
		}

		m_bIsGrabAvailable = true;
		m_bIsLive = false;

		if(m_pLiveThread)
		{
			try
			{
				m_pCamera->IssueAcquisitionStopCommand();
			}
			catch(...)
			{
			}

			if(WaitForSingleObject(m_pLiveThread->m_hThread, 5000) == WAIT_TIMEOUT)
			{
			}
		}		

		m_pLiveThread = nullptr;

		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("Stop"));

		eReturn = EDeviceStopResult_OK;
	} 
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceTriggerResult CDeviceBaslerTOFGigE::Trigger()
{
	EDeviceTriggerResult eReturn = EDeviceTriggerResult_UnknownError;

	CString strMessage;

	do 
	{
		if(!IsInitialized())
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Ithasnotbeeninitializedyet);
			eReturn = EDeviceTriggerResult_NotInitializedError;
			break;
		}
		
		bool bFlag = false;

		try
		{
			GenApi::CCommandPtr ptrTrigger = m_pCamera->GetParameter("TriggerSoftware");
			ptrTrigger->Execute();

			bFlag = true;
		}
		catch(...)
		{

		}
		
		if(!bFlag)
			break;

		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("Trigger"));

		eReturn = EDeviceTriggerResult_OK;
	} 
	while (false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetDrawPoint(bool * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerTOFGigE_DrawPoint));

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetDrawPoint(bool bParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_DrawPoint;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		CString strSave;
		strSave.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		m_bDrawPoint = bParam;

		DrawPixelInfo(9);

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], g_lpszBaslerTOFSwitch[nPreValue], g_lpszBaslerTOFSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetDrawImage(EDeviceBaslerTOFGigESelectImage * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterBaslerTOFGigE_DrawImage));

		if(nData < 0 || nData >= (int)EDeviceBaslerTOFGigESelectImage_Count)
		{
			eReturn = EBaslerTOFGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaslerTOFGigESelectImage)nData;

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetDrawImage(EDeviceBaslerTOFGigESelectImage eParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_DrawImage;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDeviceBaslerTOFGigESelectImage)0 || eParam >= EDeviceBaslerTOFGigESelectImage_Count)
		{
			eReturn = EBaslerTOFSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EBaslerTOFSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], g_lpszBaslerTOFSelectImage[nPreValue], g_lpszBaslerTOFSelectImage[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetDeviceChannel(EDeviceBaslerTOFGigEDeviceChannel * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterBaslerTOFGigE_DeviceChannel));

		if(nData < 0 || nData >= (int)EDeviceBaslerTOFGigEDeviceChannel_Count)
		{
			eReturn = EBaslerTOFGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaslerTOFGigEDeviceChannel)nData;

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetDeviceChannel(EDeviceBaslerTOFGigEDeviceChannel eParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_DeviceChannel;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDeviceBaslerTOFGigEDeviceChannel)0 || eParam >= EDeviceBaslerTOFGigEDeviceChannel_Count)
		{
			eReturn = EBaslerTOFSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			GenApi::CIntegerPtr ptrDeviceChannel = m_pCamera->GetParameter("DeviceChannel");

			if(!ptrDeviceChannel)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				ptrDeviceChannel->SetValue(eParam);
				bError = false;
			}
			catch(...){	}
			
			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], g_lpszBaslerTOFDeviceChannel[nPreValue], g_lpszBaslerTOFDeviceChannel[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetCalibrationRangeOffset(int * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerTOFGigE_CalibrationRangeOffset));

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetCalibrationRangeOffset(int nParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_CalibrationRangeOffset;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			GenApi::CIntegerPtr ptrDeviceCalibOffset = m_pCamera->GetParameter("DeviceCalibOffset"); 

			if(!ptrDeviceCalibOffset)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				ptrDeviceCalibOffset->SetValue(nParam);
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strOffset;
		strOffset.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strOffset))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetRectification(EDeviceBaslerTOFGigESwitch * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterBaslerTOFGigE_Rectification));

		if(nData < 0 || nData >= (int)EDeviceBaslerTOFGigESwitch_Count)
		{
			eReturn = EBaslerTOFGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaslerTOFGigESwitch)nData;

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetRectification(EDeviceBaslerTOFGigESwitch eParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_Rectification;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDeviceBaslerTOFGigESwitch)0 || eParam >= EDeviceBaslerTOFGigESwitch_Count)
		{
			eReturn = EBaslerTOFSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			GenApi::CEnumerationPtr ptrRectification = m_pCamera->GetParameter("Rectification");

			if(!ptrRectification)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				switch(eParam)
				{
				case EDeviceBaslerTOFGigESwitch_Off:
					ptrRectification->FromString("Off");
					break;
				case EDeviceBaslerTOFGigESwitch_On:
					ptrRectification->FromString("On");
					break;
				}

				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}			
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], g_lpszBaslerTOFSwitch[nPreValue], g_lpszBaslerTOFSwitch[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetDeviceTemperatureSelector(EDeviceBaslerTOFGigETemperatureSelector * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterBaslerTOFGigE_DeviceTemperatureSelector));

		if(nData < 0 || nData >= (int)EDeviceBaslerTOFGigETemperatureSelector_Count)
		{
			eReturn = EBaslerTOFGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaslerTOFGigETemperatureSelector)nData;

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetDeviceTemperatureSelector(EDeviceBaslerTOFGigETemperatureSelector eParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_DeviceTemperatureSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDeviceBaslerTOFGigETemperatureSelector)0 || eParam >= EDeviceBaslerTOFGigETemperatureSelector_Count)
		{
			eReturn = EBaslerTOFSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			GenApi::CEnumerationPtr ptrDeviceTemperatureSelector = m_pCamera->GetParameter("DeviceTemperatureSelector");

			if(!ptrDeviceTemperatureSelector)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				switch(eParam)
				{
				case EDeviceBaslerTOFGigETemperatureSelector_SensorBoard:
					ptrDeviceTemperatureSelector->FromString("SensorBoard");
					break;
				case EDeviceBaslerTOFGigETemperatureSelector_LEDBoard:
					ptrDeviceTemperatureSelector->FromString("LEDBoard");
					break;
				}

				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], g_lpszBaslerTOFTemperatureSelector[nPreValue], g_lpszBaslerTOFTemperatureSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetBinning(bool * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerTOFGigE_Binning));
		
		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetBinning(bool bParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_Binning;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			GenApi::CBooleanPtr ptrBinning = m_pCamera->GetParameter("Binning");

			if(!ptrBinning)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				ptrBinning->SetValue(bParam);
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], g_lpszBaslerTOFSwitch[nPreValue], g_lpszBaslerTOFSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetCanvasWidth(int * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerTOFGigE_CanvasWidth));

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetCanvasWidth(int nParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_CanvasWidth;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EBaslerTOFSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetCanvasHeight(int * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerTOFGigE_CanvasHeight));

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetCanvasHeight(int nParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_CanvasHeight;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EBaslerTOFSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetOffsetX(int * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerTOFGigE_OffsetX));

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetOffsetX(int nParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_OffsetX;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			GenApi::CIntegerPtr ptrOffsetX = m_pCamera->GetParameter("OffsetX");

			if(!ptrOffsetX)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				ptrOffsetX->SetValue(nParam);
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetOffsetY(int * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerTOFGigE_OffsetY));

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetOffsetY(int nParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_OffsetY;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			GenApi::CIntegerPtr ptrOffsetY = m_pCamera->GetParameter("OffsetY");

			if(!ptrOffsetY)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				ptrOffsetY->SetValue(nParam);
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetMinimumDepth_mm(int * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerTOFGigE_MinimumDepth_mm));

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetMinimumDepth_mm(int nParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_MinimumDepth_mm;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			GenApi::CIntegerPtr ptrDepthMin = m_pCamera->GetParameter("DepthMin");

			if(!ptrDepthMin)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				ptrDepthMin->SetValue(nParam);
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetMaximumDepth_mm(int * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerTOFGigE_MaximumDepth_mm));

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetMaximumDepth_mm(int nParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_MaximumDepth_mm;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			GenApi::CIntegerPtr ptrDepthMax = m_pCamera->GetParameter("DepthMax");

			if(!ptrDepthMax)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				ptrDepthMax->SetValue(nParam);
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetComponentSelector(EDeviceBaslerTOFGigEComponentSelector * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterBaslerTOFGigE_ComponentSelector));

		if(nData < 0 || nData >= (int)EDeviceBaslerTOFGigEComponentSelector_Count)
		{
			eReturn = EBaslerTOFGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaslerTOFGigEComponentSelector)nData;

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetComponentSelector(EDeviceBaslerTOFGigEComponentSelector eParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_ComponentSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDeviceBaslerTOFGigEComponentSelector)0 || eParam >= EDeviceBaslerTOFGigEComponentSelector_Count)
		{
			eReturn = EBaslerTOFSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			GenApi::CEnumerationPtr ptrComponentSelector = m_pCamera->GetParameter("ComponentSelector");

			if(!ptrComponentSelector)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				switch(eParam)
				{
				case EDeviceBaslerTOFGigEComponentSelector_Range:
					ptrComponentSelector->FromString("Range");
					break;
				case EDeviceBaslerTOFGigEComponentSelector_Intensity:
					ptrComponentSelector->FromString("Intensity");
					break;
				case EDeviceBaslerTOFGigEComponentSelector_Confidence:
					ptrComponentSelector->FromString("Confidence");
					break;
				}

				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], g_lpszBaslerTOFComponentSelector[nPreValue], g_lpszBaslerTOFComponentSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetLEDDisable(bool * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerTOFGigE_LEDDisable));

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetLEDDisable(bool bParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_LEDDisable;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			GenApi::CBooleanPtr ptrLedDisable = m_pCamera->GetParameter("LedDisable");

			if(!ptrLedDisable)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				ptrLedDisable->SetValue(bParam);
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], g_lpszBaslerTOFSwitch[nPreValue], g_lpszBaslerTOFSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetProcessingMode(EDeviceBaslerTOFGigEProcessingMode * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterBaslerTOFGigE_ProcessingMode));

		if(nData < 0 || nData >= (int)EDeviceBaslerTOFGigEProcessingMode_Count)
		{
			eReturn = EBaslerTOFGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaslerTOFGigEProcessingMode)nData;

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetProcessingMode(EDeviceBaslerTOFGigEProcessingMode eParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_ProcessingMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDeviceBaslerTOFGigEProcessingMode)0 || eParam >= EDeviceBaslerTOFGigEProcessingMode_Count)
		{
			eReturn = EBaslerTOFSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			GenApi::CEnumerationPtr ptrProcessingMode = m_pCamera->GetParameter("ProcessingMode");

			if(!ptrProcessingMode)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				switch(eParam)
				{
				case EDeviceBaslerTOFGigEProcessingMode_Standard:
					ptrProcessingMode->FromString("Standard");
					break;
				case EDeviceBaslerTOFGigEProcessingMode_HDR:
					ptrProcessingMode->FromString("Hdr");
					break;
				}

				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}			
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], g_lpszBaslerTOFProcessingMode[nPreValue], g_lpszBaslerTOFProcessingMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetAcquisitionFrameRate(float * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterBaslerTOFGigE_AcquisitionFrameRate));

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetAcquisitionFrameRate(float fParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_AcquisitionFrameRate;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			GenApi::CFloatPtr ptrAcquisitionFrameRate = m_pCamera->GetParameter("AcquisitionFrameRate");

			if(!ptrAcquisitionFrameRate)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				ptrAcquisitionFrameRate->SetValue(fParam);
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%.6f"), fParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetExposureAuto(EDeviceBaslerTOFGigEExposureAuto* pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterBaslerTOFGigE_ExposureAuto));

		if(nData < 0 || nData >= (int)EDeviceBaslerTOFGigEExposureAuto_Count)
		{
			eReturn = EBaslerTOFGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaslerTOFGigEExposureAuto)nData;

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetExposureAuto(EDeviceBaslerTOFGigEExposureAuto eParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_ExposureAuto;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDeviceBaslerTOFGigEExposureAuto)0 || eParam >= EDeviceBaslerTOFGigEExposureAuto_Count)
		{
			eReturn = EBaslerTOFSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			GenApi::CEnumerationPtr ptrExposureAuto = m_pCamera->GetParameter("ExposureAuto");

			if(!ptrExposureAuto)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				switch(eParam)
				{
				case EDeviceBaslerTOFGigEExposureAuto_Off:
					ptrExposureAuto->FromString("Off");
					break;
				case EDeviceBaslerTOFGigEExposureAuto_Continuous:
					ptrExposureAuto->FromString("Continuous");
					break;
				}

				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], g_lpszBaslerTOFExposureAuto[nPreValue], g_lpszBaslerTOFExposureAuto[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetAgility(float * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterBaslerTOFGigE_Agility));

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetAgility(float fParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_Agility;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			GenApi::CFloatPtr ptrAgility = m_pCamera->GetParameter("Agility"); 

			if(!ptrAgility)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				ptrAgility->SetValue(fParam);
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%.6f"), fParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetDelay(float * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterBaslerTOFGigE_Delay));

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetDelay(float fParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_Delay;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			GenApi::CIntegerPtr ptrDelay = m_pCamera->GetParameter("Delay");

			if(!ptrDelay)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				ptrDelay->SetValue(fParam);
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%.6f"), fParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetExposureTimeSelector(int * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerTOFGigE_ExposureTimeSelector));

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetExposureTimeSelector(int nParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_ExposureTimeSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			GenApi::CIntegerPtr ptrExposureTimeSelector = m_pCamera->GetParameter("ExposureTimeSelector");

			if(!ptrExposureTimeSelector)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				ptrExposureTimeSelector->SetValue(nParam);
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetExposureTime_us(int * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerTOFGigE_ExposureTime_us));

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetExposureTime_us(int nParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_ExposureTime_us;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			GenApi::CIntegerPtr ptrExposureTimeSelector = m_pCamera->GetParameter("ExposureTimeSelector"); 

			if(!ptrExposureTimeSelector)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				ptrExposureTimeSelector->SetValue(nParam);
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}

			GenApi::CFloatPtr ptrExposureTime = m_pCamera->GetParameter("ExposureTime"); 

			if(!ptrExposureTime)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			try
			{
				ptrExposureTime->SetValue(nParam);

				bError = false;
			}
			catch(...)
			{
			}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetTriggerMode(EDeviceBaslerTOFGigESwitch * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterBaslerTOFGigE_TriggerMode));

		if(nData < 0 || nData >= (int)EDeviceBaslerTOFGigESwitch_Count)
		{
			eReturn = EBaslerTOFGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaslerTOFGigESwitch)nData;

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetTriggerMode(EDeviceBaslerTOFGigESwitch eParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_TriggerMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDeviceBaslerTOFGigESwitch)0 || eParam >= EDeviceBaslerTOFGigESwitch_Count)
		{
			eReturn = EBaslerTOFSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			GenApi::CEnumerationPtr ptrTriggerMode = m_pCamera->GetParameter("TriggerMode");

			if(!ptrTriggerMode)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				switch(eParam)
				{
				case EDeviceBaslerTOFGigESwitch_Off:
					ptrTriggerMode->FromString("Off");
					break;
				case EDeviceBaslerTOFGigESwitch_On:
					ptrTriggerMode->FromString("On");
					break;
				}

				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}		
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], g_lpszBaslerTOFSwitch[nPreValue], g_lpszBaslerTOFSwitch[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetTriggerSource(EDeviceBaslerTOFGigETriggerSource * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterBaslerTOFGigE_TriggerSource));

		if(nData < 0 || nData >= (int)EDeviceBaslerTOFGigETriggerSource_Count)
		{
			eReturn = EBaslerTOFGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaslerTOFGigETriggerSource)nData;

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetTriggerSource(EDeviceBaslerTOFGigETriggerSource eParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_TriggerSource;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDeviceBaslerTOFGigETriggerSource)0 || eParam >= EDeviceBaslerTOFGigETriggerSource_Count)
		{
			eReturn = EBaslerTOFSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			GenApi::CEnumerationPtr ptrTriggerSource = m_pCamera->GetParameter("TriggerSource"); 

			if(!ptrTriggerSource)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				switch(eParam)
				{
				case EDeviceBaslerTOFGigETriggerSource_Software:
					ptrTriggerSource->FromString("Software");
					break;
				case EDeviceBaslerTOFGigETriggerSource_Line1:
					ptrTriggerSource->FromString("Line1");
					break;
				case EDeviceBaslerTOFGigETriggerSource_Line2:
					ptrTriggerSource->FromString("Line2");
					break;
				case EDeviceBaslerTOFGigETriggerSource_SyncTimer:
					ptrTriggerSource->FromString("SyncTimer");
					break;
				}

				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], g_lpszBaslerTOFTriggerSource[nPreValue], g_lpszBaslerTOFTriggerSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetTriggerDelay_us(float * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterBaslerTOFGigE_TriggerDelay_us));

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetTriggerDelay_us(float fParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_TriggerDelay_us;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			GenApi::CFloatPtr ptrTriggerDelay = m_pCamera->GetParameter("TriggerDelay"); 

			if(!ptrTriggerDelay)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				ptrTriggerDelay->SetValue(fParam);
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%.6f"), fParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetTimestampLatch(bool * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerTOFGigE_TimestampLatch));

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetTimestampLatch(bool bParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_TimestampLatch;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			GenApi::CCommandPtr ptrTimestampLatch = m_pCamera->GetParameter("TimestampLatch");

			if(!ptrTimestampLatch)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				ptrTimestampLatch->Execute();
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], g_lpszBaslerTOFSwitch[nPreValue], g_lpszBaslerTOFSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetTimestampLow_ns(int * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerTOFGigE_TimestampLow_ns));

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetTimestampLow_ns(int nParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_TimestampLow_ns;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			GenApi::CIntegerPtr ptrTimestampLow = m_pCamera->GetParameter("TimestampLow");

			if(!ptrTimestampLow)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				nParam = ptrTimestampLow->GetValue();
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetTimestampHigh_ns(int * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerTOFGigE_TimestampHigh_ns));

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetTimestampHigh_ns(int nParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_TimestampHigh_ns;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			GenApi::CIntegerPtr ptrTimestampLow = m_pCamera->GetParameter("TimestampHigh");

			if(!ptrTimestampLow)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				nParam = ptrTimestampLow->GetValue();
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetStartTimeLow_us(int * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerTOFGigE_StartTimeLow_us));

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetStartTimeLow_us(int nParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_StartTimeLow_us;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			GenApi::CIntegerPtr ptrSyncStartLow = m_pCamera->GetParameter("SyncStartLow");

			if(!ptrSyncStartLow)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				ptrSyncStartLow->SetValue(nParam);
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}

			SetTimestampLow_ns(0);
			SetTimestampHigh_ns(0);
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetStartTimeHigh_us(int * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerTOFGigE_StartTimeHigh_us));

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetStartTimeHigh_us(int nParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_StartTimeHigh_us;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			GenApi::CIntegerPtr ptrSyncStartHigh = m_pCamera->GetParameter("SyncStartHigh");

			if(!ptrSyncStartHigh)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				ptrSyncStartHigh->SetValue(nParam);
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}

			SetTimestampLow_ns(0);
			SetTimestampHigh_ns(0);
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetTriggerRat_Hz(float * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterBaslerTOFGigE_TriggerRat_Hz));

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetTriggerRat_Hz(float fParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_TriggerRat_Hz;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			GenApi::CFloatPtr ptrSyncRate = m_pCamera->GetParameter("SyncRate");

			if(!ptrSyncRate)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				ptrSyncRate->SetValue(fParam);
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%.6f"), fParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetUpdate(bool * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerTOFGigE_Update));

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetUpdate(bool bParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_Update;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			GenApi::CCommandPtr ptrSyncUpdate = m_pCamera->GetParameter("SyncUpdate");

			if(!ptrSyncUpdate)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				ptrSyncUpdate->Execute();
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], g_lpszBaslerTOFSwitch[nPreValue], g_lpszBaslerTOFSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetConfidenceThreshold(int * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerTOFGigE_ConfidenceThreshold));

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetConfidenceThreshold(int nParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_ConfidenceThreshold;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			GenApi::CIntegerPtr ptrConfidenceThreshold = m_pCamera->GetParameter("ConfidenceThreshold");

			if(!ptrConfidenceThreshold)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				ptrConfidenceThreshold->SetValue(nParam);
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetSpatialFilter(bool * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerTOFGigE_SpatialFilter));

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetSpatialFilter(bool bParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_SpatialFilter;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			GenApi::CBooleanPtr ptrFilterSpatial = m_pCamera->GetParameter("FilterSpatial");

			if(!ptrFilterSpatial)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				ptrFilterSpatial->SetValue(bParam);
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], g_lpszBaslerTOFSwitch[nPreValue], g_lpszBaslerTOFSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetTemporalFilter(bool * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerTOFGigE_TemporalFilter));

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetTemporalFilter(bool bParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_TemporalFilter;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			GenApi::CBooleanPtr ptrFilterTemporal = m_pCamera->GetParameter("FilterTemporal");

			if(!ptrFilterTemporal)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				ptrFilterTemporal->SetValue(bParam);
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], g_lpszBaslerTOFSwitch[nPreValue], g_lpszBaslerTOFSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetStrength(int * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerTOFGigE_Strength));

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetStrength(int nParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_Strength;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			GenApi::CIntegerPtr ptrFilterStrength = m_pCamera->GetParameter("FilterStrength");

			if(!ptrFilterStrength)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				ptrFilterStrength->SetValue(nParam);
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetOutlierTolerance(int * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerTOFGigE_OutlierTolerance));

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetOutlierTolerance(int nParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_OutlierTolerance;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			GenApi::CIntegerPtr ptrOutlierTolerance = m_pCamera->GetParameter("OutlierTolerance");

			if(!ptrOutlierTolerance)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				ptrOutlierTolerance->SetValue(nParam);
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetRangeFilter(bool * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerTOFGigE_RangeFilter));

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetRangeFilter(bool bParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_RangeFilter;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			GenApi::CBooleanPtr ptrFilterRange = m_pCamera->GetParameter("FilterRange");

			if(!ptrFilterRange)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				ptrFilterRange->SetValue(bParam);
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], g_lpszBaslerTOFSwitch[nPreValue], g_lpszBaslerTOFSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetLineSelector(EDeviceBaslerTOFGigELineSelector * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterBaslerTOFGigE_LineSelector));

		if(nData < 0 || nData >= (int)EDeviceBaslerTOFGigELineSelector_Count)
		{
			eReturn = EBaslerTOFGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaslerTOFGigELineSelector)nData;

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetLineSelector(EDeviceBaslerTOFGigELineSelector eParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_LineSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDeviceBaslerTOFGigELineSelector)0 || eParam >= EDeviceBaslerTOFGigELineSelector_Count)
		{
			eReturn = EBaslerTOFSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			GenApi::CEnumerationPtr ptrLineSelector = m_pCamera->GetParameter("LineSelector");

			if(!ptrLineSelector)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				switch(eParam)
				{
				case EDeviceBaslerTOFGigELineSelector_Line1:
					ptrLineSelector->FromString("Line1");
					break;
				case EDeviceBaslerTOFGigELineSelector_Line2:
					ptrLineSelector->FromString("Line2");
					break;
				case EDeviceBaslerTOFGigELineSelector_OutputLine1:
					ptrLineSelector->FromString("Out1");
					break;
				case EDeviceBaslerTOFGigELineSelector_OutputLine2:
					ptrLineSelector->FromString("Out2");
					break;
				default:
					break;
				}

				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], g_lpszBaslerTOFLineSelector[nPreValue], g_lpszBaslerTOFLineSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetLine1Inverter(bool * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerTOFGigE_Line1Inverter));

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetLine1Inverter(bool bParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_Line1Inverter;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			EDeviceBaslerTOFGigELineSelector eType = EDeviceBaslerTOFGigELineSelector_Count;

			if(GetLineSelector(&eType))
				break;

			if(eType != EDeviceBaslerTOFGigELineSelector_Line1)
				break;

			GenApi::CEnumerationPtr ptrLineSelector = m_pCamera->GetParameter("LineSelector");

			if(!ptrLineSelector)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				ptrLineSelector->FromString("Line1");
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}

			GenApi::CBooleanPtr ptrLineInverter = m_pCamera->GetParameter("LineInverter");

			if(!ptrLineInverter)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bError = true;

			try
			{
				ptrLineInverter->SetValue(bParam);
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], g_lpszBaslerTOFSwitch[nPreValue], g_lpszBaslerTOFSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetLine1DebouncerTime_us(float * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterBaslerTOFGigE_Line1DebouncerTime_us));

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetLine1DebouncerTime_us(float fParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_Line1DebouncerTime_us;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			EDeviceBaslerTOFGigELineSelector eType = EDeviceBaslerTOFGigELineSelector_Count;

			if(GetLineSelector(&eType))
				break;

			if(eType != EDeviceBaslerTOFGigELineSelector_Line1)
				break;

			GenApi::CEnumerationPtr ptrLineSelector = m_pCamera->GetParameter("LineSelector");

			if(!ptrLineSelector)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				ptrLineSelector->FromString("Line1");
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}

			GenApi::CFloatPtr ptrLineDebouncerTime = m_pCamera->GetParameter("LineDebouncerTime");

			if(!ptrLineDebouncerTime)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bError = true;

			try
			{
				ptrLineDebouncerTime->SetValue(fParam);
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%.6f"), fParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetLine2Inverter(bool * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerTOFGigE_Line2Inverter));

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetLine2Inverter(bool bParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_Line2Inverter;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			EDeviceBaslerTOFGigELineSelector eType = EDeviceBaslerTOFGigELineSelector_Count;

			if(GetLineSelector(&eType))
				break;

			if(eType != EDeviceBaslerTOFGigELineSelector_Line2)
				break;

			GenApi::CEnumerationPtr ptrLineSelector = m_pCamera->GetParameter("LineSelector");

			if(!ptrLineSelector)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				ptrLineSelector->FromString("Line2");
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}

			GenApi::CBooleanPtr ptrLineInverter = m_pCamera->GetParameter("LineInverter");

			if(!ptrLineInverter)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bError = true;

			try
			{
				ptrLineInverter->SetValue(bParam);
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], g_lpszBaslerTOFSwitch[nPreValue], g_lpszBaslerTOFSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetLine2DebouncerTime_us(float * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterBaslerTOFGigE_Line2DebouncerTime_us));

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetLine2DebouncerTime_us(float fParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_Line2DebouncerTime_us;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			EDeviceBaslerTOFGigELineSelector eType = EDeviceBaslerTOFGigELineSelector_Count;

			if(GetLineSelector(&eType))
				break;

			if(eType != EDeviceBaslerTOFGigELineSelector_Line2)
				break;

			GenApi::CEnumerationPtr ptrLineSelector = m_pCamera->GetParameter("LineSelector");

			if(!ptrLineSelector)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				ptrLineSelector->FromString("Line2");
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}

			GenApi::CFloatPtr ptrLineDebouncerTime = m_pCamera->GetParameter("LineDebouncerTime");

			if(!ptrLineDebouncerTime)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bError = true;

			try
			{
				ptrLineDebouncerTime->SetValue(fParam);
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%.6f"), fParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetOutputLine1Source(EDeviceBaslerTOFGigELineSource * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterBaslerTOFGigE_OutputLine1Source));

		if(nData < 0 || nData >= (int)EDeviceBaslerTOFGigELineSource_Count)
		{
			eReturn = EBaslerTOFGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaslerTOFGigELineSource)nData;

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetOutputLine1Source(EDeviceBaslerTOFGigELineSource eParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_OutputLine1Source;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDeviceBaslerTOFGigELineSource)0 || eParam >= EDeviceBaslerTOFGigELineSource_Count)
		{
			eReturn = EBaslerTOFSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			EDeviceBaslerTOFGigELineSelector eType = EDeviceBaslerTOFGigELineSelector_Count;

			if(GetLineSelector(&eType))
				break;

			if(eType != EDeviceBaslerTOFGigELineSelector_OutputLine1)
				break;

			GenApi::CEnumerationPtr ptrLineSelector = m_pCamera->GetParameter("LineSelector");

			if(!ptrLineSelector)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				ptrLineSelector->FromString("Out1");
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}

			GenApi::CEnumerationPtr ptrLineSource = m_pCamera->GetParameter("LineSource");

			if(!ptrLineSource)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bError = true;

			try
			{
				switch(eParam)
				{
				case EDeviceBaslerTOFGigELineSource_ExposureActive:
					ptrLineSource->FromString("ExposureActive");
					break;
				case EDeviceBaslerTOFGigELineSource_UserOutput:
					ptrLineSource->FromString("UserOutput");
					break;
				case EDeviceBaslerTOFGigELineSource_AcquisitionTriggerWait:
					ptrLineSource->FromString("AcquisitionTriggerWait");
					break;
				}

				bError = false;
			}
			catch(...){ }

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], g_lpszBaslerTOFLineSource[nPreValue], g_lpszBaslerTOFLineSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetOutputLine1Inverter(bool * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerTOFGigE_OutputLine1Inverter));

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetOutputLine1Inverter(bool bParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_OutputLine1Inverter;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			EDeviceBaslerTOFGigELineSelector eType = EDeviceBaslerTOFGigELineSelector_Count;

			if(GetLineSelector(&eType))
				break;

			if(eType != EDeviceBaslerTOFGigELineSelector_OutputLine1)
				break;

			GenApi::CEnumerationPtr ptrLineSelector = m_pCamera->GetParameter("LineSelector");

			if(!ptrLineSelector)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				ptrLineSelector->FromString("Out1");
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}

			GenApi::CBooleanPtr ptrLineInverter = m_pCamera->GetParameter("LineInverter");

			if(!ptrLineInverter)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bError = true;

			try
			{
				ptrLineInverter->SetValue(bParam);
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], g_lpszBaslerTOFSwitch[nPreValue], g_lpszBaslerTOFSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetOutputLine1OutputPulseMinWidth(float * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterBaslerTOFGigE_OutputLine1OutputPulseMinWidth));

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetOutputLine1OutputPulseMinWidth(float fParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_OutputLine1OutputPulseMinWidth;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			EDeviceBaslerTOFGigELineSelector eType = EDeviceBaslerTOFGigELineSelector_Count;

			if(GetLineSelector(&eType))
				break;

			if(eType != EDeviceBaslerTOFGigELineSelector_OutputLine1)
				break;

			GenApi::CEnumerationPtr ptrLineSelector = m_pCamera->GetParameter("LineSelector");

			if(!ptrLineSelector)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				ptrLineSelector->FromString("Out1");
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}

			GenApi::CFloatPtr ptrMinOutPulseWidth = m_pCamera->GetParameter("MinOutPulseWidth");

			if(!ptrMinOutPulseWidth)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bError = true;

			try
			{
				ptrMinOutPulseWidth->SetValue(fParam);
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%.6f"), fParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetOutputLine2Source(EDeviceBaslerTOFGigELineSource * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterBaslerTOFGigE_OutputLine2Source));

		if(nData < 0 || nData >= (int)EDeviceBaslerTOFGigELineSource_Count)
		{
			eReturn = EBaslerTOFGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaslerTOFGigELineSource)nData;

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetOutputLine2Source(EDeviceBaslerTOFGigELineSource eParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_OutputLine2Source;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDeviceBaslerTOFGigELineSource)0 || eParam >= EDeviceBaslerTOFGigELineSource_Count)
		{
			eReturn = EBaslerTOFSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			EDeviceBaslerTOFGigELineSelector eType = EDeviceBaslerTOFGigELineSelector_Count;

			if(GetLineSelector(&eType))
				break;

			if(eType != EDeviceBaslerTOFGigELineSelector_OutputLine2)
				break;

			GenApi::CEnumerationPtr ptrLineSelector = m_pCamera->GetParameter("LineSelector");

			if(!ptrLineSelector)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				ptrLineSelector->FromString("Out2");
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}

			GenApi::CEnumerationPtr ptrLineSource = m_pCamera->GetParameter("LineSource");

			if(!ptrLineSource)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bError = true;

			try
			{
				switch(eParam)
				{
				case EDeviceBaslerTOFGigELineSource_ExposureActive:
					ptrLineSource->FromString("ExposureActive");
					break;
				case EDeviceBaslerTOFGigELineSource_UserOutput:
					ptrLineSource->FromString("UserOutput");
					break;
				case EDeviceBaslerTOFGigELineSource_AcquisitionTriggerWait:
					ptrLineSource->FromString("AcquisitionTriggerWait");
					break;
				default:
					break;
				}

				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], g_lpszBaslerTOFLineSource[nPreValue], g_lpszBaslerTOFLineSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetOutputLine2Inverter(bool * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerTOFGigE_OutputLine2Inverter));

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetOutputLine2Inverter(bool bParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_OutputLine2Inverter;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			EDeviceBaslerTOFGigELineSelector eType = EDeviceBaslerTOFGigELineSelector_Count;

			if(GetLineSelector(&eType))
				break;

			if(eType != EDeviceBaslerTOFGigELineSelector_OutputLine2)
				break;

			GenApi::CEnumerationPtr ptrLineSelector = m_pCamera->GetParameter("LineSelector");

			if(!ptrLineSelector)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				ptrLineSelector->FromString("Out2");
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}

			GenApi::CBooleanPtr ptrLineInverter = m_pCamera->GetParameter("LineInverter");

			if(!ptrLineInverter)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bError = true;

			try
			{
				ptrLineInverter->SetValue(bParam);
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], g_lpszBaslerTOFSwitch[nPreValue], g_lpszBaslerTOFSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetOutputLine2OutputPulseMinWidth(float * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterBaslerTOFGigE_OutputLine2OutputPulseMinWidth));

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetOutputLine2OutputPulseMinWidth(float fParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_OutputLine2OutputPulseMinWidth;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			EDeviceBaslerTOFGigELineSelector eType = EDeviceBaslerTOFGigELineSelector_Count;

			if(GetLineSelector(&eType))
				break;

			if(eType != EDeviceBaslerTOFGigELineSelector_OutputLine2)
				break;

			GenApi::CEnumerationPtr ptrLineSelector = m_pCamera->GetParameter("LineSelector");

			if(!ptrLineSelector)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				ptrLineSelector->FromString("Out2");
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}

			GenApi::CFloatPtr ptrMinOutPulseWidth = m_pCamera->GetParameter("MinOutPulseWidth");

			if(!ptrMinOutPulseWidth)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bError = true;

			try
			{
				ptrMinOutPulseWidth->SetValue(fParam);
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%.6f"), fParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetUserOutputSelector(EDeviceBaslerTOFGigEUserOutputSelector * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterBaslerTOFGigE_UserOutputSelector));

		if(nData < 0 || nData >= (int)EDeviceBaslerTOFGigEUserOutputSelector_Count)
		{
			eReturn = EBaslerTOFGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaslerTOFGigEUserOutputSelector)nData;

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetUserOutputSelector(EDeviceBaslerTOFGigEUserOutputSelector eParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_UserOutputSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDeviceBaslerTOFGigEUserOutputSelector)0 || eParam >= EDeviceBaslerTOFGigEUserOutputSelector_Count)
		{
			eReturn = EBaslerTOFSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			GenApi::CEnumerationPtr ptrUserOutputSelector = m_pCamera->GetParameter("UserOutputSelector");

			if(!ptrUserOutputSelector)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				switch(eParam)
				{
				case EDeviceBaslerTOFGigEUserOutputSelector_UserOutput1:
					ptrUserOutputSelector->FromString("UserOutput1");
					break;
				case EDeviceBaslerTOFGigEUserOutputSelector_UserOutput2:
					ptrUserOutputSelector->FromString("UserOutput2");
					break;
				}

				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}		
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], g_lpszBaslerTOFUserOutputSelector[nPreValue], g_lpszBaslerTOFUserOutputSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetUserOutput1Value(bool * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerTOFGigE_UserOutput1Value));

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetUserOutput1Value(bool bParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_UserOutput1Value;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			EDeviceBaslerTOFGigEUserOutputSelector eType = EDeviceBaslerTOFGigEUserOutputSelector_Count;

			if(eType != EDeviceBaslerTOFGigEUserOutputSelector_UserOutput1)
				break;

			GenApi::CEnumerationPtr ptrUserOutputSelector = m_pCamera->GetParameter("UserOutputSelector");

			if(!ptrUserOutputSelector)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				ptrUserOutputSelector->FromString("UserOutput1");
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}

			GenApi::CBooleanPtr ptrUserOutputValue = m_pCamera->GetParameter("UserOutputValue");

			if(!ptrUserOutputValue)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bError = true;

			try
			{
				ptrUserOutputValue->SetValue(bParam);
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], g_lpszBaslerTOFSwitch[nPreValue], g_lpszBaslerTOFSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetUserOutput2Value(bool * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaslerTOFGigE_UserOutput2Value));

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetUserOutput2Value(bool bParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_UserOutput2Value;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			EDeviceBaslerTOFGigEUserOutputSelector eType = EDeviceBaslerTOFGigEUserOutputSelector_Count;

			if(eType != EDeviceBaslerTOFGigEUserOutputSelector_UserOutput2)
				break;

			GenApi::CEnumerationPtr ptrUserOutputSelector = m_pCamera->GetParameter("UserOutputSelector");

			if(!ptrUserOutputSelector)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				ptrUserOutputSelector->FromString("UserOutput2");
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}

			GenApi::CBooleanPtr ptrUserOutputValue = m_pCamera->GetParameter("UserOutputValue");

			if(!ptrUserOutputValue)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bError = true;

			try
			{
				ptrUserOutputValue->SetValue(bParam);
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strSave))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], g_lpszBaslerTOFSwitch[nPreValue], g_lpszBaslerTOFSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaslerTOFGetFunction CDeviceBaslerTOFGigE::GetUserOutputValueAll(float * pParam)
{
	EBaslerTOFGetFunction eReturn = EBaslerTOFGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaslerTOFGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterBaslerTOFGigE_UserOutputValueAll));

		eReturn = EBaslerTOFGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaslerTOFSetFunction CDeviceBaslerTOFGigE::SetUserOutputValueAll(float fParam)
{
	EBaslerTOFSetFunction eReturn = EBaslerTOFSetFunction_UnknownError;

	EDeviceParameterBaslerTOFGigE eSaveID = EDeviceParameterBaslerTOFGigE_UserOutputValueAll;

	float fPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			GenApi::CIntegerPtr ptrUserOutputValueAll = m_pCamera->GetParameter("UserOutputValueAll");

			if(!ptrUserOutputValueAll)
			{
				eReturn = EBaslerTOFSetFunction_NotSupportedError;
				break;
			}

			bool bError = true;

			try
			{
				ptrUserOutputValueAll->SetValue(fParam);
				bError = false;
			}
			catch(...){	}

			if(bError)
			{
				eReturn = EBaslerTOFSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%.6f"), fParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EBaslerTOFSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaslerTOFSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaslerTOFGigE[eSaveID], fPreValue, fParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceBaslerTOFGigE::GetInformation(ptrdiff_t x, ptrdiff_t y, float * pXCoordinate, float * pYCoordinate, float * pZCoordinate, float * pDistance)
{
	bool bReturn = false;

	do 
	{
		if(!m_p3DInfo)
			break;

		if(!m_p3DInfo->GetBuffer())
			break;

		if(!pXCoordinate || !pYCoordinate || !pZCoordinate || !pDistance)
			break;

		if(x < 0 || y < 0)
			break;

		if(x >= m_nCanvasX || y >= m_nCanvasY)
			break;

		MakeInvalid(*pXCoordinate);
		*pYCoordinate = *pXCoordinate;
		*pZCoordinate = *pXCoordinate;
		*pDistance = *pXCoordinate;

		float* pDataBuffer = (float*)(m_p3DInfo->GetBuffer() + (y * m_p3DInfo->GetWidthStep()));
		GenTLConsumerImplHelper::CToFCamera::Coord3D *p3DCoordinate = (GenTLConsumerImplHelper::CToFCamera::Coord3D*) pDataBuffer + x;

		if(p3DCoordinate->IsValid())
		{
			*pXCoordinate = p3DCoordinate->x;
			*pYCoordinate = p3DCoordinate->y;
			*pZCoordinate = p3DCoordinate->z;
			*pDistance = sqrt((p3DCoordinate->x * p3DCoordinate->x) + (p3DCoordinate->y * p3DCoordinate->y) + (p3DCoordinate->z * p3DCoordinate->z));
		}

		bReturn = true;

	} 
	while(false);

	return bReturn;
}

bool CDeviceBaslerTOFGigE::GetInformation(CRavidPoint<int> point, float * pXCoordinate, float * pYCoordinate, float * pZCoordinate, float * pDistance)
{
	return GetInformation(point.x, point.y, pXCoordinate, pYCoordinate, pZCoordinate, pDistance);
}

void CDeviceBaslerTOFGigE::DrawPixelInfo(int nDrawIndex)
{
	do 
	{
		CRavidImageView* pView = GetImageView();

		if(!pView)
			break;

		CRavidImageViewLayer* pLayer = pView->GetLayer(nDrawIndex);
		if(!pLayer)
			break;

		pLayer->Clear();

		if(m_bDrawPoint)
		{
			pLayer->DrawShape(CRavidRect<double>(m_DrawPoint.x, m_DrawPoint.y, m_DrawPoint.x + 1, m_DrawPoint.y + 1), LIME, 3);

			float x = 0.f, y = 0.f, z = 0.f, dis = 0.f;
			GetInformation(m_DrawPoint.x, m_DrawPoint.y, &x, &y, &z, &dis);

			CString strUpdate;
			strUpdate.Format(_T("x : %.3f\ny : %.3f\nz : %.3f\nDist : %.3f"), x, y, z, dis);
			pLayer->DrawTextW(m_DrawPoint, strUpdate, LIME, BLACK);
		}

		pView->Invalidate();
	} 
	while(false);
}

CRavidImage * CDeviceBaslerTOFGigE::Get3DImageInfo()
{
	return m_p3DInfo;
}

CRavidImage * CDeviceBaslerTOFGigE::GetIntensityInfo()
{
	return this->GetImageInfo();
}

CRavidImage * CDeviceBaslerTOFGigE::GetConfidenceInfo()
{
	return m_pConfidenceMap;
}

bool CDeviceBaslerTOFGigE::Save3DImageData(CString strFilePath, CRavidImage* pSaveImage)
{
	bool bReturn = false;

	FILE *fp = nullptr;

	do 
	{
		if(!pSaveImage)
			break;

		if(!strFilePath.GetLength())
			break;

		if(pSaveImage->GetChannels() != 3)
			break;

		if((!pSaveImage->IsFloatingPoint()) || (pSaveImage->GetDepth() != 32))
			break;

		if(strFilePath.Right(4).CompareNoCase(_T(".dat")))
			strFilePath.AppendFormat(_T("%s"), _T(".dat"));

		fp = _tfopen(strFilePath, _T("wb"));

		if(!fp)
			break;

		long nWidth = pSaveImage->GetSizeX();
		long nHeight = pSaveImage->GetSizeY();

		fwrite(&nWidth, sizeof(long), 1, fp);
		fwrite(&nHeight, sizeof(long), 1, fp);

		for(int y = 0; y < nHeight; ++y)
		{
			float* pfData = (float*)(pSaveImage->GetYOffsetTable()[y]);

			fwrite(pfData, sizeof(float), pSaveImage->GetWidthStep(), fp);
		}

		bReturn = true;
	} 
	while(false);

	if(fp)
	{
		fclose(fp);
		fp = nullptr;
	}

	return bReturn;
}

bool CDeviceBaslerTOFGigE::Load3DImageData(CString strFilePath, CRavidImage * pLoadImage)
{
	bool bReturn = false;

	FILE *fp = nullptr;

	do
	{
		if(!pLoadImage)
			break;

		if(!strFilePath.GetLength())
			break;

		if(strFilePath.Right(4).CompareNoCase(_T(".dat")))
			strFilePath.AppendFormat(_T("%s"), _T(".dat"));

		fp = _tfopen(strFilePath, _T("rb"));

		if(!fp)
			break;

		long nWidth = 0;
		long nHeight = 0;

		fread(&nWidth, sizeof(long), 1, fp);
		fread(&nHeight, sizeof(long), 1, fp);

		CMultipleVariable mv;
		mv.AddValue(0);
		mv.AddValue(0);
		mv.AddValue(0);

		pLoadImage->CreateImageBuffer(nWidth, nHeight, mv, CRavidImage::MakeValueFormat(3, 32, Ravid::Algorithms::CRavidImage::EValueType_FloatingPoint), nWidth * 12);

		for(int y = 0; y < nHeight; ++y)
		{
			float* pfData = (float*)(pLoadImage->GetYOffsetTable()[y]);

			fread(pfData, sizeof(float), pLoadImage->GetWidthStep(), fp);
		}

		bReturn = true;
	}
	while(false);

	if(fp)
	{
		fclose(fp);
		fp = nullptr;
	}

	return bReturn;
}

bool CDeviceBaslerTOFGigE::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = false;
	bool bFoundID = true;

	do
	{
		switch(nParam)
		{
		case EDeviceParameterBaslerTOFGigE_DeviceID:
			bReturn = !SetDeviceID(strValue);
			break;
		case EDeviceParameterBaslerTOFGigE_DrawPoint:
			bReturn = !SetDrawPoint(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_DrawImage:
			bReturn = !SetDrawImage((EDeviceBaslerTOFGigESelectImage)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_DeviceChannel:
			bReturn = !SetDeviceChannel((EDeviceBaslerTOFGigEDeviceChannel)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_CalibrationRangeOffset:
			bReturn = !SetCalibrationRangeOffset(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_Rectification:
			bReturn = !SetRectification((EDeviceBaslerTOFGigESwitch)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_DeviceTemperatureSelector:
			bReturn = !SetDeviceTemperatureSelector((EDeviceBaslerTOFGigETemperatureSelector)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_Binning:
			bReturn = !SetBinning(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_CanvasWidth:
			bReturn = !SetCanvasWidth(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_CanvasHeight:
			bReturn = !SetCanvasHeight(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_OffsetX:
			bReturn = !SetOffsetX(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_OffsetY:
			bReturn = !SetOffsetY(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_MinimumDepth_mm:
			bReturn = !SetMinimumDepth_mm(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_MaximumDepth_mm:
			bReturn = !SetMaximumDepth_mm(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_ComponentSelector:
			bReturn = !SetComponentSelector((EDeviceBaslerTOFGigEComponentSelector)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_LEDDisable:
			bReturn = !SetLEDDisable(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_ProcessingMode:
			bReturn = !SetProcessingMode((EDeviceBaslerTOFGigEProcessingMode)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_AcquisitionFrameRate:
			bReturn = !SetAcquisitionFrameRate(_ttof(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_ExposureAuto:
			bReturn = !SetExposureAuto((EDeviceBaslerTOFGigEExposureAuto)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_Agility:
			bReturn = !SetAgility(_ttof(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_Delay:
			bReturn = !SetDelay(_ttof(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_ExposureTimeSelector:
			bReturn = !SetExposureTimeSelector(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_ExposureTime_us:
			bReturn = !SetExposureTime_us(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_TriggerMode:
			bReturn = !SetTriggerMode((EDeviceBaslerTOFGigESwitch)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_TriggerSource:
			bReturn = !SetTriggerSource((EDeviceBaslerTOFGigETriggerSource)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_TriggerDelay_us:
			bReturn = !SetTriggerDelay_us(_ttof(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_TimestampLatch:
			bReturn = !SetTimestampLatch(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_StartTimeLow_us:
			bReturn = !SetTimestampLow_ns(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_StartTimeHigh_us:
			bReturn = !SetTimestampHigh_ns(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_TriggerRat_Hz:
			bReturn = !SetTriggerRat_Hz(_ttof(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_Update:
			bReturn = !SetUpdate(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_ConfidenceThreshold:
			bReturn = !SetConfidenceThreshold(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_SpatialFilter:
			bReturn = !SetSpatialFilter(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_TemporalFilter:
			bReturn = !SetTemporalFilter(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_Strength:
			bReturn = !SetStrength(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_OutlierTolerance:
			bReturn = !SetOutlierTolerance(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_RangeFilter:
			bReturn = !SetRangeFilter(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_LineSelector:
			bReturn = !SetLineSelector((EDeviceBaslerTOFGigELineSelector)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_Line1Inverter:
			bReturn = !SetLine1Inverter(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_Line1DebouncerTime_us:
			bReturn = !SetLine1DebouncerTime_us(_ttof(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_Line2Inverter:
			bReturn = !SetLine2Inverter(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_Line2DebouncerTime_us:
			bReturn = !SetLine2DebouncerTime_us(_ttof(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_OutputLine1Source:
			bReturn = !SetOutputLine1Source((EDeviceBaslerTOFGigELineSource)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_OutputLine1Inverter:
			bReturn = !SetOutputLine1Inverter(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_OutputLine1OutputPulseMinWidth:
			bReturn = !SetOutputLine1OutputPulseMinWidth(_ttof(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_OutputLine2Source:
			bReturn = !SetOutputLine2Source((EDeviceBaslerTOFGigELineSource)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_OutputLine2Inverter:
			bReturn = !SetOutputLine2Inverter(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_OutputLine2OutputPulseMinWidth:
			bReturn = !SetOutputLine2OutputPulseMinWidth(_ttof(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_UserOutputSelector:
			bReturn = !SetUserOutputSelector((EDeviceBaslerTOFGigEUserOutputSelector)_ttoi(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_UserOutput1Value:
			bReturn = !SetUserOutput1Value(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_UserOutput2Value:
			bReturn = !SetUserOutput2Value(_ttoi(strValue));
			break;
		case EDeviceParameterBaslerTOFGigE_UserOutputValueAll:
			bReturn = !SetUserOutputValueAll(_ttof(strValue));
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

bool CDeviceBaslerTOFGigE::DoesModuleExist()
{
	bool bReturn = false;

	CString strModuleName;

	do
	{
		strModuleName.Format(_T("GCBase_MD_VC120_v3_0_Basler_pylon_v5_0.dll"));

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

UINT CDeviceBaslerTOFGigE::CallbackFunction(LPVOID pParam)
{
	CDeviceBaslerTOFGigE* pInstance = (CDeviceBaslerTOFGigE*)pParam;

	if(pInstance)
	{
		try
		{
			int nCount = 0;

			pInstance->m_pCamera->IssueAcquisitionStartCommand();

			do
			{
				GenTLConsumerImplHelper::GrabResult grabResult;

				pInstance->m_pCamera->GetGrabResult(grabResult, 1000);

				if(grabResult.status == GenTLConsumerImplHelper::GrabResult::Failed)
					break;

				if(grabResult.status == GenTLConsumerImplHelper::GrabResult::Timeout)
					break;

				nCount++;

				pInstance->NextImageIndex();

				CRavidImage* pCurrentImage = pInstance->GetImageInfo();

				BYTE* pCurrentBuffer = pCurrentImage->GetBuffer();

				GenTLConsumerImplHelper::BufferParts parts;
				pInstance->m_pCamera->GetBufferParts(grabResult, parts);

				BYTE* p3DBuffer = pInstance->m_p3DInfo->GetBuffer();
				BYTE* pIntensityBuffer = pCurrentImage->GetBuffer();
				BYTE* pConfidenceBuffer = pInstance->m_pConfidenceMap->GetBuffer();

				BYTE* p3D = (BYTE*)parts[0].pData;
				uint16_t *pIntensity = (uint16_t*)parts[1].pData;
				uint16_t *pConfidence = (uint16_t*)parts[2].pData;

				memcpy(p3DBuffer, p3D, sizeof(BYTE) * parts[0].size);
				memcpy(pIntensityBuffer, pIntensity, parts[1].size);
				memcpy(pConfidenceBuffer, pConfidence, parts[2].size);

				CRavidImageView* pCurView = pInstance->GetImageView();
				if(pCurView)
				{
					pCurView->Invalidate();
					pInstance->DrawPixelInfo(9);
				}

				pInstance->ConnectImage(false);

				CEventHandlerManager::BroadcastOnAcquisition(pCurrentImage, pInstance->GetObjectID(), pInstance->GetImageView(), pInstance);
				
				pInstance->m_pCamera->QueueBuffer(grabResult.hBuffer);
			}
			while(pInstance->IsInitialized() && (pInstance->IsLive() || (unsigned long)nCount > pInstance->m_nGrabCount));

			pInstance->m_pCamera->IssueAcquisitionStopCommand();
		}
		catch (...)
		{
		}
		
		pInstance->m_bIsLive = false;
		pInstance->m_bIsGrabAvailable = true;
	}

	return 0;
}

bool CDeviceBaslerTOFGigE::SetBoolean(const char * pCommand, bool bData)
{
	bool bReturn = false;

	do 
	{
		GenApi::CBooleanPtr ptrBoolean = m_pCamera->GetParameter(pCommand);

		if(!ptrBoolean)
			break;

		try
		{
			ptrBoolean->SetValue(bData);
			bReturn = true;
		}
		catch(...) { }
	} 
	while(false);

	return bReturn;
}

bool CDeviceBaslerTOFGigE::SetInteger(const char * pCommand, int nData)
{
	bool bReturn = false;

	do
	{
		GenApi::CIntegerPtr ptrInteger = m_pCamera->GetParameter(pCommand);

		if(!ptrInteger)
			break;

		try
		{
			ptrInteger->SetValue(nData);
			bReturn = true;
		}
		catch(...) { }
	}
	while(false);

	return bReturn;
}

bool CDeviceBaslerTOFGigE::SetFloating(const char * pCommand, float fData)
{
	bool bReturn = false;

	do
	{
		GenApi::CFloatPtr ptrFloating = m_pCamera->GetParameter(pCommand);

		if(!ptrFloating)
			break;

		try
		{
			ptrFloating->SetValue(fData);
			bReturn = true;
		}
		catch(...) { }
	}
	while(false);

	return bReturn;
}

bool CDeviceBaslerTOFGigE::SetEnumeration(const char * pCommand, const char * pData)
{
	bool bReturn = false;

	do
	{
		GenApi::CEnumerationPtr ptrProcessingMode = m_pCamera->GetParameter(pCommand);

		if(!ptrProcessingMode)
			break;

		try
		{
			ptrProcessingMode->FromString(pData);
			bReturn = true;
		}
		catch(...) { }
	}
	while(false);

	return bReturn;
}

#endif