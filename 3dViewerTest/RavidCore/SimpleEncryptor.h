#pragma once

namespace Ravid
{
	namespace Miscellaneous
	{
		class CInternalSimpleEncryptor;

		/**
		* 입력된 데이터를 단순 암호화, 복호화하는 클래스입니다.
		*/
		class CSimpleEncryptor
		{
		public:
			CSimpleEncryptor();
			virtual ~CSimpleEncryptor();

			void ResetTransaction();
			
			/**
			* 입력된 데이터를 암호화합니다.
			*
			* param[in_out] pData : 입출력 데이터
			* param[in] nSize : 암호화 할 길이
			* 시작으로부터 nSize 만큼 암호화 합니다.
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool EncryptData(_Inout_ char* pData, _In_ int nSize);
			
			/**
			* 입력된 데이터를 복호화합니다.
			*
			* param[in_out] pData : 입출력 데이터
			* param[in] nSize : 복호화 할 길이
			* 시작으로부터 nSize 만큼 복호화 합니다.
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool DecryptData(_Inout_ char* pData, _In_ int nSize);

		private:
			BYTE BitRL(_In_ char cIn, _In_ int nCount);
			BYTE BitRR(_In_ char cIn, _In_ int nCount);
			CInternalSimpleEncryptor* m_pInternal = nullptr;
			
		};
	}
}

