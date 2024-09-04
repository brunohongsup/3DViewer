public:
#pragma region [CRavidPoint]
	CRavidPoint(_In_ int x, _In_ int y);
	CRavidPoint(_In_ int x, _In_ long long y);
	CRavidPoint(_In_ int x, _In_ float y);
	CRavidPoint(_In_ int x, _In_ double y);
	CRavidPoint(_In_ long long x, _In_ int y);
	CRavidPoint(_In_ long long x, _In_ long long y);
	CRavidPoint(_In_ long long x, _In_ float y);
	CRavidPoint(_In_ long long x, _In_ double y);
	CRavidPoint(_In_ float x, _In_ int y);
	CRavidPoint(_In_ float x, _In_ long long y);
	CRavidPoint(_In_ float x, _In_ float y);
	CRavidPoint(_In_ float x, _In_ double y);
	CRavidPoint(_In_ double x, _In_ int y);
	CRavidPoint(_In_ double x, _In_ long long y);
	CRavidPoint(_In_ double x, _In_ float y);
	CRavidPoint(_In_ double x, _In_ double y);
	CRavidPoint(_In_ const POINT& point);
	CRavidPoint(_In_ POINT* pPoint);
	CRavidPoint(_In_ const SIZE& size);
	CRavidPoint(_In_ SIZE* pSize);
	CRavidPoint(_In_ const CRavidPoint<int>& point);
	CRavidPoint(_In_ CRavidPoint<int>* pPoint);
	CRavidPoint(_In_ const CRavidPoint<long long>& point);
	CRavidPoint(_In_ CRavidPoint<long long>* pPoint);
	CRavidPoint(_In_ const CRavidPoint<float>& point);
	CRavidPoint(_In_ CRavidPoint<float>* pPoint);
	CRavidPoint(_In_ const CRavidPoint<double>& point);
	CRavidPoint(_In_ CRavidPoint<double>* pPoint);
#pragma endregion
	
#pragma region [SetPoint]
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] x : x 좌표
	* param[in] y : y 좌표
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetPoint(_In_ int x, _In_ int y);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] x : x 좌표
	* param[in] y : y 좌표
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetPoint(_In_ int x, _In_ long long y);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] x : x 좌표
	* param[in] y : y 좌표
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetPoint(_In_ int x, _In_ float y);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] x : x 좌표
	* param[in] y : y 좌표
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetPoint(_In_ int x, _In_ double y);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] x : x 좌표
	* param[in] y : y 좌표
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetPoint(_In_ long long x, _In_ int y);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] x : x 좌표
	* param[in] y : y 좌표
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetPoint(_In_ long long x, _In_ long long y);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] x : x 좌표
	* param[in] y : y 좌표
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetPoint(_In_ long long x, _In_ float y);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] x : x 좌표
	* param[in] y : y 좌표
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetPoint(_In_ long long x, _In_ double y);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] x : x 좌표
	* param[in] y : y 좌표
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetPoint(_In_ float x, _In_ int y);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] x : x 좌표
	* param[in] y : y 좌표
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetPoint(_In_ float x, _In_ long long y);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] x : x 좌표
	* param[in] y : y 좌표
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetPoint(_In_ float x, _In_ float y);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] x : x 좌표
	* param[in] y : y 좌표
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetPoint(_In_ float x, _In_ double y);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] x : x 좌표
	* param[in] y : y 좌표
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetPoint(_In_ double x, _In_ int y);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] x : x 좌표
	* param[in] y : y 좌표
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetPoint(_In_ double x, _In_ long long y);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] x : x 좌표
	* param[in] y : y 좌표
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetPoint(_In_ double x, _In_ float y);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] x : x 좌표
	* param[in] y : y 좌표
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetPoint(_In_ double x, _In_ double y);	
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] refGeometry : POINT x, y 좌표 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetPoint(_In_ POINT& refGeometry);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] pGeometry : POINT x, y 좌표 (Pointer)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetPoint(_In_ POINT* pGeometry);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] refGeometry : SIZE x, y 좌표 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetPoint(_In_ SIZE& refGeometry);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] pGeometry : SIZE x, y 좌표 (Pointer)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetPoint(_In_ SIZE* pGeometry);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] refGeometry : CRavidPoint x, y 좌표 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetPoint(_In_ CRavidPoint<int>& refGeometry);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] pGeometry : CRavidPoint x, y 좌표 (Pointer)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetPoint(_In_ CRavidPoint<int>* pGeometry);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] refGeometry : CRavidPoint x, y 좌표 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetPoint(_In_ CRavidPoint<long long>& refGeometry);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] pGeometry : CRavidPoint x, y 좌표 (Pointer)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetPoint(_In_ CRavidPoint<long long>* pGeometry);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] refGeometry : CRavidPoint x, y 좌표 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetPoint(_In_ CRavidPoint<float>& refGeometry);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] pGeometry : CRavidPoint x, y 좌표 (Pointer)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetPoint(_In_ CRavidPoint<float>* pGeometry);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] refGeometry : x, y 좌표 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetPoint(_In_ CRavidPoint<double>& refGeometry);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] pGeometry : CRavidPoint x, y 좌표 (Pointer)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetPoint(_In_ CRavidPoint<double>* pGeometry);
#pragma endregion
