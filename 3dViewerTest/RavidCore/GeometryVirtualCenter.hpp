public:
	
#pragma region [GetCenter]
	/**
	* 현재 객체의 중심을 계산하여 인자값으로 반환합니다.
	* 
	* param[in] x : 중심 x 좌표 (Reference)
	* param[in] y : 중심 y 좌표 (Reference)
	*
	* return value : 계산 실패시 false 를 반환합니다.
	*/
	virtual bool GetCenter(_Out_ int& x, _Out_ int& y);
	/**
	* 현재 객체의 중심을 계산하여 인자값으로 반환합니다.
	*
	* param[in] x : 중심 x 좌표 (Pointer)
	* param[in] y : 중심 y 좌표 (Pointer)
	*
	* return value : 계산 실패시 false 를 반환합니다.
	*/
	virtual bool GetCenter(_Out_ int* x, _Out_ int* y);
	/**
	* 현재 객체의 중심을 계산하여 인자값으로 반환합니다.
	* 
	* param[in] x : 중심 x 좌표 (Reference)
	* param[in] y : 중심 y 좌표 (Reference)
	*
	* return value : 계산 실패시 false 를 반환합니다.
	*/
	virtual bool GetCenter(_Out_ long long& x, _Out_ long long& y);
	/**
	* 현재 객체의 중심을 계산하여 인자값으로 반환합니다.
	*
	* param[in] x : 중심 x 좌표 (Pointer)
	* param[in] y : 중심 y 좌표 (Pointer)
	*
	* return value : 계산 실패시 false 를 반환합니다.
	*/
	virtual bool GetCenter(_Out_ long long* x, _Out_ long long* y);
	/**
	* 현재 객체의 중심을 계산하여 인자값으로 반환합니다.
	* 
	* param[in] x : 중심 x 좌표 (Reference)
	* param[in] y : 중심 y 좌표 (Reference)
	*
	* return value : 계산 실패시 false 를 반환합니다.
	*/
	virtual bool GetCenter(_Out_ float& x, _Out_ float& y);
	/**
	* 현재 객체의 중심을 계산하여 인자값으로 반환합니다.
	*
	* param[in] x : 중심 x 좌표 (Pointer)
	* param[in] y : 중심 y 좌표 (Pointer)
	*
	* return value : 계산 실패시 false 를 반환합니다.
	*/
	virtual bool GetCenter(_Out_ float* x, _Out_ float* y);
	/**
	* 현재 객체의 중심을 계산하여 인자값으로 반환합니다.
	* 
	* param[in] x : 중심 x 좌표 (Reference)
	* param[in] y : 중심 y 좌표 (Reference)
	*
	* return value : 계산 실패시 false 를 반환합니다.
	*/
	virtual bool GetCenter(_Out_ double& x, _Out_ double& y);
	/**
	* 현재 객체의 중심을 계산하여 인자값으로 반환합니다.
	*
	* param[in] x : 중심 x 좌표 (Pointer)
	* param[in] y : 중심 y 좌표 (Pointer)
	*
	* return value : 계산 실패시 false 를 반환합니다.
	*/
	virtual bool GetCenter(_Out_ double* x, _Out_ double* y);
	/**
	* 현재 객체의 중심을 계산하여 인자값으로 반환합니다.
	* 
	* param[in] point : 중심 x, y 좌표 (Reference)
	*
	* return value : 계산 실패시 false 를 반환합니다.
	*/
	virtual bool GetCenter(_Out_ POINT& point);
	/**
	* 현재 객체의 중심을 계산하여 인자값으로 반환합니다.
	* 
	* param[in] pPoint : 중심 x, y 좌표 (Pointer)
	*
	* return value : 계산 실패시 false 를 반환합니다.
	*/
	virtual bool GetCenter(_Out_ POINT* pPoint);
	/**
	* 현재 객체의 중심을 계산하여 인자값으로 반환합니다.
	* 
	* param[in] size : 중심 x, y 좌표 (Reference)
	*
	* return value : 계산 실패시 false 를 반환합니다.
	*/
	virtual bool GetCenter(_Out_ SIZE& size);
	/**
	* 현재 객체의 중심을 계산하여 인자값으로 반환합니다.
	* 
	* param[in] pSize : 중심 x, y 좌표 (Pointer)
	*
	* return value : 계산 실패시 false 를 반환합니다.
	*/
	virtual bool GetCenter(_Out_ SIZE* pSize);
	/**
	* 현재 객체의 중심을 계산하여 인자값으로 반환합니다.
	* 
	* param[in] point : 중심 x, y 좌표 (Reference)
	*
	* return value : 계산 실패시 false 를 반환합니다.
	*/
	virtual bool GetCenter(_Out_ CRavidPoint<int>& point);
	/**
	* 현재 객체의 중심을 계산하여 인자값으로 반환합니다.
	* 
	* param[in] pPoint : 중심 x, y 좌표 (Pointer)
	*
	* return value : 계산 실패시 false 를 반환합니다.
	*/
	virtual bool GetCenter(_Out_ CRavidPoint<int>* pPoint);
	/**
	* 현재 객체의 중심을 계산하여 인자값으로 반환합니다.
	* 
	* param[in] point : 중심 x, y 좌표 (Reference)
	*
	* return value : 계산 실패시 false 를 반환합니다.
	*/
	virtual bool GetCenter(_Out_ CRavidPoint<long long>& point);
	/**
	* 현재 객체의 중심을 계산하여 인자값으로 반환합니다.
	* 
	* param[in] pPoint : 중심 x, y 좌표 (Pointer)
	*
	* return value : 계산 실패시 false 를 반환합니다.
	*/
	virtual bool GetCenter(_Out_ CRavidPoint<long long>* pPoint);
	/**
	* 현재 객체의 중심을 계산하여 인자값으로 반환합니다.
	* 
	* param[in] point : 중심 x, y 좌표 (Reference)
	*
	* return value : 계산 실패시 false 를 반환합니다.
	*/
	virtual bool GetCenter(_Out_ CRavidPoint<float>& point);
	/**
	* 현재 객체의 중심을 계산하여 인자값으로 반환합니다.
	* 
	* param[in] pPoint : 중심 x, y 좌표 (Pointer)
	*
	* return value : 계산 실패시 false 를 반환합니다.
	*/
	virtual bool GetCenter(_Out_ CRavidPoint<float>* pPoint);
	/**
	* 현재 객체의 중심을 계산하여 인자값으로 반환합니다.
	* 
	* param[in] point : 중심 x, y 좌표 (Reference)
	*
	* return value : 계산 실패시 false 를 반환합니다.
	*/
	virtual bool GetCenter(_Out_ CRavidPoint<double>& point);
	/**
	* 현재 객체의 중심을 계산하여 인자값으로 반환합니다.
	* 
	* param[in] pPoint : 중심 x, y 좌표 (Pointer)
	*
	* return value : 계산 실패시 false 를 반환합니다.
	*/
	virtual bool GetCenter(_Out_ CRavidPoint<double>* pPoint);
#pragma endregion
	
#pragma region [GetCenterOfGravity]
	/**
	* 현재 객체의 무게중심을 계산하여 인자값으로 반환합니다.
	*
	* param[in] x : 무게중심 x 좌표 (Reference)
	* param[in] y : 무게중심 y 좌표 (Reference)
	*
	* return value : 현재 위치에서 이동된 좌표를 반환합니다.
	*/
	virtual bool GetCenterOfGravity(_Out_ int& x, _Out_ int& y);
	/**
	* 현재 객체의 무게중심을 계산하여 인자값으로 반환합니다.
	*
	* param[in] x : 무게중심 x 좌표 (Pointer)
	* param[in] y : 무게중심 y 좌표 (Pointer)
	*
	* return value : 현재 위치에서 이동된 좌표를 반환합니다.
	*/
	virtual bool GetCenterOfGravity(_Out_ int* x, _Out_ int* y);
	/**
	* 현재 객체의 무게중심을 계산하여 인자값으로 반환합니다.
	*
	* param[in] x : 무게중심 x 좌표 (Reference)
	* param[in] y : 무게중심 y 좌표 (Reference)
	*
	* return value : 현재 위치에서 이동된 좌표를 반환합니다.
	*/
	virtual bool GetCenterOfGravity(_Out_ long long& x, _Out_ long long& y);
	/**
	* 현재 객체의 무게중심을 계산하여 인자값으로 반환합니다.
	*
	* param[in] x : 무게중심 x 좌표 (Pointer)
	* param[in] y : 무게중심 y 좌표 (Pointer)
	*
	* return value : 현재 위치에서 이동된 좌표를 반환합니다.
	*/
	virtual bool GetCenterOfGravity(_Out_ long long* x, _Out_ long long* y);
	/**
	* 현재 객체의 무게중심을 계산하여 인자값으로 반환합니다.
	*
	* param[in] x : 무게중심 x 좌표 (Reference)
	* param[in] y : 무게중심 y 좌표 (Reference)
	*
	* return value : 현재 위치에서 이동된 좌표를 반환합니다.
	*/
	virtual bool GetCenterOfGravity(_Out_ float& x, _Out_ float& y);
	/**
	* 현재 객체의 무게중심을 계산하여 인자값으로 반환합니다.
	*
	* param[in] x : 무게중심 x 좌표 (Pointer)
	* param[in] y : 무게중심 y 좌표 (Pointer)
	*
	* return value : 현재 위치에서 이동된 좌표를 반환합니다.
	*/
	virtual bool GetCenterOfGravity(_Out_ float* x, _Out_ float* y);
	/**
	* 현재 객체의 무게중심을 계산하여 인자값으로 반환합니다.
	*
	* param[in] x : 무게중심 x 좌표 (Reference)
	* param[in] y : 무게중심 y 좌표 (Reference)
	*
	* return value : 현재 위치에서 이동된 좌표를 반환합니다.
	*/
	virtual bool GetCenterOfGravity(_Out_ double& x, _Out_ double& y);
	/**
	* 현재 객체의 무게중심을 계산하여 인자값으로 반환합니다.
	*
	* param[in] x : 무게중심 x 좌표 (Pointer)
	* param[in] y : 무게중심 y 좌표 (Pointer)
	*
	* return value : 현재 위치에서 이동된 좌표를 반환합니다.
	*/
	virtual bool GetCenterOfGravity(_Out_ double* x, _Out_ double* y);
	/**
	* 현재 객체의 무게중심을 계산하여 인자값으로 반환합니다.
	*
	* param[in] point : 무게중심 x, y 좌표 (Reference)
	*
	* return value : 현재 위치에서 이동된 좌표를 반환합니다.
	*/
	virtual bool GetCenterOfGravity(_Out_ POINT& point);
	/**
	* 현재 객체의 무게중심을 계산하여 인자값으로 반환합니다.
	*
	* param[in] pPoint : 무게중심 x, y 좌표 (Pointer)
	*
	* return value : 현재 위치에서 이동된 좌표를 반환합니다.
	*/
	virtual bool GetCenterOfGravity(_Out_ POINT* pPoint);
	/**
	* 현재 객체의 무게중심을 계산하여 인자값으로 반환합니다.
	*
	* param[in] size : 무게중심 x, y 좌표 (Reference)
	*
	* return value : 현재 위치에서 이동된 좌표를 반환합니다.
	*/
	virtual bool GetCenterOfGravity(_Out_ SIZE& size);
	/**
	* 현재 객체의 무게중심을 계산하여 인자값으로 반환합니다.
	*
	* param[in] pSize : 무게중심 x, y 좌표 (Pointer)
	*
	* return value : 현재 위치에서 이동된 좌표를 반환합니다.
	*/
	virtual bool GetCenterOfGravity(_Out_ SIZE* pSize);
	/**
	* 현재 객체의 무게중심을 계산하여 인자값으로 반환합니다.
	*
	* param[in] point : 무게중심 x, y 좌표 (Reference)
	*
	* return value : 현재 위치에서 이동된 좌표를 반환합니다.
	*/
	virtual bool GetCenterOfGravity(_Out_ CRavidPoint<int>& point);
	/**
	* 현재 객체의 무게중심을 계산하여 인자값으로 반환합니다.
	*
	* param[in] pPoint : 무게중심 x, y 좌표 (Pointer)
	*
	* return value : 현재 위치에서 이동된 좌표를 반환합니다.
	*/
	virtual bool GetCenterOfGravity(_Out_ CRavidPoint<int>* pPoint);
	/**
	* 현재 객체의 무게중심을 계산하여 인자값으로 반환합니다.
	*
	* param[in] point : 무게중심 x, y 좌표 (Reference)
	*
	* return value : 현재 위치에서 이동된 좌표를 반환합니다.
	*/
	virtual bool GetCenterOfGravity(_Out_ CRavidPoint<long long>& point);
	/**
	* 현재 객체의 무게중심을 계산하여 인자값으로 반환합니다.
	*
	* param[in] pPoint : 무게중심 x, y 좌표 (Pointer)
	*
	* return value : 현재 위치에서 이동된 좌표를 반환합니다.
	*/
	virtual bool GetCenterOfGravity(_Out_ CRavidPoint<long long>* pPoint);
	/**
	* 현재 객체의 무게중심을 계산하여 인자값으로 반환합니다.
	*
	* param[in] point : 무게중심 x, y 좌표 (Reference)
	*
	* return value : 현재 위치에서 이동된 좌표를 반환합니다.
	*/
	virtual bool GetCenterOfGravity(_Out_ CRavidPoint<float>& point);
	/**
	* 현재 객체의 무게중심을 계산하여 인자값으로 반환합니다.
	*
	* param[in] pPoint : 무게중심 x, y 좌표 (Pointer)
	*
	* return value : 현재 위치에서 이동된 좌표를 반환합니다.
	*/
	virtual bool GetCenterOfGravity(_Out_ CRavidPoint<float>* pPoint);
	/**
	* 현재 객체의 무게중심을 계산하여 인자값으로 반환합니다.
	*
	* param[in] point : 무게중심 x, y 좌표 (Reference)
	*
	* return value : 현재 위치에서 이동된 좌표를 반환합니다.
	*/
	virtual bool GetCenterOfGravity(_Out_ CRavidPoint<double>& point);
	/**
	* 현재 객체의 무게중심을 계산하여 인자값으로 반환합니다.
	*
	* param[in] pPoint : 무게중심 x, y 좌표 (Pointer)
	*
	* return value : 현재 위치에서 이동된 좌표를 반환합니다.
	*/
	virtual bool GetCenterOfGravity(_Out_ CRavidPoint<double>* pPoint);
#pragma endregion

