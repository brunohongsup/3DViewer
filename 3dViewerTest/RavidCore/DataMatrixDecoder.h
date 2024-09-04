#pragma once

#include "RavidQuadrangle.h"

#include <vector>
#include <memory>

namespace Ravid
{
	namespace Algorithms
	{
		class CRavidImage;

		class CInternalDataMatrixDecoder;
		
		/**
		* 이미지에서 2차원 데이터 메트릭스 코드를 분석하는 클래스입니다.
		*/
		class AFX_EXT_CLASS CDataMatrixDecoder : public CRavidObject
		{
			RavidUseDynamicCreation();
			RavidPreventCopySelf(CDataMatrixDecoder);

		public:
			CDataMatrixDecoder();
			virtual ~CDataMatrixDecoder();

			/**
			* 단일, 다중 알고리즘 동작 방법입니다.
			*/
			enum EDataMatrixInspectionCount
			{
				EDataMatrixInspectionCount_Single = 0,
				EDataMatrixInspectionCount_All
			};
			/**
			* 배경대비 코드의 색 및 방법을 결정합니다.
			*/
			enum EDataMatrixColorLayer
			{
				EDataMatrixColorLayer_Black = 0,
				EDataMatrixColorLayer_White,
				EDataMatrixColorLayer_Auto_BlackToWhite,
				EDataMatrixColorLayer_Auto_WhiteToBlack
			};
			/**
			* 코드를 찾기위한 이진화 알고리즘입니다.
			*/
			enum EDataMatrixThresholdMethodOption
			{
				EDataMatrixThresholdMethodOption_Ostu = 0x1,
				EDataMatrixThresholdMethodOption_Adaptive1 = 0x2,
				EDataMatrixThresholdMethodOption_Adaptive2 = 0x4,
				EDataMatrixThresholdMethodOption_ISODATA = 0x8,
				EDataMatrixThresholdMethodOption_MaximumEntropy = 0x10
			};
			/**
			* 코드의 방향 반전을 결정합니다.
			*/
			enum EDataMatrixFlippingRegion
			{
				EDataMatrixFlippingRegion_No = 0x1,
				EDataMatrixFlippingRegion_Yes = 0x2,
				EDataMatrixFlippingRegion_Auto = 0x3
			};
			/**
			* 코드를 정밀하게 맞추기 위한 알고리즘입니다.
			*/
			enum EDataMatrixLineFittingMethod
			{
				EDataMatrixLineFittingMethod_Cluster = 0,
				EDataMatrixLineFittingMethod_HoughTransform,
				EDataMatrixLineFittingMethod_RANSAC
			};
			/**
			* 메트릭스 코드의 해석 결과 구조체입니다.
			*/
			struct SDataMatrixInformation
			{
				CRavidQuadrangle<double> rqRegion;

				std::vector<CRavidQuadrangle<double>> vctGrid;

				int nSizeRows;
				int nSizeCols;
				int nErrorCount;

				CString strColor;
				CString strVersion;
				CString strCode;

				bool bFlip;
				bool bSuccess;
			};
			/**
			* 메트릭스 코드의 크기(버전에 따른 크기) 사용 여부 구조체입니다.
			*/
			struct SDataMatrixExplicitSize
			{
				bool bUsing;
				int nRow;
				int nCol;
			};

			/**
			* 알고리즘을 실행합니다.
			*
			* param[in] pImgInfo : 입력 이미지 (Pointer)
			* param[in_opt] pGeometry : CRavidRect 검사 ROI (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Read(_In_ CRavidImage* pImgInfo, _In_opt_ CRavidRect<int>* pGeometry);
			
			/**
			* 알고리즘을 실행합니다.
			*
			* param[in] refImgInfo : 입력 이미지 (Reference)
			* param[in_opt] pGeometry : CRavidRect 검사 ROI (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Read(_In_ CRavidImage& refImgInfo, _In_opt_ CRavidRect<int>* pGeometry);
			
			/**
			* 알고리즘을 실행합니다.
			*
			* param[in] pImgInfo : 입력 이미지 (Pointer)
			* param[in_opt] pGeometry : CRavidRect 검사 ROI (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Read(_In_ CRavidImage* pImgInfo, _In_opt_ CRavidRect<long long>* pGeometry);
			
			/**
			* 알고리즘을 실행합니다.
			*
			* param[in] refImgInfo : 입력 이미지 (Reference)
			* param[in_opt] pGeometry : CRavidRect 검사 ROI (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Read(_In_ CRavidImage& refImgInfo, _In_opt_ CRavidRect<long long>* pGeometry);
			
			/**
			* 알고리즘을 실행합니다.
			*
			* param[in] pImgInfo : 입력 이미지 (Pointer)
			* param[in_opt] pGeometry : CRavidRect 검사 ROI (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Read(_In_ CRavidImage* pImgInfo, _In_opt_ CRavidRect<float>* pGeometry);
			
			/**
			* 알고리즘을 실행합니다.
			*
			* param[in] refImgInfo : 입력 이미지 (Reference)
			* param[in_opt] pGeometry : CRavidRect 검사 ROI (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Read(_In_ CRavidImage& refImgInfo, _In_opt_ CRavidRect<float>* pGeometry);
			
			/**
			* 알고리즘을 실행합니다.
			*
			* param[in] pImgInfo : 입력 이미지 (Pointer)
			* param[in_opt] pGeometry : CRavidRect 검사 ROI (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Read(_In_ CRavidImage* pImgInfo, _In_opt_ CRavidRect<double>* pGeometry = nullptr);
			
			/**
			* 알고리즘을 실행합니다.
			*
			* param[in] refImgInfo : 입력 이미지 (Reference)
			* param[in_opt] pGeometry : CRavidRect 검사 ROI (Pointer)
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult Read(_In_ CRavidImage& refImgInfo, _In_opt_ CRavidRect<double>* pGeometry = nullptr);
			
			/**
			* 분석을 실행 전 사용합니다.
			* 배경대비 코드의 색 및 방법을 결정합니다.
			*
			* param[in] ePriorityColorLayer : 결정 enum
			*	EDataMatrixColorLayer_Black : 코드 검정색 만 탐색
			*	EDataMatrixColorLayer_White : 코드 흰색 만 탐색
			*	EDataMatrixColorLayer_Auto_BlackToWhite : 코드 검정색 -> 흰색 순서로 탐색
			*	EDataMatrixColorLayer_Auto_WhiteToBlack : 코드 흰색 -> 검정색 순서로 탐색
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetColorLayer(_In_ EDataMatrixColorLayer ePriorityColorLayer);
			
			/**
			* 분석을 실행 전 사용합니다.
			* 찾을 객체의 단일, 다중 방법을 설정합니다.
			*
			* param[in] eDataMatrixInspectionCount : 동작 방법 enum
			*	EDataMatrixInspectionCount_Single : 단일 인식
			*	EDataMatrixInspectionCount_All : 다중  인식
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetInspectionCount(_In_ EDataMatrixInspectionCount eDataMatrixInspectionCount);
			
			/**
			* 설정된 배경대비 코드의 색 및 방법을 가져옵니다.
			*
			* return value : EDataMatrixColorLayer enum 반환합니다.
			*	EDataMatrixColorLayer_Black : 코드 검정색 만 탐색
			*	EDataMatrixColorLayer_White : 코드 흰색 만 탐색
			*	EDataMatrixColorLayer_Auto_BlackToWhite : 코드 검정색 -> 흰색 순서로 탐색
			*	EDataMatrixColorLayer_Auto_WhiteToBlack : 코드 흰색 -> 검정색 순서로 탐색
			*/
			EDataMatrixColorLayer GetColorLayer(void);
			
			/**
			* 설정된 찾을 객체의 단일, 다중 방법을 가져옵니다.
			*
			* return value : EDataMatrixInspectionCount enum 반환합니다.
			*	EDataMatrixInspectionCount_Single : 단일 인식
			*	EDataMatrixInspectionCount_All : 다중  인식
			*/
			EDataMatrixInspectionCount GetInspectionCount(void);
			
			/**
			* 분석을 실행 전 사용합니다.
			* 찾을 객체의 최소 길이를 설정합니다.
			*
			* param[in] nMinDist : 최소 길이 설정
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetMinRegionLength(_In_ int nMinDist);
			
			/**
			* 설정된 찾을 객체의 최소 길이를 가져옵니다.
			*
			* return value : int 형으로 반환됩니다.
			*/
			_Out_ int GetMinRegionLength(void);
			
			/**
			* 분석을 실행 전 사용합니다.
			* 찾을 객체의 최대 회전 각도를 설정합니다.
			*
			* param[in] dblMeasureAngleRatio : 최대 회전 각도
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetMeasureAngleRatio(_In_ double dblMeasureAngleRatio);
			
			/**
			* 설정된 찾을 객체의 최대 회전 각도를 가져옵니다.
			*
			* return value : double 형으로 반환됩니다.
			*/
			_Out_ double GetMeasureAngleRatio(void);
			
			/**
			* 분석을 실행 전 사용합니다.
			* 찾을 객체의 탐색 회전 간격을 설정합니다.
			*
			* param[in] dblMeasurAngleUnit : 탐색 각도 간격
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetMeasureAngleUnit(_In_ double dblMeasurAngleUnit);
			
			/**
			* 설정된 찾을 객체의 탐색 회전 간격을 가져옵니다.
			*
			* return value : double 형으로 반환됩니다.
			*/
			_Out_ double GetMeasureAngleUnit(void);
			
			/**
			* 분석을 실행 전 사용합니다.
			* 찾을 객체의 방향 반전을 설정합니다.
			*
			* param[in] eFlippingRegion : 동작 방법 enum
			*	EDataMatrixFlippingRegion_No : 정방향
			*	EDataMatrixFlippingRegion_Yes : 반전
			*	EDataMatrixFlippingRegion_Auto : 자동
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetFlippingRegion(_In_ EDataMatrixFlippingRegion eFlippingRegion);
			
			/**
			* 설정된 찾을 객체의 방향 반전을 가져옵니다.
			*
			* return value : EDataMatrixFlippingRegion enum 반환합니다.
			*	EDataMatrixFlippingRegion_No : 정방향
			*	EDataMatrixFlippingRegion_Yes : 반전
			*	EDataMatrixFlippingRegion_Auto : 자동
			*/
			_Out_ EDataMatrixFlippingRegion GetFlippingRegion(void);
			
			/**
			* 분석을 실행 전 사용합니다.
			* 코드의 정밀한 위치 설정 알고리즘을 설정합니다.
			*
			* param[in] eLFM : 동작 방법 enum
			*	EDataMatrixLineFittingMethod_Cluster : 클러스터
			*	EDataMatrixLineFittingMethod_HoughTransform : 허프 트렌스폼
			*	EDataMatrixLineFittingMethod_RANSAC : 랜삭
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			void SetFittingLine(_In_ EDataMatrixLineFittingMethod eLFM);
			
			/**
			* 설정된 코드의 정밀한 위치 설정 알고리즘을 가져옵니다.
			*
			* return value : EDataMatrixLineFittingMethod enum 반환합니다.
			*	EDataMatrixLineFittingMethod_Cluster : 클러스터
			*	EDataMatrixLineFittingMethod_HoughTransform : 허프 트렌스폼
			*	EDataMatrixLineFittingMethod_RANSAC : 랜삭
			*/
			_Out_ EDataMatrixLineFittingMethod GetFittingLine(void);
			
			/**
			* 분석을 실행 전 사용합니다.
			* 객체 해석시 보간 사용을 설정합니다.
			*
			* param[in] bCleanImage : 설정 On/ Off
			*/
			void SetCleanImage(_In_ bool bCleanImage);
			
			/**
			* 설정된 객체 해석시 보간 사용을 가져옵니다.
			*
			* return value : bool 형태로 반환합니다.
			*/
			_Out_ bool IsCleanImage(void);
			
			/**
			* 분석을 실행 전 사용합니다.
			* 메트릭스 코드의 크기(버전) 을 설정합니다.
			*
			* param[in] sDMES : SDataMatrixExplicitSize 구조체
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetExplicitSize(_In_ SDataMatrixExplicitSize sDMES);
			
			/**
			* 설정된 메트릭스 코드의 크기(버전) 을 가져옵니다.
			*
			* return value : SDataMatrixExplicitSize 구조체로 반환합니다.
			*/
			_Out_ SDataMatrixExplicitSize GetExplicitSize(void);
			
			/**
			* 분석을 실행 전 사용합니다.
			* 객체 해석시 가우시안 필터 사용여부를 설정합니다.
			*
			* param[in] bGaussianProcess : 설정 On/ Off
			*/
			void SetGaussianProcess(_In_ bool bGaussianProcess);
			
			/**
			* 설정된 객체 해석시 가우시안 필터 사용여부를 가져옵니다.
			*
			* return value : bool 형태로 반환합니다.
			*/
			_Out_ bool IsGaussianProcess(void);
			
			/**
			* 분석을 실행 전 사용합니다.
			* 코드의 위치를 정의하고 활성화 합니다.
			*
			* param[in] rqrDirectROI : CRavidQuadrangle 위치 설정
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult EnableDirectROI(_In_ CRavidQuadrangle<double>& rqrDirectROI);
			
			/**
			* 코드 위치 정의를 비활성화 합니다.
			*/
			void DisableDirectROI();
			
			/**
			* 분석을 실행 전 사용합니다.
			* 코드의 위치를 찾기 위한 이진화 방법을 설정합니다.
			*
			* param[in] nDMTM : 이진화 enum 값 (비트 연산된 값을 넣으면 개별 활성화)
			*	EDataMatrixThresholdMethodOption_Ostu = 0x1
			*	EDataMatrixThresholdMethodOption_Adaptive1 = 0x2
			*	EDataMatrixThresholdMethodOption_Adaptive2 = 0x4
			*	EDataMatrixThresholdMethodOption_ISODATA = 0x8
			*	EDataMatrixThresholdMethodOption_MaximumEntropy = 0x10
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetThresholdMethod(_In_ int nDMTM);
			
			/**
			* 설정된 코드의 위치를 찾기 위한 이진화 방법을 가져옵니다.
			*
			* return value : int 형태로 반환합니다.
			*/
			_Out_ int GetThresholdMethod();
			
			/**
			* 분석을 실행 전 사용합니다.
			* 입력된 이진화 방법을 활성화 합니다.
			*
			* param[in] eDMTMO : EDataMatrixThresholdMethodOption enum
			*	EDataMatrixThresholdMethodOption_Ostu = 0x1
			*	EDataMatrixThresholdMethodOption_Adaptive1 = 0x2
			*	EDataMatrixThresholdMethodOption_Adaptive2 = 0x4
			*	EDataMatrixThresholdMethodOption_ISODATA = 0x8
			*	EDataMatrixThresholdMethodOption_MaximumEntropy = 0x10
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult EnableThresholdMethod(_In_ EDataMatrixThresholdMethodOption eDMTMO);
			
			/**
			* 분석을 실행 전 사용합니다.
			* 입력된 이진화 방법을 비활성화 합니다.
			*
			* param[in] eDMTMO : EDataMatrixThresholdMethodOption enum
			*	EDataMatrixThresholdMethodOption_Ostu = 0x1
			*	EDataMatrixThresholdMethodOption_Adaptive1 = 0x2
			*	EDataMatrixThresholdMethodOption_Adaptive2 = 0x4
			*	EDataMatrixThresholdMethodOption_ISODATA = 0x8
			*	EDataMatrixThresholdMethodOption_MaximumEntropy = 0x10
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult DisableThresholdMethod(_In_ EDataMatrixThresholdMethodOption eDMTMO);
			
			/**
			* 해석 데이터를 초기화 합니다. 
			*/
			void Clear(void);
			
			/**
			* 분석을 실행 후 사용합니다.
			* 해석된 코드의 개수를 가져옵니다.
			*
			* return value : size_t 로 반환됩니다.
			*/
			size_t GetCount(void);
			
			/**
			* 분석을 실행 후 사용합니다.
			* 해석된 코드의 리스트를 가져옵니다.
			*
			* return value : SDataMatrixInformation 구조체의 벡터로 반환됩니다.
			*/
			std::vector<SDataMatrixInformation>* GetDataMatrixInformation();

		private:
			std::unique_ptr<CInternalDataMatrixDecoder> m_pInternal;
		};
	}
}
