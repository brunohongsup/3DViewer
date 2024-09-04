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
		// �Է� ��Ʈ
		Input = 0x00, // R
		// ��� ��Ʈ
		Output = 0x80, // W
		OutputRead = 0x0C, // R

		// ���� ���ڴ� ���� �����ɴϴ�.
		EncoderRead = 0x04, // R

		// ���� Ʈ���� ���� �����ɴϴ�.
		TriggerRead = 0x08, // R

		// ���ڴ�, Ʈ���� ���� �ʱ�ȭ �մϴ�.
		// ��Ʈ�� ��������� ��� �ʱ�ȭ �˴ϴ�.
		// �ʱ�ȭ�� �Ϸ�Ǹ� 0���� ��Ʈ�� ��Ȱ��ȭ�մϴ�.
		// 0 -> 1 -> 0 �� �Է��մϴ�.
		ResetCounter = 0x84, // W

		// Ʈ���� ������ �����մϴ�.
		// 0x00 : Off, 0x01 : On
		TriggerEnable = 0x88, // W

		// ���ڴ� ���� ����
		// 0x00 : ������ , 0x01 : ������
		EncoderDirection = 0xA4, // W

		// Ʈ���� �߻� ���� ���ڴ�
		EncoderBeginPosition = 0x90, // W
		// Ʈ���� �߻� �� ���ڴ� (���۾���)
		EncoderEndPosition = 0x94, // W
		
		// Ʈ���� �߻� ����
		// ���� 8��Ʈ 0x0000XX00 (On Time �ð�)
		// ���� 8��Ʈ 0x000000XX (Cycle Encoder �ֱ�)
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

			// override �Լ�
			virtual bool EnableTrigger() override;
			virtual bool DisableTrigger() override;

			virtual bool IsTriggerEnabled() override;

			virtual long GetEncoderCount() override;
			virtual long GetTriggerCount() override;
			virtual bool ResetCounter() override;


			//////////////////////////////////////////////////////////////////////////
			//�⺻ �б�, ���� �Լ�
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
			* ����̽� �ʱ�ȭ �� ��� ��Ʈ ���¸� ���ϴ�.
			*/
			virtual EGetFunction GetUseClearInitalize(_Out_ bool * pParam);
			virtual ESetFunction SetUseClearInitalize(_In_ bool bParam);

			/**
			* ����̽� ���� �� ��� ��Ʈ ���¸� ���ϴ�.
			*/
			virtual EGetFunction GetUseClearTerminate(_Out_ bool * pParam);
			virtual ESetFunction SetUseClearTerminate(_In_ bool bParam);
			
			/**
			* Ʈ���� ���� ��ġ�� �����մϴ�.
			*/
			virtual EGetFunction GetStartPosition(_Out_ int* pParam);
			virtual ESetFunction SetStartPosition(_In_ int nParam);
			
			/**
			* Ʈ���� �� ��ġ�� �����մϴ�. (�޸𸮴� ������ �������� �ʽ��ϴ�.)
			*/
			virtual EGetFunction GetEndPosition(_Out_ int* pParam);
			virtual ESetFunction SetEndPosition(_In_ int nParam);
			
			/**
			* Ʈ���� �ֱ⸦ �����մϴ�.
			*/
			virtual EGetFunction GetTriggerCycle(_Out_ int* pParam);
			virtual ESetFunction SetTriggerCycle(_In_ int nParam);
			
			/**
			* Ʈ���� Ȱ�� �ð��� �����մϴ�.
			*/
			virtual EGetFunction GetTriggerOnTime(_Out_ int* pParam);
			virtual ESetFunction SetTriggerOnTime(_In_ int nParam);
			
			/**
			* ���ڴ� ���� ������ �����մϴ�.
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

