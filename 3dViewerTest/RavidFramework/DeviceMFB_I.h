#pragma once

#include "DeviceDio.h"

namespace CDeviceMFB_I_DIO
{
	enum
	{
		MFB_IO_IN_READ = 0x00,
		MFB_IO_OUT_WRITE = 0x80,
		MFB_IO_OUT_READ = 0x0C,

		MFB_IO_MODULE_MIN = 0,
		MFB_IO_MODULE_MAX = 2,

		MFB_IO_OUT_PORT = 32,
		MFB_IO_IN_PORT = 32,

		MFB_MAX_CARD_NUMS = 6,
		MFB_MAX_ADDR_NUMS = 4,
		MFB_MAX_IO_BUFSIZE = 1024,
		MFB_LOCAL_CONFIG_REGCNT = 22,

		MFB_PCI_BASE_CS0 = 0,
		MFB_PCI_BASE_CS1 = 1,
		MFB_PCI_BASE_CS2 = 2,
		MFB_PCI_BASE_CS3 = 3,

		MFB_PCI_BASE_IO = 0,
	};
}

namespace Ravid
{
	namespace Device
	{
		class AFX_EXT_CLASS CDeviceMFB_I : public CDeviceDio
		{
		public:
			RavidUseDynamicCreation();

			RavidPreventCopySelf(CDeviceMFB_I);

			DECLARE_DYNAMIC(CDeviceMFB_I)

			CDeviceMFB_I();
			virtual ~CDeviceMFB_I();

			virtual EDeviceInitializeResult Initialize() override;
			virtual EDeviceTerminateResult Terminate() override;

			virtual bool LoadSettings() override;
			//////////////////////////////////////////////////////////////////////////
			//기본 읽기, 쓰기 함수
			virtual bool PCIWrite(_In_ DWORD dwAddr, _In_ DWORD dwData);
			virtual DWORD PCIRead(_In_ DWORD dwAddr);

			virtual bool PCIWrite(_In_ DWORD dwCS, _In_ DWORD dwAddr, _In_ DWORD dwData);
			virtual DWORD PCIRead(_In_ DWORD dwCS, _In_ DWORD dwAddr);

			virtual DWORD PciReadReg(_In_ DWORD dwOffset);
			virtual bool PciWriteReg(_In_ DWORD dwOffset, _In_ DWORD dwData);
			//////////////////////////////////////////////////////////////////////////

			virtual bool ReadOutBit(_In_ int nBit) override;
			virtual bool ReadInBit(_In_ int nBit) override;

			virtual BYTE ReadOutByte() override;
			virtual BYTE ReadInByte() override;

			virtual WORD ReadOutWord() override;
			virtual WORD ReadInWord() override;

			virtual DWORD ReadOutDword() override;
			virtual DWORD ReadInDword() override;

			virtual bool WriteOutBit(_In_ int nBit, _In_ bool bOn) override;
			virtual bool WriteInBit(_In_ int nBit, _In_ bool bOn) override;

			virtual bool WriteOutByte(_In_ BYTE cByte) override;
			virtual bool WriteInByte(_In_ BYTE cByte) override;

			virtual bool WriteOutWord(_In_ WORD wWord) override;
			virtual bool WriteInWord(_In_ WORD wWord) override;

			virtual bool WriteOutDword(_In_ DWORD dwDword) override;
			virtual bool WriteInDword(_In_ DWORD dwDword) override;

			virtual bool ClearOutPort() override;
			virtual bool ClearInPort() override;

			virtual long GetOutputPortCount() override;
			virtual long GetInputPortCount() override;

			EGetFunctionMFB_I GetUseClearInitalize(_Out_ bool *pParam);
			ESetFunctionMFB_I SetUseClearInitalize(_In_ bool bParam);

			EGetFunctionMFB_I GetUseClearTerminate(_Out_ bool *pParam);
			ESetFunctionMFB_I SetUseClearTerminate(_In_ bool bParam);

			virtual void SetPCIVendorID(_In_ unsigned short vendorId);
			virtual void SetPCIDeviceID(_In_ unsigned short deviceId);

			virtual BOOL OnInitDialog();
			afx_msg void OnTimer(_In_ UINT_PTR nIDEvent);
			afx_msg void OnDestroy();

			virtual bool OnParameterChanged(_In_ int nParam, _In_ CString strValue) override;

		public:
			DECLARE_MESSAGE_MAP()

		protected:
			void OnBnClickedOutLed(_In_ UINT nID);

			virtual bool DoesModuleExist();

		protected:
			bool m_bInvert = false;

			DWORD m_dwInStatus = 0;
			DWORD m_dwOutStatus = 0;

			DWORD *m_parrDwCS = nullptr;
			DWORD *m_parrDwCSLen = nullptr;

			void* m_pDeviceObject = nullptr;

			unsigned short m_vendorId = 0x10b8;
			unsigned short m_deviceId = 0x9030;
		};
	}
}

