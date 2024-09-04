#pragma once

#include "DeviceDio.h"

namespace DeviceMVTechDio
{
	enum
	{
		IO_READ = 0x00,
		IO_WRITE = 0x80,
		IO_OUT_READ = 0x0C,

		MT_DIO_MODULE_MIN = 0,
		MT_DIO_MODULE_MAX = 2,

		MIO_OUT_PORT = 32,
		MIO_IN_PORT = 32,

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
		class AFX_EXT_CLASS CDeviceMVTechDio : public CDeviceDio
		{
		public:
			RavidUseDynamicCreation();

			RavidPreventCopySelf(CDeviceMVTechDio);

			DECLARE_DYNAMIC(CDeviceMVTechDio)

			CDeviceMVTechDio();
			virtual ~CDeviceMVTechDio();

			virtual EDeviceInitializeResult Initialize() override;
			virtual EDeviceTerminateResult Terminate() override;

			virtual bool LoadSettings() override;

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

			EGetFunctionMVTechDio GetUseClearInitalize(_Out_ bool *pParam);
			ESetFunctionMVTechDio SetUseClearInitalize(_In_ bool bParam);

			EGetFunctionMVTechDio GetUseClearTerminate(_Out_ bool *pParam);
			ESetFunctionMVTechDio SetUseClearTerminate(_In_ bool bParam);
			
			EGetFunctionMVTechDio GetCheckPortFromBoard(_Out_ bool *pParam);
			ESetFunctionMVTechDio SetCheckPortFromBoard(_In_ bool bParam);

			virtual bool OnParameterChanged(_In_ int nParam, _In_ CString strValue) override;

		public:
			DECLARE_MESSAGE_MAP()

		protected:
			void OnBnClickedOutLed(_In_ UINT nID);

			virtual bool DoesModuleExist();

		protected:
			bool m_bCheckFromBoard = false;

			bool m_bInvert = false;

			DWORD m_dwInStatus = 0;
			DWORD m_dwOutStatus = 0;

			DWORD *m_arrDwCS = nullptr;
			DWORD *m_arrDwCSLen = nullptr;

			void* m_pDeviceObject = nullptr;
		};
	}
}

