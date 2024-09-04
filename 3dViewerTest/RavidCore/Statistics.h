#pragma once

#include <vector>

namespace Ravid
{
	using namespace Algorithms;

	namespace Mathematics
	{
		/**
		* 입력된 데이터로 분석하는 클래스입니다.
		*/
		class AFX_EXT_CLASS CStatistics : public CRavidObject
		{
			//RavidUseDynamicCreation(this);
			RavidPreventCopySelf(CStatistics);

		public:
			CStatistics() = delete;
						
			/**
			* 입력된 객체로 최소값를 계산합니다.
			*
			* param[in] pData : 입력 데이터 리스트 (Pointer)
			* param[in] stCount : 입력 데이터 수
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetMin(_In_ float* pData, _In_ size_t stCount, _Out_ float &OutputData);
			
			/**
			* 입력된 객체로 최소값를 계산합니다.
			*
			* param[in] pData : 입력 데이터 리스트 (Pointer)
			* param[in] stCount : 입력 데이터 수
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetMin(_In_ float* pData, _In_ size_t stCount, _Out_ double &OutputData);
			
			/**
			* 입력된 객체로 최소값를 계산합니다.
			*
			* param[in] pData : 입력 데이터 리스트 (Pointer)
			* param[in] stCount : 입력 데이터 수
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetMin(_In_ double* pData, _In_ size_t stCount, _Out_ float &OutputData);
			
			/**
			* 입력된 객체로 최소값를 계산합니다.
			*
			* param[in] pData : 입력 데이터 리스트 (Pointer)
			* param[in] stCount : 입력 데이터 수
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetMin(_In_ double* pData, _In_ size_t stCount, _Out_ double &OutputData);
			
			/**
			* 입력된 객체로 최소값를 계산합니다.
			*
			* param[in] pData : 입력 데이터 리스트 (Pointer)
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetMin(_In_ std::vector<float>* pDataList, _Out_ float &OutputData);
			
			/**
			* 입력된 객체로 최소값를 계산합니다.
			*
			* param[in] pData : 입력 데이터 리스트 (Pointer)
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetMin(_In_ std::vector<float>* pDataList, _Out_ double &OutputData);
			
			/**
			* 입력된 객체로 최소값를 계산합니다.
			*
			* param[in] pData : 입력 데이터 리스트 (Pointer)
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetMin(_In_ std::vector<double>* pDataList, _Out_ float &OutputData);
			
			/**
			* 입력된 객체로 최소값를 계산합니다.
			*
			* param[in] pData : 입력 데이터 리스트 (Pointer)
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetMin(_In_ std::vector<double>* pDataList, _Out_ double &OutputData);
			
			/**
			* 입력된 객체로 최소값를 계산합니다.
			*
			* param[in] vctData : 입력 데이터 리스트 (Reference)
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetMin(_In_ std::vector<float>& refDataList, _Out_ float &OutputData);
			
			/**
			* 입력된 객체로 최소값를 계산합니다.
			*
			* param[in] vctData : 입력 데이터 리스트 (Reference)
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetMin(_In_ std::vector<float>& refDataList, _Out_ double &OutputData);
			
			/**
			* 입력된 객체로 최소값를 계산합니다.
			*
			* param[in] vctData : 입력 데이터 리스트 (Reference)
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetMin(_In_ std::vector<double>& refDataList, _Out_ float &OutputData);
			
			/**
			* 입력된 객체로 최소값를 계산합니다.
			*
			* param[in] vctData : 입력 데이터 리스트 (Reference)
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetMin(_In_ std::vector<double>& refDataList, _Out_ double &OutputData);
						
			/**
			* 입력된 객체로 최대값를 계산합니다.
			*
			* param[in] pData : 입력 데이터 리스트 (Pointer)
			* param[in] stCount : 입력 데이터 수
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetMax(_In_ float* pData, _In_ size_t stCount, _Out_ float &OutputData);
			
			/**
			* 입력된 객체로 최대값를 계산합니다.
			*
			* param[in] pData : 입력 데이터 리스트 (Pointer)
			* param[in] stCount : 입력 데이터 수
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetMax(_In_ float* pData, _In_ size_t stCount, _Out_ double &OutputData);
			
			/**
			* 입력된 객체로 최대값를 계산합니다.
			*
			* param[in] pData : 입력 데이터 리스트 (Pointer)
			* param[in] stCount : 입력 데이터 수
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetMax(_In_ double* pData, _In_ size_t stCount, _Out_ float &OutputData);
			
			/**
			* 입력된 객체로 최대값를 계산합니다.
			*
			* param[in] pData : 입력 데이터 리스트 (Pointer)
			* param[in] stCount : 입력 데이터 수
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetMax(_In_ double* pData, _In_ size_t stCount, _Out_ double &OutputData);
			
			/**
			* 입력된 객체로 최대값를 계산합니다.
			*
			* param[in] pData : 입력 데이터 리스트 (Pointer)
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetMax(_In_ std::vector<float>* pDataList, _Out_ float &OutputData);
			
			/**
			* 입력된 객체로 최대값를 계산합니다.
			*
			* param[in] pData : 입력 데이터 리스트 (Pointer)
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetMax(_In_ std::vector<float>* pDataList, _Out_ double &OutputData);
			
			/**
			* 입력된 객체로 최대값를 계산합니다.
			*
			* param[in] pData : 입력 데이터 리스트 (Pointer)
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetMax(_In_ std::vector<double>* pDataList, _Out_ float &OutputData);
			
			/**
			* 입력된 객체로 최대값를 계산합니다.
			*
			* param[in] pData : 입력 데이터 리스트 (Pointer)
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetMax(_In_ std::vector<double>* pDataList, _Out_ double &OutputData);
			
			/**
			* 입력된 객체로 최대값를 계산합니다.
			*
			* param[in] vctData : 입력 데이터 리스트 (Reference)
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetMax(_In_ std::vector<float>& refDataList, _Out_ float &OutputData);
			
			/**
			* 입력된 객체로 최대값를 계산합니다.
			*
			* param[in] vctData : 입력 데이터 리스트 (Reference)
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetMax(_In_ std::vector<float>& refDataList, _Out_ double &OutputData);
			
			/**
			* 입력된 객체로 최대값를 계산합니다.
			*
			* param[in] vctData : 입력 데이터 리스트 (Reference)
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetMax(_In_ std::vector<double>& refDataList, _Out_ float &OutputData);
			
			/**
			* 입력된 객체로 최대값를 계산합니다.
			*
			* param[in] vctData : 입력 데이터 리스트 (Reference)
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetMax(_In_ std::vector<double>& refDataList, _Out_ double &OutputData);
						
			/**
			* 입력된 객체로 중앙값를 계산합니다.
			*
			* param[in] pData : 입력 데이터 리스트 (Pointer)
			* param[in] stCount : 입력 데이터 수
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetMedian(_In_ float* pData, _In_ size_t stCount, _Out_ float &OutputData);
			
			/**
			* 입력된 객체로 중앙값를 계산합니다.
			*
			* param[in] pData : 입력 데이터 리스트 (Pointer)
			* param[in] stCount : 입력 데이터 수
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetMedian(_In_ float* pData, _In_ size_t stCount, _Out_ double &OutputData);
			
			/**
			* 입력된 객체로 중앙값를 계산합니다.
			*
			* param[in] pData : 입력 데이터 리스트 (Pointer)
			* param[in] stCount : 입력 데이터 수
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetMedian(_In_ double* pData, _In_ size_t stCount, _Out_ float &OutputData);
			
			/**
			* 입력된 객체로 중앙값를 계산합니다.
			*
			* param[in] pData : 입력 데이터 리스트 (Pointer)
			* param[in] stCount : 입력 데이터 수
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetMedian(_In_ double* pData, _In_ size_t stCount, _Out_ double &OutputData);
			
			/**
			* 입력된 객체로 중앙값를 계산합니다.
			*
			* param[in] pData : 입력 데이터 리스트 (Pointer)
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetMedian(_In_ std::vector<float>* pDataList, _Out_ float &OutputData);
			
			/**
			* 입력된 객체로 중앙값를 계산합니다.
			*
			* param[in] pData : 입력 데이터 리스트 (Pointer)
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetMedian(_In_ std::vector<float>* pDataList, _Out_ double &OutputData);
			
			/**
			* 입력된 객체로 중앙값를 계산합니다.
			*
			* param[in] pData : 입력 데이터 리스트 (Pointer)
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetMedian(_In_ std::vector<double>* pDataList, _Out_ float &OutputData);
			
			/**
			* 입력된 객체로 중앙값를 계산합니다.
			*
			* param[in] pData : 입력 데이터 리스트 (Pointer)
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetMedian(_In_ std::vector<double>* pDataList, _Out_ double &OutputData);
			
			/**
			* 입력된 객체로 중앙값를 계산합니다.
			*
			* param[in] vctData : 입력 데이터 리스트 (Reference)
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetMedian(_In_ std::vector<float>& refDataList, _Out_ float &OutputData);
			
			/**
			* 입력된 객체로 중앙값를 계산합니다.
			*
			* param[in] vctData : 입력 데이터 리스트 (Reference)
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetMedian(_In_ std::vector<float>& refDataList, _Out_ double &OutputData);
			
			/**
			* 입력된 객체로 중앙값를 계산합니다.
			*
			* param[in] vctData : 입력 데이터 리스트 (Reference)
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetMedian(_In_ std::vector<double>& refDataList, _Out_ float &OutputData);
			
			/**
			* 입력된 객체로 중앙값를 계산합니다.
			*
			* param[in] vctData : 입력 데이터 리스트 (Reference)
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetMedian(_In_ std::vector<double>& refDataList, _Out_ double &OutputData);
						
			/**
			* 입력된 객체로 분산를 계산합니다.
			*
			* param[in] pData : 입력 데이터 리스트 (Pointer)
			* param[in] stCount : 입력 데이터 수
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetVariance(_In_ float* pData, _In_ size_t stCount, _Out_ float &OutputData);
			
			/**
			* 입력된 객체로 분산를 계산합니다.
			*
			* param[in] pData : 입력 데이터 리스트 (Pointer)
			* param[in] stCount : 입력 데이터 수
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetVariance(_In_ float* pData, _In_ size_t stCount, _Out_ double &OutputData);
			
			/**
			* 입력된 객체로 분산를 계산합니다.
			*
			* param[in] pData : 입력 데이터 리스트 (Pointer)
			* param[in] stCount : 입력 데이터 수
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetVariance(_In_ double* pData, _In_ size_t stCount, _Out_ float &OutputData);
			
			/**
			* 입력된 객체로 분산를 계산합니다.
			*
			* param[in] pData : 입력 데이터 리스트 (Pointer)
			* param[in] stCount : 입력 데이터 수
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetVariance(_In_ double* pData, _In_ size_t stCount, _Out_ double &OutputData);
			
			/**
			* 입력된 객체로 분산를 계산합니다.
			*
			* param[in] pData : 입력 데이터 리스트 (Pointer)
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetVariance(_In_ std::vector<float>* pDataList, _Out_ float &OutputData);
			
			/**
			* 입력된 객체로 분산를 계산합니다.
			*
			* param[in] pData : 입력 데이터 리스트 (Pointer)
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetVariance(_In_ std::vector<float>* pDataList, _Out_ double &OutputData);
			
			/**
			* 입력된 객체로 분산를 계산합니다.
			*
			* param[in] pData : 입력 데이터 리스트 (Pointer)
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetVariance(_In_ std::vector<double>* pDataList, _Out_ float &OutputData);
			
			/**
			* 입력된 객체로 분산를 계산합니다.
			*
			* param[in] pData : 입력 데이터 리스트 (Pointer)
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetVariance(_In_ std::vector<double>* pDataList, _Out_ double &OutputData);
			
			/**
			* 입력된 객체로 분산를 계산합니다.
			*
			* param[in] vctData : 입력 데이터 리스트 (Reference)
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetVariance(_In_ std::vector<float>& refDataList, _Out_ float &OutputData);
			
			/**
			* 입력된 객체로 분산를 계산합니다.
			*
			* param[in] vctData : 입력 데이터 리스트 (Reference)
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetVariance(_In_ std::vector<float>& refDataList, _Out_ double &OutputData);
			
			/**
			* 입력된 객체로 분산를 계산합니다.
			*
			* param[in] vctData : 입력 데이터 리스트 (Reference)
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetVariance(_In_ std::vector<double>& refDataList, _Out_ float &OutputData);
			
			/**
			* 입력된 객체로 분산를 계산합니다.
			*
			* param[in] vctData : 입력 데이터 리스트 (Reference)
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetVariance(_In_ std::vector<double>& refDataList, _Out_ double &OutputData);
						
			/**
			* 입력된 객체로 표준편차를 계산합니다.
			*
			* param[in] pData : 입력 데이터 리스트 (Pointer)
			* param[in] stCount : 입력 데이터 수
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetStandardDeviation(_In_ float* pData, _In_ size_t stCount, _Out_ float &OutputData);
			
			/**
			* 입력된 객체로 표준편차를 계산합니다.
			*
			* param[in] pData : 입력 데이터 리스트 (Pointer)
			* param[in] stCount : 입력 데이터 수
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetStandardDeviation(_In_ float* pData, _In_ size_t stCount, _Out_ double &OutputData);
			
			/**
			* 입력된 객체로 표준편차를 계산합니다.
			*
			* param[in] pData : 입력 데이터 리스트 (Pointer)
			* param[in] stCount : 입력 데이터 수
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetStandardDeviation(_In_ double* pData, _In_ size_t stCount, _Out_ float &OutputData);
			
			/**
			* 입력된 객체로 표준편차를 계산합니다.
			*
			* param[in] pData : 입력 데이터 리스트 (Pointer)
			* param[in] stCount : 입력 데이터 수
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetStandardDeviation(_In_ double* pData, _In_ size_t stCount, _Out_ double &OutputData);
			
			/**
			* 입력된 객체로 표준편차를 계산합니다.
			*
			* param[in] pData : 입력 데이터 리스트 (Pointer)
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetStandardDeviation(_In_ std::vector<float>* pDataList, _Out_ float &OutputData);
			
			/**
			* 입력된 객체로 표준편차를 계산합니다.
			*
			* param[in] pData : 입력 데이터 리스트 (Pointer)
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetStandardDeviation(_In_ std::vector<float>* pDataList, _Out_ double &OutputData);
			
			/**
			* 입력된 객체로 표준편차를 계산합니다.
			*
			* param[in] pData : 입력 데이터 리스트 (Pointer)
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetStandardDeviation(_In_ std::vector<double>* pDataList, _Out_ float &OutputData);
			
			/**
			* 입력된 객체로 표준편차를 계산합니다.
			*
			* param[in] pData : 입력 데이터 리스트 (Pointer)
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetStandardDeviation(_In_ std::vector<double>* pDataList, _Out_ double &OutputData);
			
			/**
			* 입력된 객체로 표준편차를 계산합니다.
			*
			* param[in] vctData : 입력 데이터 리스트 (Reference)
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetStandardDeviation(_In_ std::vector<float>& refDataList, _Out_ float &OutputData);
			
			/**
			* 입력된 객체로 표준편차를 계산합니다.
			*
			* param[in] vctData : 입력 데이터 리스트 (Reference)
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetStandardDeviation(_In_ std::vector<float>& refDataList, _Out_ double &OutputData);
			
			/**
			* 입력된 객체로 표준편차를 계산합니다.
			*
			* param[in] vctData : 입력 데이터 리스트 (Reference)
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetStandardDeviation(_In_ std::vector<double>& refDataList, _Out_ float &OutputData);
			
			/**
			* 입력된 객체로 표준편차를 계산합니다.
			*
			* param[in] vctData : 입력 데이터 리스트 (Reference)
			* param[out] OutputData : 출력 데이터 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			static EAlgorithmResult GetStandardDeviation(_In_ std::vector<double>& refDataList, _Out_ double &OutputData);

		};
	}
}