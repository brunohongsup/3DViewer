#include "stdafx.h"

#include "DeviceEuresysGrablink.h"

#ifdef USING_DEVICE

#include "../RavidCore/RavidRect.h"
#include "../RavidCore/RavidImage.h"
#include "../RavidCore/RavidDatabase.h"
#include "../RavidCore/RavidRecordset.h"
#include "../RavidCore/PerformanceCounter.h"

#include "../RavidFramework/UIManager.h"
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

static LPCTSTR g_lpszParamGrablink[EDeviceParameterEuresysGrablink_Count - EDeviceParameterEuresysGrablinkFull_Count] =
{
	_T("SummaryControl"),
	_T("AcquisitionMode"),
	_T("TrigMode"),
	_T("Expose_us"),
	_T("AreaCamera"),
	_T("SeqLength_Fr"),
	_T("PhaseLength_Fr"),
	_T("LineCamera"),
	_T("SeqLength_Ln"),
	_T("SeqLength_Pg"),
	_T("PageLength_Ln"),
	_T("LineCaptureMode"),
	_T("LineRateMode"),
	_T("CameraSpecification"),
	_T("CamConfig"),
	_T("Camera"),
	_T("DataLink"),
	_T("Imaging"),
	_T("Spectrum"),
	_T("CameraTiming"),
	_T("ExposeRecovery_us"),
	_T("FrameRate_mHz"),
	_T("Hactive_Px"),
	_T("HsyncAft_Tk"),
	_T("PixelClkMode"),
	_T("ReadoutRecovery_us"),
	_T("Vactive_Ln"),
	_T("VsyncAft_Ln"),
	_T("LineRate_Hz"),
	_T("CameraFeatures"),
	_T("TapConfiguration"),
	_T("AuxResetCtl"),
	_T("CC1USage"),
	_T("CC2Usage"),
	_T("CC3Usage"),
	_T("CC4Usage"),
	_T("ColorMethod"),
	_T("ColorRegistrationControl"),
	_T("DvalMode"),
	_T("Expose"),
	_T("ExposeMin_us"),
	_T("ExposeMax_us"),
	_T("ExposeOverlap"),
	_T("FvalMode"),
	_T("LvalMode"),
	_T("Readout"),
	_T("ResetCtl"),
	_T("ResetEdge"),
	_T("TwoLineSynchronization"),
	_T("TapGeometry"),
	_T("CableFeatures"),
	_T("AuxresetLine"),
	_T("ResetLine"),
	_T("AcquisitionControl"),
	_T("ActivityLength"),
	_T("BreakEffect"),
	_T("EndTrigMode"),
	_T("NextTrigMode"),
	_T("SynchronizedAcquisition"),
	_T("TriggerControl"),
	_T("ForceTrig"),
	_T("NextTrigDelay_Pls"),
	_T("TrigCtl"),
	_T("TrigDelay_Pls"),
	_T("TrigDelay_us"),
	_T("TrigEdge"),
	_T("TrigFilter"),
	_T("TrigLine"),
	_T("PageDelay_Ln"),
	_T("ExposureControl"),
	_T("ExposeTrim"),
	_T("StrobeControl"),
	_T("StrobeCtl"),
	_T("StrobeDur"),
	_T("StrobeMode"),
	_T("StrobePos"),
	_T("PreStrobe_us"),
	_T("EncoderControl"),
	_T("LineTrigCtl"),
	_T("LineTrigEdge"),
	_T("LineTrigFilter"),
	_T("RateDivisionFactor"),
	_T("LineTrigLine"),
	_T("RateDividerRestart"),
	_T("GrabberConfiguration"),
	_T("ECCO_PLLResetControl"),
	_T("ECCO_SkewCompensation"),
	_T("FvalMin_Tk"),
	_T("LvalMin_Tk"),
	_T("MetadataInsertion"),
	_T("GrabberTiming"),
	_T("GrabWindow"),
	_T("OffsetX_Px"),
	_T("WindowX_Px"),
	_T("Cluster"),
	_T("ColorFormat"),
	_T("ImageFlipX"),
	_T("ImageFlipY"),
	_T("RedBlueSwap"),
	_T("MaxFillingSurfaces"),
	_T("SurfaceCount"),
	_T("ExceptionManagement"),
	_T("AcquisitionCleanup"),
	_T("AcqTimeout_ms"),
	_T("ParamsConsistencyCheck"),
	_T("TriggerSkipHold"),
	_T("Signaling"),
	_T("CallbackPriority"),
	_T("InterleavedAcquisitionCategory"),
	_T("InterleavedAcquisition"),
	_T("ExposureTime_P1_us"),
	_T("ExposureTime_P2_us"),
	_T("StrobeDuration_P1_us"),
	_T("StrobeDuration_P2_us"),
	_T("StrobeDelay_P1_us"),
	_T("StrobeDelay_P2_us"),
	_T("ExposureDelay_MAN_P1_us"),
	_T("ExposureDelay_MAN_P2_us"),
};

static LPCTSTR g_lpszGrablinkAcquisitionMode[EDeviceGrablinkAcquisitionMode_Count] =
{
	_T("HFR"),
	_T("INVALID"),
	_T("LONGPAGE"),
	_T("PAGE"),
	_T("SNAPSHOT"),
	_T("VIDEO"),
	_T("WEB"),
};

static LPCTSTR g_lpszGrablinkTrigMode[EDeviceGrablinkTrigMode_Count] =
{
	_T("AUTO"),
	_T("COMBINED"),
	_T("ENC"),
	_T("EXT"),
	_T("EXTRC"),
	_T("HARD"),
	_T("IMMEDIATE"),
	_T("INT"),
	_T("MASTER_CHANNEL"),
	_T("PAUSE"),
	_T("SLAVE"),
	_T("SOFT"),
};

static LPCTSTR g_lpszGrablinkLineCaptureMode[EDeviceGrablinkLineCaptureMode_Count] =
{
	_T("ADR"),
	_T("ALL"),
	_T("PICK"),
	_T("TAG"),
};

static LPCTSTR g_lpszGrablinkLineRateMode[EDeviceGrablinkLineRateMode_Count] =
{
	_T("CAMERA"),
	_T("CONVERT"),
	_T("EXPOSE"),
	_T("MASTER_CHANNEL"),
	_T("PERIOD"),
	_T("PULSE"),
	_T("SLAVE"),
};

static LPCTSTR g_lpszGrablinkCamConfig[EDeviceGrablinkCamConfig_Count] =
{
	_T("FACTORY"),
	_T("I10SC"),
	_T("I120RA"),
	_T("I120SA"),
	_T("I120SM"),
	_T("I15RA"),
	_T("I15SA"),
	_T("I25SA"),
	_T("I30SA"),
	_T("I50RA"),
	_T("I50SA"),
	_T("I50SM"),
	_T("I50SM_R"),
	_T("I60RA"),
	_T("I60SA"),
	_T("I60SM"),
	_T("I60SM_R"),
	_T("I7RA"),
	_T("I7SA"),
	_T("IxxRM"),
	_T("IxxRM_4"),
	_T("IxxSA"),
	_T("IxxSA_D"),
	_T("IxxSC"),
	_T("IxxSD"),
	_T("IxxSM"),
	_T("IxxSM_L"),
	_T("IxxSM_R"),
	_T("L1024RC"),
	_T("L1024RG"),
	_T("L1024RG_Base"),
	_T("L1024RG_Medium"),
	_T("L1024RGM"),
	_T("L1024RP"),
	_T("L1024SC"),
	_T("L1024SP"),
	_T("L12500RP"),
	_T("L1365RG"),
	_T("L2048RC"),
	_T("L2048RG"),
	_T("L2048RG_Base"),
	_T("L2048RG_Medium"),
	_T("L2048RGM"),
	_T("L2048RP"),
	_T("L2048SC"),
	_T("L2048SP"),
	_T("L2098RC"),
	_T("L2098RG"),
	_T("L2098RP"),
	_T("L2098SC"),
	_T("L2098SP"),
	_T("L4080RC"),
	_T("L4080RG"),
	_T("L4080RP"),
	_T("L4080SC"),
	_T("L4080SP"),
	_T("L4096RC"),
	_T("L4096RG"),
	_T("L4096RGS"),
	_T("L4096RP"),
	_T("L4096SC"),
	_T("L4096SP"),
	_T("L512RC"),
	_T("L512RG"),
	_T("L512RP"),
	_T("L512SC"),
	_T("L512SP"),
	_T("L5150RP"),
	_T("L5150SP"),
	_T("L6144RC"),
	_T("L6144RG"),
	_T("L6144RP"),
	_T("L6144SC"),
	_T("L6144SP"),
	_T("L7400RP"),
	_T("L7400SP"),
	_T("L7500RC"),
	_T("L7500RG"),
	_T("L7500RP"),
	_T("L7500SC"),
	_T("L7500SP"),
	_T("L8192RC"),
	_T("L8192RG"),
	_T("L8192RP"),
	_T("L8192SC"),
	_T("L8192SP"),
	_T("LxxxxRC"),
	_T("LxxxxRG"),
	_T("LxxxxRG2"),
	_T("LxxxxRP"),
	_T("LxxxxSC"),
	_T("LxxxxSP"),
	_T("P08RG"),
	_T("P08SC"),
	_T("P100RA"),
	_T("P100RA_G"),
	_T("P100RC"),
	_T("P100SA"),
	_T("P100SC"),
	_T("P106RM_D"),
	_T("P106SM"),
	_T("P10RA"),
	_T("P10RC"),
	_T("P10RD"),
	_T("P10RG"),
	_T("P10RM"),
	_T("P10SA"),
	_T("P10SC"),
	_T("P10SD"),
	_T("P10SM"),
	_T("P117RG"),
	_T("P117SC"),
	_T("P120RA"),
	_T("P120RG"),
	_T("P120RM"),
	_T("P120RM_D"),
	_T("P120RM_G"),
	_T("P120SA"),
	_T("P120SC"),
	_T("P120SM"),
	_T("P12RA"),
	_T("P12RC"),
	_T("P12RD"),
	_T("P12RG"),
	_T("P12RM"),
	_T("P12SA"),
	_T("P12SC"),
	_T("P12SD"),
	_T("P12SM"),
	_T("P13SC"),
	_T("P150RG"),
	_T("P150SC"),
	_T("P15RA"),
	_T("P15RC"),
	_T("P15RD"),
	_T("P15RG"),
	_T("P15RM"),
	_T("P15SA"),
	_T("P15SC"),
	_T("P15SD"),
	_T("P15SM"),
	_T("P16RA"),
	_T("P16RA_G"),
	_T("P16RC"),
	_T("P16RD"),
	_T("P16RD_G"),
	_T("P16RG"),
	_T("P16RM"),
	_T("P16RM_G"),
	_T("P16SA"),
	_T("P16SC"),
	_T("P16SD"),
	_T("P16SM"),
	_T("P17RA"),
	_T("P17RA_G"),
	_T("P17RD"),
	_T("P17RD_G"),
	_T("P17RG"),
	_T("P17SA"),
	_T("P17SC"),
	_T("P17SD"),
	_T("P200RC"),
	_T("P200RG"),
	_T("P200SC"),
	_T("P21SC"),
	_T("P22RC"),
	_T("P22RG"),
	_T("P22SC"),
	_T("P240RA"),
	_T("P24RG"),
	_T("P24SC"),
	_T("P25RA"),
	_T("P25RA_G"),
	_T("P25RD"),
	_T("P25RG"),
	_T("P25SA"),
	_T("P25SC"),
	_T("P25SC_S10"),
	_T("P25SC_S8"),
	_T("P25SM_L"),
	_T("P27RC"),
	_T("P27SC"),
	_T("P30RA"),
	_T("P30RA_D"),
	_T("P30RA_G"),
	_T("P30RC"),
	_T("P30RD"),
	_T("P30RG"),
	_T("P30RG_Base"),
	_T("P30RG_Medium"),
	_T("P30RG_O"),
	_T("P30RG_O_Base"),
	_T("P30RG_O_Medium"),
	_T("P30RM"),
	_T("P30RM_2"),
	_T("P30RM_G"),
	_T("P30SA"),
	_T("P30SC"),
	_T("P30SC_Base"),
	_T("P30SC_Medium"),
	_T("P30SD"),
	_T("P30SM"),
	_T("P30SM_L"),
	_T("P32RG"),
	_T("P32SC"),
	_T("P33RG"),
	_T("P33SC"),
	_T("P34RG"),
	_T("P34SC"),
	_T("P400SM"),
	_T("P42RG"),
	_T("P42SC"),
	_T("P48RG"),
	_T("P48SC"),
	_T("P50"),
	_T("P50RA"),
	_T("P50RA_G"),
	_T("P50RD"),
	_T("P50RG"),
	_T("P50RG_DX"),
	_T("P50RM"),
	_T("P50RM_G"),
	_T("P50SA"),
	_T("P50SC"),
	_T("P50SC_D10"),
	_T("P50SC_D8"),
	_T("P50SD"),
	_T("P50SM"),
	_T("P50SM_L"),
	_T("P60RA"),
	_T("P60RA_D"),
	_T("P60RA_G"),
	_T("P60RC"),
	_T("P60RD"),
	_T("P60RD_G"),
	_T("P60RG"),
	_T("P60RM"),
	_T("P60RM_A"),
	_T("P60RM_D"),
	_T("P60RM_G"),
	_T("P60RM_O"),
	_T("P60RM_R"),
	_T("P60SA"),
	_T("P60SC"),
	_T("P60SD"),
	_T("P60SM"),
	_T("P60SM_L"),
	_T("P64RG"),
	_T("P64SC"),
	_T("P70RA"),
	_T("P70RM"),
	_T("P70SA"),
	_T("P70SM"),
	_T("P74RG"),
	_T("P74SC"),
	_T("P75RG"),
	_T("P75SC"),
	_T("P80RG"),
	_T("P80SC"),
	_T("P8RC"),
	_T("P8RG"),
	_T("P8SC"),
	_T("P94RG"),
	_T("P94SC"),
	_T("PxxRA"),
	_T("PxxRA_D"),
	_T("PxxRA_M"),
	_T("PxxRC"),
	_T("PxxRD"),
	_T("PxxRG"),
	_T("PxxRG_PartialScan"),
	_T("PxxRM_1"),
	_T("PxxRM_2"),
	_T("PxxRM_A"),
	_T("PxxRM_D"),
	_T("PxxSA"),
	_T("PxxSA_D"),
	_T("PxxSC"),
	_T("PxxSD"),
	_T("PxxSM"),
	_T("PxxSM_L"),
	_T("STD"),
	_T("TEST"),
};

static LPCTSTR g_lpszGrablinkCamera[EDeviceGrablinkCamera_Count] =
{
	_T("1000m"),
	_T("1000m_D"),
	_T("1000m_S"),
	_T("1600c"),
	_T("1600c_D"),
	_T("1600c_D_RGB"),
	_T("1600c_S"),
	_T("1600c_S_RGB"),
	_T("1600m"),
	_T("1600m_D"),
	_T("1600m_S"),
	_T("1M28_SA"),
	_T("2000c_D"),
	_T("2000m"),
	_T("2000m_D"),
	_T("2000m_S"),
	_T("2M30_SA"),
	_T("4000m"),
	_T("4000m_D"),
	_T("4000m_S"),
	_T("A102k"),
	_T("A201b"),
	_T("A202k"),
	_T("A301bc"),
	_T("A302b"),
	_T("A402k"),
	_T("A403k"),
	_T("A501k"),
	_T("AVIIVA"),
	_T("AViiVA_C2CL4010"),
	_T("AViiVA_M4CL6007"),
	_T("AViiVA_M4CL8007"),
	_T("AVIIVA0514"),
	_T("AVIIVA1014"),
	_T("AVIIVA2014"),
	_T("AVIIVA4010"),
	_T("CAMERA_CCIR"),
	_T("CAMERA_EIA"),
	_T("CAMERA_NTSC"),
	_T("CAMERA_PAL"),
	_T("CAMERA_sCCIR"),
	_T("CAMERA_sEIA"),
	_T("CAMERA_sNTSC"),
	_T("CAMERA_sPAL"),
	_T("CameraExposeAR"),
	_T("CCIR"),
	_T("COLIBRI1024CL"),
	_T("COLIBRI2048CL"),
	_T("CS3920"),
	_T("CS6910CL"),
	_T("CS8530"),
	_T("CS8531"),
	_T("CS8531D"),
	_T("CS8541D"),
	_T("CS8550D"),
	_T("CS8550i"),
	_T("CS8550i_01"),
	_T("CSB1100CL"),
	_T("CSB4000CL"),
	_T("CUSTOM"),
	_T("CV_A1"),
	_T("CV_A11"),
	_T("CV_A33"),
	_T("CV_A50C"),
	_T("CV_A50E"),
	_T("CV_M1"),
	_T("CV_M10C"),
	_T("CV_M10E"),
	_T("CV_M30"),
	_T("CV_M40"),
	_T("CV_M4CL"),
	_T("CV_M4CLp"),
	_T("CV_M50"),
	_T("CV_M50C"),
	_T("CV_M50E"),
	_T("CV_M77"),
	_T("CV_M7CL"),
	_T("CV_M7CLp"),
	_T("CV_M8CL"),
	_T("CV_M9CL"),
	_T("DT4100"),
	_T("EIA"),
	_T("Generic"),
	_T("Generic_Dual"),
	_T("Generic_RGB"),
	_T("GrabberExposeAR"),
	_T("HV_C20AEK"),
	_T("HV_C20AP"),
	_T("ICD_FA41"),
	_T("ICD_FA41E"),
	_T("IK_TF1"),
	_T("IK_TF5"),
	_T("IK_TF7"),
	_T("iMVS157"),
	_T("InterlaceFR"),
	_T("InterlaceRR"),
	_T("KP_F100BCL"),
	_T("KP_F120CL"),
	_T("KP_F200CL"),
	_T("KP_F2B"),
	_T("KP_F3"),
	_T("KP_F30"),
	_T("KP_F30x"),
	_T("KP_F3W"),
	_T("KP_M32P"),
	_T("KP120CL"),
	_T("L101k"),
	_T("L103k"),
	_T("L104k"),
	_T("L301bc"),
	_T("L301kc"),
	_T("L401k"),
	_T("L402k"),
	_T("L403k"),
	_T("LD3521PGx"),
	_T("LD3522PGx"),
	_T("LD3523PGx"),
	_T("LD3541PGx"),
	_T("LD3542PGx"),
	_T("LD3543PGx"),
	_T("LongExposeFR"),
	_T("MV_D1024"),
	_T("MV_D1024_160_CL_8"),
	_T("MV_D1024_80_CL_8"),
	_T("MV_D640"),
	_T("MV_D640_33_CL_10"),
	_T("MV_D640_66_CL_10"),
	_T("MV_D640C_33_CL_10"),
	_T("MV_D640C_66_CL_10"),
	_T("MyCameraLink"),
	_T("MyColorScanCameraLink"),
	_T("NoCamera"),
	_T("NSUF7400CL"),
	_T("NTSC"),
	_T("NUCL4096D"),
	_T("NUCL7500D"),
	_T("P2_2x_01k40"),
	_T("P2_2x_02k40"),
	_T("P2_2x_04k40"),
	_T("P2_2x_06k40"),
	_T("P2_2x_08k40"),
	_T("P2_4x_04k40"),
	_T("P2_4x_06k40"),
	_T("P2_4x_08k40"),
	_T("PAL"),
	_T("PC_640CL"),
	_T("PRICOLOR1024C"),
	_T("PRICOLOR2048C"),
	_T("ProgressiveFR"),
	_T("SI_M100"),
	_T("SIMA"),
	_T("SIMUL"),
	_T("SKC_131"),
	_T("SKC_141"),
	_T("SKC_141TC"),
	_T("SKCm141TC"),
	_T("STC_1000"),
	_T("STC_1100b"),
	_T("Std_BW_CCIR"),
	_T("Std_BW_EIA"),
	_T("Std_RGB_CCIR"),
	_T("Std_RGB_EIA"),
	_T("SU51"),
	_T("SU74"),
	_T("SVS084MFCL"),
	_T("SVS084MSCL"),
	_T("SVS085MFCL"),
	_T("SVS085MSCL"),
	_T("SVS204MFCL"),
	_T("SVS204MSCL"),
	_T("SVS282CSCL"),
	_T("SVS285MFCL"),
	_T("SVS285MSCL"),
	_T("TI_324A"),
	_T("TM_1020_15CL"),
	_T("TM_1320_15CL"),
	_T("TM_2016_8CL"),
	_T("TM_4000CL"),
	_T("TM_6702"),
	_T("TM_6710CL"),
	_T("TM_9701"),
	_T("TMC_1000"),
	_T("TMC_4000CL"),
	_T("TMC_6700"),
	_T("VCC_810"),
	_T("VCC_8350"),
	_T("VCC_8350CL"),
	_T("VCC_850A"),
	_T("VCC_8550"),
	_T("VCC_8550CL"),
	_T("VCC_870"),
	_T("VCC_870A"),
	_T("VCC_8750CL"),
	_T("VCC_8850CL"),
	_T("VCC_G22V31CL_PoCL"),
	_T("XC_003"),
	_T("XC_55"),
	_T("XC_56"),
	_T("XC_7500"),
	_T("XC_8500CE"),
	_T("XC_ES30"),
	_T("XC_ES30CE"),
	_T("XC_HR300"),
	_T("XC_HR50"),
	_T("XC_HR57"),
	_T("XC_HR58"),
	_T("XC_HR70"),
	_T("XC_ST50"),
	_T("XC_ST50CE"),
	_T("XC_ST70CE"),
	_T("XCL_U1000"),
	_T("XCL_U1000C"),
	_T("XCL_V500"),
	_T("XCL_X700"),
	_T("XCLmU1000"),
	_T("XCLmU1000C"),
	_T("XCLmV500"),
	_T("XCLmX700"),
	_T("YD5040"),
	_T("YD5060"),
};

static LPCTSTR g_lpszGrablinkDataLink[EDeviceGrablinkDataLink_Count] =
{
	_T("ANALOG"),
	_T("CAMERALINK"),
	_T("COMPOSITE"),
};

static LPCTSTR g_lpszGrablinkImaging[EDeviceGrablinkImaging_Count] =
{
	_T("AREA"),
	_T("LINE"),
	_T("TDI"),
};

static LPCTSTR g_lpszGrablinkSpectrum[EDeviceGrablinkSpectrum_Count] =
{
	_T("BW"),
	_T("COLOR"),
	_T("IR"),
};

static LPCTSTR g_lpszGrablinkPixelClkMode[EDeviceGrablinkPixelClkMode_Count] =
{
	_T("INTERMITTENT"),
	_T("PERMANENT"),
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

static LPCTSTR g_lpszGrablinkAuxResetCtl[EDeviceGrablinkAuxResetCtl_Count] =
{
	_T("DIFF"),
	_T("NONE"),
	_T("TTL"),
};

static LPCTSTR g_lpszGrablinkCC1Usage[EDeviceGrablinkCC1Usage_Count] =
{
	_T("ACQPHASE"),
	_T("ACQSEQUENCE"),
	_T("AUXRESET"),
	_T("DIN1"),
	_T("HIGH"),
	_T("IIN1"),
	_T("LOW"),
	_T("RESET"),
	_T("SOFT"),
};

static LPCTSTR g_lpszGrablinkCC2Usage[EDeviceGrablinkCC2Usage_Count] =
{
	_T("ACQPHASE"),
	_T("ACQSEQUENCE"),
	_T("AUXRESET"),
	_T("DIN2"),
	_T("HIGH"),
	_T("LOW"),
	_T("RESET"),
	_T("SOFT"),
};

static LPCTSTR g_lpszGrablinkCC3Usage[EDeviceGrablinkCC3Usage_Count] =
{
	_T("ACQPHASE"),
	_T("ACQSEQUENCE"),
	_T("AUXRESET"),
	_T("HIGH"),
	_T("IIN1"),
	_T("LOW"),
	_T("READOUTALLOWED"),
	_T("RESET"),
	_T("SOFT"),
};

static LPCTSTR g_lpszGrablinkCC4Usage[EDeviceGrablinkCC4Usage_Count] =
{
	_T("ACQPHASE"),
	_T("ACQSEQUENCE"),
	_T("AUXRESET"),
	_T("HIGH"),
	_T("LOW"),
	_T("RESET"),
	_T("SOFT"),
	_T("TRIGGERALLOWED"),
};

static LPCTSTR g_lpszGrablinkColorMethod[EDeviceGrablinkColorMethod_Count] =
{
	_T("BAYER"),
	_T("NONE"),
	_T("PRISM"),
	_T("RGB"),
	_T("TRILINEAR"),
};

static LPCTSTR g_lpszGrablinkColorRegistrationControl[EDeviceGrablinkColorRegistrationControl_Count] =
{
	_T("FVAL"),
	_T("NONE"),
};

static LPCTSTR g_lpszGrablinkDvalMode[EDeviceGrablinkDvalMode_Count] =
{
	_T("DC"),
	_T("DG"),
	_T("DID2P0"),
	_T("DN"),
};

static LPCTSTR g_lpszGrablinkExpose[EDeviceGrablinkExpose_Count] =
{
	_T("EXTPRM"),
	_T("Fld"),
	_T("INTCTL"),
	_T("INTPRM"),
	_T("P1_Effective_us"),
	_T("P2_Effective_us"),
	_T("PLSCTL"),
	_T("PLSTRG"),
	_T("Tk"),
	_T("us"),
	_T("WIDTH"),
	_T("WIDTH_DX"),
};

static LPCTSTR g_lpszGrablinkExposeOverlap[EDeviceGrablinkExposeOverlap_Count] =
{
	_T("ALLOW"),
	_T("FORBID"),
};

static LPCTSTR g_lpszGrablinkFvalMode[EDeviceGrablinkFvalMode_Count] =
{
	_T("FA"),
	_T("FC"),
	_T("FN"),
	_T("LA"),
	_T("PC"),
};

static LPCTSTR g_lpszGrablinkLvalMode[EDeviceGrablinkLvalMode_Count] =
{
	_T("DG"),
	_T("LA"),
	_T("LB"),
	_T("LC"),
	_T("LN"),
	_T("LP"),
};

static LPCTSTR g_lpszGrablinkReadout[EDeviceGrablinkReadout_Count] =
{
	_T("100ns"),
	_T("INTCTL"),
	_T("PLSTRG"),
	_T("us"),
	_T("WIDTH"),
};

static LPCTSTR g_lpszGrablinkResetCtl[EDeviceGrablinkResetCtl_Count] =
{
	_T("DIFF"),
	_T("NONE"),
	_T("TTL"),
};

static LPCTSTR g_lpszGrablinkResetEdge[EDeviceGrablinkResetEdge_Count] =
{
	_T("GOHIGH"),
	_T("GOLOW"),
};

static LPCTSTR g_lpszGrablinkTwoLineSynchronization[EDeviceGrablinkTwoLineSynchronization_Count] =
{
	_T("DISABLE"),
	_T("ENABLE"),
};

static LPCTSTR g_lpszGrablinkTapGeometry[EDeviceGrablinkTapGeometry_Count] =
{
	_T("10X"),
	_T("10X_1Y"),
	_T("1X"),
	_T("1X_1Y"),
	_T("1X_1Y2"),
	_T("1X_2Y"),
	_T("1X_2YE"),
	_T("1X10"),
	_T("1X10_1Y"),
	_T("1X2"),
	_T("1X2_1Y"),
	_T("1X2_1Y2"),
	_T("1X2_2YE"),
	_T("1X3"),
	_T("1X3_1Y"),
	_T("1X3_1Y2"),
	_T("1X3_2YE"),
	_T("1X4"),
	_T("1X4_1Y"),
	_T("1X4_1Y2"),
	_T("1X4_2YE"),
	_T("1X8"),
	_T("1X8_1Y"),
	_T("2X"),
	_T("2X_1Y"),
	_T("2X_1Y2"),
	_T("2X_2YE"),
	_T("2X2"),
	_T("2X2_1Y"),
	_T("2X2_1Y2"),
	_T("2X2_2YE"),
	_T("2X2E"),
	_T("2X2E_1Y"),
	_T("2X2E_1Y2"),
	_T("2X2E_2YE"),
	_T("2X2M"),
	_T("2X2M_1Y"),
	_T("2X2M_1Y2"),
	_T("2X2M_2YE"),
	_T("2X4"),
	_T("2X4_1Y"),
	_T("2XE"),
	_T("2XE_1Y"),
	_T("2XE_1Y2"),
	_T("2XE_2YE"),
	_T("2XM"),
	_T("2XM_1Y"),
	_T("2XM_1Y2"),
	_T("2XM_2YE"),
	_T("2XR"),
	_T("2XR_1Y"),
	_T("2XR_1Y2"),
	_T("2XR_2YE"),
	_T("3X"),
	_T("3X_1Y"),
	_T("3X_1Y2"),
	_T("3X_1Y3"),
	_T("3X_2YE"),
	_T("4X"),
	_T("4X_1Y"),
	_T("4X_1Y2"),
	_T("4X_2YE"),
	_T("4X2"),
	_T("4X2_1Y"),
	_T("4X2E"),
	_T("4X2E_1Y"),
	_T("4XE"),
	_T("4XE_1Y"),
	_T("4XE_1Y2"),
	_T("4XE_2YE"),
	_T("4XR"),
	_T("4XR_1Y"),
	_T("4XR_1Y2"),
	_T("4XR_2YE"),
	_T("8X"),
	_T("8X_1Y"),
	_T("8XR"),
	_T("8XR_1Y"),
	_T("Y"),
	_T("Y_1Y"),
	_T("Y_1Y2"),
	_T("Y_1Y3"),
	_T("Y_2YE"),
};

static LPCTSTR g_lpszGrablinkAuxresetLine[EDeviceGrablinkAuxresetLine_Count] =
{
	_T("CC1"),
	_T("CC2"),
	_T("CC3"),
	_T("CC4"),
	_T("CC5"),
	_T("DUAL"),
	_T("EXP"),
	_T("Feature"),
	_T("Feature_Available"),
	_T("Feature_NotAvailable"),
	_T("GATE"),
	_T("HIO"),
	_T("NC"),
	_T("RST"),
	_T("VIO"),
};

static LPCTSTR g_lpszGrablinkResetLine[EDeviceGrablinkResetLine_Count] =
{
	_T("CC1"),
	_T("CC2"),
	_T("CC3"),
	_T("CC4"),
	_T("CC5"),
	_T("DUAL"),
	_T("EXP"),
	_T("GATE"),
	_T("HIO"),
	_T("NC"),
	_T("RST"),
	_T("VIO"),
};

static LPCTSTR g_lpszGrablinkBreakEffect[EDeviceGrablinkBreakEffect_Count] =
{
	_T("ABORT"),
	_T("FINISH"),
};

static LPCTSTR g_lpszGrablinkEndTrigMode[EDeviceGrablinkEndTrigMode_Count] =
{
	_T("AUTO"),
	_T("HARD"),
	_T("MASTER_CHANNEL"),
	_T("SLAVE"),
};

static LPCTSTR g_lpszGrablinkNextTrigMode[EDeviceGrablinkNextTrigMode_Count] =
{
	_T("COMBINED"),
	_T("HARD"),
	_T("IMMEDIATE"),
	_T("NextTrigMode"),
	_T("PAUSE"),
	_T("PERIODIC"),
	_T("REPEAT"),
	_T("SAME"),
	_T("SLAVE"),
	_T("SOFT"),
	_T("TIMER"),
};

static LPCTSTR g_lpszGrablinkSynchronizedAcquisition[EDeviceGrablinkSynchronizedAcquisition_Count] =
{
	_T("INTERNAL_MAST"),
	_T("INTERNAL_SLAV"),
	_T("LOCAL_MASTER"),
	_T("LOCAL_SLAVE"),
	_T("MASTER"),
	_T("OFF"),
	_T("SLAVE"),
};

static LPCTSTR g_lpszGrablinkForceTrig[EDeviceGrablinkForceTrig_Count] =
{
	_T("ENDTRIG"),
	_T("TRIG"),
};

static LPCTSTR g_lpszGrablinkTrigCtl[EDeviceGrablinkTrigCtl_Count] =
{
	_T("12V"),
	_T("5V"),
	_T("AUTO"),
	_T("CAMERA"),
	_T("DIFF"),
	_T("DTTL"),
	_T("I12V"),
	_T("ISO"),
	_T("ITTL"),
	_T("LVDS"),
	_T("RELAY"),
	_T("RS485"),
	_T("TGOHIGH"),
	_T("TGOLOW"),
	_T("TTL"),
	_T("TTLHiz"),
	_T("TTLPD"),
	_T("VGOHIGH"),
	_T("VGOLOW"),
};

static LPCTSTR g_lpszGrablinkTrigEdge[EDeviceGrablinkTrigEdge_Count] =
{
	_T("GOHIGH"),
	_T("GOLOW"),
	_T("GOOPEN"),
	_T("HIGH"),
	_T("LOW"),
	_T("OPEN"),
};

static LPCTSTR g_lpszGrablinkTrigFilter[EDeviceGrablinkTrigFilter_Count] =
{
	_T("FILTER_100ms"),
	_T("FILTER_10ms"),
	_T("MEDIUM"),
	_T("OFF"),
	_T("ON"),
	_T("STRONG"),
};

static LPCTSTR g_lpszGrablinkTrigLine[EDeviceGrablinkTrigLine_Count] =
{
	_T("ALT1"),
	_T("ALT2"),
	_T("ALT3"),
	_T("ALT4"),
	_T("CAM"),
	_T("DIN1"),
	_T("DIN2"),
	_T("Filte"),
	_T("FVAL"),
	_T("I1"),
	_T("I2"),
	_T("I3"),
	_T("I4"),
	_T("IIN1"),
	_T("IIN2"),
	_T("IIN3"),
	_T("IIN4"),
	_T("IO"),
	_T("IO1"),
	_T("IO2"),
	_T("IO3"),
	_T("IO4"),
	_T("ISOA1"),
	_T("ISOA2"),
	_T("LTACT"),
	_T("NOM"),
	_T("NONE"),
	_T("PG"),
	_T("TR"),
	_T("TRA"),
	_T("TRA1"),
	_T("TRA2"),
	_T("TRB"),
	_T("TRC"),
	_T("TRD"),
	_T("TRG"),
	_T("TRX"),
	_T("TRY"),
	_T("TRZ"),
};

static LPCTSTR g_lpszGrablinkStrobeCtl[EDeviceGrablinkStrobeCtl_Count] =
{
	_T("DIFF"),
	_T("IOC"),
	_T("IOE"),
	_T("ITTL"),
	_T("NONE"),
	_T("OPTO"),
	_T("SSRLY"),
	_T("TTL"),
};

static LPCTSTR g_lpszGrablinkStrobeMode[EDeviceGrablinkStrobeMode_Count] =
{
	_T("AUTO"),
	_T("MAN"),
	_T("NONE"),
	_T("OFF"),
};

static LPCTSTR g_lpszGrablinkLineTrigCtl[EDeviceGrablinkLineTrigCtl_Count] =
{
	_T("DIFF"),
	_T("DIFF_PAIRED"),
	_T("I12V"),
	_T("ISO"),
	_T("ISO_PAIRED"),
	_T("ITTL"),
	_T("LVDS"),
	_T("TTL"),
};

static LPCTSTR g_lpszGrablinkLineTrigEdge[EDeviceGrablinkLineTrigEdge_Count] =
{
	_T("ALL_A"),
	_T("ALL_A_B"),
	_T("FALLING_A"),
	_T("GOHIGH"),
	_T("GOLOW"),
	_T("RISING_A"),
};

static LPCTSTR g_lpszGrablinkLineTrigFilter[EDeviceGrablinkLineTrigFilter_Count] =
{
	_T("Filter_100ns"),
	_T("Filter_10us"),
	_T("Filter_1us"),
	_T("Filter_200ns"),
	_T("Filter_40ns"),
	_T("Filter_500ns"),
	_T("Filter_5us"),
	_T("MEDIUM"),
	_T("OFF"),
	_T("STRONG"),
};

static LPCTSTR g_lpszGrablinkLineTrigLine[EDeviceGrablinkLineTrigLine_Count] =
{
	_T("CAM"),
	_T("DIN1"),
	_T("DIN1_DIN2"),
	_T("DIN2"),
	_T("IIN1"),
	_T("IIN1_IIN2"),
	_T("IIN2"),
	_T("IIN3"),
	_T("IIN3_IIN4"),
	_T("IIN4"),
	_T("IO1"),
	_T("IO2"),
	_T("IO3"),
	_T("IO4"),
	_T("ISOA1"),
	_T("ISOA2"),
	_T("NOM"),
	_T("TRA1"),
	_T("TRA2"),
	_T("TRG"),
	_T("TRX"),
	_T("TRY"),
	_T("TRZ"),
};

static LPCTSTR g_lpszGrablinkRateDividerRestart[EDeviceGrablinkRateDividerRestart_Count] =
{
	_T("NEVER"),
	_T("START_OF_SCAN"),
};

static LPCTSTR g_lpszGrablinkConnectLoc[EDeviceGrablinkConnectLoc_Count] =
{
	_T("BOTH"),
	_T("CENTER"),
	_T("FOURTH"),
	_T("LEFTMOST"),
	_T("LOWER"),
	_T("MIDDLELEFT"),
	_T("MIDDLERIGHT"),
	_T("RIGHTMOST"),
	_T("SECOND"),
	_T("THIRD"),
	_T("UPPER"),
};

static LPCTSTR g_lpszGrablinkECCO_PLLResetControl[EDeviceGrablinkECCO_PLLResetControl_Count] =
{
	_T("AUTOMATIC"),
	_T("CHANNEL_ACTIVATI"),
	_T("CHANNEL_ACTIVE"),
};

static LPCTSTR g_lpszGrablinkECCO_SkewCompensation[EDeviceGrablinkECCO_SkewCompensation_Count] =
{
	_T("OFF"),
	_T("ON"),
};

static LPCTSTR g_lpszGrablinkFvalMin_Tk[EDeviceGrablinkFvalMin_Tk_Count] =
{
	_T("1"),
	_T("3"),
};

static LPCTSTR g_lpszGrablinkLvalMin_Tk[EDeviceGrablinkLvalMin_Tk_Count] =
{
	_T("1"),
	_T("2"),
};

static LPCTSTR g_lpszGrablinkMetadataContent[EDeviceGrablinkMetadataContent_Count] =
{
	_T("NONE"),
	_T("ONE_FIELD"),
	_T("THREE_FIELD"),
	_T("TWO_FIELD"),
};

static LPCTSTR g_lpszGrablinkMetadataInsertion[EDeviceGrablinkMetadataInsertion_Count] =
{
	_T("DISABLE"),
	_T("ENABLE"),
};

static LPCTSTR g_lpszGrablinkMetadataLocation[EDeviceGrablinkMetadataLocation_Count] =
{
	_T("EFT"),
	_T("EFT_BOARDER"),
	_T("VALRISE"),
	_T("PARSE_1"),
	_T("AP10"),
};

static LPCTSTR g_lpszGrablinkGrabWindow[EDeviceGrablinkGrabWindow_Count] =
{
	_T("FIT"),
	_T("MAN"),
	_T("NOBLACK"),
	_T("NOLOSS"),
	_T("STD"),
	_T("WOI"),
	_T("WOI_NO"),
	_T("WOI_YES"),
};

static LPCTSTR g_lpszGrablinkColorFormat[EDeviceGrablinkColorFormat_Count] =
{
	_T("ARGB32"),
	_T("ARGB64"),
	_T("BAYER10"),
	_T("BAYER12"),
	_T("BAYER14"),
	_T("BAYER16"),
	_T("BAYER8"),
	_T("BGR24"),
	_T("BGR32"),
	_T("DX50"),
	_T("FLOAT32"),
	_T("GRABBER"),
	_T("I420"),
	_T("IYU2"),
	_T("IYUV"),
	_T("JPEG"),
	_T("MJPG"),
	_T("MP4S"),
	_T("RAW10"),
	_T("RAW12"),
	_T("RAW14"),
	_T("RAW16"),
	_T("RAW8"),
	_T("RGB10_10_10"),
	_T("RGB10_12_10"),
	_T("RGB15"),
	_T("RGB15Y8"),
	_T("RGB16"),
	_T("RGB16Y8"),
	_T("RGB24"),
	_T("RGB24PL"),
	_T("RGB24PLY10"),
	_T("RGB24PLY12"),
	_T("RGB24PLY16"),
	_T("RGB24PLY8"),
	_T("RGB24Y10"),
	_T("RGB24Y12"),
	_T("RGB24Y16"),
	_T("RGB24Y8"),
	_T("RGB30"),
	_T("RGB30PL"),
	_T("RGB30PLY10"),
	_T("RGB30PLY12"),
	_T("RGB30PLY16"),
	_T("RGB30PLY8"),
	_T("RGB32"),
	_T("RGB36"),
	_T("RGB36PL"),
	_T("RGB36PLY10"),
	_T("RGB36PLY12"),
	_T("RGB36PLY16"),
	_T("RGB36PLY8"),
	_T("RGB42"),
	_T("RGB42PL"),
	_T("RGB48"),
	_T("RGB48PL"),
	_T("RGB48PLY10"),
	_T("RGB48PLY12"),
	_T("RGB48PLY16"),
	_T("RGB48PLY8"),
	_T("RGB64"),
	_T("UINT32"),
	_T("Y10"),
	_T("Y12"),
	_T("Y14"),
	_T("Y16"),
	_T("Y41B"),
	_T("Y41P"),
	_T("Y42B"),
	_T("Y42P"),
	_T("Y8"),
	_T("YRGB32"),
	_T("YRGB64"),
	_T("YUV411"),
	_T("YUV411PL"),
	_T("YUV411PL_DEC"),
	_T("YUV422"),
	_T("YUV422PL"),
	_T("YUV422PL_DEC"),
	_T("YUV444"),
	_T("YUV444PL"),
	_T("YUV9"),
	_T("YV12"),
	_T("YVU9"),
};

static LPCTSTR g_lpszGrablinkImageFlipX[EDeviceGrablinkImageFlipX_Count] =
{
	_T("OFF"),
	_T("ON"),
};

static LPCTSTR g_lpszGrablinkImageFlipY[EDeviceGrablinkImageFlipY_Count] =
{
	_T("OFF"),
	_T("ON"),
};

static LPCTSTR g_lpszGrablinkMaxFillingSurfaces[EDeviceGrablinkMaxFillingSurfaces_Count] =
{
	_T("MAXIMUM"),
	_T("MINIMUM"),
};

static LPCTSTR g_lpszGrablinkRedBlueSwap[EDeviceGrablinkRedBlueSwap_Count] =
{
	_T("DISABLE"),
	_T("ENABLE"),
};

static LPCTSTR g_lpszGrablinkAcquisitionCleanup[EDeviceGrablinkAcquisitionCleanup_Count] =
{
	_T("DISABLED"),
	_T("ENABLED"),
};

static LPCTSTR g_lpszGrablinkParamsConsistencyCheck[EDeviceGrablinkParamsConsistencyCheck_Count] =
{
	_T("AUTO"),
	_T("MANUAL"),
};

static LPCTSTR g_lpszGrablinkTriggerSkipHold[EDeviceGrablinkTriggerSkipHold_Count] =
{
	_T("HOLD"),
	_T("SKIP"),
};

static LPCTSTR g_lpszGrablinkCallbackPriority[EDeviceGrablinkCallbackPriority_Count] =
{
	_T("ABOVE_NORMAL"),
	_T("BELOW_NORMAL"),
	_T("HIGHEST"),
	_T("LOWEST"),
	_T("NORMAL"),
	_T("TIME_CRITICAL"),
};

static LPCTSTR g_lpszGrablinkInterleavedAcquisition[EDeviceGrablinkInterleavedAcquisition_Count] =
{
	_T("OFF"),
	_T("ON"),
};

CDeviceEuresysGrablink::CDeviceEuresysGrablink()
{
}


CDeviceEuresysGrablink::~CDeviceEuresysGrablink()
{
}

EDeviceInitializeResult CDeviceEuresysGrablink::Initialize()
{
	return __super::Initialize();
}

EDeviceTerminateResult CDeviceEuresysGrablink::Terminate()
{
	return __super::Terminate();
}

bool CDeviceEuresysGrablink::LoadSettings()
{
	bool bReturn = false;
	
	do 
	{
		AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_SummaryControl, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_SummaryControl - EDeviceParameterEuresysGrablinkFull_Count], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		{
			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_AcquisitionMode, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_AcquisitionMode - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkAcquisitionMode, EDeviceGrablinkAcquisitionMode_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_TrigMode, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_TrigMode - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkTrigMode, EDeviceGrablinkTrigMode_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_Expose_us, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_Expose_us - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_AreaCamera, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_AreaCamera - EDeviceParameterEuresysGrablinkFull_Count], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
			{
				AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_SeqLength_Fr, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_SeqLength_Fr - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);

				AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_PhaseLength_Fr, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_PhaseLength_Fr - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
			}

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_LineCamera, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_LineCamera - EDeviceParameterEuresysGrablinkFull_Count], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
			{
				AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_SeqLength_Ln, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_SeqLength_Ln - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);

				AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_SeqLength_Pg, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_SeqLength_Pg - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);

				AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_PageLength_Ln, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_PageLength_Ln - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);

				AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_LineCaptureMode, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_LineCaptureMode - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkLineCaptureMode, EDeviceGrablinkLineCaptureMode_Count), nullptr, 2);

				AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_LineRateMode, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_LineRateMode - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkLineRateMode, EDeviceGrablinkLineRateMode_Count), nullptr, 2);
			}
		}


		AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_CameraSpecification, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_CameraSpecification - EDeviceParameterEuresysGrablinkFull_Count], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		{
			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_CamConfig, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_CamConfig - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkCamConfig, EDeviceGrablinkCamConfig_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_Camera, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_Camera - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkCamera, EDeviceGrablinkCamera_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_DataLink, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_DataLink - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkDataLink, EDeviceGrablinkDataLink_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_Imaging, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_Imaging - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkImaging, EDeviceGrablinkImaging_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_Spectrum, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_Spectrum - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkSpectrum, EDeviceGrablinkSpectrum_Count), nullptr, 1);
		}


		AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_CameraTiming, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_CameraTiming - EDeviceParameterEuresysGrablinkFull_Count], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		{
			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_ExposeRecovery_us, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_ExposeRecovery_us - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_FrameRate_mHz, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_FrameRate_mHz - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_Hactive_Px, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_Hactive_Px - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_HsyncAft_Tk, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_HsyncAft_Tk - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_PixelClkMode, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_PixelClkMode - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkPixelClkMode, EDeviceGrablinkPixelClkMode_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_ReadoutRecovery_us, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_ReadoutRecovery_us - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_Vactive_Ln, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_Vactive_Ln - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_VsyncAft_Ln, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_VsyncAft_Ln - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_LineRate_Hz, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_LineRate_Hz - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		}

		AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_CameraFeatures, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_CameraFeatures - EDeviceParameterEuresysGrablinkFull_Count], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		{
			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_TapConfiguration, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_TapConfiguration - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkTapConfiguration, EDeviceGrablinkTapConfiguration_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_AuxResetCtl, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_AuxResetCtl - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkAuxResetCtl, EDeviceGrablinkAuxResetCtl_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_CC1USage, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_CC1USage - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkCC1Usage, EDeviceGrablinkCC1Usage_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_CC2Usage, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_CC2Usage - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkCC2Usage, EDeviceGrablinkCC2Usage_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_CC3Usage, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_CC3Usage - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkCC3Usage, EDeviceGrablinkCC3Usage_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_CC4Usage, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_CC4Usage - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkCC4Usage, EDeviceGrablinkCC4Usage_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_ColorMethod, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_ColorMethod - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkColorMethod, EDeviceGrablinkColorMethod_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_ColorRegistrationControl, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_ColorRegistrationControl - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkColorRegistrationControl, EDeviceGrablinkColorRegistrationControl_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_DvalMode, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_DvalMode - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkDvalMode, EDeviceGrablinkDvalMode_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_Expose, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_Expose - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkExpose, EDeviceGrablinkExpose_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_ExposeMin_us, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_ExposeMin_us - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_ExposeMax_us, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_ExposeMax_us - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_ExposeOverlap, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_ExposeOverlap - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkExposeOverlap, EDeviceGrablinkExposeOverlap_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_FvalMode, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_FvalMode - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkFvalMode, EDeviceGrablinkFvalMode_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_LvalMode, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_LvalMode - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkLvalMode, EDeviceGrablinkLvalMode_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_Readout, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_Readout - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkReadout, EDeviceGrablinkReadout_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_ResetCtl, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_ResetCtl - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkResetCtl, EDeviceGrablinkResetCtl_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_ResetEdge, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_ResetEdge - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkResetEdge, EDeviceGrablinkResetEdge_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_TwoLineSynchronization, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_TwoLineSynchronization - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkTwoLineSynchronization, EDeviceGrablinkTwoLineSynchronization_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_TapGeometry, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_TapGeometry - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkTapGeometry, EDeviceGrablinkTapGeometry_Count), nullptr, 1);
		}

		AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_CableFeatures, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_CableFeatures - EDeviceParameterEuresysGrablinkFull_Count], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		{
			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_AuxresetLine, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_AuxresetLine - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkAuxresetLine, EDeviceGrablinkAuxresetLine_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_ResetLine, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_ResetLine - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkResetLine, EDeviceGrablinkResetLine_Count), nullptr, 1);
		}

		AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_AcquisitionControl, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_AcquisitionControl - EDeviceParameterEuresysGrablinkFull_Count], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		{
			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_ActivityLength, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_ActivityLength - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_BreakEffect, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_BreakEffect - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkBreakEffect, EDeviceGrablinkBreakEffect_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_EndTrigMode, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_EndTrigMode - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkEndTrigMode, EDeviceGrablinkEndTrigMode_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_NextTrigMode, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_NextTrigMode - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkNextTrigMode, EDeviceGrablinkNextTrigMode_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_SynchronizedAcquisition, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_SynchronizedAcquisition - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkSynchronizedAcquisition, EDeviceGrablinkSynchronizedAcquisition_Count), nullptr, 1);
		}

		AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_TriggerControl, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_TriggerControl - EDeviceParameterEuresysGrablinkFull_Count], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		{
			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_ForceTrig, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_ForceTrig - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkForceTrig, EDeviceGrablinkForceTrig_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_NextTrigDelay_Pls, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_NextTrigDelay_Pls - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_TrigCtl, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_TrigCtl - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkTrigCtl, EDeviceGrablinkTrigCtl_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_TrigDelay_Pls, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_TrigDelay_Pls - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_TrigDelay_us, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_TrigDelay_us - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_TrigEdge, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_TrigEdge - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkTrigEdge, EDeviceGrablinkTrigEdge_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_TrigFilter, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_TrigFilter - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkTrigFilter, EDeviceGrablinkTrigFilter_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_TrigLine, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_TrigLine - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkTrigLine, EDeviceGrablinkTrigLine_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_PageDelay_Ln, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_PageDelay_Ln - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		}

		AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_ExposureControl, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_ExposureControl - EDeviceParameterEuresysGrablinkFull_Count], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);

		AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_ExposeTrim, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_ExposeTrim - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_StrobeControl, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_StrobeControl - EDeviceParameterEuresysGrablinkFull_Count], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		{
			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_StrobeCtl, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_StrobeCtl - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkStrobeCtl, EDeviceGrablinkStrobeCtl_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_StrobeDur, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_StrobeDur - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_StrobeMode, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_StrobeMode - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkStrobeMode, EDeviceGrablinkStrobeMode_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_StrobePos, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_StrobePos - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_PreStrobe_us, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_PreStrobe_us - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		}
		//////////////////////////////////////////////////////////////////////////
		// only linescan

		AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_EncoderControl, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_EncoderControl - EDeviceParameterEuresysGrablinkFull_Count], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		{
			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_LineTrigCtl, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_LineTrigCtl - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkLineTrigCtl, EDeviceGrablinkLineTrigCtl_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_LineTrigEdge, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_LineTrigEdge - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkLineTrigEdge, EDeviceGrablinkLineTrigEdge_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_LineTrigFilter, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_LineTrigFilter - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkLineTrigFilter, EDeviceGrablinkLineTrigFilter_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_RateDivisionFactor, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_RateDivisionFactor - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_LineTrigLine, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_LineTrigLine - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkLineTrigLine, EDeviceGrablinkLineTrigLine_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_RateDividerRestart, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_RateDividerRestart - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkRateDividerRestart, EDeviceGrablinkRateDividerRestart_Count), nullptr, 1);
		}

		AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_GrabberConfiguration, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_GrabberConfiguration - EDeviceParameterEuresysGrablinkFull_Count], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		{
			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_ECCO_PLLResetControl, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_ECCO_PLLResetControl - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkECCO_PLLResetControl, EDeviceGrablinkECCO_PLLResetControl_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_ECCO_SkewCompensation, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_ECCO_SkewCompensation - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkECCO_SkewCompensation, EDeviceGrablinkECCO_SkewCompensation_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_FvalMin_Tk, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_FvalMin_Tk - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkFvalMin_Tk, EDeviceGrablinkFvalMin_Tk_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_LvalMin_Tk, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_LvalMin_Tk - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkLvalMin_Tk, EDeviceGrablinkLvalMin_Tk_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_MetadataInsertion, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_MetadataInsertion - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkMetadataInsertion, EDeviceGrablinkMetadataInsertion_Count), nullptr, 1);
		}

		AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_GrabberTiming, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_GrabberTiming - EDeviceParameterEuresysGrablinkFull_Count], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		{
			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_GrabWindow, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_GrabWindow - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkGrabWindow, EDeviceGrablinkGrabWindow_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_OffsetX_Px, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_OffsetX_Px - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_WindowX_Px, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_WindowX_Px - EDeviceParameterEuresysGrablinkFull_Count], _T("1024"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		}
		
		AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_Cluster, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_Cluster - EDeviceParameterEuresysGrablinkFull_Count], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		{
			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_ColorFormat, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_ColorFormat - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkColorFormat, EDeviceGrablinkColorFormat_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_ImageFlipX, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_ImageFlipX - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkImageFlipX, EDeviceGrablinkImageFlipX_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_ImageFlipY, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_ImageFlipY - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkImageFlipY, EDeviceGrablinkImageFlipY_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_RedBlueSwap, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_RedBlueSwap - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkRedBlueSwap, EDeviceGrablinkRedBlueSwap_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_MaxFillingSurfaces, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_MaxFillingSurfaces - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkMaxFillingSurfaces, EDeviceGrablinkMaxFillingSurfaces_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_SurfaceCount, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_SurfaceCount - EDeviceParameterEuresysGrablinkFull_Count], _T("4"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		}

		AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_ExceptionManagement, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_ExceptionManagement - EDeviceParameterEuresysGrablinkFull_Count], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		{
			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_AcquisitionCleanup, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_AcquisitionCleanup - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkAcquisitionCleanup, EDeviceGrablinkAcquisitionCleanup_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_AcqTimeout_ms, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_AcqTimeout_ms - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_ParamsConsistencyCheck, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_ParamsConsistencyCheck - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkParamsConsistencyCheck, EDeviceGrablinkParamsConsistencyCheck_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_TriggerSkipHold, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_TriggerSkipHold - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkTriggerSkipHold, EDeviceGrablinkTriggerSkipHold_Count), nullptr, 1);
		}

		AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_Signaling, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_Signaling - EDeviceParameterEuresysGrablinkFull_Count], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);

		AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_CallbackPriority, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_CallbackPriority - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkCallbackPriority, EDeviceGrablinkCallbackPriority_Count), nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_InterleavedAcquisitionCategory, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_InterleavedAcquisitionCategory - EDeviceParameterEuresysGrablinkFull_Count], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		{
			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_InterleavedAcquisition, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_InterleavedAcquisition - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszGrablinkInterleavedAcquisition, EDeviceGrablinkInterleavedAcquisition_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_ExposureTime_P1_us, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_ExposureTime_P1_us - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_ExposureTime_P2_us, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_ExposureTime_P2_us - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_StrobeDuration_P1_us, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_StrobeDuration_P1_us - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_StrobeDuration_P2_us, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_StrobeDuration_P2_us - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_StrobeDelay_P1_us, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_StrobeDelay_P1_us - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_StrobeDelay_P2_us, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_StrobeDelay_P2_us - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_ExposureDelay_MAN_P1_us, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_ExposureDelay_MAN_P1_us - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysGrablink_ExposureDelay_MAN_P2_us, g_lpszParamGrablink[EDeviceParameterEuresysGrablink_ExposureDelay_MAN_P2_us - EDeviceParameterEuresysGrablinkFull_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		}

		bReturn = true;
	} 
	while(false);

	return bReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetAcquisitionMode(EDeviceGrablinkAcquisitionMode * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam =  (EDeviceGrablinkAcquisitionMode)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_AcquisitionMode));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetAcquisitionMode(EDeviceGrablinkAcquisitionMode eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_AcquisitionMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkAcquisitionMode_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_AcquisitionMode, CStringA(g_lpszGrablinkAcquisitionMode[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkAcquisitionMode[nPreValue], g_lpszGrablinkAcquisitionMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetTrigMode(EDeviceGrablinkTrigMode * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkTrigMode)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_TrigMode));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetTrigMode(EDeviceGrablinkTrigMode eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_TrigMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkTrigMode_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_TrigMode, CStringA(g_lpszGrablinkTrigMode[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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
	 
	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID- EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkTrigMode[nPreValue], g_lpszGrablinkTrigMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetExpose_us(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysGrablink_Expose_us));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetExpose_us(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_Expose_us;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(McSetParamInt(m_hDevice, MC_Expose_us, nParam) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetSeqLength_Fr(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysGrablink_SeqLength_Fr));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetSeqLength_Fr(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_SeqLength_Fr;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamInt(m_hDevice, MC_SeqLength_Fr, nParam) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetPhaseLength_Fr(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysGrablink_PhaseLength_Fr));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetPhaseLength_Fr(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_PhaseLength_Fr;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamInt(m_hDevice, MC_PhaseLength_Fr, nParam) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetSeqLength_Ln(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysGrablink_SeqLength_Ln));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetSeqLength_Ln(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_SeqLength_Ln;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(!IsGrabAvailable() || IsLive())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamInt(m_hDevice, MC_ChannelState, MC_ChannelState_IDLE) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}

			if(McSetParamInt(m_hDevice, MC_SeqLength_Ln, nParam) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetSeqLength_Pg(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysGrablink_SeqLength_Pg));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetSeqLength_Pg(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_SeqLength_Pg;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamInt(m_hDevice, MC_SeqLength_Pg, nParam) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetPageLength_Ln(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysGrablink_PageLength_Ln));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetPageLength_Ln(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_PageLength_Ln;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsLive() || !IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_AlreadyGrabError;
			break;
		}

		if(IsInitialized())
		{
			bool bError = false;

			do 
			{
				if(McSetParamInt(m_hDevice, MC_ChannelState, MC_ChannelState_IDLE) != MC_OK)
				{
					eReturn = EEuresysSetFunction_WriteToDeviceError;
					break;
				}

				if(McSetParamInt(m_hDevice, MC_PageLength_Ln, nParam) != MC_OK)
				{
					eReturn = EEuresysSetFunction_WriteToDeviceError;
					break;
				}

				bError = true;

				int nWidth = -1, nHeight = -1;

				if(McGetParamInt(m_hDevice, MC_ImageSizeX, &nWidth) != MC_OK)
				{
					eReturn = EEuresysSetFunction_ReadOnDeviceError;
					break;
				}

				if(McGetParamInt(m_hDevice, MC_ImageSizeY, &nHeight) != MC_OK)
				{
					eReturn = EEuresysSetFunction_ReadOnDeviceError;
					break;
				}

				if(nWidth < 1 || nHeight < 1)
				{
					eReturn = EEuresysSetFunction_ReadOnDeviceError;
					break;
				}

				int nColor = -1;

				McGetParamInt(m_hDevice, MC_Spectrum, &nColor);

				if(nColor == MC_Spectrum_COLOR)
					m_bIsColor = true;
				else
					m_bIsColor = false;

				this->ClearBuffer();

				int64_t i64Channels = m_bIsColor ? 3 : 1;

				CMultipleVariable mv;
				for(int i = 0; i < i64Channels; ++i)
					mv.AddValue(0xff);

				this->InitBuffer(nWidth, nHeight, mv, CRavidImage::MakeValueFormat(i64Channels));
				this->ConnectImage();

				bError = false;
			} 
			while(false);
			
			if(bError)
				Terminate();
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetLineCaptureMode(EDeviceGrablinkLineCaptureMode * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkLineCaptureMode)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_LineCaptureMode));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetLineCaptureMode(EDeviceGrablinkLineCaptureMode eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_LineCaptureMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkLineCaptureMode_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_LineCaptureMode, CStringA(g_lpszGrablinkLineCaptureMode[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkLineCaptureMode[nPreValue], g_lpszGrablinkLineCaptureMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetLineRateMode(EDeviceGrablinkLineRateMode * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkLineRateMode)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_LineRateMode));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetLineRateMode(EDeviceGrablinkLineRateMode eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_LineRateMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkLineRateMode_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_LineRateMode, CStringA(g_lpszGrablinkLineRateMode[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkLineRateMode[nPreValue], g_lpszGrablinkLineRateMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetCamConfig(EDeviceGrablinkCamConfig * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkCamConfig)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_CamConfig));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetCamConfig(EDeviceGrablinkCamConfig eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_CamConfig;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkCamConfig_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_CamConfig, CStringA(g_lpszGrablinkCamConfig[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkCamConfig[nPreValue], g_lpszGrablinkCamConfig[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetCamera(EDeviceGrablinkCamera * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkCamera)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_Camera));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetCamera(EDeviceGrablinkCamera eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_Camera;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkCamera_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_Camera, CStringA(g_lpszGrablinkCamera[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkCamera[nPreValue], g_lpszGrablinkCamera[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetDataLink(EDeviceGrablinkDataLink * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkDataLink)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_DataLink));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetDataLink(EDeviceGrablinkDataLink eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_DataLink;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkDataLink_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_DataLink, CStringA(g_lpszGrablinkDataLink[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkDataLink[nPreValue], g_lpszGrablinkDataLink[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetImaging(EDeviceGrablinkImaging * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkImaging)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_Imaging));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetImaging(EDeviceGrablinkImaging eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_Imaging;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkImaging_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_Imaging, CStringA(g_lpszGrablinkImaging[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkImaging[nPreValue], g_lpszGrablinkImaging[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetSpectrum(EDeviceGrablinkSpectrum * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkSpectrum)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_Spectrum));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetSpectrum(EDeviceGrablinkSpectrum eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_Spectrum;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkSpectrum_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_Spectrum, CStringA(g_lpszGrablinkSpectrum[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkSpectrum[nPreValue], g_lpszGrablinkSpectrum[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetExposeRecovery_us(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysGrablink_ExposeRecovery_us));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetExposeRecovery_us(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_ExposeRecovery_us;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamInt(m_hDevice, MC_ExposeRecovery_us, nParam) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetFrameRate_mHz(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysGrablink_FrameRate_mHz));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetFrameRate_mHz(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_FrameRate_mHz;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamInt(m_hDevice, MC_FrameRate_mHz, nParam) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetHactive_Px(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysGrablink_Hactive_Px));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetHactive_Px(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_Hactive_Px;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EEuresysSetFunction_AlreadyInitializedError;
			break;
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetHsyncAft_Tk(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysGrablink_HsyncAft_Tk));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetHsyncAft_Tk(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_HsyncAft_Tk;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamInt(m_hDevice, MC_HsyncAft_Tk, nParam) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetPixelClkMode(EDeviceGrablinkPixelClkMode * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkPixelClkMode)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_PixelClkMode));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetPixelClkMode(EDeviceGrablinkPixelClkMode eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_PixelClkMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkPixelClkMode_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_PixelClkMode, CStringA(g_lpszGrablinkPixelClkMode[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkPixelClkMode[nPreValue], g_lpszGrablinkPixelClkMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetReadoutRecovery_us(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysGrablink_ReadoutRecovery_us));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetReadoutRecovery_us(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_ReadoutRecovery_us;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamInt(m_hDevice, MC_ReadoutRecovery_us, nParam) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetVactive_Ln(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysGrablink_Vactive_Ln));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetVactive_Ln(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_Vactive_Ln;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EEuresysSetFunction_AlreadyInitializedError;
			break;
		}
		
		CString strSave;
		strSave.Format(_T("%d"), nParam);

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetVsyncAft_Ln(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysGrablink_VsyncAft_Ln));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetVsyncAft_Ln(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_VsyncAft_Ln;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamInt(m_hDevice, MC_VsyncAft_Ln, nParam) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetLineRate_Hz(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysGrablink_LineRate_Hz));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetLineRate_Hz(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_LineRate_Hz;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamInt(m_hDevice, MC_LineRate_Hz, nParam) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetTapConfiguration(EDeviceGrablinkTapConfiguration * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkTapConfiguration)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_TapConfiguration));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetTapConfiguration(EDeviceGrablinkTapConfiguration eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_TapConfiguration;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkTapConfiguration_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_TapConfiguration, CStringA(g_lpszGrablinkTapConfiguration[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkTapConfiguration[nPreValue], g_lpszGrablinkTapConfiguration[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetAuxResetCtl(EDeviceGrablinkAuxResetCtl * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkAuxResetCtl)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_AuxResetCtl));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetAuxResetCtl(EDeviceGrablinkAuxResetCtl eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_AuxResetCtl;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkAuxResetCtl_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_AuxResetCtl, CStringA(g_lpszGrablinkAuxResetCtl[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkAuxResetCtl[nPreValue], g_lpszGrablinkAuxResetCtl[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetCC1Usage(EDeviceGrablinkCC1Usage * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkCC1Usage)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_CC1USage));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetCC1Usage(EDeviceGrablinkCC1Usage eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_CC1USage;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkCC1Usage_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_CC1Usage, CStringA(g_lpszGrablinkCC1Usage[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkCC1Usage[nPreValue], g_lpszGrablinkCC1Usage[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetCC2Usage(EDeviceGrablinkCC2Usage * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkCC2Usage)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_CC2Usage));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetCC2Usage(EDeviceGrablinkCC2Usage eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_CC2Usage;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkCC2Usage_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_CC2Usage, CStringA(g_lpszGrablinkCC2Usage[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkCC2Usage[nPreValue], g_lpszGrablinkCC2Usage[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetCC3Usage(EDeviceGrablinkCC3Usage * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}
		
		*pParam = (EDeviceGrablinkCC3Usage)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_CC3Usage));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetCC3Usage(EDeviceGrablinkCC3Usage eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_CC3Usage;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkCC3Usage_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_CC3Usage, CStringA(g_lpszGrablinkCC3Usage[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkCC3Usage[nPreValue], g_lpszGrablinkCC3Usage[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetCC4Usage(EDeviceGrablinkCC4Usage * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkCC4Usage)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_CC4Usage));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetCC4Usage(EDeviceGrablinkCC4Usage eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_CC4Usage;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkCC4Usage_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_CC4Usage, CStringA(g_lpszGrablinkCC4Usage[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkCC4Usage[nPreValue], g_lpszGrablinkCC4Usage[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetColorMethod(EDeviceGrablinkColorMethod * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}
		
		*pParam = (EDeviceGrablinkColorMethod)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_ColorMethod));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetColorMethod(EDeviceGrablinkColorMethod eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_ColorMethod;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkColorMethod_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_ColorMethod, CStringA(g_lpszGrablinkColorMethod[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkColorMethod[nPreValue], g_lpszGrablinkColorMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetColorRegistrationControl(EDeviceGrablinkColorRegistrationControl * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}
		
		*pParam = (EDeviceGrablinkColorRegistrationControl)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_ColorRegistrationControl));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetColorRegistrationControl(EDeviceGrablinkColorRegistrationControl eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_ColorRegistrationControl;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkColorRegistrationControl_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_ColorRegistrationControl, CStringA(g_lpszGrablinkColorRegistrationControl[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkColorRegistrationControl[nPreValue], g_lpszGrablinkColorRegistrationControl[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetDvalMode(EDeviceGrablinkDvalMode * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}
		
		*pParam = (EDeviceGrablinkDvalMode)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_DvalMode));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetDvalMode(EDeviceGrablinkDvalMode eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_DvalMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkDvalMode_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_DvalMode, CStringA(g_lpszGrablinkDvalMode[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkDvalMode[nPreValue], g_lpszGrablinkDvalMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetExpose(EDeviceGrablinkExpose * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkExpose)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_Expose));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetExpose(EDeviceGrablinkExpose eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_Expose;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkExpose_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_Expose, CStringA(g_lpszGrablinkExpose[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkExpose[nPreValue], g_lpszGrablinkExpose[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetExposeMin_us(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysGrablink_ExposeMin_us));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetExposeMin_us(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_ExposeMin_us;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamInt(m_hDevice, MC_ExposeMin_us, nParam) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetExposeMax_us(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysGrablink_ExposeMax_us));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetExposeMax_us(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_ExposeMax_us;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamInt(m_hDevice, MC_ExposeMax_us, nParam) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetExposeOverlap(EDeviceGrablinkExposeOverlap * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkExposeOverlap)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_ExposeOverlap));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetExposeOverlap(EDeviceGrablinkExposeOverlap eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_ExposeOverlap;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkExposeOverlap_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_ExposeOverlap, CStringA(g_lpszGrablinkExposeOverlap[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkExposeOverlap[nPreValue], g_lpszGrablinkExposeOverlap[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetFvalMode(EDeviceGrablinkFvalMode * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkFvalMode)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_FvalMode));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetFvalMode(EDeviceGrablinkFvalMode eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_FvalMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkFvalMode_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_FvalMode, CStringA(g_lpszGrablinkFvalMode[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkFvalMode[nPreValue], g_lpszGrablinkFvalMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetLvalMode(EDeviceGrablinkLvalMode * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}
		
		*pParam = (EDeviceGrablinkLvalMode)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_LvalMode));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetLvalMode(EDeviceGrablinkLvalMode eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_LvalMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkLvalMode_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_LvalMode, CStringA(g_lpszGrablinkLvalMode[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkLvalMode[nPreValue], g_lpszGrablinkLvalMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetReadout(EDeviceGrablinkReadout * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkReadout)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_Readout));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetReadout(EDeviceGrablinkReadout eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_Readout;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkReadout_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_Readout, CStringA(g_lpszGrablinkReadout[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkReadout[nPreValue], g_lpszGrablinkReadout[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetResetCtl(EDeviceGrablinkResetCtl * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkResetCtl)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_ResetCtl));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetResetCtl(EDeviceGrablinkResetCtl eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_ResetCtl;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkResetCtl_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_ResetCtl, CStringA(g_lpszGrablinkResetCtl[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkResetCtl[nPreValue], g_lpszGrablinkResetCtl[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetResetEdge(EDeviceGrablinkResetEdge * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkResetEdge)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_ResetEdge));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetResetEdge(EDeviceGrablinkResetEdge eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_ResetEdge;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkResetEdge_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_ResetEdge, CStringA(g_lpszGrablinkResetEdge[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkResetEdge[nPreValue], g_lpszGrablinkResetEdge[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetTwoLineSynchronization(EDeviceGrablinkTwoLineSynchronization * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkTwoLineSynchronization)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_TwoLineSynchronization));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetTwoLineSynchronization(EDeviceGrablinkTwoLineSynchronization eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_TwoLineSynchronization;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkTwoLineSynchronization_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_TwoLineSynchronization, CStringA(g_lpszGrablinkTwoLineSynchronization[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkTwoLineSynchronization[nPreValue], g_lpszGrablinkTwoLineSynchronization[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetTapGeometry(EDeviceGrablinkTapGeometry * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}
		
		*pParam = (EDeviceGrablinkTapGeometry)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_TapGeometry));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetTapGeometry(EDeviceGrablinkTapGeometry eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_TapGeometry;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkTapGeometry_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_TapGeometry, CStringA(g_lpszGrablinkTapGeometry[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkTapGeometry[nPreValue], g_lpszGrablinkTapGeometry[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetAuxresetLine(EDeviceGrablinkAuxresetLine * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkAuxresetLine)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_AuxresetLine));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetAuxresetLine(EDeviceGrablinkAuxresetLine eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_AuxresetLine;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

		do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkAuxresetLine_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_AuxresetLine, CStringA(g_lpszGrablinkAuxresetLine[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkAuxresetLine[nPreValue], g_lpszGrablinkAuxresetLine[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetResetLine(EDeviceGrablinkResetLine * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkResetLine)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_ResetLine));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetResetLine(EDeviceGrablinkResetLine eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_ResetLine;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkResetLine_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_ResetLine, CStringA(g_lpszGrablinkResetLine[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkResetLine[nPreValue], g_lpszGrablinkResetLine[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetActivityLength(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysGrablink_ActivityLength));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetActivityLength(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_ActivityLength;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamInt(m_hDevice, MC_ActivityLength, nParam) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetBreakEffect(EDeviceGrablinkBreakEffect * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkBreakEffect)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_BreakEffect));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetBreakEffect(EDeviceGrablinkBreakEffect eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_BreakEffect;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkBreakEffect_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_BreakEffect, CStringA(g_lpszGrablinkBreakEffect[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkBreakEffect[nPreValue], g_lpszGrablinkBreakEffect[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetEndTrigMode(EDeviceGrablinkEndTrigMode * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkEndTrigMode)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_EndTrigMode));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetEndTrigMode(EDeviceGrablinkEndTrigMode eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_EndTrigMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkEndTrigMode_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_EndTrigMode, CStringA(g_lpszGrablinkEndTrigMode[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkEndTrigMode[nPreValue], g_lpszGrablinkEndTrigMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);
		
	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetNextTrigMode(EDeviceGrablinkNextTrigMode * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkNextTrigMode)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_NextTrigMode));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetNextTrigMode(EDeviceGrablinkNextTrigMode eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_NextTrigMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkNextTrigMode_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_NextTrigMode, CStringA(g_lpszGrablinkNextTrigMode[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkNextTrigMode[nPreValue], g_lpszGrablinkNextTrigMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetSynchronizedAcquisition(EDeviceGrablinkSynchronizedAcquisition * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkSynchronizedAcquisition)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_SynchronizedAcquisition));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetSynchronizedAcquisition(EDeviceGrablinkSynchronizedAcquisition eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_SynchronizedAcquisition;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkSynchronizedAcquisition_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_SynchronizedAcquisition, CStringA(g_lpszGrablinkSynchronizedAcquisition[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkSynchronizedAcquisition[nPreValue], g_lpszGrablinkSynchronizedAcquisition[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetForceTrig(EDeviceGrablinkForceTrig * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkForceTrig)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_ForceTrig));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetForceTrig(EDeviceGrablinkForceTrig eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_ForceTrig;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkForceTrig_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_ForceTrig, CStringA(g_lpszGrablinkForceTrig[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkForceTrig[nPreValue], g_lpszGrablinkForceTrig[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetNextTrigDelay_Pls(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysGrablink_NextTrigDelay_Pls));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetNextTrigDelay_Pls(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_NextTrigDelay_Pls;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamInt(m_hDevice, MC_NextTrigDelay_Pls, nParam) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetTrigCtl(EDeviceGrablinkTrigCtl * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkTrigCtl)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_TrigCtl));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetTrigCtl(EDeviceGrablinkTrigCtl eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_TrigCtl;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkTrigCtl_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_TrigCtl, CStringA(g_lpszGrablinkTrigCtl[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkTrigCtl[nPreValue], g_lpszGrablinkTrigCtl[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetTrigDelay_Pls(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysGrablink_TrigDelay_Pls));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetTrigDelay_Pls(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_TrigDelay_Pls;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamInt(m_hDevice, MC_TrigDelay_Pls, nParam) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetTrigDelay_us(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysGrablink_TrigDelay_us));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetTrigDelay_us(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_TrigDelay_us;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamInt(m_hDevice, MC_TrigDelay_us, nParam) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetTrigEdge(EDeviceGrablinkTrigEdge * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkTrigEdge)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_TrigEdge));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetTrigEdge(EDeviceGrablinkTrigEdge eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_TrigEdge;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkTrigEdge_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_TrigEdge, CStringA(g_lpszGrablinkTrigEdge[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkTrigEdge[nPreValue], g_lpszGrablinkTrigEdge[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetTrigFilter(EDeviceGrablinkTrigFilter * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkTrigFilter)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_TrigFilter));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetTrigFilter(EDeviceGrablinkTrigFilter eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_TrigFilter;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkTrigFilter_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_TrigFilter, CStringA(g_lpszGrablinkTrigFilter[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkTrigFilter[nPreValue], g_lpszGrablinkTrigFilter[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetTrigLine(EDeviceGrablinkTrigLine * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkTrigLine)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_TrigLine));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetTrigLine(EDeviceGrablinkTrigLine eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_TrigLine;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkTrigLine_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_TrigLine, CStringA(g_lpszGrablinkTrigLine[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkTrigLine[nPreValue], g_lpszGrablinkTrigLine[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetPageDelay_Ln(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysGrablink_PageDelay_Ln));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetPageDelay_Ln(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_PageDelay_Ln;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamInt(m_hDevice, MC_PageDelay_Ln, nParam) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetExposeTrim(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysGrablink_ExposeTrim));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetExposeTrim(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_ExposeTrim;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamInt(m_hDevice, MC_ExposeTrim, nParam) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetStrobeCtl(EDeviceGrablinkStrobeCtl * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkStrobeCtl)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_StrobeCtl));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetStrobeCtl(EDeviceGrablinkStrobeCtl eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_StrobeCtl;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkStrobeCtl_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_StrobeCtl, CStringA(g_lpszGrablinkStrobeCtl[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkStrobeCtl[nPreValue], g_lpszGrablinkStrobeCtl[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetStrobeDur(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysGrablink_StrobeDur));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetStrobeDur(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_StrobeDur;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamInt(m_hDevice, MC_StrobeDur, nParam) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetStrobeMode(EDeviceGrablinkStrobeMode * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkStrobeMode)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_StrobeMode));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetStrobeMode(EDeviceGrablinkStrobeMode eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_StrobeMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkStrobeMode_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_StrobeMode, CStringA(g_lpszGrablinkStrobeMode[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkStrobeMode[nPreValue], g_lpszGrablinkStrobeMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetStrobePos(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysGrablink_StrobePos));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetStrobePos(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_StrobePos;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamInt(m_hDevice, MC_StrobePos, nParam) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetPreStrobe_us(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysGrablink_PreStrobe_us));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetPreStrobe_us(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_PreStrobe_us;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamInt(m_hDevice, MC_PreStrobe_us, nParam) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetLineTrigCtl(EDeviceGrablinkLineTrigCtl * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkLineTrigCtl)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_LineTrigCtl));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetLineTrigCtl(EDeviceGrablinkLineTrigCtl eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_LineTrigCtl;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkLineTrigCtl_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_LineTrigCtl, CStringA(g_lpszGrablinkLineTrigCtl[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkLineTrigCtl[nPreValue], g_lpszGrablinkLineTrigCtl[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetLineTrigEdge(EDeviceGrablinkLineTrigEdge * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkLineTrigEdge)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_LineTrigEdge));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetLineTrigEdge(EDeviceGrablinkLineTrigEdge eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_LineTrigEdge;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkLineTrigEdge_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_LineTrigEdge, CStringA(g_lpszGrablinkLineTrigEdge[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkLineTrigEdge[nPreValue], g_lpszGrablinkLineTrigEdge[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetLineTrigFilter(EDeviceGrablinkLineTrigFilter * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkLineTrigFilter)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_LineTrigFilter));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetLineTrigFilter(EDeviceGrablinkLineTrigFilter eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_LineTrigFilter;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkLineTrigFilter_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_LineTrigFilter, CStringA(g_lpszGrablinkLineTrigFilter[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkLineTrigFilter[nPreValue], g_lpszGrablinkLineTrigFilter[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetRateDivisionFactor(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysGrablink_RateDivisionFactor));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetRateDivisionFactor(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_RateDivisionFactor;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamInt(m_hDevice, MC_RateDivisionFactor, nParam) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetLineTrigLine(EDeviceGrablinkLineTrigLine * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkLineTrigLine)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_LineTrigLine));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetLineTrigLine(EDeviceGrablinkLineTrigLine eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_LineTrigLine;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkLineTrigLine_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_LineTrigLine, CStringA(g_lpszGrablinkLineTrigLine[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkLineTrigLine[nPreValue], g_lpszGrablinkLineTrigLine[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetRateDividerRestart(EDeviceGrablinkRateDividerRestart * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkRateDividerRestart)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_RateDividerRestart));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetRateDividerRestart(EDeviceGrablinkRateDividerRestart eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_RateDividerRestart;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkRateDividerRestart_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_RateDividerRestart, CStringA(g_lpszGrablinkRateDividerRestart[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkRateDividerRestart[nPreValue], g_lpszGrablinkRateDividerRestart[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetECCO_PLLResetControl(EDeviceGrablinkECCO_PLLResetControl * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkECCO_PLLResetControl)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_ECCO_PLLResetControl));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetECCO_PLLResetControl(EDeviceGrablinkECCO_PLLResetControl eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_ECCO_PLLResetControl;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkECCO_PLLResetControl_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_ECCO_PLLResetControl, CStringA(g_lpszGrablinkECCO_PLLResetControl[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkECCO_PLLResetControl[nPreValue], g_lpszGrablinkECCO_PLLResetControl[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetECCO_SkewCompensation(EDeviceGrablinkECCO_SkewCompensation * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkECCO_SkewCompensation)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_ECCO_SkewCompensation));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetECCO_SkewCompensation(EDeviceGrablinkECCO_SkewCompensation eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_ECCO_SkewCompensation;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkECCO_SkewCompensation_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_ECCO_SkewCompensation, CStringA(g_lpszGrablinkECCO_SkewCompensation[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkECCO_SkewCompensation[nPreValue], g_lpszGrablinkECCO_SkewCompensation[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetFvalMin_Tk(EDeviceGrablinkFvalMin_Tk * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkFvalMin_Tk)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_FvalMin_Tk));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetFvalMin_Tk(EDeviceGrablinkFvalMin_Tk eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_FvalMin_Tk;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkFvalMin_Tk_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_FvalMin_Tk, CStringA(g_lpszGrablinkFvalMin_Tk[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkFvalMin_Tk[nPreValue], g_lpszGrablinkFvalMin_Tk[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetLvalMin_Tk(EDeviceGrablinkLvalMin_Tk * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkLvalMin_Tk)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_LvalMin_Tk));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetLvalMin_Tk(EDeviceGrablinkLvalMin_Tk eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_LvalMin_Tk;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkLvalMin_Tk_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_LvalMin_Tk, CStringA(g_lpszGrablinkLvalMin_Tk[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkLvalMin_Tk[nPreValue], g_lpszGrablinkLvalMin_Tk[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetMetadataInsertion(EDeviceGrablinkMetadataInsertion * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkMetadataInsertion)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_MetadataInsertion));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetMetadataInsertion(EDeviceGrablinkMetadataInsertion eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_MetadataInsertion;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkMetadataInsertion_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_MetadataInsertion, CStringA(g_lpszGrablinkMetadataInsertion[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkMetadataInsertion[nPreValue], g_lpszGrablinkMetadataInsertion[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetGrabWindow(EDeviceGrablinkGrabWindow * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkGrabWindow)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_GrabWindow));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetGrabWindow(EDeviceGrablinkGrabWindow eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_GrabWindow;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkGrabWindow_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_GrabWindow, CStringA(g_lpszGrablinkGrabWindow[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkGrabWindow[nPreValue], g_lpszGrablinkGrabWindow[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetOffsetX_Px(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysGrablink_OffsetX_Px));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetOffsetX_Px(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_OffsetX_Px;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(McSetParamInt(m_hDevice, MC_OffsetX_Px, nParam) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetWindowX_Px(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysGrablink_WindowX_Px));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetWindowX_Px(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_WindowX_Px;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamInt(m_hDevice, MC_WindowX_Px, nParam) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetColorFormat(EDeviceGrablinkColorFormat * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkColorFormat)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_ColorFormat));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetColorFormat(EDeviceGrablinkColorFormat eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_ColorFormat;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			eReturn = EEuresysSetFunction_AlreadyInitializedError;
			break;
		}

		if(eParam < 0 || eParam >= EDeviceGrablinkColorFormat_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkColorFormat[nPreValue], g_lpszGrablinkColorFormat[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetImageFlipX(EDeviceGrablinkImageFlipX * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkImageFlipX)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_ImageFlipX));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetImageFlipX(EDeviceGrablinkImageFlipX eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_ImageFlipX;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkImageFlipX_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_ImageFlipX, CStringA(g_lpszGrablinkImageFlipX[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkImageFlipX[nPreValue], g_lpszGrablinkImageFlipX[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetImageFlipY(EDeviceGrablinkImageFlipY * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkImageFlipY)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_ImageFlipY));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetImageFlipY(EDeviceGrablinkImageFlipY eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_ImageFlipY;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkImageFlipY_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_ImageFlipY, CStringA(g_lpszGrablinkImageFlipY[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkImageFlipY[nPreValue], g_lpszGrablinkImageFlipY[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetRedBlueSwap(EDeviceGrablinkRedBlueSwap * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkRedBlueSwap)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_RedBlueSwap));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetRedBlueSwap(EDeviceGrablinkRedBlueSwap eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_RedBlueSwap;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkRedBlueSwap_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_RedBlueSwap, CStringA(g_lpszGrablinkRedBlueSwap[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkRedBlueSwap[nPreValue], g_lpszGrablinkRedBlueSwap[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetMaxFillingSurfaces(EDeviceGrablinkMaxFillingSurfaces * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkMaxFillingSurfaces)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_MaxFillingSurfaces));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetMaxFillingSurfaces(EDeviceGrablinkMaxFillingSurfaces eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_MaxFillingSurfaces;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkMaxFillingSurfaces_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_MaxFillingSurfaces, CStringA(g_lpszGrablinkMaxFillingSurfaces[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkMaxFillingSurfaces[nPreValue], g_lpszGrablinkMaxFillingSurfaces[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetSurfaceCount(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysGrablink_SurfaceCount));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetSurfaceCount(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_SurfaceCount;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamInt(m_hDevice, MC_SurfaceCount, nParam) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetAcquisitionCleanup(EDeviceGrablinkAcquisitionCleanup * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}
		
		*pParam = (EDeviceGrablinkAcquisitionCleanup)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_AcquisitionCleanup));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetAcquisitionCleanup(EDeviceGrablinkAcquisitionCleanup eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_AcquisitionCleanup;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkAcquisitionCleanup_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_AcquisitionCleanup, CStringA(g_lpszGrablinkAcquisitionCleanup[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkAcquisitionCleanup[nPreValue], g_lpszGrablinkAcquisitionCleanup[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetAcqTimeout_ms(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysGrablink_AcqTimeout_ms));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetAcqTimeout_ms(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_AcqTimeout_ms;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamInt(m_hDevice, MC_AcqTimeout_ms, nParam) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetParamsConsistencyCheck(EDeviceGrablinkParamsConsistencyCheck * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkParamsConsistencyCheck)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_ParamsConsistencyCheck));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetParamsConsistencyCheck(EDeviceGrablinkParamsConsistencyCheck eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_ParamsConsistencyCheck;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkParamsConsistencyCheck_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_ParamsConsistencyCheck, CStringA(g_lpszGrablinkParamsConsistencyCheck[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkParamsConsistencyCheck[nPreValue], g_lpszGrablinkParamsConsistencyCheck[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetTriggerSkipHold(EDeviceGrablinkTriggerSkipHold * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkTriggerSkipHold)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_TriggerSkipHold));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetTriggerSkipHold(EDeviceGrablinkTriggerSkipHold eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_TriggerSkipHold;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkTriggerSkipHold_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_TriggerSkipHold, CStringA(g_lpszGrablinkTriggerSkipHold[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkTriggerSkipHold[nPreValue], g_lpszGrablinkTriggerSkipHold[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetCallbackPriority(EDeviceGrablinkCallbackPriority * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkCallbackPriority)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_CallbackPriority));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetCallbackPriority(EDeviceGrablinkCallbackPriority eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_CallbackPriority;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkCallbackPriority_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_CallbackPriority, CStringA(g_lpszGrablinkCallbackPriority[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkCallbackPriority[nPreValue], g_lpszGrablinkCallbackPriority[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetInterleavedAcquisition(EDeviceGrablinkInterleavedAcquisition * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = (EDeviceGrablinkInterleavedAcquisition)_ttoi(GetParamValue(EDeviceParameterEuresysGrablink_InterleavedAcquisition));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetInterleavedAcquisition(EDeviceGrablinkInterleavedAcquisition eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_InterleavedAcquisition;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceGrablinkInterleavedAcquisition_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_InterleavedAcquisition, CStringA(g_lpszGrablinkInterleavedAcquisition[eParam])) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], g_lpszGrablinkInterleavedAcquisition[nPreValue], g_lpszGrablinkInterleavedAcquisition[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetExposureTime_P1_us(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysGrablink_ExposureTime_P1_us));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetExposureTime_P1_us(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_ExposureTime_P1_us;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamInt(m_hDevice, MC_ExposureTime_P1_us, nParam) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetExposureTime_P2_us(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysGrablink_ExposureTime_P2_us));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetExposureTime_P2_us(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_ExposureTime_P2_us;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamInt(m_hDevice, MC_ExposureTime_P2_us, nParam) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetStrobeDuration_P1_us(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysGrablink_StrobeDuration_P1_us));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetStrobeDuration_P1_us(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_StrobeDuration_P1_us;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamInt(m_hDevice, MC_StrobeDuration_P1_us, nParam) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetStrobeDuration_P2_us(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysGrablink_StrobeDuration_P2_us));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetStrobeDuration_P2_us(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_StrobeDuration_P2_us;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamInt(m_hDevice, MC_StrobeDuration_P2_us, nParam) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetStrobeDelay_P1_us(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysGrablink_StrobeDelay_P1_us));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetStrobeDelay_P1_us(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_StrobeDelay_P1_us;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamInt(m_hDevice, MC_StrobeDelay_P1_us, nParam) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetStrobeDelay_P2_us(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysGrablink_StrobeDelay_P2_us));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetStrobeDelay_P2_us(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_StrobeDelay_P2_us;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamInt(m_hDevice, MC_StrobeDelay_P2_us, nParam) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetExposureDelay_MAN_P1_us(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysGrablink_ExposureDelay_MAN_P1_us));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetExposureDelay_MAN_P1_us(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_ExposureDelay_MAN_P1_us;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamInt(m_hDevice, MC_ExposureDelay_MAN_P1_us, nParam) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);
		
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysGrablink::GetExposureDelay_MAN_P2_us(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysGrablink_ExposureDelay_MAN_P2_us));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysGrablink::SetExposureDelay_MAN_P2_us(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysGrablink eSaveID = EDeviceParameterEuresysGrablink_ExposureDelay_MAN_P2_us;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamInt(m_hDevice, MC_ExposureDelay_MAN_P2_us, nParam) != MC_OK)
			{
				eReturn = EEuresysSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strSave;
		strSave.Format(_T("%d"), nParam);

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszParamGrablink[eSaveID - EDeviceParameterEuresysGrablinkFull_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

void CDeviceEuresysGrablink::CallbackFunction(PMCSIGNALINFO CbInfo)
{
	if(!CbInfo)
		return;

	if(CbInfo->Signal != MC_SIG_SURFACE_FILLED)
		return;
	
	CDeviceEuresysGrablink* pDevice = (CDeviceEuresysGrablink*)CbInfo->Context;

	if(!pDevice)
		return;

	if(!pDevice->IsInitialized())
		return;

	--pDevice->m_nCurrentCount;

	do
	{
		BYTE* pCurrent = nullptr;

		int nImageSizeX = -1;
		int nImageSizeY = -1;

		if(McGetParamPtr(CbInfo->SignalInfo, MC_SurfaceAddr, (PVOID*)&pCurrent) != MC_OK)
			break;

		if(!pCurrent)
			break;

		if(McGetParamInt(CbInfo->SignalInfo, MC_SurfaceSizeX, (PINT32)&nImageSizeX) != MC_OK)
			break;

		if(McGetParamInt(CbInfo->SignalInfo, MC_SurfaceSizeY, (PINT32)&nImageSizeY) != MC_OK)
			break;

		if(nImageSizeX < 1 && nImageSizeY < 1)
			break;

		pDevice->NextImageIndex();

		CRavidImage* pCurrentImage = pDevice->GetImageInfo();

		BYTE** ppCurrentOffsetY = pCurrentImage->GetYOffsetTable();

		const int64_t i64Width = (int64_t)pCurrentImage->GetSizeX();
		const int64_t i64Height = (int64_t)pCurrentImage->GetSizeY();
		const int64_t i64WidthStep = (int64_t)pCurrentImage->GetWidthStep();
		const int64_t i64ImgChannels = (int64_t)pCurrentImage->GetChannels();
		const int64_t i64ImageSizeByte = (int64_t)pCurrentImage->GetImageSizeByte();
		const int64_t i64PixelSizeByte = (int64_t)pCurrentImage->GetPixelSizeByte();


		int64_t nCopySize = nImageSizeX * i64PixelSizeByte;

		for(int y = 0; y < nImageSizeY; ++y)
			memcpy(ppCurrentOffsetY[y], pCurrent + (y * nCopySize), sizeof(BYTE) * nCopySize);

		pDevice->ConnectImage(false);

		CEventHandlerManager::BroadcastOnAcquisition(pCurrentImage, pDevice->GetObjectID(), pDevice->GetImageView(), pDevice);
	}
	while(false);

	if(pDevice->m_nCurrentCount <= 0)
	{
		pDevice->m_bIsGrabAvailable = true;

		McSetParamInt(pDevice->m_hDevice, MC_ChannelState, MC_ChannelState_IDLE);
	}
}

bool CDeviceEuresysGrablink::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = false;

	if(__super::OnParameterChanged(nParam, strValue))
		return true;

	do
	{
		switch(nParam)
		{
		case EDeviceParameterEuresysGrablink_AcquisitionMode:
			bReturn = !SetAcquisitionMode((EDeviceGrablinkAcquisitionMode)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_TrigMode:
			bReturn = !SetTrigMode((EDeviceGrablinkTrigMode)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_Expose_us:
			bReturn = !SetExpose_us(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_SeqLength_Fr:
			bReturn = !SetSeqLength_Fr(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_PhaseLength_Fr:
			bReturn = !SetPhaseLength_Fr(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_SeqLength_Ln:
			bReturn = !SetSeqLength_Ln(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_SeqLength_Pg:
			bReturn = !SetSeqLength_Pg(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_PageLength_Ln:
			bReturn = !SetPageLength_Ln(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_LineCaptureMode:
			bReturn = !SetLineCaptureMode((EDeviceGrablinkLineCaptureMode)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_LineRateMode:
			bReturn = !SetLineRateMode((EDeviceGrablinkLineRateMode)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_CamConfig:
			bReturn = !SetCamConfig((EDeviceGrablinkCamConfig)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_Camera:
			bReturn = !SetCamera((EDeviceGrablinkCamera)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_DataLink:
			bReturn = !SetDataLink((EDeviceGrablinkDataLink)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_Imaging:
			bReturn = !SetImaging((EDeviceGrablinkImaging)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_Spectrum:
			bReturn = !SetSpectrum((EDeviceGrablinkSpectrum)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_ExposeRecovery_us:
			bReturn = !SetExposeRecovery_us(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_FrameRate_mHz:
			bReturn = !SetFrameRate_mHz(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_Hactive_Px:
			bReturn = !SetHactive_Px(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_HsyncAft_Tk:
			bReturn = !SetHsyncAft_Tk(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_PixelClkMode:
			bReturn = !SetPixelClkMode((EDeviceGrablinkPixelClkMode)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_ReadoutRecovery_us:
			bReturn = !SetReadoutRecovery_us(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_Vactive_Ln:
			bReturn = !SetVactive_Ln(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_VsyncAft_Ln:
			bReturn = !SetVsyncAft_Ln(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_LineRate_Hz:
			bReturn = !SetLineRate_Hz(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_TapConfiguration:
			bReturn = !SetTapConfiguration((EDeviceGrablinkTapConfiguration)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_AuxResetCtl:
			bReturn = !SetAuxResetCtl((EDeviceGrablinkAuxResetCtl)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_CC1USage:
			bReturn = !SetCC1Usage((EDeviceGrablinkCC1Usage)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_CC2Usage:
			bReturn = !SetCC2Usage((EDeviceGrablinkCC2Usage)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_CC3Usage:
			bReturn = !SetCC3Usage((EDeviceGrablinkCC3Usage)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_CC4Usage:
			bReturn = !SetCC4Usage((EDeviceGrablinkCC4Usage)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_ColorMethod:
			bReturn = !SetColorMethod((EDeviceGrablinkColorMethod)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_ColorRegistrationControl:
			bReturn = !SetColorRegistrationControl((EDeviceGrablinkColorRegistrationControl)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_DvalMode:
			bReturn = !SetDvalMode((EDeviceGrablinkDvalMode)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_Expose:
			bReturn = !SetExpose((EDeviceGrablinkExpose)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_ExposeMin_us:
			bReturn = !SetExposeMin_us(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_ExposeMax_us:
			bReturn = !SetExposeMax_us(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_ExposeOverlap:
			bReturn = !SetExposeOverlap((EDeviceGrablinkExposeOverlap)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_FvalMode:
			bReturn = !SetFvalMode((EDeviceGrablinkFvalMode)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_LvalMode:
			bReturn = !SetLvalMode((EDeviceGrablinkLvalMode)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_Readout:
			bReturn = !SetReadout((EDeviceGrablinkReadout)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_ResetCtl:
			bReturn = !SetResetCtl((EDeviceGrablinkResetCtl)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_ResetEdge:
			bReturn = !SetResetEdge((EDeviceGrablinkResetEdge)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_TwoLineSynchronization:
			bReturn = !SetTwoLineSynchronization((EDeviceGrablinkTwoLineSynchronization)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_TapGeometry:
			bReturn = !SetTapGeometry((EDeviceGrablinkTapGeometry)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_AuxresetLine:
			bReturn = !SetAuxresetLine((EDeviceGrablinkAuxresetLine)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_ResetLine:
			bReturn = !SetResetLine((EDeviceGrablinkResetLine)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_ActivityLength:
			bReturn = !SetActivityLength(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_BreakEffect:
			bReturn = !SetBreakEffect((EDeviceGrablinkBreakEffect)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_EndTrigMode:
			bReturn = !SetEndTrigMode((EDeviceGrablinkEndTrigMode)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_NextTrigMode:
			bReturn = !SetNextTrigMode((EDeviceGrablinkNextTrigMode)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_SynchronizedAcquisition:
			bReturn = !SetSynchronizedAcquisition((EDeviceGrablinkSynchronizedAcquisition)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_ForceTrig:
			bReturn = !SetForceTrig((EDeviceGrablinkForceTrig)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_NextTrigDelay_Pls:
			bReturn = !SetNextTrigDelay_Pls(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_TrigCtl:
			bReturn = !SetTrigCtl((EDeviceGrablinkTrigCtl)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_TrigDelay_Pls:
			bReturn = !SetTrigDelay_Pls(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_TrigDelay_us:
			bReturn = !SetTrigDelay_us(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_TrigEdge:
			bReturn = !SetTrigEdge((EDeviceGrablinkTrigEdge)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_TrigFilter:
			bReturn = !SetTrigFilter((EDeviceGrablinkTrigFilter)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_TrigLine:
			bReturn = !SetTrigLine((EDeviceGrablinkTrigLine)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_PageDelay_Ln:
			bReturn = !SetPageDelay_Ln(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_ExposeTrim:
			bReturn = !SetExposeTrim(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_StrobeCtl:
			bReturn = !SetStrobeCtl((EDeviceGrablinkStrobeCtl)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_StrobeDur:
			bReturn = !SetStrobeDur(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_StrobeMode:
			bReturn = !SetStrobeMode((EDeviceGrablinkStrobeMode)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_StrobePos:
			bReturn = !SetStrobePos(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_PreStrobe_us:
			bReturn = !SetPreStrobe_us(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_LineTrigCtl:
			bReturn = !SetLineTrigCtl((EDeviceGrablinkLineTrigCtl)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_LineTrigEdge:
			bReturn = !SetLineTrigEdge((EDeviceGrablinkLineTrigEdge)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_LineTrigFilter:
			bReturn = !SetLineTrigFilter((EDeviceGrablinkLineTrigFilter)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_RateDivisionFactor:
			bReturn = !SetRateDivisionFactor(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_LineTrigLine:
			bReturn = !SetLineTrigLine((EDeviceGrablinkLineTrigLine)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_RateDividerRestart:
			bReturn = !SetRateDividerRestart((EDeviceGrablinkRateDividerRestart)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_ECCO_PLLResetControl:
			bReturn = !SetECCO_PLLResetControl((EDeviceGrablinkECCO_PLLResetControl)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_ECCO_SkewCompensation:
			bReturn = !SetECCO_SkewCompensation((EDeviceGrablinkECCO_SkewCompensation)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_FvalMin_Tk:
			bReturn = !SetFvalMin_Tk((EDeviceGrablinkFvalMin_Tk)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_LvalMin_Tk:
			bReturn = !SetLvalMin_Tk((EDeviceGrablinkLvalMin_Tk)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_MetadataInsertion:
			bReturn = !SetMetadataInsertion((EDeviceGrablinkMetadataInsertion)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_GrabWindow:
			bReturn = !SetGrabWindow((EDeviceGrablinkGrabWindow)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_OffsetX_Px:
			bReturn = !SetOffsetX_Px(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_WindowX_Px:
			bReturn = !SetWindowX_Px(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_ColorFormat:
			bReturn = !SetColorFormat((EDeviceGrablinkColorFormat)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_ImageFlipX:
			bReturn = !SetImageFlipX((EDeviceGrablinkImageFlipX)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_ImageFlipY:
			bReturn = !SetImageFlipY((EDeviceGrablinkImageFlipY)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_RedBlueSwap:
			bReturn = !SetRedBlueSwap((EDeviceGrablinkRedBlueSwap)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_MaxFillingSurfaces:
			bReturn = !SetMaxFillingSurfaces((EDeviceGrablinkMaxFillingSurfaces)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_SurfaceCount:
			bReturn = !SetSurfaceCount(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_AcquisitionCleanup:
			bReturn = !SetAcquisitionCleanup((EDeviceGrablinkAcquisitionCleanup)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_AcqTimeout_ms:
			bReturn = !SetAcqTimeout_ms(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_ParamsConsistencyCheck:
			bReturn = !SetParamsConsistencyCheck((EDeviceGrablinkParamsConsistencyCheck)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_TriggerSkipHold:
			bReturn = !SetTriggerSkipHold((EDeviceGrablinkTriggerSkipHold)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_CallbackPriority:
			bReturn = !SetCallbackPriority((EDeviceGrablinkCallbackPriority)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_InterleavedAcquisition:
			bReturn = !SetInterleavedAcquisition((EDeviceGrablinkInterleavedAcquisition)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_ExposureTime_P1_us:
			bReturn = !SetExposureTime_P1_us(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_ExposureTime_P2_us:
			bReturn = !SetExposureTime_P2_us(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_StrobeDuration_P1_us:
			bReturn = !SetStrobeDuration_P1_us(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_StrobeDuration_P2_us:
			bReturn = !SetStrobeDuration_P2_us(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_StrobeDelay_P1_us:
			bReturn = !SetStrobeDelay_P1_us(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_StrobeDelay_P2_us:
			bReturn = !SetStrobeDelay_P2_us(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_ExposureDelay_MAN_P1_us:
			bReturn = !SetExposureDelay_MAN_P1_us(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysGrablink_ExposureDelay_MAN_P2_us:
			bReturn = !SetExposureDelay_MAN_P2_us(_ttoi(strValue));
			break;
		default:
			break;
		}
	}
	while(false);

	return bReturn;
}

void CDeviceEuresysGrablink::SetUpdateDevice()
{
	int nLoadDB = -1;

	EDeviceGrablinkCamera eCamera = EDeviceGrablinkCamera_Count;
	if(!GetCamera(&eCamera))
		McSetParamStr(m_hDevice, MC_Camera, CStringA(g_lpszGrablinkCamera[eCamera]));

	EDeviceGrablinkCamConfig eCamConfig = EDeviceGrablinkCamConfig_Count;
	if(!GetCamConfig(&eCamConfig))
		McSetParamStr(m_hDevice, MC_CamConfig, CStringA(g_lpszGrablinkCamConfig[eCamConfig]));

	EDeviceGrablinkDataLink eDataLink = EDeviceGrablinkDataLink_Count;
	if(!GetDataLink(&eDataLink))
		McSetParamStr(m_hDevice, MC_DataLink, CStringA(g_lpszGrablinkDataLink[eDataLink]));

	EDeviceGrablinkImaging eImaging = EDeviceGrablinkImaging_Count;
	if(!GetImaging(&eImaging))
		McSetParamStr(m_hDevice, MC_Imaging, CStringA(g_lpszGrablinkImaging[eImaging]));

	if(eImaging == EDeviceGrablinkImaging_AREA)
		m_bIsLine = false;
	else
		m_bIsLine = true;
	
	EDeviceGrablinkSpectrum eSpectrum = EDeviceGrablinkSpectrum_Count;
	if(!GetSpectrum(&eSpectrum))
		McSetParamStr(m_hDevice, MC_Spectrum, CStringA(g_lpszGrablinkSpectrum[eSpectrum]));

	if(eSpectrum == EDeviceGrablinkSpectrum_COLOR)
		m_bIsColor = true;
	else
		m_bIsColor = false;
	
	EDeviceGrablinkTapConfiguration eTapConfiguration = EDeviceGrablinkTapConfiguration_Count;
	if(!GetTapConfiguration(&eTapConfiguration))
		McSetParamStr(m_hDevice, MC_TapConfiguration, CStringA(g_lpszGrablinkTapConfiguration[eTapConfiguration]));
	
	EDeviceGrablinkTapGeometry eTapGeometry = EDeviceGrablinkTapGeometry_Count;
	if(!GetTapGeometry(&eTapGeometry))
		McSetParamStr(m_hDevice, MC_TapGeometry, CStringA(g_lpszGrablinkTapGeometry[eTapGeometry]));

	EDeviceGrablinkExpose eExpose = EDeviceGrablinkExpose_Count;
	if(!GetExpose(&eExpose))
		McSetParamStr(m_hDevice, MC_Expose, CStringA(g_lpszGrablinkExpose[eExpose]));

	EDeviceGrablinkReadout eReadout = EDeviceGrablinkReadout_Count;
	if(!GetReadout(&eReadout))
		McSetParamStr(m_hDevice, MC_Readout, CStringA(g_lpszGrablinkReadout[eReadout]));

	EDeviceGrablinkColorMethod eColorMethod = EDeviceGrablinkColorMethod_Count;
	if(!GetColorMethod(&eColorMethod))
		McSetParamStr(m_hDevice, MC_ColorMethod, CStringA(g_lpszGrablinkColorMethod[eColorMethod]));
	
	EDeviceGrablinkFvalMode eFvalMode = EDeviceGrablinkFvalMode_Count;
	if(!GetFvalMode(&eFvalMode))
		McSetParamStr(m_hDevice, MC_FvalMode, CStringA(g_lpszGrablinkFvalMode[eFvalMode]));

	EDeviceGrablinkLvalMode eLvalMode = EDeviceGrablinkLvalMode_Count;
	if(!GetLvalMode(&eLvalMode))
		McSetParamStr(m_hDevice, MC_LvalMode, CStringA(g_lpszGrablinkLvalMode[eLvalMode]));
	
	EDeviceGrablinkDvalMode eDvalMode = EDeviceGrablinkDvalMode_Count;
	if(!GetDvalMode(&eDvalMode))
		McSetParamStr(m_hDevice, MC_DvalMode, CStringA(g_lpszGrablinkDvalMode[eDvalMode]));
	
	EDeviceGrablinkAuxResetCtl eAuxResetCtl = EDeviceGrablinkAuxResetCtl_Count;
	if(!GetAuxResetCtl(&eAuxResetCtl))
		McSetParamStr(m_hDevice, MC_AuxResetCtl, CStringA(g_lpszGrablinkAuxResetCtl[eAuxResetCtl]));
	
	EDeviceGrablinkResetEdge eResetEdge = EDeviceGrablinkResetEdge_Count;
	if(!GetResetEdge(&eResetEdge))
		McSetParamStr(m_hDevice, MC_ResetEdge, CStringA(g_lpszGrablinkResetEdge[eResetEdge]));

	EDeviceGrablinkCC1Usage eCC1Usage = EDeviceGrablinkCC1Usage_Count;
	if(!GetCC1Usage(&eCC1Usage))
		McSetParamStr(m_hDevice, MC_CC1Usage, CStringA(g_lpszGrablinkCC1Usage[eCC1Usage]));

	EDeviceGrablinkCC2Usage eCC2Usage = EDeviceGrablinkCC2Usage_Count;
	if(!GetCC2Usage(&eCC2Usage))
		McSetParamStr(m_hDevice, MC_CC2Usage, CStringA(g_lpszGrablinkCC2Usage[eCC2Usage]));
	
	EDeviceGrablinkCC3Usage eCC3Usage = EDeviceGrablinkCC3Usage_Count;
	if(!GetCC3Usage(&eCC3Usage))
		McSetParamStr(m_hDevice, MC_CC3Usage, CStringA(g_lpszGrablinkCC3Usage[eCC3Usage]));

	EDeviceGrablinkCC4Usage eCC4Usage = EDeviceGrablinkCC4Usage_Count;
	if(!GetCC4Usage(&eCC4Usage))
		McSetParamStr(m_hDevice, MC_CC4Usage, CStringA(g_lpszGrablinkCC4Usage[eCC4Usage]));

	nLoadDB = -1;
	if(!GetHactive_Px(&nLoadDB))
		McSetParamInt(m_hDevice, MC_Hactive_Px, nLoadDB);

	nLoadDB = -1;
	if(!GetHsyncAft_Tk(&nLoadDB))
		McSetParamInt(m_hDevice, MC_HsyncAft_Tk, nLoadDB);

	nLoadDB = -1;
	if(!GetLineRate_Hz(&nLoadDB))
		McSetParamInt(m_hDevice, MC_LineRate_Hz, nLoadDB);

	EDeviceGrablinkAcquisitionMode eAcquisitionMode = EDeviceGrablinkAcquisitionMode_Count;
	if(!GetAcquisitionMode(&eAcquisitionMode))
		McSetParamStr(m_hDevice, MC_AcquisitionMode, CStringA(g_lpszGrablinkAcquisitionMode[eAcquisitionMode]));

	EDeviceGrablinkTrigMode eTrigMode = EDeviceGrablinkTrigMode_Count;
	if(!GetTrigMode(&eTrigMode))
		McSetParamStr(m_hDevice, MC_TrigMode, CStringA(g_lpszGrablinkTrigMode[eTrigMode]));

	nLoadDB = -1;
	if(!GetSeqLength_Fr(&nLoadDB))
		McSetParamInt(m_hDevice, MC_SeqLength_Fr, nLoadDB);

	nLoadDB = -1;
	if(!GetPhaseLength_Fr(&nLoadDB))
		McSetParamInt(m_hDevice, MC_PhaseLength_Fr, nLoadDB);

	nLoadDB = -1;
	if(!GetSeqLength_Ln(&nLoadDB))
		McSetParamInt(m_hDevice, MC_SeqLength_Ln, nLoadDB);

	nLoadDB = -1;
	if(!GetSeqLength_Pg(&nLoadDB))
		McSetParamInt(m_hDevice, MC_SeqLength_Pg, nLoadDB);

	nLoadDB = -1;
	if(!GetPageLength_Ln(&nLoadDB))
		McSetParamInt(m_hDevice, MC_PageLength_Ln, nLoadDB);

	nLoadDB = -1;
	if(!GetExpose_us(&nLoadDB))
		McSetParamInt(m_hDevice, MC_Expose_us, nLoadDB);

	EDeviceGrablinkLineCaptureMode eLineCaptureMode = EDeviceGrablinkLineCaptureMode_Count;
	if(!GetLineCaptureMode(&eLineCaptureMode))
		McSetParamStr(m_hDevice, MC_LineCaptureMode, CStringA(g_lpszGrablinkLineCaptureMode[eLineCaptureMode]));

	EDeviceGrablinkLineRateMode eLineRateMode = EDeviceGrablinkLineRateMode_Count;
	if(!GetLineRateMode(&eLineRateMode))
		McSetParamStr(m_hDevice, MC_LineRateMode, CStringA(g_lpszGrablinkLineRateMode[eLineRateMode]));

	nLoadDB = -1;
	if(!GetExposeRecovery_us(&nLoadDB))
		McSetParamInt(m_hDevice, MC_ExposeRecovery_us, nLoadDB);

	nLoadDB = -1;
	if(!GetFrameRate_mHz(&nLoadDB))
		McSetParamInt(m_hDevice, MC_FrameRate_mHz, nLoadDB);

	EDeviceGrablinkPixelClkMode ePixelClkMode = EDeviceGrablinkPixelClkMode_Count;
	if(!GetPixelClkMode(&ePixelClkMode))
		McSetParamStr(m_hDevice, MC_PixelClkMode, CStringA(g_lpszGrablinkPixelClkMode[ePixelClkMode]));

	nLoadDB = -1;
	if(!GetReadoutRecovery_us(&nLoadDB))
		McSetParamInt(m_hDevice, MC_ReadoutRecovery_us, nLoadDB);

	nLoadDB = -1;
	if(!GetVactive_Ln(&nLoadDB))
		McSetParamInt(m_hDevice, MC_Vactive_Ln, nLoadDB);

	nLoadDB = -1;
	if(!GetVsyncAft_Ln(&nLoadDB))
		McSetParamInt(m_hDevice, MC_VsyncAft_Ln, nLoadDB);

	EDeviceGrablinkColorRegistrationControl eColorRegistrationControl = EDeviceGrablinkColorRegistrationControl_Count;
	if(!GetColorRegistrationControl(&eColorRegistrationControl))
		McSetParamStr(m_hDevice, MC_ColorRegistrationControl, CStringA(g_lpszGrablinkColorRegistrationControl[eColorRegistrationControl]));

	nLoadDB = -1;
	if(!GetExposeMin_us(&nLoadDB))
		McSetParamInt(m_hDevice, MC_ExposeMin_us, nLoadDB);

	nLoadDB = -1;
	if(!GetExposeMax_us(&nLoadDB))
		McSetParamInt(m_hDevice, MC_ExposeMax_us, nLoadDB);

	EDeviceGrablinkExposeOverlap eExposeOverlap = EDeviceGrablinkExposeOverlap_Count;
	if(!GetExposeOverlap(&eExposeOverlap))
		McSetParamStr(m_hDevice, MC_ExposeOverlap, CStringA(g_lpszGrablinkExposeOverlap[eExposeOverlap]));

	EDeviceGrablinkResetCtl eResetCtl = EDeviceGrablinkResetCtl_Count;
	if(!GetResetCtl(&eResetCtl))
		McSetParamStr(m_hDevice, MC_ResetCtl, CStringA(g_lpszGrablinkResetCtl[eResetCtl]));

	EDeviceGrablinkTwoLineSynchronization eTwoLineSynchronization = EDeviceGrablinkTwoLineSynchronization_Count;
	if(!GetTwoLineSynchronization(&eTwoLineSynchronization))
		McSetParamStr(m_hDevice, MC_TwoLineSynchronization, CStringA(g_lpszGrablinkTwoLineSynchronization[eTwoLineSynchronization]));

	EDeviceGrablinkAuxresetLine eAuxresetLine = EDeviceGrablinkAuxresetLine_Count;
	if(!GetAuxresetLine(&eAuxresetLine))
		McSetParamStr(m_hDevice, MC_AuxresetLine, CStringA(g_lpszGrablinkAuxresetLine[eAuxresetLine]));
	
	EDeviceGrablinkResetLine eResetLine = EDeviceGrablinkResetLine_Count;
	if(!GetResetLine(&eResetLine))
		McSetParamStr(m_hDevice, MC_ResetLine, CStringA(g_lpszGrablinkResetLine[eResetLine]));

	nLoadDB = -1;
	if(!GetActivityLength(&nLoadDB))
		McSetParamInt(m_hDevice, MC_ActivityLength, nLoadDB);

	EDeviceGrablinkBreakEffect eBreakEffect = EDeviceGrablinkBreakEffect_Count;
	if(!GetBreakEffect(&eBreakEffect))
		McSetParamStr(m_hDevice, MC_BreakEffect, CStringA(g_lpszGrablinkBreakEffect[eBreakEffect]));

	EDeviceGrablinkEndTrigMode eEndTrigMode = EDeviceGrablinkEndTrigMode_Count;
	if(!GetEndTrigMode(&eEndTrigMode))
		McSetParamStr(m_hDevice, MC_EndTrigMode, CStringA(g_lpszGrablinkEndTrigMode[eEndTrigMode]));

	EDeviceGrablinkNextTrigMode eNextTrigMode = EDeviceGrablinkNextTrigMode_Count;
	if(!GetNextTrigMode(&eNextTrigMode))
		McSetParamStr(m_hDevice, MC_NextTrigMode, CStringA(g_lpszGrablinkNextTrigMode[eNextTrigMode]));
	
	EDeviceGrablinkSynchronizedAcquisition eSynchronizedAcquisition = EDeviceGrablinkSynchronizedAcquisition_Count;
	if(!GetSynchronizedAcquisition(&eSynchronizedAcquisition))
		McSetParamStr(m_hDevice, MC_SynchronizedAcquisition, CStringA(g_lpszGrablinkSynchronizedAcquisition[eSynchronizedAcquisition]));
	
	EDeviceGrablinkForceTrig eForceTrig = EDeviceGrablinkForceTrig_Count;
	if(!GetForceTrig(&eForceTrig))
		McSetParamStr(m_hDevice, MC_ForceTrig, CStringA(g_lpszGrablinkForceTrig[eForceTrig]));

	nLoadDB = -1;
	if(!GetNextTrigDelay_Pls(&nLoadDB))
		McSetParamInt(m_hDevice, MC_NextTrigDelay_Pls, nLoadDB);

	EDeviceGrablinkTrigCtl eTrigCtl = EDeviceGrablinkTrigCtl_Count;
	if(!GetTrigCtl(&eTrigCtl))
		McSetParamStr(m_hDevice, MC_TrigCtl, CStringA(g_lpszGrablinkTrigCtl[eTrigCtl]));

	nLoadDB = -1;
	if(!GetTrigDelay_Pls(&nLoadDB))
		McSetParamInt(m_hDevice, MC_TrigDelay_Pls, nLoadDB);

	nLoadDB = -1;
	if(!GetTrigDelay_us(&nLoadDB))
		McSetParamInt(m_hDevice, MC_TrigDelay_us, nLoadDB);
	
	EDeviceGrablinkTrigEdge eTrigEdge = EDeviceGrablinkTrigEdge_Count;
	if(!GetTrigEdge(&eTrigEdge))
		McSetParamStr(m_hDevice, MC_TrigEdge, CStringA(g_lpszGrablinkTrigEdge[eTrigEdge]));
	
	EDeviceGrablinkTrigFilter eTrigFilter = EDeviceGrablinkTrigFilter_Count;
	if(!GetTrigFilter(&eTrigFilter))
		McSetParamStr(m_hDevice, MC_TrigFilter, CStringA(g_lpszGrablinkTrigFilter[eTrigFilter]));
	
	EDeviceGrablinkTrigLine eTrigLine = EDeviceGrablinkTrigLine_Count;
	if(!GetTrigLine(&eTrigLine))
		McSetParamStr(m_hDevice, MC_TrigLine, CStringA(g_lpszGrablinkTrigLine[eTrigLine]));

	nLoadDB = -1;
	if(!GetPageDelay_Ln(&nLoadDB))
		McSetParamInt(m_hDevice, MC_PageDelay_Ln, nLoadDB);

	nLoadDB = -1;
	if(!GetExposeTrim(&nLoadDB))
		McSetParamInt(m_hDevice, MC_ExposeTrim, nLoadDB);
	
	EDeviceGrablinkStrobeCtl eStrobeCtl = EDeviceGrablinkStrobeCtl_Count;
	if(!GetStrobeCtl(&eStrobeCtl))
		McSetParamStr(m_hDevice, MC_StrobeCtl, CStringA(g_lpszGrablinkStrobeCtl[eStrobeCtl]));

	nLoadDB = -1;
	if(!GetStrobeDur(&nLoadDB))
		McSetParamInt(m_hDevice, MC_StrobeDur, nLoadDB);
	
	EDeviceGrablinkStrobeMode eStrobeMode = EDeviceGrablinkStrobeMode_Count;
	if(!GetStrobeMode(&eStrobeMode))
		McSetParamStr(m_hDevice, MC_StrobeMode, CStringA(g_lpszGrablinkStrobeMode[eStrobeMode]));

	nLoadDB = -1;
	if(!GetStrobePos(&nLoadDB))
		McSetParamInt(m_hDevice, MC_StrobePos, nLoadDB);

	nLoadDB = -1;
	if(!GetPreStrobe_us(&nLoadDB))
		McSetParamInt(m_hDevice, MC_PreStrobe_us, nLoadDB);
	
	EDeviceGrablinkLineTrigCtl eLineTrigCtl = EDeviceGrablinkLineTrigCtl_Count;
	if(!GetLineTrigCtl(&eLineTrigCtl))
		McSetParamStr(m_hDevice, MC_LineTrigCtl, CStringA(g_lpszGrablinkLineTrigCtl[eLineTrigCtl]));
	
	EDeviceGrablinkLineTrigEdge eLineTrigEdge = EDeviceGrablinkLineTrigEdge_Count;
	if(!GetLineTrigEdge(&eLineTrigEdge))
		McSetParamStr(m_hDevice, MC_LineTrigEdge, CStringA(g_lpszGrablinkLineTrigEdge[eLineTrigEdge]));
	
	EDeviceGrablinkLineTrigFilter eLineTrigFilter = EDeviceGrablinkLineTrigFilter_Count;
	if(!GetLineTrigFilter(&eLineTrigFilter))
		McSetParamStr(m_hDevice, MC_LineTrigFilter, CStringA(g_lpszGrablinkLineTrigFilter[eLineTrigFilter]));

	nLoadDB = -1;
	if(!GetRateDivisionFactor(&nLoadDB))
		McSetParamInt(m_hDevice, MC_RateDivisionFactor, nLoadDB);

	EDeviceGrablinkLineTrigLine eLineTrigLine = EDeviceGrablinkLineTrigLine_Count;
	if(!GetLineTrigLine(&eLineTrigLine))
		McSetParamStr(m_hDevice, MC_LineTrigLine, CStringA(g_lpszGrablinkLineTrigLine[eLineTrigLine]));
	
	EDeviceGrablinkRateDividerRestart eRateDividerRestart = EDeviceGrablinkRateDividerRestart_Count;
	if(!GetRateDividerRestart(&eRateDividerRestart))
		McSetParamStr(m_hDevice, MC_RateDividerRestart, CStringA(g_lpszGrablinkRateDividerRestart[eRateDividerRestart]));
	
	EDeviceGrablinkECCO_PLLResetControl eECCO_PLLResetControl = EDeviceGrablinkECCO_PLLResetControl_Count;
	if(!GetECCO_PLLResetControl(&eECCO_PLLResetControl))
		McSetParamStr(m_hDevice, MC_ECCO_PLLResetControl, CStringA(g_lpszGrablinkECCO_PLLResetControl[eECCO_PLLResetControl]));
	
	EDeviceGrablinkECCO_SkewCompensation eECCO_SkewCompensation = EDeviceGrablinkECCO_SkewCompensation_Count;
	if(!GetECCO_SkewCompensation(&eECCO_SkewCompensation))
		McSetParamStr(m_hDevice, MC_ECCO_SkewCompensation, CStringA(g_lpszGrablinkECCO_SkewCompensation[eECCO_SkewCompensation]));
	
	EDeviceGrablinkFvalMin_Tk eFvalMin_Tk = EDeviceGrablinkFvalMin_Tk_Count;
	if(!GetFvalMin_Tk(&eFvalMin_Tk))
		McSetParamStr(m_hDevice, MC_FvalMin_Tk, CStringA(g_lpszGrablinkFvalMin_Tk[eFvalMin_Tk]));
	
	EDeviceGrablinkLvalMin_Tk eLvalMin_Tk = EDeviceGrablinkLvalMin_Tk_Count;
	if(!GetLvalMin_Tk(&eLvalMin_Tk))
		McSetParamStr(m_hDevice, MC_LvalMin_Tk, CStringA(g_lpszGrablinkLvalMin_Tk[eLvalMin_Tk]));
	
	EDeviceGrablinkMetadataInsertion eMetadataInsertion = EDeviceGrablinkMetadataInsertion_Count;
	if(!GetMetadataInsertion(&eMetadataInsertion))
		McSetParamStr(m_hDevice, MC_MetadataInsertion, CStringA(g_lpszGrablinkMetadataInsertion[eMetadataInsertion]));
	
	EDeviceGrablinkGrabWindow eGrabWindow = EDeviceGrablinkGrabWindow_Count;
	if(!GetGrabWindow(&eGrabWindow))
		McSetParamStr(m_hDevice, MC_GrabWindow, CStringA(g_lpszGrablinkGrabWindow[eGrabWindow]));
	
	if(eGrabWindow == EDeviceGrablinkGrabWindow_MAN)
	{
		int nOffsetX_Px = 0;
		if(!GetOffsetX_Px(&nOffsetX_Px))
			McSetParamInt(m_hDevice, MC_OffsetX_Px, nOffsetX_Px);

		int nWindowX_Px = 0;
		if(!GetWindowX_Px(&nWindowX_Px))
			McSetParamInt(m_hDevice, MC_WindowX_Px, nWindowX_Px);
	}

	EDeviceGrablinkColorFormat eColorFormat = EDeviceGrablinkColorFormat_Count;
	if(!GetColorFormat(&eColorFormat))
		McSetParamStr(m_hDevice, MC_ColorFormat, CStringA(g_lpszGrablinkColorFormat[eColorFormat]));
	
	EDeviceGrablinkImageFlipX eImageFlipX = EDeviceGrablinkImageFlipX_Count;
	if(!GetImageFlipX(&eImageFlipX))
		McSetParamStr(m_hDevice, MC_ImageFlipX, CStringA(g_lpszGrablinkImageFlipX[eImageFlipX]));

	EDeviceGrablinkImageFlipY eImageFlipY = EDeviceGrablinkImageFlipY_Count;
	if(!GetImageFlipY(&eImageFlipY))
		McSetParamStr(m_hDevice, MC_ImageFlipY, CStringA(g_lpszGrablinkImageFlipY[eImageFlipY]));
	
	EDeviceGrablinkRedBlueSwap eRedBlueSwap = EDeviceGrablinkRedBlueSwap_Count;
	if(!GetRedBlueSwap(&eRedBlueSwap))
		McSetParamStr(m_hDevice, MC_RedBlueSwap, CStringA(g_lpszGrablinkRedBlueSwap[eRedBlueSwap]));
	
	EDeviceGrablinkMaxFillingSurfaces eMaxFillingSurfaces = EDeviceGrablinkMaxFillingSurfaces_Count;
	if(!GetMaxFillingSurfaces(&eMaxFillingSurfaces))
		McSetParamStr(m_hDevice, MC_MaxFillingSurfaces, CStringA(g_lpszGrablinkMaxFillingSurfaces[eMaxFillingSurfaces]));

	nLoadDB = -1;
	if(!GetSurfaceCount(&nLoadDB))
		McSetParamInt(m_hDevice, MC_SurfaceCount, nLoadDB);
	
	EDeviceGrablinkAcquisitionCleanup eAcquisitionCleanup = EDeviceGrablinkAcquisitionCleanup_Count;
	if(!GetAcquisitionCleanup(&eAcquisitionCleanup))
		McSetParamStr(m_hDevice, MC_AcquisitionCleanup, CStringA(g_lpszGrablinkAcquisitionCleanup[eAcquisitionCleanup]));

	nLoadDB = -1;
	if(!GetAcqTimeout_ms(&nLoadDB))
		McSetParamInt(m_hDevice, MC_AcqTimeout_ms, nLoadDB);
	
	EDeviceGrablinkParamsConsistencyCheck eParamsConsistencyCheck = EDeviceGrablinkParamsConsistencyCheck_Count;
	if(!GetParamsConsistencyCheck(&eParamsConsistencyCheck))
		McSetParamStr(m_hDevice, MC_ParamsConsistencyCheck, CStringA(g_lpszGrablinkParamsConsistencyCheck[eParamsConsistencyCheck]));
	
	EDeviceGrablinkCallbackPriority eCallbackPriority = EDeviceGrablinkCallbackPriority_Count;
	if(!GetCallbackPriority(&eCallbackPriority))
		McSetParamStr(m_hDevice, MC_CallbackPriority, CStringA(g_lpszGrablinkCallbackPriority[eCallbackPriority]));

	EDeviceGrablinkInterleavedAcquisition eInterleavedAcquisition = EDeviceGrablinkInterleavedAcquisition_Count;
	if(!GetInterleavedAcquisition(&eInterleavedAcquisition))
		McSetParamStr(m_hDevice, MC_InterleavedAcquisition, CStringA(g_lpszGrablinkInterleavedAcquisition[eInterleavedAcquisition]));

	nLoadDB = -1;
	if(!GetExposureTime_P1_us(&nLoadDB))
		McSetParamInt(m_hDevice, MC_ExposureTime_P1_us, nLoadDB);

	nLoadDB = -1;
	if(!GetExposureTime_P2_us(&nLoadDB))
		McSetParamInt(m_hDevice, MC_ExposureTime_P2_us, nLoadDB);

	nLoadDB = -1;
	if(!GetStrobeDuration_P1_us(&nLoadDB))
		McSetParamInt(m_hDevice, MC_StrobeDuration_P1_us, nLoadDB);

	nLoadDB = -1;
	if(!GetStrobeDuration_P2_us(&nLoadDB))
		McSetParamInt(m_hDevice, MC_StrobeDuration_P2_us, nLoadDB);

	nLoadDB = -1;
	if(!GetStrobeDelay_P1_us(&nLoadDB))
		McSetParamInt(m_hDevice, MC_StrobeDelay_P1_us, nLoadDB);

	nLoadDB = -1;
	if(!GetStrobeDelay_P2_us(&nLoadDB))
		McSetParamInt(m_hDevice, MC_StrobeDelay_P2_us, nLoadDB);

	nLoadDB = -1;
	if(!GetExposureDelay_MAN_P1_us(&nLoadDB))
		McSetParamInt(m_hDevice, MC_ExposureDelay_MAN_P1_us, nLoadDB);

	nLoadDB = -1;
	if(!GetExposureDelay_MAN_P2_us(&nLoadDB))
		McSetParamInt(m_hDevice, MC_ExposureDelay_MAN_P2_us, nLoadDB);
}

void CDeviceEuresysGrablink::SetUpdateParameter()
{
	char strSaveDB[32];
	int nSaveDB = -1;

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_CamConfig, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkCamConfig, EDeviceGrablinkCamConfig_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_CamConfig, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_CamConfig);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_Camera, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkCamera, EDeviceGrablinkCamera_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_Camera, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_Camera);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_DataLink, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkDataLink, EDeviceGrablinkDataLink_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_DataLink, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_DataLink);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_Imaging, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkImaging, EDeviceGrablinkImaging_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_Imaging, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_Imaging);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_Spectrum, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkSpectrum, EDeviceGrablinkSpectrum_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_Spectrum, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_Spectrum);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_AcquisitionMode, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkAcquisitionMode, EDeviceGrablinkAcquisitionMode_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_AcquisitionMode, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_AcquisitionMode);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_TrigMode, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkTrigMode, EDeviceGrablinkTrigMode_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_TrigMode, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_TrigMode);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_Expose_us, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysGrablink_Expose_us, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_Expose_us);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_SeqLength_Fr, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysGrablink_SeqLength_Fr, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_SeqLength_Fr);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_PhaseLength_Fr, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysGrablink_PhaseLength_Fr, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_PhaseLength_Fr);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_SeqLength_Ln, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysGrablink_SeqLength_Ln, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_SeqLength_Ln);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_SeqLength_Pg, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysGrablink_SeqLength_Pg, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_SeqLength_Pg);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_PageLength_Ln, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysGrablink_PageLength_Ln, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_PageLength_Ln);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_LineCaptureMode, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkLineCaptureMode, EDeviceGrablinkLineCaptureMode_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_LineCaptureMode, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_LineCaptureMode);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_LineRateMode, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkLineRateMode, EDeviceGrablinkLineRateMode_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_LineRateMode, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_LineRateMode);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_ExposeRecovery_us, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysGrablink_ExposeRecovery_us, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_ExposeRecovery_us);
	}
	
	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_FrameRate_mHz, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysGrablink_FrameRate_mHz, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_FrameRate_mHz);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_Hactive_Px, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysGrablink_Hactive_Px, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_Hactive_Px);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_HsyncAft_Tk, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysGrablink_HsyncAft_Tk, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_HsyncAft_Tk);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_PixelClkMode, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkPixelClkMode, EDeviceGrablinkPixelClkMode_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_PixelClkMode, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_PixelClkMode);
	}
	
	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_Vactive_Ln, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysGrablink_Vactive_Ln, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_Vactive_Ln);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_VsyncAft_Ln, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysGrablink_VsyncAft_Ln, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_VsyncAft_Ln);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_LineRate_Hz, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysGrablink_LineRate_Hz, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_LineRate_Hz);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_TapConfiguration, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkTapConfiguration, EDeviceGrablinkTapConfiguration_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_TapConfiguration, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_TapConfiguration);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_AuxResetCtl, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkAuxResetCtl, EDeviceGrablinkAuxResetCtl_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_AuxResetCtl, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_AuxResetCtl);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_CC1Usage, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkCC1Usage, EDeviceGrablinkCC1Usage_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_CC1USage, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_CC1USage);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_CC2Usage, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkCC2Usage, EDeviceGrablinkCC2Usage_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_CC2Usage, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_CC2Usage);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_CC3Usage, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkCC3Usage, EDeviceGrablinkCC3Usage_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_CC3Usage, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_CC3Usage);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_CC4Usage, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkCC4Usage, EDeviceGrablinkCC4Usage_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_CC4Usage, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_CC4Usage);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_ColorMethod, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkColorMethod, EDeviceGrablinkColorMethod_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_ColorMethod, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_ColorMethod);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_ColorRegistrationControl, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkColorRegistrationControl, EDeviceGrablinkColorRegistrationControl_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_ColorRegistrationControl, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_ColorRegistrationControl);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_DvalMode, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkDvalMode, EDeviceGrablinkDvalMode_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_DvalMode, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_DvalMode);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_Expose, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkExpose, EDeviceGrablinkExpose_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_Expose, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_Expose);
	}
	
	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_ExposeMin_us, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysGrablink_ExposeMin_us, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_ExposeMin_us);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_ExposeMax_us, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysGrablink_ExposeMax_us, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_ExposeMax_us);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_ExposeOverlap, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkExposeOverlap, EDeviceGrablinkExposeOverlap_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_ExposeOverlap, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_ExposeOverlap);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_FvalMode, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkFvalMode, EDeviceGrablinkFvalMode_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_FvalMode, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_FvalMode);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_LvalMode, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkLvalMode, EDeviceGrablinkLvalMode_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_LvalMode, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_LvalMode);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_Readout, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkReadout, EDeviceGrablinkReadout_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_Readout, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_Readout);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_ResetCtl, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkResetCtl, EDeviceGrablinkResetCtl_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_ResetCtl, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_ResetCtl);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_ResetEdge, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkResetEdge, EDeviceGrablinkResetEdge_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_ResetEdge, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_ResetEdge);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_TwoLineSynchronization, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkTwoLineSynchronization, EDeviceGrablinkTwoLineSynchronization_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_TwoLineSynchronization, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_TwoLineSynchronization);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_TapGeometry, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkTapGeometry, EDeviceGrablinkTapGeometry_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_TapGeometry, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_TapGeometry);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_AuxresetLine, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkAuxresetLine, EDeviceGrablinkAuxresetLine_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_AuxresetLine, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_AuxresetLine);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_ResetLine, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkResetLine, EDeviceGrablinkResetLine_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_ResetLine, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_ResetLine);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_ActivityLength, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysGrablink_ActivityLength, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_ActivityLength);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_BreakEffect, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkBreakEffect, EDeviceGrablinkBreakEffect_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_BreakEffect, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_BreakEffect);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_EndTrigMode, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkEndTrigMode, EDeviceGrablinkEndTrigMode_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_EndTrigMode, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_EndTrigMode);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_NextTrigMode, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkNextTrigMode, EDeviceGrablinkNextTrigMode_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_NextTrigMode, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_NextTrigMode);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_SynchronizedAcquisition, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkSynchronizedAcquisition, EDeviceGrablinkSynchronizedAcquisition_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_SynchronizedAcquisition, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_SynchronizedAcquisition);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_ForceTrig, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkForceTrig, EDeviceGrablinkForceTrig_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_ForceTrig, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_ForceTrig);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_NextTrigDelay_Pls, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysGrablink_NextTrigDelay_Pls, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_NextTrigDelay_Pls);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_TrigCtl, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkTrigCtl, EDeviceGrablinkTrigCtl_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_TrigCtl, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_TrigCtl);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_TrigDelay_Pls, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysGrablink_TrigDelay_Pls, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_TrigDelay_Pls);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_TrigDelay_us, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysGrablink_TrigDelay_us, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_TrigDelay_us);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_TrigEdge, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkTrigEdge, EDeviceGrablinkTrigEdge_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_TrigEdge, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_TrigEdge);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_TrigFilter, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkTrigFilter, EDeviceGrablinkTrigFilter_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_TrigFilter, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_TrigFilter);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_TrigLine, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkTrigLine, EDeviceGrablinkTrigLine_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_TrigLine, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_TrigLine);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_PageDelay_Ln, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysGrablink_PageDelay_Ln, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_PageDelay_Ln);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_ExposeTrim, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysGrablink_ExposeTrim, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_ExposeTrim);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_StrobeCtl, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkStrobeCtl, EDeviceGrablinkStrobeCtl_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_StrobeCtl, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_StrobeCtl);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_StrobeDur, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysGrablink_StrobeDur, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_StrobeDur);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_StrobeMode, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkStrobeMode, EDeviceGrablinkStrobeMode_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_StrobeMode, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_StrobeMode);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_StrobePos, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysGrablink_StrobePos, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_StrobePos);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_PreStrobe_us, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysGrablink_PreStrobe_us, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_PreStrobe_us);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_LineTrigCtl, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkLineTrigCtl, EDeviceGrablinkLineTrigCtl_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_LineTrigCtl, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_LineTrigCtl);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_LineTrigEdge, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkLineTrigEdge, EDeviceGrablinkLineTrigEdge_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_LineTrigEdge, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_LineTrigEdge);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_LineTrigFilter, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkLineTrigFilter, EDeviceGrablinkLineTrigFilter_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_LineTrigFilter, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_LineTrigFilter);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_RateDivisionFactor, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysGrablink_RateDivisionFactor, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_RateDivisionFactor);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_LineTrigLine, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkLineTrigLine, EDeviceGrablinkLineTrigLine_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_LineTrigLine, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_LineTrigLine);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_RateDividerRestart, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkRateDividerRestart, EDeviceGrablinkRateDividerRestart_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_RateDividerRestart, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_RateDividerRestart);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_ECCO_PLLResetControl, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkECCO_PLLResetControl, EDeviceGrablinkECCO_PLLResetControl_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_ECCO_PLLResetControl, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_ECCO_PLLResetControl);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_ECCO_SkewCompensation, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkECCO_SkewCompensation, EDeviceGrablinkECCO_SkewCompensation_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_ECCO_SkewCompensation, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_ECCO_SkewCompensation);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_FvalMin_Tk, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkFvalMin_Tk, EDeviceGrablinkFvalMin_Tk_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_FvalMin_Tk, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_FvalMin_Tk);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_LvalMin_Tk, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkLvalMin_Tk, EDeviceGrablinkLvalMin_Tk_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_LvalMin_Tk, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_LvalMin_Tk);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_MetadataInsertion, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkMetadataInsertion, EDeviceGrablinkMetadataInsertion_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_MetadataInsertion, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_MetadataInsertion);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_GrabWindow, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkGrabWindow, EDeviceGrablinkGrabWindow_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_GrabWindow, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_GrabWindow);
	}

	EDeviceGrablinkGrabWindow eType = EDeviceGrablinkGrabWindow_Count;

	if(!GetGrabWindow(&eType))
	{
		if(eType == EDeviceGrablinkGrabWindow_MAN)
		{
			nSaveDB = -1;

			if(McGetParamInt(m_hDevice, MC_OffsetX_Px, &nSaveDB) == MC_OK)
			{
				CString strSave;
				strSave.Format(_T("%d"), nSaveDB);

				SetParamValue(EDeviceParameterEuresysGrablink_OffsetX_Px, strSave);
				SaveSettings(EDeviceParameterEuresysGrablink_OffsetX_Px);
			}
			
			nSaveDB = -1;

			if(McGetParamInt(m_hDevice, MC_WindowX_Px, &nSaveDB) == MC_OK)
			{
				CString strSave;
				strSave.Format(_T("%d"), nSaveDB);

				SetParamValue(EDeviceParameterEuresysGrablink_WindowX_Px, strSave);
				SaveSettings(EDeviceParameterEuresysGrablink_WindowX_Px);
			}
		}
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_ColorFormat, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkColorFormat, EDeviceGrablinkColorFormat_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_ColorFormat, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_ColorFormat);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_ImageFlipX, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkImageFlipX, EDeviceGrablinkImageFlipX_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_ImageFlipX, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_ImageFlipX);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_ImageFlipY, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkImageFlipY, EDeviceGrablinkImageFlipY_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_ImageFlipY, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_ImageFlipY);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_RedBlueSwap, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkRedBlueSwap, EDeviceGrablinkRedBlueSwap_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_RedBlueSwap, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_RedBlueSwap);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_MaxFillingSurfaces, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkMaxFillingSurfaces, EDeviceGrablinkMaxFillingSurfaces_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_MaxFillingSurfaces, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_MaxFillingSurfaces);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_SurfaceCount, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysGrablink_SurfaceCount, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_SurfaceCount);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_AcquisitionCleanup, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkAcquisitionCleanup, EDeviceGrablinkAcquisitionCleanup_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_AcquisitionCleanup, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_AcquisitionCleanup);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_AcqTimeout_ms, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysGrablink_AcqTimeout_ms, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_AcqTimeout_ms);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_ParamsConsistencyCheck, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkParamsConsistencyCheck, EDeviceGrablinkParamsConsistencyCheck_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_ParamsConsistencyCheck, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_ParamsConsistencyCheck);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_CallbackPriority, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszGrablinkCallbackPriority, EDeviceGrablinkCallbackPriority_Count));

		SetParamValue(EDeviceParameterEuresysGrablink_CallbackPriority, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_CallbackPriority);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_ExposureTime_P1_us, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysGrablink_ExposureTime_P1_us, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_ExposureTime_P1_us);
	}
	
	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_ExposureTime_P2_us, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysGrablink_ExposureTime_P2_us, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_ExposureTime_P2_us);
	}
	
	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_StrobeDuration_P1_us, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysGrablink_StrobeDuration_P1_us, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_StrobeDuration_P1_us);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_StrobeDuration_P2_us, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysGrablink_StrobeDuration_P2_us, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_StrobeDuration_P2_us);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_StrobeDelay_P1_us, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysGrablink_StrobeDelay_P1_us, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_StrobeDelay_P1_us);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_StrobeDelay_P2_us, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysGrablink_StrobeDelay_P2_us, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_StrobeDelay_P2_us);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_ExposureDelay_MAN_P1_us, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysGrablink_ExposureDelay_MAN_P1_us, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_ExposureDelay_MAN_P1_us);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_ExposureDelay_MAN_P2_us, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysGrablink_ExposureDelay_MAN_P2_us, strSave);
		SaveSettings(EDeviceParameterEuresysGrablink_ExposureDelay_MAN_P2_us);
	}
}

#endif