#pragma once

#include <vector>

namespace Ravid
{
	namespace Mathematics
	{
		/**
		* 대수관계의 유한체 계산을 위한 클래스입니다.
		* 사용자는 상속받아 원하는 설정을 해야합니다.
		* 일반적인 사칙연산이 아닌 대수의 연산을 합니다.
		*/
		class AFX_EXT_CLASS CFiniteField : public CRavidObject
		{
		protected:
			explicit CFiniteField(_In_ int nPrim, _In_ int nFCR, _In_ int nGenerator);
			virtual ~CFiniteField();

			void InitTables(_In_ int nPrim);

			int GetBitLength(_In_ int nNumber);
			int MathModulo(int nDividend, int nDivisor);

			int CLMul(_In_ int nA, _In_ int nB);
			int CLDiv(_In_ int nDividend, _In_ int nDivisor);

			int Add(_In_ int nA, _In_ int nB);
			int Sub(_In_ int nA, _In_ int nB);
			int Mul(_In_ int nA, _In_ int nB);
			int Div(_In_ int nA, _In_ int nB);
			int Pow(_In_ int nA, _In_ int nPower);
			int Inv(_In_ int nA);

			int MulNoLUT(_In_ int nA, _In_ int nB, _In_ int nPrim = 0);
			int MulOpNoLUT(_In_ int nA, _In_ int nB, _In_ int nPrim = 0, _In_ int nFieldCharacFull = 256, _In_ bool bCarryless = true);

			std::vector<int> PolyScale(_In_ const std::vector<int>& vctP, _In_ int nScale);
			std::vector<int> PolyAdd(_In_ const std::vector<int>& vctP, _In_ const std::vector<int>& vctQ);
			std::vector<int> PolyMul(_In_ const std::vector<int>& vctP, _In_ const std::vector<int>& vctQ);
			bool PolyDiv(_In_ const std::vector<int>& vctDividend, _In_ const std::vector<int>& vctDivisor, _Out_ std::vector<int>& vctQuotient, _Out_ std::vector<int>& vctRemainder);

			int PolyEval(_In_ const std::vector<int>& vctPoly, _In_ int nA);

			int pExpCacheGF[512];
			int pLogCacheGF[256];

			int m_nFCR;
			int m_nGenerator;
		};
	}
}

