public:

#pragma region [CRavidQuadrangle]
	CRavidQuadrangle(_In_ Ravid::Algorithms::CRavidImage& ri);
	CRavidQuadrangle(_In_ T x1, _In_ T y1, _In_ Ravid::Algorithms::CRavidImage& ri);
	CRavidQuadrangle(_In_ const SIZE& size, _In_ Ravid::Algorithms::CRavidImage& ri);
	CRavidQuadrangle(_In_ SIZE* pSize, _In_ Ravid::Algorithms::CRavidImage& ri);
	CRavidQuadrangle(_In_ const POINT& point, _In_ Ravid::Algorithms::CRavidImage& ri);
	CRavidQuadrangle(_In_ POINT* pPoint, _In_ Ravid::Algorithms::CRavidImage& ri);
	CRavidQuadrangle(_In_ const CRavidPoint<int>& point, Ravid::Algorithms::CRavidImage& ri);
	CRavidQuadrangle(_In_ CRavidPoint<int>* pPoint, Ravid::Algorithms::CRavidImage& ri);
	CRavidQuadrangle(_In_ const CRavidPoint<long long>& po, Ravid::Algorithms::CRavidImage& ri);
	CRavidQuadrangle(_In_ CRavidPoint<long long>* pPoint, Ravid::Algorithms::CRavidImage& ri);
	CRavidQuadrangle(_In_ const CRavidPoint<float>& point, Ravid::Algorithms::CRavidImage& ri);
	CRavidQuadrangle(_In_ CRavidPoint<float>* pPoint, Ravid::Algorithms::CRavidImage& ri);
	CRavidQuadrangle(_In_ const CRavidPoint<double>& point, Ravid::Algorithms::CRavidImage& ri);
	CRavidQuadrangle(_In_ CRavidPoint<double>* pPoint, Ravid::Algorithms::CRavidImage& ri);

	CRavidQuadrangle(_In_ T x1, _In_ T y1, _In_ T x2, _In_ T y2, _In_ T x3, _In_ T y3, _In_ T x4, _In_ T y4);
	CRavidQuadrangle(_In_ const SIZE& size);
	CRavidQuadrangle(_In_ SIZE* pSize);
	CRavidQuadrangle(_In_ const POINT& point1, _In_ const POINT& point2, _In_ const POINT& point3, _In_ const POINT& point4);
	CRavidQuadrangle(_In_ POINT* pPoint1, _In_ POINT* pPoint2, _In_ POINT* pPoint3, _In_ POINT* pPoint4);
	CRavidQuadrangle(_In_ const CRavidPoint<int>& point1, _In_ const CRavidPoint<int>& point2, _In_ const CRavidPoint<int>& point3, _In_ const CRavidPoint<int>& point4);
	CRavidQuadrangle(_In_ CRavidPoint<int>* pPoint1, _In_ CRavidPoint<int>* pPoint2, _In_ CRavidPoint<int>* pPoint3, _In_ CRavidPoint<int>* pPoint4);
	CRavidQuadrangle(_In_ const CRavidPoint<long long>& point1, _In_ const CRavidPoint<long long>& point2, _In_ const CRavidPoint<long long>& point3, _In_ const CRavidPoint<long long>& point4);
	CRavidQuadrangle(_In_ CRavidPoint<long long>* pPoint1, _In_ CRavidPoint<long long>* pPoint2, _In_ CRavidPoint<long long>* pPoint3, _In_ CRavidPoint<long long>* pPoint4);
	CRavidQuadrangle(_In_ const CRavidPoint<float>& point1, _In_ const CRavidPoint<float>& point2, _In_ const CRavidPoint<float>& point3, _In_ const CRavidPoint<float>& point4);
	CRavidQuadrangle(_In_ CRavidPoint<float>* pPoint1, _In_ CRavidPoint<float>* pPoint2, _In_ CRavidPoint<float>* pPoint3, _In_ CRavidPoint<float>* pPoint4);
	CRavidQuadrangle(_In_ const CRavidPoint<double>& point1, _In_ const CRavidPoint<double>& point2, _In_ const CRavidPoint<double>& point3, _In_ const CRavidPoint<double>& point4);
	CRavidQuadrangle(_In_ CRavidPoint<double>* pPoint1, _In_ CRavidPoint<double>* pPoint2, _In_ CRavidPoint<double>* pPoint3, _In_ CRavidPoint<double>* pPoint4);
	CRavidQuadrangle(_In_ const RECT& rect);
	CRavidQuadrangle(_In_ RECT* pRect);
	CRavidQuadrangle(_In_ const CRavidRect<int>& rect);
	CRavidQuadrangle(_In_ CRavidRect<int>* pRect);
	CRavidQuadrangle(_In_ const CRavidRect<long long>& rect);
	CRavidQuadrangle(_In_ CRavidRect<long long>* pRect);
	CRavidQuadrangle(_In_ const CRavidRect<float>& rect);
	CRavidQuadrangle(_In_ CRavidRect<float>* pRect);
	CRavidQuadrangle(_In_ const CRavidRect<double>& rect);
	CRavidQuadrangle(_In_ CRavidRect<double>* pRect);
	CRavidQuadrangle(_In_ const CRavidQuadrangle<int>& quad);
	CRavidQuadrangle(_In_ CRavidQuadrangle<int>* pQuad);
	CRavidQuadrangle(_In_ const CRavidQuadrangle<long long>& quad);
	CRavidQuadrangle(_In_ CRavidQuadrangle<long long>* pQuad);
	CRavidQuadrangle(_In_ const CRavidQuadrangle<float>& quad);
	CRavidQuadrangle(_In_ CRavidQuadrangle<float>* pQuad);
	CRavidQuadrangle(_In_ const CRavidQuadrangle<double>& quad);
	CRavidQuadrangle(_In_ CRavidQuadrangle<double>* pQuad);
#pragma endregion

#pragma region [SetQuadrangle]
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] x1 : 1번째 x축 값
	* param[in] y1 : 1번째 y축 값
	* param[in] x2 : 2번째 x축 값
	* param[in] y2 : 2번째 y축 값
	* param[in] x3 : 3번째 x축 값
	* param[in] y3 : 3번째 y축 값
	* param[in] x4 : 4번째 x축 값
	* param[in] y4 : 4번째 y축 값
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetQuadrangle(_In_ int x1, _In_ int y1, _In_ int x2, _In_ int y2, _In_ int x3, _In_ int y3, _In_ int x4, _In_ int y4);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] x1 : 1번째 x축 값
	* param[in] y1 : 1번째 y축 값
	* param[in] x2 : 2번째 x축 값
	* param[in] y2 : 2번째 y축 값
	* param[in] x3 : 3번째 x축 값
	* param[in] y3 : 3번째 y축 값
	* param[in] x4 : 4번째 x축 값
	* param[in] y4 : 4번째 y축 값
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetQuadrangle(_In_ long long x1, _In_ long long y1, _In_ long long x2, _In_ long long y2, _In_ long long x3, _In_ long long y3, _In_ long long x4, _In_ long long y4);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] x1 : 1번째 x축 값
	* param[in] y1 : 1번째 y축 값
	* param[in] x2 : 2번째 x축 값
	* param[in] y2 : 2번째 y축 값
	* param[in] x3 : 3번째 x축 값
	* param[in] y3 : 3번째 y축 값
	* param[in] x4 : 4번째 x축 값
	* param[in] y4 : 4번째 y축 값
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetQuadrangle(_In_ float x1, _In_ float y1, _In_ float x2, _In_ float y2, _In_ float x3, _In_ float y3, _In_ float x4, _In_ float y4);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] x1 : 1번째 x축 값
	* param[in] y1 : 1번째 y축 값
	* param[in] x2 : 2번째 x축 값
	* param[in] y2 : 2번째 y축 값
	* param[in] x3 : 3번째 x축 값
	* param[in] y3 : 3번째 y축 값
	* param[in] x4 : 4번째 x축 값
	* param[in] y4 : 4번째 y축 값
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetQuadrangle(_In_ double x1, _In_ double y1, _In_ double x2, _In_ double y2, _In_ double x3, _In_ double y3, _In_ double x4, _In_ double y4);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] refGeometry1 : 1번째 POINT 점 x, y 값 (Reference)
	* param[in] refGeometry2 : 2번째 POINT 점 x, y 값 (Reference)
	* param[in] refGeometry3 : 3번째 POINT 점 x, y 값 (Reference)
	* param[in] refGeometry4 : 4번째 POINT 점 x, y 값 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetQuadrangle(_In_ POINT& refGeometry1, _In_ POINT& refGeometry2, _In_ POINT& refGeometry3, _In_ POINT& refGeometry4);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] pGeometry1 : 1번째 POINT 점 x, y 값 (Pointer)
	* param[in] pGeometry2 : 2번째 POINT 점 x, y 값 (Pointer)
	* param[in] pGeometry3 : 3번째 POINT 점 x, y 값 (Pointer)
	* param[in] pGeometry4 : 4번째 POINT 점 x, y 값 (Pointer)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetQuadrangle(_In_ POINT* pGeometry1, _In_ POINT* pGeometry2, _In_ POINT* pGeometry3, _In_ POINT* pGeometry4);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] refGeometry1 : 1번째 CRavidPoint 점 x, y 값 (Reference)
	* param[in] refGeometry2 : 2번째 CRavidPoint 점 x, y 값 (Reference)
	* param[in] refGeometry3 : 3번째 CRavidPoint 점 x, y 값 (Reference)
	* param[in] refGeometry4 : 4번째 CRavidPoint 점 x, y 값 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetQuadrangle(_In_ CRavidPoint<int>& refGeometry1, _In_ CRavidPoint<int>& refGeometry2, _In_ CRavidPoint<int>& refGeometry3, _In_ CRavidPoint<int>& refGeometry4);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] pGeometry1 : 1번째 CRavidPoint 점 x, y 값 (Pointer)
	* param[in] pGeometry2 : 2번째 CRavidPoint 점 x, y 값 (Pointer)
	* param[in] pGeometry3 : 3번째 CRavidPoint 점 x, y 값 (Pointer)
	* param[in] pGeometry4 : 4번째 CRavidPoint 점 x, y 값 (Pointer)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetQuadrangle(_In_ CRavidPoint<int>* pGeometry1, _In_ CRavidPoint<int>* pGeometry2, _In_ CRavidPoint<int>* pGeometry3, _In_ CRavidPoint<int>* pGeometry4);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] refGeometry1 : 1번째 CRavidPoint 점 x, y 값 (Reference)
	* param[in] refGeometry2 : 2번째 CRavidPoint 점 x, y 값 (Reference)
	* param[in] refGeometry3 : 3번째 CRavidPoint 점 x, y 값 (Reference)
	* param[in] refGeometry4 : 4번째 CRavidPoint 점 x, y 값 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetQuadrangle(_In_ CRavidPoint<long long>& refGeometry1, _In_ CRavidPoint<long long>& refGeometry2, _In_ CRavidPoint<long long>& refGeometry3, _In_ CRavidPoint<long long>& refGeometry4);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] pGeometry1 : 1번째 CRavidPoint 점 x, y 값 (Pointer)
	* param[in] pGeometry2 : 2번째 CRavidPoint 점 x, y 값 (Pointer)
	* param[in] pGeometry3 : 3번째 CRavidPoint 점 x, y 값 (Pointer)
	* param[in] pGeometry4 : 4번째 CRavidPoint 점 x, y 값 (Pointer)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetQuadrangle(_In_ CRavidPoint<long long>* pGeometry1, _In_ CRavidPoint<long long>* pGeometry2, _In_ CRavidPoint<long long>* pGeometry3, _In_ CRavidPoint<long long>* pGeometry4);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] refGeometry1 : 1번째 CRavidPoint 점 x, y 값 (Reference)
	* param[in] refGeometry2 : 2번째 CRavidPoint 점 x, y 값 (Reference)
	* param[in] refGeometry3 : 3번째 CRavidPoint 점 x, y 값 (Reference)
	* param[in] refGeometry4 : 4번째 CRavidPoint 점 x, y 값 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetQuadrangle(_In_ CRavidPoint<float>& refGeometry1, _In_ CRavidPoint<float>& refGeometry2, _In_ CRavidPoint<float>& refGeometry3, _In_ CRavidPoint<float>& refGeometry4);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] pGeometry1 : 1번째 CRavidPoint 점 x, y 값 (Pointer)
	* param[in] pGeometry2 : 2번째 CRavidPoint 점 x, y 값 (Pointer)
	* param[in] pGeometry3 : 3번째 CRavidPoint 점 x, y 값 (Pointer)
	* param[in] pGeometry4 : 4번째 CRavidPoint 점 x, y 값 (Pointer)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetQuadrangle(_In_ CRavidPoint<float>* pGeometry1, _In_ CRavidPoint<float>* pGeometry2, _In_ CRavidPoint<float>* pGeometry3, _In_ CRavidPoint<float>* pGeometry4);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] refGeometry1 : 1번째 CRavidPoint 점 x, y 값 (Reference)
	* param[in] refGeometry2 : 2번째 CRavidPoint 점 x, y 값 (Reference)
	* param[in] refGeometry3 : 3번째 CRavidPoint 점 x, y 값 (Reference)
	* param[in] refGeometry4 : 4번째 CRavidPoint 점 x, y 값 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetQuadrangle(_In_ CRavidPoint<double>& refGeometry1, _In_ CRavidPoint<double>& refGeometry2, _In_ CRavidPoint<double>& refGeometry3, _In_ CRavidPoint<double>& refGeometry4);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] pGeometry1 : 1번째 CRavidPoint 점 x, y 값 (Pointer)
	* param[in] pGeometry2 : 2번째 CRavidPoint 점 x, y 값 (Pointer)
	* param[in] pGeometry3 : 3번째 CRavidPoint 점 x, y 값 (Pointer)
	* param[in] pGeometry4 : 4번째 CRavidPoint 점 x, y 값 (Pointer)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetQuadrangle(_In_ CRavidPoint<double>* pGeometry1, _In_ CRavidPoint<double>* pGeometry2, _In_ CRavidPoint<double>* pGeometry3, _In_ CRavidPoint<double>* pGeometry4);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] refGeometry : RECT 직사각형 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetQuadrangle(_In_ RECT& refGeometry);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] pGeometry : RECT 직사각형 (Pointer)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetQuadrangle(_In_ RECT* pGeometry);
	/*
	* 입력된 객체의 정보를 이용하여 4 좌표를 계산하여 설정합니다.
	*
	* param[in] refGeometry : CRavidRect 직사각형 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetQuadrangle(_In_ CRavidRect<int>& refGeometry);
	/*
	* 입력된 객체의 정보를 이용하여 4 좌표를 계산하여 설정합니다.
	*
	* param[in] pGeometry : CRavidRect 직사각형 (Pointer)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetQuadrangle(_In_ CRavidRect<int>* pGeometry);
	/*
	* 입력된 객체의 정보를 이용하여 4 좌표를 계산하여 설정합니다.
	*
	* param[in] refGeometry : CRavidRect 직사각형 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetQuadrangle(_In_ CRavidRect<long long>& refGeometry);
	/*
	* 입력된 객체의 정보를 이용하여 4 좌표를 계산하여 설정합니다.
	*
	* param[in] pGeometry : CRavidRect 직사각형 (Pointer)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetQuadrangle(_In_ CRavidRect<long long>* pGeometry);
	/*
	* 입력된 객체의 정보를 이용하여 4 좌표를 계산하여 설정합니다.
	*
	* param[in] refGeometry : CRavidRect 직사각형 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetQuadrangle(_In_ CRavidRect<float>& refGeometry);
	/*
	* 입력된 객체의 정보를 이용하여 4 좌표를 계산하여 설정합니다.
	*
	* param[in] pGeometry : CRavidRect 직사각형 (Pointer)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetQuadrangle(_In_ CRavidRect<float>* pGeometry);
	/*
	* 입력된 객체의 정보를 이용하여 4 좌표를 계산하여 설정합니다.
	*
	* param[in] refGeometry : CRavidRect 직사각형 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetQuadrangle(_In_ CRavidRect<double>& refGeometry);
	/*
	* 입력된 객체의 정보를 이용하여 4 좌표를 계산하여 설정합니다.
	*
	* param[in] pGeometry : CRavidRect 직사각형 (Pointer)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetQuadrangle(_In_ CRavidRect<double>* pGeometry);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] refGeometry : 사각형의 위치 값 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetQuadrangle(_In_ CRavidQuadrangle<int>& refGeometry);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] pGeometry : 사각형의 위치 값 (Pointer)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetQuadrangle(_In_ CRavidQuadrangle<int>* pGeometry);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] refGeometry : 사각형의 위치 값 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetQuadrangle(_In_ CRavidQuadrangle<long long>& refGeometry);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] pGeometry : 사각형의 위치 값 (Pointer)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetQuadrangle(_In_ CRavidQuadrangle<long long>* pGeometry);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] refGeometry : 사각형의 위치 값 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetQuadrangle(_In_ CRavidQuadrangle<float>& refGeometry);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] pGeometry : 사각형의 위치 값 (Pointer)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetQuadrangle(_In_ CRavidQuadrangle<float>* pGeometry);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] refGeometry : 사각형의 위치 값 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetQuadrangle(_In_ CRavidQuadrangle<double>& refGeometry);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] pGeometry : 사각형의 위치 값 (Pointer)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetQuadrangle(_In_ CRavidQuadrangle<double>* pGeometry);
#pragma endregion