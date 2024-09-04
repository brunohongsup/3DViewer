public:

#pragma region [CRavidPolygon]
	CRavidPolygon(_In_ const CRavidPolygon& poly);
	CRavidPolygon(_In_ CRavidPolygon* pPoly);
#pragma endregion

#pragma region [SetPolygon]
	/**
	* 현재 객체를 입력된 객체로 설정합니다.
	*
	* param[in] refGeometry : 추가 객체 (Reference)
	*
	* return value : 추가에 실패하면 false 를 반환합니다.
	*/
	bool SetPolygon(_In_ CRavidPolygon& refGeometry);
	/**
	* 현재 객체를 입력된 객체로 설정합니다.
	*
	* param[in] pGeometry : 추가 객체 (Pointer)
	*
	* return value : 추가에 실패하면 false 를 반환합니다.
	*/
	bool SetPolygon(_In_ CRavidPolygon* pGeometry);
#pragma endregion