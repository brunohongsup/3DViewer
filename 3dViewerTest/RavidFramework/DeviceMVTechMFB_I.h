#pragma once

#include "DeviceTrigger.h"

namespace MFB_I
{
	// ChannelCount : Channels
	// OutportCount : DIO outputs
	// InportCount : DIO inputs
	// QueueCount : memory queue position
	// QueueStatusCount : queue free status (bit operator)
	// EncoderCount : input encoder line
	// TriggerCount : output trigger line
	// RegisterCount : PCI internal register lines
	// PCI_BASE_CS0 : User extension area 0 (MVTech Only used)
	// PCI_BASE_CS1 : User extension area 1
	// PCI_BASE_CS2 : User extension area 2
	// PCI_BASE_CS3 : User extension area 3
	enum class BoardInfo
	{
		ChannelCount = 1,
		InportCount = 32,
		OutportCount = 32,
		QueueCount = 0,
		QueueStatusCount = 0,
		EncoderCount = 1,
		TriggerCount = 1,

		RegisterCount = 22,

		MAX_BASE_ADDR_NUMS = 4,

		PCI_BASE_CS0 = 0,
		PCI_BASE_CS1 = 1,
		PCI_BASE_CS2 = 2,
		PCI_BASE_CS3 = 3,
	};

	enum class AddressMap
	{
		// 입력 포트
		Input = 0x00, // R
		// 출력 포트
		Output = 0x80, // W
		OutputRead = 0x0C, // R

		// 현재 엔코더 값을 가져옵니다.
		EncoderRead = 0x04, // R

		// 현재 트리거 값을 가져옵니다.
		TriggerRead = 0x08, // R

		// 엔코더, 트리거 수를 초기화 합니다.
		// 비트가 살아있으면 계속 초기화 됩니다.
		// 초기화가 완료되면 0으로 비트를 비활성화합니다.
		// 0 -> 1 -> 0 을 입력합니다.
		ResetCounter = 0x84, // W

		// 트리거 동작을 설정합니다.
		// 0x00 : Off, 0x01 : On
		TriggerEnable = 0x88, // W

		// 엔코더 방향 설정
		// 0x00 : 정방향 , 0x01 : 역방향
		EncoderDirection = 0xA4, // W

		// 트리거 발생 시작 엔코더
		EncoderBeginPosition = 0x90, // W
		// 트리거 발생 끝 엔코더 (동작안함)
		EncoderEndPosition = 0x94, // W
		
		// 트리거 발생 조건
		// 상위 8비트 0x0000XX00 (On Time 시간)
		// 하위 8비트 0x000000XX (Cycle Encoder 주기)
		SettingTriggerEvent = 0x8C, // W
	};
}

namespace Ravid
{
	namespace Miscellaneous
	{
		class CPerformanceCounter;
	}

	namespace Device
	{
		class AFX_EXT_CLASS CDeviceMVTechMFB_I : public CDeviceTrigger
		{
		public:
			RavidUseDynamicCreation();

			RavidPreventCopySelf(CDeviceMVTechMFB_I);

			DECLARE_DYNAMIC(CDeviceMVTechMFB_I)

			CDeviceMVTechMFB_I();
			virtual ~CDeviceMVTechMFB_I();

			virtual ptrdiff_t OnMessage(_In_ Ravid::Framework::CMessageBase* pMessage);

			virtual EDeviceInitializeResult Initialize() override;
			virtual EDeviceTerminateResult Terminate() override;

			virtual bool LoadSettings() override;

			// override 함수
			virtual bool EnableTrigger() override;
			virtual bool DisableTrigger() override;

			virtual bool IsTriggerEnabled() override;

			virtual long GetEncoderCount() override;
			virtual long GetTriggerCount() override;
			virtual bool ResetCounter() override;


			//////////////////////////////////////////////////////////////////////////
			//기본 읽기, 쓰기 함수
			virtual bool PCIWrite(_In_ DWORD dwAddr, _In_ DWORD dwData);
			virtual DWORD PCIRead(_In_ DWORD dwAddr);

			virtual bool PCIWrite(_In_ DWORD dwCS, _In_ DWORD dwAddr, _In_ DWORD dwData);
			virtual DWORD PCIRead(_In_ DWORD dwCS, _In_ DWORD dwAddr);

			virtual DWORD PciReadReg(_In_ DWORD dwOffset);
			virtual bool PciWriteReg(_In_ DWORD dwOffset, _In_ DWORD dwData);
			//////////////////////////////////////////////////////////////////////////

			virtual bool ReadOutBit(_In_ int nBit);
			virtual bool ReadInBit(_In_ int nBit);

			virtual BYTE ReadOutByte();
			virtual BYTE ReadInByte();

			virtual WORD ReadOutWord();
			virtual WORD ReadInWord();

			virtual DWORD ReadOutDword();
			virtual DWORD ReadInDword();

			virtual bool WriteOutBit(_In_ int nBit, _In_ bool bOn);

			virtual bool WriteOutByte(_In_ BYTE cByte);

			virtual bool WriteOutWord(_In_ WORD wWord);

			virtual bool WriteOutDword(_In_ DWORD dwDword);

			virtual bool ClearOutPort();

			//////////////////////////////////////////////////////////////////////////
			virtual void SetPCIVendorID(_In_ unsigned short vendorId);
			virtual void SetPCIDeviceID(_In_ unsigned short deviceId);

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
			* 트리거 시작 위치를 설정합니다.
			*/
			virtual EGetFunction GetStartPosition(_Out_ int* pParam);
			virtual ESetFunction SetStartPosition(_In_ int nParam);
			
			/**
			* 트리거 끝 위치를 설정합니다. (메모리는 있으나 동작하지 않습니다.)
			*/
			virtual EGetFunction GetEndPosition(_Out_ int* pParam);
			virtual ESetFunction SetEndPosition(_In_ int nParam);
			
			/**
			* 트리거 주기를 설정합니다.
			*/
			virtual EGetFunction GetTriggerCycle(_Out_ int* pParam);
			virtual ESetFunction SetTriggerCycle(_In_ int nParam);
			
			/**
			* 트리거 활성 시간을 설정합니다.
			*/
			virtual EGetFunction GetTriggerOnTime(_Out_ int* pParam);
			virtual ESetFunction SetTriggerOnTime(_In_ int nParam);
			
			/**
			* 엔코더 증감 방향을 설정합니다.
			*/
			virtual EGetFunction GetEncoderDirection(_Out_ EDeviceSimplexDirection* pParam);
			virtual ESetFunction SetEncoderDirection(_In_ EDeviceSimplexDirection eParam);

			virtual bool OnParameterChanged(_In_ int nParam, _In_ CString strValue) override;

		public:
			DECLARE_MESSAGE_MAP()

			virtual BOOL OnInitDialog();
			afx_msg void OnTimer(_In_ UINT_PTR nIDEvent);
			afx_msg void OnDestroy();

		protected:
			void OnBnClickedOutLed(_In_ UINT nID);

			virtual bool DoesModuleExist();
			virtual bool AddControls();
			virtual bool CreateLedArray(_In_ double dblDPIScale, _In_ CFont* pBaseFont, _In_ CPoint rpLeftTopPt, _In_ CPoint rpRightTopPt, _Out_ CRect& rrResultGroupBox);

			void AllocateIO(_In_ long nInputSize, _In_ long nOutputSize);
			void FreeIO();


		protected:
			// Ctrl Trigger
			bool m_bTriggerEnabled;

			// Ctrl DIO
			DWORD m_dwInStatus;
			DWORD m_dwOutStatus;

			bool* m_pUsedInput;
			bool* m_pUsedOutput;

			CString* m_pStrInputName;
			CString* m_pStrOutputName;

			// Ctrl Device
			DWORD* m_pDwCS;
			DWORD* m_pDwCSLen;

			// Device Object
			void* m_pDeviceObject;

			// Board Info
			unsigned short m_vendorId;
			unsigned short m_deviceId;
		};
	}
}

