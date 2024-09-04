#pragma once

#include "FiniteField.h"

#include <memory>
#include <vector>

/*
 Abbreviation
 - CL : Carry-Less
 - GF : Galois Field(=Finite Field)
*/

namespace Ravid
{
	namespace Mathematics
	{
		class CInternalReedSolomon;
		
		/**
		* 리드 솔로몬 부호를 계산하는 클래스입니다.
		* 이 클래스는 현재 코드분석에만 사용합니다.
		*/
		class AFX_EXT_CLASS CReedSolomon : public CFiniteField
		{
		public:
			explicit CReedSolomon(_In_opt_ int nPrim = EPrimitivePolynomial_QRCode, _In_opt_ int nFCR = EFirstConsecutiveRoot_QRCode, _In_opt_ int nGenerator = 2);
			virtual ~CReedSolomon();
	
			/**
			* QR, DataMatrix 코드 전용 원시 값입니다.
			*/
			enum EPrimitivePolynomial
			{
				EPrimitivePolynomial_QRCode = 0x11d,
				EPrimitivePolynomial_DataMatrix = 0x12d
			};
			/**
			* QR, DataMatrix 코드 전용 초기 값입니다.
			*/
			enum EFirstConsecutiveRoot
			{
				EFirstConsecutiveRoot_QRCode = 0,
				EFirstConsecutiveRoot_DataMatrix = 1
			};
			
			/**
			* 입력된 객체로 유한체 정보를 설정합니다.
			*
			* param[in] ePP : 원시 값
			* param[in] eFCR : 초기 값
			* param[in] nGenerator : 생성 변수
			*/
			void SetFiniteField(EPrimitivePolynomial ePP, EFirstConsecutiveRoot eFCR, int nGenerator);
			
			/**
			* 입력된 객체로 부호화된 데이터를 계산합니다.
			*
			* param[in] vctMsgIn : 입력 데이터 (Reference)
			* param[in] nErrCnt : 입력 에러 카운트
			*
			* return value : std::vector<int> 의 변환을 반환합니다.
			*/
			std::vector<int> Encode(_In_ const std::vector<int>& vctMsgIn, _In_ int nErrCnt);
			
			/**
			* 입력된 객체로 부호화된 데이터를 계산합니다.
			*
			* param[in] vctMsgIn : 입력 데이터 (Reference)
			* param[in] nErrCnt : 입력 에러 카운트
			* param[out] vctData : 출력 데이터 (Reference)
			* param[out] vctErrorCorrection : 출력 오류 정정 데이터 (Reference)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool Encode(_In_ const std::vector<int>& vctMsgIn, _In_ int nErrCnt, _Out_ std::vector<int>& vctData, _Out_ std::vector<int>& vctErrorCorrection);
			
			/**
			* 입력된 객체로 부호화된 데이터를 복호화 계산합니다.
			*
			* param[in] vctSrc : 입력 데이터 (Reference)
			* param[in] nErrCnt : 입력 에러 카운트
			* param[out] vctCorrectedMsg : 출력 정정 데이터 (Reference)
			* param[out] vctCorrectedEcc : 출력 오류 정정 데이터 (Reference)
			* param[in_opt] vctErasePos : 입력 소거 포지션 (Reference)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool Decode(_In_ const std::vector<int>& vctSrc, _In_ int nErrCnt, _Out_ std::vector<int>& vctCorrectedMsg, _Out_ std::vector<int>& vctCorrectedEcc, _In_opt_ const std::vector<int>& vctErasePos = std::vector<int>());
			
			/**
			* 입력된 객체로 부호화된 데이터를 복호화 계산합니다.
			*
			* param[in] vctSrc : 입력 데이터 (Reference)
			* param[in] nErrCnt : 입력 에러 카운트
			* param[out] vctCorrectedMsg : 출력 정정 데이터 (Reference)
			* param[out] vctCorrectedEcc : 출력 오류 정정 데이터 (Reference)
			* param[in_opt] vctErasePos : 입력 소거 포지션 (Reference)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool Decode(_In_ const std::vector<unsigned char>& vctSrc, _In_ int nErrCnt, _Out_ std::vector<int>& vctCorrectedMsg, _Out_ std::vector<int>& vctCorrectedEcc, _In_opt_ const std::vector<int>& vctErasePos = std::vector<int>());
			
			/**
			* 설정된 설정을 초기화합니다.
			*/
			void Clear();
			
			/**
			* 복호화하며 발생한 소거 횟수를 계산합니다.
			*
			* return value : int 의 소거 횟수를 반환합니다.
			*/
			int GetErrorErasureCount(void);

		private:
			std::vector<int> GeneratorPoly(_In_ int nErrCnt);

			std::vector<int> CalcSyndromes(_In_ const std::vector<int>& vctMsg, _In_ int nErrCnt);
			bool Check(_In_ const std::vector<int>& vctMsg, _In_ int nErrCnt);

			std::vector<int> FindErrataLocator(_In_ const std::vector<int>& vctErrPos);
			std::vector<int> FindErrorEvaluator(_In_ const std::vector<int>& vctSynd, _In_ const std::vector<int>& vctErrLoc, _In_ int nErrCnt);
			std::vector<int> CorrectErrata(_In_ const std::vector<int>& vctMsgIn, _In_ const std::vector<int>& vctSynd, _In_ const std::vector<int>& vctErrPos);

			bool FindErrorLocator(_In_ const std::vector<int>& vctSynd, _In_ int nErrCnt, _Out_ std::vector<int>& vctErrLoc, _In_opt_ const std::vector<int>& vctEraseLoc = std::vector<int>(), _In_opt_ int nEraseCount = 0);
			bool FindErrors(_In_ const std::vector<int>& vctErrLoc, _In_ int nMsgCnt, _Out_ std::vector<int>& vctErrPos);
			std::vector<int> ForneySyndromes(_In_ const std::vector<int>& vctSynd, _In_ const std::vector<int>& vctPos, _In_ int nErrCnt);

			bool CorrectMessage(_In_ const std::vector<int>& vctMsgIn, _In_ int nErrCnt, _Out_ std::vector<int>& vctCorrectedMsg, _Out_ std::vector<int>& vctCorrectedEcc, _In_opt_ const std::vector<int>& vctErasePos = std::vector<int>());

		private:
			std::unique_ptr<CInternalReedSolomon> m_pInternal;
		};
	}
}






