#pragma once

#include <vector>
#include <memory>

class CRavidGeometry;

namespace Ravid
{
	namespace Algorithms
	{
		class CInternalConnectedComponent;

		class CRavidImage;
		
		/**
		* 이미지를 분석하여 연결된 화소의 레이블링 결과를 제공하는 클래스입니다.
		*/
		class AFX_EXT_CLASS CConnectedComponent : public CRavidObject
		{
			RavidUseDynamicCreation();
			RavidPreventCopySelf(CConnectedComponent);

		public:
			/**
			* 화소 연결의 이진화 처리 방법입니다.
			*/
			enum EThresholdType
			{
				EThresholdType_Single = 0,
				EThresholdType_Double,
			};
			/**
			* 컬러 채널간 연산 방법입니다.
			*/
			enum EMultipleChannelCondition
			{
				EMultipleChannelCondition_1 = 0,
				EMultipleChannelCondition_1or2,
				EMultipleChannelCondition_1and2,
				EMultipleChannelCondition_1or2or3,
				EMultipleChannelCondition_1and2and3,
				EMultipleChannelCondition_1or2or3or4,
				EMultipleChannelCondition_1and2and3and4,
			};
			/**
			* 필터 처리 방법입니다.
			*/
			enum EFilterItem
			{
				EFilterItem_Area = 0,
				EFilterItem_Angle,
				EFilterItem_BoundingBoxCenterX,
				EFilterItem_BoundingBoxCenterY,
				EFilterItem_BoundingBoxWidth,
				EFilterItem_BoundingBoxHeight,
				EFilterItem_GravityCenterX,
				EFilterItem_GravityCenterY,
				EFilterItem_MinimumEnclosingRectangleCenterX,
				EFilterItem_MinimumEnclosingRectangleCenterY,
				EFilterItem_MinimumEnclosingRectangleHeight,
				EFilterItem_MinimumEnclosingRectangleWidth,
				EFilterItem_MinimumEnclosingRectangleCircularity,
			};
			/**
			* 결과 정렬 기준입니다.
			*/
			enum ESortItem
			{
				ESortItem_Area = 0,
				ESortItem_Angle,
				ESortItem_BoundingBoxCenterX,
				ESortItem_BoundingBoxCenterY,
				ESortItem_BoundingBoxWidth,
				ESortItem_BoundingBoxHeight,
				ESortItem_GravityCenterX,
				ESortItem_GravityCenterY,
				ESortItem_MinimumEnclosingRectangleCenterX,
				ESortItem_MinimumEnclosingRectangleCenterY,
				ESortItem_MinimumEnclosingRectangleHeight,
				ESortItem_MinimumEnclosingRectangleWidth,
				ESortItem_MinimumEnclosingRectangleCircularity,
			};
			/**
			* 결과 정렬 방법입니다.
			*/
			enum ESortMethod
			{
				ESortMethod_AscendingOrder = 0,
				ESortMethod_DescendingOrder
			};

			CConnectedComponent();
			virtual ~CConnectedComponent();
			
			/**
			* 알고리즘을 실행합니다.
			*
			* param[in] pImgInfo : 입력 이미지 (Pointer)
			* param[in_opt] pGeometry : CRavidRect 검사 ROI (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Encode(_In_ CRavidImage* pImgInfo, _In_opt_ CRavidGeometry* pGeometry = nullptr);
			
			/**
			* 실행 전 사용합니다.
			* 목표 화소를 구분하기 위한 기준값을 설정합니다.
			*
			* param[in] rmv : 화소를 나누기 위한 기준값
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetSingleThreshold(_In_ Framework::CMultipleVariable rmv);

			/**
			* 설정된 목표 화소를 구분하기 위한 기준값을 가져옵니다.
			*
			* param[out] rmv : 화소를 나누기 위한 기준값 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetSingleThreshold(_Out_ Framework::CMultipleVariable &rmv);

			/**
			* 실행 전 사용합니다.
			* 목표 화소를 구분하기 위한 기준값 범위를 설정합니다.
			*
			* param[in] rmvMin : 화소를 나누기 위한 최소 기준값
			* param[in] rmvMax : 화소를 나누기 위한 최대 기준값
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetDoubleThreshold(_In_ Framework::CMultipleVariable rmvMin, _In_ Framework::CMultipleVariable rmvMax);

			/**
			* 설정된 목표 화소를 나누기 위한 기준값 범위를 가져옵니다.
			*
			* param[out] rmvMin : 화소를 나누기 위한 최소 기준값 (Reference)
			* param[out] rmvMax : 화소를 나누기 위한 최대 기준값 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetDoubleThreshold(_Out_ Framework::CMultipleVariable &rmvMin, _Out_ Framework::CMultipleVariable &rmvMax);

			/**
			* 실행 전 사용합니다.
			* 목표 화소를 구분할 때 기준값 기준으로 연산할 방법을 설정합니다.
			*
			* param[in] lc : 연산자 enum
			* ELogicalCondition_Greater			: 초과
			* ELogicalCondition_GreaterEqual	: 이상
			* ELogicalCondition_Equal			: 같다
			* ELogicalCondition_NotEqual		: 다르다
			* ELogicalCondition_LessEqual		: 이하
			* ELogicalCondition_Less			: 미만
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult CConnectedComponent::SetLogicalCondition(_In_ ELogicalCondition lc);
			
			/**
			* 설정된 목표 화소를 구분할 때 기준값 기준으로 연산할 방법을 가져옵니다.
			*
			* param[out] lc : 연산자 enum (Reference)
			* ELogicalCondition_Greater			: 초과
			* ELogicalCondition_GreaterEqual	: 이상
			* ELogicalCondition_Equal			: 같다
			* ELogicalCondition_NotEqual		: 다르다
			* ELogicalCondition_LessEqual		: 이하
			* ELogicalCondition_Less			: 미만
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult CConnectedComponent::GetLogicalCondition(_Out_ ELogicalCondition& lc);
			
			/**
			* 실행 전 사용합니다.
			* 목표 화소를 구분할 때 컬러 채널 간 연산할 방법을 설정합니다.
			*
			* param[in] mcc : 연산자 enum 설정에 땨라 모든 채널이 만족하거나 한곳이라도 만족하면 목표 화소로 인정함
			*	EMultipleChannelCondition_1 : 1채널 사용
			*	EMultipleChannelCondition_1or2 : 1, 2 채널 OR 연산
			*	EMultipleChannelCondition_1and2 : 1, 2 채널 AND 연산
			*	EMultipleChannelCondition_1or2or3 : 1, 2, 3 채널 OR 연산
			*	EMultipleChannelCondition_1and2and3 : 1, 2, 3 채널 AND 연산
			*	EMultipleChannelCondition_1or2or3or4 : 1, 2, 3, 4 채널 OR 연산
			*	EMultipleChannelCondition_1and2and3and4 : 1, 2, 3, 4 채널 AND 연산
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult CConnectedComponent::SetMultipleChannelCondition(_In_ EMultipleChannelCondition mcc);
			
			/**
			* 설정된 목표 화소를 구분할 때 컬러 채널 간 연산할 방법을 가져옵니다.
			*
			* param[out] mcc : 연산자 enum 설정에 땨라 모든 채널이 만족하거나 한곳이라도 만족하면 목표 화소로 인정함 (Reference)
			*	EMultipleChannelCondition_1 : 1채널 사용
			*	EMultipleChannelCondition_1or2 : 1, 2 채널 OR 연산
			*	EMultipleChannelCondition_1and2 : 1, 2 채널 AND 연산
			*	EMultipleChannelCondition_1or2or3 : 1, 2, 3 채널 OR 연산
			*	EMultipleChannelCondition_1and2and3 : 1, 2, 3 채널 AND 연산
			*	EMultipleChannelCondition_1or2or3or4 : 1, 2, 3, 4 채널 OR 연산
			*	EMultipleChannelCondition_1and2and3and4 : 1, 2, 3, 4 채널 AND 연산
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult CConnectedComponent::GetMultipleChannelCondition(_Out_ EMultipleChannelCondition& mcc);
			
			/**
			* 실행 후 사용합니다.
			* 군집된 영역의 가로, 세로 크기를 가져옵니다.
			*
			* param[out] vctBoundarySize : CRavidPoint 의 벡터 형태로 전체 리스트를 가져옵니다. (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetBoundarySize(_Out_ std::vector<CRavidPoint<int> >& vctBoundarySize);
			
			/**
			* 실행 후 사용합니다.
			* 군집된 영역의 가로, 세로 크기를 가져옵니다.
			*
			* param[out] refGeometryArray : CRavidGeometryArray 의 형태로 전체 리스트를 가져옵니다. (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetBoundarySize(_Out_ CRavidGeometryArray& refGeometryArray);
			
			/**
			* 실행 후 사용합니다.
			* 군집된 영역의 가로, 세로 크기를 가져옵니다.
			*
			* param[out] pGeometryArray : CRavidGeometryArray 의 형태로 전체 리스트를 가져옵니다. (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetBoundarySize(_Out_ CRavidGeometryArray* pGeometryArray);
			
			/**
			* 실행 후 사용합니다.
			* 군집된 영역의 가로, 세로 크기를 가져옵니다.
			*
			* return value : CRavidPoint 의 벡터 형태로 전체 리스트를 반환합니다.
			*/
			std::vector<CRavidPoint<int> > GetBoundarySize();
			
			/**
			* 실행 후 사용합니다.
			* 군집된 영역의 범위를 가져옵니다.
			*
			* param[out] vctBoundary : CRavidRect 의 벡터 형태로 전체 리스트를 가져옵니다. (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetBoundary(_Out_ std::vector<CRavidRect<int> >& vctBoundary);
			
			/**
			* 실행 후 사용합니다.
			* 군집된 영역의 범위를 가져옵니다.
			*
			* param[out] refGeometryArray : CRavidGeometryArray 의 형태로 전체 리스트를 가져옵니다. (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetBoundary(_Out_ CRavidGeometryArray& refGeometryArray);
			
			/**
			* 실행 후 사용합니다.
			* 군집된 영역의 범위를 가져옵니다.
			*
			* param[out] pGeometryArray : CRavidGeometryArray 의 형태로 전체 리스트를 가져옵니다. (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetBoundary(_Out_ CRavidGeometryArray* pGeometryArray);
			
			/**
			* 실행 후 사용합니다.
			* 군집된 영역의 범위를 가져옵니다.
			*
			* return value : CRavidRect 의 벡터 형태로 전체 리스트를 반환합니다.
			*/
			std::vector<CRavidRect<int> > GetBoundary();
			
			/**
			* 실행 후 사용합니다.
			* 군집된 영역의 면적을 가져옵니다.
			*
			* param[out] vctBoundary : size_t 의 벡터 형태로 전체 리스트를 가져옵니다. (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetArea(_Out_ std::vector<size_t>& vctArea);
			
			/**
			* 실행 후 사용합니다.
			* 군집된 영역의 면적을 가져옵니다.
			*
			* return value : size_t 의 벡터 형태로 전체 리스트를 반환합니다.
			*/
			std::vector<size_t> GetArea();
			
			/**
			* 실행 후 사용합니다.
			* 군집을 이루는 좌표를 포함하는 볼록한 형태의 좌표를 가져옵니다.
			*
			* param[out] vctConvex : CRavidPoint 의 벡터 형태로 전체 리스트를 군집별 좌표로 가져옵니다. (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetConvexHull(_Out_ std::vector<std::vector<CRavidPoint<int>> >& vctConvex);
			
			/**
			* 실행 후 사용합니다.
			* 군집을 이루는 좌표를 포함하는 볼록한 형태의 좌표를 가져옵니다.
			*
			* param[out] refGeometryArray : CRavidGeometryArray 의 형태로 전체 리스트를 군집별 좌표로 가져옵니다. (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetConvexHull(_Out_ CRavidGeometryArray& refGeometryArray);
			
			/**
			* 실행 후 사용합니다.
			* 군집을 이루는 좌표를 포함하는 볼록한 형태의 좌표를 가져옵니다.
			*
			* param[out] pGeometryArray : CRavidGeometryArray 의 형태로 전체 리스트를 군집별 좌표로 가져옵니다. (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetConvexHull(_Out_ CRavidGeometryArray* pGeometryArray);
			
			/**
			* 실행 후 사용합니다.
			* 군집을 이루는 좌표를 포함하는 볼록한 형태의 좌표를 가져옵니다.
			*
			* return value : CRavidPoint 의 벡터 형태로 전체 리스트를 군집별 좌표로 반환합니다.
			*/
			std::vector<std::vector<CRavidPoint<int>> > GetConvexHull();
			
			/**
			* 실행 후 사용합니다.
			* 군집을 이루는 좌표를 입력된 거리만큼 포함하는 볼록한 형태의 좌표를 가져옵니다.
			*
			* param[in] dblAlpha : 좌표 간 최대 거리값
			* param[out] vctConvex : CRavidPoint 의 벡터 형태로 전체 리스트를 군집별 좌표로 가져옵니다. (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetConcaveHull(_In_ double dblAlpha, _Out_ std::vector<std::vector<CRavidPoint<int>> >& vctConcave);
			
			/**
			* 실행 후 사용합니다.
			* 군집을 이루는 좌표를 입력된 거리만큼 포함하는 볼록한 형태의 좌표를 가져옵니다.
			*
			* param[in] dblAlpha : 좌표 간 최대 거리값
			* param[out] refGeometryArray : CRavidGeometryArray 의 형태로 전체 리스트를 군집별 좌표로 가져옵니다. (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetConcaveHull(_In_ double dblAlpha, _Out_ CRavidGeometryArray& refGeometryArray);
			
			/**
			* 실행 후 사용합니다.
			* 군집을 이루는 좌표를 입력된 거리만큼 포함하는 볼록한 형태의 좌표를 가져옵니다.
			*
			* param[in] dblAlpha : 좌표 간 최대 거리값
			* param[out] pGeometryArray : CRavidGeometryArray 의 형태로 전체 리스트를 군집별 좌표로 가져옵니다. (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetConcaveHull(_In_ double dblAlpha, _Out_ CRavidGeometryArray* pGeometryArray);
			
			/**
			* 실행 후 사용합니다.
			* 군집을 이루는 좌표를 입력된 거리만큼 포함하는 볼록한 형태의 좌표를 가져옵니다.
			*
			* param[in] dblAlpha : 좌표 간 최대 거리값
			*
			* return value : CRavidPoint 의 벡터 형태로 전체 리스트를 군집별 좌표로 반환합니다.
			*/
			std::vector<std::vector<CRavidPoint<int>> > GetConcaveHull(_In_ double dblAlpha);
			
			/**
			* 실행 후 사용합니다.
			* 군집을 이루는 모든 좌표를 가져옵니다.
			*
			* param[out] vctConvex : CRavidPoint 의 벡터 형태로 전체 리스트를 군집별 좌표로 가져옵니다. (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetContour(_Out_ std::vector<std::vector<CRavidPoint<int>> >& vctContour);
			
			/**
			* 실행 후 사용합니다.
			* 군집을 이루는 모든 좌표를 가져옵니다.
			*
			* param[out] refGeometryArray : CRavidGeometryArray 의 형태로 전체 리스트를 군집별 좌표로 가져옵니다. (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetContour(_Out_ CRavidGeometryArray& refGeometryArray);
			
			/**
			* 실행 후 사용합니다.
			* 군집을 이루는 모든 좌표를 가져옵니다.
			*
			* param[out] pGeometryArray : CRavidGeometryArray 의 형태로 전체 리스트를 군집별 좌표로 가져옵니다. (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetContour(_Out_ CRavidGeometryArray* pGeometryArray);
			
			/**
			* 실행 후 사용합니다.
			* 군집을 이루는 모든 좌표를 가져옵니다.
			*
			* return value : CRavidPoint 의 벡터 형태로 전체 리스트를 군집별 좌표로 반환합니다.
			*/
			std::vector<std::vector<CRavidPoint<int>> > GetContour();
			
			/**
			* 실행 후 사용합니다.
			* 군집된 영역의 최소면적 직사각형 범위를 좌표로 가져옵니다.
			*
			* param[out] vctMER : CRavidQuadrangle 의 벡터 형태로 전체 리스트를 가져옵니다. (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetMinimumEnclosingRectangle(_Out_ std::vector<CRavidQuadrangle<double> >& vctMER);
			
			/**
			* 실행 후 사용합니다.
			* 군집된 영역의 최소면적 직사각형 범위를 가져옵니다.(회전 각도 있음)
			*
			* param[out] vctMER : CRavidRect 의 벡터 형태로 전체 리스트를 가져옵니다. (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetMinimumEnclosingRectangle(_Out_ std::vector<CRavidRect<double> >& vctMER);
			
			/**
			* 실행 후 사용합니다.
			* 군집된 영역의 최소면적 직사각형 범위를 좌표로 가져옵니다.
			*
			* param[out] vctMER : CRavidGeometryArray 의 형태로 전체 리스트를 가져옵니다. (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetMinimumEnclosingRectangle(_Out_ CRavidGeometryArray& refGeometryArray);
			
			/**
			* 실행 후 사용합니다.
			* 군집된 영역의 최소면적 직사각형 범위를 좌표로 가져옵니다.
			*
			* param[out] pGeometryArray : CRavidGeometryArray 의 형태로 전체 리스트를 가져옵니다. (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetMinimumEnclosingRectangle(_Out_ CRavidGeometryArray* pGeometryArray);
			
			/**
			* 실행 후 사용합니다.
			* 군집된 영역의 최소면적 직사각형 범위를 좌표로 가져옵니다.
			*
			* return value : CRavidQuadrangle 의 형태로 전체 리스트를 반환합니다.
			*/
			std::vector<CRavidQuadrangle<double> > GetMinimumEnclosingRectangle();
			
			/**
			* 실행 후 사용합니다.
			* 군집된 영역의 중심을 기준으로 픽셀 분포 방향을 가져옵니다.
			*
			* param[out] vctAngles : float 의 벡터 형태로 전체 리스트를 가져옵니다. (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetAngles(_Out_ std::vector<float>& vctAngles);
			
			/**
			* 실행 후 사용합니다.
			* 군집된 영역의 중심을 기준으로 픽셀 분포 방향을 가져옵니다.
			*
			* return value : float 의 벡터 형태로 전체 리스트를 반환합니다.
			*/
			std::vector<float> GetAngles();
			
			/**
			* 실행 후 사용합니다.
			* 군집된 영역의 무게중심을 가져옵니다.
			*
			* param[out] vctGravityPoints : CRavidPoint 의 벡터 형태로 전체 리스트를 가져옵니다. (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetGravityPoint(_Out_ std::vector<CRavidPoint<double> >& vctGravityPoints);
			
			/**
			* 실행 후 사용합니다.
			* 군집된 영역의 무게중심을 가져옵니다.
			*
			* return value : CRavidPoint 의 벡터 형태로 전체 리스트를 반환합니다.
			*/
			std::vector<CRavidPoint<double> > GetGravityPoint();
			
			/**
			* 실행 후 사용합니다.
			* 군집된 영역의 최소면적 장,단축을 가져옵니다.
			*
			* param[out] vctFeretMax : CRavidLine 의 벡터 형태로 전체 리스트를 가져옵니다. (Reference)
			* param[out] vctFeretMin : CRavidLine 의 벡터 형태로 전체 리스트를 가져옵니다. (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetFeret(_Out_ std::vector<CRavidLine<double> >& vctFeretMax, _Out_ std::vector<CRavidLine<double> >& vctFeretMin);
			
			/**
			* 실행 후 사용합니다.
			* 군집된 영역의 최소면적 장,단축을 가져옵니다.
			*
			* param[out] refGeometryArrayMax : CRavidGeometryArray 의 형태로 전체 리스트를 가져옵니다. (Reference)
			* param[out] refGeometryArrayMin : CRavidGeometryArray 의 형태로 전체 리스트를 가져옵니다. (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetFeret(_Out_ CRavidGeometryArray& refGeometryArrayMax, _Out_ CRavidGeometryArray& refGeometryArrayMin);
			
			/**
			* 실행 후 사용합니다.
			* 군집된 영역의 최소면적 장,단축을 가져옵니다.
			*
			* param[out] pGeometryArrayMax : CRavidGeometryArray 의 형태로 전체 리스트를 가져옵니다. (Pointer)
			* param[out] pGeometryArrayMin : CRavidGeometryArray 의 형태로 전체 리스트를 가져옵니다. (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetFeret(_Out_ CRavidGeometryArray* pGeometryArrayMax, _Out_ CRavidGeometryArray* pGeometryArrayMin);
			
			/**
			* 실행 후 사용합니다.
			* 군집된 영역의 진원도를 가져옵니다.
			*
			* param[out] vctCircularity : float 의 벡터 형태로 전체 리스트를 가져옵니다. (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetCircularity(_Out_ std::vector<float>& vctCircularity);
			
			/**
			* 실행 후 사용합니다.
			* 군집된 영역의 진원도를 가져옵니다.
			*
			* param[out] pVctCircularity : float 의 벡터 형태로 전체 리스트를 가져옵니다. (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetCircularity(_Out_ std::vector<float>* pVctCircularity);

			/**
			* 실행 후 사용합니다.
			* 군집된 영역의 진원도를 가져옵니다.
			*
			* return value : float 의 벡터 형태로 전체 리스트를 반환합니다.
			*/
			std::vector<float> GetCircularity();
			
			/**
			* 실행 후 사용합니다.
			* 탐색된 결과에 필터를 적용합니다. 필터를 만족하는 결과만 보관합니다.
			*
			* param[in] eItem : 적용 필터 enum 
			*	EFilterItem_Area : 면적
			*	EFilterItem_Angle : 각도
			*	EFilterItem_BoundingBoxCenterX : 외곽 중심 X
			*	EFilterItem_BoundingBoxCenterY : 외곽 중심 Y
			*	EFilterItem_BoundingBoxWidth : 외곽 가로
			*	EFilterItem_BoundingBoxHeight : 외곽 세로
			*	EFilterItem_GravityCenterX : 무게 중심 X
			*	EFilterItem_GravityCenterY : 무게 중심 Y
			*	EFilterItem_MinimumEnclosingRectangleCenterX : 최소면적 중심 X
			*	EFilterItem_MinimumEnclosingRectangleCenterY : 최소면적 중심 Y
			*	EFilterItem_MinimumEnclosingRectangleHeight : 최소면적 세로
			*	EFilterItem_MinimumEnclosingRectangleWidth : 최소면적 가로
			*	EFilterItem_MinimumEnclosingRectangleCircularity : 최소면적 진원도
			* param[in] dblValue : 기준 값
			* param[in] eCondition : 연산자 enum
			*	ELogicalCondition_Greater		: 초과
			*	ELogicalCondition_GreaterEqual	: 이상
			*	ELogicalCondition_Equal			: 같다
			*	ELogicalCondition_NotEqual		: 다르다
			*	ELogicalCondition_LessEqual		: 이하
			*	ELogicalCondition_Less			: 미만
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Filter(_In_ EFilterItem eItem, _In_ double dblValue, _In_ ELogicalCondition eCondition);
			
			/**
			* 실행 후 사용합니다.
			* 탐색된 결과를 입력된 정렬 방식으로 정렬합니다.
			*
			* param[in] eItem : 적용 정렬 데이터 enum 
			*	ESortItem_Area : 면적
			*	ESortItem_Angle : 각도
			*	ESortItem_BoundingBoxCenterX : 외곽 중심 X
			*	ESortItem_BoundingBoxCenterY : 외곽 중심 Y
			*	ESortItem_BoundingBoxWidth : 외곽 가로
			*	ESortItem_BoundingBoxHeight : 외곽 세로
			*	ESortItem_GravityCenterX : 무게 중심 X
			*	ESortItem_GravityCenterY : 무게 중심 Y
			*	ESortItem_MinimumEnclosingRectangleCenterX : 최소면적 중심 X
			*	ESortItem_MinimumEnclosingRectangleCenterY : 최소면적 중심 Y
			*	ESortItem_MinimumEnclosingRectangleHeight : 최소면적 세로
			*	ESortItem_MinimumEnclosingRectangleWidth : 최소면적 가로
			*	ESortItem_MinimumEnclosingRectangleCircularity : 최소면적 진원도
			* param[in] eMethod : 정렬 방법 enum
			*	ESortMethod_AscendingOrder : 오름차순
			*	ESortMethod_DescendingOrder : 내림차순
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Sort(_In_ ESortItem eItem, _In_ ESortMethod eMethod);
			
			/**
			* 실행 후 사용합니다.
			* 입력된 인덱스의 가로, 세로 크기를 가져옵니다.
			*
			* return value : CRavidPoint 형태로 반환합니다.
			*/
			CRavidPoint<int> GetBoundarySizeByIndex(int nIndex);
			
			/**
			* 실행 후 사용합니다.
			* 입력된 인덱스의 가로, 세로 크기를 가져옵니다.
			*
			* return value : CRavidRect 형태로 반환합니다.
			*/
			CRavidRect<int> GetBoundaryByIndex(int nIndex);
			
			/**
			* 실행 후 사용합니다.
			* 입력된 인덱스의 군집을 이루는 모든 좌표를 가져옵니다.
			*
			* return value : CRavidPoint 벡터 형태로 반환합니다.
			*/
			std::vector<CRavidPoint<int>> GetContourByIndex(int nIndex);
			
			/**
			* 실행 후 사용합니다.
			* 입력된 인덱스의 군집을 이루는 좌표들을 포함하는 볼록한 형태의 좌표를 가져옵니다.
			*
			* return value : CRavidPoint 벡터 형태로 반환합니다.
			*/
			std::vector<CRavidPoint<int>> GetConvexHullByIndex(int nIndex);
			
			/**
			* 실행 후 사용합니다.
			* 입력된 인덱스의 최소면적 직사각형 범위를 가져옵니다.
			*
			* return value : CRavidRect 형태로 반환합니다.
			*/
			CRavidRect<double> GetMinimumEnclosingRectangleByIndex(int nIndex);
			
			/**
			* 실행 후 사용합니다.
			* 군집된 영역 범위의 내부 메모리를 가져옵니다.
			*
			* return value : vector<RECT>* 형태로 반환합니다.
			*/
			std::vector<RECT>* GetBoundaryPtr();
			
			/**
			* 실행 후 사용합니다.
			* 군집된 영역 면적의 내부 메모리를 가져옵니다.
			*
			* return value : vector<size_t>* 형태로 반환합니다.
			*/
			std::vector<size_t>* GetAreaPtr();
			
			/**
			* 실행 후 사용합니다.
			* 군집된 영역 각도의 내부 메모리를 가져옵니다.
			*
			* return value : vector<float>* 형태로 반환합니다.
			*/
			std::vector<float>* GetAnglesPtr();
			
			/**
			* 실행 후 사용합니다.
			* 군집된 영역 무게중심의 내부 메모리를 가져옵니다.
			*
			* return value : vector<CRavidPoint<double>>* 형태로 반환합니다.
			*/
			std::vector<CRavidPoint<double>>* GetGravityPointPtr();
			
			/**
			* 실행 후 사용합니다.
			* 군집된 영역 좌표리스트의 내부 메모리를 가져옵니다.
			*
			* return value : std::vector<std::vector<CRavidPoint<int>>>* 형태로 반환합니다.
			*/
			std::vector<std::vector<CRavidPoint<int>>>* GetContourPtr();
			
			/**
			* 실행 후 사용합니다.
			* 군집된 영역 볼록한 형태의 좌표의 내부 메모리를 가져옵니다.
			*
			* return value : std::vector<std::vector<CRavidPoint<int>>>* 형태로 반환합니다.
			*/
			std::vector<std::vector<CRavidPoint<int>>>* GetConvexHullPtr();
			
			/**
			* 실행 후 사용합니다.
			* 군집된 영역 최소면적 직사각형의 내부 메모리를 가져옵니다.
			*
			* return value : vector<CRavidPoint<double>>* 형태로 반환합니다.
			*/
			std::vector<CRavidQuadrangle<double>>* GetMinimumEnclosingRectanglePtr();
			
			/**
			* 실행 후 사용합니다.
			* 군집된 영역 진원도의 내부 메모리를 가져옵니다.
			*
			* return value : vector<float>* 형태로 반환합니다.
			*/
			std::vector<float>* GetCircularityPtr();
			
			/**
			* 실행 후 사용합니다.
			* 군집된 영역 최소면적 장축을의 내부 메모리를 가져옵니다.
			*
			* return value : vector<float>* 형태로 반환합니다.
			*/
			std::vector<CRavidLine<double>>* GetFeretMax();
			
			/**
			* 실행 후 사용합니다.
			* 군집된 영역 최소면적 단축을의 내부 메모리를 가져옵니다.
			*
			* return value : vector<float>* 형태로 반환합니다.
			*/
			std::vector<CRavidLine<double>>* GetFeretMin();
			
			/**
			* 실행 후 사용합니다.
			* 군집된 영역 거리만큼 포함하는 볼록한 형태의 좌표의 내부 메모리를 가져옵니다.
			*
			* param[in] dblAlpha : 좌표 간 최대 거리값
			*
			* return value : std::vector<std::vector<CRavidPoint<int>>>* 형태로 반환합니다.
			*/
			std::vector<std::vector<CRavidPoint<int>> >* GetConcaveHullPtr(double dblAlpha);
			
		private:
			std::unique_ptr<CInternalConnectedComponent> m_pInternal;

		};
	}
}
