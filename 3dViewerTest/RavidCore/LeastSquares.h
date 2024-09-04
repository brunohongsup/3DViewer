#pragma once

#include <memory>
#include <vector>

namespace Ravid
{
	namespace Mathematics
	{
		class CInternalLeastSquares;
		
		/**
		* 여러 좌표를 이용하여 회귀분석을 통해 방정식을 도출하는 클래스입니다.
		* 결정계수는 데이터의 종속성을 표현합니다.
		*/
		class AFX_EXT_CLASS CLeastSquares : public CRavidObject
		{
		public:
			RavidPreventCopySelf(CLeastSquares);

			CLeastSquares();
			virtual ~CLeastSquares();
			
			/**
			* 설정된 좌표를 초기화합니다.
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool Clear();
			
			/**
			* 입력된 객체의 중심으로 내부 좌표리스트를 설정합니다.
			*
			* param[in] pData : 입력 객체 리스트 (Pointer)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool SetData(_In_ CRavidGeometryArray* pData);
			
			/**
			* 입력된 객체로 내부 좌표리스트를 설정합니다.
			*
			* param[in] pData : 입력 객체 리스트 (Pointer)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool SetData(_In_ std::vector<CRavidPoint<int> >* pData);
			
			/**
			* 입력된 객체로 내부 좌표리스트를 설정합니다.
			*
			* param[in] pData : 입력 객체 리스트 (Pointer)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool SetData(_In_ std::vector<CRavidPoint<long long> >* pData);
			
			/**
			* 입력된 객체로 내부 좌표리스트를 설정합니다.
			*
			* param[in] pData : 입력 객체 리스트 (Pointer)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool SetData(_In_ std::vector<CRavidPoint<float> >* pData);
			
			/**
			* 입력된 객체로 내부 좌표리스트를 설정합니다.
			*
			* param[in] pData : 입력 객체 리스트 (Pointer)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool SetData(_In_ std::vector<CRavidPoint<double> >* pData);
						
			/**
			* 입력된 객체로 내부 좌표리스트를 설정합니다.
			* 입력된 x, y 의 수는 같고, count 보다 같거나 작아야 합니다.
			*
			* param[in] pDblX : x 좌표 리스트 (Pointer)
			* param[in] pDblY : y 좌표 리스트 (Pointer)
			* param[in] nCount : 데이터 수
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool SetData(_In_ double* pDblX, _In_ double* pDblY, _In_ long nCount);
						
			/**
			* 입력된 객체로 내부 좌표리스트를 설정합니다.
			* 입력된 pData 의 수는 count 보다 같거나 작아야 합니다.
			*
			* param[in] pData : x, y 좌표 리스트 (Pointer)
			* param[in] nCount : 데이터 수
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool SetData(_In_ CRavidPoint<int>* pData, _In_ long nCount);
						
			/**
			* 입력된 객체로 내부 좌표리스트를 설정합니다.
			* 입력된 pData 의 수는 count 보다 같거나 작아야 합니다.
			*
			* param[in] pData : x, y 좌표 리스트 (Pointer)
			* param[in] nCount : 데이터 수
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool SetData(_In_ CRavidPoint<long long>* pData, _In_ long nCount);
						
			/**
			* 입력된 객체로 내부 좌표리스트를 설정합니다.
			* 입력된 pData 의 수는 count 보다 같거나 작아야 합니다.
			*
			* param[in] pData : x, y 좌표 리스트 (Pointer)
			* param[in] nCount : 데이터 수
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool SetData(_In_ CRavidPoint<float>* pData, _In_ long nCount);
						
			/**
			* 입력된 객체로 내부 좌표리스트를 설정합니다.
			* 입력된 pData 의 수는 count 보다 같거나 작아야 합니다.
			*
			* param[in] pData : x, y 좌표 리스트 (Pointer)
			* param[in] nCount : 데이터 수
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool SetData(_In_ CRavidPoint<double>* pData, _In_ long nCount);
									
			/**
			* 입력된 객체를 내부 좌표리스트에 추가합니다.
			*
			* param[in] dblX : x 좌표
			* param[in] dblY : y 좌표
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool AddData(_In_ double dblX, _In_ double dblY);
									
			/**
			* 입력된 객체를 내부 좌표리스트에 추가합니다.
			*
			* param[in] pData : x, y 좌표 (Pointer)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool AddData(_In_ CRavidPoint<int>* pData);
									
			/**
			* 입력된 객체를 내부 좌표리스트에 추가합니다.
			*
			* param[in] pData : x, y 좌표 (Pointer)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool AddData(_In_ CRavidPoint<long long>* pData);
									
			/**
			* 입력된 객체를 내부 좌표리스트에 추가합니다.
			*
			* param[in] pData : x, y 좌표 (Pointer)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool AddData(_In_ CRavidPoint<float>* pData);
									
			/**
			* 입력된 객체를 내부 좌표리스트에 추가합니다.
			*
			* param[in] pData : x, y 좌표 (Pointer)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool AddData(_In_ CRavidPoint<double>* pData);
			
			/**
			* 내부 좌표리스트로 1차 방정식을 계산합니다.
			*
			* param[out] refA : 1차 계수 (Reference)
			* param[out] refB : 상수항 (Reference)
			* param[out] refRSquare : 결정계수 (Reference)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool GetLinear(_Out_ double& refA, _Out_ double& refB, _Out_ double& refRSquare);
			
			/**
			* 내부 좌표리스트로 1차 방정식을 계산합니다.
			*
			* param[out] pA : 1차 계수 (Pointer)
			* param[out] pB : 상수항 (Pointer)
			* param[out] pRSquare : 결정계수 (Pointer)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool GetLinear(_Out_ double* pA, _Out_ double* pB, _Out_ double* pRSquare);
			
			/**
			* 내부 좌표리스트로 2차 방정식을 계산합니다.
			*
			* param[out] refA : x 2차 계수 (Reference)
			* param[out] refB : x 1차 계수 (Reference)
			* param[out] refC : 상수항 (Reference)
			* param[out] refRSquare : 결정계수 (Reference)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool GetQuadratic(_Out_ double& refA, _Out_ double& refB, _Out_ double& refC, _Out_ double& refRSquare);
			
			/**
			* 내부 좌표리스트로 2차 방정식을 계산합니다.
			*
			* param[out] pA : x 2차 계수 (Pointer)
			* param[out] pB : x 1차 계수 (Pointer)
			* param[out] pC : 상수항 (Pointer)
			* param[out] pRSquare : 결정계수 (Pointer)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool GetQuadratic(_Out_ double* pA, _Out_ double* pB, _Out_ double* pC, _Out_ double* pRSquare);
			
			/**
			* 내부 좌표리스트로 3차 방정식을 계산합니다.
			*
			* param[out] refA : x 3차 계수 (Reference)
			* param[out] refB : x 2차 계수 (Reference)
			* param[out] refC : x 1차 계수 (Reference)
			* param[out] refD : 상수항 (Reference)
			* param[out] refRSquare : 결정계수 (Reference)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool GetCubic(_Out_ double& refA, _Out_ double& refB, _Out_ double& refC, _Out_ double& refD, _Out_ double& refRSquare);
			
			/**
			* 내부 좌표리스트로 3차 방정식을 계산합니다.
			*
			* param[out] pA : x 3차 계수 (Pointer)
			* param[out] pB : x 2차 계수 (Pointer)
			* param[out] pC : x 1차 계수 (Pointer)
			* param[out] pD : 상수항 (Pointer)
			* param[out] pRSquare : 결정계수 (Pointer)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool GetCubic(_Out_ double* pA, _Out_ double* pB, _Out_ double* pC, _Out_ double* pD, _Out_ double* pRSquare);
			
			/**
			* 내부 좌표리스트로 4차 방정식을 계산합니다.
			*
			* param[out] refA : x 4차 계수 (Reference)
			* param[out] refB : x 3차 계수 (Reference)
			* param[out] refC : x 2차 계수 (Reference)
			* param[out] refD : x 1차 계수 (Reference)
			* param[out] refE : 상수항 (Reference)
			* param[out] refRSquare : 결정계수 (Reference)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool GetQuartic(_Out_ double& refA, _Out_ double& refB, _Out_ double& refC, _Out_ double& refD, _Out_ double& refE, _Out_ double& refRSquare);
			
			/**
			* 내부 좌표리스트로 4차 방정식을 계산합니다.
			*
			* param[out] pA : x 4차 계수 (Pointer)
			* param[out] pB : x 3차 계수 (Pointer)
			* param[out] pC : x 2차 계수 (Pointer)
			* param[out] pD : x 1차 계수 (Pointer)
			* param[out] pE : 상수항 (Pointer)
			* param[out] pRSquare : 결정계수 (Pointer)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool GetQuartic(_Out_ double* pA, _Out_ double* pB, _Out_ double* pC, _Out_ double* pD, _Out_ double* pE, _Out_ double* pRSquare);
			
			/**
			* 내부 좌표리스트로 n차 방정식을 계산합니다.
			*
			* param[out] nDegree : n 차 방정식 설정 (Reference)
			* param[out] refVctResult : 계수, 상수 리스트 (Reference)
			* param[out] refRSquare : 결정계수 (Reference)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool GetNDegree(_In_ int nDegree, _Out_ std::vector<double>& refVctResult, _Out_ double& refRSquare);
			
			/**
			* 내부 좌표리스트로 n차 방정식을 계산합니다.
			*
			* param[out] nDegree : n 차 방정식 설정 (Pointer)
			* param[out] pVctResult : 계수, 상수 리스트 (Pointer)
			* param[out] pRSquare : 결정계수 (Pointer)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool GetNDegree(_In_ int nDegree, _Out_ std::vector<double>* pVctResult, _Out_ double* pRSquare);

		private:
			std::unique_ptr<CInternalLeastSquares> m_pInternal;
		};
	}
}

