#pragma once

namespace Ravid
{
	namespace Miscellaneous
	{
		/**
		* 데이터 오류 체크를 위한 순환중복검사(CRC) 생성 클래스입니다.
		*/
		class AFX_EXT_CLASS CCyclicRedundancyCheck : public CRavidObject
		{
			RavidUseDynamicCreation();
			RavidPreventCopySelf(CCyclicRedundancyCheck);

		public:
			CCyclicRedundancyCheck();
			virtual ~CCyclicRedundancyCheck();
			
			/**
			* CRC8 의 결과를 계산합니다.
			*
			* param[in] pSrcData : 입력 데이터 (Pointer)
			* param[in_opt] stLength : 입력 데이터 길이 (Pointer)
			*
			* return value : BYTE (8 bit) 의 결과를 반환합니다.
			*/
			BYTE MakeCRC8(_In_ unsigned char* pSrcData, _In_ size_t stLength);
			
			/**
			* CRC16 의 결과를 계산합니다.
			*
			* param[in] pSrcData : 입력 데이터 (Pointer)
			* param[in_opt] stLength : 입력 데이터 길이 (Pointer)
			*
			* return value : unsigned short (16 bit) 의 결과를 반환합니다.
			*/
			unsigned short MakeCRC16(_In_ unsigned char* pSrcData, _In_ size_t stLength);
			
			/**
			* CRC32 의 결과를 계산합니다.
			*
			* param[in] pSrcData : 입력 데이터 (Pointer)
			* param[in_opt] stLength : 입력 데이터 길이 (Pointer)
			*
			* return value : unsigned int (32 bit) 의 결과를 반환합니다.
			*/
			unsigned int MakeCRC32(_In_ unsigned char* pSrcData, _In_ size_t stLength);
		};
	}
}

