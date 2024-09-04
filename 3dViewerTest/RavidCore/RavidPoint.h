#pragma once

#include "RavidGeometry.h"

/**
* "점 (Point)" 을 표현하는 클래스입니다.
* 데이터 타입은 int, long long, float, double 만 가능합니다.
* CRavidGeometry 를 상속받은 클래스로 "점"과 관련된 알고리즘을 포함하고 있습니다.
*/
template <typename T>
class AFX_EXT_CLASS CRavidPoint : public CRavidGeometry
{
#include "GeometryCommonObject.hpp"

#include "GeometryVirtualInformation.hpp"
#include "GeometryVirtualOffset.hpp"
#include "GeometryVirtualRect.hpp"
#include "GeometryVirtualCenter.hpp"
#include "GeometryVirtualOperator.hpp"
#include "GeometryVirtualShape.hpp"
	
#include "GeometryVirtualRotate.hpp"

#include "RavidPointCtor.hpp"

public:
	RavidUseDynamicCreation(this);

	static_assert(CheckIntegerAndRealNumber<T>::value, "CRavidPoint template class is able to use type within int, long long, float and double.");

	CRavidPoint();
	virtual ~CRavidPoint();
	
#pragma region [SetExclusiveRegion]
	/**
	* 면적을 가지지 않는 현재 좌표 객체는 사용할 수 없습니다.
	* You must have a surface to use this feature.
	*
	* return value : false 만 반환합니다.
	*/
	virtual bool SetExclusiveRegion(_In_ CRavidGeometryArray* pExclusiveRegion);
	virtual bool SetExclusiveRegion(_In_ CRavidGeometryArray& refExclusiveRegion);
#pragma endregion

#pragma region [AddExclusiveRegion]
	/**
	* 면적을 가지지 않는 현재 좌표 객체는 사용할 수 없습니다.
	* You must have a surface to use this feature.
	*
	* return value : false 만 반환합니다.
	*/
	virtual bool AddExclusiveRegion(_In_ CRavidGeometry* pGeometry);
	virtual bool AddExclusiveRegion(_In_ CRavidGeometry& refGeometry);
#pragma endregion

#pragma region [Assignment operator]
	void operator=(_In_ const POINT& point);
	void operator=(_In_ POINT* pPoint);
	void operator=(_In_ const SIZE& size);
	void operator=(_In_ SIZE* pSize);
	void operator=(_In_ const CRavidPoint<int>& point);
	void operator=(_In_ CRavidPoint<int>* pPoint);
	void operator=(_In_ const CRavidPoint<long long>& point);
	void operator=(_In_ CRavidPoint<long long>* pPoint);
	void operator=(_In_ const CRavidPoint<float>& point);
	void operator=(_In_ CRavidPoint<float>* pPoint);
	void operator=(_In_ const CRavidPoint<double>& point);
	void operator=(_In_ CRavidPoint<double>* pPoint);
#pragma endregion

#pragma region [Comparison operator]
	bool operator==(_In_ const POINT& point);
	bool operator==(_In_ const SIZE& size);
	bool operator==(_In_ const CRavidPoint<int>& point);
	bool operator==(_In_ const CRavidPoint<long long>& point);
	bool operator==(_In_ const CRavidPoint<float>& point);
	bool operator==(_In_ const CRavidPoint<double>& point);

	bool operator!=(_In_ const POINT& point);
	bool operator!=(_In_ const SIZE& size);
	bool operator!=(_In_ const CRavidPoint<int>& point);
	bool operator!=(_In_ const CRavidPoint<long long>& point);
	bool operator!=(_In_ const CRavidPoint<float>& point);
	bool operator!=(_In_ const CRavidPoint<double>& point);
#pragma endregion

#pragma region [Arithmetic operator]
	CRavidPoint<double> operator+(_In_ const int& value);
	CRavidPoint<double> operator+(_In_ const long long& value);
	CRavidPoint<double> operator+(_In_ const float& value);
	CRavidPoint<double> operator+(_In_ const double& value);
	CRavidPoint<double> operator+(_In_ const POINT& point);
	CRavidPoint<double> operator+(_In_ const SIZE& size);
	CRavidPoint<double> operator+(_In_ const CRavidPoint<int>& point);
	CRavidPoint<double> operator+(_In_ const CRavidPoint<long long>& point);
	CRavidPoint<double> operator+(_In_ const CRavidPoint<float>& point);
	CRavidPoint<double> operator+(_In_ const CRavidPoint<double>& point);

	CRavidPoint<double> operator-(_In_ const int& value);
	CRavidPoint<double> operator-(_In_ const long long& value);
	CRavidPoint<double> operator-(_In_ const float& value);
	CRavidPoint<double> operator-(_In_ const double& value);
	CRavidPoint<double> operator-(_In_ const POINT& point);
	CRavidPoint<double> operator-(_In_ const SIZE& size);
	CRavidPoint<double> operator-(_In_ const CRavidPoint<int>& point);
	CRavidPoint<double> operator-(_In_ const CRavidPoint<long long>& point);
	CRavidPoint<double> operator-(_In_ const CRavidPoint<float>& point);
	CRavidPoint<double> operator-(_In_ const CRavidPoint<double>& point);

	CRavidPoint<double> operator*(_In_ const int& value);
	CRavidPoint<double> operator*(_In_ const long long& value);
	CRavidPoint<double> operator*(_In_ const float& value);
	CRavidPoint<double> operator*(_In_ const double& value);
	CRavidPoint<double> operator*(_In_ const POINT& point);
	CRavidPoint<double> operator*(_In_ const SIZE& size);
	CRavidPoint<double> operator*(_In_ const CRavidPoint<int>& point);
	CRavidPoint<double> operator*(_In_ const CRavidPoint<long long>& point);
	CRavidPoint<double> operator*(_In_ const CRavidPoint<float>& point);
	CRavidPoint<double> operator*(_In_ const CRavidPoint<double>& point);

	CRavidPoint<double> operator/(_In_ const int& value);
	CRavidPoint<double> operator/(_In_ const long long& value);
	CRavidPoint<double> operator/(_In_ const float& value);
	CRavidPoint<double> operator/(_In_ const double& value);
	CRavidPoint<double> operator/(_In_ const POINT& point);
	CRavidPoint<double> operator/(_In_ const SIZE& size);
	CRavidPoint<double> operator/(_In_ const CRavidPoint<int>& point);
	CRavidPoint<double> operator/(_In_ const CRavidPoint<long long>& point);
	CRavidPoint<double> operator/(_In_ const CRavidPoint<float>& point);
	CRavidPoint<double> operator/(_In_ const CRavidPoint<double>& point);
#pragma endregion

#pragma region [GetRect]
	/**
	* 현재 좌표를 CRavidRect<T> 형태로 계산 반환합니다.
	*
	* return value : 현재 객체는 점이므로 left == right, top == bottom 의 값으로 반환합니다.
	*/
	virtual CRavidRect<T> GetRect();
#pragma endregion

#pragma region [GetPointStruct]
	/**
	* POINT 구조체를 사용하는 환경을 위해 현재 좌표를 POINT 구조체로 계산 반환합니다.
	*
	* return value : 현재 좌표를 POINT 구조체로 변경 반환합니다.
	*/
	virtual POINT GetPointStruct() override;
#pragma endregion

#pragma region [GetRectStruct]
	/**
	* RECT 구조체를 사용하는 환경을 위해 현재 좌표를 RECT 구조체로 계산 반환합니다.
	*
	* return value : 현재 객체는 점이므로 left == right, top == bottom 의 값으로 반환합니다.
	*/
	virtual RECT GetRectStruct() override;
#pragma endregion
	
#pragma region [GetCenter]
	/**
	* 현재 좌표의 중심을 계산합니다.
	*
	* return value : 현재 객체는 점이므로 현재 좌표를 반환합니다.
	*/
	CRavidPoint<double> GetCenter();
#pragma endregion

#pragma region [GetCenterOfGravity]
	/**
	* 현재 좌표의 무게 중심을 계산합니다.
	*
	* return value : 현재 객체는 점이므로 현재 좌표를 반환합니다.
	*/
	CRavidPoint<double> GetCenterOfGravity();
#pragma endregion

#pragma region [GetPointsDirection]
	
	/**
	* 1번째 상대 위치와 현재 위치를 이용하여 2번째 상대 좌표의 방향성을 계산합니다. (pt1 - this - pt2)
	*
	* param[in] x1 : 1번째 x 좌표
	* param[in] y1 : 1번째 y 좌표
	* param[in] x2 : 2번째 x 좌표
	* param[in] y2 : 2번째 y 좌표
	*
	* return value : 계산 결과는 ERavidPointsDirection 로 반환합니다.
	*                ERavidPointsDirection_Invalid : 계산 실패 혹은 오류 발생
	*                ERavidPointsDirection_CW : 시계 방향
	*                ERavidPointsDirection_CCW : 반시계 방향
	*                ERavidPointsDirection_Straight : 직진성 방향	
	*/
	ERavidPointsDirection GetPointsDirection(_In_ double x1, _In_ double y1, _In_ double x2, _In_ double y2);
	/**
	* 1번째 상대 위치와 현재 위치를 이용하여 2번째 상대 좌표의 방향성을 계산합니다. (pt1 - this - pt2)
	*
	* param[in] refPoint1 : 1번째 x, y 좌표 (Reference)
	* param[in] refPoint2 : 2번째 x, y 좌표 (Reference)
	*
	* return value : 계산 결과는 ERavidPointsDirection 로 반환합니다.
	*                ERavidPointsDirection_Invalid : 계산 실패 혹은 오류 발생
	*                ERavidPointsDirection_CW : 시계 방향
	*                ERavidPointsDirection_CCW : 반시계 방향
	*                ERavidPointsDirection_Straight : 직진성 방향	
	*/
	ERavidPointsDirection GetPointsDirection(_In_ CRavidPoint<int>& refPoint1, _In_ CRavidPoint<int>& refPoint2);
	/**
	* 1번째 상대 위치와 현재 위치를 이용하여 2번째 상대 좌표의 방향성을 계산합니다. (pt1 - this - pt2)
	*
	* param[in] pPoint1 : 1번째 x, y 좌표 (Pointer)
	* param[in] pPoint2 : 2번째 x, y 좌표 (Pointer)
	*
	* return value : 계산 결과는 ERavidPointsDirection 로 반환합니다.
	*                ERavidPointsDirection_Invalid : 계산 실패 혹은 오류 발생
	*                ERavidPointsDirection_CW : 시계 방향
	*                ERavidPointsDirection_CCW : 반시계 방향
	*                ERavidPointsDirection_Straight : 직진성 방향	
	*/
	ERavidPointsDirection GetPointsDirection(_In_ CRavidPoint<int>* pPoint1, _In_ CRavidPoint<int>* pPoint2);
	/**
	* 1번째 상대 위치와 현재 위치를 이용하여 2번째 상대 좌표의 방향성을 계산합니다. (pt1 - this - pt2)
	*
	* param[in] refPoint1 : 1번째 x, y 좌표 (Reference)
	* param[in] refPoint2 : 2번째 x, y 좌표 (Reference)
	*
	* return value : 계산 결과는 ERavidPointsDirection 로 반환합니다.
	*                ERavidPointsDirection_Invalid : 계산 실패 혹은 오류 발생
	*                ERavidPointsDirection_CW : 시계 방향
	*                ERavidPointsDirection_CCW : 반시계 방향
	*                ERavidPointsDirection_Straight : 직진성 방향	
	*/
	ERavidPointsDirection GetPointsDirection(_In_ CRavidPoint<long long>& refPoint1, _In_ CRavidPoint<long long>& refPoint2);
	/**
	* 1번째 상대 위치와 현재 위치를 이용하여 2번째 상대 좌표의 방향성을 계산합니다. (pt1 - this - pt2)
	*
	* param[in] pPoint1 : 1번째 x, y 좌표 (Pointer)
	* param[in] pPoint2 : 2번째 x, y 좌표 (Pointer)
	*
	* return value : 계산 결과는 ERavidPointsDirection 로 반환합니다.
	*                ERavidPointsDirection_Invalid : 계산 실패 혹은 오류 발생
	*                ERavidPointsDirection_CW : 시계 방향
	*                ERavidPointsDirection_CCW : 반시계 방향
	*                ERavidPointsDirection_Straight : 직진성 방향	
	*/
	ERavidPointsDirection GetPointsDirection(_In_ CRavidPoint<long long>* pPoint1, _In_ CRavidPoint<long long>* pPoint2);
	/**
	* 1번째 상대 위치와 현재 위치를 이용하여 2번째 상대 좌표의 방향성을 계산합니다. (pt1 - this - pt2)
	*
	* param[in] refPoint1 : 1번째 x, y 좌표 (Reference)
	* param[in] refPoint2 : 2번째 x, y 좌표 (Reference)
	*
	* return value : 계산 결과는 ERavidPointsDirection 로 반환합니다.
	*                ERavidPointsDirection_Invalid : 계산 실패 혹은 오류 발생
	*                ERavidPointsDirection_CW : 시계 방향
	*                ERavidPointsDirection_CCW : 반시계 방향
	*                ERavidPointsDirection_Straight : 직진성 방향	
	*/
	ERavidPointsDirection GetPointsDirection(_In_ CRavidPoint<float>& refPoint1, _In_ CRavidPoint<float>& refPoint2);
	/**
	* 1번째 상대 위치와 현재 위치를 이용하여 2번째 상대 좌표의 방향성을 계산합니다. (pt1 - this - pt2)
	*
	* param[in] pPoint1 : 1번째 x, y 좌표 (Pointer)
	* param[in] pPoint2 : 2번째 x, y 좌표 (Pointer)
	*
	* return value : 계산 결과는 ERavidPointsDirection 로 반환합니다.
	*                ERavidPointsDirection_Invalid : 계산 실패 혹은 오류 발생
	*                ERavidPointsDirection_CW : 시계 방향
	*                ERavidPointsDirection_CCW : 반시계 방향
	*                ERavidPointsDirection_Straight : 직진성 방향	
	*/
	ERavidPointsDirection GetPointsDirection(_In_ CRavidPoint<float>* pPoint1, _In_ CRavidPoint<float>* pPoint2);
	/**
	* 1번째 상대 위치와 현재 위치를 이용하여 2번째 상대 좌표의 방향성을 계산합니다. (pt1 - this - pt2)
	*
	* param[in] refPoint1 : 1번째 x, y 좌표 (Reference)
	* param[in] refPoint2 : 2번째 x, y 좌표 (Reference)
	*
	* return value : 계산 결과는 ERavidPointsDirection 로 반환합니다.
	*                ERavidPointsDirection_Invalid : 계산 실패 혹은 오류 발생
	*                ERavidPointsDirection_CW : 시계 방향
	*                ERavidPointsDirection_CCW : 반시계 방향
	*                ERavidPointsDirection_Straight : 직진성 방향	
	*/
	ERavidPointsDirection GetPointsDirection(_In_ CRavidPoint<double>& refPoint1, _In_ CRavidPoint<double>& refPoint2);
	/**
	* 1번째 상대 위치와 현재 위치를 이용하여 2번째 상대 좌표의 방향성을 계산합니다. (pt1 - this - pt2)
	*
	* param[in] pPoint1 : 1번째 x, y 좌표 (Pointer)
	* param[in] pPoint2 : 2번째 x, y 좌표 (Pointer)
	*
	* return value : 계산 결과는 ERavidPointsDirection 로 반환합니다.
	*                ERavidPointsDirection_Invalid : 계산 실패 혹은 오류 발생
	*                ERavidPointsDirection_CW : 시계 방향
	*                ERavidPointsDirection_CCW : 반시계 방향
	*                ERavidPointsDirection_Straight : 직진성 방향	
	*/
	ERavidPointsDirection GetPointsDirection(_In_ CRavidPoint<double>* pPoint1, _In_ CRavidPoint<double>* pPoint2);	
	/**
	* 1번째 상대 위치와 현재 위치를 이용하여 2번째 상대 좌표의 방향성을 계산합니다. (pt1 - this - pt2)
	*
	* param[in] refLine : 두 좌표의 직선 또는 선분 (Reference)
	*
	* return value : 계산 결과는 ERavidPointsDirection 로 반환합니다.
	*                ERavidPointsDirection_Invalid : 계산 실패 혹은 오류 발생
	*                ERavidPointsDirection_CW : 시계 방향
	*                ERavidPointsDirection_CCW : 반시계 방향
	*                ERavidPointsDirection_Straight : 직진성 방향	
	*/
	ERavidPointsDirection GetPointsDirection(_In_ CRavidLine<int>& refLine);
	/**
	* 1번째 상대 위치와 현재 위치를 이용하여 2번째 상대 좌표의 방향성을 계산합니다. (pt1 - this - pt2)
	*
	* param[in] pLine : 두 좌표의 직선 또는 선분 (Pointer)
	*
	* return value : 계산 결과는 ERavidPointsDirection 로 반환합니다.
	*                ERavidPointsDirection_Invalid : 계산 실패 혹은 오류 발생
	*                ERavidPointsDirection_CW : 시계 방향
	*                ERavidPointsDirection_CCW : 반시계 방향
	*                ERavidPointsDirection_Straight : 직진성 방향	
	*/
	ERavidPointsDirection GetPointsDirection(_In_ CRavidLine<int>* pLine);
	/**
	* 1번째 상대 위치와 현재 위치를 이용하여 2번째 상대 좌표의 방향성을 계산합니다. (pt1 - this - pt2)
	*
	* param[in] refLine : 두 좌표의 직선 또는 선분 (Reference)
	*
	* return value : 계산 결과는 ERavidPointsDirection 로 반환합니다.
	*                ERavidPointsDirection_Invalid : 계산 실패 혹은 오류 발생
	*                ERavidPointsDirection_CW : 시계 방향
	*                ERavidPointsDirection_CCW : 반시계 방향
	*                ERavidPointsDirection_Straight : 직진성 방향	
	*/
	ERavidPointsDirection GetPointsDirection(_In_ CRavidLine<long long>& refLine);
	/**
	* 1번째 상대 위치와 현재 위치를 이용하여 2번째 상대 좌표의 방향성을 계산합니다. (pt1 - this - pt2)
	*
	* param[in] pLine : 두 좌표의 직선 또는 선분 (Pointer)
	*
	* return value : 계산 결과는 ERavidPointsDirection 로 반환합니다.
	*                ERavidPointsDirection_Invalid : 계산 실패 혹은 오류 발생
	*                ERavidPointsDirection_CW : 시계 방향
	*                ERavidPointsDirection_CCW : 반시계 방향
	*                ERavidPointsDirection_Straight : 직진성 방향	
	*/
	ERavidPointsDirection GetPointsDirection(_In_ CRavidLine<long long>* pLine);
	/**
	* 1번째 상대 위치와 현재 위치를 이용하여 2번째 상대 좌표의 방향성을 계산합니다. (pt1 - this - pt2)
	*
	* param[in] refLine : 두 좌표의 직선 또는 선분 (Reference)
	*
	* return value : 계산 결과는 ERavidPointsDirection 로 반환합니다.
	*                ERavidPointsDirection_Invalid : 계산 실패 혹은 오류 발생
	*                ERavidPointsDirection_CW : 시계 방향
	*                ERavidPointsDirection_CCW : 반시계 방향
	*                ERavidPointsDirection_Straight : 직진성 방향	
	*/
	ERavidPointsDirection GetPointsDirection(_In_ CRavidLine<float>& refLine);
	/**
	* 1번째 상대 위치와 현재 위치를 이용하여 2번째 상대 좌표의 방향성을 계산합니다. (pt1 - this - pt2)
	*
	* param[in] pLine : 두 좌표의 직선 또는 선분 (Pointer)
	*
	* return value : 계산 결과는 ERavidPointsDirection 로 반환합니다.
	*                ERavidPointsDirection_Invalid : 계산 실패 혹은 오류 발생
	*                ERavidPointsDirection_CW : 시계 방향
	*                ERavidPointsDirection_CCW : 반시계 방향
	*                ERavidPointsDirection_Straight : 직진성 방향	
	*/
	ERavidPointsDirection GetPointsDirection(_In_ CRavidLine<float>* pLine);
	/**
	* 1번째 상대 위치와 현재 위치를 이용하여 2번째 상대 좌표의 방향성을 계산합니다. (pt1 - this - pt2)
	* 
	* param[in] refLine : 두 좌표의 직선 또는 선분 (Reference)
	*
	* return value : 계산 결과는 ERavidPointsDirection 로 반환합니다.
	*                ERavidPointsDirection_Invalid : 계산 실패 혹은 오류 발생
	*                ERavidPointsDirection_CW : 시계 방향
	*                ERavidPointsDirection_CCW : 반시계 방향
	*                ERavidPointsDirection_Straight : 직진성 방향	
	*/
	ERavidPointsDirection GetPointsDirection(_In_ CRavidLine<double>& refLine);
	/**
	* 1번째 상대 위치와 현재 위치를 이용하여 2번째 상대 좌표의 방향성을 계산합니다. (pt1 - this - pt2)
	* 
	* param[in] pLine : 두 좌표의 직선 또는 선분 (Pointer)
	*
	* return value : 계산 결과는 ERavidPointsDirection 로 반환합니다.
	*                ERavidPointsDirection_Invalid : 계산 실패 혹은 오류 발생
	*                ERavidPointsDirection_CW : 시계 방향
	*                ERavidPointsDirection_CCW : 반시계 방향
	*                ERavidPointsDirection_Straight : 직진성 방향	
	*/
	ERavidPointsDirection GetPointsDirection(_In_ CRavidLine<double>* pLine);
#pragma endregion

#pragma region [Rotate]
	/**
	* 상대 위치를 기준으로 인자 값 만큼 현재 위치를 회전합니다.
	* 
	* param[in] dblAngle : 회전각도
	* param[in] cx : 중심 x 좌표
	* param[in] cy : 중심 y 좌표	
	*
	* return value : true만 반환합니다.
	*/
	bool Rotate(_In_ double dblAngle, _In_ double cx, _In_ double cy);
#pragma endregion

#pragma region [GetDistance]
	/**
	* 현재 위치를 기준으로 상대 위치와의 거리를 계산합니다.
	* 
	* param[in] x : x 좌표
	* param[in] y : y 좌표
	*
	* return value : 계산 결과는 double로만 반환합니다.
	*/
	double GetDistance(_In_ double x, _In_ double y);
	/**
	* 현재 위치를 기준으로 상대 위치와의 거리를 계산합니다.
	* 
	* param[in] refPoint : 시작 x, y 좌표 (Reference)
	*
	* return value : 계산 결과는 double로만 반환합니다.
	*/
	double GetDistance(_In_ CRavidPoint<int>& refPoint);
	/**
	* 현재 위치를 기준으로 상대 위치와의 거리를 계산합니다.
	* 
	* param[in] pPoint : 시작 x, y 좌표 (Pointer)
	*
	* return value : 계산 결과는 double로만 반환합니다.
	*/
	double GetDistance(_In_ CRavidPoint<int>* pPoint);
	/**
	* 현재 위치를 기준으로 상대 위치와의 거리를 계산합니다.
	* 
	* param[in] refPoint : 시작 x, y 좌표 (Reference)
	*
	* return value : 계산 결과는 double로만 반환합니다.
	*/
	double GetDistance(_In_ CRavidPoint<long long>& refPoint);
	/**
	* 현재 위치를 기준으로 상대 위치와의 거리를 계산합니다.
	* 
	* param[in] pPoint : 시작 x, y 좌표 (Pointer)
	*
	* return value : 계산 결과는 double로만 반환합니다.
	*/
	double GetDistance(_In_ CRavidPoint<long long>* pPoint);
	/**
	* 현재 위치를 기준으로 상대 위치와의 거리를 계산합니다.
	* 
	* param[in] refPoint : 시작 x, y 좌표 (Reference)
	*
	* return value : 계산 결과는 double로만 반환합니다.
	*/
	double GetDistance(_In_ CRavidPoint<float>& refPoint);
	/**
	* 현재 위치를 기준으로 상대 위치와의 거리를 계산합니다.
	* 
	* param[in] pPoint : 시작 x, y 좌표 (Pointer)
	*
	* return value : 계산 결과는 double로만 반환합니다.
	*/
	double GetDistance(_In_ CRavidPoint<float>* pPoint);
	/**
	* 현재 위치를 기준으로 상대 위치와의 거리를 계산합니다.
	* 
	* param[in] refPoint : 시작 x, y 좌표 (Reference)
	*
	* return value : 계산 결과는 double로만 반환합니다.
	*/
	double GetDistance(_In_ CRavidPoint<double>& refPoint);
	/**
	* 현재 위치를 기준으로 상대 위치와의 거리를 계산합니다.
	* 
	* param[in] pPoint : 시작 x, y 좌표 (Pointer)
	*
	* return value : 계산 결과는 double로만 반환합니다.
	*/
	double GetDistance(_In_ CRavidPoint<double>* pPoint);
#pragma endregion

#pragma region [GetOrthogonalPoint]
	/**
	* 현재 위치를 기준으로 상대 객체와의 가장 짧은 직교(수직) 위치를 계산합니다.
	* 
	* param[in] refLine : 선분 또는 직선 (Reference)
	*
	* return value : 계산에 실패하면 무한대 (Inf, Inf) 의 값을 반환합니다.
	*/
	CRavidPoint<double> GetOrthogonalPoint(_In_ CRavidLine<int>& refLine);
	/**
	* 현재 위치를 기준으로 상대 객체와의 가장 짧은 직교(수직) 위치를 계산합니다.
	* 
	* param[in] pLine : 선분 또는 직선 (Pointer)
	*
	* return value : 계산에 실패하면 무한대 (Inf, Inf) 의 값을 반환합니다.
	*/
	CRavidPoint<double> GetOrthogonalPoint(_In_ CRavidLine<int>* pLine);
	/**
	* 현재 위치를 기준으로 상대 객체와의 가장 짧은 직교(수직) 위치를 계산합니다.
	* 
	* param[in] refLine : 선분 또는 직선 (Reference)
	*
	* return value : 계산에 실패하면 무한대 (Inf, Inf) 의 값을 반환합니다.
	*/
	CRavidPoint<double> GetOrthogonalPoint(_In_ CRavidLine<long long>& refLine);
	/**
	* 현재 위치를 기준으로 상대 객체와의 가장 짧은 직교(수직) 위치를 계산합니다.
	* 
	* param[in] pLine : 선분 또는 직선 (Pointer)
	*
	* return value : 계산에 실패하면 무한대 (Inf, Inf) 의 값을 반환합니다.
	*/
	CRavidPoint<double> GetOrthogonalPoint(_In_ CRavidLine<long long>* pLine);
	/**
	* 현재 위치를 기준으로 상대 객체와의 가장 짧은 직교(수직) 위치를 계산합니다.
	* 
	* param[in] refLine : 선분 또는 직선 (Reference)
	*
	* return value : 계산에 실패하면 무한대 (Inf, Inf) 의 값을 반환합니다.
	*/
	CRavidPoint<double> GetOrthogonalPoint(_In_ CRavidLine<float>& refLine);
	/**
	* 현재 위치를 기준으로 상대 객체와의 가장 짧은 직교(수직) 위치를 계산합니다.
	* 
	* param[in] pLine : 선분 또는 직선 (Pointer)
	*
	* return value : 계산에 실패하면 무한대 (Inf, Inf) 의 값을 반환합니다.
	*/
	CRavidPoint<double> GetOrthogonalPoint(_In_ CRavidLine<float>* pLine);
	/**
	* 현재 위치를 기준으로 상대 객체와의 가장 짧은 직교(수직) 위치를 계산합니다.
	* 
	* param[in] refLine : 선분 또는 직선 (Reference)
	*
	* return value : 계산에 실패하면 무한대 (Inf, Inf) 의 값을 반환합니다.
	*/
	CRavidPoint<double> GetOrthogonalPoint(_In_ CRavidLine<double>& refLine);
	/**
	* 현재 위치를 기준으로 상대 객체와의 가장 짧은 직교(수직) 위치를 계산합니다.
	* 
	* param[in] pLine : 선분 또는 직선 (Pointer)
	*
	* return value : 계산에 실패하면 무한대 (Inf, Inf) 의 값을 반환합니다.
	*/
	CRavidPoint<double> GetOrthogonalPoint(_In_ CRavidLine<double>* pLine);
#pragma endregion

#pragma region [GetOrthogonalDistance]
	/**
	* 현재 위치를 기준으로 상대 객체와의 가장 짧은 직교(수직) 거리를 계산합니다.
	* 
	* param[in] refLine : 선분 또는 직선 (Reference)
	*
	* return value : 계산에 실패하면 무한대 (Inf) 의 값을 반환합니다.
	*/
	double GetOrthogonalDistance(_In_ CRavidLine<int>& refLine);
	/**
	* 현재 위치를 기준으로 상대 객체와의 가장 짧은 직교(수직) 거리를 계산합니다.
	* 
	* param[in] pLine : 선분 또는 직선 (Pointer)
	*
	* return value : 계산에 실패하면 무한대 (Inf) 의 값을 반환합니다.
	*/
	double GetOrthogonalDistance(_In_ CRavidLine<int>* pLine);
	/**
	* 현재 위치를 기준으로 상대 객체와의 가장 짧은 직교(수직) 거리를 계산합니다.
	* 
	* param[in] refLine : 선분 또는 직선 (Reference)
	*
	* return value : 계산에 실패하면 무한대 (Inf) 의 값을 반환합니다.
	*/
	double GetOrthogonalDistance(_In_ CRavidLine<long long>& refLine);
	/**
	* 현재 위치를 기준으로 상대 객체와의 가장 짧은 직교(수직) 거리를 계산합니다.
	* 
	* param[in] pLine : 선분 또는 직선 (Pointer)
	*
	* return value : 계산에 실패하면 무한대 (Inf) 의 값을 반환합니다.
	*/
	double GetOrthogonalDistance(_In_ CRavidLine<long long>* pLine);
	/**
	* 현재 위치를 기준으로 상대 객체와의 가장 짧은 직교(수직) 거리를 계산합니다.
	* 
	* param[in] refLine : 선분 또는 직선 (Reference)
	*
	* return value : 계산에 실패하면 무한대 (Inf) 의 값을 반환합니다.
	*/
	double GetOrthogonalDistance(_In_ CRavidLine<float>& refLine);
	/**
	* 현재 위치를 기준으로 상대 객체와의 가장 짧은 직교(수직) 거리를 계산합니다.
	* 
	* param[in] pLine : 선분 또는 직선 (Pointer)
	*
	* return value : 계산에 실패하면 무한대 (Inf) 의 값을 반환합니다.
	*/
	double GetOrthogonalDistance(_In_ CRavidLine<float>* pLine);
	/**
	* 현재 위치를 기준으로 상대 객체와의 가장 짧은 직교(수직) 거리를 계산합니다.
	* 
	* param[in] refLine : 선분 또는 직선 (Reference)
	*
	* return value : 계산에 실패하면 무한대 (Inf) 의 값을 반환합니다.
	*/
	double GetOrthogonalDistance(_In_ CRavidLine<double>& refLine);
	/**
	* 현재 위치를 기준으로 상대 객체와의 가장 짧은 직교(수직) 거리를 계산합니다.
	* 
	* param[in] pLine : 선분 또는 직선 (Pointer)
	*
	* return value : 계산에 실패하면 무한대 (Inf) 의 값을 반환합니다.
	*/
	double GetOrthogonalDistance(_In_ CRavidLine<double>* pLine);
#pragma endregion

#pragma region [GetAngle]
	/**
	* 현재 위치를 기준으로 상대 위치와의 각도를 계산합니다.
	* 
	* param[in] x : x 좌표
	* param[in] y : y 좌표
	*
	* return value : 계산 결과는 Degree 로 반환합니다.
	*/
	double GetAngle(_In_ double x, _In_ double y);
	/**
	* 현재 위치를 기준으로 상대 위치와의 각도를 계산합니다.
	* 
	* param[in] refPoint : 시작 x, y 좌표 (Reference)
	*
	* return value : 계산 결과는 Degree 로 반환합니다.
	*/
	double GetAngle(_In_ CRavidPoint<int>& refPoint);
	/**
	* 현재 위치를 기준으로 상대 위치와의 각도를 계산합니다.
	* 
	* param[in] pPoint : 시작 x, y 좌표 (Pointer)
	*
	* return value : 계산 결과는 Degree 로 반환합니다.
	*/
	double GetAngle(_In_ CRavidPoint<int>* pPoint);
	/**
	* 현재 위치를 기준으로 상대 위치와의 각도를 계산합니다.
	* 
	* param[in] refPoint : 시작 x, y 좌표 (Reference)
	*
	* return value : 계산 결과는 Degree 로 반환합니다.
	*/
	double GetAngle(_In_ CRavidPoint<long long>& refPoint);
	/**
	* 현재 위치를 기준으로 상대 위치와의 각도를 계산합니다.
	* 
	* param[in] pPoint : 시작 x, y 좌표 (Pointer)
	*
	* return value : 계산 결과는 Degree 로 반환합니다.
	*/
	double GetAngle(_In_ CRavidPoint<long long>* pPoint);
	/**
	* 현재 위치를 기준으로 상대 위치와의 각도를 계산합니다.
	* 
	* param[in] refPoint : 시작 x, y 좌표 (Reference)
	*
	* return value : 계산 결과는 Degree 로 반환합니다.
	*/
	double GetAngle(_In_ CRavidPoint<float>& refPoint);
	/**
	* 현재 위치를 기준으로 상대 위치와의 각도를 계산합니다.
	* 
	* param[in] pPoint : 시작 x, y 좌표 (Pointer)
	*
	* return value : 계산 결과는 Degree 로 반환합니다.
	*/
	double GetAngle(_In_ CRavidPoint<float>* pPoint);
	/**
	* 현재 위치를 기준으로 상대 위치와의 각도를 계산합니다.
	* 
	* param[in] refPoint : 시작 x, y 좌표 (Reference)
	*
	* return value : 계산 결과는 Degree 로 반환합니다.
	*/
	double GetAngle(_In_ CRavidPoint<double>& refPoint);
	/**
	* 현재 위치를 기준으로 상대 위치와의 각도를 계산합니다.
	* 
	* param[in] pPoint : 시작 x, y 좌표 (Pointer)
	*
	* return value : 계산 결과는 Degree 로 반환합니다.
	*/
	double GetAngle(_In_ CRavidPoint<double>* pPoint);
#pragma endregion

#pragma region [GetInteriorAngle]
	/**
	* 2개의 상대 위치와 현재 위치의 사이각을 계산합니다. (pt1 - this - pt3)
	* 
	* param[in] x1 : 시작 x 좌표
	* param[in] y1 : 시작 y 좌표
	* param[in] x3 : 마지막 x 좌표
	* param[in] y3 : 마지막 y 좌표
	*
	* return value : 계산 결과는 Degree 로 반환합니다.
	*/
	double GetInteriorAngle(_In_ double x1, _In_ double y1, _In_ double x3, _In_ double y3);
	/**
	* 2개의 상대 위치와 현재 위치의 사이각을 계산합니다. (pt1 - this - pt3)
	* 
	* param[in] refPoint1 : 시작 x, y 좌표 (Reference)
	* param[in] refPoint3 : 마지막 x, y 좌표 (Reference)
	*
	* return value : 계산 결과는 Degree 로 반환합니다.
	*/
	double GetInteriorAngle(_In_ CRavidPoint<int>& refPoint1, _In_ CRavidPoint<int>& refPoint3);
	/**
	* 2개의 상대 위치와 현재 위치의 사이각을 계산합니다. (pt1 - this - pt3)
	* 
	* param[in] pPoint1 : 시작 x, y 좌표 (Pointer)
	* param[in] pPoint3 : 마지막 x, y 좌표 (Pointer)
	*
	* return value : 계산 결과는 Degree 로 반환합니다.
	*/
	double GetInteriorAngle(_In_ CRavidPoint<int>* pPoint1, _In_ CRavidPoint<int>* pPoint3);
	/**
	* 2개의 상대 위치와 현재 위치의 사이각을 계산합니다. (pt1 - this - pt3)
	* 
	* param[in] refPoint1 : 시작 x, y 좌표 (Reference)
	* param[in] refPoint3 : 마지막 x, y 좌표 (Reference)
	*
	* return value : 계산 결과는 Degree 로 반환합니다.
	*/
	double GetInteriorAngle(_In_ CRavidPoint<long long>& refPoint1, _In_ CRavidPoint<long long>& refPoint3);
	/**
	* 2개의 상대 위치와 현재 위치의 사이각을 계산합니다. (pt1 - this - pt3)
	* 
	* param[in] pPoint1 : 시작 x, y 좌표 (Pointer)
	* param[in] pPoint3 : 마지막 x, y 좌표 (Pointer)
	*
	* return value : 계산 결과는 Degree 로 반환합니다.
	*/
	double GetInteriorAngle(_In_ CRavidPoint<long long>* pPoint1, _In_ CRavidPoint<long long>* pPoint3);
	/**
	* 2개의 상대 위치와 현재 위치의 사이각을 계산합니다. (pt1 - this - pt3)
	* 
	* param[in] refPoint1 : 시작 x, y 좌표 (Reference)
	* param[in] refPoint3 : 마지막 x, y 좌표 (Reference)
	*
	* return value : 계산 결과는 Degree 로 반환합니다.
	*/
	double GetInteriorAngle(_In_ CRavidPoint<float>& refPoint1, _In_ CRavidPoint<float>& refPoint3);
	/**
	* 2개의 상대 위치와 현재 위치의 사이각을 계산합니다. (pt1 - this - pt3)
	* 
	* param[in] pPoint1 : 시작 x, y 좌표 (Pointer)
	* param[in] pPoint3 : 마지막 x, y 좌표 (Pointer)
	*
	* return value : 계산 결과는 Degree 로 반환합니다.
	*/
	double GetInteriorAngle(_In_ CRavidPoint<float>* pPoint1, _In_ CRavidPoint<float>* pPoint3);
	/**
	* 2개의 상대 위치와 현재 위치의 사이각을 계산합니다. (pt1 - this - pt3)
	* 
	* param[in] refPoint1 : 시작 x, y 좌표 (Reference)
	* param[in] refPoint3 : 마지막 x, y 좌표 (Reference)
	*
	* return value : 계산 결과는 Degree 로 반환합니다.
	*/
	double GetInteriorAngle(_In_ CRavidPoint<double>& refPoint1, _In_ CRavidPoint<double>& refPoint3);
	/**
	* 2개의 상대 위치와 현재 위치의 사이각을 계산합니다. (pt1 - this - pt3)
	* 
	* param[in] pPoint1 : 시작 x, y 좌표 (Pointer)
	* param[in] pPoint3 : 마지막 x, y 좌표 (Pointer)
	*
	* return value : 계산 결과는 Degree 로 반환합니다.
	*/
	double GetInteriorAngle(_In_ CRavidPoint<double>* pPoint1, _In_ CRavidPoint<double>* pPoint3);
#pragma endregion

#pragma region [GetCircumcenter]
	/**
	* 현재 위치와 2개의 상대 위치를 이용하여 외접원의 중심을 계산합니다.
	* 
	* param[in] x1 : 1번째 x 좌표
	* param[in] y1 : 1번째 y 좌표
	* param[in] x2 : 2번째 x 좌표
	* param[in] y2 : 2번째 y 좌표
	*
	* return value : 계산에 실패하면 무한대 (Inf, Inf) 의 값을 반환합니다.
	*/
	CRavidPoint<double> GetCircumcenter(_In_ double x1, _In_ double y1, _In_ double x2, _In_ double y2);
	/**
	* 현재 위치와 2개의 상대 위치를 이용하여 외접원의 중심을 계산합니다.
	* 
	* param[in] refPoint1 : 1번째 x, y 좌표 (Reference)
	* param[in] refPoint2 : 2번째 x, y 좌표 (Reference)
	*
	* return value : 계산에 실패하면 무한대 (Inf, Inf) 의 값을 반환합니다.
	*/
	CRavidPoint<double> GetCircumcenter(_In_ CRavidPoint<int>& refPoint1, _In_ CRavidPoint<int>& refPoint2);
	/**
	* 현재 위치와 2개의 상대 위치를 이용하여 외접원의 중심을 계산합니다.
	* 
	* param[in] pPoint1 : 1번째 x, y 좌표 (Pointer)
	* param[in] pPoint2 : 2번째 x, y 좌표 (Pointer)
	*
	* return value : 계산에 실패하면 무한대 (Inf, Inf) 의 값을 반환합니다.
	*/
	CRavidPoint<double> GetCircumcenter(_In_ CRavidPoint<int>* pPoint1, _In_ CRavidPoint<int>* pPoint2);
	/**
	* 현재 위치와 2개의 상대 위치를 이용하여 외접원의 중심을 계산합니다.
	* 
	* param[in] refPoint1 : 1번째 x, y 좌표 (Reference)
	* param[in] refPoint2 : 2번째 x, y 좌표 (Reference)
	*
	* return value : 계산에 실패하면 무한대 (Inf, Inf) 의 값을 반환합니다.
	*/
	CRavidPoint<double> GetCircumcenter(_In_ CRavidPoint<long long>& refPoint1, _In_ CRavidPoint<long long>& refPoint2);
	/**
	* 현재 위치와 2개의 상대 위치를 이용하여 외접원의 중심을 계산합니다.
	* 
	* param[in] pPoint1 : 1번째 x, y 좌표 (Pointer)
	* param[in] pPoint2 : 2번째 x, y 좌표 (Pointer)
	*
	* return value : 계산에 실패하면 무한대 (Inf, Inf) 의 값을 반환합니다.
	*/
	CRavidPoint<double> GetCircumcenter(_In_ CRavidPoint<long long>* pPoint1, _In_ CRavidPoint<long long>* pPoint2);
	/**
	* 현재 위치와 2개의 상대 위치를 이용하여 외접원의 중심을 계산합니다.
	* 
	* param[in] refPoint1 : 1번째 x, y 좌표 (Reference)
	* param[in] refPoint2 : 2번째 x, y 좌표 (Reference)
	*
	* return value : 계산에 실패하면 무한대 (Inf, Inf) 의 값을 반환합니다.
	*/
	CRavidPoint<double> GetCircumcenter(_In_ CRavidPoint<float>& refPoint1, _In_ CRavidPoint<float>& refPoint2);
	/**
	* 현재 위치와 2개의 상대 위치를 이용하여 외접원의 중심을 계산합니다.
	* 
	* param[in] pPoint1 : 1번째 x, y 좌표 (Pointer)
	* param[in] pPoint2 : 2번째 x, y 좌표 (Pointer)
	*
	* return value : 계산에 실패하면 무한대 (Inf, Inf) 의 값을 반환합니다.
	*/
	CRavidPoint<double> GetCircumcenter(_In_ CRavidPoint<float>* pPoint1, _In_ CRavidPoint<float>* pPoint2);
	/**
	* 현재 위치와 2개의 상대 위치를 이용하여 외접원의 중심을 계산합니다.
	* 
	* param[in] refPoint1 : 1번째 x, y 좌표 (Reference)
	* param[in] refPoint2 : 2번째 x, y 좌표 (Reference)
	*
	* return value : 계산에 실패하면 무한대 (Inf, Inf) 의 값을 반환합니다.
	*/
	CRavidPoint<double> GetCircumcenter(_In_ CRavidPoint<double>& refPoint1, _In_ CRavidPoint<double>& refPoint2);
	/**
	* 현재 위치와 2개의 상대 위치를 이용하여 외접원의 중심을 계산합니다.
	* 
	* param[in] pPoint1 : 1번째 x, y 좌표 (Pointer)
	* param[in] pPoint2 : 2번째 x, y 좌표 (Pointer)
	*
	* return value : 계산에 실패하면 무한대 (Inf, Inf) 의 값을 반환합니다.
	*/
	CRavidPoint<double> GetCircumcenter(_In_ CRavidPoint<double>* pPoint1, _In_ CRavidPoint<double>* pPoint2);
#pragma endregion

#pragma region [GetUnitVector]
	/**
	* 현재 위치를 기준으로 상대 위치와의 단위 벡터를 계산합니다.
	* 
	* param[in] tx : x 좌표
	* param[in] ty : y 좌표
	*
	* return value : 계산에 실패하면 (0, 0) 의 값을 반환합니다.
	*/
	CRavidPoint<double> GetUnitVector(_In_ double tx, _In_ double ty);
	/**
	* 현재 위치를 기준으로 상대 위치와의 단위 벡터를 계산합니다.
	* 
	* param[in] refPoint : x, y 좌표 (Reference)
	*
	* return value : 계산에 실패하면 (0, 0) 의 값을 반환합니다.
	*/
	CRavidPoint<double> GetUnitVector(_In_ CRavidPoint<int>& refPoint);
	/**
	* 현재 위치를 기준으로 상대 위치와의 단위 벡터를 계산합니다.
	* 
	* param[in] pPoint : x, y 좌표 (Pointer)
	*
	* return value : 계산에 실패하면 (0, 0) 의 값을 반환합니다.
	*/
	CRavidPoint<double> GetUnitVector(_In_ CRavidPoint<int>* pPoint);
	/**
	* 현재 위치를 기준으로 상대 위치와의 단위 벡터를 계산합니다.
	* 
	* param[in] refPoint : x, y 좌표 (Reference)
	*
	* return value : 계산에 실패하면 (0, 0) 의 값을 반환합니다.
	*/
	CRavidPoint<double> GetUnitVector(_In_ CRavidPoint<long long>& refPoint);
	/**
	* 현재 위치를 기준으로 상대 위치와의 단위 벡터를 계산합니다.
	* 
	* param[in] pPoint : x, y 좌표 (Pointer)
	*
	* return value : 계산에 실패하면 (0, 0) 의 값을 반환합니다.
	*/
	CRavidPoint<double> GetUnitVector(_In_ CRavidPoint<long long>* pPoint);
	/**
	* 현재 위치를 기준으로 상대 위치와의 단위 벡터를 계산합니다.
	* 
	* param[in] refPoint : x, y 좌표 (Reference)
	*
	* return value : 계산에 실패하면 (0, 0) 의 값을 반환합니다.
	*/
	CRavidPoint<double> GetUnitVector(_In_ CRavidPoint<float>& refPoint);
	/**
	* 현재 위치를 기준으로 상대 위치와의 단위 벡터를 계산합니다.
	* 
	* param[in] pPoint : x, y 좌표 (Pointer)
	*
	* return value : 계산에 실패하면 (0, 0) 의 값을 반환합니다.
	*/
	CRavidPoint<double> GetUnitVector(_In_ CRavidPoint<float>* pPoint);
	/**
	* 현재 위치를 기준으로 상대 위치와의 단위 벡터를 계산합니다.
	* 
	* param[in] refPoint : x, y 좌표 (Reference)
	*
	* return value : 계산에 실패하면 (0, 0) 의 값을 반환합니다.
	*/
	CRavidPoint<double> GetUnitVector(_In_ CRavidPoint<double>& refPoint);
	/**
	* 현재 위치를 기준으로 상대 위치와의 단위 벡터를 계산합니다.
	* 
	* param[in] pPoint : x, y 좌표 (Pointer)
	*
	* return value : 계산에 실패하면 (0, 0) 의 값을 반환합니다.
	*/
	CRavidPoint<double> GetUnitVector(_In_ CRavidPoint<double>* pPoint);
#pragma endregion

#pragma region [GetNormalVector]
	/**
	* 현재 위치와 상대 위치의 법선(수직) 벡터를 계산합니다.
	*
	* param[in] tx : x 좌표
	* param[in] ty : y 좌표
	*
	* return value : 계산에 실패하면 (0, 0) 의 값을 반환합니다.
	*/
	CRavidPoint<double> GetNormalVector(_In_ double tx, _In_ double ty);
	/**
	* 현재 위치와 상대 위치의 법선(수직) 벡터를 계산합니다.
	*
	* param[in] refPoint : x, y 좌표 (Reference)
	*
	* return value : 계산에 실패하면 (0, 0) 의 값을 반환합니다.
	*/
	CRavidPoint<double> GetNormalVector(_In_ CRavidPoint<int>& refPoint);
	/**
	* 현재 위치와 상대 위치의 법선(수직) 벡터를 계산합니다.
	*
	* param[in] pPoint : x, y 좌표 (Pointer)
	*
	* return value : 계산에 실패하면 (0, 0) 의 값을 반환합니다.
	*/
	CRavidPoint<double> GetNormalVector(_In_ CRavidPoint<int>* pPoint);
	/**
	* 현재 위치와 상대 위치의 법선(수직) 벡터를 계산합니다.
	*
	* param[in] refPoint : x, y 좌표 (Reference)
	*
	* return value : 계산에 실패하면 (0, 0) 의 값을 반환합니다.
	*/
	CRavidPoint<double> GetNormalVector(_In_ CRavidPoint<long long>& refPoint);
	/**
	* 현재 위치와 상대 위치의 법선(수직) 벡터를 계산합니다.
	*
	* param[in] pPoint : x, y 좌표 (Pointer)
	*
	* return value : 계산에 실패하면 (0, 0) 의 값을 반환합니다.
	*/
	CRavidPoint<double> GetNormalVector(_In_ CRavidPoint<long long>* pPoint);
	/**
	* 현재 위치와 상대 위치의 법선(수직) 벡터를 계산합니다.
	*
	* param[in] refPoint : x, y 좌표 (Reference)
	*
	* return value : 계산에 실패하면 (0, 0) 의 값을 반환합니다.
	*/
	CRavidPoint<double> GetNormalVector(_In_ CRavidPoint<float>& refPoint);
	/**
	* 현재 위치와 상대 위치의 법선(수직) 벡터를 계산합니다.
	*
	* param[in] pPoint : x, y 좌표 (Pointer)
	*
	* return value : 계산에 실패하면 (0, 0) 의 값을 반환합니다.
	*/
	CRavidPoint<double> GetNormalVector(_In_ CRavidPoint<float>* pPoint);
	/**
	* 현재 위치와 상대 위치의 법선(수직) 벡터를 계산합니다.
	*
	* param[in] refPoint : x, y 좌표 (Reference)
	*
	* return value : 계산에 실패하면 (0, 0) 의 값을 반환합니다.
	*/
	CRavidPoint<double> GetNormalVector(_In_ CRavidPoint<double>& refPoint);
	/**
	* 현재 위치와 상대 위치의 법선(수직) 벡터를 계산합니다.
	*
	* param[in] pPoint : x, y 좌표 (Pointer)
	*
	* return value : 계산에 실패하면 (0, 0) 의 값을 반환합니다.
	*/
	CRavidPoint<double> GetNormalVector(_In_ CRavidPoint<double>* pPoint);
#pragma endregion

#pragma region [MakeCrossHair]
	/**
	* 현재 위치를 십자가 형태로 변형하여 내보냅니다.
	*
	* param[in_opt] dblLineLength : 수직, 수평 길이
	* param[in_opt] bDiagonal : 십자가의 회전 설정
	*                       true : 45도 회전된 대각선 / false : 0도 수직,수평
	*
	* return value : CRavidGeometryArray 반환 형태를 사용하면 십자가 형태의 객체 array 를 반환합니다.
	*/
	CRavidGeometryArray MakeCrossHair(_In_opt_ double dblLineLength = 1., _In_opt_ bool bDiagonal = false);
	/**
	* 현재 위치를 십자가 형태로 변형하여 내보냅니다.
	*
	* param[out] refGeometryArray : 십자가 형태의 객체 array (Reference)
	* param[in_opt] dblLineLength : 수직, 수평 길이
	* param[in_opt] bDiagonal : 십자가의 회전 설정
	*                       true : 45도 회전된 대각선 / false : 0도 수직,수평
	*
	* return value : bool 반환 형태를 사용하면 성공 여부를 반환합니다.
	*/
	bool MakeCrossHair(_Out_ CRavidGeometryArray& refGeometryArray, _In_opt_ double dblLineLength = 1., _In_opt_ bool bDiagonal = false);
	/**
	* 현재 위치를 십자가 형태로 변형하여 내보냅니다.
	*
	* param[out] pGeometryArray : 십자가 형태의 객체 array (Pointer)
	* param[in_opt] dblLineLength : 수직, 수평 길이
	* param[in_opt] bDiagonal : 십자가의 회전 설정
	*                       true : 45도 회전된 대각선 / false : 0도 수직,수평
	*
	* return value : bool 반환 형태를 사용하면 성공 여부를 반환합니다.
	*/
	bool MakeCrossHair(_Out_ CRavidGeometryArray* pGeometryArray, _In_opt_ double dblLineLength = 1., _In_opt_ bool bDiagonal = false);
#pragma endregion

public:
#pragma region [Data]
	/**
	* 객체를 이루는 x 좌표를 나타냅니다.
	* 템플릿 타입에 따라 타입이 설정됩니다.
	* 
	* T x : 화면 및 이미지를 중심으로 가로방향
	*/
	T x;
	/**
	* 객체를 이루는 y 좌표를 나타냅니다.
	* 템플릿 타입에 따라 타입이 설정됩니다.
	* 
	* T y : 화면 및 이미지를 중심으로 세로방향
	*/
	T y;
#pragma endregion
};