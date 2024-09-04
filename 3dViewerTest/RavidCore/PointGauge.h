#pragma once

#include <vector>

namespace Ravid
{
	namespace Algorithms
	{
		class CInternalPointGauge;
		class CRavidImage;
		/**
		* 이미지의 정보를 연결된 객체로 측정하여 추정된 결과를 제공하는 클래스입니다.
		*/
		class AFX_EXT_CLASS CPointGauge : public CRavidObject
		{
			RavidUseDynamicCreation();
			RavidPreventCopySelf(CPointGauge);

		public:
			CPointGauge();
			virtual ~CPointGauge();
			
			/**
			* 특징을 찾기 위한 화소 값의 변화 방법입니다.
			*/
			enum ETransitionType
			{
				ETransitionType_BW = 0,
				ETransitionType_WB,
				ETransitionType_BWorWB,
				ETransitionType_BWB,
				ETransitionType_WBW,
			};
			/**
			* 특징이 나타난 위치를 보정하는 방법입니다.
			*/
			enum ETransitionChoice
			{
				ETransitionChoice_Begin = 0,
				ETransitionChoice_End,
				ETransitionChoice_LargestAmplitude,
				ETransitionChoice_LargestArea,
				ETransitionChoice_Closest,
				ETransitionChoice_All,
			};

		/**
			* 알고리즘을 실행합니다.
			*
			* param[in] pImgInfo : 입력 이미지 (Pointer)
			* param[in_opt] pGeometry : CRavidRect 검사 ROI (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Measure(_In_ CRavidImage* pImgInfo, _In_opt_ CRavidRect<int>* pGeometry);

			/**
			* 알고리즘을 실행합니다.
			*
			* param[in] refImgInfo : 입력 이미지 (Reference)
			* param[in_opt] pGeometry : CRavidRect 검사 ROI (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Measure(_In_ CRavidImage& refImgInfo, _In_opt_ CRavidRect<int>* pGeometry);

			/**
			* 알고리즘을 실행합니다.
			*
			* param[in] pImgInfo : 입력 이미지 (Pointer)
			* param[in_opt] pGeometry : CRavidRect 검사 ROI (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Measure(_In_ CRavidImage* pImgInfo, _In_opt_ CRavidRect<long long>* pGeometry);

			/**
			* 알고리즘을 실행합니다.
			*
			* param[in] refImgInfo : 입력 이미지 (Reference)
			* param[in_opt] pGeometry : CRavidRect 검사 ROI (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Measure(_In_ CRavidImage& refImgInfo, _In_opt_ CRavidRect<long long>* pGeometry);

			/**
			* 알고리즘을 실행합니다.
			*
			* param[in] pImgInfo : 입력 이미지 (Pointer)
			* param[in_opt] pGeometry : CRavidRect 검사 ROI (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Measure(_In_ CRavidImage* pImgInfo, _In_opt_ CRavidRect<float>* pGeometry);

			/**
			* 알고리즘을 실행합니다.
			*
			* param[in] refImgInfo : 입력 이미지 (Reference)
			* param[in_opt] pGeometry : CRavidRect 검사 ROI (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Measure(_In_ CRavidImage& refImgInfo, _In_opt_ CRavidRect<float>* pGeometry);

			/**
			* 알고리즘을 실행합니다.
			*
			* param[in] pImgInfo : 입력 이미지 (Pointer)
			* param[in_opt] pGeometry : CRavidRect 검사 ROI (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Measure(_In_ CRavidImage* pImgInfo, _In_opt_ CRavidRect<double>* pGeometry = nullptr);

			/**
			* 알고리즘을 실행합니다.
			*
			* param[in] refImgInfo : 입력 이미지 (Reference)
			* param[in_opt] pGeometry : CRavidRect 검사 ROI (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Measure(_In_ CRavidImage& refImgInfo, _In_opt_ CRavidRect<double>* pGeometry = nullptr);
			
			/**
			* 실행 전 사용합니다.
			* 측정하기 위한 위치, 형태를 설정합니다.
			*
			* param[in] refGeometry : 중심 위치 (Reference)
			* param[in] dblTolerance : 탐색 너비
			* param[in] dblAngle : 탐색 회전 각도
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetMeasurementRegion(_In_ CRavidPoint<double>& refGeometry, _In_ double dblTolerance, _In_ double dblAngle);
			
			/**
			* 실행 전 사용합니다.
			* 측정하기 위한 위치, 형태를 설정합니다.
			*
			* param[in] pGeometry : 중심 위치 (Pointer)
			* param[in] dblTolerance : 탐색 너비
			* param[in] dblAngle : 탐색 회전 각도
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetMeasurementRegion(_In_ CRavidPoint<double>* pCenter, _In_ double dblTolerance, _In_ double dblAngle);
			
			/**
			* 실행 전 사용합니다.
			* 측정하기 위한 위치, 형태를 설정합니다.
			*
			* param[in] refStart : 탐색 시작 위치 (Reference)
			* param[in] refEnd : 탐색 끝 위치 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetMeasurementRegion(_In_ CRavidPoint<double>&  refStart, _In_ CRavidPoint<double>& refEnd);
			
			/**
			* 실행 전 사용합니다.
			* 측정하기 위한 위치, 형태를 설정합니다.
			*
			* param[in] pStart : 탐색 시작 위치 (Pointer)
			* param[in] pEnd : 탐색 끝 위치 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetMeasurementRegion(_In_ CRavidPoint<double>* pRpStart, _In_ CRavidPoint<double>* pRpEnd);
			
			/**
			* 실행 전 사용합니다.
			* 측정하기 위한 위치, 형태를 설정합니다.
			*
			* param[in] refMeasurementLine : 탐색 시작, 끝 위치 (Reference)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetMeasurementRegion(_In_ CRavidLine<double>& refMeasurementLine);
			
			/**
			* 실행 전 사용합니다.
			* 측정하기 위한 위치, 형태를 설정합니다.
			*
			* param[in] pMeasurementLine : 탐색 시작, 끝 위치 (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetMeasurementRegion(_In_ CRavidLine<double>* pRlMeasurementLine);
			
			/**
			* 설정된 탐색 중심 위치를 가져옵니다.
			*
			* return value : CRavidPoint 클래스로 반환합니다.
			*/
			CRavidPoint<double> GetCenter();
			
			/**
			* 설정된 탐색 영역 범위를 가져옵니다.
			*
			* return value : double 값으로 반환합니다.
			*/
			double GetTolerance();
			
			/**
			* 설정된 탐색 영역 각도를 가져옵니다.
			*
			* return value : double 값으로 반환합니다.
			*/
			double GetAngle();
			
			/**
			* 설정된 탐색 영역 각도를 Radian 으로 가져옵니다.
			*
			* return value : double 값으로 반환합니다.
			*/
			double GetRadian();
					
			/**
			* 설정된 화소값 탐색 방법을 가져옵니다.
			*
			* return value : enum ETransitionType 타입을 반환합니다.
			*/
			ETransitionType GetTransitionType();
						
			/**
			* 실행 전 사용합니다.
			* 화소값 변화 탐색 방법을 설정함니다.
			*
			* param[in] eTransitionType : 화소값 변화 탐색 방법
			*/
			void SetTransitionType(_In_ ETransitionType eTransitionType);
			
			/**
			* 설정된 탐색 보정 방법을 가져옵니다.
			*
			* return value : enum ETransitionChoice 타입을 반환합니다.
			*/
			ETransitionChoice GetTransitionChoice();

			/**
			* 실행 전 사용합니다.
			* 탐색 보정 방법을 설정함니다.
			*
			* param[in] eTransitionChoice : 보정 방법
			*/
			void SetTransitionChoice(_In_ ETransitionChoice eTransitionChoice);
			
			/**
			* 설정된 변화 최소량을 가져옵니다.
			*
			* return value : long 값으로 반환합니다.
			*/
			long GetThreshold();

			/**
			* 실행 전 사용합니다.
			* 변화 최소량을 설정합니다.
			*
			* param[in] nThreshold : 밝기 변화량
			*/
			void SetThreshold(_In_ long nThreshold);
			
			/**
			* 설정된 변화 증폭량을 가져옵니다.
			*
			* return value : long 값으로 반환합니다.
			*/
			long GetMinimumAmplitude();

			/**
			* 실행 전 사용합니다.
			* 변화 증폭량을 설정합니다.
			*
			* param[in] nMinimumAmplitude : 변화 증폭량
			*/
			void SetMinimumAmplitude(_In_ long nMinimumAmplitude);
			
			/**
			* 설정된 탐색 유효 주변 참조 데이터 수를 가져옵니다.
			*
			* return value : int 값으로 반환합니다.
			*/
			long GetThickness();

			/**
			* 실행 전 사용합니다.
			* 탐색 유효 주변 참조 데이터 수를 설정합니다.
			*
			* param[in] nThickness : 주변 참조 데이터 수
			*/
			void SetThickness(_In_ long nThickness);
			
			/**
			* 실행 후 사용합니다.
			* 입력된 인덱스의 모든 점의 리스트를 가져옵니다.
			*
			* param[in_opt] nIndex : 결과 인덱스 (Default : 0)
			*
			* return value : std::vector<CRavidPoint<double> > 형태로 반환합니다.
			*/
			std::vector<CRavidPoint<double> > GetMeasuredPoints(_In_opt_ int nIndex = 0);
			
			/**
			* 실행 후 사용합니다.
			* 입력된 인덱스의 모든 점의 리스트를 가져옵니다.
			*
			* param[out] refVctResult : 결과 객체 리스트 (Reference)
			* param[in_opt] nIndex : 결과 인덱스 (Default : 0)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetMeasuredPoints(_Out_ std::vector<CRavidPoint<double> >& refVctResult, _In_opt_ int nIndex = 0);
			
			/**
			* 실행 후 사용합니다.
			* 입력된 인덱스의 모든 점의 리스트를 가져옵니다.
			*
			* param[out] pVctResult : 결과 객체 리스트 (Pointer)
			* param[in_opt] nIndex : 결과 인덱스 (Default : 0)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult GetMeasuredPoints(_Out_ std::vector<CRavidPoint<double> >* pVctResult, _In_opt_ int nIndex = 0);
			
			/**
			* 2023.05.16
			* PointGauge Division Algorithm 2 add
			*
			* param[in] nAlgorithmSelect : 엣지 추정 알고리즘 인덱스
			* 0 : 실수 핏팅 알고리즘 
			* 1 : 기존 핏팅 알고리즘
			*
			*/
			void SetMeasureAlgorithm(int nAlgorithmSelect);

		private:

			void ClearInternalData();
			CInternalPointGauge* m_pInternal = nullptr;


		};
	}
}

