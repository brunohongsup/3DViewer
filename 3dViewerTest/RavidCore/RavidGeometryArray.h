#pragma once

#include "RavidGeometry.h"

#include <vector>

/**
* "여러 도형의 리스트" 을 표현하는 클래스입니다.
* Point, Line, Rect, Quadrangle, Circle, Ellipse, Polygon 등 다중 객체를 표현합니다.
* CRavidGeometry 를 상속받은 클래스로 "도형의 공통 기능"과 관련된 알고리즘을 포함하고 있습니다.
*/
class AFX_EXT_CLASS CRavidGeometryArray : public CRavidGeometry
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

#include "RavidGeometryArrayCtor.hpp"

public:
	RavidUseDynamicCreation(this);

	CRavidGeometryArray();
	virtual ~CRavidGeometryArray();

	/**
	* 객체를 배열과 같이 접근합니다.
	*
	* param[in] nIndex : 저장된 인덱스
	* 
	* return value : 해당 인덱스의 객체를 가져옵니다.
	*/
	CRavidGeometry* operator [](_In_ size_t nIndex);
	
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

#pragma region [Copy]
	/**
	* 입력된 객체를 깊은 복사합니다.
	*
	* param[in] refGeometryArray : 복사 객체 (Reference)
	*
	* return value : 복사에 실패하면 false 를 반환합니다.
	*/
	virtual bool Copy(_In_ CRavidGeometryArray& refGeometryArray);
	/**
	* 입력된 객체를 깊은 복사합니다.
	*
	* param[in] pGeometryArray : 복사 객체 (Pointer)
	*
	* return value : 복사에 실패하면 false 를 반환합니다.
	*/
	virtual bool Copy(_In_ CRavidGeometryArray* pGeometryArray);
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

#pragma region [At]
	/**
	* 해당 인덱스의 객체를 접근합니다.
	*
	* param[in] nIndex : 접근 인덱스
	* 
	* return value : 해당 인덱스의 객체 Pointer 를 반환합니다.
	*/
	virtual CRavidGeometry* GetAt(_In_ int nIndex);
	/**
	* 해당 인덱스의 객체를 삭제합니다.
	*
	* param[in] nIndex : 삭제 인덱스
	*
	* return value : 삭제에 실패하면 false 를 반환합니다.
	*/
	virtual bool RemoveAt(_In_ int nIndex);
	/**
	* 현재 객체에 저장된 모든 객체의 갯수를 가져옵니다.
	*
	* return value : 객체의 개수가 반환됩니다.
	*/
	virtual long GetCount();
	/**
	* 현재 객체에 저장된 모든 객체를 제거합니다.
	*/
	virtual void Clear();
#pragma endregion
	
#pragma region [GetArea]
	/**
	* 현재 객체의 면적을 계산합니다.
	*
	* return value : 계산에 오류가 있으면 INF 를 반환합니다.
	*/
	virtual double GetArea();
#pragma endregion
	
#pragma region [GetAngle]
	/**
	* 각도를 출력합니다. 현재 객체는 회전 각을 알 수 없습니다.
	*
	* return value : 0 만 반환합니다.
	*/
	virtual double GetAngle();
#pragma endregion

#pragma region [Reserve]
	/**
	* 입력된 크기만큼 미리 할당합니다.
	*
	* param[in] szSize : 입력할 예상 객체의 수
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool Reserve(_In_ size_t szSize);
#pragma endregion

protected:
#pragma region [Data]
	/**
	* 객체를 이루는 객체들의 리스트를 vector 로 저장합니다.
	* Polygon 은 템플릿 타입이 아니기 때문에 입력된 상태 그대로를 입력합니다.
	* 내부 알고리즘을 동작하기 위해서는 겹쳐지거나 꼬이지 않아야 정확합니다.
	*
	* std::vector<CRavidGeometry*> m_vctArray : 겹쳐지거나 꼬이지 않은 상태의 객체 리스트
	*/
	std::vector<CRavidGeometry*> m_vctArray;
#pragma endregion};
};
