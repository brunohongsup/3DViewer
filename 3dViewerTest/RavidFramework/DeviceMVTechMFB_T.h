#pragma once

#include "DeviceTrigger.h"

namespace MFB_T
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
		InportCount = 16,
		OutportCount = 16,
		QueueCount = 8,
		QueueStatusCount = 1,
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

		// 현재 Queue가 실행 되었는지 확인합니다.
		// 설정되면 0b0000 0000 의 비트 값이 0->1로 변경됩니다.
		// 모두 설정하면 255
		// 엔코더가 동작하여 트리거를 발생시키면 0->1으로 변경됩니다.
		// 모두 완료되면 0
		QueueStatus = 0x40, // R

		// 트리거를 발생할 엔코더 값을 설정합니다.
		// DWORD 값을 입력하면 됩니다.
		QueuePosition0 = 0xC0, // R/W 
		QueuePosition1 = 0xC4, // R/W 
		QueuePosition2 = 0xC8, // R/W 
		QueuePosition3 = 0xCC, // R/W 
		QueuePosition4 = 0xD0, // R/W 
		QueuePosition5 = 0xD4, // R/W 
		QueuePosition6 = 0xD8, // R/W 
		QueuePosition7 = 0xDC, // R/W 

		// Queue Position 시 동작할 트리거 출력 포트를 설정합니다.
		// 0 ~ 255 의 값으로 0b0000 0000 로 이해하면 쉬움
		// 비트연산으로 출력할 포트를 1 로 변환하면 됩니다.
		// Queue Position 이 설정되지 않았다면 동작하지 않습니다.
		QueueOrder0 = 0x200, // W
		QueueOrder1 = 0x204, // W
		QueueOrder2 = 0x208, // W
		QueueOrder3 = 0x20C, // W
		QueueOrder4 = 0x210, // W
		QueueOrder5 = 0x214, // W
		QueueOrder6 = 0x218, // W
		QueueOrder7 = 0x21C, // W

		// 현재 엔코더 값을 가져옵니다.
		EncoderRead = 0x04, // R

		// 현재 트리거 값을 가져옵니다.
		TriggerRead = 0x08, // R

		// 엔코더, 트리거 수를 초기화 합니다.
		// 비트가 살아있으면 계속 초기화 됩니다.
		// 초기화가 완료되면 0으로 비트를 비활성화합니다.
		// 0 -> 1 -> 0 을 입력합니다.
		ResetCounter = 0x84, // W

		// 트리거 동작 방향을 설정합니다.
		// 0x04 : 정방향, 0x08 : 역방향, 0x0C : 양방향
		TriggerEnable = 0x88, // W

		// 엔코더 방향 설정
		// 0x00 : 정방향 , 0x01 : 역방향
		EncoderDirection = 0xA4, // W

		// 현재 엔코더 값을 설정
		SetEncoder = 0xE0, // W

		// 입력된 값의 하위 8 비트로 출력을 결정
		// ex) 0b0000 0101 <- 이 방향으로 읽음
		// X00, X02 Live Out, CAM+ Out
		// ex) 0b0101 0101 <- 이 방향으로 읽음 
		// X00, X02, Y00, X02 Live Out, CAM+ Out
		LiveMode = 0xA0, // W
	};
}

namespace Ravid
{
	namespace Device
	{
		class AFX_EXT_CLASS CDeviceMVTechMFB_T : public CDeviceTrigger
		{
		public:
			RavidUseDynamicCreation();

			RavidPreventCopySelf(CDeviceMVTechMFB_T);

			DECLARE_DYNAMIC(CDeviceMVTechMFB_T)
			
			CDeviceMVTechMFB_T();
			virtual ~CDeviceMVTechMFB_T();

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
			// IO
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
			
			/**
			* 현재 트리거 큐 상태를 가져옵니다. (큐 포지션을 설정하면 8 비트로 확인합니다.)
			* Queue Position 이 등록되면 0 -> 1 로 비트가 활성화됩니다.
			* Min 0 (0b0000 0000) ~ 255 (0b1111 1111)
			*/
			virtual DWORD GetQueuePosition(_In_ int nIndex);
			/**
			* 트리거 발생 엔코더 위치를 설정합니다.
			*/
			virtual bool SetQueuePosition(_In_ int nIndex, _In_ DWORD dwPos);
			
			/**
			* 트리거 발생시 외부 출력 라인을 설정합니다.
			* 0b00000000 단자대 8 포트를 각각 비트로 관리합니다. 0 -> 1로 변경한 값을 넣어야합니다.
			*/
			virtual bool SetQueueOrder(_In_ int nIndex,_In_ DWORD dwCommand);
			
			/**
			* 현재 트리거 큐 상태를 가져옵니다. (인덱스로 8 개 큐 상태를 확인합니다.)
			* Queue Position 이 등록되면 0 -> 1 로 비트가 활성화됩니다.
			* Min 0 (0b0000 0000) ~ 255 (0b1111 1111)
			*/
			virtual bool GetQueueStatus(_In_ int nIndex);
			
			/**
			* 현재 트리거 큐 상태를 가져옵니다. (큐 포지션을 설정하면 8 비트로 확인합니다.)
			* Queue Position 이 등록되면 0 -> 1 로 비트가 활성화됩니다.
			* Min 0 (0b0000 0000) ~ 255 (0b1111 1111)
			*/
			virtual DWORD GetQueueStatus();

			/**
			* 엔코더 값을 입력값으로 변경합니다.
			*/
			virtual bool WriteEncoder(_In_ DWORD dwCommand);
			
			/**
			* 일정 주기로 외부 출력 라인에 신호를 내보냅니다. (주기는 변경이 불가능합니다.)
			*/
			virtual bool WriteLive(_In_ DWORD dwCommand);
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
			* 디바이스 해제 시 출력 포트 상태를 끕니다. (트리거 보드는 IO 출력 상태를 클래스 변수로 관리합니다. 디바이스 주소 없음.)
			*/
			virtual EGetFunction GetUseClearTerminate(_Out_ bool * pParam);
			virtual ESetFunction SetUseClearTerminate(_In_ bool bParam);
						
			/**
			* 트리거 발생 방향을 설정합니다. (디바이스 초기화 전에 설정하세요.)
			*/
			virtual EGetFunction GetTriggerDirection(_Out_ EDeviceDuplexDirection* pParam);
			virtual ESetFunction SetTriggerDirection(_In_ EDeviceDuplexDirection eParam);
						
			/**
			* 엔코더 증감 방향을 설정합니다.
			*/
			virtual EGetFunction GetEncoderDirection(_Out_ EDeviceSimplexDirection* pParam);
			virtual ESetFunction SetEncoderDirection(_In_ EDeviceSimplexDirection eParam);

			virtual bool OnParameterChanged(_In_ int nParam, _In_ CString strValue) override;

		public:
			DECLARE_MESSAGE_MAP()

			virtual BOOL OnInitDialog();
			afx_msg void OnDestroy();
			afx_msg void OnTimer(UINT_PTR nIDEvent);

			afx_msg void OnBnClickedOutLed(_In_ UINT nID);

		protected:
			virtual bool DoesModuleExist();

			virtual bool AddControls();
			virtual bool AddQueues(double dblDPIScale, int nStartID, CFont * pBaseFont, CPoint cpLeftTopPt, CPoint cpRightBottomPt, CPoint * pResultGroupBox);
			virtual bool CreateLedArray(_In_ double dblDPIScale, _In_ CFont* pBaseFont, _In_ CPoint rpLeftTopPt, _In_ CPoint rpRightTopPt, _Out_ CRect& rrResultGroupBox);

			void AllocateIO(_In_ long nInputSize, _In_ long nOutputSize);
			void FreeIO();

		public:
			virtual bool PCIWrite(_In_ DWORD dwAddr, _In_ DWORD dwData);
			virtual DWORD PCIRead(_In_ DWORD dwAddr);

			virtual bool PCIWrite(_In_ DWORD dwCS, _In_ DWORD dwAddr, _In_ DWORD dwData);
			virtual DWORD PCIRead(_In_ DWORD dwCS, _In_ DWORD dwAddr);

			virtual DWORD PciReadReg(_In_ DWORD dwOffset);
			virtual bool PciWriteReg(_In_ DWORD dwOffset, _In_ DWORD dwData);

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

