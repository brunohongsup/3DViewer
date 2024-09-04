#include "stdafx.h"

#include "DeviceMVTechMSPDio.h"

#ifdef USING_DEVICE

#include "../RavidCore/RavidDatabase.h"
#include "../RavidCore/RavidRecordset.h"
#include "../RavidCore/PerformanceCounter.h"

#include "../RavidFramework/LogManager.h"
#include "../RavidFramework/DeviceManager.h"
#include "../RavidFramework/LibraryManager.h"
#include "../RavidFramework/SequenceManager.h"
#include "../RavidFramework/AuthorityManager.h"
#include "../RavidFramework/FrameworkManager.h"
#include "../RavidFramework/EventHandlerManager.h"
#include "../RavidFramework/MultiLanguageManager.h"

#include "../RavidFramework/MessageBase.h"
#include "../RavidFramework/RavidLedCtrl.h"

#include "../Libraries/Includes/PlxApi/PlxApi.h"

// plxapi650.dll
// plxapi650_x64.dll
#pragma comment(lib, COMMONLIB_PREFIX "PlxApi/PlxApi.lib")

using namespace Ravid;
using namespace Ravid::Device;
using namespace Ravid::Database;
using namespace Ravid::Framework;
using namespace Ravid::Algorithms;
using namespace Ravid::Miscellaneous;

static DWORD g_arrQueueStatusAddress_P16T6Q288_1[MSP_P16_E2T6_Q288::EBoardInfo::EBoardInfo_Channel][MSP_P16_E2T6_Q288::EBoardInfo::EBoardInfo_QueueStatusCount] =
{
	{
		MSP_P16_E2T6_Q288::EQueueStatus::EQueueStatus_CH1_Cam1Queue1,
		MSP_P16_E2T6_Q288::EQueueStatus::EQueueStatus_CH1_Cam1Queue2,
		MSP_P16_E2T6_Q288::EQueueStatus::EQueueStatus_CH1_Cam2Queue1,
		MSP_P16_E2T6_Q288::EQueueStatus::EQueueStatus_CH1_Cam2Queue2,
		MSP_P16_E2T6_Q288::EQueueStatus::EQueueStatus_CH1_Cam3Queue1,
		MSP_P16_E2T6_Q288::EQueueStatus::EQueueStatus_CH1_Cam3Queue2,
	},
	{
		MSP_P16_E2T6_Q288::EQueueStatus::EQueueStatus_CH2_Cam1Queue1,
		MSP_P16_E2T6_Q288::EQueueStatus::EQueueStatus_CH2_Cam1Queue2,
		MSP_P16_E2T6_Q288::EQueueStatus::EQueueStatus_CH2_Cam2Queue1,
		MSP_P16_E2T6_Q288::EQueueStatus::EQueueStatus_CH2_Cam2Queue2,
		MSP_P16_E2T6_Q288::EQueueStatus::EQueueStatus_CH2_Cam3Queue1,
		MSP_P16_E2T6_Q288::EQueueStatus::EQueueStatus_CH2_Cam3Queue2,
	}
};

static DWORD g_arrQueueAddress_P16T6Q288_1[MSP_P16_E2T6_Q288::EBoardInfo::EBoardInfo_Channel][MSP_P16_E2T6_Q288::EBoardInfo::EBoardInfo_QueueCount] =
{
	{
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue1_1,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue1_2,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue1_3,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue1_4,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue1_5,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue1_6,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue1_7,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue1_8,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue1_9,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue1_10,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue1_11,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue1_12,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue1_13,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue1_14,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue1_15,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue1_16,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue1_17,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue1_18,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue1_19,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue1_20,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue1_21,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue1_22,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue1_23,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue1_24,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue2_1,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue2_2,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue2_3,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue2_4,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue2_5,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue2_6,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue2_7,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue2_8,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue2_9,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue2_10,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue2_11,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue2_12,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue2_13,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue2_14,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue2_15,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue2_16,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue2_17,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue2_18,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue2_19,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue2_20,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue2_21,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue2_22,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue2_23,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam1Queue2_24,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue1_1,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue1_2,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue1_3,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue1_4,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue1_5,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue1_6,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue1_7,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue1_8,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue1_9,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue1_10,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue1_11,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue1_12,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue1_13,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue1_14,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue1_15,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue1_16,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue1_17,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue1_18,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue1_19,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue1_20,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue1_21,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue1_22,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue1_23,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue1_24,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue2_1,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue2_2,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue2_3,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue2_4,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue2_5,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue2_6,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue2_7,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue2_8,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue2_9,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue2_10,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue2_11,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue2_12,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue2_13,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue2_14,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue2_15,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue2_16,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue2_17,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue2_18,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue2_19,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue2_20,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue2_21,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue2_22,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue2_23,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam2Queue2_24,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue1_1,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue1_2,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue1_3,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue1_4,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue1_5,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue1_6,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue1_7,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue1_8,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue1_9,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue1_10,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue1_11,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue1_12,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue1_13,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue1_14,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue1_15,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue1_16,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue1_17,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue1_18,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue1_19,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue1_20,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue1_21,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue1_22,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue1_23,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue1_24,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue2_1,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue2_2,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue2_3,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue2_4,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue2_5,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue2_6,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue2_7,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue2_8,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue2_9,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue2_10,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue2_11,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue2_12,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue2_13,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue2_14,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue2_15,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue2_16,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue2_17,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue2_18,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue2_19,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue2_20,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue2_21,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue2_22,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue2_23,
		MSP_P16_E2T6_Q288::EQueueRegister_CH1_Cam3Queue2_24,
	},
	{
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue1_1,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue1_2,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue1_3,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue1_4,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue1_5,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue1_6,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue1_7,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue1_8,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue1_9,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue1_10,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue1_11,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue1_12,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue1_13,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue1_14,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue1_15,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue1_16,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue1_17,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue1_18,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue1_19,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue1_20,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue1_21,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue1_22,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue1_23,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue1_24,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue2_1,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue2_2,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue2_3,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue2_4,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue2_5,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue2_6,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue2_7,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue2_8,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue2_9,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue2_10,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue2_11,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue2_12,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue2_13,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue2_14,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue2_15,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue2_16,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue2_17,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue2_18,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue2_19,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue2_20,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue2_21,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue2_22,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue2_23,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam1Queue2_24,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue1_1,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue1_2,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue1_3,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue1_4,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue1_5,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue1_6,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue1_7,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue1_8,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue1_9,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue1_10,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue1_11,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue1_12,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue1_13,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue1_14,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue1_15,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue1_16,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue1_17,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue1_18,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue1_19,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue1_20,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue1_21,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue1_22,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue1_23,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue1_24,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue2_1,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue2_2,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue2_3,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue2_4,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue2_5,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue2_6,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue2_7,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue2_8,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue2_9,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue2_10,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue2_11,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue2_12,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue2_13,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue2_14,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue2_15,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue2_16,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue2_17,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue2_18,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue2_19,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue2_20,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue2_21,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue2_22,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue2_23,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam2Queue2_24,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue1_1,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue1_2,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue1_3,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue1_4,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue1_5,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue1_6,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue1_7,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue1_8,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue1_9,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue1_10,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue1_11,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue1_12,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue1_13,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue1_14,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue1_15,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue1_16,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue1_17,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue1_18,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue1_19,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue1_20,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue1_21,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue1_22,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue1_23,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue1_24,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue2_1,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue2_2,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue2_3,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue2_4,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue2_5,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue2_6,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue2_7,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue2_8,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue2_9,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue2_10,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue2_11,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue2_12,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue2_13,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue2_14,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue2_15,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue2_16,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue2_17,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue2_18,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue2_19,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue2_20,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue2_21,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue2_22,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue2_23,
		MSP_P16_E2T6_Q288::EQueueRegister_CH2_Cam3Queue2_24,
	}
};

static DWORD g_arrQueueStatusAddress_P16T2Q16_1[MSP_P16_E2T2_Q16_1::EBoardInfo::EBoardInfo_Channel][MSP_P16_E2T2_Q16_1::EBoardInfo::EBoardInfo_QueueStatusCount] =
{
	{
		MSP_P16_E2T2_Q16_1::EQueueStatus::EQueueStatus_CH1_Queue,
	},
	{
		MSP_P16_E2T2_Q16_1::EQueueStatus::EQueueStatus_CH2_Queue,
	}
};

static DWORD g_arrQueueAddress_P16T2Q16_1[MSP_P16_E2T2_Q16_1::EBoardInfo::EBoardInfo_Channel][MSP_P16_E2T2_Q16_1::EBoardInfo::EBoardInfo_QueueCount] =
{
	{
		MSP_P16_E2T2_Q16_1::EQueueRegister::EQueueRegister_CH1_Queue1,
		MSP_P16_E2T2_Q16_1::EQueueRegister::EQueueRegister_CH1_Queue2,
		MSP_P16_E2T2_Q16_1::EQueueRegister::EQueueRegister_CH1_Queue3,
		MSP_P16_E2T2_Q16_1::EQueueRegister::EQueueRegister_CH1_Queue4,
		MSP_P16_E2T2_Q16_1::EQueueRegister::EQueueRegister_CH1_Queue5,
		MSP_P16_E2T2_Q16_1::EQueueRegister::EQueueRegister_CH1_Queue6,
		MSP_P16_E2T2_Q16_1::EQueueRegister::EQueueRegister_CH1_Queue7,
		MSP_P16_E2T2_Q16_1::EQueueRegister::EQueueRegister_CH1_Queue8,
	},
	{
		MSP_P16_E2T2_Q16_1::EQueueRegister::EQueueRegister_CH2_Queue1,
		MSP_P16_E2T2_Q16_1::EQueueRegister::EQueueRegister_CH2_Queue2,
		MSP_P16_E2T2_Q16_1::EQueueRegister::EQueueRegister_CH2_Queue3,
		MSP_P16_E2T2_Q16_1::EQueueRegister::EQueueRegister_CH2_Queue4,
		MSP_P16_E2T2_Q16_1::EQueueRegister::EQueueRegister_CH2_Queue5,
		MSP_P16_E2T2_Q16_1::EQueueRegister::EQueueRegister_CH2_Queue6,
		MSP_P16_E2T2_Q16_1::EQueueRegister::EQueueRegister_CH2_Queue7,
		MSP_P16_E2T2_Q16_1::EQueueRegister::EQueueRegister_CH2_Queue8,
	}
};

static DWORD g_arrQueueOrderAdress_P16T2Q16_1[MSP_P16_E2T2_Q16_1::EBoardInfo::EBoardInfo_Channel][MSP_P16_E2T2_Q16_1::EBoardInfo::EBoardInfo_QueueCount] =
{
	{
		MSP_P16_E2T2_Q16_1::EQueueOrder_CH1_Queue1,
		MSP_P16_E2T2_Q16_1::EQueueOrder_CH1_Queue2,
		MSP_P16_E2T2_Q16_1::EQueueOrder_CH1_Queue3,
		MSP_P16_E2T2_Q16_1::EQueueOrder_CH1_Queue4,
		MSP_P16_E2T2_Q16_1::EQueueOrder_CH1_Queue5,
		MSP_P16_E2T2_Q16_1::EQueueOrder_CH1_Queue6,
		MSP_P16_E2T2_Q16_1::EQueueOrder_CH1_Queue7,
		MSP_P16_E2T2_Q16_1::EQueueOrder_CH1_Queue8,
	},
	{
		MSP_P16_E2T2_Q16_1::EQueueOrder_CH2_Queue1,
		MSP_P16_E2T2_Q16_1::EQueueOrder_CH2_Queue2,
		MSP_P16_E2T2_Q16_1::EQueueOrder_CH2_Queue3,
		MSP_P16_E2T2_Q16_1::EQueueOrder_CH2_Queue4,
		MSP_P16_E2T2_Q16_1::EQueueOrder_CH2_Queue5,
		MSP_P16_E2T2_Q16_1::EQueueOrder_CH2_Queue6,
		MSP_P16_E2T2_Q16_1::EQueueOrder_CH2_Queue7,
		MSP_P16_E2T2_Q16_1::EQueueOrder_CH2_Queue8,
	}
};

static DWORD g_arrQueueStatusAddress_P16T2Q16_2[MSP_P16_E2T2_Q16_2::EBoardInfo::EBoardInfo_Channel][MSP_P16_E2T2_Q16_2::EBoardInfo::EBoardInfo_QueueStatusCount] =
{
	{
		MSP_P16_E2T2_Q16_2::EQueueStatus::EQueueStatus_CH1_Queue,
	},
	{
		MSP_P16_E2T2_Q16_2::EQueueStatus::EQueueStatus_CH2_Queue,
	}
};

static DWORD g_arrQueueAddress_P16T2Q16_2[MSP_P16_E2T2_Q16_2::EBoardInfo::EBoardInfo_Channel][MSP_P16_E2T2_Q16_2::EBoardInfo::EBoardInfo_QueueCount] =
{
	{
		MSP_P16_E2T2_Q16_2::EQueueRegister::EQueueRegister_CH1_Queue1,
		MSP_P16_E2T2_Q16_2::EQueueRegister::EQueueRegister_CH1_Queue2,
		MSP_P16_E2T2_Q16_2::EQueueRegister::EQueueRegister_CH1_Queue3,
		MSP_P16_E2T2_Q16_2::EQueueRegister::EQueueRegister_CH1_Queue4,
		MSP_P16_E2T2_Q16_2::EQueueRegister::EQueueRegister_CH1_Queue5,
		MSP_P16_E2T2_Q16_2::EQueueRegister::EQueueRegister_CH1_Queue6,
		MSP_P16_E2T2_Q16_2::EQueueRegister::EQueueRegister_CH1_Queue7,
		MSP_P16_E2T2_Q16_2::EQueueRegister::EQueueRegister_CH1_Queue8,
	},
	{
		MSP_P16_E2T2_Q16_2::EQueueRegister::EQueueRegister_CH2_Queue1,
		MSP_P16_E2T2_Q16_2::EQueueRegister::EQueueRegister_CH2_Queue2,
		MSP_P16_E2T2_Q16_2::EQueueRegister::EQueueRegister_CH2_Queue3,
		MSP_P16_E2T2_Q16_2::EQueueRegister::EQueueRegister_CH2_Queue4,
		MSP_P16_E2T2_Q16_2::EQueueRegister::EQueueRegister_CH2_Queue5,
		MSP_P16_E2T2_Q16_2::EQueueRegister::EQueueRegister_CH2_Queue6,
		MSP_P16_E2T2_Q16_2::EQueueRegister::EQueueRegister_CH2_Queue7,
		MSP_P16_E2T2_Q16_2::EQueueRegister::EQueueRegister_CH2_Queue8,
	}
};

IMPLEMENT_DYNAMIC(CDeviceMVTechMSPDio, CDeviceDio)

BEGIN_MESSAGE_MAP(CDeviceMVTechMSPDio, CDeviceDio)
	ON_COMMAND_RANGE(EDeviceAddEnumeratedControlID_OutputLedStart, EDeviceAddEnumeratedControlID_OutputLedEnd, OnBnClickedOutLed)
	ON_COMMAND_RANGE(EDeviceAddEnumeratedControlID_ResetButtonStart, EDeviceAddEnumeratedControlID_ResetButtonEnd, OnBnClickedReset)
	ON_COMMAND_RANGE(EDeviceAddEnumeratedControlID_EnableButtonStart, EDeviceAddEnumeratedControlID_EnableButtonEnd, OnBnClickedEnableTrigger)
	ON_COMMAND_RANGE(EDeviceAddEnumeratedControlID_DisableButtonStart, EDeviceAddEnumeratedControlID_DisableButtonEnd, OnBnClickedDisableTrigger)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static LPCTSTR g_lpszDeviceMSPTrigger[EDeviceParameterMVTechMSPTrigger_Count] =
{
	_T("DeviceID"),
	_T("Board type"),
	_T("User interface information"),
	_T("Show DIO"),
	_T("Show Trg & Enc"),
	_T("[CH1]Show Queue"),
	_T("[Ch2]Show Queue"),
	_T("Show ID"),
	_T("Trigger infomation"),
	_T("Channel 1"),
	_T("[CH1]Encoder direction"),
	_T("[CH1]Start position"),
	_T("[CH1]End position"),
	_T("[CH1]Trigger onTime"),
	_T("[CH1]Trigger cycle"),
	_T("[CH1]Strobe time"),
	_T("Channel 2"),
	_T("[CH2]Encoder direction"),
	_T("[CH2]Start position"),
	_T("[CH2]End position"),
	_T("[CH2]Trigger onTime"),
	_T("[CH2]Trigger cycle"),
	_T("[CH2]Strobe time"),
	_T("Input status"),
	_T("Use Input No. 0"),
	_T("Use Input No. 1"),
	_T("Use Input No. 2"),
	_T("Use Input No. 3"),
	_T("Use Input No. 4"),
	_T("Use Input No. 5"),
	_T("Use Input No. 6"),
	_T("Use Input No. 7"),
	_T("Use Input No. 8"),
	_T("Use Input No. 9"),
	_T("Use Input No. 10"),
	_T("Use Input No. 11"),
	_T("Use Input No. 12"),
	_T("Use Input No. 13"),
	_T("Use Input No. 14"),
	_T("Use Input No. 15"),
	_T("Output status"),
	_T("Use Output No. 0"),
	_T("Use Output No. 1"),
	_T("Use Output No. 2"),
	_T("Use Output No. 3"),
	_T("Use Output No. 4"),
	_T("Use Output No. 5"),
	_T("Use Output No. 6"),
	_T("Use Output No. 7"),
	_T("Use Output No. 8"),
	_T("Use Output No. 9"),
	_T("Use Output No. 10"),
	_T("Use Output No. 11"),
	_T("Use Output No. 12"),
	_T("Use Output No. 13"),
	_T("Use Output No. 14"),
	_T("Use Output No. 15"),
	_T("Input Name"),
	_T("Input No. 0 Name"),
	_T("Input No. 1 Name"),
	_T("Input No. 2 Name"),
	_T("Input No. 3 Name"),
	_T("Input No. 4 Name"),
	_T("Input No. 5 Name"),
	_T("Input No. 6 Name"),
	_T("Input No. 7 Name"),
	_T("Input No. 8 Name"),
	_T("Input No. 9 Name"),
	_T("Input No. 10 Name"),
	_T("Input No. 11 Name"),
	_T("Input No. 12 Name"),
	_T("Input No. 13 Name"),
	_T("Input No. 14 Name"),
	_T("Input No. 15 Name"),
	_T("Output Name"),
	_T("Output No. 0 Name"),
	_T("Output No. 1 Name"),
	_T("Output No. 2 Name"),
	_T("Output No. 3 Name"),
	_T("Output No. 4 Name"),
	_T("Output No. 5 Name"),
	_T("Output No. 6 Name"),
	_T("Output No. 7 Name"),
	_T("Output No. 8 Name"),
	_T("Output No. 9 Name"),
	_T("Output No. 10 Name"),
	_T("Output No. 11 Name"),
	_T("Output No. 12 Name"),
	_T("Output No. 13 Name"),
	_T("Output No. 14 Name"),
	_T("Output No. 15 Name"),
};

static LPCTSTR g_lpszDeviceMSPBoardType[EDeviceMSPBoardType_Count] =
{
	_T("MSP_P16_E2T6_Q288"),
	_T("MSP_P16_E2T2_Q16_1"),
	_T("MSP_P16_E2T2_Q16_2"),
	_T("Delayed Queue"),
};

static LPCTSTR g_lpszDeviceEncoderDirection[EDeviceEncoderDirection_Count] =
{
	_T("CW"),
	_T("CCW"),
	_T("Both"),
};

static LPCTSTR g_lpszDeviceMSPSwitch[2] =
{
	_T("False"),
	_T("True"),
};

static LPCTSTR g_lpszDeviceDirection[2] =
{
	_T("CW"),
	_T("CCW"),
};

CDeviceMVTechMSPDio::CDeviceMVTechMSPDio()
{
	m_pPreEncoderValue = new long[MAX_ENCODER_COUNT];
	memset(m_pPreEncoderValue, 0, sizeof(int) * MAX_ENCODER_COUNT);
	
	m_pPreTriggerValue = new long[MAX_TRIGGER_COUNT];
	memset(m_pPreTriggerValue, 0, sizeof(int) * MAX_TRIGGER_COUNT);
	
	m_pDwCS = new DWORD[MAX_BASE_ADDR_NUMS];
	memset(m_pDwCS, 0, sizeof(DWORD) * MAX_BASE_ADDR_NUMS);
	
	m_pDwCSLen = new DWORD[MAX_BASE_ADDR_NUMS];
	memset(m_pDwCSLen, 0, sizeof(DWORD) * MAX_BASE_ADDR_NUMS);

	m_pTriggerEnabled = new bool[MAX_ENCODER_COUNT];
	memset(m_pDwCS, 0, sizeof(bool) * MAX_ENCODER_COUNT);
	
	m_pLiveStatus = new DWORD[MAX_CHANNEL_COUNT];
	memset(m_pLiveStatus, 0, sizeof(DWORD) * MAX_CHANNEL_COUNT);
	
	m_pRpc = new Ravid::Miscellaneous::CPerformanceCounter;
}


CDeviceMVTechMSPDio::~CDeviceMVTechMSPDio()
{
	FreeIO();
	FreeQueue();
	FreeEncoder();
	FreeTrigger();

	if(m_pLiveStatus)
	{
		delete[] m_pLiveStatus;
		m_pLiveStatus = nullptr;
	}

	if(m_pDwCS)
	{
		delete[] m_pDwCS;
		m_pDwCS = nullptr;
	}

	if(m_pDwCSLen)
	{
		delete[] m_pDwCSLen;
		m_pDwCSLen = nullptr;
	}

	if(m_pTriggerEnabled)
	{
		delete[] m_pTriggerEnabled;
		m_pTriggerEnabled = nullptr;
	}

	if(m_pPreEncoderValue)
	{
		delete[] m_pPreEncoderValue;
		m_pPreEncoderValue = nullptr;
	}

	if(m_pPreTriggerValue)
	{
		delete[] m_pPreTriggerValue;
		m_pPreTriggerValue = nullptr;
	}

	if(m_pDeviceObject)
	{
		delete m_pDeviceObject;
		m_pDeviceObject = nullptr;
	}

	if(m_pRpc)
	{
		delete m_pRpc;
		m_pRpc = nullptr;
	}
}

ptrdiff_t CDeviceMVTechMSPDio::OnMessage(CMessageBase * pMessage)
{
	__super::OnMessage(pMessage);

	do
	{
		if(!pMessage)
			break;

		if(pMessage->GetMessage() != EMessage_LanguageChange)
			break;

		if(!IsWindow(GetSafeHwnd()))
			break;

		CWnd* pCtrlUI = nullptr;

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_Input));

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 1);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_Output));

		long nEncoderCounts = GetEncoderPortNumber();
		long nChannel = GetChannelNumber();

		for(int i = 0; i < nEncoderCounts * nChannel; ++i)
		{
			pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 2 + i);
			if(pCtrlUI)
				pCtrlUI->SetWindowTextW(!i ? _T("[CH1] ") + CMultiLanguageManager::GetString(ELanguageParameter_EncoderCount) + _T(" :") : _T("[CH2] ") + CMultiLanguageManager::GetString(ELanguageParameter_EncoderCount) + _T(" :"));
		}

		long nTriggerCounts = GetTriggerPortNumber();


		for(int i = 0; i < nTriggerCounts * nChannel; ++i)
		{
			pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 4 + i);
			if(pCtrlUI)
			{
				CString strName;
				strName.Format(_T("%s %s %d :"), i < 3 ? _T("[CH1]") : _T("[CH2]"), CMultiLanguageManager::GetString(ELanguageParameter_TriggerCount), (i % 3) + 1);

				pCtrlUI->SetWindowTextW(strName);
			}
		}

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 10);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_EncoderAndTriggerInfo));


		for(int i = 0; i < nTriggerCounts * nChannel; ++i)
		{
			pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_EnableButtonStart + i);

			CString strChannel = i < nTriggerCounts ? _T("[CH1]") : _T("[CH2]");

			int nIndex = (i % 3) + 1;

			if(pCtrlUI)
			{
				CString strName;
				strName.Format(_T("%s %s %d"), strChannel, CMultiLanguageManager::GetString(ELanguageParameter_EnableTrigger), nIndex);

				pCtrlUI->SetWindowTextW(strName);
			}

			pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_DisableButtonStart + i);
			if(pCtrlUI)
			{
				CString strName;
				strName.Format(_T("%s %s %d"), strChannel, CMultiLanguageManager::GetString(ELanguageParameter_DisableTrigger), nIndex);

				pCtrlUI->SetWindowTextW(strName);
			}


			if(i < 2)
			{
				pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_ResetButtonStart + i);
				if(pCtrlUI)
				{
					CString strName;
					strName.Format(_T("%s %d"), CMultiLanguageManager::GetString(ELanguageParameter_ResetCounter), nIndex);

					pCtrlUI->SetWindowTextW(strName);
				}
			}
		}

		long nQueueCounts = GetQueueNumber();

		

		if(GetChannelNumber() != 1)
		{
			pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 18);
			if(pCtrlUI)
			{
				pCtrlUI->SetWindowTextW(_T("[CH1] ") + CMultiLanguageManager::GetString(ELanguageParameter_QueueInfo));
			}

			pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 19);
			if(pCtrlUI)
			{
				pCtrlUI->SetWindowTextW(_T("[CH2] ") + CMultiLanguageManager::GetString(ELanguageParameter_QueueInfo));
			}
		}
		else
		{
			pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 17);
			if(pCtrlUI)
				pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_QueueInfo));
		}

		pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 20);
		if(pCtrlUI)
			pCtrlUI->SetWindowTextW(CMultiLanguageManager::GetString(ELanguageParameter_Information));


		long nIDCount = GetIDCount();

		if(nIDCount > 0)
		{
			for(int i = 0; i < nIDCount; ++i)
			{
				pCtrlUI = GetDlgItem(EDeviceAddEnumeratedControlID_StaticStart + 21 + i);
				if(pCtrlUI)
				{
					CString strIDIndex;
					strIDIndex.Format(_T("%s %d : "), CMultiLanguageManager::GetString(ELanguageParameter_ID), i);

					pCtrlUI->SetWindowTextW(strIDIndex);
				}
			}
		}
	}
	while(false);

	return 0;
}

EDeviceInitializeResult CDeviceMVTechMSPDio::Initialize()
{
	EDeviceInitializeResult eReturn = EDeviceInitializeResult_UnknownError;

	CString strMessage, strStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("MVTechDio"));

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

		PLX_STATUS rc;
		PLX_DEVICE_KEY plxDeviceKey;

		DWORD dwRegValue[LOCAL_CONFIG_REG_CNT];

		memset(m_pDwCS, 0, sizeof(m_pDwCS));
		memset(m_pDwCSLen, 0, sizeof(m_pDwCSLen));
		memset(&plxDeviceKey, PCI_FIELD_IGNORE, sizeof(PLX_DEVICE_KEY));

		plxDeviceKey.VendorId = 0x10B7;
		plxDeviceKey.DeviceId = 0x9030;

		if(m_pDeviceObject)
		{
			delete m_pDeviceObject;
			m_pDeviceObject = nullptr;
		}

		m_pDeviceObject = new PLX_DEVICE_OBJECT;

		rc = PlxPci_DeviceFind(&plxDeviceKey, _ttoi(GetDeviceID()));

		if(rc != ApiSuccess)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtofindthedevice);
			eReturn = EDeviceInitializeResult_NotFoundDeviceInfo;
			break;
		}

		rc = PlxPci_DeviceOpen(&plxDeviceKey, (PLX_DEVICE_OBJECT*)m_pDeviceObject);

		if(rc != ApiSuccess)
		{
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Couldntopenthedevice);
			eReturn = EDeviceInitializeResult_NotOpenedDeviceError;
			break;
		}

		for(int i = 0; i < LOCAL_CONFIG_REG_CNT; i++)
		{
			dwRegValue[i] = PlxPci_PlxRegisterRead((PLX_DEVICE_OBJECT*)m_pDeviceObject, 0x00 + (i * 4), &rc);

			if(rc != ApiSuccess)
				break;
		}

		if(rc != ApiSuccess)
		{
			strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Failedtoread_s_fromthedevice), _T("apiRegister"));
			eReturn = EDeviceInitializeResult_ReadOnDeviceError;
			break;
		}

		m_pDwCS[0] = dwRegValue[5] & ~0x00000001;
		m_pDwCS[1] = dwRegValue[6] & ~0x00000001;
		m_pDwCS[2] = dwRegValue[7] & ~0x00000001;
		m_pDwCS[3] = dwRegValue[8] & ~0x00000001;

		m_pDwCSLen[0] = dwRegValue[0] >> 4;
		m_pDwCSLen[1] = dwRegValue[1] >> 4;
		m_pDwCSLen[2] = dwRegValue[2] >> 4;
		m_pDwCSLen[3] = dwRegValue[3] >> 4;

		m_bIsInitialized = true;

		ClearOutPort();

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

EDeviceTerminateResult CDeviceMVTechMSPDio::Terminate()
{
	EDeviceTerminateResult eReturn = EDeviceTerminateResult_UnknownError;

	CString strMessage;

	SetStatus(CMultiLanguageManager::GetString(ELanguageParameter_Terminated));

	if(!DoesModuleExist())
	{
		strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter_Couldntfind_s_APIdlls), _T("MVTechDio"));

		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);

		return EDeviceTerminateResult_NotFoundApiError;
	}

	do
	{
		ClearOutPort();

		m_bIsInitialized = false;

		if(m_pDeviceObject)
		{
			PlxPci_DeviceClose((PLX_DEVICE_OBJECT*)m_pDeviceObject);

			delete m_pDeviceObject;
			m_pDeviceObject = nullptr;
		}

		strMessage = CMultiLanguageManager::GetString(ELanguageParameter_TheDevicehasbeensuccessfullyterminated);

		CEventHandlerManager::BroadcastOnDeviceTerminated(this);
	}
	while(false);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceMVTechMSPDio::LoadSettings()
{
	bool bReturn = false;

	m_vctParameterFieldConfigurations.clear();

	do
	{
		AddParameterFieldConfigurations(EDeviceParameterMVTechMSPTrigger_DeviceID, g_lpszDeviceMSPTrigger[EDeviceParameterMVTechMSPTrigger_DeviceID], _T("0"));

		AddParameterFieldConfigurations(EDeviceParameterMVTechMSPTrigger_BoardType, g_lpszDeviceMSPTrigger[EDeviceParameterMVTechMSPTrigger_BoardType], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDeviceMSPBoardType, EDeviceMSPBoardType_Count), nullptr, 0);

		AddParameterFieldConfigurations(EDeviceParameterMVTechMSPTrigger_UIInfo, g_lpszDeviceMSPTrigger[EDeviceParameterMVTechMSPTrigger_UIInfo], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);

		AddParameterFieldConfigurations(EDeviceParameterMVTechMSPTrigger_ShowDIO, g_lpszDeviceMSPTrigger[EDeviceParameterMVTechMSPTrigger_ShowDIO], _T("1"), EParameterFieldType_Check, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterMVTechMSPTrigger_ShowTrgEnc, g_lpszDeviceMSPTrigger[EDeviceParameterMVTechMSPTrigger_ShowTrgEnc], _T("1"), EParameterFieldType_Check, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterMVTechMSPTrigger_ShowQueueCH1, g_lpszDeviceMSPTrigger[EDeviceParameterMVTechMSPTrigger_ShowQueueCH1], _T("1"), EParameterFieldType_Check, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterMVTechMSPTrigger_ShowQueueCH2, g_lpszDeviceMSPTrigger[EDeviceParameterMVTechMSPTrigger_ShowQueueCH2], _T("1"), EParameterFieldType_Check, nullptr, nullptr, 1);
		AddParameterFieldConfigurations(EDeviceParameterMVTechMSPTrigger_ShowID, g_lpszDeviceMSPTrigger[EDeviceParameterMVTechMSPTrigger_ShowID], _T("1"), EParameterFieldType_Check, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterMVTechMSPTrigger_TriggerInfo, g_lpszDeviceMSPTrigger[EDeviceParameterMVTechMSPTrigger_TriggerInfo], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);

		AddParameterFieldConfigurations(EDeviceParameterMVTechMSPTrigger_Channel1, g_lpszDeviceMSPTrigger[EDeviceParameterMVTechMSPTrigger_Channel1], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterMVTechMSPTrigger_EncoderDirectionCH1, g_lpszDeviceMSPTrigger[EDeviceParameterMVTechMSPTrigger_EncoderDirectionCH1], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDeviceEncoderDirection, EDeviceEncoderDirection_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMVTechMSPTrigger_StartPositionCH1, g_lpszDeviceMSPTrigger[EDeviceParameterMVTechMSPTrigger_StartPositionCH1], _T("1"), EParameterFieldType_Edit, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMVTechMSPTrigger_EndPositionCH1, g_lpszDeviceMSPTrigger[EDeviceParameterMVTechMSPTrigger_EndPositionCH1], _T("1"), EParameterFieldType_Edit, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMVTechMSPTrigger_TriggerOnTimeCH1, g_lpszDeviceMSPTrigger[EDeviceParameterMVTechMSPTrigger_TriggerOnTimeCH1], _T("1"), EParameterFieldType_Edit, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMVTechMSPTrigger_TriggerCycleCH1, g_lpszDeviceMSPTrigger[EDeviceParameterMVTechMSPTrigger_TriggerCycleCH1], _T("1"), EParameterFieldType_Edit, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMVTechMSPTrigger_StrobeTimeCH1, g_lpszDeviceMSPTrigger[EDeviceParameterMVTechMSPTrigger_StrobeTimeCH1], _T("1"), EParameterFieldType_Edit, nullptr, nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterMVTechMSPTrigger_Channel2, g_lpszDeviceMSPTrigger[EDeviceParameterMVTechMSPTrigger_Channel2], _T("1"), EParameterFieldType_None, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterMVTechMSPTrigger_EncoderDirectionCH2, g_lpszDeviceMSPTrigger[EDeviceParameterMVTechMSPTrigger_EncoderDirectionCH2], _T("0"), EParameterFieldType_Combo, ConvertStringArrayToComboElement(g_lpszDeviceEncoderDirection, EDeviceEncoderDirection_Count), nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMVTechMSPTrigger_StartPositionCH2, g_lpszDeviceMSPTrigger[EDeviceParameterMVTechMSPTrigger_StartPositionCH2], _T("1"), EParameterFieldType_Edit, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMVTechMSPTrigger_EndPositionCH2, g_lpszDeviceMSPTrigger[EDeviceParameterMVTechMSPTrigger_EndPositionCH2], _T("1"), EParameterFieldType_Edit, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMVTechMSPTrigger_TriggerOnTimeCH2, g_lpszDeviceMSPTrigger[EDeviceParameterMVTechMSPTrigger_TriggerOnTimeCH2], _T("1"), EParameterFieldType_Edit, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMVTechMSPTrigger_TriggerCycleCH2, g_lpszDeviceMSPTrigger[EDeviceParameterMVTechMSPTrigger_TriggerCycleCH2], _T("1"), EParameterFieldType_Edit, nullptr, nullptr, 2);
		AddParameterFieldConfigurations(EDeviceParameterMVTechMSPTrigger_StrobeTimeCH2, g_lpszDeviceMSPTrigger[EDeviceParameterMVTechMSPTrigger_StrobeTimeCH2], _T("1"), EParameterFieldType_Edit, nullptr, nullptr, 2);

		AddParameterFieldConfigurations(EDeviceParameterMVTechMSPTrigger_UseInput, g_lpszDeviceMSPTrigger[EDeviceParameterMVTechMSPTrigger_UseInput], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);

		for(int i = 0; i < MAX_IN_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterMVTechMSPTrigger_UseInputNo0 + i, g_lpszDeviceMSPTrigger[EDeviceParameterMVTechMSPTrigger_UseInputNo0 + i], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterMVTechMSPTrigger_UseOutput, g_lpszDeviceMSPTrigger[EDeviceParameterMVTechMSPTrigger_UseOutput], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);

		for(int i = 0; i < MAX_OUT_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterMVTechMSPTrigger_UseOutputNo0 + i, g_lpszDeviceMSPTrigger[EDeviceParameterMVTechMSPTrigger_UseOutputNo0 + i], _T("0"), EParameterFieldType_Check, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterMVTechMSPTrigger_InputName, g_lpszDeviceMSPTrigger[EDeviceParameterMVTechMSPTrigger_InputName], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);

		for(int i = 0; i < MAX_IN_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterMVTechMSPTrigger_InputNo0Name + i, g_lpszDeviceMSPTrigger[EDeviceParameterMVTechMSPTrigger_InputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);

		AddParameterFieldConfigurations(EDeviceParameterMVTechMSPTrigger_OutputName, g_lpszDeviceMSPTrigger[EDeviceParameterMVTechMSPTrigger_OutputName], _T("1"), EParameterFieldType_None, nullptr, nullptr, 0);

		for(int i = 0; i < MAX_OUT_PORT; ++i)
			AddParameterFieldConfigurations(EDeviceParameterMVTechMSPTrigger_OutputNo0Name + i, g_lpszDeviceMSPTrigger[EDeviceParameterMVTechMSPTrigger_OutputNo0Name + i], _T(""), EParameterFieldType_Edit, nullptr, nullptr, 1);

		bReturn = __super::LoadSettings();

		AllocateIO(MAX_IN_PORT, MAX_OUT_PORT);

		AllocateQueue(MAX_QUEUE_PORT, MAX_QUEUE_PORT);

		AllocateEncoder(MAX_ENCODER_COUNT);

		AllocateTrigger(MAX_TRIGGER_COUNT);

		EDeviceMSPBoardType eBoardType = (EDeviceMSPBoardType)_ttoi(m_vctParameterFieldConfigurations[EDeviceParameterMVTechMSPTrigger_BoardType].strParameterValue);

		DisplayParameter(eBoardType);

		for(auto iter = m_vctParameterFieldConfigurations.begin(); iter != m_vctParameterFieldConfigurations.end(); ++iter)
		{
			if(iter->strParameterName.Find(_T("Use Input No. ")) >= 0)
			{
				int nNumber = _ttoi(iter->strParameterName.Mid(14, 2));

				m_pUsedInput[nNumber] = _ttoi(iter->strParameterValue);
			}
			else if(iter->strParameterName.Find(_T("Use Output No. ")) >= 0)
			{
				int nNumber = _ttoi(iter->strParameterName.Mid(15, 2));

				m_pUsedOutput[nNumber] = _ttoi(iter->strParameterValue);
			}
			else if(iter->strParameterName.Find(_T("Input No. ")) >= 0)
			{
				int nNumber = _ttoi(iter->strParameterName.Mid(10, 2));

				m_pStrInputName[nNumber] = iter->strParameterValue;
			}
			else if(iter->strParameterName.Find(_T("Output No. ")) >= 0)
			{
				int nNumber = _ttoi(iter->strParameterName.Mid(11, 2));

				m_pStrOutputName[nNumber] = iter->strParameterValue;
			}
			else
				bReturn = false;
		}

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceMVTechMSPDio::PCIWrite(DWORD dwAddr, DWORD dwData)
{
	bool bReturn = false;

	do
	{
		PLX_STATUS rc;

		rc = PlxPci_PciBarSpaceWrite((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, dwAddr, &dwData, sizeof(dwData), BitSize32, true);

		if(rc != ApiSuccess)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}


DWORD CDeviceMVTechMSPDio::PCIRead(DWORD dwAddr)
{
	DWORD dwData = 0;

	do
	{
		PLX_STATUS rc;

		rc = PlxPci_PciBarSpaceRead((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, dwAddr, &dwData, sizeof(dwData), BitSize32, true);

		if(rc != ApiSuccess)
			dwData = 0;
	}
	while(false);

	return dwData;
}


DWORD CDeviceMVTechMSPDio::PciReadReg(DWORD dwOffset)
{
	DWORD dwRegValue = 0;

	do
	{
		if(!IsInitialized())
			break;

		PLX_STATUS rc;

		dwRegValue = PlxPci_PlxRegisterRead((PLX_DEVICE_OBJECT*)m_pDeviceObject, dwOffset, &rc);

		if(rc != ApiSuccess)
			dwRegValue = 0;
	}
	while(false);

	return dwRegValue;
}


bool CDeviceMVTechMSPDio::PciWriteReg(DWORD dwOffset, DWORD dwData)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		PLX_STATUS rc;

		rc = PlxPci_PlxRegisterWrite((PLX_DEVICE_OBJECT*)m_pDeviceObject, dwOffset, dwData);

		if(rc != ApiSuccess)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceMVTechMSPDio::PCIWrite(DWORD dwCS, DWORD dwAddr, DWORD dwData)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(dwCS >= MAX_BASE_ADDR_NUMS)
			break;

		bReturn = PCIWrite(m_pDwCS[dwCS] + dwAddr, dwData);
	}
	while(false);

	return bReturn;
}


DWORD CDeviceMVTechMSPDio::PCIRead(DWORD dwCS, DWORD dwAddr)
{
	DWORD dwReturn = 0;

	do
	{
		if(!IsInitialized())
			break;

		if(dwCS >= MAX_BASE_ADDR_NUMS)
			break;

		dwReturn = PCIRead(m_pDwCS[dwCS] + dwAddr);
	}
	while(false);

	return dwReturn;
}

long CDeviceMVTechMSPDio::GetChannelNumber()
{
	long nCount = 0;

	do
	{
		EDeviceMSPBoardType eBoardType = EDeviceMSPBoardType_Count;

		if(GetBoardType(&eBoardType))
			break;

		switch(eBoardType)
		{
		case EDeviceMSPBoardType_MSP_P16_E2T6_Q288:
			nCount = MSP_P16_E2T6_Q288::EBoardInfo::EBoardInfo_Channel;
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
			nCount = MSP_P16_E2T2_Q16_1::EBoardInfo::EBoardInfo_Channel;
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2:
			nCount = MSP_P16_E2T2_Q16_2::EBoardInfo::EBoardInfo_Channel;
			break;
		default:
			break;
		}
	}
	while(false);

	return nCount;
}

long CDeviceMVTechMSPDio::GetQueueNumber()
{
	long nCount = 0;

	do
	{
		EDeviceMSPBoardType eBoardType = EDeviceMSPBoardType_Count;

		if(GetBoardType(&eBoardType))
			break;

		switch(eBoardType)
		{
		case EDeviceMSPBoardType_MSP_P16_E2T6_Q288:
			nCount = MSP_P16_E2T6_Q288::EBoardInfo::EBoardInfo_QueueCount;
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
			nCount = MSP_P16_E2T2_Q16_1::EBoardInfo::EBoardInfo_QueueCount;
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2:
			nCount = MSP_P16_E2T2_Q16_2::EBoardInfo::EBoardInfo_QueueCount;
			break;
		default:
			break;
		}
	}
	while(false);

	return nCount;
}

long CDeviceMVTechMSPDio::GetQueueStatusPortNumber()
{
	long nCount = 0;

	do
	{
		EDeviceMSPBoardType eBoardType = EDeviceMSPBoardType_Count;

		if(GetBoardType(&eBoardType))
			break;

		switch(eBoardType)
		{
		case EDeviceMSPBoardType_MSP_P16_E2T6_Q288:
			nCount = MSP_P16_E2T6_Q288::EBoardInfo::EBoardInfo_QueueStatusCount;
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
			nCount = MSP_P16_E2T2_Q16_1::EBoardInfo::EBoardInfo_QueueStatusCount;
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2:
			nCount = MSP_P16_E2T2_Q16_2::EBoardInfo::EBoardInfo_QueueStatusCount;
			break;
		default:
			break;
		}
	}
	while(false);

	return nCount;
}

long CDeviceMVTechMSPDio::GetOutputPortNumber()
{
	long nCount = 0;

	do
	{
		EDeviceMSPBoardType eBoardType = EDeviceMSPBoardType_Count;

		if(GetBoardType(&eBoardType))
			break;

		switch(eBoardType)
		{
		case EDeviceMSPBoardType_MSP_P16_E2T6_Q288:
			nCount = MSP_P16_E2T6_Q288::EBoardInfo::EBoardInfo_Outport;
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
			nCount = MSP_P16_E2T2_Q16_1::EBoardInfo::EBoardInfo_Outport;
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2:
			nCount = MSP_P16_E2T2_Q16_2::EBoardInfo::EBoardInfo_Outport;
			break;
		default:
			break;
		}
	}
	while(false);

	return nCount;
}

long CDeviceMVTechMSPDio::GetInputPortNumber()
{
	long nCount = 0;

	do
	{
		EDeviceMSPBoardType eBoardType = EDeviceMSPBoardType_Count;

		if(GetBoardType(&eBoardType))
			break;

		switch(eBoardType)
		{
		case EDeviceMSPBoardType_MSP_P16_E2T6_Q288:
			nCount = MSP_P16_E2T6_Q288::EBoardInfo::EBoardInfo_Inport;
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
			nCount = MSP_P16_E2T2_Q16_1::EBoardInfo::EBoardInfo_Inport;
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2:
			nCount = MSP_P16_E2T2_Q16_2::EBoardInfo::EBoardInfo_Inport;
			break;
		default:
			break;
		}
	}
	while(false);

	return nCount;
}

long CDeviceMVTechMSPDio::GetEncoderPortNumber()
{
	long nCount = 0;

	do
	{
		EDeviceMSPBoardType eBoardType = EDeviceMSPBoardType_Count;

		if(GetBoardType(&eBoardType))
			break;

		switch(eBoardType)
		{
		case EDeviceMSPBoardType_MSP_P16_E2T6_Q288:
			nCount = MSP_P16_E2T6_Q288::EBoardInfo::EBoardInfo_EncoderCount;
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
			nCount = MSP_P16_E2T2_Q16_1::EBoardInfo::EBoardInfo_EncoderCount;
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2:
			nCount = MSP_P16_E2T2_Q16_2::EBoardInfo::EBoardInfo_EncoderCount;
			break;
		default:
			break;
		}
	}
	while(false);

	return nCount;
}

long CDeviceMVTechMSPDio::GetTriggerPortNumber()
{
	long nCount = 0;

	do
	{
		EDeviceMSPBoardType eBoardType = EDeviceMSPBoardType_Count;

		if(GetBoardType(&eBoardType))
			break;

		switch(eBoardType)
		{
		case EDeviceMSPBoardType_MSP_P16_E2T6_Q288:
			nCount = MSP_P16_E2T6_Q288::EBoardInfo::EBoardInfo_TriggerCount;
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
			nCount = MSP_P16_E2T2_Q16_1::EBoardInfo::EBoardInfo_TriggerCount;
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2:
			nCount = MSP_P16_E2T2_Q16_2::EBoardInfo::EBoardInfo_TriggerCount;
			break;
		default:
			break;
		}
	}
	while(false);

	return nCount;
}

long CDeviceMVTechMSPDio::GetEncoderCount(int nChannel)
{
	DWORD dwReturn = 0;

	do
	{
		if(!IsInitialized())
			break;

		if(nChannel < 0 || nChannel >= GetChannelNumber())
			break;

		EDeviceMSPBoardType eBoardType = EDeviceMSPBoardType_Count;

		if(GetBoardType(&eBoardType))
			break;

		switch(eBoardType)
		{
		case EDeviceMSPBoardType_MSP_P16_E2T6_Q288:
			{
				switch(nChannel)
				{
				case 0:
					dwReturn = PCIRead(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T6_Q288::EEncoderAddress_CH1);
					break;
				case 1:
					dwReturn = PCIRead(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T6_Q288::EEncoderAddress_CH2);
					break;
				default:
					break;
				}
			}
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
			{
				switch(nChannel)
				{
				case 0:
					dwReturn = PCIRead(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EEncoderAddress_CH1);
					break;
				case 1:
					dwReturn = PCIRead(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EEncoderAddress_CH2);
					break;
				default:
					break;
				}
			}
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2:
			{
				switch(nChannel)
				{
				case 0:
					dwReturn = PCIRead(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EEncoderAddress_CH1);
					break;
				case 1:
					dwReturn = PCIRead(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EEncoderAddress_CH2);
					break;
				default:
					break;
				}
			}
			break;
		default:
			break;
		}

	}
	while(false);

	return dwReturn;
}

long CDeviceMVTechMSPDio::GetTriggerCount(int nChannel, int nSelect)
{
	DWORD dwReturn = 0;

	do
	{
		if(!IsInitialized())
			break;

		if(nChannel < 0 || nChannel >= GetChannelNumber())
			break;

		if(nSelect < 0 || nSelect >= GetTriggerPortNumber())
			break;

		EDeviceMSPBoardType eBoardType = EDeviceMSPBoardType_Count;

		if(GetBoardType(&eBoardType))
			break;

		switch(eBoardType)
		{
		case EDeviceMSPBoardType_MSP_P16_E2T6_Q288:
			{
				switch(nChannel)
				{
				case 0:
					dwReturn = PCIRead(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T6_Q288::ETriggerAddress_CH1_Cam1 + (nSelect * 4));
					break;
				case 1:
					dwReturn = PCIRead(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T6_Q288::ETriggerAddress_CH2_Cam1 + (nSelect * 4));
					break;
				default:
					break;
				}
			}
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
			{
				switch(nChannel)
				{
				case 0:
					dwReturn = PCIRead(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::ETriggerAddress_CH1);
					break;
				case 1:
					dwReturn = PCIRead(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::ETriggerAddress_CH2);
					break;
				default:
					break;
				}
			}
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2:
			{
				switch(nChannel)
				{
				case 0:
					dwReturn = PCIRead(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::ETriggerAddress_CH1);
					break;
				case 1:
					dwReturn = PCIRead(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::ETriggerAddress_CH2);
					break;
				default:
					break;
				}
			}
			break;
		default:
			break;
		}
	}
	while(false);

	return dwReturn;
}

bool CDeviceMVTechMSPDio::ResetDevice(int nChannel)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(nChannel < 0 || nChannel >= GetChannelNumber())
			break;

		EDeviceMSPBoardType eBoardType = EDeviceMSPBoardType_Count;

		if(GetBoardType(&eBoardType))
			break;

		switch(eBoardType)
		{
		case EDeviceMSPBoardType_MSP_P16_E2T6_Q288:
			{
				switch(nChannel)
				{
				case 0:
					{
						bReturn &= PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T6_Q288::EResetAddress_CH1, 0x00);
						Sleep(1);
						bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T6_Q288::EResetAddress_CH1, 0x01);
						Sleep(1);
						bReturn &= PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T6_Q288::EResetAddress_CH1, 0x00);
					}
					break;
				case 1:
					{
						bReturn &= PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T6_Q288::EResetAddress_CH2, 0x00);
						Sleep(1);
						bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T6_Q288::EResetAddress_CH2, 0x01);
						Sleep(1);
						bReturn &= PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T6_Q288::EResetAddress_CH2, 0x00);
					}
					break;
				default:
					break;
				}
			}
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
			{
				switch(nChannel)
				{
				case 0:
					{
						bReturn &= PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EResetAddress_CH1, 0x00);
						Sleep(1);
						bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EResetAddress_CH1, 0x01);
						Sleep(1);
						bReturn &= PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EResetAddress_CH1, 0x00);
					}
					break;
				case 1:
					{
						bReturn &= PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EResetAddress_CH2, 0x00);
						Sleep(1);
						bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EResetAddress_CH2, 0x01);
						Sleep(1);
						bReturn &= PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EResetAddress_CH2, 0x00);
					}
					break;
				default:
					break;
				}
			}
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2:
			{
				switch(nChannel)
				{
				case 0:
					{
						bReturn &= PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EResetAddress_CH1, 0x00);
						Sleep(1);
						bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EResetAddress_CH1, 0x01);
						Sleep(1);
						bReturn &= PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EResetAddress_CH1, 0x00);
					}
					break;
				case 1:
					{
						bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EResetAddress_CH2, 0x00);
						Sleep(1);
						bReturn &= PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EResetAddress_CH2, 0x01);
						Sleep(1);
						bReturn &= PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EResetAddress_CH2, 0x00);
					}
					break;
				default:
					break;
				}
			}
			break;
		default:
			break;
		}

	}
	while(false);

	return bReturn;
}

bool CDeviceMVTechMSPDio::ExcuteGrab(int nChannel)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(nChannel < 0 || nChannel >= GetChannelNumber())
			break;

		EDeviceMSPBoardType eBoardType = EDeviceMSPBoardType_Count;

		if(GetBoardType(&eBoardType))
			break;

		switch(eBoardType)
		{
		case EDeviceMSPBoardType_MSP_P16_E2T6_Q288:
			{
				switch(nChannel)
				{
				case 0:
					{
						bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T6_Q288::EGrabAddress_CH1, 0x01);
						Sleep(1);
						bReturn &= PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T6_Q288::EGrabAddress_CH1, 0x00);
					}
					break;
				case 1:
					{
						bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T6_Q288::EGrabAddress_CH2, 0x01);
						Sleep(1);
						bReturn &= PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T6_Q288::EGrabAddress_CH2, 0x00);
					}
					break;
				default:
					break;
				}
			}
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
			{
				switch(nChannel)
				{
				case 0:
					{
						bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EGrabAddress_CH1, 0x01);
						Sleep(1);
						bReturn &= PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EGrabAddress_CH1, 0x00);
					}
					break;
				case 1:
					{
						bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EGrabAddress_CH2, 0x01);
						Sleep(1);
						bReturn &= PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EGrabAddress_CH2, 0x00);
					}
					break;
				default:
					break;
				}
			}
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2:
			{
				switch(nChannel)
				{
				case 0:
					bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EGrabAddress_CH1, 0x01);
					break;
				case 1:
					bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EGrabAddress_CH2, 0x01);
					break;
				default:
					break;
				}
			}
			break;
		default:
			break;
		}

	}
	while(false);

	return bReturn;
}

bool CDeviceMVTechMSPDio::ReadInBit(int nBit)
{
	bool bReturn = false;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		long nPort = GetInputPortNumber();

		if(nBit >= nPort || nBit < 0)
			break;

		DWORD dwInAddress = 0;

		EDeviceMSPBoardType eBoardType = EDeviceMSPBoardType_Count;

		if(GetBoardType(&eBoardType))
			break;

		switch(eBoardType)
		{
		case EDeviceMSPBoardType_MSP_P16_E2T6_Q288:
			dwInAddress = MSP_P16_E2T6_Q288::EIOAddress_Input;
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
			dwInAddress = MSP_P16_E2T2_Q16_1::EIOAddress_Input;
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2:
			dwInAddress = MSP_P16_E2T2_Q16_2::EIOAddress_Input;
			break;
		default:
			break;
		}

		PLX_STATUS rc = PlxPci_PciBarSpaceRead((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_pDwCS[PCI_BASE_CS0] + dwInAddress, &m_dwInStatus, sizeof(m_dwInStatus), BitSize32, true);

		if(rc != ApiSuccess)
			break;

		bReturn = (m_dwInStatus >> nBit) & 0x01;
	}
	while(false);

	Unlock();

	return bReturn;
}

bool CDeviceMVTechMSPDio::ReadOutBit(int nBit)
{
	bool bReturn = false;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		long nPort = GetOutputPortNumber();

		if(nBit >= nPort || nBit < 0)
			break;

		bReturn = (m_dwOutStatus >> nBit) & 0x01;
	}
	while(false);

	Unlock();

	return bReturn;
}

BYTE CDeviceMVTechMSPDio::ReadOutByte()
{
	BYTE cReturn = 0;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		cReturn = m_dwOutStatus & BYTE_MAX;
	}
	while(false);

	Unlock();

	return cReturn;
}

BYTE CDeviceMVTechMSPDio::ReadInByte()
{
	BYTE cReturn = 0;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		DWORD dwInAddress = 0x00;

		EDeviceMSPBoardType eBoardType = EDeviceMSPBoardType_Count;

		if(GetBoardType(&eBoardType))
			break;

		switch(eBoardType)
		{
		case EDeviceMSPBoardType_MSP_P16_E2T6_Q288:
			dwInAddress = MSP_P16_E2T6_Q288::EIOAddress_Input;
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
			dwInAddress = MSP_P16_E2T2_Q16_1::EIOAddress_Input;
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2:
			dwInAddress = MSP_P16_E2T2_Q16_2::EIOAddress_Input;
			break;
		default:
			break;
		}

		PLX_STATUS rc = PlxPci_PciBarSpaceRead((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_pDwCS[PCI_BASE_CS0] + dwInAddress, &m_dwInStatus, sizeof(m_dwInStatus), BitSize32, true);

		if(rc != ApiSuccess)
			break;

		cReturn = m_dwInStatus & BYTE_MAX;
	}
	while(false);

	Unlock();

	return cReturn;
}

WORD CDeviceMVTechMSPDio::ReadOutWord()
{
	WORD wReturn = 0;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		wReturn = m_dwOutStatus & WORD_MAX;
	}
	while(false);

	Unlock();

	return wReturn;
}

WORD CDeviceMVTechMSPDio::ReadInWord()
{
	WORD wReturn = 0;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		DWORD dwInAddress = 0x00;

		EDeviceMSPBoardType eBoardType = EDeviceMSPBoardType_Count;

		if(GetBoardType(&eBoardType))
			break;

		switch(eBoardType)
		{
		case EDeviceMSPBoardType_MSP_P16_E2T6_Q288:
			dwInAddress = MSP_P16_E2T6_Q288::EIOAddress_Input;
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
			dwInAddress = MSP_P16_E2T2_Q16_1::EIOAddress_Input;
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2:
			dwInAddress = MSP_P16_E2T2_Q16_2::EIOAddress_Input;
			break;
		default:
			break;
		}

		PLX_STATUS rc = PlxPci_PciBarSpaceRead((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_pDwCS[PCI_BASE_CS0] + dwInAddress, &m_dwInStatus, sizeof(m_dwInStatus), BitSize32, true);

		if(rc != ApiSuccess)
			break;

		wReturn = m_dwInStatus & WORD_MAX;
	}
	while(false);

	Unlock();

	return wReturn;
}

DWORD CDeviceMVTechMSPDio::ReadOutDword()
{
	DWORD dwReturn = 0;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		dwReturn = m_dwOutStatus & DWORD_MAX;
	}
	while(false);

	Unlock();

	return dwReturn;
}

DWORD CDeviceMVTechMSPDio::ReadInDword()
{
	DWORD dwReturn = 0;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		DWORD dwInAddress = 0x00;

		EDeviceMSPBoardType eBoardType = EDeviceMSPBoardType_Count;

		if(GetBoardType(&eBoardType))
			break;

		switch(eBoardType)
		{
		case EDeviceMSPBoardType_MSP_P16_E2T6_Q288:
			dwInAddress = MSP_P16_E2T6_Q288::EIOAddress_Input;
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
			dwInAddress = MSP_P16_E2T2_Q16_1::EIOAddress_Input;
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2:
			dwInAddress = MSP_P16_E2T2_Q16_2::EIOAddress_Input;
			break;
		default:
			break;
		}

		PLX_STATUS rc = PlxPci_PciBarSpaceRead((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_pDwCS[PCI_BASE_CS0] + dwInAddress, &m_dwInStatus, sizeof(m_dwInStatus), BitSize32, true);

		if(rc != ApiSuccess)
			break;

		dwReturn = m_dwInStatus;
	}
	while(false);

	Unlock();

	return dwReturn;
}

bool CDeviceMVTechMSPDio::WriteOutBit(int nBit, bool bOn)
{
	bool bReturn = false;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		long nPort = GetOutputPortCount();

		if(nBit >= nPort)
			break;

		if(bOn)
			m_dwOutStatus |= 1 << nBit;
		else
			m_dwOutStatus &= ~(1 << nBit);

		DWORD dwOutAddress = 0x80;

		EDeviceMSPBoardType eBoardType = EDeviceMSPBoardType_Count;

		if(GetBoardType(&eBoardType))
			break;

		switch(eBoardType)
		{
		case EDeviceMSPBoardType_MSP_P16_E2T6_Q288:
			dwOutAddress = MSP_P16_E2T6_Q288::EIOAddress_Output;
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
			dwOutAddress = MSP_P16_E2T2_Q16_1::EIOAddress_Output;
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2:
			dwOutAddress = MSP_P16_E2T2_Q16_2::EIOAddress_Output;
			break;
		default:
			break;
		}

		PLX_STATUS rc = PlxPci_PciBarSpaceWrite((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_pDwCS[PCI_BASE_CS0] + dwOutAddress, &m_dwOutStatus, sizeof(m_dwOutStatus), BitSize32, true);

		bReturn = rc == ApiSuccess;
	}
	while(false);

	Unlock();

	return bReturn;
}

bool CDeviceMVTechMSPDio::WriteInBit(int nBit, bool bOn)
{
	return false;
}

bool CDeviceMVTechMSPDio::WriteOutByte(BYTE cByte)
{
	bool bReturn = false;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		m_dwOutStatus = cByte;

		DWORD dwOutAddress = 0x80;

		EDeviceMSPBoardType eBoardType = EDeviceMSPBoardType_Count;

		if(GetBoardType(&eBoardType))
			break;

		switch(eBoardType)
		{
		case EDeviceMSPBoardType_MSP_P16_E2T6_Q288:
			dwOutAddress = MSP_P16_E2T6_Q288::EIOAddress_Output;
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
			dwOutAddress = MSP_P16_E2T2_Q16_1::EIOAddress_Output;
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2:
			dwOutAddress = MSP_P16_E2T2_Q16_2::EIOAddress_Output;
			break;
		default:
			break;
		}

		PLX_STATUS rc = PlxPci_PciBarSpaceWrite((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_pDwCS[PCI_BASE_CS0] + dwOutAddress, &m_dwOutStatus, sizeof(m_dwOutStatus), BitSize32, true);

		bReturn = rc == ApiSuccess;
	}
	while(false);

	Unlock();

	return bReturn;
}

bool CDeviceMVTechMSPDio::WriteInByte(BYTE cByte)
{
	return false;
}

bool CDeviceMVTechMSPDio::WriteOutWord(WORD wWord)
{
	bool bReturn = false;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		DWORD dwOutAddress = 0x80;

		EDeviceMSPBoardType eBoardType = EDeviceMSPBoardType_Count;

		if(GetBoardType(&eBoardType))
			break;

		switch(eBoardType)
		{
		case EDeviceMSPBoardType_MSP_P16_E2T6_Q288:
			dwOutAddress = MSP_P16_E2T6_Q288::EIOAddress_Output;
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
			dwOutAddress = MSP_P16_E2T2_Q16_1::EIOAddress_Output;
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2:
			dwOutAddress = MSP_P16_E2T2_Q16_2::EIOAddress_Output;
			break;
		default:
			break;
		}

		m_dwOutStatus = wWord;

		PLX_STATUS rc = PlxPci_PciBarSpaceWrite((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_pDwCS[PCI_BASE_CS0] + dwOutAddress, &m_dwOutStatus, sizeof(m_dwOutStatus), BitSize32, true);

		bReturn = rc == ApiSuccess;
	}
	while(false);

	Unlock();

	return bReturn;
}

bool CDeviceMVTechMSPDio::WriteInWord(WORD wWord)
{
	return false;
}

bool CDeviceMVTechMSPDio::WriteOutDword(DWORD dwDword)
{
	bool bReturn = false;

	Lock();

	do
	{
		if(!IsInitialized())
			break;

		DWORD dwOutAddress = 0x80;

		EDeviceMSPBoardType eBoardType = EDeviceMSPBoardType_Count;

		if(GetBoardType(&eBoardType))
			break;

		switch(eBoardType)
		{
		case EDeviceMSPBoardType_MSP_P16_E2T6_Q288:
			dwOutAddress = MSP_P16_E2T6_Q288::EIOAddress_Output;
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
			dwOutAddress = MSP_P16_E2T2_Q16_1::EIOAddress_Output;
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2:
			dwOutAddress = MSP_P16_E2T2_Q16_2::EIOAddress_Output;
			break;
		default:
			break;
		}

		m_dwOutStatus = dwDword;

		PLX_STATUS rc = PlxPci_PciBarSpaceWrite((PLX_DEVICE_OBJECT*)m_pDeviceObject, 2, m_pDwCS[PCI_BASE_CS0] + dwOutAddress, &m_dwOutStatus, sizeof(m_dwOutStatus), BitSize32, true);

		bReturn = rc == ApiSuccess;
	}
	while(false);

	Unlock();

	return bReturn;
}

bool CDeviceMVTechMSPDio::WriteInDword(DWORD dwDword)
{
	return false;
}

bool CDeviceMVTechMSPDio::ClearOutPort()
{
	return WriteOutDword(0);
}

bool CDeviceMVTechMSPDio::ClearInPort()
{
	return false;
}

long CDeviceMVTechMSPDio::GetOutputPortCount()
{
	return MAX_OUT_PORT;
}

long CDeviceMVTechMSPDio::GetInputPortCount()
{
	return MAX_IN_PORT;
}

bool CDeviceMVTechMSPDio::SetQueuePosition(long nChannel, long nIndex, DWORD dwPos)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(nChannel < 0 || nChannel >= GetChannelNumber())
			break;

		if(nIndex < 0 || nIndex >= GetQueueNumber())
			break;

		EDeviceMSPBoardType eBoardType = EDeviceMSPBoardType_Count;

		if(GetBoardType(&eBoardType))
			break;

		switch(eBoardType)
		{
		case EDeviceMSPBoardType_MSP_P16_E2T6_Q288:
			bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + g_arrQueueAddress_P16T6Q288_1[nChannel][nIndex], dwPos);
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
			bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + g_arrQueueAddress_P16T2Q16_1[nChannel][nIndex], dwPos);
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2:
			bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + g_arrQueueAddress_P16T2Q16_2[nChannel][nIndex], dwPos);
			break;
		default:
			break;
		}

		if(bReturn)
		{
			*(m_pQueueValueCh1 + nIndex) = dwPos;
			*(m_pQueueValueCh2 + nIndex) = dwPos;
		}
	}
	while(false);

	return bReturn;
}

bool CDeviceMVTechMSPDio::SetQueueOrderPosition(long nChannel, long nIndex, DWORD dwCh)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(nChannel < 0 || nChannel >= GetChannelNumber())
			break;

		if(nIndex < 0 || nIndex >= GetQueueNumber())
			break;

		EDeviceMSPBoardType eBoardType = EDeviceMSPBoardType_Count;

		if(GetBoardType(&eBoardType))
			break;

		switch(eBoardType)
		{
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
			bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + g_arrQueueOrderAdress_P16T2Q16_1[nChannel][nIndex], dwCh);
			break;
		default:
			break;
		}
	}
	while(false);

	return bReturn;
}

DWORD CDeviceMVTechMSPDio::GetQueueStatus(long nChannel, long nIndex)
{
	DWORD dwReturn = 0;

	do
	{
		if(!IsInitialized())
			break;

		if(nChannel < 0 || nChannel >= GetChannelNumber())
			break;

		if(nIndex < 0 || nIndex >= GetQueueStatusPortNumber())
			break;

		EDeviceMSPBoardType eBoardType = EDeviceMSPBoardType_Count;

		if(GetBoardType(&eBoardType))
			break;

		switch(eBoardType)
		{
		case EDeviceMSPBoardType_MSP_P16_E2T6_Q288:
			dwReturn = PCIRead(m_pDwCS[PCI_BASE_CS0] + g_arrQueueStatusAddress_P16T6Q288_1[nChannel][nIndex]);
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
			dwReturn = PCIRead(m_pDwCS[PCI_BASE_CS0] + g_arrQueueStatusAddress_P16T2Q16_1[nChannel][nIndex]);
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2:
			dwReturn = PCIRead(m_pDwCS[PCI_BASE_CS0] + g_arrQueueStatusAddress_P16T2Q16_2[nChannel][nIndex]);
			break;
		default:
			break;
		}
	}
	while(false);

	return dwReturn;
}

bool CDeviceMVTechMSPDio::EnableTrigger(int nChannel)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		int nChannelNumber = GetChannelNumber();

		if(nChannel < 0 || nChannel >= nChannelNumber)
			break;

		EDeviceMSPBoardType eBoardType = EDeviceMSPBoardType_Count;

		if(GetBoardType(&eBoardType))
			break;

		EDeviceEncoderDirection eDir = EDeviceEncoderDirection_Count;

		if(!nChannel)
		{
			if(GetEncoderDirection_CH1(&eDir))
				break;
		}
		else
		{
			if(GetEncoderDirection_CH2(&eDir))
				break;
		}

		DWORD dwTriggerMode = 0;

		switch(eBoardType)
		{
		case EDeviceMSPBoardType_MSP_P16_E2T6_Q288:
			dwTriggerMode = 0x01;
			bReturn = true;
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
			{
				if(eDir >= EDeviceEncoderDirection_Both)
					break;

				switch(nChannel)
				{
				case 0:
					dwTriggerMode = (eDir + 1) * 0x04;
					bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::ETriggerEnableAddress_CH1, dwTriggerMode);
					break;
				case 1:
					dwTriggerMode = (eDir + 1) * 0x04;
					bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::ETriggerEnableAddress_CH2, dwTriggerMode);
					break;
				default:
					break;
				}
			}
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2:
			{
				if(eDir >= EDeviceEncoderDirection_Both)
					break;

				switch(nChannel)
				{
				case 0:
					dwTriggerMode = (eDir + 1) * 0x04;
					bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::ETriggerEnableAddress_CH1, dwTriggerMode);
					break;
				case 1:
					dwTriggerMode = (eDir + 1) * 0x04;
					bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::ETriggerEnableAddress_CH2, dwTriggerMode);
					break;
				default:
					break;
				}
			}
			break;
		default:
			break;
		}

		if(bReturn)
			m_pTriggerEnabled[nChannel] = dwTriggerMode;
	}
	while(false);

	return bReturn;
}

bool CDeviceMVTechMSPDio::DisableTrigger(int nChannel)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		int nChannelNumber = GetChannelNumber();

		if(nChannel < 0 || nChannel >= nChannelNumber)
			break;

		EDeviceMSPBoardType eBoardType = EDeviceMSPBoardType_Count;

		if(GetBoardType(&eBoardType))
			break;

		DWORD dwTriggerMode = m_pTriggerEnabled[nChannel];

		switch(eBoardType)
		{
		case EDeviceMSPBoardType_MSP_P16_E2T6_Q288:
			dwTriggerMode = 0x00;
			bReturn = true;
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
			{
				switch(nChannel)
				{
				case 0:
					dwTriggerMode = 0x00;
					bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::ETriggerEnableAddress_CH1, dwTriggerMode);
					break;
				case 1:
					dwTriggerMode = 0x00;
					bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::ETriggerEnableAddress_CH2, dwTriggerMode);

					break;
				default:
					break;
				}
			}
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2:
			{
				switch(nChannel)
				{
				case 0:
					dwTriggerMode = 0x00;
					bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::ETriggerEnableAddress_CH1, dwTriggerMode);
					break;
				case 1:
					dwTriggerMode = 0x00;
					bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::ETriggerEnableAddress_CH2, dwTriggerMode);
					break;
				default:
					break;
				}
			}
			break;
		default:
			break;
		}

		if(bReturn)
			m_pTriggerEnabled[nChannel] = dwTriggerMode;
	}
	while(false);

	return bReturn;
}

bool CDeviceMVTechMSPDio::IsTriggerEnabled(int nChannel)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		int nChannelNumber = GetChannelNumber();

		if(nChannel < 0 || nChannel >= nChannelNumber)
			break;

		bReturn = m_pTriggerEnabled[nChannel];
	}
	while(false);

	return bReturn;
}

bool CDeviceMVTechMSPDio::EnableLive(int nChannel)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		int nChannelNumber = GetChannelNumber();

		if(nChannel < 0 || nChannel >= nChannelNumber)
			break;

		EDeviceMSPBoardType eBoardType = EDeviceMSPBoardType_Count;

		if(GetBoardType(&eBoardType))
			break;

		DWORD dwLiveStatus = m_pLiveStatus[nChannel];

		switch(eBoardType)
		{
		case EDeviceMSPBoardType_MSP_P16_E2T6_Q288:
			{
				switch(nChannel)
				{
				case 0:
					dwLiveStatus |= 0x07;
					bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T6_Q288::ELiveModeAddress_CH1, dwLiveStatus);
					break;
				case 1:
					dwLiveStatus |= 0x07;
					bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T6_Q288::ELiveModeAddress_CH2, dwLiveStatus);
					break;
				default:
					break;
				}
			}
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
			{
				switch(nChannel)
				{
				case 0:
					dwLiveStatus |= 0x0F;
					bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EAreaLiveModeAddress_CH1, dwLiveStatus);
					break;
				case 1:
					dwLiveStatus |= 0x0F;
					bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EAreaLiveModeAddress_CH2, dwLiveStatus);
					break;
				default:
					break;
				}
			}
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2:
			{
				switch(nChannel)
				{
				case 0:
					dwLiveStatus |= 0x0F;
					bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EAreaLiveModeAddress_CH1, dwLiveStatus);
					break;
				case 1:
					dwLiveStatus |= 0x0F;
					bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EAreaLiveModeAddress_CH2, dwLiveStatus);
					break;
				default:
					break;
				}
			}
			break;
		default:
			break;
		}

		if(bReturn)
			m_pLiveStatus[nChannel] = dwLiveStatus;
	}
	while(false);

	return bReturn;
}

bool CDeviceMVTechMSPDio::DisableLive(int nChannel)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		int nChannelNumber = GetChannelNumber();

		if(nChannel < 0 || nChannel >= nChannelNumber)
			break;

		EDeviceMSPBoardType eBoardType = EDeviceMSPBoardType_Count;

		if(GetBoardType(&eBoardType))
			break;

		DWORD dwLiveStatus = m_pLiveStatus[nChannel];

		switch(eBoardType)
		{
		case EDeviceMSPBoardType_MSP_P16_E2T6_Q288:
			{
				switch(nChannel)
				{
				case 0:
					dwLiveStatus = 0x00;
					bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T6_Q288::ELiveModeAddress_CH1, dwLiveStatus);
					break;
				case 1:
					dwLiveStatus = 0x00;
					bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T6_Q288::ELiveModeAddress_CH2, dwLiveStatus);
					break;
				default:
					break;
				}
			}
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
			{
				switch(nChannel)
				{
				case 0:
					dwLiveStatus = 0x00;
					bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EAreaLiveModeAddress_CH1, dwLiveStatus);
					break;
				case 1:
					dwLiveStatus = 0x00;
					bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EAreaLiveModeAddress_CH2, dwLiveStatus);
					break;
				default:
					break;
				}
			}
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2:
			{
				switch(nChannel)
				{
				case 0:
					dwLiveStatus = 0x00;
					bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EAreaLiveModeAddress_CH1, dwLiveStatus);
					break;
				case 1:
					dwLiveStatus = 0x00;
					bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EAreaLiveModeAddress_CH2, dwLiveStatus);
					break;
				default:
					break;
				}
			}
			break;
		default:
			break;
		}

		if(bReturn)
			m_pLiveStatus[nChannel] &= dwLiveStatus;
	}
	while(false);

	return bReturn;
}

bool CDeviceMVTechMSPDio::EnableLive(int nChannel, int nSelect)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		int nChannelNumber = GetChannelNumber();

		if(nChannel < 0 || nChannel >= nChannelNumber)
			break;

		EDeviceMSPBoardType eBoardType = EDeviceMSPBoardType_Count;

		if(GetBoardType(&eBoardType))
			break;

		DWORD dwLiveStatus = m_pLiveStatus[nChannel];

		switch(eBoardType)
		{
		case EDeviceMSPBoardType_MSP_P16_E2T6_Q288:
			{
				switch(nChannel)
				{
				case 0:
					{
						switch(nSelect)
						{
						case 0:
							dwLiveStatus |= 0x01;
							bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T6_Q288::ELiveModeAddress_CH1, dwLiveStatus);
							break;
						case 1:
							dwLiveStatus |= 0x02;
							bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T6_Q288::ELiveModeAddress_CH1, dwLiveStatus);
							break;
						case 2:
							dwLiveStatus |= 0x04;
							bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T6_Q288::ELiveModeAddress_CH1, dwLiveStatus);
							break;
						default:
							break;
						}
					}
					break;
				case 1:
					{
						switch(nSelect)
						{
						case 0:
							dwLiveStatus |= 0x01;
							bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T6_Q288::ELiveModeAddress_CH2, dwLiveStatus);
							break;
						case 1:
							dwLiveStatus |= 0x02;
							bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T6_Q288::ELiveModeAddress_CH2, dwLiveStatus);
							break;
						case 2:
							dwLiveStatus |= 0x04;
							bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T6_Q288::ELiveModeAddress_CH2, dwLiveStatus);
							break;
						default:
							break;
						}
					}
					break;
				default:
					break;
				}
			}
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
			{
				switch(nChannel)
				{
				case 0:
					{
						switch(nSelect)
						{
						case 0:
							dwLiveStatus |= 0x01;
							bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EAreaLiveModeAddress_CH1, dwLiveStatus);
							break;
						case 1:
							dwLiveStatus |= 0x02;
							bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EAreaLiveModeAddress_CH1, dwLiveStatus);
							break;
						case 2:
							dwLiveStatus |= 0x04;
							bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EAreaLiveModeAddress_CH1, dwLiveStatus);
							break;
						case 3:
							dwLiveStatus |= 0x08;
							bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EAreaLiveModeAddress_CH1, dwLiveStatus);
							break;
						default:
							break;
						}
					}
					break;
				case 1:
					{
						switch(nSelect)
						{
						case 0:
							dwLiveStatus |= 0x01;
							bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EAreaLiveModeAddress_CH2, dwLiveStatus);
							break;
						case 1:
							dwLiveStatus |= 0x02;
							bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EAreaLiveModeAddress_CH2, dwLiveStatus);
							break;
						case 2:
							dwLiveStatus |= 0x04;
							bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EAreaLiveModeAddress_CH2, dwLiveStatus);
							break;
						case 3:
							dwLiveStatus |= 0x08;
							bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EAreaLiveModeAddress_CH2, dwLiveStatus);
							break;
						default:
							break;
						}
					}
					break;
				default:
					break;
				}
			}
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2:
			{
				switch(nChannel)
				{
				case 0:
					{
						switch(nSelect)
						{
						case 0:
							dwLiveStatus |= 0x01;
							bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EAreaLiveModeAddress_CH1, dwLiveStatus);
							break;
						case 1:
							dwLiveStatus |= 0x02;
							bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EAreaLiveModeAddress_CH1, dwLiveStatus);
							break;
						case 2:
							dwLiveStatus |= 0x04;
							bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EAreaLiveModeAddress_CH1, dwLiveStatus);
							break;
						case 3:
							dwLiveStatus |= 0x08;
							bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EAreaLiveModeAddress_CH1, dwLiveStatus);
							break;
						default:
							break;
						}
					}
					break;
				case 1:
					{
						switch(nSelect)
						{
						case 0:
							dwLiveStatus |= 0x01;
							bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EAreaLiveModeAddress_CH2, dwLiveStatus);
							break;
						case 1:
							dwLiveStatus |= 0x02;
							bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EAreaLiveModeAddress_CH2, dwLiveStatus);
							break;
						case 2:
							dwLiveStatus |= 0x04;
							bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EAreaLiveModeAddress_CH2, dwLiveStatus);
							break;
						case 3:
							dwLiveStatus |= 0x08;
							bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EAreaLiveModeAddress_CH2, dwLiveStatus);
							break;
						default:
							break;
						}
					}
					break;
				default:
					break;
				}
			}
			break;
		default:
			break;
		}

		if(bReturn)
			m_pLiveStatus[nChannel] = dwLiveStatus;
	}
	while(false);

	return bReturn;
}

bool CDeviceMVTechMSPDio::DisableLive(int nChannel, int nSelect)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		int nChannelNumber = GetChannelNumber();

		if(nChannel < 0 || nChannel >= nChannelNumber)
			break;

		EDeviceMSPBoardType eBoardType = EDeviceMSPBoardType_Count;

		if(GetBoardType(&eBoardType))
			break;

		DWORD dwLiveStatus = m_pLiveStatus[nChannel];

		switch(eBoardType)
		{
		case EDeviceMSPBoardType_MSP_P16_E2T6_Q288:
			{
				switch(nChannel)
				{
				case 0:
					{
						switch(nSelect)
						{
						case 0:
							dwLiveStatus &= 0x06;
							bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T6_Q288::ELiveModeAddress_CH1, 0x01);
							break;
						case 1:
							dwLiveStatus &= 0x05;
							bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T6_Q288::ELiveModeAddress_CH1, 0x02);
							break;
						case 2:
							dwLiveStatus &= 0x03;
							bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T6_Q288::ELiveModeAddress_CH1, 0x04);
							break;
						default:
							break;
						}
					}
					break;
				case 1:
					{
						switch(nSelect)
						{
						case 0:
							dwLiveStatus &= 0x06;
							bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T6_Q288::ELiveModeAddress_CH2, 0x01);
							break;
						case 1:
							dwLiveStatus &= 0x05;
							bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T6_Q288::ELiveModeAddress_CH2, 0x02);
							break;
						case 2:
							dwLiveStatus &= 0x03;
							bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T6_Q288::ELiveModeAddress_CH2, 0x04);
							break;
						default:
							break;
						}
					}
					break;
				default:
					break;
				}
			}
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
			{
				switch(nChannel)
				{
				case 0:
					{
						switch(nSelect)
						{
						case 0:
							dwLiveStatus &= 0x0E;
							bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EAreaLiveModeAddress_CH1, dwLiveStatus);
							break;
						case 1:
							dwLiveStatus &= 0x0D;
							bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EAreaLiveModeAddress_CH1, dwLiveStatus);
							break;
						case 2:
							dwLiveStatus &= 0x0B;
							bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EAreaLiveModeAddress_CH1, dwLiveStatus);
							break;
						case 3:
							dwLiveStatus &= 0x07;
							bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EAreaLiveModeAddress_CH1, dwLiveStatus);
							break;
						default:
							break;
						}
					}
					break;
				case 1:
					{
						switch(nSelect)
						{
						case 0:
							dwLiveStatus &= 0x0E;
							bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EAreaLiveModeAddress_CH2, dwLiveStatus);
							break;
						case 1:
							dwLiveStatus &= 0x0D;
							bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EAreaLiveModeAddress_CH2, dwLiveStatus);
							break;
						case 2:
							dwLiveStatus &= 0x0B;
							bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EAreaLiveModeAddress_CH2, dwLiveStatus);
							break;
						case 3:
							dwLiveStatus &= 0x07;
							bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EAreaLiveModeAddress_CH2, dwLiveStatus);
							break;
						default:
							break;
						}
					}
					break;
				default:
					break;
				}
			}
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2:
			{
				switch(nChannel)
				{
				case 0:
					{
						switch(nSelect)
						{
						case 0:
							dwLiveStatus &= 0x0E;
							bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EAreaLiveModeAddress_CH1, dwLiveStatus);
							break;
						case 1:
							dwLiveStatus &= 0x0D;
							bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EAreaLiveModeAddress_CH1, dwLiveStatus);
							break;
						case 2:
							dwLiveStatus &= 0x0B;
							bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EAreaLiveModeAddress_CH1, dwLiveStatus);
							break;
						case 3:
							dwLiveStatus &= 0x07;
							bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EAreaLiveModeAddress_CH1, dwLiveStatus);
							break;
						default:
							break;
						}
					}
					break;
				case 1:
					{
						switch(nSelect)
						{
						case 0:
							dwLiveStatus &= 0x0E;
							bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EAreaLiveModeAddress_CH2, dwLiveStatus);
							break;
						case 1:
							dwLiveStatus &= 0x0D;
							bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EAreaLiveModeAddress_CH2, dwLiveStatus);
							break;
						case 2:
							dwLiveStatus &= 0x0B;
							bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EAreaLiveModeAddress_CH2, dwLiveStatus);
							break;
						case 3:
							dwLiveStatus &= 0x07;
							bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EAreaLiveModeAddress_CH2, dwLiveStatus);
							break;
						default:
							break;
						}
					}
					break;
				default:
					break;
				}
			}
			break;
		default:
			break;
		}

		if(bReturn)
			m_pLiveStatus[nChannel] = dwLiveStatus;
	}
	while(false);

	return bReturn;
}

bool CDeviceMVTechMSPDio::EnableGrab(int nChannel)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		int nChannelNumber = GetChannelNumber();

		if(nChannel < 0 || nChannel >= nChannelNumber)
			break;

		EDeviceMSPBoardType eBoardType = EDeviceMSPBoardType_Count;

		if(GetBoardType(&eBoardType))
			break;

		switch(eBoardType)
		{
		case EDeviceMSPBoardType_MSP_P16_E2T6_Q288:
			{
				switch(nChannel)
				{
				case 0:
					bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T6_Q288::EGrabAddress_CH1, 0x07);
					break;
				case 1:
					bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T6_Q288::EGrabAddress_CH2, 0x07);
					break;
				default:
					break;
				}
			}
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
			{
				switch(nChannel)
				{
				case 0:
					bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EGrabAddress_CH1, 0x07);
					break;
				case 1:
					bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EGrabAddress_CH2, 0x07);
					break;
				default:
					break;
				}
			}
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2:
			{
				switch(nChannel)
				{
				case 0:
					bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EGrabAddress_CH1, 0x01);
					break;
				case 1:
					bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EGrabAddress_CH2, 0x01);
					break;
				default:
					break;
				}
			}
			break;
		default:
			break;
		}

	}
	while(false);

	return bReturn;
}

bool CDeviceMVTechMSPDio::EnableGrab(int nChannel, int nSelect)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(nChannel < 0 || nChannel >= GetChannelNumber())
			break;

		EDeviceMSPBoardType eBoardType = EDeviceMSPBoardType_Count;

		if(GetBoardType(&eBoardType))
			break;

		switch(eBoardType)
		{
		case EDeviceMSPBoardType_MSP_P16_E2T6_Q288:
			{
				switch(nChannel)
				{
				case 0:
					bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T6_Q288::EGrabAddress_CH1, 0x07);
					break;
				case 1:
					bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T6_Q288::EGrabAddress_CH2, 0x07);
					break;
				default:
					break;
				}
			}
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
			{
				switch(nChannel)
				{
				case 0:
					bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EGrabAddress_CH1, 0x07);
					break;
				case 1:
					bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EGrabAddress_CH2, 0x07);
					break;
				default:
					break;
				}
			}
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2:
			{
				switch(nChannel)
				{
				case 0:
					bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EGrabAddress_CH1, 0x01);
					break;
				case 1:
					bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EGrabAddress_CH2, 0x01);
					break;
				default:
					break;
				}
			}
			break;
		default:
			break;
		}

	}
	while(false);

	return bReturn;
}

bool CDeviceMVTechMSPDio::ResetDevice()
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		bReturn = true;

		for(int i = 0; i < GetChannelNumber(); ++i)
			bReturn &= ResetDevice(i);
	}
	while(false);

	return bReturn;
}

DWORD CDeviceMVTechMSPDio::GetAreaCameraCount(int nChannel)
{
	DWORD dwReturn = 0;

	do
	{
		if(!IsInitialized())
			break;

		EDeviceMSPBoardType eBoardType = EDeviceMSPBoardType_Count;

		if(GetBoardType(&eBoardType))
			break;

		switch(eBoardType)
		{
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
			{
				switch(nChannel)
				{
				case 0:
					dwReturn = PCIRead(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EAreaCameraCounterReadAddress_CH1);
					break;
				case 1:
					dwReturn = PCIRead(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EAreaCameraCounterReadAddress_CH2);
					break;
				default:
					break;
				}
			}
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2:
			{
				switch(nChannel)
				{
				case 0:
					dwReturn = PCIRead(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EAreaCameraCounterReadAddress_CH1);
					break;
				case 1:
					dwReturn = PCIRead(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EAreaCameraCounterReadAddress_CH2);
					break;
				default:
					break;
				}
			}
			break;
		default:
			break;
		}
	}
	while(false);

	return dwReturn;
}

bool CDeviceMVTechMSPDio::UsingTrigger(int nChannel, bool bUse)
{
	bool bReturn = false;

	do
	{
		if(!IsInitialized())
			break;

		if(nChannel < 0 || nChannel >= GetChannelNumber())
			break;

		EDeviceMSPBoardType eBoardType = EDeviceMSPBoardType_Count;

		if(GetBoardType(&eBoardType))
			break;

		switch(eBoardType)
		{
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
			{
				switch(nChannel)
				{
				case 0:
					bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EUsingTriggerAddress_CH1, bUse ? 0x01 : 0x00);
					break;
				case 1:
					bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EUsingTriggerAddress_CH2, bUse ? 0x01 : 0x00);
					break;
				default:
					break;
				}
			}
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2:
			{
				switch(nChannel)
				{
				case 0:
					bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EUsingTriggerAddress_CH1, bUse ? 0x01 : 0x00);
					break;
				case 1:
					bReturn = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EUsingTriggerAddress_CH2, bUse ? 0x01 : 0x00);
					break;
				default:
					break;
				}
			}
			break;
		default:
			break;
		}

	}
	while(false);

	return bReturn;
}

long CDeviceMVTechMSPDio::GetIDCount()
{
	long nCount = 0;

	do
	{
		EDeviceMSPBoardType eBoardType = EDeviceMSPBoardType_Count;

		if(GetBoardType(&eBoardType))
			break;

		if(eBoardType != EDeviceMSPBoardType_DelayedQueue)
			break;

		nCount = DelayedQueue::MAX_READ_COUNT;
	}
	while(false);

	return nCount;
}

bool CDeviceMVTechMSPDio::GetID(int nChannel, DWORD* pParam)
{
	bool bReturn = false;

	do
	{
		if(!pParam)
			break;

		if(!IsInitialized())
			break;

		if(nChannel < 0 || nChannel > 2)
			break;

		switch(nChannel)
		{
		case 0:
			(*pParam) = (PCIRead(m_pDwCS[PCI_BASE_CS0] + DelayedQueue::ID_READING_1));// >> 16;
			break;
		case 1:
			(*pParam) = (PCIRead(m_pDwCS[PCI_BASE_CS0] + DelayedQueue::ID_READING_2));// >> 16;
			break;
		case 2:
			(*pParam) = (PCIRead(m_pDwCS[PCI_BASE_CS0] + DelayedQueue::ID_READING_3));// >> 16;
			break;
		default:
			break;
		}

		bReturn = true;
	}
	while(false);

	return bReturn;
}

EMSPGetFunction CDeviceMVTechMSPDio::GetBoardType(EDeviceMSPBoardType * pParam)
{
	EMSPGetFunction eReturn = EMSPGetFunction_NullptrError;

	do
	{
		if(!pParam)
		{
			eReturn = EMSPGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterMVTechMSPTrigger_BoardType));

		if(nData < 0 || nData >= (int)EDeviceMSPBoardType_Count)
		{
			eReturn = EMSPGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceMSPBoardType)nData;

		eReturn = EMSPGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMSPSetFunction CDeviceMVTechMSPDio::SetBoardType(EDeviceMSPBoardType eParam)
{
	EMSPSetFunction eReturn = EMSPSetFunction_UnknownError;

	EDeviceParameterMVTechMSPTrigger eSaveID = EDeviceParameterMVTechMSPTrigger_BoardType;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDeviceMSPBoardType)0 || eParam >= EDeviceMSPBoardType_Count)
		{
			eReturn = EMSPSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			eReturn = EMSPSetFunction_AlreadyInitializedError;
			break;
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EMSPSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMSPSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMSPSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMSPTrigger[eSaveID], g_lpszDeviceMSPBoardType[nPreValue], g_lpszDeviceMSPBoardType[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	if(!eReturn)
	{
		DisplayParameter(eParam);

		MakeViewThreadDisplayer(pDisp)
		{
			AddControls();
			UpdateParameterList();
		};

		AddThreadDisplayer(pDisp);
	}

	return eReturn;
}

EMSPGetFunction CDeviceMVTechMSPDio::GetUIShowDio(bool* pParam)
{
	EMSPGetFunction eReturn = EMSPGetFunction_NullptrError;

	do
	{
		if(!pParam)
		{
			eReturn = EMSPGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMVTechMSPTrigger_ShowDIO));

		eReturn = EMSPGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMSPSetFunction CDeviceMVTechMSPDio::SetUIShowDio(bool bParam)
{
	EMSPSetFunction eReturn = EMSPSetFunction_UnknownError;

	EDeviceParameterMVTechMSPTrigger eSaveID = EDeviceParameterMVTechMSPTrigger_ShowDIO;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%d"), (int)bParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EMSPSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMSPSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMSPSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMSPTrigger[eSaveID], g_lpszDeviceMSPSwitch[nPreValue], g_lpszDeviceMSPSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	if(!eReturn)
	{
		MakeViewThreadDisplayer(pDisp)
		{
			AddControls();
		};

		AddThreadDisplayer(pDisp);
	}

	return eReturn;
}

EMSPGetFunction CDeviceMVTechMSPDio::GetUIShowTrgEnc(bool* pParam)
{
	EMSPGetFunction eReturn = EMSPGetFunction_NullptrError;

	do
	{
		if(!pParam)
		{
			eReturn = EMSPGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMVTechMSPTrigger_ShowTrgEnc));

		eReturn = EMSPGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMSPSetFunction CDeviceMVTechMSPDio::SetUIShowTrgEnc(bool bParam)
{
	EMSPSetFunction eReturn = EMSPSetFunction_UnknownError;

	EDeviceParameterMVTechMSPTrigger eSaveID = EDeviceParameterMVTechMSPTrigger_ShowTrgEnc;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%d"), (int)bParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EMSPSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMSPSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMSPSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMSPTrigger[eSaveID], g_lpszDeviceMSPSwitch[nPreValue], g_lpszDeviceMSPSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	if(!eReturn)
	{
		MakeViewThreadDisplayer(pDisp)
		{
			AddControls();
		};

		AddThreadDisplayer(pDisp);
	}

	return eReturn;
}

EMSPGetFunction CDeviceMVTechMSPDio::GetUIShowQueue_CH1(bool* pParam)
{
	EMSPGetFunction eReturn = EMSPGetFunction_NullptrError;

	do
	{
		if(!pParam)
		{
			eReturn = EMSPGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMVTechMSPTrigger_ShowQueueCH1));

		eReturn = EMSPGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMSPSetFunction CDeviceMVTechMSPDio::SetUIShowQueue_CH1(bool bParam)
{
	EMSPSetFunction eReturn = EMSPSetFunction_UnknownError;

	EDeviceParameterMVTechMSPTrigger eSaveID = EDeviceParameterMVTechMSPTrigger_ShowQueueCH1;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%d"), (int)bParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EMSPSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMSPSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMSPSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMSPTrigger[eSaveID], g_lpszDeviceMSPSwitch[nPreValue], g_lpszDeviceMSPSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	if(!eReturn)
	{
		MakeViewThreadDisplayer(pDisp)
		{
			AddControls();
		};

		AddThreadDisplayer(pDisp);
	}

	return eReturn;
}

EMSPGetFunction CDeviceMVTechMSPDio::GetUIShowQueue_CH2(bool* pParam)
{
	EMSPGetFunction eReturn = EMSPGetFunction_NullptrError;

	do
	{
		if(!pParam)
		{
			eReturn = EMSPGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMVTechMSPTrigger_ShowQueueCH2));

		eReturn = EMSPGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMSPSetFunction CDeviceMVTechMSPDio::SetUIShowQueue_CH2(bool bParam)
{
	EMSPSetFunction eReturn = EMSPSetFunction_UnknownError;

	EDeviceParameterMVTechMSPTrigger eSaveID = EDeviceParameterMVTechMSPTrigger_ShowQueueCH2;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%d"), (int)bParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EMSPSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMSPSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMSPSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMSPTrigger[eSaveID], g_lpszDeviceMSPSwitch[nPreValue], g_lpszDeviceMSPSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	if(!eReturn)
	{
		MakeViewThreadDisplayer(pDisp)
		{
			AddControls();
		};

		AddThreadDisplayer(pDisp);
	}

	return eReturn;
}

EMSPGetFunction CDeviceMVTechMSPDio::GetUIShowID(bool* pParam)
{
	EMSPGetFunction eReturn = EMSPGetFunction_NullptrError;

	do
	{
		if(!pParam)
		{
			eReturn = EMSPGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMVTechMSPTrigger_ShowID));

		eReturn = EMSPGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMSPSetFunction CDeviceMVTechMSPDio::SetUIShowID(bool bParam)
{
	EMSPSetFunction eReturn = EMSPSetFunction_UnknownError;

	EDeviceParameterMVTechMSPTrigger eSaveID = EDeviceParameterMVTechMSPTrigger_ShowID;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		CString strData;
		strData.Format(_T("%d"), (int)bParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EMSPSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMSPSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMSPSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMSPTrigger[eSaveID], g_lpszDeviceMSPSwitch[nPreValue], g_lpszDeviceMSPSwitch[bParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	if(!eReturn)
	{
		MakeViewThreadDisplayer(pDisp)
		{
			AddControls();
		};

		AddThreadDisplayer(pDisp);
	}

	return eReturn;
}

EMSPGetFunction CDeviceMVTechMSPDio::GetEncoderDirection_CH1(EDeviceEncoderDirection * pParam)
{
	EMSPGetFunction eReturn = EMSPGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMSPGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterMVTechMSPTrigger_EncoderDirectionCH1));

		if(nData < 0 || nData >= (int)EDeviceEncoderDirection_Count)
		{
			eReturn = EMSPGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceEncoderDirection)nData;

		eReturn = EMSPGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMSPSetFunction CDeviceMVTechMSPDio::SetEncoderDirection_CH1(EDeviceEncoderDirection eParam)
{
	EMSPSetFunction eReturn = EMSPSetFunction_UnknownError;

	EDeviceParameterMVTechMSPTrigger eSaveID = EDeviceParameterMVTechMSPTrigger_EncoderDirectionCH1;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDeviceEncoderDirection)0 || eParam >= EDeviceEncoderDirection_Count)
		{
			eReturn = EMSPSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			EDeviceMSPBoardType eBoardType = EDeviceMSPBoardType_Count;

			if(GetBoardType(&eBoardType))
			{
				eReturn = EMSPSetFunction_ReadOnDatabaseError;
				break;
			}

			bool bSetDevice = false;

			switch(eBoardType)
			{
			case EDeviceMSPBoardType_MSP_P16_E2T6_Q288:
				{
					bSetDevice = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T6_Q288::EDirectionAddress_CH1, 0x00);
					bSetDevice &= PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T6_Q288::EDirectionAddress_CH1, (DWORD)((eParam + 1) * 4));
				}
				break;
			case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
				{
					if(eParam == EDeviceEncoderDirection_Both)
						break;

					bSetDevice = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EDirectionAddress_CH1, 0x00);
					bSetDevice &= PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EDirectionAddress_CH1, (DWORD)(eParam));
				}
				break;
			case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2:
				{
					if(eParam == EDeviceEncoderDirection_Both)
						break;

					bSetDevice = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EDirectionAddress_CH1, 0x00);
					bSetDevice &= PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EDirectionAddress_CH1, (DWORD)(eParam));
				}
				break;
			default:
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EMSPSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMSPSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMSPSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMSPTrigger[eSaveID], g_lpszDeviceEncoderDirection[nPreValue], g_lpszDeviceEncoderDirection[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMSPGetFunction CDeviceMVTechMSPDio::GetEncoderDirection_CH2(EDeviceEncoderDirection * pParam)
{
	EMSPGetFunction eReturn = EMSPGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMSPGetFunction_NullptrError;
			break;
		}

		int nData = _ttoi(GetParamValue(EDeviceParameterMVTechMSPTrigger_EncoderDirectionCH2));

		if(nData < 0 || nData >= (int)EDeviceEncoderDirection_Count)
		{
			eReturn = EMSPGetFunction_EnumTypeError;
			break;
		}

		*pParam = (EDeviceEncoderDirection)nData;

		eReturn = EMSPGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMSPSetFunction CDeviceMVTechMSPDio::SetEncoderDirection_CH2(EDeviceEncoderDirection eParam)
{
	EMSPSetFunction eReturn = EMSPSetFunction_UnknownError;

	EDeviceParameterMVTechMSPTrigger eSaveID = EDeviceParameterMVTechMSPTrigger_EncoderDirectionCH2;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(eParam < (EDeviceEncoderDirection)0 || eParam >= EDeviceEncoderDirection_Count)
		{
			eReturn = EMSPSetFunction_NotFindCommandError;
			break;
		}

		if(IsInitialized())
		{
			EDeviceMSPBoardType eBoardType = EDeviceMSPBoardType_Count;

			if(GetBoardType(&eBoardType))
			{
				eReturn = EMSPSetFunction_ReadOnDatabaseError;
				break;
			}

			bool bSetDevice = false;

			switch(eBoardType)
			{
			case EDeviceMSPBoardType_MSP_P16_E2T6_Q288:
				{
					bSetDevice = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T6_Q288::EDirectionAddress_CH2, 0x00);
					bSetDevice &= PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T6_Q288::EDirectionAddress_CH2, (DWORD)((eParam + 1) * 4));
				}
				break;
			case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
				{
					if(eParam == EDeviceEncoderDirection_Both)
						break;

					bSetDevice = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EDirectionAddress_CH2, 0x00);
					bSetDevice &= PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EDirectionAddress_CH2, (DWORD)(eParam));
				}
				break;
			case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2:
				{
					if(eParam == EDeviceEncoderDirection_Both)
						break;

					bSetDevice = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EDirectionAddress_CH2, 0x00);
					bSetDevice &= PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EDirectionAddress_CH2, (DWORD)(eParam));
				}
				break;
			default:
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), (int)eParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EMSPSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMSPSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMSPSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMSPTrigger[eSaveID], g_lpszDeviceEncoderDirection[nPreValue], g_lpszDeviceEncoderDirection[eParam]);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMSPGetFunction CDeviceMVTechMSPDio::GetStartPosition_CH1(int * pParam)
{
	EMSPGetFunction eReturn = EMSPGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMSPGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMVTechMSPTrigger_StartPositionCH1));

		eReturn = EMSPGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMSPSetFunction CDeviceMVTechMSPDio::SetStartPosition_CH1(int nParam)
{
	EMSPSetFunction eReturn = EMSPSetFunction_UnknownError;

	EDeviceParameterMVTechMSPTrigger eSaveID = EDeviceParameterMVTechMSPTrigger_StartPositionCH1;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			EDeviceMSPBoardType eBoardType = EDeviceMSPBoardType_Count;

			if(GetBoardType(&eBoardType))
				break;

			bool bSetDevice = false;

			switch(eBoardType)
			{
			case EDeviceMSPBoardType_MSP_P16_E2T6_Q288:
				bSetDevice = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T6_Q288::EEncoderStartAddress_CH1, (DWORD)nParam);
				break;
			case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
				bSetDevice = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EAreaCameraCounterWriteAddress_CH1, (DWORD)nParam);
				break;
			case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2:
				bSetDevice = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EAreaCameraCounterWriteAddress_CH1, (DWORD)nParam);
				break;
			default:
				break;
			}

			if(!bSetDevice)
			{
				eReturn = EMSPSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EMSPSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMSPSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMSPSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMSPTrigger[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMSPGetFunction CDeviceMVTechMSPDio::GetStartPosition_CH2(int * pParam)
{
	EMSPGetFunction eReturn = EMSPGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMSPGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMVTechMSPTrigger_StartPositionCH2));

		eReturn = EMSPGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMSPSetFunction CDeviceMVTechMSPDio::SetStartPosition_CH2(int nParam)
{
	EMSPSetFunction eReturn = EMSPSetFunction_UnknownError;

	EDeviceParameterMVTechMSPTrigger eSaveID = EDeviceParameterMVTechMSPTrigger_StartPositionCH2;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			EDeviceMSPBoardType eBoardType = EDeviceMSPBoardType_Count;

			if(GetBoardType(&eBoardType))
				break;

			bool bSetDevice = false;

			switch(eBoardType)
			{
			case EDeviceMSPBoardType_MSP_P16_E2T6_Q288:
				bSetDevice = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T6_Q288::EEncoderStartAddress_CH2, (DWORD)nParam);
				break;
			case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
				bSetDevice = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EAreaCameraCounterWriteAddress_CH2, (DWORD)nParam);
				break;
			case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2:
				bSetDevice = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EAreaCameraCounterWriteAddress_CH2, (DWORD)nParam);
				break;
			default:
				break;
			}

			if(!bSetDevice)
			{
				eReturn = EMSPSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EMSPSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMSPSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMSPSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMSPTrigger[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMSPGetFunction CDeviceMVTechMSPDio::GetEndPosition_CH1(int * pParam)
{
	EMSPGetFunction eReturn = EMSPGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMSPGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMVTechMSPTrigger_EndPositionCH1));

		eReturn = EMSPGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMSPSetFunction CDeviceMVTechMSPDio::SetEndPosition_CH1(int nParam)
{
	EMSPSetFunction eReturn = EMSPSetFunction_UnknownError;

	EDeviceParameterMVTechMSPTrigger eSaveID = EDeviceParameterMVTechMSPTrigger_EndPositionCH1;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			EDeviceMSPBoardType eBoardType = EDeviceMSPBoardType_Count;

			if(GetBoardType(&eBoardType))
				break;

			bool bSetDevice = false;

			switch(eBoardType)
			{
			case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
				bSetDevice = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EEncoderEndAddress_CH1, (DWORD)nParam);
				break;
			case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2:
				bSetDevice = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EEncoderEndAddress_CH1, (DWORD)nParam);
				break;
			default:
				break;
			}

			if(!bSetDevice)
			{
				eReturn = EMSPSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EMSPSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMSPSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMSPSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMSPTrigger[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMSPGetFunction CDeviceMVTechMSPDio::GetEndPosition_CH2(int * pParam)
{
	EMSPGetFunction eReturn = EMSPGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMSPGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMVTechMSPTrigger_EndPositionCH2));

		eReturn = EMSPGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMSPSetFunction CDeviceMVTechMSPDio::SetEndPosition_CH2(int nParam)
{
	EMSPSetFunction eReturn = EMSPSetFunction_UnknownError;

	EDeviceParameterMVTechMSPTrigger eSaveID = EDeviceParameterMVTechMSPTrigger_EndPositionCH2;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			EDeviceMSPBoardType eBoardType = EDeviceMSPBoardType_Count;

			if(GetBoardType(&eBoardType))
				break;

			bool bSetDevice = false;

			switch(eBoardType)
			{
			case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
				bSetDevice = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EEncoderEndAddress_CH2, (DWORD)nParam);
				break;
			case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2:
				bSetDevice = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EEncoderEndAddress_CH2, (DWORD)nParam);
				break;
			default:
				break;
			}

			if(!bSetDevice)
			{
				eReturn = EMSPSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EMSPSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMSPSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMSPSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMSPTrigger[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMSPGetFunction CDeviceMVTechMSPDio::GetTriggerOnTime_CH1(int* pParam)
{
	EMSPGetFunction eReturn = EMSPGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMSPGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMVTechMSPTrigger_TriggerOnTimeCH1));

		eReturn = EMSPGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMSPSetFunction CDeviceMVTechMSPDio::SetTriggerOnTime_CH1(int nParam)
{
	EMSPSetFunction eReturn = EMSPSetFunction_UnknownError;

	EDeviceParameterMVTechMSPTrigger eSaveID = EDeviceParameterMVTechMSPTrigger_TriggerOnTimeCH1;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			int nCycle = 0;

			if(GetTriggerCycle_CH1(&nCycle))
				break;

			EDeviceMSPBoardType eBoardType = EDeviceMSPBoardType_Count;

			if(GetBoardType(&eBoardType))
				break;

			bool bSetDevice = false;

			switch(eBoardType)
			{
			case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
				{
					DWORD dwOntime = ((nParam & 0xFF) << 8) + (nCycle & 0xFF);
					bSetDevice = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::ETriggerCycleAddress_CH1, dwOntime);
				}
				break;
			case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2:
				{
					DWORD dwOntime = ((nParam & 0xFF) << 8) + (nCycle & 0xFF);
					bSetDevice = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::ETriggerCycleAddress_CH1, dwOntime);
				}
				break;
			default:
				break;
			}

			if(!bSetDevice)
			{
				eReturn = EMSPSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EMSPSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMSPSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMSPSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMSPTrigger[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMSPGetFunction CDeviceMVTechMSPDio::GetTriggerOnTime_CH2(int* pParam)
{
	EMSPGetFunction eReturn = EMSPGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMSPGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMVTechMSPTrigger_TriggerOnTimeCH2));

		eReturn = EMSPGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMSPSetFunction CDeviceMVTechMSPDio::SetTriggerOnTime_CH2(int nParam)
{
	EMSPSetFunction eReturn = EMSPSetFunction_UnknownError;

	EDeviceParameterMVTechMSPTrigger eSaveID = EDeviceParameterMVTechMSPTrigger_TriggerOnTimeCH2;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			int nCycle = 0;

			if(GetTriggerCycle_CH2(&nCycle))
				break;

			EDeviceMSPBoardType eBoardType = EDeviceMSPBoardType_Count;

			if(GetBoardType(&eBoardType))
				break;

			bool bSetDevice = false;

			switch(eBoardType)
			{
			case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
				{
					DWORD dwOntime = ((nParam & 0xFF) << 8) + (nCycle & 0xFF);
					bSetDevice = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::ETriggerCycleAddress_CH2, dwOntime);
				}
				break;
			case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2:
				{
					DWORD dwOntime = ((nParam & 0xFF) << 8) + (nCycle & 0xFF);
					bSetDevice = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::ETriggerCycleAddress_CH2, dwOntime);
				}
				break;
			default:
				break;
			}

			if(!bSetDevice)
			{
				eReturn = EMSPSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EMSPSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMSPSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMSPSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMSPTrigger[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMSPGetFunction CDeviceMVTechMSPDio::GetTriggerCycle_CH1(int* pParam)
{
	EMSPGetFunction eReturn = EMSPGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMSPGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMVTechMSPTrigger_TriggerCycleCH1));

		eReturn = EMSPGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMSPSetFunction CDeviceMVTechMSPDio::SetTriggerCycle_CH1(int nParam)
{
	EMSPSetFunction eReturn = EMSPSetFunction_UnknownError;

	EDeviceParameterMVTechMSPTrigger eSaveID = EDeviceParameterMVTechMSPTrigger_TriggerCycleCH1;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			int nOnTime = 0;

			if(GetTriggerOnTime_CH1(&nOnTime))
				break;

			EDeviceMSPBoardType eBoardType = EDeviceMSPBoardType_Count;

			if(GetBoardType(&eBoardType))
				break;

			bool bSetDevice = false;

			switch(eBoardType)
			{
			case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
				{
					DWORD dwOntime = ((nOnTime & 0xFF) << 8) + (nParam & 0xFF);
					bSetDevice = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::ETriggerCycleAddress_CH1, dwOntime);
				}
				break;
			case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2:
				{
					DWORD dwOntime = ((nOnTime & 0xFF) << 8) + (nParam & 0xFF);
					bSetDevice = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::ETriggerCycleAddress_CH1, dwOntime);
				}
				break;
			default:
				break;
			}

			if(!bSetDevice)
			{
				eReturn = EMSPSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EMSPSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMSPSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMSPSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMSPTrigger[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMSPGetFunction CDeviceMVTechMSPDio::GetTriggerCycle_CH2(int* pParam)
{
	EMSPGetFunction eReturn = EMSPGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMSPGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMVTechMSPTrigger_TriggerCycleCH2));

		eReturn = EMSPGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMSPSetFunction CDeviceMVTechMSPDio::SetTriggerCycle_CH2(int nParam)
{
	EMSPSetFunction eReturn = EMSPSetFunction_UnknownError;

	EDeviceParameterMVTechMSPTrigger eSaveID = EDeviceParameterMVTechMSPTrigger_TriggerCycleCH2;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			int nOnTime = 0;

			if(GetTriggerOnTime_CH2(&nOnTime))
				break;

			EDeviceMSPBoardType eBoardType = EDeviceMSPBoardType_Count;

			if(GetBoardType(&eBoardType))
				break;

			bool bSetDevice = false;

			switch(eBoardType)
			{
			case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
				{
					DWORD dwOntime = ((nOnTime & 0xFF) << 8) + (nParam & 0xFF);
					bSetDevice = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::ETriggerCycleAddress_CH2, dwOntime);
				}
				break;
			case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2:
				{
					DWORD dwOntime = ((nOnTime & 0xFF) << 8) + (nParam & 0xFF);
					bSetDevice = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::ETriggerCycleAddress_CH2, dwOntime);
				}
				break;
			default:
				break;
			}

			if(!bSetDevice)
			{
				eReturn = EMSPSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EMSPSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMSPSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMSPSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMSPTrigger[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMSPGetFunction CDeviceMVTechMSPDio::GetStrobeTime_CH1(int * pParam)
{
	EMSPGetFunction eReturn = EMSPGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMSPGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMVTechMSPTrigger_StrobeTimeCH1));

		eReturn = EMSPGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMSPSetFunction CDeviceMVTechMSPDio::SetStrobeTime_CH1(int nParam)
{
	EMSPSetFunction eReturn = EMSPSetFunction_UnknownError;

	EDeviceParameterMVTechMSPTrigger eSaveID = EDeviceParameterMVTechMSPTrigger_StrobeTimeCH1;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			int nOnTime = 0;

			if(GetTriggerOnTime_CH2(&nOnTime))
				break;

			EDeviceMSPBoardType eBoardType = EDeviceMSPBoardType_Count;

			if(GetBoardType(&eBoardType))
				break;

			bool bSetDevice = false;

			switch(eBoardType)
			{
			case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
				bSetDevice = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EStrobeExposerTimeAddress_CH1, (DWORD)nParam);
				break;
			case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2:
				bSetDevice = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EStrobeExposerTimeAddress_CH1, (DWORD)nParam);
				break;
			default:
				break;
			}

			if(!bSetDevice)
			{
				eReturn = EMSPSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EMSPSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMSPSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMSPSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMSPTrigger[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

EMSPGetFunction CDeviceMVTechMSPDio::GetStrobeTime_CH2(int * pParam)
{
	EMSPGetFunction eReturn = EMSPGetFunction_UnknownError;

	do
	{
		if(!pParam)
		{
			eReturn = EMSPGetFunction_NullptrError;
			break;
		}

		*pParam = _ttoi(GetParamValue(EDeviceParameterMVTechMSPTrigger_StrobeTimeCH2));

		eReturn = EMSPGetFunction_OK;
	}
	while(false);

	return eReturn;
}

EMSPSetFunction CDeviceMVTechMSPDio::SetStrobeTime_CH2(int nParam)
{
	EMSPSetFunction eReturn = EMSPSetFunction_UnknownError;

	EDeviceParameterMVTechMSPTrigger eSaveID = EDeviceParameterMVTechMSPTrigger_StrobeTimeCH2;

	int nPreValue = _ttoi(GetParamValue(eSaveID));

	do
	{
		if(IsInitialized())
		{
			int nOnTime = 0;

			if(GetTriggerOnTime_CH2(&nOnTime))
				break;

			EDeviceMSPBoardType eBoardType = EDeviceMSPBoardType_Count;

			if(GetBoardType(&eBoardType))
				break;

			bool bSetDevice = false;

			switch(eBoardType)
			{
			case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
				bSetDevice = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_1::EStrobeExposerTimeAddress_CH2, (DWORD)nParam);
				break;
			case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2:
				bSetDevice = PCIWrite(m_pDwCS[PCI_BASE_CS0] + MSP_P16_E2T2_Q16_2::EStrobeExposerTimeAddress_CH2, (DWORD)nParam);
				break;
			default:
				break;
			}

			if(!bSetDevice)
			{
				eReturn = EMSPSetFunction_WriteToDeviceError;
				break;
			}
		}

		CString strData;
		strData.Format(_T("%d"), nParam);

		if(!SetParamValue(eSaveID, strData))
		{
			eReturn = EMSPSetFunction_WriteToDatabaseError;
			break;
		}

		if(!SaveSettings(eSaveID))
		{
			eReturn = EMSPSetFunction_WriteToDatabaseError;
			break;
		}

		eReturn = EMSPSetFunction_OK;
	}
	while(false);

	CString strMessage;

	strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_d_to_d), !eReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMSPTrigger[eSaveID], nPreValue, nParam);

	CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

	SetMessage(strMessage);

	return eReturn;
}

bool CDeviceMVTechMSPDio::OnParameterChanged(int nParam, CString strValue)
{
	bool bReturn = false;

	bool bFoundID = true;

	CString strMessage;

	do
	{
		switch(nParam)
		{
		case EDeviceParameterMVTechMSPTrigger_DeviceID:
			bReturn = !SetDeviceID(strValue);
			break;
		case EDeviceParameterMVTechMSPTrigger_BoardType:
			bReturn = !SetBoardType((EDeviceMSPBoardType)_ttoi(strValue));
			break;
		case EDeviceParameterMVTechMSPTrigger_ShowDIO:
			bReturn = !SetUIShowDio(_ttoi(strValue));
			break;
		case EDeviceParameterMVTechMSPTrigger_ShowTrgEnc:
			bReturn = !SetUIShowTrgEnc(_ttoi(strValue));
			break;
		case EDeviceParameterMVTechMSPTrigger_ShowQueueCH1:
			bReturn = !SetUIShowQueue_CH1(_ttoi(strValue));
			break;
		case EDeviceParameterMVTechMSPTrigger_ShowQueueCH2:
			bReturn = !SetUIShowQueue_CH2(_ttoi(strValue));
			break;
		case EDeviceParameterMVTechMSPTrigger_ShowID:
			bReturn = !SetUIShowID(_ttoi(strValue));
			break;
		case EDeviceParameterMVTechMSPTrigger_EncoderDirectionCH1:
			bReturn = !SetEncoderDirection_CH1((EDeviceEncoderDirection)_ttoi(strValue));
			break;
		case EDeviceParameterMVTechMSPTrigger_EncoderDirectionCH2:
			bReturn = !SetEncoderDirection_CH2((EDeviceEncoderDirection)_ttoi(strValue));
			break;
		case EDeviceParameterMVTechMSPTrigger_TriggerOnTimeCH1:
			bReturn = !SetTriggerOnTime_CH1(_ttoi(strValue));
			break;
		case EDeviceParameterMVTechMSPTrigger_TriggerOnTimeCH2:
			bReturn = !SetTriggerOnTime_CH2(_ttoi(strValue));
			break;
		case EDeviceParameterMVTechMSPTrigger_TriggerCycleCH1:
			bReturn = !SetTriggerCycle_CH1(_ttoi(strValue));
			break;
		case EDeviceParameterMVTechMSPTrigger_TriggerCycleCH2:
			bReturn = !SetTriggerCycle_CH2(_ttoi(strValue));
			break;
		case EDeviceParameterMVTechMSPTrigger_StartPositionCH1:
			bReturn = !SetStartPosition_CH1(_ttoi(strValue));
			break;
		case EDeviceParameterMVTechMSPTrigger_EndPositionCH1:
			bReturn = !SetEndPosition_CH1(_ttoi(strValue));
			break;
		case EDeviceParameterMVTechMSPTrigger_StartPositionCH2:
			bReturn = !SetStartPosition_CH2(_ttoi(strValue));
			break;
		case EDeviceParameterMVTechMSPTrigger_EndPositionCH2:
			bReturn = !SetEndPosition_CH2(_ttoi(strValue));
			break;
		case EDeviceParameterMVTechMSPTrigger_StrobeTimeCH1:
			bReturn = !SetStrobeTime_CH1(_ttoi(strValue));
			break;
		case EDeviceParameterMVTechMSPTrigger_StrobeTimeCH2:
			bReturn = !SetStrobeTime_CH2(_ttoi(strValue));
			break;
		case EDeviceParameterMVTechMSPTrigger_UseInputNo0:
		case EDeviceParameterMVTechMSPTrigger_UseInputNo1:
		case EDeviceParameterMVTechMSPTrigger_UseInputNo2:
		case EDeviceParameterMVTechMSPTrigger_UseInputNo3:
		case EDeviceParameterMVTechMSPTrigger_UseInputNo4:
		case EDeviceParameterMVTechMSPTrigger_UseInputNo5:
		case EDeviceParameterMVTechMSPTrigger_UseInputNo6:
		case EDeviceParameterMVTechMSPTrigger_UseInputNo7:
		case EDeviceParameterMVTechMSPTrigger_UseInputNo8:
		case EDeviceParameterMVTechMSPTrigger_UseInputNo9:
		case EDeviceParameterMVTechMSPTrigger_UseInputNo10:
		case EDeviceParameterMVTechMSPTrigger_UseInputNo11:
		case EDeviceParameterMVTechMSPTrigger_UseInputNo12:
		case EDeviceParameterMVTechMSPTrigger_UseInputNo13:
		case EDeviceParameterMVTechMSPTrigger_UseInputNo14:
		case EDeviceParameterMVTechMSPTrigger_UseInputNo15:
			{
				int nPreValue = _ttoi(GetParamValue(nParam));
				int nValue = _ttoi(strValue);

				if(SetParamValue(nParam, strValue))
				{
					m_pUsedInput[nParam - EDeviceParameterMVTechMSPTrigger_UseInputNo0] = nValue;

					bReturn = SaveSettings(nParam);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMSPTrigger[nParam], g_lpszDeviceMSPSwitch[nPreValue], g_lpszDeviceMSPSwitch[nValue]);

				if(bReturn)
				{
					MakeViewThreadDisplayer(pDisp)
					{
						AddControls();
					};

					AddThreadDisplayer(pDisp);
				}
			}
			break;
		case EDeviceParameterMVTechMSPTrigger_UseOutputNo0:
		case EDeviceParameterMVTechMSPTrigger_UseOutputNo1:
		case EDeviceParameterMVTechMSPTrigger_UseOutputNo2:
		case EDeviceParameterMVTechMSPTrigger_UseOutputNo3:
		case EDeviceParameterMVTechMSPTrigger_UseOutputNo4:
		case EDeviceParameterMVTechMSPTrigger_UseOutputNo5:
		case EDeviceParameterMVTechMSPTrigger_UseOutputNo6:
		case EDeviceParameterMVTechMSPTrigger_UseOutputNo7:
		case EDeviceParameterMVTechMSPTrigger_UseOutputNo8:
		case EDeviceParameterMVTechMSPTrigger_UseOutputNo9:
		case EDeviceParameterMVTechMSPTrigger_UseOutputNo10:
		case EDeviceParameterMVTechMSPTrigger_UseOutputNo11:
		case EDeviceParameterMVTechMSPTrigger_UseOutputNo12:
		case EDeviceParameterMVTechMSPTrigger_UseOutputNo13:
		case EDeviceParameterMVTechMSPTrigger_UseOutputNo14:
		case EDeviceParameterMVTechMSPTrigger_UseOutputNo15:
			{
				int nPreValue = _ttoi(GetParamValue(nParam));
				int nValue = _ttoi(strValue);

				if(SetParamValue(nParam, strValue))
				{
					m_pUsedOutput[nParam - EDeviceParameterMVTechMSPTrigger_UseOutputNo0] = nValue;

					bReturn = SaveSettings(nParam);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMSPTrigger[nParam], g_lpszDeviceMSPSwitch[nPreValue], g_lpszDeviceMSPSwitch[nValue]);

				if(bReturn)
				{
					MakeViewThreadDisplayer(pDisp)
					{
						AddControls();
					};

					AddThreadDisplayer(pDisp);
				}
			}
			break;
		case EDeviceParameterMVTechMSPTrigger_InputNo0Name:
		case EDeviceParameterMVTechMSPTrigger_InputNo1Name:
		case EDeviceParameterMVTechMSPTrigger_InputNo2Name:
		case EDeviceParameterMVTechMSPTrigger_InputNo3Name:
		case EDeviceParameterMVTechMSPTrigger_InputNo4Name:
		case EDeviceParameterMVTechMSPTrigger_InputNo5Name:
		case EDeviceParameterMVTechMSPTrigger_InputNo6Name:
		case EDeviceParameterMVTechMSPTrigger_InputNo7Name:
		case EDeviceParameterMVTechMSPTrigger_InputNo8Name:
		case EDeviceParameterMVTechMSPTrigger_InputNo9Name:
		case EDeviceParameterMVTechMSPTrigger_InputNo10Name:
		case EDeviceParameterMVTechMSPTrigger_InputNo11Name:
		case EDeviceParameterMVTechMSPTrigger_InputNo12Name:
		case EDeviceParameterMVTechMSPTrigger_InputNo13Name:
		case EDeviceParameterMVTechMSPTrigger_InputNo14Name:
		case EDeviceParameterMVTechMSPTrigger_InputNo15Name:
			{
				CString strPreValue = GetParamValue(nParam);

				if(SetParamValue(nParam, strValue))
				{
					int nIndex = nParam - EDeviceParameterMVTechMSPTrigger_InputNo0Name;

					m_pStrInputName[nIndex] = strValue;

					bReturn = SaveSettings(nParam);

					CString strSave;
					strSave.Format(_T("%d : %s"), nIndex, strValue);

					SetDlgItemTextW(EDeviceAddEnumeratedControlID_InputLedLabelStart + nIndex, strSave);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMSPTrigger[nParam], strPreValue, strValue);

				bFoundID = false;
			}
			break;
		case EDeviceParameterMVTechMSPTrigger_OutputNo0Name:
		case EDeviceParameterMVTechMSPTrigger_OutputNo1Name:
		case EDeviceParameterMVTechMSPTrigger_OutputNo2Name:
		case EDeviceParameterMVTechMSPTrigger_OutputNo3Name:
		case EDeviceParameterMVTechMSPTrigger_OutputNo4Name:
		case EDeviceParameterMVTechMSPTrigger_OutputNo5Name:
		case EDeviceParameterMVTechMSPTrigger_OutputNo6Name:
		case EDeviceParameterMVTechMSPTrigger_OutputNo7Name:
		case EDeviceParameterMVTechMSPTrigger_OutputNo8Name:
		case EDeviceParameterMVTechMSPTrigger_OutputNo9Name:
		case EDeviceParameterMVTechMSPTrigger_OutputNo10Name:
		case EDeviceParameterMVTechMSPTrigger_OutputNo11Name:
		case EDeviceParameterMVTechMSPTrigger_OutputNo12Name:
		case EDeviceParameterMVTechMSPTrigger_OutputNo13Name:
		case EDeviceParameterMVTechMSPTrigger_OutputNo14Name:
		case EDeviceParameterMVTechMSPTrigger_OutputNo15Name:
			{
				CString strPreValue = GetParamValue(nParam);

				if(SetParamValue(nParam, strValue))
				{
					int nIndex = nParam - EDeviceParameterMVTechMSPTrigger_OutputNo0Name;

					m_pStrOutputName[nIndex] = strValue;

					bReturn = SaveSettings(nParam);

					CString strSave;
					strSave.Format(_T("%d : %s"), nIndex, strValue);

					SetDlgItemTextW(EDeviceAddEnumeratedControlID_OutputLedLabelStart + nIndex, strSave);
				}

				strMessage.Format(CMultiLanguageManager::GetString(ELanguageParameter__s_tochangetheparameter_s_from_s_to_s), bReturn ? CMultiLanguageManager::GetString(ELanguageParameter_succeed) : CMultiLanguageManager::GetString(ELanguageParameter_failed), g_lpszDeviceMSPTrigger[nParam], strPreValue, strValue);

				bFoundID = false;
			}
			break;
		default:
			strMessage = CMultiLanguageManager::GetString(ELanguageParameter_Failedtofindtheparameter);
			break;
		}
	}
	while(false);

	if(!bFoundID)
	{
		CLogManager::Write(EFrameworkLogTypes_Device, _T("[%s,%d] %s"), GetClassNameStr(), GetObjectID(), strMessage);

		SetMessage(strMessage);
	}

	return bReturn;
}

bool CDeviceMVTechMSPDio::Display_P16_E2T6_Q288()
{
	bool bReturn = false;

	long* pEncoderCounts = nullptr;
	long* pTriggerCounts = nullptr;
	DWORD* pQueueStatus = nullptr;

	do
	{
		bool bShowIO = true;
		bool bShowTrgEnc = true;
		bool bShowQueueCh1 = true;
		bool bShowQueueCh2 = true;

		GetUIShowDio(&bShowIO);
		GetUIShowTrgEnc(&bShowTrgEnc);
		GetUIShowQueue_CH1(&bShowQueueCh1);
		GetUIShowQueue_CH2(&bShowQueueCh2);

		int nChannel = GetChannelNumber();
		int nQueueNumber = GetQueueNumber();
		int nInportNumber = GetInputPortNumber();
		int nOutportNumber = GetOutputPortNumber();
		int nEncoderNumber = GetEncoderPortNumber();
		int nTriggerNumber = GetTriggerPortNumber();
		int nQueueStatusCount = GetQueueStatusPortNumber();
		int nTotalQueueStatus = nChannel * nQueueStatusCount;
		int nTotalQueue = nChannel * nQueueNumber;

		for(int i = 0; i < nInportNumber; ++i)
		{
			CRavidLedCtrl* pLed = (CRavidLedCtrl*)GetDlgItem(EDeviceAddEnumeratedControlID_InputLedStart + i);

			if(pLed)
			{
				pLed->SetActivate(IsInitialized());

				if(ReadInBit(i))
					pLed->On();
				else
					pLed->Off();
			}
		}

		for(int i = 0; i < nOutportNumber; ++i)
		{
			CRavidLedCtrl* pLed = (CRavidLedCtrl*)GetDlgItem(EDeviceAddEnumeratedControlID_OutputLedStart + i);

			if(pLed)
			{
				pLed->SetActivate(IsInitialized());

				if(ReadOutBit(i))
					pLed->On();
				else
					pLed->Off();
			}
		}

		int nTotalEncoders = nChannel * nEncoderNumber;
		int nTotalTriggers = nChannel * nTriggerNumber;

		pEncoderCounts = new long[nTotalEncoders];
		memset(pEncoderCounts, 0, sizeof(long) * nTotalEncoders);

		pTriggerCounts = new long[nTotalTriggers];
		memset(pTriggerCounts, 0, sizeof(long) * nTotalTriggers);


		for(int j = 0; j < nChannel; ++j)
		{
			int nTriggerIndex = j * nTriggerNumber;

			for(int i = 0; i < nTriggerNumber; ++i)
				pTriggerCounts[nTriggerIndex + i] = GetTriggerCount(j, i);

			pEncoderCounts[j] = GetEncoderCount(j);
		}

		float fDuration = m_pRpc->GetElapsedTime();
		m_pRpc->Start();

		for(int j = 0; j < nChannel; ++j)
		{
			int nEncoderIndex = j * nEncoderNumber;

			for(int i = 0; i < nEncoderNumber; ++i)
			{
				int nIDIndex = nEncoderIndex + i;

				CRavidLedCtrl* pLedEncoder = (CRavidLedCtrl*)GetDlgItem(EDeviceAddEnumeratedControlID_EncLedStart + nIDIndex);

				if(pLedEncoder)
					pLedEncoder->SetActivate(IsInitialized());

				CWnd* pWnd = GetDlgItem(EDeviceAddEnumeratedControlID_EncCaptionStart + nIDIndex);

				if(pWnd)
				{
					long nEncoderDiff = pEncoderCounts[nIDIndex] - m_pPreEncoderValue[nIDIndex];
					m_pPreEncoderValue[nIDIndex] = pEncoderCounts[nIDIndex];

					CString strCounter;
					strCounter.Format(_T("%d (%.03f KHz)"), pEncoderCounts[nIDIndex], nEncoderDiff / fDuration);

					CString strOrigCounter;
					pWnd->GetWindowText(strOrigCounter);

					if(strCounter.Compare(strOrigCounter))
					{
						pWnd->SetWindowText(strCounter);

						if(pLedEncoder)
							pLedEncoder->On();
					}
					else
					{
						if(pLedEncoder)
							pLedEncoder->Off();
					}
				}
			}
		}

		for(int j = 0; j < nChannel; ++j)
		{
			int nTriggerIndex = j * nTriggerNumber;

			for(int i = 0; i < nTriggerNumber; ++i)
			{
				int nIDIndex = nTriggerIndex + i;

				CRavidLedCtrl* pLedTrigger = (CRavidLedCtrl*)GetDlgItem(EDeviceAddEnumeratedControlID_TrgLedStart + nIDIndex);

				if(pLedTrigger)
					pLedTrigger->SetActivate(IsInitialized());


				CWnd* pWnd = GetDlgItem(EDeviceAddEnumeratedControlID_TrgCaptionStart + nIDIndex);

				if(pWnd)
				{
					long nTriggerDiff = pTriggerCounts[nIDIndex] - m_pPreTriggerValue[nIDIndex];
					m_pPreTriggerValue[nIDIndex] = pTriggerCounts[nIDIndex];

					CString strCounter;
					strCounter.Format(_T("%d (%.03f KHz)"), pTriggerCounts[nIDIndex], nTriggerDiff / fDuration);

					CString strOrigCounter;

					pWnd->GetWindowText(strOrigCounter);

					if(strCounter.Compare(strOrigCounter))
					{
						pWnd->SetWindowText(strCounter);

						if(pLedTrigger)
							pLedTrigger->On();
					}
					else
					{
						if(pLedTrigger)
							pLedTrigger->Off();
					}
				}
			}
		}

		pQueueStatus = new DWORD[nTotalQueueStatus];
		memset(pQueueStatus, 0, sizeof(DWORD) * nTotalQueueStatus);

		for(int j = 0; j < nChannel; ++j)
		{
			int nQueueIndex = j * nQueueStatusCount;

			for(int i = 0; i < nQueueStatusCount; ++i)
				pQueueStatus[nQueueIndex + i] = GetQueueStatus(j, i);
		}

		if(bShowQueueCh1)
		{
			for(int i = 0; i < nQueueNumber; ++i)
			{
				CRavidLedCtrl* pLedTrigger = (CRavidLedCtrl*)GetDlgItem(EDeviceAddEnumeratedControlID_QueueLedStart + i);

				if(pLedTrigger)
				{
					pLedTrigger->SetActivate(IsInitialized());

					int nQueueIndex = i / 24;

					int nQueuePos = i % 24;

					bool bQueueStatus = (pQueueStatus[nQueueIndex] >> nQueuePos) & 0x01;

					if(bQueueStatus)
						pLedTrigger->On();
					else
						pLedTrigger->Off();

					CStatic* pLabel = (CStatic*)GetDlgItem(EDeviceAddEnumeratedControlID_QueueLedLabelStart + i);

					if(pLabel)
					{
						CString strPreQueue;
						pLabel->GetWindowTextW(strPreQueue);

						CString strQueue;
						strQueue.Format(_T("%d"), *(m_pQueueValueCh1 + i));

						if(!bQueueStatus)
						{
							*(m_pQueueValueCh1 + i) = 0;
							if(strPreQueue != _T("0"))
								pLabel->SetWindowTextW(_T("0"));
						}
						else
						{
							if(strPreQueue != strQueue)
								pLabel->SetWindowTextW(strQueue);
						}
					}
				}
			}
		}

		if(bShowQueueCh2)
		{
			for(int i = nQueueNumber; i < nTotalQueue; ++i)
			{
				CRavidLedCtrl* pLedTrigger = (CRavidLedCtrl*)GetDlgItem(EDeviceAddEnumeratedControlID_QueueLedStart + i);

				if(pLedTrigger)
				{
					pLedTrigger->SetActivate(IsInitialized());

					int nQueueIndex = i / 24;

					int nQueuePos = i % 24;

					bool bQueueStatus = (pQueueStatus[nQueueIndex] >> nQueuePos) & 0x01;

					if(bQueueStatus)
						pLedTrigger->On();
					else
						pLedTrigger->Off();

					CStatic* pLabel = (CStatic*)GetDlgItem(EDeviceAddEnumeratedControlID_QueueLedLabelStart + i);

					if(pLabel)
					{
						CString strPreQueue;
						pLabel->GetWindowTextW(strPreQueue);

						CString strQueue;
						strQueue.Format(_T("%d"), *(m_pQueueValueCh2 + i - nQueueNumber));

						if(!bQueueStatus)
						{
							*(m_pQueueValueCh2 + i - nQueueNumber) = 0;
							if(strPreQueue != _T("0"))
								pLabel->SetWindowTextW(_T("0"));
						}
						else
						{
							if(strPreQueue != strQueue)
								pLabel->SetWindowTextW(strQueue);
						}
					}
				}
			}
		}

		bReturn = true;
	}
	while(false);

	if(pEncoderCounts)
		delete[] pEncoderCounts;
	pEncoderCounts = nullptr;

	if(pTriggerCounts)
		delete[] pTriggerCounts;
	pTriggerCounts = nullptr;

	if(pQueueStatus)
		delete[] pQueueStatus;
	pQueueStatus = nullptr;

	return bReturn;
}

bool CDeviceMVTechMSPDio::Display_P16_E2T2_Q16()
{
	bool bReturn = false;

	long* pEncoderCounts = nullptr;
	long* pTriggerCounts = nullptr;
	DWORD* pQueueStatus = nullptr;

	do
	{
		bool bShowIO = true;
		bool bShowTrgEnc = true;
		bool bShowQueueCh1 = true;
		bool bShowQueueCh2 = true;

		GetUIShowDio(&bShowIO);
		GetUIShowTrgEnc(&bShowTrgEnc);
		GetUIShowQueue_CH1(&bShowQueueCh1);
		GetUIShowQueue_CH2(&bShowQueueCh2);

		int nChannel = GetChannelNumber();
		int nQueueNumber = GetQueueNumber();
		int nInportNumber = GetInputPortNumber();
		int nOutportNumber = GetOutputPortNumber();
		int nEncoderNumber = GetEncoderPortNumber();
		int nTriggerNumber = GetTriggerPortNumber();
		int nQueueStatusCount = GetQueueStatusPortNumber();
		int nTotalQueueStatus = nChannel * nQueueStatusCount;
		int nTotalQueue = nChannel * nQueueNumber;

		for(int i = 0; i < nInportNumber; ++i)
		{
			CRavidLedCtrl* pLed = (CRavidLedCtrl*)GetDlgItem(EDeviceAddEnumeratedControlID_InputLedStart + i);

			if(pLed)
			{
				pLed->SetActivate(IsInitialized());

				if(ReadInBit(i))
					pLed->On();
				else
					pLed->Off();
			}
		}

		for(int i = 0; i < nOutportNumber; ++i)
		{
			CRavidLedCtrl* pLed = (CRavidLedCtrl*)GetDlgItem(EDeviceAddEnumeratedControlID_OutputLedStart + i);

			if(pLed)
			{
				pLed->SetActivate(IsInitialized());

				if(ReadOutBit(i))
					pLed->On();
				else
					pLed->Off();
			}
		}

		int nTotalEncoders = nChannel * nEncoderNumber;
		int nTotalTriggers = nChannel * nTriggerNumber;

		pEncoderCounts = new long[nTotalEncoders];
		memset(pEncoderCounts, 0, sizeof(long) * nTotalEncoders);

		pTriggerCounts = new long[nTotalTriggers];
		memset(pTriggerCounts, 0, sizeof(long) * nTotalTriggers);

		for(int j = 0; j < nChannel; ++j)
		{
			int nTriggerIndex = j * nTriggerNumber;

			for(int i = 0; i < nTriggerNumber; ++i)
				pTriggerCounts[nTriggerIndex + i] = GetTriggerCount(j, i);

			pEncoderCounts[j] = GetEncoderCount(j);
		}

		float fDuration = m_pRpc->GetElapsedTime();
		m_pRpc->Start();

		for(int j = 0; j < nChannel; ++j)
		{
			int nEncoderIndex = j * nEncoderNumber;

			for(int i = 0; i < nEncoderNumber; ++i)
			{
				int nIDIndex = nEncoderIndex + i;

				CRavidLedCtrl* pLedEncoder = (CRavidLedCtrl*)GetDlgItem(EDeviceAddEnumeratedControlID_EncLedStart + nIDIndex);

				if(pLedEncoder)
					pLedEncoder->SetActivate(IsInitialized());

				CWnd* pWnd = GetDlgItem(EDeviceAddEnumeratedControlID_EncCaptionStart + nIDIndex);

				if(pWnd)
				{
					long nEncoderDiff = pEncoderCounts[nIDIndex] - m_pPreEncoderValue[nIDIndex];
					m_pPreEncoderValue[nIDIndex] = pEncoderCounts[nIDIndex];

					CString strCounter;
					strCounter.Format(_T("%d (%.03f KHz)"), pEncoderCounts[nIDIndex], nEncoderDiff / fDuration);

					CString strOrigCounter;
					pWnd->GetWindowText(strOrigCounter);

					if(strCounter.Compare(strOrigCounter))
					{
						pWnd->SetWindowText(strCounter);

						if(pLedEncoder)
							pLedEncoder->On();
					}
					else
					{
						if(pLedEncoder)
							pLedEncoder->Off();
					}
				}
			}
		}

		for(int j = 0; j < nChannel; ++j)
		{
			int nTriggerIndex = j * nTriggerNumber;

			for(int i = 0; i < nTriggerNumber; ++i)
			{
				int nIDIndex = nTriggerIndex + i;

				CRavidLedCtrl* pLedTrigger = (CRavidLedCtrl*)GetDlgItem(EDeviceAddEnumeratedControlID_TrgLedStart + nIDIndex);

				if(pLedTrigger)
					pLedTrigger->SetActivate(IsInitialized());


				CWnd* pWnd = GetDlgItem(EDeviceAddEnumeratedControlID_TrgCaptionStart + nIDIndex);

				if(pWnd)
				{
					long nTriggerDiff = pTriggerCounts[nIDIndex] - m_pPreTriggerValue[nIDIndex];
					m_pPreTriggerValue[nIDIndex] = pTriggerCounts[nIDIndex];

					CString strCounter;
					strCounter.Format(_T("%d (%.03f KHz)"), pTriggerCounts[nIDIndex], nTriggerDiff / fDuration);

					CString strOrigCounter;

					pWnd->GetWindowText(strOrigCounter);

					if(strCounter.Compare(strOrigCounter))
					{
						pWnd->SetWindowText(strCounter);

						if(pLedTrigger)
							pLedTrigger->On();
					}
					else
					{
						if(pLedTrigger)
							pLedTrigger->Off();
					}
				}
			}
		}

		pQueueStatus = new DWORD[nTotalQueue];
		memset(pQueueStatus, 0, sizeof(DWORD) * nTotalQueue);

		for(int j = 0; j < nChannel; ++j)
		{
			int nQueueIndex = j * nQueueStatusCount;

			for(int i = 0; i < nQueueStatusCount; ++i)
				pQueueStatus[nQueueIndex + i] = GetQueueStatus(j, i);
		}

		if(bShowQueueCh1)
		{
			for(int i = 0; i < nQueueNumber; ++i)
			{
				CRavidLedCtrl* pLedTrigger = (CRavidLedCtrl*)GetDlgItem(EDeviceAddEnumeratedControlID_QueueLedStart + i);

				if(pLedTrigger)
				{
					pLedTrigger->SetActivate(IsInitialized());

					bool bQueueStatus = (pQueueStatus[0] >> i) & 0x01;

					if(bQueueStatus)
						pLedTrigger->On();
					else
						pLedTrigger->Off();

					CStatic* pLabel = (CStatic*)GetDlgItem(EDeviceAddEnumeratedControlID_QueueLedLabelStart + i);

					if(pLabel)
					{
						CString strPreQueue;
						pLabel->GetWindowTextW(strPreQueue);

						CString strQueue;
						strQueue.Format(_T("%d"), *(m_pQueueValueCh1 + i));

						if(!bQueueStatus)
						{
							*(m_pQueueValueCh1 + i) = 0;
							if(strPreQueue != _T("0"))
								pLabel->SetWindowTextW(_T("0"));
						}
						else
						{
							if(strPreQueue != strQueue)
								pLabel->SetWindowTextW(strQueue);
						}
					}
				}
			}
		}

		if(bShowQueueCh2)
		{
			for(int i = nQueueNumber; i < nTotalQueue; ++i)
			{
				CRavidLedCtrl* pLedTrigger = (CRavidLedCtrl*)GetDlgItem(EDeviceAddEnumeratedControlID_QueueLedStart + i);

				if(pLedTrigger)
				{
					pLedTrigger->SetActivate(IsInitialized());

					bool bQueueStatus = (pQueueStatus[1] >> (i - nQueueNumber)) & 0x01;

					if(bQueueStatus)
						pLedTrigger->On();
					else
						pLedTrigger->Off();

					CStatic* pLabel = (CStatic*)GetDlgItem(EDeviceAddEnumeratedControlID_QueueLedLabelStart + i);

					if(pLabel)
					{
						CString strPreQueue;
						pLabel->GetWindowTextW(strPreQueue);

						CString strQueue;
						strQueue.Format(_T("%d"), *(m_pQueueValueCh2 + i - nQueueNumber));

						if(!bQueueStatus)
						{
							*(m_pQueueValueCh2 + i - nQueueNumber) = 0;
							if(strPreQueue != _T("0"))
								pLabel->SetWindowTextW(_T("0"));
						}
						else
						{
							if(strPreQueue != strQueue)
								pLabel->SetWindowTextW(strQueue);
						}
					}
				}
			}
		}

		bReturn = true;
	}
	while(false);

	if(pEncoderCounts)
		delete[] pEncoderCounts;
	pEncoderCounts = nullptr;

	if(pTriggerCounts)
		delete[] pTriggerCounts;
	pTriggerCounts = nullptr;

	if(pQueueStatus)
		delete[] pQueueStatus;
	pQueueStatus = nullptr;

	return bReturn;
}

bool CDeviceMVTechMSPDio::Display_DelayedQueue()
{
	bool bReturn = false;

	do
	{
		bool bShowID = true;

		GetUIShowID(&bShowID);

		long nCount = GetIDCount();

		float fDuration = m_pRpc->GetElapsedTime();
		m_pRpc->Start();

		if(bShowID)
		{
			if(IsInitialized())
			{
				for(int i = 0; i < nCount; ++i)
				{
					CStatic* pLabel = (CStatic*)GetDlgItem(EDeviceAddEnumeratedControlID_TriggerIDStart + i);

					if(pLabel)
					{
						DWORD dwData = -1;

						GetID(i, &dwData);

						CString strPreParam;
						pLabel->GetWindowTextW(strPreParam);

						CString strData;
						strData.Format(_T("%lu"), dwData);

						if(strPreParam != strData)
							pLabel->SetWindowTextW(strData);
					}
				}
			}
		}

		bReturn = true;
	}
	while(false);

	return bReturn;
}

void CDeviceMVTechMSPDio::OnTimer(UINT_PTR nIDEvent)
{
	if(IsWindowVisible())
	{
		if(nIDEvent == RAVID_TIMER_DIO)
		{
			EDeviceMSPBoardType eBoardType = (EDeviceMSPBoardType)_ttoi(m_vctParameterFieldConfigurations[EDeviceParameterMVTechMSPTrigger_BoardType].strParameterValue);

			switch(eBoardType)
			{
			case EDeviceMSPBoardType_MSP_P16_E2T6_Q288:
				{
					Display_P16_E2T6_Q288();
				}
				break;
			case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
			case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2:
				{
					Display_P16_E2T2_Q16();
				}
				break;
			case EDeviceMSPBoardType_DelayedQueue:
				{
					Display_DelayedQueue();
				}
				break;
			default:
				break;
			}
		}
	}

	CDeviceDio::OnTimer(nIDEvent);
}

bool CDeviceMVTechMSPDio::DoesModuleExist()
{
	bool bReturn = false;

	CString strModuleName;

	do
	{
#ifdef _WIN64
		strModuleName.Format(_T("PlxApi650_x64.dll"));
#else
		strModuleName.Format(_T("PlxApi650.dll"));
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

bool CDeviceMVTechMSPDio::AddControls()
{
	bool bReturn = false;
	CDC* pDC = nullptr;
	CFont* pOldFont = nullptr;

	LockWindowUpdate();

	do
	{
		pDC = GetDC();

		if(!pDC)
			break;

		for(auto iter = m_vctCtrl.begin(); iter != m_vctCtrl.end(); ++iter)
			delete *iter;

		m_vctCtrl.clear();

		CWnd* pWndParamTree = GetDlgItem(int32_t(RavidInterfaceControlID::DeviceSheetParameter));
		if(!pWndParamTree)
			break;

		CWnd* pWndInitBtn = GetDlgItem(int32_t(RavidInterfaceControlID::DeviceButtonInitialize));
		if(!pWndInitBtn)
			break;

		CWnd* pWndTerBtn = GetDlgItem(int32_t(RavidInterfaceControlID::DeviceButtonTerminate));
		if(!pWndTerBtn)
			break;

		CFont* pBaseFont = pWndInitBtn->GetFont();
		if(!pBaseFont)
			break;

		double dblDPIScale = DPISCALE;

		pOldFont = pDC->SelectObject(pBaseFont);

		CRect rrDlg, rrSheet, rrInitializeBtn, rrTerminateBtn;

		GetClientRect(rrDlg);

		pWndParamTree->GetWindowRect(rrSheet);
		ScreenToClient(rrSheet);

		pWndInitBtn->GetWindowRect(rrInitializeBtn);
		ScreenToClient(rrInitializeBtn);

		pWndTerBtn->GetWindowRect(rrTerminateBtn);
		ScreenToClient(rrTerminateBtn);

		bool bShowID = true;
		bool bShowIO = true;
		bool bShowTrgEnc = true;
		bool bShowQueueCh1 = true;
		bool bShowQueueCh2 = true;

		GetUIShowID(&bShowID);
		GetUIShowDio(&bShowIO);
		GetUIShowTrgEnc(&bShowTrgEnc);
		GetUIShowQueue_CH1(&bShowQueueCh1);
		GetUIShowQueue_CH2(&bShowQueueCh2);

		int nGroupBoxVerticalMargin = 25 * dblDPIScale;
		int nDialogEdgeMargin = 10 * dblDPIScale;
		int nButtonInterval = 5 * dblDPIScale;
		int nInfoTextHeight = 20 * dblDPIScale;
		int nInfomationRows = 2 * dblDPIScale;

		CRavidPoint<int> rpSheetLeftTop(rrSheet.right + nDialogEdgeMargin, rrInitializeBtn.top - nButtonInterval);
		CRavidPoint<int> rpSheetRightBottom(rpSheetLeftTop.x + rrSheet.Width(), rpSheetLeftTop.y + nDialogEdgeMargin);

		CRavidRect<int> rrDIOGroup;
		CRavidRect<int> rrTriggerGroup;
		CRavidRect<int> rrQueueGroup1;
		CRavidRect<int> rrQueueGroup2;
		CRavidRect<int> rrBtn;

		EDeviceMSPBoardType eBoardType = EDeviceMSPBoardType_Count;

		GetBoardType(&eBoardType);

		switch(eBoardType)
		{
		case EDeviceMSPBoardType_MSP_P16_E2T6_Q288:
			{
				if(bShowIO)
				{
					AddIOs(dblDPIScale, pBaseFont, rpSheetLeftTop, rpSheetRightBottom, &rrDIOGroup);

					rpSheetLeftTop.SetPoint(rrDIOGroup.left, rrDIOGroup.bottom + nDialogEdgeMargin);
					rpSheetRightBottom.SetPoint(rrDIOGroup.right, rpSheetLeftTop.y + nDialogEdgeMargin);
				}
				else
				{
					rpSheetLeftTop.SetPoint(rrSheet.right + nDialogEdgeMargin, rrInitializeBtn.top - nButtonInterval);
					rpSheetRightBottom.SetPoint(rpSheetLeftTop.x + rrSheet.Width(), rpSheetLeftTop.y + nDialogEdgeMargin);
				}

				if(bShowTrgEnc)
				{
					AddTriggers(dblDPIScale, pBaseFont, rpSheetLeftTop, rpSheetRightBottom, &rrTriggerGroup);

					rpSheetLeftTop.SetPoint(rrTriggerGroup.left, rrTriggerGroup.bottom + nDialogEdgeMargin);
					rpSheetRightBottom.SetPoint(rrTriggerGroup.right, rpSheetLeftTop.y + rrInitializeBtn.Height());

					AddButton(dblDPIScale, pBaseFont, rpSheetLeftTop, rpSheetRightBottom, &rrBtn);

					rpSheetLeftTop.SetPoint(rrTriggerGroup.right + nDialogEdgeMargin, rrInitializeBtn.top - nButtonInterval);
					rpSheetRightBottom.SetPoint(rpSheetLeftTop.x + rrSheet.Width(), rpSheetLeftTop.y + nDialogEdgeMargin);

				}
				else
				{
					if(bShowIO)
					{
						rpSheetLeftTop.SetPoint(rrDIOGroup.right + nDialogEdgeMargin, rrInitializeBtn.top - nButtonInterval);
						rpSheetRightBottom.SetPoint(rpSheetLeftTop.x + rrSheet.Width(), rpSheetLeftTop.y + nDialogEdgeMargin);
					}
				}

				if(bShowQueueCh1)
				{
					AddQueues(dblDPIScale, 0, pBaseFont, rpSheetLeftTop, rpSheetRightBottom, &rrQueueGroup1);

					rpSheetLeftTop.SetPoint(rrQueueGroup1.right, rrInitializeBtn.top - nButtonInterval);
					rpSheetRightBottom.SetPoint(rpSheetLeftTop.x + rrSheet.Width(), rpSheetLeftTop.y + nDialogEdgeMargin);
				}

				if(bShowQueueCh2)
				{
					if(GetChannelNumber() != 1)
						AddQueues(dblDPIScale, GetQueueNumber(), pBaseFont, rpSheetLeftTop, rpSheetRightBottom, &rrQueueGroup2);
				}

				rrDlg.bottom = __max(__max(__max(__max(__max(rrDIOGroup.bottom, rrQueueGroup1.bottom), rrQueueGroup2.bottom), rrTriggerGroup.bottom), rrBtn.bottom), 500) + nDialogEdgeMargin;

				rrDlg.right = __max(__max(__max(__max(__max(rrDIOGroup.right, rrQueueGroup1.right), rrQueueGroup2.right), rrTriggerGroup.right), rrBtn.right), rrSheet.right) + nDialogEdgeMargin;

				pWndParamTree->SetWindowPos(nullptr, 0, 0, rrSheet.Width(), rrDlg.bottom - (rrSheet.top - rrDIOGroup.top + nButtonInterval + nDialogEdgeMargin), SWP_NOMOVE);
			}
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
			{
				if(bShowIO)
				{
					AddIOs(dblDPIScale, pBaseFont, rpSheetLeftTop, rpSheetRightBottom, &rrDIOGroup);

					rpSheetLeftTop.SetPoint(rrDIOGroup.left, rrDIOGroup.bottom + nDialogEdgeMargin);
					rpSheetRightBottom.SetPoint(rrDIOGroup.right, rpSheetLeftTop.y + nDialogEdgeMargin);
				}
				else
				{
					rpSheetLeftTop.SetPoint(rrSheet.right + nDialogEdgeMargin, rrInitializeBtn.top - nButtonInterval);
					rpSheetRightBottom.SetPoint(rpSheetLeftTop.x + rrSheet.Width(), rpSheetLeftTop.y + nDialogEdgeMargin);
				}

				if(bShowQueueCh1)
				{
					AddQueues(dblDPIScale, 0, pBaseFont, rpSheetLeftTop, rpSheetRightBottom, &rrQueueGroup1);

					rpSheetLeftTop.SetPoint(rrQueueGroup1.left, rrQueueGroup1.bottom + nDialogEdgeMargin);
					rpSheetRightBottom.SetPoint(rrQueueGroup1.right, rpSheetLeftTop.y + nDialogEdgeMargin);
				}

				if(bShowQueueCh2)
				{
					if(GetChannelNumber() == 2)
					{
						AddQueues(dblDPIScale, GetQueueNumber(), pBaseFont, rpSheetLeftTop, rpSheetRightBottom, &rrQueueGroup2);

						rpSheetLeftTop.SetPoint(rrQueueGroup2.left, rrQueueGroup2.bottom + nDialogEdgeMargin);
						rpSheetRightBottom.SetPoint(rrQueueGroup2.right, rpSheetLeftTop.y + nDialogEdgeMargin);
					}
				}

				if(bShowTrgEnc)
				{
					AddTriggers(dblDPIScale, pBaseFont, rpSheetLeftTop, rpSheetRightBottom, &rrTriggerGroup);

					rpSheetLeftTop.SetPoint(rrTriggerGroup.left, rrTriggerGroup.bottom + nDialogEdgeMargin);
					rpSheetRightBottom.SetPoint(rrTriggerGroup.right, rpSheetLeftTop.y + rrInitializeBtn.Height());

					AddButton(dblDPIScale, pBaseFont, rpSheetLeftTop, rpSheetRightBottom, &rrBtn);

					rpSheetLeftTop.SetPoint(rrTriggerGroup.right + nDialogEdgeMargin, rrInitializeBtn.top - nButtonInterval);
					rpSheetRightBottom.SetPoint(rpSheetLeftTop.x + rrSheet.Width(), rpSheetLeftTop.y + nDialogEdgeMargin);
				}

				rrDlg.bottom = __max(__max(__max(__max(__max(rrDIOGroup.bottom, rrQueueGroup1.bottom), rrQueueGroup2.bottom), rrTriggerGroup.bottom), rrBtn.bottom), 500) + nDialogEdgeMargin;

				rrDlg.right = __max(__max(__max(__max(__max(rrDIOGroup.right, rrQueueGroup1.right), rrQueueGroup2.right), rrTriggerGroup.right), rrBtn.right), rrSheet.right) + nDialogEdgeMargin;

				pWndParamTree->SetWindowPos(nullptr, 0, 0, rrSheet.Width(), rrDlg.bottom - (rrSheet.top - rrDIOGroup.top + nButtonInterval + nDialogEdgeMargin), SWP_NOMOVE);
			}
			break;
		case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2:
			{
				if(1)
				{
					if(bShowIO)
					{
						AddIOsNotOutput3(dblDPIScale, pBaseFont, rpSheetLeftTop, rpSheetRightBottom, &rrDIOGroup);

						rpSheetLeftTop.SetPoint(rrDIOGroup.left, rrDIOGroup.bottom + nDialogEdgeMargin);
						rpSheetRightBottom.SetPoint(rrDIOGroup.right, rpSheetLeftTop.y + nDialogEdgeMargin);
					}
					else
					{
						rpSheetLeftTop.SetPoint(rrSheet.right + nDialogEdgeMargin, rrInitializeBtn.top - nButtonInterval);
						rpSheetRightBottom.SetPoint(rpSheetLeftTop.x + rrSheet.Width(), rpSheetLeftTop.y + nDialogEdgeMargin);
					}

					if(bShowQueueCh1)
					{
						AddQueues(dblDPIScale, 0, pBaseFont, rpSheetLeftTop, rpSheetRightBottom, &rrQueueGroup1);

						rpSheetLeftTop.SetPoint(rrQueueGroup1.left, rrQueueGroup1.bottom + nDialogEdgeMargin);
						rpSheetRightBottom.SetPoint(rrQueueGroup1.right, rpSheetLeftTop.y + nDialogEdgeMargin);
					}

					if(bShowQueueCh2)
					{
						if(GetChannelNumber() == 2)
						{
							AddQueues(dblDPIScale, GetQueueNumber(), pBaseFont, rpSheetLeftTop, rpSheetRightBottom, &rrQueueGroup2);

							rpSheetLeftTop.SetPoint(rrQueueGroup2.left, rrQueueGroup2.bottom + nDialogEdgeMargin);
							rpSheetRightBottom.SetPoint(rrQueueGroup2.right, rpSheetLeftTop.y + nDialogEdgeMargin);
						}
					}

					if(bShowTrgEnc)
					{
						AddTriggers(dblDPIScale, pBaseFont, rpSheetLeftTop, rpSheetRightBottom, &rrTriggerGroup);

						rpSheetLeftTop.SetPoint(rrTriggerGroup.left, rrTriggerGroup.bottom + nDialogEdgeMargin);
						rpSheetRightBottom.SetPoint(rrTriggerGroup.right, rpSheetLeftTop.y + rrInitializeBtn.Height());

						AddButton(dblDPIScale, pBaseFont, rpSheetLeftTop, rpSheetRightBottom, &rrBtn);

						rpSheetLeftTop.SetPoint(rrTriggerGroup.right + nDialogEdgeMargin, rrInitializeBtn.top - nButtonInterval);
						rpSheetRightBottom.SetPoint(rpSheetLeftTop.x + rrSheet.Width(), rpSheetLeftTop.y + nDialogEdgeMargin);
					}

					rrDlg.bottom = __max(__max(__max(__max(__max(rrDIOGroup.bottom, rrQueueGroup1.bottom), rrQueueGroup2.bottom), rrTriggerGroup.bottom), rrBtn.bottom), 500) + nDialogEdgeMargin;

					rrDlg.right = __max(__max(__max(__max(__max(rrDIOGroup.right, rrQueueGroup1.right), rrQueueGroup2.right), rrTriggerGroup.right), rrBtn.right), rrSheet.right) + nDialogEdgeMargin;

					pWndParamTree->SetWindowPos(nullptr, 0, 0, rrSheet.Width(), rrDlg.bottom - (rrSheet.top - rrDIOGroup.top + nButtonInterval + nDialogEdgeMargin), SWP_NOMOVE);
				}



				if(0)
				{
					if(bShowIO)
					{
						AddIOsNotOutput1(dblDPIScale, pBaseFont, rpSheetLeftTop, rpSheetRightBottom, &rrDIOGroup);

						rpSheetLeftTop.SetPoint(rrDIOGroup.right, rpSheetLeftTop.y);
						rpSheetRightBottom.SetPoint(rrDIOGroup.right + rrSheet.Width() / 2, rpSheetLeftTop.y + nDialogEdgeMargin);
					}
					else
					{
						rpSheetLeftTop.SetPoint(rrSheet.right + nDialogEdgeMargin, rrInitializeBtn.top - nButtonInterval);
						rpSheetRightBottom.SetPoint(rpSheetLeftTop.x + rrSheet.Width(), rpSheetLeftTop.y + nDialogEdgeMargin);
					}

					if(bShowQueueCh1)
					{
						if(!bShowIO)
							AddQueues(dblDPIScale, 0, pBaseFont, rpSheetLeftTop, rpSheetRightBottom, &rrQueueGroup1);
						else
							AddQueuesNotOutput(dblDPIScale, 0, pBaseFont, rpSheetLeftTop, rpSheetRightBottom, &rrQueueGroup1);

						rpSheetLeftTop.SetPoint(rrQueueGroup1.left, rrQueueGroup1.bottom + nDialogEdgeMargin);
						rpSheetRightBottom.SetPoint(rrQueueGroup1.right, rpSheetLeftTop.y + nDialogEdgeMargin);
					}

					if(bShowQueueCh2)
					{
						if(GetChannelNumber() == 2)
						{
							if(!bShowIO)
								AddQueues(dblDPIScale, GetQueueNumber(), pBaseFont, rpSheetLeftTop, rpSheetRightBottom, &rrQueueGroup2);
							else
								AddQueuesNotOutput(dblDPIScale, GetQueueNumber(), pBaseFont, rpSheetLeftTop, rpSheetRightBottom, &rrQueueGroup2);

							rpSheetLeftTop.SetPoint(rrQueueGroup2.left, rrQueueGroup2.bottom + nDialogEdgeMargin);
							rpSheetRightBottom.SetPoint(rrQueueGroup2.right, rpSheetLeftTop.y + nDialogEdgeMargin);
						}
					}

					if(bShowTrgEnc)
					{
						rpSheetLeftTop.SetPoint(rrSheet.right + nDialogEdgeMargin, rrInitializeBtn.top - nButtonInterval);
						rpSheetRightBottom.SetPoint(rpSheetLeftTop.x + rrSheet.Width(), rpSheetLeftTop.y + nDialogEdgeMargin);

						if(bShowIO)
						{
							rpSheetLeftTop.y = __max(__max(rrDIOGroup.bottom, rrQueueGroup1.bottom), rrQueueGroup2.bottom) + nDialogEdgeMargin;
							rpSheetRightBottom.y = rpSheetLeftTop.y + nDialogEdgeMargin;
						}

						AddTriggers(dblDPIScale, pBaseFont, rpSheetLeftTop, rpSheetRightBottom, &rrTriggerGroup);

						rpSheetLeftTop.SetPoint(rrTriggerGroup.left, rrTriggerGroup.bottom + nDialogEdgeMargin);
						rpSheetRightBottom.SetPoint(rrTriggerGroup.right, rpSheetLeftTop.y + rrInitializeBtn.Height());

						AddButton(dblDPIScale, pBaseFont, rpSheetLeftTop, rpSheetRightBottom, &rrBtn);

						rpSheetLeftTop.SetPoint(rrTriggerGroup.right + nDialogEdgeMargin, rrInitializeBtn.top - nButtonInterval);
						rpSheetRightBottom.SetPoint(rpSheetLeftTop.x + rrSheet.Width(), rpSheetLeftTop.y + nDialogEdgeMargin);
					}
				}
			}
			break;
			case EDeviceMSPBoardType_DelayedQueue:
				{
					rpSheetLeftTop.SetPoint(rrSheet.left, 500 + nDialogEdgeMargin);
					rpSheetRightBottom.SetPoint(rpSheetLeftTop.x + rrSheet.Width(), rpSheetLeftTop.y + nDialogEdgeMargin);

					if(bShowID)
						AddIDs(dblDPIScale, pBaseFont, rpSheetLeftTop, rpSheetRightBottom, &rrDIOGroup);

					rrDlg.bottom = __max(500 + nDialogEdgeMargin, rrDIOGroup.bottom);

					rrDlg.right = __max(__max(__max(__max(__max(rrDIOGroup.right, rrQueueGroup1.right), rrQueueGroup2.right), rrTriggerGroup.right), rrBtn.right), rrSheet.right) + nDialogEdgeMargin;

					pWndParamTree->SetWindowPos(nullptr, 0, 0, rrSheet.Width(), rrDlg.Height() - (rrSheet.top - rrDlg.top) - rrDIOGroup.GetHeight() - nDialogEdgeMargin, SWP_NOMOVE);
				}
				break;
		default:
			break;
		}

		RecalcSheet();

		SetDefaultDialogRect(rrDlg);

		bReturn = true;
	}
	while(false);

	UnlockWindowUpdate();

	if(pDC)
	{
		pDC->SelectObject(pOldFont);
		ReleaseDC(pDC);
	}

	return bReturn;
}

bool CDeviceMVTechMSPDio::AddIOs(double dblDPIScale, CFont * pBaseFont, CRavidPoint<int> cpLeftTopPt, CRavidPoint<int> cpRightBottomPt, CRavidRect<int>* pResultGroupBox)
{
	bool bReturn = false;

	do
	{
		CRavidRect<int> rrGroupBox[2];

		rrGroupBox[0].SetRect(cpLeftTopPt.x, cpLeftTopPt.y, (cpLeftTopPt.x + cpRightBottomPt.x) / 2., cpRightBottomPt.y + 100 * dblDPIScale);
		rrGroupBox[1].SetRect((cpLeftTopPt.x + cpRightBottomPt.x) / 2., cpLeftTopPt.y, cpRightBottomPt.x, cpRightBottomPt.y + 100 * dblDPIScale);

		CRavidPoint<int> rpLedBasePos(rrGroupBox[0].left + 10 * dblDPIScale, rrGroupBox[0].top + 20 * dblDPIScale);
		CRavidRect<int> rrLedRect(rpLedBasePos.x, rpLedBasePos.y, rpLedBasePos.x + 15 * dblDPIScale, rpLedBasePos.y + 15 * dblDPIScale);
		CRavidPoint<int> rpLedYGap(0, 25 * dblDPIScale);
		CRavidPoint<int> rpLedXGap(20 * dblDPIScale, 0);

		int nInputLedCreatedCount = 0;
		int nOutputLedCreatedCount = 0;

		int nRealPos = 0;

		long nIOPort = GetInputPortNumber();

		for(int i = 0; i < nIOPort; ++i)
		{
			if(!m_pUsedInput[i])
				continue;

			CRavidLedCtrl* pLed = new CRavidLedCtrl;
			pLed->Create(nullptr, WS_VISIBLE | WS_CHILD | SS_NOTIFY, rrLedRect + rpLedYGap * nRealPos, this, EDeviceAddEnumeratedControlID_InputLedStart + i);
			pLed->SetFont(pBaseFont);
			m_vctCtrl.push_back(pLed);

			CStatic* pLabel = new CStatic;
			CRavidRect<int> rrLabelPosition(rrLedRect + rpLedYGap * nRealPos);
			rrLabelPosition += rpLedXGap;
			rrLabelPosition.right = rrGroupBox[0].right - 10 * dblDPIScale;

			CString strName;
			strName.Format(_T("%d : %s"), i, m_pStrInputName[i]);
			pLabel->Create(strName, WS_VISIBLE | WS_CHILD | SS_NOTIFY, rrLabelPosition, this, EDeviceAddEnumeratedControlID_InputLedLabelStart + i);
			pLabel->SetFont(pBaseFont);
			m_vctCtrl.push_back(pLabel);

			++nRealPos;
		}

		nInputLedCreatedCount = nRealPos;

		rpLedBasePos.SetPoint(rrGroupBox[1].left + 10 * dblDPIScale, rrGroupBox[1].top + 20 * dblDPIScale);
		rrLedRect.SetRect(rpLedBasePos.x, rpLedBasePos.y, rpLedBasePos.x + 15 * dblDPIScale, rpLedBasePos.y + 15 * dblDPIScale);

		nRealPos = 0;
		nIOPort = GetOutputPortNumber();

		for(int i = 0; i < nIOPort; ++i)
		{
			if(!m_pUsedOutput[i])
				continue;

			CRavidLedCtrl* pLed = new CRavidLedCtrl;
			pLed->Create(nullptr, WS_VISIBLE | WS_CHILD | SS_NOTIFY, rrLedRect + rpLedYGap * nRealPos, this, EDeviceAddEnumeratedControlID_OutputLedStart + i);
			pLed->SetFont(pBaseFont);
			m_vctCtrl.push_back(pLed);

			CStatic* pLabel = new CStatic;
			CRavidRect<int> rrLabelPosition(rrLedRect + rpLedYGap * nRealPos);
			rrLabelPosition += rpLedXGap;
			rrLabelPosition.right = rrGroupBox[1].right - 10 * dblDPIScale;
			CString strName;
			strName.Format(_T("%d : %s"), i, m_pStrOutputName[i]);
			pLabel->Create(strName, WS_VISIBLE | WS_CHILD | SS_NOTIFY, rrLabelPosition, this, EDeviceAddEnumeratedControlID_OutputLedLabelStart + i);
			pLabel->SetFont(pBaseFont);
			m_vctCtrl.push_back(pLabel);

			++nRealPos;
		}

		nOutputLedCreatedCount = nRealPos;

		rrGroupBox[0].bottom = (rrLedRect + rpLedYGap * (__max(nInputLedCreatedCount, nOutputLedCreatedCount) - 1)).bottom + 10 * dblDPIScale;
		rrGroupBox[1].bottom = (rrLedRect + rpLedYGap * (__max(nInputLedCreatedCount, nOutputLedCreatedCount) - 1)).bottom + 10 * dblDPIScale;

		for(int i = 0; i < 2; ++i)
		{
			CButton* pGroupBox = new CButton;

			pGroupBox->Create(i ? CMultiLanguageManager::GetString(ELanguageParameter_Output) : CMultiLanguageManager::GetString(ELanguageParameter_Input), BS_GROUPBOX | WS_CHILD | WS_VISIBLE, rrGroupBox[i], this, EDeviceAddEnumeratedControlID_StaticStart + i);
			pGroupBox->SetFont(pBaseFont);
			m_vctCtrl.push_back(pGroupBox);
		}

		(*pResultGroupBox).left = rrGroupBox[0].left;
		(*pResultGroupBox).top = rrGroupBox[0].top;
		(*pResultGroupBox).right = rrGroupBox[1].right;
		(*pResultGroupBox).bottom = rrGroupBox[1].bottom;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceMVTechMSPDio::AddIOsNotOutput1(double dblDPIScale, CFont * pBaseFont, CRavidPoint<int> cpLeftTopPt, CRavidPoint<int> cpRightBottomPt, CRavidRect<int>* pResultGroupBox)
{
	bool bReturn = false;

	do
	{
		CRavidRect<int> rrGroupBox;

		rrGroupBox.SetRect(cpLeftTopPt.x, cpLeftTopPt.y, (cpLeftTopPt.x + cpRightBottomPt.x) / 2., cpRightBottomPt.y + 100 * dblDPIScale);

		CRavidPoint<int> rpLedBasePos(rrGroupBox[0].left + 10 * dblDPIScale, rrGroupBox[0].top + 20 * dblDPIScale);
		CRavidRect<int> rrLedRect(rpLedBasePos.x, rpLedBasePos.y, rpLedBasePos.x + 15 * dblDPIScale, rpLedBasePos.y + 15 * dblDPIScale);
		CRavidPoint<int> rpLedYGap(0, 25 * dblDPIScale);
		CRavidPoint<int> rpLedXGap(20 * dblDPIScale, 0);

		int nInputLedCreatedCount = 0;
		int nOutputLedCreatedCount = 0;

		int nRealPos = 0;

		long nIOPort = GetInputPortNumber();

		for(int i = 0; i < nIOPort; ++i)
		{
			if(!m_pUsedInput[i])
				continue;

			CRavidLedCtrl* pLed = new CRavidLedCtrl;
			pLed->Create(nullptr, WS_VISIBLE | WS_CHILD | SS_NOTIFY, rrLedRect + rpLedYGap * nRealPos, this, EDeviceAddEnumeratedControlID_InputLedStart + i);
			pLed->SetFont(pBaseFont);
			m_vctCtrl.push_back(pLed);

			CStatic* pLabel = new CStatic;
			CRavidRect<int> rrLabelPosition(rrLedRect + rpLedYGap * nRealPos);
			rrLabelPosition += rpLedXGap;
			rrLabelPosition.right = rrGroupBox[0].right - 10 * dblDPIScale;

			CString strName;
			strName.Format(_T("%d : %s"), i, m_pStrInputName[i]);
			pLabel->Create(strName, WS_VISIBLE | WS_CHILD | SS_NOTIFY, rrLabelPosition, this, EDeviceAddEnumeratedControlID_InputLedLabelStart + i);
			pLabel->SetFont(pBaseFont);
			m_vctCtrl.push_back(pLabel);

			++nRealPos;
		}

		nInputLedCreatedCount = nRealPos;

		rrGroupBox.bottom = (rrLedRect + rpLedYGap * (__max(nInputLedCreatedCount, nOutputLedCreatedCount) - 1)).bottom + 10 * dblDPIScale;

		CButton* pGroupBox = new CButton;

		pGroupBox->Create(CMultiLanguageManager::GetString(ELanguageParameter_Input), BS_GROUPBOX | WS_CHILD | WS_VISIBLE, rrGroupBox, this, EDeviceAddEnumeratedControlID_StaticStart);
		pGroupBox->SetFont(pBaseFont);
		m_vctCtrl.push_back(pGroupBox);

		(*pResultGroupBox).left = rrGroupBox.left;
		(*pResultGroupBox).top = rrGroupBox.top;
		(*pResultGroupBox).right = rrGroupBox.right;
		(*pResultGroupBox).bottom = rrGroupBox.bottom;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceMVTechMSPDio::AddIOsNotOutput2(double dblDPIScale, CFont * pBaseFont, CRavidPoint<int> cpLeftTopPt, CRavidPoint<int> cpRightBottomPt, CRavidRect<int>* pResultGroupBox)
{
	bool bReturn = false;

	do
	{
		CRavidRect<int> rrGroupBox[2];

		rrGroupBox[0].SetRect(cpLeftTopPt.x, cpLeftTopPt.y, (cpLeftTopPt.x + cpRightBottomPt.x) / 2., cpRightBottomPt.y + 100 * dblDPIScale);
		rrGroupBox[1].SetRect((cpLeftTopPt.x + cpRightBottomPt.x) / 2., cpLeftTopPt.y, cpRightBottomPt.x, cpRightBottomPt.y + 100 * dblDPIScale);

		CRavidPoint<int> rpLedBasePos1(rrGroupBox[0].left + 10 * dblDPIScale, rrGroupBox[0].top + 20 * dblDPIScale);
		CRavidRect<int> rrLedRect1(rpLedBasePos1.x, rpLedBasePos1.y, rpLedBasePos1.x + 15 * dblDPIScale, rpLedBasePos1.y + 15 * dblDPIScale);
		CRavidPoint<int> rpLedYGap(0, 25 * dblDPIScale);
		CRavidPoint<int> rpLedXGap(20 * dblDPIScale, 0);

		CRavidPoint<int>  rpLedBasePos2(rrGroupBox[1].left + 10 * dblDPIScale, rrGroupBox[1].top + 20 * dblDPIScale);
		CRavidRect<int>  rrLedRect2(rpLedBasePos2.x, rpLedBasePos2.y, rpLedBasePos2.x + 15 * dblDPIScale, rpLedBasePos2.y + 15 * dblDPIScale);

		int nInputLedCreatedCount = 0;
		int nOutputLedCreatedCount = 0;

		int nRealPosX = 0;
		int nRealPosY = 0;

		long nIOPort = GetInputPortNumber();

		for(int i = 0; i < nIOPort; ++i)
		{
			if(!m_pUsedInput[i])
				continue;

			if(!(nRealPosX % 2))
			{
				CRavidLedCtrl* pLed = new CRavidLedCtrl;
				pLed->Create(nullptr, WS_VISIBLE | WS_CHILD | SS_NOTIFY, rrLedRect1 + rpLedYGap * nRealPosY, this, EDeviceAddEnumeratedControlID_InputLedStart + i);
				pLed->SetFont(pBaseFont);
				m_vctCtrl.push_back(pLed);

				CStatic* pLabel = new CStatic;
				CRavidRect<int> rrLabelPosition(rrLedRect1 + rpLedYGap * nRealPosY);
				rrLabelPosition += rpLedXGap;
				rrLabelPosition.right = rrGroupBox[0].right - 10 * dblDPIScale;

				CString strName;
				strName.Format(_T("%d : %s"), i, m_pStrInputName[i]);
				pLabel->Create(strName, WS_VISIBLE | WS_CHILD | SS_NOTIFY, rrLabelPosition, this, EDeviceAddEnumeratedControlID_InputLedLabelStart + i);
				pLabel->SetFont(pBaseFont);
				m_vctCtrl.push_back(pLabel);
			}
			else
			{
				CRavidLedCtrl* pLed = new CRavidLedCtrl;
				pLed->Create(nullptr, WS_VISIBLE | WS_CHILD | SS_NOTIFY, rrLedRect2 + rpLedYGap * nRealPosY, this, EDeviceAddEnumeratedControlID_InputLedStart + i);
				pLed->SetFont(pBaseFont);
				m_vctCtrl.push_back(pLed);

				CStatic* pLabel = new CStatic;
				CRavidRect<int> rrLabelPosition(rrLedRect2 + rpLedYGap * nRealPosY);
				rrLabelPosition += rpLedXGap;
				rrLabelPosition.right = rrGroupBox[1].right - 10 * dblDPIScale;

				CString strName;
				strName.Format(_T("%d : %s"), i, m_pStrInputName[i]);
				pLabel->Create(strName, WS_VISIBLE | WS_CHILD | SS_NOTIFY, rrLabelPosition, this, EDeviceAddEnumeratedControlID_InputLedLabelStart + i);
				pLabel->SetFont(pBaseFont);
				m_vctCtrl.push_back(pLabel);
			}

			++nRealPosX;

			if(nRealPosX == 2)
			{
				nRealPosX = 0;
				++nRealPosY;
			}
		}

		if(!(nRealPosX % 2) && nRealPosY)
			--nRealPosY;

		rrGroupBox[0].right = rrGroupBox[1].right;
		rrGroupBox[0].bottom = (rrLedRect1 + (rpLedYGap * nRealPosY)).bottom + 10 * dblDPIScale;

		CButton* pGroupBox = new CButton;

		pGroupBox->Create(CMultiLanguageManager::GetString(ELanguageParameter_Input), BS_GROUPBOX | WS_CHILD | WS_VISIBLE, rrGroupBox[0], this, EDeviceAddEnumeratedControlID_StaticStart);
		pGroupBox->SetFont(pBaseFont);
		m_vctCtrl.push_back(pGroupBox);

		(*pResultGroupBox).left = rrGroupBox[0].left;
		(*pResultGroupBox).top = rrGroupBox[0].top;
		(*pResultGroupBox).right = rrGroupBox[0].right;
		(*pResultGroupBox).bottom = rrGroupBox[0].bottom;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceMVTechMSPDio::AddIOsNotOutput3(double dblDPIScale, CFont * pBaseFont, CRavidPoint<int> cpLeftTopPt, CRavidPoint<int> cpRightBottomPt, CRavidRect<int>* pResultGroupBox)
{
	bool bReturn = false;

	do
	{
		CRavidRect<int> rrGroupBox;

		rrGroupBox.SetRect(cpLeftTopPt.x, cpLeftTopPt.y, cpRightBottomPt.x, cpRightBottomPt.y + 100 * dblDPIScale);


		CRavidPoint<int> rpLedBasePos(rrGroupBox.left + 10 * dblDPIScale, rrGroupBox.top + 20 * dblDPIScale);
		CRavidRect<int> rrLedRect(rpLedBasePos.x, rpLedBasePos.y, rpLedBasePos.x + 15 * dblDPIScale, rpLedBasePos.y + 15 * dblDPIScale);
		CRavidPoint<int> rpLedYGap(0, 25 * dblDPIScale);
		CRavidPoint<int> rpLedXGap(20 * dblDPIScale, 0);

		int nInputLedCreatedCount = 0;
		int nOutputLedCreatedCount = 0;

		int nRealPosY = 0;

		long nIOPort = GetInputPortNumber();

		for(int i = 0; i < nIOPort; ++i)
		{
			if(!m_pUsedInput[i])
				continue;

			CRavidLedCtrl* pLed = new CRavidLedCtrl;
			pLed->Create(nullptr, WS_VISIBLE | WS_CHILD | SS_NOTIFY, rrLedRect + rpLedYGap * nRealPosY, this, EDeviceAddEnumeratedControlID_InputLedStart + i);
			pLed->SetFont(pBaseFont);
			m_vctCtrl.push_back(pLed);

			CStatic* pLabel = new CStatic;
			CRavidRect<int> rrLabelPosition(rrLedRect + rpLedYGap * nRealPosY);
			rrLabelPosition += rpLedXGap;
			rrLabelPosition.right = rrGroupBox.right - 10 * dblDPIScale;

			CString strName;
			strName.Format(_T("%d : %s"), i, m_pStrInputName[i]);
			pLabel->Create(strName, WS_VISIBLE | WS_CHILD | SS_NOTIFY, rrLabelPosition, this, EDeviceAddEnumeratedControlID_InputLedLabelStart + i);
			pLabel->SetFont(pBaseFont);
			m_vctCtrl.push_back(pLabel);

			++nRealPosY;
		}

		if(nRealPosY)
			--nRealPosY;

		rrGroupBox.right = rrGroupBox.right;
		rrGroupBox.bottom = (rrLedRect + (rpLedYGap * nRealPosY)).bottom + 10 * dblDPIScale;

		CButton* pGroupBox = new CButton;

		pGroupBox->Create(CMultiLanguageManager::GetString(ELanguageParameter_Input), BS_GROUPBOX | WS_CHILD | WS_VISIBLE, rrGroupBox, this, EDeviceAddEnumeratedControlID_StaticStart);
		pGroupBox->SetFont(pBaseFont);
		m_vctCtrl.push_back(pGroupBox);

		(*pResultGroupBox).left = rrGroupBox.left;
		(*pResultGroupBox).top = rrGroupBox.top;
		(*pResultGroupBox).right = rrGroupBox.right;
		(*pResultGroupBox).bottom = rrGroupBox.bottom;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CDeviceMVTechMSPDio::AddTriggers(double dblDPIScale, CFont * pBaseFont, CRavidPoint<int> cpLeftTopPt, CRavidPoint<int> cpRightBottomPt, CRavidRect<int>* pResultGroupBox)
{
	bool bReturn = false;

	do
	{
		CWnd* pWndInitBtn = GetDlgItem(int32_t(RavidInterfaceControlID::DeviceButtonInitialize));
		if(!pWndInitBtn)
			break;

		CRect rrInitializeBtn;

		pWndInitBtn->GetWindowRect(rrInitializeBtn);
		ScreenToClient(rrInitializeBtn);

		CRavidRect<int> rrGroupBox;

		rrGroupBox.SetRect(cpLeftTopPt.x, cpLeftTopPt.y, cpRightBottomPt.x, cpRightBottomPt.y);

		CRavidPoint<int> rpLedBasePos(rrGroupBox.left + 10 * dblDPIScale, rrGroupBox.top + 20 * dblDPIScale);

		int nDialogEdgeMargin = 10 * dblDPIScale;
		int nButtonInterval = 5 * dblDPIScale;
		int nLabelCaption = 100 * dblDPIScale;
		int nInfoTextHeight = 20 * dblDPIScale;
		int nInfomationRows = 2 * dblDPIScale;
		int nGroupBoxVerticalMargin = 25 * dblDPIScale;
		int nLedLength = 15 * dblDPIScale;
		int nLedHeightInterval = 18 * dblDPIScale;

		int nUseWidth = (cpRightBottomPt.x - cpLeftTopPt.x) - (nButtonInterval * 2);
		int nButtonWidth = (nUseWidth / 3);

		CRavidRect<int> rrLedRect(rpLedBasePos.x, rpLedBasePos.y, rpLedBasePos.x + 15 * dblDPIScale, rpLedBasePos.y + 15 * dblDPIScale);
		CRavidRect<int> rrCaption(rrLedRect.right, rrLedRect.top, rrLedRect.right + nLabelCaption, rrLedRect.bottom);
		CRavidRect<int> rrValue(rrCaption.right, rrCaption.top, cpRightBottomPt.x - nInfomationRows, rrCaption.bottom);
		CRavidPoint<int> rpLedYGap(0, 25 * dblDPIScale);
		CRavidPoint<int> rpLedXGap(20 * dblDPIScale, 0);

		int nPos = 0;

		long nEncoderCounts = GetEncoderPortNumber();
		long nChannel = GetChannelNumber();

		for(int i = 0; i < nEncoderCounts * nChannel; ++i)
		{
			CRavidPoint<int> rpInterval = rpLedYGap * nPos;

			CRavidRect<int> rrLedEncoder(rrLedRect + rpInterval);

			CRavidLedCtrl* pLedEncoder = new CRavidLedCtrl;

			if(pLedEncoder)
			{
				pLedEncoder->Create(nullptr, WS_VISIBLE | WS_CHILD | SS_NOTIFY, rrLedEncoder, this, EDeviceAddEnumeratedControlID_EncLedStart + i);
				pLedEncoder->SetFont(pBaseFont);
				m_vctCtrl.push_back(pLedEncoder);
			}

			CRavidRect<int> rrEncoderCaption(rrCaption + rpLedXGap + rpInterval);

			CStatic* pSttEncoderCountCaption = new CStatic;

			if(!pSttEncoderCountCaption)
				break;

			pSttEncoderCountCaption->Create(!i ? _T("[CH1] ") + CMultiLanguageManager::GetString(ELanguageParameter_EncoderCount) + _T(" :") : _T("[CH2] ") + CMultiLanguageManager::GetString(ELanguageParameter_EncoderCount) + _T(" :"), WS_CHILD | WS_VISIBLE | WS_TABSTOP, rrEncoderCaption, this, EDeviceAddEnumeratedControlID_StaticStart + 2 + i);
			pSttEncoderCountCaption->SetFont(pBaseFont);
			m_vctCtrl.push_back(pSttEncoderCountCaption);

			CRavidRect<int> rrEncoderValue(rrValue + rpLedXGap + rpInterval);
			rrEncoderValue.right = cpRightBottomPt.x - nDialogEdgeMargin;

			CStatic* pSttEncoderCountValue = new CStatic;

			if(!pSttEncoderCountValue)
				break;

			pSttEncoderCountValue->Create(_T("0 (0.000 KHz)"), WS_CHILD | WS_VISIBLE | WS_TABSTOP | DT_RIGHT, rrEncoderValue, this, EDeviceAddEnumeratedControlID_EncCaptionStart + i);
			pSttEncoderCountValue->SetFont(pBaseFont);
			m_vctCtrl.push_back(pSttEncoderCountValue);

			++nPos;
		}

		long nTriggerCounts = GetTriggerPortNumber();

		for(int i = 0; i < nTriggerCounts * nChannel; ++i)
		{
			CRavidPoint<int> rpInterval = rpLedYGap * nPos;

			CRavidRect<int> rrTrigger(rrLedRect + rpInterval);

			CRavidLedCtrl* pLedTrigger = new CRavidLedCtrl;

			if(pLedTrigger)
			{
				pLedTrigger->Create(nullptr, WS_VISIBLE | WS_CHILD | SS_NOTIFY, rrTrigger, this, EDeviceAddEnumeratedControlID_TrgLedStart + i);
				pLedTrigger->SetFont(pBaseFont);
				m_vctCtrl.push_back(pLedTrigger);
			}

			CRavidRect<int> rrTriggerCaption(rrCaption + rpLedXGap + rpInterval);

			CStatic* pSttTriggerCountCaption = new CStatic;

			if(!pSttTriggerCountCaption)
				break;

			CString strName;
			strName.Format(_T("%s %s %d :"), i < nTriggerCounts ? _T("[CH1]") : _T("[CH2]"), CMultiLanguageManager::GetString(ELanguageParameter_TriggerCount), (i % nTriggerCounts) + 1);

			pSttTriggerCountCaption->Create(strName, WS_CHILD | WS_VISIBLE | WS_TABSTOP, rrTriggerCaption, this, EDeviceAddEnumeratedControlID_StaticStart + 4 + i);
			pSttTriggerCountCaption->SetFont(pBaseFont);
			m_vctCtrl.push_back(pSttTriggerCountCaption);

			CRavidRect<int> rrTriggerValue(rrValue + rpLedXGap + rpInterval);
			rrTriggerValue.right = cpRightBottomPt.x - nDialogEdgeMargin;

			CStatic* pSttTriggerCountValue = new CStatic;

			if(!pSttTriggerCountValue)
				break;

			pSttTriggerCountValue->Create(_T("0 (0.000 KHz)"), WS_CHILD | WS_VISIBLE | WS_TABSTOP | DT_RIGHT, rrTriggerValue, this, EDeviceAddEnumeratedControlID_TrgCaptionStart + i);
			pSttTriggerCountValue->SetFont(pBaseFont);
			m_vctCtrl.push_back(pSttTriggerCountValue);

			++nPos;
		}

		--nPos;

		CRavidPoint<int> rpInterval = rpLedYGap * nPos;

		CRavidRect<int> rrTrigger(rrLedRect + rpInterval);

		rrGroupBox.bottom = __max(rrTrigger.bottom + nDialogEdgeMargin, rrGroupBox.bottom);

		CButton* pGroupBox = new CButton;
		
		pGroupBox->Create(CMultiLanguageManager::GetString(ELanguageParameter_EncoderAndTriggerInfo), BS_GROUPBOX | WS_CHILD | WS_VISIBLE, rrGroupBox, this, EDeviceAddEnumeratedControlID_StaticStart + 10);
		pGroupBox->SetFont(pBaseFont);
		m_vctCtrl.push_back(pGroupBox);

		*pResultGroupBox = rrGroupBox;
	}
	while(false);

	return bReturn;
}

bool CDeviceMVTechMSPDio::AddQueues(double dblDPIScale, int nStartID, CFont* pBaseFont, CRavidPoint<int> cpLeftTopPt, CRavidPoint<int> cpRightBottomPt, CRavidRect<int>* pResultGroupBox)
{
	bool bReturn = false;

	do
	{
		CRavidRect<int> rrGroupBox;

		rrGroupBox.SetRect(cpLeftTopPt.x, cpLeftTopPt.y, cpRightBottomPt.x, cpRightBottomPt.y);

		CRavidPoint<int> rpLedBasePos(rrGroupBox.left + 10 * dblDPIScale, rrGroupBox.top + 20 * dblDPIScale);

		CRavidRect<int> rrLedRect(rpLedBasePos.x, rpLedBasePos.y, rpLedBasePos.x + 15 * dblDPIScale, rpLedBasePos.y + 15 * dblDPIScale);
		CRavidRect<int> rrQueueValue(0, 0, 65 * dblDPIScale, 15 * dblDPIScale);

		CRavidPoint<int> rpLedXGap(rrQueueValue.GetWidth() + 40 * dblDPIScale, 0);
		CRavidPoint<int> rpLedYGap(0, 22 * dblDPIScale);

		int nDialogEdgeMargin = 10 * dblDPIScale;

		int nRealPosX = 0;
		int nRealPosY = 0;

		long nQueueCounts = GetQueueNumber();

		for(int i = 0; i < nQueueCounts; ++i)
		{
			CRavidRect<int> rrLed(rrLedRect + rpLedYGap * nRealPosY + rpLedXGap * nRealPosX);
			CRavidRect<int> rrValue(rrQueueValue);
			rrValue += CRavidPoint<int>(rrLed.right, rrLed.top);
			rrValue.left += (5 * dblDPIScale);
			rrValue.right += (5 * dblDPIScale);

			CString strID;
			strID.Format(_T("%d"), i);

			CRavidLedCtrl* pLed = new CRavidLedCtrl;
			pLed->Create(strID, WS_VISIBLE | WS_CHILD | SS_NOTIFY, rrLed, this, EDeviceAddEnumeratedControlID_QueueLedStart + i + nStartID);
			pLed->SetFont(pBaseFont);
			m_vctCtrl.push_back(pLed);

			CStatic* pLabel = new CStatic;

			CString strName;
			strName.Format(_T("%d"), 0);
			pLabel->Create(strName, WS_VISIBLE | WS_CHILD | SS_NOTIFY | DT_RIGHT, rrValue, this, EDeviceAddEnumeratedControlID_QueueLedLabelStart + i + nStartID);
			pLabel->SetFont(pBaseFont);
			m_vctCtrl.push_back(pLabel);

			++nRealPosX;

			if(!(nRealPosX % 4))
			{
				++nRealPosY;
				nRealPosX = 0;
			}
		}

		int nSyncPosition = !nRealPosX ? 1 : 0;

		rrGroupBox.bottom = __max(rrLedRect.bottom + rpLedYGap.y * (nRealPosY - nSyncPosition) + nDialogEdgeMargin, rrGroupBox.bottom);

		CButton* pGroupBox = new CButton;

		int nCtrlIndex = !nStartID ? 0 : 1;

		if(GetChannelNumber() != 1)
			pGroupBox->Create(!nStartID ? _T("[CH1] ") + CMultiLanguageManager::GetString(ELanguageParameter_QueueInfo) : _T("[CH2] ") + CMultiLanguageManager::GetString(ELanguageParameter_QueueInfo), BS_GROUPBOX | WS_CHILD | WS_VISIBLE, rrGroupBox, this, EDeviceAddEnumeratedControlID_StaticStart + 18 + nCtrlIndex);
		else
			pGroupBox->Create(CMultiLanguageManager::GetString(ELanguageParameter_QueueInfo), BS_GROUPBOX | WS_CHILD | WS_VISIBLE, rrGroupBox, this, EDeviceAddEnumeratedControlID_StaticStart + 17);

		pGroupBox->SetFont(pBaseFont);
		m_vctCtrl.push_back(pGroupBox);

		*pResultGroupBox = rrGroupBox;
	}
	while(false);

	return bReturn;
}

bool CDeviceMVTechMSPDio::AddQueuesNotOutput(double dblDPIScale, int nStartID, CFont * pBaseFont, CRavidPoint<int> cpLeftTopPt, CRavidPoint<int> cpRightBottomPt, CRavidRect<int>* pResultGroupBox)
{
	bool bReturn = false;

	do
	{
		CRavidRect<int> rrGroupBox;

		rrGroupBox.SetRect(cpLeftTopPt.x, cpLeftTopPt.y, cpRightBottomPt.x, cpRightBottomPt.y);

		CRavidPoint<int> rpLedBasePos(rrGroupBox.left + 10 * dblDPIScale, rrGroupBox.top + 20 * dblDPIScale);

		CRavidRect<int> rrLedRect(rpLedBasePos.x, rpLedBasePos.y, rpLedBasePos.x + 15 * dblDPIScale, rpLedBasePos.y + 15 * dblDPIScale);
		CRavidRect<int> rrQueueValue(0, 0, 65 * dblDPIScale, 15 * dblDPIScale);

		CRavidPoint<int> rpLedXGap(rrQueueValue.GetWidth() + 50 * dblDPIScale, 0);
		CRavidPoint<int> rpLedYGap(0, 22 * dblDPIScale);

		int nDialogEdgeMargin = 10 * dblDPIScale;

		int nRealPosX = 0;
		int nRealPosY = 0;

		long nQueueCounts = GetQueueNumber();

		for(int i = 0; i < nQueueCounts; ++i)
		{
			CRavidRect<int> rrLed(rrLedRect + rpLedYGap * nRealPosY + rpLedXGap * nRealPosX);
			CRavidRect<int> rrValue(rrQueueValue);
			rrValue += CRavidPoint<int>(rrLed.right, rrLed.top);
			rrValue.left += (5 * dblDPIScale);
			rrValue.right += (5 * dblDPIScale);

			CString strID;
			strID.Format(_T("%d"), i);

			CRavidLedCtrl* pLed = new CRavidLedCtrl;
			pLed->Create(strID, WS_VISIBLE | WS_CHILD | SS_NOTIFY, rrLed, this, EDeviceAddEnumeratedControlID_QueueLedStart + i + nStartID);
			pLed->SetFont(pBaseFont);
			m_vctCtrl.push_back(pLed);

			CStatic* pLabel = new CStatic;

			CString strName;
			strName.Format(_T("%d"), 0);
			pLabel->Create(strName, WS_VISIBLE | WS_CHILD | SS_NOTIFY | DT_RIGHT, rrValue, this, EDeviceAddEnumeratedControlID_QueueLedLabelStart + i + nStartID);
			pLabel->SetFont(pBaseFont);
			m_vctCtrl.push_back(pLabel);

			++nRealPosX;

			if(!(nRealPosX % 2))
			{
				++nRealPosY;
				nRealPosX = 0;
			}
		}

		int nSyncPosition = !nRealPosX ? 1 : 0;

		rrGroupBox.bottom = __max(rrLedRect.bottom + rpLedYGap.y * (nRealPosY - nSyncPosition) + nDialogEdgeMargin, rrGroupBox.bottom);

		CButton* pGroupBox = new CButton;

		int nCtrlIndex = !nStartID ? 0 : 1;

		if(GetChannelNumber() != 1)
			pGroupBox->Create(!nStartID ? _T("[CH1] ") + CMultiLanguageManager::GetString(ELanguageParameter_QueueInfo) : _T("[CH2] ") + CMultiLanguageManager::GetString(ELanguageParameter_QueueInfo), BS_GROUPBOX | WS_CHILD | WS_VISIBLE, rrGroupBox, this, EDeviceAddEnumeratedControlID_StaticStart + 18 + nCtrlIndex);
		else
			pGroupBox->Create(CMultiLanguageManager::GetString(ELanguageParameter_QueueInfo), BS_GROUPBOX | WS_CHILD | WS_VISIBLE, rrGroupBox, this, EDeviceAddEnumeratedControlID_StaticStart + 17);

		pGroupBox->SetFont(pBaseFont);
		m_vctCtrl.push_back(pGroupBox);

		*pResultGroupBox = rrGroupBox;
	}
	while(false);

	return bReturn;
}

bool CDeviceMVTechMSPDio::AddButton(double dblDPIScale, CFont * pBaseFont, CRavidPoint<int> cpLeftTopPt, CRavidPoint<int> cpRightBottomPt, CRavidRect<int>* pResultGroupBox)
{
	bool bReturn = false;

	do
	{
		int nDialogEdgeMargin = 10 * dblDPIScale;
		int nWidthDistance = cpRightBottomPt.x - cpLeftTopPt.x - (2 * nDialogEdgeMargin);
		int nHeightDistance = cpRightBottomPt.y - cpLeftTopPt.y;

		int nBtnWidth = nWidthDistance / 3;

		CRavidPoint<int> rpBasePos(cpLeftTopPt.x, cpLeftTopPt.y);
		CRavidPoint<int> rpYGap(0, 25 * dblDPIScale);
		CRavidPoint<int> rpXGap(nBtnWidth + nDialogEdgeMargin, 0);

		int nPosY = 0;

		long nTriggerCounts = GetTriggerPortNumber();
		long nChannel = GetChannelNumber();

		for(int i = 0; i < nTriggerCounts * nChannel; ++i)
		{
			CButton* pBtnEnableTrigger = new CButton;

			if(!pBtnEnableTrigger)
				break;

			CRavidRect<int> rrBtnEnableTrigger(rpBasePos.x, rpBasePos.y, rpBasePos.x + nBtnWidth, rpBasePos.y + nHeightDistance);
			rrBtnEnableTrigger += (rpYGap * nPosY);

			CString strChannel = i < nTriggerCounts ? _T("[CH1]") : _T("[CH2]");
			int nIndex = (i % 3) + 1;

			CString strName;
			strName.Format(_T("%s %s %d"), strChannel, CMultiLanguageManager::GetString(ELanguageParameter_EnableTrigger), nIndex);

			pBtnEnableTrigger->Create(strName, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_FLAT, rrBtnEnableTrigger, this, EDeviceAddEnumeratedControlID_EnableButtonStart + i);
			pBtnEnableTrigger->SetFont(pBaseFont);
			m_vctCtrl.push_back(pBtnEnableTrigger);

			rrBtnEnableTrigger += rpXGap;

			CButton* pBtnDisableTrigger = new CButton;

			if(!pBtnDisableTrigger)
				break;

			strName.Format(_T("%s %s %d"), strChannel, CMultiLanguageManager::GetString(ELanguageParameter_DisableTrigger), nIndex);

			pBtnDisableTrigger->Create(strName, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_FLAT, rrBtnEnableTrigger, this, EDeviceAddEnumeratedControlID_DisableButtonStart + i);
			pBtnDisableTrigger->SetFont(pBaseFont);
			m_vctCtrl.push_back(pBtnDisableTrigger);

			if(i < 2)
			{
				rrBtnEnableTrigger += rpXGap;

				CButton* pBtnReset = new CButton;

				if(!pBtnReset)
					break;

				strName.Format(_T("%s %d"), CMultiLanguageManager::GetString(ELanguageParameter_ResetCounter), nIndex);

				pBtnReset->Create(strName, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_FLAT, rrBtnEnableTrigger, this, EDeviceAddEnumeratedControlID_ResetButtonStart + i);
				pBtnReset->SetFont(pBaseFont);
				m_vctCtrl.push_back(pBtnReset);
			}

			++nPosY;
		}
		(*pResultGroupBox).left = cpLeftTopPt.x;
		(*pResultGroupBox).top = cpLeftTopPt.y;
		(*pResultGroupBox).right = cpRightBottomPt.x;
		(*pResultGroupBox).bottom = rpBasePos.y + nHeightDistance + rpYGap.y * (nPosY - 1);
	}
	while(false);

	return bReturn;
}

bool CDeviceMVTechMSPDio::AddIDs(double dblDPIScale, CFont * pBaseFont, CRavidPoint<int> cpLeftTopPt, CRavidPoint<int> cpRightBottomPt, CRavidRect<int>* pResultGroupBox)
{
	bool bReturn = false;

	do
	{
		CRavidRect<int> rrGroupBox;

		rrGroupBox.SetRect(cpLeftTopPt.x, cpLeftTopPt.y, cpRightBottomPt.x, cpRightBottomPt.y);

		long nIDCount = GetIDCount();

		if(nIDCount < 1)
			break;

		int nDialogEdgeMargin = 10 * dblDPIScale;
		int nWidthDistance = cpRightBottomPt.x - cpLeftTopPt.x - (2 * nDialogEdgeMargin);
		int nHeightDistance = 25 * dblDPIScale;

		int nSize = nIDCount;

		int nBtnWidth = nWidthDistance / (nSize * 2);

		CRavidPoint<int> rpBasePos(cpLeftTopPt.x, cpLeftTopPt.y + nDialogEdgeMargin);
		CRavidPoint<int> rpXSize(nBtnWidth, 0);
		CRavidPoint<int> rpXGap(nDialogEdgeMargin, 0);

		CRavidRect<int> rrStatic(rpBasePos.x + 1, rpBasePos.y + nDialogEdgeMargin, rpBasePos.x + nBtnWidth, rpBasePos.y + nHeightDistance + nDialogEdgeMargin);

		for(int i = 0; i < nIDCount; ++i)
		{
			DWORD dwData = -1;

			GetID(i, &dwData);

			CStatic* pIdName = new CStatic;

			CString strIDIndex;
			strIDIndex.Format(_T("%s %d : "), CMultiLanguageManager::GetString(ELanguageParameter_ID), i);

			pIdName->Create(strIDIndex, WS_VISIBLE | WS_CHILD | SS_NOTIFY | DT_CENTER, rrStatic, this, EDeviceAddEnumeratedControlID_StaticStart + 21 + i);
			pIdName->SetFont(pBaseFont);
			m_vctCtrl.push_back(pIdName);

			rrStatic += rpXSize;

			CStatic* pIdValue = new CStatic;

			if(!pIdValue)
				break;

			strIDIndex.Format(_T("%lu"), dwData);

			pIdValue->Create(strIDIndex, WS_VISIBLE | WS_CHILD | SS_NOTIFY | DT_CENTER, rrStatic, this, EDeviceAddEnumeratedControlID_TriggerIDStart + i);
			pIdValue->SetFont(pBaseFont);
			m_vctCtrl.push_back(pIdValue);

			rrStatic += rpXSize;
		}

		rrGroupBox.bottom = __max(rrStatic.bottom + nDialogEdgeMargin, rrGroupBox.bottom);

		CButton* pGroupBox = new CButton;

		pGroupBox->Create(CMultiLanguageManager::GetString(ELanguageParameter_Information), BS_GROUPBOX | WS_CHILD | WS_VISIBLE, rrGroupBox, this, EDeviceAddEnumeratedControlID_StaticStart + 20);
		pGroupBox->SetFont(pBaseFont);
		m_vctCtrl.push_back(pGroupBox);

		(*pResultGroupBox).left = cpLeftTopPt.x;
		(*pResultGroupBox).top = cpLeftTopPt.y;
		(*pResultGroupBox).right = cpRightBottomPt.x;
		(*pResultGroupBox).bottom = rrGroupBox.bottom + nDialogEdgeMargin;
	}
	while(false);

	return bReturn;
}

void CDeviceMVTechMSPDio::DisplayParameter(EDeviceMSPBoardType eType)
{
	switch(eType)
	{
	case EDeviceMSPBoardType_MSP_P16_E2T6_Q288:
		{
			memset(m_pUsedInput, 1, sizeof(bool) * MAX_IN_PORT);
			memset(m_pUsedOutput, 1, sizeof(bool) * MAX_OUT_PORT);

			for(int i = 0; i < MAX_IN_PORT; ++i)
			{
				m_pStrInputName[i] = m_vctParameterFieldConfigurations[EDeviceParameterMVTechMSPTrigger_InputNo0Name + i].strParameterValue;
				m_vctParameterFieldConfigurations[EDeviceParameterMVTechMSPTrigger_UseInputNo0 + i].strParameterValue = _T("1");
				SaveSettings(EDeviceParameterMVTechMSPTrigger_UseInputNo0 + i);
			}

			for(int i = 0; i < MAX_OUT_PORT; ++i)
			{
				m_pStrOutputName[i] = m_vctParameterFieldConfigurations[EDeviceParameterMVTechMSPTrigger_OutputNo0Name + i].strParameterValue;
				m_vctParameterFieldConfigurations[EDeviceParameterMVTechMSPTrigger_UseOutputNo0 + i].strParameterValue = _T("1");
				SaveSettings(EDeviceParameterMVTechMSPTrigger_UseOutputNo0 + i);
			}

			for(int i = EDeviceParameterMVTechMSPTrigger_UIInfo; i < EDeviceParameterMVTechMSPTrigger_TriggerInfo; ++i)
				m_vctParameterFieldConfigurations[i].bShow = true;

			for(int i = EDeviceParameterMVTechMSPTrigger_Channel1; i < EDeviceParameterMVTechMSPTrigger_EndPositionCH1; ++i)
				m_vctParameterFieldConfigurations[i].bShow = true;

			for(int i = EDeviceParameterMVTechMSPTrigger_EndPositionCH1; i < EDeviceParameterMVTechMSPTrigger_Channel2; ++i)
				m_vctParameterFieldConfigurations[i].bShow = false;

			for(int i = EDeviceParameterMVTechMSPTrigger_Channel2; i < EDeviceParameterMVTechMSPTrigger_EndPositionCH2; ++i)
				m_vctParameterFieldConfigurations[i].bShow = true;

			for(int i = EDeviceParameterMVTechMSPTrigger_EndPositionCH2; i < EDeviceParameterMVTechMSPTrigger_UseInput; ++i)
				m_vctParameterFieldConfigurations[i].bShow = false;

			for(int i = EDeviceParameterMVTechMSPTrigger_UseInput; i < EDeviceParameterMVTechMSPTrigger_InputName; ++i)
				m_vctParameterFieldConfigurations[i].bShow = false;

			for(int i = EDeviceParameterMVTechMSPTrigger_InputName; i < EDeviceParameterMVTechMSPTrigger_Count; ++i)
				m_vctParameterFieldConfigurations[i].bShow = true;

			m_vctParameterFieldConfigurations[EDeviceParameterMVTechMSPTrigger_ShowID].bShow = false;
		}
		break;
	case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_1:
		{
			for(int i = 0; i < MAX_IN_PORT; ++i)
				m_pStrInputName[i] = m_vctParameterFieldConfigurations[EDeviceParameterMVTechMSPTrigger_InputNo0Name + i].strParameterValue;

			for(int i = 0; i < MAX_OUT_PORT; ++i)
				m_pStrOutputName[i] = m_vctParameterFieldConfigurations[EDeviceParameterMVTechMSPTrigger_OutputNo0Name + i].strParameterValue;

			for(int i = EDeviceParameterMVTechMSPTrigger_UIInfo; i < EDeviceParameterMVTechMSPTrigger_TriggerInfo; ++i)
				m_vctParameterFieldConfigurations[i].bShow = true;

			for(int i = EDeviceParameterMVTechMSPTrigger_Channel1; i < EDeviceParameterMVTechMSPTrigger_Count; ++i)
				m_vctParameterFieldConfigurations[i].bShow = true;

			m_vctParameterFieldConfigurations[EDeviceParameterMVTechMSPTrigger_ShowID].bShow = false;
		}
		break;
	case EDeviceMSPBoardType_MSP_P16_E2T2_Q16_2:
		{
			for(int i = 0; i < MAX_IN_PORT; ++i)
				m_pStrInputName[i] = m_vctParameterFieldConfigurations[EDeviceParameterMVTechMSPTrigger_InputNo0Name + i].strParameterValue;

			for(int i = 0; i < MAX_OUT_PORT; ++i)
				m_pStrOutputName[i] = m_vctParameterFieldConfigurations[EDeviceParameterMVTechMSPTrigger_OutputNo0Name + i].strParameterValue;

			for(int i = EDeviceParameterMVTechMSPTrigger_UIInfo; i < EDeviceParameterMVTechMSPTrigger_TriggerInfo; ++i)
				m_vctParameterFieldConfigurations[i].bShow = true;

			for(int i = EDeviceParameterMVTechMSPTrigger_Channel1; i < EDeviceParameterMVTechMSPTrigger_UseOutput; ++i)
				m_vctParameterFieldConfigurations[i].bShow = true;

			for(int i = EDeviceParameterMVTechMSPTrigger_UseOutput; i < EDeviceParameterMVTechMSPTrigger_InputName; ++i)
				m_vctParameterFieldConfigurations[i].bShow = false;

			for(int i = EDeviceParameterMVTechMSPTrigger_InputName; i < EDeviceParameterMVTechMSPTrigger_OutputName; ++i)
				m_vctParameterFieldConfigurations[i].bShow = true;

			for(int i = EDeviceParameterMVTechMSPTrigger_OutputName; i < EDeviceParameterMVTechMSPTrigger_Count; ++i)
				m_vctParameterFieldConfigurations[i].bShow = false;

			m_vctParameterFieldConfigurations[EDeviceParameterMVTechMSPTrigger_ShowID].bShow = false;
		}
		break;
	case EDeviceMSPBoardType_DelayedQueue:
		{
			for(int i = EDeviceParameterMVTechMSPTrigger_UIInfo; i < EDeviceParameterMVTechMSPTrigger_Count; ++i)
				m_vctParameterFieldConfigurations[i].bShow = false;

			m_vctParameterFieldConfigurations[EDeviceParameterMVTechMSPTrigger_UIInfo].bShow = true;
			m_vctParameterFieldConfigurations[EDeviceParameterMVTechMSPTrigger_ShowID].bShow = true;
		}
		break;
	default:
		break;
	}
}

void CDeviceMVTechMSPDio::OnBnClickedOutLed(UINT nID)
{
	do
	{
		int nNumber = nID - EDeviceAddEnumeratedControlID_OutputLedStart;

		if((CAuthorityManager::GetDeviceControlPermission() <= CAuthorityManager::GetCurrentUserType()) && !CSequenceManager::IsRunning())
			WriteOutBit(nNumber, !ReadOutBit(nNumber));
	}
	while(false);
}

void CDeviceMVTechMSPDio::OnBnClickedReset(UINT nID)
{
	do
	{
		int nNumber = nID - EDeviceAddEnumeratedControlID_ResetButtonStart;

		if((CAuthorityManager::GetDeviceControlPermission() <= CAuthorityManager::GetCurrentUserType()) && !CSequenceManager::IsRunning())
			ResetDevice(nNumber);
	}
	while(false);
}

void CDeviceMVTechMSPDio::OnBnClickedEnableTrigger(UINT nID)
{
	do
	{
		int nNumber = nID - EDeviceAddEnumeratedControlID_EnableButtonStart;

		if((CAuthorityManager::GetDeviceControlPermission() <= CAuthorityManager::GetCurrentUserType()) && !CSequenceManager::IsRunning())
			EnableTrigger(nNumber);
	}
	while(false);
}

void CDeviceMVTechMSPDio::OnBnClickedDisableTrigger(UINT nID)
{
	do
	{
		int nNumber = nID - EDeviceAddEnumeratedControlID_DisableButtonStart;

		if((CAuthorityManager::GetDeviceControlPermission() <= CAuthorityManager::GetCurrentUserType()) && !CSequenceManager::IsRunning())
			DisableTrigger(nNumber);
	}
	while(false);
}


void CDeviceMVTechMSPDio::AllocateIO(long nInputSize, long nOutputSize)
{
	if(!m_pUsedInput)
		m_pUsedInput = new bool[nInputSize];

	if(!m_pStrInputName)
		m_pStrInputName = new CString[nInputSize];

	if(!m_pUsedOutput)
		m_pUsedOutput = new bool[nOutputSize];

	if(!m_pStrOutputName)
		m_pStrOutputName = new CString[nOutputSize];
}

void CDeviceMVTechMSPDio::FreeIO()
{
	if(m_pUsedInput)
		delete[] m_pUsedInput;
	m_pUsedInput = nullptr;

	if(m_pStrInputName)
		delete[] m_pStrInputName;
	m_pStrInputName = nullptr;

	if(m_pUsedOutput)
		delete[] m_pUsedOutput;
	m_pUsedOutput = nullptr;

	if(m_pStrOutputName)
		delete[] m_pStrOutputName;
	m_pStrOutputName = nullptr;
}

void CDeviceMVTechMSPDio::AllocateQueue(long nQueueSizeX, long nQueueSizeY)
{
	if(!m_pQueueValueCh1)
	{
		m_pQueueValueCh1 = new DWORD[nQueueSizeX];
		memset(m_pQueueValueCh1, 0, sizeof(DWORD) * nQueueSizeX);
	}

	if(!m_pQueueValueCh2)
	{
		m_pQueueValueCh2 = new DWORD[nQueueSizeY];
		memset(m_pQueueValueCh2, 0, sizeof(DWORD) * nQueueSizeY);
	}
}

void CDeviceMVTechMSPDio::FreeQueue()
{
	if(m_pQueueValueCh1)
		delete[] m_pQueueValueCh1;
	m_pQueueValueCh1 = nullptr;

	if(m_pQueueValueCh2)
		delete[] m_pQueueValueCh2;
	m_pQueueValueCh2 = nullptr;
}

void CDeviceMVTechMSPDio::AllocateEncoder(long nEncoderSize)
{
	if(!m_arrEncoderValue)
	{
		m_arrEncoderValue = new DWORD[nEncoderSize];
		memset(m_arrEncoderValue, 0, sizeof(DWORD) * nEncoderSize);
	}
}

void CDeviceMVTechMSPDio::FreeEncoder()
{
	if(m_arrEncoderValue)
		delete[] m_arrEncoderValue;
	m_arrEncoderValue = nullptr;
}

void CDeviceMVTechMSPDio::AllocateTrigger(long nTriggerSize)
{
	if(!m_arrTriggerValue)
	{
		m_arrTriggerValue = new DWORD[nTriggerSize];
		memset(m_arrTriggerValue, 0, sizeof(DWORD) * nTriggerSize);
	}
}

void CDeviceMVTechMSPDio::FreeTrigger()
{
	if(m_arrTriggerValue)
		delete[] m_arrTriggerValue;
	m_arrTriggerValue = nullptr;
}

BOOL CDeviceMVTechMSPDio::OnInitDialog()
{
	CDeviceDio::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	SetTimer(RAVID_TIMER_DIO, 100, nullptr);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDeviceMVTechMSPDio::OnDestroy()
{
	CDeviceDio::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	KillTimer(RAVID_TIMER_DIO);
}

#endif