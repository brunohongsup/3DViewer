public:

#pragma region [GetIntersection]
	/**
	* 현재 객체와 상대 객체와의 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidRect<int>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidRect<int>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidRect<long long>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidRect<long long>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidRect<float>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidRect<float>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidRect<double>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidRect<double>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidQuadrangle<int>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidQuadrangle<int>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidQuadrangle<long long>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidQuadrangle<long long>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidQuadrangle<float>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidQuadrangle<float>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidQuadrangle<double>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidQuadrangle<double>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidCircle<int>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidCircle<int>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidCircle<long long>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidCircle<long long>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidCircle<float>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidCircle<float>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidCircle<double>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidCircle<double>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidEllipse<int>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidEllipse<int>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidEllipse<long long>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidEllipse<long long>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidEllipse<float>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidEllipse<float>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidEllipse<double>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidEllipse<double>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidPolygon& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersection(_In_ CRavidPolygon* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
#pragma endregion

#pragma region [DoesIgnoreInternalIntersection]
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool DoesIgnoreInternalIntersection(_In_ CRavidRect<int>& geometry);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool DoesIgnoreInternalIntersection(_In_ CRavidRect<int>* pGeometry);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool DoesIgnoreInternalIntersection(_In_ CRavidRect<long long>& geometry);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool DoesIgnoreInternalIntersection(_In_ CRavidRect<long long>* pGeometry);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool DoesIgnoreInternalIntersection(_In_ CRavidRect<float>& geometry);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool DoesIgnoreInternalIntersection(_In_ CRavidRect<float>* pGeometry);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool DoesIgnoreInternalIntersection(_In_ CRavidRect<double>& geometry);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool DoesIgnoreInternalIntersection(_In_ CRavidRect<double>* pGeometry);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool DoesIgnoreInternalIntersection(_In_ CRavidQuadrangle<int>& geometry);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool DoesIgnoreInternalIntersection(_In_ CRavidQuadrangle<int>* pGeometry);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool DoesIgnoreInternalIntersection(_In_ CRavidQuadrangle<long long>& geometry);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool DoesIgnoreInternalIntersection(_In_ CRavidQuadrangle<long long>* pGeometry);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool DoesIgnoreInternalIntersection(_In_ CRavidQuadrangle<float>& geometry);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool DoesIgnoreInternalIntersection(_In_ CRavidQuadrangle<float>* pGeometry);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool DoesIgnoreInternalIntersection(_In_ CRavidQuadrangle<double>& geometry);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool DoesIgnoreInternalIntersection(_In_ CRavidQuadrangle<double>* pGeometry);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool DoesIgnoreInternalIntersection(_In_ CRavidCircle<int>& geometry);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool DoesIgnoreInternalIntersection(_In_ CRavidCircle<int>* pGeometry);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool DoesIgnoreInternalIntersection(_In_ CRavidCircle<long long>& geometry);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool DoesIgnoreInternalIntersection(_In_ CRavidCircle<long long>* pGeometry);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool DoesIgnoreInternalIntersection(_In_ CRavidCircle<float>& geometry);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool DoesIgnoreInternalIntersection(_In_ CRavidCircle<float>* pGeometry);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool DoesIgnoreInternalIntersection(_In_ CRavidCircle<double>& geometry);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool DoesIgnoreInternalIntersection(_In_ CRavidCircle<double>* pGeometry);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool DoesIgnoreInternalIntersection(_In_ CRavidEllipse<int>& geometry);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool DoesIgnoreInternalIntersection(_In_ CRavidEllipse<int>* pGeometry);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool DoesIgnoreInternalIntersection(_In_ CRavidEllipse<long long>& geometry);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool DoesIgnoreInternalIntersection(_In_ CRavidEllipse<long long>* pGeometry);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool DoesIgnoreInternalIntersection(_In_ CRavidEllipse<float>& geometry);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool DoesIgnoreInternalIntersection(_In_ CRavidEllipse<float>* pGeometry);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool DoesIgnoreInternalIntersection(_In_ CRavidEllipse<double>& geometry);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool DoesIgnoreInternalIntersection(_In_ CRavidEllipse<double>* pGeometry);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool DoesIgnoreInternalIntersection(_In_ CRavidPolygon& geometry);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool DoesIgnoreInternalIntersection(_In_ CRavidPolygon* pGeometry);
#pragma endregion

#pragma region [GetIgnoreInternalIntersection]
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIgnoreInternalIntersection(_In_ CRavidRect<int>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIgnoreInternalIntersection(_In_ CRavidRect<int>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIgnoreInternalIntersection(_In_ CRavidRect<long long>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIgnoreInternalIntersection(_In_ CRavidRect<long long>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIgnoreInternalIntersection(_In_ CRavidRect<float>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIgnoreInternalIntersection(_In_ CRavidRect<float>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIgnoreInternalIntersection(_In_ CRavidRect<double>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIgnoreInternalIntersection(_In_ CRavidRect<double>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIgnoreInternalIntersection(_In_ CRavidQuadrangle<int>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIgnoreInternalIntersection(_In_ CRavidQuadrangle<int>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIgnoreInternalIntersection(_In_ CRavidQuadrangle<long long>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIgnoreInternalIntersection(_In_ CRavidQuadrangle<long long>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIgnoreInternalIntersection(_In_ CRavidQuadrangle<float>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIgnoreInternalIntersection(_In_ CRavidQuadrangle<float>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIgnoreInternalIntersection(_In_ CRavidQuadrangle<double>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIgnoreInternalIntersection(_In_ CRavidQuadrangle<double>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIgnoreInternalIntersection(_In_ CRavidCircle<int>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIgnoreInternalIntersection(_In_ CRavidCircle<int>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIgnoreInternalIntersection(_In_ CRavidCircle<long long>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIgnoreInternalIntersection(_In_ CRavidCircle<long long>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIgnoreInternalIntersection(_In_ CRavidCircle<float>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIgnoreInternalIntersection(_In_ CRavidCircle<float>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIgnoreInternalIntersection(_In_ CRavidCircle<double>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIgnoreInternalIntersection(_In_ CRavidCircle<double>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIgnoreInternalIntersection(_In_ CRavidEllipse<int>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIgnoreInternalIntersection(_In_ CRavidEllipse<int>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIgnoreInternalIntersection(_In_ CRavidEllipse<long long>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIgnoreInternalIntersection(_In_ CRavidEllipse<long long>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIgnoreInternalIntersection(_In_ CRavidEllipse<float>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIgnoreInternalIntersection(_In_ CRavidEllipse<float>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIgnoreInternalIntersection(_In_ CRavidEllipse<double>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIgnoreInternalIntersection(_In_ CRavidEllipse<double>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIgnoreInternalIntersection(_In_ CRavidPolygon& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 충돌 중 내부 충돌을 제외한 충돌 여부를 판단하며, 충돌 좌표를 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIgnoreInternalIntersection(_In_ CRavidPolygon* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
#pragma endregion

#pragma region [GetIntersectionRegion]
	/**
	* 현재 객체와 상대 객체와의 공통 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersectionRegion(_In_ CRavidRect<int>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 공통 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersectionRegion(_In_ CRavidRect<int>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 공통 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersectionRegion(_In_ CRavidRect<long long>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 공통 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersectionRegion(_In_ CRavidRect<long long>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 공통 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersectionRegion(_In_ CRavidRect<float>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 공통 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersectionRegion(_In_ CRavidRect<float>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 공통 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersectionRegion(_In_ CRavidRect<double>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 공통 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersectionRegion(_In_ CRavidRect<double>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 공통 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersectionRegion(_In_ CRavidQuadrangle<int>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 공통 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersectionRegion(_In_ CRavidQuadrangle<int>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 공통 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersectionRegion(_In_ CRavidQuadrangle<long long>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 공통 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersectionRegion(_In_ CRavidQuadrangle<long long>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 공통 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersectionRegion(_In_ CRavidQuadrangle<float>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 공통 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersectionRegion(_In_ CRavidQuadrangle<float>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 공통 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersectionRegion(_In_ CRavidQuadrangle<double>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 공통 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersectionRegion(_In_ CRavidQuadrangle<double>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 공통 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersectionRegion(_In_ CRavidCircle<int>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 공통 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersectionRegion(_In_ CRavidCircle<int>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 공통 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersectionRegion(_In_ CRavidCircle<long long>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 공통 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersectionRegion(_In_ CRavidCircle<long long>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 공통 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersectionRegion(_In_ CRavidCircle<float>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 공통 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersectionRegion(_In_ CRavidCircle<float>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 공통 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersectionRegion(_In_ CRavidCircle<double>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 공통 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersectionRegion(_In_ CRavidCircle<double>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 공통 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersectionRegion(_In_ CRavidEllipse<int>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 공통 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersectionRegion(_In_ CRavidEllipse<int>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 공통 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersectionRegion(_In_ CRavidEllipse<long long>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 공통 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersectionRegion(_In_ CRavidEllipse<long long>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 공통 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersectionRegion(_In_ CRavidEllipse<float>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 공통 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersectionRegion(_In_ CRavidEllipse<float>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 공통 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersectionRegion(_In_ CRavidEllipse<double>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 공통 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersectionRegion(_In_ CRavidEllipse<double>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 공통 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersectionRegion(_In_ CRavidPolygon& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 공통 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetIntersectionRegion(_In_ CRavidPolygon* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
#pragma endregion

#pragma region [GetUnionRegion]
	/**
	* 현재 객체와 상대 객체와의 합 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetUnionRegion(_In_ CRavidRect<int>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 합 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetUnionRegion(_In_ CRavidRect<int>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 합 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetUnionRegion(_In_ CRavidRect<long long>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 합 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetUnionRegion(_In_ CRavidRect<long long>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 합 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetUnionRegion(_In_ CRavidRect<float>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 합 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetUnionRegion(_In_ CRavidRect<float>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 합 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetUnionRegion(_In_ CRavidRect<double>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 합 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetUnionRegion(_In_ CRavidRect<double>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 합 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetUnionRegion(_In_ CRavidQuadrangle<int>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 합 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetUnionRegion(_In_ CRavidQuadrangle<int>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 합 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetUnionRegion(_In_ CRavidQuadrangle<long long>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 합 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetUnionRegion(_In_ CRavidQuadrangle<long long>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 합 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetUnionRegion(_In_ CRavidQuadrangle<float>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 합 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetUnionRegion(_In_ CRavidQuadrangle<float>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 합 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetUnionRegion(_In_ CRavidQuadrangle<double>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 합 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetUnionRegion(_In_ CRavidQuadrangle<double>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 합 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetUnionRegion(_In_ CRavidCircle<int>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 합 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetUnionRegion(_In_ CRavidCircle<int>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 합 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetUnionRegion(_In_ CRavidCircle<long long>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 합 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetUnionRegion(_In_ CRavidCircle<long long>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 합 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetUnionRegion(_In_ CRavidCircle<float>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 합 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetUnionRegion(_In_ CRavidCircle<float>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 합 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetUnionRegion(_In_ CRavidCircle<double>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 합 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetUnionRegion(_In_ CRavidCircle<double>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 합 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetUnionRegion(_In_ CRavidEllipse<int>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 합 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetUnionRegion(_In_ CRavidEllipse<int>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 합 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetUnionRegion(_In_ CRavidEllipse<long long>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 합 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetUnionRegion(_In_ CRavidEllipse<long long>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 합 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetUnionRegion(_In_ CRavidEllipse<float>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 합 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetUnionRegion(_In_ CRavidEllipse<float>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 합 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetUnionRegion(_In_ CRavidEllipse<double>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 합 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetUnionRegion(_In_ CRavidEllipse<double>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 합 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetUnionRegion(_In_ CRavidPolygon& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 합 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetUnionRegion(_In_ CRavidPolygon* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
#pragma endregion

#pragma region [GetSubtractionRegion]
	/**
	* 현재 객체와 상대 객체와의 차 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetSubtractionRegion(_In_ CRavidRect<int>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 차 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetSubtractionRegion(_In_ CRavidRect<int>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 차 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetSubtractionRegion(_In_ CRavidRect<long long>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 차 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetSubtractionRegion(_In_ CRavidRect<long long>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 차 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetSubtractionRegion(_In_ CRavidRect<float>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 차 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetSubtractionRegion(_In_ CRavidRect<float>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 차 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetSubtractionRegion(_In_ CRavidRect<double>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 차 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetSubtractionRegion(_In_ CRavidRect<double>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 차 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetSubtractionRegion(_In_ CRavidQuadrangle<int>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 차 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetSubtractionRegion(_In_ CRavidQuadrangle<int>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 차 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetSubtractionRegion(_In_ CRavidQuadrangle<long long>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 차 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetSubtractionRegion(_In_ CRavidQuadrangle<long long>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 차 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetSubtractionRegion(_In_ CRavidQuadrangle<float>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 차 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetSubtractionRegion(_In_ CRavidQuadrangle<float>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 차 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetSubtractionRegion(_In_ CRavidQuadrangle<double>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 차 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetSubtractionRegion(_In_ CRavidQuadrangle<double>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 차 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetSubtractionRegion(_In_ CRavidCircle<int>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 차 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetSubtractionRegion(_In_ CRavidCircle<int>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 차 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetSubtractionRegion(_In_ CRavidCircle<long long>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 차 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetSubtractionRegion(_In_ CRavidCircle<long long>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 차 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetSubtractionRegion(_In_ CRavidCircle<float>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 차 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetSubtractionRegion(_In_ CRavidCircle<float>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 차 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetSubtractionRegion(_In_ CRavidCircle<double>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 차 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetSubtractionRegion(_In_ CRavidCircle<double>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 차 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetSubtractionRegion(_In_ CRavidEllipse<int>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 차 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetSubtractionRegion(_In_ CRavidEllipse<int>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 차 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetSubtractionRegion(_In_ CRavidEllipse<long long>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 차 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetSubtractionRegion(_In_ CRavidEllipse<long long>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 차 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetSubtractionRegion(_In_ CRavidEllipse<float>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 차 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetSubtractionRegion(_In_ CRavidEllipse<float>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 차 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetSubtractionRegion(_In_ CRavidEllipse<double>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 차 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetSubtractionRegion(_In_ CRavidEllipse<double>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 차 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetSubtractionRegion(_In_ CRavidPolygon& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 차 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetSubtractionRegion(_In_ CRavidPolygon* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
#pragma endregion

#pragma region [GetExclusiveOrRegion]
	/**
	* 현재 객체와 상대 객체와의 배타적 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetExclusiveOrRegion(_In_ CRavidRect<int>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 배타적 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetExclusiveOrRegion(_In_ CRavidRect<int>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 배타적 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetExclusiveOrRegion(_In_ CRavidRect<long long>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 배타적 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetExclusiveOrRegion(_In_ CRavidRect<long long>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 배타적 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetExclusiveOrRegion(_In_ CRavidRect<float>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 배타적 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetExclusiveOrRegion(_In_ CRavidRect<float>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 배타적 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetExclusiveOrRegion(_In_ CRavidRect<double>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 배타적 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetExclusiveOrRegion(_In_ CRavidRect<double>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 배타적 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetExclusiveOrRegion(_In_ CRavidQuadrangle<int>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 배타적 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetExclusiveOrRegion(_In_ CRavidQuadrangle<int>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 배타적 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetExclusiveOrRegion(_In_ CRavidQuadrangle<long long>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 배타적 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetExclusiveOrRegion(_In_ CRavidQuadrangle<long long>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 배타적 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetExclusiveOrRegion(_In_ CRavidQuadrangle<float>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 배타적 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetExclusiveOrRegion(_In_ CRavidQuadrangle<float>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 배타적 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetExclusiveOrRegion(_In_ CRavidQuadrangle<double>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 배타적 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetExclusiveOrRegion(_In_ CRavidQuadrangle<double>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 배타적 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetExclusiveOrRegion(_In_ CRavidCircle<int>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 배타적 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetExclusiveOrRegion(_In_ CRavidCircle<int>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 배타적 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetExclusiveOrRegion(_In_ CRavidCircle<long long>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 배타적 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetExclusiveOrRegion(_In_ CRavidCircle<long long>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 배타적 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetExclusiveOrRegion(_In_ CRavidCircle<float>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 배타적 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetExclusiveOrRegion(_In_ CRavidCircle<float>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 배타적 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetExclusiveOrRegion(_In_ CRavidCircle<double>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 배타적 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetExclusiveOrRegion(_In_ CRavidCircle<double>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 배타적 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetExclusiveOrRegion(_In_ CRavidEllipse<int>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 배타적 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetExclusiveOrRegion(_In_ CRavidEllipse<int>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 배타적 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetExclusiveOrRegion(_In_ CRavidEllipse<long long>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 배타적 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetExclusiveOrRegion(_In_ CRavidEllipse<long long>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 배타적 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetExclusiveOrRegion(_In_ CRavidEllipse<float>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 배타적 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetExclusiveOrRegion(_In_ CRavidEllipse<float>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 배타적 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetExclusiveOrRegion(_In_ CRavidEllipse<double>& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 배타적 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetExclusiveOrRegion(_In_ CRavidEllipse<double>* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 배타적 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] geometry : 계산할 상대 객체 (Reference)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetExclusiveOrRegion(_In_ CRavidPolygon& geometry, _Out_ CRavidGeometryArray* pGeometryArray);
	/**
	* 현재 객체와 상대 객체와의 배타적 영역을 계산하고, 그 영역만 반환합니다.
	* 알고리즘에 의해 여러 객체로 나눠지는 경우가 발생하기 때문에 Array 형태로 반환합니다.
	*
	* param[in] pGeometry : 계산할 상대 객체 (Pointer)
	* param[out] pGeometryArray : 계산 결과 객체 리스트 (Pointer)
	*
	* return value : 계산 중 오류가 발생하면 false 를 반환합니다.
	*/
	bool GetExclusiveOrRegion(_In_ CRavidPolygon* pGeometry, _Out_ CRavidGeometryArray* pGeometryArray);
#pragma endregion