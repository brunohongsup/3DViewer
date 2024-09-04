public:

#pragma region [Rotate]
	/**
	* 상대 위치를 기준으로 회전각도 만큼 현재 위치를 회전합니다.
	* 
	* param[in] dblAngle : 회전각도
	* param[in] center : 중심 좌표 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	virtual bool Rotate(_In_ double dblAngle, _In_ CRavidPoint<int>& center);
	/**
	* 상대 위치를 기준으로 회전각도 만큼 현재 위치를 회전합니다.
	* 
	* param[in] dblAngle : 회전각도
	* param[in] pCenter : 중심 좌표 (Pointer)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	virtual bool Rotate(_In_ double dblAngle, _In_ CRavidPoint<int>* pCenter);
	/**
	* 상대 위치를 기준으로 회전각도 만큼 현재 위치를 회전합니다.
	* 
	* param[in] dblAngle : 회전각도
	* param[in] center : 중심 좌표 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	virtual bool Rotate(_In_ double dblAngle, _In_ CRavidPoint<long long>& center);
	/**
	* 상대 위치를 기준으로 회전각도 만큼 현재 위치를 회전합니다.
	* 
	* param[in] dblAngle : 회전각도
	* param[in] pCenter : 중심 좌표 (Pointer)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	virtual bool Rotate(_In_ double dblAngle, _In_ CRavidPoint<long long>* pCenter);
	/**
	* 상대 위치를 기준으로 회전각도 만큼 현재 위치를 회전합니다.
	* 
	* param[in] dblAngle : 회전각도
	* param[in] center : 중심 좌표 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	virtual bool Rotate(_In_ double dblAngle, _In_ CRavidPoint<float>& center);
	/**
	* 상대 위치를 기준으로 회전각도 만큼 현재 위치를 회전합니다.
	* 
	* param[in] dblAngle : 회전각도
	* param[in] pCenter : 중심 좌표 (Pointer)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	virtual bool Rotate(_In_ double dblAngle, _In_ CRavidPoint<float>* pCenter);
	/**
	* 상대 위치를 기준으로 회전각도 만큼 현재 위치를 회전합니다.
	* 
	* param[in] dblAngle : 회전각도
	* param[in] center : 중심 좌표 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	virtual bool Rotate(_In_ double dblAngle, _In_ CRavidPoint<double>& center);
	/**
	* 상대 위치를 기준으로 회전각도 만큼 현재 위치를 회전합니다.
	* 
	* param[in] dblAngle : 회전각도
	* param[in] pCenter : 중심 좌표 (Pointer)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	virtual bool Rotate(_In_ double dblAngle, _In_ CRavidPoint<double>* pCenter);
#pragma endregion