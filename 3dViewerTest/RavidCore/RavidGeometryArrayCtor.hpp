public:

#pragma region [CRavidGeometryArray]
	CRavidGeometryArray(_In_ const CRavidGeometryArray& refGeometryArray);
	CRavidGeometryArray(_In_ CRavidGeometryArray* pGeometryArray);
#pragma endregion
		
#pragma region [SetArray]
	/**
	* 현재 객체를 입력된 객체로 설정합니다.
	*
	* param[in] refGeometryArray : 설정 객체 (Reference)
	*
	* return value : 설정에 실패하면 false 를 반환합니다.
	*/
	bool SetArray(_In_ CRavidGeometryArray& refGeometryArray);
	/**
	* 현재 객체를 입력된 객체로 설정합니다.
	*
	* param[in] pGeometryArray : 설정 객체 (Pointer)
	*
	* return value : 설정에 실패하면 false 를 반환합니다.
	*/
	bool SetArray(_In_ CRavidGeometryArray* pGeometryArray);
#pragma endregion