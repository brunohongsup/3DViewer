#pragma once

namespace Ravid
{
	namespace Miscellaneous
	{
		template <typename T>
		class CInternalNumberFormat;
		
		/**
		* 입력된 값을 문자열로 변환하는 클래스입니다.
		*/
		template <typename T>
		class AFX_EXT_CLASS CNumberFormat : public CRavidObject
		{
			RavidUseDynamicCreation();
			RavidPreventCopySelf(CNumberFormat);

		public:
			CNumberFormat();
			virtual ~CNumberFormat();
			
			/**
			* 입력된 값을 가져옵니다.
			*
			* return value : Template type 의 데이터를 반환합니다.
			*/
			T GetNumber();
			
			/**
			* Int 형 데이터를 설정합니다.
			*
			* param[in] nValue : 입력 값
			*/
			void SetNumber(_In_ int nValue);
			
			/**
			* long long 형 데이터를 설정합니다.
			*
			* param[in] llValue : 입력 값
			*/
			void SetNumber(_In_ long long llValue);
			
			/**
			* float 형 데이터를 설정합니다.
			*
			* param[in] fValue : 입력 값
			*/
			void SetNumber(_In_ float fValue);
			
			/**
			* double 형 데이터를 설정합니다.
			*
			* param[in] dblValue : 입력 값
			*/
			void SetNumber(_In_ double dblValue);
			
			/**
			* 설정된 소수점 이하 출력 자릿수를 가져옵니다.
			*
			* return value : long 의 출력 자릿수를 반환합니다.
			*/
			long GetDecimalPlace();
			
			/**
			* 소수점 이하 출력 자릿수를 설정합니다.
			*
			* param[in] nValue : 자릿수
			*/
			void SetDecimalPlace(_In_ long nValue);
			
			/**
			* 설정된 콤마를 포함 여부를 가져옵니다.
			*
			* return value : bool 의 콤마를 포함 여부를 반환합니다.
			*/
			bool GetIncludingComma();
			
			/**
			* 소수의 경우 콤마를 포함 여부를 설정합니다.
			*
			* param[in] bSet : 콤마 포함 설정
			*/
			void SetIncludingComma(_In_ bool bSet);
			
			/**
			* 설정된 데이터를 문자열로 계산합니다.
			*
			* return value : CString 의 변환된 문자열을 반환합니다.
			*/
			CString GetResult();

		protected:
			CInternalNumberFormat<T>* m_pInternal = nullptr;
		
		};
	}
}

