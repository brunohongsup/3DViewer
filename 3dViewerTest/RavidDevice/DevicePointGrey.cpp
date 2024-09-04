#include "stdafx.h"

#include "DevicePointGrey.h"

#ifdef USING_DEVICE

#include "../RavidCore/RavidRect.h"
#include "../RavidCore/RavidImage.h"
#include "../RavidCore/RavidDatabase.h"
#include "../RavidCore/RavidRecordset.h"
#include "../RavidCore/PerformanceCounter.h"

#include "../RavidFramework/LogManager.h"
#include "../RavidFramework/DeviceManager.h"
#include "../RavidFramework/LibraryManager.h"
#include "../RavidFramework/FrameworkManager.h"
#include "../RavidFramework/EventHandlerManager.h"
#include "../RavidFramework/MultiLanguageManager.h"

#include "../RavidFramework/RavidImageView.h" 

#include "../Libraries/Includes/PointGrey/C/FlyCapture2_C.h"

// flycapture2_c_v140.dll
#pragma comment(lib, COMMONLIB_PREFIX "PointGrey/FlyCapture2_C_v140.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SOFTWARE_TRIGGER_CAMERA

using namespace Ravid;
using namespace Ravid::Device;
using namespace Ravid::Database;
using namespace Ravid::Framework;
using namespace Ravid::Algorithms;
using namespace Ravid::Miscellaneous;

IMPLEMENT_DYNAMIC(CDevicePointGrey, CDeviceCamera)

BEGIN_MESSAGE_MAP(CDevicePointGrey, CDeviceCamera)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static LPCTSTR g_lpszDevicePointGrey[EDeviceParameterPointGrey_Count] =
{
	_T("Device ID"),
	_T("Video Modes"),
	_T("Grab Waiting Time[ms]"),
	_T("Grab Mode"),
	_T("GrabNumBuffers"),
	_T("Canvas Width"),
	_T("Canvas Height"),
	_T("Offset X"),
	_T("Offset Y"),
	_T("PixelFormat"),
	_T("Format Mode"),
	_T("Camera Setting"),
	_T("Grab Count"),
	_T("Packet Size"),
	_T("Packet Delay"),
	_T("Brightness Setting"),
	_T("Brightness Manual"),
	_T("Brightness Value"),
	_T("Exposure Setting"),
	_T("Exposure Manual"),
	_T("Exposure Value"),
	_T("Sharpness Setting"),
	_T("Sharpness Manual"),
	_T("Sharpness Value"),
	_T("WhiteBalance Setting"),
	_T("WhiteBalance Manual"),
	_T("WhiteBalance Value"),
	_T("Hue Setting"),
	_T("Hue Manual"),
	_T("Hue Value"),
	_T("Saturation Setting"),
	_T("Saturation Manual"),
	_T("Saturation Value"),
	_T("Gamma Setting"),
	_T("Gamma Manual"),
	_T("Gamma Value"),
	_T("Iris Setting"),
	_T("Iris Manual"),
	_T("Iris Value"),
	_T("Focus Setting"),
	_T("Focus Manual"),
	_T("Focus Value"),
	_T("Zoom Setting"),
	_T("Zoom Manual"),
	_T("Zoom Value"),
	_T("Pan Setting"),
	_T("Pan Manual"),
	_T("Pan Value"),
	_T("Tilt Setting"),
	_T("Tilt Manual"),
	_T("Tilt Value"),
	_T("Shutter Setting"),
	_T("Shutter Manual"),
	_T("Shutter Value"),
	_T("Gain Setting"),
	_T("Gain Manual"),
	_T("Gain Value"),
	_T("Frame Rate Setting"),
	_T("Frame Rate Manual"),
	_T("Frame Rate Value"),
	_T("Trigger Setting"),
	_T("Trigger On/Off"),
	_T("Trigger Mode"),
	_T("Trigger Source"),
	_T("Trigger Parameter"),
	_T("Trigger Polarity"),
};

static LPCTSTR g_lpszFrameRate[EDevicePointGreyFrameRate_Count] =
{
	_T("1_875"),
	_T("3_75"),
	_T("7_5"),
	_T("15"),
	_T("30"),
	_T("60"),
	_T("120"),
	_T("240"),
	_T("Custom"),
};

static LPCTSTR g_lpszPixelFormat[EDevicePointGreyPixelFormat_Count] =
{
	_T("MONO8"),
	_T("411YUV8"),
	_T("422YUV8"),
	_T("444YUV8"),
	_T("RGB8"),
	_T("MONO16"),
	_T("RGB16"),
	_T("S_MONO16"),
	_T("S_RGB16"),
	_T("RAW8"),
	_T("RAW16"),
	_T("MONO12"),
	_T("RAW12"),
	_T("BGR"),
	_T("BGRU"),
	_T("RGB"),
	_T("RGBU"),
	_T("BGR16"),
	_T("BGRU16"),
	_T("422YUV8_JPEG"),
};

static LPCTSTR g_lpszPixelFormatValue[EDevicePointGreyPixelFormat_Count] =
{
	_T("0x80000000"),
	_T("0x40000000"),
	_T("0x20000000"),
	_T("0x10000000"),
	_T("0x08000000"),
	_T("0x04000000"),
	_T("0x02000000"),
	_T("0x01000000"),
	_T("0x00800000"),
	_T("0x00400000"),
	_T("0x00200000"),
	_T("0x00100000"),
	_T("0x00080000"),
	_T("0x80000008"),
	_T("0x40000008"),
	_T("0x08000000"),
	_T("0x40000002"),
	_T("0x02000001"),
	_T("0x02000002"),
	_T("0x40000001"),
};

static LPCTSTR g_lpszImagemode[EDevicePointGreyImagemode_Count] =
{
	_T("0"),
	_T("1"),
	_T("2"),
	_T("3"),
	_T("4"),
	_T("5"),
	_T("6"),
	_T("7"),
	_T("8"),
	_T("9"),
	_T("10"),
	_T("11"),
	_T("12"),
	_T("13"),
	_T("14"),
	_T("15"),
	_T("16"),
	_T("17"),
	_T("18"),
	_T("19"),
	_T("20"),
	_T("21"),
	_T("22"),
	_T("23"),
	_T("24"),
	_T("25"),
	_T("26"),
	_T("27"),
	_T("28"),
	_T("29"),
	_T("30"),
	_T("31"),
};

static LPCTSTR g_lpszTriggerMode[EDevicePointGreyTriggerMode_Count] =
{
	_T("StandardExternal"), //Mode 0
	_T("BulbShutter"), //Mode 1
	_T("SkipFrames"), //Mode 3
	_T("MultipleExposurePreset"), //Mode 4
	_T("MultipleExposurePulseWidth"), //Mode 5
	_T("LowSmearTrigger"), //Mode 13
	_T("OverlappedExposureReadout"), //Mode 14
	_T("MultiShot"), //Mode 15
};

static LPCTSTR g_lpszTriggerModeValue[EDevicePointGreyTriggerMode_Count] =
{
	_T("0"),
	_T("1"),
	_T("3"),
	_T("4"),
	_T("5"),
	_T("13"),
	_T("14"),
	_T("15"),
};

static LPCTSTR g_lpszGrabMode[EDevicePointGreyGrabMode_Count] =
{
	_T("DROP_FRAMES"),
	_T("BUFFER_FRAMES"),
};

static LPCTSTR g_lpszSwitch[EDevicePointGreySwitch_Count] =
{
	_T("CustomValue"),
	_T("Auto"),
	_T("OnePush"),
};

static LPCTSTR g_lpszOnOff[2] =
{
	_T("Off"),
	_T("On"),
};

CDevicePointGrey::CDevicePointGrey()
{
}


CDevicePointGrey::~CDevicePointGrey()
{
	Terminate();

	if(m_pLiveThread)
	{
		delete m_pLiveThread;
		m_pLiveThread = nullptr;
	}

	if(m_pContext)
	{
		delete m_pContext;
		m_pContext = nullptr;
	}
}

bool CDevicePointGrey::PollForTriggerReady(fc2Context context)
{
	const unsigned int k_softwareTrigger = 0x62C;
	fc2Error error;
	unsigned int regVal = 0;

	do
	{
		error = fc2ReadRegister(context, k_softwareTrigger, &regVal);
		if(error != FC2_ERROR_OK)
		{
			return false;
		}

		if(regVal >= 2147483648)
			break;

	}
	while((regVal >> 31) != 0);

	return true;
}

EDeviceInitializeResult CDevicePointGrey::Initialize()
{
	EDeviceInitializeResult eReturn = EDeviceInitializeResult_UnknownError;

	CString strMessage, strStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("PointGrey"));

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

		//fc2Error error = fc2CreateContext(&m_pContext);
		fc2Error error = fc2CreateGigEContext(&m_pContext);
		if(error != FC2_ERROR_OK)
		{
			strMessage.Format(_T("Failed to create m_pContext"));
			eReturn = EDeviceInitializeResult_NotCreateDeviceError;
			break;
		}

		unsigned int nTempNum = 0; // Int형 Get 변수
		bool bTemp = false; // Bool형 Get 변수
		CString strTemp; //CString형 Get 변수

		//Camera Connect Setting Start

		GetDeviceID(&nTempNum);

		error = fc2ForceIPAddressAutomatically(nTempNum);
		//error = fc2ForceAllIPAddressesAutomatically();
		if(error != FC2_ERROR_OK)
		{
			strMessage.Format(_T("Failed to match the camera of S/N %d"), nTempNum);
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		error = fc2GetNumOfCameras(m_pContext, &m_nNumCameras);
		if(error != FC2_ERROR_OK)
		{
			strMessage.Format(_T("Couldn't read 'NumOfCameras' from the device"));
			eReturn = EDeviceInitializeResult_NotFoundDeviceError;
			break;
		}

		if(m_nNumCameras == 0)
		{
			strMessage.Format(_T("No cameras detected"));
			eReturn = EDeviceInitializeResult_NotFoundDeviceError;
			break;
		}

		fc2PGRGuid guid;

		error = fc2GetCameraFromIndex(m_pContext, 0, &guid);
		if(error != FC2_ERROR_OK)
		{
			strMessage.Format(_T("Couldn't read 'CameraFromIndex' from the device"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		error = fc2Connect(m_pContext, &guid);
		if(error != FC2_ERROR_OK)
		{
			strMessage.Format(_T("Failed to connect camera"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		//Camera Connect Setting End

		// Trigger Mode Setting Start
		const unsigned int k_cameraPower = 0x610;
		const unsigned int k_powerVal = 0x80000000;
		error = fc2WriteRegister(m_pContext, k_cameraPower, k_powerVal);
		if(error != FC2_ERROR_OK)
		{
			strMessage.Format(_T("Couldn't write 'Register' to the device"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}


		const unsigned int millisecondsToSleep = 100;
		unsigned int regVal = 0;
		unsigned int retries = 10;

		// Wait for camera to complete power-up
		do
		{
			Sleep(millisecondsToSleep);

			error = fc2ReadRegister(m_pContext, k_cameraPower, &regVal);
			if(error == FC2_ERROR_TIMEOUT)
			{
				// ignore timeout errors, camera may not be responding to
				// register reads during power-up
			}
			else if(error != FC2_ERROR_OK)
			{
				strMessage.Format(_T("Couldn't read 'Register' from the device"));
				eReturn = EDeviceInitializeResult_ReadOnDeviceError;
				break;
			}

			retries--;
		}
		while((regVal & k_powerVal) == 0 && retries > 0);

		fc2TriggerModeInfo triggerModeInfo;
		if(fc2GetTriggerModeInfo(m_pContext, &triggerModeInfo) != FC2_ERROR_OK)
		{
			strMessage.Format(_T("Couldn't read 'TriggerModeInfo' from the device"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		if(triggerModeInfo.readOutSupported)
		{
			fc2TriggerMode triggerMode;
			error = fc2GetTriggerMode(m_pContext, &triggerMode);
			if(error != FC2_ERROR_OK)
			{
				strMessage.Format(_T("Couldn't read 'TriggerMode' from the device"));
				eReturn = EDeviceInitializeResult_ReadOnDeviceError;
				break;
			}

			if(triggerModeInfo.onOffSupported)
			{
				if(GetTriggerOnOff(&m_bTriggerOnOff))
				{
					strMessage.Format(_T("Couldn't read 'TriggerOnOff' from the device"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				triggerMode.onOff = m_bTriggerOnOff;
				error = fc2SetTriggerMode(m_pContext, &triggerMode);
				if(error != FC2_ERROR_OK)
				{
					strMessage.Format(_T("Couldn't write 'TriggerMode' to the device"));
					eReturn = EDeviceInitializeResult_ReadOnDeviceError;
					break;
				}
			}

			if(m_bTriggerOnOff)
			{
				unsigned int nTempTriggerNum = 0;

				if(GetTriggerMode(&nTempTriggerNum))
				{
					strMessage.Format(_T("Couldn't read 'TriggerMode' from the database"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				triggerMode.mode = _ttoi(g_lpszTriggerModeValue[nTempTriggerNum]);

				error = fc2SetTriggerMode(m_pContext, &triggerMode);
				if(error != FC2_ERROR_OK)
				{
					strMessage.Format(_T("Couldn't write 'TriggerMode' to the device"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}


				if(GetTriggerSource(&nTempTriggerNum))
				{
					strMessage.Format(_T("Couldn't read 'TriggerSource' from the database"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				triggerMode.source = nTempTriggerNum;

				error = fc2SetTriggerMode(m_pContext, &triggerMode);
				if(error != FC2_ERROR_OK)
				{
					strMessage.Format(_T("Couldn't write 'TriggerMode' to the device"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				if(GetTriggerParameter(&nTempTriggerNum))
				{
					strMessage.Format(_T("Couldn't read 'TriggerParameter' from the database"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				triggerMode.parameter = nTempTriggerNum;

				error = fc2SetTriggerMode(m_pContext, &triggerMode);
				if(error != FC2_ERROR_OK)
				{
					strMessage.Format(_T("Couldn't write 'TriggerMode' to the device"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				if(triggerModeInfo.polaritySupported)
				{
					if(GetTriggerPolarity(&nTempTriggerNum))
					{
						strMessage.Format(_T("Couldn't read 'TriggerPolarity' from the database"));
						eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
						break;
					}

					triggerMode.polarity = nTempTriggerNum;

					error = fc2SetTriggerMode(m_pContext, &triggerMode);
					if(error != FC2_ERROR_OK)
					{
						strMessage.Format(_T("Couldn't write 'TriggerMode' to the device"));
						eReturn = EDeviceInitializeResult_WriteToDeviceError;
						break;
					}
				}

				// Poll to ensure camera is ready
				bool retVal = PollForTriggerReady(m_pContext);
				if(!retVal)
				{
					strMessage.Format(_T("Couldn't write 'PollForTriggerReady' to the device"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}

				error = fc2WriteRegister(m_pContext, 0x62C, 0);
				if(error != FC2_ERROR_OK)
				{
					strMessage.Format(_T("Couldn't write 'Register' to the device"));
					eReturn = EDeviceInitializeResult_WriteToDeviceError;
					break;
				}
			}
		}

		EDevicePointGreyImagemode Imagemode;
		if(GetFormatMode(&Imagemode))
		{
			strMessage.Format(_T("Couldn't read 'FormatMode' from the database"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		error = fc2SetGigEImagingMode(m_pContext, (fc2Mode)Imagemode);
		if(error != FC2_ERROR_OK)
		{
			strMessage.Format(_T("Couldn't write 'FormatMode' to the device"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		fc2GigEImageSettings GigeImageSettingFormat;
		error = fc2GetGigEImageSettings(m_pContext, &GigeImageSettingFormat);
		if(error != FC2_ERROR_OK)
		{
			strMessage.Format(_T("Couldn't read 'GigEImageSettings' from the device"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}


		if(GetCanvasWidth(&nTempNum))
		{
			strMessage.Format(_T("Couldn't read 'CanvasWidth' from the database"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		GigeImageSettingFormat.width = nTempNum;

		error = fc2SetGigEImageSettings(m_pContext, &GigeImageSettingFormat);
		if(error != FC2_ERROR_OK)
		{
			strMessage.Format(_T("Couldn't write 'CanvasWidth' to the device"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		if(GetCanvasHeight(&nTempNum))
		{
			strMessage.Format(_T("Couldn't read 'CanvasHeight' from the database"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		GigeImageSettingFormat.height = nTempNum;

		error = fc2SetGigEImageSettings(m_pContext, &GigeImageSettingFormat);
		if(error != FC2_ERROR_OK)
		{
			strMessage.Format(_T("Couldn't write 'CanvasHeight' to the device"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		if(GetOffsetX(&nTempNum))
		{
			strMessage.Format(_T("Couldn't read 'OffsetX' from the database"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		GigeImageSettingFormat.offsetX = nTempNum;

		error = fc2SetGigEImageSettings(m_pContext, &GigeImageSettingFormat);
		if(error != FC2_ERROR_OK)
		{
			strMessage.Format(_T("Couldn't write 'OffsetX' to the device"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		if(GetOffsetY(&nTempNum))
		{
			strMessage.Format(_T("Couldn't read 'OffsetY' from the database"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		GigeImageSettingFormat.offsetY = nTempNum;

		error = fc2SetGigEImageSettings(m_pContext, &GigeImageSettingFormat);
		if(error != FC2_ERROR_OK)
		{
			strMessage.Format(_T("Couldn't write 'OffsetY' to the device"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		EDevicePointGreyPixelFormat PixelFormat;
		if(GetPixelFormat(&PixelFormat))
		{
			strMessage.Format(_T("Couldn't read 'PixelFormat' from the database"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(PixelFormat != EDevicePointGreyPixelFormat_MONO8 && PixelFormat != EDevicePointGreyPixelFormat_MONO16 && PixelFormat != EDevicePointGreyPixelFormat_RGB8)
		{
			strMessage.Format(_T("Couldn't support 'PixelFormat' to the device"));
			eReturn = EDeviceInitializeResult_NotSupportedDeviceError;
			break;
		}

		GigeImageSettingFormat.pixelFormat = (fc2PixelFormat)_ttoi(g_lpszPixelFormatValue[PixelFormat]);

		error = fc2SetGigEImageSettings(m_pContext, &GigeImageSettingFormat);
		if(error != FC2_ERROR_OK)
		{
			strMessage.Format(_T("Couldn't write 'PixelFormat' to the device"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		// Config Setting Start
		fc2Config Config;
		error = fc2GetConfiguration(m_pContext, &Config); //아래 셋팅값 Get으로 불러오도록 변경
		if(error != FC2_ERROR_OK)
		{
			strMessage.Format(_T("Couldn't read 'Configuration' from the device"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		if(GetGrabWaitingTime(&nTempNum))
		{
			strMessage.Format(_T("Couldn't read 'GrabWaitingTime' from the database"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		Config.grabTimeout = nTempNum;

		error = fc2SetConfiguration(m_pContext, &Config);
		if(error != FC2_ERROR_OK)
		{
			strMessage.Format(_T("Couldn't write 'GrabWaitingTime' to the device"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		EDevicePointGreyGrabMode GrabMode;
		if(GetGrabMode(&GrabMode))
		{
			strMessage.Format(_T("Couldn't read 'GrabMode' from the database"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		Config.grabMode = (fc2GrabMode)GrabMode;

		error = fc2SetConfiguration(m_pContext, &Config);
		if(error != FC2_ERROR_OK)
		{
			strMessage.Format(_T("Couldn't write 'GrabMode' to the device"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		if(GetGrabNumBuffers(&nTempNum))
		{
			strMessage.Format(_T("Couldn't read 'GrabNumBuffers' from the database"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		Config.numBuffers = nTempNum;

		error = fc2SetConfiguration(m_pContext, &Config);
		if(error != FC2_ERROR_OK)
		{
			strMessage.Format(_T("Couldn't write 'GrabNumBuffers' to the device"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		//GigEProperty Setting Start

		fc2GigEProperty GigEProperty;

		if(GetPacketSize(&nTempNum))
		{
			strMessage.Format(_T("Couldn't read 'PacketSize' from the database"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		GigEProperty.propType = PACKET_SIZE;
		error = fc2GetGigEProperty(m_pContext, &GigEProperty);
		if(error != FC2_ERROR_OK)
		{
			strMessage.Format(_T("Couldn't read 'PacketSize' from the device"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		GigEProperty.value = nTempNum;
		error = fc2SetGigEProperty(m_pContext, &GigEProperty);
		if(error != FC2_ERROR_OK)
		{
			strMessage.Format(_T("Couldn't write 'PacketSize' to the device"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		if(GetPacketDelay(&nTempNum))
		{
			strMessage.Format(_T("Couldn't read 'PacketDelay' from the database"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		GigEProperty.propType = PACKET_DELAY;
		error = fc2GetGigEProperty(m_pContext, &GigEProperty);
		if(error != FC2_ERROR_OK)
		{
			strMessage.Format(_T("Couldn't read 'PacketDelay' from the device"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}
		GigEProperty.value = nTempNum;
		error = fc2SetGigEProperty(m_pContext, &GigEProperty);
		if(error != FC2_ERROR_OK)
		{
			strMessage.Format(_T("Couldn't write 'PacketDelay' to the device"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		//fc2Property Setting
		fc2PropertyInfo PropertyInfo;
		fc2Property Property;

		for(unsigned int i = 0; i < 15; ++i)
		{
			if(i == 14) // FRAME_RATE
				i = 16;

			PropertyInfo.type = (fc2PropertyType)i;

			if(fc2GetPropertyInfo(m_pContext, &PropertyInfo) != FC2_ERROR_OK) // 입력을 못하는 경우도 있기때문에 확인을 해줘야함
			{
				strMessage.Format(_T("Couldn't read 'PropertyInfo' from the device"));
				eReturn = EDeviceInitializeResult_ReadOnDeviceError;
				break;
			}

			if(!PropertyInfo.present)
				continue;


			EDevicePointGreySwitch eParam;

			if(i == 0)
			{
				if(GetBrightnessManual(&eParam))
				{
					strMessage.Format(_T("Couldn't read 'BrightnessManual' from the database"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(GetBrightnessValue(&nTempNum))
				{
					strMessage.Format(_T("Couldn't read 'BrightnessManual' from the database"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
			}
			else if(i == 1)
			{
				if(GetExposureManual(&eParam))
				{
					strMessage.Format(_T("Couldn't read 'ExposureManual' from the database"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(GetExposureValue(&nTempNum))
				{
					strMessage.Format(_T("Couldn't read 'ExposureValue' from the database"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
			}
			else if(i == 2)
			{
				if(GetSharpnessManual(&eParam))
				{
					strMessage.Format(_T("Couldn't read 'SharpnessManual' from the database"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(GetSharpnessValue(&nTempNum))
				{
					strMessage.Format(_T("Couldn't read 'SharpnessValue' from the database"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
			}
			else if(i == 3)
			{
				if(GetWhiteBalanceManual(&eParam))
				{
					strMessage.Format(_T("Couldn't read 'WhiteBalanceManual' from the database"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(GetWhiteBalanceValue(&nTempNum))
				{
					strMessage.Format(_T("Couldn't read 'WhiteBalanceValue' from the database"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
			}
			else if(i == 4)
			{
				if(GetHueManual(&eParam))
				{
					strMessage.Format(_T("Couldn't read 'HueManual' from the database"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(GetHueValue(&nTempNum))
				{
					strMessage.Format(_T("Couldn't read 'HueValue' from the database"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
			}
			else if(i == 5)
			{
				if(GetSaturationManual(&eParam))
				{
					strMessage.Format(_T("Couldn't read 'SaturationManual' from the database"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(GetSaturationValue(&nTempNum))
				{
					strMessage.Format(_T("Couldn't read 'SaturationValue' from the database"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
			}
			else if(i == 6)
			{
				if(GetGammaManual(&eParam))
				{
					strMessage.Format(_T("Couldn't read 'GammaManual' from the database"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(GetGammaValue(&nTempNum))
				{
					strMessage.Format(_T("Couldn't read 'GammaValue' from the database"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
			}
			else if(i == 7)
			{
				if(GetIrisManual(&eParam))
				{
					strMessage.Format(_T("Couldn't read 'IrisManual' from the database"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(GetIrisValue(&nTempNum))
				{
					strMessage.Format(_T("Couldn't read 'IrisValue' from the database"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
			}
			else if(i == 8)
			{
				if(GetFocusManual(&eParam))
				{
					strMessage.Format(_T("Couldn't read 'FocusManual' from the database"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(GetFocusValue(&nTempNum))
				{
					strMessage.Format(_T("Couldn't read 'FocusValue' from the database"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
			}
			else if(i == 9)
			{
				if(GetZoomManual(&eParam))
				{
					strMessage.Format(_T("Couldn't read 'ZoomManual' from the database"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(GetZoomValue(&nTempNum))
				{
					strMessage.Format(_T("Couldn't read 'ZoomValue' from the database"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
			}
			else if(i == 10)
			{
				if(GetPanManual(&eParam))
				{
					strMessage.Format(_T("Couldn't read 'PanManual' from the database"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(GetPanValue(&nTempNum))
				{
					strMessage.Format(_T("Couldn't read 'PanValue' from the database"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
			}
			else if(i == 11)
			{
				if(GetTiltManual(&eParam))
				{
					strMessage.Format(_T("Couldn't read 'TiltManual' from the database"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(GetTiltValue(&nTempNum))
				{
					strMessage.Format(_T("Couldn't read 'TiltValue' from the database"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
			}
			else if(i == 12)
			{
				if(GetShutterManual(&eParam))
				{
					strMessage.Format(_T("Couldn't read 'ShutterManual' from the database"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(GetShutterValue(&nTempNum))
				{
					strMessage.Format(_T("Couldn't read 'ShutterValue' from the database"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
			}
			else if(i == 13)
			{
				if(GetGainManual(&eParam))
				{
					strMessage.Format(_T("Couldn't read 'GainManual' from the database"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(GetGainValue(&nTempNum))
				{
					strMessage.Format(_T("Couldn't read 'GainValue' from the database"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
			}
			else if(i == 16)
			{
				if(GetFrameRateManual(&eParam))
				{
					strMessage.Format(_T("Couldn't read 'FrameRateManual' from the database"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}

				if(GetFrameRateValue(&nTempNum))
				{
					strMessage.Format(_T("Couldn't read 'FrameRateValue' from the database"));
					eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
					break;
				}
			}


			Property.type = (fc2PropertyType)i;
			if(fc2GetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				strMessage.Format(_T("Couldn't read 'Property' from the device"));
				eReturn = EDeviceInitializeResult_ReadOnDeviceError;
				break;
			}

			switch(eParam)
			{
			case EDevicePointGreySwitch_CustomValue:
				Property.absControl = false;
				if(PropertyInfo.onOffSupported)
					Property.onOff = true;
				Property.autoManualMode = false;
				Property.onePush = false;
				Property.valueA = nTempNum;
				break;
			case EDevicePointGreySwitch_Auto:
				Property.absControl = false;
				Property.onOff = false;
				if(i == 16)
					Property.onOff = true;
				if(PropertyInfo.autoSupported)
					Property.autoManualMode = true;
				Property.onePush = false;
				break;
			case EDevicePointGreySwitch_OnePush:
				Property.absControl = false;
				if(PropertyInfo.onOffSupported)
					Property.onOff = true;
				Property.autoManualMode = false;
				if(PropertyInfo.onePushSupported)
					Property.onePush = true;
				break;
			default:
				break;
			}

			if(!PropertyInfo.readOutSupported || (fc2SetProperty(m_pContext, &Property) != FC2_ERROR_OK))
			{
				strMessage.Format(_T("Couldn't write 'Property' to the device"));
				eReturn = EDeviceInitializeResult_WriteToDeviceError;
				break;
			}
		}



		SetUpdateParameter(); // StartCapture와 위치조정 후 테스트

		error = fc2StartCapture(m_pContext);
		if(error != FC2_ERROR_OK)
		{
			strMessage.Format(_T("Failed to start capture"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		if(m_bTriggerOnOff)
		{
			const unsigned int k_triggerInq = 0x530;

			fc2Error error;
			unsigned int regVal = 0;

			error = fc2ReadRegister(m_pContext, k_triggerInq, &regVal);

			if(error != FC2_ERROR_OK)
			{
				strMessage.Format(_T("Couldn't write 'CheckSoftwareTriggerPresence' to the device"));
				eReturn = EDeviceInitializeResult_ReadOnDeviceError;
				break;
			}

			if((regVal & 0x10000) != 0x10000)
			{
				strMessage.Format(_T("Failed to execute 'Trigger On' to the device"));
				eReturn = EDeviceInitializeResult_ReadOnDeviceError;
				break;
			}
		}

		int nDepth = 0;
		if(PixelFormat == EDevicePointGreyPixelFormat_MONO8)
		{
			m_bColorCamera = false;
			nDepth = 8;
		}
		else if(PixelFormat == EDevicePointGreyPixelFormat_MONO16)
		{
			m_bColorCamera = false;
			nDepth = 16;
		}
		else if(PixelFormat == EDevicePointGreyPixelFormat_RGB8)
		{
			m_bColorCamera = true;
			nDepth = 8;
		}

		int nChannel = m_bColorCamera ? 3 : 1;

		int nMaxValue = (1 << nDepth) - 1;

		CMultipleVariable mv;
		for(int i = 0; i < nChannel; ++i)
			mv.AddValue(nMaxValue);

		this->InitBuffer(GigeImageSettingFormat.width, GigeImageSettingFormat.height, mv, CRavidImage::MakeValueFormat(m_bColorCamera ? 3 : 1, nDepth));
		this->ConnectImage();

		strStatus = CMultiLanguageManager::GetString(ELanguageParameter_Initialized);

		strMessage.Format(_T("Succeeded to initialize Device."));

		m_bIsInitialized = true;

		eReturn = EDeviceInitializeResult_OK;

		CEventHandlerManager::BroadcastOnDeviceInitialized(this);
	}
	while(false);

	if(!IsInitialized())
	{
		fc2Error error = fc2DestroyContext(m_pContext);
		m_pContext = nullptr;
		Terminate();
	}

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	SetStatus(strStatus);

	return eReturn;
}

EDeviceTerminateResult CDevicePointGrey::Terminate()
{
	EDeviceTerminateResult eReturn = EDeviceTerminateResult_UnknownError;

	SetStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	CString strMessage;
	strMessage.Format(_T(""));
	
	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("PointGrey"));

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

		// Disconnect the camera
		if(m_pContext)
		{
			fc2Error error = FC2_ERROR_UNDEFINED;
			if(fc2IsConnected(m_pContext))
				error = fc2Disconnect(m_pContext);

			error = fc2DestroyContext(m_pContext);

			m_pContext = nullptr;
		}

		eReturn = EDeviceTerminateResult_OK;

		strMessage = CMultiLanguageManager::GetString(ELanguageParameter_TheDevicehasbeensuccessfullyterminated);

		CEventHandlerManager::BroadcastOnDeviceTerminated(this);
	}
	while(false);

	this->ClearBuffer();

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}


bool CDevicePointGrey::LoadSettings()
{
	bool bReturn = false;

	m_vctParameterFieldConfigurations.clear();

	do
	{
		CString strTime;
		strTime.Format(_T("%d"), INT_MAX);

		AddParameterFieldConfigurations(EDeviceParameterPointGrey_DeviceID, g_lpszDevicePointGrey[EDeviceParameterPointGrey_DeviceID], _T("0"), EParameterFieldType_Edit);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_VideoModes, g_lpszDevicePointGrey[EDeviceParameterPointGrey_VideoModes], _T("1"), EParameterFieldType_None);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_GrabWaitingTime, g_lpszDevicePointGrey[EDeviceParameterPointGrey_GrabWaitingTime], _T("1"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_GrabMode, g_lpszDevicePointGrey[EDeviceParameterPointGrey_GrabMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrabMode, EDevicePointGreyGrabMode_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_GrabNumBuffers, g_lpszDevicePointGrey[EDeviceParameterPointGrey_GrabNumBuffers], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_CanvasWidth, g_lpszDevicePointGrey[EDeviceParameterPointGrey_CanvasWidth], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_CanvasHeight, g_lpszDevicePointGrey[EDeviceParameterPointGrey_CanvasHeight], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_OffsetX, g_lpszDevicePointGrey[EDeviceParameterPointGrey_OffsetX], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_OffsetY, g_lpszDevicePointGrey[EDeviceParameterPointGrey_OffsetY], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_PixelFormat, g_lpszDevicePointGrey[EDeviceParameterPointGrey_PixelFormat], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszPixelFormat, EDevicePointGreyPixelFormat_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_FormatMode, g_lpszDevicePointGrey[EDeviceParameterPointGrey_FormatMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszImagemode, EDevicePointGreyImagemode_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_CameraSetting, g_lpszDevicePointGrey[EDeviceParameterPointGrey_CameraSetting], _T("1"), EParameterFieldType_None);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_GrabCount, g_lpszDevicePointGrey[EDeviceParameterPointGrey_GrabCount], _T("1"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_PacketSize, g_lpszDevicePointGrey[EDeviceParameterPointGrey_PacketSize], _T("1"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_PacketDelay, g_lpszDevicePointGrey[EDeviceParameterPointGrey_PacketDelay], _T("1"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_BrightnessSetting, g_lpszDevicePointGrey[EDeviceParameterPointGrey_BrightnessSetting], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_BrightnessManual, g_lpszDevicePointGrey[EDeviceParameterPointGrey_BrightnessManual], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszSwitch, EDevicePointGreySwitch_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_BrightnessValue, g_lpszDevicePointGrey[EDeviceParameterPointGrey_BrightnessValue], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_ExposureSetting, g_lpszDevicePointGrey[EDeviceParameterPointGrey_ExposureSetting], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_ExposureManual, g_lpszDevicePointGrey[EDeviceParameterPointGrey_ExposureManual], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszSwitch, EDevicePointGreySwitch_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_ExposureValue, g_lpszDevicePointGrey[EDeviceParameterPointGrey_ExposureValue], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_SharpnessSetting, g_lpszDevicePointGrey[EDeviceParameterPointGrey_SharpnessSetting], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_SharpnessManual, g_lpszDevicePointGrey[EDeviceParameterPointGrey_SharpnessManual], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszSwitch, EDevicePointGreySwitch_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_SharpnessValue, g_lpszDevicePointGrey[EDeviceParameterPointGrey_SharpnessValue], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_WhiteBalanceSetting, g_lpszDevicePointGrey[EDeviceParameterPointGrey_WhiteBalanceSetting], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_WhiteBalanceManual, g_lpszDevicePointGrey[EDeviceParameterPointGrey_WhiteBalanceManual], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszSwitch, EDevicePointGreySwitch_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_WhiteBalanceValue, g_lpszDevicePointGrey[EDeviceParameterPointGrey_WhiteBalanceValue], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_HueSetting, g_lpszDevicePointGrey[EDeviceParameterPointGrey_HueSetting], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_HueManual, g_lpszDevicePointGrey[EDeviceParameterPointGrey_HueManual], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszSwitch, EDevicePointGreySwitch_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_HueValue, g_lpszDevicePointGrey[EDeviceParameterPointGrey_HueValue], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_SaturationSetting, g_lpszDevicePointGrey[EDeviceParameterPointGrey_SaturationSetting], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_SaturationManual, g_lpszDevicePointGrey[EDeviceParameterPointGrey_SaturationManual], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszSwitch, EDevicePointGreySwitch_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_SaturationValue, g_lpszDevicePointGrey[EDeviceParameterPointGrey_SaturationValue], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_GammaSetting, g_lpszDevicePointGrey[EDeviceParameterPointGrey_GammaSetting], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_GammaManual, g_lpszDevicePointGrey[EDeviceParameterPointGrey_GammaManual], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszSwitch, EDevicePointGreySwitch_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_GammaValue, g_lpszDevicePointGrey[EDeviceParameterPointGrey_GammaValue], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_IrisSetting, g_lpszDevicePointGrey[EDeviceParameterPointGrey_IrisSetting], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_IrisManual, g_lpszDevicePointGrey[EDeviceParameterPointGrey_IrisManual], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszSwitch, EDevicePointGreySwitch_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_IrisValue, g_lpszDevicePointGrey[EDeviceParameterPointGrey_IrisValue], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_FocusSetting, g_lpszDevicePointGrey[EDeviceParameterPointGrey_FocusSetting], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_FocusManual, g_lpszDevicePointGrey[EDeviceParameterPointGrey_FocusManual], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszSwitch, EDevicePointGreySwitch_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_FocusValue, g_lpszDevicePointGrey[EDeviceParameterPointGrey_FocusValue], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_ZoomSetting, g_lpszDevicePointGrey[EDeviceParameterPointGrey_ZoomSetting], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_ZoomManual, g_lpszDevicePointGrey[EDeviceParameterPointGrey_ZoomManual], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszSwitch, EDevicePointGreySwitch_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_ZoomValue, g_lpszDevicePointGrey[EDeviceParameterPointGrey_ZoomValue], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_PanSetting, g_lpszDevicePointGrey[EDeviceParameterPointGrey_PanSetting], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_PanManual, g_lpszDevicePointGrey[EDeviceParameterPointGrey_PanManual], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszSwitch, EDevicePointGreySwitch_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_PanValue, g_lpszDevicePointGrey[EDeviceParameterPointGrey_PanValue], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_TiltSetting, g_lpszDevicePointGrey[EDeviceParameterPointGrey_TiltSetting], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_TiltManual, g_lpszDevicePointGrey[EDeviceParameterPointGrey_TiltManual], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszSwitch, EDevicePointGreySwitch_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_TiltValue, g_lpszDevicePointGrey[EDeviceParameterPointGrey_TiltValue], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_ShutterSetting, g_lpszDevicePointGrey[EDeviceParameterPointGrey_ShutterSetting], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_ShutterManual, g_lpszDevicePointGrey[EDeviceParameterPointGrey_ShutterManual], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszSwitch, EDevicePointGreySwitch_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_ShutterValue, g_lpszDevicePointGrey[EDeviceParameterPointGrey_ShutterValue], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_GainSetting, g_lpszDevicePointGrey[EDeviceParameterPointGrey_GainSetting], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_GainManual, g_lpszDevicePointGrey[EDeviceParameterPointGrey_GainManual], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszSwitch, EDevicePointGreySwitch_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_GainValue, g_lpszDevicePointGrey[EDeviceParameterPointGrey_GainValue], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_FrameRateSetting, g_lpszDevicePointGrey[EDeviceParameterPointGrey_FrameRateSetting], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_FrameRateManual, g_lpszDevicePointGrey[EDeviceParameterPointGrey_FrameRateManual], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszSwitch, EDevicePointGreySwitch_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_FrameRateValue, g_lpszDevicePointGrey[EDeviceParameterPointGrey_FrameRateValue], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_TriggerSetting, g_lpszDevicePointGrey[EDeviceParameterPointGrey_TriggerSetting], _T("1"), EParameterFieldType_None);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_TriggerOnOff, g_lpszDevicePointGrey[EDeviceParameterPointGrey_TriggerOnOff], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 1);
		//AddParameterFieldConfigurations(EDeviceParameterPointGrey_TriggerMode, g_lpszDevicePointGrey[EDeviceParameterPointGrey_TriggerMode], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_TriggerMode, g_lpszDevicePointGrey[EDeviceParameterPointGrey_TriggerMode], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszTriggerMode, EDevicePointGreyTriggerMode_Count), nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_TriggerSource, g_lpszDevicePointGrey[EDeviceParameterPointGrey_TriggerSource], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_TriggerParameter, g_lpszDevicePointGrey[EDeviceParameterPointGrey_TriggerParameter], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterPointGrey_TriggerPolarity, g_lpszDevicePointGrey[EDeviceParameterPointGrey_TriggerPolarity], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);


		bReturn = true;
	}
	while(false);

	return bReturn & __super::LoadSettings();
}

EDeviceGrabResult CDevicePointGrey::Grab() // Image가 들어올때가아닌 grab신호가 들어갔을때 callback 호출하도록 변경
{
	EDeviceGrabResult eReturn = EDeviceGrabResult_UnknownError;

	do
	{
		//fc2Property TestProp;
		//TestProp.type = FC2_BRIGHTNESS;
		//if(fc2GetProperty(m_pContext, &TestProp) != FC2_ERROR_OK)
		//	break;
		//CLogManager::Write(0, _T("%d"), TestProp.valueA);

		m_nGrabCount = 0;

		if(!IsInitialized())
		{
			eReturn = EDeviceGrabResult_NotInitializedError;
			break;
		}

		if(IsLive() || !IsGrabAvailable())
		{
			eReturn = EDeviceGrabResult_AlreadyGrabError;
			break;
		}

		m_bIsGrabAvailable = false;

		m_pLiveThread = AfxBeginThread(CDevicePointGrey::CallbackFunction, this, 0, 0U, CREATE_SUSPENDED);

		if(!m_pLiveThread)
		{
			//strMessage.Format(_T("Failed to create thread"));
			eReturn = EDeviceGrabResult_CreateThreadError;
			break;
		}

		m_pLiveThread->ResumeThread();

		eReturn = EDeviceGrabResult_OK;
	}
	while(false);

	return eReturn;
}

EDeviceLiveResult CDevicePointGrey::Live()
{
	EDeviceLiveResult eReturn = EDeviceLiveResult_UnknownError;

	CString strMessage;

	do
	{
		if(!IsInitialized())
		{
			strMessage.Format(_T("Failed to initialize the device"));
			eReturn = EDeviceLiveResult_NotInitializedError;
			break;
		}
		if(IsLive() || !IsGrabAvailable())
		{
			strMessage.Format(_T("The device is already grab"));
			eReturn = EDeviceLiveResult_AlreadyGrabError;
			break;
		}

		if(m_bTriggerOnOff)
		{
			strMessage.Format(_T("The device's trigger mode is on."));
			eReturn = EDeviceLiveResult_ReadOnDeviceError;
			break;
		}

		m_bIsLive = true;

		m_pLiveThread = AfxBeginThread(CDevicePointGrey::CallbackFunction, this, 0, 0U, CREATE_SUSPENDED);

		if(!m_pLiveThread)
		{
			strMessage.Format(_T("Failed to create thread"));
			eReturn = EDeviceLiveResult_CreateThreadError;
			break;
		}

		m_pLiveThread->ResumeThread();

		strMessage.Format(_T("Succeeded to execute command 'Live'"));

		eReturn = EDeviceLiveResult_OK;
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceStopResult CDevicePointGrey::Stop()
{
	EDeviceStopResult eReturn = EDeviceStopResult_UnknownError;

	CString strMessage;

	do
	{
		if(!IsInitialized())
		{
			strMessage.Format(_T("Failed to initialize the device."));
			eReturn = EDeviceStopResult_NotInitializedError;
			break;
		}
		if(!IsLive() && IsGrabAvailable())
		{
			strMessage.Format(_T("The device was already stop."));
			eReturn = EDeviceStopResult_AlreadyStopError;
			break;
		}

		//bGrabTest = false;

		m_bIsLive = false;

		//if(WaitForSingleObject(m_pLiveThread->m_hThread, 1000) == WAIT_TIMEOUT)
		//{
		//	fc2Error error = fc2StopCapture(m_pContext);
		//	if(error != FC2_ERROR_OK)
		//	{
		//		fc2DestroyContext(m_pContext);

		//		strMessage.Format(_T("Device didn't stop capturing.")); //
		//		eReturn = EDeviceStopResult_WriteToDeviceError;
		//		break;
		//	}

		//	error = fc2DestroyContext(m_pContext);
		//	if(error != FC2_ERROR_OK)
		//	{
		//		strMessage.Format(_T("Device didn't destroy m_pContext.")); //
		//		eReturn = EDeviceStopResult_WriteToDeviceError;
		//		break;
		//	}

		//			//if(PylonDeviceClose(m_hDevice) != S_OK)
		//			//{
		//			//	strMessage.Format(_T("Device didn't close.")); //
		//			//	eReturn = EDeviceStopResult_WriteToDeviceError;
		//			//	break;
		//			//}

		//			//if(PylonDeviceOpen(m_hDevice, PYLONC_ACCESS_MODE_CONTROL | PYLONC_ACCESS_MODE_STREAM) != S_OK)
		//			//{
		//			//	strMessage.Format(_T("Device didn't open.")); //
		//			//	eReturn = EDeviceStopResult_WriteToDeviceError;
		//			//	break;
		//			//}
		//}

		m_bIsGrabAvailable = true;
		m_pLiveThread = nullptr;



		strMessage.Format(_T("Succeeded to execute command 'Stop'"));

		eReturn = EDeviceStopResult_OK;
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceTriggerResult CDevicePointGrey::Trigger()
{
	EDeviceTriggerResult eReturn = EDeviceTriggerResult_UnknownError;

	CString strMessage;

	do
	{
		if(!IsInitialized())
		{
			strMessage.Format(_T("Failed to initialize the device."));

			eReturn = EDeviceTriggerResult_NotInitializedError;

			break;
		}

		if(IsGrabAvailable() || IsLive())
			break;
		// Fire software trigger
		const unsigned int k_softwareTrigger = 0x62C;
		const unsigned int k_fireVal = 0x80000000;

		fc2Error error = fc2WriteRegister(m_pContext, k_softwareTrigger, k_fireVal);
		if(error != FC2_ERROR_OK)
		{
			break;
		}

		strMessage.Format(_T("Succeeded to execute command 'Trigger'"));

		eReturn = EDeviceTriggerResult_OK;
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDevicePointGrey::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = false;

	bool bFoundID = true;

	do
	{
		switch(nParam)
		{
		case EDeviceParameterBaslerUSB_DeviceID:
			{
				bReturn = !SetDeviceID(_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_GrabWaitingTime:
			{
				bReturn = !SetGrabWaitingTime(_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_GrabMode:
			{
				bReturn = !SetGrabMode((EDevicePointGreyGrabMode)_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_GrabNumBuffers:
			{
				bReturn = !SetGrabNumBuffers(_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_CanvasWidth:
			{
				bReturn = !SetCanvasWidth(_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_CanvasHeight:
			{
				bReturn = !SetCanvasHeight(_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_OffsetX:
			{
				bReturn = !SetOffsetX(_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_OffsetY:
			{
				bReturn = !SetOffsetY(_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_PixelFormat:
			{
				bReturn = !SetPixelFormat((EDevicePointGreyPixelFormat)_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_FormatMode:
			{
				bReturn = !SetFormatMode((EDevicePointGreyImagemode)_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_GrabCount:
			{
				bReturn = !SetGrabCount(_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_PacketSize:
			{
				bReturn = !SetPacketSize(_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_PacketDelay:
			{
				bReturn = !SetPacketDelay(_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_BrightnessManual:
			{
				bReturn = !SetBrightnessManual((EDevicePointGreySwitch)_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_BrightnessValue:
			{
				bReturn = !SetBrightnessValue(_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_ExposureManual:
			{
				bReturn = !SetExposureManual((EDevicePointGreySwitch)_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_ExposureValue:
			{
				bReturn = !SetExposureValue(_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_SharpnessManual:
			{
				bReturn = !SetSharpnessManual((EDevicePointGreySwitch)_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_SharpnessValue:
			{
				bReturn = !SetSharpnessValue(_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_WhiteBalanceManual:
			{
				bReturn = !SetWhiteBalanceManual((EDevicePointGreySwitch)_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_WhiteBalanceValue:
			{
				bReturn = !SetWhiteBalanceValue(_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_HueManual:
			{
				bReturn = !SetHueManual((EDevicePointGreySwitch)_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_HueValue:
			{
				bReturn = !SetHueValue(_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_SaturationManual:
			{
				bReturn = !SetSaturationManual((EDevicePointGreySwitch)_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_SaturationValue:
			{
				bReturn = !SetSaturationValue(_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_GammaManual:
			{
				bReturn = !SetGammaManual((EDevicePointGreySwitch)_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_GammaValue:
			{
				bReturn = !SetGammaValue(_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_IrisManual:
			{
				bReturn = !SetIrisManual((EDevicePointGreySwitch)_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_IrisValue:
			{
				bReturn = !SetIrisValue(_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_FocusManual:
			{
				bReturn = !SetFocusManual((EDevicePointGreySwitch)_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_FocusValue:
			{
				bReturn = !SetFocusValue(_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_ZoomManual:
			{
				bReturn = !SetZoomManual((EDevicePointGreySwitch)_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_ZoomValue:
			{
				bReturn = !SetZoomValue(_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_PanManual:
			{
				bReturn = !SetPanManual((EDevicePointGreySwitch)_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_PanValue:
			{
				bReturn = !SetPanValue(_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_TiltManual:
			{
				bReturn = !SetTiltManual((EDevicePointGreySwitch)_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_TiltValue:
			{
				bReturn = !SetTiltValue(_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_ShutterManual:
			{
				bReturn = !SetShutterManual((EDevicePointGreySwitch)_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_ShutterValue:
			{
				bReturn = !SetShutterValue(_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_GainManual:
			{
				bReturn = !SetGainManual((EDevicePointGreySwitch)_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_GainValue:
			{
				bReturn = !SetGainValue(_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_FrameRateManual:
			{
				bReturn = !SetFrameRateManual((EDevicePointGreySwitch)_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_FrameRateValue:
			{
				bReturn = !SetFrameRateValue(_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_TriggerOnOff:
			{
				bReturn = !SetTriggerOnOff(_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_TriggerMode:
			{
				bReturn = !SetTriggerMode((EDevicePointGreyTriggerMode)_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_TriggerSource:
			{
				bReturn = !SetTriggerSource(_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_TriggerParameter:
			{
				bReturn = !SetTriggerParameter(_ttoi(strValue));
			}
			break;
		case EDeviceParameterPointGrey_TriggerPolarity:
			{
				bReturn = !SetTriggerPolarity((EDevicePointGreyTriggerMode)_ttoi(strValue));
			}
			break;
		default:
			{
				bFoundID = false;
			}
			break;
		}
	}
	while(false);

	if(!bFoundID)
	{
		CString strMessage;

		strMessage.Format(_T("Failed to find the parameter"));

		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);
	}	

	return bReturn;
}

// bool CDevicePointGrey::OnParameterChanged(CString strParam, CString strValue)
// {
// 	bool bReturn = false;
// 
// 	do
// 	{
// 		SetMessage(_T(""));
// 		
// 		if(!strParam.CompareNoCase(_T("Device ID")))
// 			bReturn = !SetDeviceID(_ttoi(strValue));
// 		else if(!strParam.CompareNoCase(_T("Grab count")))
// 			bReturn = !SetGrabCount(_ttoi(strValue));
// 		else if(!strParam.CompareNoCase(_T("Packet Size")))
// 			bReturn = !SetPacketSize(_ttoi(strValue));
// 		else if(!strParam.CompareNoCase(_T("Packet Delay")))
// 			bReturn = !SetPacketDelay(_ttoi(strValue));
// 		else if(!strParam.CompareNoCase(_T("Grab Waiting Time[ms]")))
// 			bReturn = !SetGrabWaitingTime(_ttoi(strValue));
// 		else if(!strParam.CompareNoCase(_T("Grab Mode")))
// 			bReturn = !SetGrabMode((EDevicePointGreyGrabMode)_ttoi(strValue));
// 		else if(!strParam.CompareNoCase(_T("GrabNumBuffers")))
// 			bReturn = !SetGrabNumBuffers(_ttoi(strValue));
// 
// 		else if(!strParam.CompareNoCase(_T("Canvas Width")))
// 			bReturn = !SetCanvasWidth(_ttoi(strValue));
// 		else if(!strParam.CompareNoCase(_T("Canvas Height")))
// 			bReturn = !SetCanvasHeight(_ttoi(strValue));
// 		else if(!strParam.CompareNoCase(_T("Offset X")))
// 			bReturn = !SetOffsetX(_ttoi(strValue));
// 		else if(!strParam.CompareNoCase(_T("Offset Y")))
// 			bReturn = !SetOffsetY(_ttoi(strValue));
// 		//else if(!strParam.CompareNoCase(_T("Custom FrameRate")))
// 		//	bReturn = !SetCustomFrameRate(_ttoi(strValue));
// 
// 		//else if(!strParam.CompareNoCase(_T("Frame Rate")))
// 		//	bReturn = !SetFrameRate((EDevicePointGreyFrameRate)_ttoi(strValue));
// 		else if(!strParam.CompareNoCase(_T("PixelFormat")))
// 			bReturn = !SetPixelFormat((EDevicePointGreyPixelFormat)_ttoi(strValue));
// 		else if(!strParam.CompareNoCase(_T("Format Mode")))
// 			bReturn = !SetFormatMode((EDevicePointGreyImagemode)_ttoi(strValue));
// 
// 		//else if(!strParam.CompareNoCase(_T("Absolute Mode")))
// 		//	bReturn = !SetAbsoluteMode(_ttoi(strValue));
// 
// 		else if(!strParam.CompareNoCase(_T("Brightness Value")))
// 			bReturn = !SetBrightnessValue(_ttoi(strValue));
// 		else if(!strParam.CompareNoCase(_T("Brightness Manual")))
// 			bReturn = !SetBrightnessManual((EDevicePointGreySwitch)_ttoi(strValue));
// 
// 		else if(!strParam.CompareNoCase(_T("Exposure Value")))
// 			bReturn = !SetExposureValue(_ttoi(strValue));
// 		else if(!strParam.CompareNoCase(_T("Exposure Manual")))
// 			bReturn = !SetExposureManual((EDevicePointGreySwitch)_ttoi(strValue));
// 
// 		else if(!strParam.CompareNoCase(_T("Sharpness Value")))
// 			bReturn = !SetSharpnessValue(_ttoi(strValue));
// 		else if(!strParam.CompareNoCase(_T("Sharpness Manual")))
// 			bReturn = !SetSharpnessManual((EDevicePointGreySwitch)_ttoi(strValue));
// 
// 		else if(!strParam.CompareNoCase(_T("WhiteBalance Value")))
// 			bReturn = !SetWhiteBalanceValue(_ttoi(strValue));
// 		else if(!strParam.CompareNoCase(_T("WhiteBalance Manual")))
// 			bReturn = !SetWhiteBalanceManual((EDevicePointGreySwitch)_ttoi(strValue));
// 
// 		else if(!strParam.CompareNoCase(_T("Hue Value")))
// 			bReturn = !SetHueValue(_ttoi(strValue));
// 		else if(!strParam.CompareNoCase(_T("Hue Manual")))
// 			bReturn = !SetHueManual((EDevicePointGreySwitch)_ttoi(strValue));
// 
// 		else if(!strParam.CompareNoCase(_T("Saturation Value")))
// 			bReturn = !SetSaturationValue(_ttoi(strValue));
// 		else if(!strParam.CompareNoCase(_T("Saturation Manual")))
// 			bReturn = !SetSaturationManual((EDevicePointGreySwitch)_ttoi(strValue));
// 
// 		else if(!strParam.CompareNoCase(_T("Gamma Value")))
// 			bReturn = !SetGammaValue(_ttoi(strValue));
// 		else if(!strParam.CompareNoCase(_T("Gamma Manual")))
// 			bReturn = !SetGammaManual((EDevicePointGreySwitch)_ttoi(strValue));
// 
// 		else if(!strParam.CompareNoCase(_T("Iris Value")))
// 			bReturn = !SetIrisValue(_ttoi(strValue));
// 		else if(!strParam.CompareNoCase(_T("Iris Manual")))
// 			bReturn = !SetIrisManual((EDevicePointGreySwitch)_ttoi(strValue));
// 
// 		else if(!strParam.CompareNoCase(_T("Focus Value")))
// 			bReturn = !SetFocusValue(_ttoi(strValue));
// 		else if(!strParam.CompareNoCase(_T("Focus Manual")))
// 			bReturn = !SetFocusManual((EDevicePointGreySwitch)_ttoi(strValue));
// 
// 		else if(!strParam.CompareNoCase(_T("Zoom Value")))
// 			bReturn = !SetZoomValue(_ttoi(strValue));
// 		else if(!strParam.CompareNoCase(_T("Zoom Manual")))
// 			bReturn = !SetZoomManual((EDevicePointGreySwitch)_ttoi(strValue));
// 
// 		else if(!strParam.CompareNoCase(_T("Pan Value")))
// 			bReturn = !SetPanValue(_ttoi(strValue));
// 		else if(!strParam.CompareNoCase(_T("Pan Manual")))
// 			bReturn = !SetPanManual((EDevicePointGreySwitch)_ttoi(strValue));
// 
// 		else if(!strParam.CompareNoCase(_T("Tilt Value")))
// 			bReturn = !SetTiltValue(_ttoi(strValue));
// 		else if(!strParam.CompareNoCase(_T("Tilt Manual")))
// 			bReturn = !SetTiltManual((EDevicePointGreySwitch)_ttoi(strValue));
// 
// 		else if(!strParam.CompareNoCase(_T("Shutter Value")))
// 			bReturn = !SetShutterValue(_ttoi(strValue));
// 		else if(!strParam.CompareNoCase(_T("Shutter Manual")))
// 			bReturn = !SetShutterManual((EDevicePointGreySwitch)_ttoi(strValue));
// 
// 		else if(!strParam.CompareNoCase(_T("Gain Value")))
// 			bReturn = !SetGainValue(_ttoi(strValue));
// 		else if(!strParam.CompareNoCase(_T("Gain Manual")))
// 			bReturn = !SetGainManual((EDevicePointGreySwitch)_ttoi(strValue));
// 
// 		else if (!strParam.CompareNoCase(_T("Frame Rate Value")))
// 			bReturn = !SetFrameRateValue(_ttoi(strValue));
// 		else if (!strParam.CompareNoCase(_T("Frame Rate Manual")))
// 			bReturn = !SetFrameRateManual((EDevicePointGreySwitch)_ttoi(strValue));
// 
// 
// 		else if (!strParam.CompareNoCase(_T("Trigger On/Off")))
// 			bReturn = !SetTriggerOnOff(_ttoi(strValue));
// 		else if(!strParam.CompareNoCase(_T("Trigger Mode")))
// 			bReturn = !SetTriggerMode(_ttoi(strValue));
// 		else if(!strParam.CompareNoCase(_T("Trigger Source")))
// 			bReturn = !SetTriggerSource(_ttoi(strValue));
// 		else if(!strParam.CompareNoCase(_T("Trigger Parameter")))
// 			bReturn = !SetTriggerParameter(_ttoi(strValue));
// 		else if(!strParam.CompareNoCase(_T("Trigger Polarity")))
// 			bReturn = !SetTriggerPolarity(_ttoi(strValue));		
// 		else
// 		{
// 			SetMessage(_T("Failed to find the parameter."));
// 			break;
// 		}
// 
// 		if(!bReturn)
// 		{
// 			SetMessage(_T("Failed to change the parameter."));
// 			break;
// 		}
// 
// 		bReturn = SaveSettings();
// 
// 		if(!bReturn)
// 		{
// 			SetMessage(_T("Failed to save the parameter."));
// 			break;
// 		}
// 	}
// 	while(false);
// 
// 	return bReturn;
// }
//bool CDevicePointGrey::OnParameterChanged(CString strParam, CString strValue)
//{
//	bool bReturn = false;
//
//	do
//	{
//		SetMessage(_T(""));
//
//		if(0)
//		{
//			SetMessage(_T("Failed to find the parameter."));
//			break;
//		}
//
//		if(!bReturn)
//		{
//			SetMessage(_T("Failed to change the parameter."));
//			break;
//		}
//
//		bReturn = SaveSettings();
//
//		if(!bReturn)
//		{
//			SetMessage(_T("Failed to save the parameter."));
//			break;
//		}
//
//		MakeViewThreadDisplayer(pDisp)
//		{
//			UpdateParameterList();
//		};
//		AddThreadDisplayer(pDisp);
//	}
//	while(false);
//	
//	return bReturn;
//}

bool CDevicePointGrey::DoesModuleExist()
{
	bool bReturn = false;

	CString strModuleName;

	do
	{
#ifdef _DEBUG
		strModuleName.Format(_T("FlyCapture2_C_v140.dll"));
#else
		strModuleName.Format(_T("FlyCapture2_Cd_v140.dll"));
#endif

		EDeviceLibraryStatus eStatus = CLibraryManager::GetFoundLibrary(strModuleName);

		if(!eStatus)
		{
			HMODULE hModule = LoadLibrary(strModuleName);

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



EPointGreyGetFunction CDevicePointGrey::GetDeviceID(unsigned int * pDeviceID)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pDeviceID)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		*pDeviceID = _ttoi(GetParamValue(EDeviceParameterPointGrey_DeviceID));

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetDeviceID(int nDeviceID)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_DeviceID;

	CString strPreValue = GetParamValue(eSaveNum);
	CString strValue;

	do
	{
		if(IsInitialized())
		{
			eReturn = EPointGreySetFunction_AlreadyInitializedError;
			break;
		}

		strValue.Format(_T("%d"), nDeviceID);

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], strPreValue, strValue);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPointGreyGetFunction CDevicePointGrey::GetGrabCount(unsigned int * pGrabCount)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pGrabCount)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		*pGrabCount = _ttoi(GetParamValue(EDeviceParameterPointGrey_GrabCount));

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetGrabCount(int nGrabCount)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_GrabCount;

	CString strPreValue = GetParamValue(eSaveNum);
	CString strValue;

	do
	{
		strValue.Format(_T("%d"), nGrabCount);

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], strPreValue, strValue);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPointGreyGetFunction CDevicePointGrey::GetPacketSize(unsigned int * pPacketSize)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pPacketSize)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		*pPacketSize = _ttoi(GetParamValue(EDeviceParameterPointGrey_PacketSize));

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetPacketSize(unsigned int nPacketSize)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_PacketSize;

	CString strPreValue = GetParamValue(eSaveNum);
	CString strValue;

	do
	{
		if(IsInitialized())
		{
			fc2GigEProperty GigEProperty;
			GigEProperty.propType = PACKET_SIZE;
			if(fc2GetGigEProperty(m_pContext, &GigEProperty) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			GigEProperty.value = nPacketSize;

			if(fc2SetGigEProperty(m_pContext, &GigEProperty) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}
		}

		strValue.Format(_T("%d"), nPacketSize);

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], strPreValue, strValue);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPointGreyGetFunction CDevicePointGrey::GetPacketDelay(unsigned int * pPacketDelay)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pPacketDelay)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		*pPacketDelay = _ttoi(GetParamValue(EDeviceParameterPointGrey_PacketDelay));

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetPacketDelay(unsigned int nPacketDelay)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_PacketDelay;

	CString strPreValue = GetParamValue(eSaveNum);
	CString strValue;

	do
	{
		if(IsInitialized())
		{
			fc2GigEProperty GigEProperty;
			GigEProperty.propType = PACKET_DELAY;
			if(fc2GetGigEProperty(m_pContext, &GigEProperty) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			GigEProperty.value = nPacketDelay;

			if(fc2SetGigEProperty(m_pContext, &GigEProperty) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}
		}

		strValue.Format(_T("%d"), nPacketDelay);

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], strPreValue, strValue);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}



EPointGreyGetFunction CDevicePointGrey::GetGrabWaitingTime(unsigned int * pGrabWaitingTime)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pGrabWaitingTime)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		*pGrabWaitingTime = _ttoi(GetParamValue(EDeviceParameterPointGrey_GrabWaitingTime));

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetGrabWaitingTime(int nGrabWaitingTime)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_GrabWaitingTime;

	CString strPreValue = GetParamValue(eSaveNum);
	CString strValue;

	do
	{
		strValue.Format(_T("%d"), nGrabWaitingTime);

		if(IsInitialized())
		{
			eReturn = EPointGreySetFunction_AlreadyInitializedError;
			break;
		}

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], strPreValue, strValue);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPointGreyGetFunction CDevicePointGrey::GetGrabMode(EDevicePointGreyGrabMode * pParam)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterPointGrey_GrabMode));

		if(nData < 0 || nData >= (int)EDevicePointGreyGrabMode_Count)
		{
			eReturn = EPointGreyGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDevicePointGreyGrabMode)nData;

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetGrabMode(EDevicePointGreyGrabMode eParam)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_GrabMode;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < 0 || eParam >= EDevicePointGreyGrabMode_Count)
		{
			eReturn = EPointGreySetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EPointGreySetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EPointGreySetFunction_AlreadyInitializedError;
			break;
		}
			   
		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveNum, strSave))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], g_lpszGrabMode[nPreValue], g_lpszGrabMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPointGreyGetFunction CDevicePointGrey::GetGrabNumBuffers(unsigned int * pGrabNumBuffers)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pGrabNumBuffers)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		*pGrabNumBuffers = _ttoi(GetParamValue(EDeviceParameterPointGrey_GrabNumBuffers));

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetGrabNumBuffers(unsigned int nGrabNumBuffers)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_GrabNumBuffers;

	CString strPreValue = GetParamValue(eSaveNum);
	CString strValue;

	do
	{
		strValue.Format(_T("%d"), nGrabNumBuffers);

		if(IsInitialized())
		{
			eReturn = EPointGreySetFunction_AlreadyInitializedError;
			break;
		}

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], strPreValue, strValue);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPointGreyGetFunction CDevicePointGrey::GetCanvasWidth(unsigned int * pCanvasWidth)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pCanvasWidth)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		*pCanvasWidth = _ttoi(GetParamValue(EDeviceParameterPointGrey_CanvasWidth));

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetCanvasWidth(unsigned int nCanvasWidth)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_CanvasWidth;

	CString strPreValue = GetParamValue(eSaveNum);
	CString strValue;

	do
	{
		strValue.Format(_T("%d"), nCanvasWidth);

		if(IsInitialized())
		{
			eReturn = EPointGreySetFunction_AlreadyInitializedError;
			break;
		}

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], strPreValue, strValue);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPointGreyGetFunction CDevicePointGrey::GetCanvasHeight(unsigned int * pCanvasHeight)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pCanvasHeight)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		*pCanvasHeight = _ttoi(GetParamValue(EDeviceParameterPointGrey_CanvasHeight));

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetCanvasHeight(unsigned int nCanvasHeight)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_CanvasHeight;

	CString strPreValue = GetParamValue(eSaveNum);
	CString strValue;

	do
	{
		strValue.Format(_T("%d"), nCanvasHeight);

		if(IsInitialized())
		{
			eReturn = EPointGreySetFunction_AlreadyInitializedError;
			break;
		}

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], strPreValue, strValue);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPointGreyGetFunction CDevicePointGrey::GetOffsetX(unsigned int * pOffsetX)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pOffsetX)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		*pOffsetX = _ttoi(GetParamValue(EDeviceParameterPointGrey_OffsetX));

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetOffsetX(unsigned int nOffsetX)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_OffsetX;

	CString strPreValue = GetParamValue(eSaveNum);
	CString strValue;

	do
	{
		strValue.Format(_T("%d"), nOffsetX);

		if(IsInitialized())
		{
			eReturn = EPointGreySetFunction_AlreadyInitializedError;
			break;
		}

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], strPreValue, strValue);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPointGreyGetFunction CDevicePointGrey::GetOffsetY(unsigned int * pOffsetY)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pOffsetY)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		*pOffsetY = _ttoi(GetParamValue(EDeviceParameterPointGrey_OffsetY));

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetOffsetY(unsigned int nOffsetY)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_OffsetY;

	CString strPreValue = GetParamValue(eSaveNum);
	CString strValue;

	do
	{
		strValue.Format(_T("%d"), nOffsetY);

		if(IsInitialized())
		{
			eReturn = EPointGreySetFunction_AlreadyInitializedError;
			break;
		}

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], strPreValue, strValue);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPointGreyGetFunction CDevicePointGrey::GetPixelFormat(EDevicePointGreyPixelFormat * pParam)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterPointGrey_PixelFormat));

		if(nData < 0 || nData >= (int)EDevicePointGreyPixelFormat_Count)
		{
			eReturn = EPointGreyGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDevicePointGreyPixelFormat)nData;

		//if(nData == 0)
		//	*pParam = EDevicePointGreyPixelFormat_MONO8;
		//else if(nData == 1)
		//	*pParam = EDevicePointGreyPixelFormat_411YUV8;
		//else if(nData == 2)
		//	*pParam = EDevicePointGreyPixelFormat_422YUV8;
		//else if(nData == 3)
		//	*pParam = EDevicePointGreyPixelFormat_444YUV8;
		//else if(nData == 4)
		//	*pParam = EDevicePointGreyPixelFormat_RGB8;
		//else if(nData == 5)
		//	*pParam = EDevicePointGreyPixelFormat_MONO16;
		//else if(nData == 6)
		//	*pParam = EDevicePointGreyPixelFormat_RGB16;
		//else if(nData == 7)
		//	*pParam = EDevicePointGreyPixelFormat_S_MONO16;
		//else if(nData == 8)
		//	*pParam = EDevicePointGreyPixelFormat_S_RGB16;
		//else if(nData == 9)
		//	*pParam = EDevicePointGreyPixelFormat_RAW8;
		//else if(nData == 10)
		//	*pParam = EDevicePointGreyPixelFormat_RAW16;
		//else if(nData == 11)
		//	*pParam = EDevicePointGreyPixelFormat_MONO12;
		//else if(nData == 12)
		//	*pParam = EDevicePointGreyPixelFormat_RAW12;
		//else if(nData == 13)
		//	*pParam = EDevicePointGreyPixelFormat_BGR;
		//else if(nData == 14)
		//	*pParam = EDevicePointGreyPixelFormat_BGRU;
		//else if(nData == 15)
		//	*pParam = EDevicePointGreyPixelFormat_RGB;
		//else if(nData == 16)
		//	*pParam = EDevicePointGreyPixelFormat_RGBU;
		//else if(nData == 17)
		//	*pParam = EDevicePointGreyPixelFormat_BGR16;
		//else if(nData == 18)
		//	*pParam = EDevicePointGreyPixelFormat_BGRU16;
		//else if(nData == 19)
		//	*pParam = EDevicePointGreyPixelFormat_422YUV8_JPEG;

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetPixelFormat(EDevicePointGreyPixelFormat eParam)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_PixelFormat;
	
	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < 0 || eParam >= EDevicePointGreyPixelFormat_Count)
		{
			eReturn = EPointGreySetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EPointGreySetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EPointGreySetFunction_AlreadyInitializedError;
			break;
		}



		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveNum, strSave))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], g_lpszPixelFormat[nPreValue], g_lpszPixelFormat[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPointGreyGetFunction CDevicePointGrey::GetFormatMode(EDevicePointGreyImagemode * pParam)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterPointGrey_FormatMode));

		if(nData < 0 || nData >= (int)EDevicePointGreyImagemode_Count)
		{
			eReturn = EPointGreyGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDevicePointGreyImagemode)nData;

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetFormatMode(EDevicePointGreyImagemode eParam)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_FormatMode;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < 0 || eParam >= EDevicePointGreyImagemode_Count)
		{
			eReturn = EPointGreySetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EPointGreySetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EPointGreySetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveNum, strSave))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], g_lpszImagemode[nPreValue], g_lpszImagemode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}


EPointGreyGetFunction CDevicePointGrey::GetTriggerOnOff(bool * pOn)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pOn)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		*pOn = _ttoi(GetParamValue(EDeviceParameterPointGrey_TriggerOnOff));

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetTriggerOnOff(bool bOn)
{
	//fc2TriggerMode triggerMode;
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_TriggerOnOff;

	bool bPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		CString strValue;
		strValue.Format(_T("%d"), bOn);

		if(IsInitialized())
		{
			eReturn = EPointGreySetFunction_AlreadyInitializedError;
			break;
		}

		//if (fc2GetTriggerMode(m_pContext, &triggerMode))
		//	break;

		//triggerMode.onOff = bOn;

		//if (fc2SetTriggerMode(m_pContext, &triggerMode))
		//{
		//	eReturn = EPointGreySetFunction_WriteToDeviceError;
		//	break;
		//}

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], g_lpszOnOff[bPreValue], g_lpszOnOff[bOn]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPointGreyGetFunction CDevicePointGrey::GetTriggerMode(unsigned int * pTriggerMode)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pTriggerMode)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		*pTriggerMode = _ttoi(GetParamValue(EDeviceParameterPointGrey_TriggerMode));

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetTriggerMode(int nTriggerMode)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_TriggerMode;

	CString strPreValue = g_lpszTriggerMode[_ttoi(GetParamValue(eSaveNum))];
	CString strValue;

	do
	{
		strValue.Format(_T("%d"), nTriggerMode);

		if(IsInitialized())
		{
			eReturn = EPointGreySetFunction_AlreadyInitializedError;
			break;
		}

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], strPreValue, g_lpszTriggerMode[_ttoi(strValue)]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPointGreyGetFunction CDevicePointGrey::GetTriggerSource(unsigned int * pTriggerSource)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pTriggerSource)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		*pTriggerSource = _ttoi(GetParamValue(EDeviceParameterPointGrey_TriggerSource));

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetTriggerSource(int nTriggerSource)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_TriggerSource;

	CString strPreValue = GetParamValue(eSaveNum);
	CString strValue;

	do
	{
		strValue.Format(_T("%d"), nTriggerSource);

		if(IsInitialized())
		{
			eReturn = EPointGreySetFunction_AlreadyInitializedError;
			break;
		}

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], strPreValue, strValue);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPointGreyGetFunction CDevicePointGrey::GetTriggerParameter(unsigned int * pTriggerParameter)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pTriggerParameter)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		*pTriggerParameter = _ttoi(GetParamValue(EDeviceParameterPointGrey_TriggerParameter));

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetTriggerParameter(int nTriggerParameter)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_TriggerParameter;

	CString strPreValue = GetParamValue(eSaveNum);
	CString strValue;

	do
	{
		strValue.Format(_T("%d"), nTriggerParameter);

		if(IsInitialized())
		{
			eReturn = EPointGreySetFunction_AlreadyInitializedError;
			break;
		}

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], strPreValue, strValue);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPointGreyGetFunction CDevicePointGrey::GetTriggerPolarity(unsigned int * pTriggerPolarity)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pTriggerPolarity)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		*pTriggerPolarity = _ttoi(GetParamValue(EDeviceParameterPointGrey_TriggerPolarity));

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetTriggerPolarity(int nTriggerPolarity)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_TriggerPolarity;

	CString strPreValue = GetParamValue(eSaveNum);
	CString strValue;

	do
	{
		strValue.Format(_T("%d"), nTriggerPolarity);

		if(IsInitialized())
		{
			eReturn = EPointGreySetFunction_AlreadyInitializedError;
			break;
		}

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], strPreValue, strValue);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

//EPointGreyGetFunction CDevicePointGrey::GetAbsoluteMode(bool * pOn)
//{
//	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;
//
//	do
//	{
//		if(!pOn)
//		{
//			eReturn = EPointGreyGetFunction_NullptrError;
//			break;
//		}
//
//		*pOn = _ttoi(GetParamValue(_T("Absolute Mode")));
//
//		eReturn = EPointGreyGetFunction_OK;
//	}
//	while(false);
//
//	return eReturn;
//}
//
//EPointGreySetFunction CDevicePointGrey::SetAbsoluteMode(bool bOn)
//{
//	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
//
//	do
//	{
//		CString strValue;
//		strValue.Format(_T("%d"), bOn);
//		
//		if(!SetParamValue(_T("Absolute Mode"), strValue))
//		{
//			eReturn = EPointGreySetFunction_WriteToDatabaseError;
//			break;
//		}
//
//		eReturn = EPointGreySetFunction_OK;
//	}
//	while(false);
//
//	CString strMessage;
//
//	strMessage.Format(_T("%s to change the parameter 'Absolute Mode %s'"), !eReturn ? _T("Succeeded") : _T("Failed"), bOn ? _T("On") : _T("Off"));
//
//	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);
//
//	SetMessage(strMessage);
//
//	return eReturn;
//}

EPointGreyGetFunction CDevicePointGrey::GetBrightnessValue(unsigned int * pBrightness)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pBrightness)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		*pBrightness = _ttoi(GetParamValue(EDeviceParameterPointGrey_BrightnessValue));

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetBrightnessValue(unsigned int nBrightness)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_BrightnessValue;

	CString strPreValue = GetParamValue(eSaveNum);
	CString strValue;

	do
	{
		if(IsInitialized())
		{
			fc2PropertyInfo PropertyInfo;

			PropertyInfo.type = FC2_BRIGHTNESS;
			if(fc2GetPropertyInfo(m_pContext, &PropertyInfo) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(!PropertyInfo.present)
			{
				eReturn = EPointGreySetFunction_NotSupportError;
				break;
			}

			fc2Property Property;
			Property.type = FC2_BRIGHTNESS;
			if(fc2GetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			EDevicePointGreySwitch eParam;
			if(GetBrightnessManual(&eParam))
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(eParam != EDevicePointGreySwitch_CustomValue)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}

			Property.valueA = nBrightness;

			if(fc2SetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}
		}

		strValue.Format(_T("%d"), nBrightness);

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], strPreValue, strValue);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPointGreyGetFunction CDevicePointGrey::GetBrightnessManual(EDevicePointGreySwitch * pParam)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterPointGrey_BrightnessManual));

		if(nData < 0 || nData >= (int)EDevicePointGreySwitch_Count)
		{
			eReturn = EPointGreyGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDevicePointGreySwitch)nData;

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetBrightnessManual(EDevicePointGreySwitch eParam)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_BrightnessManual;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < 0 || eParam >= EDevicePointGreySwitch_Count)
		{
			eReturn = EPointGreySetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EPointGreySetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			fc2PropertyInfo PropertyInfo;

			PropertyInfo.type = FC2_BRIGHTNESS;
			if(fc2GetPropertyInfo(m_pContext, &PropertyInfo) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(!PropertyInfo.present)
			{
				eReturn = EPointGreySetFunction_NotSupportError;
				break;
			}

			fc2Property Property;
			unsigned int nTempNum = 0;

			Property.type = FC2_BRIGHTNESS;
			if(fc2GetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(GetBrightnessValue(&nTempNum))
			{
				eReturn = EPointGreySetFunction_ReadOnDatabaseError;
				break;
			}

			switch(eParam)
			{
			case EDevicePointGreySwitch_CustomValue:
				Property.absControl = false;
				if(PropertyInfo.onOffSupported)
					Property.onOff = true;
				Property.autoManualMode = false;
				Property.onePush = false;
				Property.valueA = nTempNum;
				break;
			case EDevicePointGreySwitch_Auto:
				Property.absControl = false;
				Property.onOff = false;
				if(PropertyInfo.autoSupported)
					Property.autoManualMode = true;
				Property.onePush = false;
				break;
			case EDevicePointGreySwitch_OnePush:
				Property.absControl = false;
				if(PropertyInfo.onOffSupported)
					Property.onOff = true;
				Property.autoManualMode = false;
				if(PropertyInfo.onePushSupported)
					Property.onePush = true;
				break;
			default:
				break;
			}

			if(fc2SetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveNum, strSave))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], g_lpszSwitch[nPreValue], g_lpszSwitch[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}


EPointGreyGetFunction CDevicePointGrey::GetExposureValue(unsigned int * pExposure)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pExposure)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		*pExposure = _ttoi(GetParamValue(EDeviceParameterPointGrey_ExposureValue));

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetExposureValue(unsigned int nExposure)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_ExposureValue;

	CString strPreValue = GetParamValue(eSaveNum);
	CString strValue;

	do
	{
		if(IsInitialized())
		{
			fc2PropertyInfo PropertyInfo;

			PropertyInfo.type = FC2_AUTO_EXPOSURE;
			if(fc2GetPropertyInfo(m_pContext, &PropertyInfo) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(!PropertyInfo.present)
			{
				eReturn = EPointGreySetFunction_NotSupportError;
				break;
			}

			fc2Property Property;
			Property.type = FC2_AUTO_EXPOSURE;
			if(fc2GetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			EDevicePointGreySwitch eParam;
			if(GetExposureManual(&eParam))
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(eParam != EDevicePointGreySwitch_CustomValue)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}

			Property.valueA = nExposure;

			if(fc2SetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}
		}

		strValue.Format(_T("%d"), nExposure);

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter 'Exposure Value %d'"), !eReturn ? _T("Succeeded") : _T("Failed"), nExposure);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPointGreyGetFunction CDevicePointGrey::GetExposureManual(EDevicePointGreySwitch * pParam)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterPointGrey_ExposureManual));

		if(nData < 0 || nData >= (int)EDevicePointGreySwitch_Count)
		{
			eReturn = EPointGreyGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDevicePointGreySwitch)nData;

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetExposureManual(EDevicePointGreySwitch eParam)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_ExposureManual;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < 0 || eParam >= EDevicePointGreySwitch_Count)
		{
			eReturn = EPointGreySetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EPointGreySetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			fc2PropertyInfo PropertyInfo;

			PropertyInfo.type = FC2_AUTO_EXPOSURE;
			if(fc2GetPropertyInfo(m_pContext, &PropertyInfo) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(!PropertyInfo.present)
			{
				eReturn = EPointGreySetFunction_NotSupportError;
				break;
			}

			fc2Property Property;
			unsigned int nTempNum = 0;

			Property.type = FC2_AUTO_EXPOSURE;
			if(fc2GetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(GetExposureValue(&nTempNum))
			{
				eReturn = EPointGreySetFunction_ReadOnDatabaseError;
				break;
			}

			switch(eParam)
			{
			case EDevicePointGreySwitch_CustomValue:
				Property.absControl = false;
				if(PropertyInfo.onOffSupported)
					Property.onOff = true;
				Property.autoManualMode = false;
				Property.onePush = false;
				Property.valueA = nTempNum;
				break;
			case EDevicePointGreySwitch_Auto:
				Property.absControl = false;
				Property.onOff = false;
				if(PropertyInfo.autoSupported)
					Property.autoManualMode = true;
				Property.onePush = false;
				break;
			case EDevicePointGreySwitch_OnePush:
				Property.absControl = false;
				if(PropertyInfo.onOffSupported)
					Property.onOff = true;
				Property.autoManualMode = false;
				if(PropertyInfo.onePushSupported)
					Property.onePush = true;
				break;
			default:
				break;
			}

			if(fc2SetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveNum, strSave))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], g_lpszSwitch[nPreValue], g_lpszSwitch[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}


EPointGreyGetFunction CDevicePointGrey::GetSharpnessValue(unsigned int * pSharpness)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pSharpness)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		*pSharpness = _ttoi(GetParamValue(EDeviceParameterPointGrey_SharpnessValue));

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetSharpnessValue(unsigned int nSharpness)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_SharpnessValue;

	CString strPreValue = GetParamValue(eSaveNum);
	CString strValue;

	do
	{
		if(IsInitialized())
		{
			fc2PropertyInfo PropertyInfo;

			PropertyInfo.type = FC2_SHARPNESS;
			if(fc2GetPropertyInfo(m_pContext, &PropertyInfo) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(!PropertyInfo.present)
			{
				eReturn = EPointGreySetFunction_NotSupportError;
				break;
			}

			fc2Property Property;
			Property.type = FC2_SHARPNESS;
			if(fc2GetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			EDevicePointGreySwitch eParam;
			if(GetSharpnessManual(&eParam))
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(eParam != EDevicePointGreySwitch_CustomValue)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}

			Property.valueA = nSharpness;

			if(fc2SetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}
		}

		strValue.Format(_T("%d"), nSharpness);

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], strPreValue, strValue);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPointGreyGetFunction CDevicePointGrey::GetSharpnessManual(EDevicePointGreySwitch * pParam)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterPointGrey_SharpnessManual));

		if(nData < 0 || nData >= (int)EDevicePointGreySwitch_Count)
		{
			eReturn = EPointGreyGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDevicePointGreySwitch)nData;

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetSharpnessManual(EDevicePointGreySwitch eParam)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_SharpnessManual;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < 0 || eParam >= EDevicePointGreySwitch_Count)
		{
			eReturn = EPointGreySetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EPointGreySetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			fc2PropertyInfo PropertyInfo;

			PropertyInfo.type = FC2_SHARPNESS;
			if(fc2GetPropertyInfo(m_pContext, &PropertyInfo) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(!PropertyInfo.present)
			{
				eReturn = EPointGreySetFunction_NotSupportError;
				break;
			}

			fc2Property Property;
			unsigned int nTempNum = 0;

			Property.type = FC2_SHARPNESS;
			if(fc2GetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(GetSharpnessValue(&nTempNum))
			{
				eReturn = EPointGreySetFunction_ReadOnDatabaseError;
				break;
			}

			switch(eParam)
			{
			case EDevicePointGreySwitch_CustomValue:
				Property.absControl = false;
				if(PropertyInfo.onOffSupported)
					Property.onOff = true;
				Property.autoManualMode = false;
				Property.onePush = false;
				Property.valueA = nTempNum;
				break;
			case EDevicePointGreySwitch_Auto:
				Property.absControl = false;
				Property.onOff = false;
				if(PropertyInfo.autoSupported)
					Property.autoManualMode = true;
				Property.onePush = false;
				break;
			case EDevicePointGreySwitch_OnePush:
				Property.absControl = false;
				if(PropertyInfo.onOffSupported)
					Property.onOff = true;
				Property.autoManualMode = false;
				if(PropertyInfo.onePushSupported)
					Property.onePush = true;
				break;
			default:
				break;
			}

			if(fc2SetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveNum, strSave))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], g_lpszSwitch[nPreValue], g_lpszSwitch[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}


EPointGreyGetFunction CDevicePointGrey::GetWhiteBalanceValue(unsigned int * pWhiteBalance)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pWhiteBalance)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		*pWhiteBalance = _ttoi(GetParamValue(EDeviceParameterPointGrey_WhiteBalanceValue));

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetWhiteBalanceValue(unsigned int nWhiteBalance)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_WhiteBalanceValue;

	CString strPreValue = GetParamValue(eSaveNum);
	CString strValue;

	do
	{
		if(IsInitialized())
		{
			fc2PropertyInfo PropertyInfo;

			PropertyInfo.type = FC2_WHITE_BALANCE;
			if(fc2GetPropertyInfo(m_pContext, &PropertyInfo) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(!PropertyInfo.present)
			{
				eReturn = EPointGreySetFunction_NotSupportError;
				break;
			}

			fc2Property Property;
			Property.type = FC2_WHITE_BALANCE;
			if(fc2GetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			EDevicePointGreySwitch eParam;
			if(GetWhiteBalanceManual(&eParam))
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(eParam != EDevicePointGreySwitch_CustomValue)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}

			Property.valueA = nWhiteBalance;

			if(fc2SetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}
		}

		strValue.Format(_T("%d"), nWhiteBalance);

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], strPreValue, strValue);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPointGreyGetFunction CDevicePointGrey::GetWhiteBalanceManual(EDevicePointGreySwitch * pParam)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterPointGrey_WhiteBalanceManual));

		if(nData < 0 || nData >= (int)EDevicePointGreySwitch_Count)
		{
			eReturn = EPointGreyGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDevicePointGreySwitch)nData;

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetWhiteBalanceManual(EDevicePointGreySwitch eParam)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_WhiteBalanceManual;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < 0 || eParam >= EDevicePointGreySwitch_Count)
		{
			eReturn = EPointGreySetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EPointGreySetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			fc2PropertyInfo PropertyInfo;

			PropertyInfo.type = FC2_WHITE_BALANCE;
			if(fc2GetPropertyInfo(m_pContext, &PropertyInfo) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(!PropertyInfo.present)
			{
				eReturn = EPointGreySetFunction_NotSupportError;
				break;
			}

			fc2Property Property;
			unsigned int nTempNum = 0;

			Property.type = FC2_WHITE_BALANCE;
			if(fc2GetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(GetWhiteBalanceValue(&nTempNum))
			{
				eReturn = EPointGreySetFunction_ReadOnDatabaseError;
				break;
			}

			switch(eParam)
			{
			case EDevicePointGreySwitch_CustomValue:
				Property.absControl = false;
				if(PropertyInfo.onOffSupported)
					Property.onOff = true;
				Property.autoManualMode = false;
				Property.onePush = false;
				Property.valueA = nTempNum;
				break;
			case EDevicePointGreySwitch_Auto:
				Property.absControl = false;
				Property.onOff = false;
				if(PropertyInfo.autoSupported)
					Property.autoManualMode = true;
				Property.onePush = false;
				break;
			case EDevicePointGreySwitch_OnePush:
				Property.absControl = false;
				if(PropertyInfo.onOffSupported)
					Property.onOff = true;
				Property.autoManualMode = false;
				if(PropertyInfo.onePushSupported)
					Property.onePush = true;
				break;
			default:
				break;
			}

			if(fc2SetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveNum, strSave))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], g_lpszSwitch[nPreValue], g_lpszSwitch[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}


EPointGreyGetFunction CDevicePointGrey::GetHueValue(unsigned int * pHue)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pHue)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		*pHue = _ttoi(GetParamValue(EDeviceParameterPointGrey_HueValue));

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetHueValue(unsigned int nHue)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_HueValue;

	CString strPreValue = GetParamValue(eSaveNum);
	CString strValue;

	do
	{
		if(IsInitialized())
		{
			fc2PropertyInfo PropertyInfo;

			PropertyInfo.type = FC2_HUE;
			if(fc2GetPropertyInfo(m_pContext, &PropertyInfo) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(!PropertyInfo.present)
			{
				eReturn = EPointGreySetFunction_NotSupportError;
				break;
			}

			fc2Property Property;
			Property.type = FC2_HUE;
			if(fc2GetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			EDevicePointGreySwitch eParam;
			if(GetHueManual(&eParam))
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(eParam != EDevicePointGreySwitch_CustomValue)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}

			Property.valueA = nHue;

			if(fc2SetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}
		}

		strValue.Format(_T("%d"), nHue);

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], strPreValue, strValue);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPointGreyGetFunction CDevicePointGrey::GetHueManual(EDevicePointGreySwitch * pParam)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterPointGrey_HueManual));

		if(nData < 0 || nData >= (int)EDevicePointGreySwitch_Count)
		{
			eReturn = EPointGreyGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDevicePointGreySwitch)nData;

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetHueManual(EDevicePointGreySwitch eParam)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_HueManual;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < 0 || eParam >= EDevicePointGreySwitch_Count)
		{
			eReturn = EPointGreySetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EPointGreySetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			fc2PropertyInfo PropertyInfo;

			PropertyInfo.type = FC2_HUE;
			if(fc2GetPropertyInfo(m_pContext, &PropertyInfo) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(!PropertyInfo.present)
			{
				eReturn = EPointGreySetFunction_NotSupportError;
				break;
			}

			fc2Property Property;
			unsigned int nTempNum = 0;

			Property.type = FC2_HUE;
			if(fc2GetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(GetHueValue(&nTempNum))
			{
				eReturn = EPointGreySetFunction_ReadOnDatabaseError;
				break;
			}

			switch(eParam)
			{
			case EDevicePointGreySwitch_CustomValue:
				Property.absControl = false;
				if(PropertyInfo.onOffSupported)
					Property.onOff = true;
				Property.autoManualMode = false;
				Property.onePush = false;
				Property.valueA = nTempNum;
				break;
			case EDevicePointGreySwitch_Auto:
				Property.absControl = false;
				Property.onOff = false;
				if(PropertyInfo.autoSupported)
					Property.autoManualMode = true;
				Property.onePush = false;
				break;
			case EDevicePointGreySwitch_OnePush:
				Property.absControl = false;
				if(PropertyInfo.onOffSupported)
					Property.onOff = true;
				Property.autoManualMode = false;
				if(PropertyInfo.onePushSupported)
					Property.onePush = true;
				break;
			default:
				break;
			}

			if(fc2SetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveNum, strSave))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], g_lpszSwitch[nPreValue], g_lpszSwitch[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}


EPointGreyGetFunction CDevicePointGrey::GetSaturationValue(unsigned int * pSaturation)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pSaturation)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		*pSaturation = _ttoi(GetParamValue(EDeviceParameterPointGrey_SaturationValue));

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetSaturationValue(unsigned int nSaturation)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_SaturationValue;

	CString strPreValue = GetParamValue(eSaveNum);
	CString strValue;

	do
	{
		if(IsInitialized())
		{
			fc2PropertyInfo PropertyInfo;

			PropertyInfo.type = FC2_SATURATION;
			if(fc2GetPropertyInfo(m_pContext, &PropertyInfo) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(!PropertyInfo.present)
			{
				eReturn = EPointGreySetFunction_NotSupportError;
				break;
			}

			fc2Property Property;
			Property.type = FC2_SATURATION;
			if(fc2GetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			EDevicePointGreySwitch eParam;
			if(GetSaturationManual(&eParam))
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(eParam != EDevicePointGreySwitch_CustomValue)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}

			Property.valueA = nSaturation;

			if(fc2SetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}
		}

		strValue.Format(_T("%d"), nSaturation);

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], strPreValue, strValue);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPointGreyGetFunction CDevicePointGrey::GetSaturationManual(EDevicePointGreySwitch * pParam)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterPointGrey_SaturationManual));

		if(nData < 0 || nData >= (int)EDevicePointGreySwitch_Count)
		{
			eReturn = EPointGreyGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDevicePointGreySwitch)nData;

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetSaturationManual(EDevicePointGreySwitch eParam)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_SaturationManual;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < 0 || eParam >= EDevicePointGreySwitch_Count)
		{
			eReturn = EPointGreySetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EPointGreySetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			fc2PropertyInfo PropertyInfo;

			PropertyInfo.type = FC2_SATURATION;
			if(fc2GetPropertyInfo(m_pContext, &PropertyInfo) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(!PropertyInfo.present)
			{
				eReturn = EPointGreySetFunction_NotSupportError;
				break;
			}

			fc2Property Property;
			unsigned int nTempNum = 0;

			Property.type = FC2_SATURATION;
			if(fc2GetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(GetSaturationValue(&nTempNum))
			{
				eReturn = EPointGreySetFunction_ReadOnDatabaseError;
				break;
			}

			switch(eParam)
			{
			case EDevicePointGreySwitch_CustomValue:
				Property.absControl = false;
				if(PropertyInfo.onOffSupported)
					Property.onOff = true;
				Property.autoManualMode = false;
				Property.onePush = false;
				Property.valueA = nTempNum;
				break;
			case EDevicePointGreySwitch_Auto:
				Property.absControl = false;
				Property.onOff = false;
				if(PropertyInfo.autoSupported)
					Property.autoManualMode = true;
				Property.onePush = false;
				break;
			case EDevicePointGreySwitch_OnePush:
				Property.absControl = false;
				if(PropertyInfo.onOffSupported)
					Property.onOff = true;
				Property.autoManualMode = false;
				if(PropertyInfo.onePushSupported)
					Property.onePush = true;
				break;
			default:
				break;
			}

			if(fc2SetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveNum, strSave))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], g_lpszSwitch[nPreValue], g_lpszSwitch[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}


EPointGreyGetFunction CDevicePointGrey::GetGammaValue(unsigned int * pGamma)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pGamma)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		*pGamma = _ttoi(GetParamValue(EDeviceParameterPointGrey_GammaValue));

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetGammaValue(unsigned int nGamma)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_GammaValue;

	CString strPreValue = GetParamValue(eSaveNum);
	CString strValue;

	do
	{
		if(IsInitialized())
		{
			fc2PropertyInfo PropertyInfo;

			PropertyInfo.type = FC2_GAMMA;
			if(fc2GetPropertyInfo(m_pContext, &PropertyInfo) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(!PropertyInfo.present)
			{
				eReturn = EPointGreySetFunction_NotSupportError;
				break;
			}

			fc2Property Property;
			Property.type = FC2_GAMMA;
			if(fc2GetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			EDevicePointGreySwitch eParam;
			if(GetGammaManual(&eParam))
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(eParam != EDevicePointGreySwitch_CustomValue)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}

			Property.valueA = nGamma;

			if(fc2SetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}
		}

		strValue.Format(_T("%d"), nGamma);

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], strPreValue, strValue);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPointGreyGetFunction CDevicePointGrey::GetGammaManual(EDevicePointGreySwitch * pParam)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterPointGrey_GammaManual));

		if(nData < 0 || nData >= (int)EDevicePointGreySwitch_Count)
		{
			eReturn = EPointGreyGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDevicePointGreySwitch)nData;

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetGammaManual(EDevicePointGreySwitch eParam)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_GammaManual;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < 0 || eParam >= EDevicePointGreySwitch_Count)
		{
			eReturn = EPointGreySetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EPointGreySetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			fc2PropertyInfo PropertyInfo;

			PropertyInfo.type = FC2_GAMMA;
			if(fc2GetPropertyInfo(m_pContext, &PropertyInfo) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(!PropertyInfo.present)
			{
				eReturn = EPointGreySetFunction_NotSupportError;
				break;
			}

			fc2Property Property;
			unsigned int nTempNum = 0;

			Property.type = FC2_GAMMA;
			if(fc2GetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(GetGammaValue(&nTempNum))
			{
				eReturn = EPointGreySetFunction_ReadOnDatabaseError;
				break;
			}

			switch(eParam)
			{
			case EDevicePointGreySwitch_CustomValue:
				Property.absControl = false;
				if(PropertyInfo.onOffSupported)
					Property.onOff = true;
				Property.autoManualMode = false;
				Property.onePush = false;
				Property.valueA = nTempNum;
				break;
			case EDevicePointGreySwitch_Auto:
				Property.absControl = false;
				Property.onOff = false;
				if(PropertyInfo.autoSupported)
					Property.autoManualMode = true;
				Property.onePush = false;
				break;
			case EDevicePointGreySwitch_OnePush:
				Property.absControl = false;
				if(PropertyInfo.onOffSupported)
					Property.onOff = true;
				Property.autoManualMode = false;
				if(PropertyInfo.onePushSupported)
					Property.onePush = true;
				break;
			default:
				break;
			}

			if(fc2SetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveNum, strSave))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], g_lpszSwitch[nPreValue], g_lpszSwitch[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}


EPointGreyGetFunction CDevicePointGrey::GetIrisValue(unsigned int * pIris)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pIris)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		*pIris = _ttoi(GetParamValue(EDeviceParameterPointGrey_IrisValue));

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetIrisValue(unsigned int nIris)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_IrisValue;

	CString strPreValue = GetParamValue(eSaveNum);
	CString strValue;

	do
	{
		if(IsInitialized())
		{
			fc2PropertyInfo PropertyInfo;

			PropertyInfo.type = FC2_IRIS;
			if(fc2GetPropertyInfo(m_pContext, &PropertyInfo) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(!PropertyInfo.present)
			{
				eReturn = EPointGreySetFunction_NotSupportError;
				break;
			}

			fc2Property Property;
			Property.type = FC2_IRIS;
			if(fc2GetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			EDevicePointGreySwitch eParam;
			if(GetIrisManual(&eParam))
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(eParam != EDevicePointGreySwitch_CustomValue)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}

			Property.valueA = nIris;

			if(fc2SetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}
		}

		strValue.Format(_T("%d"), nIris);

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], strPreValue, strValue);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPointGreyGetFunction CDevicePointGrey::GetIrisManual(EDevicePointGreySwitch * pParam)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterPointGrey_IrisManual));

		if(nData < 0 || nData >= (int)EDevicePointGreySwitch_Count)
		{
			eReturn = EPointGreyGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDevicePointGreySwitch)nData;

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetIrisManual(EDevicePointGreySwitch eParam)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_IrisManual;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < 0 || eParam >= EDevicePointGreySwitch_Count)
		{
			eReturn = EPointGreySetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EPointGreySetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			fc2PropertyInfo PropertyInfo;

			PropertyInfo.type = FC2_IRIS;
			if(fc2GetPropertyInfo(m_pContext, &PropertyInfo) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(!PropertyInfo.present)
			{
				eReturn = EPointGreySetFunction_NotSupportError;
				break;
			}

			fc2Property Property;
			unsigned int nTempNum = 0;

			Property.type = FC2_IRIS;
			if(fc2GetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(GetIrisValue(&nTempNum))
			{
				eReturn = EPointGreySetFunction_ReadOnDatabaseError;
				break;
			}

			switch(eParam)
			{
			case EDevicePointGreySwitch_CustomValue:
				Property.absControl = false;
				if(PropertyInfo.onOffSupported)
					Property.onOff = true;
				Property.autoManualMode = false;
				Property.onePush = false;
				Property.valueA = nTempNum;
				break;
			case EDevicePointGreySwitch_Auto:
				Property.absControl = false;
				Property.onOff = false;
				if(PropertyInfo.autoSupported)
					Property.autoManualMode = true;
				Property.onePush = false;
				break;
			case EDevicePointGreySwitch_OnePush:
				Property.absControl = false;
				if(PropertyInfo.onOffSupported)
					Property.onOff = true;
				Property.autoManualMode = false;
				if(PropertyInfo.onePushSupported)
					Property.onePush = true;
				break;
			default:
				break;
			}

			if(fc2SetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveNum, strSave))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], g_lpszSwitch[nPreValue], g_lpszSwitch[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}


EPointGreyGetFunction CDevicePointGrey::GetFocusValue(unsigned int * pFocus)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pFocus)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		*pFocus = _ttoi(GetParamValue(EDeviceParameterPointGrey_FocusValue));

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetFocusValue(unsigned int nFocus)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_FocusValue;

	CString strPreValue = GetParamValue(eSaveNum);
	CString strValue;

	do
	{
		if(IsInitialized())
		{
			fc2PropertyInfo PropertyInfo;

			PropertyInfo.type = FC2_FOCUS;
			if(fc2GetPropertyInfo(m_pContext, &PropertyInfo) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(!PropertyInfo.present)
			{
				eReturn = EPointGreySetFunction_NotSupportError;
				break;
			}

			fc2Property Property;
			Property.type = FC2_FOCUS;
			if(fc2GetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			EDevicePointGreySwitch eParam;
			if(GetFocusManual(&eParam))
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(eParam != EDevicePointGreySwitch_CustomValue)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}

			Property.valueA = nFocus;

			if(fc2SetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}
		}

		strValue.Format(_T("%d"), nFocus);

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], strPreValue, strValue);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPointGreyGetFunction CDevicePointGrey::GetFocusManual(EDevicePointGreySwitch * pParam)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterPointGrey_FocusManual));

		if(nData < 0 || nData >= (int)EDevicePointGreySwitch_Count)
		{
			eReturn = EPointGreyGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDevicePointGreySwitch)nData;

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetFocusManual(EDevicePointGreySwitch eParam)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_FocusManual;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < 0 || eParam >= EDevicePointGreySwitch_Count)
		{
			eReturn = EPointGreySetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EPointGreySetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			fc2PropertyInfo PropertyInfo;

			PropertyInfo.type = FC2_FOCUS;
			if(fc2GetPropertyInfo(m_pContext, &PropertyInfo) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(!PropertyInfo.present)
			{
				eReturn = EPointGreySetFunction_NotSupportError;
				break;
			}

			fc2Property Property;
			unsigned int nTempNum = 0;

			Property.type = FC2_FOCUS;
			if(fc2GetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(GetFocusValue(&nTempNum))
			{
				eReturn = EPointGreySetFunction_ReadOnDatabaseError;
				break;
			}

			switch(eParam)
			{
			case EDevicePointGreySwitch_CustomValue:
				Property.absControl = false;
				if(PropertyInfo.onOffSupported)
					Property.onOff = true;
				Property.autoManualMode = false;
				Property.onePush = false;
				Property.valueA = nTempNum;
				break;
			case EDevicePointGreySwitch_Auto:
				Property.absControl = false;
				Property.onOff = false;
				if(PropertyInfo.autoSupported)
					Property.autoManualMode = true;
				Property.onePush = false;
				break;
			case EDevicePointGreySwitch_OnePush:
				Property.absControl = false;
				if(PropertyInfo.onOffSupported)
					Property.onOff = true;
				Property.autoManualMode = false;
				if(PropertyInfo.onePushSupported)
					Property.onePush = true;
				break;
			default:
				break;
			}

			if(fc2SetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveNum, strSave))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], g_lpszSwitch[nPreValue], g_lpszSwitch[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}


EPointGreyGetFunction CDevicePointGrey::GetZoomValue(unsigned int * pZoom)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pZoom)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		*pZoom = _ttoi(GetParamValue(EDeviceParameterPointGrey_ZoomValue));

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetZoomValue(unsigned int nZoom)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_ZoomValue;

	CString strPreValue = GetParamValue(eSaveNum);
	CString strValue;

	do
	{
		if(IsInitialized())
		{
			fc2PropertyInfo PropertyInfo;

			PropertyInfo.type = FC2_ZOOM;
			if(fc2GetPropertyInfo(m_pContext, &PropertyInfo) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(!PropertyInfo.present)
			{
				eReturn = EPointGreySetFunction_NotSupportError;
				break;
			}

			fc2Property Property;
			Property.type = FC2_ZOOM;
			if(fc2GetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			EDevicePointGreySwitch eParam;
			if(GetZoomManual(&eParam))
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(eParam != EDevicePointGreySwitch_CustomValue)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}

			Property.valueA = nZoom;

			if(fc2SetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}
		}

		strValue.Format(_T("%d"), nZoom);

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], strPreValue, strValue);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPointGreyGetFunction CDevicePointGrey::GetZoomManual(EDevicePointGreySwitch * pParam)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterPointGrey_ZoomManual));

		if(nData < 0 || nData >= (int)EDevicePointGreySwitch_Count)
		{
			eReturn = EPointGreyGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDevicePointGreySwitch)nData;

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetZoomManual(EDevicePointGreySwitch eParam)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_ZoomManual;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < 0 || eParam >= EDevicePointGreySwitch_Count)
		{
			eReturn = EPointGreySetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EPointGreySetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			fc2PropertyInfo PropertyInfo;

			PropertyInfo.type = FC2_ZOOM;
			if(fc2GetPropertyInfo(m_pContext, &PropertyInfo) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(!PropertyInfo.present)
			{
				eReturn = EPointGreySetFunction_NotSupportError;
				break;
			}

			fc2Property Property;
			unsigned int nTempNum = 0;

			Property.type = FC2_ZOOM;
			if(fc2GetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(GetZoomValue(&nTempNum))
			{
				eReturn = EPointGreySetFunction_ReadOnDatabaseError;
				break;
			}

			switch(eParam)
			{
			case EDevicePointGreySwitch_CustomValue:
				Property.absControl = false;
				if(PropertyInfo.onOffSupported)
					Property.onOff = true;
				Property.autoManualMode = false;
				Property.onePush = false;
				Property.valueA = nTempNum;
				break;
			case EDevicePointGreySwitch_Auto:
				Property.absControl = false;
				Property.onOff = false;
				if(PropertyInfo.autoSupported)
					Property.autoManualMode = true;
				Property.onePush = false;
				break;
			case EDevicePointGreySwitch_OnePush:
				Property.absControl = false;
				if(PropertyInfo.onOffSupported)
					Property.onOff = true;
				Property.autoManualMode = false;
				if(PropertyInfo.onePushSupported)
					Property.onePush = true;
				break;
			default:
				break;
			}

			if(fc2SetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveNum, strSave))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], g_lpszSwitch[nPreValue], g_lpszSwitch[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}


EPointGreyGetFunction CDevicePointGrey::GetPanValue(unsigned int * pPan)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pPan)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		*pPan = _ttoi(GetParamValue(EDeviceParameterPointGrey_PanValue));

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetPanValue(unsigned int nPan)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_PanValue;

	CString strPreValue = GetParamValue(eSaveNum);
	CString strValue;

	do
	{
		if(IsInitialized())
		{
			fc2PropertyInfo PropertyInfo;

			PropertyInfo.type = FC2_PAN;
			if(fc2GetPropertyInfo(m_pContext, &PropertyInfo) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(!PropertyInfo.present)
			{
				eReturn = EPointGreySetFunction_NotSupportError;
				break;
			}

			fc2Property Property;
			Property.type = FC2_PAN;
			if(fc2GetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			EDevicePointGreySwitch eParam;
			if(GetPanManual(&eParam))
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(eParam != EDevicePointGreySwitch_CustomValue)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}

			Property.valueA = nPan;

			if(fc2SetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}
		}

		strValue.Format(_T("%d"), nPan);

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], strPreValue, strValue);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPointGreyGetFunction CDevicePointGrey::GetPanManual(EDevicePointGreySwitch * pParam)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterPointGrey_PanManual));

		if(nData < 0 || nData >= (int)EDevicePointGreySwitch_Count)
		{
			eReturn = EPointGreyGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDevicePointGreySwitch)nData;

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetPanManual(EDevicePointGreySwitch eParam)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_PanManual;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < 0 || eParam >= EDevicePointGreySwitch_Count)
		{
			eReturn = EPointGreySetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EPointGreySetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			fc2PropertyInfo PropertyInfo;

			PropertyInfo.type = FC2_PAN;
			if(fc2GetPropertyInfo(m_pContext, &PropertyInfo) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(!PropertyInfo.present)
			{
				eReturn = EPointGreySetFunction_NotSupportError;
				break;
			}

			fc2Property Property;
			unsigned int nTempNum = 0;

			Property.type = FC2_PAN;
			if(fc2GetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(GetPanValue(&nTempNum))
			{
				eReturn = EPointGreySetFunction_ReadOnDatabaseError;
				break;
			}

			switch(eParam)
			{
			case EDevicePointGreySwitch_CustomValue:
				Property.absControl = false;
				if(PropertyInfo.onOffSupported)
					Property.onOff = true;
				Property.autoManualMode = false;
				Property.onePush = false;
				Property.valueA = nTempNum;
				break;
			case EDevicePointGreySwitch_Auto:
				Property.absControl = false;
				Property.onOff = false;
				if(PropertyInfo.autoSupported)
					Property.autoManualMode = true;
				Property.onePush = false;
				break;
			case EDevicePointGreySwitch_OnePush:
				Property.absControl = false;
				if(PropertyInfo.onOffSupported)
					Property.onOff = true;
				Property.autoManualMode = false;
				if(PropertyInfo.onePushSupported)
					Property.onePush = true;
				break;
			default:
				break;
			}

			if(fc2SetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveNum, strSave))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], g_lpszSwitch[nPreValue], g_lpszSwitch[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}


EPointGreyGetFunction CDevicePointGrey::GetTiltValue(unsigned int * pTilt)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pTilt)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		*pTilt = _ttoi(GetParamValue(EDeviceParameterPointGrey_TiltValue));

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetTiltValue(unsigned int nTilt)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_TiltValue;

	CString strPreValue = GetParamValue(eSaveNum);
	CString strValue;

	do
	{
		if(IsInitialized())
		{
			fc2PropertyInfo PropertyInfo;

			PropertyInfo.type = FC2_TILT;
			if(fc2GetPropertyInfo(m_pContext, &PropertyInfo) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(!PropertyInfo.present)
			{
				eReturn = EPointGreySetFunction_NotSupportError;
				break;
			}

			fc2Property Property;
			Property.type = FC2_TILT;
			if(fc2GetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			EDevicePointGreySwitch eParam;
			if(GetTiltManual(&eParam))
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(eParam != EDevicePointGreySwitch_CustomValue)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}

			Property.valueA = nTilt;

			if(fc2SetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}
		}

		strValue.Format(_T("%d"), nTilt);

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], strPreValue, strValue);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPointGreyGetFunction CDevicePointGrey::GetTiltManual(EDevicePointGreySwitch * pParam)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterPointGrey_TiltManual));

		if(nData < 0 || nData >= (int)EDevicePointGreySwitch_Count)
		{
			eReturn = EPointGreyGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDevicePointGreySwitch)nData;

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetTiltManual(EDevicePointGreySwitch eParam)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_TiltManual;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < 0 || eParam >= EDevicePointGreySwitch_Count)
		{
			eReturn = EPointGreySetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EPointGreySetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			fc2PropertyInfo PropertyInfo;

			PropertyInfo.type = FC2_TILT;
			if(fc2GetPropertyInfo(m_pContext, &PropertyInfo) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(!PropertyInfo.present)
			{
				eReturn = EPointGreySetFunction_NotSupportError;
				break;
			}

			fc2Property Property;
			unsigned int nTempNum = 0;

			Property.type = FC2_TILT;
			if(fc2GetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(GetTiltValue(&nTempNum))
			{
				eReturn = EPointGreySetFunction_ReadOnDatabaseError;
				break;
			}

			switch(eParam)
			{
			case EDevicePointGreySwitch_CustomValue:
				Property.absControl = false;
				if(PropertyInfo.onOffSupported)
					Property.onOff = true;
				Property.autoManualMode = false;
				Property.onePush = false;
				Property.valueA = nTempNum;
				break;
			case EDevicePointGreySwitch_Auto:
				Property.absControl = false;
				Property.onOff = false;
				if(PropertyInfo.autoSupported)
					Property.autoManualMode = true;
				Property.onePush = false;
				break;
			case EDevicePointGreySwitch_OnePush:
				Property.absControl = false;
				if(PropertyInfo.onOffSupported)
					Property.onOff = true;
				Property.autoManualMode = false;
				if(PropertyInfo.onePushSupported)
					Property.onePush = true;
				break;
			default:
				break;
			}

			if(fc2SetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveNum, strSave))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], g_lpszSwitch[nPreValue], g_lpszSwitch[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}


EPointGreyGetFunction CDevicePointGrey::GetShutterValue(unsigned int * pShutter)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pShutter)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		*pShutter = _ttoi(GetParamValue(EDeviceParameterPointGrey_ShutterValue));

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetShutterValue(unsigned int nShutter)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_ShutterValue;

	CString strPreValue = GetParamValue(eSaveNum);
	CString strValue;

	do
	{
		if(IsInitialized())
		{
			fc2PropertyInfo PropertyInfo;

			PropertyInfo.type = FC2_SHUTTER;
			if(fc2GetPropertyInfo(m_pContext, &PropertyInfo) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(!PropertyInfo.present)
			{
				eReturn = EPointGreySetFunction_NotSupportError;
				break;
			}

			fc2Property Property;
			Property.type = FC2_SHUTTER;
			if(fc2GetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			EDevicePointGreySwitch eParam;
			if(GetShutterManual(&eParam))
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(eParam != EDevicePointGreySwitch_CustomValue)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}

			Property.valueA = nShutter;

			if(fc2SetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}
		}

		strValue.Format(_T("%d"), nShutter);

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], strPreValue, strValue);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPointGreyGetFunction CDevicePointGrey::GetShutterManual(EDevicePointGreySwitch * pParam)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterPointGrey_ShutterManual));

		if(nData < 0 || nData >= (int)EDevicePointGreySwitch_Count)
		{
			eReturn = EPointGreyGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDevicePointGreySwitch)nData;

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetShutterManual(EDevicePointGreySwitch eParam)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_ShutterManual;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < 0 || eParam >= EDevicePointGreySwitch_Count)
		{
			eReturn = EPointGreySetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EPointGreySetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			fc2PropertyInfo PropertyInfo;

			PropertyInfo.type = FC2_SHUTTER;
			if(fc2GetPropertyInfo(m_pContext, &PropertyInfo) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(!PropertyInfo.present)
			{
				eReturn = EPointGreySetFunction_NotSupportError;
				break;
			}

			fc2Property Property;
			unsigned int nTempNum = 0;

			Property.type = FC2_SHUTTER;
			if(fc2GetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(GetShutterValue(&nTempNum))
			{
				eReturn = EPointGreySetFunction_ReadOnDatabaseError;
				break;
			}

			switch(eParam)
			{
			case EDevicePointGreySwitch_CustomValue:
				Property.absControl = false;
				if(PropertyInfo.onOffSupported)
					Property.onOff = true;
				Property.autoManualMode = false;
				Property.onePush = false;
				Property.valueA = nTempNum;
				break;
			case EDevicePointGreySwitch_Auto:
				Property.absControl = false;
				Property.onOff = false;
				if(PropertyInfo.autoSupported)
					Property.autoManualMode = true;
				Property.onePush = false;
				break;
			case EDevicePointGreySwitch_OnePush:
				Property.absControl = false;
				if(PropertyInfo.onOffSupported)
					Property.onOff = true;
				Property.autoManualMode = false;
				if(PropertyInfo.onePushSupported)
					Property.onePush = true;
				break;
			default:
				break;
			}

			if(fc2SetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveNum, strSave))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], g_lpszSwitch[nPreValue], g_lpszSwitch[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}


EPointGreyGetFunction CDevicePointGrey::GetGainValue(unsigned int * pGain)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pGain)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		*pGain = _ttoi(GetParamValue(EDeviceParameterPointGrey_GainValue));

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetGainValue(unsigned int nGain)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_GainValue;

	CString strPreValue = GetParamValue(eSaveNum);
	CString strValue;

	do
	{
		if(IsInitialized())
		{
			fc2PropertyInfo PropertyInfo;

			PropertyInfo.type = FC2_GAIN;
			if(fc2GetPropertyInfo(m_pContext, &PropertyInfo) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(!PropertyInfo.present)
			{
				eReturn = EPointGreySetFunction_NotSupportError;
				break;
			}

			fc2Property Property;
			Property.type = FC2_GAIN;
			if(fc2GetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			EDevicePointGreySwitch eParam;
			if(GetGainManual(&eParam))
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(eParam != EDevicePointGreySwitch_CustomValue)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}

			Property.valueA = nGain;

			if(fc2SetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}
		}

		strValue.Format(_T("%d"), nGain);

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], strPreValue, strValue);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPointGreyGetFunction CDevicePointGrey::GetGainManual(EDevicePointGreySwitch * pParam)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterPointGrey_GainManual));

		if(nData < 0 || nData >= (int)EDevicePointGreySwitch_Count)
		{
			eReturn = EPointGreyGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDevicePointGreySwitch)nData;

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetGainManual(EDevicePointGreySwitch eParam)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_GainManual;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < 0 || eParam >= EDevicePointGreySwitch_Count)
		{
			eReturn = EPointGreySetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EPointGreySetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			fc2PropertyInfo PropertyInfo;

			PropertyInfo.type = FC2_GAIN;
			if(fc2GetPropertyInfo(m_pContext, &PropertyInfo) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(!PropertyInfo.present)
			{
				eReturn = EPointGreySetFunction_NotSupportError;
				break;
			}

			fc2Property Property;
			unsigned int nTempNum = 0;

			Property.type = FC2_GAIN;
			if(fc2GetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(GetGainValue(&nTempNum))
			{
				eReturn = EPointGreySetFunction_ReadOnDatabaseError;
				break;
			}

			switch(eParam)
			{
			case EDevicePointGreySwitch_CustomValue:
				Property.absControl = false;
				if(PropertyInfo.onOffSupported)
					Property.onOff = true;
				Property.autoManualMode = false;
				Property.onePush = false;
				Property.valueA = nTempNum;
				break;
			case EDevicePointGreySwitch_Auto:
				Property.absControl = false;
				Property.onOff = false;
				if(PropertyInfo.autoSupported)
					Property.autoManualMode = true;
				Property.onePush = false;
				break;
			case EDevicePointGreySwitch_OnePush:
				Property.absControl = false;
				if(PropertyInfo.onOffSupported)
					Property.onOff = true;
				Property.autoManualMode = false;
				if(PropertyInfo.onePushSupported)
					Property.onePush = true;
				break;
			default:
				break;
			}

			if(fc2SetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveNum, strSave))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], g_lpszSwitch[nPreValue], g_lpszSwitch[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPointGreyGetFunction CDevicePointGrey::GetFrameRateValue(unsigned int * pFrameRate)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pFrameRate)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		*pFrameRate = _ttoi(GetParamValue(EDeviceParameterPointGrey_FrameRateValue));

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetFrameRateValue(unsigned int nFrameRate)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_FrameRateValue;

	CString strPreValue = GetParamValue(eSaveNum);
	CString strValue;

	do
	{
		if(IsInitialized())
		{
			fc2PropertyInfo PropertyInfo;

			PropertyInfo.type = FC2_FRAME_RATE;
			if(fc2GetPropertyInfo(m_pContext, &PropertyInfo) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(!PropertyInfo.present)
			{
				eReturn = EPointGreySetFunction_NotSupportError;
				break;
			}

			fc2Property Property;
			Property.type = FC2_FRAME_RATE;
			if(fc2GetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			EDevicePointGreySwitch eParam;
			if(GetFrameRateManual(&eParam))
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(eParam != EDevicePointGreySwitch_CustomValue)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}

			Property.valueA = nFrameRate;

			if(fc2SetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}
		}

		strValue.Format(_T("%d"), nFrameRate);

		if(!SetParamValue(eSaveNum, strValue))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], strPreValue, strValue);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EPointGreyGetFunction CDevicePointGrey::GetFrameRateManual(EDevicePointGreySwitch * pParam)
{
	EPointGreyGetFunction eReturn = EPointGreyGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EPointGreyGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterPointGrey_FrameRateManual));

		if(nData < 0 || nData >= (int)EDevicePointGreySwitch_Count)
		{
			eReturn = EPointGreyGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDevicePointGreySwitch)nData;

		eReturn = EPointGreyGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EPointGreySetFunction CDevicePointGrey::SetFrameRateManual(EDevicePointGreySwitch eParam)
{
	EPointGreySetFunction eReturn = EPointGreySetFunction_UnknownError;
	EDeviceParameterPointGrey eSaveNum = EDeviceParameterPointGrey_FrameRateManual;

	int nPreValue = _ttoi(GetParamValue(eSaveNum));

	do
	{
		if(eParam < 0 || eParam >= EDevicePointGreySwitch_Count)
		{
			eReturn = EPointGreySetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EPointGreySetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			fc2PropertyInfo PropertyInfo;

			PropertyInfo.type = FC2_FRAME_RATE;
			if(fc2GetPropertyInfo(m_pContext, &PropertyInfo) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(!PropertyInfo.present)
			{
				eReturn = EPointGreySetFunction_NotSupportError;
				break;
			}

			fc2Property Property;
			unsigned int nTempNum = 0;

			Property.type = FC2_FRAME_RATE;
			if(fc2GetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_ReadOnDeviceError;
				break;
			}

			if(GetFrameRateValue(&nTempNum))
			{
				eReturn = EPointGreySetFunction_ReadOnDatabaseError;
				break;
			}

			switch(eParam)
			{
			case EDevicePointGreySwitch_CustomValue:
				Property.absControl = false;
				if(PropertyInfo.onOffSupported)
					Property.onOff = true;
				Property.autoManualMode = false;
				Property.onePush = false;
				Property.valueA = nTempNum;
				break;
			case EDevicePointGreySwitch_Auto:
				Property.absControl = false;
				Property.onOff = false;
				if(PropertyInfo.autoSupported)
					Property.autoManualMode = true;
				Property.onePush = false;
				break;
			case EDevicePointGreySwitch_OnePush:
				Property.absControl = false;
				if(PropertyInfo.onOffSupported)
					Property.onOff = true;
				Property.autoManualMode = false;
				if(PropertyInfo.onePushSupported)
					Property.onePush = true;
				break;
			default:
				break;
			}

			if(fc2SetProperty(m_pContext, &Property) != FC2_ERROR_OK)
			{
				eReturn = EPointGreySetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveNum, strSave))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveNum))
		{
			eReturn = EPointGreySetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EPointGreySetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(_T("%s to change the parameter '%s' from '%s' to '%s'"), !eReturn ? _T("Succeeded") : _T("Failed"), g_lpszDevicePointGrey[eSaveNum], g_lpszSwitch[nPreValue], g_lpszSwitch[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}



EPointGreyGetFunction CDevicePointGrey::GetTriggerDelay(unsigned int * pTriggerDelay)
{
	return EPointGreyGetFunction();
}

EPointGreySetFunction CDevicePointGrey::SetTriggerDelay(int nTriggerDelay)
{
	return EPointGreySetFunction();
}


UINT CDevicePointGrey::CallbackFunction(LPVOID pParam)
{
	CDevicePointGrey* pInstance = (CDevicePointGrey*)pParam;

	if(pInstance)
	{

		fc2Image image;

		fc2Error error = FC2_ERROR_UNDEFINED;

		do
		{
			int nSize = 0;
			unsigned int nStreams = 0;
			unsigned int nCount = 0;

			if(pInstance->GetGrabCount(&nCount))
				break;

			error = fc2CreateImage(&image);
			if(error != FC2_ERROR_OK)
				break;


			//trigger mode 상태 확인

			if( pInstance->GetImageInfo()->GetChannels() == 1 &&  pInstance->GetImageInfo()->GetDepth() == 8)
			{
				do
				{
					pInstance->NextImageIndex();

					CRavidImage* pCurrentImage = pInstance->GetImageInfo();

					BYTE* pCurrentBuffer = pCurrentImage->GetBuffer();
					BYTE** ppCurrentOffsetY = pCurrentImage->GetYOffsetTable();

					const int64_t i64Width = (int64_t)pCurrentImage->GetSizeX();
					const int64_t i64Height = (int64_t)pCurrentImage->GetSizeY();
					const int64_t i64WidthStep = (int64_t)pCurrentImage->GetWidthStep();
					const int64_t i64ImageSizeByte = (int64_t)pCurrentImage->GetImageSizeByte();
					const int64_t i64PixelSizeByte = (int64_t)pCurrentImage->GetPixelSizeByte();

					error = fc2RetrieveBuffer(pInstance->m_pContext, &image);
					if(error != FC2_ERROR_OK)
						break;

					memcpy(pCurrentBuffer, (BYTE*)image.pData, i64ImageSizeByte);
					
					pInstance->ConnectImage(false);

					CEventHandlerManager::BroadcastOnAcquisition(pCurrentImage, pInstance->GetObjectID(), pInstance->GetImageView(), pInstance);

					if(!pInstance->m_bTriggerOnOff && !pInstance->IsLive())
						break;

					++pInstance->m_nGrabCount;
				}
				while(pInstance->IsInitialized() && (pInstance->IsLive() || nCount > pInstance->m_nGrabCount));
			}
			else if( pInstance->GetImageInfo()->GetChannels() == 1 &&  pInstance->GetImageInfo()->GetDepth() == 16)
			{
				do
				{
					pInstance->NextImageIndex();

					CRavidImage* pCurrentImage = pInstance->GetImageInfo();

					BYTE* pCurrentBuffer = pCurrentImage->GetBuffer();
					BYTE** ppCurrentOffsetY = pCurrentImage->GetYOffsetTable();

					const int64_t i64Width = (int64_t)pCurrentImage->GetSizeX();
					const int64_t i64Height = (int64_t)pCurrentImage->GetSizeY();
					const int64_t i64WidthStep = (int64_t)pCurrentImage->GetWidthStep();
					const int64_t i64ImageSizeByte = (int64_t)pCurrentImage->GetImageSizeByte();
					const int64_t i64PixelSizeByte = (int64_t)pCurrentImage->GetPixelSizeByte();

					error = fc2RetrieveBuffer(pInstance->m_pContext, &image);
					if(error != FC2_ERROR_OK)
						break;

					memcpy(pCurrentBuffer, (WORD*)image.pData, i64ImageSizeByte);

					CEventHandlerManager::BroadcastOnAcquisition(pCurrentImage, pInstance->GetObjectID(), pInstance->GetImageView(), pInstance);
					
					if(!pInstance->m_bTriggerOnOff && !pInstance->IsLive())
						break;

					++pInstance->m_nGrabCount;
				}
				while(pInstance->IsInitialized() && (pInstance->IsLive() || nCount > pInstance->m_nGrabCount));
			}
			else // fc2ConvertImageTo() BGR8 enum이 없어 RGB8을 으로 카메라 셋팅후 BGR로 FORMAT 변환 해줌
			{
				do
				{
					pInstance->NextImageIndex();

					CRavidImage* pCurrentImage = pInstance->GetImageInfo();

					BYTE* pCurrentBuffer = pCurrentImage->GetBuffer();
					BYTE** ppCurrentOffsetY = pCurrentImage->GetYOffsetTable();

					const int64_t i64Width = (int64_t)pCurrentImage->GetSizeX();
					const int64_t i64Height = (int64_t)pCurrentImage->GetSizeY();
					const int64_t i64WidthStep = (int64_t)pCurrentImage->GetWidthStep();
					const int64_t i64ImageSizeByte = (int64_t)pCurrentImage->GetImageSizeByte();
					const int64_t i64PixelSizeByte = (int64_t)pCurrentImage->GetPixelSizeByte();

					error = fc2RetrieveBuffer(pInstance->m_pContext, &image);
					if(error != FC2_ERROR_OK)
						break;

					fc2Image convertImage;
					fc2CreateImage(&convertImage);
					error = fc2ConvertImageTo(FC2_PIXEL_FORMAT_BGR, &image, &convertImage);
					if(error != FC2_ERROR_OK)
						break;

					memcpy(pCurrentBuffer, (BYTE*)convertImage.pData, i64ImageSizeByte);

					CEventHandlerManager::BroadcastOnAcquisition(pCurrentImage, pInstance->GetObjectID(), pInstance->GetImageView(), pInstance);

					++pInstance->m_nGrabCount;
				}
				while(pInstance->IsInitialized() && (pInstance->IsLive() || nCount > pInstance->m_nGrabCount));
			}
		}
		while(false);

		error = fc2DestroyImage(&image);
		if(error != FC2_ERROR_OK)
			CLogManager::Write(0, _T("%s"), _T("fc2Destroy Error"));

		pInstance->m_bIsLive = false;
		pInstance->m_bIsGrabAvailable = true;
	}

	return 0;
}

void CDevicePointGrey::SetUpdateParameter()
{
	unsigned int nTempNum = 0; // Int형 Get 변수

	fc2Mode Mode;
	if(fc2GetGigEImagingMode(m_pContext, &Mode) == FC2_ERROR_OK)
		SetFormatMode((EDevicePointGreyImagemode)Mode);

	fc2GigEImageSettings GigeImageSettingFormat;

	if(fc2GetGigEImageSettings(m_pContext, &GigeImageSettingFormat) == FC2_ERROR_OK)
	{
		SetCanvasWidth(GigeImageSettingFormat.width);
		SetCanvasHeight(GigeImageSettingFormat.height);
		SetOffsetX(GigeImageSettingFormat.offsetX);
		SetOffsetY(GigeImageSettingFormat.offsetY);
		GetStringToFeatureIndex(IntegerToString(GigeImageSettingFormat.pixelFormat), g_lpszPixelFormatValue, nTempNum);
		SetPixelFormat(EDevicePointGreyPixelFormat(nTempNum));
	}

	fc2Config Config;

	if(fc2GetConfiguration(m_pContext, &Config) == FC2_ERROR_OK)
	{
		SetGrabWaitingTime(Config.grabTimeout);
		SetGrabMode((EDevicePointGreyGrabMode)Config.grabMode);
		SetGrabNumBuffers(Config.numBuffers);
	}

	fc2GigEProperty GigEProperty;
	GigEProperty.propType = PACKET_SIZE;
	if(fc2GetGigEProperty(m_pContext, &GigEProperty) == FC2_ERROR_OK)
		SetPacketSize(GigEProperty.value);

	GigEProperty.propType = PACKET_DELAY;
	if(fc2GetGigEProperty(m_pContext, &GigEProperty) == FC2_ERROR_OK)
		SetPacketDelay(GigEProperty.value);

	fc2Property Property;
	for(unsigned int i = 0; i < 15; ++i)
	{
		if(i == 14) // FRAME_RATE
			i = 16;

		Property.type = (fc2PropertyType)i;
		Property.onOff = false;

		if(fc2GetProperty(m_pContext, &Property) == FC2_ERROR_OK)
		{
			if(i == 0)
				SetBrightnessValue(Property.valueA);
			else if(i == 1)
				SetExposureValue(Property.valueA);
			else if(i == 2)
				SetSharpnessValue(Property.valueA);
			else if(i == 3)
				SetWhiteBalanceValue(Property.valueA);
			else if(i == 4)
				SetHueValue(Property.valueA);
			else if(i == 5)
				SetSaturationValue(Property.valueA);
			else if(i == 6)
				SetGammaValue(Property.valueA);
			else if(i == 7)
				SetIrisValue(Property.valueA);
			else if(i == 8)
				SetFocusValue(Property.valueA);
			else if(i == 9)
				SetZoomValue(Property.valueA);
			else if(i == 10)
				SetPanValue(Property.valueA);
			else if(i == 11)
				SetTiltValue(Property.valueA);
			else if(i == 12)
				SetShutterValue(Property.valueA);
			else if(i == 13)
				SetGainValue(Property.valueA);
			else if(i == 16)
				SetFrameRateValue(Property.valueA);
		}
	}

	SaveSettings();
}

#endif