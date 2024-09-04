#pragma once

#include "DeviceTrigger.h"

#include "../RavidCore/RavidPoint.h"

namespace Ravid
{
	namespace Device
	{
		class AFX_EXT_CLASS CDeviceMFB_T : public CDeviceTrigger
		{
		public:
			RavidUseDynamicCreation();

			RavidPreventCopySelf(CDeviceMFB_T);

			DECLARE_DYNAMIC(CDeviceMFB_T)

			CDeviceMFB_T();
			virtual ~CDeviceMFB_T();

			virtual ptrdiff_t OnMessage(_In_ Ravid::Framework::CMessageBase* pMessage);

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
			//override 함수
			virtual bool EnableTrigger() override;
			virtual bool DisableTrigger() override;

			virtual bool IsTriggerEnabled() override;

			virtual long GetEncoderCount() override;
			virtual long GetTriggerCount() override;
			virtual bool ResetCounter() override;
			//////////////////////////////////////////////////////////////////////////
			//공통 사용 함수
			//Board 정보
			virtual long GetChannelNumber();
			virtual long GetQueueNumber();
			virtual long GetQueueStatusPortNumber();
			virtual long GetOutputPortNumber();
			virtual long GetInputPortNumber();
			virtual long GetEncoderPortNumber();
			virtual long GetTriggerPortNumber();

			//trigger 기능
			virtual long GetEncoderCount(_In_ int nChannel);
			virtual long GetTriggerCount(_In_ int nChannel, _In_ int nSelect);
			virtual bool ResetDevice();
			virtual bool ResetDevice(_In_ int nChannel);
			virtual bool ExcuteGrab(_In_ int nChannel, _In_opt_ DWORD dwBitMask = 0x01);

			//IO 기능
			virtual bool ReadInBit(_In_ int nBit);
			virtual bool ReadOutBit(_In_ int nBit);
			virtual bool WriteOutBit(_In_ int nBit, _In_ bool bOn);
			//////////////////////////////////////////////////////////////////////////
			//보드별 분리 기능
			//Trigger 기능
			virtual bool SetQueuePosition(_In_ long nChannel, _In_ long nIndex, _In_ DWORD dwPos);
			virtual bool SetQueueOrderPosition(_In_ long nChannel, _In_ long nIndex, _In_ DWORD dwPos);

			virtual DWORD GetQueueStatus(_In_ long nChannel, _In_ long nIndex);

			virtual bool EnableTrigger(_In_ int nChannel);
			virtual bool DisableTrigger(_In_ int nChannel);

			virtual bool IsTriggerEnabled(_In_ int nChannel);

			virtual bool EnableLive(_In_ int nChannel);
			virtual bool DisableLive(_In_ int nChannel);

			virtual bool EnableLive(_In_ int nChannel, _In_ int nSelect);
			virtual bool DisableLive(_In_ int nChannel, _In_ int nSelect);

			virtual bool EnableGrab(_In_ int nChannel);
			virtual bool EnableGrab(_In_ int nChannel, _In_ int nSelect);

			virtual DWORD GetAreaCameraCount(_In_ int nChannel);

			virtual bool UsingTrigger(_In_ int nChannel, _In_ bool bUse);

			virtual long GetIDCount();
			virtual bool GetID(_In_ int nChannel, _Out_ DWORD* pParam);

			//////////////////////////////////////////////////////////////////////////
			//parameter 함수
			//공통 함수
			EGetFunctionMFB_T GetBoardType(_Out_ EDeviceMSPBoardType* pParam);
			ESetFunctionMFB_T SetBoardType(_In_ EDeviceMSPBoardType eParam);

			EGetFunctionMFB_T GetUIShowDio(_Out_ bool* pParam);
			ESetFunctionMFB_T SetUIShowDio(_In_ bool bParam);

			EGetFunctionMFB_T GetUIShowTrgEnc(_Out_ bool* pParam);
			ESetFunctionMFB_T SetUIShowTrgEnc(_In_ bool bParam);

			EGetFunctionMFB_T GetUIShowQueue_CH1(_Out_ bool* pParam);
			ESetFunctionMFB_T SetUIShowQueue_CH1(_In_ bool bParam);

			EGetFunctionMFB_T GetUIShowQueue_CH2(_Out_ bool* pParam);
			ESetFunctionMFB_T SetUIShowQueue_CH2(_In_ bool bParam);

			EGetFunctionMFB_T GetUIShowID(_Out_ bool* pParam);
			ESetFunctionMFB_T SetUIShowID(_In_ bool bParam);

			EGetFunctionMFB_T GetEncoderDirection_CH1(_Out_ EDeviceEncoderDirection* pParam);
			ESetFunctionMFB_T SetEncoderDirection_CH1(_In_ EDeviceEncoderDirection eParam);

			EGetFunctionMFB_T GetEncoderDirection_CH2(_Out_ EDeviceEncoderDirection* pParam);
			ESetFunctionMFB_T SetEncoderDirection_CH2(_In_ EDeviceEncoderDirection eParam);

			EGetFunctionMFB_T GetStartPosition_CH1(_Out_ int* pParam);
			ESetFunctionMFB_T SetStartPosition_CH1(_In_ int nParam);
			EGetFunctionMFB_T GetStartPosition_CH2(_Out_ int* pParam);
			ESetFunctionMFB_T SetStartPosition_CH2(_In_ int nParam);

			//MSP_P16_T2_Q16_1
			//MSP_P16_T2_Q16_2

			EGetFunctionMFB_T GetEndPosition_CH1(_Out_ int* pParam);
			ESetFunctionMFB_T SetEndPosition_CH1(_In_ int nParam);
			EGetFunctionMFB_T GetEndPosition_CH2(_Out_ int* pParam);
			ESetFunctionMFB_T SetEndPosition_CH2(_In_ int nParam);

			EGetFunctionMFB_T GetTriggerOnTime_CH1(_Out_ int* pParam);
			ESetFunctionMFB_T SetTriggerOnTime_CH1(_In_ int nParam);
			EGetFunctionMFB_T GetTriggerOnTime_CH2(_Out_ int* pParam);
			ESetFunctionMFB_T SetTriggerOnTime_CH2(_In_ int nParam);

			EGetFunctionMFB_T GetTriggerCycle_CH1(_Out_ int* pParam);
			ESetFunctionMFB_T SetTriggerCycle_CH1(_In_ int nParam);
			EGetFunctionMFB_T GetTriggerCycle_CH2(_Out_ int* pParam);
			ESetFunctionMFB_T SetTriggerCycle_CH2(_In_ int nParam);

			EGetFunctionMFB_T GetStrobeTime_CH1(_Out_ int* pParam);
			ESetFunctionMFB_T SetStrobeTime_CH1(_In_ int nParam);
			EGetFunctionMFB_T GetStrobeTime_CH2(_Out_ int* pParam);
			ESetFunctionMFB_T SetStrobeTime_CH2(_In_ int nParam);

			//////////////////////////////////////////////////////////////////////////
			//사용 가능한 기능의 수 hjcho

			EGetFunctionMFB_T GetUseClearInitalize(_Out_ bool *pParam);
			ESetFunctionMFB_T SetUseClearInitalize(_In_ bool bParam);

			EGetFunctionMFB_T GetUseClearTerminate(_Out_ bool *pParam);
			ESetFunctionMFB_T SetUseClearTerminate(_In_ bool bParam);

			//////////////////////////////////////////////////////////////////////////

			virtual void SetPCIVendorID(_In_ unsigned short vendorId);
			virtual void SetPCIDeviceID(_In_ unsigned short deviceId);

			afx_msg void OnTimer(UINT_PTR nIDEvent);
			virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

			virtual bool AddControls();
			//UI 셋팅 함수
			virtual bool AddIOs(_In_ double dblDPIScale, _In_ CFont* pBaseFont, _In_ CRavidPoint<int> cpLeftTopPt, _In_ CRavidPoint<int> cpRightBottomPt, _Out_ CRavidRect<int>* pResultGroupBox);
			virtual bool AddIOsNotOutput1(_In_ double dblDPIScale, _In_ CFont* pBaseFont, _In_ CRavidPoint<int> cpLeftTopPt, _In_ CRavidPoint<int> cpRightBottomPt, _Out_ CRavidRect<int>* pResultGroupBox);
			virtual bool AddIOsNotOutput2(_In_ double dblDPIScale, _In_ CFont* pBaseFont, _In_ CRavidPoint<int> cpLeftTopPt, _In_ CRavidPoint<int> cpRightBottomPt, _Out_ CRavidRect<int>* pResultGroupBox);
			virtual bool AddIOsNotOutput3(_In_ double dblDPIScale, _In_ CFont* pBaseFont, _In_ CRavidPoint<int> cpLeftTopPt, _In_ CRavidPoint<int> cpRightBottomPt, _Out_ CRavidRect<int>* pResultGroupBox);
			virtual bool AddTriggers(_In_ double dblDPIScale, _In_ CFont* pBaseFont, _In_ CRavidPoint<int> cpLeftTopPt, _In_ CRavidPoint<int> cpRightBottomPt, _Out_ CRavidRect<int>* pResultGroupBox);
			virtual bool AddQueues(_In_ double dblDPIScale, _In_ int nStartID, _In_ CFont* pBaseFont, _In_ CRavidPoint<int> cpLeftTopPt, _In_ CRavidPoint<int> cpRightBottomPt, _Out_ CRavidRect<int>* pResultGroupBox);
			virtual bool AddQueuesNotOutput(_In_ double dblDPIScale, _In_ int nStartID, _In_ CFont* pBaseFont, _In_ CRavidPoint<int> cpLeftTopPt, _In_ CRavidPoint<int> cpRightBottomPt, _Out_ CRavidRect<int>* pResultGroupBox);
			virtual bool AddButton(_In_ double dblDPIScale, _In_ CFont* pBaseFont, _In_ CRavidPoint<int> cpLeftTopPt, _In_ CRavidPoint<int> cpRightBottomPt, _Out_ CRavidRect<int>* pResultGroupBox);

			virtual bool AddIDs(_In_ double dblDPIScale, _In_ CFont* pBaseFont, _In_ CRavidPoint<int> cpLeftTopPt, _In_ CRavidPoint<int> cpRightBottomPt, _Out_ CRavidRect<int>* pResultGroupBox);

			virtual void DisplayParameter(_In_ EDeviceMSPBoardType eType);

			virtual BOOL OnInitDialog();
			afx_msg void OnDestroy();

			virtual bool OnParameterChanged(_In_ int nParam, _In_ CString strValue) override;

		public:
			DECLARE_MESSAGE_MAP()

		protected:
			virtual bool DoesModuleExist();

			virtual bool Display_P16_E2T6_Q288();
			virtual bool Display_P16_E2T2_Q16();
			virtual bool Display_DelayedQueue();

			virtual void AllocateIO(_In_ long nInputSize, _In_ long nOutputSize);
			virtual void FreeIO();

			virtual void AllocateQueue(_In_ long nQueueSizeX, _In_ long nQueueSizeY);
			virtual void FreeQueue();

			virtual void AllocateEncoder(_In_ long nEncoderSize);
			virtual void FreeEncoder();

			virtual void AllocateTrigger(_In_ long nTriggerSize);
			virtual void FreeTrigger();

			afx_msg void OnBnClickedOutLed(_In_ UINT nID);
			afx_msg void OnBnClickedReset(_In_ UINT nID);
			afx_msg void OnBnClickedEnableTrigger(_In_ UINT nID);
			afx_msg void OnBnClickedDisableTrigger(_In_ UINT nID);

		protected:
			DWORD* m_pLiveStatus = nullptr;

			DWORD m_dwInStatus = 0;
			DWORD m_dwOutStatus = 0;

			bool* m_pUsedInput = nullptr;
			bool* m_pUsedOutput = nullptr;
			CString* m_pStrInputName = nullptr;
			CString* m_pStrOutputName = nullptr;

			DWORD* m_pQueueValueCh1 = nullptr;
			DWORD* m_pQueueValueCh2 = nullptr;

			DWORD* m_arrEncoderValue = nullptr;
			DWORD* m_arrTriggerValue = nullptr;

			DWORD* m_pDwCS = nullptr;
			DWORD* m_pDwCSLen = nullptr;

			bool* m_pTriggerEnabled = nullptr;

			long* m_pPreEncoderValue = nullptr;
			long* m_pPreTriggerValue = nullptr;

			void* m_pDeviceObject = nullptr;

			unsigned short m_vendorId = 0x10B9;
			unsigned short m_deviceId = 0x9030;
		};
	}
}

