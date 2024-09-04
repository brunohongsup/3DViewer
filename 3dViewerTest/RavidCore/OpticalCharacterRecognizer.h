#pragma once

#include <vector>
#include <string>
#include <memory>
#include "RavidQuadrangle.h"
#include "RavidRect.h"

namespace Ravid
{
	namespace Algorithms
	{
		class CInternalOCR;
		class CRavidImage;

		struct SOCRCandidate
		{
			CRavidRect<int>		 rrBoundary;
	
			wchar_t	cCharacter;
			float	fScore;
			float	fAngle;

			int		nWordID;
		};

		struct SOCRModel
		{
			wchar_t cCharacter;
			double dblPriorProb;
		};
		/**
		* 이미지의 문자를 학습, 판독하여 결과를 제공하는 클래스입니다.
		*/
		class AFX_EXT_CLASS COpticalCharacterRecognizer : public CRavidObject
		{
			RavidUseDynamicCreation();
			RavidPreventCopySelf(COpticalCharacterRecognizer);

		public:
			COpticalCharacterRecognizer();
			virtual ~COpticalCharacterRecognizer();
	
			/**
			* 문자와 배경의 색을 결정합니다.
			*/
			enum ECharColor
			{
				WhiteOnBlack = 0,
				BlackOnWhite
			};
			
			/**
			* 세그먼트 알고리즘을 실행합니다.
			*
			* param[in] pImgInfo : 입력 이미지 (Pointer)
			* param[out] refVctSegmentedCandidates : 이미지에서 문자를 세그먼트화 정보 (Reference)
			* param[in_opt] pGeometry : 검사 ROI (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Segment(_In_ CRavidImage* pImgInfo, _Out_ std::vector<SOCRCandidate>& refVctSegmentedCandidates, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 학습 알고리즘을 실행합니다.
			*
			* param[in] pImgInfo : 입력 이미지 (Pointer)
			* param[in] refVctSegmentedCandidates : 이미지에서 문자의 세그먼트화 정보 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Train(_In_ CRavidImage* pImgInfo, _In_ const std::vector<SOCRCandidate>& refVctSegmentedCandidates);
			
			/**
			* 검사 알고리즘을 실행합니다.
			*
			* param[in] pImgInfo : 입력 이미지 (Pointer)
			* param[out] refVctResultWords : 이미지에서 해석된 문자열 정보 (Reference)
			* param[out] refVctResultCandidates : 이미지에서 해석된 문자 정보 (Reference)
			* param[in_opt] pGeometry : 검사 ROI (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Decode(_In_ CRavidImage* pImgInfo, _Out_ std::vector<std::wstring>& refVctResultWords, _Out_ std::vector<SOCRCandidate>& refVctResultCandidates, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 학습 정보 파일을 읽어옵니다.
			*
			* param[in] refStrPathFile : 로드 경로
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Load(_In_ const std::wstring& refStrPathFile);
			
			/**
			* 학습 정보 파일을 자장합니다.
			*
			* param[in] refStrPathFile : 입력 이미지
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Save(_In_ const std::wstring& refStrPathFile);

			/**
			* 학습된 문자 정보 리스트를 가져옵니다.
			*
			* param[out] refVctModels : 문자 정보 리스트
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetCharacterModelList(std::vector<SOCRModel>& refVctModels) const;
			
			/**
			* 입력된 문자를 학습된 정보 리스트에서 삭제합니다.
			*
			* param[in] cCharacter : 문자 정보
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult DeleteCharacterModel(_In_ wchar_t cCharacter);
			
			/**
			* 입력된 문자를 학습된 정보 리스트에서 이미지로 가져옵니다.
			*
			* param[in] cCharacter : 문자 정보
			* param[out] pImgInfo : 학습된 이미지
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetCharacterModelImage(_In_ wchar_t cCharacter, _Out_ CRavidImage* pImgInfo);
			
			/**
			* 문자와 배경을 구분하는 값을 설정합니다.
			*
			* param[in] nThreshold : 문자와 배경의 기준값
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetThreshold(_In_ int nThreshold);
			
			/**
			* 설정된 문자와 배경을 구분하는 값을 가져옵니다.
			*
			* return value : int 타입으로 문자, 배경의 분류 기준값을 반환합니다.
			*/
			int				 GetThreshold() const;
			
			/**
			* 문자로 인식하기 위한 최소 스코어를 설정합니다.
			*
			* param[in] dblMinScore : 최소 스코어 값
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetMinScore(_In_ double dblMinScore);
			
			/**
			* 설정된 문자로 인식하기 위한 최소 스코어를 가져옵니다.
			*
			* return value : double 타입으로 문자로 인식하기 위한 최소 스코어를 반환합니다.
			*/
			double			 GetMinScore() const;
			
			/**
			* 문자로 인식하기 위한 최소 가로 길이를 설정합니다.
			*
			* param[in]nMinWidth : 최소 가로 길이
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetMinWidth(_In_ int nMinWidth);
			
			/**
			* 설정된 문자로 인식하기 위한 최소 가로 길이를 가져옵니다.
			*
			* return value : int 타입으로 문자로 인식하기 위한 최소 가로 길이를 반환합니다.
			*/
			int				 GetMinWidth() const;
			
			/**
			* 문자로 인식하기 위한 최대 가로 길이를 설정합니다.
			*
			* param[in]nMaxWidth : 최대 가로 길이
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetMaxWidth(_In_ int nMaxWidth);
			
			/**
			* 설정된 문자로 인식하기 위한 최대 가로 길이를 가져옵니다.
			*
			* return value : int 타입으로 문자로 인식하기 위한 최대 가로 길이를 반환합니다.
			*/
			int				 GetMaxWidth() const;
			
			/**
			* 문자로 인식하기 위한 최소 세로 길이를 설정합니다.
			*
			* param[in]nMinHeight : 최소 세로 길이
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetMinHeight(_In_ int nMinHeight);
			
			/**
			* 설정된 문자로 인식하기 위한 최소 세로 길이를 가져옵니다.
			*
			* return value : int 타입으로 문자로 인식하기 위한 최소 세로 길이를 반환합니다.
			*/
			int				 GetMinHeight() const;
			
			/**
			* 문자로 인식하기 위한 최대 세로 길이를 설정합니다.
			*
			* param[in]nMaxWidth : 최대 세로 길이
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetMaxHeight(_In_ int nMaxHeight);
			
			/**
			* 설정된 문자로 인식하기 위한 최대 세로 길이를 가져옵니다.
			*
			* return value : int 타입으로 문자로 인식하기 위한 최대 세로 길이를 반환합니다.
			*/
			int				 GetMaxHeight() const;
			
			/**
			* 문자의 색을 설정합니다.
			*
			* param[in]eColor : enum ECharColor 으로 색을 결정
			*	WhiteOnBlack : 검정 배경 흰 문자
			*	BlackOnWhite : 흰 배경 검정 문자
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetCharacterColor(_In_ ECharColor eColor);
			
			/**
			* 설정된 문자의 색을 가져옵니다.
			*
			* return value : enum ECharColor 타입으로 문자의 색을 반환합니다.
			*	WhiteOnBlack : 검정 배경 흰 문자
			*	BlackOnWhite : 흰 배경 검정 문자
			*/
			ECharColor		 GetCharacterColor() const;
			
			/**
			* 탐색된 문자를 방향에 따른 하나의 문자열로 처리 여부를 설정합니다.
			*
			* param[in_opt] bUse : 방향 및 거리로 문자열로 변경 사용
			*	true : 연속된 문자열로 처리
			*	false : 탐색된 순서대로 처리
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetUseWordSegmentation(_In_ bool bUse);
			
			/**
			* 탐색된 문자를 방향에 따른 하나의 문자열로 처리 여부를 가져옵니다.
			*
			* return value : bool 타입의 방향 및 거리로 문자열로 변경 사용 여부를 반환합니다.
			*/
			bool			 GetUseWordSegmentation() const;
			
			/**
			* 연속된 문자열로 변경시 문자간 간격을 설정합니다.
			*
			* param[in] dblGap : 문자간 간격
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetWordSegmentationGap(_In_ double dblGap);
			
			/**
			* 연속된 문자열로 변경시 문자간 간격을 가져옵니다.
			*
			* return value : double 타입의 문자간 간격을 반환합니다.
			*/
			double			 GetWordSegmentationGap() const;

		private:
			std::unique_ptr<CInternalOCR> m_pInternal;
		};

	}
}

