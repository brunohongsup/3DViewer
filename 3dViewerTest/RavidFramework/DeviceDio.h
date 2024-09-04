#pragma once

#include "DeviceBase.h"

#include "../RavidCore/RavidPoint.h"

namespace Ravid
{
	namespace Device
	{
		/**
		* 입출력 디바이스 계열의 기저 클래스입니다.
		*/
		class AFX_EXT_CLASS CDeviceDio : public CDeviceBase
		{
		public:
			CDeviceDio();
			virtual ~CDeviceDio();

			virtual ptrdiff_t OnMessage(_In_ Ravid::Framework::CMessageBase* pMessage);

			virtual EDeviceInitializeResult Initialize() override = 0;
			virtual EDeviceTerminateResult Terminate() override = 0;
			
			virtual bool ReadOutBit(_In_ int nBit) = 0;
			virtual bool ReadInBit(_In_ int nBit) = 0;

			virtual BYTE ReadOutByte() = 0;
			virtual BYTE ReadInByte() = 0;

			virtual WORD ReadOutWord() = 0;
			virtual WORD ReadInWord() = 0;

			virtual DWORD ReadOutDword() = 0;
			virtual DWORD ReadInDword() = 0;

			virtual bool WriteOutBit(_In_ int nBit, _In_ bool bOn) = 0;
			virtual bool WriteInBit(_In_ int nBit, _In_ bool bOn) = 0;

			virtual bool WriteOutByte(_In_ BYTE cByte) = 0;
			virtual bool WriteInByte(_In_ BYTE cByte) = 0;

			virtual bool WriteOutWord(_In_ WORD wWord) = 0;
			virtual bool WriteInWord(_In_ WORD wWord) = 0;

			virtual bool WriteOutDword(_In_ DWORD dwDword) = 0;
			virtual bool WriteInDword(_In_ DWORD dwDword) = 0;

			virtual bool ClearOutPort() = 0;
			virtual bool ClearInPort() = 0;

			virtual long GetOutputPortCount() = 0;
			virtual long GetInputPortCount() = 0;

		protected:
			virtual void AllocateIO(_In_ long nInputSize, _In_ long nOutputSize);
			virtual void FreeIO();

			virtual bool AddControls();
			virtual bool CreateLedArray(_In_ double dblDPIScale, _In_ CFont* pBaseFont, _In_ CRavidPoint<int> rpLeftTopPt, _In_ CRavidPoint<int> rpRightTopPt, _Out_ CRavidRect<int>& rrResultGroupBox);

		public:
			DECLARE_MESSAGE_MAP()

		public:
			virtual BOOL OnInitDialog();
			afx_msg void OnDestroy();
			afx_msg void OnTimer(UINT_PTR nIDEvent);

		protected:
			bool* m_pBUsingInputArray = nullptr;
			bool* m_pBUsingOutputArray = nullptr;

			CString* m_pStrInputNameArray = nullptr;
			CString* m_pStrOutputNameArray = nullptr;
		};
	}
}

