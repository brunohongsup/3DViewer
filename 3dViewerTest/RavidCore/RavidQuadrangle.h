#pragma once

#include "RavidGeometry.h"
#include "RavidPoint.h"

/**
* "사각형 (Quadrangle)" 을 표현하는 클래스입니다.
* 데이터 타입은 int, long long, float, double 만 가능합니다.
* CRavidGeometry 를 상속받은 클래스로 "사각형"과 관련된 알고리즘을 포함하고 있습니다.
*/
template <typename T>
class AFX_EXT_CLASS CRavidQuadrangle : public CRavidGeometry
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

#include "RavidQuadrangleCtor.hpp"

public:
	RavidUseDynamicCreation(this);

	static_assert(CheckIntegerAndRealNumber<T>::value, "CRavidQuadrangle template class is able to use type within int, long long, float and double.");

	CRavidQuadrangle();
	virtual ~CRavidQuadrangle();

#pragma region [Assignment operator]
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
#pragma endregion

#pragma region [Comparison operator]
	bool operator==(_In_ const CRavidQuadrangle<int>& quad);
	bool operator==(_In_ const CRavidQuadrangle<long long>& quad);
	bool operator==(_In_ const CRavidQuadrangle<float>& quad);
	bool operator==(_In_ const CRavidQuadrangle<double>& quad);

	bool operator!=(_In_ const CRavidQuadrangle<int>& quad);
	bool operator!=(_In_ const CRavidQuadrangle<long long>& quad);
	bool operator!=(_In_ const CRavidQuadrangle<float>& quad);
	bool operator!=(_In_ const CRavidQuadrangle<double>& quad);
#pragma endregion

#pragma region [Arithmetic and assignment operator]
	void operator+=(_In_ const CRavidLine<int>& line);
	void operator+=(_In_ const CRavidLine<long long>& line);
	void operator+=(_In_ const CRavidLine<float>& line);
	void operator+=(_In_ const CRavidLine<double>& line);
	void operator+=(_In_ const CRavidQuadrangle<int>& quad);
	void operator+=(_In_ const CRavidQuadrangle<long long>& quad);
	void operator+=(_In_ const CRavidQuadrangle<float>& quad);
	void operator+=(_In_ const CRavidQuadrangle<double>& quad);

	void operator-=(_In_ const CRavidLine<int>& line);
	void operator-=(_In_ const CRavidLine<long long>& line);
	void operator-=(_In_ const CRavidLine<float>& line);
	void operator-=(_In_ const CRavidLine<double>& line);
	void operator-=(_In_ const CRavidQuadrangle<int>& quad);
	void operator-=(_In_ const CRavidQuadrangle<long long>& quad);
	void operator-=(_In_ const CRavidQuadrangle<float>& quad);
	void operator-=(_In_ const CRavidQuadrangle<double>& quad);

	void operator*=(_In_ const CRavidLine<int>& line);
	void operator*=(_In_ const CRavidLine<long long>& line);
	void operator*=(_In_ const CRavidLine<float>& line);
	void operator*=(_In_ const CRavidLine<double>& line);
	void operator*=(_In_ const CRavidQuadrangle<int>& quad);
	void operator*=(_In_ const CRavidQuadrangle<long long>& quad);
	void operator*=(_In_ const CRavidQuadrangle<float>& quad);
	void operator*=(_In_ const CRavidQuadrangle<double>& quad);

	void operator/=(_In_ const CRavidLine<int>& line);
	void operator/=(_In_ const CRavidLine<long long>& line);
	void operator/=(_In_ const CRavidLine<float>& line);
	void operator/=(_In_ const CRavidLine<double>& line);
	void operator/=(_In_ const CRavidQuadrangle<int>& quad);
	void operator/=(_In_ const CRavidQuadrangle<long long>& quad);
	void operator/=(_In_ const CRavidQuadrangle<float>& quad);
	void operator/=(_In_ const CRavidQuadrangle<double>& quad);
#pragma endregion

#pragma region [Arithmetic operator]
	CRavidQuadrangle<double> operator+(_In_ const int& value);
	CRavidQuadrangle<double> operator+(_In_ const long long& value);
	CRavidQuadrangle<double> operator+(_In_ const float& value);
	CRavidQuadrangle<double> operator+(_In_ const double& value);
	CRavidQuadrangle<double> operator+(_In_ const POINT& point);
	CRavidQuadrangle<double> operator+(_In_ const SIZE& size);
	CRavidQuadrangle<double> operator+(_In_ const CRavidPoint<int>& point);
	CRavidQuadrangle<double> operator+(_In_ const CRavidPoint<long long>& point);
	CRavidQuadrangle<double> operator+(_In_ const CRavidPoint<float>& point);
	CRavidQuadrangle<double> operator+(_In_ const CRavidPoint<double>& point);
	CRavidQuadrangle<double> operator+(_In_ const CRavidLine<int>& line);
	CRavidQuadrangle<double> operator+(_In_ const CRavidLine<long long>& line);
	CRavidQuadrangle<double> operator+(_In_ const CRavidLine<float>& line);
	CRavidQuadrangle<double> operator+(_In_ const CRavidLine<double>& line);
	CRavidQuadrangle<double> operator+(_In_ const CRavidQuadrangle<int>& quad);
	CRavidQuadrangle<double> operator+(_In_ const CRavidQuadrangle<long long>& quad);
	CRavidQuadrangle<double> operator+(_In_ const CRavidQuadrangle<float>& quad);
	CRavidQuadrangle<double> operator+(_In_ const CRavidQuadrangle<double>& quad);

	CRavidQuadrangle<double> operator-(_In_ const int& value);
	CRavidQuadrangle<double> operator-(_In_ const long long& value);
	CRavidQuadrangle<double> operator-(_In_ const float& value);
	CRavidQuadrangle<double> operator-(_In_ const double& value);
	CRavidQuadrangle<double> operator-(_In_ const POINT& point);
	CRavidQuadrangle<double> operator-(_In_ const SIZE& size);
	CRavidQuadrangle<double> operator-(_In_ const CRavidPoint<int>& point);
	CRavidQuadrangle<double> operator-(_In_ const CRavidPoint<long long>& point);
	CRavidQuadrangle<double> operator-(_In_ const CRavidPoint<float>& point);
	CRavidQuadrangle<double> operator-(_In_ const CRavidPoint<double>& point);
	CRavidQuadrangle<double> operator-(_In_ const CRavidLine<int>& line);
	CRavidQuadrangle<double> operator-(_In_ const CRavidLine<long long>& line);
	CRavidQuadrangle<double> operator-(_In_ const CRavidLine<float>& line);
	CRavidQuadrangle<double> operator-(_In_ const CRavidLine<double>& line);
	CRavidQuadrangle<double> operator-(_In_ const CRavidQuadrangle<int>& quad);
	CRavidQuadrangle<double> operator-(_In_ const CRavidQuadrangle<long long>& quad);
	CRavidQuadrangle<double> operator-(_In_ const CRavidQuadrangle<float>& quad);
	CRavidQuadrangle<double> operator-(_In_ const CRavidQuadrangle<double>& quad);

	CRavidQuadrangle<double> operator*(_In_ const int& value);
	CRavidQuadrangle<double> operator*(_In_ const long long& value);
	CRavidQuadrangle<double> operator*(_In_ const float& value);
	CRavidQuadrangle<double> operator*(_In_ const double& value);
	CRavidQuadrangle<double> operator*(_In_ const POINT& point);
	CRavidQuadrangle<double> operator*(_In_ const SIZE& size);
	CRavidQuadrangle<double> operator*(_In_ const CRavidPoint<int>& point);
	CRavidQuadrangle<double> operator*(_In_ const CRavidPoint<long long>& point);
	CRavidQuadrangle<double> operator*(_In_ const CRavidPoint<float>& point);
	CRavidQuadrangle<double> operator*(_In_ const CRavidPoint<double>& point);
	CRavidQuadrangle<double> operator*(_In_ const CRavidLine<int>& line);
	CRavidQuadrangle<double> operator*(_In_ const CRavidLine<long long>& line);
	CRavidQuadrangle<double> operator*(_In_ const CRavidLine<float>& line);
	CRavidQuadrangle<double> operator*(_In_ const CRavidLine<double>& line);
	CRavidQuadrangle<double> operator*(_In_ const CRavidQuadrangle<int>& quad);
	CRavidQuadrangle<double> operator*(_In_ const CRavidQuadrangle<long long>& quad);
	CRavidQuadrangle<double> operator*(_In_ const CRavidQuadrangle<float>& quad);
	CRavidQuadrangle<double> operator*(_In_ const CRavidQuadrangle<double>& quad);

	CRavidQuadrangle<double> operator/(_In_ const int& value);
	CRavidQuadrangle<double> operator/(_In_ const long long& value);
	CRavidQuadrangle<double> operator/(_In_ const float& value);
	CRavidQuadrangle<double> operator/(_In_ const double& value);
	CRavidQuadrangle<double> operator/(_In_ const POINT& point);
	CRavidQuadrangle<double> operator/(_In_ const SIZE& size);
	CRavidQuadrangle<double> operator/(_In_ const CRavidPoint<int>& point);
	CRavidQuadrangle<double> operator/(_In_ const CRavidPoint<long long>& point);
	CRavidQuadrangle<double> operator/(_In_ const CRavidPoint<float>& point);
	CRavidQuadrangle<double> operator/(_In_ const CRavidPoint<double>& point);
	CRavidQuadrangle<double> operator/(_In_ const CRavidLine<int>& line);
	CRavidQuadrangle<double> operator/(_In_ const CRavidLine<long long>& line);
	CRavidQuadrangle<double> operator/(_In_ const CRavidLine<float>& line);
	CRavidQuadrangle<double> operator/(_In_ const CRavidLine<double>& line);
	CRavidQuadrangle<double> operator/(_In_ const CRavidQuadrangle<int>& quad);
	CRavidQuadrangle<double> operator/(_In_ const CRavidQuadrangle<long long>& quad);
	CRavidQuadrangle<double> operator/(_In_ const CRavidQuadrangle<float>& quad);
	CRavidQuadrangle<double> operator/(_In_ const CRavidQuadrangle<double>& quad);
#pragma endregion
	
#pragma region [GetRect]
	/**
	* 현재 객체를 CRavidRect<T> 형태로 계산 반환합니다.
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
	* 객체를 이루는 4개 점 x, y 좌표를 나타내고, CRavidPoint 4개의 배열로 정의됩니다.
	* 템플릿 타입에 따라 타입이 설정됩니다.
	* 내부 알고리즘을 동작하기 위해서는 일관된 좌표 방향성을 가져야 합니다.
	*
	* CRavidPoint<T> rpPoints[2] : CRavidPoint 클래스로 CRavidQuadrangle 클래스의 템플릿 타입으로 설정됩니다.
	*/
	CRavidPoint<T> rpPoints[4];
#pragma endregion
};

