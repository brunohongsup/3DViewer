#pragma once

#include "RavidPoint.h"
#include "RavidGeometry.h"

/**
* "직선 또는 선분 (Line)" 을 표현하는 클래스입니다.
* 데이터 타입은 int, long long, float, double 만 가능합니다.
* CRavidGeometry 를 상속받은 클래스로 "선"과 관련된 Geometry 알고리즘을 포함하고 있습니다.
*/
template <typename T>
class AFX_EXT_CLASS CRavidLine : public CRavidGeometry
{
#include "GeometryCommonObject.hpp"

#include "GeometryVirtualInformation.hpp"
#include "GeometryVirtualOffset.hpp"
#include "GeometryVirtualRect.hpp"
#include "GeometryVirtualCenter.hpp"
#include "GeometryVirtualDeflate.hpp"
#include "GeometryVirtualInflate.hpp"
#include "GeometryVirtualOperator.hpp"
#include "GeometryVirtualRotate.hpp"
#include "GeometryVirtualShape.hpp"

#include "RavidLineCtor.hpp"

public:
	RavidUseDynamicCreation(this);

	static_assert(CheckIntegerAndRealNumber<T>::value, "CRavidLine template class is able to use type within int, long long, float and double.");

	CRavidLine();
	virtual ~CRavidLine();
	
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
	void operator=(_In_ const CRavidLine<int>& line);
	void operator=(_In_ CRavidLine<int>* pLine);
	void operator=(_In_ const CRavidLine<long long>& line);
	void operator=(_In_ CRavidLine<long long>* pLine);
	void operator=(_In_ const CRavidLine<float>& line);
	void operator=(_In_ CRavidLine<float>* pLine);
	void operator=(_In_ const CRavidLine<double>& line);
	void operator=(_In_ CRavidLine<double>* pLine);
	void operator=(_In_ const RECT& rect);
	void operator=(_In_ RECT* pRect);
	void operator=(_In_ const CRavidRect<int>& rect);
	void operator=(_In_ CRavidRect<int>* pRect);
	void operator=(_In_ const CRavidRect<long long>& rect);
	void operator=(_In_ CRavidRect<long long>* pRect);
	void operator=(_In_ const CRavidRect<float>& rect);
	void operator=(_In_ CRavidRect<float>* pRect);
	void operator=(_In_ const CRavidRect<double>& rect);
	void operator=(_In_ CRavidRect<double>* pRect);
#pragma endregion

#pragma region [Comparison operator]
	bool operator==(_In_ const CRavidLine<int>& line);
	bool operator==(_In_ const CRavidLine<long long>& line);
	bool operator==(_In_ const CRavidLine<float>& line);
	bool operator==(_In_ const CRavidLine<double>& line);
	bool operator==(_In_ const CRavidRect<int>& rect);
	bool operator==(_In_ const CRavidRect<long long>& rect);
	bool operator==(_In_ const CRavidRect<float>& rect);
	bool operator==(_In_ const CRavidRect<double>& rect);

	bool operator!=(_In_ const CRavidLine<int>& line);
	bool operator!=(_In_ const CRavidLine<long long>& line);
	bool operator!=(_In_ const CRavidLine<float>& line);
	bool operator!=(_In_ const CRavidLine<double>& line);
	bool operator!=(_In_ const CRavidRect<int>& rect);
	bool operator!=(_In_ const CRavidRect<long long>& rect);
	bool operator!=(_In_ const CRavidRect<float>& rect);
	bool operator!=(_In_ const CRavidRect<double>& rect);
#pragma endregion

#pragma region [Arithmetic and assignment operator]
	void operator+=(_In_ const RECT& rect);
	void operator+=(_In_ const CRavidLine<int>& line);
	void operator+=(_In_ const CRavidLine<long long>& line);
	void operator+=(_In_ const CRavidLine<float>& line);
	void operator+=(_In_ const CRavidLine<double>& line);
	void operator+=(_In_ const CRavidRect<int>& rect);
	void operator+=(_In_ const CRavidRect<long long>& rect);
	void operator+=(_In_ const CRavidRect<float>& rect);
	void operator+=(_In_ const CRavidRect<double>& rect);

	void operator-=(_In_ const RECT& rect);
	void operator-=(_In_ const CRavidLine<int>& line);
	void operator-=(_In_ const CRavidLine<long long>& line);
	void operator-=(_In_ const CRavidLine<float>& line);
	void operator-=(_In_ const CRavidLine<double>& line);
	void operator-=(_In_ const CRavidRect<int>& rect);
	void operator-=(_In_ const CRavidRect<long long>& rect);
	void operator-=(_In_ const CRavidRect<float>& rect);
	void operator-=(_In_ const CRavidRect<double>& rect);

	void operator*=(_In_ const RECT& rect);
	void operator*=(_In_ const CRavidLine<int>& line);
	void operator*=(_In_ const CRavidLine<long long>& line);
	void operator*=(_In_ const CRavidLine<float>& line);
	void operator*=(_In_ const CRavidLine<double>& line);
	void operator*=(_In_ const CRavidRect<int>& rect);
	void operator*=(_In_ const CRavidRect<long long>& rect);
	void operator*=(_In_ const CRavidRect<float>& rect);
	void operator*=(_In_ const CRavidRect<double>& rect);

	void operator/=(_In_ const RECT& rect);
	void operator/=(_In_ const CRavidLine<int>& line);
	void operator/=(_In_ const CRavidLine<long long>& line);
	void operator/=(_In_ const CRavidLine<float>& line);
	void operator/=(_In_ const CRavidLine<double>& line);
	void operator/=(_In_ const CRavidRect<int>& rect);
	void operator/=(_In_ const CRavidRect<long long>& rect);
	void operator/=(_In_ const CRavidRect<float>& rect);
	void operator/=(_In_ const CRavidRect<double>& rect);
#pragma endregion

#pragma region [Arithmetic operator]
	CRavidLine<double> operator+(_In_ const int& value);
	CRavidLine<double> operator+(_In_ const long long& value);
	CRavidLine<double> operator+(_In_ const float& value);
	CRavidLine<double> operator+(_In_ const double& value);
	CRavidLine<double> operator+(_In_ const POINT& point);
	CRavidLine<double> operator+(_In_ const SIZE& size);
	CRavidLine<double> operator+(_In_ const RECT& rect);
	CRavidLine<double> operator+(_In_ const CRavidPoint<int>& point);
	CRavidLine<double> operator+(_In_ const CRavidPoint<long long>& point);
	CRavidLine<double> operator+(_In_ const CRavidPoint<float>& point);
	CRavidLine<double> operator+(_In_ const CRavidPoint<double>& point);
	CRavidLine<double> operator+(_In_ const CRavidLine<int>& line);
	CRavidLine<double> operator+(_In_ const CRavidLine<long long>& line);
	CRavidLine<double> operator+(_In_ const CRavidLine<float>& line);
	CRavidLine<double> operator+(_In_ const CRavidLine<double>& line);
	CRavidLine<double> operator+(_In_ const CRavidRect<int>& rect);
	CRavidLine<double> operator+(_In_ const CRavidRect<long long>& rect);
	CRavidLine<double> operator+(_In_ const CRavidRect<float>& rect);
	CRavidLine<double> operator+(_In_ const CRavidRect<double>& rect);

	CRavidLine<double> operator-(_In_ const int& value);
	CRavidLine<double> operator-(_In_ const long long& value);
	CRavidLine<double> operator-(_In_ const float& value);
	CRavidLine<double> operator-(_In_ const double& value);
	CRavidLine<double> operator-(_In_ const POINT& point);
	CRavidLine<double> operator-(_In_ const SIZE& size);
	CRavidLine<double> operator-(_In_ const RECT& rect);
	CRavidLine<double> operator-(_In_ const CRavidPoint<int>& point);
	CRavidLine<double> operator-(_In_ const CRavidPoint<long long>& point);
	CRavidLine<double> operator-(_In_ const CRavidPoint<float>& point);
	CRavidLine<double> operator-(_In_ const CRavidPoint<double>& point);
	CRavidLine<double> operator-(_In_ const CRavidLine<int>& line);
	CRavidLine<double> operator-(_In_ const CRavidLine<long long>& line);
	CRavidLine<double> operator-(_In_ const CRavidLine<float>& line);
	CRavidLine<double> operator-(_In_ const CRavidLine<double>& line);
	CRavidLine<double> operator-(_In_ const CRavidRect<int>& rect);
	CRavidLine<double> operator-(_In_ const CRavidRect<long long>& rect);
	CRavidLine<double> operator-(_In_ const CRavidRect<float>& rect);
	CRavidLine<double> operator-(_In_ const CRavidRect<double>& rect);

	CRavidLine<double> operator*(_In_ const int& value);
	CRavidLine<double> operator*(_In_ const long long& value);
	CRavidLine<double> operator*(_In_ const float& value);
	CRavidLine<double> operator*(_In_ const double& value);
	CRavidLine<double> operator*(_In_ const POINT& point);
	CRavidLine<double> operator*(_In_ const SIZE& size);
	CRavidLine<double> operator*(_In_ const RECT& rect);
	CRavidLine<double> operator*(_In_ const CRavidPoint<int>& point);
	CRavidLine<double> operator*(_In_ const CRavidPoint<long long>& point);
	CRavidLine<double> operator*(_In_ const CRavidPoint<float>& point);
	CRavidLine<double> operator*(_In_ const CRavidPoint<double>& point);
	CRavidLine<double> operator*(_In_ const CRavidLine<int>& line);
	CRavidLine<double> operator*(_In_ const CRavidLine<long long>& line);
	CRavidLine<double> operator*(_In_ const CRavidLine<float>& line);
	CRavidLine<double> operator*(_In_ const CRavidLine<double>& line);
	CRavidLine<double> operator*(_In_ const CRavidRect<int>& rect);
	CRavidLine<double> operator*(_In_ const CRavidRect<long long>& rect);
	CRavidLine<double> operator*(_In_ const CRavidRect<float>& rect);
	CRavidLine<double> operator*(_In_ const CRavidRect<double>& rect);

	CRavidLine<double> operator/(_In_ const int& value);
	CRavidLine<double> operator/(_In_ const long long& value);
	CRavidLine<double> operator/(_In_ const float& value);
	CRavidLine<double> operator/(_In_ const double& value);
	CRavidLine<double> operator/(_In_ const POINT& point);
	CRavidLine<double> operator/(_In_ const SIZE& size);
	CRavidLine<double> operator/(_In_ const RECT& rect);
	CRavidLine<double> operator/(_In_ const CRavidPoint<int>& point);
	CRavidLine<double> operator/(_In_ const CRavidPoint<long long>& point);
	CRavidLine<double> operator/(_In_ const CRavidPoint<float>& point);
	CRavidLine<double> operator/(_In_ const CRavidPoint<double>& point);
	CRavidLine<double> operator/(_In_ const CRavidLine<int>& line);
	CRavidLine<double> operator/(_In_ const CRavidLine<long long>& line);
	CRavidLine<double> operator/(_In_ const CRavidLine<float>& line);
	CRavidLine<double> operator/(_In_ const CRavidLine<double>& line);
	CRavidLine<double> operator/(_In_ const CRavidRect<int>& rect);
	CRavidLine<double> operator/(_In_ const CRavidRect<long long>& rect);
	CRavidLine<double> operator/(_In_ const CRavidRect<float>& rect);
	CRavidLine<double> operator/(_In_ const CRavidRect<double>& rect);
#pragma endregion
	
#pragma region [GetRect]
	/**
	* 현재 좌표를 CRavidRect<T> 형태로 계산 반환합니다.
	*
	* return value : 현재 객체의 최소, 최대 x ,y 값으로 반환합니다.
	*/
	virtual CRavidRect<T> GetRect();
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
	* return value : 현재 객체는 면적이 없으므로 0이 반환됩니다.
	*/
	virtual double GetArea() override;
#pragma endregion
	
#pragma region [GetScalar]
	/**
	* 현재 객체의 길이을 계산합니다.
	*
	* return value : 현재 객체는 길이를 반환됩니다.
	*/
	double GetScalar();
#pragma endregion
	
#pragma region [GetUnitVector]
	/**
	* 현재 객체의 단위 벡터를 계산합니다.
	* 
	* return value : 계산에 실패하면 (0, 0) 의 값을 반환합니다.
	*/
	CRavidPoint<double> GetUnitVector();
#pragma endregion
	
#pragma region [GetUnitVector]
	/**
	* 현재 객체의 법선(수직) 벡터를 계산합니다.
	* 
	* return value : 계산에 실패하면 (0, 0) 의 값을 반환합니다.
	*/	
	CRavidPoint<double> GetNormalVector();
#pragma endregion

#pragma region [Reduce]
	/**
	* 현재 객체의 길이를 입력된 길이의 2배 만큼 축소합니다.
	*
	* param[in] halfSize : 길이
	* 
	* return value : 계산에 실패하면 flase 를 반환합니다.
	*/	
	virtual bool Reduce(_In_ int halfSize);
	/**
	* 현재 객체의 길이를 입력된 길이의 2배 만큼 축소합니다.
	*
	* param[in] halfSize : 길이
	* 
	* return value : 계산에 실패하면 flase 를 반환합니다.
	*/	
	virtual bool Reduce(_In_ long long halfSize);
	/**
	* 현재 객체의 길이를 입력된 길이의 2배 만큼 축소합니다.
	*
	* param[in] halfSize : 길이
	* 
	* return value : 계산에 실패하면 flase 를 반환합니다.
	*/	
	virtual bool Reduce(_In_ float halfSize);
	/**
	* 현재 객체의 길이를 입력된 길이의 2배 만큼 축소합니다.
	*
	* param[in] halfSize : 길이
	* 
	* return value : 계산에 실패하면 flase 를 반환합니다.
	*/	
	virtual bool Reduce(_In_ double halfSize);
#pragma endregion
	
#pragma region [Reduce]
	/**
	* 현재 객체의 길이를 입력된 길이의 2배 만큼 확대합니다.
	*
	* param[in] halfSize : 길이
	* 
	* return value : 계산에 실패하면 (0, 0) 의 값을 반환합니다.
	*/	
	virtual bool Extend(_In_ int halfSize);
	/**
	* 현재 객체의 길이를 입력된 길이의 2배 만큼 확대합니다.
	*
	* param[in] halfSize : 길이
	* 
	* return value : 계산에 실패하면 (0, 0) 의 값을 반환합니다.
	*/	
	virtual bool Extend(_In_ long long halfSize);
	/**
	* 현재 객체의 길이를 입력된 길이의 2배 만큼 확대합니다.
	*
	* param[in] halfSize : 길이
	* 
	* return value : 계산에 실패하면 (0, 0) 의 값을 반환합니다.
	*/	
	virtual bool Extend(_In_ float halfSize);
	/**
	* 현재 객체의 길이를 입력된 길이의 2배 만큼 확대합니다.
	*
	* param[in] halfSize : 길이
	* 
	* return value : 계산에 실패하면 (0, 0) 의 값을 반환합니다.
	*/	
	virtual bool Extend(_In_ double halfSize);
#pragma endregion

#pragma region [GetPointsDirection]
	/**
	* 1번째 현재 좌표와 상대 좌표를 이용하여 2번째 현재 좌표의 방향성을 계산합니다. (this.pt1 - (x, y) - this.pt2)
	*
	* param[in] x : x 좌표
	* param[in] y : y 좌표
	*
	* return value : 계산 결과는 ERavidPointsDirection 로 반환합니다.
	*                ERavidPointsDirection_Invalid : 계산 실패 혹은 오류 발생
	*                ERavidPointsDirection_CW : 시계 방향
	*                ERavidPointsDirection_CCW : 반시계 방향
	*                ERavidPointsDirection_Straight : 직진성 방향	
	*/
	ERavidPointsDirection GetPointsDirection(_In_ double x, _In_ double y);
	/**
	* 1번째 현재 좌표와 상대 좌표를 이용하여 2번째 현재 좌표의 방향성을 계산합니다. (this.pt1 - (x, y) - this.pt2)
	*
	* param[in] refPoint : x, y 좌표 (Reference)
	*
	* return value : 계산 결과는 ERavidPointsDirection 로 반환합니다.
	*                ERavidPointsDirection_Invalid : 계산 실패 혹은 오류 발생
	*                ERavidPointsDirection_CW : 시계 방향
	*                ERavidPointsDirection_CCW : 반시계 방향
	*                ERavidPointsDirection_Straight : 직진성 방향	
	*/
	ERavidPointsDirection GetPointsDirection(_In_ CRavidPoint<int>& refPoint);
	/**
	* 1번째 현재 좌표와 상대 좌표를 이용하여 2번째 현재 좌표의 방향성을 계산합니다. (this.pt1 - (x, y) - this.pt2)
	*
	* param[in] pPoint : x, y 좌표 (Pointer)
	*
	* return value : 계산 결과는 ERavidPointsDirection 로 반환합니다.
	*                ERavidPointsDirection_Invalid : 계산 실패 혹은 오류 발생
	*                ERavidPointsDirection_CW : 시계 방향
	*                ERavidPointsDirection_CCW : 반시계 방향
	*                ERavidPointsDirection_Straight : 직진성 방향	
	*/
	ERavidPointsDirection GetPointsDirection(_In_ CRavidPoint<int>* pPoint);
	/**
	* 1번째 현재 좌표와 상대 좌표를 이용하여 2번째 현재 좌표의 방향성을 계산합니다. (this.pt1 - (x, y) - this.pt2)
	*
	* param[in] refPoint : x, y 좌표 (Reference)
	*
	* return value : 계산 결과는 ERavidPointsDirection 로 반환합니다.
	*                ERavidPointsDirection_Invalid : 계산 실패 혹은 오류 발생
	*                ERavidPointsDirection_CW : 시계 방향
	*                ERavidPointsDirection_CCW : 반시계 방향
	*                ERavidPointsDirection_Straight : 직진성 방향	
	*/
	ERavidPointsDirection GetPointsDirection(_In_ CRavidPoint<long long>& refPoint);
	/**
	* 1번째 현재 좌표와 상대 좌표를 이용하여 2번째 현재 좌표의 방향성을 계산합니다. (this.pt1 - (x, y) - this.pt2)
	*
	* param[in] pPoint : x, y 좌표 (Pointer)
	*
	* return value : 계산 결과는 ERavidPointsDirection 로 반환합니다.
	*                ERavidPointsDirection_Invalid : 계산 실패 혹은 오류 발생
	*                ERavidPointsDirection_CW : 시계 방향
	*                ERavidPointsDirection_CCW : 반시계 방향
	*                ERavidPointsDirection_Straight : 직진성 방향	
	*/
	ERavidPointsDirection GetPointsDirection(_In_ CRavidPoint<long long>* pPoint);
	/**
	* 1번째 현재 좌표와 상대 좌표를 이용하여 2번째 현재 좌표의 방향성을 계산합니다. (this.pt1 - (x, y) - this.pt2)
	*
	* param[in] refPoint : x, y 좌표 (Reference)
	*
	* return value : 계산 결과는 ERavidPointsDirection 로 반환합니다.
	*                ERavidPointsDirection_Invalid : 계산 실패 혹은 오류 발생
	*                ERavidPointsDirection_CW : 시계 방향
	*                ERavidPointsDirection_CCW : 반시계 방향
	*                ERavidPointsDirection_Straight : 직진성 방향	
	*/
	ERavidPointsDirection GetPointsDirection(_In_ CRavidPoint<float>& refPoint);
	/**
	* 1번째 현재 좌표와 상대 좌표를 이용하여 2번째 현재 좌표의 방향성을 계산합니다. (this.pt1 - (x, y) - this.pt2)
	*
	* param[in] pPoint : x, y 좌표 (Pointer)
	*
	* return value : 계산 결과는 ERavidPointsDirection 로 반환합니다.
	*                ERavidPointsDirection_Invalid : 계산 실패 혹은 오류 발생
	*                ERavidPointsDirection_CW : 시계 방향
	*                ERavidPointsDirection_CCW : 반시계 방향
	*                ERavidPointsDirection_Straight : 직진성 방향	
	*/
	ERavidPointsDirection GetPointsDirection(_In_ CRavidPoint<float>* pPoint);
	/**
	* 1번째 현재 좌표와 상대 좌표를 이용하여 2번째 현재 좌표의 방향성을 계산합니다. (this.pt1 - (x, y) - this.pt2)
	*
	* param[in] refPoint : x, y 좌표 (Reference)
	*
	* return value : 계산 결과는 ERavidPointsDirection 로 반환합니다.
	*                ERavidPointsDirection_Invalid : 계산 실패 혹은 오류 발생
	*                ERavidPointsDirection_CW : 시계 방향
	*                ERavidPointsDirection_CCW : 반시계 방향
	*                ERavidPointsDirection_Straight : 직진성 방향	
	*/
	ERavidPointsDirection GetPointsDirection(_In_ CRavidPoint<double>& refPoint);
	/**
	* 1번째 현재 좌표와 상대 좌표를 이용하여 2번째 현재 좌표의 방향성을 계산합니다. (this.pt1 - (x, y) - this.pt2)
	*
	* param[in] pPoint : x, y 좌표 (Pointer)
	*
	* return value : 계산 결과는 ERavidPointsDirection 로 반환합니다.
	*                ERavidPointsDirection_Invalid : 계산 실패 혹은 오류 발생
	*                ERavidPointsDirection_CW : 시계 방향
	*                ERavidPointsDirection_CCW : 반시계 방향
	*                ERavidPointsDirection_Straight : 직진성 방향	
	*/
	ERavidPointsDirection GetPointsDirection(_In_ CRavidPoint<double>* pPoint);
#pragma endregion
	
#pragma region [InfiniteLine]
	/**
	* 현재 객체가 무한한 직선인지 확인합니다.
	*
	* return value : 무한한 직선은 ture, 선분은 false 를 반환합니다.
	*/
	bool IsInfiniteLine();
	/**
	* 현재 객체를 무한한 직선으로 계산합니다.
	*
	* return value : Ravid 에서 제안하는 무한한 직선을 반환합니다.
	*/
	CRavidLine<double> GetInfiniteLine();
#pragma endregion
	
#pragma region [Equation]
	/**
	* 직선의 방정식 일반형 A, B, C 를 계산합니다.
	*
	* param[in] refA : x 계수 (Reference)
	* param[in] refB : y 계수 (Reference)
	* param[in] refC : x, y 상수 (Reference)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	bool GetEquation(_Out_ double& refA, _Out_ double& refB, _Out_ double& refC);
	/**
	* 직선의 방정식 일반형 A, B, C 를 계산합니다.
	*
	* param[in] pA : x 계수 (Pointer)
	* param[in] pB : y 계수 (Pointer)
	* param[in] pC : x, y 상수 (Pointer)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	bool GetEquation(_Out_ double* pA, _Out_ double* pB, _Out_ double* pC);
	/**
	* 직선의 방정식 일반형 A, B, C 를 계산하여 현재 객체를 직선으로 만듭니다.
	*
	* param[in] dblA : x 계수
	* param[in] dblB : y 계수
	* param[in] dblC : x, y 상수
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	bool SetEquation(_In_ double dblA, _In_ double dblB, _In_ double dblC = 0.0);
	/**
	* x 에 대한 직선의 방정식 표준형을 계산합니다.
	* 수평은 오류가 발생합니다.
	*
	* param[in] refYCoefficient : y 계수 (Reference)
	* param[in] refConstant : 상수항 (Reference)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	bool GetEquationForX(_Out_ double& refYCoefficient, _Out_ double& refConstant);
	/**
	* x 에 대한 직선의 방정식 표준형을 계산합니다.
	* 수평은 오류가 발생합니다.
	*
	* param[in] pYCoefficient : y 계수 (Pointer)
	* param[in] pConstant : 상수항 (Pointer)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	bool GetEquationForX(_Out_ double* pYCoefficient, _Out_ double* pConstant);
	/**
	* y 에 대한 직선의 방정식 표준형을 계산합니다.
	* 수평은 오류가 발생합니다.
	*
	* param[in] refXCoefficient : x 계수 (Reference)
	* param[in] refConstant : 상수항 (Reference)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	bool GetEquationForY(_Out_ double& refXCoefficient, _Out_ double& refConstant);
	/**
	* y 에 대한 직선의 방정식 표준형을 계산합니다.
	* 수평은 오류가 발생합니다.
	*
	* param[in] pXCoefficient : x 계수 (Pointer)
	* param[in] pConstant : 상수항 (Pointer)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	bool GetEquationForY(_Out_ double* pXCoefficient, _Out_ double* pConstant);
	/**
	* x 에 대한 직선의 방정식 표준형을 계산하여 현재 객체를 직선으로 만듭니다.
	* 수평은 오류가 발생합니다.
	*
	* param[in] dblYCoefficient : y 계수
	* param[in] dblConstant : 상수항
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	bool SetEquationForX(_In_ double dblYCoefficient, _In_ double dblConstant);
	/**
	* y 에 대한 직선의 방정식 표준형을 계산하여 현재 객체를 직선으로 만듭니다.
	* 수평은 오류가 발생합니다.
	*
	* param[in] dblXCoefficient : x 계수
	* param[in] dblConstant : 상수항
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	bool SetEquationForY(_In_ double dblXCoefficient, _In_ double dblConstant);
#pragma endregion
	
#pragma region [GetIntersection]
	/**
	* 현재 객체와 상대 직선과의 교점을 계산합니다.
	* 선분인 경우에는 범위 내만 계산합니다.
	*
	* param[in] refLine : x, y 좌표 (Reference)
	*
	* return value : 교점이 없으면 INF 의 값으로 설정된 좌표가 반환됩니다.
	*/
	CRavidPoint<double> GetIntersection(_In_ CRavidLine<int>& refLine);
	/**
	* 현재 객체와 상대 직선과의 교점을 계산합니다.
	* 선분인 경우에는 범위 내만 계산합니다.
	*
	* param[in] pLine : x, y 좌표 (Pointer)
	*
	* return value : 교점이 없으면 INF 의 값으로 설정된 좌표가 반환됩니다.
	*/
	CRavidPoint<double> GetIntersection(_In_ CRavidLine<int>* pLine);
	/**
	* 현재 객체와 상대 직선과의 교점을 계산합니다.
	* 선분인 경우에는 범위 내만 계산합니다.
	*
	* param[in] refLine : x, y 좌표 (Reference)
	*
	* return value : 교점이 없으면 INF 의 값으로 설정된 좌표가 반환됩니다.
	*/
	CRavidPoint<double> GetIntersection(_In_ CRavidLine<long long>& refLine);
	/**
	* 현재 객체와 상대 직선과의 교점을 계산합니다.
	* 선분인 경우에는 범위 내만 계산합니다.
	*
	* param[in] pLine : x, y 좌표 (Pointer)
	*
	* return value : 교점이 없으면 INF 의 값으로 설정된 좌표가 반환됩니다.
	*/
	CRavidPoint<double> GetIntersection(_In_ CRavidLine<long long>* pLine);
	/**
	* 현재 객체와 상대 객체와의 교점을 계산합니다.
	* 선분인 경우에는 범위 내만 계산합니다.
	*
	* param[in] refLine : x, y 좌표 (Reference)
	*
	* return value : 교점이 없으면 INF 의 값으로 설정된 좌표가 반환됩니다.
	*/
	CRavidPoint<double> GetIntersection(_In_ CRavidLine<float>& refLine);
	/**
	* 현재 객체와 상대 직선과의 교점을 계산합니다.
	* 선분인 경우에는 범위 내만 계산합니다.
	*
	* param[in] pLine : x, y 좌표 (Pointer)
	*
	* return value : 교점이 없으면 INF 의 값으로 설정된 좌표가 반환됩니다.
	*/
	CRavidPoint<double> GetIntersection(_In_ CRavidLine<float>* pLine);
	/**
	* 현재 객체와 상대 객체와의 교점을 계산합니다.
	* 선분인 경우에는 범위 내만 계산합니다.
	*
	* param[in] refLine : x, y 좌표 (Reference)
	*
	* return value : 교점이 없으면 INF 의 값으로 설정된 좌표가 반환됩니다.
	*/
	CRavidPoint<double> GetIntersection(_In_ CRavidLine<double>& refLine);
	/**
	* 현재 객체와 상대 직선과의 교점을 계산합니다.
	* 선분인 경우에는 범위 내만 계산합니다.
	*
	* param[in] pLine : x, y 좌표 (Pointer)
	*
	* return value : 교점이 없으면 INF 의 값으로 설정된 좌표가 반환됩니다.
	*/
	CRavidPoint<double> GetIntersection(_In_ CRavidLine<double>* pLine);
#pragma endregion

#pragma region [GetIntersection]
	/**
	* 현재 객체와 상대 객체와의 교점을 계산하여 인자값으로 반환합니다.
	*
	* param[in] refGeometry : 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산된 객체 리스트 (Pointer)
	*
	* return value : 교점이 없거나 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidRect<int>& refGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 교점을 계산하여 인자값으로 반환합니다.
	*
	* param[in] pGeometry : 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산된 객체 리스트 (Pointer)
	*
	* return value : 교점이 없거나 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidRect<int>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 교점을 계산하여 인자값으로 반환합니다.
	*
	* param[in] refGeometry : 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산된 객체 리스트 (Pointer)
	*
	* return value : 교점이 없거나 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidRect<long long>& refGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 교점을 계산하여 인자값으로 반환합니다.
	*
	* param[in] pGeometry : 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산된 객체 리스트 (Pointer)
	*
	* return value : 교점이 없거나 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidRect<long long>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 교점을 계산하여 인자값으로 반환합니다.
	*
	* param[in] refGeometry : 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산된 객체 리스트 (Pointer)
	*
	* return value : 교점이 없거나 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidRect<float>& refGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 교점을 계산하여 인자값으로 반환합니다.
	*
	* param[in] pGeometry : 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산된 객체 리스트 (Pointer)
	*
	* return value : 교점이 없거나 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidRect<float>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 교점을 계산하여 인자값으로 반환합니다.
	*
	* param[in] refGeometry : 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산된 객체 리스트 (Pointer)
	*
	* return value : 교점이 없거나 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidRect<double>& refGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 교점을 계산하여 인자값으로 반환합니다.
	*
	* param[in] pGeometry : 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산된 객체 리스트 (Pointer)
	*
	* return value : 교점이 없거나 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidRect<double>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 교점을 계산하여 인자값으로 반환합니다.
	*
	* param[in] refGeometry : 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산된 객체 리스트 (Pointer)
	*
	* return value : 교점이 없거나 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidQuadrangle<int>& refGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 교점을 계산하여 인자값으로 반환합니다.
	*
	* param[in] pGeometry : 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산된 객체 리스트 (Pointer)
	*
	* return value : 교점이 없거나 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidQuadrangle<int>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 교점을 계산하여 인자값으로 반환합니다.
	*
	* param[in] refGeometry : 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산된 객체 리스트 (Pointer)
	*
	* return value : 교점이 없거나 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidQuadrangle<long long>& refGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 교점을 계산하여 인자값으로 반환합니다.
	*
	* param[in] pGeometry : 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산된 객체 리스트 (Pointer)
	*
	* return value : 교점이 없거나 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidQuadrangle<long long>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 교점을 계산하여 인자값으로 반환합니다.
	*
	* param[in] refGeometry : 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산된 객체 리스트 (Pointer)
	*
	* return value : 교점이 없거나 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidQuadrangle<float>& refGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 교점을 계산하여 인자값으로 반환합니다.
	*
	* param[in] pGeometry : 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산된 객체 리스트 (Pointer)
	*
	* return value : 교점이 없거나 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidQuadrangle<float>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 교점을 계산하여 인자값으로 반환합니다.
	*
	* param[in] refGeometry : 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산된 객체 리스트 (Pointer)
	*
	* return value : 교점이 없거나 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidQuadrangle<double>& refGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 교점을 계산하여 인자값으로 반환합니다.
	*
	* param[in] pGeometry : 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산된 객체 리스트 (Pointer)
	*
	* return value : 교점이 없거나 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidQuadrangle<double>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 교점을 계산하여 인자값으로 반환합니다.
	*
	* param[in] refGeometry : 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산된 객체 리스트 (Pointer)
	*
	* return value : 교점이 없거나 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidCircle<int>& refGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 교점을 계산하여 인자값으로 반환합니다.
	*
	* param[in] pGeometry : 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산된 객체 리스트 (Pointer)
	*
	* return value : 교점이 없거나 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidCircle<int>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 교점을 계산하여 인자값으로 반환합니다.
	*
	* param[in] refGeometry : 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산된 객체 리스트 (Pointer)
	*
	* return value : 교점이 없거나 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidCircle<long long>& refGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 교점을 계산하여 인자값으로 반환합니다.
	*
	* param[in] pGeometry : 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산된 객체 리스트 (Pointer)
	*
	* return value : 교점이 없거나 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidCircle<long long>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 교점을 계산하여 인자값으로 반환합니다.
	*
	* param[in] refGeometry : 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산된 객체 리스트 (Pointer)
	*
	* return value : 교점이 없거나 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidCircle<float>& refGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 교점을 계산하여 인자값으로 반환합니다.
	*
	* param[in] pGeometry : 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산된 객체 리스트 (Pointer)
	*
	* return value : 교점이 없거나 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidCircle<float>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 교점을 계산하여 인자값으로 반환합니다.
	*
	* param[in] refGeometry : 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산된 객체 리스트 (Pointer)
	*
	* return value : 교점이 없거나 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidCircle<double>& refGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 교점을 계산하여 인자값으로 반환합니다.
	*
	* param[in] pGeometry : 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산된 객체 리스트 (Pointer)
	*
	* return value : 교점이 없거나 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidCircle<double>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 교점을 계산하여 인자값으로 반환합니다.
	*
	* param[in] refGeometry : 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산된 객체 리스트 (Pointer)
	*
	* return value : 교점이 없거나 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidEllipse<int>& refGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 교점을 계산하여 인자값으로 반환합니다.
	*
	* param[in] pGeometry : 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산된 객체 리스트 (Pointer)
	*
	* return value : 교점이 없거나 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidEllipse<int>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 교점을 계산하여 인자값으로 반환합니다.
	*
	* param[in] refGeometry : 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산된 객체 리스트 (Pointer)
	*
	* return value : 교점이 없거나 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidEllipse<long long>& refGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 교점을 계산하여 인자값으로 반환합니다.
	*
	* param[in] pGeometry : 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산된 객체 리스트 (Pointer)
	*
	* return value : 교점이 없거나 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidEllipse<long long>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 교점을 계산하여 인자값으로 반환합니다.
	*
	* param[in] refGeometry : 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산된 객체 리스트 (Pointer)
	*
	* return value : 교점이 없거나 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidEllipse<float>& refGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 교점을 계산하여 인자값으로 반환합니다.
	*
	* param[in] pGeometry : 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산된 객체 리스트 (Pointer)
	*
	* return value : 교점이 없거나 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidEllipse<float>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 교점을 계산하여 인자값으로 반환합니다.
	*
	* param[in] refGeometry : 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산된 객체 리스트 (Pointer)
	*
	* return value : 교점이 없거나 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidEllipse<double>& refGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 교점을 계산하여 인자값으로 반환합니다.
	*
	* param[in] pGeometry : 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산된 객체 리스트 (Pointer)
	*
	* return value : 교점이 없거나 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidEllipse<double>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 교점을 계산하여 인자값으로 반환합니다.
	*
	* param[in] refGeometry : 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산된 객체 리스트 (Pointer)
	*
	* return value : 교점이 없거나 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidPolygon& refGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 교점을 계산하여 인자값으로 반환합니다.
	*
	* param[in] pGeometry : 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산된 객체 리스트 (Pointer)
	*
	* return value : 교점이 없거나 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidPolygon* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
#pragma endregion

#pragma region [GetOrthogonalDistance]
	/**
	* 현재 객체와 상대 좌표와의 최단거리(수직)를 계산합니다.
	* 직선, 선분 상관없이 최단 거리를 계산합니다.
	*
	* param[in] refPoint : 상대 객체 (Reference)
	*
	* return value : 계산에 실패하면 INF 의 값을 반환합니다.
	*/
	double GetOrthogonalDistance(_In_ CRavidPoint<int>& refPoint);
	/**
	* 현재 객체와 상대 좌표와의 최단거리(수직)를 계산합니다.
	* 직선, 선분 상관없이 최단 거리를 계산합니다.
	*
	* param[in] pPoint : 상대 객체 (Pointer)
	*
	* return value : 계산에 실패하면 INF 의 값을 반환합니다.
	*/
	double GetOrthogonalDistance(_In_ CRavidPoint<int>* pPoint);
	/**
	* 현재 객체와 상대 좌표와의 최단거리(수직)를 계산합니다.
	* 직선, 선분 상관없이 최단 거리를 계산합니다.
	*
	* param[in] refPoint : 상대 객체 (Reference)
	*
	* return value : 계산에 실패하면 INF 의 값을 반환합니다.
	*/
	double GetOrthogonalDistance(_In_ CRavidPoint<long long>& refPoint);
	/**
	* 현재 객체와 상대 좌표와의 최단거리(수직)를 계산합니다.
	* 직선, 선분 상관없이 최단 거리를 계산합니다.
	*
	* param[in] pPoint : 상대 객체 (Pointer)
	*
	* return value : 계산에 실패하면 INF 의 값을 반환합니다.
	*/
	double GetOrthogonalDistance(_In_ CRavidPoint<long long>* pPoint);
	/**
	* 현재 객체와 상대 좌표와의 최단거리(수직)를 계산합니다.
	* 직선, 선분 상관없이 최단 거리를 계산합니다.
	*
	* param[in] refPoint : 상대 객체 (Reference)
	*
	* return value : 계산에 실패하면 INF 의 값을 반환합니다.
	*/
	double GetOrthogonalDistance(_In_ CRavidPoint<float>& refPoint);
	/**
	* 현재 객체와 상대 좌표와의 최단거리(수직)를 계산합니다.
	* 직선, 선분 상관없이 최단 거리를 계산합니다.
	*
	* param[in] pPoint : 상대 객체 (Pointer)
	*
	* return value : 계산에 실패하면 INF 의 값을 반환합니다.
	*/
	double GetOrthogonalDistance(_In_ CRavidPoint<float>* pPoint);
	/**
	* 현재 객체와 상대 좌표와의 최단거리(수직)를 계산합니다.
	* 직선, 선분 상관없이 최단 거리를 계산합니다.
	*
	* param[in] refPoint : 상대 객체 (Reference)
	*
	* return value : 계산에 실패하면 INF 의 값을 반환합니다.
	*/
	double GetOrthogonalDistance(_In_ CRavidPoint<double>& refPoint);
	/**
	* 현재 객체와 상대 좌표와의 최단거리(수직)를 계산합니다.
	* 직선, 선분 상관없이 최단 거리를 계산합니다.
	*
	* param[in] pPoint : 상대 객체 (Pointer)
	*
	* return value : 계산에 실패하면 INF 의 값을 반환합니다.
	*/
	double GetOrthogonalDistance(_In_ CRavidPoint<double>* pPoint);
#pragma endregion

#pragma region [GetOrthogonalPoint]
	/**
	* 현재 객체와 상대 좌표와의 최단거리(수직)의 좌표를 계산합니다.
	* 선분일 경우에는 범위내에 위치해야만 합니다.
	*
	* param[in] refPoint : 상대 객체 (Reference)
	*
	* return value : 계산에 실패하면 INF 의 값을 반환합니다.
	*/
	CRavidPoint<double> GetOrthogonalPoint(_In_ CRavidPoint<int>& refPoint);
	/**
	* 현재 객체와 상대 좌표와의 최단거리(수직)의 좌표를 계산합니다.
	* 선분일 경우에는 범위내에 위치해야만 합니다.
	*
	* param[in] pPoint : 상대 객체 (Pointer)
	*
	* return value : 계산에 실패하면 INF 의 값을 반환합니다.
	*/
	CRavidPoint<double> GetOrthogonalPoint(_In_ CRavidPoint<int>* pPoint);
	/**
	* 현재 객체와 상대 좌표와의 최단거리(수직)의 좌표를 계산합니다.
	* 선분일 경우에는 범위내에 위치해야만 합니다.
	*
	* param[in] refPoint : 상대 객체 (Reference)
	*
	* return value : 계산에 실패하면 INF 의 값을 반환합니다.
	*/
	CRavidPoint<double> GetOrthogonalPoint(_In_ CRavidPoint<long long>& refPoint);
	/**
	* 현재 객체와 상대 좌표와의 최단거리(수직)의 좌표를 계산합니다.
	* 선분일 경우에는 범위내에 위치해야만 합니다.
	*
	* param[in] pPoint : 상대 객체 (Pointer)
	*
	* return value : 계산에 실패하면 INF 의 값을 반환합니다.
	*/
	CRavidPoint<double> GetOrthogonalPoint(_In_ CRavidPoint<long long>* pPoint);
	/**
	* 현재 객체와 상대 좌표와의 최단거리(수직)의 좌표를 계산합니다.
	* 선분일 경우에는 범위내에 위치해야만 합니다.
	*
	* param[in] refPoint : 상대 객체 (Reference)
	*
	* return value : 계산에 실패하면 INF 의 값을 반환합니다.
	*/
	CRavidPoint<double> GetOrthogonalPoint(_In_ CRavidPoint<float>& refPoint);
	/**
	* 현재 객체와 상대 좌표와의 최단거리(수직)의 좌표를 계산합니다.
	* 선분일 경우에는 범위내에 위치해야만 합니다.
	*
	* param[in] pPoint : 상대 객체 (Pointer)
	*
	* return value : 계산에 실패하면 INF 의 값을 반환합니다.
	*/
	CRavidPoint<double> GetOrthogonalPoint(_In_ CRavidPoint<float>* pPoint);
	/**
	* 현재 객체와 상대 좌표와의 최단거리(수직)의 좌표를 계산합니다.
	* 선분일 경우에는 범위내에 위치해야만 합니다.
	*
	* param[in] refPoint : 상대 객체 (Reference)
	*
	* return value : 계산에 실패하면 INF 의 값을 반환합니다.
	*/
	CRavidPoint<double> GetOrthogonalPoint(_In_ CRavidPoint<double>& refPoint);
	/**
	* 현재 객체와 상대 좌표와의 최단거리(수직)의 좌표를 계산합니다.
	* 선분일 경우에는 범위내에 위치해야만 합니다.
	*
	* param[in] pPoint : 상대 객체 (Pointer)
	*
	* return value : 계산에 실패하면 INF 의 값을 반환합니다.
	*/
	CRavidPoint<double> GetOrthogonalPoint(_In_ CRavidPoint<double>* pPoint);
#pragma endregion

#pragma region [GetAngle]
	/**
	* 현재 객체의 각도를 계산합니다.
	* 인덱스 0 좌표를 중심으로 인덱스 1 좌표의 각도를 계산합니다.
	*
	* return value : 계산에 실패하면 INF 의 값을 반환합니다.
	*/
	virtual double GetAngle() override;
#pragma endregion

#pragma region [GetInteriorAngle]
	/**
	* 현재 객체의 2점과 상대객체 1 좌표를 이용한 사이각을 계산합니다.
	* 
	* param[in] refPoint : 상대 객체 좌표 (Reference)
	*
	* return value : 계산에 실패하면 INF 를 반환합니다.
	*/
	double GetInteriorAngle(_In_ CRavidPoint<int>& refPoint);
	/**
	* 현재 객체의 2점과 상대객체 1 좌표를 이용한 사이각을 계산합니다.
	* 
	* param[in] pPoint : 상대 객체 좌표 (Pointer)
	*
	* return value : 계산에 실패하면 INF 를 반환합니다.
	*/
	double GetInteriorAngle(_In_ CRavidPoint<int>* pPoint);
	/**
	* 현재 객체의 2점과 상대객체 1 좌표를 이용한 사이각을 계산합니다.
	* 
	* param[in] refPoint : 상대 객체 좌표 (Reference)
	*
	* return value : 계산에 실패하면 INF 를 반환합니다.
	*/
	double GetInteriorAngle(_In_ CRavidPoint<long long>& refPoint);
	/**
	* 현재 객체의 2점과 상대객체 1 좌표를 이용한 사이각을 계산합니다.
	* 
	* param[in] pPoint : 상대 객체 좌표 (Pointer)
	*
	* return value : 계산에 실패하면 INF 를 반환합니다.
	*/
	double GetInteriorAngle(_In_ CRavidPoint<long long>* pPoint);
	/**
	* 현재 객체의 2점과 상대객체 1 좌표를 이용한 사이각을 계산합니다.
	* 
	* param[in] refPoint : 상대 객체 좌표 (Reference)
	*
	* return value : 계산에 실패하면 INF 를 반환합니다.
	*/
	double GetInteriorAngle(_In_ CRavidPoint<float>& refPoint);
	/**
	* 현재 객체의 2점과 상대객체 1 좌표를 이용한 사이각을 계산합니다.
	* 
	* param[in] pPoint : 상대 객체 좌표 (Pointer)
	*
	* return value : 계산에 실패하면 INF 를 반환합니다.
	*/
	double GetInteriorAngle(_In_ CRavidPoint<float>* pPoint);
	/**
	* 현재 객체의 2점과 상대객체 1 좌표를 이용한 사이각을 계산합니다.
	* 
	* param[in] refPoint : 상대 객체 좌표 (Reference)
	*
	* return value : 계산에 실패하면 INF 를 반환합니다.
	*/
	double GetInteriorAngle(_In_ CRavidPoint<double>& refPoint);
	/**
	* 현재 객체의 2점과 상대객체 1 좌표를 이용한 사이각을 계산합니다.
	* 
	* param[in] pPoint : 상대 객체 좌표 (Pointer)
	*
	* return value : 계산에 실패하면 INF 를 반환합니다.
	*/
	double GetInteriorAngle(_In_ CRavidPoint<double>* pPoint);
	/**
	* 현재 객체의 2점과 상대 객체의 2 좌표를 이용한 사이각을 계산합니다.
	* 
	* param[in] refLine : 상대 객체 (Reference)
	*
	* return value : 계산에 실패하면 INF 를 반환합니다.
	*/
	double GetInteriorAngle(_In_ CRavidLine<int>& refLine);
	/**
	* 현재 객체의 2점과 상대 객체의 2 좌표를 이용한 사이각을 계산합니다.
	* 
	* param[in] pLine : 상대 객체 (Pointer)
	*
	* return value : 계산에 실패하면 INF 를 반환합니다.
	*/
	double GetInteriorAngle(_In_ CRavidLine<int>* pLine);
	/**
	* 현재 객체의 2점과 상대 객체의 2 좌표를 이용한 사이각을 계산합니다.
	* 
	* param[in] refLine : 상대 객체 (Reference)
	*
	* return value : 계산에 실패하면 INF 를 반환합니다.
	*/
	double GetInteriorAngle(_In_ CRavidLine<long long>& refLine);
	/**
	* 현재 객체의 2점과 상대 객체의 2 좌표를 이용한 사이각을 계산합니다.
	* 
	* param[in] pLine : 상대 객체 (Pointer)
	*
	* return value : 계산에 실패하면 INF 를 반환합니다.
	*/
	double GetInteriorAngle(_In_ CRavidLine<long long>* pLine);
	/**
	* 현재 객체의 2점과 상대 객체의 2 좌표를 이용한 사이각을 계산합니다.
	* 
	* param[in] refLine : 상대 객체 (Reference)
	*
	* return value : 계산에 실패하면 INF 를 반환합니다.
	*/
	double GetInteriorAngle(_In_ CRavidLine<float>& refLine);
	/**
	* 현재 객체의 2점과 상대 객체의 2 좌표를 이용한 사이각을 계산합니다.
	* 
	* param[in] pLine : 상대 객체 (Pointer)
	*
	* return value : 계산에 실패하면 INF 를 반환합니다.
	*/
	double GetInteriorAngle(_In_ CRavidLine<float>* pLine);
	/**
	* 현재 객체의 2점과 상대 객체의 2 좌표를 이용한 사이각을 계산합니다.
	* 
	* param[in] refLine : 상대 객체 (Reference)
	*
	* return value : 계산에 실패하면 INF 를 반환합니다.
	*/
	double GetInteriorAngle(_In_ CRavidLine<double>& refLine);
	/**
	* 현재 객체의 2점과 상대 객체의 2 좌표를 이용한 사이각을 계산합니다.
	* 
	* param[in] pLine : 상대 객체 (Pointer)
	*
	* return value : 계산에 실패하면 INF 를 반환합니다.
	*/
	double GetInteriorAngle(_In_ CRavidLine<double>* pLine);
#pragma endregion

#pragma region [GetCircumcenter]
	/**
	* 현재 객체의 2점과 상대객체 1 좌표를 이용한 외접원의 중심을 계산합니다.
	* 
	* param[in] refPoint : 상대 객체 좌표 (Reference)
	*
	* return value : 계산에 실패하면 INF 를 반환합니다.
	*/
	CRavidPoint<double> GetCircumcenter(_In_ CRavidPoint<int>& refPoint);
	/**
	* 현재 객체의 2점과 상대객체 1 좌표를 이용한 외접원의 중심을 계산합니다.
	* 
	* param[in] pPoint : 상대 객체 좌표 (Pointer)
	*
	* return value : 계산에 실패하면 INF 를 반환합니다.
	*/
	CRavidPoint<double> GetCircumcenter(_In_ CRavidPoint<int>* pPoint);
	/**
	* 현재 객체의 2점과 상대객체 1 좌표를 이용한 외접원의 중심을 계산합니다.
	* 
	* param[in] refPoint : 상대 객체 좌표 (Reference)
	*
	* return value : 계산에 실패하면 INF 를 반환합니다.
	*/
	CRavidPoint<double> GetCircumcenter(_In_ CRavidPoint<long long>& refPoint);
	/**
	* 현재 객체의 2점과 상대객체 1 좌표를 이용한 외접원의 중심을 계산합니다.
	* 
	* param[in] pPoint : 상대 객체 좌표 (Pointer)
	*
	* return value : 계산에 실패하면 INF 를 반환합니다.
	*/
	CRavidPoint<double> GetCircumcenter(_In_ CRavidPoint<long long>* pPoint);
	/**
	* 현재 객체의 2점과 상대객체 1 좌표를 이용한 외접원의 중심을 계산합니다.
	* 
	* param[in] refPoint : 상대 객체 좌표 (Reference)
	*
	* return value : 계산에 실패하면 INF 를 반환합니다.
	*/
	CRavidPoint<double> GetCircumcenter(_In_ CRavidPoint<float>& refPoint);
	/**
	* 현재 객체의 2점과 상대객체 1 좌표를 이용한 외접원의 중심을 계산합니다.
	* 
	* param[in] pPoint : 상대 객체 좌표 (Pointer)
	*
	* return value : 계산에 실패하면 INF 를 반환합니다.
	*/
	CRavidPoint<double> GetCircumcenter(_In_ CRavidPoint<float>* pPoint);
	/**
	* 현재 객체의 2점과 상대객체 1 좌표를 이용한 외접원의 중심을 계산합니다.
	* 
	* param[in] refPoint : 상대 객체 좌표 (Reference)
	*
	* return value : 계산에 실패하면 INF 를 반환합니다.
	*/
	CRavidPoint<double> GetCircumcenter(_In_ CRavidPoint<double>& refPoint);
	/**
	* 현재 객체의 2점과 상대객체 1 좌표를 이용한 외접원의 중심을 계산합니다.
	* 
	* param[in] pPoint : 상대 객체 좌표 (Pointer)
	*
	* return value : 계산에 실패하면 INF 를 반환합니다.
	*/
	CRavidPoint<double> GetCircumcenter(_In_ CRavidPoint<double>* pPoint);
#pragma endregion

public:
#pragma region [Data]
	/**
	* 객체를 이루는 두 점 x, y 좌표를 나타내고, 2개의 배열로 정의됩니다.
	* 내부 계산에서 사용할 때 인덱스 0 의 좌표를 기준으로 인덱스 1의 좌표를 계산합니다.
	* 
	* CRavidPoint<T> rpPoints[2] : CRavidPoint 클래스로 CRavidLine 클래스의 템플릿 타입으로 설정됩니다.
	*/
	CRavidPoint<T> rpPoints[2];
#pragma endregion
};