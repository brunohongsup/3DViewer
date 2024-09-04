#pragma once

#include "RavidObjectEx.h"

#include <memory>

namespace Ravid
{
	namespace Framework
	{
		class CSequenceInfo;
		
		/**
		* 프레임워크 시퀀스를 관리하는 클래스입니다.
		*/
		class AFX_EXT_CLASS CSequenceManager : public CRavidObjectEx
		{
		private:
			CSequenceManager();

		public:
			virtual ~CSequenceManager();

			// 해당 Sequence를 등록 한다.
			static bool RegisterSequence(_In_ CSequenceInfo* pSequence, bool bAutoDestroyWhenTerminating = true);

			// 해당 Sequence를 등록 해제 한다.
			static bool UnregisterSequence(_In_ CSequenceInfo* pSequence);

			static size_t GetSequenceCount();

			// 등록된 Sequences를 모두 시작한다. 
			static bool Run();

			// 등록된 모든 Sequence를 중지한다. (비동기 함수)
			static bool Stop(_In_ ESequenceStatus eFlag = ESequenceStatus_Stop);

			// Stop함수가 비동기 이므로 만약 Stop함수 호출 이후 모든 Sequence가 정지될 때 까지 기다려야 한다면 아래 함수를 호출해서 기다린다.
			static bool WaitStopForAllSequence();

			// Sequence들이 동작중인지 확인한다. 등록된 Sequence중 단 하나의 Sequence라도 동작중에 있다면 true를 리턴한다.
			static bool IsRunning();

			static ESequenceStatus GetStatus();

			static CSequenceInfo* GetSequence(_In_ size_t stNumber);

		private:
			static CSequenceManager* GetInstance();

		protected:
			std::vector<CSequenceInfo*> m_vctSequences;
			std::vector<bool> m_vctSequenceAutoDestroyFlags;

		};
	}
}

