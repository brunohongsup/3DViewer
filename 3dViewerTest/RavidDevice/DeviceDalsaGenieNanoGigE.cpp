#include "stdafx.h"

#include "DeviceDalsaGenieNanoGigE.h"

#ifdef USING_DEVICE

#include "../RavidCore/RavidRect.h"
#include "../RavidCore/RavidImage.h"
#include "../RavidCore/RavidDatabase.h"
#include "../RavidCore/RavidRecordset.h"
#include "../RavidCore/PerformanceCounter.h"
#include "../RavidCore/OperatingSystemInfo.h"

#include "../RavidFramework/LogManager.h"
#include "../RavidFramework/DeviceManager.h"
#include "../RavidFramework/LibraryManager.h" 
#include "../RavidFramework/FrameworkManager.h"
#include "../RavidFramework/EventHandlerManager.h"
#include "../RavidFramework/MultiLanguageManager.h"

#include "../RavidFramework/RavidTreeView.h"
#include "../RavidFramework/RavidImageView.h"

#include "../Libraries/Includes/DalsaXtiumMX4/SapClassBasic.h"

using namespace Ravid;
using namespace Ravid::Device;
using namespace Ravid::Database;
using namespace Ravid::Framework;
using namespace Ravid::Algorithms;
using namespace Ravid::Miscellaneous;

IMPLEMENT_DYNAMIC(CDeviceDalsaGenieNanoGigE, CDeviceCamera)

BEGIN_MESSAGE_MAP(CDeviceDalsaGenieNanoGigE, CDeviceCamera)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static LPCTSTR g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_Count] =
{
	_T("DeviceID"),
	_T("Camera Setting"),
	_T("Grab count"),
	_T("Canvas width"),
	_T("Canvas height"),
	_T("Offset X"),
	_T("Offset Y"),
	_T("Flip Horizontal"),
	_T("Flip Vertical"),
	_T("PixelFormat"),
	_T("Packet Size"),
	_T("Device Buffer Count"),
	_T("Trigger Setting"),
	_T("Trigger Selector"),
	_T("FrameStart"),
	_T("FrameStart Trigger Mode"),
	_T("FrameStart Trigger Source"),
	_T("FrameStart Trigger Activation"),
	_T("FrameBurstStart"),
	_T("FrameBurstStart Trigger Mode"),
	_T("FrameBurstStart Trigger Source"),
	_T("FrameBurstStart Trigger Count"),
	_T("AcquisitionStart"),
	_T("AcquisitionStart Trigger Mode"),
	_T("AcquisitionStart Trigger Source"),
	_T("Exposure and Gain Setting"),
	_T("FrameRate Control"),
	_T("FrameRate"),
	_T("Exposure Time"),
	_T("Gain Selector"),
	_T("Sensor Gain"),
	_T("Digital Gain"),
	_T("Black level selector"),
	_T("Black level"),
	_T("LineSelector"),
	_T("Line1"),
	_T("Line1DebouncingPeriod"),
	_T("Line1Inverter"),
	_T("Line2"),
	_T("Line2DebouncingPeriod"),
	_T("Line2Inverter"),
	_T("Line3"),
	_T("Line3Source"),
	_T("Line3Inverter"),
	_T("Line3Activation"),
	_T("Line3Delay"),
	_T("Line3Duration"),
	_T("Line3Value"),
	_T("Line4"),
	_T("Line4Source"),
	_T("Line4Inverter"),
	_T("Line4Activation"),
	_T("Line4Delay"),
	_T("Line4Duration"),
	_T("Line4Value"),
};

static LPCTSTR g_lpszDalsaGenieNanoGigEPixelFormat[EDalsaGenieNanoGigEPixelFormat_Count] =
{
	_T("Mono8"),
	_T("Mono8Signed"),
	_T("Mono10"),
	_T("Mono12"),
	_T("Mono14"),
	_T("Mono16"),
	_T("BayerRG8"),
	_T("BayerRG10"),
	_T("BayerRG12"),
	_T("BayerRG16"),
	_T("BayerGB8"),
	_T("BayerGB10"),
	_T("BayerGB12"),
	_T("BayerGB16"),
	_T("BayerGR8"),
	_T("BayerGR10"),
	_T("BayerGR12"),
	_T("BayerGR16"),
	_T("BayerBG8"),
	_T("BayerBG10"),
	_T("BayerBG12"),
	_T("BayerBG16"),
	_T("BGR8Packed"),
	_T("BGRA8Packed"),
	_T("BGR10Packed"),
	_T("BGR12Packed"),
	_T("YUV411Packed"),
	_T("YUV422_YUYV_Packed"),
	_T("YUV422Packed"),
};

static LPCTSTR g_lpszDalsaGenieNanoGigETriggerSelect[EDalsaGenieNanoGigETriggerSelector_Count] =
{
	_T("FrameStart"),
	_T("FrameBurstStart"),
	_T("AcquisitionStart"),
};

static LPCTSTR g_lpszDalsaGenieNanoGigESwitch[EDalsaGenieNanoGigESwitch_Count] =
{
	_T("Off"),
	_T("On")
};

static LPCTSTR g_lpszDalsaGenieNanoGigESource[EDalsaGenieNanoGigESource_Count] =
{
	_T("Software"),
	_T("Line1"),
	_T("Line2"),
	_T("Action1"),
	_T("Action2"),
};

static LPCTSTR g_lpszDalsaGenieNanoGigEActivation[EDalsaGenieNanoGigEActivation_Count] =
{
	_T("RisingEdge"),
	_T("FallingEdge"),
	_T("AnyEdge"),
};

static LPCTSTR g_lpszDalsaGenieNanoGigEFrameRateControl[EDalsaGenieNanoGigEGainSelector_Count] =
{
	_T("Programmable"),
	_T("MaximumSpeed"),
};

static LPCTSTR g_lpszDalsaGenieNanoGigEGainSelect[EDalsaGenieNanoGigEGainSelector_Count] =
{
	_T("SensorAll"),
	_T("DigitalAll"),
};

static LPCTSTR g_lpszDalsaGenieNanoGigEBlackSelector[EDalsaGenieNanoGigEBlackLevelSelector_Count] =
{
	_T("AnalogAll"),
};

static LPCTSTR g_lpszDalsaGenieNanoGigELineSelector[EDalsaGenieNanoGigELineSelector_Count] =
{
	_T("Line1"),
	_T("Line2"),
	_T("Line3"),
	_T("Line4"),
};

static LPCTSTR g_lpszDalsaGenieNanoGigELineSource[EDalsaGenieNanoGigELineSource_Count] =
{
	_T("Off"),
	_T("SoftwareControlled"),
	_T("PulseOnStartofFrame"),
	_T("PulseOnStartofExposure"),
	_T("PulseOnEndofExposure"),
	_T("PulseOnStartofReadout"),
	_T("PulseOnEndofReadout"),
	_T("PulseOnValidFrameTrigger"),
	_T("PulseOnInvalidFrameTrigger"),
	_T("PulseOnStratofAcquisition"),
	_T("PulseOnEndofAcquisition"),
	_T("PulseOnEndofTimer1"),
	_T("PulseOnEndofCounter1"),
	_T("PulseOnInput1"),
	_T("PulseOnInput2"),
	_T("PulseOnAction1"),
	_T("PulseOnAction2"),
	_T("PulseOnSoftwareCmd"),
	_T("PulseOnExposureActive"),
};

static LPCTSTR g_lpszDalsaGenieNanoGigELineValue[EDalsaGenieNanoGigELineValue_Count] =
{
	_T("Inactive"),
	_T("Active"),
};


class SapMyProcessing : public SapProcessing
{
public:
   // Constructor/Destructor
	SapMyProcessing(SapBuffer *pBuffers, SapColorConversion* pColorConv, SapProCallback pCallback, void *pContext);
	virtual ~SapMyProcessing();


protected:
	virtual BOOL Run();

protected:
	SapColorConversion * m_ColorConv;
};

SapMyProcessing::SapMyProcessing(SapBuffer *pBuffers, SapColorConversion* pColorConv, SapProCallback pCallback, void *pContext)
	: SapProcessing(pBuffers, pCallback, pContext)
{
	m_ColorConv = pColorConv;
}

SapMyProcessing::~SapMyProcessing()
{
	if(m_bInitOK)
		Destroy();
}

BOOL SapMyProcessing::Run()
{
	if(m_ColorConv->IsSoftwareEnabled())
	{
		int nIdx = GetIndex();
		m_ColorConv->Convert(nIdx);
	}

	return TRUE;
}

CDeviceDalsaGenieNanoGigE::CDeviceDalsaGenieNanoGigE()
{
}


CDeviceDalsaGenieNanoGigE::~CDeviceDalsaGenieNanoGigE()
{
	Terminate();
}

EDeviceInitializeResult CDeviceDalsaGenieNanoGigE::Initialize()
{
	EDeviceInitializeResult eReturn = EDeviceInitializeResult_UnknownError;

	CString strMessage, strStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	COperatingSystemInfo osInfo;

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("DalsaGenieNano"));
		
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

		SapManager::SetDisplayStatusMode(SapManager::StatusCustom);

		if(!SapManager::Open())
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntopenthelibrary);
			eReturn = EDeviceInitializeResult_NotFoundLibraries;
			break;
		}
		
		int serverCount = SapManager::GetServerCount();

		if(!serverCount)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtofindthedevice);
			eReturn = EDeviceInitializeResult_NotFoundDeviceError;
			break;
		}

		int featureCount = 0;

		char serverName[30];
		char serialNumberName[256];

		bool bError = false;

		CString strSerial = GetDeviceID();
		
		bool bSerialMatching = false;

		for(int serverIndex = 0; serverIndex < serverCount; serverIndex++)
		{
			int tt = SapManager::GetResourceCount(serverIndex, SapManager::ResourceAcqDevice) != 0;

			if(SapManager::GetResourceCount(serverIndex, SapManager::ResourceAcqDevice) != 0)
			{
				SapManager::GetServerName(serverIndex, serverName, sizeof(serverName));
				
				if(!SapManager::IsResourceAvailable(serverIndex, SapManager::ResourceAcqDevice))
					continue;
				
				SapAcqDevice camera(serverName);
				
				if(!camera.Create())
					continue;

				if(!camera.GetFeatureCount(&featureCount))
					continue;

				if(featureCount > 0)
				{
					if(!camera.GetFeatureValue("DeviceID", serialNumberName, sizeof(serialNumberName)))
						continue;
				}

				if(!strSerial.CompareNoCase(CString(serialNumberName)))
				{
					bSerialMatching = true;

					if(!camera.Destroy())
					{
						bError = true;
						break;
					}

					SapLocation loc(serverName, serverIndex);
					
					m_pDevice = new SapAcqDevice(serverName);

					if(!m_pDevice || !m_pDevice->Create())
					{
						bError = true;
						break;
					}

					break;
				}

				if(!camera.Destroy())
				{
					bError = true;
					break;
				}
			}

			if(bSerialMatching)
				break;
		}

		if(!bSerialMatching || !m_pDevice || bError)
		{
			m_pDevice = nullptr;
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntopenthedevice);
			eReturn = EDeviceInitializeResult_NotFoundDeviceError;
			break;
		}

		BOOL bCommandAvailable = false;

		m_pDevice->IsFeatureAvailable("Width", &bCommandAvailable);

		if(bCommandAvailable)
		{
			int nWidth = 0;

			if(GetCanvasWidth(&nWidth))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Width"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(!nWidth)
			{
				if(!m_pDevice->GetFeatureValue("Width", &nWidth))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Width"));
					eReturn = EDeviceInitializeResult_ReadOnDeviceError;
					break;
				}

				if(SetCanvasWidth(nWidth))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Width"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}
			}

			if(!m_pDevice->SetFeatureValue("Width", nWidth))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Width"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}
		}

		m_pDevice->IsFeatureAvailable("Height", &bCommandAvailable);

		if(bCommandAvailable)
		{
			int nHeight = 0;

			if(GetCanvasHeight(&nHeight))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Height"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(!nHeight)
			{
				if(!m_pDevice->GetFeatureValue("Height", &nHeight))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Height"));
					eReturn = EDeviceInitializeResult_ReadOnDeviceError;
					break;
				}

				if(SetCanvasHeight(nHeight))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Height"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}
			}

			if(!m_pDevice->SetFeatureValue("Height", nHeight))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Height"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}
		}

		m_pDevice->IsFeatureAvailable("OffsetX", &bCommandAvailable);

		if(bCommandAvailable)
		{
			int nParamX = 0;

			if(GetOffsetX(&nParamX))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("OffsetX"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(!m_pDevice->SetFeatureValue("OffsetX", nParamX))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("OffsetX"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}
		}

		m_pDevice->IsFeatureAvailable("OffsetY", &bCommandAvailable);

		if(bCommandAvailable)
		{
			int nParamY = 0;

			if(GetOffsetY(&nParamY))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("OffsetY"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(!m_pDevice->SetFeatureValue("OffsetY", nParamY))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("OffsetY"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}
		}

		m_pDevice->IsFeatureAvailable("ReverseX", &bCommandAvailable);

		if(bCommandAvailable)
		{
			bool bFlipHorizontal = false;

			if(GetFlipHorizontal(&bFlipHorizontal))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Flip Horizontal"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(!m_pDevice->SetFeatureValue("ReverseX", bFlipHorizontal))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ReverseX"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}
		}

		m_pDevice->IsFeatureAvailable("ReverseY", &bCommandAvailable);

		if(bCommandAvailable)
		{
			bool bFlipVertical = false;

			if(GetFlipVertical(&bFlipVertical))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Flip Vertical"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(!m_pDevice->SetFeatureValue("ReverseY", bFlipVertical))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ReverseY"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}
		}

		m_pDevice->IsFeatureAvailable("PixelFormat", &bCommandAvailable);

		if(bCommandAvailable)
		{
			EDalsaGenieNanoGigEPixelFormat ePixelFormat = EDalsaGenieNanoGigEPixelFormat_Count;

			if(GetPixelFormat(&ePixelFormat))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("PixelFormat"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(!m_pDevice->SetFeatureValue("PixelFormat", CStringA(g_lpszDalsaGenieNanoGigEPixelFormat[ePixelFormat])))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("PixelFormat"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}
		}

		int nPacketSize = 0;

		if(GetPacketSize(&nPacketSize))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("PacketSize"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(!m_pDevice->IsFeatureAvailable("GevSCPSPacketSize", &bCommandAvailable))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("PacketSize"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		if(!bCommandAvailable)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("PacketSize"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		if(!m_pDevice->SetFeatureValue("GevSCPSPacketSize", nPacketSize))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("PacketSize"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}


		m_pDevice->IsFeatureAvailable("TriggerSelector", &bCommandAvailable);

		if(bCommandAvailable)
		{
			EDalsaGenieNanoGigETriggerSelector eTriggerSelector = EDalsaGenieNanoGigETriggerSelector_Count;

			if(GetTriggerSelector(&eTriggerSelector))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TriggerSelector"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(!m_pDevice->SetFeatureValue("TriggerSelector", CStringA(g_lpszDalsaGenieNanoGigETriggerSelect[eTriggerSelector])))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TriggerSelector"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}

			m_pDevice->IsFeatureAvailable("TriggerMode", &bCommandAvailable);

			if(bCommandAvailable)
			{
				switch(eTriggerSelector)
				{
				case EDalsaGenieNanoGigETriggerSelector_FrameStart:
					{
						EDalsaGenieNanoGigESwitch eTrigMode = EDalsaGenieNanoGigESwitch_Count;

						if(GetFrameStartMode(&eTrigMode))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("FrameStartMode"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}

						if(!m_pDevice->SetFeatureValue("TriggerMode", CStringA(g_lpszDalsaGenieNanoGigESwitch[eTrigMode])))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("FrameStartMode"));
							eReturn = EDeviceInitializeResult_WriteToDeviceError;
							break;
						}

						if(eTrigMode == EDalsaGenieNanoGigESwitch_On)
						{
							m_pDevice->IsFeatureAvailable("TriggerSource", &bCommandAvailable);

							if(bCommandAvailable)
							{
								EDalsaGenieNanoGigESource eTrigSource = EDalsaGenieNanoGigESource_Count;

								if(GetFrameStartTriggerSource(&eTrigSource))
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("FrameStartTriggerSource"));
									eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
									break;
								}

								if(!m_pDevice->SetFeatureValue("TriggerSource", CStringA(g_lpszDalsaGenieNanoGigESource[eTrigSource])))
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("FrameStartTriggerSource"));
									eReturn = EDeviceInitializeResult_WriteToDeviceError;
									break;
								}

								if(eTrigSource != EDalsaGenieNanoGigESource_Software)
								{
									m_pDevice->IsFeatureAvailable("TriggerActivation", &bCommandAvailable);

									if(bCommandAvailable)
									{
										EDalsaGenieNanoGigEActivation eTrigActivation = EDalsaGenieNanoGigEActivation_Count;

										if(GetFrameStartTriggerActivation(&eTrigActivation))
										{
											strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("FrameStartTriggerActivation"));
											eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
											break;
										}

										if(!m_pDevice->SetFeatureValue("TriggerActivation", CStringA(g_lpszDalsaGenieNanoGigEActivation[eTrigActivation])))
										{
											strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("FrameStartTriggerActivation"));
											eReturn = EDeviceInitializeResult_WriteToDeviceError;
											break;
										}
									}
								}
							}
						}
					}
					break;
				case EDalsaGenieNanoGigETriggerSelector_FrameBurstStart:
					{
						EDalsaGenieNanoGigESwitch eTrigMode = EDalsaGenieNanoGigESwitch_Count;

						if(GetFrameBurstStartMode(&eTrigMode))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("FrameBurstStartMode"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}

						if(!m_pDevice->SetFeatureValue("TriggerMode", CStringA(g_lpszDalsaGenieNanoGigESwitch[eTrigMode])))
						{

							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("FrameBurstStartMode"));
							eReturn = EDeviceInitializeResult_WriteToDeviceError;
							break;
						}

						if(eTrigMode == EDalsaGenieNanoGigESwitch_On)
						{
							m_pDevice->IsFeatureAvailable("TriggerSource", &bCommandAvailable);

							if(bCommandAvailable)
							{
								EDalsaGenieNanoGigESource eTrigSource = EDalsaGenieNanoGigESource_Count;

								if(GetFrameBurstStartTriggerSource(&eTrigSource))
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("FrameBurstStartTriggerSource"));
									eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
									break;
								}

								if(!m_pDevice->SetFeatureValue("TriggerSource", CStringA(g_lpszDalsaGenieNanoGigESource[eTrigSource])))
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("FrameBurstStartTriggerSource"));
									eReturn = EDeviceInitializeResult_WriteToDeviceError;
									break;
								}

								m_pDevice->IsFeatureAvailable("triggerFrameCount", &bCommandAvailable);

								if(bCommandAvailable)
								{
									int nFrameCounr = 0;

									if(GetFrameBurstStartTriggerCount(&nFrameCounr))
									{
										strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("FrameBurstStartTriggerCount"));
										eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
										break;
									}

									if(!m_pDevice->SetFeatureValue("triggerFrameCount", nFrameCounr))
									{
										strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("FrameBurstStartTriggerCount"));
										eReturn = EDeviceInitializeResult_WriteToDeviceError;
										break;
									}
								}
							}
						}
					}
					break;
				case EDalsaGenieNanoGigETriggerSelector_AcquisitionStart:
					{
						EDalsaGenieNanoGigESwitch eTrigMode = EDalsaGenieNanoGigESwitch_Count;

						if(GetAcquisitionStartMode(&eTrigMode))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AcquisitionStartMode"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}

						if(!m_pDevice->SetFeatureValue("TriggerMode", CStringA(g_lpszDalsaGenieNanoGigESwitch[eTrigMode])))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("AcquisitionStartMode"));
							eReturn = EDeviceInitializeResult_WriteToDeviceError;
							break;
						}

						if(eTrigMode == EDalsaGenieNanoGigESwitch_On)
						{
							m_pDevice->IsFeatureAvailable("TriggerSource", &bCommandAvailable);

							if(bCommandAvailable)
							{
								EDalsaGenieNanoGigESource eTrigSource = EDalsaGenieNanoGigESource_Count;

								if(GetAcquisitionStartTriggerSource(&eTrigSource))
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AcquisitionStartTriggerSource"));
									eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
									break;
								}

								if(!m_pDevice->SetFeatureValue("TriggerSource", CStringA(g_lpszDalsaGenieNanoGigESource[eTrigSource])))
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("AcquisitionStartTriggerSource"));
									eReturn = EDeviceInitializeResult_WriteToDeviceError;
									break;
								}
							}
						}
					}
					break;
				}
			}
		}

		EDalsaGenieNanoGigEFrameRateControl eFrameControl = EDalsaGenieNanoGigEFrameRateControl_Count;

		if(GetFrameRateControl(&eFrameControl))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("FrameRateControl"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		m_pDevice->IsFeatureAvailable("acquisitionFrameRateControlMode", &bCommandAvailable);
		

		if(bCommandAvailable)
		{
			if(!m_pDevice->SetFeatureValue("acquisitionFrameRateControlMode", CStringA(g_lpszDalsaGenieNanoGigEFrameRateControl[eFrameControl])))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("FrameRateControl"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}
		}
		

		if(eFrameControl == EDalsaGenieNanoGigEFrameRateControl_MaximumSpeed)
		{
			m_pDevice->IsFeatureAvailable("ExposureTime", &bCommandAvailable);

			if(bCommandAvailable)
			{
				double dblExposureTime = 0.;

				if(GetExposureTime(&dblExposureTime))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ExposureTime"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(!m_pDevice->SetFeatureValue("ExposureTime", dblExposureTime))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ExposureTime"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}
			}
		}
		else if(eFrameControl == EDalsaGenieNanoGigEFrameRateControl_Programmable)
		{
			m_pDevice->IsFeatureAvailable("AcquisitionFrameRate", &bCommandAvailable);

			if(bCommandAvailable)
			{
				double dblFrameRate = 0.;

				if(GetFrameRate(&dblFrameRate))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AcquisitionFrameRate"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(!m_pDevice->SetFeatureValue("AcquisitionFrameRate", dblFrameRate))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("AcquisitionFrameRate"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}
			}
		}
		
		m_pDevice->IsFeatureAvailable("GainSelector", &bCommandAvailable);

		if(bCommandAvailable)
		{
			m_pDevice->IsFeatureAvailable("SensorAll", &bCommandAvailable);

			if(bCommandAvailable)
			{
				if(!m_pDevice->SetFeatureValue("GainSelector", "SensorAll"))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("SensorAll"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				m_pDevice->IsFeatureAvailable("Gain", &bCommandAvailable);

				if(bCommandAvailable)
				{
					double dblParam = 0.;

					if(GetSensorGain(&dblParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("SensorGain"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(!m_pDevice->SetFeatureValue("Gain", dblParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("SensorGain"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}
				}
			}

			m_pDevice->IsFeatureAvailable("DigitalAll", &bCommandAvailable);

			if(bCommandAvailable)
			{
				if(!m_pDevice->SetFeatureValue("GainSelector", "DigitalAll"))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("DigitalAll"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				m_pDevice->IsFeatureAvailable("Gain", &bCommandAvailable);

				if(bCommandAvailable)
				{
					double dblParam = 0.;

					if(GetDigitalGain(&dblParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("DigitalGain"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(!m_pDevice->SetFeatureValue("Gain", dblParam))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("DigitalGain"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}
				}
			}
			EDalsaGenieNanoGigEGainSelector eGainSelector = EDalsaGenieNanoGigEGainSelector_Count;
			
			if(GetGainSelector(&eGainSelector))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("GainSelector"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(!m_pDevice->SetFeatureValue("GainSelector", CStringA(g_lpszDalsaGenieNanoGigEGainSelect[eGainSelector])))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("GainSelector"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}
		}

		int nWidth = 0, nHeight = 0;

		if(!m_pDevice->GetFeatureValue("Width", &nWidth))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Width"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		if(!m_pDevice->GetFeatureValue("Height", &nHeight))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Height"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		if(!nWidth || !nHeight)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("Width or Height"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		bool bColor = false;

		if(!m_pDevice->GetFeatureValue("sensorColorType", serialNumberName, sizeof(serialNumberName)))
			break;
			
		bColor = strcmp(serialNumberName, "Monochrome");

		int nBpp = -1;
		if(!m_pDevice->GetFeatureValue("PixelSize", &nBpp))
			break;

		if(nBpp < 0)
			break;

		int nBufferCount = 2;
		if(GetDeviceBufferCount(&nBufferCount))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("DeviceBufferCount"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		m_pBuffers = new SapBufferWithTrash(nBufferCount, m_pDevice);

		if(!m_pBuffers || !m_pBuffers->Create())
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("device-buffer"));
			eReturn = EDeviceInitializeResult_NotCreateDeviceError;
			if(m_pBuffers)
				delete m_pBuffers;
			m_pBuffers = nullptr;

			bError = true;
			break;
		}

		m_pView = new SapView(m_pBuffers, SapHwndAutomatic);

		if(!m_pView)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("image-buffer"));
			eReturn = EDeviceInitializeResult_NotCreateDeviceError;
			if(m_pView)
				delete m_pView;
			m_pView = nullptr;

			bError = true;
			break;
		}

		m_pXfer = new SapAcqDeviceToBuf(m_pDevice, m_pBuffers, CallbackFunction, this);

		if(!m_pXfer || !m_pXfer->Create())
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("acq-buffer"));
			eReturn = EDeviceInitializeResult_NotCreateDeviceError;
			if(m_pXfer)
				delete m_pXfer;
			m_pXfer = nullptr;

			bError = true;
			break;
		}

		m_pXfer->SetAutoEmpty(TRUE);

		if(bColor)
		{
			m_pColorConv = new SapColorConversion(m_pDevice, m_pBuffers);

			m_pColorConv->SetAcqDevice(m_pDevice);
			m_pColorConv->SetOutputBufferCount(10);

			if(m_pColorConv && !*m_pColorConv && !m_pColorConv->Create())
				break;

			m_pProcessing = new SapMyProcessing(m_pBuffers, m_pColorConv, CallbackProcessing, this);

			if(m_pProcessing && !*m_pProcessing && !m_pProcessing->Create())
				break;

			nBpp = 8;

			if(!m_pColorConv->Enable(true, false))
				break;

			if(!m_pColorConv->SetOutputFormat(SapFormatRGB888))
				break;

			if(m_pColorConv->IsSoftwareEnabled())
				m_pProcessing->Execute();

			int nCount = m_pColorConv->GetOutputBufferCount();

			m_pProcessing->SetAutoEmpty(TRUE);
		}

		if(!bColor)
		{
			int nMaxValue = (1 << nBpp) - 1;

			CMultipleVariable mv;

			for(int i = 0; i < 1; ++i)
				mv.AddValue(nMaxValue);

			this->InitBuffer(nWidth, nHeight, mv, CRavidImage::MakeValueFormat(1, nBpp));
			this->ConnectImage();
		}
		else
		{
			int nMaxValue = (1 << nBpp) - 1;

			CMultipleVariable mv;

			for(int i = 0; i < 3; ++i)
				mv.AddValue(nMaxValue);

			this->InitBuffer(nWidth, nHeight, mv, CRavidImage::MakeValueFormat(3, nBpp));
			this->ConnectImage();
		}

		EDalsaGenieNanoGigELineSelector eLineSelector = EDalsaGenieNanoGigELineSelector_Count;

		if(GetLineSelector(&eLineSelector))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineSelector"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(!m_pDevice->IsFeatureAvailable("LineSelector", &bCommandAvailable))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineSelector"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		if(!bCommandAvailable)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntsupportthe_s), _T("LineSelector"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		if(!m_pDevice->SetFeatureValue("LineSelector", CStringA(g_lpszDalsaGenieNanoGigELineSelector[eLineSelector])))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineSelector"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		bool bLineError = true;

		switch(eLineSelector)
		{
		case EDalsaGenieNanoGigELineSelector_Line1:
			{
				EDalsaGenieNanoGigESwitch eLineSwitch = EDalsaGenieNanoGigESwitch_Count;

				if(GetLine1Inverter(&eLineSwitch))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line1Inverter"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(!m_pDevice->IsFeatureAvailable("LineInverter", &bCommandAvailable))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntsupportthe_s), _T("Line1Inverter"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				if(!bCommandAvailable)
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntsupportthe_s), _T("Line1Inverter"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				if(!m_pDevice->SetFeatureValue("LineInverter", (bool)eLineSwitch))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Line1Inverter"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				int nPeriod = 0;

				if(GetLine1DebouncingPeriod(&nPeriod))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line1DebouncingPeriod"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(!m_pDevice->IsFeatureAvailable("lineDebouncingPeriod", &bCommandAvailable))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntsupportthe_s), _T("Line1DebouncingPeriod"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				if(!bCommandAvailable)
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntsupportthe_s), _T("Line1DebouncingPeriod"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				if(!m_pDevice->SetFeatureValue("lineDebouncingPeriod", nPeriod))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Line1DebouncingPeriod"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}
				
				bLineError = false;
			}
			break;
		case EDalsaGenieNanoGigELineSelector_Line2:
			{
				EDalsaGenieNanoGigESwitch eLineSwitch = EDalsaGenieNanoGigESwitch_Count;

				if(GetLine2Inverter(&eLineSwitch))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line2Inverter"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(!m_pDevice->IsFeatureAvailable("LineInverter", &bCommandAvailable))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntsupportthe_s), _T("Line2Inverter"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				if(!bCommandAvailable)
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntsupportthe_s), _T("Line2Inverter"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				if(!m_pDevice->SetFeatureValue("LineInverter", (bool)eLineSwitch))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Line2Inverter"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				int nPeriod = 0;

				if(GetLine2DebouncingPeriod(&nPeriod))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line2DebouncingPeriod"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(!m_pDevice->IsFeatureAvailable("lineDebouncingPeriod", &bCommandAvailable))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntsupportthe_s), _T("Line2DebouncingPeriod"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				if(!bCommandAvailable)
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntsupportthe_s), _T("Line2DebouncingPeriod"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				if(!m_pDevice->SetFeatureValue("lineDebouncingPeriod", nPeriod))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Line2DebouncingPeriod"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				bLineError = false;
			}
			break;
		case EDalsaGenieNanoGigELineSelector_Line3:
			{
				EDalsaGenieNanoGigELineSource eLineSource = EDalsaGenieNanoGigELineSource_Count;

				if(GetLine3Source(&eLineSource))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line3Source"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(!m_pDevice->IsFeatureAvailable("outputLineSource", &bCommandAvailable))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntsupportthe_s), _T("Line3Source"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				if(!bCommandAvailable)
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntsupportthe_s), _T("Line3Source"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				if(!m_pDevice->SetFeatureValue("outputLineSource", CStringA(g_lpszDalsaGenieNanoGigELineSource[eLineSource])))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Line3Source"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				EDalsaGenieNanoGigESwitch eLineSwitch = EDalsaGenieNanoGigESwitch_Count;

				if(GetLine3Inverter(&eLineSwitch))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line3Inverter"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(!m_pDevice->IsFeatureAvailable("Line3Inverter", &bCommandAvailable))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntsupportthe_s), _T("Line3Inverter"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				if(!bCommandAvailable)
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntsupportthe_s), _T("Line3Inverter"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				if(!m_pDevice->SetFeatureValue("Line3Inverter", (bool)eLineSwitch))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Line3Inverter"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}
				
				if(eLineSource == EDalsaGenieNanoGigELineSource_Input1Event || eLineSource == EDalsaGenieNanoGigELineSource_Input2Event)
				{
					EDalsaGenieNanoGigEActivation eLineActivation = EDalsaGenieNanoGigEActivation_Count;

					if(GetLine3Activation(&eLineActivation))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line3Activation"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(!m_pDevice->IsFeatureAvailable("outputLinePulseActivation", &bCommandAvailable))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntsupportthe_s), _T("Line3Activation"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					if(!bCommandAvailable)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntsupportthe_s), _T("Line3Activation"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					if(!m_pDevice->SetFeatureValue("outputLinePulseActivation", CStringA(g_lpszDalsaGenieNanoGigEActivation[eLineActivation])))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Line3Activation"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}
				}

				if(eLineSource != EDalsaGenieNanoGigELineSource_Off && eLineSource != EDalsaGenieNanoGigELineSource_SoftwareControlled && eLineSource != EDalsaGenieNanoGigELineSource_ExposureActive)
				{
					int nLineDelay = 0;

					if(GetLine3Delay(&nLineDelay))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line3Delay"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(!m_pDevice->IsFeatureAvailable("outputLinePulseDelay", &bCommandAvailable))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntsupportthe_s), _T("Line3Delay"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					if(!bCommandAvailable)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntsupportthe_s), _T("Line3Delay"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					if(!m_pDevice->SetFeatureValue("outputLinePulseDelay", nLineDelay))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Line3Delay"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					int nLineDuration = 0;

					if(GetLine3Duration(&nLineDuration))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line3Duration"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(!m_pDevice->IsFeatureAvailable("outputLinePulseDuration", &bCommandAvailable))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntsupportthe_s), _T("Line3Duration"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					if(!bCommandAvailable)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntsupportthe_s), _T("Line3Duration"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					if(!m_pDevice->SetFeatureValue("outputLinePulseDuration", nLineDuration))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Line3Duration"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}
				}


				if(eLineSource == EDalsaGenieNanoGigELineSource_SoftwareControlled)
				{
					EDalsaGenieNanoGigELineValue eLineValue = EDalsaGenieNanoGigELineValue_Count;

					if(GetLine3Value(&eLineValue))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line3Value"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(!m_pDevice->IsFeatureAvailable("outputLineValue", &bCommandAvailable))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntsupportthe_s), _T("Line3Value"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					if(!bCommandAvailable)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntsupportthe_s), _T("Line3Value"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					if(!m_pDevice->SetFeatureValue("outputLineValue", CStringA(g_lpszDalsaGenieNanoGigELineValue[eLineValue])))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Line3Value"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}
				}

				bLineError = false;
			}
			break;
		case EDalsaGenieNanoGigELineSelector_Line4:
			{
				EDalsaGenieNanoGigELineSource eLineSource = EDalsaGenieNanoGigELineSource_Count;

				if(GetLine4Source(&eLineSource))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line4Source"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(!m_pDevice->IsFeatureAvailable("outputLineSource", &bCommandAvailable))
				{

					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntsupportthe_s), _T("Line4Source"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				if(!bCommandAvailable)
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntsupportthe_s), _T("Line4Source"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				if(!m_pDevice->SetFeatureValue("outputLineSource", CStringA(g_lpszDalsaGenieNanoGigELineSource[eLineSource])))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Line4Source"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				EDalsaGenieNanoGigESwitch eLineSwitch = EDalsaGenieNanoGigESwitch_Count;

				if(GetLine4Inverter(&eLineSwitch))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line4Inverter"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(!m_pDevice->IsFeatureAvailable("Line4Inverter", &bCommandAvailable))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntsupportthe_s), _T("Line4Inverter"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				if(!bCommandAvailable)
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntsupportthe_s), _T("Line4Inverter"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				if(!m_pDevice->SetFeatureValue("Line4Inverter", (bool)eLineSwitch))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line4Inverter"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}
				
				if(eLineSource == EDalsaGenieNanoGigELineSource_Input1Event || eLineSource == EDalsaGenieNanoGigELineSource_Input2Event)
				{
					EDalsaGenieNanoGigEActivation eLineActivation = EDalsaGenieNanoGigEActivation_Count;

					if(GetLine4Activation(&eLineActivation))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line4Activation"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(!m_pDevice->IsFeatureAvailable("outputLinePulseActivation", &bCommandAvailable))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntsupportthe_s), _T("Line4Activation"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					if(!bCommandAvailable)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntsupportthe_s), _T("Line4Activation"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					if(!m_pDevice->SetFeatureValue("outputLinePulseActivation", CStringA(g_lpszDalsaGenieNanoGigEActivation[eLineActivation])))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Line4Activation"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}
				}

				if(eLineSource != EDalsaGenieNanoGigELineSource_Off && eLineSource != EDalsaGenieNanoGigELineSource_SoftwareControlled && eLineSource != EDalsaGenieNanoGigELineSource_ExposureActive)
				{
					int nLineDelay = 0;

					if(GetLine4Delay(&nLineDelay))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line4Delay"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(!m_pDevice->IsFeatureAvailable("outputLinePulseDelay", &bCommandAvailable))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntsupportthe_s), _T("Line4Delay"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					if(!bCommandAvailable)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntsupportthe_s), _T("Line4Delay"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					if(!m_pDevice->SetFeatureValue("outputLinePulseDelay", nLineDelay))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Line4Delay"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					int nLineDuration = 0;

					if(GetLine4Duration(&nLineDuration))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line2Inverter"));
						strMessage.Format(_T("Couldn't read 'Line4Duration' from the database"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(!m_pDevice->IsFeatureAvailable("outputLinePulseDuration", &bCommandAvailable))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntsupportthe_s), _T("Line4Duration"));
						strMessage.Format(_T("Failed to send command 'Line4Duration'"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					if(!bCommandAvailable)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntsupportthe_s), _T("Line4Duration"));
						strMessage.Format(_T("Failed to send command 'Line4Duration'"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					if(!m_pDevice->SetFeatureValue("outputLinePulseDuration", nLineDuration))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Line4Duration"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}
				}


				if(eLineSource == EDalsaGenieNanoGigELineSource_SoftwareControlled)
				{
					EDalsaGenieNanoGigELineValue eLineValue = EDalsaGenieNanoGigELineValue_Count;

					if(GetLine4Value(&eLineValue))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Line4Value"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					if(!m_pDevice->IsFeatureAvailable("outputLineValue", &bCommandAvailable))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntsupportthe_s), _T("Line4Value"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					if(!bCommandAvailable)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntsupportthe_s), _T("Line4Value"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

					if(!m_pDevice->SetFeatureValue("outputLineValue", CStringA(g_lpszDalsaGenieNanoGigELineValue[eLineValue])))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Line4Value"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}
				}

				bLineError = false;
			}
			break;
		default:
			break;
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

EDeviceTerminateResult CDeviceDalsaGenieNanoGigE::Terminate()
{
	EDeviceTerminateResult eReturn = EDeviceTerminateResult_UnknownError;

	CString strMessage;

	SetStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	COperatingSystemInfo osInfo;

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("DalsaGenieNano"));

		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);
		
		return EDeviceTerminateResult_NotFoundApiError;
	}

	do
	{
		if(IsLive() || !IsGrabAvailable())
			Stop();

		m_bIsInitialized = false;
		m_bIsGrabAvailable = true;

		if(m_pXfer && *m_pXfer)
			m_pXfer->Destroy();

		if(m_pView && *m_pView)
			m_pView->Destroy();

		if(m_pProcessing && *m_pProcessing)
			m_pProcessing->Destroy();

		if(m_pColorConv && *m_pColorConv)
			m_pColorConv->Destroy();

		if(m_pBuffers && *m_pBuffers)
			m_pBuffers->Destroy();

		if(m_pDevice && *m_pDevice)
			m_pDevice->Destroy();

		if(m_pColorConv)
			delete m_pColorConv;
		m_pColorConv = nullptr;

		if(m_pXfer)
			delete m_pXfer;
		m_pXfer = nullptr;

		if(m_pProcessing)
			delete m_pProcessing;
		m_pProcessing = nullptr;

		if(m_pView)
			delete m_pView;
		m_pView = nullptr;

		if(m_pBuffers)
			delete m_pBuffers;
		m_pBuffers = nullptr;

		if(m_pDevice)
			delete m_pDevice;
		m_pDevice = nullptr;

		strMessage = CMultiLanguageManager::GetString(ELanguageParameter_TheDevicehasbeensuccessfullyterminated);

		CEventHandlerManager::BroadcastOnDeviceTerminated(this);
	}
	while(false);

	this->ClearBuffer();

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceDalsaGenieNanoGigE::LoadSettings()
{
	bool bReturn = false;

	m_vctParameterFieldConfigurations.clear();

	do
	{
		CString strTime;
		strTime.Format(_T("%d"), INT_MAX);
		
		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_DeviceID, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_DeviceID], _T("0"), EParameterFieldType_Edit);
		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_CameraSetting, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_CameraSetting], _T("1"), EParameterFieldType_None);
		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_GrabCount, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_GrabCount], _T("1"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_CanvasWidth, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_CanvasWidth], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_CanvasHeight, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_CanvasHeight], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_OffsetX, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_OffsetX], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_OffsetY, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_OffsetY], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_FlipHorizontal, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_FlipHorizontal], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_FlipVertical, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_FlipVertical], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_PixelFormat, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_PixelFormat], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDalsaGenieNanoGigEPixelFormat, EDalsaGenieNanoGigEPixelFormat_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_PacketSize, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_PacketSize], _T("1500"), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_DeviceBufferCount, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_DeviceBufferCount], _T("2"), EParameterFieldType_Edit, nullptr, _T("1 < Mono \n10 < Color"), 1);


		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_TriggerSetting, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_TriggerSetting], _T("1"), EParameterFieldType_None);
		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_TriggerSelector, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_TriggerSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDalsaGenieNanoGigETriggerSelect, EDalsaGenieNanoGigETriggerSelector_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_FrameStart, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_FrameStart], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_FrameStartTriggerMode, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_FrameStartTriggerMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDalsaGenieNanoGigESwitch, EDalsaGenieNanoGigESwitch_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_FrameStartTriggerSource, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_FrameStartTriggerSource], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDalsaGenieNanoGigESource, EDalsaGenieNanoGigESource_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_FrameStartTriggerActivation, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_FrameStartTriggerActivation], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDalsaGenieNanoGigEActivation, EDalsaGenieNanoGigEActivation_Count), nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_FrameBurstStart, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_FrameBurstStart], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_FrameBurstStartTriggerMode, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_FrameBurstStartTriggerMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDalsaGenieNanoGigESwitch, EDalsaGenieNanoGigESwitch_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_FrameBurstStartTriggerSource, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_FrameBurstStartTriggerSource], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDalsaGenieNanoGigESource, EDalsaGenieNanoGigESource_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_FrameBurstStartTriggerCount, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_FrameBurstStartTriggerCount], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_AcquisitionStart, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_AcquisitionStart], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_AcquisitionStartTriggerMode, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_AcquisitionStartTriggerMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDalsaGenieNanoGigESwitch, EDalsaGenieNanoGigESwitch_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_AcquisitionStartTriggerSource, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_AcquisitionStartTriggerSource], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDalsaGenieNanoGigESource, EDalsaGenieNanoGigESource_Count), nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_ExposureAndGainSetting, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_ExposureAndGainSetting], _T("1"), EParameterFieldType_None);


		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_FrameRateControl, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_FrameRateControl], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDalsaGenieNanoGigEFrameRateControl, EDalsaGenieNanoGigEFrameRateControl_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_FrameRate, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_FrameRate], _T("10.000000"), EParameterFieldType_Edit, nullptr, nullptr, 1);



		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_ExposureTime, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_ExposureTime], _T("50"), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_GainSelector, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_GainSelector], _T("1"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDalsaGenieNanoGigEGainSelect, EDalsaGenieNanoGigEGainSelector_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_SensorGain, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_SensorGain], _T("0.000000"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_DigitalGain, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_DigitalGain], _T("0.000000"), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_BlackLevelSelector, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_BlackLevelSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDalsaGenieNanoGigEBlackSelector, EDalsaGenieNanoGigEBlackLevelSelector_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_BlackLevel, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_BlackLevel], _T("0.000000"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		
		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_LineSelector, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_LineSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDalsaGenieNanoGigELineSelector, EDalsaGenieNanoGigELineSelector_Count), nullptr, 0);

		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_Line1, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_Line1], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_Line1DebouncingPeriod, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_Line1DebouncingPeriod], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_Line1Inverter, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_Line1Inverter], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDalsaGenieNanoGigESwitch, EDalsaGenieNanoGigESwitch_Count), nullptr, 1);
		
		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_Line2, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_Line2], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_Line2DebouncingPeriod, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_Line2DebouncingPeriod], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_Line2Inverter, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_Line2Inverter], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDalsaGenieNanoGigESwitch, EDalsaGenieNanoGigESwitch_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_Line3, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_Line3], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_Line3Source, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_Line3Source], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDalsaGenieNanoGigELineSource, EDalsaGenieNanoGigELineSource_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_Line3Inverter, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_Line3Inverter], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDalsaGenieNanoGigESwitch, EDalsaGenieNanoGigESwitch_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_Line3Activation, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_Line3Activation], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDalsaGenieNanoGigEActivation, EDalsaGenieNanoGigEActivation_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_Line3Delay, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_Line3Delay], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_Line3Duration, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_Line3Duration], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_Line3Value, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_Line3Value], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDalsaGenieNanoGigELineValue, EDalsaGenieNanoGigELineValue_Count), nullptr, 1);
		
		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_Line4, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_Line4], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_Line4Source, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_Line4Source], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDalsaGenieNanoGigELineSource, EDalsaGenieNanoGigELineSource_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_Line4Inverter, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_Line4Inverter], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDalsaGenieNanoGigESwitch, EDalsaGenieNanoGigESwitch_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_Line4Activation, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_Line4Activation], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDalsaGenieNanoGigEActivation, EDalsaGenieNanoGigEActivation_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_Line4Delay, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_Line4Delay], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_Line4Duration, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_Line4Duration], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterDalsaGenieNanoGigE_Line4Value, g_lpszParamDalsaGenieNanoGigE[EDeviceParameterDalsaGenieNanoGigE_Line4Value], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDalsaGenieNanoGigELineValue, EDalsaGenieNanoGigELineValue_Count), nullptr, 1);
		
		bReturn = true;
	}
	while(false);

	return bReturn & __super::LoadSettings();
}

EDeviceGrabResult CDeviceDalsaGenieNanoGigE::Grab()
{
	EDeviceGrabResult eReturn = EDeviceGrabResult_UnknownError;

	CString strMessage;

	do
	{
		m_nGrabCount = 0;

		unsigned int nRequestGrabCount = 0;

		if(!IsInitialized())
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Ithasnotbeeninitializedyet);
			eReturn = EDeviceGrabResult_NotInitializedError;
			break;
		}

		if(IsLive() || !IsGrabAvailable())
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Ithasbeenlivingorgrabbing);
			eReturn = EDeviceGrabResult_AlreadyGrabError;
			break;
		}
		
		EDalsaGenieNanoGigETriggerSelector eTriggerSelect = EDalsaGenieNanoGigETriggerSelector_Count;

		if(GetTriggerSelector(&eTriggerSelect))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Trigger Selector"));
			eReturn = EDeviceGrabResult_ReadOnDatabaseError;
			break;
		}

		bool bSetCount = false;

		if(eTriggerSelect == EDalsaGenieNanoGigETriggerSelector_FrameBurstStart)
		{
			EDalsaGenieNanoGigESwitch eTriggerMode = EDalsaGenieNanoGigESwitch_Count;
			
			if(GetFrameBurstStartMode(&eTriggerMode))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("FrameBurstStar"));
				eReturn = EDeviceGrabResult_ReadOnDatabaseError;
				break;
			}

			if(eTriggerMode == EDalsaGenieNanoGigESwitch_On)
			{
				int nParam = 0;

				if(GetFrameBurstStartTriggerCount(&nParam))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("FrameBurstStartTriggerCount"));
					eReturn = EDeviceGrabResult_ReadOnDatabaseError;
					break;
				}

				m_nGrabCount = nParam;

				bSetCount = true;
			}
		}

		if(!bSetCount)
		{
			if(GetGrabCount(&m_nGrabCount))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Grab Count"));
				eReturn = EDeviceGrabResult_ReadOnDatabaseError;
				break;
			}
		}		

		m_pXfer->Snap(m_nGrabCount);

		m_bIsGrabAvailable = false;

		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("Grab"));

		eReturn = EDeviceGrabResult_OK;
	}
	while(false);

	return eReturn;
}

EDeviceLiveResult CDeviceDalsaGenieNanoGigE::Live()
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

		m_nGrabCount = INT_MAX;

		m_pXfer->Snap(-1);

		m_bIsLive = true;

		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("Live"));

		eReturn = EDeviceLiveResult_OK;
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceStopResult CDeviceDalsaGenieNanoGigE::Stop()
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

		if(m_pXfer->Freeze())
			m_pXfer->Abort();

		m_bIsLive = false;

		m_bIsGrabAvailable = true;

		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("Stop"));

		eReturn = EDeviceStopResult_OK;
	} 
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceTriggerResult CDeviceDalsaGenieNanoGigE::Trigger()
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

		if(!m_pDevice->SetFeatureValue("TriggerSoftware", true))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TriggerSoftware"));
			eReturn = EDeviceTriggerResult_WriteToDeviceError;
			break;
		}

		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("Trigger"));

		eReturn = EDeviceTriggerResult_OK;
	} 
	while (false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDalsaGenieNanoGigEGetFunction CDeviceDalsaGenieNanoGigE::GetGrabCount(int* pParam)
{
	EDalsaGenieNanoGigEGetFunction eReturn = EDalsaGenieNanoGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterDalsaGenieNanoGigE_GrabCount));

		eReturn = EDalsaGenieNanoGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDalsaGenieNanoGigESetFunction CDeviceDalsaGenieNanoGigE::SetGrabCount(int nParam)
{
	EDalsaGenieNanoGigESetFunction eReturn = EDalsaGenieNanoGigESetFunction_UnknownError;
	
	EDeviceParameterDalsaGenieNanoGigE eSaveID = EDeviceParameterDalsaGenieNanoGigE_GrabCount;

	int nPreValue = _ttoi(GetParamValue(eSaveID));
	
	do
	{
		if(IsLive() || !IsGrabAvailable())
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDalsaGenieNanoGigESetFunction_OK;
	}
	while(false);
	
	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDalsaGenieNanoGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDalsaGenieNanoGigEGetFunction CDeviceDalsaGenieNanoGigE::GetCanvasWidth(int* pParam)
{
	EDalsaGenieNanoGigEGetFunction eReturn = EDalsaGenieNanoGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterDalsaGenieNanoGigE_CanvasWidth));

		eReturn = EDalsaGenieNanoGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDalsaGenieNanoGigESetFunction CDeviceDalsaGenieNanoGigE::SetCanvasWidth(int nParam)
{
	EDalsaGenieNanoGigESetFunction eReturn = EDalsaGenieNanoGigESetFunction_UnknownError;

	EDeviceParameterDalsaGenieNanoGigE eSaveID = EDeviceParameterDalsaGenieNanoGigE_CanvasWidth;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDalsaGenieNanoGigESetFunction_InitializedDeviceError;
			break;
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDalsaGenieNanoGigESetFunction_OK;
	}
	while(false);
	
	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDalsaGenieNanoGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDalsaGenieNanoGigEGetFunction CDeviceDalsaGenieNanoGigE::GetCanvasHeight(int* pParam)
{
	EDalsaGenieNanoGigEGetFunction eReturn = EDalsaGenieNanoGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterDalsaGenieNanoGigE_CanvasHeight));

		eReturn = EDalsaGenieNanoGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDalsaGenieNanoGigESetFunction CDeviceDalsaGenieNanoGigE::SetCanvasHeight(int nParam)
{
	EDalsaGenieNanoGigESetFunction eReturn = EDalsaGenieNanoGigESetFunction_UnknownError;

	EDeviceParameterDalsaGenieNanoGigE eSaveID = EDeviceParameterDalsaGenieNanoGigE_CanvasHeight;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDalsaGenieNanoGigESetFunction_InitializedDeviceError;
			break;
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDalsaGenieNanoGigESetFunction_OK;
	}
	while(false);
	
	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDalsaGenieNanoGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDalsaGenieNanoGigEGetFunction CDeviceDalsaGenieNanoGigE::GetOffsetX(int* pParam)
{
	EDalsaGenieNanoGigEGetFunction eReturn = EDalsaGenieNanoGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterDalsaGenieNanoGigE_OffsetX));

		eReturn = EDalsaGenieNanoGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDalsaGenieNanoGigESetFunction CDeviceDalsaGenieNanoGigE::SetOffsetX(int nParam)
{
	EDalsaGenieNanoGigESetFunction eReturn = EDalsaGenieNanoGigESetFunction_UnknownError;

	EDeviceParameterDalsaGenieNanoGigE eSaveID = EDeviceParameterDalsaGenieNanoGigE_OffsetX;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			BOOL bCommandAvailable = false;

			if(!m_pDevice->IsFeatureAvailable("OffsetX", &bCommandAvailable))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_NotSupportedError;
				break;
			}

			if(!bCommandAvailable)
			{
				eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(!m_pDevice->SetFeatureValue("OffsetX", nParam))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strOffset;
		strOffset.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strOffset))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDalsaGenieNanoGigESetFunction_OK;
	}
	while(false);
	
	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDalsaGenieNanoGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDalsaGenieNanoGigEGetFunction CDeviceDalsaGenieNanoGigE::GetOffsetY(int* pParam)
{
	EDalsaGenieNanoGigEGetFunction eReturn = EDalsaGenieNanoGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterDalsaGenieNanoGigE_OffsetY));

		eReturn = EDalsaGenieNanoGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDalsaGenieNanoGigESetFunction CDeviceDalsaGenieNanoGigE::SetOffsetY(int nParam)
{
	EDalsaGenieNanoGigESetFunction eReturn = EDalsaGenieNanoGigESetFunction_UnknownError;

	EDeviceParameterDalsaGenieNanoGigE eSaveID = EDeviceParameterDalsaGenieNanoGigE_OffsetY;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			BOOL bCommandAvailable = false;

			if(!m_pDevice->IsFeatureAvailable("OffsetY", &bCommandAvailable))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_NotSupportedError;
				break;
			}

			if(!bCommandAvailable)
			{
				eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(!m_pDevice->SetFeatureValue("OffsetY", nParam))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strOffset;
		strOffset.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strOffset))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDalsaGenieNanoGigESetFunction_OK;
	}
	while(false);
	
	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDalsaGenieNanoGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDalsaGenieNanoGigEGetFunction CDeviceDalsaGenieNanoGigE::GetFlipHorizontal(bool* pParam)
{
	EDalsaGenieNanoGigEGetFunction eReturn = EDalsaGenieNanoGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterDalsaGenieNanoGigE_FlipHorizontal));

		eReturn = EDalsaGenieNanoGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDalsaGenieNanoGigESetFunction CDeviceDalsaGenieNanoGigE::SetFlipHorizontal(bool bParam)
{
	EDalsaGenieNanoGigESetFunction eReturn = EDalsaGenieNanoGigESetFunction_UnknownError;

	EDeviceParameterDalsaGenieNanoGigE eSaveID = EDeviceParameterDalsaGenieNanoGigE_FlipHorizontal;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			BOOL bCommandAvailable = false;

			if(!m_pDevice->IsFeatureAvailable("ReverseX", &bCommandAvailable))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_NotSupportedError;
				break;
			}

			if(!bCommandAvailable)
			{
				eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(!m_pDevice->SetFeatureValue("ReverseX", bParam))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDalsaGenieNanoGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDalsaGenieNanoGigE[eSaveID], g_lpszDalsaGenieNanoGigESwitch[nPreValue], g_lpszDalsaGenieNanoGigESwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDalsaGenieNanoGigEGetFunction CDeviceDalsaGenieNanoGigE::GetFlipVertical(bool* pParam)
{
	EDalsaGenieNanoGigEGetFunction eReturn = EDalsaGenieNanoGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterDalsaGenieNanoGigE_FlipVertical));

		eReturn = EDalsaGenieNanoGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDalsaGenieNanoGigESetFunction CDeviceDalsaGenieNanoGigE::SetFlipVertical(bool bParam)
{
	EDalsaGenieNanoGigESetFunction eReturn = EDalsaGenieNanoGigESetFunction_UnknownError;

	EDeviceParameterDalsaGenieNanoGigE eSaveID = EDeviceParameterDalsaGenieNanoGigE_FlipVertical;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			BOOL bCommandAvailable = false;

			if(!m_pDevice->IsFeatureAvailable("ReverseY", &bCommandAvailable))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_NotSupportedError;
				break;
			}

			if(!bCommandAvailable)
			{
				eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(!m_pDevice->SetFeatureValue("ReverseY", bParam))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveID, strValue))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDalsaGenieNanoGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDalsaGenieNanoGigE[eSaveID], g_lpszDalsaGenieNanoGigESwitch[nPreValue], g_lpszDalsaGenieNanoGigESwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDalsaGenieNanoGigEGetFunction CDeviceDalsaGenieNanoGigE::GetPixelFormat(EDalsaGenieNanoGigEPixelFormat* pParam)
{
	EDalsaGenieNanoGigEGetFunction eReturn = EDalsaGenieNanoGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterDalsaGenieNanoGigE_PixelFormat));

		if(nData < 0 || nData >= (int)EDalsaGenieNanoGigEPixelFormat_Count)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDalsaGenieNanoGigEPixelFormat)nData;

		eReturn = EDalsaGenieNanoGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDalsaGenieNanoGigESetFunction CDeviceDalsaGenieNanoGigE::SetPixelFormat(EDalsaGenieNanoGigEPixelFormat eParam)
{
	EDalsaGenieNanoGigESetFunction eReturn = EDalsaGenieNanoGigESetFunction_UnknownError;

	EDeviceParameterDalsaGenieNanoGigE eSaveID = EDeviceParameterDalsaGenieNanoGigE_PixelFormat;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDalsaGenieNanoGigEPixelFormat)0 || eParam >= EDalsaGenieNanoGigEPixelFormat_Count)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotSupportedError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EDalsaGenieNanoGigESetFunction_InitializedDeviceError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDalsaGenieNanoGigESetFunction_OK;
	}
	while(false);
	
	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDalsaGenieNanoGigE[eSaveID], g_lpszDalsaGenieNanoGigEPixelFormat[nPreValue], g_lpszDalsaGenieNanoGigEPixelFormat[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDalsaGenieNanoGigEGetFunction CDeviceDalsaGenieNanoGigE::GetPacketSize(int* pParam)
{
	EDalsaGenieNanoGigEGetFunction eReturn = EDalsaGenieNanoGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterDalsaGenieNanoGigE_PacketSize));

		eReturn = EDalsaGenieNanoGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDalsaGenieNanoGigESetFunction CDeviceDalsaGenieNanoGigE::SetPacketSize(int nParam)
{
	EDalsaGenieNanoGigESetFunction eReturn = EDalsaGenieNanoGigESetFunction_UnknownError;

	EDeviceParameterDalsaGenieNanoGigE eSaveID = EDeviceParameterDalsaGenieNanoGigE_PacketSize;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			BOOL bCommandAvailable = false;

			if(!m_pDevice->IsFeatureAvailable("GevSCPSPacketSize", &bCommandAvailable))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_NotSupportedError;
				break;
			}

			if(!bCommandAvailable)
			{
				eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(!m_pDevice->SetFeatureValue("GevSCPSPacketSize", (INT64)nParam))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strOffset;
		strOffset.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strOffset))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDalsaGenieNanoGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDalsaGenieNanoGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDalsaGenieNanoGigEGetFunction CDeviceDalsaGenieNanoGigE::GetDeviceBufferCount(int* pParam)
{
	EDalsaGenieNanoGigEGetFunction eReturn = EDalsaGenieNanoGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterDalsaGenieNanoGigE_DeviceBufferCount));

		eReturn = EDalsaGenieNanoGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDalsaGenieNanoGigESetFunction CDeviceDalsaGenieNanoGigE::SetDeviceBufferCount(int nParam)
{
	EDalsaGenieNanoGigESetFunction eReturn = EDalsaGenieNanoGigESetFunction_UnknownError;

	EDeviceParameterDalsaGenieNanoGigE eSaveID = EDeviceParameterDalsaGenieNanoGigE_DeviceBufferCount;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EDalsaGenieNanoGigESetFunction_InitializedDeviceError;
			break;
		}

		CString strOffset;
		strOffset.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strOffset))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDalsaGenieNanoGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDalsaGenieNanoGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDalsaGenieNanoGigEGetFunction CDeviceDalsaGenieNanoGigE::GetTriggerSelector(EDalsaGenieNanoGigETriggerSelector* pParam)
{
	EDalsaGenieNanoGigEGetFunction eReturn = EDalsaGenieNanoGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterDalsaGenieNanoGigE_TriggerSelector));

		if(nData < 0 || nData >= (int)EDalsaGenieNanoGigETriggerSelector_Count)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDalsaGenieNanoGigETriggerSelector)nData;

		eReturn = EDalsaGenieNanoGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDalsaGenieNanoGigESetFunction CDeviceDalsaGenieNanoGigE::SetTriggerSelector(EDalsaGenieNanoGigETriggerSelector eParam)
{
	EDalsaGenieNanoGigESetFunction eReturn = EDalsaGenieNanoGigESetFunction_UnknownError;

	EDeviceParameterDalsaGenieNanoGigE eSaveID = EDeviceParameterDalsaGenieNanoGigE_TriggerSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDalsaGenieNanoGigETriggerSelector)0 || eParam >= EDalsaGenieNanoGigETriggerSelector_Count)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			BOOL bCommandAvailable = false;

			if(!m_pDevice->IsFeatureAvailable("TriggerSelector", &bCommandAvailable))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_NotSupportedError;
				break;
			}

			if(!bCommandAvailable)
			{
				eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(!m_pDevice->SetFeatureValue("TriggerSelector", CStringA(g_lpszDalsaGenieNanoGigETriggerSelect[eParam])))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDalsaGenieNanoGigESetFunction_OK;
	}
	while(false);
	
	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDalsaGenieNanoGigE[eSaveID], g_lpszDalsaGenieNanoGigETriggerSelect[nPreValue], g_lpszDalsaGenieNanoGigETriggerSelect[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDalsaGenieNanoGigEGetFunction CDeviceDalsaGenieNanoGigE::GetFrameStartMode(EDalsaGenieNanoGigESwitch* pParam)
{
	EDalsaGenieNanoGigEGetFunction eReturn = EDalsaGenieNanoGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterDalsaGenieNanoGigE_FrameStartTriggerMode));

		if(nData < 0 || nData >= (int)EDalsaGenieNanoGigESwitch_Count)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDalsaGenieNanoGigESwitch)nData;

		eReturn = EDalsaGenieNanoGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDalsaGenieNanoGigESetFunction CDeviceDalsaGenieNanoGigE::SetFrameStartMode(EDalsaGenieNanoGigESwitch eParam)
{
	EDalsaGenieNanoGigESetFunction eReturn = EDalsaGenieNanoGigESetFunction_UnknownError;

	EDeviceParameterDalsaGenieNanoGigE eSaveID = EDeviceParameterDalsaGenieNanoGigE_FrameStartTriggerMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDalsaGenieNanoGigESwitch)0 || eParam >= EDalsaGenieNanoGigESwitch_Count)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotFindCommandError;
			break;
		}

		EDalsaGenieNanoGigETriggerSelector eTrigSelect = EDalsaGenieNanoGigETriggerSelector_Count;

		if(GetTriggerSelector(&eTrigSelect))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDatabaseError;

			break;
		}

		if(eTrigSelect != EDalsaGenieNanoGigETriggerSelector_FrameStart)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotSelectFeatureError;

			break;
		}
		
		if(IsInitialized())
		{
			BOOL bCommandAvailable = false;

			if(!m_pDevice->IsFeatureAvailable("TriggerMode", &bCommandAvailable))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_NotSupportedError;
				break;
			}

			if(!bCommandAvailable)
			{
				eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(!m_pDevice->SetFeatureValue("TriggerMode", CStringA(g_lpszDalsaGenieNanoGigESwitch[eParam])))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDalsaGenieNanoGigESetFunction_OK;
	}
	while(false);
	
	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDalsaGenieNanoGigE[eSaveID], g_lpszDalsaGenieNanoGigESwitch[nPreValue], g_lpszDalsaGenieNanoGigESwitch[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDalsaGenieNanoGigEGetFunction CDeviceDalsaGenieNanoGigE::GetFrameStartTriggerSource(EDalsaGenieNanoGigESource* pParam)
{
	EDalsaGenieNanoGigEGetFunction eReturn = EDalsaGenieNanoGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterDalsaGenieNanoGigE_FrameStartTriggerSource));

		if(nData < 0 || nData >= (int)EDalsaGenieNanoGigESource_Count)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDalsaGenieNanoGigESource)nData;

		eReturn = EDalsaGenieNanoGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDalsaGenieNanoGigESetFunction CDeviceDalsaGenieNanoGigE::SetFrameStartTriggerSource(EDalsaGenieNanoGigESource eParam)
{
	EDalsaGenieNanoGigESetFunction eReturn = EDalsaGenieNanoGigESetFunction_UnknownError;

	EDeviceParameterDalsaGenieNanoGigE eSaveID = EDeviceParameterDalsaGenieNanoGigE_FrameStartTriggerSource;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDalsaGenieNanoGigESource)0 || eParam >= EDalsaGenieNanoGigESource_Count)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotFindCommandError;
			break;
		}

		EDalsaGenieNanoGigETriggerSelector eTrigSelect = EDalsaGenieNanoGigETriggerSelector_Count;

		if(GetTriggerSelector(&eTrigSelect))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDatabaseError;

			break;
		}

		if(eTrigSelect != EDalsaGenieNanoGigETriggerSelector_FrameStart)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotSelectFeatureError;

			break;
		}

		if(IsInitialized())
		{
			BOOL bCommandAvailable = false;

			if(!m_pDevice->IsFeatureAvailable("TriggerSource", &bCommandAvailable))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_NotSupportedError;
				break;
			}

			if(!bCommandAvailable)
			{
				eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(!m_pDevice->SetFeatureValue("TriggerSource", CStringA(g_lpszDalsaGenieNanoGigESource[eParam])))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDalsaGenieNanoGigESetFunction_OK;
	}
	while(false);
	
	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDalsaGenieNanoGigE[eSaveID], g_lpszDalsaGenieNanoGigESource[nPreValue], g_lpszDalsaGenieNanoGigESource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDalsaGenieNanoGigEGetFunction CDeviceDalsaGenieNanoGigE::GetFrameStartTriggerActivation(EDalsaGenieNanoGigEActivation* pParam)
{
	EDalsaGenieNanoGigEGetFunction eReturn = EDalsaGenieNanoGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterDalsaGenieNanoGigE_FrameStartTriggerActivation));

		if(nData < 0 || nData >= (int)EDalsaGenieNanoGigEActivation_Count)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDalsaGenieNanoGigEActivation)nData;

		eReturn = EDalsaGenieNanoGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDalsaGenieNanoGigESetFunction CDeviceDalsaGenieNanoGigE::SetFrameStartTriggerActivation(EDalsaGenieNanoGigEActivation eParam)
{
	EDalsaGenieNanoGigESetFunction eReturn = EDalsaGenieNanoGigESetFunction_UnknownError;

	EDeviceParameterDalsaGenieNanoGigE eSaveID = EDeviceParameterDalsaGenieNanoGigE_FrameStartTriggerActivation;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDalsaGenieNanoGigEActivation)0 || eParam >= EDalsaGenieNanoGigEActivation_Count)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotFindCommandError;
			break;
		}

		EDalsaGenieNanoGigETriggerSelector eTrigSelect = EDalsaGenieNanoGigETriggerSelector_Count;

		if(GetTriggerSelector(&eTrigSelect))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDatabaseError;

			break;
		}

		if(eTrigSelect != EDalsaGenieNanoGigETriggerSelector_FrameStart)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotSelectFeatureError;

			break;
		}

		if(IsInitialized())
		{
			BOOL bCommandAvailable = false;

			if(!m_pDevice->IsFeatureAvailable("TriggerActivation", &bCommandAvailable))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_NotSupportedError;
				break;
			}

			if(!bCommandAvailable)
			{
				eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(!m_pDevice->SetFeatureValue("TriggerActivation", CStringA(g_lpszDalsaGenieNanoGigEActivation[eParam])))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDalsaGenieNanoGigESetFunction_OK;
	}
	while(false);
	
	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDalsaGenieNanoGigE[eSaveID], g_lpszDalsaGenieNanoGigEActivation[nPreValue], g_lpszDalsaGenieNanoGigEActivation[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDalsaGenieNanoGigEGetFunction CDeviceDalsaGenieNanoGigE::GetFrameBurstStartMode(EDalsaGenieNanoGigESwitch* pParam)
{
	EDalsaGenieNanoGigEGetFunction eReturn = EDalsaGenieNanoGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterDalsaGenieNanoGigE_FrameBurstStartTriggerMode));

		if(nData < 0 || nData >= (int)EDalsaGenieNanoGigESwitch_Count)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDalsaGenieNanoGigESwitch)nData;

		eReturn = EDalsaGenieNanoGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDalsaGenieNanoGigESetFunction CDeviceDalsaGenieNanoGigE::SetFrameBurstStartMode(EDalsaGenieNanoGigESwitch eParam)
{
	EDalsaGenieNanoGigESetFunction eReturn = EDalsaGenieNanoGigESetFunction_UnknownError;

	EDeviceParameterDalsaGenieNanoGigE eSaveID = EDeviceParameterDalsaGenieNanoGigE_FrameBurstStartTriggerMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDalsaGenieNanoGigESwitch)0 || eParam >= EDalsaGenieNanoGigESwitch_Count)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotFindCommandError;
			break;
		}

		EDalsaGenieNanoGigETriggerSelector eTrigSelect = EDalsaGenieNanoGigETriggerSelector_Count;

		if(GetTriggerSelector(&eTrigSelect))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDatabaseError;

			break;
		}

		if(eTrigSelect != EDalsaGenieNanoGigETriggerSelector_FrameBurstStart)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotSelectFeatureError;

			break;
		}

		if(IsInitialized())
		{
			BOOL bCommandAvailable = false;

			if(!m_pDevice->IsFeatureAvailable("TriggerMode", &bCommandAvailable))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_NotSupportedError;
				break;
			}

			if(!bCommandAvailable)
			{
				eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(!m_pDevice->SetFeatureValue("TriggerMode", CStringA(g_lpszDalsaGenieNanoGigESwitch[eParam])))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDalsaGenieNanoGigESetFunction_OK;
	}
	while(false);
	
	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDalsaGenieNanoGigE[eSaveID], g_lpszDalsaGenieNanoGigESwitch[nPreValue], g_lpszDalsaGenieNanoGigESwitch[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDalsaGenieNanoGigEGetFunction CDeviceDalsaGenieNanoGigE::GetFrameBurstStartTriggerSource(EDalsaGenieNanoGigESource* pParam)
{
	EDalsaGenieNanoGigEGetFunction eReturn = EDalsaGenieNanoGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterDalsaGenieNanoGigE_FrameBurstStartTriggerSource));

		if(nData < 0 || nData >= (int)EDalsaGenieNanoGigESource_Count)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDalsaGenieNanoGigESource)nData;

		eReturn = EDalsaGenieNanoGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDalsaGenieNanoGigESetFunction CDeviceDalsaGenieNanoGigE::SetFrameBurstStartTriggerSource(EDalsaGenieNanoGigESource eParam)
{
	EDalsaGenieNanoGigESetFunction eReturn = EDalsaGenieNanoGigESetFunction_UnknownError;

	EDeviceParameterDalsaGenieNanoGigE eSaveID = EDeviceParameterDalsaGenieNanoGigE_FrameBurstStartTriggerSource;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDalsaGenieNanoGigESource)0 || eParam >= EDalsaGenieNanoGigESource_Count)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotFindCommandError;
			break;
		}

		EDalsaGenieNanoGigETriggerSelector eTrigSelect = EDalsaGenieNanoGigETriggerSelector_Count;

		if(GetTriggerSelector(&eTrigSelect))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDatabaseError;

			break;
		}

		if(eTrigSelect != EDalsaGenieNanoGigETriggerSelector_FrameBurstStart)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotSelectFeatureError;

			break;
		}

		if(IsInitialized())
		{
			BOOL bCommandAvailable = false;

			if(!m_pDevice->IsFeatureAvailable("TriggerSource", &bCommandAvailable))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_NotSupportedError;
				break;
			}

			if(!bCommandAvailable)
			{
				eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(!m_pDevice->SetFeatureValue("TriggerSource", CStringA(g_lpszDalsaGenieNanoGigESource[eParam])))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDalsaGenieNanoGigESetFunction_OK;
	}
	while(false);
	
	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDalsaGenieNanoGigE[eSaveID], g_lpszDalsaGenieNanoGigESource[nPreValue], g_lpszDalsaGenieNanoGigESource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDalsaGenieNanoGigEGetFunction CDeviceDalsaGenieNanoGigE::GetFrameBurstStartTriggerCount(int * pParam)
{
	EDalsaGenieNanoGigEGetFunction eReturn = EDalsaGenieNanoGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterDalsaGenieNanoGigE_FrameBurstStartTriggerCount));

		eReturn = EDalsaGenieNanoGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDalsaGenieNanoGigESetFunction CDeviceDalsaGenieNanoGigE::SetFrameBurstStartTriggerCount(int nParam)
{
	EDalsaGenieNanoGigESetFunction eReturn = EDalsaGenieNanoGigESetFunction_UnknownError;

	EDeviceParameterDalsaGenieNanoGigE eSaveID = EDeviceParameterDalsaGenieNanoGigE_FrameBurstStartTriggerCount;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDalsaGenieNanoGigETriggerSelector eTrigSelect = EDalsaGenieNanoGigETriggerSelector_Count;

		if(GetTriggerSelector(&eTrigSelect))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDatabaseError;

			break;
		}

		if(eTrigSelect != EDalsaGenieNanoGigETriggerSelector_FrameBurstStart)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotSelectFeatureError;

			break;
		}

		if(IsInitialized())
		{
			BOOL bCommandAvailable = false;

			if(!m_pDevice->IsFeatureAvailable("triggerFrameCount", &bCommandAvailable))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_NotSupportedError;
				break;
			}

			if(!bCommandAvailable)
			{
				eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(!m_pDevice->SetFeatureValue("triggerFrameCount", nParam))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDalsaGenieNanoGigESetFunction_OK;
	}
	while(false);
	
	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDalsaGenieNanoGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDalsaGenieNanoGigEGetFunction CDeviceDalsaGenieNanoGigE::GetAcquisitionStartMode(EDalsaGenieNanoGigESwitch* pParam)
{
	EDalsaGenieNanoGigEGetFunction eReturn = EDalsaGenieNanoGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterDalsaGenieNanoGigE_AcquisitionStartTriggerMode));

		if(nData < 0 || nData >= (int)EDalsaGenieNanoGigESwitch_Count)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDalsaGenieNanoGigESwitch)nData;

		eReturn = EDalsaGenieNanoGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDalsaGenieNanoGigESetFunction CDeviceDalsaGenieNanoGigE::SetAcquisitionStartMode(EDalsaGenieNanoGigESwitch eParam)
{
	EDalsaGenieNanoGigESetFunction eReturn = EDalsaGenieNanoGigESetFunction_UnknownError;

	EDeviceParameterDalsaGenieNanoGigE eSaveID = EDeviceParameterDalsaGenieNanoGigE_AcquisitionStartTriggerMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDalsaGenieNanoGigESwitch)0 || eParam >= EDalsaGenieNanoGigESwitch_Count)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotFindCommandError;
			break;
		}

		EDalsaGenieNanoGigETriggerSelector eTrigSelect = EDalsaGenieNanoGigETriggerSelector_Count;

		if(GetTriggerSelector(&eTrigSelect))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDatabaseError;

			break;
		}

		if(eTrigSelect != EDalsaGenieNanoGigETriggerSelector_AcquisitionStart)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotSelectFeatureError;

			break;
		}

		if(IsInitialized())
		{
			BOOL bCommandAvailable = false;

			if(!m_pDevice->IsFeatureAvailable("TriggerMode", &bCommandAvailable))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_NotSupportedError;
				break;
			}

			if(!bCommandAvailable)
			{
				eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(!m_pDevice->SetFeatureValue("TriggerMode", CStringA(g_lpszDalsaGenieNanoGigESwitch[eParam])))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDalsaGenieNanoGigESetFunction_OK;
	}
	while(false);
	
	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDalsaGenieNanoGigE[eSaveID], g_lpszDalsaGenieNanoGigESwitch[nPreValue], g_lpszDalsaGenieNanoGigESwitch[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDalsaGenieNanoGigEGetFunction CDeviceDalsaGenieNanoGigE::GetAcquisitionStartTriggerSource(EDalsaGenieNanoGigESource* pParam)
{
	EDalsaGenieNanoGigEGetFunction eReturn = EDalsaGenieNanoGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterDalsaGenieNanoGigE_AcquisitionStartTriggerSource));

		if(nData < 0 || nData >= (int)EDalsaGenieNanoGigESource_Count)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDalsaGenieNanoGigESource)nData;

		eReturn = EDalsaGenieNanoGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDalsaGenieNanoGigESetFunction CDeviceDalsaGenieNanoGigE::SetAcquisitionStartTriggerSource(EDalsaGenieNanoGigESource eParam)
{
	EDalsaGenieNanoGigESetFunction eReturn = EDalsaGenieNanoGigESetFunction_UnknownError;

	EDeviceParameterDalsaGenieNanoGigE eSaveID = EDeviceParameterDalsaGenieNanoGigE_AcquisitionStartTriggerSource;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDalsaGenieNanoGigESource)0 || eParam >= EDalsaGenieNanoGigESource_Count)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotFindCommandError;
			break;
		}

		EDalsaGenieNanoGigETriggerSelector eTrigSelect = EDalsaGenieNanoGigETriggerSelector_Count;

		if(GetTriggerSelector(&eTrigSelect))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDatabaseError;

			break;
		}

		if(eTrigSelect != EDalsaGenieNanoGigETriggerSelector_AcquisitionStart)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotSelectFeatureError;

			break;
		}

		if(IsInitialized())
		{
			BOOL bCommandAvailable = false;

			if(!m_pDevice->IsFeatureAvailable("TriggerSource", &bCommandAvailable))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_NotSupportedError;
				break;
			}

			if(!bCommandAvailable)
			{
				eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(!m_pDevice->SetFeatureValue("TriggerSource", CStringA(g_lpszDalsaGenieNanoGigESource[eParam])))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDalsaGenieNanoGigESetFunction_OK;
	}
	while(false);
	
	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDalsaGenieNanoGigE[eSaveID], g_lpszDalsaGenieNanoGigESource[nPreValue], g_lpszDalsaGenieNanoGigESource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDalsaGenieNanoGigEGetFunction CDeviceDalsaGenieNanoGigE::GetFrameRateControl(EDalsaGenieNanoGigEFrameRateControl * pParam)
{
	EDalsaGenieNanoGigEGetFunction eReturn = EDalsaGenieNanoGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterDalsaGenieNanoGigE_FrameRateControl));

		if(nData < 0 || nData >= (int)EDalsaGenieNanoGigEFrameRateControl_Count)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDalsaGenieNanoGigEFrameRateControl)nData;

		eReturn = EDalsaGenieNanoGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDalsaGenieNanoGigESetFunction CDeviceDalsaGenieNanoGigE::SetFrameRateControl(EDalsaGenieNanoGigEFrameRateControl eParam)
{
	EDalsaGenieNanoGigESetFunction eReturn = EDalsaGenieNanoGigESetFunction_UnknownError;

	EDeviceParameterDalsaGenieNanoGigE eSaveID = EDeviceParameterDalsaGenieNanoGigE_FrameRateControl;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDalsaGenieNanoGigEFrameRateControl)0 || eParam >= EDalsaGenieNanoGigEFrameRateControl_Count)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotFindCommandError;
			break;
		}
		
		if(IsLive() || !IsGrabAvailable())
		{
			eReturn = EDalsaGenieNanoGigESetFunction_ActiveDeviceError;
			break;
		}

		if(IsInitialized())
		{
			BOOL bCommandAvailable = false;

			if(!m_pDevice->IsFeatureAvailable("acquisitionFrameRateControlMode", &bCommandAvailable))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_NotSupportedError;
				break;
			}

			if(!bCommandAvailable)
			{
				eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(!m_pDevice->SetFeatureValue("acquisitionFrameRateControlMode", CStringA(g_lpszDalsaGenieNanoGigEFrameRateControl[eParam])))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDalsaGenieNanoGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDalsaGenieNanoGigE[eSaveID], g_lpszDalsaGenieNanoGigEFrameRateControl[nPreValue], g_lpszDalsaGenieNanoGigEFrameRateControl[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDalsaGenieNanoGigEGetFunction CDeviceDalsaGenieNanoGigE::GetFrameRate(double * pParam)
{
	EDalsaGenieNanoGigEGetFunction eReturn = EDalsaGenieNanoGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterDalsaGenieNanoGigE_FrameRate));

		eReturn = EDalsaGenieNanoGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDalsaGenieNanoGigESetFunction CDeviceDalsaGenieNanoGigE::SetFrameRate(double dblParam)
{
	EDalsaGenieNanoGigESetFunction eReturn = EDalsaGenieNanoGigESetFunction_UnknownError;

	EDeviceParameterDalsaGenieNanoGigE eSaveID = EDeviceParameterDalsaGenieNanoGigE_FrameRate;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsLive() || !IsGrabAvailable())
		{
			eReturn = EDalsaGenieNanoGigESetFunction_ActiveDeviceError;
			break;
		}

		if(IsInitialized())
		{
			EDalsaGenieNanoGigEFrameRateControl eFrameRateControl = EDalsaGenieNanoGigEFrameRateControl_Count;

			if(GetFrameRateControl(&eFrameRateControl))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDatabaseError;
				break;
			}

			if(eFrameRateControl != EDalsaGenieNanoGigEFrameRateControl_Programmable)
			{
				eReturn = EDalsaGenieNanoGigESetFunction_NotSelectFeatureError;
				break;
			}

			if(!m_pDevice->SetFeatureValue("AcquisitionFrameRate", dblParam))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%.6f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDalsaGenieNanoGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDalsaGenieNanoGigE[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDalsaGenieNanoGigEGetFunction CDeviceDalsaGenieNanoGigE::GetExposureTime(double* pParam)
{
	EDalsaGenieNanoGigEGetFunction eReturn = EDalsaGenieNanoGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterDalsaGenieNanoGigE_ExposureTime));

		eReturn = EDalsaGenieNanoGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDalsaGenieNanoGigESetFunction CDeviceDalsaGenieNanoGigE::SetExposureTime(double dblParam)
{
	EDalsaGenieNanoGigESetFunction eReturn = EDalsaGenieNanoGigESetFunction_UnknownError;

	EDeviceParameterDalsaGenieNanoGigE eSaveID = EDeviceParameterDalsaGenieNanoGigE_ExposureTime;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			EDalsaGenieNanoGigEFrameRateControl eFrameControl = EDalsaGenieNanoGigEFrameRateControl_Count;

			if(GetFrameRateControl(&eFrameControl))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDatabaseError;
				break;
			}

			if(eFrameControl != EDalsaGenieNanoGigEFrameRateControl_MaximumSpeed)
			{
				eReturn = EDalsaGenieNanoGigESetFunction_NotSelectFeatureError;
				break;
			}

			BOOL bCommandAvailable = false;

			if(!m_pDevice->IsFeatureAvailable("ExposureTime", &bCommandAvailable))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_NotSupportedError;
				break;
			}

			if(!bCommandAvailable)
			{
				eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(!m_pDevice->SetFeatureValue("ExposureTime", dblParam))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%.6f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDalsaGenieNanoGigESetFunction_OK;
	}
	while(false);
	
	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDalsaGenieNanoGigE[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDalsaGenieNanoGigEGetFunction CDeviceDalsaGenieNanoGigE::GetGainSelector(EDalsaGenieNanoGigEGainSelector* pParam)
{
	EDalsaGenieNanoGigEGetFunction eReturn = EDalsaGenieNanoGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterDalsaGenieNanoGigE_GainSelector));

		if(nData < 0 || nData >= (int)EDalsaGenieNanoGigEGainSelector_Count)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDalsaGenieNanoGigEGainSelector)nData;

		eReturn = EDalsaGenieNanoGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDalsaGenieNanoGigESetFunction CDeviceDalsaGenieNanoGigE::SetGainSelector(EDalsaGenieNanoGigEGainSelector eParam)
{
	EDalsaGenieNanoGigESetFunction eReturn = EDalsaGenieNanoGigESetFunction_UnknownError;

	EDeviceParameterDalsaGenieNanoGigE eSaveID = EDeviceParameterDalsaGenieNanoGigE_GainSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDalsaGenieNanoGigEGainSelector)0 || eParam >= EDalsaGenieNanoGigEGainSelector_Count)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotFindCommandError;
			break;
		}
		
		if(IsInitialized())
		{
			BOOL bCommandAvailable = false;

			if(!m_pDevice->IsFeatureAvailable("GainSelector", &bCommandAvailable))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_NotSupportedError;
				break;
			}

			if(!bCommandAvailable)
			{
				eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(!m_pDevice->SetFeatureValue("GainSelector", CStringA(g_lpszDalsaGenieNanoGigEGainSelect[eParam])))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDalsaGenieNanoGigESetFunction_OK;
	}
	while(false);
	
	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDalsaGenieNanoGigE[eSaveID], g_lpszDalsaGenieNanoGigEGainSelect[nPreValue], g_lpszDalsaGenieNanoGigEGainSelect[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDalsaGenieNanoGigEGetFunction CDeviceDalsaGenieNanoGigE::GetSensorGain(double* pParam)
{
	EDalsaGenieNanoGigEGetFunction eReturn = EDalsaGenieNanoGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterDalsaGenieNanoGigE_SensorGain));

		eReturn = EDalsaGenieNanoGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDalsaGenieNanoGigESetFunction CDeviceDalsaGenieNanoGigE::SetSensorGain(double dblParam)
{
	EDalsaGenieNanoGigESetFunction eReturn = EDalsaGenieNanoGigESetFunction_UnknownError;

	EDeviceParameterDalsaGenieNanoGigE eSaveID = EDeviceParameterDalsaGenieNanoGigE_SensorGain;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		EDalsaGenieNanoGigEGainSelector eGainSelect = EDalsaGenieNanoGigEGainSelector_Count;

		if(GetGainSelector(&eGainSelect))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDatabaseError;

			break;
		}

		if(eGainSelect != EDalsaGenieNanoGigEGainSelector_SensorAll)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotSelectFeatureError;

			break;
		}

		if(IsInitialized())
		{
			BOOL bCommandAvailable = false;

			if(!m_pDevice->IsFeatureAvailable("Gain", &bCommandAvailable))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_NotSupportedError;
				break;
			}

			if(!bCommandAvailable)
			{
				eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(!m_pDevice->SetFeatureValue("Gain", dblParam))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%.6f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDalsaGenieNanoGigESetFunction_OK;
	}
	while(false);
	
	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDalsaGenieNanoGigE[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDalsaGenieNanoGigEGetFunction CDeviceDalsaGenieNanoGigE::GetDigitalGain(double* pParam)
{
	EDalsaGenieNanoGigEGetFunction eReturn = EDalsaGenieNanoGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterDalsaGenieNanoGigE_DigitalGain));

		eReturn = EDalsaGenieNanoGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDalsaGenieNanoGigESetFunction CDeviceDalsaGenieNanoGigE::SetDigitalGain(double dblParam)
{
	EDalsaGenieNanoGigESetFunction eReturn = EDalsaGenieNanoGigESetFunction_UnknownError;

	EDeviceParameterDalsaGenieNanoGigE eSaveID = EDeviceParameterDalsaGenieNanoGigE_DigitalGain;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		EDalsaGenieNanoGigEGainSelector eGainSelect = EDalsaGenieNanoGigEGainSelector_Count;

		if(GetGainSelector(&eGainSelect))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDatabaseError;

			break;
		}

		if(eGainSelect != EDalsaGenieNanoGigEGainSelector_DigitalAll)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotSelectFeatureError;

			break;
		}

		if(IsInitialized())
		{
			BOOL bCommandAvailable = false;

			if(!m_pDevice->IsFeatureAvailable("Gain", &bCommandAvailable))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_NotSupportedError;
				break;
			}

			if(!bCommandAvailable)
			{
				eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(!m_pDevice->SetFeatureValue("Gain", dblParam))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%.6f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDalsaGenieNanoGigESetFunction_OK;
	}
	while(false);
	
	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDalsaGenieNanoGigE[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);
	
	return eReturn;
}

EDalsaGenieNanoGigEGetFunction CDeviceDalsaGenieNanoGigE::GetBlackLevelSelector(EDalsaGenieNanoGigEBlackLevelSelector* pParam)
{
	EDalsaGenieNanoGigEGetFunction eReturn = EDalsaGenieNanoGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterDalsaGenieNanoGigE_BlackLevelSelector));

		if(nData < 0 || nData >= (int)EDalsaGenieNanoGigEBlackLevelSelector_Count)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDalsaGenieNanoGigEBlackLevelSelector)nData;

		eReturn = EDalsaGenieNanoGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDalsaGenieNanoGigESetFunction CDeviceDalsaGenieNanoGigE::SetBlackLevelSelector(EDalsaGenieNanoGigEBlackLevelSelector eParam)
{
	EDalsaGenieNanoGigESetFunction eReturn = EDalsaGenieNanoGigESetFunction_UnknownError;

	EDeviceParameterDalsaGenieNanoGigE eSaveID = EDeviceParameterDalsaGenieNanoGigE_BlackLevelSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDalsaGenieNanoGigEBlackLevelSelector)0 || eParam >= EDalsaGenieNanoGigEBlackLevelSelector_Count)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			BOOL bCommandAvailable = false;

			if(!m_pDevice->IsFeatureAvailable("BlackLevelSelector", &bCommandAvailable))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_NotSupportedError;
				break;
			}

			if(!bCommandAvailable)
			{
				eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(!m_pDevice->SetFeatureValue("BlackLevelSelector", CStringA(g_lpszDalsaGenieNanoGigEBlackSelector[eParam])))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDalsaGenieNanoGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDalsaGenieNanoGigE[eSaveID], g_lpszDalsaGenieNanoGigEBlackSelector[nPreValue], g_lpszDalsaGenieNanoGigEBlackSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDalsaGenieNanoGigEGetFunction CDeviceDalsaGenieNanoGigE::GetBlackLevel(double* pParam)
{
	EDalsaGenieNanoGigEGetFunction eReturn = EDalsaGenieNanoGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterDalsaGenieNanoGigE_BlackLevel));

		eReturn = EDalsaGenieNanoGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDalsaGenieNanoGigESetFunction CDeviceDalsaGenieNanoGigE::SetBlackLevel(double dblParam)
{
	EDalsaGenieNanoGigESetFunction eReturn = EDalsaGenieNanoGigESetFunction_UnknownError;

	EDeviceParameterDalsaGenieNanoGigE eSaveID = EDeviceParameterDalsaGenieNanoGigE_BlackLevel;

	double dblPreValue = _ttof(GetParamValue(eSaveID));

	do
	{
		EDalsaGenieNanoGigEBlackLevelSelector eSelect = EDalsaGenieNanoGigEBlackLevelSelector_Count;

		if(GetBlackLevelSelector(&eSelect))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDatabaseError;

			break;
		}

		if(eSelect != EDalsaGenieNanoGigEBlackLevelSelector_Analog)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotSelectFeatureError;

			break;
		}

		if(IsInitialized())
		{
			BOOL bCommandAvailable = false;

			if(!m_pDevice->IsFeatureAvailable("BlackLevel", &bCommandAvailable))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_NotSupportedError;
				break;
			}

			if(!bCommandAvailable)
			{
				eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(!m_pDevice->SetFeatureValue("BlackLevel", dblParam))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%.6f"), dblParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDalsaGenieNanoGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDalsaGenieNanoGigE[eSaveID], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDalsaGenieNanoGigEGetFunction CDeviceDalsaGenieNanoGigE::GetLineSelector(EDalsaGenieNanoGigELineSelector* pParam)
{
	EDalsaGenieNanoGigEGetFunction eReturn = EDalsaGenieNanoGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterDalsaGenieNanoGigE_LineSelector));

		if(nData < 0 || nData >= (int)EDalsaGenieNanoGigELineSelector_Count)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDalsaGenieNanoGigELineSelector)nData;

		eReturn = EDalsaGenieNanoGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDalsaGenieNanoGigESetFunction CDeviceDalsaGenieNanoGigE::SetLineSelector(EDalsaGenieNanoGigELineSelector eParam)
{
	EDalsaGenieNanoGigESetFunction eReturn = EDalsaGenieNanoGigESetFunction_UnknownError;

	EDeviceParameterDalsaGenieNanoGigE eSaveID = EDeviceParameterDalsaGenieNanoGigE_LineSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDalsaGenieNanoGigELineSelector)0 || eParam >= EDalsaGenieNanoGigELineSelector_Count)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			BOOL bCommandAvailable = false;

			if(!m_pDevice->IsFeatureAvailable("LineSelector", &bCommandAvailable))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_NotSupportedError;
				break;
			}

			if(!bCommandAvailable)
			{
				eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(!m_pDevice->SetFeatureValue("LineSelector", CStringA(g_lpszDalsaGenieNanoGigELineSelector[eParam])))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDalsaGenieNanoGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDalsaGenieNanoGigE[eSaveID], g_lpszDalsaGenieNanoGigELineSelector[nPreValue], g_lpszDalsaGenieNanoGigELineSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDalsaGenieNanoGigEGetFunction CDeviceDalsaGenieNanoGigE::GetLine1DebouncingPeriod(int * pParam)
{
	EDalsaGenieNanoGigEGetFunction eReturn = EDalsaGenieNanoGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterDalsaGenieNanoGigE_Line1DebouncingPeriod));

		eReturn = EDalsaGenieNanoGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDalsaGenieNanoGigESetFunction CDeviceDalsaGenieNanoGigE::SetLine1DebouncingPeriod(int nParam)
{
	EDalsaGenieNanoGigESetFunction eReturn = EDalsaGenieNanoGigESetFunction_UnknownError;

	EDeviceParameterDalsaGenieNanoGigE eSaveID = EDeviceParameterDalsaGenieNanoGigE_Line1DebouncingPeriod;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDalsaGenieNanoGigELineSelector eSelect = EDalsaGenieNanoGigELineSelector_Count;

		if(GetLineSelector(&eSelect))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDatabaseError;

			break;
		}

		if(eSelect != EDalsaGenieNanoGigELineSelector_Line1)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotSelectFeatureError;

			break;
		}

		if(IsInitialized())
		{
			BOOL bCommandAvailable = false;

			if(!m_pDevice->IsFeatureAvailable("lineDebouncingPeriod", &bCommandAvailable))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_NotSupportedError;
				break;
			}

			if(!bCommandAvailable)
			{
				eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(!m_pDevice->SetFeatureValue("lineDebouncingPeriod", nParam))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDalsaGenieNanoGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDalsaGenieNanoGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDalsaGenieNanoGigEGetFunction CDeviceDalsaGenieNanoGigE::GetLine1Inverter(EDalsaGenieNanoGigESwitch* pParam)
{
	EDalsaGenieNanoGigEGetFunction eReturn = EDalsaGenieNanoGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterDalsaGenieNanoGigE_Line1Inverter));

		if(nData < 0 || nData >= (int)EDalsaGenieNanoGigESwitch_Count)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDalsaGenieNanoGigESwitch)nData;

		eReturn = EDalsaGenieNanoGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDalsaGenieNanoGigESetFunction CDeviceDalsaGenieNanoGigE::SetLine1Inverter(EDalsaGenieNanoGigESwitch eParam)
{
	EDalsaGenieNanoGigESetFunction eReturn = EDalsaGenieNanoGigESetFunction_UnknownError;

	EDeviceParameterDalsaGenieNanoGigE eSaveID = EDeviceParameterDalsaGenieNanoGigE_Line1Inverter;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDalsaGenieNanoGigESwitch)0 || eParam >= EDalsaGenieNanoGigESwitch_Count)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotFindCommandError;
			break;
		}

		EDalsaGenieNanoGigELineSelector eSelect = EDalsaGenieNanoGigELineSelector_Count;

		if(GetLineSelector(&eSelect))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDatabaseError;

			break;
		}

		if(eSelect != EDalsaGenieNanoGigELineSelector_Line1)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotSelectFeatureError;

			break;
		}

		if(IsInitialized())
		{
			BOOL bCommandAvailable = false;

			if(!m_pDevice->IsFeatureAvailable("LineInverter", &bCommandAvailable))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_NotSupportedError;
				break;
			}

			if(!bCommandAvailable)
			{
				eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(!m_pDevice->SetFeatureValue("LineInverter", (bool)eParam))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDalsaGenieNanoGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDalsaGenieNanoGigE[eSaveID], g_lpszDalsaGenieNanoGigESwitch[nPreValue], g_lpszDalsaGenieNanoGigESwitch[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDalsaGenieNanoGigEGetFunction CDeviceDalsaGenieNanoGigE::GetLine2DebouncingPeriod(int * pParam)
{
	EDalsaGenieNanoGigEGetFunction eReturn = EDalsaGenieNanoGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterDalsaGenieNanoGigE_Line2DebouncingPeriod));

		eReturn = EDalsaGenieNanoGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDalsaGenieNanoGigESetFunction CDeviceDalsaGenieNanoGigE::SetLine2DebouncingPeriod(int nParam)
{
	EDalsaGenieNanoGigESetFunction eReturn = EDalsaGenieNanoGigESetFunction_UnknownError;

	EDeviceParameterDalsaGenieNanoGigE eSaveID = EDeviceParameterDalsaGenieNanoGigE_Line2DebouncingPeriod;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDalsaGenieNanoGigELineSelector eSelect = EDalsaGenieNanoGigELineSelector_Count;

		if(GetLineSelector(&eSelect))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDatabaseError;

			break;
		}

		if(eSelect != EDalsaGenieNanoGigELineSelector_Line2)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotSelectFeatureError;

			break;
		}

		if(IsInitialized())
		{
			BOOL bCommandAvailable = false;

			if(!m_pDevice->IsFeatureAvailable("lineDebouncingPeriod", &bCommandAvailable))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_NotSupportedError;
				break;
			}

			if(!bCommandAvailable)
			{
				eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(!m_pDevice->SetFeatureValue("lineDebouncingPeriod", nParam))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDalsaGenieNanoGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDalsaGenieNanoGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDalsaGenieNanoGigEGetFunction CDeviceDalsaGenieNanoGigE::GetLine2Inverter(EDalsaGenieNanoGigESwitch* pParam)
{
	EDalsaGenieNanoGigEGetFunction eReturn = EDalsaGenieNanoGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterDalsaGenieNanoGigE_Line2Inverter));

		if(nData < 0 || nData >= (int)EDalsaGenieNanoGigESwitch_Count)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDalsaGenieNanoGigESwitch)nData;

		eReturn = EDalsaGenieNanoGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDalsaGenieNanoGigESetFunction CDeviceDalsaGenieNanoGigE::SetLine2Inverter(EDalsaGenieNanoGigESwitch eParam)
{
	EDalsaGenieNanoGigESetFunction eReturn = EDalsaGenieNanoGigESetFunction_UnknownError;

	EDeviceParameterDalsaGenieNanoGigE eSaveID = EDeviceParameterDalsaGenieNanoGigE_Line2Inverter;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDalsaGenieNanoGigESwitch)0 || eParam >= EDalsaGenieNanoGigESwitch_Count)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotFindCommandError;
			break;
		}

		EDalsaGenieNanoGigELineSelector eSelect = EDalsaGenieNanoGigELineSelector_Count;

		if(GetLineSelector(&eSelect))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDatabaseError;

			break;
		}

		if(eSelect != EDalsaGenieNanoGigELineSelector_Line2)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotSelectFeatureError;

			break;
		}

		if(IsInitialized())
		{
			BOOL bCommandAvailable = false;

			if(!m_pDevice->IsFeatureAvailable("LineInverter", &bCommandAvailable))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_NotSupportedError;
				break;
			}

			if(!bCommandAvailable)
			{
				eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(!m_pDevice->SetFeatureValue("LineInverter", (bool)eParam))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDalsaGenieNanoGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDalsaGenieNanoGigE[eSaveID], g_lpszDalsaGenieNanoGigESwitch[nPreValue], g_lpszDalsaGenieNanoGigESwitch[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDalsaGenieNanoGigEGetFunction CDeviceDalsaGenieNanoGigE::GetLine3Source(EDalsaGenieNanoGigELineSource* pParam)
{
	EDalsaGenieNanoGigEGetFunction eReturn = EDalsaGenieNanoGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterDalsaGenieNanoGigE_Line3Source));

		if(nData < 0 || nData >= (int)EDalsaGenieNanoGigELineSource_Count)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDalsaGenieNanoGigELineSource)nData;

		eReturn = EDalsaGenieNanoGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDalsaGenieNanoGigESetFunction CDeviceDalsaGenieNanoGigE::SetLine3Source(EDalsaGenieNanoGigELineSource eParam)
{
	EDalsaGenieNanoGigESetFunction eReturn = EDalsaGenieNanoGigESetFunction_UnknownError;

	EDeviceParameterDalsaGenieNanoGigE eSaveID = EDeviceParameterDalsaGenieNanoGigE_Line3Source;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDalsaGenieNanoGigELineSource)0 || eParam >= EDalsaGenieNanoGigELineSource_Count)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotFindCommandError;
			break;
		}

		EDalsaGenieNanoGigELineSelector eSelect = EDalsaGenieNanoGigELineSelector_Count;

		if(GetLineSelector(&eSelect))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDatabaseError;

			break;
		}

		if(eSelect != EDalsaGenieNanoGigELineSelector_Line3)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotSelectFeatureError;

			break;
		}

		if(IsInitialized())
		{
			BOOL bCommandAvailable = false;

			if(!m_pDevice->IsFeatureAvailable("outputLineSource", &bCommandAvailable))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_NotSupportedError;
				break;
			}

			if(!bCommandAvailable)
			{
				eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(!m_pDevice->SetFeatureValue("outputLineSource", CStringA(g_lpszDalsaGenieNanoGigELineSource[eParam])))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDalsaGenieNanoGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDalsaGenieNanoGigE[eSaveID], g_lpszDalsaGenieNanoGigELineSource[nPreValue], g_lpszDalsaGenieNanoGigELineSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDalsaGenieNanoGigEGetFunction CDeviceDalsaGenieNanoGigE::GetLine3Inverter(EDalsaGenieNanoGigESwitch* pParam)
{
	EDalsaGenieNanoGigEGetFunction eReturn = EDalsaGenieNanoGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterDalsaGenieNanoGigE_Line3Inverter));

		if(nData < 0 || nData >= (int)EDalsaGenieNanoGigESwitch_Count)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDalsaGenieNanoGigESwitch)nData;

		eReturn = EDalsaGenieNanoGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDalsaGenieNanoGigESetFunction CDeviceDalsaGenieNanoGigE::SetLine3Inverter(EDalsaGenieNanoGigESwitch eParam)
{
	EDalsaGenieNanoGigESetFunction eReturn = EDalsaGenieNanoGigESetFunction_UnknownError;

	EDeviceParameterDalsaGenieNanoGigE eSaveID = EDeviceParameterDalsaGenieNanoGigE_Line3Inverter;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDalsaGenieNanoGigESwitch)0 || eParam >= EDalsaGenieNanoGigESwitch_Count)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotFindCommandError;
			break;
		}

		EDalsaGenieNanoGigELineSelector eSelect = EDalsaGenieNanoGigELineSelector_Count;

		if(GetLineSelector(&eSelect))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDatabaseError;

			break;
		}

		if(eSelect != EDalsaGenieNanoGigELineSelector_Line3)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotSelectFeatureError;
			break;
		}

		EDalsaGenieNanoGigELineSource eLineSource = EDalsaGenieNanoGigELineSource_Count;

		if(GetLine3Source(&eLineSource))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDatabaseError;
			break;
		}

		if(eLineSource == EDalsaGenieNanoGigELineSource_Off)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotSelectFeatureError;
			break;
		}

		if(IsInitialized())
		{
			BOOL bCommandAvailable = false;

			if(!m_pDevice->IsFeatureAvailable("LineInverter", &bCommandAvailable))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_NotSupportedError;
				break;
			}

			if(!bCommandAvailable)
			{
				eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(!m_pDevice->SetFeatureValue("LineInverter", (bool)eParam))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDalsaGenieNanoGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDalsaGenieNanoGigE[eSaveID], g_lpszDalsaGenieNanoGigESwitch[nPreValue], g_lpszDalsaGenieNanoGigESwitch[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDalsaGenieNanoGigEGetFunction CDeviceDalsaGenieNanoGigE::GetLine3Activation(EDalsaGenieNanoGigEActivation* pParam)
{
	EDalsaGenieNanoGigEGetFunction eReturn = EDalsaGenieNanoGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterDalsaGenieNanoGigE_Line3Activation));

		if(nData < 0 || nData >= (int)EDalsaGenieNanoGigEActivation_Count)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDalsaGenieNanoGigEActivation)nData;

		eReturn = EDalsaGenieNanoGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDalsaGenieNanoGigESetFunction CDeviceDalsaGenieNanoGigE::SetLine3Activation(EDalsaGenieNanoGigEActivation eParam)
{
	EDalsaGenieNanoGigESetFunction eReturn = EDalsaGenieNanoGigESetFunction_UnknownError;

	EDeviceParameterDalsaGenieNanoGigE eSaveID = EDeviceParameterDalsaGenieNanoGigE_Line3Activation;

	int nPreActivation = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDalsaGenieNanoGigEActivation)0 || eParam >= EDalsaGenieNanoGigEActivation_Count)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotFindCommandError;
			break;
		}

		EDalsaGenieNanoGigELineSelector eSelect = EDalsaGenieNanoGigELineSelector_Count;

		if(GetLineSelector(&eSelect))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDatabaseError;

			break;
		}

		if(eSelect != EDalsaGenieNanoGigELineSelector_Line3)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotSelectFeatureError;
			break;
		}

		EDalsaGenieNanoGigELineSource eLineSource = EDalsaGenieNanoGigELineSource_Count;

		if(GetLine3Source(&eLineSource))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDatabaseError;
			break;
		}

		if(eLineSource != EDalsaGenieNanoGigELineSource_Input1Event && eLineSource != EDalsaGenieNanoGigELineSource_Input2Event)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotSelectFeatureError;
			break;
		}

		if(IsInitialized())
		{
			BOOL bCommandAvailable = false;

			if(!m_pDevice->IsFeatureAvailable("outputLinePulseActivation", &bCommandAvailable))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_NotSupportedError;
				break;
			}

			if(!bCommandAvailable)
			{
				eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(!m_pDevice->SetFeatureValue("outputLinePulseActivation", CStringA(g_lpszDalsaGenieNanoGigEActivation[eParam])))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDalsaGenieNanoGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDalsaGenieNanoGigE[eSaveID], g_lpszDalsaGenieNanoGigEActivation[nPreActivation], g_lpszDalsaGenieNanoGigEActivation[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDalsaGenieNanoGigEGetFunction CDeviceDalsaGenieNanoGigE::GetLine3Delay(int * pParam)
{
	EDalsaGenieNanoGigEGetFunction eReturn = EDalsaGenieNanoGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterDalsaGenieNanoGigE_Line3Delay));

		eReturn = EDalsaGenieNanoGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDalsaGenieNanoGigESetFunction CDeviceDalsaGenieNanoGigE::SetLine3Delay(int nParam)
{
	EDalsaGenieNanoGigESetFunction eReturn = EDalsaGenieNanoGigESetFunction_UnknownError;

	EDeviceParameterDalsaGenieNanoGigE eSaveID = EDeviceParameterDalsaGenieNanoGigE_Line3Delay;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDalsaGenieNanoGigELineSelector eSelect = EDalsaGenieNanoGigELineSelector_Count;

		if(GetLineSelector(&eSelect))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDatabaseError;

			break;
		}

		if(eSelect != EDalsaGenieNanoGigELineSelector_Line3)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotSelectFeatureError;
			break;
		}

		EDalsaGenieNanoGigELineSource eLineSource = EDalsaGenieNanoGigELineSource_Count;

		if(GetLine3Source(&eLineSource))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDatabaseError;
			break;
		}

		if(eLineSource == EDalsaGenieNanoGigELineSource_Off || eLineSource == EDalsaGenieNanoGigELineSource_SoftwareControlled || eLineSource == EDalsaGenieNanoGigELineSource_ExposureActive)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotSelectFeatureError;
			break;
		}

		if(IsInitialized())
		{
			BOOL bCommandAvailable = false;

			if(!m_pDevice->IsFeatureAvailable("outputLinePulseDelay", &bCommandAvailable))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_NotSupportedError;
				break;
			}

			if(!bCommandAvailable)
			{
				eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(!m_pDevice->SetFeatureValue("outputLinePulseDelay", nParam))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDalsaGenieNanoGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDalsaGenieNanoGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDalsaGenieNanoGigEGetFunction CDeviceDalsaGenieNanoGigE::GetLine3Duration(int * pParam)
{
	EDalsaGenieNanoGigEGetFunction eReturn = EDalsaGenieNanoGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterDalsaGenieNanoGigE_Line3Duration));

		eReturn = EDalsaGenieNanoGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDalsaGenieNanoGigESetFunction CDeviceDalsaGenieNanoGigE::SetLine3Duration(int nParam)
{
	EDalsaGenieNanoGigESetFunction eReturn = EDalsaGenieNanoGigESetFunction_UnknownError;

	EDeviceParameterDalsaGenieNanoGigE eSaveID = EDeviceParameterDalsaGenieNanoGigE_Line3Duration;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDalsaGenieNanoGigELineSelector eSelect = EDalsaGenieNanoGigELineSelector_Count;

		if(GetLineSelector(&eSelect))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDatabaseError;

			break;
		}

		if(eSelect != EDalsaGenieNanoGigELineSelector_Line3)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotSelectFeatureError;
			break;
		}

		EDalsaGenieNanoGigELineSource eLineSource = EDalsaGenieNanoGigELineSource_Count;

		if(GetLine3Source(&eLineSource))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDatabaseError;
			break;
		}

		if(eLineSource == EDalsaGenieNanoGigELineSource_Off || eLineSource == EDalsaGenieNanoGigELineSource_SoftwareControlled || eLineSource == EDalsaGenieNanoGigELineSource_ExposureActive)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotSelectFeatureError;
			break;
		}

		if(IsInitialized())
		{
			BOOL bCommandAvailable = false;

			if(!m_pDevice->IsFeatureAvailable("outputLinePulseDuration", &bCommandAvailable))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_NotSupportedError;
				break;
			}

			if(!bCommandAvailable)
			{
				eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(!m_pDevice->SetFeatureValue("outputLinePulseDuration", nParam))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDalsaGenieNanoGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDalsaGenieNanoGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDalsaGenieNanoGigEGetFunction CDeviceDalsaGenieNanoGigE::GetLine3Value(EDalsaGenieNanoGigELineValue* pParam)
{
	EDalsaGenieNanoGigEGetFunction eReturn = EDalsaGenieNanoGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterDalsaGenieNanoGigE_Line3Value));

		if(nData < 0 || nData >= (int)EDalsaGenieNanoGigELineValue_Count)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDalsaGenieNanoGigELineValue)nData;

		eReturn = EDalsaGenieNanoGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDalsaGenieNanoGigESetFunction CDeviceDalsaGenieNanoGigE::SetLine3Value(EDalsaGenieNanoGigELineValue eParam)
{
	EDalsaGenieNanoGigESetFunction eReturn = EDalsaGenieNanoGigESetFunction_UnknownError;

	EDeviceParameterDalsaGenieNanoGigE eSaveID = EDeviceParameterDalsaGenieNanoGigE_Line3Value;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDalsaGenieNanoGigELineValue)0 || eParam >= EDalsaGenieNanoGigELineValue_Count)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotFindCommandError;
			break;
		}

		EDalsaGenieNanoGigELineSelector eSelect = EDalsaGenieNanoGigELineSelector_Count;

		if(GetLineSelector(&eSelect))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDatabaseError;

			break;
		}

		if(eSelect != EDalsaGenieNanoGigELineSelector_Line3)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotSelectFeatureError;
			break;
		}

		EDalsaGenieNanoGigELineSource eLineSource = EDalsaGenieNanoGigELineSource_Count;

		if(GetLine3Source(&eLineSource))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDatabaseError;
			break;
		}

		if(eLineSource != EDalsaGenieNanoGigELineSource_SoftwareControlled)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotSelectFeatureError;
			break;
		}

		if(IsInitialized())
		{
			BOOL bCommandAvailable = false;

			if(!m_pDevice->IsFeatureAvailable("outputLineValue", &bCommandAvailable))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_NotSupportedError;
				break;
			}

			if(!bCommandAvailable)
			{
				eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(!m_pDevice->SetFeatureValue("outputLineValue", CStringA(g_lpszDalsaGenieNanoGigELineValue[eParam])))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDalsaGenieNanoGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDalsaGenieNanoGigE[eSaveID], g_lpszDalsaGenieNanoGigELineValue[nPreValue], g_lpszDalsaGenieNanoGigELineValue[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDalsaGenieNanoGigEGetFunction CDeviceDalsaGenieNanoGigE::GetLine4Source(EDalsaGenieNanoGigELineSource* pParam)
{
	EDalsaGenieNanoGigEGetFunction eReturn = EDalsaGenieNanoGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterDalsaGenieNanoGigE_Line4Source));

		if(nData < 0 || nData >= (int)EDalsaGenieNanoGigELineSource_Count)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDalsaGenieNanoGigELineSource)nData;

		eReturn = EDalsaGenieNanoGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDalsaGenieNanoGigESetFunction CDeviceDalsaGenieNanoGigE::SetLine4Source(EDalsaGenieNanoGigELineSource eParam)
{
	EDalsaGenieNanoGigESetFunction eReturn = EDalsaGenieNanoGigESetFunction_UnknownError;

	EDeviceParameterDalsaGenieNanoGigE eSaveID = EDeviceParameterDalsaGenieNanoGigE_Line4Source;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDalsaGenieNanoGigELineSource)0 || eParam >= EDalsaGenieNanoGigELineSource_Count)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotFindCommandError;
			break;
		}

		EDalsaGenieNanoGigELineSelector eSelect = EDalsaGenieNanoGigELineSelector_Count;

		if(GetLineSelector(&eSelect))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDatabaseError;

			break;
		}

		if(eSelect != EDalsaGenieNanoGigELineSelector_Line4)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotSelectFeatureError;

			break;
		}

		if(IsInitialized())
		{
			BOOL bCommandAvailable = false;

			if(!m_pDevice->IsFeatureAvailable("outputLineSource", &bCommandAvailable))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_NotSupportedError;
				break;
			}

			if(!bCommandAvailable)
			{
				eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(!m_pDevice->SetFeatureValue("outputLineSource", CStringA(g_lpszDalsaGenieNanoGigELineSource[eParam])))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDalsaGenieNanoGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDalsaGenieNanoGigE[eSaveID], g_lpszDalsaGenieNanoGigELineSource[nPreValue], g_lpszDalsaGenieNanoGigELineSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDalsaGenieNanoGigEGetFunction CDeviceDalsaGenieNanoGigE::GetLine4Inverter(EDalsaGenieNanoGigESwitch* pParam)
{
	EDalsaGenieNanoGigEGetFunction eReturn = EDalsaGenieNanoGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterDalsaGenieNanoGigE_Line4Inverter));

		if(nData < 0 || nData >= (int)EDalsaGenieNanoGigESwitch_Count)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDalsaGenieNanoGigESwitch)nData;

		eReturn = EDalsaGenieNanoGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDalsaGenieNanoGigESetFunction CDeviceDalsaGenieNanoGigE::SetLine4Inverter(EDalsaGenieNanoGigESwitch eParam)
{
	EDalsaGenieNanoGigESetFunction eReturn = EDalsaGenieNanoGigESetFunction_UnknownError;

	EDeviceParameterDalsaGenieNanoGigE eSaveID = EDeviceParameterDalsaGenieNanoGigE_Line4Inverter;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDalsaGenieNanoGigESwitch)0 || eParam >= EDalsaGenieNanoGigESwitch_Count)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotFindCommandError;
			break;
		}

		EDalsaGenieNanoGigELineSelector eSelect = EDalsaGenieNanoGigELineSelector_Count;

		if(GetLineSelector(&eSelect))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDatabaseError;

			break;
		}

		if(eSelect != EDalsaGenieNanoGigELineSelector_Line4)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotSelectFeatureError;
			break;
		}

		EDalsaGenieNanoGigELineSource eLineSource = EDalsaGenieNanoGigELineSource_Count;

		if(GetLine4Source(&eLineSource))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDatabaseError;
			break;
		}

		if(eLineSource == EDalsaGenieNanoGigELineSource_Off)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotSelectFeatureError;
			break;
		}

		if(IsInitialized())
		{
			BOOL bCommandAvailable = false;

			if(!m_pDevice->IsFeatureAvailable("LineInverter", &bCommandAvailable))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_NotSupportedError;
				break;
			}

			if(!bCommandAvailable)
			{
				eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(!m_pDevice->SetFeatureValue("LineInverter", (bool)eParam))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDalsaGenieNanoGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDalsaGenieNanoGigE[eSaveID], g_lpszDalsaGenieNanoGigESwitch[nPreValue], g_lpszDalsaGenieNanoGigESwitch[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDalsaGenieNanoGigEGetFunction CDeviceDalsaGenieNanoGigE::GetLine4Activation(EDalsaGenieNanoGigEActivation* pParam)
{
	EDalsaGenieNanoGigEGetFunction eReturn = EDalsaGenieNanoGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterDalsaGenieNanoGigE_Line4Activation));

		if(nData < 0 || nData >= (int)EDalsaGenieNanoGigEActivation_Count)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDalsaGenieNanoGigEActivation)nData;

		eReturn = EDalsaGenieNanoGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDalsaGenieNanoGigESetFunction CDeviceDalsaGenieNanoGigE::SetLine4Activation(EDalsaGenieNanoGigEActivation eParam)
{
	EDalsaGenieNanoGigESetFunction eReturn = EDalsaGenieNanoGigESetFunction_UnknownError;

	EDeviceParameterDalsaGenieNanoGigE eSaveID = EDeviceParameterDalsaGenieNanoGigE_Line4Activation;

	int nPreActivation = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDalsaGenieNanoGigEActivation)0 || eParam >= EDalsaGenieNanoGigEActivation_Count)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotFindCommandError;
			break;
		}

		EDalsaGenieNanoGigELineSelector eSelect = EDalsaGenieNanoGigELineSelector_Count;

		if(GetLineSelector(&eSelect))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDatabaseError;

			break;
		}

		if(eSelect != EDalsaGenieNanoGigELineSelector_Line4)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotSelectFeatureError;
			break;
		}

		EDalsaGenieNanoGigELineSource eLineSource = EDalsaGenieNanoGigELineSource_Count;

		if(GetLine4Source(&eLineSource))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDatabaseError;
			break;
		}

		if(eLineSource != EDalsaGenieNanoGigELineSource_Input1Event && eLineSource != EDalsaGenieNanoGigELineSource_Input2Event)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotSelectFeatureError;
			break;
		}

		if(IsInitialized())
		{
			BOOL bCommandAvailable = false;

			if(!m_pDevice->IsFeatureAvailable("outputLinePulseActivation", &bCommandAvailable))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_NotSupportedError;
				break;
			}

			if(!bCommandAvailable)
			{
				eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(!m_pDevice->SetFeatureValue("outputLinePulseActivation", CStringA(g_lpszDalsaGenieNanoGigEActivation[eParam])))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDalsaGenieNanoGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDalsaGenieNanoGigE[eSaveID], g_lpszDalsaGenieNanoGigEActivation[nPreActivation], g_lpszDalsaGenieNanoGigEActivation[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDalsaGenieNanoGigEGetFunction CDeviceDalsaGenieNanoGigE::GetLine4Delay(int * pParam)
{
	EDalsaGenieNanoGigEGetFunction eReturn = EDalsaGenieNanoGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterDalsaGenieNanoGigE_Line4Delay));

		eReturn = EDalsaGenieNanoGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDalsaGenieNanoGigESetFunction CDeviceDalsaGenieNanoGigE::SetLine4Delay(int nParam)
{
	EDalsaGenieNanoGigESetFunction eReturn = EDalsaGenieNanoGigESetFunction_UnknownError;

	EDeviceParameterDalsaGenieNanoGigE eSaveID = EDeviceParameterDalsaGenieNanoGigE_Line4Delay;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDalsaGenieNanoGigELineSelector eSelect = EDalsaGenieNanoGigELineSelector_Count;

		if(GetLineSelector(&eSelect))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDatabaseError;

			break;
		}

		if(eSelect != EDalsaGenieNanoGigELineSelector_Line4)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotSelectFeatureError;
			break;
		}

		EDalsaGenieNanoGigELineSource eLineSource = EDalsaGenieNanoGigELineSource_Count;

		if(GetLine4Source(&eLineSource))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDatabaseError;
			break;
		}

		if(eLineSource == EDalsaGenieNanoGigELineSource_Off || eLineSource == EDalsaGenieNanoGigELineSource_SoftwareControlled || eLineSource == EDalsaGenieNanoGigELineSource_ExposureActive)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotSelectFeatureError;
			break;
		}

		if(IsInitialized())
		{
			BOOL bCommandAvailable = false;

			if(!m_pDevice->IsFeatureAvailable("outputLinePulseDelay", &bCommandAvailable))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_NotSupportedError;
				break;
			}

			if(!bCommandAvailable)
			{
				eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(!m_pDevice->SetFeatureValue("outputLinePulseDelay", nParam))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDalsaGenieNanoGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDalsaGenieNanoGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDalsaGenieNanoGigEGetFunction CDeviceDalsaGenieNanoGigE::GetLine4Duration(int * pParam)
{
	EDalsaGenieNanoGigEGetFunction eReturn = EDalsaGenieNanoGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterDalsaGenieNanoGigE_Line4Duration));

		eReturn = EDalsaGenieNanoGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDalsaGenieNanoGigESetFunction CDeviceDalsaGenieNanoGigE::SetLine4Duration(int nParam)
{
	EDalsaGenieNanoGigESetFunction eReturn = EDalsaGenieNanoGigESetFunction_UnknownError;

	EDeviceParameterDalsaGenieNanoGigE eSaveID = EDeviceParameterDalsaGenieNanoGigE_Line4Duration;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		EDalsaGenieNanoGigELineSelector eSelect = EDalsaGenieNanoGigELineSelector_Count;

		if(GetLineSelector(&eSelect))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDatabaseError;

			break;
		}

		if(eSelect != EDalsaGenieNanoGigELineSelector_Line4)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotSelectFeatureError;
			break;
		}

		EDalsaGenieNanoGigELineSource eLineSource = EDalsaGenieNanoGigELineSource_Count;

		if(GetLine4Source(&eLineSource))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDatabaseError;
			break;
		}

		if(eLineSource == EDalsaGenieNanoGigELineSource_Off || eLineSource == EDalsaGenieNanoGigELineSource_SoftwareControlled || eLineSource == EDalsaGenieNanoGigELineSource_ExposureActive)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotSelectFeatureError;
			break;
		}

		if(IsInitialized())
		{
			BOOL bCommandAvailable = false;

			if(!m_pDevice->IsFeatureAvailable("outputLinePulseDuration", &bCommandAvailable))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_NotSupportedError;
				break;
			}

			if(!bCommandAvailable)
			{
				eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(!m_pDevice->SetFeatureValue("outputLinePulseDuration", nParam))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDalsaGenieNanoGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDalsaGenieNanoGigE[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDalsaGenieNanoGigEGetFunction CDeviceDalsaGenieNanoGigE::GetLine4Value(EDalsaGenieNanoGigELineValue* pParam)
{
	EDalsaGenieNanoGigEGetFunction eReturn = EDalsaGenieNanoGigEGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterDalsaGenieNanoGigE_Line4Value));

		if(nData < 0 || nData >= (int)EDalsaGenieNanoGigELineValue_Count)
		{
			eReturn = EDalsaGenieNanoGigEGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDalsaGenieNanoGigELineValue)nData;

		eReturn = EDalsaGenieNanoGigEGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EDalsaGenieNanoGigESetFunction CDeviceDalsaGenieNanoGigE::SetLine4Value(EDalsaGenieNanoGigELineValue eParam)
{
	EDalsaGenieNanoGigESetFunction eReturn = EDalsaGenieNanoGigESetFunction_UnknownError;

	EDeviceParameterDalsaGenieNanoGigE eSaveID = EDeviceParameterDalsaGenieNanoGigE_Line4Value;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDalsaGenieNanoGigELineValue)0 || eParam >= EDalsaGenieNanoGigELineValue_Count)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotFindCommandError;
			break;
		}

		EDalsaGenieNanoGigELineSelector eSelect = EDalsaGenieNanoGigELineSelector_Count;

		if(GetLineSelector(&eSelect))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDatabaseError;

			break;
		}

		if(eSelect != EDalsaGenieNanoGigELineSelector_Line4)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotSelectFeatureError;
			break;
		}

		EDalsaGenieNanoGigELineSource eLineSource = EDalsaGenieNanoGigELineSource_Count;

		if(GetLine4Source(&eLineSource))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDatabaseError;
			break;
		}

		if(eLineSource != EDalsaGenieNanoGigELineSource_SoftwareControlled)
		{
			eReturn = EDalsaGenieNanoGigESetFunction_NotSelectFeatureError;
			break;
		}

		if(IsInitialized())
		{
			BOOL bCommandAvailable = false;

			if(!m_pDevice->IsFeatureAvailable("outputLineValue", &bCommandAvailable))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_NotSupportedError;
				break;
			}

			if(!bCommandAvailable)
			{
				eReturn = EDalsaGenieNanoGigESetFunction_ReadOnDeviceError;
				break;
			}

			if(!m_pDevice->SetFeatureValue("outputLineValue", CStringA(g_lpszDalsaGenieNanoGigELineValue[eParam])))
			{
				eReturn = EDalsaGenieNanoGigESetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EDalsaGenieNanoGigESetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EDalsaGenieNanoGigESetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamDalsaGenieNanoGigE[eSaveID], g_lpszDalsaGenieNanoGigELineValue[nPreValue], g_lpszDalsaGenieNanoGigELineValue[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

void CDeviceDalsaGenieNanoGigE::CallbackFunction(SapXferCallbackInfo* pInfo)
{
	CDeviceDalsaGenieNanoGigE *pInstance = (CDeviceDalsaGenieNanoGigE *)pInfo->GetContext();

	do 
	{
		if(!pInstance)
			break;

		pInstance->NextImageIndex();

		CRavidImage* pCurrentImage = pInstance->GetImageInfo();

		BYTE* pCurrentBuffer = pCurrentImage->GetBuffer();
		BYTE** ppCurrentOffsetY = pCurrentImage->GetYOffsetTable();

		const int64_t i64Width = (int64_t)pCurrentImage->GetSizeX();
		const int64_t i64Height = (int64_t)pCurrentImage->GetSizeY();
		const int64_t i64WidthStep = (int64_t)pCurrentImage->GetWidthStep();
		const int64_t i64ImageSizeByte = (int64_t)pCurrentImage->GetImageSizeByte();
		const int64_t i64PixelSizeByte = (int64_t)pCurrentImage->GetPixelSizeByte();

		--pInstance->m_nGrabCount;

		if(pCurrentImage->IsColor())
			pInstance->m_pProcessing->Execute();
		else
		{
			BYTE* pImageBuffer = nullptr;
						
			SapBuffer* pBuf = pInstance->m_pView->GetBuffer();
			
			pBuf->GetAddress((void**)&pImageBuffer);

			memcpy(pCurrentBuffer, pImageBuffer, i64ImageSizeByte);

			pInstance->ConnectImage(false);

			CEventHandlerManager::BroadcastOnAcquisition(pCurrentImage, pInstance->GetObjectID(), pInstance->GetImageView(), pInstance);
			
			if(pInstance->m_nGrabCount <= 0)
			{
				pInstance->m_bIsGrabAvailable = true;
				pInstance->m_bIsLive = false;
				pInstance->m_pXfer->Freeze();
			}
		}		
	} 
	while(false);		
}

void CDeviceDalsaGenieNanoGigE::CallbackProcessing(SapProCallbackInfo * pInfo)
{
	CDeviceDalsaGenieNanoGigE *pInstance = (CDeviceDalsaGenieNanoGigE *)pInfo->GetContext();

	do 
	{
		BYTE* pImageBuffer = nullptr;

		pInstance->NextImageIndex();

		CRavidImage* pCurrentImage = pInstance->GetImageInfo();

		BYTE* pCurrentBuffer = pCurrentImage->GetBuffer();
		BYTE** ppCurrentOffsetY = pCurrentImage->GetYOffsetTable();

		const int64_t i64Width = (int64_t)pCurrentImage->GetSizeX();
		const int64_t i64Height = (int64_t)pCurrentImage->GetSizeY();
		const int64_t i64WidthStep = (int64_t)pCurrentImage->GetWidthStep();
		const int64_t i64ImageSizeByte = (int64_t)pCurrentImage->GetImageSizeByte();
		const int64_t i64PixelSizeByte = (int64_t)pCurrentImage->GetPixelSizeByte();


		if(pCurrentImage->IsColor())
		{
			SapBuffer* colorConvBuffer = pInstance->m_pColorConv->GetOutputBuffer();
			colorConvBuffer->GetAddress((void**)&pImageBuffer);
			if(!pImageBuffer)
				break;

			memcpy(pCurrentBuffer, pImageBuffer, i64ImageSizeByte);
		}

		pInstance->ConnectImage(false);

		CEventHandlerManager::BroadcastOnAcquisition(pCurrentImage, pInstance->GetObjectID(), pInstance->GetImageView(), pInstance);

		if(pInstance->m_nGrabCount <= 0)
		{
			pInstance->m_bIsGrabAvailable = true;
			pInstance->m_bIsLive = false;
			pInstance->m_pXfer->Freeze();
		}
	}
	while(false);	
}

bool CDeviceDalsaGenieNanoGigE::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = false;

	bool bFoundID = true;

	do
	{
		switch(nParam)
		{
		case EDeviceParameterDalsaGenieNanoGigE_DeviceID:
			bReturn = !SetDeviceID(strValue);
			break;
		case EDeviceParameterDalsaGenieNanoGigE_GrabCount:
			bReturn = !SetGrabCount(_ttoi(strValue));
			break;
		case EDeviceParameterDalsaGenieNanoGigE_CanvasWidth:
			bReturn = !SetCanvasWidth(_ttoi(strValue));
			break;
		case EDeviceParameterDalsaGenieNanoGigE_CanvasHeight:
			bReturn = !SetCanvasHeight(_ttoi(strValue));
			break;
		case EDeviceParameterDalsaGenieNanoGigE_OffsetX:
			bReturn = !SetOffsetX(_ttoi(strValue));
			break;
		case EDeviceParameterDalsaGenieNanoGigE_OffsetY:
			bReturn = !SetOffsetY(_ttoi(strValue));
			break;
		case EDeviceParameterDalsaGenieNanoGigE_PixelFormat:
			bReturn = !SetPixelFormat((EDalsaGenieNanoGigEPixelFormat)_ttoi(strValue));
			break;
		case EDeviceParameterDalsaGenieNanoGigE_PacketSize:
			bReturn = !SetPacketSize(_ttoi(strValue));
			break;
		case EDeviceParameterDalsaGenieNanoGigE_DeviceBufferCount:
			bReturn = !SetDeviceBufferCount(_ttoi(strValue));
			break;
		case EDeviceParameterDalsaGenieNanoGigE_FlipHorizontal:
			bReturn = !SetFlipHorizontal(_ttoi(strValue));
			break;
		case EDeviceParameterDalsaGenieNanoGigE_FlipVertical:
			bReturn = !SetFlipVertical(_ttoi(strValue));
			break;
		case EDeviceParameterDalsaGenieNanoGigE_TriggerSelector:
			bReturn = !SetTriggerSelector((EDalsaGenieNanoGigETriggerSelector)_ttoi(strValue));
			break;
		case EDeviceParameterDalsaGenieNanoGigE_FrameStartTriggerMode:
			bReturn = !SetFrameStartMode((EDalsaGenieNanoGigESwitch)_ttoi(strValue));
			break;
		case EDeviceParameterDalsaGenieNanoGigE_FrameStartTriggerSource:
			bReturn = !SetFrameStartTriggerSource((EDalsaGenieNanoGigESource)_ttoi(strValue));
			break;
		case EDeviceParameterDalsaGenieNanoGigE_FrameStartTriggerActivation:
			bReturn = !SetFrameStartTriggerActivation((EDalsaGenieNanoGigEActivation)_ttoi(strValue));
			break;
		case EDeviceParameterDalsaGenieNanoGigE_FrameBurstStartTriggerMode:
			bReturn = !SetFrameBurstStartMode((EDalsaGenieNanoGigESwitch)_ttoi(strValue));
			break;
		case EDeviceParameterDalsaGenieNanoGigE_FrameBurstStartTriggerSource:
			bReturn = !SetFrameBurstStartTriggerSource((EDalsaGenieNanoGigESource)_ttoi(strValue));
			break;
		case EDeviceParameterDalsaGenieNanoGigE_FrameBurstStartTriggerCount:
			bReturn = !SetFrameBurstStartTriggerCount(_ttoi(strValue));
			break;
		case EDeviceParameterDalsaGenieNanoGigE_AcquisitionStartTriggerMode:
			bReturn = !SetAcquisitionStartMode((EDalsaGenieNanoGigESwitch)_ttoi(strValue));
			break;
		case EDeviceParameterDalsaGenieNanoGigE_AcquisitionStartTriggerSource:
			bReturn = !SetAcquisitionStartTriggerSource((EDalsaGenieNanoGigESource)_ttoi(strValue));
			break;
		case EDeviceParameterDalsaGenieNanoGigE_FrameRateControl:
			bReturn = !SetFrameRateControl((EDalsaGenieNanoGigEFrameRateControl)_ttoi(strValue));
			break;
		case EDeviceParameterDalsaGenieNanoGigE_FrameRate:
			bReturn = !SetFrameRate(_ttof(strValue));
			break;
		case EDeviceParameterDalsaGenieNanoGigE_ExposureTime:
			bReturn = !SetExposureTime(_ttof(strValue));
			break;
		case EDeviceParameterDalsaGenieNanoGigE_GainSelector:
			bReturn = !SetGainSelector((EDalsaGenieNanoGigEGainSelector)_ttoi(strValue));
			break;
		case EDeviceParameterDalsaGenieNanoGigE_SensorGain:
			bReturn = !SetSensorGain(_ttof(strValue));
			break;
		case EDeviceParameterDalsaGenieNanoGigE_DigitalGain:
			bReturn = !SetDigitalGain(_ttof(strValue));
			break;
		case EDeviceParameterDalsaGenieNanoGigE_BlackLevelSelector:
			bReturn = !SetBlackLevelSelector((EDalsaGenieNanoGigEBlackLevelSelector)_ttoi(strValue));
			break;
		case EDeviceParameterDalsaGenieNanoGigE_BlackLevel:
			bReturn = !SetBlackLevel(_ttof(strValue));
			break;
		case EDeviceParameterDalsaGenieNanoGigE_LineSelector:
			bReturn = !SetLineSelector((EDalsaGenieNanoGigELineSelector)_ttoi(strValue));
			break;
		case EDeviceParameterDalsaGenieNanoGigE_Line1DebouncingPeriod:
			bReturn = !SetLine1DebouncingPeriod(_ttoi(strValue));
			break;
		case EDeviceParameterDalsaGenieNanoGigE_Line1Inverter:
			bReturn = !SetLine1Inverter((EDalsaGenieNanoGigESwitch)_ttoi(strValue));
			break;
		case EDeviceParameterDalsaGenieNanoGigE_Line2DebouncingPeriod:
			bReturn = !SetLine2DebouncingPeriod(_ttoi(strValue));
			break;
		case EDeviceParameterDalsaGenieNanoGigE_Line2Inverter:
			bReturn = !SetLine2Inverter((EDalsaGenieNanoGigESwitch)_ttoi(strValue));
			break;
		case EDeviceParameterDalsaGenieNanoGigE_Line3Source:
			bReturn = !SetLine3Source((EDalsaGenieNanoGigELineSource)_ttoi(strValue));
			break;
		case EDeviceParameterDalsaGenieNanoGigE_Line3Inverter:
			bReturn = !SetLine3Inverter((EDalsaGenieNanoGigESwitch)_ttoi(strValue));
			break;
		case EDeviceParameterDalsaGenieNanoGigE_Line3Activation:
			bReturn = !SetLine3Activation((EDalsaGenieNanoGigEActivation)_ttoi(strValue));
			break;
		case EDeviceParameterDalsaGenieNanoGigE_Line3Delay:
			bReturn = !SetLine3Delay(_ttoi(strValue));
			break;
		case EDeviceParameterDalsaGenieNanoGigE_Line3Duration:
			bReturn = !SetLine3Duration(_ttoi(strValue));
			break;
		case EDeviceParameterDalsaGenieNanoGigE_Line3Value:
			bReturn = !SetLine3Value((EDalsaGenieNanoGigELineValue)_ttoi(strValue));
			break;
		case EDeviceParameterDalsaGenieNanoGigE_Line4Source:
			bReturn = !SetLine4Source((EDalsaGenieNanoGigELineSource)_ttoi(strValue));
			break;
		case EDeviceParameterDalsaGenieNanoGigE_Line4Inverter:
			bReturn = !SetLine4Inverter((EDalsaGenieNanoGigESwitch)_ttoi(strValue));
			break;
		case EDeviceParameterDalsaGenieNanoGigE_Line4Activation:
			bReturn = !SetLine4Activation((EDalsaGenieNanoGigEActivation)_ttoi(strValue));
			break;
		case EDeviceParameterDalsaGenieNanoGigE_Line4Delay:
			bReturn = !SetLine4Delay(_ttoi(strValue));
			break;
		case EDeviceParameterDalsaGenieNanoGigE_Line4Duration:
			bReturn = !SetLine4Duration(_ttoi(strValue));
			break;
		case EDeviceParameterDalsaGenieNanoGigE_Line4Value:
			bReturn = !SetLine4Value((EDalsaGenieNanoGigELineValue)_ttoi(strValue));
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

bool CDeviceDalsaGenieNanoGigE::DoesModuleExist()
{
	bool bReturn = false;

	CString strModuleName;

	do
	{
		strModuleName.Format(_T("SapClassBasic84.dll"));

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

		if(!bReturn)
			break;

		strModuleName.Format(_T("corapi.dll"));

		eStatus = CLibraryManager::GetFoundLibrary(strModuleName);

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

		if(!bReturn)
			break;

		strModuleName.Format(_T("CorUtility.dll"));

		eStatus = CLibraryManager::GetFoundLibrary(strModuleName);

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

		if(!bReturn)
			break;

		strModuleName.Format(_T("CorLog.dll"));

		eStatus = CLibraryManager::GetFoundLibrary(strModuleName);

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