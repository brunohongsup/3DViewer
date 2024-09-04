#pragma once

#include <memory>

namespace Ravid
{
	namespace Mathematics
	{
		class CInternalMatrix;
		
		/**
		* 메트릭스를 계산하는 클래스입니다.
		*/
		class AFX_EXT_CLASS CMatrix : public CRavidObject
		{
			RavidUseDynamicCreation(this);

		public:
			CMatrix();
			virtual ~CMatrix();

			CMatrix(_In_ const CMatrix& matrix);
			CMatrix(_In_ CMatrix* pMatrix);
			CMatrix(_In_ ptrdiff_t nColumn, _In_ ptrdiff_t nRow);
			
			/**
			* 합 연산
			*/
			void operator+=(_In_ const int& nParam);
			void operator+=(_In_ const long long& llParam);
			void operator+=(_In_ const float& fParam);
			void operator+=(_In_ const double& dblParam);
			void operator+=(_In_ const CMatrix& cMat);
			
			/**
			* 차 연산
			*/
			void operator-=(_In_ const int& nParam);
			void operator-=(_In_ const long long& llParam);
			void operator-=(_In_ const float& fParam);
			void operator-=(_In_ const double& dblParam);
			void operator-=(_In_ const CMatrix& cMat);
			
			/**
			* 곱셈 연산
			*/
			void operator*=(_In_ const int& nParam);
			void operator*=(_In_ const long long& llParam);
			void operator*=(_In_ const float& fParam);
			void operator*=(_In_ const double& dblParam);
			void operator*=(_In_ const CMatrix& cMat);
			
			/**
			* 나눗셈 연산
			*/
			void operator/=(_In_ const int& nParam);
			void operator/=(_In_ const long long& llParam);
			void operator/=(_In_ const float& fParam);
			void operator/=(_In_ const double& dblParam);
			
			/**
			* 합 연산
			*/
			CMatrix operator+(_In_ const int& nParam);
			CMatrix operator+(_In_ const long long& llParam);
			CMatrix operator+(_In_ const float& fParam);
			CMatrix operator+(_In_ const double& dblParam);
			CMatrix operator+(_In_ const CMatrix& cMat);
			
			/**
			* 차 연산
			*/
			CMatrix operator-(_In_ const int& nParam);
			CMatrix operator-(_In_ const long long& llParam);
			CMatrix operator-(_In_ const float& fParam);
			CMatrix operator-(_In_ const double& dblParam);
			CMatrix operator-(_In_ const CMatrix& cMat);
			
			/**
			* 곱셈 연산
			*/
			CMatrix operator*(_In_ const int& nParam);
			CMatrix operator*(_In_ const long long& llParam);
			CMatrix operator*(_In_ const float& fParam);
			CMatrix operator*(_In_ const double& dblParam);
			CMatrix operator*(_In_ const CMatrix& cMat);
			
			/**
			* 나눗셈 연산
			*/
			CMatrix operator/(_In_ const int& nParam);
			CMatrix operator/(_In_ const long long& llParam);
			CMatrix operator/(_In_ const float& fParam);
			CMatrix operator/(_In_ const double& dblParam);
			
			/**
			* 대입 연산
			*/
			void operator=(_In_ const CMatrix& matrix);
			void operator=(_In_ CMatrix* pMatrix);
			
			/**
			* 비교 연산
			*/
			bool operator==(_In_ const CMatrix& cMat);
			bool operator!=(_In_ const CMatrix& cMat);
			
			/**
			* 메트릭스 크기를 설정합니다.
			*
			* param[in] nColumn : 열 크기
			* param[in] nRow : 행 크기
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool InitMatrix(_In_ ptrdiff_t nColumn, _In_ ptrdiff_t nRow);
			
			/**
			* 메트릭스 데이터 유효성을 확인합니다.
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			virtual bool __fastcall IsValid() override;
			
			/**
			* 입력된 메트릭스를 복사합니다.
			*
			* param[in] matrix : 메트릭스 정보 (Reference)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool Copy(_In_ CMatrix& matrix);
			
			/**
			* 입력된 메트릭스를 복사합니다.
			*
			* param[in] pMatrix : 메트릭스 정보 (Pointer)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool Copy(_In_ CMatrix* pMatrix);
			
			/**
			* 내부 설정 및 데이터를 초기화합니다.
			*/
			void Clear();
			
			/**
			* 설정된 메트릭스 열을 가져옵니다.
			*
			* return value : ptrdiff_t 의 메트릭스 열을 반환합니다.
			*/
			ptrdiff_t GetColumn();
			
			/**
			* 설정된 메트릭스 행을 가져옵니다.
			*
			* return value : ptrdiff_t 의 메트릭스 행을 반환합니다.
			*/
			ptrdiff_t GetRow();
			
			/**
			* 설정된 메트릭스 행열의 전체 크기를 가져옵니다.
			*
			* return value : ptrdiff_t 의 메트릭스 행열의 크기를 반환합니다.
			*/
			ptrdiff_t GetElementCount();
			
			/**
			* 설정된 메트릭스 행열의 데이터 크기를 가져옵니다.
			*
			* return value : ptrdiff_t 의 메트릭스 행열의 데이터 크기를 반환합니다.
			*/
			ptrdiff_t GetTotalSize();
			
			/**
			* 설정된 메트릭스 행열의 데이터 캐시를 가져옵니다.
			*
			* return value : double** 의 메트릭스 행열의 데이터 캐시 포인터를 반환합니다.
			*/
			double** GetMatrixYCache();
			
			/**
			* 설정된 메트릭스 행열의 데이터를 가져옵니다.
			*
			* return value : double* 의 메트릭스 행열의 데이터 포인터를 반환합니다.
			*/
			double* GetMatrix();
			
			/**
			* 입력된 데이터 리스트를 설정합니다.
			*
			* param[in] pMatrix : 데이터 리스트 (Reference)
			* param[in] nColumn : 열 크기
			* param[in] nRow : 행 크기
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool SetMatrix(_In_ double* pMatrix, _In_ ptrdiff_t nColumn, _In_ ptrdiff_t nRow);
			
			/**
			* 해당 위치의 메트릭스 데이터를 가져옵니다.
			*
			* return value : double* 의 메트릭스 데이터 포인터를 반환합니다.
			*/
			double* GetValue(_In_ ptrdiff_t nColumn, _In_ ptrdiff_t nRow);
			
			/**
			* 해당 위치에 데이터를 설정합니다.
			*
			* param[in] nColumn : 열 크기
			* param[in] nRow : 행 크기
			* param[in] value : 데이터
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool SetValue(_In_ ptrdiff_t nColumn, _In_ ptrdiff_t nRow, _In_ double value);
			
			/**
			* 입력 메트릭스와 합 연산을 합니다.
			* 출력 메트릭스가 없으면 현재 메트릭스에 계산합니다.
			*
			* param[in] matrix : 입력 메트릭스 (Reference)
			* param[in_opt] pResultMatrix : 출력 메트릭스 (Pointer)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool Add(_In_ CMatrix& matrix, _Out_opt_ CMatrix* pResultMatrix = nullptr);
			
			/**
			* 입력 메트릭스와 합 연산을 합니다.
			* 출력 메트릭스가 없으면 현재 메트릭스에 계산합니다.
			*
			* param[in] pMatrix : 입력 메트릭스 (Pointer)
			* param[in_opt] pResultMatrix : 출력 메트릭스 (Pointer)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool Add(_In_ CMatrix* pMatrix, _Out_opt_ CMatrix* pResultMatrix = nullptr);
			
			/**
			* 입력 메트릭스와 차 연산을 합니다.
			* 출력 메트릭스가 없으면 현재 메트릭스에 계산합니다.
			*
			* param[in] matrix : 입력 메트릭스 (Reference)
			* param[in_opt] pResultMatrix : 출력 메트릭스 (Pointer)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool Subtract(_In_ CMatrix& matrix, _Out_opt_ CMatrix* pResultMatrix = nullptr);
			
			/**
			* 입력 메트릭스와 차 연산을 합니다.
			* 출력 메트릭스가 없으면 현재 메트릭스에 계산합니다.
			*
			* param[in] pMatrix : 입력 메트릭스 (Pointer)
			* param[in_opt] pResultMatrix : 출력 메트릭스 (Pointer)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool Subtract(_In_ CMatrix* pMatrix, _Out_opt_ CMatrix* pResultMatrix = nullptr);
			
			/**
			* 입력 메트릭스와 곱 연산을 합니다.
			* 출력 메트릭스가 없으면 현재 메트릭스에 계산합니다.
			*
			* param[in] matrix : 입력 메트릭스 (Reference)
			* param[in_opt] pResultMatrix : 출력 메트릭스 (Pointer)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool Multiply(_In_ CMatrix& matrix, _Out_opt_ CMatrix* pResultMatrix = nullptr);
			
			/**
			* 입력 메트릭스와 곱 연산을 합니다.
			* 출력 메트릭스가 없으면 현재 메트릭스에 계산합니다.
			*
			* param[in] pMatrix : 입력 메트릭스 (Pointer)
			* param[in_opt] pResultMatrix : 출력 메트릭스 (Pointer)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool Multiply(_In_ CMatrix* pMatrix, _Out_opt_ CMatrix* pResultMatrix = nullptr);
			
			/**
			* 행과 열을 교환합니다.
			* 출력 메트릭스가 없으면 현재 메트릭스에 계산합니다.
			*
			* param[in_opt] pResultMatrix : 출력 메트릭스 (Pointer)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool Transpose(_Out_opt_ CMatrix* pResultMatrix = nullptr);

			/**
			* 행열을 역행열로 변환합니다.
			* 출력 메트릭스가 없으면 현재 메트릭스에 계산합니다.
			*
			* param[in_opt] pResultMatrix : 출력 메트릭스 (Pointer)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool Invert(_Out_opt_ CMatrix* pResultMatrix = nullptr);

			/**
			* 행열을 단위행열로 변환합니다.
			* 출력 메트릭스가 없으면 현재 메트릭스에 계산합니다.
			*
			* param[in_opt] pResultMatrix : 출력 메트릭스 (Pointer)
			*
			* return value : bool 의 성공 여부를 반환합니다.
			*/
			bool Unit(_Out_opt_ CMatrix* pResultMatrix = nullptr);
			
			/**
			* 행열식을 계산합니다.
			*
			* return value : double 의 행열식 계산을 반환합니다.
			*/
			double GetDeterminant();

		protected:
			std::unique_ptr<CInternalMatrix> m_pInternal;
		};
	}
}

