#pragma once

#include "RavidObjectEx.h"
#include <deque>

namespace Ravid
{
	namespace Framework
	{
		class CMessageBase;
		
		/**
		* 프레임워크 메시지를 관리하는 클래스입니다.
		*/
		class AFX_EXT_CLASS CMessageManager : public CRavidObjectEx
		{
			RavidUseDynamicCreation();
			RavidPreventCopySelf(CMessageManager);

		private:
			CMessageManager();

		public:
			virtual ~CMessageManager();
			
			/**
			* 메시지 관리 설정을 초기화합니다.
			*/
			static bool Initialize();
			
			/**
			* 메시지 관리 설정을 해제합니다.
			*/
			static bool Terminate();
			
			/**
			* 메시지 관리 설정 상태를 가져옵니다.
			*/
			static bool IsInitialized();
			
			/**
			* 프레임워크 전체에 메시지를 SendMessage로 발생시킵니다.
			*/
			static ptrdiff_t SendMessage(_In_ CMessageBase* pMessage);
			
			/**
			* 프레임워크 전체에 메시지를 PostMessage로 발생시킵니다.
			*/
			static bool PostMessage(_In_ CMessageBase* pMessage);

		private:
			static bool ClearMessage();

			static UINT PostMessageThread(_In_ LPVOID pParam);

			static CMessageManager* GetInstance();

		protected:
			volatile bool m_bInitialized = false;

			CWinThread* m_pPostMessageThread = nullptr;

			std::deque<CMessageBase*> m_deqPostMessage;

			CCriticalSection m_csPostMessage;
		};
	}
}

