#pragma once

#include "../Libraries/Includes/RavidSecurity/RavidObject.h"

namespace Ravid
{
	namespace Framework
	{
		class CInterMultipleVariable;
		
		/**
		* 데이터를 저장하는 클래스입니다.
		* 이미지 처리 및 알고리즘 처리에서 사용합니다.
		*/
		class AFX_EXT_CLASS CMultipleVariable : public CRavidObject
		{
		public:
			CMultipleVariable();
			virtual ~CMultipleVariable();

			CMultipleVariable(_In_ const CMultipleVariable& rmvMultiConstant);

			CMultipleVariable(_In_ const double& dblConst1);
			CMultipleVariable(_In_ const double& dblConst1, _In_ const double& dblConst2);
			CMultipleVariable(_In_ const double& dblConst1, _In_ const double& dblConst2, _In_ const double& dblConst3);
			CMultipleVariable(_In_ const double& dblConst1, _In_ const double& dblConst2, _In_ const double& dblConst3, _In_ const double& dblConst4);

			void operator=(_In_ CMultipleVariable& rmMultiConst);
			void operator=(_In_ double dblConst1);

			double& operator[](_In_ int nIdx);
			double operator[](_In_ int nIdx) const;
			
			/**
			* 설정을 초기화 합니다. 
			*/
			virtual void Clear();
			
			/**
			* CMultipleVariable 설정을 깊은 복사합니다.
			*
			* param[in] rmvMultiConstant : 설정 데이터 (Reference)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			_Out_ bool Copy(_In_ CMultipleVariable& rmvMultiConstant);
			
			/**
			* CMultipleVariable 설정을 깊은 복사합니다.
			*
			* param[in] pRmcMultiConstant : 설정 데이터 (Pointer)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			_Out_ bool Copy(_In_ CMultipleVariable* pRmcMultiConstant);
			
			/**
			* 해당 인덱스에 입력 값을 설정합니다.
			*
			* param[in] stIndex : 입력 인덱스
			* param[in] dblVal : 입력 값
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			_Out_ bool SetAt(_In_ size_t stIndex, _In_ double dblVal);
			
			/**
			* 설정된 해당 인덱스의 값을 가져옵니다.
			*
			* param[in] stIndex : 입력 인덱스
			*
			* return value : double 의 설정 값를 반환합니다.
			*/
			_Out_ double GetAt(_In_ size_t stIndex);
			
			/**
			* 해당 인덱스에 입력 값을 추가합니다.
			*
			* param[in] stIndex : 입력 인덱스
			* param[in] dblVal : 입력 값
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			_Out_ bool InsertAt(_In_ size_t stIndex, _In_ double dblVal);
			
			/**
			* 해당 인덱스에 입력 값을 삭제합니다.
			*
			* param[in] stIndex : 입력 인덱스
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			_Out_ bool DeleteAt(_In_ size_t stIndex);
			
			/**
			* 입력 값을 추가합니다.
			*
			* param[in] dblValue : 입력 값
			*/
			_Out_ void AddValue(_In_ double dblValue);
			
			/**
			* 설정된 값의 개수를 가져옵니다.
			*
			* return value : size_t 의 설정 개수를 반환합니다.
			*/
			_Out_ size_t GetSize();

		protected:
			CInterMultipleVariable* m_pInternal = nullptr;
			

		};
	}
}
