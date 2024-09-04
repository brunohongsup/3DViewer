#pragma once

#include "RavidGeometry.h"

/**
* "원 or Arc (Circle)" 을 표현하는 클래스입니다.
* 데이터 타입은 int, long long, float, double 만 가능합니다.
* CRavidGeometry 를 상속받은 클래스로 "원형"과 관련된 알고리즘을 포함하고 있습니다.
*/
template <typename T>
class AFX_EXT_CLASS CRavidCircle : public CRavidGeometry
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

#include "RavidCircleCtor.hpp"

public:
	RavidUseDynamicCreation(this);

	static_assert(CheckIntegerAndRealNumber<T>::value, "CRavidCircle template class is able to use type within int, long long, float and double.");

	CRavidCircle();
	virtual ~CRavidCircle();

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
	void operator=(_In_ const CRavidCircle<int>& cir);
	void operator=(_In_ CRavidCircle<int>* pCir);
	void operator=(_In_ const CRavidCircle<long long>& cir);
	void operator=(_In_ CRavidCircle<long long>* pCir);
	void operator=(_In_ const CRavidCircle<float>& cir);
	void operator=(_In_ CRavidCircle<float>* pCir);
	void operator=(_In_ const CRavidCircle<double>& cir);
	void operator=(_In_ CRavidCircle<double>* pCir);
#pragma endregion

#pragma region [Comparison operator]
	bool operator==(_In_ const CRavidCircle<int>& cir);
	bool operator==(_In_ const CRavidCircle<long long>& cir);
	bool operator==(_In_ const CRavidCircle<float>& cir);
	bool operator==(_In_ const CRavidCircle<double>& cir);

	bool operator!=(_In_ const CRavidCircle<int>& cir);
	bool operator!=(_In_ const CRavidCircle<long long>& cir);
	bool operator!=(_In_ const CRavidCircle<float>& cir);
	bool operator!=(_In_ const CRavidCircle<double>& cir);
#pragma endregion
	
#pragma region [CopyArithmetic and assignment operator]
	void operator+=(_In_ const CRavidLine<int>& line);
	void operator+=(_In_ const CRavidLine<long long>& line);
	void operator+=(_In_ const CRavidLine<float>& line);
	void operator+=(_In_ const CRavidLine<double>& line);

	void operator-=(_In_ const CRavidLine<int>& line);
	void operator-=(_In_ const CRavidLine<long long>& line);
	void operator-=(_In_ const CRavidLine<float>& line);
	void operator-=(_In_ const CRavidLine<double>& line);

	void operator*=(_In_ const CRavidLine<int>& line);
	void operator*=(_In_ const CRavidLine<long long>& line);
	void operator*=(_In_ const CRavidLine<float>& line);
	void operator*=(_In_ const CRavidLine<double>& line);

	void operator/=(_In_ const CRavidLine<int>& line);
	void operator/=(_In_ const CRavidLine<long long>& line);
	void operator/=(_In_ const CRavidLine<float>& line);
	void operator/=(_In_ const CRavidLine<double>& line);
#pragma endregion
	
#pragma region [Arithmetic operator]
	CRavidCircle<double> operator+(_In_ const int& value);
	CRavidCircle<double> operator+(_In_ const long long& value);
	CRavidCircle<double> operator+(_In_ const float& value);
	CRavidCircle<double> operator+(_In_ const double& value);
	CRavidCircle<double> operator+(_In_ const POINT& point);
	CRavidCircle<double> operator+(_In_ const SIZE& size);
	CRavidCircle<double> operator+(_In_ const CRavidPoint<int>& point);
	CRavidCircle<double> operator+(_In_ const CRavidPoint<long long>& point);
	CRavidCircle<double> operator+(_In_ const CRavidPoint<float>& point);
	CRavidCircle<double> operator+(_In_ const CRavidPoint<double>& point);
	CRavidCircle<double> operator+(_In_ const CRavidLine<int>& line);
	CRavidCircle<double> operator+(_In_ const CRavidLine<long long>& line);
	CRavidCircle<double> operator+(_In_ const CRavidLine<float>& line);
	CRavidCircle<double> operator+(_In_ const CRavidLine<double>& line);

	CRavidCircle<double> operator-(_In_ const int& value);
	CRavidCircle<double> operator-(_In_ const long long& value);
	CRavidCircle<double> operator-(_In_ const float& value);
	CRavidCircle<double> operator-(_In_ const double& value);
	CRavidCircle<double> operator-(_In_ const POINT& point);
	CRavidCircle<double> operator-(_In_ const SIZE& size);
	CRavidCircle<double> operator-(_In_ const CRavidPoint<int>& point);
	CRavidCircle<double> operator-(_In_ const CRavidPoint<long long>& point);
	CRavidCircle<double> operator-(_In_ const CRavidPoint<float>& point);
	CRavidCircle<double> operator-(_In_ const CRavidPoint<double>& point);
	CRavidCircle<double> operator-(_In_ const CRavidLine<int>& line);
	CRavidCircle<double> operator-(_In_ const CRavidLine<long long>& line);
	CRavidCircle<double> operator-(_In_ const CRavidLine<float>& line);
	CRavidCircle<double> operator-(_In_ const CRavidLine<double>& line);

	CRavidCircle<double> operator*(_In_ const int& value);
	CRavidCircle<double> operator*(_In_ const long long& value);
	CRavidCircle<double> operator*(_In_ const float& value);
	CRavidCircle<double> operator*(_In_ const double& value);
	CRavidCircle<double> operator*(_In_ const POINT& point);
	CRavidCircle<double> operator*(_In_ const SIZE& size);
	CRavidCircle<double> operator*(_In_ const CRavidPoint<int>& point);
	CRavidCircle<double> operator*(_In_ const CRavidPoint<long long>& point);
	CRavidCircle<double> operator*(_In_ const CRavidPoint<float>& point);
	CRavidCircle<double> operator*(_In_ const CRavidPoint<double>& point);
	CRavidCircle<double> operator*(_In_ const CRavidLine<int>& line);
	CRavidCircle<double> operator*(_In_ const CRavidLine<long long>& line);
	CRavidCircle<double> operator*(_In_ const CRavidLine<float>& line);
	CRavidCircle<double> operator*(_In_ const CRavidLine<double>& line);

	CRavidCircle<double> operator/(_In_ const int& value);
	CRavidCircle<double> operator/(_In_ const long long& value);
	CRavidCircle<double> operator/(_In_ const float& value);
	CRavidCircle<double> operator/(_In_ const double& value);
	CRavidCircle<double> operator/(_In_ const POINT& point);
	CRavidCircle<double> operator/(_In_ const SIZE& size);
	CRavidCircle<double> operator/(_In_ const CRavidPoint<int>& point);
	CRavidCircle<double> operator/(_In_ const CRavidPoint<long long>& point);
	CRavidCircle<double> operator/(_In_ const CRavidPoint<float>& point);
	CRavidCircle<double> operator/(_In_ const CRavidPoint<double>& point);
	CRavidCircle<double> operator/(_In_ const CRavidLine<int>& line);
	CRavidCircle<double> operator/(_In_ const CRavidLine<long long>& line);
	CRavidCircle<double> operator/(_In_ const CRavidLine<float>& line);
	CRavidCircle<double> operator/(_In_ const CRavidLine<double>& line);
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

#pragma region [GetMinimumEnclosingRectangle]
	/**
	* 현재 객체의 최소면적 사각형을 계산합니다.
	*
	* return value : 4 좌표의 사각형으로 반환합니다.
	*/
	virtual CRavidQuadrangle<double> GetMinimumEnclosingRectangle();
#pragma endregion

#pragma region [GetRadius]
	/**
	* 현재 객체의 반지름을 반환합니다.
	*
	* param[out] refRadius : 반지름 값 반환 (Reference)
	*
	* return value : 계산에 실패하면 false 로 반환합니다.
	*/
	virtual bool GetRadius(_Out_ int& refRadius);
	/**
	* 현재 객체의 반지름을 반환합니다.
	*
	* param[out] pRadius : 반지름 값 반환 (Pointer)
	*
	* return value : 계산에 실패하면 false 로 반환합니다.
	*/
	virtual bool GetRadius(_Out_ int* pRadius);
	/**
	* 현재 객체의 반지름을 반환합니다.
	*
	* param[out] refRadius : 반지름 값 반환 (Reference)
	*
	* return value : 계산에 실패하면 false 로 반환합니다.
	*/
	virtual bool GetRadius(_Out_ long long& refRadius);
	/**
	* 현재 객체의 반지름을 반환합니다.
	*
	* param[out] pRadius : 반지름 값 반환 (Pointer)
	*
	* return value : 계산에 실패하면 false 로 반환합니다.
	*/
	virtual bool GetRadius(_Out_ long long* pRadius);
	/**
	virtual bool GetRadius(_Out_ long long* pRadius);
	/**
	* 현재 객체의 반지름을 반환합니다.
	*
	* param[out] refRadius : 반지름 값 반환 (Reference)
	*
	* return value : 계산에 실패하면 false 로 반환합니다.
	*/
	virtual bool GetRadius(_Out_ float& refRadius);
	/**
	* 현재 객체의 반지름을 반환합니다.
	*
	* param[out] pRadius : 반지름 값 반환 (Pointer)
	*
	* return value : 계산에 실패하면 false 로 반환합니다.
	*/
	virtual bool GetRadius(_Out_ float* pRadius);
	/**
	virtual bool GetRadius(_Out_ float* pRadius);
	/**
	* 현재 객체의 반지름을 반환합니다.
	*
	* param[out] refRadius : 반지름 값 반환 (Reference)
	*
	* return value : 계산에 실패하면 false 로 반환합니다.
	*/
	virtual bool GetRadius(_Out_ double& refRadius);
	/**
	* 현재 객체의 반지름을 반환합니다.
	*
	* param[out] pRadius : 반지름 값 반환 (Pointer)
	*
	* return value : 계산에 실패하면 false 로 반환합니다.
	*/
	virtual bool GetRadius(_Out_ double* pRadius);
#pragma endregion

#pragma region [GetDiameter]
	/**
	* 현재 객체의 지름을 반환합니다.
	*
	* param[out] refDiameter : 지름 값 반환 (Reference)
	*
	* return value : 계산에 실패하면 false 로 반환합니다.
	*/
	virtual bool GetDiameter(_Out_ int& refDiameter);
	/**
	* 현재 객체의 지름을 반환합니다.
	*
	* param[out] pDiameter : 지름 값 반환 (Pointer)
	*
	* return value : 계산에 실패하면 false 로 반환합니다.
	*/
	virtual bool GetDiameter(_Out_ int* pDiameter);
	/**
	* 현재 객체의 지름을 반환합니다.
	*
	* param[out] refDiameter : 지름 값 반환 (Reference)
	*
	* return value : 계산에 실패하면 false 로 반환합니다.
	*/
	virtual bool GetDiameter(_Out_ long long& refDiameter);
	/**
	* 현재 객체의 지름을 반환합니다.
	*
	* param[out] pDiameter : 지름 값 반환 (Pointer)
	*
	* return value : 계산에 실패하면 false 로 반환합니다.
	*/
	virtual bool GetDiameter(_Out_ long long* pDiameter);
	/**
	* 현재 객체의 지름을 반환합니다.
	*
	* param[out] refDiameter : 지름 값 반환 (Reference)
	*
	* return value : 계산에 실패하면 false 로 반환합니다.
	*/
	virtual bool GetDiameter(_Out_ float& refDiameter);
	/**
	* 현재 객체의 지름을 반환합니다.
	*
	* param[out] pDiameter : 지름 값 반환 (Pointer)
	*
	* return value : 계산에 실패하면 false 로 반환합니다.
	*/
	virtual bool GetDiameter(_Out_ float* pDiameter);
	/**
	* 현재 객체의 지름을 반환합니다.
	*
	* param[out] refDiameter : 지름 값 반환 (Reference)
	*
	* return value : 계산에 실패하면 false 로 반환합니다.
	*/
	virtual bool GetDiameter(_Out_ double& refDiameter);
	/**
	* 현재 객체의 지름을 반환합니다.
	*
	* param[out] pDiameter : 지름 값 반환 (Pointer)
	*
	* return value : 계산에 실패하면 false 로 반환합니다.
	*/
	virtual bool GetDiameter(_Out_ double* pDiameter);
#pragma endregion
	
#pragma region [GetCircumference]
	/**
	* 현재 객체 둘레를 계산합니다.
	*
	* return value : 둘레만 계산합니다. 부채꼴 형태와 같이 끝점과 중심의 거리가 있는 정보는 포함하지 않습니다.
	*/
	double GetCircumference();
#pragma endregion
	
#pragma region [Arc]
	/**
	* 현재 객체의 형태가 부채꼴, 반원과 같은 Arc 의 형태인지 반환합니다.
	*
	* return value : 데이터가 원(360도)인 경우에만 false 를 반환합니다.
	*/
	bool IsArc();	
	/**
	* 형재 객체의 경우 양 끝점, 중심을 어떻게 연결할 것인지 설정합니다.
	* 원의 경우 양 끝점이 연결된 형태로 설정됩니다.
	*
	* param[in] eClosingMethod : ERavidArcClosingMethod 로 정의된 enum 값
	*							 ERavidArcClosingMethod_NoClose : 양 끝점이 연결되지 않는 형태
	*							 ERavidArcClosingMethod_EachOther : 양 끝점이 연결된 형태
	*							 ERavidArcClosingMethod_Center : 양 끝점과 중심이 연결된 형태
	*
	* return value : 설정에 실패하면 false 를 반환합니다.
	*/
	bool SetArcClosingMethod(_In_ ERavidArcClosingMethod eClosingMethod);
	/**
	* 설정된 점의 연결 방법을 가져옵니다.
	* 원의 경우 ERavidArcClosingMethod_EachOther 의 값만 반환합니다.
	*
	* return value : ERavidArcClosingMethod 로 정의된 enum 값으로 반환합니다.
	*				 ERavidArcClosingMethod_NoClose : 양 끝점이 연결되지 않는 형태
	*				 ERavidArcClosingMethod_EachOther : 양 끝점이 연결된 형태
	*				 ERavidArcClosingMethod_Center : 양 끝점과 중심이 연결된 형태
	*/
	ERavidArcClosingMethod GetArcClosingMethod();
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

#pragma region [ConvertPointToDegree]
	/**
	* 현재 객체에서 입력된 좌표를 각도로 계산합니다.
	*
	* param[in] refGeometry : CRavidPoint 위치 좌표 (Reference)
	*
	* return value : 계산된 각도를 반환합니다.
	*/
	double ConvertPointToDegree(_In_ CRavidPoint<int>& refGeometry);
	/**
	* 현재 객체에서 입력된 좌표를 각도로 계산합니다.
	*
	* param[in] pGeometry : CRavidPoint 위치 좌표 (Pointer)
	*
	* return value : 계산된 각도를 반환합니다.
	*/
	double ConvertPointToDegree(_In_ CRavidPoint<int>* pGeometry);
	/**
	* 현재 객체에서 입력된 좌표를 각도로 계산합니다.
	*
	* param[in] refGeometry : CRavidPoint 위치 좌표 (Reference)
	*
	* return value : 계산된 각도를 반환합니다.
	*/
	double ConvertPointToDegree(_In_ CRavidPoint<long long>& refGeometry);
	/**
	* 현재 객체에서 입력된 좌표를 각도로 계산합니다.
	*
	* param[in] pGeometry : CRavidPoint 위치 좌표 (Pointer)
	*
	* return value : 계산된 각도를 반환합니다.
	*/
	double ConvertPointToDegree(_In_ CRavidPoint<long long>* pGeometry);
	/**
	* 현재 객체에서 입력된 좌표를 각도로 계산합니다.
	*
	* param[in] refGeometry : CRavidPoint 위치 좌표 (Reference)
	*
	* return value : 계산된 각도를 반환합니다.
	*/
	double ConvertPointToDegree(_In_ CRavidPoint<float>& refGeometry);
	/**
	* 현재 객체에서 입력된 좌표를 각도로 계산합니다.
	*
	* param[in] pGeometry : CRavidPoint 위치 좌표 (Pointer)
	*
	* return value : 계산된 각도를 반환합니다.
	*/
	double ConvertPointToDegree(_In_ CRavidPoint<float>* pGeometry);
	/**
	* 현재 객체에서 입력된 좌표를 각도로 계산합니다.
	*
	* param[in] refGeometry : CRavidPoint 위치 좌표 (Reference)
	*
	* return value : 계산된 각도를 반환합니다.
	*/
	double ConvertPointToDegree(_In_ CRavidPoint<double>& refGeometry);
	/**
	* 현재 객체에서 입력된 좌표를 각도로 계산합니다.
	*
	* param[in] pGeometry : CRavidPoint 위치 좌표 (Pointer)
	*
	* return value : 계산된 각도를 반환합니다.
	*/
	double ConvertPointToDegree(_In_ CRavidPoint<double>* pGeometry);
#pragma endregion

#pragma region [ConvertDegreeToPoint]
	/**
	* 현재 객체에서 입력된 각도를 좌표로 계산합니다.
	*
	* param[in] deg : 각도 값
	*
	* return value : 계산된 좌표를 반환합니다.
	*/
	CRavidPoint<double> ConvertDegreeToPoint(_In_ int deg);
	/**
	* 현재 객체에서 입력된 각도를 좌표로 계산합니다.
	*
	* param[in] deg : 각도 값
	*
	* return value : 계산된 좌표를 반환합니다.
	*/
	CRavidPoint<double> ConvertDegreeToPoint(_In_ long long deg);
	/**
	* 현재 객체에서 입력된 각도를 좌표로 계산합니다.
	*
	* param[in] deg : 각도 값
	*
	* return value : 계산된 좌표를 반환합니다.
	*/
	CRavidPoint<double> ConvertDegreeToPoint(_In_ float deg);
	/**
	* 현재 객체에서 입력된 각도를 좌표로 계산합니다.
	*
	* param[in] deg : 각도 값
	*
	* return value : 계산된 좌표를 반환합니다.
	*/
	CRavidPoint<double> ConvertDegreeToPoint(_In_ double deg);
#pragma endregion

#pragma region [ConvertDegreeToPoint]
	/**
	* 현재 객체의 4방향 극좌표를 계산합니다.
	*
	* param[out] pGeometrayArray : 4 극점 좌표 리스트 (Pointer)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	bool GetOuterMostPoints(_Out_ CRavidGeometryArray* pGeometryArray);
#pragma endregion

public:
#pragma region [Data]
	/**
	* 객체를 이루는 중심 점 x, y 좌표를 저장합니다.
	* 템플릿 타입에 따라 타입이 설정됩니다.
	*
	* CRavidPoint<T> rpCenter : CRavidPoint 클래스로 CRavidCircle 클래스의 템플릿 타입으로 설정됩니다.
	*/
	CRavidPoint<T> rpCenter;
	/**
	* 객체를 이루는 반지름 값입니다.
	* 템플릿 타입에 따라 타입이 설정됩니다.
	* 내부 알고리즘을 동작하기 위해서는 아래 규칙을 지켜야합니다.
	*
	* T radius : 0 보다 큰 양수입니다.
	*/
	T radius;
	/**
	* 객체를 이루는 회전 값입니다.
	* 타입은 double 로 고정됩니다.
	*
	* double angle : 회전 각도
	*/
	double angle;
	/**
	* 객체의 시작 위치 각도 값입니다.
	* 타입은 double 로 고정됩니다. 
	*
	* double startDeg : 객체의 시작 각도
	*/
	double startDeg;
	/**
	* 객체의 시작 위치로부터 표현되는 각도 값입니다. 360을 넘어가면 원으로 판단합니다.
	* 타입은 double 로 고정됩니다. 
	*
	* double deltaDeg : 객체의 표현 각도
	*/
	double deltaDeg;

protected:
	/**
	* 객체의 끝점, 중심의 처리 형태 입니다.
	* 원의 경우 ERavidArcClosingMethod_EachOther 의 값만 반환합니다.
	*
	* ERavidArcClosingMethod eArcClosingMethod : 객체의 처리 형태
	* 						 ERavidArcClosingMethod_NoClose : 양 끝점이 연결되지 않는 형태
	* 						 ERavidArcClosingMethod_EachOther : 양 끝점이 연결된 형태
	* 						 ERavidArcClosingMethod_Center : 양 끝점과 중심이 연결된 형태
	*/
	ERavidArcClosingMethod eArcClosingMethod;
#pragma endregion
};

