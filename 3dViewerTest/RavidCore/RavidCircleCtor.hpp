public:

#pragma region [CRavidCircle]
	CRavidCircle(_In_ Ravid::Algorithms::CRavidImage& ri, _In_opt_ double angle = 0.0, _In_opt_ double startDeg = 0.0, _In_opt_ double deltaDeg = 360.0, _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidCircle(_In_ T x, _In_ T y, _In_ Ravid::Algorithms::CRavidImage& ri, _In_opt_ double angle = 0.0, _In_opt_ double startDeg = 0.0, _In_opt_ double deltaDeg = 360.0, _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidCircle(_In_ const POINT& point, _In_ Ravid::Algorithms::CRavidImage& ri, _In_opt_ double angle = 0.0, _In_opt_ double startDeg = 0.0, _In_opt_ double deltaDeg = 360.0, _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidCircle(_In_ POINT* pPoint, _In_ Ravid::Algorithms::CRavidImage& ri, _In_opt_ double angle = 0.0, _In_opt_ double startDeg = 0.0, _In_opt_ double deltaDeg = 360.0, _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidCircle(_In_ const SIZE& size, _In_ Ravid::Algorithms::CRavidImage& ri, _In_opt_ double angle = 0.0, _In_opt_ double startDeg = 0.0, _In_opt_ double deltaDeg = 360.0, _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidCircle(_In_ SIZE* pSize, _In_ Ravid::Algorithms::CRavidImage& ri, _In_opt_ double angle = 0.0, _In_opt_ double startDeg = 0.0, _In_opt_ double deltaDeg = 360.0, _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidCircle(_In_ const CRavidPoint<int>& point, _In_ Ravid::Algorithms::CRavidImage& ri, _In_opt_ double angle = 0.0, _In_opt_ double startDeg = 0.0, _In_opt_ double deltaDeg = 360.0, _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidCircle(_In_ CRavidPoint<int>* pPoint, _In_ Ravid::Algorithms::CRavidImage& ri, _In_opt_ double angle = 0.0, _In_opt_ double startDeg = 0.0, _In_opt_ double deltaDeg = 360.0, _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidCircle(_In_ const CRavidPoint<long long>& point, _In_ Ravid::Algorithms::CRavidImage& ri, _In_opt_ double angle = 0.0, _In_opt_ double startDeg = 0.0, _In_opt_ double deltaDeg = 360.0, _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidCircle(_In_ CRavidPoint<long long>* pPoint, _In_ Ravid::Algorithms::CRavidImage& ri, _In_opt_ double angle = 0.0, _In_opt_ double startDeg = 0.0, _In_opt_ double deltaDeg = 360.0, _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidCircle(_In_ const CRavidPoint<float>& point, _In_ Ravid::Algorithms::CRavidImage& ri, _In_opt_ double angle = 0.0, _In_opt_ double startDeg = 0.0, _In_opt_ double deltaDeg = 360.0, _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidCircle(_In_ CRavidPoint<float>* pPoint, _In_ Ravid::Algorithms::CRavidImage& ri, _In_opt_ double angle = 0.0, _In_opt_ double startDeg = 0.0, _In_opt_ double deltaDeg = 360.0, _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidCircle(_In_ const CRavidPoint<double>& point, _In_ Ravid::Algorithms::CRavidImage& ri, _In_opt_ double angle = 0.0, _In_opt_ double startDeg = 0.0, _In_opt_ double deltaDeg = 360.0, _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidCircle(_In_ CRavidPoint<double>* pPoint, _In_ Ravid::Algorithms::CRavidImage& ri, _In_opt_ double angle = 0.0, _In_opt_ double startDeg = 0.0, _In_opt_ double deltaDeg = 360.0, _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidCircle(_In_ T x, _In_ T y, _In_ T radius, _In_opt_ double angle = 0., _In_opt_ double startDeg = 0., _In_opt_ double deltaDeg = 360., _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidCircle(_In_ const POINT& point, _In_ T radius, _In_opt_ double angle = 0., _In_opt_ double startDeg = 0., _In_opt_ double deltaDeg = 360., _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidCircle(_In_ POINT* pPoint, _In_ T radius, _In_opt_ double angle = 0., _In_opt_ double startDeg = 0., _In_opt_ double deltaDeg = 360., _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidCircle(_In_ const SIZE& size, _In_ T radius, _In_opt_ double angle = 0., _In_opt_ double startDeg = 0., _In_opt_ double deltaDeg = 360., _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidCircle(_In_ SIZE* pSize, _In_ T radius, _In_opt_ double angle = 0., _In_opt_ double startDeg = 0., _In_opt_ double deltaDeg = 360., _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidCircle(_In_ const CRavidPoint<int>& point, _In_ T radius, _In_opt_ double angle = 0., _In_opt_ double startDeg = 0., _In_opt_ double deltaDeg = 360., _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidCircle(_In_ CRavidPoint<int>* pPoint, _In_ T radius, _In_opt_ double angle = 0., _In_opt_ double startDeg = 0., _In_opt_ double deltaDeg = 360., _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidCircle(_In_ const CRavidPoint<long long>& point, _In_ T radius, _In_opt_ double angle = 0., _In_opt_ double startDeg = 0., _In_opt_ double deltaDeg = 360., _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidCircle(_In_ CRavidPoint<long long>* pPoint, _In_ T radius, _In_opt_ double angle = 0., _In_opt_ double startDeg = 0., _In_opt_ double deltaDeg = 360., _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidCircle(_In_ const CRavidPoint<float>& point, _In_ T radius, _In_opt_ double angle = 0., _In_opt_ double startDeg = 0., _In_opt_ double deltaDeg = 360., _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidCircle(_In_ CRavidPoint<float>* pPoint, _In_ T radius, _In_opt_ double angle = 0., _In_opt_ double startDeg = 0., _In_opt_ double deltaDeg = 360., _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidCircle(_In_ const CRavidPoint<double>& point, _In_ T radius, _In_opt_ double angle = 0., _In_opt_ double startDeg = 0., _In_opt_ double deltaDeg = 360., _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidCircle(_In_ CRavidPoint<double>* pPoint, _In_ T radius, _In_opt_ double angle = 0., _In_opt_ double startDeg = 0., _In_opt_ double deltaDeg = 360., _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidCircle(_In_ const RECT& rect, _In_opt_ double startDeg = 0., _In_opt_ double deltaDeg = 360., _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidCircle(_In_ RECT* pRect, _In_opt_ double startDeg = 0., _In_opt_ double deltaDeg = 360., _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidCircle(_In_ const CRavidRect<int>& rect, _In_opt_ double startDeg = 0., _In_opt_ double deltaDeg = 360., _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidCircle(_In_ CRavidRect<int>* pRect, _In_opt_ double startDeg = 0., _In_opt_ double deltaDeg = 360., _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidCircle(_In_ const CRavidRect<long long>& rect, _In_opt_ double startDeg = 0., _In_opt_ double deltaDeg = 360., _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidCircle(_In_ CRavidRect<long long>* pRect, _In_opt_ double startDeg = 0., _In_opt_ double deltaDeg = 360., _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidCircle(_In_ const CRavidRect<float>& rect, _In_opt_ double startDeg = 0., _In_opt_ double deltaDeg = 360., _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidCircle(_In_ CRavidRect<float>* pRect, _In_opt_ double startDeg = 0., _In_opt_ double deltaDeg = 360., _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidCircle(_In_ const CRavidRect<double>& rect, _In_opt_ double startDeg = 0., _In_opt_ double deltaDeg = 360., _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidCircle(_In_ CRavidRect<double>* pRect, _In_opt_ double startDeg = 0., _In_opt_ double deltaDeg = 360., _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	CRavidCircle(_In_ const CRavidCircle<int>& cir);
	CRavidCircle(_In_ CRavidCircle<int>* pCir);
	CRavidCircle(_In_ const CRavidCircle<long long>& cir);
	CRavidCircle(_In_ CRavidCircle<long long>* pCir);
	CRavidCircle(_In_ const CRavidCircle<float>& cir);
	CRavidCircle(_In_ CRavidCircle<float>* pCir);
	CRavidCircle(_In_ const CRavidCircle<double>& cir);
	CRavidCircle(_In_ CRavidCircle<double>* pCir);
#pragma endregion

#pragma region [SetCircle]
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] x : 객체 중심 x 값
	* param[in] y : 객체 중심 y 값
	* param[in] radius : 반지름 값
	* param[in_opt] angle : 객체 회전 각도 값
	* param[in_opt] startDeg : 객체 시작 위치 각도 값
	* param[in_opt] deltaDeg : 객체 시작 각도로부터 표현 각도 값
	* param[in_opt] eRACM : 끝점 처리 방식
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetCircle(_In_ int x, _In_ int y, _In_ double radius, _In_opt_ double angle = 0., _In_opt_ double startDeg = 0., _In_opt_ double deltaDeg = 360., _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] x : 객체 중심 x 값
	* param[in] y : 객체 중심 y 값
	* param[in] radius : 반지름 값
	* param[in_opt] angle : 객체 회전 각도 값
	* param[in_opt] startDeg : 객체 시작 위치 각도 값
	* param[in_opt] deltaDeg : 객체 시작 각도로부터 표현 각도 값
	* param[in_opt] eRACM : 끝점 처리 방식
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetCircle(_In_ long long x, _In_ long long y, _In_ double radius, _In_opt_ double angle = 0., _In_opt_ double startDeg = 0., _In_opt_ double deltaDeg = 360., _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] x : 객체 중심 x 값
	* param[in] y : 객체 중심 y 값
	* param[in] radius : 반지름 값
	* param[in_opt] angle : 객체 회전 각도 값
	* param[in_opt] startDeg : 객체 시작 위치 각도 값
	* param[in_opt] deltaDeg : 객체 시작 각도로부터 표현 각도 값
	* param[in_opt] eRACM : 끝점 처리 방식
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetCircle(_In_ float x, _In_ float y, _In_ double radius, _In_opt_ double angle = 0., _In_opt_ double startDeg = 0., _In_opt_ double deltaDeg = 360., _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] x : 객체 중심 x 값
	* param[in] y : 객체 중심 y 값
	* param[in] radius : 반지름 값
	* param[in_opt] angle : 객체 회전 각도 값
	* param[in_opt] startDeg : 객체 시작 위치 각도 값
	* param[in_opt] deltaDeg : 객체 시작 각도로부터 표현 각도 값
	* param[in_opt] eRACM : 끝점 처리 방식
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetCircle(_In_ double x, _In_ double y, _In_ double radius, _In_opt_ double angle, _In_opt_ double startDeg = 0, _In_opt_ double deltaDeg = 360, _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] refGeometry : CRavidCircle 원 정보 (Reference)
	* 
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetCircle(_In_ CRavidCircle<int>& refGeometry);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] pGeometry : CRavidCircle 원 정보 (Pointer)
	* 
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetCircle(_In_ CRavidCircle<int>* pGeometry);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] refGeometry : CRavidCircle 원 정보 (Reference)
	* 
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetCircle(_In_ CRavidCircle<long long>& refGeometry);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] pGeometry : CRavidCircle 원 정보 (Pointer)
	* 
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetCircle(_In_ CRavidCircle<long long>* pGeometry);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] refGeometry : CRavidCircle 원 정보 (Reference)
	* 
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetCircle(_In_ CRavidCircle<float>& refGeometry);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] pGeometry : CRavidCircle 원 정보 (Pointer)
	* 
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetCircle(_In_ CRavidCircle<float>* pGeometry);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] refGeometry : CRavidCircle 원 정보 (Reference)
	* 
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetCircle(_In_ CRavidCircle<double>& refGeometry);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] pGeometry : CRavidCircle 원 정보 (Pointer)
	* 
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetCircle(_In_ CRavidCircle<double>* pGeometry);
	/*
	* 외접원을 이용한 현재 객체를 설정하는 기능입니다.
	*
	* param[in] x1 : 외접원 계산 1번째 x 값
	* param[in] y1 : 외접원 계산 1번째 y 값
	* param[in] x2 : 외접원 계산 2번째 x 값
	* param[in] y2 : 외접원 계산 2번째 y 값
	* param[in] x3 : 외접원 계산 3번째 x 값
	* param[in] y3 : 외접원 계산 3번째 y 값
	* param[in_opt] angle : 객체 회전 각도 값
	* param[in_opt] startDeg : 객체 시작 위치 각도 값
	* param[in_opt] deltaDeg : 객체 시작 각도로부터 표현 각도 값
	* param[in_opt] eRACM : 끝점 처리 방식
	* 
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetCircle(_In_ int x1, _In_ int y1, _In_ int x2, _In_ int y2, _In_ int x3, _In_ int y3, _In_opt_ double angle = 0, _In_opt_ double startDeg = 0, _In_opt_ double deltaDeg = 360, _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	/*
	* 외접원을 이용한 현재 객체를 설정하는 기능입니다.
	*
	* param[in] x1 : 외접원 계산 1번째 x 값
	* param[in] y1 : 외접원 계산 1번째 y 값
	* param[in] x2 : 외접원 계산 2번째 x 값
	* param[in] y2 : 외접원 계산 2번째 y 값
	* param[in] x3 : 외접원 계산 3번째 x 값
	* param[in] y3 : 외접원 계산 3번째 y 값
	* param[in_opt] angle : 객체 회전 각도 값
	* param[in_opt] startDeg : 객체 시작 위치 각도 값
	* param[in_opt] deltaDeg : 객체 시작 각도로부터 표현 각도 값
	* param[in_opt] eRACM : 끝점 처리 방식
	* 
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetCircle(_In_ long long x1, _In_ long long y1, _In_ long long x2, _In_ long long y2, _In_ long long x3, _In_ long long y3, _In_opt_ double angle = 0, _In_opt_ double startDeg = 0, _In_opt_ double deltaDeg = 360, _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	/*
	* 외접원을 이용한 현재 객체를 설정하는 기능입니다.
	*
	* param[in] x1 : 외접원 계산 1번째 x 값
	* param[in] y1 : 외접원 계산 1번째 y 값
	* param[in] x2 : 외접원 계산 2번째 x 값
	* param[in] y2 : 외접원 계산 2번째 y 값
	* param[in] x3 : 외접원 계산 3번째 x 값
	* param[in] y3 : 외접원 계산 3번째 y 값
	* param[in_opt] angle : 객체 회전 각도 값
	* param[in_opt] startDeg : 객체 시작 위치 각도 값
	* param[in_opt] deltaDeg : 객체 시작 각도로부터 표현 각도 값
	* param[in_opt] eRACM : 끝점 처리 방식
	* 
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetCircle(_In_ float x1, _In_ float y1, _In_ float x2, _In_ float y2, _In_ float x3, _In_ float y3, _In_opt_ double angle = 0, _In_opt_ double startDeg = 0, _In_opt_ double deltaDeg = 360, _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	/*
	* 외접원을 이용한 현재 객체를 설정하는 기능입니다.
	*
	* param[in] x1 : 외접원 계산 1번째 x 값
	* param[in] y1 : 외접원 계산 1번째 y 값
	* param[in] x2 : 외접원 계산 2번째 x 값
	* param[in] y2 : 외접원 계산 2번째 y 값
	* param[in] x3 : 외접원 계산 3번째 x 값
	* param[in] y3 : 외접원 계산 3번째 y 값
	* param[in_opt] angle : 객체 회전 각도 값
	* param[in_opt] startDeg : 객체 시작 위치 각도 값
	* param[in_opt] deltaDeg : 객체 시작 각도로부터 표현 각도 값
	* param[in_opt] eRACM : 끝점 처리 방식
	* 
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetCircle(_In_ double x1, _In_ double y1, _In_ double x2, _In_ double y2, _In_ double x3, _In_ double y3, _In_opt_ double angle = 0, _In_opt_ double startDeg = 0, _In_opt_ double deltaDeg = 360, _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	/*
	* 외접원을 이용한 현재 객체를 설정하는 기능입니다.
	*
	* param[in] refGeometry1 : CRavidPoint 외접원 계산 1번째 x, y 값 (Reference)
	* param[in] refGeometry2 : CRavidPoint 외접원 계산 2번째 x, y 값 (Reference)
	* param[in] refGeometry3 : CRavidPoint 외접원 계산 3번째 x, y 값 (Reference)
	* param[in_opt] angle : 객체 회전 각도 값
	* param[in_opt] startDeg : 객체 시작 위치 각도 값
	* param[in_opt] deltaDeg : 객체 시작 각도로부터 표현 각도 값
	* param[in_opt] eRACM : 끝점 처리 방식
	* 
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetCircle(_In_ CRavidPoint<int>& refGeometry1, _In_ CRavidPoint<int>& refGeometry2, _In_ CRavidPoint<int>& refGeometry3, _In_opt_ double angle = 0, _In_opt_ double startDeg = 0, _In_opt_ double deltaDeg = 360, _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	/*
	* 외접원을 이용한 현재 객체를 설정하는 기능입니다.
	*
	* param[in] pGeometry1 : CRavidPoint 외접원 계산 1번째 x, y 값 (Pointer)
	* param[in] pGeometry2 : CRavidPoint 외접원 계산 2번째 x, y 값 (Pointer)
	* param[in] pGeometry3 : CRavidPoint 외접원 계산 3번째 x, y 값 (Pointer)
	* param[in_opt] angle : 객체 회전 각도 값
	* param[in_opt] startDeg : 객체 시작 위치 각도 값
	* param[in_opt] deltaDeg : 객체 시작 각도로부터 표현 각도 값
	* param[in_opt] eRACM : 끝점 처리 방식
	* 
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetCircle(_In_ CRavidPoint<int>* pGeometry1, _In_ CRavidPoint<int>* pGeometry2, _In_ CRavidPoint<int>* pGeometry3, _In_opt_ double angle = 0, _In_opt_ double startDeg = 0, _In_opt_ double deltaDeg = 360, _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	/*
	* 외접원을 이용한 현재 객체를 설정하는 기능입니다.
	*
	* param[in] refGeometry1 : CRavidPoint 외접원 계산 1번째 x, y 값 (Reference)
	* param[in] refGeometry2 : CRavidPoint 외접원 계산 2번째 x, y 값 (Reference)
	* param[in] refGeometry3 : CRavidPoint 외접원 계산 3번째 x, y 값 (Reference)
	* param[in_opt] angle : 객체 회전 각도 값
	* param[in_opt] startDeg : 객체 시작 위치 각도 값
	* param[in_opt] deltaDeg : 객체 시작 각도로부터 표현 각도 값
	* param[in_opt] eRACM : 끝점 처리 방식
	* 
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetCircle(_In_ CRavidPoint<long long>& refGeometry1, _In_ CRavidPoint<long long>& refGeometry2, _In_ CRavidPoint<long long>& refGeometry3, _In_opt_ double angle = 0, _In_opt_ double startDeg = 0, _In_opt_ double deltaDeg = 360, _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	/*
	* 외접원을 이용한 현재 객체를 설정하는 기능입니다.
	*
	* param[in] pGeometry1 : CRavidPoint 외접원 계산 1번째 x, y 값 (Pointer)
	* param[in] pGeometry2 : CRavidPoint 외접원 계산 2번째 x, y 값 (Pointer)
	* param[in] pGeometry3 : CRavidPoint 외접원 계산 3번째 x, y 값 (Pointer)
	* param[in_opt] angle : 객체 회전 각도 값
	* param[in_opt] startDeg : 객체 시작 위치 각도 값
	* param[in_opt] deltaDeg : 객체 시작 각도로부터 표현 각도 값
	* param[in_opt] eRACM : 끝점 처리 방식
	* 
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetCircle(_In_ CRavidPoint<long long>* pGeometry1, _In_ CRavidPoint<long long>* pGeometry2, _In_ CRavidPoint<long long>* pGeometry3, _In_opt_ double angle = 0, _In_opt_ double startDeg = 0, _In_opt_ double deltaDeg = 360, _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	/*
	* 외접원을 이용한 현재 객체를 설정하는 기능입니다.
	*
	* param[in] refGeometry1 : CRavidPoint 외접원 계산 1번째 x, y 값 (Reference)
	* param[in] refGeometry2 : CRavidPoint 외접원 계산 2번째 x, y 값 (Reference)
	* param[in] refGeometry3 : CRavidPoint 외접원 계산 3번째 x, y 값 (Reference)
	* param[in_opt] angle : 객체 회전 각도 값
	* param[in_opt] startDeg : 객체 시작 위치 각도 값
	* param[in_opt] deltaDeg : 객체 시작 각도로부터 표현 각도 값
	* param[in_opt] eRACM : 끝점 처리 방식
	* 
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetCircle(_In_ CRavidPoint<float>& refGeometry1, _In_ CRavidPoint<float>& refGeometry2, _In_ CRavidPoint<float>& refGeometry3, _In_opt_ double angle = 0, _In_opt_ double startDeg = 0, _In_opt_ double deltaDeg = 360, _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	/*
	* 외접원을 이용한 현재 객체를 설정하는 기능입니다.
	*
	* param[in] pGeometry1 : CRavidPoint 외접원 계산 1번째 x, y 값 (Pointer)
	* param[in] pGeometry2 : CRavidPoint 외접원 계산 2번째 x, y 값 (Pointer)
	* param[in] pGeometry3 : CRavidPoint 외접원 계산 3번째 x, y 값 (Pointer)
	* param[in_opt] angle : 객체 회전 각도 값
	* param[in_opt] startDeg : 객체 시작 위치 각도 값
	* param[in_opt] deltaDeg : 객체 시작 각도로부터 표현 각도 값
	* param[in_opt] eRACM : 끝점 처리 방식
	* 
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetCircle(_In_ CRavidPoint<float>* pGeometry1, _In_ CRavidPoint<float>* pGeometry2, _In_ CRavidPoint<float>* pGeometry3, _In_opt_ double angle = 0, _In_opt_ double startDeg = 0, _In_opt_ double deltaDeg = 360, _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	/*
	* 외접원을 이용한 현재 객체를 설정하는 기능입니다.
	*
	* param[in] refGeometry1 : CRavidPoint 외접원 계산 1번째 x, y 값 (Reference)
	* param[in] refGeometry2 : CRavidPoint 외접원 계산 2번째 x, y 값 (Reference)
	* param[in] refGeometry3 : CRavidPoint 외접원 계산 3번째 x, y 값 (Reference)
	* param[in_opt] angle : 객체 회전 각도 값
	* param[in_opt] startDeg : 객체 시작 위치 각도 값
	* param[in_opt] deltaDeg : 객체 시작 각도로부터 표현 각도 값
	* param[in_opt] eRACM : 끝점 처리 방식
	* 
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetCircle(_In_ CRavidPoint<double>& refGeometry1, _In_ CRavidPoint<double>& refGeometry2, _In_ CRavidPoint<double>& refGeometry3, _In_opt_ double angle = 0, _In_opt_ double startDeg = 0, _In_opt_ double deltaDeg = 360, _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
	/*
	* 외접원을 이용한 현재 객체를 설정하는 기능입니다.
	*
	* param[in] pGeometry1 : CRavidPoint 외접원 계산 1번째 x, y 값 (Pointer)
	* param[in] pGeometry2 : CRavidPoint 외접원 계산 2번째 x, y 값 (Pointer)
	* param[in] pGeometry3 : CRavidPoint 외접원 계산 3번째 x, y 값 (Pointer)
	* param[in_opt] angle : 객체 회전 각도 값
	* param[in_opt] startDeg : 객체 시작 위치 각도 값
	* param[in_opt] deltaDeg : 객체 시작 각도로부터 표현 각도 값
	* param[in_opt] eRACM : 끝점 처리 방식
	* 
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetCircle(_In_ CRavidPoint<double>* pGeometry1, _In_ CRavidPoint<double>* pGeometry2, _In_ CRavidPoint<double>* pGeometry3, _In_opt_ double angle = 0, _In_opt_ double startDeg = 0, _In_opt_ double deltaDeg = 360, _In_opt_ ERavidArcClosingMethod eRACM = ERavidArcClosingMethod_EachOther);
#pragma endregion