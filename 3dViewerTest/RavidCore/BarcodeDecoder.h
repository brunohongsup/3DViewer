#pragma once

#include "RavidQuadrangle.h"

#include <vector>
#include <memory>

namespace Ravid
{
	namespace Algorithms
	{
		class CInternalRavidImage;

		class CInternalBarcodeDecoder;

		class CRavidImage;

		/**
		* 이미지에서 1차원 바코드를 분석하는 클래스입니다.
		*/
		class AFX_EXT_CLASS CBarcodeDecoder : public CRavidObject
		{
			RavidUseDynamicCreation();
			RavidPreventCopySelf(CBarcodeDecoder);

		public:
			/**
			* 바코드의 해석 방법입니다.
			*/
			enum E1DBarcodeDecoderType
			{
				E1DBarcodeDecoderType_Unknown = -1,	// 알려지지 않음
				E1DBarcodeDecoderType_EAN8,
				E1DBarcodeDecoderType_EAN13,
				E1DBarcodeDecoderType_EAN128,
				E1DBarcodeDecoderType_CODE11,
				E1DBarcodeDecoderType_CODE39,
				E1DBarcodeDecoderType_CODE128,
				E1DBarcodeDecoderType_MSI,
				E1DBarcodeDecoderType_UPCA,
				E1DBarcodeDecoderType_UPCE,
				E1DBarcodeDecoderType_Interleaved2Of5,
				E1DBarcodeDecoderType_Codabar
			};
			/**
			* 바코드의 해석 결과 구조체입니다.
			*/
			struct BarcodeInformation
			{
				bool bSuccess;
				CRavidQuadrangle<double> rqr;
				CString strSymbologyType;
				CString strResult;
			};

			CBarcodeDecoder();
			virtual ~CBarcodeDecoder();
			
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
			* 바코드 해석하는 방법을 설정합니다.
			*
			* param[in] eBarcodeType : 바코드 해석 방법
			* E1DBarcodeDecoderType_Unknown 은 모든 해석 방법을 적용해 봅니다.
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetType(_In_ E1DBarcodeDecoderType eBarcodeType);
			
			/**
			* 바코드 탐색시 이미지 픽셀 기준 최소 두께를 설정합니다.
			*
			* param[in] dblMinWidth : 30 이상의 값을 가집니다.
			*
			* return value : 실패하면 EAlgorithmResult_OK 이외의 값을 가집니다.
			*/
			EAlgorithmResult SetMinWidth(_In_ double dblMinWidth);
			
			/**
			* 현재 설정된 바코드 해석하는 방법을 가져옵니다.
			*
			* return value : enum E1DBarcodeDecoderType 의 타입을 반환합니다.
			*/
			E1DBarcodeDecoderType GetType(void);
			
			/**
			* 현재 설정된 바코드 탐색시 이미지 픽셀 기준 최소 두께를 가져옵니다.
			*
			* return value : 기본값 30으로 설정된 값을 반환합니다.
			*/
			double GetMinWidth(void);
			
			/**
			* 단일 탐색 및 해석한 결과를 가져옵니다.
			* 이미지에 바코드가 1개일 경우 혹은 다중 객체는 마지막 정보가 반환됩니다.
			*
			* return value : struct BarcodeInformation 구조체로 결과를 반환합니다.
			*/
			BarcodeInformation GetBarcodeInfomation(void);
			
			/**
			* 다중 탐색 및 해석한 결과를 가져옵니다.
			* 이미지에서 읽은 모든 바코드의 정보를 가져옵니다.
			*
			* return value : struct BarcodeInformation 구조체 리스트로 결과를 반환합니다.
			*/
			std::vector<std::pair<bool, CString>>* GetBarcodeResult(void);

		private:
			std::unique_ptr<CInternalBarcodeDecoder> m_pInternal;

		};
	}
}
