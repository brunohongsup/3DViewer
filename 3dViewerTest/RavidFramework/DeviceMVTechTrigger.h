#pragma once

#include "DeviceTrigger.h"

namespace DeviceMVTechTrigger
{
	enum
	{
		MT_TRIGGER_MODULE_MIN = 0,
		MT_TRIGGER_MODULE_MAX = 2,

		GET_ENCODER = 0x04,				// Input pulse counter
		GET_TRIGGER = 0x08,				// Output pulse counter
		COUNT_CLEAR = 0x84,				// Counter reset

		TRIG_ENABLE = 0x88,				// Clockwise Enable : bit(0)=1, Clockwise Disable : bit(0)=0
		// Counter clockwise Enable : bit(1)=1, Counter clockwise Disable : bit(1)=0 
		SET_START_POS = 0x90,
		SET_END_POS = 0x94,
		SET_CYCLE = 0x8c,				// Upper 8Bit(FF)=On Time, Lower 8Bit(FF)=Cycle

		ENCODER_DIR = 0xa4,				// Clockwise = 0, Counter clockwise = 1

		// Position Trigger
		MAX_POSITION_REGISTER = 6,
		POSITION_REGISTER_1 = 0xc0,
		POSITION_REGISTER_2 = 0xc4,
		POSITION_REGISTER_3 = 0xc8,
		POSITION_REGISTER_4 = 0xcc,
		POSITION_REGISTER_5 = 0xd0,
		POSITION_REGISTER_6 = 0xd4,

		POSITION_REGISTER_STATUS = 0x40,
		POSITION_TRIGGER_COUNT = 0X44,

		INT_TRIG_ENABLE = 0x98,


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
}

namespace Ravid
{
	namespace Device
	{
		class AFX_EXT_CLASS CDeviceMVTechTrigger : public CDeviceTrigger
		{
		public:
			RavidUseDynamicCreation();

			RavidPreventCopySelf(CDeviceMVTechTrigger);

			DECLARE_DYNAMIC(CDeviceMVTechTrigger)
			
			CDeviceMVTechTrigger();
			virtual ~CDeviceMVTechTrigger();

			virtual EDeviceInitializeResult Initialize() override;
			virtual EDeviceTerminateResult Terminate() override;

			virtual bool LoadSettings() override;

			virtual bool EnableTrigger() override;
			virtual bool DisableTrigger() override;

			virtual bool IsTriggerEnabled() override;

			virtual long GetEncoderCount() override;
			virtual long GetTriggerCount() override;
			virtual bool ResetCounter() override;

			virtual EMVTTriggerGetFunction GetStartPosition(_Out_ int* pParam);
			virtual EMVTTriggerSetFunction SetStartPosition(_In_ int nParam);

			virtual EMVTTriggerGetFunction GetEndPosition(_Out_ int* pParam);
			virtual EMVTTriggerSetFunction SetEndPosition(_In_ int nParam);

			virtual EMVTTriggerGetFunction GetTriggerCycle(_Out_ int* pParam);
			virtual EMVTTriggerSetFunction SetTriggerCycle(_In_ int nParam);

			virtual EMVTTriggerGetFunction GetTriggerPulseDurationTime(_Out_ int* pParam);
			virtual EMVTTriggerSetFunction SetTriggerPulseDurationTime(_In_ int nParam);

			virtual EMVTTriggerGetFunction GetEncoderIncreasementDirection(_Out_ EDeviceMVTTriggerEncoderIncreasementDirection* pParam);
			virtual EMVTTriggerSetFunction SetEncoderIncreasementDirection(_In_ EDeviceMVTTriggerEncoderIncreasementDirection eParam);

			virtual EMVTTriggerGetFunction GetTriggerCountingDirection(_Out_ EDeviceMVTTriggerTriggerCountingDirection* pParam);
			virtual EMVTTriggerSetFunction SetTriggerCountingDirection(_In_ EDeviceMVTTriggerTriggerCountingDirection eParam);

			virtual void SetPCIVendorID(_In_ unsigned short vendorId);
			virtual void SetPCIDeviceID(_In_ unsigned short deviceId);

			virtual bool OnParameterChanged(_In_ int nParam, _In_ CString strValue) override;
		public:
			virtual BOOL OnInitDialog();
			afx_msg void OnDestroy();
			afx_msg void OnTimer(UINT_PTR nIDEvent);

		public:
			DECLARE_MESSAGE_MAP()

		protected:
			virtual bool DoesModuleExist();

			virtual bool DisplayCounter();

		protected:
			DWORD* m_arrDwCS = nullptr;
			DWORD* m_arrDwCSLen = nullptr;

			bool m_bTriggerEnabled = false;

			void* m_pDeviceObject = nullptr;

			unsigned short m_vendorId = 0x10b6;
			unsigned short m_deviceId = 0x9030;
		};
	}
}

