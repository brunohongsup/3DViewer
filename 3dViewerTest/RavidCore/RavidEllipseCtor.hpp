public:

#pragma region [CRavidEllipse]
	CRavidEllipse(_In_ Ravid::Algorithms::CRavidImage& ri, _In_ double angle = 0., _In_ double startDeg = 0., _In_ double deltaDeg = 360., _In_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidEllipse(_In_ T x, _In_ T y, _In_ Ravid::Algorithms::CRavidImage& ri, _In_ double angle = 0., _In_ double startDeg = 0., _In_ double deltaDeg = 360., _In_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidEllipse(_In_ const POINT& point, _In_ Ravid::Algorithms::CRavidImage& ri, _In_ double angle = 0., _In_ double startDeg = 0., _In_ double deltaDeg = 360., _In_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidEllipse(_In_ POINT* pPoint, _In_ Ravid::Algorithms::CRavidImage& ri, _In_ double angle = 0., _In_ double startDeg = 0., _In_ double deltaDeg = 360., _In_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidEllipse(_In_ const SIZE& size, _In_ Ravid::Algorithms::CRavidImage& ri, _In_ double angle = 0., _In_ double startDeg = 0., _In_ double deltaDeg = 360., _In_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidEllipse(_In_ SIZE* pSize, _In_ Ravid::Algorithms::CRavidImage& ri, _In_ double angle = 0., _In_ double startDeg = 0., _In_ double deltaDeg = 360., _In_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidEllipse(_In_ const CRavidPoint<int>& point, _In_ Ravid::Algorithms::CRavidImage& ri, _In_ double angle = 0., _In_ double startDeg = 0., _In_ double deltaDeg = 360., _In_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidEllipse(_In_ CRavidPoint<int>* pPoint, _In_ Ravid::Algorithms::CRavidImage& ri, _In_ double angle = 0., _In_ double startDeg = 0., _In_ double deltaDeg = 360., _In_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidEllipse(_In_ const CRavidPoint<long long>& point, _In_ Ravid::Algorithms::CRavidImage& ri, _In_ double angle = 0., _In_ double startDeg = 0., _In_ double deltaDeg = 360., _In_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidEllipse(_In_ CRavidPoint<long long>* pPoint, _In_ Ravid::Algorithms::CRavidImage& ri, _In_ double angle = 0., _In_ double startDeg = 0., _In_ double deltaDeg = 360., _In_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidEllipse(_In_ const CRavidPoint<float>& point, _In_ Ravid::Algorithms::CRavidImage& ri, _In_ double angle = 0., _In_ double startDeg = 0., _In_ double deltaDeg = 360., _In_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidEllipse(_In_ CRavidPoint<float>* pPoint, _In_ Ravid::Algorithms::CRavidImage& ri, _In_ double angle = 0., _In_ double startDeg = 0., _In_ double deltaDeg = 360., _In_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidEllipse(_In_ const CRavidPoint<double>& point, _In_ Ravid::Algorithms::CRavidImage& ri, _In_ double angle = 0., _In_ double startDeg = 0., _In_ double deltaDeg = 360., _In_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidEllipse(_In_ CRavidPoint<double>* pPoint, _In_ Ravid::Algorithms::CRavidImage& ri, _In_ double angle = 0., _In_ double startDeg = 0., _In_ double deltaDeg = 360., _In_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);

	CRavidEllipse(_In_ T x, _In_ T y, _In_ T radius1, _In_ T radius2, _In_ double angle = 0., _In_ double startDeg = 0., _In_ double deltaDeg = 360., _In_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidEllipse(_In_ const POINT& point, _In_ T radius1, _In_ T radius2, _In_ double angle = 0., _In_ double startDeg = 0., _In_ double deltaDeg = 360., _In_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidEllipse(_In_ POINT* pPoint, _In_ T radius1, _In_ T radius2, _In_ double angle = 0., _In_ double startDeg = 0., _In_ double deltaDeg = 360., _In_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidEllipse(_In_ const SIZE& size, _In_ T radius1, _In_ T radius2, _In_ double angle = 0., _In_ double startDeg = 0., _In_ double deltaDeg = 360., _In_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidEllipse(_In_ SIZE* pSize, _In_ T radius1, _In_ T radius2, _In_ double angle = 0., _In_ double startDeg = 0., _In_ double deltaDeg = 360., _In_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidEllipse(_In_ const CRavidPoint<int>& point, _In_ T radius1, _In_ T radius2, _In_ double angle = 0., _In_ double startDeg = 0., _In_ double deltaDeg = 360., _In_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidEllipse(_In_ CRavidPoint<int>* pPoint, _In_ T radius1, _In_ T radius2, _In_ double angle = 0., _In_ double startDeg = 0., _In_ double deltaDeg = 360., _In_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidEllipse(_In_ const CRavidPoint<long long>& point, _In_ T radius1, _In_ T radius2, _In_ double angle = 0., _In_ double startDeg = 0., _In_ double deltaDeg = 360., _In_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidEllipse(_In_ CRavidPoint<long long>* pPoint, _In_ T radius1, _In_ T radius2, _In_ double angle = 0., _In_ double startDeg = 0., _In_ double deltaDeg = 360., _In_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidEllipse(_In_ const CRavidPoint<float>& point, _In_ T radius1, _In_ T radius2, _In_ double angle = 0., _In_ double startDeg = 0., _In_ double deltaDeg = 360., _In_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidEllipse(_In_ CRavidPoint<float>* pPoint, _In_ T radius1, _In_ T radius2, _In_ double angle = 0., _In_ double startDeg = 0., _In_ double deltaDeg = 360., _In_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidEllipse(_In_ const CRavidPoint<double>& point, _In_ T radius1, _In_ T radius2, _In_ double angle = 0., _In_ double startDeg = 0., _In_ double deltaDeg = 360., _In_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidEllipse(_In_ CRavidPoint<double>* pPoint, _In_ T radius1, _In_ T radius2, _In_ double angle = 0., _In_ double startDeg = 0., _In_ double deltaDeg = 360., _In_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidEllipse(_In_ const RECT& rect, _In_ double startDeg = 0., _In_ double deltaDeg = 360., _In_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidEllipse(_In_ RECT* pRect, _In_ double startDeg = 0., _In_ double deltaDeg = 360., _In_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidEllipse(_In_ const CRavidRect<int>& rect, _In_ double startDeg = 0., _In_ double deltaDeg = 360., _In_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidEllipse(_In_ CRavidRect<int>* pRect, _In_ double startDeg = 0., _In_ double deltaDeg = 360., _In_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidEllipse(_In_ const CRavidRect<long long>& rect, _In_ double startDeg = 0., _In_ double deltaDeg = 360., _In_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidEllipse(_In_ CRavidRect<long long>* pRect, _In_ double startDeg = 0., _In_ double deltaDeg = 360., _In_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidEllipse(_In_ const CRavidRect<float>& rect, _In_ double startDeg = 0., _In_ double deltaDeg = 360., _In_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidEllipse(_In_ CRavidRect<float>* pRect, _In_ double startDeg = 0., _In_ double deltaDeg = 360., _In_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidEllipse(_In_ const CRavidRect<double>& rect, _In_ double startDeg = 0., _In_ double deltaDeg = 360., _In_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidEllipse(_In_ CRavidRect<double>* pRect, _In_ double startDeg = 0., _In_ double deltaDeg = 360., _In_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidEllipse(_In_ const CRavidCircle<int>& cir);
	CRavidEllipse(_In_ CRavidCircle<int>* pCir);
	CRavidEllipse(_In_ const CRavidCircle<long long>& cir);
	CRavidEllipse(_In_ CRavidCircle<long long>* pCir);
	CRavidEllipse(_In_ const CRavidCircle<float>& cir);
	CRavidEllipse(_In_ CRavidCircle<float>* pCir);
	CRavidEllipse(_In_ const CRavidCircle<double>& cir);
	CRavidEllipse(_In_ CRavidCircle<double>* pCir);
	CRavidEllipse(_In_ const CRavidEllipse<int>& ell);
	CRavidEllipse(_In_ CRavidEllipse<int>* pEll);
	CRavidEllipse(_In_ const CRavidEllipse<long long>& ell);
	CRavidEllipse(_In_ CRavidEllipse<long long>* pEll);
	CRavidEllipse(_In_ const CRavidEllipse<float>& ell);
	CRavidEllipse(_In_ CRavidEllipse<float>* pEll);
	CRavidEllipse(_In_ const CRavidEllipse<double>& ell);
	CRavidEllipse(_In_ CRavidEllipse<double>* pEll);

	CRavidEllipse(_In_ T x1, _In_ T y1, _In_ T x2, _In_ T y2, _In_ T x3, _In_ T y3, _In_ T x4, _In_ T y4, _In_ T x5, _In_ T y5, _In_ double startDeg = 0., _In_ double deltaDeg = 360., _In_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidEllipse(_In_ const CRavidPoint<int>& rpPoint1, _In_ const CRavidPoint<int>& rpPoint2, _In_ const CRavidPoint<int>& rpPoint3, _In_ const CRavidPoint<int>& rpPoint4, _In_ const CRavidPoint<int>& rpPoint5, _In_ double startDeg = 0., _In_ double deltaDeg = 360., _In_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidEllipse(_In_ const CRavidPoint<long long>& rpPoint1, _In_ const CRavidPoint<long long>& rpPoint2, _In_ const CRavidPoint<long long>& rpPoint3, _In_ const CRavidPoint<long long>& rpPoint4, _In_ const CRavidPoint<long long>& rpPoint5, _In_ double startDeg = 0., _In_ double deltaDeg = 360., _In_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidEllipse(_In_ const CRavidPoint<float>& rpPoint1, _In_ const CRavidPoint<float>& rpPoint2, _In_ const CRavidPoint<float>& rpPoint3, _In_ const CRavidPoint<float>& rpPoint4, _In_ const CRavidPoint<float>& rpPoint5, _In_ double startDeg = 0., _In_ double deltaDeg = 360., _In_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidEllipse(_In_ const CRavidPoint<double>& rpPoint1, _In_ const CRavidPoint<double>& rpPoint2, _In_ const CRavidPoint<double>& rpPoint3, _In_ const CRavidPoint<double>& rpPoint4, _In_ const CRavidPoint<double>& rpPoint5, _In_ double startDeg = 0., _In_ double deltaDeg = 360., _In_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidEllipse(_In_ CRavidPoint<int>* pRpPoint1, _In_ CRavidPoint<int>* pRpPoint2, _In_ CRavidPoint<int>* pRpPoint3, _In_ CRavidPoint<int>* pRpPoint4, _In_ CRavidPoint<int>* pRpPoint5, _In_ double startDeg = 0., _In_ double deltaDeg = 360., _In_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidEllipse(_In_ CRavidPoint<long long>* pRpPoint1, _In_ CRavidPoint<long long>* pRpPoint2, _In_ CRavidPoint<long long>* pRpPoint3, _In_ CRavidPoint<long long>* pRpPoint4, _In_ CRavidPoint<long long>* pRpPoint5, _In_ double startDeg = 0., _In_ double deltaDeg = 360., _In_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidEllipse(_In_ CRavidPoint<float>* pRpPoint1, _In_ CRavidPoint<float>* pRpPoint2, _In_ CRavidPoint<float>* pRpPoint3, _In_ CRavidPoint<float>* pRpPoint4, _In_ CRavidPoint<float>* pRpPoint5, _In_ double startDeg = 0., _In_ double deltaDeg = 360., _In_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidEllipse(_In_ CRavidPoint<double>* pRpPoint1, _In_ CRavidPoint<double>* pRpPoint2, _In_ CRavidPoint<double>* pRpPoint3, _In_ CRavidPoint<double>* pRpPoint4, _In_ CRavidPoint<double>* pRpPoint5, _In_ double startDeg = 0., _In_ double deltaDeg = 360., _In_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
#pragma endregion

#pragma region [SetEllipse]
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] x : 객체 중심 x 값
	* param[in] y : 객체 중심 y 값
	* param[in] radius1 : x축 반지름 값
	* param[in] radius2 : y축 반지름 값
	* param[in_opt] angle : 객체 회전 각도 값
	* param[in_opt] startDeg : 객체 시작 위치 각도 값
	* param[in_opt] deltaDeg : 객체 시작 각도로부터 표현 각도 값
	* param[in_opt] eRACM : 끝점 처리 방식
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetEllipse(_In_ int x, _In_ int y, _In_ int radius1, _In_ int radius2, _In_opt_ double angle = 0.0, _In_opt_ double startDeg = 0.0, _In_opt_ double deltaDeg = 360.0, _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] x : 객체 중심 x 값
	* param[in] y : 객체 중심 y 값
	* param[in] radius1 : x축 반지름 값
	* param[in] radius2 : y축 반지름 값
	* param[in_opt] angle : 객체 회전 각도 값
	* param[in_opt] startDeg : 객체 시작 위치 각도 값
	* param[in_opt] deltaDeg : 객체 시작 각도로부터 표현 각도 값
	* param[in_opt] eRACM : 끝점 처리 방식
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetEllipse(_In_ long long x, _In_ long long y, _In_ long long radius1, _In_ long long radius2, _In_opt_ double angle = 0.0, _In_opt_ double startDeg = 0., _In_opt_ double deltaDeg = 360.0, _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] x : 객체 중심 x 값
	* param[in] y : 객체 중심 y 값
	* param[in] radius1 : x축 반지름 값
	* param[in] radius2 : y축 반지름 값
	* param[in_opt] angle : 객체 회전 각도 값
	* param[in_opt] startDeg : 객체 시작 위치 각도 값
	* param[in_opt] deltaDeg : 객체 시작 각도로부터 표현 각도 값
	* param[in_opt] eRACM : 끝점 처리 방식
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetEllipse(_In_ float x, _In_ float y, _In_ float radius1, _In_ float radius2, _In_opt_ double angle = 0.0, _In_opt_ double startDeg = 0.0, _In_opt_ double deltaDeg = 360.0, _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] x : 객체 중심 x 값
	* param[in] y : 객체 중심 y 값
	* param[in] radius1 : x축 반지름 값
	* param[in] radius2 : y축 반지름 값
	* param[in_opt] angle : 객체 회전 각도 값
	* param[in_opt] startDeg : 객체 시작 위치 각도 값
	* param[in_opt] deltaDeg : 객체 시작 각도로부터 표현 각도 값
	* param[in_opt] eRACM : 끝점 처리 방식
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetEllipse(_In_ double x, _In_ double y, _In_ double radius1, _In_ double radius2, _In_opt_ double angle = 0.0, _In_opt_ double startDeg = 0.0, _In_opt_ double deltaDeg = 360.0, _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] refGeometry : CRavidEllipse 타원 정보 (Reference)
	* 
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetEllipse(_In_ CRavidEllipse<int>& refGeometry);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] pGeometry : CRavidEllipse 타원 정보 (Pointer)
	* 
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetEllipse(_In_ CRavidEllipse<int>* pGeometry);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] refGeometry : CRavidEllipse 타원 정보 (Reference)
	* 
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetEllipse(_In_ CRavidEllipse<long long>& refGeometry);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] pGeometry : CRavidEllipse 타원 정보 (Pointer)
	* 
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetEllipse(_In_ CRavidEllipse<long long>* pGeometry);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] refGeometry : CRavidEllipse 타원 정보 (Reference)
	* 
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetEllipse(_In_ CRavidEllipse<float>& refGeometry);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] pGeometry : CRavidEllipse 타원 정보 (Pointer)
	* 
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetEllipse(_In_ CRavidEllipse<float>* pGeometry);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] refGeometry : CRavidEllipse 타원 정보 (Reference)
	* 
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetEllipse(_In_ CRavidEllipse<double>& refGeometry);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] pGeometry : CRavidEllipse 타원 정보 (Pointer)
	* 
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetEllipse(_In_ CRavidEllipse<double>* pGeometry);
	/*
	* 5 점을 이용하여 타원을 추정합니다.
	*
	* param[in] x1 : 1번째 x 값
	* param[in] y1 : 1번째 y 값
	* param[in] x2 : 2번째 x 값
	* param[in] y2 : 2번째 y 값
	* param[in] x3 : 3번째 x 값
	* param[in] y3 : 3번째 y 값
	* param[in] x4 : 4번째 x 값
	* param[in] y4 : 4번째 y 값
	* param[in] x5 : 5번째 x 값
	* param[in] y5 : 5번째 y 값
	* param[in_opt] startDeg : 객체 시작 위치 각도 값
	* param[in_opt] deltaDeg : 객체 시작 각도로부터 표현 각도 값
	* param[in_opt] eRACM : 끝점 처리 방식
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetEllipse(_In_ int x1, _In_ int y1, _In_ int x2, _In_ int y2, _In_ int x3, _In_ int y3, _In_ int x4, _In_ int y4, _In_ int x5, _In_ int y5, _In_opt_ double startDeg = 0, _In_opt_ double deltaDeg = 360, _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	/*
	* 5 점을 이용하여 타원을 추정합니다.
	*
	* param[in] x1 : 1번째 x 값
	* param[in] y1 : 1번째 y 값
	* param[in] x2 : 2번째 x 값
	* param[in] y2 : 2번째 y 값
	* param[in] x3 : 3번째 x 값
	* param[in] y3 : 3번째 y 값
	* param[in] x4 : 4번째 x 값
	* param[in] y4 : 4번째 y 값
	* param[in] x5 : 5번째 x 값
	* param[in] y5 : 5번째 y 값
	* param[in_opt] startDeg : 객체 시작 위치 각도 값
	* param[in_opt] deltaDeg : 객체 시작 각도로부터 표현 각도 값
	* param[in_opt] eRACM : 끝점 처리 방식
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetEllipse(_In_ long long x1, _In_ long long y1, _In_ long long x2, _In_ long long y2, _In_ long long x3, _In_ long long y3, _In_ long long x4, _In_ long long y4, _In_ long long x5, _In_ long long y5, _In_opt_ double startDeg = 0, _In_opt_ double deltaDeg = 360, _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	/*
	* 5 점을 이용하여 타원을 추정합니다.
	*
	* param[in] x1 : 1번째 x 값
	* param[in] y1 : 1번째 y 값
	* param[in] x2 : 2번째 x 값
	* param[in] y2 : 2번째 y 값
	* param[in] x3 : 3번째 x 값
	* param[in] y3 : 3번째 y 값
	* param[in] x4 : 4번째 x 값
	* param[in] y4 : 4번째 y 값
	* param[in] x5 : 5번째 x 값
	* param[in] y5 : 5번째 y 값
	* param[in_opt] startDeg : 객체 시작 위치 각도 값
	* param[in_opt] deltaDeg : 객체 시작 각도로부터 표현 각도 값
	* param[in_opt] eRACM : 끝점 처리 방식
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetEllipse(_In_ float x1, _In_ float y1, _In_ float x2, _In_ float y2, _In_ float x3, _In_ float y3, _In_ float x4, _In_ float y4, _In_ float x5, _In_ float y5, _In_opt_ double startDeg = 0, _In_opt_ double deltaDeg = 360, _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	/*
	* 5 점을 이용하여 타원을 추정합니다.
	*
	* param[in] x1 : 1번째 x 값
	* param[in] y1 : 1번째 y 값
	* param[in] x2 : 2번째 x 값
	* param[in] y2 : 2번째 y 값
	* param[in] x3 : 3번째 x 값
	* param[in] y3 : 3번째 y 값
	* param[in] x4 : 4번째 x 값
	* param[in] y4 : 4번째 y 값
	* param[in] x5 : 5번째 x 값
	* param[in] y5 : 5번째 y 값
	* param[in_opt] startDeg : 객체 시작 위치 각도 값
	* param[in_opt] deltaDeg : 객체 시작 각도로부터 표현 각도 값
	* param[in_opt] eRACM : 끝점 처리 방식
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetEllipse(_In_ double x1, _In_ double y1, _In_ double x2, _In_ double y2, _In_ double x3, _In_ double y3, _In_ double x4, _In_ double y4, _In_ double x5, _In_ double y5, _In_opt_ double startDeg = 0, _In_opt_ double deltaDeg = 360, _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	/*
	* 5 점을 이용하여 타원을 추정합니다.
	*
	* param[in] refGeometry1 : CRavidPoint 1번째 x, y 값 (Reference)
	* param[in] refGeometry2 : CRavidPoint 2번째 x, y 값 (Reference)
	* param[in] refGeometry3 : CRavidPoint 3번째 x, y 값 (Reference)
	* param[in] refGeometry4 : CRavidPoint 4번째 x, y 값 (Reference)
	* param[in] refGeometry5 : CRavidPoint 5번째 x, y 값 (Reference)
	* param[in_opt] startDeg : 객체 시작 위치 각도 값
	* param[in_opt] deltaDeg : 객체 시작 각도로부터 표현 각도 값
	* param[in_opt] eRACM : 끝점 처리 방식
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetEllipse(_In_ CRavidPoint<int>& refGeometry1, _In_ CRavidPoint<int>& refGeometry2, _In_ CRavidPoint<int>& refGeometry3, _In_ CRavidPoint<int>& refGeometry4, _In_ CRavidPoint<int>& refGeometry5, _In_opt_ double startDeg = 0, _In_opt_ double deltaDeg = 360, _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	/*
	* 5 점을 이용하여 타원을 추정합니다.
	*
	* param[in] pGeometry1 : CRavidPoint 1번째 x, y 값 (Pointer)
	* param[in] pGeometry2 : CRavidPoint 2번째 x, y 값 (Pointer)
	* param[in] pGeometry3 : CRavidPoint 3번째 x, y 값 (Pointer)
	* param[in] pGeometry4 : CRavidPoint 4번째 x, y 값 (Pointer)
	* param[in] pGeometry5 : CRavidPoint 5번째 x, y 값 (Pointer)
	* param[in_opt] startDeg : 객체 시작 위치 각도 값
	* param[in_opt] deltaDeg : 객체 시작 각도로부터 표현 각도 값
	* param[in_opt] eRACM : 끝점 처리 방식
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetEllipse(_In_ CRavidPoint<int>* pGeometry1, _In_ CRavidPoint<int>* pGeometry2, _In_ CRavidPoint<int>* pGeometry3, _In_ CRavidPoint<int>* pGeometry4, _In_ CRavidPoint<int>* pGeometry5, _In_opt_ double startDeg = 0, _In_opt_ double deltaDeg = 360, _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	/*
	* 5 점을 이용하여 타원을 추정합니다.
	*
	* param[in] refGeometry1 : CRavidPoint 1번째 x, y 값 (Reference)
	* param[in] refGeometry2 : CRavidPoint 2번째 x, y 값 (Reference)
	* param[in] refGeometry3 : CRavidPoint 3번째 x, y 값 (Reference)
	* param[in] refGeometry4 : CRavidPoint 4번째 x, y 값 (Reference)
	* param[in] refGeometry5 : CRavidPoint 5번째 x, y 값 (Reference)
	* param[in_opt] startDeg : 객체 시작 위치 각도 값
	* param[in_opt] deltaDeg : 객체 시작 각도로부터 표현 각도 값
	* param[in_opt] eRACM : 끝점 처리 방식
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetEllipse(_In_ CRavidPoint<long long>& refGeometry1, _In_ CRavidPoint<long long>& refGeometry2, _In_ CRavidPoint<long long>& refGeometry3, _In_ CRavidPoint<long long>& refGeometry4, _In_ CRavidPoint<long long>& refGeometry5, _In_opt_ double startDeg = 0, _In_opt_ double deltaDeg = 360, _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	/*
	* 5 점을 이용하여 타원을 추정합니다.
	*
	* param[in] pGeometry1 : CRavidPoint 1번째 x, y 값 (Pointer)
	* param[in] pGeometry2 : CRavidPoint 2번째 x, y 값 (Pointer)
	* param[in] pGeometry3 : CRavidPoint 3번째 x, y 값 (Pointer)
	* param[in] pGeometry4 : CRavidPoint 4번째 x, y 값 (Pointer)
	* param[in] pGeometry5 : CRavidPoint 5번째 x, y 값 (Pointer)
	* param[in_opt] startDeg : 객체 시작 위치 각도 값
	* param[in_opt] deltaDeg : 객체 시작 각도로부터 표현 각도 값
	* param[in_opt] eRACM : 끝점 처리 방식
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetEllipse(_In_ CRavidPoint<long long>* pGeometry1, _In_ CRavidPoint<long long>* pGeometry2, _In_ CRavidPoint<long long>* pGeometry3, _In_ CRavidPoint<long long>* pGeometry4, _In_ CRavidPoint<long long>* pGeometry5, _In_opt_ double startDeg = 0, _In_opt_ double deltaDeg = 360, _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	/*
	* 5 점을 이용하여 타원을 추정합니다.
	*
	* param[in] refGeometry1 : CRavidPoint 1번째 x, y 값 (Reference)
	* param[in] refGeometry2 : CRavidPoint 2번째 x, y 값 (Reference)
	* param[in] refGeometry3 : CRavidPoint 3번째 x, y 값 (Reference)
	* param[in] refGeometry4 : CRavidPoint 4번째 x, y 값 (Reference)
	* param[in] refGeometry5 : CRavidPoint 5번째 x, y 값 (Reference)
	* param[in_opt] startDeg : 객체 시작 위치 각도 값
	* param[in_opt] deltaDeg : 객체 시작 각도로부터 표현 각도 값
	* param[in_opt] eRACM : 끝점 처리 방식
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetEllipse(_In_ CRavidPoint<float>& refGeometry1, _In_ CRavidPoint<float>& refGeometry2, _In_ CRavidPoint<float>& refGeometry3, _In_ CRavidPoint<float>& refGeometry4, _In_ CRavidPoint<float>& refGeometry5, _In_opt_ double startDeg = 0, _In_opt_ double deltaDeg = 360, _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	/*
	* 5 점을 이용하여 타원을 추정합니다.
	*
	* param[in] pGeometry1 : CRavidPoint 1번째 x, y 값 (Pointer)
	* param[in] pGeometry2 : CRavidPoint 2번째 x, y 값 (Pointer)
	* param[in] pGeometry3 : CRavidPoint 3번째 x, y 값 (Pointer)
	* param[in] pGeometry4 : CRavidPoint 4번째 x, y 값 (Pointer)
	* param[in] pGeometry5 : CRavidPoint 5번째 x, y 값 (Pointer)
	* param[in_opt] startDeg : 객체 시작 위치 각도 값
	* param[in_opt] deltaDeg : 객체 시작 각도로부터 표현 각도 값
	* param[in_opt] eRACM : 끝점 처리 방식
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetEllipse(_In_ CRavidPoint<float>* pGeometry1, _In_ CRavidPoint<float>* pGeometry2, _In_ CRavidPoint<float>* pGeometry3, _In_ CRavidPoint<float>* pGeometry4, _In_ CRavidPoint<float>* pGeometry5, _In_opt_ double startDeg = 0, _In_opt_ double deltaDeg = 360, _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	/*
	* 5 점을 이용하여 타원을 추정합니다.
	*
	* param[in] refGeometry1 : CRavidPoint 1번째 x, y 값 (Reference)
	* param[in] refGeometry2 : CRavidPoint 2번째 x, y 값 (Reference)
	* param[in] refGeometry3 : CRavidPoint 3번째 x, y 값 (Reference)
	* param[in] refGeometry4 : CRavidPoint 4번째 x, y 값 (Reference)
	* param[in] refGeometry5 : CRavidPoint 5번째 x, y 값 (Reference)
	* param[in_opt] startDeg : 객체 시작 위치 각도 값
	* param[in_opt] deltaDeg : 객체 시작 각도로부터 표현 각도 값
	* param[in_opt] eRACM : 끝점 처리 방식
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetEllipse(_In_ CRavidPoint<double>& refGeometry1, _In_ CRavidPoint<double>& refGeometry2, _In_ CRavidPoint<double>& refGeometry3, _In_ CRavidPoint<double>& refGeometry4, _In_ CRavidPoint<double>& refGeometry5, _In_opt_ double startDeg = 0, _In_opt_ double deltaDeg = 360, _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	/*
	* 5 점을 이용하여 타원을 추정합니다.
	*
	* param[in] pGeometry1 : CRavidPoint 1번째 x, y 값 (Pointer)
	* param[in] pGeometry2 : CRavidPoint 2번째 x, y 값 (Pointer)
	* param[in] pGeometry3 : CRavidPoint 3번째 x, y 값 (Pointer)
	* param[in] pGeometry4 : CRavidPoint 4번째 x, y 값 (Pointer)
	* param[in] pGeometry5 : CRavidPoint 5번째 x, y 값 (Pointer)
	* param[in_opt] startDeg : 객체 시작 위치 각도 값
	* param[in_opt] deltaDeg : 객체 시작 각도로부터 표현 각도 값
	* param[in_opt] eRACM : 끝점 처리 방식
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetEllipse(_In_ CRavidPoint<double>* pGeometry1, _In_ CRavidPoint<double>* pGeometry2, _In_ CRavidPoint<double>* pGeometry3, _In_ CRavidPoint<double>* pGeometry4, _In_ CRavidPoint<double>* pGeometry5, _In_opt_ double startDeg = 0, _In_opt_ double deltaDeg = 360, _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
#pragma endregion