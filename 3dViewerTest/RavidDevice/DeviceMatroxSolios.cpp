#include "stdafx.h"

#include "DeviceMatroxSolios.h"

#ifdef USING_DEVICE

#include "../RavidCore/RavidRect.h"
#include "../RavidCore/RavidImage.h"
#include "../RavidCore/RavidDatabase.h"
#include "../RavidCore/RavidRecordset.h"
#include "../RavidCore/PerformanceCounter.h"

#include "../RavidFramework/UIManager.h"
#include "../RavidFramework/LogManager.h"
#include "../RavidFramework/DeviceManager.h"
#include "../RavidFramework/FrameworkManager.h"
#include "../RavidFramework/EventHandlerManager.h"
#include "../RavidFramework/MultiLanguageManager.h"

#include "../RavidFramework/RavidImageView.h"

#include "../Libraries/Includes/MatroxIntellicam/Mil.h"
#include "../Libraries/Includes/MatroxIntellicam/Milproto.h"

// mil.dll
#pragma comment(lib, COMMONLIB_PREFIX "MatroxIntellicam/mil.lib")

using namespace Ravid;
using namespace Ravid::Device;
using namespace Ravid::Database;
using namespace Ravid::Framework;
using namespace Ravid::Algorithms;
using namespace Ravid::Miscellaneous;

enum EMatoxDCF
{
	EMatoxDCF_EOFCamera_Name = 1,
	EMatoxDCF_EOFConfig_File = 4,
	EMatoxDCF_EOFInfo_File_Rev = 3,
	EMatoxDCF_EOFGeneral_Parameters = 654,
	EMatoxDCF_EOFExp_Trigger = 847,
	EMatoxDCF_EOFDef_Board = 254,
	EMatoxDCF_EOFReg_Digit = 299,
	EMatoxDCF_EOFReg_Modify_state = 301,
};

IMPLEMENT_DYNAMIC(CDeviceMatroxSolios, CDeviceFrameGrabber)

BEGIN_MESSAGE_MAP(CDeviceMatroxSolios, CDeviceFrameGrabber)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


static LPCTSTR g_lpszCamera_Name[EMatoxDCF_EOFCamera_Name] =
{
	_T("[CAMERA_NAME]"),
};

static LPCTSTR g_lpszConfig_File[EMatoxDCF_EOFConfig_File] =
{
	_T("[CONFIG_FILE]"),
	_T("50CF"),
	_T("ODYSSEY"),
	_T("Mon Jan 01 00:00:00 2018"),
};

static LPCTSTR g_lpszInfo_File_Rev[EMatoxDCF_EOFInfo_File_Rev] =
{
	_T("[INFO_FILE_REV]"),
	_T("0010.0001.0000"),
	_T("SOLIOS/CL/FULL/EVCL"),
};

static LPCTSTR g_lpszGeneral_Parameters[EMatoxDCF_EOFGeneral_Parameters] =
{
	_T("[GENERAL_PARAMETERS]"),
	_T("GEN_MATCH_HW                                             0x1"),
	_T("GEN_SAVED_W_ERR                                          0x0"),
	_T("CT_LS                                                    0x%x"),
	_T("CT_FS                                                    0x%x"),
	_T("CT_CONV_INVERTED                                         0x%x"),
	_T("CT_TAPS                                                  0x%x"),
	_T("CT_CAMERA                                                0x%x"),
	_T("CT_BAYER_DISABLE                                         0x%x"),
	_T("CT_BAYER_BG                                              0x%x"),
	_T("CT_BAYER_GB                                              0x%x"),
	_T("CT_BAYER_GR                                              0x%x"),
	_T("CT_BAYER_RG                                              0x%x"),
	_T("VDC_DIG                                                  0x%x"),
	_T("VDC_ANA                                                  0x%x"),
	_T("VDC_MONO                                                 0x%x"),
	_T("VDC_C_COLOR                                              0x%x"),
	_T("VDC_RGB_COL                                              0x%x"),
	_T("VDC_RGB_PACK                                             0x%x"),
	_T("VDC_RGB_ALPHA                                            0x%x"),
	_T("VDC_SVID                                                 0x%x"),
	_T("VDC_YUVVID                                               0x%x"),
	_T("VDC_TTL                                                  0x%x"),
	_T("VDC_422                                                  0x%x"),
	_T("VDC_OPTO                                                 0x%x"),
	_T("VDC_LVDS                                                 0x%x"),
	_T("VDC_WD8                                                  0x%x"),
	_T("VDC_WD16                                                 0x%x"),
	_T("VDC_WD24                                                 0x%x"),
	_T("VDC_WD32                                                 0x%x"),
	_T("VDC_WD64                                                 0x%x"),
	_T("VDC_ALT_GRAB                                             0x%x"),
	_T("VDC_FROM_VCR                                             0x%x"),
	_T("VDC_IN_CH0                                               0x%x"),
	_T("VDC_IN_CH1                                               0x%x"),
	_T("VDC_IN_CH2                                               0x%x"),
	_T("VDC_IN_CH3                                               0x%x"),
	_T("VDC_IN_CH_C                                              0x0"),
	_T("VDC_DIGITIZER                                            0x0"),
	_T("VDC_PSG_MODE_1_CHECK                                     0x1"),
	_T("VDC_PSG_MODE_2_CHECKS                                    0x0"),
	_T("VDC_PSG_MODE_3_CHECKS                                    0x0"),
	_T("VDC_PSG_MODE_4_CHECKS                                    0x0"),
	_T("VDC_PSG_MODE_1_3_CHECKS                                  0x0"),
	_T("VDC_PSG_MODE_ANY_CHECKS                                  0x0"),
	_T("VDC_MIL_CHANNEL                                          0x0"),
	_T("VDC_USE_PSG_0                                            0x1"),
	_T("VDC_USE_PSG_1                                            0x0"),
	_T("VDC_USE_PSG_2                                            0x0"),
	_T("VDC_USE_PSG_3                                            0x0"),
	_T("VDC_0_AC_WITH_DC                                         0x0"),
	_T("VDC_0_DC_WITH_DC                                         0x0"),
	_T("VDC_0_DC_WITHOUT_DC                                      0x0"),
	_T("VDC_0_NO_FILTER                                          0x0"),
	_T("VDC_0_FILTER_0                                           0x0"),
	_T("VDC_0_FILTER_1                                           0x0"),
	_T("VDC_1_AC_WITH_DC                                         0x0"),
	_T("VDC_1_DC_WITH_DC                                         0x0"),
	_T("VDC_1_DC_WITHOUT_DC                                      0x0"),
	_T("VDC_1_NO_FILTER                                          0x0"),
	_T("VDC_1_FILTER_0                                           0x0"),
	_T("VDC_1_FILTER_1                                           0x0"),
	_T("VDC_2_AC_WITH_DC                                         0x0"),
	_T("VDC_2_DC_WITH_DC                                         0x0"),
	_T("VDC_2_DC_WITHOUT_DC                                      0x0"),
	_T("VDC_2_NO_FILTER                                          0x0"),
	_T("VDC_2_FILTER_0                                           0x0"),
	_T("VDC_2_FILTER_1                                           0x0"),
	_T("VDC_3_AC_WITH_DC                                         0x0"),
	_T("VDC_3_DC_WITH_DC                                         0x0"),
	_T("VDC_3_DC_WITHOUT_DC                                      0x0"),
	_T("VDC_3_NO_FILTER                                          0x0"),
	_T("VDC_3_FILTER_0                                           0x0"),
	_T("VDC_3_FILTER_1                                           0x0"),
	_T("VDT_USE_HLOCK                                            0x%x"),
	_T("VDT_USE_VLOCK                                            0x%x"),
	_T("VDT_STD_170                                              0x0"),
	_T("VDT_STD_330                                              0x0"),
	_T("VDT_STD_CCIR                                             0x0"),
	_T("VDT_STD_NTSC                                             0x0"),
	_T("VDT_STD_PAL                                              0x0"),
	_T("VDT_STD_CL                                               0x0"),
	_T("VDT_STD_DIGITAL                                          0x0"),
	_T("VDT_NOVERT                                               0x%x"),
	_T("VDT_HSYNC                                                0x%x"),
	_T("VDT_HBPORCH                                              0x%x"),
	_T("VDT_HFPORCH                                              0x%x"),
	_T("VDT_HACTIVE                                              0x%x"),
	_T("VDT_HTOTAL                                               0x%x"),
	_T("VDT_HSYNC_FREQ                                           0x%x"),
	_T("VDT_VSYNC                                                0x%x"),
	_T("VDT_VBPORCH                                              0x%x"),
	_T("VDT_VFPORCH                                              0x%x"),
	_T("VDT_VACTIVE                                              0x%x"),
	_T("VDT_VTOTAL                                               0x%x"),
	_T("VDT_VSYNC_FREQ                                           0x%x"),
	_T("VDT_CL_IMAGE_SIZE_X                                      0x0"),
	_T("VDT_CL_IMAGE_SIZE_Y                                      0x0"),
	_T("VDT_CL_CROPPING_X                                        0x0"),
	_T("VDT_CL_CROPPING_Y                                        0x0"),
	_T("VDT_INTERL                                               0x%x"),
	_T("VDT_NINTRL                                               0x%x"),
	_T("VDT_SER                                                  0x0"),
	_T("VDT_EQU                                                  0x0"),
	_T("VDT_CLP_SYN                                              0x0"),
	_T("VDT_CLP_BPO                                              0x0"),
	_T("VDT_CLP_FPO                                              0x0"),
	_T("PCK_CAM_GEN                                              0x%x"),
	_T("PCK_CAM_REC                                              0x%x"),
	_T("PCK_CAM_R&G                                              0x%x"),
	_T("PCK_OTH_REC                                              0x%x"),
	_T("PCK_USE_OUT                                              0x%x"),
	_T("PCK_CAM_XCHG                                             0x%x"),
	_T("PCK_ITTL                                                 0x%x"),
	_T("PCK_I422                                                 0x%x"),
	_T("PCK_IOPTO                                                0x%x"),
	_T("PCK_ILVDS                                                0x%x"),
	_T("PCK_IPOS                                                 0x%x"),
	_T("PCK_INEG                                                 0x%x"),
	_T("PCK_FREQ                                                 0x%x"),
	_T("PCK_INTDVED                                              0x%x"),
	_T("PCK_INTDIVF                                              0x%x"),
	_T("PCK_ODVED                                                0x%x"),
	_T("PCK_ODIVF                                                0x%x"),
	_T("PCK_OFREQDV                                              0x%x"),
	_T("PCK_OTTL                                                 0x%x"),
	_T("PCK_O422                                                 0x%x"),
	_T("PCK_OOPTO                                                0x%x"),
	_T("PCK_OLVDS                                                0x%x"),
	_T("PCK_OPOS                                                 0x%x"),
	_T("PCK_ONEG                                                 0x%x"),
	_T("PCK_IDELAY                                               0x%x"),
	_T("SYC_DIG                                                  0x%x"),
	_T("SYC_ANA                                                  0x%x"),
	_T("SYC_CAM_GEN                                              0x%x"),
	_T("SYC_CAM_R&G                                              0x%x"),
	_T("SYC_CAM_LATENCY                                          0x%x"),
	_T("SYC_MD_CSYN                                              0x%x"),
	_T("SYC_MD_HVSY                                              0x%x"),
	_T("SYC_MD_VSYN                                              0x%x"),
	_T("SYC_MD_HSYN                                              0x%x"),
	_T("SYC_EXT_VSY                                              0x%x"),
	_T("SYC_H_IN                                                 0x%x"),
	_T("SYC_H_OUT                                                0x%x"),
	_T("SYC_H_ITTL                                               0x%x"),
	_T("SYC_H_I422                                               0x%x"),
	_T("SYC_H_IOPTO                                              0x%x"),
	_T("SYC_H_ILVDS                                              0x%x"),
	_T("SYC_H_IPOS                                               0x%x"),
	_T("SYC_H_INEG                                               0x%x"),
	_T("SYC_H_OTTL                                               0x%x"),
	_T("SYC_H_O422                                               0x%x"),
	_T("SYC_H_OOPTO                                              0x%x"),
	_T("SYC_H_OLVDS                                              0x%x"),
	_T("SYC_H_OPOS                                               0x%x"),
	_T("SYC_H_ONEG                                               0x%x"),
	_T("SYC_V_IN                                                 0x%x"),
	_T("SYC_V_OUT                                                0x%x"),
	_T("SYC_V_ITTL                                               0x%x"),
	_T("SYC_V_I422                                               0x%x"),
	_T("SYC_V_IOPTO                                              0x%x"),
	_T("SYC_V_ILVDS                                              0x%x"),
	_T("SYC_V_IPOS                                               0x%x"),
	_T("SYC_V_INEG                                               0x%x"),
	_T("SYC_V_OTTL                                               0x%x"),
	_T("SYC_V_O422                                               0x%x"),
	_T("SYC_V_OOPTO                                              0x%x"),
	_T("SYC_V_OLVDS                                              0x%x"),
	_T("SYC_V_OPOS                                               0x%x"),
	_T("SYC_V_ONEG                                               0x%x"),
	_T("SYC_C_IN                                                 0x%x"),
	_T("SYC_C_OUT                                                0x%x"),
	_T("SYC_C_ITTL                                               0x%x"),
	_T("SYC_C_I422                                               0x%x"),
	_T("SYC_C_IOPTO                                              0x%x"),
	_T("SYC_C_ILVDS                                              0x%x"),
	_T("SYC_C_IPOS                                               0x%x"),
	_T("SYC_C_INEG                                               0x%x"),
	_T("SYC_C_OTTL                                               0x%x"),
	_T("SYC_C_O422                                               0x%x"),
	_T("SYC_C_OOPTO                                              0x%x"),
	_T("SYC_C_OLVDS                                              0x%x"),
	_T("SYC_C_OPOS                                               0x%x"),
	_T("SYC_C_ONEG                                               0x%x"),
	_T("SYC_BLK                                                  0x0"),
	_T("SYC_COMP                                                 0x0"),
	_T("SYC_SEP                                                  0x0"),
	_T("SYC_IN_CH                                                0x0"),
	_T("EXP_SYN_CLK                                              0x%x"),
	_T("EXP_ASY_CLK                                              0x%x"),
	_T("EXP_CLK_FREQ                                             0x%x"),
	_T("EXP_CLK_DVED                                             0x%x"),
	_T("EXP_CLK_DIVF                                             0x%x"),
	_T("EXP_MD_PERD                                              0x%x"),
	_T("EXP_MD_W_TRG                                             0x%x"),
	_T("EXP_MD_EXT                                               0x%x"),
	_T("EXP_MD_HSY                                               0x%x"),
	_T("EXP_MD_VSY                                               0x%x"),
	_T("EXP_MD_SW                                                0x%x"),
	_T("EXP_TRG_TTL                                              0x%x"),
	_T("EXP_TRG_422                                              0x%x"),
	_T("EXP_TRG_OPTO                                             0x%x"),
	_T("EXP_TRG_LVDS                                             0x%x"),
	_T("EXP_TRG_DEFAULT                                          0x%x"),
	_T("EXP_TRG_POS                                              0x%x"),
	_T("EXP_TRG_NEG                                              0x%x"),
	_T("EXP_OUT_DLYD                                             0x%x"),
	_T("EXP_OUT_T0                                               0x%x"),
	_T("EXP_OUT_T1                                               0x%x"),
	_T("EXP_OUT_T2                                               0x%x"),
	_T("EXP_OUT_T3                                               0x%x"),
	_T("EXP_OUT_TTL                                              0x%x"),
	_T("EXP_OUT_422                                              0x%x"),
	_T("EXP_OUT_OPTO                                             0x%x"),
	_T("EXP_OUT_LVDS                                             0x%x"),
	_T("EXP_OUT_DEFAULT                                          0x%x"),
	_T("EXP_OUT_POS                                              0x%x"),
	_T("EXP_OUT_NEG                                              0x%x"),
	_T("EXP_ARM_ENABLE                                           0x%x"),
	_T("EXP_ARM_DISABLE                                          0x%x"),
	_T("EXP_ARM_TTL                                              0x%x"),
	_T("EXP_ARM_422                                              0x%x"),
	_T("EXP_ARM_OPTO                                             0x%x"),
	_T("EXP_ARM_LVDS                                             0x%x"),
	_T("EXP_ARM_DEFAULT                                          0x%x"),
	_T("EXP_ARM_POS                                              0x%x"),
	_T("EXP_ARM_NEG                                              0x%x"),
	_T("GRB_MD_CONT                                              0x%x"),
	_T("GRB_MD_SW_TRG                                            0x%x"),
	_T("GRB_MD_HW_TRG                                            0x%x"),
	_T("GRB_START_ODD                                            0x%x"),
	_T("GRB_START_EVEN                                           0x%x"),
	_T("GRB_START_ANY                                            0x%x"),
	_T("GRB_ACT_NXT_FRM                                          0x%x"),
	_T("GRB_ACT_IMMEDIATE                                        0x%x"),
	_T("GRB_ACT_IMM_SKP_NFR                                      0x%x"),
	_T("GRB_TRG_TTL                                              0x%x"),
	_T("GRB_TRG_422                                              0x%x"),
	_T("GRB_TRG_OPTO                                             0x%x"),
	_T("GRB_TRG_LVDS                                             0x%x"),
	_T("GRB_TRG_DEFAULT                                          0x%x"),
	_T("GRB_TRG_POS                                              0x%x"),
	_T("GRB_TRG_NEG                                              0x%x"),
	_T("GRB_LS_FREE_RUN                                          0x%x"),
	_T("GRB_LS_FIXED_LINE                                        0x%x"),
	_T("GRB_LS_VARIABLE_LINE                                     0x%x"),
	_T("GRB_LS_FRMFIX_LINEFIX                                    0x%x"),
	_T("GRB_LS_FRMFIX_LINEVAR                                    0x%x"),
	_T("GRB_LS_FRMVAR_LINEFIX                                    0x%x"),
	_T("GRB_LS_FRMVAR_LINEVAR                                    0x%x"),
	_T("GRB_TRG_ARM_TTL                                          0x%x"),
	_T("GRB_TRG_ARM_422                                          0x%x"),
	_T("GRB_TRG_ARM_OPTO                                         0x%x"),
	_T("GRB_TRG_ARM_LVDS                                         0x%x"),
	_T("GRB_TRG_ARM_DEFAULT                                      0x%x"),
	_T("GRB_TRG_ARM_POS                                          0x%x"),
	_T("GRB_TRG_ARM_NEG                                          0x%x"),
	_T("VDL_USE_DEFVAL                                           0x1"),
	_T("VDL_POS_SWG                                              0x1"),
	_T("VDL_NEG_SWG                                              0x0"),
	_T("VDL_BTH_SWG                                              0x0"),
	_T("VDL_AMPL                                                 0x2bc"),
	_T("VDL_PEDEST                                               0x0"),
	_T("VDL_PED_AMP                                              0x32"),
	_T("VDL_GAIN_IND                                             0x2"),
	_T("VDL_GAIN                                                 0xaf0"),
	_T("VDL_BRGHT                                                0x32"),
	_T("VDL_CONTR                                                0x32"),
	_T("VDL_SATUR                                                0x32"),
	_T("VDL_HUE                                                  0x32"),
	_T("DCF_IS_VIRTUAL                                           0x0"),
	_T("DAT_INFOFILE_REV_MAJOR                                   0xa"),
	_T("DAT_INFOFILE_REV_MINOR                                   0x1"),
	_T("DAT_INFOFILE_REV_BUILD                                   0x0"),
	_T("EXP_SYN_CLK_2                                            0x%x"),
	_T("EXP_ASY_CLK_2                                            0x%x"),
	_T("EXP_CLK_FREQ_2                                           0x%x"),
	_T("EXP_CLK_DVED_2                                           0x%x"),
	_T("EXP_CLK_DIVF_2                                           0x%x"),
	_T("EXP_MD_PERD_2                                            0x%x"),
	_T("EXP_MD_W_TRG_2                                           0x%x"),
	_T("EXP_MD_EXT_2                                             0x%x"),
	_T("EXP_MD_HSY_2                                             0x%x"),
	_T("EXP_MD_VSY_2                                             0x%x"),
	_T("EXP_MD_SW_2                                              0x%x"),
	_T("EXP_TRG_TTL_2                                            0x%x"),
	_T("EXP_TRG_422_2                                            0x%x"),
	_T("EXP_TRG_OPTO_2                                           0x%x"),
	_T("EXP_TRG_LVDS_2                                           0x%x"),
	_T("EXP_TRG_DEFAULT_2                                        0x%x"),
	_T("EXP_TRG_POS_2                                            0x%x"),
	_T("EXP_TRG_NEG_2                                            0x%x"),
	_T("EXP_OUT_DLYD_2                                           0x%x"),
	_T("EXP_OUT_T0_2                                             0x%x"),
	_T("EXP_OUT_T1_2                                             0x%x"),
	_T("EXP_OUT_T2_2                                             0x%x"),
	_T("EXP_OUT_T3_2                                             0x%x"),
	_T("EXP_OUT_TTL_2                                            0x%x"),
	_T("EXP_OUT_422_2                                            0x%x"),
	_T("EXP_OUT_OPTO_2                                           0x%x"),
	_T("EXP_OUT_LVDS_2                                           0x%x"),
	_T("EXP_OUT_DEFAULT_2                                        0x%x"),
	_T("EXP_OUT_POS_2                                            0x%x"),
	_T("EXP_OUT_NEG_2                                            0x%x"),
	_T("EXP_ARM_ENABLE_2                                         0x%x"),
	_T("EXP_ARM_DISABLE_2                                        0x%x"),
	_T("EXP_ARM_TTL_2                                            0x%x"),
	_T("EXP_ARM_422_2                                            0x%x"),
	_T("EXP_ARM_OPTO_2                                           0x%x"),
	_T("EXP_ARM_LVDS_2                                           0x%x"),
	_T("EXP_ARM_DEFAULT_2                                        0x%x"),
	_T("EXP_ARM_POS_2                                            0x%x"),
	_T("EXP_ARM_NEG_2                                            0x%x"),
	_T("TAP_MULTIPLEX_X                                          0x1"),
	_T("TAP_MULTIPLEX_Y                                          0x1"),
	_T("TAP_ORDERS                                               0x343efcea"),
	_T("REGION_DIRECTIONS                                        0x%x"),
	_T("TAP_REGIONSX                                             0x%x"),
	_T("TAP_REGIONSY                                             0x%x"),
	_T("TAP_PIXADJX                                              0x%x"),
	_T("TAP_PIXADJY                                              0x%x"),
	_T("CLC_MODE                                                 0x%x"),
	_T("CLC_MODE_INIT_WIDTH_10_16                                0x%x"),
	_T("CLC_ACTIVE_CH0                                           0x1"),
	_T("VDT_CL_USE_CAMERA_VALID                                  0x0"),
	_T("CL_MODE_BITMAP                                           0x%x"),
	_T("CLC_MODE_CH0                                             0x%x"),
	_T("CLC_SYNC_SOURCE                                          0x0"),
	_T("CLC_VSYNC_SEL                                            0x0"),
	_T("CLC_VSYNC_POL                                            0x0"),
	_T("CLC_HSYNC_SEL                                            0x0"),
	_T("CLC_HSYNC_POL                                            0x0"),
	_T("CLB_CC1                                                  0x%x"),
	_T("CLB_CC2                                                  0x%x"),
	_T("CLB_CC3                                                  0x%x"),
	_T("CLB_CC4                                                  0x%x"),
	_T("CLB_CCOUTEN1                                             0x%x"),
	_T("CLB_CCOUTEN2                                             0x%x"),
	_T("USR_IENABLE                                              0x%x"),
	_T("USR_ITTL                                                 0x%x"),
	_T("USR_ILVDS                                                0x%x"),
	_T("USR_IOPTO                                                0x%x"),
	_T("USR_OENABLE                                              0x%x"),
	_T("USR_OTTL                                                 0x%x"),
	_T("USR_OLVDS                                                0x%x"),
	_T("USR_BIT_0_OTH0                                           0x%x"),
	_T("USR_BIT_1_OTH0                                           0x%x"),
	_T("USR_BIT_2_OTH0                                           0x%x"),
	_T("USR_BIT_3_OTH0                                           0x%x"),
	_T("USR_BIT_4_OTH0                                           0x%x"),
	_T("USR_BIT_5_OTH0                                           0x%x"),
	_T("USR_BIT_6_OTH0                                           0x%x"),
	_T("TM_ENABLE                                                0x%x"),
	_T("TM_LINENUMBER                                            0x%x"),
	_T("TM_PIXELMODE                                             0x%x"),
	_T("VDC_VID_WIDTH_10                                         0x%x"),
	_T("VDC_VID_WIDTH_12                                         0x%x"),
	_T("VDC_VID_WIDTH_14                                         0x%x"),
	_T("VDC_VID_WIDTH_16                                         0x%x"),
	_T("VDC_ODYSSEY_ANA_FILTER                                   0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_OPTO                                 0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_TTL                                  0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_LVDS                                 0x%x"),
	_T("VDT_HCNT_LD_ARM_DIS                                      0x%x"),
	_T("VDT_HCNT_LD_ARM_QUADTIMERSTART                           0x%x"),
	_T("VDT_HCNT_LD_ARM_TIMER0                                   0x%x"),
	_T("VDT_HCNT_LD_ARM_TIMER1                                   0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_2_AC01_OPTO_CL                       0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_3_AC01_OPTO_CL                       0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_0_AC0_OPTO_CL_SOL_BASE               0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_1_AC0_OPTO_CL_SOL_BASE               0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_0_AC1_OPTO_CL                        0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_1_AC1_OPTO_CL                        0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_0_AC0_TTL_CL_SOL_BASE                0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_1_AC0_TTL_CL_SOL_BASE                0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_2_AC01_TTL_CL                        0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_3_AC01_TTL_CL                        0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_2_AC01_LVDS_CL                       0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_3_AC01_LVDS_CL                       0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_0_AC0_LVDS_CL_SOL_BASE               0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_1_AC0_LVDS_CL_SOL_BASE               0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_0_AC1_LVDS_CL                        0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_1_AC1_LVDS_CL                        0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_0_AC0_OPTO_CL                        0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_1_AC0_OPTO_CL                        0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_0_AC0_TTL_CL                         0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_1_AC0_TTL_CL                         0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_0_AC1_TTL_CL                         0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_1_AC1_TTL_CL                         0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_0_AC0_LVDS_CL                        0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_1_AC0_LVDS_CL                        0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_1_AC0_OPTO_ANA                       0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_1_AC1_OPTO_ANA                       0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_1_AC2_OPTO_ANA                       0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_1_AC3_OPTO_ANA                       0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_0_AC0_TTL_ANA                        0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_0_AC1_TTL_ANA                        0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_0_AC2_TTL_ANA                        0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_0_AC3_TTL_ANA                        0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_2_4AC_AUX0_TTL_ANA                   0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_3_4AC_AUX1_TTL_ANA                   0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_2_4AC_AUX2_TTL_ANA                   0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_3_4AC_AUX3_TTL_ANA                   0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_2_4AC_AUX4_TTL_ANA                   0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_3_4AC_AUX5_TTL_ANA                   0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_2_4AC_AUX6_TTL_ANA                   0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_3_4AC_AUX7_TTL_ANA                   0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_2_4AC_AUX0_LVDS_ANA                  0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_3_4AC_AUX1_LVDS_ANA                  0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_2_4AC_AUX2_LVDS_ANA                  0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_3_4AC_AUX3_LVDS_ANA                  0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_2_4AC_AUX4_LVDS_ANA                  0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_3_4AC_AUX5_LVDS_ANA                  0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_2_4AC_AUX6_LVDS_ANA                  0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_3_4AC_AUX7_LVDS_ANA                  0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_0_AC0_OPTO_DIG                       0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_1_AC0_OPTO_DIG                       0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_0_AC1_OPTO_DIG                       0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_1_AC1_OPTO_DIG                       0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_0_AC2_OPTO_DIG                       0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_1_AC2_OPTO_DIG                       0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_0_AC3_OPTO_DIG                       0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_1_AC3_OPTO_DIG                       0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_1_AC0_TTL_DIG                        0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_1_AC1_TTL_DIG                        0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_1_AC2_TTL_DIG                        0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_1_AC3_TTL_DIG                        0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_0_AC0_LVDS_DIG                       0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_0_AC1_LVDS_DIG                       0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_0_AC2_LVDS_DIG                       0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_0_AC3_LVDS_DIG                       0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_2_AC0_AUX1_TTL_DIG                   0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_3_AC0_AUX2_TTL_DIG                   0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_2_AC1_AUX1_TTL_DIG                   0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_3_AC1_AUX2_TTL_DIG                   0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_2_AC2_AUX1_TTL_DIG                   0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_3_AC2_AUX2_TTL_DIG                   0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_2_AC3_AUX1_TTL_DIG                   0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_3_AC3_AUX2_TTL_DIG                   0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_2_AC0_AUX1_LVDS_DIG                  0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_3_AC0_AUX2_LVDS_DIG                  0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_2_AC1_AUX1_LVDS_DIG                  0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_3_AC1_AUX2_LVDS_DIG                  0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_2_AC2_AUX1_LVDS_DIG                  0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_3_AC2_AUX2_LVDS_DIG                  0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_2_AC3_AUX1_LVDS_DIG                  0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_3_AC3_AUX2_LVDS_DIG                  0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_AC0_LVDS_CL_SOL_BASE_ROTARY_ENCODER_FOW  0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_AC0_LVDS_CL_SOL_BASE_ROTARY_ENCODER_REV  0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_AC0_LVDS_CL_ROTARY_ENCODER_FOW       0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_AC0_LVDS_CL_ROTARY_ENCODER_REV       0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_AC1_LVDS_CL_ROTARY_ENCODER_FOW       0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_AC1_LVDS_CL_ROTARY_ENCODER_REV       0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_AC0_LVDS_ANA_ROTARY_ENCODER_FOW      0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_AC0_LVDS_ANA_ROTARY_ENCODER_REV      0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_AC1_LVDS_ANA_ROTARY_ENCODER_FOW      0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_AC1_LVDS_ANA_ROTARY_ENCODER_REV      0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_AC2_LVDS_ANA_ROTARY_ENCODER_FOW      0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_AC2_LVDS_ANA_ROTARY_ENCODER_REV      0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_AC3_LVDS_ANA_ROTARY_ENCODER_FOW      0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_AC3_LVDS_ANA_ROTARY_ENCODER_REV      0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_AC0_LVDS_DIG_ROTARY_ENCODER_FOW      0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_AC0_LVDS_DIG_ROTARY_ENCODER_REV      0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_AC1_LVDS_DIG_ROTARY_ENCODER_FOW      0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_AC1_LVDS_DIG_ROTARY_ENCODER_REV      0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_AC2_LVDS_DIG_ROTARY_ENCODER_FOW      0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_AC2_LVDS_DIG_ROTARY_ENCODER_REV      0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_AC3_LVDS_DIG_ROTARY_ENCODER_FOW      0x%x"),
	_T("VDT_HCNT_LD_ARM_TRG_AC3_LVDS_DIG_ROTARY_ENCODER_REV      0x%x"),
	_T("SLOW_SCAN_PLL_HREFSEL_DEFAULT                            0x%x"),
	_T("SLOW_SCAN_PLL_HREFSEL_NONE                               0x%x"),
	_T("SLOW_SCAN_PLL_HREFSEL_DVI                                0x%x"),
	_T("SLOW_SCAN_PLL_HREFSEL_AUX                                0x%x"),
	_T("SLOW_SCAN_PLL_HREFSEL_VIDEO                              0x%x"),
	_T("SLOW_SCAN_PLL_HREFSEL_DVI_OTHERAC                        0x%x"),
	_T("SLOW_SCAN_PLL_HREFSEL_DVI_AC1                            0x%x"),
	_T("SLOW_SCAN_PLL_PCKSEL_DEFAULT                             0x%x"),
	_T("SLOW_SCAN_PLL_PCKSEL_NONE                                0x%x"),
	_T("SLOW_SCAN_PLL_PCKSEL_DVI                                 0x%x"),
	_T("SLOW_SCAN_PLL_PCKSEL_AUX                                 0x%x"),
	_T("SLOW_SCAN_PLL_PCKSEL_VIDEO                               0x%x"),
	_T("SLOW_SCAN_PLL_PCKSEL_DVI_OTHERAC                         0x%x"),
	_T("SLOW_SCAN_PLL_PCKSEL_DVI_AC1                             0x%x"),
	_T("GRB_TRG_HS_PSG                                           0x%x"),
	_T("GRB_TRG_VS_PSG                                           0x%x"),
	_T("GRB_TRG_TIMER0                                           0x%x"),
	_T("GRB_TRG_TIMER1                                           0x%x"),
	_T("GRB_TRG_TIMER2                                           0x%x"),
	_T("GRB_TRG_TIMER3                                           0x%x"),
	_T("GRB_TRG_2_AC01_OPTO_CL_SOL_EVCL                          0x%x"),
	_T("GRB_TRG_3_AC01_OPTO_CL_SOL_EVCL                          0x%x"),
	_T("GRB_TRG_2_AC01_OPTO_CL                                   0x%x"),
	_T("GRB_TRG_3_AC01_OPTO_CL                                   0x%x"),
	_T("GRB_TRG_0_AC0_OPTO_CL_SOL_BASE                           0x%x"),
	_T("GRB_TRG_1_AC0_OPTO_CL_SOL_BASE                           0x%x"),
	_T("GRB_TRG_0_AC0_OPTO_CL_SOL_EVCL                           0x%x"),
	_T("GRB_TRG_1_AC0_OPTO_CL_SOL_EVCL                           0x%x"),
	_T("GRB_TRG_0_AC0_OPTO_CL                                    0x%x"),
	_T("GRB_TRG_1_AC0_OPTO_CL                                    0x%x"),
	_T("GRB_TRG_0_AC1_OPTO_CL_SOL_EVCL                           0x%x"),
	_T("GRB_TRG_1_AC1_OPTO_CL_SOL_EVCL                           0x%x"),
	_T("GRB_TRG_0_AC1_OPTO_CL                                    0x%x"),
	_T("GRB_TRG_1_AC1_OPTO_CL                                    0x%x"),
	_T("GRB_TRG_0_4AC_CON0_OPTO_CL_RADIENT                       0x%x"),
	_T("GRB_TRG_1_4AC_CON0_OPTO_CL_RADIENT                       0x%x"),
	_T("GRB_TRG_0_4AC_CON2_OPTO_CL_RADIENT                       0x%x"),
	_T("GRB_TRG_1_4AC_CON2_OPTO_CL_RADIENT                       0x%x"),
	_T("GRB_TRG_2_4AC_CON1_OPTO_CL_RADIENT                       0x%x"),
	_T("GRB_TRG_3_4AC_CON1_OPTO_CL_RADIENT                       0x%x"),
	_T("GRB_TRG_2_4AC_CON3_OPTO_CL_RADIENT                       0x%x"),
	_T("GRB_TRG_3_4AC_CON3_OPTO_CL_RADIENT                       0x%x"),
	_T("GRB_TRG_0_AC0_TTL_CL_SOL_BASE                            0x%x"),
	_T("GRB_TRG_1_AC0_TTL_CL_SOL_BASE                            0x%x"),
	_T("GRB_TRG_0_AC0_TTL_CL_SOL_EVCL                            0x%x"),
	_T("GRB_TRG_1_AC0_TTL_CL_SOL_EVCL                            0x%x"),
	_T("GRB_TRG_0_AC0_TTL_CL                                     0x%x"),
	_T("GRB_TRG_1_AC0_TTL_CL                                     0x%x"),
	_T("GRB_TRG_0_AC1_TTL_CL_SOL_EVCL                            0x%x"),
	_T("GRB_TRG_1_AC1_TTL_CL_SOL_EVCL                            0x%x"),
	_T("GRB_TRG_0_AC1_TTL_CL                                     0x%x"),
	_T("GRB_TRG_1_AC1_TTL_CL                                     0x%x"),
	_T("GRB_TRG_2_AC01_TTL_CL_SOL_EVCL                           0x%x"),
	_T("GRB_TRG_3_AC01_TTL_CL_SOL_EVCL                           0x%x"),
	_T("GRB_TRG_2_AC01_TTL_CL                                    0x%x"),
	_T("GRB_TRG_3_AC01_TTL_CL                                    0x%x"),
	_T("GRB_TRG_0_4AC_CON0_TTL_CL_RADIENT                        0x%x"),
	_T("GRB_TRG_1_4AC_CON0_TTL_CL_RADIENT                        0x%x"),
	_T("GRB_TRG_0_4AC_CON1_TTL_CL_RADIENT                        0x%x"),
	_T("GRB_TRG_1_4AC_CON1_TTL_CL_RADIENT                        0x%x"),
	_T("GRB_TRG_0_4AC_CON2_TTL_CL_RADIENT                        0x%x"),
	_T("GRB_TRG_1_4AC_CON2_TTL_CL_RADIENT                        0x%x"),
	_T("GRB_TRG_0_4AC_CON3_TTL_CL_RADIENT                        0x%x"),
	_T("GRB_TRG_1_4AC_CON3_TTL_CL_RADIENT                        0x%x"),
	_T("GRB_TRG_2_4AC_CON0_TTL_CL_RADIENT                        0x%x"),
	_T("GRB_TRG_3_4AC_CON1_TTL_CL_RADIENT                        0x%x"),
	_T("GRB_TRG_2_4AC_CON2_TTL_CL_RADIENT                        0x%x"),
	_T("GRB_TRG_3_4AC_CON3_TTL_CL_RADIENT                        0x%x"),
	_T("GRB_TRG_0_AC0_LVDS_CL_SOL_BASE                           0x%x"),
	_T("GRB_TRG_1_AC0_LVDS_CL_SOL_BASE                           0x%x"),
	_T("GRB_TRG_0_AC0_LVDS_CL_SOL_EVCL                           0x%x"),
	_T("GRB_TRG_1_AC0_LVDS_CL_SOL_EVCL                           0x%x"),
	_T("GRB_TRG_0_AC0_LVDS_CL                                    0x%x"),
	_T("GRB_TRG_1_AC0_LVDS_CL                                    0x%x"),
	_T("GRB_TRG_0_AC1_LVDS_CL_SOL_EVCL                           0x%x"),
	_T("GRB_TRG_1_AC1_LVDS_CL_SOL_EVCL                           0x%x"),
	_T("GRB_TRG_0_AC1_LVDS_CL                                    0x%x"),
	_T("GRB_TRG_1_AC1_LVDS_CL                                    0x%x"),
	_T("GRB_TRG_2_AC01_LVDS_CL_SOL_EVCL                          0x%x"),
	_T("GRB_TRG_3_AC01_LVDS_CL_SOL_EVCL                          0x%x"),
	_T("GRB_TRG_2_AC01_LVDS_CL                                   0x%x"),
	_T("GRB_TRG_3_AC01_LVDS_CL                                   0x%x"),
	_T("GRB_TRG_0_4AC_CON0_LVDS_CL_RADIENT                       0x%x"),
	_T("GRB_TRG_1_4AC_CON0_LVDS_CL_RADIENT                       0x%x"),
	_T("GRB_TRG_0_4AC_CON2_LVDS_CL_RADIENT                       0x%x"),
	_T("GRB_TRG_1_4AC_CON2_LVDS_CL_RADIENT                       0x%x"),
	_T("GRB_TRG_2_4AC_CON1_LVDS_CL_RADIENT                       0x%x"),
	_T("GRB_TRG_3_4AC_CON1_LVDS_CL_RADIENT                       0x%x"),
	_T("GRB_TRG_2_4AC_CON3_LVDS_CL_RADIENT                       0x%x"),
	_T("GRB_TRG_3_4AC_CON3_LVDS_CL_RADIENT                       0x%x"),
	_T("GRB_TRG_1_AC0_OPTO_ANA                                   0x%x"),
	_T("GRB_TRG_1_AC1_OPTO_ANA                                   0x%x"),
	_T("GRB_TRG_1_AC2_OPTO_ANA                                   0x%x"),
	_T("GRB_TRG_1_AC3_OPTO_ANA                                   0x%x"),
	_T("GRB_TRG_0_AC0_TTL_ANA                                    0x%x"),
	_T("GRB_TRG_0_AC1_TTL_ANA                                    0x%x"),
	_T("GRB_TRG_0_AC2_TTL_ANA                                    0x%x"),
	_T("GRB_TRG_0_AC3_TTL_ANA                                    0x%x"),
	_T("GRB_TRG_2_4AC_AUX0_TTL_ANA                               0x%x"),
	_T("GRB_TRG_3_4AC_AUX1_TTL_ANA                               0x%x"),
	_T("GRB_TRG_2_4AC_AUX2_TTL_ANA                               0x%x"),
	_T("GRB_TRG_3_4AC_AUX3_TTL_ANA                               0x%x"),
	_T("GRB_TRG_2_4AC_AUX4_TTL_ANA                               0x%x"),
	_T("GRB_TRG_3_4AC_AUX5_TTL_ANA                               0x%x"),
	_T("GRB_TRG_2_4AC_AUX6_TTL_ANA                               0x%x"),
	_T("GRB_TRG_3_4AC_AUX7_TTL_ANA                               0x%x"),
	_T("GRB_TRG_2_4AC_AUX0_LVDS_ANA                              0x%x"),
	_T("GRB_TRG_3_4AC_AUX1_LVDS_ANA                              0x%x"),
	_T("GRB_TRG_2_4AC_AUX2_LVDS_ANA                              0x%x"),
	_T("GRB_TRG_3_4AC_AUX3_LVDS_ANA                              0x%x"),
	_T("GRB_TRG_2_4AC_AUX4_LVDS_ANA                              0x%x"),
	_T("GRB_TRG_3_4AC_AUX5_LVDS_ANA                              0x%x"),
	_T("GRB_TRG_2_4AC_AUX6_LVDS_ANA                              0x%x"),
	_T("GRB_TRG_3_4AC_AUX7_LVDS_ANA                              0x%x"),
	_T("GRB_TRG_0_AC0_OPTO_DIG                                   0x%x"),
	_T("GRB_TRG_1_AC0_OPTO_DIG                                   0x%x"),
	_T("GRB_TRG_0_AC1_OPTO_DIG                                   0x%x"),
	_T("GRB_TRG_1_AC1_OPTO_DIG                                   0x%x"),
	_T("GRB_TRG_0_AC2_OPTO_DIG                                   0x%x"),
	_T("GRB_TRG_1_AC2_OPTO_DIG                                   0x%x"),
	_T("GRB_TRG_0_AC3_OPTO_DIG                                   0x%x"),
	_T("GRB_TRG_1_AC3_OPTO_DIG                                   0x%x"),
	_T("GRB_TRG_1_AC0_TTL_DIG                                    0x%x"),
	_T("GRB_TRG_1_AC1_TTL_DIG                                    0x%x"),
	_T("GRB_TRG_1_AC2_TTL_DIG                                    0x%x"),
	_T("GRB_TRG_1_AC3_TTL_DIG                                    0x%x"),
	_T("GRB_TRG_0_AC0_LVDS_DIG                                   0x%x"),
	_T("GRB_TRG_0_AC1_LVDS_DIG                                   0x%x"),
	_T("GRB_TRG_0_AC2_LVDS_DIG                                   0x%x"),
	_T("GRB_TRG_0_AC3_LVDS_DIG                                   0x%x"),
	_T("GRB_TRG_2_AC0_AUX1_TTL_DIG                               0x%x"),
	_T("GRB_TRG_3_AC0_AUX2_TTL_DIG                               0x%x"),
	_T("GRB_TRG_2_AC1_AUX1_TTL_DIG                               0x%x"),
	_T("GRB_TRG_3_AC1_AUX2_TTL_DIG                               0x%x"),
	_T("GRB_TRG_2_AC2_AUX1_TTL_DIG                               0x%x"),
	_T("GRB_TRG_3_AC2_AUX2_TTL_DIG                               0x%x"),
	_T("GRB_TRG_2_AC3_AUX1_TTL_DIG                               0x%x"),
	_T("GRB_TRG_3_AC3_AUX2_TTL_DIG                               0x%x"),
	_T("GRB_TRG_2_AC0_AUX1_LVDS_DIG                              0x%x"),
	_T("GRB_TRG_3_AC0_AUX2_LVDS_DIG                              0x%x"),
	_T("GRB_TRG_2_AC1_AUX1_LVDS_DIG                              0x%x"),
	_T("GRB_TRG_3_AC1_AUX2_LVDS_DIG                              0x%x"),
	_T("GRB_TRG_2_AC2_AUX1_LVDS_DIG                              0x%x"),
	_T("GRB_TRG_3_AC2_AUX2_LVDS_DIG                              0x%x"),
	_T("GRB_TRG_2_AC3_AUX1_LVDS_DIG                              0x%x"),
	_T("GRB_TRG_3_AC3_AUX2_LVDS_DIG                              0x%x"),
	_T("GRB_TRG_AC0_LVDS_CL_SOL_BASE_ROTARY_ENCODER_FOW          0x%x"),
	_T("GRB_TRG_AC0_LVDS_CL_SOL_BASE_ROTARY_ENCODER_REV          0x%x"),
	_T("GRB_TRG_AC0_LVDS_CL_SOL_EVCL_ROTARY_ENCODER_FOW          0x%x"),
	_T("GRB_TRG_AC0_LVDS_CL_SOL_EVCL_ROTARY_ENCODER_REV          0x%x"),
	_T("GRB_TRG_AC0_LVDS_CL_ROTARY_ENCODER_FOW                   0x%x"),
	_T("GRB_TRG_AC0_LVDS_CL_ROTARY_ENCODER_REV                   0x%x"),
	_T("GRB_TRG_AC1_LVDS_CL_SOL_EVCL_ROTARY_ENCODER_FOW          0x%x"),
	_T("GRB_TRG_AC1_LVDS_CL_SOL_EVCL_ROTARY_ENCODER_REV          0x%x"),
	_T("GRB_TRG_AC1_LVDS_CL_ROTARY_ENCODER_FOW                   0x%x"),
	_T("GRB_TRG_AC1_LVDS_CL_ROTARY_ENCODER_REV                   0x%x"),
	_T("GRB_TRG_AC0_LVDS_CL_RADIENT_ROTARY_ENCODER_FOW           0x%x"),
	_T("GRB_TRG_AC0_LVDS_CL_RADIENT_ROTARY_ENCODER_REV           0x%x"),
	_T("GRB_TRG_AC1_LVDS_CL_RADIENT_ROTARY_ENCODER_FOW           0x%x"),
	_T("GRB_TRG_AC1_LVDS_CL_RADIENT_ROTARY_ENCODER_REV           0x%x"),
	_T("GRB_TRG_AC2_LVDS_CL_RADIENT_ROTARY_ENCODER_FOW           0x%x"),
	_T("GRB_TRG_AC2_LVDS_CL_RADIENT_ROTARY_ENCODER_REV           0x%x"),
	_T("GRB_TRG_AC3_LVDS_CL_RADIENT_ROTARY_ENCODER_FOW           0x%x"),
	_T("GRB_TRG_AC3_LVDS_CL_RADIENT_ROTARY_ENCODER_REV           0x%x"),
	_T("GRB_TRG_AC0_LVDS_ANA_ROTARY_ENCODER_FOW                  0x%x"),
	_T("GRB_TRG_AC0_LVDS_ANA_ROTARY_ENCODER_REV                  0x%x"),
	_T("GRB_TRG_AC1_LVDS_ANA_ROTARY_ENCODER_FOW                  0x%x"),
	_T("GRB_TRG_AC1_LVDS_ANA_ROTARY_ENCODER_REV                  0x%x"),
	_T("GRB_TRG_AC2_LVDS_ANA_ROTARY_ENCODER_FOW                  0x%x"),
	_T("GRB_TRG_AC2_LVDS_ANA_ROTARY_ENCODER_REV                  0x%x"),
	_T("GRB_TRG_AC3_LVDS_ANA_ROTARY_ENCODER_FOW                  0x%x"),
	_T("GRB_TRG_AC3_LVDS_ANA_ROTARY_ENCODER_REV                  0x%x"),
	_T("GRB_TRG_AC0_LVDS_DIG_ROTARY_ENCODER_FOW                  0x%x"),
	_T("GRB_TRG_AC0_LVDS_DIG_ROTARY_ENCODER_REV                  0x%x"),
	_T("GRB_TRG_AC1_LVDS_DIG_ROTARY_ENCODER_FOW                  0x%x"),
	_T("GRB_TRG_AC1_LVDS_DIG_ROTARY_ENCODER_REV                  0x%x"),
	_T("GRB_TRG_AC2_LVDS_DIG_ROTARY_ENCODER_FOW                  0x%x"),
	_T("GRB_TRG_AC2_LVDS_DIG_ROTARY_ENCODER_REV                  0x%x"),
	_T("GRB_TRG_AC3_LVDS_DIG_ROTARY_ENCODER_FOW                  0x%x"),
	_T("GRB_TRG_AC3_LVDS_DIG_ROTARY_ENCODER_REV                  0x%x"),
	_T("GRB_TRG_LVDS_ROTARY_ENCODER                              0x%x"),
	_T("GRB_DISABLED_ARM                                         0x%x"),
	_T("GRB_HARDWARE_ARM                                         0x%x"),
	_T("GRB_SOFTWARE_ARM                                         0x%x"),
	_T("GRB_HARDWARE_LATCH_ARM                                   0x%x"),
};


static LPCTSTR g_lpszExp_Trigger[EMatoxDCF_EOFExp_Trigger] =
{
	_T("EXP_TRG_TTL_TIMER0                                       0x%x"),
	_T("EXP_TRG_CNTEQ0_TIMER0                                    0x%x"),
	_T("EXP_TRG_TIMER1                                           0x%x"),
	_T("EXP_TRG_TIMER2                                           0x%x"),
	_T("EXP_TRG_TIMER3                                           0x%x"),
	_T("EXP_0_TRG_TIMER0_AC0                                     0x%x"),
	_T("EXP_0_TRG_TIMER0_AC1                                     0x%x"),
	_T("EXP_0_TRG_TIMER0_AC2                                     0x%x"),
	_T("EXP_0_TRG_TIMER0_AC3                                     0x%x"),
	_T("EXP_0_TRG_2_AC01_OPTO_CL_SOL_EVCL                        0x%x"),
	_T("EXP_0_TRG_3_AC01_OPTO_CL_SOL_EVCL                        0x%x"),
	_T("EXP_0_TRG_2_AC01_OPTO_CL                                 0x%x"),
	_T("EXP_0_TRG_3_AC01_OPTO_CL                                 0x%x"),
	_T("EXP_0_TRG_0_AC0_OPTO_CL_SOL_BASE                         0x%x"),
	_T("EXP_0_TRG_1_AC0_OPTO_CL_SOL_BASE                         0x%x"),
	_T("EXP_0_TRG_0_AC0_OPTO_CL_SOL_EVCL                         0x%x"),
	_T("EXP_0_TRG_1_AC0_OPTO_CL_SOL_EVCL                         0x%x"),
	_T("EXP_0_TRG_0_AC0_OPTO_CL                                  0x%x"),
	_T("EXP_0_TRG_1_AC0_OPTO_CL                                  0x%x"),
	_T("EXP_0_TRG_0_AC1_OPTO_CL_SOL_EVCL                         0x%x"),
	_T("EXP_0_TRG_1_AC1_OPTO_CL_SOL_EVCL                         0x%x"),
	_T("EXP_0_TRG_0_AC1_OPTO_CL                                  0x%x"),
	_T("EXP_0_TRG_1_AC1_OPTO_CL                                  0x%x"),
	_T("EXP_0_TRG_0_4AC_CON0_OPTO_CL_RADIENT                     0x%x"),
	_T("EXP_0_TRG_1_4AC_CON0_OPTO_CL_RADIENT                     0x%x"),
	_T("EXP_0_TRG_0_4AC_CON2_OPTO_CL_RADIENT                     0x%x"),
	_T("EXP_0_TRG_1_4AC_CON2_OPTO_CL_RADIENT                     0x%x"),
	_T("EXP_0_TRG_2_4AC_CON1_OPTO_CL_RADIENT                     0x%x"),
	_T("EXP_0_TRG_3_4AC_CON1_OPTO_CL_RADIENT                     0x%x"),
	_T("EXP_0_TRG_2_4AC_CON3_OPTO_CL_RADIENT                     0x%x"),
	_T("EXP_0_TRG_3_4AC_CON3_OPTO_CL_RADIENT                     0x%x"),
	_T("EXP_0_TRG_0_AC0_TTL_CL_SOL_BASE                          0x%x"),
	_T("EXP_0_TRG_1_AC0_TTL_CL_SOL_BASE                          0x%x"),
	_T("EXP_0_TRG_0_AC0_TTL_CL_SOL_EVCL                          0x%x"),
	_T("EXP_0_TRG_1_AC0_TTL_CL_SOL_EVCL                          0x%x"),
	_T("EXP_0_TRG_0_AC0_TTL_CL                                   0x%x"),
	_T("EXP_0_TRG_1_AC0_TTL_CL                                   0x%x"),
	_T("EXP_0_TRG_0_AC1_TTL_CL_SOL_EVCL                          0x%x"),
	_T("EXP_0_TRG_1_AC1_TTL_CL_SOL_EVCL                          0x%x"),
	_T("EXP_0_TRG_0_AC1_TTL_CL                                   0x%x"),
	_T("EXP_0_TRG_1_AC1_TTL_CL                                   0x%x"),
	_T("EXP_0_TRG_2_AC01_TTL_CL_SOL_EVCL                         0x%x"),
	_T("EXP_0_TRG_3_AC01_TTL_CL_SOL_EVCL                         0x%x"),
	_T("EXP_0_TRG_2_AC01_TTL_CL                                  0x%x"),
	_T("EXP_0_TRG_3_AC01_TTL_CL                                  0x%x"),
	_T("EXP_0_TRG_0_4AC_CON0_TTL_CL_RADIENT                      0x%x"),
	_T("EXP_0_TRG_1_4AC_CON0_TTL_CL_RADIENT                      0x%x"),
	_T("EXP_0_TRG_0_4AC_CON1_TTL_CL_RADIENT                      0x%x"),
	_T("EXP_0_TRG_1_4AC_CON1_TTL_CL_RADIENT                      0x%x"),
	_T("EXP_0_TRG_0_4AC_CON2_TTL_CL_RADIENT                      0x%x"),
	_T("EXP_0_TRG_1_4AC_CON2_TTL_CL_RADIENT                      0x%x"),
	_T("EXP_0_TRG_0_4AC_CON3_TTL_CL_RADIENT                      0x%x"),
	_T("EXP_0_TRG_1_4AC_CON3_TTL_CL_RADIENT                      0x%x"),
	_T("EXP_0_TRG_2_4AC_CON0_TTL_CL_RADIENT                      0x%x"),
	_T("EXP_0_TRG_3_4AC_CON1_TTL_CL_RADIENT                      0x%x"),
	_T("EXP_0_TRG_2_4AC_CON2_TTL_CL_RADIENT                      0x%x"),
	_T("EXP_0_TRG_3_4AC_CON3_TTL_CL_RADIENT                      0x%x"),
	_T("EXP_0_TRG_0_AC0_LVDS_CL_SOL_BASE                         0x%x"),
	_T("EXP_0_TRG_1_AC0_LVDS_CL_SOL_BASE                         0x%x"),
	_T("EXP_0_TRG_0_AC0_LVDS_CL_SOL_EVCL                         0x%x"),
	_T("EXP_0_TRG_1_AC0_LVDS_CL_SOL_EVCL                         0x%x"),
	_T("EXP_0_TRG_0_AC0_LVDS_CL                                  0x%x"),
	_T("EXP_0_TRG_1_AC0_LVDS_CL                                  0x%x"),
	_T("EXP_0_TRG_0_AC1_LVDS_CL_SOL_EVCL                         0x%x"),
	_T("EXP_0_TRG_1_AC1_LVDS_CL_SOL_EVCL                         0x%x"),
	_T("EXP_0_TRG_0_AC1_LVDS_CL                                  0x%x"),
	_T("EXP_0_TRG_1_AC1_LVDS_CL                                  0x%x"),
	_T("EXP_0_TRG_2_AC01_LVDS_CL_SOL_EVCL                        0x%x"),
	_T("EXP_0_TRG_3_AC01_LVDS_CL_SOL_EVCL                        0x%x"),
	_T("EXP_0_TRG_2_AC01_LVDS_CL                                 0x%x"),
	_T("EXP_0_TRG_3_AC01_LVDS_CL                                 0x%x"),
	_T("EXP_0_TRG_0_4AC_CON0_LVDS_CL_RADIENT                     0x%x"),
	_T("EXP_0_TRG_1_4AC_CON0_LVDS_CL_RADIENT                     0x%x"),
	_T("EXP_0_TRG_0_4AC_CON2_LVDS_CL_RADIENT                     0x%x"),
	_T("EXP_0_TRG_1_4AC_CON2_LVDS_CL_RADIENT                     0x%x"),
	_T("EXP_0_TRG_2_4AC_CON1_LVDS_CL_RADIENT                     0x%x"),
	_T("EXP_0_TRG_3_4AC_CON1_LVDS_CL_RADIENT                     0x%x"),
	_T("EXP_0_TRG_2_4AC_CON3_LVDS_CL_RADIENT                     0x%x"),
	_T("EXP_0_TRG_3_4AC_CON3_LVDS_CL_RADIENT                     0x%x"),
	_T("EXP_0_TRG_1_AC0_OPTO_ANA                                 0x%x"),
	_T("EXP_0_TRG_1_AC1_OPTO_ANA                                 0x%x"),
	_T("EXP_0_TRG_1_AC2_OPTO_ANA                                 0x%x"),
	_T("EXP_0_TRG_1_AC3_OPTO_ANA                                 0x%x"),
	_T("EXP_0_TRG_0_AC0_TTL_ANA                                  0x%x"),
	_T("EXP_0_TRG_0_AC1_TTL_ANA                                  0x%x"),
	_T("EXP_0_TRG_0_AC2_TTL_ANA                                  0x%x"),
	_T("EXP_0_TRG_0_AC3_TTL_ANA                                  0x%x"),
	_T("EXP_0_TRG_2_4AC_AUX0_TTL_ANA                             0x%x"),
	_T("EXP_0_TRG_3_4AC_AUX1_TTL_ANA                             0x%x"),
	_T("EXP_0_TRG_2_4AC_AUX2_TTL_ANA                             0x%x"),
	_T("EXP_0_TRG_3_4AC_AUX3_TTL_ANA                             0x%x"),
	_T("EXP_0_TRG_2_4AC_AUX4_TTL_ANA                             0x%x"),
	_T("EXP_0_TRG_3_4AC_AUX5_TTL_ANA                             0x%x"),
	_T("EXP_0_TRG_2_4AC_AUX6_TTL_ANA                             0x%x"),
	_T("EXP_0_TRG_3_4AC_AUX7_TTL_ANA                             0x%x"),
	_T("EXP_0_TRG_2_4AC_AUX0_LVDS_ANA                            0x%x"),
	_T("EXP_0_TRG_3_4AC_AUX1_LVDS_ANA                            0x%x"),
	_T("EXP_0_TRG_2_4AC_AUX2_LVDS_ANA                            0x%x"),
	_T("EXP_0_TRG_3_4AC_AUX3_LVDS_ANA                            0x%x"),
	_T("EXP_0_TRG_2_4AC_AUX4_LVDS_ANA                            0x%x"),
	_T("EXP_0_TRG_3_4AC_AUX5_LVDS_ANA                            0x%x"),
	_T("EXP_0_TRG_2_4AC_AUX6_LVDS_ANA                            0x%x"),
	_T("EXP_0_TRG_3_4AC_AUX7_LVDS_ANA                            0x%x"),
	_T("EXP_0_TRG_0_AC0_OPTO_DIG                                 0x%x"),
	_T("EXP_0_TRG_1_AC0_OPTO_DIG                                 0x%x"),
	_T("EXP_0_TRG_0_AC1_OPTO_DIG                                 0x%x"),
	_T("EXP_0_TRG_1_AC1_OPTO_DIG                                 0x%x"),
	_T("EXP_0_TRG_0_AC2_OPTO_DIG                                 0x%x"),
	_T("EXP_0_TRG_1_AC2_OPTO_DIG                                 0x%x"),
	_T("EXP_0_TRG_0_AC3_OPTO_DIG                                 0x%x"),
	_T("EXP_0_TRG_1_AC3_OPTO_DIG                                 0x%x"),
	_T("EXP_0_TRG_1_AC0_TTL_DIG                                  0x%x"),
	_T("EXP_0_TRG_1_AC1_TTL_DIG                                  0x%x"),
	_T("EXP_0_TRG_1_AC2_TTL_DIG                                  0x%x"),
	_T("EXP_0_TRG_1_AC3_TTL_DIG                                  0x%x"),
	_T("EXP_0_TRG_0_AC0_LVDS_DIG                                 0x%x"),
	_T("EXP_0_TRG_0_AC1_LVDS_DIG                                 0x%x"),
	_T("EXP_0_TRG_0_AC2_LVDS_DIG                                 0x%x"),
	_T("EXP_0_TRG_0_AC3_LVDS_DIG                                 0x%x"),
	_T("EXP_0_TRG_2_AC0_AUX1_TTL_DIG                             0x%x"),
	_T("EXP_0_TRG_3_AC0_AUX2_TTL_DIG                             0x%x"),
	_T("EXP_0_TRG_2_AC1_AUX1_TTL_DIG                             0x%x"),
	_T("EXP_0_TRG_3_AC1_AUX2_TTL_DIG                             0x%x"),
	_T("EXP_0_TRG_2_AC2_AUX1_TTL_DIG                             0x%x"),
	_T("EXP_0_TRG_3_AC2_AUX2_TTL_DIG                             0x%x"),
	_T("EXP_0_TRG_2_AC3_AUX1_TTL_DIG                             0x%x"),
	_T("EXP_0_TRG_3_AC3_AUX2_TTL_DIG                             0x%x"),
	_T("EXP_0_TRG_2_AC0_AUX1_LVDS_DIG                            0x%x"),
	_T("EXP_0_TRG_3_AC0_AUX2_LVDS_DIG                            0x%x"),
	_T("EXP_0_TRG_2_AC1_AUX1_LVDS_DIG                            0x%x"),
	_T("EXP_0_TRG_3_AC1_AUX2_LVDS_DIG                            0x%x"),
	_T("EXP_0_TRG_2_AC2_AUX1_LVDS_DIG                            0x%x"),
	_T("EXP_0_TRG_3_AC2_AUX2_LVDS_DIG                            0x%x"),
	_T("EXP_0_TRG_2_AC3_AUX1_LVDS_DIG                            0x%x"),
	_T("EXP_0_TRG_3_AC3_AUX2_LVDS_DIG                            0x%x"),
	_T("EXP_0_TRG_AC0_LVDS_CL_SOL_BASE_ROTARY_ENCODER_FOW        0x%x"),
	_T("EXP_0_TRG_AC0_LVDS_CL_SOL_BASE_ROTARY_ENCODER_REV        0x%x"),
	_T("EXP_0_TRG_AC0_LVDS_CL_SOL_EVCL_ROTARY_ENCODER_FOW        0x%x"),
	_T("EXP_0_TRG_AC0_LVDS_CL_SOL_EVCL_ROTARY_ENCODER_REV        0x%x"),
	_T("EXP_0_TRG_AC0_LVDS_CL_ROTARY_ENCODER_FOW                 0x%x"),
	_T("EXP_0_TRG_AC0_LVDS_CL_ROTARY_ENCODER_REV                 0x%x"),
	_T("EXP_0_TRG_AC1_LVDS_CL_SOL_EVCL_ROTARY_ENCODER_FOW        0x%x"),
	_T("EXP_0_TRG_AC1_LVDS_CL_SOL_EVCL_ROTARY_ENCODER_REV        0x%x"),
	_T("EXP_0_TRG_AC1_LVDS_CL_ROTARY_ENCODER_FOW                 0x%x"),
	_T("EXP_0_TRG_AC1_LVDS_CL_ROTARY_ENCODER_REV                 0x%x"),
	_T("EXP_0_TRG_AC0_LVDS_CL_RADIENT_ROTARY_ENCODER_FOW         0x%x"),
	_T("EXP_0_TRG_AC0_LVDS_CL_RADIENT_ROTARY_ENCODER_REV         0x%x"),
	_T("EXP_0_TRG_AC1_LVDS_CL_RADIENT_ROTARY_ENCODER_FOW         0x%x"),
	_T("EXP_0_TRG_AC1_LVDS_CL_RADIENT_ROTARY_ENCODER_REV         0x%x"),
	_T("EXP_0_TRG_AC2_LVDS_CL_RADIENT_ROTARY_ENCODER_FOW         0x%x"),
	_T("EXP_0_TRG_AC2_LVDS_CL_RADIENT_ROTARY_ENCODER_REV         0x%x"),
	_T("EXP_0_TRG_AC3_LVDS_CL_RADIENT_ROTARY_ENCODER_FOW         0x%x"),
	_T("EXP_0_TRG_AC3_LVDS_CL_RADIENT_ROTARY_ENCODER_REV         0x%x"),
	_T("EXP_0_TRG_AC0_LVDS_ANA_ROTARY_ENCODER_FOW                0x%x"),
	_T("EXP_0_TRG_AC0_LVDS_ANA_ROTARY_ENCODER_REV                0x%x"),
	_T("EXP_0_TRG_AC1_LVDS_ANA_ROTARY_ENCODER_FOW                0x%x"),
	_T("EXP_0_TRG_AC1_LVDS_ANA_ROTARY_ENCODER_REV                0x%x"),
	_T("EXP_0_TRG_AC2_LVDS_ANA_ROTARY_ENCODER_FOW                0x%x"),
	_T("EXP_0_TRG_AC2_LVDS_ANA_ROTARY_ENCODER_REV                0x%x"),
	_T("EXP_0_TRG_AC3_LVDS_ANA_ROTARY_ENCODER_FOW                0x%x"),
	_T("EXP_0_TRG_AC3_LVDS_ANA_ROTARY_ENCODER_REV                0x%x"),
	_T("EXP_0_TRG_AC0_LVDS_DIG_ROTARY_ENCODER_FOW                0x%x"),
	_T("EXP_0_TRG_AC0_LVDS_DIG_ROTARY_ENCODER_REV                0x%x"),
	_T("EXP_0_TRG_AC1_LVDS_DIG_ROTARY_ENCODER_FOW                0x%x"),
	_T("EXP_0_TRG_AC1_LVDS_DIG_ROTARY_ENCODER_REV                0x%x"),
	_T("EXP_0_TRG_AC2_LVDS_DIG_ROTARY_ENCODER_FOW                0x%x"),
	_T("EXP_0_TRG_AC2_LVDS_DIG_ROTARY_ENCODER_REV                0x%x"),
	_T("EXP_0_TRG_AC3_LVDS_DIG_ROTARY_ENCODER_FOW                0x%x"),
	_T("EXP_0_TRG_AC3_LVDS_DIG_ROTARY_ENCODER_REV                0x%x"),
	_T("EXP_0_TRG_LVDS_ROTARY_ENCODER                            0x%x"),
	_T("EXP_CLK_CLKGEN                                           0x%x"),
	_T("EXP_CLK_HS                                               0x%x"),
	_T("EXP_CLK_VS                                               0x%x"),
	_T("EXP_CLK_TIMER1                                           0x%x"),
	_T("EXP_CLK_TIMER2                                           0x%x"),
	_T("EXP_CLK_TIMER3                                           0x%x"),
	_T("EXP_CLK_AUXIN1_LVDS                                      0x%x"),
	_T("EXP_CLK_AUXIN3_LVDS                                      0x%x"),
	_T("EXP_CLK_AUXIN1_AC0_CL                                    0x%x"),
	_T("EXP_CLK_AUXIN3_AC1_CL                                    0x%x"),
	_T("EXP_CLK_AUXIN1_AC0_CL_SOL_BASE_EVCL_RADIENT              0x%x"),
	_T("EXP_CLK_AUXIN3_AC1_CL_SOL_BASE_EVCL_RADIENT              0x%x"),
	_T("EXP_CLK_AUXIN1_AC2_CL_RADIENT                            0x%x"),
	_T("EXP_CLK_AUXIN3_AC3_CL_RADIENT                            0x%x"),
	_T("EXP_CLK_AUXIN1_AC0_ANA                                   0x%x"),
	_T("EXP_CLK_AUXIN3_AC1_ANA                                   0x%x"),
	_T("EXP_CLK_AUXIN5_AC2_ANA                                   0x%x"),
	_T("EXP_CLK_AUXIN7_AC3_ANA                                   0x%x"),
	_T("EXP_CLK_AUXIN2_AC0_DIG                                   0x%x"),
	_T("EXP_CLK_AUXIN2_AC1_DIG                                   0x%x"),
	_T("EXP_CLK_AUXIN2_AC2_DIG                                   0x%x"),
	_T("EXP_CLK_AUXIN2_AC3_DIG                                   0x%x"),
	_T("EXP_CLK_AUXIN1_AC0_CL_SOL_EVCL                           0x%x"),
	_T("EXP_CLK_AUXIN3_AC1_CL_SOL_EVCL                           0x%x"),
	_T("ARM_EXP_0_CNTEQ0                                         0x%x"),
	_T("ARM_EXP_0_SOFTWARE                                       0x%x"),
	_T("ARM_EXP_0_TIMER1                                         0x%x"),
	_T("ARM_EXP_0_TIMER2                                         0x%x"),
	_T("ARM_EXP_0_TIMER3                                         0x%x"),
	_T("ARM_EXP_0_TRG_TIMER0_AC0                                 0x%x"),
	_T("ARM_EXP_0_TRG_TIMER0_AC1                                 0x%x"),
	_T("ARM_EXP_0_TRG_TIMER0_AC2                                 0x%x"),
	_T("ARM_EXP_0_TRG_TIMER0_AC3                                 0x%x"),
	_T("ARM_EXP_0_HS_PSG                                         0x%x"),
	_T("ARM_EXP_0_VS_PSG                                         0x%x"),
	_T("ARM_EXP_0_TRG_2_AC01_OPTO_CL_SOL_EVCL                    0x%x"),
	_T("ARM_EXP_0_TRG_3_AC01_OPTO_CL_SOL_EVCL                    0x%x"),
	_T("ARM_EXP_0_TRG_2_AC01_OPTO_CL                             0x%x"),
	_T("ARM_EXP_0_TRG_3_AC01_OPTO_CL                             0x%x"),
	_T("ARM_EXP_0_TRG_0_AC0_OPTO_CL_SOL_BASE                     0x%x"),
	_T("ARM_EXP_0_TRG_1_AC0_OPTO_CL_SOL_BASE                     0x%x"),
	_T("ARM_EXP_0_TRG_0_AC0_OPTO_CL_SOL_EVCL                     0x%x"),
	_T("ARM_EXP_0_TRG_1_AC0_OPTO_CL_SOL_EVCL                     0x%x"),
	_T("ARM_EXP_0_TRG_0_AC0_OPTO_CL                              0x%x"),
	_T("ARM_EXP_0_TRG_1_AC0_OPTO_CL                              0x%x"),
	_T("ARM_EXP_0_TRG_0_AC1_OPTO_CL_SOL_EVCL                     0x%x"),
	_T("ARM_EXP_0_TRG_1_AC1_OPTO_CL_SOL_EVCL                     0x%x"),
	_T("ARM_EXP_0_TRG_0_AC1_OPTO_CL                              0x%x"),
	_T("ARM_EXP_0_TRG_1_AC1_OPTO_CL                              0x%x"),
	_T("ARM_EXP_0_TRG_0_4AC_CON0_OPTO_CL_RADIENT                 0x%x"),
	_T("ARM_EXP_0_TRG_1_4AC_CON0_OPTO_CL_RADIENT                 0x%x"),
	_T("ARM_EXP_0_TRG_0_4AC_CON2_OPTO_CL_RADIENT                 0x%x"),
	_T("ARM_EXP_0_TRG_1_4AC_CON2_OPTO_CL_RADIENT                 0x%x"),
	_T("ARM_EXP_0_TRG_2_4AC_CON1_OPTO_CL_RADIENT                 0x%x"),
	_T("ARM_EXP_0_TRG_3_4AC_CON1_OPTO_CL_RADIENT                 0x%x"),
	_T("ARM_EXP_0_TRG_2_4AC_CON3_OPTO_CL_RADIENT                 0x%x"),
	_T("ARM_EXP_0_TRG_3_4AC_CON3_OPTO_CL_RADIENT                 0x%x"),
	_T("ARM_EXP_0_TRG_0_AC0_TTL_CL_SOL_BASE                      0x%x"),
	_T("ARM_EXP_0_TRG_1_AC0_TTL_CL_SOL_BASE                      0x%x"),
	_T("ARM_EXP_0_TRG_0_AC0_TTL_CL_SOL_EVCL                      0x%x"),
	_T("ARM_EXP_0_TRG_1_AC0_TTL_CL_SOL_EVCL                      0x%x"),
	_T("ARM_EXP_0_TRG_0_AC0_TTL_CL                               0x%x"),
	_T("ARM_EXP_0_TRG_1_AC0_TTL_CL                               0x%x"),
	_T("ARM_EXP_0_TRG_0_AC1_TTL_CL_SOL_EVCL                      0x%x"),
	_T("ARM_EXP_0_TRG_1_AC1_TTL_CL_SOL_EVCL                      0x%x"),
	_T("ARM_EXP_0_TRG_0_AC1_TTL_CL                               0x%x"),
	_T("ARM_EXP_0_TRG_1_AC1_TTL_CL                               0x%x"),
	_T("ARM_EXP_0_TRG_2_AC01_TTL_CL_SOL_EVCL                     0x%x"),
	_T("ARM_EXP_0_TRG_3_AC01_TTL_CL_SOL_EVCL                     0x%x"),
	_T("ARM_EXP_0_TRG_2_AC01_TTL_CL                              0x%x"),
	_T("ARM_EXP_0_TRG_3_AC01_TTL_CL                              0x%x"),
	_T("ARM_EXP_0_TRG_0_4AC_CON0_TTL_CL_RADIENT                  0x%x"),
	_T("ARM_EXP_0_TRG_1_4AC_CON0_TTL_CL_RADIENT                  0x%x"),
	_T("ARM_EXP_0_TRG_0_4AC_CON1_TTL_CL_RADIENT                  0x%x"),
	_T("ARM_EXP_0_TRG_1_4AC_CON1_TTL_CL_RADIENT                  0x%x"),
	_T("ARM_EXP_0_TRG_0_4AC_CON2_TTL_CL_RADIENT                  0x%x"),
	_T("ARM_EXP_0_TRG_1_4AC_CON2_TTL_CL_RADIENT                  0x%x"),
	_T("ARM_EXP_0_TRG_0_4AC_CON3_TTL_CL_RADIENT                  0x%x"),
	_T("ARM_EXP_0_TRG_1_4AC_CON3_TTL_CL_RADIENT                  0x%x"),
	_T("ARM_EXP_0_TRG_2_4AC_CON0_TTL_CL_RADIENT                  0x%x"),
	_T("ARM_EXP_0_TRG_3_4AC_CON1_TTL_CL_RADIENT                  0x%x"),
	_T("ARM_EXP_0_TRG_2_4AC_CON2_TTL_CL_RADIENT                  0x%x"),
	_T("ARM_EXP_0_TRG_3_4AC_CON3_TTL_CL_RADIENT                  0x%x"),
	_T("ARM_EXP_0_TRG_0_AC0_LVDS_CL_SOL_BASE                     0x%x"),
	_T("ARM_EXP_0_TRG_1_AC0_LVDS_CL_SOL_BASE                     0x%x"),
	_T("ARM_EXP_0_TRG_0_AC0_LVDS_CL_SOL_EVCL                     0x%x"),
	_T("ARM_EXP_0_TRG_1_AC0_LVDS_CL_SOL_EVCL                     0x%x"),
	_T("ARM_EXP_0_TRG_0_AC0_LVDS_CL                              0x%x"),
	_T("ARM_EXP_0_TRG_1_AC0_LVDS_CL                              0x%x"),
	_T("ARM_EXP_0_TRG_0_AC1_LVDS_CL_SOL_EVCL                     0x%x"),
	_T("ARM_EXP_0_TRG_1_AC1_LVDS_CL_SOL_EVCL                     0x%x"),
	_T("ARM_EXP_0_TRG_0_AC1_LVDS_CL                              0x%x"),
	_T("ARM_EXP_0_TRG_1_AC1_LVDS_CL                              0x%x"),
	_T("ARM_EXP_0_TRG_2_AC01_LVDS_CL_SOL_EVCL                    0x%x"),
	_T("ARM_EXP_0_TRG_3_AC01_LVDS_CL_SOL_EVCL                    0x%x"),
	_T("ARM_EXP_0_TRG_2_AC01_LVDS_CL                             0x%x"),
	_T("ARM_EXP_0_TRG_3_AC01_LVDS_CL                             0x%x"),
	_T("ARM_EXP_0_TRG_0_4AC_CON0_LVDS_CL_RADIENT                 0x%x"),
	_T("ARM_EXP_0_TRG_1_4AC_CON0_LVDS_CL_RADIENT                 0x%x"),
	_T("ARM_EXP_0_TRG_0_4AC_CON2_LVDS_CL_RADIENT                 0x%x"),
	_T("ARM_EXP_0_TRG_1_4AC_CON2_LVDS_CL_RADIENT                 0x%x"),
	_T("ARM_EXP_0_TRG_2_4AC_CON1_LVDS_CL_RADIENT                 0x%x"),
	_T("ARM_EXP_0_TRG_3_4AC_CON1_LVDS_CL_RADIENT                 0x%x"),
	_T("ARM_EXP_0_TRG_2_4AC_CON3_LVDS_CL_RADIENT                 0x%x"),
	_T("ARM_EXP_0_TRG_3_4AC_CON3_LVDS_CL_RADIENT                 0x%x"),
	_T("ARM_EXP_0_TRG_1_AC0_OPTO_ANA                             0x%x"),
	_T("ARM_EXP_0_TRG_1_AC1_OPTO_ANA                             0x%x"),
	_T("ARM_EXP_0_TRG_1_AC2_OPTO_ANA                             0x%x"),
	_T("ARM_EXP_0_TRG_1_AC3_OPTO_ANA                             0x%x"),
	_T("ARM_EXP_0_TRG_0_AC0_TTL_ANA                              0x%x"),
	_T("ARM_EXP_0_TRG_0_AC1_TTL_ANA                              0x%x"),
	_T("ARM_EXP_0_TRG_0_AC2_TTL_ANA                              0x%x"),
	_T("ARM_EXP_0_TRG_0_AC3_TTL_ANA                              0x%x"),
	_T("ARM_EXP_0_TRG_2_4AC_AUX0_TTL_ANA                         0x%x"),
	_T("ARM_EXP_0_TRG_3_4AC_AUX1_TTL_ANA                         0x%x"),
	_T("ARM_EXP_0_TRG_2_4AC_AUX2_TTL_ANA                         0x%x"),
	_T("ARM_EXP_0_TRG_3_4AC_AUX3_TTL_ANA                         0x%x"),
	_T("ARM_EXP_0_TRG_2_4AC_AUX4_TTL_ANA                         0x%x"),
	_T("ARM_EXP_0_TRG_3_4AC_AUX5_TTL_ANA                         0x%x"),
	_T("ARM_EXP_0_TRG_2_4AC_AUX6_TTL_ANA                         0x%x"),
	_T("ARM_EXP_0_TRG_3_4AC_AUX7_TTL_ANA                         0x%x"),
	_T("ARM_EXP_0_TRG_2_4AC_AUX0_LVDS_ANA                        0x%x"),
	_T("ARM_EXP_0_TRG_3_4AC_AUX1_LVDS_ANA                        0x%x"),
	_T("ARM_EXP_0_TRG_2_4AC_AUX2_LVDS_ANA                        0x%x"),
	_T("ARM_EXP_0_TRG_3_4AC_AUX3_LVDS_ANA                        0x%x"),
	_T("ARM_EXP_0_TRG_2_4AC_AUX4_LVDS_ANA                        0x%x"),
	_T("ARM_EXP_0_TRG_3_4AC_AUX5_LVDS_ANA                        0x%x"),
	_T("ARM_EXP_0_TRG_2_4AC_AUX6_LVDS_ANA                        0x%x"),
	_T("ARM_EXP_0_TRG_3_4AC_AUX7_LVDS_ANA                        0x%x"),
	_T("ARM_EXP_0_TRG_0_AC0_OPTO_DIG                             0x%x"),
	_T("ARM_EXP_0_TRG_1_AC0_OPTO_DIG                             0x%x"),
	_T("ARM_EXP_0_TRG_0_AC1_OPTO_DIG                             0x%x"),
	_T("ARM_EXP_0_TRG_1_AC1_OPTO_DIG                             0x%x"),
	_T("ARM_EXP_0_TRG_0_AC2_OPTO_DIG                             0x%x"),
	_T("ARM_EXP_0_TRG_1_AC2_OPTO_DIG                             0x%x"),
	_T("ARM_EXP_0_TRG_0_AC3_OPTO_DIG                             0x%x"),
	_T("ARM_EXP_0_TRG_1_AC3_OPTO_DIG                             0x%x"),
	_T("ARM_EXP_0_TRG_1_AC0_TTL_DIG                              0x%x"),
	_T("ARM_EXP_0_TRG_1_AC1_TTL_DIG                              0x%x"),
	_T("ARM_EXP_0_TRG_1_AC2_TTL_DIG                              0x%x"),
	_T("ARM_EXP_0_TRG_1_AC3_TTL_DIG                              0x%x"),
	_T("ARM_EXP_0_TRG_0_AC0_LVDS_DIG                             0x%x"),
	_T("ARM_EXP_0_TRG_0_AC1_LVDS_DIG                             0x%x"),
	_T("ARM_EXP_0_TRG_0_AC2_LVDS_DIG                             0x%x"),
	_T("ARM_EXP_0_TRG_0_AC3_LVDS_DIG                             0x%x"),
	_T("ARM_EXP_0_TRG_2_AC0_AUX1_TTL_DIG                         0x%x"),
	_T("ARM_EXP_0_TRG_3_AC0_AUX2_TTL_DIG                         0x%x"),
	_T("ARM_EXP_0_TRG_2_AC1_AUX1_TTL_DIG                         0x%x"),
	_T("ARM_EXP_0_TRG_3_AC1_AUX2_TTL_DIG                         0x%x"),
	_T("ARM_EXP_0_TRG_2_AC2_AUX1_TTL_DIG                         0x%x"),
	_T("ARM_EXP_0_TRG_3_AC2_AUX2_TTL_DIG                         0x%x"),
	_T("ARM_EXP_0_TRG_2_AC3_AUX1_TTL_DIG                         0x%x"),
	_T("ARM_EXP_0_TRG_3_AC3_AUX2_TTL_DIG                         0x%x"),
	_T("ARM_EXP_0_TRG_2_AC0_AUX1_LVDS_DIG                        0x%x"),
	_T("ARM_EXP_0_TRG_3_AC0_AUX2_LVDS_DIG                        0x%x"),
	_T("ARM_EXP_0_TRG_2_AC1_AUX1_LVDS_DIG                        0x%x"),
	_T("ARM_EXP_0_TRG_3_AC1_AUX2_LVDS_DIG                        0x%x"),
	_T("ARM_EXP_0_TRG_2_AC2_AUX1_LVDS_DIG                        0x%x"),
	_T("ARM_EXP_0_TRG_3_AC2_AUX2_LVDS_DIG                        0x%x"),
	_T("ARM_EXP_0_TRG_2_AC3_AUX1_LVDS_DIG                        0x%x"),
	_T("ARM_EXP_0_TRG_3_AC3_AUX2_LVDS_DIG                        0x%x"),
	_T("ARM_EXP_0_TRG_AC0_LVDS_CL_SOL_BASE_ROTARY_ENCODER_FOW    0x%x"),
	_T("ARM_EXP_0_TRG_AC0_LVDS_CL_SOL_BASE_ROTARY_ENCODER_REV    0x%x"),
	_T("ARM_EXP_0_TRG_AC0_LVDS_CL_SOL_EVCL_ROTARY_ENCODER_FOW    0x%x"),
	_T("ARM_EXP_0_TRG_AC0_LVDS_CL_SOL_EVCL_ROTARY_ENCODER_REV    0x%x"),
	_T("ARM_EXP_0_TRG_AC0_LVDS_CL_ROTARY_ENCODER_FOW             0x%x"),
	_T("ARM_EXP_0_TRG_AC0_LVDS_CL_ROTARY_ENCODER_REV             0x%x"),
	_T("ARM_EXP_0_TRG_AC1_LVDS_CL_SOL_EVCL_ROTARY_ENCODER_FOW    0x%x"),
	_T("ARM_EXP_0_TRG_AC1_LVDS_CL_SOL_EVCL_ROTARY_ENCODER_REV    0x%x"),
	_T("ARM_EXP_0_TRG_AC1_LVDS_CL_ROTARY_ENCODER_FOW             0x%x"),
	_T("ARM_EXP_0_TRG_AC1_LVDS_CL_ROTARY_ENCODER_REV             0x%x"),
	_T("ARM_EXP_0_TRG_AC0_LVDS_CL_RADIENT_ROTARY_ENCODER_FOW     0x%x"),
	_T("ARM_EXP_0_TRG_AC0_LVDS_CL_RADIENT_ROTARY_ENCODER_REV     0x%x"),
	_T("ARM_EXP_0_TRG_AC1_LVDS_CL_RADIENT_ROTARY_ENCODER_FOW     0x%x"),
	_T("ARM_EXP_0_TRG_AC1_LVDS_CL_RADIENT_ROTARY_ENCODER_REV     0x%x"),
	_T("ARM_EXP_0_TRG_AC2_LVDS_CL_RADIENT_ROTARY_ENCODER_FOW     0x%x"),
	_T("ARM_EXP_0_TRG_AC2_LVDS_CL_RADIENT_ROTARY_ENCODER_REV     0x%x"),
	_T("ARM_EXP_0_TRG_AC3_LVDS_CL_RADIENT_ROTARY_ENCODER_FOW     0x%x"),
	_T("ARM_EXP_0_TRG_AC3_LVDS_CL_RADIENT_ROTARY_ENCODER_REV     0x%x"),
	_T("ARM_EXP_0_TRG_AC0_LVDS_ANA_ROTARY_ENCODER_FOW            0x%x"),
	_T("ARM_EXP_0_TRG_AC0_LVDS_ANA_ROTARY_ENCODER_REV            0x%x"),
	_T("ARM_EXP_0_TRG_AC1_LVDS_ANA_ROTARY_ENCODER_FOW            0x%x"),
	_T("ARM_EXP_0_TRG_AC1_LVDS_ANA_ROTARY_ENCODER_REV            0x%x"),
	_T("ARM_EXP_0_TRG_AC2_LVDS_ANA_ROTARY_ENCODER_FOW            0x%x"),
	_T("ARM_EXP_0_TRG_AC2_LVDS_ANA_ROTARY_ENCODER_REV            0x%x"),
	_T("ARM_EXP_0_TRG_AC3_LVDS_ANA_ROTARY_ENCODER_FOW            0x%x"),
	_T("ARM_EXP_0_TRG_AC3_LVDS_ANA_ROTARY_ENCODER_REV            0x%x"),
	_T("ARM_EXP_0_TRG_AC0_LVDS_DIG_ROTARY_ENCODER_FOW            0x%x"),
	_T("ARM_EXP_0_TRG_AC0_LVDS_DIG_ROTARY_ENCODER_REV            0x%x"),
	_T("ARM_EXP_0_TRG_AC1_LVDS_DIG_ROTARY_ENCODER_FOW            0x%x"),
	_T("ARM_EXP_0_TRG_AC1_LVDS_DIG_ROTARY_ENCODER_REV            0x%x"),
	_T("ARM_EXP_0_TRG_AC2_LVDS_DIG_ROTARY_ENCODER_FOW            0x%x"),
	_T("ARM_EXP_0_TRG_AC2_LVDS_DIG_ROTARY_ENCODER_REV            0x%x"),
	_T("ARM_EXP_0_TRG_AC3_LVDS_DIG_ROTARY_ENCODER_FOW            0x%x"),
	_T("ARM_EXP_0_TRG_AC3_LVDS_DIG_ROTARY_ENCODER_REV            0x%x"),
	_T("ARM_EXP_0_TRG_LVDS_ROTARY_ENCODER                        0x%x"),
	_T("EXP_NOCOMBINE                                            0x%x"),
	_T("EXP_COMBINE_XOR_T1                                       0x%x"),
	_T("EXP_COMBINE_XOR_T0AC1                                    0x%x"),
	_T("EXP_COMBINE_T1                                           0x%x"),
	_T("EXP_0_NOCOMBINE_AC0_CL_SOL_BASE_HD_CC                    0x%x"),
	_T("EXP_0_COMBINE_XOR_T1_AC0_CL_SOL_BASE_HD_CC               0x%x"),
	_T("EXP_0_COMBINE_T1_AC0_CL_SOL_BASE_HD_CC                   0x%x"),
	_T("EXP_0_NOCOMBINE_AC0_CL_HD_CC                             0x%x"),
	_T("EXP_0_COMBINE_XOR_T1_AC0_CL_HD_CC                        0x%x"),
	_T("EXP_0_COMBINE_T1_AC0_CL_HD_CC                            0x%x"),
	_T("EXP_0_NOCOMBINE_AC1_CL_HD_CC                             0x%x"),
	_T("EXP_0_COMBINE_XOR_T1_AC1_CL_HD_CC                        0x%x"),
	_T("EXP_0_COMBINE_T1_AC1_CL_HD_CC                            0x%x"),
	_T("EXP_0_NOCOMBINE_CL_CC                                    0x%x"),
	_T("EXP_0_COMBINE_XOR_T1_CL_CC                               0x%x"),
	_T("EXP_0_COMBINE_T1_CL_CC                                   0x%x"),
	_T("EXP_0_NOCOMBINE_AC0_ANA_HD_DVI                           0x%x"),
	_T("EXP_0_COMBINE_XOR_T1_AC0_ANA_HD_DVI                      0x%x"),
	_T("EXP_0_COMBINE_T1_AC0_ANA_HD_DVI                          0x%x"),
	_T("EXP_0_NOCOMBINE_AC1_ANA_HD_DVI                           0x%x"),
	_T("EXP_0_COMBINE_XOR_T1_AC1_ANA_HD_DVI                      0x%x"),
	_T("EXP_0_COMBINE_T1_AC1_ANA_HD_DVI                          0x%x"),
	_T("EXP_0_NOCOMBINE_AC2_ANA_HD_DVI                           0x%x"),
	_T("EXP_0_COMBINE_XOR_T1_AC2_ANA_HD_DVI                      0x%x"),
	_T("EXP_0_COMBINE_T1_AC2_ANA_HD_DVI                          0x%x"),
	_T("EXP_0_NOCOMBINE_AC3_ANA_HD_DVI                           0x%x"),
	_T("EXP_0_COMBINE_XOR_T1_AC3_ANA_HD_DVI                      0x%x"),
	_T("EXP_0_COMBINE_T1_AC3_ANA_HD_DVI                          0x%x"),
	_T("EXP_0_NOCOMBINE_AC0_ANA_DVI                              0x%x"),
	_T("EXP_0_COMBINE_XOR_T1_AC0_ANA_DVI                         0x%x"),
	_T("EXP_0_COMBINE_T1_AC0_ANA_DVI                             0x%x"),
	_T("EXP_0_NOCOMBINE_AC1_ANA_DVI                              0x%x"),
	_T("EXP_0_COMBINE_XOR_T1_AC1_ANA_DVI                         0x%x"),
	_T("EXP_0_COMBINE_T1_AC1_ANA_DVI                             0x%x"),
	_T("EXP_0_NOCOMBINE_AC2_ANA_DVI                              0x%x"),
	_T("EXP_0_COMBINE_XOR_T1_AC2_ANA_DVI                         0x%x"),
	_T("EXP_0_COMBINE_T1_AC2_ANA_DVI                             0x%x"),
	_T("EXP_0_NOCOMBINE_AC3_ANA_DVI                              0x%x"),
	_T("EXP_0_COMBINE_XOR_T1_AC3_ANA_DVI                         0x%x"),
	_T("EXP_0_COMBINE_T1_AC3_ANA_DVI                             0x%x"),
	_T("EXP_0_NOCOMBINE_AC0_DIG_HD_DCON                          0x%x"),
	_T("EXP_0_COMBINE_XOR_T1_AC0_DIG_HD_DCON                     0x%x"),
	_T("EXP_0_COMBINE_T1_AC0_DIG_HD_DCON                         0x%x"),
	_T("EXP_0_NOCOMBINE_AC1_DIG_HD_DCON                          0x%x"),
	_T("EXP_0_COMBINE_XOR_T1_AC1_DIG_HD_DCON                     0x%x"),
	_T("EXP_0_COMBINE_T1_AC1_DIG_HD_DCON                         0x%x"),
	_T("EXP_0_NOCOMBINE_AC2_DIG_HD_DCON                          0x%x"),
	_T("EXP_0_COMBINE_XOR_T1_AC2_DIG_HD_DCON                     0x%x"),
	_T("EXP_0_COMBINE_T1_AC2_DIG_HD_DCON                         0x%x"),
	_T("EXP_0_NOCOMBINE_AC3_DIG_HD_DCON                          0x%x"),
	_T("EXP_0_COMBINE_XOR_T1_AC3_DIG_HD_DCON                     0x%x"),
	_T("EXP_0_COMBINE_T1_AC3_DIG_HD_DCON                         0x%x"),
	_T("EXP_0_NOCOMBINE_AC0_DIG_DCON                             0x%x"),
	_T("EXP_0_COMBINE_XOR_T1_AC0_DIG_DCON                        0x%x"),
	_T("EXP_0_COMBINE_T1_AC0_DIG_DCON                            0x%x"),
	_T("EXP_0_NOCOMBINE_AC1_DIG_DCON                             0x%x"),
	_T("EXP_0_COMBINE_XOR_T1_AC1_DIG_DCON                        0x%x"),
	_T("EXP_0_COMBINE_T1_AC1_DIG_DCON                            0x%x"),
	_T("EXP_0_NOCOMBINE_AC2_DIG_DCON                             0x%x"),
	_T("EXP_0_COMBINE_XOR_T1_AC2_DIG_DCON                        0x%x"),
	_T("EXP_0_COMBINE_T1_AC2_DIG_DCON                            0x%x"),
	_T("EXP_0_NOCOMBINE_AC3_DIG_DCON                             0x%x"),
	_T("EXP_0_COMBINE_XOR_T1_AC3_DIG_DCON                        0x%x"),
	_T("EXP_0_COMBINE_T1_AC3_DIG_DCON                            0x%x"),
	_T("EXP_PRESCALE1_1                                          0x%x"),
	_T("EXP_PRESCALE1_2                                          0x%x"),
	_T("EXP_PRESCALE1_4                                          0x%x"),
	_T("EXP_PRESCALE1_8                                          0x%x"),
	_T("EXP_PRESCALE1_16                                         0x%x"),
	_T("EXP_TRG_TTL_TIMER1                                       0x%x"),
	_T("EXP_TRG_TTL_TIMER2                                       0x%x"),
	_T("EXP_TRG_CNTEQ0_TIMER1                                    0x%x"),
	_T("EXP_TRG_TIMER0_2                                         0x%x"),
	_T("EXP_TRG_TIMER2_2                                         0x%x"),
	_T("EXP_TRG_TIMER3_2                                         0x%x"),
	_T("EXP_1_TRG_TIMER1_AC0                                     0x%x"),
	_T("EXP_1_TRG_TIMER1_AC1                                     0x%x"),
	_T("EXP_1_TRG_TIMER1_AC2                                     0x%x"),
	_T("EXP_1_TRG_TIMER1_AC3                                     0x%x"),
	_T("EXP_1_TRG_2_AC01_OPTO_CL_SOL_EVCL                        0x%x"),
	_T("EXP_1_TRG_3_AC01_OPTO_CL_SOL_EVCL                        0x%x"),
	_T("EXP_1_TRG_2_AC01_OPTO_CL                                 0x%x"),
	_T("EXP_1_TRG_3_AC01_OPTO_CL                                 0x%x"),
	_T("EXP_1_TRG_0_AC0_OPTO_CL_SOL_BASE                         0x%x"),
	_T("EXP_1_TRG_1_AC0_OPTO_CL_SOL_BASE                         0x%x"),
	_T("EXP_1_TRG_0_AC0_OPTO_CL_SOL_EVCL                         0x%x"),
	_T("EXP_1_TRG_1_AC0_OPTO_CL_SOL_EVCL                         0x%x"),
	_T("EXP_1_TRG_0_AC0_OPTO_CL                                  0x%x"),
	_T("EXP_1_TRG_1_AC0_OPTO_CL                                  0x%x"),
	_T("EXP_1_TRG_0_AC1_OPTO_CL_SOL_EVCL                         0x%x"),
	_T("EXP_1_TRG_1_AC1_OPTO_CL_SOL_EVCL                         0x%x"),
	_T("EXP_1_TRG_0_AC1_OPTO_CL                                  0x%x"),
	_T("EXP_1_TRG_1_AC1_OPTO_CL                                  0x%x"),
	_T("EXP_1_TRG_0_4AC_CON0_OPTO_CL_RADIENT                     0x%x"),
	_T("EXP_1_TRG_1_4AC_CON0_OPTO_CL_RADIENT                     0x%x"),
	_T("EXP_1_TRG_0_4AC_CON2_OPTO_CL_RADIENT                     0x%x"),
	_T("EXP_1_TRG_1_4AC_CON2_OPTO_CL_RADIENT                     0x%x"),
	_T("EXP_1_TRG_2_4AC_CON1_OPTO_CL_RADIENT                     0x%x"),
	_T("EXP_1_TRG_3_4AC_CON1_OPTO_CL_RADIENT                     0x%x"),
	_T("EXP_1_TRG_2_4AC_CON3_OPTO_CL_RADIENT                     0x%x"),
	_T("EXP_1_TRG_3_4AC_CON3_OPTO_CL_RADIENT                     0x%x"),
	_T("EXP_1_TRG_0_AC0_TTL_CL_SOL_BASE                          0x%x"),
	_T("EXP_1_TRG_1_AC0_TTL_CL_SOL_BASE                          0x%x"),
	_T("EXP_1_TRG_0_AC0_TTL_CL_SOL_EVCL                          0x%x"),
	_T("EXP_1_TRG_1_AC0_TTL_CL_SOL_EVCL                          0x%x"),
	_T("EXP_1_TRG_0_AC0_TTL_CL                                   0x%x"),
	_T("EXP_1_TRG_1_AC0_TTL_CL                                   0x%x"),
	_T("EXP_1_TRG_0_AC1_TTL_CL_SOL_EVCL                          0x%x"),
	_T("EXP_1_TRG_1_AC1_TTL_CL_SOL_EVCL                          0x%x"),
	_T("EXP_1_TRG_0_AC1_TTL_CL                                   0x%x"),
	_T("EXP_1_TRG_1_AC1_TTL_CL                                   0x%x"),
	_T("EXP_1_TRG_2_AC01_TTL_CL_SOL_EVCL                         0x%x"),
	_T("EXP_1_TRG_3_AC01_TTL_CL_SOL_EVCL                         0x%x"),
	_T("EXP_1_TRG_2_AC01_TTL_CL                                  0x%x"),
	_T("EXP_1_TRG_3_AC01_TTL_CL                                  0x%x"),
	_T("EXP_1_TRG_0_4AC_CON0_TTL_CL_RADIENT                      0x%x"),
	_T("EXP_1_TRG_1_4AC_CON0_TTL_CL_RADIENT                      0x%x"),
	_T("EXP_1_TRG_0_4AC_CON1_TTL_CL_RADIENT                      0x%x"),
	_T("EXP_1_TRG_1_4AC_CON1_TTL_CL_RADIENT                      0x%x"),
	_T("EXP_1_TRG_0_4AC_CON2_TTL_CL_RADIENT                      0x%x"),
	_T("EXP_1_TRG_1_4AC_CON2_TTL_CL_RADIENT                      0x%x"),
	_T("EXP_1_TRG_0_4AC_CON3_TTL_CL_RADIENT                      0x%x"),
	_T("EXP_1_TRG_1_4AC_CON3_TTL_CL_RADIENT                      0x%x"),
	_T("EXP_1_TRG_2_4AC_CON0_TTL_CL_RADIENT                      0x%x"),
	_T("EXP_1_TRG_3_4AC_CON1_TTL_CL_RADIENT                      0x%x"),
	_T("EXP_1_TRG_2_4AC_CON2_TTL_CL_RADIENT                      0x%x"),
	_T("EXP_1_TRG_3_4AC_CON3_TTL_CL_RADIENT                      0x%x"),
	_T("EXP_1_TRG_0_AC0_LVDS_CL_SOL_BASE                         0x%x"),
	_T("EXP_1_TRG_1_AC0_LVDS_CL_SOL_BASE                         0x%x"),
	_T("EXP_1_TRG_0_AC0_LVDS_CL_SOL_EVCL                         0x%x"),
	_T("EXP_1_TRG_1_AC0_LVDS_CL_SOL_EVCL                         0x%x"),
	_T("EXP_1_TRG_0_AC0_LVDS_CL                                  0x%x"),
	_T("EXP_1_TRG_1_AC0_LVDS_CL                                  0x%x"),
	_T("EXP_1_TRG_0_AC1_LVDS_CL_SOL_EVCL                         0x%x"),
	_T("EXP_1_TRG_1_AC1_LVDS_CL_SOL_EVCL                         0x%x"),
	_T("EXP_1_TRG_0_AC1_LVDS_CL                                  0x%x"),
	_T("EXP_1_TRG_1_AC1_LVDS_CL                                  0x%x"),
	_T("EXP_1_TRG_2_AC01_LVDS_CL_SOL_EVCL                        0x%x"),
	_T("EXP_1_TRG_3_AC01_LVDS_CL_SOL_EVCL                        0x%x"),
	_T("EXP_1_TRG_2_AC01_LVDS_CL                                 0x%x"),
	_T("EXP_1_TRG_3_AC01_LVDS_CL                                 0x%x"),
	_T("EXP_1_TRG_0_4AC_CON0_LVDS_CL_RADIENT                     0x%x"),
	_T("EXP_1_TRG_1_4AC_CON0_LVDS_CL_RADIENT                     0x%x"),
	_T("EXP_1_TRG_0_4AC_CON2_LVDS_CL_RADIENT                     0x%x"),
	_T("EXP_1_TRG_1_4AC_CON2_LVDS_CL_RADIENT                     0x%x"),
	_T("EXP_1_TRG_2_4AC_CON1_LVDS_CL_RADIENT                     0x%x"),
	_T("EXP_1_TRG_3_4AC_CON1_LVDS_CL_RADIENT                     0x%x"),
	_T("EXP_1_TRG_2_4AC_CON3_LVDS_CL_RADIENT                     0x%x"),
	_T("EXP_1_TRG_3_4AC_CON3_LVDS_CL_RADIENT                     0x%x"),
	_T("EXP_1_TRG_1_AC0_OPTO_ANA                                 0x%x"),
	_T("EXP_1_TRG_1_AC1_OPTO_ANA                                 0x%x"),
	_T("EXP_1_TRG_1_AC2_OPTO_ANA                                 0x%x"),
	_T("EXP_1_TRG_1_AC3_OPTO_ANA                                 0x%x"),
	_T("EXP_1_TRG_0_AC0_TTL_ANA                                  0x%x"),
	_T("EXP_1_TRG_0_AC1_TTL_ANA                                  0x%x"),
	_T("EXP_1_TRG_0_AC2_TTL_ANA                                  0x%x"),
	_T("EXP_1_TRG_0_AC3_TTL_ANA                                  0x%x"),
	_T("EXP_1_TRG_2_4AC_AUX0_TTL_ANA                             0x%x"),
	_T("EXP_1_TRG_3_4AC_AUX1_TTL_ANA                             0x%x"),
	_T("EXP_1_TRG_2_4AC_AUX2_TTL_ANA                             0x%x"),
	_T("EXP_1_TRG_3_4AC_AUX3_TTL_ANA                             0x%x"),
	_T("EXP_1_TRG_2_4AC_AUX4_TTL_ANA                             0x%x"),
	_T("EXP_1_TRG_3_4AC_AUX5_TTL_ANA                             0x%x"),
	_T("EXP_1_TRG_2_4AC_AUX6_TTL_ANA                             0x%x"),
	_T("EXP_1_TRG_3_4AC_AUX7_TTL_ANA                             0x%x"),
	_T("EXP_1_TRG_2_4AC_AUX0_LVDS_ANA                            0x%x"),
	_T("EXP_1_TRG_3_4AC_AUX1_LVDS_ANA                            0x%x"),
	_T("EXP_1_TRG_2_4AC_AUX2_LVDS_ANA                            0x%x"),
	_T("EXP_1_TRG_3_4AC_AUX3_LVDS_ANA                            0x%x"),
	_T("EXP_1_TRG_2_4AC_AUX4_LVDS_ANA                            0x%x"),
	_T("EXP_1_TRG_3_4AC_AUX5_LVDS_ANA                            0x%x"),
	_T("EXP_1_TRG_2_4AC_AUX6_LVDS_ANA                            0x%x"),
	_T("EXP_1_TRG_3_4AC_AUX7_LVDS_ANA                            0x%x"),
	_T("EXP_1_TRG_0_AC0_OPTO_DIG                                 0x%x"),
	_T("EXP_1_TRG_1_AC0_OPTO_DIG                                 0x%x"),
	_T("EXP_1_TRG_0_AC1_OPTO_DIG                                 0x%x"),
	_T("EXP_1_TRG_1_AC1_OPTO_DIG                                 0x%x"),
	_T("EXP_1_TRG_0_AC2_OPTO_DIG                                 0x%x"),
	_T("EXP_1_TRG_1_AC2_OPTO_DIG                                 0x%x"),
	_T("EXP_1_TRG_0_AC3_OPTO_DIG                                 0x%x"),
	_T("EXP_1_TRG_1_AC3_OPTO_DIG                                 0x%x"),
	_T("EXP_1_TRG_1_AC0_TTL_DIG                                  0x%x"),
	_T("EXP_1_TRG_1_AC1_TTL_DIG                                  0x%x"),
	_T("EXP_1_TRG_1_AC2_TTL_DIG                                  0x%x"),
	_T("EXP_1_TRG_1_AC3_TTL_DIG                                  0x%x"),
	_T("EXP_1_TRG_0_AC0_LVDS_DIG                                 0x%x"),
	_T("EXP_1_TRG_0_AC1_LVDS_DIG                                 0x%x"),
	_T("EXP_1_TRG_0_AC2_LVDS_DIG                                 0x%x"),
	_T("EXP_1_TRG_0_AC3_LVDS_DIG                                 0x%x"),
	_T("EXP_1_TRG_2_AC0_AUX1_TTL_DIG                             0x%x"),
	_T("EXP_1_TRG_3_AC0_AUX2_TTL_DIG                             0x%x"),
	_T("EXP_1_TRG_2_AC1_AUX1_TTL_DIG                             0x%x"),
	_T("EXP_1_TRG_3_AC1_AUX2_TTL_DIG                             0x%x"),
	_T("EXP_1_TRG_2_AC2_AUX1_TTL_DIG                             0x%x"),
	_T("EXP_1_TRG_3_AC2_AUX2_TTL_DIG                             0x%x"),
	_T("EXP_1_TRG_2_AC3_AUX1_TTL_DIG                             0x%x"),
	_T("EXP_1_TRG_3_AC3_AUX2_TTL_DIG                             0x%x"),
	_T("EXP_1_TRG_2_AC0_AUX1_LVDS_DIG                            0x%x"),
	_T("EXP_1_TRG_3_AC0_AUX2_LVDS_DIG                            0x%x"),
	_T("EXP_1_TRG_2_AC1_AUX1_LVDS_DIG                            0x%x"),
	_T("EXP_1_TRG_3_AC1_AUX2_LVDS_DIG                            0x%x"),
	_T("EXP_1_TRG_2_AC2_AUX1_LVDS_DIG                            0x%x"),
	_T("EXP_1_TRG_3_AC2_AUX2_LVDS_DIG                            0x%x"),
	_T("EXP_1_TRG_2_AC3_AUX1_LVDS_DIG                            0x%x"),
	_T("EXP_1_TRG_3_AC3_AUX2_LVDS_DIG                            0x%x"),
	_T("EXP_1_TRG_AC0_LVDS_CL_SOL_BASE_ROTARY_ENCODER_FOW        0x%x"),
	_T("EXP_1_TRG_AC0_LVDS_CL_SOL_BASE_ROTARY_ENCODER_REV        0x%x"),
	_T("EXP_1_TRG_AC0_LVDS_CL_SOL_EVCL_ROTARY_ENCODER_FOW        0x%x"),
	_T("EXP_1_TRG_AC0_LVDS_CL_SOL_EVCL_ROTARY_ENCODER_REV        0x%x"),
	_T("EXP_1_TRG_AC0_LVDS_CL_ROTARY_ENCODER_FOW                 0x%x"),
	_T("EXP_1_TRG_AC0_LVDS_CL_ROTARY_ENCODER_REV                 0x%x"),
	_T("EXP_1_TRG_AC1_LVDS_CL_SOL_EVCL_ROTARY_ENCODER_FOW        0x%x"),
	_T("EXP_1_TRG_AC1_LVDS_CL_SOL_EVCL_ROTARY_ENCODER_REV        0x%x"),
	_T("EXP_1_TRG_AC1_LVDS_CL_ROTARY_ENCODER_FOW                 0x%x"),
	_T("EXP_1_TRG_AC1_LVDS_CL_ROTARY_ENCODER_REV                 0x%x"),
	_T("EXP_1_TRG_AC0_LVDS_CL_RADIENT_ROTARY_ENCODER_FOW         0x%x"),
	_T("EXP_1_TRG_AC0_LVDS_CL_RADIENT_ROTARY_ENCODER_REV         0x%x"),
	_T("EXP_1_TRG_AC1_LVDS_CL_RADIENT_ROTARY_ENCODER_FOW         0x%x"),
	_T("EXP_1_TRG_AC1_LVDS_CL_RADIENT_ROTARY_ENCODER_REV         0x%x"),
	_T("EXP_1_TRG_AC2_LVDS_CL_RADIENT_ROTARY_ENCODER_FOW         0x%x"),
	_T("EXP_1_TRG_AC2_LVDS_CL_RADIENT_ROTARY_ENCODER_REV         0x%x"),
	_T("EXP_1_TRG_AC3_LVDS_CL_RADIENT_ROTARY_ENCODER_FOW         0x%x"),
	_T("EXP_1_TRG_AC3_LVDS_CL_RADIENT_ROTARY_ENCODER_REV         0x%x"),
	_T("EXP_1_TRG_AC0_LVDS_ANA_ROTARY_ENCODER_FOW                0x%x"),
	_T("EXP_1_TRG_AC0_LVDS_ANA_ROTARY_ENCODER_REV                0x%x"),
	_T("EXP_1_TRG_AC1_LVDS_ANA_ROTARY_ENCODER_FOW                0x%x"),
	_T("EXP_1_TRG_AC1_LVDS_ANA_ROTARY_ENCODER_REV                0x%x"),
	_T("EXP_1_TRG_AC2_LVDS_ANA_ROTARY_ENCODER_FOW                0x%x"),
	_T("EXP_1_TRG_AC2_LVDS_ANA_ROTARY_ENCODER_REV                0x%x"),
	_T("EXP_1_TRG_AC3_LVDS_ANA_ROTARY_ENCODER_FOW                0x%x"),
	_T("EXP_1_TRG_AC3_LVDS_ANA_ROTARY_ENCODER_REV                0x%x"),
	_T("EXP_1_TRG_AC0_LVDS_DIG_ROTARY_ENCODER_FOW                0x%x"),
	_T("EXP_1_TRG_AC0_LVDS_DIG_ROTARY_ENCODER_REV                0x%x"),
	_T("EXP_1_TRG_AC1_LVDS_DIG_ROTARY_ENCODER_FOW                0x%x"),
	_T("EXP_1_TRG_AC1_LVDS_DIG_ROTARY_ENCODER_REV                0x%x"),
	_T("EXP_1_TRG_AC2_LVDS_DIG_ROTARY_ENCODER_FOW                0x%x"),
	_T("EXP_1_TRG_AC2_LVDS_DIG_ROTARY_ENCODER_REV                0x%x"),
	_T("EXP_1_TRG_AC3_LVDS_DIG_ROTARY_ENCODER_FOW                0x%x"),
	_T("EXP_1_TRG_AC3_LVDS_DIG_ROTARY_ENCODER_REV                0x%x"),
	_T("EXP_1_TRG_LVDS_ROTARY_ENCODER                            0x%x"),
	_T("EXP_CLK_2_CLKGEN                                         0x%x"),
	_T("EXP_CLK_2_HS                                             0x%x"),
	_T("EXP_CLK_2_VS                                             0x%x"),
	_T("EXP_CLK_2_TIMER0                                         0x%x"),
	_T("EXP_CLK_2_TIMER2                                         0x%x"),
	_T("EXP_CLK_2_TIMER3                                         0x%x"),
	_T("EXP_CLK_2_AUXIN1_LVDS                                    0x%x"),
	_T("EXP_CLK_2_AUXIN3_LVDS                                    0x%x"),
	_T("EXP_CLK_2_AUXIN1_AC0_CL                                  0x%x"),
	_T("EXP_CLK_2_AUXIN3_AC1_CL                                  0x%x"),
	_T("EXP_CLK_2_AUXIN1_AC0_CL_SOL_BASE_EVCL_RADIENT            0x%x"),
	_T("EXP_CLK_2_AUXIN3_AC1_CL_SOL_BASE_EVCL_RADIENT            0x%x"),
	_T("EXP_CLK_2_AUXIN1_AC2_CL_RADIENT                          0x%x"),
	_T("EXP_CLK_2_AUXIN3_AC3_CL_RADIENT                          0x%x"),
	_T("EXP_CLK_2_AUXIN1_AC0_ANA                                 0x%x"),
	_T("EXP_CLK_2_AUXIN3_AC1_ANA                                 0x%x"),
	_T("EXP_CLK_2_AUXIN5_AC2_ANA                                 0x%x"),
	_T("EXP_CLK_2_AUXIN7_AC3_ANA                                 0x%x"),
	_T("EXP_CLK_2_AUXIN2_AC0_DIG                                 0x%x"),
	_T("EXP_CLK_2_AUXIN2_AC1_DIG                                 0x%x"),
	_T("EXP_CLK_2_AUXIN2_AC2_DIG                                 0x%x"),
	_T("EXP_CLK_2_AUXIN2_AC3_DIG                                 0x%x"),
	_T("EXP_CLK_2_AUXIN1_AC0_CL_SOL_EVCL                         0x%x"),
	_T("EXP_CLK_2_AUXIN3_AC1_CL_SOL_EVCL                         0x%x"),
	_T("ARM_EXP_1_CNTEQ0                                         0x%x"),
	_T("ARM_EXP_1_SOFTWARE                                       0x%x"),
	_T("ARM_EXP_1_TIMER0                                         0x%x"),
	_T("ARM_EXP_1_TIMER2                                         0x%x"),
	_T("ARM_EXP_1_TIMER3                                         0x%x"),
	_T("ARM_EXP_1_TRG_TIMER1_AC0                                 0x%x"),
	_T("ARM_EXP_1_TRG_TIMER1_AC1                                 0x%x"),
	_T("ARM_EXP_1_TRG_TIMER1_AC2                                 0x%x"),
	_T("ARM_EXP_1_TRG_TIMER1_AC3                                 0x%x"),
	_T("ARM_EXP_1_HS_PSG                                         0x%x"),
	_T("ARM_EXP_1_VS_PSG                                         0x%x"),
	_T("ARM_EXP_1_TRG_2_AC01_OPTO_CL_SOL_EVCL                    0x%x"),
	_T("ARM_EXP_1_TRG_3_AC01_OPTO_CL_SOL_EVCL                    0x%x"),
	_T("ARM_EXP_1_TRG_2_AC01_OPTO_CL                             0x%x"),
	_T("ARM_EXP_1_TRG_3_AC01_OPTO_CL                             0x%x"),
	_T("ARM_EXP_1_TRG_0_AC0_OPTO_CL_SOL_BASE                     0x%x"),
	_T("ARM_EXP_1_TRG_1_AC0_OPTO_CL_SOL_BASE                     0x%x"),
	_T("ARM_EXP_1_TRG_0_AC0_OPTO_CL_SOL_EVCL                     0x%x"),
	_T("ARM_EXP_1_TRG_1_AC0_OPTO_CL_SOL_EVCL                     0x%x"),
	_T("ARM_EXP_1_TRG_0_AC0_OPTO_CL                              0x%x"),
	_T("ARM_EXP_1_TRG_1_AC0_OPTO_CL                              0x%x"),
	_T("ARM_EXP_1_TRG_0_AC1_OPTO_CL_SOL_EVCL                     0x%x"),
	_T("ARM_EXP_1_TRG_1_AC1_OPTO_CL_SOL_EVCL                     0x%x"),
	_T("ARM_EXP_1_TRG_0_AC1_OPTO_CL                              0x%x"),
	_T("ARM_EXP_1_TRG_1_AC1_OPTO_CL                              0x%x"),
	_T("ARM_EXP_1_TRG_0_4AC_CON0_OPTO_CL_RADIENT                 0x%x"),
	_T("ARM_EXP_1_TRG_1_4AC_CON0_OPTO_CL_RADIENT                 0x%x"),
	_T("ARM_EXP_1_TRG_0_4AC_CON2_OPTO_CL_RADIENT                 0x%x"),
	_T("ARM_EXP_1_TRG_1_4AC_CON2_OPTO_CL_RADIENT                 0x%x"),
	_T("ARM_EXP_1_TRG_2_4AC_CON1_OPTO_CL_RADIENT                 0x%x"),
	_T("ARM_EXP_1_TRG_3_4AC_CON1_OPTO_CL_RADIENT                 0x%x"),
	_T("ARM_EXP_1_TRG_2_4AC_CON3_OPTO_CL_RADIENT                 0x%x"),
	_T("ARM_EXP_1_TRG_3_4AC_CON3_OPTO_CL_RADIENT                 0x%x"),
	_T("ARM_EXP_1_TRG_0_AC0_TTL_CL_SOL_BASE                      0x%x"),
	_T("ARM_EXP_1_TRG_1_AC0_TTL_CL_SOL_BASE                      0x%x"),
	_T("ARM_EXP_1_TRG_0_AC0_TTL_CL_SOL_EVCL                      0x%x"),
	_T("ARM_EXP_1_TRG_1_AC0_TTL_CL_SOL_EVCL                      0x%x"),
	_T("ARM_EXP_1_TRG_0_AC0_TTL_CL                               0x%x"),
	_T("ARM_EXP_1_TRG_1_AC0_TTL_CL                               0x%x"),
	_T("ARM_EXP_1_TRG_0_AC1_TTL_CL_SOL_EVCL                      0x%x"),
	_T("ARM_EXP_1_TRG_1_AC1_TTL_CL_SOL_EVCL                      0x%x"),
	_T("ARM_EXP_1_TRG_0_AC1_TTL_CL                               0x%x"),
	_T("ARM_EXP_1_TRG_1_AC1_TTL_CL                               0x%x"),
	_T("ARM_EXP_1_TRG_2_AC01_TTL_CL_SOL_EVCL                     0x%x"),
	_T("ARM_EXP_1_TRG_3_AC01_TTL_CL_SOL_EVCL                     0x%x"),
	_T("ARM_EXP_1_TRG_2_AC01_TTL_CL                              0x%x"),
	_T("ARM_EXP_1_TRG_3_AC01_TTL_CL                              0x%x"),
	_T("ARM_EXP_1_TRG_0_4AC_CON0_TTL_CL_RADIENT                  0x%x"),
	_T("ARM_EXP_1_TRG_1_4AC_CON0_TTL_CL_RADIENT                  0x%x"),
	_T("ARM_EXP_1_TRG_0_4AC_CON1_TTL_CL_RADIENT                  0x%x"),
	_T("ARM_EXP_1_TRG_1_4AC_CON1_TTL_CL_RADIENT                  0x%x"),
	_T("ARM_EXP_1_TRG_0_4AC_CON2_TTL_CL_RADIENT                  0x%x"),
	_T("ARM_EXP_1_TRG_1_4AC_CON2_TTL_CL_RADIENT                  0x%x"),
	_T("ARM_EXP_1_TRG_0_4AC_CON3_TTL_CL_RADIENT                  0x%x"),
	_T("ARM_EXP_1_TRG_1_4AC_CON3_TTL_CL_RADIENT                  0x%x"),
	_T("ARM_EXP_1_TRG_2_4AC_CON0_TTL_CL_RADIENT                  0x%x"),
	_T("ARM_EXP_1_TRG_3_4AC_CON1_TTL_CL_RADIENT                  0x%x"),
	_T("ARM_EXP_1_TRG_2_4AC_CON2_TTL_CL_RADIENT                  0x%x"),
	_T("ARM_EXP_1_TRG_3_4AC_CON3_TTL_CL_RADIENT                  0x%x"),
	_T("ARM_EXP_1_TRG_0_AC0_LVDS_CL_SOL_BASE                     0x%x"),
	_T("ARM_EXP_1_TRG_1_AC0_LVDS_CL_SOL_BASE                     0x%x"),
	_T("ARM_EXP_1_TRG_0_AC0_LVDS_CL_SOL_EVCL                     0x%x"),
	_T("ARM_EXP_1_TRG_1_AC0_LVDS_CL_SOL_EVCL                     0x%x"),
	_T("ARM_EXP_1_TRG_0_AC0_LVDS_CL                              0x%x"),
	_T("ARM_EXP_1_TRG_1_AC0_LVDS_CL                              0x%x"),
	_T("ARM_EXP_1_TRG_0_AC1_LVDS_CL_SOL_EVCL                     0x%x"),
	_T("ARM_EXP_1_TRG_1_AC1_LVDS_CL_SOL_EVCL                     0x%x"),
	_T("ARM_EXP_1_TRG_0_AC1_LVDS_CL                              0x%x"),
	_T("ARM_EXP_1_TRG_1_AC1_LVDS_CL                              0x%x"),
	_T("ARM_EXP_1_TRG_2_AC01_LVDS_CL_SOL_EVCL                    0x%x"),
	_T("ARM_EXP_1_TRG_3_AC01_LVDS_CL_SOL_EVCL                    0x%x"),
	_T("ARM_EXP_1_TRG_2_AC01_LVDS_CL                             0x%x"),
	_T("ARM_EXP_1_TRG_3_AC01_LVDS_CL                             0x%x"),
	_T("ARM_EXP_1_TRG_0_4AC_CON0_LVDS_CL_RADIENT                 0x%x"),
	_T("ARM_EXP_1_TRG_1_4AC_CON0_LVDS_CL_RADIENT                 0x%x"),
	_T("ARM_EXP_1_TRG_0_4AC_CON2_LVDS_CL_RADIENT                 0x%x"),
	_T("ARM_EXP_1_TRG_1_4AC_CON2_LVDS_CL_RADIENT                 0x%x"),
	_T("ARM_EXP_1_TRG_2_4AC_CON1_LVDS_CL_RADIENT                 0x%x"),
	_T("ARM_EXP_1_TRG_3_4AC_CON1_LVDS_CL_RADIENT                 0x%x"),
	_T("ARM_EXP_1_TRG_2_4AC_CON3_LVDS_CL_RADIENT                 0x%x"),
	_T("ARM_EXP_1_TRG_3_4AC_CON3_LVDS_CL_RADIENT                 0x%x"),
	_T("ARM_EXP_1_TRG_1_AC0_OPTO_ANA                             0x%x"),
	_T("ARM_EXP_1_TRG_1_AC1_OPTO_ANA                             0x%x"),
	_T("ARM_EXP_1_TRG_1_AC2_OPTO_ANA                             0x%x"),
	_T("ARM_EXP_1_TRG_1_AC3_OPTO_ANA                             0x%x"),
	_T("ARM_EXP_1_TRG_0_AC0_TTL_ANA                              0x%x"),
	_T("ARM_EXP_1_TRG_0_AC1_TTL_ANA                              0x%x"),
	_T("ARM_EXP_1_TRG_0_AC2_TTL_ANA                              0x%x"),
	_T("ARM_EXP_1_TRG_0_AC3_TTL_ANA                              0x%x"),
	_T("ARM_EXP_1_TRG_2_4AC_AUX0_TTL_ANA                         0x%x"),
	_T("ARM_EXP_1_TRG_3_4AC_AUX1_TTL_ANA                         0x%x"),
	_T("ARM_EXP_1_TRG_2_4AC_AUX2_TTL_ANA                         0x%x"),
	_T("ARM_EXP_1_TRG_3_4AC_AUX3_TTL_ANA                         0x%x"),
	_T("ARM_EXP_1_TRG_2_4AC_AUX4_TTL_ANA                         0x%x"),
	_T("ARM_EXP_1_TRG_3_4AC_AUX5_TTL_ANA                         0x%x"),
	_T("ARM_EXP_1_TRG_2_4AC_AUX6_TTL_ANA                         0x%x"),
	_T("ARM_EXP_1_TRG_3_4AC_AUX7_TTL_ANA                         0x%x"),
	_T("ARM_EXP_1_TRG_2_4AC_AUX0_LVDS_ANA                        0x%x"),
	_T("ARM_EXP_1_TRG_3_4AC_AUX1_LVDS_ANA                        0x%x"),
	_T("ARM_EXP_1_TRG_2_4AC_AUX2_LVDS_ANA                        0x%x"),
	_T("ARM_EXP_1_TRG_3_4AC_AUX3_LVDS_ANA                        0x%x"),
	_T("ARM_EXP_1_TRG_2_4AC_AUX4_LVDS_ANA                        0x%x"),
	_T("ARM_EXP_1_TRG_3_4AC_AUX5_LVDS_ANA                        0x%x"),
	_T("ARM_EXP_1_TRG_2_4AC_AUX6_LVDS_ANA                        0x%x"),
	_T("ARM_EXP_1_TRG_3_4AC_AUX7_LVDS_ANA                        0x%x"),
	_T("ARM_EXP_1_TRG_0_AC0_OPTO_DIG                             0x%x"),
	_T("ARM_EXP_1_TRG_1_AC0_OPTO_DIG                             0x%x"),
	_T("ARM_EXP_1_TRG_0_AC1_OPTO_DIG                             0x%x"),
	_T("ARM_EXP_1_TRG_1_AC1_OPTO_DIG                             0x%x"),
	_T("ARM_EXP_1_TRG_0_AC2_OPTO_DIG                             0x%x"),
	_T("ARM_EXP_1_TRG_1_AC2_OPTO_DIG                             0x%x"),
	_T("ARM_EXP_1_TRG_0_AC3_OPTO_DIG                             0x%x"),
	_T("ARM_EXP_1_TRG_1_AC3_OPTO_DIG                             0x%x"),
	_T("ARM_EXP_1_TRG_1_AC0_TTL_DIG                              0x%x"),
	_T("ARM_EXP_1_TRG_1_AC1_TTL_DIG                              0x%x"),
	_T("ARM_EXP_1_TRG_1_AC2_TTL_DIG                              0x%x"),
	_T("ARM_EXP_1_TRG_1_AC3_TTL_DIG                              0x%x"),
	_T("ARM_EXP_1_TRG_0_AC0_LVDS_DIG                             0x%x"),
	_T("ARM_EXP_1_TRG_0_AC1_LVDS_DIG                             0x%x"),
	_T("ARM_EXP_1_TRG_0_AC2_LVDS_DIG                             0x%x"),
	_T("ARM_EXP_1_TRG_0_AC3_LVDS_DIG                             0x%x"),
	_T("ARM_EXP_1_TRG_2_AC0_AUX1_TTL_DIG                         0x%x"),
	_T("ARM_EXP_1_TRG_3_AC0_AUX2_TTL_DIG                         0x%x"),
	_T("ARM_EXP_1_TRG_2_AC1_AUX1_TTL_DIG                         0x%x"),
	_T("ARM_EXP_1_TRG_3_AC1_AUX2_TTL_DIG                         0x%x"),
	_T("ARM_EXP_1_TRG_2_AC2_AUX1_TTL_DIG                         0x%x"),
	_T("ARM_EXP_1_TRG_3_AC2_AUX2_TTL_DIG                         0x%x"),
	_T("ARM_EXP_1_TRG_2_AC3_AUX1_TTL_DIG                         0x%x"),
	_T("ARM_EXP_1_TRG_3_AC3_AUX2_TTL_DIG                         0x%x"),
	_T("ARM_EXP_1_TRG_2_AC0_AUX1_LVDS_DIG                        0x%x"),
	_T("ARM_EXP_1_TRG_3_AC0_AUX2_LVDS_DIG                        0x%x"),
	_T("ARM_EXP_1_TRG_2_AC1_AUX1_LVDS_DIG                        0x%x"),
	_T("ARM_EXP_1_TRG_3_AC1_AUX2_LVDS_DIG                        0x%x"),
	_T("ARM_EXP_1_TRG_2_AC2_AUX1_LVDS_DIG                        0x%x"),
	_T("ARM_EXP_1_TRG_3_AC2_AUX2_LVDS_DIG                        0x%x"),
	_T("ARM_EXP_1_TRG_2_AC3_AUX1_LVDS_DIG                        0x%x"),
	_T("ARM_EXP_1_TRG_3_AC3_AUX2_LVDS_DIG                        0x%x"),
	_T("ARM_EXP_1_TRG_AC0_LVDS_CL_SOL_BASE_ROTARY_ENCODER_FOW    0x%x"),
	_T("ARM_EXP_1_TRG_AC0_LVDS_CL_SOL_BASE_ROTARY_ENCODER_REV    0x%x"),
	_T("ARM_EXP_1_TRG_AC0_LVDS_CL_SOL_EVCL_ROTARY_ENCODER_FOW    0x%x"),
	_T("ARM_EXP_1_TRG_AC0_LVDS_CL_SOL_EVCL_ROTARY_ENCODER_REV    0x%x"),
	_T("ARM_EXP_1_TRG_AC0_LVDS_CL_ROTARY_ENCODER_FOW             0x%x"),
	_T("ARM_EXP_1_TRG_AC0_LVDS_CL_ROTARY_ENCODER_REV             0x%x"),
	_T("ARM_EXP_1_TRG_AC1_LVDS_CL_SOL_EVCL_ROTARY_ENCODER_FOW    0x%x"),
	_T("ARM_EXP_1_TRG_AC1_LVDS_CL_SOL_EVCL_ROTARY_ENCODER_REV    0x%x"),
	_T("ARM_EXP_1_TRG_AC1_LVDS_CL_ROTARY_ENCODER_FOW             0x%x"),
	_T("ARM_EXP_1_TRG_AC1_LVDS_CL_ROTARY_ENCODER_REV             0x%x"),
	_T("ARM_EXP_1_TRG_AC0_LVDS_CL_RADIENT_ROTARY_ENCODER_FOW     0x%x"),
	_T("ARM_EXP_1_TRG_AC0_LVDS_CL_RADIENT_ROTARY_ENCODER_REV     0x%x"),
	_T("ARM_EXP_1_TRG_AC1_LVDS_CL_RADIENT_ROTARY_ENCODER_FOW     0x%x"),
	_T("ARM_EXP_1_TRG_AC1_LVDS_CL_RADIENT_ROTARY_ENCODER_REV     0x%x"),
	_T("ARM_EXP_1_TRG_AC2_LVDS_CL_RADIENT_ROTARY_ENCODER_FOW     0x%x"),
	_T("ARM_EXP_1_TRG_AC2_LVDS_CL_RADIENT_ROTARY_ENCODER_REV     0x%x"),
	_T("ARM_EXP_1_TRG_AC3_LVDS_CL_RADIENT_ROTARY_ENCODER_FOW     0x%x"),
	_T("ARM_EXP_1_TRG_AC3_LVDS_CL_RADIENT_ROTARY_ENCODER_REV     0x%x"),
	_T("ARM_EXP_1_TRG_AC0_LVDS_ANA_ROTARY_ENCODER_FOW            0x%x"),
	_T("ARM_EXP_1_TRG_AC0_LVDS_ANA_ROTARY_ENCODER_REV            0x%x"),
	_T("ARM_EXP_1_TRG_AC1_LVDS_ANA_ROTARY_ENCODER_FOW            0x%x"),
	_T("ARM_EXP_1_TRG_AC1_LVDS_ANA_ROTARY_ENCODER_REV            0x%x"),
	_T("ARM_EXP_1_TRG_AC2_LVDS_ANA_ROTARY_ENCODER_FOW            0x%x"),
	_T("ARM_EXP_1_TRG_AC2_LVDS_ANA_ROTARY_ENCODER_REV            0x%x"),
	_T("ARM_EXP_1_TRG_AC3_LVDS_ANA_ROTARY_ENCODER_FOW            0x%x"),
	_T("ARM_EXP_1_TRG_AC3_LVDS_ANA_ROTARY_ENCODER_REV            0x%x"),
	_T("ARM_EXP_1_TRG_AC0_LVDS_DIG_ROTARY_ENCODER_FOW            0x%x"),
	_T("ARM_EXP_1_TRG_AC0_LVDS_DIG_ROTARY_ENCODER_REV            0x%x"),
	_T("ARM_EXP_1_TRG_AC1_LVDS_DIG_ROTARY_ENCODER_FOW            0x%x"),
	_T("ARM_EXP_1_TRG_AC1_LVDS_DIG_ROTARY_ENCODER_REV            0x%x"),
	_T("ARM_EXP_1_TRG_AC2_LVDS_DIG_ROTARY_ENCODER_FOW            0x%x"),
	_T("ARM_EXP_1_TRG_AC2_LVDS_DIG_ROTARY_ENCODER_REV            0x%x"),
	_T("ARM_EXP_1_TRG_AC3_LVDS_DIG_ROTARY_ENCODER_FOW            0x%x"),
	_T("ARM_EXP_1_TRG_AC3_LVDS_DIG_ROTARY_ENCODER_REV            0x%x"),
	_T("ARM_EXP_1_TRG_LVDS_ROTARY_ENCODER                        0x%x"),
	_T("EXP_NOCOMBINE_2                                          0x%x"),
	_T("EXP_COMBINE_2_XOR_T0                                     0x%x"),
	_T("EXP_COMBINE_2_XOR_T1AC1                                  0x%x"),
	_T("EXP_COMBINE_2_T0                                         0x%x"),
	_T("EXP_1_NOCOMBINE_AC0_CL_SOL_BASE_HD_CC                    0x%x"),
	_T("EXP_1_COMBINE_XOR_T0_AC0_CL_SOL_BASE_HD_CC               0x%x"),
	_T("EXP_1_COMBINE_T0_AC0_CL_SOL_BASE_HD_CC                   0x%x"),
	_T("EXP_1_NOCOMBINE_AC0_CL_HD_CC                             0x%x"),
	_T("EXP_1_COMBINE_XOR_T0_AC0_CL_HD_CC                        0x%x"),
	_T("EXP_1_COMBINE_T0_AC0_CL_HD_CC                            0x%x"),
	_T("EXP_1_NOCOMBINE_AC1_CL_HD_CC                             0x%x"),
	_T("EXP_1_COMBINE_XOR_T0_AC1_CL_HD_CC                        0x%x"),
	_T("EXP_1_COMBINE_T0_AC1_CL_HD_CC                            0x%x"),
	_T("EXP_1_NOCOMBINE_CL_CC                                    0x%x"),
	_T("EXP_1_COMBINE_XOR_T0_CL_CC                               0x%x"),
	_T("EXP_1_COMBINE_T0_CL_CC                                   0x%x"),
	_T("EXP_1_NOCOMBINE_AC0_ANA_HD                               0x%x"),
	_T("EXP_1_COMBINE_XOR_T0_AC0_ANA_HD                          0x%x"),
	_T("EXP_1_COMBINE_T0_AC0_ANA_HD                              0x%x"),
	_T("EXP_1_NOCOMBINE_AC1_ANA_HD                               0x%x"),
	_T("EXP_1_COMBINE_XOR_T0_AC1_ANA_HD                          0x%x"),
	_T("EXP_1_COMBINE_T0_AC1_ANA_HD                              0x%x"),
	_T("EXP_1_NOCOMBINE_AC2_ANA_HD                               0x%x"),
	_T("EXP_1_COMBINE_XOR_T0_AC2_ANA_HD                          0x%x"),
	_T("EXP_1_COMBINE_T0_AC2_ANA_HD                              0x%x"),
	_T("EXP_1_NOCOMBINE_AC3_ANA_HD                               0x%x"),
	_T("EXP_1_COMBINE_XOR_T0_AC3_ANA_HD                          0x%x"),
	_T("EXP_1_COMBINE_T0_AC3_ANA_HD                              0x%x"),
	_T("EXP_1_NOCOMBINE_AC0_DIG_DCON                             0x%x"),
	_T("EXP_1_COMBINE_XOR_T0_AC0_DIG_DCON                        0x%x"),
	_T("EXP_1_COMBINE_T0_AC0_DIG_DCON                            0x%x"),
	_T("EXP_1_NOCOMBINE_AC1_DIG_DCON                             0x%x"),
	_T("EXP_1_COMBINE_XOR_T0_AC1_DIG_DCON                        0x%x"),
	_T("EXP_1_COMBINE_T0_AC1_DIG_DCON                            0x%x"),
	_T("EXP_1_NOCOMBINE_AC2_DIG_DCON                             0x%x"),
	_T("EXP_1_COMBINE_XOR_T0_AC2_DIG_DCON                        0x%x"),
	_T("EXP_1_COMBINE_T0_AC2_DIG_DCON                            0x%x"),
	_T("EXP_1_NOCOMBINE_AC3_DIG_DCON                             0x%x"),
	_T("EXP_1_COMBINE_XOR_T0_AC3_DIG_DCON                        0x%x"),
	_T("EXP_1_COMBINE_T0_AC3_DIG_DCON                            0x%x"),
	_T("EXP_PRESCALE2_1                                          0x%x"),
	_T("EXP_PRESCALE2_2                                          0x%x"),
	_T("EXP_PRESCALE2_4                                          0x%x"),
	_T("EXP_PRESCALE2_8                                          0x%x"),
	_T("EXP_PRESCALE2_16                                         0x%x"),
};


static LPCTSTR g_lpszDef_Board[EMatoxDCF_EOFDef_Board] =
{
	_T("DEF_ODYSSEY_CL                                           0x%x"),
	_T("DEF_ODYSSEY_CL_DUAL                                      0x%x"),
	_T("DEF_ODYSSEY_ANA                                          0x%x"),
	_T("DEF_ODYSSEY_DIG                                          0x%x"),
	_T("DEF_HELIOS_CL                                            0x%x"),
	_T("DEF_HELIOS_CL_DUAL                                       0x%x"),
	_T("DEF_HELIOS_ANA                                           0x%x"),
	_T("DEF_HELIOS_DIG                                           0x%x"),
	_T("DEF_SOLIOS_CL_FULL                                       0x%x"),
	_T("DEF_SOLIOS_CL_MEDIUM                                     0x%x"),
	_T("DEF_SOLIOS_CL_DUAL                                       0x%x"),
	_T("DEF_SOLIOS_CL_BASE                                       0x%x"),
	_T("DEF_SOLIOS_CL_DUAL_EVCL                                  0x%x"),
	_T("DEF_SOLIOS_CL_MEDIUM_EVCL                                0x%x"),
	_T("DEF_SOLIOS_CL_FULL_EVCL                                  0x%x"),
	_T("DEF_SOLIOS_CL_DUAL_EMCL                                  0x%x"),
	_T("DEF_SOLIOS_CL_MEDIUM_EMCL                                0x%x"),
	_T("DEF_SOLIOS_CL_FULL_EMCL                                  0x%x"),
	_T("DEF_SOLIOS_CL_EVCL                                       0x%x"),
	_T("DEF_SOLIOS_ALL_CL                                        0x%x"),
	_T("DEF_CAMERA_LINK_TIME_MULTIPLEX_MODES                     0x%x"),
	_T("DEF_SOLIOS_ALL_ANA                                       0x%x"),
	_T("DEF_SOLIOS_DIG                                           0x%x"),
	_T("DEF_RADIENT_CL_DUAL                                      0x%x"),
	_T("DEF_RADIENT_CL_QUAD                                      0x%x"),
	_T("DEF_RADIENT_CL_DUAL_FULL                                 0x%x"),
	_T("DEF_RADIENT_ALL_CL                                       0x%x"),
	_T("DEF_NRT_SOL_EVCL_RADIENT                                 0x%x"),
	_T("DEF_NRT_SOL_BASE_EVCL_RADIENT                            0x%x"),
	_T("DEF_ANALOG                                               0x%x"),
	_T("DEF_AC0_PROGRAMMED                                       0x%x"),
	_T("DEF_AC1_PROGRAMMED                                       0x%x"),
	_T("DEF_AC2_PROGRAMMED                                       0x%x"),
	_T("DEF_AC3_PROGRAMMED                                       0x%x"),
	_T("DEF_AC0_CL_TRG2_TTL_ACTIF                                0x%x"),
	_T("DEF_AC1_CL_TRG3_TTL_ACTIF                                0x%x"),
	_T("DEF_TAPS_MULTIPLEX_X                                     0x%x"),
	_T("DEF_TAPS_MULTIPLEX_Y                                     0x%x"),
	_T("DEF_INFO_XSIZE_DIVISOR                                   0x%x"),
	_T("DEF_INFO_YSIZE_DIVISOR                                   0x%x"),
	_T("DEF_ADD_HACTIVE_MULTIPLEX                                0x%x"),
	_T("DEF_ADD_VACTIVE_MULTIPLEX                                0x%x"),
	_T("DEF_HTOTAL_ENTRY                                         0x%x"),
	_T("DEF_HACTIVE_ENTRY                                        0x%x"),
	_T("DEF_VTOTAL_ENTRY                                         0x%x"),
	_T("DEF_VACTIVE_ENTRY                                        0x%x"),
	_T("DEF_CL_NEW_HCROPPING                                     0x%x"),
	_T("DEF_CL_NEW_VCROPPING                                     0x%x"),
	_T("DEF_DLY_ANA_FACTOR                                       0x%x"),
	_T("DEF_CLOCK_DELAY_ANA                                      0x%x"),
	_T("DEF_PCK_DELAY_STEP                                       0x%x"),
	_T("DEF_HDELAY                                               0x%x"),
	_T("DEF_ERR_MIN_HSYNC_PLL_LOCK                               0x%x"),
	_T("DEF_ADJUST_PLUS_HOTAL                                    0x%x"),
	_T("DEF_ADJUST_PLUS_HDLY_HSVAL                               0x%x"),
	_T("DEF_ADJUST_MINUS_HDLY_HSVAL                              0x%x"),
	_T("DEF_ADJUST_PLUS_HDLY_HEVAL                               0x%x"),
	_T("DEF_ADJUST_MINUS_HDLY_HEVAL                              0x%x"),
	_T("DEF_NTSC                                                 0x%x"),
	_T("DEF_PAL                                                  0x%x"),
	_T("DEF_CCIR601                                              0x%x"),
	_T("DEF_SLOW_SCAN_PCK_OTHER_CHSYNC_DVI                       0x%x"),
	_T("DEF_SLOW_SCAN_HSYNC_OTHER_CON_DVI_AUX                    0x%x"),
	_T("DEF_HSUBSEL_REGVALUE                                     0x%x"),
	_T("DEF_MONO_CAM                                             0x%x"),
	_T("DEF_COLOR_CAM                                            0x%x"),
	_T("DEF_VACTIVE_ODD                                          0x%x"),
	_T("DEF_VSTART_ODD                                           0x%x"),
	_T("DEF_VTOTAL_ENTRY_NOTSTD                                  0x%x"),
	_T("DEF_VDELAY_MIN                                           0x%x"),
	_T("DEF_VINTRL_ODD_EVEN                                      0x%x"),
	_T("DEF_VSVAL_EQUA_ZERO                                      0x%x"),
	_T("DEF_VEVAL_EQUA_VTOTAL                                    0x%x"),
	_T("DEF_ANA_SYC_DIG_FOLLOW_VIDEO                             0x%x"),
	_T("DEF_HFP_COMP_CLAMP_PARA                                  0x%x"),
	_T("DEF_MIN_CNT_CLAMP                                        0x%x"),
	_T("DEF_MIN_CLP_MARGIN                                       0x%x"),
	_T("DEF_HCLAMP_MARGIN                                        0x%x"),
	_T("DEF_MIN_CNT_CLAMP_REACHED                                0x%x"),
	_T("DEF_MINUS_CNT_CLAMP_REACHED                              0x%x"),
	_T("DEF_ADD_HCLAMP_COMPENSATION                              0x%x"),
	_T("DEF_ADD_HSCLAMP_COMPENSATION                             0x%x"),
	_T("DEF_ADD_HECLAMP_COMPENSATION                             0x%x"),
	_T("DEF_MINUS_HSCLAMP_COMPENSATION                           0x%x"),
	_T("DEF_MINUS_HECLAMP_COMPENSATION                           0x%x"),
	_T("DEF_SOLIOS_MINUS_HCLAMP_COMP                             0x%x"),
	_T("DEF_DIGITIZER_MASTER                                     0x%x"),
	_T("DEF_ERR_HCLAMP_MIN                                       0x%x"),
	_T("DEF_CASE_HVBLANK_ZERO                                    0x%x"),
	_T("DEF_ADD_3MAX_TOTAL_HVBLANK_ZERO                          0x%x"),
	_T("DEF_ADD_HTOTAL_EQUA_HEVAL                                0x%x"),
	_T("DEF_HOR_COUNT_MAX_BITWISE                                0x%x"),
	_T("DEF_VERT_COUNT_MAX_BITWISE                               0x%x"),
	_T("DEF_DIG_HTOTAL                                           0x%x"),
	_T("DEF_DIG_VTOTAL                                           0x%x"),
	_T("DEF_ERR_NGHECNT_HTOTAL_MIN                               0x%x"),
	_T("DEF_HFPORCH_MIN_ANA                                      0x%x"),
	_T("DEF_TEST_MODE_HBP_MIN                                    0x%x"),
	_T("DEF_TEST_MODE_HFP_MIN                                    0x%x"),
	_T("DEF_TEST_MODE_HSY_HBP_MIN_CL                             0x%x"),
	_T("DEF_TEST_MODE_VFP_MIN                                    0x%x"),
	_T("DEF_TEST_MODE_VBP_MIN                                    0x%x"),
	_T("DEF_TEST_MODE_VS_VBP_MIN_ANA_DIG_CL                      0x%x"),
	_T("DEF_HSVAL_EQUA_ZERO                                      0x%x"),
	_T("DEF_HEVAL_EQUA_HTOTAL                                    0x%x"),
	_T("DEF_HTOTAL_OVERFLOW                                      0x%x"),
	_T("DEF_HACTIVE_OVERFLOW                                     0x%x"),
	_T("DEF_VTOTAL_OVERFLOW                                      0x%x"),
	_T("DEF_VACTIVE_OVERFLOW                                     0x%x"),
	_T("DEF_VAL_SOFSEL                                           0x%x"),
	_T("DEF_ANA_TANDEM_MODE                                      0x%x"),
	_T("DEF_TIMER0_ENABLED                                       0x%x"),
	_T("DEF_TIMER1_ENABLED                                       0x%x"),
	_T("DEF_TIMER0_TRIGGERS_PIPE_DELAY                           0x%x"),
	_T("DEF_TIMER1_TRIGGERS_PIPE_DELAY                           0x%x"),
	_T("DEF_TIMER0_CLK_NOTAV                                     0x%x"),
	_T("DEF_TIMER1_CLK_NOTAV                                     0x%x"),
	_T("DEF_TIMER0_FREQ_NO_T1CLK                                 0x%x"),
	_T("DEF_TIMER1_FREQ_NO_T0CLK                                 0x%x"),
	_T("DEF_TIMER0_FREQ                                          0x%x"),
	_T("DEF_TIMER1_FREQ                                          0x%x"),
	_T("DEF_TIMER0_AUX_CLK_IN                                    0x%x"),
	_T("DEF_TIMER1_AUX_CLK_2_IN                                  0x%x"),
	_T("DEF_TIMER0_PIPE_DELAY1                                   0x%x"),
	_T("DEF_TIMER1_PIPE_DELAY1                                   0x%x"),
	_T("DEF_TMR0_CLKTMR1_CNT                                     0x%x"),
	_T("DEF_TMR1_CLKTMR0_CNT                                     0x%x"),
	_T("DEF_TIMER01_CLK_HS_FREQ                                  0x%x"),
	_T("DEF_TIMER01_CLK_HS_PERIOD                                0x%x"),
	_T("DEF_TIMER0_CLK_HS_PERIOD_DLY1_CNT                        0x%x"),
	_T("DEF_TIMER0_CLK_HS_PERIOD_DLY2_CNT                        0x%x"),
	_T("DEF_TIMER0_CLK_HS_PERIOD_T1_CNT                          0x%x"),
	_T("DEF_TIMER0_CLK_HS_PERIOD_T2_CNT                          0x%x"),
	_T("DEF_VALUE_EXP_0_DELAY1                                   0x%x"),
	_T("DEF_VALUE_EXP_0_DELAY2                                   0x%x"),
	_T("DEF_VALUE_EXP_0_TIME1                                    0x%x"),
	_T("DEF_VALUE_EXP_0_TIME2                                    0x%x"),
	_T("DEF_TIMER1_CLK_HS_PERIOD_DLY1_CNT                        0x%x"),
	_T("DEF_TIMER1_CLK_HS_PERIOD_DLY2_CNT                        0x%x"),
	_T("DEF_TIMER1_CLK_HS_PERIOD_T1_CNT                          0x%x"),
	_T("DEF_TIMER1_CLK_HS_PERIOD_T2_CNT                          0x%x"),
	_T("DEF_VALUE_EXP_1_DELAY1                                   0x%x"),
	_T("DEF_VALUE_EXP_1_DELAY2                                   0x%x"),
	_T("DEF_VALUE_EXP_1_TIME1                                    0x%x"),
	_T("DEF_VALUE_EXP_1_TIME2                                    0x%x"),
	_T("DEF_TOTAL_TIME_VALUE_EXP_0_NO_CLK_TMR1                   0x%x"),
	_T("DEF_TOTAL_TIME_VALUE_EXP_1_NO_CLK_TMR0                   0x%x"),
	_T("DEF_PCLK_PERIOD_NS_NO_ROUND                              0x%x"),
	_T("DEF_VALUE_EXP_0_CLK_TMR1_DELAY1                          0x%x"),
	_T("DEF_VALUE_EXP_0_CLK_TMR1_DELAY2                          0x%x"),
	_T("DEF_VALUE_EXP_0_CLK_TMR1_TIME1                           0x%x"),
	_T("DEF_VALUE_EXP_0_CLK_TMR1_TIME2                           0x%x"),
	_T("DEF_VALUE_EXP_1_CLK_TMR0_DELAY1                          0x%x"),
	_T("DEF_VALUE_EXP_1_CLK_TMR0_DELAY2                          0x%x"),
	_T("DEF_VALUE_EXP_1_CLK_TMR0_TIME1                           0x%x"),
	_T("DEF_VALUE_EXP_1_CLK_TMR0_TIME2                           0x%x"),
	_T("DEF_VALUE_EXP_0_CLK_TMR1_DELAY1_ADJUST                   0x%x"),
	_T("DEF_VALUE_EXP_0_CLK_TMR1_TIME1_ADJUST                    0x%x"),
	_T("DEF_VALUE_EXP_1_CLK_TMR0_DELAY1_ADJUST                   0x%x"),
	_T("DEF_VALUE_EXP_1_CLK_TMR0_TIME1_ADJUST                    0x%x"),
	_T("DEF_EXP_0_TIME1_VALUE_LESS_HW                            0x%x"),
	_T("DEF_TIMER0_MAX_CNT_TIM_DLY                               0x%x"),
	_T("DEF_TIMER1_MAX_CNT_TIM_DLY                               0x%x"),
	_T("DEF_INFO_TIME_OVRF_EXP0                                  0x%x"),
	_T("DEF_INFO_TIME_OVRF_EXP1                                  0x%x"),
	_T("DEF_TIMER0_CNT_OVERFLOW                                  0x%x"),
	_T("DEF_TIMER1_CNT_OVERFLOW                                  0x%x"),
	_T("DEF_TIMER0_T1CLK_PERD_FREQ                               0x%x"),
	_T("DEF_TIMER1_T0CLK_PERD_FREQ                               0x%x"),
	_T("DEF_TMR0_CLKFREQ_LESS_EQU_TRGFREQ                        0x%x"),
	_T("DEF_TMR1_CLKFREQ_LESS_EQU_TRGFREQ                        0x%x"),
	_T("DEF_SOLIOS_EVCL_EMCL_TIMER0_OUT_HD15_AV                  0x%x"),
	_T("DEF_SOLIOS_EVCL_EMCL_TIMER1_OUT_HD15_AV                  0x%x"),
	_T("DEF_RADIENT_TIMER0_OUT_AV                                0x%x"),
	_T("DEF_RADIENT_TIMER1_OUT_AV                                0x%x"),
	_T("DEF_HW_CHANNEL_USED                                      0x%x"),
	_T("DEF_ATTENUATOR                                           0x%x"),
	_T("DEF_CODE_GAIN                                            0x%x"),
	_T("DEF_CODE_OFFSET                                          0x%x"),
	_T("DEF_CL_BUS_WIDTH                                         0x%x"),
	_T("DEF_CL_NUM_TAPS                                          0x%x"),
	_T("DEF_ERR_BUS_WIDTH                                        0x%x"),
	_T("DEF_GRAB_PSG_CHANGE_ERROR                                0x%x"),
	_T("DEF_EXP0_PSG_CHANGE_ERROR                                0x%x"),
	_T("DEF_ARM_EXP0_PSG_CHANGE_ERROR                            0x%x"),
	_T("DEF_EXP1_PSG_CHANGE_ERROR                                0x%x"),
	_T("DEF_ARM_EXP1_PSG_CHANGE_ERROR                            0x%x"),
	_T("DEF_ERR_INTERNAL_TRG0_2_FORMATS_SEL                      0x%x"),
	_T("DEF_ERR_INTERNAL_TRG1_2_FORMATS_SEL                      0x%x"),
	_T("DEF_ERR_INTERNAL_TRG2_2_FORMATS_SEL                      0x%x"),
	_T("DEF_ERR_INTERNAL_TRG3_2_FORMATS_SEL                      0x%x"),
	_T("DEF_ERR_TRIGGER2_PATH_CONFLICT                           0x%x"),
	_T("DEF_ERR_TRIGGER3_PATH_CONFLICT                           0x%x"),
	_T("DEF_ROTARY_CL_HCNT_LD_ARM_AC01_NRT_REV                   0x%x"),
	_T("DEF_ROTARY_CL_GRB_AC01_NRT_REV                           0x%x"),
	_T("DEF_ROTARY_CL_EXP0_AC01_NRT_REV                          0x%x"),
	_T("DEF_ROTARY_CL_EXP1_AC01_NRT_REV                          0x%x"),
	_T("DEF_ROTARY_CL_AEXP0_AC01_NRT_REV                         0x%x"),
	_T("DEF_ROTARY_CL_AEXP1_AC01_NRT_REV                         0x%x"),
	_T("DEF_ROTARY_CL_HCNT_LD_ARM_AC01_NRT_FOW                   0x%x"),
	_T("DEF_ROTARY_CL_GRB_AC01_NRT_FOW                           0x%x"),
	_T("DEF_ROTARY_CL_EXP0_AC01_NRT_FOW                          0x%x"),
	_T("DEF_ROTARY_CL_EXP1_AC01_NRT_FOW                          0x%x"),
	_T("DEF_ROTARY_CL_AEXP0_AC01_NRT_FOW                         0x%x"),
	_T("DEF_ROTARY_CL_AEXP1_AC01_NRT_FOW                         0x%x"),
	_T("DEF_ROTARY_ANA_HCNT_LD_ARM_AC03_NRT_REV                  0x%x"),
	_T("DEF_ROTARY_ANA_GRB_AC03_NRT_REV                          0x%x"),
	_T("DEF_ROTARY_ANA_EXP0_AC03_NRT_REV                         0x%x"),
	_T("DEF_ROTARY_ANA_EXP1_AC03_NRT_REV                         0x%x"),
	_T("DEF_ROTARY_ANA_AEXP0_AC03_NRT_REV                        0x%x"),
	_T("DEF_ROTARY_ANA_AEXP1_AC03_NRT_REV                        0x%x"),
	_T("DEF_ROTARY_ANA_HCNT_LD_ARM_AC03_NRT_FOW                  0x%x"),
	_T("DEF_ROTARY_ANA_GRB_AC03_NRT_FOW                          0x%x"),
	_T("DEF_ROTARY_ANA_EXP0_AC03_NRT_FOW                         0x%x"),
	_T("DEF_ROTARY_ANA_EXP1_AC03_NRT_FOW                         0x%x"),
	_T("DEF_ROTARY_ANA_AEXP0_AC03_NRT_FOW                        0x%x"),
	_T("DEF_ROTARY_ANA_AEXP1_AC03_NRT_FOW                        0x%x"),
	_T("DEF_ROTARY_DIG_HCNT_LD_ARM_AC03_NRT_REV                  0x%x"),
	_T("DEF_ROTARY_DIG_GRB_AC03_NRT_REV                          0x%x"),
	_T("DEF_ROTARY_DIG_EXP0_AC03_NRT_REV                         0x%x"),
	_T("DEF_ROTARY_DIG_EXP1_AC03_NRT_REV                         0x%x"),
	_T("DEF_ROTARY_DIG_AEXP0_AC03_NRT_REV                        0x%x"),
	_T("DEF_ROTARY_DIG_AEXP1_AC03_NRT_REV                        0x%x"),
	_T("DEF_ROTARY_DIG_HCNT_LD_ARM_AC03_NRT_FOW                  0x%x"),
	_T("DEF_ROTARY_DIG_GRB_AC03_NRT_FOW                          0x%x"),
	_T("DEF_ROTARY_DIG_EXP0_AC03_NRT_FOW                         0x%x"),
	_T("DEF_ROTARY_DIG_EXP1_AC03_NRT_FOW                         0x%x"),
	_T("DEF_ROTARY_DIG_AEXP0_AC03_NRT_FOW                        0x%x"),
	_T("DEF_ROTARY_DIG_AEXP1_AC03_NRT_FOW                        0x%x"),
	_T("DEF_USE_ROTARY_ALL_BOARDS_GRB                            0x%x"),
	_T("DEF_USE_ROTARY_ALL_BOARDS_EXP0                           0x%x"),
	_T("DEF_USE_ROTARY_ALL_BOARDS_EXP1                           0x%x"),
	_T("DEF_USE_ROTARY_ALL_BOARDS_AEXP0                          0x%x"),
	_T("DEF_USE_ROTARY_ALL_BOARDS_AEXP1                          0x%x"),
	_T("DEF_USE_ROTARY_CL_BOARD                                  0x%x"),
	_T("DEF_USE_ROTARY_ANA_BOARD                                 0x%x"),
	_T("DEF_USE_ROTARY_DIG_BOARD                                 0x%x"),
	_T("DEF_DIG1_TAPDIR                                          0x%x"),
	_T("DEF_DIG2_TAPDIR                                          0x%x"),
	_T("DEF_DIG3_TAPDIR                                          0x%x"),
	_T("DEF_DIG4_TAPDIR                                          0x%x"),
	_T("DEF_DIG5_TAPDIR                                          0x%x"),
	_T("DEF_DIG6_TAPDIR                                          0x%x"),
	_T("DEF_DIG7_TAPDIR                                          0x%x"),
	_T("DEF_DIG8_TAPDIR                                          0x%x"),
	_T("DEF_DIG8_TAPORDER                                        0x%x"),
	_T("DEF_DIG7_TAPORDER                                        0x%x"),
	_T("DEF_DIG6_TAPORDER                                        0x%x"),
	_T("DEF_DIG5_TAPORDER                                        0x%x"),
	_T("DEF_DIG4_TAPORDER                                        0x%x"),
	_T("DEF_DIG3_TAPORDER                                        0x%x"),
	_T("DEF_DIG2_TAPORDER                                        0x%x"),
	_T("DEF_DIG1_TAPORDER                                        0x%x"),
	_T("DEF_TAPS_ORDER                                           0x%x"),
};

static LPCTSTR g_lpszReg_Digit[EMatoxDCF_EOFReg_Digit] =
{
	_T("[REG_DIGIT]"),
	_T("INFO_CUSTOM                                              0x%x"),
	_T("INFO_REGISTER_REV                                        0x%x"),
	_T("INFO_XSIZE                                               0x%x"),
	_T("INFO_YSIZE                                               0x%x"),
	_T("INFO_TYPE                                                0x%x"),
	_T("INFO_BAYER                                               0x%x"),
	_T("INFO_BURSTSIZE                                           0x%x"),
	_T("INFO_CAM                                                 0x%x"),
	_T("INFO_GRABPATH                                            0x%x"),
	_T("INFO_SSPCLKSEL                                           0x%x"),
	_T("INFO_SSHREFSEL                                           0x%x"),
	_T("INFO_PIXCLK                                              0x%x"),
	_T("INFO_CLOCKDELAY                                          0x%x"),
	_T("INFO_USRCLK                                              0x%x"),
	_T("INFO_SAMPLEMODE                                          0x%x"),
	_T("INFO_SIGNALTYPE                                          0x%x"),
	_T("INFO_INPUTSOURCE                                         0x%x"),
	_T("INFO_CHANNEL                                             0x%x"),
	_T("INFO_SYNCGRABCHAN                                        0x%x"),
	_T("INFO_SYNCCHANNEL                                         0x%x"),
	_T("INFO_ATTENUATION                                         0x%x"),
	_T("INFO_GAIN0                                               0x%x"),
	_T("INFO_GAIN1                                               0x%x"),
	_T("INFO_GAIN2                                               0x%x"),
	_T("INFO_GAIN3                                               0x%x"),
	_T("INFO_OFFSET0                                             0x%x"),
	_T("INFO_OFFSET1                                             0x%x"),
	_T("INFO_OFFSET2                                             0x%x"),
	_T("INFO_OFFSET3                                             0x%x"),
	_T("INFO_CLAMP                                               0x%x"),
	_T("INFO_LUTBUFID                                            0x%x"),
	_T("INFO_LUTPROG                                             0x%x"),
	_T("INFO_LUTMODE                                             0x%x"),
	_T("INFO_LUTPALETTE                                          0x%x"),
	_T("INFO_CLMODE                                              0x%x"),
	_T("INFO_TESTMODE                                            0x%x"),
	_T("INFO_BITSPERCOMPONENT                                    0x%x"),
	_T("INFO_NUMCOMPONENTS                                       0x%x"),
	_T("INFO_COMPONENTSPERPIXEL                                  0x%x"),
	_T("INFO_PACKEDCOMPONENTS                                    0x%x"),
	_T("INFO_HDELAY                                              0x%x"),
	_T("INFO_LINEDELAY                                           0x%x"),
	_T("INFO_TIMEMULTICH                                         0x%x"),
	_T("INFO_TIMEMULTIPIX                                        0x%x"),
	_T("INFO_XTAPSPERCH                                          0x%x"),
	_T("INFO_YTAPSPERCH                                          0x%x"),
	_T("INFO_XTAPSPERCHADJ                                       0x%x"),
	_T("INFO_YTAPSPERCHADJ                                       0x%x"),
	_T("INFO_TAPSDIR                                             0x%x"),
	_T("INFO_TAPSORDER                                           0x%x"),
	_T("INFO_BYTESORDER                                          0x%x"),
	_T("INFO_HARDGRABTRIG                                        0x%x"),
	_T("INFO_TRIGSRC                                             0x%x"),
	_T("INFO_T0DELAY                                             0x%x"),
	_T("INFO_T0DELAY1                                            0x%x"),
	_T("INFO_T0TRIGSRC                                           0x%x"),
	_T("INFO_T0CLKSRC                                            0x%x"),
	_T("INFO_T0USRCLK                                            0x%x"),
	_T("INFO_T0OTHERUSRCLK                                       0x%x"),
	_T("INFO_T1DELAY                                             0x%x"),
	_T("INFO_T1DELAY1                                            0x%x"),
	_T("INFO_T1TRIGSRC                                           0x%x"),
	_T("INFO_T1CLKSRC                                            0x%x"),
	_T("INFO_T1USRCLK                                            0x%x"),
	_T("INFO_T1OTHERUSRCLK                                       0x%x"),
	_T("INFO_T2DELAY                                             0x%x"),
	_T("INFO_T2DELAY1                                            0x%x"),
	_T("INFO_T2TRIGSRC                                           0x%x"),
	_T("INFO_T2CLKSRC                                            0x%x"),
	_T("INFO_T2USRCLK                                            0x%x"),
	_T("INFO_T2OTHERUSRCLK                                       0x%x"),
	_T("INFO_T3DELAY                                             0x%x"),
	_T("INFO_T3DELAY1                                            0x%x"),
	_T("INFO_T3TRIGSRC                                           0x%x"),
	_T("INFO_T3CLKSRC                                            0x%x"),
	_T("INFO_T3USRCLK                                            0x%x"),
	_T("INFO_T3OTHERUSRCLK                                       0x%x"),
	_T("INFO_MISC                                                0x%x"),
	_T("INFO_DEPTH                                               0x%x"),
	_T("INFO_BAND                                                0x%x"),
	_T("INFO_INPUT                                               0x%x"),
	_T("INFO_MODULE_422                                          0x%x"),
	_T("INFO_FORMAT                                              0x%x"),
	_T("INFO_INPUT_MUX_SEL                                       0x%x"),
	_T("INFO_M_CHANNEL                                           0x%x"),
	_T("INFO_M_CHANNEL_SYNC                                      0x%x"),
	_T("INFO_M_GRAB_INPUT_GAIN                                   0x%x"),
	_T("INFO_M_INPUT_FILTER                                      0x%x"),
	_T("INFO_M_GRAB_TRIGGER_ENABLE                               0x%x"),
	_T("INFO_M_GRAB_TRIGGER_MODE                                 0x%x"),
	_T("INFO_M_GRAB_TRIGGER_FORMAT                               0x%x"),
	_T("INFO_M_GRAB_TRIGGER_SOURCE                               0x%x"),
	_T("INFO_M_GRAB_EXPOSURE_0_MODE                              0x%x"),
	_T("INFO_M_GRAB_EXPOSURE_0_TRIGGER_MODE                      0x%x"),
	_T("INFO_M_GRAB_EXPOSURE_0_TRIGGER_FORMAT                    0x%x"),
	_T("INFO_M_GRAB_EXPOSURE_0_TRIGGER_SOURCE                    0x%x"),
	_T("INFO_M_GRAB_EXPOSURE_0_TIME_DELAY1                       0x%x"),
	_T("INFO_M_GRAB_EXPOSURE_0_TIME_DELAY2                       0x%x"),
	_T("INFO_M_GRAB_EXPOSURE_0_TIME1                             0x%x"),
	_T("INFO_M_GRAB_EXPOSURE_0_TIME2                             0x%x"),
	_T("INFO_M_GRAB_EXPOSURE_1_MODE                              0x%x"),
	_T("INFO_M_GRAB_EXPOSURE_1_TRIGGER_MODE                      0x%x"),
	_T("INFO_M_GRAB_EXPOSURE_1_TRIGGER_FORMAT                    0x%x"),
	_T("INFO_M_GRAB_EXPOSURE_1_TRIGGER_SOURCE                    0x%x"),
	_T("INFO_M_GRAB_EXPOSURE_1_TIME_DELAY1                       0x%x"),
	_T("INFO_M_GRAB_EXPOSURE_1_TIME_DELAY2                       0x%x"),
	_T("INFO_M_GRAB_EXPOSURE_1_TIME1                             0x%x"),
	_T("INFO_M_GRAB_EXPOSURE_1_TIME2                             0x%x"),
	_T("INFO_M_GRAB_EXPOSURE_2_MODE                              0x%x"),
	_T("INFO_M_GRAB_EXPOSURE_2_TRIGGER_MODE                      0x%x"),
	_T("INFO_M_GRAB_EXPOSURE_2_TRIGGER_FORMAT                    0x%x"),
	_T("INFO_M_GRAB_EXPOSURE_2_TIME_DELAY1                       0x%x"),
	_T("INFO_M_GRAB_EXPOSURE_2_TIME_DELAY2                       0x%x"),
	_T("INFO_M_GRAB_EXPOSURE_2_TIME1                             0x%x"),
	_T("INFO_M_GRAB_EXPOSURE_2_TIME2                             0x%x"),
	_T("INFO_M_GRAB_EXPOSURE_3_MODE                              0x%x"),
	_T("INFO_M_GRAB_EXPOSURE_3_TRIGGER_MODE                      0x%x"),
	_T("INFO_M_GRAB_EXPOSURE_3_TRIGGER_FORMAT                    0x%x"),
	_T("INFO_M_GRAB_EXPOSURE_3_TIME_DELAY1                       0x%x"),
	_T("INFO_M_GRAB_EXPOSURE_3_TIME_DELAY2                       0x%x"),
	_T("INFO_M_GRAB_EXPOSURE_3_TIME1                             0x%x"),
	_T("INFO_M_GRAB_EXPOSURE_3_TIME2                             0x%x"),
	_T("INFO_MODE                                                0x%x"),
	_T("INFO_SIGNALBLACKLVL                                      0x%x"),
	_T("INFO_SIGNALWHITELVL                                      0x%x"),
	_T("INFO_LUTINSIZE                                           0x%x"),
	_T("INFO_LUTOUTSIZE                                          0x%x"),
	_T("INFO_CLCONFIGMODE                                        0x%x"),
	_T("INFO_PACKEDPIXELS                                        0x%x"),
	_T("INFO_T0TRGSRC                                            0x%x"),
	_T("INFO_T1TRGSRC                                            0x%x"),
	_T("INFO_T2TRGSRC                                            0x%x"),
	_T("INFO_T3TRGSRC                                            0x%x"),
	_T("INFO_T1EXPMOD                                            0x%x"),
	_T("INFO_T1DELAY2                                            0x%x"),
	_T("INFO_T2EXPMOD                                            0x%x"),
	_T("INFO_T2DELAY2                                            0x%x"),
	_T("INFO_M_GRAB_EXPOSURE_0_CLOCK_SOURCE                      0x%x"),
	_T("INFO_M_GRAB_EXPOSURE_1_CLOCK_SOURCE                      0x%x"),
	_T("INFO_M_GRAB_EXPOSURE_2_CLOCK_SOURCE                      0x%x"),
	_T("INFO_M_GRAB_EXPOSURE_3_CLOCK_SOURCE                      0x%x"),
	_T("INFO_M_GRAB_EXPOSURE_1_TIME                              0x%x"),
	_T("INFO_M_GRAB_EXPOSURE_2_TIME                              0x%x"),
	_T("INFO_M_GRAB_EXPOSURE_1_TIME_DELAY                        0x%x"),
	_T("INFO_M_GRAB_EXPOSURE_2_TIME_DELAY                        0x%x"),
	_T("INFO_M_GRAB_EXPOSURE_2_TRIGGER_SOURCE                    0x%x"),
	_T("INFO_M_GRAB_EXPOSURE_3_TRIGGER_SOURCE                    0x%x"),
	_T("INFO_M_GRAB_EXPOSURE_2_TRIGGER_MODE                      0x%x"),
	_T("INFO_M_GRAB_EXPOSURE_2_TRIGGER_FORMAT                    0x%x"),
	_T("INFO_MASK_TRGIN                                          0x%x"),
	_T("INFO_MASK_USROUTDYN                                      0x%x"),
	_T("INFO_MASK_USROUT0                                        0x%x"),
	_T("INFO_MASK_USROUT1                                        0x%x"),
	_T("INFO_MASK_USROUT2                                        0x%x"),
	_T("INFO_MASK_USROUT3                                        0x%x"),
	_T("INFO_MASK_IOCTLCLDYNL                                    0x%x"),
	_T("INFO_MASK_IOCTLCLDYNH                                    0x%x"),
	_T("INFO_MASK_IOCTLCL0L                                      0x%x"),
	_T("INFO_MASK_IOCTLCL0H                                      0x%x"),
	_T("INFO_MASK_IOCTLCL1L                                      0x%x"),
	_T("INFO_MASK_IOCTLCL1H                                      0x%x"),
	_T("INFO_MASK_IOCTLANDYN                                     0x%x"),
	_T("INFO_MASK_IOCTLAN0                                       0x%x"),
	_T("INFO_MASK_IOCTLAN1                                       0x%x"),
	_T("INFO_MASK_IOCTLAN2                                       0x%x"),
	_T("INFO_MASK_IOCTLAN3                                       0x%x"),
	_T("INFO_MASK_IOCTLDIDYN                                     0x%x"),
	_T("INFO_MASK_IOCTLDI0                                       0x%x"),
	_T("INFO_MASK_IOCTLDI1                                       0x%x"),
	_T("INFO_MASK_IOCTLDI2                                       0x%x"),
	_T("INFO_MASK_IOCTLDI3                                       0x%x"),
	_T("INFO_MASK_ENCTLCLDYN                                     0x%x"),
	_T("INFO_MASK_ENCTLCL0                                       0x%x"),
	_T("INFO_MASK_ENCTLCL1                                       0x%x"),
	_T("INFO_MASK_ENCTLANDYN                                     0x%x"),
	_T("INFO_MASK_ENCTLAN0                                       0x%x"),
	_T("INFO_MASK_ENCTLAN1                                       0x%x"),
	_T("INFO_MASK_ENCTLAN2                                       0x%x"),
	_T("INFO_MASK_ENCTLAN3                                       0x%x"),
	_T("INFO_MASK_ENCTLDIDYN                                     0x%x"),
	_T("INFO_MASK_ENCTLDI0                                       0x%x"),
	_T("INFO_MASK_ENCTLDI1                                       0x%x"),
	_T("INFO_MASK_ENCTLDI2                                       0x%x"),
	_T("INFO_MASK_ENCTLDI3                                       0x%x"),
	_T("INFO_MASK_T0CTLL                                         0x%x"),
	_T("INFO_MASK_T0CTLH                                         0x%x"),
	_T("INFO_MASK_T1CTLL                                         0x%x"),
	_T("INFO_MASK_T1CTLH                                         0x%x"),
	_T("INFO_MASK_T2CTLL                                         0x%x"),
	_T("INFO_MASK_T2CTLH                                         0x%x"),
	_T("INFO_MASK_T3CTLL                                         0x%x"),
	_T("INFO_MASK_T3CTLH                                         0x%x"),
	_T("DIG_HCNT                                                 0x%x"),
	_T("DIG_HTOTAL                                               0x%x"),
	_T("DIG_HSCNT                                                0x%x"),
	_T("DIG_HECNT                                                0x%x"),
	_T("DIG_HSSYNC                                               0x%x"),
	_T("DIG_HESYNC                                               0x%x"),
	_T("DIG_HSVAL                                                0x%x"),
	_T("DIG_HEVAL                                                0x%x"),
	_T("DIG_HSCLM                                                0x%x"),
	_T("DIG_HECLM                                                0x%x"),
	_T("DIG_HCTL                                                 0x%x"),
	_T("DIG_VCNT                                                 0x%x"),
	_T("DIG_VTOTAL                                               0x%x"),
	_T("DIG_VSCNT                                                0x%x"),
	_T("DIG_VECNT                                                0x%x"),
	_T("DIG_VSSYNC                                               0x%x"),
	_T("DIG_VESYNC                                               0x%x"),
	_T("DIG_VSVAL                                                0x%x"),
	_T("DIG_VEVAL                                                0x%x"),
	_T("DIG_VSCLM                                                0x%x"),
	_T("DIG_VECLM                                                0x%x"),
	_T("DIG_VCTL                                                 0x%x"),
	_T("DIG_T0CNT                                                0x%x"),
	_T("DIG_T0SCNT                                               0x%x"),
	_T("DIG_T0S0PUL                                              0x%x"),
	_T("DIG_T0E0PUL                                              0x%x"),
	_T("DIG_T0S1PUL                                              0x%x"),
	_T("DIG_T0CTLL                                               0x%x"),
	_T("DIG_T0CTLH                                               0x%x"),
	_T("DIG_T1CNT                                                0x%x"),
	_T("DIG_T1SCNT                                               0x%x"),
	_T("DIG_T1S0PUL                                              0x%x"),
	_T("DIG_T1E0PUL                                              0x%x"),
	_T("DIG_T1S1PUL                                              0x%x"),
	_T("DIG_T1CTLL                                               0x%x"),
	_T("DIG_T1CTLH                                               0x%x"),
	_T("DIG_QUADCTL                                              0x%x"),
	_T("DIG_QUADCTL_HIGH                                         0x%x"),
	_T("DIG_QUADCNT                                              0x%x"),
	_T("DIG_CLKCTL                                               0x%x"),
	_T("DIG_GRBCTL                                               0x%x"),
	_T("DIG_VALCTL                                               0x%x"),
	_T("DIG_FLDCTL                                               0x%x"),
	_T("DIG_SYNCOUT                                              0x%x"),
	_T("DIG_TRGIN                                                0x%x"),
	_T("DIG_EXPOUT                                               0x%x"),
	_T("DIG_USROUT                                               0x%x"),
	_T("DIG_USROUT0                                              0x%x"),
	_T("DIG_USROUT1                                              0x%x"),
	_T("DIG_USROUT2                                              0x%x"),
	_T("DIG_USROUT3                                              0x%x"),
	_T("DIG_CLCTL                                                0x%x"),
	_T("DIG_IOCTL0L                                              0x%x"),
	_T("DIG_IOCTL0H                                              0x%x"),
	_T("DIG_IOCTLCL0L                                            0x%x"),
	_T("DIG_IOCTLCL0H                                            0x%x"),
	_T("DIG_IOCTLCL1L                                            0x%x"),
	_T("DIG_IOCTLCL1H                                            0x%x"),
	_T("DIG_IOCTL1                                               0x%x"),
	_T("DIG_IOCTLAN0                                             0x%x"),
	_T("DIG_IOCTLAN1                                             0x%x"),
	_T("DIG_IOCTLAN2                                             0x%x"),
	_T("DIG_IOCTLAN3                                             0x%x"),
	_T("DIG_IOCTL2                                               0x%x"),
	_T("DIG_IOCTLDI0                                             0x%x"),
	_T("DIG_IOCTLDI1                                             0x%x"),
	_T("DIG_IOCTLDI2                                             0x%x"),
	_T("DIG_IOCTLDI3                                             0x%x"),
	_T("DIG_ENCTL0                                               0x%x"),
	_T("DIG_ENCTLCL0                                             0x%x"),
	_T("DIG_ENCTLCL1                                             0x%x"),
	_T("DIG_ENCTL1                                               0x%x"),
	_T("DIG_ENCTLAN0                                             0x%x"),
	_T("DIG_ENCTLAN1                                             0x%x"),
	_T("DIG_ENCTLAN2                                             0x%x"),
	_T("DIG_ENCTLAN3                                             0x%x"),
	_T("DIG_ENCTL2                                               0x%x"),
	_T("DIG_ENCTLDI0                                             0x%x"),
	_T("DIG_ENCTLDI1                                             0x%x"),
	_T("DIG_ENCTLDI2                                             0x%x"),
	_T("DIG_ENCTLDI3                                             0x%x"),
	_T("DIG_ANACTL                                               0x%x"),
	_T("DIG_ANCTL                                                0x%x"),
	_T("DIG_ANCTL0                                               0x%x"),
	_T("DIG_ANCTL1                                               0x%x"),
	_T("DIG_ANCTL2                                               0x%x"),
	_T("DIG_ANCTL3                                               0x%x"),
	_T("DIG_PLLCTL                                               0x%x"),
	_T("DIG_NGHECNT                                              0x%x"),
	_T("DIG_NGVECNT                                              0x%x"),
	_T("DIG_NGFECNT                                              0x%x"),
	_T("DIG_GRABCTRL                                             0x%x"),
	_T("DIG_GTM                                                  0x%x"),
	_T("DIG_GCTRLCHNL                                            0x%x"),
	_T("DIG_T0CTL_L                                              0x%x"),
	_T("DIG_T0CTL_H                                              0x%x"),
	_T("DIG_T1CTL_L                                              0x%x"),
	_T("DIG_T1CTL_H                                              0x%x"),
	_T("DIG_IOCTL0_L                                             0x%x"),
	_T("DIG_IOCTL0_H                                             0x%x"),
	_T("DIG_IOCTL1_L                                             0x%x"),
	_T("DIG_IOCTL1_H                                             0x%x"),
	_T("DIG_IOCTL1L                                              0x%x"),
	_T("DIG_IOCTL1H                                              0x%x"),
	_T("DIG_IOCTL0_L                                             0x%x"),
	_T("DIG_ENCTL                                                0x%x"),
};

static LPCTSTR g_lpszReg_Modify_state[EMatoxDCF_EOFReg_Modify_state] =
{
	_T("[REG_MODIF_STATE]"),
	_T("INFO_CUSTOM                                            not_modified"),
	_T("INFO_REGISTER_REV                                      not_modified"),
	_T("INFO_XSIZE                                             not_modified"),
	_T("INFO_YSIZE                                             not_modified"),
	_T("INFO_TYPE                                              not_modified"),
	_T("INFO_BAYER                                             not_modified"),
	_T("INFO_BURSTSIZE                                         not_modified"),
	_T("INFO_CAM                                               not_modified"),
	_T("INFO_GRABPATH                                          not_modified"),
	_T("INFO_SSPCLKSEL                                         not_modified"),
	_T("INFO_SSHREFSEL                                         not_modified"),
	_T("INFO_PIXCLK                                            not_modified"),
	_T("INFO_CLOCKDELAY                                        not_modified"),
	_T("INFO_USRCLK                                            not_modified"),
	_T("INFO_SAMPLEMODE                                        not_modified"),
	_T("INFO_SIGNALTYPE                                        not_modified"),
	_T("INFO_INPUTSOURCE                                       not_modified"),
	_T("INFO_CHANNEL                                           not_modified"),
	_T("INFO_SYNCGRABCHAN                                      not_modified"),
	_T("INFO_SYNCCHANNEL                                       not_modified"),
	_T("INFO_ATTENUATION                                       not_modified"),
	_T("INFO_GAIN0                                             not_modified"),
	_T("INFO_GAIN1                                             not_modified"),
	_T("INFO_GAIN2                                             not_modified"),
	_T("INFO_GAIN3                                             not_modified"),
	_T("INFO_OFFSET0                                           not_modified"),
	_T("INFO_OFFSET1                                           not_modified"),
	_T("INFO_OFFSET2                                           not_modified"),
	_T("INFO_OFFSET3                                           not_modified"),
	_T("INFO_CLAMP                                             not_modified"),
	_T("INFO_LUTBUFID                                          not_modified"),
	_T("INFO_LUTPROG                                           not_modified"),
	_T("INFO_LUTMODE                                           not_modified"),
	_T("INFO_LUTPALETTE                                        not_modified"),
	_T("INFO_CLMODE                                            not_modified"),
	_T("INFO_TESTMODE                                          not_modified"),
	_T("INFO_BITSPERCOMPONENT                                  not_modified"),
	_T("INFO_NUMCOMPONENTS                                     not_modified"),
	_T("INFO_COMPONENTSPERPIXEL                                not_modified"),
	_T("INFO_PACKEDCOMPONENTS                                  not_modified"),
	_T("INFO_HDELAY                                            not_modified"),
	_T("INFO_LINEDELAY                                         not_modified"),
	_T("INFO_TIMEMULTICH                                       not_modified"),
	_T("INFO_TIMEMULTIPIX                                      not_modified"),
	_T("INFO_XTAPSPERCH                                        not_modified"),
	_T("INFO_YTAPSPERCH                                        not_modified"),
	_T("INFO_XTAPSPERCHADJ                                     not_modified"),
	_T("INFO_YTAPSPERCHADJ                                     not_modified"),
	_T("INFO_TAPSDIR                                           not_modified"),
	_T("INFO_TAPSORDER                                         not_modified"),
	_T("INFO_BYTESORDER                                        not_modified"),
	_T("INFO_HARDGRABTRIG                                      not_modified"),
	_T("INFO_TRIGSRC                                           not_modified"),
	_T("INFO_T0DELAY                                           not_modified"),
	_T("INFO_T0DELAY1                                          not_modified"),
	_T("INFO_T0TRIGSRC                                         not_modified"),
	_T("INFO_T0CLKSRC                                          not_modified"),
	_T("INFO_T0USRCLK                                          not_modified"),
	_T("INFO_T0OTHERUSRCLK                                     not_modified"),
	_T("INFO_T1DELAY                                           not_modified"),
	_T("INFO_T1DELAY1                                          not_modified"),
	_T("INFO_T1TRIGSRC                                         not_modified"),
	_T("INFO_T1CLKSRC                                          not_modified"),
	_T("INFO_T1USRCLK                                          not_modified"),
	_T("INFO_T1OTHERUSRCLK                                     not_modified"),
	_T("INFO_T2DELAY                                           not_modified"),
	_T("INFO_T2DELAY1                                          not_modified"),
	_T("INFO_T2TRIGSRC                                         not_modified"),
	_T("INFO_T2CLKSRC                                          not_modified"),
	_T("INFO_T2USRCLK                                          not_modified"),
	_T("INFO_T2OTHERUSRCLK                                     not_modified"),
	_T("INFO_T3DELAY                                           not_modified"),
	_T("INFO_T3DELAY1                                          not_modified"),
	_T("INFO_T3TRIGSRC                                         not_modified"),
	_T("INFO_T3CLKSRC                                          not_modified"),
	_T("INFO_T3USRCLK                                          not_modified"),
	_T("INFO_T3OTHERUSRCLK                                     not_modified"),
	_T("INFO_MISC                                              not_modified"),
	_T("INFO_DEPTH                                             not_modified"),
	_T("INFO_BAND                                              not_modified"),
	_T("INFO_INPUT                                             not_modified"),
	_T("INFO_MODULE_422                                        not_modified"),
	_T("INFO_FORMAT                                            not_modified"),
	_T("INFO_INPUT_MUX_SEL                                     not_modified"),
	_T("INFO_M_CHANNEL                                         not_modified"),
	_T("INFO_M_CHANNEL_SYNC                                    not_modified"),
	_T("INFO_M_GRAB_INPUT_GAIN                                 not_modified"),
	_T("INFO_M_INPUT_FILTER                                    not_modified"),
	_T("INFO_M_GRAB_TRIGGER_ENABLE                             not_modified"),
	_T("INFO_M_GRAB_TRIGGER_MODE                               not_modified"),
	_T("INFO_M_GRAB_TRIGGER_FORMAT                             not_modified"),
	_T("INFO_M_GRAB_TRIGGER_SOURCE                             not_modified"),
	_T("INFO_M_GRAB_EXPOSURE_0_MODE                            not_modified"),
	_T("INFO_M_GRAB_EXPOSURE_0_TRIGGER_MODE                    not_modified"),
	_T("INFO_M_GRAB_EXPOSURE_0_TRIGGER_FORMAT                  not_modified"),
	_T("INFO_M_GRAB_EXPOSURE_0_TRIGGER_SOURCE                  not_modified"),
	_T("INFO_M_GRAB_EXPOSURE_0_TIME_DELAY1                     not_modified"),
	_T("INFO_M_GRAB_EXPOSURE_0_TIME_DELAY2                     not_modified"),
	_T("INFO_M_GRAB_EXPOSURE_0_TIME1                           not_modified"),
	_T("INFO_M_GRAB_EXPOSURE_0_TIME2                           not_modified"),
	_T("INFO_M_GRAB_EXPOSURE_1_MODE                            not_modified"),
	_T("INFO_M_GRAB_EXPOSURE_1_TRIGGER_MODE                    not_modified"),
	_T("INFO_M_GRAB_EXPOSURE_1_TRIGGER_FORMAT                  not_modified"),
	_T("INFO_M_GRAB_EXPOSURE_1_TRIGGER_SOURCE                  not_modified"),
	_T("INFO_M_GRAB_EXPOSURE_1_TIME_DELAY1                     not_modified"),
	_T("INFO_M_GRAB_EXPOSURE_1_TIME_DELAY2                     not_modified"),
	_T("INFO_M_GRAB_EXPOSURE_1_TIME1                           not_modified"),
	_T("INFO_M_GRAB_EXPOSURE_1_TIME2                           not_modified"),
	_T("INFO_M_GRAB_EXPOSURE_2_MODE                            not_modified"),
	_T("INFO_M_GRAB_EXPOSURE_2_TRIGGER_MODE                    not_modified"),
	_T("INFO_M_GRAB_EXPOSURE_2_TRIGGER_FORMAT                  not_modified"),
	_T("INFO_M_GRAB_EXPOSURE_2_TIME_DELAY1                     not_modified"),
	_T("INFO_M_GRAB_EXPOSURE_2_TIME_DELAY2                     not_modified"),
	_T("INFO_M_GRAB_EXPOSURE_2_TIME1                           not_modified"),
	_T("INFO_M_GRAB_EXPOSURE_2_TIME2                           not_modified"),
	_T("INFO_M_GRAB_EXPOSURE_3_MODE                            not_modified"),
	_T("INFO_M_GRAB_EXPOSURE_3_TRIGGER_MODE                    not_modified"),
	_T("INFO_M_GRAB_EXPOSURE_3_TRIGGER_FORMAT                  not_modified"),
	_T("INFO_M_GRAB_EXPOSURE_3_TIME_DELAY1                     not_modified"),
	_T("INFO_M_GRAB_EXPOSURE_3_TIME_DELAY2                     not_modified"),
	_T("INFO_M_GRAB_EXPOSURE_3_TIME1                           not_modified"),
	_T("INFO_M_GRAB_EXPOSURE_3_TIME2                           not_modified"),
	_T("INFO_MODE                                              not_modified"),
	_T("INFO_SIGNALBLACKLVL                                    not_modified"),
	_T("INFO_SIGNALWHITELVL                                    not_modified"),
	_T("INFO_LUTINSIZE                                         not_modified"),
	_T("INFO_LUTOUTSIZE                                        not_modified"),
	_T("INFO_CLCONFIGMODE                                      not_modified"),
	_T("INFO_PACKEDPIXELS                                      not_modified"),
	_T("INFO_T0TRGSRC                                          not_modified"),
	_T("INFO_T1TRGSRC                                          not_modified"),
	_T("INFO_T2TRGSRC                                          not_modified"),
	_T("INFO_T3TRGSRC                                          not_modified"),
	_T("INFO_T1EXPMOD                                          not_modified"),
	_T("INFO_T1DELAY2                                          not_modified"),
	_T("INFO_T2EXPMOD                                          not_modified"),
	_T("INFO_T2DELAY2                                          not_modified"),
	_T("INFO_M_GRAB_EXPOSURE_0_CLOCK_SOURCE                    not_modified"),
	_T("INFO_M_GRAB_EXPOSURE_1_CLOCK_SOURCE                    not_modified"),
	_T("INFO_M_GRAB_EXPOSURE_2_CLOCK_SOURCE                    not_modified"),
	_T("INFO_M_GRAB_EXPOSURE_3_CLOCK_SOURCE                    not_modified"),
	_T("INFO_M_GRAB_EXPOSURE_1_TIME                            not_modified"),
	_T("INFO_M_GRAB_EXPOSURE_2_TIME                            not_modified"),
	_T("INFO_M_GRAB_EXPOSURE_1_TIME_DELAY                      not_modified"),
	_T("INFO_M_GRAB_EXPOSURE_2_TIME_DELAY                      not_modified"),
	_T("INFO_M_GRAB_EXPOSURE_2_TRIGGER_SOURCE                  not_modified"),
	_T("INFO_M_GRAB_EXPOSURE_3_TRIGGER_SOURCE                  not_modified"),
	_T("INFO_M_GRAB_EXPOSURE_2_TRIGGER_MODE                    not_modified"),
	_T("INFO_M_GRAB_EXPOSURE_2_TRIGGER_FORMAT                  not_modified"),
	_T("INFO_MASK_TRGIN                                        not_modified"),
	_T("INFO_MASK_USROUTDYN                                    not_modified"),
	_T("INFO_MASK_USROUT0                                      not_modified"),
	_T("INFO_MASK_USROUT1                                      not_modified"),
	_T("INFO_MASK_USROUT2                                      not_modified"),
	_T("INFO_MASK_USROUT3                                      not_modified"),
	_T("INFO_MASK_IOCTLCLDYNL                                  not_modified"),
	_T("INFO_MASK_IOCTLCLDYNH                                  not_modified"),
	_T("INFO_MASK_IOCTLCL0L                                    not_modified"),
	_T("INFO_MASK_IOCTLCL0H                                    not_modified"),
	_T("INFO_MASK_IOCTLCL1L                                    not_modified"),
	_T("INFO_MASK_IOCTLCL1H                                    not_modified"),
	_T("INFO_MASK_IOCTLANDYN                                   not_modified"),
	_T("INFO_MASK_IOCTLAN0                                     not_modified"),
	_T("INFO_MASK_IOCTLAN1                                     not_modified"),
	_T("INFO_MASK_IOCTLAN2                                     not_modified"),
	_T("INFO_MASK_IOCTLAN3                                     not_modified"),
	_T("INFO_MASK_IOCTLDIDYN                                   not_modified"),
	_T("INFO_MASK_IOCTLDI0                                     not_modified"),
	_T("INFO_MASK_IOCTLDI1                                     not_modified"),
	_T("INFO_MASK_IOCTLDI2                                     not_modified"),
	_T("INFO_MASK_IOCTLDI3                                     not_modified"),
	_T("INFO_MASK_ENCTLCLDYN                                   not_modified"),
	_T("INFO_MASK_ENCTLCL0                                     not_modified"),
	_T("INFO_MASK_ENCTLCL1                                     not_modified"),
	_T("INFO_MASK_ENCTLANDYN                                   not_modified"),
	_T("INFO_MASK_ENCTLAN0                                     not_modified"),
	_T("INFO_MASK_ENCTLAN1                                     not_modified"),
	_T("INFO_MASK_ENCTLAN2                                     not_modified"),
	_T("INFO_MASK_ENCTLAN3                                     not_modified"),
	_T("INFO_MASK_ENCTLDIDYN                                   not_modified"),
	_T("INFO_MASK_ENCTLDI0                                     not_modified"),
	_T("INFO_MASK_ENCTLDI1                                     not_modified"),
	_T("INFO_MASK_ENCTLDI2                                     not_modified"),
	_T("INFO_MASK_ENCTLDI3                                     not_modified"),
	_T("INFO_MASK_T0CTLL                                       not_modified"),
	_T("INFO_MASK_T0CTLH                                       not_modified"),
	_T("INFO_MASK_T1CTLL                                       not_modified"),
	_T("INFO_MASK_T1CTLH                                       not_modified"),
	_T("INFO_MASK_T2CTLL                                       not_modified"),
	_T("INFO_MASK_T2CTLH                                       not_modified"),
	_T("INFO_MASK_T3CTLL                                       not_modified"),
	_T("INFO_MASK_T3CTLH                                       not_modified"),
	_T("DIG_HCNT                                               not_modified"),
	_T("DIG_HTOTAL                                             not_modified"),
	_T("DIG_HSCNT                                              not_modified"),
	_T("DIG_HECNT                                              not_modified"),
	_T("DIG_HSSYNC                                             not_modified"),
	_T("DIG_HESYNC                                             not_modified"),
	_T("DIG_HSVAL                                              not_modified"),
	_T("DIG_HEVAL                                              not_modified"),
	_T("DIG_HSCLM                                              not_modified"),
	_T("DIG_HECLM                                              not_modified"),
	_T("DIG_HCTL                                               not_modified"),
	_T("DIG_VCNT                                               not_modified"),
	_T("DIG_VTOTAL                                             not_modified"),
	_T("DIG_VSCNT                                              not_modified"),
	_T("DIG_VECNT                                              not_modified"),
	_T("DIG_VSSYNC                                             not_modified"),
	_T("DIG_VESYNC                                             not_modified"),
	_T("DIG_VSVAL                                              not_modified"),
	_T("DIG_VEVAL                                              not_modified"),
	_T("DIG_VSCLM                                              not_modified"),
	_T("DIG_VECLM                                              not_modified"),
	_T("DIG_VCTL                                               not_modified"),
	_T("DIG_T0CNT                                              not_modified"),
	_T("DIG_T0SCNT                                             not_modified"),
	_T("DIG_T0S0PUL                                            not_modified"),
	_T("DIG_T0E0PUL                                            not_modified"),
	_T("DIG_T0S1PUL                                            not_modified"),
	_T("DIG_T0CTLL                                             not_modified"),
	_T("DIG_T0CTLH                                             not_modified"),
	_T("DIG_T1CNT                                              not_modified"),
	_T("DIG_T1SCNT                                             not_modified"),
	_T("DIG_T1S0PUL                                            not_modified"),
	_T("DIG_T1E0PUL                                            not_modified"),
	_T("DIG_T1S1PUL                                            not_modified"),
	_T("DIG_T1CTLL                                             not_modified"),
	_T("DIG_T1CTLH                                             not_modified"),
	_T("DIG_QUADCTL                                            not_modified"),
	_T("DIG_QUADCTL_HIGH                                       not_modified"),
	_T("DIG_QUADCNT                                            not_modified"),
	_T("DIG_CLKCTL                                             not_modified"),
	_T("DIG_GRBCTL                                             not_modified"),
	_T("DIG_VALCTL                                             not_modified"),
	_T("DIG_FLDCTL                                             not_modified"),
	_T("DIG_SYNCOUT                                            not_modified"),
	_T("DIG_TRGIN                                              not_modified"),
	_T("DIG_EXPOUT                                             not_modified"),
	_T("DIG_USROUT                                             not_modified"),
	_T("DIG_USROUT0                                            not_modified"),
	_T("DIG_USROUT1                                            not_modified"),
	_T("DIG_USROUT2                                            not_modified"),
	_T("DIG_USROUT3                                            not_modified"),
	_T("DIG_CLCTL                                              not_modified"),
	_T("DIG_IOCTL0L                                            not_modified"),
	_T("DIG_IOCTL0H                                            not_modified"),
	_T("DIG_IOCTLCL0L                                          not_modified"),
	_T("DIG_IOCTLCL0H                                          not_modified"),
	_T("DIG_IOCTLCL1L                                          not_modified"),
	_T("DIG_IOCTLCL1H                                          not_modified"),
	_T("DIG_IOCTL1                                             not_modified"),
	_T("DIG_IOCTLAN0                                           not_modified"),
	_T("DIG_IOCTLAN1                                           not_modified"),
	_T("DIG_IOCTLAN2                                           not_modified"),
	_T("DIG_IOCTLAN3                                           not_modified"),
	_T("DIG_IOCTL2                                             not_modified"),
	_T("DIG_IOCTLDI0                                           not_modified"),
	_T("DIG_IOCTLDI1                                           not_modified"),
	_T("DIG_IOCTLDI2                                           not_modified"),
	_T("DIG_IOCTLDI3                                           not_modified"),
	_T("DIG_ENCTL0                                             not_modified"),
	_T("DIG_ENCTLCL0                                           not_modified"),
	_T("DIG_ENCTLCL1                                           not_modified"),
	_T("DIG_ENCTL1                                             not_modified"),
	_T("DIG_ENCTLAN0                                           not_modified"),
	_T("DIG_ENCTLAN1                                           not_modified"),
	_T("DIG_ENCTLAN2                                           not_modified"),
	_T("DIG_ENCTLAN3                                           not_modified"),
	_T("DIG_ENCTL2                                             not_modified"),
	_T("DIG_ENCTLDI0                                           not_modified"),
	_T("DIG_ENCTLDI1                                           not_modified"),
	_T("DIG_ENCTLDI2                                           not_modified"),
	_T("DIG_ENCTLDI3                                           not_modified"),
	_T("DIG_ANACTL                                             not_modified"),
	_T("DIG_ANCTL                                              not_modified"),
	_T("DIG_ANCTL0                                             not_modified"),
	_T("DIG_ANCTL1                                             not_modified"),
	_T("DIG_ANCTL2                                             not_modified"),
	_T("DIG_ANCTL3                                             not_modified"),
	_T("DIG_PLLCTL                                             not_modified"),
	_T("DIG_NGHECNT                                            not_modified"),
	_T("DIG_NGVECNT                                            not_modified"),
	_T("DIG_NGFECNT                                            not_modified"),
	_T("DIG_GRABCTRL                                           not_modified"),
	_T("DIG_GTM                                                not_modified"),
	_T("DIG_GCTRLCHNL                                          not_modified"),
	_T("DIG_T0CTL_L                                            not_modified"),
	_T("DIG_T0CTL_H                                            not_modified"),
	_T("DIG_T1CTL_L                                            not_modified"),
	_T("DIG_T1CTL_H                                            not_modified"),
	_T("DIG_IOCTL0_L                                           not_modified"),
	_T("DIG_IOCTL0_H                                           not_modified"),
	_T("DIG_IOCTL1_L                                           not_modified"),
	_T("DIG_IOCTL1_H                                           not_modified"),
	_T("DIG_IOCTL1L                                            not_modified"),
	_T("DIG_IOCTL1H                                            not_modified"),
	_T("DIG_IOCTL0_L                                           not_modified"),
	_T("DIG_ENCTL                                              not_modified"),
	_T("[EOF]"),
	_T("FILE_SIZE                                                0x3000000"),
};

CDeviceMatroxSolios::CDeviceMatroxSolios()
{
	m_bIsFramegrabber = true;
	m_vctMappingData.clear();
	m_vctMappingData.reserve(2365);
}


CDeviceMatroxSolios::~CDeviceMatroxSolios()
{
}

EDeviceInitializeResult CDeviceMatroxSolios::Initialize()
{
	EDeviceInitializeResult eReturn = EDeviceInitializeResult_UnknownError;

	eReturn = __super::Initialize();

	if(eReturn)
		return eReturn;
	
	eReturn = EDeviceInitializeResult_UnknownError;
	
	CString strMessage, strStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	do
	{
		if(IsInitialized())
		{
			strStatus = CMultiLanguageManager::GetString(ELanguageParameter_Initialized);
			strMessage.Format(_T("The device was already initialized"));
			eReturn = EDeviceInitializeResult_AlreadyInitializedError;
			break;
		}

		int nDeviceID = _ttoi(GetDeviceID());
		int nSubUnitID = _ttoi(GetSubUnitID());

		if(nDeviceID < 0 || nDeviceID > M_NUMBER_OF_USER_MODULES)
		{
			strMessage.Format(_T("Failed to write 'Device ID' to the device"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		MappControl(M_ERROR, M_PRINT_DISABLE);

		MIL_TEXT_CHAR lpszFramegrabberName[MAX_PATH];

		memset(&lpszFramegrabberName, 0, sizeof(lpszFramegrabberName));
		 
		MappInquire(M_INSTALLED_SYSTEM_DESCRIPTOR, lpszFramegrabberName);
		
		if(IsError())
		{
			strMessage.Format(_T("Failed to read 'Debice name' from the device"));
			eReturn = EDeviceInitializeResult_NotInitializeDevice;
			break;
		}

		CString strFramegrabberName = lpszFramegrabberName;

		if(strFramegrabberName.CompareNoCase(M_SYSTEM_SOLIOS))
		{
			strMessage.Format(_T("Couldn't find Solois board"));
			eReturn = EDeviceInitializeResult_NotFoundDeviceInfo;
			break;
		}

		if(IsError())
		{
			strMessage.Format(_T("Failed to write 'Error print' to the device"));
			eReturn = EDeviceInitializeResult_NotInitializeDevice;
			break;
		}

		if(m_hMilImage)
		{
			MbufFree(m_hMilImage);
			m_hMilImage = M_NULL;
		}

		if(m_hMilDigitizer)
		{
			MdigFree(m_hMilDigitizer);
			m_hMilDigitizer = M_NULL;
		}

		if(m_hMilSystem)
			m_hMilSystem = M_NULL;

		bool bAlreadyBeenAllocated = false;
		bool bErrorSub = false;

		for(int i = 0; i < (int)CDeviceManager::GetDeviceCount(); ++i)
		{
			CDeviceBase* pInstance = CDeviceManager::GetDeviceByIndex(i);

			if(!pInstance)
				continue;

			CDeviceMatroxSolios* pMatrox = dynamic_cast<CDeviceMatroxSolios*>(pInstance);

			if(!pMatrox)
				continue;

			if(pMatrox == this)
				continue;

			if(nDeviceID != _ttoi(pMatrox->GetDeviceID()))
				continue;

			if(!pMatrox->IsInitialized())
				continue;

			if(nSubUnitID == _ttoi(pMatrox->GetSubUnitID()))
			{
				bErrorSub = true;
				break;
			}

			m_hMilSystem = pMatrox->m_hMilSystem;
			bAlreadyBeenAllocated = true;
			break;
		}		

		if(bErrorSub)
		{
			strMessage.Format(_T("Couldn't read Camera index"));
			eReturn = EDeviceInitializeResult_NotInitializeDevice;
			break;
		}

		if(!bAlreadyBeenAllocated)
		{
			long lRet = MsysAlloc(M_SYSTEM_SOLIOS, nDeviceID, M_DEFAULT, &m_hMilSystem);

			if(IsError())
			{
				strMessage.Format(_T("Couldn't allocate solios board infomation"));
				eReturn = EDeviceInitializeResult_NotInitializeDevice;
				break;
			}
		}
		
		if(!m_hMilSystem)
		{
			strMessage.Format(_T("Couldn't allocate solios board infomation"));
			eReturn = EDeviceInitializeResult_NotInitializeDevice;
			break;
		}

		EDeviceMatrox_InitilizeType eInitType = EDeviceMatrox_InitilizeType_Count;

		if(GetInitilizeType(&eInitType))
			break;

		CString strCamfilePath;

		if(GetCamfilePath(&strCamfilePath))
		{
			strMessage.Format(_T("Couldn't read 'Camfile Path' from the database"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		bool bError = true;

		switch(eInitType)
		{
		case EDeviceMatrox_InitilizeType_Camfile:
			{
				MdigAlloc(m_hMilSystem, nSubUnitID, strCamfilePath, M_DEFAULT, &m_hMilDigitizer);

				if(IsError())
				{
					strMessage.Format(_T("Failed to allocate camera infomation"));
					eReturn = EDeviceInitializeResult_NotInitializeDevice;
					break;
				}

				if(!m_hMilDigitizer)
				{
					strMessage.Format(_T("Couldn't allocate camera infomation"));
					eReturn = EDeviceInitializeResult_NotInitializeDevice;
					break;
				}

				bError = false;
			}
			break;
		case EDeviceMatrox_InitilizeType_Parameter:
			{
				if(!MappingDcfFile())
				{
					strMessage.Format(_T("Failed to mapping the dcf file"));
					eReturn = EDeviceInitializeResult_NotMappingFile;
					break;
				}

				if(!MakeDcfFile(strCamfilePath))
				{
					strMessage.Format(_T("Failed to make the dcf file"));
					eReturn = EDeviceInitializeResult_NotMakeFile;
					break;
				}

				MdigAlloc(m_hMilSystem, nSubUnitID, strCamfilePath, M_DEFAULT, &m_hMilDigitizer);

				if(IsError())
				{
					strMessage.Format(_T("Failed to allocate camera infomation"));
					eReturn = EDeviceInitializeResult_NotInitializeDevice;
					break;
				}

				if(!m_hMilDigitizer)
				{
					strMessage.Format(_T("Couldn't allocate camera infomation"));
					eReturn = EDeviceInitializeResult_NotInitializeDevice;
					break;
				}

				bError = false;
			}
			break;
		default: 
			bError = true;
			break;
		}

		if(bError)
			break;

		MIL_INT nSizeX = -1, nSizeY = -1;

		MdigInquire(m_hMilDigitizer, M_SOURCE_SIZE_X, &nSizeX);

		if(IsError())
		{
			strMessage.Format(_T("Couldn't read 'Source Size X' from the device"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		MdigInquire(m_hMilDigitizer, M_SOURCE_SIZE_Y, &nSizeY);

		if(IsError())
		{
			strMessage.Format(_T("Couldn't read 'Source Size Y' from the device"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		int nGrabbufferCount = 0;

		if(GetMILBufferCount(&nGrabbufferCount))
		{
			strMessage.Format(_T("Couldn't read 'GetMILBufferCount' from the database"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(nGrabbufferCount < 1)
		{
			strMessage.Format(_T("Couldn't allocate grabbuffer"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		int nGrabcount = 0;

		if(GetMILImageCount(&nGrabcount))
		{
			strMessage.Format(_T("Couldn't read 'GetMILImageCount' from the database"));
			eReturn = EDeviceInitializeResult_ReadOnDatabaseError;
			break;
		}

		if(nGrabcount < 1)
		{
			strMessage.Format(_T("Couldn't write 'GetMILImageCount < 1' to the device"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		if(nGrabbufferCount > nGrabcount)
		{
			strMessage.Format(_T("Couldn't set 'GetMILBufferCount > GetMILImageCount'"));
			eReturn = EDeviceInitializeResult_NotInitializeDevice;
			break;
		}

		MbufAlloc2d(m_hMilSystem, nSizeX, nSizeY * nGrabcount, 8L + M_UNSIGNED, M_IMAGE + M_GRAB, &m_hMilImage);

		if(IsError())
		{
			strMessage.Format(_T("Failed to allocate Mil image buffer"));
			eReturn = EDeviceInitializeResult_NotInitializeDevice;
			break;
		}

		m_pMilImageList = new MIL_ID[nGrabbufferCount];

		if(!m_pMilImageList)
		{
			strMessage.Format(_T("Failed to allocate Mil child buffer"));
			eReturn = EDeviceInitializeResult_NotInitializeDevice;
			break;
		}

		ZeroMemory(m_pMilImageList, sizeof(*m_pMilImageList));

		for(int i = 0; i < nGrabbufferCount; ++i)
		{
			MbufChild2d(m_hMilImage, 0, i * nSizeY, nSizeX, nSizeY, &m_pMilImageList[i]);
			MbufClear(m_pMilImageList[i], 0);

			if(IsError())
			{
				strMessage.Format(_T("Failed to allocate Mil child buffer"));
				eReturn = EDeviceInitializeResult_NotInitializeDevice;
				break;
			}
		}

		MdigControl(m_hMilDigitizer, M_GRAB_MODE, M_ASYNCHRONOUS);

		if(IsError())
		{
			strMessage.Format(_T("Couldn't write 'Grab mode' to the device"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}

		MdigControl(m_hMilDigitizer, M_GRAB_TIMEOUT, -1);

		if(IsError())
		{
			strMessage.Format(_T("Couldn't write 'Grab timeout' to the device"));
			eReturn = EDeviceInitializeResult_WriteToDeviceError;
			break;
		}
		
		CMultipleVariable mv;
		for(int i = 0; i < 1; ++i)
			mv.AddValue(0xff);

		this->InitBuffer(nSizeX, nSizeY, mv, CRavidImage::MakeValueFormat(1, 8));
		this->ConnectImage();

		m_bIsInitialized = true;

		strStatus = CMultiLanguageManager::GetString(ELanguageParameter_Initialized);

		strMessage.Format(_T("Succeeded to initialize Device"));

		eReturn = EDeviceInitializeResult_OK;

		CEventHandlerManager::BroadcastOnDeviceInitialized(this);
	}
	while(false);

	if(!IsInitialized())
		Terminate();

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetStatus(strStatus);

	SetMessage(strMessage);

	return eReturn;
}

EDeviceTerminateResult CDeviceMatroxSolios::Terminate()
{
	return __super::Terminate();
}

bool CDeviceMatroxSolios::MappingDcfFile()
{
	bool bReturn = false;

	do
	{
		m_vctMappingData.clear();

		CString strMapping;

		//////////////////////////////////////////////////////////////////////////
		
		//g_lpszCamera_Name
		//EMatoxDCF_EOFCamera_Name

		CString strCamName;

		if(GetCameraName(&strCamName))
			break;

		m_vctMappingData.push_back(g_lpszCamera_Name[0]);

		if(strCamName.GetLength())
			m_vctMappingData.push_back(strCamName);
		else
			m_vctMappingData.push_back(_T("Sample Camera"));
		
		//////////////////////////////////////////////////////////////////////////

		//g_lpszConfig_File
		//EMatoxDCF_EOFConfig_File

		m_vctMappingData.push_back(g_lpszConfig_File[0]);
		m_vctMappingData.push_back(g_lpszConfig_File[1]);
		m_vctMappingData.push_back(g_lpszConfig_File[2]);
		m_vctMappingData.push_back(g_lpszConfig_File[3]);
		
		//////////////////////////////////////////////////////////////////////////

		//g_lpszInfo_File_Rev
		//EMatoxDCF_EOFInfo_File_Rev

		m_vctMappingData.push_back(g_lpszInfo_File_Rev[0]);
		m_vctMappingData.push_back(g_lpszInfo_File_Rev[1]);
		m_vctMappingData.push_back(g_lpszInfo_File_Rev[2]);

		//////////////////////////////////////////////////////////////////////////

		//Get the parameter

		EDeviceMatrox_CameraType eCameraType = EDeviceMatrox_CameraType_Count;
		EDeviceMatrox_NumberOfTaps eCameraNumberOfTaps = EDeviceMatrox_NumberOfTaps_Count;
		EDeviceMatrox_BayerMode eBayerMode = EDeviceMatrox_BayerMode_Count;
		EDeviceMatrox_ConfigurationType eConfigurationType = EDeviceMatrox_ConfigurationType_Count;
		EDeviceMatrox_CameraTapsMode eCameraTapsMode = EDeviceMatrox_CameraTapsMode_Count;
		EDeviceMatrox_TapsSelector eRegionsX = EDeviceMatrox_TapsSelector_Count;
		EDeviceMatrox_TapsSelector eRegionsY = EDeviceMatrox_TapsSelector_Count;
		EDeviceMatrox_TapsDirection eTaps1Dir = EDeviceMatrox_TapsDirection_Count;
		EDeviceMatrox_TapsDirection eTaps2Dir = EDeviceMatrox_TapsDirection_Count;
		EDeviceMatrox_TapsDirection eTaps3Dir = EDeviceMatrox_TapsDirection_Count;
		EDeviceMatrox_TapsDirection eTaps4Dir = EDeviceMatrox_TapsDirection_Count;
		EDeviceMatrox_TapsDirection eTaps5Dir = EDeviceMatrox_TapsDirection_Count;
		EDeviceMatrox_TapsDirection eTaps6Dir = EDeviceMatrox_TapsDirection_Count;
		EDeviceMatrox_TapsDirection eTaps7Dir = EDeviceMatrox_TapsDirection_Count;
		EDeviceMatrox_TapsDirection eTaps8Dir = EDeviceMatrox_TapsDirection_Count;
		EDeviceMatrox_TapsSelector eAdjacentX = EDeviceMatrox_TapsSelector_Count;
		EDeviceMatrox_TapsSelector eAdjacentY = EDeviceMatrox_TapsSelector_Count;
		EDeviceMatrox_VideoSignalType eVideoSignalType = EDeviceMatrox_VideoSignalType_Count;
		EDeviceMatrox_DataBusWidth eDataBusWidth = EDeviceMatrox_DataBusWidth_Count;
		EDeviceMatrox_Standard eVideoStandard = EDeviceMatrox_Standard_Count;
		EDeviceMatrox_MILChannel eMilChannel = EDeviceMatrox_MILChannel_Count;
		EDeviceMatrox_InoutFormat eDigitalVideoFormat = EDeviceMatrox_InoutFormat_Count;
		EDeviceMatrox_ValidSignalSource eValidSignalSource = EDeviceMatrox_ValidSignalSource_Count;
		EDeviceMatrox_InterlacedSelector eTimingsVertical = EDeviceMatrox_InterlacedSelector_Count;

		int nHorizontalSync = 0;
		int nHorizontalBProch = 0;
		int nHorizontalActive = 0;
		int nHorizontalFProch = 0;
		int nHorizontalTotal = 0;
		double dblHorizontalFrequencyHz = 0.;
		bool bHorizontalLockActiveAndTotal = false;
		int nVerticalSync = 0;
		int nVerticalBProch = 0;
		int nVerticalActive = 0;
		int nVerticalFProch = 0;
		int nVerticalTotal = 0;
		double dblVerticalFrequencyHz = 0.;
		bool bVerticalLockActiveAndTotal = false;
		
		int nImageSizeX = 0;
		int nImageSizeY = 0;
		int nImageDelayX = 0;
		int nImageDelayY = 0;

		double dblPixelClock = 0.;

		EDeviceMatrox_ExternalClockSignal eExternalClockSignal = EDeviceMatrox_ExternalClockSignal_Count;
		EDeviceMatrox_PixelClockFrequency eIOInputFrequency = EDeviceMatrox_PixelClockFrequency_Count;
		EDeviceMatrox_InoutFormat eIOInputFormat = EDeviceMatrox_InoutFormat_Count;
		EDeviceMatrox_InoutPolarity eIOInputPolarity = EDeviceMatrox_InoutPolarity_Count;

		EDeviceMatrox_PixelClockFrequency eIOOutputFrequency = EDeviceMatrox_PixelClockFrequency_Count;
		EDeviceMatrox_InoutFormat eIOOutputFormat = EDeviceMatrox_InoutFormat_Count;
		EDeviceMatrox_InoutPolarity eIOOutputPolarity = EDeviceMatrox_InoutPolarity_Count;

		int nIODelay = 0;

		EDeviceMatrox_GenerationMode eTimer1Mode = EDeviceMatrox_GenerationMode_Count;
		EDeviceMatrox_InoutFormatDefault eTimer1ExpFormat = EDeviceMatrox_InoutFormatDefault_Count;
		EDeviceMatrox_InoutPolarity eTimer1ExpPolarity = EDeviceMatrox_InoutPolarity_Count;
		EDeviceMatrox_InoutFormatDefault eTimer1TrgFormat = EDeviceMatrox_InoutFormatDefault_Count;
		EDeviceMatrox_ExposureTriggerSignal eTimer1TrgSignal = EDeviceMatrox_ExposureTriggerSignal_Count;
		EDeviceMatrox_InoutPolarity eTimer1TrgPolarity = EDeviceMatrox_InoutPolarity_Count;

		int nTimer1Pulse1 = 0;
		int nTimer1Delay1 = 0;
		int nTimer1Pulse2 = 0;
		int nTimer1Delay2 = 0;

		EDeviceMatrox_ExposureClockType eTimer1ExpType = EDeviceMatrox_ExposureClockType_Count;
		EDeviceMatrox_DivisionFactor eTimer1ExpDiv = EDeviceMatrox_DivisionFactor_Count;

		double dblTimer1ClockFreq = 0.;

		EDeviceMatrox_GenerationMode eTimer2Mode = EDeviceMatrox_GenerationMode_Count;
		EDeviceMatrox_InoutFormatDefault eTimer2ExpFormat = EDeviceMatrox_InoutFormatDefault_Count;
		EDeviceMatrox_InoutPolarity eTimer2ExpPolarity = EDeviceMatrox_InoutPolarity_Count;
		EDeviceMatrox_InoutFormatDefault eTimer2TrgFormat = EDeviceMatrox_InoutFormatDefault_Count;
		EDeviceMatrox_ExposureTriggerSignal eTimer2TrgSignal = EDeviceMatrox_ExposureTriggerSignal_Count;
		EDeviceMatrox_InoutPolarity eTimer2TrgPolarity = EDeviceMatrox_InoutPolarity_Count;

		int nTimer2Pulse1 = 0;
		int nTimer2Delay1 = 0;
		int nTimer2Pulse2 = 0;
		int nTimer2Delay2 = 0;

		EDeviceMatrox_ExposureClockType eTimer2ExpType = EDeviceMatrox_ExposureClockType_Count;
		EDeviceMatrox_DivisionFactor eTimer2ExpDiv = EDeviceMatrox_DivisionFactor_Count;

		double dblTimer2ClockFreq = 0.;

		EDeviceMatrox_TimerOutputSignal eTimer1AdvancedSignal = EDeviceMatrox_TimerOutputSignal_Count;
		EDeviceMatrox_CaptureATriggerEveryTrigger eTimer1AdvancedTrigger = EDeviceMatrox_CaptureATriggerEveryTrigger_Count;
		EDeviceMatrox_TriggerArmMode eTimer1AdvancedArmMode = EDeviceMatrox_TriggerArmMode_Count;
		EDeviceMatrox_InoutFormatDefault eTimer1AdvancedArmFormat = EDeviceMatrox_InoutFormatDefault_Count;
		EDeviceMatrox_TriggerArmSource eTimer1AdvancedArmSource = EDeviceMatrox_TriggerArmSource_Count;
		EDeviceMatrox_InoutPolarity eTimer1AdvancedArmPolarity = EDeviceMatrox_InoutPolarity_Count;

		EDeviceMatrox_TimerOutputSignal eTimer2AdvancedSignal = EDeviceMatrox_TimerOutputSignal_Count;
		EDeviceMatrox_CaptureATriggerEveryTrigger eTimer2AdvancedTrigger = EDeviceMatrox_CaptureATriggerEveryTrigger_Count;
		EDeviceMatrox_TriggerArmMode eTimer2AdvancedArmMode = EDeviceMatrox_TriggerArmMode_Count;
		EDeviceMatrox_InoutFormatDefault eTimer2AdvancedArmFormat = EDeviceMatrox_InoutFormatDefault_Count;
		EDeviceMatrox_TriggerArmSource eTimer2AdvancedArmSource = EDeviceMatrox_TriggerArmSource_Count;
		EDeviceMatrox_InoutPolarity eTimer2AdvancedArmPolarity = EDeviceMatrox_InoutPolarity_Count;

		EDeviceMatrox_GrabMode eGrabMode = EDeviceMatrox_GrabMode_Count;
		EDeviceMatrox_GrabModeActivation eGrabModeActivation = EDeviceMatrox_GrabModeActivation_Count;
		EDeviceMatrox_InoutFormatDefault eTrgFormat = EDeviceMatrox_InoutFormatDefault_Count;
		EDeviceMatrox_GrabModeSignal eTrgSignal = EDeviceMatrox_GrabModeSignal_Count;
		EDeviceMatrox_InoutPolarity eTrgPolarity = EDeviceMatrox_InoutPolarity_Count;
		EDeviceMatrox_TriggerArmSignal eTrgArmSignal = EDeviceMatrox_TriggerArmSignal_Count;
		EDeviceMatrox_InoutPolarity eTrgArmPolarity = EDeviceMatrox_InoutPolarity_Count;
		
		EDeviceMatrox_SyncSignalSource eSyncSource = EDeviceMatrox_SyncSignalSource_Count;
		EDeviceMatrox_SyncSignalFormat eSyncFormat = EDeviceMatrox_SyncSignalFormat_Count;
		EDeviceMatrox_SynchronizAtionSignalAvailable eSyncAvaliable = EDeviceMatrox_SynchronizAtionSignalAvailable_Count;

		bool bHsyncInputActive = false;
		EDeviceMatrox_InoutFormat eHsyncInputFormat = EDeviceMatrox_InoutFormat_Count;
		EDeviceMatrox_InoutPolarity eHsyncInputPolarity = EDeviceMatrox_InoutPolarity_Count;
		bool bHsyncOutputActive = false;
		EDeviceMatrox_InoutFormat eHsyncOutputFormat = EDeviceMatrox_InoutFormat_Count;
		EDeviceMatrox_InoutPolarity eHsyncOutputPolarity = EDeviceMatrox_InoutPolarity_Count;

		bool bVsyncInputActive = false;
		EDeviceMatrox_InoutFormat eVsyncInputFormat = EDeviceMatrox_InoutFormat_Count;
		EDeviceMatrox_InoutPolarity eVsyncInputPolarity = EDeviceMatrox_InoutPolarity_Count;
		bool bVsyncOutputActive = false;
		EDeviceMatrox_InoutFormat eVsyncOutputFormat = EDeviceMatrox_InoutFormat_Count;
		EDeviceMatrox_InoutPolarity eVsyncOutputPolarity = EDeviceMatrox_InoutPolarity_Count;

		bool bCsyncInputActive = false;
		EDeviceMatrox_InoutFormat eCsyncInputFormat = EDeviceMatrox_InoutFormat_Count;
		EDeviceMatrox_InoutPolarity eCsyncInputPolarity = EDeviceMatrox_InoutPolarity_Count;
		bool bCsyncOutputActive = false;
		EDeviceMatrox_InoutFormat eCsyncOutputFormat = EDeviceMatrox_InoutFormat_Count;
		EDeviceMatrox_InoutPolarity eCsyncOutputPolarity = EDeviceMatrox_InoutPolarity_Count;

		EDeviceMatrox_CameraBitsSourcesControl eCameraControl1 = EDeviceMatrox_CameraBitsSourcesControl_Count;
		EDeviceMatrox_CameraBitsSourcesControl eCameraControl2 = EDeviceMatrox_CameraBitsSourcesControl_Count;
		EDeviceMatrox_CameraBitsSourcesControl eCameraControl3 = EDeviceMatrox_CameraBitsSourcesControl_Count;
		EDeviceMatrox_CameraBitsSourcesControl eCameraControl4 = EDeviceMatrox_CameraBitsSourcesControl_Count;
		bool bEnableConnector1 = false;
		bool bEnableConnector2 = false;

		if(GetCameraType(&eCameraType))
			break;

		if(eCameraType >= EDeviceMatrox_CameraType_Count)
			break;

		if(GetCameraNumberOfTaps(&eCameraNumberOfTaps))
			break;

		if(eCameraNumberOfTaps >= EDeviceMatrox_NumberOfTaps_Count)
			break;

		if(GetCameraBayerMode(&eBayerMode))
			break;

		if(eBayerMode >= EDeviceMatrox_BayerMode_Count)
			break;

		if(GetCameraLinkConfiguration(&eConfigurationType))
			break;

		if(eConfigurationType >= EDeviceMatrox_ConfigurationType_Count)
			break;

		if(GetCameraMode(&eCameraTapsMode))
			break;

		if(eCameraTapsMode >= EDeviceMatrox_CameraTapsMode_Count)
			break;

		if(GetRegionsX(&eRegionsX))
			break;

		if(eRegionsX >= EDeviceMatrox_TapsSelector_Count)
			break;

		if(GetRegionsY(&eRegionsY))
			break;

		if(eRegionsY >= EDeviceMatrox_TapsSelector_Count)
			break;

		if(GetTaps1Direction(&eTaps1Dir))
			break;

		if(eTaps1Dir >= EDeviceMatrox_TapsDirection_Count)
			break;

		if(GetTaps2Direction(&eTaps2Dir))
			break;

		if(eTaps2Dir >= EDeviceMatrox_TapsDirection_Count)
			break;

		if(GetTaps3Direction(&eTaps3Dir))
			break;

		if(eTaps3Dir >= EDeviceMatrox_TapsDirection_Count)
			break;

		if(GetTaps4Direction(&eTaps4Dir))
			break;

		if(eTaps4Dir >= EDeviceMatrox_TapsDirection_Count)
			break;

		if(GetTaps5Direction(&eTaps5Dir))
			break;

		if(eTaps5Dir >= EDeviceMatrox_TapsDirection_Count)
			break;

		if(GetTaps6Direction(&eTaps6Dir))
			break;

		if(eTaps6Dir >= EDeviceMatrox_TapsDirection_Count)
			break;

		if(GetTaps7Direction(&eTaps7Dir))
			break;

		if(eTaps7Dir >= EDeviceMatrox_TapsDirection_Count)
			break;

		if(GetTaps8Direction(&eTaps8Dir))
			break;

		if(eTaps8Dir >= EDeviceMatrox_TapsDirection_Count)
			break;

		if(GetAdjacentPixelsX(&eAdjacentX))
			break;

		if(eAdjacentX >= EDeviceMatrox_TapsSelector_Count)
			break;

		if(GetAdjacentPixelsY(&eAdjacentY))
			break;

		if(eAdjacentY >= EDeviceMatrox_TapsSelector_Count)
			break;

		if(GetVideoSignalType(&eVideoSignalType))
			break;

		if(eVideoSignalType >= EDeviceMatrox_VideoSignalType_Count)
			break;

		if(GetDataBusWidth(&eDataBusWidth))
			break;

		if(eDataBusWidth >= EDeviceMatrox_DataBusWidth_Count)
			break;

		if(GetVideoSignalStandard(&eVideoStandard))
			break;

		if(eVideoStandard >= EDeviceMatrox_Standard_Count)
			break;

		if(GetMILChannel(&eMilChannel))
			break;

		if(eMilChannel >= EDeviceMatrox_MILChannel_Count)
			break;

		if(GetVideoSignalFormat(&eDigitalVideoFormat))
			break;

		if(eDigitalVideoFormat >= EDeviceMatrox_InoutFormat_Count)
			break;

		if(GetVideoSignalSource(&eValidSignalSource))
			break;

		if(eValidSignalSource >= EDeviceMatrox_ValidSignalSource_Count)
			break;

		if(GetTimingsVertical(&eTimingsVertical))
			break;

		if(eTimingsVertical >= EDeviceMatrox_InterlacedSelector_Count)
			break;

		if(GetHorizontalSyncClk(&nHorizontalSync))
			break;

		if(GetHorizontalBPorchClk(&nHorizontalBProch))
			break;

		if(GetHorizontalActiveClk(&nHorizontalActive))
			break;

		if(GetHorizontalFPorchClk(&nHorizontalFProch))
			break;

		if(GetHorizontalFrequencyHz(&dblHorizontalFrequencyHz))
			break;

		if(GetHorizontalLockActiveAndTotal(&bHorizontalLockActiveAndTotal))
			break;

		if(GetVerticalSyncLines(&nVerticalSync))
			break;

		if(GetVerticalBPorchLines(&nVerticalBProch))
			break;

		if(GetVerticalActiveLines(&nVerticalActive))
			break;

		if(GetVerticalFPorchLines(&nVerticalFProch))
			break;

		if(GetVerticalFrequencyHz(&dblVerticalFrequencyHz))
			break;

		if(GetVerticalLockActiveAndTotal(&bVerticalLockActiveAndTotal))
			break;

		if(GetVideoTimingImagesizeX(&nImageSizeX))
			break;

		if(GetVideoTimingImagesizeY(&nImageSizeY))
			break;

		if(GetVideoTimingDelayX(&nImageDelayX))
			break;

		if(GetVideoTimingDelayY(&nImageDelayY))
			break;

		if(GetPixelClockFrequency(&dblPixelClock))
			break;

		if(GetExternalClockSignal(&eExternalClockSignal))
			break;

		if(eExternalClockSignal >= EDeviceMatrox_ExternalClockSignal_Count)
			break;

		if(GetPixelClockInputFrequency(&eIOInputFrequency))
			break;

		if(eIOInputFrequency >= EDeviceMatrox_PixelClockFrequency_Count)
			break;

		if(GetPixelClockInputFormat(&eIOInputFormat))
			break;

		if(eIOInputFormat >= EDeviceMatrox_InoutFormat_Count)
			break;

		if(GetPixelClockInputPolarity(&eIOInputPolarity))
			break;

		if(eIOInputPolarity >= EDeviceMatrox_InoutPolarity_Count)
			break;

		if(GetPixelClockOutputFrequency(&eIOInputFrequency))
			break;

		if(eIOInputFrequency >= EDeviceMatrox_PixelClockFrequency_Count)
			break;

		if(GetPixelClockOutputFormat(&eIOInputFormat))
			break;

		if(eIOInputFormat >= EDeviceMatrox_InoutFormat_Count)
			break;

		if(GetPixelClockOutputPolarity(&eIOInputPolarity))
			break;

		if(eIOInputPolarity >= EDeviceMatrox_InoutPolarity_Count)
			break;

		if(GetPixelClockDelay(&nIODelay))
			break;

		if(GetTimer1Mode(&eTimer1Mode))
			break;

		if(eTimer1Mode >= EDeviceMatrox_GenerationMode_Count)
			break;

		if(GetTimer1Format(&eTimer1ExpFormat))
			break;

		if(eTimer1ExpFormat >= EDeviceMatrox_InoutFormatDefault_Count)
			break;

		if(GetTimer1Polarity(&eTimer1ExpPolarity))
			break;

		if(eTimer1ExpPolarity >= EDeviceMatrox_InoutPolarity_Count)
			break;

		if(GetTimer1TrgFormat(&eTimer1TrgFormat))
			break;

		if(eTimer1TrgFormat >= EDeviceMatrox_InoutFormatDefault_Count)
			break;

		if(GetTimer1TrgSignal(&eTimer1TrgSignal))
			break;

		if(eTimer1TrgSignal >= EDeviceMatrox_ExposureTriggerSignal_Count)
			break;

		if(GetTimer1TrgPolarity(&eTimer1TrgPolarity))
			break;

		if(eTimer1TrgPolarity >= EDeviceMatrox_InoutPolarity_Count)
			break;

		if(GetTimer1Pulse1Clk(&nTimer1Pulse1))
			break;

		if(GetTimer1Delay1Clk(&nTimer1Delay1))
			break;

		if(GetTimer1Pulse2Clk(&nTimer1Pulse2))
			break;

		if(GetTimer1Delay2Clk(&nTimer1Delay2))
			break;

		if(GetTimer1ExposureClockType(&eTimer1ExpType))
			break;

		if(eTimer1ExpType >= EDeviceMatrox_ExposureClockType_Count)
			break;

		if(GetTimer1ExposureClockDivisionFactor(&eTimer1ExpDiv))
			break;

		if(eTimer1ExpDiv >= EDeviceMatrox_DivisionFactor_Count)
			break;

		if(GetTimer1ExposureClockBaseClockFrequency(&dblTimer1ClockFreq))
			break;

		if(GetTimer2Mode(&eTimer2Mode))
			break;

		if(eTimer2Mode >= EDeviceMatrox_GenerationMode_Count)
			break;

		if(GetTimer2Format(&eTimer2ExpFormat))
			break;

		if(eTimer2ExpFormat >= EDeviceMatrox_InoutFormatDefault_Count)
			break;

		if(GetTimer2Polarity(&eTimer2ExpPolarity))
			break;

		if(eTimer2ExpPolarity >= EDeviceMatrox_InoutPolarity_Count)
			break;

		if(GetTimer2TrgFormat(&eTimer2TrgFormat))
			break;

		if(eTimer2TrgFormat >= EDeviceMatrox_InoutFormatDefault_Count)
			break;

		if(GetTimer2TrgSignal(&eTimer2TrgSignal))
			break;

		if(eTimer2TrgSignal >= EDeviceMatrox_ExposureTriggerSignal_Count)
			break;

		if(GetTimer2TrgPolarity(&eTimer2TrgPolarity))
			break;

		if(eTimer2TrgPolarity >= EDeviceMatrox_InoutPolarity_Count)
			break;

		if(GetTimer2Pulse1Clk(&nTimer2Pulse1))
			break;

		if(GetTimer2Delay1Clk(&nTimer2Delay1))
			break;

		if(GetTimer2Pulse2Clk(&nTimer2Pulse2))
			break;

		if(GetTimer2Delay2Clk(&nTimer2Delay2))
			break;

		if(GetTimer2ExposureClockType(&eTimer2ExpType))
			break;

		if(eTimer2ExpType >= EDeviceMatrox_ExposureClockType_Count)
			break;

		if(GetTimer2ExposureClockDivisionFactor(&eTimer2ExpDiv))
			break;

		if(eTimer2ExpDiv >= EDeviceMatrox_DivisionFactor_Count)
			break;

		if(GetTimer2ExposureClockBaseClockFrequency(&dblTimer2ClockFreq))
			break;

		if(GetTimer1AdvancedTimerOutputSignal(&eTimer1AdvancedSignal))
			break;

		if(eTimer1AdvancedSignal >= EDeviceMatrox_TimerOutputSignal_Count)
			break;

		if(GetTimer1AdvancedCaptureTrigger(&eTimer1AdvancedTrigger))
			break;

		if(eTimer1AdvancedTrigger >= EDeviceMatrox_CaptureATriggerEveryTrigger_Count)
			break;

		if(GetTimer1AdvancedTriggerArmMode(&eTimer1AdvancedArmMode))
			break;

		if(eTimer1AdvancedArmMode >= EDeviceMatrox_TriggerArmMode_Count)
			break;

		if(GetTimer1AdvancedTriggerArmFormat(&eTimer1AdvancedArmFormat))
			break;

		if(eTimer1AdvancedArmFormat >= EDeviceMatrox_InoutFormatDefault_Count)
			break;

		if(GetTimer1AdvancedTriggerArmSource(&eTimer1AdvancedArmSource))
			break;

		if(eTimer1AdvancedArmSource >= EDeviceMatrox_TriggerArmSource_Count)
			break;

		if(GetTimer1AdvancedTriggerArmPolarity(&eTimer1AdvancedArmPolarity))
			break;

		if(eTimer1AdvancedArmPolarity >= EDeviceMatrox_InoutPolarity_Count)
			break;

		if(GetTimer2AdvancedTimerOutputSignal(&eTimer2AdvancedSignal))
			break;

		if(eTimer2AdvancedSignal >= EDeviceMatrox_TimerOutputSignal_Count)
			break;

		if(GetTimer2AdvancedCaptureATrigger(&eTimer2AdvancedTrigger))
			break;

		if(eTimer2AdvancedTrigger >= EDeviceMatrox_CaptureATriggerEveryTrigger_Count)
			break;

		if(GetTimer2AdvancedTriggerArmMode(&eTimer2AdvancedArmMode))
			break;

		if(eTimer2AdvancedArmMode >= EDeviceMatrox_TriggerArmMode_Count)
			break;

		if(GetTimer2AdvancedTriggerArmFormat(&eTimer2AdvancedArmFormat))
			break;

		if(eTimer2AdvancedArmFormat >= EDeviceMatrox_InoutFormatDefault_Count)
			break;

		if(GetTimer2AdvancedTriggerArmSource(&eTimer2AdvancedArmSource))
			break;

		if(eTimer2AdvancedArmSource >= EDeviceMatrox_TriggerArmSource_Count)
			break;

		if(GetTimer2AdvancedTriggerArmPolarity(&eTimer2AdvancedArmPolarity))
			break;

		if(eTimer2AdvancedArmPolarity >= EDeviceMatrox_InoutPolarity_Count)
			break;

		if(GetGrabMode(&eGrabMode))
			break;

		if(eGrabMode >= EDeviceMatrox_GrabMode_Count)
			break;

		if(GetGrabActivationMode(&eGrabModeActivation))
			break;

		if(eGrabModeActivation >= EDeviceMatrox_GrabModeActivation_Count)
			break;

		if(GetGrabFormat(&eTrgFormat))
			break;

		if(eTrgFormat >= EDeviceMatrox_InoutFormatDefault_Count)
			break;

		if(GetGrabSignal(&eTrgSignal))
			break;

		if(eTrgSignal >= EDeviceMatrox_GrabModeSignal_Count)
			break;

		if(GetGrabPolarity(&eTrgPolarity))
			break;

		if(eTrgPolarity >= EDeviceMatrox_InoutPolarity_Count)
			break;

		if(GetGrabTriggerArmSignal(&eTrgArmSignal))
			break;

		if(eTrgArmSignal >= EDeviceMatrox_TriggerArmSignal_Count)
			break;

		if(GetGrabTriggerArmPolarity(&eTrgArmPolarity))
			break;

		if(eTrgArmPolarity >= EDeviceMatrox_InoutPolarity_Count)
			break;

		if(GetSyncSignalSource(&eSyncSource))
			break;

		if(eSyncSource >= EDeviceMatrox_SyncSignalSource_Count)
			break;

		if(GetSyncSignalFormat(&eSyncFormat))
			break;

		if(eSyncFormat >= EDeviceMatrox_SyncSignalFormat_Count)
			break;

		if(GetSyncSignalSynchronizAtionSignalAvailable(&eSyncAvaliable))
			break;

		if(eSyncAvaliable >= EDeviceMatrox_SynchronizAtionSignalAvailable_Count)
			break;

		if(GetHSyncInputActivce(&bHsyncInputActive))
			break;

		if(GetHSyncInputFormat(&eHsyncInputFormat))
			break;

		if(eHsyncInputFormat >= EDeviceMatrox_InoutFormat_Count)
			break;

		if(GetHSyncInputPolarity(&eHsyncInputPolarity))
			break;

		if(eHsyncInputPolarity >= EDeviceMatrox_InoutPolarity_Count)
			break;

		if(GetHSyncOutputActivce(&bHsyncOutputActive))
			break;

		if(GetHSyncOutputFormat(&eHsyncOutputFormat))
			break;

		if(eHsyncOutputFormat >= EDeviceMatrox_InoutFormat_Count)
			break;

		if(GetHSyncOutputPolarity(&eHsyncOutputPolarity))
			break;

		if(eHsyncOutputPolarity >= EDeviceMatrox_InoutPolarity_Count)
			break;

		if(GetVSyncInputActivce(&bVsyncInputActive))
			break;

		if(GetVSyncInputFormat(&eVsyncInputFormat))
			break;

		if(eVsyncInputFormat >= EDeviceMatrox_InoutFormat_Count)
			break;

		if(GetVSyncInputPolarity(&eVsyncInputPolarity))
			break;

		if(eVsyncInputPolarity >= EDeviceMatrox_InoutPolarity_Count)
			break;

		if(GetVSyncOutputActivce(&bVsyncOutputActive))
			break;

		if(GetVSyncOutputFormat(&eVsyncOutputFormat))
			break;

		if(eVsyncOutputFormat >= EDeviceMatrox_InoutFormat_Count)
			break;

		if(GetVSyncOutputPolarity(&eVsyncOutputPolarity))
			break;

		if(eVsyncOutputPolarity >= EDeviceMatrox_InoutPolarity_Count)
			break;

		if(GetCSyncInputActivce(&bCsyncInputActive))
			break;

		if(GetCSyncInputFormat(&eCsyncInputFormat))
			break;

		if(eCsyncInputFormat >= EDeviceMatrox_InoutFormat_Count)
			break;

		if(GetCSyncInputPolarity(&eCsyncInputPolarity))
			break;

		if(eCsyncInputPolarity >= EDeviceMatrox_InoutPolarity_Count)
			break;

		if(GetCSyncOutputActivce(&bCsyncOutputActive))
			break;

		if(GetCSyncOutputFormat(&eCsyncOutputFormat))
			break;

		if(eCsyncOutputFormat >= EDeviceMatrox_InoutFormat_Count)
			break;

		if(GetCSyncOutputPolarity(&eCsyncOutputPolarity))
			break;

		if(eCsyncOutputPolarity >= EDeviceMatrox_InoutPolarity_Count)
			break;
				
		if(GetCameraControlBitsSourcesControl1(&eCameraControl1))
			break;

		if(eCameraControl1 >= EDeviceMatrox_CameraBitsSourcesControl_Count)
			break;

		if(GetCameraControlBitsSourcesControl2(&eCameraControl2))
			break;

		if(eCameraControl2 >= EDeviceMatrox_CameraBitsSourcesControl_Count)
			break;

		if(GetCameraControlBitsSourcesControl3(&eCameraControl3))
			break;

		if(eCameraControl3 >= EDeviceMatrox_CameraBitsSourcesControl_Count)
			break;

		if(GetCameraControlBitsSourcesControl4(&eCameraControl4))
			break;

		if(eCameraControl4 >= EDeviceMatrox_CameraBitsSourcesControl_Count)
			break;

		if(GetEnableCCOutputsOnConnector1(&bEnableConnector1))
			break;

		if(GetEnableCCOutputsOnConnector2(&bEnableConnector2))
			break;

		//////////////////////////////////////////////////////////////////////////

		//g_lpszGeneral_Parameters
		//EMatoxDCF_EOFGeneral_Parameters

		bool bError = false;

		int nParamIdx = 0;
		int nSeqIdx = 0;

		m_vctMappingData.push_back(g_lpszGeneral_Parameters[nParamIdx++]);
		m_vctMappingData.push_back(g_lpszGeneral_Parameters[nParamIdx++]);
		m_vctMappingData.push_back(g_lpszGeneral_Parameters[nParamIdx++]);

		switch(eCameraType)
		{
		case EDeviceMatrox_CameraType_FrameScan:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_CameraType_LineScan:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		}

		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);

		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], eCameraNumberOfTaps);
		m_vctMappingData.push_back(strMapping);

		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], _ttoi(GetDeviceID()));
		m_vctMappingData.push_back(strMapping);

		switch(eBayerMode)
		{
		case EDeviceMatrox_BayerMode_Disable:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_BayerMode_Blue_Green:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_BayerMode_Green_Blue:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_BayerMode_Green_Red:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_BayerMode_Red_Green:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		}

		switch(eVideoSignalType)
		{
		case EDeviceMatrox_VideoSignalType_Digital:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_VideoSignalType_Analog:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		}

		switch(eVideoStandard)
		{
		case EDeviceMatrox_Standard_Monochrome:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_Standard_RGBColor:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_Standard_RGBPack:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_Standard_RGBAlpha:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_Standard_SVID:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_Standard_YUVVID:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		}

		switch(eDigitalVideoFormat)
		{
		case EDeviceMatrox_InoutFormat_TTL:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_InoutFormat_422:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_InoutFormat_OPTO:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_InoutFormat_LVDS:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		}

		switch(eDataBusWidth)
		{
		case EDeviceMatrox_DataBusWidth_8bits:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_DataBusWidth_10bits:
		case EDeviceMatrox_DataBusWidth_12bits:
		case EDeviceMatrox_DataBusWidth_14bits:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_DataBusWidth_16bits:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_DataBusWidth_24bits:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_DataBusWidth_32bits:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_DataBusWidth_64bits:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		}

		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping); 
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);

		switch(eMilChannel)
		{
		case EDeviceMatrox_MILChannel_0:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_MILChannel_1:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_MILChannel_2:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_MILChannel_3:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		}

		nSeqIdx = nParamIdx + 37;

		for(;nParamIdx < nSeqIdx;)
			m_vctMappingData.push_back(g_lpszGeneral_Parameters[nParamIdx++]);

		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], bHorizontalLockActiveAndTotal);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], bVerticalLockActiveAndTotal);
		m_vctMappingData.push_back(strMapping);

		nSeqIdx = nParamIdx + 7;

		for(; nParamIdx < nSeqIdx;)
			m_vctMappingData.push_back(g_lpszGeneral_Parameters[nParamIdx++]);

		if(eCameraType == EDeviceMatrox_CameraType_FrameScan)
		{
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
		}
		else
		{
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
			m_vctMappingData.push_back(strMapping);
		}

		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nHorizontalSync);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nHorizontalBProch);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nHorizontalFProch);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nHorizontalActive);
		m_vctMappingData.push_back(strMapping);

		nHorizontalTotal = nHorizontalSync + nHorizontalBProch + nHorizontalFProch + nHorizontalActive;

		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nHorizontalTotal);
		m_vctMappingData.push_back(strMapping);

		dblHorizontalFrequencyHz = ((dblPixelClock * 1000000) / (double)(!nHorizontalTotal ? 1 : nHorizontalTotal)) + .5;

		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], (int)dblHorizontalFrequencyHz);
		m_vctMappingData.push_back(strMapping);

		switch(eCameraType)
		{
		case EDeviceMatrox_CameraType_FrameScan:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nVerticalSync);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nVerticalBProch);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nVerticalFProch);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nVerticalActive);
				m_vctMappingData.push_back(strMapping);

				nVerticalTotal = nVerticalSync + nVerticalBProch + nVerticalFProch + nVerticalActive;

				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nVerticalTotal);
				m_vctMappingData.push_back(strMapping);

				dblVerticalFrequencyHz = (dblHorizontalFrequencyHz / (double)nVerticalTotal) + .5;

				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], (int)dblVerticalFrequencyHz);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_CameraType_LineScan:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nVerticalActive);
				m_vctMappingData.push_back(strMapping);

				nVerticalTotal = nVerticalActive;

				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nVerticalTotal);
				m_vctMappingData.push_back(strMapping);
				
				dblVerticalFrequencyHz = 0.;

				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], (int)dblVerticalFrequencyHz);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		}

		nSeqIdx = nParamIdx + 4;

		for(; nParamIdx < nSeqIdx;)
			m_vctMappingData.push_back(g_lpszGeneral_Parameters[nParamIdx++]);

		switch(eTimingsVertical)
		{
		case EDeviceMatrox_InterlacedSelector_Interlaced:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_InterlacedSelector_Non_Interlaced:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		}

		nSeqIdx = nParamIdx + 5;

		for(; nParamIdx < nSeqIdx;)
			m_vctMappingData.push_back(g_lpszGeneral_Parameters[nParamIdx++]);

		switch(eExternalClockSignal)
		{
		case EDeviceMatrox_ExternalClockSignal_NoClockExchange:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);

				int nPixelClock = (int)(dblPixelClock * 1000000.);

				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nPixelClock);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nPixelClock);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_ExternalClockSignal_GeneratedByCamera:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);

				switch(eIOInputFrequency)
				{
				case EDeviceMatrox_PixelClockFrequency_1PixelClock:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				}
				
				switch(eIOInputFormat)
				{
				case EDeviceMatrox_InoutFormat_TTL:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_InoutFormat_422:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_InoutFormat_OPTO:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_InoutFormat_LVDS:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				}

				switch(eIOInputPolarity)
				{
				case EDeviceMatrox_InoutPolarity_PosEdgeTrig:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_InoutPolarity_NegEdgeTrig:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				}

				int nPixelClock = (int)(dblPixelClock * 1000000.);

				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nPixelClock);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nPixelClock);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_ExternalClockSignal_GeneratedByDigitizer:
		case EDeviceMatrox_ExternalClockSignal_GeneratedByDigitizerAndReturnedByCamera:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);

				switch(eIOOutputFrequency)
				{
				case EDeviceMatrox_PixelClockFrequency_1PixelClock:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				}

				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);

				int nPixelClock = dblPixelClock * 1000000;

				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nPixelClock);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nPixelClock);
				m_vctMappingData.push_back(strMapping);

				switch(eIOOutputFormat)
				{
				case EDeviceMatrox_InoutFormat_TTL:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_InoutFormat_422:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_InoutFormat_OPTO:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_InoutFormat_LVDS:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				}

				switch(eIOOutputPolarity)
				{
				case EDeviceMatrox_InoutPolarity_PosEdgeTrig:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_InoutPolarity_NegEdgeTrig:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				}

				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		}

		switch(eSyncFormat)
		{
		case EDeviceMatrox_SyncSignalFormat_Digital:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_SyncSignalFormat_Analog:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		}

		switch(eSyncSource)
		{
		case EDeviceMatrox_SyncSignalSource_Digitizer:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_SyncSignalSource_Camera:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		}

		switch(eSyncAvaliable)
		{
		case EDeviceMatrox_SynchronizAtionSignalAvailable_CSync:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_SynchronizAtionSignalAvailable_HSync_And_VSync:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_SynchronizAtionSignalAvailable_VSync_Only:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_SynchronizAtionSignalAvailable_HSync_Only:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		}

		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);

		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], bHsyncInputActive);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], bHsyncOutputActive);
		m_vctMappingData.push_back(strMapping);

		if(bHsyncInputActive)
		{
			switch(eHsyncInputFormat)
			{
			case EDeviceMatrox_InoutFormat_TTL:
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
				break;
			case EDeviceMatrox_InoutFormat_422:
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
				break;
			case EDeviceMatrox_InoutFormat_OPTO:
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
				break;
			case EDeviceMatrox_InoutFormat_LVDS:
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);
				}
				break;
			}

			switch(eHsyncInputPolarity)
			{
			case EDeviceMatrox_InoutPolarity_PosEdgeTrig:
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
				break;
			case EDeviceMatrox_InoutPolarity_NegEdgeTrig:
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);
				}
				break;
			}
		}
		else
		{
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
		}

		if(bHsyncOutputActive)
		{
			switch(eHsyncOutputFormat)
			{
			case EDeviceMatrox_InoutFormat_TTL:
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);

				}
				break;
			case EDeviceMatrox_InoutFormat_422:
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
				break;
			case EDeviceMatrox_InoutFormat_OPTO:
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
				break;
			case EDeviceMatrox_InoutFormat_LVDS:
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);
				}
				break;
			}
			
			switch(eHsyncOutputPolarity)
			{
			case EDeviceMatrox_InoutPolarity_PosEdgeTrig:
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
				break;
			case EDeviceMatrox_InoutPolarity_NegEdgeTrig:
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);
				}
				break;
			}
		}
		else
		{
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
		}

		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], bVsyncInputActive);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], bVsyncOutputActive);
		m_vctMappingData.push_back(strMapping);

		if(bVsyncInputActive)
		{
			switch(eVsyncInputFormat)
			{
			case EDeviceMatrox_InoutFormat_TTL:
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
				break;
			case EDeviceMatrox_InoutFormat_422:
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
				break;
			case EDeviceMatrox_InoutFormat_OPTO:
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
				break;
			case EDeviceMatrox_InoutFormat_LVDS:
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);
				}
				break;
			}

			switch(eVsyncInputPolarity)
			{
			case EDeviceMatrox_InoutPolarity_PosEdgeTrig:
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
				break;
			case EDeviceMatrox_InoutPolarity_NegEdgeTrig:
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);
				}
				break;
			}
		}
		else
		{
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
		}

		if(bVsyncOutputActive)
		{
			switch(eVsyncOutputFormat)
			{
			case EDeviceMatrox_InoutFormat_TTL:
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
				break;
			case EDeviceMatrox_InoutFormat_422:
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
				break;
			case EDeviceMatrox_InoutFormat_OPTO:
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
				break;
			case EDeviceMatrox_InoutFormat_LVDS:
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);
				}
				break;
			}

			switch(eVsyncOutputPolarity)
			{
			case EDeviceMatrox_InoutPolarity_PosEdgeTrig:
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
				break;
			case EDeviceMatrox_InoutPolarity_NegEdgeTrig:
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);
				}
				break;
			}
		}
		else
		{
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
		}

		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], bCsyncInputActive);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], bCsyncOutputActive);
		m_vctMappingData.push_back(strMapping);

		if(bCsyncInputActive)
		{
			switch(eCsyncInputFormat)
			{
			case EDeviceMatrox_InoutFormat_TTL:
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
				break;
			case EDeviceMatrox_InoutFormat_422:
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
				break;
			case EDeviceMatrox_InoutFormat_OPTO:
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
				break;
			case EDeviceMatrox_InoutFormat_LVDS:
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);
				}
				break;
			}

			switch(eCsyncInputPolarity)
			{
			case EDeviceMatrox_InoutPolarity_PosEdgeTrig:
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
				break;
			case EDeviceMatrox_InoutPolarity_NegEdgeTrig:
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);
				}
				break;
			}
		}
		else
		{
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
		}

		if(bCsyncOutputActive)
		{
			switch(eCsyncOutputFormat)
			{
			case EDeviceMatrox_InoutFormat_TTL:
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
				break;
			case EDeviceMatrox_InoutFormat_422:
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
				break;
			case EDeviceMatrox_InoutFormat_OPTO:
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
				break;
			case EDeviceMatrox_InoutFormat_LVDS:
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);
				}
				break;
			}
			
			switch(eCsyncOutputPolarity)
			{
			case EDeviceMatrox_InoutPolarity_PosEdgeTrig:
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
				break;
			case EDeviceMatrox_InoutPolarity_NegEdgeTrig:
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);
				}
				break;
			}
		}
		else
		{
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
		}
		
		nSeqIdx = nParamIdx + 4;

		for(; nParamIdx < nSeqIdx;)
			m_vctMappingData.push_back(g_lpszGeneral_Parameters[nParamIdx++]);

		//

		switch(eTimer1Mode)
		{
		case EDeviceMatrox_GenerationMode_Disable:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);

				int nPixelClock = (int)(dblPixelClock * 1000000.);

				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nPixelClock);
				m_vctMappingData.push_back(strMapping);

				nSeqIdx = nParamIdx + 18;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}

				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nTimer1Delay2);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nTimer1Pulse2);
				m_vctMappingData.push_back(strMapping);

				nSeqIdx = nParamIdx + 7;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
			}
			break;
		case EDeviceMatrox_GenerationMode_Periodic:
			{
				switch(eTimer1ExpType)
				{
				case EDeviceMatrox_ExposureClockType_SynchronousToPixelClock:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);

						int nPixelClock = dblPixelClock * 1000000;

						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nPixelClock);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_ExposureClockType_HSyncClock:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], (int)dblHorizontalFrequencyHz);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_ExposureClockType_VSyncClock:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], (int)dblVerticalFrequencyHz);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_ExposureClockType_Timer_Output:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_ExposureClockType_ClockGenerator:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);

						int nClockFrequency = dblTimer1ClockFreq * 1000000;

						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nClockFrequency);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_ExposureClockType_Aux1_Input_LVDS:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], (int)dblTimer1ClockFreq);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				}

				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], eTimer1ExpDiv);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);

				switch(eTimer1TrgSignal)
				{
				case EDeviceMatrox_ExposureTriggerSignal_PSG_HSync:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_ExposureTriggerSignal_PSG_VSync:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_ExposureTriggerSignal_Software:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_ExposureTriggerSignal_Timer_Output:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_0_Common:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_1_Common:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_2_Common:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_3_Common:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_4_Common:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_5_Common:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_6:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_7:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_8:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_9:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_10:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_11:
				case EDeviceMatrox_ExposureTriggerSignal_RotaryEncoderFowardTrigger:
				case EDeviceMatrox_ExposureTriggerSignal_RotaryEncoderReversedTrigger:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				}

				nSeqIdx = nParamIdx + 8;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}

				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nTimer1Delay1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nTimer1Pulse1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nTimer1Delay2);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nTimer1Pulse2);
				m_vctMappingData.push_back(strMapping);

				switch(eTimer1ExpFormat)
				{
				case EDeviceMatrox_InoutFormatDefault_TTL:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_InoutFormatDefault_422:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_InoutFormatDefault_OPTO:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_InoutFormatDefault_LVDS:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_InoutFormatDefault_Default:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				}

				switch(eTimer1ExpPolarity)
				{
				case EDeviceMatrox_InoutPolarity_PosEdgeTrig:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_InoutPolarity_NegEdgeTrig:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				}
			}
			break;
		case EDeviceMatrox_GenerationMode_OnTriggerEvent:
			{
				switch(eTimer1ExpType)
				{
				case EDeviceMatrox_ExposureClockType_SynchronousToPixelClock:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);

						int nPixelClock = dblPixelClock * 1000000;

						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nPixelClock);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_ExposureClockType_HSyncClock:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], (int)dblHorizontalFrequencyHz);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_ExposureClockType_VSyncClock:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], (int)dblVerticalFrequencyHz);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_ExposureClockType_Timer_Output:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_ExposureClockType_ClockGenerator:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);

						int nClockFrequency = dblTimer1ClockFreq * 1000000;

						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nClockFrequency);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_ExposureClockType_Aux1_Input_LVDS:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], (int)dblTimer1ClockFreq);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				}

				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], eTimer1ExpDiv);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);

				switch(eTimer1TrgSignal)
				{
				case EDeviceMatrox_ExposureTriggerSignal_PSG_HSync:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_ExposureTriggerSignal_PSG_VSync:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_ExposureTriggerSignal_Software:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_ExposureTriggerSignal_Timer_Output:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_0_Common:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_1_Common:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_2_Common:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_3_Common:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_4_Common:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_5_Common:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_6:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_7:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_8:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_9:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_10:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_11:
				case EDeviceMatrox_ExposureTriggerSignal_RotaryEncoderFowardTrigger:
				case EDeviceMatrox_ExposureTriggerSignal_RotaryEncoderReversedTrigger:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				}

				switch(eTimer1TrgFormat)
				{
				case EDeviceMatrox_InoutFormatDefault_TTL:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_InoutFormatDefault_422:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_InoutFormatDefault_OPTO:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_InoutFormatDefault_LVDS:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_InoutFormatDefault_Default:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				}

				switch(eTimer1TrgPolarity)
				{
				case EDeviceMatrox_InoutPolarity_PosEdgeTrig:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_InoutPolarity_NegEdgeTrig:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				}

				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);

				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nTimer1Delay1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nTimer1Pulse1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nTimer1Delay2);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nTimer1Pulse2);
				m_vctMappingData.push_back(strMapping);

				switch(eTimer1ExpFormat)
				{
				case EDeviceMatrox_InoutFormatDefault_TTL:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_InoutFormatDefault_422:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_InoutFormatDefault_OPTO:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_InoutFormatDefault_LVDS:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_InoutFormatDefault_Default:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				}

				switch(eTimer1ExpPolarity)
				{
				case EDeviceMatrox_InoutPolarity_PosEdgeTrig:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_InoutPolarity_NegEdgeTrig:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				}
			}
			break;
		}

		switch(eTimer1Mode)
		{
		case EDeviceMatrox_GenerationMode_Disable:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_GenerationMode_Periodic:
		case EDeviceMatrox_GenerationMode_OnTriggerEvent:
			{
				switch(eTimer1AdvancedArmMode)
				{
				case EDeviceMatrox_TriggerArmMode_Enable:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);

						switch(eTimer1AdvancedArmFormat)
						{
						case EDeviceMatrox_InoutFormatDefault_TTL:
							{
								strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
								m_vctMappingData.push_back(strMapping);
								strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
								m_vctMappingData.push_back(strMapping);
								strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
								m_vctMappingData.push_back(strMapping);
								strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
								m_vctMappingData.push_back(strMapping);
								strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
								m_vctMappingData.push_back(strMapping);
							}
							break;
						case EDeviceMatrox_InoutFormatDefault_422:
							{
								strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
								m_vctMappingData.push_back(strMapping);
								strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
								m_vctMappingData.push_back(strMapping);
								strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
								m_vctMappingData.push_back(strMapping);
								strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
								m_vctMappingData.push_back(strMapping);
								strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
								m_vctMappingData.push_back(strMapping);
							}
							break;
						case EDeviceMatrox_InoutFormatDefault_OPTO:
							{
								strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
								m_vctMappingData.push_back(strMapping);
								strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
								m_vctMappingData.push_back(strMapping);
								strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
								m_vctMappingData.push_back(strMapping);
								strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
								m_vctMappingData.push_back(strMapping);
								strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
								m_vctMappingData.push_back(strMapping);
							}
							break;
						case EDeviceMatrox_InoutFormatDefault_LVDS:
							{
								strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
								m_vctMappingData.push_back(strMapping);
								strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
								m_vctMappingData.push_back(strMapping);
								strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
								m_vctMappingData.push_back(strMapping);
								strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
								m_vctMappingData.push_back(strMapping);
								strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
								m_vctMappingData.push_back(strMapping);
							}
							break;
						case EDeviceMatrox_InoutFormatDefault_Default:
							{
								strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
								m_vctMappingData.push_back(strMapping);
								strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
								m_vctMappingData.push_back(strMapping);
								strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
								m_vctMappingData.push_back(strMapping);
								strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
								m_vctMappingData.push_back(strMapping);
								strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
								m_vctMappingData.push_back(strMapping);
							}
							break;
						}

						switch(eTimer1AdvancedArmPolarity)
						{
						case EDeviceMatrox_InoutPolarity_PosEdgeTrig:
							{
								strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
								m_vctMappingData.push_back(strMapping);
								strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
								m_vctMappingData.push_back(strMapping);
							}
							break;
						case EDeviceMatrox_InoutPolarity_NegEdgeTrig:
							{
								strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
								m_vctMappingData.push_back(strMapping);
								strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
								m_vctMappingData.push_back(strMapping);
							}
							break;
						}
					}
					break;
				case EDeviceMatrox_TriggerArmMode_Disable:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				}
			}
			break;
		}

		switch(eGrabMode)
		{
		case EDeviceMatrox_GrabMode_Continuous:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);

				nSeqIdx = nParamIdx + 27;

				for(;nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
			}
			break;
		case EDeviceMatrox_GrabMode_SoftwareTrigger:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);

				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);

				switch(eGrabModeActivation)
				{
				case EDeviceMatrox_GrabModeActivation_NextValidFrameOrField:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_GrabModeActivation_AsynchronousReset:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_GrabModeActivation_Asynchronous_ResetDelayed_1_Frame:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				}

				nSeqIdx = nParamIdx + 21;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
			}
			break;
		case EDeviceMatrox_GrabMode_HardwareTrigger:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);

				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);

				switch(eGrabModeActivation)
				{
				case EDeviceMatrox_GrabModeActivation_NextValidFrameOrField:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_GrabModeActivation_AsynchronousReset:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_GrabModeActivation_Asynchronous_ResetDelayed_1_Frame:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				}

				switch(eTrgFormat)
				{
				case EDeviceMatrox_InoutFormatDefault_TTL:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_InoutFormatDefault_422:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_InoutFormatDefault_OPTO:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_InoutFormatDefault_LVDS:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_InoutFormatDefault_Default:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				}

				switch(eTrgPolarity)
				{
				case EDeviceMatrox_InoutPolarity_PosEdgeTrig:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_InoutPolarity_NegEdgeTrig:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				}

				nSeqIdx = nParamIdx + 7;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}

				nSeqIdx = nParamIdx + 5;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}

				switch(eTrgArmPolarity)
				{
				case EDeviceMatrox_InoutPolarity_PosEdgeTrig:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_InoutPolarity_NegEdgeTrig:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				}
			}
			break;
		}

		nSeqIdx = nParamIdx + 17;

		for(; nParamIdx < nSeqIdx;)
			m_vctMappingData.push_back(g_lpszGeneral_Parameters[nParamIdx++]);

		switch(eTimer2Mode)
		{
		case EDeviceMatrox_GenerationMode_Disable:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);

				int nPixelClock = dblPixelClock * 1000000.;

				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nPixelClock);
				m_vctMappingData.push_back(strMapping);

				nSeqIdx = nParamIdx + 18;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}

				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nTimer2Delay2);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nTimer2Pulse2);
				m_vctMappingData.push_back(strMapping);

				nSeqIdx = nParamIdx + 16;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
			}
			break;
		case EDeviceMatrox_GenerationMode_Periodic:
			{
				switch(eTimer2ExpType)
				{
				case EDeviceMatrox_ExposureClockType_SynchronousToPixelClock:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);

						int nPixelClock = dblPixelClock * 1000000;

						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nPixelClock);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_ExposureClockType_HSyncClock:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], (int)dblHorizontalFrequencyHz);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_ExposureClockType_VSyncClock:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], (int)dblVerticalFrequencyHz);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_ExposureClockType_Timer_Output:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_ExposureClockType_ClockGenerator:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);

						int nClockFrequency = dblTimer2ClockFreq * 1000000;

						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nClockFrequency);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_ExposureClockType_Aux1_Input_LVDS:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], (int)dblTimer2ClockFreq);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				}

				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], eTimer2ExpDiv);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);

				switch(eTimer2TrgSignal)
				{
				case EDeviceMatrox_ExposureTriggerSignal_PSG_HSync:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_ExposureTriggerSignal_PSG_VSync:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_ExposureTriggerSignal_Software:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_ExposureTriggerSignal_Timer_Output:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_0_Common:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_1_Common:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_2_Common:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_3_Common:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_4_Common:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_5_Common:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_6:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_7:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_8:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_9:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_10:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_11:
				case EDeviceMatrox_ExposureTriggerSignal_RotaryEncoderFowardTrigger:
				case EDeviceMatrox_ExposureTriggerSignal_RotaryEncoderReversedTrigger:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				}

				nSeqIdx = nParamIdx + 8;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}

				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nTimer2Delay1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nTimer2Pulse1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nTimer2Delay2);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nTimer2Pulse2);
				m_vctMappingData.push_back(strMapping);

				switch(eTimer2ExpFormat)
				{
				case EDeviceMatrox_InoutFormatDefault_TTL:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_InoutFormatDefault_422:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_InoutFormatDefault_OPTO:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_InoutFormatDefault_LVDS:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_InoutFormatDefault_Default:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				}
				
				switch(eTimer2ExpPolarity)
				{
				case EDeviceMatrox_InoutPolarity_PosEdgeTrig:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_InoutPolarity_NegEdgeTrig:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				}
			}
			break;
		case EDeviceMatrox_GenerationMode_OnTriggerEvent:
			{
				switch(eTimer2ExpType)
				{
				case EDeviceMatrox_ExposureClockType_SynchronousToPixelClock:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);

						int nPixelClock = dblPixelClock * 1000000;

						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nPixelClock);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_ExposureClockType_HSyncClock:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], (int)dblHorizontalFrequencyHz);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_ExposureClockType_VSyncClock:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], (int)dblVerticalFrequencyHz);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_ExposureClockType_Timer_Output:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_ExposureClockType_ClockGenerator:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);

						int nClockFrequency = dblTimer2ClockFreq * 1000000;

						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nClockFrequency);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_ExposureClockType_Aux1_Input_LVDS:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], (int)dblTimer2ClockFreq);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				}

				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], eTimer2ExpDiv);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);

				switch(eTimer2TrgSignal)
				{
				case EDeviceMatrox_ExposureTriggerSignal_PSG_HSync:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_ExposureTriggerSignal_PSG_VSync:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_ExposureTriggerSignal_Software:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_ExposureTriggerSignal_Timer_Output:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_0_Common:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_1_Common:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_2_Common:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_3_Common:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_4_Common:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_5_Common:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_6:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_7:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_8:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_9:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_10:
				case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_11:
				case EDeviceMatrox_ExposureTriggerSignal_RotaryEncoderFowardTrigger:
				case EDeviceMatrox_ExposureTriggerSignal_RotaryEncoderReversedTrigger:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				}

				switch(eTimer2TrgFormat)
				{
				case EDeviceMatrox_InoutFormatDefault_TTL:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_InoutFormatDefault_422:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_InoutFormatDefault_OPTO:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_InoutFormatDefault_LVDS:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_InoutFormatDefault_Default:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				}

				switch(eTimer2TrgPolarity)
				{
				case EDeviceMatrox_InoutPolarity_PosEdgeTrig:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_InoutPolarity_NegEdgeTrig:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				}
				
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nTimer2Delay1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nTimer2Pulse1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nTimer2Delay2);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nTimer2Pulse2);
				m_vctMappingData.push_back(strMapping);

				switch(eTimer2ExpFormat)
				{
				case EDeviceMatrox_InoutFormatDefault_TTL:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_InoutFormatDefault_422:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_InoutFormatDefault_OPTO:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_InoutFormatDefault_LVDS:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_InoutFormatDefault_Default:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				}

				switch(eTimer2ExpPolarity)
				{
				case EDeviceMatrox_InoutPolarity_PosEdgeTrig:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_InoutPolarity_NegEdgeTrig:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				}
			}
			break;
		}
		
		m_vctMappingData.push_back(g_lpszGeneral_Parameters[nParamIdx++]);
		m_vctMappingData.push_back(g_lpszGeneral_Parameters[nParamIdx++]);
		m_vctMappingData.push_back(g_lpszGeneral_Parameters[nParamIdx++]);

		int nTapsFormat = eTaps1Dir + (eTaps2Dir * 10) + (eTaps3Dir * 100) + (eTaps4Dir * 1000) + (eTaps5Dir * 10000) + (eTaps6Dir * 100000) + (eTaps7Dir * 1000000) + (eTaps8Dir * 10000000);

		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nTapsFormat);
		m_vctMappingData.push_back(strMapping);

		switch(eRegionsX)
		{
		case EDeviceMatrox_TapsSelector_1:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_TapsSelector_2:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 2);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_TapsSelector_4:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 4);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_TapsSelector_8:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 8);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_TapsSelector_10:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 10);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		}

		switch(eRegionsY)
		{
		case EDeviceMatrox_TapsSelector_1:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_TapsSelector_2:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 2);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_TapsSelector_4:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 4);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_TapsSelector_8:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 8);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_TapsSelector_10:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 10);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		}

		switch(eAdjacentX)
		{
		case EDeviceMatrox_TapsSelector_1:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_TapsSelector_2:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 2);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_TapsSelector_4:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 4);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_TapsSelector_8:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 8);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_TapsSelector_10:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 10);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		}

		switch(eAdjacentY)
		{
		case EDeviceMatrox_TapsSelector_1:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_TapsSelector_2:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 2);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_TapsSelector_4:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 4);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_TapsSelector_8:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 8);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_TapsSelector_10:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 10);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		}

		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], eConfigurationType);
		m_vctMappingData.push_back(strMapping);

		switch(eDataBusWidth)
		{
		case EDeviceMatrox_DataBusWidth_10bits:
		case EDeviceMatrox_DataBusWidth_12bits:
		case EDeviceMatrox_DataBusWidth_14bits:
		case EDeviceMatrox_DataBusWidth_16bits:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_DataBusWidth_8bits:
		case EDeviceMatrox_DataBusWidth_24bits:
		case EDeviceMatrox_DataBusWidth_32bits:
		case EDeviceMatrox_DataBusWidth_64bits:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		}

		m_vctMappingData.push_back(g_lpszGeneral_Parameters[nParamIdx++]);
		m_vctMappingData.push_back(g_lpszGeneral_Parameters[nParamIdx++]);

		switch(eConfigurationType)
		{
		case EDeviceMatrox_ConfigurationType_Base:
			{
				switch(eCameraTapsMode)
				{
				case EDeviceMatrox_CameraTapsMode_1Tap8_16Bits:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_CameraTapsMode_2Taps8Bits:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 3);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 2);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_CameraTapsMode_2Taps10_12Bits:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 4);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 3);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_CameraTapsMode_2Taps14_16BitsTimeMultiplexed:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 5);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 4);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_CameraTapsMode_4Taps8BitsTimeMultiplexed:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 9);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 7);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_CameraTapsMode_4Taps10_12BitsTimeMultiplexed:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 10);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 8);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_CameraTapsMode_3Taps8Bits_RGB:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 6);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 5);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_CameraTapsMode_6Taps8BitsTimeMultiplexed_2RGB:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 7);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 10);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				}
			}
			break;
		case EDeviceMatrox_ConfigurationType_Medium:
			{
				switch(eCameraTapsMode)
				{
				case EDeviceMatrox_CameraTapsMode_2Taps14_16Bits_ABCD:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 11);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 9);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_CameraTapsMode_2Taps14_16Bits_ABDE:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 19);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 18);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_CameraTapsMode_4Taps8Bits:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 14);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 12);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_CameraTapsMode_4Taps10_12Bits:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 15);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 13);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_CameraTapsMode_8Taps8BitsTimeMultiplexed:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 17);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 15);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_CameraTapsMode_3Taps10_12Bits_RGB:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 12);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 10);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_CameraTapsMode_3Taps14_16Bits_RGB:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 16);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 14);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_CameraTapsMode_6Taps8_2RGB_ABC_DEF_NonADJ:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 13);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 11);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_CameraTapsMode_6Taps8_2RGB_ABCDEFGH_ADJ:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 20);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 20);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				}
			}
			break;
		case EDeviceMatrox_ConfigurationType_Full:
			{
				switch(eCameraTapsMode)
				{
				case EDeviceMatrox_CameraTapsMode_4Taps14_16Bits:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 18);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 17);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_CameraTapsMode_8Taps8Bits:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 18);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 16);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				case EDeviceMatrox_CameraTapsMode_10Taps8Bits:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 18);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 19);
						m_vctMappingData.push_back(strMapping);
					}
					break;
				}
			}
			break;
		}

		m_vctMappingData.push_back(g_lpszGeneral_Parameters[nParamIdx++]);
		m_vctMappingData.push_back(g_lpszGeneral_Parameters[nParamIdx++]);
		m_vctMappingData.push_back(g_lpszGeneral_Parameters[nParamIdx++]);
		m_vctMappingData.push_back(g_lpszGeneral_Parameters[nParamIdx++]);
		m_vctMappingData.push_back(g_lpszGeneral_Parameters[nParamIdx++]);

		if(bEnableConnector1 || bEnableConnector2)
		{
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], eCameraControl1 + 1);
			m_vctMappingData.push_back(strMapping);
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], eCameraControl2 + 1);
			m_vctMappingData.push_back(strMapping);
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], eCameraControl3 + 1);
			m_vctMappingData.push_back(strMapping);
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], eCameraControl4 + 1);
			m_vctMappingData.push_back(strMapping);
		}
		else
		{
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
		}

		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], bEnableConnector1);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], bEnableConnector2);
		m_vctMappingData.push_back(strMapping);


		nSeqIdx = nParamIdx + 4;

		for(; nParamIdx < nSeqIdx;)
		{
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
		}

		nSeqIdx = nParamIdx + 3;

		for(; nParamIdx < nSeqIdx;)
		{
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
		}

		nSeqIdx = nParamIdx + 7;

		for(; nParamIdx < nSeqIdx;)
		{
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
		}

		nSeqIdx = nParamIdx + 3;

		for(; nParamIdx < nSeqIdx;)
		{
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
		}

		switch(eDataBusWidth)
		{
		case EDeviceMatrox_DataBusWidth_10bits:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_DataBusWidth_12bits:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_DataBusWidth_14bits:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_DataBusWidth_16bits:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		default:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		}

		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);

		nSeqIdx = nParamIdx + 109;

		for(; nParamIdx < nSeqIdx;)
		{
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
		}

		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
		m_vctMappingData.push_back(strMapping);

		nSeqIdx = nParamIdx + 6;

		for(; nParamIdx < nSeqIdx;)
		{
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
		}

		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
		m_vctMappingData.push_back(strMapping);

		nSeqIdx = nParamIdx + 6;

		for(; nParamIdx < nSeqIdx;)
		{
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
		}
		
		if(eTimer1Mode != EDeviceMatrox_GenerationMode_Disable)
		{
			if(eGrabMode == EDeviceMatrox_GrabMode_HardwareTrigger)
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);

				switch(eTrgSignal)
				{
				case EDeviceMatrox_GrabModeSignal_PSH_VSync:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 166;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_GrabModeSignal_Timer_1_Output:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 165;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_GrabModeSignal_Timer_2_Output:
					{
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 164;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_GrabModeSignal_AuxiliaryIO_0_Common:
					{
						nSeqIdx = nParamIdx + 5;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 161;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_GrabModeSignal_AuxiliaryIO_1_Common:
					{
						nSeqIdx = nParamIdx + 6;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 160;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_GrabModeSignal_AuxiliaryIO_2_Common:
					{
						nSeqIdx = nParamIdx + 36;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 130;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_GrabModeSignal_AuxiliaryIO_3_Common:
					{
						nSeqIdx = nParamIdx + 37;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 129;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_GrabModeSignal_AuxiliaryIO_4_Common:
					{
						nSeqIdx = nParamIdx + 62;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 104;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_GrabModeSignal_AuxiliaryIO_5_Common:
					{
						nSeqIdx = nParamIdx + 63;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 103;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_GrabModeSignal_AuxiliaryIO_6:
					{
						nSeqIdx = nParamIdx + 10;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 156;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_GrabModeSignal_AuxiliaryIO_7:
					{
						nSeqIdx = nParamIdx + 11;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 155;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_GrabModeSignal_AuxiliaryIO_8:
					{
						nSeqIdx = nParamIdx + 28;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 138;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_GrabModeSignal_AuxiliaryIO_9:
					{
						nSeqIdx = nParamIdx + 29;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 137;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_GrabModeSignal_AuxiliaryIO_10:
					{
						nSeqIdx = nParamIdx + 54;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 112;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_GrabModeSignal_AuxiliaryIO_11:
					{
						nSeqIdx = nParamIdx + 55;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 111;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_GrabModeSignal_RotaryEncoderFowardTrigger:
					{
						nSeqIdx = nParamIdx + 132;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 34;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_GrabModeSignal_RotaryEncoderReversedTrigger:
					{
						nSeqIdx = nParamIdx + 133;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 33;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				}
			}
			else
			{
				nSeqIdx = nParamIdx + 168;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
			}

			switch(eTrgArmSignal)
			{
			case EDeviceMatrox_TriggerArmSignal_FrameRetrigger:
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
				break;
			case EDeviceMatrox_TriggerArmSignal_HardwareIgnoreFrameRetrigger:
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
				break;
			case EDeviceMatrox_TriggerArmSignal_SoftwareIgnoreFrameRetrigger:
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
				break;
			case EDeviceMatrox_TriggerArmSignal_FrameRetriggerLatched:
				{
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);
				}
				break;
			}
		}
		else
		{
			nSeqIdx = nParamIdx + 171;

			for(; nParamIdx < nSeqIdx;)
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		
		//g_lpszGeneral_Parameters
		//EMatoxDCF_EOFExp_Trigger

		nParamIdx = 0;

		strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);

		strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);

		switch(eTimer1TrgSignal)
		{
		case EDeviceMatrox_ExposureTriggerSignal_PSG_HSync:
		case EDeviceMatrox_ExposureTriggerSignal_PSG_VSync:
		case EDeviceMatrox_ExposureTriggerSignal_Software:
			{
				nSeqIdx = nParamIdx + 168;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
			}
			break;
		case EDeviceMatrox_ExposureTriggerSignal_Timer_Output:
			{
				strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);

				nSeqIdx = nParamIdx + 167;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
			}
			break;
		case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_0_Common:
			{
				nSeqIdx = nParamIdx + 8;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}

				strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);

				nSeqIdx = nParamIdx + 159;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
			}
			break;
		case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_1_Common:
			{
				nSeqIdx = nParamIdx + 9;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}

				strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);

				nSeqIdx = nParamIdx + 158;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
			}
			break;
		case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_2_Common:
			{
				nSeqIdx = nParamIdx + 50;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}

				strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);

				nSeqIdx = nParamIdx + 117;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
			}
			break;
		case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_3_Common:
			{
				nSeqIdx = nParamIdx + 51;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}

				strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);

				nSeqIdx = nParamIdx + 116;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
			}
			break;
		case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_4_Common:
			{
				nSeqIdx = nParamIdx + 66;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}

				strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);

				nSeqIdx = nParamIdx + 101;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
			}
			break;
		case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_5_Common:
			{
				nSeqIdx = nParamIdx + 67;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}

				strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);

				nSeqIdx = nParamIdx + 100;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
			}
			break;
		case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_6:
			{
				nSeqIdx = nParamIdx + 14;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}

				strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);

				nSeqIdx = nParamIdx + 153;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
			}
			break;
		case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_7:
			{
				nSeqIdx = nParamIdx + 15;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}

				strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);

				nSeqIdx = nParamIdx + 152;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
			}
			break;
		case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_8:
			{
				nSeqIdx = nParamIdx + 32;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}

				strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);

				nSeqIdx = nParamIdx + 135;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
			}
			break;
		case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_9:
			{
				nSeqIdx = nParamIdx + 33;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}

				strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);

				nSeqIdx = nParamIdx + 134;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
			}
			break;
		case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_10:
			{
				nSeqIdx = nParamIdx + 58;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}

				strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);

				nSeqIdx = nParamIdx + 109;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
			}
			break;
		case EDeviceMatrox_ExposureTriggerSignal_AuxiliaryIO_11:
			{
				nSeqIdx = nParamIdx + 59;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}

				strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);

				nSeqIdx = nParamIdx + 108;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
			}
			break;
		case EDeviceMatrox_ExposureTriggerSignal_RotaryEncoderFowardTrigger:
			{
				nSeqIdx = nParamIdx + 136;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}

				strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);

				nSeqIdx = nParamIdx + 31;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
			}
			break;
		case EDeviceMatrox_ExposureTriggerSignal_RotaryEncoderReversedTrigger:
			{
				nSeqIdx = nParamIdx + 137;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}

				strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);

				nSeqIdx = nParamIdx + 30;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
			}
			break;
		}

		switch(eTimer1ExpType)
		{
		case EDeviceMatrox_ExposureClockType_SynchronousToPixelClock:
			{
				nSeqIdx = nParamIdx + 24;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
			}
			break;
		case EDeviceMatrox_ExposureClockType_ClockGenerator:
			{
				strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);

				nSeqIdx = nParamIdx + 23;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
			}
			break;
		case EDeviceMatrox_ExposureClockType_HSyncClock:
			{
				nSeqIdx = nParamIdx + 1;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}

				strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);

				nSeqIdx = nParamIdx + 22;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
			}
			break;
		case EDeviceMatrox_ExposureClockType_VSyncClock:
			{
				nSeqIdx = nParamIdx + 2;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}

				strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);

				nSeqIdx = nParamIdx + 21;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
			}
			break;
		case EDeviceMatrox_ExposureClockType_Timer_Output:
			{
				nSeqIdx = nParamIdx + 3;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}

				strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);

				nSeqIdx = nParamIdx + 20;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
			}
			break;
		case EDeviceMatrox_ExposureClockType_Aux1_Input_LVDS:
			{
				nSeqIdx = nParamIdx + 10;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}

				strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);

				nSeqIdx = nParamIdx + 13;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
			}
			break;
		}

		switch(eTimer1AdvancedArmMode)
		{
		case EDeviceMatrox_TriggerArmMode_Enable:
			{
				EDeviceMatrox_TriggerArmSource eTimer1TriggerSource = EDeviceMatrox_TriggerArmSource_Count;

				if(GetTimer1AdvancedTriggerArmSource(&eTimer1TriggerSource))
				{
					bError = true;
					break;
				}

				switch(eTimer1TriggerSource)
				{
				case EDeviceMatrox_TriggerArmSource_Timer1_0:
					{
						strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 171;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_TriggerArmSource_Software_Arm:
					{
						strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);

						strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 170;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_TriggerArmSource_Timer_Output:
					{
						nSeqIdx = nParamIdx + 2;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 169;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_TriggerArmSource_PSG_HSync:
					{
						nSeqIdx = nParamIdx + 9;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 162;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_TriggerArmSource_PSG_VSync:
					{
						nSeqIdx = nParamIdx + 10;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 161;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_TriggerArmSource_AuxiliaryIO_0_Common:
					{
						nSeqIdx = nParamIdx + 11;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 160;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_TriggerArmSource_AuxiliaryIO_1_Common:
					{
						nSeqIdx = nParamIdx + 14;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 157;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_TriggerArmSource_AuxiliaryIO_2_Common:
					{
						nSeqIdx = nParamIdx + 44;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 127;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_TriggerArmSource_AuxiliaryIO_3_Common:
					{
						nSeqIdx = nParamIdx + 45;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 126;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_TriggerArmSource_AuxiliaryIO_4_Common:
					{
						nSeqIdx = nParamIdx + 70;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 101;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_TriggerArmSource_AuxiliaryIO_5_Common:
					{
						nSeqIdx = nParamIdx + 71;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 100;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_TriggerArmSource_AuxiliaryIO_6:
					{
						nSeqIdx = nParamIdx + 18;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 153;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_TriggerArmSource_AuxiliaryIO_7:
					{
						nSeqIdx = nParamIdx + 19;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 152;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_TriggerArmSource_AuxiliaryIO_8:
					{
						nSeqIdx = nParamIdx + 36;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 135;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_TriggerArmSource_AuxiliaryIO_9:
					{
						nSeqIdx = nParamIdx + 37;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 134;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_TriggerArmSource_AuxiliaryIO_10:
					{
						nSeqIdx = nParamIdx + 62;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 109;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_TriggerArmSource_AuxiliaryIO_11:
					{
						nSeqIdx = nParamIdx + 63;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 108;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_TriggerArmSource_RotaryEncoderFowardTrigger:
					{
						nSeqIdx = nParamIdx + 140;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 31;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_TriggerArmSource_RotaryEncoderReversedTrigger:
					{
						nSeqIdx = nParamIdx + 141;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 30;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				}
			}
			break;
		case EDeviceMatrox_TriggerArmMode_Disable:
			{
				nSeqIdx = nParamIdx + 172;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
			}
			break;
		}


		nSeqIdx = nParamIdx + 240;

		for(; nParamIdx < nSeqIdx;)
		{
			strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
		}

		switch(eTimer2ExpType)
		{
		case EDeviceMatrox_ExposureClockType_SynchronousToPixelClock:
			{
				nSeqIdx = nParamIdx + 24;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
			}
			break;
		case EDeviceMatrox_ExposureClockType_ClockGenerator:
			{
				strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);

				nSeqIdx = nParamIdx + 23;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
			}
			break;
		case EDeviceMatrox_ExposureClockType_HSyncClock:
			{
				nSeqIdx = nParamIdx + 1;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}

				strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);

				nSeqIdx = nParamIdx + 22;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
			}
			break;
		case EDeviceMatrox_ExposureClockType_VSyncClock:
			{
				nSeqIdx = nParamIdx + 2;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}

				strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);

				nSeqIdx = nParamIdx + 21;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
			}
			break;
		case EDeviceMatrox_ExposureClockType_Timer_Output:
			{
				nSeqIdx = nParamIdx + 3;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}

				strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);

				nSeqIdx = nParamIdx + 20;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
			}
			break;
		case EDeviceMatrox_ExposureClockType_Aux1_Input_LVDS:
			{
				nSeqIdx = nParamIdx + 10;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}

				strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);

				nSeqIdx = nParamIdx + 13;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
			}
			break;
		}

		switch(eTimer2AdvancedArmMode)
		{
		case EDeviceMatrox_TriggerArmMode_Enable:
			{
				switch(eTimer2AdvancedArmSource)
				{
				case EDeviceMatrox_TriggerArmSource_Timer1_0:
					{
						strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 171;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_TriggerArmSource_Software_Arm:
					{
						strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);

						strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 170;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_TriggerArmSource_Timer_Output:
					{
						nSeqIdx = nParamIdx + 2;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 169;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_TriggerArmSource_PSG_HSync:
					{
						nSeqIdx = nParamIdx + 9;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 162;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_TriggerArmSource_PSG_VSync:
					{
						nSeqIdx = nParamIdx + 10;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 161;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_TriggerArmSource_AuxiliaryIO_0_Common:
					{
						nSeqIdx = nParamIdx + 11;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 160;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_TriggerArmSource_AuxiliaryIO_1_Common:
					{
						nSeqIdx = nParamIdx + 12;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 159;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_TriggerArmSource_AuxiliaryIO_2_Common:
					{
						nSeqIdx = nParamIdx + 44;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 127;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_TriggerArmSource_AuxiliaryIO_3_Common:
					{
						nSeqIdx = nParamIdx + 45;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 126;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_TriggerArmSource_AuxiliaryIO_4_Common:
					{
						nSeqIdx = nParamIdx + 70;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 101;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_TriggerArmSource_AuxiliaryIO_5_Common:
					{
						nSeqIdx = nParamIdx + 71;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 100;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_TriggerArmSource_AuxiliaryIO_6:
					{
						nSeqIdx = nParamIdx + 18;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 153;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_TriggerArmSource_AuxiliaryIO_7:
					{
						nSeqIdx = nParamIdx + 19;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 152;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_TriggerArmSource_AuxiliaryIO_8:
					{
						nSeqIdx = nParamIdx + 36;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 135;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_TriggerArmSource_AuxiliaryIO_9:
					{
						nSeqIdx = nParamIdx + 37;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 134;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_TriggerArmSource_AuxiliaryIO_10:
					{
						nSeqIdx = nParamIdx + 62;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 109;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_TriggerArmSource_AuxiliaryIO_11:
					{
						nSeqIdx = nParamIdx + 63;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 108;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_TriggerArmSource_RotaryEncoderFowardTrigger:
					{
						nSeqIdx = nParamIdx + 140;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 31;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				case EDeviceMatrox_TriggerArmSource_RotaryEncoderReversedTrigger:
					{
						nSeqIdx = nParamIdx + 141;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}

						strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
						m_vctMappingData.push_back(strMapping);

						nSeqIdx = nParamIdx + 30;

						for(; nParamIdx < nSeqIdx;)
						{
							strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
							m_vctMappingData.push_back(strMapping);
						}
					}
					break;
				default:
					bError = true;
					break;
				}

				if(bError)
					break;
			}
			break;
		case EDeviceMatrox_TriggerArmMode_Disable:
			{
				nSeqIdx = nParamIdx + 172;

				for(; nParamIdx < nSeqIdx;)
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
			}
			break;
		}

		if(eTimer2AdvancedArmMode == EDeviceMatrox_TriggerArmMode_Enable)
		{
			switch(eTimer2AdvancedSignal)
			{
			case EDeviceMatrox_TimerOutputSignal_Timer1:
				{
					nSeqIdx = nParamIdx + 3;

					for(; nParamIdx < nSeqIdx;)
					{
						strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}

					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);

					nSeqIdx = nParamIdx + 48;

					for(; nParamIdx < nSeqIdx;)
					{
						strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
				}
				break;
			case EDeviceMatrox_TimerOutputSignal_Timer1_XOR_T_imer2:
				{
					nSeqIdx = nParamIdx + 5;

					for(; nParamIdx < nSeqIdx;)
					{
						strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}

					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);

					nSeqIdx = nParamIdx + 58;

					for(; nParamIdx < nSeqIdx;)
					{
						strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
				}
				break;
			case EDeviceMatrox_TimerOutputSignal_Timer2:
				{
					nSeqIdx = nParamIdx + 6;

					for(; nParamIdx < nSeqIdx;)
					{
						strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}

					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);

					nSeqIdx = nParamIdx + 57;

					for(; nParamIdx < nSeqIdx;)
					{
						strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
						m_vctMappingData.push_back(strMapping);
					}
				}
				break;
			}

			switch(eTimer2AdvancedTrigger)
			{
			case EDeviceMatrox_CaptureATriggerEveryTrigger_1:
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
				break;
			case EDeviceMatrox_CaptureATriggerEveryTrigger_2:
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
				break;
			case EDeviceMatrox_CaptureATriggerEveryTrigger_4:
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
				break;
			case EDeviceMatrox_CaptureATriggerEveryTrigger_8:
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
				}
				break;
			case EDeviceMatrox_CaptureATriggerEveryTrigger_16:
				{
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
					m_vctMappingData.push_back(strMapping);
					strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 1);
					m_vctMappingData.push_back(strMapping);
				}
				break;
			}
		}
		else
		{
			nSeqIdx = nParamIdx + 64;

			for(; nParamIdx < nSeqIdx;)
			{
				strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
			}

			strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
			strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
			strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
			strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
			strMapping.Format(g_lpszExp_Trigger[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
		}

		//Board type define
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);

		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nHorizontalTotal);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nHorizontalActive);
		m_vctMappingData.push_back(strMapping);

		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nVerticalTotal);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nVerticalActive);
		m_vctMappingData.push_back(strMapping);

		int nHorizontalCrop = nHorizontalSync + nHorizontalBProch;
		int nVerticalCrop = 0;

		if(eCameraType == EDeviceMatrox_CameraType_FrameScan)
			nVerticalCrop = nVerticalSync + nVerticalBProch;
		else
			nVerticalCrop = nVerticalActive;

		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nHorizontalCrop);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nVerticalCrop);
		m_vctMappingData.push_back(strMapping);

		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 10000);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);

		switch(eVideoStandard)
		{
		case EDeviceMatrox_Standard_Monochrome:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_Standard_RGBColor:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		}

		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 6);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 17);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 8);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 65535);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 65535);
		m_vctMappingData.push_back(strMapping);

		int nHorizontalDig = nHorizontalTotal - 1;
		int nVerticalDig = nVerticalTotal - 1;

		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nHorizontalDig);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nVerticalDig);
		m_vctMappingData.push_back(strMapping);

		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);

		if(eTimer1Mode != EDeviceMatrox_GenerationMode_Disable)
		{
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
			m_vctMappingData.push_back(strMapping);
		}
		else
		{
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
		}

		if(eTimer2Mode != EDeviceMatrox_GenerationMode_Disable)
		{
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
			m_vctMappingData.push_back(strMapping);
		}
		else
		{
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
		}

		if(eTimer1Mode == EDeviceMatrox_GenerationMode_OnTriggerEvent)
		{
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 4);
			m_vctMappingData.push_back(strMapping);
		}
		else
		{
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
		}

		if(eTimer2Mode == EDeviceMatrox_GenerationMode_OnTriggerEvent)
		{
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 4);
			m_vctMappingData.push_back(strMapping);
		}
		else
		{
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
		}

		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
		m_vctMappingData.push_back(strMapping);

		int nDivValue1 = 1 << eTimer1ExpDiv;

		double dblTimer1Freq = 0;

		switch(eTimer1ExpType)
		{
		case EDeviceMatrox_ExposureClockType_SynchronousToPixelClock:
			{
				dblTimer1Freq = dblPixelClock;
			}
			break;
		case EDeviceMatrox_ExposureClockType_ClockGenerator:
			{
				dblTimer1Freq = (dblTimer1ClockFreq * 1000000);
			}
			break;
		case EDeviceMatrox_ExposureClockType_HSyncClock:
			{
				dblTimer1Freq = dblHorizontalFrequencyHz;
			}
			break;
		case EDeviceMatrox_ExposureClockType_VSyncClock:
			{
				dblTimer1Freq = dblVerticalFrequencyHz;
			}
			break;
		case EDeviceMatrox_ExposureClockType_Timer_Output:
			{
				dblTimer1Freq = 0;
			}
			break;
		case EDeviceMatrox_ExposureClockType_Aux1_Input_LVDS:
			{
				dblTimer1Freq = 1;
			}
			break;
		}

		int nDivValue2 = 1 << eTimer2ExpDiv;

		double dblTimer2Freq = 0;

		switch(eTimer2ExpType)
		{
		case EDeviceMatrox_ExposureClockType_SynchronousToPixelClock:
			{
				dblTimer2Freq = dblPixelClock;
			}
			break;
		case EDeviceMatrox_ExposureClockType_ClockGenerator:
			{
				dblTimer2Freq = (dblTimer1ClockFreq * 1000000.);
			}
			break;
		case EDeviceMatrox_ExposureClockType_HSyncClock:
			{
				dblTimer2Freq = dblHorizontalFrequencyHz;
			}
			break;
		case EDeviceMatrox_ExposureClockType_VSyncClock:
			{
				dblTimer2Freq = dblVerticalFrequencyHz;
			}
			break;
		case EDeviceMatrox_ExposureClockType_Timer_Output:
			{
				dblTimer2Freq = 0;
			}
			break;
		case EDeviceMatrox_ExposureClockType_Aux1_Input_LVDS:
			{
				dblTimer2Freq = 1;
			}
			break;
		}

		int nFreq1 = dblTimer1Freq / nDivValue1;

		int nFreq2 = dblTimer2Freq / nDivValue2;

		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nFreq1);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nFreq2);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nFreq1);
		m_vctMappingData.push_back(strMapping);
		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nFreq2);
		m_vctMappingData.push_back(strMapping);

		if(eTimer1ExpType == EDeviceMatrox_ExposureClockType_Aux1_Input_LVDS)
		{
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
			m_vctMappingData.push_back(strMapping);
		}
		else
		{
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
		}

		if(eTimer2ExpType == EDeviceMatrox_ExposureClockType_Aux1_Input_LVDS)
		{
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
			m_vctMappingData.push_back(strMapping);
		}
		else
		{
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
		}

		switch(eTimer1Mode)
		{
		case EDeviceMatrox_GenerationMode_Disable:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_GenerationMode_Periodic:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_GenerationMode_OnTriggerEvent:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 4);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		}

		switch(eTimer2ExpType)
		{
		case EDeviceMatrox_GenerationMode_Disable:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_GenerationMode_Periodic:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 1);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		case EDeviceMatrox_GenerationMode_OnTriggerEvent:
			{
				strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 4);
				m_vctMappingData.push_back(strMapping);
			}
			break;
		}

		int nTotalPulse2 = nTimer2Pulse1 + nTimer2Delay1 + nTimer2Pulse2 + nTimer2Delay2;
		int nPulseCount2 = 0;

		if(nTimer2Pulse1 || nTimer2Delay1)
			++nPulseCount2;

		if(nTimer2Pulse2 || nTimer2Delay2)
			++nPulseCount2;

		if(nPulseCount2)
			nTotalPulse2 /= nPulseCount2;
		else
			nTotalPulse2 = 0;

		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nTotalPulse2);
		m_vctMappingData.push_back(strMapping);

		int nTotalPulse1 = nTimer1Pulse1 + nTimer1Delay1 + nTimer1Pulse2 + nTimer1Delay2;

		int nPulseCount1 = 0;

		if(nTimer1Pulse1 || nTimer1Delay1)
			++nPulseCount1;

		if(nTimer1Pulse2 || nTimer1Delay2)
			++nPulseCount1;

		if(nPulseCount1)
			nTotalPulse1 /= nPulseCount1;
		else
			nTotalPulse1 = 0;

		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nTotalPulse1);
		m_vctMappingData.push_back(strMapping);

		if(!dblPixelClock)
			break;

		double dblConvertTotal = nHorizontalActive;
		double dblConvertBProch = nHorizontalBProch ? nHorizontalBProch : 1;
		double dblConvertFProch = nHorizontalFProch ? nHorizontalFProch : 1;

		dblConvertTotal /= dblPixelClock;
		dblConvertBProch /= dblPixelClock;
		dblConvertFProch /= dblPixelClock;

		dblConvertTotal += dblConvertBProch;
		dblConvertTotal += dblConvertFProch;

		if(!dblConvertTotal)
			break;

		int nConvertPeriod = dblConvertTotal * 1000.;

		double dblFreq = 1. / dblConvertTotal;

		int nConvertFreq = dblFreq * 1000000.;

		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nConvertFreq);
		m_vctMappingData.push_back(strMapping);

		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nConvertPeriod);
		m_vctMappingData.push_back(strMapping);

		int nConvertPeriodDelay1 = (nTimer1Delay1 - 1) * nConvertPeriod;
		int nConvertPeriodDelay2 = nTimer1Delay2 * nConvertPeriod;

		int nConvertPeriodPulse1 = nTimer1Pulse1 * nConvertPeriod;
		int nConvertPeriodPulse2 = nTimer1Pulse2 * nConvertPeriod;

		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nConvertPeriodDelay1);
		m_vctMappingData.push_back(strMapping);

		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nConvertPeriodDelay2);
		m_vctMappingData.push_back(strMapping);

		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nConvertPeriodPulse1);
		m_vctMappingData.push_back(strMapping);

		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], nConvertPeriodPulse2);
		m_vctMappingData.push_back(strMapping);

		if(!dblTimer1Freq)
			break;

		double dblFreqToTime = 1. / dblTimer1Freq;
		dblFreqToTime *= 1000000000;

		double dblPulse1Active = dblFreqToTime;
		double dblPulse1Inactive = dblFreqToTime;

		double dblPulse2Active = dblFreqToTime;
		double dblPulse2Inactive = dblFreqToTime;

		dblPulse1Active *= nTimer1Pulse1;
		dblPulse2Active *= nTimer1Pulse2;

		dblPulse1Inactive *= (nTimer1Delay1 - 1);
		dblPulse2Inactive *= nTimer1Delay2;

		dblPulse1Active += 2;
		dblPulse2Active += 2;

		dblPulse1Inactive += 2;
		dblPulse2Inactive += 2;

		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], (int)dblPulse1Inactive);
		m_vctMappingData.push_back(strMapping);

		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], (int)dblPulse2Inactive);
		m_vctMappingData.push_back(strMapping);

		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], (int)dblPulse1Active);
		m_vctMappingData.push_back(strMapping);

		strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], (int)dblPulse2Active);
		m_vctMappingData.push_back(strMapping);


		nSeqIdx = EMatoxDCF_EOFGeneral_Parameters;

		for(; nParamIdx < nSeqIdx;)
		{
			strMapping.Format(g_lpszGeneral_Parameters[nParamIdx++], 0);
			m_vctMappingData.push_back(strMapping);
		}

		//////////////////////////////////////////////////////////////////////////

		//[REG_MODIF_STATE]		

		for(int i = 0; i < 301; ++i)
			m_vctMappingData.push_back(g_lpszReg_Modify_state[i]);

		//////////////////////////////////////////////////////////////////////////

	
		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceMatroxSolios::MakeDcfFile(CString strPath)
{
	bool bReturn = false;

	do 
	{
		CString strSave(strPath);

		if(strSave.Right(4).CompareNoCase(_T(".dcf")))
			strSave.AppendFormat(_T("%s"), _T(".dcf"));

		DWORD dwWritten = 0;

		FILE* fp = nullptr;

		fp = _tfopen(strSave, _T("wt"));
		
		if(!fp)
			break;

		for(int i = 0; i < (int)m_vctMappingData.size(); ++i)
		{
			int nLen = WideCharToMultiByte(CP_ACP, 0, m_vctMappingData[i], -1, nullptr, 0, NULL, NULL);

			char *strDcf = (char*)calloc(nLen, sizeof(char));

			WideCharToMultiByte(CP_ACP, 0, m_vctMappingData[i], -1, strDcf, nLen - 1, NULL, NULL);

			fprintf(fp, "%s\n", strDcf);

			free(strDcf);
		}

		fclose(fp);

		bReturn = true;
	} 
	while(false);

	return bReturn;
}

bool CDeviceMatroxSolios::LoadSettings()
{
	bool bReturn = false;

	do 
	{
		__super::LoadSettings();
		
		bReturn = true;
	} 
	while(false);

	return bReturn & CDeviceBase::LoadSettings();
}

bool CDeviceMatroxSolios::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = false;

	if(__super::OnParameterChanged(nParam, strValue))
		return true;

	bool bFoundID = true;

	do
	{
		switch(nParam)
		{
		case -1:
			break;
		default:
			{
				bFoundID = false;
			}
			break;
		}

		if(!bFoundID)
			break;
	}
	while(false);

	if(!bFoundID)
	{
		CString strMessage(_T("Failed to find the parameter"));

		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);
	}

	return bReturn;
}


#endif