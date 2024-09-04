public:

#pragma region [Offset]
	/**
	* 현재 객체를 인자값 만큼 이동합니다.
	*
	* param[in] x : x 축 증분
	* param[in] y : y 축 증분
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	virtual bool Offset(_In_ int x, _In_ int y);
	/**
	* 현재 객체를 인자값 만큼 이동합니다.
	*
	* param[in] x : x 축 증분
	* param[in] y : y 축 증분
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	virtual bool Offset(_In_ long long x, _In_ long long y);
	/**
	* 현재 객체를 인자값 만큼 이동합니다.
	*
	* param[in] x : x 축 증분
	* param[in] y : y 축 증분
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	virtual bool Offset(_In_ float x, _In_ float y);
	/**
	* 현재 객체를 인자값 만큼 이동합니다.
	*
	* param[in] x : x 축 증분
	* param[in] y : y 축 증분
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	virtual bool Offset(_In_ double x, _In_ double y);
	/**
	* 현재 객체를 인자값 만큼 이동합니다.
	*
	* param[in] point : x, y 축 증분 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	virtual bool Offset(_In_ POINT& point);
	/**
	* 현재 객체를 인자값 만큼 이동합니다.
	*
	* param[in] pPoint : x, y 축 증분 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	virtual bool Offset(_In_ POINT* pPoint);
	/**
	* 현재 객체를 인자값 만큼 이동합니다.
	*
	* param[in] size : x, y 축 증분 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	virtual bool Offset(_In_ SIZE& size);
	/**
	* 현재 객체를 인자값 만큼 이동합니다.
	*
	* param[in] pSize : x, y 축 증분 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	virtual bool Offset(_In_ SIZE* pSize);
	/**
	* 현재 객체를 인자값 만큼 이동합니다.
	*
	* param[in] point : x, y 축 증분 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	virtual bool Offset(_In_ CRavidPoint<int>& point);
	/**
	* 현재 객체를 인자값 만큼 이동합니다.
	*
	* param[in] pSize : x, y 축 증분 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	virtual bool Offset(_In_ CRavidPoint<int>* pPoint);
	/**
	* 현재 객체를 인자값 만큼 이동합니다.
	*
	* param[in] point : x, y 축 증분 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	virtual bool Offset(_In_ CRavidPoint<long long>& point);
	/**
	* 현재 객체를 인자값 만큼 이동합니다.
	*
	* param[in] pSize : x, y 축 증분 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	virtual bool Offset(_In_ CRavidPoint<long long>* pPoint);
	/**
	* 현재 객체를 인자값 만큼 이동합니다.
	*
	* param[in] point : x, y 축 증분 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	virtual bool Offset(_In_ CRavidPoint<float>& point);
	/**
	* 현재 객체를 인자값 만큼 이동합니다.
	*
	* param[in] pSize : x, y 축 증분 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	virtual bool Offset(_In_ CRavidPoint<float>* pPoint);
	/**
	* 현재 객체를 인자값 만큼 이동합니다.
	*
	* param[in] point : x, y 축 증분 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	virtual bool Offset(_In_ CRavidPoint<double>& point);
	/**
	* 현재 객체를 인자값 만큼 이동합니다.
	*
	* param[in] pSize : x, y 축 증분 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	virtual bool Offset(_In_ CRavidPoint<double>* pPoint);
#pragma endregion

#pragma region [PowOffset]
	/**
	* 현재 객체를 인자값의 곱 만큼 이동합니다.
	* 
	* param[in] powX : 
	* param[in] powY : 
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	virtual bool PowOffset(_In_ double powX, _In_ double powY);
#pragma endregion

