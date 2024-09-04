public:

#pragma region [SetExclusiveRegion]
	/**
	* 현재 객체와 상대 객체의 충돌 판정 계산합니다.
	* 
	* param[in] point : x, y 좌표 (Reference)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidPoint<int>& point);
	/**
	* 현재 객체와 상대 객체의 충돌 판정 계산합니다.
	* 
	* param[in] pPoint : x, y 좌표 (Pointer)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidPoint<int>* pPoint);
	/**
	* 현재 객체와 상대 객체의 충돌 판정 계산합니다.
	* 
	* param[in] point : x, y 좌표 (Reference)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidPoint<long long>& point);
	/**
	* 현재 객체와 상대 객체의 충돌 판정 계산합니다.
	* 
	* param[in] pPoint : x, y 좌표 (Pointer)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidPoint<long long>* pPoint);
	/**
	* 현재 객체와 상대 객체의 충돌 판정 계산합니다.
	* 
	* param[in] point : x, y 좌표 (Reference)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidPoint<float>& point);
	/**
	* 현재 객체와 상대 객체의 충돌 판정 계산합니다.
	* 
	* param[in] pPoint : x, y 좌표 (Pointer)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidPoint<float>* pPoint);
	/**
	* 현재 객체와 상대 객체의 충돌 판정 계산합니다.
	* 
	* param[in] point : x, y 좌표 (Reference)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidPoint<double>& point);
	/**
	* 현재 객체와 상대 객체의 충돌 판정 계산합니다.
	* 
	* param[in] pPoint : x, y 좌표 (Pointer)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidPoint<double>* pPoint);
	/**
	* 현재 객체와 상대 객체의 충돌 판정 계산합니다.
	* 
	* param[in] line : 직선 또는 선분 (Reference)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidLine<int>& line);
	/**
	* 현재 객체와 상대 객체의 충돌 판정 계산합니다.
	* 
	* param[in] pLine : 직선 또는 선분 (Pointer)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidLine<int>* pLine);
	/**
	* 현재 객체와 상대 객체의 충돌 판정 계산합니다.
	* 
	* param[in] line : 직선 또는 선분 (Reference)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidLine<long long>& line);
	/**
	* 현재 객체와 상대 객체의 충돌 판정 계산합니다.
	* 
	* param[in] pLine : 직선 또는 선분 (Pointer)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidLine<long long>* pLine);
	/**
	* 현재 객체와 상대 객체의 충돌 판정 계산합니다.
	* 
	* param[in] line : 직선 또는 선분 (Reference)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidLine<float>& line);
	/**
	* 현재 객체와 상대 객체의 충돌 판정 계산합니다.
	* 
	* param[in] pLine : 직선 또는 선분 (Pointer)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidLine<float>* pLine);
	/**
	* 현재 객체와 상대 객체의 충돌 판정 계산합니다.
	* 
	* param[in] line : 직선 또는 선분 (Reference)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidLine<double>& line);
	/**
	* 현재 객체와 상대 객체의 충돌 판정 계산합니다.
	* 
	* param[in] pLine : 직선 또는 선분 (Pointer)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidLine<double>* pLine);
	/**
	* 현재 객체와 상대 객체의 충돌 및 내부에 위치 판정을 계산합니다.
	* 
	* param[in] rect : 직사각형 (Reference)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidRect<int>& rect);
	/**
	* 현재 객체와 상대 객체의 충돌 및 내부에 위치 판정을 계산합니다.
	* 
	* param[in] pRect : 직사각형 (Pointer)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidRect<int>* pRect);
	/**
	* 현재 객체와 상대 객체의 충돌 및 내부에 위치 판정을 계산합니다.
	* 
	* param[in] rect : 직사각형 (Reference)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidRect<long long>& rect);
	/**
	* 현재 객체와 상대 객체의 충돌 및 내부에 위치 판정을 계산합니다.
	* 
	* param[in] pRect : 직사각형 (Pointer)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidRect<long long>* pRect);
	/**
	* 현재 객체와 상대 객체의 충돌 및 내부에 위치 판정을 계산합니다.
	* 
	* param[in] rect : 직사각형 (Reference)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidRect<float>& rect);
	/**
	* 현재 객체와 상대 객체의 충돌 및 내부에 위치 판정을 계산합니다.
	* 
	* param[in] pRect : 직사각형 (Pointer)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidRect<float>* pRect);
	/**
	* 현재 객체와 상대 객체의 충돌 및 내부에 위치 판정을 계산합니다.
	* 
	* param[in] rect : 직사각형 (Reference)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidRect<double>& rect);
	/**
	* 현재 객체와 상대 객체의 충돌 및 내부에 위치 판정을 계산합니다.
	* 
	* param[in] pRect : 직사각형 (Pointer)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidRect<double>* pRect);
	/**
	* 현재 객체와 상대 객체의 충돌 및 내부에 위치 판정을 계산합니다.
	* 
	* param[in] quad : 사각형 (Reference)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidQuadrangle<int>& quad);
	/**
	* 현재 객체와 상대 객체의 충돌 및 내부에 위치 판정을 계산합니다.
	* 
	* param[in] pQuad : 사각형 (Pointer)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidQuadrangle<int>* pQuad);
	/**
	* 현재 객체와 상대 객체의 충돌 및 내부에 위치 판정을 계산합니다.
	* 
	* param[in] quad : 사각형 (Reference)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidQuadrangle<long long>& quad);
	/**
	* 현재 객체와 상대 객체의 충돌 및 내부에 위치 판정을 계산합니다.
	* 
	* param[in] pQuad : 사각형 (Pointer)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidQuadrangle<long long>* pQuad);
	/**
	* 현재 객체와 상대 객체의 충돌 및 내부에 위치 판정을 계산합니다.
	* 
	* param[in] quad : 사각형 (Reference)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidQuadrangle<float>& quad);
	/**
	* 현재 객체와 상대 객체의 충돌 및 내부에 위치 판정을 계산합니다.
	* 
	* param[in] pQuad : 사각형 (Pointer)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidQuadrangle<float>* pQuad);
	/**
	* 현재 객체와 상대 객체의 충돌 및 내부에 위치 판정을 계산합니다.
	* 
	* param[in] quad : 사각형 (Reference)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidQuadrangle<double>& quad);
	/**
	* 현재 객체와 상대 객체의 충돌 및 내부에 위치 판정을 계산합니다.
	* 
	* param[in] pQuad : 사각형 (Pointer)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidQuadrangle<double>* pQuad);
	/**
	* 현재 객체와 상대 객체의 충돌 및 내부에 위치 판정을 계산합니다.
	* 
	* param[in] cir : 원 (Reference)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidCircle<int>& cir);
	/**
	* 현재 객체와 상대 객체의 충돌 및 내부에 위치 판정을 계산합니다.
	* 
	* param[in] pCir : 원 (Pointer)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidCircle<int>* pCir);
	/**
	* 현재 객체와 상대 객체의 충돌 및 내부에 위치 판정을 계산합니다.
	* 
	* param[in] cir : 원 (Reference)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidCircle<long long>& cir);
	/**
	* 현재 객체와 상대 객체의 충돌 및 내부에 위치 판정을 계산합니다.
	* 
	* param[in] pCir : 원 (Pointer)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidCircle<long long>* pCir);
	/**
	* 현재 객체와 상대 객체의 충돌 및 내부에 위치 판정을 계산합니다.
	* 
	* param[in] cir : 원 (Reference)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidCircle<float>& cir);
	/**
	* 현재 객체와 상대 객체의 충돌 및 내부에 위치 판정을 계산합니다.
	* 
	* param[in] pCir : 원 (Pointer)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidCircle<float>* pCir);
	/**
	* 현재 객체와 상대 객체의 충돌 및 내부에 위치 판정을 계산합니다.
	* 
	* param[in] cir : 원 (Reference)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidCircle<double>& cir);
	/**
	* 현재 객체와 상대 객체의 충돌 및 내부에 위치 판정을 계산합니다.
	* 
	* param[in] pCir : 원 (Pointer)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidCircle<double>* pCir);
	/**
	* 현재 객체와 상대 객체의 충돌 및 내부에 위치 판정을 계산합니다.
	* 
	* param[in] ell : 타원 (Reference)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidEllipse<int>& ell);
	/**
	* 현재 객체와 상대 객체의 충돌 및 내부에 위치 판정을 계산합니다.
	* 
	* param[in] pEll : 타원 (Pointer)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidEllipse<int>* pEll);
	/**
	* 현재 객체와 상대 객체의 충돌 및 내부에 위치 판정을 계산합니다.
	* 
	* param[in] ell : 타원 (Reference)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidEllipse<long long>& ell);
	/**
	* 현재 객체와 상대 객체의 충돌 및 내부에 위치 판정을 계산합니다.
	* 
	* param[in] pEll : 타원 (Pointer)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidEllipse<long long>* pEll);
	/**
	* 현재 객체와 상대 객체의 충돌 및 내부에 위치 판정을 계산합니다.
	* 
	* param[in] ell : 타원 (Reference)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidEllipse<float>& ell);
	/**
	* 현재 객체와 상대 객체의 충돌 및 내부에 위치 판정을 계산합니다.
	* 
	* param[in] pEll : 타원 (Pointer)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidEllipse<float>* pEll);
	/**
	* 현재 객체와 상대 객체의 충돌 및 내부에 위치 판정을 계산합니다.
	* 
	* param[in] ell : 타원 (Reference)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidEllipse<double>& ell);
	/**
	* 현재 객체와 상대 객체의 충돌 및 내부에 위치 판정을 계산합니다.
	* 
	* param[in] pEll : 타원 (Pointer)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidEllipse<double>* pEll);
	/**
	* 현재 객체와 상대 객체의 충돌 및 내부에 위치 판정을 계산합니다.
	* 
	* param[in] rpg : 폴리곤 (Reference)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidPolygon& rpg);
	/**
	* 현재 객체와 상대 객체의 충돌 및 내부에 위치 판정을 계산합니다.
	* 
	* param[in] pRpg : 폴리곤 (Pointer)
	*
	* return value : 충돌하지 않으면 false 를 반환합니다.
	*/
	bool DoesIntersect(_In_ CRavidPolygon* pRpg);
#pragma endregion