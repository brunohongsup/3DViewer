public:

#pragma region [Deflate]
	/**
	* 현재 객체의 중심을 기준으로 입력된 값 만큼 크기를 축소합니다.
	*
	* param[in] nRadius : left, top, right, bottom 4 방향 동일 축소 값
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool Deflate(_In_ int nRadius);
	/**
	* 현재 객체의 중심을 기준으로 입력된 값 만큼 크기를 축소합니다.
	*
	* param[in] llRadius : left, top, right, bottom 4 방향 동일 축소 값
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool Deflate(_In_ long long llRadius);
	/**
	* 현재 객체의 중심을 기준으로 입력된 값 만큼 크기를 축소합니다.
	*
	* param[in] fRadius : left, top, right, bottom 4 방향 동일 축소 값
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool Deflate(_In_ float fRadius);
	/**
	* 현재 객체의 중심을 기준으로 입력된 값 만큼 크기를 축소합니다.
	*
	* param[in] dblRadius : left, top, right, bottom 4 방향 동일 축소 값
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool Deflate(_In_ double dblRadius);
	/**
	* 현재 객체의 중심을 기준으로 입력된 값 만큼 크기를 축소합니다.
	*
	* param[in] left : left 방향 축소 값
	* param[in] top : top 방향 축소 값
	* param[in] right : right 방향 축소 값
	* param[in] bottom : bottom 방향 축소 값
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool Deflate(_In_ int left, _In_ int top, _In_ int right, _In_ int bottom);
	/**
	* 현재 객체의 중심을 기준으로 입력된 값 만큼 크기를 축소합니다.
	*
	* param[in] left : left 방향 축소 값
	* param[in] top : top 방향 축소 값
	* param[in] right : right 방향 축소 값
	* param[in] bottom : bottom 방향 축소 값
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool Deflate(_In_ long long left, _In_ long long top, _In_ long long right, _In_ long long bottom);
	/**
	* 현재 객체의 중심을 기준으로 입력된 값 만큼 크기를 축소합니다.
	*
	* param[in] left : left 방향 축소 값
	* param[in] top : top 방향 축소 값
	* param[in] right : right 방향 축소 값
	* param[in] bottom : bottom 방향 축소 값
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool Deflate(_In_ float left, _In_ float top, _In_ float right, _In_ float bottom);
	/**
	* 현재 객체의 중심을 기준으로 입력된 값 만큼 크기를 축소합니다.
	*
	* param[in] left : left 방향 축소 값
	* param[in] top : top 방향 축소 값
	* param[in] right : right 방향 축소 값
	* param[in] bottom : bottom 방향 축소 값
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool Deflate(_In_ double left, _In_ double top, _In_ double right, _In_ double bottom);
	/**
	* 현재 객체의 중심을 기준으로 입력된 값 만큼 크기를 축소합니다.
	*
	* param[in] width : x 축 양방향 동일 축소 값
	* param[in] height : y 축 양방향 동일 축소 값
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool Deflate(_In_ int width, _In_ int height);
	/**
	* 현재 객체의 중심을 기준으로 입력된 값 만큼 크기를 축소합니다.
	*
	* param[in] width : x 축 양방향 동일 축소 값
	* param[in] height : y 축 양방향 동일 축소 값
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool Deflate(_In_ long long width, _In_ long long height);
	/**
	* 현재 객체의 중심을 기준으로 입력된 값 만큼 크기를 축소합니다.
	*
	* param[in] width : x 축 양방향 동일 축소 값
	* param[in] height : y 축 양방향 동일 축소 값
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool Deflate(_In_ float width, _In_ float height);
	/**
	* 현재 객체의 중심을 기준으로 입력된 값 만큼 크기를 축소합니다.
	*
	* param[in] width : x 축 양방향 동일 축소 값
	* param[in] height : y 축 양방향 동일 축소 값
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool Deflate(_In_ double width, _In_ double height);
	/**
	* 현재 객체의 중심을 기준으로 입력된 값 만큼 크기를 축소합니다.
	*
	* param[in] point : x, y 축 각 양방향 동일 축소 값 (Reference)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool Deflate(_In_ POINT& point);
	/**
	* 현재 객체의 중심을 기준으로 입력된 값 만큼 크기를 축소합니다.
	*
	* param[in] pPoint : x, y 축 각 양방향 동일 축소 값 (Pointer)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool Deflate(_In_ POINT* pPoint);
	/**
	* 현재 객체의 중심을 기준으로 입력된 값 만큼 크기를 축소합니다.
	*
	* param[in] size : x, y 축 각 양방향 동일 축소 값 (Reference)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool Deflate(_In_ SIZE& size);
	/**
	* 현재 객체의 중심을 기준으로 입력된 값 만큼 크기를 축소합니다.
	*
	* param[in] pSize : x, y 축 각 양방향 동일 축소 값 (Pointer)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool Deflate(_In_ SIZE* pSize);
	/**
	* 현재 객체의 중심을 기준으로 입력된 값 만큼 크기를 축소합니다.
	*
	* param[in] point : x, y 축 각 양방향 동일 축소 값 (Reference)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool Deflate(_In_ CRavidPoint<int>& point);
	/**
	* 현재 객체의 중심을 기준으로 입력된 값 만큼 크기를 축소합니다.
	*
	* param[in] pPoint : x, y 축 각 양방향 동일 축소 값 (Pointer)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool Deflate(_In_ CRavidPoint<int>* pPoint);
	/**
	* 현재 객체의 중심을 기준으로 입력된 값 만큼 크기를 축소합니다.
	*
	* param[in] point : x, y 축 각 양방향 동일 축소 값 (Reference)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool Deflate(_In_ CRavidPoint<long long>& point);
	/**
	* 현재 객체의 중심을 기준으로 입력된 값 만큼 크기를 축소합니다.
	*
	* param[in] pPoint : x, y 축 각 양방향 동일 축소 값 (Pointer)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool Deflate(_In_ CRavidPoint<long long>* pPoint);
	/**
	* 현재 객체의 중심을 기준으로 입력된 값 만큼 크기를 축소합니다.
	*
	* param[in] point : x, y 축 각 양방향 동일 축소 값 (Reference)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool Deflate(_In_ CRavidPoint<float>& point);
	/**
	* 현재 객체의 중심을 기준으로 입력된 값 만큼 크기를 축소합니다.
	*
	* param[in] pPoint : x, y 축 각 양방향 동일 축소 값 (Pointer)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool Deflate(_In_ CRavidPoint<float>* pPoint);
	/**
	* 현재 객체의 중심을 기준으로 입력된 값 만큼 크기를 축소합니다.
	*
	* param[in] point : x, y 축 각 양방향 동일 축소 값 (Reference)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool Deflate(_In_ CRavidPoint<double>& point);
	/**
	* 현재 객체의 중심을 기준으로 입력된 값 만큼 크기를 축소합니다.
	*
	* param[in] pPoint : x, y 축 각 양방향 동일 축소 값 (Pointer)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool Deflate(_In_ CRavidPoint<double>* pPoint);
	/**
	* 현재 객체의 중심을 기준으로 입력된 값 만큼 크기를 축소합니다.
	*
	* param[in] rect : left, top, right, bottom 각 방향 축소 값 (Reference)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool Deflate(_In_ RECT& rect);
	/**
	* 현재 객체의 중심을 기준으로 입력된 값 만큼 크기를 축소합니다.
	*
	* param[in] pRect : left, top, right, bottom 각 방향 축소 값 (Pointer)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool Deflate(_In_ RECT* pRect);
	/**
	* 현재 객체의 중심을 기준으로 입력된 값 만큼 크기를 축소합니다.
	*
	* param[in] rect : left, top, right, bottom 각 방향 축소 값 (Reference)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool Deflate(_In_ CRavidRect<int>& rect);
	/**
	* 현재 객체의 중심을 기준으로 입력된 값 만큼 크기를 축소합니다.
	*
	* param[in] pRect : left, top, right, bottom 각 방향 축소 값 (Pointer)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool Deflate(_In_ CRavidRect<int>* pRect);
	/**
	* 현재 객체의 중심을 기준으로 입력된 값 만큼 크기를 축소합니다.
	*
	* param[in] rect : left, top, right, bottom 각 방향 축소 값 (Reference)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool Deflate(_In_ CRavidRect<long long>& rect);
	/**
	* 현재 객체의 중심을 기준으로 입력된 값 만큼 크기를 축소합니다.
	*
	* param[in] pRect : left, top, right, bottom 각 방향 축소 값 (Pointer)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool Deflate(_In_ CRavidRect<long long>* pRect);
	/**
	* 현재 객체의 중심을 기준으로 입력된 값 만큼 크기를 축소합니다.
	*
	* param[in] rect : left, top, right, bottom 각 방향 축소 값 (Reference)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool Deflate(_In_ CRavidRect<float>& rect);
	/**
	* 현재 객체의 중심을 기준으로 입력된 값 만큼 크기를 축소합니다.
	*
	* param[in] pRect : left, top, right, bottom 각 방향 축소 값 (Pointer)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool Deflate(_In_ CRavidRect<float>* pRect);
	/**
	* 현재 객체의 중심을 기준으로 입력된 값 만큼 크기를 축소합니다.
	*
	* param[in] rect : left, top, right, bottom 각 방향 축소 값 (Reference)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool Deflate(_In_ CRavidRect<double>& rect);
	/**
	* 현재 객체의 중심을 기준으로 입력된 값 만큼 크기를 축소합니다.
	*
	* param[in] pRect : left, top, right, bottom 각 방향 축소 값 (Pointer)
	*
	* return value : 계산에 실패하면 false 를 반환합니다.
	*/
	virtual bool Deflate(_In_ CRavidRect<double>* pRect);
#pragma endregion