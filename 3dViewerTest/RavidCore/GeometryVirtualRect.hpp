public:
		
#pragma region [GetRect]
	/**
	* 현재 객체를 포함하는 최소 면적의 직사각형(각도 0)을 계산하여 인자값으로 반환합니다.
	*
	* param[in] left : 최소 x 축 값 (Reference)
	* param[in] top : 최소 y 축 값 (Reference)
	* param[in] right : 최대 x 축 값 (Reference)
	* param[in] bottom : 최대 y 축 값 (Reference)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool GetRect(_Out_ int& left, _Out_ int& top, _Out_ int& right, _Out_ int& bottom);
	/**
	* 현재 객체를 포함하는 최소 면적의 직사각형(각도 0)을 계산하여 인자값으로 반환합니다.
	*
	* param[in] pLeft : 최소 x 축 값 (Pointer)
	* param[in] pTop : 최소 y 축 값 (Pointer)
	* param[in] pRight : 최대 x 축 값 (Pointer)
	* param[in] pBottom : 최대 y 축 값 (Pointer)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool GetRect(_Out_ int* pLeft, _Out_ int* pTop, _Out_ int* pRight, _Out_ int* pBottom);
	/**
	* 현재 객체를 포함하는 최소 면적의 직사각형(각도 0)을 계산하여 인자값으로 반환합니다.
	*
	* param[in] left : 최소 x 축 값 (Reference)
	* param[in] top : 최소 y 축 값 (Reference)
	* param[in] right : 최대 x 축 값 (Reference)
	* param[in] bottom : 최대 y 축 값 (Reference)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool GetRect(_Out_ long long& left, _Out_ long long& top, _Out_ long long& right, _Out_ long long& bottom);
	/**
	* 현재 객체를 포함하는 최소 면적의 직사각형(각도 0)을 계산하여 인자값으로 반환합니다.
	*
	* param[in] pLeft : 최소 x 축 값 (Pointer)
	* param[in] pTop : 최소 y 축 값 (Pointer)
	* param[in] pRight : 최대 x 축 값 (Pointer)
	* param[in] pBottom : 최대 y 축 값 (Pointer)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool GetRect(_Out_ long long* pLeft, _Out_ long long* pTop, _Out_ long long* pRight, _Out_ long long* pBottom);
	/**
	* 현재 객체를 포함하는 최소 면적의 직사각형(각도 0)을 계산하여 인자값으로 반환합니다.
	*
	* param[in] left : 최소 x 축 값 (Reference)
	* param[in] top : 최소 y 축 값 (Reference)
	* param[in] right : 최대 x 축 값 (Reference)
	* param[in] bottom : 최대 y 축 값 (Reference)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool GetRect(_Out_ float& left, _Out_ float& top, _Out_ float& right, _Out_ float& bottom);
	/**
	* 현재 객체를 포함하는 최소 면적의 직사각형(각도 0)을 계산하여 인자값으로 반환합니다.
	*
	* param[in] pLeft : 최소 x 축 값 (Pointer)
	* param[in] pTop : 최소 y 축 값 (Pointer)
	* param[in] pRight : 최대 x 축 값 (Pointer)
	* param[in] pBottom : 최대 y 축 값 (Pointer)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool GetRect(_Out_ float* pLeft, _Out_ float* pTop, _Out_ float* pRight, _Out_ float*pBottom);
	/**
	* 현재 객체를 포함하는 최소 면적의 직사각형(각도 0)을 계산하여 인자값으로 반환합니다.
	*
	* param[in] left : 최소 x 축 값 (Reference)
	* param[in] top : 최소 y 축 값 (Reference)
	* param[in] right : 최대 x 축 값 (Reference)
	* param[in] bottom : 최대 y 축 값 (Reference)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool GetRect(_Out_ double& left, _Out_ double& top, _Out_ double& right, _Out_ double& bottom);
	/**
	* 현재 객체를 포함하는 최소 면적의 직사각형(각도 0)을 계산하여 인자값으로 반환합니다.
	*
	* param[in] pLeft : 최소 x 축 값 (Pointer)
	* param[in] pTop : 최소 y 축 값 (Pointer)
	* param[in] pRight : 최대 x 축 값 (Pointer)
	* param[in] pBottom : 최대 y 축 값 (Pointer)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool GetRect(_Out_ double* pLeft, _Out_ double* pTop, _Out_ double* pRight, _Out_ double* pBottom);
	/**
	* 현재 객체를 포함하는 최소 면적의 직사각형(각도 0)을 계산하여 인자값으로 반환합니다.
	*
	* param[in] rect : left = min X, top = min Y, right = max X, bottom = max Y 영역 (Reference)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool GetRect(_Out_ RECT& rect);
	/**
	* 현재 객체를 포함하는 최소 면적의 직사각형(각도 0)을 계산하여 인자값으로 반환합니다.
	*
	* param[in] pRect : left = min X, top = min Y, right = max X, bottom = max Y 영역 (Pointer)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool GetRect(_Out_ RECT* pRect);
	/**
	* 현재 객체를 포함하는 최소 면적의 직사각형(각도 0)을 계산하여 인자값으로 반환합니다.
	*
	* param[in] rect : left = min X, top = min Y, right = max X, bottom = max Y 영역 (Reference)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool GetRect(_Out_ CRavidRect<int>& rect);
	/**
	* 현재 객체를 포함하는 최소 면적의 직사각형(각도 0)을 계산하여 인자값으로 반환합니다.
	*
	* param[in] pRect : left = min X, top = min Y, right = max X, bottom = max Y 영역 (Pointer)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool GetRect(_Out_ CRavidRect<int>* pRect);
	/**
	* 현재 객체를 포함하는 최소 면적의 직사각형(각도 0)을 계산하여 인자값으로 반환합니다.
	*
	* param[in] rect : left = min X, top = min Y, right = max X, bottom = max Y 영역 (Reference)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool GetRect(_Out_ CRavidRect<long long>& rect);
	/**
	* 현재 객체를 포함하는 최소 면적의 직사각형(각도 0)을 계산하여 인자값으로 반환합니다.
	*
	* param[in] pRect : left = min X, top = min Y, right = max X, bottom = max Y 영역 (Pointer)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool GetRect(_Out_ CRavidRect<long long>* pRect);
	/**
	* 현재 객체를 포함하는 최소 면적의 직사각형(각도 0)을 계산하여 인자값으로 반환합니다.
	*
	* param[in] rect : left = min X, top = min Y, right = max X, bottom = max Y 영역 (Reference)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool GetRect(_Out_ CRavidRect<float>& rect);
	/**
	* 현재 객체를 포함하는 최소 면적의 직사각형(각도 0)을 계산하여 인자값으로 반환합니다.
	*
	* param[in] pRect : left = min X, top = min Y, right = max X, bottom = max Y 영역 (Pointer)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool GetRect(_Out_ CRavidRect<float>* pRect);
	/**
	* 현재 객체를 포함하는 최소 면적의 직사각형(각도 0)을 계산하여 인자값으로 반환합니다.
	*
	* param[in] rect : left = min X, top = min Y, right = max X, bottom = max Y 영역 (Reference)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool GetRect(_Out_ CRavidRect<double>& rect);
	/**
	* 현재 객체를 포함하는 최소 면적의 직사각형(각도 0)을 계산하여 인자값으로 반환합니다.
	*
	* param[in] pRect : left = min X, top = min Y, right = max X, bottom = max Y 영역 (Pointer)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool GetRect(_Out_ CRavidRect<double>* pRect);
#pragma endregion