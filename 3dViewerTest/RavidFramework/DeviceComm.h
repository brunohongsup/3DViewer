#pragma once

#include "DeviceBase.h"

namespace Ravid
{
	namespace Framework
	{
		class CPacket;
	}

	namespace Miscellaneous
	{
		class CPerformanceCounter;
	}

	namespace Device
	{
		/**
		* 통신 디바이스 계열의 기저 클래스입니다.
		*/
		class AFX_EXT_CLASS CDeviceComm : public CDeviceBase
		{
		public:
			CDeviceComm();
			virtual ~CDeviceComm();
			
			/**
			* 통신을 하기 위한 초기화 작업을 실행합니다.
			* 순수 가상함수로 꼭 정의해야 합니다.
			*
			* return value : 실패하면 EDeviceInitializeResult_OK 이외의 값을 가집니다.
			*/
			virtual EDeviceInitializeResult Initialize() override = 0;
			
			/**
			* 통신의 리소스를 반환하기 위한 해제 작업을 실행합니다.
			* 순수 가상함수로 꼭 정의해야 합니다.
			*
			* return value : 실패하면 EDeviceTerminateResult_OK 이외의 값을 가집니다.
			*/
			virtual EDeviceTerminateResult Terminate() override = 0;
			
			/**
			* 송신 합니다.
			* 순수 가상함수로 꼭 정의해야 합니다.
			*
			* param[in] refPacket : 전송하기 위한 패킷 정보 데이터 (Reference)
			*
			* return value : 실패하면 ECommSendResult_OK 이외의 값을 가집니다.
			*/
			virtual ECommSendResult Send(_In_ Framework::CPacket& refPacket) = 0;
			
			/**
			* 송신 합니다.
			* 순수 가상함수로 꼭 정의해야 합니다.
			*
			* param[in] pPacket : 전송하기 위한 패킷 정보 데이터 (Pointer)
			*
			* return value : 실패하면 ECommSendResult_OK 이외의 값을 가집니다.
			*/
			virtual ECommSendResult Send(_In_ Framework::CPacket* pPacket) = 0;
			
			virtual bool IsSending();
			virtual bool IsReceiving();

			virtual unsigned long long GetTotalReceivedBytes();
			virtual unsigned long long GetTotalSentBytes();

		protected:
			unsigned long long m_ullSentBytes;
			unsigned long long m_ullReceivedBytes;

			Miscellaneous::CPerformanceCounter* m_pPcSending = nullptr;
			Miscellaneous::CPerformanceCounter* m_pPcReceiving = nullptr;
		};
	}
}

