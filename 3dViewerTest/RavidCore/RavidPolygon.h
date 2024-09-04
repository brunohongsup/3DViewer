#pragma once

#include "RavidGeometryArray.h"

#include <vector>

/**
* "다각형 (Polygon)" 을 표현하는 클래스입니다.
* 여러 객체를 이용해서 하나의 객체로 표현됩니다. 곡선과 직선을 혼합 포함 가능합니다.
* CRavidGeometryArray 를 상속받은 클래스로 "다각형"과 관련된 알고리즘을 포함하고 있습니다.
*/
class AFX_EXT_CLASS CRavidPolygon : public CRavidGeometryArray
{
#include "GeometryCommonObject.hpp"
#include "GeometryCommonShape.hpp"

#include "GeometryVirtualInformation.hpp"
#include "GeometryVirtualOffset.hpp"
#include "GeometryVirtualRect.hpp"
#include "GeometryVirtualCenter.hpp"
#include "GeometryVirtualDeflate.hpp"
#include "GeometryVirtualInflate.hpp"
#include "GeometryVirtualOperator.hpp"
#include "GeometryVirtualRotate.hpp"
#include "GeometryVirtualShape.hpp"

#include "RavidPolygonCtor.hpp"

public:
	RavidUseDynamicCreation(this);


	CRavidPolygon();
	virtual ~CRavidPolygon();

	enum EObjectType
	{
		EObjectType_Polygon = 0,
		EObjectType_ClosedPolyline,
		EObjectType_OpenPolyline,
	};

#pragma region [Polygon Object Type]
	/**
	* 현재 폴리곤의 타입을 설정합니다.
	*
	* param[in] eType : 폴리곤 형태
	* EObjectType_Polygon : 기본 폴리곤 형태
	* EObjectType_ClosedPolyline : 닫힌 폴리라인 형태
	* EObjectType_OpenPolyline : 열린 폴리라인 형태
	*/
	void SetObjectType(_In_ EObjectType eType);
	/**
	* 현재 설정된 폴리곤의 타입을 가져옵니다.
	*
	* return value : EObjectType 의 정의된 상태를 반환합니다.
	* EObjectType_Polygon : 기본 폴리곤 형태
	* EObjectType_ClosedPolyline : 닫힌 폴리라인 형태
	* EObjectType_OpenPolyline : 열린 폴리라인 형태
	*/
	EObjectType GetObjectType();
#pragma endregion

#pragma region [GetCircumference]
	/**
	* 현재 폴리곤의 둘레를 계산합니다.
	*
	* return value : 객체의 길이, 둘레, 객체간 거리 등 모든 정보를 포함하여 계산합니다.
	*/
	virtual double GetCircumference();
#pragma endregion

#pragma region [Assignment operator]
	CRavidPolygon& operator=(_In_ const CRavidPolygon& poly);
	CRavidPolygon& operator=(_In_ CRavidPolygon* pPoly);
#pragma endregion

#pragma region [Arithmetic operator]
	CRavidPolygon operator+(_In_ const POINT& point);
	CRavidPolygon operator+(_In_ const SIZE& size);
	CRavidPolygon operator+(_In_ const CRavidPoint<int>& point);
	CRavidPolygon operator+(_In_ const CRavidPoint<long long>& point);
	CRavidPolygon operator+(_In_ const CRavidPoint<float>& point);
	CRavidPolygon operator+(_In_ const CRavidPoint<double>& point);

	CRavidPolygon operator-(_In_ const POINT& point);
	CRavidPolygon operator-(_In_ const SIZE& size);
	CRavidPolygon operator-(_In_ const CRavidPoint<int>& point);
	CRavidPolygon operator-(_In_ const CRavidPoint<long long>& point);
	CRavidPolygon operator-(_In_ const CRavidPoint<float>& point);
	CRavidPolygon operator-(_In_ const CRavidPoint<double>& point);
#pragma endregion
	
#pragma region [Add]
	/**
	* 현재 객체에 입력된 객체를 추가합니다.
	* 입력된 순서가 객체의 형태를 나타냅니다.
	*
	* param[in] refGeometry : 추가 객체 (Reference)
	*
	* return value : 추가에 실패하면 false 를 반환합니다.
	*/
	virtual bool Add(_In_ CRavidGeometry& refGeometry);
	/**
	* 현재 객체에 입력된 객체를 추가합니다.
	* 입력된 순서가 객체의 형태를 나타냅니다.
	*
	* param[in] pGeometry : 추가 객체 (Pointer)
	*
	* return value : 추가에 실패하면 false 를 반환합니다.
	*/
	virtual bool Add(_In_ CRavidGeometry* pGeometry);
#pragma endregion
	
#pragma region [GetRect]
	/**
	* 현재 객체를 CRavidRect<double> 형태로 계산 반환합니다.
	* 무조건 double의 형태로 반환됩니다.
	*
	* return value : 현재 객체의 최소, 최대 x ,y 값으로 반환합니다.
	*/
	CRavidRect<double> GetRect();
#pragma endregion
	
#pragma region [GetPointStruct]
	/**
	* POINT 구조체를 사용하는 환경을 위해 현재 좌표를 POINT 구조체로 계산 반환합니다.
	*
	* return value : 현재 객체의 중심 좌표를 POINT 구조체로 변경 반환합니다.
	*/
	virtual POINT GetPointStruct() override;
#pragma endregion

#pragma region [GetRectStruct]
	/**
	* RECT 구조체를 사용하는 환경을 위해 현재 좌표를 RECT 구조체로 계산 반환합니다.
	*
	* return value : 현재 객체의 최소, 최대 x ,y 값을 RECT 구조체로 반환합니다.
	*/
	virtual RECT GetRectStruct() override;
#pragma endregion
	
#pragma region [GetCenter]
	/**
	* 현재 객체의 중심을 계산합니다.
	*
	* return value : 중심 좌표를 반환합니다.
	*/
	CRavidPoint<double> GetCenter();
#pragma endregion

#pragma region [GetCenterOfGravity]
	/**
	* 현재 객체의 무게 중심을 계산합니다.
	*
	* return value : 무게 중심 좌표를 반환합니다.
	*/
	CRavidPoint<double> GetCenterOfGravity();
#pragma endregion
	
#pragma region [GetArea]
	/**
	* 현재 객체의 면적을 계산합니다.
	*
	* return value : 계산에 오류가 있으면 INF 를 반환합니다.
	*/
	virtual double GetArea() override;
#pragma endregion
	
#pragma region [GetAngle]
	/**
	* 각도를 출력합니다. 현재 객체는 회전 각을 알 수 없습니다.
	*
	* return value : 0 만 반환합니다.
	*/
	virtual double GetAngle() override;
#pragma endregion
	
#pragma region [GetIntersection]
	/**
	* 현재 객체와 상대 직선과의 교점을 계산합니다.
	* 선분인 경우에는 범위 내만 계산합니다.
	*
	* param[in] refGeometry : 상대 객체 (Reference)
	* param[out] pGeometrayArray : 교점 좌표 리스트 (Pointer)
	*
	* return value : 교점이 없으면 false 로 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidLine<int>& refGeometry, _Out_ CRavidGeometryArray* pRga);
	/**
	* 현재 객체와 상대 직선과의 교점을 계산합니다.
	* 선분인 경우에는 범위 내만 계산합니다.
	*
	* param[in] pGometry : 상대 객체 (Pointer)
	* param[out] pGeometrayArray : 교점 좌표 리스트 (Pointer)
	*
	* return value : 교점이 없으면 false 로 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidLine<int>* pGometry, _Out_ CRavidGeometryArray* pRga);
	/**
	* 현재 객체와 상대 직선과의 교점을 계산합니다.
	* 선분인 경우에는 범위 내만 계산합니다.
	*
	* param[in] refGeometry : 상대 객체 (Reference)
	* param[out] pGeometrayArray : 교점 좌표 리스트 (Pointer)
	*
	* return value : 교점이 없으면 false 로 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidLine<long long>& refGeometry, _Out_ CRavidGeometryArray* pRga);
	/**
	* 현재 객체와 상대 직선과의 교점을 계산합니다.
	* 선분인 경우에는 범위 내만 계산합니다.
	*
	* param[in] pGometry : 상대 객체 (Pointer)
	* param[out] pGeometrayArray : 교점 좌표 리스트 (Pointer)
	*
	* return value : 교점이 없으면 false 로 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidLine<long long>* pGometry, _Out_ CRavidGeometryArray* pRga);
	/**
	* 현재 객체와 상대 직선과의 교점을 계산합니다.
	* 선분인 경우에는 범위 내만 계산합니다.
	*
	* param[in] refGeometry : 상대 객체 (Reference)
	* param[out] pGeometrayArray : 교점 좌표 리스트 (Pointer)
	*
	* return value : 교점이 없으면 false 로 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidLine<float>& refGeometry, _Out_ CRavidGeometryArray* pRga);
	/**
	* 현재 객체와 상대 직선과의 교점을 계산합니다.
	* 선분인 경우에는 범위 내만 계산합니다.
	*
	* param[in] pGometry : 상대 객체 (Pointer)
	* param[out] pGeometrayArray : 교점 좌표 리스트 (Pointer)
	*
	* return value : 교점이 없으면 false 로 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidLine<float>* pGometry, _Out_ CRavidGeometryArray* pRga);
	/**
	* 현재 객체와 상대 직선과의 교점을 계산합니다.
	* 선분인 경우에는 범위 내만 계산합니다.
	*
	* param[in] refGeometry : 상대 객체 (Reference)
	* param[out] pGeometrayArray : 교점 좌표 리스트 (Pointer)
	*
	* return value : 교점이 없으면 false 로 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidLine<double>& refGeometry, _Out_ CRavidGeometryArray* pRga);
	/**
	* 현재 객체와 상대 직선과의 교점을 계산합니다.
	* 선분인 경우에는 범위 내만 계산합니다.
	*
	* param[in] pGometry : 상대 객체 (Pointer)
	* param[out] pGeometrayArray : 교점 좌표 리스트 (Pointer)
	*
	* return value : 교점이 없으면 false 로 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidLine<double>* pGometry, _Out_ CRavidGeometryArray* pRga);
#pragma endregion

#pragma region [MakeFullSegmentPolygon]
	/**
	* 현재 객체를 모두 연결된 상태로 변환하는 계산을 합니다.
	* 메모리를 동적 생성하였기 때문에 사용자는 필수 해제해야 합니다.
	* CRavidPolygon 으로 변환하되어 반환됩니다.
	*
	* return value : 동적 할단된 메모리를 반환합니다.
	*/
	CRavidPolygon* MakeFullSegmentPolygon();
#pragma endregion

#pragma region [GetIntersectionAboutRegion]
	/**
	* 현재 객체와 상대 객체의 영역 교차에 대한 좌표만 반환합니다.
	* 속도 개선 테스트 버전입니다. 정식 배포전까지 사용은 하지 말아주세요.
	*
	* param[in] refGeometry : 상대 객체 (Pointer)
	* param[out] pGeometrayArray : 교점 좌표 리스트 (Pointer)
	*
	* return value : 교점이 없으면 false 로 반환합니다.
	*/
	bool GetIntersectionAboutRegion(_In_ CRavidPolygon* pGeometry, _Out_ CRavidGeometryArray* pGeometrayArray);
#pragma endregion

#pragma region [CurveCorrect]
	/**
	* 현재 객체의 꼬인 상태를 풀어주는 함수입니다.
	* 테스트 버전입니다. 정식 배포전까지 사용은 하지 말아주세요.
	* 메모리를 동적 생성하였기 때문에 사용자는 필수 해제해야 합니다.
	*
	* return value : 동적 할당된 풀린 폴리곤 메모리를 반환합니다.
	*/
	CRavidPolygon* CurveCorrect();
#pragma endregion

#pragma region [GetDirection]
	/**
	* 현재 객체의 회전 방향을 계산합니다.
	*
	* return value : true : cw / false : ccw 를 반환합니다.
	*/
	bool GetDirection();
#pragma endregion

#pragma region [GetConvexHull]
	/**
	* 현재 객체에서 컨벡스헐을 계산합니다.
	*
	* param[in] pvctPoints : CRavidPoint<double> 의 vector 리스트 (Pointer)
	*
	* return value : 계산에 실패하면 false 로 반환합니다.
	*/
	bool GetConvexHull(_Out_ std::vector<CRavidPoint<double>>* pvctPoints);
	/**
	* 현재 객체에서 컨벡스헐을 계산합니다.
	*
	* param[in] pGeometrayArray : CRavidPoint<double> 의 CRavidGeometryArray 클래스 (Pointer)
	*
	* return value : 계산에 실패하면 false 로 반환합니다.
	*/
	bool GetConvexHull(_Out_ CRavidGeometryArray* pGeometrayArray);
#pragma endregion

#pragma region [GetMinimumEnclosingRectangle]
	/**
	* 현재 객체에서 최소면적 직사각형을 계산합니다.
	*
	* param[in] pGeometry : CRavidRect<double> 의 정보 (Pointer)
	*
	* return value : 계산에 실패하면 false 로 반환합니다.
	*/
	bool GetMinimumEnclosingRectangle(_Out_ CRavidRect<double>* pGeometry);
	/**
	* 현재 객체에서 최소면적 직사각형을 계산합니다.
	*
	* param[in] pGeometry : CRavidQuadrangle<double> 의 좌표 정보 (Pointer)
	*
	* return value : 계산에 실패하면 false 로 반환합니다.
	*/
	bool GetMinimumEnclosingRectangle(_Out_ CRavidQuadrangle<double>* pGeometry);
#pragma endregion

private:

#pragma region [private]
	/**
	* 클래스 내부 계산 전용 함수 목록입니다.
	* 사용자에게는 내부 알고리즘의 결과만 확인 가능합니다.
	*/
	struct RegionPoint
	{
		double x;
		double y;

		RegionPoint(_In_opt_ double x = 0, _In_opt_ double y = 0);
		RegionPoint(_In_ const CRavidPoint<double>& rp);

		bool operator==(_In_ const RegionPoint& ip) const;
		bool operator!=(_In_ const RegionPoint& ip) const;
	};

	// Main Region Function
	bool GetIntersectionPointAboutRegion(_In_ CRavidPolygon* pRpg, _Out_ std::vector<RegionPoint>& vctIP, _Out_ std::vector<std::vector<RegionPoint>>& vctIntersectionAboutRpg1, _Out_ std::vector<std::vector<RegionPoint>>& vctIntersectionAboutRpg2);
	bool GetUnionPointAboutRegion(_In_ CRavidPolygon* pRpg, _Out_ std::vector<RegionPoint>& vctSurfaceIntersection, _Out_ std::vector<RegionPoint>& vctInternalIntersection, _Out_ std::vector<std::vector<RegionPoint>>& vctUnionAboutRpg1, _Out_ std::vector<std::vector<RegionPoint>>& vctUnionAboutRpg2);
	bool GetSubtractionPointAboutRegion(_In_ CRavidPolygon* pRpg, _Out_ std::vector<RegionPoint>& vctSurfaceIntersection, _Out_ std::vector<RegionPoint>& vctInternalIntersection, _Out_ std::vector<std::vector<RegionPoint>>& vctSubtractionAboutRpg1, _Out_ std::vector<std::vector<RegionPoint>>& vctSubtractionAboutRpg2);

	EObjectType m_eObjectType = EObjectType_Polygon;

	bool GetIntersectionForRasterRegion(_In_ CRavidLine<double>* pLine, _Out_ CRavidGeometryArray* pRga);
	
	bool InternalComp(std::vector<RegionPoint>& vctPoints, RegionPoint &A, RegionPoint &B);
	void InternalSortByAngle(std::vector<RegionPoint>& vctPoints, int first, int last);
#pragma endregion
};

