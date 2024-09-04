#pragma once

#include "RavidObjectEx.h"

#include <memory>

namespace Ravid
{
	namespace Framework
	{
		class AFX_EXT_CLASS CSequenceInfo : public CRavidObjectEx
		{
		public:
			CSequenceInfo();
			virtual ~CSequenceInfo();

			// 해당 Sequence를 실행한다.
			bool Run(_In_opt_ ESequencePriority ePriority = ESequencePriority_Normal);

			// 해당 Sequence를 정지/ 일시정지 한다.
			//  - 정지는 Sequence가 정지한 뒤에 현재 Step 상태를 0으로 클리어 합니다.
			//  - 일시정지는 Sequence가 일시정지한 뒤에 현재 Step 상태를 그대로 둡니다. (재개 가능 하도록) 
			bool Stop(_In_ ESequenceStatus eFlag = ESequenceStatus_Stop);

			// 해당 Sequence가 동작 중인지 얻어온다.
			//  - The sequence is running status, if the return value is true. otherwise, the sequence is idle status.
			bool IsRunning();

			// 해당 Sequence의 동작을 활성화 할 것인지, 비활성화 할 것인지 정한다.
			//  - true : 활성화
			//  - false : 비활성화
			void Enable(_In_ bool bEnable);

			// 해당 Sequence의 동작 활성 상태를 확인한다.
			bool IsEnabled();

			// 현재 Sequence Step값을 설정한다.
			void SetStep(_In_ long nStep);

			// 현재 Sequence Step값을 얻어온다.
			long GetStep();


			// 정지 함수 호출 시 해당 스텝 종료후 바로 정지 할 것인지, 특정 스텝 까지 진행 후 종료할 것인지 Operation Mode를 정한다.
			//  - true : 해당 스텝 종료후 바로 정지.
			//  - false : 사용자가 지정한 지점까지 진행 후 정지.
			void SetImmediateStop(_In_ bool bImmediateStop);

			// SetImmediateStop에서 설정된 값을 반환한다.
			bool GetImmediateStop();


			// 해당 Sequence의 이름을 설정한다.
			// Sequence생성 후 해당 함수를 호출하지 않거나, 빈 문자열 입력 시(ex. "") "Default sequence name" 으로 설정 된다.
			void SetName(_In_ CString strName);

			// 해당 Sequence의 이름을 얻어온다.
			CString GetName();


			// 해당 Sequence 정지시에 설정 시간이 지날때 까지 정지하지 않을 시 강제 종료한다. (단위 : Millisecond)
			void SetSequenceStopTimeout(_In_ float fTimeoutMillisecond);

			// 해당 Sequence 정지시에 종료되지 않을 시 강제종료를 시작할 시간 설정값을 얻어온다. (단위 : Millisecond)
			float GetSequenceStopTimeout();

			ESequenceStatus GetStatus();

		protected:

			//////////////////////////////////////////////////////////////////////////
			// 서브클래싱시에 아래 추상 함수들을 반드시 구현해 주세요.
			// 아래 함수들은 임의로 호출하지 마십시오.

			virtual void OnStart() = 0;
			virtual void OnPause() = 0;
			virtual void OnStop() = 0;

			virtual bool Work() = 0;

			//////////////////////////////////////////////////////////////////////////
			// SetImmediateStop 함수에서 false로 설정 시 IndicateStopPosition함수를 호출 하는 구간이 나올 때 까지 정지 하지 않는데
			// 정지를 원하는 구간의 맨 마지막에 아래 함수를 추가한다.
			void IndicateStopPosition();


		private:
			static UINT SequenceThread(_In_ LPVOID pParam);
			static UINT SequenceTimeoutMonitor(_In_ LPVOID pParam);

		protected:
			CString m_strName;
			float m_fStopTimeout = 10000.f;


			volatile long m_nStep = 0;

			volatile bool m_bImmediateStop = true;
			volatile bool m_bIsRunning = false;
			volatile bool m_bIsEnabled = true;
			volatile ESequenceStatus m_eStatus = ESequenceStatus_Stop;

			CWinThread* m_pThread = nullptr;
		};
	}
}