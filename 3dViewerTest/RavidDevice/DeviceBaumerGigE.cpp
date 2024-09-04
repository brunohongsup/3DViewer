#include "stdafx.h"

#include "DeviceBaumerGigE.h"

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
#include "../RavidFramework/FrameworkManager.h"
#include "../RavidFramework/EventHandlerManager.h"
#include "../RavidFramework/MultiLanguageManager.h" 

#include "../RavidFramework/RavidImageView.h"

#include "../Libraries/Includes/Baumer/bgapi2_genicam.hpp"

// bgapi2_genicam.dll
#pragma comment(lib, COMMONLIB_PREFIX "Baumer/bgapi2_genicam.lib")

using namespace Ravid;
using namespace Ravid::Device;
using namespace Ravid::Database;
using namespace Ravid::Framework;
using namespace Ravid::Algorithms;
using namespace Ravid::Miscellaneous;

IMPLEMENT_DYNAMIC(CDeviceBaumerGigE, CDeviceCamera)

BEGIN_MESSAGE_MAP(CDeviceBaumerGigE, CDeviceCamera)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

CCriticalSection CDeviceBaumerGigE::m_csDeviceList;
BGAPI2::ImageProcessor* CDeviceBaumerGigE::m_pImgProc;

bool __InternalDeviceOpenCheck(BGAPI2::Device* pDevice = nullptr)
{
	bool bReturn = true;

	do
	{
		if(!pDevice)
			break;

		try
		{
			bReturn = pDevice->IsOpen();
		}
		catch(BGAPI2::Exceptions::IException&)
		{
			bReturn = false;
		}
	}
	while(false);

	return bReturn;
}

bool __InternalInterfaceOpenCheck(BGAPI2::Interface* pInterface = nullptr)
{
	bool bReturn = true;

	do 
	{
		if(!pInterface)
			break;

		try
		{
			BGAPI2::DeviceList* pDeviceList = pInterface->GetDevices();
			if(!pDeviceList)
				break;

			for(BGAPI2::DeviceList::iterator devIter = pDeviceList->begin(); devIter != pDeviceList->end(); devIter++)
			{
				if(!__InternalDeviceOpenCheck(devIter->second))
					continue;

				bReturn = false;

				break;
			}

		}
		catch(BGAPI2::Exceptions::IException&)
		{
			bReturn = false;
		}
	} 
	while(false);

	return bReturn;
}

bool __InternalSystemOpenCheck(BGAPI2::System* pSystem = nullptr)
{
	bool bReturn = true;

	do
	{
		if(!pSystem)
			break;

		try
		{
			BGAPI2::InterfaceList* pInterfaceList = pSystem->GetInterfaces();
			if(!pInterfaceList)
				break;

			for(BGAPI2::InterfaceList::iterator devIter = pInterfaceList->begin(); devIter != pInterfaceList->end(); devIter++)
			{
				if(!__InternalInterfaceOpenCheck(devIter->second))
					continue;

				bReturn = false;

				break;
			}

		}
		catch(BGAPI2::Exceptions::IException&)
		{
			bReturn = false;
		}
	}
	while(false);

	return bReturn;
}

static LPCTSTR g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_Count] =
{
	_T("DeviceID"),
	_T("Camera Setting"),
	_T("Grab count"),
	_T("Canvas width"),
	_T("Canvas height"),
	_T("Offset X"),
	_T("Offset Y"),
	_T("Grab Waiting Time[ms]"),
	_T("Grab Buffer Count"),
	_T("Pixel Format"),
	_T("Flip Horizontal"),
	_T("Flip Vertical"),
	_T("Trigger Setting"),
	_T("Trigger Selector"),
	_T("FrameStart"),
	_T("FrameStart Trigger Mode"),
	_T("FrameStart Trigger Source"),
	_T("FrameStart Trigger Activation"),
	_T("LineStart"),
	_T("LineStart Trigger Mode"),
	_T("LineStart Trigger Source"),
	_T("LineStart Trigger Activation"),
	_T("AcquisitionStart"),
	_T("AcquisitionStart Trigger Mode"),
	_T("AcquisitionStart Trigger Source"),
	_T("AcquisitionStart Trigger Activation"),
	_T("Exposure and Gain Setting"),
	_T("Exposure Auto"),
	_T("Exposure Mode"),
	_T("Exposure"),
	_T("Gain Auto"),
	_T("Gain Selector"),
	_T("Gain"),
	_T("Analog Gain"),
	_T("Digital Gain"),
	_T("Tap1 Gain"),
	_T("Tap2 Gain"),
	_T("Digital IO Control"),
	_T("LineSelector"),
	_T("Line0"),
	_T("Line0 Inverter"),
	_T("Line0 Mode"),
	_T("Line1"),
	_T("Line1 Inverter"),
	_T("Line1 Mode"),
	_T("Line1 Source"),
	_T("Line2"),
	_T("Line2 Inverter"),
	_T("Line2 Mode"),
	_T("Line2 Source"),
	_T("Line3"),
	_T("Line3 Inverter"),
	_T("Line3 Mode"),
	_T("Line3 Source"),
	_T("User Output Selector"),
	_T("User Output Value"),
	_T("User Output ValueAll"),
	_T("Timer Control"),
	_T("Timer Selector"),
	_T("Timer Trigger Source"),
	_T("Timer Trigger Activation"),
	_T("Timer Duration"),
	_T("Timer Delay"),
};

static LPCTSTR g_lpszPixelFormat[EDeviceBaumerPixelFormat_Count] =
{
	_T("Mono8"),
	_T("Mono10"),
	_T("Mono12"),
	_T("Mono14"),
	_T("Mono16"),
	_T("BayerBG8"),
	_T("BayerBG10"),
	_T("BayerBG12"),
	_T("BayerBG16"),
	_T("BayerGB8"),
	_T("BayerGB10"),
	_T("BayerGB12"),
	_T("BayerGB16"),
	_T("BayerGR8"),
	_T("BayerGR10"),
	_T("BayerGR12"),
	_T("BayerGR16"),
	_T("BayerRG8"),
	_T("BayerRG10"),
	_T("BayerRG12"),
	_T("BayerRG16"),
	_T("RGB8"),
	_T("RGB10"),
	_T("RGB12"),
	_T("RGB16"),
	_T("BGR8"),
	_T("BGR10"),
	_T("BGR12"),
	_T("BGR16")
};

static LPCTSTR g_lpszTriggerSelect[EDeviceBaumerTriggerSelector_Count] =
{
	_T("FrameStart"),
	_T("LineStart"),
	_T("AcquisitionStart")
};

static LPCTSTR g_lpszSwitch[EDeviceBaumerSwitch_Count] =
{
	_T("Off"),
	_T("On")
};

static LPCTSTR g_lpszSource[EDeviceBaumerTriggerSource_Count] =
{
	_T("Software"),
	_T("Line1"),
	_T("Line2"),
	_T("Line3"),
	_T("Line4"),
	_T("Line5"),
	_T("Line6"),
	_T("Line7"),
	_T("Line8"),
	_T("CC1"),
	_T("CC2"),
	_T("CC3"),
	_T("CC4"),
	_T("Action1"),
	_T("Action2"),
	_T("Action3"),
	_T("Action4")
};

static LPCTSTR g_lpszActivation[EDeviceBaumerTriggerActivation_Count] =
{
	_T("RisingEdge"),
	_T("FallingEdge"),
	_T("AnyEdge"),
	_T("LevelHigh"),
	_T("LevelLow")
};

static LPCTSTR g_lpszAuto[EDeviceBaumerAuto_Count] =
{
	_T("Off"),
	_T("Once"),
	_T("Continuous"),
};

static LPCTSTR g_lpszGainSelect[EDeviceBaumerGainSelector_Count] =
{
	_T("All"),
	_T("AnalogAll"),
	_T("DigitalAll")
};

static LPCTSTR g_lpszExposureMode[EDeviceBaumerExposureMode_Count] =
{
	_T("Off"),
	_T("Timed"),
	_T("TriggerWidth"),
	_T("TriggerControlledl")
};

static LPCTSTR g_lpszLineSelector[EDeviceBaumerLineSelector_Count] =
{
	_T("Line0"),
	_T("Line1"),
	_T("Line2"),
	_T("Line3"),
};

static LPCTSTR g_lpszLineMode[EDeviceBaumerLineMode_Count] =
{
	_T("Input"),
	_T("Output"),
};

static LPCTSTR g_lpszLineSource[EDeviceBaumerLineSource_Count] =
{
	_T("Off"),
	_T("ExposureActive"),
	_T("ReadoutActive"),
	_T("Timer1Active"),
	_T("TriggerReady"),
	_T("UserOutput1"),
};

static LPCTSTR g_lpszUserOutputSelector[EDeviceBaumerUserOutputSelector_Count] =
{
	_T("UserOutput0"),
	_T("UserOutput1"),
	_T("UserOutput2"),
	_T("UserOutput3"),
};

static LPCTSTR g_lpszTimerSelector[EDeviceBaumerTimerSelector_Count] =
{
	_T("Timer1"),
	_T("Timer2"),
	_T("Timer3"),
	_T("Timer4"),
	_T("Timer5"),
	_T("Timer6"),
	_T("Timer7"),
	_T("Timer8"),
};

static LPCTSTR g_lpszTimerTriggerSource[EDeviceBaumerTimerTriggerSource_Count] =
{
	_T("Off"),
	_T("Action1"),
	_T("ExposureEnd"),
	_T("ExposureStart"),
	_T("FrameTransferSkipped"),
	_T("Line0"),
	_T("Software"),
	_T("TriggerSkipped"),
};


CDeviceBaumerGigE::CDeviceBaumerGigE()
{
	m_pImgProc = nullptr;
}


CDeviceBaumerGigE::~CDeviceBaumerGigE()
{
	Terminate();
}

EDeviceInitializeResult CDeviceBaumerGigE::Initialize()
{
	EDeviceInitializeResult eReturn = EDeviceInitializeResult_UnknownError;

	CString strMessage, strStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("Baumer"));		
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

		CString strInputSerial = GetDeviceID();

		BGAPI2::SystemList* pSystemList = nullptr;

		try
		{
			pSystemList = BGAPI2::SystemList::GetInstance();
			pSystemList->Refresh();
		}
		catch(BGAPI2::Exceptions::IException&){}

		if(!pSystemList)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("System List"));
			eReturn = EDeviceInitializeResult_EnumerateDeviceError;
			break;
		}

		if(!m_pImgProc)
			m_pImgProc = BGAPI2::ImageProcessor::GetInstance();

		int nSystemSize = pSystemList->size();

		for(BGAPI2::SystemList::iterator sysIter = pSystemList->begin(); sysIter != pSystemList->end(); sysIter++)
		{
			BGAPI2::InterfaceList* pInterfaceList = nullptr;

			m_pSystem = sysIter->second;

			if(!m_pSystem)
				continue;

			try
			{
				if(!m_pSystem->IsOpen())
					m_pSystem->Open();
			}
			catch(BGAPI2::Exceptions::IException&)
			{
				if(m_pSystem->IsOpen())
					m_pSystem->Close();
			}

			if(!m_pSystem->IsOpen())
				continue;

			try
			{
				pInterfaceList = m_pSystem->GetInterfaces();
				pInterfaceList->Refresh(100);
			}
			catch(BGAPI2::Exceptions::IException&)
			{
				if(m_pSystem->IsOpen())
					m_pSystem->Close();
			}
			
			if(!pInterfaceList)
			{
				if(m_pSystem->IsOpen())
					m_pSystem->Close();

				continue;
			}

			int nInterfaceSize = pInterfaceList->size();

			for(BGAPI2::InterfaceList::iterator intIter = pInterfaceList->begin(); intIter != pInterfaceList->end(); intIter++)
			{
				BGAPI2::DeviceList* pDeviceList = nullptr;

				m_pInterface = intIter->second;

				if(!m_pInterface)
					continue;

				try
				{
					if(!m_pInterface->IsOpen())
						m_pInterface->Open();
				}
				catch(BGAPI2::Exceptions::IException&)
				{
					if(m_pInterface->IsOpen())
						m_pInterface->Close();
				}

				if(!m_pInterface->IsOpen())
					continue;

				try
				{
					pDeviceList = intIter->second->GetDevices();
					pDeviceList->Refresh(100);
				}
				catch(BGAPI2::Exceptions::IException&) 
				{
					if(m_pInterface->IsOpen())
						m_pInterface->Close();
				}

				if(!pDeviceList)
				{
					if(m_pInterface->IsOpen())
						m_pInterface->Close();

					continue;
				}

				int nDeviceSize = pDeviceList->size();

				for(BGAPI2::DeviceList::iterator devIter = pDeviceList->begin(); devIter != pDeviceList->end(); devIter++)
				{
					BGAPI2::Device* pDevice = devIter->second;

					if(!pDevice)
						continue;

					try
					{
						if(pDevice->IsOpen())
							continue;

						if(!pDevice->IsOpen())
							pDevice->Open();
					}
					catch(BGAPI2::Exceptions::IException&)
					{
						if(pDevice->IsOpen())
							pDevice->Close();
					}

					if(!pDevice->IsOpen())
						continue;

					try
					{
						CString strSerial;
						strSerial.Format(_T("%s"), CString(pDevice->GetSerialNumber()));

						if(!strSerial.CompareNoCase(strInputSerial))
						{
							m_pDevice = pDevice;
							break;
						}

						pDevice->Close();
					}
					catch(BGAPI2::Exceptions::IException&)
					{						
					}

					if(m_pDevice)
						break;

					m_pDevice = nullptr;
				}

				if(m_pDevice)
					break;

				if(__InternalInterfaceOpenCheck(m_pInterface))
				{
					if(m_pInterface->IsOpen())
						m_pInterface->Close();

					m_pInterface = nullptr;
				}
			}

			if(m_pDevice)
				break;

			if(__InternalSystemOpenCheck(m_pSystem))
			{
				if(m_pSystem->IsOpen())
					m_pSystem->Close();

				m_pSystem = nullptr;
			}
		}

		if(!m_pDevice)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntopenthedevice);
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		// camera setting
		//////////////////////////////////////////////////////////////////////////
				
		int nTime = 0;

		if(GetGrabWaitingTime(&nTime))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("Waiting Time"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		m_nWaitTime = nTime;

		//CString strFormat;
		EDeviceBaumerPixelFormat ePixelFormat = EDeviceBaumerPixelFormat_Count;

		try
		{
			if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_PIXELFORMAT))
			{
				if(!m_pDevice->GetRemoteNode(SFNC_PIXELFORMAT)->GetLocked())
				{
					if(GetPixelFormat(&ePixelFormat))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("PixelFormat"));						
						eReturn = EDeviceInitializeResult_CanNotReadDBPixelFormat;
						break;
					}

					try
					{
						m_pDevice->GetRemoteNode(SFNC_PIXELFORMAT)->SetValue(CStringA(g_lpszPixelFormat[ePixelFormat]).GetBuffer());
					}
					catch(BGAPI2::Exceptions::IException&)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("PixelFormat"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}
				}
				else
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("PixelFormat"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}
			}
			else
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntsupportthe_s), _T("PixelFormat"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}
		}
		catch(BGAPI2::Exceptions::IException&)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntsupportthe_s), _T("Access to Pixel Format"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		int nWidth = 0, nHeight = 0;

		try
		{
			if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_WIDTH))
			{
				if(GetCanvasWidth(&nWidth))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("CanvasWidth"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				try
				{
					if(!nWidth)
					{
						if(!m_pDevice->GetRemoteNode(SFNC_WIDTH)->GetLocked())
							nWidth = m_pDevice->GetRemoteNode(SFNC_WIDTH)->GetIntMax();
					}

					if(!m_pDevice->GetRemoteNode(SFNC_WIDTH)->GetLocked())
						m_pDevice->GetRemoteNode(SFNC_WIDTH)->SetInt(nWidth);

					nWidth = m_pDevice->GetRemoteNode(SFNC_WIDTH)->GetInt();

					if(SetCanvasWidth(nWidth))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("CanvasWidth"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
				}
				catch(BGAPI2::Exceptions::IException&)
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Width"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}				
			}
			else
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntsupportthe_s), _T("Width"));
				eReturn = EDeviceInitializeResult_NotSupportedDeviceError;
				break;
			}

		}
		catch(BGAPI2::Exceptions::IException&)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntsupportthe_s), _T("Access to Width"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		try
		{
			if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_HEIGHT))
			{
				if(GetCanvasHeight(&nHeight))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("CanvasHeight"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				try
				{
					if(!nHeight)
					{
						if(!m_pDevice->GetRemoteNode(SFNC_HEIGHT)->GetLocked())
							nHeight= m_pDevice->GetRemoteNode(SFNC_HEIGHT)->GetIntMax();
					}

					if(!m_pDevice->GetRemoteNode(SFNC_HEIGHT)->GetLocked())
						m_pDevice->GetRemoteNode(SFNC_HEIGHT)->SetInt(nHeight);

					nHeight = m_pDevice->GetRemoteNode(SFNC_HEIGHT)->GetInt();

					if(SetCanvasHeight(nHeight))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedatabase), _T("CanvasHeight"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
				}
				catch(BGAPI2::Exceptions::IException&)
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Height"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}
			}
			else
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntsupportthe_s), _T("Height"));
				eReturn = EDeviceInitializeResult_NotSupportedDeviceError;
				break;
			}

		}
		catch(BGAPI2::Exceptions::IException&)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntsupportthe_s), _T("Access to Height"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		int nOffsetX = 0, nOffsetY = 0;
		
		try
		{
			if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_OFFSETX))
			{
				if(GetOffsetX(&nOffsetX))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("OffsetX"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(!m_pDevice->GetRemoteNode(SFNC_OFFSETX)->GetLocked())
					m_pDevice->GetRemoteNode(SFNC_OFFSETX)->SetValue(nOffsetX);
			}
		}
		catch(BGAPI2::Exceptions::IException&){}
		
		try
		{
			if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_OFFSETY))
			{
				if(GetOffsetY(&nOffsetY))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("OffsetY"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(!m_pDevice->GetRemoteNode(SFNC_OFFSETY)->GetLocked())
					m_pDevice->GetRemoteNode(SFNC_OFFSETY)->SetValue(nOffsetY);
			}
		}
		catch(BGAPI2::Exceptions::IException&){}

		try
		{
			if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_TRIGGERSELECTOR))
			{
				if(!m_pDevice->GetRemoteNode(SFNC_TRIGGERSELECTOR)->GetLocked())
				{
					if(m_pDevice->GetRemoteNode(SFNC_TRIGGERSELECTOR)->GetEnumNodeList()->GetNodePresent(SFNC_TRIGGERSELECTORVALUE_FRAMESTART))
					{
						m_pDevice->GetRemoteNode(SFNC_TRIGGERSELECTOR)->SetString(SFNC_TRIGGERSELECTORVALUE_FRAMESTART);
						
						if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_TRIGGERSOURCE))
						{
							if(!m_pDevice->GetRemoteNode(SFNC_TRIGGERSOURCE)->GetLocked())
							{
								EDeviceBaumerTriggerSource eTriggerSource = EDeviceBaumerTriggerSource_Count;
								if(GetFrameStartTriggerSource(&eTriggerSource))
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("FrameStart TriggerSource"));
									eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
									break;
								}

								if(m_pDevice->GetRemoteNode(SFNC_TRIGGERSOURCE)->GetEnumNodeList()->GetNodePresent(CStringA(g_lpszSource[eTriggerSource]).GetBuffer()))
									m_pDevice->GetRemoteNode(SFNC_TRIGGERSOURCE)->SetString(CStringA(g_lpszSource[eTriggerSource]).GetBuffer());
							}
						}
						
						if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_TRIGGERMODE))
						{
							if(!m_pDevice->GetRemoteNode(SFNC_TRIGGERMODE)->GetLocked())
							{
								//CString strMode;
								EDeviceBaumerSwitch eSwitch = EDeviceBaumerSwitch_Count;
								if(GetFrameStartMode(&eSwitch))
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("FrameStart Mode"));
									eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
									break;
								}

								if(m_pDevice->GetRemoteNode(SFNC_TRIGGERMODE)->GetEnumNodeList()->GetNodePresent(CStringA(g_lpszSwitch[eSwitch]).GetBuffer()))
									m_pDevice->GetRemoteNode(SFNC_TRIGGERMODE)->SetString(CStringA(g_lpszSwitch[eSwitch]).GetBuffer());
							}
						}						
					}
				}
			}
		}
		catch(BGAPI2::Exceptions::IException&) { }
		
		try
		{
			if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_TRIGGERSELECTOR))
			{
				if(!m_pDevice->GetRemoteNode(SFNC_TRIGGERSELECTOR)->GetLocked())
				{
					if(m_pDevice->GetRemoteNode(SFNC_TRIGGERSELECTOR)->GetEnumNodeList()->GetNodePresent(SFNC_TRIGGERSELECTORVALUE_LINESTART))
					{
						m_pDevice->GetRemoteNode(SFNC_TRIGGERSELECTOR)->SetString(SFNC_TRIGGERSELECTORVALUE_LINESTART);

						if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_TRIGGERSOURCE))
						{
							if(!m_pDevice->GetRemoteNode(SFNC_TRIGGERSOURCE)->GetLocked())
							{
								EDeviceBaumerTriggerSource eTriggerSource = EDeviceBaumerTriggerSource_Count;
								if(GetLineStartTriggerSource(&eTriggerSource))
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineStart TriggerSource"));
									eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
									break;
								}

								if(m_pDevice->GetRemoteNode(SFNC_TRIGGERSOURCE)->GetEnumNodeList()->GetNodePresent(CStringA(g_lpszSource[eTriggerSource]).GetBuffer()))
									m_pDevice->GetRemoteNode(SFNC_TRIGGERSOURCE)->SetString(CStringA(g_lpszSource[eTriggerSource]).GetBuffer());
							}
						}

						if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_TRIGGERMODE))
						{
							if(!m_pDevice->GetRemoteNode(SFNC_TRIGGERMODE)->GetLocked())
							{
								//CString strMode;
								EDeviceBaumerSwitch eSwitch = EDeviceBaumerSwitch_Count;
								if(GetLineStartMode(&eSwitch))
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineStart Mode"));
									eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
									break;
								}

								if(m_pDevice->GetRemoteNode(SFNC_TRIGGERMODE)->GetEnumNodeList()->GetNodePresent(CStringA(g_lpszSwitch[eSwitch]).GetBuffer()))
									m_pDevice->GetRemoteNode(SFNC_TRIGGERMODE)->SetString(CStringA(g_lpszSwitch[eSwitch]).GetBuffer());
							}
						}
					}
				}
			}
		}
		catch(BGAPI2::Exceptions::IException&) { }

		try
		{
			if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_TRIGGERSELECTOR))
			{
				if(!m_pDevice->GetRemoteNode(SFNC_TRIGGERSELECTOR)->GetLocked())
				{
					if(m_pDevice->GetRemoteNode(SFNC_TRIGGERSELECTOR)->GetEnumNodeList()->GetNodePresent(SFNC_TRIGGERSELECTORVALUE_ACQUISITIONSTART))
					{
						m_pDevice->GetRemoteNode(SFNC_TRIGGERSELECTOR)->SetString(SFNC_TRIGGERSELECTORVALUE_ACQUISITIONSTART);

						if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_TRIGGERSOURCE))
						{
							if(!m_pDevice->GetRemoteNode(SFNC_TRIGGERSOURCE)->GetLocked())
							{
								EDeviceBaumerTriggerSource eTriggerSource = EDeviceBaumerTriggerSource_Count;
								if(GetAcquisitionStartTriggerSource(&eTriggerSource))
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AcquisitionStart TriggerSource"));
									eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
									break;
								}

								if(m_pDevice->GetRemoteNode(SFNC_TRIGGERSOURCE)->GetEnumNodeList()->GetNodePresent(CStringA(g_lpszSource[eTriggerSource]).GetBuffer()))
									m_pDevice->GetRemoteNode(SFNC_TRIGGERSOURCE)->SetString(CStringA(g_lpszSource[eTriggerSource]).GetBuffer());
							}
						}

						if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_TRIGGERMODE))
						{
							if(!m_pDevice->GetRemoteNode(SFNC_TRIGGERMODE)->GetLocked())
							{
								//CString strMode;
								EDeviceBaumerSwitch eSwitch = EDeviceBaumerSwitch_Count;
								if(GetAcquisitionStartMode(&eSwitch))
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AcquisitionStart Mode"));
									eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
									break;
								}

								if(m_pDevice->GetRemoteNode(SFNC_TRIGGERMODE)->GetEnumNodeList()->GetNodePresent(CStringA(g_lpszSwitch[eSwitch]).GetBuffer()))
									m_pDevice->GetRemoteNode(SFNC_TRIGGERMODE)->SetString(CStringA(g_lpszSwitch[eSwitch]).GetBuffer());
							}
						}
					}
				}
			}
		}
		catch(BGAPI2::Exceptions::IException&) { }


		try
		{
			if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_TRIGGERSELECTOR))
			{
				if(!m_pDevice->GetRemoteNode(SFNC_TRIGGERSELECTOR)->GetLocked())
				{
					EDeviceBaumerTriggerSelector eTriggerSelector = EDeviceBaumerTriggerSelector_FrameStart;
					if(GetTriggerSelector(&eTriggerSelector))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Trigger Selector"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}
					
					if(m_pDevice->GetRemoteNode(SFNC_TRIGGERSELECTOR)->GetEnumNodeList()->GetNodePresent(CStringA(g_lpszTriggerSelect[eTriggerSelector]).GetBuffer()))
						m_pDevice->GetRemoteNode(SFNC_TRIGGERSELECTOR)->SetString(CStringA(g_lpszTriggerSelect[eTriggerSelector]).GetBuffer());
				}
			}
		}
		catch(BGAPI2::Exceptions::IException&) { }

		bool bError = false;

		try
		{
			//CString strExposureAuto;
			EDeviceBaumerAuto eExposureAuto = EDeviceBaumerAuto_Count;

			if(GetExposureAuto(&eExposureAuto))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ExposureAuto"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(eExposureAuto != EDeviceBaumerAuto_Off)
			{
				if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_EXPOSUREAUTO))
				{
					if(!m_pDevice->GetRemoteNode(SFNC_EXPOSUREAUTO)->GetLocked())
					{
						try
						{
							m_pDevice->GetRemoteNode(SFNC_EXPOSUREAUTO)->SetValue(CStringA(g_lpszAuto[eExposureAuto]).GetBuffer());
						}
						catch(BGAPI2::Exceptions::IException&)
						{
							bError = true;
						}

						if(bError)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ExposureAuto"));
							eReturn = EDeviceInitializeResult_WriteToDeviceError;
							break;
						}
					}
				}
				else
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ExposureAuto"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}
			}
			else
			{
				if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_EXPOSUREMODE))
				{
					if(!m_pDevice->GetRemoteNode(SFNC_EXPOSUREMODE)->GetLocked())
					{
						//CString strExposureMode;
						EDeviceBaumerExposureMode eExposureMode = EDeviceBaumerExposureMode_Count;

						if(GetExposureMode(&eExposureMode))
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("ExposureMode"));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}

						try
						{
							m_pDevice->GetRemoteNode(SFNC_EXPOSUREMODE)->SetValue(CStringA(g_lpszExposureMode[eExposureMode]).GetBuffer());

							CString strExposure;

							if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_EXPOSURETIME))
								strExposure = SFNC_EXPOSURETIME;
							else if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_EXPOSURETIMEABS))
								strExposure = SFNC_EXPOSURETIMEABS;

							if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(CStringA(strExposure).GetBuffer()))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Exposure"));
								eReturn = EDeviceInitializeResult_WriteToDeviceError;
								break;
							}

							if(m_pDevice->GetRemoteNode(CStringA(strExposure).GetBuffer())->GetLocked())
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("ExposureTime"));
								eReturn = EDeviceInitializeResult_WriteToDeviceError;

								break;
							}

							int nExposure = 0;
							if(GetExposure(&nExposure))
							{
								strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Exposure"));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							m_pDevice->GetRemoteNode(CStringA(strExposure).GetBuffer())->SetInt(nExposure);
						}
						catch(BGAPI2::Exceptions::IException&)
						{
							bError = true;
						}

						if(bError)
						{
							strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("exposure mode or exposure value"));
							eReturn = EDeviceInitializeResult_WriteToDeviceError;
							break;
						}
					}
				}
			}

			EDeviceBaumerAuto eGainAuto = EDeviceBaumerAuto_Count;

			if(GetGainAuto(&eGainAuto))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("GainAuto"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_GAINAUTO))
			{
				if(!m_pDevice->GetRemoteNode(SFNC_GAINAUTO)->GetLocked())
				{

					try
					{
						m_pDevice->GetRemoteNode(SFNC_GAINAUTO)->SetValue(CStringA(g_lpszAuto[eGainAuto]).GetBuffer());
					}
					catch(const std::exception&)
					{
						bError = true;
					}

					if(bError)
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("GainAuto"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}

				}
			}

			if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_USEROUTPUTSELECTOR))
			{
				if(!m_pDevice->GetRemoteNode(SFNC_USEROUTPUTSELECTOR)->GetLocked())
				{
					EDeviceBaumerUserOutputSelector eOutputSelector = EDeviceBaumerUserOutputSelector_Count;

					if(GetUserOutputSelector(&eOutputSelector))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("UserOutputSelector"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					try
					{
						m_pDevice->GetRemoteNode(SFNC_USEROUTPUTSELECTOR)->SetString(CStringA(g_lpszUserOutputSelector[eOutputSelector]).GetBuffer());
					}
					catch(BGAPI2::Exceptions::IException&) { }
				}
			}

			if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_USEROUTPUTVALUE))
			{
				if(!m_pDevice->GetRemoteNode(SFNC_USEROUTPUTVALUE)->GetLocked())
				{
					EDeviceBaumerSwitch eOutputValue = EDeviceBaumerSwitch_Count;

					if(GetUserOutputValue(&eOutputValue))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("UserOutputValue"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					try
					{
						m_pDevice->GetRemoteNode(SFNC_USEROUTPUTVALUE)->SetBool(eOutputValue);
					}
					catch(BGAPI2::Exceptions::IException&) { }
				}
			}

			if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_USEROUTPUTVALUEALL))
			{
				if(!m_pDevice->GetRemoteNode(SFNC_USEROUTPUTVALUEALL)->GetLocked())
				{
					int nOutputValue = 0;

					if(GetUserOutputValueAll(&nOutputValue))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("UserOutputValueAll"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					try
					{
						m_pDevice->GetRemoteNode(SFNC_USEROUTPUTVALUEALL)->SetInt(nOutputValue);
					}
					catch(BGAPI2::Exceptions::IException&) { }
				}
			}

			if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_TIMERSELECTOR))
			{
				if(!m_pDevice->GetRemoteNode(SFNC_TIMERSELECTOR)->GetLocked())
				{
					EDeviceBaumerTimerSelector eTimerSelector = EDeviceBaumerTimerSelector_Count;

					if(GetTimerSelector(&eTimerSelector))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TimerSelector"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					try
					{
						m_pDevice->GetRemoteNode(SFNC_TIMERSELECTOR)->SetString(CStringA(g_lpszTimerSelector[eTimerSelector]).GetBuffer());
					}
					catch(BGAPI2::Exceptions::IException&) { }
				}
			}

			if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_TIMERTRIGGERSOURCE))
			{
				if(!m_pDevice->GetRemoteNode(SFNC_TIMERTRIGGERSOURCE)->GetLocked())
				{
					EDeviceBaumerTimerTriggerSource eTimerSource = EDeviceBaumerTimerTriggerSource_Count;

					if(GetTimerTriggerSource(&eTimerSource))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TimerSource"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					try
					{
						m_pDevice->GetRemoteNode(SFNC_TIMERTRIGGERSOURCE)->SetString(CStringA(g_lpszTimerTriggerSource[eTimerSource]).GetBuffer());
					}
					catch(BGAPI2::Exceptions::IException&) { }
				}
			}

			if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_TIMERTRIGGERACTIVATION))
			{
				if(!m_pDevice->GetRemoteNode(SFNC_TIMERTRIGGERACTIVATION)->GetLocked())
				{
					EDeviceBaumerTriggerActivation eTriggerActivation = EDeviceBaumerTriggerActivation_Count;

					if(GetTimerTriggerActivation(&eTriggerActivation))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TimerTriggerActivation"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					try
					{
						m_pDevice->GetRemoteNode(SFNC_TIMERTRIGGERACTIVATION)->SetString(CStringA(g_lpszActivation[eTriggerActivation]).GetBuffer());
					}
					catch(BGAPI2::Exceptions::IException&) { }
				}
			}

			if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_TIMERDURATION))
			{
				if(!m_pDevice->GetRemoteNode(SFNC_TIMERDURATION)->GetLocked())
				{
					double dblTimerDuration = 0.;

					if(GetTimerDuration(&dblTimerDuration))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TimerDuration"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					try
					{
						m_pDevice->GetRemoteNode(SFNC_TIMERDURATION)->SetDouble(dblTimerDuration);
					}
					catch(BGAPI2::Exceptions::IException&) { }
				}
			}

			if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_TIMERDELAY))
			{
				if(!m_pDevice->GetRemoteNode(SFNC_TIMERDELAY)->GetLocked())
				{
					double dblTimerDelay = 0.;

					if(GetTimerDelay(&dblTimerDelay))
					{
						strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("TimerDelay"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					try
					{
						m_pDevice->GetRemoteNode(SFNC_TIMERDELAY)->SetDouble(dblTimerDelay);
					}
					catch(BGAPI2::Exceptions::IException&) { }
				}
			}

			BGAPI2::String strValue;

			if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_GAINSELECTOR))
			{
				if(!m_pDevice->GetRemoteNode(SFNC_GAINSELECTOR)->GetLocked())
				{
					if(m_pDevice->GetRemoteNode(SFNC_GAINSELECTOR)->GetEnumNodeList()->GetNodePresent(SFNC_GAINSELECTORVALUE_ALL))
					{
						try
						{
							m_pDevice->GetRemoteNode(SFNC_GAINSELECTOR)->SetValue(SFNC_GAINSELECTORVALUE_ALL);

							if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_GAIN))
							{
								try
								{
									double dblMin = m_pDevice->GetRemoteNode(SFNC_GAIN)->GetDoubleMin();
									double dblMax = m_pDevice->GetRemoteNode(SFNC_GAIN)->GetDoubleMax();

									double dblGain = 0.;

									if(GetAllGain(&dblGain))
									{
										strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AllGain"));
										eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
										break;
									}

									if(dblGain < dblMin || dblGain > dblMax)
									{
										strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("AllGain"));
										eReturn = EDeviceInitializeResult_WriteToDeviceError;
										break;
									}

									m_pDevice->GetRemoteNode(SFNC_GAIN)->SetDouble(dblGain);
								}
								catch(const std::exception&)
								{
									bError = true;
								}

								if(bError)
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("AllGain"));
									eReturn = EDeviceInitializeResult_WriteToDeviceError;
									break;
								}
							}
						}
						catch(const std::exception&) { }
					}

					if(m_pDevice->GetRemoteNode(SFNC_GAINSELECTOR)->GetEnumNodeList()->GetNodePresent(SFNC_GAINSELECTORVALUE_ANALOGALL))
					{
						try
						{
							m_pDevice->GetRemoteNode(SFNC_GAINSELECTOR)->SetValue(SFNC_GAINSELECTORVALUE_ANALOGALL);

							if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_GAIN))
							{
								try
								{
									int nMin = m_pDevice->GetRemoteNode(SFNC_GAIN)->GetIntMin();
									int nMax = m_pDevice->GetRemoteNode(SFNC_GAIN)->GetIntMax();

									int nGain = 0;
									if(GetAnalogGain(&nGain))
									{
										strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("AnalogGain"));
										eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
										break;
									}

									if(nGain < nMin || nGain > nMax)
									{
										strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("AnalogGain"));
										eReturn = EDeviceInitializeResult_WriteToDeviceError;
										break;
									}

									m_pDevice->GetRemoteNode(SFNC_GAIN)->SetInt(nGain);
								}
								catch(const std::exception&)
								{
									bError = true;
								}

								if(bError)
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("AnalogGain"));
									eReturn = EDeviceInitializeResult_WriteToDeviceError;
									break;
								}
							}
						}
						catch(const std::exception&) { }
					}

					if(m_pDevice->GetRemoteNode(SFNC_GAINSELECTOR)->GetEnumNodeList()->GetNodePresent(SFNC_GAINSELECTORVALUE_DIGITALALL))
					{
						try
						{
							m_pDevice->GetRemoteNode(SFNC_GAINSELECTOR)->SetValue(SFNC_GAINSELECTORVALUE_DIGITALALL);

							if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_GAIN))
							{
								try
								{
									int nMin = m_pDevice->GetRemoteNode(SFNC_GAIN)->GetIntMin();
									int nMax = m_pDevice->GetRemoteNode(SFNC_GAIN)->GetIntMax();

									int nGain = 0;
									if(GetDigitalGain(&nGain))
									{
										strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("DigitalGain"));
										eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
										break;
									}

									if(nGain < nMin || nGain > nMax)
									{
										strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("DigitalGain"));
										eReturn = EDeviceInitializeResult_WriteToDeviceError;
										break;
									}

									m_pDevice->GetRemoteNode(SFNC_GAIN)->SetInt(nGain);
								}
								catch(const std::exception&)
								{
									bError = true;
								}

								if(bError)
								{
									strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("DigitalGain"));
									eReturn = EDeviceInitializeResult_WriteToDeviceError;
									break;
								}
							}
						}
						catch(const std::exception&) { }
					}
				}
			}

			if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_LINESELECTOR))
			{
				EDeviceBaumerLineSelector eLineSelector = EDeviceBaumerLineSelector_Count;

				if(GetLineSelector(&eLineSelector))
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("LineSelector"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				bool bErrorSetting = true;

				try
				{
					m_pDevice->GetRemoteNode(SFNC_LINESELECTOR)->SetString(CStringA(g_lpszLineSelector[eLineSelector]).GetBuffer());
				}
				catch(BGAPI2::Exceptions::IException&)
				{
					bErrorSetting = false;
				}

				if(!bErrorSetting)
				{
					strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("LineSelector"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				switch(eLineSelector)
				{
				case EDeviceBaumerLineSelector_Line0:
					{
						EDeviceBaumerSwitch eSwitch = EDeviceBaumerSwitch_Count;

						if(GetLine0Inverter(&eSwitch))
						{
							strMessage.Format(_T("Couldn't read 'Line0Inverter' from the database."));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}

						if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_LINEINVERTER))
						{
							if(!m_pDevice->GetRemoteNode(SFNC_LINEINVERTER)->GetLocked())
							{
								try
								{
									m_pDevice->GetRemoteNode(SFNC_LINEINVERTER)->SetBool(eSwitch);
								}
								catch(BGAPI2::Exceptions::IException&) { }
							}
						}

						bErrorSetting = false;
					}
					break;
				case EDeviceBaumerLineSelector_Line1:
					{
						EDeviceBaumerSwitch eSwitch = EDeviceBaumerSwitch_Count;

						if(GetLine1Inverter(&eSwitch))
						{
							strMessage.Format(_T("Couldn't read 'Line1Inverter' from the database."));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}

						if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_LINEINVERTER))
						{
							if(!m_pDevice->GetRemoteNode(SFNC_LINEINVERTER)->GetLocked())
							{
								try
								{
									m_pDevice->GetRemoteNode(SFNC_LINEINVERTER)->SetBool(eSwitch);
								}
								catch(BGAPI2::Exceptions::IException&) { }
							}
						}

						EDeviceBaumerLineMode eLineMode = EDeviceBaumerLineMode_Count;

						if(GetLine1Mode(&eLineMode))
						{
							strMessage.Format(_T("Couldn't read 'Line1 Mode' from the database."));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}

						if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_LINEMODE))
						{
							if(!m_pDevice->GetRemoteNode(SFNC_LINEMODE)->GetLocked())
							{
								try
								{
									m_pDevice->GetRemoteNode(SFNC_LINEMODE)->SetString(CStringA(g_lpszLineMode[eLineMode]).GetBuffer());
								}
								catch(BGAPI2::Exceptions::IException&) { }
							}
						}

						if(eLineMode == EDeviceBaumerLineMode_Output)
						{
							EDeviceBaumerLineSource eLineSource = EDeviceBaumerLineSource_Count;

							if(GetLine1Source(&eLineSource))
							{
								strMessage.Format(_T("Couldn't read 'Line1 Source' from the database."));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_LINESOURCE))
							{
								if(!m_pDevice->GetRemoteNode(SFNC_LINESOURCE)->GetLocked())
								{
									try
									{
										m_pDevice->GetRemoteNode(SFNC_LINESOURCE)->SetString(CStringA(g_lpszLineSource[eLineSource]).GetBuffer());
									}
									catch(BGAPI2::Exceptions::IException&) { }
								}
							}
						}

						bErrorSetting = false;
					}
					break;
				case EDeviceBaumerLineSelector_Line2:
					{
						EDeviceBaumerSwitch eSwitch = EDeviceBaumerSwitch_Count;

						if(GetLine2Inverter(&eSwitch))
						{
							strMessage.Format(_T("Couldn't read 'Line2Inverter' from the database."));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}

						if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_LINEINVERTER))
						{
							if(!m_pDevice->GetRemoteNode(SFNC_LINEINVERTER)->GetLocked())
							{
								try
								{
									m_pDevice->GetRemoteNode(SFNC_LINEINVERTER)->SetBool(eSwitch);
								}
								catch(BGAPI2::Exceptions::IException&) { }
							}
						}

						EDeviceBaumerLineMode eLineMode = EDeviceBaumerLineMode_Count;

						if(GetLine2Mode(&eLineMode))
						{
							strMessage.Format(_T("Couldn't read 'Line2 Mode' from the database."));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}

						if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_LINEMODE))
						{
							if(!m_pDevice->GetRemoteNode(SFNC_LINEMODE)->GetLocked())
							{
								try
								{
									m_pDevice->GetRemoteNode(SFNC_LINEMODE)->SetString(CStringA(g_lpszLineMode[eLineMode]).GetBuffer());
								}
								catch(BGAPI2::Exceptions::IException&) { }
							}
						}

						if(eLineMode == EDeviceBaumerLineMode_Output)
						{
							EDeviceBaumerLineSource eLineSource = EDeviceBaumerLineSource_Count;

							if(GetLine2Source(&eLineSource))
							{
								strMessage.Format(_T("Couldn't read 'Line2 Source' from the database."));
								eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
								break;
							}

							if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_LINESOURCE))
							{
								if(!m_pDevice->GetRemoteNode(SFNC_LINESOURCE)->GetLocked())
								{
									try
									{
										m_pDevice->GetRemoteNode(SFNC_LINESOURCE)->SetString(CStringA(g_lpszLineSource[eLineSource]).GetBuffer());
									}
									catch(BGAPI2::Exceptions::IException&) { }
								}
							}
						}

						bErrorSetting = false;
					}
					break;
				case EDeviceBaumerLineSelector_Line3:
					{
						EDeviceBaumerSwitch eSwitch = EDeviceBaumerSwitch_Count;

						if(GetLine3Inverter(&eSwitch))
						{
							strMessage.Format(_T("Couldn't read 'Line3Inverter' from the database."));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}

						if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_LINEINVERTER))
						{
							if(!m_pDevice->GetRemoteNode(SFNC_LINEINVERTER)->GetLocked())
							{
								try
								{
									m_pDevice->GetRemoteNode(SFNC_LINEINVERTER)->SetBool(eSwitch);
								}
								catch(BGAPI2::Exceptions::IException&) { }
							}
						}

						EDeviceBaumerLineSource eLineSource = EDeviceBaumerLineSource_Count;

						if(GetLine3Source(&eLineSource))
						{
							strMessage.Format(_T("Couldn't read 'Line3 Source' from the database."));
							eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
							break;
						}

						if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_LINESOURCE))
						{
							if(!m_pDevice->GetRemoteNode(SFNC_LINESOURCE)->GetLocked())
							{
								try
								{
									m_pDevice->GetRemoteNode(SFNC_LINESOURCE)->SetString(CStringA(g_lpszLineSource[eLineSource]).GetBuffer());
								}
								catch(BGAPI2::Exceptions::IException&) { }
							}
						}
					}
					break;
				}
			}
			try
			{
				m_pDataStreamList = m_pDevice->GetDataStreams();

				m_pDataStreamList->Refresh();
			}
			catch(BGAPI2::Exceptions::IException&)
			{
				bError = true;
			}

			if(bError)
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("data stream list"));
				eReturn = EDeviceInitializeResult_ReadOnDeviceError;
				break;
			}


			try
			{
				size_t st = m_pDataStreamList->size();
				m_pDataStream = m_pDataStreamList->begin()->second;
				m_pDataStream->Open();
			}
			catch(BGAPI2::Exceptions::IException&)
			{
				bError = true;
			}

			if(bError)
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("data stream"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}

			int nGrabBufferCount = 0;

			if(GetGrabBufferCount(&nGrabBufferCount))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Grab Buffer Count"));	
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			try
			{
				m_pBufferList = m_pDataStream->GetBufferList();

				size_t st = m_pBufferList->size();

				for(int i = 0; i < nGrabBufferCount; i++)
				{
					m_pBuffer = new BGAPI2::Buffer();
					m_pBufferList->Add(m_pBuffer);

					m_pBuffer->QueueBuffer();
				}
			}
			catch(BGAPI2::Exceptions::IException&)
			{
				bError = true;
			}

			if(bError)
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("image Buffer"));
				eReturn = EDeviceInitializeResult_ReadOnDeviceError;
				break;
			}

			//////////////////////////////////////////////////////////////////////////
			// Camera setting End
			//////////////////////////////////////////////////////////////////////////


			EDeviceBaumerPixelFormat ePixelFormat = EDeviceBaumerPixelFormat_Count;

			if(GetPixelFormat(&ePixelFormat))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("pixelformat"));
				break;
			}

			bool bColor = false;

			int nBpp = 8;
			int nChannel = 1;
			int nWidthStep = nWidth;
			int nMaxValue = 255;

			switch(ePixelFormat)
			{
			case EDeviceBaumerPixelFormat_Mono8:
				nBpp = 8;
				nWidthStep = nWidth;
				nMaxValue = 255;
				break;
			case EDeviceBaumerPixelFormat_Mono10:
				nBpp = 10;
				nWidthStep = nWidth * 2;

				if(nWidthStep % 4)
					nWidthStep = nWidthStep + (4 - (nWidthStep % 4));

				nMaxValue = (1 << 10) - 1;
				break;
			case EDeviceBaumerPixelFormat_Mono12:
				nBpp = 12;
				nWidthStep = nWidth * 2;

				if(nWidthStep % 4)
					nWidthStep = nWidthStep + (4 - (nWidthStep % 4));

				nMaxValue = (1 << 12) - 1;
				break;
			case EDeviceBaumerPixelFormat_Mono14:
				nBpp = 14;
				nWidthStep = nWidth * 2;

				if(nWidthStep % 4)
					nWidthStep = nWidthStep + (4 - (nWidthStep % 4));

				nMaxValue = (1 << 14) - 1;
				break;
			case EDeviceBaumerPixelFormat_Mono16:
				nBpp = 16;
				nWidthStep = nWidth * 2;

				if(nWidthStep % 4)
					nWidthStep = nWidthStep + (4 - (nWidthStep % 4));

				nMaxValue = (1 << 16) - 1;
				break;
			case EDeviceBaumerPixelFormat_BayerBG8:
			case EDeviceBaumerPixelFormat_BayerBG10:
			case EDeviceBaumerPixelFormat_BayerBG12:
			case EDeviceBaumerPixelFormat_BayerBG16:
			case EDeviceBaumerPixelFormat_BayerGB8:
			case EDeviceBaumerPixelFormat_BayerGB10:
			case EDeviceBaumerPixelFormat_BayerGB12:
			case EDeviceBaumerPixelFormat_BayerGB16:
			case EDeviceBaumerPixelFormat_BayerGR8:
			case EDeviceBaumerPixelFormat_BayerGR10:
			case EDeviceBaumerPixelFormat_BayerGR12:
			case EDeviceBaumerPixelFormat_BayerGR16:
			case EDeviceBaumerPixelFormat_BayerRG8:
			case EDeviceBaumerPixelFormat_BayerRG10:
			case EDeviceBaumerPixelFormat_BayerRG12:
			case EDeviceBaumerPixelFormat_BayerRG16:
			case EDeviceBaumerPixelFormat_RGB8:
			case EDeviceBaumerPixelFormat_RGB10:
			case EDeviceBaumerPixelFormat_RGB12:
			case EDeviceBaumerPixelFormat_RGB16:
			case EDeviceBaumerPixelFormat_BGR8:
			case EDeviceBaumerPixelFormat_BGR10:
			case EDeviceBaumerPixelFormat_BGR12:
			case EDeviceBaumerPixelFormat_BGR16:
				bColor = true;
				nBpp = 8;
				nChannel = 3;
				nWidthStep = nWidth * nChannel;
				nMaxValue = 255;
				break;
			default:
				break;
			}
			
			CMultipleVariable mv;

			if(bColor)
			{
				for(int i = 0; i < 3; ++i)
					mv.AddValue(nMaxValue);
			}
			else
				mv.AddValue(nMaxValue);

			this->InitBuffer(nWidth, nHeight, mv, CRavidImage::MakeValueFormat(nChannel, nBpp), nWidthStep, 4);
			this->ConnectImage();

			try
			{
				m_pDataStream->RegisterNewBufferEvent(BGAPI2::Events::EVENTMODE_EVENT_HANDLER);

				m_pDataStream->RegisterNewBufferEventHandler(this, (BGAPI2::Events::NewBufferEventHandler) &CallbackFunction);
			}
			catch(BGAPI2::Exceptions::IException&)
			{
				bError = true;
			}

			if(bError)
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("BufferEventHandler"));
				eReturn = EDeviceInitializeResult_NotSupportedDeviceError;
				break;
			}

			int nGrabCount = 0;

			if(GetGrabCount(&nGrabCount))
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Grab count"));
				eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
				break;
			}

			if(nGrabCount <= 0)
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Grab count"));
				eReturn = EDeviceInitializeResult_MatchedInfoError;
				break;
			}

			m_nGrabCount = nGrabCount;

			try
			{
				m_pDataStream->StartAcquisitionContinuous();
			}
			catch(BGAPI2::Exceptions::IException&)
			{
				bError = true;
			}

			if(bError)
			{
				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Acquisition mode"));
				eReturn = EDeviceInitializeResult_MatchedInfoError;
				break;
			}
		}
		catch(BGAPI2::Exceptions::IException&)
		{
			bError = true;
		}

		if(bError)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Setting Param"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
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

EDeviceTerminateResult CDeviceBaumerGigE::Terminate()
{
	CString strMessage;

	SetStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("Baumer"));
		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);
		SetMessage(strMessage);

		return EDeviceTerminateResult_NotFoundApiError;
	}

	EDeviceTerminateResult eReturn = EDeviceTerminateResult_UnknownError;

	do
	{
		if(IsLive() || !IsGrabAvailable())
			Stop();

		m_bIsInitialized = false;
		m_bIsGrabAvailable = true;

		bool bError = false;

		try
		{
			if(m_pDataStream)
				m_pDataStream->StopAcquisition();

			if(m_pBufferList)
			{
				m_pBufferList->DiscardAllBuffers();

				while(m_pBufferList->size() > 0)
				{
					m_pBuffer = m_pBufferList->begin()->second;

					if(m_pBuffer)
					{
						m_pBufferList->RevokeBuffer(m_pBuffer);
						delete m_pBuffer;
					}
					m_pBuffer = nullptr;
				}

				m_pBufferList = nullptr;
			}
		}
		catch(BGAPI2::Exceptions::IException&)
		{
		}

		if(m_pDataStream)
		{
			try
			{
				if(m_pDataStream->IsOpen())
					m_pDataStream->Close();
			}
			catch(BGAPI2::Exceptions::IException&) { }

			m_pDataStream = nullptr;
		}

		if(m_pDevice)
		{
			try
			{
				if(m_pDevice->IsOpen())
					m_pDevice->Close();
			}
			catch(BGAPI2::Exceptions::IException&) { }

			m_pDevice = nullptr;
		}

		if(m_pInterface)
		{
			bool bNeedTerminate = false;

			try
			{
				BGAPI2::DeviceList* pDeviceList = m_pInterface->GetDevices();
				
				if(m_pInterface->IsOpen())
				{
					bNeedTerminate = true;

					for(BGAPI2::DeviceList::iterator devIter = pDeviceList->begin(); devIter != pDeviceList->end(); devIter++)
					{
						BGAPI2::Device* pDevice = devIter->second;
						if(!pDevice)
							continue;

						if(!pDevice->IsOpen())
							continue;

						bNeedTerminate = false;

						break;
					}
				}
			}
			catch(BGAPI2::Exceptions::IException&) { }

			if(bNeedTerminate)
				m_pInterface->Close();

			m_pInterface = nullptr;
		}

		if(m_pSystem)
		{
			bool bNeedTerminate = false;

			try
			{
				BGAPI2::InterfaceList* pInterfaceList = m_pSystem->GetInterfaces();

				if(m_pSystem->IsOpen())
				{
					bNeedTerminate = true;

					for(BGAPI2::InterfaceList::iterator intIter = pInterfaceList->begin(); intIter != pInterfaceList->end(); intIter++)
					{
						BGAPI2::Interface* pInterface = intIter->second;
						if(!pInterface)
							continue;

						if(!pInterface->IsOpen())
							continue;

						bNeedTerminate = false;

						break;
					}
				}
			}
			catch(BGAPI2::Exceptions::IException&) { }

			if(bNeedTerminate)
			{
				try
				{
					m_pSystem->Close();
					BGAPI2::SystemList::ReleaseInstance();

					m_pImgProc->ReleaseInstance();
					m_pImgProc = nullptr;
				}
				catch(const BGAPI2::Exceptions::IException&)
				{
					m_pImgProc->ReleaseInstance();
					m_pImgProc = nullptr;
				}
			}

			m_pSystem = nullptr;
		}
		
		strMessage = CMultiLanguageManager::GetString(ELanguageParameter_TheDevicehasbeensuccessfullyterminated);

		eReturn = EDeviceTerminateResult_OK;

		CEventHandlerManager::BroadcastOnDeviceTerminated(this);
	}
	while(false);

	this->ClearBuffer();

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceBaumerGigE::LoadSettings()
{
	bool bReturn = false;

	m_vctParameterFieldConfigurations.clear();

	do
	{
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_DeviceID, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_DeviceID], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 0);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_CameraSetting, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_CameraSetting], _T("1"), EParameterFieldType_None);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_GrabCount, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_GrabCount], _T("1"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_CanvasWidth, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_CanvasWidth], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_CanvasHeight, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_CanvasHeight], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_OffsetX, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_OffsetX], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_OffsetY, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_OffsetY], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_GrabWaitingTime, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_GrabWaitingTime], IntegerToString(INT_MAX), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_GrabBufferCount, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_GrabBufferCount], _T("4"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_PixelFormat, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_PixelFormat], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszPixelFormat, EDeviceBaumerPixelFormat_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_FlipHorizontal, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_FlipHorizontal], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_FlipVertical, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_FlipVertical], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_TriggerSetting, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_TriggerSetting], _T("1"), EParameterFieldType_None);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_TriggerSelector, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_TriggerSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszTriggerSelect, EDeviceBaumerTriggerSelector_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_FrameStart, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_FrameStart], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_FrameStartTriggerMode, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_FrameStartTriggerMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszSwitch, EDeviceBaumerSwitch_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_FrameStartTriggerSource, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_FrameStartTriggerSource], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszSource, EDeviceBaumerTriggerSource_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_FrameStartTriggerActivation, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_FrameStartTriggerActivation], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszActivation, EDeviceBaumerTriggerActivation_Count), nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_LineStart, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_LineStart], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_LineStartTriggerMode, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_LineStartTriggerMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszSwitch, EDeviceBaumerSwitch_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_LineStartTriggerSource, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_LineStartTriggerSource], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszSource, EDeviceBaumerTriggerSource_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_LineStartTriggerActivation, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_LineStartTriggerActivation], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszActivation, EDeviceBaumerTriggerActivation_Count), nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_AcquisitionStart, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_AcquisitionStart], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_AcquisitionStartTriggerMode, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_AcquisitionStartTriggerMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszSwitch, EDeviceBaumerSwitch_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_AcquisitionStartTriggerSource, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_AcquisitionStartTriggerSource], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszSource, EDeviceBaumerTriggerSource_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_AcquisitionStartTriggerActivation, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_AcquisitionStartTriggerActivation], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszActivation, EDeviceBaumerTriggerActivation_Count), nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_ExposureAndGainSetting, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_ExposureAndGainSetting], _T("1"), EParameterFieldType_None);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_ExposureAuto, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_ExposureAuto], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAuto, EDeviceBaumerAuto_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_ExposureMode, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_ExposureMode], _T("1"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszExposureMode, EDeviceBaumerExposureMode_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_Exposure, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_Exposure], _T("50000"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_GainAuto, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_GainAuto], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszAuto, EDeviceBaumerAuto_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_GainSelector, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_GainSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGainSelect, EDeviceBaumerGainSelector_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_Gain, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_Gain], _T("4"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_AnalogGain, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_AnalogGain], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_DigitalGain, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_DigitalGain], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_Tap1Gain, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_Tap1Gain], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_Tap2Gain, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_Tap2Gain], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		

		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_DigitalIOControl, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_DigitalIOControl], _T("1"), EParameterFieldType_None);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_LineSelector, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_LineSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszLineSelector, EDeviceBaumerLineSelector_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_Line0, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_Line0], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_Line0Inverter, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_Line0Inverter], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszSwitch, EDeviceBaumerSwitch_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_Line0Mode, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_Line0Mode], _T("Intput"), EParameterFieldType_Static, nullptr, nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_Line1, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_Line1], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_Line1Inverter, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_Line1Inverter], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszSwitch, EDeviceBaumerSwitch_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_Line1Mode, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_Line1Mode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszLineMode, EDeviceBaumerLineMode_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_Line1Source, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_Line1Source], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszLineSource, EDeviceBaumerLineSource_Count), nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_Line2, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_Line2], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_Line2Inverter, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_Line2Inverter], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszSwitch, EDeviceBaumerSwitch_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_Line2Mode, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_Line2Mode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszLineMode, EDeviceBaumerLineMode_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_Line2Source, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_Line2Source], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszLineSource, EDeviceBaumerLineSource_Count), nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_Line3, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_Line3], _T("0"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_Line3Inverter, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_Line3Inverter], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszSwitch, EDeviceBaumerSwitch_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_Line3Mode, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_Line3Mode],  _T("Output"), EParameterFieldType_Static, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_Line3Source, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_Line3Source], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszLineSource, EDeviceBaumerLineSource_Count), nullptr, 2);


		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_UserOutputSelector, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_UserOutputSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszUserOutputSelector, EDeviceBaumerUserOutputSelector_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_UserOutputValue, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_UserOutputValue], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszSwitch, EDeviceBaumerSwitch_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_UserOutputValueAll, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_UserOutputValueAll], _T("0"), EParameterFieldType_Edit, nullptr, _T("Pure number"), 1);



		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_TimerControl, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_TimerControl], _T("1"), EParameterFieldType_None);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_TimerSelector, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_TimerSelector], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszTimerSelector, EDeviceBaumerTimerSelector_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_TimerTriggerSource, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_TimerTriggerSource], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszTimerTriggerSource, EDeviceBaumerTimerTriggerSource_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_TimerTriggerActivation, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_TimerTriggerActivation], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszActivation, EDeviceBaumerTriggerActivation_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_TimerDuration, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_TimerDuration], _T("0.000000"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterBaumerGigE_TimerDelay, g_lpszParamBaumerGigE[EDeviceParameterBaumerGigE_TimerDelay], _T("0.000000"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		
		bReturn = true;
	}
	while(false);

	return bReturn & __super::LoadSettings();
}

EDeviceGrabResult CDeviceBaumerGigE::Grab()
{
	EDeviceGrabResult eReturn = EDeviceGrabResult_UnknownError;

	CString strMessage;

	do
	{
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

		bool bError = false;

		int nGrabCount = 0;

		if(GetGrabCount(&nGrabCount))
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedatabase), _T("Grab count"));
			eReturn = EDeviceGrabResult_ReadOnDatabaseError;
			break;
		}

		if(nGrabCount <= 0)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("Grab count"));
			eReturn = EDeviceGrabResult_ReadOnDeviceError;
			break;
		}

		m_nGrabCount = nGrabCount;

		try
		{
			m_pDevice->GetRemoteNode("AcquisitionStart")->Execute();
		}
		catch(BGAPI2::Exceptions::IException&)
		{
			bError = true;
		}

		if(bError)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("AcquisitionStart"));
			eReturn = EDeviceGrabResult_WriteToDeviceError;
			break;
		}

		m_bIsGrabAvailable = false;

		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("Grab"));

		eReturn = EDeviceGrabResult_OK;
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceLiveResult CDeviceBaumerGigE::Live()
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

		bool bError = false;

		m_nGrabCount = INT_MAX;

		try
		{
			m_pDevice->GetRemoteNode("AcquisitionStart")->Execute();
			m_bIsLive = true;
			m_bIsGrabAvailable = false;
		}
		catch(BGAPI2::Exceptions::IException&)
		{
			bError = true;
		}

		if(bError)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("AcquisitionStart"));
			eReturn = EDeviceLiveResult_WriteToDeviceError;
			break;
		}

		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("Live"));

		eReturn = EDeviceLiveResult_OK;
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceStopResult CDeviceBaumerGigE::Stop()
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
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Ithasbeenlivingorgrabbing);
			eReturn = EDeviceStopResult_AlreadyStopError;
			break;
		}

		bool bError = false;

		try
		{
			if(m_pDevice->GetRemoteNodeList()->GetNodePresent("AcquisitionAbort"))
			{
				m_pDevice->GetRemoteNode("AcquisitionAbort")->Execute();
			}

			m_pDevice->GetRemoteNode("AcquisitionStop")->Execute();

		}
		catch(BGAPI2::Exceptions::IException&)
		{
			bError = true;
		}

		if(bError)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("AcquisitionStop"));
			eReturn = EDeviceStopResult_WriteToDeviceError;
			break;
		}

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

EDeviceTriggerResult CDeviceBaumerGigE::Trigger()
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

		bool bError = false;

		try
		{
			m_pDevice->GetRemoteNode("TriggerSoftware")->Execute();
		}
		catch(BGAPI2::Exceptions::IException&)
		{
			bError = true;
		}

		if(bError)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtowrite_s_tothedevice), _T("TriggerSoftware"));
			eReturn = EDeviceTriggerResult_WriteToDeviceError;
			break;
		}

		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Succeededtoexecutecommand_s), _T("Trigger"));

		eReturn = EDeviceTriggerResult_OK;
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetPixelFormat(EDeviceBaumerPixelFormat* pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_PixelFormat));

		if(nData < 0 || nData >= (int)EDeviceBaumerPixelFormat_Count)
		{
			eReturn = EBaumerGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaumerPixelFormat)nData;

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetPixelFormat(EDeviceBaumerPixelFormat eParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_PixelFormat;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < (EDeviceBaumerPixelFormat)0 || eParam >= EDeviceBaumerPixelFormat_Count)
		{
			eReturn = EBaumerSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EBaumerSetFunction_ActivatedDeviceError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], g_lpszPixelFormat[nPreValue], g_lpszPixelFormat[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetTriggerSelector(EDeviceBaumerTriggerSelector* pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_TriggerSelector));

		if(nData < 0 || nData >= (int)EDeviceBaumerTriggerSelector_Count)
		{
			eReturn = EBaumerGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaumerTriggerSelector)nData;

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetTriggerSelector(EDeviceBaumerTriggerSelector eParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;
	
	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_TriggerSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < (EDeviceBaumerTriggerSelector)0 || eParam >= EDeviceBaumerTriggerSelector_Count)
		{
			eReturn = EBaumerSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			bool bError = false;

			if(!m_pDevice)
			{
				eReturn = EBaumerSetFunction_DeviceNullptrError;
				break;
			}

			try
			{
				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_TRIGGERSELECTOR))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(m_pDevice->GetRemoteNode(SFNC_TRIGGERSELECTOR)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_TRIGGERSELECTOR)->SetString(CStringA(g_lpszTriggerSelect[eParam]).GetBuffer());
			}
			catch(BGAPI2::Exceptions::IException&)
			{
				bError = true;
			}

			if(bError)
			{
				eReturn = EBaumerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], g_lpszTriggerSelect[nPreValue], g_lpszTriggerSelect[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetFrameStartMode(EDeviceBaumerSwitch* pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_FrameStartTriggerMode));

		if(nData < 0 || nData >= (int)EDeviceBaumerSwitch_Count)
		{
			eReturn = EBaumerGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaumerSwitch)nData;

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetFrameStartMode(EDeviceBaumerSwitch eParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_FrameStartTriggerMode;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < (EDeviceBaumerSwitch)0 || eParam >= EDeviceBaumerSwitch_Count)
		{
			eReturn = EBaumerSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			bool bError = false;

			if(!m_pDevice)
			{
				eReturn = EBaumerSetFunction_DeviceNullptrError;
				break;
			}

			try
			{
				if(m_pDevice->GetRemoteNode(SFNC_TRIGGERSELECTOR)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_TRIGGERSELECTOR)->SetString(SFNC_TRIGGERSELECTORVALUE_FRAMESTART);

				if(m_pDevice->GetRemoteNode(SFNC_TRIGGERMODE)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_TRIGGERMODE)->SetString(CStringA(g_lpszSwitch[eParam]).GetBuffer());
			}
			catch(BGAPI2::Exceptions::IException&)
			{
				bError = true;
			}

			if(bError)
			{
				eReturn = EBaumerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], g_lpszSwitch[nPreValue], g_lpszSwitch[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetLineStartMode(EDeviceBaumerSwitch* pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_LineStartTriggerMode));

		if(nData < 0 || nData >= (int)EDeviceBaumerSwitch_Count)
		{
			eReturn = EBaumerGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaumerSwitch)nData;

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetLineStartMode(EDeviceBaumerSwitch eParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_LineStartTriggerMode;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < (EDeviceBaumerSwitch)0 || eParam >= EDeviceBaumerSwitch_Count)
		{
			eReturn = EBaumerSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			bool bError = false;

			if(!m_pDevice)
			{
				eReturn = EBaumerSetFunction_DeviceNullptrError;
				break;
			}

			try
			{
				if(m_pDevice->GetRemoteNode(SFNC_TRIGGERSELECTOR)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_TRIGGERSELECTOR)->SetString(SFNC_TRIGGERSELECTORVALUE_LINESTART);

				if(m_pDevice->GetRemoteNode(SFNC_TRIGGERMODE)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_TRIGGERMODE)->SetString(CStringA(g_lpszSwitch[eParam]).GetBuffer());
			}
			catch(BGAPI2::Exceptions::IException&)
			{
				bError = true;
			}

			if(bError)
			{
				eReturn = EBaumerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], g_lpszSwitch[nPreValue], g_lpszSwitch[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetAcquisitionStartMode(EDeviceBaumerSwitch* pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_AcquisitionStartTriggerMode));

		if(nData < 0 || nData >= (int)EDeviceBaumerSwitch_Count)
		{
			eReturn = EBaumerGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaumerSwitch)nData;

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetAcquisitionStartMode(EDeviceBaumerSwitch eParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_AcquisitionStartTriggerMode;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < (EDeviceBaumerSwitch)0 || eParam >= EDeviceBaumerSwitch_Count)
		{
			eReturn = EBaumerSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			bool bError = false;

			if(!m_pDevice)
			{
				eReturn = EBaumerSetFunction_DeviceNullptrError;
				break;
			}
			try
			{
				if(m_pDevice->GetRemoteNode(SFNC_TRIGGERSELECTOR)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_TRIGGERSELECTOR)->SetString(SFNC_TRIGGERSELECTORVALUE_ACQUISITIONSTART);

				if(m_pDevice->GetRemoteNode(SFNC_TRIGGERMODE)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_TRIGGERMODE)->SetString(CStringA(g_lpszSwitch[eParam]).GetBuffer());
			}
			catch(BGAPI2::Exceptions::IException&)
			{
				bError = true;
			}

			if(bError)
			{
				eReturn = EBaumerSetFunction_WriteToDeviceError;
				break;
			}			
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], g_lpszSwitch[nPreValue], g_lpszSwitch[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetFrameStartTriggerSource(EDeviceBaumerTriggerSource* pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_FrameStartTriggerSource));

		if(nData < 0 || nData >= (int)EDeviceBaumerTriggerSource_Count)
		{
			eReturn = EBaumerGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaumerTriggerSource)nData;

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetFrameStartTriggerSource(EDeviceBaumerTriggerSource eParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_FrameStartTriggerSource;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < (EDeviceBaumerTriggerSource)0 || eParam >= EDeviceBaumerTriggerSource_Count)
		{
			eReturn = EBaumerSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			bool bError = false;

			if(!m_pDevice)
			{
				eReturn = EBaumerSetFunction_DeviceNullptrError;
				break;
			}

			try
			{
				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_TRIGGERSELECTOR))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(m_pDevice->GetRemoteNode(SFNC_TRIGGERSELECTOR)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_TRIGGERSELECTOR)->SetString(SFNC_TRIGGERSELECTORVALUE_FRAMESTART);

				if(m_pDevice->GetRemoteNode(SFNC_TRIGGERSOURCE)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_TRIGGERSOURCE)->SetString(CStringA(g_lpszSource[eParam]).GetBuffer());
			}
			catch(BGAPI2::Exceptions::IException&)
			{
				bError = true;
			}

			if(bError)
			{
				eReturn = EBaumerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], g_lpszSource[nPreValue], g_lpszSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetLineStartTriggerSource(EDeviceBaumerTriggerSource* pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_LineStartTriggerSource));

		if(nData < 0 || nData >= (int)EDeviceBaumerTriggerSource_Count)
		{
			eReturn = EBaumerGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaumerTriggerSource)nData;

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetLineStartTriggerSource(EDeviceBaumerTriggerSource eParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_LineStartTriggerSource;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < (EDeviceBaumerTriggerSource)0 || eParam >= EDeviceBaumerTriggerSource_Count)
		{
			eReturn = EBaumerSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			bool bError = false;

			if(!m_pDevice)
			{
				eReturn = EBaumerSetFunction_DeviceNullptrError;
				break;
			}

			try
			{
				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_TRIGGERSELECTOR))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(m_pDevice->GetRemoteNode(SFNC_TRIGGERSELECTOR)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_TRIGGERSELECTOR)->SetString(SFNC_TRIGGERSELECTORVALUE_LINESTART);

				if(m_pDevice->GetRemoteNode(SFNC_TRIGGERSOURCE)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_TRIGGERSOURCE)->SetString(CStringA(g_lpszSource[eParam]).GetBuffer());
			}
			catch(BGAPI2::Exceptions::IException&)
			{
				bError = true;
			}

			if(bError)
			{
				eReturn = EBaumerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], g_lpszSource[nPreValue], g_lpszSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetAcquisitionStartTriggerSource(EDeviceBaumerTriggerSource* pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_AcquisitionStartTriggerSource));

		if(nData < 0 || nData >= (int)EDeviceBaumerTriggerSource_Count)
		{
			eReturn = EBaumerGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaumerTriggerSource)nData;

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetAcquisitionStartTriggerSource(EDeviceBaumerTriggerSource eParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_AcquisitionStartTriggerSource;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < (EDeviceBaumerTriggerSource)0 || eParam >= EDeviceBaumerTriggerSource_Count)
		{
			eReturn = EBaumerSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			bool bError = false;

			if(!m_pDevice)
			{
				eReturn = EBaumerSetFunction_DeviceNullptrError;
				break;
			}

			try
			{
				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_TRIGGERSELECTOR))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(m_pDevice->GetRemoteNode(SFNC_TRIGGERSELECTOR)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_TRIGGERSELECTOR)->SetString(SFNC_TRIGGERSELECTORVALUE_ACQUISITIONSTART);

				if(m_pDevice->GetRemoteNode(SFNC_TRIGGERSOURCE)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_TRIGGERSOURCE)->SetString(CStringA(g_lpszSource[eParam]).GetBuffer());
			}
			catch(BGAPI2::Exceptions::IException&)
			{
				bError = true;
			}

			if(bError)
			{
				eReturn = EBaumerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], g_lpszSource[nPreValue], g_lpszSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetFrameStartTriggerActivation(EDeviceBaumerTriggerActivation* pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_FrameStartTriggerActivation));

		if(nData < 0 || nData >= (int)EDeviceBaumerTriggerActivation_Count)
		{
			eReturn = EBaumerGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaumerTriggerActivation)nData;

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetFrameStartTriggerActivation(EDeviceBaumerTriggerActivation eParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_FrameStartTriggerActivation;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));


	do
	{
		if(eParam < (EDeviceBaumerTriggerActivation)0 || eParam >= EDeviceBaumerTriggerActivation_Count)
		{
			eReturn = EBaumerSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			bool bError = false;

			if(!m_pDevice)
			{
				eReturn = EBaumerSetFunction_DeviceNullptrError;
				break;
			}

			try
			{
				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_TRIGGERSELECTOR))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(m_pDevice->GetRemoteNode(SFNC_TRIGGERSELECTOR)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_TRIGGERSELECTOR)->SetString(SFNC_TRIGGERSELECTORVALUE_FRAMESTART);

				if(m_pDevice->GetRemoteNode(SFNC_TRIGGERACTIVATION)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_TRIGGERACTIVATION)->SetString(CStringA(g_lpszActivation[eParam]).GetBuffer());
			}
			catch(BGAPI2::Exceptions::IException&)
			{
				bError = true;
			}

			if(bError)
			{
				eReturn = EBaumerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], g_lpszActivation[nPreValue], g_lpszActivation[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetLineStartTriggerActivation(EDeviceBaumerTriggerActivation* pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_LineStartTriggerActivation));

		if(nData < 0 || nData >= (int)EDeviceBaumerTriggerActivation_Count)
		{
			eReturn = EBaumerGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaumerTriggerActivation)nData;

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetLineStartTriggerActivation(EDeviceBaumerTriggerActivation eParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_LineStartTriggerActivation;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < (EDeviceBaumerTriggerActivation)0 || eParam >= EDeviceBaumerTriggerActivation_Count)
		{
			eReturn = EBaumerSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			bool bError = false;

			if(!m_pDevice)
			{
				eReturn = EBaumerSetFunction_DeviceNullptrError;
				break;
			}

			try
			{
				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_TRIGGERSELECTOR))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(m_pDevice->GetRemoteNode(SFNC_TRIGGERSELECTOR)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_TRIGGERSELECTOR)->SetString(SFNC_TRIGGERSELECTORVALUE_LINESTART);

				if(m_pDevice->GetRemoteNode(SFNC_TRIGGERACTIVATION)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_TRIGGERACTIVATION)->SetString(CStringA(g_lpszActivation[eParam]).GetBuffer());
			}
			catch(BGAPI2::Exceptions::IException&)
			{
				bError = true;
			}

			if(bError)
			{
				eReturn = EBaumerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], g_lpszActivation[nPreValue], g_lpszActivation[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetAcquisitionStartTriggerActivation(EDeviceBaumerTriggerActivation* pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_AcquisitionStartTriggerActivation));

		if(nData < 0 || nData >= (int)EDeviceBaumerTriggerActivation_Count)
		{
			eReturn = EBaumerGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaumerTriggerActivation)nData;

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetAcquisitionStartTriggerActivation(EDeviceBaumerTriggerActivation eParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_AcquisitionStartTriggerActivation;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < (EDeviceBaumerTriggerActivation)0 || eParam >= EDeviceBaumerTriggerActivation_Count)
		{
			eReturn = EBaumerSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			bool bError = false;

			if(!m_pDevice)
			{
				eReturn = EBaumerSetFunction_DeviceNullptrError;
				break;
			}

			try
			{
				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_TRIGGERSELECTOR))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(m_pDevice->GetRemoteNode(SFNC_TRIGGERSELECTOR)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_TRIGGERSELECTOR)->SetString(SFNC_TRIGGERSELECTORVALUE_ACQUISITIONSTART);

				if(m_pDevice->GetRemoteNode(SFNC_TRIGGERACTIVATION)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_TRIGGERACTIVATION)->SetString(CStringA(g_lpszActivation[eParam]).GetBuffer());
			}
			catch(BGAPI2::Exceptions::IException&)
			{
				bError = true;
			}

			if(bError)
			{
				eReturn = EBaumerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], g_lpszActivation[nPreValue], g_lpszActivation[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetExposureAuto(EDeviceBaumerAuto* pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_ExposureAuto));

		if(nData < 0 || nData >= (int)EDeviceBaumerAuto_Count)
		{
			eReturn = EBaumerGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaumerAuto)nData;

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetExposureAuto(EDeviceBaumerAuto eParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_ExposureAuto;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < (EDeviceBaumerAuto)0 || eParam >= EDeviceBaumerAuto_Count)
			break;

		if(IsInitialized())
		{
			bool bError = false;

			if(!m_pDevice)
			{
				eReturn = EBaumerSetFunction_DeviceNullptrError;
				break;
			}

			try
			{
				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_EXPOSUREAUTO))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(m_pDevice->GetRemoteNode(SFNC_EXPOSUREAUTO)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_EXPOSUREAUTO)->SetString(CStringA(g_lpszAuto[eParam]).GetBuffer());
			}
			catch(BGAPI2::Exceptions::IException&)
			{
				bError = true;
			}

			if(bError)
			{
				eReturn = EBaumerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], g_lpszAuto[nPreValue], g_lpszAuto[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetExposureMode(EDeviceBaumerExposureMode* pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_ExposureMode));

		if(nData < 0 || nData >= (int)EDeviceBaumerExposureMode_Count)
		{
			eReturn = EBaumerGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaumerExposureMode)nData;

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetExposureMode(EDeviceBaumerExposureMode eParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_ExposureMode;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < (EDeviceBaumerExposureMode)0 || eParam >= EDeviceBaumerExposureMode_Count)
			break;

		if(IsInitialized())
		{
			bool bError = false;

			if(!m_pDevice)
			{
				eReturn = EBaumerSetFunction_DeviceNullptrError;
				break;
			}

			try
			{
				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_EXPOSUREMODE))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(m_pDevice->GetRemoteNode(SFNC_EXPOSUREMODE)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_EXPOSUREMODE)->SetString(CStringA(g_lpszExposureMode[eParam]).GetBuffer());
			}
			catch(BGAPI2::Exceptions::IException&)
			{
				bError = true;
			}

			if(bError)
			{
				eReturn = EBaumerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], g_lpszExposureMode[nPreValue], g_lpszExposureMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetGainAuto(EDeviceBaumerAuto* pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_GainAuto));

		if(nData < 0 || nData >= (int)EDeviceBaumerAuto_Count)
		{
			eReturn = EBaumerGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaumerAuto)nData;

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetGainAuto(EDeviceBaumerAuto eParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_GainAuto;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < (EDeviceBaumerAuto)0 || eParam >= EDeviceBaumerAuto_Count)
			break;

		if(IsInitialized())
		{
			bool bError = false;

			if(!m_pDevice)
			{
				eReturn = EBaumerSetFunction_DeviceNullptrError;
				break;
			}

			try
			{
				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_GAINAUTO))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(m_pDevice->GetRemoteNode(SFNC_GAINAUTO)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_GAINAUTO)->SetString(CStringA(g_lpszAuto[eParam]).GetBuffer());
			}
			catch(BGAPI2::Exceptions::IException&)
			{
				bError = true;
			}

			if(bError)
			{
				eReturn = EBaumerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], g_lpszAuto[nPreValue], g_lpszAuto[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetGainSelector(EDeviceBaumerGainSelector* pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_GainSelector));

		if(nData < 0 || nData >= (int)EDeviceBaumerGainSelector_Count)
		{
			eReturn = EBaumerGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaumerGainSelector)nData;

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetGainSelector(EDeviceBaumerGainSelector eParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_GainSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < (EDeviceBaumerGainSelector)0 || eParam >= EDeviceBaumerGainSelector_Count)
		{
			eReturn = EBaumerSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			bool bError = false;

			if(!m_pDevice)
			{
				eReturn = EBaumerSetFunction_DeviceNullptrError;
				break;
			}

			try
			{
				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_GAINSELECTOR))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(m_pDevice->GetRemoteNode(SFNC_GAINSELECTOR)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				if(!m_pDevice->GetRemoteNode(SFNC_GAINSELECTOR)->GetEnumNodeList()->GetNodePresent(CStringA(g_lpszGainSelect[eParam]).GetBuffer()))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_GAINSELECTOR)->SetValue(CStringA(g_lpszGainSelect[eParam]).GetBuffer());
			}
			catch(const std::exception&)
			{
				bError = true;
			}

			if(bError)
			{
				eReturn = EBaumerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], g_lpszGainSelect[nPreValue], g_lpszGainSelect[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetGrabCount(int* pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_GrabCount));

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetGrabCount(int nParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_GrabCount;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));
	
	do
	{
		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetCanvasWidth(int* pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_CanvasWidth));

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetCanvasWidth(int nParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_CanvasWidth;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));
	
	do
	{
		if(IsInitialized())
		{
			eReturn = EBaumerSetFunction_AlreadyInitializedError;
			break;
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetCanvasHeight(int* pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_CanvasHeight));

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetCanvasHeight(int nParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_CanvasHeight;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));
	
	do
	{
		if(IsInitialized())
		{
			eReturn = EBaumerSetFunction_AlreadyInitializedError;
			break;
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetOffsetX(int* pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_OffsetX));

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetOffsetX(int pParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_OffsetX;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));
	
	do
	{
		if(IsInitialized())
		{
			bool bError = false;

			if(!m_pDevice)
			{
				eReturn = EBaumerSetFunction_DeviceNullptrError;
				break;
			}

			try
			{
				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_OFFSETX))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(m_pDevice->GetRemoteNode(SFNC_OFFSETX)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_OFFSETX)->SetInt(pParam);
			}
			catch(BGAPI2::Exceptions::IException&)
			{
				bError = true;
			}

			if(bError)
			{
				eReturn = EBaumerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), pParam);

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], nPreValue, pParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetOffsetY(int* pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_OffsetY));

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetOffsetY(int pParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_OffsetY;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(IsInitialized())
		{
			bool bError = false;

			if(!m_pDevice)
			{
				eReturn = EBaumerSetFunction_DeviceNullptrError;
				break;
			}

			try
			{
				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_OFFSETY))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(m_pDevice->GetRemoteNode(SFNC_OFFSETY)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_OFFSETY)->SetInt(pParam);

			}
			catch(BGAPI2::Exceptions::IException&)
			{
				bError = true;
			}

			if(bError)
			{
				eReturn = EBaumerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), pParam);

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], nPreValue, pParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetGrabWaitingTime(int * pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_GrabWaitingTime));

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetGrabWaitingTime(int nParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_GrabWaitingTime;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(IsLive() || !IsGrabAvailable())
		{
			eReturn = EBaumerSetFunction_ActivatedDeviceError;
			break;
		}

		m_nWaitTime = nParam;

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetGrabBufferCount(int* pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_GrabBufferCount));

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetGrabBufferCount(int nParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_GrabBufferCount;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(IsInitialized())
		{
			eReturn = EBaumerSetFunction_AlreadyInitializedError;
			break;
		}

		if(nParam < 1)
		{
			eReturn = EBaumerSetFunction_NotSupportError;
			break;
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetExposure(int* pExposure)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pExposure)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		*pExposure = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_Exposure));

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetExposure(int nParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_Exposure;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(IsInitialized())
		{
			bool bError = false;

			int nMin = LONG_MAX;
			int nMax = 0;

			try
			{
				BGAPI2::String strCommand = "";

				if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_EXPOSURETIMERAW))
					strCommand = SFNC_EXPOSURETIMERAW;
				else if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_EXPOSURETIMEABS))
					strCommand = SFNC_EXPOSURETIMEABS;
				else if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_EXPOSURETIME))
					strCommand = SFNC_EXPOSURETIME;

				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(strCommand))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(m_pDevice->GetRemoteNode(strCommand)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				nMin = m_pDevice->GetRemoteNode(strCommand)->GetIntMin();

				nMax = m_pDevice->GetRemoteNode(strCommand)->GetIntMax();

				if(nParam < nMin || nParam > nMax)
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				m_pDevice->GetRemoteNode(strCommand)->SetInt(nParam);
			}
			catch(BGAPI2::Exceptions::IException&)
			{
				bError = true;
			}

			if(bError)
			{
				eReturn = EBaumerSetFunction_ReadOnDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetAllGain(double* pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterBaumerGigE_Gain));

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetAllGain(double dblParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_Gain;

	double dblPreValue = _ttof(GetParamValue(eSaveNum));

	do
	{
		if(IsInitialized())
		{
			bool bError = false;

			double dblMin = LONG_MAX;
			double dblMax = 0;

			try
			{
				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_GAINSELECTOR))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(m_pDevice->GetRemoteNode(SFNC_GAINSELECTOR)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				if(!m_pDevice->GetRemoteNode(SFNC_GAINSELECTOR)->GetEnumNodeList()->GetNodePresent(SFNC_GAINSELECTORVALUE_ALL))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_GAINSELECTOR)->SetValue(SFNC_GAINSELECTORVALUE_ALL);

				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_GAIN))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				dblMin = m_pDevice->GetRemoteNode(SFNC_GAIN)->GetDoubleMin();

				dblMax = m_pDevice->GetRemoteNode(SFNC_GAIN)->GetDoubleMax();

				if(dblParam < dblMin || dblParam > dblMax)
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_GAIN)->SetDouble(dblParam);
			}
			catch(const std::exception&)
			{
				bError = true;
			}

			if(bError)
			{
				eReturn = EBaumerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetAnalogGain(int* pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_AnalogGain));

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetAnalogGain(int nParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_AnalogGain;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(IsInitialized())
		{
			bool bError = false;

			int nMin = LONG_MAX;
			int nMax = 0;

			try
			{
				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_GAINSELECTOR))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(m_pDevice->GetRemoteNode(SFNC_GAINSELECTOR)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				if(!m_pDevice->GetRemoteNode(SFNC_GAINSELECTOR)->GetEnumNodeList()->GetNodePresent(SFNC_GAINSELECTORVALUE_ANALOGALL))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_GAINSELECTOR)->SetValue(SFNC_GAINSELECTORVALUE_ANALOGALL);

				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_GAIN))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				nMin = m_pDevice->GetRemoteNode(SFNC_GAIN)->GetIntMin();
				nMax = m_pDevice->GetRemoteNode(SFNC_GAIN)->GetIntMax();

				if(nParam < nMin || nParam > nMax)
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_GAIN)->SetInt(nParam);
			}
			catch(const std::exception&)
			{
				bError = true;
			}

			if(bError)
			{
				eReturn = EBaumerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetDigitalGain(int* pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_DigitalGain));

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetDigitalGain(int nParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_DigitalGain;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(IsInitialized())
		{
			bool bError = false;

			int nMin = LONG_MAX;
			int nMax = 0;

			try
			{
				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_GAINSELECTOR))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(m_pDevice->GetRemoteNode(SFNC_GAINSELECTOR)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				if(!m_pDevice->GetRemoteNode(SFNC_GAINSELECTOR)->GetEnumNodeList()->GetNodePresent(SFNC_GAINSELECTORVALUE_DIGITALALL))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_GAINSELECTOR)->SetValue(SFNC_GAINSELECTORVALUE_DIGITALALL);

				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_GAIN))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				nMin = m_pDevice->GetRemoteNode(SFNC_GAIN)->GetIntMin();
				nMax = m_pDevice->GetRemoteNode(SFNC_GAIN)->GetIntMax();

				if(nParam < nMin || nParam > nMax)
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_GAIN)->SetInt(nParam);
			}
			catch(const std::exception&)
			{
				bError = true;
			}

			if(bError)
			{
				eReturn = EBaumerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetTap1Gain(int* pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_Tap1Gain));

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetTap1Gain(int nParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_Tap1Gain;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(IsInitialized())
		{
			bool bError = false;

			int nMin = LONG_MAX;
			int nMax = 0;

			try
			{
				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_GAINSELECTOR))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(m_pDevice->GetRemoteNode(SFNC_GAINSELECTOR)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				if(!m_pDevice->GetRemoteNode(SFNC_GAINSELECTOR)->GetEnumNodeList()->GetNodePresent(SFNC_GAINSELECTORVALUE_DIGITALTAP1))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_GAINSELECTOR)->SetValue(SFNC_GAINSELECTORVALUE_DIGITALTAP1);

				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_GAIN))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				nMin = m_pDevice->GetRemoteNode(SFNC_GAIN)->GetIntMin();
				nMax = m_pDevice->GetRemoteNode(SFNC_GAIN)->GetIntMax();

				if(nParam < nMin || nParam > nMax)
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_GAIN)->SetInt(nParam);
			}
			catch(const std::exception&)
			{
				bError = true;
			}

			if(bError)
			{
				eReturn = EBaumerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetTap2Gain(int* pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_Tap2Gain));

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetTap2Gain(int nParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_Tap2Gain;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(IsInitialized())
		{
			bool bError = false;

			int nMin = LONG_MAX;
			int nMax = 0;

			try
			{
				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_GAINSELECTOR))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(m_pDevice->GetRemoteNode(SFNC_GAINSELECTOR)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				if(!m_pDevice->GetRemoteNode(SFNC_GAINSELECTOR)->GetEnumNodeList()->GetNodePresent(SFNC_GAINSELECTORVALUE_DIGITALTAP2))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_GAINSELECTOR)->SetValue(SFNC_GAINSELECTORVALUE_DIGITALTAP2);

				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_GAIN))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				nMin = m_pDevice->GetRemoteNode(SFNC_GAIN)->GetIntMin();
				nMax = m_pDevice->GetRemoteNode(SFNC_GAIN)->GetIntMax();

				if(nParam < nMin || nParam > nMax)
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_GAIN)->SetInt(nParam);
			}
			catch(const std::exception&)
			{
				bError = true;
			}

			if(bError)
			{
				eReturn = EBaumerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetFlipHorizontal(bool * pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_FlipHorizontal));

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetFlipHorizontal(bool bParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_FlipHorizontal;

	bool bPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(IsLive() || !IsGrabAvailable())
		{
			eReturn = EBaumerSetFunction_WriteToDeviceError;
			break;
		}

		if(IsInitialized())
		{
			bool bError = false;

			try
			{
				if(!m_pDevice->GetRemoteNodeList()->GetNode(SFNC_REVERSEX))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(m_pDevice->GetRemoteNode(SFNC_REVERSEX)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_REVERSEX)->SetBool(bParam);
			}
			catch(BGAPI2::Exceptions::IException&)
			{
				bError = true;
			}

			if(bError)
			{
				eReturn = EBaumerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], g_lpszSwitch[bPreValue], g_lpszSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetFlipVertical(bool * pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_FlipVertical));

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetFlipVertical(bool bParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_FlipVertical;

	bool bPreValue = _ttoi(GetParamValue(eSaveNum));
	
	do
	{
		if(IsLive() || !IsGrabAvailable())
		{
			eReturn = EBaumerSetFunction_WriteToDeviceError;
			break;
		}

		if(IsInitialized())
		{
			bool bError = false;

			try
			{
				if(!m_pDevice->GetRemoteNodeList()->GetNode(SFNC_REVERSEY))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(m_pDevice->GetRemoteNode(SFNC_REVERSEY)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_REVERSEY)->SetBool(bParam);
			}
			catch(BGAPI2::Exceptions::IException&)
			{
				bError = true;
			}

			if(bError)
			{
				eReturn = EBaumerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), bParam);

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], g_lpszSwitch[bPreValue], g_lpszSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetLineSelector(EDeviceBaumerLineSelector* pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_LineSelector));

		if(nData < 0 || nData >= (int)EDeviceBaumerLineSelector_Count)
		{
			eReturn = EBaumerGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaumerLineSelector)nData;

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetLineSelector(EDeviceBaumerLineSelector eParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_LineSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < (EDeviceBaumerLineSelector)0 || eParam >= EDeviceBaumerLineSelector_Count)
		{
			eReturn = EBaumerSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			bool bError = false;

			try
			{
				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_LINESELECTOR))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(m_pDevice->GetRemoteNode(SFNC_LINESELECTOR)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_LINESELECTOR)->SetString(CStringA(g_lpszLineSelector[eParam]).GetBuffer());
			}
			catch(BGAPI2::Exceptions::IException&)
			{
				bError = true;
			}

			if(bError)
			{
				eReturn = EBaumerSetFunction_NotSupportError;
				break;
			}			
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], g_lpszLineSelector[nPreValue], g_lpszLineSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetLine0Inverter(EDeviceBaumerSwitch* pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_Line0Inverter));

		if(nData < 0 || nData >= (int)EDeviceBaumerSwitch_Count)
		{
			eReturn = EBaumerGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaumerSwitch)nData;

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetLine0Inverter(EDeviceBaumerSwitch eParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_Line0Inverter;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < (EDeviceBaumerSwitch)0 || eParam >= EDeviceBaumerSwitch_Count)
		{
			eReturn = EBaumerSetFunction_NotSupportError;
			break;
		}

		EDeviceBaumerLineSelector eLineSelector = EDeviceBaumerLineSelector_Count;

		if(GetLineSelector(&eLineSelector))
		{
			eReturn = EBaumerSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eLineSelector != EDeviceBaumerLineSelector_Line0)
		{
			eReturn = EBaumerSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			bool bError = false;

			try
			{
				if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_LINESELECTOR))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				CString strLine(m_pDevice->GetRemoteNode(SFNC_LINESELECTOR)->GetString().get());

				if(strLine.CompareNoCase(g_lpszLineSelector[EDeviceBaumerLineSelector_Line0]))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_LINEINVERTER))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(m_pDevice->GetRemoteNode(SFNC_LINEINVERTER)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_LINEINVERTER)->SetBool(eParam);
			}
			catch(BGAPI2::Exceptions::IException&)
			{
				bError = true;
			}

			if(bError)
			{
				eReturn = EBaumerSetFunction_NotSupportError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], g_lpszSwitch[nPreValue], g_lpszSwitch[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetLine0Mode(EDeviceBaumerLineMode* pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_Line0Mode));

		if(nData < 0 || nData >= (int)EDeviceBaumerLineMode_Count)
		{
			eReturn = EBaumerGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaumerLineMode)nData;

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetLine0Mode(EDeviceBaumerLineMode eParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_Line0Mode;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < (EDeviceBaumerLineMode)0 || eParam >= EDeviceBaumerLineMode_Count)
		{
			eReturn = EBaumerSetFunction_NotSupportError;
			break;
		}

		EDeviceBaumerLineSelector eLineSelector = EDeviceBaumerLineSelector_Count;

		if(GetLineSelector(&eLineSelector))
		{
			eReturn = EBaumerSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eLineSelector != EDeviceBaumerLineSelector_Line0)
		{
			eReturn = EBaumerSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			bool bError = false;

			try
			{
				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_LINESELECTOR))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				CString strLine(m_pDevice->GetRemoteNode(SFNC_LINESELECTOR)->GetString().get());

				if(strLine.CompareNoCase(g_lpszLineSelector[EDeviceBaumerLineSelector_Line0]))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_LINEMODE))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(m_pDevice->GetRemoteNode(SFNC_LINEMODE)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_LINEMODE)->SetString(CStringA(g_lpszLineMode[eParam]).GetBuffer());
			}
			catch(BGAPI2::Exceptions::IException&)
			{
				bError = true;
			}

			if(bError)
			{
				eReturn = EBaumerSetFunction_NotSupportError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], g_lpszLineMode[nPreValue], g_lpszLineMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetLine1Inverter(EDeviceBaumerSwitch* pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_Line1Inverter));

		if(nData < 0 || nData >= (int)EDeviceBaumerSwitch_Count)
		{
			eReturn = EBaumerGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaumerSwitch)nData;

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetLine1Inverter(EDeviceBaumerSwitch eParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_Line1Inverter;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < (EDeviceBaumerSwitch)0 || eParam >= EDeviceBaumerSwitch_Count)
		{
			eReturn = EBaumerSetFunction_NotSupportError;
			break;
		}

		EDeviceBaumerLineSelector eLineSelector = EDeviceBaumerLineSelector_Count;

		if(GetLineSelector(&eLineSelector))
		{
			eReturn = EBaumerSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eLineSelector != EDeviceBaumerLineSelector_Line1)
		{
			eReturn = EBaumerSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			bool bError = false;

			try
			{
				if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_LINESELECTOR))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				CString strLine(m_pDevice->GetRemoteNode(SFNC_LINESELECTOR)->GetString().get());

				if(strLine.CompareNoCase(g_lpszLineSelector[EDeviceBaumerLineSelector_Line1]))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_LINEINVERTER))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(m_pDevice->GetRemoteNode(SFNC_LINEINVERTER)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_LINEINVERTER)->SetBool(eParam);
			}
			catch(BGAPI2::Exceptions::IException&)
			{
				bError = true;
			}

			if(bError)
			{
				eReturn = EBaumerSetFunction_NotSupportError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], g_lpszSwitch[nPreValue], g_lpszSwitch[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetLine1Mode(EDeviceBaumerLineMode* pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_Line1Mode));

		if(nData < 0 || nData >= (int)EDeviceBaumerLineMode_Count)
		{
			eReturn = EBaumerGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaumerLineMode)nData;

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetLine1Mode(EDeviceBaumerLineMode eParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_Line1Mode;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < (EDeviceBaumerLineMode)0 || eParam >= EDeviceBaumerLineMode_Count)
		{
			eReturn = EBaumerSetFunction_NotSupportError;
			break;
		}

		EDeviceBaumerLineSelector eLineSelector = EDeviceBaumerLineSelector_Count;

		if(GetLineSelector(&eLineSelector))
		{
			eReturn = EBaumerSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eLineSelector != EDeviceBaumerLineSelector_Line1)
		{
			eReturn = EBaumerSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			bool bError = false;

			try
			{
				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_LINESELECTOR))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				CString strLine(m_pDevice->GetRemoteNode(SFNC_LINESELECTOR)->GetString().get());

				if(strLine.CompareNoCase(g_lpszLineSelector[EDeviceBaumerLineSelector_Line1]))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_LINEMODE))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(m_pDevice->GetRemoteNode(SFNC_LINEMODE)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_LINEMODE)->SetString(CStringA(g_lpszLineMode[eParam]).GetBuffer());
			}
			catch(BGAPI2::Exceptions::IException&)
			{
				bError = true;
			}

			if(bError)
			{
				eReturn = EBaumerSetFunction_NotSupportError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], g_lpszLineMode[nPreValue], g_lpszLineMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetLine1Source(EDeviceBaumerLineSource* pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_Line1Source));

		if(nData < 0 || nData >= (int)EDeviceBaumerLineSource_Count)
		{
			eReturn = EBaumerGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaumerLineSource)nData;

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetLine1Source(EDeviceBaumerLineSource eParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_Line1Source;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < (EDeviceBaumerLineSource)0 || eParam >= EDeviceBaumerLineSource_Count)
		{
			eReturn = EBaumerSetFunction_NotSupportError;
			break;
		}

		EDeviceBaumerLineSelector eLineSelector = EDeviceBaumerLineSelector_Count;

		if(GetLineSelector(&eLineSelector))
		{
			eReturn = EBaumerSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eLineSelector != EDeviceBaumerLineSelector_Line1)
		{
			eReturn = EBaumerSetFunction_NotSupportError;
			break;
		}

		EDeviceBaumerLineMode eLineMode = EDeviceBaumerLineMode_Count;

		if(GetLine1Mode(&eLineMode))
		{
			eReturn = EBaumerSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eLineMode != EDeviceBaumerLineMode_Output)
		{
			eReturn = EBaumerSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			bool bError = false;

			try
			{
				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_LINESELECTOR))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				CString strLine(m_pDevice->GetRemoteNode(SFNC_LINESELECTOR)->GetString().get());

				if(strLine.CompareNoCase(g_lpszLineSelector[EDeviceBaumerLineSelector_Line1]))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_LINESOURCE))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(m_pDevice->GetRemoteNode(SFNC_LINESOURCE)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_LINESOURCE)->SetString(CStringA(g_lpszLineSource[eParam]).GetBuffer());
			}
			catch(BGAPI2::Exceptions::IException&)
			{
				bError = true;
			}

			if(bError)
			{
				eReturn = EBaumerSetFunction_NotSupportError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], g_lpszLineSource[nPreValue], g_lpszLineSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetLine2Inverter(EDeviceBaumerSwitch* pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_Line2Inverter));

		if(nData < 0 || nData >= (int)EDeviceBaumerSwitch_Count)
		{
			eReturn = EBaumerGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaumerSwitch)nData;

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetLine2Inverter(EDeviceBaumerSwitch eParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_Line2Inverter;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < (EDeviceBaumerSwitch)0 || eParam >= EDeviceBaumerSwitch_Count)
		{
			eReturn = EBaumerSetFunction_NotSupportError;
			break;
		}

		EDeviceBaumerLineSelector eLineSelector = EDeviceBaumerLineSelector_Count;

		if(GetLineSelector(&eLineSelector))
		{
			eReturn = EBaumerSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eLineSelector != EDeviceBaumerLineSelector_Line2)
		{
			eReturn = EBaumerSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			bool bError = false;

			try
			{
				if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_LINESELECTOR))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				CString strLine(m_pDevice->GetRemoteNode(SFNC_LINESELECTOR)->GetString().get());

				if(strLine.CompareNoCase(g_lpszLineSelector[EDeviceBaumerLineSelector_Line2]))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_LINEINVERTER))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(m_pDevice->GetRemoteNode(SFNC_LINEINVERTER)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_LINEINVERTER)->SetBool(eParam);
			}
			catch(BGAPI2::Exceptions::IException&)
			{
				bError = true;
			}

			if(bError)
			{
				eReturn = EBaumerSetFunction_NotSupportError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], g_lpszSwitch[nPreValue], g_lpszSwitch[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetLine2Mode(EDeviceBaumerLineMode* pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_Line2Mode));

		if(nData < 0 || nData >= (int)EDeviceBaumerLineMode_Count)
		{
			eReturn = EBaumerGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaumerLineMode)nData;

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetLine2Mode(EDeviceBaumerLineMode eParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_Line2Mode;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < (EDeviceBaumerLineMode)0 || eParam >= EDeviceBaumerLineMode_Count)
		{
			eReturn = EBaumerSetFunction_NotSupportError;
			break;
		}

		EDeviceBaumerLineSelector eLineSelector = EDeviceBaumerLineSelector_Count;

		if(GetLineSelector(&eLineSelector))
		{
			eReturn = EBaumerSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eLineSelector != EDeviceBaumerLineSelector_Line2)
		{
			eReturn = EBaumerSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			bool bError = false;

			try
			{
				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_LINESELECTOR))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				CString strLine(m_pDevice->GetRemoteNode(SFNC_LINESELECTOR)->GetString().get());

				if(strLine.CompareNoCase(g_lpszLineSelector[EDeviceBaumerLineSelector_Line2]))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_LINEMODE))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(m_pDevice->GetRemoteNode(SFNC_LINEMODE)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_LINEMODE)->SetString(CStringA(g_lpszLineMode[eParam]).GetBuffer());
			}
			catch(BGAPI2::Exceptions::IException&)
			{
				bError = true;
			}

			if(bError)
			{
				eReturn = EBaumerSetFunction_NotSupportError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], g_lpszLineMode[nPreValue], g_lpszLineMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetLine2Source(EDeviceBaumerLineSource* pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_Line2Source));

		if(nData < 0 || nData >= (int)EDeviceBaumerLineSource_Count)
		{
			eReturn = EBaumerGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaumerLineSource)nData;

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetLine2Source(EDeviceBaumerLineSource eParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_Line2Source;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < (EDeviceBaumerLineSource)0 || eParam >= EDeviceBaumerLineSource_Count)
		{
			eReturn = EBaumerSetFunction_NotSupportError;
			break;
		}

		EDeviceBaumerLineSelector eLineSelector = EDeviceBaumerLineSelector_Count;

		if(GetLineSelector(&eLineSelector))
		{
			eReturn = EBaumerSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eLineSelector != EDeviceBaumerLineSelector_Line2)
		{
			eReturn = EBaumerSetFunction_NotSupportError;
			break;
		}

		EDeviceBaumerLineMode eLineMode = EDeviceBaumerLineMode_Count;

		if(GetLine2Mode(&eLineMode))
		{
			eReturn = EBaumerSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eLineMode != EDeviceBaumerLineMode_Output)
		{
			eReturn = EBaumerSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			bool bError = false;

			try
			{
				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_LINESELECTOR))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				CString strLine(m_pDevice->GetRemoteNode(SFNC_LINESELECTOR)->GetString().get());

				if(strLine.CompareNoCase(g_lpszLineSelector[EDeviceBaumerLineSelector_Line2]))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_LINESOURCE))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(m_pDevice->GetRemoteNode(SFNC_LINESOURCE)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_LINESOURCE)->SetString(CStringA(g_lpszLineSource[eParam]).GetBuffer());
			}
			catch(BGAPI2::Exceptions::IException&)
			{
				bError = true;
			}

			if(bError)
			{
				eReturn = EBaumerSetFunction_NotSupportError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], g_lpszLineSource[nPreValue], g_lpszLineSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetLine3Inverter(EDeviceBaumerSwitch* pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_Line3Inverter));

		if(nData < 0 || nData >= (int)EDeviceBaumerSwitch_Count)
		{
			eReturn = EBaumerGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaumerSwitch)nData;

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetLine3Inverter(EDeviceBaumerSwitch eParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_Line3Inverter;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < (EDeviceBaumerSwitch)0 || eParam >= EDeviceBaumerSwitch_Count)
		{
			eReturn = EBaumerSetFunction_NotSupportError;
			break;
		}

		EDeviceBaumerLineSelector eLineSelector = EDeviceBaumerLineSelector_Count;

		if(GetLineSelector(&eLineSelector))
		{
			eReturn = EBaumerSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eLineSelector != EDeviceBaumerLineSelector_Line3)
		{
			eReturn = EBaumerSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			bool bError = false;

			try
			{
				if(m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_LINESELECTOR))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				CString strLine(m_pDevice->GetRemoteNode(SFNC_LINESELECTOR)->GetString().get());

				if(strLine.CompareNoCase(g_lpszLineSelector[EDeviceBaumerLineSelector_Line3]))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_LINEINVERTER))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(m_pDevice->GetRemoteNode(SFNC_LINEINVERTER)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_LINEINVERTER)->SetBool(eParam);
			}
			catch(BGAPI2::Exceptions::IException&)
			{
				bError = true;
			}

			if(bError)
			{
				eReturn = EBaumerSetFunction_NotSupportError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], g_lpszSwitch[nPreValue], g_lpszSwitch[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetLine3Mode(EDeviceBaumerLineMode* pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_Line3Mode));

		if(nData < 0 || nData >= (int)EDeviceBaumerLineMode_Count)
		{
			eReturn = EBaumerGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaumerLineMode)nData;

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetLine3Mode(EDeviceBaumerLineMode eParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_Line3Mode;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < (EDeviceBaumerLineMode)0 || eParam >= EDeviceBaumerLineMode_Count)
		{
			eReturn = EBaumerSetFunction_NotSupportError;
			break;
		}

		EDeviceBaumerLineSelector eLineSelector = EDeviceBaumerLineSelector_Count;

		if(GetLineSelector(&eLineSelector))
		{
			eReturn = EBaumerSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eLineSelector != EDeviceBaumerLineSelector_Line3)
		{
			eReturn = EBaumerSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			bool bError = false;

			try
			{
				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_LINESELECTOR))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				CString strLine(m_pDevice->GetRemoteNode(SFNC_LINESELECTOR)->GetString().get());

				if(strLine.CompareNoCase(g_lpszLineSelector[EDeviceBaumerLineSelector_Line3]))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_LINEMODE))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(m_pDevice->GetRemoteNode(SFNC_LINEMODE)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_LINEMODE)->SetString(CStringA(g_lpszLineMode[eParam]).GetBuffer());
			}
			catch(BGAPI2::Exceptions::IException&)
			{
				bError = true;
			}

			if(bError)
			{
				eReturn = EBaumerSetFunction_NotSupportError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], g_lpszLineMode[nPreValue], g_lpszLineMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetLine3Source(EDeviceBaumerLineSource* pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_Line3Source));

		if(nData < 0 || nData >= (int)EDeviceBaumerLineSource_Count)
		{
			eReturn = EBaumerGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaumerLineSource)nData;

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetLine3Source(EDeviceBaumerLineSource eParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_Line3Source;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < (EDeviceBaumerLineSource)0 || eParam >= EDeviceBaumerLineSource_Count)
		{
			eReturn = EBaumerSetFunction_NotSupportError;
			break;
		}

		EDeviceBaumerLineSelector eLineSelector = EDeviceBaumerLineSelector_Count;

		if(GetLineSelector(&eLineSelector))
		{
			eReturn = EBaumerSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eLineSelector != EDeviceBaumerLineSelector_Line3)
		{
			eReturn = EBaumerSetFunction_NotSupportError;
			break;
		}

		EDeviceBaumerLineMode eLineMode = EDeviceBaumerLineMode_Count;

		if(GetLine3Mode(&eLineMode))
		{
			eReturn = EBaumerSetFunction_ReadOnDatabaseError;
			break;
		}

		if(eLineMode != EDeviceBaumerLineMode_Output)
		{
			eReturn = EBaumerSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			bool bError = false;

			try
			{
				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_LINESELECTOR))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				CString strLine(m_pDevice->GetRemoteNode(SFNC_LINESELECTOR)->GetString().get());

				if(strLine.CompareNoCase(g_lpszLineSelector[EDeviceBaumerLineSelector_Line3]))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_LINESOURCE))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(m_pDevice->GetRemoteNode(SFNC_LINESOURCE)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_LINESOURCE)->SetString(CStringA(g_lpszLineSource[eParam]).GetBuffer());
			}
			catch(BGAPI2::Exceptions::IException&)
			{
				bError = true;
			}

			if(bError)
			{
				eReturn = EBaumerSetFunction_NotSupportError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], g_lpszLineSource[nPreValue], g_lpszLineSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetUserOutputSelector(EDeviceBaumerUserOutputSelector* pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_UserOutputSelector));

		if(nData < 0 || nData >= (int)EDeviceBaumerUserOutputSelector_Count)
		{
			eReturn = EBaumerGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaumerUserOutputSelector)nData;

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetUserOutputSelector(EDeviceBaumerUserOutputSelector eParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_UserOutputSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < (EDeviceBaumerUserOutputSelector)0 || eParam >= EDeviceBaumerUserOutputSelector_Count)
		{
			eReturn = EBaumerSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			bool bError = false;

			try
			{
				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_USEROUTPUTSELECTOR))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(m_pDevice->GetRemoteNode(SFNC_USEROUTPUTSELECTOR)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_USEROUTPUTSELECTOR)->SetString(CStringA(g_lpszUserOutputSelector[eParam]).GetBuffer());
			}
			catch(BGAPI2::Exceptions::IException&)
			{
				bError = true;
			}

			if(bError)
			{
				eReturn = EBaumerSetFunction_NotSupportError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], g_lpszUserOutputSelector[nPreValue], g_lpszUserOutputSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetUserOutputValue(EDeviceBaumerSwitch* pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_UserOutputValue));

		if(nData < 0 || nData >= (int)EDeviceBaumerSwitch_Count)
		{
			eReturn = EBaumerGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaumerSwitch)nData;

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetUserOutputValue(EDeviceBaumerSwitch eParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_UserOutputValue;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < (EDeviceBaumerSwitch)0 || eParam >= EDeviceBaumerSwitch_Count)
		{
			eReturn = EBaumerSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			bool bError = false;

			try
			{
				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_USEROUTPUTVALUE))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(m_pDevice->GetRemoteNode(SFNC_USEROUTPUTVALUE)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_USEROUTPUTVALUE)->SetBool(eParam);
			}
			catch(BGAPI2::Exceptions::IException&)
			{
				bError = true;
			}

			if(bError)
			{
				eReturn = EBaumerSetFunction_NotSupportError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], g_lpszSwitch[nPreValue], g_lpszSwitch[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetUserOutputValueAll(int* pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_UserOutputValueAll));

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetUserOutputValueAll(int nParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_UserOutputValueAll;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(IsInitialized())
		{
			bool bError = false;

			if(!m_pDevice)
			{
				eReturn = EBaumerSetFunction_DeviceNullptrError;
				break;
			}

			try
			{
				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_USEROUTPUTVALUEALL))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(m_pDevice->GetRemoteNode(SFNC_USEROUTPUTVALUEALL)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_USEROUTPUTVALUEALL)->SetInt(nParam);
			}
			catch(BGAPI2::Exceptions::IException&)
			{
				bError = true;
			}

			if(bError)
			{
				eReturn = EBaumerSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetTimerSelector(EDeviceBaumerTimerSelector* pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_TimerSelector));

		if(nData < 0 || nData >= (int)EDeviceBaumerTimerSelector_Count)
		{
			eReturn = EBaumerGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaumerTimerSelector)nData;

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetTimerSelector(EDeviceBaumerTimerSelector eParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_TimerSelector;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < (EDeviceBaumerTimerSelector)0 || eParam >= EDeviceBaumerTimerSelector_Count)
		{
			eReturn = EBaumerSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			bool bError = false;

			try
			{
				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_TIMERSELECTOR))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(m_pDevice->GetRemoteNode(SFNC_TIMERSELECTOR)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_TIMERSELECTOR)->SetString(CStringA(g_lpszTimerSelector[eParam]).GetBuffer());
			}
			catch(BGAPI2::Exceptions::IException&)
			{
				bError = true;
			}

			if(bError)
			{
				eReturn = EBaumerSetFunction_NotSupportError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], g_lpszTimerSelector[nPreValue], g_lpszTimerSelector[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetTimerTriggerSource(EDeviceBaumerTimerTriggerSource* pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_TimerTriggerSource));

		if(nData < 0 || nData >= (int)EDeviceBaumerTimerTriggerSource_Count)
		{
			eReturn = EBaumerGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaumerTimerTriggerSource)nData;

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetTimerTriggerSource(EDeviceBaumerTimerTriggerSource eParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_TimerTriggerSource;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < (EDeviceBaumerTimerTriggerSource)0 || eParam >= EDeviceBaumerTimerTriggerSource_Count)
		{
			eReturn = EBaumerSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			bool bError = false;

			try
			{
				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_TIMERTRIGGERSOURCE))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(m_pDevice->GetRemoteNode(SFNC_TIMERTRIGGERSOURCE)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_TIMERTRIGGERSOURCE)->SetString(CStringA(g_lpszTimerTriggerSource[eParam]).GetBuffer());
			}
			catch(BGAPI2::Exceptions::IException&)
			{
				bError = true;
			}

			if(bError)
			{
				eReturn = EBaumerSetFunction_NotSupportError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], g_lpszTimerTriggerSource[nPreValue], g_lpszTimerTriggerSource[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetTimerTriggerActivation(EDeviceBaumerTriggerActivation* pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterBaumerGigE_TimerTriggerActivation));

		if(nData < 0 || nData >= (int)EDeviceBaumerTriggerActivation_Count)
		{
			eReturn = EBaumerGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceBaumerTriggerActivation)nData;

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetTimerTriggerActivation(EDeviceBaumerTriggerActivation eParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_TimerTriggerActivation;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < (EDeviceBaumerTriggerActivation)0 || eParam >= EDeviceBaumerTriggerActivation_Count)
		{
			eReturn = EBaumerSetFunction_NotSupportError;
			break;
		}

		if(IsInitialized())
		{
			bool bError = false;

			try
			{
				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_TIMERTRIGGERACTIVATION))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(m_pDevice->GetRemoteNode(SFNC_TIMERTRIGGERACTIVATION)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_TIMERTRIGGERACTIVATION)->SetString(CStringA(g_lpszActivation[eParam]).GetBuffer());
			}
			catch(BGAPI2::Exceptions::IException&)
			{
				bError = true;
			}

			if(bError)
			{
				eReturn = EBaumerSetFunction_NotSupportError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), eParam);

		if(!SetParamValue(eSaveNum, strData))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], g_lpszActivation[nPreValue], g_lpszActivation[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetTimerDuration(double* pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterBaumerGigE_TimerDuration));

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetTimerDuration(double dblParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_TimerDuration;

	double dblPreValue = _ttof(GetParamValue(eSaveNum));

	do
	{
		if(IsInitialized())
		{
			bool bError = false;

			try
			{
				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_TIMERDURATION))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(m_pDevice->GetRemoteNode(SFNC_TIMERDURATION)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_TIMERDURATION)->SetDouble(dblParam);
			}
			catch(BGAPI2::Exceptions::IException&)
			{
				bError = true;
			}

			if(bError)
			{
				eReturn = EBaumerSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EBaumerGetFunction CDeviceBaumerGigE::GetTimerDelay(double* pParam)
{
	EBaumerGetFunction eReturn = EBaumerGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EBaumerGetFunction_NullptrError;
			break;
		}

		*pParam = _ttof(GetParamValue(EDeviceParameterBaumerGigE_TimerDelay));

		eReturn = EBaumerGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EBaumerSetFunction CDeviceBaumerGigE::SetTimerDelay(double dblParam)
{
	EBaumerSetFunction eReturn = EBaumerSetFunction_UnknownError;

	EDeviceParameterBaumerGigE eSaveNum = EDeviceParameterBaumerGigE_TimerDelay;

	double dblPreValue = _ttof(GetParamValue(eSaveNum));

	do
	{
		if(IsInitialized())
		{
			bool bError = false;

			try
			{
				if(!m_pDevice->GetRemoteNodeList()->GetNodePresent(SFNC_TIMERDELAY))
				{
					eReturn = EBaumerSetFunction_NotSupportError;
					break;
				}

				if(m_pDevice->GetRemoteNode(SFNC_TIMERDELAY)->GetLocked())
				{
					eReturn = EBaumerSetFunction_FunctionLockedError;
					break;
				}

				m_pDevice->GetRemoteNode(SFNC_TIMERDELAY)->SetDouble(dblParam);
			}
			catch(BGAPI2::Exceptions::IException&)
			{
				bError = true;
			}

			if(bError)
			{
				eReturn = EBaumerSetFunction_NotSupportError;
				break;
			}
		}

		CString strValue;
		strValue.Format(_T("%f"), dblParam);

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EBaumerSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EBaumerSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_f_to_f), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamBaumerGigE[eSaveNum], dblPreValue, dblParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}


bool CDeviceBaumerGigE::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = false;

	bool bFoundID = true;

	do
	{
		switch(nParam)
		{
		case EDeviceParameterBaumerGigE_DeviceID:
			bReturn = !SetDeviceID(strValue);			
			break;
		case EDeviceParameterBaumerGigE_GrabCount:
			bReturn = !SetGrabCount(_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_CanvasWidth:
			bReturn = !SetCanvasWidth(_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_CanvasHeight:
			bReturn = !SetCanvasHeight(_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_OffsetX:
			bReturn = !SetOffsetX(_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_OffsetY:
			bReturn = !SetOffsetY(_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_GrabWaitingTime:
			bReturn = !SetGrabWaitingTime(_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_GrabBufferCount:
			bReturn = !SetGrabBufferCount(_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_PixelFormat:
			bReturn = !SetPixelFormat((EDeviceBaumerPixelFormat)_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_FlipHorizontal:
			bReturn = !SetFlipHorizontal(_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_FlipVertical:
			bReturn = !SetFlipVertical(_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_TriggerSelector:
			bReturn = !SetTriggerSelector((EDeviceBaumerTriggerSelector)_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_FrameStartTriggerMode:
			bReturn = !SetFrameStartMode((EDeviceBaumerSwitch)_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_FrameStartTriggerSource:
			bReturn = !SetFrameStartTriggerSource((EDeviceBaumerTriggerSource)_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_FrameStartTriggerActivation:
			bReturn = !SetFrameStartTriggerActivation((EDeviceBaumerTriggerActivation)_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_LineStartTriggerMode:
			bReturn = !SetLineStartMode((EDeviceBaumerSwitch)_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_LineStartTriggerSource:
			bReturn = !SetLineStartTriggerSource((EDeviceBaumerTriggerSource)_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_LineStartTriggerActivation:
			bReturn = !SetLineStartTriggerActivation((EDeviceBaumerTriggerActivation)_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_AcquisitionStartTriggerMode:
			bReturn = !SetAcquisitionStartMode((EDeviceBaumerSwitch)_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_AcquisitionStartTriggerSource:
			bReturn = !SetAcquisitionStartTriggerSource((EDeviceBaumerTriggerSource)_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_AcquisitionStartTriggerActivation:
			bReturn = !SetAcquisitionStartTriggerActivation((EDeviceBaumerTriggerActivation)_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_ExposureAuto:
			bReturn = !SetExposureAuto((EDeviceBaumerAuto)_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_ExposureMode:
			bReturn = !SetExposureMode((EDeviceBaumerExposureMode)_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_Exposure:
			bReturn = !SetExposure(_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_GainAuto:
			bReturn = !SetGainAuto((EDeviceBaumerAuto)_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_GainSelector:
			bReturn = !SetGainSelector((EDeviceBaumerGainSelector)_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_Gain:
			bReturn = !SetAllGain(_ttof(strValue));
			break;
		case EDeviceParameterBaumerGigE_AnalogGain:
			bReturn = !SetAnalogGain(_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_DigitalGain:
			bReturn = !SetDigitalGain(_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_Tap1Gain:
			bReturn = !SetTap1Gain(_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_Tap2Gain:
			bReturn = !SetTap2Gain(_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_LineSelector:
			bReturn = !SetLineSelector((EDeviceBaumerLineSelector)_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_Line0Inverter:
			bReturn = !SetLine0Inverter((EDeviceBaumerSwitch)_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_Line0Mode:
			bReturn = !SetLine0Mode((EDeviceBaumerLineMode)_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_Line1Inverter:
			bReturn = !SetLine1Inverter((EDeviceBaumerSwitch)_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_Line1Mode:
			bReturn = !SetLine1Mode((EDeviceBaumerLineMode)_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_Line1Source:
			bReturn = !SetLine1Source((EDeviceBaumerLineSource)_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_Line2Inverter:
			bReturn = !SetLine2Inverter((EDeviceBaumerSwitch)_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_Line2Mode:
			bReturn = !SetLine2Mode((EDeviceBaumerLineMode)_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_Line2Source:
			bReturn = !SetLine2Source((EDeviceBaumerLineSource)_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_Line3Inverter:
			bReturn = !SetLine3Inverter((EDeviceBaumerSwitch)_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_Line3Mode:
			bReturn = !SetLine3Mode((EDeviceBaumerLineMode)_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_Line3Source:
			bReturn = !SetLine3Source((EDeviceBaumerLineSource)_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_UserOutputSelector:
			bReturn = !SetUserOutputSelector((EDeviceBaumerUserOutputSelector)_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_UserOutputValue:
			bReturn = !SetUserOutputValue((EDeviceBaumerSwitch)_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_UserOutputValueAll:
			bReturn = !SetUserOutputValueAll(_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_TimerSelector:
			bReturn = !SetTimerSelector((EDeviceBaumerTimerSelector)_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_TimerTriggerSource:
			bReturn = !SetTimerTriggerSource((EDeviceBaumerTimerTriggerSource)_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_TimerTriggerActivation:
			bReturn = !SetTimerTriggerActivation((EDeviceBaumerTriggerActivation)_ttoi(strValue));
			break;
		case EDeviceParameterBaumerGigE_TimerDuration:
			bReturn = !SetTimerDuration(_ttof(strValue));
			break;
		case EDeviceParameterBaumerGigE_TimerDelay:
			bReturn = !SetTimerDelay(_ttof(strValue));
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

bool CDeviceBaumerGigE::DoesModuleExist()
{
	bool bReturn = false;

	CString strModuleName;

	do
	{
		strModuleName.Format(_T("bgapi2_genicam.dll"));

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

		strModuleName.Format(_T("bgapi2_img.dll"));

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

void CDeviceBaumerGigE::CallbackFunction(void * callBackOwner, BGAPI2::Buffer * pBufferFilled)
{
	if(!pBufferFilled)
		return;

	CDeviceBaumerGigE* pInstance = (CDeviceBaumerGigE*)callBackOwner;
	if(!pInstance)
		return;

	BGAPI2::DataStream* pStream = pInstance->m_pDataStream;

	if(!pStream)
		return;
	
	if(!pInstance)
		return;

	do
	{
		pInstance->NextImageIndex();

		CRavidImage* pCurrentImage = pInstance->GetImageInfo();

		BYTE* pCurrentBuffer = pCurrentImage->GetBuffer();

		const int64_t i64Width = (int64_t)pCurrentImage->GetSizeX();
		const int64_t i64Height = (int64_t)pCurrentImage->GetSizeY();
		const int64_t i64ImageSizeByte = (int64_t)pCurrentImage->GetImageSizeByte();

		bool bError = false;

		pInstance->m_nGrabCount--;

		if(pInstance->m_nGrabCount <= 0)
		{
			pInstance->m_bIsLive = false;
			pInstance->m_bIsGrabAvailable = true;

			try
			{
				pInstance->m_pDevice->GetRemoteNode("AcquisitionStop")->Execute();
			}
			catch(BGAPI2::Exceptions::IException&)
			{
			}			
		}

		if(pBufferFilled == NULL)
			break;
		else if(pBufferFilled->GetIsIncomplete())
		{
			try
			{
				pBufferFilled->QueueBuffer();
			}
			catch(BGAPI2::Exceptions::IException&)
			{
			}
		}
		else
		{
			if(pInstance->m_nGrabCount >= 0)
			{
				EDeviceBaumerPixelFormat eFormat = EDeviceBaumerPixelFormat_Mono8;

				if(pInstance->GetPixelFormat(&eFormat))
					break;

				if(!pCurrentImage->IsColor())
				{
					BYTE* pNewBuffer = nullptr;
					BGAPI2::Image* pImage = nullptr;
					BGAPI2::Image* pTransImage = nullptr;

					switch(eFormat)
					{
					case EDeviceBaumerPixelFormat_Mono8:
					case EDeviceBaumerPixelFormat_Mono10:
					case EDeviceBaumerPixelFormat_Mono12:
					case EDeviceBaumerPixelFormat_Mono14:
					case EDeviceBaumerPixelFormat_Mono16:
						{
							try
							{
								bo_uint64 stWidth = pBufferFilled->GetWidth();
								bo_uint64 stHeight = pBufferFilled->GetHeight();
								BGAPI2::String strPixel = pBufferFilled->GetPixelFormat();
								size_t stMemSize = pBufferFilled->GetMemSize();

								pNewBuffer = new BYTE[stMemSize];
								if(!pNewBuffer)
									break;

								memcpy(pNewBuffer, pBufferFilled->GetMemPtr(), stMemSize);

								pBufferFilled->QueueBuffer();

								pImage = pInstance->m_pImgProc->CreateImage(stWidth, stHeight, strPixel, pNewBuffer, stMemSize);

								if(!pImage)
									break;
								
								memcpy(pCurrentBuffer, pImage->GetBuffer(), i64ImageSizeByte);

								pImage->Release();
								pImage = nullptr;
							}
							catch(BGAPI2::Exceptions::IException&)
							{
								if(pImage)
								{
									pImage->Release();
									pImage = nullptr;
								}
							}
						}
						break;
					default:
						{
							try
							{
								bo_uint64 stWidth = pBufferFilled->GetWidth();
								bo_uint64 stHeight = pBufferFilled->GetHeight();
								BGAPI2::String strPixel = pBufferFilled->GetPixelFormat();
								size_t stMemSize = pBufferFilled->GetMemSize();

								pNewBuffer = new BYTE[stMemSize];
								if(!pNewBuffer)
									break;

								memcpy(pNewBuffer, pBufferFilled->GetMemPtr(), stMemSize);

								pBufferFilled->QueueBuffer();

								pImage = pInstance->m_pImgProc->CreateImage(stWidth, stHeight, strPixel, pNewBuffer, stMemSize);

								if(!pImage)
									break;

								pTransImage = pInstance->m_pImgProc->CreateTransformedImage(pImage, "Mono8");

								if(!pTransImage)
								{
									pImage->Release();
									pImage = nullptr;
									break;
								}

								memcpy(pCurrentBuffer, pTransImage->GetBuffer(), i64ImageSizeByte);

								pImage->Release();
								pImage = nullptr;

								pTransImage->Release();
								pTransImage = nullptr;
							}
							catch(BGAPI2::Exceptions::IException&)
							{
								if(pImage)
								{
									pImage->Release();
									pImage = nullptr;
								}

								if(pTransImage)
								{
									pTransImage->Release();
									pTransImage = nullptr;
								}
							}
						}
						break;
					}

					if(pNewBuffer)
					{
						delete pNewBuffer;
						pNewBuffer = nullptr;
					}
				}
				else
				{
					BYTE* pNewBuffer = nullptr;
					BGAPI2::Image* pImage = nullptr;
					BGAPI2::Image* pTransImage = nullptr;

					do 
					{
						try
						{
							bo_uint64 stWidth = pBufferFilled->GetWidth();
							bo_uint64 stHeight = pBufferFilled->GetHeight();
							BGAPI2::String strPixel = pBufferFilled->GetPixelFormat();
							size_t stMemSize = pBufferFilled->GetMemSize();

							pNewBuffer = new BYTE[stMemSize];
							if(!pNewBuffer)
								break;

							memcpy(pNewBuffer, pBufferFilled->GetMemPtr(), stMemSize);

							pBufferFilled->QueueBuffer();

							pImage = pInstance->m_pImgProc->CreateImage(stWidth, stHeight, strPixel, pNewBuffer, stMemSize);

							if(!pImage)
								break;

							pTransImage = pInstance->m_pImgProc->CreateTransformedImage(pImage, "BGR8");

							if(!pTransImage)
							{
								pImage->Release();
								pImage = nullptr;
								break;
							}

							memcpy(pCurrentBuffer, pTransImage->GetBuffer(), i64ImageSizeByte);

							pImage->Release();
							pImage = nullptr;

							pTransImage->Release();
							pTransImage = nullptr;
						}
						catch(BGAPI2::Exceptions::IException&)
						{
							if(pImage)
							{
								pImage->Release();
								pImage = nullptr;
							}

							if(pTransImage)
							{
								pTransImage->Release();
								pTransImage = nullptr;
							}
						}
					} 
					while(false);
					
					if(pNewBuffer)
					{
						delete pNewBuffer;
						pNewBuffer = nullptr;
					}
				}
			}
		}

		if(pInstance->m_nGrabCount >= 0)
		{
			pInstance->ConnectImage(false);

			CEventHandlerManager::BroadcastOnAcquisition(pCurrentImage, pInstance->GetObjectID(), pInstance->GetImageView(), pInstance);
		}
	}
	while(false);
}

#endif