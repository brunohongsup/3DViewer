#pragma once
 
 enum
 {
 	MAX_OUT_PORT = 16,
 	MAX_IN_PORT = 16,
 	MAX_QUEUE_PORT = 144,
 	MAX_ENCODER_COUNT = 2,
 	MAX_TRIGGER_COUNT = 6,
 	MAX_CHANNEL_COUNT = 2,
 
 	MAX_CARD_NUMS = 6,
 	MAX_BASE_ADDR_NUMS = 4,
 	MAX_IO_BUF_SIZE = 1024,
 	LOCAL_CONFIG_REG_CNT = 22,
 
 	PCI_BASE_CS0 = 0,
 	PCI_BASE_CS1 = 1,
 	PCI_BASE_CS2 = 2,
 	PCI_BASE_CS3 = 3,
 
 	PCI_BASE_IO = 0,
 };
 
 namespace DelayedQueue
 {
 	enum
 	{
 		MAX_READ_COUNT = 3,
 	};
 
 	enum EReadID
 	{
 		ID_READING_1 = 0xa8,		//DATA(0)~DATA(5) 6bit
 		ID_READING_2 = 0xb8,
 		ID_READING_3 = 0xc8,
 	};
 }
 
 //SDC Nozzle
 namespace MSP_P16_E2T6_Q288
 {
 	enum EBoardInfo
 	{
 		EBoardInfo_Channel = 2,
 		EBoardInfo_Outport = 16,
 		EBoardInfo_Inport = 16,
 		EBoardInfo_QueueCount = 144,
 		EBoardInfo_QueueStatusCount = 6,
 		EBoardInfo_EncoderCount = 1,
 		EBoardInfo_TriggerCount = 3,
 	};
 
 	enum EQueueStatus
 	{
 		EQueueStatus_CH1_Cam1Queue1 = 0x40,
 		EQueueStatus_CH1_Cam1Queue2 = 0x44,
 		EQueueStatus_CH1_Cam2Queue1 = 0x220,
 		EQueueStatus_CH1_Cam2Queue2 = 0x224,
 		EQueueStatus_CH1_Cam3Queue1 = 0x2F0,
 		EQueueStatus_CH1_Cam3Queue2 = 0x2F4,
 
 		EQueueStatus_CH2_Cam1Queue1 = 0x3C0,
 		EQueueStatus_CH2_Cam1Queue2 = 0x3C4,
 		EQueueStatus_CH2_Cam2Queue1 = 0x4F0,
 		EQueueStatus_CH2_Cam2Queue2 = 0x4F4,
 		EQueueStatus_CH2_Cam3Queue1 = 0x5C0,
 		EQueueStatus_CH2_Cam3Queue2 = 0x5C4,
 	};
 
 	enum EQueueRegister
 	{
 		EQueueRegister_CH1_Cam1Queue1_1 = 0xC0,
 		EQueueRegister_CH1_Cam1Queue1_2 = 0xC4,
 		EQueueRegister_CH1_Cam1Queue1_3 = 0xC8,
 		EQueueRegister_CH1_Cam1Queue1_4 = 0xCC,
 		EQueueRegister_CH1_Cam1Queue1_5 = 0xD0,
 		EQueueRegister_CH1_Cam1Queue1_6 = 0xD4,
 		EQueueRegister_CH1_Cam1Queue1_7 = 0xD8,
 		EQueueRegister_CH1_Cam1Queue1_8 = 0xDC,
 		EQueueRegister_CH1_Cam1Queue1_9 = 0xE0,
 		EQueueRegister_CH1_Cam1Queue1_10 = 0xE4,
 		EQueueRegister_CH1_Cam1Queue1_11 = 0xE8,
 		EQueueRegister_CH1_Cam1Queue1_12 = 0xEC,
 		EQueueRegister_CH1_Cam1Queue1_13 = 0xF0,
 		EQueueRegister_CH1_Cam1Queue1_14 = 0xF4,
 		EQueueRegister_CH1_Cam1Queue1_15 = 0xF8,
 		EQueueRegister_CH1_Cam1Queue1_16 = 0xFC,
 		EQueueRegister_CH1_Cam1Queue1_17 = 0x200,
 		EQueueRegister_CH1_Cam1Queue1_18 = 0x204,
 		EQueueRegister_CH1_Cam1Queue1_19 = 0x208,
 		EQueueRegister_CH1_Cam1Queue1_20 = 0x20C,
 		EQueueRegister_CH1_Cam1Queue1_21 = 0x210,
 		EQueueRegister_CH1_Cam1Queue1_22 = 0x214,
 		EQueueRegister_CH1_Cam1Queue1_23 = 0x218,
 		EQueueRegister_CH1_Cam1Queue1_24 = 0x21C,
 
 		EQueueRegister_CH1_Cam1Queue2_1 = 0x60,
 		EQueueRegister_CH1_Cam1Queue2_2 = 0x64,
 		EQueueRegister_CH1_Cam1Queue2_3 = 0x68,
 		EQueueRegister_CH1_Cam1Queue2_4 = 0x6C,
 		EQueueRegister_CH1_Cam1Queue2_5 = 0x70,
 		EQueueRegister_CH1_Cam1Queue2_6 = 0x74,
 		EQueueRegister_CH1_Cam1Queue2_7 = 0x78,
 		EQueueRegister_CH1_Cam1Queue2_8 = 0x7C,
 		EQueueRegister_CH1_Cam1Queue2_9 = 0x120,
 		EQueueRegister_CH1_Cam1Queue2_10 = 0x124,
 		EQueueRegister_CH1_Cam1Queue2_11 = 0x128,
 		EQueueRegister_CH1_Cam1Queue2_12 = 0x12C,
 		EQueueRegister_CH1_Cam1Queue2_13 = 0x130,
 		EQueueRegister_CH1_Cam1Queue2_14 = 0x134,
 		EQueueRegister_CH1_Cam1Queue2_15 = 0x138,
 		EQueueRegister_CH1_Cam1Queue2_16 = 0x13C,
 		EQueueRegister_CH1_Cam1Queue2_17 = 0x140,
 		EQueueRegister_CH1_Cam1Queue2_18 = 0x144,
 		EQueueRegister_CH1_Cam1Queue2_19 = 0x148,
 		EQueueRegister_CH1_Cam1Queue2_20 = 0x14C,
 		EQueueRegister_CH1_Cam1Queue2_21 = 0x150,
 		EQueueRegister_CH1_Cam1Queue2_22 = 0x154,
 		EQueueRegister_CH1_Cam1Queue2_23 = 0x158,
 		EQueueRegister_CH1_Cam1Queue2_24 = 0x15C,
 
 		EQueueRegister_CH1_Cam2Queue1_1 = 0x290,
 		EQueueRegister_CH1_Cam2Queue1_2 = 0x294,
 		EQueueRegister_CH1_Cam2Queue1_3 = 0x298,
 		EQueueRegister_CH1_Cam2Queue1_4 = 0x29C,
 		EQueueRegister_CH1_Cam2Queue1_5 = 0x2A0,
 		EQueueRegister_CH1_Cam2Queue1_6 = 0x2A4,
 		EQueueRegister_CH1_Cam2Queue1_7 = 0x2A8,
 		EQueueRegister_CH1_Cam2Queue1_8 = 0x2AC,
 		EQueueRegister_CH1_Cam2Queue1_9 = 0x2B0,
 		EQueueRegister_CH1_Cam2Queue1_10 = 0x2B4,
 		EQueueRegister_CH1_Cam2Queue1_11 = 0x2B8,
 		EQueueRegister_CH1_Cam2Queue1_12 = 0x2BC,
 		EQueueRegister_CH1_Cam2Queue1_13 = 0x2C0,
 		EQueueRegister_CH1_Cam2Queue1_14 = 0x2C4,
 		EQueueRegister_CH1_Cam2Queue1_15 = 0x2C8,
 		EQueueRegister_CH1_Cam2Queue1_16 = 0x2CC,
 		EQueueRegister_CH1_Cam2Queue1_17 = 0x2D0,
 		EQueueRegister_CH1_Cam2Queue1_18 = 0x2D4,
 		EQueueRegister_CH1_Cam2Queue1_19 = 0x2D8,
 		EQueueRegister_CH1_Cam2Queue1_20 = 0x2DC,
 		EQueueRegister_CH1_Cam2Queue1_21 = 0x2E0,
 		EQueueRegister_CH1_Cam2Queue1_22 = 0x2E4,
 		EQueueRegister_CH1_Cam2Queue1_23 = 0x2E8,
 		EQueueRegister_CH1_Cam2Queue1_24 = 0x2EC,
 
 		EQueueRegister_CH1_Cam2Queue2_1 = 0x230,
 		EQueueRegister_CH1_Cam2Queue2_2 = 0x234,
 		EQueueRegister_CH1_Cam2Queue2_3 = 0x238,
 		EQueueRegister_CH1_Cam2Queue2_4 = 0x23C,
 		EQueueRegister_CH1_Cam2Queue2_5 = 0x240,
 		EQueueRegister_CH1_Cam2Queue2_6 = 0x244,
 		EQueueRegister_CH1_Cam2Queue2_7 = 0x248,
 		EQueueRegister_CH1_Cam2Queue2_8 = 0x24C,
 		EQueueRegister_CH1_Cam2Queue2_9 = 0x250,
 		EQueueRegister_CH1_Cam2Queue2_10 = 0x254,
 		EQueueRegister_CH1_Cam2Queue2_11 = 0x258,
 		EQueueRegister_CH1_Cam2Queue2_12 = 0x25C,
 		EQueueRegister_CH1_Cam2Queue2_13 = 0x260,
 		EQueueRegister_CH1_Cam2Queue2_14 = 0x264,
 		EQueueRegister_CH1_Cam2Queue2_15 = 0x268,
 		EQueueRegister_CH1_Cam2Queue2_16 = 0x26C,
 		EQueueRegister_CH1_Cam2Queue2_17 = 0x270,
 		EQueueRegister_CH1_Cam2Queue2_18 = 0x274,
 		EQueueRegister_CH1_Cam2Queue2_19 = 0x278,
 		EQueueRegister_CH1_Cam2Queue2_20 = 0x27C,
 		EQueueRegister_CH1_Cam2Queue2_21 = 0x280,
 		EQueueRegister_CH1_Cam2Queue2_22 = 0x284,
 		EQueueRegister_CH1_Cam2Queue2_23 = 0x288,
 		EQueueRegister_CH1_Cam2Queue2_24 = 0x28C,
 
 		EQueueRegister_CH1_Cam3Queue1_1 = 0x360,
 		EQueueRegister_CH1_Cam3Queue1_2 = 0x364,
 		EQueueRegister_CH1_Cam3Queue1_3 = 0x368,
 		EQueueRegister_CH1_Cam3Queue1_4 = 0x36C,
 		EQueueRegister_CH1_Cam3Queue1_5 = 0x370,
 		EQueueRegister_CH1_Cam3Queue1_6 = 0x374,
 		EQueueRegister_CH1_Cam3Queue1_7 = 0x378,
 		EQueueRegister_CH1_Cam3Queue1_8 = 0x37C,
 		EQueueRegister_CH1_Cam3Queue1_9 = 0x380,
 		EQueueRegister_CH1_Cam3Queue1_10 = 0x384,
 		EQueueRegister_CH1_Cam3Queue1_11 = 0x388,
 		EQueueRegister_CH1_Cam3Queue1_12 = 0x38C,
 		EQueueRegister_CH1_Cam3Queue1_13 = 0x390,
 		EQueueRegister_CH1_Cam3Queue1_14 = 0x394,
 		EQueueRegister_CH1_Cam3Queue1_15 = 0x398,
 		EQueueRegister_CH1_Cam3Queue1_16 = 0x39C,
 		EQueueRegister_CH1_Cam3Queue1_17 = 0x3A0,
 		EQueueRegister_CH1_Cam3Queue1_18 = 0x3A4,
 		EQueueRegister_CH1_Cam3Queue1_19 = 0x3A8,
 		EQueueRegister_CH1_Cam3Queue1_20 = 0x3AC,
 		EQueueRegister_CH1_Cam3Queue1_21 = 0x3B0,
 		EQueueRegister_CH1_Cam3Queue1_22 = 0x3B4,
 		EQueueRegister_CH1_Cam3Queue1_23 = 0x3B8,
 		EQueueRegister_CH1_Cam3Queue1_24 = 0x3BC,
 
 		EQueueRegister_CH1_Cam3Queue2_1 = 0x300,
 		EQueueRegister_CH1_Cam3Queue2_2 = 0x304,
 		EQueueRegister_CH1_Cam3Queue2_3 = 0x308,
 		EQueueRegister_CH1_Cam3Queue2_4 = 0x30C,
 		EQueueRegister_CH1_Cam3Queue2_5 = 0x310,
 		EQueueRegister_CH1_Cam3Queue2_6 = 0x314,
 		EQueueRegister_CH1_Cam3Queue2_7 = 0x318,
 		EQueueRegister_CH1_Cam3Queue2_8 = 0x31C,
 		EQueueRegister_CH1_Cam3Queue2_9 = 0x320,
 		EQueueRegister_CH1_Cam3Queue2_10 = 0x324,
 		EQueueRegister_CH1_Cam3Queue2_11 = 0x328,
 		EQueueRegister_CH1_Cam3Queue2_12 = 0x32C,
 		EQueueRegister_CH1_Cam3Queue2_13 = 0x330,
 		EQueueRegister_CH1_Cam3Queue2_14 = 0x334,
 		EQueueRegister_CH1_Cam3Queue2_15 = 0x338,
 		EQueueRegister_CH1_Cam3Queue2_16 = 0x33C,
 		EQueueRegister_CH1_Cam3Queue2_17 = 0x340,
 		EQueueRegister_CH1_Cam3Queue2_18 = 0x344,
 		EQueueRegister_CH1_Cam3Queue2_19 = 0x348,
 		EQueueRegister_CH1_Cam3Queue2_20 = 0x34C,
 		EQueueRegister_CH1_Cam3Queue2_21 = 0x350,
 		EQueueRegister_CH1_Cam3Queue2_22 = 0x354,
 		EQueueRegister_CH1_Cam3Queue2_23 = 0x358,
 		EQueueRegister_CH1_Cam3Queue2_24 = 0x35C,
 
 		EQueueRegister_CH2_Cam1Queue1_1 = 0x430,
 		EQueueRegister_CH2_Cam1Queue1_2 = 0x434,
 		EQueueRegister_CH2_Cam1Queue1_3 = 0x438,
 		EQueueRegister_CH2_Cam1Queue1_4 = 0x43C,
 		EQueueRegister_CH2_Cam1Queue1_5 = 0x440,
 		EQueueRegister_CH2_Cam1Queue1_6 = 0x444,
 		EQueueRegister_CH2_Cam1Queue1_7 = 0x448,
 		EQueueRegister_CH2_Cam1Queue1_8 = 0x44C,
 		EQueueRegister_CH2_Cam1Queue1_9 = 0x450,
 		EQueueRegister_CH2_Cam1Queue1_10 = 0x454,
 		EQueueRegister_CH2_Cam1Queue1_11 = 0x458,
 		EQueueRegister_CH2_Cam1Queue1_12 = 0x45C,
 		EQueueRegister_CH2_Cam1Queue1_13 = 0x460,
 		EQueueRegister_CH2_Cam1Queue1_14 = 0x464,
 		EQueueRegister_CH2_Cam1Queue1_15 = 0x468,
 		EQueueRegister_CH2_Cam1Queue1_16 = 0x46C,
 		EQueueRegister_CH2_Cam1Queue1_17 = 0x470,
 		EQueueRegister_CH2_Cam1Queue1_18 = 0x474,
 		EQueueRegister_CH2_Cam1Queue1_19 = 0x478,
 		EQueueRegister_CH2_Cam1Queue1_20 = 0x47C,
 		EQueueRegister_CH2_Cam1Queue1_21 = 0x480,
 		EQueueRegister_CH2_Cam1Queue1_22 = 0x484,
 		EQueueRegister_CH2_Cam1Queue1_23 = 0x488,
 		EQueueRegister_CH2_Cam1Queue1_24 = 0x48C,
 
 		EQueueRegister_CH2_Cam1Queue2_1 = 0x3D0,
 		EQueueRegister_CH2_Cam1Queue2_2 = 0x3D4,
 		EQueueRegister_CH2_Cam1Queue2_3 = 0x3D8,
 		EQueueRegister_CH2_Cam1Queue2_4 = 0x3DC,
 		EQueueRegister_CH2_Cam1Queue2_5 = 0x3E0,
 		EQueueRegister_CH2_Cam1Queue2_6 = 0x3E4,
 		EQueueRegister_CH2_Cam1Queue2_7 = 0x3E8,
 		EQueueRegister_CH2_Cam1Queue2_8 = 0x3EC,
 		EQueueRegister_CH2_Cam1Queue2_9 = 0x3F0,
 		EQueueRegister_CH2_Cam1Queue2_10 = 0x3F4,
 		EQueueRegister_CH2_Cam1Queue2_11 = 0x3F8,
 		EQueueRegister_CH2_Cam1Queue2_12 = 0x3FC,
 		EQueueRegister_CH2_Cam1Queue2_13 = 0x400,
 		EQueueRegister_CH2_Cam1Queue2_14 = 0x404,
 		EQueueRegister_CH2_Cam1Queue2_15 = 0x408,
 		EQueueRegister_CH2_Cam1Queue2_16 = 0x40C,
 		EQueueRegister_CH2_Cam1Queue2_17 = 0x410,
 		EQueueRegister_CH2_Cam1Queue2_18 = 0x414,
 		EQueueRegister_CH2_Cam1Queue2_19 = 0x418,
 		EQueueRegister_CH2_Cam1Queue2_20 = 0x41C,
 		EQueueRegister_CH2_Cam1Queue2_21 = 0x420,
 		EQueueRegister_CH2_Cam1Queue2_22 = 0x424,
 		EQueueRegister_CH2_Cam1Queue2_23 = 0x428,
 		EQueueRegister_CH2_Cam1Queue2_24 = 0x42C,
 
 		EQueueRegister_CH2_Cam2Queue1_1 = 0x560,
 		EQueueRegister_CH2_Cam2Queue1_2 = 0x564,
 		EQueueRegister_CH2_Cam2Queue1_3 = 0x568,
 		EQueueRegister_CH2_Cam2Queue1_4 = 0x56C,
 		EQueueRegister_CH2_Cam2Queue1_5 = 0x570,
 		EQueueRegister_CH2_Cam2Queue1_6 = 0x574,
 		EQueueRegister_CH2_Cam2Queue1_7 = 0x578,
 		EQueueRegister_CH2_Cam2Queue1_8 = 0x57C,
 		EQueueRegister_CH2_Cam2Queue1_9 = 0x580,
 		EQueueRegister_CH2_Cam2Queue1_10 = 0x584,
 		EQueueRegister_CH2_Cam2Queue1_11 = 0x588,
 		EQueueRegister_CH2_Cam2Queue1_12 = 0x58C,
 		EQueueRegister_CH2_Cam2Queue1_13 = 0x590,
 		EQueueRegister_CH2_Cam2Queue1_14 = 0x594,
 		EQueueRegister_CH2_Cam2Queue1_15 = 0x598,
 		EQueueRegister_CH2_Cam2Queue1_16 = 0x59C,
 		EQueueRegister_CH2_Cam2Queue1_17 = 0x5A0,
 		EQueueRegister_CH2_Cam2Queue1_18 = 0x5A4,
 		EQueueRegister_CH2_Cam2Queue1_19 = 0x5A8,
 		EQueueRegister_CH2_Cam2Queue1_20 = 0x5AC,
 		EQueueRegister_CH2_Cam2Queue1_21 = 0x5B0,
 		EQueueRegister_CH2_Cam2Queue1_22 = 0x5B4,
 		EQueueRegister_CH2_Cam2Queue1_23 = 0x5B8,
 		EQueueRegister_CH2_Cam2Queue1_24 = 0x5BC,
 
 		EQueueRegister_CH2_Cam2Queue2_1 = 0x500,
 		EQueueRegister_CH2_Cam2Queue2_2 = 0x504,
 		EQueueRegister_CH2_Cam2Queue2_3 = 0x508,
 		EQueueRegister_CH2_Cam2Queue2_4 = 0x50C,
 		EQueueRegister_CH2_Cam2Queue2_5 = 0x510,
 		EQueueRegister_CH2_Cam2Queue2_6 = 0x514,
 		EQueueRegister_CH2_Cam2Queue2_7 = 0x518,
 		EQueueRegister_CH2_Cam2Queue2_8 = 0x51C,
 		EQueueRegister_CH2_Cam2Queue2_9 = 0x520,
 		EQueueRegister_CH2_Cam2Queue2_10 = 0x524,
 		EQueueRegister_CH2_Cam2Queue2_11 = 0x528,
 		EQueueRegister_CH2_Cam2Queue2_12 = 0x52C,
 		EQueueRegister_CH2_Cam2Queue2_13 = 0x530,
 		EQueueRegister_CH2_Cam2Queue2_14 = 0x534,
 		EQueueRegister_CH2_Cam2Queue2_15 = 0x538,
 		EQueueRegister_CH2_Cam2Queue2_16 = 0x53C,
 		EQueueRegister_CH2_Cam2Queue2_17 = 0x540,
 		EQueueRegister_CH2_Cam2Queue2_18 = 0x544,
 		EQueueRegister_CH2_Cam2Queue2_19 = 0x548,
 		EQueueRegister_CH2_Cam2Queue2_20 = 0x54C,
 		EQueueRegister_CH2_Cam2Queue2_21 = 0x550,
 		EQueueRegister_CH2_Cam2Queue2_22 = 0x554,
 		EQueueRegister_CH2_Cam2Queue2_23 = 0x558,
 		EQueueRegister_CH2_Cam2Queue2_24 = 0x55C,
 
 		EQueueRegister_CH2_Cam3Queue1_1 = 0x630,
 		EQueueRegister_CH2_Cam3Queue1_2 = 0x634,
 		EQueueRegister_CH2_Cam3Queue1_3 = 0x638,
 		EQueueRegister_CH2_Cam3Queue1_4 = 0x63C,
 		EQueueRegister_CH2_Cam3Queue1_5 = 0x640,
 		EQueueRegister_CH2_Cam3Queue1_6 = 0x644,
 		EQueueRegister_CH2_Cam3Queue1_7 = 0x648,
 		EQueueRegister_CH2_Cam3Queue1_8 = 0x64C,
 		EQueueRegister_CH2_Cam3Queue1_9 = 0x650,
 		EQueueRegister_CH2_Cam3Queue1_10 = 0x654,
 		EQueueRegister_CH2_Cam3Queue1_11 = 0x658,
 		EQueueRegister_CH2_Cam3Queue1_12 = 0x65C,
 		EQueueRegister_CH2_Cam3Queue1_13 = 0x660,
 		EQueueRegister_CH2_Cam3Queue1_14 = 0x664,
 		EQueueRegister_CH2_Cam3Queue1_15 = 0x668,
 		EQueueRegister_CH2_Cam3Queue1_16 = 0x66C,
 		EQueueRegister_CH2_Cam3Queue1_17 = 0x670,
 		EQueueRegister_CH2_Cam3Queue1_18 = 0x674,
 		EQueueRegister_CH2_Cam3Queue1_19 = 0x678,
 		EQueueRegister_CH2_Cam3Queue1_20 = 0x67C,
 		EQueueRegister_CH2_Cam3Queue1_21 = 0x680,
 		EQueueRegister_CH2_Cam3Queue1_22 = 0x684,
 		EQueueRegister_CH2_Cam3Queue1_23 = 0x688,
 		EQueueRegister_CH2_Cam3Queue1_24 = 0x68C,
 
 		EQueueRegister_CH2_Cam3Queue2_1 = 0x5D0,
 		EQueueRegister_CH2_Cam3Queue2_2 = 0x5D4,
 		EQueueRegister_CH2_Cam3Queue2_3 = 0x5D8,
 		EQueueRegister_CH2_Cam3Queue2_4 = 0x5DC,
 		EQueueRegister_CH2_Cam3Queue2_5 = 0x5E0,
 		EQueueRegister_CH2_Cam3Queue2_6 = 0x5E4,
 		EQueueRegister_CH2_Cam3Queue2_7 = 0x5E8,
 		EQueueRegister_CH2_Cam3Queue2_8 = 0x5EC,
 		EQueueRegister_CH2_Cam3Queue2_9 = 0x5F0,
 		EQueueRegister_CH2_Cam3Queue2_10 = 0x5F4,
 		EQueueRegister_CH2_Cam3Queue2_11 = 0x5F8,
 		EQueueRegister_CH2_Cam3Queue2_12 = 0x5FC,
 		EQueueRegister_CH2_Cam3Queue2_13 = 0x600,
 		EQueueRegister_CH2_Cam3Queue2_14 = 0x604,
 		EQueueRegister_CH2_Cam3Queue2_15 = 0x608,
 		EQueueRegister_CH2_Cam3Queue2_16 = 0x60C,
 		EQueueRegister_CH2_Cam3Queue2_17 = 0x610,
 		EQueueRegister_CH2_Cam3Queue2_18 = 0x614,
 		EQueueRegister_CH2_Cam3Queue2_19 = 0x618,
 		EQueueRegister_CH2_Cam3Queue2_20 = 0x61C,
 		EQueueRegister_CH2_Cam3Queue2_21 = 0x620,
 		EQueueRegister_CH2_Cam3Queue2_22 = 0x624,
 		EQueueRegister_CH2_Cam3Queue2_23 = 0x628,
 		EQueueRegister_CH2_Cam3Queue2_24 = 0x62C,
 	};
 
 	enum EIOAddress
 	{
 		EIOAddress_Input = 0x00,
 		EIOAddress_Output = 0x80,
 	};
 
 	enum EEncoderStartAddress
 	{
 		EEncoderStartAddress_CH1 = 0x20,
 		EEncoderStartAddress_CH2 = 0x24,
 	};
 
 	enum EEncoderAddress
 	{
 		EEncoderAddress_CH1 = 0x04,
 		EEncoderAddress_CH2 = 0x104,
 	};
 
 	enum ETriggerAddress
 	{
 		ETriggerAddress_CH1_Cam1 = 0x08,
 		ETriggerAddress_CH1_Cam2 = 0x0C,
 		ETriggerAddress_CH1_Cam3 = 0x10,
 
 		ETriggerAddress_CH2_Cam1 = 0x108,
 		ETriggerAddress_CH2_Cam2 = 0x10C,
 		ETriggerAddress_CH2_Cam3 = 0x110,
 	};
 
 	enum EResetAddress
 	{
 		EResetAddress_CH1 = 0x84,
 		EResetAddress_CH2 = 0x184,
 	};
 
 	enum EDirectionAddress
 	{
 		EDirectionAddress_CH1 = 0x88,
 		EDirectionAddress_CH2 = 0x188,
 	};
 
 	enum ELiveModeAddress
 	{
 		ELiveModeAddress_CH1 = 0xA0,
 		ELiveModeAddress_CH2 = 0xA8,
 	};
 
 	enum EGrabAddress
 	{
 		EGrabAddress_CH1 = 0xA4,
 		EGrabAddress_CH2 = 0xAC,
 	};
 }
 //Powerlogics IR Filter 
 namespace MSP_P16_E2T2_Q16_1
 {
 	enum EBoardInfo
 	{
 		EBoardInfo_Channel = 2,
 		EBoardInfo_Outport = 16,
 		EBoardInfo_Inport = 16,
 		EBoardInfo_QueueCount = 8,
 		EBoardInfo_QueueStatusCount = 1,
 		EBoardInfo_EncoderCount = 1,
 		EBoardInfo_TriggerCount = 1,
 	};
 
 	enum EIOAddress
 	{
 		EIOAddress_Input = 0x00,
 		EIOAddress_Output = 0x80,
 	};
 
 	enum EQueueStatus
 	{
 		EQueueStatus_CH1_Queue = 0x40,
 		EQueueStatus_CH2_Queue = 0x140,
 	};
 
 	enum EQueueRegister
 	{
 		EQueueRegister_CH1_Queue1 = 0xC0,
 		EQueueRegister_CH1_Queue2 = 0xC4,
 		EQueueRegister_CH1_Queue3 = 0xC8,
 		EQueueRegister_CH1_Queue4 = 0xCC,
 		EQueueRegister_CH1_Queue5 = 0xD0,
 		EQueueRegister_CH1_Queue6 = 0xD4,
 		EQueueRegister_CH1_Queue7 = 0xD8,
 		EQueueRegister_CH1_Queue8 = 0xDC,
 
 		EQueueRegister_CH2_Queue1 = 0x1C0,
 		EQueueRegister_CH2_Queue2 = 0x1C4,
 		EQueueRegister_CH2_Queue3 = 0x1C8,
 		EQueueRegister_CH2_Queue4 = 0x1CC,
 		EQueueRegister_CH2_Queue5 = 0x1D0,
 		EQueueRegister_CH2_Queue6 = 0x1D4,
 		EQueueRegister_CH2_Queue7 = 0x1D8,
 		EQueueRegister_CH2_Queue8 = 0x1DC,
 	};
 
 	enum EQueueOrder
 	{
 		EQueueOrder_CH1_Queue1 = 0x200,
 		EQueueOrder_CH1_Queue2 = 0x204,
 		EQueueOrder_CH1_Queue3 = 0x208,
 		EQueueOrder_CH1_Queue4 = 0x20C,
 		EQueueOrder_CH1_Queue5 = 0x210,
 		EQueueOrder_CH1_Queue6 = 0x214,
 		EQueueOrder_CH1_Queue7 = 0x218,
 		EQueueOrder_CH1_Queue8 = 0x21C,
 
 		EQueueOrder_CH2_Queue1 = 0x300,
 		EQueueOrder_CH2_Queue2 = 0x304,
 		EQueueOrder_CH2_Queue3 = 0x308,
 		EQueueOrder_CH2_Queue4 = 0x30C,
 		EQueueOrder_CH2_Queue5 = 0x310,
 		EQueueOrder_CH2_Queue6 = 0x314,
 		EQueueOrder_CH2_Queue7 = 0x318,
 		EQueueOrder_CH2_Queue8 = 0x31C,
 	};
 
 	enum EEncoderStartAddress
 	{
 		EEncoderStartAddress_CH1 = 0x90,
 		EEncoderStartAddress_CH2 = 0x190,
 	};
 
 	enum EEncoderEndAddress
 	{
 		EEncoderEndAddress_CH1 = 0x94,
 		EEncoderEndAddress_CH2 = 0x194,
 	};
 
 	enum EEncoderAddress
 	{
 		EEncoderAddress_CH1 = 0x04,
 		EEncoderAddress_CH2 = 0x104,
 	};
 
 	enum ETriggerAddress
 	{
 		ETriggerAddress_CH1 = 0x08,
 		ETriggerAddress_CH2 = 0x108,
 	};
 
 	enum EResetAddress
 	{
 		EResetAddress_CH1 = 0x84,
 		EResetAddress_CH2 = 0x184,
 	};
 
 	enum EDirectionAddress
 	{
 		EDirectionAddress_CH1 = 0xA4,
 		EDirectionAddress_CH2 = 0x1A4,
 	};
 
 	enum ETriggerEnableAddress
 	{
 		ETriggerEnableAddress_CH1 = 0x88,
 		ETriggerEnableAddress_CH2 = 0x188,
 	};
 
 	enum ETriggerCycleAddress
 	{
 		ETriggerCycleAddress_CH1 = 0x8C,
 		ETriggerCycleAddress_CH2 = 0x18C,
 	};
 
 	enum EStrobeExposerTimeAddress
 	{
 		EStrobeExposerTimeAddress_CH1 = 0x98,
 		EStrobeExposerTimeAddress_CH2 = 0x198,
 	};
 
 	enum EAreaCameraCounterReadAddress
 	{
 		EAreaCameraCounterReadAddress_CH1 = 0x48,
 		EAreaCameraCounterReadAddress_CH2 = 0x148,
 	};
 
 	enum EAreaCameraCounterWriteAddress
 	{
 		EAreaCameraCounterWriteAddress_CH1 = 0xE0,
 		EAreaCameraCounterWriteAddress_CH2 = 0x1E0,
 	};
 
 	enum EAreaCameraTriggerCounterAddress
 	{
 		EAreaCameraTriggerCounterAddress_CH1 = 0x44,
 		EAreaCameraTriggerCounterAddress_CH2 = 0x144,
 	};
 
 	enum EAreaLiveModeAddress
 	{
 		EAreaLiveModeAddress_CH1 = 0xA0,
 		EAreaLiveModeAddress_CH2 = 0x1A0,
 	};
 
 	enum EUsingTriggerAddress
 	{
 		EUsingTriggerAddress_CH1 = 0x2D0,
 		EUsingTriggerAddress_CH2 = 0x2D4,
 	};
 
 	enum EGrabAddress
 	{
 		EGrabAddress_CH1 = 0x400,
 		EGrabAddress_CH2 = 0x500,
 	};
 }
 //Meere Pol cutting
 namespace MSP_P16_E2T2_Q16_2
 {
 	enum EBoardInfo
 	{
 		EBoardInfo_Channel = 2,
 		EBoardInfo_Outport = 0,
 		EBoardInfo_Inport = 16,
 		EBoardInfo_QueueCount = 8,
 		EBoardInfo_QueueStatusCount = 1,
 		EBoardInfo_EncoderCount = 1,
 		EBoardInfo_TriggerCount = 1,
 	};
 
 	enum EIOAddress
 	{
 		EIOAddress_Input = 0x00,
 		EIOAddress_Output = 0x80,
 	};
 
 	enum EQueueStatus
 	{
 		EQueueStatus_CH1_Queue = 0x40,
 		EQueueStatus_CH2_Queue = 0x140,
 	};
 
 	enum EQueueRegister
 	{
 		EQueueRegister_CH1_Queue1 = 0xC0,
 		EQueueRegister_CH1_Queue2 = 0xC4,
 		EQueueRegister_CH1_Queue3 = 0xC8,
 		EQueueRegister_CH1_Queue4 = 0xCC,
 		EQueueRegister_CH1_Queue5 = 0xD0,
 		EQueueRegister_CH1_Queue6 = 0xD4,
 		EQueueRegister_CH1_Queue7 = 0xD8,
 		EQueueRegister_CH1_Queue8 = 0xDC,
 
 		EQueueRegister_CH2_Queue1 = 0x1C0,
 		EQueueRegister_CH2_Queue2 = 0x1C4,
 		EQueueRegister_CH2_Queue3 = 0x1C8,
 		EQueueRegister_CH2_Queue4 = 0x1CC,
 		EQueueRegister_CH2_Queue5 = 0x1D0,
 		EQueueRegister_CH2_Queue6 = 0x1D4,
 		EQueueRegister_CH2_Queue7 = 0x1D8,
 		EQueueRegister_CH2_Queue8 = 0x1DC,
 	};
 
 	enum EEncoderStartAddress
 	{
 		EEncoderStartAddress_CH1 = 0x90,
 		EEncoderStartAddress_CH2 = 0x190,
 	};
 
 	enum EEncoderEndAddress
 	{
 		EEncoderEndAddress_CH1 = 0x94,
 		EEncoderEndAddress_CH2 = 0x194,
 	};
 
 	enum EEncoderAddress
 	{
 		EEncoderAddress_CH1 = 0x04,
 		EEncoderAddress_CH2 = 0x104,
 	};
 
 	enum ETriggerAddress
 	{
 		ETriggerAddress_CH1 = 0x08,
 		ETriggerAddress_CH2 = 0x108,
 	};
 
 	enum EResetAddress
 	{
 		EResetAddress_CH1 = 0x84,
 		EResetAddress_CH2 = 0x184,
 	};
 
 	enum EDirectionAddress
 	{
 		EDirectionAddress_CH1 = 0xA4,
 		EDirectionAddress_CH2 = 0x1A4,
 	};
 
 	enum ETriggerEnableAddress
 	{
 		ETriggerEnableAddress_CH1 = 0x88,
 		ETriggerEnableAddress_CH2 = 0x188,
 	};
 
 	enum ETriggerCycleAddress
 	{
 		ETriggerCycleAddress_CH1 = 0x8C,
 		ETriggerCycleAddress_CH2 = 0x18C,
 	};
 
 	enum EStrobeExposerTimeAddress
 	{
 		EStrobeExposerTimeAddress_CH1 = 0x98,
 		EStrobeExposerTimeAddress_CH2 = 0x198,
 	};
 
 	enum EAreaCameraCounterReadAddress
 	{
 		EAreaCameraCounterReadAddress_CH1 = 0x48,
 		EAreaCameraCounterReadAddress_CH2 = 0x148,
 	};
 
 	enum EAreaCameraCounterWriteAddress
 	{
 		EAreaCameraCounterWriteAddress_CH1 = 0xE0,
 		EAreaCameraCounterWriteAddress_CH2 = 0x1E0,
 	};
 
 	enum EAreaCameraTriggerCounterAddress
 	{
 		EAreaCameraTriggerCounterAddress_CH1 = 0x44,
 		EAreaCameraTriggerCounterAddress_CH2 = 0x144,
 	};
 
 	enum EAreaLiveModeAddress
 	{
 		EAreaLiveModeAddress_CH1 = 0xA0,
 		EAreaLiveModeAddress_CH2 = 0x1A0,
 	};
 
 	enum EUsingTriggerAddress
 	{
 		EUsingTriggerAddress_CH1 = 0x2D0,
 		EUsingTriggerAddress_CH2 = 0x2D4,
 	};
 
 	enum EGrabAddress
 	{
 		EGrabAddress_CH1 = 0x2D8,
 		EGrabAddress_CH2 = 0x2DC,
 	};
 }
