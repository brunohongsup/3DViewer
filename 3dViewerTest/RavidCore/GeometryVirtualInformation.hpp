public:

#pragma region [IsValid]
	/**
	* 현재 객체의 데이터 값이 유효한지 확인합니다.
	*
	* return value : 값의 범위를 벗어나거나 유효하지 않은 무한대의 값일 경우에 false 를 반환합니다.
	*/
	virtual bool __fastcall IsValid() override;
#pragma endregion

#pragma region [IsShapeValid]
	/**
	* 현재 객체의 형태가 유효한지 확인합니다.
	*
	* return value : 형태가 겹쳐지거나 값의 크기가 맞지 않을 경우에 false 를 반환합니다.
	*/
	virtual bool IsShapeValid();
#pragma endregion

#pragma region [GetShapeType]
	/**
	* 현재 객체의 형태를 반환합니다.
	*
	* return value : ERavidGeometryShapeType enum 으로 반환됩니다.
	* ERavidGeometryShapeType_Unknown : 알 수 없는 객체
	* ERavidGeometryShapeType_Point : 점 객체
	* ERavidGeometryShapeType_Line : 직선 또는 선분 객체
	* ERavidGeometryShapeType_Rectangle : 직사각형 객체
	* ERavidGeometryShapeType_Quadrangle : 사각형 객체
	* ERavidGeometryShapeType_Circle : 원 객체
	* ERavidGeometryShapeType_Ellipse : 타원 객체
	* ERavidGeometryShapeType_Polygon : 폴리곤 객체
	* ERavidGeometryShapeType_Array : 객체 리스트 형태
	*/
	virtual ERavidGeometryShapeType GetShapeType();
#pragma endregion

#pragma region [GetTemplateType]
	/**
	* 현재 객체의 데이터 타입을 반환합니다.
	*
	* return value : ERavidGeometryTemplateType enum 으로 반환되며, 템플릿으로 구현된 객체의 타입을 반환합니다.
	* RavidPolygon 또는 RavidGeometryArray 는 ERavidGeometryTemplateType_Unknown 을 반환합니다.
	* ERavidGeometryTemplateType_Unknown : 알 수 없는 데이터 타입
	* ERavidGeometryTemplateType_Int : 정수형 32bit 데이터 타입
	* ERavidGeometryTemplateType_LongLong : 정수현 64 bit 데이터 타입
	* ERavidGeometryTemplateType_Float : 실수형 32 bit 데이터 타입
	* ERavidGeometryTemplateType_Double : 실수형 64 bit 데이터 타입
	*/
	virtual ERavidGeometryTemplateType GetTemplateType();
#pragma endregion

#pragma region [MakeDifferentTemplateType]
	/**
	* 현재 객체의 데이터 타입을 입력된 형태로 변환하여 동적 할당된 객체를 반환합니다.
	* 이 함수를 사용할 경우 개발자는 직접 메모리를 해제해야 합니다.
	*
	* return value : 변환된 객체의 메모리를 반환합니다. 실패한 경우 nullptr 을 반환합니다.
	*/
	virtual CRavidGeometry* MakeDifferentTemplateType(ERavidGeometryTemplateType eType);
#pragma endregion

