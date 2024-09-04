#pragma once

namespace Ravid
{
	namespace Framework
	{
		class CInterMultipleCondition;
		
		/**
		* ELogicalCondition 을 저장하는 클래스입니다.
		* 이미지 처리 및 알고리즘 처리에서 사용합니다.
		* ELogicalCondition enum 을 가집니다.
		*	ELogicalCondition_Greater : 초과
		*	ELogicalCondition_GreaterEqual : 이상
		*	ELogicalCondition_Equal : 같다
		*	ELogicalCondition_NotEqual : 다르다
		*	ELogicalCondition_LessEqual : 이하
		*	ELogicalCondition_Less : 미만
		*/
		class AFX_EXT_CLASS CMultipleCondition : public CRavidObject
		{
		public:
			CMultipleCondition();
			virtual ~CMultipleCondition();

			CMultipleCondition(_In_ CMultipleCondition& rmvdCondition);

			CMultipleCondition(_In_ const ELogicalCondition& elcCondition);
			CMultipleCondition(_In_ const ELogicalCondition& elcCondition1, const ELogicalCondition& elcCondition2);
			CMultipleCondition(_In_ const ELogicalCondition& elcCondition1, const ELogicalCondition& elcCondition2, const ELogicalCondition& elcCondition3);
			CMultipleCondition(_In_ const ELogicalCondition& elcCondition1, const ELogicalCondition& elcCondition2, const ELogicalCondition& elcCondition3, const ELogicalCondition& elcCondition4);

			void operator=(_In_ CMultipleCondition rmvdMultiConst);
			void operator=(_In_ ELogicalCondition elcCondition);
			
			/**
			* 설정을 초기화 합니다. 
			*/
			virtual void Clear();
			
			/**
			* CMultipleCondition 설정을 깊은 복사합니다.
			*
			* param[in] rmvdMultipleCondition : 설정 데이터 (Reference)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			_Out_ bool Copy(_In_ CMultipleCondition& rmvdMultipleCondition);
			
			/**
			* CMultipleCondition 설정을 깊은 복사합니다.
			*
			* param[in] pRmcdMultipleCondition : 설정 데이터 (Pointer)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			_Out_ bool Copy(_In_ CMultipleCondition* pRmcdMultipleCondition);
			
			/**
			* 해당 인덱스에 ELogicalCondition 을 설정합니다.
			*
			* param[in] stIndex : 입력 인덱스
			* param[in] elcCondition : 입력 ELogicalCondition enum (Reference)
			*	ELogicalCondition_Greater : 초과
			*	ELogicalCondition_GreaterEqual : 이상
			*	ELogicalCondition_Equal : 같다
			*	ELogicalCondition_NotEqual : 다르다
			*	ELogicalCondition_LessEqual : 이하
			*	ELogicalCondition_Less : 미만
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			_Out_ bool SetAt(_In_ size_t stIndex, _In_ const ELogicalCondition& elcCondition);
			
			/**
			* 설정된 해당 인덱스의 ELogicalCondition 을 가져옵니다.
			*
			* param[in] stIndex : 입력 인덱스
			*
			* return value : ELogicalCondition enum 의 설정 값를 반환합니다.
			*	ELogicalCondition_Greater : 초과
			*	ELogicalCondition_GreaterEqual : 이상
			*	ELogicalCondition_Equal : 같다
			*	ELogicalCondition_NotEqual : 다르다
			*	ELogicalCondition_LessEqual : 이하
			*	ELogicalCondition_Less : 미만
			*/
			_Out_ ELogicalCondition GetAt(_In_ size_t stIndex);
			
			/**
			* 해당 인덱스에 ELogicalCondition 을 추가합니다.
			*
			* param[in] stIndex : 입력 인덱스
			* param[in] elcCondition : 입력 ELogicalCondition enum (Reference)
			*	ELogicalCondition_Greater : 초과
			*	ELogicalCondition_GreaterEqual : 이상
			*	ELogicalCondition_Equal : 같다
			*	ELogicalCondition_NotEqual : 다르다
			*	ELogicalCondition_LessEqual : 이하
			*	ELogicalCondition_Less : 미만
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			_Out_ bool InsertAt(_In_ size_t stIndex, _In_ const ELogicalCondition& elcCondition);
			
			/**
			* 해당 인덱스에 ELogicalCondition 을 삭제합니다.
			*
			* param[in] stIndex : 입력 인덱스
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			_Out_ bool DeleteAt(_In_ size_t stIndex);
			
			/**
			* ELogicalCondition 을 추가합니다.
			*
			* param[in] elcCondition : 입력 ELogicalCondition enum (Reference)
			*	ELogicalCondition_Greater : 초과
			*	ELogicalCondition_GreaterEqual : 이상
			*	ELogicalCondition_Equal : 같다
			*	ELogicalCondition_NotEqual : 다르다
			*	ELogicalCondition_LessEqual : 이하
			*	ELogicalCondition_Less : 미만
			*/
			_Out_ void AddValue(_In_ const ELogicalCondition& elcCondition);
			
			/**
			* 설정된 ELogicalCondition 의 개수를 가져옵니다.
			*
			* return value : size_t 의 설정 개수를 반환합니다.
			*/
			_Out_ size_t GetSize();

		protected:
			CInterMultipleCondition* m_pInternal = nullptr; 
			
		};
	}
}

