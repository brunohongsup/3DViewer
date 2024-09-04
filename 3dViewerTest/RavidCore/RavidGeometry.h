#pragma once

class CRavidGeometryArray;

namespace Ravid
{
	namespace Algorithms
	{
		class CRavidImage;
	}
}

/**
* "도형" 을 표현하는 기저클래스입니다.
* 도형의 공통된 기능은 바로 호출하여 사용 가능합니다.
*/
class AFX_EXT_CLASS CRavidGeometry : public CRavidObject
{
#include "GeometryVirtualInformation.hpp"

#include "GeometryVirtualOffset.hpp"
#include "GeometryVirtualRect.hpp"
#include "GeometryVirtualCenter.hpp"
#include "GeometryVirtualDeflate.hpp"
#include "GeometryVirtualInflate.hpp"
#include "GeometryVirtualOperator.hpp"
#include "GeometryVirtualRotate.hpp"
#include "GeometryVirtualShape.hpp"

	RavidUseDynamicCreation();

public:
	CRavidGeometry();
	virtual ~CRavidGeometry();
	
#pragma region [ExclusiveRegion]
	/**
	* 면적을 가진 객체에 내부에 뚫린 객체를 설정합니다.
	* 면적을 가지지 않은 객체는 사용할 수 없습니다.
	* You must have a surface to use this feature.
	*
	* param[in] pExclusiveRegion : 설정 할 객체 리스트 (Pointer)
	*)
	* return value : 설정에 실패하면 false 를 반환합니다.
	*/
	virtual bool SetExclusiveRegion(_In_ CRavidGeometryArray* pExclusiveRegion);
	/**
	* 면적을 가진 객체에 내부에 뚫린 객체를 설정합니다.
	* 면적을 가지지 않은 객체는 사용할 수 없습니다.
	* You must have a surface to use this feature.
	*
	* param[in] refExclusiveRegion : 설정 할 객체 리스트 (Reference)
	*)
	* return value : 설정에 실패하면 false 를 반환합니다.
	*/
	virtual bool SetExclusiveRegion(_In_ CRavidGeometryArray& refExclusiveRegion);
	/**
	* 면적을 가진 객체에 내부에 뚫린 객체를 추가합니다.
	* 면적을 가지지 않은 객체는 사용할 수 없습니다.
	* You must have a surface to use this feature.
	*
	* param[in] pGeometry : 추가 할 객체 리스트 (Pointer)
	*
	* return value : 추가에 실패하면 false 를 반환합니다.
	*/
	virtual bool AddExclusiveRegion(_In_ CRavidGeometry* pGeometry);
	/**
	* 면적을 가진 객체에 내부에 뚫린 객체를 추가합니다.
	* 면적을 가지지 않은 객체는 사용할 수 없습니다.
	* You must have a surface to use this feature.
	*
	* param[in] refGeometry : 추가 할 객체 리스트 (Reference)
	*
	* return value : 추가에 실패하면 false 를 반환합니다.
	*/
	virtual bool AddExclusiveRegion(_In_ CRavidGeometry& refGeometry);
	/**
	* 현재 객체에 설정된 뚫린 객체 리스트를 반환합니다.
	*
	* return value : 객체가 없을 경우 nullptr 을 반환합니다.
	*/
	virtual CRavidGeometryArray* GetExclusiveRegion() const;
	/**
	* 현재 객체에 설정된 뚫린 객체 리스트를 모두 삭제합니다.
	*
	*/
	virtual void ClearExclusiveRegion();
	/**
	* 현재 객체에 설정된 뚫린 객체가 있는지 확인합니다.
	*
	* return value : 없으면 false 를 반환합니다.
	*/
	virtual bool IsThereExclusiveRegion() const;
#pragma endregion

	/**
	* 현재 객체의 중심을 POINT 구조체로 반환합니다.
	*
	* return value : 중심을 구 할 수 없을 경우 INF 로 반환합니다.
	*/
	virtual POINT GetPointStruct();

	/**
	* 현재 객체를 감싸는 최소 면적 정보를 RECT 구조체로 반환합니다.
	*
	* return value : 면적 정보를 구 할 수 없을 경우 INF 로 반환합니다.
	*/
	virtual RECT GetRectStruct();

	/**
	* 현재 객체의 면적을 계산합니다.
	*
	* return value : 면적의 크기를 반환합니다. 면적이 없을 경우 (0, 0) 으로 반환합니다.
	*/
	virtual double GetArea();
	
	/**
	* 현재 객체의 각도를 계산합니다.
	*
	* return value : 각도가 없을 경우 0 으로 반환합니다.
	*/
	virtual double GetAngle();
	
	/**
	* 현재 객체와 상대 객체와의 공통 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pRgShape : 계산할 상대 객체 (Pointer)
	* param[out] pRgResultGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	virtual bool GetIntersectionRegion(_In_ CRavidGeometry* pRgShape, _Out_ CRavidGeometry* pRgResultGeometryArray);
	
	/**
	* 현재 객체와 상대 객체와의 합 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pRgShape : 계산할 상대 객체 (Pointer)
	* param[out] pRgResultGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	virtual bool GetUnionRegion(_In_ CRavidGeometry* pRgShape, _Out_ CRavidGeometry* pRgResultGeometryArray);
	
	/**
	* 현재 객체와 상대 객체와의 차 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pRgShape : 계산할 상대 객체 (Pointer)
	* param[out] pRgResultGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	virtual bool GetSubtractionRegion(_In_ CRavidGeometry* pRgShape, _Out_ CRavidGeometry* pRgResultGeometryArray);
	
	/**
	* 현재 객체와 상대 객체의 배타적 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pRgShape : 계산할 상대 객체 (Pointer)
	* param[out] pRgResultGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	virtual bool GetExclusiveOrRegion(_In_ CRavidGeometry* pRgShape, _Out_ CRavidGeometry* pRgResultGeometryArray);
		
	/**
	* 현재 객체와 상대 객체와의 충돌 여부를 판단합니다.
	*
	* param[in] pRgShape : 계산할 상대 객체 (Pointer)
	*
	* return value : 충돌하지 않을 경우 false 를 반환합니다.
	*/
	virtual bool DoesIntersect(_In_ CRavidGeometry* pRgShape);
		
	/**
	* 현재 객체와 상대 객체와의 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] pRgShape : 계산할 상대 객체 (Pointer)
	* param[in] pRgResultGeometryArray : 계산 결과 충돌 좌표 (Pointer)
	*
	* return value : 충돌하지 않을 경우 false 를 반환합니다.
	*/
	virtual bool GetIntersection(_In_ CRavidGeometry* pRgShape, _Out_ CRavidGeometry* pRgResultGeometryArray);
	
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단합니다.
	*
	* param[in] pRgShape : 계산할 상대 객체 (Pointer)
	*
	* return value : 충돌하지 않을 경우 false 를 반환합니다.
	*/
	virtual bool DoesIgnoreInternalIntersection(_In_ CRavidGeometry* pRgShape);
		
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] pRgShape : 계산할 상대 객체 (Pointer)
	* param[in] pRgResultGeometryArray : 계산 결과 충돌 좌표 (Pointer)
	*
	* return value : 충돌하지 않을 경우 false 를 반환합니다.
	*/
	virtual bool GetIgnoreInternalIntersection(_In_ CRavidGeometry* pRgShape, _Out_ CRavidGeometry* pRgResultGeometryArray);
	
protected:
	CRavidGeometryArray* m_pExclusiveRegion = nullptr;
};
