public:

#pragma region [CRavidRect]
	CRavidRect(_In_ Ravid::Algorithms::CRavidImage& ri, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ int p1y, _In_ Ravid::Algorithms::CRavidImage& ri, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ long long p1y, _In_ Ravid::Algorithms::CRavidImage& ri, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ float p1y, Ravid::Algorithms::CRavidImage& ri, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ double p1y, Ravid::Algorithms::CRavidImage& ri, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ int p1y, _In_ Ravid::Algorithms::CRavidImage& ri, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ long long p1y, _In_ Ravid::Algorithms::CRavidImage& ri, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ float p1y, _In_ Ravid::Algorithms::CRavidImage& ri, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ double p1y, _In_ Ravid::Algorithms::CRavidImage& ri, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ int p1y, _In_ Ravid::Algorithms::CRavidImage& ri, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ long long p1y, _In_ Ravid::Algorithms::CRavidImage& ri, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ float p1y, _In_ Ravid::Algorithms::CRavidImage& ri, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ double p1y, _In_ Ravid::Algorithms::CRavidImage& ri, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ int p1y, _In_ Ravid::Algorithms::CRavidImage& ri, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ long long p1y, _In_ Ravid::Algorithms::CRavidImage& ri, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ float p1y, _In_ Ravid::Algorithms::CRavidImage& ri, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ double p1y, _In_ Ravid::Algorithms::CRavidImage& ri, _In_opt_ double angle = 0);
	CRavidRect(_In_ const POINT& point, _In_ Ravid::Algorithms::CRavidImage& ri, _In_opt_ double angle = 0);
	CRavidRect(_In_ POINT* pPoint, _In_ Ravid::Algorithms::CRavidImage& ri, _In_opt_ double angle = 0);
	CRavidRect(_In_ const SIZE& size, _In_ Ravid::Algorithms::CRavidImage& ri, _In_opt_ double angle = 0);
	CRavidRect(_In_ SIZE* pSize, _In_ Ravid::Algorithms::CRavidImage& ri, _In_opt_ double angle = 0);
	CRavidRect(_In_ const CRavidPoint<int>& point, _In_ Ravid::Algorithms::CRavidImage& ri, _In_opt_ double angle = 0);
	CRavidRect(_In_ CRavidPoint<int>* pPoint, _In_ Ravid::Algorithms::CRavidImage& ri, _In_opt_ double angle = 0);
	CRavidRect(_In_ const CRavidPoint<long long>& point, _In_ Ravid::Algorithms::CRavidImage& ri, _In_opt_ double angle = 0);
	CRavidRect(_In_ CRavidPoint<long long>* pPoint, _In_ Ravid::Algorithms::CRavidImage& ri, _In_opt_ double angle = 0);
	CRavidRect(_In_ const CRavidPoint<float>& point, _In_ Ravid::Algorithms::CRavidImage& ri, _In_opt_ double angle = 0);
	CRavidRect(_In_ CRavidPoint<float>* pPoint, _In_ Ravid::Algorithms::CRavidImage& ri, _In_opt_ double angle = 0);
	CRavidRect(_In_ const CRavidPoint<double>& point, _In_ Ravid::Algorithms::CRavidImage& ri, _In_opt_ double angle = 0);
	CRavidRect(_In_ CRavidPoint<double>* pPoint, _In_ Ravid::Algorithms::CRavidImage& ri, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ int p1y, _In_ int p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ int p1y, _In_ int p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ int p1y, _In_ int p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ int p1y, _In_ int p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ int p1y, _In_ long long p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ int p1y, _In_ long long p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ int p1y, _In_ long long p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ int p1y, _In_ long long p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ int p1y, _In_ float p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ int p1y, _In_ float p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ int p1y, _In_ float p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ int p1y, _In_ float p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ int p1y, _In_ double p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ int p1y, _In_ double p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ int p1y, _In_ double p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ int p1y, _In_ double p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ long long p1y, _In_ int p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ long long p1y, _In_ int p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ long long p1y, _In_ int p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ long long p1y, _In_ int p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ long long p1y, _In_ long long p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ long long p1y, _In_ long long p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ long long p1y, _In_ long long p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ long long p1y, _In_ long long p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ long long p1y, _In_ float p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ long long p1y, _In_ float p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ long long p1y, _In_ float p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ long long p1y, _In_ float p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ long long p1y, _In_ double p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ long long p1y, _In_ double p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ long long p1y, _In_ double p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ long long p1y, _In_ double p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ float p1y, _In_ int p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ float p1y, _In_ int p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ float p1y, _In_ int p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ float p1y, _In_ int p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ float p1y, _In_ long long p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ float p1y, _In_ long long p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ float p1y, _In_ long long p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ float p1y, _In_ long long p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ float p1y, _In_ float p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ float p1y, _In_ float p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ float p1y, _In_ float p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ float p1y, _In_ float p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ float p1y, _In_ double p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ float p1y, _In_ double p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ float p1y, _In_ double p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ float p1y, _In_ double p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ double p1y, _In_ int p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ double p1y, _In_ int p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ double p1y, _In_ int p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ double p1y, _In_ int p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ double p1y, _In_ long long p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ double p1y, _In_ long long p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ double p1y, _In_ long long p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ double p1y, _In_ long long p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ double p1y, _In_ float p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ double p1y, _In_ float p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ double p1y, _In_ float p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ double p1y, _In_ float p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ double p1y, _In_ double p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ double p1y, _In_ double p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ double p1y, _In_ double p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ int p1x, _In_ double p1y, _In_ double p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ int p1y, _In_ int p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ int p1y, _In_ int p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ int p1y, _In_ int p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ int p1y, _In_ int p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ int p1y, _In_ long long p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ int p1y, _In_ long long p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ int p1y, _In_ long long p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ int p1y, _In_ long long p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ int p1y, _In_ float p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ int p1y, _In_ float p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ int p1y, _In_ float p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ int p1y, _In_ float p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ int p1y, _In_ double p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ int p1y, _In_ double p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ int p1y, _In_ double p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ int p1y, _In_ double p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ long long p1y, _In_ int p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ long long p1y, _In_ int p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ long long p1y, _In_ int p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ long long p1y, _In_ int p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ long long p1y, _In_ long long p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ long long p1y, _In_ long long p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ long long p1y, _In_ long long p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ long long p1y, _In_ long long p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ long long p1y, _In_ float p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ long long p1y, _In_ float p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ long long p1y, _In_ float p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ long long p1y, _In_ float p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ long long p1y, _In_ double p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ long long p1y, _In_ double p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ long long p1y, _In_ double p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ long long p1y, _In_ double p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ float p1y, _In_ int p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ float p1y, _In_ int p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ float p1y, _In_ int p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ float p1y, _In_ int p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ float p1y, _In_ long long p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ float p1y, _In_ long long p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ float p1y, _In_ long long p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ float p1y, _In_ long long p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ float p1y, _In_ float p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ float p1y, _In_ float p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ float p1y, _In_ float p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ float p1y, _In_ float p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ float p1y, _In_ double p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ float p1y, _In_ double p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ float p1y, _In_ double p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ float p1y, _In_ double p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ double p1y, _In_ int p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ double p1y, _In_ int p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ double p1y, _In_ int p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ double p1y, _In_ int p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ double p1y, _In_ long long p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ double p1y, _In_ long long p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ double p1y, _In_ long long p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ double p1y, _In_ long long p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ double p1y, _In_ float p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ double p1y, _In_ float p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ double p1y, _In_ float p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ double p1y, _In_ float p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ double p1y, _In_ double p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ double p1y, _In_ double p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ double p1y, _In_ double p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ long long p1x, _In_ double p1y, _In_ double p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ int p1y, _In_ int p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ int p1y, _In_ int p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ int p1y, _In_ int p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ int p1y, _In_ int p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ int p1y, _In_ long long p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ int p1y, _In_ long long p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ int p1y, _In_ long long p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ int p1y, _In_ long long p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ int p1y, _In_ float p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ int p1y, _In_ float p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ int p1y, _In_ float p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ int p1y, _In_ float p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ int p1y, _In_ double p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ int p1y, _In_ double p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ int p1y, _In_ double p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ int p1y, _In_ double p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ long long p1y, _In_ int p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ long long p1y, _In_ int p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ long long p1y, _In_ int p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ long long p1y, _In_ int p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ long long p1y, _In_ long long p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ long long p1y, _In_ long long p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ long long p1y, _In_ long long p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ long long p1y, _In_ long long p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ long long p1y, _In_ float p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ long long p1y, _In_ float p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ long long p1y, _In_ float p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ long long p1y, _In_ float p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ long long p1y, _In_ double p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ long long p1y, _In_ double p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ long long p1y, _In_ double p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ long long p1y, _In_ double p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ float p1y, _In_ int p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ float p1y, _In_ int p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ float p1y, _In_ int p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ float p1y, _In_ int p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ float p1y, _In_ long long p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ float p1y, _In_ long long p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ float p1y, _In_ long long p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ float p1y, _In_ long long p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ float p1y, _In_ float p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ float p1y, _In_ float p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ float p1y, _In_ float p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ float p1y, _In_ float p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ float p1y, _In_ double p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ float p1y, _In_ double p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ float p1y, _In_ double p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ float p1y, _In_ double p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ double p1y, _In_ int p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ double p1y, _In_ int p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ double p1y, _In_ int p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ double p1y, _In_ int p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ double p1y, _In_ long long p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ double p1y, _In_ long long p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ double p1y, _In_ long long p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ double p1y, _In_ long long p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ double p1y, _In_ float p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ double p1y, _In_ float p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ double p1y, _In_ float p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ double p1y, _In_ float p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ double p1y, _In_ double p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ double p1y, _In_ double p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ double p1y, _In_ double p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ float p1x, _In_ double p1y, _In_ double p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ int p1y, _In_ int p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ int p1y, _In_ int p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ int p1y, _In_ int p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ int p1y, _In_ int p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ int p1y, _In_ long long p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ int p1y, _In_ long long p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ int p1y, _In_ long long p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ int p1y, _In_ long long p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ int p1y, _In_ float p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ int p1y, _In_ float p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ int p1y, _In_ float p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ int p1y, _In_ float p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ int p1y, _In_ double p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ int p1y, _In_ double p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ int p1y, _In_ double p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ int p1y, _In_ double p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ long long p1y, _In_ int p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ long long p1y, _In_ int p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ long long p1y, _In_ int p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ long long p1y, _In_ int p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ long long p1y, _In_ long long p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ long long p1y, _In_ long long p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ long long p1y, _In_ long long p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ long long p1y, _In_ long long p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ long long p1y, _In_ float p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ long long p1y, _In_ float p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ long long p1y, _In_ float p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ long long p1y, _In_ float p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ long long p1y, _In_ double p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ long long p1y, _In_ double p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ long long p1y, _In_ double p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ long long p1y, _In_ double p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ float p1y, _In_ int p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ float p1y, _In_ int p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ float p1y, _In_ int p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ float p1y, _In_ int p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ float p1y, _In_ long long p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ float p1y, _In_ long long p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ float p1y, _In_ long long p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ float p1y, _In_ long long p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ float p1y, _In_ float p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ float p1y, _In_ float p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ float p1y, _In_ float p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ float p1y, _In_ float p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ float p1y, _In_ double p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ float p1y, _In_ double p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ float p1y, _In_ double p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ float p1y, _In_ double p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ double p1y, _In_ int p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ double p1y, _In_ int p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ double p1y, _In_ int p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ double p1y, _In_ int p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ double p1y, _In_ long long p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ double p1y, _In_ long long p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ double p1y, _In_ long long p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ double p1y, _In_ long long p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ double p1y, _In_ float p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ double p1y, _In_ float p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ double p1y, _In_ float p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ double p1y, _In_ float p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ double p1y, _In_ double p2x, _In_ int p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ double p1y, _In_ double p2x, _In_ long long p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ double p1y, _In_ double p2x, _In_ float p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ double p1x, _In_ double p1y, _In_ double p2x, _In_ double p2y, _In_opt_ double angle = 0);
	CRavidRect(_In_ const POINT& point, _In_opt_ double angle = 0);
	CRavidRect(_In_ POINT* pPoint, _In_opt_ double angle = 0);
	CRavidRect(_In_ const SIZE& size, _In_opt_ double angle = 0);
	CRavidRect(_In_ SIZE* pSize, _In_opt_ double angle = 0);
	CRavidRect(_In_ const CRavidPoint<int>& point, _In_opt_ double angle = 0);
	CRavidRect(_In_ CRavidPoint<int>* pPoint, _In_opt_ double angle = 0);
	CRavidRect(_In_ const CRavidPoint<long long>& point, _In_opt_ double angle = 0);
	CRavidRect(_In_ CRavidPoint<long long>* pPoint, _In_opt_ double angle = 0);
	CRavidRect(_In_ const CRavidPoint<float>& point, _In_opt_ double angle = 0);
	CRavidRect(_In_ CRavidPoint<float>* pPoint, _In_opt_ double angle = 0);
	CRavidRect(_In_ const CRavidPoint<double>& point, _In_opt_ double angle = 0);
	CRavidRect(_In_ CRavidPoint<double>* pPoint, _In_opt_ double angle = 0);
	CRavidRect(_In_ const POINT& point1, _In_ const POINT& point2, _In_opt_ double angle = 0);
	CRavidRect(_In_ POINT* pPoint1, _In_ POINT* pPoint2, _In_opt_ double angle = 0);
	CRavidRect(_In_ const CRavidPoint<int>& point1, _In_ const CRavidPoint<int>& point2, _In_opt_ double angle = 0);
	CRavidRect(_In_ CRavidPoint<int>* pPoint1, _In_ CRavidPoint<int>* pPoint2, _In_opt_ double angle = 0);
	CRavidRect(_In_ const CRavidPoint<long long>& point1, _In_ const CRavidPoint<long long>& point2, _In_opt_ double angle = 0);
	CRavidRect(_In_ CRavidPoint<long long>* pPoint1, _In_ CRavidPoint<long long>* pPoint2, _In_opt_ double angle = 0);
	CRavidRect(_In_ const CRavidPoint<float>& point1, _In_ const CRavidPoint<float>& point2, _In_opt_ double angle = 0);
	CRavidRect(_In_ CRavidPoint<float>* pPoint1, _In_ CRavidPoint<float>* pPoint2, _In_opt_ double angle = 0);
	CRavidRect(_In_ const CRavidPoint<double>& point1, _In_ const CRavidPoint<double>& point2, _In_opt_ double angle = 0);
	CRavidRect(_In_ CRavidPoint<double>* pPoint1, _In_ CRavidPoint<double>* pPoint2, _In_opt_ double angle = 0);
	CRavidRect(_In_ const CRavidLine<int>& line, _In_opt_ double angle = 0);
	CRavidRect(_In_ CRavidLine<int>* pLine, _In_opt_ double angle = 0);
	CRavidRect(_In_ const CRavidLine<long long>& line, _In_opt_ double angle = 0);
	CRavidRect(_In_ CRavidLine<long long>* pLine, _In_opt_ double angle = 0);
	CRavidRect(_In_ const CRavidLine<float>& line, _In_opt_ double angle = 0);
	CRavidRect(_In_ CRavidLine<float>* pLine, _In_opt_ double angle = 0);
	CRavidRect(_In_ const CRavidLine<double>& line, _In_opt_ double angle = 0);
	CRavidRect(_In_ CRavidLine<double>* pLine, _In_opt_ double angle = 0);
	CRavidRect(_In_ const RECT& rect, _In_opt_ double angle = 0);
	CRavidRect(_In_ RECT* pRect, _In_opt_ double angle = 0);
	CRavidRect(_In_ const CRavidRect<int>& rect);
	CRavidRect(_In_ CRavidRect<int>* pRect);
	CRavidRect(_In_ const CRavidRect<long long>& rect);
	CRavidRect(_In_ CRavidRect<long long>* pRect);
	CRavidRect(_In_ const CRavidRect<float>& rect);
	CRavidRect(_In_ CRavidRect<float>* pRect);
	CRavidRect(_In_ const CRavidRect<double>& rect);
	CRavidRect(_In_ CRavidRect<double>* pRect);
	CRavidRect(_In_ const CRavidQuadrangle<int>& quad);
	CRavidRect(_In_ CRavidQuadrangle<int>* pQuad);
	CRavidRect(_In_ const CRavidQuadrangle<long long>& quad);
	CRavidRect(_In_ CRavidQuadrangle<long long>* pQuad);
	CRavidRect(_In_ const CRavidQuadrangle<float>& quad);
	CRavidRect(_In_ CRavidQuadrangle<float>* pQuad);
	CRavidRect(_In_ const CRavidQuadrangle<double>& quad);
	CRavidRect(_In_ CRavidQuadrangle<double>* pQuad);
	CRavidRect(_In_ const CRavidCircle<int>& cir);
	CRavidRect(_In_ CRavidCircle<int>* pCir);
	CRavidRect(_In_ const CRavidCircle<long long>& cir);
	CRavidRect(_In_ CRavidCircle<long long>* pCir);
	CRavidRect(_In_ const CRavidCircle<float>& cir);
	CRavidRect(_In_ CRavidCircle<float>* pCir);
	CRavidRect(_In_ const CRavidCircle<double>& cir);
	CRavidRect(_In_ CRavidCircle<double>* pCir);
	CRavidRect(_In_ const CRavidEllipse<int>& ell);
	CRavidRect(_In_ CRavidEllipse<int>* pEll);
	CRavidRect(_In_ const CRavidEllipse<long long>& ell);
	CRavidRect(_In_ CRavidEllipse<long long>* pEll);
	CRavidRect(_In_ const CRavidEllipse<float>& ell);
	CRavidRect(_In_ CRavidEllipse<float>* pEll);
	CRavidRect(_In_ const CRavidEllipse<double>& ell);
	CRavidRect(_In_ CRavidEllipse<double>* pEll);
	CRavidRect(_In_ const CRavidPolygon& rpg);
	CRavidRect(_In_ CRavidPolygon* pRpg);
#pragma endregion

#pragma region [SetRect]
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ int top, _In_ int right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ int top, _In_ int right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ int top, _In_ int right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ int top, _In_ int right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ int top, _In_ long long right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ int top, _In_ long long right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ int top, _In_ long long right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ int top, _In_ long long right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ int top, _In_ float right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ int top, _In_ float right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ int top, _In_ float right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ int top, _In_ float right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ int top, _In_ double right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ int top, _In_ double right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ int top, _In_ double right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ int top, _In_ double right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ long long top, _In_ int right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ long long top, _In_ int right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ long long top, _In_ int right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ long long top, _In_ int right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ long long top, _In_ long long right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ long long top, _In_ long long right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ long long top, _In_ long long right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ long long top, _In_ long long right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ long long top, _In_ float right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ long long top, _In_ float right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ long long top, _In_ float right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ long long top, _In_ float right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ long long top, _In_ double right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ long long top, _In_ double right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ long long top, _In_ double right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ long long top, _In_ double right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ float top, _In_ int right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ float top, _In_ int right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ float top, _In_ int right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ float top, _In_ int right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ float top, _In_ long long right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ float top, _In_ long long right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ float top, _In_ long long right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ float top, _In_ long long right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ float top, _In_ float right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ float top, _In_ float right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ float top, _In_ float right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ float top, _In_ float right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ float top, _In_ double right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ float top, _In_ double right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ float top, _In_ double right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ float top, _In_ double right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ double top, _In_ int right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ double top, _In_ int right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ double top, _In_ int right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ double top, _In_ int right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ double top, _In_ long long right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ double top, _In_ long long right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ double top, _In_ long long right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ double top, _In_ long long right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ double top, _In_ float right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ double top, _In_ float right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ double top, _In_ float right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ double top, _In_ float right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ double top, _In_ double right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ double top, _In_ double right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ double top, _In_ double right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ int left, _In_ double top, _In_ double right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ int top, _In_ int right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ int top, _In_ int right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ int top, _In_ int right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ int top, _In_ int right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ int top, _In_ long long right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ int top, _In_ long long right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ int top, _In_ long long right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ int top, _In_ long long right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ int top, _In_ float right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ int top, _In_ float right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ int top, _In_ float right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ int top, _In_ float right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ int top, _In_ double right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ int top, _In_ double right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ int top, _In_ double right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ int top, _In_ double right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ long long top, _In_ int right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ long long top, _In_ int right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ long long top, _In_ int right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ long long top, _In_ int right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ long long top, _In_ long long right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ long long top, _In_ long long right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ long long top, _In_ long long right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ long long top, _In_ long long right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ long long top, _In_ float right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ long long top, _In_ float right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ long long top, _In_ float right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ long long top, _In_ float right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ long long top, _In_ double right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ long long top, _In_ double right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ long long top, _In_ double right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ long long top, _In_ double right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ float top, _In_ int right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ float top, _In_ int right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ float top, _In_ int right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ float top, _In_ int right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ float top, _In_ long long right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ float top, _In_ long long right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ float top, _In_ long long right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ float top, _In_ long long right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ float top, _In_ float right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ float top, _In_ float right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ float top, _In_ float right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ float top, _In_ float right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ float top, _In_ double right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ float top, _In_ double right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ float top, _In_ double right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ float top, _In_ double right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ double top, _In_ int right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ double top, _In_ int right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ double top, _In_ int right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ double top, _In_ int right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ double top, _In_ long long right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ double top, _In_ long long right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ double top, _In_ long long right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ double top, _In_ long long right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ double top, _In_ float right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ double top, _In_ float right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ double top, _In_ float right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ double top, _In_ float right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ double top, _In_ double right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ double top, _In_ double right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ double top, _In_ double right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ long long left, _In_ double top, _In_ double right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ int top, _In_ int right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ int top, _In_ int right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ int top, _In_ int right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ int top, _In_ int right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ int top, _In_ long long right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ int top, _In_ long long right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ int top, _In_ long long right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ int top, _In_ long long right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ int top, _In_ float right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ int top, _In_ float right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ int top, _In_ float right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ int top, _In_ float right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ int top, _In_ double right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ int top, _In_ double right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ int top, _In_ double right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ int top, _In_ double right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ long long top, _In_ int right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ long long top, _In_ int right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ long long top, _In_ int right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ long long top, _In_ int right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ long long top, _In_ long long right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ long long top, _In_ long long right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ long long top, _In_ long long right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ long long top, _In_ long long right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ long long top, _In_ float right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ long long top, _In_ float right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ long long top, _In_ float right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ long long top, _In_ float right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ long long top, _In_ double right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ long long top, _In_ double right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ long long top, _In_ double right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ long long top, _In_ double right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ float top, _In_ int right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ float top, _In_ int right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ float top, _In_ int right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ float top, _In_ int right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ float top, _In_ long long right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ float top, _In_ long long right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ float top, _In_ long long right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ float top, _In_ long long right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ float top, _In_ float right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ float top, _In_ float right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ float top, _In_ float right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ float top, _In_ float right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ float top, _In_ double right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ float top, _In_ double right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ float top, _In_ double right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ float top, _In_ double right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ double top, _In_ int right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ double top, _In_ int right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ double top, _In_ int right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ double top, _In_ int right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ double top, _In_ long long right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ double top, _In_ long long right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ double top, _In_ long long right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ double top, _In_ long long right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ double top, _In_ float right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ double top, _In_ float right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ double top, _In_ float right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ double top, _In_ float right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ double top, _In_ double right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ double top, _In_ double right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ double top, _In_ double right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ float left, _In_ double top, _In_ double right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ int top, _In_ int right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ int top, _In_ int right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ int top, _In_ int right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ int top, _In_ int right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ int top, _In_ long long right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ int top, _In_ long long right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ int top, _In_ long long right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ int top, _In_ long long right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ int top, _In_ float right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ int top, _In_ float right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ int top, _In_ float right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ int top, _In_ float right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ int top, _In_ double right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ int top, _In_ double right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ int top, _In_ double right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ int top, _In_ double right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ long long top, _In_ int right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ long long top, _In_ int right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ long long top, _In_ int right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ long long top, _In_ int right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ long long top, _In_ long long right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ long long top, _In_ long long right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ long long top, _In_ long long right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ long long top, _In_ long long right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ long long top, _In_ float right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ long long top, _In_ float right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ long long top, _In_ float right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ long long top, _In_ float right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ long long top, _In_ double right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ long long top, _In_ double right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ long long top, _In_ double right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ long long top, _In_ double right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ float top, _In_ int right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ float top, _In_ int right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ float top, _In_ int right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ float top, _In_ int right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ float top, _In_ long long right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ float top, _In_ long long right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ float top, _In_ long long right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ float top, _In_ long long right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ float top, _In_ float right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ float top, _In_ float right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ float top, _In_ float right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ float top, _In_ float right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ float top, _In_ double right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ float top, _In_ double right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ float top, _In_ double right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ float top, _In_ double right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ double top, _In_ int right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ double top, _In_ int right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ double top, _In_ int right, _In_ float bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ double top, _In_ int right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ double top, _In_ long long right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ double top, _In_ long long right, _In_ long long bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ double top, _In_ long long right, _In_ float bottom, _In_opt_ double angle = 0.); 
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ double top, _In_ long long right, _In_ double bottom, _In_opt_ double angle = 0.); 
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ double top, _In_ float right, _In_ int bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ double top, _In_ float right, _In_ long long bottom, _In_opt_ double angle = 0.); 
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ double top, _In_ float right, _In_ float bottom, _In_opt_ double angle = 0.); 
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ double top, _In_ float right, _In_ double bottom, _In_opt_ double angle = 0.); 
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ double top, _In_ double right, _In_ int bottom, _In_opt_ double angle = 0.); 
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ double top, _In_ double right, _In_ long long bottom, _In_opt_ double angle = 0.); 
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ double top, _In_ double right, _In_ float bottom, _In_opt_ double angle = 0.); 
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] left : x축 최소값
	* param[in] top : y축 최소값
	* param[in] right : x축 최대값
	* param[in] bottom : y축 최대값
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ double left, _In_ double top, _In_ double right, _In_ double bottom, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] refGeometry : RECT 구조체 (Reference)
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ RECT& refGeometry, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 설정하는 기능입니다.
	*
	* param[in] pGeometry : RECT 구조체 (Pointer)
	* param[in_opt] angle : 회전 각도
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ RECT* pGeometry, _In_opt_ double angle = 0.);
	/*
	* 현재 객체를 입력된 객체로 설정합니다.
	*
	* param[in] refGeometry : CRavidRect 객체 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ CRavidRect<int>& refGeometry);
	/*
	* 현재 객체를 입력된 객체로 설정합니다.
	*
	* param[in] pGeometry : CRavidRect 객체 (Pointer)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ CRavidRect<int>* pGeometry);
	/*
	* 현재 객체를 입력된 객체로 설정합니다.
	*
	* param[in] refGeometry : CRavidRect 객체 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ CRavidRect<long long>& refGeometry);
	/*
	* 현재 객체를 입력된 객체로 설정합니다.
	*
	* param[in] pGeometry : CRavidRect 객체 (Pointer)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ CRavidRect<long long>* pGeometry);
	/*
	* 현재 객체를 입력된 객체로 설정합니다.
	*
	* param[in] refGeometry : CRavidRect 객체 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ CRavidRect<float>& refGeometry);
	/*
	* 현재 객체를 입력된 객체로 설정합니다.
	*
	* param[in] pGeometry : CRavidRect 객체 (Pointer)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ CRavidRect<float>* pGeometry);
	/*
	* 현재 객체를 입력된 객체로 설정합니다.
	*
	* param[in] refGeometry : CRavidRect 객체 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ CRavidRect<double>& refGeometry);
	/*
	* 현재 객체를 입력된 객체로 설정합니다.
	*
	* param[in] pGeometry : CRavidRect 객체 (Pointer)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ CRavidRect<double>* pGeometry);
	/*
	* 현재 객체를 입력된 객체의 외곽으로 0도의 직사각형으로 설정합니다.
	*
	* param[in] refGeometry : CRavidQuadrangle 객체 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ CRavidQuadrangle<int>& refGeometry);
	/*
	* 현재 객체를 입력된 객체의 외곽으로 0도의 직사각형으로 설정합니다.
	*
	* param[in] pGeometry : CRavidQuadrangle 객체 (Pointer)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ CRavidQuadrangle<int>* pGeometry);
	/*
	* 현재 객체를 입력된 객체의 외곽으로 0도의 직사각형으로 설정합니다.
	*
	* param[in] refGeometry : CRavidQuadrangle 객체 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ CRavidQuadrangle<long long>& refGeometry);
	/*
	* 현재 객체를 입력된 객체의 외곽으로 0도의 직사각형으로 설정합니다.
	*
	* param[in] pGeometry : CRavidQuadrangle 객체 (Pointer)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ CRavidQuadrangle<long long>* pGeometry);
	/*
	* 현재 객체를 입력된 객체의 외곽으로 0도의 직사각형으로 설정합니다.
	*
	* param[in] refGeometry : CRavidQuadrangle 객체 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ CRavidQuadrangle<float>& refGeometry);
	/*
	* 현재 객체를 입력된 객체의 외곽으로 0도의 직사각형으로 설정합니다.
	*
	* param[in] pGeometry : CRavidQuadrangle 객체 (Pointer)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ CRavidQuadrangle<float>* pGeometry);
	/*
	* 현재 객체를 입력된 객체의 외곽으로 0도의 직사각형으로 설정합니다.
	*
	* param[in] refGeometry : CRavidQuadrangle 객체 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ CRavidQuadrangle<double>& refGeometry);
	/*
	* 현재 객체를 입력된 객체의 외곽으로 0도의 직사각형으로 설정합니다.
	*
	* param[in] pGeometry : CRavidQuadrangle 객체 (Pointer)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ CRavidQuadrangle<double>* pGeometry);
	/*
	* 현재 객체를 입력된 객체의 외곽으로 0도의 직사각형으로 설정합니다.
	*
	* param[in] refGeometry : CRavidCircle 객체 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ CRavidCircle<int>& refGeometry);
	/*
	* 현재 객체를 입력된 객체의 외곽으로 0도의 직사각형으로 설정합니다.
	*
	* param[in] pGeometry : CRavidCircle 객체 (Pointer)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ CRavidCircle<int>* pGeometry);
	/*
	* 현재 객체를 입력된 객체의 외곽으로 0도의 직사각형으로 설정합니다.
	*
	* param[in] refGeometry : CRavidCircle 객체 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ CRavidCircle<long long>& refGeometry);
	/*
	* 현재 객체를 입력된 객체의 외곽으로 0도의 직사각형으로 설정합니다.
	*
	* param[in] pGeometry : CRavidCircle 객체 (Pointer)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ CRavidCircle<long long>* pGeometry);
	/*
	* 현재 객체를 입력된 객체의 외곽으로 0도의 직사각형으로 설정합니다.
	*
	* param[in] refGeometry : CRavidCircle 객체 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ CRavidCircle<float>& refGeometry);
	/*
	* 현재 객체를 입력된 객체의 외곽으로 0도의 직사각형으로 설정합니다.
	*
	* param[in] pGeometry : CRavidCircle 객체 (Pointer)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ CRavidCircle<float>* pGeometry);
	/*
	* 현재 객체를 입력된 객체의 외곽으로 0도의 직사각형으로 설정합니다.
	*
	* param[in] refGeometry : CRavidCircle 객체 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ CRavidCircle<double>& refGeometry);
	/*
	* 현재 객체를 입력된 객체의 외곽으로 0도의 직사각형으로 설정합니다.
	*
	* param[in] pGeometry : CRavidCircle 객체 (Pointer)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ CRavidCircle<double>* pGeometry);
	/*
	* 현재 객체를 입력된 객체의 외곽으로 0도의 직사각형으로 설정합니다.
	*
	* param[in] refGeometry : CRavidEllipse 객체 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ CRavidEllipse<int>& refGeometry);
	/*
	* 현재 객체를 입력된 객체의 외곽으로 0도의 직사각형으로 설정합니다.
	*
	* param[in] pGeometry : CRavidEllipse 객체 (Pointer)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ CRavidEllipse<int>* pGeometry);
	/*
	* 현재 객체를 입력된 객체의 외곽으로 0도의 직사각형으로 설정합니다.
	*
	* param[in] refGeometry : CRavidEllipse 객체 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ CRavidEllipse<long long>& refGeometry);
	/*
	* 현재 객체를 입력된 객체의 외곽으로 0도의 직사각형으로 설정합니다.
	*
	* param[in] pGeometry : CRavidEllipse 객체 (Pointer)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ CRavidEllipse<long long>* pGeometry);
	/*
	* 현재 객체를 입력된 객체의 외곽으로 0도의 직사각형으로 설정합니다.
	*
	* param[in] refGeometry : CRavidEllipse 객체 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ CRavidEllipse<float>& refGeometry);
	/*
	* 현재 객체를 입력된 객체의 외곽으로 0도의 직사각형으로 설정합니다.
	*
	* param[in] pGeometry : CRavidEllipse 객체 (Pointer)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ CRavidEllipse<float>* pGeometry);
	/*
	* 현재 객체를 입력된 객체의 외곽으로 0도의 직사각형으로 설정합니다.
	*
	* param[in] refGeometry : CRavidEllipse 객체 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ CRavidEllipse<double>& refGeometry);
	/*
	* 현재 객체를 입력된 객체의 외곽으로 0도의 직사각형으로 설정합니다.
	*
	* param[in] pGeometry : CRavidEllipse 객체 (Pointer)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ CRavidEllipse<double>* pGeometry);
	/*
	* 현재 객체를 입력된 객체의 외곽으로 0도의 직사각형으로 설정합니다.
	*
	* param[in] refGeometry : CRavidPolygon 객체 (Reference)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ CRavidPolygon& refGeometry);
	/*
	* 현재 객체를 입력된 객체의 외곽으로 0도의 직사각형으로 설정합니다.
	*
	* param[in] pGeometry : CRavidPolygon 객체 (Pointer)
	*
	* return value : 실패하면 false 를 반환합니다.
	*/
	bool SetRect(_In_ CRavidPolygon* pGeometry);
#pragma endregion