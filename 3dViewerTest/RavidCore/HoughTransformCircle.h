#pragma once

#include <vector>

class CRavidGeometry;

namespace Ravid
{
	namespace Algorithms
	{
		class CInternalHoughTransformCircle;
		class CRavidImage;

		/**
		* 이미지의 화소에서 원을 감지, 추출하는 클래스입니다.
		*/
		class AFX_EXT_CLASS CHoughTransformCircle : public CRavidObject
		{
			RavidUseDynamicCreation();
			RavidPreventCopySelf(CHoughTransformCircle);

		public:
			CHoughTransformCircle();
			virtual ~CHoughTransformCircle();
			
			/**
			* 알고리즘을 실행합니다.
			*
			* param[in] refVctRp : CRavidPoint 벡터 형태의 특징점 좌표 리스트 (Reference)
			* param[in] pGeometry : 검사 ROI (Pointer)
			* param[out] refVctCircles : CRavidCircle 벡터 형태의 원 리스트 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Transform( _In_ const std::vector<CRavidPoint<double>>& refVctRp, _In_ CRavidGeometry* pGeometry, _Out_ std::vector<CRavidCircle<double>>& refVctCircles);
			
			/**
			* 알고리즘을 실행합니다.
			*
			* param[in] pVctRp : CRavidPoint 벡터 형태의 특징점 좌표 리스트 (Pointer)
			* param[in] pGeometry : 검사 ROI (Pointer)
			* param[out] refVctCircles : CRavidCircle 벡터 형태의 원 리스트 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Transform(_In_ const std::vector<CRavidPoint<double>>* pVctRp, _In_ CRavidGeometry* pGeometry, _Out_ std::vector<CRavidCircle<double>>& refVctCircles);
			
			/**
			* 알고리즘을 실행합니다.
			*
			* param[in] pImgInfo : 입력 이미지 (Pointer)
			* param[out] refVctCircles : CRavidCircle 벡터 형태의 원 리스트 (Reference)
			* param[in_opt] pGeometry : 검사 ROI (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Transform(_In_ CRavidImage* pImgInfo, _Out_ std::vector<CRavidCircle<int>>& vctCircles, _In_opt_ CRavidGeometry* pGeometry);
			
			/**
			* 알고리즘을 실행합니다.
			*
			* param[in] pImgInfo : 입력 이미지 (Pointer)
			* param[out] pVctCircles : CRavidCircle 벡터 형태의 원 리스트 (Pointer)
			* param[in_opt] pGeometry : 검사 ROI (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Transform(_In_ CRavidImage* pImgInfo, _Out_ std::vector<CRavidCircle<int>>* pVctCircles, _In_opt_ CRavidGeometry* pGeometry);
			
			/**
			* 알고리즘을 실행합니다.
			*
			* param[in] refImgInfo : 입력 이미지 (Pointer)
			* param[out] vctCircles : CRavidCircle 벡터 형태의 원 리스트 (Reference)
			* param[in_opt] pGeometry : 검사 ROI (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Transform(_In_ CRavidImage& refImgInfo, _Out_ std::vector<CRavidCircle<int>>& vctCircles, _In_opt_ CRavidGeometry* pGeometry);
			
			/**
			* 알고리즘을 실행합니다.
			*
			* param[in] refImgInfo : 입력 이미지 (Pointer)
			* param[out] pVctCircles : CRavidCircle 벡터 형태의 원 리스트 (Pointer)
			* param[in_opt] pGeometry : 검사 ROI (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Transform(_In_ CRavidImage& refImgInfo, _Out_ std::vector<CRavidCircle<int>>* pVctCircles, _In_opt_ CRavidGeometry* pGeometry);
			
			/**
			* 특징 기준이 되는 픽셀 값을 설정합니다.
			*
			* param[in] nThreshold : 특징 기준 픽셀 값
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetThreshold(_In_ int nThreshold);
			
			/**
			* 감지된 원의 최대 개수를 설정합니다.
			*
			* param[in] nMaxCircleCnt : 원의 최대 개수
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetMaxCircleCount(_In_ int nMaxCircleCnt);
			
			/**
			* 감지된 원의 지역 범위를 설정합니다.
			* 원에 접하는 픽셀을 판정시 최대 포함 거리를 표현합니다.
			*
			* param[in] nRegionalRange : 지역 범위 절대 값
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetRegionRange(_In_ int nRegionalRange);
			
			/**
			* 감지된 원의 크기 범위를 설정합니다.
			*
			* param[in] nMinRadius : 최소 반지름 값
			* param[in] nMaxRadius : 최대 반지름 값
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetRadius(_In_ int nMinRadius, _In_ int nMaxRadius);
			
			/**
			* 설정된 특징 기준이 되는 픽셀 값을 가져옵니다.
			*
			* return value : int 형 특징 기준 픽셀 값 을 반환합니다.
			*/
			int GetThreshold() const;
			
			/**
			* 설정된 감지된 원의 최대 개수를 가져옵니다.
			*
			* return value : int 형 감지된 원의 최대 개수를 반환합니다.
			*/
			int GetMaxCircleCount() const;
			
			/**
			* 설정된 감지된 원의 지역 범위를 가져옵니다.
			*
			* return value : int 형 지역 범위 절대 값을 반환합니다.
			*/
			int GetRegionRange() const;
			
			/**
			* 설정된 감지된 원의 크기 범위를 가져옵니다.
			*
			* param[out] refMinRadius : 최소 반지름 값 (Reference)
			* param[out] refMaxRadius : 최대 반지름 값 (Reference)
			*/
			void GetRadius(_Out_ int& refMinRadius, _Out_ int& refMaxRadius) const;
		
		private:
			CInternalHoughTransformCircle* m_pInternal = nullptr;

		};
	}
}

