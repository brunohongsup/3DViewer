#pragma once

#include <functional>

#define CreateRavidThreadContext(pContext) Ravid::Miscellaneous::ThreadContext* pContext = new Ravid::Miscellaneous::ThreadContext; *pContext = [=](void) -> void
#define CreateRavidThreadContextCaptureRef(pContext) Ravid::Miscellaneous::ThreadContext* pContext = new Ravid::Miscellaneous::ThreadContext; *pContext = [&](void) -> void

namespace Ravid
{
	namespace Miscellaneous
	{
		typedef std::function<void(void)> ThreadContext;
		typedef ThreadContext* LPThreadContext;
		
		/**
		* 사용자 편의성을 위한 스레드 제어 및 관리를 제공하는 클래스입니다.
		*/
		class AFX_EXT_CLASS CThreadHelper : public CRavidObject
		{
		public:
			/**
			* 스레드 중요도 정책을 결정합니다.
			*/
			enum EThreadPriority
			{
				EThreadPriority_Lowest = THREAD_PRIORITY_LOWEST,
				EThreadPriority_BelowNormal = THREAD_PRIORITY_BELOW_NORMAL,
				EThreadPriority_Normal = THREAD_PRIORITY_NORMAL,
				EThreadPriority_Highest = THREAD_PRIORITY_HIGHEST,
				EThreadPriority_AboveNormal = THREAD_PRIORITY_ABOVE_NORMAL,
				EThreadPriority_ErrorReturn = THREAD_PRIORITY_ERROR_RETURN,

				EThreadPriority_TimeCritical = THREAD_BASE_PRIORITY_LOWRT,
				EThreadPriority_Idle = THREAD_BASE_PRIORITY_IDLE,
			};
			
		private:
			CThreadHelper();

		public:
			virtual ~CThreadHelper();
			/**
			* 클래스 인스턴스를 가져옵니다.
			*
			* return value : CThreadHelper* 의 현재 스레드 포인터를 반환합니다.
			*/
			static CThreadHelper* GetInstance();
			
			/**
			* 스레드를 실행합니다.
			*
			* param[in] pContext : 스레드 정보
			* param[in_opt] bCreateSuspended : 생성시 정지 상태
			* param[in_opt] ePriority : 스레드 중요도 정책
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			static bool Run(_In_ ThreadContext* pContext, _In_opt_ bool bCreateSuspended = false, _In_opt_ EThreadPriority ePriority = EThreadPriority_Normal);
			
			/**
			* 스레드를 일시 정지합니다.
			*
			* param[in] pContext : 스레드 정보
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			static bool Suspend(_In_ ThreadContext* pContext);
			
			/**
			* 스레드를 다시 시작합니다.
			*
			* param[in] pContext : 스레드 정보
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			static bool Resume(_In_ ThreadContext* pContext);
			
			/**
			* 스레드를 종료합니다.
			*
			* param[in] pContext : 스레드 정보
			* param[in] dwWaitTimeMS : 대기 시간
			* param[in_opt] bTerminateWhenTimeout : 대기 시간 이후 강제 해제 판단
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			static bool Wait(_In_ ThreadContext* pContext, _In_ DWORD dwWaitTimeMS, _In_opt_ bool bTerminateWhenTimeout = true);
		};
	}
}

