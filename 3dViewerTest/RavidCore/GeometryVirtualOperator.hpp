public:

#pragma region [operator+=]
	/**
	* 현재 위치를 입력된 값 만큼 증가 이동한 결과를 적용하는 operator 연산자입니다.
	* 
	* param[in] value : x, y 동일 이동 거리
	*
	*/
	virtual void operator+=(_In_ const int& value);
	/**
	* 현재 위치를 입력된 값 만큼 증가 이동한 결과를 적용하는 operator 연산자입니다.
	* 
	* param[in] value : x, y 동일 이동 거리
	*
	*/
	virtual void operator+=(_In_ const long long& value);
	/**
	* 현재 위치를 입력된 값 만큼 증가 이동한 결과를 적용하는 operator 연산자입니다.
	* 
	* param[in] value : x, y 동일 이동 거리
	*
	*/
	virtual void operator+=(_In_ const float& value);
	/**
	* 현재 위치를 입력된 값 만큼 증가 이동한 결과를 적용하는 operator 연산자입니다.
	* 
	* param[in] value : x, y 동일 이동 거리
	*
	*/
	virtual void operator+=(_In_ const double& value);
	/**
	* 현재 위치를 입력된 값 만큼 증가 이동한 결과를 적용하는 operator 연산자입니다.
	* 
	* param[in] point : x, y 이동 거리 (Reference)
	*
	*/
	virtual void operator+=(_In_ const POINT& point);
	/**
	* 현재 위치를 입력된 값 만큼 증가 이동한 결과를 적용하는 operator 연산자입니다.
	* 
	* param[in] size : x, y 이동 거리 (Reference)
	*
	*/
	virtual void operator+=(_In_ const SIZE& size);
	/**
	* 현재 위치를 입력된 값 만큼 증가 이동한 결과를 적용하는 operator 연산자입니다.
	* 
	* param[in] point : x, y 이동 거리 (Reference)
	*
	*/
	virtual void operator+=(_In_ const CRavidPoint<int>& point);
	/**
	* 현재 위치를 입력된 값 만큼 증가 이동한 결과를 적용하는 operator 연산자입니다.
	* 
	* param[in] point : x, y 이동 거리 (Reference)
	*
	*/
	virtual void operator+=(_In_ const CRavidPoint<long long>& point);
	/**
	* 현재 위치를 입력된 값 만큼 증가 이동한 결과를 적용하는 operator 연산자입니다.
	* 
	* param[in] point : x, y 이동 거리 (Reference)
	*
	*/
	virtual void operator+=(_In_ const CRavidPoint<float>& point);
	/**
	* 현재 위치를 입력된 값 만큼 증가 이동한 결과를 적용하는 operator 연산자입니다.
	* 
	* param[in] point : x, y 이동 거리 (Reference)
	*
	*/
	virtual void operator+=(_In_ const CRavidPoint<double>& point);
#pragma endregion

#pragma region [operator-=]
	/**
	* 현재 위치를 입력된 값 만큼 감소 이동한 결과를 적용하는 operator 연산자입니다.
	* 
	* param[in] value : x, y 동일 이동 거리
	*
	*/
	virtual void operator-=(_In_ const int& value);
	/**
	* 현재 위치를 입력된 값 만큼 감소 이동한 결과를 적용하는 operator 연산자입니다.
	* 
	* param[in] value : x, y 동일 이동 거리
	*
	*/
	virtual void operator-=(_In_ const long long& value);
	/**
	* 현재 위치를 입력된 값 만큼 감소 이동한 결과를 적용하는 operator 연산자입니다.
	* 
	* param[in] value : x, y 동일 이동 거리
	*
	*/
	virtual void operator-=(_In_ const float& value);
	/**
	* 현재 위치를 입력된 값 만큼 감소 이동한 결과를 적용하는 operator 연산자입니다.
	* 
	* param[in] value : x, y 동일 이동 거리
	*
	*/
	virtual void operator-=(_In_ const double& value);
	/**
	* 현재 위치를 입력된 값 만큼 감소 이동한 결과를 적용하는 operator 연산자입니다.
	* 
	* param[in] point : x, y 이동 거리 (Reference)
	*
	*/
	virtual void operator-=(_In_ const POINT& point);
	/**
	* 현재 위치를 입력된 값 만큼 감소 이동한 결과를 적용하는 operator 연산자입니다.
	* 
	* param[in] size : x, y 이동 거리 (Reference)
	*
	*/
	virtual void operator-=(_In_ const SIZE& size);
	/**
	* 현재 위치를 입력된 값 만큼 감소 이동한 결과를 적용하는 operator 연산자입니다.
	* 
	* param[in] point : x, y 이동 거리 (Reference)
	*
	*/
	virtual void operator-=(_In_ const CRavidPoint<int>& point);
	/**
	* 현재 위치를 입력된 값 만큼 감소 이동한 결과를 적용하는 operator 연산자입니다.
	* 
	* param[in] point : x, y 이동 거리 (Reference)
	*
	*/
	virtual void operator-=(_In_ const CRavidPoint<long long>& point);
	/**
	* 현재 위치를 입력된 값 만큼 감소 이동한 결과를 적용하는 operator 연산자입니다.
	* 
	* param[in] point : x, y 이동 거리 (Reference)
	*
	*/
	virtual void operator-=(_In_ const CRavidPoint<float>& point);
	/**
	* 현재 위치를 입력된 값 만큼 감소 이동한 결과를 적용하는 operator 연산자입니다.
	* 
	* param[in] point : x, y 이동 거리 (Reference)
	*
	*/
	virtual void operator-=(_In_ const CRavidPoint<double>& point);
#pragma endregion

#pragma region [operator*=]
	/**
	* 현재 위치를 입력된 값의 배율곱 만큼 이동한 결과를 적용하는 operator 연산자입니다.
	* 
	* param[in] value : x, y 동일 배율
	*
	*/
	virtual void operator*=(_In_ const int& value);
	/**
	* 현재 위치를 입력된 값의 배율곱 만큼 이동한 결과를 적용하는 operator 연산자입니다.
	* 
	* param[in] value : x, y 동일 배율
	*
	*/
	virtual void operator*=(_In_ const long long& value);
	/**
	* 현재 위치를 입력된 값의 배율곱 만큼 이동한 결과를 적용하는 operator 연산자입니다.
	* 
	* param[in] value : x, y 동일 배율
	*
	*/
	virtual void operator*=(_In_ const float& value);
	/**
	* 현재 위치를 입력된 값의 배율곱 만큼 이동한 결과를 적용하는 operator 연산자입니다.
	* 
	* param[in] value : x, y 동일 배율
	*
	*/
	virtual void operator*=(_In_ const double& value);
	/**
	* 현재 위치를 입력된 값의 배율곱 만큼 이동한 결과를 적용하는 operator 연산자입니다.
	* 
	* param[in] point : x, y 배율 (Reference)
	*
	*/
	virtual void operator*=(_In_ const POINT& point);
	/**
	* 현재 위치를 입력된 값의 배율곱 만큼 이동한 결과를 적용하는 operator 연산자입니다.
	* 
	* param[in] size : x, y 배율 (Reference)
	*
	*/
	virtual void operator*=(_In_ const SIZE& size);
	/**
	* 현재 위치를 입력된 값의 배율곱 만큼 이동한 결과를 적용하는 operator 연산자입니다.
	* 
	* param[in] point : x, y 배율 (Reference)
	*
	*/
	virtual void operator*=(_In_ const CRavidPoint<int>& point);
	/**
	* 현재 위치를 입력된 값의 배율곱 만큼 이동한 결과를 적용하는 operator 연산자입니다.
	* 
	* param[in] point : x, y 배율 (Reference)
	*
	*/
	virtual void operator*=(_In_ const CRavidPoint<long long>& point);
	/**
	* 현재 위치를 입력된 값의 배율곱 만큼 이동한 결과를 적용하는 operator 연산자입니다.
	* 
	* param[in] point : x, y 배율 (Reference)
	*
	*/
	virtual void operator*=(_In_ const CRavidPoint<float>& point);
	/**
	* 현재 위치를 입력된 값의 배율곱 만큼 이동한 결과를 적용하는 operator 연산자입니다.
	* 
	* param[in] point : x, y 배율 (Reference)
	*
	*/
	virtual void operator*=(_In_ const CRavidPoint<double>& point);
#pragma endregion

#pragma region [operator/=]
	/**
	* 현재 위치를 입력된 값의 배율나눗셈 만큼 이동한 결과를 적용하는 operator 연산자입니다.
	* 
	* param[in] value : x, y 동일 배율
	*
	*/
	virtual void operator/=(_In_ const int& value);
	/**
	* 현재 위치를 입력된 값의 배율나눗셈 만큼 이동한 결과를 적용하는 operator 연산자입니다.
	* 
	* param[in] value : x, y 동일 배율
	*
	*/
	virtual void operator/=(_In_ const long long& value);
	/**
	* 현재 위치를 입력된 값의 배율나눗셈 만큼 이동한 결과를 적용하는 operator 연산자입니다.
	* 
	* param[in] value : x, y 동일 배율
	*
	*/
	virtual void operator/=(_In_ const float& value);
	/**
	* 현재 위치를 입력된 값의 배율나눗셈 만큼 이동한 결과를 적용하는 operator 연산자입니다.
	* 
	* param[in] value : x, y 동일 배율
	*
	*/
	virtual void operator/=(_In_ const double& value);
	/**
	* 현재 위치를 입력된 값의 배율나눗셈 만큼 이동한 결과를 적용하는 operator 연산자입니다.
	*
	* param[in] point : x, y 배율 (Reference)
	*
	*/
	virtual void operator/=(_In_ const POINT& point);
	/**
	* 현재 위치를 입력된 값의 배율나눗셈 만큼 이동한 결과를 적용하는 operator 연산자입니다.
	*
	* param[in] size : x, y 배율 (Reference)
	*
	*/
	virtual void operator/=(_In_ const SIZE& size);
	/**
	* 현재 위치를 입력된 값의 배율나눗셈 만큼 이동한 결과를 적용하는 operator 연산자입니다.
	*
	* param[in] point : x, y 배율 (Reference)
	*
	*/
	virtual void operator/=(_In_ const CRavidPoint<int>& point);
	/**
	* 현재 위치를 입력된 값의 배율나눗셈 만큼 이동한 결과를 적용하는 operator 연산자입니다.
	*
	* param[in] point : x, y 배율 (Reference)
	*
	*/
	virtual void operator/=(_In_ const CRavidPoint<long long>& point);
	/**
	* 현재 위치를 입력된 값의 배율나눗셈 만큼 이동한 결과를 적용하는 operator 연산자입니다.
	*
	* param[in] point : x, y 배율 (Reference)
	*
	*/
	virtual void operator/=(_In_ const CRavidPoint<float>& point);
	/**
	* 현재 위치를 입력된 값의 배율나눗셈 만큼 이동한 결과를 적용하는 operator 연산자입니다.
	*
	* param[in] point : x, y 배율 (Reference)
	*
	*/
	virtual void operator/=(_In_ const CRavidPoint<double>& point);
#pragma endregion