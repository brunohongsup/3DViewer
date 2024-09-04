#pragma once

#include <vector>

namespace Ravid
{
	namespace Miscellaneous
	{
		class CInternalPerformanceCounter;
		
		/**
		* 동작 시간을 측정하는 클래스입니다.
		*/
		class AFX_EXT_CLASS CPerformanceCounter : public CRavidObject
		{
			RavidUseDynamicCreation();
			RavidPreventCopySelf(CPerformanceCounter);

		public:
			CPerformanceCounter(void);
			virtual ~CPerformanceCounter(void);
			
			/**
			* 프로그램 카운트를 시작합니다.
			*/
			void Start();

			/**
			* 호출된 시점을 기록합니다.
			*
			* return value : bool 의 기록 성공 여부를 반환합니다.
			*/
			bool CheckPoint();
			
			/**
			* 호출된 시점을 가져옵니다.
			*
			* return value : float 의 측정 시간을 반환합니다.
			*/
			float GetElapsedTime();
			
			/**
			* 기록된 시점 리스트를 가져옵니다.
			*
			* param[in] pReturn : 출력 기록된 시간 리스트
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool GetCheckPoints(_Out_ std::vector<float>* pReturn);

		private:
			CInternalPerformanceCounter* m_pInternal = nullptr;
			
		};
	}
}

