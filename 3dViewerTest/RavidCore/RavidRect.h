#pragma once

#include "RavidGeometry.h"

namespace Ravid
{
	namespace Algorithms
	{
		class CRavidImage;
	}
}

/**
* "직사각형 (Rectangle)" 을 표현하는 클래스입니다.
* 데이터 타입은 int, long long, float, double 만 가능합니다.
* CRavidGeometry 를 상속받은 클래스로 "직사각형"과 관련된 알고리즘을 포함하고 있습니다.
*/
template <typename T>
class AFX_EXT_CLASS CRavidRect : public CRavidGeometry
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

#include "RavidRectCtor.hpp"

public:
	RavidUseDynamicCreation(this);

	static_assert(CheckIntegerAndRealNumber<T>::value, "CRavidRect template class is able to use type within int, long long, float and double.");

	CRavidRect();
	virtual ~CRavidRect();

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
	void operator=(_In_ const CRavidQuadrangle<int>& quad);
	void operator=(_In_ CRavidQuadrangle<int>* pQuad);
	void operator=(_In_ const CRavidQuadrangle<long long>& quad);
	void operator=(_In_ CRavidQuadrangle<long long>* pQuad);
	void operator=(_In_ const CRavidQuadrangle<float>& quad);
	void operator=(_In_ CRavidQuadrangle<float>* pQuad);
	void operator=(_In_ const CRavidQuadrangle<double>& quad);
	void operator=(_In_ CRavidQuadrangle<double>* pQuad);
	void operator=(_In_ const CRavidCircle<int>& cir);
	void operator=(_In_ CRavidCircle<int>* pCir);
	void operator=(_In_ const CRavidCircle<long long>& cir);
	void operator=(_In_ CRavidCircle<long long>* pCir);
	void operator=(_In_ const CRavidCircle<float>& cir);
	void operator=(_In_ CRavidCircle<float>* pCir);
	void operator=(_In_ const CRavidCircle<double>& cir);
	void operator=(_In_ CRavidCircle<double>* pCir);
	void operator=(_In_ const CRavidEllipse<int>& ell);
	void operator=(_In_ CRavidEllipse<int>* pEll);
	void operator=(_In_ const CRavidEllipse<long long>& ell);
	void operator=(_In_ CRavidEllipse<long long>* pEll);
	void operator=(_In_ const CRavidEllipse<float>& ell);
	void operator=(_In_ CRavidEllipse<float>* pEll);
	void operator=(_In_ const CRavidEllipse<double>& ell);
	void operator=(_In_ CRavidEllipse<double>* pEll);
	void operator=(_In_ const CRavidPolygon& rpg);
	void operator=(_In_ CRavidPolygon* pRpg);
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
	CRavidRect<double> operator+(_In_ const int& value);
	CRavidRect<double> operator+(_In_ const long long& value);
	CRavidRect<double> operator+(_In_ const float& value);
	CRavidRect<double> operator+(_In_ const double& value);
	CRavidRect<double> operator+(_In_ const POINT& point);
	CRavidRect<double> operator+(_In_ const SIZE& size);
	CRavidRect<double> operator+(_In_ const RECT& rect);
	CRavidRect<double> operator+(_In_ const CRavidPoint<int>& point);
	CRavidRect<double> operator+(_In_ const CRavidPoint<long long>& point);
	CRavidRect<double> operator+(_In_ const CRavidPoint<float>& point);
	CRavidRect<double> operator+(_In_ const CRavidPoint<double>& point);
	CRavidRect<double> operator+(_In_ const CRavidLine<int>& line);
	CRavidRect<double> operator+(_In_ const CRavidLine<long long>& line);
	CRavidRect<double> operator+(_In_ const CRavidLine<float>& line);
	CRavidRect<double> operator+(_In_ const CRavidLine<double>& line);
	CRavidRect<double> operator+(_In_ const CRavidRect<int>& rect);
	CRavidRect<double> operator+(_In_ const CRavidRect<long long>& rect);
	CRavidRect<double> operator+(_In_ const CRavidRect<float>& rect);
	CRavidRect<double> operator+(_In_ const CRavidRect<double>& rect);

	CRavidRect<double> operator-(_In_ const int& value);
	CRavidRect<double> operator-(_In_ const long long& value);
	CRavidRect<double> operator-(_In_ const float& value);
	CRavidRect<double> operator-(_In_ const double& value);
	CRavidRect<double> operator-(_In_ const POINT& point);
	CRavidRect<double> operator-(_In_ const SIZE& size);
	CRavidRect<double> operator-(_In_ const RECT& rect);
	CRavidRect<double> operator-(_In_ const CRavidPoint<int>& point);
	CRavidRect<double> operator-(_In_ const CRavidPoint<long long>& point);
	CRavidRect<double> operator-(_In_ const CRavidPoint<float>& point);
	CRavidRect<double> operator-(_In_ const CRavidPoint<double>& point);
	CRavidRect<double> operator-(_In_ const CRavidLine<int>& line);
	CRavidRect<double> operator-(_In_ const CRavidLine<long long>& line);
	CRavidRect<double> operator-(_In_ const CRavidLine<float>& line);
	CRavidRect<double> operator-(_In_ const CRavidLine<double>& line);
	CRavidRect<double> operator-(_In_ const CRavidRect<int>& rect);
	CRavidRect<double> operator-(_In_ const CRavidRect<long long>& rect);
	CRavidRect<double> operator-(_In_ const CRavidRect<float>& rect);
	CRavidRect<double> operator-(_In_ const CRavidRect<double>& rect);

	CRavidRect<double> operator*(_In_ const int& value);
	CRavidRect<double> operator*(_In_ const long long& value);
	CRavidRect<double> operator*(_In_ const float& value);
	CRavidRect<double> operator*(_In_ const double& value);
	CRavidRect<double> operator*(_In_ const POINT& point);
	CRavidRect<double> operator*(_In_ const SIZE& size);
	CRavidRect<double> operator*(_In_ const RECT& rect);
	CRavidRect<double> operator*(_In_ const CRavidPoint<int>& point);
	CRavidRect<double> operator*(_In_ const CRavidPoint<long long>& point);
	CRavidRect<double> operator*(_In_ const CRavidPoint<float>& point);
	CRavidRect<double> operator*(_In_ const CRavidPoint<double>& point);
	CRavidRect<double> operator*(_In_ const CRavidLine<int>& line);
	CRavidRect<double> operator*(_In_ const CRavidLine<long long>& line);
	CRavidRect<double> operator*(_In_ const CRavidLine<float>& line);
	CRavidRect<double> operator*(_In_ const CRavidLine<double>& line);
	CRavidRect<double> operator*(_In_ const CRavidRect<int>& rect);
	CRavidRect<double> operator*(_In_ const CRavidRect<long long>& rect);
	CRavidRect<double> operator*(_In_ const CRavidRect<float>& rect);
	CRavidRect<double> operator*(_In_ const CRavidRect<double>& rect);

	CRavidRect<double> operator/(_In_ const int& value);
	CRavidRect<double> operator/(_In_ const long long& value);
	CRavidRect<double> operator/(_In_ const float& value);
	CRavidRect<double> operator/(_In_ const double& value);
	CRavidRect<double> operator/(_In_ const POINT& point);
	CRavidRect<double> operator/(_In_ const SIZE& size);
	CRavidRect<double> operator/(_In_ const RECT& rect);
	CRavidRect<double> operator/(_In_ const CRavidPoint<int>& point);
	CRavidRect<double> operator/(_In_ const CRavidPoint<long long>& point);
	CRavidRect<double> operator/(_In_ const CRavidPoint<float>& point);
	CRavidRect<double> operator/(_In_ const CRavidPoint<double>& point);
	CRavidRect<double> operator/(_In_ const CRavidLine<int>& line);
	CRavidRect<double> operator/(_In_ const CRavidLine<long long>& line);
	CRavidRect<double> operator/(_In_ const CRavidLine<float>& line);
	CRavidRect<double> operator/(_In_ const CRavidLine<double>& line);
	CRavidRect<double> operator/(_In_ const CRavidRect<int>& rect);
	CRavidRect<double> operator/(_In_ const CRavidRect<long long>& rect);
	CRavidRect<double> operator/(_In_ const CRavidRect<float>& rect);
	CRavidRect<double> operator/(_In_ const CRavidRect<double>& rect);
#pragma endregion

#pragma region [Casting operator]
	/**
	* 현재 객체를 RECT 구조체의 포인터로 반환합니다.
	*
	* return value : 객체를 RECT 구조체로 케스팅합니다.
	*/
	operator LPRECT();
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
	* return value : 계산에 오류가 있으면 INF 를 반환합니다.
	*/
	virtual double GetArea() override;
#pragma endregion

#pragma region [GetWidth]
	/**
	* 현재 객체의 가로 길이를 반환합니다.
	*
	* return value : 설정된 값에 따라 - 값이 발생할 수 있습니다.
	*/
	T GetWidth();
#pragma endregion

#pragma region [GetHeight]
	/**
	* 현재 객체의 세로 길이를 반환합니다.
	*
	* return value : 설정된 값에 따라 - 값이 발생할 수 있습니다.
	*/
	T GetHeight();
#pragma endregion
	
#pragma region [GetAngle]
	/**
	* 현재 객체의 각도를 계산합니다.
	* CRavidRect 는 angle 을 가지고 있기 때문에 angle 값을 반환합니다.
	*
	* return value : 설정된 angle 값을 가져옵니다.
	*/
	virtual double GetAngle() override;
#pragma endregion

#pragma region [Normalize]
	/**
	* 현재 객체에 설정된 값을 확인하여 불안전한 값을 재 배치합니다.
	* Rect의 값은 다음과 같은 규칙을 따릅니다.
	* left : x 축의 최소값, right : x 축의 최대값
	* top : y 축의 최소값, bottom : y 축의 최대값
	*/
	void Normalize();
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
	bool GetIntersection(_In_ CRavidLine<int>& refGeometry, _Out_ CRavidGeometryArray* pGeometrayArray);
	/**
	* 현재 객체와 상대 직선과의 교점을 계산합니다.
	* 선분인 경우에는 범위 내만 계산합니다.
	*
	* param[in] pGometry : 상대 객체 (Pointer)
	* param[out] pGeometrayArray : 교점 좌표 리스트 (Pointer)
	*
	* return value : 교점이 없으면 false 로 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidLine<int>* pGometry, _Out_ CRavidGeometryArray* pGeometrayArray);
	/**
	* 현재 객체와 상대 직선과의 교점을 계산합니다.
	* 선분인 경우에는 범위 내만 계산합니다.
	*
	* param[in] refGeometry : 상대 객체 (Reference)
	* param[out] pGeometrayArray : 교점 좌표 리스트 (Pointer)
	*
	* return value : 교점이 없으면 false 로 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidLine<long long>& refGeometry, _Out_ CRavidGeometryArray* pGeometrayArray);
	/**
	* 현재 객체와 상대 직선과의 교점을 계산합니다.
	* 선분인 경우에는 범위 내만 계산합니다.
	*
	* param[in] pGometry : 상대 객체 (Pointer)
	* param[out] pGeometrayArray : 교점 좌표 리스트 (Pointer)
	*
	* return value : 교점이 없으면 false 로 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidLine<long long>* pGometry, _Out_ CRavidGeometryArray* pGeometrayArray);
	/**
	* 현재 객체와 상대 직선과의 교점을 계산합니다.
	* 선분인 경우에는 범위 내만 계산합니다.
	*
	* param[in] refGeometry : 상대 객체 (Reference)
	* param[out] pGeometrayArray : 교점 좌표 리스트 (Pointer)
	*
	* return value : 교점이 없으면 false 로 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidLine<float>& refGeometry, _Out_ CRavidGeometryArray* pGeometrayArray);
	/**
	* 현재 객체와 상대 직선과의 교점을 계산합니다.
	* 선분인 경우에는 범위 내만 계산합니다.
	*
	* param[in] pGometry : 상대 객체 (Pointer)
	* param[out] pGeometrayArray : 교점 좌표 리스트 (Pointer)
	*
	* return value : 교점이 없으면 false 로 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidLine<float>* pGometry, _Out_ CRavidGeometryArray* pGeometrayArray);
	/**
	* 현재 객체와 상대 직선과의 교점을 계산합니다.
	* 선분인 경우에는 범위 내만 계산합니다.
	*
	* param[in] refGeometry : 상대 객체 (Reference)
	* param[out] pGeometrayArray : 교점 좌표 리스트 (Pointer)
	*
	* return value : 교점이 없으면 false 로 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidLine<double>& refGeometry, _Out_ CRavidGeometryArray* pGeometrayArray);
	/**
	* 현재 객체와 상대 직선과의 교점을 계산합니다.
	* 선분인 경우에는 범위 내만 계산합니다.
	*
	* param[in] pGometry : 상대 객체 (Pointer)
	* param[out] pGeometrayArray : 교점 좌표 리스트 (Pointer)
	*
	* return value : 교점이 없으면 false 로 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidLine<double>* pGometry, _Out_ CRavidGeometryArray* pGeometrayArray);
#pragma endregion

public:
#pragma region [Data]
	/**
	* 객체를 이루는 최소 x 축 값입니다.
	* 템플릿 타입에 따라 타입이 설정됩니다.
	* 내부 알고리즘을 동작하기 위해서는 아래 데이터의 규칙을 지켜야합니다.
	* 
	* T left : x 축 최소값으로 right 보다 작거나 같아야 합니다.
	*/
	T left = 0;
	/**
	* 객체를 이루는 최소 y 축 값입니다.
	* 템플릿 타입에 따라 타입이 설정됩니다.
	* 내부 알고리즘을 동작하기 위해서는 아래 데이터의 규칙을 지켜야합니다.
	* 
	* T top : y 축 최소값으로 bottom 보다 작거나 같아야 합니다.
	*/
	T top = 0;
	/**
	* 객체를 이루는 최대 x 축 값입니다.
	* 템플릿 타입에 따라 타입이 설정됩니다.
	* 내부 알고리즘을 동작하기 위해서는 아래 데이터의 규칙을 지켜야합니다.
	* 
	* T right : x 축 최대값으로 left 보다 크나 같아야 합니다.
	*/
	T right = 0;
	/**
	* 객체를 이루는 최대 y 축 값입니다.
	* 템플릿 타입에 따라 타입이 설정됩니다.
	* 내부 알고리즘을 동작하기 위해서는 아래 데이터의 규칙을 지켜야합니다.
	* 
	* T bottom : y 축 최대값으로 top 보다 크나 같아야 합니다.
	*/
	T bottom = 0;
	/**
	* 객체의 회전 상태를 나타내는 값입니다.
	* 템플릿 타입과 무관하게 항상 double 로 성정됩니다.
	* 
	* double angle : 기본 0 도로 설정되며, -360 도에서 360 도를 표현하지만 그 이상도 가능합니다.
	*/
	double angle = 0.;
#pragma endregion
};