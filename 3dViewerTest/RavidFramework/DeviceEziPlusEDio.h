#pragma once

#include "DeviceDio.h"

namespace Ravid
{
	namespace Device
	{
		class AFX_EXT_CLASS CDeviceEziPlusEDio : public CDeviceDio
		{
		public:
			RavidUseDynamicCreation();

			RavidPreventCopySelf(CDeviceEziPlusEDio);

			DECLARE_DYNAMIC(CDeviceEziPlusEDio)

			CDeviceEziPlusEDio();
			virtual ~CDeviceEziPlusEDio();

			virtual EDeviceInitializeResult Initialize() override;
			virtual EDeviceTerminateResult Terminate() override;

			virtual bool LoadSettings() override;

			EEziPlusEDioGetFunction GetIPAddress(_Out_ CString* pParam);
			EEziPlusEDioSetFunction SetIPAddress(_In_ CString strParam);
					   			 
		public:
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

			virtual bool OnParameterChanged(_In_ int nParam, _In_ CString strValue) override;

		public:
			DECLARE_MESSAGE_MAP()

		protected:
			virtual void OnBnClickedOutLed(_In_ UINT nID);

			virtual bool DoesModuleExist();

		protected:
			bool m_bInvert = false;
		};
	}
}

