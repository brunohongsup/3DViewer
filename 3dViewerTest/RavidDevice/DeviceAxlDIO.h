#pragma once

#ifdef USING_DEVICE

#include "../RavidFramework/DeviceDio.h"
#include "../RavidFramework/DefinitionsDevice.h"

namespace Ravid
{
	namespace Device
	{
		class CDeviceAxlDio : public CDeviceDio
		{
		public:
			RavidUseDynamicCreation();

			RavidPreventCopySelf(CDeviceAxlDio);

			DECLARE_DYNAMIC(CDeviceAxlDio)
			
			CDeviceAxlDio();
			virtual ~CDeviceAxlDio();

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

			virtual bool OnParameterChanged(_In_ int nParam, _In_ CString strValue) override;

		public:
			DECLARE_MESSAGE_MAP()

		protected:
			virtual void OnBnClickedOutLed(_In_ UINT nID);

			virtual bool DoesModuleExist();

		public:
			/**
			* 디바이스 초기화 시 출력 포트 상태를 끕니다.
			*/
			virtual EGetFunction GetUseClearInitalize(_Out_ bool * pParam);
			virtual ESetFunction SetUseClearInitalize(_In_ bool bParam);

			/**
			* 디바이스 해제 시 출력 포트 상태를 끕니다.
			*/
			virtual EGetFunction GetUseClearTerminate(_Out_ bool * pParam);
			virtual ESetFunction SetUseClearTerminate(_In_ bool bParam);
			
			/**
			* 디바이스 초기화 시 하드웨어 초기화를 켜고 끕니다.
			*/
			virtual EGetFunction GetOpenNoHardware(_Out_ bool * pParam);
			virtual ESetFunction SetOpenNoHardware(_In_ bool bParam);

		};
	}
}

#endif