public:
	
	/**
	* 현재 위치를 POINT 구조체로 가져옵니다.
	*
	* return value : POINT 구조체를 반환합니다.
	*/
	virtual operator POINT();
	/**
	* 현재 위치를 CPoint 클래스로 가져옵니다.
	*
	* return value : CPoint 클래스를 반환합니다.
	*/
	virtual operator CPoint();
	/**
	* 현재 위치를 RECT 구조체로 가져옵니다.
	*
	* return value : RECT 구조체를 반환합니다.
	*/
	virtual operator RECT();
	/**
	* 현재 위치를 CRect 클래스로 가져옵니다.
	*
	* return value : CRect 클래스를 반환합니다.
	*/
	virtual operator CRect();
	

#pragma region [GetRasterRegion]
	/**
	* (0, 0) 부터 입력된 크기 내부에 해당 객체의 면적을 Y축 1 단위당 X 의 시작, 끝 위치 리스트를 계산합니다.
	*
	* param[out] RGA : 해당 객체를 계산한 리스트 (Reference)
	* param[in_opt] pRpImageSize : 이미지크기 혹은 계산할 크기. (0,0) 부터 계산한다. (Pointer)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	virtual bool GetRasterRegion(_Out_ CRavidGeometryArray& RGA, _In_opt_ CRavidPoint<int>* pRpImageSize = nullptr);
	/**
	* (0, 0) 부터 입력된 크기 내부에 해당 객체의 면적을 Y축 1 단위당 X 의 시작, 끝 위치 리스트를 계산합니다.
	*
	* param[out] RGA : 해당 객체를 계산한 리스트 (Pointer)
	* param[in_opt] pRpImageSize : 계산할 크기 영역. (0,0) 부터 계산한다. (Pointer)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	virtual bool GetRasterRegion(_Out_ CRavidGeometryArray* pRGA, _In_opt_ CRavidPoint<int>* pRpImageSize = nullptr);
	/**
	* 입력된 크기 내부에 해당 객체의 면적을 Y축 1 단위당 X 의 시작, 끝 위치 리스트를 계산합니다.
	*
	* param[out] pRGA : 해당 객체를 계산한 리스트 (Reference)
	* param[in_opt] pRpImageSize : 계산할 영역 (Pointer)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	virtual bool GetRasterRegion(_Out_ CRavidGeometryArray& RGA, _In_opt_ CRavidRect<int>* pRrImageSize);
	/**
	* 입력된 크기 내부에 해당 객체의 면적을 Y축 1 단위당 X 의 시작, 끝 위치 리스트를 계산합니다.
	*
	* param[out] RGA : 해당 객체를 계산한 리스트 (Reference)
	* param[in_opt] pRpImageSize : 계산할 영역 (Pointer)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	virtual bool GetRasterRegion(_Out_ CRavidGeometryArray* pRGA, _In_opt_ CRavidRect<int>* pRrImageSize);
#pragma endregion
