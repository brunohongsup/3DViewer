#pragma once

#include "RavidRect.h"

#include <vector>

namespace Ravid
{
	namespace Algorithms
	{
		class CInternalObjectFinder;
		/**
		* 이미지의 화소 값을 이용하여 목표물을 찾는 클래스입니다.
		*/
		class AFX_EXT_CLASS CObjectFinder : public CRavidObject
		{
		public:
			CObjectFinder();
			virtual ~CObjectFinder();

			RavidPreventCopySelf(CObjectFinder);
			
			/**
			* 찾은 결과 구조체입니다.
			*/
			typedef struct sObjectFinderRt
			{
				float fScore;
				float fAngle;
				float fScale;
				CRavidRect<double> rrResultRegion;
			}
			SObjectFinderResult;
			
			/**
			* 해석 데이터 및 설정을 초기화 합니다. 
			*/
			bool Clear();

			/**
			* 학습 알고리즘을 실행합니다.
			*
			* param[in] refImgInfo : 입력 이미지 (Reference)
			* param[in_opt] pGeometry : 검사 ROI (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Learn(_In_ CRavidImage& refImgInfo, _In_opt_ CRavidGeometry* pGeometry = nullptr);

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
			* 학습 정보를 가지고 있는지 확인합니다. 
			*
			* return value : bool 형태로 반환합니다.
			*/
			bool IsLearned();
			
			/**
			* 탐색 알고리즘을 실행합니다.
			*
			* param[in] refImgInfo : 입력 이미지 (Reference)
			* param[in_opt] pGeometry : 검사 ROI (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Find(_In_ CRavidImage& refImgInfo, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 탐색 알고리즘을 실행합니다.
			*
			* param[in] pImgInfo : 입력 이미지 (Pointer)
			* param[in_opt] pGeometry : 검사 ROI (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Find(_In_ CRavidImage* pImgInfo, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 분석을 실행 후 사용합니다.
			* 탐색된 결과 리스트를 가져옵니다.
			*
			* param[out] refVctResults : 탐색 결과 리스트 (Reference)
			*	SObjectFinderResult 구조체의 벡터로 반환
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetResults(_Out_ std::vector<SObjectFinderResult>& refVctResults);

			/**
			* 학습 정보 파일을 읽어옵니다.
			* The file extension is ".rof"
			*
			* param[in] strPatternFile : 로드 경로
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Load(_In_ std::wstring strPatternFile);

			/**
			* 학습 정보 파일을 자장합니다.
			* The file extension is ".rof"
			*
			* param[in] strPatternFile : 저장 경로
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Save(_In_ std::wstring strPatternFile);
			
			/**
			* 학습 패턴 이미지를 가져옵니다.
			*
			* param[out] strPatternFile : 패턴 이미지 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetPatternImage(_Out_ CRavidImage& refPatternImage);
			
			/**
			* 학습 패턴 이미지를 가져옵니다.
			*
			* param[out] pPatternImage : 패턴 이미지 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetPatternImage(_Out_ CRavidImage* pPatternImage);
			
			/**
			* 찾을 패턴의 최대 개수를 설정합니다.
			*
			* param[in_opt] stMaxInstance : 최대 패턴 개수
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetMaxInstance(_In_opt_ size_t stMaxInstance = 1);
			
			/**
			* 찾을 패턴의 최대 개수를 가져옵니다.
			*
			* return value : size_t타입의 최대 패턴 개수를 반환합니다.
			*/
			size_t GetMaxInstance();
			
			/**
			* 찾을 패턴의 최소 스코어를 설정합니다.
			*
			* param[in_opt] fScore : 최소 스코어
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetMinimalDetectingScore(_In_opt_ float fScore = .5f);
			
			/**
			* 찾을 패턴의 최소 스코어를 가져옵니다.
			*
			* return value : float 타입의 최소 스코어를 반환합니다.
			*/
			float GetMinimalDetectingScore();
			
			/**
			* 찾을 패턴의 기준 각도를 설정합니다.
			*
			* param[in_opt] fAngleBias : 기준 각도
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetAngleBias(_In_opt_ float fAngleBias = 0.f);
			
			/**
			* 찾을 패턴의 기준 각도를 가져옵니다.
			*
			* return value : float 타입의 기준 각도를 반환합니다.
			*/
			float GetAngleBias();
			
			/**
			* 찾을 패턴의 탐색 각도 범위를 설정합니다.
			*
			* param[in_opt] fAngleTolerance : 절대 각도 범위
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetAngleTolerance(_In_opt_ float fAngleTolerance = 0.f);
			
			/**
			* 찾을 패턴의 탐색 각도 범위를 가져옵니다.
			*
			* return value : float 타입의 절대 각도 범위를 반환합니다.
			*/
			float GetAngleTolerance();
			
			/**
			* 찾을 패턴의 기준 크기를 설정합니다.
			*
			* param[in_opt] fAngleBias : 기준 크기
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetScaleBias(_In_opt_ float fScaleBias = 1.f);
			
			/**
			* 찾을 패턴의 기준 크기를 가져옵니다.
			*
			* return value : float 타입의 기준 크기를 반환합니다.
			*/
			float GetScaleBias();
			
			/**
			* 찾을 패턴의 탐색 크기 범위를 설정합니다.
			*
			* param[in_opt] fAngleTolerance : 절대 크기 범위
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetScaleTolerance(_In_opt_ float fScaleTolerance = 0.f);
			
			/**
			* 찾을 패턴의 탐색 크기 범위를 가져옵니다.
			*
			* return value : float 타입의 절대 크기 범위를 반환합니다.
			*/
			float GetScaleTolerance();
			
			/**
			* 패턴 탐색 시 이미지 보간 사용 여부를 설정합니다.
			*
			* param[in_opt] bUse : 보간 사용 여부
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetInterpolation(_In_opt_ bool bUse = true);
			
			/**
			* 패턴 탐색 시 이미지 보간 사용 여부를 가져옵니다.
			*
			* return value : bool 타입의 이미지 보간 사용 여부를 반환합니다.
			*/
			bool GetInterpolation();
			
			/**
			* 패턴 탐색 시 이미지 다운 샘플링 사용 여부를 설정합니다.
			*
			* param[in_opt] bUse :이미지 다운 샘플링 사용 여부
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetUseDownSampling(_In_ bool bUse);
			
			/**
			* 패턴 탐색 시 이미지 다운 샘플링 사용 여부를 가져옵니다.
			*
			* return value : bool 타입의 이미지 다운 샘플링 사용 여부를 반환합니다.
			*/
			bool GetUseDownSampling();

		private:

			CInternalObjectFinder* m_pInternal = nullptr;
		};
	}
}

