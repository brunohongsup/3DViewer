#include "stdafx.h"

#include "DeviceEuresysDomino.h"

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

static LPCTSTR g_lpszDeviceDomino[EDeviceParameterEuresysDomino_Count - EDeviceParameterEuresysDominoSymphony_Count] =
{
	_T("Summary Control"),
	_T("AcquisitionMode"),
	_T("TrigMode"),
	_T("Expose_us"),
	_T("Area Camera"),
	_T("SeqLength_Fr"),
	_T("PhaseLength_Fr"),
	_T("TapStructure"),
	_T("Gain"),
	_T("Offset"),
	_T("VideoFilter"),
	_T("Camera Specification"),
	_T("CamConfig"),
	_T("Camera"),
	_T("DataLink"),
	_T("Imaging"),
	_T("Spectrum"),
	_T("Camera Timing"),
	_T("Vactive_Ln"),
	_T("VsyncAft_Ln"),
	_T("LineRate_Hz"),
	_T("ACResetPostDelay_us"),
	_T("ACResetWidth_us"),
	_T("HCSyncAft_ns"),
	_T("HCsyncBfr_ns"),
	_T("HCsyncDur_ns"),
	_T("HsyncDly_ns"),
	_T("HdriveDur_ns"),
	_T("HdriveDly_ns"),
	_T("Hactive_ns"),
	_T("PixelClk_Hz"),
	_T("Scanning"),
	_T("Standard"),
	_T("VCsyncAft_Ln"),
	_T("VCgatePos_Ln"),
	_T("Vtotal_Ln"),
	_T("VsyncBfrEndExp_us"),
	_T("VdriveDur_Ln"),
	_T("VdriveDly_Ln"),
	_T("Camera Features"),
	_T("AuxResetCtl"),
	_T("ColorMethod"),
	_T("Expose"),
	_T("ExposeMin_us"),
	_T("ExposeMax_us"),
	_T("ExposeOverlap"),
	_T("Readout"),
	_T("ResetCtl"),
	_T("ResetEdge"),
	_T("ACReset"),
	_T("ChannelTopology"),
	_T("CsyncCtl"),
	_T("CsyncEdge"),
	_T("CameraControlSignalSet"),
	_T("CameraSignalSet"),
	_T("HReset"),
	_T("HsyncCtl"),
	_T("HsyncEdge"),
	_T("HVdriveCtl"),
	_T("HVdriveEdge"),
	_T("PixelClkCtl"),
	_T("ResetDur"),
	_T("ResetPos"),
	_T("VCSync"),
	_T("VHSync"),
	_T("VsyncCtl"),
	_T("VsyncEdge"),
	_T("Cable Features"),
	_T("AuxResetLine"),
	_T("ResetLine"),
	_T("Cable"),
	_T("CsyncLine"),
	_T("HdriveLine"),
	_T("HsyncLine"),
	_T("PixelClkLine"),
	_T("VdriveLine"),
	_T("VsyncLine"),
	_T("CableTopology"),
	_T("Acquisition Control"),
	_T("ActivityLength"),
	_T("BreakEffect"),
	_T("EndTrigMode"),
	_T("NextTrigMode"),
	_T("GrabField"),
	_T("NextGrabField"),
	_T("Trigger Control"),
	_T("ForceTrig"),
	_T("TrigDelay_us"),
	_T("Exposure Control"),
	_T("ExposeTrim"),
	_T("TrueExp_us"),
	_T("Strobe Control"),
	_T("StrobeCtl"),
	_T("StrobeDur"),
	_T("StrobeMode"),
	_T("StrobePos"),
	_T("PreStrobe_us"),
	_T("StrobeLine"),
	_T("StrobeLevel"),
	_T("Grabber Configuration"),
	_T("ADresolution"),
	_T("VideoTerminator"),
	_T("Grabber Timing"),
	_T("GrabWindow"),
	_T("OffsetX_Px"),
	_T("OffsetY_Ln"),
	_T("SampleTrim"),
	_T("SampleClk_Hz"),
	_T("Grabber Conditioning"),
	_T("GainCtl"),
	_T("CalPat"),
	_T("InputLut"),
	_T("LutIndex"),
	_T("Board Linkage"),
	_T("BoardIdentifier"),
	_T("BoardName"),
	_T("DriverIndex"),
	_T("PciPosition"),
	_T("Cluster"),
	_T("ColorFormat"),
	_T("ImageFlipX"),
	_T("ImageFlipY"),
	_T("ImageSizeX"),
	_T("SurfaceCount"),
	_T("SurfaceAllocation"),
	_T("Channel Management"),
	_T("ChannelState"),
	_T("Exception Management"),
	_T("AcquisitionCleanup"),
	_T("AcqTimeout_ms"),
	_T("ParamsConsistencyCheck"),
};


static LPCTSTR g_lpszDominoVideoFilter[EDeviceDominoVideoFilter_Count] =
{
	_T("NONE"),
	_T("COLORTRAP"),
};

static LPCTSTR g_lpszDominoTapStructure[EDeviceDominoTapStructure_Count] =
{
	_T("SINGLE_A"),
	_T("TRIPLE_A"),
};

static LPCTSTR g_lpszDominoScanning[EDeviceDominoScanning_Count] =
{
	_T("PROGRESSIVE"),
	_T("INTERLACE"),
};

static LPCTSTR g_lpszDominoStandard[EDeviceDominoStandard_Count] =
{
	_T("CCIR"),
	_T("EIA"),
};

static LPCTSTR g_lpszDominoACReset[EDeviceDominoACReset_Count] =
{
	_T("NO"),
	_T("YES"),
};

static LPCTSTR g_lpszDominoChannelTopology[EDeviceDominoChannelTopology_Count] =
{
	_T("SINGLE"),
	_T("TRIPLE"),
};

static LPCTSTR g_lpszDominoCsyncCtl[EDeviceDominoCsyncCtl_Count] =
{
	_T("NONE"),
	_T("TTL"),
	_T("DIFF"),
};

static LPCTSTR g_lpszDominoCsyncEdge[EDeviceDominoCsyncEdge_Count] =
{
	_T("GOHIGH"),
	_T("GOLOW"),
};

static LPCTSTR g_lpszDominoCameraControlSignalSet[EDeviceDominoCameraControlSignalSet_Count] =
{
	_T("CSYNC"),
	_T("HDRIVE"),
	_T("HDRIVE_RESET"),
	_T("HDRIVE_RESET_VSYNC"),
	_T("HDRIVE_VDRIVE"),
	_T("HDRIVE_VSYNC"),
	_T("HSYNC"),
	_T("HSYNC_VSYNC"),
	_T("INVALID"),
	_T("NONE"),
	_T("RESET"),
	_T("RESET_HSYNC"),
	_T("RESET_HSYNC_VSYNC"),
	_T("RESET_VSYNC"),
	_T("UNAVAILABLE_PxxRA_M"),
	_T("UNAVAILABLE_PxxRM_1"),
	_T("UNAVAILABLE_PxxRM_2"),
};

static LPCTSTR g_lpszDominoHReset[EDeviceDominoHReset_Count] =
{
	_T("NO"),
	_T("YES"),
};

static LPCTSTR g_lpszDominoHsyncCtl[EDeviceDominoHsyncCtl_Count] =
{
	_T("NONE"),
	_T("TTL"),
	_T("DIFF"),
};

static LPCTSTR g_lpszDominoHsyncEdge[EDeviceDominoHsyncEdge_Count] =
{
	_T("GOHIGH"),
	_T("GOLOW"),
};

static LPCTSTR g_lpszDominoHVdriveCtl[EDeviceDominoHVdriveCtl_Count] =
{
	_T("NONE"),
	_T("TTL"),
	_T("DIFF"),
};

static LPCTSTR g_lpszDominoHVdriveEdge[EDeviceDominoHVdriveEdge_Count] =
{
	_T("GOHIGH"),
	_T("GOLOW"),
};

static LPCTSTR g_lpszDominoPixelClkCtl[EDeviceDominoPixelClkCtl_Count] =
{
	_T("NONE"),
};

static LPCTSTR g_lpszDominoVCSync[EDeviceDominoVCSync_Count] =
{
	_T("YES"),
	_T("NO"),
};

static LPCTSTR g_lpszDominoVHSync[EDeviceDominoVHSync_Count] =
{
	_T("YES"),
	_T("NO"),
};

static LPCTSTR g_lpszDominoVsyncCtl[EDeviceDominoVsyncCtl_Count] =
{
	_T("NONE"),
	_T("TTL"),
	_T("DIFF"),
};

static LPCTSTR g_lpszDominoVsyncEdge[EDeviceDominoVsyncEdge_Count] =
{
	_T("GOHIGH"),
	_T("GOLOW"),
};

static LPCTSTR g_lpszDominoCable[EDeviceDominoCable_Count] =
{
	_T("A15_C00"),
	_T("A15_C01"),
	_T("A15_C02"),
	_T("A15_C03"),
	_T("A15_C05"),
	_T("A15_C06"),
	_T("A15_C07"),
	_T("A15_C08"),
	_T("A15_C09"),
	_T("A15_C11"),
	_T("A15_C12"),
	_T("A15_C13"),
	_T("A15_C14"),
	_T("A15_C15"),
	_T("A15_C16"),
	_T("A15_C17"),
	_T("A15_C18"),
	_T("A15_C19"),
	_T("A15_C20"),
	_T("A15_C21"),
};

static LPCTSTR g_lpszDominoCableName[EDeviceDominoCableName_Count] =
{
	_T("My_Cable"),
	_T("GenericCable"),
	_T("Single_Channel_R"),
	_T("Single_Channel_R_B"),
	_T("Single_Channel_R_C"),
	_T("Single_Channel_RC"),
	_T("Single_Channel_RG"),
	_T("Single_Channel_RGC"),
	_T("Single_Channel_RGC_A"),
	_T("Single_Channel_RGC_wo_PxClk"),
	_T("Single_Channel_RGC_2"),
	_T("Dual_Channel"),
	_T("Dual_Channel_RG"),
	_T("Dual_Channel_RG_2"),
	_T("Dual_Channel_RG_2_A"),
	_T("Triple_Channel_R"),
	_T("Triple_Channel_RG_2"),
	_T("Triple_Channel_RG_3"),
};

static LPCTSTR g_lpszDominoAuxResetLine[EDeviceDominoAuxResetLine_Count] =
{
	_T("NC"),
	_T("CC5"),
	_T("CC3"),
};

static LPCTSTR g_lpszDominoCsyncLine[EDeviceDominoCsyncLine_Count] =
{
	_T("CC1"),
	_T("CC2"),
	_T("CC3"),
	_T("CC4"),
	_T("CC5"),
	_T("NC"),
};

static LPCTSTR g_lpszDominoHdriveLine[EDeviceDominoHdriveLine_Count] =
{
	_T("CC1"),
	_T("CC2"),
	_T("CC3"),
	_T("CC4"),
	_T("CC5"),
	_T("NC"),
};

static LPCTSTR g_lpszDominoHsyncLine[EDeviceDominoHsyncLine_Count] =
{
	_T("CC1"),
	_T("CC2"),
	_T("CC3"),
	_T("CC4"),
	_T("CC5"),
	_T("NC"),
};

static LPCTSTR g_lpszDominoPixelClkLine[EDeviceDominoPixelClkLine_Count] =
{
	_T("NC"),
};

static LPCTSTR g_lpszDominoVdriveLine[EDeviceDominoVdriveLine_Count] =
{
	_T("CC1"),
	_T("CC2"),
	_T("CC3"),
	_T("CC4"),
	_T("CC5"),
	_T("NC"),
};

static LPCTSTR g_lpszDominoVsyncLine[EDeviceDominoVsyncLine_Count] =
{
	_T("CC1"),
	_T("CC2"),
	_T("CC3"),
	_T("CC4"),
	_T("CC5"),
	_T("NC"),
};

static LPCTSTR g_lpszDominoGrabField[EDeviceDominoGrabField_Count] =
{
	_T("FLD"),
	_T("UP"),
	_T("DOWN"),
	_T("FRAME"),
	_T("UPDW"),
	_T("DWUP"),
};

static LPCTSTR g_lpszDominoNextGrabField[EDeviceDominoNextGrabField_Count] =
{
	_T("FLD"),
	_T("UP"),
	_T("DOWN"),
	_T("FRAME"),
	_T("UPDW"),
	_T("DWUP"),
	_T("SAME"),
	_T("KEEP"),
	_T("CHANGE"),
};

static LPCTSTR g_lpszDominoStrobeLine[EDeviceDominoStrobeLine_Count] =
{
	_T("NOM"),
	_T("IO1"),
	_T("IO2"),
	_T("IO3"),
	_T("IO4"),
	_T("ST"),
	_T("STX"),
	_T("STY"),
	_T("STA"),
	_T("STB"),
	_T("STC"),
	_T("STD"),
};

static LPCTSTR g_lpszDominoStrobeLevel[EDeviceDominoStrobeLevel_Count] =
{
	_T("PLSHIGH"),
	_T("PLSLOW"),
};

static LPCTSTR g_lpszDominoADresolution[EDeviceDominoADresolution_Count] =
{
	_T("10BIT"),
};

static LPCTSTR g_lpszDominoVideoTerminator[EDeviceDominoVideoTerminator_Count] =
{
	_T("ON"),
	_T("OFF"),
};

static LPCTSTR g_lpszDominoHMode[EDeviceDominoHMode_Count] =
{
	_T("ANALOG_LANE1"),
	_T("DIGITAL_PULSED"),
	_T("DIGITAL_MIXED"),
	_T("LOCAL"),
};

static LPCTSTR g_lpszDominoHReference[EDeviceDominoHReference_Count] =
{
	_T("CAMERA"),
	_T("LOCAL"),
};

static LPCTSTR g_lpszDominoVMode[EDeviceDominoVMode_Count] =
{
	_T("ANALOG_LANE1"),
	_T("DIGITAL_PULSED"),
	_T("DIGITAL_MIXED"),
	_T("LOCAL"),
};

static LPCTSTR g_lpszDominoVReference[EDeviceDominoVReference_Count] =
{
	_T("CAMERA"),
	_T("LOCAL"),
};

static LPCTSTR g_lpszDominoGainCtl[EDeviceDominoGainCtl_Count] =
{
	_T("LIN"),
	_T("P3DB"),
	_T("P2DB"),
	_T("P1DB"),
	_T("0DB"),
	_T("M1dB"),
	_T("M2dB"),
	_T("M3dB"),
};

static LPCTSTR g_lpszDominoCalPat[EDeviceDominoCalPat_Count] =
{
	_T("NONE"),
	_T("TPG"),
};

static LPCTSTR g_lpszDominoSurfaceAllocation[EDeviceDominoSurfaceAllocation_Count] =
{
	_T("ANYWHERE"),
	_T("BELOW4G"),
};

static LPCTSTR g_lpszDominoAcquisitionMode[EDeviceDominoAcquisitionMode_Count] =
{
	_T("HFR"),
	_T("INVALID"),
	_T("LONGPAGE"),
	_T("PAGE"),
	_T("SNAPSHOT"),
	_T("VIDEO"),
	_T("WEB"),
};

static LPCTSTR g_lpszDominoTrigMode[EDeviceDominoTrigMode_Count] =
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

static LPCTSTR g_lpszDominoCamConfig[EDeviceDominoCamConfig_Count] =
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

static LPCTSTR g_lpszDominoCamera[EDeviceDominoCamera_Count] =
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

static LPCTSTR g_lpszDominoDataLink[EDeviceDominoDataLink_Count] =
{
	_T("ANALOG"),
	_T("CAMERALINK"),
	_T("COMPOSITE"),
};

static LPCTSTR g_lpszDominoImaging[EDeviceDominoImaging_Count] =
{
	_T("AREA"),
	_T("LINE"),
	_T("TDI"),
};

static LPCTSTR g_lpszDominoSpectrum[EDeviceDominoSpectrum_Count] =
{
	_T("BW"),
	_T("COLOR"),
	_T("IR"),
};

static LPCTSTR g_lpszDominoAuxResetCtl[EDeviceDominoAuxResetCtl_Count] =
{
	_T("DIFF"),
	_T("NONE"),
	_T("TTL"),
};

static LPCTSTR g_lpszDominoColorMethod[EDeviceDominoColorMethod_Count] =
{
	_T("BAYER"),
	_T("NONE"),
	_T("PRISM"),
	_T("RGB"),
	_T("TRILINEAR"),
};

static LPCTSTR g_lpszDominoExposeOverlap[EDeviceDominoExposeOverlap_Count] =
{
	_T("ALLOW"),
	_T("FORBID"),
};

static LPCTSTR g_lpszDominoReadout[EDeviceDominoReadout_Count] =
{
	_T("100ns"),
	_T("INTCTL"),
	_T("PLSTRG"),
	_T("us"),
	_T("WIDTH"),
};

static LPCTSTR g_lpszDominoResetCtl[EDeviceDominoResetCtl_Count] =
{
	_T("DIFF"),
	_T("NONE"),
	_T("TTL"),
};

static LPCTSTR g_lpszDominoResetEdge[EDeviceDominoResetEdge_Count] =
{
	_T("GOHIGH"),
	_T("GOLOW"),
};

static LPCTSTR g_lpszDominoAuxresetLine[EDeviceDominoAuxresetLine_Count] =
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

static LPCTSTR g_lpszDominoResetLine[EDeviceDominoResetLine_Count] =
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

static LPCTSTR g_lpszDominoBreakEffect[EDeviceDominoBreakEffect_Count] =
{
	_T("ABORT"),
	_T("FINISH"),
};

static LPCTSTR g_lpszDominoEndTrigMode[EDeviceDominoEndTrigMode_Count] =
{
	_T("AUTO"),
	_T("HARD"),
	_T("MASTER_CHANNEL"),
	_T("SLAVE"),
};

static LPCTSTR g_lpszDominoNextTrigMode[EDeviceDominoNextTrigMode_Count] =
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

static LPCTSTR g_lpszDominoForceTrig[EDeviceDominoForceTrig_Count] =
{
	_T("ENDTRIG"),
	_T("TRIG"),
};

static LPCTSTR g_lpszDominoStrobeCtl[EDeviceDominoStrobeCtl_Count] =
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

static LPCTSTR g_lpszDominoStrobeMode[EDeviceDominoStrobeMode_Count] =
{
	_T("AUTO"),
	_T("MAN"),
	_T("NONE"),
	_T("OFF"),
};

static LPCTSTR g_lpszDominoGrabWindow[EDeviceDominoGrabWindow_Count] =
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

static LPCTSTR g_lpszDominoColorFormat[EDeviceDominoColorFormat_Count] =
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

static LPCTSTR g_lpszDominoImageFlipX[EDeviceDominoImageFlipX_Count] =
{
	_T("OFF"),
	_T("ON"),
};

static LPCTSTR g_lpszDominoImageFlipY[EDeviceDominoImageFlipY_Count] =
{
	_T("OFF"),
	_T("ON"),
};

static LPCTSTR g_lpszDominoChannelState[EDeviceDominoChannelState_Count] =
{
	_T("ACTIVE"),
	_T("FREE"),
	_T("IDLE"),
	_T("ORPHAN"),
	_T("READY"),
};

static LPCTSTR g_lpszDominoAcquisitionCleanup[EDeviceDominoAcquisitionCleanup_Count] =
{
	_T("DISABLED"),
	_T("ENABLED"),
};

static LPCTSTR g_lpszDominoParamsConsistencyCheck[EDeviceDominoParamsConsistencyCheck_Count] =
{
	_T("AUTO"),
	_T("MANUAL"),
};

static LPCTSTR g_lpszDominoExpose[EDeviceDominoExpose_Count] =
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




CDeviceEuresysDomino::CDeviceEuresysDomino()
{
}


CDeviceEuresysDomino::~CDeviceEuresysDomino()
{
}

EDeviceInitializeResult CDeviceEuresysDomino::Initialize()
{
	return __super::Initialize();
}

EDeviceTerminateResult CDeviceEuresysDomino::Terminate()
{
	return __super::Terminate();
}

EEuresysGetFunction CDeviceEuresysDomino::GetTapStructure(EDeviceDominoTapStructure * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_TapStructure));

		if(nData < 0 || nData >= (int)EDeviceDominoTapStructure_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoTapStructure)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetTapStructure(EDeviceDominoTapStructure eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_TapStructure;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoTapStructure_Count)
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
			if(McSetParamStr(m_hDevice, MC_TapStructure, CStringA(g_lpszDominoTapStructure[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoTapStructure[nPreValue], g_lpszDominoTapStructure[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetGain(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_Gain));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetGain(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_Gain;

	int nPreValue = _ttoi(GetParamValue(eSaveID));
	
	do
	{
		if(IsInitialized())
		{
			if(McSetParamInt(m_hDevice, MC_Gain, nParam) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetOffset(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_Offset));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetOffset(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_Offset;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			if(McSetParamInt(m_hDevice, MC_Offset, nParam) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetCamConfig(EDeviceDominoCamConfig * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_CamConfig));

		if(nData < 0 || nData >= (int)EDeviceDominoCamConfig_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoCamConfig)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetCamConfig(EDeviceDominoCamConfig eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_CamConfig;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoCamConfig_Count)
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
			if(McSetParamStr(m_hDevice, MC_CamConfig, CStringA(g_lpszDominoCamConfig[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoCamConfig[nPreValue], g_lpszDominoCamConfig[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetCamera(EDeviceDominoCamera * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_Camera));

		if(nData < 0 || nData >= (int)EDeviceDominoCamera_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoCamera)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetCamera(EDeviceDominoCamera eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_Camera;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoCamera_Count)
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
			if(McSetParamStr(m_hDevice, MC_Camera, CStringA(g_lpszDominoCamera[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoCamera[nPreValue], g_lpszDominoCamera[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetDataLink(EDeviceDominoDataLink * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_DataLink));

		if(nData < 0 || nData >= (int)EDeviceDominoDataLink_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoDataLink)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetDataLink(EDeviceDominoDataLink eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_DataLink;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoDataLink_Count)
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
			if(McSetParamStr(m_hDevice, MC_DataLink, CStringA(g_lpszDominoDataLink[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoDataLink[nPreValue], g_lpszDominoDataLink[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetImaging(EDeviceDominoImaging * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_Imaging));

		if(nData < 0 || nData >= (int)EDeviceDominoImaging_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoImaging)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetImaging(EDeviceDominoImaging eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_Imaging;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoImaging_Count)
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
			if(McSetParamStr(m_hDevice, MC_Imaging, CStringA(g_lpszDominoImaging[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoImaging[nPreValue], g_lpszDominoImaging[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetSpectrum(EDeviceDominoSpectrum * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_Spectrum));

		if(nData < 0 || nData >= (int)EDeviceDominoSpectrum_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoSpectrum)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetSpectrum(EDeviceDominoSpectrum eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_Spectrum;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoSpectrum_Count)
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
			if(McSetParamStr(m_hDevice, MC_Spectrum, CStringA(g_lpszDominoSpectrum[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoSpectrum[nPreValue], g_lpszDominoSpectrum[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetVideoFilter(EDeviceDominoVideoFilter * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_VideoFilter));

		if(nData < 0 || nData >= (int)EDeviceDominoVideoFilter_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoVideoFilter)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetVideoFilter(EDeviceDominoVideoFilter eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_VideoFilter;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoVideoFilter_Count)
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
			if(McSetParamStr(m_hDevice, MC_VideoFilter, CStringA(g_lpszDominoVideoFilter[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoVideoFilter[nPreValue], g_lpszDominoVideoFilter[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetVactive_Ln(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_Vactive_Ln));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetVactive_Ln(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_Vactive_Ln;

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetVsyncAft_Ln(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_VsyncAft_Ln));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetVsyncAft_Ln(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_VsyncAft_Ln;

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetLineRate_Hz(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_LineRate_Hz));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetLineRate_Hz(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_LineRate_Hz;

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetACResetPostDelay_us(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_ACResetPostDelay_us));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetACResetPostDelay_us(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_ACResetPostDelay_us;

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
			if(McSetParamInt(m_hDevice, MC_ACResetPostDelay_us, nParam) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetACResetWidth_us(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_ACResetWidth_us));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetACResetWidth_us(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_ACResetWidth_us;

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
			if(McSetParamInt(m_hDevice, MC_ACResetWidth_us, nParam) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetHCSyncAft_ns(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_HCSyncAft_ns));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetHCSyncAft_ns(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_HCSyncAft_ns;

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
			eReturn = EEuresysSetFunction_WriteToDeviceError;
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetHCsyncBfr_ns(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_HCsyncBfr_ns));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetHCsyncBfr_ns(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_HCsyncBfr_ns;

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
			eReturn = EEuresysSetFunction_WriteToDeviceError;
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetHCsyncDur_ns(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_HCsyncDur_ns));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetHCsyncDur_ns(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_HCsyncDur_ns;

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
			if(McSetParamInt(m_hDevice, MC_HCsyncDur_ns, nParam) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetHsyncDly_ns(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_HsyncDly_ns));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetHsyncDly_ns(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_HsyncDly_ns;

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
			if(McSetParamInt(m_hDevice, MC_HsyncDly_ns, nParam) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetHdriveDur_ns(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_HdriveDur_ns));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetHdriveDur_ns(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_HdriveDur_ns;

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
			if(McSetParamInt(m_hDevice, MC_HdriveDur_ns, nParam) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetHdriveDly_ns(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_HdriveDly_ns));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetHdriveDly_ns(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_HdriveDly_ns;

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
			if(McSetParamInt(m_hDevice, MC_HdriveDly_ns, nParam) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetHactive_ns(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_Hactive_ns));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetHactive_ns(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_Hactive_ns;

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
			if(McSetParamInt(m_hDevice, MC_Hactive_ns, nParam) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetPixelClk_Hz(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_PixelClk_Hz));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetPixelClk_Hz(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_PixelClk_Hz;

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetScanning(EDeviceDominoScanning * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_Scanning));

		if(nData < 0 || nData >= (int)EDeviceDominoScanning_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoScanning)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetScanning(EDeviceDominoScanning eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_Scanning;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoScanning_Count)
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
			if(McSetParamStr(m_hDevice, MC_Scanning, CStringA(g_lpszDominoScanning[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoScanning[nPreValue], g_lpszDominoScanning[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetStandard(EDeviceDominoStandard * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_Standard));

		if(nData < 0 || nData >= (int)EDeviceDominoStandard_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoStandard)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetStandard(EDeviceDominoStandard eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_Standard;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoStandard_Count)
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
			if(McSetParamStr(m_hDevice, MC_Standard, CStringA(g_lpszDominoStandard[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoStandard[nPreValue], g_lpszDominoStandard[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetVCsyncAft_Ln(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_VCsyncAft_Ln));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetVCsyncAft_Ln(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_VCsyncAft_Ln;

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
			if(McSetParamInt(m_hDevice, MC_VCsyncAft_Ln, nParam) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetVCgatePos_Ln(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_VCgatePos_Ln));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetVCgatePos_Ln(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_VCgatePos_Ln;

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
			if(McSetParamInt(m_hDevice, MC_VCgatePos_Ln, nParam) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetVtotal_Ln(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_Vtotal_Ln));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetVtotal_Ln(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_Vtotal_Ln;

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
			if(McSetParamInt(m_hDevice, MC_Vtotal_Ln, nParam) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetVsyncBfrEndExp_us(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_VsyncBfrEndExp_us));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetVsyncBfrEndExp_us(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_VsyncBfrEndExp_us;

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
			if(McSetParamInt(m_hDevice, MC_VsyncBfrEndExp_us, nParam) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetVdriveDur_Ln(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_VdriveDur_Ln));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetVdriveDur_Ln(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_VdriveDur_Ln;

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
			if(McSetParamInt(m_hDevice, MC_VdriveDur_Ln, nParam) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetVdriveDly_Ln(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_VdriveDly_Ln));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetVdriveDly_Ln(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_VdriveDly_Ln;

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
			if(McSetParamInt(m_hDevice, MC_VdriveDly_Ln, nParam) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetAuxResetCtl(EDeviceDominoAuxResetCtl * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_AuxResetCtl));

		if(nData < 0 || nData >= (int)EDeviceDominoAuxResetCtl_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoAuxResetCtl)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetAuxResetCtl(EDeviceDominoAuxResetCtl eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_AuxResetCtl;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoAuxResetCtl_Count)
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
			if(McSetParamStr(m_hDevice, MC_AuxResetCtl, CStringA(g_lpszDominoAuxResetCtl[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoAuxResetCtl[nPreValue], g_lpszDominoAuxResetCtl[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetColorMethod(EDeviceDominoColorMethod * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_ColorMethod));

		if(nData < 0 || nData >= (int)EDeviceDominoColorMethod_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoColorMethod)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetColorMethod(EDeviceDominoColorMethod eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_ColorMethod;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoColorMethod_Count)
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
			if(McSetParamStr(m_hDevice, MC_ColorMethod, CStringA(g_lpszDominoColorMethod[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoColorMethod[nPreValue], g_lpszDominoColorMethod[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetExposeOverlap(EDeviceDominoExposeOverlap * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_ExposeOverlap));

		if(nData < 0 || nData >= (int)EDeviceDominoExposeOverlap_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoExposeOverlap)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetExposeOverlap(EDeviceDominoExposeOverlap eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_ExposeOverlap;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoExposeOverlap_Count)
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
			if(McSetParamStr(m_hDevice, MC_ExposeOverlap, CStringA(g_lpszDominoExposeOverlap[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoExposeOverlap[nPreValue], g_lpszDominoExposeOverlap[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetReadout(EDeviceDominoReadout * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_Readout));

		if(nData < 0 || nData >= (int)EDeviceDominoReadout_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoReadout)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetReadout(EDeviceDominoReadout eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_Readout;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoReadout_Count)
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
			if(McSetParamStr(m_hDevice, MC_Readout, CStringA(g_lpszDominoReadout[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoReadout[nPreValue], g_lpszDominoReadout[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetResetCtl(EDeviceDominoResetCtl * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_ResetCtl));

		if(nData < 0 || nData >= (int)EDeviceDominoResetCtl_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoResetCtl)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetResetCtl(EDeviceDominoResetCtl eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_ResetCtl;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoResetCtl_Count)
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
			if(McSetParamStr(m_hDevice, MC_ResetCtl, CStringA(g_lpszDominoResetCtl[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoResetCtl[nPreValue], g_lpszDominoResetCtl[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetResetEdge(EDeviceDominoResetEdge * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_ResetEdge));

		if(nData < 0 || nData >= (int)EDeviceDominoResetEdge_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoResetEdge)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetResetEdge(EDeviceDominoResetEdge eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_ResetEdge;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoResetEdge_Count)
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
			if(McSetParamStr(m_hDevice, MC_ResetEdge, CStringA(g_lpszDominoResetEdge[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoResetEdge[nPreValue], g_lpszDominoResetEdge[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetExposeMin_us(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_ExposeMin_us));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetExposeMin_us(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_ExposeMin_us;

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetExposeMax_us(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_ExposeMax_us));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetExposeMax_us(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_ExposeMax_us;

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetACReset(EDeviceDominoACReset * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_ACReset));

		if(nData < 0 || nData >= (int)EDeviceDominoACReset_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoACReset)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetACReset(EDeviceDominoACReset eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_ACReset;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoACReset_Count)
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
			if(McSetParamStr(m_hDevice, MC_ACReset, CStringA(g_lpszDominoACReset[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoACReset[nPreValue], g_lpszDominoACReset[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetChannelTopology(EDeviceDominoChannelTopology * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_ChannelTopology));

		if(nData < 0 || nData >= (int)EDeviceDominoChannelTopology_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoChannelTopology)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetChannelTopology(EDeviceDominoChannelTopology eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_ChannelTopology;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoChannelTopology_Count)
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
			if(McSetParamStr(m_hDevice, MC_ChannelTopology, CStringA(g_lpszDominoChannelTopology[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoChannelTopology[nPreValue], g_lpszDominoChannelTopology[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetCameraSignalSet(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_CameraSignalSet));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetCameraSignalSet(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_CameraSignalSet;

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
			if(McSetParamInt(m_hDevice, MC_CameraSignalSet, nParam) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetHReset(EDeviceDominoHReset * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_HReset));

		if(nData < 0 || nData >= (int)EDeviceDominoHReset_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoHReset)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetHReset(EDeviceDominoHReset eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_HReset;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoHReset_Count)
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
			if(McSetParamStr(m_hDevice, MC_HReset, CStringA(g_lpszDominoHReset[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoHReset[nPreValue], g_lpszDominoHReset[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetHsyncCtl(EDeviceDominoHsyncCtl * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_HsyncCtl));

		if(nData < 0 || nData >= (int)EDeviceDominoHsyncCtl_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoHsyncCtl)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetHsyncCtl(EDeviceDominoHsyncCtl eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_HsyncCtl;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoHsyncCtl_Count)
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
			if(McSetParamStr(m_hDevice, MC_HsyncCtl, CStringA(g_lpszDominoHsyncCtl[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoHsyncCtl[nPreValue], g_lpszDominoHsyncCtl[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetHsyncEdge(EDeviceDominoHsyncEdge * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_HsyncEdge));

		if(nData < 0 || nData >= (int)EDeviceDominoHsyncEdge_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoHsyncEdge)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetHsyncEdge(EDeviceDominoHsyncEdge eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_HsyncEdge;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoHsyncEdge_Count)
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
			if(McSetParamStr(m_hDevice, MC_HsyncEdge, CStringA(g_lpszDominoHsyncEdge[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoHsyncEdge[nPreValue], g_lpszDominoHsyncEdge[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetHVdriveCtl(EDeviceDominoHVdriveCtl * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_HVdriveCtl));

		if(nData < 0 || nData >= (int)EDeviceDominoHVdriveCtl_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoHVdriveCtl)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetHVdriveCtl(EDeviceDominoHVdriveCtl eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_HVdriveCtl;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoHVdriveCtl_Count)
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
			if(McSetParamStr(m_hDevice, MC_HVdriveCtl, CStringA(g_lpszDominoHVdriveCtl[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoHVdriveCtl[nPreValue], g_lpszDominoHVdriveCtl[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetHVdriveEdge(EDeviceDominoHVdriveEdge * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_HVdriveEdge));

		if(nData < 0 || nData >= (int)EDeviceDominoHVdriveEdge_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoHVdriveEdge)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetHVdriveEdge(EDeviceDominoHVdriveEdge eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_HVdriveEdge;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoHVdriveEdge_Count)
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
			if(McSetParamStr(m_hDevice, MC_HVdriveEdge, CStringA(g_lpszDominoHVdriveEdge[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoHVdriveEdge[nPreValue], g_lpszDominoHVdriveEdge[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetPixelClkCtl(EDeviceDominoPixelClkCtl * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_PixelClkCtl));

		if(nData < 0 || nData >= (int)EDeviceDominoPixelClkCtl_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoPixelClkCtl)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetPixelClkCtl(EDeviceDominoPixelClkCtl eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_PixelClkCtl;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoPixelClkCtl_Count)
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
			if(McSetParamStr(m_hDevice, MC_PixelClkCtl, CStringA(g_lpszDominoPixelClkCtl[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoPixelClkCtl[nPreValue], g_lpszDominoPixelClkCtl[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetResetDur(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_ResetDur));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetResetDur(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_ResetDur;

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
			if(McSetParamInt(m_hDevice, MC_ResetDur, nParam) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetResetPos(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_ResetPos));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetResetPos(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_ResetPos;

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
			if(McSetParamInt(m_hDevice, MC_ResetPos, nParam) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetVCSync(EDeviceDominoVCSync * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_VCSync));

		if(nData < 0 || nData >= (int)EDeviceDominoVCSync_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoVCSync)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetVCSync(EDeviceDominoVCSync eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_VCSync;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoVCSync_Count)
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
			if(McSetParamStr(m_hDevice, MC_VCSync, CStringA(g_lpszDominoVCSync[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoVCSync[nPreValue], g_lpszDominoVCSync[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetVHSync(EDeviceDominoVHSync * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_VHSync));

		if(nData < 0 || nData >= (int)EDeviceDominoVHSync_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoVHSync)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetVHSync(EDeviceDominoVHSync eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_VHSync;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(eParam < 0 || eParam >= EDeviceDominoVHSync_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_VHSync, CStringA(g_lpszDominoVHSync[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoVHSync[nPreValue], g_lpszDominoVHSync[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetVsyncCtl(EDeviceDominoVsyncCtl * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_VsyncCtl));

		if(nData < 0 || nData >= (int)EDeviceDominoVsyncCtl_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoVsyncCtl)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetVsyncCtl(EDeviceDominoVsyncCtl eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_VsyncCtl;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoVsyncCtl_Count)
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
			if(McSetParamStr(m_hDevice, MC_VsyncCtl, CStringA(g_lpszDominoVsyncCtl[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoVsyncCtl[nPreValue], g_lpszDominoVsyncCtl[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetVsyncEdge(EDeviceDominoVsyncEdge * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_VsyncEdge));

		if(nData < 0 || nData >= (int)EDeviceDominoVsyncEdge_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoVsyncEdge)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetVsyncEdge(EDeviceDominoVsyncEdge eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_VsyncEdge;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(eParam < 0 || eParam >= EDeviceDominoVsyncEdge_Count)
		{
			eReturn = EEuresysSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			if(McSetParamStr(m_hDevice, MC_VsyncEdge, CStringA(g_lpszDominoVsyncEdge[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoVsyncEdge[nPreValue], g_lpszDominoVsyncEdge[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetBreakEffect(EDeviceDominoBreakEffect * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_BreakEffect));

		if(nData < 0 || nData >= (int)EDeviceDominoBreakEffect_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoBreakEffect)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetBreakEffect(EDeviceDominoBreakEffect eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_BreakEffect;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoBreakEffect_Count)
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
			if(McSetParamStr(m_hDevice, MC_BreakEffect, CStringA(g_lpszDominoBreakEffect[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoBreakEffect[nPreValue], g_lpszDominoBreakEffect[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetEndTrigMode(EDeviceDominoEndTrigMode * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_EndTrigMode));

		if(nData < 0 || nData >= (int)EDeviceDominoEndTrigMode_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoEndTrigMode)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetEndTrigMode(EDeviceDominoEndTrigMode eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_EndTrigMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoEndTrigMode_Count)
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
			if(McSetParamStr(m_hDevice, MC_EndTrigMode, CStringA(g_lpszDominoEndTrigMode[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoEndTrigMode[nPreValue], g_lpszDominoEndTrigMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetNextTrigMode(EDeviceDominoNextTrigMode * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_NextTrigMode));

		if(nData < 0 || nData >= (int)EDeviceDominoNextTrigMode_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoNextTrigMode)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetNextTrigMode(EDeviceDominoNextTrigMode eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_NextTrigMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoNextTrigMode_Count)
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
			if(McSetParamStr(m_hDevice, MC_NextTrigMode, CStringA(g_lpszDominoNextTrigMode[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoNextTrigMode[nPreValue], g_lpszDominoNextTrigMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetAuxResetLine(EDeviceDominoAuxResetLine * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_AuxResetLine));

		if(nData < 0 || nData >= (int)EDeviceDominoAuxResetLine_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoAuxResetLine)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetAuxResetLine(EDeviceDominoAuxResetLine eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_AuxResetLine;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoAuxResetLine_Count)
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
			if(McSetParamStr(m_hDevice, MC_AuxresetLine, CStringA(g_lpszDominoAuxResetLine[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoAuxResetLine[nPreValue], g_lpszDominoAuxResetLine[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetCable(EDeviceDominoCable * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_Cable));

		if(nData < 0 || nData >= (int)EDeviceDominoCable_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoCable)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetCable(EDeviceDominoCable eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_Cable;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoCable_Count)
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
			if(McSetParamStr(m_hDevice, MC_Cable, CStringA(g_lpszDominoCable[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoCable[nPreValue], g_lpszDominoCable[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetCsyncLine(EDeviceDominoCsyncLine * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_CsyncLine));

		if(nData < 0 || nData >= (int)EDeviceDominoCsyncLine_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoCsyncLine)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetCsyncLine(EDeviceDominoCsyncLine eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_CsyncLine;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoCsyncLine_Count)
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
			if(McSetParamStr(m_hDevice, MC_CsyncLine, CStringA(g_lpszDominoCsyncLine[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoCsyncLine[nPreValue], g_lpszDominoCsyncLine[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetHdriveLine(EDeviceDominoHdriveLine * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_HdriveLine));

		if(nData < 0 || nData >= (int)EDeviceDominoHdriveLine_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoHdriveLine)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetHdriveLine(EDeviceDominoHdriveLine eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_HdriveLine;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoHdriveLine_Count)
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
			if(McSetParamStr(m_hDevice, MC_HdriveLine, CStringA(g_lpszDominoHdriveLine[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoHdriveLine[nPreValue], g_lpszDominoHdriveLine[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetHsyncLine(EDeviceDominoHsyncLine * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_HsyncLine));

		if(nData < 0 || nData >= (int)EDeviceDominoHsyncLine_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoHsyncLine)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetHsyncLine(EDeviceDominoHsyncLine eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_HsyncLine;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoHsyncLine_Count)
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
			if(McSetParamStr(m_hDevice, MC_HsyncLine, CStringA(g_lpszDominoHsyncLine[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoHsyncLine[nPreValue], g_lpszDominoHsyncLine[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetPixelClkLine(EDeviceDominoPixelClkLine * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_PixelClkLine));

		if(nData < 0 || nData >= (int)EDeviceDominoPixelClkLine_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoPixelClkLine)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetPixelClkLine(EDeviceDominoPixelClkLine eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_PixelClkLine;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoPixelClkLine_Count)
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
			if(McSetParamStr(m_hDevice, MC_PixelClkLine, CStringA(g_lpszDominoPixelClkLine[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoPixelClkLine[nPreValue], g_lpszDominoPixelClkLine[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetVdriveLine(EDeviceDominoVdriveLine * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_VdriveLine));

		if(nData < 0 || nData >= (int)EDeviceDominoVdriveLine_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoVdriveLine)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetVdriveLine(EDeviceDominoVdriveLine eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_VdriveLine;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoVdriveLine_Count)
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
			if(McSetParamStr(m_hDevice, MC_VdriveLine, CStringA(g_lpszDominoVdriveLine[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoVdriveLine[nPreValue], g_lpszDominoVdriveLine[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetVsyncLine(EDeviceDominoVsyncLine * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_VsyncLine));

		if(nData < 0 || nData >= (int)EDeviceDominoVsyncLine_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoVsyncLine)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetVsyncLine(EDeviceDominoVsyncLine eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_VsyncLine;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoVsyncLine_Count)
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
			if(McSetParamStr(m_hDevice, MC_VsyncLine, CStringA(g_lpszDominoVsyncLine[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoVsyncLine[nPreValue], g_lpszDominoVsyncLine[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetCableTopology(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_CableTopology));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetCableTopology(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_CableTopology;

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
			if(McSetParamInt(m_hDevice, MC_CableTopology, nParam) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetResetLine(EDeviceDominoResetLine * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_ResetLine));

		if(nData < 0 || nData >= (int)EDeviceDominoResetLine_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoResetLine)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetResetLine(EDeviceDominoResetLine eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_ResetLine;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoResetLine_Count)
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
			if(McSetParamStr(m_hDevice, MC_ResetLine, CStringA(g_lpszDominoResetLine[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoResetLine[nPreValue], g_lpszDominoResetLine[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetActivityLength(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_ActivityLength));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetActivityLength(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_ActivityLength;

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetGrabField(EDeviceDominoGrabField * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_GrabField));

		if(nData < 0 || nData >= (int)EDeviceDominoGrabField_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoGrabField)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetGrabField(EDeviceDominoGrabField eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_GrabField;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoGrabField_Count)
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
			if(McSetParamStr(m_hDevice, MC_GrabField, CStringA(g_lpszDominoGrabField[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoGrabField[nPreValue], g_lpszDominoGrabField[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetNextGrabField(EDeviceDominoNextGrabField * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_NextGrabField));

		if(nData < 0 || nData >= (int)EDeviceDominoNextGrabField_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoNextGrabField)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetNextGrabField(EDeviceDominoNextGrabField eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_NextGrabField;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoNextGrabField_Count)
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
			if(McSetParamStr(m_hDevice, MC_NextGrabField, CStringA(g_lpszDominoNextGrabField[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoNextGrabField[nPreValue], g_lpszDominoNextGrabField[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetForceTrig(EDeviceDominoForceTrig * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_ForceTrig));

		if(nData < 0 || nData >= (int)EDeviceDominoForceTrig_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoForceTrig)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetForceTrig(EDeviceDominoForceTrig eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_ForceTrig;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoForceTrig_Count)
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
			if(McSetParamStr(m_hDevice, MC_ForceTrig, CStringA(g_lpszDominoForceTrig[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoForceTrig[nPreValue], g_lpszDominoForceTrig[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetTrigDelay_us(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_TrigDelay_us));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetTrigDelay_us(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_TrigDelay_us;

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetTrueExp_us(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_TrueExp_us));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetTrueExp_us(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_TrueExp_us;

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
			if(McSetParamInt(m_hDevice, MC_TrueExp_us, nParam) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetExposeTrim(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_ExposeTrim));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetExposeTrim(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_ExposeTrim;

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetStrobeCtl(EDeviceDominoStrobeCtl * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_StrobeCtl));

		if(nData < 0 || nData >= (int)EDeviceDominoStrobeCtl_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoStrobeCtl)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetStrobeCtl(EDeviceDominoStrobeCtl eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_StrobeCtl;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoStrobeCtl_Count)
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
			if(McSetParamStr(m_hDevice, MC_StrobeCtl, CStringA(g_lpszDominoStrobeCtl[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoStrobeCtl[nPreValue], g_lpszDominoStrobeCtl[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetStrobeDur(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_StrobeDur));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetStrobeDur(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_StrobeDur;

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetStrobeMode(EDeviceDominoStrobeMode * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_StrobeMode));

		if(nData < 0 || nData >= (int)EDeviceDominoStrobeMode_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoStrobeMode)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetStrobeMode(EDeviceDominoStrobeMode eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_StrobeMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoStrobeMode_Count)
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
			if(McSetParamStr(m_hDevice, MC_StrobeMode, CStringA(g_lpszDominoStrobeMode[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoStrobeMode[nPreValue], g_lpszDominoStrobeMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetStrobePos(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_StrobePos));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetStrobePos(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_StrobePos;

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetPreStrobe_us(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_PreStrobe_us));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetPreStrobe_us(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_PreStrobe_us;

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetStrobeLine(EDeviceDominoStrobeLine * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_StrobeLine));

		if(nData < 0 || nData >= (int)EDeviceDominoStrobeLine_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoStrobeLine)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetStrobeLine(EDeviceDominoStrobeLine eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_StrobeLine;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoStrobeLine_Count)
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
			if(McSetParamStr(m_hDevice, MC_StrobeLine, CStringA(g_lpszDominoStrobeLine[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoStrobeLine[nPreValue], g_lpszDominoStrobeLine[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetStrobeLevel(EDeviceDominoStrobeLevel * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_StrobeLevel));

		if(nData < 0 || nData >= (int)EDeviceDominoStrobeLevel_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoStrobeLevel)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetStrobeLevel(EDeviceDominoStrobeLevel eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_StrobeLevel;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoStrobeLevel_Count)
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
			if(McSetParamStr(m_hDevice, MC_StrobeLevel, CStringA(g_lpszDominoStrobeLevel[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoStrobeLevel[nPreValue], g_lpszDominoStrobeLevel[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetADresolution(EDeviceDominoADresolution * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_ADresolution));

		if(nData < 0 || nData >= (int)EDeviceDominoADresolution_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoADresolution)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetADresolution(EDeviceDominoADresolution eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_ADresolution;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoADresolution_Count)
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
			if(McSetParamStr(m_hDevice, MC_ADresolution, CStringA(g_lpszDominoADresolution[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoADresolution[nPreValue], g_lpszDominoADresolution[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetVideoTerminator(EDeviceDominoVideoTerminator * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_VideoTerminator));

		if(nData < 0 || nData >= (int)EDeviceDominoVideoTerminator_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoVideoTerminator)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetVideoTerminator(EDeviceDominoVideoTerminator eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_VideoTerminator;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoVideoTerminator_Count)
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
			if(McSetParamStr(m_hDevice, MC_VideoTerminator, CStringA(g_lpszDominoVideoTerminator[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoVideoTerminator[nPreValue], g_lpszDominoVideoTerminator[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetGrabWindow(EDeviceDominoGrabWindow * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_GrabWindow));

		if(nData < 0 || nData >= (int)EDeviceDominoGrabWindow_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoGrabWindow)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetGrabWindow(EDeviceDominoGrabWindow eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_GrabWindow;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoGrabWindow_Count)
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
			if(McSetParamStr(m_hDevice, MC_GrabWindow, CStringA(g_lpszDominoGrabWindow[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoGrabWindow[nPreValue], g_lpszDominoGrabWindow[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetOffsetX_Px(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_OffsetX_Px));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetOffsetX_Px(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_OffsetX_Px;

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetOffsetY_Ln(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_OffsetY_Ln));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetOffsetY_Ln(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_OffsetY_Ln;

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
			if(McSetParamInt(m_hDevice, MC_OffsetY_Ln, nParam) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetSampleTrim(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_SampleTrim));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetSampleTrim(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_SampleTrim;

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
			if(McSetParamInt(m_hDevice, MC_SampleTrim, nParam) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetSampleClk_Hz(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_SampleClk_Hz));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetSampleClk_Hz(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_SampleClk_Hz;

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
			if(McSetParamInt(m_hDevice, MC_SampleClk_Hz, nParam) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetGainCtl(EDeviceDominoGainCtl * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_GainCtl));

		if(nData < 0 || nData >= (int)EDeviceDominoGainCtl_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoGainCtl)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetGainCtl(EDeviceDominoGainCtl eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;
	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_GainCtl;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoGainCtl_Count)
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
			if(McSetParamStr(m_hDevice, MC_GainCtl, CStringA(g_lpszDominoGainCtl[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoGainCtl[nPreValue], g_lpszDominoGainCtl[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetCalPat(EDeviceDominoCalPat * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_CalPat));

		if(nData < 0 || nData >= (int)EDeviceDominoCalPat_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoCalPat)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetCalPat(EDeviceDominoCalPat eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_CalPat;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoCalPat_Count)
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
			if(McSetParamStr(m_hDevice, MC_CalPat, CStringA(g_lpszDominoCalPat[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoCalPat[nPreValue], g_lpszDominoCalPat[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetInputLut(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_InputLut));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetInputLut(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_InputLut;

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
			if(McSetParamInt(m_hDevice, MC_InputLut, nParam) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetLutIndex(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_LutIndex));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetLutIndex(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_LutIndex;

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
			if(McSetParamInt(m_hDevice, MC_LutIndex, nParam) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetBoardIdentifier(CString * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = GetParamValue(EDeviceParameterEuresysDomino_BoardIdentifier);

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetBoardIdentifier(CString strParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_BoardIdentifier;

	CString strPreValue = GetParamValue(eSaveID);

	do
	{
		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EEuresysSetFunction_AlreadyInitializedError;
			break;
		}

		if(!SetParamValue(eSaveID, strParam))
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], strPreValue, strParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetBoardName(CString * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = GetParamValue(EDeviceParameterEuresysDomino_BoardName);

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetBoardName(CString strParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;
	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_BoardName;

	CString strPreValue = GetParamValue(eSaveID);

	do
	{
		if(!IsGrabAvailable())
		{
			eReturn = EEuresysSetFunction_ActivatedDeviceError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EEuresysSetFunction_AlreadyInitializedError;
			break;
		}

		if(!SetParamValue(eSaveID, strParam))
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], strPreValue, strParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetDriverIndex(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_DriverIndex));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetDriverIndex(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_DriverIndex;

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
			if(McSetParamInt(m_hDevice, MC_DriverIndex, nParam) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetPciPosition(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_PciPosition));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetPciPosition(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_PciPosition;

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
			if(McSetParamInt(m_hDevice, MC_PciPosition, nParam) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetColorFormat(EDeviceDominoColorFormat * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_ColorFormat));

		if(nData < 0 || nData >= (int)EDeviceDominoColorFormat_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoColorFormat)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetColorFormat(EDeviceDominoColorFormat eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_ColorFormat;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoColorFormat_Count)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoColorFormat[nPreValue], g_lpszDominoColorFormat[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetImageFlipX(EDeviceDominoImageFlipX * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_ImageFlipX));

		if(nData < 0 || nData >= (int)EDeviceDominoImageFlipX_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoImageFlipX)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetImageFlipX(EDeviceDominoImageFlipX eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_ImageFlipX;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoImageFlipX_Count)
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
			if(McSetParamStr(m_hDevice, MC_ImageFlipX, CStringA(g_lpszDominoImageFlipX[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoImageFlipX[nPreValue], g_lpszDominoImageFlipX[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetImageFlipY(EDeviceDominoImageFlipY * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_ImageFlipY));

		if(nData < 0 || nData >= (int)EDeviceDominoImageFlipY_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoImageFlipY)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetImageFlipY(EDeviceDominoImageFlipY eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_ImageFlipY;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoImageFlipY_Count)
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
			if(McSetParamStr(m_hDevice, MC_ImageFlipY, CStringA(g_lpszDominoImageFlipY[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoImageFlipY[nPreValue], g_lpszDominoImageFlipY[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetImageSizeX(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_ImageSizeX));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetImageSizeX(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_ImageSizeX;

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetSurfaceAllocation(EDeviceDominoSurfaceAllocation * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_SurfaceAllocation));

		if(nData < 0 || nData >= (int)EDeviceDominoSurfaceAllocation_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoSurfaceAllocation)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetSurfaceAllocation(EDeviceDominoSurfaceAllocation eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_SurfaceAllocation;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoSurfaceAllocation_Count)
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
			if(McSetParamStr(m_hDevice, MC_SurfaceAllocation, CStringA(g_lpszDominoSurfaceAllocation[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoSurfaceAllocation[nPreValue], g_lpszDominoSurfaceAllocation[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetChannelState(EDeviceDominoChannelState * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_ChannelState));

		if(nData < 0 || nData >= (int)EDeviceDominoChannelState_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoChannelState)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetChannelState(EDeviceDominoChannelState eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_ChannelState;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoChannelState_Count)
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
			if(McSetParamStr(m_hDevice, MC_ChannelState, CStringA(g_lpszDominoChannelState[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoChannelState[nPreValue], g_lpszDominoChannelState[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetAcquisitionCleanup(EDeviceDominoAcquisitionCleanup * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_AcquisitionCleanup));

		if(nData < 0 || nData >= (int)EDeviceDominoAcquisitionCleanup_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoAcquisitionCleanup)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetAcquisitionCleanup(EDeviceDominoAcquisitionCleanup eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_AcquisitionCleanup;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoAcquisitionCleanup_Count)
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
			if(McSetParamStr(m_hDevice, MC_AcquisitionCleanup, CStringA(g_lpszDominoAcquisitionCleanup[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoAcquisitionCleanup[nPreValue], g_lpszDominoAcquisitionCleanup[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetAcqTimeout_ms(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_AcqTimeout_ms));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetAcqTimeout_ms(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_AcqTimeout_ms;

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetSurfaceCount(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_SurfaceCount));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetSurfaceCount(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_SurfaceCount;

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetCsyncCtl(EDeviceDominoCsyncCtl * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_CsyncCtl));

		if(nData < 0 || nData >= (int)EDeviceDominoCsyncCtl_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoCsyncCtl)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetCsyncCtl(EDeviceDominoCsyncCtl eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_CsyncCtl;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoCsyncCtl_Count)
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
			if(McSetParamStr(m_hDevice, MC_CsyncCtl, CStringA(g_lpszDominoCsyncCtl[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoCsyncCtl[nPreValue], g_lpszDominoCsyncCtl[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetCsyncEdge(EDeviceDominoCsyncEdge * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_CsyncEdge));

		if(nData < 0 || nData >= (int)EDeviceDominoCsyncEdge_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoCsyncEdge)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetCsyncEdge(EDeviceDominoCsyncEdge eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_CsyncEdge;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoCsyncEdge_Count)
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
			if(McSetParamStr(m_hDevice, MC_CsyncEdge, CStringA(g_lpszDominoCsyncEdge[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoCsyncEdge[nPreValue], g_lpszDominoCsyncEdge[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetCameraControlSignalSet(EDeviceDominoCameraControlSignalSet * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_CameraControlSignalSet));

		if(nData < 0 || nData >= (int)EDeviceDominoCameraControlSignalSet_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoCameraControlSignalSet)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetCameraControlSignalSet(EDeviceDominoCameraControlSignalSet eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_CameraControlSignalSet;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoCameraControlSignalSet_Count)
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
			if(McSetParamStr(m_hDevice, MC_CameraControlSignalSet, CStringA(g_lpszDominoCameraControlSignalSet[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoCameraControlSignalSet[nPreValue], g_lpszDominoCameraControlSignalSet[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetParamsConsistencyCheck(EDeviceDominoParamsConsistencyCheck * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_ParamsConsistencyCheck));

		if(nData < 0 || nData >= (int)EDeviceDominoParamsConsistencyCheck_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoParamsConsistencyCheck)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetParamsConsistencyCheck(EDeviceDominoParamsConsistencyCheck eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_ParamsConsistencyCheck;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoParamsConsistencyCheck_Count)
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
			if(McSetParamStr(m_hDevice, MC_ParamsConsistencyCheck, CStringA(g_lpszDominoParamsConsistencyCheck[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoParamsConsistencyCheck[nPreValue], g_lpszDominoParamsConsistencyCheck[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetExpose(EDeviceDominoExpose * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_Expose));

		if(nData < 0 || nData >= (int)EDeviceDominoExpose_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoExpose)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetExpose(EDeviceDominoExpose eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_Expose;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoExpose_Count)
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
			if(McSetParamStr(m_hDevice, MC_Expose, CStringA(g_lpszDominoExpose[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoExpose[nPreValue], g_lpszDominoExpose[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetAcquisitionMode(EDeviceDominoAcquisitionMode * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_AcquisitionMode));

		if(nData < 0 || nData >= (int)EDeviceDominoAcquisitionMode_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoAcquisitionMode)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetAcquisitionMode(EDeviceDominoAcquisitionMode eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_AcquisitionMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoAcquisitionMode_Count)
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
			if(McSetParamStr(m_hDevice, MC_AcquisitionMode, CStringA(g_lpszDominoAcquisitionMode[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoAcquisitionMode[nPreValue], g_lpszDominoAcquisitionMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetTrigMode(EDeviceDominoTrigMode * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_TrigMode));

		if(nData < 0 || nData >= (int)EDeviceDominoTrigMode_Count)
		{
			eReturn = EEuresysGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceDominoTrigMode)nData;

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetTrigMode(EDeviceDominoTrigMode eParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_TrigMode;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < 0 || eParam >= EDeviceDominoTrigMode_Count)
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
			if(McSetParamStr(m_hDevice, MC_TrigMode, CStringA(g_lpszDominoTrigMode[eParam])) != MC_OK)
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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], g_lpszDominoTrigMode[nPreValue], g_lpszDominoTrigMode[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetExpose_us(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_Expose_us));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetExpose_us(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_Expose_us;

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetSeqLength_Fr(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_SeqLength_Fr));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetSeqLength_Fr(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_SeqLength_Fr;

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EEuresysGetFunction CDeviceEuresysDomino::GetPhaseLength_Fr(int * pParam)
{
	EEuresysGetFunction eReturn = EEuresysGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EEuresysGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterEuresysDomino_PhaseLength_Fr));

		eReturn = EEuresysGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EEuresysSetFunction CDeviceEuresysDomino::SetPhaseLength_Fr(int nParam)
{
	EEuresysSetFunction eReturn = EEuresysSetFunction_UnknownError;

	EDeviceParameterEuresysDomino eSaveID = EDeviceParameterEuresysDomino_PhaseLength_Fr;

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

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceDomino[eSaveID - EDeviceParameterEuresysDominoSymphony_Count], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceEuresysDomino::LoadSettings()
{
	bool bReturn = false;

	do
	{
		AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_SummaryControl, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_SummaryControl - EDeviceParameterEuresysDominoSymphony_Count], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		{
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_AcquisitionMode, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_AcquisitionMode - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoAcquisitionMode, EDeviceDominoAcquisitionMode_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_TrigMode, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_TrigMode - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoTrigMode, EDeviceDominoTrigMode_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_Expose_us, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_Expose_us - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_AreaCamera, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_AreaCamera - EDeviceParameterEuresysDominoSymphony_Count], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);
			{
				AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_SeqLength_Fr, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_SeqLength_Fr - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);

				AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_PhaseLength_Fr, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_PhaseLength_Fr - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 2);
			}

			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_TapStructure, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_TapStructure - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoTapStructure, EDeviceDominoTapStructure_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_Gain, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_Gain - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_Offset, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_Offset - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_VideoFilter, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_VideoFilter - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoVideoFilter, EDeviceDominoVideoFilter_Count), nullptr, 1);
		}

		AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_CameraSpecification, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_CameraSpecification - EDeviceParameterEuresysDominoSymphony_Count], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		{
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_CamConfig, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_CamConfig - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoCamConfig, EDeviceDominoCamConfig_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_Camera, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_Camera - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoCamera, EDeviceDominoCamera_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_DataLink, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_DataLink - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoDataLink, EDeviceDominoDataLink_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_Imaging, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_Imaging - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoImaging, EDeviceDominoImaging_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_Spectrum, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_Spectrum - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoSpectrum, EDeviceDominoSpectrum_Count), nullptr, 1);
		}

		AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_CameraTiming, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_CameraTiming - EDeviceParameterEuresysDominoSymphony_Count], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		{
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_Vactive_Ln, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_Vactive_Ln - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_VsyncAft_Ln, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_VsyncAft_Ln - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_LineRate_Hz, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_LineRate_Hz - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_ACResetPostDelay_us, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_ACResetPostDelay_us - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_ACResetWidth_us, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_ACResetWidth_us - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_HCSyncAft_ns, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_HCSyncAft_ns - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_HCsyncBfr_ns, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_HCsyncBfr_ns - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_HCsyncDur_ns, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_HCsyncDur_ns - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_HsyncDly_ns, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_HsyncDly_ns - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_HdriveDur_ns, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_HdriveDur_ns - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_HdriveDly_ns, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_HdriveDly_ns - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_Hactive_ns, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_Hactive_ns - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_PixelClk_Hz, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_PixelClk_Hz - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_Scanning, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_Scanning - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoScanning, EDeviceDominoScanning_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_Standard, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_Standard - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoStandard, EDeviceDominoStandard_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_VCsyncAft_Ln, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_VCsyncAft_Ln - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_VCgatePos_Ln, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_VCgatePos_Ln - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_Vtotal_Ln, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_Vtotal_Ln - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_VsyncBfrEndExp_us, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_VsyncBfrEndExp_us - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_VdriveDur_Ln, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_VdriveDur_Ln - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_VdriveDly_Ln, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_VdriveDly_Ln - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		}


		AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_CameraFeatures, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_CameraFeatures - EDeviceParameterEuresysDominoSymphony_Count], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		{
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_AuxResetCtl, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_AuxResetCtl - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoAuxResetCtl, EDeviceDominoAuxResetCtl_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_ColorMethod, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_ColorMethod - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoColorMethod, EDeviceDominoColorMethod_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_Expose, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_Expose - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoExpose, EDeviceDominoExpose_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_ExposeMin_us, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_ExposeMin_us - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_ExposeMax_us, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_ExposeMax_us - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_ExposeOverlap, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_ExposeOverlap - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoExposeOverlap, EDeviceDominoExposeOverlap_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_Readout, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_Readout - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoReadout, EDeviceDominoReadout_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_ResetCtl, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_ResetCtl - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoResetCtl, EDeviceDominoResetCtl_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_ResetEdge, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_ResetEdge - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoResetEdge, EDeviceDominoResetEdge_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_ACReset, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_ACReset - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoACReset, EDeviceDominoACReset_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_ChannelTopology, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_ChannelTopology - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoChannelTopology, EDeviceDominoChannelTopology_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_CsyncCtl, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_CsyncCtl - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoCsyncCtl, EDeviceDominoCsyncCtl_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_CsyncEdge, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_CsyncEdge - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoCsyncEdge, EDeviceDominoCsyncEdge_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_CameraControlSignalSet, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_CameraControlSignalSet - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoCameraControlSignalSet, EDeviceDominoCameraControlSignalSet_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_CameraSignalSet, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_CameraSignalSet - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_HReset, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_HReset - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoHReset, EDeviceDominoHReset_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_HsyncCtl, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_HsyncCtl - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoHsyncCtl, EDeviceDominoHsyncCtl_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_HsyncEdge, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_HsyncEdge - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoHsyncEdge, EDeviceDominoHsyncEdge_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_HVdriveCtl, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_HVdriveCtl - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoHVdriveCtl, EDeviceDominoHVdriveCtl_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_HVdriveEdge, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_HVdriveEdge - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoHVdriveEdge, EDeviceDominoHVdriveEdge_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_PixelClkCtl, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_PixelClkCtl - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoPixelClkCtl, EDeviceDominoPixelClkCtl_Count), nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_ResetDur, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_ResetDur - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_ResetPos, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_ResetPos - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_VCSync, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_VCSync - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoVCSync, EDeviceDominoVCSync_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_VHSync, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_VHSync - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoVHSync, EDeviceDominoVHSync_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_VsyncCtl, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_VsyncCtl - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoVsyncCtl, EDeviceDominoVsyncCtl_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_VsyncEdge, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_VsyncEdge - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoVsyncEdge, EDeviceDominoVsyncEdge_Count), nullptr, 1);
		}


		AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_CableFeatures, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_CableFeatures - EDeviceParameterEuresysDominoSymphony_Count], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		{
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_AuxResetLine, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_AuxResetLine - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoAuxResetLine, EDeviceDominoAuxResetLine_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_ResetLine, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_ResetLine - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoResetLine, EDeviceDominoResetLine_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_Cable, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_Cable - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoCable, EDeviceDominoCable_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_CsyncLine, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_CsyncLine - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoCsyncLine, EDeviceDominoCsyncLine_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_HdriveLine, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_HdriveLine - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoHdriveLine, EDeviceDominoHdriveLine_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_HsyncLine, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_HsyncLine - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoHsyncLine, EDeviceDominoHsyncLine_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_PixelClkLine, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_PixelClkLine - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoPixelClkLine, EDeviceDominoPixelClkLine_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_VdriveLine, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_VdriveLine - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoVdriveLine, EDeviceDominoVdriveLine_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_VsyncLine, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_VsyncLine - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoVsyncLine, EDeviceDominoVsyncLine_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_CableTopology, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_CableTopology - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		}


		AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_AcquisitionControl, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_AcquisitionControl - EDeviceParameterEuresysDominoSymphony_Count], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		{
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_ActivityLength, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_ActivityLength - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_BreakEffect, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_BreakEffect - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoBreakEffect, EDeviceDominoBreakEffect_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_EndTrigMode, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_EndTrigMode - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoEndTrigMode, EDeviceDominoEndTrigMode_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_NextTrigMode, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_NextTrigMode - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoNextTrigMode, EDeviceDominoNextTrigMode_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_GrabField, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_GrabField - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoGrabField, EDeviceDominoGrabField_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_NextGrabField, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_NextGrabField - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoNextGrabField, EDeviceDominoNextGrabField_Count), nullptr, 1);
		}


		AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_TriggerControl, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_TriggerControl - EDeviceParameterEuresysDominoSymphony_Count], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		{
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_ForceTrig, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_ForceTrig - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoForceTrig, EDeviceDominoForceTrig_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_TrigDelay_us, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_TrigDelay_us - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		}


		AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_ExposureControl, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_ExposureControl - EDeviceParameterEuresysDominoSymphony_Count], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		{
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_ExposeTrim, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_ExposeTrim - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_TrueExp_us, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_TrueExp_us - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		}


		AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_StrobeControl, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_StrobeControl - EDeviceParameterEuresysDominoSymphony_Count], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		{
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_StrobeCtl, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_StrobeCtl - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoStrobeCtl, EDeviceDominoStrobeCtl_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_StrobeDur, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_StrobeDur - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_StrobeMode, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_StrobeMode - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoStrobeMode, EDeviceDominoStrobeMode_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_StrobePos, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_StrobePos - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_PreStrobe_us, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_PreStrobe_us - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_StrobeLine, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_StrobeLine - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoStrobeLine, EDeviceDominoStrobeLine_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_StrobeLevel, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_StrobeLevel - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoStrobeLevel, EDeviceDominoStrobeLevel_Count), nullptr, 1);
		}


		AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_GrabberConfiguration, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_GrabberConfiguration - EDeviceParameterEuresysDominoSymphony_Count], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		{
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_ADresolution, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_ADresolution - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoADresolution, EDeviceDominoADresolution_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_VideoTerminator, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_VideoTerminator - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoVideoTerminator, EDeviceDominoVideoTerminator_Count), nullptr, 1);
		}


		AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_GrabberTiming, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_GrabberTiming - EDeviceParameterEuresysDominoSymphony_Count], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		{
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_GrabWindow, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_GrabWindow - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoGrabWindow, EDeviceDominoGrabWindow_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_OffsetX_Px, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_OffsetX_Px - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_OffsetY_Ln, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_OffsetY_Ln - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_SampleTrim, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_SampleTrim - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_SampleClk_Hz, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_SampleClk_Hz - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		}

		AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_GrabberConditioning, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_GrabberConditioning - EDeviceParameterEuresysDominoSymphony_Count], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		{
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_GainCtl, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_GainCtl - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoGainCtl, EDeviceDominoGainCtl_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_CalPat, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_CalPat - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoCalPat, EDeviceDominoCalPat_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_InputLut, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_InputLut - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_LutIndex, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_LutIndex - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		}


		AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_BoardLinkage, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_BoardLinkage - EDeviceParameterEuresysDominoSymphony_Count], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		{
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_BoardIdentifier, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_BoardIdentifier - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_BoardName, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_BoardName - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_DriverIndex, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_DriverIndex - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_PciPosition, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_PciPosition - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
		}


		AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_Cluster, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_Cluster - EDeviceParameterEuresysDominoSymphony_Count], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		{
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_ColorFormat, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_ColorFormat - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoColorFormat, EDeviceDominoColorFormat_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_ImageFlipX, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_ImageFlipX - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoImageFlipX, EDeviceDominoImageFlipX_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_ImageFlipY, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_ImageFlipY - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoImageFlipY, EDeviceDominoImageFlipY_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_ImageSizeX, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_ImageSizeX - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);

			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_SurfaceCount, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_SurfaceCount - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_SurfaceAllocation, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_SurfaceAllocation - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoSurfaceAllocation, EDeviceDominoSurfaceAllocation_Count), nullptr, 1);
		}


		AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_ChannelManagement, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_ChannelManagement - EDeviceParameterEuresysDominoSymphony_Count], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		{
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_ChannelState, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_ChannelState - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoChannelState, EDeviceDominoChannelState_Count), nullptr, 1);
		}



		AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_ExceptionManagement, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_ExceptionManagement - EDeviceParameterEuresysDominoSymphony_Count], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);
		{
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_AcquisitionCleanup, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_AcquisitionCleanup - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoAcquisitionCleanup, EDeviceDominoAcquisitionCleanup_Count), nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_AcqTimeout_ms, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_AcqTimeout_ms - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Edit, nullptr, nullptr, 1);
			AddParameterFieldConfigurations(EDeviceParameterEuresysDomino_ParamsConsistencyCheck, g_lpszDeviceDomino[EDeviceParameterEuresysDomino_ParamsConsistencyCheck - EDeviceParameterEuresysDominoSymphony_Count], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDominoParamsConsistencyCheck, EDeviceDominoParamsConsistencyCheck_Count), nullptr, 1);
		}

		bReturn = true;
	}
	while(false);

	return bReturn;
}

void CDeviceEuresysDomino::CallbackFunction(PMCSIGNALINFO CbInfo)
{
	if(!CbInfo)
		return;

	if(CbInfo->Signal != MC_SIG_SURFACE_FILLED)
		return;

	CDeviceEuresysDomino* pDevice = (CDeviceEuresysDomino*)CbInfo->Context;

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

		int nTempWidthStep = 0;
		McGetParamInt(CbInfo->Instance, MC_BufferPitch, &nTempWidthStep);

		pDevice->NextImageIndex();

		CRavidImage* pCurrentImage = pDevice->GetImageInfo();

		BYTE* pCurrentBuffer = pCurrentImage->GetBuffer();
		BYTE** ppCurrentOffsetY = pCurrentImage->GetYOffsetTable();

		const int64_t i64Width = (int64_t)pCurrentImage->GetSizeX();
		const int64_t i64Height = (int64_t)pCurrentImage->GetSizeY();
		const int64_t i64WidthStep = (int64_t)pCurrentImage->GetWidthStep();
		const int64_t i64ImgChannels = (int64_t)pCurrentImage->GetChannels();
		const int64_t i64ImageSizeByte = (int64_t)pCurrentImage->GetImageSizeByte();
		const int64_t i64PixelSizeByte = (int64_t)pCurrentImage->GetPixelSizeByte();

		for(int i = 0; i < nImageSizeY; ++i)
			memcpy((ppCurrentOffsetY[i]), (pCurrent + i * nTempWidthStep), sizeof(BYTE) * nImageSizeX);

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

bool CDeviceEuresysDomino::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = false;

	if(__super::OnParameterChanged(nParam, strValue))
		return true;

	do
	{
		switch(nParam)
		{
		case EDeviceParameterEuresysDomino_AcquisitionMode:
			bReturn = !SetAcquisitionMode((EDeviceDominoAcquisitionMode)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_TrigMode:
			bReturn = !SetTrigMode((EDeviceDominoTrigMode)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_Expose_us:
			bReturn = !SetExpose_us(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_SeqLength_Fr:
			bReturn = !SetSeqLength_Fr(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_PhaseLength_Fr:
			bReturn = !SetPhaseLength_Fr(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_TapStructure:
			bReturn = !SetTapStructure((EDeviceDominoTapStructure)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_Gain:
			bReturn = !SetGain(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_Offset:
			bReturn = !SetOffset(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_VideoFilter:
			bReturn = !SetVideoFilter((EDeviceDominoVideoFilter)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_CamConfig:
			bReturn = !SetCamConfig((EDeviceDominoCamConfig)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_Camera:
			bReturn = !SetCamera((EDeviceDominoCamera)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_DataLink:
			bReturn = !SetDataLink((EDeviceDominoDataLink)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_Imaging:
			bReturn = !SetImaging((EDeviceDominoImaging)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_Spectrum:
			bReturn = !SetSpectrum((EDeviceDominoSpectrum)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_Vactive_Ln:
			bReturn = !SetVactive_Ln(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_VsyncAft_Ln:
			bReturn = !SetVsyncAft_Ln(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_LineRate_Hz:
			bReturn = !SetLineRate_Hz(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_ACResetPostDelay_us:
			bReturn = !SetACResetPostDelay_us(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_ACResetWidth_us:
			bReturn = !SetACResetWidth_us(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_HCSyncAft_ns:
			bReturn = !SetHCSyncAft_ns(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_HCsyncBfr_ns:
			bReturn = !SetHCsyncBfr_ns(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_HCsyncDur_ns:
			bReturn = !SetHCsyncDur_ns(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_HsyncDly_ns:
			bReturn = !SetHsyncDly_ns(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_HdriveDur_ns:
			bReturn = !SetHdriveDur_ns(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_HdriveDly_ns:
			bReturn = !SetHdriveDly_ns(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_Hactive_ns:
			bReturn = !SetHactive_ns(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_PixelClk_Hz:
			bReturn = !SetPixelClk_Hz(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_Scanning:
			bReturn = !SetScanning((EDeviceDominoScanning)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_Standard:
			bReturn = !SetStandard((EDeviceDominoStandard)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_VCsyncAft_Ln:
			bReturn = !SetVCsyncAft_Ln(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_VCgatePos_Ln:
			bReturn = !SetVCgatePos_Ln(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_Vtotal_Ln:
			bReturn = !SetVtotal_Ln(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_VsyncBfrEndExp_us:
			bReturn = !SetVsyncBfrEndExp_us(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_VdriveDur_Ln:
			bReturn = !SetVdriveDur_Ln(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_VdriveDly_Ln:
			bReturn = !SetVdriveDly_Ln(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_AuxResetCtl:
			bReturn = !SetAuxResetCtl((EDeviceDominoAuxResetCtl)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_ColorMethod:
			bReturn = !SetColorMethod((EDeviceDominoColorMethod)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_Expose:
			bReturn = !SetExpose((EDeviceDominoExpose)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_ExposeMin_us:
			bReturn = !SetExposeMin_us(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_ExposeMax_us:
			bReturn = !SetExposeMax_us(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_ExposeOverlap:
			bReturn = !SetExposeOverlap((EDeviceDominoExposeOverlap)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_Readout:
			bReturn = !SetReadout((EDeviceDominoReadout)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_ResetCtl:
			bReturn = !SetResetCtl((EDeviceDominoResetCtl)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_ResetEdge:
			bReturn = !SetResetEdge((EDeviceDominoResetEdge)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_ACReset:
			bReturn = !SetACReset((EDeviceDominoACReset)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_ChannelTopology:
			bReturn = !SetChannelTopology((EDeviceDominoChannelTopology)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_CsyncCtl:
			bReturn = !SetCsyncCtl((EDeviceDominoCsyncCtl)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_CsyncEdge:
			bReturn = !SetCsyncEdge((EDeviceDominoCsyncEdge)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_CameraControlSignalSet:
			bReturn = !SetCameraControlSignalSet((EDeviceDominoCameraControlSignalSet)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_CameraSignalSet:
			bReturn = !SetCameraSignalSet(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_HReset:
			bReturn = !SetHReset((EDeviceDominoHReset)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_HsyncCtl:
			bReturn = !SetHsyncCtl((EDeviceDominoHsyncCtl)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_HsyncEdge:
			bReturn = !SetHsyncEdge((EDeviceDominoHsyncEdge)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_HVdriveCtl:
			bReturn = !SetHVdriveCtl((EDeviceDominoHVdriveCtl)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_HVdriveEdge:
			bReturn = !SetHVdriveEdge((EDeviceDominoHVdriveEdge)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_PixelClkCtl:
			bReturn = !SetPixelClkCtl((EDeviceDominoPixelClkCtl)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_ResetDur:
			bReturn = !SetResetDur(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_ResetPos:
			bReturn = !SetResetPos(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_VCSync:
			bReturn = !SetVCSync((EDeviceDominoVCSync)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_VHSync:
			bReturn = !SetVHSync((EDeviceDominoVHSync)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_VsyncCtl:
			bReturn = !SetVsyncCtl((EDeviceDominoVsyncCtl)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_VsyncEdge:
			bReturn = !SetVsyncEdge((EDeviceDominoVsyncEdge)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_AuxResetLine:
			bReturn = !SetAuxResetLine((EDeviceDominoAuxResetLine)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_ResetLine:
			bReturn = !SetResetLine((EDeviceDominoResetLine)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_Cable:
			bReturn = !SetCable((EDeviceDominoCable)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_CsyncLine:
			bReturn = !SetCsyncLine((EDeviceDominoCsyncLine)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_HdriveLine:
			bReturn = !SetHdriveLine((EDeviceDominoHdriveLine)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_HsyncLine:
			bReturn = !SetHsyncLine((EDeviceDominoHsyncLine)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_PixelClkLine:
			bReturn = !SetPixelClkLine((EDeviceDominoPixelClkLine)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_VdriveLine:
			bReturn = !SetVdriveLine((EDeviceDominoVdriveLine)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_VsyncLine:
			bReturn = !SetVsyncLine((EDeviceDominoVsyncLine)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_CableTopology:
			bReturn = !SetCableTopology(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_ActivityLength:
			bReturn = !SetActivityLength(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_BreakEffect:
			bReturn = !SetBreakEffect((EDeviceDominoBreakEffect)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_EndTrigMode:
			bReturn = !SetEndTrigMode((EDeviceDominoEndTrigMode)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_NextTrigMode:
			bReturn = !SetNextTrigMode((EDeviceDominoNextTrigMode)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_GrabField:
			bReturn = !SetGrabField((EDeviceDominoGrabField)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_NextGrabField:
			bReturn = !SetNextGrabField((EDeviceDominoNextGrabField)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_ForceTrig:
			bReturn = !SetForceTrig((EDeviceDominoForceTrig)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_TrigDelay_us:
			bReturn = !SetTrigDelay_us(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_ExposeTrim:
			bReturn = !SetExposeTrim(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_TrueExp_us:
			bReturn = !SetTrueExp_us(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_StrobeCtl:
			bReturn = !SetStrobeCtl((EDeviceDominoStrobeCtl)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_StrobeDur:
			bReturn = !SetStrobeDur(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_StrobeMode:
			bReturn = !SetStrobeMode((EDeviceDominoStrobeMode)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_StrobePos:
			bReturn = !SetStrobePos(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_PreStrobe_us:
			bReturn = !SetPreStrobe_us(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_StrobeLine:
			bReturn = !SetStrobeLine((EDeviceDominoStrobeLine)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_StrobeLevel:
			bReturn = !SetStrobeLevel((EDeviceDominoStrobeLevel)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_ADresolution:
			bReturn = !SetADresolution((EDeviceDominoADresolution)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_VideoTerminator:
			bReturn = !SetVideoTerminator((EDeviceDominoVideoTerminator)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_GrabWindow:
			bReturn = !SetGrabWindow((EDeviceDominoGrabWindow)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_OffsetX_Px:
			bReturn = !SetOffsetX_Px(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_OffsetY_Ln:
			bReturn = !SetOffsetY_Ln(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_SampleTrim:
			bReturn = !SetSampleTrim(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_SampleClk_Hz:
			bReturn = !SetSampleClk_Hz(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_GainCtl:
			bReturn = !SetGainCtl((EDeviceDominoGainCtl)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_CalPat:
			bReturn = !SetCalPat((EDeviceDominoCalPat)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_InputLut:
			bReturn = !SetInputLut(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_LutIndex:
			bReturn = !SetLutIndex(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_BoardIdentifier:
			bReturn = !SetBoardIdentifier(strValue);
			break;
		case EDeviceParameterEuresysDomino_BoardName:
			bReturn = !SetBoardName(strValue);
			break;
		case EDeviceParameterEuresysDomino_DriverIndex:
			bReturn = !SetDriverIndex(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_PciPosition:
			bReturn = !SetPciPosition(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_ColorFormat:
			bReturn = !SetColorFormat((EDeviceDominoColorFormat)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_ImageFlipX:
			bReturn = !SetImageFlipX((EDeviceDominoImageFlipX)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_ImageFlipY:
			bReturn = !SetImageFlipY((EDeviceDominoImageFlipY)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_ImageSizeX:
			bReturn = !SetImageSizeX(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_SurfaceCount:
			bReturn = !SetSurfaceCount(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_SurfaceAllocation:
			bReturn = !SetSurfaceAllocation((EDeviceDominoSurfaceAllocation)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_ChannelState:
			bReturn = !SetChannelState((EDeviceDominoChannelState)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_AcquisitionCleanup:
			bReturn = !SetAcquisitionCleanup((EDeviceDominoAcquisitionCleanup)_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_AcqTimeout_ms:
			bReturn = !SetAcqTimeout_ms(_ttoi(strValue));
			break;
		case EDeviceParameterEuresysDomino_ParamsConsistencyCheck:
			bReturn = !SetParamsConsistencyCheck((EDeviceDominoParamsConsistencyCheck)_ttoi(strValue));
			break;
		default:
			break;
		}
	}
	while(false);

	return bReturn;
}

void CDeviceEuresysDomino::SetUpdateDevice()
{
	CString strLoadDB;
	int nLoadDB = -1;

	strLoadDB.Empty();

	EDeviceDominoAcquisitionMode eAcquisitionMode = EDeviceDominoAcquisitionMode_Count;
	if(!GetAcquisitionMode(&eAcquisitionMode))
		McSetParamStr(m_hDevice, MC_AcquisitionMode, CStringA(g_lpszDominoAcquisitionMode[eAcquisitionMode]));

	EDeviceDominoTrigMode eTrigMode = EDeviceDominoTrigMode_Count;
	if(!GetTrigMode(&eTrigMode))
		McSetParamStr(m_hDevice, MC_TrigMode, CStringA(g_lpszDominoTrigMode[eTrigMode]));

	EDeviceDominoVideoFilter eVideoFilter = EDeviceDominoVideoFilter_Count;
	if(!GetVideoFilter(&eVideoFilter))
		McSetParamStr(m_hDevice, MC_VideoFilter, CStringA(g_lpszDominoVideoFilter[eVideoFilter]));

	EDeviceDominoCamConfig eCamConfig = EDeviceDominoCamConfig_Count;
	if(!GetCamConfig(&eCamConfig))
		McSetParamStr(m_hDevice, MC_CamConfig, CStringA(g_lpszDominoCamConfig[eCamConfig]));

	EDeviceDominoCamera eCamera = EDeviceDominoCamera_Count;
	if(!GetCamera(&eCamera))
		McSetParamStr(m_hDevice, MC_Camera, CStringA(g_lpszDominoCamera[eCamera]));

	EDeviceDominoDataLink eDataLink = EDeviceDominoDataLink_Count;
	if(!GetDataLink(&eDataLink))
		McSetParamStr(m_hDevice, MC_DataLink, CStringA(g_lpszDominoDataLink[eDataLink]));



	EDeviceDominoImaging eImaging = EDeviceDominoImaging_Count;
	if(!!GetImaging(&eImaging))
		McSetParamStr(m_hDevice, MC_Imaging, CStringA(g_lpszDominoImaging[eImaging]));

	if(eImaging == 0)
		m_bIsLine = false;
	else
		m_bIsLine = true;



	EDeviceDominoSpectrum eSpectrum = EDeviceDominoSpectrum_Count;
	if(!!GetSpectrum(&eSpectrum))
		McSetParamStr(m_hDevice, MC_Spectrum, CStringA(g_lpszDominoSpectrum[eSpectrum]));

	if(eSpectrum == 1)
		m_bIsColor = true;
	else
		m_bIsColor = false;



	EDeviceDominoTapStructure eTapStructure = EDeviceDominoTapStructure_Count;
	if(!GetTapStructure(&eTapStructure))
		McSetParamStr(m_hDevice, MC_TapStructure, CStringA(g_lpszDominoTapStructure[eTapStructure]));



	EDeviceDominoScanning eScanning = EDeviceDominoScanning_Count;
	if(!GetScanning(&eScanning))
		McSetParamStr(m_hDevice, MC_Scanning, CStringA(g_lpszDominoScanning[eScanning]));



	EDeviceDominoStandard eStandard = EDeviceDominoStandard_Count;
	if(!GetStandard(&eStandard))
		McSetParamStr(m_hDevice, MC_Standard, CStringA(g_lpszDominoStandard[eStandard]));



	EDeviceDominoAuxResetCtl eAuxResetCtl = EDeviceDominoAuxResetCtl_Count;
	if(!GetAuxResetCtl(&eAuxResetCtl))
		McSetParamStr(m_hDevice, MC_AuxResetCtl, CStringA(g_lpszDominoAuxResetCtl[eAuxResetCtl]));



	EDeviceDominoColorMethod eColorMethod = EDeviceDominoColorMethod_Count;
	if(!GetColorMethod(&eColorMethod))
		McSetParamStr(m_hDevice, MC_ColorMethod, CStringA(g_lpszDominoColorMethod[eColorMethod]));



	EDeviceDominoExposeOverlap eExposeOverlap = EDeviceDominoExposeOverlap_Count;
	if(!GetExposeOverlap(&eExposeOverlap))
		McSetParamStr(m_hDevice, MC_ExposeOverlap, CStringA(g_lpszDominoExposeOverlap[eExposeOverlap]));



	EDeviceDominoReadout eDominoReadout = EDeviceDominoReadout_Count;
	if(!GetReadout(&eDominoReadout))
		McSetParamStr(m_hDevice, MC_Readout, CStringA(g_lpszDominoReadout[eDominoReadout]));



	EDeviceDominoResetCtl eResetCtl = EDeviceDominoResetCtl_Count;
	if(!GetResetCtl(&eResetCtl))
		McSetParamStr(m_hDevice, MC_ResetCtl, CStringA(g_lpszDominoResetCtl[eResetCtl]));




	EDeviceDominoResetEdge eResetEdge = EDeviceDominoResetEdge_Count;
	if(!GetResetEdge(&eResetEdge))
		McSetParamStr(m_hDevice, MC_ResetEdge, CStringA(g_lpszDominoResetEdge[eResetEdge]));



	EDeviceDominoACReset eACReset = EDeviceDominoACReset_Count;
	if(!GetACReset(&eACReset))
		McSetParamStr(m_hDevice, MC_ACReset, CStringA(g_lpszDominoACReset[eACReset]));



	EDeviceDominoChannelTopology eChannelTopology = EDeviceDominoChannelTopology_Count;
	if(!GetChannelTopology(&eChannelTopology))
		McSetParamStr(m_hDevice, MC_ChannelTopology, CStringA(g_lpszDominoChannelTopology[eChannelTopology]));



	EDeviceDominoHReset eHReset = EDeviceDominoHReset_Count;
	if(!GetHReset(&eHReset))
		McSetParamStr(m_hDevice, MC_HReset, CStringA(g_lpszDominoHReset[eHReset]));



	EDeviceDominoHsyncCtl eHsyncCtl = EDeviceDominoHsyncCtl_Count;
	if(!GetHsyncCtl(&eHsyncCtl))
		McSetParamStr(m_hDevice, MC_HsyncCtl, CStringA(g_lpszDominoHsyncCtl[eHsyncCtl]));



	EDeviceDominoHsyncEdge eHsyncEdge = EDeviceDominoHsyncEdge_Count;
	if(!GetHsyncEdge(&eHsyncEdge))
		McSetParamStr(m_hDevice, MC_HsyncEdge, CStringA(g_lpszDominoHsyncEdge[eHsyncEdge]));



	EDeviceDominoHVdriveCtl eHVdriveCtl = EDeviceDominoHVdriveCtl_Count;
	if(!GetHVdriveCtl(&eHVdriveCtl))
		McSetParamStr(m_hDevice, MC_HVdriveCtl, CStringA(g_lpszDominoHVdriveCtl[eHVdriveCtl]));



	EDeviceDominoHVdriveEdge eHVdriveEdge = EDeviceDominoHVdriveEdge_Count;
	if(!GetHVdriveEdge(&eHVdriveEdge))
		McSetParamStr(m_hDevice, MC_HVdriveEdge, CStringA(g_lpszDominoHVdriveEdge[eHVdriveEdge]));



	EDeviceDominoPixelClkCtl ePixelClkCtl = EDeviceDominoPixelClkCtl_Count;
	if(!GetPixelClkCtl(&ePixelClkCtl))
		McSetParamStr(m_hDevice, MC_PixelClkCtl, CStringA(g_lpszDominoPixelClkCtl[ePixelClkCtl]));



	EDeviceDominoVCSync eVCSync = EDeviceDominoVCSync_Count;
	if(!GetVCSync(&eVCSync))
		McSetParamStr(m_hDevice, MC_VCSync, CStringA(g_lpszDominoVCSync[eVCSync]));



	EDeviceDominoVHSync eVHSync = EDeviceDominoVHSync_Count;
	if(!GetVHSync(&eVHSync))
		McSetParamStr(m_hDevice, MC_VHSync, CStringA(g_lpszDominoVHSync[eVHSync]));



	EDeviceDominoVsyncCtl eVsyncCtl = EDeviceDominoVsyncCtl_Count;
	if(!GetVsyncCtl(&eVsyncCtl))
		McSetParamStr(m_hDevice, MC_VsyncCtl, CStringA(g_lpszDominoVsyncCtl[eVsyncCtl]));



	EDeviceDominoVsyncEdge eVsyncEdge = EDeviceDominoVsyncEdge_Count;
	if(!GetVsyncEdge(&eVsyncEdge))
		McSetParamStr(m_hDevice, MC_VsyncEdge, CStringA(g_lpszDominoVsyncEdge[eVsyncEdge]));



	EDeviceDominoBreakEffect eBreakEffect = EDeviceDominoBreakEffect_Count;
	if(!GetBreakEffect(&eBreakEffect))
		McSetParamStr(m_hDevice, MC_BreakEffect, CStringA(g_lpszDominoBreakEffect[eBreakEffect]));

	EDeviceDominoEndTrigMode eEndTrigMode = EDeviceDominoEndTrigMode_Count;
	if(!GetEndTrigMode(&eEndTrigMode))
		McSetParamStr(m_hDevice, MC_EndTrigMode, CStringA(g_lpszDominoEndTrigMode[eEndTrigMode]));


	EDeviceDominoNextTrigMode eNextTrigMode = EDeviceDominoNextTrigMode_Count;
	if(!GetNextTrigMode(&eNextTrigMode))
		McSetParamStr(m_hDevice, MC_NextTrigMode, CStringA(g_lpszDominoNextTrigMode[eNextTrigMode]));


	EDeviceDominoAuxResetLine eAuxResetLine = EDeviceDominoAuxResetLine_Count;
	if(!GetAuxResetLine(&eAuxResetLine))
		McSetParamStr(m_hDevice, MC_AuxresetLine, CStringA(g_lpszDominoAuxResetLine[eAuxResetLine]));



	EDeviceDominoCable eCable = EDeviceDominoCable_Count;
	if(!GetCable(&eCable))
		McSetParamStr(m_hDevice, MC_Cable, CStringA(g_lpszDominoCable[eCable]));



	EDeviceDominoCsyncLine eCsyncLine = EDeviceDominoCsyncLine_Count;
	if(!GetCsyncLine(&eCsyncLine))
		McSetParamStr(m_hDevice, MC_CsyncLine, CStringA(g_lpszDominoCsyncLine[eCsyncLine]));



	EDeviceDominoHdriveLine eHdriveLine = EDeviceDominoHdriveLine_Count;
	if(!GetHdriveLine(&eHdriveLine))
		McSetParamStr(m_hDevice, MC_HdriveLine, CStringA(g_lpszDominoHdriveLine[eHdriveLine]));



	EDeviceDominoHsyncLine eHsyncLine = EDeviceDominoHsyncLine_Count;
	if(!GetHsyncLine(&eHsyncLine))
		McSetParamStr(m_hDevice, MC_HsyncLine, CStringA(g_lpszDominoHsyncLine[eHsyncLine]));



	EDeviceDominoPixelClkLine ePixelClkLine = EDeviceDominoPixelClkLine_Count;
	if(!GetPixelClkLine(&ePixelClkLine))
		McSetParamStr(m_hDevice, MC_PixelClkLine, CStringA(g_lpszDominoPixelClkLine[ePixelClkLine]));



	EDeviceDominoVdriveLine eVdriveLine = EDeviceDominoVdriveLine_Count;
	if(!GetVdriveLine(&eVdriveLine))
		McSetParamStr(m_hDevice, MC_VdriveLine, CStringA(g_lpszDominoVdriveLine[eVdriveLine]));



	EDeviceDominoVsyncLine eVsyncLine = EDeviceDominoVsyncLine_Count;
	if(!GetVsyncLine(&eVsyncLine))
		McSetParamStr(m_hDevice, MC_VsyncLine, CStringA(g_lpszDominoVsyncLine[eVsyncLine]));



	EDeviceDominoResetLine eResetLine = EDeviceDominoResetLine_Count;
	if(!GetResetLine(&eResetLine))
		McSetParamStr(m_hDevice, MC_ResetLine, CStringA(g_lpszDominoResetLine[eResetLine]));



	EDeviceDominoGrabField eGrabField = EDeviceDominoGrabField_Count;
	if(!GetGrabField(&eGrabField))
		McSetParamStr(m_hDevice, MC_GrabField, CStringA(g_lpszDominoGrabField[eGrabField]));



	EDeviceDominoNextGrabField eNextGrabField = EDeviceDominoNextGrabField_Count;
	if(!GetNextGrabField(&eNextGrabField))
		McSetParamStr(m_hDevice, MC_NextGrabField, CStringA(g_lpszDominoNextGrabField[eNextGrabField]));



	EDeviceDominoForceTrig eForceTrig = EDeviceDominoForceTrig_Count;
	if(!GetForceTrig(&eForceTrig))
		McSetParamStr(m_hDevice, MC_ForceTrig, CStringA(g_lpszDominoForceTrig[eForceTrig]));



	EDeviceDominoStrobeCtl eStrobeCtl = EDeviceDominoStrobeCtl_Count;
	if(!GetStrobeCtl(&eStrobeCtl))
		McSetParamStr(m_hDevice, MC_StrobeCtl, CStringA(g_lpszDominoStrobeCtl[eStrobeCtl]));



	EDeviceDominoStrobeMode eStrobeMode = EDeviceDominoStrobeMode_Count;
	if(!GetStrobeMode(&eStrobeMode))
		McSetParamStr(m_hDevice, MC_StrobeMode, CStringA(g_lpszDominoStrobeMode[eStrobeMode]));



	EDeviceDominoStrobeLine eStrobeLine = EDeviceDominoStrobeLine_Count;
	if(!GetStrobeLine(&eStrobeLine))
		McSetParamStr(m_hDevice, MC_StrobeLine, CStringA(g_lpszDominoStrobeLine[eStrobeLine]));



	EDeviceDominoStrobeLevel eStrobeLevel = EDeviceDominoStrobeLevel_Count;
	if(!GetStrobeLevel(&eStrobeLevel))
		McSetParamStr(m_hDevice, MC_StrobeLevel, CStringA(g_lpszDominoStrobeLevel[eStrobeLevel]));



	EDeviceDominoADresolution eADresolution = EDeviceDominoADresolution_Count;
	if(!GetADresolution(&eADresolution))
		McSetParamStr(m_hDevice, MC_ADresolution, CStringA(g_lpszDominoADresolution[eADresolution]));



	EDeviceDominoVideoTerminator eVideoTerminator = EDeviceDominoVideoTerminator_Count;
	if(!GetVideoTerminator(&eVideoTerminator))
		McSetParamStr(m_hDevice, MC_VideoTerminator, CStringA(g_lpszDominoVideoTerminator[eVideoTerminator]));



	EDeviceDominoGrabWindow eGrabWindow = EDeviceDominoGrabWindow_Count;
	if(!GetGrabWindow(&eGrabWindow))
		McSetParamStr(m_hDevice, MC_GrabWindow, CStringA(g_lpszDominoGrabWindow[eGrabWindow]));



	EDeviceDominoGainCtl eGainCtl = EDeviceDominoGainCtl_Count;
	if(!GetGainCtl(&eGainCtl))
		McSetParamStr(m_hDevice, MC_GainCtl, CStringA(g_lpszDominoGainCtl[eGainCtl]));



	EDeviceDominoCalPat eCalPat = EDeviceDominoCalPat_Count;
	if(!GetCalPat(&eCalPat))
		McSetParamStr(m_hDevice, MC_CalPat, CStringA(g_lpszDominoCalPat[eCalPat]));

	strLoadDB.Empty();

	if(!GetBoardIdentifier(&strLoadDB))
		McSetParamStr(m_hDevice, MC_BoardIdentifier, CStringA(strLoadDB));

	strLoadDB.Empty();

	if(!GetBoardName(&strLoadDB))
		McSetParamStr(m_hDevice, MC_BoardName, CStringA(strLoadDB));


	EDeviceDominoColorFormat eColorFormat = EDeviceDominoColorFormat_Count;
	if(!GetColorFormat(&eColorFormat))
		McSetParamStr(m_hDevice, MC_ColorFormat, CStringA(g_lpszDominoColorFormat[eColorFormat]));



	EDeviceDominoImageFlipX eImageFlipX = EDeviceDominoImageFlipX_Count;
	if(!GetImageFlipX(&eImageFlipX))
		McSetParamStr(m_hDevice, MC_ImageFlipX, CStringA(g_lpszDominoImageFlipX[eImageFlipX]));



	EDeviceDominoImageFlipY eImageFlipY = EDeviceDominoImageFlipY_Count;
	if(!GetImageFlipY(&eImageFlipY))
		McSetParamStr(m_hDevice, MC_ImageFlipY, CStringA(g_lpszDominoImageFlipY[eImageFlipY]));



	EDeviceDominoSurfaceAllocation eSurfaceAllocation = EDeviceDominoSurfaceAllocation_Count;
	if(!GetSurfaceAllocation(&eSurfaceAllocation))
		McSetParamStr(m_hDevice, MC_SurfaceAllocation, CStringA(g_lpszDominoSurfaceAllocation[eSurfaceAllocation]));



	EDeviceDominoChannelState eChannelState = EDeviceDominoChannelState_Count;
	if(!GetChannelState(&eChannelState))
		McSetParamStr(m_hDevice, MC_ChannelState, CStringA(g_lpszDominoChannelState[eChannelState]));



	EDeviceDominoAcquisitionCleanup eAcquisitionCleanup = EDeviceDominoAcquisitionCleanup_Count;
	if(!GetAcquisitionCleanup(&eAcquisitionCleanup))
		McSetParamStr(m_hDevice, MC_AcquisitionCleanup, CStringA(g_lpszDominoAcquisitionCleanup[eAcquisitionCleanup]));



	EDeviceDominoParamsConsistencyCheck eParamsConsistencyCheck = EDeviceDominoParamsConsistencyCheck_Count;
	if(!GetParamsConsistencyCheck(&eParamsConsistencyCheck))
		McSetParamStr(m_hDevice, MC_ParamsConsistencyCheck, CStringA(g_lpszDominoParamsConsistencyCheck[eParamsConsistencyCheck]));

	nLoadDB = -1;

	if(!GetExpose_us(&nLoadDB))
		McSetParamInt(m_hDevice, MC_Expose_us, nLoadDB);

	nLoadDB = -1;

	if(!GetSeqLength_Fr(&nLoadDB))
		McSetParamInt(m_hDevice, MC_SeqLength_Fr, nLoadDB);

	nLoadDB = -1;

	if(!GetPhaseLength_Fr(&nLoadDB))
		McSetParamInt(m_hDevice, MC_PhaseLength_Fr, nLoadDB);

	nLoadDB = -1;

	if(!GetGain(&nLoadDB))
		McSetParamInt(m_hDevice, MC_Gain, nLoadDB);

	nLoadDB = -1;

	if(!GetOffset(&nLoadDB))
		McSetParamInt(m_hDevice, MC_Offset, nLoadDB);

	nLoadDB = -1;

	if(!GetVactive_Ln(&nLoadDB))
		McSetParamInt(m_hDevice, MC_Vactive_Ln, nLoadDB);

	nLoadDB = -1;

	if(!GetVsyncAft_Ln(&nLoadDB))
		McSetParamInt(m_hDevice, MC_VsyncAft_Ln, nLoadDB);

	nLoadDB = -1;

	if(!GetLineRate_Hz(&nLoadDB))
		McSetParamInt(m_hDevice, MC_LineRate_Hz, nLoadDB);

	nLoadDB = -1;

	if(!GetACResetPostDelay_us(&nLoadDB))
		McSetParamInt(m_hDevice, MC_ACResetPostDelay_us, nLoadDB);

	nLoadDB = -1;

	if(!GetACResetWidth_us(&nLoadDB))
		McSetParamInt(m_hDevice, MC_ACResetWidth_us, nLoadDB);

	nLoadDB = -1;

	if(!GetHCSyncAft_ns(&nLoadDB))
		McSetParamInt(m_hDevice, MC_HCSyncAft_ns, nLoadDB);

	nLoadDB = -1;

	if(!GetHCsyncBfr_ns(&nLoadDB))
		McSetParamInt(m_hDevice, MC_HCsyncBfr_ns, nLoadDB);

	nLoadDB = -1;

	if(!GetHCsyncDur_ns(&nLoadDB))
		McSetParamInt(m_hDevice, MC_HCsyncDur_ns, nLoadDB);

	nLoadDB = -1;

	if(!GetHsyncDly_ns(&nLoadDB))
		McSetParamInt(m_hDevice, MC_HsyncDly_ns, nLoadDB);

	nLoadDB = -1;

	if(!GetHdriveDur_ns(&nLoadDB))
		McSetParamInt(m_hDevice, MC_HdriveDur_ns, nLoadDB);

	nLoadDB = -1;

	if(!GetHdriveDly_ns(&nLoadDB))
		McSetParamInt(m_hDevice, MC_HdriveDly_ns, nLoadDB);

	nLoadDB = -1;

	if(!GetHactive_ns(&nLoadDB))
		McSetParamInt(m_hDevice, MC_Hactive_ns, nLoadDB);

	nLoadDB = -1;

	if(!GetPixelClk_Hz(&nLoadDB))
		McSetParamInt(m_hDevice, MC_PixelClk_Hz, nLoadDB);

	nLoadDB = -1;

	if(!GetVCsyncAft_Ln(&nLoadDB))
		McSetParamInt(m_hDevice, MC_VCsyncAft_Ln, nLoadDB);

	nLoadDB = -1;

	if(!GetVCgatePos_Ln(&nLoadDB))
		McSetParamInt(m_hDevice, MC_VCgatePos_Ln, nLoadDB);

	nLoadDB = -1;

	if(!GetVtotal_Ln(&nLoadDB))
		McSetParamInt(m_hDevice, MC_Vtotal_Ln, nLoadDB);

	nLoadDB = -1;

	if(!GetVsyncBfrEndExp_us(&nLoadDB))
		McSetParamInt(m_hDevice, MC_VsyncBfrEndExp_us, nLoadDB);

	nLoadDB = -1;

	if(!GetVdriveDur_Ln(&nLoadDB))
		McSetParamInt(m_hDevice, MC_VdriveDur_Ln, nLoadDB);

	nLoadDB = -1;

	if(!GetVdriveDly_Ln(&nLoadDB))
		McSetParamInt(m_hDevice, MC_VdriveDly_Ln, nLoadDB);

	nLoadDB = -1;

	if(!GetExposeMin_us(&nLoadDB))
		McSetParamInt(m_hDevice, MC_ExposeMin_us, nLoadDB);

	nLoadDB = -1;

	if(!GetExposeMax_us(&nLoadDB))
		McSetParamInt(m_hDevice, MC_ExposeMax_us, nLoadDB);

	nLoadDB = -1;

	if(!GetCameraSignalSet(&nLoadDB))
		McSetParamInt(m_hDevice, MC_CameraSignalSet, nLoadDB);

	nLoadDB = -1;

	if(!GetResetDur(&nLoadDB))
		McSetParamInt(m_hDevice, MC_ResetDur, nLoadDB);

	nLoadDB = -1;

	if(!GetResetPos(&nLoadDB))
		McSetParamInt(m_hDevice, MC_ResetPos, nLoadDB);

	nLoadDB = -1;

	if(!GetCableTopology(&nLoadDB))
		McSetParamInt(m_hDevice, MC_CableTopology, nLoadDB);

	nLoadDB = -1;

	if(!GetActivityLength(&nLoadDB))
		McSetParamInt(m_hDevice, MC_ActivityLength, nLoadDB);

	nLoadDB = -1;

	if(!GetTrigDelay_us(&nLoadDB))
		McSetParamInt(m_hDevice, MC_TrigDelay_us, nLoadDB);

	nLoadDB = -1;

	if(!GetTrueExp_us(&nLoadDB))
		McSetParamInt(m_hDevice, MC_TrueExp_us, nLoadDB);

	nLoadDB = -1;

	if(!GetExposeTrim(&nLoadDB))
		McSetParamInt(m_hDevice, MC_ExposeTrim, nLoadDB);

	nLoadDB = -1;

	if(!GetStrobeDur(&nLoadDB))
		McSetParamInt(m_hDevice, MC_StrobeDur, nLoadDB);

	nLoadDB = -1;

	if(!GetStrobePos(&nLoadDB))
		McSetParamInt(m_hDevice, MC_StrobePos, nLoadDB);

	nLoadDB = -1;

	if(!GetPreStrobe_us(&nLoadDB))
		McSetParamInt(m_hDevice, MC_PreStrobe_us, nLoadDB);

	nLoadDB = -1;

	if(!GetOffsetX_Px(&nLoadDB))
		McSetParamInt(m_hDevice, MC_OffsetX_Px, nLoadDB);

	nLoadDB = -1;

	if(!GetOffsetY_Ln(&nLoadDB))
		McSetParamInt(m_hDevice, MC_OffsetY_Ln, nLoadDB);

	nLoadDB = -1;

	if(!GetSampleTrim(&nLoadDB))
		McSetParamInt(m_hDevice, MC_SampleTrim, nLoadDB);

	nLoadDB = -1;

	if(!GetSampleClk_Hz(&nLoadDB))
		McSetParamInt(m_hDevice, MC_SampleClk_Hz, nLoadDB);

	nLoadDB = -1;

	if(!GetInputLut(&nLoadDB))
		McSetParamInt(m_hDevice, MC_InputLut, nLoadDB);

	nLoadDB = -1;

	if(!GetLutIndex(&nLoadDB))
		McSetParamInt(m_hDevice, MC_LutIndex, nLoadDB);

	nLoadDB = -1;

	if(!GetDriverIndex(&nLoadDB))
		McSetParamInt(m_hDevice, MC_DriverIndex, nLoadDB);

	nLoadDB = -1;

	if(!GetPciPosition(&nLoadDB))
		McSetParamInt(m_hDevice, MC_PciPosition, nLoadDB);

	nLoadDB = -1;

	if(!GetImageSizeX(&nLoadDB))
		McSetParamInt(m_hDevice, MC_ImageSizeX, nLoadDB);

	nLoadDB = -1;

	if(!GetAcqTimeout_ms(&nLoadDB))
		McSetParamInt(m_hDevice, MC_AcqTimeout_ms, nLoadDB);
}

void CDeviceEuresysDomino::SetUpdateParameter()
{
	char strSaveDB[32];
	int nSaveDB = -1;

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_AcquisitionMode, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoAcquisitionMode, EDeviceDominoAcquisitionMode_Count));

		SetParamValue(EDeviceParameterEuresysDomino_AcquisitionMode, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_AcquisitionMode);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_TrigMode, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoTrigMode, EDeviceDominoTrigMode_Count));

		SetParamValue(EDeviceParameterEuresysDomino_TrigMode, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_TrigMode);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_VideoFilter, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoVideoFilter, EDeviceDominoVideoFilter_Count));

		SetParamValue(EDeviceParameterEuresysDomino_VideoFilter, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_VideoFilter);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_CamConfig, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoCamConfig, EDeviceDominoCamConfig_Count));

		SetParamValue(EDeviceParameterEuresysDomino_CamConfig, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_CamConfig);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_Camera, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoCamera, EDeviceDominoCamera_Count));

		SetParamValue(EDeviceParameterEuresysDomino_Camera, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_Camera);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_DataLink, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoDataLink, EDeviceDominoDataLink_Count));

		SetParamValue(EDeviceParameterEuresysDomino_DataLink, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_DataLink);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_Imaging, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoImaging, EDeviceDominoImaging_Count));

		SetParamValue(EDeviceParameterEuresysDomino_Imaging, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_Imaging);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_Spectrum, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoSpectrum, EDeviceDominoSpectrum_Count));

		SetParamValue(EDeviceParameterEuresysDomino_Spectrum, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_Spectrum);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_TapStructure, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoTapStructure, EDeviceDominoTapStructure_Count));

		SetParamValue(EDeviceParameterEuresysDomino_TapStructure, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_TapStructure);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_Scanning, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoScanning, EDeviceDominoScanning_Count));

		SetParamValue(EDeviceParameterEuresysDomino_Scanning, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_Scanning);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_Standard, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoStandard, EDeviceDominoStandard_Count));

		SetParamValue(EDeviceParameterEuresysDomino_Standard, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_Standard);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_AuxResetCtl, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoAuxResetCtl, EDeviceDominoAuxResetCtl_Count));

		SetParamValue(EDeviceParameterEuresysDomino_AuxResetCtl, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_AuxResetCtl);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_ColorMethod, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoColorMethod, EDeviceDominoColorMethod_Count));

		SetParamValue(EDeviceParameterEuresysDomino_ColorMethod, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_ColorMethod);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_ExposeOverlap, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoExposeOverlap, EDeviceDominoExposeOverlap_Count));

		SetParamValue(EDeviceParameterEuresysDomino_ExposeOverlap, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_ExposeOverlap);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_Readout, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoReadout, EDeviceDominoReadout_Count));

		SetParamValue(EDeviceParameterEuresysDomino_Readout, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_Readout);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_ResetCtl, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoResetCtl, EDeviceDominoResetCtl_Count));

		SetParamValue(EDeviceParameterEuresysDomino_ResetCtl, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_ResetCtl);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_ResetEdge, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoResetEdge, EDeviceDominoResetEdge_Count));

		SetParamValue(EDeviceParameterEuresysDomino_ResetEdge, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_ResetEdge);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_ACReset, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoACReset, EDeviceDominoACReset_Count));

		SetParamValue(EDeviceParameterEuresysDomino_ACReset, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_ACReset);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_ChannelTopology, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoChannelTopology, EDeviceDominoChannelTopology_Count));

		SetParamValue(EDeviceParameterEuresysDomino_ChannelTopology, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_ChannelTopology);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_HReset, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoHReset, EDeviceDominoHReset_Count));

		SetParamValue(EDeviceParameterEuresysDomino_HReset, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_HReset);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_HsyncCtl, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoHsyncCtl, EDeviceDominoHsyncCtl_Count));

		SetParamValue(EDeviceParameterEuresysDomino_HsyncCtl, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_HsyncCtl);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_HsyncEdge, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoHsyncEdge, EDeviceDominoHsyncEdge_Count));

		SetParamValue(EDeviceParameterEuresysDomino_HsyncEdge, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_HsyncEdge);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_HVdriveCtl, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoHVdriveCtl, EDeviceDominoHVdriveCtl_Count));

		SetParamValue(EDeviceParameterEuresysDomino_HVdriveCtl, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_HVdriveCtl);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_HVdriveEdge, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoHVdriveEdge, EDeviceDominoHVdriveEdge_Count));

		SetParamValue(EDeviceParameterEuresysDomino_HVdriveEdge, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_HVdriveEdge);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_PixelClkCtl, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoPixelClkCtl, EDeviceDominoPixelClkCtl_Count));

		SetParamValue(EDeviceParameterEuresysDomino_PixelClkCtl, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_PixelClkCtl);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_VCSync, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoVCSync, EDeviceDominoVCSync_Count));

		SetParamValue(EDeviceParameterEuresysDomino_VCSync, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_VCSync);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_VHSync, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoVHSync, EDeviceDominoVHSync_Count));

		SetParamValue(EDeviceParameterEuresysDomino_VHSync, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_VHSync);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_VsyncCtl, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoVsyncCtl, EDeviceDominoVsyncCtl_Count));

		SetParamValue(EDeviceParameterEuresysDomino_VsyncCtl, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_VsyncCtl);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_VsyncEdge, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoVsyncEdge, EDeviceDominoVsyncEdge_Count));

		SetParamValue(EDeviceParameterEuresysDomino_VsyncEdge, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_VsyncEdge);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_BreakEffect, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoBreakEffect, EDeviceDominoBreakEffect_Count));

		SetParamValue(EDeviceParameterEuresysDomino_BreakEffect, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_BreakEffect);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_EndTrigMode, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoEndTrigMode, EDeviceDominoEndTrigMode_Count));

		SetParamValue(EDeviceParameterEuresysDomino_EndTrigMode, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_EndTrigMode);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_NextTrigMode, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoNextTrigMode, EDeviceDominoNextTrigMode_Count));

		SetParamValue(EDeviceParameterEuresysDomino_NextTrigMode, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_NextTrigMode);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_AuxresetLine, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoAuxresetLine, EDeviceDominoAuxresetLine_Count));

		SetParamValue(EDeviceParameterEuresysDomino_AuxResetLine, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_AuxResetLine);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_Cable, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoCable, EDeviceDominoCable_Count));

		SetParamValue(EDeviceParameterEuresysDomino_Cable, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_Cable);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_CsyncLine, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoCsyncLine, EDeviceDominoCsyncLine_Count));

		SetParamValue(EDeviceParameterEuresysDomino_CsyncLine, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_CsyncLine);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_HdriveLine, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoHdriveLine, EDeviceDominoHdriveLine_Count));

		SetParamValue(EDeviceParameterEuresysDomino_HdriveLine, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_HdriveLine);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_HsyncLine, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoHsyncLine, EDeviceDominoHsyncLine_Count));

		SetParamValue(EDeviceParameterEuresysDomino_HsyncLine, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_HsyncLine);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_PixelClkLine, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoPixelClkLine, EDeviceDominoPixelClkLine_Count));

		SetParamValue(EDeviceParameterEuresysDomino_PixelClkLine, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_PixelClkLine);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_VdriveLine, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoVdriveLine, EDeviceDominoVdriveLine_Count));

		SetParamValue(EDeviceParameterEuresysDomino_VdriveLine, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_VdriveLine);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_VsyncLine, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoVsyncLine, EDeviceDominoVsyncLine_Count));

		SetParamValue(EDeviceParameterEuresysDomino_VsyncLine, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_VsyncLine);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_ResetLine, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoResetLine, EDeviceDominoResetLine_Count));

		SetParamValue(EDeviceParameterEuresysDomino_ResetLine, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_ResetLine);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_GrabField, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoGrabField, EDeviceDominoGrabField_Count));

		SetParamValue(EDeviceParameterEuresysDomino_GrabField, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_GrabField);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_NextGrabField, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoNextGrabField, EDeviceDominoNextGrabField_Count));

		SetParamValue(EDeviceParameterEuresysDomino_NextGrabField, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_NextGrabField);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_ForceTrig, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoForceTrig, EDeviceDominoForceTrig_Count));

		SetParamValue(EDeviceParameterEuresysDomino_ForceTrig, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_ForceTrig);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_StrobeCtl, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoStrobeCtl, EDeviceDominoStrobeCtl_Count));

		SetParamValue(EDeviceParameterEuresysDomino_StrobeCtl, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_StrobeCtl);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_StrobeMode, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoStrobeMode, EDeviceDominoStrobeMode_Count));

		SetParamValue(EDeviceParameterEuresysDomino_StrobeMode, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_StrobeMode);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_StrobeLine, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoStrobeLine, EDeviceDominoStrobeLine_Count));

		SetParamValue(EDeviceParameterEuresysDomino_StrobeLine, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_StrobeLine);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_StrobeLevel, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoStrobeLevel, EDeviceDominoStrobeLevel_Count));

		SetParamValue(EDeviceParameterEuresysDomino_StrobeLevel, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_StrobeLevel);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_ADresolution, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoADresolution, EDeviceDominoADresolution_Count));

		SetParamValue(EDeviceParameterEuresysDomino_ADresolution, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_ADresolution);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_VideoTerminator, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoVideoTerminator, EDeviceDominoVideoTerminator_Count));

		SetParamValue(EDeviceParameterEuresysDomino_VideoTerminator, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_VideoTerminator);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_GrabWindow, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoGrabWindow, EDeviceDominoGrabWindow_Count));

		SetParamValue(EDeviceParameterEuresysDomino_GrabWindow, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_GrabWindow);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_GainCtl, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoGainCtl, EDeviceDominoGainCtl_Count));

		SetParamValue(EDeviceParameterEuresysDomino_GainCtl, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_GainCtl);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_CalPat, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoCalPat, EDeviceDominoCalPat_Count));

		SetParamValue(EDeviceParameterEuresysDomino_CalPat, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_CalPat);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_BoardIdentifier, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		SetParamValue(EDeviceParameterEuresysDomino_BoardIdentifier, CString(strSaveDB));
		SaveSettings(EDeviceParameterEuresysDomino_BoardIdentifier);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_BoardName, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		SetParamValue(EDeviceParameterEuresysDomino_BoardName, CString(strSaveDB));
		SaveSettings(EDeviceParameterEuresysDomino_BoardName);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_ColorFormat, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoColorFormat, EDeviceDominoColorFormat_Count));

		SetParamValue(EDeviceParameterEuresysDomino_ColorFormat, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_ColorFormat);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_ImageFlipX, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoImageFlipX, EDeviceDominoImageFlipX_Count));

		SetParamValue(EDeviceParameterEuresysDomino_ImageFlipX, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_ImageFlipX);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_ImageFlipY, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoImageFlipY, EDeviceDominoImageFlipY_Count));

		SetParamValue(EDeviceParameterEuresysDomino_ImageFlipY, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_ImageFlipY);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_SurfaceAllocation, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoSurfaceAllocation, EDeviceDominoSurfaceAllocation_Count));

		SetParamValue(EDeviceParameterEuresysDomino_SurfaceAllocation, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_SurfaceAllocation);
	}
	
	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_ChannelState, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoChannelState, EDeviceDominoChannelState_Count));

		SetParamValue(EDeviceParameterEuresysDomino_ChannelState, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_ChannelState);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_AcquisitionCleanup, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoAcquisitionCleanup, EDeviceDominoAcquisitionCleanup_Count));

		SetParamValue(EDeviceParameterEuresysDomino_AcquisitionCleanup, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_AcquisitionCleanup);
	}

	memset(strSaveDB, 0, sizeof(strSaveDB));

	if(McGetParamStr(m_hDevice, MC_ParamsConsistencyCheck, strSaveDB, sizeof(strSaveDB)) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), GetStringToFeatureIndex(CString(strSaveDB), g_lpszDominoParamsConsistencyCheck, EDeviceDominoParamsConsistencyCheck_Count));

		SetParamValue(EDeviceParameterEuresysDomino_ParamsConsistencyCheck, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_ParamsConsistencyCheck);
	}
	
	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_Expose_us, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_Expose_us, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_Expose_us);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_SeqLength_Fr, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_SeqLength_Fr, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_SeqLength_Fr);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_PhaseLength_Fr, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_PhaseLength_Fr, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_PhaseLength_Fr);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_Gain, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_Gain, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_Gain);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_Offset, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_Offset, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_Offset);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_Vactive_Ln, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_Vactive_Ln, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_Vactive_Ln);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_VsyncAft_Ln, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_VsyncAft_Ln, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_VsyncAft_Ln);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_LineRate_Hz, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_LineRate_Hz, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_LineRate_Hz);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_ACResetPostDelay_us, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_ACResetPostDelay_us, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_ACResetPostDelay_us);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_ACResetWidth_us, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_ACResetWidth_us, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_ACResetWidth_us);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_HCSyncAft_ns, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_HCSyncAft_ns, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_HCSyncAft_ns);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_HCsyncBfr_ns, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_HCsyncBfr_ns, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_HCsyncBfr_ns);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_HCsyncDur_ns, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_HCsyncDur_ns, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_HCsyncDur_ns);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_HsyncDly_ns, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_HsyncDly_ns, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_HsyncDly_ns);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_HdriveDur_ns, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_HdriveDur_ns, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_HdriveDur_ns);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_HdriveDly_ns, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_HdriveDly_ns, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_HdriveDly_ns);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_Hactive_ns, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_Hactive_ns, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_Hactive_ns);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_PixelClk_Hz, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_PixelClk_Hz, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_PixelClk_Hz);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_VCsyncAft_Ln, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_VCsyncAft_Ln, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_VCsyncAft_Ln);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_VCgatePos_Ln, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_VCgatePos_Ln, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_VCgatePos_Ln);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_Vtotal_Ln, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_Vtotal_Ln, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_Vtotal_Ln);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_VsyncBfrEndExp_us, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_VsyncBfrEndExp_us, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_VsyncBfrEndExp_us);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_VdriveDur_Ln, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_VdriveDur_Ln, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_VdriveDur_Ln);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_VdriveDly_Ln, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_VdriveDly_Ln, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_VdriveDly_Ln);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_ExposeMin_us, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_ExposeMin_us, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_ExposeMin_us);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_ExposeMax_us, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_ExposeMax_us, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_ExposeMax_us);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_CameraSignalSet, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_CameraSignalSet, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_CameraSignalSet);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_ResetDur, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_ResetDur, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_ResetDur);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_ResetPos, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_ResetPos, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_ResetPos);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_CableTopology, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_CableTopology, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_CableTopology);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_ActivityLength, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_ActivityLength, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_ActivityLength);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_TrigDelay_us, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_TrigDelay_us, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_TrigDelay_us);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_TrueExp_us, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_TrueExp_us, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_TrueExp_us);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_ExposeTrim, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_ExposeTrim, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_ExposeTrim);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_StrobeDur, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_StrobeDur, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_StrobeDur);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_StrobePos, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_StrobePos, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_StrobePos);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_PreStrobe_us, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_PreStrobe_us, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_PreStrobe_us);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_OffsetX_Px, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_OffsetX_Px, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_OffsetX_Px);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_OffsetY_Ln, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_OffsetY_Ln, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_OffsetY_Ln);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_SampleTrim, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_SampleTrim, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_SampleTrim);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_SampleClk_Hz, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_SampleClk_Hz, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_SampleClk_Hz);
	}
	
	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_InputLut, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_InputLut, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_InputLut);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_LutIndex, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_LutIndex, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_LutIndex);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_DriverIndex, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_DriverIndex, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_DriverIndex);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_PciPosition, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_PciPosition, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_PciPosition);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_ImageSizeX, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_ImageSizeX, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_ImageSizeX);
	}

	nSaveDB = -1;

	if(McGetParamInt(m_hDevice, MC_AcqTimeout_ms, &nSaveDB) == MC_OK)
	{
		CString strSave;
		strSave.Format(_T("%d"), nSaveDB);

		SetParamValue(EDeviceParameterEuresysDomino_AcqTimeout_ms, strSave);
		SaveSettings(EDeviceParameterEuresysDomino_AcqTimeout_ms);
	}
}

#endif