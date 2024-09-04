#pragma once

#include "RavidRect.h"

#include <vector>

namespace Ravid
{
	namespace Algorithms
	{
		class CInternalShapeFinder;
		/**
		* 이미지의 특징점을 이용하여 목표물을 찾는 클래스입니다.
		*/
		class AFX_EXT_CLASS CShapeFinder : public CRavidObject
		{
		public:
			CShapeFinder();
			virtual ~CShapeFinder();

			RavidPreventCopySelf(CShapeFinder);
			
			/**
			* 찾은 결과 구조체입니다.
			*/
			typedef struct _sShapeFinderResult
			{
				float fScore;
				float fAngle;
				float fScale;
				CRavidRect<double> rrRegion;

			} SShapeFinderResult;
			
			/**
			* 학습 알고리즘을 실행합니다.
			*
			* param[in] pImgInfo : 입력 이미지 (Pointer)
			* param[in_opt] pGeometry : 검사 ROI (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Learn(_In_ CRavidImage* pImgInfo, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 탐색 알고리즘을 실행합니다.
			*
			* param[in] pImgInfo : 입력 이미지 (Pointer)
			* param[out] refVctResults : 탐색 결과 리스트 (Reference)
			* param[in_opt] pGeometry : 검사 ROI (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Find(_In_ CRavidImage* pImgInfo, _Out_ std::vector<SShapeFinderResult>& refVctResults, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 입력된 정보의 특징점을 계산합니다.
			*
			* param[in] fAngleDegree : 회전 각도
			* param[in] fScale : 스케일
			* param[out] vctFeaturePoints : 특징점 좌표 리스트 출력 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetModelFeaturePoints(_In_ float fAngleDegree, _In_ float fScale, _Out_ std::vector<CRavidPoint<float>>& vctFeaturePoints);
			
			/**
			* 학습 및 검사에 사용될 이미지 레벨 수를 반환합니다.
			*
			* return value : int 타입의 이미지 레벨 수를 반환합니다.
			*/
			int			     GetModelLevelNumber() const;
			
			/**
			* 학습 완료 여부를 판단합니다.
			*
			* return value : bool 타입의 학습 완료 여부를 반환합니다.
			*/
			bool			 GetLearningDone() const;		

			/**
			* 학습 정보 파일을 자장합니다.
			*
			* param[in] strModelFile : 저장 경로
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Save(_In_ std::wstring strModelFile);
			
			/**
			* 학습 정보 파일을 읽어옵니다.
			*
			* param[in] strModelFile : 로드 경로
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Load(_In_ std::wstring strModelFile);
			
			/**
			* 찾을 목표 객체 수를 설정합니다.
			*
			* param[in] nInstanceNumber : 객체의 수
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetInstanceNumber(_In_ int nInstanceNumber);
			
			/**
			* 설정된 찾을 목표 객체 수를 가져옵니다.
			*
			* return value : int 타입의 찾을 목표 객체 수를 반환합니다.
			*/
			int				 GetInstanceNumber() const;
			
			/**
			* 찾을 객체의 시작 각도를 설정합니다.
			*
			* param[in] fAngleDegreeStart : 시작 각도
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetAngleStart(_In_ float fAngleDegreeStart);
			
			/**
			* 설정된 찾을 객체의 시작 각도를 가져옵니다.
			*
			* return value : float 타입의 찾을 객체의 시작 각도를 반환합니다.
			*/
			float			 GetAngleStart() const;
			
			/**
			* 찾을 객체의 끝 각도를 설정합니다.
			*
			* param[in] fAngleDegreeEnd : 끝 각도
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetAngleEnd(_In_ float fAngleDegreeEnd);
			
			/**
			* 설정된 찾을 객체의 끝 각도를 가져옵니다.
			*
			* return value : float 타입의 찾을 객체의 끝 각도를 반환합니다.
			*/
			float			 GetAngleEnd() const;
			
			/**
			* 찾을 객체의 최소 크기를 설정합니다.
			*
			* param[in] fScaleMin : 최소 크기
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetScaleMin(_In_ float fScaleMin);
			
			/**
			* 설정된 찾을 객체의 최소 크기를 가져옵니다.
			*
			* return value : float 타입의 찾을 객체의 최소 크기를 반환합니다.
			*/
			float			 GetScaleMin() const;
			
			/**
			* 찾을 객체의 최대 크기를 설정합니다.
			*
			* param[in] fScaleMax : 최대 크기
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetScaleMax(_In_ float fScaleMax);
			
			/**
			* 설정된 찾을 객체의 최대 크기를 가져옵니다.
			*
			* return value : float 타입의 찾을 객체의 최대 크기를 반환합니다.
			*/
			float			 GetScaleMax() const;
			
			/**
			* 찾을 객체의 최소 스코어를 설정합니다.
			*
			* param[in] fScaleMax : 최소 스코어
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetMinScore(_In_ float fMinScore);
			
			/**
			* 설정된 찾을 객체의 최소 스코어를 가져옵니다.
			*
			* return value : float 타입의 찾을 객체의 최소 스코어를 반환합니다.
			*/
			float			 GetMinScore() const;
			
			/**
			* 찾을 객체의 밝기 반전을 설정합니다.
			*
			* param[in] bInvertedContrast : 밝기 반전
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetContrast(_In_ bool bInvertedContrast);
			
			/**
			* 설정된 찾을 객체의 밝기 반전을 가져옵니다.
			*
			* return value : bool 타입의 찾을 객체의 밝기 반전을 반환합니다.
			*/
			bool			 GetContrast() const;
			
			/**
			* 찾을 객체의 확장 영역 크기를 설정합니다.
			*
			* param[in] fExtendedArea : 확장 영역 크기
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetExtendedArea(_In_ float fExtendedArea);
			
			/**
			* 설정된 찾을 객체의 확장 영역 크기를 가져옵니다.
			*
			* return value : float 타입의 찾을 객체의 확장 영역 크기를 반환합니다.
			*/
			float			 GetExtendedArea() const;
			
			/**
			* 찾을 객체의 특징점 추출 최대 깊이를 설정합니다.
			*
			* param[in] nMaxLevel : 특징점 추출 최대 깊이
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetMaxLevel(_In_ int nMaxLevel);
			
			/**
			* 설정된 찾을 객체의 특징점 추출 최대 깊이를 가져옵니다.
			*
			* return value : int 타입의 찾을 객체의 특징점 추출 최대 깊이를 반환합니다.
			*/
			int				 GetMaxLevel() const;
			
			/**
			* 찾을 객체의 특징점 최소 개수를 설정합니다.
			*
			* param[in] nMaxLevel : 특징점 최소 개수
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetMinFeaturePoints(_In_ int nMinFeaturePoints);
			
			/**
			* 설정된 찾을 객체의 특징점 최소 개수를 가져옵니다.
			*
			* return value : int 타입의 찾을 객체의 특징점 최소 개수를 반환합니다.
			*/
			int				 GetMinFeaturePoints() const;
			
			/**
			* 찾을 객체의 특징점 최대 개수를 설정합니다.
			*
			* param[in] nMaxLevel : 특징점 최대 개수
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetMaxFeaturePoints(_In_ int nMaxFeaturePoints);
			
			/**
			* 설정된 찾을 객체의 특징점 최대 개수를 가져옵니다.
			*
			* return value : int 타입의 찾을 객체의 특징점 최대 개수를 반환합니다.
			*/
			int				 GetMaxFeaturePoints() const;
			
			/**
			* 찾을 객체의 특징 강도를 설정합니다.
			*
			* param[in] fStrength : 특징 강도
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetStrength(_In_ float fStrength);

			/**
			* 설정된 찾을 객체의 특징 강도를 가져옵니다.
			*
			* return value : float 타입의 찾을 객체의 특징 강도를 반환합니다.
			*/
			float			 GetStrength() const;
			
			/**
			* 찾을 객체의 자동 특징 강도 사용을 설정합니다.
			*
			* param[in] bStrengthModeAuto : 자동 특징 강도 사용
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetStrengthModeAuto(_In_ bool bStrengthModeAuto);

			/**
			* 설정된 찾을 객체의 자동 특징 강도 사용을 가져옵니다.
			*
			* return value : bool 타입의 찾을 객체의 자동 특징 강도 사용을 반환합니다.
			*/
			bool			 GetStrengthModeAuto() const;

			/// MASKING ////
			/**
			* MASKING 기능을 위해 추가된 함수입니다.
			* 학습 모델 이미지를 가져옵니다.
			*
			* param[in] pRiSrc : 학습 이미지
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetModelImage(_In_ CRavidImage* pRiSrc) const;
			
			/**
			* MASKING 기능을 위해 추가된 함수입니다.
			* 입력된 영역 내부의 특징점을 제거합니다.
			*
			* param[in] pRegion : 특징점 제거 영역
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult CleanFeaturePoint(_In_ CRavidGeometryArray* pRegion);
			
			/**
			* MASKING 기능을 위해 추가된 함수입니다.
			* 입력된 이미지에서 BitMaskValue 값의 픽셀 영역 특징점을 제거합니다.
			*
			* param[in] pImage : 특징점 제거 이미지 (1채널 8bit 이미지로 학습 이미지 크기와 동일해야 함)
			* param[in_opt] bitMaskValue : 특징점 제거 지정 할 픽셀 값
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult CleanFeaturePoint(_In_ CRavidImage* pImage, _In_opt_ BYTE bitMaskValue = 0);
			
			/**
			* MASKING 기능을 위해 추가된 함수입니다.
			* 이전의 특징점 상태로 복원합니다.
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult UndoCleanFeaturePoint();

			/**
			* MASKING 기능을 위해 추가된 함수입니다.
			* 현재 특징점 상태를 학습 모델에 적용합니다.
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult CommitCleanFeaturePoint();

			/**
			* MASKING 기능을 위해 추가된 함수입니다.
			* 초기의 특징점 상태로 복원합니다.
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult ResetCleanFeaturePoint();

		private:
			CInternalShapeFinder* m_pInternal;
		};

	}
}

