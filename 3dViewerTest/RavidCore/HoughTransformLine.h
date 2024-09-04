#pragma once

#include <vector>

class CRavidGeometry;

namespace Ravid
{
	namespace Algorithms
	{
		class CInternalHoughTransformLine;
		class CRavidImage;
		/**
		* 이미지의 화소에서 직선을 감지, 추출하는 클래스입니다.
		*/
		class AFX_EXT_CLASS CHoughTransformLine : public CRavidObject
		{
			RavidUseDynamicCreation();
			RavidPreventCopySelf(CHoughTransformLine);

		public:

			CHoughTransformLine();
			virtual ~CHoughTransformLine();
			
			/**
			* 알고리즘을 실행합니다.
			*
			* param[in] refVctRp : CRavidPoint 벡터 형태의 특징점 좌표 리스트 (Reference)
			* param[in] pGeometry : 검사 ROI (Pointer)
			* param[out] refVctLines : CRavidLine 벡터 형태의 직선 리스트 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Transform(_In_ const std::vector<CRavidPoint<double> >& refVctRp, _In_ CRavidGeometry* pGeometry, _Out_ std::vector<CRavidLine<double> >& refVctLines);
			
			/**
			* 알고리즘을 실행합니다.
			*
			* param[in] pVctRp : CRavidPoint 벡터 형태의 특징점 좌표 리스트 (Pointer)
			* param[in] pGeometry : 검사 ROI (Pointer)
			* param[out] refVctLines : CRavidLine 벡터 형태의 직선 리스트 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Transform(_In_ const std::vector<CRavidPoint<double> >* pVctRp, _In_ CRavidGeometry* pGeometry, _Out_ std::vector<CRavidLine<double> >& refVctLines);
			
			/**
			* 알고리즘을 실행합니다.
			*
			* param[in] pImgInfo : 입력 이미지 (Pointer)
			* param[out] vctLines : CRavidLine 벡터 형태의 직선 리스트 (Reference)
			* param[in_opt] pGeometry : 검사 ROI (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Transform(_In_ CRavidImage* pImgInfo, _Out_ std::vector<CRavidLine<int>>& vctLines, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 알고리즘을 실행합니다.
			*
			* param[in] pImgInfo : 입력 이미지 (Pointer)
			* param[out] pVctLines : CRavidLine 벡터 형태의 직선 리스트 (Pointer)
			* param[in_opt] pGeometry : 검사 ROI (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Transform(_In_ CRavidImage* pImgInfo, _Out_ std::vector<CRavidLine<int>>* pVctLines, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 알고리즘을 실행합니다.
			*
			* param[in] refImgInfo : 입력 이미지 (Pointer)
			* param[out] vctLines : CRavidLine 벡터 형태의 직선 리스트 (Reference)
			* param[in_opt] pGeometry : 검사 ROI (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Transform(_In_ CRavidImage& refImgInfo, _Out_ std::vector<CRavidLine<int>>& vctLines, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 알고리즘을 실행합니다.
			*
			* param[in] refImgInfo : 입력 이미지 (Pointer)
			* param[out] pVctLines : CRavidLine 벡터 형태의 직선 리스트 (Pointer)
			* param[in_opt] pGeometry : 검사 ROI (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Transform(_In_ CRavidImage& refImgInfo, _Out_ std::vector<CRavidLine<int>>* pVctLines, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 특징 기준이 되는 픽셀 값을 설정합니다.
			*
			* param[in] nThreshold : 특징 기준 픽셀 값
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetThreshold(_In_ int nThreshold);
			
			/**
			* 설정된 특징 기준이 되는 픽셀 값을 가져옵니다.
			*
			* return value : int 형 특징 기준 픽셀 값 을 반환합니다.
			*/
			int GetThreshold(void) const;
			
			/**
			* 감지된 직선의 최대 개수를 설정합니다.
			*
			* param[in] nMaxLineCnt : 직선의 최대 개수
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetMaxLineCount(_In_ int nMaxLineCnt);
			
			/**
			* 설정된 감지된 직선의 최대 개수를 가져옵니다.
			*
			* return value : int 형 감지된 직선의 최대 개수를 반환합니다.
			*/
			int GetMaxLineCount(void) const;
			
			/**
			* 감지된 직선의 지역 범위를 설정합니다.
			*
			* param[in] nRegionalRange : 지역 범위 절대 값
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetRegionRange(_In_ int nRegionalRange);
			
			/**
			* 설정된 감지된 직선의 지역 범위를 가져옵니다.
			*
			* return value : int 형 지역 범위 절대 값을 반환합니다.
			*/
			int GetRegionRange(void) const;

		private:
			CInternalHoughTransformLine* m_pInternal = nullptr;

		};
	}
}

