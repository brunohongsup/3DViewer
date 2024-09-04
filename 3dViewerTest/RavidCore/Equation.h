#pragma once

#include <complex>
#include <vector>
#include <type_traits>

namespace Ravid
{
	namespace Mathematics
	{
		/**
		* 회귀 분석하는 클래스입니다.
		*/
		class AFX_EXT_CLASS CEquation : public CRavidObject
		{
		public:
			RavidPreventCopySelf(CEquation);

			CEquation() = delete;
			virtual ~CEquation();
			
			/**
			* 2개의 데이터를 이용하여 1차 방정식을 계산합니다.
			*
			* param[in] cpxA : 1번째 데이터
			* param[in] cpxB : 2번째 데이터
			* param[out] pResult : 결과 (Pointer)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			static Ravid::Algorithms::EAlgorithmResult LinearEquation(_In_ std::complex<double> cpxA, _In_ std::complex<double> cpxB, _Out_ std::complex<double>* pResult);

			/**
			* 3개의 데이터를 이용하여 2차 방정식을 계산합니다.
			*
			* param[in] cpxA : 1번째 데이터
			* param[in] cpxB : 2번째 데이터
			* param[in] cpxC : 3번째 데이터
			* param[out] pResult : 결과 (Pointer)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			static Ravid::Algorithms::EAlgorithmResult QuadraticEquation(_In_ std::complex<double> cpxA, _In_ std::complex<double> cpxB, _In_ std::complex<double> cpxC, _Out_ std::vector<std::complex<double> >* pResult);

			/**
			* 4개의 데이터를 이용하여 3차 방정식을 계산합니다.
			*
			* param[in] cpxA : 1번째 데이터
			* param[in] cpxB : 2번째 데이터
			* param[in] cpxC : 3번째 데이터
			* param[in] cpxD : 4번째 데이터
			* param[out] pResult : 결과 (Pointer)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			static Ravid::Algorithms::EAlgorithmResult CubicEquation(_In_ std::complex<double> cpxA, _In_ std::complex<double> cpxB, _In_ std::complex<double> cpxC, _In_ std::complex<double> cpxD, _Out_ std::vector<std::complex<double> >* pResult);

			/**
			* 5개의 데이터를 이용하여 4차 방정식을 계산합니다.
			*
			* param[in] cpxA : 1번째 데이터
			* param[in] cpxB : 2번째 데이터
			* param[in] cpxC : 3번째 데이터
			* param[in] cpxD : 4번째 데이터
			* param[in] cpxE : 5번째 데이터
			* param[out] pResult : 결과 (Pointer)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			static Ravid::Algorithms::EAlgorithmResult QuarticEquation(_In_ std::complex<double> cpxA, _In_ std::complex<double> cpxB, _In_ std::complex<double> cpxC, _In_ std::complex<double> cpxD, _In_ std::complex<double> cpxE, _Out_ std::vector<std::complex<double> >* pResult);

			/**
			* N개의 데이터를 이용하여 (N-1)차 방정식을 계산합니다.
			*
			* param[in] vctCoef : n 개의 데이터
			* param[out] pResult : 결과 (Pointer)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			static Ravid::Algorithms::EAlgorithmResult NDegreeEquation(_In_ std::vector<double> vctCoef, _Out_ std::vector<double>* pResult);

			/**
			* N개의 데이터를 이용하여 (N-1)차 방정식을 계산합니다.
			*
			* param[in] vctCpx : n 개의 데이터 (Pointer)
			* param[out] pResult : 결과 (Pointer)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			static Ravid::Algorithms::EAlgorithmResult NDegreeEquation(_In_ std::vector<std::complex<double> > vctCpx, _Out_ std::vector<std::complex<double> >* pResult);
		};
	}
}

